#ifndef _IQ_TUNING_H_
#define _IQ_TUNING_H_

extern void iq_intpl_iso(UINT32 Id, IQS_FLOW_MSG MsgID);

////////////////////////////////
//             SIE             /
////////////////////////////////
//**  SIE-ECS
extern UINT32 g_ecs_tab_buf[Total_SENIDNum][4225];
extern SIE_EmbeddedCS g_ecs_buf[Total_SENIDNum];

extern SIE_CA_VIG g_ca_vig_buf[Total_SENIDNum];

////////////////////////////////
//             RHE             /
////////////////////////////////
//**  RHE-CGain
extern UINT32 g_rhe_ob_buf[Total_SENIDNum][4];
extern RHE_HAL_COLOR_GAIN_INFO g_rhe_color_gain_buf[Total_SENIDNum];

//**  RHE-WDR
extern UINT8 g_wdr_strength_buf[Total_SENIDNum];

//**  RHE-LCE
extern IQS_LCE g_lce_strength_buf[Total_SENIDNum];
extern UINT32 g_lce_dif_gain[Total_SENIDNum][17];
extern UINT32 g_lce_luma_gain[Total_SENIDNum][17];
extern RHE_HAL_LTC_PARAM g_lce_buf[Total_SENIDNum];

//**  RHE-GCE
extern SR_WDR_GCE_PARAM g_gce_strength_buf[Total_SENIDNum];

////////////////////////////////
//             IFE             /
////////////////////////////////
//**  IFE-Outlier
extern UINT32 g_outl_bri_buf[Total_SENIDNum][5];
extern UINT32 g_outl_dark_buf[Total_SENIDNum][5];
extern UINT32 g_outl_cnt_buf[Total_SENIDNum][2];
extern IFE_OUTL g_outl_set_buf[Total_SENIDNum];

//**  IFE-Spatial
extern UINT32 g_spatial_w_buf[Total_SENIDNum][10];
extern Spatial_Filter g_spatial_buf[Total_SENIDNum];

//**  IFE-NLM
extern UINT8 g_range_ker_radius_buf[Total_SENIDNum][8];
extern IFE_HAL_NLM_KER g_range_ker_buf[Total_SENIDNum];
extern UINT8 g_range_mwth_buf[Total_SENIDNum][2];
extern UINT8 g_range_bwa_buf[Total_SENIDNum][8];
extern UINT8 g_range_bwb_buf[Total_SENIDNum][4];
extern UINT8 g_range_bwc_buf[Total_SENIDNum][7];
extern UINT8 g_range_bwbl_buf[Total_SENIDNum][8];
extern UINT8 g_range_bwbm_buf[Total_SENIDNum][8];
extern UINT8 g_range_bwbh_buf[Total_SENIDNum][8];

//**  IFE-Range
extern UINT32 g_range_a_thr_buf[Total_SENIDNum][6];
extern UINT32 g_range_a_lut_buf[Total_SENIDNum][17];
extern UINT32 g_range_b_thr_buf[Total_SENIDNum][6];
extern UINT32 g_range_b_lut_buf[Total_SENIDNum][17];
extern RangeCFGA g_range_a_buf[Total_SENIDNum];
extern RangeCFGB g_range_b_buf[Total_SENIDNum];
extern Range_Filter g_range_filter_buf[Total_SENIDNum];
extern IFE_FILT g_filter_buf[Total_SENIDNum];

//**  IFE-CLAMP
extern RANGE_FILT_CLAMP_PARAM g_filter_clamp_buf[Total_SENIDNum];

//**  IFE-CGain
extern UINT32 g_ife_ob_buf[Total_SENIDNum][4];
extern UINT32 g_ife_c_gain_buf[Total_SENIDNum][4];
extern IFE_CG g_ife_color_gain_buf[Total_SENIDNum];

//**  IFE-Vig
extern IQS_VIG_CENTER_RATIO g_vig_center_buf[Total_SENIDNum];
extern IQS_VIG_SET g_vig_set_temp[Total_SENIDNum];
extern IQS_VIG_SET g_vig_set_buf[Total_SENIDNum];
extern IFE_VIG g_vig_buf[Total_SENIDNum];

//**  IFE-Gbal
extern UINT8 g_gbal_s_tab_buf[Total_SENIDNum][17];
extern UINT8 g_gbal_d_tab_buf[Total_SENIDNum][17];
extern UINT32 g_gbal_ir_sub_buf[Total_SENIDNum][4];
extern IFE_GBAL g_gbal_buf[Total_SENIDNum];

////////////////////////////////
//             DCE             /
////////////////////////////////
//**  DCE-DIST
extern DIST g_dist_buf[Total_SENIDNum];

//**  DCE-RADIOUS
extern Fact_Norm g_radious_buf[Total_SENIDNum];

//**  DCE-CFA
extern DCE_CFAPARAM g_cfa_buf[Total_SENIDNum];

//**  DCE-GDC
extern FOV g_fov_buf[Total_SENIDNum];
extern UINT32 g_ldc_g_lut_buf[Total_SENIDNum][65];
extern INT32 g_ldc_r_lut_buf[Total_SENIDNum][65];
extern INT32 g_ldc_b_lut_buf[Total_SENIDNum][65];
extern EnH g_ldc_enh_buf[Total_SENIDNum];

//**  DCE-CAC
extern Aberation g_cac_buf[Total_SENIDNum];

extern DCE_Distortion  g_ldc_buf[Total_SENIDNum];

////////////////////////////////
//             IPE             /
////////////////////////////////
//**  IPE-Edge EXT
extern UINT32 g_edge_gamma_buf[Total_SENIDNum][65];
extern INT16 g_edge_kernel_buf[Total_SENIDNum][12];
extern UINT8 g_edir_tab_buf[Total_SENIDNum][8];
extern IPE_DEE g_edir_ext_buf[Total_SENIDNum];
extern IPE_DirScorCal g_score_tab_buf[Total_SENIDNum];
extern IPE_DirScorCalTh g_score_th_buf[Total_SENIDNum];
extern IPE_DEConParam g_decon_buf[Total_SENIDNum];
extern IPE_KAEdgeW g_ewa_buf[Total_SENIDNum];
extern IPE_KBEdgeW g_ewb_buf[Total_SENIDNum];
extern IPE_EdgeW g_ewparam_buf[Total_SENIDNum];
extern IPE_EDGEEXT g_edge_ext_buf[Total_SENIDNum];

//**  IPE-Edge Enh
extern IPE_EDGEENH g_edge_enh_buf[Total_SENIDNum];

//**  IPE-Edge Map
extern EdgeMap g_ed_map_buf[Total_SENIDNum];
extern UINT8 g_ed_tab_buf[Total_SENIDNum][16];
extern EdgeMapS g_es_map_buf[Total_SENIDNum];
extern UINT8 g_es_tab_buf[Total_SENIDNum][16];
extern IPE_EDGEMAP g_edge_map_buf[Total_SENIDNum];

//**  IPE-LPF
extern IPE_RGBLPF g_lpf_buf[Total_SENIDNum];

//**  IPE-CC
extern UINT16 g_cc_matrix_buf[Total_SENIDNum][9];
extern UINT8 g_fs_tab_buf[Total_SENIDNum][16];
extern UINT8 g_fd_tab_buf[Total_SENIDNum][16];
extern IPE_CC g_cc_buf[Total_SENIDNum];

//**  IPE-Ycurve
extern UINT32 g_y_curve_buf[Total_SENIDNum][129];

//**  IPE-Gamma
extern UINT32 g_gamma_buf[Total_SENIDNum][129];

//**  IPE-3DCC
extern IPE_3DCC g_color_3d_buf[Total_SENIDNum];

//**  IPE-CCtrl
extern INT16 g_int_ofs_buf[Total_SENIDNum];

extern UINT8 g_hue_tab_blend[Total_SENIDNum][24];
extern UINT8 g_hue_tab_buf[Total_SENIDNum][24];
extern INT32 g_sat_tab_blend[Total_SENIDNum][24];
extern INT32 g_sat_tab_buf[Total_SENIDNum][24];
extern INT32 g_int_tab_blend[Total_SENIDNum][24];
extern INT32 g_int_tab_buf[Total_SENIDNum][24];
extern UINT8 g_edg_tab_buf[Total_SENIDNum][24];
extern UINT8 g_dds_tab_buf[Total_SENIDNum][8];

extern INT16 g_saturation_buf[Total_SENIDNum];
extern UINT32 g_awb2sat_ratio_buf[Total_SENIDNum];
extern INT16 g_contrast_buf[Total_SENIDNum];
extern INT16 g_cont2sat_adj_buf[Total_SENIDNum];
extern IQS_CCTRL_INFO g_cctrl_info_buf[Total_SENIDNum];
extern IPE_CCTRL g_cctrl_buf[Total_SENIDNum];

//**  IPE-CST
extern INT16 g_cst_coef_buf[Total_SENIDNum][9];
extern IPE_CST g_cst_buf[Total_SENIDNum];

//**  IPE-YCCFIX
extern IPE_YCCFIX g_ycc_fix_buf[Total_SENIDNum];

//**  IPE-NoiseParam
extern IPE_RAND_NR g_noise_param_buf[Total_SENIDNum];

//**  IPE-GammaRand
extern IPE_GamRand g_gamma_rand_buf[Total_SENIDNum];

//**  IPE-YCbCr
extern IPE_CCOFS g_cc_ofs_buf[Total_SENIDNum];
extern IPE_CCON g_ycc_contrast_buf[Total_SENIDNum];

////////////////////////////////
//             IFE2            /
////////////////////////////////
//**  IFE2-ReferenceCenter
extern UINT32 g_ref_center_y_th_buf[Total_SENIDNum][3];
extern UINT32 g_ref_center_y_wt_buf[Total_SENIDNum][4];
extern IFE2_REFCENT_Set g_ref_center_y_buf[Total_SENIDNum];
extern UINT32 g_ref_center_uv_th_buf[Total_SENIDNum][3];
extern UINT32 g_ref_center_uv_wt_buf[Total_SENIDNum][4];
extern IFE2_REFCENT_Set g_ref_center_uv_buf[Total_SENIDNum];
extern IFE2_Refcent_Param g_ref_center_buf[Total_SENIDNum];

//**  IFE2-Filter
extern UINT32 g_ife2filter_y_th_buf[Total_SENIDNum][5];
extern UINT32 g_ife2filter_y_wt_buf[Total_SENIDNum][6];
extern UINT32 g_ife2filter_uv_th_buf[Total_SENIDNum][5];
extern UINT32 g_ife2filter_uv_wt_buf[Total_SENIDNum][6];
extern IQS_IFE2_FILTER g_ife2filter_buf[Total_SENIDNum];

//**  IFE2-Gray
extern IFE2_GRAY_Statal g_gray_statical_buf[Total_SENIDNum];

////////////////////////////////
//             IME             /
////////////////////////////////
//**  IME-LCA
extern IME_CHRA_ADAP g_chroma_adapt_buf[Total_SENIDNum];
extern IME_CHRA_CADAP g_chroma_adapt_color_buf[Total_SENIDNum];
extern IME_CHRA_LADAP g_chroma_adapt_lum_buf[Total_SENIDNum];

//**  IME-ChromaSuppression
extern UINT32 g_chroma_supp_w_y_buf[Total_SENIDNum][16];
extern UINT32 g_chroma_supp_w_c_buf[Total_SENIDNum][16];
extern IME_ChromaSupp g_chroma_supp_buf[Total_SENIDNum];

//**  IME-SuperResolution
extern IME_SuperRes g_super_res_buf[Total_SENIDNum];

//**  IME-SQUARE
extern UINT32 g_sq_level_buf[Total_SENIDNum];
extern IME_HAL_3DNR_INFO g_sq_param_buf[Total_SENIDNum];
extern UINT32 g_sq_luma_sigma_buf[Total_SENIDNum];
extern UINT32 g_sq_luma_detail_penalty_buf[Total_SENIDNum][6];
extern UINT32 g_sq_luma_sad_penalty_buf[Total_SENIDNum][8];
extern UINT32 g_sq_luma_snr_ratio_buf[Total_SENIDNum][6];
extern UINT32 g_sq_luma_tnr_ratio_buf[Total_SENIDNum][6];
extern UINT32 g_sq_luma_luma_gain_buf[Total_SENIDNum][8];
extern UINT32 g_sq_luma_snr_freq_buf[Total_SENIDNum][4];
extern UINT32 g_sq_chroma_y_strg_buf[Total_SENIDNum];
extern UINT32 g_sq_chroma_c_strg_buf[Total_SENIDNum];
extern UINT32 g_sq_chroma_luma_gain_buf[Total_SENIDNum][8];

#endif
