#ifndef _COREINFO_H
#define _COREINFO_H

/**
    Core Infomation

    CPU1, CPU2, and DSP memory pool and excuting address export here.
    this structure should be addressing to fixed address '0x800003C0'.
    This structure size must always be 64 bytes.

    This is common header used between firmware of uITRON, eCos, Linux, DSP
    so !!!!!! DO NOT modify it !!!!!!
*/

#define CORE_INFO_OFFSET 0x000003C0

typedef struct _CORE_INFO {
	UINT32 InfoFourCC;  ///< FourCC CORE
	UINT32 InfoSize;    ///< Structure Size
	UINT32 uItronAddr;  ///< uITRON address
	UINT32 uItronSize;  ///< uITRON size
	UINT32 eCosAddr;    ///< eCos address
	UINT32 eCosSize;    ///< eCos size
	UINT32 uBootAddr;   ///< uboot address
	UINT32 uBootSize;   ///< uboot size
	UINT32 uImageAddr;  ///< uImage address
	UINT32 uImageSize;  ///< uImage size
	UINT32 LinuxAddr;   ///< linux address
	UINT32 LinuxSize;   ///< linux size
	UINT32 RootFsAddr;  ///< root-fs address
	UINT32 RootFsSize;  ///< root-fs size
	UINT32 DspAddr;     ///< dsp address
	UINT32 DspSize;     ///< dsp size
} CORE_INFO;

#endif