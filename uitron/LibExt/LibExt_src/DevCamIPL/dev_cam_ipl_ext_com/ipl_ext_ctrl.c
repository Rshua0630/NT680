/**
    Ctrl sample.

    @file       ipl_ext_ctrl.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "ipl_ext_common.h"
#include "dis_alg.h"

//extern BOOL b_ipl_ctrl_dbg_en;    //ipl control debug enable/disable
CHAR *ipl_dbg_mode_tbl[] = {
	"OFF",
	"PRV",
	"VDO",
	"REC",
	"CAP",
	"MAX_NUM",
	"BUF_QUERY",
	""
};

CHAR *ipl_dbg_trigflg_tbl[] = {
	"NONE",
	"START",
	"STOP",
	"PAUSE",
	"RESTART",
	"RESUME",
	"OFF",
	""
};

BOOL gCapFlg = FALSE;
IPL_OUT_BUF_CFG_CB ipl_out_buf_cfg_cb[IPL_ID_MAX_NUM] = {NULL};
#define total_dbg_data_size 0x400	//total reserve size for debug data
#define sys_dbg_data_size 0x80		//sys dbg data size

void IPL_SIESetOB_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	//auto ob init
	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVAUTO_OB_EN) == ENABLE) {
		OBAvg->StartPix.x = sen_status.sensor_info.mode->ob.top_win[0];
		OBAvg->StartPix.y = sen_status.sensor_info.mode->ob.top_win[1];
		OBAvg->SizeH = (sen_status.sensor_info.mode->ob.top_win[2] - sen_status.sensor_info.mode->ob.top_win[0]);
		OBAvg->SizeV = (sen_status.sensor_info.mode->ob.top_win[3] - sen_status.sensor_info.mode->ob.top_win[1]);
		OBSub->Offset = 0;
		OBAvg->SubRatio_X = 0;
		OBAvg->AvgActThres = 0x80;
		OBAvg->ObSubRatio = 0x20;
		*func_en |= (OBSubSelEn | OBAvgEn);
		*Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SUBFEN);
	}
}

void IPL_SIESetCA_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *pPath)
{
	THRESHOL_SET CA_ThrInit = {0, 4095};

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_EN) == ENABLE) {
		cacc->CACrop.StartPix.x = 0;
		cacc->CACrop.StartPix.y = 0;
		cacc->CACrop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
		cacc->CACrop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V);

		cacc->CA_WIN.WinNum_X = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_WINXNUM);
		cacc->CA_WIN.WinNum_Y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_WINYNUM);

		//ca threshold init
		cacc->CA_TH_INFO.Gth = CA_ThrInit;
		cacc->CA_TH_INFO.Rth = CA_ThrInit;
		cacc->CA_TH_INFO.Bth = CA_ThrInit;
		cacc->CA_TH_INFO.Pth = CA_ThrInit;
		pPath->bVig = DISABLE;
		pPath->bCaThreshold = ENABLE;

		*func_en |= CAEn;
		*Update |= (SIE_STATS_CA_SIZE | SIE_STATS_CA_);
	} else {
		*func_en &= ~CAEn;
	}
	*Update |= SIE_SUBFEN;
}

void IPL_SIESetLA_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_LACC *lacc, SIE_STATS_PATH_INFO *pPath)
{
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_EN) == ENABLE) {
		lacc->LACrop.StartPix.x = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H) - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H)) >> 1;
		lacc->LACrop.StartPix.y = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V) - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V)) >> 1;
		lacc->LACrop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H);
		lacc->LACrop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V);

		lacc->LA_WIN.WinNum_X = IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_WINXNUM);
		lacc->LA_WIN.WinNum_Y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_WINYNUM);

		pPath->bLaCg = ENABLE;
		pPath->bLaGama1 = ENABLE;
		pPath->bHistogramSel = STATS_HISTO_POST_GAMMA;
		pPath->bLa1SrcSel = STATS_LA1_POST_CG;

		*func_en |= (LAEn | HistoYEn);
		*Update |= (SIE_STATS_LA_ | SIE_STATS_LA_SIZE | SIE_STATS_LA_CG | SIE_STATS_LA_GAMMA);
	} else {
		*func_en &= ~LAEn;
	}
	*Update |= SIE_SUBFEN;
}

void IPL_SIESetEthOut_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_ETH *eth, SIE_VACC *va, SIE_EDGETHRE *eththr)
{
	SIE_EDGETHRE eth_threshold = {16, 48, 128};

	if (id != IPL_ID_1) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH4_EN, DISABLE);
		return;
	}

	if ((IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH4_EN) == ENABLE) && (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH4_SRC) == CH4_ETH)) {
		eth->ETH_OUT_FMT = ETH_2BIT;
		//eth->ETH_OUT_SEL = ETH_OUT_SEL_FORMAT;
		eth->ETH_OUT_SEL = ETH_OUT_ALL_PIXEL;
		eth->ETH_HOUT_SEL = ETH_H_EVEN;
		eth->ETH_VOUT_SEL = ETH_V_EVEN;

		va->VACrop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
		va->VACrop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V) - 4;
		va->VACrop.StartPix.x = 0;
		va->VACrop.StartPix.y = 0;
		*eththr = eth_threshold;

		*func_en |= ETHEn;
		*InteEn |= (SIE__INTE_DRAM_OUT4_END);
		*Update |= (SIE_OUTADDR_CH4 | SIE_SUBFEN | SIE_OUTINFO_CH4 | SIE_STATS_ETH_ | SIE_STATS_VA_ETH_SIZE);
	} else {
		*func_en &= ~(ETHEn);
		*InteEn &= ~(SIE__INTE_DRAM_OUT4_END);
		*Update |= (SIE_SUBFEN);
	}
}

void IPL_SIESetBP_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
	//bp
	Sub->BP.BP1 = 16;
	Sub->BP.BP2 = 32;
	Sub->BP.BP3 = 48;

	*InteEn |= (SIE__INTE_BP1 | SIE__INTE_BP2 | SIE__INTE_BP3);
	*Update |= (SIE_BP_ | SIE_SUBFEN);
}

void IPL_SIESetDMAOutSrc_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update)
{
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE) {
		*func_en |= DMAOutCh0En;
		*Update |= (SIE_SUBFEN | SIE_OUTINFO_CH0);
	} else {
		*func_en &= ~DMAOutCh0En;
		*Update |= SIE_SUBFEN;
	}
}

void IPL_SIESetDMAOutCh3_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_HAL_YOUT_INFO *yinfo)
{
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_EN) == ENABLE) {
		if (id <= IPL_ID_4 && IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SRC) == CH3_YOUT_ACC) {
			yinfo->win_num_x = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM);
			yinfo->win_num_y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM);
			yinfo->crp_size_x = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
			yinfo->crp_size_y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
			yinfo->cgain_en = DISABLE;

			*func_en |= DMAOutCh3En | SHDRYOutEn;
			*Update |= (SIE_SUBFEN | SIE_CH3_YOUT_SIZE | SIE_OUTINFO_CH3);
		}
	}
}

void IPL_SIESetDP_FCB(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
	ER rt = E_OK;
	UINT32 data_addr = 0;
	CAL_RW_HEADER_INFO rw_header_info = {0};

	if (id >= IPL_ID_5) {
		CHKPNT;
		*func_en &= ~DpcEn;
		*Update &= ~SIE_DPC_; // update defect pixel concealment eng: SIE1234
		return;
	}

	rw_header_info.sec.cal_item = CAL_ITEM_DP;
	rw_header_info.key.id = (IPL_PROC_ID)id;
	rw_header_info.key.cal_cond = CAL_COND_DFT;
	rw_header_info.key.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);

	rt =  ipl_cal_getcalrst(id, &rw_header_info, &data_addr);

	if (rt == E_OK) {
		if ((rw_header_info.value.cal_status == CAL_STATUS_OK) && (data_addr != 0) && (rw_header_info.value.read_data_size != 0)) {
			Sub->Dpc.DP_TblAddr = data_addr;
			Sub->Dpc.TblSize = rw_header_info.value.read_data_size;
			Sub->Dpc.Weight = _50F00_PERCENT;
			*func_en |= DpcEn;
			*Update |= (SIE_DPC_ | SIE_SUBFEN);
		} else {
			*func_en &= ~DpcEn;
			*Update |= (SIE_DPC_ | SIE_SUBFEN);
		}
	} else {
		if (rt != E_NOSPT) { // E_NOSPT: CALIBRATION_FUNC DISABLE
			DBG_ERR("ipl_cal_getcalrst error %d id %d\r\n", rt, id);
		}
		*func_en &= ~DpcEn;
		*Update |= (SIE_DPC_ | SIE_SUBFEN);
	}
}

#if 0
#endif
void IPL_SIEPatGen_FCB(UINT32 id, SieFuncEn *func_en, SIE_UPDATE *Update, PAT_GEN_INFO *PatGen)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if (sen_status.sensor_info.data_type == SENSOR_DATA_SIEPATGEN) {
		*Update |= SIE_PATGEN;
		*func_en |= PatGenEn;
		PatGen->type = SIE_PAT_COLORBAR;
		PatGen->val = sen_status.sensor_info.mode->valid_width / 7;
		PatGen->val &= ~0x1;
	} else {
		*func_en &= ~PatGenEn;
	}
}


void IPL_SIESetRawEnc_FCB(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *Update, SIE_RAWENC *RawEnc)
{
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_EN) == ENABLE) {
		*Update |= SIE_SUBFEN;
		*func_en |= RawEncEn;
	} else {
		*func_en &= ~RawEncEn;
	}
}


void IPL_SIECCIR_FCB(UINT32 id, SieFuncEn *func_en,  SIE_UPDATE *Update, SIE_DVI *pDvi)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if ((sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR) && (sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR_INTERLACE)) {
		return;
	}

	switch (sen_status.sensor_info.mode->dvi->fmt) {
	case SENSOR_DVI_CCIR601:
		pDvi->SIE_DVIFmt = SIE_CCIR601;
		break;

	case SENSOR_DVI_CCIR656_EAV:
		pDvi->SIE_DVIFmt = SIE_CCIR656_EAV;
		break;

	case SENSOR_DVI_CCIR656_ACT:
		pDvi->SIE_DVIFmt = SIE_CCIR656_ACT;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIFmt = %d\r\n", id, sen_status.sensor_info.mode->dvi->fmt);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->mode) {
	case SENSOR_DVI_MODE_SD:
		pDvi->SIE_DVIMode = SIE_SDMode;
		break;

	case SENSOR_DVI_MODE_HD:
		pDvi->SIE_DVIMode = SIE_HDMode;
		break;

	case SENSOR_DVI_MODE_HD_INV:
		pDvi->SIE_DVIMode = SIE_HDMode_Inv;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIMode = %d\r\n", id, sen_status.sensor_info.mode->dvi->mode);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->data_fmt) {
	case SENSOR_DVI_YUYV:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
		break;

	case SENSOR_DVI_YVYU:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
		break;

	case SENSOR_DVI_UYVY:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
		break;

	case SENSOR_DVI_VYUY:
		pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
		break;

	default:
		DBG_ERR("id: %d, unsupport DVIOUT_SWAPSEL = %d\r\n", id, sen_status.sensor_info.mode->dvi->data_fmt);
		break;
	}

	*Update |= SIE_DVI_;
	*func_en |= DviEn;
	pDvi->FldEn = sen_status.sensor_info.mode->dvi->fld_en;
	pDvi->FldSelEn = sen_status.sensor_info.mode->dvi->fld_inv_en;
	pDvi->OutSplit = sen_status.sensor_info.mode->dvi->out_split_en;
	pDvi->CCIR656VdSel = sen_status.sensor_info.mode->dvi->ccir656_info.vd_mode;
	pDvi->DataPeriod = sen_status.sensor_info.mode->dvi->ahd_info.combine_num;
	//pDvi->DataIdx = sen_status.sensor_info.mode->dvi->ahd_info.select_id;
    if(pDvi->DataPeriod != 0){
        pDvi->DataIdx = id % (pDvi->DataPeriod+1);//temp solution. it should be modified by real case
    }
    else{
        pDvi->DataIdx = 0;
    }
	pDvi->AutoAlign = sen_status.sensor_info.mode->dvi->ahd_info.auto_align;
}

#if 0
#endif
void IPL_CtrlInitIpc_SIEAct(UINT32 id, SENSOR_INFO *SenInfo, SIE_ACT_MODE *ActMode)
{
	INT32 base_id;
	SEN_STATUS_PARAM sensor_status;
	ER rt = E_OK;

	base_id = IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID);
	sensor_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(base_id), SEN_GET_STATUS, (void *)&sensor_status);
	if (rt != E_OK || sensor_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	*ActMode = IPL_UTI_CONV2_SEN_ACT_MODE(sensor_status.sensor_info.sig_mode);
}

void IPL_CtrlInitIpc_SIEIn(UINT32 id, SENSOR_INFO *SenInfo, SIE_WINDOW *Win, BAYER_TYPE *bayer_type)
{
	UINT32 i, frm_cnt = 0;
	INT32 crop_shift_x, crop_shift_y;

	if (SenInfo->mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		for (i = IPL_ID_1 ; i < SEN_MFRAME_MAX_NUM ; i++) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT) & (1 << i)) {
				if (i == id) {
					break;
				}
				frm_cnt++;
			}
		}
	}

	if(SenInfo->mode->dvi != NULL) {
		//adjust valid_width when HD mode(16bit), align to 8bit
		if(SenInfo->mode->dvi->mode != SENSOR_DVI_MODE_SD){
			Win->Act.StartPix.x = SenInfo->mode->trans_hd[frm_cnt]->data_start << 1;
			Win->Act.SizeH = SenInfo->mode->trans_hd[frm_cnt]->data_size << 1;
		} else {
			Win->Act.StartPix.x = SenInfo->mode->trans_hd[frm_cnt]->data_start;
			Win->Act.SizeH = SenInfo->mode->trans_hd[frm_cnt]->data_size;
		}
	} else {
		Win->Act.StartPix.x = SenInfo->mode->trans_hd[frm_cnt]->data_start;
		Win->Act.SizeH = SenInfo->mode->trans_hd[frm_cnt]->data_size;
	}

	Win->Act.StartPix.y = SenInfo->mode->trans_vd[frm_cnt]->data_start;
	Win->Act.SizeV = SenInfo->mode->trans_vd[frm_cnt]->data_size;

	Win->Act.CFA = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);

	Win->Crop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
	Win->Crop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
	Win->Crop.CFA = Win->Act.CFA;

	if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV)) {
		DBG_ERR("id: %d, act size (%d %d) < crop Size(%d %d)\r\n", id, Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
		Win->Crop.StartPix.x = 0;
		Win->Crop.StartPix.y = 0;
		Win->Crop.SizeH = Win->Act.SizeH;
		Win->Crop.SizeV = Win->Act.SizeV;
	} else {
		Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
		Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;

		if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X) != 0 || IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y) != 0) {
			crop_shift_x = IPL_Ctrl_cal_input_offset(Win->Act.SizeH, Win->Crop.SizeH, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), &Win->Crop.StartPix.x);
			crop_shift_y = IPL_Ctrl_cal_input_offset(Win->Act.SizeV, Win->Crop.SizeV, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), &Win->Crop.StartPix.y);

			if (crop_shift_x != 0 || crop_shift_y != 0) {
				DBG_WRN("Shift Overflow, start: (%d, %d), dest_size: (%d, %d), src_size: (%d, %d)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), Win->Crop.SizeH, Win->Crop.SizeV, Win->Act.SizeH, Win->Act.SizeV);
			}
		}
	}

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_X, Win->Crop.StartPix.x);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_Y, Win->Crop.StartPix.y);

	switch (SenInfo->mode->stpix) {
	case SENSOR_STPIX_R:
	case SENSOR_STPIX_GR:
	case SENSOR_STPIX_GB:
	case SENSOR_STPIX_B:
		*bayer_type = BAYER_FORMAT;
		break;

	case SENSOR_STPIX_RGBIR_RIR:
	case SENSOR_STPIX_RGBIR_RG:
		*bayer_type = RGBIR_FORMAT;
		break;

	case SENSOR_STPIX_RCCB_RC:
	case SENSOR_STPIX_RCCB_CR:
	case SENSOR_STPIX_RCCB_CB:
	case SENSOR_STPIX_RCCB_BC:
		*bayer_type = RCCB_FORMAT;
		break;

	default:
		*bayer_type = BAYER_FORMAT;
		break;
	}
}

void IPL_CtrlInitIpc_SIESignal(UINT32 id, IPP_SIE_SIGNAL *Signal)
{
	Signal->Sie_HDVD.VD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_INV);
	Signal->Sie_HDVD.HD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_INV);

	Signal->SensorPhase.VD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_PHASE);
	Signal->SensorPhase.HD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_PHASE);
	Signal->SensorPhase.Data = IPL_CtrlGetInfor(id, IPLCTRL_SIE_DATA_PHASE);
}

void IPL_CtrlInitIpc_SIECh0PPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, UINT32 *buf_num, UINT32 *addr)
{
	UINT32 buf_size;
	UINT32 i, raw_buf_num = 0;

	raw_info->width = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
	raw_info->height = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
	raw_info->bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT);
	raw_info->line_offset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
	raw_info->st_pix = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM) != 0) {
		raw_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM);
		*buf_num = raw_buf_num;
		for (i = 0; i < raw_buf_num; i ++) {
			IPL_BufGet(id, IPLBUF_SIE_CH0_1 + i, &addr[i], &buf_size);
		}
	}
}

void IPL_CtrlInitIpc_SIECh1PPB(UINT32 id, IPL_RAW_IMG_INFO *dma_out, UINT32 *addr)
{
	UINT32 buf_size;
	UINT32 i, ch1_buf_num;

	ch1_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_BUFNUM);
	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_EN) == ENABLE) {
		dma_out->line_offset = IPL_UTI_RAWH_CONV2_LOFS(IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H), IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT));
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_EN) == ENABLE || IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_EN) == ENABLE) {
		for (i = 0; i < ch1_buf_num; i ++) {
			IPL_BufGet(id, (IPLBUF_SIE_CH1_1 + i), &addr[i], &buf_size);
			memset((void*)addr[i], 0x80, buf_size);
		}

	}
}

void IPL_CtrlInitIpc_SIECh2PPB(UINT32 id, UINT32 *addr)
{
	UINT32 buf_size;
	UINT32 i, ch2_buf_num;

	ch2_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_BUFNUM);

	for (i = 0; i < ch2_buf_num; i ++) {
		IPL_BufGet(id, (IPLBUF_SIE_CH2_1 + i), &addr[i], &buf_size);
	}
}

void IPL_CtrlInitIpc_SIECh3PPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, UINT32 *addr, UINT32 *buf_num, CH3_SRC *out_src)
{
	UINT32 buf_size;
	UINT32 i, ch3_buf_num;

	ch3_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_BUFNUM);
	*buf_num = ch3_buf_num;
	*out_src = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SRC);

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SRC) == CH3_SCALED_IMG) {
		raw_info->width = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SIZE_H);
		raw_info->height = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SIZE_V);
		raw_info->bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_PRVBIT);
		raw_info->line_offset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SIZE_HLOS);
		raw_info->st_pix = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);
	}

	for (i = 0; i < ch3_buf_num; i ++) {
		IPL_BufGet(id, (IPLBUF_SIE_CH3_1 + i), &addr[i], &buf_size);
	}
}

void IPL_CtrlInitIpc_SIECh4PPB(UINT32 id, UINT32 *addr, CH4_SRC *out_src)
{
	UINT32 buf_size;
	UINT32 i, ch4_buf_num;

	ch4_buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH4_BUFNUM);
	*out_src = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH4_SRC);

	for (i = 0; i < ch4_buf_num; i ++) {
		IPL_BufGet(id, (IPLBUF_SIE_CH4_1 + i), &addr[i], &buf_size);
	}
}

void IPL_CtrlInitIpc_SIECh5PPB(UINT32 id, UINT32 *addr)
{
	UINT32 buf_size;
	UINT32 i, buf_num = 0;

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM) != 0) {
		buf_num = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM);
		for (i = 0; i < buf_num; i ++) {
			IPL_BufGet(id, IPLBUF_SIE_CH5_1 + i, &addr[i], &buf_size);
		}
	}
}

void IPL_CtrlInitIpc_IPLInPPB(UINT32 id, IPL_RAW_IMG_INFO *raw_info, Coordinate *st_pos, RHE_HAL_FUNC_MODE *rhe_mode)
{
	INT32 crop_shift_x, crop_shift_y;

	raw_info->width = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H);
	raw_info->height = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V);
	st_pos->x = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H) - raw_info->width) >> 1;
	st_pos->y = (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V) - raw_info->height) >> 1;
	raw_info->bit = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_BIT);
	raw_info->line_offset = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_HLOS);
	raw_info->st_pix = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVSTPIX);

	if (IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_X) != 0 && IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_Y) != 0) {
		crop_shift_x = IPL_Ctrl_cal_input_offset(IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H), raw_info->width, IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_X), &st_pos->x);
		crop_shift_y = IPL_Ctrl_cal_input_offset(IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V), raw_info->height, IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_Y), &st_pos->y);

		if (crop_shift_x != 0 || crop_shift_y != 0) {
			DBG_WRN("Shift Overflow, start: (%d, %d), dest_size: (%d, %d), src_size: (%d, %d)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_X), IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_OFS_Y), raw_info->width, raw_info->height, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H), IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V));
		}
	}

	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_X, st_pos->x);
	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_Y, st_pos->y);

	if (IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) {

        if      (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_OFF)
            *rhe_mode = RHE_HAL_FUNC_MODE_LINEAR;

        else if (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_ON)
		*rhe_mode = RHE_HAL_FUNC_MODE_SHDR;

        else if (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_WDR)
            *rhe_mode = RHE_HAL_FUNC_MODE_PHDR;

        else {
            DBG_ERR("Wrong DR setting(%x, %u)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN), IPL_AlgGetUIInfo(id, IPL_SEL_SHDR));
            *rhe_mode = RHE_HAL_FUNC_MODE_LINEAR;
        }
    }
    else if (IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) {
		*rhe_mode = RHE_HAL_FUNC_MODE_PHDR;
	} else {
		*rhe_mode = RHE_HAL_FUNC_MODE_LINEAR;
	}
}

void IPL_CtrlInitIpc_IPLStrpInfo(UINT32 id, IPL_STRP_INFO *strp_info)
{
    IPL_GetDceStrpInfo(id, strp_info);
}

void IPL_CtrlInitIpc_RHESubOutPPB(UINT32 id, UINT32 *addr, UINT32 *width, UINT32 *height)
{
	UINT32 buf_size;

	IPL_BufGet(id, IPLBUF_RHESUB_1, &addr[0], &buf_size);
	IPL_BufGet(id, IPLBUF_RHESUB_2, &addr[1], &buf_size);

	*width = IPL_CtrlGetInfor(id, IPLCTRL_RHE_DEFOG_SUB_OUT_SIZE);
	*height = IPL_CtrlGetInfor(id, IPLCTRL_RHE_DEFOG_SUB_OUT_SIZE);
}

void IPL_CtrlInitIpc_DCEIn(UINT32 id, SENSOR_INFO *SenInfo, USIZE *pRef_1x, Coordinate *pCenter_1x, iCoordinate *pCenter_1xOFS, UINT32 *HRatio, UINT32 *VRatio)
{
	SEN_STATUS_PARAM sensor_status = {0};
	ER rt = E_OK;

	if (SenInfo->data_type == SENSOR_DATA_SIEPATGEN) {
		pRef_1x->w = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
		pRef_1x->h = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
	} else {
		sensor_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
		rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sensor_status);
		if (rt != E_OK || sensor_status.sensor_info.mode == NULL) {
			DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
			return;
		}
		pRef_1x->w = sensor_status.sensor_info.width_1x;
		pRef_1x->h = sensor_status.sensor_info.height_1x;
	}

	pCenter_1x->x = (pRef_1x->w >> 1);
	pCenter_1x->y = (pRef_1x->h >> 1);
	pCenter_1xOFS->x = 0;
	pCenter_1xOFS->y = 0;
	*HRatio = SenInfo->mode->ratio.ratio_h;
	*VRatio = SenInfo->mode->ratio.ratio_v;
}

void IPL_CtrlInitIpc_VR360Info(UINT32 id, VR360_DATA *VR360Data)
{
    VR360Data->p1.uiHofs        = 0;
    VR360Data->p1.uiHeight      = 1024;
    VR360Data->p1.uiExtWidth    = IPL_CtrlGetInfor(id, IPLCTRL_VR360_IME_P1_EXT_WIDTH);
    VR360Data->p2.uiHofs        = 0;
    VR360Data->p2.uiHeight      = 1024;
    VR360Data->p2.uiExtWidth    = IPL_CtrlGetInfor(id, IPLCTRL_VR360_IME_P2_EXT_WIDTH);
    VR360Data->p3.uiHofs        = 0;
    VR360Data->p3.uiHeight      = 1024;
    VR360Data->p3.uiExtWidth    = IPL_CtrlGetInfor(id, IPLCTRL_VR360_IME_P3_EXT_WIDTH);
    VR360Data->p4.uiHofs        = 0;
    VR360Data->p4.uiHeight      = 1024;
    VR360Data->p4.uiExtWidth    = IPL_CtrlGetInfor(id, IPLCTRL_VR360_IME_P4_EXT_WIDTH);
    VR360Data->p5.uiHofs        = 0;
    VR360Data->p5.uiHeight      = 1024;
    VR360Data->p5.uiExtWidth    = IPL_CtrlGetInfor(id, IPLCTRL_VR360_IME_P5_EXT_WIDTH);

    if (IPL_CtrlGetInfor(id, IPLCTRL_VR360_4SPLIT_EN))
        VR360Data->ext_func |= VR360_EXT_FUNC_4SPLIT;
}

void IPL_CtrlInitIpc_DCEDeWarpInfo(UINT32 id, DCE_Stripe *StripeInfo, _2DLUT_PARAM *Dce2DInfo)
{

}

void IPL_CtrlInitIpc_IPEEthPPB(UINT32 id, UINT32 *Addr, UINT32 *LineOfs)
{
	UINT32 buf_size;

	IPL_BufGet(id, IPLBUF_IPEETH_1, &Addr[0], &buf_size);
	IPL_BufGet(id, IPLBUF_IPEETH_2, &Addr[1], &buf_size);

	*LineOfs = IPL_CtrlGetInfor(id, IPLCTRL_IPE_ETH_OUT_SIZE_HLOS);
}

void IPL_CtrlInitIpc_IMESubFuncPPB(UINT32 id, UINT32 buf_idx, UINT32 *PP0Addr, UINT32 *PP1Addr)
{
	UINT32 buf_size;

	if (PP0Addr != NULL) {
		IPL_BufGet(id, buf_idx, PP0Addr, &buf_size);
	}

	if (PP1Addr != NULL) {
		IPL_BufGet(id, buf_idx + 1, PP1Addr, &buf_size);
	}
}

void IPL_CtrlInitIpc_IMEPxOutInfo(UINT32 id, UINT32 path_en_idx, IPLC_IME_PATH_INFO *path_info)
{
	UINT32 Px;
	IPLC_IME_PATH_INFO YCCInfo = {0};

	UINT32 Pxidx[5][10] = {
		{IPLCTRL_IME_P1_EN, IPLCTRL_IME_P1_SAMPLE_RATE, IPLCTRL_IME_P1_IMG_FMT, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLCTRL_IME_P1_OUT_CROP_STR_X, IPLCTRL_IME_P1_OUT_CROP_STR_Y, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V},
		{IPLCTRL_IME_P2_EN, IPLCTRL_IME_P2_SAMPLE_RATE, IPLCTRL_IME_P2_IMG_FMT, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLCTRL_IME_P2_OUT_CROP_STR_X, IPLCTRL_IME_P2_OUT_CROP_STR_Y, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V},
		{IPLCTRL_IME_P3_EN, IPLCTRL_IME_P3_SAMPLE_RATE, IPLCTRL_IME_P3_IMG_FMT, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLCTRL_IME_P3_OUT_CROP_STR_X, IPLCTRL_IME_P3_OUT_CROP_STR_Y, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V},
		{IPLCTRL_IME_P4_EN, IPLCTRL_IME_P4_SAMPLE_RATE, IPLCTRL_IME_P4_IMG_FMT, IPLCTRL_IME_P4_OUT_SIZE_H, IPLCTRL_IME_P4_OUT_SIZE_V, IPLCTRL_IME_P4_OUT_SIZE_HLOS, IPLCTRL_IME_P4_OUT_CROP_STR_X, IPLCTRL_IME_P4_OUT_CROP_STR_Y, IPLCTRL_IME_P4_OUT_CROP_H, IPLCTRL_IME_P4_OUT_CROP_V},
		{IPLCTRL_IME_P5_EN, IPLCTRL_IME_P5_SAMPLE_RATE, IPLCTRL_IME_P5_IMG_FMT, IPLCTRL_IME_P5_OUT_SIZE_H, IPLCTRL_IME_P5_OUT_SIZE_V, IPLCTRL_IME_P5_OUT_SIZE_HLOS, IPLCTRL_IME_P5_OUT_CROP_STR_X, IPLCTRL_IME_P5_OUT_CROP_STR_Y, IPLCTRL_IME_P5_OUT_CROP_H, IPLCTRL_IME_P5_OUT_CROP_V},
	};

	switch (path_en_idx) {
	case IPLCTRL_IME_P1_EN:
		Px = 0;
		break;

	case IPLCTRL_IME_P2_EN:
		Px = 1;
		break;

	case IPLCTRL_IME_P3_EN:
		Px = 2;
		break;

	case IPLCTRL_IME_P4_EN:
		Px = 3;
		break;

	case IPLCTRL_IME_P5_EN:
		Px = 4;
		break;

	default:
		DBG_ERR("id: %d, PathEnidx error %d\r\n", id, path_en_idx);
		return;
	}

	//calculate ycc information
	YCCInfo.enable = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
	YCCInfo.sample_rate = IPL_CtrlGetInfor(id, Pxidx[Px][1]);
	YCCInfo.img_info.type = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
	YCCInfo.img_info.ch[IPL_IMG_YUV_Y].width = IPL_CtrlGetInfor(id, Pxidx[Px][3]);
	YCCInfo.img_info.ch[IPL_IMG_YUV_Y].height = IPL_CtrlGetInfor(id, Pxidx[Px][4]);
	YCCInfo.img_info.ch[IPL_IMG_YUV_Y].line_ofs = IPL_CtrlGetInfor(id, Pxidx[Px][5]);
	YCCInfo.img_info.ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.img_info.type, YCCInfo.img_info.ch[IPL_IMG_YUV_Y]);

	if (path_en_idx == IPLCTRL_IME_P1_EN && IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_ENC_EN) == ENABLE) {
		YCCInfo.img_info.ch[IPL_IMG_YUV_Y].line_ofs = ALIGN_CEIL(YCCInfo.img_info.ch[IPL_IMG_YUV_Y].line_ofs, 128) << 2;
		YCCInfo.img_info.ch[IPL_IMG_YUV_U].line_ofs = ALIGN_CEIL(YCCInfo.img_info.ch[IPL_IMG_YUV_U].line_ofs, 128) * 3 / 4;
	}
	YCCInfo.img_info.ch[IPL_IMG_YUV_V] = YCCInfo.img_info.ch[IPL_IMG_YUV_U];

	YCCInfo.crop_info.Start.x = IPL_CtrlGetInfor(id, Pxidx[Px][6]);
	YCCInfo.crop_info.Start.y = IPL_CtrlGetInfor(id, Pxidx[Px][7]);
	YCCInfo.crop_info.Width = IPL_CtrlGetInfor(id, Pxidx[Px][8]);
	YCCInfo.crop_info.Height = IPL_CtrlGetInfor(id, Pxidx[Px][9]);

	*path_info = YCCInfo;
}

/* IME Output Image Set As below
-----------------------------
|           L_Sprt          |
-----------------------------
|             R             |
-----------------------------
|           |   |           |
|           |   |           |
|           |   |           |
|      L    | S |   R_Sprt  |
|           |   |           |
|           |   |           |
|           |   |           |
-----------------------------
*/
void IPL_CtrlInitIpc_IMEPxOutInfo_Stitch(UINT32 id, UINT32 path_en_idx, IPL_YCC_IMG_INFO *path_info, STITCH_IMG *stitch_img)
{
#if 0   //Jarkko tmp
	UINT32 buf_size, Px;
	IPL_IMG_BUF_INFO buf_info = {0};
	IPL_YCC_IMG_INFO YCCInfo = {0};
	UINT32 YSprtAddr, CSprtAddr;
	UINT32 SprtR = 0, SprtL = 0, SeamH = 0;   //Get from Stitch lib

	UINT32 Pxidx[3][10] = {
		{IPLCTRL_IME_P1_IMG_FMT, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLBUF_IMEP1_1, IPLCTRL_IME_P1_SPRT_YBUFOFS, IPLCTRL_IME_P1_SPRT_CBUFOFS, IPLBUF_IMEP1_SPRT, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V},
		{IPLCTRL_IME_P2_IMG_FMT, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLBUF_IMEP2_1, IPLCTRL_IME_P2_SPRT_YBUFOFS, IPLCTRL_IME_P2_SPRT_CBUFOFS, IPLBUF_IMEP2_SPRT, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V},
		{IPLCTRL_IME_P3_IMG_FMT, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLBUF_IMEP3_1, IPLCTRL_IME_P3_SPRT_YBUFOFS, IPLCTRL_IME_P3_SPRT_CBUFOFS, IPLBUF_IMEP3_SPRT, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V},
	};

	switch (path_en_idx) {
	case IPLCTRL_IME_P1_EN:
		Px = 0;
		break;

	case IPLCTRL_IME_P2_EN:
		Px = 1;
		break;

	case IPLCTRL_IME_P3_EN:
		Px = 2;
		break;

	default:
		DBG_ERR("id: %d, PathEnidx error %d\r\n", id, path_en_idx);
		return;
	}
#if 0   //Jarkko tmp
	SeamH = IPL_ALIGN_ROUNDUP(Stitch_GetStitchAreaWidthInPixel(IPL_CtrlGetInfor(id, Pxidx[Px][1])), 4);
	SprtL = IPL_ALIGN_ROUNDUP(Stitch_GetLeftImgCutPointInPixel(IPL_CtrlGetInfor(id, Pxidx[Px][1])), 4);
	SprtR = IPL_ALIGN_ROUNDUP(Stitch_GetRightImgCutPointInPixel(IPL_CtrlGetInfor(id, Pxidx[Px][1])), 4);
#endif
	if (id == IPL_ID_1) {
		//calculate Separate img information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtL;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = YCCInfo.Ch[IPL_IMG_YUV_Y].Width;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][7], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		stitch_img->StPos = IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtL;
		stitch_img->LineOfsY = YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs;
		stitch_img->LineOfsCC = YCCInfo.Ch[IPL_IMG_YUV_U].LineOfs;

		YSprtAddr = YCCInfo.PixelAddr[0];
		CSprtAddr = YCCInfo.PixelAddr[1];

		//calculate main output address
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtL;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (YCCInfo.Ch[IPL_IMG_YUV_Y].Width + (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR)) + SeamH;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][4], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		IPL_CtrlSetInfor(id, Pxidx[Px][5], (YCCInfo.PixelAddr[0] - YSprtAddr));
		IPL_CtrlSetInfor(id, Pxidx[Px][6], (YCCInfo.PixelAddr[1] - CSprtAddr));

		//calculate ycc information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][8]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][9]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtL) + (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR) + SeamH;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
	} else if (id == IPL_ID_2) {
		//calculate Separate img information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(IPL_ID_1, Pxidx[Px][1]) - SprtL) + (IPL_CtrlGetInfor(id, Pxidx[Px][1]) - SprtR) + SeamH;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][4], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		stitch_img->StPos = SprtR;
		stitch_img->LineOfsY = YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs;
		stitch_img->LineOfsCC = YCCInfo.Ch[IPL_IMG_YUV_U].LineOfs;

		YSprtAddr = YCCInfo.PixelAddr[0];
		CSprtAddr = YCCInfo.PixelAddr[1];

		//calculate main output address
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtR;
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][2]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = YCCInfo.Ch[IPL_IMG_YUV_Y].Width;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
		IPL_BufGet(id, Pxidx[Px][7], &buf_info.Buf[0], &buf_size);
		buf_info.Buf[1] = 0;
		buf_info.Buf[2] = 0;
		IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, buf_info, &YCCInfo);

		IPL_CtrlSetInfor(id, Pxidx[Px][5], (YSprtAddr - YCCInfo.PixelAddr[0]));
		IPL_CtrlSetInfor(id, Pxidx[Px][6], (CSprtAddr - YCCInfo.PixelAddr[1]));

		//calculate ycc information
		YCCInfo.type = IPL_CtrlGetInfor(id, Pxidx[Px][0]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(id, Pxidx[Px][8]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(id, Pxidx[Px][9]);
		YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = SprtR;
		YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
		YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
	}

	*path_info = YCCInfo;
#endif
}

#if 0
#endif
void IPL_CtrlInitIpc_SIEBurstLength_Cap(UINT32 id, SIE_UPDATE *Update, SENSOR_INFO *SenInfo, SIE_BURSTLENGTH_SET *Length)
{
	//unnecessary to set burstlength
}

void IPL_CtrlInitIpc_SIEAct_Cap(UINT32 id, SIE_UPDATE *Update, SIE_ACT_MODE *ActMode)
{
	INT32 base_id;
	SEN_STATUS_PARAM sensor_status;
	ER rt = E_OK;

	base_id = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_BASE_ID);
	sensor_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(base_id), SEN_GET_STATUS, (void *)&sensor_status);
	if (rt != E_OK || sensor_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), sensor_status.sensor_mode);
	}
	*ActMode = IPL_UTI_CONV2_SEN_ACT_MODE(sensor_status.sensor_info.sig_mode);
}

void IPL_CtrlInitIpc_SIESignal_Cap(UINT32 id, SIE_UPDATE *Update, IPP_SIE_SIGNAL *Signal)
{
	Signal->Sie_HDVD.HD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_INV);
	Signal->Sie_HDVD.VD_ini = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_INV);

	Signal->SensorPhase.VD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_VD_PHASE);
	Signal->SensorPhase.HD = IPL_CtrlGetInfor(id, IPLCTRL_SIE_HD_PHASE);
	Signal->SensorPhase.Data = IPL_CtrlGetInfor(id, IPLCTRL_SIE_DATA_PHASE);

	*Update |= (SIE_INIT);
}

void IPL_CtrlInitIpc_SIECh0PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out, SIE_PACKBUS *Bit)
{
	*Bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAPBIT);
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS);
	Out->PPB_BufNum = 0;
	Out->PPB_FrmOfs = 0;
	*Update |= SIE_OUTINFO_CH0;
}

void IPL_CtrlInitIpc_SIECh1PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out)
{
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	Out->PPB_BufNum = 0;
	Out->PPB_FrmOfs = 0;
	*Update |= SIE_OUTINFO_CH1;
}

void IPL_CtrlInitIpc_SIECh2PPB_Cap(UINT32 id, SIE_UPDATE *Update, SIE_Dma_Out *Out)
{
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	Out->PPB_BufNum = 0;
	Out->PPB_FrmOfs = 0;
	*Update |= SIE_OUTINFO_CH2;
}


void IPL_CtrlInitIpc_SIECh3PPB_Cap(UINT32 id,  SIE_UPDATE *Update, SIE_Dma_Out *Out, SIE_PACKBUS *Bit)
{

	*Bit = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_CAPBIT);
	Out->LineOffset = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_HLOS);
	Out->PPB_BufNum = 0;
	Out->PPB_FrmOfs = 0;
	*Update |= SIE_OUTINFO_CH3;
}

void IPL_CtrlInitIpc_SIEIn_Cap(UINT32 id,  SIE_UPDATE *Update, SIE_WINDOW *Win, BAYER_TYPE *bayer_type, UINT32 *OutH, UINT32 *OutV)
{
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 i, frm_cnt = 0;
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if (sen_status.sensor_info.mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		for (i = IPL_ID_1 ; i < SEN_MFRAME_MAX_NUM ; i++) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT) & (1 << i)) {
				if (i == id) {
					break;
				}
				frm_cnt++;
			}
		}
	}

	Win->Act.StartPix.x = sen_status.sensor_info.mode->trans_hd[frm_cnt]->data_start;
	Win->Act.StartPix.y = sen_status.sensor_info.mode->trans_vd[frm_cnt]->data_start;
	Win->Act.SizeH = sen_status.sensor_info.mode->trans_hd[frm_cnt]->data_size;
	Win->Act.SizeV = sen_status.sensor_info.mode->trans_vd[frm_cnt]->data_size;

	Win->Act.CFA = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CAPSTPIX);

	Win->Crop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_H);
	Win->Crop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_V);
	Win->Crop.CFA = Win->Act.CFA;

	if (sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR || sen_status.sensor_info.mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
		if (sen_status.sensor_info.mode->dvi != NULL) {
			if (sen_status.sensor_info.mode->dvi->mode != SENSOR_DVI_MODE_SD) {
				Win->Act.SizeH  = Win->Act.SizeH << 1;
				Win->Act.StartPix.x = Win->Act.StartPix.x << 1;
			} else {
			}
		} else {
			DBG_ERR("dvi information NULL\r\n");
		}
	} else {
	}

	if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV)) {
		DBG_ERR("id: %d, act size (%d %d) < crop Size(%d %d)\r\n", id, Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
		Win->Crop.StartPix.x = 0;
		Win->Crop.StartPix.y = 0;
		Win->Crop.SizeH = Win->Act.SizeH;
		Win->Crop.SizeV = Win->Act.SizeV;
	} else {
		Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
		Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;
	}

	*OutH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	*OutV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);

	switch (sen_status.sensor_info.mode->stpix) {
	case SENSOR_STPIX_R:
	case SENSOR_STPIX_GR:
	case SENSOR_STPIX_GB:
	case SENSOR_STPIX_B:
		*bayer_type = BAYER_FORMAT;
		break;

	case SENSOR_STPIX_RGBIR_RIR:
	case SENSOR_STPIX_RGBIR_RG:
		*bayer_type = RGBIR_FORMAT;
		break;

	case SENSOR_STPIX_RCCB_RC:
	case SENSOR_STPIX_RCCB_CR:
	case SENSOR_STPIX_RCCB_CB:
	case SENSOR_STPIX_RCCB_BC:
		*bayer_type = RCCB_FORMAT;
		break;

	default:
		DBG_ERR("bayer type unkonw (start pix = %d)\r\n", sen_status.sensor_info.mode->stpix);
		break;
	}

	*Update |= SIE_IOSIZE;
}

void IPL_CtrlInitIpc_SIEOB_Cap(UINT32 id,  SIE_UPDATE *Update, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_CAPAUTO_OB_EN) == DISABLE) {
		OBAvg->ObSubRatio = 0x20;
	} else {
		OBAvg->StartPix.x = sen_status.sensor_info.mode->ob.top_win[0];
		OBAvg->StartPix.y = sen_status.sensor_info.mode->ob.top_win[1];
		OBAvg->SizeH = (sen_status.sensor_info.mode->ob.top_win[2] - sen_status.sensor_info.mode->ob.top_win[0]);
		OBAvg->SizeV = (sen_status.sensor_info.mode->ob.top_win[3] - sen_status.sensor_info.mode->ob.top_win[1]);
		OBSub->Offset = 0;
		OBAvg->SubRatio_X = 0;
		OBAvg->AvgActThres = 0x80;
		OBAvg->ObSubRatio = 0x20;
		*func_en |= (OBSubSelEn | OBAvgEn);
		*Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SUBFEN);
	}
}
void IPL_CtrlInitIpc_SIECCIR_Cap(UINT32 id, SIE *sie, FLIP_TYPE flip_type)
{
	SEN_STATUS_PARAM sen_status = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if ((sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR) && (sen_status.sensor_info.mode->mode_type != SENSOR_MODE_CCIR_INTERLACE)) {
		return;
	}

	switch (sen_status.sensor_info.mode->dvi->fmt) {
	case SENSOR_DVI_CCIR601:
		sie->pSubFunc->pDvi->SIE_DVIFmt = SIE_CCIR601;
		break;

	case SENSOR_DVI_CCIR656_EAV:
		sie->pSubFunc->pDvi->SIE_DVIFmt = SIE_CCIR656_EAV;
		break;

	case SENSOR_DVI_CCIR656_ACT:
		sie->pSubFunc->pDvi->SIE_DVIFmt = SIE_CCIR656_ACT;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIFmt = %d\r\n", id, sen_status.sensor_info.mode->dvi->fmt);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->mode) {
	case SENSOR_DVI_MODE_SD:
		sie->pSubFunc->pDvi->SIE_DVIMode = SIE_SDMode;
		break;

	case SENSOR_DVI_MODE_HD:
		sie->pSubFunc->pDvi->SIE_DVIMode = SIE_HDMode;
		break;

	case SENSOR_DVI_MODE_HD_INV:
		sie->pSubFunc->pDvi->SIE_DVIMode = SIE_HDMode_Inv;
		break;

	default:
		DBG_ERR("id: %d, unsupport SIE_DVIMode = %d\r\n", id, sen_status.sensor_info.mode->dvi->mode);
		break;
	}

	switch (sen_status.sensor_info.mode->dvi->data_fmt) {
	case SENSOR_DVI_YUYV:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
		break;

	case SENSOR_DVI_YVYU:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
		break;

	case SENSOR_DVI_UYVY:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
		break;

	case SENSOR_DVI_VYUY:
		sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
		break;

	default:
		DBG_ERR("id: %d, unsupport DVIOUT_SWAPSEL = %d\r\n", id, sen_status.sensor_info.mode->dvi->data_fmt);
		break;
	}

	sie->pSubFunc->pDvi->FldEn = sen_status.sensor_info.mode->dvi->fld_en;
	sie->pSubFunc->pDvi->FldSelEn = sen_status.sensor_info.mode->dvi->fld_inv_en;
	sie->pSubFunc->pDvi->OutSplit = sen_status.sensor_info.mode->dvi->out_split_en;
	sie->pSubFunc->pDvi->CCIR656VdSel = sen_status.sensor_info.mode->dvi->ccir656_info.vd_mode;
	sie->pSubFunc->pDvi->DataPeriod = sen_status.sensor_info.mode->dvi->ahd_info.combine_num;
	sie->pSubFunc->pDvi->DataIdx = sen_status.sensor_info.mode->dvi->ahd_info.select_id;
	sie->pSubFunc->pDvi->AutoAlign = sen_status.sensor_info.mode->dvi->ahd_info.auto_align;

	//crop, adjust dviout pattern if crop start is not 4x
	if ((sie->SensorIn_Win.Crop.StartPix.x & 0x3) != 0) {
		switch (sie->pSubFunc->pDvi->DVIOUT_SWAPSEL) {
		case DVIOUT_SWAPVYUY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
			break;

		case DVIOUT_SWAPUYVY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
			break;

		case DVIOUT_SWAPYUYV:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
			break;

		case DVIOUT_SWAPYVYU:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
			break;

		default:
			DBG_ERR("unknown swapsel %d\r\n", sie->pSubFunc->pDvi->DVIOUT_SWAPSEL);
		}
	}

	//flip
	if (flip_type & FLIP_H) {
		sie->CH0.Out.HFlip = TRUE;
		sie->CH1.Out.HFlip = TRUE;

		switch (sie->pSubFunc->pDvi->DVIOUT_SWAPSEL) {
		case DVIOUT_SWAPVYUY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
			break;

		case DVIOUT_SWAPUYVY:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
			break;

		case DVIOUT_SWAPYUYV:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
			break;

		case DVIOUT_SWAPYVYU:
			sie->pSubFunc->pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
			break;

		default:
			DBG_ERR("unknown swapsel %d\r\n", sie->pSubFunc->pDvi->DVIOUT_SWAPSEL);
		}
	}

	if (flip_type & FLIP_V) {
		sie->CH0.Out.VFlip = TRUE;
		sie->CH1.Out.VFlip = TRUE;
	}



}
void IPL_CtrlInitIpc_SIECH3Info_Cap(UINT32 id, SIE *psie, SIE_UPDATE *Update)
{
	SR_WDR_SUBIMG_INFO wdr_sub_info = {0};
	SHDR_SUBIMG_INFO shdr_sub_info = {0};
	if (psie->CH3.Src == CH3_YOUT_ACC) {
		if (IPL_CtrlGetInfor(id, IPLCTRL_WDR_CAP_EN) == ENABLE) {
			SceneRender_WDR_GetSubImgInfo(id, &wdr_sub_info);
			if (wdr_sub_info.uiSubRatio == 1) {
				psie->pSubFunc->yout_info.win_num_x = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), wdr_sub_info.uiSubWidth, 1);
				psie->pSubFunc->yout_info.win_num_y = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), wdr_sub_info.uiSubHeight, 1);
			} else {
				psie->pSubFunc->yout_info.win_num_x = wdr_sub_info.uiSubWidth;
				psie->pSubFunc->yout_info.win_num_y = wdr_sub_info.uiSubHeight;
			}
		} else if (IPL_CtrlGetInfor(id, IPLCTRL_SHDR_CAP_EN) == ENABLE) {
			shdr_sub_info = SensorHDR_GetSubImgInfo();
			if (shdr_sub_info.uiSubRatio == 1) {
				psie->pSubFunc->yout_info.win_num_x = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), shdr_sub_info.uiSubWidth, 1);
				psie->pSubFunc->yout_info.win_num_y = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), IPL_UTI_CONV2_UINT32(1, 1), shdr_sub_info.uiSubHeight, 1);
			} else {
				psie->pSubFunc->yout_info.win_num_x = shdr_sub_info.uiSubWidth;
				psie->pSubFunc->yout_info.win_num_y = shdr_sub_info.uiSubHeight;
			}
		} else {
			DBG_WRN("shdr or wdr sould be opened when CH3_YOUT_ACC\r\n");
			return;
		}
		psie->pSubFunc->yout_info.crp_size_x = psie->SensorIn_Win.Crop.SizeH;
		psie->pSubFunc->yout_info.crp_size_y = psie->SensorIn_Win.Crop.SizeV;
		psie->pSubFunc->sieFuncEn |= SHDRYOutEn;
		*Update |= SIE_CH3_YOUT_SIZE ;
	}
}
#if 0   //Jarkko tmp
void IPL_CtrlInitIpc_SIEGamma_Cap(UINT32 id,  SIE_UPDATE *Update, SieFuncEn *func_en, SIE_GAMMA **pGamma)
{
	if ((id != IPL_ID_1) && (id != IPL_ID_2)) {
		DBG_ERR("IPL ID Error %d : SIE Gamma only support IPL_ID_1 and IPL_ID_2\r\n", id);
		return;
	}

	*Update |= SIE_GAMMA_;
	*func_en |= GammaEn;

	*pGamma = &SieRawEncTab;
}
#endif

void IPL_CtrlInitIpc_SIERawEnc_Cap(UINT32 id,  SIE_UPDATE *Update, SieFuncEn *func_en, SIE_RAWENC *RawEnc)
{

	if (id > IPL_ID_4) {
		DBG_ERR("IPL ID Error %d : SIE RawEnc only support IPL_ID_1 TO IPL_ID_4\r\n", id);
		*func_en &= ~RawEncEn;
		return;
	} else {
		*func_en |= RawEncEn ;
	}

}

void IPL_CtrlInitIpc_SIECASet_Cap(UINT32 id, SieFuncEn *func_en, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *pPath, UINT32 CAEnable)
{
	THRESHOL_SET CA_ThrInit = {0, 4095};

	if (CAEnable == ENABLE) {
		cacc->CACrop.StartPix.x = 0;
		cacc->CACrop.StartPix.y = 0;
		cacc->CACrop.SizeH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
		cacc->CACrop.SizeV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);

		cacc->CA_WIN.WinNum_X = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_WINXNUM);
		cacc->CA_WIN.WinNum_Y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_WINYNUM);

		//ca threshold init
		cacc->CA_TH_INFO.Gth = CA_ThrInit;
		cacc->CA_TH_INFO.Rth = CA_ThrInit;
		cacc->CA_TH_INFO.Bth = CA_ThrInit;
		cacc->CA_TH_INFO.Pth = CA_ThrInit;
		pPath->bVig = DISABLE;
		pPath->bCaThreshold = ENABLE;

		*func_en |= CAEn;
		*Update |= (SIE_STATS_CA_SIZE | SIE_STATS_CA_);
	} else {
		*func_en &= ~CAEn;
	}
	*Update |= SIE_SUBFEN;
}

void IPL_CtrlInitCapIpcInfo(IPL_GET_CAP_RAW_DATA *cap_info, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	//IPL_HAL_READ readBit = {0};
	SEN_STATUS_PARAM sen_status = {0};
	//SIE_UPDATE tmpUpdate = 0;
	//SIE_WINDOW tmpWin = {0};
	IPC_QINFO QInfo = {0};
	ER rt = E_OK;


	sen_status.sensor_mode = IPL_CtrlGetInfor(cap_info->id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(cap_info->id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(cap_info->id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(cap_info->id, IPLCTRL_SEN_MODE));
		return;
	}

	if (IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(cap_info->id, IPLCTRL_CAPFLOW)) {
#if 0
		//read sie original settings
		readBit.sieRead = SIE_R_SUBFEN;
		SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)&Info->psie->pSubFunc->siefunc_en);
		//readBit.sieRead = SIE_R_IO;
		//SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)Info->psie);
		//readBit.sieRead = SIE_R_Out;
		//SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)Info->psie);

		readBit.sieRead = SIE_R_INSIZE;
		SIEx_ENG_Read(cap_info->Id, IPL_CtrlGetHalSetId(cap_info->Id), readBit, (void *)&Info->psie->IMG_Window);

		//read pre dram in information(special case)
		QInfo = IPL_CTRLQuery(cap_info->Id, ICF_IPLIO_RDY);

		/**************** save last sie information ****************/
		ExtInfo->Param[0] = Info->psie->CH0.Out.PPB0_Addr;
		ExtInfo->Param[1] = Info->psie->CH0.Out.PPB1_Addr;
		ExtInfo->Param[2] = Info->psie->CH0.Out.LineOffset;
		ExtInfo->Param[3] = IPL_CtrlGetInfor(cap_info->Id, IPLCTRL_PRVBURSTLENGTH);
		ExtInfo->Param[4] = Info->psie->IMG_Window.Crop.StartPix.x;
		ExtInfo->Param[5] = Info->psie->IMG_Window.Crop.StartPix.y;
		ExtInfo->Param[6] = Info->psie->IMG_Window.Crop.SizeH;
		ExtInfo->Param[7] = Info->psie->IMG_Window.Crop.SizeV;
		//ExtInfo->Param[8] = QInfo.PreDmaIn_Rdy.InDma.PixelAddr;
		//ExtInfo->Param[9] = QInfo.PreDmaIn_Rdy.PPB1_Addr;
		/**************** load new sie information ****************/
		IPL_CtrlInitIpc_SIEBurstLength_Cap(cap_info->Id, &Info->sieUpdate, &SenInfo, &Info->psie->BurstLength);
		IPL_CtrlInitIpc_SIEIn_Cap(cap_info->Id, &tmpUpdate, &tmpWin);
		Info->psie->IMG_Window.Crop.StartPix.x = tmpWin.Crop.StartPix.x;
		Info->psie->IMG_Window.Crop.StartPix.y = tmpWin.Crop.StartPix.y;
		Info->psie->IMG_Window.Crop.SizeH = tmpWin.Crop.SizeH;
		Info->psie->IMG_Window.Crop.SizeV = tmpWin.Crop.SizeV;

		IPL_CtrlInitIpc_SIECh0PPB_Cap(cap_info->Id, &Info->sieUpdate, &Info->psie->CH0.Out, &Ch0HSize, &Ch0VSize);
		Info->psie->CH0.Src = NonScaled_Img;
		Info->sieUpdate |= SIE_CH0Out_SRC;
#endif
	} else if (IPLCTRL_CAP_FLOW_SIM == IPL_CtrlGetInfor(cap_info->id, IPLCTRL_CAPFLOW)) {
		if (cap_info->out_ch0_en == ENABLE) {
			Info->psie->CH0.Out.PPB_Addr = cap_info->buf_addr_ch0;
			Info->psie->pSubFunc->sieFuncEn |= DMAOutCh0En;
			Info->sieUpdate |= SIE_OUTADDR_CH0;
		}

		if (cap_info->out_ch3_yout_en == ENABLE) {
			Info->psie->CH3.Out.PPB_Addr = cap_info->buf_addr_ch3;
			Info->psie->pSubFunc->sieFuncEn |= DMAOutCh3En;
			Info->sieUpdate |= (SIE_OUTADDR_CH3 | SIE_OUTINFO_CH3);
		}
	} else {
		IPL_CtrlInitIpc_SIEBurstLength_Cap(cap_info->id, &Info->sieUpdate, &sen_status.sensor_info, &Info->psie->BurstLength);

		if (cap_info->res[0] == 0x123) { // capture CCIR image
			IPL_CtrlInitIpc_SIESignal_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->Sie_Signal);
			IPL_CtrlInitIpc_SIEAct_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->ActMode);
			IPL_CtrlInitIpc_SIECh0PPB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->CH0.Out, &Info->psie->CH0.Bit);
			Info->psie->CH0.Src = CH0_NONSCALED_IMG;
			IPL_CtrlInitIpc_SIECh1PPB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->CH1.Out);
			IPL_CtrlInitIpc_SIEIn_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->SensorIn_Win, &Info->psie->Bayer_Type, &Info->psie->BSOutImg.SizeH, &Info->psie->BSOutImg.SizeV);
			IPL_CtrlInitIpc_SIECCIR_Cap(cap_info->id, Info->psie, cap_info->flip_info);
			Info->psie->CH0.Out.PPB_Addr = cap_info->buf_addr_ch0;

			//if y-only,set ub buffer value as 0x80
			if(sen_status.sensor_info.mode->dvi->out_split_en == FALSE) {
				memset((void*)cap_info->buf_addr_ch0_1, 0x80, IPL_CtrlGetInfor(cap_info->id,IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V) *IPL_CtrlGetInfor(cap_info->id,IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H));
			}

			Info->psie->CH1.Out.PPB_Addr = cap_info->buf_addr_ch0_1;
			Info->psie->pSubFunc->sieFuncEn |= DviEn;
			Info->sieUpdate |= (SIE_OUTADDR_CH0 | SIE_OUTADDR_CH1 | SIE_DVI_);

		} else { // capture raw image
			if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(cap_info->id, IPLCTRL_CAPFLOW)) {
				ExtInfo->param[0] = cap_info->res[0];
				IPL_CtrlInitIpc_SIERawEnc_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn,
											  &Info->psie->pSubFunc->RawEnc);
				// for frame rate ctrl
				if (cap_info->frm_ctrl != 0) {
					ExtInfo->frm_ctrl = cap_info->frm_ctrl;
				}
			} else {
			}
			IPL_SIESetDP_FCB(cap_info->id, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->InteEn, &Info->sieUpdate, Info->psie->pSubFunc);
			IPL_CtrlInitIpc_SIESignal_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->Sie_Signal);
			IPL_CtrlInitIpc_SIEAct_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->ActMode);
			IPL_CtrlInitIpc_SIECh0PPB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->CH0.Out, &Info->psie->CH0.Bit);
			IPL_CtrlInitIpc_SIECh3PPB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->CH3.Out, &Info->psie->CH3.Bit);
			IPL_CtrlInitIpc_SIEIn_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->SensorIn_Win, &Info->psie->Bayer_Type, &Info->psie->BSOutImg.SizeH, &Info->psie->BSOutImg.SizeV);
			IPL_CtrlInitIpc_SIECASet_Cap(cap_info->id,&Info->psie->pSubFunc->sieFuncEn,&Info->psie->pSubFunc->InteEn,&Info->sieUpdate,&Info->psie->pSubFunc->Ca,&Info->psie->pSubFunc->pStatPathInfo,cap_info->out_ca_en);
			Info->psie->CH0.Src = CH0_NONSCALED_IMG;

			if ((Info->psie->SensorIn_Win.Crop.SizeH != Info->psie->BSOutImg.SizeH) || (Info->psie->SensorIn_Win.Crop.SizeV != Info->psie->BSOutImg.SizeV)) {
				DBG_ERR("Id: %d, ch0 output size (%d %d) < crop Size(%d %d)\r\n", cap_info->id, Info->psie->BSOutImg.SizeH, Info->psie->BSOutImg.SizeV, Info->psie->SensorIn_Win.Crop.SizeH, Info->psie->SensorIn_Win.Crop.SizeV);
			}
			IPL_CtrlInitIpc_SIEOB_Cap(cap_info->id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->InteEn, &Info->psie->pSubFunc->OBAvg, &Info->psie->pSubFunc->OBSub);
		}
		Info->sieUpdate |= SIE_SUBFEN;
		QInfo = IPL_Ctrl_Query(cap_info->id, ICF_SIE_DFT_INTE_EN);
		Info->psie->pSubFunc->InteEn |= QInfo.sie_dft_inte_en;

		//ch0
		if (cap_info->out_ch0_en == ENABLE) {
			Info->psie->CH0.Out.PPB_Addr = cap_info->buf_addr_ch0;
			Info->psie->pSubFunc->sieFuncEn |= DMAOutCh0En;
			Info->sieUpdate |= SIE_OUTADDR_CH0;
			if(cap_info->out_ca_en == ENABLE) {
				Info->psie->CH1.Out.PPB_Addr = cap_info->buf_addr_ca;
				Info->psie->CH1.Out.PPB_BufNum = 0;
				Info->psie->CH1.Out.PPB_FrmOfs = 0;
				Info->psie->CH1.Out.LineOffset = 0;
				Info->psie->CH1.Out.HFlip = 0;
				Info->psie->CH1.Out.VFlip = 0;
				Info->sieUpdate |= (SIE_OUTADDR_CH1 |SIE_OUTINFO_CH1);
			}

			//ch5
			if (cap_info->out_ch5_en == ENABLE) {
				Info->psie->CH5.Out.PPB_Addr = cap_info->buf_addr_ch5;
				Info->psie->CH5.Out.LineOffset = 0;
				Info->psie->CH5.Out.PPB_BufNum = 0;
				Info->psie->CH5.Out.PPB_FrmOfs = 0;
				Info->sieUpdate |= (SIE_OUTADDR_CH5 | SIE_OUTINFO_CH5);
			}
		}
		//ch3
		if (cap_info->out_ch3_yout_en == ENABLE) {
			Info->psie->CH3.Src = CH3_YOUT_ACC ;
			IPL_CtrlInitIpc_SIECH3Info_Cap(cap_info->id, Info->psie, &Info->sieUpdate);
			Info->psie->CH3.Out.PPB_Addr = cap_info->buf_addr_ch3;
			Info->psie->CH3.Out.LineOffset = IPL_ALIGN_ROUNDUP((Info->psie->pSubFunc->yout_info.win_num_x * 12 / 8),4);

			Info->psie->pSubFunc->sieFuncEn |= DMAOutCh3En;
			Info->sieUpdate |= (SIE_OUTADDR_CH3 | SIE_OUTINFO_CH3 | SIE_SUBFEN);
			if (cap_info->praw_info != NULL) {
				cap_info->praw_info->yout_info.yout_x_winnum = Info->psie->pSubFunc->yout_info.win_num_x;
				cap_info->praw_info->yout_info.yout_y_winnum = Info->psie->pSubFunc->yout_info.win_num_y;
			}

		}
	}

}

void IPL_CtrlInitPrvIpcInfo(UINT32 id, IPC_INFO *ipc_info, IPL_ISR_OBJ_INFO *isr_info, IPL_MODE_DATA *chg_mode)
{
	UINT32 i, idx = IPL_ISR_SIE1;


	switch (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)) {
	case ICF_FLOW_C:
		IPL_CtrlInitIpcInfoFlowCommon(id, ipc_info);
		break;

	case ICF_FLOW_CCIR:
		IPL_CtrlInitIpcInfoFlowCCIR(id, ipc_info);
		break;

	case ICF_FLOW_VIRTUAL:
		IPL_CtrlInitIpcInfoFlowVirtual(id, ipc_info);
		break;

	default:
		DBG_ERR("id: %d, unsupport IPL flow %d\r\n", id, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
		break;
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
		isr_info->RAWHeader_CB = IPL_RAWHeaderCB;
		isr_info->raw_postproc_cb = chg_mode->raw_postproc_cb_fp;
		isr_info->pm_cb = chg_mode->pm_cb_fp;
		switch (id) {
		case IPL_ID_1:
			idx = IPL_ISR_SIE1;
			break;

		case IPL_ID_2:
			idx = IPL_ISR_SIE2;
			break;

		case IPL_ID_3:
			idx = IPL_ISR_SIE3;
			break;

		case IPL_ID_4:
			idx = IPL_ISR_SIE4;
			break;

		case IPL_ID_5:
			idx = IPL_ISR_SIE5;
			break;

		case IPL_ID_6:
			idx = IPL_ISR_SIE6;
			break;

		case IPL_ID_7:
			idx = IPL_ISR_SIE7;
			break;

		case IPL_ID_8:
			idx = IPL_ISR_SIE8;
			break;

		default:
			idx = IPL_ISR_SIE1;
			break;
		}
		isr_info->EventTab.FP[idx] = IPL_IsrSIECB;
	}

	ipl_out_buf_cfg_cb[id] = chg_mode->out_buf_cfg_cb_fp;
	isr_info->datastamp_cb = chg_mode->datastamp_cb_fp;

	if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_CCIR) {
		isr_info->EventTab.FP[IPL_ISR_RHE] = IPL_IsrRHECB;
		isr_info->EventTab.FP[IPL_ISR_IFE] = IPL_IsrIFECB;
		isr_info->EventTab.FP[IPL_ISR_IPE] = IPL_IsrIPECB;
		isr_info->EventTab.FP[IPL_ISR_IFE2] = NULL;
		isr_info->EventTab.FP[IPL_ISR_DCE] = IPL_IsrDCECB;
	}
	isr_info->EventTab.FP[IPL_ISR_IME] = IPL_IsrIMECB;

	for (i = 0; i < IPL_ISR_MAX_CNT; i ++) {
		isr_info->CB_FP.FP[i] = chg_mode->isr_cb_fp[i];
	}
}

#if 0
#endif

INT32 IPL_Ctrl_cal_input_offset(UINT32 src_size, UINT32 dest_size, INT32 start_ofs, UINT32 *start_pos)
{
	INT32 start_ofs_2 = 0;

	if ((UINT32)start_ofs == IPL_CTRL_NO_USE || start_ofs == 0) {
		return start_ofs_2;
	}

	if (start_ofs > 0) {
		if (*start_pos + start_ofs + dest_size > src_size) {	//dest window > source window
			start_ofs_2 = start_ofs - (src_size - dest_size - *start_pos);
			*start_pos = src_size - dest_size;
		} else {
			start_ofs_2 = 0;
			*start_pos = *start_pos + start_ofs;
		}
	} else {
		if ((UINT32)(-start_ofs) > *start_pos) {	//start position < 0
			start_ofs_2 = start_ofs + *start_pos;
			*start_pos = 0;
		} else {
			start_ofs_2 = 0;
			*start_pos = start_ofs + *start_pos;
		}
	}

	return start_ofs_2;
}

UINT32 IPL_CtrlGetSIEVAbuf_info(void)
{
	return (16 * 16 * 2 * 4);
}

void IPL_Getime_path_Info(UINT32 id, IPL_IME_PATH Pathid, IPL_IME_PATH_INFO *Info)
{
	if (Info == NULL) {
		DBG_ERR("id: %d, input fail\r\n", id);
		return;
	}

	switch (Pathid) {
	case IPL_IME_PATH1:
		Info->output_en = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_EN);
		Info->out_ycc_compress_en = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_ENC_EN);
		Info->out_ycc_enc_fmt = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_ENC_FMT);
		Info->sample_rate = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_SAMPLE_RATE);
		Info->scale_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_H);
		Info->scale_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_V);
		Info->out_img_lofs = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_HLOS);
		Info->img_fmt = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_IMG_FMT);

		Info->out_crop_start_x = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_CROP_STR_X);
		Info->out_crop_start_y = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_CROP_STR_Y);
		Info->out_crop_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_CROP_H);
		Info->out_crop_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_CROP_V);
		break;

	case IPL_IME_PATH2:
		Info->output_en = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_EN);
		Info->sample_rate = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_SAMPLE_RATE);
		Info->scale_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_OUT_SIZE_H);
		Info->scale_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_OUT_SIZE_V);
		Info->out_img_lofs = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_OUT_SIZE_HLOS);
		Info->img_fmt = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_IMG_FMT);

		Info->out_crop_start_x = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_OUT_CROP_STR_X);
		Info->out_crop_start_y = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_OUT_CROP_STR_Y);
		Info->out_crop_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_OUT_CROP_H);
		Info->out_crop_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P2_OUT_CROP_V);
		break;

	case IPL_IME_PATH3:
		Info->output_en = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_EN);
		Info->sample_rate = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_SAMPLE_RATE);
		Info->scale_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_OUT_SIZE_H);
		Info->scale_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_OUT_SIZE_V);
		Info->out_img_lofs = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_OUT_SIZE_HLOS);
		Info->img_fmt = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_IMG_FMT);

		Info->out_crop_start_x = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_OUT_CROP_STR_X);
		Info->out_crop_start_y = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_OUT_CROP_STR_Y);
		Info->out_crop_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_OUT_CROP_H);
		Info->out_crop_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P3_OUT_CROP_V);
		break;

	case IPL_IME_PATH4:
		Info->output_en = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EN);
		Info->sample_rate = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_SAMPLE_RATE);
		Info->scale_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_OUT_SIZE_H);
		Info->scale_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_OUT_SIZE_V);
		Info->out_img_lofs = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_OUT_SIZE_HLOS);
		Info->img_fmt = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_IMG_FMT);

		Info->out_crop_start_x = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_OUT_CROP_STR_X);
		Info->out_crop_start_y = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_OUT_CROP_STR_Y);
		Info->out_crop_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_OUT_CROP_H);
		Info->out_crop_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_OUT_CROP_V);
		break;

	case IPL_IME_PATH5:
		Info->output_en = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_EN);
		Info->sample_rate = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_SAMPLE_RATE);
		Info->scale_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_OUT_SIZE_H);
		Info->scale_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_OUT_SIZE_V);
		Info->out_img_lofs = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_OUT_SIZE_HLOS);
		Info->img_fmt = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_IMG_FMT);

		Info->out_crop_start_x = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_OUT_CROP_STR_X);
		Info->out_crop_start_y = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_OUT_CROP_STR_Y);
		Info->out_crop_size_h = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_OUT_CROP_H);
		Info->out_crop_size_v = IPL_CtrlGetInfor(id, IPLCTRL_IME_P5_OUT_CROP_V);
		break;

	default:
		DBG_ERR("id: %d, path id fail %d\r\n", id, Pathid);
		break;
	}
}

void IPL_Setime_path_Info(UINT32 id, IPL_IME_PATH Pathid, IPL_IME_PATH_INFO *Info)
{
	if (Info == NULL) {
		DBG_ERR("id: %d, input fail\r\n", id);
		return;
	}

	//output size check
	if ((Info->output_en == ENABLE) && (Info->scale_size_h == 0 || Info->scale_size_v == 0 || Info->out_crop_size_h == 0 || Info->out_crop_size_v == 0)) {
		DBG_ERR("IME Output Size Error, id: %d, IPL_IME_PATH: %d, scale_size (%d, %d), crop_size (%d, %d)\r\n", id, Pathid,
			Info->scale_size_h,Info->scale_size_v, Info->out_crop_size_h, Info->out_crop_size_v);
		DBG_ERR("DISABLE IME Output, id: %d, IPL_IME_PATH: %d, en %d\r\n", id, Pathid, Info->output_en);
		Info->output_en = DISABLE;
	}

	switch (Pathid) {
	case IPL_IME_PATH1:
		if (Info->output_en == ENABLE) {
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_SAMPLE_RATE, Info->sample_rate);
			//Jarkko tmp, wait image unit ready
			if (IPL_CtrlGetInfor(id, IPLCTRL_FUNC_EN) & IPL_FUNC_VR360) {
                if (Info->out_ycc_compress_en)
                    DBG_WRN("Id: %d, Not Support Enable both VR360 and IME Path1 encoding! Force disable IME Path1 encoding\r\n", id);

                IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_ENC_EN, DISABLE);
    		} else {
			    IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_ENC_EN, Info->out_ycc_compress_en);
    		}
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_ENC_FMT, Info->out_ycc_enc_fmt);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_H, Info->scale_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_V, Info->scale_size_v);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, Info->out_img_lofs);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_IMG_FMT, Info->img_fmt);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_OUT_CROP_STR_X, Info->out_crop_start_x);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_OUT_CROP_STR_Y, Info->out_crop_start_y);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_OUT_CROP_H, Info->out_crop_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_OUT_CROP_V, Info->out_crop_size_v);
		}
		IPL_CtrlSetInfor(id, IPLCTRL_IME_P1_EN, Info->output_en);
		break;

	case IPL_IME_PATH2:
		if (Info->output_en == ENABLE) {
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_SAMPLE_RATE, Info->sample_rate);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_OUT_SIZE_H, Info->scale_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_OUT_SIZE_V, Info->scale_size_v);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, Info->out_img_lofs);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_IMG_FMT, Info->img_fmt);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_OUT_CROP_STR_X, Info->out_crop_start_x);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_OUT_CROP_STR_Y, Info->out_crop_start_y);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_OUT_CROP_H, Info->out_crop_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_OUT_CROP_V, Info->out_crop_size_v);
		}
		IPL_CtrlSetInfor(id, IPLCTRL_IME_P2_EN, Info->output_en);
		break;

	case IPL_IME_PATH3:
		if (Info->output_en == ENABLE) {
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_SAMPLE_RATE, Info->sample_rate);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_OUT_SIZE_H, Info->scale_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_OUT_SIZE_V, Info->scale_size_v);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, Info->out_img_lofs);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_IMG_FMT, Info->img_fmt);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_OUT_CROP_STR_X, Info->out_crop_start_x);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_OUT_CROP_STR_Y, Info->out_crop_start_y);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_OUT_CROP_H, Info->out_crop_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_OUT_CROP_V, Info->out_crop_size_v);
		}
		IPL_CtrlSetInfor(id, IPLCTRL_IME_P3_EN, Info->output_en);
		break;

	case IPL_IME_PATH4:
		if (Info->output_en == ENABLE) {
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_SAMPLE_RATE, Info->sample_rate);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_OUT_SIZE_H, Info->scale_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_OUT_SIZE_V, Info->scale_size_v);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_OUT_SIZE_HLOS, Info->out_img_lofs);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_IMG_FMT, Info->img_fmt);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_OUT_CROP_STR_X, Info->out_crop_start_x);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_OUT_CROP_STR_Y, Info->out_crop_start_y);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_OUT_CROP_H, Info->out_crop_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_OUT_CROP_V, Info->out_crop_size_v);
		}
		IPL_CtrlSetInfor(id, IPLCTRL_IME_P4_EN, Info->output_en);
		break;

	case IPL_IME_PATH5:
		if (Info->output_en == ENABLE) {
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_SAMPLE_RATE, Info->sample_rate);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_OUT_SIZE_H, Info->scale_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_OUT_SIZE_V, Info->scale_size_v);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_OUT_SIZE_HLOS, Info->out_img_lofs);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_IMG_FMT, Info->img_fmt);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_OUT_CROP_STR_X, Info->out_crop_start_x);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_OUT_CROP_STR_Y, Info->out_crop_start_y);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_OUT_CROP_H, Info->out_crop_size_h);
			IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_OUT_CROP_V, Info->out_crop_size_v);
		}
		IPL_CtrlSetInfor(id, IPLCTRL_IME_P5_EN, Info->output_en);
		break;

	default:
		DBG_ERR("id: %d, path id fail %d\r\n", id, Pathid);
		break;
	}
}

void IPL_CtrlPrvSetDZoomInfo(UINT32 id)
{
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 width = 0, height = 0, ipl_in_width = 0, ipl_in_height = 0;
	UINT32 dz_idx;
	SR_WDR_SUBIMG_INFO wdr_sub_info = {0};
	SHDR_SUBIMG_INFO shdr_sub_info = {0};
	ER rt = E_OK;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	dz_idx = IPL_DzoomGetIndex(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID));
	//SIE crop
	IPL_DzoomGetSIECropSize(id, dz_idx, FALSE, &width, &height);
	width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, width, 4);
	height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, height, 4);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V, height);

	//SIE ch0
	IPL_DzoomGetSIEOutSize(id, dz_idx, &width, &height);
	width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, width, 4);
	height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, height, 4);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V, height);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_PRVBIT, IPL_AlgGetUIInfo(id, IPL_SEL_RAW_BITDEPTH_PRV));

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_EN) == ENABLE) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_ALIGN_ROUNDUP(IPL_UTI_RAWH_CONV2_LOFS(width, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT))*IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_BUF_RATIO) / 100, 4));
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(width, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT)));
	}

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_EN) == ENABLE && IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_SRC) == CH3_YOUT_ACC) {
		if (IPL_CtrlGetInfor(id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) {
			SceneRender_WDR_GetSubImgInfo(IPL_UTI_CONV2_WDR_ID(id), &wdr_sub_info);
			if (wdr_sub_info.uiSubRatio == 1) {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), wdr_sub_info.uiSubWidth, 1)); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), wdr_sub_info.uiSubHeight, 1)); //maximum y output window number 128
			} else {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, wdr_sub_info.uiSubWidth); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, wdr_sub_info.uiSubHeight); //maximum y output window number 128
			}
		} else {
			shdr_sub_info = SensorHDR_GetSubImgInfo();
			if (shdr_sub_info.uiSubRatio == 1) {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), shdr_sub_info.uiSubWidth, 1)); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), shdr_sub_info.uiSubHeight, 1)); //maximum y output window number 128
			} else {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, shdr_sub_info.uiSubWidth); //maximum y output window number 128
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, shdr_sub_info.uiSubHeight); //maximum y output window number 128
			}
		}
	}

	//Set IPL IN for RHE IN
	IPL_DzoomGetRaw2YUVInSize(id, dz_idx, &width, &height);
	width  = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, width, 4);
	height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_PRV_CROP_RATIO), sen_status.sensor_info.mode->ratio.ratio_h_v, height, 4);
	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_BIT, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_PRVBIT));

	if (id <= RSC_SURPPORT_NUM && rsc_ext_getenable(id) == SEL_RSC_ON){
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_RSC_CROP_RATIO, rsc_ext_geteisratio(id,SEL_EIS_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE)));
		if (IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_SQUARE_EN) == ENABLE) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_SQUARE_PAD) {
				ipl_in_width = IPL_UTI_EISDIS_ADJ_HSIZE(width - IPL_SQUARE_PATCH_W_PADDING, rsc_ext_geteisratio(id,SEL_EIS_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE)), 16) + IPL_SQUARE_PATCH_W_PADDING;
		        ipl_in_height = IPL_UTI_EISDIS_ADJ_VSIZE(height - IPL_SQUARE_PATCH_H_PADDING,rsc_ext_geteisratio(id,SEL_EIS_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE)), 4) + IPL_SQUARE_PATCH_H_PADDING;
			} else {
				ipl_in_width = IPL_UTI_EISDIS_ADJ_HSIZE(width, rsc_ext_geteisratio(id,SEL_EIS_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE)), 16);
		        ipl_in_height = IPL_UTI_EISDIS_ADJ_VSIZE(height,rsc_ext_geteisratio(id,SEL_EIS_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE)), 4);
			}
			if (ipl_in_width != IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_H)) {
				ipl_in_width = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_H);
			}
			if (ipl_in_height != IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_V)) {
				ipl_in_height = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_V);
			}
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_H, ipl_in_width);
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_V, ipl_in_height);
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_H, IPL_UTI_EISDIS_ADJ_HSIZE(width, rsc_ext_geteisratio(id,SEL_EIS_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE)), 4));
	        IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_V, IPL_UTI_EISDIS_ADJ_VSIZE(height,rsc_ext_geteisratio(id,SEL_EIS_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE)), 4));
		}
	} else if (IPL_AlgGetUIInfo(id, IPL_SEL_DISCOMP_IMM) == SEL_DISCOMP_ON) {
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_DIS_CROP_RATIO, IPL_CtrlGetInfor(id, IPLCTRL_DIS_RATIO));
		if (IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_SQUARE_EN) == ENABLE) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_SQUARE_PAD) {
				ipl_in_width = IPL_UTI_EISDIS_ADJ_HSIZE(width - IPL_SQUARE_PATCH_W_PADDING, IPL_CtrlGetInfor(id, IPLCTRL_DIS_RATIO), 16) + IPL_SQUARE_PATCH_W_PADDING;
				ipl_in_height = IPL_UTI_EISDIS_ADJ_VSIZE(height - IPL_SQUARE_PATCH_H_PADDING,IPL_CtrlGetInfor(id, IPLCTRL_DIS_RATIO), 4) + IPL_SQUARE_PATCH_H_PADDING;
			} else {
				ipl_in_width = IPL_UTI_EISDIS_ADJ_HSIZE(width, IPL_CtrlGetInfor(id, IPLCTRL_DIS_RATIO), 16);
				ipl_in_height = IPL_UTI_EISDIS_ADJ_VSIZE(height,IPL_CtrlGetInfor(id, IPLCTRL_DIS_RATIO), 4);
			}
			if (ipl_in_width != IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_H)) {
				ipl_in_width = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_H);
			}
			if (ipl_in_height != IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_V)) {
				ipl_in_height = IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_OUT_SIZE_V);
			}
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_H, ipl_in_width);
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_V, ipl_in_height);

		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_H, IPL_UTI_EISDIS_ADJ_HSIZE(width, IPL_CtrlGetInfor(id, IPLCTRL_DIS_RATIO), 4));
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_V, IPL_UTI_EISDIS_ADJ_VSIZE(height,IPL_CtrlGetInfor(id, IPLCTRL_DIS_RATIO), 4));
		}
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_RSC_CROP_RATIO, 0);
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_RSC_CROP_RATIO, 0);
		if (IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_SQUARE_EN) == ENABLE) {
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_H, ALIGN_CEIL(width, 16));
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_H, width);
		}
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_V, height);
	}

	IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_SIZE_HLOS, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS));
}

void IPL_CtrlPrvGetDZoomInfo(UINT32 id, IPC_Dzoom_Info *Info)
{
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 act_width = 0, act_height = 0;
	UINT32 i, frm_cnt = 0;
	ER rt = E_OK;
	INT32 crop_shift_x, crop_shift_y;

	sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
	if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
		DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
		return;
	}

	if (sen_status.sensor_info.mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		for (i = IPL_ID_1 ; i < SEN_MFRAME_MAX_NUM ; i++) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT) & (1 << i)) {
				if (i == id) {
					break;
				}
				frm_cnt++;
			}
		}
		act_width = sen_status.sensor_info.mode->trans_hd[frm_cnt]->data_size;
	} else {
		act_width = sen_status.sensor_info.mode->trans_hd[0]->data_size;
	}

	if(sen_status.sensor_info.mode->dvi != NULL){
		if(sen_status.sensor_info.mode->dvi->mode != SENSOR_DVI_MODE_SD)
			act_width = act_width << 1;
	}
	act_height = sen_status.sensor_info.mode->trans_vd[0]->data_size;

	Info->Id = id;
	Info->Idx = IPL_CtrlGetHalSetId(id);
	Info->sieCropHini = (act_width - IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H)) >> 1;
	Info->sieCropVini = (act_height - IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V)) >> 1;
	Info->sieCropH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_H);
	Info->sieCropV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_PRVCROP_SIZE_V);
	Info->sieOutH = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
	Info->sieOutV = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
	Info->sieOutLineOfs = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
	Info->sie_yout_winnum_x = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINXNUM);
	Info->sie_yout_winnum_y = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_YOUT_WINYNUM);
	Info->iplCropX = ((Info->sieOutH - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H)) >> 1);
	Info->iplCropY = ((Info->sieOutV - IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V)) >> 1);
	Info->iplInH = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_H);
	Info->iplInV = IPL_CtrlGetInfor(id, IPLCTRL_IPL_IN_SIZE_V);
	Info->iplInLineOfs = IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
	Info->bChgCAParam = (BOOL)IPL_CtrlGetInfor(id, IPLCTRL_SIE_CA_EN);
	Info->bChgLAParam = (BOOL)IPL_CtrlGetInfor(id, IPLCTRL_SIE_LA_EN);
	Info->bChgEthParam = (BOOL)(IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH4_EN) && (IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH4_SRC) == CH4_ETH));
	Info->bChgYOutParam = (BOOL)IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_EN);
	Info->flip = IPL_CtrlGetInfor(id, IPLCTRL_FLIP_TYPE);

	if (IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X) != 0 || IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y) != 0) {
		crop_shift_x = IPL_Ctrl_cal_input_offset(act_width, Info->sieCropH, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), &Info->sieCropHini);
		crop_shift_y = IPL_Ctrl_cal_input_offset(act_height, Info->sieCropV, IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), &Info->sieCropVini);
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_X, Info->sieCropHini);
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_PRVCROP_START_Y, Info->sieCropVini);
		if (crop_shift_x != 0 || crop_shift_y != 0) {
			DBG_WRN("Shift Overflow, start: (%d, %d), dest_size: (%d, %d), src_size: (%d, %d)\r\n", IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_X), IPL_CtrlGetInfor(id, IPLCTRL_SIE_IN_OFS_Y), Info->sieCropH, Info->sieCropV, act_width, act_height);
		}

		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_X, Info->iplCropX);
		IPL_CtrlSetInfor(id, IPLCTRL_IPL_IN_START_Y, Info->iplCropY);
	}
}

void IPL_GetDceStrpInfo(UINT32 id, IPL_STRP_INFO *strp_info)
{
    if (IPL_CtrlGetInfor(id, IPLCTRL_FUNC_EN) & IPL_FUNC_VR360)
        *strp_info = *(iq_get_stripe_info(id, IQCB_DCE_LUT_TOP));
    else
        strp_info->strp_mode = IPL_STRPMODE_AUTO_124ST;

    if (strp_info->strp_mode >= IPL_STRPMODE_MAX) {
        DBG_ERR("Wrong IPL_ID_%d stripe mode (%u). Force set to auto mode!\r\n", id, strp_info->strp_mode);
        strp_info->strp_mode = IPL_STRPMODE_AUTO_124ST;
    }
}

#if 0
#endif
void IPL_CtrlInitCapbuf_info(UINT32 id, IPL_MODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *buf_info)
{
	buf_info->id = ChgMode->proc_sen_id;
}

void IPL_CtrlInitPrvbuf_info(IPL_MODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *buf_info)
{
	switch (IPL_CtrlGetInfor(ChgMode->proc_sen_id, IPLCTRL_PRVFLOW)) {
	case ICF_FLOW_C:
	case ICF_FLOW_CCIR:
	case ICF_FLOW_VIRTUAL:
		IPL_CtrlInitbuf_infoFlowCommon(ChgMode->proc_sen_id, buf_info);
		break;

	default:
		DBG_ERR("id: %d, unsupport IPL flow %d\r\n", ChgMode->proc_sen_id, IPL_CtrlGetInfor(ChgMode->proc_sen_id, IPLCTRL_PRVFLOW));
		break;
	}

	buf_info->id = ChgMode->proc_sen_id;
}

#if 0
#endif
void IPL_CtrlCapInit(UINT32 id, UINT32 event_id, IPLCTRL_INFO *ctrl_info)
{
	UINT32 width = 0, height = 0, BitNum = 0;
	awb_ca_info CAInfo = {0};
	BAYER_TYPE bayer_type = 0;

	switch (event_id) {
	case IPL_FLOW_NOR_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NORMAL);
		break;

	case IPL_FLOW_HDR_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_HDR);
		break;

	case IPL_FLOW_VID_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_INVID);
		break;

	case IPL_FLOW_HS_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_HS);
		break;

	case IPL_FLOW_3D_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_3D);
		break;

	case IPL_FLOW_EB_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_EBURST);
		break;

	case IPL_FLOW_AEB_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AEB);
		break;

	case IPL_FLOW_AWB_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AWB);
		break;

	case IPL_FLOW_MFHDR_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_MFHDR);
		break;

	case IPL_FLOW_MFANTISHAKE_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_MFANTISHAKE);
		break;

	case IPL_FLOW_DUALSBS_GETRAW:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_DUALSBS);
		break;

	case IPL_FLOW_RHDR_GETRAW :
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_RHDR);
		break;

	default:
		IPL_CtrlSetInfor(id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NONE);
		DBG_ERR("id: %d, unsupport event_id flow %d\r\n", id, event_id);
		break;
	}
	//set sie signal
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_VD_INV, ctrl_info->info->sig_info.vd_inv);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_HD_INV, ctrl_info->info->sig_info.hd_inv);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_VD_PHASE, ctrl_info->info->sig_info.vd_phase);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_HD_PHASE, ctrl_info->info->sig_info.hd_phase);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_DATA_PHASE, ctrl_info->info->sig_info.data_phase);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_BASE_ID, IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID));
	IPL_CtrlSetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO, ctrl_info->chg_mode->crop_ratio);
	IPL_CtrlSetInfor(id, IPLCTRL_CAPAUTO_OB_EN, DISABLE);
	IPL_CtrlSetInfor(id, IPLCTRL_CAPDF_EN, DISABLE);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CAPSTPIX, IPL_UTI_CONV_SEN_STPIX(ctrl_info->info->mode->stpix));
	IPL_CtrlSetInfor(id, IPLCTRL_IME_CAP_SUBRATIO, iq_get_ife2_lca_ratio(id, TRUE, FALSE));
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_CAP_SUBRATIO, 25);

	if (iq_check_ife2_lca(id, TRUE) == ENABLE) {
		IPL_CtrlSetInfor(id, IPLCTRL_IFE2_LCA_EN, ENABLE);
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_IFE2_LCA_EN, DISABLE);
	}

	if (iq_get_msnr_time(id) > 0) {
		IPL_CtrlSetInfor(id, IPLCTRL_MSNR_CAP_EN, ENABLE);
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_MSNR_CAP_EN, DISABLE);
	}

	if(ctrl_info->chg_mode->func_en & IPL_FUNC_WDR || ctrl_info->chg_mode->func_en & IPL_FUNC_SHDR) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_EN, ENABLE);

		if (ctrl_info->chg_mode->func_en & IPL_FUNC_WDR) {
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_SRC, CH3_YOUT_ACC);
			IPL_CtrlSetInfor(id, IPLCTRL_WDR_CAP_EN, ENABLE);
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_WDR_CAP_EN, DISABLE);
		}

		if (ctrl_info->chg_mode->func_en & IPL_FUNC_SHDR) {
			if (event_id == IPL_FLOW_RHDR_GETRAW) {
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_SRC, CH3_YOUT_ACC);
				IPL_CtrlSetInfor(id, IPLCTRL_SHDR_CAP_EN, ENABLE);
			} else {
				DBG_WRN("SHDR can not open when event = %d\r\n", event_id);
				IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_EN, DISABLE);
			}
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_SHDR_CAP_EN, DISABLE);
		}
	} else {
			IPL_CtrlSetInfor(id, IPLCTRL_SHDR_CAP_EN, DISABLE);
			IPL_CtrlSetInfor(id, IPLCTRL_WDR_CAP_EN, DISABLE);
	}

	if(ctrl_info->chg_mode->func_en & IPL_FUNC_DEFOG ) {
		IPL_CtrlSetInfor(id, IPLCTRL_DEFOG_CAP_EN, ENABLE);
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_DEFOG_CAP_EN, DISABLE);
	}

	IPL_DzoomGetSIECropSize(id, IPL_DzoomGetIndex(id), TRUE, &width, &height);

	width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), ctrl_info->info->mode->ratio.ratio_h_v, width, 8);
	height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), ctrl_info->info->mode->ratio.ratio_h_v, height, 4);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CAPCROP_SIZE_V, height);



	//when ccir split enable, sie output >> 1
	if(ctrl_info->info->mode->dvi != NULL) {
		if(ctrl_info->info->mode->dvi->out_split_en == TRUE) {
			width = width >> 1;
		} else {
			//do nothing
		}
	} else {
		//do nothing
	}

	if (event_id == IPL_FLOW_EB_GETRAW) {
		BitNum = IPL_RAW_BIT_12;
	} else if (ctrl_info->info->mode->mode_type == SENSOR_MODE_CCIR || ctrl_info->info->mode->mode_type == SENSOR_MODE_CCIR_INTERLACE) {
		BitNum = IPL_RAW_BIT_8;
	} else {
		BitNum = IPL_AlgGetUIInfo(id, IPL_SEL_RAW_BITDEPTH_CAP);
	}

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_RAW_BUFNUM, 1);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAPBIT, BitNum);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V, height);
	if (IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW) == IPLCTRL_CAP_FLOW_EBURST) {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS,  IPL_ALIGN_ROUNDUP(IPL_UTI_RAWH_CONV2_LOFS(width, BitNum)*IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH0_RAWENC_BUF_RATIO) / 100, 4));
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS,  IPL_UTI_RAWH_CONV2_LOFS(width, BitNum));
	}

	// CA information
	if(ctrl_info->info->mode->mode_type == SENSOR_MODE_LINEAR) {
		if(event_id == IPL_FLOW_NOR_GETRAW) {
			AWB_GetCAInfo(id, &CAInfo);
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_EN, ENABLE);
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_WINXNUM, CAInfo.WinNumX);
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_WINYNUM, CAInfo.WinNumY);
		} else {
			IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_EN, DISABLE);
		}
	} else {
		IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH1_CAP_CA_EN, DISABLE);
	}


	switch (ctrl_info->info->mode->stpix) {
	case SENSOR_STPIX_R:
	case SENSOR_STPIX_GR:
	case SENSOR_STPIX_GB:
	case SENSOR_STPIX_B:
		bayer_type = BAYER_FORMAT;
		break;

	case SENSOR_STPIX_RGBIR_RIR:
	case SENSOR_STPIX_RGBIR_RG:
		bayer_type = RGBIR_FORMAT;
		break;

	case SENSOR_STPIX_RCCB_RC:
	case SENSOR_STPIX_RCCB_CR:
	case SENSOR_STPIX_RCCB_CB:
	case SENSOR_STPIX_RCCB_BC:
		bayer_type = RCCB_FORMAT;
		break;

	default:
		DBG_ERR("bayer type unkonw (start pix = %d)\r\n", ctrl_info->info->mode->stpix);
		break;
	}
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH0_CAP_RAW_DATAFMT, bayer_type);

	IPL_DzoomGetSIECropSize(id, IPL_DzoomGetIndex(id), TRUE, &width, &height);
	width = IPL_UTI_SIESUB_ADJ_HSIZE(width, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_CAP_SUBRATIO), 4);
	height = IPL_UTI_SIESUB_ADJ_VSIZE(height, IPL_CtrlGetInfor(id, IPLCTRL_SIE_CH3_CAP_SUBRATIO), 4);

	width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), ctrl_info->info->mode->ratio.ratio_h_v, width, 8);
	height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(id, IPLCTRL_SEN_CAP_CROP_RATIO), ctrl_info->info->mode->ratio.ratio_h_v, height, 4);

	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_BUFNUM, 1);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_H, width);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_CAPBIT, BitNum);
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(width, BitNum));
	IPL_CtrlSetInfor(id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_V, height);
	IPL_CtrlSetInfor(id, IPLCTRL_SENSOR_TYPE, ctrl_info->info->mode->mode_type);

}

void IPL_CtrlPrvInit(UINT32 id, IPLCTRL_INFO *ctrl_info)
{
	switch (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)) {
	case ICF_FLOW_C:
		IPL_CtrlPrvInitFlowCommon(id, ctrl_info);
		break;

	case ICF_FLOW_CCIR:
		IPL_CtrlPrvInitFlowCCIR(id, ctrl_info);
		break;

	case ICF_FLOW_VIRTUAL:
		IPL_CtrlPrvInitFlowVirtual(id, ctrl_info);
		break;

	default:
		DBG_ERR("id: %d, unsupport IPL flow %d\r\n", id, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
		break;
	}
}

#if 0
#endif

IPL_HAL_PARASET IPL_CtrlGetHalSetId(UINT32 id)
{

	IPL_MODE mode;

	mode = IPL_CtrlGetInfor(id, IPLCTRL_CUR_MODE);

	if ((IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW)) && (mode == IPL_MODE_CAP)) {
		switch (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)) {
		case ICF_FLOW_C:
		case ICF_FLOW_CCIR:
		case ICF_FLOW_VIRTUAL:
			return IPL_CtrlGetHalSetIdFlow(id);

		default:
			DBG_ERR("id: %d, unsupport IPL flow %d\r\n", id, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
			return IPL_HAL_PAR_CAP;
		}
	} else {
		if (mode == IPL_MODE_CAP) {
			return IPL_HAL_PAR_CAP;
		} else {

			switch (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)) {
			case ICF_FLOW_C:
			case ICF_FLOW_CCIR:
			case ICF_FLOW_VIRTUAL:
				return IPL_CtrlGetHalSetIdFlow(id);

			default:
				DBG_ERR("id: %d, unsupport IPL flow %d\r\n", id, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW));
				return IPL_HAL_PAR_CAP;
			}
		}
	}
}

ER IPL_Ctrl_Sen_ChgMode(UINT32 id, SEN_CHGMODE_DATA sen_mode_data)
{
	ER rt;
	rt = sensor_chg_mode(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), sen_mode_data);
	iq_init(id);
	return rt;
}

IPLBUF_MODE IPL_MapICF2BufMode(IPLCTRL_FLOW flow)
{
	switch (flow) {
	case ICF_FLOW_C:
	case ICF_FLOW_CCIR:
	case ICF_FLOW_VIRTUAL:
		return IPLBUF_MODE_FLOW_STREAM;
	default:
		DBG_ERR("flow map fail %d\r\n", flow);
		return IPLBUF_MODE_NULL;
	}
}

static BOOL IPL_CtrlChkOutEn(UINT32 id, IPL_IME_PATH_ID path_idx)
{
	UINT32 Pxidx[IPL_IME_PATH_ID_MAX] = {IPLCTRL_IME_P1_EN,	IPLCTRL_IME_P2_EN,	IPLCTRL_IME_P3_EN,	IPLCTRL_IME_P4_EN, IPLCTRL_IME_P5_EN};

	if (path_idx < IPL_IME_PATH_ID_MAX) {
		if (IPL_CtrlGetInfor(id, Pxidx[path_idx]) == ENABLE) {
			return TRUE;
		}
	} else {
		DBG_ERR("id: %d, path error %d\r\n", id, path_idx);
	}
	return FALSE;
}

void IPL_Ctrl_Out_Buf_Cfg(IPL_PROC_ID id, IPL_BUF_IO_CFG buf_io_cfg, IPL_YCC_ADDR_INFO *path[])
{
	UINT32 i;
	IPL_YCC_IMG_INFO ycc_info;
	UINT32 start_addr = 0, img_size, sideinfo_size, ext_out_size;
	IPL_IMG_BUF_INFO buf_info;
	IPL_IMG_OUT_INFO img_out_info = {0};
	IPL_YCC_CH_INFO y_info, uv_info;
	HEADER_YUV_DATA *pyuv_header_out;

	for (i = 0 ; i < IPL_IME_PATH_ID_MAX ; i++) {
		if (path[i] != NULL) {
			switch (buf_io_cfg) {
			case IPL_BUF_IO_GET_NEXT_OUT_ADDR:
				switch (i) {
				case IPL_IME_PATH_ID_1:
					ext_out_size = 0;
					if (path[i]->p1_enc_fmt != IME_P1_ENC_NONE) {
						y_info.width = path[i]->size.width;
						y_info.height = path[i]->size.height;
						y_info.line_ofs = path[i]->size.width;
						uv_info = IPL_UTI_Y_INFO_CONV2(path[i]->type, y_info);

						y_info.line_ofs = ALIGN_CEIL(path[i]->size.width, 128) << 2;
						uv_info.line_ofs = ALIGN_CEIL(uv_info.line_ofs, 128) * 3 / 4; //fixed encode ratio

						if (path[i]->p1_enc_fmt == IME_P1_ENC_HEVC) {
							y_info.height = ALIGN_CEIL(y_info.height, 64) >> 2;
							uv_info.height = ALIGN_CEIL(uv_info.height, 32);
						} else if (path[i]->p1_enc_fmt == IME_P1_ENC_H264) {
							y_info.height = ALIGN_CEIL(y_info.height, 16) >> 2;
							uv_info.height = ALIGN_CEIL(uv_info.height, 8);
						}

						img_size = y_info.line_ofs * y_info.height + uv_info.line_ofs * uv_info.height;
						img_size = IPL_ALIGN_ROUNDUP(img_size, 4);

						//side infor
						sideinfo_size = ipl_header_getsinfo_size(path[i]->size.width, path[i]->size.height, IPL_CtrlGetInfor(id, IPLCTRL_IME_P1_ENC_FMT));
					} else {
						//image buffer
						img_size = IPL_ALIGN_ROUNDUP(IPL_YUV_SIZE(path[i]->type, path[i]->size.line_ofs, path[i]->size.height), 4);
						y_info = path[i]->size;
						sideinfo_size = 0;
					}
					break;

				case IPL_IME_PATH_ID_2:
				case IPL_IME_PATH_ID_3:
				case IPL_IME_PATH_ID_4:
				case IPL_IME_PATH_ID_5:
					ext_out_size = 0;
					sideinfo_size = 0;
					//image buffer
					img_size = IPL_ALIGN_ROUNDUP(IPL_YUV_SIZE(path[i]->type, path[i]->size.line_ofs, path[i]->size.height), 4);
					y_info = path[i]->size;
					if (i == IPL_IME_PATH_ID_4 && IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE) {
						ext_out_size = IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS);
						ext_out_size += IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_HISTO_BUFOFS);
					}
					break;
				}

				if (IPL_CtrlGetInfor(id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_PATH_BIND && i == IPL_IME_PATH_ID_2) {
					img_out_info.Path_id = IPL_IME_PATH_ID_1;
				} else {
					img_out_info.Path_id = i;
				}
				if ((IPL_CtrlGetInfor(id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_VIRTUAL_SEN_IPE)||(IPL_CtrlChkOutEn(id, i) == TRUE)) {
					if (ipl_out_buf_cfg_cb[id] != NULL) {
						start_addr = ipl_out_buf_cfg_cb[id](buf_io_cfg, (img_size + sideinfo_size + ext_out_size), &img_out_info);
					} else {
						DBG_ERR("NULL IPL Out Buffer Cfg CB Fp\r\n");
						start_addr = 0;
					}
				} else {
					start_addr = 0;
				}

				IPL_YUVBufStsDump(id, buf_io_cfg, path[i], &img_out_info);	//debug

				//start address
				path[i]->addr_start = start_addr;
				path[i]->buf_handle = img_out_info.buf_handle;

				//yuv header address
				if (sizeof(HEADER_YUV_DATA) <= (total_dbg_data_size-sys_dbg_data_size)) {
					path[i]->pixel_yuv_header = start_addr + sys_dbg_data_size;
				} else {
					DBG_ERR("Id: %d, Path: %d, YUV Header buffer allocate fail, header size: %d > dbg_data_size %d \r\n", id, img_out_info.Path_id + 1, sizeof(HEADER_YUV_DATA), (total_dbg_data_size-sys_dbg_data_size));
				}

				//side info address
				path[i]->addr_side_info = start_addr + total_dbg_data_size;

				//ycc address
				ycc_info.type = path[i]->type;
				ycc_info.ch[0] = y_info;
				ycc_info.ch[1] = IPL_UTI_Y_INFO_CONV2(ycc_info.type, ycc_info.ch[0]);
				buf_info.Buf[0] = start_addr + sideinfo_size + total_dbg_data_size;
				buf_info.Buf[1] = 0;
				buf_info.Buf[2] = 0;

				if (start_addr == 0) {
					buf_info.Buf[0] = 0;
					path[i]->addr_side_info = 0;
				} else {
					memset((UINT32 *)start_addr, 0, total_dbg_data_size);   //total 1024 byte for debug information

				}

				IPL_UTI_SET_CONT_BUFINFO(ycc_info.type, buf_info, &ycc_info);
				path[i]->pixel_addr[0] = ycc_info.pixel_addr[0];
				path[i]->pixel_addr[1] = ycc_info.pixel_addr[1];
				path[i]->pixel_addr[2] = ycc_info.pixel_addr[2];

				if (i == IPL_IME_PATH_ID_4 && IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE) {
					path[i]->pixel_addr[1] = path[i]->pixel_addr[0] + img_size;	//Edge map
					path[i]->pixel_addr[2] = path[i]->pixel_addr[1] + IPL_CtrlGetInfor(id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS);	//Histogram buffer
				}

                if (IPL_CtrlGetInfor(id, IPLCTRL_FUNC_EN) & IPL_FUNC_VR360) {
                    memset((UINT32 *)path[i]->pixel_addr[0], 0x00, ycc_info.ch[0].line_ofs * ycc_info.ch[0].height);
                    if (path[i]->pixel_addr[1])
                        memset((UINT32 *)path[i]->pixel_addr[1], 0x80, ycc_info.ch[1].line_ofs * ycc_info.ch[1].height);
                    if (path[i]->pixel_addr[2])
                        memset((UINT32 *)path[i]->pixel_addr[2], 0x80, ycc_info.ch[2].line_ofs * ycc_info.ch[2].height);
                }

				break;

			case IPL_BUF_IO_PUSH_READY_ADDR:
				pyuv_header_out = (HEADER_YUV_DATA *) path[i]->pixel_yuv_header;
				if (IPL_CtrlGetInfor(id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_PATH_BIND && i == IPL_IME_PATH_ID_2) {
					img_out_info.Path_id = IPL_IME_PATH_ID_1;
				} else {
					img_out_info.Path_id = i;
				}
				img_out_info.buf_handle = path[i]->buf_handle;
				img_out_info.out_ycc_compress_en = pyuv_header_out->encode_info.enable;
				img_out_info.raw_framecnt = pyuv_header_out->raw_info.framecnt[0];
				img_out_info.vd_timestamp = pyuv_header_out->raw_info.timestamp[0];
				img_out_info.dramend_timestamp = pyuv_header_out->raw_info.dramend_timestamp[0];
				img_out_info.ipl_timestamp_start = pyuv_header_out->ipl_timestamp_start;
				img_out_info.ipl_timestamp_end = pyuv_header_out->ipl_timestamp_end;
				img_out_info.img = pyuv_header_out->img_info;

				{
					static UINT32 last_count = 0x12345678;

					if(last_count != img_out_info.raw_framecnt) {
						last_count = img_out_info.raw_framecnt;
						dis_push_time_stamp(img_out_info.ipl_timestamp_start,img_out_info.raw_framecnt);
					}
				}

				IPL_YUVBufStsDump(id, buf_io_cfg, path[i], &img_out_info);	//debug

				if (ipl_out_buf_cfg_cb[id] != NULL) {
					ipl_out_buf_cfg_cb[id](buf_io_cfg, path[i]->addr_start, &img_out_info);
				} else {
					DBG_ERR("NULL IPL Out Buffer Cfg CB Fp\r\n");
				}
				break;

			case IPL_BUF_IO_LOCK_BUF:
			case IPL_BUF_IO_UNLOCK_BUF:
				if (IPL_CtrlGetInfor(id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_PATH_BIND && i == IPL_IME_PATH_ID_2) {
					img_out_info.Path_id = IPL_IME_PATH_ID_1;
				} else {
					img_out_info.Path_id = i;
				}
				img_out_info.buf_handle = path[i]->buf_handle;

				IPL_YUVBufStsDump(id, buf_io_cfg, path[i], &img_out_info);	//debug

				if (ipl_out_buf_cfg_cb[id] != NULL) {
					ipl_out_buf_cfg_cb[id](buf_io_cfg, path[i]->addr_start, &img_out_info);
				} else {
					DBG_ERR("NULL IPL Out Buffer Cfg CB Fp\r\n");
				}
				break;

			case IPL_BUF_IO_START:
			case IPL_BUF_IO_STOP:
				for (i = 0 ; i < IPL_IME_PATH_ID_MAX ; i++) {
					if (path[i] != NULL) {
						img_out_info.Path_id = i;

						IPL_YUVBufStsDump(id, buf_io_cfg, path[i], &img_out_info);	//debug

						if (ipl_out_buf_cfg_cb[id] != NULL) {
							ipl_out_buf_cfg_cb[id](buf_io_cfg, path[i]->addr_start, &img_out_info);
						} else {
							DBG_ERR("NULL IPL Out Buffer Cfg CB Fp\r\n");
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}
}

#if 0
#endif

void IPL_Stream2PauseFCB(IPL_MODE_DATA **ipl_mode_info, IPL_MODE_DATA *chg_mode, UINT32 *trig_flg)
{
	UINT32 id, i;
	SEN_STATUS_PARAM sen_status = {0};
	IPLCTRL_INFO ctrl_info = {0};
	IPC_CTRL_CMD trig_stop_cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	IPC_CTRL_CMD trig_pause_cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	static IPL_MODE prv_mode[IPL_ID_MAX_NUM] = {IPL_MODE_OFF}; //Previous IPL Mode
	IPL_PROC_BIT_ID ipl_stop_id_bit = 0, ipl_pause_id_bit = 0;
	ER rt = E_OK;

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nS2P-> PRV->Nxt(Act):\r\n%s->%s(%d),%s->%s(%d),%s->%s(%d),%s->%s(%d),\r\n%s->%s(%d),%s->%s(%d),%s->%s(%d),%s->%s(%d)\r\n",
				 ipl_dbg_mode_tbl[prv_mode[0]], ipl_dbg_mode_tbl[chg_mode[0].mode], chg_mode[0].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[1]], ipl_dbg_mode_tbl[chg_mode[1].mode], chg_mode[1].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[2]], ipl_dbg_mode_tbl[chg_mode[2].mode], chg_mode[2].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[3]], ipl_dbg_mode_tbl[chg_mode[3].mode], chg_mode[3].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[4]], ipl_dbg_mode_tbl[chg_mode[4].mode], chg_mode[4].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[5]], ipl_dbg_mode_tbl[chg_mode[5].mode], chg_mode[5].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[6]], ipl_dbg_mode_tbl[chg_mode[6].mode], chg_mode[6].act_flg,
				 ipl_dbg_mode_tbl[prv_mode[7]], ipl_dbg_mode_tbl[chg_mode[7].mode], chg_mode[7].act_flg);
	}

	for (i = 0 ; i < IPL_ID_MAX_NUM ; i++) {
		if (chg_mode[i].act_flg == TRUE) {
			switch (chg_mode[i].mode) {
			case IPL_MODE_OFF:
				if (prv_mode[i] == IPL_MODE_OFF) {
					trig_flg[i] = IPL_NONE;
				} else {
					trig_flg[i] = IPL_OFF;
				}
				break;

			case IPL_MODE_PREVIEW:
			case IPL_MODE_VIDEO:
			case IPL_MODE_VIDEOREC:
				IPL_CtrlSetInfor(IPL_ID_1, IPLCTRL_IME_OUT_GROUP_BUF_ARRANGE, chg_mode->ime_path1_out_group.group_en);//Jarkko tmp, not ready
				if (prv_mode[i] == IPL_MODE_CAP) {
					trig_flg[i] = IPL_RESUME;
				} else if (prv_mode[i] == IPL_MODE_OFF || prv_mode[i] == IPL_MODE_BUF_QUERY) {
					trig_flg[i] = IPL_START;
				} else {
					trig_flg[i] = IPL_STOP;
				}
				break;

			case IPL_MODE_CAP:
				trig_flg[i] = IPL_PAUSE;
				gCapFlg = TRUE;
				break;

			default:
				trig_flg[i] = IPL_NONE;
				break;
			}
			prv_mode[i] = chg_mode[i].mode;
		}
	}

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nS2P-> Cur->Nxt(Trg):\r\n%s->%s(%s),%s->%s(%s),%s->%s(%s),%s->%s(%s),\r\n%s->%s(%s),%s->%s(%s),%s->%s(%s),%s->%s(%s)\r\n\r\n",
				 ipl_dbg_mode_tbl[prv_mode[0]], ipl_dbg_mode_tbl[chg_mode[0].mode], ipl_dbg_trigflg_tbl[trig_flg[0]],
				 ipl_dbg_mode_tbl[prv_mode[1]], ipl_dbg_mode_tbl[chg_mode[1].mode], ipl_dbg_trigflg_tbl[trig_flg[1]],
				 ipl_dbg_mode_tbl[prv_mode[2]], ipl_dbg_mode_tbl[chg_mode[2].mode], ipl_dbg_trigflg_tbl[trig_flg[2]],
				 ipl_dbg_mode_tbl[prv_mode[3]], ipl_dbg_mode_tbl[chg_mode[3].mode], ipl_dbg_trigflg_tbl[trig_flg[3]],
				 ipl_dbg_mode_tbl[prv_mode[4]], ipl_dbg_mode_tbl[chg_mode[4].mode], ipl_dbg_trigflg_tbl[trig_flg[4]],
				 ipl_dbg_mode_tbl[prv_mode[5]], ipl_dbg_mode_tbl[chg_mode[5].mode], ipl_dbg_trigflg_tbl[trig_flg[5]],
				 ipl_dbg_mode_tbl[prv_mode[6]], ipl_dbg_mode_tbl[chg_mode[6].mode], ipl_dbg_trigflg_tbl[trig_flg[6]],
				 ipl_dbg_mode_tbl[prv_mode[7]], ipl_dbg_mode_tbl[chg_mode[7].mode], ipl_dbg_trigflg_tbl[trig_flg[7]]);
	}

	//stop all other ipl when capture
	for (id = 0 ; id < IPL_ID_MAX_NUM ; id++) {
		if (gCapFlg == TRUE) {
			if (prv_mode[id] == IPL_MODE_PREVIEW || prv_mode[id] == IPL_MODE_VIDEO || prv_mode[id] == IPL_MODE_VIDEOREC) {
				trig_flg[id] = IPL_STOP;
			}
		}

		//RSC CB MSG
		IPL_RSC_CBMsgProc(id, IPL_RSC_CBMSG_CHGMODE_START, NULL);

		switch (trig_flg[id]) {
		case IPL_STOP:
		case IPL_PAUSE:
			//pause AE/AWB/IQ/ICFFunc
			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
				if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_C) {
					AE_Pause(IPL_UTI_CONV2_AE_ID(id), ENABLE);
					AWB_Pause(IPL_UTI_CONV2_AWB_ID(id), ENABLE);
					if (id == IPL_ID_1) {
						AF_Pause(IPL_UTI_CONV2_AF_ID(id), ENABLE);
					}
					IQS_Pause(IPL_UTI_CONV2_IQS_ID(id), ENABLE);
					ICFFunc_Pause(id, ENABLE);
					ExtFunc_UnInit(id);
				} else if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) {
					IQS_Pause(IPL_UTI_CONV2_IQS_ID(id), ENABLE);
				}

			}

			if (trig_flg[id] == IPL_STOP) {
				ipl_stop_id_bit |= 1 << id;
				trig_stop_cmd[id] = IPC_STOP_ALL;
				trig_flg[id] = IPL_RESTART;
			} else {
				if (chg_mode[id].event_id == IPL_FLOW_VID_GETRAW) {
					if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
						//initial capture parameter
						sen_status.sensor_mode = IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_SEN_MODE);
						rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
						if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
							DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(chg_mode[id].proc_sen_id, IPLCTRL_SEN_MODE));
						}
					}

					ctrl_info.info = &sen_status.sensor_info;
					ctrl_info.chg_mode = &chg_mode[id];
					IPL_CtrlCapInit(chg_mode[id].proc_sen_id, chg_mode[id].event_id, &ctrl_info);
					chg_mode[id].buf_size = 0;
				} else {
					if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
						//PreAE Setting
						IPL_FCB_PrepareCapAE(chg_mode[id].proc_sen_id);
						//PreFlash Setting
						IPL_FCB_PreFlash(chg_mode[id].proc_sen_id);
						//stop IPL
						trig_pause_cmd[chg_mode[id].proc_sen_id] = IPC_STOP_YCC;
						ipl_pause_id_bit = IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT);
						IPL_Ctrl_Run(ipl_pause_id_bit, &trig_pause_cmd[IPL_ID_1]);
						IPL_Wait_Vd(ipl_pause_id_bit, FALSE, IPL_TIMEOUT_MS);
						trig_pause_cmd[chg_mode[id].proc_sen_id] = IPC_PAUSE_SIE;
						ipl_pause_id_bit = IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT);
						IPL_Ctrl_Run(ipl_pause_id_bit, &trig_pause_cmd[IPL_ID_1]);
						trig_pause_cmd[chg_mode[id].proc_sen_id] = IPC_IGNORE;

						IPL_RSC_CBMsgProc(chg_mode[id].proc_sen_id, IPL_RSC_CBMSG_CHGMODE_IPLSTOP, NULL);
					}
				}
				trig_flg[id] = IPL_RESUME;
			}
			break;
		case IPL_OFF:
			ipl_stop_id_bit |= 1 << id;
			trig_stop_cmd[id] = IPC_STOP_ALL;

			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
				AWB_Close(IPL_UTI_CONV2_AWB_ID(id));
				AE_Close(IPL_UTI_CONV2_AE_ID(id));
				if (id == IPL_ID_1) {
					AF_Close(IPL_UTI_CONV2_AF_ID(id));
				}
				IQS_Close(IPL_UTI_CONV2_IQS_ID(id));
				ICFFunc_Close(id);
				ExtFunc_UnInit(id);
			}
			break;

		default:
			ipl_stop_id_bit |= 1 << id;
			trig_stop_cmd[id] = IPC_IGNORE;
			break;
		}
	}
	IPL_Ctrl_Run(ipl_stop_id_bit, &trig_stop_cmd[IPL_ID_1]);
	//IPL STOP MSG
	for (id = 0 ; id < IPL_ID_MAX_NUM ; id++) {
		if (trig_stop_cmd[id] != IPC_IGNORE) {
			IPL_RSC_CBMsgProc(id, IPL_RSC_CBMSG_CHGMODE_IPLSTOP, NULL);
		}
	}
}

void IPL_Pause2StreamFCB(IPL_MODE_DATA **ipl_mode_info, IPL_MODE_DATA *chg_mode, UINT32 *trig_flg, IPC_CTRL_CMD *proc_cmd)
{
	UINT32 zoom_idx[IPL_ID_MAX_NUM] = {0};
	IPL_ISR_OBJ_INFO IsrObjInfo = {0};
	IPC_INFO IpcInfo = {0};
	IPLCTRL_INFO ctrl_info = {0};
	SEN_STATUS_PARAM sen_status = {0};
	UINT32 *zoom_tab, zoom_tab_max = 0;
	UINT32 id;
	IPL_PROC_BIT_ID ipl_proc_id_bit = 0;
	UINT32 Group = 0, Order = 0;
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	IPL_BUF_OBJ_INFO buf_obj;
	IPLBUF_ENG2DRAMINFO buf_info;
	IPLBUF_RST buf_rst = {0};
	ER rt = E_OK;

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nP2S-> Cur->Nxt(Act):\r\n%s->%s(%s),%s->%s(%s),%s->%s(%s),%s->%s(%s),\r\n%s->%s(%s),%s->%s(%s),%s->%s(%s),%s->%s(%s)\r\n\r\n",
				 ipl_dbg_mode_tbl[ipl_mode_info[0]->mode], ipl_dbg_mode_tbl[chg_mode[0].mode], ipl_dbg_trigflg_tbl[trig_flg[0]],
				 ipl_dbg_mode_tbl[ipl_mode_info[1]->mode], ipl_dbg_mode_tbl[chg_mode[1].mode], ipl_dbg_trigflg_tbl[trig_flg[1]],
				 ipl_dbg_mode_tbl[ipl_mode_info[2]->mode], ipl_dbg_mode_tbl[chg_mode[2].mode], ipl_dbg_trigflg_tbl[trig_flg[2]],
				 ipl_dbg_mode_tbl[ipl_mode_info[3]->mode], ipl_dbg_mode_tbl[chg_mode[3].mode], ipl_dbg_trigflg_tbl[trig_flg[3]],
				 ipl_dbg_mode_tbl[ipl_mode_info[4]->mode], ipl_dbg_mode_tbl[chg_mode[4].mode], ipl_dbg_trigflg_tbl[trig_flg[4]],
				 ipl_dbg_mode_tbl[ipl_mode_info[5]->mode], ipl_dbg_mode_tbl[chg_mode[5].mode], ipl_dbg_trigflg_tbl[trig_flg[5]],
				 ipl_dbg_mode_tbl[ipl_mode_info[6]->mode], ipl_dbg_mode_tbl[chg_mode[6].mode], ipl_dbg_trigflg_tbl[trig_flg[6]],
				 ipl_dbg_mode_tbl[ipl_mode_info[7]->mode], ipl_dbg_mode_tbl[chg_mode[7].mode], ipl_dbg_trigflg_tbl[trig_flg[7]]);
	}

	if (gCapFlg == TRUE) {
		gCapFlg = FALSE;
		return;
	}

	for (id = 0 ; id < IPL_ID_MAX_NUM ; id++) {
		switch (trig_flg[id]) {
		case IPL_START:
		case IPL_RESTART:
			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
				//change sensor mode
				sen_chg_mode.mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
				sen_chg_mode.multi_id = IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT);
				sen_chg_mode.frame_rate = IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS);
				IPL_Ctrl_Sen_ChgMode(id, sen_chg_mode);

				//get mode status
				sen_status.sensor_mode = IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE);
				rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
				if (rt != E_OK || sen_status.sensor_info.mode == NULL) {
					DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_CtrlGetInfor(id, IPLCTRL_SEN_MODE));
					return;
				}

				zoom_tab = ipl_ext_senmode2tbl(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), &zoom_tab_max);
				IPL_DZoomSetInfor(id, zoom_tab, zoom_tab_max);
			}

			ctrl_info.info = &sen_status.sensor_info;
			chg_mode[id] = *ipl_mode_info[id];
			ctrl_info.chg_mode = &chg_mode[id];
			IPL_CtrlPrvInit(id, &ctrl_info);

			//allocate buffer & initial buffer size
			buf_obj.id = ctrl_info.chg_mode->proc_sen_id;
			buf_obj.buf_addr = ctrl_info.chg_mode->buf_addr;
			buf_obj.buf_size = ctrl_info.chg_mode->buf_size;

			IPL_BufInit(&buf_obj);
			IPL_CtrlInitPrvbuf_info(&chg_mode[id], &buf_info);
			buf_rst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW)), &buf_info);

			if (b_ipl_ctrl_dbg_en) {
				IPL_BufDump(buf_obj.id); //DEBUG Using, use for dump buffer information
			}
			ctrl_info.chg_mode->buf_size = buf_rst.buf_size;

			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) != ICF_FLOW_VIRTUAL) {
				//initial digital zoom
				zoom_idx[id] = IPL_DzoomGetIndex(id);
				IPL_DzoomSetIndex(id, zoom_idx[id]);
				IPL_CtrlPrvSetDZoomInfo(id);
			}
			IPL_CtrlInitPrvIpcInfo(id, &IpcInfo, &IsrObjInfo, &chg_mode[id]);

			if (IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_NUM) > 1) {
				Order = IPL_UTI_CONV2_ORDER(id, IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT));
				Group = IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID);
			} else {
				Order = IPL_CtrlGetInfor(id, IPLCTRL_ORDER_IDX);
				Group = IPL_CtrlGetInfor(id, IPLCTRL_GROUP_IDX);
			}

			IPL_Ctrl_Cfg(Group, id, Order, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_Ctrl_Out_Buf_Cfg);
			ipl_proc_id_bit |= 1 << id;
			proc_cmd[id] = IPC_START_RUN;
			break;

		case IPL_RESUME:
			IPL_CtrlInitPrvIpcInfo(id, &IpcInfo, &IsrObjInfo, &chg_mode[id]);
			Order = IPL_UTI_CONV2_ORDER(id, IPL_CtrlGetInfor(id, IPLCTRL_MF_TOTAL_FRM_BIT));
			IPL_Ctrl_Cfg(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), id, Order, IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_Ctrl_Out_Buf_Cfg);
			ipl_proc_id_bit |= 1 << id;
			proc_cmd[id] = IPC_START_RUN;
			break;

		case IPL_OFF:
			proc_cmd[id] = IPC_STOP_ALL;
			break;

		default:
			break;
		}
	}

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("\r\nTrig Bit 0x%x, Cmd %d ,%d ,%d ,%d ,%d ,%d ,%d ,%d\r\n\r\n",
				 ipl_proc_id_bit,
				 proc_cmd[IPL_ID_1], proc_cmd[IPL_ID_2], proc_cmd[IPL_ID_3], proc_cmd[IPL_ID_4],
				 proc_cmd[IPL_ID_5], proc_cmd[IPL_ID_6], proc_cmd[IPL_ID_7], proc_cmd[IPL_ID_8]);
	}

	IPL_Ctrl_Run(ipl_proc_id_bit, &proc_cmd[IPL_ID_1]);

	for (id = IPL_ID_1; id < IPL_ID_MAX_NUM ; id++) {
		if (proc_cmd[id] == IPC_START_RUN) {
			if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_C || IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_CCIR) {
				if (IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) == id) {
					ipl_proc_id_bit = 1 << id;
					IPL_Wait_Ime_Fmd(ipl_proc_id_bit, TRUE, IPL_TIMEOUT_MS);

					IPL_CBMsgProc(id, IPL_CBMSG_PREVIEW, NULL);
					//Trigger RSC
					IPL_RSC_CBMsgProc(id, IPL_RSC_CBMSG_CHGMODE_END, NULL);

					if (trig_flg[id] != IPL_START) {
						IQS_Resume(IPL_UTI_CONV2_IQS_ID(id));
						if (IPL_CtrlGetInfor(id, IPLCTRL_PRVFLOW) == ICF_FLOW_C) {
							AE_Resume(IPL_UTI_CONV2_AE_ID(id));
							AWB_Resume(IPL_UTI_CONV2_AWB_ID(id));
							if (id == IPL_ID_1) {
								AF_Resume(IPL_UTI_CONV2_AF_ID(id));
							}
							ICFFunc_Resume(id);
						}
					}
				}
			}
		}
		trig_flg[id] = IPL_NONE;
	}
}

void IPL_YUVBufStsDump(IPL_PROC_ID id, IPL_BUF_IO_CFG buf_io_cfg, IPL_YCC_ADDR_INFO *path, IPL_IMG_OUT_INFO *img_out_info)
{
	static UINT8 dump_buf_cnt[IPL_ID_MAX_NUM] = {0};

	if (ipl_yuv_buf_dump_id == id) {
		switch (buf_io_cfg) {
		case IPL_BUF_IO_GET_NEXT_OUT_ADDR:
			dump_buf_cnt[id]++;
			DBG_DUMP("[Get] Id:%d, p:%d, comp:%d, hdl:%x, size (%d, %d), lofs:%d\r\n",
				id, img_out_info->Path_id + 1,
				img_out_info->out_ycc_compress_en,
				img_out_info->buf_handle,
				path->size.width,
				path->size.height,
				path->size.line_ofs);
			break;

		case IPL_BUF_IO_PUSH_READY_ADDR:
			DBG_DUMP("[Push] Id:%d, p:%d, comp:%d, hdl:%x, frm_cnt:%d, y_addr:%#x, c_addr:%#x, size (%d, %d), lofs:%d\r\n",
				id,
				img_out_info->Path_id + 1,
				img_out_info->out_ycc_compress_en,
				img_out_info->buf_handle,
				img_out_info->raw_framecnt,
				img_out_info->img.pixel_addr[0],
				img_out_info->img.pixel_addr[1],
				img_out_info->img.ch[0].width,
				img_out_info->img.ch[0].height,
				img_out_info->img.ch[0].line_ofs);
			break;

		case IPL_BUF_IO_LOCK_BUF:
			DBG_DUMP("[Lock], id:%d, p:%d, hdl:%x\r\n", id, img_out_info->Path_id+1, img_out_info->buf_handle);
			break;

		case IPL_BUF_IO_UNLOCK_BUF:
			DBG_DUMP("[UnLock] id:%d, p:%d, hdl:%x, addr:%x\r\n", id, img_out_info->Path_id+1, img_out_info->buf_handle, path->addr_start);
			break;

		case IPL_BUF_IO_START:
			DBG_DUMP("[Start] id %d, p %d\r\n", id, img_out_info->Path_id+1);
			break;

		case IPL_BUF_IO_STOP:
			DBG_DUMP("[Stop] id %d, p %d\r\n", id, img_out_info->Path_id+1);
			break;

		default:
			break;
		}
	}

	if (dump_buf_cnt[id] >= ipl_yuv_buf_dump_cnt) {
		ipl_yuv_buf_dump_id = IPL_ID_MAX_NUM;
		dump_buf_cnt[id] = 0;
	}
}
