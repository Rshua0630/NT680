/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for SIE
    module.

    @file       sie3_lib.h
    @ingroup    mIDrvIPPSIE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SIE3_LIB_H
#define _SIE3_LIB_H

#include "sie_lib.h"

extern UINT32 g_uiSie3Out0Cnt;
extern UINT32 g_uiSie3BccOvflCnt;
extern UINT32 g_uiSie3VdCnt;
extern UINT32 g_uiSie3ActStCnt;
extern UINT32 g_uiSie3Out5Cnt;

/**
    SIE Entire Configuration.

    Structure of entire SIE configuration.
*/
//@{
typedef struct {
	// No run-time change
	SIE_MAIN_INPUT_INFO         MainInInfo;
	SIE_DVI_INFO                DviInfo;
	SIE_PATGEN_INFO             PatGenInfo;

	// run-time change-able
	UINT32  uiFuncEn;
	UINT32  uiIntrpEn;
	SIE_BREAKPOINT_INFO BreakPointInfo;
	SIE_SRC_WIN_INFO    SrcWinInfo;
	SIE_ACT_WIN_INFO    ActWinInfo;
	SIE_CRP_WIN_INFO    CrpWinInfo;

//NA//    SIE_DRAM_IN0_INFO   DramIn0Info;
	SIE_DRAM_IN1_INFO   DramIn1Info;
	SIE_DRAM_IN2_INFO   DramIn2Info;
	SIE_DRAM_OUT0_INFO  DramOut0Info;
	SIE_DRAM_OUT1_INFO  DramOut1Info;
	SIE_DRAM_OUT2_INFO  DramOut2Info;
	SIE_DRAM_OUT3_INFO  DramOut3Info;
//NA//    SIE_DRAM_OUT4_INFO  DramOut4Info;
	SIE_DRAM_OUT5_INFO  DramOut5Info;

	SIE_DRAM_BURST_INFO DramBurstInfo;// would be removed soon

	SIE_OB_DT_INFO          ObDtInfo;
	SIE_OB_DT_RSLT_INFO     ObDtRsltInfo;// would be removed soon
	SIE_OB_OFS_INFO         ObOfsInfo;
//NA//    SIE_OB_FRAME_INFO       ObFrmInfo;
//NA//    SIE_OB_PLANE_INFO       ObPlnInfo;

	SIE_DPC_INFO        DefectInfo;
	SIE_MAIN_GAMMA_INFO MainGmaInfo;
	SIE_ECS_INFO        EcsInfo;
	SIE_DGAIN_INFO      DgainInfo;

	SIE_BS_H_INFO       BshInfo;
	SIE_BS_V_INFO       BsvInfo;
	SIE_CG_INFO         CGainInfo;
	SIE_GRIDLINE_INFO   GridLineInfo;
	SIE_YOUT_INFO       YoutInfo;

	SIE_IR_INFO         IrInfo;
	SIE_RCCB_INFO       RccbInfo;
//NA//    SIE_BCC_ADJ_INFO    BccAdjInfo;
	SIE_BCC_PARAM_INFO  BccParamInfo;
//NA//    //there would be default inside driver//SIE_BCC_TBL_INFO    BccTblInfo;

	SIE_STCS_PATH_INFO  StcsPathInfo;
	SIE_STCS_CALASIZE_GRP_INFO  StcsCaLaSzGrpInfo;
	SIE_VIG_INFO        StcsVigInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_CROP_INFO    StcsCaCrpInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_SCAL_INFO    StcsCaSmplInfo;
	SIE_CA_TH_INFO      StcsCaThInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_WIN_INFO     StcsCaWinInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_LA_CROP_INFO    StcsLaCrpInfo;
	SIE_LA_CG_INFO      StcsLaCgInfo;
	SIE_LA_GMA_INFO     StcsLaGma1Info;
//NA//    //SIE_STCS_CALASIZE_GRP_INFO//SIE_LA_WIN_INFO     StcsLaWinInfo;
//NA//    SIE_VA_CG_INFO      StcsVaCgInfo;
//NA//    SIE_VA_CROP_INFO    StcsVaCrpInfo;
//NA//    SIE_VA_GMA_INFO     StcsVaGma2Info;
//NA//    SIE_VA_FLTR_GROUP_INFO    StcsVaFltrG1;
//NA//    SIE_VA_FLTR_GROUP_INFO    StcsVaFltrG2;
//NA//    SIE_VA_WIN_INFO     StcsVaWinInfo;
//NA//    SIE_VA_INDEP_WIN_INFO StcsVaIndepWinInfo[5];
//NA//    SIE_ETH_INFO          StcsEthInfo;

//NA//    SIE_PFPC_INFO       PfpcInfo;
} SIE3_MODE_PARAM;
//@}


//@}

/**
SIE API

@name   SIE_API
*/
//@{

// sie3_lib.c
extern void sie3_isr(void);
extern ER sie3_open(SIE_OPENOBJ *pObjCB);
extern BOOL sie3_isOpened(void);
extern ER sie3_close(void);
extern ER sie3_pause(void);
extern ER sie3_start(void);
extern ER sie3_setMode(SIE3_MODE_PARAM *pSieParam);
extern ER sie3_chgParam(void *pParam, SIE_CHANGE_FUN_PARAM_SEL FunSel);
extern ER sie3_chgFuncEn(SIE_FUNC_SWITCH_SEL FuncSwitch, UINT32 uiFuncSel);
extern ER sie3_waitEvent(SIE_WAIT_EVENT_SEL WaitEvent, BOOL bClrFlag);
extern ER sie3_abortEvent(SIE_WAIT_EVENT_SEL WaitEvent);
//NA//extern void sie3_getBccRslt_LastFrame(SIE_BCC_RSLT_INFO *pBccRslt);
extern UINT32 sie3_getClockRate(void);
// sie3_int.c
extern void sie3_getMainInput(SIE_MAIN_INPUT_INFO *pMainInInfo);
//NA//extern void sie3_setDramInStart(void);
//NA//extern void sie3_calcObPlnScl(SIE_OB_PLANE_INFO *pObpParam, SIE_OB_PLANE_ADJ_INFO *pObpAdjParam);
extern UINT32 sie3_getIntrStatus(void);
extern UINT32 sie3_getFunction(void);
extern void sie3_getEngineStatus(SIE_ENGINE_STATUS_INFO *pEngineStatus);
extern void sie3_getActiveWindow(SIE_ACT_WIN_INFO *pActWin);
extern void sie3_getCropWindow(SIE_CRP_WIN_INFO *pCrpWin);
extern void sie3_getDVI(SIE_DVI_INFO *pDviParam);
extern void sie3_getObOfs(SIE_OB_OFS_INFO *pObDtOfs);
extern void sie3_calcBSHScl(SIE_BS_H_INFO *pBshParam, SIE_BS_H_ADJ_INFO *pBshAdjParam);
extern void sie3_calcBSVScl(SIE_BS_V_INFO *pBsvParam, SIE_BS_V_ADJ_INFO *pBsvAdjParam);
extern void sie3_calcVIG(SIE_VIG_INFO *pVigParam, SIE_VIG_ADJ_INFO *pVigAdjParam);
extern void sie3_calcVIG_TwoImg(SIE_VIG_INFO *pVigParam, SIE_VIG_ADJ_2IMG_INFO *pVigAdjTwoImgParam);
extern void sie3_calcECSScl(SIE_ECS_INFO *pEcsParam, SIE_ECS_ADJ_INFO *pEcsAdjParam);
extern void sie3_calcCaLaSize(SIE_STCS_CALASIZE_GRP_INFO *pVaSzGrpInfo, SIE_STCS_CALASIZE_ADJ_INFO *pVaAdjInfo);
extern void sie3_calcYoutInfo(SIE_YOUT_GRP_INFO *pSzGrpInfo, SIE_YOUT_ADJ_INFO *pSzAdjInfo);
extern UINT32 sie3_getOutAdd(UINT32 uiOutChIdx, SIE_PINGPONG_BUFF_SEL uiBufSel);
extern UINT32 sie3_getOutAdd_Flip(UINT32 uiOutChIdx, SIE_PINGPONG_BUFF_SEL uiBufSel);
extern void sie3_getCAResult(SIE_STCS_CA_RSLT_INFO *CaRsltInfo);
//NA//extern void sie3_getCAFlipResult(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt);
extern void sie3_getLaWin(SIE_LA_WIN_INFO *pSetting);
extern UINT32 sie3_getLaWinSum(void);
extern void sie3_getCAResultManual(SIE_STCS_CA_RSLT_INFO *CaRsltInfo, SIE_CA_WIN_INFO *CaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie3_getCAFlipResultManual(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt, SIE_CA_WIN_INFO *CaWinInfo, UINT32 uiBuffAddr);
extern void sie3_getHisto(UINT16 *puiHisto);
extern void sie3_getLAResult(UINT16 *puiBufLa1, UINT16 *puiBufLa2);
//NA//extern void sie3_getLAFlipResult(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_FLIP_SEL pFlipSel);
extern void sie3_getLAResultManual(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_LA_WIN_INFO *LaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie3_getLAFlipResultManual(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_LA_WIN_INFO *LaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie3_getVAResult(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V);
//NA//extern void sie3_getVAFlipResult(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie3_getVAResultManual(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_VA_WIN_INFO *VaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie3_getVAFlipResultManual(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_VA_WIN_INFO *VaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie3_getBccRslt(SIE_BCC_RSLT_INFO *pBccRslt);
//NA//extern void sie3_getVaIndepWinRslt(SIE_VA_INDEP_WIN_RSLT_INFO *pSetting, UINT32 uiIdx);
extern void sie3_getBurstLength(SIE_BURST_LENGTH *pBurstLen);


extern ER   sie3_SIE3_CLK_openClock(void);
extern ER   sie3_SIE3_CLK_closeClock(void);
extern BOOL sie3_SIE3_CLK_isClockOpened(void);
extern ER   sie3_SIE3_CLK_chgClockSource(SIE_CLKSRC_SEL ClkSrc);
extern ER   sie3_SIE3_CLK_chgClockRate(UINT32 uiClkRate);
ER sie3_chgBurstLengthByMode(SIE_DRAM_BURST_MODE_SEL BurstMdSel);
UINT32 sie3_getBccsideSz(UINT32 uiWidth, UINT32 uiHeight);

#endif//_SIE3_LIB_H
//@}

//@}


