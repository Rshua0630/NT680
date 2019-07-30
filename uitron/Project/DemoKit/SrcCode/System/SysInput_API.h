/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIDef.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef NVT_SYSINPUT_API_H
#define NVT_SYSINPUT_API_H

#include "GxLib.h"

/**
    KEY OBJECT for registering key table.

    The project UI will receive the uiKeyEvent and the fpKeySoundCB \n
    is invoked with uiSoundID every time the key action matches the "uiKeyFlag and status".
*/
typedef struct _KEY_OBJ {
	UINT32      uiKeyFlag;   ///< Key flag, one bit of the flag represents a specified key.
	KEY_STATUS  status;      ///< Key status, refer to KEY_STATUS.
	UINT32      uiKeyEvent;  ///< Key event.
	UINT32      uiParam1;    ///< The parameter of posted uiKeyEvent.
	UINT32      uiSoundID;   ///< The parameter of KeySoundCB.
} KEY_OBJ, *PKEY_OBJ;

/**
    TOUCH OBJECT for registering key table.

    The project UI will receive the uiTouchEvent and the fpTouchSoundCB \n
    is invoked with uiSoundID every time the touch action matches the uiGesture.
*/
typedef struct _TOUCH_OBJ {
	UINT32      uiGesture;      //TP_GESTURE_CODE
	UINT32      uiTouchEvent;
	UINT32      uiSoundID;
} TOUCH_OBJ, *PTOUCH_OBJ;

/**
     @name Prototype of key and touch sound callback.
*/
typedef void (*KEY_SOUND_CB)(UINT32 uiSoundID);


//for key
/**
    The Callback function for GxKey_RegCB().

    This callback funtion will post the events which are registered by SysMan_RegKeyTable().

    @param[in] event the key type of callback event
    @param[in] param1 the key status for normal key
    @param[in] param2 the key value
*/
extern void Key_CB(UINT32 event, UINT32 param1, UINT32 param2); //for System_RegAllModule() only

/**
    Register callback function for key sound.

    Refering to KEY_OBJ, the callback is invoked with uiSoundID every time the key action matches the "uiKeyFlag and status".
    @param[in] fpKeySoundCB callback function to be registered
*/
extern void   SysMan_RegKeySoundCB(KEY_SOUND_CB fpKeySoundCB);

/**
    Register key table for detection.

    @param[in] pKeyTable table of KEY_OBJ
    @param[in] uiCnt the amount of key object
*/
extern void   SysMan_RegKeyTable(PKEY_OBJ pKeyTable, UINT32 uiCnt);

/**
  Set key mask
  Set key mask, there are 3 different modes of key mask now.
  (1) KEY_PRESS     (Pressed)
  (2) KEY_RELEASE   (Released)
  (3) KEY_CONTINUE  (Continue)
  The registed key event could be posted only if the bit of key mask is 1.

  @param[in] uiMode: which mode do you want to set the mask, refer to KEY_STATUS
  @param[in] uiMask: The mask you want to set (FLGKEY_UP, FLGKEY_DOWN...)
*/
extern void SysMan_SetKeyMask(KEY_STATUS uiMode, UINT32 uiMask);

/**
  Get key mask
  Get current key mask setting.

  @param[in] uiMode: refer to KEY_STATUS
*/
extern UINT32 SysMan_GetKeyMask(KEY_STATUS uiMode);

/**
  Set key sound mask
  The registed key sound callback will be invoked only if the bit of key mask is 1.

  @param[in] uiMode: which mode do you want to set the mask, refer to KEY_STATUS
  @param[in] uiMask: The mask you want to set (FLGKEY_UP, FLGKEY_DOWN...)
*/
extern void SysMan_SetKeySoundMask(KEY_STATUS uiMode, UINT32 uiMask);

/**
  Get key sound mask
  Get current key sound mask setting.

  @param[in] uiMode: refer to KEY_STATUS
*/
extern UINT32 SysMan_GetKeySoundMask(KEY_STATUS uiMode);

//for touch
extern void Touch_CB(UINT32 event, UINT32 param1, UINT32 param2);

/**
    Register callback function for touch sound.

    Refering to TOUCH_OBJ, the callback is invoked with uiSoundID every time the key action matches the "uiGesture".
    @param[in] fpTouchSoundCB callback function to be registered
*/
extern void SysMan_RegTouchSoundCB(KEY_SOUND_CB fpTouchSoundCB);

/**
    Register touch table for detection.

    @param[in] pTouchTable table of TOUCH_OBJ
    @param[in] uiCnt the amount of touch object
*/
extern void SysMan_RegTouchTable(PTOUCH_OBJ pTouchTable, UINT32 uiCnt);

/**
  Set touch mask
  The registed touch event could be posted only if the bit of key mask is 1.

  @param[in] uiMask: The mask you want to set (TP_GESTURE_PRESS, TP_GESTURE_CLICK...)
*/
extern void SysMan_SetTouchMask(UINT32 uiMask);

#endif//NVT_SYSINPUT_API_H

