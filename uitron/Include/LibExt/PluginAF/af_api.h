/**
 * Public header file for AF API.
 * 
 * This file is the header file that define the API and data type for AF.
 * 
 * @file af_api.h
 * @note Nothing (or anything need to be mentioned).
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _AF_API_H_
#define _AF_API_H_

#include "af_alg.h"
#include "af_task.h"
#include "af_ui_info.h"
#include "ipl_cmd.h"


/**
     AF callback message
*/
typedef enum
{
    AF_CBMSG_AF_START = 0,     ///< user will receive this notification when AF process is started.
    AF_CBMSG_AF_DONE  = 1,     ///< user will receive this notification when AF process is end.
    AF_CBMSG_MAX_CNT,          ///< total callback message count
    ENUM_DUMMY4WORD(AF_CBMSG)
} AF_CBMSG;


typedef struct {
    INT16 sManualStartIdx;
    INT16 sManualEndIdx;
    INT8  cManualStep;
} AF_DRV_MANUAL;


/**
    AF callback funcion prototype
*/
typedef void (*AF_CBMSG_FP)(AF_CBMSG msg_id, void *data);

/**
     register callback function

     @note User must register callback funcion then can receive the callback message notification when AF done.

     @param[in] CB: the callback function pointer
*/
extern void AF_RegisterCB(AF_CBMSG_FP cb_func);

extern ER AF_Run(AF_PROC_ID id, BOOL wait_finish);

extern ER AF_Release(AF_PROC_ID id, BOOL wait_finish);

/**
     AF initialize function

     @param[in] current_status    reserved
     @return
         - @b void
*/
extern void AF_Init(UINT32 id, UINT32 current_status);

/**
     AF process function

     @param[in] id
     @param[in] current_status    reserved
     @return
         - @b void
*/
extern void AF_Process(UINT32 id, UINT32 current_status);

/**
     setup IPE module to retrieve VA

     @param[in] id
     @return
         - @b void
*/
extern void AF_VA_Setup(UINT32 id);


extern void AF_DRV_GetCurrentRange(UINT32 id, AF_F af_mode);
extern UINT16 AF_DRV_GetFocusDist(void);

extern void AF_VA_SetCrop(UINT32 id, VACROP win_info);

extern void AF_SetManualRange(UINT32 id, INT16 start_pos, INT16 end_pos, INT8 step_size);

/**
     write AF debugging info at the end of JPEG file

     @param[in] id
     @param[in] addr    Address to save AF debugging info at the end of JPEG file.
     @return
         - @b void
*/
extern UINT32 AF_WriteDebugInfo(UINT32 id, UINT32 addr);


#endif    // _AF_API_H_


