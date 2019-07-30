/**
    Calibration item

    @file       CalibrationItem.c
    @ingroup    calibration
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#include "PrjCfg.h"
#if (CALIBRATION_FUNC == ENABLE)
// system
#include "stdio.h"
#include "Type.h"
#include "GxVideo.h"
#include "SysCfg.h"
#include "SysKer.h"
#include "PStore.h"
#include "FileSysTsk.h"
#include "GxImage.h"
#include "EngineerMode.h"
#include "KeyDef.h"
// cal
#include "ipl_calibration.h"
#include "ipl_cal_api.h"
#include "ipl_cal_obj.h"
#include "ipl_cal_obj_drv.h"
#include "ipl_cal_buf.h"
#include "ipl_cal_rw.h"
#include "ipl_cal_alg.h"
#include "ipl_cal_debug.h"
#include "cal_header_int.h"
#include "calibration_item.h"
#include "cal_ui_sample.h"
// ipl
#include "ipl_utility.h"
#include "ipl_api.h"
#include "ipl_ini.h"
#include "ipl_header_infor_raw.h"

// user setting
CAL_UI_STYLE g_cal_ui_style = CAL_UI_STYLE_NORMAL;
UINT32 g_cal_id[IPL_ID_MAX_NUM] = {IPL_ID_1, CAL_END}; // set the IPL ID you want to do calibration
BOOL g_dp_bright_ony = FALSE;
BOOL g_b_saveraw = FALSE;
#define PRV_SHOW_VA ENABLE // only SIE1 support
#define CAL_UI_MAX_STR 100

// for internal flow use
UINT32 g_current_cal_id = IPL_ID_MAX_NUM;
BOOL g_b_cal_id[IPL_ID_MAX_NUM] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
CAL_GETRAW_RST getraw_rst = {0};
CAL_ALG_PARAM alg_param = {0};
CAL_PRV_DATA prv_data_rst = {0};
CAL_PRV_DATA prv_data[IPL_MAX_SEN_NUM] = {0};

/**
    rw function point
*/
UINT32 cal_rw_store_op_open(char *p_sec_name, CAL_RW_OPEN_SEL open_sel);
ER cal_rw_store_op_close(UINT32 p_section_handle);
ER cal_rw_store_op_read(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle);
ER cal_rw_store_op_write(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle);
ER cal_rw_store_op_delete(char *p_sec_name);
UINT32 cal_rw_store_op_get_sec_size(char *p_sec_name);
BOOL cal_rw_store_op_is_sec_exist(char *p_sec_name);
CAL_RW_STORE_OP cal_rw_store_op = {
	cal_rw_store_op_open,
	cal_rw_store_op_close,
	cal_rw_store_op_read,
	cal_rw_store_op_write,
	cal_rw_store_op_delete,
	cal_rw_store_op_get_sec_size,
	cal_rw_store_op_is_sec_exist,
};


#if PRV_SHOW_VA
/**
    show VA for focus adjust
*/
#define CAL_VA_HW_WIN 16
#define CAL_VA_HW_IND_WIN 5
#define CAL_VA_WIN(x, y) (x + (y * CAL_VA_HW_WIN))
#define CAL_VA_SHOW_BLOCK_NUM 5
static UINT16 cal_focus_va_gamma[33] = {
	0, 36, 66, 89, 105, 120, 135, 146, 156, 164,
	172, 179, 186, 192, 197, 202, 207, 211, 215, 219,
	223, 227, 231, 234, 236, 238, 240, 242, 244, 247,
	250, 253, 255,
};
UINT32 cal_va_win_g1_h[CAL_VA_HW_WIN * CAL_VA_HW_WIN + CAL_VA_HW_IND_WIN] = {0};
UINT32 cal_va_win_g1_v[CAL_VA_HW_WIN * CAL_VA_HW_WIN + CAL_VA_HW_IND_WIN] = {0};
UINT32 cal_va_win_g2_h[CAL_VA_HW_WIN * CAL_VA_HW_WIN + CAL_VA_HW_IND_WIN] = {0};
UINT32 cal_va_win_g2_v[CAL_VA_HW_WIN * CAL_VA_HW_WIN + CAL_VA_HW_IND_WIN] = {0};
UINT32 cal_va_win_sum[CAL_VA_HW_WIN * CAL_VA_HW_WIN + CAL_VA_HW_IND_WIN] = {0};
IPL_VA_INFOR va_info = {0};
static void cal_focus_getva(void)
{
	UINT32 show_win_cnt;
	IPOINT va_win_block[CAL_VA_SHOW_BLOCK_NUM] = {{2, 2}, {13, 2}, {8, 8}, {2, 13}, {13, 13}};
	CHAR cal_ui_msg[CAL_UI_MAX_STR];
	URECT show_rect_info;

	IPL_GetCmd(IPL_GET_VA_CUR_INFOR, &va_info);


	for (show_win_cnt = 0; show_win_cnt < CAL_VA_SHOW_BLOCK_NUM; show_win_cnt++) {
		snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%d"
				 , cal_va_win_g1_h[CAL_VA_WIN(va_win_block[show_win_cnt].x, va_win_block[show_win_cnt].y)]);
		show_rect_info.x = (cal_ui()->get_dispsize().w / CAL_VA_HW_WIN) * (va_win_block[show_win_cnt].x);
		show_rect_info.y = (cal_ui()->get_dispsize().h / CAL_VA_HW_WIN) * (va_win_block[show_win_cnt].y);
		show_rect_info.w = (cal_ui()->get_dispsize().w / CAL_VA_HW_WIN);
		show_rect_info.h = (cal_ui()->get_dispsize().h / CAL_VA_HW_WIN);
		cal_ui()->ui_showstring(show_win_cnt, cal_ui_msg, CAL_UICOL_RED, &show_rect_info, TRUE, TRUE);
	}
}

static void cal_focus(IPL_PROC_ID id)
{
	IPL_SET_VA_DATA va_data = {0};
	IPL_SIE_CUR_FRAME_INFO sie_cur_info = {0};

	if (id != IPL_ID_1) {
		DBG_ERR("only SIE1 support get VA value\r\n");
		return;
	}

	sie_cur_info.id = id;
	IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &sie_cur_info);

	va_data.id = id;
	va_data.va_out_en = ENABLE;
	va_data.va_gamma_en = ENABLE;
	va_data.va_cg_en = DISABLE;
	va_data.va_info.VACrop.StartPix.x = 0;
	va_data.va_info.VACrop.StartPix.y = 0;
	va_data.va_info.VACrop.SizeH = sie_cur_info.out_size_h;
	va_data.va_info.VACrop.SizeV = sie_cur_info.out_size_v - 4;
	va_data.va_info.VA_WIN.WinNum_X = 16;
	va_data.va_info.VA_WIN.WinNum_Y = 16;
	va_data.va_info.VA_WIN.WinSpaceX = 0;
	va_data.va_info.VA_WIN.WinSpaceY = 0;
	va_data.va_info.VA_WIN.WinSizeX = va_data.va_info.VACrop.SizeH / va_data.va_info.VA_WIN.WinNum_X;
	va_data.va_info.VA_WIN.WinSizeY = va_data.va_info.VACrop.SizeV / va_data.va_info.VA_WIN.WinNum_Y;
	va_data.va_info.va_outsel = STATS_VA_OUT_GP_1;
	va_data.va_info.GmaTbl = cal_focus_va_gamma;

	va_data.va_info.G1.FilterH.FilterSize = STATS_VA_FLTR_SIZE_7;
	va_data.va_info.G1.FilterH.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
	va_data.va_info.G1.FilterH.FILT_ITEMS.A = 2;
	va_data.va_info.G1.FilterH.FILT_ITEMS.B = 0;
	va_data.va_info.G1.FilterH.FILT_ITEMS.C = -1;
	va_data.va_info.G1.FilterH.FILT_ITEMS.D = 0;
	va_data.va_info.G1.FilterH.uiDiv = 2;
	va_data.va_info.G1.FilterH.thres.Lower = 0x05;//0x80;
	va_data.va_info.G1.FilterH.thres.Upper = 0xc0;

	va_data.va_info.G1.HExtend = DISABLE;
	va_data.va_info.G1.LineMax = DISABLE;
	va_data.va_info.G1.FilterV.FilterSize = STATS_VA_FLTR_SIZE_7;
	va_data.va_info.G1.FilterV.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
	va_data.va_info.G1.FilterV.FILT_ITEMS.A = 2;
	va_data.va_info.G1.FilterV.FILT_ITEMS.B = 0;
	va_data.va_info.G1.FilterV.FILT_ITEMS.C = -1;
	va_data.va_info.G1.FilterV.FILT_ITEMS.D = 0;
	va_data.va_info.G1.FilterV.uiDiv = 2;
	va_data.va_info.G1.FilterV.thres.Lower = 0x05;//0x80;
	va_data.va_info.G1.FilterV.thres.Upper = 0xc0;

	if (va_data.va_info.va_outsel == STATS_VA_OUT_GP_1n2) {
		va_data.va_info.G2.FilterH.FilterSize = STATS_VA_FLTR_SIZE_7;
		va_data.va_info.G2.FilterH.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
		va_data.va_info.G2.FilterH.FILT_ITEMS.A = 2;
		va_data.va_info.G2.FilterH.FILT_ITEMS.B = 0;
		va_data.va_info.G2.FilterH.FILT_ITEMS.C = -1;
		va_data.va_info.G2.FilterH.FILT_ITEMS.D = 0;
		va_data.va_info.G2.FilterH.uiDiv = 2;
		va_data.va_info.G2.FilterH.thres.Lower = 0x5;//0x80;
		va_data.va_info.G2.FilterH.thres.Upper = 0x0;

		va_data.va_info.G2.HExtend = DISABLE;
		va_data.va_info.G2.LineMax = DISABLE;
		va_data.va_info.G2.FilterV.FilterSize = STATS_VA_FLTR_SIZE_7;
		va_data.va_info.G2.FilterV.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
		va_data.va_info.G2.FilterV.FILT_ITEMS.A = 2;
		va_data.va_info.G2.FilterV.FILT_ITEMS.B = 0;
		va_data.va_info.G2.FilterV.FILT_ITEMS.C = -1;
		va_data.va_info.G2.FilterV.FILT_ITEMS.D = 0;
		va_data.va_info.G2.FilterV.uiDiv = 2;
		va_data.va_info.G2.FilterV.thres.Lower = 0x5;//0x80;
		va_data.va_info.G2.FilterV.thres.Upper = 0x0;
	}


	va_data.va_info.VA_INDEP_WIN_INFO[0].bVA_INDEP_En = ENABLE;
	va_data.va_info.VA_INDEP_WIN_INFO[0].LineMaxG1 = DISABLE;
	va_data.va_info.VA_INDEP_WIN_INFO[0].LineMaxG2 = DISABLE;
	va_data.va_info.VA_INDEP_WIN_INFO[0].INDEP_WIN.StartPix.x = sie_cur_info.out_size_h / 2;
	va_data.va_info.VA_INDEP_WIN_INFO[0].INDEP_WIN.StartPix.y = sie_cur_info.out_size_v / 2;
	va_data.va_info.VA_INDEP_WIN_INFO[0].INDEP_WIN.SizeH =  120;
	va_data.va_info.VA_INDEP_WIN_INFO[0].INDEP_WIN.SizeV =  64;

	va_data.va_info.VA_INDEP_WIN_INFO[1] = va_data.va_info.VA_INDEP_WIN_INFO[0];
	va_data.va_info.VA_INDEP_WIN_INFO[2] = va_data.va_info.VA_INDEP_WIN_INFO[0];
	va_data.va_info.VA_INDEP_WIN_INFO[3] = va_data.va_info.VA_INDEP_WIN_INFO[0];
	va_data.va_info.VA_INDEP_WIN_INFO[4] = va_data.va_info.VA_INDEP_WIN_INFO[0];

	IPL_SetCmd(IPL_SET_VA, (void *)&va_data);
	IPL_WaitCmdFinish();

	va_info.id = id;
	va_info.grp1_h = cal_va_win_g1_h;
	va_info.grp1_v = cal_va_win_g1_v;
	va_info.grp2_h = cal_va_win_g2_h;
	va_info.grp2_v = cal_va_win_g2_v;
	va_info.va_win = VA_WIN_16X16;
	//va_info.va_win = VA_WIN_INDP1;

	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, cal_focus_getva);

}
#endif

static void cal_get_prv_param(CAL_PRV_DATA *prv_data)
{
	ISIZE  disp_size;

	disp_size = GxVideo_GetDeviceSize(DOUT1);

	prv_data[0].id = g_current_cal_id;
	prv_data[0].crop_ratio = IPL_UTI_CONV2_UINT32(16, 9);
	prv_data[0].raw_out_buf_num = 3;
	prv_data[0].ime_path_en = CAL_IME_PATH_EN_2;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].output_en = ENABLE;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].sample_rate = IPL_SAMPLE_RATE_STREAM;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].scale_size_h = disp_size.w;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].scale_size_v = disp_size.h;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_size_h = disp_size.w;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_size_v = disp_size.h;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_img_lofs = disp_size.w;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_start_x = 0;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].out_crop_start_y = 0;
	prv_data[0].ime_path_info[CAL_IME_PATH_2].img_fmt = IPL_IMG_Y_PACK_UV420;
	prv_data[0].ime_ppb_num[CAL_IME_PATH_2] = 3;
	prv_data[0].ime_out_group[CAL_IME_PATH_2].group_en = DISABLE;
	prv_data[0].func_en = IPL_FUNC_NONE;

#if PRV_SHOW_VA
	prv_data[0].func_en |= IPL_FUNC_SIE_CH3_VA;
#endif

#if 1
	prv_data[0].pnext = NULL;
#else
	prv_data[1] = prv_data[0];
	prv_data[1].id = id + 1;
	prv_data[1].pnext = NULL;
#endif
}

void cal_item_awb(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_AWB;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_AWB_RST cal_awb_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_AWB;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_awb_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}
			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

		}
	}


	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}


void cal_item_dpb(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_DP;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			if (g_dp_bright_ony) {
				ipl_cal_set_info(g_current_cal_id, CAL_SEL_ALG_DP_TAG, CAL_ITEM_DP_TAG_BRIGHT_ONLY);
			} else {
				ipl_cal_set_info(g_current_cal_id, CAL_SEL_ALG_DP_TAG, CAL_ITEM_DP_TAG_BRIGHT);
			}

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_DP_RST cal_dp_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_DP;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_dp_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else if (rw_header_info.value.cal_status == CAL_STATUS_DP_B_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK (bright pixel)!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}


		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_dpd(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_DP;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			ipl_cal_set_info(g_current_cal_id, CAL_SEL_ALG_DP_TAG, CAL_ITEM_DP_TAG_DARK);
			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_DP_RST cal_dp_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_DP;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_dp_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else if (rw_header_info.value.cal_status == CAL_STATUS_DP_D_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK (dark pixel)!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_iso(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_ISO;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_ISO_RST cal_iso_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_ISO;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_iso_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_ecs(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_ECS;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_ECS_RST cal_ecs_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_ECS;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_ecs_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}

			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_MAX_NUM; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}
		}
	}
	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_lenscen(void)
{
	DBG_IND("----------------------->\r\n");
	CAL_ITEM cal_item = CAL_ITEM_LENSCEN;
	UINT32 cnt, cnt_id;
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_ITEM2BIT(cal_item);
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to start)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    getraw & alg
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_GETRAW_PARAM getraw_param = {0};
				CAL_ALG_LENSCEN_RST cal_lenscen_rst = {0};

				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s start", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						getraw_param.cal_item = cal_item;
						getraw_param.sen_mode = setting_info->sen_mode[cnt];
						if (ipl_cal_getraw(g_current_cal_id, &getraw_param, &getraw_rst) == E_OK) {
							if (g_b_saveraw) {
								cal_dbg_saveraw(getraw_rst.raw_info->pixel_addr, getraw_rst.raw_info->img_info.line_offset, getraw_rst.raw_info->img_info.height
												, g_current_cal_id, getraw_param.cal_item, getraw_param.sen_mode);
							}
							alg_param.cal_item = cal_item;
							alg_param.getraw_rst = &getraw_rst;
							CAL_DATA_BUF_INFO buf_info = {0};
							buf_info.id = g_current_cal_id;
							buf_info.cal_item = CAL_ITEM_LENSCEN;
							buf_info.sen_mode = setting_info->sen_mode[cnt];
							buf_info.cal_cond = CAL_COND_DFT;
							UINT32 data_buf_addr;
							ER rt = ipl_cal_get_data_buf_addr(&buf_info, &data_buf_addr);
							if (rt == E_OK) {
								alg_param.res[0] = data_buf_addr;
							}
							ipl_cal_alg(g_current_cal_id, &alg_param, (void *)&cal_lenscen_rst);
						} else {
							DBG_ERR("getraw error,  skip ALG\r\n");
						}
					}
				}
				cal_ui()->ui_deleteline();
				snprintf(cal_ui_msg, CAL_UI_MAX_STR, "%s finish", cal_dbg_str_calitem(cal_item));
				cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			}


			/*
			    prv
			*/
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}
			snprintf(cal_ui_msg, CAL_UI_MAX_STR, "preview (press key to show rst)");
			cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
			cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

			/*
			    show result
			*/
			{
				CAL_SETTING_INFO *setting_info;
				CAL_RW_HEADER_INFO rw_header_info = {0};
				UINT32 data_addr = 0;

				setting_info = (CAL_SETTING_INFO *)ipl_cal_setting(g_current_cal_id)->fp_get_setting_info(cal_item);
				for (cnt = 0; cnt < SENSOR_MODE_Num - 1; cnt++) {
					if (setting_info->sen_mode[cnt] == SENSOR_MODE_UNKNOWN) {
						break;
					} else {
						rw_header_info.sec.cal_item = cal_item;
						rw_header_info.key.id = g_current_cal_id;
						rw_header_info.key.sensor_mode = setting_info->sen_mode[cnt];
						rw_header_info.key.cal_cond = CAL_COND_DFT;
						ipl_cal_getcalrst(g_current_cal_id, &rw_header_info, &data_addr);
						if (rw_header_info.value.cal_status == CAL_STATUS_OK) {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   OK!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_GREEN, CAL_UICOL_NONE);
						} else {
							snprintf(cal_ui_msg, CAL_UI_MAX_STR, "mode %d   NG!!!!!!!!!!!!!", rw_header_info.key.sensor_mode);
							cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_RED, CAL_UICOL_NONE);
						}
					}
				}
			}

			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

		}
	}

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "press any key to finish");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();
}

void cal_item_dnp(void)
{
	cal_item_dpb();
	DBG_ERR("START BRIGHT ------------------------------->\r\n");
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);
	cal_item_awb();
	cal_item_dpd();
	cal_item_iso();
	cal_item_ecs();
}

void cal_item_focus(void)
{
	DBG_IND("----------------------->\r\n");
	UINT32 cnt, cnt_id;

	cal_ui()->init(g_cal_ui_style);

	// set current cal id
	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_b_cal_id[cnt_id]) {
			g_current_cal_id = cnt_id;
			//break;

			/*
			    open
			*/
			cal_ui()->init(CAL_UI_STYLE_NORMAL);
			CAL_OPEN_PARAM open_param = {0};
			open_param.cal_item_bit = CAL_ITEM_BIT_NONE;
			if (ipl_cal_open(g_current_cal_id, &open_param) != E_OK) {
				return;
			}

			/*
			    prv
			*/
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_FPS, SEN_FPS(30, 1));
			IPL_AlgSetUIInfo(g_current_cal_id, IPL_SEL_VIDEO_SIZE, IPL_UTI_CONV2_UINT32(1920, 1080));
			{
				for (cnt = 0; cnt < IPL_MAX_SEN_NUM; cnt++) {
					prv_data[cnt] = prv_data_rst;
				}
				cal_get_prv_param(&prv_data[0]);
				if (ipl_cal_prv(&prv_data[0]) != E_OK) {
					return;
				}
			}

#if PRV_SHOW_VA
			cal_focus(g_current_cal_id);
#endif


			/*
			    close
			*/
			if (ipl_cal_close(g_current_cal_id) != E_OK) {
				return;
			}

		}
	}


	cal_ui()->ui_cleanscreen();
	cal_ui()->uninit();

}

void cal_item_test(void)
{
	DBG_IND("----------------------->\r\n");
	CHAR cal_ui_msg[CAL_UI_MAX_STR];

	cal_ui()->init(g_cal_ui_style);

	snprintf(cal_ui_msg, CAL_UI_MAX_STR, "start to test (press any key to exit)");
	cal_ui()->ui_showline(cal_ui_msg, CAL_UICOL_BLUE, CAL_UICOL_NONE);
	cal_ui()->ui_waitkey(FLGKEY_KEY_MASK, NULL);

	cal_ui()->uninit();
}

UINT32 cal_rw_store_op_open(char *p_sec_name, CAL_RW_OPEN_SEL open_sel)
{
	UINT32 p_section_handle = 0;
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	UINT32 RWOperation = 0;
	RWOperation |= (open_sel & CAL_RD_ONLY) ? PS_RDONLY : 0;
	RWOperation |= (open_sel & CAL_WR_ONLY) ? PS_WRONLY : 0;
	RWOperation |= (open_sel & CAL_CREATE) ? PS_CREATE : 0;
	RWOperation |= (open_sel & CAL_UPDATE) ? PS_UPDATE : 0;
	p_section_handle = (UINT32)PStore_OpenSection(p_sec_name, RWOperation);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	UINT32 rw_operation = 0;
	rw_operation |= (open_sel & CAL_RD_ONLY) ? FST_OPEN_READ : 0;
	rw_operation |= (open_sel & CAL_WR_ONLY) ? FST_OPEN_WRITE : 0;
	rw_operation |= (open_sel & CAL_CREATE) ? FST_CREATE_ALWAYS : 0;
	p_section_handle = (UINT32)FileSys_OpenFile(p_sec_name, rw_operation);
#else
#endif

	return p_section_handle;
}

ER cal_rw_store_op_close(UINT32 p_section_handle)
{
	ER rt;
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_CloseSection((PSTORE_SECTION_HANDLE *) p_section_handle);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_CloseFile((FST_FILE) p_section_handle);
#else
#endif
	return rt;
}

// p_buffer: read out the information to which buffer addr to put
// data_length: read out the information size
ER cal_rw_store_op_read(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle)
{
	ER rt;

#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_ReadSection(p_buffer, start_addr, data_length, (PSTORE_SECTION_HANDLE *)p_section_handle);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_ReadFile((FST_FILE)p_section_handle, p_buffer, &data_length, 0, NULL);
#else
#endif
	return rt;
}

ER cal_rw_store_op_write(UINT8 *p_buffer, UINT32 start_addr, UINT32 data_length, UINT32 p_section_handle)
{
	ER rt;
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_WriteSection(p_buffer, start_addr, data_length, (PSTORE_SECTION_HANDLE *)p_section_handle);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_WriteFile((FST_FILE)p_section_handle, p_buffer, &data_length, 0, NULL);
#else
#endif
	return rt;
}

ER cal_rw_store_op_delete(char *p_sec_name)
{
	ER rt;

#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	rt = PStore_DeleteSection(p_sec_name);
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	rt = FileSys_DeleteFile(p_sec_name);
#else
#endif

	return rt;
}

UINT32 cal_rw_store_op_get_sec_size(char *p_sec_name)
{

#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	UINT32 size = 0;
	BOOL b_next = FALSE;
	BOOL b_find = FALSE;
	PSTORE_SEARCH_HANDLE search_hdl;

	// search section name
	b_next = PStore_SearchSectionOpen(&search_hdl);
	while (b_next) {
		DBG_IND("%15s 0x%08x  %d  0x%08x\r\n", search_hdl.pSectionName, search_hdl.uiSectionSize, search_hdl.uiReadOnly, search_hdl.uiDescCheckSum);
		if (!strncmp(p_sec_name, search_hdl.pSectionName, CAL_HEADER_SEC_LEN)) {
			size = search_hdl.uiSectionSize;
			b_find = TRUE;
			break;
		}
		b_next = PStore_SearchSection(&search_hdl);
	}
	PStore_SearchSectionClose();

	if (b_find == FALSE) {
		DBG_IND("section %s does not exist\r\n", p_sec_name);
	}
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	INT32 size = 0;
	size = FileSys_GetFileLen(p_sec_name);
	if (size < 0) {
		size = 0;
	}
#else
#endif

	return size;
}

BOOL cal_rw_store_op_is_sec_exist(char *p_sec_name)
{
	BOOL is_exist = FALSE;
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
	BOOL b_next = FALSE;
	PSTORE_SEARCH_HANDLE search_hdl;

	// search section name
	b_next = PStore_SearchSectionOpen(&search_hdl);
	while (b_next) {
		DBG_IND("%15s\r\n", search_hdl.pSectionName);
		if (!strncmp(p_sec_name, search_hdl.pSectionName, CAL_HEADER_SEC_LEN)) {
			is_exist = TRUE;
			break;
		}
		b_next = PStore_SearchSection(&search_hdl);
	}
	PStore_SearchSectionClose();

	if (is_exist == FALSE) {
		DBG_IND("section %s does not exist\r\n", p_sec_name);
	}
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
	DBG_ERR("not implement\r\n");

//	int FileSys_SearchFile(FS_SEARCH_HDL pSearch, FIND_DATA *pFindData, int Direction, UINT16 *pLongFilename)
#else
#endif

	return is_exist;
}

void cal_system_init(void)
{
	ER rt = E_OK;
	UINT32 cnt_id;
	CAL_INIT_DATA init_data = {0};

	if (EngineerMode_CheckEng()) {
		init_data.buf_addr = OS_GetMempoolAddr(POOL_ID_APP);
		init_data.buf_size = POOL_SIZE_APP / 2;
		init_data.cal_rw_fp = &cal_rw_store_op;
	} else {
		init_data.buf_addr = OS_GetMempoolAddr(POOL_ID_CAL);
		init_data.buf_size = POOL_SIZE_CAL / 2;
		init_data.cal_rw_fp = &cal_rw_store_op;
	}

	DBG_IND("init_data buf_addr 0x%.8x buf_size 0x%.8x EngineerMode_CheckEng %d\r\n"
			, init_data.buf_addr, init_data.buf_size, EngineerMode_CheckEng());

	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_cal_id[cnt_id] == CAL_END) {
			break;
		}
		g_b_cal_id[cnt_id] = TRUE;
		rt = ipl_cal_init(&init_data);
		rt |= ipl_cal_ext_init(cnt_id);

		if (rt != E_OK) {
			DBG_ERR("cal init fail\r\n");
		}
	}
}

void cal_system_uninit(void)
{
	ER rt = E_OK;
	UINT32 cnt_id;

	for (cnt_id = 0; cnt_id < IPL_ID_MAX_NUM; cnt_id++) {
		if (g_cal_id[cnt_id] == CAL_END) {
			break;
		}
		rt = ipl_cal_ext_uninit(cnt_id);
		rt = ipl_cal_uninit();

		if (rt != E_OK) {
			DBG_ERR("cal uninit fail\r\n");
		}
	}

}

#endif

