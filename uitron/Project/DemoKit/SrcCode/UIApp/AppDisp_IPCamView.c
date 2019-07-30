#include "AppDisp_IPCamView.h"
#include "GxImage.h"
#include "Color.h"
#include <stdio.h>
#include "SxCmd.h"

#define THIS_DBGLVL         2       // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          IPCamV
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"

#if _TODO
static INT32 m_iCleanCnt = 1; //indicate the times of cleaning screen, set 1 as default, for the first time clean screen

#if (TV_FUNC == ENABLE)
extern DX_HANDLE gDevTVObj;
#endif
#if (HDMI_FUNC == ENABLE)
extern DX_HANDLE gDevHDMIObj;
#endif
#if(IPCAM_FUNC != ENABLE)
#include "UIInfo.h"
#endif
void IPCamView_SetImg(ISIZE *pImgSize, USIZE *pAspectRatio)
{
}

INT32 IPCamView_OnDraw(DISPSRV_DRAW *pDraw) //PIP = Picture In Picture
{
	ER er;
	UINT32 i, n = pDraw->uiSrcCount;
	UINT32 uiLockIdx[2];
	IMG_BUF *pSrc = NULL;
	IMG_BUF *pSrcImg[2] = {0};
	IMG_BUF *pDst = pDraw->pImg[0];

#if 0
	{
#if (TV_FUNC == ENABLE)
		if (GxVideo_GetDevice(DOUT1) != (UINT32)gDevTVObj) {
			return E_OK; //if not insert TV yet, ignore this OnDraw
		}
#endif
#if (HDMI_FUNC == ENABLE)
		if (GxVideo_GetDevice(DOUT1) != (UINT32)gDevHDMIObj) {
			return E_OK; //if not insert HDMI yet, ignore this OnDraw
		}
#endif
	}
#endif

	for (i = 0; i < n; i++) {
		if ((er = pDraw->fpSrcLock(i, &uiLockIdx[i])) != E_OK) {
			DBG_ERR("failed to lock %d\r\n", i);
			return er;
		}
	}

	//--------------------Customer Draw Begin-----------------------------------
	//Clean Screen
	if (m_iCleanCnt > 0) {
		GxImg_FillData(pDst, NULL, COLOR_YUV_BLACK);
		m_iCleanCnt--;
	}

	pSrc = pSrcImg[0];
	if (pSrc != NULL) {
		GxImg_ScaleData(pSrc, NULL, pDst, NULL);
	}

	for (i = 0; i < n; i++) {
		if ((er = pDraw->fpSrcUnlock(i, uiLockIdx[i])) != E_OK) {
			DBG_ERR("failed to unlock %d\r\n", i);
			return er;
		}
	}
	//--------------------Customer Draw End-------------------------------------
	pDraw->fpFlip(TRUE);

	return E_OK;
}

#endif
