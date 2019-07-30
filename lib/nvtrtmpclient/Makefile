include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	 = -I$(INCLUDE_DIR)
WARNING	     = -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS = $(INCLUDES) -I. -Os -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	 =
CFLAGS		 = $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS)
C_FLAGS 	 = $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	 = -fPIC -lpthread -L$(LIBRARY_DIR) -lpushrtmp -lnvtstreamsender
#--------- END OF ENVIRONMENT SETTING -------------
DEP_LIBRARIES :=nvt@lib@pushrtmp nvt@lib@nvtstreamsender

#--------- Compiling --------------------
LIB_NAME=libnvtrtmpclient
INC =nvtrtmpclient.h
INC_PROTECT =
SRC = nvtrtmpclient.c
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
	@cp -avf *.a $(LIBRARY_DIR)
