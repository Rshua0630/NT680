/**
    Public header file for SVM module.

    @file       svm_lib.h
    @ingroup    mIIPPSVM

    @brief

    How to start:\n
    1. svm_open\n
    2. svm_setMode\n
    3. svm_start\n


    How to stop:\n
    1. svm_pause\n
    2. svm_close\n

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _SVM_LIB_H
#define _SVM_LIB_H

#include "Type.h"
/**
    @addtogroup mIIPPSVM
*/
//@{

#define SVM_DMA_CACHE_HANDLE    (1)

/**
    Define SVM interrput enable.
*/
//@{
#define SVM_INTE_ROUTEND                0x00000001
#define SVM_INTE_DMAIN0END              0x00000002
#define SVM_INTE_DMAIN1END              0x00000004
#define SVM_INTE_DMAIN2END              0x00000008
#define SVM_INTE_ALL                    0x0000000f
//@}

/**
    Define SVM interrput status.
*/
//@{
#define SVM_INT_ROUTEND                0x00000001
#define SVM_INT_DMAIN0END              0x00000002
#define SVM_INT_DMAIN1END              0x00000004
#define SVM_INT_DMAIN2END              0x00000008
#define SVM_INT_ALL                    0x0000000f
//@}

/**
    @name Define SVM function enable.
*/
//@{
#define SVM_KER2_EN                 0x00010000  ///< Kernel2 function enable
#define SVM_CAL_EN                  0x00020000  ///< Svmcal function enable
#define SVM_RELU_EN                 0x00040000  ///< RELU function enable
#define SVM_ALL_EN                  0x00070000  ///< RELU function enable
//@}


/**
    SVM mode selection

    Select svm mode
*/
typedef enum {
	SVM_OPMODE_SVM              = 0,    ///< output SVM results
	SVM_OPMODE_M_LINEAR_SVM     = 1,    ///< output multi-linear svm results
	SVM_OPMODE_RELU             = 2,    ///< output RELU results
	SVM_OPMODE_FULLYCONN        = 3,    ///< output fully connected results
	SVM_OPMODE_USERDEFINE       = 4,    ///< all manual mode
	ENUM_DUMMY4WORD(SVM_OPMODE)
} SVM_OPMODE;

/**
    SVM Kernel Type
*/
//@{
typedef enum {
	SVM_Ker_Linear      = 0,
	SVM_Ker_Poly        = 1,
	SVM_Ker_RBF         = 2,
	SVM_Ker_Sigmoid     = 3,
	SVM_Ker_Inter       = 4,
	ENUM_DUMMY4WORD(SVM_KER_TYPE)
} SVM_KER_TYPE;
//@}

/*
    SVM RELU Type
*/
typedef enum {
	SVM_RELU_ZERO       = 0,
	SVM_RELU_ABS        = 1,
	ENUM_DUMMY4WORD(SVM_RELU_TYPE)
} SVM_RELU_TYPE;

/**
    SVM Input Buffer ID
    SVM Input buffer ID.
*/
//@{
typedef enum {
	SVM_F_BUF       = 0, ///< input buffer 0
	SVM_SV_BUF      = 1, ///< input buffer 1
	SVM_ALPHA_BUF   = 2, ///< input buffer 2
	SVM_RHO_BUF     = 3, ///< input buffer 3
	ENUM_DUMMY4WORD(SVM_IN_BUFID)
} SVM_IN_BUFID;
//@}

/**
    SVM Open Object.

    Open Object is used for opening SVM module.
    \n Used for svm_open()

*/
//@{
typedef struct _SVM_OPENOBJ {
	void (*FP_SVMISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 uiSvmClockSel; ///< support 220/192/160/120/80/60/48 Mhz
} SVM_OPENOBJ;
//@}

/**
    SVM SV Size.

    Structure of size in SV.
*/
//@{
typedef struct _SVM_SV_SIZE {
	UINT32 uiWidth;         ///< sv width
	UINT32 uiHeight;        ///< sv Height
} SVM_SV_SIZE;
//@}

/**
    SVM Kernel Parameters.

    Structure of information in SVM kernel parameters.
*/
//@{
typedef struct _SVM_KERL_PARM {
	UINT32 uiGV;                        ///< input gamma value
	UINT32 uiGV_Shift;                  ///< gamma format; 0: 10 bits, 1: 7 bits, 2: 4 bits, 3: 0 bits for fraction
	INT32  iCoef;                       ///< input coefficient value
	UINT32 uiDegree;                    ///< input polynomial degree
	INT32  iRho;                        ///< input rho value
	UINT32 uiRho_fmt;                   ///< rho format; 0: 7 bits, 1: 6 bits, 2: 5 bits, 3: 4 bits for fraction
	UINT32 uiAlpha_shift;               ///< alpha format; 0: 10 bits, 1: 7 bits, 2: 4 bits, 3: 0 bits for fraction
} SVM_KERL_PARM;
//@}

/**
    SVM DRAM input and output addresses.

    Structure of information in DRAM addresses.
*/
//@{
typedef struct _SVM_DMAIO_ADDR {
	UINT32 uiInAddF;                    ///< input starting address of feature
	UINT32 uiInAddSV;                   ///< input starting address of SV
	UINT32 uiInAddAlpha;                ///< input starting address of alpha
	UINT32 uiInAddRho;                  ///< input starting address of rho
	UINT32 uiOutAdd;                    ///< output starting address
} SVM_DMAIO_ADDR;
//@}

/*
    SVM Kernel1 Type
*/
typedef enum {
	SVM_KER1_DOT    = 0,
	SVM_KER1_RBF    = 1,
	SVM_KER1_INTER  = 2,
	ENUM_DUMMY4WORD(SVM_KER1_TYPE)
} SVM_KER1_TYPE;

/*
    SVM Kernel2 Type
*/
typedef enum {
	SVM_KER2_POLY       = 0,
	SVM_KER2_SIGMOID    = 1,
	ENUM_DUMMY4WORD(SVM_KER2_TYPE)
} SVM_KER2_TYPE;

/*
    SVM Result Type
*/
typedef enum {
	SVM_RST_FULL        = 0,
	SVM_RST_SUB_RHO     = 1,
	ENUM_DUMMY4WORD(SVM_RST_TYPE)
} SVM_RST_TYPE;

/*
    SVM DRAM Output Path
*/
typedef enum {
	SVM_OUT_RST         = 0,
	SVM_OUT_RELU        = 1,
	SVM_OUT_KERL2       = 2,
	SVM_OUT_KERL1       = 3,
	ENUM_DUMMY4WORD(SVM_DMAO_PATH_TYPE)
} SVM_DMAO_PATH_TYPE;

/*
    RELU DRAM Output Selection
*/
typedef enum {
	SVM_RELU12          = 0,
	SVM_RELU22          = 1,
	ENUM_DUMMY4WORD(SVM_RELU_DMAOSEL_TYPE)
} SVM_RELU_DMAOSEL_TYPE;

/**
    SVM Results.

    Structure of information in results.
*/
//@{
typedef struct _SVM_RSTS {
	INT32 iRST0;                        ///< output result 0
	INT32 iRST1;                        ///< output result 1
	INT32 iRST2;                        ///< output result 2
	INT32 iRST3;                        ///< output result 3
	INT32 iRST4;                        ///< output result 4
	INT32 iRST5;                        ///< output result 5
	INT32 iRST6;                        ///< output result 6
	INT32 iRST7;                        ///< output result 7
	INT32 iRST8;                        ///< output result 8
	INT32 iRST9;                        ///< output result 9
	INT32 iRST10;                       ///< output result 10
	INT32 iRST11;                       ///< output result 11
	INT32 iRST12;                       ///< output result 12
	INT32 iRST13;                       ///< output result 13
	INT32 iRST14;                       ///< output result 14
} SVM_RSTS;
//@}

/*
    User Define

*/
typedef struct _SVM_USER_DEF {
	UINT32 uiKerlEn;                    ///< kernel enable
	SVM_KER1_TYPE Kerl1Type;            ///< kernel1 type
	SVM_KER2_TYPE Kerl2Type;            ///< kernel2 type
	SVM_RST_TYPE Rst_Type;              ///< Result type
	BOOL bDMAOEn;                       ///< DMA output enable
	SVM_DMAO_PATH_TYPE DMAO_Path;       ///< DMA output path
} SVM_USER_DEF;

/**
    SVM Entire Configuration

    Structure of SVM module parameters when one wants to configure this module.
*/
//@{
typedef struct _SVM_PARAM_STRUCT {
	SVM_KER_TYPE SVMKerType;            ///< input SVM kernel type
	SVM_RELU_TYPE RELUType;             ///< input RELU type
	SVM_RELU_DMAOSEL_TYPE RELU_DMAOSel; ///< RELU DMA output selection
	SVM_SV_SIZE SVSize;                 ///< input sv size
	UINT32 uiF_Num;                     ///< input number of features
	BOOL bF_RFH;                        ///< input feature refresh
	SVM_KERL_PARM SvmKerlParms;         ///< input svm kernel parameters
	SVM_DMAIO_ADDR DMAIO_Addr;          ///< DRAM input and output addresses
	SVM_USER_DEF userDef;               ///< input user define
} SVM_PARAM;
//@}


//ISR
extern void     svm_isr(void);
//basic function
extern ER       svm_open(SVM_OPENOBJ *pObjCB);
extern BOOL     svm_isOpened(void);
extern ER       svm_close(void);
extern ER       svm_start(void);
extern ER       svm_pause(void);
extern ER       svm_setMode(SVM_OPMODE Mode, SVM_PARAM *pSvmParms);
extern BOOL     svm_isEnabled(void);
extern void     svm_clrDMAIN0END(void);
extern void     svm_clrDMAIN1END(void);
extern void     svm_clrDMAIN2END(void);
extern void     svm_clrROutEnd(void);
//interrupt
extern ER       svm_changeInterrupt(UINT32 uiIntEn);
extern UINT32   svm_getIntEnable(void);
extern void     svm_clrIntrStatus(UINT32 uiStatus);
extern UINT32   svm_getIntrStatus(void);
//kernel information
extern SVM_KER_TYPE     svm_getKernel(void);
extern SVM_RELU_TYPE    svm_getRELUMode(void);
extern UINT32   svm_getF_Num(void);
extern BOOL     svm_getF_RFH(void);
extern void     svm_getSVSize(SVM_SV_SIZE *pSize);
extern void     svm_getParm(SVM_KERL_PARM *pParm);
extern void     svm_getRsts(SVM_RSTS *pRsts);
extern void     svm_getDMAIOAddr(SVM_DMAIO_ADDR *pAddr);
//get status
extern UINT32   svm_getClockRate(void);
extern SVM_RST_TYPE svm_getRstMode(void);
extern SVM_RELU_DMAOSEL_TYPE svm_getRELU_DMAOSel(void);
extern SVM_DMAO_PATH_TYPE svm_getDMAOPath(void);
extern BOOL svm_getDMAOEN(void);
//@}
#endif
