DEP_LIBRARIES :=nvt@lib@nvtipc nvt@lib@nvtinfo nvt@lib@nvtstreamsender

ifeq ("$(wildcard */*/*/*.c)","")
all:
	echo ">>> Skip"
clean:
	echo ">>> Skip"
install:
	@echo ">>>>>>>>>>>>>>>>>>> live555MediaServer >>>>>>>>>>>>>>>>>>>$(LIBRARY_DIR)"
	@cp -avf ./release/nvtlive555/lib/*.* $(LIBRARY_DIR)
else
all:
ifeq ($(findstring arm-ca53, $(NVT_CROSS)), arm-ca53)
	@echo live555 config to na51000-linux
	@cd src/live; sh genMakefiles na51000-linux; make
else ifeq ($(findstring mipsel-24kec, $(NVT_CROSS)), mipsel-24kec)
	@echo live555 config to na51023-linux
	@cd src/live; sh genMakefiles na51023-linux; make
else
	@echo live555 config to ubuntu
	@cd src/live; sh genMakefiles ubuntu; make
endif

clean:
ifeq ($(findstring arm-ca53, $(NVT_CROSS)), arm-ca53)
	@echo live555 config to na51000-linux
	@cd src/live; sh genMakefiles na51000-linux; make clean
else ifeq ($(findstring mipsel-24kec, $(NVT_CROSS)), mipsel-24kec)
	@echo live555 config to na51023-linux
	@cd src/live; sh genMakefiles na51023-linux; make clean
else
	@echo ubuntu
	@cd src/live; sh genMakefiles ubuntu; make clean
endif
	-rm -f src/live/BasicUsageEnvironment/Makefile
	-rm -f src/live/Makefile
	-rm -f src/live/UsageEnvironment/Makefile
	-rm -f src/live/groupsock/Makefile
	-rm -f src/live/liveMedia/Makefile
	-rm -f src/live/mediaServer/Makefile
	-rm -f src/live/proxyServer/Makefile
	-rm -f src/live/testProgs/Makefile

install:
	@echo ">>>>>>>>>>>>>>>>>>> live555MediaServer >>>>>>>>>>>>>>>>>>>$(LIBRARY_DIR)"
	@cd src/live; make install
endif

