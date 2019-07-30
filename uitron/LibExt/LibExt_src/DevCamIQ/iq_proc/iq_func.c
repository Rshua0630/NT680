#include "iq_common.h"
#include "pq_obj.h"

///////////////////////////////////////////////
// Flow Export
///////////////////////////////////////////////
UINT32 iq_get_iso_range(UINT32 Id, IQS_FLOW_MSG MsgID, UINT32 *IsoValue, UINT32 *IsoStart, UINT32 *IsoEnd, UINT32 *IQIdxL, UINT32 *IQIdxH)
{
	UINT32 i, ISO, ExpT, Iris;
	ISOMAP *ISOMap;

	if (iq_check_capture(MsgID) == FALSE) { // movie mode
		AE_GetPrvAEArg(Id, &ISO, &ExpT, &Iris);
	} else {                            // capture mode
		AE_GetCapAEArg(Id, &ISO, &ExpT, &Iris);
	}

	*IsoValue = ISO;

	//**  Find ISO Map
	if (ISO >= (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->ISO) {
		*IQIdxL     = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->Index;
		*IQIdxH     = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->Index;
		*IsoStart   = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->ISO;
		*IsoEnd     = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->ISO;
		return FALSE;
	}
	ISOMap = g_iso_map_tab_addr[Id];

	//**  Find ISO information
	for (i = 1; i < IQS_ISOMAPTAB_NUM; i++) {
		if (ISO < ISOMap[i].ISO) {
			*IQIdxL = ISOMap[i - 1].Index;
			*IQIdxH = ISOMap[i].Index;
			*IsoStart = ISOMap[i - 1].ISO;
			*IsoEnd = ISOMap[i].ISO;
			break;
		}
	}
	return TRUE;
}

UINT32 iq_get_iso_range_adj(UINT32 Id, IQS_FLOW_MSG MsgID, UINT32 *IsoValue, UINT32 *IsoStart, UINT32 *IsoEnd, UINT32 *IQIdxL, UINT32 *IQIdxH)
{
    ER rt = E_OK;
	UINT32 i, ISO, ExpT, Iris;
	ISOMAP *ISOMap;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sensor_status = {0};

	sen_info.mode = NULL;

	if (iq_check_capture(MsgID) == FALSE) { // movie mode
		AE_GetPrvAEArg(Id, &ISO, &ExpT, &Iris);
	} else {                            // capture mode
		AE_GetCapAEArg(Id, &ISO, &ExpT, &Iris);
	}

	if (iq_check_capture(MsgID) == TRUE) {
		sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_CAPSENMODE);
		rt = sensor_get_info((SENSOR_ID)IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_STATUS, (void *)&sensor_status);
		sen_info = sensor_status.sensor_info;
	} else {
		sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_PRVSENMODE);
		rt = sensor_get_info((SENSOR_ID)IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_STATUS, (void *)&sensor_status);
		sen_info = sensor_status.sensor_info;
	}
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("NULL sen_info.mode\r\n");
		return 0;
	}

	if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) &&
        ((IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_OFF) || (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_ON))) {
		//ISO = (ISO * 128) >> 7;
	} else if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) ||
        ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) && (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_WDR))) {
	    UINT32 wdr_str = SceneRender_WDR_GetStrth((SR_WDR_PROCID)Id);
        if (wdr_str <= 128) {
            wdr_str = 128;
        }
		ISO = ISO * wdr_str >> 7;
	}
	*IsoValue = ISO;

	//**  Find ISO Map
	if (ISO >= (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->ISO) {
		*IQIdxL     = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->Index;
		*IQIdxH     = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->Index;
		*IsoStart   = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->ISO;
		*IsoEnd     = (g_iso_map_tab_addr[Id]+(IQS_ISOMAPTAB_NUM - 1))->ISO;
		return FALSE;
	}
	ISOMap = g_iso_map_tab_addr[Id];

	//**  Find ISO information
	for (i = 1; i < IQS_ISOMAPTAB_NUM; i++) {
		if (ISO < ISOMap[i].ISO) {
			*IQIdxL = ISOMap[i - 1].Index;
			*IQIdxH = ISOMap[i].Index;
			*IsoStart = ISOMap[i - 1].ISO;
			*IsoEnd = ISOMap[i].ISO;
			break;
		}
	}
	return TRUE;
}

UINT32 iq_get_iso_idx(UINT32 Id, IQS_FLOW_MSG MsgID)
{
	UINT32 IsoValue = 0, IsoStart = 0, IsoEnd = 0, IQL = 0, IQH = 0, ret;
	ret = iq_get_iso_range(Id, MsgID, &IsoValue, &IsoStart, &IsoEnd, &IQL, &IQH);
	if (ret == FALSE) {
		;//DBG_ERR("Get ISO Index error!!\r\n");
	}
	return IQL;
}

UINT32 iq_get_sensor_idx(UINT32 Id)
{
	return IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID));
}

UINT32 iq_check_capture(IQS_FLOW_MSG MsgID)
{
	if ((MsgID >= IQCB_CAP_SUB_START && MsgID <= IQCB_CAP_SUB_END) ||
		(MsgID >= IQCB_CAP_PRI_START && MsgID <= IQCB_CAP_PRI_END)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

UINT32 iq_get_ob_sub_mode(UINT32 Id)
{
	if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) ||
        ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) && (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_WDR))) {
		return IQS_SIE_OBSUB;
	} else if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) &&
        ((IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_OFF) || (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_ON))) {
		return IQS_SIE_OBSUB;
	} else if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_DEFOG) {
		return IQS_SIE_OBSUB;
	} else {
		return IQS_IFE_OBSUB;
	}
}

UINT32 iq_get_cg_mode(UINT32 Id)
{
	if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) &&
        ((IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_OFF) || (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_ON))) {
		return IQS_RHE_CG;
	} else if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_DEFOG) {
		return IQS_RHE_CG;
	} else if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) ||
        ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) && (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_WDR))) {
		return IQS_IFE_CG;
	} else {
		return IQS_IFE_CG;
	}
}

UINT32 iq_get_ycurve_mode(UINT32 Id)
{
	if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) {
        if(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR_IMM) == SEL_WDR_OFF) {
            return IQS_Y_OFF;
        } else {
    		return IQS_Y_AUTO;
        }
	} else if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) {
        if(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_OFF) {
            return IQS_Y_OFF;
        } else if (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_WDR) {
    		return IQS_Y_AUTO;
        } else {
            SHDR_TEST_PARA test_param = SensorHDR_GetTestMode();
            if(test_param.SHDR_TestMode != 0) {
                return IQS_Y_AUTO;
            }
            if(g_hdr_addr[Id]->uiCtrlMode == SHDR_CTRL_byIQPara) {
                return IQS_Y_MANUAL;
            } else {
                return IQS_Y_AUTO;
            }
        }
	} else {
		return IQS_Y_MANUAL;
	}
}

BOOL iq_get_cfa_en(UINT32 Id, IQS_FLOW_MSG MsgID)
{
    BOOL sim_status = iq_get_sim_status();
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	ER rt = E_OK;
	sen_info.mode = NULL;

    if(sim_status == TRUE) {
        DBG_DUMP("Fixed CFA enable at SIM\r\n");
        return TRUE;
    }

	if (iq_check_capture(MsgID) == TRUE) {
		sensor_status.sensor_mode = IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE);
		rt = sensor_get_info((SENSOR_ID)IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_STATUS, (void *)&sensor_status);
		sen_info = sensor_status.sensor_info;
	} else {
		sensor_status.sensor_mode = IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE);
		rt = sensor_get_info((SENSOR_ID)IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_STATUS, (void *)&sensor_status);
		sen_info = sensor_status.sensor_info;
	}
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("NULL sen_info.mode\r\n");
		return TRUE;
	}

	if (sen_info.mode->stpix == SENSOR_STPIX_Y_ONLY) {
		return FALSE;
	} else {
		return TRUE;
	}
}

void iq_get_ipe_eth_param(IPE_EethParam *ipe_eth_buf)
{
    memcpy(ipe_eth_buf, &g_ipe_eth, sizeof(IPE_EethParam));
}

void iq_get_ref_image_info(UINT32 Id, INT32 SensorMode, IQS_REFIMG_INFO *info)
{
#ifndef IQS_SIMT_ENABLE
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	ER rt = E_OK;
	sen_info.mode = NULL;

	sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_PRVSENMODE);
	rt = sensor_get_info((SENSOR_ID)IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_STATUS, (void *)&sensor_status);
	sen_info = sensor_status.sensor_info;

	info->width = sen_info.width_1x;
	info->height = sen_info.height_1x;
	if (rt == E_OK || sen_info.mode != NULL) {
		info->hratio = sen_info.mode->ratio.ratio_h;
		info->vratio = sen_info.mode->ratio.ratio_v;
	} else {
		DBG_ERR("Get Ratio without sensor mode\r\n");
	}
#else
    //Only for IQSIM
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();

    info->width = AppInfo->IQSimPriInfo.RawInfo.Size.w;
    info->height = AppInfo->IQSimPriInfo.RawInfo.Size.h;

    info->hratio = 1000;
    info->vratio = 1000;
#endif //end #ifndef IQS_SIMT_ENABLE
}

UINT32 iq_get_msnr_time(UINT32 Id)
{
	return IQS_MSNR.uiProcRepNum;
}

UINT32 iq_get_ife2_lca_time(UINT32 Id, IQS_FLOW_MSG MsgID)  //PRV & CAP
{
#ifndef IQS_SIMT_ENABLE
	UINT32 ISO_Value;
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	ISO_Value = iq_get_iso_idx(SenId_Value, MsgID);

    UINT32 LcaTime = 0;
    LcaTime = (g_ife2filter_addr[SenId_Value]+ISO_Value)->RepNum;
	if(LcaTime>10) {
		DBG_ERR("ISO:%d too many times %d, change to 10 times\r\n",ISO_Value,LcaTime);
		LcaTime = 10;
	}
	return LcaTime;
#else
    //Only for IQSIM
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    DBG_ERR("IQS_IFE2Filter.RepNum %d\r\n", AppInfo->IQSimPriInfo.IQS_IFE2_Filter.RepNum);
    return AppInfo->IQSimPriInfo.IQS_IFE2_Filter.RepNum;
#endif //end #ifndef IQS_SIMT_ENABLE
}

UINT32 iq_get_ife2_lca_ratio(UINT32 Id, BOOL IsCap, BOOL GetMaxRatio)
{
#ifndef IQS_SIMT_ENABLE
	//get IQS min chroma adaption ratio in streaming mode
	//for IPL prepare buffer
	UINT32 ISO_Value = 0;
    UINT32 Ratio = 0;
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
    UINT32 aRatio = 0;
    UINT32 i, s = 0, e = 0;

	if (IsCap == TRUE) {
		ISO_Value = iq_get_iso_idx(SenId_Value, IQCB_CAP_PRI_START);

        Ratio = (g_chroma_adapt_addr[SenId_Value]+ISO_Value)->SubRatio;

		if ((Ratio > 0)&&(Ratio < 100)) {
			return Ratio;
		} else {
			DBG_DUMP("IME Ratio is illegal, use default IME SubRatio 25!!!!\r\n");
			return 25;  //default value
		}
	} else if (GetMaxRatio == FALSE) {
		ISO_Value = iq_get_iso_idx(SenId_Value, IQCB_PRV_START);

		Ratio = (g_chroma_adapt_addr[SenId_Value]+ISO_Value)->SubRatio;

		if ((Ratio > 0)&&(Ratio < 100)) {
			return Ratio;
		} else {
			DBG_DUMP("IME Ratio is illegal, use default IME SubRatio 25!!!!\r\n");
			return 25;  //default value
		}
	} else { //GetMaxRatio
		if (SenId_Value < ISP_IQ_MAX_NUM) {
            if ((g_iso_map_tab_addr[SenId_Value] != NULL)&&(g_iso_index_addr[SenId_Value] != NULL)) {
                s = (g_iso_map_tab_addr[SenId_Value])->Index;
                e = (g_iso_index_addr[SenId_Value])->num;
    			//DBG_DUMP("get IME Ratio s=%d, e=%d\r\n",s,e);
                for ( i=s; i<=e; i++ )
                {
                    if ( (g_chroma_adapt_addr[SenId_Value]+i)->SubRatio <= 0 || (g_chroma_adapt_addr[SenId_Value]+i)->SubRatio >= 100) {
                        DBG_ERR("invalidate IME subratio (%d)\r\n", (g_chroma_adapt_addr[SenId_Value]+i)->SubRatio);
                    }
                    if ( aRatio < (g_chroma_adapt_addr[SenId_Value]+i)->SubRatio ) {
                        aRatio = (g_chroma_adapt_addr[SenId_Value]+i)->SubRatio;
                    }
                }
                //DBG_DUMP("^G GetMaxRatio=%d\r\n",aRatio);
    			return aRatio;  //default value
            } else {
                DBG_ERR("IQ param is not init, fixed MaxRatio=25!!!\r\n");
                return 25;
            }
		} else {
			DBG_ERR("wrong Id (%d)\r\n", SenId_Value);
			return 100;
		}
	}
	return 100;
#else
    //Only for IQSIM
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    DBG_ERR("IQS_ChromaAdapt.SubRatio %d\r\n", AppInfo->IQSimPriInfo.IQS_IME_ChromaAdapt.SubRatio);
    return AppInfo->IQSimPriInfo.IQS_IME_ChromaAdapt.SubRatio;
#endif //end #ifndef IQS_SIMT_ENABLE
}

UINT32 iq_check_ife2_lca(UINT32 Id, BOOL IsCap)
{
#ifndef IQS_SIMT_ENABLE
	UINT32 ISO_Value = 0;
    UINT32 LcaTime = 0;
    UINT32 Ratio = 0;
	UINT32 SenId_Value = iq_get_sensor_idx(Id);

	if (IsCap == TRUE) {
		ISO_Value = iq_get_iso_idx(SenId_Value, IQCB_CAP_PRI_START);
	} else {
		ISO_Value = iq_get_iso_idx(SenId_Value, IQCB_PRV_START);
	}

    LcaTime = (g_ife2filter_addr[SenId_Value]+ISO_Value)->RepNum;
	if (LcaTime == 0) {
		return DISABLE;
	}

	Ratio = (g_chroma_adapt_addr[SenId_Value]+ISO_Value)->SubRatio;
	if (Ratio == 0) {
		DBG_ERR("invalidate IME subratio (%d)\r\n", Ratio);
		return DISABLE;
	}

    if(g_e_ctrl_addr[SenId_Value]->lca.enable == LCA_OFF) {
		return DISABLE;
    }
	return ENABLE;
#else
    //Only for IQSIM
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    if ( IsCap == TRUE )
    {
        if ( AppInfo->IQSimPriInfo.IQS_IME_ChromaAdapt.SubRatio == 0 )
            return DISABLE;
        if ( AppInfo->IQSimPriInfo.IQS_IFE2_Filter.RepNum == 0 )
            return DISABLE;
        return ENABLE;
    }
    else
    {
        return ENABLE;
    }
    return DISABLE;
#endif //end #ifndef IQS_SIMT_ENABLE
}

void iq_get_dce_param(UINT32 Id, UINT32 MsgID, UINT32 ZoomIdx, DCE_Distortion *pDistortion, DIST *pDce_Dist, Fact_Norm *pRadious)
{
	UINT32 ImgWidth = 0, ImgHeight = 0;

	if (iq_check_capture(MsgID)) { //capture
		ImgWidth  = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
		ImgHeight = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);
	} else {                    //preview
		ImgWidth  = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H);
		ImgHeight = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V);
	}

	pDistortion->Center.x = ImgWidth >> 1;
	pDistortion->Center.y = ImgHeight >> 1;
	pDistortion->Fov = g_fov_buf[ZoomIdx];
	pDistortion->Enh = g_ldc_enh_buf[ZoomIdx];

	pDistortion->ColorAber = g_cac_buf[ZoomIdx];
	pDistortion->ColorAber.CACGain.Fact_G = 4096;   // 4096 = 1x
	pDistortion->ColorAber.CACGain.Fact_R = (UINT32)IQ_CLAMP(g_cac_buf[Id].CACGain.Fact_R, 4055, 4137);
	pDistortion->ColorAber.CACGain.Fact_B = (UINT32)IQ_CLAMP(g_cac_buf[Id].CACGain.Fact_B, 4055, 4137);

	pDistortion->UV_FilterEn = FALSE;

	pDistortion->GeoLUT.pGGeoLut = g_ldc_g_lut_buf[ZoomIdx];
    //DBG_DUMP("GDC table:%d,%d,,,%d\r\n",g_ldc_g_lut_buf[ZoomIdx][0],g_ldc_g_lut_buf[ZoomIdx][1],g_ldc_g_lut_buf[ZoomIdx][64]);
	pDistortion->GeoLUT.pRGeoLut = g_ldc_r_lut_buf[ZoomIdx];
	pDistortion->GeoLUT.pBGeoLut = g_ldc_b_lut_buf[ZoomIdx];

	*pDce_Dist = g_dist_buf[ZoomIdx];
	*pRadious = g_radious_buf[ZoomIdx];
}

UINT32* iq_get_gamma_lut(UINT32 id)
{
    return g_gamma_buf[id];
}

UINT32 iq_get_gamma_value(UINT32 Id, UINT32 Data12Bit)
{
	INT32 idx, YLum, LumIdx, LumIdx1, data = Data12Bit;
	UINT32 *GammaLUT_128Tab;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	ER rt = E_OK;
	sen_info.mode = NULL;

	sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_PRVSENMODE);
	rt = sensor_get_info((SENSOR_ID)IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_STATUS, (void *)&sensor_status);
	sen_info = sensor_status.sensor_info;
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("NULL sen_info.mode\r\n");
		return 0;
	}

	if (SenId_Value > 1) {
		DBG_ERR("%d %d\r\n", SenId_Value, Data12Bit);
		return Data12Bit;
	}
	if (sen_info.mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		if (data < 1) {
			YLum = 0;
		} else {
			YLum = data >> 2;
		}

		if (YLum > 1023) {
			YLum = 1023;
		}

		return YLum;
	} else {
		GammaLUT_128Tab =  g_gamma_buf[SenId_Value];

		if (data < 1) {
			idx = 0;
			LumIdx = 0;
		} else {
			idx = (data * 128) / 4096;
			LumIdx = (idx * 4096) / 128;
		}
		LumIdx1 = ((idx + 1) * 4096) / 128;

		YLum = ((GammaLUT_128Tab[(idx + 1)] - GammaLUT_128Tab[idx]) * (data - LumIdx)) / (LumIdx1 - LumIdx);
		YLum += GammaLUT_128Tab[idx];

		if ((YLum < 0) || (YLum > 1023)) {

			DBG_ERR("%d %d %d %d %d\r\n", idx, LumIdx, LumIdx1, GammaLUT_128Tab[idx], GammaLUT_128Tab[(idx + 1)]);

			DBG_ERR("data = %d YLum = %d\r\n", data, YLum);
		}
		return YLum;
	}
}

UINT32 iq_get_inv_gamma_value(UINT32 Id, UINT32 Data10Bit)
{
	UINT32 i;
	UINT32 ChkFlag = FALSE;
	UINT32 Idx1 = 0, Idx = 0;
	UINT64 RawLum;
	UINT32 *GammaLUT_128Tab;
	SENSOR_INFO sen_info = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	UINT32 SenId_Value = iq_get_sensor_idx(Id);
	ER rt = E_OK;
	sen_info.mode = NULL;

	sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_PRVSENMODE);
	rt = sensor_get_info((SENSOR_ID)IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_STATUS, (void *)&sensor_status);
	sen_info = sensor_status.sensor_info;
	if (rt != E_OK || sen_info.mode == NULL) {
		DBG_ERR("NULL sen_info.Mode\r\n");
		return 0;
	}

	if (sen_info.mode->mode_type == SENSOR_MODE_STAGGER_HDR) {
		if (Data10Bit < 1) {
			RawLum = 0;
		} else {
			RawLum = Data10Bit << 2;
		}

		if (RawLum > 4095) {
			RawLum = 4095;
		}

		return RawLum;
	} else {
		if (Data10Bit < 1) {
			RawLum = 0;
			return RawLum;
		}

		GammaLUT_128Tab =  g_gamma_buf[SenId_Value];

		for (i = 1; i < 129; i ++) {
			if (Data10Bit <= GammaLUT_128Tab[i]) {
				Idx1 = i;
				Idx = (Idx1 - 1);
				ChkFlag = TRUE;
				break;
			}
            if (i == 128) {
				RawLum = 4095;
			    return RawLum;
            }
		}

		if ((GammaLUT_128Tab[Idx1] - GammaLUT_128Tab[Idx]) == 0) {
			RawLum = (Idx << 10);
			RawLum = ((RawLum << 12) >> 7) >> 10;

			if (RawLum > 4096) {
				DBG_ERR("%d %d %d %d %d\r\n", Idx, Idx1, Data10Bit, GammaLUT_128Tab[Idx], GammaLUT_128Tab[Idx1]);
			} else if (RawLum == 4096) {
				RawLum = 4095;
			}

			return RawLum;
		}

		if (ChkFlag == TRUE) {
			RawLum = ((UINT64)((Idx1 - Idx) * (Data10Bit - GammaLUT_128Tab[Idx])) << 10) / (GammaLUT_128Tab[Idx1] - GammaLUT_128Tab[Idx]);
			RawLum += (Idx << 10);
			RawLum = ((RawLum << 12) >> 7) >> 10;

			if (RawLum > 4096) {
				DBG_ERR("%d %d %d %d %d\r\n", Idx, Idx1, Data10Bit, GammaLUT_128Tab[Idx], GammaLUT_128Tab[Idx1]);
			} else if (RawLum == 4096) {
				RawLum = 4095;
			}

			return RawLum;
		} else {
			DBG_ERR("input overflow %d\r\n", Data10Bit);
			return 0;
		}
	}
}

UINT32 iq_get_ob(UINT32 Id)
{
    UINT32 SenId_Value = iq_get_sensor_idx(Id);
    UINT32 OB = (g_rhe_ob_buf[SenId_Value][1] + g_rhe_ob_buf[SenId_Value][2]) >> 1;
    return OB;
}

void iq_set_awb2sat_ratio(UINT32 id, UINT32 ratio)
{
    UINT32 SenId_Value = iq_get_sensor_idx(id);
    if (ratio<=100) {
        g_awb2sat_ratio_buf[SenId_Value] = ratio;
    } else {
        DBG_ERR("Set illegal awb2sat ratio!!!\r\n");
    }
}

////////////////////////////////////
// Movie MCTF setting
////////////////////////////////////
static UINT32 g3DNR[ISP_IQ_MAX_NUM] = {0};
INT32 UI_3DNR[ISP_IQ_MAX_NUM] = {-1,-1,-1,-1,-1,-1,-1,-1};

void iq_get_tnr_config(UINT32 sen_id, UINT32 p3DNRConfig)
{
	UINT32 ui3DNRLevel = 0;
	UINT32 IsoValue = 0, IsoStart = 0, IsoEnd = 0, IQIdxL = 0, IQIdxH = 0;
	iq_get_iso_range_adj(sen_id, IQS_AE_END, &IsoValue, &IsoStart, &IsoEnd, &IQIdxL, &IQIdxH);

	if((g_tnr_level_addr[sen_id] == NULL)||(g_tnr_param_addr[sen_id] == NULL)) {    //IQ param is not ready yet!!
		DBG_ERR("Codec TNR is not ready yet!!\r\n");
        return;
	}

	//DBG_DUMP("sen_id=%d, IQIdxL=%d, IQIdexH=%d, L_level=%d, H_level=%d, max=%d\r\n",Id,IQIdxL,IQIdxH,*(g_tnr_level_addr[sen_id]+IQIdxL),*(g_tnr_level_addr[sen_id]+IQIdxH),*(g_tnr_total_num_addr[sen_id])-1);
	ui3DNRLevel = IQS_Intpl(IsoValue, *(g_tnr_level_addr[sen_id]+IQIdxL), *(g_tnr_level_addr[sen_id]+IQIdxH), IsoStart, IsoEnd);

	if (g_e_ctrl_addr[sen_id]->tnr.level == 0) {
		ui3DNRLevel = 0;
	} else {
		ui3DNRLevel = ui3DNRLevel + g_e_ctrl_addr[sen_id]->tnr.level - 5;
	}
 	g3DNR[sen_id] = IQ_CLAMP(ui3DNRLevel, 0, *(g_tnr_total_num_addr[sen_id]) - 1);

    //** TNR Enable **//
    if(g_e_ctrl_addr[sen_id]->tnr.enable == TNR_OFF) {
		g3DNR[sen_id] = 0;
    }
    //*************** TNR ***************//

	if (UI_3DNR[sen_id] >= 0) {
		g3DNR[sen_id] = UI_3DNR[sen_id];
	}
	//DBG_DUMP("^G g3DNR[%d]=%d\r\n", sen_id, g3DNR[sen_id]);

	if (g3DNR[sen_id] >= (*(g_tnr_total_num_addr[sen_id])) ) {
		memcpy((void*)p3DNRConfig, *g_tnr_param_addr[sen_id], sizeof(H26XEncTnr));
		DBG_ERR("iq_get_tnr_config Wrong parameter %d, disable 3DNR!\r\n", g3DNR[sen_id]);
	} else {
		//DBG_DUMP("^G tnr: en: %d, level = %d, adpTH: %d, %d, %d\r\n",(H26XEncTnr*)(*(g_tnr_param_addr[sen_id]+level))->nr_3d_mode,level,(H26XEncTnr*)(*(g_tnr_param_addr[sen_id]+level))->nr_3d_adp_th_p2p[0],(H26XEncTnr*)(*(g_tnr_param_addr[sen_id]+level))->nr_3d_adp_th_p2p[1],(H26XEncTnr*)(*(g_tnr_param_addr[sen_id]+level))->nr_3d_adp_th_p2p[2]);
		memcpy((void*)p3DNRConfig, *(g_tnr_param_addr[sen_id]+g3DNR[sen_id]), sizeof(H26XEncTnr));
	}
}

UINT32 iq_get_tnr_level(UINT32 sen_id)
{
	return g3DNR[sen_id];
}

////////////////////////////////////
// IQS Config setting
////////////////////////////////////
sISP_isp_info isp_info = {
    0,0,
    "UNKNOW_CHIP",
    {
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
        {99,99,99,"CMOS_OFF","IQ_PARAM_FAKE","AE_PARAM_FAKE","AWB_PARAM_FAKE",{99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99}},
    }
};

iq_param_table* iq_tbl[Total_SENIDNum];
void iqs_get_isp_info(sISP_isp_info	*info)
{
    UINT32 i,j;
    SEN_STATUS_PARAM sensor_status = {0};
	pqs_drv_INFO *drv_name=	pqs_get_drv_name();

	isp_info.size = sizeof(sISP_isp_info);
	strcpy(isp_info.isp_name, drv_name->isp_name);
	for (i=0; i<Total_SENIDNum; i++)
	{
		strcpy(isp_info.iqs_info[i].sensor_name, drv_name->sensor_name[i]);
		strcpy(isp_info.iqs_info[i].ae_setting_name, drv_name->aes_name[i]);
		strcpy(isp_info.iqs_info[i].awb_setting_name , drv_name->awb_name[i]);
		strcpy(isp_info.iqs_info[i].iq_setting_name , drv_name->iqs_name[i]);

		for(j = 0;j<SENSOR_MODE_Num;j++)
            if(pqs_iq_check(i)==TRUE) {
        		isp_info.iqs_info[i].id = i;
                sensor_status.sensor_mode = IPL_AlgGetUIInfo((IPL_PROC_ID)i, IPL_SEL_PRVSENMODE);
                isp_info.iqs_info[i].cur_sen_mode = sensor_status.sensor_mode;
			    isp_info.iqs_info[i].iqs_mode_table[j] = iq_tbl[i][j];
                isp_info.iqs_info[i].iq_feature = IQ_FEATURE_COMMON;

                //if(IPL_AlgGetUIInfo((IPL_PROC_ID)i, IPL_SEL_SHDR) == SEL_SHDR_ON) {
                if(IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR){
                    isp_info.iqs_info[i].iq_feature |= IQ_FEATURE_HDR;
                }

                if((IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) ||
                  ((IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) && (IPL_AlgGetUIInfo(i, IPL_SEL_SHDR) == SEL_SHDR_WDR))) {
                    isp_info.iqs_info[i].iq_feature |= IQ_FEATURE_WDR;
                }

                if(IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_DEFOG) {
                    isp_info.iqs_info[i].iq_feature |= IQ_FEATURE_DEFOG;
                }
            }
	}

	memcpy(info, &isp_info, sizeof(sISP_isp_info));

#if 0
	DBG_DUMP("id:%d, size:%d, ISP_name:%s\r\n",isp_info.id,isp_info.size,isp_info.isp_name);
	for(i=0;i<Total_SENIDNum;i++) {
    	DBG_DUMP("id:%d, CurSensorMode:%d, iq_feature:%d\r\n",isp_info.iqs_info[i].id,isp_info.iqs_info[i].cur_sen_mode,isp_info.iqs_info[i].iq_feature);
    	DBG_DUMP("Sensor:%s, IQ_setting:%s\r\n",isp_info.iqs_info[i].sensor_name,isp_info.iqs_info[i].iq_setting_name);
    	DBG_DUMP("AE_setting:%s, AWB_setting:%s\r\n",isp_info.iqs_info[i].ae_setting_name,isp_info.iqs_info[i].awb_setting_name);
    	DBG_DUMP("ModeTab:");
    	for(j=0;j<SENSOR_MODE_Num;j++) {
            DBG_DUMP("%d, ",isp_info.iqs_info[i].iqs_mode_table[j]);
    	}
    	DBG_DUMP("\r\n");
	}
#endif
}

void iq_set_mode2tbl(UINT32 id, iq_param_table* piq_tbl)
{
	iq_tbl[id] = piq_tbl;
}

void iq_get_hdr_param(UINT id, SHDR_IQ_PARA *param)
{
	*param = *g_hdr_addr[id];
}

void iq_get_defog_param(UINT id, S_DEFOG_LIB_INIT_PARA *param)
{
	*param = *g_defog_addr[id];
}

UINT32* iq_get_defog_wet(UINT id)
{
	return g_defog_wet_addr[id];
}

UINT8 iq_get_wdr_strength(UINT id)
{
    //DBG_DUMP("iq_get_wdr_param:%d\r\n",g_wdr_strength_buf[id]);
    return g_wdr_strength_buf[id];
}

void iq_get_gce_strength(UINT id , SR_WDR_GCE_PARAM *gce_param)
{
    *gce_param = g_gce_strength_buf[id];
}

void iq_get_status(sISP_IQ_STATUS *iq_status)
{
    iq_status->size = sizeof(sISP_IQ_STATUS);

    iq_status->wdr_strength = SceneRender_WDR_GetStrth(iq_status->id);
    iq_status->hdr_strength = 128;//SensorHDR_GetDELevel(); for new HDR, 20180716
    //DBG_DUMP("iq_status:id=%d,wdr_strength=%d,hdr_strength=%d\r\n",iq_status->id,SceneRender_WDR_GetStrth(iq_status->id),SensorHDR_GetDELevel());
}

IPL_STRP_INFO * iq_get_stripe_info(UINT id, UINT32 msg_id)
{
	static IPL_STRP_INFO *strp_info;

	switch ((UINT32)msg_id) {
		case IQCB_DCE_LUT_SIDE_FULL:
			strp_info = &((g_strp_info_addr[id])[0]);
			break;
		case IQCB_DCE_LUT_SIDE_HORI:
			strp_info = &((g_strp_info_addr[id])[1]);
			break;
		case IQCB_DCE_LUT_TOP:
			strp_info = &((g_strp_info_addr[id])[2]);
			break;
		case IQCB_DCE_LUT_USER:
		default:
			strp_info = &((g_strp_info_addr[id])[3]);
			break;
	}

	return strp_info;

}

