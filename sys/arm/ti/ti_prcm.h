/*
 * Copyright (c) 2010
 *	Ben Gray <ben.r.gray@gmail.com>.
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Ben Gray.
 * 4. The name of the company nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BEN GRAY ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BEN GRAY BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Texas Instruments - OMAP3xxx series processors
 *
 * Reference:
 *  OMAP35x Applications Processor
 *   Technical Reference Manual
 *  (omap35xx_techref.pdf)
 */
#ifndef _TI_PRCM_H_
#define _TI_PRCM_H_

typedef enum {

	/* System clocks, typically you can only call ti_prcm_clk_get_source_freq()
	 * on these clocks as they are enabled by default.
	 */
	SYS_CLK = 1,

	/* The MPU (ARM) core clock */
	MPU_CLK = 20,

	/* MMC modules */
	MMC0_CLK = 100,
	MMC1_CLK,
	MMC2_CLK,
	MMC3_CLK,
	MMC4_CLK,
	MMC5_CLK,

	/* I2C modules */
	I2C0_CLK = 200,
	I2C1_CLK,
	I2C2_CLK,
	I2C3_CLK,
	I2C4_CLK,

	/* USB module(s) */
	USBTLL_CLK = 300,
	USBHSHOST_CLK,
	USBFSHOST_CLK,
	USBP1_PHY_CLK,
	USBP2_PHY_CLK,
	USBP1_UTMI_CLK,
	USBP2_UTMI_CLK,
	USBP1_HSIC_CLK,
	USBP2_HSIC_CLK,

	/* UART modules */
	UART1_CLK = 400,
	UART2_CLK,
	UART3_CLK,
	UART4_CLK,

	/* General purpose timer modules */
	GPTIMER1_CLK = 500,
	GPTIMER2_CLK,
	GPTIMER3_CLK,
	GPTIMER4_CLK,
	GPTIMER5_CLK,
	GPTIMER6_CLK,
	GPTIMER7_CLK,
	GPTIMER8_CLK,
	GPTIMER9_CLK,
	GPTIMER10_CLK,
	GPTIMER11_CLK,
	GPTIMER12_CLK,

	/* McBSP module(s) */
	MCBSP1_CLK = 600,
	MCBSP2_CLK,
	MCBSP3_CLK,
	MCBSP4_CLK,
	MCBSP5_CLK,

	/* General purpose I/O modules */
	GPIO0_CLK = 700,
	GPIO1_CLK,
	GPIO2_CLK,
	GPIO3_CLK,
	GPIO4_CLK,
	GPIO5_CLK,
	GPIO6_CLK,

	/* sDMA module */
	SDMA_CLK = 800,

	/* DMTimer modules */
	DMTIMER0_CLK = 900,
	DMTIMER1_CLK,
	DMTIMER2_CLK,
	DMTIMER3_CLK,
	DMTIMER4_CLK,
	DMTIMER5_CLK,
	DMTIMER6_CLK,
	DMTIMER7_CLK,

	/* CPSW modules */
	CPSW_CLK = 1000,

	/* Mentor USB modules */
	MUSB0_CLK = 1100,

	/* EDMA module */
	EDMA_TPCC_CLK = 1200,
	EDMA_TPTC0_CLK,
	EDMA_TPTC1_CLK,
	EDMA_TPTC2_CLK,

	INVALID_CLK_IDENT

} clk_ident_t;

/*
 *
 */
typedef enum {
	SYSCLK_CLK,   /* System clock */
	EXT_CLK,

	F32KHZ_CLK,   /* 32KHz clock */
	F48MHZ_CLK,   /* 48MHz clock */
	F64MHZ_CLK,   /* 64MHz clock */
	F96MHZ_CLK,   /* 96MHz clock */

} clk_src_t;

struct ti_clock_dev {
	/* The profile of the timer */
	clk_ident_t  id;

	/* A bunch of callbacks associated with the clock device */
	int (*clk_activate)(struct ti_clock_dev *clkdev);
	int (*clk_deactivate)(struct ti_clock_dev *clkdev);
	int (*clk_set_source)(struct ti_clock_dev *clkdev,
	    clk_src_t clksrc);
	int (*clk_accessible)(struct ti_clock_dev *clkdev);
	int (*clk_get_source_freq)(struct ti_clock_dev *clkdev,
	    unsigned int *freq);
};

int ti_prcm_clk_valid(clk_ident_t clk);
int ti_prcm_clk_enable(clk_ident_t clk);
int ti_prcm_clk_disable(clk_ident_t clk);
int ti_prcm_clk_accessible(clk_ident_t clk);
int ti_prcm_clk_disable_autoidle(clk_ident_t clk);
int ti_prcm_clk_set_source(clk_ident_t clk, clk_src_t clksrc);
int ti_prcm_clk_get_source_freq(clk_ident_t clk, unsigned int *freq);
void ti_prcm_reset(void);

#endif   /* _TI_PRCM_H_ */
