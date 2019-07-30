/**
    Header file of NvtRtmp Client

    Exported header file of NvtRtmp Client.

    @file       nvtrtmpclient.h
    @ingroup    NVTRTMPCLIENT
    @version    V1.00.000
    @date       2018/03/14

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/

#ifndef _NVTRTMPCLIENT_H_
#define _NVTRTMPCLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <sys/time.h>
#include "nvt_type.h"

/**
    @addtogroup NVTRTMPCLIENT
*/
//@{

/**
    Struture of NVT RTMP Client User Object

    Input structure for NvtRtmp Client open.

    @note for NvtRtmpClient_Open()
*/
typedef struct _NVTRTMPCLIENT_USEROBJ {
	UINT32      channel;                        ///<[in] Source channel to push stream
	char        *rtmpURL;                       ///<[in] RTMP URL to push stream
} NVTRTMPCLIENT_USEROBJ, *PNVTRTMPCLIENT_USEROBJ;


/**
    NvtRtmpClient handler

    Handler for calling NvtRtmpClient functions.

    @note get from NvtRtmpClient_Open(), used in NvtRtmpClient_Start() / NvtRtmpClient_Stop() / NvtRtmpClient_Close()
*/
typedef  void*     NVTRTMPCLIENT_HANDLER;       ///< NvtRtmpClient handler


/**
    Open NvtRtmpClient.

    This API would initialize NvtRtmpClient.
    Other APIs should be invoked after calling this API.

    @param[in]  pNvtRtmpClientUsrObj    the NvtRtmpClient user object.
    @param[out] phRtmpC                 NvtRtmpClient handler, to be used in other NvtRtmpClient functions.

    @return
        - @b ER_SUCCESS if success.
        - @b ER_MEM_ALLOC_FAIL if alloc NvtRtmpClient handler fail.
        - @b ER_FUNC_START_FAIL if streamsender/pushrtmp init fail. Or connecting to given rtmp URL fail.
*/
extern NVTRET NvtRtmpClient_Open(PNVTRTMPCLIENT_USEROBJ pNvtRtmpClientUsrObj, NVTRTMPCLIENT_HANDLER *phRtmpC);


/**
    Start NvtRtmpClient to push stream.

    This API will start to get video/audio bitstream and push to given rtmp server URL.

    @param[in] hRtmpC      NvtRtmpClient handler.

    @return
        - @b ER_SUCCESS if success.
        - @b ER_FUNC_START_FAIL if fail to create video/audio threads.
        - @b ER_INVALID_INPUT_DATA if given hRtmpC is NULL.
*/
extern NVTRET NvtRtmpClient_Start(NVTRTMPCLIENT_HANDLER hRtmpC);


/**
    Stop NvtRtmpClient to push stream.

    This API will stop pushing stream to rtmp server.

    @param[in] hRtmpC      NvtRtmpClient handler.

    @return
        - @b ER_SUCCESS if success.
        - @b ER_INVALID_INPUT_DATA if given hRtmpC is NULL.
*/
extern NVTRET NvtRtmpClient_Stop(NVTRTMPCLIENT_HANDLER hRtmpC);


/**
    Close NvtRtmpClient.

    This API will close NvtRtmpClient and release resource.

    @param[in] hRtmpC      NvtRtmpClient handler.

    @return
        - @b ER_SUCCESS if success.
        - @b ER_FUNC_STOP_FAIL if streamsender close fail.
        - @b ER_INVALID_INPUT_DATA if given hRtmpC is NULL.
*/
extern NVTRET NvtRtmpClient_Close(NVTRTMPCLIENT_HANDLER hRtmpC);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
//@}
#endif //_NVTRTMPCLIENT_H_
