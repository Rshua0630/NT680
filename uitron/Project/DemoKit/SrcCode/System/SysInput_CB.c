/**
    DevMan, Service command function implementation

    @file       DeviceUsbMan.c
    @ingroup    mDEVMAN

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysInputExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// INPUT

#include "SysKer.h"
#include "Debug.h"
#include "NvtUser.h"

#include "SysInput_API.h"
#include "GxSystem.h"
#include "AppControl.h"

#include "GxInput.h"

#define KEY_MASK_DEFAULT 0xFFFFFFFF

static KEY_SOUND_CB m_fpKeySoundCB            = NULL;
static PKEY_OBJ     m_pKeyTable               = NULL;
static UINT32       m_uiKeyNum                = 0;
static UINT32       m_uiKeyMaskPress          = KEY_MASK_DEFAULT;
static UINT32       m_uiKeyMaskRelease        = KEY_MASK_DEFAULT;
static UINT32       m_uiKeyMaskContinue       = KEY_MASK_DEFAULT;   ////not every UI need continue key
static UINT32       m_uiKeySoundMaskPress     = KEY_MASK_DEFAULT;
static UINT32       m_uiKeySoundMaskRelease   = 0;
static UINT32       m_uiKeySoundMaskContinue  = 0;                  ////not every UI need continue key
static UINT32       m_uiTouchMask             = KEY_MASK_DEFAULT;
static KEY_SOUND_CB m_fpTouchSoundCB          = NULL;
static PTOUCH_OBJ   m_pTouchTable             = NULL;
static UINT32       m_uiTouchNum              = 0;
UINT32              g_LastGesture             = TP_GESTURE_IDLE;    //// For engineer mode only


static PKEY_OBJ xSysMan_FindKey(KEY_STATUS status, UINT32 key)
{
	UINT32  i;
	PKEY_OBJ pKeyObj = m_pKeyTable;
	for (i = 0; i < m_uiKeyNum; i++) {
		if ((pKeyObj->uiKeyFlag & key) && pKeyObj->status == status) {
			return pKeyObj;
		}
		pKeyObj++;
	}
	return NULL;
}

static void xSysMan_PowerKeyCB(KEY_STATUS status, UINT32 key)
{
	PKEY_OBJ pKeyObj;
	DBG_MSG("Power key=0x%x status=%d\r\n", key, status);
	pKeyObj = xSysMan_FindKey(status, key);
	if (pKeyObj) {
		if (m_fpKeySoundCB && (pKeyObj->uiKeyFlag & m_uiKeySoundMaskPress)) {
			m_fpKeySoundCB(pKeyObj->uiSoundID);
		}
		Ux_PostEvent(pKeyObj->uiKeyEvent, 1, pKeyObj->uiParam1);
	}
}
static void xSysMan_NormalKeyCB(KEY_STATUS status, UINT32 key)
{
	PKEY_OBJ pKeyObj = NULL;
	UINT32  uiKeyMask = 0;
	UINT32  uiKeySoundMask = 0;
	DBG_MSG("Normal key=0x%X status=%d\r\n", key, status);
	switch (status) {
	case KEY_RELEASE:
		uiKeyMask = m_uiKeyMaskRelease;
		uiKeySoundMask = m_uiKeySoundMaskRelease;
		break;
	case KEY_CONTINUE:
		uiKeyMask = m_uiKeyMaskContinue & m_uiKeyMaskPress;
		uiKeySoundMask = m_uiKeySoundMaskContinue & m_uiKeySoundMaskPress;
		break;
	case KEY_PRESS:
		uiKeyMask = m_uiKeyMaskPress;
		uiKeySoundMask = m_uiKeySoundMaskPress;
		break;
	default:
		DBG_ERR("[xSysMan_OnNormalKeyChangedCB] PARAMETER ERROR!\r\n");
		break;
	}
	DBG_MSG("uiKeyMask=0x%X uiKeySoundMask=0x%X\r\n", uiKeyMask, uiKeySoundMask);
	key &= uiKeyMask;
	if (key) {
		UINT32 i;
		for (i = 0; i < m_uiKeyNum; i++) {
			pKeyObj = xSysMan_FindKey(status, key);
			if (pKeyObj) {
				if (m_fpKeySoundCB && (pKeyObj->uiKeyFlag & uiKeySoundMask)) {
					m_fpKeySoundCB(pKeyObj->uiSoundID);
				}
				Ux_PostEvent(pKeyObj->uiKeyEvent, 1, pKeyObj->uiParam1);
				key &= ~pKeyObj->uiKeyFlag;
				if (key == 0) {
					break;
				}
			} else {
				break;
			}
		}
	}


}

static void xSysMan_StatusKeyCB(STATUS_KEY_GROUP KeyGroup, UINT32 value)
{
	PKEY_OBJ pKeyObj;
	UINT32  i;
	DBG_MSG("Status_KeyGroup=%d value=0x%X\r\n", KeyGroup, value);

	//#NT#2015/11/17#Marked the dummy code to fix Coverity 45147#KCHong
	//pKeyObj = xSysMan_FindKey(value, KeyGroup);

	pKeyObj = m_pKeyTable;
	for (i = 0; i < m_uiKeyNum; i++) {
		if ((pKeyObj->uiKeyFlag == KeyGroup) && pKeyObj->status == value) {
			Ux_PostEvent(pKeyObj->uiKeyEvent, 1, pKeyObj->uiParam1);
			break;
		}
		pKeyObj++;
	}
}
void Key_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	UINT32 status, key;
	status = param1;
	key = param2;
	switch (event) {
	case SYSTEM_CB_CONFIG:
#if 0
		if (FlowMode_IsPowerOnPlayback()) {
			GxKey_SetFirstKeyInvalid(KEY_PRESS, FLGKEY_PLAYBACK);
		}
#endif
		break;
	case KEY_CB_POWER:
		xSysMan_PowerKeyCB(status, key);
		break;

	case KEY_CB_NORMAL:
		xSysMan_NormalKeyCB(status, key);
		break;

	case KEY_CB_STATUS:
		xSysMan_StatusKeyCB(status, key);
		break;
	}
}
void SysMan_RegKeySoundCB(KEY_SOUND_CB fpKeySoundCB)
{
	m_fpKeySoundCB = fpKeySoundCB;
}
void SysMan_RegKeyTable(PKEY_OBJ pKeyTable, UINT32 uiCnt)
{
	m_pKeyTable = pKeyTable;
	m_uiKeyNum = uiCnt;
}

void SysMan_RegTouchSoundCB(KEY_SOUND_CB fpTouchSoundCB)
{
	m_fpTouchSoundCB = fpTouchSoundCB;
}
void SysMan_RegTouchTable(PTOUCH_OBJ pTouchTable, UINT32 uiCnt)
{
	m_pTouchTable = pTouchTable;
	m_uiTouchNum = uiCnt;
}

void Touch_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
#if defined(_TOUCH_ON_)
	PGX_TOUCH_DATA pTouchData = (PGX_TOUCH_DATA)param1;
	PTOUCH_OBJ pTouchObj = m_pTouchTable;
	UINT32  i, uiGestureCode;
	uiGestureCode = pTouchData->Gesture;
	//DBG_DUMP("Gesture=0x%x x,y=(%d, %d), m_uiTouchMask=0x%x\r\n", pTouchData->Gesture, pTouchData->Point.x, pTouchData->Point.y, m_uiTouchMask);

	uiGestureCode &= m_uiTouchMask;

	if (uiGestureCode) {
		for (i = 0; i < m_uiTouchNum; i++) {
			if (pTouchObj->uiGesture == uiGestureCode) {
				if (m_fpTouchSoundCB) {
					m_fpTouchSoundCB(pTouchObj->uiSoundID);
				}
				//#NT#2016/10/14#KCHong -begin
				//#NT#Forced use 320x240 UI for engineer mode
#if (CALIBRATION_FUNC == ENABLE)
				if (IsEngineerMode()) {
					g_LastGesture = uiGestureCode;
					Ux_PostEvent(pTouchObj->uiTouchEvent, 2, pTouchData->Point.x * 320 / OSD_W, pTouchData->Point.y * 240 / OSD_H);
				} else {
#else
				{
#endif
					Ux_PostEvent(pTouchObj->uiTouchEvent, 2, pTouchData->Point.x, pTouchData->Point.y);
				}
				//#NT#2016/10/14#KCHong -end
				break;
			}
			pTouchObj++;
		}
	}
#endif
}


/**
  Set key mask
  Set key mask, there are 4 different modes of key mask now.
  (1) KEY_PRESS     (Pressed)
  (2) KEY_RELEASE   (Released)
  (3) KEY_CONTINUE  (Continue)
  Key Scan task will only issue the flag if the bit of key mask is 1 and
  the event (pressed, released...) is happened.
  Use KeyScan_GetKeyMask() API to get current key mask before you set the
  new key mask.

  @param UINT32 uiMode: Which mode do you want to set the mask
    KEY_PRESS       (Pressed)
    KEY_RELEASE     (Released)
    KEY_CONTINUE    (Continue)
  @param UINT32 uiMask: The mask you want to set (FLGKEY_UP, FLGKEY_DOWN...)
  @return void
*/
void SysMan_SetKeyMask(KEY_STATUS uiMode, UINT32 uiMask)
{
	switch (uiMode) {
	case KEY_PRESS:
		m_uiKeyMaskPress      = uiMask;
		break;

	case KEY_RELEASE:
		m_uiKeyMaskRelease    = uiMask;
		break;

	case KEY_CONTINUE:
		m_uiKeyMaskContinue   = uiMask;
		break;

	default:
		break;
	}
}

UINT32 SysMan_GetKeyMask(KEY_STATUS uiMode)
{
	UINT32 keyMask = 0;
	switch (uiMode) {
	case KEY_PRESS:
		keyMask = m_uiKeyMaskPress;
		break;

	case KEY_RELEASE:
		keyMask = m_uiKeyMaskRelease;
		break;

	case KEY_CONTINUE:
		keyMask = m_uiKeyMaskContinue;
		break;

	default:
		break;
	}
	return keyMask;
}

/**
  Set key sound mask
  Set sound key mask, there are 4 different modes of key mask now.
  (1) KEY_PRESS     (Pressed)
  (2) KEY_RELEASE   (Released)
  (3) KEY_CONTINUE  (Continue)

  @param UINT32 uiMode: Which mode do you want to set the mask
    KEY_PRESS       (Pressed)
    KEY_RELEASE     (Released)
    KEY_CONTINUE    (Continue)
  @param UINT32 uiMask: The mask you want to set (FLGKEY_UP, FLGKEY_DOWN...)
  @return void
*/
void SysMan_SetKeySoundMask(KEY_STATUS uiMode, UINT32 uiMask)
{
	switch (uiMode) {
	case KEY_PRESS:
		m_uiKeySoundMaskPress      = uiMask;
		break;

	case KEY_RELEASE:
		m_uiKeySoundMaskRelease    = uiMask;
		break;

	case KEY_CONTINUE:
		m_uiKeySoundMaskContinue   = uiMask;
		break;

	default:
		break;
	}
}
UINT32 SysMan_GetKeySoundMask(KEY_STATUS uiMode)
{
	UINT32 keyMask = 0;
	switch (uiMode) {
	case KEY_PRESS:
		keyMask = m_uiKeySoundMaskPress;
		break;

	case KEY_RELEASE:
		keyMask = m_uiKeySoundMaskRelease;
		break;

	case KEY_CONTINUE:
		keyMask = m_uiKeySoundMaskContinue;
		break;

	default:
		DBG_ERR("uiMode(%d)\r\n", uiMode);
		break;
	}
	return keyMask;
}


void SysMan_SetTouchMask(UINT32 uiMask)
{
	m_uiTouchMask = uiMask;
}


