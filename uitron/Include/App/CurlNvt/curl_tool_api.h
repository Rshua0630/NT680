/////////////////////////////////////////////////////////////////
/*
    Copyright (c) 2014~  Novatek Microelectronics Corporation

    @file curl_tool_api.h

    @version

    @date

*//////////////////////////////////////////////////////////////////
#ifndef HEADER_CURL_TOOL_AP_H
#define HEADER_CURL_TOOL_AP_H
#define CURL_INTERFACE_VER 0x16051315


//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef unsigned long       CURL_U32;                ///<  Unsigned 32 bits data type
typedef unsigned char       CURL_U8;                 ///<  Unsigned 8 bits data type
typedef signed long         CURL_I32;                ///<  Signed 32 bits data type

#define CURL_IPC_TOKEN_PATH    "curl"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((CURL_U32)(CURL_U8)(ch0) | ((CURL_U32)(CURL_U8)(ch1) << 8) | ((CURL_U32)(CURL_U8)(ch2) << 16) | ((CURL_U32)(CURL_U8)(ch3) << 24 ))
#endif
#define CURL_IPCKEYID   MAKEFOURCC('C','U','R','L')

#define CURL_CMD_SIZE                512
#define CURL_CLIENTS_MAX             4
#define CURL_IPC_RW_BUFSIZE          16384
#define CURL_IPC_CMD_BUFSIZE         1024


#ifndef ENUM_DUMMY4WORD
/**
    Macro to generate dummy element for enum type to expand enum size to word (4 bytes)
*/
//@{
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
//@}
#endif

/**
   Error Code.

   Any error code occurred will display on uart or return.
*/
typedef enum _CURL_ER {
	CURL_ER_OK                      =   0,  ///< no error
	CURL_ER_UNSUPPORTED_PROTOCOL    =   1,    /* 1 */
	CURL_ER_FAILED_INIT,             /* 2 */
	CURL_ER_URL_MALFORMAT,           /* 3 */
	CURL_ER_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
                                     7.17.0, reused in April 2011 for 7.21.5] */
	CURL_ER_COULDNT_RESOLVE_PROXY,   /* 5 */
	CURL_ER_COULDNT_RESOLVE_HOST,    /* 6 */
	CURL_ER_COULDNT_CONNECT,         /* 7 */
	CURL_ER_FTP_WEIRD_SERVER_REPLY,  /* 8 */
	CURL_ER_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
                                     due to lack of access - when login fails
                                     this is not returned. */
	CURL_ER_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
                                     7.15.4, reused in Dec 2011 for 7.24.0]*/
	CURL_ER_FTP_WEIRD_PASS_REPLY,    /* 11 */
	CURL_ER_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
                                     [was obsoleted in August 2007 for 7.17.0,
                                     reused in Dec 2011 for 7.24.0]*/
	CURL_ER_FTP_WEIRD_PASV_REPLY,    /* 13 */
	CURL_ER_FTP_WEIRD_227_FORMAT,    /* 14 */
	CURL_ER_FTP_CANT_GET_HOST,       /* 15 */
	CURL_ER_HTTP2,                   /* 16 - A problem in the http2 framing layer.
                                     [was obsoleted in August 2007 for 7.17.0,
                                     reused in July 2014 for 7.38.0] */
	CURL_ER_FTP_COULDNT_SET_TYPE,    /* 17 */
	CURL_ER_PARTIAL_FILE,            /* 18 */
	CURL_ER_FTP_COULDNT_RETR_FILE,   /* 19 */
	CURL_ER_OBSOLETE20,              /* 20 - NOT USED */
	CURL_ER_QUOTE_ERROR,             /* 21 - quote command failure */
	CURL_ER_HTTP_RETURNED_ERROR,     /* 22 */
	CURL_ER_WRITE_ERROR,             /* 23 */
	CURL_ER_OBSOLETE24,              /* 24 - NOT USED */
	CURL_ER_UPLOAD_FAILED,           /* 25 - failed upload "command" */
	CURL_ER_READ_ERROR,              /* 26 - couldn't open/read from file */
	CURL_ER_OUT_OF_MEMORY,           /* 27 */
	/* N_oRte: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
	        instead of a memory allocation error if CURL_DOES_CONVERSIONS
	        is defined
	*/
	CURL_ER_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
	CURL_ER_OBSOLETE29,              /* 29 - NOT USED */
	CURL_ER_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
	CURL_ER_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
	CURL_ER_OBSOLETE32,              /* 32 - NOT USED */
	CURL_ER_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
	CURL_ER_HTTP_POST_ERROR,         /* 34 */
	CURL_ER_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
	CURL_ER_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
	CURL_ER_FILE_COULDNT_READ_FILE,  /* 37 */
	CURL_ER_LDAP_CANNOT_BIND,        /* 38 */
	CURL_ER_LDAP_SEARCH_FAILED,      /* 39 */
	CURL_ER_OBSOLETE40,              /* 40 - NOT USED */
	CURL_ER_FUNCTION_NOT_FOUND,      /* 41 */
	CURL_ER_ABORTED_BY_CALLBACK,     /* 42 */
	CURL_ER_BAD_FUNCTION_ARGUMENT,   /* 43 */
	CURL_ER_OBSOLETE44,              /* 44 - NOT USED */
	CURL_ER_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
	CURL_ER_OBSOLETE46,              /* 46 - NOT USED */
	CURL_ER_TOO_MANY_REDIRECTS,      /* 47 - catch endless re-direct loops */
	CURL_ER_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
	CURL_ER_TELNET_OPTION_SYNTAX,    /* 49 - Malformed telnet option */
	CURL_ER_OBSOLETE50,              /* 50 - NOT USED */
	CURL_ER_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
                                    wasn't verified fine */
	CURL_ER_GOT_NOTHING,             /* 52 - when this is a specific error */
	CURL_ER_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
	CURL_ER_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
                                   default */
	CURL_ER_SEND_ERROR,              /* 55 - failed sending network data */
	CURL_ER_RECV_ERROR,              /* 56 - failure in receiving network data */
	CURL_ER_OBSOLETE57,              /* 57 - NOT IN USE */
	CURL_ER_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
	CURL_ER_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
	CURL_ER_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
	CURL_ER_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
	CURL_ER_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
	CURL_ER_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
	CURL_ER_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
	CURL_ER_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
                                   that failed */
	CURL_ER_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
	CURL_ER_LOGIN_DENIED,            /* 67 - user, password or similar was not
                                   accepted and we failed to login */
	CURL_ER_TFTP_NOTFOUND,           /* 68 - file not found on server */
	CURL_ER_TFTP_PERM,               /* 69 - permission problem on server */
	CURL_ER_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
	CURL_ER_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
	CURL_ER_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
	CURL_ER_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
	CURL_ER_TFTP_NOSUCHUSER,         /* 74 - No such user */
	CURL_ER_CONV_FAILED,             /* 75 - conversion failed */
	CURL_ER_CONV_REQD,               /* 76 - caller must register conversion
                                     callbacks using curl_easy_setopt options
                                     CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                                     CURLOPT_CONV_TO_NETWORK_FUNCTION, and
                                     CURLOPT_CONV_FROM_UTF8_FUNCTION */
	CURL_ER_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
                                     or wrong format */
	CURL_ER_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
	CURL_ER_SSH,                     /* 79 - error from the SSH layer, somewhat
                                   generic so the error message will be of
                                   interest when this has happened */

	CURL_ER_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
                                      connection */
	CURL_ER_AGAIN,                   /* 81 - socket is not ready for send/recv,
                                     wait till it's ready and try again (Added
                                     in 7.18.2) */
	CURL_ER_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
                                     wrong format (Added in 7.19.0) */
	CURL_ER_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
                                     7.19.0) */
	CURL_ER_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
	CURL_ER_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
	CURL_ER_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
	CURL_ER_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
	CURL_ER_CHUNK_FAILED,            /* 88 - chunk callback reported error */
	CURL_ER_NO_CONNECTION_AVAILABLE, /* 89 - No connection available, the
                                     session will be queued */
	CURL_ER_SSL_PINNEDPUBKEYNOTMATCH, /* 90 - specified pinned public key did not
                                     match */
	CURL_ER_SSL_INVALIDCERTSTATUS,   /* 91 - invalid certificate status */


	CURL_ER_NOT_OPENED,              /* 92 ///< the curl is not opened  */
	CURL_ER_EXCEED_MAX_CLIENTS,      /* 93 ///< exceed max clients limit */
	CURL_ER_PARM,                    /* 94 ///< has some error of input parameter */
	CURL_ER_ID_NOT_INSTALLED,        /* 95 ///< the Curl used flag, semaphore ID is not installed */
	CURL_ER_SYS,                     /* 96 ///< the Curl has some system error */

	CURL_ER_LAST,                    /* never use! */
	ENUM_DUMMY4WORD(CURL_ER)
} CURL_ER;



/**
     @name callback function
*/
//@{
typedef void   curl_cmd_finish_cb(CURL_ER cmdSts, void *userdata);                       ///< Callback function for notify the curl command finish.

/* This is a return code for the read callback that, when returned, will
   signal libcurl to immediately abort the current transfer. */
#define CURL_READFUNC_ABORT 0x10000000
/* This is a return code for the read callback that, when returned, will
   signal libcurl to pause sending data on the current transfer. */
#define CURL_READFUNC_PAUSE 0x10000001
typedef size_t curl_cmd_read_cb(void  *buffer, size_t sz, size_t nmemb, void *userdata); ///< Callback function for content data read.
typedef size_t curl_cmd_write_cb(void *buffer, size_t sz, size_t nmemb, void *userdata); ///< Callback function for content data write.
typedef size_t curl_cmd_header_cb(void *buffer, size_t sz, size_t nmemb, void *userdata);///< Callback function for header data write.

//@}

/**
    Parameters for execute curl command.

    This structure is used in curl_cmd() to specify the curl command normal working
    and user customization callback.
*/
typedef struct {
	char                    *strCmd;                                  ///<  the curl command to run
	int                     isWaitFinish;                             ///<  wait curl command finish then exit the API, set true means sync mode, else means async mode.
	curl_cmd_finish_cb      *cmdFinishCB;                             ///<  command finish callback function
	curl_cmd_read_cb        *cmdReadCB;                               ///<  content data read callback function
	curl_cmd_write_cb       *cmdWriteCB;                              ///<  content data write callback function
	curl_cmd_header_cb      *cmdHeaderCB;                             ///<  header data write callback function
	void                    *userdata;                                ///<  user data to pass to callback function
} curl_cmd_s;



/**
    IPC message type.

*/
typedef enum _CURL_IPC_MSG_TYPE {
	CURL_IPC_MSG_TYPE_S2C = 1,         ///<   server to client message
	CURL_IPC_MSG_TYPE_C2S = 2,         ///<   client to server message
} CURL_IPC_MSG_TYPE;

/**
    IPC message command ID.


*/
typedef enum {

	CURL_IPC_CMDID_STARTED,           ///<   (cpu2-> cpu1)
	CURL_IPC_CMDID_RUNCMD,
	CURL_IPC_CMDID_RUNCMD_ACK,
	CURL_IPC_CMDID_RUNCMD_FINISH_CB,
	CURL_IPC_CMDID_RUNCMD_FINISH_CB_ACK,

	CURL_IPC_CMDID_READ_CB,           ///<   read callback  (cpu2-> cpu1)
	CURL_IPC_CMDID_READ_CB_ACK,       ///<
	CURL_IPC_CMDID_WRITE_CB,          ///<   write callback  (cpu2-> cpu1)
	CURL_IPC_CMDID_WRITE_CB_ACK,      ///<
	CURL_IPC_CMDID_HEADER_CB,         ///<   header callback  (cpu2-> cpu1)
	CURL_IPC_CMDID_HEADER_CB_ACK,     ///<




	CURL_IPC_CMDID_CLOSE = 0x80,      ///<   close    (cpu1-> cpu2)
	CURL_IPC_CMDID_CLOSE_ACK,         ///<   ack of CURL_IPC_CMDID_CLOSE
	CURL_IPC_CMDID_CLOSE_FINISH,


} CURL_IPC_CMD;

/**
    General IPC message.
*/
typedef struct _CURL_IPC_MSG {
	long       mtype;                  ///<   CURL_IPC_MSG_TYPE
	CURL_U32   CmdId;                  ///<   CURL_IPC_CMD
	CURL_U32   ShareMem;               ///<   share memory address
} CURL_IPC_MSG;


typedef struct {
	char                 strCmd[CURL_CMD_SIZE];      ///< [in]  the curl command to run
	int                  isWaitFinish;               ///< [in]  wait curl command finish then exit the API, set true means sync mode, else means async mode.
	curl_cmd_finish_cb   *cmdFinishCB;               ///< [in]  command finish callback function
	curl_cmd_read_cb     *cmdReadCB;                 ///< [in]  data read callback function
	curl_cmd_write_cb    *cmdWriteCB;                ///< [in]  data write callback function
	curl_cmd_header_cb   *cmdHeaderCB;               ///< [in]  header data write callback function
	void                 *userdata;                  ///< [in]  user data to pass to callback function
	CURL_ER               rtnSts;                    ///< [out] the command return status
} CURL_RUNCMD_S;

/*
typedef struct {
    CURL_ER       cmdSts;                     ///< the status of run command
    CURL_U32      sessionHdl;                 ///< the curl session handle
}CURL_RUNCMD_ACK_S;
*/

typedef struct {
	CURL_ER       cmdSts;                     ///< [in] the status of run command
	void          *userdata;                  ///< [in] the user data
	CURL_ER        rtnSts;                    ///< [out] the command return status
} CURL_RUNCMD_FINISH_CB_S;

typedef struct {
	CURL_U8       buffer[CURL_IPC_RW_BUFSIZE];                ///< [in]
	int           sz;                     ///< [in]
	int           nmemb;                  ///< [in]
	void          *userdata;              ///< [in]
	int           rtn;                    ///< [out]
} CURL_READ_CB_S;


typedef struct {
	CURL_U8       buffer[CURL_IPC_RW_BUFSIZE];                ///< [in]
	int           sz;                     ///< [in]
	int           nmemb;                  ///< [in]
	void          *userdata;              ///< [in]
	int           rtn;                    ///< [out]
} CURL_WRITE_CB_S;


typedef struct {
	CURL_U8       buffer[CURL_IPC_RW_BUFSIZE];                ///< [in]
	int           sz;                     ///< [in]
	int           nmemb;                  ///< [in]
	void          *userdata;              ///< [in]
	int           rtn;                    ///< [out]
} CURL_HEADER_CB_S;

#define CURL_IPC_MSGSZ          (sizeof(CURL_IPC_MSG))


#ifdef __cplusplus
extern "C"
{
#endif
/**
    Start curl.

    @param[in] cmd:  Init command for curl.
      -s         maximum client session number


*/
extern CURL_ER curl_open(char *cmd);

extern CURL_ER curl_close(void);

extern CURL_ER curl_cmd(curl_cmd_s *curlcmd);


#ifdef __cplusplus
} //extern "C"
#endif

#endif /* HEADER_CURL_TOOL_AP_H */

