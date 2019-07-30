/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_RSC_MN34110_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#include "Lens.h"
#include "Delay.h"
#include "gyro_drv_ext.h"
#include "rsc_common.h"
#include "DxSensor.h"
#include "iq_api.h"
#include "ipl_cmd.h"
#include "ipl_ctrl.h"
#include "ipl_header_infor_raw.h"
#include "ipl_cb_msg.h"
#include "Platform.h"
#include "ae_api.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          RSC_sample
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

static UINT32 RSC_DCE_GLUT_Buf[65];
static INT32 RSC_DCE_RLUT_Buf[65];
static INT32 RSC_DCE_BLUT_Buf[65];
static DCE_Distortion  RSC_DCE_Distortion = {0};
static DCE_SUBFUNC DceSubF;
static DCE_SUBFUNC *pRSC_DCE_subf = &DceSubF;
static IQS_FLOW_MSG MsgID[RSC_SURPPORT_NUM + 1] = {IQS_FLOW_MSG_NUM,IQS_FLOW_MSG_NUM,IQS_FLOW_MSG_NUM,IQS_FLOW_MSG_NUM};
static UINT32 Zoom_Value_Cur = 0, Zoom_Value_Prev = 0;
RSCCTRL_INIT InitInfo = {0};
RSC_LENS_DISTORT_INFO *RSC_DistorInfor = NULL;


UINT16 InitDistortTbl[RSC_SURPPORT_NUM + 1][33] = {
	{
		4096, 4050, 3940, 3781, 3592, 3390, 3188, 2993,
		2809, 2640, 2485, 2344, 2215, 2098, 1992, 1894,
		1806, 1724, 1649, 1581, 1517, 1458, 1403, 1353,
		1305, 1261, 1220, 1181, 1144, 1110, 1078, 1047,
		1018,
	},
	{
		4096, 4053, 3947, 3790, 3602, 3399, 3196, 3000,
		2817, 2648, 2493, 2352, 2223, 2107, 2000, 1903,
		1815, 1733, 1659, 1590, 1526, 1467, 1413, 1362,
		1314, 1270, 1229, 1190, 1153, 1119, 1086, 1055,
		1026,
	},
	{
		4096, 4053, 3947, 3790, 3602, 3399, 3196, 3000,
		2817, 2648, 2493, 2352, 2223, 2107, 2000, 1903,
		1815, 1733, 1659, 1590, 1526, 1467, 1413, 1362,
		1314, 1270, 1229, 1190, 1153, 1119, 1086, 1055,
		1026,
	},
	{
		4096, 4053, 3947, 3790, 3602, 3399, 3196, 3000,
		2817, 2648, 2493, 2352, 2223, 2107, 2000, 1903,
		1815, 1733, 1659, 1590, 1526, 1467, 1413, 1362,
		1314, 1270, 1229, 1190, 1153, 1119, 1086, 1055,
		1026,
	}
};

static BOOL RscFirstRunFlag = TRUE;
static HEADER_RAW_DATA CurRaw = {0};

static SEL_RSC_STATUS m_RSC_Status [RSC_SURPPORT_NUM + 1] = {SEL_RSC_OFF, SEL_RSC_OFF, SEL_RSC_OFF, SEL_RSC_OFF};
static SEL_RSC_UISTATUS m_RSC_UISTATUS[RSC_SURPPORT_NUM + 1] = {SEL_RSC_NONE, SEL_RSC_NONE, SEL_RSC_NONE, SEL_RSC_NONE};
static IPL_MODE m_RSC_CurMode[RSC_SURPPORT_NUM + 1] = {IPL_MODE_NONE, IPL_MODE_NONE, IPL_MODE_NONE, IPL_MODE_NONE};
SEL_RSC_MODECHG rsc_modechg[RSC_SURPPORT_NUM + 1] = {SEL_RSC_IDLE, SEL_RSC_IDLE, SEL_RSC_IDLE, SEL_RSC_IDLE} ;
SEL_RSC_STATUS  rsc_OnOffInfor[RSC_SURPPORT_NUM + 1] = {SEL_RSC_OFF, SEL_RSC_OFF, SEL_RSC_OFF, SEL_RSC_OFF};
static UINT32 RSC_CHGMODESIGNAL[RSC_SURPPORT_NUM + 1] = {0};

static RSCCTRL_Q m_QInfo[RSC_SURPPORT_NUM + 1] = {{10, 10, 100}, {10, 10, 50}, {10, 10, 50}, {10, 10, 50}};

RSCCTRL_AXIS_SWITCH Axis_Set[RSC_SURPPORT_NUM + 1] = {{TRUE, TRUE, TRUE}, {TRUE, TRUE, TRUE}, {TRUE, TRUE, TRUE}, {TRUE, TRUE, TRUE}};

static RSCCTRL_STAB StabInfo[RSC_SURPPORT_NUM + 1] = {
	{1, 1, 1, 3, 3, 3, 100, 100, 100, 100, 100, 100, RSC_DIM_CTRL, RSC_TYPE_CTRL},
	{1, 1, 1, 3, 3, 3, 100, 100, 100, 100, 100, 100, RSC_DIM_CTRL, RSC_TYPE_CTRL},
	{1, 1, 1, 3, 3, 3, 100, 100, 100, 100, 100, 100, RSC_DIM_CTRL, RSC_TYPE_CTRL},
	{1, 1, 1, 3, 3, 3, 100, 100, 100, 100, 100, 100, RSC_DIM_CTRL, RSC_TYPE_CTRL}
};
static RSC_EXT_LIB_TAB m_p_rsc_ext_lib = {
	RSC_SetSwitch,
	RSC_OnOffInfor,
	RSC_GetEISRatio,
};
static void RSC_FCB_RunTimeChg(IPL_PROC_ID Id, RSCCTRL_INFO *pInfo)
{
	IPL_DCE_NOR_FACTOR_INFO Info = {0};

	//Cap mode has no header to get information
	if (RSC_GetCurMode(Id) == IPL_MODE_CAP) {
		IPL_SIE_RAW_INFO RawInfo = {0};
		UINT32 ISO, ExpT, Iris;

		RawInfo.id = Id;
		IPL_GetCmd(IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR, &RawInfo);
		pInfo->pIPLSize->SieCropSize.h = RawInfo.img_height;
		pInfo->pIPLSize->SieCropSize.w = RawInfo.img_width;
		pInfo->pIPLSize->SieOutSize.h  = RawInfo.img_height;
		pInfo->pIPLSize->SieOutSize.w  = RawInfo.img_width;
		pInfo->pIPLSize->IfeInSize.h   = RawInfo.img_height;
		pInfo->pIPLSize->IfeInSize.w   = RawInfo.img_width;
		pInfo->pIPLSize->DceOutSize.h  = RawInfo.img_height;
		pInfo->pIPLSize->DceOutSize.w  = RawInfo.img_width;

		AE_GetCapAEArg(Id, &ISO, &ExpT, &Iris);
		*pInfo->pExpT = ExpT;
		pInfo->pFlipInfor->uiHflipEn = InitInfo.Flip_Parm.uiHflipEn;
		pInfo->pFlipInfor->uiVflipEn = InitInfo.Flip_Parm.uiVflipEn;

	} else {
		if (IPL_HeaderRawGetCurInfo_Dly1VD(Id, &CurRaw) == E_SYS) {
			DBG_ERR("RSC: Get header error!!!\r\n");
		}
		// update exp when RscFirstRun and exposure time has been set
		if (CurRaw.exp.expt != 0 || RscFirstRunFlag) {
			*pInfo->pExpT = CurRaw.exp.expt;
		}
		pInfo->pIPLSize->SieCropSize.h = CurRaw.raw_sie_crop.img_info.height;
		pInfo->pIPLSize->SieCropSize.w = CurRaw.raw_sie_crop.img_info.width;
		pInfo->pIPLSize->SieOutSize.h = CurRaw.raw_sie_ch0_out.img_info.height;
		pInfo->pIPLSize->SieOutSize.w = CurRaw.raw_sie_ch0_out.img_info.width;
		pInfo->pIPLSize->IfeInSize.h = CurRaw.raw_ipl_in.img_info.height;
		pInfo->pIPLSize->IfeInSize.w = CurRaw.raw_ipl_in.img_info.width;
		pInfo->pIPLSize->DceOutSize.h = CurRaw.raw_ipl_in.img_info.height;
		pInfo->pIPLSize->DceOutSize.w = CurRaw.raw_ipl_in.img_info.width;
		pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNFact = CurRaw.dce.NFact;
		pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNBit = CurRaw.dce.NBit;

		//H/V Flip
		if (CurRaw.flip_info & FLIP_H) {
			pInfo->pFlipInfor->uiHflipEn = TRUE;
		} else {
			pInfo->pFlipInfor->uiHflipEn = FALSE;
		}
		if (CurRaw.flip_info & FLIP_V) {
			pInfo->pFlipInfor->uiVflipEn = TRUE;
		} else {
			pInfo->pFlipInfor->uiVflipEn = FALSE;
		}


	}
	// unit:mm
	pInfo->pLensInfo->FocalLength = Lens_FocusGetFocalLength(ZOOM_SECTION_WIDE) * (1024 + 100) / 1024;

	if (RSC_DistorInfor == NULL) {
		pInfo->pDistorInfo->puiDistortTbl = &(InitDistortTbl[Id][0]);
		pInfo->pDistorInfo->uiDistortNorm = 128;
		//debug_msg("Null DistorTable Adrress\r\n");
	} else {
		pInfo->pDistorInfo->puiDistortTbl = RSC_DistorInfor->puiDistortTbl;
		pInfo->pDistorInfo->uiDistortNorm = RSC_DistorInfor->uiDistortNorm;
	}



	if ((RscFirstRunFlag == TRUE) || (Zoom_Value_Cur != Zoom_Value_Prev)) {
		RSC_DCE_Distortion.GeoLUT.pGGeoLut = (UINT32 *)RSC_DCE_GLUT_Buf;
		RSC_DCE_Distortion.GeoLUT.pRGeoLut = (INT32 *)RSC_DCE_RLUT_Buf;
		RSC_DCE_Distortion.GeoLUT.pBGeoLut = (INT32 *)RSC_DCE_BLUT_Buf;

		pRSC_DCE_subf->pDistortion = &RSC_DCE_Distortion;

		//DBG_ERR("IQ flow is not ready\r\n");
		iq_get_dce_param(Id, MsgID[Id], Zoom_Value_Cur, pRSC_DCE_subf->pDistortion, &pRSC_DCE_subf->Dce_Dist, &pRSC_DCE_subf->Radious);
		//liwei rsc temp


		pInfo->pDceInfo->DcSel = _GDC_2DLUT_CASCADE; //RSC default open / GDC must open / if RSC opne --> CASCADE!

		pInfo->pDceInfo->ImgCent.iXCent = RSC_DCE_Distortion.Center.x;
		pInfo->pDceInfo->ImgCent.iYCent = RSC_DCE_Distortion.Center.y;

		pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiXDist = pRSC_DCE_subf->Dce_Dist.Fact_X;
		pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiYDist = pRSC_DCE_subf->Dce_Dist.Fact_Y;

		pInfo->pDceInfo->FovParam.uiFovGain = RSC_DCE_Distortion.Fov.Gain;
		if (RSC_DCE_Distortion.Fov.FOV_SEL == FOV_BOUND_DUPLICATE) {
			pInfo->pDceInfo->FovParam.FovSel = _FOV_BOUND_DUPLICATE;
		} else if (RSC_DCE_Distortion.Fov.FOV_SEL == FOV_BOUND_REG_RGB) {
			pInfo->pDceInfo->FovParam.FovSel = _FOV_BOUND_REG_RGB;
		}
		pInfo->pDceInfo->FovParam.FovRGB.uiRBound = RSC_DCE_Distortion.Fov.BoundryProcess.R;
		pInfo->pDceInfo->FovParam.FovRGB.uiGBound = RSC_DCE_Distortion.Fov.BoundryProcess.G;
		pInfo->pDceInfo->FovParam.FovRGB.uiBBound = RSC_DCE_Distortion.Fov.BoundryProcess.B;

		if (RSC_DCE_Distortion.ColorAber.Cacsel == CAC_RGB_GAIN) {
			pInfo->pDceInfo->GdcCacOzParam.CacSel = _RGB_GAIN;
		} else if (RSC_DCE_Distortion.ColorAber.Cacsel == CAC_RGB_LUT) {
			pInfo->pDceInfo->GdcCacOzParam.CacSel = _RGB_LUT;
		}
		pInfo->pDceInfo->GdcCacOzParam.CacGain.uiRLutGain = RSC_DCE_Distortion.ColorAber.CACGain.Fact_R;
		pInfo->pDceInfo->GdcCacOzParam.CacGain.uiGLutGain = RSC_DCE_Distortion.ColorAber.CACGain.Fact_G;
		pInfo->pDceInfo->GdcCacOzParam.CacGain.uiBLutGain = RSC_DCE_Distortion.ColorAber.CACGain.Fact_B;

		pInfo->pDceInfo->GdcCacOzParam.puiGGeoLut = RSC_DCE_Distortion.GeoLUT.pGGeoLut;
		pInfo->pDceInfo->GdcCacOzParam.piRGeoLut = RSC_DCE_Distortion.GeoLUT.pRGeoLut;
		pInfo->pDceInfo->GdcCacOzParam.piBGeoLut = RSC_DCE_Distortion.GeoLUT.pBGeoLut;
	}

	//Only for cap mode
	if (RSC_GetCurMode(Id) == IPL_MODE_CAP) {

		RSCCTRL_SEN_INFO SensorInfor = {0};
		Info.id = Id;
		RSCCTRL_GetSensorInfor(Id, &SensorInfor);
		Info.width = SensorInfor.Width1X;
		Info.height = SensorInfor.Height1X;
		Info.cent_x = (SensorInfor.Width1X) / 2;
		Info.cent_y = (SensorInfor.Height1X) / 2;
		Info.x_disf = pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiXDist;
		Info.y_disf = pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiYDist;
		IPL_GetCmd(IPL_GET_DCE_NOR_FACTOR_INFOR, (void *)&Info);
		if (Info.height == 0 || Info.width == 0) {
			debug_msg("Width1X and Height1X are zero\r\n");
		}

		pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNFact = Info.norm_fact;
		pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNBit  = Info.norm_bit;

	}

	Zoom_Value_Prev = Zoom_Value_Cur;
	if (RscFirstRunFlag == FALSE) {
		//gyro_getFreeRunData(GYRO_SENSOR_ID_1, &pInfo->pGDTbl->DataCnt, pInfo->pGDTbl->pGyroX, pInfo->pGDTbl->pGyroY, pInfo->pGDTbl->pGyroZ);
	} else {
		RscFirstRunFlag = FALSE;
	}
}

static UINT32 RSC_CalGD_NumbyFPS(UINT32 FPS)
{
	UINT32 GDNum;

	if (FPS <= 30) {
		GDNum = 32;
	} else if (FPS <= 60) {
		GDNum = 16;
	} else if (FPS <= 120) {
		GDNum = 8;
	} else {
		GDNum = 8;
	}

	return GDNum;
}

IPL_MODE RSC_GetCurMode(IPL_PROC_ID Id)
{
	return m_RSC_CurMode[Id];
}

void RSC_Process(IPL_PROC_ID Id, UINT32 Value)
{
	GYRO_OPENOBJ openObj;
	GYRO_FREE_RUN_PARAM frParam = {0};
	SENSOR_INFO SenInfo = {0};
	SEN_STATUS_PARAM sen_status = {0};
	IPL_MODE_INFOR Info = {0};
	GYRO_RSCINFOINIT gyrorscInit;
	static RSCCTRL_Q m_QInfo_open = {0};
	//static RSCCTRL_Q m_QInfo_close = {0};
	static UINT32 m_TriggerIdx = 1;
	ER code, GYRO_code, rt = E_OK;
	IPL_SET_IMGINOUT_DATA imginout_info = {0};
	UINT32 gyro_id;

	Info.id = Id;
	IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
	// liwei temp , two gyro interface is not ready
	gyro_id = GYRO_SENSOR_ID_1;//RSCCTRL_GetGYROId(Id);

	m_RSC_CurMode[Id] = Info.mode;

	if ((Info.mode != IPL_MODE_PREVIEW) &&
		(Info.mode != IPL_MODE_VIDEO) &&
		(Info.mode != IPL_MODE_VIDEOREC) &&
		(Info.mode != IPL_MODE_CAP)) {
		debug_msg("Mode error!! Get Mode = %d\r\n", Info.mode);
		return;
	}

	if (Info.mode != IPL_MODE_CAP) {
		MsgID[Id] = IQCB_PRV_DCE_INIT;
		sen_status.sensor_mode = IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE);

	} else {
		MsgID[Id] = IQCB_PREIME_D2D_DCE_PRI;
		sen_status.sensor_mode = IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE);
	}

	if (Value == SEL_RSC_ON) {
		rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_STATUS, (void *)&sen_status);
		SenInfo = sen_status.sensor_info;
		if (rt != E_OK || SenInfo.mode == NULL) {
			DBG_ERR("Get Sensor Status Fail id:%d, Sensor_Mode: %d\r\n", Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE));
		}

		m_QInfo_open = m_QInfo[Id];
		if (Info.mode == IPL_MODE_CAP) {
			m_QInfo_open.CompRatioRSC = m_QInfo_open.CompRatioRSC + m_QInfo_open.CompRatioEIS;
			m_QInfo_open.CompRatioEIS = 0 ;
		}

		if (Info.mode == IPL_MODE_CAP) {
			InitInfo.IPL_MODE = RSCCTRL_MODE_CAP;
		} else {
			InitInfo.IPL_MODE = RSCCTRL_MODE_NORMAL;
		}

		InitInfo.pRSC_Q = &m_QInfo_open;
		InitInfo.FuncEn = _RSC_EIS;
		InitInfo.GD_Num = RSC_CalGD_NumbyFPS(SEN_FPS_FMT_CONV2_REAL(SenInfo.mode->frame_rate) / RSC_SENSOR_FRM_UNIT);
		InitInfo.TwoDLutSz = 9;
		InitInfo.SenSize.w = SenInfo.cell_width;
		InitInfo.SenSize.h = SenInfo.cell_height;


		gyro_RSCInfoInit(gyro_id, &gyrorscInit);
		InitInfo.GyroRef.GDFmt = (RSCCTRL_GD_FMT)gyrorscInit.gdFormat;
		InitInfo.GyroRef.GDAxis[0] = (RSCCTRL_GD_AXIS)gyrorscInit.AxisSelec[0];
		InitInfo.GyroRef.GDAxis[1] = (RSCCTRL_GD_AXIS)gyrorscInit.AxisSelec[1];
		InitInfo.GyroRef.GDAxis[2] = (RSCCTRL_GD_AXIS)gyrorscInit.AxisSelec[2];
		InitInfo.GyroRef.GDDir[0] = (RSCCTRL_GD_DIR)gyrorscInit.DirSelec[0];
		InitInfo.GyroRef.GDDir[1] = (RSCCTRL_GD_DIR)gyrorscInit.DirSelec[1];
		InitInfo.GyroRef.GDDir[2] = (RSCCTRL_GD_DIR)gyrorscInit.DirSelec[2];
		InitInfo.GyroRef.GDunit = gyrorscInit.uiDataUnit;
		InitInfo.GyroRef.GDCalGain[0] = gyrorscInit.uiCalGain[0];
		InitInfo.GyroRef.GDCalGain[1] = gyrorscInit.uiCalGain[1];
		InitInfo.GyroRef.GDCalGain[2] = gyrorscInit.uiCalGain[2];
		InitInfo.GyroRef.GDCalOfs[0] = gyrorscInit.iCalOfs[0];
		InitInfo.GyroRef.GDCalOfs[1] = gyrorscInit.iCalOfs[1];
		InitInfo.GyroRef.GDCalOfs[2] = gyrorscInit.iCalOfs[2];
		InitInfo.fpRunTimeChg = RSC_FCB_RunTimeChg;
		InitInfo.fpGetGyroData = RSC_GetGyroData;

		InitInfo.StabInfor = &StabInfo[Id];
		switch (IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEFLIP_IMM)) {
		case SEL_IMAGEFLIP_H:
			InitInfo.Flip_Parm.uiHflipEn = TRUE;
			break;
		case SEL_IMAGEFLIP_V:
			InitInfo.Flip_Parm.uiVflipEn = TRUE;
			break;
		case SEL_IMAGEFLIP_H_V:
			InitInfo.Flip_Parm.uiHflipEn = TRUE;
			InitInfo.Flip_Parm.uiVflipEn = TRUE;
			break;
		default:
			InitInfo.Flip_Parm.uiHflipEn = FALSE;
			InitInfo.Flip_Parm.uiVflipEn = FALSE;
			break;
		}

		InitInfo.Axiset.uiCompXEn = Axis_Set[Id].uiCompXEn;
		InitInfo.Axiset.uiCompYEn = Axis_Set[Id].uiCompYEn;
		InitInfo.Axiset.uiCompZEn = Axis_Set[Id].uiCompZEn;

		// do not use dzoom cmd when CHGMODE_END
		if (RSC_GetCurMode(Id) != IPL_MODE_CAP && RSC_CHGMODESIGNAL[Id] != IPL_RSC_CBMSG_CHGMODE_END) {

			imginout_info.id = Id;
			imginout_info.crop_ratio = IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRV_CROP_RATIO);
			imginout_info.path_id = IPL_IME_PATH_NONE;
			imginout_info.pnext = NULL;

			IPL_SetCmd(IPL_SET_IMGINOUT, (void *)&imginout_info);
			IPL_WaitCmdFinish();

			IPL_Wait_Vd((1 << Id), TRUE, RSC_TIMEOUT_MS);

		}
		//clear RSC_CHGMODESIGNAL
		RSC_CHGMODESIGNAL[Id] = 0xff;


		code = RSCCTRL_OPEN(Id, &InitInfo);

		if (code == E_SYS) {
			debug_msg("RSC ERROR_SYSTEM \r\n");
			return;
		}

		//check gyro ready
		//liwei rsc temp (wait for gyro driver ready)
		openObj.gyro_cb = RSCCTRL_TRIG;
		openObj.vd_src = SPI_VD_SRC_SIE1 << Id ;
		GYRO_code = gyro_open(gyro_id, &openObj);
		if (GYRO_code == E_SYS) {
			debug_msg("GYRO OPEN Fail \r\n");
			return;
		}

		frParam.uiPeriodUs = 1000000 * RSC_SENSOR_FRM_UNIT / SEN_FPS_FMT_CONV2_REAL(SenInfo.mode->frame_rate);
		frParam.uiDataNum = InitInfo.GD_Num;
		frParam.uiTriggerIdx = (InitInfo.GD_Num - 1) - m_TriggerIdx;
		frParam.triggerMode = GYRO_FREERUN_SIE_SYNC;
		gyro_setFreeRunParam(gyro_id, &frParam);
		gyro_setMode(gyro_id, GYRO_OP_MODE_FREE_RUN);

		m_RSC_Status[Id] = Value;
	} else if (Value == SEL_RSC_OFF) {

		m_RSC_Status[Id] = Value;

		// enter free-run form single-access
		gyro_setMode(gyro_id, GYRO_OP_MODE_SINGLE_ACCESS);
		// close gyro
		gyro_close(gyro_id);
		//rsc off
		RSC_SetCmd(RSC_END_DONE_1);
		RSCCTRL_CLOSE(Id, RSC_CHGMODESIGNAL[Id]);
		RSC_CHGMODESIGNAL[Id] = 0;


		RscFirstRunFlag = TRUE;
	}
}

void RSC_SetStatus(IPL_PROC_ID Id, SEL_RSC_STATUS Value)
{
	IPL_MODE_INFOR Info = {0};

	Info.id = Id;
	IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
	if (Value == SEL_RSC_ON) {

		RSC_Process(Id, Value);

	} else if (Value == SEL_RSC_OFF) {
		DBG_IND("RSC func = %d\r\n", Value);
		RSC_Process(Id, Value);

	} else {
		debug_msg("RSC Set error!! Get SetStatus = %d\r\n", Value);
	}
}
void RSC_GetGyroData(RSCCTRL_GYRO_TBL *gyrodata)
{
	gyro_getFreeRunData(GYRO_SENSOR_ID_1, &gyrodata->DataCnt, gyrodata->pGyroX, gyrodata->pGyroY, gyrodata->pGyroZ);

}
SEL_RSC_STATUS RSC_GetStatus(IPL_PROC_ID Id)
{
	return m_RSC_Status[Id];
}

SEL_RSC_STATUS RSC_OnOffInfor(UINT32 Id)
{
	if (Id > RSC_SURPPORT_NUM) {
		DBG_WRN("Id overflow (%d)\r\n", Id);
		return SEL_RSC_OFF;
	}
	return rsc_OnOffInfor[Id];
}
SEL_RSC_UISTATUS RSC_GetUIStatus(IPL_PROC_ID Id)
{
	return m_RSC_UISTATUS[Id];
}


UINT32 RSC_GetEISRatio(UINT32 Id, SEL_RSC_GETRATIO Ratio_Type, SENSOR_MODE NextSensoeMode)
{
	UINT32 Ratio;
	UINT32 RatioPnt;
	static RSCCTRL_Q m_QInfo_open = {0};
	static RSCCTRL_Q m_QInfo_close = {0};

	if (Id > RSC_SURPPORT_NUM) {
		DBG_WRN("id overflow(%d) \r\n", Id);
		return IMG_EISDIS_RATIO_UINT;
	}

	m_QInfo_open = m_QInfo[Id];
	if (Ratio_Type == SEL_EIS_RATIO) {
		if (rsc_OnOffInfor[Id] == SEL_RSC_ON) {
			Ratio = m_QInfo_open.CompRatioEIS;
		} else {
			Ratio = m_QInfo_close.CompRatioEIS;
		}
		RatioPnt = (100 * IMG_EISDIS_RATIO_UINT) / (100 + Ratio);
		return RatioPnt;
	} else if (Ratio_Type == SEL_EIS_O_RATIO) {
		if (rsc_OnOffInfor[Id] == SEL_RSC_ON) {
			Ratio = m_QInfo_open.CompRatioEIS;
		} else {
			Ratio = m_QInfo_close.CompRatioEIS;
		}
		return Ratio;
	} else if (Ratio_Type == SEL_RSC_O_RATIO) {
		if (rsc_OnOffInfor[Id] == SEL_RSC_ON) {
			Ratio = m_QInfo_open.CompRatioRSC;
		} else {
			Ratio = m_QInfo_close.CompRatioRSC;
		}
		return Ratio;
	} else {
		DBG_ERR("unknow type (%d)\r\n", Ratio_Type);
		return 0;
	}
}

static void RSC_IPL_CB(IPL_RSC_CBMSG Msg, void *Data)
{
	// liwei temp libextern need pass ID number
	if (Msg == IPL_RSC_CBMSG_CHGMODE_START) {

		if (RSC_GetStatus(IPL_ID_1) == SEL_RSC_OFF) {
			return;
		}
		RSC_CHGMODESIGNAL[IPL_ID_1] = Msg;
		RSC_TrigEvent(IPL_ID_1, RSC_END_1);

	} else if (Msg == IPL_RSC_CBMSG_CHGMODE_END) {
		if (RSC_GetStatus(IPL_ID_1) == SEL_RSC_ON) {

			return;
		}
		if (rsc_OnOffInfor[IPL_ID_1] == SEL_RSC_ON) {

			//open rsc
		} else {
			return;
		}
		RSC_CHGMODESIGNAL[IPL_ID_1] = Msg;
		RSC_TrigEvent(IPL_ID_1, RSC_PROC_1);

	} else if (Msg == IPL_RSC_CBMSG_CHGMODE_IPLSTOP) {
		//do nothing

	} else {
		debug_msg("RSC IPL CB error!! Get CBMSG = %d\r\n", Msg);
	}
}


void RSC_SetSwitch(UINT32 Id, SEL_RSC_MODECHG Value, SEL_RSC_STATUS Value2)
{

	RSC_SetCmdEvent2(RSC_Process);
	rsc_modechg[Id] = Value;
	rsc_OnOffInfor[Id]  = Value2;

	if (rsc_OnOffInfor[Id] == SEL_RSC_ON) {
		IPL_RSC_CBMsgInit(Id, RSC_IPL_CB);
		RSC_OpenTsk();
	} else {
		IPL_RSC_CBMsgUnInit(Id);
	}

	if (IPL_IsOpen()) {
		IPL_MODE_INFOR Info = {0};

		if (Id <= RSC_SURPPORT_NUM) {
			Info.id = Id;
			IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

			if ((Info.mode != IPL_MODE_PREVIEW) &&
				(Info.mode != IPL_MODE_VIDEO) &&
				(Info.mode != IPL_MODE_VIDEOREC) &&
				(Info.mode != IPL_MODE_CAP)) {
				//debug_msg("RSC Set switch wrong mode = %d\r\n",Info.Mode);
			} else {
				if (rsc_OnOffInfor[Id] == SEL_RSC_ON) {

					if (RSC_GetStatus(Id) == SEL_RSC_ON) {
						return;
					}
					if (rsc_modechg[Id] == SEL_RSC_RUNTIME) {

						RSC_TrigEvent(Id, (RSC_PROC_1 << Id));

					}
				} else if (rsc_modechg[Id] == SEL_RSC_RUNTIME && rsc_OnOffInfor[Id] == SEL_RSC_OFF) {
					if (RSC_GetStatus(Id) == SEL_RSC_OFF) {
						return;
					}
					RSC_TrigEvent(Id, (RSC_END_1 << Id));
					RSC_CloseTsk();
				} else {
				}

			}
		} else {
			debug_msg("Surpport Id must<=%d(%d)\r\n", RSC_SURPPORT_NUM, Id);
		}
	} else {
		debug_msg("IPL should open first(id = %d)\r\n", Id);
	}
}

ER RSC_SetDistorTable(RSC_LENS_DISTORT_INFO *address)
{

	RSC_DistorInfor = address;
	return E_OK;

}

RSC_EXT_LIB_TAB *RSC_ZJ3029B_EXT_LIB_TAB(void)
{
	return &m_p_rsc_ext_lib;
}

