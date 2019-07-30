include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections -D$(NVT_DEFAULT_NETWORK_BOOT_PROTOCOL) -D$(NVT_DEFAULT_CVBS_TYPE)
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -fPIC -shared -L$(LIBRARY_DIR) -lnvtpstore -lcrypto -lrt
#--------- END OF ENVIRONMENT SETTING -------------
DEP_LIBRARIES := nvt@lib@nvtpstore

LIB_NAME=libnvtdevinfo
INC = nvtdevinfo.h nvtdevinfo_ctrl.h
INC_PROTECT = 
SRC = nvtdevinfo_cfg.c nvtdevinfo_defcfg.c nvtdevinfo_status.c nvtdevinfo_ctrl.c
HEADER = $(shell find . -name "*.h")

copy = if [ ! -z "$(1)" -a "$(1)" != " " ]; then cp -avf $(1) $(2); fi

OBJ = $(SRC:.c=.o)

ifeq ("$(wildcard *.c */*.c)","")
all:
	@echo ">>> Skip"
clean:
	@echo ">>> Skip"
install_header:
	@echo ">>> Skip"
else
all : $(OBJ)
	$(CC) -o $(LIB_NAME).so $(OBJ) $(LD_FLAGS)
	$(AR) rcs $(LIB_NAME).a *.o

%.o:%.c $(HEADER)
	$(CC) $(C_FLAGS) $< -c

clean:
	rm -vf $(OBJ) *.o *.a *.so*

install_header:
	@$(call copy, $(INC), $(INCLUDE_DIR))
	@$(call copy, $(INC_PROTECT), $(INCLUDE_DIR)/protected)
endif

install: install_header
	@cp -avf *.so* *.a $(LIBRARY_DIR)
