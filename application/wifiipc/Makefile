include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING	        = -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS) -I./include -D__LINUX -D__LINUX_660 -D__USE_IPC
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -L$(LIBRARY_DIR) -Wl,-rpath-link=$(LIBRARY_DIR) -lpthread -lnvtipc
#--------- END OF ENVIRONMENT SETTING -------------
#--------- Compiling -------------------
BIN = wifiipc
SRC = src/nvtwifi_ipc.c src/nvtwifi.c src/adaptor.c
HEADER = $(shell find . -name "*.h")

OBJ = $(SRC:.c=.o)

ifeq ("$(wildcard *.c */*.c)","")
all:
	@echo ">>> Skip"
clean:
	@echo ">>> Skip"

else
all : $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LD_FLAGS)
	$(NM) -n $@ > $@.sym
	$(STRIP) $@
	$(OBJCOPY) -R .comment -R .note.ABI-tag -R .gnu.version $@

%.o:%.c $(HEADER)
	$(CC) $(C_FLAGS) $< -c -o $@

clean:
	rm -vf $(BIN) $(OBJ) $(BIN).sym *.o *.a *.so*

endif

install: 
	@cp -avf ${BIN} ${ROOTFS_DIR}/rootfs/bin/
	@mkdir -p ${ROOTFS_DIR}/rootfs/usr/share/wifiscripts
ifeq ($(NVT_SDIO_WIFI), $(filter $(NVT_SDIO_WIFI),NVT_SDIO_WIFI_BRCM))
ifeq ($(WIFI_BRCM_MDL), $(filter $(WIFI_BRCM_MDL),WIFI_BRCM_MDL_43438a1_ampk6212axtal26))
	@cp -avfr scripts/ap6212a/* ${ROOTFS_DIR}/rootfs/usr/share/wifiscripts/
else ifeq ($(WIFI_BRCM_MDL), $(filter $(WIFI_BRCM_MDL),WIFI_BRCM_MDL_43455c0_ampk6255c0))
	@cp -avfr scripts/ap6255/* ${ROOTFS_DIR}/rootfs/usr/share/wifiscripts/
endif
endif
ifeq ($(NVT_SDIO_WIFI), $(filter $(NVT_SDIO_WIFI),NVT_SDIO_WIFI_RTK))
	@cp -avfr scripts/8189es/* ${ROOTFS_DIR}/rootfs/usr/share/wifiscripts/
endif

ifeq ($(NVT_SDIO_WIFI), $(filter $(NVT_SDIO_WIFI),NVT_SDIO_WIFI_NVT))
ifeq ($(WIFI_NVT_MDL), $(filter $(WIFI_NVT_MDL),WIFI_NVT_MDL_18211))
	@cp -avfr scripts/nvt18211/* ${ROOTFS_DIR}/rootfs/usr/share/wifiscripts/
else ifeq ($(WIFI_NVT_MDL), $(filter $(WIFI_NVT_MDL),WIFI_NVT_MDL_18202))
	@cp -avfr scripts/nvt18202/* ${ROOTFS_DIR}/rootfs/usr/share/wifiscripts/
endif
endif
