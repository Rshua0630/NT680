#include "WifiCmdParser.h"
#include "FileSysTsk.h"
#include "stdio.h"
#include "NvtVerInfo.h"
#include "WifiCmdParserInt.h"


NVTVER_VERSION_ENTRY(WiFiCmdParser, 1, 00, 000, 00)

#define WIFICMD_STRCPY(dst, src, dst_size)  do { strncpy(dst, src, dst_size-1); dst[dst_size-1] = '\0'; } while(0)
#define WIFICMD_MIMETYPE_MAXLEN             40   ///<  mime type max length,refer to CYG_HFS_MIMETYPE_MAXLEN


static WIFI_CMD_ENTRY *g_pCmdTab;
static WifiCmd_DefCB *gDefReturnFormat = 0;
static WifiCmd_EventHandle *gEventHandle = 0;
static WifiCmd_APPStartupCheck *gAppStartupChecker = NULL;
static UINT32 g_result = 0;
static UINT32 g_receiver = 0;
static UINT32 g_app_startup_cmd = 0;
static UINT32 g_curWifiCmd = 0;
static char   g_parStr[WIFI_PAR_STR_LEN];
extern UINT32 WifiCmd_WaitFinish(FLGPTN waiptn);
extern void   WifiCmd_Lock(void);
extern void   WifiCmd_Unlock(void);
typedef enum {
	WIFICMD_PAR_NULL = 0,
	WIFICMD_PAR_NUM,
	WIFICMD_PAR_STR,
	ENUM_DUMMY4WORD(WIFICMD_PAR_TYPE)
} WIFICMD_PAR_TYPE;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WifiCmd_ReceiveCmd(UINT32 enable)
{
	DBG_IND("WifiCmd_ReceiveCmd %d\r\n", enable);

	if (enable) {
		if (g_receiver) {
			DBG_WRN("is enabled\r\n");
			return ;
		}

		if ((!g_pCmdTab) || (!gEventHandle)) {
			DBG_ERR("no Cmd Table or event handle\r\n");
			return;
		}
		clr_flg(FLG_ID_WIFICMD, 0xFFFFFFFF);
		g_receiver = enable;
	} else {
		if (!g_receiver) {
			DBG_WRN("is disabled\r\n");
			return ;
		}

		g_receiver = enable;
		//UI want to close,some app cmd might wait UI finish
		//can not wait flag by UI,therefore set flag for app cmd
		WifiCmd_Done(0xFFFFFFFF, WIFI_CMD_TERMINATE);
	}
}

void WifiCmd_UnlockString(char *string)
{
	sig_sem(WIFISTR_SEM_ID);
}

char *WifiCmd_LockString(void)
{
	wai_sem(WIFISTR_SEM_ID);
	return g_parStr;
}

void WifiCmd_Lock(void)
{
	DBG_IND("Lock\r\n");
	wai_sem(WIFICMD_SEM_ID);
}
void WifiCmd_Unlock(void)
{
	DBG_IND("Unlock\r\n");
	sig_sem(WIFICMD_SEM_ID);
}
void WifiCmd_Done(FLGPTN flag, UINT32 result)
{
	DBG_IND("flag %x result %d\r\n", flag, result);
	g_result = result;
	set_flg(FLG_ID_WIFICMD, flag);

}

UINT32 WifiCmd_WaitFinish(FLGPTN waiptn)
{
	FLGPTN uiFlag;

	DBG_IND("waiptn %x\r\n", waiptn);
	wai_flg(&uiFlag, FLG_ID_WIFICMD, waiptn, (TWF_ORW));
	return g_result;
}

void WifiCmd_ClrFlg(FLGPTN flag)
{
	DBG_IND("clear %x\r\n", flag);
	clr_flg(FLG_ID_WIFICMD, flag);

}
void WifiCmd_SetExecTable(WIFI_CMD_ENTRY *pAppCmdTbl)
{
	g_pCmdTab = pAppCmdTbl;
}
WIFI_CMD_ENTRY *WifiCmd_GetExecTable(void)
{
	return g_pCmdTab;
}
void WifiCmd_SetDefautCB(UINT32 defaultCB)
{
	gDefReturnFormat = (WifiCmd_DefCB *)defaultCB;
}
void WifiCmd_SetEventHandle(UINT32 eventHandle)
{
	gEventHandle = (WifiCmd_EventHandle *)eventHandle;
}
void WifiCmd_SetAppStartupChecker(UINT32 app_startupchecker)
{
	gAppStartupChecker = (WifiCmd_APPStartupCheck *)app_startupchecker;
}
void WifiCmd_SetAppStartupCmdCode(UINT32 cmd_code)
{
	g_app_startup_cmd = cmd_code;
}


UINT32 WifiCmd_GetCurCmd(void)
{
	return g_curWifiCmd;
}
static UINT32 WifiCmd_DispatchCmd(UINT32 cmd, WIFICMD_PAR_TYPE parType, UINT32 par, UINT32 *UserCB)
{
	int i = 0;
	UINT32 ret = 0;

	g_curWifiCmd = 0;

	if (g_receiver) {
		// Do cmd dispatch if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI || cmd == WIFIAPP_CMD_APP_STARTUP)
		if (!gAppStartupChecker) {
			DBG_ERR("Call WifiCmd_SetAppStartupChecker() first!\r\n");
			return WIFI_CMD_NOT_FOUND;
		}

		if (gAppStartupChecker() || cmd == g_app_startup_cmd) {
			while (g_pCmdTab[i].cmd != 0) {
				if (cmd == g_pCmdTab[i].cmd) {
					//UINT32 t1,t2;
					//t1 = Perf_GetCurrent()/1000;
					DBG_IND("cmd:%d evt:%x par:%d CB:%x wait:%x\r\n", g_pCmdTab[i].cmd, g_pCmdTab[i].event, par, g_pCmdTab[i].usrCB, g_pCmdTab[i].Waitflag);
					g_curWifiCmd = cmd;
					if (g_pCmdTab[i].Waitflag) {
						clr_flg(FLG_ID_WIFICMD, g_pCmdTab[i].Waitflag);
						g_result = 0;
					}
					if (gEventHandle && g_pCmdTab[i].event) {
						if (parType == WIFICMD_PAR_NUM) {
							gEventHandle(g_pCmdTab[i].event, 1, par);
						} else if (parType == WIFICMD_PAR_STR) {
							char  *parStr;//post event,data in stack would release
							parStr = WifiCmd_LockString();
							memset(parStr, '\0', WIFI_PAR_STR_LEN);
							sscanf_s((char *)par, "%s", parStr, WIFI_PAR_STR_LEN);
							gEventHandle(g_pCmdTab[i].event, 1, parStr);
						} else {
							gEventHandle(g_pCmdTab[i].event, 0);
						}
					}
					if (g_pCmdTab[i].Waitflag) {
						ret = WifiCmd_WaitFinish(g_pCmdTab[i].Waitflag);
					}
					if (g_pCmdTab[i].usrCB) {
						*UserCB = (UINT32)g_pCmdTab[i].usrCB;
					}
					//t2 = Perf_GetCurrent()/1000;
					//DBG_ERR("time %d ms\r\n",t2-t1);
					DBG_IND("ret %d\r\n", ret);
					g_curWifiCmd = 0;
					return ret;
				}
				i++;
			}
		} else {
			DBG_ERR("should perform WIFIAPP_CMD_APP_STARTUP first!\r\n");
			return WIFI_CMD_STATE_ERR;
		}
	} else {
		DBG_ERR("should WifiCmd_ReceiveCmd enable\r\n");
	}
	return WIFI_CMD_NOT_FOUND;

}
INT32 WifiCmd_GetData(char *path, char *argument, UINT32 bufAddr, UINT32 *bufSize, char *mimeType, UINT32 segmentCount)
{
	UINT32            cmd = 0, par = 0;
	char             *pch = 0;
	UINT32            ret = 0;
	UINT32            UserCB = 0;
	//char              *parStr;//post event,data in stack would release
	DBG_IND("Data2 path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n", path, argument, mimeType, *bufSize, segmentCount);


	if (segmentCount == WIFI_CMD_GETDATA_SEGMENT_ERROR_BREAK) {
		DBG_ERR("segmentCount %d\r\n", segmentCount);
		WifiCmd_Unlock();
		return WIFI_CMD_GETDATA_RETURN_ERROR;
	}

	if (strncmp(argument, CMD_STR, strlen(CMD_STR)) == 0) {
		if (segmentCount == 0) {
			WifiCmd_Lock();
		}
		sscanf_s(argument + strlen(CMD_STR), "%d", &cmd);
		pch = strchr(argument + strlen(CMD_STR), '&');
		if (pch) {
			if (strncmp(pch, PAR_STR, strlen(PAR_STR)) == 0) {
				sscanf_s(pch + strlen(PAR_STR), "%d", &par);
				ret = WifiCmd_DispatchCmd(cmd, WIFICMD_PAR_NUM, par, &UserCB);
			} else if (strncmp(pch, PARS_STR, strlen(PARS_STR)) == 0) {
				//DBG_ERR("%s\r\n",pch+strlen(PARS_STR));
				ret = WifiCmd_DispatchCmd(cmd, WIFICMD_PAR_STR, (UINT32)pch + strlen(PARS_STR), &UserCB);
			}
		} else {
			ret = WifiCmd_DispatchCmd(cmd, WIFICMD_PAR_NULL, 0, &UserCB);
		}
		if (UserCB) {
			UINT32 hfs_result;
			hfs_result = ((WifiCmd_getCustomData *)UserCB)(path, argument, bufAddr, bufSize, mimeType, segmentCount);

			if (hfs_result != WIFI_CMD_GETDATA_RETURN_CONTINUE) {
				WifiCmd_Unlock();
			}
			return hfs_result;
		} else { //default return value xml
			if (gDefReturnFormat) {
				gDefReturnFormat(cmd, ret, bufAddr, bufSize, mimeType);
				WifiCmd_Unlock();
				return WIFI_CMD_GETDATA_RETURN_OK;
			} else {
				DBG_ERR("no default CB\r\n");
				WifiCmd_Unlock();
				return WIFI_CMD_GETDATA_RETURN_ERROR;
			}
		}
	} else { //for test,list url command
		UINT32            len = 0;
		char             *buf = (char *)bufAddr;
		FST_FILE          filehdl = 0;
		char              pFilePath[32];
		UINT32            fileLen = *bufSize;

		snprintf(pFilePath, sizeof(pFilePath), "%s", HTML_PATH); //html of all command list
		filehdl = FileSys_OpenFile(pFilePath, FST_OPEN_READ);
		if (filehdl) {
			// set the data mimetype
			WIFICMD_STRCPY(mimeType, "text/html", WIFICMD_MIMETYPE_MAXLEN);
			FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0, 0);
			FileSys_CloseFile(filehdl);
			*bufSize = fileLen;
			*(buf + fileLen) = '\0';
		} else {
			WIFICMD_STRCPY(mimeType, "text/html", WIFICMD_MIMETYPE_MAXLEN);
			len = snprintf(buf, *bufSize, "no %s file", HTML_PATH);
			buf += len;
			*bufSize = (UINT32)(buf) - bufAddr;

		}
	}
	return WIFI_CMD_GETDATA_RETURN_OK;
}
INT32 WifiCmd_PutData(char *path, char *argument, UINT32 bufAddr, UINT32 bufSize, UINT32 segmentCount, UINT32 putStatus)  ///< Callback function for put custom data.
{
	UINT32            cmd = 0, par = 0;
	char             *pch = 0;
	UINT32            ret = 0;
	UINT32            UserCB = 0;

	DBG_IND("path =%s, argument = %s, bufAddr = 0x%x, bufSize =0x%x , segmentCount  =%d , putStatus = %d\r\n", path, argument, bufAddr, bufSize, segmentCount, putStatus);

	if (strncmp(argument, CMD_STR, strlen(CMD_STR)) == 0) {
		if (segmentCount == 0) {
			WifiCmd_Lock();
		}
		sscanf_s(argument + strlen(CMD_STR), "%d", &cmd);
		pch = strchr(argument + strlen(CMD_STR), '&');
		if (pch) {
			if (strncmp(pch, PAR_STR, strlen(PAR_STR)) == 0) {
				sscanf_s(pch + strlen(PAR_STR), "%d", &par);
				ret = WifiCmd_DispatchCmd(cmd, WIFICMD_PAR_NUM, par, &UserCB);
			} else if (strncmp(pch, PARS_STR, strlen(PARS_STR)) == 0) {
				ret = WifiCmd_DispatchCmd(cmd, WIFICMD_PAR_STR, (UINT32)pch + strlen(PARS_STR), &UserCB);
			}
		} else {
			ret = WifiCmd_DispatchCmd(cmd, WIFICMD_PAR_NULL, 0, &UserCB);
		}

		if ((ret == 0) && (UserCB)) {
			UINT32 hfs_result;
			hfs_result = ((WifiCmd_puttCustomData *)UserCB)(path, argument, bufAddr, bufSize, segmentCount, putStatus);

			if (putStatus == WIFI_CMD_PUT_STATUS_FINISH) {
				WifiCmd_Unlock();
			}
			return hfs_result;
		} else {
			DBG_ERR("no default CB\r\n");
			WifiCmd_Unlock();
			return WIFI_CMD_UPLOAD_FAIL_WRITE_ERROR;
		}
	}

	return WIFI_CMD_UPLOAD_OK;
}
