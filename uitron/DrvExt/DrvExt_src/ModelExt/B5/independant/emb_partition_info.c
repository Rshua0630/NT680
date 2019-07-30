#include "modelext_info.h"
#include "emb_partition_info.h"

#define BLK_UNIT(x) ((((x)%_EMBMEM_BLK_SIZE_)==0)?((x)/_EMBMEM_BLK_SIZE_):1/0) //block alignment
//#define BLK_UNIT(x) ((((x)%_EMBMEM_BLK_SIZE_)==0)?(x):1/0) //byte alignment

EMB_PARTITION ind_emb_partition_info_data[EMB_PARTITION_INFO_COUNT] __attribute__((section("modelext_data.emb_partition_info"))) = {
	[0] = {EMBTYPE_LOADER,  0,  BLK_UNIT(0x00000000), BLK_UNIT(0x00040000), BLK_UNIT(0x00000000)},
	[1] = {EMBTYPE_MODELEXT, 0,  BLK_UNIT(0x00040000), BLK_UNIT(0x00040000), BLK_UNIT(0x00000000)},
	[2] = {EMBTYPE_UITRON,  0,  BLK_UNIT(0x00080000), BLK_UNIT(0x00880000), BLK_UNIT(0x00000000)},
	[3] = {EMBTYPE_UBOOT,   0,  BLK_UNIT(0x00900000), BLK_UNIT(0x00200000), BLK_UNIT(0x00000000)},
	[4] = {EMBTYPE_UENV,    0,  BLK_UNIT(0x00B00000), BLK_UNIT(0x00040000), BLK_UNIT(0x00000000)},
	[5] = {EMBTYPE_LINUX,   0,  BLK_UNIT(0x00B40000), BLK_UNIT(0x00400000), BLK_UNIT(0x00000000)},
	[6] = {EMBTYPE_DSP,     0,  BLK_UNIT(0x00F40000), BLK_UNIT(0x00500000), BLK_UNIT(0x00000000)},
	[7] = {EMBTYPE_DSP,     1,  BLK_UNIT(0x01440000), BLK_UNIT(0x00800000), BLK_UNIT(0x00000000)},
	[8] = {EMBTYPE_PSTORE,  0,  BLK_UNIT(0x01C40000), BLK_UNIT(0x00400000), BLK_UNIT(0x00000000)},
	[9] = {EMBTYPE_ROOTFS,  0,  BLK_UNIT(0x02040000), BLK_UNIT(0x057C0000), BLK_UNIT(0x00000000)},
	[10] = {EMBTYPE_FAT,     0,  BLK_UNIT(0x07800000), BLK_UNIT(0x00800000), BLK_UNIT(0x00000000)},
};

MODELEXT_HEADER ind_emb_partition_info_header __attribute__((section("modelext_header.emb_partition_info"))) = {
	.size = sizeof(ind_emb_partition_info_data) + sizeof(MODELEXT_HEADER),
	.type = MODELEXT_TYPE_EMB_PARTITION,
	.number = sizeof(ind_emb_partition_info_data) / sizeof(EMB_PARTITION),
	.version = EMB_PARTITION_INFO_VER,
};
