/*-
 * Copyright (c) 2012 Oleksandr Tymoshenko <gonzo@freebsd.org>
 * Copyright (c) 2012 Damjan Marion <dmarion@freebsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/malloc.h>
#include <sys/rman.h>
#include <sys/timeet.h>
#include <sys/timetc.h>
#include <sys/watchdog.h>
#include <machine/bus.h>
#include <machine/cpu.h>
#include <machine/frame.h>
#include <machine/intr.h>

#include <dev/fdt/fdt_common.h>
#include <dev/ofw/openfirm.h>
#include <dev/ofw/ofw_bus.h>
#include <dev/ofw/ofw_bus_subr.h>

#include <machine/bus.h>
#include <machine/fdt.h>

#define	BCM2835_NUM_TIMERS	4
#define	DEFAULT_FREQUENCY	1000000

#define	SYSTIMER_CS	0x00
#define	SYSTIMER_CLO	0x04
#define	SYSTIMER_CHI	0x08
#define	SYSTIMER_C0	0x0C
#define	SYSTIMER_C1	0x10
#define	SYSTIMER_C2	0x14
#define	SYSTIMER_C3	0x18

struct systimer {
	int			index;
	bool			enabled;
	struct eventtimer	et;
};

struct brcm_systimer_softc {
	struct resource*	mem_res;
	struct resource*	irq_res[BCM2835_NUM_TIMERS];
	void*			intr_hl[BCM2835_NUM_TIMERS];
	uint32_t		sysclk_freq;
	bus_space_tag_t		bst;
	bus_space_handle_t	bsh;
	struct systimer		st[BCM2835_NUM_TIMERS];
};

static struct resource_spec brcm_systimer_irq_spec[] = {
	{ SYS_RES_IRQ,      0,  RF_ACTIVE },
	{ SYS_RES_IRQ,      1,  RF_ACTIVE },
	{ SYS_RES_IRQ,      2,  RF_ACTIVE },
	{ SYS_RES_IRQ,      3,  RF_ACTIVE },
	{ -1,               0,  0 }
};

static struct brcm_systimer_softc *brcm_systimer_sc = NULL;

/* Read/Write macros for Timer used as timecounter */
#define brcm_systimer_tc_read_4(reg)		\
	bus_space_read_4(brcm_systimer_sc->bst, \
		brcm_systimer_sc->bsh, reg)

#define brcm_systimer_tc_write_4(reg, val)	\
	bus_space_write_4(brcm_systimer_sc->bst, \
		brcm_systimer_sc->bsh, reg, val)

static unsigned brcm_systimer_tc_get_timecount(struct timecounter *);

static struct timecounter brcm_systimer_tc = {
	.tc_name           = "BCM2835 Timecouter",
	.tc_get_timecount  = brcm_systimer_tc_get_timecount,
	.tc_poll_pps       = NULL,
	.tc_counter_mask   = ~0u,
	.tc_frequency      = 0,
	.tc_quality        = 1000,
};

static unsigned
brcm_systimer_tc_get_timecount(struct timecounter *tc)
{
	return brcm_systimer_tc_read_4(SYSTIMER_CLO);
}

static int
brcm_systimer_start(struct eventtimer *et, struct bintime *first,
              struct bintime *period)
{
	struct systimer *st = et->et_priv;
	uint32_t clo;
	uint32_t count;

	if (first != NULL) {
		count = (st->et.et_frequency * (first->frac >> 32)) >> 32;
		if (first->sec != 0)
			count += st->et.et_frequency * first->sec;


		clo = brcm_systimer_tc_read_4(SYSTIMER_CLO);
		clo += count;
		brcm_systimer_tc_write_4(SYSTIMER_C0 + st->index*4, clo);

		st->enabled = 1;

		return (0);
	} 

	return (EINVAL);
}

static int
brcm_systimer_stop(struct eventtimer *et)
{
	struct systimer *st = et->et_priv;
	st->enabled = 0;

	return (0);
}

static int
brcm_systimer_intr(void *arg)
{
	struct systimer *st = (struct systimer *)arg;
	uint32_t status;

	if (st->enabled) {
		status = brcm_systimer_tc_read_4(SYSTIMER_CLO);
		if (status & (1 << st->index))
			if (st->et.et_active)
				st->et.et_event_cb(&st->et, st->et.et_arg);
	}
	
	return (FILTER_HANDLED);
}

static int
brcm_systimer_probe(device_t dev)
{
	struct	brcm_systimer_softc *sc;
	sc = (struct brcm_systimer_softc *)device_get_softc(dev);

	if (ofw_bus_is_compatible(dev, "broadcom,bcm2835-system-timer")) {
		device_set_desc(dev, "BCM2835 System Timer");
		return (BUS_PROBE_DEFAULT);
	}

	return (ENXIO);
}

static int
brcm_systimer_attach(device_t dev)
{
	struct brcm_systimer_softc *sc = device_get_softc(dev);
	int i;
	int err;
	int rid = 0;

	if (brcm_systimer_sc != NULL)
		return (EINVAL);

	sc->mem_res = bus_alloc_resource_any(dev, SYS_RES_MEMORY, &rid, RF_ACTIVE);
	if (sc->mem_res == NULL) {
		device_printf(dev, "could not allocate memory resource\n");
		return (ENXIO);
	}

	sc->bst = rman_get_bustag(sc->mem_res);
	sc->bsh = rman_get_bushandle(sc->mem_res);

	/* Request the IRQ resources */
	err = bus_alloc_resources(dev, brcm_systimer_irq_spec,
		sc->irq_res);
	if (err) {
		device_printf(dev, "Error: could not allocate irq resources\n");
		return (ENXIO);
	}

	/* TODO: get frequency from FDT */
	sc->sysclk_freq = DEFAULT_FREQUENCY;

	for (i = 0; i < BCM2835_NUM_TIMERS; i++) {
		/* Setup and enable the timer */
		if (bus_setup_intr(dev, sc->irq_res[i], INTR_TYPE_CLK,
				brcm_systimer_intr, NULL, &sc->st[i], &sc->intr_hl[i]) != 0) {
			bus_release_resources(dev, brcm_systimer_irq_spec,
				sc->irq_res);
			device_printf(dev, "Unable to setup the clock irq handler.\n");
			return (ENXIO);
		}

		sc->st[i].index = i;
		sc->st[i].enabled = 0;
		sc->st[i].et.et_name = malloc(64, M_DEVBUF, M_NOWAIT | M_ZERO);
		sprintf(sc->st[i].et.et_name, "BCM2835 Event Timer %d\n", i + 1);
		sc->st[i].et.et_flags = ET_FLAGS_ONESHOT;
		sc->st[i].et.et_quality = 1000;
		sc->st[i].et.et_frequency = sc->sysclk_freq;
		sc->st[i].et.et_min_period.sec = 0;
		sc->st[i].et.et_min_period.frac =
		    ((0x00000002LLU << 32) / sc->st[i].et.et_frequency) << 32;
		sc->st[i].et.et_max_period.sec = 0xfffffff0U / sc->st[i].et.et_frequency;
		sc->st[i].et.et_max_period.frac =
		    ((0xfffffffeLLU << 32) / sc->st[i].et.et_frequency) << 32;
		sc->st[i].et.et_start = brcm_systimer_start;
		sc->st[i].et.et_stop = brcm_systimer_stop;
		sc->st[i].et.et_priv = &sc->st[i];
		et_register(&sc->st[i].et);
	}

	brcm_systimer_sc = sc;

	return (0);
}

static device_method_t brcm_systimer_methods[] = {
	DEVMETHOD(device_probe,		brcm_systimer_probe),
	DEVMETHOD(device_attach,	brcm_systimer_attach),
	{ 0, 0 }
};

static driver_t brcm_systimer_driver = {
	"bcm2835_systimer",
	brcm_systimer_methods,
	sizeof(struct brcm_systimer_softc),
};

static devclass_t brcm_systimer_devclass;

DRIVER_MODULE(brcm_systimer, simplebus, brcm_systimer_driver, brcm_systimer_devclass, 0, 0);

void
cpu_initclocks(void)
{
}

void
DELAY(int usec)
{
	int32_t counts;
	uint32_t first, last;

	if (brcm_systimer_sc == NULL) {
		for (; usec > 0; usec--)
			for (counts = 200; counts > 0; counts--)
				/* Prevent gcc from optimizing  out the loop */
				cpufunc_nullop();
		return;
	}

	/* Get the number of times to count */
	counts = usec * ((brcm_systimer_tc.tc_frequency / 1000000) + 1);;

	first = brcm_systimer_tc_read_4(SYSTIMER_CLO);

	while (counts > 0) {
		last = brcm_systimer_tc_read_4(SYSTIMER_CLO);
		if (last>first) {
			counts -= (int32_t)(last - first);
		} else {
			counts -= (int32_t)((0xFFFFFFFF - first) + last);
		}
		first = last;
	}
}

