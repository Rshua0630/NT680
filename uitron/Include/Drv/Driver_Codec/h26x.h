#ifndef _H26x_H_
#define _H26x_H_

#include "Type.h"

#define H26x_PIC_FSH_INT        0x00000001  /*!< Picture Finish Interrupt. */
#define H26X_INIT_INT_EN        0x00111111
#define H26X_INIT_INT_STATUS    0x00111111

#define H26X_SRAQ             1
#define H26X_MAX_BSCMD_NUM   80 // 5120/64 = 80

typedef enum _H26xStatus {
	H26X_STATUS_ERROR_SRC_BUF_IS_NOT_USED = -12,  /*!< Current source buffer is not used. */
	H26X_STATUS_ERROR_YADDR_INVALID       = -11,  /*!< Luma address of current picture is invalid. */
	H26X_STATUS_ERROR_DISPLAY_FULL        = -10,  /*!< All display-buffer is full. */
	H26X_STATUS_ERROR_DISPLAY_EMPTY       = -9,   /*!< There is no extra buffer to be displayed. */
	H26X_STATUS_ERROR_REC_BUF_FULL        = -8,   /*!< All recontruction buffer is full. */
	H26X_STATUS_ERROR_REGOBJ_EMPTY        = -7,   /*!< There is no register object for h26x to process. */
	H26X_STATUS_ERROR_REGOBJ_FULL         = -6,
	H26X_STATUS_ERROR_NO_BS_BUF           = -5,   /*!< h26x module is already closed. */
	H26X_STATUS_ERROR_NO_SRC_BUF          = -4,   /*!< There is no free source buffer. */
	H26X_STATUS_ERROR_CLOSED              = -3,   /*!< h26x module is already closed. */
	H26X_STATUS_ERROR_OPENED              = -2,   /*!< h26x module open error. */
	H26X_STATUS_ERROR                     = -1,   /*!< Error. */
	H26X_STATUS_OK                        = 0,    /*!< OK. */
	ENUM_DUMMY4WORD(H26xStatus),
} H26xStatus;

typedef struct _H26XYuvAddr {
	UINT32 uiYAddr;
	UINT32 uiUVAddr;
} H26XYuvAddr;

typedef struct _H26X3dnr {
	UINT32 ui3dnrCfg;
	UINT32 uiP2PRefCfg;
	UINT32 uiP2PCfg_0;
	UINT32 uiP2PCfg_1;
	UINT32 uiP2PCfg_2;
	UINT32 uiP2PCfg_3;
	UINT32 uiP2PCfg_4;
	UINT32 uiMctfCfg_0;
	UINT32 uiMctfCfg_1;
	UINT32 uiMctfCfg;
	UINT32 uiP2PClampCfg;
	UINT32 uiMCTFClampCfg;
	UINT32 uiP2PBoard;
} H26X3dnr;

typedef struct _H26XPicUnit {

	H26X3dnr e3dnr;

	UINT32 uiSrcPicSize;
	UINT32 uiEncPicSize;

	UINT32 uiPicSize;       // not use in driver //
	UINT32 uiSrcHeight;     // not use in driver //

	UINT32 uiSrcLineOffset;
	UINT32 uiRecLineOffset;
	UINT32 uiPicCfg;

	UINT32 uiQpSetting;
	UINT32 uiIlfCfg;
	UINT32 uiIlfLineOffset;
	UINT32 uiNROutLineOffset;
	UINT32 uiPicCfg1;
	UINT32 uiSaoParam;
	UINT32 uiIlfDisableCtb;
	UINT32 uiECCfg;
	UINT32 uiK_Y_W0;
	UINT32 uiK_Y_W1;
	UINT32 uiK_Y_W2;
	UINT32 uiK_Y_R0;
	UINT32 uiK_Y_R1;
	UINT32 uiK_Y_R2;

	UINT32 uiK_U_W0;
	UINT32 uiK_U_W1;
	UINT32 uiK_U_W2;
	UINT32 uiK_U_R0;
	UINT32 uiK_U_R1;
	UINT32 uiK_U_R2;

	UINT32 uiK_V_W0;
	UINT32 uiK_V_W1;
	UINT32 uiK_V_W2;
	UINT32 uiK_V_R0;
	UINT32 uiK_V_R1;
	UINT32 uiK_V_R2;
	UINT32 uiECKTablW[9];
	UINT32 uiEDKTabl[9];

	UINT32 uiFRO_Y_INTRA_I_16;
	UINT32 uiFRO_Y_INTRA_I_8;
	UINT32 uiFRO_Y_INTRA_P_16;
	UINT32 uiFRO_Y_INTRA_P_8;
	UINT32 uiFRO_C_INTRA_I_8;
	UINT32 uiFRO_C_INTRA_I_4;
	UINT32 uiFRO_C_INTRA_P_8;
	UINT32 uiFRO_C_INTRA_P_4;
	UINT32 uiFRO_Y_INTER_P_16;
	UINT32 uiFRO_Y_INTER_P_8;
	UINT32 uiFRO_C_INTER_P_8;
	UINT32 uiFRO_C_INTER_P_4;
	UINT32 uiFRO_AC_Y_INTRA;
	UINT32 uiFRO_AC_C_INTRA;
	UINT32 uiFRO_AC_Y_INTER;
	UINT32 uiGDR_CTRL_1;
	UINT32 uiGDR_CTRL_2;

	UINT32 uiROI_Ctrl;
	UINT32 uiROI_LT[10];
	UINT32 uiROI_RD[10];
	UINT32 uiROI_QP[3];
	UINT32 uiROI_QpMode;   // 0 : fixed qp, 1 : delta qp
	UINT32 uiROI_ValidWin;  // 1 window 1 bit

	UINT32 uiAEAD_0;
	UINT32 uiAEAD_1;
	UINT32 uiAEAD_2;
	UINT32 uiAEAD_3;

	UINT32 uiSliceRowNum;

	UINT32 uiCHROMA_DISTORTION;
	UINT32 uiVirBsdmaCmdBufAddr;
	UINT32 uiHeaderValid;

	UINT32 uiHwHeaderNum;
	UINT32 uiHwHeaderAddr;
	UINT32 uiHwHeaderSize[H26X_MAX_BSCMD_NUM];

	UINT32 uiHwBsAddr;
	UINT32 uiHwBsLen;

	H26XYuvAddr eSrcYuvAddr;

	UINT32 uiPicNum;
#if H26X_SRAQ
	UINT32 uiAQ_0;
	UINT32 uiAQ_1;
	UINT32 uiAQ_2;
#endif

	UINT32 uiSrcDecSideInfoLoffset;
	UINT32 uiSrcDecStripNum;
	UINT32 uiSrcDecStrip01Size;
	UINT32 uiSrcDecStrip23Size;
	UINT32 uiSrcDecktable0;
	UINT32 uiSrcDecktable1;
	UINT32 uiSrcDecktable2;

	UINT32 uiRC_EN; //0x0 bit28
	UINT32 uiRC_0; //0x22c
	UINT32 uiRC_1; //0x230
	UINT32 uiRC_2; //0x234
	UINT32 uiRC_3; //0x238
	UINT32 uiRC_4; //0x23c
	UINT32 uiRC_5; //0x240
	UINT32 uiRC_6; //0x244
	UINT32 uiRC_7; //0x248
	UINT32 uiRC_8; //0x24c

	UINT32 uiRC_frmqp; //0x288

	UINT32 uiAVC_RDO_0;
	UINT32 uiAVC_RDO_1;
	UINT32 uiAVC_RDO_2;
} H26XPicUnit;

typedef struct _H26XRegSet {
	UINT32 IsHevc;
	H26XPicUnit ePicUnit;
	H26XYuvAddr eRecYuvAddr;
	H26XYuvAddr eRefYuvAddr;

	UINT32 uiColMvsWtAddr;
	UINT32 uiColMvsRdAddr;
	UINT32 uiUriAddr;
	UINT32 uiRecUrAddr;
	UINT32 uiIlfUpBotAddr;
	UINT32 uiIlfSideInfoAddr;
	UINT32 uiEdIlfSideInfoAddr;
	UINT32 uiHwBsMaxLen;
	UINT32 uiUserQpAddr;
	UINT32 uiNROutYAddr;
	UINT32 uiNROutUVAddr;
	UINT32 uiSrcDecSideInfoAddr;
	UINT32 uiRCStatReadIAddr;
	UINT32 uiRCStatWriteIAddr;
	UINT32 uiRCStatReadPAddr;
	UINT32 uiRCStatWritePAddr;
} H26XRegSet;


typedef enum {
	H26X_CONFIG_ID_FREQ,                    ///< H26x module clock (Unit: MHz), will be active on succeeding h26x_open().
	///< Context can be one of the following:
	///< - @b 120   : 120 MHz
	///< - @b 160   : 160 MHz
	///< - @b 192   : 192 MHz (Default value)
} H26X_CONFIG_ID;

extern ER h26x_setConfig(H26X_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32 h26x_getConfig(H26X_CONFIG_ID ConfigID);
extern H26xStatus h26x_open(UINT32 h26x_clk);
extern H26xStatus h26x_open2(void);
extern H26xStatus h26x_close(void);
extern H26xStatus h26x_close2(void);
extern void h26x_resetINT(void);
extern UINT32 h26x_waitINT(void);
extern UINT32 h26x_checkINT(void);
extern void h26x_resetHW(void);
extern void h26x_isr(void);
extern UINT32 h26x_exit_isr(void);
extern void h26x_resetIntStatus(void);
extern void h26x_setEncInitReg(H26XRegSet *pH26xRegSet);
extern void h26x_setKTbl_R_and_Ed(void);
extern void h26x_getKTbl_R_and_Ed(void);
extern void h26x_doAgain(UINT32 uiIsEnc);
extern void h26x_setEncPicReg(H26XRegSet *pH26xRegSet);
extern void h26x_setDecInitReg(H26XRegSet *pH26xRegSet);
extern void h26x_setDecPicReg(H26XRegSet *pH26xRegSet);
void h26x_setSystemReset(void);

extern void h26x_prtMem(UINT32 uiMemAddr, UINT32 uiMemLen);
extern void h26x_prtReg(void);
extern void h26x_start(void);
extern void h26x_hack(UINT32 uiPicNum, UINT32 uiPatNum);
extern void h26x_reset(void);
extern void h26x_setPicReg(H26XPicUnit *pH26XPicUnit, H26XRegSet *pH26XRegSet);
extern void h26x_setBsCmdSize(UINT32 uiBsLen, UINT32 *uiCmdSize, UINT32 *uiCmdNum);;
extern void h26x_setBsdmaCmdBufAddrEn(void);
extern UINT32 h26x_getBsOudAddr(void);
extern void h26x_setBsOutAddr(UINT32 uiAddr, UINT32 uiSize);
extern void h26x_setBsOutBufEn(void);
extern UINT32 h26x_getBsDmaAddr(void);
extern void h26x_setBsDmaAddr(UINT32 uiAddr);

extern void h26x_openIntEn(void);
extern void h26x_closeIntEn(void);
extern void h26x_clearIntStatus(UINT32 uiIntStatus);

extern UINT32 h26x_getIntStatus(void);
extern UINT32 h26x_getPicBsLen(void);
extern UINT32 h26x_getRcSumQStep(void);
extern UINT32 h26x_getPicCost(void);
extern UINT32 h26x_getIlfDmaCheckSum(void);
extern UINT32 h26x_getIlfSideInfoCheckSum(void);
extern UINT32 h26x_getBsCheckSum(void);
extern void h26x_getKTblW(UINT32 *uiY0, UINT32 *uiY1, UINT32 *uiY2,
						  UINT32 *uiU0, UINT32 *uiU1, UINT32 *uiU2, UINT32 *uiV0, UINT32 *uiV1, UINT32 *uiV2);
extern void h26x_getKTblR(UINT32 *uiY0, UINT32 *uiY1, UINT32 *uiY2,
						  UINT32 *uiU0, UINT32 *uiU1, UINT32 *uiU2, UINT32 *uiV0, UINT32 *uiV1, UINT32 *uiV2);
extern void h26x_setKTblW(UINT32 uiY0, UINT32 uiY1, UINT32 uiY2,
						  UINT32 uiU0, UINT32 uiU1, UINT32 uiU2, UINT32 uiV0, UINT32 uiV1, UINT32 uiV2);
extern void h26x_setKTblR(UINT32 uiY0, UINT32 uiY1, UINT32 uiY2,
						  UINT32 uiU0, UINT32 uiU1, UINT32 uiU2, UINT32 uiV0, UINT32 uiV1, UINT32 uiV2);
extern UINT32 h26x_getCtrlInfo(void);
extern UINT32 h26x_getSrcCheckSum_Y(void);
extern UINT32 h26x_getSrcCheckSum_C(void);
extern UINT32 h26x_getECCheckSum(void);
extern UINT32 h26x_getURICheckSum(void);
extern UINT32 h26x_getCycle(void);
extern UINT32 h26x_getQP(void);
extern UINT32 h26x_getFastSearch(void);
extern UINT32 h26x_getFroEn(UINT32 *uiFro140, UINT32 *uiFro150, UINT32 *uiFro160, UINT32 *uiFro20c);
extern UINT32 h26x_getMvpEn(void);
extern UINT32 h26x_getMbqpEn(void);
extern UINT32 h26x_getDbEn(UINT32 *uiBeta, UINT32 *uiTc);
extern UINT32 h26x_getSaoEn(UINT32 *uiLuma, UINT32 *uiChroma, UINT32 *uiMerge, UINT32 *uiBoundary);
extern UINT32 h26x_getEcEn(void);
extern UINT32 h26x_get3DnREn(UINT32 *uiBlending, UINT32 *uiNr10, UINT32 *uiNr20, UINT32 *uiNr30);
extern void h26x_getMS(UINT32 *row, UINT32 *cross);
extern void h26x_getROI(UINT32 *en, UINT32 *fix, UINT32 *num);
extern UINT32 h26x_getImeCheckSum_L(void);
extern UINT32 h26x_getImeCheckSum_H(void);
extern UINT32 h26x_getEcCheckSum(void);

extern UINT32 h26x_getDebugCtr(void);
extern UINT32 h26x_getSrcLT0(void);
extern UINT32 h26x_getSrcLT1(void);
extern UINT32 h26x_getGdr(UINT32 *uiGdr134, UINT32 *uiGdr138);
extern UINT32 h26x_getDecBsLen(void);
extern UINT32 h26x_getHwRegBase(void);
extern void h26x_getSkip(UINT32 *bias, UINT32 *merge);
extern UINT32 h26x_getPicCfg(void);
extern void h26x_getXY(UINT32 *x, UINT32 *y);
extern void h26x_get350Debug(UINT32 *d1, UINT32 *d2, UINT32 *d3, UINT32 *d4, UINT32 *d5);
extern void h26x_prtDebug(void);
extern void h26x_get370Debug(void);
extern void h26x_setUnitChecksum(UINT32 uiVal);
extern void h26x_get380Debug(UINT32 *uiVal1, UINT32 *uiVal2);
extern UINT32 h26x_getQpSum(void);
extern UINT32 h26x_getSraq2(void);
extern void h26x_load(void);
extern void h26x_setDebugSel(UINT32 uiVal370, UINT32 uiVal374, UINT32 uiVal378);
extern UINT32 h26x_getDebugSel(void);
extern UINT32 h26x_getDebugSelResult(void);
extern UINT32 h26x_getCheckSumSelResult(void);
extern UINT32 h26x_getStatusSelResult(void);
extern void h26x_get370Debug(void);
extern void h26x_prtMemory(UINT32 uiMemAddr, UINT32 uiMemLen);
extern void h26x_setUnitChecksum(UINT32 uiVal);
extern void h26x_get380Debug(UINT32 *uiVal1, UINT32 *uiVal2);


extern UINT32 h26x_getRc1(void);
extern UINT32 h26x_getRcStatHigh(void);
extern UINT32 h26x_getRcStatLow(void);
extern UINT32 h26x_getRowRCEn(void);
extern void h26x_getSWRSTDebug(void);
extern void h26x_setRcQpStep(UINT32 uiVal);

extern UINT32 h26x_getVersion(void);
extern void h26x_setECKTablW(UINT32 EC_Tabl[9]);
extern void h26x_getECKTablR(UINT32 EC_Tabl[9]);
extern void h26x_setEDKTabl(UINT32 ED_Tabl[9]);
extern void h26x_getEDKTabl(UINT32 ED_Tabl[9]);
extern void h26x_getSliceResult(UINT32 SliceNum, UINT32 SliceLen[8]);
extern void h26x_get_ED_KTbl(UINT32 ED_Tabl[9]);
extern void h26x_set_ED_KTbl(UINT32 ED_Tabl[9]);
extern UINT32 h26x_getRCEn(void);
extern void h26x_setDramBurstLen(UINT32 uiVal);
extern void osd_bypass(UINT32 uiWidth, UINT32 uiHeight);
extern void h26x_apb_read_load(UINT32 isNextFrame);

extern void h26x_setHwRegBase(UINT32 *baddr);
extern INT32 h26x_getRegSize(void);
extern void h26x_setEncMode(void);
extern void h26x_setTVMode(void);
extern void h26x_setIMMode(void);
extern void h26x_setDecMode(void);
extern void h26x_setBSDMA(UINT32 uiBSDMAAddr, UINT32 uiHwHeaderNum, UINT32 uiHwHeaderAddr, UINT32 *uiHwHeaderSize);
extern void h26x_setMemAddrReg(H26XRegSet *pH26XRegSet);
extern void h26x_setEcUpdate(void);
void h26x_setEncStat(UINT32 uiVal);
extern UINT64 h26x_getRCPicCost(void);
extern void h26x_getRCStat(UINT32 uiSel, UINT32 *high, UINT32 *low);
extern UINT32 h26x_getRegData(UINT32 uiOffset);

extern void h26x_backup_reg(UINT32 uiTbl[0x100]);
extern void h26x_rollback_reg(UINT32 uiTbl[0x100]);
extern UINT32 h26x_getDramBurstLen(void);
#endif


