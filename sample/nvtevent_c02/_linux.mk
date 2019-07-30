include $(NVT_PRJCFG_MODEL)
#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -I./include -D__LINUX660 -D__LINUX
C_FLAGS		= $(COMPILE_OPTS) $(WARNING)
CPPFLAGS	= $(C_FLAGS)
CFLAGS		= $(C_FLAGS) -Wstrict-prototypes
LD_FLAGS	= -L$(LIBRARY_DIR) -lnvtevent -lpthread -Wl,-rpath-link=$(LIBRARY_DIR) -lnvtipc -lnvtinfo
#--------- END OF ENVIRONMENT SETTING -------------

#--------- Compiling -------------------
# BINTYPE = APP or LIB
BIN_TYPE = APP

ifeq ($(BIN_TYPE),LIB)
EXPORT_HEADER = $(MAIN_NAME).h
endif

ifeq ($(BIN_TYPE),APP)
BIN = $(MAIN_NAME)
else ifeq ($(BIN_TYPE),LIB)
BIN = lib$(MAIN_NAME).a
endif

HEADER = $(shell find . -name "*.h")

OBJ = $(patsubst %.c,%.o,$(filter %.c, $(SRC))) $(patsubst %.cpp,%.o,$(filter %.cpp, $(SRC)))

.PHONY: all clean install

ifeq ("$(wildcard *.c */*.c *.cpp */*.cpp)","")
all:
	@echo ">>> Skip"
clean:
	@echo ">>> Skip"
else

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ)  $(LD_FLAGS)
	$(NM) -n $@ > $@.sym
	$(STRIP) $@
	$(OBJCOPY) -R .comment -R .note.ABI-tag -R .gnu.version $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp $(HEADER)
	$(CXX) $(CPPFLAGS) -c $< -o $@

ifeq ($(BIN_TYPE),APP)
clean:
	rm -vf $(BIN) $(OBJ_C) $(OBJ_CPP) $(BIN) *.sym *.o *.a *.so*
	rm -vf $(ROOTFS_DIR)/rootfs/bin/$(BIN)

install: $(BIN) 
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	@cp -af $(BIN) $(ROOTFS_DIR)/rootfs/bin/

else ifeq ($(BIN_TYPE),LIB)

clean:
	rm -vf $(BIN) $(OBJ_C) $(OBJ_CPP) $(BIN) *.sym *.o *.a *.so*
	rm -vf ./release/lib/$(BIN) 
ifneq "$(EXPORT_HEADER)" ""
	rm -rf ./release/include/$(EXPORT_HEADER)
endif
	
install: $(BIN) $(EXPORT_HEADER)
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	mkdir -p ./release/lib \
	&& cp -af $(BIN) ./release/lib 
ifneq "$(EXPORT_HEADER)" ""
	mkdir -p ./release/include \
	&& cp -af $(EXPORT_HEADER) ./release/include 
endif
endif
endif
