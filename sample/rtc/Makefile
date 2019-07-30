#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING	= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -Wl,-rpath-link=$(LIBRARY_DIR) -L$(LIBRARY_DIR)
#--------- END OF ENVIRONMENT SETTING -------------

TARGET = rtc_test.sh
BIN = nvtrtc_test
SRC = nvtrtc_test.c

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

install:
	@echo ">>>>>>>>>>>>>>>>>>> ${TARGET} $@ >>>>>>>>>>>>>>>>>>>"
	cp -avf ${TARGET} ${BIN} ${ROOTFS_DIR}/rootfs/usr/bin

clean:
	@if [ -f ${ROOTFS_DIR}/rootfs/usr/bin/${TARGET} ]; then rm ${ROOTFS_DIR}/rootfs/usr/bin/${TARGET}; fi
	@rm -vf $(BIN) $(OBJ) $(BIN).sym *.o
