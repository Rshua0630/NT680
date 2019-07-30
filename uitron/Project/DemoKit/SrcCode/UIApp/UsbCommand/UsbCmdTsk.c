#include "SysKer.h"
#include "UsbCmdAPI.h"
#include "UsbCmdInt.h"
#include "PrjCfg.h"
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)

#define MAX_USB_CMD_LEN  128
static BOOL     g_bUsbCmdTskOpened = FALSE;

ER UsbCmdTsk_Open(void)
{
	if (g_bUsbCmdTskOpened) {
		return E_SYS;
	}

	clr_flg(FLG_ID_USBCMD, FLGUSBCMD_ALL);
	g_bUsbCmdTskOpened = TRUE;
	sta_tsk(USBCMDTSK_ID, 0);
	sta_tsk(USBCMDTSK2_ID, 0);
	sta_tsk(USBCMDDOWNLOADTSK_ID, 0);

	return E_OK;
}

ER UsbCmdTsk_Close(void)
{
	FLGPTN  FlgPtn;

	if (!g_bUsbCmdTskOpened) {
		return E_SYS;
	}

	wai_flg(&FlgPtn, FLG_ID_USBCMD, FLGUSBCMD_IDLE, TWF_ORW);
	//use abort to exit task
	UVAC_AbortCdcRead(USB_CMD_PORT);

	wai_flg(&FlgPtn, FLG_ID_USBCMD, FLGUSBCMD_IDLE2, TWF_ORW);
	//use abort to exit task
	UVAC_AbortCdcRead(USB_DATA_PORT);

	wai_flg(&FlgPtn, FLG_ID_USBCMD, FLGUSBCMD_IDLE3, TWF_ORW);
	ter_tsk(USBCMDDOWNLOADTSK_ID);
	g_bUsbCmdTskOpened = FALSE;

	return E_OK;
}

//for receving XML cmd
void UsbCmdTsk(void)
{
	ER  Ret;
	UINT8 UsbCmdStr[MAX_USB_CMD_LEN];
	UINT32 CmdStrLen;
	kent_tsk();
	DBG_FUNC_BEGIN("\r\n");
	while (USBCMDTSK_ID) {
		CmdStrLen = MAX_USB_CMD_LEN;
		memset(UsbCmdStr, 0, CmdStrLen);
		set_flg(FLG_ID_USBCMD, FLGUSBCMD_IDLE);
		Ret = UVAC_ReadCdcData(USB_CMD_PORT, (UINT8 *)UsbCmdStr, &CmdStrLen);
		clr_flg(FLG_ID_USBCMD, FLGUSBCMD_IDLE);

		if (E_OK == Ret) {
			if (MAX_USB_CMD_LEN == CmdStrLen) {
				DBG_WRN("Cmd may overflow!\r\n");
			}
			UsbCmdHandler(UsbCmdStr, CmdStrLen);
		} else if (E_RSATR == Ret) {
			DBG_IND("Aobrt ReadCdc to exit.\r\n");
			break;
		}
	}
	DBG_FUNC_END("\r\n");
	ext_tsk();
}

//for receving raw data ?
void UsbCmdTsk2(void)
{
	ER  Ret;
	UINT32 Temp[512 / 4]; //just get a 4-byte-aligned buffer
	UINT32 DataLen;
	kent_tsk();
	DBG_FUNC_BEGIN("\r\n");
	while (USBCMDTSK2_ID) {
		DataLen = sizeof(Temp);
		set_flg(FLG_ID_USBCMD, FLGUSBCMD_IDLE2);
		Ret = UVAC_ReadCdcData(USB_DATA_PORT, (UINT8 *)Temp, &DataLen);
		clr_flg(FLG_ID_USBCMD, FLGUSBCMD_IDLE2);

		if (E_OK == Ret) {
			UsbRcvDataHandler((UINT32)Temp, DataLen);
		} else if (E_RSATR == Ret) {
			DBG_IND("Aobrt ReadCdc to exit.\r\n");
			break;
		}
	}
	DBG_FUNC_END("\r\n");
	ext_tsk();
}

//for downloading file
void UsbCmdDownloadTsk(void)
{
	FLGPTN      uiFlag = 0;
	kent_tsk();
	DBG_FUNC_BEGIN("\r\n");
	while (USBCMDDOWNLOADTSK_ID) {
		set_flg(FLG_ID_USBCMD, FLGUSBCMD_IDLE3);
		wai_flg(&uiFlag, FLG_ID_USBCMD, FLGUSBCMD_DOWNLOAD, TWF_ORW | TWF_CLR);
		clr_flg(FLG_ID_USBCMD, FLGUSBCMD_IDLE3);
		UsbCmdDownloadHandler();
	}
	DBG_FUNC_END("\r\n");
}
#endif
//#NT#2016/05/31#Ben Wang -end