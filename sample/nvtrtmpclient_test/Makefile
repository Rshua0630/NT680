#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING	        = -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -Os -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	=
CFLAGS		= $(PLATFORM_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -Wl,-rpath-link=$(LIBRARY_DIR) -L$(LIBRARY_DIR) -lnvtstreamsender -lnvtrtmpclient -lpushrtmp -lm -lpthread  -lcrypto -lssl -lz 
#--------- END OF ENVIRONMENT SETTING -------------

#--------- Compiling -------------------
BIN = nvtrtmpclient_test
SRC = nvtrtmpclient_test.c

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
	@rm -vf $(BIN) $(OBJ) $(BIN).sym *.o *.a *.so*

install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	@cp -avf ${BIN} ${ROOTFS_DIR}/rootfs/usr/bin
