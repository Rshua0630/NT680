/*
    IPL command API

    This file is the API of the IPL control.

    @file       ipl_ext_cmd.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "ipl_ext_common.h"

void IPL_SetDZoomFCB(IPL_DZOOM *info, IPC_FRONTEND *set_info)
{
	IPL_CtrlPrvSetDZoomInfo(info->id);
	IPL_CtrlPrvGetDZoomInfo(info->id, set_info);
}

void IPL_SetSleepFCB(IPL_SLEEP_INFO *info)
{
	IPL_PROC_BIT_ID ipl_proc_id_bit = 0;
	UINT32 i;

	IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	AE_Pause(IPL_UTI_CONV2_AE_ID(info->id), ENABLE);
	AWB_Pause(IPL_UTI_CONV2_AWB_ID(info->id), ENABLE);
	IPL_Wait_Vd((IPL_PROC_BIT_ID)(1 << info->id), TRUE, IPL_TIMEOUT_MS);

	ProcCmd[info->id] = IPC_PAUSE_SIE;
	ipl_proc_id_bit = 1 << info->id;
	if (IPL_CtrlGetInfor(info->id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) {
		ipl_proc_id_bit = IPL_CtrlGetInfor(info->id, IPLCTRL_MF_TOTAL_FRM_BIT);
		for (i = info->id + 1 ; i < IPL_ID_MAX_NUM ; i++) {
			if ((ipl_proc_id_bit & (1 << i)) && (i < IPL_ID_MAX_NUM)) {
				ProcCmd[i] = IPC_PAUSE_SIE;
			}
		}
	}

	IPL_Ctrl_Run(ipl_proc_id_bit, &ProcCmd[IPL_ID_1]);
	IPL_Wait_Vd(ipl_proc_id_bit, FALSE, IPL_TIMEOUT_MS);
	drv_sensor_power_save_on(IPL_UTI_CONV2_SEN_ID(info->id));
}

void IPL_SetWakeupFCB(IPL_WAKEUP_INFO *info)
{
	IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	IPL_PROC_BIT_ID ipl_proc_id_bit = 0;
	UINT32 i;
	drv_sensor_power_save_off(IPL_UTI_CONV2_SEN_ID(info->id));

	ProcCmd[info->id] = IPC_RESUME_SIE;
	ipl_proc_id_bit = 1 << info->id;
	if (IPL_CtrlGetInfor(info->id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) {
		ipl_proc_id_bit = IPL_CtrlGetInfor(info->id, IPLCTRL_MF_TOTAL_FRM_BIT);
		for (i = info->id + 1 ; i < IPL_ID_MAX_NUM ; i++) {
			if ((ipl_proc_id_bit & (1 << i)) && (i < IPL_ID_MAX_NUM)) {
				ProcCmd[i] = IPC_RESUME_SIE;
			}
		}
	}

	IPL_Ctrl_Run(ipl_proc_id_bit, &ProcCmd[IPL_ID_1]);
	IPL_Wait_Vd(ipl_proc_id_bit, FALSE, IPL_TIMEOUT_MS);
	AE_Resume(IPL_UTI_CONV2_AE_ID(info->id));
	AWB_Resume(IPL_UTI_CONV2_AWB_ID(info->id));
}

void IPL_GetCapRawFCB(IPL_GET_CAP_RAW_DATA *info)
{
	IPC_SIE_Info sie_info = {0};
	SIE sie = {0};
	SIE_SUBFUNC sub_func = {0};
	IPLCTRL_CAP_EXT_INFO ext_info = {0};
	SIE_DVI dvi_info = {0};

	//prepare ipc sie information
	sie_info.Idx = IPL_CtrlGetHalSetId(info->id);
	sie_info.psie = &sie;
	sie_info.psie->pSubFunc = &sub_func;
	sie_info.psie->pSubFunc->pDvi = &dvi_info;
	switch (IPL_CtrlGetInfor(info->id, IPLCTRL_CAPFLOW)) {
	case IPLCTRL_CAP_FLOW_HDR:
		IPL_CtrlInitCapIpcInfo(info, &sie_info, &ext_info);
		IPL_FCB_GetCapRaw(info->id, &sie_info, &ext_info);
		break;

	case IPLCTRL_CAP_FLOW_MFHDR:
		IPL_CtrlInitCapIpcInfo(info, &sie_info, &ext_info);
		IPL_FCB_GetCapRaw(info->id, &sie_info, &ext_info);
		break;

	case IPLCTRL_CAP_FLOW_DUALSBS:
		//output to raw buffer 0
		ext_info.param[6] = info->res[6];
		IPL_CtrlInitCapIpcInfo(info, &sie_info, &ext_info);
		IPL_FCB_GetCapRaw(info->id, &sie_info, &ext_info);
		break;

	case IPLCTRL_CAP_FLOW_RHDR:
		IPL_CtrlInitCapIpcInfo(info, &sie_info, &ext_info);
		IPL_FCB_GetCapRaw(info->id, &sie_info, &ext_info);
		break;
	default:
		IPL_CtrlInitCapIpcInfo(info, &sie_info, &ext_info);
		IPL_FCB_GetCapRaw(info->id, &sie_info, &ext_info);
		//AWB setting
		IPL_FCB_SetCapAWB(info->id, &sie_info);
		break;
	}
}

void IPL_GetCapMFHDRCB(IPL_CAP_MFHDR_INFOR *info)
{
	UINT32 hdr_ev_gap;

	hdr_ev_gap = ae_get_ui_info((AE_PROC_ID)info->id, AE_UI_CAP_MFHDR_EV);

	if( hdr_ev_gap == AE_CAP_MFHDR_1EV) {
		info->ev_gap = 1;
	} else if(hdr_ev_gap == AE_CAP_MFHDR_2EV) {
		info->ev_gap = 2;
	} else {
		DBG_WRN("MGHDR ev error (%d)\r\n",hdr_ev_gap);
		info->ev_gap = 1;
	}
	return;
}

SIE_SUBFUNC gSieSubFunc[IPL_ID_MAX_NUM] = {0}; //for Pauase/Resume DMA
void IPL_SetPauseDMAFCB(IPL_PAUSEDMA_INFO *info)
{
	IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	IPL_PROC_BIT_ID ipl_proc_id_bit = 0;
	UINT32 i;

	AE_Pause(IPL_UTI_CONV2_AE_ID(info->id), ENABLE);
	AWB_Pause(IPL_UTI_CONV2_AWB_ID(info->id), ENABLE);

	ProcCmd[info->id] = IPC_PAUSE_SIE;

	ipl_proc_id_bit = 1 << info->id;
	if (IPL_CtrlGetInfor(info->id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR)  {
		ipl_proc_id_bit = IPL_CtrlGetInfor(info->id, IPLCTRL_MF_TOTAL_FRM_BIT);
		for (i = info->id + 1 ; i < IPL_ID_MAX_NUM ; i++) {
			if ((ipl_proc_id_bit & (1 << i)) && (i < IPL_ID_MAX_NUM)) {
				ProcCmd[i] = IPC_PAUSE_SIE;
			}
		}
	}

	IPL_Ctrl_Run(ipl_proc_id_bit, &ProcCmd[IPL_ID_1]);
	IPL_Wait_Vd(ipl_proc_id_bit, FALSE, IPL_TIMEOUT_MS);
}

void IPL_SetResumeDMAFCB(IPL_RESUMEDMA_INFO *info)
{
	IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE};
	IPL_PROC_BIT_ID ipl_proc_id_bit = 0;
	UINT32 i;

	ProcCmd[info->id] = IPC_RESUME_SIE;
	ipl_proc_id_bit |= 1 << info->id;
	if (IPL_CtrlGetInfor(info->id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) {
		ipl_proc_id_bit = IPL_CtrlGetInfor(info->id, IPLCTRL_MF_TOTAL_FRM_BIT);
		for (i = info->id + 1 ; i < IPL_ID_MAX_NUM ; i++) {
			if ((ipl_proc_id_bit & (1 << i)) && (i < IPL_ID_MAX_NUM)) {
				ProcCmd[i] = IPC_RESUME_SIE;
			}
		}
	}

	IPL_Ctrl_Run(ipl_proc_id_bit, &ProcCmd[IPL_ID_1]);
	IPL_Wait_Vd(ipl_proc_id_bit, FALSE, IPL_TIMEOUT_MS);

	AE_Resume(IPL_UTI_CONV2_AE_ID(info->id));
	AWB_Resume(IPL_UTI_CONV2_AWB_ID(info->id));
}

IPL_IME_PATH IPL_SetImgInOutFCB(IPL_SET_IMGINOUT_DATA *info, IPC_IMEOut_Info *ipc_info_p1, IPC_IMEOut_Info *ipc_info_p2, IPC_IMEOut_Info *ipc_info_p3, IPC_IMEOut_Info *ipc_info_p4, IPC_IMEOut_Info *ipc_info_p5)
{
	IPL_IME_PATH rt = IPL_IME_PATH_NONE;
	IPL_IME_PATH_INFO tmp_ime_info = {0};
	IPL_YCC_CH_INFO tmp_info = {0};

	if (info->path_id & IPL_IME_PATH1) {
		//p1
		ipc_info_p1->Idx = IPL_CtrlGetHalSetId(info->id);
		IPL_Setime_path_Info(info->id, IPL_IME_PATH1, &info->ime_path_1);
		IPL_Getime_path_Info(info->id, IPL_IME_PATH1, &tmp_ime_info);
		ipc_info_p1->Path.PathEn = tmp_ime_info.output_en;
		ipc_info_p1->Path.DmaOutEn = info->ime_path_1.output_en;
		if (tmp_ime_info.output_en == ENABLE) {
			ipc_info_p1->sample_rate = tmp_ime_info.sample_rate;
			ipc_info_p1->p1_encode_en = tmp_ime_info.out_ycc_compress_en;
			if (tmp_ime_info.out_ycc_enc_fmt == IME_P1_ENC_HEVC) {
				ipc_info_p1->p1_sinfo_fmt = IME_HAL_SINFO_MT_HEVC;
			} else {
				ipc_info_p1->p1_sinfo_fmt = IME_HAL_SINFO_MT_H264;
			}
			ipc_info_p1->Path.Size.Y.width = tmp_ime_info.scale_size_h;
			ipc_info_p1->Path.Size.Y.height = tmp_ime_info.scale_size_v;
			ipc_info_p1->Path.Size.Y.line_ofs = tmp_ime_info.out_img_lofs;
			ipc_info_p1->Path.Fmt = tmp_ime_info.img_fmt;
			ipc_info_p1->Path.Crop.Start.x = tmp_ime_info.out_crop_start_x;
			ipc_info_p1->Path.Crop.Start.y = tmp_ime_info.out_crop_start_y;
			ipc_info_p1->Path.Crop.SizeH = tmp_ime_info.out_crop_size_h;
			ipc_info_p1->Path.Crop.SizeV = tmp_ime_info.out_crop_size_v;
			tmp_info = IPL_UTI_Y_INFO_CONV2(ipc_info_p1->Path.Fmt, ipc_info_p1->Path.Size.Y);
			ipc_info_p1->Path.Size.Lineoffset_CC = tmp_info.line_ofs;

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path1], en: %d, enc: %d, scl_size: (%d, %d), crp_start: (%d, %d), crp_size: (%d, %d), lofs: %d, smp_rate: %x\r\n",
						 info->id, info->ime_path_1.output_en, info->ime_path_1.out_ycc_compress_en,
						 ipc_info_p1->Path.Size.Y.width, ipc_info_p1->Path.Size.Y.height,
						 ipc_info_p1->Path.Crop.Start.x, ipc_info_p1->Path.Crop.Start.y, ipc_info_p1->Path.Crop.SizeH, ipc_info_p1->Path.Crop.SizeV,
						 ipc_info_p1->Path.Size.Y.line_ofs, ipc_info_p1->sample_rate);
			}

			if (IPL_CtrlGetInfor(info->id, IPLCTRL_IME_P1_ENC_EN) == ENABLE) {
				ipc_info_p1->Path.Size.Y.line_ofs = ALIGN_CEIL(ipc_info_p1->Path.Size.Y.line_ofs, 128) << 2;
				ipc_info_p1->Path.Size.Lineoffset_CC = ALIGN_CEIL(ipc_info_p1->Path.Size.Lineoffset_CC, 128) * 3 / 4;
			}
		} else {
			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path1], en: %d\r\n", info->id, info->ime_path_1.output_en);
			}
		}
		rt |= IPL_IME_PATH1;
	}

	if (info->path_id & IPL_IME_PATH2) {
		//p2
		ipc_info_p2->Idx = IPL_CtrlGetHalSetId(info->id);
		IPL_Setime_path_Info(info->id, IPL_IME_PATH2, &info->ime_path_2);
		IPL_Getime_path_Info(info->id, IPL_IME_PATH2, &tmp_ime_info);
		ipc_info_p2->Path.PathEn = tmp_ime_info.output_en;
		ipc_info_p2->Path.DmaOutEn = tmp_ime_info.output_en;
		if (tmp_ime_info.output_en == ENABLE) {
			ipc_info_p2->sample_rate = tmp_ime_info.sample_rate;
			ipc_info_p2->Path.Size.Y.width = tmp_ime_info.scale_size_h;
			ipc_info_p2->Path.Size.Y.height = tmp_ime_info.scale_size_v;
			ipc_info_p2->Path.Size.Y.line_ofs = tmp_ime_info.out_img_lofs;
			ipc_info_p2->Path.Fmt = tmp_ime_info.img_fmt;
			ipc_info_p2->Path.Crop.Start.x = tmp_ime_info.out_crop_start_x;
			ipc_info_p2->Path.Crop.Start.y = tmp_ime_info.out_crop_start_y;
			ipc_info_p2->Path.Crop.SizeH = tmp_ime_info.out_crop_size_h;
			ipc_info_p2->Path.Crop.SizeV = tmp_ime_info.out_crop_size_v;
			tmp_info = IPL_UTI_Y_INFO_CONV2(ipc_info_p2->Path.Fmt, ipc_info_p2->Path.Size.Y);
			ipc_info_p2->Path.Size.Lineoffset_CC = tmp_info.line_ofs;

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path2], en: %d, scl_size: (%d, %d), crp_start: (%d, %d), crp_size: (%d, %d), lofs: %d, smp_rate: %x\r\n",
						 info->id, info->ime_path_2.output_en,
						 ipc_info_p2->Path.Size.Y.width, ipc_info_p2->Path.Size.Y.height,
						 ipc_info_p2->Path.Crop.Start.x, ipc_info_p2->Path.Crop.Start.y, ipc_info_p2->Path.Crop.SizeH, ipc_info_p2->Path.Crop.SizeV,
						 ipc_info_p2->Path.Size.Y.line_ofs, ipc_info_p2->sample_rate);
			}
		} else {
			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path2], en: %d\r\n", info->id, info->ime_path_2.output_en);
			}
		}
		rt |= IPL_IME_PATH2;
	}

	if (info->path_id & IPL_IME_PATH3) {
		//p3
		ipc_info_p3->Idx = IPL_CtrlGetHalSetId(info->id);
		IPL_Setime_path_Info(info->id, IPL_IME_PATH3, &info->ime_path_3);
		IPL_Getime_path_Info(info->id, IPL_IME_PATH3, &tmp_ime_info);
		ipc_info_p3->Path.PathEn = tmp_ime_info.output_en;
		ipc_info_p3->Path.DmaOutEn = tmp_ime_info.output_en;
		if (tmp_ime_info.output_en == ENABLE) {
			ipc_info_p3->sample_rate = tmp_ime_info.sample_rate;
			ipc_info_p3->Path.Size.Y.width = tmp_ime_info.scale_size_h;
			ipc_info_p3->Path.Size.Y.height = tmp_ime_info.scale_size_v;
			ipc_info_p3->Path.Size.Y.line_ofs = tmp_ime_info.out_img_lofs;
			ipc_info_p3->Path.Fmt = tmp_ime_info.img_fmt;
			ipc_info_p3->Path.Crop.Start.x = tmp_ime_info.out_crop_start_x;
			ipc_info_p3->Path.Crop.Start.y = tmp_ime_info.out_crop_start_y;
			ipc_info_p3->Path.Crop.SizeH = tmp_ime_info.out_crop_size_h;
			ipc_info_p3->Path.Crop.SizeV = tmp_ime_info.out_crop_size_v;
			tmp_info = IPL_UTI_Y_INFO_CONV2(ipc_info_p3->Path.Fmt, ipc_info_p3->Path.Size.Y);
			ipc_info_p3->Path.Size.Lineoffset_CC = tmp_info.line_ofs;

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path3], en: %d, scl_size: (%d, %d), crp_start: (%d, %d), crp_size: (%d, %d), lofs: %d, smp_rate: %x\r\n",
						 info->id, info->ime_path_3.output_en,
						 ipc_info_p3->Path.Size.Y.width, ipc_info_p3->Path.Size.Y.height,
						 ipc_info_p3->Path.Crop.Start.x, ipc_info_p3->Path.Crop.Start.y, ipc_info_p3->Path.Crop.SizeH, ipc_info_p3->Path.Crop.SizeV,
						 ipc_info_p3->Path.Size.Y.line_ofs, ipc_info_p3->sample_rate);
			}
		} else {
			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path3], en: %d\r\n", info->id, info->ime_path_3.output_en);
			}
		}
		rt |= IPL_IME_PATH3;
	}

	if (info->path_id & IPL_IME_PATH4) {
		//p4
		ipc_info_p4->Idx = IPL_CtrlGetHalSetId(info->id);
		IPL_Setime_path_Info(info->id, IPL_IME_PATH4, &info->ime_path_4);
		IPL_Getime_path_Info(info->id, IPL_IME_PATH4, &tmp_ime_info);
		ipc_info_p4->Path.PathEn = tmp_ime_info.output_en;
		ipc_info_p4->Path.DmaOutEn = tmp_ime_info.output_en;
		if (tmp_ime_info.output_en == ENABLE) {
			ipc_info_p4->sample_rate = tmp_ime_info.sample_rate;
			ipc_info_p4->Path.Size.Y.width = tmp_ime_info.scale_size_h;
			ipc_info_p4->Path.Size.Y.height = tmp_ime_info.scale_size_v;
			ipc_info_p4->Path.Size.Y.line_ofs = tmp_ime_info.out_img_lofs;
			ipc_info_p4->Path.Fmt = tmp_ime_info.img_fmt;
			ipc_info_p4->Path.Crop.Start.x = tmp_ime_info.out_crop_start_x;
			ipc_info_p4->Path.Crop.Start.y = tmp_ime_info.out_crop_start_y;
			ipc_info_p4->Path.Crop.SizeH = tmp_ime_info.out_crop_size_h;
			ipc_info_p4->Path.Crop.SizeV = tmp_ime_info.out_crop_size_v;
			tmp_info = IPL_UTI_Y_INFO_CONV2(ipc_info_p4->Path.Fmt, ipc_info_p4->Path.Size.Y);
			ipc_info_p4->Path.Size.Lineoffset_CC = tmp_info.line_ofs;

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path4], en: %d, scl_size: (%d, %d), crp_start: (%d, %d), crp_size: (%d, %d), lofs: %d, smp_rate: %x\r\n",
						 info->id, info->ime_path_4.output_en,
						 ipc_info_p4->Path.Size.Y.width, ipc_info_p4->Path.Size.Y.height,
						 ipc_info_p4->Path.Crop.Start.x, ipc_info_p4->Path.Crop.Start.y, ipc_info_p4->Path.Crop.SizeH, ipc_info_p4->Path.Crop.SizeV,
						 ipc_info_p4->Path.Size.Y.line_ofs, ipc_info_p4->sample_rate);
			}
		} else {
			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path4], en: %d\r\n", info->id, info->ime_path_4.output_en);
			}
		}
		rt |= IPL_IME_PATH4;
	}

	if (info->path_id & IPL_IME_PATH5) {
		//p5
		ipc_info_p5->Idx = IPL_CtrlGetHalSetId(info->id);
		IPL_Setime_path_Info(info->id, IPL_IME_PATH5, &info->ime_path_5);
		IPL_Getime_path_Info(info->id, IPL_IME_PATH5, &tmp_ime_info);
		ipc_info_p5->Path.PathEn = tmp_ime_info.output_en;
		ipc_info_p5->Path.DmaOutEn = tmp_ime_info.output_en;
		if (tmp_ime_info.output_en == ENABLE) {
			ipc_info_p5->sample_rate = tmp_ime_info.sample_rate;
			ipc_info_p5->Path.Size.Y.width = tmp_ime_info.scale_size_h;
			ipc_info_p5->Path.Size.Y.height = tmp_ime_info.scale_size_v;
			ipc_info_p5->Path.Size.Y.line_ofs = tmp_ime_info.out_img_lofs;
			ipc_info_p5->Path.Fmt = tmp_ime_info.img_fmt;
			ipc_info_p5->Path.Crop.Start.x = tmp_ime_info.out_crop_start_x;
			ipc_info_p5->Path.Crop.Start.y = tmp_ime_info.out_crop_start_y;
			ipc_info_p5->Path.Crop.SizeH = tmp_ime_info.out_crop_size_h;
			ipc_info_p5->Path.Crop.SizeV = tmp_ime_info.out_crop_size_v;
			tmp_info = IPL_UTI_Y_INFO_CONV2(ipc_info_p5->Path.Fmt, ipc_info_p5->Path.Size.Y);
			ipc_info_p5->Path.Size.Lineoffset_CC = tmp_info.line_ofs;

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path5], en: %d, scl_size: (%d, %d), crp_start: (%d, %d), crp_size: (%d, %d), lofs: %d, smp_rate: %x\r\n",
						 info->id, info->ime_path_5.output_en,
						 ipc_info_p5->Path.Size.Y.width, ipc_info_p5->Path.Size.Y.height,
						 ipc_info_p5->Path.Crop.Start.x, ipc_info_p5->Path.Crop.Start.y, ipc_info_p5->Path.Crop.SizeH, ipc_info_p5->Path.Crop.SizeV,
						 ipc_info_p5->Path.Size.Y.line_ofs, ipc_info_p5->sample_rate);
			}
		} else {
			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("\r\nIPL_RT: Id: %d[Path5], en: %d\r\n", info->id, info->ime_path_5.output_en);
			}
		}
		rt |= IPL_IME_PATH5;
	}

	return rt;
}

void IPL_SetVAFCB(IPL_SET_VA_DATA *info, IPL_HAL_PARASET *hal_para_idx, SIE_Dma_Out *out)
{
	UINT32 buf_size = 0, addr = 0;

	*hal_para_idx = IPL_CtrlGetHalSetId(info->id);

	if (IPL_CtrlGetInfor(info->id, IPLCTRL_SIE_CH3_EN) == ENABLE && IPL_CtrlGetInfor(info->id, IPLCTRL_SIE_CH3_SRC) == CH3_VACC) {
		IPL_BufGet(info->id, IPLBUF_SIE_CH3_1, &out->PPB_Addr, &buf_size);
		IPL_BufGet(info->id, IPLBUF_SIE_CH3_2, &addr, &buf_size);
		out->PPB_BufNum = IPL_CtrlGetInfor(info->id, IPLCTRL_SIE_CH3_BUFNUM);
	} else if (IPL_CtrlGetInfor(info->id, IPLCTRL_SIE_CH4_EN) == ENABLE && IPL_CtrlGetInfor(info->id, IPLCTRL_SIE_CH4_SRC) == CH4_VACC) {
		IPL_BufGet(info->id, IPLBUF_SIE_CH4_1, &out->PPB_Addr, &buf_size);
		IPL_BufGet(info->id, IPLBUF_SIE_CH4_2, &addr, &buf_size);
		out->PPB_BufNum = IPL_CtrlGetInfor(info->id, IPLCTRL_SIE_CH4_BUFNUM);
	}
	out->PPB_FrmOfs = addr - out->PPB_Addr;

	out->HFlip = FALSE;
	out->VFlip = FALSE;
}

void IPL_GetIMEHalIdxFCB(IPL_IME_INFOR *ime_info, IPL_HAL_PARASET *hal_para_idx)
{
	*hal_para_idx = IPL_CtrlGetHalSetId(ime_info->id);
}

#if 0
#endif
static BOOL IPL_ChkSupportLibExt(UINT32 id)
{
	return TRUE;
#if 0
	UINT32 i;
	CHAR *verify_name[] = {
		"IPL_SAMPLE_FF",
		"IPL_DVCAM_SAMPLE_FF",
		""
	};

	CHAR *input_name = ipl_ext_lib_get_name(id);

	i = 0;
	while (strcmp(verify_name[i], "")) {
		if (strcmp(verify_name[i], input_name) == 0) {
			return TRUE;
		}
		i += 1;
	}
	return FALSE;
#endif
}


/******************************************************************************/
/* auto select sensor mode (only for ipl ext sample)                          */
/******************************************************************************/
#define MODE_RAT_BASE 100
typedef struct {
	UINT32 fps_rat;
	UINT32 img_rat;
	UINT32 fov_rat;
	UINT32 mode;
} MODE_RAT_INFO;

static int IPL_CompImgRatio(const void *a, const void *b)
{
	if (((MODE_RAT_INFO *)a)->img_rat > ((MODE_RAT_INFO *)b)->img_rat) {
		return -1;
	} else if (((MODE_RAT_INFO *)a)->img_rat < ((MODE_RAT_INFO *)b)->img_rat) {
		return 1;
	}
	return 0;
}

static int IPL_CompFpsRatio(const void *a, const void *b)
{
	if (((MODE_RAT_INFO *)a)->fps_rat > ((MODE_RAT_INFO *)b)->fps_rat) {
		return -1;
	} else if (((MODE_RAT_INFO *)a)->fps_rat < ((MODE_RAT_INFO *)b)->fps_rat) {
		return 1;
	}
	return 0;
}

static int IPL_CompfovRatio(const void *a, const void *b)
{
	if (((MODE_RAT_INFO *)a)->fov_rat > ((MODE_RAT_INFO *)b)->fov_rat) {
		return -1;
	} else if (((MODE_RAT_INFO *)a)->fov_rat < ((MODE_RAT_INFO *)b)->fov_rat) {
		return 1;
	}
	return 0;
}

typedef enum {
	SEARCH_IMG_FIT_FPS_FIT = 0,
	SEARCH_IMG_FIT_FPS_MAX,
	SEARCH_IMG_MAX_FPS_FIT,
	SEARCH_IMG_MAX_FPS_MAX,
} MODE_SEARCH_METHOD;

static UINT32 IPL_SearchSensorModeMethod(MODE_RAT_INFO *mode_ratio_tab, UINT32 tab_max_idx, MODE_SEARCH_METHOD search_method)
{
	UINT32 i;
	UINT32 rt_mode = SENSOR_MODE_UNKNOWN;
	MODE_RAT_INFO mode_ratio_info[tab_max_idx];

	memcpy((void *)mode_ratio_info, (void *)mode_ratio_tab, (sizeof(MODE_RAT_INFO) * tab_max_idx));

	INT32 fps_meet_last_idx = -1, img_meet_last_idx = -1;
	//sort by fps ratio, big -> small
	qsort((void *)mode_ratio_info, tab_max_idx, sizeof(MODE_RAT_INFO), IPL_CompFpsRatio);

	//find last bigger than ratio_base fps idx
	for (i = 0; i < tab_max_idx; i ++) {

		if (b_ipl_ctrl_dbg_en) {
			DBG_DUMP("idx(%d) mode = %d, fps ratio = %d, size ratio = %d\r\n", i, mode_ratio_info[i].mode,
					 mode_ratio_info[i].fps_rat,
					 mode_ratio_info[i].img_rat);
		}
		if (mode_ratio_info[i].fps_rat >= MODE_RAT_BASE) {
			fps_meet_last_idx = i;  //nth element of array
		}
	}
	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("fps meet last idx = %d\r\n", fps_meet_last_idx);
	}

	//base on fps ratio, search image ratio
	if (fps_meet_last_idx > 0) {

		UINT32 fps_match_cnt = 0;

		//find match fps ratio count
		for (i = 0; i < tab_max_idx; i ++) {
			if (mode_ratio_info[i].fps_rat >= mode_ratio_info[fps_meet_last_idx].fps_rat) {
				fps_match_cnt += 1;
			}
		}

		if (b_ipl_ctrl_dbg_en) {
			DBG_DUMP("fps_match_cnt = %d\r\n", fps_match_cnt);
		}
		//copy match fps_fit data
		MODE_RAT_INFO mode_ratio_info_match_fps[fps_match_cnt];
		UINT32 src_addr;
		src_addr = (UINT32)&mode_ratio_info[fps_meet_last_idx];
		src_addr -= ((fps_match_cnt - 1) * sizeof(MODE_RAT_INFO));
		memcpy((void *)&mode_ratio_info_match_fps[0], (void *)src_addr, (sizeof(MODE_RAT_INFO) * fps_match_cnt));

		//base on match fps ratio, sort by image ratio, big -> small
		if (fps_match_cnt > 1) {
			qsort(mode_ratio_info_match_fps, fps_match_cnt, sizeof(MODE_RAT_INFO), IPL_CompImgRatio);
		}

		//find last bigger than ratio_base img idx
		for (i = 0; i < fps_match_cnt; i ++) {

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("idx(%d) mode = %d, fps ratio = %d, size ratio = %d\r\n", i, mode_ratio_info_match_fps[i].mode,
						 mode_ratio_info_match_fps[i].fps_rat,
						 mode_ratio_info_match_fps[i].img_rat);
			}

			if (mode_ratio_info_match_fps[i].img_rat >= MODE_RAT_BASE) {
				img_meet_last_idx = i;
			}
		}

		if (b_ipl_ctrl_dbg_en) {
			DBG_DUMP("img meet last idx = %d\r\n", img_meet_last_idx);
		}

		if (img_meet_last_idx > 0) {
			UINT32 img_match_cnt = 0;

			//find match fps ratio count
			for (i = 0; i < fps_match_cnt; i ++) {
				if (mode_ratio_info_match_fps[i].img_rat >= mode_ratio_info_match_fps[img_meet_last_idx].img_rat) {
					img_match_cnt += 1;
				}
			}

			if (b_ipl_ctrl_dbg_en) {
				DBG_DUMP("img_match_cnt = %d\r\n", img_match_cnt);
			}
			//copy match data
			MODE_RAT_INFO mode_ratio_info_match_img[img_match_cnt];
			src_addr = (UINT32)&mode_ratio_info_match_fps[img_meet_last_idx];
			src_addr -= ((img_match_cnt - 1) * sizeof(MODE_RAT_INFO));
			memcpy((void *)&mode_ratio_info_match_img[0], (void *)src_addr, (sizeof(MODE_RAT_INFO) * img_match_cnt));

			//base on match img ratio, sort by fov , big -> small
			if (img_match_cnt > 1) {
				qsort(mode_ratio_info_match_img, img_match_cnt, sizeof(MODE_RAT_INFO), IPL_CompfovRatio);
			}

			if (b_ipl_ctrl_dbg_en) {
				for (i = 0; i < img_match_cnt; i++) {
					DBG_DUMP("idx(%d) mode = %d, fps ratio = %d, size ratio = %d, fov ratio = %d\r\n", i, mode_ratio_info_match_img[i].mode,
							 mode_ratio_info_match_img[i].fps_rat,
							 mode_ratio_info_match_img[i].img_rat, mode_ratio_info_match_img[i].fov_rat);
				}
				DBG_DUMP("search mode method = %d \r\n", search_method);
			}
			UINT32 match_mode_idx = 0;
			//choose mode by search method
			if (search_method == SEARCH_IMG_FIT_FPS_FIT || search_method == SEARCH_IMG_FIT_FPS_MAX) {
				rt_mode = mode_ratio_info_match_img[match_mode_idx].mode;
				for (i = 1; i < img_match_cnt; i++) {

					if (mode_ratio_info_match_img[i].fov_rat == mode_ratio_info_match_img[match_mode_idx].fov_rat) {
						//same fov,choose smaller img_rat
						if (mode_ratio_info_match_img[i].img_rat < mode_ratio_info_match_img[match_mode_idx].img_rat) {
							rt_mode = mode_ratio_info_match_img[i].mode;
							match_mode_idx = i;
						} else if (mode_ratio_info_match_img[i].img_rat == mode_ratio_info_match_img[match_mode_idx].img_rat) {
							if (search_method == SEARCH_IMG_FIT_FPS_FIT) {
								//same fov,same img_rat,choose smaller fps_rat
								if (mode_ratio_info_match_img[i].fps_rat < mode_ratio_info_match_img[match_mode_idx].fps_rat) {
									rt_mode = mode_ratio_info_match_img[i].mode;
									match_mode_idx = i;
								}
							} else {
								//same fov,same img_rat,choose max fps_rat
								if (mode_ratio_info_match_img[i].fps_rat > mode_ratio_info_match_img[match_mode_idx].fps_rat) {
									rt_mode = mode_ratio_info_match_img[i].mode;
									match_mode_idx = i;
								}
							}
						}
					}
				}
			} else if (search_method == SEARCH_IMG_MAX_FPS_FIT || search_method == SEARCH_IMG_MAX_FPS_MAX) {
				rt_mode = mode_ratio_info_match_img[match_mode_idx].mode;
				for (i = 1; i < img_match_cnt; i++) {
					if (mode_ratio_info_match_img[i].fov_rat == mode_ratio_info_match_img[match_mode_idx].fov_rat) {
						//same fov,choose max img_rat
						if (mode_ratio_info_match_img[i].img_rat > mode_ratio_info_match_img[match_mode_idx].img_rat) {
							rt_mode = mode_ratio_info_match_img[i].mode;
							match_mode_idx = i;
						} else if (mode_ratio_info_match_img[i].img_rat == mode_ratio_info_match_img[match_mode_idx].img_rat) {
							if (search_method == SEARCH_IMG_MAX_FPS_FIT) {
								//same fov,same img_rat,choose smaller fps_rat
								if (mode_ratio_info_match_img[i].fps_rat < mode_ratio_info_match_img[match_mode_idx].fps_rat) {
									rt_mode = mode_ratio_info_match_img[i].mode;
									match_mode_idx = i;
								}
							} else {
								//same fov,same img_rat,choose max fps_rat
								if (mode_ratio_info_match_img[i].fps_rat > mode_ratio_info_match_img[match_mode_idx].fps_rat) {
									rt_mode = mode_ratio_info_match_img[i].mode;
									match_mode_idx = i;
								}
							}
						}
					}
				}
			} else {
				DBG_ERR("unknown search mode type = %d \r\n", search_method);
			}

		} else {
			rt_mode = mode_ratio_info_match_fps[0].mode;
		}
	} else {
		rt_mode = mode_ratio_info[0].mode;
	}

	return rt_mode;
}


UINT32 IPL_ChkSensorMode(IPL_PROC_ID id, IPL_MODE mode)
{
	UINT32 max_mode_num = 1, i, width, height, ratio_tab_cnt;
	UINT32 img_size_h, img_size_v,img_size_h_pad = 0, img_size_v_pad = 0;
	UINT32 img_fps_src, img_fps_div, sen_fps_src, sen_fps_div, ui_frm_num;
	UINT32 sensor_ratio, output_ratio;
	UINT32 sensor_crop_tatio_h, sensor_crop_tatio_v;
	SEN_STATUS_PARAM sen_param;
	UINT32 rt_mode;
	ER rt = E_OK;

	if (IPL_ChkSupportLibExt(id) == FALSE) {
		return SENSOR_MODE_UNKNOWN;
	}

	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_MAX_MODE, (void *)&max_mode_num);
	if (rt != E_OK) {
		DBG_ERR("Get Sensor Maximum Mode Fail, id:%d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID));
		return E_SYS;
	}

	MODE_RAT_INFO mode_ratio_info[max_mode_num];

	img_fps_src = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS) & 0xffff0000) >> 16;
	img_fps_div = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS) & 0x0000ffff);
	if (img_fps_div == 0 || img_fps_src == 0) {
		img_fps_src = 30;
		img_fps_div = 1;
		DBG_ERR("IPL_SEL_VIDEO_FPS %d Error\r\n", IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_FPS));
	}

	ui_frm_num = IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_SHDR_FRAME_NUM);
	img_size_h = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16;
	img_size_v = (IPL_AlgGetUIInfo(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPL_SEL_VIDEO_SIZE) & 0x0000ffff);
	if (img_size_h == 0 || img_size_v == 0) {
		DBG_ERR("IPL_ID_%d, IPL_SEL_VIDEO_SIZE Error, H, V\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID) + 1, img_size_h, img_size_v);
		img_size_h = 1;
		img_size_v = 1;
	}

	if ((IPL_CtrlGetInfor(id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_SQUARE_PAD) && (IPL_CtrlGetInfor(id, IPLCTRL_FUNC_EN) & IPL_FUNC_IME_SQUARE)) {
		img_size_h_pad = IPL_SQUARE_PATCH_W_PADDING;
		img_size_v_pad = IPL_SQUARE_PATCH_H_PADDING;
	}

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("ui size = %d %d, fps = %d/%d, h/v pad: %d/%d\r\n", img_size_h, img_size_v, img_fps_src, img_fps_div, img_size_h_pad, img_size_v_pad);
	}

	//scan mode information & convert ratio
	ratio_tab_cnt = 0;
	for (i = 0; i < max_mode_num; i ++) {
		sen_param.sensor_mode = (i + 1);
		rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_param);
		if (rt != E_OK || sen_param.sensor_info.mode == NULL) {
			DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), (i + 1));
			return SENSOR_MODE_UNKNOWN;
		}

		if (SENSOR_MODE_STAGGER_HDR == sen_param.sensor_info.mode->mode_type) {
			if ((!(IPL_CtrlGetInfor(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPLCTRL_FUNC_EN) & IPL_FUNC_SHDR)) || (ui_frm_num != sen_param.sensor_info.mode->multi_frame_info->total_frm)) {
				continue;
			}
		} else {
			if (IPL_CtrlGetInfor(IPL_CtrlGetInfor(id, IPLCTRL_MF_BASE_ID), IPLCTRL_FUNC_EN) & IPL_FUNC_SHDR) {
				continue;
			}
		}
		sensor_ratio = sen_param.sensor_info.mode->ratio.ratio_h_v;
		output_ratio = IPL_UTI_CONV2_UINT32(img_size_h, img_size_v);
		width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(output_ratio, sensor_ratio, sen_param.sensor_info.mode->valid_width, 4);
		height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(output_ratio, sensor_ratio, sen_param.sensor_info.mode->valid_height, 4);
		sensor_crop_tatio_h = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(output_ratio, sensor_ratio, sen_param.sensor_info.mode->ratio.crop_ratio_h, 4);
		sensor_crop_tatio_v = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(output_ratio, sensor_ratio, sen_param.sensor_info.mode->ratio.crop_ratio_v, 4);

		if (b_ipl_ctrl_dbg_en) {
			DBG_DUMP("sensor size = %d %d, fps = %d\r\n", width, height, sen_param.sensor_info.mode->frame_rate);
		}

		sen_fps_src = (sen_param.sensor_info.mode->frame_rate & 0xffff0000) >> 16;
		sen_fps_div = (sen_param.sensor_info.mode->frame_rate & 0x0000ffff);
		if (sen_fps_div == 0 || sen_fps_src == 0) {
			sen_fps_src = 30;
			sen_fps_div = 1;
			DBG_ERR("Sensor fps Error, Id: %d, fps: %d\r\n", id, sen_param.sensor_info.mode->frame_rate);
		}
		mode_ratio_info[ratio_tab_cnt].fps_rat = (((sen_fps_src * MODE_RAT_BASE) / sen_fps_div) * MODE_RAT_BASE) / ((img_fps_src * MODE_RAT_BASE) / img_fps_div);
		mode_ratio_info[ratio_tab_cnt].img_rat = (width * height * MODE_RAT_BASE) / (img_size_h + img_size_h_pad) / (img_size_v + img_size_v_pad);
		mode_ratio_info[ratio_tab_cnt].fov_rat = sensor_crop_tatio_h * sensor_crop_tatio_v;
		mode_ratio_info[ratio_tab_cnt].mode = (i + 1);
		if (b_ipl_ctrl_dbg_en) {
			DBG_DUMP("mode(%d) fps ratio=%d, size ratio=%d, fov ratioH = %d,fov ratioV = %d\r\n", mode_ratio_info[ratio_tab_cnt].mode,
					 mode_ratio_info[ratio_tab_cnt].fps_rat,
					 mode_ratio_info[ratio_tab_cnt].img_rat, sensor_crop_tatio_h, sensor_crop_tatio_v);
		}
		ratio_tab_cnt += 1;
	}

	if (ratio_tab_cnt == 0) {
		DBG_ERR("No sensor mode could be used\r\n");
		DBG_DUMP("ui size = %d %d, fps = %d/%d, frm_num = %d\r\n", img_size_h, img_size_v, img_fps_src, img_fps_div, ui_frm_num);
		return E_SYS;
	}

	if (mode == IPL_MODE_CAP) {
		if (ratio_tab_cnt > 1) {
			rt_mode = IPL_SearchSensorModeMethod(mode_ratio_info, ratio_tab_cnt, SEARCH_IMG_MAX_FPS_FIT);
		} else {
			rt_mode = mode_ratio_info[ratio_tab_cnt - 1].mode;
		}
		IPL_AlgSetUIInfo(id, IPL_SEL_CAPSENMODE, rt_mode);
	} else {
		if (ratio_tab_cnt > 1) {
			rt_mode = IPL_SearchSensorModeMethod(mode_ratio_info, ratio_tab_cnt, SEARCH_IMG_FIT_FPS_FIT);
		} else {
			rt_mode = mode_ratio_info[ratio_tab_cnt - 1].mode;
		}
		IPL_AlgSetUIInfo(id, IPL_SEL_PRVSENMODE, rt_mode);
	}
	IPL_CtrlSetInfor(id, IPLCTRL_SEN_MODE, rt_mode);

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("rt_mode = %d\r\n", rt_mode);
	}

	return rt_mode;
}

static UINT32 *GenSenModeTbl(UINT32 Id, SENSOR_INFO *SenInfo, UINT32 *DzMaxidx, IPL_DZOOM_SEL_INFO *DzInfo)
{
#define MAX_FPS (30)
#define MAX_W (3840)
#define MAX_H (2160)
#define MAX_DATA (MAX_H * MAX_W)
	UINT32 cur_max_width = 0;
	UINT32 cur_max_height = 0;
	UINT32 width = 0, height = 0;
	UINT32 new_width = 0, new_height = 0;
	UINT32 new_width2 = 0, new_height2 = 0;
	double ratio;
	UINT32 *Ptr = NULL;
	UINT32 dz_cur_idx = 0;
	UINT32 ipl_crop_oft_h = 0;
	double dz_ratio = 0;
	UINT32 dz_factor;
	IPL_DZOOM_SEL_INFO dz_info = {SENSOR_MODE_1, 61, 8, 0, 0, IPL_CROP};
	UINT32(*vdozoom_infor_def_table)[1][DZOOM_ITEM_MAX];
	UINT32 eis_ratio;
	UINT32 func_en = IPL_CtrlGetInfor(Id, IPLCTRL_FUNC_EN);
	UINT32 ui_size_h = 0, ui_size_v = 0;

	/*
	  1.if normal mode and 3DNR is opened,        sie crop width align to 4x, sie out width align to 4x,  ipl in width align to 16x
	  2.if CCIR no split mode and 3DNR is opened, sie crop width align to 16x,sie out width align to 16x, ipl in width align to 16x
	  3.if CCIR split mode and 3DNR is opened,    sie crop width align to 32x,sie out width align to 16x, ipl in width align to 16x
	*/
	UINT32 sie_crop_w_ali = 4, sie_out_w_ali = 4; // alignment number

	// copy user parameters
	if (DzInfo != NULL) {
		dz_info = *DzInfo;
		if (b_ipl_ctrl_dbg_en) {
			DBG_DUMP("gen Tle %d %d %d %d %d %d\r\n", DzInfo->mode, DzInfo->max_index, DzInfo->factor, DzInfo->img_size_h, DzInfo->img_size_v, DzInfo->crop_p);
		}
	}
	dz_info.max_index = dz_info.max_index -1;
	if (dz_info.max_index == 0) {
		dz_info.max_index = 1;
		DBG_WRN("dzoom index must > 0\r\n");
	}

	//redefine valid_size in ccir mode
	if (SenInfo->mode->dvi != NULL) {
		// adjust valid_width when HD mode(16bit), align to 8bit
		if (SenInfo->mode->dvi->mode != SENSOR_DVI_MODE_SD) {
			width = SenInfo->mode->valid_width << 1;
		} else {
			width = SenInfo->mode->valid_width;

		}
		if(SenInfo->mode->dvi->out_split_en) {
			sie_crop_w_ali = 32;
			sie_out_w_ali = 16;
		} else {
			sie_crop_w_ali = 16;
			sie_out_w_ali = 16;
		}
	} else {
		width = SenInfo->mode->valid_width;
	}
	height = SenInfo->mode->valid_height;

	// size =0, as default
	if(dz_info.img_size_h*dz_info.img_size_v == 0) {
		dz_info.img_size_h = width;
		dz_info.img_size_v = height;
	}

	//calculate dzoom ratio
	if(dz_info.max_index > 1)
		dz_ratio = pow(dz_info.factor, 1.0 / (double)(dz_info.max_index));
	else
		dz_ratio = 1;
	// get current dzoom index
	dz_cur_idx = IPL_DzoomGetIndex(Id);
	// get auto dzoom buffer
	vdozoom_infor_def_table = (UINT32(*)[1][DZOOM_ITEM_MAX])IPL_DzoomGetAutoAddr(Id);
	if (vdozoom_infor_def_table == 0) {
		DBG_ERR("buffer NULL,DZ auto gen fail %d\r\n", Id);
		return NULL;
	}

	if (dz_cur_idx > dz_info.max_index) {
		DBG_WRN("dzoom idx must <= max idx \r\n");
		dz_cur_idx = dz_info.max_index;
	}

	//using ratio to calculate size
	width = width / (pow(dz_ratio, dz_cur_idx));
	height = height / (pow(dz_ratio, dz_cur_idx));
	// allign width,height for 3DNR function
	width = IPL_ALIGN_ROUNDDOWN(width, sie_crop_w_ali);
	height = IPL_ALIGN_ROUNDDOWN(height, 4);
	//assign capture and sie crop size
	vdozoom_infor_def_table[Id][0][6] = vdozoom_infor_def_table[Id][0][0] = width;
	vdozoom_infor_def_table[Id][0][7] = vdozoom_infor_def_table[Id][0][1] = height;


	// DIS/RSC case,increase sie output size
	eis_ratio = rsc_ext_geteisratio(Id,SEL_EIS_RATIO, IPL_CtrlGetInfor(Id, IPLCTRL_SEN_MODE));
	if(rsc_ext_getenable(Id) == SEL_RSC_ON || IPL_AlgGetUIInfo(Id, IPL_SEL_DISCOMP_IMM) == SEL_DISCOMP_ON){
		if (Id <= RSC_SURPPORT_NUM && rsc_ext_getenable(Id) == SEL_RSC_ON) {
			if(eis_ratio != 0){
				new_width = dz_info.img_size_h *IMG_EISDIS_RATIO_UINT / eis_ratio;
				new_height = dz_info.img_size_v *IMG_EISDIS_RATIO_UINT / eis_ratio;

				//for 3DNR
				if (func_en & IPL_FUNC_IME_SQUARE) {
					if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_SQUARE_PAD) {
						ui_size_h = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16)*IMG_EISDIS_RATIO_UINT / eis_ratio + IPL_SQUARE_PATCH_W_PADDING;
						ui_size_v = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0x0000ffff))*IMG_EISDIS_RATIO_UINT / eis_ratio + IPL_SQUARE_PATCH_H_PADDING;
					} else {
						ui_size_h = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16)*IMG_EISDIS_RATIO_UINT / eis_ratio;
						ui_size_v = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0x0000ffff))*IMG_EISDIS_RATIO_UINT / eis_ratio;
					}
				}
				if (b_ipl_ctrl_dbg_en) {
					DBG_DUMP("eis ratio = %d\r\n",eis_ratio);
				}
			}
		} else if (IPL_AlgGetUIInfo(Id, IPL_SEL_DISCOMP_IMM) == SEL_DISCOMP_ON) {
			new_width = dz_info.img_size_h *IMG_EISDIS_RATIO_UINT / dis_getDisViewRatio();
			new_height = dz_info.img_size_v *IMG_EISDIS_RATIO_UINT / dis_getDisViewRatio();
			//for 3DNR
			if (func_en & IPL_FUNC_IME_SQUARE) {
				if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_SQUARE_PAD) {
					ui_size_h = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16)*IMG_EISDIS_RATIO_UINT / dis_getDisViewRatio() + IPL_SQUARE_PATCH_W_PADDING;
					ui_size_v = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0x0000ffff))*IMG_EISDIS_RATIO_UINT / dis_getDisViewRatio() + IPL_SQUARE_PATCH_H_PADDING;
				} else {
					ui_size_h = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16)*IMG_EISDIS_RATIO_UINT / dis_getDisViewRatio();
					ui_size_v = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0x0000ffff))*IMG_EISDIS_RATIO_UINT / dis_getDisViewRatio();
				}
			}
			if (b_ipl_ctrl_dbg_en) {
					DBG_DUMP("dis ratio = %d\r\n",dis_getDisViewRatio());
			}
		} else {
		}

		if(new_width > SenInfo->mode->valid_width ){
		} else {
			dz_info.img_size_h = new_width;
		}

		if (new_height > SenInfo->mode->valid_height){
		} else {
			dz_info.img_size_v = new_height ;
		}

	} else{
		//for 3DNR
		if (func_en & IPL_FUNC_IME_SQUARE) {
			if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_SQUARE_PAD) {
				ui_size_h = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16) + IPL_SQUARE_PATCH_W_PADDING;
				ui_size_v = (IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0x0000ffff) + IPL_SQUARE_PATCH_H_PADDING;
			} else {
				ui_size_h = ((IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0xffff0000) >> 16);
				ui_size_v = (IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE) & 0x0000ffff);
			}
		}
	}

	//in normal mode,sie output will be clamped to ipl input size(user define)
	if(SenInfo->mode->dvi == NULL) {
		//clamp sie output size
		if (width > dz_info.img_size_h) {
			width = dz_info.img_size_h;
		}
		if (height > dz_info.img_size_v) {
			height = dz_info.img_size_v;
		}
	}

	//redefine sie crop size and sie output size when using ipl crop
	if ((dz_info.crop_p == IPL_CROP) && (SenInfo->mode->dvi == NULL)) {
		if (vdozoom_infor_def_table[Id][0][0] < dz_info.img_size_h) {
			ipl_crop_oft_h = dz_info.img_size_h - vdozoom_infor_def_table[Id][0][0];
			vdozoom_infor_def_table[Id][0][0] = dz_info.img_size_h;
		}
		if (vdozoom_infor_def_table[Id][0][1] < dz_info.img_size_v) {
			vdozoom_infor_def_table[Id][0][1] = dz_info.img_size_v;
		}
		width = dz_info.img_size_h;
		height = dz_info.img_size_v;
	}

    if (func_en & IPL_FUNC_IME_SQUARE) {
		if(ui_size_h > SenInfo->mode->valid_width) {
			ui_size_h = SenInfo->mode->valid_width;
		}
		if(ui_size_v > SenInfo->mode->valid_height) {
			ui_size_v = SenInfo->mode->valid_height;
		}

		if(ui_size_h*ui_size_v != 0) {
			width = ui_size_h;
			height = ui_size_v;
		} else {
			DBG_ERR("ui size error (%d %d)\r\n",ui_size_h,ui_size_v);
		}

		if (b_ipl_ctrl_dbg_en) {
			DBG_DUMP("uisize h = %d,v = %d\r\n",ui_size_h,ui_size_v);
		}
    }

	//check bandwith
	ratio = (double)MAX_DATA / (double)(width * height);
	ratio = (ratio * MAX_FPS) * ((SenInfo->mode->frame_rate & 0xffff)) / ((SenInfo->mode->frame_rate & 0xffff0000) >> 16);

	ratio = sqrt(ratio);
	cur_max_width = IPL_ALIGN_ROUNDDOWN((UINT32)((double)width * ratio), 4);
	cur_max_height = IPL_ALIGN_ROUNDDOWN((UINT32)((double)height * ratio), 4);

	if ((width <= cur_max_width) && (height <= cur_max_height)) {

		new_width = width;
		new_height = height;
	} else {
		new_width = 0;
		new_height = 0;
		if (width > cur_max_width) {
			new_width = cur_max_width;
			new_height = height * cur_max_width / width;
			new_height = IPL_ALIGN_ROUNDDOWN(new_height, 4);
		}
		new_width2 = 0;
		new_height2 = 0;
		if (height > cur_max_height) {
			new_height2 = cur_max_height;
			new_width2 = width * cur_max_height / height;
			new_width2 = IPL_ALIGN_ROUNDDOWN(new_width2, 4);
		}

		if ((new_height * new_width) < (new_height2 * new_width2)) {
			new_width = new_width2;
			new_height = new_height2;
		}
	}

	//if CCIR MODE and Split enable,sie out and ipl in should be half
	if (SenInfo->mode->dvi != NULL) {
		if (SenInfo->mode->dvi->out_split_en == TRUE) {
			vdozoom_infor_def_table[Id][0][2] = new_width >> 1;
			vdozoom_infor_def_table[Id][0][4] = new_width >> 1;
		} else {
			vdozoom_infor_def_table[Id][0][2] = new_width;
			vdozoom_infor_def_table[Id][0][4] = new_width;
		}
	} else {
		vdozoom_infor_def_table[Id][0][2] = new_width;
		vdozoom_infor_def_table[Id][0][4] = new_width - ipl_crop_oft_h;
	}
	vdozoom_infor_def_table[Id][0][3] = new_height;

	// allign width,height for 3DNR function
	vdozoom_infor_def_table[Id][0][2] = IPL_ALIGN_ROUNDDOWN(vdozoom_infor_def_table[Id][0][2], sie_out_w_ali);
	vdozoom_infor_def_table[Id][0][3] = IPL_ALIGN_ROUNDDOWN(vdozoom_infor_def_table[Id][0][3], 4);
	vdozoom_infor_def_table[Id][0][4] = IPL_ALIGN_ROUNDDOWN(vdozoom_infor_def_table[Id][0][4], 16);

	if(SenInfo->mode->dvi != NULL) {
		vdozoom_infor_def_table[Id][0][5] = vdozoom_infor_def_table[Id][0][3];
	} else if(func_en & IPL_FUNC_IME_SQUARE) {
		vdozoom_infor_def_table[Id][0][5] = vdozoom_infor_def_table[Id][0][3];
	} else {
		//calculate ipl input height by width and sie output ratio
		vdozoom_infor_def_table[Id][0][5] = (vdozoom_infor_def_table[Id][0][4] * dz_info.img_size_v) / dz_info.img_size_h;
		vdozoom_infor_def_table[Id][0][5] = IPL_ALIGN_ROUNDDOWN(vdozoom_infor_def_table[Id][0][5], 4);
	}

	if (b_ipl_ctrl_dbg_en) {
		DBG_DUMP("%d,%d,%d,%d,%d,%d,%d,%d\r\n", vdozoom_infor_def_table[Id][0][0], vdozoom_infor_def_table[Id][0][1],
				 vdozoom_infor_def_table[Id][0][2], vdozoom_infor_def_table[Id][0][3],
				 vdozoom_infor_def_table[Id][0][4], vdozoom_infor_def_table[Id][0][5],
				 vdozoom_infor_def_table[Id][0][6], vdozoom_infor_def_table[Id][0][7]);
		DBG_DUMP("ratio = %f, idx=%d , max_idx = %d\r\n",dz_ratio,dz_cur_idx,dz_info.max_index);
	}
	dz_factor = pow(dz_ratio, dz_cur_idx)*100;
	IPL_DzoomSetRatio(Id,dz_factor);
	Ptr = (UINT32 *)&vdozoom_infor_def_table[Id][0][0];
	*DzMaxidx = dz_info.max_index;
	return Ptr;
}

UINT32 *ChkSenModeTbl(UINT32 Id, SENSOR_INFO *SenInfo, UINT32 *TabPtr, UINT32 *DzMaxidx, IPL_DZOOM_SEL_INFO *DzInfo)
{
	UINT32 *Ptr = NULL;
	UINT32(*DzTable)[DZOOM_ITEM_MAX] = (UINT32(*)[DZOOM_ITEM_MAX])TabPtr;
	UINT32 width, height;
	IPL_DZOOM_EXT_INFO dzoom_ext_info = {0};
	UINT32 func_en = IPL_CtrlGetInfor(Id, IPLCTRL_FUNC_EN);

	if (IPL_ChkSupportLibExt(Id) == FALSE) {
		return TabPtr;
	}

	Ptr = TabPtr;

	if (Ptr != NULL) {
		if (SenInfo->mode->dvi != NULL) {
			// adjust valid_width when HD mode(16bit), align to 8bit
			if (SenInfo->mode->dvi->mode != SENSOR_DVI_MODE_SD) {
				width = SenInfo->mode->valid_width << 1;
			} else {
				width = SenInfo->mode->valid_width;
			}
		} else {
			width = SenInfo->mode->valid_width;
		}
		height = SenInfo->mode->valid_height;

		if (func_en & IPL_FUNC_IME_SQUARE) {
			if(IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) & IPL_EXT_CTRL_IME_SQUARE_PAD) {
				dzoom_ext_info.ext_func = IPL_DZOOM_EXT_FUNC_IME_SQUARE_PAD;
				IPL_DzoomSetExtInfo(Id,&dzoom_ext_info);
			} else {
				IPL_DzoomSetExtInfo(Id,&dzoom_ext_info);
			}
		} else {
				IPL_DzoomSetExtInfo(Id,&dzoom_ext_info);
		}

		if ((width < DzTable[0][0]) || (height < DzTable[0][1]) || (width < DzTable[0][6]) || (height < DzTable[0][7])) {
			DBG_DUMP("dzoom table(%d, %d)(%d, %d) & sensor valid size(%d %d) overflow\r\n",
					 DzTable[0][0], DzTable[0][1], DzTable[0][6], DzTable[0][7], SenInfo->mode->valid_width, SenInfo->mode->valid_height);

			Ptr = GenSenModeTbl(Id, SenInfo, DzMaxidx, DzInfo);
		}
	} else {
		DBG_IND("auto gen dzoom table\r\n");
		Ptr = GenSenModeTbl(Id, SenInfo, DzMaxidx, DzInfo);
	}
	return Ptr;
}
