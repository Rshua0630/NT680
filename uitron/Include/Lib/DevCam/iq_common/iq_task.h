#ifndef _IQ_TASK_H_
#define _IQ_TASK_H_
/**
    Public header file for IQ

    This file is the header file that define the API and data type for IQ.

    @file       iq_task.h
    @ingroup    mILibIQAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#include "Type.h"
#include "ErrorNo.h"

/** \addtogroup mIIQS */
//@{

/**
    iqs process id
*/
typedef enum {
	IQS_ID_1 = 0,   //< iqs id 1
	IQS_ID_2,       //< iqs id 2
	IQS_ID_3,
	IQS_ID_4,
	IQS_ID_5,
	IQS_ID_6,
	IQS_ID_7,
	IQS_ID_8,
	IQS_ID_MAX_NUM,
	ENUM_DUMMY4WORD(IQS_PROC_ID)
} IQS_PROC_ID;

#define MAX_IQS_PROC_NUM      IQS_ID_MAX_NUM

/**
    iqs status
*/
typedef enum {
	IQS_TASK_STATUS_CLOSE = 0,
	IQS_TASK_STATUS_OPEN = 1,
	IQS_TASK_STATUS_START = 2,
	IQS_TASK_STATUS_PROC = 3,
	IQS_TASK_STATUS_PAUSE = 4,
	ENUM_DUMMY4WORD(IQS_TASK_STATUS)
} IQS_TASK_STATUS;

/**
    iqs event function type
*/
typedef void (*IQS_EVENT_FP)(UINT32 Id, UINT32 CurrentStatus);

/**
    iqs event

    control state:

        IQS_Open:    do nothing
        IQS_Start:   Start Event -> Process Event -> Process Event -> Process Event....
        IQS_Pause:   ....->Process Event -> Process Event -> Process Pause
        IQS_Resume:  Start Event -> Process Event -> Process Event -> Process Event....
*/
typedef struct {
	IQS_EVENT_FP Start;      ///< start event
	IQS_EVENT_FP Process;    ///< process event
	IQS_EVENT_FP Pause;      ///< pause event
} IQS_EVENT_OBJ;

/**
    iqs input object
*/
typedef struct {
	IQS_EVENT_OBJ EventTab;     ///< event table
} IQS_OBJ_INFO;

/**
     iqs lock/unlock for user suspend ae process

     @param[in] Id          iqs process id (IQS_PROC_ID)
     @param[in] flag        lock flag: enable, unlock flag: disable
     @param[in] WaitFinish  wait suspend flow finish
*/
extern void IQS_Lock(IQS_PROC_ID Id, BOOL flag, BOOL WaitFinish);

/**
     check iqs is lock or not

     @param[in] Id          iqs process id (IQS_PROC_ID)
     @return Description of data returned.
         - @b BOOL: true lock, false unlock
*/
extern BOOL IQS_IsLock(IQS_PROC_ID Id);

/**
     trigger event, for IPL hook

     @param[in] Id          iqs process id (IQS_PROC_ID)
*/
extern void IQS_TrigEvent(IQS_PROC_ID Id);

/**
     open iqs process

     @param[in] Id          iqs process id
     @param[in] Obj         iqs process event
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER IQS_Open(IQS_PROC_ID Id, IQS_OBJ_INFO *Obj);

/**
     close iqs process
     @param[in] Id          iqs process id

     @return Description of data returned.
         - @b ER: E_OK success
*/
ER IQS_Close(IQS_PROC_ID Id);

/**
     start iqs process

     @param[in] Id          iqs process id (IQS_PROC_ID)
     @param[in] WaitFinish  wait start event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER IQS_Start(IQS_PROC_ID Id, BOOL WaitFinish);

/**
     resume iqs process(for IPL control)

     @param[in] Id          iqs process id (IQS_PROC_ID)
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER IQS_Resume(IQS_PROC_ID Id);

/**
     check IQS is pause or not

     @param[in] Id          IQS process id (IQS_PROC_ID)
     @return Description of data returned.
         - @b BOOL: true pause, false not pause
*/
extern BOOL IQS_IsPause(IQS_PROC_ID Id);

/**
     pause iqs process(for IPL control)

     @param[in] Id          iqs process id (IQS_PROC_ID)
     @param[in] WaitFinish  wait pause event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER IQS_Pause(IQS_PROC_ID Id, BOOL WaitFinish);


/**
    install IQS control id
*/

extern void IQS_InstallID(void) _SECTION(".kercfg_text");
//@}

extern UINT32 IQS_FrameCnt[MAX_IQS_PROC_NUM];

#endif 
