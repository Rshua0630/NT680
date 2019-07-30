/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIAppUsbCharge_Exe.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for USB mode
                Get memory and set parameters for USB mode

    @note       Nothing.

    @date       2010/12/15
*/

/** \addtogroup mIPRJAPCfg */
//@{

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIAppUsbCharge.h"
#include "UIInfo.h"
#include "DeviceCtrl.h"

/**
  Initialize application for USB CHARGE mode

  Initialize application for USB CHARGE mode.

  @param void
  @return void
*/
INT32 USBChargeExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);
	return NVTEVT_CONSUME;
}

INT32 USBChargeExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);
	return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////

EVENT_ENTRY CustomUSBChargeObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,               USBChargeExe_OnOpen},
	{NVTEVT_EXE_CLOSE,              USBChargeExe_OnClose},
	{NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomUSBChargeObj, APP_SETUP)

//#NT#2010/07/29#Lily Kao -end

