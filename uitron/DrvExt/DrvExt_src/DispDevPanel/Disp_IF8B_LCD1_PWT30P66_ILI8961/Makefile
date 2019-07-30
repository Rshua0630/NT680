include ../PanelDef.txt

DISPLCDSEL_PARAMETER += -DDISPLCDSEL_IF8BITS_TYPE=DISPLCDSEL_IF8BITS_LCD1
DISPDEV_PARAMETER += -DDISPDEV_IF8BITS_TYPE=DISPDEV_IF8BITS_PWT30P66_ILI8961

#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = Disp_IF8B_LCD1_PWT30P66_ILI8961

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
    ../dispdev_IF8bits.c \
    T30P66_ILI8961.c

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
