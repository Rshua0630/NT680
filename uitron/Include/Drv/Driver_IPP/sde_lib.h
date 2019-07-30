#ifndef _SDE_LIB_H_
#define _SDE_LIB_H_

//#ifdef __cplusplus
//extern "C" {
//#endif


//#include "Type.h"

/*
    Define SDE function enable.
*/

#define SDE_S_SCAN_MODE                   0x00000001  ///< scanning range mode small or big
#define SDE_S_PATH2_INPUT_EN              0x00000002  ///< has input path of disparity volume
#define SDE_S_OUT_SEL                     0x00000004
#define SDE_S_COST_COMP_MODE              0x00000008
#define SDE_S_SCAN_FUN_EN                 0x00000010  ///< enable function: cost volume scanning
#define SDE_S_DIAG_FUN_EN                 0x00000020  ///< enable function: dignal left-right checking
#define SDE_S_H_FLIP01                    0x00000040  ///< input 0 and 1 is H flipped
#define SDE_S_V_FLIP01                    0x00000080  ///< input 0 and 1 is V flipped
#define SDE_S_H_FLIP2                     0x00000100  ///< input 2 is H flipped
#define SDE_S_V_FLIP2                     0x00000200  ///< input 2 is V flipped
#define SDE_S_DIAG_MODE                   0x00008000  //internal debugging usage
//#define SDE_SCAN_FUN_SEL
//#define DISP_INVALID_VAL_SEL
#define SDE_FUNC_ALL                      0x000003FF  ///< all func enable mask

/**
    Define SDE interrput enable.
*/
//@{
#define SDE_INTE_FRMEND                 0x00000001  ///< frame end interrupt

//@}

/**
    Define SDE interrput status.
*/
//@{
#define SDE_INT_FRMEND                 0x00000001  ///< frame end interrupt

//@}

/**
    Struct SDE open object.

    ISR callback function
*/
//@{
typedef struct _SDE_OPENOBJ {
	VOID (*FP_SDEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 uiSdeClockSel; ///< support 220/192/160/120/80/60/48 Mhz
} SDE_OPENOBJ;
//@}


/**
    SDE Stereo Depth Disparity Operation Mode.

    Select SDE Operation Mode.
\n  Used for sde_setMode() ?
*/
//@{
typedef enum _SDE_OP_MODE {
	SDE_MAX_DISP_31    = 0,    ///< max disparity 31
	SDE_MAX_DISP_63    = 1,    ///< max disparity 63
	SDE_OPMODE_UNKNOWN = 2,    ///< undefined mode
	ENUM_DUMMY4WORD(SDE_OP_MODE)
} SDE_OP_MODE;
//@}

/**
    SDE burst length define
*/
//@{
typedef enum _SDE_BURST_SEL {
	SDE_BURST_64W    = 0,       ///< burst length 64 word
	SDE_BURST_48W    = 1,       ///< burst length 32 word
	SDE_BURST_32W    = 2,       ///< burst length 32 word
	ENUM_DUMMY4WORD(SDE_BURST_SEL)
} SDE_BURST_SEL;


/**
    SDE Stereo Depth Disparity Function Selection.

    Select SDE functions.
\n  Used for sde_setMode()
*/
//@{
typedef enum _SDE_OUTPUT_SEL {
	SDE_OUT_DISPARITY           = 0,       ///< output disparity map
	SDE_OUT_VOL                 = 1,       ///< output scan volume
	ENUM_DUMMY4WORD(SDE_OUTPUT_SEL)
} SDE_OUTPUT_SEL;
//@}


/**
    SDE Stereo Depth Disparity Function Selection.

    Select SDE functions.
\n  Used for sde_setMode()
*/
//@{
typedef enum _SDE_OUTVOL_SEL {
	SDE_OUT_SCAN_VOL        = 0,       ///< output temporal scan volume for this round
	SDE_OUT_SCAN_DIR_1      = 1,       ///< output temporal scan volume direction 1
	SDE_OUT_SCAN_DIR_3      = 2,       ///< output temporal scan volume direction 1
	SDE_OUT_SCAN_DIR_5      = 3,       ///< output temporal scan volume direction 1
	SDE_OUT_SCAN_DIR_7      = 4,       ///< output temporal scan volume direction 1
	ENUM_DUMMY4WORD(SDE_OUTVOL_SEL)
} SDE_OUTVOL_SEL;
//@}


/**
    SDE Stereo Depth Invalid Value Selection.

    Select SDE invalid value to fill in.
*/
//@{
typedef enum _SDE_INV_SEL {
	SDE_INV_0               = 0,        ///< fill in 0 to invalid point
	SDE_INV_63              = 1,       ///< fill in 63 to invalid point
	SDE_INV_255             = 2,       ///< fill in 255 to invalid point
	ENUM_DUMMY4WORD(SDE_INV_SEL)
} SDE_INV_SEL;
//@}


/**
    Struct SDE Cost Parameters.

    SDE Cost computation related parameters.
*/
//@{
typedef struct _SDE_COST_PARA {
	UINT32          uiAdBoundUpper;         ///< upper bound value of absolute difference
	UINT32          uiAdBoundLower;         ///< lower bound value of absolute difference
	UINT32          uiCensusBoundUpper;     ///< upper bound value of census bit code
	UINT32          uiCensusBoundLower;     ///< lower bound value of census bit code
	UINT32          uiCensusAdRatio;        ///< weighted ratio with absolute difference on luminance
} SDE_COST_PARA;
//@}


/**
    Struct SDE in out size Parameters.

    SDE Size related parameters.
*/
//@{
typedef struct _SDE_SIZE_PARA {
	UINT32 uiWidth;               ///< image width
	UINT32 uiHeight;              ///< image height
	UINT32 uiOfsi0;                ///< image input lineoffset of input 0
	UINT32 uiOfsi1;                ///< image input lineoffset of input 1
	UINT32 uiOfsi2;                ///< image input lineoffset of input 2
	UINT32 uiOfso;                ///< image output lineoffset
} SDE_SIZE_PARA;
//@}

/**
    Struct SDE Lumincnace related parameters.

    SDE Luminance related parameters used in cost and scan.
*/
//@{
typedef struct _SDE_LUMA_PARA {
	UINT32          uiLumaThreshSaturated;  ///< threshold to determine saturated pixel
	UINT32          uiCostSaturatedMatch;   ///< cost value assigned to saturated right image point encountered
	UINT32          uiDeltaLumaSmoothThresh;///< threshold to let data term cost dominate
} SDE_LUMA_PARA;
//@}


/**
    Struct SDE Scan Penalty parameters.

    SDE penalty values for scan.
*/
//@{
typedef struct _SDE_PENALTY_PARA {
	UINT32          uiScanPenaltyNonsmooth; ///< smallest penalty for big luminance difference
	UINT32          uiScanPenaltySmooth0;   ///< small penalty for small disparity change
	UINT32          uiScanPenaltySmooth1;   ///< small penalty for middle disparity change
	UINT32          uiScanPenaltySmooth2;   ///< small penalty for big disparity change
} SDE_PENALTY_PARA;
//@}


/**
    Struct SDE Scan smoothness parameters.

    SDE threshold to determined penalties aggigned.
*/
//@{
typedef struct _SDE_SMOOTH_PARA {
	UINT32          uiDeltaDispLut0;        ///< Threshold to remove smooth penalty
	UINT32          uiDeltaDispLut1;        ///< Threshold to give small penalty
} SDE_SMOOTH_PARA;
//@}

/**
    Struct SDE LRC parameters.

    SDE Threshold for LRC.
*/
//@{
typedef struct _SDE_DIAG_THRESH {
	UINT32          uiDiagThreshLut0;       ///< diagonal search threshold look-up table 0
	UINT32          uiDiagThreshLut1;       ///< diagonal search threshold look-up table 1
	UINT32          uiDiagThreshLut2;       ///< diagonal search threshold look-up table 2
	UINT32          uiDiagThreshLut3;       ///< diagonal search threshold look-up table 3
	UINT32          uiDiagThreshLut4;       ///< diagonal search threshold look-up table 4
	UINT32          uiDiagThreshLut5;       ///< diagonal search threshold look-up table 5
	UINT32          uiDiagThreshLut6;       ///< diagonal search threshold look-up table 6

} SDE_DIAG_THRESH;
//@}

/**
    Struct SDE IO Parameters.

    SDE Address
*/
//@{
typedef struct _SDE_IO_PARA {
	// sde-todo
	SDE_SIZE_PARA   Size;           ///< input size and offset information
	UINT32          uiInAddr0;       ///< input starting address of input 0
	UINT32          uiInAddr1;       ///< input starting address of input 1
	UINT32          uiInAddr2;       ///< input starting address of input 2
	UINT32          uiOutAddr;      ///< output starting address
	//BOOL            bHflip01;        ///< input 0 and 1 H fliped
	//BOOL            bVflip01;        ///< input 0 and 1 V fliped
	//BOOL            bHflip2;        ///< input 2 H fliped
	//BOOL            bVflip2;        ///< input 2 V fliped
} SDE_IO_PARA;
//@}

/**
    Struct SDE Parameters.

    Set SDE Parameters to execute.
*/
//@{
typedef struct _SDE_PARAM {
	BOOL            bScanEn;                ///< SDE scanning functionality enable
	BOOL            bDiagEn;                ///< SDE Diag LEC functionaltiy enable
	BOOL            bInput2En;              ///< input2 enabled
	BOOL            bCostCompMode;          ///< SDE cost volume computation mode (flip-orN)
	BOOL            bHflip01;               ///< SDE H flip 01
	BOOL            bVflip01;               ///< SDE V flip 01
	BOOL            bHflip2;               ///< SDE H flip 2
	BOOL            bVflip2;               ///< SDE V flip 2
	SDE_OP_MODE     opMode;                 ///< SDE opertaion mode, disaprity range big or small
	SDE_IO_PARA     ioPara;                 ///< input output related parameters
	SDE_INV_SEL     invSel;                 ///< invlaid value selection
	SDE_OUTPUT_SEL  outSel;                 ///< engine output selection
	SDE_OUTVOL_SEL  outVolSel;              ///< select which kind of inner volume result to output for outSel
	SDE_BURST_SEL   burstSel;               ///< burst length selection

	UINT32          uiIntrEn;               ///< interrupt enable

	SDE_COST_PARA   costPara;               ///< cost computation related parameters

	SDE_LUMA_PARA   lumaPara;
	SDE_PENALTY_PARA    penaltyValues;      ///< scan penalty values
	SDE_SMOOTH_PARA thSmooth;               ///< scan threshold of penlaties assigned


	SDE_DIAG_THRESH thDiag;                 ///< Threshold for LRC
} SDE_PARAM;
//@}


extern ER       sde_open(SDE_OPENOBJ *pObjCB);
extern BOOL     sde_isOpened(VOID);
extern ER       sde_close(VOID);
extern ER       sde_setMode(SDE_PARAM *pSdeInfo);
extern ER       sde_start(VOID);
extern ER       sde_pause(VOID);

extern ER       sde_enableInt(UINT32 uiIntr);
extern UINT32   sde_getIntEnable(VOID);
extern UINT32   sde_getIntStatus(VOID);
extern ER       sde_clearInt(UINT32 uiIntr);
//
extern ER       sde_enableCtrlBit(BOOL bEnable, UINT32 uiFunc);
extern VOID     sde_waitFlagFrameEnd(VOID);
extern ER       sde_enableFunction(BOOL bEnable, UINT32 uiFunc);
extern BOOL     sde_checkFunctionEnable(UINT32 uiFunc);

extern UINT32   sde_getClockRate(VOID);
extern ER       sde_getBurstLength(SDE_BURST_SEL *pBurstLen);

extern UINT32  sde_getDmaInAddr0(VOID);
extern UINT32  sde_getDmaInAddr1(VOID);
extern UINT32  sde_getDmaInAddr2(VOID);
extern UINT32  sde_getDmaInLofs0(VOID);
extern UINT32  sde_getDmaInLofs1(VOID);
extern UINT32  sde_getDmaInLofs2(VOID);
extern UINT32  sde_getDmaOutAddr(VOID);
extern UINT32  sde_getDmaOutLofs(VOID);
extern UINT32  sde_getInVsize(VOID);
extern UINT32  sde_getInHsize(VOID);
extern UINT32  sde_getInvalidCount(VOID);

#endif
