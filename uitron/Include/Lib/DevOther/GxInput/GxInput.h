/**
    GxInput module

    Detect key or touch input. User should implement the API listed in DxInput.h.

    @file       GxInput.h
    @ingroup    mIGxInput

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _GXINPUT_H
#define _GXINPUT_H

#include "GxCommon.h"
#include "GxRect.h"

/**
    @addtogroup mIGxInput
*/
//@{


/**
    Touch callback event ID.
*/
#define TOUCH_CB_GESTURE   0

/**
     @name The key type of callback event
*/
//@{
#define KEY_CB_POWER    0   ///< power key
#define KEY_CB_NORMAL   1   ///< normal key
#define KEY_CB_STATUS   2   ///< status key
//@}

/**
    GxKey Data.

    The type of GxKey data.
*/
typedef enum _GX_KEY_DATA {
	GXKEY_NORMAL_KEY, ///< normal key
	GXKEY_STS_KEY1,   ///< status key 1
	GXKEY_STS_KEY2,   ///< status key 2
	GXKEY_STS_KEY3,   ///< status key 3
	GXKEY_STS_KEY4,   ///< status key 4
	GXKEY_STS_KEY5,   ///< status key 5
	GXKEY_DATA_MAX,
	ENUM_DUMMY4WORD(GX_KEY_DATA)
} GX_KEY_DATA;

/**
    Status key group.

    Only support at most five status keys.
*/
typedef enum _STATUS_KEY_GROUP {
	STATUS_KEY_GROUP1 = 0,  ///< status key 1
	STATUS_KEY_GROUP2,      ///< status key 2
	STATUS_KEY_GROUP3,      ///< status key 3
	STATUS_KEY_GROUP4,      ///< status key 4
	STATUS_KEY_GROUP5,      ///< status key 5
	STATUS_KEY_GROUP_MAX,
	ENUM_DUMMY4WORD(STATUS_KEY_GROUP)
} STATUS_KEY_GROUP;

/**
    Key status for normal key.
*/
typedef enum _KEY_STATUS {
	KEY_RELEASE,        ///< The key is from active to inactive.
	KEY_PRESS,          ///< The key is from inactive to active.
	KEY_CONTINUE,       ///< The key is still being pressed.
	ENUM_DUMMY4WORD(KEY_STATUS)
} KEY_STATUS;

/**
    GxKey initialization.

    Hardware initialization.

    @note User should implement DrvKey_Init() first.
*/
extern void     GxKey_Init(void);

/**
  Detect the normal keys.

  Detect that the normal keys are pressed or released and set flg (Gx_FLG_ID_KEY).
  @note This API should be registered to SxTimer.
*/
extern void     GxKey_DetNormalKey(void);

/**
  Detect the power key.

  Detect that the power key is pressed or not.
  @note This API should be registered to SxTimer.
*/
extern void     GxKey_DetPwrKey(void);

/**
  Detect the status keys.

  Detect the "change" of the key state, such as wheeled-mode key.
  @note This API should be registered to SxTimer.
*/
extern void     GxKey_DetStatusKey(void);

/**
  Set first key invalid.

  The first key state will be ignored after system boot up.

  @param[in] status Key status: #_KEY_STATUS.
  @param[in] uiKey Key value.
*/
extern void     GxKey_SetFirstKeyInvalid(KEY_STATUS status, UINT32 uiKey);

/**
  Force the status key be detected again.

  @param[in] Group Status key group: #_STATUS_KEY_GROUP.
*/
extern void GxKey_ForceStatusKeyDet(STATUS_KEY_GROUP Group);

extern void Key_OnSystem(int cmd);

/**
  Get current key data.

  @param[in] attribute The type of GxKey: #_GX_KEY_DATA.
  @return Current key data.
*/
extern UINT32 GxKey_GetData(GX_KEY_DATA attribute);

/**
  Set continue key debounce time.

  The time is from the key be pressed to the first continue event be post.

  @param[in] DebounceCnt A DebounceCnt is based on the interval of GxKey_DetNormalKey().
*/
extern void   GxKey_SetContDebounce(UINT32 DebounceCnt);

/**
  Set repeat key interval.

  @param[in] RepeatCnt A RepeatCnt is based on the interval of GxKey_DetNormalKey().
*/
extern void   GxKey_SetRepeatInterval(UINT32 RepeatCnt);

/**
  Register callback function for GxKey.

  @param[in] pFuncKeyCB The callback function, refer to GX_CALLBACK_PTR. The parameters of callback function are the followings.
    - @b event The key type of callback event(#KEY_CB_POWER/#KEY_CB_NORMAL/#KEY_CB_STATUS).
    - @b param1 Key status: #_KEY_STATUS.
    - @b param2 Key flag.
*/
extern void   GxKey_RegCB(GX_CALLBACK_PTR pFuncKeyCB);

/**
    Install SxCmd.

    Install Key command to SxCmd.
*/
extern void GxKey_InstallCmd(void);

/**
    The gesture code of touch callback event.
*/
typedef enum _TP_GESTURE_CODE {
	TP_GESTURE_IDLE          = 0,            ///< idle, no touch
	TP_GESTURE_PRESS         = 0x00000001,   ///< touch press
	TP_GESTURE_MOVE          = 0x00000002,   ///< touch move
	TP_GESTURE_HOLD          = 0x00000004,   ///< touch and hold
	TP_GESTURE_RELEASE       = 0x00000008,   ///< touch release
	TP_GESTURE_CLICK         = 0x00000010,   ///< touch click
	TP_GESTURE_DOUBLE_CLICK  = 0x00000020,   ///< touch double click
	TP_GESTURE_SLIDE_LEFT    = 0x00000040,   ///< touch slide left
	TP_GESTURE_SLIDE_RIGHT   = 0x00000080,   ///< touch slide right
	TP_GESTURE_SLIDE_UP      = 0x00000100,   ///< touch slide up
	TP_GESTURE_SLIDE_DOWN    = 0x00000200,   ///< touch slide down
	ENUM_DUMMY4WORD(TP_GESTURE_CODE)
} TP_GESTURE_CODE;


/**
    Parameter for GxTouch control.
*/
typedef enum _GXTOUCH_CTRL {
	GXTCH_DOUBLE_CLICK_INTERVAL,///< double click interval, unit:touch sample count
	GXTCH_MOVE_SENSITIVITY_X,   ///< move sensitivity on horizontal direction, unit:OSD pixel
	GXTCH_MOVE_SENSITIVITY_Y,   ///< move sensitivity on vertical direction, unit:OSD pixel
	GXTCH_CLICK_TH,             ///< click threshold by distant, unit:OSD pixel
	GXTCH_SLIDE_TH_HORIZONTAL,  ///< slide distant on horizontal direction, unit:OSD pixel
	GXTCH_SLIDE_TH_VERTICAL,    ///< slide distant on vertical direction, unit:OSD pixel
	GXTCH_HOLD_DELAY_TIME,      ///< hold event (1st) delay time, unit:touch sample count
	GXTCH_HOLD_REPEAT_RATE,     ///< hold event (2nd and others) repeat rate, unit:touch sample count
	ENUM_DUMMY4WORD(GXTOUCH_CTRL)
} GXTOUCH_CTRL;

/**
    GxTouch data for callback function.
*/
typedef struct _GX_TOUCH_DATA {
	TP_GESTURE_CODE Gesture;    ///< Gesture code: #_TP_GESTURE_CODE.
	IPOINT   Point;             ///< Touch point.
} GX_TOUCH_DATA, *PGX_TOUCH_DATA;

/**
    GxTouch calibration data.
*/
typedef struct _GX_TOUCH_CALI {
	IPOINT   UIPoint[3];        ///< UI points
	IPOINT   TouchPoint[3];     ///< Touch points
} GX_TOUCH_CALI, *PGX_TOUCH_CALI;

/**
    GxTouch initialization.

    Hardware initialization.
*/
extern void     GxTouch_Init(void);

/**
  Detect the touch point.

  @note This API should be registered to SxTimer.
*/
extern void     GxTouch_DetTP(void);

/**
  Check if touch is happened.

  @return
        - @b TRUE: Touch panel is pressed.
        - @b FALSE: Touch panel is not pressed.
*/
extern BOOL   GxTouch_IsPenDown(void);

/**
  Reset calibrated touch data.

  @note After running this API, the touch point will not be calibrated by GxTouch.
*/
extern void   GxTouch_ResetCal(void);

/**
  Set touch parameter.

  @param[in] data Parameter for GxTouch control: #_GXTOUCH_CTRL.
  @param[in] value The value of the specified parameter.
*/
extern void   GxTouch_SetCtrl(GXTOUCH_CTRL data, UINT32 value);

/**
  Get touch parameter.

  @param[in] data Parameter for GxTouch control: #_GXTOUCH_CTRL.
  @return The value of the specified parameter.
*/
extern UINT32 GxTouch_GetCtrl(GXTOUCH_CTRL data);

/**
  Register callback function for GxTouch.

  @param[in] fpTouchCB The callback function of GxTouch. The param1 of callback function is touch data: #_GX_TOUCH_DATA.
*/
extern void   GxTouch_RegCB(GX_CALLBACK_PTR fpTouchCB);

/**
  Initialize calibrated touch data.

  @param[in] pTouchCali  Calibrated data: #_GX_TOUCH_CALI.
  @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER     GxTouch_InitCalibration(PGX_TOUCH_CALI pTouchCali);

/**
    Install SxCmd.

    Install Touch command to SxCmd.
*/
extern void GxTouch_InstallCmd(void);

//@}
#endif //_GXINPUT_H
