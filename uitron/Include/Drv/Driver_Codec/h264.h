#ifndef _H264_H_
#define _H264_H_

#include "Type.h"

#define H264_PIC_FSH_INT        0x00000001  /*!< Picture Finish Interrupt. */
#define H264_INIT_INT_EN        0x00111111
#define H264_INIT_INT_STATUS    0x00111111

#define H264_SRAQ             1

typedef enum _H264Status {
	H264_STATUS_ERROR_SRC_BUF_IS_NOT_USED = -12,  /*!< Current source buffer is not used. */
	H264_STATUS_ERROR_YADDR_INVALID       = -11,  /*!< Luma address of current picture is invalid. */
	H264_STATUS_ERROR_DISPLAY_FULL        = -10,  /*!< All display-buffer is full. */
	H264_STATUS_ERROR_DISPLAY_EMPTY       = -9,   /*!< There is no extra buffer to be displayed. */
	H264_STATUS_ERROR_REC_BUF_FULL        = -8,   /*!< All recontruction buffer is full. */
	H264_STATUS_ERROR_REGOBJ_EMPTY        = -7,   /*!< There is no register object for h26x to process. */
	H264_STATUS_ERROR_REGOBJ_FULL         = -6,
	H264_STATUS_ERROR_NO_BS_BUF           = -5,   /*!< h26x module is already closed. */
	H264_STATUS_ERROR_NO_SRC_BUF          = -4,   /*!< There is no free source buffer. */
	H264_STATUS_ERROR_CLOSED              = -3,   /*!< h26x module is already closed. */
	H264_STATUS_ERROR_OPENED              = -2,   /*!< h26x module open error. */
	H264_STATUS_ERROR                     = -1,   /*!< Error. */
	H264_STATUS_OK                        = 0,    /*!< OK. */
	ENUM_DUMMY4WORD(H264Status),
} H264Status;

typedef struct _H264YuvAddr {
	UINT32 uiYAddr;
	UINT32 uiUVAddr;
} H264YuvAddr;

typedef struct _H2643dnr {
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
} H2643dnr;

typedef struct _H264PicUnit {

	H2643dnr e3dnr;
	UINT32 uiSrcPicSize;
	UINT32 uiEncPicSize;
	//UINT32 uiSrcHeight;
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
	UINT32 uiHwHeaderSize[68];

	UINT32 uiHwBsAddr;
	UINT32 uiHwBsLen;

	H264YuvAddr eSrcYuvAddr;
	H264YuvAddr eTNRYuvAddr;

	UINT32 uiPicNum;

	UINT32 uiAVC_RDO_0;
	UINT32 uiAVC_RDO_1;
	UINT32 uiAVC_RDO_2;

#if H264_SRAQ
	UINT32 uiAQ_0;
	UINT32 uiAQ_1;
	UINT32 uiAQ_2;
#endif

	UINT32 uiQP_RNG;
	UINT32 uiRc_Ctrl[9];
} H264PicUnit;

typedef struct _H264RegSet {

	H264PicUnit ePicUnit;
	H264YuvAddr eRecYuvAddr;
	H264YuvAddr eRefYuvAddr;
	UINT32 uiIlfSideInfoAddr;
	UINT32 uiEdIlfSideInfoAddr;
	UINT32 uiHwBsMaxLen;
	UINT32 uiColMvsAddr;
	UINT32 uiUserQpAddr;
	UINT32 uiNROutYAddr;
	UINT32 uiNROutUVAddr;
	UINT32 uiRcRefRdAddr;
	UINT32 uiRcRefWrAddr;
} H264RegSet;


typedef enum {
	H264_CONFIG_ID_FREQ,                    ///< H264 module clock (Unit: MHz), will be active on succeeding h264_open().
	///< Context can be one of the following:
	///< - @b 120   : 120 MHz
	///< - @b 160   : 160 MHz
	///< - @b 192   : 192 MHz (Default value)
} H264_CONFIG_ID;

extern ER h264_setConfig(H264_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32 h264_getConfig(H264_CONFIG_ID ConfigID);
extern H264Status h264_open(void);
extern H264Status h264_open2(void);
extern H264Status h264_close(void);
extern H264Status h264_close2(void);
extern void h264_resetINT(void);
extern UINT32 h264_waitINT(void);
extern UINT32 h264_checkINT(void);
extern void h264_resetHW(void);
extern void h264_isr(void);
extern UINT32 h264_exit_isr(void);
extern void h264_resetIntStatus(void);
extern void h264_setEncInitReg(H264RegSet *pH264RegSet);
extern void h264_doAgain(void);
extern void h264_setEncPicReg(H264RegSet *pH264RegSet);
extern void h264_setDecInitReg(H264RegSet *pH264RegSet);
extern void h264_setDecPicReg(H264RegSet *pH264RegSet);
extern void h264_prtMem(UINT32 uiMemAddr, UINT32 uiMemLen);
extern void h264_prtReg(void);
extern void h264_start(void);
extern void h264_hack(UINT32 uiPicNum, UINT32 uiPatNum);
extern void h264_reset(void);
extern void h264_setPicReg(H264PicUnit *pH264PicUnit, H264RegSet *pH264RegSet);
extern void h264_setBsCmdSize(UINT32 uiBsLen, UINT32 *uiCmdSize, UINT32 *uiCmdNum);;
extern void h264_setBsdmaCmdBufAddrEn(void);
extern UINT32 h264_getBsOudAddr(void);
extern void h264_setBsOutAddr(UINT32 uiAddr, UINT32 uiSize);
extern void h264_setBsOutBufEn(void);
extern UINT32 h264_getBsDmaAddr(void);
extern void h264_setBsDmaAddr(UINT32 uiAddr);

extern void h264_openIntEn(void);
extern void h264_closeIntEn(void);
extern void h264_clearIntStatus(UINT32 uiIntStatus);

extern UINT32 h264_getIntStatus(void);
extern UINT32 h264_getPicBsLen(void);
extern UINT32 h264_getRcSumQStep(void);
extern UINT32 h264_getPicCost(void);
extern UINT32 h264_getIlfDmaCheckSum(void);
extern UINT32 h264_getIlfSideInfoCheckSum(void);
extern UINT32 h264_getBsCheckSum(void);
extern void h264_getKTblW(UINT32 *uiY0, UINT32 *uiY1, UINT32 *uiY2,
						  UINT32 *uiU0, UINT32 *uiU1, UINT32 *uiU2, UINT32 *uiV0, UINT32 *uiV1, UINT32 *uiV2);
extern void h264_getKTblR(UINT32 *uiY0, UINT32 *uiY1, UINT32 *uiY2,
						  UINT32 *uiU0, UINT32 *uiU1, UINT32 *uiU2, UINT32 *uiV0, UINT32 *uiV1, UINT32 *uiV2);
extern void h264_setKTblW(UINT32 uiY0, UINT32 uiY1, UINT32 uiY2,
						  UINT32 uiU0, UINT32 uiU1, UINT32 uiU2, UINT32 uiV0, UINT32 uiV1, UINT32 uiV2);
extern void h264_setKTblR(UINT32 uiY0, UINT32 uiY1, UINT32 uiY2,
						  UINT32 uiU0, UINT32 uiU1, UINT32 uiU2, UINT32 uiV0, UINT32 uiV1, UINT32 uiV2);

extern void h264_get_ED_KTbl(UINT32 ED_Tabl[9]);
extern void h264_set_ED_KTbl(UINT32 ED_Tabl[9]);

extern UINT32 h264_getCtrlInfo(void);
extern UINT32 h264_getSrcCheckSum_Y(void);
extern UINT32 h264_getSrcCheckSum_C(void);
extern UINT32 h264_getECCheckSum(void);
extern UINT32 h264_getURICheckSum(void);
extern UINT32 h264_getCycle(void);
extern UINT32 h264_getQP(void);
extern UINT32 h264_getFastSearch(void);
extern UINT32 h264_getFroEn(UINT32 *uiFro140, UINT32 *uiFro150, UINT32 *uiFro160, UINT32 *uiFro20c);
extern UINT32 h264_getMvpEn(void);
extern UINT32 h264_getDbEn(UINT32 *uiBeta, UINT32 *uiTc);
extern UINT32 h264_getSaoEn(UINT32 *uiLuma, UINT32 *uiChroma, UINT32 *uiMerge, UINT32 *uiBoundary);
extern UINT32 h264_getEcEn(void);
extern UINT32 h264_get3DnREn(UINT32 *uiBlending, UINT32 *uiNr10, UINT32 *uiNr20, UINT32 *uiNr30);
extern void h264_getMS(UINT32 *row, UINT32 *cross);
extern void h264_getROI(UINT32 *en, UINT32 *fix, UINT32 *num);
extern UINT32 h264_getImeCheckSum_L(void);
extern UINT32 h264_getImeCheckSum_H(void);
extern UINT32 h264_getEcCheckSum(void);

extern UINT32 h264_getDebugCtr(void);
extern UINT32 h264_getSrcLT0(void);
extern UINT32 h264_getSrcLT1(void);
extern UINT32 h264_getGdr(UINT32 *uiGdr134, UINT32 *uiGdr138);
extern UINT32 h264_getDecBsLen(void);
extern UINT32 h264_getHwRegBase(void);
extern void h264_getSkip(UINT32 *bias, UINT32 *merge);
extern UINT32 h264_getPicCfg(void);
extern void h264_getXY(UINT32 *x, UINT32 *y);
extern void h264_get350Debug(UINT32 *d1, UINT32 *d2, UINT32 *d3, UINT32 *d4, UINT32 *d5);
extern void h264_prtDebug(void);
extern UINT32 h264_getSraq2(void);
extern void h264_setDramBurstLen(UINT32 uiVal);
extern void h264_get370Debug(void);

extern void h264_setECKTablW(UINT32 EC_Tabl[9]);
extern void h264_getECKTablR(UINT32 EC_Tabl[9]);

void h264_prt_cur_reg(void);
void h264_prt_nxt_reg(void);

UINT32 h264_prt_dbg_sel_val(UINT32 uiSel);
UINT64 h264_prt_rc_sel_val(UINT32 uiSel);
UINT32 h264_prt_stat_sel_val(UINT32 uiSel);
UINT32 h264_prt_chksum_sel_val(UINT32 uiSel);

void avc_enc_hack(UINT32 uiPicNum, UINT32 uiPatNum);
void avc_enc_end_hack(void);
void avc_dec_hack(UINT32 uiPicNum, UINT32 uiPatNum);

void avc_enc_dse(UINT32 ui_vir_sde_side_info_addr, UINT32 kTab[3], UINT32 uiWidth, UINT32 uiHeight, UINT32 uiPatNum);
void h264_dec_do_again(void);

#endif

