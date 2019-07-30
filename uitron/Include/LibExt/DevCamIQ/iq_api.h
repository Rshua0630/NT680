#ifndef _IQ_API_H_
#define _IQ_API_H_
#include "ipl_alg_infor.h"
#include "iq_param_ctrl.h"
#include "iq_tuning_api.h"
#include "h26x_def.h"

#define IQS_DUMP_ENGREG   0

typedef enum {
	IQ_LINUX_SET_BRI,
	IQ_LINUX_SET_SAT,
	IQ_LINUX_SET_CON,
	IQ_LINUX_SET_SHARP,
	IQ_LINUX_SET_HUE,
	IQ_LINUX_SET_NR,
	IQ_LINUX_SET_TNR,
	IQ_LINUX_SET_GDC,
	IQ_LINUX_SET_VIG,
} IQ_LINUX_SET_ITEM;

typedef enum {
	IQS_NR_MODE_AUTO,
	IQS_NR_MODE_MANUAL,
	IQS_NR_MODE_MAX,
} IQ_LINUX_TNR_MODE;

typedef struct {
	IQ_LINUX_TNR_MODE mode;
	UINT32 ratio;
} IQ_LINUX_TNR_INFO;

typedef struct {
	BOOL enable;
	UINT32 ratio;
} IQ_LINUX_GDC_INFO;

typedef struct {
	BOOL enable;
	UINT32 ratio;
	UINT32 center_x;
	UINT32 center_y;
} IQ_LINUX_VIG_INFO;

typedef struct {
	UINT32 bri_ratio;
	UINT32 sat_ratio;
	UINT32 con_ratio;
	UINT32 sharp_ratio;
	UINT32 hue_angle;
	UINT32 nr_ratio;
	IQ_LINUX_TNR_INFO tnr;
	IQ_LINUX_GDC_INFO gdc;
	IQ_LINUX_VIG_INFO vig;
} IQ_LINUX_SET_INFO;

/**
    IQ_TUNING_C.
*/
extern void iq_set_linux_iq_info(UINT32 id, IQ_LINUX_SET_ITEM item, IQ_LINUX_SET_INFO iq_linux_set_info);
extern void iq_get_linux_iq_info(UINT32 Id, IQ_LINUX_SET_INFO *iq_linux_set_info);

/**
    IQ_PROC_C.
*/
extern ER iq_set_flow(UINT32 Id, IQS_FLOW_MSG MsgID);
extern ER iq_set_flow_cb(UINT32 Id, IQS_FLOW_MSG MsgID, ENG_SUBFUNC *Info, IPL_HAL_GROUP *group);
extern void iq_init(UINT32 Id);

/**
    IQ_FUNC_C.
*/
extern void iq_get_ipe_eth_param(IPE_EethParam *ipe_eth_buf);
extern UINT32 iq_get_msnr_time(UINT32 Id);
extern UINT32 iq_get_ife2_lca_time(UINT32 Id, IQS_FLOW_MSG MsgID);
extern UINT32 iq_get_ife2_lca_ratio(UINT32 Id, BOOL IsCap, BOOL GetMaxRatio);
extern UINT32 iq_check_ife2_lca(UINT32 Id, BOOL IsCap);
extern void iq_get_dce_param(UINT32 Id, UINT32 MsgID, UINT32 ZoomIdx, DCE_Distortion *pDistortion, DIST *pDce_Dist, Fact_Norm *pRadious);
extern UINT32* iq_get_gamma_lut(UINT32 id);
extern UINT32 iq_get_gamma_value(UINT32 Id, UINT32 Data12Bit);
extern UINT32 iq_get_ob(UINT32 Id);
extern void iq_set_awb2sat_ratio(UINT32 id, UINT32 ratio);
extern void iq_get_tnr_config(UINT32 sen_id, UINT32 p3DNRConfig);
extern UINT32 iq_get_tnr_level(UINT32 sen_id);
extern void iqs_get_isp_info(sISP_isp_info	*info);
extern void iq_get_hdr_param(UINT id, SHDR_IQ_PARA *param);
extern void iq_get_defog_param(UINT id,S_DEFOG_LIB_INIT_PARA *param);
extern UINT32* iq_get_defog_wet(UINT id);
extern UINT8 iq_get_wdr_strength(UINT id);
extern void iq_get_gce_strength(UINT id , SR_WDR_GCE_PARAM *gce_param);
extern void iq_get_status(sISP_IQ_STATUS *iq_status);
extern IPL_STRP_INFO * iq_get_stripe_info(UINT id, UINT32 msg_id);
#endif //_IQ_API_H_
