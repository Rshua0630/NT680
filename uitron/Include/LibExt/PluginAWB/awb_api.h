/**
    API header file for AWB

    This file is the header file that defines the API and data type for AWB

    @file       awb_api.h
    @ingroup    LibExt\AWB
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _AWB_API_H_
#define _AWB_API_H_

#include "ipl_alg_infor.h"
#include "awb_ui_info.h"
#include "awb_alg.h"

/*
    AWB converge speed level
*/

typedef enum _SEL_AWB_CONV_SKIP_FRAME_LEVEL {
	SEL_AWB_CONV_SKIP_FRAME_LV1 = 1,            ///<-slow
	SEL_AWB_CONV_SKIP_FRAME_LV2 = 2,            ///<-
	SEL_AWB_CONV_SKIP_FRAME_LV3 = 3,            ///<-
	SEL_AWB_CONV_SKIP_FRAME_LV4 = 4,            ///<-
	SEL_AWB_CONV_SKIP_FRAME_LV5 = 5,            ///<-normal
	SEL_AWB_CONV_SKIP_FRAME_LV6 = 6,            ///<-
	SEL_AWB_CONV_SKIP_FRAME_LV7 = 7,            ///<-
	SEL_AWB_CONV_SKIP_FRAME_LV8 = 8,            ///<-
	SEL_AWB_CONV_SKIP_FRAME_LV9 = 9,            ///<-
	SEL_AWB_CONV_SKIP_FRAME_LV10 = 10,          ///<-fast
	SEL_AWB_CONV_SKIP_FRAME_LEVEL_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_AWB_CONV_SKIP_FRAME_LEVEL)
} SEL_AWB_CONV_SKIP_FRAME_LEVEL;

typedef enum _SEL_AWB_CONV_SPEED_LEVEL {
	SEL_AWB_CONV_SPEED_LV1 =  5,            ///<-slow
	SEL_AWB_CONV_SPEED_LV2 = 10,            ///<-
	SEL_AWB_CONV_SPEED_LV3 = 15,            ///<-
	SEL_AWB_CONV_SPEED_LV4 = 20,            ///<-
	SEL_AWB_CONV_SPEED_LV5 = 25,            ///<-normal
	SEL_AWB_CONV_SPEED_LV6 = 30,            ///<-
	SEL_AWB_CONV_SPEED_LV7 = 35,            ///<-
	SEL_AWB_CONV_SPEED_LV8 = 40,            ///<-
	SEL_AWB_CONV_SPEED_LV9 = 45,            ///<-
	SEL_AWB_CONV_SPEED_LV10 = 50,           ///<-fast
	SEL_AWB_CONV_SPEED_LEVEL_MAX_CNT,
	ENUM_DUMMY4WORD(SEL_AWB_CONV_SPEED_LEVEL)
} SEL_AWB_CONV_SPEED_LEVEL;

extern UINT32 AWB_GetCAInfo(UINT32 Id, awb_ca_info *CAInfo);

/**

     awb init function
*/
extern void AWB_Init(UINT32 id, UINT32 CurrentStatus);

/**

     awb process function
*/
extern void AWB_Process(UINT32 id, UINT32 CurrentStatus);

/**

     awb single function
*/
void awb_single(UINT32 id);


/**

     awb get current gain.

     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void AWB_GetColorGain(UINT32 id, UINT32 *Rg, UINT32 *Gg, UINT32 *Bg);

/**

     awb set current gain.

     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void AWB_SetColorGain(UINT32 id, UINT32 Rg, UINT32 Gg, UINT32 Bg);

/**
     awb get capture gain.

     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void awb_get_cap_color_gain(UINT32 id, UINT32 *Rg, UINT32 *Gg, UINT32 *Bg);

//U ctrl
typedef enum {
	AWB_SET_SKIP_FRAME,
	AWB_SET_SPEED,
	AWB_SET_USER_RATIO,
	AWB_SET_GAIN,
	AWB_SET_MAX,
} AwbSetItem;

typedef struct {
	SEL_AWB_CONV_SKIP_FRAME_LEVEL uiConvSkipFrame;
	SEL_AWB_CONV_SPEED_LEVEL uiConvSpeed;
    UINT32 uiRratio;
    UINT32 uiBratio;
	UINT32 uiManualRGain;
	UINT32 uiManualGGain;
	UINT32 uiManualBGain;
} AwbSetParam;


typedef struct {
	SEL_AWB_CONV_SKIP_FRAME_LEVEL ConvSkipFrame;
	SEL_AWB_CONV_SPEED_LEVEL ConvSpeed;
    UINT32 UserRratio;
    UINT32 UserBratio;
	UINT32 RGain;
	UINT32 GGain;
	UINT32 BGain;
	UINT32 ColorTemperature;
} AWBStatus;

/**

     awb get Information.

     @note

     @param[out] AwbSetParam
*/
extern void AWB_SetInfo(UINT32 Id, AwbSetItem awbItem, AwbSetParam *pAWBSetting);

/**

     awb set Information.

     @note

     @param[in] AwbSetParam
*/
extern void AWB_GetInfo(UINT32 Id, AwbSetParam *pAWBSetting);

/**

     awb get Status.

     @note

     @param[out] AWBStatus
*/
extern void AWB_GetStatus(UINT32 Id, AWBStatus *pAWBStatus);
/**

     awb write debug information

     @note

     @param[in] buffer start address
     @return
         - @b buffer end address
*/
extern UINT32 AWB_WriteDebugInfo(UINT32 id, UINT32 Addr);

/**
     awb get color average.

     @note

     @param[out] average value of r/g/b channel
*/
extern void AWB_Get_ColorAvg(UINT32 Id, UINT32 *r, UINT32 *g, UINT32 *b);

/**
    MWB Customer Process
*/
extern ER MWB_CalcCustomerColorGain(UINT32 id, AWB_WB mode, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain);
extern void MWB_WriteColorGain(UINT32 id, AWB_WB mode, UINT32 Rg, UINT32 Gg, UINT32 Bg);
extern void MWB_ReadColorGain(UINT32 id, AWB_WB mode, UINT32 *Rg, UINT32 *Gg, UINT32 *Bg);

extern void awb_param_init(UINT32 id, UINT32* init);

typedef struct _AWB_SENSOR_MODE_PARAM {
	void(*init_func)(UINT32 id);
}AWB_SENSOR_MODE_PARAM;

#endif //_AWB_API_H_
