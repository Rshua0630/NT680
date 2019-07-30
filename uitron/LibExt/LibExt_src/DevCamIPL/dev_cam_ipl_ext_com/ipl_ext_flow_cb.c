/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       ipl_ext_flow_cb.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#include "iq_sim.h"
#include "Delay.h"
#include "sie_lib.h"
#include "ipl_ext_common.h"
#include "awb_api.h"

void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawMFHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawDual(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawRHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);

typedef void (*CapFP)(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);



CapFP CaptureFP[IPLCTRL_CAP_FLOW_MAX] = {
	NULL,
	IPL_FCB_GetCapRawNormal,
	IPL_FCB_GetCapRawHDR,
	IPL_FCB_GetCapRawNormal,
	IPL_FCB_GetCapRawNormal,
	IPL_FCB_GetCapRawEBurst,
	IPL_FCB_GetCapRawInVid,
	IPL_FCB_GetCapRawNormal,
	IPL_FCB_GetCapRawNormal,
	IPL_FCB_GetCapRawSim,
	IPL_FCB_GetCapRawMFHDR,
	IPL_FCB_GetCapRawNormal,
	IPL_FCB_GetCapRawDual,
	IPL_FCB_GetCapRawRHDR
};

/*
void IPL_FCB_FlashTrig(FLASH_INFOR *Data)
{

}
*/


void IPL_FCB_PreFlash(IPL_PROC_ID Id)
{

	//IPL_CBMsgProc(Id, IPL_CBMSG_PREFLASH, NULL);
	//MainFlashInfo = flash_PreFlashFlow(IPL_FCB_FlashTrig);
	//IPL_CBMsgProc(Id, IPL_CBMSG_PREFLASHEND, NULL);

}
void IPL_FCB_PrepareCapAE(IPL_PROC_ID Id)
{

}


void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	FPS_PARAM fps_param = {0};
	ER rt;
	UINT32 sen_fps = 0;
	IPL_SIE_CUR_FRAME_INFO Rsc_Info = {0};
	IPC_CTRL_CMD cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};
	IPC_SIE_Info sie_info[IPL_ID_MAX_NUM] = {0};

	//change sensor mode to capture
	sen_chg_mode.multi_id = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_FRM_BIT);
	sen_chg_mode.mode = IPL_CtrlGetInfor(Id, IPLCTRL_SEN_MODE);
	sen_chg_mode.frame_rate = SEN_FPS_DFT;
	IPL_Ctrl_Sen_ChgMode(Id, sen_chg_mode);
	//get sie iq parameters
	IPL_FCB_CAPSIEIQ(Id, Info->psie->pSubFunc, &Info->sieUpdate);
	sie_info[Id] = *Info;
	//set AE
	AE_SetCapAEArg(Id);

	/***********************************RSC OPEN*************************************/
	if (RSC_OnOffInfor(Id) == SEL_RSC_ON && Id <= RSC_SURPPORT_NUM) {
		if (RSC_GetStatus(Id) == SEL_RSC_OFF) {
			RSC_TrigEvent(Id, (RSC_PROC_1 << Id));
		}
	}
	/***********************************RSC OPEN*************************************/

	//get current framerate
	fps_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_FPS, (void *)&fps_param);
	if (rt == E_OK && fps_param.cur_fps != 0) {
		sen_fps = SEN_FPS_FMT_CONV2_REAL(fps_param.cur_fps);
	} else {
		if (rt != E_OK) {
			DBG_ERR("get sensor info fail\r\n");
			return;
		} else if (fps_param.cur_fps == 0) {
			DBG_WRN("fps is 0\r\n");
			sen_fps = 30000;
		}

	}
	sen_fps = 1000000 / sen_fps;

	IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
	IPL_Ctrl_Runtime_Chg((1 << Id), IPC_SIEx_, (void *)&sie_info[0]);
	cmd[Id] = IPC_RESUME_SIE;
	IPL_Ctrl_Run((1 << Id), cmd);
	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);
	cmd[Id] = IPC_PAUSE_SIE;
	IPL_Ctrl_Run((1 << Id), cmd);

	if (RSC_OnOffInfor(Id) == SEL_RSC_ON) {
		Rsc_Info.id = Id;
		IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &Rsc_Info);
		RSC_SetCapFrameCn(Id, Rsc_Info.frame_cnt);
	}

	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);

	/***********************************RSC CLOSE*************************************/

	if (RSC_GetStatus(Id) == SEL_RSC_ON && Id <= RSC_SURPPORT_NUM) {
		if (RSC_IsIdle() == TRUE) {
			RSCCTRL_SETFLAG(FALSE);

			while (RSCCTRL_GETFLAG(Id) == FALSE) {
				Delay_DelayMs(1);
			}
		}
		RSC_TrigEvent(Id, (RSC_END_1 << Id));
	}

	/***********************************RSC CLOSE*************************************/

}

void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
#if 0   //Jarkko tmp
	IPL_HAL_GROUP Update;
	IPC_IPLInAddr rdeinfo;
	IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};

	Update.sieUpdate = Info->sieUpdate;

	IPC_WaitVD(Id, TRUE);
	SIEx_ENG_ChgParam(Id, Info->Idx, Update, Info->psie);
	SIEx_ENG_Load(Id, Info->Idx, Update);

	IPC_WaitVD(Id, FALSE);

	ProcCmd[Id] = IPC_PAUSE_IPL;
	IPL_CTRLMode(ProcCmd[IPL_ID_1], ProcCmd[IPL_ID_2], ProcCmd[IPL_ID_3], ProcCmd[IPL_ID_4]);
	//check Ch0 need scale or not
	if ((Info->psie->DmaOutSizeH == Info->psie->IMG_Window.Crop.SizeH) &&
		(Info->psie->DmaOutSizeV == Info->psie->IMG_Window.Crop.SizeV)) {
		Info->psie->CH0.Src = NonScaled_Img;
	} else {
		Info->psie->CH0.Src = CH0_Scaled_Img;
	}
	Info->psie->CH0.Out.PPB0_Addr = ExtInfo->Param[1];
	Info->psie->CH0.Out.PPB1_Addr = ExtInfo->Param[0];
	Info->psie->CH0.Out.LineOffset = ExtInfo->Param[2];
	Info->psie->IMG_Window.Crop.StartPix.x = ExtInfo->Param[4];
	Info->psie->IMG_Window.Crop.StartPix.y = ExtInfo->Param[5];
	Info->psie->IMG_Window.Crop.SizeH = ExtInfo->Param[6];
	Info->psie->IMG_Window.Crop.SizeV = ExtInfo->Param[7];
	SIEx_ENG_ChgParam(Id, Info->Idx, Update, Info->psie);
	SIEx_ENG_Load(Id, Info->Idx, Update);

	IPC_WaitVD(Id, FALSE);

	rdeinfo.Idx = Info->Idx;
	rdeinfo.PPB0Addr = ExtInfo->Param[9];
	rdeinfo.PPB1Addr = ExtInfo->Param[8];
	IPL_CTRLRunTimeChg(Id, IPC_Chg_IPL_INAddr, (void *)&rdeinfo);

	ProcCmd[Id] = IPC_RESUME_IPL;
	IPL_CTRLMode(ProcCmd[IPL_ID_1], ProcCmd[IPL_ID_2], ProcCmd[IPL_ID_3], ProcCmd[IPL_ID_4]);
#endif
}

void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	FPS_PARAM fps_param = {0};
	ER rt;
	UINT32 sen_fps = 0;
	IPC_CTRL_CMD cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};
	IPC_SIE_Info sie_info[IPL_ID_MAX_NUM] = {0};
	sie_info[Id] = *Info;

	if (ExtInfo->param[0] == 0) {
		//change sensor mode to capture
		sen_chg_mode.multi_id = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_FRM_BIT);
		sen_chg_mode.mode = IPL_CtrlGetInfor(Id, IPLCTRL_SEN_MODE);
		sen_chg_mode.frame_rate = ExtInfo->frm_ctrl;
		if (ExtInfo->frm_ctrl == 0) {
			sen_chg_mode.frame_rate = SEN_FPS_DFT;
		} else {
			sen_chg_mode.frame_rate = ExtInfo->frm_ctrl;
		}
		IPL_Ctrl_Sen_ChgMode(Id, sen_chg_mode);
		//get sie iq parameters
		IPL_FCB_CAPSIEIQ(Id, sie_info[Id].psie->pSubFunc, &sie_info[Id].sieUpdate);
		//get current framerate
		fps_param.sensor_mode = SENSOR_MODE_CUR;
		rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_FPS, (void *)&fps_param);
		if (rt == E_OK && fps_param.cur_fps != 0) {
			sen_fps = SEN_FPS_FMT_CONV2_REAL(fps_param.cur_fps);
		} else {
			if (rt != E_OK) {
				DBG_ERR("get sensor info fail\r\n");
				return;
			} else if (fps_param.cur_fps == 0) {
				DBG_WRN("fps is 0\r\n");
				sen_fps = 30000;
			}

		}
		sen_fps = 1000000 / sen_fps;

		//set AE
		AE_SetCapAEArg(Id);
		IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
		IPL_Ctrl_Runtime_Chg((1 << Id), IPC_SIEx_, (void *)&sie_info[0]);
		cmd[Id] = IPC_RESUME_SIE;
		IPL_Ctrl_Run((1 << Id), cmd);
		IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
		cmd[Id] = IPC_PAUSE_SIE;
		IPL_Ctrl_Run((1 << Id), cmd);
	}

	else if (ExtInfo->param[0] == 1 || ExtInfo->param[0] == 2) {
		//get sie iq parameters
		IPL_FCB_CAPSIEIQ(Id, sie_info[Id].psie->pSubFunc, &sie_info[Id].sieUpdate);
		IPL_Ctrl_Runtime_Chg((1 << Id), IPC_SIEx_, (void *)&sie_info[0]);
		cmd[Id] = IPC_RESUME_SIE;
		IPL_Ctrl_Run((1 << Id), cmd);
		IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
		cmd[Id] = IPC_PAUSE_SIE;
		IPL_Ctrl_Run((1 << Id), cmd);
	}

	else if (ExtInfo->param[0] == 2) {
		//get sie iq parameters
		IPL_FCB_CAPSIEIQ(Id, sie_info[Id].psie->pSubFunc, &sie_info[Id].sieUpdate);
		IPL_Ctrl_Runtime_Chg((1 << Id), IPC_SIEx_, (void *)&sie_info[0]);
		cmd[Id] = IPC_RESUME_SIE;
		IPL_Ctrl_Run((1 << Id), cmd);
		IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
		cmd[Id] = IPC_PAUSE_SIE;
		IPL_Ctrl_Run((1 << Id), cmd);
		IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
	}
}


void IPL_FCB_GetCapRawHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	FPS_PARAM fps_param = {0};
	ER rt;
	UINT32 sen_fps = 0;
	IPC_CTRL_CMD cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};
	IPC_SIE_Info sie_info[IPL_ID_MAX_NUM] = {0};

	//change sensor mode to capture
	sen_chg_mode.multi_id = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_FRM_BIT);
	sen_chg_mode.mode = IPL_CtrlGetInfor(Id, IPLCTRL_SEN_MODE);
	sen_chg_mode.frame_rate = SEN_FPS_DFT;
	IPL_Ctrl_Sen_ChgMode(Id, sen_chg_mode);
	//get sie iq parameters
	IPL_FCB_CAPSIEIQ(Id, Info->psie->pSubFunc, &Info->sieUpdate);
	sie_info[Id] = *Info;
	//set AE
	AE_SetCapAEArg_MFHDR(Id,3);

	//get current framerate
	fps_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_FPS, (void *)&fps_param);
	if (rt == E_OK && fps_param.cur_fps != 0) {
		sen_fps = SEN_FPS_FMT_CONV2_REAL(fps_param.cur_fps);
	} else {
		if (rt != E_OK) {
			DBG_ERR("get sensor info fail\r\n");
			return;
		} else if (fps_param.cur_fps == 0) {
			DBG_WRN("fps is 0\r\n");
			sen_fps = 30000;
		}

	}
	sen_fps = 1000000 / sen_fps;

	IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
	IPL_Ctrl_Runtime_Chg((1 << Id), IPC_SIEx_, (void *)&sie_info[0]);
	cmd[Id] = IPC_RESUME_SIE;
	IPL_Ctrl_Run((1 << Id), cmd);
	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);
	cmd[Id] = IPC_PAUSE_SIE;
	IPL_Ctrl_Run((1 << Id), cmd);

	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);
}


void IPL_FCB_GetCapRawMFHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	FPS_PARAM fps_param = {0};
	ER rt;
	UINT32 sen_fps = 0;
	IPC_CTRL_CMD cmd[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};
	IPC_SIE_Info sie_info[IPL_ID_MAX_NUM] = {0};

	//change sensor mode to capture
	sen_chg_mode.multi_id = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_FRM_BIT);
	sen_chg_mode.mode = IPL_CtrlGetInfor(Id, IPLCTRL_SEN_MODE);
	sen_chg_mode.frame_rate = SEN_FPS_DFT;
	IPL_Ctrl_Sen_ChgMode(Id, sen_chg_mode);
	//get sie iq parameters
	IPL_FCB_CAPSIEIQ(Id, Info->psie->pSubFunc, &Info->sieUpdate);
	sie_info[Id] = *Info;
	//set AE
	AE_SetCapAEArg_MFHDR(Id,3);

	//get current framerate
	fps_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_FPS, (void *)&fps_param);
	if (rt == E_OK && fps_param.cur_fps != 0) {
		sen_fps = SEN_FPS_FMT_CONV2_REAL(fps_param.cur_fps);
	} else {
		if (rt != E_OK) {
			DBG_ERR("get sensor info fail\r\n");
			return;
		} else if (fps_param.cur_fps == 0) {
			DBG_WRN("fps is 0\r\n");
			sen_fps = 30000;
		}

	}
	sen_fps = 1000000 / sen_fps;

	IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
	IPL_Ctrl_Runtime_Chg((1 << Id), IPC_SIEx_, (void *)&sie_info[0]);
	cmd[Id] = IPC_RESUME_SIE;
	IPL_Ctrl_Run((1 << Id), cmd);
	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);
	cmd[Id] = IPC_PAUSE_SIE;
	IPL_Ctrl_Run((1 << Id), cmd);

	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);
}

void IPL_FCB_GetCapRawDual(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
#if 0   //Jarkko tmp
	EXPOSURE_SETTING ExpTSetting = {0};
	GAIN_SETTING Gain = {0};
	SENSOR_MODE Mode = 0;
	UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
	UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0}, capext[SEN_MFRAME_MAX_NUM] = {0}, iris[SEN_MFRAME_MAX_NUM] = {0};
	IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};
	SENSOR_ID SenId;
	SenId = IPL_UTI_CONV2_SEN_ID(Id);

	//change sensor mode to capture
	AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);

	sensor_chg_mode(SenId, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
	Sensor_GetCurMode(SenId, &Mode);
	Sensor_GetExpoSetting(SenId, &capext[0], Mode, &ExpTSetting, &Ratio[0]);
	capiso[0] = capiso[0] * Ratio[0] / 100;
	Sensor_GetGainSetting(SenId, &capiso[0], &Gain);
	Sensor_SetExpTime(SenId, &ExpTSetting);
	Sensor_SetGain(SenId, &Gain);

	IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void *)Info);

	IPC_WaitVD(Id, FALSE);

	if (ExtInfo->Param[6] == 0) {
		return;
	}

	//Hot code
#if 1
	ProcCmd[IPL_ID_1] = IPC_RESUME_SIE;
	ProcCmd[IPL_ID_2] = IPC_RESUME_SIE;
	IPL_CTRLMode(ProcCmd[IPL_ID_1], ProcCmd[IPL_ID_2], ProcCmd[IPL_ID_3], ProcCmd[IPL_ID_4]);

	IPC_WaitVD(IPL_ID_1, FALSE);
	IPC_WaitVD(IPL_ID_2, FALSE);

	ProcCmd[IPL_ID_1] = IPC_PAUSE_SIE;
	ProcCmd[IPL_ID_2] = IPC_PAUSE_SIE;
	IPL_CTRLMode(ProcCmd[IPL_ID_1], ProcCmd[IPL_ID_2], ProcCmd[IPL_ID_3], ProcCmd[IPL_ID_4]);

	IPC_WaitVD(IPL_ID_1, FALSE);
	IPC_WaitVD(IPL_ID_2, FALSE);

#else
	ProcCmd[Id] = IPC_RESUME_SIE;
	IPL_CTRLMode(ProcCmd[IPL_ID_1], ProcCmd[IPL_ID_2], ProcCmd[IPL_ID_3], ProcCmd[IPL_ID_4]);
	IPC_WaitVD(Id, FALSE);

	ProcCmd[Id] = IPC_PAUSE_SIE;
	IPL_CTRLMode(ProcCmd[IPL_ID_1], ProcCmd[IPL_ID_2], ProcCmd[IPL_ID_3], ProcCmd[IPL_ID_4]);

	IPC_WaitVD(Id, FALSE);
#endif
#endif
}
void IPL_FCB_GetCapRawRHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	SEN_CHGMODE_DATA sen_chg_mode = {0};
	FPS_PARAM fps_param = {0};
	ER rt;
	UINT32 sen_fps = 0;
	static IPC_CTRL_CMD cmd_resume[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};
	static IPC_CTRL_CMD cmd_pause[IPL_ID_MAX_NUM] = {IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE, IPC_IGNORE};
	static UINT32 total_frm_cnt = 1;
	IPC_SIE_Info sie_info[IPL_ID_MAX_NUM] = {0};
	static IPL_PROC_BIT_ID hdr_proc_id = 0;
	UINT32 i ;

	hdr_proc_id |= (1 << Id);
	//change sensor mode to capture
	if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_BASE_ID) == Id) {
		sen_chg_mode.multi_id = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_FRM_BIT);
		sen_chg_mode.mode = IPL_CtrlGetInfor(Id, IPLCTRL_SEN_MODE);
		sen_chg_mode.frame_rate = SEN_FPS_DFT;
		IPL_Ctrl_Sen_ChgMode(Id, sen_chg_mode);
		//set AE
		AE_SetCapAEArg(Id);

	}
	//get sie iq parameters
	IPL_FCB_CAPSIEIQ(Id, Info->psie->pSubFunc, &Info->sieUpdate);
	sie_info[Id] = *Info;
	//get current framerate
	fps_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_FPS, (void *)&fps_param);
	if (rt == E_OK && fps_param.cur_fps != 0) {
		sen_fps = SEN_FPS_FMT_CONV2_REAL(fps_param.cur_fps);
	} else {
		if (rt != E_OK) {
			DBG_ERR("get sensor info fail\r\n");
			return;
		} else if (fps_param.cur_fps == 0) {
			DBG_WRN("fps is 0\r\n");
			sen_fps = 30000;
		}

	}
	sen_fps = 1000000 / sen_fps;

	IPL_Ctrl_Runtime_Chg((1 << Id), IPC_SIEx_, (void *)&sie_info[0]);
	IPL_Wait_Vd((1 << Id), TRUE, sen_fps + IPL_TIMEOUT_MS);
	cmd_resume[Id] = IPC_RESUME_SIE;
	cmd_pause[Id]  = IPC_PAUSE_SIE;

	if (total_frm_cnt < IPL_CtrlGetInfor(IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_HDR_BASE_ID), IPLCTRL_SIE_CH0_CAP_RAW_HDR_TOTAL_FRM)) {
		total_frm_cnt++;
		return;
	}
	IPL_Ctrl_Run(hdr_proc_id, cmd_resume);

	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);
	IPL_Ctrl_Run(hdr_proc_id, cmd_pause);

	IPL_Wait_Vd((1 << Id), FALSE, sen_fps + IPL_TIMEOUT_MS);
	total_frm_cnt = 1;

	for (i = IPL_ID_1; i < IPL_ID_MAX_NUM; i++) {
		cmd_resume[Id] = IPC_IGNORE;
		cmd_pause[Id]  = IPC_IGNORE;
	}

}

void IPL_FCB_GetCapRaw(IPL_PROC_ID id, IPC_SIE_Info *info, IPLCTRL_CAP_EXT_INFO *ext_info)
{
	if (CaptureFP[IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW)] != NULL) {
		if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW)) {
			if (ext_info->param[0] != 2) {
				IPL_CBMsgProc(id, IPL_CBMSG_SHUTTER_SOUND, NULL);
			}
		} else {
			IPL_CBMsgProc(id, IPL_CBMSG_SHUTTER_SOUND, NULL);
		}
		CaptureFP[IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW)](id, info, ext_info);
	} else {
		DBG_ERR("capture flow(%d) FP null\r\n", IPL_CtrlGetInfor(id, IPLCTRL_CAPFLOW));
	}
}

void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
	DBG_ERR("IQSIM Cap Raw\r\n");
	UINT32 ch0, ch2;
	IQSIM_APPINFO *AppInfo;

	IQSIM_GetRawAddr(&ch0, &ch2);
	AppInfo = (IQSIM_APPINFO *)IQSIM_GetAppInfo();

	UINT32 size = AppInfo->IQSimPriInfo.RawInfo.Size.w * AppInfo->IQSimPriInfo.RawInfo.Size.h * AppInfo->IQSimPriInfo.RawInfo.BitNum / 8;
	DBG_ERR("Ch0 = %x, %x, size = %d\r\n", Info->psie->CH0.Out.PPB_Addr, ch0, size);
	memcpy((void *)Info->psie->CH0.Out.PPB_Addr, (void *)ch0, size);
}

void IPL_FCB_SetCapAWB(IPL_PROC_ID Id, IPC_SIE_Info *Info)
{
	#if 0
	UINT32 i;
	SIE_STCS_CA_RSLT_INFO CARst;
	UINT16 BufR[32*32];
	UINT16 BufG[32*32];
	UINT16 BufB[32*32];
	UINT16 BufIR[32*32];
	UINT16 BufRth[32*32];
	UINT16 BufGth[32*32];
	UINT16 BufBth[32*32];
	UINT16 AccCnt[32*32];

	CARst.puiBufR = BufR;
	CARst.puiBufG = BufG;
	CARst.puiBufB = BufB;
	CARst.puiBufIR = BufIR;
	CARst.puiBufRth = BufRth;
	CARst.puiBufGth = BufGth;
	CARst.puiBufBth = BufBth;

	CARst.puiAccCnt = AccCnt;

	sie1_getCAResult(&CARst);

	for(i=0;i<32*32;i++){
		if(i% 32 == 0 )
			debug_msg("%d\r\n",BufR[i]);
		else
			debug_msg("%d \t",BufR[i]);

	}
	#endif
	if(IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_CAP_CA_EN) == ENABLE) {
        awb_single(Id);
	} else {
	}

}

void IPL_FCB_AlgGDC(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
	IPL_MODE Mode;

	Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

	if ((Mode != IPL_MODE_PREVIEW) &&
		(Mode != IPL_MODE_VIDEO) &&
		(Mode != IPL_MODE_VIDEOREC)) {
		return;
	}

//    iq_set_flow(Id, IQS_MANUAL_TRIG); //Jarkko tmp:IQ
}

void IPL_FCB_AlgDIS(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
#if 0   //Jarkko tmp
	UINT32 Buf, Size;
	IPL_MODE Mode;
	IPL_DZOOM Info_DZ;

	Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

	if ((Mode != IPL_MODE_PREVIEW) &&
		(Mode != IPL_MODE_VIDEO) &&
		(Mode != IPL_MODE_VIDEOREC)) {
		return;
	}

	if (Value == SEL_DISCOMP_OFF) {
		iq_set_flow(Id, IQS_DISCOMP_OFF);
		IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, DISABLE);
	} else if (Value == SEL_DISCOMP_ON) {
		IPL_BufGet(Id, IPLBUF_SIEETH_1, &Buf, &Size);
		if (Size != 0) {
			//iq_set_flow(Id, IQS_DISCOMP_ON);
			IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, ENABLE);
		} else {
			return;
		}
	}

	Info_DZ.Id = Id;
	Info_DZ.Idx = IPL_DzoomGetIndex(Info_DZ.Id);
	IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info_DZ);
	IPL_WaitCmdFinish();
#endif
}

void IPL_FCB_AlgIE(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
//    iq_set_flow(Id, IQS_IMAGE_EFFECT);    //Jarkko tmp:IQ
}

void IPL_FCB_ImageFlip(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
	IPC_Flip_Info FlipInfo;

	FlipInfo.Id = Id;

	switch (Value) {
	case SEL_IMAGEFLIP_OFF:
		FlipInfo.type = FLIP_NONE;
		break;

	case SEL_IMAGEFLIP_H:
		FlipInfo.type = FLIP_H;
		break;

	case SEL_IMAGEFLIP_V:
		FlipInfo.type = FLIP_V;
		break;

	case SEL_IMAGEFLIP_H_V:
		FlipInfo.type = FLIP_H_V;
		break;

	default:
		FlipInfo.type = FLIP_NONE;
		DBG_ERR("Id: %d, Flip type error %d\r\n", FlipInfo.Id, Value);
		break;
	}

	IPL_Ctrl_Runtime_Chg((1 << FlipInfo.Id), IPC_Flip, (void *)&FlipInfo);
}

void IPL_FCB_AlgDR(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
	switch (SelIdx) {
	case IPL_SEL_SHDR:
	case IPL_SEL_WDR_IMM:
	case IPL_SEL_DEFOG_IMM:
		ExtFunc_ChgMode(Id, FALSE);
		break;
	default:
		break;
	}
}

void IPL_RAWHeaderCB(IPL_PROC_ID Id, UINT32 FrmCnt)
{
	if ((Id == IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)) && (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR)) {
		UINT32 addr = SHDRCtrl_GetParam(Id, FrmCnt);
		if (addr != 0) {
			IPL_HeaderRawSetInfo(Id, IPL_HEADER_RAW_RHE_IQ, &addr);
		}
	}

	AEALG_ARG AeData = {0};
	IPL_HEADER_RAW_EXP_INFO AeHeaderInfo = {0};

	IPL_HeaderRawLock(Id);
	//write header information at here
	if (FrmCnt > 0) {
		AE_GetSyncInfo(Id, FrmCnt, &AeData);

		AeHeaderInfo.expt = AeData.ExpoTime;
		AeHeaderInfo.iris = AeData.Iris;
		AeHeaderInfo.iso = AeData.ISOGain;

		IPL_HeaderRawSetInfo_Dly1VD(Id, IPL_HEADER_RAW_EXP, &AeHeaderInfo);
	}

	IPL_HeaderRawUnlock(Id);

}
UINT32 IPL_FCB_CAPSIEIQ(IPL_PROC_ID id, SIE_SUBFUNC *subF, SIE_UPDATE *Update)
{
	IPL_HAL_GROUP group;
	ENG_SUBFUNC Info = {0};
	Info.pSie_sub = subF;
	group.sieUpdate = *Update;
	iq_set_flow_cb(id, IQCB_PREIME_D2D_SIE_PRI, &Info, &group);
	*Update = group.sieUpdate;
	return 1;
}