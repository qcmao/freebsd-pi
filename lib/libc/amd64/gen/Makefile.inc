#	@(#)Makefile.inc	8.1 (Berkeley) 6/4/93
# $FreeBSD: projects/armv6/lib/libc/amd64/gen/Makefile.inc 232120 2012-02-24 18:39:55Z cognet $

SRCS+=	_setjmp.S _set_tp.c rfork_thread.S setjmp.S sigsetjmp.S \
	fabs.S getcontextx.c \
	infinity.c ldexp.c makecontext.c signalcontext.c \
	flt_rounds.c fpgetmask.c fpsetmask.c fpgetprec.c fpsetprec.c \
	fpgetround.c fpsetround.c fpgetsticky.c
