export USE_SSL := OPENSSL
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING	= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
ifeq ($(USE_SSL),OPENSSL)
	EXT_CFLAGS = -DUSE_OPENSSL=1
else
	EXT_CFLAGS = -DUSE_WOLFSSL=1
endif
CFLAGS		= $(PLATFORM_CFLAGS) $(PRJCFG_CFLAGS) $(EXT_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
ifeq ($(USE_SSL),OPENSSL)
	LD_FLAGS	= -fPIC -shared -L$(LIBRARY_DIR) -lssl
else
	LD_FLAGS	= -fPIC -shared -L$(LIBRARY_DIR) -lwolfssl
endif
#--------- END OF ENVIRONMENT SETTING -------------

LIB_NAME=libnvtssl
INC = include/nvtssl.h
INC_PROTECT = 
SRC = src/nvtssl_wolfssl.c src/nvtssl_openssl.c
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
	$(AR) rcs $(LIB_NAME).a $(OBJ)

%.o: %.c $(HEADER)
	$(CC) $(C_FLAGS) -c $< -o $@

clean:
	rm -vf $(OBJ) *.o *.a *.so*

install_header:
	@$(call copy, $(INC), $(INCLUDE_DIR))
	@$(call copy, $(INC_PROTECT), $(INCLUDE_DIR)/protected)
endif

install: install_header
	@cp -avf *.so* *.a $(LIBRARY_DIR)
