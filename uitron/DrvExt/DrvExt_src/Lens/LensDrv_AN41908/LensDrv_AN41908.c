/**
 * Novatek PANASONIC AN41908 lens driver.
 * 
 * Reference lens driver implementation.
 * 
 * @file LensDrv_AN41908.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include "LensDrv_AN41908.h"


// prototype declaration
static void     lensAN41908_Init(PLENS_DEVICE_OBJ plens_dev);
static void     lensAN41908_SetSignal(MD_SIGNAL signal, UINT32 on);
static UINT32   lensAN41908_GetSignal(MD_SIGNAL signal);
static UINT32   lensAN41908_GetBusyState(void);
static INT32    lensAN41908_ZoomInit(UINT32 stage);
static UINT32   lensAN41908_ZoomGetSpeed(void);
static void     lensAN41908_ZoomSetSpeed(LENS_SPEED_CATEGORY category);
static UINT32   lensAN41908_ZoomGetMinSection(void);
static UINT32   lensAN41908_ZoomGetMaxSection(void);
static UINT32   lensAN41908_ZoomGetSection(void);
static INT32    lensAN41908_ZoomGetSectionPosition(void);
static INT32    lensAN41908_ZoomGetPosition(void);
static void     lensAN41908_ZoomSetPosition(INT32 position, INT32 dir);
static INT32    lensAN41908_ZoomGo2Section(UINT32 section);
static INT32    lensAN41908_ZoomGo2Position(INT32 position);
static INT32    lensAN41908_ZoomPress(UINT32 zoom);
static UINT32   lensAN41908_ZoomRelease(void);
static INT32    lensAN41908_ZoomRetract(void);
static INT32    lensAN41908_FocusInit(void);
static UINT32   lensAN41908_FocusGetSpeed(void);
static void     lensAN41908_FocusSetSpeed(LENS_SPEED_CATEGORY category);
static UINT32   lensAN41908_FocusGetFocalLength(UINT32 zoom_section);
static void     lensAN41908_FocusSetFocalLength(UINT32 zoom_section, UINT32 focal_length);
static UINT32   lensAN41908_FocusGetDistTable(UINT32 pos_idx);
static INT32    lensAN41908_FocusGetDefultTable(UINT32 section, UINT32 focus_distance);
static INT32    lensAN41908_FocusGetTable(UINT32 section, UINT32 focus_distance);
static INT32    lensAN41908_FocusGetRange(FOCUS_RANGE range);
static void     lensAN41908_FocusDoExc(BOOL on, UINT32 dir);
static INT32    lensAN41908_FocusGetPosition(void);
static void     lensAN41908_FocusSetPosition(INT32 position, INT32 dir);
static INT32    lensAN41908_FocusGo2Position(INT32 position, BOOL limit_range);
static INT32    lensAN41908_FocusPress(UINT32 focus);
static UINT32   lensAN41908_FocusRelease(void);
static INT32    lensAN41908_FocusRetract(FOCUS_RETRACT_POSITION position);
static void     lensAN41908_ApertureInit(void);
static UINT32   lensAN41908_ApertureGetFNo(UINT32 zoom_section, UINT32 iris_pos);
static IRIS_POS lensAN41908_ApertureGetPosition(void);
static void     lensAN41908_ApertureGo2Position(IRIS_POS position);
static void     lensAN41908_ShutterSetState(MOTOR_SHUTTER_ACT state, UINT32 param);
static void     lensAN41908_IrcutSetState(MOTOR_IRCUT_ACT state, UINT32 param);
static INT32    lensAN41908_LensRetract(void);
static INT32    lensAN41908_LensReset(void);
static INT32    lensAN41908_LensPowerOff(void);
static UINT32   lensAN41908_LensGetInfo(void);
static void     lensAN41908_MakeAFTable(void);
static INT32    lensAN41908_SetCaliData(INT16 *pcal_tbl, UINT32 tbl_size);
static UINT32   lensAN41908_GetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl);
static UINT32   lensAN41908_SetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl);


static LENSDRV_INFO g_LensDrvInfo =
{
    .LensDrv.init                    = lensAN41908_Init,
    .LensDrv.setSignal               = lensAN41908_SetSignal,
    .LensDrv.getSignal               = lensAN41908_GetSignal,
    .LensDrv.getBusyState            = lensAN41908_GetBusyState,

    .LensDrv.zoom_init               = lensAN41908_ZoomInit,
    .LensDrv.zoom_getSpeed           = lensAN41908_ZoomGetSpeed,
    .LensDrv.zoom_setSpeed           = lensAN41908_ZoomSetSpeed,
    .LensDrv.zoom_getMinSection      = lensAN41908_ZoomGetMinSection,
    .LensDrv.zoom_getMaxSection      = lensAN41908_ZoomGetMaxSection,
    .LensDrv.zoom_getSection         = lensAN41908_ZoomGetSection,
    .LensDrv.zoom_getSectionPosition = lensAN41908_ZoomGetSectionPosition,
    .LensDrv.zoom_getPosition        = lensAN41908_ZoomGetPosition,
    .LensDrv.zoom_setPostion         = lensAN41908_ZoomSetPosition,
    .LensDrv.zoom_go2Section         = lensAN41908_ZoomGo2Section,
    .LensDrv.zoom_go2Position        = lensAN41908_ZoomGo2Position,
    .LensDrv.zoom_press              = lensAN41908_ZoomPress,
    .LensDrv.zoom_release            = lensAN41908_ZoomRelease,
    .LensDrv.zoom_retract            = lensAN41908_ZoomRetract,

    .LensDrv.focus_init              = lensAN41908_FocusInit,
    .LensDrv.focus_getSpeed          = lensAN41908_FocusGetSpeed,
    .LensDrv.focus_setSpeed          = lensAN41908_FocusSetSpeed,
    .LensDrv.focus_getFocalLength    = lensAN41908_FocusGetFocalLength,
    .LensDrv.focus_setFocalLength    = lensAN41908_FocusSetFocalLength,
    .LensDrv.focus_getDistTable      = lensAN41908_FocusGetDistTable,
    .LensDrv.focus_getDefaultTable   = lensAN41908_FocusGetDefultTable,
    .LensDrv.focus_getTable          = lensAN41908_FocusGetTable,
    .LensDrv.focus_getRange          = lensAN41908_FocusGetRange,
    .LensDrv.focus_doExc             = lensAN41908_FocusDoExc,
    .LensDrv.focus_getPostion        = lensAN41908_FocusGetPosition,
    .LensDrv.focus_setPostion        = lensAN41908_FocusSetPosition,
    .LensDrv.focus_go2Position       = lensAN41908_FocusGo2Position,
    .LensDrv.focus_press             = lensAN41908_FocusPress,
    .LensDrv.focus_release           = lensAN41908_FocusRelease,
    .LensDrv.focus_retract           = lensAN41908_FocusRetract,

    .LensDrv.aperture_init           = lensAN41908_ApertureInit,
    .LensDrv.aperture_getFNo         = lensAN41908_ApertureGetFNo,
    .LensDrv.aperture_getPosition    = lensAN41908_ApertureGetPosition,
    .LensDrv.aperture_go2Position    = lensAN41908_ApertureGo2Position,

    .LensDrv.shutter_setState        = lensAN41908_ShutterSetState,

    .LensDrv.ircut_setState          = lensAN41908_IrcutSetState,

    .LensDrv.lensRetract             = lensAN41908_LensRetract,
    .LensDrv.lensReset               = lensAN41908_LensReset,
    .LensDrv.lensPowerOff            = lensAN41908_LensPowerOff,
    .LensDrv.lensGetInfo             = lensAN41908_LensGetInfo,
    .LensDrv.makeAFTable             = lensAN41908_MakeAFTable,
    .LensDrv.setCaliData             = lensAN41908_SetCaliData,
    .LensDrv.getZoomFocusTable       = lensAN41908_GetZoomFocusTable,
    .LensDrv.setZoomFocusTable       = lensAN41908_SetZoomFocusTable,

    .LensDrv.pMotor       = NULL,
    .LensDrv.pLensCtrlTsk = NULL,

    .LensCtrlTsk = {NULL},

    .uiFocalLengthMin = LENS_FOCAL_LENGTH_WIDE,    // unit: um
    .uiFocalLengthMax = LENS_FOCAL_LENGTH_TELE,    // unit: um

    .uiZoomSecPos   = ZOOM_SECTION_WIDE,
    .iFocusCaliOfst = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .eAperturePos   = IRIS_POS_CLOSE,
    .eShutterState  = MOTOR_SHUTTER_NORMAL,
    .eIrCutState    = MOTOR_IRCUT_OPEN,
};


//----------------------------------------------------------------------------------------------------------------------
// general
//----------------------------------------------------------------------------------------------------------------------
PLENS_TAB DrvLens_GetLens(void)
{
    g_LensDrvInfo.LensDrv.pMotor = DrvLens_GetMotor();
    g_LensDrvInfo.LensDrv.pLensCtrlTsk = &g_LensDrvInfo.LensCtrlTsk;

    return &g_LensDrvInfo.LensDrv;
}


static void lensAN41908_Init(PLENS_DEVICE_OBJ plens_dev)
{
    DBG_DUMP("%s %s\r\n", LENS_DRV_NAME, LENS_DRV_ID);
}


static void lensAN41908_SetSignal(MD_SIGNAL signal, UINT32 on)
{
    g_LensDrvInfo.LensDrv.pMotor->setSignal(signal, on);
}


static UINT32 lensAN41908_GetSignal(MD_SIGNAL signal)
{
    return g_LensDrvInfo.LensDrv.pMotor->getSignal(signal);
}


static UINT32 lensAN41908_GetBusyState(void)
{
    return g_LensDrvInfo.LensDrv.pMotor->getBusyState();
}


//----------------------------------------------------------------------------------------------------------------------
// zoom
//----------------------------------------------------------------------------------------------------------------------
static UINT32 AN41908_CalcFocusRange(INT32 *pmin_value, INT32 *pmax_value, BOOL limit_range)
{
    UINT32 i = ZOOM_SECTION_WIDE;
    INT32 tmp, fmin, fmax;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (limit_range == TRUE) {
        fmin = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iFocusMin;
        fmax = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iFocusMax;

        for (i = ZOOM_SECTION_WIDE; i < ZOOM_SECTION_NUM; i++) {
            if (pmotor_info->ZoomFocusTbl[i].iZoomPosition >= pmotor_info->LensMotorConfig[1].iCurrPosition) {
                tmp = (i != ZOOM_SECTION_WIDE) ? (pmotor_info->ZoomFocusTbl[i].iZoomPosition - pmotor_info->ZoomFocusTbl[i-1].iZoomPosition) : 1;

                if (tmp != 0) {
                    fmin = (i != ZOOM_SECTION_WIDE) ? (pmotor_info->ZoomFocusTbl[i-1].iFocusMin
                         + (pmotor_info->ZoomFocusTbl[i].iFocusMin - pmotor_info->ZoomFocusTbl[i-1].iFocusMin) * (pmotor_info->LensMotorConfig[1].iCurrPosition - pmotor_info->ZoomFocusTbl[i-1].iZoomPosition) / tmp) : fmin;
                    fmax = (i != ZOOM_SECTION_WIDE) ? (pmotor_info->ZoomFocusTbl[i-1].iFocusMax
                         + (pmotor_info->ZoomFocusTbl[i].iFocusMax - pmotor_info->ZoomFocusTbl[i-1].iFocusMax) * (pmotor_info->LensMotorConfig[1].iCurrPosition - pmotor_info->ZoomFocusTbl[i-1].iZoomPosition) / tmp) : fmax;
                } else {
                    DBG_ERR("incorrect pmotor_info->ZoomFocusTbl[]!\r\n");
                }
                break;
            }
        }
    } else {
        fmin = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iFocusMin;
        fmax = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_TELE].iFocusMax;
    }

    *pmin_value = fmin;
    *pmax_value = fmax;

    return i;
}


static void AN41908_UpdateFocusPosition(LENSMOTOR_INFO *pmotor_info)
{
    UINT32 cmd_param = 0;
    INT32 set_step, rest_step, dly_value, focus_pos, near_bound, inf_bound;

    AN41908_CalcFocusRange(&near_bound, &inf_bound, TRUE);

    if ((pmotor_info->LensMotorConfig[0].iCurrPosition < near_bound) || (pmotor_info->LensMotorConfig[0].iCurrPosition > inf_bound)) {
        focus_pos = (near_bound + inf_bound) / 2;

        if (focus_pos != pmotor_info->LensMotorConfig[0].iCurrPosition) {
            if (focus_pos > pmotor_info->LensMotorConfig[0].iCurrPosition) {
                // compensate backlash first
                if ((pmotor_info->LensMotorConfig[0].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[0].iMoveDir != MOVE_DIR_FWD)) {
                    rest_step = pmotor_info->LensMotorConfig[0].iBacklashValue;

                    do {
                        set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                                  ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                        cmd_param = (set_step << 16) + 0;
                        rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
                        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                        dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                        g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, cmd_param);
                        LENS_DELAY_MS(dly_value);
                    } while (rest_step > 0);
                }

                rest_step = focus_pos - pmotor_info->LensMotorConfig[0].iCurrPosition;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                    cmd_param = (0 << 16) + set_step;
                    rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                    dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                    pmotor_info->LensMotorConfig[0].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
                    g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, cmd_param);
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);

                g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, 0);
            } else {
                // compensate backlash first
                if ((pmotor_info->LensMotorConfig[0].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[0].iMoveDir != MOVE_DIR_BWD)) {
                    rest_step = pmotor_info->LensMotorConfig[0].iBacklashValue;

                    do {
                        set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                                  ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                        cmd_param = (set_step << 16) + 0;
                        rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
                        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                        dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                        g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, cmd_param);
                        LENS_DELAY_MS(dly_value);
                    } while (rest_step > 0);
                }

                rest_step = pmotor_info->LensMotorConfig[0].iCurrPosition - focus_pos;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                    cmd_param = (0 << 16) + set_step;
                    rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                    dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                    pmotor_info->LensMotorConfig[0].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
                    g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, cmd_param);
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);

                g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, 0);
            }
        }
    }
}


static INT32 lensAN41908_ZoomInit(UINT32 stage)
{
    UINT32 cmd_param = 0, wait_cnt = 0;
    INT32 set_step, rest_step, dly_value;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (g_LensDrvInfo.LensDrv.pMotor->zoom_setState == NULL) {
        DBG_ERR("%s zoom_setState = NULL!\r\n", __func__);

        return ERR_OPERATION_FAIL;
    }

    if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (GOHOME_ZPEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (GOHOME_FPEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (GOHOME_ZPEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (GOHOME_FPEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }

    rest_step = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_TELE].iZoomPosition + INIT_ZOOM_POS
              + pmotor_info->LensMotorConfig[1].iInitPosBias + (TELE_BOUND/10);

    do {
        set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                  ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
        cmd_param = (0 << 16) + set_step;
        rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
        dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
        pmotor_info->LensMotorConfig[1].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
        g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, cmd_param);
        LENS_DELAY_MS(dly_value);
    } while (rest_step > 0);

    g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

#if 1    // increase go home consistency
    // release motor excitation
    pmotor_info->LensMotorConfig[1].iExciteEnable = 0x0000;

    if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_STEP, 0);
    } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_STEP, 0);
    }

    LENS_DELAY_MS(500);

    if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (0x5454 << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (0x5454 << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    }

    // tighten motor and gear
    pmotor_info->LensMotorConfig[1].iExciteEnable = 0x0400;

    rest_step = TELE_BOUND / 20;

    do {
        set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                  ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
        cmd_param = (0 << 16) + set_step;
        rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
    #else
        dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
    #endif
        pmotor_info->LensMotorConfig[1].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
        g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, cmd_param);
        LENS_DELAY_MS(dly_value);
    } while (rest_step > 0);

    g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);

    if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (GOHOME_ZPEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (GOHOME_ZPEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    }
#endif

    // compensate backlash first
    if ((pmotor_info->LensMotorConfig[1].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[1].iMoveDir != MOVE_DIR_FWD)) {
        rest_step = pmotor_info->LensMotorConfig[1].iBacklashValue;

        do {
            set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                      ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
            cmd_param = (set_step << 16) + 0;
            rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
            dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
            dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
            pmotor_info->LensMotorConfig[1].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
            g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, cmd_param);
            LENS_DELAY_MS(dly_value);
        } while (rest_step > 0);
    }

    rest_step = INIT_ZOOM_POS + pmotor_info->LensMotorConfig[1].iInitPosBias;

    do {
        set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                  ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
        cmd_param = (0 << 16) + set_step;
        rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
        dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
        pmotor_info->LensMotorConfig[1].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
        g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, cmd_param);
        LENS_DELAY_MS(dly_value);
    } while (rest_step > 0);

    g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, 0);

    pmotor_info->LensMotorConfig[1].iCurrPosition = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iZoomPosition;

    // wait for focus move finish
    while ((pmotor_info->LensMotorConfig[0].iWorkStatus == MOTOR_STATUS_BUSY) && (wait_cnt++ < 400)) {
        LENS_DELAY_MS(5);
    }

    AN41908_UpdateFocusPosition(pmotor_info);

    pmotor_info->LensMotorConfig[1].bMove2Home = TRUE;

    if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }

    return ERR_OK;
}

static UINT32 lensAN41908_ZoomGetSpeed(void)
{
    return g_LensDrvInfo.LensDrv.pMotor->zoom_getSpeed();
}

static void lensAN41908_ZoomSetSpeed(LENS_SPEED_CATEGORY category)
{
    UINT32 curr_speed = 0, zoom_speed = 20;

    if (g_LensDrvInfo.LensDrv.pMotor->zoom_getSpeed != NULL) {
        curr_speed = g_LensDrvInfo.LensDrv.pMotor->zoom_getSpeed();
    } else {
        DBG_ERR("%s zoom_getSpeed = NULL!\r\n", __func__);

        return;
    }

    if (category == LENS_SPEED_VERY_LOW) {
        zoom_speed = 20;
    } else if (category == LENS_SPEED_LOW) {
        zoom_speed = 40;
    } else if (category == LENS_SPEED_MEDIUM) {
        zoom_speed = 60;
    } else if (category == LENS_SPEED_HIGH) {
        zoom_speed = 80;
    } else if (category == LENS_SPEED_VERY_HIGH) {
        zoom_speed = 100;
    }

    if (curr_speed != zoom_speed) {
        if (g_LensDrvInfo.LensDrv.pMotor->zoom_setState != NULL) {
            g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_SETSPEED, zoom_speed);
        }
    } else {
        DBG_MSG("set current zoom speed is the same!\r\n");
    }
}


static UINT32 lensAN41908_ZoomGetMinSection(void)
{
    return ZOOM_SECTION_WIDE;
}


static UINT32 lensAN41908_ZoomGetMaxSection(void)
{
    return ZOOM_SECTION_TELE;
}


static UINT32 AN41908_ZoomSearchSection(void)
{
    UINT32 sec_pos;
    INT32 sec_zone_pts;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    for (sec_pos = ZOOM_SECTION_WIDE; sec_pos < ZOOM_SECTION_TELE; sec_pos++) {
        sec_zone_pts = (pmotor_info->ZoomFocusTbl[sec_pos].iZoomPosition + pmotor_info->ZoomFocusTbl[sec_pos+1].iZoomPosition) / 2;

        if (pmotor_info->LensMotorConfig[1].iCurrPosition <= sec_zone_pts) {
            break;
        }
    }

    sec_pos = LENS_CLAMP(sec_pos, ZOOM_SECTION_WIDE, ZOOM_SECTION_TELE);

    return sec_pos;
}


static UINT32 lensAN41908_ZoomGetSection(void)
{
    return AN41908_ZoomSearchSection();
}


static INT32 lensAN41908_ZoomGetSectionPosition(void)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    return LENS_MAX(pmotor_info->ZoomFocusTbl[AN41908_ZoomSearchSection()].iZoomPosition, 0);
}


static INT32 lensAN41908_ZoomGetPosition(void)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    return (pmotor_info->LensMotorConfig[1].iMoveDir << 16) | pmotor_info->LensMotorConfig[1].iCurrPosition;
}


static void lensAN41908_ZoomSetPosition(INT32 position, INT32 dir)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    pmotor_info->LensMotorConfig[1].iCurrPosition = position;
    pmotor_info->LensMotorConfig[1].iMoveDir = dir;

    pmotor_info->LensMotorConfig[1].bMove2Home = TRUE;
}


static INT32 lensAN41908_ZoomGo2Section(UINT32 section)
{
    UINT32 cmd_param = 0, wait_cnt = 0;
    INT32 tar_pos, set_step, rest_step, acc_step = 0, dly_value;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    section = LENS_CLAMP(section, ZOOM_SECTION_WIDE, ZOOM_SECTION_TELE);

    tar_pos = pmotor_info->ZoomFocusTbl[section].iZoomPosition;

    if (pmotor_info->LensMotorConfig[1].bMove2Home == FALSE) {
        DBG_ERR("%s not initialized yet!\r\n", __func__);

        return 0;
    }

    if (g_LensDrvInfo.LensDrv.pMotor->zoom_setState == NULL) {
        DBG_ERR("%s zoom_setState = NULL!\r\n", __func__);

        return 0;
    }

    if (tar_pos != pmotor_info->LensMotorConfig[1].iCurrPosition) {
        // set peak pulse width to 130%
        if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
            AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[1].wMaxPulseWidth << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
            AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[0].wMaxPulseWidth << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
        } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
            AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[1].wMaxPulseWidth << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
            AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[0].wMaxPulseWidth << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
        }

        LENS_DELAY_MS(3);    // for power high ready

        if (tar_pos > pmotor_info->LensMotorConfig[1].iCurrPosition) {
            // compensate backlash first
            if ((pmotor_info->LensMotorConfig[1].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[1].iMoveDir != MOVE_DIR_FWD)) {
                rest_step = pmotor_info->LensMotorConfig[1].iBacklashValue;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                    cmd_param = (set_step << 16) + 0;
                    rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                    dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                    g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, cmd_param);
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);
            }

            rest_step = tar_pos - pmotor_info->LensMotorConfig[1].iCurrPosition;

            do {
                set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                          ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                cmd_param = (0 << 16) + set_step;
                rest_step -= set_step;
                acc_step += set_step;
#if FZ_PLS_PIN_EXIST
                dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                if ((rest_step <= 0) || (pmotor_info->LensMotorConfig[1].bSetMove2Stop == TRUE)) {
                    rest_step = 0;
                    pmotor_info->LensMotorConfig[1].bSetFinalStep = TRUE;
                }

                g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, cmd_param);
                LENS_DELAY_MS(dly_value);
            } while (rest_step > 0);

            g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, 0);
        } else if (tar_pos < pmotor_info->LensMotorConfig[1].iCurrPosition) {
            // compensate backlash first
            if ((pmotor_info->LensMotorConfig[1].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[1].iMoveDir != MOVE_DIR_BWD)) {
                rest_step = pmotor_info->LensMotorConfig[1].iBacklashValue;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                    cmd_param = (set_step << 16) + 0;
                    g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, cmd_param);
                    rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                    dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);
            }

            rest_step = pmotor_info->LensMotorConfig[1].iCurrPosition - tar_pos;

            do {
                set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                          ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                cmd_param = (0 << 16) + set_step;
                rest_step -= set_step;
                acc_step += set_step;
#if FZ_PLS_PIN_EXIST
                dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                if ((rest_step <= 0) || (pmotor_info->LensMotorConfig[1].bSetMove2Stop == TRUE)) {
                    rest_step = 0;
                    pmotor_info->LensMotorConfig[1].bSetFinalStep = TRUE;
                }

                g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, cmd_param);
                LENS_DELAY_MS(dly_value);
            } while (rest_step > 0);

            g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);
        }
    } else {
        DBG_MSG("the same with current zoom position!\r\n");
    }

    g_LensDrvInfo.uiZoomSecPos = section;

    // wait for focus move finish
    while ((pmotor_info->LensMotorConfig[0].iWorkStatus == MOTOR_STATUS_BUSY) && (wait_cnt++ < 400)) {
        LENS_DELAY_MS(5);
    }

    AN41908_UpdateFocusPosition(pmotor_info);

    // restore standby peak pulse width
    if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }

    return acc_step;
}


static INT32 lensAN41908_ZoomGo2Position(INT32 position)
{
    UINT32 cmd_param = 0, wait_cnt = 0;
    INT32 set_step, rest_step, acc_step = 0, dly_value;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (pmotor_info->LensMotorConfig[1].bMove2Home == FALSE) {
        DBG_ERR("%s not initialized yet!\r\n", __func__);

        return 0;
    }

    if (g_LensDrvInfo.LensDrv.pMotor->zoom_setState == NULL) {
        DBG_ERR("%s zoom_setState = NULL!\r\n", __func__);

        return 0;
    }

    if ((position >= pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iZoomPosition) && (position <= pmotor_info->ZoomFocusTbl[ZOOM_SECTION_TELE].iZoomPosition)) {
        if (position != pmotor_info->LensMotorConfig[1].iCurrPosition) {
            // set peak pulse width to 130%
            if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
                AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[1].wMaxPulseWidth << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
                AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[0].wMaxPulseWidth << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
            } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
                AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[1].wMaxPulseWidth << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
                AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[0].wMaxPulseWidth << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
            }
        }

        LENS_DELAY_MS(3);    // for power high ready

        pmotor_info->LensMotorConfig[1].bSetMove2Stop = FALSE;

        if (position > pmotor_info->LensMotorConfig[1].iCurrPosition) {
            // compensate backlash first
            if ((pmotor_info->LensMotorConfig[1].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[1].iMoveDir != MOVE_DIR_FWD)) {
                rest_step = pmotor_info->LensMotorConfig[1].iBacklashValue;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                    cmd_param = (set_step << 16) + 0;
                    rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                    dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                    g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, cmd_param);
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);
            }

            rest_step = position - pmotor_info->LensMotorConfig[1].iCurrPosition;

            do {
                set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                          ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                cmd_param = (0 << 16) + set_step;
                rest_step -= set_step;
                acc_step += set_step;
#if FZ_PLS_PIN_EXIST
                dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                if ((rest_step <= 0) || (pmotor_info->LensMotorConfig[1].bSetMove2Stop == TRUE)) {
                    rest_step = 0;
                    pmotor_info->LensMotorConfig[1].bSetFinalStep = TRUE;
                }

                g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, cmd_param);
                LENS_DELAY_MS(dly_value);
            } while (rest_step > 0);

            g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_FWD, 0);
        } else if (position < pmotor_info->LensMotorConfig[1].iCurrPosition) {
            // compensate backlash first
            if ((pmotor_info->LensMotorConfig[1].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[1].iMoveDir != MOVE_DIR_BWD)) {
                rest_step = pmotor_info->LensMotorConfig[1].iBacklashValue;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                    cmd_param = (set_step << 16) + 0;
                    rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                    dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                    g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, cmd_param);
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);
            }

            rest_step = pmotor_info->LensMotorConfig[1].iCurrPosition - position;

            do {
                set_step = (rest_step > pmotor_info->LensMotorConfig[1].iMaxStepSize)
                          ? pmotor_info->LensMotorConfig[1].iMaxStepSize : rest_step;
                cmd_param = (0 << 16) + set_step;
                rest_step -= set_step;
                acc_step += set_step;
#if FZ_PLS_PIN_EXIST
                dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[1].iDlyTime;
#else
                dly_value = pmotor_info->LensMotorConfig[1].iDlyTime * set_step / pmotor_info->LensMotorConfig[1].iMaxStepSize + 1;
#endif
                if ((rest_step <= 0) || (pmotor_info->LensMotorConfig[1].bSetMove2Stop == TRUE)) {
                    rest_step = 0;
                    pmotor_info->LensMotorConfig[1].bSetFinalStep = TRUE;
                }

                g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, cmd_param);
                LENS_DELAY_MS(dly_value);
            } while (rest_step > 0);

            g_LensDrvInfo.LensDrv.pMotor->zoom_setState(MOTOR_ZOOM_BWD, 0);
        } else {
            DBG_MSG("%s step=0!\r\n", __func__);
        }
    } else {
        DBG_MSG("set zoom position is out of range!\r\n");
    }

    g_LensDrvInfo.uiZoomSecPos = AN41908_ZoomSearchSection();

    // wait for focus move finish
    while ((pmotor_info->LensMotorConfig[0].iWorkStatus == MOTOR_STATUS_BUSY) && (wait_cnt++ < 400)) {
        LENS_DELAY_MS(5);
    }

    AN41908_UpdateFocusPosition(pmotor_info);

    // restore standby peak pulse width
    if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[1].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }

    return acc_step;
}


static INT32 lensAN41908_ZoomPress(UINT32 zoom)
{
    INT32 target_pos;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (zoom == 0) {    // ZOOM_OUT = 0
        target_pos = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iZoomPosition;
    } else {    // ZOOM_IN = 1
        target_pos = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_TELE].iZoomPosition;
    }

    lensAN41908_ZoomGo2Position(target_pos);

    return ERR_OK;
}


static UINT32 lensAN41908_ZoomRelease(void)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    pmotor_info->LensMotorConfig[1].bSetMove2Stop = TRUE;

    return 0;
}


static INT32 lensAN41908_ZoomRetract(void)
{
    return ERR_OK;
}


//----------------------------------------------------------------------------------------------------------------------
// focus
//----------------------------------------------------------------------------------------------------------------------
static INT32 lensAN41908_FocusInit(void)
{
    UINT32 cmd_param = 0;
    INT32 set_step, rest_step, dly_value;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (g_LensDrvInfo.LensDrv.pMotor->focus_setState == NULL) {
        DBG_ERR("%s focus_setState = NULL!\r\n", __func__);

        return ERR_OPERATION_FAIL;
    }

    if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (GOHOME_FPEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (GOHOME_FPEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }

    rest_step = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_TELE].iFocusMax + INIT_FOCUS_POS
              + pmotor_info->LensMotorConfig[0].iInitPosBias + (INF_BOUND/10);

    do {
        set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                  ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
        cmd_param = (0 << 16) + set_step;
        rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
#else
        dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
#endif
        pmotor_info->LensMotorConfig[0].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
        g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, cmd_param);
        LENS_DELAY_MS(dly_value);
    } while (rest_step > 0);

    g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, 0);

#if 1    // increase go home consistency
    // release motor excitation
    pmotor_info->LensMotorConfig[0].iExciteEnable = 0x0000;

    if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_STEP, 0);
    } else if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_STEP, 0);
    }

    LENS_DELAY_MS(500);

    if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (0x5454 << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (0x5454 << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }

    // tighten motor and gear
    pmotor_info->LensMotorConfig[0].iExciteEnable = 0x0400;

    rest_step = INF_BOUND / 20;

    do {
        set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                  ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
        cmd_param = (0 << 16) + set_step;
        rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
        dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
        pmotor_info->LensMotorConfig[0].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
        g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, cmd_param);
        LENS_DELAY_MS(dly_value);
    } while (rest_step > 0);

    g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, 0);

    if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (GOHOME_FPEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (GOHOME_FPEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }
#endif

    if ((pmotor_info->LensMotorConfig[0].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[0].iMoveDir != MOVE_DIR_FWD)) {
        rest_step = pmotor_info->LensMotorConfig[0].iBacklashValue;

        do {
            set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                      ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
            cmd_param = (set_step << 16) + 0;
            rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
            dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
#else
            dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
#endif
            pmotor_info->LensMotorConfig[0].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
            g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, cmd_param);
            LENS_DELAY_MS(dly_value);
        } while (rest_step > 0);
    }

    rest_step = INIT_FOCUS_POS + pmotor_info->LensMotorConfig[0].iInitPosBias;

    do {
        set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                  ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
        cmd_param = (0 << 16) + set_step;
        rest_step -= set_step;
#if FZ_PLS_PIN_EXIST
        dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
#else
        dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
#endif
        pmotor_info->LensMotorConfig[0].bSetFinalStep = (rest_step <= 0) ? TRUE : FALSE;
        g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, cmd_param);
        LENS_DELAY_MS(dly_value);
    } while (rest_step > 0);

    g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, 0);

    pmotor_info->LensMotorConfig[0].iCurrPosition = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iFocusMin;
    pmotor_info->LensMotorConfig[0].bMove2Home = TRUE;

    if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL1) {
        AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
    } else if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL3) {
        AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
    }

    return ERR_OK;
}


static UINT32 lensAN41908_FocusGetSpeed(void)
{
    return g_LensDrvInfo.LensDrv.pMotor->focus_getSpeed();
}


static void lensAN41908_FocusSetSpeed(LENS_SPEED_CATEGORY category)
{
    UINT32 curr_speed = 0, focus_speed = 20;

    if (g_LensDrvInfo.LensDrv.pMotor->focus_getSpeed != NULL) {
        curr_speed = g_LensDrvInfo.LensDrv.pMotor->focus_getSpeed();
    } else {
        DBG_ERR("%s focus_getSpeed = NULL!\r\n", __func__);

        return;
    }

    if (category == LENS_SPEED_VERY_LOW) {
        focus_speed = 20;
    } else if (category == LENS_SPEED_LOW) {
        focus_speed = 40;
    } else if (category == LENS_SPEED_MEDIUM) {
        focus_speed = 60;
    } else if (category == LENS_SPEED_HIGH) {
        focus_speed = 80;
    } else if (category == LENS_SPEED_VERY_HIGH) {
        focus_speed = 100;
    }

    if (curr_speed != focus_speed) {
        if (g_LensDrvInfo.LensDrv.pMotor->focus_setState != NULL) {
            g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_SETSPEED, focus_speed);
        }
    } else {
        DBG_MSG("set current focus speed is the same!\r\n");
    }
}


static UINT32 lensAN41908_FocusGetFocalLength(UINT32 zoom_section)
{
    UINT32 focal_length;

    zoom_section = LENS_CLAMP(zoom_section, ZOOM_SECTION_WIDE, ZOOM_SECTION_TELE);

    if (zoom_section > ZOOM_SECTION_WIDE) {
        focal_length = g_LensDrvInfo.uiFocalLengthMin + (zoom_section - ZOOM_SECTION_WIDE)
                    * (g_LensDrvInfo.uiFocalLengthMax - g_LensDrvInfo.uiFocalLengthMin + (ZOOM_SECTION_TELE - ZOOM_SECTION_WIDE) / 2) / (ZOOM_SECTION_TELE - ZOOM_SECTION_WIDE);
    } else {
        focal_length = g_LensDrvInfo.uiFocalLengthMin;
    }

    return focal_length;
}


static void lensAN41908_FocusSetFocalLength(UINT32 zoom_section, UINT32 focal_length)
{
    zoom_section = LENS_CLAMP(zoom_section, ZOOM_SECTION_WIDE, ZOOM_SECTION_TELE);

    if (zoom_section == ZOOM_SECTION_WIDE) {
        g_LensDrvInfo.uiFocalLengthMin = LENS_CLAMP(focal_length, LENS_FOCAL_LENGTH_WIDE, g_LensDrvInfo.uiFocalLengthMax);
    } else if (zoom_section == ZOOM_SECTION_TELE) {
        g_LensDrvInfo.uiFocalLengthMax = LENS_CLAMP(focal_length, g_LensDrvInfo.uiFocalLengthMin, LENS_FOCAL_LENGTH_TELE);
    }
}


static UINT32 lensAN41908_FocusGetDistTable(UINT32 pos_idx)
{
    pos_idx = LENS_MIN(pos_idx, FOCUS_DISTANCE_TABLE_NUM-1);

    return g_uiFocusDistanceTbl[pos_idx];
}


static INT32 lensAN41908_FocusGetDefultTable(UINT32 section, UINT32 focus_distance)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    section = LENS_CLAMP(section, ZOOM_SECTION_WIDE, ZOOM_SECTION_TELE);
    focus_distance = LENS_MIN(focus_distance, FOCUS_DISTANCE_TABLE_NUM-1);

    return pmotor_info->FocusDistanceTbl[section].iLensPosition[focus_distance];
}


static INT32 lensAN41908_FocusGetTable(UINT32 section, UINT32 focus_distance)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    section = LENS_CLAMP(section, ZOOM_SECTION_WIDE, ZOOM_SECTION_TELE);
    focus_distance = LENS_MIN(focus_distance, FOCUS_DISTANCE_TABLE_NUM-1);

    return (pmotor_info->FocusDistanceTbl[section].iLensPosition[focus_distance] + g_LensDrvInfo.iFocusCaliOfst[section]);
}


static INT32 lensAN41908_FocusGetRange(FOCUS_RANGE range)
{
    INT32 low_bound, high_bound, ret_value = 0;

    AN41908_CalcFocusRange(&low_bound, &high_bound, TRUE);

    if (range == FOCUS_RANGE_START) {
        ret_value = low_bound;
    } else if (range == FOCUS_RANGE_END) {
        ret_value = high_bound;
    } else if (range == FOCUS_RANGE_BOTH) {
        ret_value = (high_bound << 16) + low_bound;
    }

    return ret_value;
}


static void lensAN41908_FocusDoExc(BOOL on, UINT32 dir)
{
    if (g_LensDrvInfo.LensDrv.pMotor->focus_setState != NULL) {
        if (on) {
            g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_EXCITE_ON, 0);
        } else {
            g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_EXCITE_OFF, 0);
        }
    } else {
        DBG_MSG("%s focus_setState = NULL!\r\n", __func__);
    }
}


static INT32 lensAN41908_FocusGetPosition(void)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    return (pmotor_info->LensMotorConfig[0].iMoveDir << 16) | pmotor_info->LensMotorConfig[0].iCurrPosition;
}


static void lensAN41908_FocusSetPosition(INT32 position, INT32 dir)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    pmotor_info->LensMotorConfig[0].iCurrPosition = position;
    pmotor_info->LensMotorConfig[0].iMoveDir = dir;

    pmotor_info->LensMotorConfig[0].bMove2Home = TRUE;
}


static INT32 lensAN41908_FocusGo2Position(INT32 position, BOOL limit_range)
{
    UINT32 cmd_param = 0;
    INT32 set_step, rest_step, acc_step = 0, dly_value, near_bound, inf_bound;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (pmotor_info->LensMotorConfig[0].bMove2Home == FALSE) {
        DBG_ERR("%s not initialized yet!\r\n", __func__);

        return 0;
    }

    if (g_LensDrvInfo.LensDrv.pMotor->focus_setState == NULL) {
        DBG_ERR("%s focus_setState = NULL!\r\n", __func__);

        return 0;
    }

    AN41908_CalcFocusRange(&near_bound, &inf_bound, limit_range);

    if ((position >= near_bound) && (position <= inf_bound)) {
        if (position != pmotor_info->LensMotorConfig[0].iCurrPosition) {
            // set peak pulse width to 130%
            if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL1) {
                AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[0].wMaxPulseWidth << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
            } else if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL3) {
                AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (pmotor_info->LensMotorConfig[0].wMaxPulseWidth << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
            }
        }

        LENS_DELAY_MS(3);    // for power high ready

        pmotor_info->LensMotorConfig[0].bSetMove2Stop = FALSE;

        if (position > pmotor_info->LensMotorConfig[0].iCurrPosition) {
            // compensate backlash first
            if ((pmotor_info->LensMotorConfig[0].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[0].iMoveDir != MOVE_DIR_FWD)) {
                rest_step = pmotor_info->LensMotorConfig[0].iBacklashValue;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                    cmd_param = (set_step << 16) + 0;
                    rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                    dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                    g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, cmd_param);
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);
            }

            rest_step = position - pmotor_info->LensMotorConfig[0].iCurrPosition;

            do {
                set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                          ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                cmd_param = (0 << 16) + set_step;
                rest_step -= set_step;
                acc_step += set_step;
    #if FZ_PLS_PIN_EXIST
                dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                if ((rest_step <= 0) || (pmotor_info->LensMotorConfig[0].bSetMove2Stop == TRUE)) {
                    rest_step = 0;
                    pmotor_info->LensMotorConfig[0].bSetFinalStep = TRUE;
                }

                g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, cmd_param);
                LENS_DELAY_MS(dly_value);
            } while (rest_step > 0);

            g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_FWD, 0);
        } else if (position < pmotor_info->LensMotorConfig[0].iCurrPosition) {
            // compensate backlash first
            if ((pmotor_info->LensMotorConfig[0].iBacklashValue != 0) && (pmotor_info->LensMotorConfig[0].iMoveDir != MOVE_DIR_BWD)) {
                rest_step = pmotor_info->LensMotorConfig[0].iBacklashValue;

                do {
                    set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                              ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                    cmd_param = (set_step << 16) + 0;
                    rest_step -= set_step;
    #if FZ_PLS_PIN_EXIST
                    dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                    dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                    g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, cmd_param);
                    LENS_DELAY_MS(dly_value);
                } while (rest_step > 0);
            }

            rest_step = pmotor_info->LensMotorConfig[0].iCurrPosition - position;

            do {
                set_step = (rest_step > pmotor_info->LensMotorConfig[0].iMaxStepSize)
                          ? pmotor_info->LensMotorConfig[0].iMaxStepSize : rest_step;
                cmd_param = (0 << 16) + set_step;
                rest_step -= set_step;
                acc_step += set_step;
    #if FZ_PLS_PIN_EXIST
                dly_value = (rest_step <= 0) ? 0 : pmotor_info->LensMotorConfig[0].iDlyTime;
    #else
                dly_value = pmotor_info->LensMotorConfig[0].iDlyTime * set_step / pmotor_info->LensMotorConfig[0].iMaxStepSize + 1;
    #endif
                if ((rest_step <= 0) || (pmotor_info->LensMotorConfig[0].bSetMove2Stop == TRUE)) {
                    rest_step = 0;
                    pmotor_info->LensMotorConfig[0].bSetFinalStep = TRUE;
                }

                g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, cmd_param);
                LENS_DELAY_MS(dly_value);
            } while (rest_step > 0);

            g_LensDrvInfo.LensDrv.pMotor->focus_setState(MOTOR_FOCUS_BWD, 0);
        } else {
            DBG_MSG("%s step=0!\r\n", __func__);
        }

        // restore standby peak pulse width
        if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL1) {
            AN41908_TransmitCmd(AN41908_CHNEL1, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE1_MAX_DUTY_WIDTH);
        } else if (pmotor_info->LensMotorConfig[0].eChnelSel == AN41908_CHNEL3) {
            AN41908_TransmitCmd(AN41908_CHNEL3, AN41908_SET_MOTOR_CFG, (STANDBY_PEAK_PULSE_WIDTH << 8) | AN41908_PULSE2_MAX_DUTY_WIDTH);
        }
    } else {
        DBG_MSG("set focus position is out of range!\r\n");
    }

    return acc_step;
}


static INT32 lensAN41908_FocusPress(UINT32 focus)
{
    INT32 target_pos;
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (focus == 0) {    // FOCUS_WIDE = 0
        target_pos = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_WIDE].iFocusMin;
    } else {    // FOCUS_TELE = 1
        target_pos = pmotor_info->ZoomFocusTbl[ZOOM_SECTION_TELE].iFocusMax;
    }

    lensAN41908_FocusGo2Position(target_pos, FALSE);

    return ERR_OK;
}


static UINT32 lensAN41908_FocusRelease(void)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    pmotor_info->LensMotorConfig[0].bSetMove2Stop = TRUE;

    return 0;
}


static INT32 lensAN41908_FocusRetract(FOCUS_RETRACT_POSITION position)
{
    return ERR_OK;
}


//----------------------------------------------------------------------------------------------------------------------
// aperture
//----------------------------------------------------------------------------------------------------------------------
static void lensAN41908_ApertureInit(void)
{
    g_LensDrvInfo.eAperturePos = IRIS_POS_BIG;

    lensAN41908_ApertureGo2Position(IRIS_POS_BIG);
}


static UINT32 lensAN41908_ApertureGetFNo(UINT32 zoom_section, UINT32 iris_pos)
{
    UINT32 fno_value, focal_length;

    zoom_section = LENS_CLAMP(zoom_section, ZOOM_SECTION_WIDE, ZOOM_SECTION_TELE);

    fno_value = g_uiFNumberMapping[g_LensDrvInfo.eAperturePos];

    if (zoom_section > ZOOM_SECTION_WIDE) {
        focal_length = g_LensDrvInfo.uiFocalLengthMin + (zoom_section - ZOOM_SECTION_WIDE)
                    * (g_LensDrvInfo.uiFocalLengthMax - g_LensDrvInfo.uiFocalLengthMin + (ZOOM_SECTION_TELE - ZOOM_SECTION_WIDE) / 2) / (ZOOM_SECTION_TELE - ZOOM_SECTION_WIDE);
    } else {
        focal_length = g_LensDrvInfo.uiFocalLengthMin;
    }

    fno_value *= (focal_length / g_LensDrvInfo.uiFocalLengthMin);

    return fno_value;
}


static IRIS_POS lensAN41908_ApertureGetPosition(void)
{
    return g_LensDrvInfo.eAperturePos;
}


static void lensAN41908_ApertureGo2Position(IRIS_POS position)
{
    if (position > IRIS_POS_CLOSE) {
        DBG_ERR("%s parameters error: (%d)\r\n", __func__, position);
    } else if (g_LensDrvInfo.eAperturePos != position) {
        if (g_LensDrvInfo.LensDrv.pMotor->aperture_setState != NULL) {
            g_LensDrvInfo.LensDrv.pMotor->aperture_setState(MOTOR_APERTURE_NORMAL, position);
            g_LensDrvInfo.eAperturePos = position;
        } else {
            DBG_MSG("%s aperture_setState = NULL!\r\n", __func__);
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------
// shutter
//----------------------------------------------------------------------------------------------------------------------
static void lensAN41908_ShutterSetState(MOTOR_SHUTTER_ACT state, UINT32 param)
{
    if (state > MOTOR_SHUTTER_CAP) {
        DBG_ERR("%s parameters error: (%d)\r\n", __func__, state);
    } else if (g_LensDrvInfo.eShutterState != state) {
        if (g_LensDrvInfo.LensDrv.pMotor->shutter_setState != NULL) {
            g_LensDrvInfo.LensDrv.pMotor->shutter_setState(state, param);
            g_LensDrvInfo.eShutterState = state;
        } else {
            DBG_MSG("%s shutter_setState = NULL!\r\n", __func__);
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------
// IR cut
//----------------------------------------------------------------------------------------------------------------------
static void lensAN41908_IrcutSetState(MOTOR_IRCUT_ACT state, UINT32 param)
{
    if (state > MOTOR_IRCUT_CLOSE) {
        DBG_ERR("%s parameters error: (%d)\r\n", __func__, state);
    } else {
        if (g_LensDrvInfo.LensDrv.pMotor->ircut_setState != NULL) {
            g_LensDrvInfo.LensDrv.pMotor->ircut_setState(state, param);
            g_LensDrvInfo.eIrCutState = state;
        } else {
            DBG_MSG("%s ircut_setState = NULL!\r\n", __func__);
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------
// miscellaneous
//----------------------------------------------------------------------------------------------------------------------
static INT32 lensAN41908_LensRetract(void)
{
    return ERR_OK;
}


static INT32 lensAN41908_LensReset(void)
{
    return ERR_OK;
}


static INT32 lensAN41908_LensPowerOff(void)
{
    if (g_LensDrvInfo.LensDrv.pMotor->setSignal != NULL) {
        g_LensDrvInfo.LensDrv.pMotor->setSignal(MD_SIGNAL_STANDBY, 0);

        return ERR_OK;
    } else {
        DBG_MSG("%s setSignal = NULL!\r\n", __func__);

        return ERR_OPERATION_FAIL;
    }
}


static UINT32 lensAN41908_LensGetInfo(void)
{
    // return lens capability
    return LENS_SUPPORT_IRCUT | LENS_SUPPORT_ZOOM | LENS_SUPPORT_FOCUS;
}


static void lensAN41908_MakeAFTable(void)
{
    static BOOL ever_entry = FALSE;
    UINT32 i;
    static LENS_ZOOM_FOCUS_TAB zoom_focus_tbl[ZOOM_SECTION_NUM] = {{ .iZoomPosition = 0, .iFocusMin = 0, .iFocusMax = 0 }};
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (ever_entry == FALSE) {
        ever_entry = TRUE;

        for (i = ZOOM_SECTION_WIDE; i <= ZOOM_SECTION_TELE; i++) {
            zoom_focus_tbl[i].iFocusMin = pmotor_info->ZoomFocusTbl[i].iFocusMin;
            zoom_focus_tbl[i].iFocusMax = pmotor_info->ZoomFocusTbl[i].iFocusMax;
        }
    }

    // read lens calibration data from PStore
    for (i = ZOOM_SECTION_WIDE; i <= ZOOM_SECTION_TELE; i++) {
        zoom_focus_tbl[i].iFocusMin += g_LensDrvInfo.iFocusCaliOfst[i];
        zoom_focus_tbl[i].iFocusMax += g_LensDrvInfo.iFocusCaliOfst[i];
    }
}


static INT32 lensAN41908_SetCaliData(INT16 *pcal_tbl, UINT32 tbl_size)
{
    UINT32 i;

    if ((pcal_tbl != NULL) && (tbl_size == ZOOM_SECTION_TELE)) {
        for (i = ZOOM_SECTION_WIDE; i <= ZOOM_SECTION_TELE; i++) {
            g_LensDrvInfo.iFocusCaliOfst[i] = pcal_tbl[i];
            DBG_DUMP("zoom_sec#%d cali_focus_offset=%d\r\n", i, g_LensDrvInfo.iFocusCaliOfst[i]);
        }
    } else {
        DBG_ERR("pcal_tbl = NULL or tbl_size NG!\r\n");

        return ERR_OPERATION_FAIL;
    }

    return ERR_OK;
}


static UINT32 lensAN41908_GetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (tbl_idx == 0xFFFF) {    // all table
        memcpy(zf_tbl, &pmotor_info->ZoomFocusTbl[0].iZoomPosition, ZOOM_SECTION_NUM*sizeof(LENS_ZOOM_FOCUS_TAB));
    } else {    // single table
        tbl_idx = LENS_MIN(tbl_idx, ZOOM_SECTION_NUM-1);

        memcpy(zf_tbl, &pmotor_info->ZoomFocusTbl[tbl_idx].iZoomPosition, sizeof(LENS_ZOOM_FOCUS_TAB));
    }

    return tbl_idx;
}

static UINT32 lensAN41908_SetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl)
{
    LENSMOTOR_INFO *pmotor_info = AN41908_GetMotorInfo();

    if (tbl_idx == 0xFFFF) {    // all table
        memcpy(&pmotor_info->ZoomFocusTbl[0].iZoomPosition, zf_tbl, ZOOM_SECTION_NUM*sizeof(LENS_ZOOM_FOCUS_TAB));
    } else {    // single table
        tbl_idx = LENS_MIN(tbl_idx, ZOOM_SECTION_NUM-1);

        memcpy(&pmotor_info->ZoomFocusTbl[tbl_idx].iZoomPosition, zf_tbl, sizeof(LENS_ZOOM_FOCUS_TAB));
    }

    return tbl_idx;
}


