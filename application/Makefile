include $(NVT_PRJCFG_MODEL_CFG)
include $(BUILD_DIR)/definitions.mk

APP_DIR ?= $(PWD)
APPSUBDIRS := $(subst /,,$(filter $(patsubst %,%/,$(NVT_CFG_APP)), $(dir $(wildcard */Makefile */makefile))))
APP_EXTLIST := $(shell cd $(APP_DIR)/external; make -s list)
LIBSUBDIRS := $(filter-out %external, $(subst /,,$(subst $(LIBRARY_DIR)/, , $(dir $(wildcard $(LIBRARY_DIR)/*/Makefile $(LIBRARY_DIR)/*/makefile)))))
LIB_EXTLIST := $(shell cd $(LIBRARY_DIR)/external; make -s list)
BUILD_LIST := $(LIB_EXTLIST:%=nvt@ext@lib@%) $(LIBSUBDIRS:%=nvt@lib@%) $(APP_EXTLIST:%=nvt@ext@app@%) $(APPSUBDIRS)

all:
	@$(call makedep, $(BUILD_LIST))

listpackage:
	@echo -e "\e[1;36mAvailable packages: \e[0m"
	@for n in $(BUILD_LIST); do \
		echo -e "\e[1;33m\t$$n \e[0m"; \
	done;
	@echo -e "\e[1;36mAdd \"DEP_LIBRARIES= package_name\" to your Makefile can support dependent build.\e[0m"

install:
	@$(call makelist, $(APPSUBDIRS), install)
	@cd $(APP_DIR)/external; make install;

clean:
	@$(call makelist, $(APPSUBDIRS), clean)
	@cd $(APP_DIR)/external; make clean;
