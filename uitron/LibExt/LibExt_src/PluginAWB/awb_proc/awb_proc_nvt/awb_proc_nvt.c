/**
    Auto white balance flow.

    awb flow.

    @file       awb_proc_nvt.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "Type.h"
#include "awb_alg.h"
#include "awb_api.h"
#include "awb_task.h"
#include "awb_proc_nvt_int.h"
#include "ae_api.h"
#include "note.h"
#include "awb_ui_info.h"
#include "iq_api.h"
#include "sensor.h"
#include "pq_obj.h"

extern UINT32 ui_r_gain[AWB_ID_MAX_NUM], ui_g_gain[AWB_ID_MAX_NUM], ui_b_gain[AWB_ID_MAX_NUM];

#include "ipl_cal_obj_drv.h"
#include "ipl_cal_rw.h"
#include "ipl_cal_api.h"

#define __MODULE__ AWB
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#include "NvtVerInfo.h"

NVTVER_VERSION_ENTRY(AWB_PROC_NVT, 1, 00, 000, 00)
/******************************/
/**
    AWB Info
*/

#define max(a,b) (((a) > (b)) ? (a) : (b))

static AWBALG_INFO awb_info[AWB_PROC_CNT] = {0};

extern UINT32 awb_uart_enable[AWB_ID_MAX_NUM];
extern UINT32 awb_keep_wb_enable[AWB_ID_MAX_NUM];

static AWB_APPMODE awb_app_mode = AWB_APPMODE_NORMAL;
static AWBALG_CT_CALMODE awb_ct_cal_mode = AWBALG_CT_CAL_Cx;
static UINT16 acc_tab[AWB_PROC_CNT][AWBALG_CH_MAX][AWB_CABUFFER_SIZE];
static UINT16 acc_cnt[AWB_PROC_CNT][AWB_CABUFFER_SIZE];
static UINT16 cal_tab[AWB_PROC_CNT][AWBALG_CH_MAX][AWB_CABUFFER_SIZE];
static UINT16 flag_tab[AWB_PROC_CNT][AWB_CABUFFER_SIZE];

static void AWB_setGain(AWBALG_INFO *Info, UINT32 RG, UINT32 GG, UINT32 BG);
static void AWB_getCA(UINT32 id, UINT16 *R, UINT16 *G, UINT16 *B, UINT16 *IR, UINT16 *Rth, UINT16 *Gth, UINT16 *Bth, UINT16 *Cnt, UINT32 Size);
static AWB_MODE AWB_getMode(UINT32 Idx);
static void AWB_autoProc(AWBALG_INFO *Info, awb_param *m1_par);
static void AWB_manualProc(AWBALG_INFO *Info, AWB_MODE Idx);
static void awb_manual_single(AWBALG_INFO *Info, AWB_MODE Idx);
static UINT16 cap_acc_tab[AWB_PROC_CNT][AWBALG_CH_MAX][AWB_CABUFFER_SIZE];
static UINT16 cap_flag_tab[AWB_PROC_CNT][AWB_CABUFFER_SIZE];


awb_param *awb_param_addr[AWB_ID_MAX_NUM] = {NULL};


////////////////////////////////////////////////////////
//
// extern function
//
////////////////////////////////////////////////////////
UINT32 AWB_GetCAInfo(UINT32 Id, awb_ca_info *CAInfo)
{
    if(awb_param_addr[Id] != NULL){
        //DBG_ERR("id=%d,CaInfo:{%d,%d},%d,%d,%d,%d,%d,%d,%d,%d\r\n",Id,awb_param_addr[Id]->ca_info->WinNumX,awb_param_addr[Id]->ca_info->WinNumY,
        //    awb_param_addr[Id]->ca_info->RGl,awb_param_addr[Id]->ca_info->RGu,awb_param_addr[Id]->ca_info->GGl,awb_param_addr[Id]->ca_info->GGu,
        //    awb_param_addr[Id]->ca_info->BGl,awb_param_addr[Id]->ca_info->BGu,awb_param_addr[Id]->ca_info->RBGl,awb_param_addr[Id]->ca_info->RBGu);
        CAInfo->WinNumX = awb_param_addr[Id]->ca_info->WinNumX;
        CAInfo->WinNumY = awb_param_addr[Id]->ca_info->WinNumY;
        CAInfo->RGl = awb_param_addr[Id]->ca_info->RGl;
        CAInfo->RGu = awb_param_addr[Id]->ca_info->RGu;
        CAInfo->GGl = awb_param_addr[Id]->ca_info->GGl;
        CAInfo->GGu = awb_param_addr[Id]->ca_info->GGu;
        CAInfo->BGl = awb_param_addr[Id]->ca_info->BGl;
        CAInfo->BGu = awb_param_addr[Id]->ca_info->BGu;
        CAInfo->RBGl = awb_param_addr[Id]->ca_info->RBGl;
        CAInfo->RBGu = awb_param_addr[Id]->ca_info->RBGu;
    }else{
        DBG_WRN("AWB param ptr is not ready, CAInfo use default value 32*32!!!");
        CAInfo->WinNumX = AWB_WIN;
        CAInfo->WinNumY = AWB_WIN;
        CAInfo->RGl = 0;
        CAInfo->RGu = 4095;
        CAInfo->GGl = 0;
        CAInfo->GGu = 4095;
        CAInfo->BGl = 0;
        CAInfo->BGu = 4095;
        CAInfo->RBGl = 0;
        CAInfo->RBGu = 4095;
    }
    return 0;
}

void AWB_SetInfo(UINT32 Id, AwbSetItem awbItem, AwbSetParam *pAWBSetting)
{
    if (Id >= AWB_PROC_CNT) {
        DBG_ERR("wrong AWB ID (%d)\r\n", Id);
        return ;
    }

    AWB_MODE Mode = AWB_MODE_AUTO;
    Mode = AWB_getMode(awb_get_ui_info(Id, AWB_UI_WB));

    switch (awbItem) {
    case AWB_SET_SKIP_FRAME:
        if (pAWBSetting->uiConvSkipFrame >= SEL_AWB_CONV_SKIP_FRAME_LEVEL_MAX_CNT) {
            DBG_ERR("convergence Skip Frame level %d > max level %d\r\n", pAWBSetting->uiConvSkipFrame, SEL_AWB_CONV_SKIP_FRAME_LV10);
            break;
        }

        awb_info[Id].AwbConvSkipFrame = pAWBSetting->uiConvSkipFrame;

        if (Mode != AWB_MODE_AUTO) {
            DBG_WRN("wb gain are fixed\r\n");
        }

        break;

    case AWB_SET_SPEED:
        if (pAWBSetting->uiConvSpeed >= SEL_AWB_CONV_SPEED_LEVEL_MAX_CNT) {
            DBG_ERR("convergence Speed level %d > max level %d\r\n", pAWBSetting->uiConvSpeed, SEL_AWB_CONV_SPEED_LV10);
            break;
        }

        awb_info[Id].AwbConvSpeed = pAWBSetting->uiConvSpeed;

        if (Mode != AWB_MODE_AUTO) {
            DBG_WRN("wb gain are fixed\r\n");
        }

        break;

    case AWB_SET_USER_RATIO:
        if(pAWBSetting->uiRratio > 400 || pAWBSetting->uiBratio > 400 || pAWBSetting->uiRratio < 1 || pAWBSetting->uiBratio < 1){
            DBG_ERR("ratio over flow, Rratio = %d, Bratio = %d\r\n", pAWBSetting->uiRratio, pAWBSetting->uiBratio);
        }

        awb_info[Id].AwbUserRratio = pAWBSetting->uiRratio;
        awb_info[Id].AwbUserBratio = pAWBSetting->uiBratio;

        break;
    case AWB_SET_GAIN:
        switch (Mode) {
        case AWB_MODE_AUTO:
            DBG_WRN("awb cannot set gain in AWB_MDOE_AUTO\r\n");
            break;

        case AWB_MODE_DAYLIGHT:
        case AWB_MODE_CLOUDY:
        case AWB_MODE_TUNGSTEN:
        case AWB_MODE_SUNSET:
        case AWB_MODE_CUSTOMER1:
        case AWB_MODE_CUSTOMER2:
        case AWB_MODE_CUSTOMER3:
        case AWB_MODE_CUSTOMER4:
        case AWB_MODE_CUSTOMER5:
        case AWB_MODE_CUSTOMER6:
        case AWB_MODE_MGAIN:
            MWB_WriteColorGain(Id, Mode, pAWBSetting->uiManualRGain, pAWBSetting->uiManualGGain, pAWBSetting->uiManualBGain);
            break;

        default:
            DBG_WRN("wrong awb mode %d \r\n", Mode);
            break;
        }

        break;

    default:
        DBG_WRN("wrong awb set item\r\n");
        break;
    }
}

void AWB_GetInfo(UINT32 Id, AwbSetParam *pAWBSetting)
{
    if (Id >= AWB_PROC_CNT) {
        DBG_ERR("wrong AWB ID (%d)\r\n", Id);
        return ;
    }

    AWB_MODE Mode = AWB_MODE_AUTO;
    Mode = AWB_getMode(awb_get_ui_info(Id, AWB_UI_WB));

    pAWBSetting->uiRratio = awb_info[Id].AwbUserRratio;
    pAWBSetting->uiBratio = awb_info[Id].AwbUserBratio;

    pAWBSetting->uiConvSkipFrame = awb_info[Id].AwbConvSkipFrame;
    pAWBSetting->uiConvSpeed = awb_info[Id].AwbConvSpeed;

    if (Mode == AWB_MODE_AUTO) {
        pAWBSetting->uiManualRGain = 256;
        pAWBSetting->uiManualGGain = 256;
        pAWBSetting->uiManualBGain = 256;
    } else {
        MWB_ReadColorGain(Id, Mode, &pAWBSetting->uiManualRGain, &pAWBSetting->uiManualGGain, &pAWBSetting->uiManualBGain);
    }
}

void AWB_GetStatus(UINT32 Id, AWBStatus *pAWBStatus)
{
    if (Id >= AWB_PROC_CNT) {
        DBG_ERR("wrong AWB ID (%d)\r\n", Id);
        return ;
    }

    pAWBStatus->ConvSkipFrame= awb_info[Id].AwbConvSkipFrame;
    pAWBStatus->ConvSpeed = awb_info[Id].AwbConvSpeed;
    pAWBStatus->UserRratio = awb_info[Id].AwbUserRratio;
    pAWBStatus->UserBratio = awb_info[Id].AwbUserBratio;

    AWB_MODE Mode = AWB_MODE_AUTO;
    Mode = AWB_getMode(awb_get_ui_info(Id, AWB_UI_WB));

    if (Mode == AWB_MODE_AUTO) {
        pAWBStatus->RGain = awb_info[Id].AWBGain.r_gain;
        pAWBStatus->GGain = awb_info[Id].AWBGain.g_gain;
        pAWBStatus->BGain = awb_info[Id].AWBGain.b_gain;
	} else {
        MWB_ReadColorGain(Id, Mode, &pAWBStatus->RGain, &pAWBStatus->GGain, &pAWBStatus->BGain);
    }

    pAWBStatus->ColorTemperature = awb_info[Id].CurCT;
}

void AWB_Init(UINT32 id, UINT32 CurrentStatus)
{
    //DBG_ERR("==========AWB_Init id=%d==========\r\n",id);

    UINT32 i;
    SENSOR_INFO SenInfo = {0};
    SEN_STATUS_PARAM sensor_status = {0};
    static BOOL Init_Flag[AWB_PROC_CNT] = {FALSE, FALSE};
    AWB_PARAM_Pr AWB_Table_Ptr = {0};
    CAL_SETTING_INFO *setting_info;
    CAL_ALG_AWB_SETTING *cal_awb_etting;

    ER rt = E_OK;
    SenInfo.mode = NULL;

    if(!pqs_awb_check(id)) {
    	UINT32 awb_init = 0;
    	pqs_awb_init(id, &awb_init);
    	awb_param_init(id, &awb_init);
    }

    if (id >= AWB_PROC_CNT) {
        DBG_ERR("wrong AWB ID (%d)\r\n", id);
        return ;
    }

    sensor_status.sensor_mode = IPL_AlgGetUIInfo(id, IPL_SEL_PRVSENMODE);
    rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(id), SEN_GET_STATUS, (void *)&sensor_status);
    SenInfo = sensor_status.sensor_info;

    if (rt != E_OK || SenInfo.mode == NULL) {
        return;
    }

    if (IPL_AlgGetUIInfo(id, IPL_SEL_SHDR) == SEL_SHDR_ON) {
        awb_app_mode = AWB_APPMODE_SHDR;
	} else {
        awb_app_mode = AWB_APPMODE_NORMAL;
    }

    awb_info[id].Id = id;
    awb_info[id].awb_single_mode = 0;

    //** AWB Calibration
    CAL_RW_HEADER_INFO rw_header_info = {0};
    UINT32 data_addr = 0;

    rw_header_info.sec.cal_item = CAL_ITEM_AWB;
    rw_header_info.key.id = id;
    rw_header_info.key.cal_cond = CAL_COND_DFT;
    rw_header_info.key.sensor_mode = IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE);

    rt =  ipl_cal_getcalrst(id, &rw_header_info, &data_addr);

    if (rt == E_OK) {
        DBG_IND("get AWB Calibration data\r\n");
        if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
            setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(id)->fp_get_setting_info(rw_header_info.sec.cal_item);
            cal_awb_etting = setting_info->alg;
            DBG_IND("cal gain[%d,%d], golden gain[%d,%d]\r\n",rw_header_info.value.data[0],rw_header_info.value.data[1],cal_awb_etting->gs.rgain,cal_awb_etting->gs.bgain);
            
            if(rw_header_info.value.data[0] != 0 && rw_header_info.value.data[1] != 0 && cal_awb_etting->gs.rgain !=0 && cal_awb_etting->gs.bgain !=0){
                awb_info[id].k_gain.gs_r_gain =  cal_awb_etting->gs.rgain;
                awb_info[id].k_gain.gs_b_gain =  cal_awb_etting->gs.bgain;
                awb_info[id].k_gain.k_r_gain =  rw_header_info.value.data[0];
                awb_info[id].k_gain.k_b_gain =  rw_header_info.value.data[1];
                awb_info[id].k_gain.r_ratio = awb_info[id].k_gain.k_r_gain * 100 / awb_info[id].k_gain.gs_r_gain;
                awb_info[id].k_gain.b_ratio = awb_info[id].k_gain.k_b_gain * 100 / awb_info[id].k_gain.gs_b_gain;
			} else {
                DBG_ERR("read AWB Calibration data Fail\r\n");
                awb_info[id].k_gain.r_ratio = AWB_DEF_KRGAIN;
                awb_info[id].k_gain.b_ratio = AWB_DEF_KBGAIN;
            }
            DBG_IND("AWB Cal Rg=%d, Bg=%d, SenMode=%d\r\n",rw_header_info.value.data[0],rw_header_info.value.data[1], IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE));
        } else {
            DBG_IND("cal status not OK\r\n");
            awb_info[id].k_gain.r_ratio = AWB_DEF_KRGAIN;
            awb_info[id].k_gain.b_ratio = AWB_DEF_KBGAIN;
        }
	} else {
        DBG_IND("NO AWB cal data\r\n");
        awb_info[id].k_gain.r_ratio = AWB_DEF_KRGAIN;
        awb_info[id].k_gain.b_ratio = AWB_DEF_KBGAIN;
    }
    //****

    awb_info[id].status = AWBALG_STATUS_INIT;
    awb_info[id].ob = iq_get_ob(id);

    if (Init_Flag[id] == FALSE) {
        Init_Flag[id] = TRUE;

        for(i = 0;i < AWB_TUNING_CT_MAX;i++)
        {
            if((awb_param_addr[id]->ct_table+i)->temperature>4000 && (awb_param_addr[id]->ct_table+i)->temperature<6000)
            {
                awb_info[id].DefGain.r_gain = (((awb_param_addr[id]->ct_table+i)->gain.r_gain * awb_info[id].k_gain.r_ratio)<< 12) / (4096 - awb_info[id].ob) / AWB_BASE_KGAIN;
                awb_info[id].DefGain.g_gain = (256 << 12) / (4096 - awb_info[id].ob);
                awb_info[id].DefGain.b_gain = (((awb_param_addr[id]->ct_table+i)->gain.b_gain * awb_info[id].k_gain.b_ratio)<< 12) / (4096 - awb_info[id].ob) / AWB_BASE_KGAIN;
                break;
            }
        }
        if(awb_info[id].DefGain.r_gain==0 || awb_info[id].DefGain.g_gain==0 || awb_info[id].DefGain.b_gain==0)
        {
            awb_info[id].DefGain.r_gain = (((awb_param_addr[id]->ct_table+3)->gain.r_gain * awb_info[id].k_gain.r_ratio)<< 12) / (4096 - awb_info[id].ob) / AWB_BASE_KGAIN;
            awb_info[id].DefGain.g_gain = (256 << 12) / (4096 - awb_info[id].ob);
            awb_info[id].DefGain.b_gain = (((awb_param_addr[id]->ct_table+3)->gain.b_gain * awb_info[id].k_gain.b_ratio)<< 12) / (4096 - awb_info[id].ob) / AWB_BASE_KGAIN;
        }

	} else {
        awb_info[id].DefGain = awb_info[id].AWBGain;
    }
    awb_info[id].PreGain = awb_info[id].DefGain;
    awb_info[id].AWBGain = awb_info[id].DefGain;
    awb_info[id].cap_gain = awb_info[id].DefGain;
    awb_info[id].WinNumX = awb_param_addr[id]->ca_info->WinNumX;
    awb_info[id].WinNumY = awb_param_addr[id]->ca_info->WinNumY;
    awb_info[id].acc_tab_type = AWB_CA_TYPE_NORMAL;
    awb_info[id].AccTab[AWBALG_CH_R] = acc_tab[id][AWBALG_CH_R];
    awb_info[id].AccTab[AWBALG_CH_G] = acc_tab[id][AWBALG_CH_G];
    awb_info[id].AccTab[AWBALG_CH_B] = acc_tab[id][AWBALG_CH_B];
    awb_info[id].AccTab[AWBALG_CH_IR] = acc_tab[id][AWBALG_CH_IR];
    awb_info[id].AccTab[AWBALG_CH_Rth] = acc_tab[id][AWBALG_CH_Rth];
    awb_info[id].AccTab[AWBALG_CH_Gth] = acc_tab[id][AWBALG_CH_Gth];
    awb_info[id].AccTab[AWBALG_CH_Bth] = acc_tab[id][AWBALG_CH_Bth];
    awb_info[id].AccCnt = acc_cnt[id];
    awb_info[id].CalTab[AWBALG_CH_R] = cal_tab[id][AWBALG_CH_R];
    awb_info[id].CalTab[AWBALG_CH_G] = cal_tab[id][AWBALG_CH_G];
    awb_info[id].CalTab[AWBALG_CH_B] = cal_tab[id][AWBALG_CH_B];
    awb_info[id].CalTab[AWBALG_CH_IR] = cal_tab[id][AWBALG_CH_IR];
    awb_info[id].CalTab[AWBALG_CH_Rth] = cal_tab[id][AWBALG_CH_Rth];
    awb_info[id].CalTab[AWBALG_CH_Gth] = cal_tab[id][AWBALG_CH_Gth];
    awb_info[id].CalTab[AWBALG_CH_Bth] = cal_tab[id][AWBALG_CH_Bth];
    awb_info[id].FlagTab = flag_tab[id];
    awb_info[id].StableGain.tolerance = awb_param_addr[id]->converge->tolerance;  //268/256=5%
    awb_info[id].StableGain.score = 0;
    awb_info[id].StableGain.r_gain = awb_info[id].DefGain.r_gain;
    awb_info[id].StableGain.g_gain = 256;
    awb_info[id].StableGain.b_gain = awb_info[id].DefGain.b_gain;

    awb_info[id].keep_wb.en = DISABLE;
    awb_info[id].keep_wb.ct_number = 4;//0:2300 1:2800 2:3700 3:4700 4:6500 5:11000

    awb_info[id].expand_ct_en = ENABLE;

    awb_info[id].multi_ct_check_en = DISABLE;
    awb_info[id].multi_ct_check_min_sat = 50; //range:0~80
    awb_info[id].saturation = 100;

    awb_info[id].AwbUserRratio = 100;
    awb_info[id].AwbUserBratio = 100;


    awb_info[id].AwbConvSkipFrame = awb_param_addr[id]->converge->skip_frame;
    awb_info[id].AwbConvSpeed = awb_param_addr[id]->converge->speed;
    if (awb_info[id].AwbConvSkipFrame > 10)awb_info[id].AwbConvSkipFrame = 10;
    if (awb_info[id].AwbConvSkipFrame < 1)awb_info[id].AwbConvSkipFrame = 1;
    if (awb_info[id].AwbConvSpeed > 50)awb_info[id].AwbConvSpeed = 50;
    if (awb_info[id].AwbConvSpeed < 1)awb_info[id].AwbConvSpeed = 1;

    awb_info[id].MinWTotal = AWB_W_THRES;

    awb_info[id].AWBCnt = 0;
    awb_info[id].AWBEnterCnt = 0;
    awb_info[id].AwbWeightEn = TRUE;

    AWB_Table_Ptr.Now_AWB_Info = &awb_info[id];
    AWB_Table_Ptr.Now_AWB_Par = awb_param_addr[id];
    awb_set_addr(id, AWB_Table_Ptr);

    CA_DumpFP(AWB_DumpCAInfo);
    Flag_DumpFP(AWB_DumpFlagInfo);

    isp_awb_init(id);
}

void AWB_Process(UINT32 id, UINT32 CurrentStatus)
{
    AWB_MODE Mode;
    UINT32 AWB_WIN_X = awb_info[id].WinNumX;
    UINT32 AWB_WIN_Y = awb_info[id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;
    awb_info[id].AWBCnt ++;
    awb_info[id].awb_single_mode = 0;

    if (AWB_SKIP_FRAME < 1) {
        DBG_ERR("AWB_SKIP_FRAME = %d, AWB must skip at least a frame to avoid garbage\r\n", AWB_SKIP_FRAME);
    }

    //if ((0 != (awb_info[id].AWBCnt % AWB_RATE)) && ((awb_info[id].AWBCnt > AWB_RATE) || (awb_info[id].AWBCnt <= AWB_SKIP_FRAME)))
    if ((0 != (awb_info[id].AWBCnt % awb_info[id].AwbConvSkipFrame)) && ((awb_info[id].AWBCnt > awb_info[id].AwbConvSkipFrame) || (awb_info[id].AWBCnt <= AWB_SKIP_FRAME))) {
        return;
	} else {
        // set awb weighting previous frame enable flag
        awb_info[id].AWBEnterCnt++;

        if (awb_info[id].AWBEnterCnt <= AWB_WEIGHT_SKIP_FRAME) {
            awb_info[id].AwbWeightEn = FALSE;
		} else {
            awb_info[id].AwbWeightEn = TRUE;
        }

        DBG_IND("AWB_Process in %d  enter in %d\r\n", awb_info[id].AWBCnt, awb_info[id].AWBEnterCnt);
    }

    Mode = AWB_getMode(awb_get_ui_info(id, AWB_UI_WB));

    if(awb_info[id].AWBCnt < 50){
        awb_info[id].expand_ct_en = ENABLE;
    } else {
        awb_info[id].expand_ct_en = DISABLE;
    }

    // for uart cmd debug
    if (ui_r_gain[id] != 0 && ui_g_gain[id] != 0 && ui_b_gain[id] != 0) {
        AWB_setGain(&awb_info[id], ui_r_gain[id], ui_g_gain[id], ui_b_gain[id]);
        iq_set_awb2sat_ratio(id,100);
        awb_info[id].status = AWBALG_STATUS_MANUAL;
	} else if (Mode == AWB_MODE_AUTO) {
        awb_info[id].CurLV = AE_AlgGetUIInfo(id, AE_SEL_PRV_LV);
        awb_info[id].ob = iq_get_ob(id);
        AWB_getCA(id,
                  awb_info[id].AccTab[AWBALG_CH_R],
                  awb_info[id].AccTab[AWBALG_CH_G],
                  awb_info[id].AccTab[AWBALG_CH_B],
                  awb_info[id].AccTab[AWBALG_CH_IR],
                  awb_info[id].AccTab[AWBALG_CH_Rth],
                  awb_info[id].AccTab[AWBALG_CH_Gth],
                  awb_info[id].AccTab[AWBALG_CH_Bth],
                  awb_info[id].AccCnt,
                  AWB_WIN_TOTAL);

        AWB_autoProc(&awb_info[id], awb_param_addr[id]);
        AWB_GetCurCT(id, awb_ct_cal_mode, awb_info[id].AWBGain.r_gain, awb_info[id].AWBGain.g_gain, awb_info[id].AWBGain.b_gain, &awb_info[id].CurCT);

        awb_info[id].AWBGain.r_gain = awb_info[id].AWBGain.r_gain * awb_info[id].AwbUserRratio / 100;
        awb_info[id].AWBGain.b_gain = awb_info[id].AWBGain.b_gain * awb_info[id].AwbUserBratio / 100;

        if (awb_info[id].AWBGain.r_gain > 2047) awb_info[id].AWBGain.r_gain = 2047;
        if (awb_info[id].AWBGain.b_gain > 2047) awb_info[id].AWBGain.b_gain = 2047;
        if (awb_info[id].AWBGain.r_gain < 1) awb_info[id].AWBGain.r_gain = 1;
        if (awb_info[id].AWBGain.b_gain < 1) awb_info[id].AWBGain.b_gain = 1;

        AWB_setGain(&awb_info[id], awb_info[id].AWBGain.r_gain, awb_info[id].AWBGain.g_gain, awb_info[id].AWBGain.b_gain);
        DBG_IND("sat=%d\r\n",awb_info[id].saturation);
        iq_set_awb2sat_ratio(id,awb_info[id].saturation);
        awb_info[id].DefGain = awb_info[id].AWBGain;
	} else {
        AWB_manualProc(&awb_info[id], Mode);
    }

    if ((awb_info[id].AWBCnt % 120) == 0) {
        if (awb_uart_enable[id] == ENABLE) {
            DBG_ERR("AWB ID = %d AWBCnt = %d, GAIN = (%3d %3d %3d), Saturation = %d, Stasus = %d (0:init, 1:adjust, 2:freeze, 3:converge, 4:manual)\r\n"
                , id, awb_info[id].AWBCnt, awb_info[id].AWBGain.r_gain, awb_info[id].AWBGain.g_gain, awb_info[id].AWBGain.b_gain, awb_info[id].saturation, awb_info[id].status);
        }
    }

    if ((awb_info[id].AWBCnt % 20) == 0) {
        if (awb_keep_wb_enable[id] == ENABLE) {
            DBG_ERR("AWB ID = %d AWBCnt = %d, keep wb emable = %d, keep ct number = %d\r\n", id,
                awb_info[id].AWBCnt, awb_info[id].keep_wb.en, awb_info[id].keep_wb.ct_number);
        }
    }
}

void awb_single(UINT32 id)
{
    AWB_MODE Mode;
    UINT32 AWB_WIN_X = awb_info[id].WinNumX;
    UINT32 AWB_WIN_Y = awb_info[id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;
    UINT32 i;
    awb_info[id].awb_single_mode = 1;

    Mode = AWB_getMode(awb_get_ui_info(id, AWB_UI_WB));

    // for uart cmd debug
    if (ui_r_gain[id] != 0 && ui_g_gain[id] != 0 && ui_b_gain[id] != 0) {
        iq_set_awb2sat_ratio(id,100);
        awb_info[id].cap_gain.r_gain = ui_r_gain[id];
        awb_info[id].cap_gain.g_gain = ui_g_gain[id];
        awb_info[id].cap_gain.b_gain = ui_b_gain[id];
        awb_info[id].status = AWBALG_STATUS_MANUAL;
	} else if (Mode == AWB_MODE_AUTO) {
        awb_info[id].CurLV = AE_AlgGetUIInfo(id, AE_SEL_CAP_LV);
        awb_info[id].ob = iq_get_ob(id);
        AWB_getCA(id,
                  awb_info[id].AccTab[AWBALG_CH_R],
                  awb_info[id].AccTab[AWBALG_CH_G],
                  awb_info[id].AccTab[AWBALG_CH_B],
                  awb_info[id].AccTab[AWBALG_CH_IR],
                  awb_info[id].AccTab[AWBALG_CH_Rth],
                  awb_info[id].AccTab[AWBALG_CH_Gth],
                  awb_info[id].AccTab[AWBALG_CH_Bth],
                  awb_info[id].AccCnt,
                  AWB_WIN_TOTAL);

        AWB_autoProc(&awb_info[id], awb_param_addr[id]);
        for (i = 0; i < AWB_WIN_TOTAL; i++){
             cap_flag_tab[id][i] = awb_info[id].FlagTab[i];
        }
        AWB_GetCurCT(id, awb_ct_cal_mode, awb_info[id].cap_gain.r_gain, awb_info[id].cap_gain.g_gain, awb_info[id].cap_gain.b_gain, &awb_info[id].CurCT);

        awb_info[id].cap_gain.r_gain = awb_info[id].cap_gain.r_gain * awb_info[id].AwbUserRratio / 100;
        awb_info[id].cap_gain.b_gain = awb_info[id].cap_gain.b_gain * awb_info[id].AwbUserBratio / 100;

        if (awb_info[id].cap_gain.r_gain > 2047) awb_info[id].cap_gain.r_gain = 2047;
        if (awb_info[id].cap_gain.b_gain > 2047) awb_info[id].cap_gain.b_gain = 2047;
        if (awb_info[id].cap_gain.r_gain < 1) awb_info[id].cap_gain.r_gain = 1;
        if (awb_info[id].cap_gain.b_gain < 1) awb_info[id].cap_gain.b_gain = 1;

        DBG_IND("sat=%d\r\n",awb_info[id].saturation);
        iq_set_awb2sat_ratio(id,awb_info[id].saturation);

	} else {
        awb_manual_single(&awb_info[id], Mode);
    }

    if (awb_uart_enable[id] == ENABLE) {
        DBG_ERR("AWB ID = %d AWBCnt = %d, CAP GAIN = (%3d %3d %3d), Stasus = %d (0:init, 1:adjust, 2:freeze, 3:converge, 4:manual)\r\n"
            , id, 1, awb_info[id].cap_gain.r_gain, awb_info[id].cap_gain.g_gain, awb_info[id].cap_gain.b_gain, awb_info[id].status);
    }
}

void AWB_SetColorGain(UINT32 id, UINT32 Rg, UINT32 Gg, UINT32 Bg)
{
    AWB_setGain(&awb_info[id], Rg, Gg, Bg);
    iq_set_awb2sat_ratio(id, 100);
}

void AWB_GetColorGain(UINT32 id, UINT32 *Rg, UINT32 *Gg, UINT32 *Bg)
{
    UINT32 AwbId = id;
    UINT32 i;

    if (awb_app_mode == AWB_APPMODE_SHDR) {
        AwbId = AWB_ID_1;
    }
    if((awb_param_addr[id] != NULL)&&(iq_get_ui_info(id, IQ_UI_IRCUT) == IQ_IRCUT_DAY)){
        if ((awb_info[AwbId].AWBGain.r_gain == 0) || (awb_info[AwbId].AWBGain.g_gain == 0) || (awb_info[AwbId].AWBGain.b_gain == 0)) {
            for(i = 0;i < AWB_TUNING_CT_MAX;i++)
            {
                if((awb_param_addr[id]->ct_table+i)->temperature>4000 && (awb_param_addr[id]->ct_table+i)->temperature<6000)
                {
                    *Rg = (awb_param_addr[id]->ct_table+i)->gain.r_gain * awb_info[id].k_gain.r_ratio / AWB_BASE_KGAIN;
                    *Gg = 256;
                    *Bg = (awb_param_addr[id]->ct_table+i)->gain.b_gain * awb_info[id].k_gain.b_ratio / AWB_BASE_KGAIN;
                    break;
                }
            }
            if(awb_info[id].DefGain.r_gain==0 || awb_info[id].DefGain.g_gain==0 || awb_info[id].DefGain.b_gain==0)
            {
                *Rg = (awb_param_addr[id]->ct_table+3)->gain.r_gain * awb_info[id].k_gain.r_ratio / AWB_BASE_KGAIN;
                *Gg = 256;
                *Bg = (awb_param_addr[id]->ct_table+3)->gain.b_gain * awb_info[id].k_gain.b_ratio / AWB_BASE_KGAIN;
            }
    	} else {
            *Rg = awb_info[AwbId].AWBGain.r_gain;
            *Gg = awb_info[AwbId].AWBGain.g_gain;
            *Bg = awb_info[AwbId].AWBGain.b_gain;
        }
    }else{
    DBG_IND("Night mode or NULL\r\n");
        *Rg = 256;
        *Gg = 256;
        *Bg = 256;
    }
}

void awb_get_cap_color_gain(UINT32 id, UINT32 *Rg, UINT32 *Gg, UINT32 *Bg)
{
    *Rg = awb_info[id].cap_gain.r_gain;
    *Gg = awb_info[id].cap_gain.g_gain;
    *Bg = awb_info[id].cap_gain.b_gain;
}

UINT32 AWB_WriteDebugInfo(UINT32 id, UINT32 Addr)
{
    UINT32 i, j;
    UINT32 AWB_WIN_X = awb_info[id].WinNumX;
    UINT32 AWB_WIN_Y = awb_info[id].WinNumY;
    UINT32 buf_cnt = 0;
    buf_cnt = snprintf(NULL, 0, "Kgain = %ld\t%ld\r\n", awb_info[id].k_gain.r_ratio, awb_info[id].k_gain.b_ratio);
    Addr += snprintf((char *)Addr, buf_cnt + 1, "Kgain = %ld\t%ld\r\n", awb_info[id].k_gain.r_ratio, awb_info[id].k_gain.b_ratio);
    buf_cnt = snprintf(NULL, 0, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", awb_info[id].AWBGain.r_gain, awb_info[id].AWBGain.g_gain, awb_info[id].AWBGain.b_gain);
    Addr += snprintf((char *)Addr, buf_cnt + 1, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", awb_info[id].AWBGain.r_gain, awb_info[id].AWBGain.g_gain, awb_info[id].AWBGain.b_gain);
    buf_cnt = snprintf(NULL, 0, "Flag Table:\r\n");
    Addr += snprintf((char *)Addr, buf_cnt + 1, "Flag Table:\r\n");

    for (j = 0; j < AWB_WIN_Y; j++) {
        buf_cnt = snprintf(NULL, 0, "F  \t: ");
        Addr += snprintf((char *)Addr, buf_cnt + 1, "F  \t: ");

        for (i = 0; i < AWB_WIN_X; i++) {
            buf_cnt = snprintf(NULL, 0, "%d\t", awb_info[id].FlagTab[i + (j * AWB_WIN_X)]);
            Addr += snprintf((char *)Addr, buf_cnt + 1, "%d\t", awb_info[id].FlagTab[i + (j * AWB_WIN_X)]);
        }

        buf_cnt = snprintf(NULL, 0, "\r\n");
        Addr += snprintf((char *)Addr, buf_cnt + 1, "\r\n");
    }


    buf_cnt = snprintf(NULL, 0, "\t");
    Addr += snprintf((char *)Addr, buf_cnt + 1, "\t");

    for (j = 0; j < 10; j++) {
        buf_cnt = snprintf(NULL, 0, "-");
        Addr += snprintf((char *)Addr, buf_cnt + 1, "-");
    }

    buf_cnt = snprintf(NULL, 0, "\r\n");
    Addr += snprintf((char *)Addr, buf_cnt + 1, "\r\n");

    //#NT#2016/11/01#Jarkko Chang -begin
    //#NT# code arrange
    for (j = 0; j < AWB_WIN_Y; j++) {
        buf_cnt = snprintf(NULL, 0, "RGB(10b) \t: ");
        Addr += snprintf((char *)Addr, buf_cnt + 1, "RGB(10b) \t: ");

        for (i = 0; i < AWB_WIN_X; i++) {
            buf_cnt = snprintf(NULL, 0, "%d %d %d\t", cap_acc_tab[id][AWBALG_CH_R][j * AWB_WIN_Y + i] >> 2, cap_acc_tab[id][AWBALG_CH_G][j * AWB_WIN_Y + i] >> 2, cap_acc_tab[id][AWBALG_CH_B][j * AWB_WIN_Y + i] >> 2);
            Addr += snprintf((char *)Addr, buf_cnt + 1, "%d %d %d\t", cap_acc_tab[id][AWBALG_CH_R][j * AWB_WIN_Y + i] >> 2, cap_acc_tab[id][AWBALG_CH_G][j * AWB_WIN_Y + i] >> 2, cap_acc_tab[id][AWBALG_CH_B][j * AWB_WIN_Y + i] >> 2);
        }

        buf_cnt = snprintf(NULL, 0, "\r\n");
        Addr += snprintf((char *)Addr, buf_cnt + 1, "\r\n");
    }

    return Addr;
}

ER MWB_CalcCustomerColorGain(UINT32 id, AWB_WB mode, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain)
{
    DBG_ERR("not ready\r\n");
    return E_OK;
}



void MWB_WriteColorGain(UINT32 id, AWB_WB mode, UINT32 Rg, UINT32 Gg, UINT32 Bg)
{
    AWB_MODE AWBMode = AWB_getMode((UINT32)mode);

    if ((Rg > 2048) || (Gg > 2048) || (Bg > 2048)) {
        DBG_ERR("MWB gain is out of range 2048(Rg %d Gg %d Bg %d)\r\n", Rg, Gg, Bg);
		} else {
        (awb_param_addr[id]->mwb_tab+AWBMode)->r_gain = Rg;
        (awb_param_addr[id]->mwb_tab+AWBMode)->g_gain = Gg;
        (awb_param_addr[id]->mwb_tab+AWBMode)->b_gain = Bg;
    }

}

void MWB_ReadColorGain(UINT32 id, AWB_WB mode, UINT32 *Rg, UINT32 *Gg, UINT32 *Bg)
{
    AWB_MODE AWBMode = AWB_getMode((UINT32)mode);
    *Rg = (awb_param_addr[id]->mwb_tab+AWBMode)->r_gain;
    *Gg = (awb_param_addr[id]->mwb_tab+AWBMode)->g_gain;
    *Bg = (awb_param_addr[id]->mwb_tab+AWBMode)->b_gain;
}

////////////////////////////////////////////////////////
//
// static function
//
////////////////////////////////////////////////////////
static void AWB_setGain(AWBALG_INFO *Info, UINT32 RG, UINT32 GG, UINT32 BG)
{
    Info->AWBGain.r_gain = RG;
    Info->AWBGain.g_gain = GG;
    Info->AWBGain.b_gain = BG;
    iq_set_flow(Info->Id, IQS_AWB_END);
}
static void AWB_getCA(UINT32 id, UINT16 *R, UINT16 *G, UINT16 *B, UINT16 *IR, UINT16 *Rth, UINT16 *Gth, UINT16 *Bth, UINT16 *Cnt, UINT32 Size)
{
    UINT16 i, j;
    IPL_CA_INFOR CA1Info, CA2Info, CA3Info;
    UINT32 AWB_WIN_X = awb_info[id].WinNumX;
    UINT32 AWB_WIN_Y = awb_info[id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;
    UINT32 m_cnt;

    if (awb_app_mode == AWB_APPMODE_NORMAL) {
        CA1Info.id = id;
        CA1Info.r = R;
        CA1Info.g = G;
        CA1Info.b = B;
        CA1Info.ir = IR;
        CA1Info.r_th = Rth;
        CA1Info.g_th = Gth;
        CA1Info.b_th = Bth;
        CA1Info.acc_cnt = Cnt;
        CA1Info.length = AWB_WIN_TOTAL;
        CA1Info.pnext = NULL;
        IPL_GetCmd(IPL_GET_CA_CUR_INFOR, (void *)&CA1Info);
        awb_info[id].acc_tab_type = AWB_CA_TYPE_NORMAL;

        for (i = 0; i < AWB_WIN_TOTAL; i++) {
            CA1Info.r[i] = (CA1Info.r[i] >> (12 - AWB_ACC_BIT));
            CA1Info.g[i] = (CA1Info.g[i] >> (12 - AWB_ACC_BIT));
            CA1Info.b[i] = (CA1Info.b[i] >> (12 - AWB_ACC_BIT));
            CA1Info.ir[i] = (CA1Info.ir[i] >> (12 - AWB_ACC_BIT));
            CA1Info.r_th[i] = (CA1Info.r_th[i] >> (12 - AWB_ACC_BIT));
            CA1Info.g_th[i] = (CA1Info.g_th[i] >> (12 - AWB_ACC_BIT));
            CA1Info.b_th[i] = (CA1Info.b_th[i] >> (12 - AWB_ACC_BIT));

            if (CA1Info.acc_cnt[i] == 0) {
                CA1Info.acc_cnt[i] = 1;
            }
        }
    } else if (awb_app_mode == AWB_APPMODE_STITCH && id == AWB_ID_1) {
        if (awb_param_addr[AWB_ID_1]->ca_info->WinNumX != awb_param_addr[AWB_ID_2]->ca_info->WinNumX ||
            awb_param_addr[AWB_ID_1]->ca_info->WinNumY != awb_param_addr[AWB_ID_2]->ca_info->WinNumY) {
            DBG_ERR("Wrong CA win size for STITCH mode\r\n");
        }
        awb_info[id].acc_tab_type = AWB_CA_TYPE_NORMAL;

        CA1Info.id = IPL_ID_1;
        CA1Info.r = acc_tab[AWB_ID_1][AWBALG_CH_R];
        CA1Info.g = acc_tab[AWB_ID_1][AWBALG_CH_G];
        CA1Info.b = acc_tab[AWB_ID_1][AWBALG_CH_B];
        CA1Info.ir = acc_tab[AWB_ID_1][AWBALG_CH_IR];
        CA1Info.r_th = acc_tab[AWB_ID_1][AWBALG_CH_Rth];
        CA1Info.g_th = acc_tab[AWB_ID_1][AWBALG_CH_Gth];
        CA1Info.b_th = acc_tab[AWB_ID_1][AWBALG_CH_Bth];
        CA1Info.acc_cnt = acc_cnt[AWB_ID_1];
        CA1Info.length = AWB_WIN_TOTAL;
        CA1Info.pnext = NULL;
        IPL_GetCmd(IPL_GET_CA_CUR_INFOR, (void *)&CA1Info);
        CA2Info.id = IPL_ID_2;
        CA2Info.r = acc_tab[AWB_ID_2][AWBALG_CH_R];
        CA2Info.g = acc_tab[AWB_ID_2][AWBALG_CH_G];
        CA2Info.b = acc_tab[AWB_ID_2][AWBALG_CH_B];
        CA2Info.ir = acc_tab[AWB_ID_2][AWBALG_CH_IR];
        CA2Info.r_th = acc_tab[AWB_ID_2][AWBALG_CH_Rth];
        CA2Info.g_th = acc_tab[AWB_ID_2][AWBALG_CH_Gth];
        CA2Info.b_th = acc_tab[AWB_ID_2][AWBALG_CH_Bth];
        CA2Info.acc_cnt = acc_cnt[AWB_ID_2];
        CA2Info.length = AWB_WIN_TOTAL;
        CA2Info.pnext = NULL;
        IPL_GetCmd(IPL_GET_CA_CUR_INFOR, (void *)&CA2Info);

        for (j = 0; j < AWB_WIN_Y; j++) {
            UINT32 LineHead = j * AWB_WIN_X;

            for (i = 0; i < (AWB_WIN_X >> 1); i++) {
                R[LineHead + i] = (CA1Info.r[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                G[LineHead + i] = (CA1Info.g[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                B[LineHead + i] = (CA1Info.b[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                IR[LineHead + i] = (CA1Info.ir[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Rth[LineHead + i] = (CA1Info.r_th[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Gth[LineHead + i] = (CA1Info.g_th[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Bth[LineHead + i] = (CA1Info.b_th[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Cnt[LineHead + i] = (CA1Info.acc_cnt[LineHead + i * 2]);
            }

            for (i = (AWB_WIN_X >> 1); i < AWB_WIN_X; i++) {
                R[LineHead + i] = (CA2Info.r[LineHead + i * 2 - AWB_WIN_X] >> (12 - AWB_ACC_BIT));
                G[LineHead + i] = (CA2Info.g[LineHead + i * 2 - AWB_WIN_X] >> (12 - AWB_ACC_BIT));
                B[LineHead + i] = (CA2Info.b[LineHead + i * 2 - AWB_WIN_X] >> (12 - AWB_ACC_BIT));
                IR[LineHead + i] = (CA2Info.ir[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Rth[LineHead + i] = (CA2Info.r_th[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Gth[LineHead + i] = (CA2Info.g_th[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Bth[LineHead + i] = (CA2Info.b_th[LineHead + i * 2] >> (12 - AWB_ACC_BIT));
                Cnt[LineHead + i] = (CA2Info.acc_cnt[LineHead + i * 2 - AWB_WIN_X]);
            }
        }
    } else if (awb_app_mode == AWB_APPMODE_SHDR && id == AWB_ID_1) {
        awb_info[id].acc_tab_type= AWB_CA_TYPE_THRESHOLD;

        CA1Info.id = IPL_ID_1;
        CA1Info.r = acc_tab[AWB_ID_1][AWBALG_CH_R];
        CA1Info.g = acc_tab[AWB_ID_1][AWBALG_CH_G];
        CA1Info.b = acc_tab[AWB_ID_1][AWBALG_CH_B];
        CA1Info.ir = acc_tab[AWB_ID_1][AWBALG_CH_IR];
        CA1Info.r_th = acc_tab[AWB_ID_1][AWBALG_CH_Rth];
        CA1Info.g_th = acc_tab[AWB_ID_1][AWBALG_CH_Gth];
        CA1Info.b_th = acc_tab[AWB_ID_1][AWBALG_CH_Bth];
        CA1Info.acc_cnt = acc_cnt[AWB_ID_1];
        CA1Info.length = AWB_WIN_TOTAL;
        CA1Info.pnext = NULL;
        IPL_GetCmd(IPL_GET_CA_CUR_INFOR, (void *)&CA1Info);

        CA2Info.id = IPL_ID_2;
        CA2Info.r = acc_tab[AWB_ID_2][AWBALG_CH_R];
        CA2Info.g = acc_tab[AWB_ID_2][AWBALG_CH_G];
        CA2Info.b = acc_tab[AWB_ID_2][AWBALG_CH_B];
        CA2Info.ir = acc_tab[AWB_ID_2][AWBALG_CH_IR];
        CA2Info.r_th = acc_tab[AWB_ID_2][AWBALG_CH_Rth];
        CA2Info.g_th = acc_tab[AWB_ID_2][AWBALG_CH_Gth];
        CA2Info.b_th = acc_tab[AWB_ID_2][AWBALG_CH_Bth];
        CA2Info.acc_cnt = acc_cnt[AWB_ID_2];
        CA2Info.length = AWB_WIN_TOTAL;
        CA2Info.pnext = NULL;
        IPL_GetCmd(IPL_GET_CA_CUR_INFOR, (void *)&CA2Info);

        CA3Info.id = IPL_ID_3;
        CA3Info.r = acc_tab[AWB_ID_3][AWBALG_CH_R];
        CA3Info.g = acc_tab[AWB_ID_3][AWBALG_CH_G];
        CA3Info.b = acc_tab[AWB_ID_3][AWBALG_CH_B];
        CA3Info.ir = acc_tab[AWB_ID_3][AWBALG_CH_IR];
        CA3Info.r_th = acc_tab[AWB_ID_3][AWBALG_CH_Rth];
        CA3Info.g_th = acc_tab[AWB_ID_3][AWBALG_CH_Gth];
        CA3Info.b_th = acc_tab[AWB_ID_3][AWBALG_CH_Bth];
        CA3Info.acc_cnt = acc_cnt[AWB_ID_3];
        CA3Info.length = AWB_WIN_TOTAL;
        CA3Info.pnext = NULL;
        IPL_GetCmd(IPL_GET_CA_CUR_INFOR, (void *)&CA3Info);

        for (j = 0; j < AWB_WIN_TOTAL; j++) {
            m_cnt = max(max(CA1Info.acc_cnt[j],CA2Info.acc_cnt[j]),CA3Info.acc_cnt[j]);
            if (CA1Info.acc_cnt[j] == m_cnt) {
                R[j] = (CA1Info.r[j] >> (12 - AWB_ACC_BIT));
                G[j] = (CA1Info.g[j] >> (12 - AWB_ACC_BIT));
                B[j] = (CA1Info.b[j] >> (12 - AWB_ACC_BIT));
                IR[j] = (CA1Info.ir[j] >> (12 - AWB_ACC_BIT));
                Rth[j] = (CA1Info.r_th[j] >> (12 - AWB_ACC_BIT));
                Gth[j] = (CA1Info.g_th[j] >> (12 - AWB_ACC_BIT));
                Bth[j] = (CA1Info.b_th[j] >> (12 - AWB_ACC_BIT));
                Cnt[j] = (CA1Info.acc_cnt[j]);
            } else if(CA2Info.acc_cnt[j] == m_cnt){
                R[j] = (CA2Info.r[j] >> (12 - AWB_ACC_BIT));
                G[j] = (CA2Info.g[j] >> (12 - AWB_ACC_BIT));
                B[j] = (CA2Info.b[j] >> (12 - AWB_ACC_BIT));
                IR[j] = (CA2Info.ir[j] >> (12 - AWB_ACC_BIT));
                Rth[j] = (CA2Info.r_th[j] >> (12 - AWB_ACC_BIT));
                Gth[j] = (CA2Info.g_th[j] >> (12 - AWB_ACC_BIT));
                Bth[j] = (CA2Info.b_th[j] >> (12 - AWB_ACC_BIT));
                Cnt[j] = (CA2Info.acc_cnt[j]);
            }else {
                R[j] = (CA3Info.r[j] >> (12 - AWB_ACC_BIT));
                G[j] = (CA3Info.g[j] >> (12 - AWB_ACC_BIT));
                B[j] = (CA3Info.b[j] >> (12 - AWB_ACC_BIT));
                IR[j] = (CA3Info.ir[j] >> (12 - AWB_ACC_BIT));
                Rth[j] = (CA3Info.r_th[j] >> (12 - AWB_ACC_BIT));
                Gth[j] = (CA3Info.g_th[j] >> (12 - AWB_ACC_BIT));
                Bth[j] = (CA3Info.b_th[j] >> (12 - AWB_ACC_BIT));
                Cnt[j] = (CA3Info.acc_cnt[j]);
            }
        }
    }
    if(awb_info[id].awb_single_mode == 1){
        for (j = 0; j < AWB_WIN_TOTAL; j++) {
            cap_acc_tab[id][AWBALG_CH_R][j]=acc_tab[id][AWBALG_CH_R][j];
            cap_acc_tab[id][AWBALG_CH_G][j]=acc_tab[id][AWBALG_CH_G][j];
            cap_acc_tab[id][AWBALG_CH_B][j]=acc_tab[id][AWBALG_CH_B][j];
            cap_acc_tab[id][AWBALG_CH_IR][j]=acc_tab[id][AWBALG_CH_IR][j];
            cap_acc_tab[id][AWBALG_CH_Rth][j]=acc_tab[id][AWBALG_CH_Rth][j];
            cap_acc_tab[id][AWBALG_CH_Gth][j]=acc_tab[id][AWBALG_CH_Gth][j];
            cap_acc_tab[id][AWBALG_CH_Bth][j]=acc_tab[id][AWBALG_CH_Bth][j];
        }
    }
}

static AWB_MODE AWB_getMode(UINT32 Idx)
{
    if (Idx == AWB_WB_AUTO) {
        return AWB_MODE_AUTO;
    }

    if (Idx == AWB_WB_DAYLIGHT) {
        return AWB_MODE_DAYLIGHT;
    }

    if (Idx == AWB_WB_CLOUDY) {
        return AWB_MODE_CLOUDY;
    }

    if (Idx == AWB_WB_TUNGSTEN) {
        return AWB_MODE_TUNGSTEN;
    }

    if (Idx == AWB_WB_SUNSET) {
        return AWB_MODE_SUNSET;
    }

    if (Idx == AWB_WB_CUSTOMER1) {
        return AWB_MODE_CUSTOMER1;
    }

    if (Idx == AWB_WB_CUSTOMER2) {
        return AWB_MODE_CUSTOMER2;
    }

    if (Idx == AWB_WB_CUSTOMER3) {
        return AWB_MODE_CUSTOMER3;
    }

    if (Idx == AWB_WB_CUSTOMER4) {
        return AWB_MODE_CUSTOMER4;
    }

    if (Idx == AWB_WB_CUSTOMER5) {
        return AWB_MODE_CUSTOMER5;
    }

    if (Idx == AWB_WB_CUSTOMER6) {
        return AWB_MODE_CUSTOMER6;
    }

    if (Idx == AWB_WB_MGAIN) {
        return AWB_MODE_MGAIN;
    }

    return AWB_MODE_AUTO;
}

static void AWB_autoProc(AWBALG_INFO *Info, awb_param *m1_par)
{
    UINT32 NewRGain = 256, NewGGain = 256, NewBGain = 256;
    AWBALG_ER ER_code;
    ER_code = AWBALG_Sim(Info, m1_par, &NewRGain, &NewGGain, &NewBGain);

    if (ER_code == AWBALG_OK) {
        if(Info->awb_single_mode == 1){
            Info->cap_gain.r_gain = NewRGain;
            Info->cap_gain.g_gain = NewGGain;
            Info->cap_gain.b_gain = NewBGain;
        } else {
            Info->AWBGain.r_gain = NewRGain;
            Info->AWBGain.g_gain = NewGGain;
            Info->AWBGain.b_gain = NewBGain;
        }
    } else {
        Info->status = AWBALG_STATUS_FREEZE;
    }
}

static void AWB_manualProc(AWBALG_INFO *Info, AWB_MODE Idx)
{
    Info->AWBGain.r_gain = (awb_param_addr[Info->Id]->mwb_tab+Idx)->r_gain;
    Info->AWBGain.g_gain = (awb_param_addr[Info->Id]->mwb_tab+Idx)->g_gain;
    Info->AWBGain.b_gain = (awb_param_addr[Info->Id]->mwb_tab+Idx)->b_gain;

    if ((Idx != AWB_MODE_CUSTOMER1) && (Idx != AWB_MODE_CUSTOMER2) && (Idx != AWB_MODE_CUSTOMER3)) {
        Info->AWBGain.r_gain = Info->AWBGain.r_gain * 100 / Info->k_gain.r_ratio;
        Info->AWBGain.b_gain = Info->AWBGain.b_gain * 100 / Info->k_gain.b_ratio;
    }
    Info->status = AWBALG_STATUS_MANUAL;

    AWB_setGain(Info, Info->AWBGain.r_gain, Info->AWBGain.g_gain, Info->AWBGain.b_gain);
}

static void awb_manual_single(AWBALG_INFO *Info, AWB_MODE Idx)
{
    Info->cap_gain.r_gain = (awb_param_addr[Info->Id]->mwb_tab+Idx)->r_gain;
    Info->cap_gain.g_gain = (awb_param_addr[Info->Id]->mwb_tab+Idx)->g_gain;
    Info->cap_gain.b_gain = (awb_param_addr[Info->Id]->mwb_tab+Idx)->b_gain;

    if ((Idx != AWB_MODE_CUSTOMER1) && (Idx != AWB_MODE_CUSTOMER2) && (Idx != AWB_MODE_CUSTOMER3)) {
        Info->cap_gain.r_gain = Info->cap_gain.r_gain * 100 / Info->k_gain.r_ratio;
        Info->cap_gain.b_gain = Info->cap_gain.b_gain * 100 / Info->k_gain.b_ratio;
    }
    Info->status = AWBALG_STATUS_MANUAL;
}

void AWB_Get_ColorAvg(UINT32 Id, UINT32 *r, UINT32 *g, UINT32 *b)
{
    UINT32 AWB_WIN_X = awb_info[Id].WinNumX;
    UINT32 AWB_WIN_Y = awb_info[Id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;
    IPL_CA_INFOR CAInfo;
    CAInfo.id = Id;
    CAInfo.length = AWB_WIN_TOTAL;
    CAInfo.r = awb_info[Id].AccTab[AWBALG_CH_R];
    CAInfo.g = awb_info[Id].AccTab[AWBALG_CH_G];
    CAInfo.b = awb_info[Id].AccTab[AWBALG_CH_B];
    CAInfo.ir = awb_info[Id].AccTab[AWBALG_CH_IR];
    CAInfo.r_th = awb_info[Id].AccTab[AWBALG_CH_Rth];
    CAInfo.g_th = awb_info[Id].AccTab[AWBALG_CH_Gth];
    CAInfo.b_th = awb_info[Id].AccTab[AWBALG_CH_Bth];
    CAInfo.acc_cnt = awb_info[Id].AccCnt;
    CAInfo.pnext = NULL;
    IPL_GetCmd(IPL_GET_CA_CUR_INFOR, (void *)&CAInfo);
    UINT32 i;
    (*r) = 0;
    (*g) = 0;
    (*b) = 0;

    for (i = 0; i < AWB_WIN_TOTAL; i++) {
        (*r) += (CAInfo.r[i] >> (12 - AWB_ACC_BIT));
        (*g) += (CAInfo.g[i] >> (12 - AWB_ACC_BIT));
        (*b) += (CAInfo.b[i] >> (12 - AWB_ACC_BIT));
    }

    (*r) /= AWB_WIN_TOTAL;
    (*g) /= AWB_WIN_TOTAL;
    (*b) /= AWB_WIN_TOTAL;
}

void awb_param_init(UINT32 id, UINT32* init)
{

    awb_param_addr[id] = (awb_param*)(*init);
    //DBG_ERR("%x \r\n", *init);
}

