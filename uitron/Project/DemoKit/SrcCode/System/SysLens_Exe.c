/*
    System Lens Callback

    System Callback for Lens Module.

    @file       SysLens_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "DeviceCtrl.h"


#include "UIMode.h"
#include "UIModePhoto.h"
#include "UIAppMovie.h"
#include "ImageApp_Photo.h" //for NVTEVT_CB_ZOOM

#include "Lens.h"
#include "UIInfo.h" //for UI_SetData(), UI_GetData()
#include "GxFlash.h"
#include "SwTimer.h"


//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysLensExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


extern SX_CMD_ENTRY Cmd_lens[]; //device (lens)

int SX_TIMER_DET_CLOSE_LENS_ID = -1;

#if (LENS_FUNCTION == ENABLE)
SX_TIMER_ITEM(System_DetCloseLens, System_DetCloseLens, 50, TRUE)
#endif

extern void DrvLens_PowerOn(void);
extern void DrvLens_PowerOff(void);
extern void Photo_LensCB(UINT32 MessageID, UINT32 *Parameter);
extern void LensAPICB(UINT32 MessageID, UINT32 *Parameter);

void System_OnLensInit(void)
{
#if (LENS_FUNCTION == ENABLE)
    PLENSCTRL_OBJ plens_ctrl;

    DrvLens_RegLensCB((void *)Photo_LensCB, (void *)LensAPICB);

    if (System_GetState(SYS_STATE_POWERON) != SYS_POWERON_LOWPOWER) {
        DrvLens_PowerOn();

        if ((plens_ctrl = DrvLens_GetLensCtrl()) != NULL) {
            Lens_ModuleInit(plens_ctrl);
        }
    }

    ///W SX_TIMER_DET_CLOSE_LENS_ID = SxTimer_AddItem(&Timer_System_DetCloseLens);
#else
    Lens_FocusSetFocalLength(ZOOM_SECTION_WIDE, LENS_FOCAL_LENGTH);
#endif
}

void System_OnLensExit(void)
{
#if (LENS_FUNCTION == ENABLE)
    INT32 curr_state = System_GetState(SYS_STATE_POWEROFF);

    DBG_IND("power off state=%d\r\n", curr_state);

    if (curr_state != SYS_POWEROFF_LOWPOWER) {
        if (DevCtrl_Lens_PowerOff_IsNeedRetract()) {
            DevCtrl_Lens_PowerOff_Retract();
        }
    }

    DrvLens_PowerOff();
#endif
}

static UINT32 g_uiPBCloseLensCnt = 0;

void System_DetCloseLens(void)
{
#define CLOSE_LENS_MAX_COUNT  10   //10secs
    g_uiPBCloseLensCnt++;

    DBG_IND("Cnt=%d\r\n", g_uiPBCloseLensCnt);

    if ((UI_GetData(FL_ModeIndex) == DSC_MODE_PLAYBACK) && (UI_GetData(FL_NextMode) == DSC_MODE_PLAYBACK)  && UI_GetData(FL_LensOpen) && (g_uiPBCloseLensCnt >= CLOSE_LENS_MAX_COUNT)) {
        Ux_FlushEventByRange(NVTEVT_EXE_PB_RETRACT_LENS, NVTEVT_EXE_PB_RETRACT_LENS);
        Ux_PostEvent(NVTEVT_EXE_PB_RETRACT_LENS, 0);
    }

    if ((g_uiPBCloseLensCnt >= CLOSE_LENS_MAX_COUNT) || (UI_GetData(FL_ModeIndex) != DSC_MODE_PLAYBACK) || UI_GetData(FL_NextMode) != DSC_MODE_PLAYBACK) { //10secs
        g_uiPBCloseLensCnt = 0;
    }
}

void System_ResetDetCloseLenCount(void)
{
    g_uiPBCloseLensCnt = 0;
}

INT32 System_OnLensAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (LENS_FUNCTION == ENABLE)
    #if 0
    INT32 curr_mode = System_GetState(SYS_STATE_NEXTMODE);

    if (curr_mode == PRIMARY_MODE_PLAYBACK) {
        //DevCtrl_Lens(FALSE);  //delay this after enter Photo MODE!
    } else if (curr_mode == PRIMARY_MODE_PHOTO) {
        DevCtrl_Lens(TRUE);
    } else if (curr_mode == PRIMARY_MODE_MOVIE) {
        DevCtrl_Lens(TRUE);
    } else if (curr_mode == PRIMARY_MODE_USBPCC) {
        DevCtrl_Lens(TRUE);
    } else if (curr_mode == PRIMARY_MODE_IPCAM) {
        DevCtrl_Lens(TRUE);
    } else {
        DevCtrl_Lens(FALSE);
    }
    #endif
#endif

    return NVTEVT_CONSUME;
}

INT32 System_OnLensDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}


/////////////////////////////////////////////////////////////////////////////
// Power Off Flow
/////////////////////////////////////////////////////////////////////////////

BOOL DevCtrl_Lens_PowerOff_IsNeedRetract(void)
{
#if (LENS_FUNCTION == ENABLE)
    return (Lens_ModuleGetState() != LENS_STATE_IDLE) ? 1 : 0;
#else
    return 0;
#endif
}

void DevCtrl_Lens_PowerOff_Retract(void)
{
#if (LENS_FUNCTION == ENABLE)
    if (Lens_Retract() != ERR_OK) {
        DBG_ERR("lens error #1\r\n");
    } else if (!GxPower_GetControl(GXPWR_CTRL_IS_DUMMUYLOAD_POWEROFF)) {
        Lens_Init(LENS_INIT_ZOOM);
        Lens_Init(LENS_INIT_APERTURE);

        if (Lens_Retract() != ERR_OK) {
            DBG_ERR("lens error #2\r\n");
        }
    }
#endif
}


/////////////////////////////////////////////////////////////////////////////
// Playback Mode Control Flow
/////////////////////////////////////////////////////////////////////////////

void DevCtrl_Lens_Playback_Retract(void)
{
#if (LENS_FUNCTION == ENABLE)
    BOOL lens_init_err = FALSE;

    // during lens retract, does not detect key
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID, FALSE);
    lens_init_err = Lens_Retract();
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID, TRUE);

    if (lens_init_err == TRUE) {
    #if (LENSERROFF_FUNCTION == ENABLE)
        System_PowerOff(SYS_POWEROFF_LENSERROR);
    #endif
    }

    UI_SetData(FL_LensOpen, FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID, FALSE);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Photo Mode Control Flow
/////////////////////////////////////////////////////////////////////////////
void DevCtrl_LensDriverInit(void)
{
#if (LENS_FUNCTION == ENABLE)
    PLENSCTRL_OBJ plens_ctrl;

    if ((plens_ctrl = DrvLens_GetLensCtrl()) != NULL) {
        Lens_DriverInit(plens_ctrl);
    } else {
        DBG_ERR("lens driver init fail!\r\n");
    }
#endif
}

/**
  Control the devices for Photo mode

  Control the devices before entering Photo mode.

  @param void
  @return void
*/
BOOL DevCtrl_Lens(BOOL bIsOpen)
{
#if (LENS_FUNCTION == ENABLE)
    BOOL ret_status = TRUE, lens_init_err = TRUE;
    INT32 ret_value = ERR_OK;

    if (bIsOpen) {
        if (!(Lens_ModuleGetState() & (LENS_STATE_ZOOM_INIT|LENS_STATE_FOCUS_INIT))) {
            ret_value = Lens_Init(LENS_INIT_ZOOM);

            if (ret_value == ERR_OK) {
                Lens_Init(LENS_INIT_APERTURE);

                if (Lens_Init(LENS_INIT_FOCUS) == ERR_OK) {
                    lens_init_err = FALSE;
                }
            } else if (ret_value == ERR_LENS_INIT_FAIL) {
                DBG_IND("lens retract\r\n");
                Lens_Retract();

                ret_value = Lens_Init(LENS_INIT_ZOOM);

                SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID, TRUE);

                if (ret_value == ERR_OK) {
                    Lens_Init(LENS_INIT_APERTURE);

                    if (Lens_Init(LENS_INIT_FOCUS) == ERR_OK) {
                        lens_init_err = FALSE;
                    } else {
                        lens_init_err = TRUE;
                    }
                } else {
                        lens_init_err = TRUE;
                }
            }

            if (lens_init_err == TRUE) {
    #if (LENSERROFF_FUNCTION == ENABLE)
                DBG_ERR("initiation fail -> power off\r\n");
                System_PowerOff(SYS_POWEROFF_LENSERROR);
                ret_status = FALSE;
    #endif
            }
        }
    }

    return ret_status;
#else
    return TRUE;
#endif
}

