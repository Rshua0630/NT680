/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for RDE
    module.

    @file       rde_lib.h
    @ingroup    mIDrvIPPRDE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _RDE_LIB_H
#define _RDE_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include    "Type.h"//a header for basic variable type

/**
    @addtogroup mIDrvIPPRDE
*/
//@{

/**
    Define RDE interrput enable.
*/
//@{
#define RDE_INTE_DEC_END                    0x00000001  ///< Enable interrupt of frame end
#define RDE_INTE_DEC_ERR                    0x00000002  ///< Enable interrupt of coding error
#define RDE_INTE_STPBD_ERR                  0x00000004  ///< Enable interrupt of DCE stripe out of bound error
#define RDE_INTE_DEC_ALL                    0x00000007  ///< Enable all interrupts(default)

//@}

/**
    Define RDE interrput status.
*/
//@{
#define RDE_INT_DEC_END                     0x00000001  ///< Status of interrupt of frame end
#define RDE_INT_DEC_ERR                     0x00000002  ///< Status of interrupt of coding error
#define RDE_INT_STPBD_ERR                   0x00000004  ///< Status interrupt of DCE stripe out of bound error
#define RDE_INT_DEC_ALL                     0x00000007  ///< Status of all interrupts
//@}

/*----------- engine control enumeration ------------*/
/**
    State of State Machine
*/
//@{
typedef enum {
	RDE_ENGINE_IDLE  = 0,        ///< Idel state
	RDE_ENGINE_READY = 1,        ///< Ready state
	RDE_ENGINE_PAUSE = 2,        ///< Pause state
	RDE_ENGINE_RUN   = 3,        ///< Run state

	ENUM_DUMMY4WORD(RDE_STATE_MACHINE)
} RDE_STATE_MACHINE;
//@}

/**
    Operations of engine action
*/
//@{
typedef enum {
	RDE_OP_OPEN          = 0,    ///< Open engine
	RDE_OP_CLOSE         = 1,    ///< Close engine
	RDE_OP_SETPARAM      = 2,    ///< Set parameter
	RDE_OP_START         = 3,    ///< Start engine
	RDE_OP_PAUSE         = 4,    ///< Pause engine

	ENUM_DUMMY4WORD(RDE_ACTION_OP)
} RDE_ACTION_OP;
//@}

/**
    Engine operation mode
*/
//@{
typedef enum {
	LINE_MODE  = 0,         ///< Encoder use line mode
	FRAME_MODE = 1,         ///< Encoder use frame mode

	ENUM_DUMMY4WORD(RDE_OP_MODE)
} RDE_OP_MODE;
//@}

/**
    Stripe ID
*/
typedef enum {
	STRP_1ST = 0,           ///< Line mode first stripe
	STRP_2ND = 1,           ///< Line mode second stripe

	ENUM_DUMMY4WORD(RDE_STRP_ID)
} RDE_STRP_ID;

/**
    input burst settings
*/
typedef enum {
	RDE_BURST_32W = 0,
	RDE_BURST_48W = 1
} RDE_BURST_SEL;

/*----------- engine driver structure -------------*/
/*
    RDE Open Object - Open Object is used for opening RDE module.
*/
typedef struct _RDE_OPENOBJ {
	VOID (*FP_RDEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 RDE_CLOCKSEL;
} RDE_OPENOBJ;

/**
    RDE output setting

    Structure of settings of output function
*/
//@{
typedef struct _RDE_FUNC_EN_STRUCT {
	BOOL bOpMode;   ///< Enable line or frame mode
	BOOL bMstrpEn;  ///< Enable multiple stripe
	BOOL bHFlipEn;  ///< Enable horizontal flip
	BOOL bVFlipEn;  ///< Enable vertical flip
	BOOL bDegammaEn; ///< Enable degamma
} RDE_FUNC_EN;
//@}

/**
    RDE DMA in info

    Structure of setting of DMA input
*/
//@{
typedef struct _RDE_DMA_INFO_STRUCT {
	UINT32 uiBsiSAddr;        ///< Start address of crop Y line of bitstream
	UINT32 uiBsiLofst;       ///< Line offset of input bit stream
} RDE_DMA_INFO, RDE_LINE_IN_INFO;
//@}

/**
    RDE DMA in info

    Structure of setting of DMA input
*/
//@{
typedef struct _RDE_FRAME_IN_INFO_STRUCT {
	UINT32 uiBsiSAddr;       ///< Start address of input bit stream
	UINT32 uiBsiLength;      ///< Length of input bit stream
} RDE_FRAME_IN_INFO;
//@}

/**
    RDE DMA out info

    Structure of setting of DMA output
*/
//@{
typedef struct _RDE_FRAME_OUT_INFO_STRUCT {
	UINT32 uiImgoSAddr;      ///< Start address of output image
	UINT32 uiImgoLofst;      ///< Line offset of output image
} RDE_FRAME_OUT_INFO;
//@}

/**
    RDE image size info

    Structure of target image size
*/
//@{
typedef struct _RDE_CROP_PARAM_STRUCT {
	UINT32 uiCropX;             ///< Crop start X coordinate
	RDE_LINE_IN_INFO StrpIn[2]; ///< start address of 1st & 2nd stripe(change address for Crop start Y)
	UINT32 uiCropHeight;        ///< height of "crop image"
	UINT32 uiSecStrpSpt;        ///< start point of 2nd stripe
} RDE_CROP_PARAM;
//@}

/**
    RDE engine parameter for library

    Structure of all avaliable engine parameter
*/
//@{
typedef struct _RDE_PARAM_STRUCT {
	RDE_FUNC_EN FuncEn;             ///< Enable function of RDE
	RDE_FRAME_IN_INFO FrameIn;      ///< Info of DMA frame mode input
	RDE_FRAME_OUT_INFO FrameOut;    ///< Info of DMA frame mode output
	UINT32 uiActWidth;              ///< width of original raw image
	RDE_CROP_PARAM CropInfo;        ///< Crop parameter
	UINT32 uiBrcQInit;              ///< Initial Q table set(0~7)
	UINT32 *puiDgmaTbl;             ///< Pointer of degamma table
} RDE_PARAM;
//@}

#if (_EMULATION_ == ENABLE)
// enable when verifying HW, remove when releasing
/*
    RDE output random padding seed value
*/
typedef struct _RDE_OUT_RAND_INIT_STRUCT {
	UINT32 uiUserOutRandInit0;     ///< Random seed 0 of output padding
	UINT32 uiUserOutRandInit1;     ///< Random seed 1 of output padding
} RDE_OUT_RAND_INIT;

/*
    RDE line mode debug control
*/
typedef struct _RDE_LINE_MODE_DBG_STRUCT {
	BOOL bOutSel;                   ///< Output destination selection
	RDE_STRP_ID StrpID;           ///< Stripe ID
} RDE_LINE_MODE_DBG;

/*
    RDE engine parameter for library

    Structure of all avaliable engine parameter
*/
typedef struct _RDE_INT_PARAM_STRUCT {
	BOOL bOutPadSel;                ///< Padding method of output
	RDE_OUT_RAND_INIT OutRandInit;  ///< Output random padding seed
	BOOL bInBurst;                  ///< Dma input burst length
	UINT32 *puiQTbl;                ///< Pointer of Q table
	RDE_LINE_MODE_DBG LineModeDbg;        ///< Line mode debug control
} RDE_INT_PARAM;

extern ER rde_setMode(RDE_PARAM *pRdeParam, RDE_INT_PARAM *pRdeIntParam);// for verif HW
#else
extern ER rde_setMode(RDE_PARAM *pRdeParam);
#endif

//function setting
extern ER rde_setLineStrpDMAInInfo(UINT32 uiBsiSAddr, UINT32 uiBsiLofst, RDE_STRP_ID StrpId);
extern ER rde_getLineStrpDMAInInfo(UINT32 *puiBsiSAddr, UINT32 *puiBsiLofst, RDE_STRP_ID StrpId);
extern ER rde_getFrameDMAInInfo(UINT32 *puiBsiSAddr, UINT32 *puiBsiLength);
extern ER rde_getFrameDMAOutInfo(UINT32 *puiImgoSAddr, UINT32 *puiImgoLofst);
extern ER rde_chgQInit(UINT32 uiQInit);
extern ER rde_chgCropParam(RDE_CROP_PARAM *pCropParam, BOOL bMstrpEn);
extern ER rde_chgActWidth(UINT32 uiWidth);
extern UINT32 rde_getClockRate(void);
extern VOID rde_getInBurstSetting(RDE_BURST_SEL *pRdeInBurst);

//engine control
extern ER rde_open(RDE_OPENOBJ *pRdeObjCB);//Available clock rate(MHz): 120, 160, 192, 240
//extern ER rde_setMode(RDE_PARAM *pRdeParam);
extern ER rde_start(void);
extern ER rde_pause(void);
extern ER rde_close(void);
extern ER rde_waitFrameEnd(void);
extern ER rde_clearFrameEndFlag(void);

#ifdef __cplusplus
}
#endif

#endif

//@}
