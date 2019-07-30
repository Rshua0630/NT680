#include "SysKer.h"
#include "display.h"
#include "Color.h"
#include "SizeConvert.h"
#include "AppDisp_3DNRView.h"
//#include "AppDisp_ScaleView.h"
#include "PrjCfg.h"
#include "GxVideo.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          3DNRVIEW
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//#define USE_DISPSRV

static BOOL gAppDisp3DNRView_StartPlay = FALSE;

#ifdef USE_DISPSRV
static BOOL gbCleanFirst = TRUE;

static ER AppDisp_3DNRView_OnLock(UINT32 *puiLocked, UINT32 uiIdx)
{
	*puiLocked = 0;
	return E_OK;
}

static ER AppDisp_3DNRView_OnUnlock(UINT32 uiIdx)
{
	// Do nothing
	return E_OK;
}

static ER AppDisp_3DNRView_OnDraw(DISPSRV_DRAW *pDraw)
{
	ER       er;
	UINT32   uiLockIdx;
	IMG_BUF *pSrc = NULL;
	IMG_BUF *pDst = pDraw->pImg[0];
	IRECT SrcRegion = {0};

	if (gAppDisp3DNRView_StartPlay == FALSE) {
		return E_OK;    // Media Play has been closed on dual-view mode
	}

	if ((er = pDraw->fpSrcLock(0, &uiLockIdx)) != E_OK) {
		//locked fail indicate skip this draw
		return er;
	}

	pSrc = &pDraw->pDS[DISPSRV_SRC_IDX_PRIMARY]->pSrc[uiLockIdx];

	if (pSrc->PxlAddr[0]) {
		SrcRegion.x = 0;
		SrcRegion.y = 0;
		SrcRegion.w = pSrc->Width;
		SrcRegion.h = (pSrc->Height == 1088) ? 1080 : pSrc->Height;

		//clean screen
		if (gbCleanFirst) {
			gbCleanFirst = FALSE;
			GxImg_FillData(pDst, NULL, COLOR_YUV_BLACK);
		}

		GxImg_ScaleData(pSrc, &SrcRegion, pDst, NULL);
	}

	pDraw->fpFlip(TRUE);
	pDraw->fpSrcUnlock(0, uiLockIdx);
	return E_OK;
}
#endif

void AppDisp_3DNRView_Start(void)
{
	gAppDisp3DNRView_StartPlay = TRUE;
}

void AppDisp_3DNRView_Stop(void)
{
	gAppDisp3DNRView_StartPlay = FALSE;
}

void AppDisp_3DNRView_Cfg(IMG_BUF *pSrc)
{
#ifdef USE_DISPSRV
	DISPSRV_CMD     Cmd = {0};
	DISPSRV_SRC_CFG CfgSrc = {0};
	UINT32          fpDrawCb;

	gbCleanFirst = TRUE;
	// Configure
	CfgSrc.fpLockCb[DISPSRV_SRC_IDX_PRIMARY] = (DISPSRV_FP_LOCK_CB) AppDisp_3DNRView_OnLock;
	CfgSrc.fpUnlockCb[DISPSRV_SRC_IDX_PRIMARY] = (DISPSRV_FP_UNLOCK_CB) AppDisp_3DNRView_OnUnlock;
	CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].uiNumSrc = 1;
	CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].pSrc = pSrc;
	CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].pSrcRect = NULL;  //No DIS

	//Set the source buffer
	memset(&Cmd, 0, sizeof(DISPSRV_CMD));
	Cmd.Prop.bExitCmdFinish = TRUE;
	Cmd.Idx = DISPSRV_CMD_IDX_SET_SRC;
	Cmd.In.uiNumByte = sizeof(CfgSrc);
	Cmd.In.pData = &CfgSrc;
	DispSrv_Cmd(&Cmd);

	// Set the Draw Callback
	fpDrawCb = (UINT32) AppDisp_3DNRView_OnDraw;
	memset(&Cmd, 0, sizeof(DISPSRV_CMD));
	Cmd.Prop.bExitCmdFinish = TRUE;
	Cmd.Idx = DISPSRV_CMD_IDX_SET_DRAW_CB;
	Cmd.In.uiNumByte = sizeof(fpDrawCb);
	Cmd.In.pData = &fpDrawCb;
	DispSrv_Cmd(&Cmd);
#endif
}

void AppDisp_3DNRView_Trigger(void)
{
#ifdef USE_DISPSRV
	DispSrv_TriggerNext();
#endif
}



