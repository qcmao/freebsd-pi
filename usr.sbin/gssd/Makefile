# $FreeBSD: projects/armv6/usr.sbin/gssd/Makefile 232120 2012-02-24 18:39:55Z cognet $

PROG=	gssd
MAN=	gssd.8
SRCS=	gssd.c gssd.h gssd_svc.c gssd_xdr.c gssd_prot.c

CFLAGS+= -I.
WARNS?= 1

DPADD=	${LIBGSSAPI}
LDADD=	-lgssapi

CLEANFILES= gssd_svc.c gssd.h

RPCSRC=	${.CURDIR}/../../sys/kgssapi/gssd.x
RPCGEN= RPCGEN_CPP=${CPP:Q} rpcgen -L -C -M

gssd_svc.c: ${RPCSRC} gssd.h
	${RPCGEN} -m -o ${.TARGET} ${RPCSRC}

gssd_xdr.c: ${RPCSRC} gssd.h
	${RPCGEN} -c -o ${.TARGET} ${RPCSRC}

gssd.h: ${RPCSRC}
	${RPCGEN} -h -o ${.TARGET} ${RPCSRC}

.PATH:	${.CURDIR}/../../sys/kgssapi

.include <bsd.prog.mk>
