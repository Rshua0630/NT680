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
BIN = usockipc
SRC = src/usocket.c src/usocket_ipc.c src/usocketudp.c
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
