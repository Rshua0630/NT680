/**
    User socket export variables and function prototypes.

    NvtUctrl is simple socket API.
    Through this API user can send and receive network data.

    @file       NvtUctrlAPI.h
    @ingroup    mINvtUctrlAPI

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _NVTUCTRL_H
#define _NVTUCTRL_H

#include "Type.h"

/**
    @name NVTUCTRL error code.
*/
//@{
#define NVTUCTRL_RET_OK           0             ///<command OK
#define NVTUCTRL_RET_ERR          (-1)          ///<command fail
#define NVTUCTRL_RET_NO_FUNC      (-2)          ///<no operation
#define NVTUCTRL_RET_PAR_ERR      (-3)          ///<parameter error
//@}


/**
    The prototype of NvtUctrl command callback
*/
typedef int NvtUctrl_cmd(char *addr);

#define NVT_UCTRL_CMD_SIZE           (256)                          ///<maximum command/notify size
#define NVT_UCTRL_CFG_SIZE           (40960)                        ///<maximum config size
#define NVT_UCTRL_RAW_SIZE           (32*1024)                        ///<maximum raw size
#define NVTUCTRL_PARAM_BUF_SIZE      (1024+NVT_UCTRL_CFG_SIZE)      ///<shared memory size
#define NVTUCTRL_BUF_CHK_SIZE        (4)                            ///<the size of check buffer tag
#define NVTUCTRL_BUF_TAG              MAKEFOURCC('U','T','R','L')   ///<check tag for share memory

/**
    NVTUCTRL open structure.
    @note   shared memory should be non-cached memory
*/
typedef struct {
	UINT32                  sharedMemAddr;  // shared memory address
	UINT32                  sharedMemSize;  // shared memory size
} NVTUCTRL_OPEN;


/**
    Install NvtUctrl flag and semaphore id.

    @return void.
*/
extern void NvtUctrl_InstallID(void) _SECTION(".kercfg_text");

/**
    Open NvtUctrl message queue and task.

    @param[in] pOpen  the share memory for IPC

    @return
        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_Open(NVTUCTRL_OPEN *pOpen);

/**
    Close NvtUctrl message queue and task.


    @return
        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_Close(void);

/**
    Notify uItron status to Linux nvt_uctrl.

    @note   the max cmd is NVT_UCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_Notify(char *cmd);

/**
    Notify uItron raw data to Linux nvt_uctrl.

    @param[in] rawData raw data buffer pointer.
    @param[in] dataSize raw data size.
    @return

        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_NotifyRaw(void *rawData, int dataSize);

/**
    Register string command callback.
    The callback would recevie Linux nvt_uctrl command and return callback result to Linux.

    @return
        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_RegCmdCB(NvtUctrl_cmd *pCB);

/**
    Set return string to Linux nvt_uctrl.This API should be called in command callback function.
    While command callback finish,callback would return result and string.

    @note : maximum string legth is NVT_UCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_SetRetString(char *String);

/**
    Get config data from nvt_uctrl.


    @param[in] configData config data buffer pointer.
    @param[in] dataSize config data size.
    @return
        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_GetConfigData(void *configData, int dataSize);

/**
    Set config data from nvt_uctrl.


    @param[in] configData config data buffer pointer.
    @param[in] dataSize config data size.
    @return
        - @b NVTUCTRL_RET_OK if success
        - @b NVTUCTRL_RET_ERR if fail
*/
extern ER   NvtUctrl_SetConfigData(void *configData, int dataSize);

#endif /* _NVTUCTRL_H  */
