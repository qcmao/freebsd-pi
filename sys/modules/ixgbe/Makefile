#$FreeBSD: projects/armv6/sys/modules/ixgbe/Makefile 232120 2012-02-24 18:39:55Z cognet $
.PATH:  ${.CURDIR}/../../dev/ixgbe
KMOD    = ixgbe
SRCS    = device_if.h bus_if.h pci_if.h
SRCS    += ixgbe.c ixv.c
# Shared source
SRCS    += ixgbe_common.c ixgbe_api.c ixgbe_phy.c ixgbe_mbx.c ixgbe_vf.c
SRCS    += ixgbe_82599.c ixgbe_82598.c ixgbe_x540.c
CFLAGS+= -I${.CURDIR}/../../dev/ixgbe -DSMP -DIXGBE_FDIR

.include <bsd.kmod.mk>
