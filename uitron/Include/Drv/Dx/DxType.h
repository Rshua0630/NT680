/**
    Device/Driver Types

    The definition for Dx group APIs.

    @file       DxType.h
    @ingroup    mDX

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/


#ifndef _DX_TYPE_H
#define _DX_TYPE_H

#include "Type.h"

/**
     @addtogroup mDX
*/
//@{

#define DX_CLASS_MASK           0xffff0000 ///< the mask of DX_CLASS used bit

#define DX_CLASS_NULL           0x00000000 ///< unknown dx class
#if 0 //internal driver not be used currently.
//internal devices
#define DX_CLASS_TIMER          0x00010000
#define DX_CLASS_GRPH2D         0x00020000 //ImgTrans
#define DX_CLASS_CODECJPEG      0x00030000 //VdoEnc
#define DX_CLASS_CODECH264      0x00040000 //VdoEnc
#define DX_CLASS_DISPLAY        0x00050000 //VdoOut
#define DX_CLASS_AUDIO          0x00060000 //AudIn, AudOut
#endif

//external devices
#define DX_CLASS_DISPLAY_EXT    0x00150000 ///< used in Dx_GetObject to get a class of display object (like LCD,TV,HDMI)
#define DX_CLASS_AUDIO_EXT      0x00160000 ///< used in Dx_GetObject to get a class of audio object (like AUDIO-DAC or AUDIO-ADC)
#define DX_CLASS_STORAGE_EXT    0x00170000 ///< used in Dx_GetObject to get a class of storage object (like NAND,SPI,SD,RAM)
#define DX_CLASS_USB_EXT        0x00180000 ///< used in Dx_GetObject to get a class of USB object (like USB)
#define DX_CLASS_SENSOR_EXT     0x00190000 ///< used in Dx_GetObject to get a class of sensor object
#define DX_CLASS_POWER_EXT      0x001a0000 ///< used in Dx_GetObject to get a class of power object (like DCIN,BATTERY)
#define DX_CLASS_INPUT_EXT      0x001b0000 ///< used in Dx_GetObject to get a class of input object (like KEY,TOUCH)
#define DX_CLASS_STATUS_EXT     0x001c0000 ///< used in Dx_GetObject to get a class of status object (like LED,IRCUT)

#define DX_TYPE_MASK            0x0000ffff ///< the mask of DX_TYPE used bit

/**
     @name display ext types
*/
//@{
#define DX_TYPE_LCD             0x00000000 ///< used in DX_CLASS_DISPLAY_EXT to get LCD object
#define DX_TYPE_TVOUT           0x00000001 ///< used in DX_CLASS_DISPLAY_EXT to get TV object
#define DX_TYPE_HDMIOUT         0x00000002 ///< used in DX_CLASS_DISPLAY_EXT to get HDMI object
#define DX_TYPE_LCD2            0x00000003 ///< used in DX_CLASS_DISPLAY_EXT to get LCD2 object
//@}

#if 0 //udio ext types not be used currently.
//audio ext types
#define DX_TYPE_SPEAKER         0x00000000
#define DX_TYPE_LINEOUT         0x00000001
#endif

/**
     @name storage ext types
     The definition of EmbMem is the storage without any card detection,
     which means the end-user cannot take that storage out. The EmbMem can also
     be a combination of partition such as partition 0 is spi-uITRON,
     partition 1 is ram-FAT these depend on what you designed on DxStorage_Mem.c

*/
//@{
#define DX_TYPE_EMBMEM0         0x00000000 ///< used in DX_CLASS_STORAGE_EXT to get 1st nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM1         0x00000001 ///< used in DX_CLASS_STORAGE_EXT to get 2nd nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM2         0x00000002 ///< used in DX_CLASS_STORAGE_EXT to get 3rd nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM3         0x00000003 ///< used in DX_CLASS_STORAGE_EXT to get 4th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM4         0x00000004 ///< used in DX_CLASS_STORAGE_EXT to get 5th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM5         0x00000005 ///< used in DX_CLASS_STORAGE_EXT to get 6th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM6         0x00000006 ///< used in DX_CLASS_STORAGE_EXT to get 7th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM7         0x00000007 ///< used in DX_CLASS_STORAGE_EXT to get 8th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM8         0x00000008 ///< used in DX_CLASS_STORAGE_EXT to get 9th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM9         0x00000009 ///< used in DX_CLASS_STORAGE_EXT to get 10th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM10        0x0000000a ///< used in DX_CLASS_STORAGE_EXT to get 11th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM11        0x0000000b ///< used in DX_CLASS_STORAGE_EXT to get 12th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM12        0x0000000c ///< used in DX_CLASS_STORAGE_EXT to get 13th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM13        0x0000000d ///< used in DX_CLASS_STORAGE_EXT to get 14th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM14        0x0000000e ///< used in DX_CLASS_STORAGE_EXT to get 15th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_EMBMEM15        0x0000000f ///< used in DX_CLASS_STORAGE_EXT to get 16th nand partition mapping to DxStorage_EmbMem.c
#define DX_TYPE_CARD0           0x00000010 ///< used in DX_CLASS_STORAGE_EXT to get 1st card device mapping to DxStorage_Card.c
#define DX_TYPE_CARD1           0x00000011 ///< used in DX_CLASS_STORAGE_EXT to get 2nd card device mapping to DxStorage_Card.c
#define DX_TYPE_CARD2           0x00000012 ///< used in DX_CLASS_STORAGE_EXT to get 3rd card device mapping to DxStorage_Card.c
#define DX_TYPE_CARD3           0x00000013 ///< used in DX_CLASS_STORAGE_EXT to get 4th card device mapping to DxStorage_Card.c
#define DX_TYPE_RAMDISK0        0x00000020 ///< used in DX_CLASS_STORAGE_EXT to get 1st ramdisk device mapping to DxStorage_Ram.c
#define DX_TYPE_RAMDISK1        0x00000021 ///< used in DX_CLASS_STORAGE_EXT to get 2nd ramdisk device mapping to DxStorage_Ram.c
#define DX_TYPE_RAMDISK2        0x00000022 ///< used in DX_CLASS_STORAGE_EXT to get 3rd ramdisk device mapping to DxStorage_Ram.c
#define DX_TYPE_RAMDISK3        0x00000023 ///< used in DX_CLASS_STORAGE_EXT to get 4th ramdisk device mapping to DxStorage_Ram.c
#define DX_TYPE_OUTLED          0x00000030 ///< used in DX_CLASS_STATUS_EXT
#define DX_TYPE_OUTIRCUT        0x00000031 ///< used in DX_CLASS_STATUS_EXT
//@}

//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection
/**
     @name USB ext types
*/
//@{
#define DX_TYPE_USB0         0x00000000 ///< used in DX_CLASS_USB_EXT to get 1st USB object
#define DX_TYPE_USB1         0x00000001 ///< used in DX_CLASS_USB_EXT to get 2nd USB object
//@}
//#NT#2018/01/12#Ben Wang -end
#if 0 //power ext types not be used currently.
#define DX_TYPE_BATTERY         0x00000000
#define DX_TYPE_DCIN            0x00000001
#endif

typedef struct _DX_SIGN {
	UINT32 sign; //Signature
}
DX_SIGN, *PDX_SIGN;

typedef PDX_SIGN DX_HANDLE; ///< DX object handle to operate any Dx APIs

//@}
#endif //_DX_TYPE_H
