/**
    ipl_ext_extend_func.c

    @file       ipl_ext_extend_func.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "ipl_ext_common.h"
#include "Delay.h"


#define EXTFUNC_MODE_ERR 0xffffffff

#define EXTFUNC_STAGE_GOT_MODE      EXTFUNC_STAGE_1
#define EXTFUNC_STAGE_INITING       EXTFUNC_STAGE_2
#define EXTFUNC_STAGE_INITED        EXTFUNC_STAGE_3
#define EXTFUNC_STAGE_WILL_UNINIT   EXTFUNC_STAGE_4
#define EXTFUNC_STAGE_UNINITING     EXTFUNC_STAGE_5
#define EXTFUNC_STAGE_PROCING       EXTFUNC_STAGE_1

static EXTFUNC_STAGE    gExtFuncInitStage   [EXTFUNC_MODE_MAX]  [IPL_ID_MAX_NUM]    = {EXTFUNC_STAGE_N};
static EXTFUNC_STAGE    gExtFuncProcStage   [EXTFUNC_MODE_MAX]  [IPL_ID_MAX_NUM]    = {EXTFUNC_STAGE_N};
static EXTFUNC_DR       gExtFuncDRMode                          [IPL_ID_MAX_NUM]    = {EXTFUNC_DR_NONE};
static EXTFUNC_DEFOG    gExtFuncDefogMode                       [IPL_ID_MAX_NUM]    = {EXTFUNC_DEFOG_OFF};
static BOOL             gNeed_chgmode                           [IPL_ID_MAX_NUM]    = {FALSE};

static ER               ExtFunc_InitHDR     (UINT32 Id);
static ER               ExtFunc_InitWDR     (UINT32 Id);
static ER               ExtFunc_InitDefog   (UINT32 Id);
static ER               ExtFunc_UnInitHDR   (UINT32 Id);
static ER               ExtFunc_UnInitWDR   (UINT32 Id);
static ER               ExtFunc_UnInitDefog (UINT32 Id);
static ER               ExtFunc_ProcessHDR  (UINT32 Id);
static ER               ExtFunc_ProcessWDR  (UINT32 Id);
static ER               ExtFunc_ProcessDefog(UINT32 Id);
static EXTFUNC_DR       ExtFunc_GetDRMode   (UINT32 Id);
static EXTFUNC_DEFOG    ExtFunc_GetDefogMode(UINT32 Id);
static ER               ExtFunc_WaitStage   (EXTFUNC_STAGE *flg, EXTFUNC_STAGE pat);
static BOOL             ExtFunc_SetTrigEn   (UINT32 Id);
static void             ExtFunc_SetSHDRInfo (UINT32 Id);
static void             ExtFunc_SetParam    (UINT32 Id, EXTFUNC_MODE mode);

#if 0
#endif

void ExtFunc_Init(UINT32 Id)
{
    ER rt;

    /*************************** DR ***************************/

	// if DR stage is none, get new DR mode
	if (gExtFuncInitStage[EXTFUNC_MODE_DR][Id] == EXTFUNC_STAGE_N) {
	    gExtFuncDRMode[Id] = ExtFunc_GetDRMode(Id);

	    if (gExtFuncDRMode[Id] != EXTFUNC_MODE_ERR) {
			gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_GOT_MODE;
	    } else {
	        DBG_ERR("Id %u, DR Init: Get mode fail\r\n", Id);
	        gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_ERR;
	    }
	} else {
		DBG_ERR("Id %u, DR Init: Init fail. Stage should be none\r\n", Id);
	}

    if (gExtFuncInitStage[EXTFUNC_MODE_DR][Id] == EXTFUNC_STAGE_GOT_MODE) {
        gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_INITING;

        if      (gExtFuncDRMode[Id] == EXTFUNC_DR_NONE){
            rt = E_OK;
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_OFF) {
            rt = ExtFunc_InitHDR(Id);
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_ON)  {
            rt = ExtFunc_InitHDR(Id);
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_WDR) {
            rt = ExtFunc_InitWDR(Id);
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_OFF)  {
            rt = ExtFunc_InitWDR(Id);
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_ON)   {
            rt = ExtFunc_InitWDR(Id);
        }
        else                                                {
            DBG_ERR("Id %u, DR Init: Wrong mode\r\n", Id);
            rt = E_SYS;
        }

        if (rt == E_OK) {
            gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_INITED;
        } else {
            DBG_ERR("Id %u, DR Init: Init fail\r\n", Id);
            gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_N;
        }
    }


    /*************************** Defog ***************************/

	// if Defog stage is none, get new Defog mode
	if (gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] == EXTFUNC_STAGE_N) {
	    gExtFuncDefogMode[Id] = ExtFunc_GetDefogMode(Id);

	    if (gExtFuncDefogMode[Id] != EXTFUNC_MODE_ERR) {
			gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_GOT_MODE;
	    } else {
	        DBG_ERR("Id %u, Defog Init: Get mode fail\r\n", Id);
	        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_ERR;
	    }
	} else {
		DBG_ERR("Id %u, Defog Init: Init fail. Stage should be none\r\n", Id);
	}

    if (gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] == EXTFUNC_STAGE_GOT_MODE) {
        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_INITING;

        if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_NONE)
            rt = E_OK;
		else if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_OFF) {
			rt = ExtFunc_InitDefog(Id);
			if (rt == E_OK)
				rt = ExtFunc_UnInitDefog(Id);
        } else if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_ON)
            rt = ExtFunc_InitDefog(Id);
        else {
            DBG_ERR("Id %u, Defog Init: Wrong mode\r\n", Id);
            rt = E_SYS;
        }

        if (rt == E_OK) {
            gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_INITED;
        }
        else {
            DBG_ERR("Id %u, Defog Init: Init fail\r\n", Id);
            gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_N;
        }
    }
}

void ExtFunc_UnInit(UINT32 Id)
{
    ER rt;

    /*************************** DR ***************************/

    gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_N;


    /*************************** Defog ***************************/

    rt = E_OK;

    if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_ON) {
        if (defog_lib_close(Id) != E_OK) {
    		DBG_ERR("Id %u, UnInit Defog Lib fail\r\n", Id);
            rt = E_SYS;
    	}
    }

    if (rt == E_OK)
        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_N;
    else {
        DBG_ERR("Id %u, Defog UnInit: Uninit fail\r\n", Id);
        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_ERR;
    }
}

void ExtFunc_ChgMode(UINT32 Id, BOOL isInstant)
{
	// Delay runtime change HDR mode until next ExtFunc_Process() triggered
	if (!isInstant) {
		gNeed_chgmode[Id] = TRUE;
		return;
	}

	ER              rt;
    IPC_HDR_Info    hdr_rtc_info = {0};


    /*************************** DR ***************************/

    // check if stage is unknown
    if (gExtFuncInitStage[EXTFUNC_MODE_DR][Id] == EXTFUNC_STAGE_ERR) {
        DBG_ERR("Id %u, DR ChgMode: Stage error\r\n", Id);
    }

    // if DR init is done and DR mode is changed, prepare to wait ExtFunc_Process done
    if (gExtFuncInitStage[EXTFUNC_MODE_DR][Id] == EXTFUNC_STAGE_INITED) {
		if (gExtFuncDRMode[Id] != ExtFunc_GetDRMode(Id)) {
        	gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_WILL_UNINIT;
		}
    } else {
        DBG_WRN("Id %u, DR ChgMode: Can't change ExtFunc mode. Init not done yet!\r\n", Id);
    }

    // if someone is waiting and process is done, do uninit. Otherwise, wait until timeout
    if (gExtFuncInitStage[EXTFUNC_MODE_DR][Id] == EXTFUNC_STAGE_WILL_UNINIT &&
        ExtFunc_WaitStage(&gExtFuncProcStage[EXTFUNC_MODE_DR][Id], EXTFUNC_STAGE_N) == E_OK) {
        gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_UNINITING;

        rt = E_OK;

        // DR uninit
        if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_ON || gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_OFF)
            rt = ExtFunc_UnInitHDR(Id);
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_WDR || gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_OFF || gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_ON)
            rt = ExtFunc_UnInitWDR(Id);

        if (rt == E_OK)
            gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_N;
        else {
            DBG_ERR("Id %u, DR ChgMode: Uninit fail\r\n", Id);
            gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_ERR;
        }
    }

    // if DR uninit is done, get new DR mode
    if (gExtFuncInitStage[EXTFUNC_MODE_DR][Id] == EXTFUNC_STAGE_N) {
	    gExtFuncDRMode[Id] = ExtFunc_GetDRMode(Id);

	    if (gExtFuncDRMode[Id] != EXTFUNC_MODE_ERR) {
			gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_GOT_MODE;
	    } else {
	        DBG_ERR("Id %u, DR ChgMode: Get mode fail\r\n", Id);
	        gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_ERR;
	    }
    }

    // if got new DR mode, do init
    if (gExtFuncInitStage[EXTFUNC_MODE_DR][Id] == EXTFUNC_STAGE_GOT_MODE) {
        gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_INITING;

        rt = E_OK;
        if      (gExtFuncDRMode[Id] == EXTFUNC_DR_NONE){
            hdr_rtc_info.rhe_func_mode = RHE_HAL_FUNC_MODE_LINEAR;
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_OFF) {
            rt = ExtFunc_InitHDR(Id);
            hdr_rtc_info.rhe_func_mode = RHE_HAL_FUNC_MODE_LINEAR;
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_ON)  {
            rt = ExtFunc_InitHDR(Id);
            hdr_rtc_info.rhe_func_mode = RHE_HAL_FUNC_MODE_SHDR;
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_WDR) {
            rt = ExtFunc_InitWDR(Id);
            hdr_rtc_info.rhe_func_mode = RHE_HAL_FUNC_MODE_PHDR;
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_OFF)  {
            rt = ExtFunc_InitWDR(Id);
            hdr_rtc_info.rhe_func_mode = RHE_HAL_FUNC_MODE_PHDR;
        }
        else if (gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_ON)   {
            rt = ExtFunc_InitWDR(Id);
            hdr_rtc_info.rhe_func_mode = RHE_HAL_FUNC_MODE_PHDR;
        }
        else {
            DBG_ERR("Id %u, DR ChgMode: Wrong mode\r\n", Id);
            rt = E_SYS;
        }

        if (rt == E_OK) {
            hdr_rtc_info.win_num_x = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINXNUM);
            hdr_rtc_info.win_num_y = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINYNUM);

            rt = IPL_Ctrl_Runtime_Chg((IPL_PROC_BIT_ID)(1 << Id), IPC_HDR_Mode, &hdr_rtc_info);

            if (rt == E_OK)
                gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_INITED;
            else {
                DBG_ERR("Id %u, DR ChgMode: Runtime chg HDR mode fail\r\n", Id);
                gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_N;
            }
        }
        else {
            DBG_ERR("Id %u, DR ChgMode: Init fail\r\n", Id);
            gExtFuncInitStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_N;
        }
    }


    /*************************** Defog ***************************/

    // check if stage is unknown
    if (gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] == EXTFUNC_STAGE_ERR) {
        DBG_ERR("Id %u, Defog ChgMode: Stage error\r\n", Id);
    }

    // if Defog init is done and Defog mode is changed, prepare to wait ExtFunc_Process done
    if (gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] == EXTFUNC_STAGE_INITED) {
		if (gExtFuncDefogMode[Id] != ExtFunc_GetDefogMode(Id)) {
        	gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_WILL_UNINIT;
		}
    } else {
        DBG_WRN("Id %u, Defog ChgMode: Can't change ExtFunc mode. Init not done yet!\r\n", Id);
    }

    // if someone is waiting and process is done, do uninit. Otherwise, wait until timeout
    if (gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] == EXTFUNC_STAGE_WILL_UNINIT &&
        ExtFunc_WaitStage(&gExtFuncProcStage[EXTFUNC_MODE_DEFOG][Id], EXTFUNC_STAGE_N) == E_OK) {
        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_UNINITING;

        rt = E_OK;

        // Defog uninit
        if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_ON)
            rt = ExtFunc_UnInitDefog(Id);

        if (rt == E_OK)
            gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_N;
        else {
            DBG_ERR("Id %u, Defog ChgMode: Uninit fail\r\n", Id);
            gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_ERR;
        }
    }

    // if Defog uninit is done, get new Defog mode
    if (gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] == EXTFUNC_STAGE_N) {
	    gExtFuncDefogMode[Id] = ExtFunc_GetDefogMode(Id);

	    if (gExtFuncDefogMode[Id] != EXTFUNC_MODE_ERR) {
			gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_GOT_MODE;
	    } else {
	        DBG_ERR("Id %u, Defog ChgMode: Get mode fail\r\n", Id);
	        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_ERR;
	    }
    }

    // if got new Defog mode, do init
    if (gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] == EXTFUNC_STAGE_GOT_MODE) {
        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_INITING;

		if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_NONE)
			rt = E_OK;
        else if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_OFF)
            rt = E_OK;
        else if (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_ON)
            rt = ExtFunc_InitDefog(Id);
        else {
            DBG_ERR("Id %u, Defog ChgMode: Wrong mode\r\n", Id);
            rt = E_SYS;
        }

        if (rt == E_OK) {
            gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_INITED;
        }
        else {
            DBG_ERR("Id %u, Defog ChgMode: Init fail\r\n", Id);
            gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_N;
        }
    }
}

void ExtFunc_Process(UINT32 Id)
{
    static BOOL isPrinted [EXTFUNC_MODE_MAX][IPL_ID_MAX_NUM] = {FALSE};

    /*************************** Change mode ***************************/

    if (gNeed_chgmode[Id] == TRUE) {
        ExtFunc_ChgMode(Id, TRUE);
        gNeed_chgmode[Id] = FALSE;
    }


    /*************************** DR ***************************/

    if (        gExtFuncInitStage[EXTFUNC_MODE_DR][Id]  != EXTFUNC_STAGE_INITED) {

        if (isPrinted[EXTFUNC_MODE_DR][Id] != TRUE) {
            DBG_WRN("Id %u, DR Proc: Process fail. Init not done yet\r\n", Id);
            isPrinted[EXTFUNC_MODE_DR][Id] = TRUE;
        }
    }
	else if (   (gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_ON || gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_OFF) &&
                gExtFuncProcStage[EXTFUNC_MODE_DR][Id]  == EXTFUNC_STAGE_N) {

        if (isPrinted[EXTFUNC_MODE_DR][Id] == TRUE) {
            DBG_WRN("Id %u, DR Proc: HDR process resume\r\n", Id);
            isPrinted[EXTFUNC_MODE_DR][Id] = FALSE;
        }

        gExtFuncProcStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_PROCING;
		ExtFunc_ProcessHDR(Id);
        gExtFuncProcStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_N;
	}
    else if ((  gExtFuncDRMode[Id] == EXTFUNC_DR_SHDR_WDR || gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_OFF || gExtFuncDRMode[Id] == EXTFUNC_DR_WDR_ON) &&
                gExtFuncProcStage[EXTFUNC_MODE_DR][Id]  == EXTFUNC_STAGE_N) {

        if (isPrinted[EXTFUNC_MODE_DR][Id] == TRUE) {
            DBG_WRN("Id %u, DR Proc: WDR process resume\r\n", Id);
            isPrinted[EXTFUNC_MODE_DR][Id] = FALSE;
        }

        gExtFuncProcStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_PROCING;
		ExtFunc_ProcessWDR(Id);
        gExtFuncProcStage[EXTFUNC_MODE_DR][Id] = EXTFUNC_STAGE_N;
    }


    /*************************** Defog ***************************/

    if (        gExtFuncInitStage[EXTFUNC_MODE_DEFOG][Id]   != EXTFUNC_STAGE_INITED) {

        if (isPrinted[EXTFUNC_MODE_DEFOG][Id] != TRUE) {
            DBG_WRN("Id %u, Defog Proc: Process fail. Init not done yet\r\n", Id);
            isPrinted[EXTFUNC_MODE_DEFOG][Id] = TRUE;
        }
    }
	else if (   (gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_OFF || gExtFuncDefogMode[Id] == EXTFUNC_DEFOG_ON) &&
                gExtFuncProcStage[EXTFUNC_MODE_DEFOG][Id]   == EXTFUNC_STAGE_N) {

        if (isPrinted[EXTFUNC_MODE_DEFOG][Id] == TRUE) {
            DBG_WRN("Id %u, Defog Proc: Process resume\r\n", Id);
            isPrinted[EXTFUNC_MODE_DEFOG][Id] = FALSE;
        }

        gExtFuncProcStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_PROCING;
		ExtFunc_ProcessDefog(Id);
        gExtFuncProcStage[EXTFUNC_MODE_DEFOG][Id] = EXTFUNC_STAGE_N;
	}
}

#if 0
#endif

static ER ExtFunc_InitHDR(UINT32 Id)
{
    SHDR_OPEN_INFO      hdr_init_info;
	SHDR_SUBIMG_INFO    shdr_sub_info   = {0};

	hdr_init_info.uiHDRframeNo = IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM);
	hdr_init_info.evRatioExpect = ae_get_shdr_ev_ratio(Id);
    iq_get_hdr_param(Id, &hdr_init_info.shdrIQpara);

	ExtFunc_SetSHDRInfo(Id);
	if (SHDRCtrl_Init(Id, &hdr_init_info) != E_OK) {
		DBG_ERR("Id %u, Init SHDR Lib fail\r\n", Id);
        return E_SYS;
	}

    shdr_sub_info = SensorHDR_GetSubImgInfo();
    if (shdr_sub_info.uiSubRatio == 1) {
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), shdr_sub_info.uiSubWidth,  1)); //maximum y output window number 128
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), shdr_sub_info.uiSubHeight, 1)); //maximum y output window number 128
	} else {
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, shdr_sub_info.uiSubWidth);  //maximum y output window number 128
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, shdr_sub_info.uiSubHeight); //maximum y output window number 128
	}

    return E_OK;
}

static ER ExtFunc_InitWDR(UINT32 Id)
{
    SR_WDR_OPEN_INFO    pOpenInfo;
    SR_WDR_SUBIMG_INFO  wdr_sub_info    = {0};

	if (SceneRender_WDR_Init(IPL_UTI_CONV2_WDR_ID(Id), &pOpenInfo) != E_OK) {
		DBG_ERR("Id %u, Init WDR Lib fail\r\n", Id);
        return E_SYS;
	}

    SceneRender_WDR_GetSubImgInfo(IPL_UTI_CONV2_WDR_ID(Id), &wdr_sub_info);
    if (wdr_sub_info.uiSubRatio == 1) {
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), wdr_sub_info.uiSubWidth,  1)); //maximum y output window number 128
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRV_CROP_RATIO), IPL_UTI_CONV2_UINT32(1,1), wdr_sub_info.uiSubHeight, 1)); //maximum y output window number 128
    } else {
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINXNUM, wdr_sub_info.uiSubWidth);  //maximum y output window number 128
		IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_YOUT_WINYNUM, wdr_sub_info.uiSubHeight); //maximum y output window number 128
    }

    return E_OK;
}

static ER ExtFunc_InitDefog(UINT32 Id)
{
	S_DEFOG_LIB_INIT_PARA defog_init_para;

	iq_get_defog_param(Id, &defog_init_para);
	if (defog_lib_init(Id, &defog_init_para) != E_OK) {
		DBG_ERR("Id %u, Init Defog Lib fail\r\n", Id);
        return E_SYS;
	}

    return E_OK;
}

static ER ExtFunc_UnInitHDR(UINT32 Id)
{
    SHDR_OPEN_INFO  hdr_init_info					= {0};
    SHDR_UI_INFO    shdr_info						= {0};
    IPC_QINFO       Info 							= {0};
    SHDR_CAL_INFO   HdrCalInfo						= {0};
	SHDR_YOUT_INFO  hdr_yout_info[SHDR_MAXFRAMES]	= {0};
	UINT32			i;

	hdr_init_info.uiHDRframeNo  = IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM);
	hdr_init_info.evRatioExpect = ae_get_shdr_ev_ratio(Id);
    iq_get_hdr_param(Id, &hdr_init_info.shdrIQpara);
    shdr_info.shdrMode          = SHDR_LINEAR_MODE;
	shdr_info.uiHDRLevel        = IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR_LEVEL_IMM);
	SensorHDR_SetUI(&shdr_info);
	if (SHDRCtrl_Init(Id, &hdr_init_info) != E_OK) {
		DBG_ERR("Id %u, Init SHDR Lib fail\r\n", Id);
        return E_SYS;
	}

	if (IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM) > SHDR_MAXFRAMES) {
		DBG_WRN("HDR frame number (%u) exceed max frame number (%u)\r\n", IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM), SHDR_MAXFRAMES);
		return E_SYS;
	}

	HdrCalInfo.evRatioR             = ae_get_shdr_ev_ratio(Id);
	HdrCalInfo.uiAELV               = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_PRV_LV);
	HdrCalInfo.uiAEExpectLumRaw     = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_CURR_EXPY);
	HdrCalInfo.uiAECurrentLumRaw    = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_CURR_Y);
    iq_get_hdr_param(Id, &HdrCalInfo.shdrIQpara);

	for (i = 0; i < IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM); i++) {
		Info = IPL_Ctrl_Query(i, ICF_SIE_YOUT_RDY);
		HdrCalInfo.pYoutInfo[i]              	= &hdr_yout_info[i];
		HdrCalInfo.pYoutInfo[i]->uiYoutAddr  	= Info.sie_rdy_yout.addr;
		HdrCalInfo.pYoutInfo[i]->uiYoutLofs  	= Info.sie_rdy_yout.lofs;
		HdrCalInfo.pYoutInfo[i]->uiYoutSizeH 	= Info.sie_rdy_yout.win_num_x;
		HdrCalInfo.pYoutInfo[i]->uiYoutSizeV	= Info.sie_rdy_yout.win_num_y;
	    HdrCalInfo.pYoutInfo[i]->uiOB        	= iq_get_ob(i);
	}

    SensorHDR_SetUI(&shdr_info);
	Info = IPL_Ctrl_Query(Id, ICF_FRAME_CNT);
	if (SHDRCtrl_Proc(Id, &HdrCalInfo, Info.FrameCnt) != E_OK) {
		DBG_ERR("Id %u, SHDR Proc fail\r\n", Id);
        return E_SYS;
	}
    SHDRCtrl_Reset(Id);

    return E_OK;
}

static ER ExtFunc_UnInitWDR(UINT32 Id)
{
    SR_WDR_OPEN_INFO        pOpenInfo;
    IPC_QINFO               Info            = {0};
    SR_WDR_CAL_INFO         wdr_info        = {0};
	SR_WDR_YOUT_INFO        wdr_yout_info   = {0};
	SR_WDR_CHG_PARAMS       wdr_chg_par     = {0};
    SR_WDR_GCE_PARAM        gce_param       = {0};

    iq_get_gce_strength(Id, &gce_param);

	if (SceneRender_WDR_Init(IPL_UTI_CONV2_WDR_ID(Id), &pOpenInfo) != E_OK) {
		DBG_ERR("Id %u, Init WDR Lib fail\r\n", Id);
        return E_SYS;
	}

	Info                            = IPL_Ctrl_Query(Id, ICF_SIE_YOUT_RDY);

    wdr_info.WdrProcMode            = SR_WDR_OFF;
	wdr_info.pYoutInfo              = &wdr_yout_info;
	wdr_info.pYoutInfo->uiYoutAddr  = Info.sie_rdy_yout.addr;
	wdr_info.pYoutInfo->uiYoutLofs  = Info.sie_rdy_yout.lofs;
	wdr_info.pYoutInfo->uiYoutSizeH = Info.sie_rdy_yout.win_num_x;
	wdr_info.pYoutInfo->uiYoutSizeV = Info.sie_rdy_yout.win_num_y;
    wdr_info.pYoutInfo->uiOB        = iq_get_ob(Id);
	wdr_info.puiGGammaLut           = iq_get_gamma_lut(IPL_UTI_CONV2_IQS_ID(Id));
	wdr_info.uiWDRLevel             = IPL_AlgGetUIInfo(Id, IPL_SEL_WDR_M_LEVEL_IMM); //only for manual mode

	wdr_chg_par.uiChgSel                    = (SR_WDR_CHG_WDR_STRENGTH | SR_WDR_CHG_AE_TAG_Y | SR_WDR_CHG_GCE_LEVEL);
    wdr_chg_par.uiChgWDRUserSth             = (UINT32)iq_get_wdr_strength(Id); //only for auto mode
	wdr_chg_par.uiChgAeTagY                 = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_CURR_EXPY);
    wdr_chg_par.ChgGceParam.uiGceBrightSth  = gce_param.uiGceBrightSth;
	wdr_chg_par.ChgGceParam.uiGceDarkSth    = gce_param.uiGceDarkSth;
	if (SceneRender_WDR_ChgParams   (IPL_UTI_CONV2_WDR_ID(Id), &wdr_chg_par)    != E_OK ||
        SceneRender_WDR_Proc        (IPL_UTI_CONV2_WDR_ID(Id), &wdr_info)       != E_OK) {
		DBG_ERR("Id %u, WHDR Proc fail\r\n", Id);
        return E_SYS;
	}

    return E_OK;
}

static ER ExtFunc_UnInitDefog(UINT32 Id)
{
    if (defog_lib_close(Id) != E_OK) {
		DBG_ERR("Id %u, UnInit Defog Lib fail\r\n", Id);
        return E_SYS;
	}

    return E_OK;
}

static ER ExtFunc_ProcessHDR(UINT32 Id)
{
    IPC_QINFO       Info                            = {0};
    ER              rt                              = E_OK;
    SHDR_CAL_INFO   HdrCalInfo						= {0};
	SHDR_YOUT_INFO  hdr_yout_info[SHDR_MAXFRAMES]   = {0};
	UINT32			i;

	if (IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM) > SHDR_MAXFRAMES) {
		DBG_WRN("HDR frame number (%u) exceed max frame number (%u)\r\n", IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM), SHDR_MAXFRAMES);
		return E_SYS;
	}

	HdrCalInfo.evRatioR             = ae_get_shdr_ev_ratio(Id);
	HdrCalInfo.uiAELV               = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_PRV_LV);
	HdrCalInfo.uiAEExpectLumRaw     = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_CURR_EXPY);
	HdrCalInfo.uiAECurrentLumRaw    = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_CURR_Y);
    iq_get_hdr_param(Id, &HdrCalInfo.shdrIQpara);

	for (i = 0; i < IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM); i++) {
		Info = IPL_Ctrl_Query(i, ICF_SIE_YOUT_RDY);
	    HdrCalInfo.pYoutInfo[i]              	= &hdr_yout_info[i];
		HdrCalInfo.pYoutInfo[i]->uiYoutAddr  	= Info.sie_rdy_yout.addr;
		HdrCalInfo.pYoutInfo[i]->uiYoutLofs  	= Info.sie_rdy_yout.lofs;
		HdrCalInfo.pYoutInfo[i]->uiYoutSizeH	= Info.sie_rdy_yout.win_num_x;
		HdrCalInfo.pYoutInfo[i]->uiYoutSizeV 	= Info.sie_rdy_yout.win_num_y;
	    HdrCalInfo.pYoutInfo[i]->uiOB        	= iq_get_ob(i);
	}

	ExtFunc_SetSHDRInfo(Id);
	Info = IPL_Ctrl_Query(Id, ICF_FRAME_CNT);
    rt = SHDRCtrl_Proc(Id, &HdrCalInfo, Info.FrameCnt);
	if (rt != E_OK) {
		DBG_ERR("Id %u, SHDR Proc fail\r\n", Id);
        return E_SYS;
	} else {
		iq_set_flow(Id, IQS_SHDR_END);
	}

    return E_OK;
}

static ER ExtFunc_ProcessWDR(UINT32 Id)
{
    ER                      rt              = E_OK;
    IPC_QINFO               Info            = {0};
    SR_WDR_CAL_INFO         wdr_info        = {0};
	SR_WDR_YOUT_INFO        wdr_yout_info   = {0};
	SR_WDR_CHG_PARAMS       wdr_chg_par     = {0};
    SR_WDR_GCE_PARAM        gce_param       = {0};

    iq_get_gce_strength(Id, &gce_param);

	if (ExtFunc_SetTrigEn(Id)) {
		switch (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR_IMM)) {
		case SEL_WDR_OFF:
			wdr_info.WdrProcMode = SR_WDR_OFF;
			break;

		case SEL_WDR_AUTO:
			wdr_info.WdrProcMode = SR_WDR_AUTO;
			break;

		case SEL_WDR_MANUAL:
			wdr_info.WdrProcMode = SR_WDR_MANUAL;
			break;

		default:
			wdr_info.WdrProcMode = SR_WDR_OFF;
			break;
		}

		Info = IPL_Ctrl_Query(Id, ICF_SIE_YOUT_RDY);

		wdr_info.pYoutInfo              = &wdr_yout_info;
		wdr_info.pYoutInfo->uiYoutAddr  = Info.sie_rdy_yout.addr;
		wdr_info.pYoutInfo->uiYoutLofs  = Info.sie_rdy_yout.lofs;
		wdr_info.pYoutInfo->uiYoutSizeH = Info.sie_rdy_yout.win_num_x;
		wdr_info.pYoutInfo->uiYoutSizeV = Info.sie_rdy_yout.win_num_y;
        wdr_info.pYoutInfo->uiOB        = iq_get_ob(Id);

		wdr_info.puiGGammaLut           = iq_get_gamma_lut(IPL_UTI_CONV2_IQS_ID(Id));

		wdr_info.uiWDRLevel             = IPL_AlgGetUIInfo(Id, IPL_SEL_WDR_M_LEVEL_IMM); //only for manual mode

		wdr_chg_par.uiChgSel                    = (SR_WDR_CHG_WDR_STRENGTH | SR_WDR_CHG_AE_TAG_Y | SR_WDR_CHG_GCE_LEVEL);
        wdr_chg_par.uiChgWDRUserSth             = (UINT32)iq_get_wdr_strength(Id); //only for auto mode
		wdr_chg_par.uiChgAeTagY                 = AE_AlgGetUIInfo(IPL_UTI_CONV2_AE_ID(Id), AE_SEL_CURR_EXPY);
        wdr_chg_par.ChgGceParam.uiGceBrightSth  = gce_param.uiGceBrightSth;
		wdr_chg_par.ChgGceParam.uiGceDarkSth    = gce_param.uiGceDarkSth;
        //DBG_DUMP("gce_param:%d,%d\r\n",wdr_chg_par.ChgGceParam.uiGceBrightSth,wdr_chg_par.ChgGceParam.uiGceDarkSth);

		rt |= SceneRender_WDR_ChgParams(IPL_UTI_CONV2_WDR_ID(Id), &wdr_chg_par);
		rt |= SceneRender_WDR_Proc(IPL_UTI_CONV2_WDR_ID(Id), &wdr_info);
	}
	if (rt != E_OK) {
		DBG_ERR("Id %u, WHDR Proc fail\r\n", Id);
        return E_SYS;
	} else {
		ExtFunc_SetParam(Id, EXTFUNC_MODE_DR);
		iq_set_flow(Id, IQS_WDR_END);
	}

    return E_OK;
}

static ER ExtFunc_ProcessDefog(UINT32 Id)
{
    IPC_QINFO           	Info			= {0};
	S_DEFOG_LIB_INIT_PARA 	defog_para		= {0};
    S_DEFOG_LIB_PARA    	defog_lib_param	= {0};

	Info = IPL_Ctrl_Query(Id, ICF_RHE_SUBOUT_RDY);

    //chg_defog_strength(UINT8 str, UINT32 sie_id); //only for manual mode
	iq_get_defog_param(Id, &defog_para);
    chg_defog_percentages(defog_para.percentages, Id); //only for auto mode
    chg_defog_lut(defog_para.auto_defog_lut, Id); //only for auto mode
    //chg_defog_mode(UINT32 mode, Id);

    defog_lib_param.p_subImg        = (UINT32 *)Info.rhe_rdy_subout;
	defog_lib_param.user_strength   = IPL_AlgGetUIInfo(Id, IPL_SEL_DEFOG_LEVEL_IMM); //only for auto mode
	defog_lib_param.blending_factor = 128;
	defog_lib_proc(Id, &defog_lib_param);

	ExtFunc_SetParam(Id, EXTFUNC_MODE_DEFOG);

    return E_OK;
}

static EXTFUNC_DR ExtFunc_GetDRMode(UINT32 Id)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) {

        if      (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_OFF)
            return EXTFUNC_DR_SHDR_OFF;

        else if (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_ON)
            return EXTFUNC_DR_SHDR_ON;

        else if (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR) == SEL_SHDR_WDR)
            return EXTFUNC_DR_SHDR_WDR;

        else
            DBG_ERR("Id %u, Get DR mode fail(%x, %u)\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN), IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR));

        return EXTFUNC_MODE_ERR;
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) {

        if      (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR_IMM) == SEL_WDR_OFF)
            return EXTFUNC_DR_WDR_OFF;

        else if (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR_IMM) == SEL_WDR_AUTO)
            return EXTFUNC_DR_WDR_ON;

        else if (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR_IMM) == SEL_WDR_MANUAL)
            return EXTFUNC_DR_WDR_ON;

        else
            DBG_ERR("Id %u, Get DR mode fail(%x, %u)\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN), IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR));

        return EXTFUNC_MODE_ERR;
    }

    return EXTFUNC_DR_NONE;
}

static EXTFUNC_DEFOG ExtFunc_GetDefogMode(UINT32 Id)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_DEFOG) {

        if      (IPL_AlgGetUIInfo(Id, IPL_SEL_DEFOG_IMM) == SEL_DEFOG_OFF)
            return EXTFUNC_DEFOG_OFF;

        else if (IPL_AlgGetUIInfo(Id, IPL_SEL_DEFOG_IMM) == SEL_DEFOG_ON)
            return EXTFUNC_DEFOG_ON;

        else
            DBG_ERR("Id %u, Get Defog mode fail(%x, %x)\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN), IPL_AlgGetUIInfo(Id, IPL_SEL_DEFOG_IMM));

        return EXTFUNC_MODE_ERR;
    }

    return EXTFUNC_DEFOG_NONE;
}

static ER ExtFunc_WaitStage(EXTFUNC_STAGE *flg, EXTFUNC_STAGE pat)
{
    UINT32 timeout_cnt;

    timeout_cnt = 0;
    while (*flg != pat) {
        if (timeout_cnt++ > 5) {
            DBG_WRN("Wait stage timeout!\r\n");
            return E_TMOUT;
        }
        Delay_DelayMs(10);
    }

    return E_OK;
}

static BOOL ExtFunc_SetTrigEn(UINT32 Id)
{
	static UINT32   frm_idx[]	= {[0 ... (IPL_ID_MAX_NUM-1)] = 1};
	UINT32          sen_fps		= DEFAULT_TRIG_FPS;
	FPS_PARAM       fps_param	= {0};
	ER              rt;

	fps_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID)), SEN_GET_FPS, (void *)&fps_param);

	if (rt == E_OK && fps_param.chg_fps != 0) {
		sen_fps = ((fps_param.cur_fps & 0xffff0000) >> 16)/(fps_param.cur_fps & 0xffff);
		if (sen_fps < DEFAULT_TRIG_FPS) {
			sen_fps = DEFAULT_TRIG_FPS;
		}
	}

	if (frm_idx[Id]%(sen_fps/DEFAULT_TRIG_FPS) == 0) {
		frm_idx[Id] = 1;
		return TRUE;
	} else {
		frm_idx[Id]++;
		return FALSE;
	}
}

static void ExtFunc_SetSHDRInfo(UINT32 Id)
{
	SHDR_UI_INFO shdr_info;
	switch (IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR))
	{
		case SEL_SHDR_ON:
        case SEL_SHDR_WDR:
			shdr_info.shdrMode = SHDR_HDR_MODE;
			break;

		case SEL_SHDR_OFF:
			shdr_info.shdrMode = SHDR_LINEAR_MODE;
			break;

		default:
			DBG_WRN("Id %u, Not Support SHDR Mode %d\r\n", Id, IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR));
			shdr_info.shdrMode = SHDR_LINEAR_MODE;
			break;
	}

	shdr_info.uiHDRLevel = IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR_LEVEL_IMM);

	SensorHDR_SetUI(&shdr_info);
}

static void ExtFunc_SetParam(UINT32 Id, EXTFUNC_MODE mode)
{
    static SR_WDR_PARAM             wdr_param;
    static SR_WDR_PATH_CURVE_PARAM	GetPathParam;   ///< path parameters
    static SR_WDR_BLEND_PARAM       GetBlendParam;  ///< blend parameters
    static SR_WDR_LCE_PARAM         GetLceParam;    ///< local contrast enhancement parameters
    static SR_WDR_GTC_PARAM         GetGtcParam;    ///< global tone curve parameters
	static IPC_IQparam              Ipc_IQ  = {0};
	static IPL_HAL_GROUP            group   = {0};
	static RHE_SUBFUNC 	            rheSubf = {0};
	static IPL_HAL_READ	            readBit = {0};

	//read previous function enable
	readBit.rheRead = RHE_R_SUBF_EN;
    IPL_Ctrl_Read(Id, IPL_HAL_RHE, IPL_CtrlGetHalSetId(Id), readBit, (void *)&rheSubf.rheFuncEn);

	Ipc_IQ.pGrp             = &group;
	Ipc_IQ.EngSubF.pRhe_sub = &rheSubf;

	if (mode == EXTFUNC_MODE_DR) {
		wdr_param.pGetPathParam     = &GetPathParam;
		wdr_param.pGetBlendParam    = &GetBlendParam;
		wdr_param.pGetLceParam      = &GetLceParam;
		wdr_param.pGetGtcParam      = &GetGtcParam;
		SceneRender_WDR_GetParam(IPL_UTI_CONV2_WDR_ID(Id), &wdr_param);

		Ipc_IQ.Idx      = IPL_CtrlGetHalSetId(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID));
		group.rheUpdate = RHE_SUBF_EN|RHE_PATH_CURVE|RHE_BLEND|RHE_TONE_CURVE|RHE_SUBIMG_FILTER;

		//ext
		rheSubf.path_curve_info.PathCurveParamMode  = RHE_HAL_PARAM_USER_MODE;
		rheSubf.path_curve_info.puiCurveA           = wdr_param.pGetPathParam->puiCurveA;
		rheSubf.path_curve_info.puiCurveB           = wdr_param.pGetPathParam->puiCurveB;
		rheSubf.path_curve_info.puiCurveC           = wdr_param.pGetPathParam->puiCurveC;
		rheSubf.path_curve_info.puiCurveD           = wdr_param.pGetPathParam->puiCurveD;
		rheSubf.path_curve_info.puiCurveE           = wdr_param.pGetPathParam->puiCurveE;

		//BlendInfo
		rheSubf.blending_info.BldParamMode          = RHE_HAL_PARAM_USER_MODE;
		rheSubf.blending_info.uiRefWet              = wdr_param.pGetBlendParam->uiBlendRefWet;
		rheSubf.blending_info.puiBldCurve           = wdr_param.pGetBlendParam->puiBlendCurve;

		//GblTnInfo
		rheSubf.tone_curve_info.TcMode              = RHE_HAL_TC_RW_MODE_DRAM;
		rheSubf.tone_curve_info.TcDataMode          = wdr_param.pGetGtcParam->GtcProcSel;
		rheSubf.tone_curve_info.puiTcCurve          = wdr_param.pGetGtcParam->puiGtcCurve;

		rheSubf.subimg_filter_info.FtrParamMode     = RHE_HAL_PARAM_AUTO_MODE;

		rheSubf.rheFuncEn                           |= (SubImgFilterEn|ImgBlendingEn|ToneCurveEn);
	}

	if (mode == EXTFUNC_MODE_DEFOG) {
		S_DEFOG_LIB_RET defog_lib_ret;
		UINT32          CAdjModLutA[9]  = {0, 32,  64,  96, 128, 160, 192, 224, 256};
		UINT32          CAdjModLutB[9]  = {0,512,1024,1536,2048,2560,3072,3584,4096};
        UINT32*         out_wet_addr    = iq_get_defog_wet(Id);;
        UINT32          out_wet[17]     = {0};
        memcpy(out_wet, out_wet_addr, sizeof(UINT32)*17);

		defog_lib_get_para(Id, &defog_lib_ret);

		Ipc_IQ.Idx      = IPL_CtrlGetHalSetId(IPL_CtrlGetInfor(Id, IPLCTRL_MF_BASE_ID));
		group.rheUpdate = RHE_SUBF_EN|RHE_COLOR_CONTRAST;

		rheSubf.contrast_adj_info.CAdjParam.CAdjMin.uiDiv                   = defog_lib_ret.RHE_MIN_DIV;
		rheSubf.contrast_adj_info.CAdjParam.CAdjMin.uiClamp                 = defog_lib_ret.RHE_MIN_CLAMP;

		rheSubf.contrast_adj_info.CAdjParam.CAdjMax.uiDiv                   = defog_lib_ret.RHE_MAX_DIV;
		rheSubf.contrast_adj_info.CAdjParam.CAdjMax.uiClamp                 = defog_lib_ret.RHE_MAX_CLAMP;

		rheSubf.contrast_adj_info.CAdjParam.CAdjEnh.uiCAdjRto               = defog_lib_ret.REH_CTADJ_RTO;
		rheSubf.contrast_adj_info.CAdjParam.CAdjEnh.uiCAdjMinDif            = defog_lib_ret.RHE_CTADJ_MIN_DIF;
		rheSubf.contrast_adj_info.CAdjParam.CAdjEnh.uiCAdjGainRto           = defog_lib_ret.RHE_CTADJ_GRATIO;
		rheSubf.contrast_adj_info.CAdjParam.CAdjEnh.CAdjOpt                 = RHE_HAL_CADJ_OPT_B;

		rheSubf.contrast_adj_info.CAdjParam.CAdjBld.puiCAdjBldRto           = &defog_lib_ret.BLDRTO[0];
		rheSubf.contrast_adj_info.CAdjParam.CAdjBld.CadjBldRef              = RHE_HAL_CADJ_BLD_REF_GAMMA;

	    rheSubf.contrast_adj_info.CAdjParam.CAdjOutCoefs.puiCAdjOutputWet   = &out_wet[0];
		rheSubf.contrast_adj_info.CAdjParam.CAdjOutCoefs.CAdjOutWetRef      = RHE_HAL_CADJ_OUT_AADJ;
		rheSubf.contrast_adj_info.CAdjParam.CAdjOutCoefs.CAdjRefGmaEn       = DISABLE;

		rheSubf.contrast_adj_info.CAdjParam.CAdjParamMode                   = RHE_HAL_PARAM_USER_MODE;
		rheSubf.contrast_adj_info.CAdjParam.puiCAdjModLutA                  = &CAdjModLutA[0];
		rheSubf.contrast_adj_info.CAdjParam.puiCAdjModLutB                  = &CAdjModLutB[0];
		rheSubf.contrast_adj_info.CAdjParam.puiCAdjGamma                    = &defog_lib_ret.RHE_YGAMMA[0];

		rheSubf.contrast_adj_info.CAdjFilter.FtrParamMode                   = RHE_HAL_PARAM_AUTO_MODE;
		rheSubf.contrast_adj_info.CAdjSclFtr.CAdjParamMode                  = RHE_HAL_PARAM_AUTO_MODE;

		rheSubf.rheFuncEn                                                   |= ContrastAdjEn;
	}

	IPL_Ctrl_Runtime_Chg_IQ(Id, IPC_RHE_IQparam, (void *)&Ipc_IQ);
}

