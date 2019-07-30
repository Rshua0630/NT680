include ../MakeCommon/MakeOption.txt

ifeq "$(shell uname)" "Linux"
MAKEFILE_SEARCH := $(sort $(dir $(shell find . -name 'Makefile')))
else
MAKEFILE_SEARCH := $(sort $(dir $(shell find . -name \'Makefile\')))
endif
mk_all := $(filter-out ./, $(MAKEFILE_SEARCH))

ifeq "$(ISOLATE_DEP)" "ON"
.PHONY: all $(mk_all) clean rebuild debug release dep

all:
	@make dep
	@make debug
	@make release
else
.PHONY: all $(mk_all) clean rebuild debug release

all: rm_log $(mk_all)
endif

$(mk_all):
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

rm_log:
ifeq "$(CLEAN_LOG)" "ON"
	@-rm -f log*.txt
endif

PRJ_NAME_ALL = $(shell find -name 'Makefile' -not -path "./Makefile" -print | xargs grep "PRJ_NAME" | sed 's/[[:digit:][:alpha:][:punct:]]*:PRJ_NAME[ ]*=[ ]*//g')
ifeq "$(USE_ECOS_KERNEL)" "ON"
LIB_R = $(addprefix "../ARC_eCos/Drv/Release/", $(addsuffix ".a", $(PRJ_NAME_ALL)))
LIB_D = $(addprefix "../ARC_eCos/Drv/Debug/", $(addsuffix "_D.a", $(PRJ_NAME_ALL)))
else
LIB_R = $(addprefix "../ARC/Drv/Release/", $(addsuffix ".a", $(PRJ_NAME_ALL)))
LIB_D = $(addprefix "../ARC/Drv/Debug/", $(addsuffix "_D.a", $(PRJ_NAME_ALL)))
endif

clean:
	@-echo Clean DrvExt ... \
	&& find ./ -type d -name '*_Data' | xargs rm -rf --no-preserve-root $(LIB_D) $(LIB_R) log*.txt lint*.txt

ifeq "$(ISOLATE_DEP)" "ON"
rebuild:
	@make clean
	@make dep
	@make debug
	@make release

dep: rm_log $(mk_all)
else
rebuild: rm_log $(mk_all)
endif

debug: rm_log $(mk_all)

release: rm_log $(mk_all)

rm_lint_log:
	@-rm -f lint*.txt

lint: rm_lint_log $(mk_all)

lintclean:
	@-echo Clean DrvExt lint data ... \
	&& find ./ -type d -name 'CheckData' | xargs rm -rf --no-preserve-root lint*.txt
