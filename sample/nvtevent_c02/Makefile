# output prefix
# e.g. $(MAIN_NAME).exe, lib$(MAIN_NAME).a  $(MAIN_NAME)
MAIN_NAME = nvtevent_c02

#source files
SRC = \
	./main.cpp

#don't edit following for every platform
ifeq "$(shell uname)" "Linux"
ifeq "$(NVT_CROSS)" ""
# ubuntu
include _ubuntu.mk
else
# NVT-LINUX
include _linux.mk
endif
else
# eCos
include _ecos.mk
endif
