/**
 * Novatek lens API function.
 *
 * Lens API implementation.
 *
 * @file LensAPI.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include "Type.h"
#include "kernel.h"
#include "PStore.h"
#include "Lens.h"
#include "LensAPI.h"


#define __MODULE__          LENS_API
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    // *=All, [mark]=CustomClass
#include "DebugModule.h"


#define PS_FOCUS_DATA             "AUTO-FOCUS"    // name section name of PStore to save calibration data


static BOOL g_bLensCaliData = FALSE, g_bLensSemaInit = FALSE;
static LENSCTRL_OBJ g_LensCtrl;
static LENS_STATE g_uiLensModuleState = LENS_STATE_IDLE;
static UINT32 g_lens_focal_length = 2880;    // unit: um


void Lens_ModuleInit(PLENSCTRL_OBJ plens_ctrl)
{
#if 0
    INT16 tmp_idx[16] = {0};
    UINT32 i, cali_tbl_size = Lens_ZoomGetSection(ZOOM_MAX_SECTION);
    PPSTORE_SECTION_HANDLE psec_location = NULL;
#endif

    if ((plens_ctrl->pLens == NULL) || (plens_ctrl->pLensDev == NULL)) {
        DBG_ERR("%s param error!!\r\n", __func__);

        return;
    }

    memcpy(&g_LensCtrl, plens_ctrl, sizeof(LENSCTRL_OBJ));

    // assign semaphore ID
    g_LensCtrl.semid = g_uiLensCtrlSemaID;

    if (g_LensCtrl.semid == LENS_SEMAID_INIT_VALUE) {
        DBG_ERR("%s semaphore init error!!\r\n", __func__);
    } else {
        g_bLensSemaInit = (g_LensCtrl.pLens != NULL) ? TRUE : FALSE;
    }

#if 0    // read lens calibration data from flash ROM
    if (sizeof(tmp_idx) >= cali_tbl_size) {
        if ((INT32)(psec_location = PStore_OpenSection(PS_FOCUS_DATA, PS_RDONLY)) != E_PS_SECHDLER) {
            PStore_ReadSection((UINT8 *)&tmp_idx, 0, cali_tbl_size * sizeof(INT16), psec_location);
            PStore_CloseSection(psec_location);

            for (i = 0; i < cali_tbl_size; i++) {
                DBG_MSG("^GZoom sec_%d: peak_idx:%d\r\n", i+1, tmp_idx[i]);
            }

            if (Lens_FocusSetCaliData(tmp_idx, cali_tbl_size) == ERR_OK) {
                g_bLensCaliData = TRUE;
            } else {
                DBG_WRN("set focus cali data failure!\r\n");
            }
        } else {
            DBG_WRN("no focus calibration data!\r\n");
        }
    } else {
        DBG_WRN("buffer size isn't enough for AF cali data!\r\n");
    }
#endif
}

LENS_STATE Lens_ModuleGetState(void)
{
    return g_uiLensModuleState;
}

static void Lens_ModuleSetState(LENS_STATE state, BOOL setup)
{
    if (g_bLensSemaInit == TRUE) {
        wai_sem(g_LensCtrl.semid);

        if (setup == TRUE) {
            g_uiLensModuleState |= state;
        } else {
            g_uiLensModuleState &= ~state;
        }

        sig_sem(g_LensCtrl.semid);
    } else {
        //DBG_ERR("%s g_LensCtrl.semid init error\r\n", __func__);
    }
}


//----------------------------------------------------------------------------------------------------------------------
// general
//----------------------------------------------------------------------------------------------------------------------
void Lens_DriverInit(PLENSCTRL_OBJ plens_ctrl)
{
    // init motor
    g_LensCtrl.pLens->pMotor->init(plens_ctrl->pLensDev);

    // init lens
    g_LensCtrl.pLens->init(plens_ctrl->pLensDev);

    // open lens task
    LensCtrl_Open(plens_ctrl);

    Lens_InstallCmd();
}

INT32 Lens_Init(LENS_INIT_STATE init_item_sel)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    }

    switch (init_item_sel) {
    case LENS_INIT_ZOOM:
        DBG_MSG("start of zoom init\r\n");

        if (g_LensCtrl.pLens->zoom_init) {
            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_INITZOOM_START, NULL);
            }

            ret_status = g_LensCtrl.pLens->zoom_init(0);

            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_INITZOOM_END, NULL);
            }

            Lens_ModuleSetState(LENS_STATE_ZOOM_INIT, TRUE);
        } else {
            ret_status = ERR_FUNC_PTR_NULL;
        }

        DBG_MSG("end of zoom init\r\n");
        break;

    case LENS_INIT_FOCUS:
        DBG_MSG("start of focus init\r\n");

        if (g_LensCtrl.pLens->focus_init) {
            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_INITFOCUS_START, NULL);
            }

            if (g_LensCtrl.pLens->makeAFTable && (g_bLensCaliData == TRUE)) {
                g_LensCtrl.pLens->makeAFTable();
            }

            ret_status = g_LensCtrl.pLens->focus_init();

            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_INITFOCUS_END, NULL);
            }

            Lens_ModuleSetState(LENS_STATE_FOCUS_INIT, TRUE);
        } else {
            ret_status = ERR_FUNC_PTR_NULL;
        }

        DBG_MSG("end of focus init\r\n");
        break;

    case LENS_INIT_APERTURE:
        DBG_MSG("start of IRIS init\r\n");

        if (g_LensCtrl.pLens->aperture_init) {
            Lens_ModuleSetState(LENS_STATE_APERTURE_MOVING, TRUE);
            DBG_MSG("LENS_STATE_APERTURE_MOVING\r\n");

            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_INITAPERTURE_START, NULL);
            }

            g_LensCtrl.pLens->aperture_init();

            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_INITAPERTURE_END, NULL);
            }

            Lens_ModuleSetState(LENS_STATE_APERTURE_MOVING, FALSE);
        } else {
            ret_status = ERR_FUNC_PTR_NULL;
        }

        DBG_MSG("end of IRIS init\r\n");
        break;

    default:
        DBG_ERR("init selection err!\r\n");
        ret_status = ERR_OPERATION_FAIL;
        break;
    }

    return ret_status;
}

INT32 Lens_Uninit(void)
{
    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (Lens_ModuleGetState() & ~(LENS_STATE_ZOOM_INIT|LENS_STATE_FOCUS_INIT)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    Lens_ModuleSetState(LENS_STATE_POWER_OFF, TRUE);
    DBG_MSG("LENS_STATE_POWER_OFF\r\n");

    if (g_LensCtrl.pLens->lensPowerOff) {
        g_LensCtrl.pLens->lensPowerOff();

        return ERR_OK;
    } else {
        return ERR_FUNC_PTR_NULL;
    }
}

INT32 Lens_Retract(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    }

    if (g_LensCtrl.pLens->lensRetract && g_LensCtrl.pLens->shutter_setState) {
        ret_status = g_LensCtrl.pLens->lensRetract();

        g_LensCtrl.pLens->shutter_setState(MOTOR_SHUTTER_NORMAL, CLOSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    Lens_ModuleSetState(LENS_STATE_ZOOM_INIT|LENS_STATE_FOCUS_INIT, FALSE);

    return ret_status;
}

INT32 Lens_Reset(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (Lens_ModuleGetState() & ~(LENS_STATE_ZOOM_INIT|LENS_STATE_FOCUS_INIT)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->lensReset) {
        Lens_ModuleSetState(LENS_STATE_RESET, TRUE);
        DBG_MSG("LENS_STATE_RESET\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_RESET_START, NULL);
        }

        ret_status = g_LensCtrl.pLens->lensReset();

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_RESET_END, NULL);
        }

        Lens_ModuleSetState(LENS_STATE_RESET, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}


//----------------------------------------------------------------------------------------------------------------------
// misc
//----------------------------------------------------------------------------------------------------------------------
UINT32 Lens_GetCapabilityInfo(void)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->lensGetInfo) {
            return g_LensCtrl.pLens->lensGetInfo();
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

UINT32 Lens_GetMDSignal(MD_SIGNAL signal)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->getSignal) {
            return g_LensCtrl.pLens->getSignal(signal);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

void Lens_SetMDSignal(MD_SIGNAL signal, BOOL on)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->setSignal) {
            g_LensCtrl.pLens->setSignal(signal, on);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }
}

UINT32 Lens_GetBusyState(void)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->getBusyState) {
            return g_LensCtrl.pLens->getBusyState();
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}


UINT32 Lens_GetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->getZoomFocusTable) {
            return g_LensCtrl.pLens->getZoomFocusTable(tbl_idx, zf_tbl);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}


UINT32 Lens_SetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->setZoomFocusTable) {
            g_LensCtrl.pLens->setZoomFocusTable(tbl_idx, zf_tbl);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}


//----------------------------------------------------------------------------------------------------------------------
// zoom
//----------------------------------------------------------------------------------------------------------------------
void Lens_ZoomSetFunc(LENS_ZOOM_FUNC zoom_func, UINT32 param)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_zoom_setFunc) {
            g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_zoom_setFunc(zoom_func, param);
        }
    }
}

UINT32 Lens_ZoomGetSpeed(void)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->zoom_getSpeed) {
            return g_LensCtrl.pLens->zoom_getSpeed();
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

void Lens_ZoomSetSpeed(LENS_SPEED_CATEGORY category)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->zoom_setSpeed) {
            g_LensCtrl.pLens->zoom_setSpeed(category);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }
}

UINT32 Lens_ZoomGetSection(ZOOM_SECTION_CATEGORY category)
{
    UINT32 zoom_sec = 0;

    if (g_bLensSemaInit == FALSE) {
        return 0;
    }

    if (g_LensCtrl.pLens->zoom_getMinSection && g_LensCtrl.pLens->zoom_getMaxSection && g_LensCtrl.pLens->zoom_getSection) {
        switch (category) {
        case ZOOM_MIN_SECTION:
            zoom_sec = g_LensCtrl.pLens->zoom_getMinSection();
            break;

        case ZOOM_MAX_SECTION:
            zoom_sec = g_LensCtrl.pLens->zoom_getMaxSection();
            break;

        case ZOOM_CURR_SECTION:
        default:
            if (Lens_ModuleGetState() & LENS_STATE_ZOOM_INIT) {
                zoom_sec = g_LensCtrl.pLens->zoom_getMinSection();
            } else {
                zoom_sec = g_LensCtrl.pLens->zoom_getSection();
            }
            break;
        }
    } else {
        DBG_ERR("lens obj func is not registered!\r\n");
    }

    return zoom_sec;
}

void Lens_ZoomGo2Section(UINT32 section)
{
    if (g_bLensSemaInit == FALSE) {
        return;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_ZOOM_INIT) || (Lens_ModuleGetState() & LENS_STATE_ZOOM_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return;
    }

    if (g_LensCtrl.pLens->zoom_go2Section) {
        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, TRUE);
        DBG_MSG("LENS_STATE_ZOOM_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_START, NULL);
        }

        g_LensCtrl.pLens->zoom_go2Section(section);

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_END, NULL);
        }

        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, FALSE);
    } else {
        DBG_ERR("lens obj func is not registered!\r\n");
    }
}

INT32 Lens_ZoomGetPosition(ZOOM_POS_CATEGORY category)
{
    INT32 zoom_pos = 0;

    if (g_bLensSemaInit == FALSE) {
        return 0;
    }

    if (g_LensCtrl.pLens->zoom_getSectionPosition && g_LensCtrl.pLens->zoom_getPosition) {
        switch (category) {
        case ZOOM_CURR_SECTION_POS:
            zoom_pos = g_LensCtrl.pLens->zoom_getSectionPosition();
            break;

        case ZOOM_CURR_POS:
        default:
            zoom_pos = g_LensCtrl.pLens->zoom_getPosition();
            break;
        }
    } else {
        DBG_ERR("lens obj func is not registered!\r\n");
    }

    return zoom_pos;
}

void Lens_ZoomSetPosition(INT32 position, INT32 dir)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->zoom_setPostion) {
            g_LensCtrl.pLens->zoom_setPostion(position, dir);

            Lens_ModuleSetState(LENS_STATE_ZOOM_INIT, TRUE);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }
}

void Lens_ZoomDoAction(INT32 position)
{
    if (g_bLensSemaInit == FALSE) {
        return;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_ZOOM_INIT) || (Lens_ModuleGetState() & LENS_STATE_ZOOM_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return;
    }

    if (g_LensCtrl.pLens->zoom_go2Position && g_LensCtrl.pLens->zoom_getPosition) {
        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, TRUE);
        DBG_MSG("LENS_STATE_ZOOM_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_START, NULL);
        }

        g_LensCtrl.pLens->zoom_go2Position(position);

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_END, NULL);
        }

        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, FALSE);
    } else {
        DBG_ERR("lens obj func is not registered!\r\n");
    }
}

INT32 Lens_ZoomIn(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_ZOOM_INIT) || (Lens_ModuleGetState() & LENS_STATE_ZOOM_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->zoom_press) {
        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, TRUE);
        DBG_MSG("LENS_STATE_ZOOM_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_START, NULL);
        }

        g_LensCtrl.pLens->zoom_press(ZOOM_IN);

        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}

INT32 Lens_ZoomOut(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_ZOOM_INIT) || (Lens_ModuleGetState() & LENS_STATE_ZOOM_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->zoom_press) {
        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, TRUE);
        DBG_MSG("LENS_STATE_ZOOM_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_START, NULL);
        }

        g_LensCtrl.pLens->zoom_press(ZOOM_OUT);

        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}

INT32 Lens_ZoomStop(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_ZOOM_MOVING)) {
        DBG_MSG("%s already stop (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->zoom_release) {
        g_LensCtrl.pLens->zoom_release();

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_END, NULL);
        }
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}

INT32 Lens_ZoomRetract(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_ZOOM_INIT) || (Lens_ModuleGetState() & LENS_STATE_ZOOM_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->zoom_retract && g_LensCtrl.pLens->shutter_setState) {
        Lens_ModuleSetState(LENS_STATE_ZOOM_MOVING, TRUE);
        DBG_MSG("LENS_STATE_ZOOM_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_START, NULL);
        }

        ret_status = g_LensCtrl.pLens->zoom_retract();

        g_LensCtrl.pLens->shutter_setState(MOTOR_SHUTTER_NORMAL, CLOSE);

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_ZOOM_END, NULL);
        }

        Lens_ModuleSetState(LENS_STATE_ZOOM_INIT|LENS_STATE_ZOOM_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}


//----------------------------------------------------------------------------------------------------------------------
// focus
//----------------------------------------------------------------------------------------------------------------------
void Lens_FocusSetFunc(LENS_FOCUS_FUNC focus_func, UINT32 param)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_focus_setFunc) {
            g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_focus_setFunc(focus_func, param);
        }
    }
}

UINT32 Lens_FocusGetSpeed(void)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_getSpeed) {
            return g_LensCtrl.pLens->focus_getSpeed();
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

void Lens_FocusSetSpeed(LENS_SPEED_CATEGORY category)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_setSpeed) {
            g_LensCtrl.pLens->focus_setSpeed(category);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }
}

UINT32 Lens_FocusGetFocalLength(UINT32 zoom_section)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_getFocalLength) {
            return (g_LensCtrl.pLens->focus_getFocalLength(zoom_section));
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return g_lens_focal_length;
}

void Lens_FocusSetFocalLength(UINT32 zoom_section, UINT32 focal_length)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_setFocalLength) {
            g_LensCtrl.pLens->focus_setFocalLength(zoom_section,focal_length);

            return;
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    g_lens_focal_length = focal_length;
}

UINT32 Lens_FocusGetDistTable(UINT32 pos_idx)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_getDistTable) {
            return g_LensCtrl.pLens->focus_getDistTable(pos_idx);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

INT32 Lens_FocusGetDefaultTable(UINT32 section, UINT32 focus_distance)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_getDefaultTable) {
            return g_LensCtrl.pLens->focus_getDefaultTable(section, focus_distance);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

INT32 Lens_FocusGetTable(UINT32 section, UINT32 focus_distance)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_getTable) {
            return g_LensCtrl.pLens->focus_getTable(section, focus_distance);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

INT32 Lens_FocusSetCaliData(INT16 *pcali_tbl, UINT32 tbl_size)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->setCaliData) {
            return g_LensCtrl.pLens->setCaliData(pcali_tbl, tbl_size);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");

            return ERR_FUNC_PTR_NULL;
        }
    } else {
        return ERR_LENS_INIT_FAIL;
    }
}

INT32 Lens_FocusGetRange(FOCUS_RANGE range)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_getRange) {
            return g_LensCtrl.pLens->focus_getRange(range);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

INT32 Lens_FocusGetPosition(void)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_getPostion) {
            return g_LensCtrl.pLens->focus_getPostion();
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

void Lens_FocusSetPosition(INT32 position, INT32 dir)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->focus_setPostion) {
            g_LensCtrl.pLens->focus_setPostion(position, dir);

            Lens_ModuleSetState(LENS_STATE_FOCUS_INIT, TRUE);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }
}

INT32 Lens_FocusDoAction(FOCUS_CATEGORY category, INT32 position)
{
    INT32 move_step = 0;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_FOCUS_INIT) || (Lens_ModuleGetState() & LENS_STATE_FOCUS_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->focus_doExc && g_LensCtrl.pLens->focus_go2Position) {
        Lens_ModuleSetState(LENS_STATE_FOCUS_MOVING, TRUE);
        DBG_MSG("LENS_STATE_FOCUS_MOVING\r\n");

        if (category & FOCUS_PREEXC) {
            if (g_LensCtrl.pLens->focus_doExc) {
                g_LensCtrl.pLens->focus_doExc(ON, 0);
            } else {
                DBG_ERR("lens obj is not registered!\r\n");
            }
        }

        if (category & FOCUS_RUN) {
            if (g_LensCtrl.pLens->focus_go2Position) {
                // callback function
                if (g_LensCtrl.APICB) {
                    g_LensCtrl.APICB(LENS_CB_FOCUS_START, NULL);
                }

                move_step = g_LensCtrl.pLens->focus_go2Position(position, TRUE);

                // callback function
                if (g_LensCtrl.APICB) {
                    g_LensCtrl.APICB(LENS_CB_FOCUS_END, NULL);
                }
            } else {
                DBG_ERR("lens obj is not registered!\r\n");
            }
        }

        if (category & FOCUS_POSTEXC) {
            if (g_LensCtrl.pLens->focus_doExc) {
                g_LensCtrl.pLens->focus_doExc(OFF, 0);
            } else {
                DBG_ERR("lens obj is not registered!\r\n");
            }
        }

        Lens_ModuleSetState(LENS_STATE_FOCUS_MOVING, FALSE);
    } else {
        DBG_ERR("lens obj func is not registered!\r\n");
    }

    return move_step;
}

INT32 Lens_FocusTele(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
        } else if (!(Lens_ModuleGetState() & LENS_STATE_FOCUS_INIT) || (Lens_ModuleGetState() & LENS_STATE_FOCUS_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->focus_press) {
        Lens_ModuleSetState(LENS_STATE_FOCUS_MOVING, TRUE);
        DBG_MSG("LENS_STATE_FOCUS_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_FOCUS_START, NULL);
        }

        g_LensCtrl.pLens->focus_press(FOCUS_TELE);

        Lens_ModuleSetState(LENS_STATE_FOCUS_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}

INT32 Lens_FocusWide(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_FOCUS_INIT) || (Lens_ModuleGetState() & LENS_STATE_FOCUS_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->focus_press) {
        Lens_ModuleSetState(LENS_STATE_FOCUS_MOVING, TRUE);
        DBG_MSG("LENS_STATE_FOCUS_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_FOCUS_START, NULL);
        }

        g_LensCtrl.pLens->focus_press(FOCUS_WIDE);

        Lens_ModuleSetState(LENS_STATE_FOCUS_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}

INT32 Lens_FocusStop(void)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_FOCUS_MOVING)) {
        DBG_MSG("%s already stop (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->focus_release) {
        g_LensCtrl.pLens->focus_release();

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_FOCUS_END, NULL);
        }
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
    }

    return ret_status;
}

INT32 Lens_FocusRetract(FOCUS_RETRACT_POSITION position)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (!(Lens_ModuleGetState() & LENS_STATE_FOCUS_INIT) || (Lens_ModuleGetState() & LENS_STATE_FOCUS_MOVING)) {
        DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->focus_retract) {
        Lens_ModuleSetState(LENS_STATE_FOCUS_MOVING, TRUE);
        DBG_MSG("LENS_STATE_FOCUS_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_FOCUS_START, NULL);
        }

        ret_status = g_LensCtrl.pLens->focus_retract(position);

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_FOCUS_END, NULL);
        }

        Lens_ModuleSetState(LENS_STATE_FOCUS_INIT|LENS_STATE_FOCUS_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
        DBG_ERR("lens obj is not registered!\r\n");
    }

    return ret_status;
}


//----------------------------------------------------------------------------------------------------------------------
// aperture
//----------------------------------------------------------------------------------------------------------------------
void Lens_ApertureSetAct(MOTOR_APERTURE_ACT state, UINT32 param)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_aperture_setState) {
            g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_aperture_setState(state, param);
        }
    }
}

UINT32 Lens_ApertureGetFNo(UINT32 zoom_section,UINT32 iris_pos)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->aperture_getFNo) {
            return g_LensCtrl.pLens->aperture_getFNo(zoom_section, iris_pos);
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return 0;
}

IRIS_POS Lens_ApertureGetPosition(void)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->aperture_getPosition) {
            return g_LensCtrl.pLens->aperture_getPosition();
        } else {
            DBG_ERR("lens obj func is not registered!\r\n");
        }
    }

    return IRIS_POS_BIG;
}

INT32 Lens_ApertureSetPosition(IRIS_POS position)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (Lens_ModuleGetState() & LENS_STATE_APERTURE_MOVING) {
        DBG_WRN("%s, State incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->aperture_go2Position) {
        Lens_ModuleSetState(LENS_STATE_APERTURE_MOVING, TRUE);
        DBG_MSG("LENS_STATE_APERTURE_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_APERTURE_START, NULL);
        }

        g_LensCtrl.pLens->aperture_go2Position(position);

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_APERTURE_END, NULL);
        }

        Lens_ModuleSetState(LENS_STATE_APERTURE_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
        DBG_ERR("lens obj is not registered!\r\n");
    }

    return ret_status;
}


//----------------------------------------------------------------------------------------------------------------------
// shutter
//----------------------------------------------------------------------------------------------------------------------
void Lens_ShutterSetAct(MOTOR_SHUTTER_ACT state, UINT32 param)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_shutter_setState) {
            g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_shutter_setState(state, param);
        }
    }
}

INT32 Lens_ShutterSetState(MOTOR_SHUTTER_ACT state, UINT32 param)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    }

    if ((state == MOTOR_SHUTTER_PSEUDO_OPEN) || (state == MOTOR_SHUTTER_PSEUDO_CLOSE)) {
        g_LensCtrl.pLens->shutter_setState(state, param);
    } else {
        if (Lens_ModuleGetState() & LENS_STATE_SHUTTER_MOVING) {
            DBG_WRN("%s, state incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

            return ERR_OPERATION_FAIL;
        }

        if (g_LensCtrl.pLens->shutter_setState) {
            Lens_ModuleSetState(LENS_STATE_SHUTTER_MOVING, TRUE);
            DBG_MSG("LENS_STATE_SHUTTER_MOVING\r\n");

            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_SHUTTER_START, NULL);
            }

            g_LensCtrl.pLens->shutter_setState(state, param);

            // callback function
            if (g_LensCtrl.APICB) {
                g_LensCtrl.APICB(LENS_CB_SHUTTER_END, NULL);
            }

            Lens_ModuleSetState(LENS_STATE_SHUTTER_MOVING, FALSE);
        } else {
            ret_status = ERR_FUNC_PTR_NULL;
            DBG_ERR("lens obj is not registered!\r\n");
        }
    }

    return ret_status;
}


//----------------------------------------------------------------------------------------------------------------------
// IR cut
//----------------------------------------------------------------------------------------------------------------------
void Lens_IRCUTSetAct(MOTOR_IRCUT_ACT state, UINT32 param)
{
    if (g_bLensSemaInit == TRUE) {
        if (g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_ircut_setState) {
            g_LensCtrl.pLens->pLensCtrlTsk->lenstsk_ircut_setState(state, param);
        }
    }
}

INT32 Lens_IRCUTSetState(IRCUT_POS position, UINT32 param)
{
    INT32 ret_status = ERR_OK;

    if (g_bLensSemaInit == FALSE) {
        return ERR_LENS_INIT_FAIL;
    } else if (Lens_ModuleGetState() & LENS_STATE_IRCUT_MOVING) {
        DBG_WRN("%s, State incorrect, (%d)\r\n", __func__, Lens_ModuleGetState());

        return ERR_OPERATION_FAIL;
    }

    if (g_LensCtrl.pLens->ircut_setState) {
        Lens_ModuleSetState(LENS_STATE_IRCUT_MOVING, TRUE);
        DBG_MSG("LENS_STATE_IRCUT_MOVING\r\n");

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_IRCUT_START, NULL);
        }

        if (position == IRCUT_POS_OPEN) {
            g_LensCtrl.pLens->ircut_setState(MOTOR_IRCUT_OPEN, param);     // param: driving interval (msec)
        } else if (position == IRCUT_POS_CLOSE) {
            g_LensCtrl.pLens->ircut_setState(MOTOR_IRCUT_CLOSE, param);    // param: driving interval (msec)
        }

        // callback function
        if (g_LensCtrl.APICB) {
            g_LensCtrl.APICB(LENS_CB_IRCUT_END, NULL);
        }

        Lens_ModuleSetState(LENS_STATE_IRCUT_MOVING, FALSE);
    } else {
        ret_status = ERR_FUNC_PTR_NULL;
        DBG_ERR("lens obj func is not registered!\r\n");
    }

    return ret_status;
}


