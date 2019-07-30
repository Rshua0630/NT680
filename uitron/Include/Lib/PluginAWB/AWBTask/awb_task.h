/**
    Public header file for AWB

    This file is the header file that defines the API and data type for AWB task.

    @file       awb_task.h
    @ingroup    Lib\AWBTask
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _AWB_TASK_H_
#define _AWB_TASK_H_

#include "Type.h"
#include "ErrorNo.h"

/******************************/
/**
    AWB process id
*/
typedef enum {
	AWB_ID_1 = 0,       ///< awb id 1
	AWB_ID_2,           ///< awb id 2
	AWB_ID_3,           ///< awb id 3
	AWB_ID_4,           ///< awb id 4
	AWB_ID_5,           ///< awb id 5
	AWB_ID_6,           ///< awb id 6
	AWB_ID_7,           ///< awb id 7
	AWB_ID_8,           ///< awb id 8
	AWB_ID_MAX_NUM,
	ENUM_DUMMY4WORD(AWB_PROC_ID)
} AWB_PROC_ID;

#define MAX_AWB_PROC_NUM      AWB_ID_MAX_NUM
/**********************************************************************/

/******************************/
/**
    AWB status
*/
typedef enum {
	AWB_TASK_STATUS_CLOSE   = 0,
	AWB_TASK_STATUS_OPEN    = 1,
	AWB_TASK_STATUS_START   = 2,
	AWB_TASK_STATUS_PROC    = 3,
	AWB_TASK_STATUS_PAUSE   = 4,
	ENUM_DUMMY4WORD(AWB_TASK_STATUS)
} AWB_TASK_STATUS;
/**********************************************************************/

/******************************/
/**
    AWB event
    control state:
        AWB_Open:    hook event object
        AWB_Start:   Start Event -> Process Event -> Process Event -> Process Event....
        AWB_Pause:   ....->Process Event -> Process Event -> Process Pause
        AWB_Resume:  Start Event -> Process Event -> Process Event -> Process Event....
*/

/**
    awb event function pointer
*/
typedef void (*AWB_EVENT_FP)(UINT32 Id, UINT32 CurrentStatus);

/**
    AWB event function
*/
typedef struct {
	AWB_EVENT_FP Start;     ///< start event
	AWB_EVENT_FP Process;   ///< process event
	AWB_EVENT_FP Pause;     ///< pause event
} AWB_EVENT_OBJ;

/**
    AWB event object
*/
typedef struct {
	AWB_EVENT_OBJ EventTab;     ///< event table
} AWB_OBJ_INFO;

/**
    Open AWB process
    @param[in] Id          awb process id
    @param[in] Obj         awb event object
    @return Description of data returned.
        ER: E_OK success
*/
ER AWB_Open(AWB_PROC_ID Id, AWB_OBJ_INFO *Obj);

/**
    Close AWB process
    @param[in] Id          awb process id
    @return Description of data returned.
        ER: E_OK success
*/
ER AWB_Close(AWB_PROC_ID Id);

/**
    Start AWB process
    @param[in] Id          awb process id (AWB_PROC_ID)
    @param[in] WaitFinish  wait start event finish
    @return Description of data returned.
        ER: E_OK success
*/
ER AWB_Start(AWB_PROC_ID Id, BOOL WaitFinish);

/**
    Resume AWB process(for IPL control)
    @param[in] Id          awb process id (AWB_PROC_ID)
    @return Description of data returned.
        ER: E_OK success
*/
ER AWB_Resume(AWB_PROC_ID Id);

/**
    Pause AWB process(for IPL control)
    @param[in] Id          awb process id (AWB_PROC_ID)
    @param[in] WaitFinish  wait pause event finish
    @return Description of data returned.
        ER: E_OK success
*/
ER AWB_Pause(AWB_PROC_ID Id, BOOL WaitFinish);

/**
    check AWB is pause or not
    @param[in] Id          awb process id (AWB_PROC_ID)
    @return Description of data returned.
        BOOL: true pause, false not pause
*/
BOOL AWB_IsPause(AWB_PROC_ID Id);
/**********************************************************************/

/******************************/
/**
    trigger event, for IPL hook
    @param[in] Id          awb process id (AWB_PROC_ID)
*/
void AWB_TrigEvent(AWB_PROC_ID Id);
/**********************************************************************/

/******************************/
/**
    Awb lock/unlock for user suspend Awb process

    @param[in] Id          awb process id (AWB_PROC_ID)
    @param[in] flag        lock flag: enable, unlock flag: disable
    @param[in] WaitFinish  wait suspend flow finish
*/
void AWB_Lock(AWB_PROC_ID Id, BOOL flag, BOOL WaitFinish);

/**
    check AWB is lock or not

    @param[in] Id          awb process id (AWB_PROC_ID)
    @return Description of data returned.
        BOOL: true lock, false unlock
*/
BOOL AWB_IsLock(AWB_PROC_ID Id);
/**********************************************************************/

/******************************/
/**
    AWB Tuning Lock/Unlock
*/
void AWBTuning_Lock(void);
void AWBTuning_Unlock(void);
/**********************************************************************/

/******************************/
/**
    UI AWB control value
*/
extern UINT32 ui_r_gain[AWB_ID_MAX_NUM], ui_g_gain[AWB_ID_MAX_NUM], ui_b_gain[AWB_ID_MAX_NUM];
/**********************************************************************/

/**
    install AWB control id
*/
extern void AWB_InstallID(void) _SECTION(".kercfg_text");
#endif //_AWB_TASK_H_