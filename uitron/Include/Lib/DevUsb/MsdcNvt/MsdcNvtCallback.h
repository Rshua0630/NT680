/**
    Msdc-Nvt Vendor Callback for PC

    This system for PC control device(DSC) via MSDC. User can register callbacks
    to respond PC command.

    @file       MsdcNvtCallback.h
    @ingroup    mMsdcNvt

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _MSDCNVTCALLBACK_H
#define _MSDCNVTCALLBACK_H

#ifndef _WIN32
#include "UMSDC.h"
#else
#include <wTypes.h>
typedef unsigned char UINT8, *PUINT8;
#endif

/**
     Basic Data Structure

     Msdc-Nvt basic data structure for bi-direction transmission
*/
typedef struct _MSDCEXT_PARENT {
	UINT32 biSize;      ///< Structure Size
	UINT32 bHandled;    ///< Indicate Device Handle this function
	UINT32 bOK;         ///< Indicate Function Action is OK or Not
	UINT8  Reversed[4]; ///< Reversed Value for Structure DWORD Alignment
} tMSDCEXT_PARENT;

/**
     BOOL

     Msdc-Nvt BOOL data for bi-direction transmission
*/
typedef struct _MSDCEXT_BOOL {
	tMSDCEXT_PARENT tParent;
	UINT32          bValue; ///< a BOOL value
} tMSDCEXT_BOOL;

/**
     UINT32

     Msdc-Nvt BOOL data for bi-direction transmission
*/
typedef struct _MSDCEXT_UINT32 {
	tMSDCEXT_PARENT tParent;
	UINT32          uiValue;///< a UINT32 value
} tMSDCEXT_UINT32;

/**
     INT32

     Msdc-Nvt BOOL data for bi-direction transmission
*/
typedef struct _MSDCEXT_INT32 {
	tMSDCEXT_PARENT tParent;
	INT32           iValue; ///< a INT32 value
} tMSDCEXT_INT32;
//------------------------------------------------------------------------------
// Photo Data Types for MsdcNvt_AddCallback_Bi
//------------------------------------------------------------------------------
/**
     Display Setting

     MsdcNvtCb_GetDisplaySettings used structure
*/
typedef struct _MSDCEXT_DISPLAY_SETTING { ///< PC <- DSC
	tMSDCEXT_PARENT Parent;
	UINT32 AddrY[3];    ///< IDE 3 Buffers of Y
	UINT32 AddrU[3];    ///< IDE 3 Buffers of U
	UINT32 AddrV[3];    ///< IDE 3 Buffers of V
	UINT32 PitchY;      ///< IDE 3 Buffers of LineOffset Y
	UINT32 PitchUV;     ///< IDE 3 Buffers of LineOffset UV
	UINT32 Width;       ///< IDE 3 Buffers of Width
	UINT32 HeightY;     ///< IDE Height of Y
	UINT32 HeightUV;    ///< IDE Height of UV
} tMSDCEXT_DISPLAY_SETTING;

/**
     Display Setting 2

     MsdcNvtCb_GetDisplaySettings2 used structure
*/
typedef struct _MSDCEXT_DISPLAY_SETTING2 { ///< PC <- DSC
	tMSDCEXT_PARENT Parent;
	UINT32 nValidFrm;   ///< Valid Frame Number
	UINT32 AddrY[10];   ///< Display Buffers of Y
	UINT32 AddrU[10];   ///< Display Buffers of U
	UINT32 AddrV[10];   ///< Display Buffers of V
	UINT32 PitchY;      ///< Display Buffers of LineOffset Y
	UINT32 PitchUV;     ///< Display Buffers of LineOffset UV
	UINT32 Width;       ///< Display Buffers of Width
	UINT32 HeightY;     ///< IDE Height of Y
	UINT32 HeightUV;    ///< IDE Height of UV
} tMSDCEXT_DISPLAY_SETTING2;

/**
     Display Current

     MsdcNvtCb_GetDisplayCurrentBufSel used structure
*/
typedef struct _MSDCEXT_DISPLYA_CURRENT { ///< PC <- DSC
	tMSDCEXT_PARENT Parent;
	UINT32 OPTBUF;      ///< Current IDE Buffer Index
	UINT32 BUFNUM;      ///< Total IDE Buffers
} tMSDCEXT_DISPLYA_CURRENT;

/**
     Capture Jpeg Information

     MsdcNvtCb_CaptureJpgWidthGetInfo used structure
*/
typedef struct _MSDCEXT_INFO_CAPTURE_JPG { ///< PC <- DSC
	tMSDCEXT_PARENT Parent;
	UINT32  JpgAddr;    ///< Jpeg Data Address
	UINT32  JpgFileSize;///< Jpg File Size
} tMSDCEXT_INFO_CAPTURE_JPG;

/**
     Capture Raw Information

     MsdcNvtCb_CaptureRawWidthGetInfo used structure
*/
typedef struct _MSDCEXT_INFO_CAPTURE_RAW { ///< PC <- DSC
	tMSDCEXT_PARENT Parent;
	UINT32  RawAddr;    ///< Memory Address of Raw Data
	UINT32  RawFileSize;///< Raw Data Size
	UINT32  RawWidth;   ///< Raw Width
	UINT32  RawHeight;  ///< Raw Height
	UINT32  RawNbit;    ///< Raw 8bits or 12bits
} tMSDCEXT_INFO_CAPTURE_RAW;

//------------------------------------------------------------------------------
// Display Utility Implementation Data Types for MsdcNvt_AddCallback_Bi
//------------------------------------------------------------------------------
/**
     Display Format
*/
typedef enum _MSDCEXT_DISP_FORMAT {
	MSDCEXT_DISP_FORMAT_4_BIT = 0,  ///< Color format 4 bit
	MSDCEXT_DISP_FORMAT_2_BIT,      ///< Color format 2 bit
	MSDCEXT_DISP_FORMAT_1_BIT,      ///< Color format 1 bit
	MSDCEXT_DISP_FORMAT_8_BIT,      ///< Color format 8 bit
	MSDCEXT_DISP_FORMAT_YCBCR444,   ///< Color format YCBCR444
	MSDCEXT_DISP_FORMAT_YCBCR422,   ///< Color format YCBCR422
	MSDCEXT_DISP_FORMAT_YCBCR420,   ///< Color format YCBCR420
	MSDCEXT_DISP_FORMAT_ARGB4565,   ///< Color format ARGB4565
	MSDCEXT_DISP_FORMAT_ARGB8565,   ///< Color format ARGB8565
	MSDCEXT_DISP_FORMAT_YCBCR422_UVPACK, ///< Color format YCBCR422 with UV packing
	MSDCEXT_DISP_FORMAT_YCBCR420_UVPACK, ///< Color format YCBCR420 with UV packing
	MSDCEXT_DISP_FORMAT_YCBCR444_UVPACK, ///< Color format YCBCR444 with UV packing
	MSDCEXT_DISP_FORMAT_UNKNOWN = -1 ///< Unknown Format
} eMSDCEXT_DISP_FORMAT;

/**
     Display Engine
*/
typedef enum _MSDCEXT_DISP_ENG {
	MSDCEXT_DISP_ENG_VDO_0 = 0, ///< Video 0 Engine
	MSDCEXT_DISP_ENG_VDO_1, ///< Video 1 Engine
	MSDCEXT_DISP_ENG_OSD_0, ///< OSD 0 Engine
	MSDCEXT_DISP_ENG_OSD_1  ///< OSD 1 Engine
} eMSDCEXT_DISP_ENG;

/**
     Video Engine Information
*/
typedef struct _MSDCEXT_DISP_VDO_INFO {
	BOOL                    bEnable;
	UINT32                  Format;     ///< cast to eMSDCEXT_DISP_FORMAT
	UINT32                  AddrY[3];   ///< AddrY[0] is RGB565 when format is ARGB565
	UINT32                  AddrU[3];   ///< AddrU[0] is Alpha Plane when format is ARGB565
	UINT32                  AddrV[3];   ///< V address
	UINT32                  Width;      ///< width
	UINT32                  Height;     ///< height
	UINT32                  PitchY;     ///< Y Lineoffset
	UINT32                  PitchUV;    ///< UV Lineoffset
} tMSDCEXT_DISP_VDO_INFO;

/**
     OSD Engine Information
*/
typedef struct _MSDCEXT_DISP_OSD_INFO {
	BOOL                    bEnable;    ///< enable / disable
	UINT32                  Format;     ///< cast to eMSDCEXT_DISP_FORMAT
	UINT32                  AddrBuf;    ///< main buffer
	UINT32                  AddrAlpha;  ///< ARGB565's Alpha
	UINT32                  Width;      ///< width
	UINT32                  Height;     ///< height
	UINT32                  PitchBuf;   ///< main buffer lineoffset
	UINT32                  PitchAlpha; ///< alpah buffer lineoffset
} tMSDCEXT_DISP_OSD_INFO;

/**
     IDE Engine configure
*/
typedef struct _MSDCEXT_DISP_CFG {
	tMSDCEXT_PARENT         tParent; ///< basic data
	//[Device Set]
	tMSDCEXT_DISP_VDO_INFO  tInfoVdo[2];    ///< 2 videoes information
	tMSDCEXT_DISP_OSD_INFO  tInfoOsd[2];    ///< 2 OSDs information
} tMSDCEXT_DISP_CFG;

/**
     Video Current Buffer Index
*/
typedef struct _MSDCEXT_DISP_VDO_CURR_BUFF_IDX {
	tMSDCEXT_PARENT         tParent; ///< basic data
	//[Device Set]
	UINT32                  iSel[2]; ///< 0:Video1, 1:Video2
} tMSDCEXT_DISP_VDO_CURR_BUFF_IDX;

/**
     Enable / Disable a engine
*/
typedef struct _MSDCEXT_DISP_ENG_ONOFF {
	tMSDCEXT_PARENT         tParent; ///< basic data
	//[Host Set]
	UINT32                  eEngine; ///< cast to eMSDCEXT_DISP_ENG
	BOOL                    bEnable;
} tMSDCEXT_DISP_ENG_ONOFF;

/**
     OSD Palette
*/
typedef struct _MSDCEXT_DISP_OSD_PALETTE {
	tMSDCEXT_PARENT         tParent;        ///< basic data
	//[Host Set or Device Set]
	UINT32                  iSel;           ///< 0 or 1 for Palette Select
	UINT32                  nEntrys;        ///< Total effective enterys
	UINT32                  Palette[256];   ///< if is 16 color only use first 16 entrys
} tMSDCEXT_DISP_OSD_PALETTE;

/**
     Gamma Table
*/
typedef struct _MSDCEXT_DISP_GAMMA_TBL {
	tMSDCEXT_PARENT         tParent;        ///< basic data
	//[Host Set or Device Set]
	UINT8                   GammaTbl[256];  ///< Gamma Table, Max size is 256 Level for any chip
} tMSDCEXT_DISP_GAMMA_TBL;

/**
     Color Matrix Coefficients
*/
typedef struct _MSDCEXT_DISP_ICST_COEF_INFO {
	tMSDCEXT_PARENT         tParent;        ///< basic data
	//[Device Set]
	UINT32                  NumBitsInteger; ///< Numbers of Maxrix Coefficient Integer Bits
	UINT32                  NumBitsFraction;///< Numbers of Maxrix Coefficient Fraction Bits
	INT32                   PreMatrix[9];   ///< CoefTrans = PreMatrix*GainMatrix*PostMatrix
	INT32                   PostMatrix[9];  ///< CoefTrans = PreMatrix*GainMatrix*PostMatrix
} tMSDCEXT_DISP_ICST_COEF_INFO;

/**
     Color Matrix Enable/Disable
*/
typedef struct _MSDCEXT_DISP_ICST_COEF {
	tMSDCEXT_PARENT         tParent;        ///< basic data
	//[Host Set]
	UINT32                  CoefTrans[9];   ///< 3x3 Transform Matrix
} tMSDCEXT_DISP_ICST_COEF;

/**
     Saturation Information
*/
typedef struct _MSDCEXT_DISP_SATURATION_INFO {
	tMSDCEXT_PARENT         tParent;    ///< basic data
	//[Device Set]
	INT32                   ValMax;     ///< maximum value
	INT32                   ValMin;     ///< minimum value
	INT32                   ValDefault; ///< default value
} tMSDCEXT_DISP_SATURATION_INFO;

/**
     Contrast Information
*/
typedef struct _MSDCEXT_DISP_CONTRAST_INFO {
	tMSDCEXT_PARENT         tParent;    ///< basic data
	//[Device Set]
	INT32                   ValMax;     ///< maximum value
	INT32                   ValMin;     ///< minimum value
	INT32                   ValDefault; ///< default value
} tMSDCEXT_DISP_CONTRAST_INFO;

/**
     Brightness Information
*/
typedef struct _MSDCEXT_DISP_BRIGHTNESS_INFO {
	tMSDCEXT_PARENT         tParent;    ///< basic data
	//[Device Set]
	INT32                   ValMax;     ///< maximum value
	INT32                   ValMin;     ///< minimum value
	INT32                   ValDefault; ///< default value
} tMSDCEXT_DISP_BRIGHTNESS_INFO;

//------------------------------------------------------------------------------
// Filesys Utility Implementation Data Types for MsdcNvt_AddCallback_Bi
//------------------------------------------------------------------------------
/**
     Working Buffer
*/
typedef struct _MSDCEXT_FILE_WORKING_MEM {
	tMSDCEXT_PARENT         tParent; ///< basic data
	//[Device Set]
	UINT32                  Address; ///< address of working buffer begin
	UINT32                  Size;    ///< size of working buffer
} tMSDCEXT_FILE_WORKING_MEM;

/**
     File Information
*/
typedef struct _MSDCEXT_FILE_INFO {
	tMSDCEXT_PARENT         tParent;
	//[Host Set(Copy PC To Device) or Device Set(Copy Device To PC)]
	char                    FilePath[256]; ///< File Name / Path
	UINT32                  FileSize;      ///< File Total Size
	UINT32                  TransmitOffset;///< File Transmit Offset
	UINT32                  TransmitSize;  ///< File Transmit Size
	UINT32                  WorkingMem;    ///< Device Wroking Buffer
} tMSDCEXT_FILE_INFO;

//------------------------------------------------------------------------------
// Universal Adjustment Implementation Data Types for MsdcNvt_AddCallback_Bi
//------------------------------------------------------------------------------
/**
     Adjustment Description
*/
typedef struct _MSDCEXT_ADJ_ITEM_DESC {
	UINT32 bIsSign;     ///< signed or unsigned type
	UINT32 MaxValue;    ///< maximum value
	UINT32 MinValue;    ///< minimum value
	CHAR   Tag[64];     ///< a tag of string
} tMSDCEXT_ADJ_ITEM_DESC;

/**
     Adjustment Basic

     tMSDCEXT_ADJ_ITEM_DESC* with following data
*/
typedef struct _MSDCEXT_ADJ_ALL_DESC {
	tMSDCEXT_PARENT         tParent; ///< basic data
} tMSDCEXT_ADJ_ALL_DESC;

/**
     Adjustment Data

     void*  pVars; with following data
*/
typedef struct _MSDCEXT_ADJ_DATA {
	tMSDCEXT_PARENT         tParent;  ///< basic data
} tMSDCEXT_ADJ_DATA;

//------------------------------------------------------------------------------
// Standard Update Firmware
//------------------------------------------------------------------------------
/**
     PC Get Nand Block Information
*/
typedef struct _MSDCEXT_UPDFW_GET_BLOCK_INFO {
	tMSDCEXT_PARENT tParent;  ///< basic data
	UINT32 uiBytesPerBlock;   ///< Each Block Size
} tMSDCEXT_UPDFW_GET_BLOCK_INFO;

typedef struct _MSDCEXT_UPDFW_GET_WORK_MEM {
	tMSDCEXT_PARENT tParent;  ///< basic data
	UINT32 uiAddr; ///< working memory address
	UINT32 uiSize; ///< working memory size
} tMSDCEXT_UPDFW_GET_WORK_MEM;

typedef tMSDCEXT_UPDFW_GET_WORK_MEM tMSDCEXT_UPDFWALLINONE_STEP1;

/**
     PC Send Nand Block Data
*/
typedef struct _MSDCEXT_UPDFW_SET_BLOCK_INFO {
	tMSDCEXT_PARENT tParent;    ///< basic data
	UINT32 uiKey;               ///< a key value to indicate valid command ('N','O','V','A')
	UINT32 iBlock;              ///< Block Index of this Packet
	UINT32 IsLastBlock;         ///< Indicate Last Block for Reset System
	UINT32 EffectDataSize;      ///< Current Effective Data Size
} tMSDCEXT_UPDFW_SET_BLOCK_INFO;

typedef tMSDCEXT_UPDFW_SET_BLOCK_INFO tMSDCEXT_UPDFWALLINONE_STEP2;
#endif
