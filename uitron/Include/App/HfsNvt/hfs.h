#ifndef CYGONCE_NET_HFS_H
#define CYGONCE_NET_HFS_H
/* =================================================================
 *
 *      hfs.h
 *
 *      A simple http file server.
 *
 * =================================================================
 */
#define HFS_INTERFACE_VER 0x16121309

#include <stdio.h>

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef unsigned long       HFS_U32;                ///<  Unsigned 32 bits data type
typedef unsigned char       HFS_U8;                 ///<  Unsigned 8 bits data type

#ifndef ENUM_DUMMY4WORD
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
#endif

/**
     @name HFS notify status
*/
//@{
#define CYG_HFS_STATUS_CLIENT_REQUEST           0   ///<  client has request comimg in
#define CYG_HFS_STATUS_SERVER_RESPONSE_START    1   ///<  server send response data start
#define CYG_HFS_STATUS_SERVER_RESPONSE_END      2   ///<  server send response data end
#define CYG_HFS_STATUS_CLIENT_DISCONNECT        3   ///<  client disconnect.
#define CYG_HFS_STATUS_SERVER_EXIT              4   ///<  notify that the server is exit
//@}

/**
     @name HFS get custom data return value
*/
//@{
#define CYG_HFS_CB_GETDATA_RETURN_ERROR        -1   ///<  has error
#define CYG_HFS_CB_GETDATA_RETURN_OK           0    ///<  ok get all data
#define CYG_HFS_CB_GETDATA_RETURN_CONTINUE     1    ///<  has more data need to get

#define CYG_HFS_CB_GETDATA_SEGMENT_ERROR_BREAK         0xFFFFFFFF    ///<  hfs error break, the connection may be closed

//@}


/**
     @name HFS list dir return value
*/
//@{
#define CYG_HFS_LIST_DIR_OPEN_FAIL             -1   ///<  has error when open dir
#define CYG_HFS_LIST_DIR_OK                    0    ///<  ok get all data
#define CYG_HFS_LIST_DIR_CONTINUE              1    ///<  has more data need to list
//@}


/**
     @name HFS upload file result
*/
//@{
#define CYG_HFS_UPLOAD_OK                      0    ///<  upload file ok
#define CYG_HFS_UPLOAD_FAIL_FILE_EXIST         -1   ///<  upload file fail because of file exist
#define CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR      -2   ///<  receive data has some error
#define CYG_HFS_UPLOAD_FAIL_WRITE_ERROR        -3   ///<  write file has some error
#define CYG_HFS_UPLOAD_FAIL_FILENAME_EMPTY     -4   ///<  file name is emtpy
#define CYG_HFS_UPLOAD_FAIL_MD5_CHK_FAIL       -5   ///<  upload file MD5 check fail
//@}

/**
     @name HFS download file result
*/
//@{
#define CYG_HFS_DOWNLOAD_OK                      0    ///<  download file ok
#define CYG_HFS_DOWNLOAD_FILE_NOT_EXIST         -1    ///<  download file not exist
#define CYG_HFS_DOWNLOAD_FAIL                   -2    ///<  download file fail
//@}

/**
     @name HFS delete file result
*/
//@{
#define CYG_HFS_DELETE_OK                      0    ///<  delete file ok
#define CYG_HFS_DELETE_FAIL                    -1   ///<  delete file fail
//@}


/**
     @name HFS maximum length
*/
//@{
#define CYG_HFS_ROOT_DIR_MAXLEN                64   ///<  root dir max path length
#define CYG_HFS_KEY_MAXLEN                     64   ///<  key max length
#define CYG_HFS_NAME_MAXLEN                    32   ///<  user name max length
#define CYG_HFS_PWD_MAXLEN                     32   ///<  user passwd max length
#define CYG_HFS_REQUEST_PATH_MAXLEN            1024 ///<  request url path max length
#define CYG_HFS_FILE_PATH_MAXLEN               256  ///<  file path max length
#define CYG_HFS_MIMETYPE_MAXLEN                40   ///<  mime type max length
#define CYG_HFS_USER_QUERY_MAXLEN              24   ///<  client query string max length
#define CYG_HFS_CUSTOM_STR_MAXLEN              24   // custom string max length
//@}

/**
    HFS put data status.

*/
typedef enum _HFS_PUT_STATUS {
	HFS_PUT_STATUS_CONTINUE               =   0, ///< still have data need to put
	HFS_PUT_STATUS_FINISH                 =   1, ///< put data finish
	HFS_PUT_STATUS_ERR                    =  -1, ///< some error happened
	ENUM_DUMMY4WORD(HFS_PUT_STATUS)
} HFS_PUT_STATUS;


/**
     @name HFS header callback return result
*/
//@{
#define CYG_HFS_CB_HEADER_RETURN_DEFAULT       0    ///<  the http request need to be handle by hfs internal
#define CYG_HFS_CB_HEADER_RETURN_CUSTOM        1    ///<  the http request is handled by customer
#define CYG_HFS_CB_HEADER_RETURN_REDIRECT      2    ///<  the http request need to redirect
#define CYG_HFS_CB_HEADER_RETURN_ERROR        -1    ///<  the http request has some error
//@}

/**
     @name callback function
*/
//@{
typedef void cyg_hfs_notify(int status); ///< Callback function for notify the HFS server status.
typedef int  cyg_hfs_getCustomData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount);///< Callback function for get custom data.
typedef int  cyg_hfs_check_password(const char *username, const char *password, char *key, char *questionstr); ///< Callback function for check password.
typedef int  cyg_hfs_gen_dirlist_html(const char *path, HFS_U32 bufAddr, HFS_U32 *bufSize, const char *usr, const char *pwd, HFS_U32 segmentCount);
typedef int  cyg_hfs_client_query(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount); ///< Callback function for client query
typedef int  cyg_hfs_upload_result_cb(int result, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType);///< Callback function for upload command complete
typedef int  cyg_hfs_delete_result_cb(int result, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType);///< Callback function for delete command complete
typedef int  cyg_hfs_putCustomData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 bufSize, HFS_U32 segmentCount, HFS_PUT_STATUS putStatus);///< Callback function for put custom data.
typedef int  cyg_hfs_header_cb(HFS_U32 headerAddr, HFS_U32 headerSize, char *filepath, char *mimeType, void *reserved);  ///< Callback function for http header parse custom.
typedef void cyg_hfs_download_result_cb(int result, char *path);///< Callback function for download command complete


//@}

/**
    Init parameters for HFS.

    This structure is used in cyg_hfs_open() to specify the HFS needed information for normal working
    and user customization callback.
*/
typedef struct {
	cyg_hfs_notify          *notify;                                     ///<  Callback function for notify the HFS server status
	cyg_hfs_getCustomData   *getCustomData;                              ///<  Callback function for get custom data
	cyg_hfs_putCustomData   *putCustomData;                              ///<  Callback function for put custom data
	cyg_hfs_check_password  *check_pwd;                                  ///<  Callback function for check password.
	cyg_hfs_gen_dirlist_html *genDirListHtml;                            ///<  Callback function for generate directory list with html format
	cyg_hfs_upload_result_cb *uploadResultCb;                            ///<  Callback function for upload command complete
	cyg_hfs_delete_result_cb *deleteResultCb;                            ///<  Callback function for delete command complete
	char                    clientQueryStr[CYG_HFS_USER_QUERY_MAXLEN + 1]; ///<  client query string
	cyg_hfs_client_query    *clientQueryCb;                              ///<  client query callback function
	int                     portNum;                                     ///<  server port number
	int                     threadPriority;                              ///<  server thread priority
	char                    rootdir[CYG_HFS_ROOT_DIR_MAXLEN + 1];        ///<  root of the visible filesystem
	char                    key[CYG_HFS_KEY_MAXLEN + 1];                 ///<  the key used in check password callback function
	int                     sockbufSize;                                 ///<  socket buffer size
	int                     tos;                                         ///<  type of service
	HFS_U32                 sharedMemAddr;                               ///<  shared memory address
	HFS_U32                 sharedMemSize;                               ///<  shared memory size
	HFS_U32                 maxClientNum;                                ///<  maximum client connection at the same time
	int                     timeoutCnt;                                  ///<  timeout counter for send & receive , time base is 0.5 sec
	int                     httpsPortNum;                                ///<  https port number
	char                    customStr[CYG_HFS_CUSTOM_STR_MAXLEN + 1];    ///<  custom string
	int                     forceCustomCallback;                         ///<  force custom callback even url not have "custom=1"
	cyg_hfs_header_cb       *headerCb;                                   ///<  Callback function for header data parse by customer
	cyg_hfs_download_result_cb *downloadResultCb;                        ///<  Callback function for download command complete
} cyg_hfs_open_obj;



/**
    IPC message type.

*/
typedef enum _HFS_IPC_MSG_TYPE {
	HFS_IPC_MSG_TYPE_S2C = 1,         ///<   server to client message
	HFS_IPC_MSG_TYPE_C2S = 2,         ///<   client to server message
	ENUM_DUMMY4WORD(HFS_IPC_MSG_TYPE)
} HFS_IPC_MSG_TYPE;


/**
    IPC message command ID.


*/
typedef enum _HFS_IPC_CMD {
	HFS_IPC_SERVER_STARTED = 0,       ///<   the server is stared               (server-> client)
	HFS_IPC_NOTIFY_CLIENT,            ///<   notify the server status to client (server-> client)
	HFS_IPC_NOTIFY_CLIENT_ACK,        ///<   ack of HFS_IPC_NOTIFY_CLIENT
	HFS_IPC_GET_CUSTOM_DATA,          ///<   get user custom data from client   (server-> client)
	HFS_IPC_GET_CUSTOM_DATA_ACK,      ///<   ack of HFS_IPC_GET_CUSTOM_DATA
	HFS_IPC_CHK_PASSWD,               ///<   check password                     (server-> client)
	HFS_IPC_CHK_PASSWD_ACK,           ///<   ack of HFS_IPC_CHK_PASSWD
	HFS_IPC_GEN_DIRLIST_DATA,         ///<   generate dirlist html data         (server-> client)
	HFS_IPC_GEN_DIRLIST_DATA_ACK,     ///<   ack of HFS_IPC_GEN_DIRLIST_DATA
	HFS_IPC_CLIENT_QUERY_DATA,        ///<   client query request               (server-> client)
	HFS_IPC_CLIENT_QUERY_DATA_ACK,    ///<   ack of HFS_IPC_CLIENT_QUERY_DATA
	HFS_IPC_UPLOAD_RESULT_DATA,       ///<   get upload result page request     (server-> client)
	HFS_IPC_UPLOAD_RESULT_DATA_ACK,   ///<   ack of HFS_IPC_UPLOAD_RESULT_DATA
	HFS_IPC_DELETE_RESULT_DATA,       ///<   get delete result page request     (server-> client)
	HFS_IPC_DELETE_RESULT_DATA_ACK,   ///<   ack of HFS_IPC_DELETE_RESULT_DATA
	HFS_IPC_PUT_CUSTOM_DATA,          ///<   get user custom data from client   (server-> client)
	HFS_IPC_PUT_CUSTOM_DATA_ACK,      ///<   ack of HFS_IPC_GET_CUSTOM_DATA
	HFS_IPC_HEADER_CB,                ///<   http header callback to client   (server-> client)
	HFS_IPC_HEADER_CB_ACK,            ///<   ack of HFS_IPC_HEADER_CB
	HFS_IPC_DOWNLOAD_RESULT_DATA,     ///<   get download result page request     (server-> client)
	HFS_IPC_DOWNLOAD_RESULT_DATA_ACK, ///<   ack of HFS_IPC_DOWNLOAD_RESULT_DATA



	HFS_IPC_CLOSE_SERVER   = 0x80,    ///<   close server                       (client-> server)
	HFS_IPC_CLOSE_SERVER_ACK,         ///<   ack of HFS_IPC_CLOSE_SERVER
	HFS_IPC_CLOSE_FINISH,             ///<   close server handshake finish      (client-> server)
	ENUM_DUMMY4WORD(HFS_IPC_CMD)
} HFS_IPC_CMD;

/**
    General IPC message.


*/
typedef struct _HFS_IPC_MSG {
	long      mtype;                  ///<   HFS_IPC_MSG_TYPE
	HFS_U32   uiIPC;                  ///<   HFS_IPC_CMD
	HFS_U32   shareMem;               ///<   share memory address
} HFS_IPC_MSG;

/**
    Notify IPC message.


*/
typedef struct _HFS_IPC_NOTIFY_MSG {
	long      mtype;                  ///<   HFS_IPC_MSG_TYPE
	HFS_U32   uiIPC;                  ///<   HFS_IPC_CMD
	HFS_U32   notifyStatus;           ///<   the notify status
} HFS_IPC_NOTIFY_MSG;


#define HFS_IPC_MSGSZ          (sizeof(HFS_IPC_MSG))

/**
    Get custom IPC message.


*/
typedef struct _HFS_IPC_GET_CUSTOM_S {
	HFS_U32   bufAddr;                                      ///<   buffer to put custom data
	HFS_U32   bufSize;                                      ///<   buffer size
	HFS_U32   segmentCount;                                 ///<   the buffer may be not enough to put custom data on time, so need to separate to several segments.
	char      mimeType[CYG_HFS_MIMETYPE_MAXLEN + 1];        ///<   the mimeType of the custom data
	char      path[CYG_HFS_REQUEST_PATH_MAXLEN + 1];        ///<   the url path not inlcude the arguments after the question mark.
	char      argument[CYG_HFS_REQUEST_PATH_MAXLEN + 1];    ///<   the argument passed in the url after the question mark.
	int       returnStatus;                                 ///<   the return value, return CYG_HFS_CB_GETDATA_RETURN_OK means get data ok, CYG_HFS_CB_GETDATA_RETURN_ERROR means get data fail.
} HFS_IPC_GET_CUSTOM_S;

/**
    Check password IPC message.


*/
typedef struct _HFS_IPC_CHK_PASSWD_S {
	char      username[CYG_HFS_NAME_MAXLEN + 1];            ///<   user name
	char      password[CYG_HFS_PWD_MAXLEN + 1];             ///<   password
	char      key[CYG_HFS_KEY_MAXLEN + 1];                  ///<   the key that setted in cyg_hfs_open_obj when start hfs.
	char      questionstr[CYG_HFS_REQUEST_PATH_MAXLEN + 1]; ///<   the argument passed in the url after the question mark.
	int       returnStatus;                                 ///<   the return value, return 1 means check password pass, else means fail.
} HFS_IPC_CHK_PASSWD_S;

/**
    Generate dirlist IPC message.


*/
typedef struct _HFS_IPC_GEN_DIRLIST_S {
	HFS_U32   bufAddr;                                      ///<   buffer to put dirlist data
	HFS_U32   bufSize;                                      ///<   buffer size
	HFS_U32   segmentCount;                                 ///<   the buffer may be not enough to put custom data on time, so need to separate to several segments.
	char      usr[CYG_HFS_NAME_MAXLEN + 1];                 ///<   user name
	char      pwd[CYG_HFS_PWD_MAXLEN + 1];                  ///<   password
	char      path[CYG_HFS_REQUEST_PATH_MAXLEN + 1];        ///<   the url path not inlcude the arguments after the question mark.
	int       returnStatus;                                 ///<   the return value, return CYG_HFS_LIST_DIR_OK means get data ok, CYG_HFS_LIST_DIR_OPEN_FAIL means get data fail.
} HFS_IPC_GEN_DIRLIST_S;


/**
    Client query IPC message.


*/
typedef struct _HFS_IPC_CLIENT_QUERY_S {
	HFS_U32   bufAddr;                                      ///<   buffer to put data
	HFS_U32   bufSize;                                      ///<   buffer size
	HFS_U32   segmentCount;                                 ///<   the buffer may be not enough to put custom data on time, so need to separate to several segments.
	char      mimeType[CYG_HFS_MIMETYPE_MAXLEN + 1];        ///<   the mimeType of the data
	char      path[CYG_HFS_REQUEST_PATH_MAXLEN + 1];        ///<   the url path not inlcude the arguments after the question mark.
	char      argument[CYG_HFS_REQUEST_PATH_MAXLEN + 1];    ///<   the argument passed in the url after the question mark.
	int       returnStatus;                                 ///<   the return value, return CYG_HFS_CB_GETDATA_RETURN_OK means get data ok, CYG_HFS_CB_GETDATA_RETURN_ERROR means get data fail.
} HFS_IPC_CLIENT_QUERY_S;

/**
    Upload result IPC message.


*/
typedef struct _HFS_IPC_UPLOAD_RESULT_S {
	HFS_U32   bufAddr;                                      ///<   buffer to put upload result page data
	HFS_U32   bufSize;                                      ///<   buffer size
	int       uploadResult;                                 ///<   upload result, CYG_HFS_UPLOAD_OK means upload ok else means has some error.
	char      mimeType[CYG_HFS_MIMETYPE_MAXLEN + 1];        ///<   the mimeType of the upload result page data
	int       returnStatus;                                 ///<   the return value, return CYG_HFS_CB_GETDATA_RETURN_OK means get data ok, CYG_HFS_CB_GETDATA_RETURN_ERROR means get data fail.
} HFS_IPC_UPLOAD_RESULT_S;

/**
    Download result IPC message.


*/
typedef struct _HFS_IPC_DOWNLOAD_RESULT_S {
	int       downloadResult;                               ///<   download result, CYG_HFS_DOWNLOAD_OK means upload ok else means has some error.
	char      path[CYG_HFS_REQUEST_PATH_MAXLEN + 1];        ///<   the file path that want to download
} HFS_IPC_DOWNLOAD_RESULT_S;

/**
    Delete result IPC message.


*/
typedef struct _HFS_IPC_DELETE_RESULT_S {
	HFS_U32   bufAddr;                                      ///<   buffer to put delete result page data
	HFS_U32   bufSize;                                      ///<   buffer size
	int       deleteResult;                                 ///<   delete result, CYG_HFS_DELETE_OK means upload ok else means has some error.
	char      mimeType[CYG_HFS_MIMETYPE_MAXLEN + 1];        ///<   the mimeType of the delete result page data
	int       returnStatus;                                 ///<   the return value, return CYG_HFS_CB_GETDATA_RETURN_OK means get data ok, CYG_HFS_CB_GETDATA_RETURN_ERROR means get data fail.
} HFS_IPC_DELETE_RESULT_S;

/**
    Put custom IPC message.

*/
typedef struct _HFS_IPC_PUT_CUSTOM_S {
	HFS_U32         bufAddr;                                      ///<   buffer to put custom data
	HFS_U32         bufSize;                                      ///<   buffer size
	HFS_U32         segmentCount;                                 ///<   the buffer may be not enough to put custom data on time, so need to separate to several segments.
	char            path[CYG_HFS_REQUEST_PATH_MAXLEN + 1];        ///<   the url path not inlcude the arguments after the question mark.
	char            argument[CYG_HFS_REQUEST_PATH_MAXLEN + 1];    ///<   the argument passed in the url after the question mark.
	HFS_PUT_STATUS  putStatus;                                    ///<   HFS put data status
	int             returnStatus;                                 ///<   the return value, return CYG_HFS_CB_GETDATA_RETURN_OK means get data ok, CYG_HFS_CB_GETDATA_RETURN_ERROR means get data fail.
} HFS_IPC_PUT_CUSTOM_S;


/**
    HTTP Header IPC message.

*/

typedef struct _HFS_IPC_HEADER_S {
	HFS_U32         headerAddr;                                   ///<   buffer to put custom data
	HFS_U32         headerSize;                                   ///<   buffer size
	char            filepath[CYG_HFS_FILE_PATH_MAXLEN + 1];       ///<   the filepath that want to get or set.
	char            mimeType[CYG_HFS_MIMETYPE_MAXLEN + 1];        ///<   the mimeType of get file
	void           *reserved;                                     ///<   reserved parameter
	int             returnStatus;                                 ///<   the return value, return CYG_HFS_CB_HEADER_RETURN_DEFAULT means need to be handle by hfs internal.
} HFS_IPC_HEADER_S;



#define HFS_IPC_TOKEN_PATH    "hfs"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((HFS_U32)(HFS_U8)(ch0) | ((HFS_U32)(HFS_U8)(ch1) << 8) | ((HFS_U32)(HFS_U8)(ch2) << 16) | ((HFS_U32)(HFS_U8)(ch3) << 24 ))
#endif
#define HFS_IPCKEYID   MAKEFOURCC('H','F','S','K')

#ifdef __cplusplus
extern "C"
{
#endif
/**
    Start hfs server.

    @param[in] pObj:  Init info for Hfs.

*/
extern void cyg_hfs_open(cyg_hfs_open_obj  *pObj);


/**
    Start hfs server by command.

    @param[in] cmd:  Init command for Hfs.
      -put         put custom callback
      -p           port number
      -to          timeout count
      -t           thread priority
      -r           root path
      -s           socket buffer size
      -ma          shared memory address
      -ms          shared memory size
      -w           check password callback
      -c           get custom callback
      -g           generate dirlist callback
      -n           maximum client number
      -k           the key passed to check password callback
      -q           client query callback
      -u           upload result callback
      -dl          download result callback
      -d           delete result callback
      -f           force custom callback
      -h           header callback

*/
extern void cyg_hfs_open2(char *cmd);

/**
    Close hfs server.

*/
extern void cyg_hfs_close(void);

#ifdef __cplusplus
} //extern "C"
#endif

/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_HFS_H                                          */
/* end of hfs.h*/

