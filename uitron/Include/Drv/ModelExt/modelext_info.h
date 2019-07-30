#ifndef _MODELEXT_INFO_H
#define _MODELEXT_INFO_H

#define MODELEXT_INFO_VER    0x16072219 ///< YYYY/MM/DD HH

typedef enum _MODELEXT_TYPE {
	MODELEXT_TYPE_DUMMY     = 0,
	MODELEXT_TYPE_INFO,
	MODELEXT_TYPE_BIN_INFO,
	MODELEXT_TYPE_PINMUX_CFG,
	MODELEXT_TYPE_INTDIR_CFG,
	MODELEXT_TYPE_EMB_PARTITION,
	MODELEXT_TYPE_GPIO_INFO,
	MODELEXT_TYPE_DRAM_PARTITION,
	MODELEXT_TYPE_MODEL_CFG,
	MODELEXT_TYPE_MAX
} MODELEXT_TYPE;

typedef struct _MODELEXT_HEADER {
	unsigned int size; ///< header size + container size
	unsigned int type; ///< MODELEXT_TYPE
	unsigned int number; ///< number of carried data elements
	unsigned int version; ///< data header version
} MODELEXT_HEADER;

typedef struct _MODELEXT_INFO {
	char name[8];       ///< always be 'M','O','D','E','L','E','X','T'
	char chip_name[8];  ///< CHIP_NAME (8) ---- check by encrypt_bin
	char version[8];    ///< Reversed
	char date[8];       ///< date
	unsigned int ext_bin_length;  ///< Bin File Length (4) --- write by encrypt_bin
	unsigned int check_sum;       ///< Check Sum Value (4) ----- write by encrypt_bin
} MODELEXT_INFO;

#endif
