# $FreeBSD: projects/armv6/kerberos5/usr.bin/kcc/Makefile 233294 2012-03-22 08:48:42Z stas $

PROG=	kcc
MAN=	klist.1 kswitch.1
LINKS=	${BINDIR}/kcc ${BINDIR}/klist \
	${BINDIR}/kcc ${BINDIR}/kswitch
CFLAGS+=	-I${KRB5DIR}/lib/hx509 \
		-I${KRB5DIR}/lib/asn1 \
		-I${KRB5DIR}/lib/roken \
		-I${KRB5DIR}/lib/sl -I.
DPADD=	${LIBKRB5} ${LIBHEIMNTLM} ${LIBROKEN} ${LIBASN1} ${LIBCRYPTO} \
	${LIBCRYPT} ${LIBKAFS5} ${LIBSL} ${LIBVERS} ${LIBEDIT}
LDADD=	-lkrb5 -lheimntlm -lroken -lasn1 -lcrypto -lcrypt -lkafs5 ${LIBSL} \
	${LIBVERS} -ledit
SRCS=	kcc.c \
	kcc-commands.c \
	kcc-commands.h \
	klist.c \
	kswitch.c \
	copy_cred_cache.c

kcc-commands.c kcc-commands.h: kcc-commands.in
	slc ${.ALLSRC:M*.in}

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/kuser
