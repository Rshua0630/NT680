#ifndef CYGONCE_NET_LVIEW_LVIEWD_H
#define CYGONCE_NET_LVIEW_LVIEWD_H
/* =================================================================
 *
 *      lviewd.h
 *
 *      A simple live view streaming server
 *
 * =================================================================
 */
#define LVIEWD_INTERFACE_VER 0x17112018


#include <stdio.h>

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef unsigned long       LVIEWD_U32;               ///<   Unsigned 32 bits data type
typedef unsigned char       LVIEWD_U8;                ///<   Unsigned 8 bits data type

/**
     @name lviewd notify status
*/
//@{
#define CYG_LVIEW_STATUS_CLIENT_REQUEST           0   ///<   client has request comimg in
#define CYG_LVIEW_STATUS_SERVER_RESPONSE_START    1   ///<   server send response live view data start
#define CYG_LVIEW_STATUS_SERVER_RESPONSE_END      2   ///<   server send response live view data end
#define CYG_LVIEW_STATUS_CLIENT_DISCONNECT        3   ///<   client disconnect.
//@}

/**
     @name lviewd callback function
*/
//@{
typedef int cyg_lviewd_getJpg(int *jpgAddr, int *jpgSize); ///< Callback function for get one jpg frame.
typedef void cyg_lviewd_notify(int status);                ///< Callback function for notify the lviewd status.
typedef void cyg_lviewd_hwmem_memcpy(LVIEWD_U32 uiDst, LVIEWD_U32 uiSrc, LVIEWD_U32 uiSize);///< Callback function for doing HW memcpy.
//@}

typedef struct{
    cyg_lviewd_getJpg       *getJpg;        ///<   get jpg function handler
    cyg_lviewd_notify       *notify;        ///<   notify the status
    cyg_lviewd_hwmem_memcpy *hwmem_memcpy;  ///<   Hw memcpy API
    int                     portNum;        ///<   server port number
    int                     threadPriority; ///<   server thread priority
    int                     maxJpgSize;     ///<   support max jpg size
    int                     frameRate;      ///<   live view streaming frame rate, default is 30
    int                     sockbufSize;    ///<   socket buffer size
    int                     is_ssl;         ///<   is support ssl
    unsigned int            shareMemAddr;   ///<   the share memory address for put jpg frames
    unsigned int            shareMemSize;   ///<   the share memory size for put jpg frames
    int                     timeoutCnt;     ///<   timeout counter for send & receive , time base is 0.5 sec
    int                     tos;            ///<   type of service
    int                     is_push_mode;   ///<   is push mode
    void                   *arg;            ///<   reserved for future use
} cyg_lviewd_install_obj;


typedef struct{
    unsigned int            addr;           ///<   frame address
    unsigned int            size;           ///<   frame size
} cyg_lviewd_frame_info;

/**
    IPC message type.

*/
typedef enum _LVIEWD_IPC_MSG_TYPE {
	LVIEWD_IPC_MSG_TYPE_S2C = 1,            ///<   server to client message
	LVIEWD_IPC_MSG_TYPE_C2S = 2,            ///<   client to server message
} LVIEWD_IPC_MSG_TYPE;


/**
    IPC message command ID.

*/
typedef enum _LVIEWD_IPC_CMD {
	LVIEWD_IPC_SERVER_STARTED = 0,          ///<   the server is stared               (server-> client)
	LVIEWD_IPC_NOTIFY_CLIENT,               ///<   notify the server status to client (server-> client)
	LVIEWD_IPC_NOTIFY_CLIENT_ACK,           ///<   ack of LVIEWD_IPC_NOTIFY_CLIENT
	LVIEWD_IPC_GET_JPG,                     ///<   get user jpg data from client      (server-> client)
	LVIEWD_IPC_GET_JPG_ACK,                 ///<   ack of LVIEWD_IPC_GET_JPG
	LVIEWD_IPC_PUSH_FRAME,                  ///<   push frame to server               (client-> server)

	LVIEWD_IPC_CLOSE_SERVER = 0x80,         ///<   close server                       (client-> server)
	LVIEWD_IPC_CLOSE_SERVER_ACK,            ///<   ack of LVIEWD_IPC_CLOSE_SERVER
	LVIEWD_IPC_CLOSE_FINISH,                ///<   close server handshake finish      (client-> server)
} LVIEWD_IPC_CMD;

/**
    General IPC message.

*/
typedef struct _LVIEWD_IPC_MSG {
	long         mtype;                     ///<   LVIEWD_IPC_MSG_TYPE
	LVIEWD_U32   uiIPC;                     ///<   LVIEWD_IPC_CMD
	LVIEWD_U32   parm1;                     ///<   parameter 1
	LVIEWD_U32   parm2;                     ///<   parameter 2
} LVIEWD_IPC_MSG;

/**
    Notify IPC message.

*/
typedef struct _LVIEWD_IPC_NOTIFY_MSG {
	long         mtype;                     ///<   LVIEWD_IPC_MSG_TYPE
	LVIEWD_U32   uiIPC;                     ///<   LVIEWD_IPC_CMD
	LVIEWD_U32   notifyStatus;              ///<   the notify status, ex: CYG_LVIEW_STATUS_CLIENT_REQUEST
} LVIEWD_IPC_NOTIFY_MSG;

/**
    Get JPG IPC message.

*/
typedef struct _LVIEWD_IPC_GETJPG_MSG {
	long         mtype;                     ///<   LVIEWD_IPC_MSG_TYPE
	LVIEWD_U32   uiIPC;                     ///<   LVIEWD_IPC_CMD
	LVIEWD_U32   jpgAddr;                   ///<   the jpg Addr
	LVIEWD_U32   jpgSize;                   ///<   the jpg Size
} LVIEWD_IPC_GETJPG_MSG;

/**
    Push frame IPC message.

*/
typedef struct _LVIEWD_IPC_PUSH_FRAME_MSG{
    long         mtype;                     ///<   LVIEWD_IPC_MSG_TYPE
    LVIEWD_U32   uiIPC;                     ///<   LVIEWD_IPC_CMD
    LVIEWD_U32   addr;                      ///<   the frame Addr
    LVIEWD_U32   size;                     ///<   the frame Size
}LVIEWD_IPC_PUSH_FRAME_MSG;



#define LVIEWD_IPC_MSGSZ          (sizeof(LVIEWD_IPC_MSG))

#define LVIEWD_IPC_TOKEN_PATH    "lviewd"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((LVIEWD_U32)(LVIEWD_U8)(ch0) | ((LVIEWD_U32)(LVIEWD_U8)(ch1) << 8) | ((LVIEWD_U32)(LVIEWD_U8)(ch2) << 16) | ((LVIEWD_U32)(LVIEWD_U8)(ch3) << 24 ))
#endif

#define LVIEWD_IPCKEYID   MAKEFOURCC('L','V','I','K')

#ifdef __cplusplus
extern "C"
{
#endif

/**
    Install some callback function & config some settings.

    @param[in] pObj:  Init info for lviewd.

*/
extern void cyg_lviewd_install(cyg_lviewd_install_obj  *pObj);

/**
    Start lview daemon.

*/
extern void cyg_lviewd_startup(void);

/**
    Start lview daemon by command string.

    @param[in] cmd:  Init command for lviewd.
      -p           port number
      -t           thread priority
      -f           frame rate
      -s           socket buffer size
      -j           maximum jpg size
      -ssl         is enable ssl
      -pm          is push mode

*/
extern void  cyg_lviewd_startup2(char *cmd);

/**
    Stop lview daemon.

*/
extern void cyg_lviewd_stop(void);

/**
    Push jpeg frame to lview daemon.

    @param[in] frame_info:  The jpeg frame information.

*/
extern void cyg_lviewd_push_frame(cyg_lviewd_frame_info  *frame_info);



#ifdef __cplusplus
} //extern "C"
#endif

/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_LVIEW_LVIEWD_H                                  */
/* end of lviewd.h                                                   */

