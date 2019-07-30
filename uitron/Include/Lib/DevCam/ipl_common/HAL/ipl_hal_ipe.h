/**
    IPH module (IPE type define)

    @file       ipl_hal_ipe.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IPE_H
#define _IPL_Hal_IPE_H
#include "ipl_hal_ipe_info.h"
/** \addtogroup mILibIPH */
//@{

typedef enum _IPH_IPE_QUE {
	ENUM_DUMMY4WORD(IPH_IPE_QUE)
} IPH_IPE_QUE;

/**
     enum of IPE update selection
*/
typedef enum _IPE_UPDATE {
	IPE_IN_ADDR         = 0x00000001,   ///< update input addr
	IPE_OUT_ADDR        = 0x00000002,   ///< update output addr
	IPE_IN_SIZE         = 0x00000004,   ///< update input size/lineofs
	IPE_OUT_SIZE        = 0x00000008,   ///< update output lineofs(dma out only)
	IPE_SUBFEN          = 0x00000010,   ///< update func enable/disable
	IPE_EDGEEXT_INFO    = 0x00000020,   ///< update edge extraction
	IPE_EDGEENH_INFO    = 0x00000040,   ///< update positive/negative edge enhance/inverse
	IPE_EDGEMAP_INFO    = 0x00000080,   ///< update edge map
	IPE_RGBLPF_INFO     = 0x00000100,   ///< update rgb low pass
	IPE_CC_INFO         = 0x00000200,   ///< update color correction
	IPE_CST_INFO        = 0x00000400,   ///< update color space transfor
	IPE_C_CTRL          = 0x00000800,   ///< update color control
	IPE_C_CON           = 0x00001000,   ///< update color contrast(Y/CC)
	IPE_C_OFS           = 0x00002000,   ///< update Cb/Cr offset
	IPE_RANDNOISE       = 0x00004000,   ///< update YCC random noise
	IPE_YCCMASK_INFO    = 0x00008000,   ///< update YCC mask
	IPE_YCCFIX_INFO     = 0x00010000,   ///< update YCC fix replace
	IPE_CSTP            = 0x00020000,   ///< update color space transfor protection
	IPE_3DCC_INFO       = 0x00040000,   ///< update 3d color correction dram addr
	IPE_RGBGAMMA        = 0x00080000,   ///< update rgb gamma dram addr
	IPE_YCURVE          = 0x00100000,   ///< update y gamma dram addr
	IPE_GAMRAND         = 0x00200000,   ///< update gamma random noise
	IPE_ETH_INFO        = 0x00400000,   ///< update eth
	IPE_INIT            = 0x00800000,   ///< init ipe
	IPE_LOADALL_FD      = 0x01000000,   ///< speical case for IPL pipe mode, only valid at _IPE_Load function
	///< (remove 3DCC & RGBGAMMA & YCURVE update bit)
	IPE_LOADALL_NON_FD  = 0x02000000,   ///< speical case for IPL pipe mode, only valid at _IPE_Load function
	IPE_HOVLP_SEL       = 0x04000000,   ///< Choose IPE H overlap to fixed 8 pixels (when MST + SQUARE)
	IPE_DMAOUT			= 0x08000000,	///< update dma output setting
	ENUM_DUMMY4WORD(IPE_UPDATE)
} IPE_UPDATE;

/**
     enum of IPE read selection

     read from inner table, not from hw register.
*/
typedef enum _IPE_READ {
	IPE_R_IO        = 0x00000001,   ///< read in/out info. exclude IPE_SUBFUNC,         Data type: IPE
	IPE_R_EDGEEXT   = 0X00000002,   ///< read edge extraction,                          Data type: IPE_EdgeExt
	IPE_R_EDGEENH   = 0X00000003,   ///< read positive/negative edge enhance/inverse,   Data type: IPE_EdgeEnh
	IPE_R_EDGEMAP   = 0X00000004,   ///< read edge map,                                 Data type: IPE_EdgeMap
	IPE_R_RGBLPF    = 0X00000005,   ///< read rgb low pass,                             Data type: IPE_RGBLPF
	IPE_R_CC        = 0X00000006,   ///< read color correction,                         Data type: IPE_CC
	IPE_R_CST       = 0X00000007,   ///< read color space transfor,                     Data type: IPE_CST
	IPE_R_C_CTRL    = 0X00000008,   ///< read color control,                            Data type: IPE_CCTRL
	IPE_R_C_CON     = 0X00000009,   ///< read color contrast(Y/CC),                     Data type: IPE_CCON
	IPE_R_C_OFS     = 0X0000000A,   ///< read Cb/Cr offset,                             Data type: IPE_CCOFS
	IPE_R_RANDNOISE = 0X0000000B,   ///< read YCC random noise,                         Data type: IPE_RAND_NR
	IPE_R_YCCMASK   = 0X0000000C,   ///< read YCC mask,                                 Data type: IPE_YCCMASK
	IPE_R_YCCFIX    = 0X0000000D,   ///< read YCC fix replace,                          Data type: IPE_YCCFIX
	IPE_R_CSTP      = 0X0000000E,   ///< read color space transfor protection,          Data type: UINT8
	IPE_R_3DCC      = 0X0000000F,   ///< read 3d color correction dram addr,            Data type: UINT32
	IPE_R_RGBGAMMA  = 0X00000010,   ///< read rgb gamma dram addr,                      Data type: UINT32
	IPE_R_INTEEN    = 0X00000011,   ///< read interruppt enabe bit,                     Data type: IPEINTE
	IPE_R_SUBFEN    = 0X00000012,   ///< read sub function enbable,                     Data type: IPEINTE
	IPE_R_OUT_ADDR  = 0X00000013,   ///< read output address,                           Data type: UINT32
	IPE_R_ETH_PARAM = 0X00000014,   ///< read eth parameter                             Data type: IPE_ETH_DramRst
	IPE_R_IO_LIGHT  = 0x00000015,   ///< read in/out info. exclude IPE_SUBFUNC,         Data type: IPE_HAL_IOINFO
	ENUM_DUMMY4WORD(IPE_READ)
} IPE_READ;

/**
	IPE IO Information

	for IPE_R_IO_LIGHT used
*/
typedef struct _IPE_HAL_IOINFO {
	//in
	IPE_SIZE InSizeY;         ///< in Y information
	UINT32 InLofsC;           ///< in C lineoffset
	IPEFmt InFmt;             ///< in format
	UINT32 InAddr[2];         ///< in addr (Image: idx0 = y, idx1 = uv)
	UINT32 InRandEn;
	UINT32 InRandRst;

	//out
	IPEDataDrop DataDrop;      ///< YCC sub-sample selection
	UINT32 OutLofsY;            ///< out Y lineoffset
	UINT32 OutLofsC;           ///< out C lineoffset
	IPEFmt OutFmt;             ///< out format
	UINT32 OutAddr[2];         ///< out addr (Image: idx0 = y, idx1 = uv)
	BOOL Output2IMEEn;        ///< main img output to ime directly
	BOOL Output2DMAEn;        ///< enable/disable output to dram
	IPEDMAOutSel DMAOutSel;   ///< dma ouput selection
} IPE_HAL_IOINFO;


/**
     IPE configuration

     all IPE configuration including all IPE sub function
*/
typedef struct _IPE_Manager {
	IPEMode IPE_Mode;         ///< ipe input source

	//in
	IPE_SIZE InSizeY;         ///< in Y information
	UINT32 InLofsC;           ///< in C lineoffset
	IPEFmt InFmt;             ///< in format
	UINT32 InAddr[2];         ///< in addr (Image: idx0 = y, idx1 = uv)

	UINT32 InRandEn;
	UINT32 InRandRst;

	IPE_HAL_HOVLP_SEL_ENUM InHovlpOpt;       ///< Choose IPE H overlap to fixed 8 pixels (when MST + SQUARE)

	//out
	IPEDataDrop DataDrop;      ///< YCC sub-sample selection
	UINT32 OutLofsY;            ///< out Y lineoffset
	UINT32 OutLofsC;           ///< out C lineoffset
	IPEFmt OutFmt;             ///< out format
	UINT32 OutAddr[2];         ///< out addr (Image: idx0 = y, idx1 = uv)
	BOOL Output2IMEEn;        ///< main img output to ime directly
	BOOL Output2DMAEn;        ///< enable/disable output to dram
	IPEDMAOutSel DMAOutSel;   ///< dma ouput selection

	IPE_SUBFUNC *pSubFunc;          ///< -
	//reverse
	UINT32 rev;                     //reserve for IPH paraset idx
} IPE_Manager, *pIPE_Manager;

/**
     IPE configuration
*/
typedef IPE_Manager IPE;

//@}
#endif
