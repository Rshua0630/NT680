#ifndef _UIIPCAM_H_
#define _UIIPCAM_H_

#include "AppLib.h"
#include "UIAppCommon.h"
#include "NVTEvent.h"
#include "UVAC.h"
extern VControl CustomIPCamObjCtrl;


typedef enum {
	NVTEVT_IPCAM_EVT_START      = APPUSER_IPCAM_BASE, ///< Min value = 0x14001000
	//Flow and basic parameter
	NVTEVT_EXE_IPCAM_OPEN         = NVTEVT_IPCAM_EVT_START,
	NVTEVT_EXE_IPCAM_CLOSE,

} CUSTOM_IPCAM_EVENT;


//IPCam CB
extern void IPCam_RegCB(void);

//VdoEnc
extern void IPCamExe_UIDefine_3DNR(UINT32 pathID, UINT32 p3DNRConfig);
extern UINT32 IPCamExe_VdoEnc_ProcCB(UINT32 id, UINT32 state);
extern void IPCamExe_VdoEnc_CB(CHAR* Name, UINT32 event_id, UINT32 value);

//NetRTSP
extern void IPCamExe_RtspIFramReq_CB(UINT32 pathId);

//IPCAM check FPS
#if (IPCAM_RESET_FPS == ENABLE)
extern void IPCam_checkFPS_CB(void);
#endif

//UVAC
extern UINT32 IPCam_StartVideoCB(UVAC_VID_DEV_CNT vidDevIdx, UVAC_STRM_INFO *pStrmInfo);
extern void IPCam_StopVideoCB(UVAC_VID_DEV_CNT vidDevIdx);
extern UINT32 IPCam_SetVolumeCB(UINT32 volume);

#endif //_UIIPCAM_H_
