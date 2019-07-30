include ../PanelDef.txt

DISPLCDSEL_PARAMETER += -DDISPLCDSEL_IFMI_TYPE=DISPLCDSEL_IFMI_LCD1
DISPDEV_PARAMETER += -DDISPDEV_IFMI_TYPE=DISPDEV_IFMI_LTM018G149_8BIT

#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = Disp_IFMI_LCD1_LTM018G149_8BIT

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
    ../dispdev_IFmi.c \
    LTM018G149_8BIT.c

ASM =

#----------------------------------------------------------------------
# set the ROOT directory here
#----------------------------------------------------------------------
ROOT_DIR = ../../../..

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = .. \
    $(ROOT_DIR)/Include

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(ROOT_DIR)/ARC/Drv

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D += $(DISPDEV_PARAMETER) \
            $(DISPLCDSEL_PARAMETER)
ASMFLAGS_D =

CFLAGS_R += $(DISPDEV_PARAMETER) \
            $(DISPLCDSEL_PARAMETER)
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = $(ROOT_DIR)/MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
