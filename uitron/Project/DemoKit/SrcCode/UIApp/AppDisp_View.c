#include "Color.h"
#include <stdio.h>
#include "SysCfg.h"
#include "AppDisp_View.h"
#include "AppDisp_SbsView.h"
#include "AppDisp_PipView.h"

#define THIS_DBGLVL         2       // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          AppDisp
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

APPDISP_VIEW_OPS  gAppDispView_ops = {0};


void            AppDispView_Init(void)
{
	#if (SBS_VIEW_FUNC == ENABLE)
	gAppDispView_ops.SetStyle = SbsView_SetStyle;
	gAppDispView_ops.GetInfo  = SbsView_GetInfo;
	gAppDispView_ops.OnDraw   = SbsView_OnDraw;
	#else
    gAppDispView_ops.SetStyle = PipView_SetStyle;
	gAppDispView_ops.GetInfo  = PipView_GetInfo;
	gAppDispView_ops.OnDraw   = PipView_OnDraw;
	#endif

}
void            AppDispView_SetStyle(UINT32 uiStyle)
{
	if (!gAppDispView_ops.SetStyle){
		DBG_ERR("Not Support\r\n");
	}
	gAppDispView_ops.SetStyle(uiStyle);
}

INT32           AppDispView_GetInfo(APPDISP_VIEW_INFO* Info)
{
	if (!gAppDispView_ops.GetInfo) {
		DBG_ERR("Not Support\r\n");
		return E_SYS;
	}
	return gAppDispView_ops.GetInfo(Info);

}

INT32           AppDispView_OnDraw(APPDISP_VIEW_DRAW *pDraw)
{
	if (!gAppDispView_ops.OnDraw){
		DBG_ERR("Not Support\r\n");
		return E_SYS;
	}
	return gAppDispView_ops.OnDraw(pDraw);
}