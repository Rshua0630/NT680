/**
    Public header file for cal setting information

    @file       ipl_cal_infor.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#ifndef _IPL_CAL_INFOR_H_
#define _IPL_CAL_INFOR_H_

#include "Type.h"
#include "ipl_utility.h"
#include "ErrorNo.h"

#define CAL_INFOR_ERR   0xffffffff    ///< error code(ipl_cal_get_info())
#define CAL_SEL_TAG   0x87650000

/**
    calibration informatin selection index(ipl_cal_set_info() or ipl_cal_get_info())
*/
typedef enum  {
	CAL_SEL_GETRAW_SENMODE = CAL_SEL_TAG,   ///<getraw sensor mode, data type #SENSOR_MODE
	CAL_SEL_GETRAW_CALCOND,     			///<getraw cal condition, data type #CAL_COND

	CAL_SEL_GETRAW_SIE_ACT_STR_X,     		///<getraw sie active start x
	CAL_SEL_GETRAW_SIE_ACT_STR_Y,     		///<getraw sie active start y
	CAL_SEL_GETRAW_SIE_CRP_STR_X,     		///<getraw sie crop start x
	CAL_SEL_GETRAW_SIE_CRP_STR_Y,     		///<getraw sie crop start y

	CAL_SEL_ALG_DP_TAG,     				///<alg dp tag,  data type #CAL_ITEM_DP_TAG

	CAL_SEL_USER_DEFINE1,
	CAL_SEL_USER_DEFINE2,

	CAL_SEL_MAX_CNT,
	ENUM_DUMMY4WORD(CAL_SEL_ITEM)
} CAL_SEL_ITEM;

#define CAL_SEL_MAX_ITEM  (CAL_SEL_MAX_CNT - CAL_SEL_TAG)

/**
     ipl cal get Information

     @param[in] id    IPL process id
     @param[in] sel_item    cal select item

     @return
        -b UINT32: select item information.
*/
extern UINT32 ipl_cal_get_info(IPL_PROC_ID id, CAL_SEL_ITEM sel_item);

/**
     ipl cal set Information

     @param[in] id    IPL process id
     @param[in] sel_item    cal select item
     @param[in] Value     information for select item

     @return Description of data returned.
         - @b E_OK
         - @b E_SYS
*/
extern ER ipl_cal_set_info(IPL_PROC_ID id, CAL_SEL_ITEM sel_item, UINT32 value);


/**
     register event callback function

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
typedef void (*IPL_CAL_INFOR_EVENT_FP)(IPL_PROC_ID id, UINT32 sel_item, UINT32 value);
extern ER ipl_cal_set_event_fp(IPL_PROC_ID id, CAL_SEL_ITEM sel_item, IPL_CAL_INFOR_EVENT_FP fp);


/**
     initial cal information

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER ipl_cal_info_init(void);

/**
     un-initial cal information

     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern ER ipl_cal_info_uninit(void);

//@}
#endif
