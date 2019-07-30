/*
    IPL calibration ext ctrl flow

    @file       ipl_cal_ctrl_sample_ff_int.h
    @ingroup    mILibCal
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _CAL_CTRL_SAMPLE_FF_INT_H
#define _CAL_CTRL_SAMPLE_FF_INT_H
#include "ipl_cal_setting_sample_ff_int.h"
#include "ipl_ctrl_infor.h"

typedef enum {
	CAL_BUF_ID_PRV_ITEM_NONE,

	CAL_BUF_ID_PRV_ITEM_IPL_1,  ///< for IPL
	CAL_BUF_ID_PRV_ITEM_IPL_2,  ///< for IPL
	CAL_BUF_ID_PRV_ITEM_IPL_3,  ///< for IPL
	CAL_BUF_ID_PRV_ITEM_IPL_4,  ///< for IPL
	CAL_BUF_ID_PRV_ITEM_IPL_5,  ///< for IPL
	CAL_BUF_ID_PRV_ITEM_IPL_6,  ///< for IPL
	CAL_BUF_ID_PRV_ITEM_IPL_MAX,

	CAL_BUF_ID_PRV_ITEM_IME1_1, ///< for IME path 1
	CAL_BUF_ID_PRV_ITEM_IME1_2, ///< for IME path 1
	CAL_BUF_ID_PRV_ITEM_IME1_3, ///< for IME path 1
	CAL_BUF_ID_PRV_ITEM_IME1_4, ///< for IME path 1
	CAL_BUF_ID_PRV_ITEM_IME1_5, ///< for IME path 1
	CAL_BUF_ID_PRV_ITEM_IME1_6, ///< for IME path 1
	CAL_BUF_ID_PRV_ITEM_IME1_7, ///< for IME path 1
	CAL_BUF_ID_PRV_ITEM_IME1_MAX,

	CAL_BUF_ID_PRV_ITEM_IME2_1, ///< for IME path 2
	CAL_BUF_ID_PRV_ITEM_IME2_2, ///< for IME path 2
	CAL_BUF_ID_PRV_ITEM_IME2_3, ///< for IME path 2
	CAL_BUF_ID_PRV_ITEM_IME2_4, ///< for IME path 2
	CAL_BUF_ID_PRV_ITEM_IME2_5, ///< for IME path 2
	CAL_BUF_ID_PRV_ITEM_IME2_6, ///< for IME path 2
	CAL_BUF_ID_PRV_ITEM_IME2_7, ///< for IME path 2
	CAL_BUF_ID_PRV_ITEM_IME2_MAX,

	CAL_BUF_ID_PRV_ITEM_IME3_1, ///< for IME path 3
	CAL_BUF_ID_PRV_ITEM_IME3_2, ///< for IME path 3
	CAL_BUF_ID_PRV_ITEM_IME3_3, ///< for IME path 3
	CAL_BUF_ID_PRV_ITEM_IME3_4, ///< for IME path 3
	CAL_BUF_ID_PRV_ITEM_IME3_5, ///< for IME path 3
	CAL_BUF_ID_PRV_ITEM_IME3_6, ///< for IME path 3
	CAL_BUF_ID_PRV_ITEM_IME3_7, ///< for IME path 3
	CAL_BUF_ID_PRV_ITEM_IME3_MAX,

	CAL_BUF_ID_PRV_ITEM_IME4_1, ///< for IME path 4
	CAL_BUF_ID_PRV_ITEM_IME4_2, ///< for IME path 4
	CAL_BUF_ID_PRV_ITEM_IME4_3, ///< for IME path 4
	CAL_BUF_ID_PRV_ITEM_IME4_4, ///< for IME path 4
	CAL_BUF_ID_PRV_ITEM_IME4_5, ///< for IME path 4
	CAL_BUF_ID_PRV_ITEM_IME4_6, ///< for IME path 4
	CAL_BUF_ID_PRV_ITEM_IME4_7, ///< for IME path 4
	CAL_BUF_ID_PRV_ITEM_IME4_MAX,

	CAL_BUF_ID_PRV_ITEM_IME5_1, ///< for IME path 5
	CAL_BUF_ID_PRV_ITEM_IME5_2, ///< for IME path 5
	CAL_BUF_ID_PRV_ITEM_IME5_3, ///< for IME path 5
	CAL_BUF_ID_PRV_ITEM_IME5_4, ///< for IME path 5
	CAL_BUF_ID_PRV_ITEM_IME5_5, ///< for IME path 5
	CAL_BUF_ID_PRV_ITEM_IME5_6, ///< for IME path 5
	CAL_BUF_ID_PRV_ITEM_IME5_7, ///< for IME path 5
	CAL_BUF_ID_PRV_ITEM_IME5_MAX,

	CAL_BUF_ID_PRV_ITEM_MAX_NUM = CAL_BUF_ITEM_MAX,

	ENUM_DUMMY4WORD(CAL_BUF_ID_PRV_ITEM_IDX)
} CAL_BUF_ID_PRV_ITEM_IDX;

typedef enum {
	CAL_BUF_ID_INI_DATA_ITEM_NONE = 0,

	CAL_BUF_ID_DATA_ITEM_STORAGE = 1, // ori data in storage, ex: PStore
	CAL_BUF_ID_DATA_ITEM_STORAGE_1,
	CAL_BUF_ID_DATA_ITEM_STORAGE_2,
	CAL_BUF_ID_DATA_ITEM_STORAGE_3,
	CAL_BUF_ID_DATA_ITEM_STORAGE_MAX,

	CAL_BUF_ID_DATA_ITEM_CUR = 6,
	CAL_BUF_ID_DATA_ITEM_CUR_1,
	CAL_BUF_ID_DATA_ITEM_CUR_2,
	CAL_BUF_ID_DATA_ITEM_CUR_3,
	CAL_BUF_ID_DATA_ITEM_CUR_MAX,

	CAL_BUF_ID_INI_DATA_ITEM_MAX_NUM = CAL_BUF_ITEM_MAX,

	ENUM_DUMMY4WORD(CAL_BUF_ID_INI_DATA_ITEM_IDX)
} CAL_BUF_ID_INI_DATA_ITEM_IDX;

/*******************************/
//////////////////////////////
//     raw header (ppb0)    //
//////////////////////////////
//                          //
//        raw (ppb0)        //
//                          //
//////////////////////////////
//     raw header (ppb1)    //
//////////////////////////////
//                          //
//        raw (ppb1)        //
//                          //
//////////////////////////////
/*******************************/
typedef enum {
	CAL_BUF_ID_RAW_ITEM_NONE = 0,

	// ipl only support max ppb*10 (SIE_Ch0_Ppb_Addr[10])
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_1 = 1,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_2,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_3,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_4,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_5,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_6,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_7,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_8,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_9,  ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_10, ///< for SIE ch0 output pingpong buffer (include raw header)
	CAL_BUF_ID_RAW_ITEM_CH0_PPB_MAX,

	// ipl only support max ppb*10 (SIE_Ch1_Ppb_Addr[10])
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_1 = 12,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_2,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_3,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_4,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_5,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_6,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_7,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_8,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_9,  ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_10, ///< for SIE ch1 output pingpong buffer (for CA)
	CAL_BUF_ID_RAW_ITEM_CH1_PPB_MAX,

	// ipl only support max ppb*10 (SIE_Ch2_Ppb_Addr[10])
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_1 = 23,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_2,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_3,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_4,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_5,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_6,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_7,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_8,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_9,  ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_10, ///< for SIE ch2 output pingpong buffer (for LA)
	CAL_BUF_ID_RAW_ITEM_CH2_PPB_MAX,

	CAL_BUF_ID_RAW_ITEM_MAX_NUM = CAL_BUF_ITEM_MAX,

	ENUM_DUMMY4WORD(CAL_BUF_ID_RAW_ITEM_IDX)
} CAL_BUF_ID_RAW_ITEM_IDX;

extern UINT32 cal_ext_get_buf_id_sample(UINT32 buf_id, CAL_GET_BUF_ID_INFO buf_id_info);
extern pCAL_EXT_CTRL_OBJ ipl_cal_ext_ctrl_sample(void);
extern IPL_PROC_ID display_id;

#endif
