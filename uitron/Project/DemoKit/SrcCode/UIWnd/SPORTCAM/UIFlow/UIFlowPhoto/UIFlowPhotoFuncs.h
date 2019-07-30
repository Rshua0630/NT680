#ifndef _UIFLOWPHOTOFUNCS_H
#define _UIFLOWPHOTOFUNCS_H
#include "UIFlow.h"
#include "Type.h"

enum _PHOTO_TASK_STATE {
	PHOTO_ST_IDLE = 0x0001,      // idle state
	PHOTO_ST_MENU = 0x0002,         // menu state
	PHOTO_ST_WARNING_MENU = 0x0004,
	PHOTO_ST_QMENU = 0x0008,        // quick menu state
	PHOTO_ST_QDMENU = 0x0010,       // quick delete menu state
	PHOTO_ST_VIEW = 0x0020,         // preview state
	PHOTO_ST_MODEMENU = 0x0040,
	PHOTO_ST_MENUSCENE = 0x0080,    // quick delete menu state
	PHOTO_ST_SELFTIMER = 0x0100,    // self-timer state
	PHOTO_ST_ZOOM = 0x0200,         // zoom state
	//#NT#2011/03/07#Chris Chung -begin
	PHOTO_ST_ZOOM_STOP = 0x0400,    // zoom stop state
	//#NT#2011/03/07#Chris Chung -end
	//#NT#2008/10/15#Hideo Lin -begin
	//#Add shutter 1 key pressed state
	PHOTO_ST_SHUTTER_1 = 0x0800,         // shutter 1 key pressed state
	//#NT#2008/10/15#Hideo Lin -end
	PHOTO_ST_CAPTURE       // capture state
};

typedef struct _PHOTO_TASK_DATA {
	UINT16   State;          // current state
	UINT16   err;            // error status
	UINT16  TimerCount;     // timer count (per 100 ms)
	UINT16  MsgCount;       // message display count (per 100 ms)
	UINT16  QuickViewCount; // quick view time count (per 100 ms)
	UINT16  SelfTimerCount; // quick view time count (per 100 ms)
	UINT16  SysTimeCount;   // System time count (per 1s)
} PHOTO_TASK_DATA;

extern PHOTO_TASK_DATA gPhotoData;

extern void   FlowPhoto_SetVivilinkData(UINT32 uiVivilinkData);
extern UINT8  FlowPhoto_GetVivilinkData(void);
//extern UINT32 Get_FreePicNum(UINT32 uiSizeIndex, UINT32 uiQualityIndex);
extern CHAR *Get_FreePicNumString(UINT32 uiValue);
extern void FlowPhoto_DoCapture(void);
extern BOOL FlowPhoto_GetSDStatus(void);
extern void FlowPhoto_SetFdSdProc(BOOL sd_start);
extern void FlowPhoto_ForceLockFdSd(void);
extern UINT32 FlowPhoto_GetFreePicNum(void);
extern BOOL FlowPhoto_IsStorageErr(void);
extern void FlowPhoto_OnTimer01SecIndex(void);
extern void FlowPhoto_OnTimer1SecIndex(void);
extern void FlowPhoto_InitCfgSetting(void);


#endif
