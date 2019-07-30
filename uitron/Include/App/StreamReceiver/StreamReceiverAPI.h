/**
    User socket export variables and function prototypes.

    StreamReceiver is simple socket API.
    Through this API user can send and receive network data.

    @file       StreamReceiverAPI.h
    @ingroup    mIStreamReceiverAPI

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _STREAMRECEIVER_H
#define _STREAMRECEIVER_H

#include "Type.h"

/**
    @name STREAMRECEIVER error code.
*/
//@{
#define STREAMRECEIVER_RET_OK           0             ///<command OK
#define STREAMRECEIVER_RET_ERR          (-1)          ///<command fail
#define STREAMRECEIVER_RET_NO_FUNC      (-2)          ///<no operation
#define STREAMRECEIVER_RET_PAR_ERR      (-3)          ///<parameter error
#define STREAMRECEIVER_RET_QUE_FULL     (-5)          ///<queue full
//@}

#if 0
/**
    The prototype of StreamReceiver command callback
*/
typedef int StreamReceiver_cmd(char *addr);
#endif
#define STREAMRECEIVER_BUF_CHK_SIZE        (4)                            ///<the size of check buffer tag
#define STREAMRECEIVER_BUF_TAG              MAKEFOURCC('A','S','T','M')   ///<check tag for share memory
#define STREAMRECEIVER_CMD_SIZE           (256)                          ///<maximum command/notify size
#define STREAMRECEIVER_CFG_SIZE           (4096)                         ///<maximum config size


/**
    STREAMRECEIVER open structure.
    @note   shared memory should be non-cached memory
*/
typedef struct {
	UINT32                  sharedMemAddr;  // shared memory address
	UINT32                  sharedMemSize;  // shared memory size
} STREAMRECEIVER_OPEN;

typedef struct{
    UINT32 CodecType;         ///< video codec type
    UINT32 FrameRate;         ///< video frame rate
    UINT32 Width;             ///< video width
    UINT32 Height;            ///< video height
} STREAMRECEIVER_INFO;

/**
    The prototype of data receive callback
*/
typedef INT32 STREAMRECEIVER_DATA_CB(UINT32 did, UINT32 addr, UINT32 *ptrsize);
typedef INT32 STREAMRECEIVER_VINFO_CB(UINT32 did, STREAMRECEIVER_INFO *pVidInfo);


/**
    Install StreamReceiver flag and semaphore id.

    @return void.
*/
extern void StreamReceiver_InstallID(void) _SECTION(".kercfg_text");

/**
    Open StreamReceiver message queue and task.

    @param[in] pOpen  the share memory for IPC

    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_Open(STREAMRECEIVER_OPEN *pOpen);

/**
    Close StreamReceiver message queue and task.


    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_Close(void);

/**
    Register audio data receive callback.
    The callback would recevie Linux nvastreamreceiver data.

    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_RegAStreamCB(STREAMRECEIVER_DATA_CB *pCB);

/**
    Register video data receive callback.
    The callback would recevie Linux nvastreamreceiver data.

    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_RegVStreamCB(STREAMRECEIVER_DATA_CB *pCB);

/**
    Register video data receive callback.
    The callback would recevie Linux nvastreamreceiver data.

    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_RegVInfoCB(STREAMRECEIVER_VINFO_CB *pCB);


#if 0
/**
    Notify uItron status to Linux nvastreamreceiver.

    @note   the max cmd is STREAMRECEIVER_CMD_SIZE

    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_Notify(char *cmd);

/**
    Register string command callback.
    The callback would recevie Linux nvastreamreceiver command and return callback result to Linux.

    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_RegCmdCB(StreamReceiver_cmd *pCB);

/**
    Set return string to Linux nvastreamreceiver.This API should be called in command callback function.
    While command callback finish,callback would return result and string.

    @note : maximum string legth is STREAMRECEIVER_CMD_SIZE

    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_SetRetString(char *String);

/**
    Get config data from nvastreamreceiver.


    @param[in] configData config data buffer pointer.
    @param[in] dataSize config data size.
    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_GetConfigData(void *configData, int dataSize);

/**
    Set config data from nvastreamreceiver.


    @param[in] configData config data buffer pointer.
    @param[in] dataSize config data size.
    @return
        - @b STREAMRECEIVER_RET_OK if success
        - @b STREAMRECEIVER_RET_ERR if fail
*/
extern ER   StreamReceiver_SetConfigData(void *configData, int dataSize);

#endif

#endif /* _STREAMRECEIVER_H  */
