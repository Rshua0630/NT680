/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitUSB.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for USB mode
                Get memory and set parameters for USB mode

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#include "PrjCfg.h"
#include "SysCfg.h"
#include "GxStrg.h"
#include "AppLib.h"
#include "UIInfo.h"
#include "UIMode.h"

#if _TODO
#include "USBSIDC2.h"
#include "GxDisplay.h"
#include "PlaybackTsk.h"
#include "DPSFuncs.h"
#include "DxType.h"
#include "DxStorage.h"

#define SIDC_WORKING_BUFFER     0x200000// 1024KB about 3740 obj, 1724KB about 10380 obj;

//#NT#2010/10/21#Lily Kao -begin
_ALIGNED(4) const UINT8 gUSIDCManuStrDesc[] = {
	USB_VENDER_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_VENDER_DESC_STRING
};

_ALIGNED(4) const UINT8 gUSIDCSIDCProdStrDesc[] = {
	USB_PRODUCT_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_PRODUCT_DESC_STRING
};
//#NT#2010/10/21#Lily Kao -end

//Serial number string descriptor, the content should be updated according to serial number
_ALIGNED(4) UINT16 gUSIDCSerialStrDesc3[] = {
	0x0320,                             // 20: size of String Descriptor = 32 bytes
	// 03: String Descriptor type
	'9', '6', '4', '3', '2',            // 96611-00000-001 (default)
	'0', '0', '0', '0', '0',
	'0', '0', '1', '0', '0'
};


_ALIGNED(4) UINT8 gUSIDCSIDCString[] = {
	USB_VENDER_SIDC_DESC_STRING_LEN, // Manufacturer (String), Num of Chars //#NT#2010/07/19#Lily Kao
	USB_VENDER_SIDC_DESC_STRING,

	USB_SIDC_DESC_STRING_LEN, // Model (String), Num of Chars  //#NT#2010/07/19#Lily Kao
	USB_SIDC_DESC_STRING,

	0x05, // DeviceVersion (String), Num of Chars
	0x31, 0x00, // 1
	0x2E, 0x00, // .
	0x30, 0x00, // 0
	0x30, 0x00, // 0
	0x00, 0x00, // NULL
	0x00 // SerialNumber (String), Num of Chars};
};

_ALIGNED(4) const UINT8 gUIDpsVendSpecVersion[5] = {
	'1', '.', '0', '0', 0
};

//#NT#2010/11/02#Lily Kao -begin
//maximum length is 254 bytes
_ALIGNED(4) UINT8 gMTPFriendNameStr[USB_MTP_FRIENDNAME_STRING_LEN + 1] = {
	USB_MTP_FRIENDNAME_STRING, 0x00
};
//#NT#2010/11/02#Lily Kao -end


//support max 14 serial number
void USBMakerInit_SerialNumber(void)
{
#if 0
	UINT32 i, srcLen;
	UINT32 destLen = ((gUSIDCSerialStrDesc3[0] & 0xFF) - 2) / 2;

	srcLen = sizeof(gCalData.Sys_Adjust.SerialNumber);
	//debug_msg("#serailNum:0x%x,0x%x\r\n",srcLen,destLen);
	if (gCalData.Sys_Adjust.SerialNumber[0] != 0) {
		for (i = 0; (i < destLen) && (i < srcLen); i++) {
			gUSIDCSerialStrDesc3[i + 1] = gCalData.Sys_Adjust.SerialNumber[i];
		}
		for (; i < destLen; i++) {
			gUSIDCSerialStrDesc3[i + 1] = 0;
		}
	}
#endif
}
void USBMakerInit_USIDC(USB_SIDC_INFO *pUSBSIDCInfo)
{
	pUSBSIDCInfo->pManuStringDesc = (SIDC_STRING_DESC *)gUSIDCManuStrDesc;
	pUSBSIDCInfo->pProdStringDesc = (SIDC_STRING_DESC *)gUSIDCSIDCProdStrDesc;
	USBMakerInit_SerialNumber();
	pUSBSIDCInfo->pSerialStringDesc = (SIDC_STRING_DESC *)gUSIDCSerialStrDesc3;
	pUSBSIDCInfo->VID = USB_VID;
	pUSBSIDCInfo->PID = USB_PID_PRINT;
	pUSBSIDCInfo->pSIDCString = (char *)&gUSIDCSIDCString;
	pUSBSIDCInfo->SIDCStringLen = sizeof(gUSIDCSIDCString);

	PictB_SetVendSpecVer(strlen((char *)gUIDpsVendSpecVersion), (UINT8 *)&gUIDpsVendSpecVersion[0]);
	MTP_Enable(TRUE);
	//#NT#2010/11/02#Lily Kao -begin
	MTP_SetDeviceFriendlyName((char *)gMTPFriendNameStr);  // Max string length is 255.
	//MTP_SetDeviceFriendlyName("Novatek MTP");  // Max string length is 255.
	//#NT#2010/11/02#Lily Kao -end
}

/**
  Initialize application for USB SIDC mode

  Initialize application for USB SIDC mode.

  @param void
  @return void
*/
void AppInit_ModeUSBSIDC(void)
{
	//#Bus-Power or Self-Power is decided by project.Bus-Powered is default.
	USB_SIDC_INFO   SIDCInfo = {0};
	char *pDxName = NULL;
	UINT32          uiPoolAddr, uiSidcBuff;
	DX_HANDLE pStrgDev = GxStrg_GetDevice(0);

	//uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
	DBG_ERR("POOL_ID_DISP_VDO1 is not found!");
	uiPoolAddr = 0;

	Sidc_SetSendObjBufAddrSize((UINT32)uiPoolAddr, MIN_BUFFER_SIZE_FOR_SENDOBJ);

	uiSidcBuff = uiPoolAddr + MIN_BUFFER_SIZE_FOR_SENDOBJ;
	SIDCInfo.uiSidcBufAddr = uiSidcBuff;
	SIDCInfo.uiSidcBufSize = SIDC_WORKING_BUFFER;
	USBMakerInit_USIDC(&SIDCInfo);

	if (Dx_GetInfo(pStrgDev, DX_INFO_NAME, &pDxName) != DX_OK) {
		pDxName = NULL;
	}

	// Check card lock status, only storage card has this status, where NAND flash does not
	if (pDxName == NULL || strcmp(pDxName, "Storage_Nand1") == 0) {
		SIDCInfo.uiStrgCardLock = FALSE;
	} else if (strcmp(pDxName, "Storage_Card1") == 0) {
		SIDCInfo.uiStrgCardLock = GxStrg_GetDeviceCtrl(0, CARD_READONLY);
	}
	//#Bus-Power or Self-Power is decided by project.Bus-Powered is default.
#if (USB_CHARGE_FUNCTION == DISABLE)
	SIDCInfo.isSelfPwred = TRUE;
#endif
	if (Sidc_Open(&SIDCInfo) != E_OK) {
		debug_err(("Error open USB SIDC task\r\n"));
	}
}

//@}

//#NT#2010/09/29#Lily Kao -begin
//#Move the configuration of Picture-Display(PLAY_OBJ) to Project level
void PrintExe_SetPlayInfo(PLAY_OBJ *pPlayObj)
{
	pPlayObj->uiPlayFileFmt = PBFMT_JPG;
}
//#NT#2010/09/29#Lily Kao -end

INT32 PrintExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);

	AppInit_ModeUSBSIDC();
	return NVTEVT_CONSUME;
}
//#NT#2010/09/29#Lily Kao -end

INT32 PrintExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);
	return NVTEVT_CONSUME;
}



////////////////////////////////////////////////////////////

EVENT_ENTRY CustomPrintObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,               PrintExe_OnOpen},
	{NVTEVT_EXE_CLOSE,              PrintExe_OnClose},
	{NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomPrintObj, APP_SETUP)
#endif

