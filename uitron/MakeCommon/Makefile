include MakeOption.txt

module_Common := $(dir $(wildcard ../Common/Makefile))
module_Drv := $(dir $(wildcard ../Drv/Makefile))
module_DrvExt := $(dir $(wildcard ../DrvExt/Makefile))
module_Lib := $(dir $(wildcard ../Lib/Makefile))
module_LibExt := $(dir $(wildcard ../LibExt/Makefile))
module_App := $(dir $(wildcard ../App/Makefile))
module_AppExt := $(dir $(wildcard ../AppExt/Makefile))
module_Alg := $(dir $(wildcard ../Alg/Makefile))
module_dal := $(dir $(wildcard ../dal/Makefile))

-include ../Project/ProjectConfig.txt

ifneq "$(ACTIVE_PROJECT)" ""
module_Prj := ../Project/$(ACTIVE_PROJECT)/
else
# Find the first folder in ../Project/ that contains "Makefile"
ifeq "$(shell uname)" "Linux"
module_Prj := $(dir $(firstword $(sort $(shell find ../Project -name 'Makefile'))))
else
module_Prj := $(dir $(firstword $(sort $(shell find ../Project -name \'Makefile\'))))
endif
endif
test_if_unix_file_fmt = $(shell cd $(1); ret=`if [ "$(OS)" == "Linux" ]; then tools/recursive_dir -t . --exclude-ext .o .a .bin .axf .7z --exclude-dir .svn .git; else tools/recursive_dir.exe -t . --exclude-ext .o .a .bin .axf .7z --exclude-dir .svn .git; fi`; echo "$$ret";)

module_all := \
	$(module_Common) \
	$(module_Drv) \
	$(module_DrvExt) \
	$(module_Lib) \
	$(module_LibExt) \
	$(module_App) \
	$(module_AppExt) \
	$(module_Alg) \
	$(module_dal)

ifeq "$(LOG_ERR)" "ON"
define copy_log
	@-cp \
	$(module_Common)log*.txt \
	$(module_Drv)log*.txt \
	$(module_DrvExt)log*.txt \
	$(module_Lib)log*.txt \
	$(module_LibExt)log*.txt \
	$(module_App)log*.txt \
	$(module_AppExt)log*.txt \
	$(module_Alg)log*.txt \
	$(module_dal)log*.txt \
	$(module_Prj)log*.txt \
	.
endef
else
define copy_log
endef
endif

ifeq "$(ISOLATE_DEP)" "ON"
.PHONY: all rm_log $(module_all) clean rebuild debug release dep

all: rm_log
	@make dep
	@make debug
	@make release
	$(call copy_log)
else
.PHONY: all rm_log $(module_all) clean rebuild debug release

all: rm_log $(module_all)
	$(call copy_log)
endif

$(module_all):
ifeq ($(OS),Linux)
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)
else
	@$(MAKE) --directory=$@ $(MAKECMDGOALS) $(JOB_FLAGS)
endif

$(module_Prj):$(module_all)
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

rm_log:
	@-rm -f log*.txt

clean: rm_log $(module_all) $(module_Prj)

ifeq "$(ISOLATE_DEP)" "ON"
rebuild:
	@make clean
	@make dep
	@make debug
	@make release
	$(call copy_log)

dep: $(module_all)
else
rebuild: rm_log $(module_all) $(module_Prj)
	$(call copy_log)
endif

debug: rm_log $(module_Prj)
	$(call copy_log)

release: rm_log $(module_Prj)
	$(call copy_log)

rm_chk_log:
	@-rm -f chk*.txt

chk: rm_chk_log $(module_all) $(module_Prj)

chkclean: $(module_all) $(module_Prj)

rm_lint_log:
	@-rm -f lint*.txt

lint: rm_lint_log $(module_all) $(module_Prj)

lintclean: $(module_all) $(module_Prj)

chk_unix:
	@result="$(call test_if_unix_file_fmt, ../)"; if [ ! -z "$$result" ]; then \
		echo -e "\e[1;31mUNIX file format check failed:\e[0m" >&2; \
		for n in $$result; do \
			echo -e "\t$$n" >&2; \
		done; \
		exit 1; \
	fi