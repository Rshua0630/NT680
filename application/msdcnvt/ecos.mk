# eCos makefile

# This is a generated file - do not edit

include ../../../../packages/pkgconf/ecos_supp.mak 

PACKAGE := nvt/msdcnvt/v2_0_60
OBJECT_PREFIX := msdcnvt
CFLAGS := $(ECOS_GLOBAL_CFLAGS) -D__ECOS -D__ECOS660
LDFLAGS := $(ECOS_GLOBAL_LDFLAGS)
VPATH := $(REPOSITORY)/$(PACKAGE)
INCLUDE_PATH := $(INCLUDE_PATH) -I$(PREFIX)/include $(foreach dir,$(VPATH),-I$(dir) -I$(dir)/include -I$(dir)/src -I$(dir)/tests) -I. -I$(REPOSITORY)/../ecos/include -I$(ECOS_PREFIX)/../ecos-nvtipc/include
MLT := $(wildcard $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.ldi $(REPOSITORY)/$(PACKAGE)/include/pkgconf/mlt*.h)
TESTS := 

build: headers libmsdcnvt.a.stamp

LIBRARY := libmsdcnvt.a
COMPILE := src/msdcnvt.c src/msdcnvt_uitron.c src/msdcnvt_ver.c
OBJECTS := $(COMPILE:.cxx=.o.d)
OBJECTS := $(OBJECTS:.cpp=.o.d)
OBJECTS := $(OBJECTS:.c=.o.d)
OBJECTS := $(OBJECTS:.S=.o.d)

$(LIBRARY).stamp: $(OBJECTS)
	@mkdir -p $(PREFIX)/lib
	$(AR) rcs $(PREFIX)/lib/$(@:.stamp=) $(foreach obj,$?,$(if $(obj:%.o=),$(dir $(obj))$(OBJECT_PREFIX)_$(notdir $(obj:.o.d=.o)),$(obj)))
	@cat $(foreach obj,$^,$(obj:.o=.o.d)) > $(@:.stamp=.deps)
	@touch $@

headers: mlt_headers $(PREFIX)/include/cyg/msdcnvt/msdcnvt.h

$(PREFIX)/include/cyg/msdcnvt/msdcnvt.h: $(REPOSITORY)/$(PACKAGE)/include/msdcnvt.h
	@mkdir -p $(dir $@)
	@cp $< $@
	@chmod u+w $@

include $(REPOSITORY)/pkgconf/rules_supp.mak

