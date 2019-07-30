include $(NVT_PRJCFG_MODEL_CFG)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -fPIC -shared -L$(LIBRARY_DIR) -lnvtuctrl
#--------- END OF ENVIRONMENT SETTING -------------
DEP_LIBRARIES :=nvt@lib@nvtuctrl

LIB_NAME=libnvtisp
INC = nvtisp.h
INC_PROTECT = 
SRC = nvtisp.c nvtisp_common.c
HEADER = $(shell find . -name "*.h")

copy = if [ ! -z $(1) ]; then cp -avf $(1) $(2); fi

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
