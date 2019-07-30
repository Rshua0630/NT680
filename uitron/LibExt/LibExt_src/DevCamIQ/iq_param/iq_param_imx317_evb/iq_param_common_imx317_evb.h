#ifndef _IQ_PARAM_COMMON_IMX317_EVB_H
#define _IQ_PARAM_COMMON_IMX317_EVB_H

#include "iq_common.h"

extern ISOMAP iso_map_tab_imx317_evb[IQS_ISOMAPTAB_NUM] ;
extern DIST dist_imx317_evb;
extern Fact_Norm   radious_imx317_evb ;
extern DCE_CFAPARAM cfa_imx317_evb;
extern FOV fov_imx317_evb;
extern UINT32 ldc_g_lut_imx317_evb[65] ;
extern INT32 ldc_r_lut_imx317_evb[65] ;
extern INT32 ldc_b_lut_imx317_evb[65] ;
extern Aberation cac_imx317_evb;
extern IQS_COLOR_TEMPERATURE_TH color_temperature_th_imx317_evb ;
extern IQS_COLOR_TEMPERATURE_PARAM color_low_imx317_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_middle_imx317_evb;
extern IQS_COLOR_TEMPERATURE_PARAM color_high_imx317_evb;
extern UINT8 fs_tab_imx317_evb[TOTAL_GAIN_NUM][16];
extern UINT8 fd_tab_imx317_evb[TOTAL_GAIN_NUM][16];
extern IPE_CC cc_imx317_evb;
extern UINT32 y_curve_imx317_evb[129];
extern IQS_GAMMA_LV_TH gamma_lv_th_imx317_evb;
extern UINT32 gamma_day_imx317_evb[129];
extern UINT32 gamma_night_imx317_evb[129];
extern UINT8 edg_tab_imx317_evb[24];
extern UINT8 dds_tab_imx317_evb[8] ;
extern INT16 cst_coef_imx317_evb[9] ;
extern IPE_CST cst_imx317_evb;
extern IPE_YCCFIX ycc_fix_imx317_evb;
extern IPE_RAND_NR noise_param_imx317_evb;
extern IPE_GamRand gamma_rand_imx317_evb ;
extern UINT32 color_3d_lut_imx317_evb[900] ;
extern IPE_3DCCRound color_3d_imx317_evb;
extern IPL_STRP_INFO strp_info_imx317_evb[4];
extern _2DLUT_PARAM dc_2d_param_imx317_evb[4];
extern UINT32 dc_2dlut00_imx317_evb[65*65];
extern UINT32 dc_2dlut01_imx317_evb[65*65];
extern UINT32 dc_2dlut02_imx317_evb[65*65];
extern UINT32 dc_2dlut03_imx317_evb[65*65];
extern SHDR_IQ_PARA hdr_imx317_evb;
extern S_DEFOG_LIB_INIT_PARA defog_init_param_imx317_evb;
extern UINT32 defog_wet_imx317_evb[17];

void iq_param_imx317_evb_table1_init(UINT32 id,UINT32 mode_idx);
void iq_param_imx317_evb_table2_init(UINT32 id,UINT32 mode_idx);
//void iq_param_imx317_evb_mode3_init(UINT32 id,UINT32 mode_idx);
//void iq_param_imx317_evb_mode4_init(UINT32 id,UINT32 mode_idx);
//void iq_param_imx317_evb_mode5_init(UINT32 id,UINT32 mode_idx);
//void iq_param_imx317_evb_mode6_init(UINT32 id,UINT32 mode_idx);

#endif
