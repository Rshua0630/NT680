/**
    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.

    @file       DxUSB.c
    @ingroup    mIDxUSB

    @brief      Detect USB connection status.

    @date       2013/04/30
*/

#include "UsbDevDef.h"
#include "usb.h"
#include "usb3dev.h"
#include "DxUSB.h"
#include "DxCommon.h"
#include "DxApi.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxUSB
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if defined(_USB1_DEVICE_)
#define USB_STATE_CHANGE() usb3dev_StateChange()
#define USB_CHK_CHARGER(x) usb3dev_chkCharger(x)
#define USB_SET_STANDARD_CHARGER(x) usb3dev_setConfig(U3DEV_CONFIG_ID_STANDARD_CHARGER, x)
#elif defined(_USB2_DEVICE_)
#define USB_STATE_CHANGE() usb_StateChange()
#define USB_CHK_CHARGER(x) usb_chkCharger(x)
#define USB_SET_STANDARD_CHARGER(x) usb_setConfig(USB_CONFIG_ID_STANDARD_CHARGER, x)
#else
#define USB_STATE_CHANGE() (FALSE)
#define USB_CHK_CHARGER(x) (USB_CHARGER_STS_NONE)
#define USB_SET_STANDARD_CHARGER(x)
#endif


//public func
UINT32 DetUSBGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DetUSBSetcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
UINT32 DetUSBInit(void *pInitParam); // Set Init Parameters
UINT32 DetUSBOpen(void); // Common Constructor
UINT32 DetUSBClose(void); // Common Destructor
UINT32 DetUSBState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DetUSBControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DetUSBCommand(CHAR *pcCmdStr); //General Command Console

//dx object
DX_OBJECT gDevUSB = {
	DXFLAG_SIGN,
	DX_CLASS_USB_EXT,
	DETUSB_VER,
	"DETECT_USB",
	0, 0, 0, 0,
	DetUSBGetcaps,
	DetUSBSetcfgs,
	DetUSBInit,
	DetUSBOpen,
	DetUSBClose,
	DetUSBState,
	DetUSBControl,
	DetUSBCommand,
	0,
};

static BOOL DxUSB_GetIsUSBPlug(void)
{
	return (USB_STATE_CHANGE());
}

static UINT32 DxUSB_UpdateConnectType(void)
{
	USB_CHARGER_STS RetUSB;
	UINT32  uiUSBCurrType;
	if (!DxUSB_GetIsUSBPlug()) {
		uiUSBCurrType = USB_CONNECT_NONE;
	} else {
		RetUSB = USB_CHK_CHARGER(0);
		switch (RetUSB) {
		case USB_CHARGER_STS_NONE:
			DBG_DUMP("CONNECT to PC\r\n");
			uiUSBCurrType = USB_CONNECT_PC;
			break;
		case USB_CHARGER_STS_CHARGING_DOWNSTREAM_PORT:
			DBG_DUMP("CONNECT to Charging PC\r\n");
			uiUSBCurrType = USB_CONNECT_CHARGING_PC;
			break;
		case USB_CHARGER_STS_CHARGER:
			DBG_DUMP("CONNECT to CHARGER\r\n");
			uiUSBCurrType = USB_CONNECT_CHARGER;
			break;
		default:
			DBG_ERR("CONNECTION UNKNOWN!\r\n");
			uiUSBCurrType = USB_CONNECT_UNKNOWN;
			break;
		}
	}
	return uiUSBCurrType;
}



UINT32 DetUSBGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID) {
	case DETUSB_CAPS_BASE:
		DBG_IND("get USB caps\r\n");
		v = DETUSB_BF_DETPLUG | DETUSB_BF_CONNTYPE;
		break;
	case DETUSB_CAPS_PLUG:
		DBG_IND("get USB plug\r\n");
		v = DxUSB_GetIsUSBPlug();
		break;
	case DETUSB_CAPS_CONN_TYPE:
		DBG_IND("get USB connection type\r\n");
		v = DxUSB_UpdateConnectType();
		break;
	default:
		break;
	}
	return v;
}
UINT32 DetUSBSetcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	switch (CfgID) {
	case DETUSB_CFG_STANDARD_CHARGER:
		DBG_IND("set standard charger %08x\r\n", Param1);
		usb_setConfig(USB_CONFIG_ID_STANDARD_CHARGER, Param1);
		break;

	default:
		break;
	}
	return DX_OK;
}
UINT32 DetUSBInit(void *pInitParam) // Set Init Parameters
{
	DBG_FUNC_BEGIN("\r\n");
	return DX_OK;
}

UINT32 DetUSBOpen(void) // Common Constructor
{
	DBG_FUNC_BEGIN("\r\n");
	return DX_OK;
}

UINT32 DetUSBClose(void) // Common Destructor
{
	DBG_FUNC_BEGIN("\r\n");
	return DX_OK;
}

UINT32 DetUSBState(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_FUNC_BEGIN("\r\n");

	if (StateID & DXGET) {
		UINT32 rvalue =  0;
		StateID &= ~DXGET;
		DBG_IND("get %08x\r\n", StateID);
		switch (StateID) {
		default:
			DBG_ERR("state=0x%02X not support!\r\n", StateID);
			break;
		}
		return rvalue;
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		DBG_IND("set %08x\r\n", StateID);
		switch (StateID) {
		default:
			DBG_ERR("state=0x%02X not support!\r\n", StateID);
			break;
		}
	}
	return DX_OK;
}

UINT32 DetUSBControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
	DBG_FUNC_BEGIN("\r\n");
	DBG_IND("ctrl %08x\r\n", CtrlID);

	switch (CtrlID) {
	default:
		DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
		break;
	}
	return DX_OK;
}

UINT32 DetUSBCommand(CHAR *pcCmdStr) //General Command Console
{
	return FALSE;
}




//============================================


