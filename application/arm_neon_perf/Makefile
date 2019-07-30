include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -fPIC -ffunction-sections -fdata-sections -g
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -L$(LIBRARY_DIR) -lnvtlibc
#--------- END OF ENVIRONMENT SETTING -------------
DEP_LIBRARIES := 
#--------- Compiling -------------------
BIN = nvt_neon_fir_filter
SRC = nvt_neon_fir_filter.c  fir_filter_intrinsics.c
BIN2 = nvt_neon_array_shift_add
SRC2 = nvt_neon_array_shift_add.c
BIN3 = nvt_neon_array_add
SRC3 = neon_basic_array_test/nvt_neon_array_add.c
BIN4 = nvt_neon_array_mul
SRC4 = neon_basic_array_test/nvt_neon_array_mul.c
BIN5 = nvt_neon_array_sub
SRC5 = neon_basic_array_test/nvt_neon_array_sub.c

OBJ = $(SRC:.c=.o)
OBJ2 = $(SRC2:.c=.o)
OBJ3 = $(SRC3:.c=.o)
OBJ4 = $(SRC4:.c=.o)
OBJ5 = $(SRC5:.c=.o)

.PHONY: all clean install

ifeq ("$(wildcard *.c */*.c)","")
all:
	@echo ">>> Skip"
clean:
	@echo ">>> Skip"
else

all: $(BIN) $(BIN2) $(BIN3) $(BIN4) $(BIN5)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LD_FLAGS)

$(BIN2): $(OBJ2)
	$(CC) -o $@ $(OBJ2) $(LD_FLAGS)

$(BIN3): $(OBJ3)
	$(CC) -o $@ $(OBJ3) $(LD_FLAGS)

$(BIN4): $(OBJ4)
	$(CC) -o $@ $(OBJ4) $(LD_FLAGS)

$(BIN5): $(OBJ5)
	$(CC) -o $@ $(OBJ5) $(LD_FLAGS)

%.o:%.c
	$(CC) $(C_FLAGS) -c $< -o $@

clean:
	rm -vf $(BIN) $(OBJ) $(BIN).sym *.o *.a *.so*
	rm -vf $(BIN2) $(OBJ2) $(BIN2).sym *.o *.a *.so*
	rm -vf $(BIN3) $(OBJ3) $(BIN3).sym *.o *.a *.so*
	rm -vf $(BIN4) $(OBJ4) $(BIN4).sym *.o *.a *.so*
	rm -vf $(BIN5) $(OBJ5) $(BIN5).sym *.o *.a *.so*

endif
install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	@cp -af $(BIN) $(ROOTFS_DIR)/rootfs/bin/
	@cp -af $(BIN2) $(ROOTFS_DIR)/rootfs/bin/
	@cp -af $(BIN3) $(ROOTFS_DIR)/rootfs/bin/
	@cp -af $(BIN4) $(ROOTFS_DIR)/rootfs/bin/
	@cp -af $(BIN5) $(ROOTFS_DIR)/rootfs/bin/
