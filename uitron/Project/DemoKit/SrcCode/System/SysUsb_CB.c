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
#define __MODULE__          SysUsbExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (USB_MODE == ENABLE)

#include "SysKer.h"
#include "Debug.h"
#include "NvtUser.h"

//#include "NvtSystem.h"
#include "GxSystem.h"
#include "AppControl.h"

/////////////////////////////////////////////////////////////////////////////
// USB

#include "GxUSB.h"

void USB_CB(UINT32 event, UINT32 param1, UINT32 param2);

void USB_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	switch (event) {
	case SYSTEM_CB_CONFIG:
		//1.設定init值
		//2.設定CB值,
		//3.註冊SxJob服務 ---------> System Job
		//4.註冊SxTimer服務 ---------> Detect Job
		//#if (USBINSERT_FUNCTION == ENABLE)
		//SX_TIMER_DET_USB_ID = System_AddSxTimer(UI_DetUSB, 5,"GxUSB");
		//#endif
		break;
//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection 
	case USB_CB_PLUG:
		//usb plug in
		DBG_IND("USB handle %d inserted.\r\n",param2);
		Ux_PostEvent(NVTEVT_USB_INSERT, 2, param1, param2);
		break;

	case USB_CB_UNPLUG:
		//usb unplug
		DBG_IND("USB handle %d removed.\r\n",param2);
		Ux_PostEvent(NVTEVT_USB_REMOVE, 2, param1, param2);
		break;

	case USB_CB_CHARGE:
		//usb charging current
		DBG_IND("USB handle %d  charging current.\r\n",param2);
		Ux_PostEvent(NVTEVT_USB_CHARGE_CURRENT, 2, param1, param2);
		break;
//#NT#2018/01/12#Ben Wang -end		
	}
}
/*
以下3個USB mode

MSDC
    需要固定的 Size 128K Bytes
    有個變數就是 Vendor Commands,
    有開Vendor Command要 142KB >> check define MSDCNVT_REQUIRE_MIN_SIZE

SIDC
    兩塊，total需要的buffer公式如下:
    2930KB(x 1024) + Object Num x 108 Byte

PCC:
    2塊 * M.
    M = ((JPEG bitstream + header + 2 )取32 byte align) + 64
    JPEG bitstream 在HD mode 必定不小於0x80000B
    JPEG bitstream 在其他mode必定不小於0x40000B
*/



#endif