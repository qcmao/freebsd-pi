# $FreeBSD: projects/armv6/kerberos5/libexec/digest-service/Makefile 233294 2012-03-22 08:48:42Z stas $

PROG=	digest-service
NO_MAN=	1
CFLAGS+=	-I${KRB5DIR}/kdc \
		-I${KRB5DIR}/lib/asn1 \
		-I${KRB5DIR}/lib/krb5 \
		-I${KRB5DIR}/lib/ipc \
		-I${KRB5DIR}/lib/wind \
		-I${KRB5DIR}/lib/roken
DPADD=	${LIBHDB} ${LIBKDC} ${LIBHEIMIPCS} ${LIBKRB5} ${LIBROKEN} ${LIBASN1} ${LIBCRYPTO} \
	${LIBCRYPT} ${LIBVERS}
LDADD=	-lhdb -lkdc ${LIBHEIMIPCS} -lkrb5 -lroken -lasn1 -lcrypto -lcrypt \
	${LIBVERS}

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/kdc
