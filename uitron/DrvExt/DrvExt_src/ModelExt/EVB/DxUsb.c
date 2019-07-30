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
//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection 
//public func
UINT32 DetUSB0Getcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DetUSB0Setcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
UINT32 DetUSB1Getcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DetUSB1Setcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
//#NT#2018/01/12#Ben Wang -end
UINT32 DetUSBInit(void *pInitParam); // Set Init Parameters
UINT32 DetUSBOpen(void); // Common Constructor
UINT32 DetUSBClose(void); // Common Destructor
UINT32 DetUSBState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DetUSBControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DetUSBCommand(CHAR *pcCmdStr); //General Command Console
//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection 
//dx object
//for USB 3.0 port
DX_OBJECT gDevUSB0 = {
	DXFLAG_SIGN,
	DX_CLASS_USB_EXT|DX_TYPE_USB0,
	DETUSB_VER,
	"DETECT_USB0",
	0, 0, 0, 0,
	DetUSB0Getcaps,
	DetUSB0Setcfgs,
	DetUSBInit,
	DetUSBOpen,
	DetUSBClose,
	DetUSBState,
	DetUSBControl,
	DetUSBCommand,
	0,
};

//for USB 2.0 port
DX_OBJECT gDevUSB1 = {
	DXFLAG_SIGN,
	DX_CLASS_USB_EXT|DX_TYPE_USB1,
	DETUSB_VER,
	"DETECT_USB1",
	0, 0, 0, 0,
	DetUSB1Getcaps,
	DetUSB1Setcfgs,
	DetUSBInit,
	DetUSBOpen,
	DetUSBClose,
	DetUSBState,
	DetUSBControl,
	DetUSBCommand,
	0,
};

static BOOL DxUSB_GetIsUSBPlug(BOOL bIsUSB3)
{
	if(bIsUSB3)
		return usb3dev_StateChange();
	else
		return usb_StateChange();
}

static UINT32 DxUSB_UpdateConnectType(BOOL bIsUSB3)
{
	USB_CHARGER_STS RetUSB;
	UINT32  uiUSBCurrType;
	DBG_MSG("^MbIsUSB3=%d", bIsUSB3);
	if (!DxUSB_GetIsUSBPlug(bIsUSB3)) {
		uiUSBCurrType = USB_CONNECT_NONE;
	} else {
		if(bIsUSB3)
			RetUSB = usb3dev_chkCharger(0);
		else
			RetUSB = usb_chkCharger(0);
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


static UINT32 xDetUSBGetcaps(BOOL bIsUSB3, UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID) {
	case DETUSB_CAPS_BASE:
		DBG_IND("get USB caps\r\n");
		v = DETUSB_BF_DETPLUG | DETUSB_BF_CONNTYPE;
		break;
	case DETUSB_CAPS_PLUG:
		DBG_IND("get USB plug\r\n");
		v = DxUSB_GetIsUSBPlug(bIsUSB3);
		break;
	case DETUSB_CAPS_CONN_TYPE:
		DBG_IND("get USB connection type\r\n");
		v = DxUSB_UpdateConnectType(bIsUSB3);
		break;
	default:
		break;
	}
	return v;
}
static UINT32 xDetUSBSetcfgs(BOOL bIsUSB3, UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	switch (CfgID) {
	case DETUSB_CFG_STANDARD_CHARGER:
		DBG_IND("set standard charger %08x\r\n", Param1);
		if(bIsUSB3){
			usb3dev_setConfig(U3DEV_CONFIG_ID_STANDARD_CHARGER, Param1);
		}else{
			usb_setConfig(USB_CONFIG_ID_STANDARD_CHARGER, Param1);
		}
		break;
	case DETUSB_CFG_CHARGER_EVT_CALLBACK:
		DBG_IND("set charger event callback %08x\r\n", Param1);
		if(bIsUSB3){
			usb3dev_setCallBack(U3DEV_CALLBACK_ID_CHARGING_EVENT, (USB_GENERIC_CB)Param1);
		}else{
			usb_setCallBack(USB_CALLBACK_CHARGING_EVENT, (USB_GENERIC_CB)Param1);
		}
		break;
	default:
		break;
	}
	return DX_OK;
}
UINT32 DetUSB0Getcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	return xDetUSBGetcaps(TRUE, CapID, Param1);
}

UINT32 DetUSB1Getcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	return xDetUSBGetcaps(FALSE, CapID, Param1);
}

UINT32 DetUSB0Setcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	return xDetUSBSetcfgs(TRUE, CfgID, Param1);
}

UINT32 DetUSB1Setcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	return xDetUSBSetcfgs(FALSE, CfgID, Param1);
}
//#NT#2018/01/12#Ben Wang -end
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


