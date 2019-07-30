ECOS_GLOBAL_CFLAGS = -Wall -Wpointer-arith -Wstrict-prototypes -Wundef -Woverloaded-virtual -Wno-write-strings -mno-thumb-interwork -mcpu=cortex-a53 -mfloat-abi=softfp -mfpu=neon-fp-armv8 -g -O2 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fvtable-gc -fno-builtin-memcpy -D__ECOS__ -D__ECOS
ECOS_GLOBAL_LDFLAGS = -mno-thumb-interwork -mcpu=cortex-a53 -mfpu=neon -Wl,--gc-sections -Wl,-static -g -nostdlib
ECOS_COMMAND_PREFIX = /opt/arm/gcc-linaro-arm-none-eabi-4.9-2014.09_linux/bin/arm-none-eabi-
