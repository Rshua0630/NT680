
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = BT_Brcm
BTEDIR = bte

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
DEMO_HOST := \
		$(BTEDIR)/demo/host/wiced_bt_cfg.c \
		$(BTEDIR)/demo/host/bt_rfc_server.c \
		$(BTEDIR)/demo/host/rfc_sdp_db.c \
		$(BTEDIR)/demo/host/bt_le_reporter.c \
		$(BTEDIR)/demo/host/wiced_bt_rfcbte_gatt_db.c \
		$(BTEDIR)/demo/host/hoGP.c	\
		$(BTEDIR)/demo/host/bt_smart_service.c		\
		$(BTEDIR)/demo/host/upio_uITRON.c \
		$(BTEDIR)/Projects/bte/embedded/wiced/lib/fwpatches/BCM43438/brcm_patch-43438-26M.c

DEMO_PERIPHERAL := \
		$(BTEDIR)/demo/peripheral/wiced_bt_cfg.c \
		$(BTEDIR)/demo/peripheral/bt_rfc_server.c \
		$(BTEDIR)/demo/peripheral/rfc_sdp_db.c \
		$(BTEDIR)/demo/peripheral/bt_le_reporter.c \
		$(BTEDIR)/demo/peripheral/wiced_bt_rfcbte_gatt_db.c \
		$(BTEDIR)/demo/peripheral/hoGP.c	\
		$(BTEDIR)/demo/peripheral/bt_smart_service.c		\
		$(BTEDIR)/demo/peripheral/upio_uITRON.c \
		$(BTEDIR)/Projects/bte/embedded/wiced/lib/fwpatches/BCM43438/brcm_patch-43438-26M.c
	
BTE_OS_SOURCES := \
		$(BTEDIR)/Components/udrv/uITRON/userial_uITRON.c \
		$(BTEDIR)/Components/udrv/uITRON/utron_utils.c
		
DEMO_APP := $(DEMO_HOST)		

BRCM_BTE_SOURCES += \
		$(DEMO_APP) \
		$(BTE_OS_SOURCES) 		
		
SRC = \
		BT_Brcm.c \
		BT_BrcmID.c \
		$(BRCM_BTE_SOURCES)    

ASM =


#----------------------------------------------------------------------
# set the ROOT directory here
#----------------------------------------------------------------------
ROOT_DIR = ../../../..

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------

BTEINCLUDES := \
		$(BTEDIR) \
		$(BTEDIR)/Components/udrv/include \
		$(BTEDIR)/Components/stack/include_wiced \
		$(BTEDIR)/Projects/bte/main
		
		
INC_DIR = . \
    $(ROOT_DIR)/Include \
	  $(BTEINCLUDES)

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(ROOT_DIR)/ARC/Drv

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------

# C define for BTE
#----------------------------------------------------------------------	
BTEDEFINE = -DBUILDCFG=1 \
	-DBLUETOOTH_BTE \
	-DWICED_BTE_LIB \
	-DUSE_UART0_TRACE \
	-DBT_TRACE_VERBOSE=0 \
	-DBT_USE_TRACES=1  \
	-DBT_TRACE_PROTOCOL=0 \
	-DUSE_EXTERNAL_HCD \
	-DNOVATEK
#----------------------------------------------------------------------

CFLAGS_D = $(BTEDEFINE)
ASMFLAGS_D =

CFLAGS_R = $(BTEDEFINE)
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = $(ROOT_DIR)/MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
