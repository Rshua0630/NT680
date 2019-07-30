##### Change the following for your environment:
ROOT_MAKEFILE = TRUE
COMPILE_OPTS = $(INCLUDES) -I. -I$(INCLUDE_DIR) -O2 -DSOCKLEN_T=socklen_t -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64 -fPIC $(PLATFORM_CFLAGS) -D__LINUX680 -D__LINUX -DXLOCALE_NOT_USED=1
C =			c
C_COMPILER =	$(CROSS_COMPILE)gcc
C_FLAGS =		$(COMPILE_OPTS) $(CPPFLAGS) $(CFLAGS)
CPP =			cpp
CPLUSPLUS_COMPILER =	$(CROSS_COMPILE)gcc 
CPLUSPLUS_FLAGS =	$(COMPILE_OPTS) -Wall -DBSD=1 $(CPPFLAGS) $(CXXFLAGS)
OBJ =			o
LINK =			$(CROSS_COMPILE)gcc 
LINK_OPTS = -L$(LIBRARY_DIR) -lstdc++ -lpthread -lm -lnvtlive555 -lnvtinfo -lnvtipc -lnvtstreamsender -lnvtuctrl
CONSOLE_LINK_OPTS =	$(LINK_OPTS)
LIBRARY_LINK =		$(CROSS_COMPILE)ar cr 
LIBRARY_LINK_OPTS =	
NM		  =     $(CROSS_COMPILE)nm
LIB_SUFFIX =			a
LIBS_FOR_CONSOLE_APPLICATION =
#_680_TODO -lnvtauth
LIBS_FOR_GUI_APPLICATION =
#EXE = 
EXE = .a
NM =
SHARED_LIB = ON
LD_FLAGS	= -fPIC -shared -L$(LIBRARY_DIR)
##### End of variables to change

LIVEMEDIA_DIR = liveMedia
GROUPSOCK_DIR = groupsock
USAGE_ENVIRONMENT_DIR = UsageEnvironment
BASIC_USAGE_ENVIRONMENT_DIR = BasicUsageEnvironment

MEDIA_SERVER_DIR = mediaServer

all:
	cd $(LIVEMEDIA_DIR) ; $(MAKE)
	cd $(GROUPSOCK_DIR) ; $(MAKE)
	cd $(USAGE_ENVIRONMENT_DIR) ; $(MAKE)
	cd $(BASIC_USAGE_ENVIRONMENT_DIR) ; $(MAKE)
	cd $(MEDIA_SERVER_DIR) ; $(MAKE)

install:
	cd $(LIVEMEDIA_DIR) ; $(MAKE) install
	cd $(GROUPSOCK_DIR) ; $(MAKE) install
	cd $(USAGE_ENVIRONMENT_DIR) ; $(MAKE) install
	cd $(BASIC_USAGE_ENVIRONMENT_DIR) ; $(MAKE) install
	cd $(MEDIA_SERVER_DIR) ; $(MAKE) install
	$(CROSS_COMPILE)ar -x ../../release/nvtlive555/lib/libMediaServer.a 
	$(CROSS_COMPILE)ar -x ../../release/nvtlive555/lib/libBasicUsageEnvironment.a 
	$(CROSS_COMPILE)ar -x ../../release/nvtlive555/lib/libgroupsock.a 
	$(CROSS_COMPILE)ar -x ../../release/nvtlive555/lib/libliveMedia.a 
	$(CROSS_COMPILE)ar -x ../../release/nvtlive555/lib/libUsageEnvironment.a
ifeq ($(SHARED_LIB), ON)
	echo share lib	
	$(CC) -o ../../release/nvtlive555/lib/libnvtlive555.so *.o $(LD_FLAGS)
	$(CROSS_COMPILE)ar rcs ../../release/nvtlive555/lib/libnvtlive555.a *.o
else
	echo non share lib	
	$(CROSS_COMPILE)ar -cru ../../release/nvtlive555/lib/libnvtlive555.a *.o	
endif
	@cp -t ../../release/nvtlive555/include mediaServer/include/nvtlive555.h
	-rm -f ../../release/nvtlive555/lib/libMediaServer.a 
	-rm -f ../../release/nvtlive555/lib/libBasicUsageEnvironment.a 
	-rm -f ../../release/nvtlive555/lib/libgroupsock.a 
	-rm -f ../../release/nvtlive555/lib/libliveMedia.a 
	-rm -f ../../release/nvtlive555/lib/libUsageEnvironment.a
	-rm -f *.o
	@cp -avf ../../release/nvtlive555/lib/*.* $(LIBRARY_DIR)	
	@cp -t $(INCLUDE_DIR) ../../release/nvtlive555/include/*.h

clean:
	cd $(LIVEMEDIA_DIR) ; $(MAKE) clean
	cd $(GROUPSOCK_DIR) ; $(MAKE) clean
	cd $(USAGE_ENVIRONMENT_DIR) ; $(MAKE) clean
	cd $(BASIC_USAGE_ENVIRONMENT_DIR) ; $(MAKE) clean
	cd $(MEDIA_SERVER_DIR) ; $(MAKE) clean
	-rm -rf ../../release
	-rm -f *.o

distclean: clean
	-rm -f $(LIVEMEDIA_DIR)/Makefile $(GROUPSOCK_DIR)/Makefile \
	  $(USAGE_ENVIRONMENT_DIR)/Makefile $(BASIC_USAGE_ENVIRONMENT_DIR)/Makefile \
	  $(MEDIA_SERVER_DIR)/Makefile $(PROXY_SERVER_DIR)/Makefile Makefile
