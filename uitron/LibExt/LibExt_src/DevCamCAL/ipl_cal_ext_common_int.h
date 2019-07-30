#ifndef _IPL_CAL_EXT_COMMON_H_
#define _IPL_CAL_EXT_COMMON_H_
// system
#include "Type.h"
#include "ErrorNo.h"
// ipl
#include "sensor.h"
#include "ipe_lib.h"
#include "ipl_utility.h"
#include "ipl_header_infor_raw.h"
#include "ipl_header_infor_yuv.h"
#include "ipl_isr_event.h"
#include "ipl_hal_op.h"
// calibration
#include "ipl_calibration.h"
#include "ipl_cal_3a.h"
#include "ipl_cal_ctrl.h"
#include "ipl_cal_api.h"
#include "ipl_cal_buf.h"
#include "ipl_cal_alg.h"
#include "ipl_cal_obj.h"
#include "ipl_cal_obj_drv.h"

#define __MODULE__ IPL_CAL_EXT_COMMON
#define __DBGLVL__ 2          // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

extern BOOL b_ipl_cal_dbg_en;  //ipl control debug enable/disable
#endif //_IPL_EXT_COMMON_H_
