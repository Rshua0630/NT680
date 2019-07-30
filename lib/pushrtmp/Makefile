include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES        = -I$(INCLUDE_DIR)
WARNING         = -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS    = $(INCLUDES) -I. -I./librtmp/ -Os -fPIC -ffunction-sections -fdata-sections
CPPFLAGS        =
CFLAGS          = $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS)
C_FLAGS         = $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS        = -fPIC -lpthread -L$(LIBRARY_DIR)
#--------- END OF ENVIRONMENT SETTING -------------
DEP_LIBRARIES :=

#--------- Compiling --------------------
LIB_NAME=libpushrtmp
INC =
INC_PROTECT = libgmrtmp.h
SRC = libgmrtmp.c
HEADER = $(shell find . -name "*.h")

copy = if [ ! -z "$(1)" -a "$(1)" != " " ]; then cp -avf $(1) $(2); fi

OBJ = $(SRC:.c=.o)

SHELL = /bin/bash
MULTI_CORES = $(shell grep -c ^processor /proc/cpuinfo)

ifeq ("$(wildcard *.c */*.c)","")
all:
	@echo ">>> Skip"
clean:
	@echo ">>> Skip"
install_header:
	@echo ">>> Skip"
else

.PHONY: librtmp libpushrtmp
all: librtmp libpushrtmp

librtmp:
	@cd librtmp; make -j${MULTI_CORES} SYS=posix SHARED= INC=-I$(INCLUDE_DIR) XLDFLAGS=-L$(LIBRARY_DIR);

libpushrtmp: librtmp $(OBJ)
	$(AR) rcs $(LIB_NAME).a *.o librtmp/*.o

%.o:%.c $(HEADER)
	$(CC) $(C_FLAGS) $< -c

clean:
	@cd librtmp; make clean;
	rm -vf $(OBJ) *.o *.a *.so*

install_header:
	@$(call copy, $(INC), $(INCLUDE_DIR))
	@$(call copy, $(INC_PROTECT), $(INCLUDE_DIR)/protected)
endif

install: install_header
	@cp -avf *.a $(LIBRARY_DIR)
