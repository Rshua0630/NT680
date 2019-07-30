all:
	@echo ">>> Skip"
clean:
	@echo ">>> Skip"
install:
	@echo ">>>>>>>>>>>>>>>>>>> $@ >>>>>>>>>>>>>>>>>>>"
	@if [ ! -z $(CROSS_COMPILE) ]; then \
		$(eval uclibc := $(shell echo $(CROSS_COMPILE) | grep uclibc)) \
		if [ ! -z $(uclibc) ]; then \
			cp -avf uclibc/*.so* $(LIBRARY_DIR) 2>/dev/null; exit 0; \
		else \
			cp -avf glibc/*.so* $(LIBRARY_DIR) 2>/dev/null; exit 0; \
		fi \
	else \
		echo "\r\nERROR: Please source build/envsetup.sh in NT96660_Linux firstly to have auto copyso function\r\n"; \
	fi
