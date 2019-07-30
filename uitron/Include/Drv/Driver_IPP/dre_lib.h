/*
    Public header file for DRE module.

    This file is the header file that define the API and data type for DRE
    module.

    @file       dre_lib.h
    @ingroup    mIDrvIPPDRE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DRE_LIB_H
#define _DRE_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#define PAT_GEN 0

#if (!PAT_GEN)//disable for PatGen
#include    "Type.h"//a header for basic variable type
#else
#include    "../Type.h"//a header for basic variable type//for PatGen
#endif

/**
    @addtogroup mIDrvIPPDRE
*/
//@{

//please do not modify the definition here
#define PROC_MAX_NUM    10
#define F5X5_WT         6
#define HDR_IN_NUM      2
#define NR_IN_NUM       1
#define F3X3_WT         3
#define COLOR3          3
#define RTH_TAP         8
#define HDR_WT_TAP      17
#define GAMLUT_TAP      128
#define YCMOD_TAP       16
#define PYMD_MIN_WIDTH  5
#define PYMD_MIN_HEIGHT 5

//------enum------

/**
    Process type of DRE engine
*/
//@{
typedef enum {
	DRE_NR  = 0,
	DRE_HDR = 1
} PROC_TYPE;
//@}

/**
    Input image type of DRE engine
*/
//@{
typedef enum {
	DRE_YUV422_COMP = 0,    ///<Complete process of YUV422 pack input
	DRE_YUV422_SIMP = 1,    ///<Simplified process of YUV422 pack input
	DRE_YUV420      = 2     ///<Complete process of YUV420 pack input
} DRE_IMG_TYPE;
//@}

/**
    DRE input image ID
*/
typedef enum {
	IN_0 = 0,
	IN_1 = 1
} DRE_INIMG_ID;

/**
    Deghost mask bit number
*/
typedef enum {
	DG_8_BIT = 0,
	DG_4_BIT = 1
} DRE_DG_MSK_BIT_NUM;

//------struct-------

/**
    DRE image size parameter

    Structure of input image size
*/
//@{
typedef struct _DRE_IMG_SIZE_STRUCT {
	UINT32 uiWidth;
	UINT32 uiHeight;
} DRE_IMG_SIZE;
//@}

/**
    Dram info
*/
//@{
typedef struct _DRAM_INFO_STRUCT {
	UINT32 uiSAddr;
	UINT32 uiLofst;
} DRAM_INFO;
//@}

/**
    DRE input image DMA info

    Structure of input image DMA info
*/
//@{
typedef struct _DRE_IMG_DRAM_PARAM_STRUCT {
	DRAM_INFO ChY;
	DRAM_INFO ChUVp;
} DRE_IMG_DRAM_PARAM;
//@}

/**
    DRE HDR deghost function control

    Structure of DRE HDR deghost function
*/
//@{
typedef struct _DRE_HDR_DG_CTRL_STRUCT {
	BOOL bDgEn;                         ///<Enable of HDR deghost function
	BOOL bDgMskBitSel;                  ///<Selection of deghost mask bit length, only effective when bDgEn = ENABLE
	UINT32 uiDgLyrNum;                  ///<Deghost layer number
	DRAM_INFO DgMskIn[PROC_MAX_NUM];    ///<Deghost mask address, only effective when bDgEn = ENABLE
} DRE_HDR_DG_CTRL;
//@}

/**
    DRE NR joint-bilateral filter info

    Structure of one image NR spatial filter

    5x5:
    w5 w4 w3 w4 w5
    w4 w2 w1 w2 w4
    w3 w1 w0 w1 w3
    w4 w2 w1 w2 w4
    w5 w4 w3 w4 w5
*/
//@{
typedef struct _DRE_NR_FILT_STRUCT {
	UINT32 uiWt[F5X5_WT];   ///< Spatial weight
	UINT32 *puiRngThrY;     ///< Table of range filter threshold, please assign a UINT32[RTH_TAP] array
	UINT32 *puiRngThrU;     ///< Table of range filter threshold, please assign a UINT32[RTH_TAP] array
	UINT32 *puiRngThrV;     ///< Table of range filter threshold, please assign a UINT32[RTH_TAP] array
} DRE_NR_FILT;
//@}

/**
    DRE NR Gamma LUT info

    Structure of NR Gamma LUT parameters
*/
//@{
typedef struct _DRE_NR_GAM_LUT_STRUCT {
	UINT32 *puiLutY;    ///< NR gamma table for Y, please assign a UINT32[GAMLUT_TAP] array
	UINT32 *puiLutU;    ///< NR gamma table for U, please assign a UINT32[GAMLUT_TAP] array
	UINT32 *puiLutV;    ///< NR gamma table for V, please assign a UINT32[GAMLUT_TAP] array
} DRE_NR_GAM_LUT;
//@}

/**
    DRE YC modify LUT info

    Structure of YC modify table
*/
//@{
typedef struct _DRE_YCMOD_TBL_STRUCT {
	UINT32 *puiYcLutY;      ///< Table of YC modify LUT for Y, please assign a UINT32[YCMOD_TAP] array
	UINT32 *puiYcLutC;      ///< Table of YC modify LUT for C, please assign a UINT32[YCMOD_TAP] array
} DRE_YCMOD_TBL;
//@}

/**
    DRE YC modify control info

    Structure of YC modify control parameters
*/
//@{
typedef struct _DRE_YCMOD_CTRL_STRUCT {
	UINT32 uiCbOfs;
	UINT32 uiCrOfs;
	UINT32 uiStepY;
	UINT32 uiStepC;
	BOOL bModSel;
	BOOL bModLutSel;
} DRE_YCMOD_CTRL;
//@}

/**
    DRE engine parameter for HDR library

    Structure of all avaliable HDR parameter
*/
//@{
typedef struct _DRE_HDR_PARAM_STRUCT {
	DRE_IMG_TYPE ImgType;
	DRE_IMG_SIZE ImgInSz;
	UINT32 uiWorkBufSAddr;
	DRE_IMG_DRAM_PARAM In[HDR_IN_NUM];
	DRE_IMG_DRAM_PARAM Out;
	DRE_HDR_DG_CTRL DgCtrl;
	BOOL bHardWtEn;
    UINT32 uiLayerNum;
} DRE_HDR_PARAM;
//@}

/**
    DRE engine parameter for NR library

    Structure of all avaliable NR parameter
*/
//@{
typedef struct _DRE_NR_PARAM_STRUCT {
	DRE_IMG_TYPE ImgType;
	DRE_IMG_SIZE ImgInSz;
	UINT32 uiWorkBufSAddr;
	UINT32 uiProcRepNum;//min=1, max=9
	DRE_IMG_DRAM_PARAM In;
	DRE_IMG_DRAM_PARAM Out;
	BOOL bNrGamYEn[PROC_MAX_NUM];
	BOOL bNrGamUEn[PROC_MAX_NUM];
	BOOL bNrGamVEn[PROC_MAX_NUM];
	DRE_NR_FILT NrJBFilt[PROC_MAX_NUM];
	DRE_NR_GAM_LUT NrGam[PROC_MAX_NUM];
	DRE_YCMOD_TBL YCMTbl[PROC_MAX_NUM];
	DRE_YCMOD_CTRL YCMCtrl[PROC_MAX_NUM];
} DRE_NR_PARAM;
//@}

/**
    function declaration
*/
extern UINT32 dre_getHdrWorkBufSize(DRE_IMG_SIZE *pImgInSz, DRE_IMG_TYPE ImgInType);
extern ER dre_HdrProcess(DRE_HDR_PARAM *pDreHdrParam);

extern UINT32 dre_getNrWorkBufSize(DRE_IMG_SIZE *pImgInSz, UINT32 uiProcRepNum, DRE_IMG_TYPE ImgInType);
extern ER dre_NrProcess(DRE_NR_PARAM *pDreNrParam);

extern VOID dre_setDbgDumpReg(BOOL bDbgDumpEn, UINT32 uiRegSAddr);//for simulation tool

extern UINT32 g_uiDreFrmEndIntCnt; //sc

//-----------------------------------------
#if 1 //For HW verif, copy this part to dre_lib.h and disable here

#define DREIN_NUM   4
#define DREOUT_NUM  3

/**
    Define DRE interrput enable.
*/
//@{
#define DRE_INTE_DISABLE_ALL    0x00000000  ///< Disable all interrupt
#define DRE_INTE_FRMEND         0x00000001  ///< Enable interrupt of frame end
#define DRE_INTE_TIMEOUTCHK     0x00000002  ///< Enable interrupt of time out
#define DRE_INTE_ALL            0x00000003  ///< Enable all interrupt

//@}

/**
    Define DRE interrput status.
*/
//@{
#define DRE_INT_FRMEND          0x00000001  ///< Status of interrupt of frame end
#define DRE_INT_TIMEOUTCHK      0x00000002  ///< Enable interrupt of time out
#define DRE_INT_ALL             0x00000003  ///< Enable all interrupt

//@}

/*----------- engine control enumeration ------------*/
/**
    State of State Machine
*/
//@{
typedef enum {
	DRE_ENGINE_IDLE  = 0, ///< Idel state
	DRE_ENGINE_READY = 1, ///< Ready state
	DRE_ENGINE_PAUSE = 2, ///< Pause state
	DRE_ENGINE_RUN   = 3, ///< Run state

	ENUM_DUMMY4WORD(DRE_STATE_MACHINE)
} DRE_STATE_MACHINE;
//@}

/**
    Operations of engine action
*/
//@{
typedef enum {
	DRE_OP_OPEN      = 0, ///< Open engine
	DRE_OP_CLOSE     = 1, ///< Close engine
	DRE_OP_SETPARAM  = 2, ///< Set parameter
	DRE_OP_SET2READY = 3, ///< Set to ready
	DRE_OP_START     = 4, ///< Start engine
	DRE_OP_PAUSE     = 5, ///< Pause engine

	ENUM_DUMMY4WORD(DRE_ACTION_OP)
} DRE_ACTION_OP;
//@}

/**
    Engine load type
*/
//@{
typedef enum {
	DRE_START_LOAD  = 0, ///< Start load, available
	DRE_FRMEND_LOAD = 1, ///< Frame end load, not available
	DRE_IMD_LOAD    = 2, ///< Immediately load, not available

	ENUM_DUMMY4WORD(DRE_LOAD_TYPE)
} DRE_LOAD_TYPE;
//@}

/**
    Color image channel name
*/
//@{
typedef enum {
	Y_CH = 0,
	U_CH = 1,
	V_CH = 2
} YUV_CH_NAME;
//@}

/**
    engine process type
*/
//@{
typedef enum {
	DCM_PROC = 0,
	RCS_PROC = 1
} DRE_PROC_TYPE;
//@}

/**
    Engine decomposition mode
*/
//@{
typedef enum {
	DCM_MODE0 = 0, ///< Decomposition mode 0
	DCM_MODE1 = 1, ///< Decomposition mode 1
	DCM_MODE2 = 2, ///< Decomposition mode 2
	DCM_MODE3 = 3, ///< Decomposition mode 3
	DCM_MODE4 = 4, ///< Decomposition mode 4

	ENUM_DUMMY4WORD(DRE_DCM_MODE)
} DRE_DCM_MODE;
//@}

/**
    Engine reconstruction mode
*/
//@{
typedef enum {
	RCS_MODE0 = 0, ///< Reconstruction mode 0
	RCS_MODE1 = 1, ///< Reconstruction mode 1
	RCS_MODE2 = 2, ///< Reconstruction mode 2
	RCS_MODE3 = 3, ///< Reconstruction mode 3
	RCS_MODE4 = 4, ///< Reconstruction mode 4

	ENUM_DUMMY4WORD(DRE_RCS_MODE)
} DRE_RCS_MODE;
//@}

/**
    Engine input image number
*/
//@{
typedef enum {
	ONE_IMG = 0, ///< One input image of decomposition
	TWO_IMG = 1, ///< Two input image of decomposition

	ENUM_DUMMY4WORD(DRE_DCM_IN_NUM)
} DRE_DCM_IN_NUM;
//@}

/*
    Deghost mask length
*/
//@{
typedef enum {
	MSK_8BIT = 0, ///< 8 bit deghost mask
	MSK_4BIT = 1, ///< 4 bit deghost mask

	ENUM_DUMMY4WORD(DRE_DG_MSK_BIT_SEL)
} DRE_DG_MSK_BIT_SEL;
//@}

/*
    dram in/out burst length
*/
//@{
typedef enum {
	DRE_BURST_64W = 0, ///< burst length 64 word
	DRE_BURST_48W = 1, ///< burst length 48 word
	DRE_BURST_32W = 2, ///< burst length 32 word
	DRE_BURST_16W = 3, ///< burst length 16 word

	ENUM_DUMMY4WORD(DRE_BURST_LEN)
} DRE_BURST_LEN;
//@}

/*----------- engine driver structure -------------*/
/*
    DRE Open Object - Open Object is used for opening DRE module.
*/
typedef struct _DRE_OPENOBJ {
	VOID (*FP_DREISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 DRE_CLOCKSEL;
} DRE_OPENOBJ;

/**
    DRE decomposition function setting

    Structure of settings of decomposition function
*/
//@{
typedef struct _DRE_DCM_FUNC_EN_STRUCT {
	BOOL bImgiNum;      ///< Enable one or two input image under decomposition process
	UINT32 uiDMode;     ///< Select mode under decomposition process
	BOOL bDgEn;         ///< Enable deghost function under decomposition process
	BOOL bDgMskBitSel;  ///< Select deghost mask bit under decomposition process
	BOOL bHardWtEn;     ///< Enable hard weighting
	BOOL bFiltEn;       ///< Enable filter under decomposition process
	BOOL bNrLutYEn;     ///< Enable noise reduction LUT Y under decomposition process
	BOOL bNrLutUEn;     ///< Enable noise reduction LUT U under decomposition process
	BOOL bNrLutVEn;     ///< Enable noise reduction LUT V under decomposition process
} DRE_DCM_FUNC_EN;

/**
    DRE reconstruction function setting

    Structure of settings of reconstruction function
*/
//@{
typedef struct _DRE_RCS_FUNC_EN_STRUCT {
	UINT32 uiRMode;   ///< Select mode under reconstruction process
	BOOL bYCModEn;    ///< Enable YC modification under reconstruction process
} DRE_RCS_FUNC_EN;

/**
    DRE output setting

    Structure of settings of output function
*/
//@{
typedef struct _DRE_FUNC_EN_STRUCT {
	BOOL bProcSel;          ///< Enable decomposition or reconstruction process
	DRE_DCM_FUNC_EN Dcm;    ///< Function settings of decomposition proecss
	DRE_RCS_FUNC_EN Rcs;    ///< Function settings of reconstruction proecss
} DRE_FUNC_EN;
//@}

/**
    DRE DMA in info

    Structure of setting of DMA input
*/
//@{
typedef struct _DRE_DRAM_IN_INFO_STRUCT {
	UINT32 uiInAddr[DREIN_NUM];   ///< Dram address of image input
	UINT32 uiInLofst[DREIN_NUM];  ///< Dram line offset of image input
	UINT32 uiDgMskAddr;         ///< Dram address of deghost mask input
	UINT32 uiDgMskLofst;        ///< Dram line offset of deghost mask input
} DRE_DRAM_IN_INFO;
//@}

/**
    DRE DMA out info

    Structure of setting of DMA output
*/
//@{
typedef struct _DRE_DRAM_OUT_INFO_STRUCT {
	UINT32 uiOutAddr[DREOUT_NUM];  ///< Dram address of image output
	UINT32 uiOutLofst[DREOUT_NUM]; ///< Dram line offset of image output
} DRE_DRAM_OUT_INFO;
//@}

/**
    DRE image size info

    Structure of input image size
*/
//@{
typedef struct _DRE_LAYER_SIZE_STRUCT {
	UINT32 uiL0Width;   ///< Width of layer 0 image
	UINT32 uiL0Height;  ///< height of layer 0 image
	UINT32 uiL1Width;   ///< Width of layer 1 image
	UINT32 uiL1Height;  ///< height of layer 1 image
} DRE_LAYER_SIZE;
//@}

/**
    DRE image scaling factor

    Structure of image scaling factor
*/
//@{
typedef struct _DRE_SCALE_FACT_STRUCT {
	UINT32 uiHDnFact;   ///< scaling down factor of horizontal direction
	UINT32 uiVDnFact;   ///< scaling down factor of vertical direction
	UINT32 uiHUpFact;   ///< scaling up factor of horizontal direction
	UINT32 uiVUpFact;   ///< scaling up factor of vertical direction
} DRE_SCALE_FACT;
//@}

/**
    DRE image scaling offset

    Structure of scaling factor offset
*/
//@{
typedef struct _DRE_SCALE_OFST_STRUCT {
	UINT32 uiHDnOfst;  ///< offset of horizontal scaling down factor
	UINT32 uiVDnOfst;  ///< offset of vertical scaling down factor
} DRE_SCALE_OFST;
//@}

/*
    DRE HDR spatial filter info

    Structure of two image HDR spatial filter

    3x3:
    w2 w1 w2
    w1 w0 w1
    w2 w1 w2
*/
//@{
typedef struct _DRE_HDR_FILT_STRUCT {
	UINT32 uiWt[F3X3_WT];  ///< Spatial weight
} DRE_HDR_FILT;
//@}

/*
    DRE Joint bilateral reference & outlier control
*/
//@{
typedef struct _DRE_JOINT_OUTL_STRUCT {
	BOOL bJointYEn[COLOR3];  ///< Enable of joint channel reference
	BOOL bJointUEn[COLOR3];  ///< Enable of joint channel reference
	BOOL bJointVEn[COLOR3];  ///< Enable of joint channel reference
	BOOL bOutlrEn[COLOR3];   ///< Enable of outlier detection of each channel
} DRE_JOINT_OUTL;
//@}

/*
    DRE scaling factor info
    Structure of scaling factor info
*/
typedef struct _DRE_INT_PARAM_STRUCT {
	UINT32 uiDreClk;                        ///< Clock rate of DRE
	UINT32 uiRunTime;                       ///< clock count of DRE
	DRE_FUNC_EN FuncEn;                     ///< Enable function of DRE
	DRE_DRAM_IN_INFO DramIn;              ///< Info of DMA input
	DRE_DRAM_OUT_INFO DramOut;            ///< Info of DMA output
	DRE_LAYER_SIZE ImgSize;                 ///< Size info of output image
	DRE_SCALE_FACT ScFact;                  ///< Scaling factor
	DRE_SCALE_OFST ScOfst;                  ///< Scaling factor offset
	DRE_HDR_FILT HdrSFilt[2];               ///< DCM Spatial filter parameters of two input HDR
	UINT32 *puiHdrWtTbl;                    ///< DCM Pointer of HDR weight table, please assign a UINT32[17] array
	DRE_BURST_LEN *pBurstLenSel;            ///< burst length selection
	DRE_NR_FILT NrJBFilt;                   ///< DCM Joint bilateral filter parameters of one input NR
	DRE_NR_GAM_LUT NrGam;                   ///< DCM Pointer of NR gamma curve LUT, please assign three UINT32[128] array
	DRE_YCMOD_TBL YCMTbl;                   ///< RCS YC modify table
	DRE_YCMOD_CTRL YCMCtrl;                 ///< RCS YC modify control
	DRE_JOINT_OUTL JntChOutl;               ///< DCM NR joint bilateral channel & outlier parameters
} DRE_INT_PARAM;

//fixed LUT
extern UINT32 g_DivMul[1024];

//default settings
extern UINT32 g_NrRngTbl[COLOR3][RTH_TAP];
extern DRE_NR_FILT g_DfltNrFilt;
extern DRE_JOINT_OUTL g_DfltJntChOutl;
extern UINT32 g_uiDfltNrGamLutTbl[COLOR3][GAMLUT_TAP];
extern UINT32 g_uiDfltNrGamLutYTbl[GAMLUT_TAP];
extern UINT32 g_uiDfltNrGamLutUTbl[GAMLUT_TAP];
extern UINT32 g_uiDfltNrGamLutVTbl[GAMLUT_TAP];
extern UINT32 g_uiDfltYcLutY[YCMOD_TAP];
extern UINT32 g_uiDfltYcLutC[YCMOD_TAP];
extern UINT32 g_DfltHdrWtLut[HDR_WT_TAP];
extern DRE_HDR_FILT g_DfltHdrFilt;

//engine control
extern ER dre_setReset(void); //SC //VOID-> void
extern ER dre_open(DRE_OPENOBJ *pDreObjCB); //Available clock rate(MHz):192, 240, PLL6, PLL13
extern ER dre_setMode(DRE_INT_PARAM *pDreIntPar);
extern ER dre_start(void);
extern ER dre_pause(void);
extern ER dre_close(void);
extern ER dre_waitFrameEnd(void);
extern ER dre_clearFrameEndFlag(void);

//get information
extern ER dre_getDMAInParam(UINT32 *puiInAddr, UINT32 *puiLofsi);
extern ER dre_getDMADgMskInParam(UINT32 *puiDgMskInAddr, UINT32 *puiDgMskLofsi);
extern ER dre_getDMAOutParam(UINT32 *puiOutAddr, UINT32 *puiLofso);
#endif//DRE_DRV_REL_EN

#ifdef __cplusplus
}
#endif

#endif

//@}
