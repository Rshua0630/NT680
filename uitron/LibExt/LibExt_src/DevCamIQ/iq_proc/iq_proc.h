#ifndef _IQ_PROC_H_
#define _IQ_PROC_H_

/* All Parameter Addr read from iqs_commom */
extern sISP_ISOINDEX* g_iso_index_addr[Total_SENIDNum];
extern ISOMAP *g_iso_map_tab_addr[Total_SENIDNum];

extern ENGINE_CTRL *g_e_ctrl_addr[Total_SENIDNum];

extern SIE_EmbeddedCS *g_ecs_addr[Total_SENIDNum];
extern UINT32 *g_ecs_tab_addr[Total_SENIDNum];

extern UINT32 *g_ob_addr[Total_SENIDNum];
extern UINT8 *g_wdr_strength_addr[Total_SENIDNum];
extern IQS_WDR_LCE_OFFSET *g_wdr_lce_offset_addr[Total_SENIDNum];
extern IQS_LCE *g_lce_strength_addr[Total_SENIDNum];
extern SR_WDR_GCE_PARAM *g_gce_strength_addr[Total_SENIDNum];
extern SHDR_IQ_PARA *g_hdr_addr[Total_SENIDNum];
extern S_DEFOG_LIB_INIT_PARA* g_defog_addr[Total_SENIDNum];
extern UINT32 *g_defog_wet_addr[Total_SENIDNum];

extern UINT32 *g_outl_bri_addr[Total_SENIDNum];
extern UINT32 *g_outl_dark_addr[Total_SENIDNum];
extern UINT32 *g_outl_cnt_addr[Total_SENIDNum];
extern IFE_OUTL *g_outl_set_addr[Total_SENIDNum];
extern UINT32 *g_spatial_w_addr[Total_SENIDNum];
extern Spatial_Filter *g_spatial_addr[Total_SENIDNum];
extern UINT8 *g_range_ker_radius_addr[Total_SENIDNum];
extern IFE_HAL_NLM_KER *g_range_ker_addr[Total_SENIDNum];
extern UINT8 *g_range_mwth_addr[Total_SENIDNum];
extern UINT8 *g_range_bwa_addr[Total_SENIDNum];
extern UINT8 *g_range_bwb_addr[Total_SENIDNum];
extern UINT8 *g_range_bwc_addr[Total_SENIDNum];
extern UINT8 *g_range_bwbl_addr[Total_SENIDNum];
extern UINT8 *g_range_bwbm_addr[Total_SENIDNum];
extern UINT8 *g_range_bwbh_addr[Total_SENIDNum];
extern UINT32 *g_range_a_thr_addr[Total_SENIDNum];
extern UINT32 *g_range_a_lut_addr[Total_SENIDNum];
extern UINT32 *g_range_b_thr_addr[Total_SENIDNum];
extern UINT32 *g_range_b_lut_addr[Total_SENIDNum];
extern RangeCFGB *g_range_b_addr[Total_SENIDNum];
extern RANGE_FILT_CLAMP_PARAM *g_filter_clamp_addr[Total_SENIDNum];
extern IQS_VIG_CENTER_RATIO *g_vig_center_addr[Total_SENIDNum];
extern IQS_VIG_SET *g_vig_addr[Total_SENIDNum];
extern IQS_VIG_ISO_TH *g_vig_iso_th_addr[Total_SENIDNum];
extern UINT8 *g_gbal_s_tab_addr[Total_SENIDNum];
extern UINT8 *g_gbal_d_tab_addr[Total_SENIDNum];
extern UINT32 *g_gbal_ir_sub_addr[Total_SENIDNum];
extern IFE_GBAL *g_gbal_addr[Total_SENIDNum];

extern DIST *g_dist_addr[Total_SENIDNum];
extern Fact_Norm   *g_radious_addr[Total_SENIDNum];
extern DCE_CFAPARAM *g_cfa_addr[Total_SENIDNum];
extern FOV *g_fov_addr[Total_SENIDNum];
extern UINT32 *g_ldc_g_lut_addr[Total_SENIDNum];
extern INT32 *g_ldc_r_lut_addr[Total_SENIDNum];
extern INT32 *g_ldc_b_lut_addr[Total_SENIDNum];
extern EnH *g_ldc_enh_addr[Total_SENIDNum];
extern Aberation *g_cac_addr[Total_SENIDNum];
extern IPL_STRP_INFO *g_strp_info_addr[Total_SENIDNum];
extern INT16 *g_edge_kernel_addr[Total_SENIDNum];
extern UINT8 *g_edir_tab_addr[Total_SENIDNum];
extern IPE_DEE *g_edir_ext_addr[Total_SENIDNum];
extern IPE_DirScorCal *g_score_tab_addr[Total_SENIDNum];
extern IPE_DirScorCalTh *g_score_th_addr[Total_SENIDNum];
extern IPE_DEConParam *g_decon_addr[Total_SENIDNum];
extern IPE_KAEdgeW *g_ewa_addr[Total_SENIDNum];
extern IPE_KBEdgeW *g_ewb_addr[Total_SENIDNum];
extern IPE_EdgeW *g_ewparam_addr[Total_SENIDNum];
extern IPE_EDGEENH *g_edge_enh_addr[Total_SENIDNum];
extern UINT8 *g_ed_tab_addr[Total_SENIDNum];
extern EdgeMap *g_ed_map_addr[Total_SENIDNum];
extern UINT8 *g_es_tab_addr[Total_SENIDNum];
extern EdgeMapS *g_es_map_addr[Total_SENIDNum];
extern LPF *g_lpf_addr[Total_SENIDNum];
extern IQS_COLOR_TEMPERATURE_TH *g_color_temperature_th_addr[Total_SENIDNum];
extern IQS_COLOR_TEMPERATURE_PARAM *g_color_low_addr[Total_SENIDNum];
extern IQS_COLOR_TEMPERATURE_PARAM *g_color_middle_addr[Total_SENIDNum];
extern IQS_COLOR_TEMPERATURE_PARAM *g_color_high_addr[Total_SENIDNum];
extern UINT8 *g_fs_tab_addr[Total_SENIDNum];
extern UINT8 *g_fd_tab_addr[Total_SENIDNum];
extern IPE_CC *g_cc_addr[Total_SENIDNum];
extern UINT32 *g_y_curve_addr[Total_SENIDNum];
extern IQS_GAMMA_LV_TH *g_gamma_lv_th_addr[Total_SENIDNum];
extern UINT32 *g_gamma_day_addr[Total_SENIDNum];
extern UINT32 *g_gamma_night_addr[Total_SENIDNum];
extern UINT32 *g_color_3d_lut_addr[Total_SENIDNum];
extern IPE_3DCCRound *g_color_3d_addr[Total_SENIDNum];
extern UINT8 *g_edg_tab_addr[Total_SENIDNum];
extern UINT8 *g_dds_tab_addr[Total_SENIDNum];
extern INT8 *g_saturation_addr[Total_SENIDNum];
extern INT8 *g_contrast_addr[Total_SENIDNum];
extern IQS_CCTRL_INFO *g_cctrl_info_addr[Total_SENIDNum];
extern INT16 *g_cst_coef_addr[Total_SENIDNum];
extern IPE_CST *g_cst_addr[Total_SENIDNum];
extern IPE_YCCFIX *g_ycc_fix_addr[Total_SENIDNum];
extern IPE_RAND_NR *g_noise_param_addr[Total_SENIDNum];
extern IPE_GamRand *g_gamma_rand_addr[Total_SENIDNum];

extern UINT32 *g_ref_center_y_th_addr[Total_SENIDNum];
extern UINT32 *g_ref_center_y_wt_addr[Total_SENIDNum];
extern IFE2_REFCENT_Set *g_ref_center_y_addr[Total_SENIDNum];
extern UINT32 *g_ref_center_uv_th_addr[Total_SENIDNum];
extern UINT32 *g_ref_center_uv_wt_addr[Total_SENIDNum];
extern IFE2_REFCENT_Set *g_ref_center_uv_addr[Total_SENIDNum];
extern UINT32 *g_ife2filter_y_th_addr[Total_SENIDNum];
extern UINT32 *g_ife2filter_y_wt_addr[Total_SENIDNum];
extern UINT32 *g_ife2filter_uv_th_addr[Total_SENIDNum];
extern UINT32 *g_ife2filter_uv_wt_addr[Total_SENIDNum];
extern IQS_IFE2_FILTER *g_ife2filter_addr[Total_SENIDNum];
extern IFE2_GRAY_Statal *g_gray_statical_addr[Total_SENIDNum];

extern IME_CHRA_ADAP *g_chroma_adapt_addr[Total_SENIDNum];
extern IME_CHRA_CADAP *g_chroma_adapt_color_addr[Total_SENIDNum];
extern IME_CHRA_LADAP *g_chroma_adapt_lum_addr[Total_SENIDNum];
extern UINT32 *g_chroma_supp_w_y_addr[Total_SENIDNum];
extern UINT32 *g_chroma_supp_w_c_addr[Total_SENIDNum];
extern IME_ChromaSupp *g_chroma_supp_addr[Total_SENIDNum];
extern IME_SuperRes *g_super_res_addr[Total_SENIDNum];
extern IME_HAL_3DNR_LUMA_PARAM **g_sq_luma_param_addr[Total_SENIDNum];
extern IME_HAL_3DNR_CHROMA_PARAM **g_sq_chroma_param_addr[Total_SENIDNum];
extern IME_HAL_3DNR_Y_PARAM **g_sq_chroma_y_param_addr[Total_SENIDNum];

extern UINT32 *g_tnr_total_num_addr[Total_SENIDNum];
extern INT8 *g_tnr_level_addr[Total_SENIDNum];
extern H26XEncTnr **g_tnr_param_addr[Total_SENIDNum];
#endif
