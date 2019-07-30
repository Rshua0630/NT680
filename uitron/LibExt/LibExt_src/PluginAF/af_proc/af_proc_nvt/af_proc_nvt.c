/**
 * Novatek AF Algorithm sample.
 * 
 * Reference AF Algorithm Implementation.
 * 
 * @file af_proc_nvt.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include "Perf.h"
#include "ipl_ctrl.h"
#include "ae_api.h"
#include "af_task.h"
#include "af_alg.h"
#include "af_ui_info.h"
#include "Lens.h"
#include "af_proc_nvt_int.h"
#if (AF_FACE_DETECTION == ENABLE)
#include "fd_lib.h"
#endif
#include "af_param_evb.h"
#include "NvtVerInfo.h"

NVTVER_VERSION_ENTRY(AF_PROC_NVT, 1, 00, 000, 00)

#define __MODULE__          AF_PROC
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    // *=All, [mark]=CustomClass
#include "DebugModule.h"


static AF_INFO g_AFInfo[AF_PROC_CNT] = 
{
    {
        .bIsInit                    = FALSE,
        .AfSubInfo.bAfEnable        = TRUE,
        .AfSubInfo.eShotMode        = AF_MODE_ONESHOT,    //AF_MODE_ONESHOT AF_MODE_CONTINUOUS
        .ManualSearchRange          = {0},
        .AfSubInfo.eExecuteStatus   = AFALG_STOP,
        .iFrameNum                  = 0,
        .bSetCropWin                = FALSE,
        .AfSubInfo.bFaceFocusEnable = FALSE,
        .AfSubInfo.uiFaceDetNum     = 0,
        .AfSubInfo.uiBlkPixelNum    = 3600,    // 1280x720/16/16
        .AfSubInfo.uiDbgMsg         = 0,       //AF_PRNT_DBG_VA
    },
    {
        .bIsInit                    = FALSE,
        .AfSubInfo.bAfEnable        = TRUE,
        .AfSubInfo.eShotMode        = AF_MODE_ONESHOT,
        .ManualSearchRange          = {0},
        .AfSubInfo.eExecuteStatus   = AFALG_STOP,
        .iFrameNum                  = 0,
        .bSetCropWin                = FALSE,
        .AfSubInfo.bFaceFocusEnable = FALSE,
        .AfSubInfo.uiFaceDetNum     = 0,
        .AfSubInfo.uiBlkPixelNum    = 3600,
        .AfSubInfo.uiDbgMsg         = 0,
    },
};


void AF_DRV_GetCurrentRange(UINT32 id, AF_F af_mode)
{
    UINT8 zoom_sec = Lens_ZoomGetSection(ZOOM_CURR_SECTION);    // 1 ~ 11
    AF_INFO *pinfo = &g_AFInfo[id];

    switch (af_mode) {
    case AF_F_SUPER_MACRO:
    case AF_F_MACRO:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(0, Lens_FocusGetTable(zoom_sec, 0) - 50);
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 7) + 50;    // 100 cm distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetTable(zoom_sec, 5);         // 50 cm distance
        break;

    case AF_F_INIFINITE:
        pinfo->AfSubInfo.iMinPos  = Lens_FocusGetTable(zoom_sec, 6);          // 70 cm distance
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 11) + 50;    // infinnite distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetTable(zoom_sec, 11);
        break;

    case AF_F_MANUAL:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(pinfo->ManualSearchRange.sManualStartIdx, Lens_FocusGetTable(zoom_sec, 0));
        pinfo->AfSubInfo.iMaxPos  = AFALG_MIN(pinfo->ManualSearchRange.sManualEndIdx, Lens_FocusGetTable(zoom_sec, 11));
        pinfo->AfSubInfo.iInitPos = (pinfo->AfSubInfo.iMinPos + pinfo->AfSubInfo.iMaxPos) / 2;
        break;

    case AF_F_CONTINUEFOCUS:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(0, Lens_FocusGetTable(zoom_sec, 5) - 50);    // 50 cm distance
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 11) + 50;                 // infinnite distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetPosition() & 0xFFFF;
        break;

    case AF_F_AUTO:
    default:
        pinfo->AfSubInfo.iMinPos  = AFALG_MAX(0, Lens_FocusGetTable(zoom_sec, 5) - 50);    // 50 cm distance
        pinfo->AfSubInfo.iMaxPos  = Lens_FocusGetTable(zoom_sec, 11) + 50;                 // infinnite distance
        pinfo->AfSubInfo.iInitPos = Lens_FocusGetTable(zoom_sec, 6);                       // 70 cm distance
        break;
    }

    DBG_MSG("AF mode = %d, range = %d ~ %d, iInitPos = %d\r\n", af_mode, pinfo->AfSubInfo.iMinPos, pinfo->AfSubInfo.iMaxPos, pinfo->AfSubInfo.iInitPos);
}


UINT16 AF_DRV_GetFocusDist(void)
{
    UINT8 i, zoom_sec = Lens_ZoomGetSection(ZOOM_CURR_SECTION);    // 1 ~ 11
    UINT32 curr_dist;

    for (i = 0; i < 11; i++) {
        if ((Lens_FocusGetPosition() & 0xFFFF) < Lens_FocusGetTable(zoom_sec, i)) {
            break;
        }
    }

    if (i == 0) {
        curr_dist = Lens_FocusGetDistTable(0);
    } else if (i == 11) {
        curr_dist = Lens_FocusGetDistTable(10);
    } else {
        UINT32 a_val, b_val, a_intvl, b_intvl;

        a_val = Lens_FocusGetDistTable(i-1);
        b_val = Lens_FocusGetDistTable(i);
        a_intvl = (Lens_FocusGetPosition() & 0xFFFF) - Lens_FocusGetTable(zoom_sec, i-1);
        b_intvl = Lens_FocusGetTable(zoom_sec, i) - (Lens_FocusGetPosition() & 0xFFFF);
        curr_dist = (a_val * b_intvl + b_val * a_intvl) / (a_intvl + b_intvl);
    }

    return (UINT16)curr_dist;
}


#if (USE_NVT_AF_ALG == ENABLE)
static INT32 AF_DRV_FocusDoAction(UINT8 category, INT32 target_pos)
{
    return Lens_FocusDoAction((FOCUS_CATEGORY)category, target_pos);
}


static INT32 AF_DRV_FocusGetPosition(void)
{
    return Lens_FocusGetPosition() & 0xFFFF;
}


static INT32 AF_DRV_FocusGetRange(void)
{
    return Lens_FocusGetRange(FOCUS_RANGE_BOTH);
}


static UINT8 AF_DRV_ZoomGetSection(UINT8 category)
{
    return Lens_ZoomGetSection((ZOOM_SECTION_CATEGORY)category);
}


static UINT32 AF_DRV_LensGetState(void)
{
    return (UINT32)Lens_ModuleGetState();
}


static UINT32 AF_ALG_AeGetLum(UINT32 id)
{
    AEStatus lum_status;

    AE_GetStatus(id, &lum_status);

    return lum_status.LumaAverage;
}


static void AF_TUNE_VaSetParam(UINT16 thr_low, UINT16 thr_high)
{

}
#endif


// call this API after setting whole parameters or changing parameters
static void AF_VA_SetParam2IPL(UINT32 id)
{
#if 0
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->VaInfo.id = id;

    IPL_SetCmd(IPL_SET_VA, (void *)&pinfo->VaInfo);
    IPL_WaitCmdFinish();
#endif
}


void AF_VA_SetCrop(UINT32 id, VACROP win_info)
{
    VACROP *pwin_sel;
    AF_INFO *pinfo = &g_AFInfo[id];

    pwin_sel = &pinfo->VaInfo.va_info.VACrop;
    memcpy(pwin_sel ,&win_info, sizeof(VACROP));

    pinfo->bSetCropWin = TRUE;
}


static void AF_VA_GetResult(UINT32 id, UINT32 *g1h, UINT32 *g1v, UINT32 *g2h, UINT32 *g2v, VA_WINIDX win_idx)
{
    IPL_VA_INFOR va_info;

    va_info.id = id;

    va_info.grp1_h = g1h;
    va_info.grp1_v = g1v;
    va_info.grp2_h = g2h;
    va_info.grp2_v = g2v;
    va_info.va_win = win_idx;

    IPL_GetCmd(IPL_GET_VA_CUR_INFOR, (void *)&va_info);
}


static UINT32 AF_VA_DumpVaInfo(UINT32 id)
{
    UINT32 i, j, idx, tmp;
    static UINT32 g1h[AF_WIN_HW_NUM] = {0}, g1v[AF_WIN_HW_NUM] = {0}, g2h[AF_WIN_HW_NUM] = {0}, g2v[AF_WIN_HW_NUM] = {0};

    AF_VA_GetResult(id, g1h, g1v, g2h, g2v, VA_WIN_16X16);

    DBG_DUMP("16x16 VA    00    01    02    03    04    05    06    07    08    09    10    11    12    13    14    15\r\n");

    for (j = 0; j < AF_WIN_HW_Y; j++) {
        DBG_DUMP("%02d      ", j);
        for (i = 0; i < AF_WIN_HW_X; i++) {
            idx = j * AF_WIN_HW_X + i;
            tmp = g1h[idx] + g1v[idx] + g2h[idx] + g2v[idx];
            DBG_DUMP("%6u", tmp);
        }
        DBG_DUMP("\r\n");
    }

    AF_VA_GetResult(id, g1h, g1v, g2h, g2v, VA_WIN_INDP0);
    tmp = g1h[0] + g1v[0] + g2h[0] + g2v[0];

    DBG_DUMP("IND0 VA = %d\r\n", tmp);

    return 0;
}


static void AF_VA_SetWindowCfg(UINT32 id, VA_WINIDX va_win, VACROP win_info)
{
    UINT32 idx = 0;
    VACROP *pwin_sel;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        pinfo->VaInfo.va_info.VA_WIN.WinNum_X = AF_WIN_HW_X;
        pinfo->VaInfo.va_info.VA_WIN.WinNum_Y = AF_WIN_HW_Y;
        pinfo->VaInfo.va_info.VA_WIN.WinSizeX = win_info.SizeH;
        pinfo->VaInfo.va_info.VA_WIN.WinSizeY = win_info.SizeV;
        pinfo->VaInfo.va_info.VA_WIN.WinSpaceX = 0;
        pinfo->VaInfo.va_info.VA_WIN.WinSpaceY = 0;
    } else if (va_win <= VA_WIN_INDP4) {
        if (va_win == VA_WIN_INDP0) {
            idx = 0;
        } else if (va_win == VA_WIN_INDP1) {
            idx = 1;
        } else if (va_win == VA_WIN_INDP2) {
            idx = 2;
        } else if (va_win == VA_WIN_INDP3) {
            idx = 3;
        } else if (va_win == VA_WIN_INDP4) {
            idx = 4;
        }

        pwin_sel = &pinfo->VaInfo.va_info.VA_INDEP_WIN_INFO[idx].INDEP_WIN;

        memcpy(pwin_sel ,&win_info, sizeof(VACROP));
    } else {    //(va_win == VA_WIN_CROP)
        pwin_sel = &pinfo->VaInfo.va_info.VACrop;

        memcpy(pwin_sel ,&win_info, sizeof(VACROP));
    }
}


#if (AF_FACE_DETECTION == ENABLE)
static UINT32 AF_VA_Set2FaceWin(UINT32 id)
{
    UINT32 i, det_face_num = 0;
    FD_FACE face_rect[10] = {0};    // must be the same with MAX_FDNUM
    URECT new_coordinate;
    VACROP va_win_info = {0};
    VA_WINIDX win_idx;
    IPL_SIE_CUR_FRAME_INFO frame_info = {0};

    frame_info.id = id;
    IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &frame_info);
    new_coordinate.x = new_coordinate.y = 0;
    new_coordinate.w = frame_info.out_size_h;
    new_coordinate.h = frame_info.out_size_v;

    if ((det_face_num = FD_GetRsltFace(face_rect, &new_coordinate)) != 0) {
        for (i = 0; i < 5; i++) {
            win_idx = (VA_WINIDX)(VA_WIN_INDP0 + i);

            if (i < det_face_num) {
                va_win_info.StartPix.x = face_rect[i].x;
                va_win_info.StartPix.y = face_rect[i].y;
                va_win_info.SizeH = AFALG_MIN(AF_INDP_WIN_W_MAX, face_rect[i].w);
                va_win_info.SizeV = AFALG_MIN(AF_INDP_WIN_H_MAX, face_rect[i].h);

                DBG_MSG("FD#%d @ (%d,%d) with %dx%d\r\n", det_face_num, va_win_info.StartPix.x, va_win_info.StartPix.y, va_win_info.SizeH, va_win_info.SizeV);
            } else {
                va_win_info.StartPix.x = 0;
                va_win_info.StartPix.y = 0;
                va_win_info.SizeH = 0;
                va_win_info.SizeV = 0;
            }

            AF_VA_SetWindowCfg(id, win_idx, va_win_info);
        }
    } else {
        DBG_MSG("face_num = 0!\r\n");
    }

    return AFALG_MIN(5, det_face_num);
}
#endif


static void AF_VA_SetThreshold(UINT32 id, VA_GRP va_grp, UINT32 thr_low, UINT32 thr_high)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_grp == VA_GRP1_H) {
        pinfo->VaInfo.va_info.G1.FilterH.thres.Lower = thr_low;
        pinfo->VaInfo.va_info.G1.FilterH.thres.Upper = AFALG_MAX(thr_low, thr_high);
    } else if (va_grp == VA_GRP1_V) {
        pinfo->VaInfo.va_info.G1.FilterV.thres.Lower = thr_low;
        pinfo->VaInfo.va_info.G1.FilterV.thres.Upper = AFALG_MAX(thr_low, thr_high);
    } else if (va_grp == VA_GRP2_H) {
        pinfo->VaInfo.va_info.G2.FilterH.thres.Lower = thr_low;
        pinfo->VaInfo.va_info.G2.FilterH.thres.Upper = AFALG_MAX(thr_low, thr_high);
    } else if (va_grp == VA_GRP2_V) {
        pinfo->VaInfo.va_info.G2.FilterV.thres.Lower = thr_low;
        pinfo->VaInfo.va_info.G2.FilterV.thres.Upper = AFALG_MAX(thr_low, thr_high);
    }
}


static void AF_VA_SetEnable(UINT32 id, BOOL enable, VA_WINIDX va_win)
{
    UINT32 idx = 0;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        pinfo->VaInfo.va_out_en = enable;
    } else if (va_win >= VA_WIN_INDP0) {
        if (va_win == VA_WIN_INDP0) {
            idx = 0;
        } else if (va_win == VA_WIN_INDP1) {
            idx = 1;
        } else if (va_win == VA_WIN_INDP2) {
            idx = 2;
        } else if (va_win == VA_WIN_INDP3) {
            idx = 3;
        } else if (va_win == VA_WIN_INDP4) {
            idx = 4;
        }

        pinfo->VaInfo.va_info.VA_INDEP_WIN_INFO[idx].bVA_INDEP_En = enable;
    }
}


static void AF_VA_SetLineMaxEnable(UINT32 id, BOOL enable, VA_WINIDX va_win, VA_GRP va_grp)
{
    UINT32 idx = 0;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        if (va_grp == VA_GRP1) {
            pinfo->VaInfo.va_info.G1.LineMax = enable;
        } else if (va_grp == VA_GRP2) {
            pinfo->VaInfo.va_info.G2.LineMax = enable;
        }
    } else if (va_win >= VA_WIN_INDP0) {
        if (va_win == VA_WIN_INDP0) {
            idx = 0;
        } else if (va_win == VA_WIN_INDP1) {
            idx = 1;
        } else if (va_win == VA_WIN_INDP2) {
            idx = 2;
        } else if (va_win == VA_WIN_INDP3) {
            idx = 3;
        } else if (va_win == VA_WIN_INDP4) {
            idx = 4;
        }

        if (va_grp == VA_GRP1) {
            pinfo->VaInfo.va_info.VA_INDEP_WIN_INFO[idx].LineMaxG1 = enable;
        } else if (va_grp == VA_GRP2) {
            pinfo->VaInfo.va_info.VA_INDEP_WIN_INFO[idx].LineMaxG2 = enable;
        }
    }
}


static void AF_VA_SetHExtendEnable(UINT32 id, BOOL enable, VA_WINIDX va_win, VA_GRP va_grp)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_win == VA_WIN_16X16) {
        if (va_grp == VA_GRP1) {
            pinfo->VaInfo.va_info.G1.HExtend = enable;
        } else if (va_grp == VA_GRP2) {
            pinfo->VaInfo.va_info.G2.HExtend = enable;
        }
    }
}


static void AF_VA_SetRGBGain(UINT32 id)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->VaInfo.va_info.VA_CG_INFO.Gain0 = 256;
    pinfo->VaInfo.va_info.VA_CG_INFO.Gain1 = 256;
    pinfo->VaInfo.va_info.VA_CG_INFO.Gain2 = 256;
    pinfo->VaInfo.va_info.VA_CG_INFO.Gain3 = 256;
}


static void AF_VA_SetPreGmaTbl(UINT32 id)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->VaInfo.va_info.GmaTbl = g_wAfPreGammaLut;
}


static void AF_VA_SetPreGmaEnable(UINT32 id, BOOL enable)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->VaInfo.va_gamma_en = enable;
}


static void AF_VA_SetColorGainEnable(UINT32 id, BOOL enable)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->VaInfo.va_cg_en = enable;
}


static void AF_VA_SetEdgeParam(UINT32 id, VA_GRP va_grp, VA_FLTR_INFO edge_ker)
{
    VA_FLTR_INFO *pgrp_sel;
    AF_INFO *pinfo = &g_AFInfo[id];

    if (va_grp == VA_GRP1_H) {
        pgrp_sel = &pinfo->VaInfo.va_info.G1.FilterH;
    } else if (va_grp == VA_GRP1_V) {
        pgrp_sel = &pinfo->VaInfo.va_info.G1.FilterV;
    } else if (va_grp == VA_GRP2_H) {
        pgrp_sel = &pinfo->VaInfo.va_info.G2.FilterH;
    } else if (va_grp == VA_GRP2_V) {
        pgrp_sel = &pinfo->VaInfo.va_info.G2.FilterV;
    } else {
        DBG_MSG("unknown va_grp value!\r\n");
        return;
    }

    memcpy(pgrp_sel, &edge_ker, sizeof(VA_FLTR_INFO));
}


static void AF_VA_UpdateParam8Iso(UINT32 id)
{
    UINT32 i, iso, iso_base = 100, exp_time, iris, tbl_idx = 0;

    AE_GetPrvAEArg(id, &iso, &exp_time, &iris);

    for (i = 0; i < 12; i++) {
        if (iso > iso_base) {
            tbl_idx++;
            break;
        }

        iso_base *= 2;
    }

    tbl_idx = AFALG_MIN(tbl_idx, 8);

    // set VA threshold for filter noise
    AF_VA_SetThreshold(id, VA_GRP1_H, g_uiVaNoiseThreshold[tbl_idx][0], g_uiVaNoiseThreshold[tbl_idx][1]);
    AF_VA_SetThreshold(id, VA_GRP1_V, g_uiVaNoiseThreshold[tbl_idx][0], g_uiVaNoiseThreshold[tbl_idx][1]);
    AF_VA_SetThreshold(id, VA_GRP2_H, g_uiVaNoiseThreshold[tbl_idx][0], g_uiVaNoiseThreshold[tbl_idx][1]);
    AF_VA_SetThreshold(id, VA_GRP2_V, g_uiVaNoiseThreshold[tbl_idx][0], g_uiVaNoiseThreshold[tbl_idx][1]);
}


static void AF_VA_SetActiveWin(UINT32 id)
{
    UINT32 width = 0, height = 0;
    VACROP va_win_info = {0};
    IPL_SIE_CUR_FRAME_INFO frame_info = {0};
    AF_INFO *pinfo = &g_AFInfo[id];

    if (pinfo->bSetCropWin == TRUE) {
        width = pinfo->VaInfo.va_info.VACrop.SizeH;
        height = pinfo->VaInfo.va_info.VACrop.SizeV;

        pinfo->bSetCropWin = FALSE;
    } else {
        frame_info.id = id;
        IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &frame_info);
        width = frame_info.out_size_h;
        height = frame_info.out_size_v;
    }

    va_win_info.StartPix.x = 0;
    va_win_info.StartPix.y = 0;
    va_win_info.SizeH = width;
    va_win_info.SizeV = height;

    AF_VA_SetWindowCfg(id, (VA_WIN_INDP4+1), va_win_info);    // VA_WIN_CROP

    va_win_info.StartPix.x = 0;
    va_win_info.StartPix.y = 0;
    va_win_info.SizeH = width / AF_WIN_HW_X;
    va_win_info.SizeV = height / AF_WIN_HW_Y;

    AF_VA_SetWindowCfg(id, VA_WIN_16X16, va_win_info);

    va_win_info.StartPix.x = (width >= AF_INDP_WIN_W_MAX) ? (((width - AF_INDP_WIN_W_MAX) / 2) & 0xFFFFFFFE) : 0;
    va_win_info.StartPix.y = (height >= AF_INDP_WIN_H_MAX) ? (((height - AF_INDP_WIN_H_MAX ) / 2) & 0xFFFFFFFE) : 0;
    va_win_info.SizeH = (width >= AF_INDP_WIN_W_MAX) ? AF_INDP_WIN_W_MAX : width;
    va_win_info.SizeV = (height >= AF_INDP_WIN_H_MAX) ? AF_INDP_WIN_H_MAX : height;
    AF_VA_SetWindowCfg(id, VA_WIN_INDP0, va_win_info);
}


void AF_VA_Setup(UINT32 id)
{
    VACROP va_win_info = {0};
    VA_FLTR_INFO va_fltr_coef;
    AF_INFO *pinfo = &g_AFInfo[id];

    AF_VA_SetRGBGain(id);
    AF_VA_SetPreGmaTbl(id);
    AF_VA_SetColorGainEnable(id, FALSE);    // disable VA color gain
    AF_VA_SetPreGmaEnable(id, TRUE);        // enable VA pre-gamma

    va_win_info.StartPix.x = 0;
    va_win_info.StartPix.y = 0;
    va_win_info.SizeH = 32;
    va_win_info.SizeV = 32;
    AF_VA_SetWindowCfg(id, VA_WIN_INDP1, va_win_info);
    AF_VA_SetWindowCfg(id, VA_WIN_INDP2, va_win_info);
    AF_VA_SetWindowCfg(id, VA_WIN_INDP3, va_win_info);
    AF_VA_SetWindowCfg(id, VA_WIN_INDP4, va_win_info);

    // enable 16x16 and independent windows 0
    AF_VA_SetEnable(id, TRUE, VA_WIN_16X16);
#if (AF_FACE_DETECTION == ENABLE)
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP0);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP1);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP2);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP3);
    AF_VA_SetEnable(id, TRUE, VA_WIN_INDP4);
#else
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP0);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP1);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP2);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP3);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP4);
#endif

    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_16X16, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_16X16, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP0, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP0, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP1, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP1, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP2, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP2, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP3, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP3, VA_GRP2);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP4, VA_GRP1);
    AF_VA_SetLineMaxEnable(id, FALSE, VA_WIN_INDP4, VA_GRP2);

    AF_VA_SetHExtendEnable(id, FALSE, VA_WIN_16X16, VA_GRP1);
    AF_VA_SetHExtendEnable(id, FALSE, VA_WIN_16X16, VA_GRP2);

    va_fltr_coef.FilterSize = STATS_VA_FLTR_SIZE_5;
    va_fltr_coef.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    va_fltr_coef.FILT_ITEMS.A = 2;
    va_fltr_coef.FILT_ITEMS.B = -1;
    va_fltr_coef.FILT_ITEMS.C = 0;
    va_fltr_coef.FILT_ITEMS.D = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP1_H, va_fltr_coef);

    va_fltr_coef.FilterSize = STATS_VA_FLTR_SIZE_5;
    va_fltr_coef.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    va_fltr_coef.FILT_ITEMS.A = 2;
    va_fltr_coef.FILT_ITEMS.B = -1;
    va_fltr_coef.FILT_ITEMS.C = 0;
    va_fltr_coef.FILT_ITEMS.D = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP1_V, va_fltr_coef);

    va_fltr_coef.FilterSize = STATS_VA_FLTR_SIZE_5;
    va_fltr_coef.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    va_fltr_coef.FILT_ITEMS.A = 2;
    va_fltr_coef.FILT_ITEMS.B = 0;
    va_fltr_coef.FILT_ITEMS.C = -1;
    va_fltr_coef.FILT_ITEMS.D = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP2_H, va_fltr_coef);

    va_fltr_coef.FilterSize = STATS_VA_FLTR_SIZE_5;
    va_fltr_coef.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    va_fltr_coef.FILT_ITEMS.A = 2;
    va_fltr_coef.FILT_ITEMS.B = 0;
    va_fltr_coef.FILT_ITEMS.C = -1;
    va_fltr_coef.FILT_ITEMS.D = 0;
    va_fltr_coef.uiDiv = AF_VA_DIV;
    AF_VA_SetEdgeParam(id, VA_GRP2_V, va_fltr_coef);

#if (AF_FACE_DETECTION == ENABLE)
    if (IPL_AlgGetUIInfo(id, IPL_SEL_FD_IMM) == SEL_FD_ON) {
        pinfo->AfSubInfo.uiFaceDetNum = AF_VA_Set2FaceWin(id);
        pinfo->AfSubInfo.bFaceFocusEnable = TRUE;
    } else {
        pinfo->AfSubInfo.uiFaceDetNum = 0;
        pinfo->AfSubInfo.bFaceFocusEnable = FALSE;
    }
#else
    pinfo->AfSubInfo.bFaceFocusEnable = FALSE;
#endif

    pinfo->VaInfo.va_info.va_outsel = STATS_VA_OUT_GP_1n2;

    AF_VA_UpdateParam8Iso(id);

    AF_VA_SetParam2IPL(id);
}


static void AF_VA_Release(UINT32 id)
{
    VACROP va_win_info;

    va_win_info.StartPix.x = 0;
    va_win_info.StartPix.y = 0;
    va_win_info.SizeH = 32;
    va_win_info.SizeV = 32;
    AF_VA_SetWindowCfg(id, VA_WIN_16X16, va_win_info);

    AF_VA_SetEnable(id, FALSE, VA_WIN_16X16);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP0);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP1);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP2);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP3);
    AF_VA_SetEnable(id, FALSE, VA_WIN_INDP4);

    AF_VA_SetParam2IPL(id);
}


#if (USE_NVT_AF_ALG == DISABLE)
static void AFALG_Process(UINT32 id)
{
    UINT32 g1h = 0, g1v = 0, g2h = 0, g2v = 0, va;

    // get current indepdent window #0 VA value
    AF_VA_GetResult(id, &g1h, &g1v, &g2h, &g2v, VA_WIN_INDP0);
    va = g1h + g1v + g2h + g2v;
}
#else    // (USE_NVT_AF_ALG == ENABLE)
static void AF_SetAlgParam(UINT32 id, AFALG_CFG_ITEM item, AFALG_SET_PARAM set_param)
{
    NVTAFALG_SetAlgParam(id, item, set_param);
}


static AFALG_SET_PARAM AF_GetAlgParam(UINT32 id, AFALG_CFG_ITEM item)
{
    return NVTAFALG_GetAlgParam(id, item);
}
#endif


static void AF_SetParam(UINT32 id)
{
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->uiId = id;

    if (pinfo->AfSubInfo.eWindowCfg == AFALG_WINDOW_CENTER) {    // single window AF
        memcpy(pinfo->AfSubInfo.ucWindowWeight, g_ucAfWinWeight8x8_4, sizeof(g_ucAfWinWeight8x8_4));
    } else if (pinfo->AfSubInfo.eWindowCfg == AFALG_WINDOW_MULTI) {    // multi window AF
        memcpy(pinfo->AfSubInfo.ucWindowWeight, g_ucAfWinWeight8x8_64, sizeof(g_ucAfWinWeight8x8_64));
    } else {
        memcpy(pinfo->AfSubInfo.ucWindowWeight, g_ucAfWinWeight8x8_36, sizeof(g_ucAfWinWeight8x8_36));
        DBG_ERR("must choose AF window weighting!\r\n");
    }
}


AF_CBMSG_FP g_AfProcCbfp = NULL;    // callback funtion to notify UI

void AF_RegisterCB(AF_CBMSG_FP cb_func)
{
    g_AfProcCbfp = cb_func;
}


ER AF_Run(AF_PROC_ID id, BOOL wait_finish)
{
    static BOOL ever_entry = FALSE;
    UINT32 af_mode;
    ER rt_status = E_OK;
#if (AF_FACE_DETECTION == ENABLE)
    AF_INFO *pinfo = &g_AFInfo[id];
#endif

    if (ever_entry == FALSE) {
        ever_entry = TRUE;
        AF_VA_Setup(id);    // setup IPE VA config
    }

    // set AF active window
    AF_VA_SetActiveWin(id);

    // get searching range
    af_mode = af_get_ui_info((IPL_PROC_ID)id, AF_UI_F);
    AF_DRV_GetCurrentRange(id, af_mode);

    AF_VA_UpdateParam8Iso(id);

    if (AF_GetCtrlObjStatus(id) >= AF_TASK_STATUS_OPEN) {
#if (AF_FACE_DETECTION == ENABLE)
        if (IPL_AlgGetUIInfo(id, IPL_SEL_FD_IMM) == SEL_FD_ON) {
            pinfo->AfSubInfo.uiFaceDetNum = AF_VA_Set2FaceWin(id);
            pinfo->AfSubInfo.bFaceFocusEnable = TRUE;
        } else {
            pinfo->AfSubInfo.uiFaceDetNum = 0;
            pinfo->AfSubInfo.bFaceFocusEnable = FALSE;
        }
#endif

        AF_VA_SetParam2IPL(id);
        IPL_Wait_Vd((IPL_PROC_BIT_ID)(1 << id), FALSE, 200);

#if (USE_NVT_AF_ALG == ENABLE)
        NVTAFALG_ResetInfo(id);    // reset AF info before AF is started
#endif
    } else {
        DBG_MSG("AF doesn't open yet(%d)!\r\n", id);
    }

    return rt_status;
}


ER AF_Release(AF_PROC_ID id, BOOL wait_finish)
{
    ER rt_status = E_OK;
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->AfSubInfo.eExecuteStatus = AFALG_QUIT;

    AF_VA_Release(id);    // release IPE VA

    if (wait_finish == TRUE) {
        AF_WaitIdle(id);
    }

    return rt_status;
}


void AF_SetManualRange(UINT32 id, INT16 start_pos, INT16 end_pos, INT8 step_size)
{
    AF_DRV_MANUAL manual_setting;
    AF_INFO *pinfo = &g_AFInfo[id];

    manual_setting.sManualStartIdx = start_pos;
    manual_setting.sManualEndIdx = end_pos;
    manual_setting.cManualStep = step_size;

    pinfo->ManualSearchRange = manual_setting;
}


UINT32 AF_WriteDebugInfo(UINT32 id, UINT32 addr)
{
    UINT32 buf_cnt;
    UINT8 zoom_sec = Lens_ZoomGetSection(ZOOM_CURR_SECTION);    // 1 ~ 11
    AF_INFO *pinfo = &g_AFInfo[id];

    buf_cnt = snprintf(NULL, 0, "\r\n\r\n\r\nAF Info------------------------------------------------------------------------------------------------------------------------\r\n");
    addr += snprintf((char *)addr, buf_cnt+1, "\r\n\r\n\r\nAF Info------------------------------------------------------------------------------------------------------------------------\r\n");

    buf_cnt = snprintf(NULL, 0, "ID=%d, ZoomSec=%d\r\n");
    addr += snprintf((char *)addr, buf_cnt+1, "ID=%d, ZoomSec=%d\r\n", pinfo->uiId, zoom_sec);

#if (USE_NVT_AF_ALG == ENABLE)
    NVTAFALG_WriteDebugInfo(id, &addr);
#endif

    buf_cnt = snprintf(NULL, 0, "\r\n");
    addr += snprintf((char *)addr, buf_cnt+1, "\r\n");

    return addr;
}


void AF_Init(UINT32 id, UINT32 current_status)
{
    UINT32 ui_status = 0, af_mode;
#if (USE_NVT_AF_ALG == ENABLE)
    UINT32 i;
    AFALG_SET_PARAM set_param = {0}, get_param = {0};
#endif
    AF_INFO *pinfo = &g_AFInfo[id];

    if (pinfo->bIsInit == TRUE) {
        return;
    }

    if (pinfo->AfSubInfo.uiDbgMsg & AF_PRNT_DBG_PERF_MEASURE) {
        Perf_Open();
        pinfo->AfSubInfo.uiPerfTime = Perf_GetCurrent();
    }

    if (g_AfProcCbfp) {
        g_AfProcCbfp(AF_CBMSG_AF_START, (void *)&ui_status);
    }

    switch (af_get_ui_info(id, AF_UI_WINDOW)) {
    case AF_WINDOW_AUTO:
        pinfo->AfSubInfo.eWindowCfg = AFALG_WINDOW_MULTI;
        break;

    case AF_WINDOW_MANUAL:
        pinfo->AfSubInfo.eWindowCfg = AFALG_WINDOW_MANUAL;
        break;

    case AF_WINDOW_CENTER:
        pinfo->AfSubInfo.eWindowCfg = AFALG_WINDOW_CENTER;
        break;

    default:
        DBG_ERR("exception AF Window!\r\n");
        break;
    }

#if (USE_NVT_AF_ALG == ENABLE)
    NVTAFALG_RegFocusDoActionCB(&AF_DRV_FocusDoAction);
    NVTAFALG_RegFocusGetPositionCB(&AF_DRV_FocusGetPosition);
    NVTAFALG_RegFocusGetRangeCB(&AF_DRV_FocusGetRange);
    NVTAFALG_RegZoomGetSectionCB(&AF_DRV_ZoomGetSection);
    NVTAFALG_RegLensGetStateCB(&AF_DRV_LensGetState);
    NVTAFALG_RegAeGetLumCB(&AF_ALG_AeGetLum);
    NVTAFALG_RegVaSetParamCB(&AF_TUNE_VaSetParam);

    NVTAFALG_SetAfInfoPtr(id, &pinfo->AfSubInfo);

    set_param.iAfAlgParam[0] = AF_SEARCH_METHOD;
    AF_SetAlgParam(id, AFALG_CFG_METHOD, set_param);

    set_param.iAfAlgParam[0] = AF_MAX_SEARCH_COUNT;
    AF_SetAlgParam(id, AFALG_CFG_TIME_OUT_NUM, set_param);

    set_param.iAfAlgParam[0] = AF_ROUGH_STEP_SIZE;
    set_param.iAfAlgParam[1] = AF_FINE_STEP_SIZE;
    set_param.iAfAlgParam[2] = AF_FINAL_STEP_SIZE;
    AF_SetAlgParam(id, AFALG_CFG_STEP_SIZE, set_param);

    set_param.iAfAlgParam[0] = AF_THRES_ROUGH_PERCENT;
    set_param.iAfAlgParam[1] = AF_THRES_FINE_PERCENT;
    set_param.iAfAlgParam[2] = AF_THRES_FINAL_PERCENT;
    set_param.iAfAlgParam[3] = AF_THRES_RESTART_PERCENT;
    AF_SetAlgParam(id, AFALG_CFG_THRS, set_param);

    for (i = AFALG_CFG_METHOD; i < AFALG_CFG_MAX_ITEM; i++) {
        memset(&get_param, 0, sizeof(AFALG_SET_PARAM));

        get_param = AF_GetAlgParam(id, (AFALG_CFG_ITEM)i);
        DBG_MSG("NVTAFALG param = %d, %d, %d, %d\r\n", get_param.iAfAlgParam[0], get_param.iAfAlgParam[1], get_param.iAfAlgParam[2], get_param.iAfAlgParam[3]);
    }
#endif

    // get searching range
    af_mode = af_get_ui_info((IPL_PROC_ID)id, AF_UI_F);
    AF_DRV_GetCurrentRange(id, af_mode);

#if (USE_NVT_AF_ALG == ENABLE)
    NVTAFALG_Init(id);
    //NVTAFALG_ResetInfo(id);    // start NVTAFALG_Process()
#endif

    AF_SetParam(id);

    AF_VA_DumpInfoFP(AF_VA_DumpVaInfo);
    AF_TriggerFP(AF_Run);

    pinfo->bIsInit = TRUE;

    PRINT_AF(AF_PRNT_DBG_PERF_MEASURE, "AF init time = %d msec\r\n", (Perf_GetCurrent()-pinfo->AfSubInfo.uiPerfTime));
}


void AF_Process(UINT32 id, UINT32 current_status)
{
    UINT32 exe_status;
    AF_INFO *pinfo = &g_AFInfo[id];

    pinfo->iFrameNum++;

    if ((pinfo->AfSubInfo.eExecuteStatus == AFALG_RUN) || (pinfo->AfSubInfo.eExecuteStatus == AFALG_END)) {
#if (USE_NVT_AF_ALG == DISABLE)
        AFALG_Process(id);
#else
        NVTAFALG_Process(id);
#endif

        if (pinfo->AfSubInfo.eExecuteStatus == AFALG_END) {
            if (g_AfProcCbfp) {
                exe_status = pinfo->AfSubInfo.uiResultStatus;
                g_AfProcCbfp(AF_CBMSG_AF_DONE, (void *)&exe_status);
            }
        }
    }
}


