## Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
## common build environment
## Date : 

ARCHNAME   	:= rc
KERNELNAME	:= linux-headers-$(shell uname -r)
#-------------------------------------------------
#	Toolchain.
#-------------------------------------------------
#CROSS		:=/opt/vasertile/arm-2011.09/bin/arm-none-linux-gnueabi-
CC 			:= $(CROSS)gcc
CXX			:= $(CROSS)g++
AR 			:= $(CROSS)ar
LD 			:= $(CROSS)ld
NM 			:= $(CROSS)nm
RANLIB 		:= $(CROSS)ranlib
OBJCOPY		:= $(CROSS)objcopy
STRIP		:= $(CROSS)strip


LBITS := $(shell getconf LONG_BIT)

ifdef CROSS
	OS_ARCH	:= linux_arm
else
	ifeq ($(LBITS),64)
		OS_ARCH	:= linux_x64
	else
		OS_ARCH	:= linux_x86
	endif

endif

#-------------------------------------------------
#	Library & Include
#-------------------------------------------------
#LIBDIR		:= -L$(CROSSDIR)/lib

LIBDIR		:= -L$(CONFIG_PATH)/../lib/$(OS_ARCH)
INCDIR		:=
OUTPUTPATH	:= $(CONFIG_PATH)/../../bin

#-------------------------------------------------
# 	Build flags.
#-------------------------------------------------
#OPTFLAGS	:= -w -O3 -mfloat-abi=softfp
OPTFLAGS	:= -w -O3
DBGFLAGS	:= # -g
CFLAGS		:= $(OPTFLAGS) $(DBGFLAGS) $(INCDIR)
ARFLAGS		:= crv
CDEFS		:= -D__int64="long long" -DGLES_FTYPE_IS_FLOAT -DGL_GLEXT_PROTOTYPES
DEP			:= $(CC)-M

ifdef CROSS
	CFLAGS:= $(CFLAGS) -DCROSS_COMPILE
endif
