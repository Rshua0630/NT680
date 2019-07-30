////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMain.h"
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
//#include "UIMainWnd.h"
//UIInfo
#include "UIInfo.h"
#include "nvtmpp.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          mode_main
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

int PRIMARY_MODE_MAIN = -1;      ///< Main

//#NT#2016/12/02#Niven Cho -begin
//#NT#MAIN mode use whole POOL_ID_APP as temp buffer
extern UINT32 User_GetTempBuffer(UINT32 uiSize);
//#NT#2016/12/02#Niven Cho -end
extern UINT32 User_RelTempBuffer(UINT32 addr);

void ModeMain_Open(void);
void ModeMain_Close(void);


static INT32 ModeMain_InitMem(void)
{

	NVTMPP_ER        ret;
	NVTMPP_VB_CONF_S st_conf;

	memset(&st_conf, 0, sizeof(NVTMPP_VB_CONF_S));
	st_conf.ddr_mem[0].Addr = OS_GetMempoolAddr(POOL_ID_APP);
	st_conf.ddr_mem[0].Size = OS_GetMempoolSize(POOL_ID_APP);
	st_conf.max_pool_cnt = 64;

	st_conf.common_pool[0].blk_size = 4;
	st_conf.common_pool[0].blk_cnt = 1;
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;

	ret = nvtmpp_vb_set_conf(&st_conf);
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp set vb err: %d\r\n", ret);
		return -1;
	}
	ret = nvtmpp_vb_init();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}
	return 0;
}

static INT32 ModeMain_ExitMem(void)
{
	NVTMPP_ER        ret;
	ret = nvtmpp_vb_exit();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp exit vb err: %d\r\n", ret);
		return -1;
	}
	return 0;
}

void ModeMain_Open(void)
{
	ModeMain_InitMem();
	//#NT#2016/12/02#Niven Cho -begin
	//#NT#MAIN mode use whole POOL_ID_APP as temp buffer
	SxCmd_RegTempMemFunc(User_GetTempBuffer);
	//#NT#2016/12/02#Niven Cho -end
	SxCmd_RegRelTempMemFunc(User_RelTempBuffer);

	Ux_SetActiveApp(&UISetupObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
#if (CALIBRATION_FUNC == ENABLE)
		if (EngineerMode_CheckEng())
		{
			EngineerMode_Open();

			// check if enter engineer mode
			if (IsEngineerModeOpened())
			{
				EngineerMode_WndOpen();
			}
		}
#endif
	    //Ux_OpenWindow((VControl *)(&UIMainWndCtrl), 0);
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnOpen) {
		    pCurSubMode->pfSubMode_OnOpen();
        }
	}
#endif

}
void ModeMain_Close(void)
{

#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
#if (CALIBRATION_FUNC == ENABLE)
		// check if enter engineer mode
		if (IsEngineerModeOpened())
		{
			EngineerMode_WndClose();
		}
#endif
		//Ux_CloseWindow((VControl *)(&UIMainWndCtrl), 0);
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnClose) {
		    pCurSubMode->pfSubMode_OnClose();
        }
	}
#endif


	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
	ModeMain_ExitMem();

}

SYS_MODE gModeMain = {
	"MAIN",
	ModeMain_Open,
	ModeMain_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
