#----------------------------------------------------------------------
# set the ROOT directory here
#----------------------------------------------------------------------
ROOT_DIR = ../../../..

include $(ROOT_DIR)/Project/ProjectConfig.txt
include $(ROOT_DIR)/Project/$(ACTIVE_PROJECT)/ModelConfig.txt
include $(ROOT_DIR)/Project/$(ACTIVE_PROJECT)/ModelConfig_$(MODEL).txt
include $(ROOT_DIR)/Project/$(ACTIVE_PROJECT)/MakeConfig.txt

#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = IPL_CAL_SAMPLE_FF

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
	ipl_cal_ctrl_sample_ff.c \
	ipl_cal_cmd_sample_ff.c \
	ipl_cal_setting_sample_ff.c \

ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
    $(ROOT_DIR)/Include \
    ../../DevCamCAL \

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR =  $(ROOT_DIR)/ARC/Lib

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D = -D_IPL_COM_INT_INC \
			$(C_DEFINE_EXT)
ASMFLAGS_D =

CFLAGS_R = -D_IPL_COM_INT_INC \
			$(C_DEFINE_EXT)
ASMFLAGS_R =

GCCFLAGS = -D_IPL_COM_INT_INC \
			$(C_DEFINE_EXT)
#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR =  $(ROOT_DIR)/MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
