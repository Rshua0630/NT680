
#ifndef _UIFLOWMOVIEFUNCS_H
#define _UIFLOWMOVIEFUNCS_H
#include "UIFlow.h"
#include "Type.h"

//------------------------------------------------------------
#define _UIRECAVI_DBG_MSG_              0
#if _UIRECAVI_DBG_MSG_
#include "Debug.h"
#define DbgMsg_UIRecAvi(msg)        debug_msg msg
#define DbgCode_UIRecAvi(x)         x
#else
#define DbgMsg_UIRecAvi(msg)
#define DbgCode_UIRecAvi(x)
#endif

#define UIFlowWndMovie_Restart_Rec      0x5a5a


enum _MOV_TASK_STATE {
	MOV_ST_IDLE = 0,        // idle mode
	MOV_ST_MENU,             // menu mode
	MOV_ST_WARNING_MENU,
	MOV_ST_QMENU,            // quick menu state
	MOV_ST_QDMENU,           // quick menu state
	MOV_ST_MENU_SCENE,            // quick menu state
	MOV_ST_MENU_EXPOSURE,           // quick menu state
	MOV_ST_MENU_VIVILINK,           // quick menu state
	MOV_ST_VIEW,             // preview mode
	MOV_ST_VIEW_WAIT,
	MOV_ST_REC,              // record movie
	MOV_ST_REC_WAIT,              // record movie
	MOV_ST_PAUSE,            // pause recording
	MOV_ST_SHUTTER_1,         // shutter 1 key pressed state
	MOV_ST_ZOOM = 0x80,      // zoom state (using MSB, because it could be combined with VIEW/REC state)
};

typedef struct _MOV_TASK_DATA {
	UINT8   State;          // current state
	UINT8   err;            // error status
	UINT16  TimerCount;     // timer count (per 100 ms)
	UINT16  SysTimeCount;   // System time count (per 1s)
	UINT16  MsgCount;       // message display count (per 100 ms)
	BOOL    bEnterMenu;     // enter menu confirm flag
} MOV_TASK_DATA;

extern MOV_TASK_DATA gMovData;

extern void   FlowMovie_SetVivilinkData(UINT32 uiVivilinkData);
extern UINT8  FlowMovie_GetVivilinkData(void);
extern UINT8  FlowMovie_GetMovDataState(void);
extern void   FlowMovie_StartRec(void);
extern void   FlowMovie_StopRec(void);
extern void   FlowMovie_StopRecSelfTimer(void);
extern UINT8  FlowMovie_GetDataState(void);
extern void   FlowMovie_SetRecMaxTime(UINT32 RecMaxTime);
extern UINT32 FlowMovie_GetRecMaxTime(void);
extern void   FlowMovie_SetRecCurrTime(UINT32 RecCurrTime);
extern UINT32 FlowMovie_GetRecCurrTime(void);
extern UINT32 FlowMovie_GetSelfTimerID(void);
extern BOOL   FlowMovie_IsStorageErr(BOOL IsCheckFull);
extern void   FlowMovie_GetPictureInMovie(void);
extern BOOL   FlowMovie_ChkDrawStoreFullFolderFull(void);

extern void   FlowMovie_OnTimer1SecIndex(void);

//Callback function
extern void FlowMovie_RawEncCB(UINT32 uiEventID);
#endif //_UIFLOWMOVIEFUNCS_H
