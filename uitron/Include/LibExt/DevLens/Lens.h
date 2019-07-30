/**
 * Public header file for Lens module.
 * 
 * This file is the header file that define the API and data type for Lens module.
 * 
 * @file Lens.h
 * @note Nothing (or anything need to be mentioned).
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
*/

#ifndef _LENS_H_
#define _LENS_H_

#include "Type.h"
#include "DxLens.h"


typedef enum {
    LENS_AF_STOP = 0,               ///< reserved
    LENS_AF_MOVE,                   ///< reserved
    ENUM_DUMMY4WORD(LENS_AF_STATUS)
} LENS_AF_STATUS;

typedef enum {
    LENS_STATE_IDLE            = 0x00,    ///< lens is in IDLE state
    LENS_STATE_ZOOM_INIT       = 0x01,    ///< lens is in ZOOM INIT state
    LENS_STATE_FOCUS_INIT      = 0x02,    ///< lens is in FOCUS INIT state
    LENS_STATE_ZOOM_MOVING     = 0x04,    ///< lens is in ZOOM moving state
    LENS_STATE_FOCUS_MOVING    = 0x08,    ///< lens is in FOCUS moving state
    LENS_STATE_APERTURE_MOVING = 0x10,    ///< lens is in APERTURE moving state
    LENS_STATE_SHUTTER_MOVING  = 0x20,    ///< lens is in SHUTTER moving state
    LENS_STATE_IRCUT_MOVING    = 0x40,    ///< lens is in IRCUT moving state
    LENS_STATE_RESET           = 0x80,    ///< lens is in RESET state
    LENS_STATE_POWER_OFF       = 0x100,   ///< lens is in POWER_OFF state
    LENS_STATE_ALL             = 0x1FF,
    ENUM_DUMMY4WORD(LENS_STATE)
} LENS_STATE;

typedef enum {
    LENS_CB_INITZOOM_START = 0,     ///< callback to notify INITZOOM_START
    LENS_CB_INITZOOM_END,           ///< callback to notify INITZOOM_END
    LENS_CB_INITFOCUS_START,        ///< callback to notify INITFOCUS_START
    LENS_CB_INITFOCUS_END,          ///< callback to notify INITFOCUS_END
    LENS_CB_INITAPERTURE_START,     ///< callback to notify INITAPERTURE_START
    LENS_CB_INITAPERTURE_END,       ///< callback to notify INITAPERTURE_END
    LENS_CB_ZOOM_START,             ///< callback to notify ZOOM start moving
    LENS_CB_ZOOM_END,               ///< callback to notify ZOOM stop
    LENS_CB_FOCUS_START,            ///< callback to notify FOCUS start moving
    LENS_CB_FOCUS_END,              ///< callback to notify FOCUS stop
    LENS_CB_APERTURE_START,         ///< callback to notify APERTURE start moving
    LENS_CB_APERTURE_END,           ///< callback to notify APERTURE stop
    LENS_CB_SHUTTER_START,          ///< callback to notify SHUTTER start moving
    LENS_CB_SHUTTER_END,            ///< callback to notify SHUTTER stop
    LENS_CB_IRCUT_START,            ///< callback to notify IRCUT start moving
    LENS_CB_IRCUT_END,              ///< callback to notify IRCUT stop
    LENS_CB_RESET_START,            ///< callback to notify LENS_RESET start
    LENS_CB_RESET_END,              ///< callback to notify LENS_RESET end

    // task usage
    LENS_CB_CHGOZOOMSTEP,           ///< callback to notify ZOOM section was changed
    LENS_CB_SHUTTER_PSEUDO_OPEN,    ///< reserved
    LENS_CB_SHUTTER_PSEUDO_CLOSE,   ///< reserved
    ENUM_DUMMY4WORD(LENS_CB_MESSAGE)
} LENS_CB_MESSAGE;

typedef enum {
    LENS_DUMMYLOAD_START,           ///< reserved
    LENS_DUMMYLOAD_END,             ///< reserved
    ENUM_DUMMY4WORD(LENS_DUMMYLOAD)
} LENS_DUMMYLOAD;

typedef enum {
    LENS_INIT_ZOOM,                 ///< init ZOOM
    LENS_INIT_FOCUS,                ///< init FOCUS
    LENS_INIT_APERTURE,             ///< init APERTURE
    ENUM_DUMMY4WORD(LENS_INIT_STATE)
} LENS_INIT_STATE;

typedef enum {
    ZOOM_MIN_SECTION,               ///< minimum section
    ZOOM_MAX_SECTION,               ///< maximum section
    ZOOM_CURR_SECTION,              ///< currect section
    ENUM_DUMMY4WORD(ZOOM_SECTION_CATEGORY)
} ZOOM_SECTION_CATEGORY;

typedef enum {
    ZOOM_CURR_POS,                  ///< current position
    ZOOM_CURR_SECTION_POS,          ///< standard position corresponding to currect located section
    ENUM_DUMMY4WORD(ZOOM_POS_CATEGORY)
} ZOOM_POS_CATEGORY;

// for focus opeartion
typedef enum {
    FOCUS_PREEXC   = 0x01,          ///< focus to pre-excitation
    FOCUS_RUN      = 0x02,          ///< focus move
    FOCUS_POSTEXC  = 0x04,          ///< focus to post-excitation
    FOCUS_EXC_ALL  = 0x07,
    ENUM_DUMMY4WORD(FOCUS_CATEGORY)
} FOCUS_CATEGORY;


/**
     Initialize lens module (mount and initiate lens and motor module).

     @param[in] plens_ctrl    Lens control object contains desired lens and motor objects.
*/
extern void Lens_ModuleInit(PLENSCTRL_OBJ plens_ctrl);

/**
     Get state of lens module.

     @param[in] void
     @return Description of data returned.
         - @b LENS_STATE: state indication of lens module.
*/
extern LENS_STATE Lens_ModuleGetState(void);

/**
     Initiate lens driver.
*/
extern void Lens_DriverInit(PLENSCTRL_OBJ plens_ctrl);

/**
     Initiate lens.

     @param[in] init_item_sel      LENS_INIT_ZOOM, LENS_INIT_FOCUS, or LENS_INIT_APERTURE.
     @return Description of data returned.
         - @b INT32: ERR_OK, ERR_LENS_INIT_FAIL or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_Init(LENS_INIT_STATE init_item_sel);

extern INT32 Lens_Uninit(void);

/**
     Lens retract back.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_Retract(void);

/**
     Reset zoom and focus.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_Reset(void);

/**
     Get lens capability information.

     @param[in] void
     @return Description of data returned.
         - @b UINT32: current lens info.
*/
extern UINT32 Lens_GetCapabilityInfo(void);

/**
     Get status of lens specific signal.

     @param[in] signal                MD_SIGNAL_ZPI, MD_SIGNAL_ZPR, or MD_SIGNAL_FPI.
     @return Description of data returned.
         - @b UINT32: 0: low; 1: high.
*/
extern UINT32 Lens_GetMDSignal(MD_SIGNAL signal);

/**
     Set status of lens specific signal.

     @param[in] signal                MD_SIGNAL_ZPI, MD_SIGNAL_ZPR, or MD_SIGNAL_FPI.
                on                    0: low; 1: high.
     @return void
*/
extern void Lens_SetMDSignal(MD_SIGNAL signal, BOOL on);

/**
     Get status of lens active state.

     @return bit0: zoom / bit1: focus (0: standby; 1: busy).
*/
extern UINT32 Lens_GetBusyState(void);

/**
     Get zoom vs focus range relationship.

     @param[in] tbl_idx         table index (0xFFFF: all)
                zf_tbl          zoom_pos & focus_range
     @return 0.
*/
extern UINT32 Lens_GetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl);

/**
     Set zoom vs focus range relationship.

     @param[in] tbl_idx         table index (0xFFFF: all)
                zf_tbl          zoom_pos & focus_range
     @return 0.
*/
extern UINT32 Lens_SetZoomFocusTable(UINT32 tbl_idx, LENS_ZOOM_FOCUS_TAB *zf_tbl);

/**
     Set zoom functions by lens task.

     @param[in] zoom_func         LENS_ZOOM_INIT ~ LENS_ZOOM_RETRACT.
     param                            
     @return void
*/
extern void Lens_ZoomSetFunc(LENS_ZOOM_FUNC zoom_func, UINT32 param);

/**
     Get zoom speed.

     @param[in] void
     @return Description of data returned.
         - @b UINT32: zoom speed (0 ~ 100).
*/
extern UINT32 Lens_ZoomGetSpeed(void);

/**
     Set zoom speed.

     @param[in] category            LENS_SPEED_VERY_LOW ~ LENS_SPEED_VERY_HIGH
     @return void
*/
extern void Lens_ZoomSetSpeed(LENS_SPEED_CATEGORY category);

/**
     Get zoom section.

     @param[in] category            ZOOM_MIN_SECTION, ZOOM_MAX_SECTION, or ZOOM_CURR_SECTION.
     @return Description of data returned.
         - @b UINT32: section number from wide to tele.
*/
extern UINT32 Lens_ZoomGetSection(ZOOM_SECTION_CATEGORY category);

/**
     Go to specific zoom section.

     @param[in] section              1 (wide) to 11 (tele).
     @return void
*/
extern void Lens_ZoomGo2Section(UINT32 section);

/**
     Get current zoom position.

     @param[in] category            ZOOM_CURR_POS or ZOOM_CURR_SECTION_POS.
     @return Description of data returned.
         - @b INT32: zoom position.
*/
extern INT32 Lens_ZoomGetPosition(ZOOM_POS_CATEGORY category);

/**
     Set current focus position to desired one.

     @param[in] position              Zoom current position.
                     dir              Zoom current direction.
     @return void
*/
extern void Lens_ZoomSetPosition(INT32 position, INT32 dir);

/**
     Move zoom position to specific target.

     @param[in] position             Zoom physical position.
     @return void
*/
extern void Lens_ZoomDoAction(INT32 position);

/**
     Zoom in until Lens_ZoomStop().

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_ZoomIn(void);

/**
     Zoom out until Lens_ZoomStop().

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_ZoomOut(void);

/**
     Stop zoom movement.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_ZoomStop(void);

/**
     Zoom retract.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_ZoomRetract(void);

/**
     Set focus functions by lens task.

     @param[in] focus_func             LENS_FOCUS_INIT ~ LENS_FOCUS_RETRACT.
     param                            
     @return void
*/
extern void Lens_FocusSetFunc(LENS_FOCUS_FUNC focus_func, UINT32 param);

/**
     Get focus speed.

     @param[in] void
     @return Description of data returned.
         - @b UINT32: focus speed (0 ~ 100).
*/
extern UINT32 Lens_FocusGetSpeed(void);

/**
     Set focus speed.

     @param[in] category               LENS_SPEED_VERY_LOW ~ LENS_SPEED_VERY_HIGH
     @return void
*/
extern void Lens_FocusSetSpeed(LENS_SPEED_CATEGORY category);

/**
     Get focal length.

     @param[in] void
     @return Description of data returned.
         - @b UINT32: focal length in 0.01 mm unit.
*/
extern UINT32 Lens_FocusGetFocalLength(UINT32 zoom_section);

/**
     Set focal length.

     @param[in] zoom_section           1 (wide) or 11 (tele).
                focal_length           focal length in 0.01 mm unit.
     @return void
*/
extern void Lens_FocusSetFocalLength(UINT32 zoom_section, UINT32 focal_length);

/**
     Get focus table position depending on position index.

     @param[in] pos_idx                Focus position index (0 ~ 11).
     @return Description of data returned.
         - @b INT32: focus table position.
*/
extern UINT32 Lens_FocusGetDistTable(UINT32 pos_idx);

/**
     Get focus physical position depending on section and position distance.

     @param[in] section                Zoom section.
                focus_distance         Focus position index (0 ~ 11).
     @return Description of data returned.
         - @b INT32: focus physical position.
*/
extern INT32 Lens_FocusGetDefaultTable(UINT32 section, UINT32 focus_distance);

/**
     Get focus physical position with calibrated compensation depending on section and position distance.

     @param[in] section                Zoom section.
                focus_distance         Focus position index (0 ~ 11).
     @return Description of data returned.
         - @b INT32: calibrated focus physical position.
*/
extern INT32 Lens_FocusGetTable(UINT32 section, UINT32 focus_distance);

/**
     Set focus calibrated compensation data to lens driver.

     @param[in] pcali_tbl              Focus calibrated data pointer.
                tbl_size               Focus calibrated data size.
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_FocusSetCaliData(INT16 *pcali_tbl, UINT32 tbl_size);

/**
     Get focus search range depending on current zoom position.

     @param[in] range                  FOCUS_RANGE_START or FOCUS_RANGE_END.
     @return Description of data returned.
         - @b INT32: focus search lower or upper bound.
*/
extern INT32 Lens_FocusGetRange(FOCUS_RANGE range);

/**
     Get current focus position.

     @param[in] void
     @return Description of data returned.
         - @b INT32: current focus position.
*/
extern INT32 Lens_FocusGetPosition(void);

/**
     Set current focus position to desired one.

     @param[in] position              Focus current position.
                     dir              Focus current direction.
     @return void
*/
extern void Lens_FocusSetPosition(INT32 position, INT32 dir);

/**
     Move focus position to specific target.

     @param[in] position               Focus physical position.
     @return Description of data returned.
         - @b INT32: focus real move step.
*/
extern INT32 Lens_FocusDoAction(FOCUS_CATEGORY category, INT32 position);

/**
     Zoom in until Lens_FocusStop().

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_FocusTele(void);

/**
     Zoom out until Lens_FocusStop().

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_FocusWide(void);

/**
     Stop focus movement.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_FocusStop(void);

/**
     Focus retract.

     @param[in] void
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_FocusRetract(FOCUS_RETRACT_POSITION position);

/**
     Set aperture position by lens task.

     @param[in] state                  MOTOR_APERTURE_NORMAL ~ MOTOR_APERTURE_RESET.
     param                            
     @return void
*/
extern void Lens_ApertureSetAct(MOTOR_APERTURE_ACT state, UINT32 param);

/**
     Get F number information.

     @param[in] zoom_section           Zoom section from 1 (wide) to 11 (tele).
     @param[in] iris_pos               Current aperture position.
     @return Description of data returned.
         - @b UINT32: F number value.
*/
extern UINT32 Lens_ApertureGetFNo(UINT32 zoom_section, UINT32 iris_pos);

/**
     Get current aperture position.

     @param[in] void
     @return Description of data returned.
         - @b IRIS_POS: current aperture position.
*/
extern IRIS_POS Lens_ApertureGetPosition(void);

/**
     Set aperture position.

     @param[in] position               IRIS_POS_BIG ~ IRIS_POS_CLOSE.

     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_ApertureSetPosition(IRIS_POS position);

/**
     Open/Close shutter by lens task.

     @param[in] state                  MOTOR_SHUTTER_NORMAL or MOTOR_SHUTTER_CAP.
     param                            
     @return void
*/
extern void Lens_ShutterSetAct(MOTOR_SHUTTER_ACT state, UINT32 param);

/**
     Open/Close shutter.

     @param[in] state                  MOTOR_SHUTTER_NORMAL for normal operation.
                                       MOTOR_SHUTTER_CAP for capture operation.
                param                  SHUTTER_POS_OPEN or SHUTTER_POS_CLOSE.
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_ShutterSetState(MOTOR_SHUTTER_ACT state, UINT32 param);

/**
     Open/Close IR cut filter by lens task.

     @param[in] state                  MOTOR_IRCUT_OPEN or MOTOR_IRCUT_CLOSE.
     param                             Circuit excited time interval.
     @return void
*/
extern void Lens_IRCUTSetAct(MOTOR_IRCUT_ACT state, UINT32 param);

/**
     Open/Close IR cut filter.

     @param[in] state                  IRCUT state with IRCUT_POS_OPEN or IRCUT_POS_CLOSE.
     param                             Circuit excited time interval.
     @return Description of data returned.
         - @b INT32: ERR_OK or ERR_OPERATION_FAIL.
*/
extern INT32 Lens_IRCUTSetState(IRCUT_POS position, UINT32 param);

/**
    Install LensCtrlTsk task, flag and semaphore ID

    @return void
*/
extern void LensCtrl_InstallID(void) _SECTION(".kercfg_text");


#endif    // _LENS_H_


