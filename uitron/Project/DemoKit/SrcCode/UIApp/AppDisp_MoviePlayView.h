#include <stdio.h>
#include "Color.h"
#include "SxCmd.h"
#include "PrjCfg.h"
#include "SysKer.h"
#include "SysCommon.h"
#include "SizeConvert.h"
#include "GxImage.h"
#include "GxVideo.h"
#include "ImageStream.h"
#include "ImageUnit_UserProc.h"
#include "ImageApp_MoviePlay.h"
#include "UIAppPlay.h"

#define FLG_APPDISP_MOVIEPLAYVIEW_IDLE		FLGPTN_BIT(0)
#define FLG_APPDISP_MOVIEPLAYVIEW_CLOSE		FLGPTN_BIT(1)
#define FLG_APPDISP_MOVIEPLAYVIEW_TRIG		FLGPTN_BIT(2)

typedef MEM_RANGE(*MoviePlay_GetBuf)(UINT32 id);

extern void AppDisp_MoviePlayView_Open(void);
extern void AppDisp_MoviePlayView_Close(void);
extern void AppDisp_MoviePlayView_OnDraw(void);
extern void AppDisp_MoviePlayView_Tsk(void);
extern void AppDisp_MoviePlayView_Init(USIZE *pDispAR);
extern UINT32 AppDisp_MoviePlayView_GetCurrPlaySecs(void);
extern UINT32 AppDisp_MoviePlayView_GetDispIdx(void);

extern UINT32 _SECTION(".kercfg_data") APPDISP_MOVIEPLAYVIEW_FLG_ID;
extern UINT32 _SECTION(".kercfg_data") APPDISP_MOVIEPLAYVIEW_TSK_ID;

extern void AppDisp_MoviePlayView_InstallID(void) _SECTION(".kercfg_text");

