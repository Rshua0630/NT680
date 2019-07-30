/**
    Public header file for IPL Callback message

    This file is the header file that define the API and data type for IPL Callback message.

    @file       IPL_CBMsg.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_CBMSG_H_
#define _IPL_CBMSG_H_
#include "Type.h"
#include "ipl_utility.h"

/** \addtogroup mIIPL */
//@{

/**
     IPL callback event
*/
typedef enum _IPL_CBMSG {
	IPL_CBMSG_PREVIEW = 0,          ///< Callback for preview first frame ready

	IPL_CBMSG_FOCUS,                ///< NULL
	IPL_CBMSG_FOCUSEND,             ///< NULL

	IPL_CBMSG_PREFLASH,             ///< Callback for enter preflash
	IPL_CBMSG_PREFLASHEND,          ///< Callback for exit preflash

	IPL_CBMSG_DZOOM,                ///< Callback for digital zoom end

	IPL_CBMSG_SHUTTER_SOUND,        ///< Callback for shutter sound event

	IPL_CBMSG_MAX_CNT,
	ENUM_DUMMY4WORD(IPL_CBMSG)
} IPL_CBMSG;

/**
     IPL RSC callback event
*/
typedef enum _IPL_RSC_CBMSG {
	IPL_RSC_CBMSG_CHGMODE_START = 0,        ///< Callback for RSC Change Mode Start
	IPL_RSC_CBMSG_CHGMODE_IPLSTOP,      ///< Callback for RSC Change Mode Ipl Stop
	IPL_RSC_CBMSG_CHGMODE_END,          ///< Callback for RSC Change Mode End

	ENUM_DUMMY4WORD(IPL_RSC_CBMSG)
} IPL_RSC_CBMSG;

/**
     IPL_CBMSG_SETDATASTAMPINFOR callback data
*/
typedef struct _IPL_CBMSG_DS_INFOR {
	IPL_YCC_IMG_INFO img_info;           ///< image information
} IPL_CBMSG_DS_INFOR;

/**
     IPL_CBMSG_SETPHOTOFRAMEINFOR callback data
*/
typedef struct _IPL_CBMSG_PF_INFOR {
	IPL_YCC_IMG_INFO img_info;           ///< image information
} IPL_CBMSG_PF_INFOR;


/**
     IPL callback function prototype
*/
typedef void (*IPL_CBMSG_FP)(IPL_PROC_ID Id, IPL_CBMSG Msg, void *Data);

/**
    callback function prototype
*/
typedef void (*IPL_RSC_CBMSG_FP)(IPL_RSC_CBMSG Msg, void *Data);


/**
     initial callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_CBMsgInit(IPL_PROC_ID Id, IPL_CBMSG_FP fp);

/**
     un-initial callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_CBMsgUnInit(IPL_PROC_ID Id);

/**
     send CB message

     @param[in] Id IPL process Id
     @param[in] IPL_CBMSG ipl callback message
     @param[in] void* data pointer
*/
extern void IPL_CBMsgProc(IPL_PROC_ID Id, IPL_CBMSG Msg, void *Data);
/**
     initial RSC callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_RSC_CBMsgInit(IPL_PROC_ID Id, IPL_RSC_CBMSG_FP fp);

/**
     un-initial RSC callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_RSC_CBMsgUnInit(IPL_PROC_ID Id);

/**
     send RSC CB message

     @param[in] Id IPL process Id
     @param[in] IPL_RSC_CBMSG ipl callback message
     @param[in] void* data pointer
*/
extern void IPL_RSC_CBMsgProc(IPL_PROC_ID Id, IPL_RSC_CBMSG Msg, void *Data);

//@}
#endif //_IPL_CBMSG_H_