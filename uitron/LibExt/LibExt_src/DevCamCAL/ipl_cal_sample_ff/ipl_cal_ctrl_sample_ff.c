/*
    IPL calibration ext ctrl flow

    @file       ipl_cal_ctrl_sample_ff.c
    @ingroup    mILibCal
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#include "ipl_cal_ctrl_sample_ff_int.h"
#include "display.h"

#if 0
- static
#endif
// for sie
SIE_EmbeddedCS cal_sie_ecs_param = {0};
// for ime
static IPL_YCC_IMG_INFO ime_path_info[IPL_ID_MAX_NUM][CAL_IME_PATH_MAX_NUM] = {0};
// for display
IPL_PROC_ID display_id = IPL_ID_1;
CAL_IME_PATH display_path = CAL_IME_PATH_2;
BOOL display_is_closed[IPL_ID_MAX_NUM] = {FALSE};

UINT32 cal_ext_get_buf_id_sample(UINT32 buf_id, CAL_GET_BUF_ID_INFO buf_id_info)
{
	switch (buf_id) {
	case CAL_BUF_ID_PRV_1:
		if (buf_id + buf_id_info.id >= CAL_BUF_ID_PRV_MAX) {
			DBG_ERR("buf_id %d error\r\n", buf_id);
			return buf_id;
		} else {
			return buf_id + buf_id_info.id;
		}
		break;

	case CAL_BUF_ID_INIH_1:
		return CAL_BUF_ID_INIH_1;
		break;


	case CAL_BUF_ID_INID_1:
		return CAL_BUF_ID_INID_1;
		/*if (buf_id + buf_id_info.id >= CAL_BUF_ID_INID_MAX) {
		    DBG_ERR("buf_id %d error\r\n", buf_id);
		    return buf_id;
		} else {
		    return buf_id + buf_id_info.id;
		}*/
		break;

	case CAL_BUF_ID_GETRAW_1:
		if (buf_id + buf_id_info.id >= CAL_BUF_ID_GETRAW_MAX) {
			DBG_ERR("buf_id %d error\r\n", buf_id);
			return buf_id;
		} else {
			return buf_id + buf_id_info.id;
		}
		break;

	case CAL_BUF_ID_OTHER_1:
		if (buf_id + buf_id_info.id >= CAL_BUF_ID_OTHER_MAX) {
			DBG_ERR("buf_id %d error\r\n", buf_id);
			return buf_id;
		} else {
			return buf_id + buf_id_info.id;
		}
		break;

	case CAL_BUF_ID_NONE:
		return CAL_BUF_ID_NONE;
		break;

	default:
		DBG_ERR("buf_id %d error\r\n", buf_id);
		return CAL_BUF_ID_NONE;
		break;
	}

	DBG_ERR("get buf_id error\r\n");
	return CAL_BUF_ID_NONE;
}



_INLINE void getraw_ctrl_sie_act(IPL_PROC_ID id, SENSOR_INFO *sen_info, SIE_ACT_MODE *sie_act_mode)
{
	ER rt;
	SEN_STATUS_PARAM sensor_status;

	sensor_status.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sensor_status);
	if (rt != E_OK) {
		sensor_status.sensor_info.sig_mode = SENSOR_SIGNAL_MODE_PARA_MSTR_SNR;
		DBG_ERR("\r\n");
	}

	*sie_act_mode = IPL_UTI_CONV2_SEN_ACT_MODE(sensor_status.sensor_info.sig_mode);
}

_INLINE void getraw_ctrl_sie_win(IPL_PROC_ID id, SENSOR_INFO *sen_info, SIE_WINDOW *sie_win)
{
	ER rt = E_OK;
	UINT32 crop_shift_x = 0;
	UINT32 crop_shift_y = 0;
	UINT32 sie_crop_sft_x = 0;
	UINT32 sie_crop_sft_y = 0;

	if (sen_info->mode == NULL) {
		DBG_ERR("sen_info->mode NULL\r\n");
		return;
	}

	if ((sen_info->mode->trans_hd[0] == NULL) || (sen_info->mode->trans_vd[0] == NULL)) {
		DBG_ERR("sen_info->mode->trans_hd[0] 0x%.8x or sen_info->mode->trans_vd[0] 0x%.8x NULL\r\n");
		return;
	}

	sie_win->Act.StartPix.x = sen_info->mode->trans_hd[0]->data_start;
	sie_win->Act.StartPix.y = sen_info->mode->trans_vd[0]->data_start;

	sie_win->Act.SizeH = sen_info->mode->trans_hd[0]->data_size;
	sie_win->Act.SizeV = sen_info->mode->trans_vd[0]->data_size;
	sie_win->Act.CFA = IPL_UTI_CONV_SEN_STPIX(sen_info->mode->stpix);

	//crop size must be mutiple of 4
	if ((sie_win->Act.SizeH % 4)) {
		crop_shift_x = sie_win->Act.SizeH % 4;
	}
	if ((sie_win->Act.SizeV % 4)) {
		crop_shift_y = sie_win->Act.SizeV % 4;
	}

	sie_win->Crop.SizeH = sie_win->Act.SizeH - sie_crop_sft_x - crop_shift_x;
	sie_win->Crop.SizeV = sie_win->Act.SizeV - sie_crop_sft_y - crop_shift_y;
	sie_win->Crop.CFA = sie_win->Act.CFA;

	if ((sie_win->Act.SizeH < sie_win->Crop.SizeH) || (sie_win->Act.SizeV < sie_win->Crop.SizeV)) {
		DBG_ERR("act size (%d %d) < crop Size(%d %d)\r\n"
				, sie_win->Act.SizeH, sie_win->Act.SizeV, sie_win->Crop.SizeH, sie_win->Crop.SizeV);
		sie_win->Crop.StartPix.x = 0;
		sie_win->Crop.StartPix.y = 0;
	} else {
		sie_win->Crop.StartPix.x = (sie_win->Act.SizeH - sie_win->Crop.SizeH) >> 1;
		sie_win->Crop.StartPix.y = (sie_win->Act.SizeV - sie_win->Crop.SizeV) >> 1;
	}


	rt |= ipl_cal_set_info(id, CAL_SEL_GETRAW_SIE_ACT_STR_X, sie_win->Act.StartPix.x);
	rt |= ipl_cal_set_info(id, CAL_SEL_GETRAW_SIE_ACT_STR_Y, sie_win->Act.StartPix.y);
	rt |= ipl_cal_set_info(id, CAL_SEL_GETRAW_SIE_CRP_STR_X, sie_win->Crop.StartPix.x);
	rt |= ipl_cal_set_info(id, CAL_SEL_GETRAW_SIE_CRP_STR_Y, sie_win->Crop.StartPix.y);

	if (rt != E_OK) {
		DBG_ERR("ipl_cal_set_info error %d\r\n", rt);
	}
}

_INLINE void getraw_ctrl_sie_sig(IPL_PROC_ID id, SENSOR_INFO *sen_info, IPP_SIE_SIGNAL *sen_sig)
{
	sen_sig->SensorPhase.VD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_PHASE);
	sen_sig->SensorPhase.HD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_PHASE);
	sen_sig->SensorPhase.Data = IPL_CtrlGetInfor(id, IPLCTRL_SIE_DATA_PHASE);
	sen_sig->Sie_HDVD.HD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_INV);
	sen_sig->Sie_HDVD.VD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_INV);
}

_INLINE void getraw_ctrl_sie_burstlenght(IPL_PROC_ID id, SIE_BURSTLENGTH_SET *burstlenght)
{
	// set in SIE driver
	return;
}


static IPH_CLK getraw_ctrl_sie_clk_cb(UINT32 id)
{
	ER rt = E_OK;
	UINT32 div;
	IPH_CLK clk = {0};
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sen_status = {0};

	sen_status.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status);
	sen_info = sen_status.sensor_info;
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("get sensor status fail id:%d, sensor mode: %d\r\n"
				, IPL_UTI_CONV2_SEN_ID(id), sen_status.sensor_mode);
		return clk;
	}

	switch (sen_info.data_type) {
	case SENSOR_DATA_PARALLEL:
		//#NT#2017/05/10#Silvia Wu -begin
		//#NT# need to check (ipl need to check)
		clk.SIEclk = sen_info.mode->pclk * 2;
		//if (getraw_ctrl_sie_clksrc_cb(id) == IPH_SIECLKSRC_480) {
		if (1) {

			if ((480000000 % clk.SIEclk) != 0) {
				div = 480000000 / clk.SIEclk;
				clk.SIEclk = 480000000 / div;
			}

			if (clk.SIEclk > 400000000) {
				clk.SIEclk = 240000000;
				DBG_WRN("sie clk error\r\n");
			}
		}
		//#NT#2017/05/10#Silvia Wu -end
		break;

	case SENSOR_DATA_SIEPATGEN:
	case SENSOR_DATA_LVDS:
	case SENSOR_DATA_MIPI:
	case SENSOR_DATA_SLVSEC:
		clk.SIEclk = 0;
		break;

	default:
		clk.SIEclk = 240000000;
		break;
	}

	return clk;
}

static UINT32 getraw_ctrl_sie_clksrc_cb(UINT32 id)
{
	//#NT#2017/05/10#Silvia Wu -begin
	//#NT# need to check (ipl need to check)
	return IPH_SIECLKSRC_CURR;
	//#NT#2017/05/10#Silvia Wu -end
}

static UINT32 getraw_ctrl_sie_pclksel_cb(UINT32 id)
{
	ER rt = E_OK;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sen_status = {0};

	sen_status.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status);
	sen_info = sen_status.sensor_info;
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("get sensor status fail id:%d, sensor mode: %d\r\n"
				, IPL_UTI_CONV2_SEN_ID(id), sen_status.sensor_mode);
	}

	if (sen_info.cmd_type == SENSOR_CMD_Vx1) {
		if (sen_status.sensor_info.mode->dvi != NULL) { // CCIR sensor
			if (sen_status.sensor_info.mode->dvi->mode == SENSOR_DVI_MODE_SD) {
				return IPH_SIEPXCLK_VX1_1X;
			} else {
				return IPH_SIEPXCLK_VX1_2X;
			}
		} else { // RAW sensor
			switch (sen_info.vx1.tx_init) {
			case VX1_TXINIT_3BYTEMODE:
			case VX1_TXINIT_4BYTEMODE:
				return IPH_SIEPXCLK_VX1_1X;
				break;

			case VX1_TXINIT_3BYTEMODE_MUX:
			case VX1_TXINIT_4BYTEMODE_MUX:
			case VX1_TXINIT_THCV241_RAW12HF2_MAP2:
			case VX1_TXINIT_THCV241_RAW10HF2_MAP3:
				return IPH_SIEPXCLK_VX1_2X;
				break;

			default:
				DBG_ERR("VX1_TXINIT %d error\r\n", sen_info.vx1.tx_init);
				return IPH_SIEPXCLK_VX1_1X;
				break;
			}
		}
	} else {
		return IPH_SIEPXCLK_PAD;
	}
	//#NT#2017/05/10#Silvia Wu -end
}

_INLINE void getraw_ctrl_sie_setob_cb(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *inte_en, SIE_UPDATE *update, SIE_OBAVG *ob_avg, SIE_OBSUB *ob_sub)
{
	ER rt = E_OK;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sen_status = {0};
	CAL_CTRL_SETTING_INFO *ctrl_setting_info;

	// get sensor information
	sen_status.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status);
	sen_info = sen_status.sensor_info;
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("get sensor status fail id:%d, sensor mode: %d\r\n"
				, IPL_UTI_CONV2_SEN_ID(id), sen_status.sensor_mode);
		return;
	}

	// get ob information & set sie
	ctrl_setting_info = ipl_cal_ext_setting_sample()->fp_get_ctrl_info();
	if (ctrl_setting_info == NULL) {
		DBG_ERR("get cal setting info error\r\n");
		return;
	}
	if (ctrl_setting_info->sie == NULL) {
		DBG_ERR("get cal sie info error\r\n");
		return;
	}
	if (ctrl_setting_info->sie->ob_info != NULL) {
		if (ctrl_setting_info->sie->ob_info->ob_auto != NULL) {
			if (ctrl_setting_info->sie->ob_info->ob_auto->ob_auto_en) {
				ob_avg->StartPix.x = sen_info.mode->ob.top_win[0];
				ob_avg->StartPix.y = sen_info.mode->ob.top_win[1];
				ob_avg->SizeH = (sen_info.mode->ob.top_win[2] - sen_info.mode->ob.top_win[0]);
				ob_avg->SizeV = (sen_info.mode->ob.top_win[3] - sen_info.mode->ob.top_win[1]);
				ob_avg->SubRatio_X = ctrl_setting_info->sie->ob_info->ob_auto->sub_ratio_x;
				ob_avg->AvgActThres = ctrl_setting_info->sie->ob_info->ob_auto->avg_act_thres;
				ob_avg->ObSubRatio = ctrl_setting_info->sie->ob_info->ob_auto->sub_ratio;
				*update |= (SIE_OBAVG_);
			}
		}

		ob_sub->Offset = ctrl_setting_info->sie->ob_info->ob_offset;

		*func_en |= (OBSubSelEn | OBAvgEn);
		*update |= (SIE_OBOFS_ | SIE_SUBFEN);
	}
}

_INLINE void getraw_ctrl_sie_setca_cb(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *inte_en, SIE_UPDATE *update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *path_info)
{
	ER rt = E_OK;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sen_status = {0};
	SIE_WINDOW sie_win_addr = {0};
	SIE_WINDOW *sie_win = &sie_win_addr;
	CAL_CTRL_SETTING_INFO *ctrl_setting_info;

	// get sensor information
	sen_status.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status);
	sen_info = sen_status.sensor_info;
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("get sensor status fail id:%d, sensor mode: %d\r\n"
				, IPL_UTI_CONV2_SEN_ID(id), sen_status.sensor_mode);
	}

	// get ca information & set sie
	ctrl_setting_info = ipl_cal_ext_setting_sample()->fp_get_ctrl_info();
	if (ctrl_setting_info == NULL) {
		DBG_ERR("get cal setting info error\r\n");
		return;
	}
	if (ctrl_setting_info->sie == NULL) {
		DBG_ERR("get cal sie info error\r\n");
		return;
	}
	if (ctrl_setting_info->sie->ca_info != NULL) {
		if (ctrl_setting_info->sie->ca_info->ca_en) {
			if ((ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].x == 0) &&
				(ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].y == 0) &&
				(ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].w == 0) &&
				(ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].h == 0)) { // auto calc
				getraw_ctrl_sie_win(id, &sen_status.sensor_info, sie_win);
				cacc->CACrop.StartPix.x = 0;
				cacc->CACrop.StartPix.y = 0;
				cacc->CACrop.SizeH = sie_win->Act.SizeH;
				cacc->CACrop.SizeV = sie_win->Act.SizeV;
			} else {
				cacc->CACrop.StartPix.x = ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].x;
				cacc->CACrop.StartPix.y = ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].y;
				cacc->CACrop.SizeH = ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].w;
				cacc->CACrop.SizeV = ctrl_setting_info->sie->ca_info->crp[sen_status.sensor_mode].h;
			}

			cacc->CA_WIN.WinNum_X = CAL_CA_WIN_X;
			cacc->CA_WIN.WinNum_Y = CAL_CA_WIN_Y;

			cacc->CA_TH_INFO.Gth = ctrl_setting_info->sie->ca_info->g_thr;
			cacc->CA_TH_INFO.Rth = ctrl_setting_info->sie->ca_info->r_thr;
			cacc->CA_TH_INFO.Bth = ctrl_setting_info->sie->ca_info->b_thr;
			cacc->CA_TH_INFO.Pth = ctrl_setting_info->sie->ca_info->p_thr;
			path_info->bVig = ctrl_setting_info->sie->ca_info->b_vig;
			path_info->bCaThreshold = ctrl_setting_info->sie->ca_info->ca_thr_en;

			if (cacc->CACrop.SizeH == 0) {
				DBG_ERR("CACrop.SizeH cannot be zero\r\n");
				cacc->CACrop.SizeH = 1;
			}
			if (cacc->CACrop.SizeV == 0) {
				DBG_ERR("CACrop.SizeV cannot be zero\r\n");
				cacc->CACrop.SizeV = 1;
			}
			if (cacc->CA_WIN.WinNum_X == 0) {
				DBG_ERR("CA_WIN.WinNum_X cannot be zero\r\n");
				cacc->CA_WIN.WinNum_X = 1;
			}
			if (cacc->CA_WIN.WinNum_Y == 0) {
				DBG_ERR("CA_WIN.WinNum_Y cannot be zero\r\n");
				cacc->CA_WIN.WinNum_Y = 1;
			}

			*func_en |= CAEn;
			*update |= (SIE_OUTADDR_CH1 | SIE_OUTINFO_CH1 | SIE_STATS_CA_SIZE | SIE_STATS_CA_);
		} else {
			*func_en &= ~CAEn;
		}
	} else {
		*func_en &= ~CAEn;
	}
	*update |= SIE_SUBFEN;
}

_INLINE void getraw_ctrl_sie_setla_cb(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *inte_en, SIE_UPDATE *update, SIE_LACC *lacc, SIE_STATS_PATH_INFO *path_info)
{
	ER rt = E_OK;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sen_status = {0};
	SIE_WINDOW sie_win_addr = {0};
	SIE_WINDOW *sie_win = &sie_win_addr;
	CAL_CTRL_SETTING_INFO *ctrl_setting_info;

	// get sensor information
	sen_status.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status);
	sen_info = sen_status.sensor_info;
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("get sensor status fail id:%d, sensor mode: %d\r\n"
				, IPL_UTI_CONV2_SEN_ID(id), sen_status.sensor_mode);
	}

	// get ca information & set sie
	ctrl_setting_info = ipl_cal_ext_setting_sample()->fp_get_ctrl_info();
	if (ctrl_setting_info == NULL) {
		DBG_ERR("get cal setting info error\r\n");
		return;
	}
	if (ctrl_setting_info->sie == NULL) {
		DBG_ERR("get cal sie info error\r\n");
		return;
	}
	if (ctrl_setting_info->sie->la_info != NULL) {
		if (ctrl_setting_info->sie->la_info->la_en) {
			if ((ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].x == 0) &&
				(ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].y == 0) &&
				(ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].w == 0) &&
				(ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].h == 0)) { // auto calc
				getraw_ctrl_sie_win(id, &sen_status.sensor_info, sie_win);
				lacc->LACrop.StartPix.x = 0;
				lacc->LACrop.StartPix.y = 0;
				lacc->LACrop.SizeH = sie_win->Act.SizeH;
				lacc->LACrop.SizeV = sie_win->Act.SizeV;
			} else {
				lacc->LACrop.StartPix.x = ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].x;
				lacc->LACrop.StartPix.y = ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].y;
				lacc->LACrop.SizeH = ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].w;
				lacc->LACrop.SizeV = ctrl_setting_info->sie->la_info->crp[sen_status.sensor_mode].h;
			}

			lacc->LA_WIN.WinNum_X = CAL_LA_WIN_X;
			lacc->LA_WIN.WinNum_Y = CAL_LA_WIN_Y;

			path_info->bLaCg = ctrl_setting_info->sie->la_info->b_la_cg;
			path_info->bLaGama1 = ctrl_setting_info->sie->la_info->b_la_gama1;
			path_info->bHistogramSel = ctrl_setting_info->sie->la_info->b_histogram_sel;
			path_info->bLa1SrcSel = ctrl_setting_info->sie->la_info->b_la1_src_sel;

			if (lacc->LACrop.SizeH == 0) {
				DBG_ERR("LACrop.SizeH cannot be zero\r\n");
				lacc->LACrop.SizeH = 1;
			}
			if (lacc->LACrop.SizeV == 0) {
				DBG_ERR("LACrop.SizeV cannot be zero\r\n");
				lacc->LACrop.SizeV = 1;
			}
			if (lacc->LA_WIN.WinNum_X == 0) {
				DBG_ERR("LA_WIN.WinNum_X cannot be zero\r\n");
				lacc->LA_WIN.WinNum_X = 1;
			}
			if (lacc->LA_WIN.WinNum_Y == 0) {
				DBG_ERR("LA_WIN.WinNum_Y cannot be zero\r\n");
				lacc->LA_WIN.WinNum_Y = 1;
			}

			*func_en |= (LAEn | HistoYEn);
			*update |= (SIE_OUTADDR_CH2 | SIE_OUTINFO_CH2 | SIE_STATS_LA_ | SIE_STATS_LA_SIZE | SIE_STATS_LA_CG | SIE_STATS_LA_GAMMA);
		} else {
			*func_en &= ~LAEn;
		}
	} else {
		*func_en &= ~LAEn;
	}
	*update |= SIE_SUBFEN;
}

_INLINE void getraw_ctrl_sie_setdmaoutsrc_cb(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *inte_en, SIE_UPDATE *update)
{
	*func_en |= DMAOutCh0En;
	*update |= (SIE_OUTADDR_CH0 | SIE_OUTINFO_CH0 | SIE_SUBFEN);
}

_INLINE void getraw_ctrl_sie_setrawenc_cb(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *update, SIE_RAWENC *raw_enc)
{
	*func_en &= ~RawEncEn;
}

_INLINE void getraw_ctrl_sie_setethout_cb(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *inte_en, SIE_UPDATE *update, SIE_ETH *eth)
{
	*func_en &= ~(ETHEn);
	*inte_en &= ~(SIE__INTE_DRAM_OUT4_END);
	*update |= (SIE_SUBFEN);
}

_INLINE void getraw_ctrl_sie_setdp_cb(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *inte_en, SIE_UPDATE *update, SIE_DPC *dpc)
{
	ER rt;
	CAL_RW_HEADER_INFO rw_header_info = {0};
	CAL_RW_DATA_INFO rw_data_info = {0};
	UINT32 sft, buf_addr, buf_size, tmp_size;
	//CAL_SETTING_INFO *setting_info;
	//CAL_ALG_DP_SETTING *dp_setting;

	if (id >= IPL_ID_5) {
		*func_en &= ~DpcEn;
		*update &= ~SIE_DPC_; // update defect pixel concealment eng: SIE1234
		return;
	}

	// get header info from dram
	rw_header_info.sec.cal_item = CAL_ITEM_DP;
	rw_header_info.key.id = (IPL_PROC_ID)id;
	rw_header_info.key.cal_cond = CAL_COND_DFT;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rt = cal_rw()->fp_header_read(&rw_header_info, &tmp_size);

	if (rt == E_OK) {
		if ((rw_header_info.value.cal_status == CAL_STATUS_OK) && (rw_header_info.value.read_data_size != 0)) {
			// get data info from dram
			rw_data_info.sec = rw_header_info.sec;
			rw_data_info.key = rw_header_info.key;
			rt = cal_rw()->fp_header_get_dram_sft(&rw_data_info, &sft);
			if (rt == E_OK) {
				CAL_GET_BUF_ID_INFO buf_id_info = {0};
				rt = cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info)
												 , CAL_BUF_ID_DATA_ITEM_STORAGE, &buf_addr, &buf_size);
				if (rt != E_OK) {
					DBG_ERR("buf\r\n");
				}
				// set sie dp info
				dpc->DP_TblAddr = buf_addr + sft;
				dpc->TblSize = rw_header_info.value.read_data_size;
				dpc->Weight = _50F00_PERCENT;
				*func_en |= DpcEn;
				*update |= (SIE_DPC_ | SIE_SUBFEN);
				return;
			}
		}
	}
	if (rt == E_OBJ) {
		DBG_IND("null key or section\r\n");
	} else if (rt != E_OK) {
		DBG_ERR("error %d id %d\r\n", rt, id);
	}
	if (rw_header_info.value.cal_status != CAL_STATUS_OK) {
		DBG_IND("cal_status 0x%.8x\r\n", rt, id);
	}
	*func_en &= ~DpcEn;
	*update |= (SIE_DPC_ | SIE_SUBFEN);
}

_INLINE void getraw_ctrl_sie_setecs_cb(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *inte_en, SIE_UPDATE *update, SIE_SUBFUNC *sub_func)
{
	ER rt;
	CAL_RW_HEADER_INFO rw_header_info = {0};
	CAL_RW_DATA_INFO rw_data_info = {0};
	UINT32 sft, buf_addr, buf_size, tmp_size;
	CAL_SETTING_INFO *setting_info;
	CAL_ALG_ECS_SETTING *ecs_setting;

	// get header info from dram
	rw_header_info.sec.cal_item = CAL_ITEM_ECS;
	rw_header_info.key.id = (IPL_PROC_ID)id;
	rw_header_info.key.cal_cond = CAL_COND_DFT;
	rw_header_info.key.sensor_mode = ipl_cal_get_info(id, CAL_SEL_GETRAW_SENMODE);
	rt = cal_rw()->fp_header_read(&rw_header_info, &tmp_size);

	if (rt == E_OK) {
		if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
			// get data info from dram
			rw_data_info.sec = rw_header_info.sec;
			rw_data_info.key = rw_header_info.key;
			rt = cal_rw()->fp_header_get_dram_sft(&rw_data_info, &sft);
			if (rt == E_OK) {
				CAL_GET_BUF_ID_INFO buf_id_info = {0};
				rt = cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_INID_1, buf_id_info)
												 , CAL_BUF_ID_DATA_ITEM_STORAGE, &buf_addr, &buf_size);
				if (rt != E_OK) {
					DBG_ERR("buf\r\n");
				}
				// set sie ecs info
				setting_info = ipl_cal_ext_setting_sample()->fp_get_setting_info(rw_header_info.sec.cal_item);
				ecs_setting = (CAL_ALG_ECS_SETTING *)(setting_info->alg);
				sub_func->Ecs.Inaddr = buf_addr + sft;
				sub_func->Ecs.EcsMap = ecs_setting->map_size;
				cal_sie_ecs_param.BitSel = ecs_setting->bit_ratio;
				cal_sie_ecs_param.DthrEn = ENABLE;
				cal_sie_ecs_param.DthrRst = ENABLE;
				cal_sie_ecs_param.ECSDthrLv = _BIT_0_1;
				sub_func->Ecs.pecs = &cal_sie_ecs_param;
				*func_en |= ECSEn;
				*update |= (SIE_SUBFEN | SIE_ECS_);
				return;
			}
		}
	}

	if ((rt != E_OK) && (rt != E_OBJ)) { // E_OBJ: null section or key
		DBG_ERR("error %d id %d\r\n", rt, id);
	}
	if (rw_header_info.value.cal_status != CAL_STATUS_OK) {
		DBG_IND("cal_status 0x%.8x\r\n", rt, id);
	}
	*func_en &= ~ECSEn;
	*update |= (SIE_SUBFEN | SIE_ECS_);
}

static UINT32 getraw_ctrl_sie_init_cb(UINT32 id, ENG_SUBFUNC *info, IPL_HAL_GROUP *group)
{
	SIE_UPDATE *sie_update;

	sie_update = (SIE_UPDATE *)group;

	// get sie init information structure
	getraw_ctrl_sie_setob_cb(id, &info->pSie_sub->sieFuncEn, &info->pSie_sub->InteEn, sie_update, &info->pSie_sub->OBAvg, &info->pSie_sub->OBSub);
	getraw_ctrl_sie_setca_cb(id, &info->pSie_sub->sieFuncEn, &info->pSie_sub->InteEn, sie_update, &info->pSie_sub->Ca, &info->pSie_sub->pStatPathInfo);
	getraw_ctrl_sie_setla_cb(id, &info->pSie_sub->sieFuncEn, &info->pSie_sub->InteEn, sie_update, &info->pSie_sub->La, &info->pSie_sub->pStatPathInfo);
	getraw_ctrl_sie_setdmaoutsrc_cb(id, &info->pSie_sub->sieFuncEn, &info->pSie_sub->InteEn, sie_update);
	getraw_ctrl_sie_setrawenc_cb(id, &info->pSie_sub->sieFuncEn, sie_update, &info->pSie_sub->RawEnc);
	getraw_ctrl_sie_setethout_cb(id, &info->pSie_sub->sieFuncEn, &info->pSie_sub->InteEn, sie_update, &info->pSie_sub->Eth);

	// get calibration result
	getraw_ctrl_sie_setdp_cb(id, &info->pSie_sub->sieFuncEn, &info->pSie_sub->InteEn, sie_update, &info->pSie_sub->Dpc);
	getraw_ctrl_sie_setecs_cb(id, &info->pSie_sub->sieFuncEn, &info->pSie_sub->InteEn, sie_update, info->pSie_sub);

	// set sie updata bit and function enable
	*sie_update |= (SIE_INIT | SIE_IOSIZE | SIE_SUBFEN);
	info->pSie_sub->sieFuncEn |= DMAOutCh0En;

	return 1;
}


static CAL_ER_TAG getraw_ctrl_base_info(IPL_PROC_ID id, SENSOR_INFO *sen_info, IPLC_BASEINFO *base_info, BOOL only_calc_buf)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_CTRL_SETTING_INFO *ctrl_setting_info;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 cnt, sie_buf_addr, sie_buf_size;

	if (sen_info == NULL) {
		DBG_ERR("innput parameter error\r\n");
		return CAL_ER_PAR;
	}
	if (sen_info->mode == NULL) {
		DBG_ERR("innput parameter error (mode)\r\n");
		return CAL_ER_PAR;
	}

	getraw_ctrl_sie_act(id, sen_info, &base_info->SIE_Act);
	getraw_ctrl_sie_win(id, sen_info, &base_info->SIE_In);
	getraw_ctrl_sie_sig(id, sen_info, &base_info->Sensor_In);
	getraw_ctrl_sie_burstlenght(id, &base_info->Burst_Length);

	// CH0 (RAW)
	base_info->SIE_Ch0_Img.bit = IPL_RAW_BIT_8;
	base_info->SIE_Ch0_Img.st_pix = IPL_UTI_CONV_SEN_STPIX(sen_info->mode->stpix);
	base_info->SIE_Ch0_Img.width = base_info->SIE_In.Crop.SizeH;
	base_info->SIE_Ch0_Img.height = base_info->SIE_In.Crop.SizeV;
	base_info->SIE_Ch0_Img.line_offset = IPL_UTI_RAWH_CONV2_LOFS(base_info->SIE_Ch0_Img.width, base_info->SIE_Ch0_Img.bit);

	if (only_calc_buf) {
		return CAL_ER_NONE;
	}

	switch (sen_info->mode->stpix) {
	case SENSOR_STPIX_R:
	case SENSOR_STPIX_GR:
	case SENSOR_STPIX_GB:
	case SENSOR_STPIX_B:
		base_info->SIE_Ch0_Bayer_Type = BAYER_FORMAT;
		break;

	case SENSOR_STPIX_RGBIR_RIR:
	case SENSOR_STPIX_RGBIR_RG:
		base_info->SIE_Ch0_Bayer_Type = RGBIR_FORMAT;
		break;

	case SENSOR_STPIX_RCCB_RC:
	case SENSOR_STPIX_RCCB_CR:
	case SENSOR_STPIX_RCCB_CB:
	case SENSOR_STPIX_RCCB_BC:
		base_info->SIE_Ch0_Bayer_Type = RCCB_FORMAT;
		break;

	default:
		base_info->SIE_Ch0_Bayer_Type = BAYER_FORMAT;
		break;
	}


	// CH0 (RAW)
	ctrl_setting_info = ipl_cal_ext_setting_sample()->fp_get_ctrl_info();
	if (ctrl_setting_info == NULL) {
		DBG_ERR("get cal setting info error\r\n");
		return CAL_ER_PAR;
	}

	if (ctrl_setting_info->sie == NULL) {
		DBG_ERR("get cal sie info error\r\n");
		return CAL_ER_PAR;
	}

	base_info->SIE_Ch0_Ppb_Num = ctrl_setting_info->sie->sie_ppb_num;
	buf_id_info.id = id;
	for (cnt = 0; cnt < base_info->SIE_Ch0_Ppb_Num; cnt++) {
		if (cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), CAL_BUF_ID_RAW_ITEM_CH0_PPB_1 + cnt, &sie_buf_addr, &sie_buf_size) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			base_info->SIE_Ch0_Ppb_Addr[cnt] = sie_buf_addr + IPL_HeaderRawGetBufSize();
		}
	}

	// CH1 (CA)
	if (ctrl_setting_info->sie->ca_info != NULL) {
		if (ctrl_setting_info->sie->ca_info->ca_en) {
			base_info->SIE_Ch1_Ppb_Num = base_info->SIE_Ch0_Ppb_Num;
			for (cnt = 0; cnt < base_info->SIE_Ch1_Ppb_Num; cnt++) {
				if (cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), CAL_BUF_ID_RAW_ITEM_CH1_PPB_1 + cnt, &sie_buf_addr, &sie_buf_size) != E_OK) {
					cal_rt |= CAL_ER_BUF;
				} else {
					base_info->SIE_Ch1_Ppb_Addr[cnt] = sie_buf_addr;
					base_info->SIE_Ch1_Ppb_FOfs = sie_buf_size;
				}
			}
		}
	}

	// CH2 (LA)
	if (ctrl_setting_info->sie->la_info != NULL) {
		if (ctrl_setting_info->sie->la_info->la_en) {
			for (cnt = 0; cnt < base_info->SIE_Ch0_Ppb_Num; cnt++) {
				if (cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), CAL_BUF_ID_RAW_ITEM_CH2_PPB_1 + cnt, &sie_buf_addr, &sie_buf_size) != E_OK) {
					cal_rt |= CAL_ER_BUF;
				} else {
					base_info->SIE_Ch2_Ppb_Addr[cnt] = sie_buf_addr;
				}
			}
		}
	}

	base_info->proc_bit_id = CAL_IPL_ID2BIT(id);

	// need to check HDR/CCIR flow
	base_info->IPLCFMode = IPL_CF_NORMAL;
	base_info->IPLCFModeExt = IPL_CFEXT_SIEONLY;

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
	}

	return cal_rt;
}

static CAL_ER_TAG getraw_ctrl_ext_info(IPL_PROC_ID id, SENSOR_INFO *sen_info, IPLC_EXT *ext_info)
{
	if (ext_info == NULL) {
		DBG_ERR("input parame error\r\n");
		return CAL_ER_PAR;
	}

	ext_info->fp_SIEClk = getraw_ctrl_sie_clk_cb;
	ext_info->fp_SIEClkSrc = getraw_ctrl_sie_clksrc_cb ;
	ext_info->fp_SIEPclkSel = getraw_ctrl_sie_pclksel_cb;
	ext_info->fp_SieInit = getraw_ctrl_sie_init_cb;

	return CAL_ER_NONE;
}

static void getraw_ctrl_isr_sie_cb(IPL_PROC_ID id, UINT32 int_status, UINT32 idx)
{
	if (int_status & SIE_FLDEND) {
		if (idx > 1) {
			AE_TrigEvent(IPL_UTI_CONV2_AE_ID(id), int_status);
		}
	}
}

static CAL_ER_TAG getraw_ctrl_isr_info(IPL_PROC_ID id, SENSOR_INFO *sen_info, IPL_ISR_OBJ_INFO *isr_info)
{
	if (isr_info == NULL) {
		DBG_ERR("input parame error\r\n");
		return CAL_ER_PAR;
	}

	isr_info->EventTab.FP[IPL_ISR_SIE1] = getraw_ctrl_isr_sie_cb;
	isr_info->EventTab.FP[IPL_ISR_SIE2] = getraw_ctrl_isr_sie_cb;
	isr_info->EventTab.FP[IPL_ISR_SIE3] = getraw_ctrl_isr_sie_cb;
	isr_info->EventTab.FP[IPL_ISR_SIE4] = getraw_ctrl_isr_sie_cb;
	isr_info->EventTab.FP[IPL_ISR_SIE5] = getraw_ctrl_isr_sie_cb;
	isr_info->EventTab.FP[IPL_ISR_SIE6] = getraw_ctrl_isr_sie_cb;
	isr_info->EventTab.FP[IPL_ISR_SIE7] = getraw_ctrl_isr_sie_cb;
	isr_info->EventTab.FP[IPL_ISR_SIE8] = getraw_ctrl_isr_sie_cb;

	return CAL_ER_NONE;
}

static ER cal_ext_get_getraw_ctrl_param(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_CTRL_PARAM *getraw_ctrl_param)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	SEN_STATUS_PARAM sen_status = {0};

	if (getraw_param == NULL) {
		DBG_ERR("input parameter error\r\n");
		return CAL_ER_PAR;
	}

	// get sensor information
	sen_status.sensor_mode = getraw_param->sen_mode;
	if (sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status) != E_OK) {
		DBG_ERR("sensor_get_info SEN_GET_STATUS error\r\n");
		return CAL_ER_SEN;
	}

	// get ipl ctrl information
	cal_rt |= getraw_ctrl_base_info(id, &sen_status.sensor_info, getraw_ctrl_param->p_base_info, FALSE);
	cal_rt |= getraw_ctrl_ext_info(id, &sen_status.sensor_info, getraw_ctrl_param->p_ext_info);
	cal_rt |= getraw_ctrl_isr_info(id, &sen_status.sensor_info, getraw_ctrl_param->p_isr_info);
	getraw_ctrl_param->p_adapt_setting = NULL;

	if (sen_status.sensor_info.mode == NULL) {
		DBG_ERR("sen_status.sensor_info.mode NULL, force to ICF_FLOW_C\r\n");
		cal_rt |= CAL_ER_SEN;
		getraw_ctrl_param->ipl_ctrl_flow = ICF_FLOW_C;
	} else {
		if ((sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR)
			|| (sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE)) {
			getraw_ctrl_param->ipl_ctrl_flow = ICF_FLOW_CCIR;
		} else {
			getraw_ctrl_param->ipl_ctrl_flow = ICF_FLOW_C;
		}
	}

	//#NT#2017/05/16#Silvia Wu -begin
	//#NT# need to review (sync condition, HDR)
	getraw_ctrl_param->sync_param.group = id;
	getraw_ctrl_param->sync_param.order = 0;
	getraw_ctrl_param->multi_frm_param.ipl_bit_id = (IPL_PROC_BIT_ID)CAL_IPL_ID2BIT(id);//sen_status.sensor_info.mode->multi_frame_info->total_frm;
	//#NT#2017/05/16#Silvia Wu -end

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER cal_ext_get_getraw_buf_size(IPL_PROC_ID id, CAL_GETRAW_PARAM *getraw_param, CAL_GETRAW_BUF_SIZE *getraw_buf_size)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	CAL_CTRL_SETTING_INFO *ctrl_setting_info;
	IPLC_BASEINFO base_info_addr = {0};
	IPLC_BASEINFO *base_info = &base_info_addr;
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 raw_header_size, raw_size, sie_ppb_num, ca_size = 0, la_size = 0;

	if (getraw_param == NULL) {
		DBG_ERR("input parameter error %d\r\n");
		return CAL_ER_PAR;
	}

	// get sie ctrl settting
	ctrl_setting_info = ipl_cal_ext_setting_sample()->fp_get_ctrl_info();

	if (ctrl_setting_info == NULL) {
		DBG_ERR("get cal setting info error\r\n");
		return CAL_ER_SETTING;
	} else if (ctrl_setting_info->sie == NULL) {
		DBG_ERR("get cal sie info error\r\n");
		return CAL_ER_SETTING;
	}

	/**
	    CH0 (RAW)
	*/
	// get raw header size
	raw_header_size = IPL_HeaderRawGetBufSize();
	// get sensor information & calc raw size
	sen_status.sensor_mode = getraw_param->sen_mode;
	if (sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status) != E_OK) {
		cal_rt |= CAL_ER_SEN;
	}
	cal_rt |= getraw_ctrl_base_info(id, &sen_status.sensor_info, base_info, TRUE);
	if ((base_info->SIE_Ch0_Img.line_offset == 0) || (base_info->SIE_Ch0_Img.height == 0)) {
		DBG_ERR("sie info error lofs %d height %d\r\n", base_info->SIE_Ch0_Img.line_offset, base_info->SIE_Ch0_Img.height);
		cal_rt |= CAL_ER_PAR;
		if (base_info->SIE_Ch0_Img.line_offset == 0) {
			base_info->SIE_Ch0_Img.line_offset = 1;
		}
		if (base_info->SIE_Ch0_Img.height == 0) {
			base_info->SIE_Ch0_Img.height = 1;
		}
	}
	raw_size = base_info->SIE_Ch0_Img.line_offset * base_info->SIE_Ch0_Img.height;

	/**
	    CH1 (CA)
	*/
	if (ctrl_setting_info->sie->ca_info != NULL) {
		if (ctrl_setting_info->sie->ca_info->ca_en) {
			ca_size = sizeof(UINT32) * CAL_CA_WIN_X * CAL_CA_WIN_Y * 4;
		}
	}

	/**
	    CH2 (LA)
	*/
	if (ctrl_setting_info->sie->la_info != NULL) {
		if (ctrl_setting_info->sie->la_info->la_en) {
			if (CAL_LA_WIN_X * CAL_LA_WIN_Y >= 64) {
				la_size = sizeof(UINT32) * CAL_LA_WIN_X * CAL_LA_WIN_Y * 3;
			} else {
				la_size = sizeof(UINT32) * (CAL_LA_WIN_X * CAL_LA_WIN_Y * 2 + 64);
			}
		}
	}

	/**
	    calc SIE total buffer size (CH0 + CH1 + CH2)
	*/
	sie_ppb_num = ctrl_setting_info->sie->sie_ppb_num;
	getraw_buf_size->sie_ch_size[CAL_SIE_CH_0] = (raw_header_size + raw_size);
	getraw_buf_size->sie_ch_size[CAL_SIE_CH_1] = ca_size;
	getraw_buf_size->sie_ch_size[CAL_SIE_CH_2] = la_size;
	getraw_buf_size->total_size = (getraw_buf_size->sie_ch_size[CAL_SIE_CH_0] + getraw_buf_size->sie_ch_size[CAL_SIE_CH_1] + getraw_buf_size->sie_ch_size[CAL_SIE_CH_2]) * sie_ppb_num;

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER cal_ext_set_getraw_buf_item(CAL_GET_BUF_ID_INFO buf_id_info, CAL_CTRL_SETTING_INFO *ctrl_setting_info, UINT32 sie_buf_addr, CAL_GETRAW_BUF_SIZE getraw_buf_size)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	UINT32 cnt, sie_ppb_num;

	if (ctrl_setting_info == NULL) {
		DBG_ERR("get cal ctrl info error\r\n");
		return CAL_ER_PAR;
	} else if (ctrl_setting_info->sie == NULL) {
		DBG_ERR("get cal sie info error\r\n");
		return CAL_ER_PAR;
	}
	sie_ppb_num = ctrl_setting_info->sie->sie_ppb_num;

	// set raw buffer (sie ch 0)
	for (cnt = 0; cnt < sie_ppb_num; cnt++) {
		if (cnt >= CAL_BUF_ID_RAW_ITEM_CH0_PPB_MAX - CAL_BUF_ID_RAW_ITEM_CH0_PPB_1) {
			DBG_ERR("RAW buffer overflow\r\n");
			cal_rt |= CAL_ER_BUF;
			break;
		}
		if (cal_buf()->fp_set_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), CAL_BUF_ID_RAW_ITEM_CH0_PPB_1 + cnt, sie_buf_addr, getraw_buf_size.sie_ch_size[CAL_SIE_CH_0]) != E_OK) {
			cal_rt |= CAL_ER_BUF;
		} else {
			sie_buf_addr += getraw_buf_size.sie_ch_size[CAL_SIE_CH_0];
		}
	}

	// set ca buffer (sie ch 1)
	if (ctrl_setting_info->sie->ca_info != NULL) {
		if (ctrl_setting_info->sie->ca_info->ca_en) {
			for (cnt = 0; cnt < sie_ppb_num; cnt++) {
				if (cnt >= CAL_BUF_ID_RAW_ITEM_CH1_PPB_MAX - CAL_BUF_ID_RAW_ITEM_CH1_PPB_1) {
					DBG_ERR("RAW buffer overflow\r\n");
					cal_rt |= CAL_ER_BUF;
					break;
				}
				if (cal_buf()->fp_set_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), CAL_BUF_ID_RAW_ITEM_CH1_PPB_1 + cnt, sie_buf_addr, getraw_buf_size.sie_ch_size[CAL_SIE_CH_1]) != E_OK) {
					cal_rt |= CAL_ER_BUF;
				} else {
					sie_buf_addr += getraw_buf_size.sie_ch_size[CAL_SIE_CH_1];
				}
			}
		}
	}

	// set la buffer (sie ch 2)
	if (ctrl_setting_info->sie->la_info != NULL) {
		if (ctrl_setting_info->sie->la_info->la_en) {
			for (cnt = 0; cnt < sie_ppb_num; cnt++) {
				if (cnt >= CAL_BUF_ID_RAW_ITEM_CH2_PPB_MAX - CAL_BUF_ID_RAW_ITEM_CH2_PPB_1) {
					DBG_ERR("RAW buffer overflow\r\n");
					cal_rt |= CAL_ER_BUF;
					break;
				}
				if (cal_buf()->fp_set_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_GETRAW_1, buf_id_info), CAL_BUF_ID_RAW_ITEM_CH2_PPB_1 + cnt, sie_buf_addr, getraw_buf_size.sie_ch_size[CAL_SIE_CH_2]) != E_OK) {
					cal_rt |= CAL_ER_BUF;
				} else {
					sie_buf_addr += getraw_buf_size.sie_ch_size[CAL_SIE_CH_2];
				}
			}
		}
	}

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static ER cal_ext_get_prv_buf_size(IPL_PROC_ID id, CAL_PRV_PARAM *prv_param, UINT32 *ipl_buf_size, UINT32 *ime_buf_size)
{
	CAL_ER_TAG cal_rt = CAL_ER_NONE;
	IPL_BUF_QUERY_DATA prv_buf_req_info = {0};
	IPL_BUF_INFOR buf_info = {0};
	CAL_IME_PATH ime_cnt = CAL_IME_PATH_1;
	UINT32 ime_buf_size_total = 0,  ime_buf_size_calc[CAL_IME_PATH_MAX_NUM] = {0};

	// get ipl buffer size (not include ime)
	prv_buf_req_info.proc_sen_id = id;
	prv_buf_req_info.raw_out_buf_num = prv_param->raw_out_buf_num;
	prv_buf_req_info.func_en = prv_param->func_en;
	prv_buf_req_info.pnext = NULL; // prv_info->pnext (prv_buf_req_info[2])

	IPL_SetCmd(IPL_SET_BUF_QUERY, (void *)&prv_buf_req_info);
	IPL_WaitCmdFinish(); // wait the buffer calculate finished
	buf_info.id = prv_buf_req_info.proc_sen_id;
	IPL_GetCmd(IPL_GET_CUR_BUF_INFOR, (void *)&buf_info);
	*ipl_buf_size = buf_info.buf_size;

	// need to check ime group information
	// need to check ime max&min ppb buf num
	// need to check exam_cal_ime_buf_cb(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info) header buffer
	// get ime buffer size // ime path * ppb * IPL_YUV_SIZE
	for (ime_cnt = CAL_IME_PATH_1; ime_cnt < CAL_IME_PATH_MAX_NUM; ime_cnt ++) {
		if (prv_param->ime_path_en & (1 << ime_cnt)) {
			ime_buf_size_calc[ime_cnt] = IPL_YUV_SIZE(prv_param->ime_path_info[ime_cnt].img_fmt
										 , prv_param->ime_path_info[ime_cnt].out_img_lofs, prv_param->ime_path_info[ime_cnt].out_crop_size_v);
			ime_buf_size_calc[ime_cnt] = IPL_CAL_BUF_4WORDALIGN(ime_buf_size_calc[ime_cnt]) * prv_param->ime_ppb_num[ime_cnt];
			ime_buf_size_total += ime_buf_size_calc[ime_cnt];
		}
	}

	*ime_buf_size = ime_buf_size_total;

	if (cal_rt != CAL_ER_NONE) {
		DBG_ERR("error 0x%.8x\r\n", cal_rt);
		return E_SYS;
	} else {
		return E_OK;
	}
}

static void cal_ext_display_close(IPL_PROC_ID id)
{
	DISPLAYER_PARAM layer_param;
	DISP_OBJ *p_disp_obj = disp_getDisplayObject(DISP_1);
	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_ENABLE.bEn = FALSE;
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_ENABLE, &layer_param);
	display_is_closed[id] = TRUE;

}

static void cal_display_cfg(IPL_YCC_IMG_INFO *path2)
{
	DISPLAYER_PARAM layer_param;
	DISP_OBJ *p_disp_obj = disp_getDisplayObject(DISP_1);
	// display after get ready buffer info. (IPL_BUF_IO_PUSH_READY_ADDR)

	//DBG_ERR("addr 0x%.8X width %d height %d line_ofs %d\r\n", path2->pixel_addr[0], path2->ch[0].width, path2->ch[0].height, path2->ch[0].line_ofs);
	if (display_is_closed[display_id]) {
		return;
	}
	if ((path2->ch[0].width == 0) || (path2->ch[0].height == 0) || (path2->ch[0].line_ofs == 0)) {
		return;
	}

	if (!p_disp_obj->isOpened()) {
		p_disp_obj->open();
	}

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_ENABLE.bEn = TRUE;
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_ENABLE, &layer_param);

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_MODE.BufFormat = DISPBUFFORMAT_YUV420PACK;
	layer_param.SEL.SET_MODE.BufMode = DISPBUFMODE_AUTO_HANDSHAKE; //don't care
	layer_param.SEL.SET_MODE.BufNumber = DISPBUFNUM_1; //don't care
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_MODE, &layer_param);

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_WINSIZE.iWinOfsX = 0;
	layer_param.SEL.SET_WINSIZE.iWinOfsY = 0;
	layer_param.SEL.SET_WINSIZE.uiWinWidth = path2->ch[0].width;
	layer_param.SEL.SET_WINSIZE.uiWinHeight = path2->ch[0].height;
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_WINSIZE, &layer_param);

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_BUFSIZE.uiBufWidth = path2->ch[0].width;
	layer_param.SEL.SET_BUFSIZE.uiBufHeight = path2->ch[0].height;
	layer_param.SEL.SET_BUFSIZE.uiBufLineOfs = path2->ch[0].line_ofs;
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_BUFSIZE, &layer_param);

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_VDOBUFADDR.BufSel = DISPBUFADR_0;
	layer_param.SEL.SET_VDOBUFADDR.uiAddrY0 = path2->pixel_addr[0];
	layer_param.SEL.SET_VDOBUFADDR.uiAddrCb0 = path2->pixel_addr[1];
	layer_param.SEL.SET_VDOBUFADDR.uiAddrCr0 = path2->pixel_addr[2];
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_VDOBUFADDR, &layer_param);

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_ACTBUF.ActiveBuf = DISPACTBUF_0;
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_ACTBUF, &layer_param);

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_NORMAL;
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_OUTDIR, &layer_param);

	memset(&layer_param, 0, sizeof(DISPLAYER_PARAM));
	layer_param.SEL.SET_BUFXY.uiBufOfsX = 0;
	layer_param.SEL.SET_BUFXY.uiBufOfsY = 0;
	p_disp_obj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_BUFXY, &layer_param);

	p_disp_obj->load(FALSE);

	//IME output addr no PPB
}

static void cal_isr_event_ime(IPL_PROC_ID id, UINT32 status, UINT32 idx)
{
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;

	if (status & IME_FMD) {
		cal_display_cfg(&ime_path_info[display_id][display_path]);
		buf_id_info.id = id;
		cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
								   , CAL_BUF_ID_PRV_ITEM_IME2_1, &ime_buf_addr, &ime_buf_size);
	}
}

static IPL_ISR_EVENT_FP cal_ext_get_prv_isr_event_fp(IPL_ISR ipl_isr)
{
	switch (ipl_isr) {
	case IPL_ISR_IME:
		return cal_isr_event_ime;
		break;
	case IPL_ISR_SIE1:
	case IPL_ISR_SIE2:
	case IPL_ISR_SIE3:
	case IPL_ISR_SIE4:
	case IPL_ISR_SIE5:
	case IPL_ISR_SIE6:
	case IPL_ISR_SIE7:
	case IPL_ISR_SIE8:
	case IPL_ISR_IFE:
	case IPL_ISR_DCE:
	case IPL_ISR_IPE:
	case IPL_ISR_IFE2:
	case IPL_ISR_RHE:
		DBG_ERR("IPL_ISR %d does not have IPL_ISR_EVENT_FP\r\n", ipl_isr);
		return NULL;
		break;
	case IPL_ISR_UNKNOWN:
	default:
		DBG_ERR("IPL_ISR param. error %d\r\n", ipl_isr);
		return NULL;
		break;
	}
	return NULL;
}

static UINT32 cal_ime_output_buf_cb_1(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_1;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static UINT32 cal_ime_output_buf_cb_2(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_2;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static UINT32 cal_ime_output_buf_cb_3(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_3;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static UINT32 cal_ime_output_buf_cb_4(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_4;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static UINT32 cal_ime_output_buf_cb_5(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_5;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static UINT32 cal_ime_output_buf_cb_6(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_6;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static UINT32 cal_ime_output_buf_cb_7(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_7;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static UINT32 cal_ime_output_buf_cb_8(IPL_BUF_IO_CFG buf_io_cfg, UINT32 buf_info, IPL_IMG_OUT_INFO *img_info)
{
	ER rt = E_OK;
	IPL_PROC_ID id = IPL_ID_8;
	CAL_GET_BUF_ID_INFO buf_id_info = {0};
	UINT32 ime_buf_addr = 0, ime_buf_size = 0;
	switch (buf_io_cfg) {
	case IPL_BUF_IO_GET_NEXT_OUT_ADDR: {
			// (ipl) input ime size
			// (cal) output ime addr
			buf_id_info.id = id;
			rt |= cal_buf()->fp_get_buf_item(cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
											 , CAL_BUF_ID_PRV_ITEM_IME2_1
											 , &ime_buf_addr, &ime_buf_size);
			if (rt != E_OK) {
				DBG_ERR("get_buf_item error %d (buf_id %d buf_item %d)\r\n"
						, cal_ext_get_buf_id_sample(CAL_BUF_ID_PRV_1, buf_id_info)
						, CAL_BUF_ID_PRV_ITEM_IME2_1
						, rt);
			}
			if (ime_buf_size == 0) {
				DBG_ERR("ime_buf_size is zero!\r\n");
			} else if (buf_info > ime_buf_size) {
				DBG_ERR("ime buffer is not enough (ipl size %d > ime buf size %d)\r\n", buf_info, ime_buf_size);
			}
			// avoid buf_handle == 0
			img_info->buf_handle = ime_buf_addr;
			return ime_buf_addr;
		}
		break;
	case IPL_BUF_IO_PUSH_READY_ADDR: {
			// input ime addr (header (HEADER_YUV_DATA) + ime output)
			// no output
			// assign ime addr to display
			switch ((CAL_IME_PATH)img_info->Path_id) {
			case CAL_IME_PATH_2: // output to display
				ime_path_info[id][img_info->Path_id] = img_info->img;
				display_is_closed[id] = FALSE;
				break;
			case CAL_IME_PATH_1: // output to H.264/HEVC
			case CAL_IME_PATH_3: // output to wifi
			case CAL_IME_PATH_4:
			case CAL_IME_PATH_5:
				DBG_ERR("ime path %d output need to implement!\r\n", img_info->Path_id);
				break;
			default:
				break;
			}
		}
		break;
	case IPL_BUF_IO_LOCK_BUF:
	case IPL_BUF_IO_UNLOCK_BUF:
	case IPL_BUF_IO_START:
	case IPL_BUF_IO_STOP:
		break;
	default:
		DBG_ERR("IPL_BUF_IO_CFG error %d\r\n", buf_io_cfg);
		break;
	}

	return 0;
}

static IPL_OUT_BUF_CFG_CB cal_ext_get_prv_output_buf_cfg_fp(IPL_PROC_ID id, IPL_HAL_ENG ipl_hal_eng)
{
	switch (ipl_hal_eng) {
	case IPL_HAL_IME:
		switch (id) {
		case IPL_ID_1:
			return cal_ime_output_buf_cb_1;
			break;
		case IPL_ID_2:
			return cal_ime_output_buf_cb_2;
			break;
		case IPL_ID_3:
			return cal_ime_output_buf_cb_3;
			break;
		case IPL_ID_4:
			return cal_ime_output_buf_cb_4;
			break;
		case IPL_ID_5:
			return cal_ime_output_buf_cb_5;
			break;
		case IPL_ID_6:
			return cal_ime_output_buf_cb_6;
			break;
		case IPL_ID_7:
			return cal_ime_output_buf_cb_7;
			break;
		case IPL_ID_8:
			return cal_ime_output_buf_cb_8;
			break;
		default:
			DBG_ERR("id error %d\r\n", id);
			return NULL;
			break;
		}
		break;
	case IPL_HAL_SIE1:
	case IPL_HAL_SIE2:
	case IPL_HAL_SIE3:
	case IPL_HAL_SIE4:
	case IPL_HAL_SIE5:
	case IPL_HAL_SIE6:
	case IPL_HAL_SIE7:
	case IPL_HAL_SIE8:
	case IPL_HAL_IFE:
	case IPL_HAL_DCE:
	case IPL_HAL_IPE:
	case IPL_HAL_IFE2:
	case IPL_HAL_RHE:
		DBG_ERR("IPL_HAL_ENG %d does not have IPL_OUT_BUF_CFG_CB\r\n", ipl_hal_eng);
		return NULL;
		break;
	case IPL_HAL_UNKNOW:
	default:
		DBG_ERR("IPL_HAL_ENG param. error %d\r\n", ipl_hal_eng);
		return NULL;
		break;
	}
	return NULL;
}

static UINT32 cal_ext_get_inidata_buf(CAL_RW_DATA_INFO *data_info)
{
	ER rt = E_OK;
	UINT32 i, buf_size = 0, senmode_size = 0, dp_max_cnt = 0, ecs_win_num = 65;
	CAL_ALG_DP_SETTING *dp_setting;
	CAL_ALG_ECS_SETTING *ecs_setting;
	SEN_STATUS_PARAM sen_status = {0};
	CAL_SETTING_INFO *setting_info;
	CAL_ITEM cal_item = data_info->sec.cal_item;
	IPL_PROC_ID id = data_info->key.id;
	SENSOR_MODE sen_mode = data_info->key.sensor_mode;

	setting_info = ipl_cal_ext_setting_sample()->fp_get_setting_info(cal_item);
	if (setting_info == NULL) {
		DBG_ERR("%s get cal setting info error\r\n", cal_dbg_str_calitem(cal_item));
		return 0;
	}
	if (setting_info->alg == NULL) {
		DBG_ERR("%s get cal setting alg info error\r\n", cal_dbg_str_calitem(cal_item));
		return 0;
	}

	switch (cal_item) {
	case CAL_ITEM_DP:
		dp_setting = (CAL_ALG_DP_SETTING *)(setting_info->alg);
		sen_status.sensor_mode = sen_mode;
		rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status);
		if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
			DBG_ERR("get sensor status fail id:%d, sensor mode: %d (0x%.8x)\r\n"
					, IPL_UTI_CONV2_SEN_ID(id), sen_status.sensor_mode, sen_status.sensor_info.mode);
			senmode_size = 0;
		} else if ((sen_status.sensor_info.mode->trans_vd[0] == NULL) || (sen_status.sensor_info.mode->trans_hd[0] == NULL)) {
			DBG_ERR("get sensor status fail id:%d, sensor mode: %d (0x%.8x 0x%.8x)\r\n"
					, IPL_UTI_CONV2_SEN_ID(id), sen_status.sensor_mode
					, sen_status.sensor_info.mode->trans_vd[0], sen_status.sensor_info.mode->trans_hd[0]);
			senmode_size = 0;
		} else {
			senmode_size = (sen_status.sensor_info.mode->trans_vd[0]->data_size * sen_status.sensor_info.mode->trans_hd[0]->data_size) / 10;
		}
		for (i = 0; i < SENSOR_MODE_Num - 1 - 1; i++) {
			if (setting_info->sen_mode[i] == sen_mode) {
				break;
			}
		}
		if (dp_setting->max_cnt_ratio[i] == 0) {
			buf_size = 0;
		} else {
			dp_max_cnt = (dp_setting->max_cnt_ratio[i] * senmode_size) / 1000;
			buf_size = dp_max_cnt * 4;
		}
		break;
	case CAL_ITEM_ECS:
		ecs_setting = (CAL_ALG_ECS_SETTING *)(setting_info->alg);
		ECS_MAP_SIZE(ecs_setting->map_size, &ecs_win_num);
		buf_size = (ecs_win_num * ecs_win_num) * sizeof(UINT32);
		break;
	case CAL_ITEM_AWB:
	case CAL_ITEM_ISO:
	case CAL_ITEM_LENSCEN:
		buf_size = 0;
		break;
	case CAL_ITEM_USER1:
	case CAL_ITEM_USER2:
	case CAL_ITEM_USER3:
	case CAL_ITEM_USER4:
	case CAL_ITEM_USER5:
		DBG_ERR("cal_item error 0x%x (if you want to use user define, please add ini data buffer calc)\r\n", cal_item);
		buf_size = 0x100;
		break;
	default:
		DBG_ERR("cal_item error 0x%x\r\n", cal_item);
		break;
	}

	return buf_size;
}

static CAL_STATUS cal_chk_dp_rst(IPL_PROC_ID id, SENSOR_MODE mode, CAL_ALG_DP_RST *alg_rst, CAL_ALG_DP_SETTING *alg)
{
	ER rt = E_OK;
	CAL_SETTING_INFO *setting_info;
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 i, senmode_size, max_dp_cnt;

	if ((alg_rst == NULL) || (alg == NULL)) {
		DBG_ERR("input param error alg_rst 0x%.8x alg 0x%.8x\r\n", alg_rst, alg);
		return CAL_STATUS_NONE;
	}
	setting_info = ipl_cal_ext_setting_sample()->fp_get_setting_info(CAL_ITEM_DP);
	if (setting_info == NULL) {
		DBG_ERR("get cal setting info error\r\n");
		return CAL_STATUS_NONE;
	}
	for (i = 0; i < SENSOR_MODE_Num; i++) {
		if (setting_info->sen_mode[i] == SENSOR_MODE_UNKNOWN) {
			break;
		}
		if (setting_info->sen_mode[i] == mode) {
			sen_status.sensor_mode = mode;
			rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sen_status);
			if (rt != E_OK) {
				DBG_ERR("get sensor status error %d\r\n", rt);
				return CAL_STATUS_NONE;
			} else if (sen_status.sensor_info.mode == NULL) {
				DBG_ERR("get sensor status sen_status.sensor_info.mode error 0x%.8x\r\n", sen_status.sensor_info.mode);
				return CAL_STATUS_NONE;
			} else if ((sen_status.sensor_info.mode->trans_vd[0] == NULL) || (sen_status.sensor_info.mode->trans_hd[0] == NULL)) {
				DBG_ERR("get sensor status sen_status.sensor_info.mode error 0x%.8x 0x%.8x\r\n", sen_status.sensor_info.mode->trans_vd[0], sen_status.sensor_info.mode->trans_hd[0]);
				return CAL_STATUS_NONE;
			}
			senmode_size = (sen_status.sensor_info.mode->trans_vd[0]->data_size * sen_status.sensor_info.mode->trans_hd[0]->data_size);
			max_dp_cnt = (senmode_size / 10000) * alg->max_cnt_ratio[i];

			if (alg_rst->pixel_cnt > max_dp_cnt) {
				DBG_IND("id %d mode %d rst %d alg %d\r\n", id, mode, alg_rst->pixel_cnt, max_dp_cnt);
				return CAL_STATUS_OVERFLOW;
			} else {
				DBG_IND("id %d mode %d rst %d alg %d\r\n", id, mode, alg_rst->pixel_cnt, max_dp_cnt);
				return CAL_STATUS_OK;
			}
		}
	}
	return CAL_STATUS_NONE;
}

static CAL_STATUS cal_chk_ecs_rst(IPL_PROC_ID id, SENSOR_MODE mode, CAL_ALG_ECS_RST *alg_rst, CAL_ALG_ECS_SETTING *alg)
{
	if ((alg_rst == NULL) || (alg == NULL)) {
		DBG_ERR("input param error alg_rst 0x%.8x alg 0x%.8x\r\n", alg_rst, alg);
		return CAL_STATUS_NONE;
	}
	if (alg_rst->ecs_over_spec) {
		return CAL_STATUS_OVERFLOW;
	} else {
		return CAL_STATUS_OK;
	}
	return CAL_STATUS_NONE;
}

static CAL_STATUS cal_chk_awb_rst(IPL_PROC_ID id, SENSOR_MODE mode, CAL_ALG_AWB_RST *alg_rst, CAL_ALG_AWB_SETTING *alg)
{
	if ((alg_rst == NULL) || (alg == NULL)) {
		DBG_ERR("input param error alg_rst 0x%.8x alg 0x%.8x\r\n", alg_rst, alg);
		return CAL_STATUS_NONE;
	}
	DBG_IND("id %d mode %d rst %d %d alg %d %d %d\r\n", id, mode, alg_rst->rgain, alg_rst->bgain, alg->gs.rgain, alg->gs.bgain, alg->threshold);
	if ((alg->gs.rgain == 0) || (alg->gs.bgain == 0)) {
		DBG_ERR("awb golden sample value (rgain %d bgain %d) cannot be zero!\r\n", alg->gs.rgain, alg->gs.bgain);
		return CAL_STATUS_NONE;
	}
	if ((((100 * abs(alg->gs.rgain - alg_rst->rgain)) / alg->gs.rgain) < alg->threshold)
		&& (((100 * abs(alg->gs.bgain - alg_rst->bgain)) / alg->gs.bgain) < alg->threshold)) {
		return CAL_STATUS_OK;
	} else {
		return CAL_STATUS_UNDERFLOW;
	}
	return CAL_STATUS_NONE;
}


static CAL_STATUS cal_chk_iso_rst(IPL_PROC_ID id, SENSOR_MODE mode, CAL_ALG_ISO_RST *alg_rst, CAL_ALG_ISO_SETTING *alg)
{
	if ((alg_rst == NULL) || (alg == NULL)) {
		DBG_ERR("input param error alg_rst 0x%.8x alg 0x%.8x\r\n", alg_rst, alg);
		return CAL_STATUS_NONE;
	}
	if (alg->gs.gain_ratio == 0) {
		DBG_ERR("iso golden sample value (gain_ratio %d) cannot be zero! force to 1000\r\n", alg->gs.gain_ratio);
		alg->gs.gain_ratio = 1000;
	}

	if (((100 * abs(alg->gs.gain_ratio - alg_rst->gain_ratio)) / alg->gs.gain_ratio) <= alg->threshold) {
		return CAL_STATUS_OK;
	} else {
		return CAL_STATUS_UNDERFLOW;
	}
	return CAL_STATUS_NONE;
}


static CAL_STATUS cal_chk_lenscen_rst(IPL_PROC_ID id, SENSOR_MODE mode, CAL_ALG_LENSCEN_RST *alg_rst, CAL_ALG_LENSCEN_SETTING *alg)
{
	if ((alg_rst == NULL) || (alg == NULL)) {
		DBG_ERR("input param error alg_rst 0x%.8x alg 0x%.8x\r\n", alg_rst, alg);
		return CAL_STATUS_NONE;
	}
	if (alg_rst->reliability >= alg->min_reliability) {
		return CAL_STATUS_OK;
	} else {
		return CAL_STATUS_UNDERFLOW;
	}
	return CAL_STATUS_NONE;
}

static CAL_STATUS cal_ext_chk_alg_rst(IPL_PROC_ID id, SENSOR_MODE mode, CAL_ITEM cal_item, void *alg_rst, void *alg)
{
	switch (cal_item) {
	case CAL_ITEM_DP:
		return cal_chk_dp_rst(id, mode, (CAL_ALG_DP_RST *)alg_rst, (CAL_ALG_DP_SETTING *)alg);
		break;
	case CAL_ITEM_ECS:
		return cal_chk_ecs_rst(id, mode, (CAL_ALG_ECS_RST *)alg_rst, (CAL_ALG_ECS_SETTING *)alg);
		break;
	case CAL_ITEM_AWB:
		return cal_chk_awb_rst(id, mode, (CAL_ALG_AWB_RST *)alg_rst, (CAL_ALG_AWB_SETTING *)alg);
		break;
	case CAL_ITEM_ISO:
		return cal_chk_iso_rst(id, mode, (CAL_ALG_ISO_RST *)alg_rst, (CAL_ALG_ISO_SETTING *)alg);
		break;
	case CAL_ITEM_LENSCEN:
		return cal_chk_lenscen_rst(id, mode, (CAL_ALG_LENSCEN_RST *)alg_rst, (CAL_ALG_LENSCEN_SETTING *)alg);
		break;
	case CAL_ITEM_USER1:
	case CAL_ITEM_USER2:
	case CAL_ITEM_USER3:
	case CAL_ITEM_USER4:
	case CAL_ITEM_USER5:
		DBG_ERR("cal_item error 0x%x (if you want to use user define, please add chk alg rst)\r\n", cal_item);
		break;
	default:
		DBG_ERR("cal_item error 0x%x\r\n", cal_item);
		break;
	}
	return CAL_STATUS_NONE;
}

#if 0
- extern
#endif

static CAL_EXT_CTRL_OBJ m_ext_ctrl_obj = {
	cal_ext_get_getraw_ctrl_param,
	cal_ext_get_getraw_buf_size,
	cal_ext_set_getraw_buf_item,
	cal_ext_get_prv_buf_size,
	cal_ext_get_prv_isr_event_fp,
	cal_ext_get_prv_output_buf_cfg_fp,
	cal_ext_get_inidata_buf,
	cal_ext_chk_alg_rst,
	cal_ext_display_close,
};

pCAL_EXT_CTRL_OBJ ipl_cal_ext_ctrl_sample(void)
{
	return &m_ext_ctrl_obj;
}

