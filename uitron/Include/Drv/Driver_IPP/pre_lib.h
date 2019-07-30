/**
    Public header file for PRE module.

    @file       pre_lib.h
    @ingroup    mIDrvIPP_PRE

    @brief

    How to start:\n
    [Preview]\n
    1. pre_attach\n
    2. pre_open\n
    3. PRE_Operation = PRE_ENGINE_SET2PRV, pre_setMode\n
    4. PRE_Operation = PRE_ENGINE_SET2RUN, pre_setMode\n

    [Preview to Capture]\n
    1. PRE_operation = PRE_ENGINE_SET2PAUSE, pre_setMode\n
    2. PRE_Operation = PRE_ENGINE_SET2CAP, pre_setMode\n
    3. PRE_Operation = PRE_ENGINE_SET2RUN, pre_setMode\n
    4. PRE_operation = PRE_ENGINE_SET2PAUSE, pre_setMode\n


    How to stop:\n
    1. PRE_operation = PRE_ENGINE_SET2PAUSE, pre_setMode\n
    2. pre_close\n
    3. pre_detach\n


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _PRE_LIB_H
#define _PRE_LIB_H

#define _PRE_LIB_H_FOR_FW_ (1)
#define _PRE_LIB_H_FOR_GEARGEN_ (0)

#if _PRE_LIB_H_FOR_FW_
// include files for FW
#include "Type.h"
#elif _PRE_LIB_H_FOR_GEARGEN_
// include files for GearGen
#include    "..\\type.h"
#endif//_PRE_LIB_H_FOR_GEARGEN_

/**
    @addtogroup mIDrvIPP_PRE
*/
//@{

/**
PRE Definition

@name   PRE_Define
*/
//@{

/*
    Define PRE function enable.
*/
//@{
#define PRE_DFS_EN                          0x00000002  ///< enable function: dark frame subtraction
#define PRE_HDIV_ARR_EN                     0x00000004  ///< enable function: Horizontal division arrangement
#define PRE_EMB_DPC_EN                      0x00000008  ///< enable function: Embedded defect pixel concealment
//#define PRE_CTC_EN                          0x00000010  ///< enable function: Cross-Talk compensation
#define PRE_VIG_EN                          0x00000020  ///< enable function: Vignette shading compensation
#define PRE_COFS_EN                         0x00000040  ///< enable function: Color-Offset compensation
//@}

/*
    Define PRE interrput enable.
*/
//@{
#define PRE_INTE_FRMST                      0x00000001  ///< enable interrupt: frame start
#define PRE_INTE_FRMEND                     0x00000002  ///< enable interrupt: frame end
//removed and would be masked-out//#define PRE_INTE_HSTPEND                    0x00000004  ///< enable interrupt: Horizontal stripe end
//removed and would be masked-out//#define PRE_INTE_DPCWARN                    0x00000008  ///< enable interrupt: Defect concealment warning
//removed and would be masked-out//#define PRE_INTE_START_LAG                  0x00000010  ///< enable interrupt: start lag
#define PRE_INTE_ALL                        0xffffffff  ///< enable interrupt: All interrupt
//@}

/*
    Define PRE interrput status.
*/
//@{
#define PRE_INT_FRMST                       0x00000001  ///< enable status: frame start
#define PRE_INT_FRMEND                      0x00000002  ///< enable status: frame end
#define PRE_INT_HSTPEND                     0x00000004  ///< enable status: Horizontal stripe end
#define PRE_INT_DPCWARN                     0x00000008  ///< enable status: Defect concealment warning
#define PRE_INT_START_LAG                   0x00000010  ///< enable status: start lag
#define PRE_INT_ALL                         0xffffffff  ///< enable status: All interrupt
//@}

//@}

/**
PRE Enumeration

@name   PRE_Enumeration
*/
//@{

typedef enum {
	PRE_CTRL_SEL_D2D = 0,         ///< Dram-PRE-Dram
	PRE_CTRL_SEL_DIRECT1 = 1,          ///< Dram-PRE-IFE-Dram
	PRE_CTRL_SEL_DIRECT2 = 2,          ///< Dram-PRE-IFE-IPE/DCT-IME-Dram
	ENUM_DUMMY4WORD(PRE_CTRL_SEL)
} PRE_CTRL_SEL;
//@}


typedef enum {
	PRE_TRIG_SEL_SIE = 0,         ///< SIE Trigger
	PRE_TRIG_SEL_FW = 1,          ///< FW Trigger
	ENUM_DUMMY4WORD(PRE_TRIG_SEL)
} PRE_TRIG_SEL;
//@}



typedef enum {
	PRE_CFA_R = 0, ///< start pixel is R
	PRE_CFA_Gr = 1, ///< start pixel is GR
	PRE_CFA_Gb = 2, ///< start pixel is GB
	PRE_CFA_B = 3, ///< start pixel is B
	ENUM_DUMMY4WORD(PRE_CFAPAT)
} PRE_CFAPAT;
//@}


typedef enum {
	PRE_BUF_NUM_1 = 0, ///< one buffer
	PRE_BUF_NUM_2 = 1,///< two ping-pong buffer
	ENUM_DUMMY4WORD(PRE_BUF_NUM)
} PRE_BUF_NUM;
//@}

typedef enum {
	PRE_PACKBUS_8 = 0, ///< 8 bits packing
	PRE_PACKBUS_10 = 1, ///< 10 bits packing
	PRE_PACKBUS_12 = 2, ///< 12 bits packing
	PRE_PACKBUS_16 = 3, ///< 16 bits packing
	ENUM_DUMMY4WORD(PRE_PACKBUS)
} PRE_PACKBUS;
//@}

typedef enum {
	PRE_DFS_OP_B_MINUS_A = 0, ///< B - A
	PRE_DFS_OP_A_MINUS_B = 1,///< A - B
	PRE_DFS_OP_A_PLUS_B = 2, ///< A + B
	PRE_DFS_OP_A_MUL_B_SHIFT = 3, ///< (A * B) >> shift
	ENUM_DUMMY4WORD(PRE_DFS_OP)
} PRE_DFS_OP;
//@}

typedef enum {
	PRE_DFS_THSOURCE_A = 0, ///< A as the source for threshold
	PRE_DFS_THSOURCE_B = 1,///< B as the source for threshold
	ENUM_DUMMY4WORD(PRE_DFS_THSOURCE)
} PRE_DFS_THSOURCE;
//@}

/**
    @name PRE enum - Change Funciton Parameter Selection
*/
//@{
typedef enum {
	PRE_CHG_SIZE       = 0,  ///< change input and output image size, refer to structure "PRE_CHG_SIZE_INFO"
	PRE_CHG_IO_ADDR       = 1,  ///< change input and output DMA Address, refer to structure "PRE_CHG_IO_ADDR_INFO"
	PRE_CHG_DFS           = 2,  ///< change dark frame subtraction parameters, refer to "PRE_DFS_INFO"
	PRE_CHG_HDIV          = 3,  ///< change horizontal division, refer to "PRE_DIV_INFO"
	PRE_CHG_EMBDPC        = 4,  ///< change defect concealment, refer to structure "PRE_DPC_INFO"
	PRE_CHG_CTC           = 5,  ///< change cross-talk compensation, refer to structure "PRE_CTC_INFO"
	PRE_CHG_VIG           = 6,  ///< change vignette shading compensation, refer to structure "PRE_VIG_INFO"
	PRE_CHG_COFS          = 7,  ///< change color offset, refer to structure "PRE_CSP_INFO"
	ENUM_DUMMY4WORD(PRE_CHANGE_FUN_PARAM_SEL)
} PRE_CHANGE_FUN_PARAM_SEL;
//@}

//@}

/**
PRE Struct

@name   PRE_Struct
*/
//@{

typedef struct {
	BOOL                   bDfsEn;        ///< dark frame subtraction - enable function
	PRE_DFS_OP             DfsOp;           ///< dark frame subtraction - operation
	PRE_DFS_THSOURCE       ThSource;     ///< dark frame subtraction - souce for threshold
	UINT32                 uiSubTh;    ///< dark frame subtraction - threshold value for operation
	BOOL                   bEmbDp;        ///< dark frame subtraction - DP symbol reservation
	UINT32                 uiShift;        ///< dark frame subtraction - shift value for operation
} PRE_DFS_INFO;
//@}


typedef struct {
	BOOL                   uiDivEn;        ///< division arrangement - enable function
	UINT32                 uiDivNum;       ///< division arrangement - division number
	UINT32                 uiDivId[10];    ///< division arrangement - division index array
} PRE_DIV_INFO;
//@}

typedef struct {
	BOOL                   bDpcEn;        ///< defect concealment - enable function
	UINT32                 uiDpMark;       ///< defect concealment - DP Mark value
	UINT32                 uiDefFact;      ///< defect concealment - concealment weighting
} PRE_DPC_INFO;
//@}

typedef struct {
//always enable//    BOOL                   ctcEn;        ///< cross talk compensation - enable function
	UINT32                 uiGainRH;       ///< cross talk compensation - R channel Horizontal gain
	UINT32                 uiGainRV;       ///< cross talk compensation - R channel Vertical gain
	UINT32                 uiGainRD;       ///< cross talk compensation - R channel Diagonal gain
	UINT32                 uiGainGrH;       ///< cross talk compensation - Gr channel Horizontal gain
	UINT32                 uiGainGrV;       ///< cross talk compensation - Gr channel Vertical gain
	UINT32                 uiGainGrD;       ///< cross talk compensation - Gr channel Diagonal gain
	UINT32                 uiGainGbH;       ///< cross talk compensation - Gb channel Horizontal gain
	UINT32                 uiGainGbV;       ///< cross talk compensation - Gb channel Vertical gain
	UINT32                 uiGainGbD;       ///< cross talk compensation - Gb channel Diagonal gain
	UINT32                 uiGainBH;       ///< cross talk compensation - B channel Horizontal gain
	UINT32                 uiGainBV;       ///< cross talk compensation - B channel Vertical gain
	UINT32                 uiGainBD;       ///< cross talk compensation - B channel Diagonal gain
} PRE_CTC_INFO;
//@}

typedef struct {
	BOOL   bVigEn;       ///< vig compensation - enable function
	BOOL   bVigDcOpEn;   ///< vig compensation - dc extraction enable
	UINT32 uiVigDcTh1;  ///< vig compensation - dc extraction threshold 1
	UINT32 uiVigDcTh2;  ///< vig compensation - dc extraction threshold 2
	UINT32 uiVigXR;      ///< vig compensation - x center of R
	UINT32 uiVigYR;      ///< vig compensation - y center of R
	UINT32 uiVigXGr;     ///< vig compensation - x center of GR
	UINT32 uiVigYGr;     ///< vig compensation - y center of GR
	UINT32 uiVigXGb;     ///< vig compensation - x center of GB
	UINT32 uiVigYGb;     ///< vig compensation - y center of GB
	UINT32 uiVigXB;      ///< vig compensation - x center of B
	UINT32 uiVigYB;      ///< vig compensation - y center of B
	UINT8 uiVigMul;      ///< vig compensation - vignette distance gain. unsigned 2.6
	UINT8 uiVigT;        ///< vig compensation - vignette disabled area
	UINT8 uiVigTabGain;  ///< vig compensation - vignette gain factor
	BOOL bVigDitherEn;  ///< vig compensation - vignette dithering enable
	BOOL bVigDitherRst;   ///< vig compensation - dc extraction enable
	//unnucessary after fld-arr//UINT8 uiYDist; ///< vignette Y subsampling factor
	UINT32 uiVigXDiv;    ///< vig compensation - vignette scaling factor in x direction. unsigned 0.6
	UINT32 uiVigYDiv;    ///< vig compensation - vignette scaling factor in y direction. unsigned 0.6
	UINT8 *puiVigLutR;   ///< vig compensation - vignette LUT of R channel
	UINT8 *puiVigLutG;   ///< vig compensation - vignette LUT of G channel
	UINT8 *puiVigLutB;   ///< vig compensation - vignette LUT of B channel
} PRE_VIG_INFO;
//@}


typedef struct {
	UINT32              uiImgXSize;     ///< vig compensation - image horizontal size
	UINT32              uiImgYSize;     ///< vig compensation - image vertical size
	UINT32              uiImgXCntr;     ///< vig compensation - image horizontal center
	UINT32              uiImgYCntr;     ///< vig compensation - image vertical center
	UINT32              uiImgXratio;    ///< vig compensation - image horizontalratio (16 from 16:9)
	UINT32              uiImgYratio;    ///< vig compensation - image vertical ratio (9 from 16:9)
} PRE_VIG_SETTING_ONE_IMG;

typedef struct {
	UINT32              uiCaliImgSizeX;     ///< vig compensation - calibration image size X, in pixel
	UINT32              uiCaliImgSizeY;     ///< vig compensation - calibration image size Y, in pixel
	UINT32              uiCaliImgVigCntrX;  ///< vig compensation - calibration image center X, in pixel
	UINT32              uiCaliImgVigCntrY;  ///< vig compensation - calibration image center Y, in pixel
	UINT32              uiCaliImgRatioX;    ///< vig compensation - calibration image X:Y Ratio (ex: 16 for 16:9)
	UINT32              uiCaliImgRatioY;    ///< vig compensation - calibration image Y:Y Ratio (ex: 9  for 16:9)
	UINT32              uiCorrImgSizeX;     ///< vig compensation - correction  image size X, in pixel
	UINT32              uiCorrImgSizeY;     ///< vig compensation - correction  image size Y, in pixel
	UINT32              uiCorrImgSclRatioX; ///< vig compensation - correction  image Scaling (down) ratio X, in 1/1000 (ex: 500 for 1/2 scaling down)
	UINT32              uiCorrImgSclRatioY; ///< vig compensation - correction  image Scaling (down) ratio Y, in 1/1000 (ex: 500 for 1/2 scaling down)
	UINT32              uiCorrImgPosRatioX; ///< vig compensation - correction  image position ratio (compared to full image) X, in 1/1000 (ex: 500 for Nx Dzoom)
	UINT32              uiCorrImgPosRatioY; ///< vig compensation - correction  image position ratio (compared to full image) Y, in 1/1000 (ex: 500 for Nx Dzoom)
} PRE_VIG_SETTING_TWO_IMG;


typedef struct {
	BOOL                   bCofsEn;       ///< color offset - enable function
	UINT32                 uiCofsR;        ///< color offset - R channel offset
	UINT32                 uiCofsGr;       ///< color offset - Gr channel offset
	UINT32                 uiCofsGb;       ///< color offset - Gb channel offset
	UINT32                 uiCofsB;        ///< color offset - B channel offset
} PRE_COFS_INFO;
//@}





/**
    PRE Open Object.

    Open Object is used for opening PRE module.
    \n Used for pre_open()
*/
//@{
typedef struct _PRE_OPENOBJ {
	void (*pfPreIsrCb)(UINT32 uiIntpStatus); ///< isr callback function
	UINT32 uiPreClkSel;                    ///< Engine clock-rate in MHz; Selection: 48/60/80/120/160/192/220/240
} PRE_OPENOBJ;
//@}



typedef struct {
	UINT32                 uiSzH;       ///< horizontal size
	UINT32                 uiSzV;       ///< vertical size
} PRE_SIZE_INFO;

/**
    PRE Entire Configuration.

    Structure of entire PRE configuration.
*/
//@{
typedef struct {
	PRE_CTRL_SEL            CtrlSel; ///< PRE Control Selection
	PRE_TRIG_SEL            TrigSel; ///< PRE Trigger Selection
	UINT32                   uiFuncEn; ///< PRE Function Enable
	UINT32                   uiIntrpEn; ///< PRE Interrupt Enable

	PRE_SIZE_INFO            InSzInfo; ///< PRE input size
	PRE_CFAPAT                 InCfa; ///< PRE input cfa pattern

	UINT32                   uiIn0Add0; ///< PRE input ch0 dram addr 0
	UINT32                   uiIn0Add1; ///< PRE input ch0 dram addr 1
	UINT32                   uiIn0Lofs; ///< PRE input ch0 lineoffset
	PRE_BUF_NUM             In0BufNum; ///< PRE input ch0 buffer num
	PRE_PACKBUS             In0PackBus; ///< PRE input ch0 pack bus

	UINT32                   uiIn1Addr; ///< PRE input ch0 dram addr
	UINT32                   uiIn1Lofs; ///< PRE input ch0 lineoffset

	UINT32                   uiOutAdd; ///< PRE output dram addr
	UINT32                   uiOutLofs; ///< PRE output lineoffset
	PRE_PACKBUS             OutPackBus; ///< PRE output pack bus

	PRE_DFS_INFO             DfsInfo; ///< PRE dark frame subtraction info
	PRE_DIV_INFO             DivInfo; ///< PRE division arrangement info
	PRE_DPC_INFO             DpcInfo; ///< PRE defect concealment info
	PRE_CTC_INFO             CtcInfo; ///< PRE cross-talk compensation info
	PRE_VIG_INFO             VigInfo; ///< PRE vignette shading compensation info
	PRE_COFS_INFO            CofsInfo; ///< PRE color offset info

} PRE_MODE_PRAM;
//@}

/**
    PRE Partial Configuration for parameter change of IO Size

    Structure of IO Size PRE configuration.
*/
//@{
typedef struct {
	PRE_SIZE_INFO            InSzInfo; ///< PRE input size
	UINT32                   uiIn0Lofs; ///< PRE input ch0 lineoffset
} PRE_CHG_SIZE_INFO;
//@}

/**
    PRE Partial Configuration for parameter change of IO Size

    Structure of IO Size PRE configuration.
*/
//@{
typedef struct {
	UINT32                   uiIn0Add0; ///< PRE input ch0 dram addr 0
	UINT32                   uiIn0Add1; ///< PRE input ch0 dram addr 1
	UINT32                   uiIn1Addr; ///< PRE input ch0 dram addr 1
	UINT32                   uiOutAdd; ///< PRE output dram addr
	UINT32                   uiIn0Size; ///< PRE input ch0 dram size in byte, for cache-handle
	UINT32                   uiIn1Size; ///< PRE input ch0 dram size in byte, for cache-handle
	UINT32                   uiOutSize; ///< PRE input ch0 dram size in byte, for cache-handle
} PRE_CHG_IO_ADDR_INFO;
//@}

//@}

/**
PRE API

@name   PRE_API
*/
//@{


// pre_lib.c
//driver-policy: SWRST is limited//extern ER pre_reset(void);
extern ER pre_open(PRE_OPENOBJ *pObjCB);
extern BOOL pre_isOpened(void);
extern ER pre_close(void);
extern ER pre_pause(void);
extern ER pre_start(void);
extern ER pre_setMode(PRE_MODE_PRAM *pPreParam);
extern void pre_clrFrameEnd(void);
extern void pre_waitFrameEnd(BOOL bClrFlag);
extern ER pre_chgClock(UINT32 uiClkSel);
extern ER pre_chgParam(void *pParam, PRE_CHANGE_FUN_PARAM_SEL FunSel);
// pre_int.c
extern void pre_getInSize(UINT32 *puiHSize, UINT32 *puiVSize);
extern void pre_setIn0Add(UINT32 uiIndex, UINT32 uiAddr);
extern void pre_setCtrlSel(PRE_CTRL_SEL CtrlSel);
extern void pre_enableFunction(BOOL bEnable, UINT32 uiPreFunction);
extern void pre_setFunction(UINT32 uiPreFunction);
extern void pre_setInPackBus(PRE_PACKBUS PackBus);
extern PRE_PACKBUS pre_getInPackBus(void);
extern void pre_setOutPackBus(PRE_PACKBUS PackBus);
extern PRE_PACKBUS pre_getOutPackBus(void);
extern UINT32 pre_getOutAdd(void);
extern UINT32 pre_getIn0Add(UINT32 uiIndex);
extern UINT32 pre_getIn1Add(void);
extern void pre_setIn0Lofs(UINT32 uiLineOffset);
extern UINT32 pre_getIn0Lofs(void);
extern void pre_setIn1Lofs(UINT32 uiLineOffset);
extern UINT32 pre_getIn1Lofs(void);
extern void pre_setDiv(PRE_DIV_INFO *pDfsInfo);
extern void pre_getDiv(PRE_DIV_INFO *pDfsInfo);
extern void pre_setOutLofs(UINT32 uiLineOffset);
extern UINT32 pre_getOutLofs(void);
extern void pre_setReset(BOOL bReset);
extern void pre_setChgSize(PRE_CHG_SIZE_INFO *pChgSzInfo);
extern void pre_setChgIOAddr(PRE_CHG_IO_ADDR_INFO *pChgIoAddrInfo);
extern UINT32 pre_getClockRate(void);
extern void pre_calcVIG_OneImg(PRE_VIG_INFO *pVigParam, PRE_VIG_SETTING_ONE_IMG *pVigSettingOneImg);
extern void pre_calcVIG_TwoImg(PRE_VIG_INFO *pVigParam, PRE_VIG_SETTING_TWO_IMG *pVigSettingTwoImg);


#endif
//@}

//@}

