# $FreeBSD: projects/armv6/release/doc/ja_JP.eucJP/errata/Makefile 109707 2003-01-22 21:05:07Z hrs $
# Original revision: 1.5

RELN_ROOT?= ${.CURDIR}/../..
.ifdef NO_LANGCODE_IN_DESTDIR
DESTDIR?= ${DOCDIR}/errata
.else
DESTDIR?= ${DOCDIR}/ja_JP.eucJP/errata
.endif

DOC?= article
FORMATS?= html
INSTALL_COMPRESSED?= gz
INSTALL_ONLY_COMPRESSED?=

# SGML content
SRCS+=	article.sgml

.include "${RELN_ROOT}/share/mk/doc.relnotes.mk"
.include "${DOC_PREFIX}/share/mk/doc.project.mk"
