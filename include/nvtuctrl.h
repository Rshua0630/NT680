/**
    Structure and API definitions of nvtuctrl library.

    @file nvtuctrl.h
    Copyright Novatek Microelectronics Corp. 2014. All rights reserved.
*/
#ifndef _NVTUCTRL_H
#define _NVTUCTRL_H

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NVTUCTRL_CMD_SIZE                       256  ///<maximum command length

/**
   Error Code.

   Any error code occured will return.
*/
#define    NVTUCTRL_ER_OK                       ( 0) ///< command OK
#define    NVTUCTRL_ER_ERR                      (-1) ///< command fail
#define    NVTUCTRL_ER_NO_FUNC                  (-2) ///< no operation
#define    NVTUCTRL_ER_PAR_ERR                  (-3) ///< parameter error
#define    NVTUCTRL_ER_IPC_ERR                  (-4) ///< IPC error

/**
    Open NvtUctrl.

    This API would initialize NvtUctrl.
    Need to open NvtUctrl before access NvtUctrl.

    @param[out] handle      NvtUctrl handle.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
*/
int NvtUctrl_Open(void);

/**
    Close NvtUctrl.

    This API will close NvtUctrl,release resource.

    @param[in] hdl      NvtUctrl handle.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
*/
int NvtUctrl_Close(int hdl);

/**
    Send command.

    This API will send command string to uItron.

    @param[in] hdl          NvtUctrl handle.
    @param[in] cmd          The pointer of command string.
    @param[in/out] retAddr  [in]a buffer addr.[out]return string after perform command
    @param[in/out] retSize  [in]buffer size.[out]return result string size.

    @note cmd and retSize maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_NO_FUNC if no uItron command table
        - @b NVTUCTRL_ER_PAR_ERR if API parameter fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail

*/
int NvtUctrl_CmdSnd(int hdl,char *cmd,char *retAddr,int retSize);

/**
    Receive command.

    This API will receive command string from uItron.

    @param[in] hdl          NvtUctrl handle.
    @param[in/out] cmd      [in]a buffer addr for receive command.[out]receive string.
    @param[in/out] cmdSize  [in]the buffer size[out]string size while received command.

    @note cmd maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail
*/
int NvtUctrl_CmdRcv(int hdl,char *cmd,int cmdSize);

/**
    Receive raw data command.

    This API will receive command raw data from uItron.

    @param[in] hdl          NvtUctrl handle.
    @param[in/out] cmd      [in]a buffer addr for receive command.[out]receive string.
    @param[in/out] cmdSize  [in]the buffer size[out]data size while received command.

    @note cmd maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail
*/
int NvtUctrl_CmdRcvRaw(int hdl,void *buf,int *bufSize);

/**
    Send command and get config.

    This API will send command to uItron to get config.

    @param[in] hdl          NvtUctrl handle.
    @param[in] confCmd      The pointer of command config.
    @param[in/out] retAddr  [in]a buffer addr.[out]return string after perform command
    @param[in/out] retSize  [in]buffer size.[out]return result string size.

    @note cmd and retSize maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_NO_FUNC if no uItron command table
        - @b NVTUCTRL_ER_PAR_ERR if API parameter fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail

*/
int NvtUctrl_CmdGetConfig(int hdl,char *cmd,void *configData,int dataSize,char *retAddr,int retSize);

/**
    Send command with config.

    This API will send command with config to uItron.

    @param[in] hdl          NvtUctrl handle.
    @param[in] confCmd      The pointer of command config.
    @param[in/out] retAddr  [in]a buffer addr.[out]return string after perform command
    @param[in/out] retSize  [in]buffer size.[out]return result string size.

    @note cmd and retSize maximun is NVTUCTRL_CMD_SIZE

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTUCTRL_ER_ERR if fail
        - @b NVTUCTRL_ER_NO_FUNC if no uItron command table
        - @b NVTUCTRL_ER_PAR_ERR if API parameter fail
        - @b NVTUCTRL_ER_IPC_ERR if ipc fail

*/
int NvtUctrl_CmdSetConfig(int hdl,char *cmd,void *configData,int dataSize,char *retAddr,int retSize);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTUCTRL_H  */
