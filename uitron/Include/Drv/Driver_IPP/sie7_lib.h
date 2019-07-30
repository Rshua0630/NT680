/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for SIE
    module.

    @file       sie7_lib.h
    @ingroup    mIDrvIPPSIE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SIE7_LIB_H
#define _SIE7_LIB_H

#include "sie_lib.h"


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
//NA//    SIE_DRAM_IN1_INFO   DramIn1Info;
	SIE_DRAM_IN2_INFO   DramIn2Info;
	SIE_DRAM_OUT0_INFO  DramOut0Info;
	SIE_DRAM_OUT1_INFO  DramOut1Info;
	SIE_DRAM_OUT2_INFO  DramOut2Info;
//NA//    SIE_DRAM_OUT3_INFO  DramOut3Info;
//NA//    SIE_DRAM_OUT4_INFO  DramOut4Info;
	SIE_DRAM_BURST_INFO DramBurstInfo;// would be removed soon

	SIE_OB_DT_INFO          ObDtInfo;
	SIE_OB_DT_RSLT_INFO     ObDtRsltInfo;// would be removed soon
	SIE_OB_OFS_INFO         ObOfsInfo;
//NA//    SIE_OB_FRAME_INFO       ObFrmInfo;
//NA//    SIE_OB_PLANE_INFO       ObPlnInfo;

//NA//    SIE_DEF_COL_INFO    DefectInfo;
//NA//    SIE_MAIN_GAMMA_INFO MainGmaInfo;
	SIE_ECS_INFO        EcsInfo;
	SIE_DGAIN_INFO      DgainInfo;

	SIE_IR_INFO         IrInfo;
	SIE_RCCB_INFO       RccbInfo;

//NA//    SIE_BS_H_INFO       BshInfo;
//NA//    SIE_BS_V_INFO       BsvInfo;

//NA//    SIE_BCC_ADJ_INFO    BccAdjInfo;
//NA//    //there would be default inside driver//SIE_BCC_PARAM_INFO  BccParamInfo;
//NA//    //there would be default inside driver//SIE_BCC_TBL_INFO    BccTblInfo;

	SIE_STCS_PATH_INFO  StcsPathInfo;
	SIE_STCS_CALASIZE_GRP_INFO  StcsCaLaSzGrpInfo;
//NA//    SIE_VIG_INFO        StcsVigInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_CROP_INFO    StcsCaCrpInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_SCAL_INFO    StcsCaSmplInfo;
	SIE_CA_TH_INFO      StcsCaThInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_WIN_INFO     StcsCaWinInfo;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_LA_CROP_INFO    StcsLaCrpInfo;
	SIE_LA_CG_INFO      StcsLaCgInfo;
	SIE_LA_GMA_INFO     StcsLaGma1Info;
	//SIE_STCS_CALASIZE_GRP_INFO//SIE_LA_WIN_INFO     StcsLaWinInfo;
	SIE_VA_CG_INFO      StcsVaCgInfo;
//NA//    SIE_VA_CROP_INFO    StcsVaCrpInfo;
//NA//    SIE_VA_GMA_INFO     StcsVaGma2Info;
//NA//    SIE_VA_FLTR_GROUP_INFO    StcsVaFltrG1;
//NA//    SIE_VA_FLTR_GROUP_INFO    StcsVaFltrG2;
//NA//    SIE_VA_WIN_INFO     StcsVaWinInfo;
//NA//    SIE_VA_INDEP_WIN_INFO StcsVaIndepWinInfo[5];
//NA//    SIE_ETH_INFO          StcsEthInfo;

//NA//    SIE_PFPC_INFO       PfpcInfo;
} SIE7_MODE_PARAM;
//@}


//@}

/**
SIE API

@name   SIE_API
*/
//@{

// sie7_lib.c
extern void sie7_isr(void);
extern ER sie7_open(SIE_OPENOBJ *pObjCB);
extern BOOL sie7_isOpened(void);
extern ER sie7_close(void);
extern ER sie7_pause(void);
extern ER sie7_start(void);
extern ER sie7_setMode(SIE7_MODE_PARAM *pSieParam);
extern ER sie7_chgParam(void *pParam, SIE_CHANGE_FUN_PARAM_SEL FunSel);
extern ER sie7_chgFuncEn(SIE_FUNC_SWITCH_SEL FuncSwitch, UINT32 uiFuncSel);
extern ER sie7_waitEvent(SIE_WAIT_EVENT_SEL WaitEvent, BOOL bClrFlag);
extern ER sie7_abortEvent(SIE_WAIT_EVENT_SEL WaitEvent);
//NA//extern void sie7_getBccRslt_LastFrame(SIE_BCC_RSLT_INFO *pBccRslt);
extern ER sie7_chgBurstLengthByMode(SIE_DRAM_BURST_MODE_SEL BurstMdSel);
extern UINT32 sie7_getClockRate(void);
// sie7_int.c
extern void sie7_getMainInput(SIE_MAIN_INPUT_INFO *pMainInInfo);
//NA//extern void sie7_setDramInStart(void);
//NA//extern void sie7_calcObPlnScl(SIE_OB_PLANE_INFO *pObpParam, SIE_OB_PLANE_ADJ_INFO *pObpAdjParam);
extern UINT32 sie7_getIntrStatus(void);
extern UINT32 sie7_getFunction(void);
extern void sie7_getEngineStatus(SIE_ENGINE_STATUS_INFO *pEngineStatus);
extern void sie7_getActiveWindow(SIE_ACT_WIN_INFO *pActWin);
extern void sie7_getCropWindow(SIE_CRP_WIN_INFO *pCrpWin);
extern void sie7_getDVI(SIE_DVI_INFO *pDviParam);
extern void sie7_getObOfs(SIE_OB_OFS_INFO *pObDtOfs);
//NA//extern void sie7_calcBSHScl(SIE_BS_H_INFO *pBshParam, SIE_BS_H_ADJ_INFO *pBshAdjParam);
//NA//extern void sie7_calcBSVScl(SIE_BS_V_INFO *pBsvParam, SIE_BS_V_ADJ_INFO *pBsvAdjParam);
//NA//extern void sie7_calcVIG(SIE_VIG_INFO *pVigParam, SIE_VIG_ADJ_INFO *pVigAdjParam);
//NA//extern void sie7_calcVIG_TwoImg(SIE_VIG_INFO *pVigParam, SIE_VIG_ADJ_2IMG_INFO *pVigAdjTwoImgParam);
extern void sie7_calcECSScl(SIE_ECS_INFO *pEcsParam, SIE_ECS_ADJ_INFO *pEcsAdjParam);
extern void sie7_calcCaLaSize(SIE_STCS_CALASIZE_GRP_INFO *pVaSzGrpInfo, SIE_STCS_CALASIZE_ADJ_INFO *pVaAdjInfo);
extern UINT32 sie7_getOutAdd(UINT32 uiOutChIdx, SIE_PINGPONG_BUFF_SEL uiBufSel);
extern UINT32 sie7_getOutAdd_Flip(UINT32 uiOutChIdx, SIE_PINGPONG_BUFF_SEL uiBufSel);
extern void sie7_getCAResult(SIE_STCS_CA_RSLT_INFO *CaRsltInfo);
//NA//extern void sie7_getCAFlipResult(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt, SIE_FLIP_SEL pFlipSel);
extern void sie7_getLaWin(SIE_LA_WIN_INFO *pSetting);
extern UINT32 sie7_getLaWinSum(void);
extern void sie7_getCAResultManual(SIE_STCS_CA_RSLT_INFO *CaRsltInfo, SIE_CA_WIN_INFO *CaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie7_getCAFlipResultManual(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt, SIE_CA_WIN_INFO *CaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie7_getHisto(UINT16 *puiHisto);
extern void sie7_getLAResult(UINT16 *puiBufLa1, UINT16 *puiBufLa2);
//NA//extern void sie7_getLAFlipResult(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_FLIP_SEL pFlipSel);
extern void sie7_getLAResultManual(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_LA_WIN_INFO *LaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie7_getLAFlipResultManual(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_LA_WIN_INFO *LaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie7_getVAResult(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V);
//NA//extern void sie7_getVAFlipResult(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie7_getVAResultManual(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_VA_WIN_INFO *VaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie7_getVAFlipResultManual(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_VA_WIN_INFO *VaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie7_getBccRslt(SIE_BCC_RSLT_INFO *pBccRslt);
//NA//extern void sie7_getVaIndepWinRslt(SIE_VA_INDEP_WIN_RSLT_INFO *pSetting, UINT32 uiIdx);
extern void sie7_getBurstLength(SIE_BURST_LENGTH *pBurstLen);


//NA//extern ER   sie5_SIE5_CLK_openClock(void);
//NA//extern ER   sie5_SIE5_CLK_closeClock(void);
//NA//extern BOOL sie5_SIE5_CLK_isClockOpened(void);
//NA//extern ER   sie5_SIE5_CLK_chgClockSource(SIE_CLKSRC_SEL ClkSrc);
//NA//extern ER   sie5_SIE5_CLK_chgClockRate(UINT32 uiClkRate);


#endif//_SIE7_LIB_H
//@}

//@}


