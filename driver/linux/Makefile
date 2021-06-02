## Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
## raycore driver
## Date : 

#-------------------------------------------------
# Makefile for driver
#-------------------------------------------------
MODULE := raycore_pcie

BUILD_NUMBER_FILE =build_number_$(MODULE)
MAJOR = 1
MINOR = 0

SRCS := \
		driver.c \
		driver_raycore.c \
		$(BUILD_NUMBER_FILE).c
			
obj-m += $(MODULE).o
$(MODULE)-objs += $(SRCS:.c=.o)

#-------------------------------------------------
# kernel path
#-------------------------------------------------
KDIR	:=/lib/modules/$(shell uname -r)/build
#KDIR	:= ~/devel/nxp3200/kernel/linux-2.6.37

PWD	:= $(shell pwd)
all: UPDATE_BUILD_NUMBER
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	gcc -o driver_test driver_test.c
	
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f $(BUILD_NUMBER_FILE).c
	rm -f driver_test
	
UPDATE_BUILD_NUMBER :
	@echo "Update Build Number: $(MODULE) version $(MAJOR).$(MINOR) ($$(date))"
	@echo
	@echo "const char* __$(MODULE)_VERSION_STRING__ = \"$(MODULE) version $(MAJOR).$(MINOR) \";" > $(BUILD_NUMBER_FILE).c
