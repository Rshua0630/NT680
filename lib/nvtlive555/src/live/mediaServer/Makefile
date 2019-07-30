INCLUDES = -I../UsageEnvironment/include -I../groupsock/include -I../liveMedia/include -I../BasicUsageEnvironment/include -I../mediaServer/include
# Default library filename suffixes for each library that we link with.  The "config.*" file might redefine these later.
libliveMedia_LIB_SUFFIX = $(LIB_SUFFIX)
libBasicUsageEnvironment_LIB_SUFFIX = $(LIB_SUFFIX)
libUsageEnvironment_LIB_SUFFIX = $(LIB_SUFFIX)
libgroupsock_LIB_SUFFIX = $(LIB_SUFFIX)
##### Change the following for your environment:
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

NAME = libMediaServer
MEDIA_SERVER_LIB = $(NAME).$(LIB_SUFFIX)

#PREFIX = /usr/local
PREFIX = ../../../release/nvtlive555
ALL = $(MEDIA_SERVER_LIB)
all: $(ALL)

.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<
.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

MEDIA_SERVER_OBJS = live555MediaServer.$(OBJ) DynamicRTSPServer.$(OBJ) StaticFrmData.$(OBJ) StaticFrm.$(OBJ) StreamSenderFrm.$(OBJ) main.$(OBJ) live555MediaClient.$(OBJ) DynamicRTSPClient.$(OBJ) FileFrm.$(OBJ) FileFrmData.$(OBJ)

live555MediaServer.$(CPP):	DynamicRTSPServer.hh version.hh
DynamicRTSPServer.$(CPP):	DynamicRTSPServer.hh
StaticFrmData.$(CPP):		DynamicRTSPServer.hh version.hh
StaticFrm.$(CPP):		DynamicRTSPServer.hh version.hh
StreamSenderFrm.$(CPP):		DynamicRTSPServer.hh version.hh
main.$(CPP):			DynamicRTSPServer.hh DynamicRTSPClient.hh version.hh
live555MediaClient.$(CPP):	DynamicRTSPClient.hh version.hh
DynamicRTSPClient.$(CPP):	DynamicRTSPClient.hh

USAGE_ENVIRONMENT_DIR = ../UsageEnvironment
USAGE_ENVIRONMENT_LIB = $(USAGE_ENVIRONMENT_DIR)/libUsageEnvironment.$(libUsageEnvironment_LIB_SUFFIX)
BASIC_USAGE_ENVIRONMENT_DIR = ../BasicUsageEnvironment
BASIC_USAGE_ENVIRONMENT_LIB = $(BASIC_USAGE_ENVIRONMENT_DIR)/libBasicUsageEnvironment.$(libBasicUsageEnvironment_LIB_SUFFIX)
LIVEMEDIA_DIR = ../liveMedia
LIVEMEDIA_LIB = $(LIVEMEDIA_DIR)/libliveMedia.$(libliveMedia_LIB_SUFFIX)
GROUPSOCK_DIR = ../groupsock
GROUPSOCK_LIB = $(GROUPSOCK_DIR)/libgroupsock.$(libgroupsock_LIB_SUFFIX)
#LOCAL_LIBS =	$(LIVEMEDIA_LIB) $(GROUPSOCK_LIB) \
#		$(BASIC_USAGE_ENVIRONMENT_LIB) $(USAGE_ENVIRONMENT_LIB)
#LIBS =			$(LOCAL_LIBS) $(LIBS_FOR_CONSOLE_APPLICATION)

#$(MEDIA_SERVER_LIB): $(MEDIA_SERVER_OBJS) $(LOCAL_LIBS)
#	$(LINK)$@ $(CONSOLE_LINK_OPTS) $(MEDIA_SERVER_OBJS) $(LIBS)
#ifneq "$(NM)" ""	
#	$(NM) -n $@ > $@.sym
#endif

$(MEDIA_SERVER_LIB): $(MEDIA_SERVER_OBJS) \
    $(PLATFORM_SPECIFIC_LIB_OBJS)
	$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) \
		$(MEDIA_SERVER_OBJS)

clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~ include/*~

install: $(MEDIA_SERVER_LIB)
	  install -d $(DESTDIR)$(PREFIX)/include $(DESTDIR)$(PREFIX)/lib	  
	  install -m 644 $(MEDIA_SERVER_LIB) $(DESTDIR)$(PREFIX)/lib
#	  install -d $(DESTDIR)$(PREFIX)/lib
#	  install -m 755 $(MEDIA_SERVER_LIB) $(DESTDIR)$(PREFIX)/lib

##### Any additional, platform-specific rules come here:
