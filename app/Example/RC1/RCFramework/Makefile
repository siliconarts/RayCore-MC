## Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
## X11 based RayCore example framework
## Date : 

CONFIG_PATH		:= $(PWD)/../..
include ../../../lib/config.mk

SRC_PATH		:= ./src

#-------------------------------------------------
# 			library
#-------------------------------------------------
TARGETPATH		:= ../../../lib
TARGETNAME		:= RCFramework
TARGET_SLIB		:= $(TARGETNAME).a

TARGET_A		:= $(TARGETPATH)/$(TARGET_SLIB)

LIBDIR			:= $(LIBDIR) -lstdc++ -lpthread -lX11 -lm -lOpenRC 

#-------------------------------------------------
# 
#-------------------------------------------------
INC  := \
	-I$(SRC_PATH) \
	-I../../../include \
	-I../../../include/khronos \
	-I../../../include/siliconarts 

SRCS := \
	$(SRC_PATH)/RCFramework.cpp \
	$(SRC_PATH)/platform/X11Platform.cpp

OBJS := $(SRCS:.cpp=.o)

DEPS := $(OBJS:.o=.d)

CFLAGS:= $(CFLAGS) -DGL_GLEXT_PROTOTYPES -DGLES_FTYPE_IS_FLOAT
#-------------------------------------------------
# 
#-------------------------------------------------
all : $(TARGET_A)

static : $(TARGET_A) 

$(TARGET_A): $(DEPS) $(OBJS) 

clean:
	@$(RM) $(OBJS)
	@$(RM) $(TARGET_A)
	@$(RM) $(TARGET_SO)
	@$(RM) -f $(DEPS) $(DEPS:.d=.d.*)

#-------------------------------------------------
# generic rules
#-------------------------------------------------
%.o: %.c 
	@echo ' -Compiling...'
	$(CC) $(CDEFS) $(CFLAGS) $(INC) -c $< -o $@

%.o: %.cpp
	@echo ' -Compiling...'
	$(CXX) $(CDEFS) $(CFLAGS) -fPIC $(INC) -c $< -o $@

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $(INC) $< >> $@.$$$$; \
	sed 's#^[^.]*[.]o#$*.o#g' $@.$$$$ > $@; \
	rm -f $@.$$$$

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CFLAGS) $(INC) -fPIC $< >> $@.$$$$; \
	sed 's#^[^.]*[.]o#$*.o#g' $@.$$$$ > $@; \
	rm -f $@.$$$$

$(TARGET_A): 
	@echo
	@echo '*** Build Static Library ***'
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(RANLIB) $@
	rm -f $(TARGETPATH)/rc_*

#########################################################################
# Dependency
ifeq ($(DEPS),$(wildcard $(DEPS)))
include  $(DEPS)
endif

