#	$FreeBSD: projects/armv6/usr.sbin/makefs/Makefile 223306 2011-06-19 18:34:49Z marcel $

PROG=	makefs

CFLAGS+=-I${.CURDIR}

SRCS=	cd9660.c ffs.c \
	getid.c \
	makefs.c \
	mtree.c \
	walk.c
MAN=	makefs.8

WARNS?=	2

.include "${.CURDIR}/cd9660/Makefile.inc"
.include "${.CURDIR}/ffs/Makefile.inc"
.include "${.CURDIR}/compat/Makefile.inc"

CFLAGS+=-DHAVE_STRUCT_STAT_ST_FLAGS=1
CFLAGS+=-DHAVE_STRUCT_STAT_ST_GEN=1

.PATH:	${.CURDIR}/../mtree
CFLAGS+=-I${.CURDIR}/../mtree
SRCS+=	misc.c spec.c

.PATH:	${.CURDIR}/../../sys/ufs/ffs
SRCS+=	ffs_tables.c

DPADD=  ${LIBSBUF}
LDADD=  -lsbuf

.include <bsd.prog.mk>
