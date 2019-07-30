#--------- ENVIRONMENT SETTING --------------------
DEP_LIBRARIES := nvt@ext@lib@zbar
INCLUDES	= -I$(INCLUDE_DIR) -I../lib/include -I$(INCLUDE_DIR)/freetype2
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS) 
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -L$(LIBRARY_DIR) -Wl,-rpath-link=$(LIBRARY_DIR) -lzbar -lnvtuctrl
#--------- END OF ENVIRONMENT SETTING -------------

#--------- Compiling -------------------
BIN = nvtqrcode_test
SRC = nvtqrcode_test.c

OBJ = $(SRC:.c=.o)

.PHONY: all clean install

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LD_FLAGS)
	$(NM) -n $@ > $@.sym
	$(STRIP) $@
	$(OBJCOPY) -R .comment -R .note.ABI-tag -R .gnu.version $@

%.o: %.c
	$(CC) $(C_FLAGS) -c $< -o $@

clean:
	rm -vf $(BIN) $(OBJ) $(BIN).sym *.o *.a *.so*

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	cp -avf ${BIN} ${ROOTFS_DIR}/rootfs/usr/bin
