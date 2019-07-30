/**
    IPL calibration data structure

    Describe calibration items which are supported.
    including working buffer control mech., calibration environment, calibration algorithm, calibration UI control.

    @file       ipl_calibration.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CALIBRATION_H_
#define _IPL_CALIBRATION_H_
// system
#include "ipl_utility.h"
#include "ipl_cmd.h"
#include "Delay.h"
#include "string.h"
// cal
#include "ipl_cal_infor.h"
// ipl
#include "ipl_alg_infor.h"
#include "ipl_isr_event.h"
#include "ipl_ctrl_par.h"
#include "ipl_ctrl.h"
#include "ipl_hal_sie_com_info.h"
#include "ipl_hal_sie_com.h"
#include "ipl_hal_op.h"
#include "ipl_ini.h"
#include "ipe_lib.h"
#include "sensor.h"
// ae
#include "ae_task.h"


#define CAL_HEADER_SIZE_DATA 0x1000
#define CAL_HEADER_SIZE CAL_HEADER_SIZE_DATA + ipl_ini_get_header_buf_size()
#define CAL_END 0xffff1234
#define CAL_IPL_ID2BIT(id) (1 << id)
#define CAL_RGB2Y(r,g,b) ((77*r+150*g+29*b) >> 12)
#define CAL_CHK_ITEM(bit, item) (bit & (1 << item))
#define CAL_CHK_ID(bit, id) (bit & (1 << id))
#define CAL_ITEM_ITEM2BIT(item) (1 << item)

typedef enum {
	CAL_ITEM_AWB        = 0,
	CAL_ITEM_DP         = 1,
	CAL_ITEM_ECS        = 2,
	CAL_ITEM_ISO        = 3,
	CAL_ITEM_LENSCEN    = 4,
	CAL_ITEM_USER1      = 5,
	CAL_ITEM_USER2      = 6,
	CAL_ITEM_USER3      = 7,
	CAL_ITEM_USER4      = 8,
	CAL_ITEM_USER5      = 9,
	CAL_ITEM_MAX_NUM    = 10,
	ENUM_DUMMY4WORD(CAL_ITEM)
} CAL_ITEM;

typedef enum {
	CAL_ITEM_BIT_NONE      	= 0x0,
	CAL_ITEM_BIT_AWB        = (1 << CAL_ITEM_AWB),
	CAL_ITEM_BIT_DP         = (1 << CAL_ITEM_DP),
	CAL_ITEM_BIT_ECS        = (1 << CAL_ITEM_ECS),
	CAL_ITEM_BIT_ISO        = (1 << CAL_ITEM_ISO),
	CAL_ITEM_BIT_LENSCEN    = (1 << CAL_ITEM_LENSCEN),
	CAL_ITEM_BIT_USER1      = (1 << CAL_ITEM_USER1),
	CAL_ITEM_BIT_USER2      = (1 << CAL_ITEM_USER2),
	CAL_ITEM_BIT_USER3      = (1 << CAL_ITEM_USER3),
	CAL_ITEM_BIT_USER4      = (1 << CAL_ITEM_USER4),
	CAL_ITEM_BIT_USER5      = (1 << CAL_ITEM_USER5),
	CAL_ITEM_BIT_ALL		= 0x3FF,
	ENUM_DUMMY4WORD(CAL_ITEM_BIT)
} CAL_ITEM_BIT;

/**
     Calibration result status.
*/
typedef enum {
	CAL_STATUS_NONE       = 0x0,          ///< no result
	CAL_STATUS_OK         = 0xfedba950,   ///< result is pass
	CAL_STATUS_OVERFLOW   = 0x00000001,   ///< result is over spec
	CAL_STATUS_UNDERFLOW  = 0x00000002,   ///< result is under spec
	CAL_STATUS_DP_D_OK    = 0x00000003,   ///< DP Dark is finish and PASS
	CAL_STATUS_DP_B_OK    = 0x00000004,   ///< DP Bright is finish and PASS
	CAL_STATUS_AE_TIMEOUT = 0x00000005,   ///< AE timeout
	ENUM_DUMMY4WORD(CAL_STATUS)
} CAL_STATUS;

typedef enum {
	CAL_COND_DFT, //default
	CAL_COND1,
	CAL_COND2,
	CAL_COND3,
	CAL_COND4,
	CAL_COND5,
	CAL_COND6,
	CAL_COND7,
	CAL_COND8,
	CAL_COND9,
	CAL_COND10,
	CAL_COND_MAX = 11,

	ENUM_DUMMY4WORD(CAL_COND)
} CAL_COND;

typedef enum {
	CAL_ITEM_DP_TAG_BRIGHT      = 0,
	CAL_ITEM_DP_TAG_DARK        = 1,
	CAL_ITEM_DP_TAG_BRIGHT_ONLY = 2,
	CAL_ITEM_DP_TAG_MAX         = 3,
	ENUM_DUMMY4WORD(CAL_ITEM_DP_TAG)
} CAL_ITEM_DP_TAG;

/**
     Calibration IPL information
*/
typedef enum {
	CAL_SIE_CH_0    = 0, // RAW
	CAL_SIE_CH_1    = 1, // CA
	CAL_SIE_CH_2    = 2, // LA
	CAL_SIE_CH_MAX  = 3,
	ENUM_DUMMY4WORD(CAL_SIE_CH)
} CAL_SIE_CH;

typedef enum {
	CAL_IME_PATH_1 = IPL_IME_PATH_ID_1,
	CAL_IME_PATH_2 = IPL_IME_PATH_ID_2,
	CAL_IME_PATH_3 = IPL_IME_PATH_ID_3,
	CAL_IME_PATH_4 = IPL_IME_PATH_ID_4,
	CAL_IME_PATH_5 = IPL_IME_PATH_ID_5,
	CAL_IME_PATH_MAX_NUM = IPL_IME_PATH_ID_MAX,
	ENUM_DUMMY4WORD(CAL_IME_PATH)
} CAL_IME_PATH;

typedef enum {
	CAL_IME_PATH_EN_1 = (1 << CAL_IME_PATH_1),
	CAL_IME_PATH_EN_2 = (1 << CAL_IME_PATH_2),
	CAL_IME_PATH_EN_3 = (1 << CAL_IME_PATH_3),
	CAL_IME_PATH_EN_4 = (1 << CAL_IME_PATH_4),
	CAL_IME_PATH_EN_5 = (1 << CAL_IME_PATH_5),
	ENUM_DUMMY4WORD(CAL_IME_PATH_EN)
} CAL_IME_PATH_EN;


// for ipl calibration r/w
typedef enum {
	CAL_RD_ONLY    = 0x01,    ///< read only
	CAL_WR_ONLY    = 0x02,    ///< write only
	CAL_CREATE     = 0x04,    ///< create
	CAL_UPDATE     = 0x08,    ///< update, only for PStore
} CAL_RW_OPEN_SEL;

typedef struct {
	UINT32(*fp_open_section)(char *p_sec_name, CAL_RW_OPEN_SEL open_sel);
	ER(*fp_close_section)(UINT32 p_section_handle);
	ER(*fp_read_section)(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle);
	ER(*fp_write_section)(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle);
	ER(*fp_delete_section)(char *p_sec_name);
	UINT32(*fp_get_sec_size)(char *p_sec_name);
	BOOL(*fp_is_sec_exist)(char *p_sec_name);
	//BOOL(*fp_search_section_open)(UINT32 *p_search_handle);
	//BOOL(*fp_search_section)(UINT32 *p_search_handle);
	//ER(*fp_search_section_close)(void);
} CAL_RW_STORE_OP, *PCAL_RW_STORE_OP;

// for ipl calibration buf
#define CAL_BUF_ID_MAX 128
#define CAL_BUF_ITEM_MAX 256
/**
	CAL_BUF_ID_INIH_X:
	//////////////////////////////
	//        HEADER TAG        //
	//////////////////////////////
	//                          //
	//    INI WORKING BUFFER    //
	//       + INI HEADER       //
	//                          //
	//////////////////////////////
*/
typedef enum {
	CAL_BUF_ID_NONE = 0,

	// for preview
	CAL_BUF_ID_PRV_1 = 1,
	CAL_BUF_ID_PRV_2,
	CAL_BUF_ID_PRV_3,
	CAL_BUF_ID_PRV_4,
	CAL_BUF_ID_PRV_5,
	CAL_BUF_ID_PRV_6,
	CAL_BUF_ID_PRV_7,
	CAL_BUF_ID_PRV_8,
	CAL_BUF_ID_PRV_MAX,

	// for ini header
	CAL_BUF_ID_INIH_1 = 10,
	CAL_BUF_ID_INIH_2,
	CAL_BUF_ID_INIH_3,
	CAL_BUF_ID_INIH_MAX,

	// for ini data
	CAL_BUF_ID_INID_1 = 14,
	CAL_BUF_ID_INID_2,
	CAL_BUF_ID_INID_3,
	CAL_BUF_ID_INID_4,
	CAL_BUF_ID_INID_5,
	CAL_BUF_ID_INID_6,
	CAL_BUF_ID_INID_7,
	CAL_BUF_ID_INID_8,
	CAL_BUF_ID_INID_9,
	CAL_BUF_ID_INID_10,
	CAL_BUF_ID_INID_11,
	CAL_BUF_ID_INID_12,
	CAL_BUF_ID_INID_13,
	CAL_BUF_ID_INID_14,
	CAL_BUF_ID_INID_15,
	CAL_BUF_ID_INID_MAX,

	// for getraw
	CAL_BUF_ID_GETRAW_1 = 30,
	CAL_BUF_ID_GETRAW_2,
	CAL_BUF_ID_GETRAW_3,
	CAL_BUF_ID_GETRAW_4,
	CAL_BUF_ID_GETRAW_5,
	CAL_BUF_ID_GETRAW_6,
	CAL_BUF_ID_GETRAW_7,
	CAL_BUF_ID_GETRAW_8,
	CAL_BUF_ID_GETRAW_9,
	CAL_BUF_ID_GETRAW_10,
	CAL_BUF_ID_GETRAW_11,
	CAL_BUF_ID_GETRAW_12,
	CAL_BUF_ID_GETRAW_13,
	CAL_BUF_ID_GETRAW_14,
	CAL_BUF_ID_GETRAW_15,
	CAL_BUF_ID_GETRAW_MAX,

	// for getraw group
	CAL_BUF_ID_GETRAW_GROUP_1 = 46,
	CAL_BUF_ID_GETRAW_GROUP_2,
	CAL_BUF_ID_GETRAW_GROUP_3,
	CAL_BUF_ID_GETRAW_GROUP_4,
	CAL_BUF_ID_GETRAW_GROUP_5,
	CAL_BUF_ID_GETRAW_GROUP_6,
	CAL_BUF_ID_GETRAW_GROUP_7,
	CAL_BUF_ID_GETRAW_GROUP_8,
	CAL_BUF_ID_GETRAW_GROUP_9,
	CAL_BUF_ID_GETRAW_GROUP_10,
	CAL_BUF_ID_GETRAW_GROUP_11,
	CAL_BUF_ID_GETRAW_GROUP_12,
	CAL_BUF_ID_GETRAW_GROUP_13,
	CAL_BUF_ID_GETRAW_GROUP_14,
	CAL_BUF_ID_GETRAW_GROUP_15,
	CAL_BUF_ID_GETRAW_GROUP_MAX,

	// by project
	CAL_BUF_ID_OTHER_1 = 62,
	CAL_BUF_ID_OTHER_2,
	CAL_BUF_ID_OTHER_3,
	CAL_BUF_ID_OTHER_4,
	CAL_BUF_ID_OTHER_5,
	CAL_BUF_ID_OTHER_6,
	CAL_BUF_ID_OTHER_7,
	CAL_BUF_ID_OTHER_8,
	CAL_BUF_ID_OTHER_9,
	CAL_BUF_ID_OTHER_10,
	CAL_BUF_ID_OTHER_11,
	CAL_BUF_ID_OTHER_12,
	CAL_BUF_ID_OTHER_13,
	CAL_BUF_ID_OTHER_14,
	CAL_BUF_ID_OTHER_15,
	CAL_BUF_ID_OTHER_MAX,

	CAL_BUF_ID_MAX_NUM = CAL_BUF_ID_MAX,

	ENUM_DUMMY4WORD(CAL_BUF_ID)
} CAL_BUF_ID;


typedef enum {
	CAL_DATA_BUF_SEL_STORAGE_ONLY  		= 0, // only storage data buffer, the largest buffer in specify "id & cal_item_bit"
	CAL_DATA_BUF_SEL_ITEM_ALL 			= 1, // all sensor mode used buffer + all of storage data buffer, except specify "id & cal_item_bit"
//	CAL_DATA_BUF_SEL_ITEM_MAX 			= 2, // the largest buffer used in each sensor mode + all of storage data buffer, except specify "id & cal_item_bit"
	ENUM_DUMMY4WORD(CAL_DATA_BUF_SEL)
} CAL_DATA_BUF_SEL;

/**
    calibration debug information
*/
typedef enum {
	CAL_ER_NONE     = 0x00000000,   //< no error
	CAL_ER_3A       = 0x00000001,   //< cal_3a()
	CAL_ER_ALG      = 0x00000002,   //< cal_alg()
	CAL_ER_API      = 0x00000004,   //< cal_api()
	CAL_ER_BUF      = 0x00000008,   //< cal_buf()
	CAL_ER_CTRL     = 0x00000010,   //< cal_ctrl()
	CAL_ER_INFOR    = 0x00000020,   //< ipl_cal_infor.c
	CAL_ER_RW       = 0x00000040,   //< cal_rw()
	CAL_ER_PAR      = 0x00000080,   //< parameter
	CAL_ER_SYS      = 0x00000100,   //< system
	CAL_ER_IPL      = 0x00000200,   //< ipl
	CAL_ER_SEN      = 0x00000400,   //< sensor
	CAL_ER_SETTING  = 0x00000800,   //< ipl_cal_setting.c
	ENUM_DUMMY4WORD(CAL_ER_TAG)
} CAL_ER_TAG;

/**
    calibration project input data
*/
typedef struct {
	BOOL b_set;                                 ///< set INI header buffer & read INI header from storage
	CAL_BUF_ID buf_id;
	UINT32 buf_size; // total size, id/item/sensor mode/condition
} CAL_INIHEADER_INFO;

typedef struct {
	CAL_INIHEADER_INFO cal_iniheader_info;
} CAL_RW_INFO;

typedef struct {
	UINT32 buf_addr;                                        ///< input calibration total buffer start address
	UINT32 buf_size;                                        ///< input calibration total buffer size
	CAL_RW_STORE_OP *cal_rw_fp;                             ///< hook calibration r/w fp
} CAL_INIT_DATA;

typedef struct {
	//#NT#2017/08/01#Silvia Wu -begin
	//#NT# need to remove
	IPL_PROC_ID id;                                         ///< assign ipl id
	//#NT#2017/08/01#Silvia Wu -end
	UINT32 crop_ratio;                                      ///< sie crop ratio (H_ratio:bit[31:16], V_ratio:bit[15:0])
	UINT32 raw_out_buf_num;                                 ///< sie ch0 raw output buffer number, minimum : 3
	CAL_IME_PATH_EN ime_path_en;
	IPL_IME_PATH_INFO ime_path_info[CAL_IME_PATH_MAX_NUM];  ///< ime path information, path4 Y only
	UINT32 ime_ppb_num[CAL_IME_PATH_MAX_NUM];               ///< ime pingpong buffer num
	IPL_IME_OUT_GROUP ime_out_group[CAL_IME_PATH_MAX_NUM];
	IPL_POST_PROC_FUNC_EN func_en;                          ///< notify IPL post process enable & prepare buffer
	struct _CAL_PRV_DATA *pnext;                            ///< Link to next property description. (Fill NULL if this is last item)
} CAL_PRV_DATA;

typedef struct _CAL_GETRAW_DATA {
	IPL_PROC_ID id;                                         ///< assign ipl id
	CAL_ITEM cal_item;
	SENSOR_MODE sen_mode; ///< sensor mode
	struct _CAL_GETRAW_DATA *pnext;                            ///< Link to next property description. (Fill NULL if this is last item)
} CAL_GETRAW_DATA;


/*
    calibration internal parameter
*/

typedef struct {
	CAL_ITEM_BIT cal_item_bit; // delete storage data, and save new cal result to storage
} CAL_OPEN_PARAM;

typedef struct {
	UINT32 buf_addr;                                        ///< input buffer start address
	UINT32 buf_size;                                        ///< input buffer size (not include IME buffer)
	UINT32 crop_ratio;                                      ///< sie crop ratio (H_ratio:bit[31:16], V_ratio:bit[15:0])
	UINT32 raw_out_buf_num;                                 ///< sie ch0 raw output buffer number, minimum : 3
	CAL_IME_PATH_EN ime_path_en;
	IPL_IME_PATH_INFO ime_path_info[CAL_IME_PATH_MAX_NUM];  ///< ime path information, path4 Y only
	UINT32 ime_ppb_num[CAL_IME_PATH_MAX_NUM];               ///< ime pingpong buffer num
	IPL_IME_OUT_GROUP ime_out_group[CAL_IME_PATH_MAX_NUM];
	IPL_POST_PROC_FUNC_EN func_en;                          ///< notify IPL post process enable & prepare buffer
	IPL_ISR_EVENT_FP isr_cb_fp[IPL_ISR_MAX_CNT];            ///< ISR CB for photo mode
	IPL_OUT_BUF_CFG_CB out_buf_cfg_cb_fp;                   ///< get ipl output address callback function
	IPL_IME_P1_ENC_FMT ime_path1_enc_fmt;           ///< select H264 Encode or HEVC Encode
	UINT32 res[10];
	// reference IPL_CHGMODE_DATA
} CAL_PRV_PARAM;

typedef struct {
	UINT32 group;
	UINT32 order;
} CAL_SYNC_PARAM;

typedef struct {
	IPL_PROC_BIT_ID ipl_bit_id;
} CAL_MULTI_FRM_PARAM;

typedef struct {
	UINT32 dram0_addr;
} CAL_GETRAW_OUT;

typedef struct {
	IPLC_BASEINFO       *p_base_info;
	IPLC_EXT            *p_ext_info;
	IPL_ISR_OBJ_INFO    *p_isr_info;
	fpEngSubFunc        p_adapt_setting;
	IPLCTRL_FLOW        ipl_ctrl_flow;
	CAL_SYNC_PARAM      sync_param;
	CAL_MULTI_FRM_PARAM multi_frm_param;
	CAL_GETRAW_OUT		out;
	//#NT#2017/05/19#Silvia Wu -begin
	//#NT# need to add interface
	//CAL_AE_OBJ_INFO // for AE_Open Obj // NULL for default
	//#NT#2017/05/19#Silvia Wu -end
} CAL_GETRAW_CTRL_PARAM;

typedef struct {
	BOOL b_converge;
	UINT32 converge_lum; // 8 bits
	UINT32 gain_ratio[SEN_MFRAME_MAX_NUM]; // gain ratio (1.5x = 1.5 x 1000 = 1500)
	UINT32 exp_time[SEN_MFRAME_MAX_NUM]; // exposue time (us)
} CAL_AE_CONVERGE_RST;

typedef struct {
	CAL_ITEM cal_item;
	SENSOR_MODE sen_mode; ///< sensor mode
} CAL_GETRAW_PARAM;

typedef struct {
	IPL_RAW_IMG *raw_info;
	CAL_AE_CONVERGE_RST ae_converge_rst;
	CAL_ER_TAG rt;
} CAL_GETRAW_RST;

typedef struct {
	UINT32 total_size;
	UINT32 sie_ch_size[CAL_SIE_CH_MAX]; // for sie ch0~ch2
} CAL_GETRAW_BUF_SIZE;

typedef struct {
	IPL_PROC_ID id;
	CAL_ITEM cal_item;
	SENSOR_MODE sen_mode; ///< sensor mode
	CAL_COND cal_cond;
	UINT32 res[10];
} CAL_DATA_BUF_INFO;

typedef struct {
	CAL_ITEM cal_item;
	CAL_GETRAW_RST *getraw_rst;
	UINT32 res[10];
} CAL_ALG_PARAM;

typedef enum {
	CAL_STATE_NONE      = 0,
	CAL_STATE_OPEN      = 1,
	CAL_STATE_PRV       = 2,
	CAL_STATE_GETRAW    = 3,
	CAL_STATE_ALG       = 4,
	CAL_STATE_CLOSE     = 5,
	CAL_STATE_GETRST    = 6,
	CAL_STATE_MAX       = 7,
	ENUM_DUMMY4WORD(CAL_STATE)
} CAL_STATE;

// rw info
typedef struct {
	CAL_ITEM cal_item;
} HEADER_SEC;

typedef struct {
	IPL_PROC_ID id;
	SENSOR_MODE sensor_mode;
	CAL_COND cal_cond;
} HEADER_KEY;

typedef struct {
	HEADER_SEC sec;
	HEADER_KEY key;
} CAL_RW_DATA_INFO;

/**
    calibration setting information (for cal setting libext)
*/
typedef struct {
	UINT32 rgain; ///< rgain
	UINT32 bgain; ///< bgain
} AWB_GS;

typedef struct {
	UINT32 top;
	UINT32 bottom;
	UINT32 left;
	UINT32 right;
} DP_SKIP;

typedef struct {
	UINT32 threshold;   ///< DP detect threshold
	UINT32 block_x;     ///< DP search block X,(must be even,because of bayer fmt)
	UINT32 block_y;     ///< DP search block Y,(must be even,because of bayer fmt)
	DP_SKIP skip_search;
} DP_SETTING;

typedef struct {
	UINT32 gain_ratio;
} ISO_GS;

typedef struct {
	AWB_GS gs;          // golden sample value
	UINT32 threshold;   // awb cal threshold, range = 0~100 (%);
	UINT32 win_num_x;
	UINT32 win_num_y;
} CAL_ALG_AWB_SETTING;

typedef struct {
	UINT32 max_cnt_ratio[SENSOR_MODE_Num - 1]; // max_cnt = height*width*max_cnt_ratio, range = 0~10000 (1/10000)
	DP_SETTING bright_set;
	DP_SETTING dark_set;
} CAL_ALG_DP_SETTING;

typedef struct {
	UINT32          compen_ratio;    //ECS Compensation ratio, range = 0~100 (%)
	ECSMAP_ENUM     map_size;        //ECS color table size select
	ECS_BitRatio    bit_ratio;       //d2f8 or d3f7
	BOOL            *user_valid_tbl; // user define ecs valid tbl addr, assgin 0 for valid all
} CAL_ALG_ECS_SETTING;

typedef struct {
	ISO_GS gs;          // golden sample value
	UINT32 threshold;   // threshold, range = 0~100 (%);
} CAL_ALG_ISO_SETTING;

typedef struct {
	UINT32  min_reliability;     // Minimum Reliability (success rate), range = 0~100 (%);
	UINT32  valid_radius;        // in pixel units
	UINT32  scan_radius;         // in pixel units
	UINT32  avg_cnt;             // in pixel units (noise filter)
	UINT32  edge_thr;            // edge threshold, range = 0~100 (%);
	iCoordinate scan_start;      // width/height * scan_start, 50% for center of the raw, range = 0~100 (%);
} CAL_ALG_LENSCEN_SETTING;

typedef enum {
	CAL_AE_ADJ_NONE         = 0x00000000,
	CAL_AE_ADJ_GAINRATIO    = 0x00000001, // noise issue
	CAL_AE_ADJ_EXPT         = 0x00000002, // ficker issue
	ENUM_DUMMY4WORD(CAL_AE_ADJ)
} CAL_AE_ADJ;

#define CAL_AE_CONV_STEP_NUM 10
typedef struct {
	INT32 lum_diff[CAL_AE_CONV_STEP_NUM]; // must be incremented, the difference lum between current and target (units: lum, range: (-target_lum) ~ (255-target_lum))
	INT32 ratio[CAL_AE_CONV_STEP_NUM]; // coverge method adjust ratio (units: %, range: 0~100)
} CAL_AE_CONV_STEP;

typedef struct {
	BOOL b_sft_ratio;
	SENSOR_MODE sen_mode;
	UINT32 ratio; // convert ae param. sft ratio, unit:%, 100%: ori
} CAL_AE_SFT_RATIO;

typedef struct _CAL_AE_CONVERGE_PARAM {
	UINT32 gain_ratio;  ///< gain (1X = 1 x 1000 = 1000)
	UINT32 exp_time;    ///< us
	UINT32 target_lum;  ///< 8 bits
	UINT32 tolerance;   ///< target_lum converge tolerance (units: lum, 8bits)
	UINT32 times;
	CAL_AE_ADJ adjust;
	CAL_AE_CONV_STEP coverge_step;
	BOOL *valid_win;    // table BOOL[CAL_CA_WIN_X*CAL_CA_WIN_Y]
	CAL_AE_SFT_RATIO sft_ratio[SENSOR_MODE_Num];
	UINT32 res[10];
	struct _CAL_AE_CONVERGE_PARAM *pnext;    ///< Link to next property description. (Fill NULL if this is last item)
} CAL_AE_CONVERGE_PARAM;

typedef struct {
	void *alg;
	CAL_AE_CONVERGE_PARAM *ae_converge_param;
	SENSOR_MODE sen_mode[SENSOR_MODE_Num];  // assign SENSOR_MODE_UNKNOWN in the last value
} CAL_SETTING_INFO;

typedef struct {
	BOOL ob_auto_en;
	UINT32 sub_ratio_x;
	UINT32 avg_act_thres;
	UINT32 sub_ratio;
} OB_AUTO;

typedef struct {
	OB_AUTO *ob_auto;
	UINT32 ob_offset;
} OB_INFO;

typedef struct {
	BOOL ca_en;
	URECT crp[SENSOR_MODE_Num]; ///< set 0 for auto
//	UINT32 win_num_x; // fix CAL_CA_WIN_X
//	UINT32 win_num_y; // fix CAL_CA_WIN_Y
	BOOL ca_thr_en;
	THRESHOL_SET r_thr;
	THRESHOL_SET g_thr;
	THRESHOL_SET b_thr;
	THRESHOL_SET p_thr;
	BOOL b_vig;
} CA_INFO;

typedef struct {
	BOOL la_en;
	URECT crp[SENSOR_MODE_Num]; ///< set 0 for auto
//	UINT32 win_num_x; // fix CAL_LA_WIN_X
//	UINT32 win_num_y; // fix CAL_LA_WIN_Y
	BOOL b_la_cg;
	BOOL b_la_gama1;
	SIE_STATS_HISTO_SRC_SEL b_histogram_sel;
	SIE_STATS_LA1_SRC_SEL b_la1_src_sel;
} LA_INFO;

typedef struct {
	UINT32 sie_ppb_num; // for ch0/ch1/ch2
	OB_INFO *ob_info;
	CA_INFO *ca_info;
	LA_INFO *la_info;
} CAL_CTRL_SETTING_SIE;

typedef struct {
	IPL_PROC_BIT_ID bit_id;
	UPOINT block;
} CAL_CTRL_GROUP;

typedef struct {
	CAL_CTRL_SETTING_SIE *sie;
	CAL_CTRL_GROUP *group;
} CAL_CTRL_SETTING_INFO;

typedef struct {
	CAL_SETTING_INFO *(*fp_get_setting_info)(CAL_ITEM cal_item);
	CAL_CTRL_SETTING_INFO *(*fp_get_ctrl_info)(void); // only for getraw
} CAL_EXT_SETTING_OBJ, *pCAL_EXT_SETTING_OBJ;

/**
    calibration ctrl information (for cal ctrl libext)
*/
typedef struct {
	IPL_PROC_ID id;
	UINT32 param[10];
} CAL_GET_BUF_ID_INFO;

typedef struct {
	// getraw
	ER(*fp_get_getraw_ctrl_param)(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_CTRL_PARAM *getraw_ctrl_param);
	ER(*fp_get_getraw_buf_size)(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_BUF_SIZE *getraw_buf_size);
	ER(*fp_set_getraw_buf_item)(CAL_GET_BUF_ID_INFO buf_id_info, CAL_CTRL_SETTING_INFO *ctrl_setting_info, UINT32 sie_buf_addr, CAL_GETRAW_BUF_SIZE getraw_buf_size);

	// prv
	ER(*fp_get_prv_buf_size)(IPL_PROC_ID id, CAL_PRV_PARAM *prv_param, UINT32 *ipl_buf_size, UINT32 *ime_buf_size);
	IPL_ISR_EVENT_FP(*fp_get_prv_isr_event_fp)(IPL_ISR ipl_isr);
	IPL_OUT_BUF_CFG_CB(*fp_get_prv_output_buf_cfg_fp)(IPL_PROC_ID id, IPL_HAL_ENG ipl_hal_eng);

	// r/w
	UINT32(*fp_get_inidata_buf)(CAL_RW_DATA_INFO *data_info);

	// alg
	CAL_STATUS(*fp_chk_alg_rst)(IPL_PROC_ID id, SENSOR_MODE mode, CAL_ITEM cal_item, void *cal_rst, void *alg);

	// close
	void(*fp_display_close)(IPL_PROC_ID id);

} CAL_EXT_CTRL_OBJ, *pCAL_EXT_CTRL_OBJ;

/**
    calibration debug information
*/
extern CHAR *cal_dbg_str_calitem(CAL_ITEM cal_item);
extern CHAR *cal_dbg_str_calstate(CAL_STATE cal_state);
extern CHAR *cal_dbg_str_calstatus(CAL_STATUS cal_status);
extern void cal_dbg_saveraw(UINT32 addr, UINT32 width, UINT32 height, IPL_PROC_ID id, CAL_ITEM cal_item, SENSOR_MODE sensor_mode);
#if 1
extern void cal_dump_all(void (*dump)(char *fmt, ...));
#else
extern void cal_dump_header_info(CAL_RW_HEADER_INFO rw_header_info);
extern void cal_dump_info(IPL_PROC_ID id);
extern void cal_dump_buf(void (*dump)(char *fmt, ...));
#endif
#endif
