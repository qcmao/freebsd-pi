# $FreeBSD: projects/armv6/sys/boot/i386/cdboot/Makefile 208789 2010-06-03 17:42:32Z ed $

PROG=	cdboot
STRIP=
BINMODE=${NOBINMODE}
NO_MAN=
SRCS=	${PROG}.s

ORG=	0x7c00

LDFLAGS=-e start -Ttext ${ORG} -Wl,-N,-S,--oformat,binary

.include <bsd.prog.mk>
