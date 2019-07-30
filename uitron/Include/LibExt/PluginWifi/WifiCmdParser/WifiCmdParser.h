#ifndef _WIFICMDPARSER_H_
#define _WIFICMDPARSER_H_

#include "Type.h"
extern void WifiCmd_InstallID(void) _SECTION(".kercfg_text");


typedef struct _WIFI_CMD_ENTRY {
	UINT32                cmd;
	UINT32                event;
	UINT32                usrCB;
	FLGPTN                Waitflag;
	UINT32                UIflag;
}
WIFI_CMD_ENTRY;

typedef void WifiCmd_DefCB(UINT32 cmd, UINT32 ret, UINT32 bufAddr, UINT32 *bufSize, char *mimeType);
typedef void WifiCmd_EventHandle(UINT32 evt, UINT32 paramNum, ...);
typedef int  WifiCmd_getCustomData(char *path, char *argument, UINT32 bufAddr, UINT32 *bufSize, char *mimeType, UINT32 segmentCount);
typedef int  WifiCmd_puttCustomData(char *path, char *argument, UINT32 bufAddr, UINT32 bufSize, UINT32 segmentCount, UINT32 putStatus);
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
typedef INT32  WifiCmd_APPStartupCheck(void);
//#NT#2016/03/23#Isiah Chang -end

#define WIFI_CMD_BEGIN(tbl) WIFI_CMD_ENTRY (Cmd_##tbl)[]={ ///< begin a command table
#define WIFI_CMD_ITEM(cmd,event,func,wait,UIflag)  {(cmd), (event),(func),(wait),(UIflag)}, ///< insert a command item in command table
#define WIFI_CMD_END()    {0,0,0,0}}; ///< end a command table

#define CMD_STR "custom=1&cmd="
#define PAR_STR "&par="
#define PARS_STR "&str="
#define HTML_PATH "A:\\test.htm"

#define WIFI_CMD_STATE_ERR    (-22)
#define WIFI_CMD_TERMINATE    (-255)
#define WIFI_CMD_NOT_FOUND    (-256)

#define WIFI_CMD_GETDATA_RETURN_ERROR               -1   // has error
#define WIFI_CMD_GETDATA_RETURN_OK                  0    // ok get all data
#define WIFI_CMD_GETDATA_RETURN_CONTINUE            1    // has more data need to get
#define WIFI_CMD_GETDATA_SEGMENT_ERROR_BREAK        0xFFFFFFFF    ///<  hfs error break, the connection may be closed

#define WIFI_CMD_UPLOAD_OK                      0    ///<  upload file ok
#define WIFI_CMD_UPLOAD_FAIL_FILE_EXIST         -1   ///<  upload file fail because of file exist
#define WIFI_CMD_UPLOAD_FAIL_RECEIVE_ERROR      -2   ///<  receive data has some error
#define WIFI_CMD_UPLOAD_FAIL_WRITE_ERROR        -3   ///<  write file has some error
#define WIFI_CMD_UPLOAD_FAIL_FILENAME_EMPTY     -4   ///<  file name is emtpy

typedef enum _WIFI_CMD_PUT_STATUS {
	WIFI_CMD_PUT_STATUS_CONTINUE               =   0, ///< still have data need to put
	WIFI_CMD_PUT_STATUS_FINISH                 =   1, ///< put data finish
	WIFI_CMD_PUT_STATUS_ERR                    =  -1, ///< some error happened
} WIFI_CMD_PUT_STATUS;

#define WIFI_PAR_STR_LEN        (1024)      ///< max string length of URL cmd

extern void   WifiCmd_Done(FLGPTN flag, UINT32 result);
extern void   WifiCmd_SetExecTable(WIFI_CMD_ENTRY *pAppCmdTbl);
extern WIFI_CMD_ENTRY   *WifiCmd_GetExecTable(void);
extern INT32  WifiCmd_GetData(char *path, char *argument, UINT32 bufAddr, UINT32 *bufSize, char *mimeType, UINT32 segmentCount);
extern INT32  WifiCmd_PutData(char *path, char *argument, UINT32 bufAddr, UINT32 bufSize, UINT32 segmentCount, UINT32 putStatus);
extern void   WifiCmd_SetDefautCB(UINT32 defaultCB);
extern void   WifiCmd_SetEventHandle(UINT32 eventHandle);
extern void   WifiCmd_ReceiveCmd(UINT32 enable);
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
extern void   WifiCmd_SetAppStartupChecker(UINT32 app_startupchecker);
extern void   WifiCmd_SetAppStartupCmdCode(UINT32 cmd_code);
//#NT#2016/03/23#Isiah Chang -end
extern void   WifiCmd_UnlockString(char *string);
extern char   *WifiCmd_LockString(void);
extern UINT32 WifiCmd_WaitFinish(FLGPTN waiptn);
extern void   WifiCmd_ClrFlg(FLGPTN flag);
extern UINT32 WifiCmd_GetCurCmd(void);
#endif //_WIFICMDPARSER_H_



