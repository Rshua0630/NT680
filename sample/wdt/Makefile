#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -Wl,-rpath-link=$(LIBRARY_DIR) -L$(LIBRARY_DIR)
#--------- END OF ENVIRONMENT SETTING -------------

TARGET = wdt_test.sh

install:
	@echo ">>>>>>>>>>>>>>>>>>> ${TARGET} $@ >>>>>>>>>>>>>>>>>>>"
	cp -avf ${TARGET} ${ROOTFS_DIR}/rootfs/usr/bin

clean:
	@if [ -f ${ROOTFS_DIR}/rootfs/usr/bin/${TARGET} ]; then rm ${ROOTFS_DIR}/rootfs/usr/bin/${TARGET}; fi
