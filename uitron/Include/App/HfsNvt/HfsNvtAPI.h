/////////////////////////////////////////////////////////////////
/*
    Copyright (c) 2014~  Novatek Microelectronics Corporation

    @file HfsNvtAPI.h

    @version

    @date

*//////////////////////////////////////////////////////////////////
#ifndef _HFSNVTAPI_H
#define _HFSNVTAPI_H
#include "hfs.h"
/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum _HFSNVT_WR {
	HFSNVT_WR_OK                =   0, ///< no warning
	HFSNVT_WR_ALREADY_OPEN      =  -1, ///< HfsNvt_Open called twice
	HFSNVT_WR_NOT_OPEN          =  -2, ///< HfsNvt_Open not to call before.
	ENUM_DUMMY4WORD(HFSNVT_WR)
} HFSNVT_WR;


/**
     @name HFS maximum length definition.
*/
//@{
#define HFS_ROOT_DIR_MAXLEN   64      ///< root dir maximum length
#define HFS_KEY_MAXLEN        64      ///< key maximum length
#define HFS_USER_QUERY_MAXLEN 24      ///< user query string maximum length
#define HFS_CUSTOM_STR_MAXLEN 24      ///< custom string max length
//@}

/**
    Callback functions prototype that HfsNvt task used.

    These definitions are used in the structure HFSNVT_OPEN and would be used during HfsNvt_Open() to specify the
    HfsNvt Tsk needed callback function.
*/
//@{
typedef void  HFSNVT_SERVER_EVENT_CB(int status);   ///< Callback function for notify the HFS server status.
typedef INT32 HFSNVT_GET_CUSTOM_CB(CHAR *path, CHAR *argument, UINT32 bufAddr, UINT32 *bufSize, CHAR *mimeType, UINT32 segmentCount);  ///< Callback function for get custom data.
typedef INT32 HFSNVT_CHK_PASSWD_CB(const CHAR *username, const CHAR *password, CHAR *key, CHAR *questionstr);  ///< Callback function for check password.
typedef INT32 HFSNVT_CLIENT_QUERY_CB(CHAR *path, CHAR *argument, UINT32 bufAddr, UINT32 *bufSize, CHAR *mimeType, UINT32 segmentCount); ///< Callback function for client query
typedef INT32 HFSNVT_UPLOAD_RESULT_CB(int result, UINT32 bufAddr, UINT32 *bufSize, CHAR *mimeType);  ///< Callback function for upload command complete
typedef INT32 HFSNVT_DELETE_RESULT_CB(int result, UINT32 bufAddr, UINT32 *bufSize, CHAR *mimeType);  ///< Callback function for delete command complete
typedef INT32 HFSNVT_PUT_CUSTOM_CB(CHAR *path, CHAR *argument, UINT32 bufAddr, UINT32 bufSize, UINT32 segmentCount, HFS_PUT_STATUS putStatus);  ///< Callback function for put custom data.
typedef INT32 HFSNVT_HEADER_CB(UINT32 headerAddr, UINT32 headerSize, CHAR *filepath, CHAR *mimeType, void *reserved);  ///< Callback function for http header parse custom.
typedef void  HFSNVT_DOWNLOAD_RESULT_CB(int result, const CHAR *path);///< Callback function for download command complete

//@}


/**
    Init parameters for HfsNvt module.

    This structure is used in HfsNvt_Open() to specify the HFSNvt task needed information for normal working
    and user customization callback.
*/
typedef struct {
	HFSNVT_SERVER_EVENT_CB  *serverEvent;                            ///<  Callback function for notify the HFS server status.
	HFSNVT_GET_CUSTOM_CB    *getCustomData;                          ///<  Callback function for get custom data.
	HFSNVT_PUT_CUSTOM_CB    *putCustomData;                          ///<  Callback function for put custom data.
	HFSNVT_CHK_PASSWD_CB    *check_pwd;                              ///<  Callback function for check password.
	HFSNVT_CLIENT_QUERY_CB  *clientQuery;                            ///<  Callback function for client query.
	char                    clientQueryStr[HFS_USER_QUERY_MAXLEN + 1]; ///<  client query string.
	HFSNVT_UPLOAD_RESULT_CB *uploadResultCb;                         ///<  Callback function for upload command complete
	HFSNVT_DELETE_RESULT_CB *deleteResultCb;                         ///<  Callback function for delete command complete
	UINT32                  portNum;                                 ///<  server port number
	UINT32                  threadPriority;                          ///<  server thread priority
	CHAR                    rootdir[HFS_ROOT_DIR_MAXLEN + 1];        ///<  root of the visible filesystem
	CHAR                    key[HFS_KEY_MAXLEN + 1];                 ///<  the key used in check password callback function
	UINT32                  sockbufSize;                             ///<  socket buffer size
	UINT32                  sharedMemAddr;                           ///<  shared memory address
	UINT32                  sharedMemSize;                           ///<  shared memory size
	UINT32                  maxClientNum;                            ///<  maximum client connection at the same time
	UINT32                  httpsPortNum;                            ///<  https port number
	char                    customStr[HFS_CUSTOM_STR_MAXLEN + 1];    ///<  custom string
	UINT32                  timeoutCnt;                              ///<  timeout counter for send & receive , time base is 0.5 sec
	BOOL                    forceCustomCallback;                     ///<  force custom callback even url not have "custom=1"
	HFSNVT_HEADER_CB        *headerCb;                               ///<  Callback function for header data parse by customer
	HFSNVT_DOWNLOAD_RESULT_CB *downloadResultCb;                        ///<  Callback function for download command complete
} HFSNVT_OPEN;
/*-----------------------------------
    functions declare
-----------------------------------*/

/**
     Install HfsNvt flag and semaphore id.

*/
extern void       HfsNvt_InstallID(void) _SECTION(".kercfg_text");

/**
    Open HfsNvt task.

    This is the HfsNvt task initial function.

    @param[in] pOpen:  Init info for HfsNvt.

    @return
        - @b E_OK: initial success.
        - @b E_PAR: the initial parameter has some error.

    Example:
    @code
    {
        HFSNVT_OPEN  hfsObj={0};
        int          checkpwd = 0;
        int          clientQuery = 0;
        char         clientQueryStr[20]="query_devinfo";
        int          uploadCB = 1;
        int          deleteCB = 1;

        hfsObj.serverEvent = xExamHfs_notifyStatus;
        hfsObj.getCustomData = xExamHfs_getCustomData;
        if (checkpwd)
        {
            hfsObj.check_pwd = xExamHfs_CheckPasswd;
        }
        if (clientQuery)
        {
            hfsObj.clientQuery = xExamHfs_ClientQueryData;
            strncpy(hfsObj.clientQueryStr, clientQueryStr,HFS_USER_QUERY_MAXLEN);
            hfsObj.clientQueryStr[HFS_USER_QUERY_MAXLEN]=0;
        }
        if (uploadCB)
            hfsObj.uploadResultCb = xExamHfs_uploadResultCb;
        if (deleteCB)
            hfsObj.deleteResultCb = xExamHfs_deleteResultCb;
        hfsObj.portNum = 80;
        hfsObj.threadPriority = 6;
        strncpy(hfsObj.rootdir, "/sdcard", HFS_ROOT_DIR_MAXLEN);
        hfsObj.rootdir[HFS_ROOT_DIR_MAXLEN]=0;
        hfsObj.sockbufSize = 51200;
        hfsObj.sharedMemAddr = SxCmd_GetTempMem(0x1000000);
        hfsObj.sharedMemSize = 66*1024;
        HfsNvt_Open(&hfsObj);
    }
    @endcode

*/
extern ER     HfsNvt_Open(HFSNVT_OPEN *pOpen);

/**
    Close HfsNvt task.

    @return
        - @b E_OK: close success.

*/
extern ER     HfsNvt_Close(void);


#endif  // _HFSNVTAPI_H

