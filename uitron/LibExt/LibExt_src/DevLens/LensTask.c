/**
 * Novatek lens task function.
 * 
 * Lens task implementation.
 * 
 * @file LensTask.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include "kernel.h"
#include "SysKer.h"
#include "LensAPI.h"


#define __MODULE__          LENS_TASK
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    // *=All, [mark]=CustomClass
#include "DebugModule.h"


typedef struct {
    LENS_ZOOM_FUNC     eZoomFunc;            ///< zoom set function
    UINT32             uiZoomParam;          ///< parameter used to set zoom function
    LENS_FOCUS_FUNC    eFocusFunc;           ///< focus set function
    UINT32             uiFocusParam;         ///< parameter used to set focus function
    MOTOR_APERTURE_ACT eApertureState;       ///< aperture set state
    UINT32             uiApertureParam;      ///< parameter used to set aperture state
    MOTOR_SHUTTER_ACT  eShutterState;        ///< shutter set state
    UINT32             uiShutterParam;       ///< parameter used to set shutter state
    MOTOR_IRCUT_ACT    eIRCUTState;          ///< IRCUT set state
    UINT32             uiIRCUTParam;         ///< parameter used to set IRCUT state
} LENSCTRL_PARAM;

static BOOL g_bLensCtrlOpened = FALSE;

static LENSCTRL_OBJ g_LensCtrl;

static LENSCTRL_PARAM g_LensCtrlParam =
{
    .eZoomFunc       = LENS_ZOOM_FUNC_UNKNOWN,
    .uiZoomParam     = 0,
    .eFocusFunc      = LENS_FOCUS_FUNC_UNKNOWN,
    .uiFocusParam    = 0,
    .eApertureState  = MOTOR_APERTURE_EXCITE_OFF,
    .uiApertureParam = 0,
    .eShutterState   = MOTOR_SHUTTER_NORMAL,
    .uiShutterParam  = 0,
    .eIRCUTState     = MOTOR_IRCUT_OPEN,
    .uiIRCUTParam    = 0,
};


static void LensCtrl_SetSignal(MD_SIGNAL signal, BOOL on)
{
    g_LensCtrl.pLens->setSignal(signal, on);
}

static void LensCtrl_ZoomSetFunc(LENS_ZOOM_FUNC zoom_func, UINT32 param)
{
    FLGPTN flag, mask;

    mask = FLGLENS_IDLE;

    wai_flg(&flag, g_uiLensFlagID, mask, TWF_ORW|TWF_CLR);

    g_LensCtrlParam.eZoomFunc = zoom_func;
    g_LensCtrlParam.uiZoomParam = param;

    set_flg(g_uiLensFlagID, FLGLENS_ZOOM_SETSTATE);
}

static void LensCtrl_FocusSetFunc(LENS_FOCUS_FUNC focus_func, UINT32 param)
{
    FLGPTN flag, mask;

    mask = FLGLENS_IDLE;

    wai_flg(&flag, g_uiLensFocusFlagID, mask, TWF_ORW|TWF_CLR);

    g_LensCtrlParam.eFocusFunc = focus_func;
    g_LensCtrlParam.uiFocusParam = param;

    set_flg(g_uiLensFocusFlagID, FLGLENS_FOCUS_SETSTATE);
}

static void LensCtrl_ApertureSetState(MOTOR_APERTURE_ACT state, UINT32 param)
{
    FLGPTN flag, mask;

    mask = FLGLENS_IDLE;

    wai_flg(&flag, g_uiLensFlagID, mask, TWF_ORW|TWF_CLR);

    g_LensCtrlParam.eApertureState = state;
    g_LensCtrlParam.uiApertureParam = param;

    set_flg(g_uiLensFlagID, FLGLENS_APERTURE_SETSTATE);
}

static void LensCtrl_ShutterSetState(MOTOR_SHUTTER_ACT state, UINT32 param)
{
    FLGPTN flag, mask;

    mask = FLGLENS_IDLE;

    wai_flg(&flag, g_uiLensFlagID, mask, TWF_ORW|TWF_CLR);

    g_LensCtrlParam.eShutterState = state;
    g_LensCtrlParam.uiShutterParam = param;

    set_flg(g_uiLensFlagID, FLGLENS_SHUTTER_SETSTATE);
}

static void LensCtrl_IRCUTSetState(MOTOR_IRCUT_ACT state, UINT32 param)
{
    FLGPTN flag, mask;

    mask = FLGLENS_IDLE;

    wai_flg(&flag, g_uiLensFlagID, mask, TWF_ORW|TWF_CLR);

    g_LensCtrlParam.eIRCUTState = state;
    g_LensCtrlParam.uiIRCUTParam = param;

    set_flg(g_uiLensFlagID, FLGLENS_IRCUT_SETSTATE);
}


ER LensCtrl_Open(PLENSCTRL_OBJ plens_ctrl)
{
    if (g_bLensCtrlOpened == TRUE) {
        return E_SYS;
    }

    clr_flg(g_uiLensFlagID, FLGLENS_ALL);
    clr_flg(g_uiLensFocusFlagID, FLGLENS_ALL);

    plens_ctrl->pLens->pLensCtrlTsk->lenstsk_setSignal         = LensCtrl_SetSignal;
    plens_ctrl->pLens->pLensCtrlTsk->lenstsk_zoom_setFunc      = LensCtrl_ZoomSetFunc;
    plens_ctrl->pLens->pLensCtrlTsk->lenstsk_focus_setFunc     = LensCtrl_FocusSetFunc;
    plens_ctrl->pLens->pLensCtrlTsk->lenstsk_aperture_setState = LensCtrl_ApertureSetState;
    plens_ctrl->pLens->pLensCtrlTsk->lenstsk_shutter_setState  = LensCtrl_ShutterSetState;
    plens_ctrl->pLens->pLensCtrlTsk->lenstsk_ircut_setState    = LensCtrl_IRCUTSetState;

    memcpy(&g_LensCtrl, plens_ctrl, sizeof(LENSCTRL_OBJ));

    sta_tsk(g_uiLensTaskID, 0);
    sta_tsk(g_uiLensFocusTaskID, 0);

    g_bLensCtrlOpened = TRUE;

    return E_OK;
}

ER LensCtrl_Close(void)
{
    if (g_bLensCtrlOpened == FALSE) {
        return E_SYS;
    }

    g_bLensCtrlOpened = FALSE;

    ter_tsk(g_uiLensTaskID);
    ter_tsk(g_uiLensFocusTaskID);

    return E_OK;
}

void LensCtrl_Tsk(void)
{
    FLGPTN ctrl_flag, ctrl_mask;

    kent_tsk();

    ctrl_mask = FLGLENS_ZOOM_SETSTATE | FLGLENS_APERTURE_SETSTATE | FLGLENS_SHUTTER_SETSTATE | FLGLENS_IRCUT_SETSTATE;

    clr_flg(g_uiLensFlagID, ctrl_mask);

    while (g_uiLensTaskID) {
        PROFILE_TASK_IDLE();
        set_flg(g_uiLensFlagID, FLGLENS_IDLE);
        wai_flg(&ctrl_flag, g_uiLensFlagID, ctrl_mask, TWF_ORW|TWF_CLR);
        PROFILE_TASK_BUSY();

        DBG_MSG("0x%08X", ctrl_flag);

        if (ctrl_flag & FLGLENS_ZOOM_SETSTATE) {
            switch (g_LensCtrlParam.eZoomFunc) {
            case LENS_ZOOM_INIT:
                Lens_Init(LENS_INIT_ZOOM);
                break;

            case LENS_ZOOM_GO2_SECTION:
                Lens_ZoomGo2Section(g_LensCtrlParam.uiZoomParam);
                break;

            case LENS_ZOOM_GO2_POSITION:
                Lens_ZoomDoAction((INT32)g_LensCtrlParam.uiZoomParam);
                break;

            case LENS_ZOOM_IN:
                Lens_ZoomIn();
                break;

            case LENS_ZOOM_OUT:
                Lens_ZoomOut();
                break;

            case LENS_ZOOM_RETRACT:
                Lens_ZoomRetract();
                break;

            default:
            case LENS_ZOOM_FUNC_UNKNOWN:
                break;
            }
        }

        if (ctrl_flag & FLGLENS_APERTURE_SETSTATE) {
            Lens_ApertureSetPosition((IRIS_POS)g_LensCtrlParam.uiApertureParam);    // g_LensCtrlParam.eApertureState
        }

        if (ctrl_flag & FLGLENS_SHUTTER_SETSTATE) {
            Lens_ShutterSetState((MOTOR_SHUTTER_ACT)g_LensCtrlParam.eShutterState, g_LensCtrlParam.uiShutterParam);
        }

        if (ctrl_flag & FLGLENS_IRCUT_SETSTATE) {
            Lens_IRCUTSetState((IRCUT_POS)g_LensCtrlParam.eIRCUTState, g_LensCtrlParam.uiIRCUTParam);
        }
    }
}

void LensFocusCtrl_Tsk(void)
{
    FLGPTN ctrl_flag, ctrl_mask;

    kent_tsk();

    ctrl_mask = FLGLENS_FOCUS_SETSTATE;

    clr_flg(g_uiLensFocusFlagID, ctrl_mask);

    while (g_uiLensFocusTaskID) {
        PROFILE_TASK_IDLE();
        set_flg(g_uiLensFocusFlagID, FLGLENS_IDLE);
        wai_flg(&ctrl_flag, g_uiLensFocusFlagID, ctrl_mask, TWF_ORW|TWF_CLR);
        PROFILE_TASK_BUSY();

        DBG_MSG("0x%08X", ctrl_flag);

        if (ctrl_flag & FLGLENS_FOCUS_SETSTATE) {
            switch (g_LensCtrlParam.eFocusFunc) {
            case LENS_FOCUS_INIT:
                Lens_Init(LENS_INIT_FOCUS);
                break;

            case LENS_FOCUS_GO2_POSITION:
                Lens_FocusDoAction(FOCUS_RUN, (INT32)g_LensCtrlParam.uiFocusParam);
                break;

            case LENS_FOCUS_TELE:
                Lens_FocusTele();
                break;

            case LENS_FOCUS_WIDE:
                Lens_FocusWide();
                break;

            case LENS_FOCUS_RETRACT:
                Lens_FocusRetract((FOCUS_RETRACT_POSITION)g_LensCtrlParam.uiFocusParam);
                break;

            default:
            case LENS_FOCUS_FUNC_UNKNOWN:
                break;
            }
        }
    }
}


