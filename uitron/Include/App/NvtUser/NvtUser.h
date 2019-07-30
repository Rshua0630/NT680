/**
    Header file of user event task

    Event driven user task.

    @file       NvtUser.h
    @ingroup    mINvtUser
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef USERTSK_H
#define USERTSK_H

#include "Type.h"
#include "NVTEvent.h"
#include "NVTReturn.h"

extern void NvtUser_InstallID(void) _SECTION(".kercfg_text");

/**
    @addtogroup mINvtUser
*/

//@{

typedef  enum {
	UXUSER_CB_QFULL = 1,  ///<message queue is full
} UXUSER_CB_TYPE;

typedef void (*USER_MAIN_FP)(void);                                             ///< main process callback
typedef void (*USER_CB_FP)(UXUSER_CB_TYPE evt, UINT32 p1, UINT32 p2, UINT32 p3);///< error status callback


/**
@name UserTsk Open Data Structures
@{
*/
typedef struct _UXUSER_OBJ {
	USER_MAIN_FP pfMainProc;
	USER_CB_FP   pfNvtUserCB;
} UXUSER_OBJ;


/**
     Open task.

     Open NvtUser task.
     @note Nothing.

     @param[in] param           Open Data Structures.
     @return
         - @b E_OK:             Successed
         - @b Other:            Failed.
*/
extern ER Ux_Open(UXUSER_OBJ *param);

/**
     Close task.

     Close NvtUser task.
     @note Nothing.

     @return
         - @b E_OK:             Successed
         - @b Other:            Failed.
*/
extern ER Ux_Close(void);

/**
     Check status.

     Check is NvtUser task is already opened or not.
     @note Nothing.

     @return
         - @b TRUE:             Opened
         - @b FALSE:            Not opened yet.
*/
extern BOOL Ux_IsOpen(void);

/**
     Dump event status.

     Dump entire event queue content of NvtUser task.
     @note Nothing.
*/
extern void Ux_DumpEvents(void);

/**
     Get task ID.

     Get task ID of NvtUser task.
     @note Nothing.

     @return task ID
*/
extern ID Ux_GetTaskID(void);

/**
     Post event.

     Post event to event queue of NvtUser task.
     @note Nothing.

     @param[in] evt             event.
     @param[in] paramNum        other parameters.
*/
extern void Ux_PostEvent(NVTEVT evt, UINT32 paramNum, ...);

/**
     Flush event.

     Flush all events of event queue of NvtUser task.
     @note Nothing.
*/
extern void Ux_FlushEvent(void);

/**
     Flush event.

     Flush event of event queue of NvtUser task,
     if the event value is inside the min-max range.
     @note Nothing.

     @param[in] start           event minimal value.
     @param[in] end             event maximal value.
*/
extern void Ux_FlushEventByRange(UINT32 start, UINT32 end);

/**
     Wait and get event.

     Wait and get event from event queue of NvtUser task.
     @note Nothing.

     @param[out] evt            pointer to event.
     @param[out] paramNum       pointer of parameter count.
     @param[out] paramArray     pointer of parameter array.
*/
extern void Ux_WaitEvent(NVTEVT *evt, UINT32 *paramNum, UINT32 *paramArray);

/**
     Enable/Disable calculate post evnt fps.

     if enable API.When event count reaches the unit. It would the event count divided by the time.
     @note Nothing.

     @param[in] bEn            enable/disalbe calculation.
     @param[in] unit       	   event count reaches the unit.It would calcualte fps one time.
*/
extern void Ux_CalPostFps(UINT32 bEn,UINT32 unit);

//@}

#endif
