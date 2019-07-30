ifdef CONFIG_LOADADDR
 LOADADDR   := $(shell perl -e 'printf "0x%08x", $(CONFIG_LOADADDR) +$(TEXT_OFFSET)')
else
 LOADADDR   := $(shell perl -e 'printf "0x%08x", 0x02800000 +$(TEXT_OFFSET)')
endif
