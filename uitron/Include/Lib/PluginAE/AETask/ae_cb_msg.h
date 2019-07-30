/**
    Public header file for IPL Callback message

    This file is the header file that define the API and data type for AE Callback message.

    @file       ae_cb_msg.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _AE_CBMSG_H_
#define _AE_CBMSG_H_

#include "Type.h"
#include "ae_task.h"

/**
     AE callback event
*/
typedef enum _AE_CBMSG {
	AE_CBMSG_PREVIEWSTABLE,        ///< Callback for preview ae stable
	AE_CBMSG_MAX_CNT,
	ENUM_DUMMY4WORD(AE_CBMSG)
} AE_CBMSG;

/**
     AE callback function prototype
*/
typedef void (*AE_CBMSG_FP)(AE_CBMSG Msg, void *Data);

/**
     initial callback message function

     @param[in] Id AE process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void AE_CBMsgInit(AE_PROC_ID Id, AE_CBMSG_FP fp);

/**
     un-initial callback message function

     @param[in] Id AE process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void AE_CBMsgUnInit(AE_PROC_ID Id);

/**
     send CB message

     @param[in] Id AE process Id
     @param[in] AE_CBMSG ipl callback message
     @param[in] void* data pointer
*/
extern void AE_CBMsgProc(AE_PROC_ID Id, AE_CBMSG Msg, void *Data);

#endif //_AE_CBMSG_H_