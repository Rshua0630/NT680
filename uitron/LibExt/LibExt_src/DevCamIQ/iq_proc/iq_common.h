#ifndef _IQ_COMMON_H_
#define _IQ_COMMON_H_

#include "Type.h"
#include "ErrorNo.h"

#include "DefogLib.h"
#include "SceneRenderLib.h"
#include "SensorHDRLib.h"

#include "ipl_ctrl_infor.h"
#include "ipl_icf_func.h"
#include "ipl_hal_op.h"

#include "ae_api.h"
#include "awb_api.h"

#include "iq_ctrl.h"
#include "iq_param_ctrl.h"
#include "iq_task.h"
#include "iq_tuning_api.h"
#include "iq_uart_cmd.h"
#include "iq_ui_info.h"
#include "iq_utility.h"
#include "iq_api.h"

#include "iq_proc.h"
#include "iq_func.h"
#include "iq_effect_table.h"
#include "iq_tuning.h"

//#define IQS_SIMT_ENABLE
#ifdef IQS_SIMT_ENABLE
#include "iq_sim.h"
#endif

#define __MODULE__ IQ
#define __DBGLVL__ 4        // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

typedef enum {
	IQS_GAIN_1X,
	IQS_GAIN_2X,
	IQS_GAIN_4X,
	IQS_GAIN_8X,
	IQS_GAIN_16X,
	IQS_GAIN_32X,
	IQS_GAIN_64X,
	IQS_GAIN_128X,
	IQS_GAIN_256X,
	IQS_GAIN_512X,
	IQS_GAIN_1024X,
	IQS_GAIN_2048X,

	TOTAL_GAIN_NUM
} IQS_ISONumEnum;

typedef enum {
	H264_UI_3DNR_OFF,
	H264_UI_3DNR_LV1,
	H264_UI_3DNR_LV2,
	H264_UI_3DNR_LV3,
	H264_UI_3DNR_LV4,
	H264_UI_3DNR_LV5,
	H264_UI_3DNR_LV6,
	H264_UI_3DNR_LV7,
	H264_UI_3DNR_LV8,
	H264_UI_3DNR_LV9,
	H264_UI_3DNR_LV10,
	H264_UI_3DNR_LV11,
	H264_UI_3DNR_LV12,

	TOTAL_UI_3DNR_LV
} IQS_UI3dnrEnum;

#endif
