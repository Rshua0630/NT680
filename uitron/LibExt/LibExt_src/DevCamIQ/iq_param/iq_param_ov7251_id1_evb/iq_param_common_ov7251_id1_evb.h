#ifndef _IQ_PARAM_COMMON_OV7251_ID1_EVB_H
#define _IQ_PARAM_COMMON_OV7251_ID1_EVB_H

#include "iq_common.h"

extern ISOMAP iso_map_tab_ov7251_id1_evb[IQS_ISOMAPTAB_NUM] ;
extern DIST dist_ov7251_id1_evb;
extern Fact_Norm   radious_ov7251_id1_evb ;
extern DCE_CFAPARAM cfa_ov7251_id1_evb;
extern FOV fov_ov7251_id1_evb;
extern UINT32 ldc_g_lut_ov7251_id1_evb[65] ;
extern INT32 ldc_r_lut_ov7251_id1_evb[65] ;
extern INT32 ldc_b_lut_ov7251_id1_evb[65] ;
extern Aberation cac_ov7251_id1_evb;
extern IQS_COLOR_TEMPERATURE_TH color_temperature_th_ov7251_id1_evb ;
extern IQS_COLOR_TEMPERATURE_PARAM color_low_ov7251_id1_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_middle_ov7251_id1_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_high_ov7251_id1_evb;
extern UINT8 fs_tab_ov7251_id1_evb[TOTAL_GAIN_NUM][16];
extern UINT8 fd_tab_ov7251_id1_evb[TOTAL_GAIN_NUM][16];
extern IPE_CC cc_ov7251_id1_evb;
extern UINT32 y_curve_ov7251_id1_evb[129];
extern IQS_GAMMA_LV_TH gamma_lv_th_ov7251_id1_evb;
extern UINT32 gamma_day_ov7251_id1_evb[129];
extern UINT32 gamma_night_ov7251_id1_evb[129];
extern UINT8 edg_tab_ov7251_id1_evb[24];
extern UINT8 dds_tab_ov7251_id1_evb[8] ;
extern INT16 cst_coef_ov7251_id1_evb[9] ;
extern IPE_CST cst_ov7251_id1_evb;
extern IPE_YCCFIX ycc_fix_ov7251_id1_evb;
extern IPE_RAND_NR noise_param_ov7251_id1_evb;
extern IPE_GamRand gamma_rand_ov7251_id1_evb ;
extern UINT32 color_3d_lut_ov7251_id1_evb[900] ;
extern IPE_3DCCRound color_3d_ov7251_id1_evb;
extern IPL_STRP_INFO strp_info_ov7251_id1_evb[4];
extern _2DLUT_PARAM dc_2d_param_ov7251_id1_evb[4];
extern UINT32 dc_2dlut00_ov7251_id1_evb[65*65];
extern UINT32 dc_2dlut01_ov7251_id1_evb[65*65];
extern UINT32 dc_2dlut02_ov7251_id1_evb[65*65];
extern UINT32 dc_2dlut03_ov7251_id1_evb[65*65];
extern SHDR_IQ_PARA hdr_ov7251_id1_evb;
extern S_DEFOG_LIB_INIT_PARA defog_init_param_ov7251_id1_evb;
extern UINT32 defog_wet_ov7251_id1_evb[17];

void iq_param_ov7251_id1_evb_table1_init(UINT32 id,UINT32 mode_idx);
//void iq_param_ov7251_id1_evb_mode2_init(UINT32 id,UINT32 mode_idx);
//void iq_param_ov7251_id1_evb_mode3_init(UINT32 id,UINT32 mode_idx);
//void iq_param_ov7251_id1_evb_mode4_init(UINT32 id,UINT32 mode_idx);
//void iq_param_ov7251_id1_evb_mode5_init(UINT32 id,UINT32 mode_idx);
//void iq_param_ov7251_id1_evb_mode6_init(UINT32 id,UINT32 mode_idx);

#endif
