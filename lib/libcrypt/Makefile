#
# $FreeBSD: projects/armv6/lib/libcrypt/Makefile 221142 2011-04-27 21:33:56Z obrien $
#

SHLIBDIR?=	/lib

.include <bsd.own.mk>

SHLIB_MAJOR=	5
LIB=		crypt

.PATH:		${.CURDIR}/../libmd
SRCS=		crypt.c misc.c \
		crypt-md5.c md5c.c \
		crypt-nthash.c md4c.c \
		crypt-sha256.c sha256c.c \
		crypt-sha512.c sha512c.c
MAN=		crypt.3
MLINKS=		crypt.3 crypt_get_format.3 crypt.3 crypt_set_format.3
CFLAGS+=	-I${.CURDIR}/../libmd -I${.CURDIR}/../libutil

# Pull in the strong crypto, if it is present.
.if exists(${.CURDIR}/../../secure/lib/libcrypt) && ${MK_CRYPT} != "no"
.PATH:		${.CURDIR}/../../secure/lib/libcrypt
SRCS+=		crypt-des.c crypt-blowfish.c blowfish.c
CFLAGS+=	-I${.CURDIR} -DHAS_DES -DHAS_BLOWFISH
.endif

# And the auth_getval() code and support.
.PATH:		${.CURDIR}/../libutil
SRCS+=		auth.c property.c
.for sym in auth_getval property_find properties_read properties_free \
	    MD4Init MD4Final MD4Update MD4Pad \
	    MD5Init MD5Final MD5Update MD5Pad \
	    SHA256_Init SHA256_Final SHA256_Update \
	    SHA512_Init SHA512_Final SHA512_Update
CFLAGS+=	-D${sym}=__${sym}
.endfor

WARNS?=		2

PRECIOUSLIB=

.include <bsd.lib.mk>
