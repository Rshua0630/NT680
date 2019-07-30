/**
    Algorithm header file for AWB.

    This file is the header file that defines the API and data type for AWB algorithm.

    @file       awb_alg.h
    @ingroup    Lib\AWBAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _AWB_ALG_H_
#define _AWB_ALG_H_

#include "awb_task.h"

/******************************/
/**
    AWB Alg type
*/
typedef enum _AWBALG_MODE {
	AWBALG_METHOD1 = 0,
	ENUM_DUMMY4WORD(AWBALG_MODE)
} AWBALG_MODE;

/**********************************************************************/

/******************************/
/**
    AWB Alg AppMode.
*/
typedef enum _AWB_APPMODE {
	AWB_APPMODE_NORMAL = 0,
	AWB_APPMODE_STITCH,
	AWB_APPMODE_SHDR,
	AWB_APPMODE_END = 0xffffffff,
	ENUM_DUMMY4WORD(AWB_APPMODE)
} AWB_APPMODE;

/**********************************************************************/

/******************************/
/**
    AWB Alg Status
*/
typedef enum _AWBALG_STATUS {
	AWBALG_STATUS_INIT = 0,
	AWBALG_STATUS_ADJUST,
	AWBALG_STATUS_FREEZE,
	AWBALG_STATUS_CONVERGE,
	AWBALG_STATUS_MANUAL,
	AWBALG_STATUS_EXPAND_CT,
	AWBALG_STATUS_NIGHTMODE,
	AWBALG_STATUS_END = 0xffffffff,
	ENUM_DUMMY4WORD(AWBALG_STATUS)
} AWBALG_STATUS;

/******************************/
/**
    AWB alg default Kgain
*/
#define AWB_BASE_KGAIN      100
/**********************************************************************/

/******************************/
/**
    AWB alg CA definition
*/
#define AWB_ACC_BIT         12   //Define CA ACC Bit Cnt
#define AWB_CABUFFER_SIZE   1024
/**********************************************************************/

/******************************/
/**
    AWB Alg channel type
*/
typedef enum _AWBALG_CH {
	AWBALG_CH_R = 0,
	AWBALG_CH_G,
	AWBALG_CH_B,
	AWBALG_CH_IR,
	AWBALG_CH_Rth,
	AWBALG_CH_Gth,
	AWBALG_CH_Bth,
	AWBALG_CH_MAX,
	ENUM_DUMMY4WORD(AWBALG_CH)
} AWBALG_CH;
/**********************************************************************/

/**
    AWB Alg CT gain
*/
typedef struct _AWBALG_GAIN {
	UINT32 r_gain;
	UINT32 g_gain;
	UINT32 b_gain;
} AWBALG_CT_GAIN;
/**********************************************************************/

/**
    AWB Alg check white type
*/
typedef enum _AWBALG_ER {
	AWBALG_FAIL = 0,
	AWBALG_OK = 1,
	AWBALG_YL = 10,
	AWBALG_YU = 15,
	AWBALG_RpBL = 20,
	AWBALG_RpBU = 25,
	AWBALG_RsBL = 30,
	AWBALG_RsBU = 35,
	AWBALG_R2GL = 40,
	AWBALG_R2GU = 45,
	AWBALG_B2GL = 50,
	AWBALG_B2GU = 55,
	AWBALG_RmBL = 60,
	AWBALG_RmBU = 65,
	AWBALG_REMOVE_EXPAND = 70,
	AWBALG_ADD_EXPAND = 80,
	AWBALG_MAX,
	ENUM_DUMMY4WORD(AWBALG_ER)
} AWBALG_ER;
/**********************************************************************/

/******************************/
/**
    AWB ca info definition
*/
typedef struct _awb_ca_info {
	UINT32 WinNumX;
	UINT32 WinNumY;
	UINT32 RGu;
	UINT32 RGl;
	UINT32 GGu;
	UINT32 GGl;
	UINT32 BGu;
	UINT32 BGl;
	UINT32 RBGu;
	UINT32 RBGl;
} awb_ca_info;

#define AWB_WIN 32

/**********************************************************************/
/**
     AWB th_m1.

     @note
*/
typedef struct _awb_th {
	INT32 YL;
	INT32 YU;
	INT32 RpBL;
	INT32 RpBU;
	INT32 RsBL;
	INT32 RsBU;
	INT32 R2GL;
	INT32 R2GU;
	INT32 B2GL;
	INT32 B2GU;
	INT32 RmBL;
	INT32 RmBU;
} awb_th;

/**
    AWB Alg LV check
*/
typedef struct _awb_lv_check {
	UINT32 lv_h;
	UINT32 lv_l;
} awb_lv_check;

/**
     AWB CT weight.
*/
typedef struct _ctmp_lut {
	INT32 cx;
	INT32 ctmp;
} ctmp_lut;

typedef struct _cwgt_lut {
	INT32 ctmp;
	INT32 weight;
} cwgt_lut;

typedef struct _awb_ct_weight {
	ctmp_lut *ctmp;
    cwgt_lut *out;
	cwgt_lut *in;
	cwgt_lut *night;
} awb_ct_weight;
/**
     AWB target.
*/
typedef struct _awb_target {
	INT32 cx;
	INT32 rg_ratio;
	INT32 bg_ratio;
} awb_target;

/**
    AWB Alg CT table
*/
typedef struct _awb_ct_info {
	UINT32 temperature;
	AWBALG_CT_GAIN gain;
} awb_ct_info;

/**
    AWB Manual WB Gain
*/
typedef struct _awb_mwb_gain {
	UINT32 r_gain;
	UINT32 g_gain;
	UINT32 b_gain;
} awb_mwb_gain;

/**
    AWB Converge
*/
typedef struct _awb_converge {
	UINT32 skip_frame;
	UINT32 speed;
	UINT32 tolerance;
} awb_converge;

/**
    AWB Expand Block
*/
typedef struct _awb_expand_block {
	UINT32 mode; //0:disable, 1:add, 2:remove
	UINT32 lv_l;
	UINT32 lv_h;
	INT32 y_l;
	INT32 y_u;
	INT32 rpb_l;
	INT32 rpb_u;
	INT32 rsb_l;
	INT32 rsb_u;
} awb_expand_block;

/******************************/
/**
    AWB tool
*/
#define AWB_TUNING_CHECK_LV_MAX 3
#define AWB_TUNING_CT_WEIGHT_MAX 6
#define AWB_TUNING_TARGET_MAX 3
#define AWB_TUNING_CT_MAX 6
#define AWB_TUNING_MWB_MAX 12
#define AWB_TUNING_BLOCK_MAX 4

typedef struct _awb_tuning_param {
    UINT32 id;
    UINT32 size;
    awb_th th;
    awb_ca_info ca_info;
    awb_lv_check lv_check[AWB_TUNING_CHECK_LV_MAX];
    ctmp_lut ct_map[AWB_TUNING_CT_WEIGHT_MAX];
    cwgt_lut out_cwgt_lut[AWB_TUNING_CT_WEIGHT_MAX];
    cwgt_lut in_cwgt_lut[AWB_TUNING_CT_WEIGHT_MAX];
    cwgt_lut night_cwgt_lut[AWB_TUNING_CT_WEIGHT_MAX];
    awb_target target[AWB_TUNING_TARGET_MAX];
    awb_ct_info ct_tab[AWB_TUNING_CT_MAX];
    awb_mwb_gain mwb_tab[AWB_TUNING_MWB_MAX];
    awb_expand_block expand_block[AWB_TUNING_BLOCK_MAX];
} awb_tuning_param;

typedef struct _awb_tuning_converge{
    UINT32 id;
    UINT32 size;
    UINT32 skip_frame;
    UINT32 speed;
    UINT32 tolerance;
} awb_tuning_converge;

/**
    AWB Alg stable table
*/
typedef struct _AWBALG_STABLE_GAIN {
	UINT32 r_gain;
	UINT32 g_gain;
	UINT32 b_gain;
	UINT32 score;
	UINT32 tolerance;   ///< 256 is 100%
} AWBALG_STABLE_GAIN;

/**
    AWB Parameter
*/
typedef struct _awb_param {
	awb_ca_info    *ca_info;
	awb_th         *th;
	awb_lv_check   *lvcheck;
    awb_ct_weight  *ct_weight;
    awb_target     *target;
	awb_ct_info    *ct_table;
	awb_mwb_gain   *mwb_tab;
    awb_converge   *converge;
    awb_expand_block *expand_block;
} awb_param;

/**
     AWB algorithm event.
     @note
*/

#define AWB_KGAIN_MAPTAB_SIZE 64

typedef struct _AWBALG_KGAIN {
	UINT32 gs_r_gain;  //awb gain of golden sample
	UINT32 gs_b_gain;
	UINT32 k_r_gain; //calibration gain of test sample
	UINT32 k_b_gain;
	UINT32 r_ratio;
	UINT32 b_ratio;
} AWBALG_KGAIN;

typedef struct _awb_keep_wb {
    INT32 en;
    UINT8 ct_number;
} awb_keep_wb;

/**
     AWB input information.

     @note
*/
typedef struct _AWBALG_INFO {
	UINT32 Id;
	AWBALG_MODE AwbAlgMode;
    UINT32 awb_single_mode;
    AWBALG_STATUS status;
	UINT32 CurLV;
	UINT32 CurCT;
	UINT32 CurCx;
	UINT32 WinNumX;
	UINT32 WinNumY;
    UINT16 acc_tab_type;
	UINT16 *AccTab[AWBALG_CH_MAX];
	UINT16 *AccCnt;
	UINT16 *CalTab[AWBALG_CH_MAX];
	UINT32 MinWTotal;
	UINT16 *FlagTab;
	AWBALG_STABLE_GAIN StableGain;

	AWBALG_CT_GAIN DefGain;
	AWBALG_CT_GAIN PreGain;
	AWBALG_CT_GAIN AWBGain;
    AWBALG_CT_GAIN cap_gain;
	AWBALG_KGAIN k_gain;
	UINT32 AwbConvSkipFrame;
	UINT32 AwbConvSpeed;
    UINT32 AwbUserRratio;
    UINT32 AwbUserBratio;
	UINT32 AWBCnt;
	UINT32 AWBEnterCnt;

	BOOL AwbWeightEn;

    UINT32 ob;
    awb_keep_wb keep_wb;
    UINT8 expand_ct_en;
    UINT8 multi_ct_check_en;
    UINT32 multi_ct_check_min_sat;
    UINT8 saturation;
} AWBALG_INFO;

extern AWBALG_ER AWBALG_Sim(AWBALG_INFO *pAWBAlgInfo, awb_param *m1_par, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain);

extern void AWB_SetAlgMode(AWBALG_MODE mode);
extern AWBALG_MODE AWB_GetAlgMode(void);

/******************************/
/**
    AWB Tuning API
*/

/**
    AWB Tuning set item
*/
typedef enum _AWB_TUNING_SET_ITEM {
    AWB_TUNING_SET_MANUAL= 0,
	AWB_TUNING_SET_PARAM,
	AWB_TUNING_SET_CONVERGE,
	AWB_TUNING_SET_ALL,
	AWB_TUNING_SET_MAX
} AWB_TUNING_SET_ITEM;

/**
    AWB Tuning get item
*/
typedef enum _AWB_TUNING_GET_ITEM {
	AWB_TUNING_GET_MANUAL  =   0,
	AWB_TUNING_GET_PARAM,
	AWB_TUNING_GET_STATUS,
	AWB_TUNING_GET_CA,
	AWB_TUNING_GET_FLAG,
	AWB_TUNING_GET_CONVERGE,
	AWB_TUNING_GET_ALL,
	AWB_TUNING_GET_MAX
} AWB_TUNING_GET_ITEM;

/**
    AWB Tuning update bit
*/
typedef enum _AWB_TUNING_UPDATE {
	AWB_TUNING_UPDATE_MANUAL        = FLGPTN_BIT(AWB_TUNING_SET_MANUAL),
	AWB_TUNING_UPDATE_PARAM         = FLGPTN_BIT(AWB_TUNING_SET_PARAM),
	AWB_TUNING_UPDATE_CONVERGE      = FLGPTN_BIT(AWB_TUNING_SET_CONVERGE),
	AWB_TUNING_UPDATE_ALL           = FLGPTN_BIT_ALL
} AWB_TUNING_UPDATE;

/**
    AWB Tuning API
*/
extern UINT32 AWBTuning_Set_Cmd(UINT32 Id, AWB_TUNING_SET_ITEM item, UINT32 addr, UINT32 size);
extern UINT32 AWBTuning_Get_Cmd(UINT32 Id, AWB_TUNING_GET_ITEM item, UINT32 addr, UINT32 size);
/**********************************************************************/

/******************************/
/**
    AWB Tuning parameter structure
*/

/**
    AWB Tuning Current Status
*/
typedef struct _AWB_TUNING_STATUS {
    UINT32 id;
    UINT32 size;
	UINT32 curRgain;
	UINT32 curBgain;
	UINT32 curGgain;
	UINT32 curCT;
	UINT32 curCx;
	UINT32 mode;
	UINT32 reserved[4];
} AWB_TUNING_STATUS;

/**
    AWB Tuning Manual Gain
*/
typedef struct _AWB_TUNING_MANUAL {
    UINT32 id;
    UINT32 size;
	UINT32 en;
	UINT32 Rgain;
	UINT32 Bgain;
	UINT32 Ggain;
    UINT32 user_rratio;
    UINT32 user_bratio;
} AWB_TUNING_MANUAL;

/**
    AWB Tuning CA Info
*/
typedef struct _AWB_TUNING_CA {
    UINT32 id;
    UINT32 size;
	UINT32 winNum_x;
	UINT32 winNum_y;
	UINT16 tab[AWBALG_CH_MAX][32][32];
} AWB_TUNING_CA;

/**
    AWB Tuning Flag Info
*/
typedef struct _AWB_TUNING_FLAG {
    UINT32 id;
    UINT32 size;
	UINT32 winNum_x;
	UINT32 winNum_y;
	UINT16 tab[32][32];
} AWB_TUNING_FLAG;

/**
    AWB Tuning all table
*/
typedef struct _AWB_TUNING_ALL {
	// get and set
	AWB_TUNING_MANUAL manual;
	awb_tuning_param param;
    awb_tuning_converge converge;

	// get only
	AWB_TUNING_STATUS status;
	AWB_TUNING_CA ca;
	AWB_TUNING_FLAG flag;
} AWB_TUNING_ALL;
/**********************************************************************/

/******************************/
/**
    Internel AWB Tuning App for LibExt
*/
void AWBTuning_getPtr(UINT32 Id, UINT32 addr);
void AWBTuning_setInit(UINT32 Id, UINT32 addr, UINT32 size);
void AWBTuning_getUpdate(UINT32 Id, UINT32 addr);
void AWBTuning_setUpdate(UINT32 Id, UINT32 up);
INT16 AWBTuning_ev2lv(UINT32 ev);
UINT32 AWBTuning_lv2ev(INT16 lv);
void AWBTuning_setAWBInfo(UINT32 Id, AWBALG_INFO *awbInfo);
/**********************************************************************/

/******************************/
/**
    AWB Common definition
*/

typedef struct _AWB_PARAM_Pr {
	AWBALG_INFO*             Now_AWB_Info;
    awb_param*               Now_AWB_Par;
} AWB_PARAM_Pr;

typedef UINT32(*CA_DUMP_FP)(UINT32 Id);
extern void CA_DumpFP(CA_DUMP_FP fp);

typedef UINT32(*Flag_DUMP_FP)(UINT32 Id);
extern void Flag_DumpFP(Flag_DUMP_FP fp);

extern UINT32 AWB_DumpCAInfo(UINT32 id);
extern UINT32 AWB_DumpFlagInfo(UINT32 id);

void awb_set_addr(UINT32 id, AWB_PARAM_Pr awb_param_addr);
void awb_get_addr(UINT32 id, AWB_PARAM_Pr* awb_param_addr);

/**
     AWB ca type
*/
typedef enum _AWB_CA_TYPE {
	AWB_CA_TYPE_NORMAL = 0,
	AWB_CA_TYPE_THRESHOLD,
	AWB_CA_TYPE_MAX,
	ENUM_DUMMY4WORD(AWB_CA_TYPE)
} AWB_CA_TYPE;

/**
    Color temperature calculation type
*/
typedef enum _AWBALG_CT_CALMODE {
	AWBALG_CT_CAL_R_GAIN,
	AWBALG_CT_CAL_B_GAIN,
	AWBALG_CT_CAL_RB_GAIN,
	AWBALG_CT_CAL_Cx,
	ENUM_DUMMY4WORD(AWBALG_CT_CALMODE)
} AWBALG_CT_CALMODE;

/**
    Color temperature range
*/
typedef enum _AWB_CT_RANGE {
	AWB_CT_RANGE_NONE   = 0x00000000,     ///< not in range
	AWB_CT_RANGE_1      = 0x00000001,     ///< awb ct range 1
	AWB_CT_RANGE_2      = 0x00000002,     ///< awb ct range 2
	AWB_CT_RANGE_3      = 0x00000004,     ///< awb ct range 3
	AWB_CT_RANGE_4      = 0x00000008,     ///< awb ct range 4
	AWB_CT_RANGE_5      = 0x00000010,     ///< awb ct range 5
	AWB_CT_RANGE_MAX    = 6,
	ENUM_DUMMY4WORD(AWB_CT_RANGE)
} AWB_CT_RANGE;

/**
    Color temperature calculation API
*/
void AWB_GetCurCT_R(UINT32 id, UINT32 Rgain, UINT32 Bgain, UINT32 *CT);
void AWB_GetCurCT_B(UINT32 id, UINT32 Rgain, UINT32 Bgain, UINT32 *CT);
void AWB_GetCurCT_RB(UINT32 id, UINT32 Rgain, UINT32 Bgain, UINT32 *CT);
void AWB_GetCurCT_Cx(UINT32 id, UINT32 Rgain, UINT32 Ggain, UINT32 Bgain, UINT32 *CT);
void AWB_GetCurCT(UINT32 id, AWBALG_CT_CALMODE CTCalMode, UINT32 CurRgain, UINT32 CurGgain, UINT32 CurBgain, UINT32 *CT);
/**********************************************************************/
#endif //_AWB_ALG_H_
