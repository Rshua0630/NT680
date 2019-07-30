#--------- ENVIRONMENT SETTING --------------------
INCLUDES	= -I$(INCLUDE_DIR)
WARNING	= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS	= $(INCLUDES) -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
CFLAGS		= $(PLATFORM_CFLAGS)
C_FLAGS		= $(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS) $(WARNING)
LD_FLAGS	= -Wl,-rpath-link=$(LIBRARY_DIR) -L$(LIBRARY_DIR)
#--------- END OF ENVIRONMENT SETTING -------------

TARGET = suspend.sh resume.sh pm_resume_ethernet.sh pm_suspend_ethernet.sh pm_suspend_usb.sh pm_resume_usb.sh

.PHONY: all clean install

install:
	@echo ">>>>>>>>>>>>>>>>>>> ${TARGET} $@ >>>>>>>>>>>>>>>>>>>"
	cp -avf ${TARGET} ${ROOTFS_DIR}/rootfs/usr/bin

clean:
	@for n in ${TARGET}; do if [ -f ${ROOTFS_DIR}/rootfs/usr/bin/$$n ]; then rm ${ROOTFS_DIR}/rootfs/usr/bin/$$n; fi; done
