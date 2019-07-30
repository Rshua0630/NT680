/*
    Header file for JPEG module

    This file is the header file that define the API for JPEG module.

    @file       jpeg.h
    @ingroup    mIDrvCodec_JPEG
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _JPEG_H
#define _JPEG_H

#include "Type.h"

/**
    @addtogroup mIDrvCodec_JPEG
*/
//@{

/**
    JPEG block format

    @note For JPG_HW_YUV_FORMAT
*/
typedef enum {
	JPEG_BLK_Y_2X    = 0x0800,      ///< Y's block width is 16
	JPEG_BLK_Y_1X    = 0x0400,      ///< Y's block width is 8
	JPEG_BLK_Y_2Y    = 0x0200,      ///< Y's block height is 16
	JPEG_BLK_Y_1Y    = 0x0100,      ///< Y's block height is 8
	JPEG_BLK_U_2X    = 0x0080,      ///< U's block width is 16
	JPEG_BLK_U_1X    = 0x0040,      ///< U's block width is 8
	JPEG_BLK_U_2Y    = 0x0020,      ///< U's block height is 16
	JPEG_BLK_U_1Y    = 0x0010,      ///< U's block height is 8
	JPEG_BLK_V_2X    = 0x0008,      ///< V's block width is 16
	JPEG_BLK_V_1X    = 0x0004,      ///< V's block width is 8
	JPEG_BLK_V_2Y    = 0x0002,      ///< V's block height is 16
	JPEG_BLK_V_1Y    = 0x0001,      ///< V's block height is 8
	ENUM_DUMMY4WORD(JPEG_BLK_FORMAT)
} JPEG_BLK_FORMAT;

/**
    JPEG YUV format

    @note For jpeg_setFileFormat()
*/
typedef enum {
	JPEG_YUV_FORMAT_100     = (JPEG_BLK_Y_1X | JPEG_BLK_Y_1Y),                                                                  ///< YUV 100 (Y plane only)
	JPEG_YUV_FORMAT_111     = (JPEG_BLK_Y_1X | JPEG_BLK_Y_1Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 111
	JPEG_YUV_FORMAT_211     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_1Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 2h11 (4:2:2 in Exif)
	JPEG_YUV_FORMAT_211V    = (JPEG_BLK_Y_1X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 2v11
	JPEG_YUV_FORMAT_222     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_1Y | JPEG_BLK_U_2X | JPEG_BLK_U_1Y | JPEG_BLK_V_2X | JPEG_BLK_V_1Y),  ///< YUV 222h
	JPEG_YUV_FORMAT_222V    = (JPEG_BLK_Y_1X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_2Y | JPEG_BLK_V_1X | JPEG_BLK_V_2Y),  ///< YUV 111v
	JPEG_YUV_FORMAT_411     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_1Y | JPEG_BLK_V_1X | JPEG_BLK_V_1Y),  ///< YUV 411  (4:2:0 in Exif)
	JPEG_YUV_FORMAT_422     = (JPEG_BLK_Y_2X | JPEG_BLK_Y_2Y | JPEG_BLK_U_2X | JPEG_BLK_U_1Y | JPEG_BLK_V_2X | JPEG_BLK_V_1Y),  ///< YUV 422h
	JPEG_YUV_FORMAT_422V    = (JPEG_BLK_Y_2X | JPEG_BLK_Y_2Y | JPEG_BLK_U_1X | JPEG_BLK_U_2Y | JPEG_BLK_V_1X | JPEG_BLK_V_2Y),  ///< YUV 422v
	ENUM_DUMMY4WORD(JPEG_YUV_FORMAT)
} JPEG_YUV_FORMAT;

/**
    JPEG interrupt status

    @note For jpeg_getStatus(), jpeg_setEnableInt(), jpeg_setDisableInt() and jpeg_waitDone().
*/
typedef enum {
	JPEG_INT_FRAMEEND   = 0x01,         ///< Frame end. Encoding/decoding a frame is done.
	///< @note  When bit-stream buffer end and frame end are both happened,
	///<        only bit-stream buffer end interrupt will be issued.
	///<        You have to set the bit-stream buffer again to issue the frame end interrupt.
	JPEG_INT_SLICEDONE  = 0x02,         ///< Slice Done. (Not support in NT96660)
	JPEG_INT_DECERR     = 0x04,         ///< Decode error. Encounter error in JPEG decode.
	JPEG_INT_BUFEND     = 0x08,         ///< Bit-stream buffer end. Bit-stream length exceed specified length.
	JPEG_INT_ALL        = 0x0F,         ///< All interrupts of above

	ENUM_DUMMY4WORD(JPEG_INT)
} JPEG_INT;

/**
    JPEG Decode Scale Ratio

    @note For jpeg_setScaleRatio().
*/
typedef enum {
	JPEG_DECODE_RATIO_WIDTH_1_2,        ///< 1/2 of Width Only
	JPEG_DECODE_RATIO_BOTH_1_2,         ///< 1/2 of Width/Height
	JPEG_DECODE_RATIO_BOTH_1_4,         ///< 1/4 of Width/Height
	JPEG_DECODE_RATIO_BOTH_1_8,         ///< 1/8 of Width/Height

	ENUM_DUMMY4WORD(JPEG_DECODE_RATIO)
} JPEG_DECODE_RATIO;

/**
    JPEG DC Output Scaling ratio

    JPEG DC Output scaling ratio.
*/
typedef enum {
	JPEG_DC_OUT_RATIO_1_2 = 1,          ///< DC output scaled to 1/2
	JPEG_DC_OUT_RATIO_1_4,              ///< DC output scaled to 1/4
	JPEG_DC_OUT_RATIO_1_8,              ///< DC output scaled to 1/8

	ENUM_DUMMY4WORD(JPEG_DC_OUT_RATIO)
} JPEG_DC_OUT_RATIO;

/**
    JPEG DC Output Configuration structure

    @note For jpeg_setDCOut().
*/
typedef struct {
	UINT32              DCEnable;       ///< DC output enable
	///< - @b TRUE  : Enable DC output function
	///< - @b FALSE : Disable DC output function
	JPEG_DC_OUT_RATIO   DCXRatio;       ///< DC output width (horizontal) ratio
	JPEG_DC_OUT_RATIO   DCYRatio;       ///< DC output height (vertical) ratio
	UINT32              DCYAddr;        ///< DC output Y DRAM address
	UINT32              DCUAddr;        ///< DC output UV packed DRAM address
	UINT32              DCVAddr;        ///< Obsoleted member
	UINT32              DCYLineOffset;  ///< DC output Y line offset
	UINT32              DCULineOffset;  ///< DC output UV packed line offset
	UINT32              DCVLineOffset;  ///< Obsoleted member
} JPEG_DC_OUT_CFG, *PJPEG_DC_OUT_CFG;

/**
    JPEG Bit Rate Control Information

    @note For jpeg_getBRCInfo().
*/
typedef struct {
	UINT32 uiBRCInfo1;                  ///< Rho Q/8
	UINT32 uiBRCInfo2;                  ///< Rho Q/4
	UINT32 uiBRCInfo3;                  ///< Rho Q/2
	UINT32 uiBRCInfo4;                  ///< Rho Q
	UINT32 uiBRCInfo5;                  ///< Rho 2Q
	UINT32 uiBRCInfo6;                  ///< Rho 4Q
	UINT32 uiBRCInfo7;                  ///< Rho 8Q
} JPEG_BRC_INFO, *PJPEG_BRC_INFO;

/**
    JPEG configuration identifier

    @note For jpeg_setConfig()
*/
typedef enum {
	JPEG_CONFIG_ID_CHECK_DEC_ERR,           ///< Enable / Disable checking decode error
	///< (Default is DISABLED)
	///< Context can be:
	///< - @b TRUE  : JPEG driver will check decode error
	///< - @b FALSE : JPEG driver will NOT check decode error

	JPEG_CONFIG_ID_FREQ,                    ///< JPEG module clock (Unit: MHz), will be active on succeeding jpeg_open().
	///< Context can be one of the following:
	///< - @b 192   : 192 MHz (Default value)
	///< - @b 240   : 240 MHz
	///< - @b 250   : 250 MHz

	ENUM_DUMMY4WORD(JPEG_CONFIG_ID)
} JPEG_CONFIG_ID;

// -------------------------------------------------
// The API for the JPEG codec driver
// -------------------------------------------------
extern ER       jpeg_open(void);
extern ER       jpeg_close(void);
extern BOOL     jpeg_isOpened(void);
extern ID       jpeg_getLockStatus(void);

extern ER       jpeg_setConfig(JPEG_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   jpeg_getConfig(JPEG_CONFIG_ID ConfigID);

extern UINT32   jpeg_getStatus(void);
extern UINT32   jpeg_getActiveStatus(void);
extern UINT32   jpeg_getComSize(void);
extern void     jpeg_setSWReset(void);

extern void     jpeg_setStartEncode(void);
extern void     jpeg_setStartDecode(void);

extern void     jpeg_setEndEncode(void);
extern void     jpeg_setEndDecode(void);

extern UINT32   jpeg_waitDone(void);
extern BOOL     jpeg_waitDonePolling(void);

extern ER       jpeg_setEnableInt(UINT32 uiInt);
extern void     jpeg_setDisableInt(UINT32 uiInt);

extern void     jpeg_setImgStartAddr(UINT32 ImgYAddr, UINT32 ImgUAddr, UINT32 ImgVAddr);
extern void     jpeg_getImgStartAddr(UINT32 *ImgYAddr, UINT32 *ImgUAddr, UINT32 *ImgVAddr);

extern ER       jpeg_setImgLineOffset(UINT32 YLOFS, UINT32 ULOFS, UINT32 VLOFS);
extern UINT32   jpeg_getImgLineOffsetY(void);
extern UINT32   jpeg_getImgLineOffsetU(void);
extern UINT32   jpeg_getImgLineOffsetV(void);

extern ER       jpeg_setFileFormat(UINT32 ImgWidth, UINT32 ImgHeight, JPEG_YUV_FORMAT FileFormat);

// Bit-stream control
extern ER       jpeg_setCmpStartAddr(UINT32 BSAddr, UINT32 BufSize);
extern UINT32   jpeg_getBSStartAddr(void);
extern UINT32   jpeg_getBSCurAddr(void);
extern void     jpeg_setBSOutput(BOOL Enable);

// Restart marker
extern ER       jpeg_setRestartInterval(UINT32 MCUNum);
extern ER       jpeg_setRestartEnable(BOOL Enable);
extern UINT32   jpeg_setRestartEOF(void);

// Encode format transform (420 -> 422)
extern void     jpeg_setFmtTranEnable(void);
extern void     jpeg_setFmtTranDisable(void);

// Encode DC output
extern ER       jpeg_setDCOut(PJPEG_DC_OUT_CFG pDCOutCfg);

// Decode crop
extern ER       jpeg_setCrop(UINT32 StartX, UINT32 StartY, UINT32 Width, UINT32 Height);
extern void     jpeg_setCropDisable(void);
extern void     jpeg_setCropEnable(void);

// Decode scaling
extern void     jpeg_setScaleEnable(void);
extern void     jpeg_setScaleDisable(void);
extern ER       jpeg_setScaleRatio(JPEG_DECODE_RATIO ScaleRatio);

// Table access
extern ER       jpeg_setQTabHW(UINT8 *pucQTabY, UINT8 *pucQTabUV);
extern void     jpeg_setEncodeHuffTabHW(UINT16 *puiHuffTabLAC, UINT16 *puiHuffTabLDC, UINT16 *puiHuffTabCAC, UINT16 *puiHuffTabCDC);
extern void     jpeg_setDecodeHuffTabHW(UINT8 *pHuffDC0th, UINT8 *pHuffDC1th, UINT8 *pHuffAC0th, UINT8 *pHuffAC1th);

// Profiling
extern UINT32   jpeg_getCycleCnt(void);

// BRC control
extern void     jpeg_getBRCInfo(PJPEG_BRC_INFO pBRCInfo);

//@}

#endif
