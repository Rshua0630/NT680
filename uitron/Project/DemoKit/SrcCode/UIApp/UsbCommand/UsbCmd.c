#include "PrjCfg.h"
#include "SysKer.h"
#include "NVTToolCommand.h"
#include "UIInfo.h"
#include "UsbCmdAPI.h"
#include "UsbCmdInt.h"
#include "WifiCmdParser.h"
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
#include "SysMain.h"
#include "AppControl.h"
#include "UIAppMovie.h"
#include "UsbCmdID.h"
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)

#define USB_CMD_ROOT        '/'
#define USB_CMD_CUSTOM_TAG  '?'

#define XML_PATH_LEN                 (128)

static BOOL     g_bUsbCmdOpened = FALSE;
static MEM_RANGE g_SendBuf = {0};
static MEM_RANGE g_SendBuf2 = {0};

static char UploadFile[XML_PATH_LEN] = {0};
static char DownloadFile[XML_PATH_LEN] = {0};



extern WIFI_CMD_ENTRY Cmd_uvc[];

static int XML_SetUploadPath(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char   *pch = 0;

	pch = strrchr(argument, '&');
	if (pch) {
		if (strncmp(pch, PARS_STR, strlen(PARS_STR)) == 0) {
			memset((void *)UploadFile, 0, sizeof(UploadFile));
			sscanf_s(pch + strlen(PARS_STR), "%s", &UploadFile, sizeof(UploadFile));
		}
		DBG_IND("Upload File path=%s\r\n", UploadFile);
		if (strlen(UploadFile) >= XML_PATH_LEN - 1) {
			DBG_WRN("path might overflow!\r\n");
		}
		XML_DefaultFormat(USBCMD_UPLOAD_FILE_PATH, WIFIAPP_RET_OK, bufAddr, bufSize, mimeType);
	} else {
		XML_DefaultFormat(USBCMD_UPLOAD_FILE_PATH, WIFIAPP_RET_PAR_ERR, bufAddr, bufSize, mimeType);
	}
	return 0;
}
static int XML_SetDownloadPath(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char   *pch = 0;


	pch = strrchr(argument, '&');
	if (pch) {
		FST_FILE fhdl;
		if (strncmp(pch, PARS_STR, strlen(PARS_STR)) == 0) {
			memset((void *)DownloadFile, 0, sizeof(DownloadFile));
			sscanf_s(pch + strlen(PARS_STR), "%s", &DownloadFile, sizeof(DownloadFile));
		}
		DBG_IND("Download File path=%s\r\n", DownloadFile);
		fhdl = FileSys_OpenFile(DownloadFile, FST_OPEN_READ | FST_OPEN_EXISTING);
		if (fhdl) {
			FileSys_CloseFile(fhdl);
			XML_DefaultFormat(USBCMD_DOWNLOAD_FILE_PATH, WIFIAPP_RET_OK, bufAddr, bufSize, mimeType);
			set_flg(FLG_ID_USBCMD, FLGUSBCMD_DOWNLOAD);
		} else {
			XML_DefaultFormat(USBCMD_DOWNLOAD_FILE_PATH, WIFIAPP_RET_NOFILE, bufAddr, bufSize, mimeType);
			//reset download file path
			DownloadFile[0] = 0;
		}

	} else {
		XML_DefaultFormat(USBCMD_DOWNLOAD_FILE_PATH, WIFIAPP_RET_PAR_ERR, bufAddr, bufSize, mimeType);
	}
	return 0;
}



static void UsbCmd_Init(void)
{
	WifiAppCmd_init();
	WifiCmd_ReceiveCmd(TRUE);
	WifiCmd_SetExecTable(Cmd_uvc);
}

ER UsbCmd_Open(PMEM_RANGE pWorkBuf)
{
	if (MIN_USB_CMD_BUF_SIZE < pWorkBuf->Size) {
		DBG_ERR("Buffer size is too small!\r\n");
		return E_SYS;
	}
	if (pWorkBuf->Addr & 0x3) {
		DBG_ERR("Buffer sholud word-aligned!\r\n");
		return E_SYS;
	}
	if (g_bUsbCmdOpened) {
		return E_SYS;
	}
	//to do, check alignment
	g_SendBuf.Addr = pWorkBuf->Addr;
	g_SendBuf.Size = pWorkBuf->Size / 2;
	g_SendBuf2.Addr = g_SendBuf.Addr + g_SendBuf.Size;
	g_SendBuf2.Size = pWorkBuf->Size - g_SendBuf.Size;

	g_bUsbCmdOpened = TRUE;
	UsbCmdTsk_Open();
	UsbCmd_Init();
	return E_OK;
}
ER UsbCmd_Close(void)
{
	if (!g_bUsbCmdOpened) {
		return E_SYS;
	}
	UsbCmdTsk_Close();
	g_bUsbCmdOpened = FALSE;
	return E_OK;
}

int UsbCmd_Send(char *addr, int *size)
{
	int ret;
	USBCMD_PAYLOAD_HEADER PayloadHeader;
	UINT32 HeaderSize;
	DBG_FUNC_BEGIN("\r\n");
	HeaderSize = sizeof(USBCMD_PAYLOAD_HEADER);
	PayloadHeader.TotalLength = HeaderSize + *size;
	PayloadHeader.Type = USBCMD_PAYLOAD_NOTIFICATION;
	wai_sem(SEMID_USBCMD_1ST_COM);
	ret = UVAC_WriteCdcData(USB_CMD_PORT, (UINT8 *)&PayloadHeader, (UINT32 *)&HeaderSize);
	if (ret == E_OK) {
		ret = UVAC_WriteCdcData(USB_CMD_PORT, (UINT8 *)addr, (UINT32 *)size);
	}
	sig_sem(SEMID_USBCMD_1ST_COM);
	DBG_FUNC_END("\r\n");
	return ret;
}

int UsbCmd_SendRAW(char *addr, int *size)
{
	int ret;
	USBCMD_PAYLOAD_HEADER PayloadHeader;
	UINT32 HeaderSize;
	DBG_FUNC_BEGIN("\r\n");
	HeaderSize = sizeof(USBCMD_PAYLOAD_HEADER);
	PayloadHeader.TotalLength = HeaderSize + *size;
	PayloadHeader.Type = USBCMD_PAYLOAD_RAW_DATA;
	ret = UVAC_WriteCdcData(USB_DATA_PORT, (UINT8 *)&PayloadHeader, (UINT32 *)&HeaderSize);
	if (ret == E_OK) {
		ret = UVAC_WriteCdcData(USB_DATA_PORT, (UINT8 *)addr, (UINT32 *)size);
	}
	DBG_FUNC_END("\r\n");
	return ret;
}

void UsbCmdHandler(UINT8 *pUsbCmdStr, UINT32 CmdStrLen)
{
	char   *pch;
	UINT32 uiSendBufSize;
	char mimeType[64];//just a dummy buffer for using WifiCmd_GetData
	INT32 WifiCmdRet;
	UINT32 segmentCount = 0;
	UINT32 cmd = 0;
	DBG_FUNC_BEGIN("\r\n");
	DBG_IND("pUsbCmdStr=%s\r\n", pUsbCmdStr);
	//if(strncmp(pch,USB_CMD_ROOT,strlen(USB_CMD_ROOT)))
	if (*pUsbCmdStr != USB_CMD_ROOT) {
		DBG_WRN("Not a valid cmd (%s)\r\n", pUsbCmdStr);
		return;
	}
	pch = strchr((char *)pUsbCmdStr, USB_CMD_CUSTOM_TAG);
	if (pch) {
		if (strncmp(pch + 1, CMD_STR, strlen(CMD_STR)) == 0) {
			sscanf_s(pch + 1 + strlen(CMD_STR), "%d", &cmd);
			DBGD(cmd);
			//to do
			//specail handle
		}
		//the argument "path" of WifiCmd_GetData is the string prior to '?'
		*pch = 0;
		wai_sem(SEMID_USBCMD_1ST_COM);
USB_GET_CMD_DATA:
		uiSendBufSize = g_SendBuf.Size;
		WifiCmdRet = WifiCmd_GetData((char *)pUsbCmdStr, pch + 1, g_SendBuf.Addr, &uiSendBufSize, mimeType, segmentCount);
		DBG_IND("WifiCmdRet=%d, len=%d(0x%X)\r\n", WifiCmdRet, uiSendBufSize, uiSendBufSize);
		if (WIFI_CMD_GETDATA_RETURN_OK == WifiCmdRet) {
			USBCMD_PAYLOAD_HEADER PayloadHeader;
			UINT32 HeaderSize;
			HeaderSize = sizeof(USBCMD_PAYLOAD_HEADER);
			PayloadHeader.Type = USBCMD_PAYLOAD_RETURN;
			if (0 == segmentCount) {
				PayloadHeader.TotalLength = HeaderSize + uiSendBufSize;
				UVAC_WriteCdcData(USB_CMD_PORT, (UINT8 *)&PayloadHeader, (UINT32 *)&HeaderSize);
			}
			//DBG_IND("%s\r\n",g_SendBuf.Addr);
			UVAC_WriteCdcData(USB_CMD_PORT, (UINT8 *)g_SendBuf.Addr, &uiSendBufSize);

			if (segmentCount) { //for unknown data length end
				PayloadHeader.TotalLength = UNKNOWN_PAYLOAD_LENGTH_END;
				UVAC_WriteCdcData(USB_CMD_PORT, (UINT8 *)&PayloadHeader, (UINT32 *)&HeaderSize);
			}

			if (WIFIAPP_CMD_MOVIE_REC_SIZE == cmd) {
				DBG_IND("FL_MOVIE_SIZE_MENU=%d, FL_MOVIE_SIZE=%d\r\n", UI_GetData(FL_MOVIE_SIZE_MENU), UI_GetData(FL_MOVIE_SIZE));
			}
			if (FlowMovie_CheckReOpenItem()) {
				DBG_IND("reopen\r\n");
				Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));

			}
		} else if (WIFI_CMD_GETDATA_RETURN_CONTINUE == WifiCmdRet) {
			//DBG_IND("%s\r\n",g_SendBuf.Addr);
			if (0 == segmentCount) { //for unknown data length begin
				USBCMD_PAYLOAD_HEADER PayloadHeader;
				UINT32 HeaderSize;
				HeaderSize = sizeof(USBCMD_PAYLOAD_HEADER);
				PayloadHeader.TotalLength = UNKNOWN_PAYLOAD_LENGTH_BEGIN;
				PayloadHeader.Type = USBCMD_PAYLOAD_RETURN;
				UVAC_WriteCdcData(USB_CMD_PORT, (UINT8 *)&PayloadHeader, (UINT32 *)&HeaderSize);
			}
			UVAC_WriteCdcData(USB_CMD_PORT, (UINT8 *)g_SendBuf.Addr, &uiSendBufSize);
			segmentCount++;
			goto USB_GET_CMD_DATA;
		} else { //WIFI_CMD_GETDATA_RETURN_ERROR
			//just do nothing or return customed tag to inform the host that data is aborted ?
		}
		sig_sem(SEMID_USBCMD_1ST_COM);
	} else { //no '?'
		//just ignore!?
	}
	DBG_FUNC_END("\r\n");
}


//handle received data
void UsbRcvDataHandler(UINT32 Addr, UINT32 Size)
{
	PUSBCMD_PAYLOAD_HEADER pPayloadHeader  = (PUSBCMD_PAYLOAD_HEADER)Addr;
	static UINT32 RemainLen;
	static FST_FILE fhdl = 0;
	UINT32 bufAddr = 0, fileSize = 0;
	UINT32 PayloadHeadSize = sizeof(USBCMD_PAYLOAD_HEADER);
	if (pPayloadHeader->Type == USBCMD_PAYLOAD_RAW_DATA && Size >= PayloadHeadSize) {
		if (pPayloadHeader->TotalLength) {
			//begin
			RemainLen = pPayloadHeader->TotalLength;
			if (RemainLen > PayloadHeadSize) {
				if (fhdl) {
					DBG_WRN("Host flow NG! Close previous file first!\r\n");
					FileSys_CloseFile(fhdl);
					fhdl = 0;
					UploadFile[0] = 0;//reset path
				}
				DBG_IND("TotalLength=%d\r\n", RemainLen);
				RemainLen -= PayloadHeadSize;
				fileSize = Size - PayloadHeadSize;
				bufAddr = Addr + PayloadHeadSize;
				if (strlen(UploadFile) > strlen("A:\\")) {
					fhdl = FileSys_OpenFile(UploadFile, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
					FileSys_SeekFile(fhdl, 0, FST_SEEK_SET);
				} else {
					DBG_ERR("Set upload file path by CMD 5001 first!\r\n");
				}
			} else {
				DBG_ERR("Payload header size(%d) error.\r\n", RemainLen);
			}
		} else {
			//end
			if (fhdl) {
				DBG_IND("Close unknown length file.\r\n");
				FileSys_CloseFile(fhdl);
				fhdl = 0;
				UploadFile[0] = 0;//reset path
			}
		}
	} else {
		bufAddr = Addr;
		fileSize = Size;
	}
	if (fileSize && fhdl) {
		//DBG_IND("write file(%d).\r\n",fileSize);
		FileSys_WriteFile(fhdl, (UINT8 *)bufAddr, &fileSize, 0, NULL);
		if (RemainLen >= fileSize) {
			RemainLen -= fileSize;
		} else {
			RemainLen = 0;
			DBG_WRN("Payload header size NG!\r\n");
		}
		if (0 == RemainLen) {
			DBG_IND("Close file.\r\n");
			FileSys_CloseFile(fhdl);
			fhdl = 0;
			UploadFile[0] = 0;//reset path
		}
	}
}

//send data to USB host
void UsbCmdDownloadHandler(void)
{
	FST_FILE fhdl = 0;
	UINT32 BufSize = g_SendBuf2.Size;
	DBG_FUNC_BEGIN("\r\n");
	fhdl = FileSys_OpenFile(DownloadFile, FST_OPEN_READ | FST_OPEN_EXISTING);
	if (fhdl) {
		USBCMD_PAYLOAD_HEADER PayloadHeader;
		UINT32 HeaderSize = sizeof(USBCMD_PAYLOAD_HEADER);
		UINT32 RemainSize;
		FST_FILE_STATUS   FileStat;
		FileSys_StatFile(fhdl, &FileStat);

		PayloadHeader.TotalLength = HeaderSize + (UINT32)FileStat.uiFileSize;
		PayloadHeader.Type = USBCMD_PAYLOAD_RAW_DATA;
		//send payload header
		UVAC_WriteCdcData(USB_DATA_PORT, (UINT8 *)&PayloadHeader, (UINT32 *)&HeaderSize);
		RemainSize = (UINT32)FileStat.uiFileSize;
		DBG_IND("Download %s, size=%d(0x%X).\r\n", DownloadFile, RemainSize, RemainSize);
		//send payload data
//Perf_Mark();
		while (RemainSize) {
			if (RemainSize > g_SendBuf2.Size) {
				BufSize = g_SendBuf2.Size;
			} else {
				BufSize = RemainSize;
			}
			FileSys_ReadFile(fhdl, (UINT8 *)g_SendBuf2.Addr, &BufSize, 0, NULL);
			if (E_OK == UVAC_WriteCdcData(USB_DATA_PORT, (UINT8 *)g_SendBuf2.Addr, &BufSize)) {
				RemainSize -= BufSize;
			} else {
				DBGD(RemainSize);
				break;
			}
		}
//DBG_DUMP("%dKB,%dms, %d KB/sec\r\n",PayloadHeader.TotalLength/1024,Perf_GetDuration()/1000,PayloadHeader.TotalLength*1000/(Perf_GetDuration()/1000)/1024);
		FileSys_CloseFile(fhdl);
		DownloadFile[0] = 0;//reset path
	} else {
		DBG_ERR("%s didn't exist.\r\n", DownloadFile);
	}

	DBG_FUNC_END("\r\n");
}

WIFI_CMD_BEGIN(uvc)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_RECORDING_TIME, 0, (UINT32)XML_GetMovieRecStatus, 0, FL_WIFI_MOVIE_FMT)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_TRIGGER_RAWENC, NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC, 0, WIFIFLAG_MOVIE_REC_RAWENC_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GET_RAWENC_JPG, 0, (UINT32)XML_GetRawEncJpg, 0, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GET_LIVEVIEW_FMT, 0, (UINT32)XML_GetLiveViewFmt, 0, FL_NULL)

WIFI_CMD_ITEM(WIFIAPP_CMD_RECORD, NVTEVT_WIFI_EXE_MOVIE_REC, 0, WIFIFLAG_RECORD_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_SIZE, NVTEVT_WIFI_EXE_MOVIE_REC_SIZE, 0, 0, FL_MOVIE_SIZE)
WIFI_CMD_ITEM(WIFIAPP_CMD_CYCLIC_REC, NVTEVT_WIFI_EXE_CYCLIC_REC, 0, 0, FL_MOVIE_CYCLIC_REC)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_WDR, NVTEVT_WIFI_EXE_MOVIE_WDR, 0, 0, FL_MOVIE_WDR)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_EV, NVTEVT_WIFI_EXE_MOVIE_EV, 0, 0, FL_EV)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOTION_DET, NVTEVT_WIFI_EXE_MOTION_DET, 0, 0, FL_MOVIE_MOTION_DET)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_AUDIO, NVTEVT_WIFI_EXE_MOVIE_AUDIO, 0, 0, FL_MOVIE_AUDIO)
WIFI_CMD_ITEM(WIFIAPP_CMD_DATEIMPRINT, NVTEVT_WIFI_EXE_DATEIMPRINT, 0, 0, FL_MOVIE_DATEIMPRINT)
WIFI_CMD_ITEM(WIFIAPP_CMD_MAX_RECORD_TIME, 0, (UINT32)XML_GetMaxRecordTime, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GSENSOR_SENS, NVTEVT_WIFI_EXE_MOVIE_GSENSOR_SENS, 0, 0, FL_GSENSOR)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_AUTO_RECORDING, NVTEVT_WIFI_EXE_SET_AUTO_RECORDING, 0, 0, FL_WIFI_AUTO_RECORDING)
//WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_BITRATE    ,NVTEVT_WIFI_EXE_MOVIE_REC_BITRATE, 0,0, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_BITRATE, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE, 0,0, FL_NULL)
//since it might change mode to re-open UsbCmdTsk, just don't wiat for WIFIFLAG_PREVIEW_DONE
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_START, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START, 0, 0/*WIFIFLAG_PREVIEW_DONE*/, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_MODECHANGE,NVTEVT_WIFI_EXE_MODE,0,WIFIFLAG_MODE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY, 0, (UINT32)XML_QueryCmd, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_DATE, NVTEVT_WIFI_EXE_SET_DATE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_TIME, NVTEVT_WIFI_EXE_SET_TIME, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_POWEROFF, NVTEVT_WIFI_EXE_POWEROFF, 0, 0, FL_AUTO_POWER_OFF)
WIFI_CMD_ITEM(WIFIAPP_CMD_LANGUAGE, NVTEVT_WIFI_EXE_LANGUAGE, 0, 0, FL_LANGUAGE)
WIFI_CMD_ITEM(WIFIAPP_CMD_TVFORMAT, NVTEVT_WIFI_EXE_TVFORMAT, 0, 0, FL_TV_MODE)
WIFI_CMD_ITEM(WIFIAPP_CMD_FORMAT, NVTEVT_WIFI_EXE_FORMAT, 0, WIFIFLAG_FORMAT_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SYSRESET, NVTEVT_WIFI_EXE_SYSRESET, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_VERSION, 0, (UINT32)XML_GetVersion, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_FWUPDATE, NVTEVT_WIFI_EXE_FWUPDATE, 0, WIFIFLAG_UPDATE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_CUR_STATUS, 0, (UINT32)XML_QueryCmd_CurSts, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_FILELIST, 0, (UINT32)XML_FileList, 0, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_HEARTBEAT, 0, (UINT32)XML_GetHeartBeat, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DISK_FREE_SPACE, 0, (UINT32)XML_GetDiskFreeSpace, 0, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_RECONNECT_WIFI,NVTEVT_WIFI_EXE_RECONNECT       ,0,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_BATTERY, 0, (UINT32)XML_GetBattery, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SAVE_MENUINFO, NVTEVT_WIFI_EXE_SAVEMENU, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_HW_CAP, 0, (UINT32)XML_HWCapability, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_REMOVE_USER, NVTEVT_WIFI_EXE_REMOVE_USER, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_CARD_STATUS, 0, (UINT32)XML_GetCardStatus, 0, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_GET_DOWNLOAD_URL, 0, (UINT32)XML_GetDownloadURL,0, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_GET_UPDATEFW_PATH, 0, (UINT32)XML_GetUpdateFWPath,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_PIP_STYLE, NVTEVT_WIFI_EXE_PIP_STYLE, 0, 0, FL_DUAL_CAM)
//WIFI_CMD_ITEM(WIFIAPP_CMD_GET_SSID_PASSPHRASE, 0, (UINT32)XML_GetSSID_passphrase, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_MOVIE_SIZE, 0, (UINT32)XML_GetMovieSizeCapability, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_MENUITEM, 0, (UINT32)XML_GetMenuItem, 0, FL_NULL)
//WIFI_CMD_ITEM(WIFIAPP_CMD_AUTO_TEST_CMD_DONE, 0       ,(UINT32)XML_AutoTestCmdDone,0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_THUMB, 0, (UINT32)XML_GetThumbnail, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SCREEN, 0, (UINT32)XML_GetScreen, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_FILE_INFO, 0, (UINT32)XML_GetMovieFileInfo, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DELETE_ONE, 0, (UINT32)XML_DeleteOnePicture, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DELETE_ALL, 0, (UINT32)XML_DeleteAllPicture, 0, FL_NULL)
WIFI_CMD_ITEM(USBCMD_UPLOAD_FILE_PATH, 0, (UINT32)XML_SetUploadPath, 0, FL_NULL)
WIFI_CMD_ITEM(USBCMD_DOWNLOAD_FILE_PATH, 0, (UINT32)XML_SetDownloadPath, 0, FL_NULL)
WIFI_CMD_END()


#endif
//#NT#2016/05/31#Ben Wang -end
