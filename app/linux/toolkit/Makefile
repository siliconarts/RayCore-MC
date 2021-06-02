## Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
## makefiles
## Date : 

all:
ifndef CROSS
	$(MAKE) -f Makefile.ddi
endif
	$(MAKE) -f Makefile.hal
	$(MAKE) -f Makefile.ddk
	$(MAKE) -f Makefile.api
	
clean:
ifndef CROSS
	$(MAKE) -f Makefile.ddi			clean
endif
	$(MAKE) -f Makefile.hal			clean
	$(MAKE) -f Makefile.ddk			clean
	$(MAKE) -f Makefile.api 		clean
