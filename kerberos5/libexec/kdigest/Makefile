# $FreeBSD: projects/armv6/kerberos5/libexec/kdigest/Makefile 233294 2012-03-22 08:48:42Z stas $

PROG=	kdigest
MAN=	kdigest.8
CFLAGS+=	-I${KRB5DIR}/lib/asn1 \
		-I${KRB5DIR}/lib/roken \
		-I${KRB5DIR}/lib/sl -I.
DPADD=	${LIBKRB5} ${LIBHEIMNTLM} ${LIBROKEN} ${LIBASN1} ${LIBCRYPTO} \
	${LIBCRYPT} ${LIBSL} ${LIBVERS} ${LIBEDIT}
LDADD=	-lkrb5 -lheimntlm -lroken -lasn1 -lcrypto -lcrypt ${LIBSL} \
	${LIBVERS} -ledit
SRCS=	kdigest.c \
	kdigest-commands.c \
	kdigest-commands.h

kdigest-commands.c kdigest-commands.h: kdigest-commands.in
	slc ${.ALLSRC:M*.in}

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/kuser
