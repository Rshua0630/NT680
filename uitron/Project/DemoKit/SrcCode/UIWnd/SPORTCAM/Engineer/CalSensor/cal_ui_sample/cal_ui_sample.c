#include "PrjCfg.h"
#if (CALIBRATION_FUNC == ENABLE)
// Gx
#include "GxGfx.h"
#include "GxDC.h"
#include "GxVideo.h"
#include "GxInput.h"
// system
#include "UIGraphics.h"
#include "UIDisplay.h"
#include "UICommon.h"
#include "KeyDef.h"
#include <stdio.h>
// cal
#include "cal_ui_sample.h"
#include "ipl_cal_debug.h"

static CAL_UI_STYLE g_cal_ui_style = CAL_UI_STYLE_NORMAL;
static UINT32 str_cnt = 0;
UINT32 str_h = 30;
static CHAR str[512];
static ISIZE cal_get_dispsize(void);

static UINT32 cal_uicol_conv2_ui(CAL_UI_COLOR cal_ui_color)
{
	switch (cal_ui_color) {
	case CAL_UICOL_NONE:
		return CLRID_IDX_TRANSPART;

	case CAL_UICOL_RED:
		return CLRID_IDX_RED;

	case CAL_UICOL_GREEN:
		return CLRID_IDX_GREEN;

	case CAL_UICOL_BLUE:
		return CLRID_IDX_BLUE;

	case CAL_UICOL_WHITE:
		return CLRID_IDX_WHITE;

	case CAL_UICOL_BLACK:
		return CLRID_IDX_BLACK;

	case CAL_UICOL_GRAY:
		return CLRID_IDX_BLACK50;

	case CAL_UICOL_YELLOW:
		return CLRID_IDX_YELLOW;

	case CAL_UICOL_MAX:
	default:
		DBG_ERR("input param. overflow %d, force to CLRID_IDX_BLACK\r\n", cal_ui_color);
		return CLRID_IDX_BLACK;
	}
	return CLRID_IDX_BLACK;
}

static void cal_showline_normal(CHAR *p_str, CAL_UI_COLOR str_color, CAL_UI_COLOR bg_color, PURECT location)
{
	DC     **pCalDCList;
	UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end

	pCalDCList = (DC **)UI_BeginScreen();

	GxGfx_FillRect(pCalDCList[GxGfx_OSD], location->x, location->y, location->x + location->w, location->y + location->h);
	GxGfx_SetShapeColor(cal_uicol_conv2_ui(bg_color), cal_uicol_conv2_ui(bg_color), 0);
	GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
	GxGfx_SetTextColor(cal_uicol_conv2_ui(str_color), CLRID_IDX_BLACK50, 0);
	GxGfx_Text(pCalDCList[GxGfx_OSD], location->x, location->y, p_str);

	UI_EndScreen((UINT32)pCalDCList);
}

static void cal_showline_web(CHAR *p_str, CAL_UI_COLOR str_color, CAL_UI_COLOR bg_color, PURECT location)
{
	DBG_ERR("need to implement\r\n");
}

static void cal_showline_usb(CHAR *p_str, CAL_UI_COLOR str_color, CAL_UI_COLOR bg_color, PURECT location)
{
	DBG_ERR("need to implement\r\n");
}

static void cal_ui_init(CAL_UI_STYLE cal_ui_style)
{
	g_cal_ui_style = cal_ui_style;
}

static void cal_ui_uninit(void)
{
	str_cnt = 0;
	g_cal_ui_style = CAL_UI_STYLE_NORMAL;
}

static void cal_waitkey_normal(UINT32 key_mask, WAITKEY_FP waitkey_fp)
{
	Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_NULL);

	while (1) {
		if (GxKey_GetData(GXKEY_NORMAL_KEY) & key_mask) {
			break;
		} else {
			if (waitkey_fp != NULL) {
				waitkey_fp();
			} else {
				Delay_DelayMs(33);
				Ux_FlushEvent();
			}
		}
	}
	//incase of ui to detect key pressed when key pressed too long.
	//but key still can't be pressed over 2 sec.
	Delay_DelayMs(2000);
	Ux_FlushEvent();
	Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
}

static void cal_waitkey_web(UINT32 key_mask, WAITKEY_FP waitkey_fp)
{
	DBG_ERR("need to implement\r\n");
}

static void cal_waitkey_usb(UINT32 key_mask, WAITKEY_FP waitkey_fp)
{
	DBG_ERR("need to implement\r\n");
}

static void cal_ui_waitkey(UINT32 key_mask, WAITKEY_FP waitkey_fp)
{
	switch (g_cal_ui_style) {
	case CAL_UI_STYLE_NORMAL:
		cal_waitkey_normal(key_mask, waitkey_fp);
		break;
	case CAL_UI_STYLE_USB:
		cal_waitkey_usb(key_mask, waitkey_fp);
		break;
	case CAL_UI_STYLE_WEB:
		cal_waitkey_web(key_mask, waitkey_fp);
		break;

	case CAL_UI_STYLE_MAX:
	default:
		DBG_ERR("param. overflow %d, force to CAL_UI_STYLE_NORMAL\r\n", g_cal_ui_style);
		cal_waitkey_normal(key_mask, waitkey_fp);
		break;
	}
}

static void cal_ui_showline(CHAR *p_str, CAL_UI_COLOR str_color, CAL_UI_COLOR bg_color)
{
	URECT location = {0};
	location.x = 0;
	location.y = 0 + str_cnt * str_h;
	location.w = cal_get_dispsize().w;
	location.h = str_h;
	str_cnt++;

	switch (g_cal_ui_style) {
	case CAL_UI_STYLE_NORMAL:
		cal_showline_normal(p_str, str_color, bg_color, &location);
		break;
	case CAL_UI_STYLE_USB:
		cal_showline_usb(p_str, str_color, bg_color, &location);
		break;
	case CAL_UI_STYLE_WEB:
		cal_showline_web(p_str, str_color, bg_color, &location);
		break;

	case CAL_UI_STYLE_MAX:
	default:
		DBG_ERR("param. overflow %d, force to CAL_UI_STYLE_NORMAL\r\n", g_cal_ui_style);
		cal_showline_normal(p_str, str_color, bg_color, &location);
		break;
	}
}

extern void Cal_ShowMultiString(UINT32 Idx, CHAR *pStr, PURECT pRect, BOOL bClear, BOOL bShowRect);
extern void Cal_SetDispColor(UINT32 Idx, UINT32 uiTextColor);
static void cal_ui_showstring(UINT32 idx, CHAR *p_str, CAL_UI_COLOR str_color, PURECT p_rect, BOOL b_clear, BOOL show_rect)
{
	Cal_SetDispColor(idx, cal_uicol_conv2_ui(str_color));
	Cal_ShowMultiString(idx, p_str, p_rect, b_clear, show_rect);

	Ux_Redraw();
}

static void cal_ui_deleteline(void)
{
	URECT location = {0};
	location.x = 0;
	location.y = 0 + str_cnt * str_h;
	location.w = cal_get_dispsize().w;
	location.h = str_h;
	str_cnt--;

	snprintf((char*)str, location.w, "                                              ");

	switch (g_cal_ui_style) {
	case CAL_UI_STYLE_NORMAL:
		cal_showline_normal((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;
	case CAL_UI_STYLE_USB:
		cal_showline_usb((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;
	case CAL_UI_STYLE_WEB:
		cal_showline_web((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;

	case CAL_UI_STYLE_MAX:
	default:
		DBG_ERR("param. overflow %d, force to CAL_UI_STYLE_NORMAL\r\n", g_cal_ui_style);
		cal_showline_normal((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;
	}
}

static void cal_ui_cleanscreen(void)
{
	URECT location = {0};
	location.x = 0;
	location.y = 0;
	location.w = cal_get_dispsize().w;
	location.h = cal_get_dispsize().h;
	str_cnt = 0;

	snprintf((char*)str, location.w, "                                              ");

	switch (g_cal_ui_style) {
	case CAL_UI_STYLE_NORMAL:
		cal_showline_normal((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;
	case CAL_UI_STYLE_USB:
		cal_showline_usb((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;
	case CAL_UI_STYLE_WEB:
		cal_showline_web((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;

	case CAL_UI_STYLE_MAX:
	default:
		DBG_ERR("param. overflow %d, force to CAL_UI_STYLE_NORMAL\r\n", g_cal_ui_style);
		cal_showline_normal((char*)str, CAL_UICOL_NONE, CAL_UICOL_NONE, &location);
		break;
	}
}

static void cal_ui_setbackground(CAL_UI_COLOR bg_color)
{
	URECT location = {0};
	location.x = 0;
	location.y = 0;
	location.w = cal_get_dispsize().w;
	location.h = cal_get_dispsize().h;

	snprintf((char*)str, location.w, "                                              ");

	switch (g_cal_ui_style) {
	case CAL_UI_STYLE_NORMAL:
		cal_showline_normal((char*)str, CAL_UICOL_NONE, bg_color, &location);
		break;
	case CAL_UI_STYLE_USB:
		cal_showline_usb((char*)str, CAL_UICOL_NONE, bg_color, &location);
		break;
	case CAL_UI_STYLE_WEB:
		cal_showline_web((char*)str, CAL_UICOL_NONE, bg_color, &location);
		break;

	case CAL_UI_STYLE_MAX:
	default:
		DBG_ERR("param. overflow %d, force to CAL_UI_STYLE_NORMAL\r\n", g_cal_ui_style);
		cal_showline_normal((char*)str, CAL_UICOL_NONE, bg_color, &location);
		break;
	}
}

static ISIZE cal_get_dispsize(void)
{
	ISIZE disp_size = {0};
	switch (g_cal_ui_style) {
	case CAL_UI_STYLE_NORMAL:
		disp_size = GxVideo_GetDeviceSize(DOUT1);
		disp_size.w = disp_size.w / 3;
		break;
	case CAL_UI_STYLE_USB:
		DBG_ERR("need to implement\r\n");
		break;
	case CAL_UI_STYLE_WEB:
		DBG_ERR("need to implement\r\n");
		break;

	case CAL_UI_STYLE_MAX:
	default:
		DBG_ERR("param. overflow %d, force to CAL_UI_STYLE_NORMAL\r\n", g_cal_ui_style);
		disp_size = GxVideo_GetDeviceSize(DOUT1);
		break;
	}

	return disp_size;
}

CAL_UI_OBJ cal_ui_obj = {

	cal_ui_init,
	cal_ui_uninit,

	cal_ui_waitkey,
	cal_ui_showline,
	cal_ui_deleteline,
	cal_ui_showstring,
	cal_ui_cleanscreen,
	cal_ui_setbackground,

	cal_get_dispsize,
};

pCAL_UI_OBJ cal_ui(void)
{
	return &cal_ui_obj;

}

#endif
