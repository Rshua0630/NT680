#ifndef _DRAM_PARTITION_INFO_H
#define _DRAM_PARTITION_INFO_H

#if defined(__UITRON)
#include "Type.h"
#else
#include "nvt_type.h"
#endif

/**
    DRAM Partition

    This is common header used between firmware of uITRON, eCos, Linux, DSP
    so !!!!!! DO NOT modify it !!!!!!
*/

#define DRAM_PARTITION_INFO_VER 0x17032309 ///< YYYY/MM/DD HH

typedef struct _DRAM_PARTITION {
	unsigned int fourcc;    ///< FourCC DRAM
	unsigned int info_size; ///< sizeof(CORE_INFO)
	unsigned int dram_addr; ///< whole dram starting address
	unsigned int dram_size; ///< whole dram size
	unsigned int rev_addr;  ///< reversed starting address
	unsigned int rev_size;  ///< reversed size
	unsigned int ipc_addr;  ///< ipc starting address
	unsigned int ipc_size;  ///< ipc size
	unsigned int linux_addr;///< linux-kernel starting address
	unsigned int linux_size;///< linux-kernel size
	unsigned int uboot_addr;///< u-boot starting address
	unsigned int uboot_size;///< u-boot size
	unsigned int uitron_addr;///< uITRON starting address
	unsigned int uitron_size;///< uITRON size
	unsigned int dsp1_addr; ///< DSP1 starting address
	unsigned int dsp1_size; ///< DSP1 size
	unsigned int dsp2_addr; ///< ecos(na51023) / DSP2(na51000) starting address
	unsigned int dsp2_size; ///< ecos(na51023) / DSP2(na51000) size
	unsigned int loader_addr; ///< loader starting address
	unsigned int loader_size; ///< loader size
	unsigned int extdram_addr; ///< extra dram address (2nd dram)
	unsigned int extdram_size; ///< extra dram size (2nd dram)
} DRAM_PARTITION;

#endif
