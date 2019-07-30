#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utility.h"

#include "DrvExt.h"

#include "adc.h" //for adc_readData()
#include "top.h" //for pinmux_select_ick(), pinmux_select_goice(), PINMUX_ICK, PINMUX_GOICE
#include "DxApi.h"
#include "Driver.h" //for drv_getVerInfo(), drv_getBuildDate()
#include "IOCfg.h" //for USE_VIO
#include "SwTimer.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#include "SxCmd.h"

static BOOL Cmd_dx_lcd(CHAR *strCmd)
{
	DX_HANDLE devLCDObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
	return Dx_Command(devLCDObj, strCmd);
}
static BOOL Cmd_dx_tv(CHAR *strCmd)
{
	DX_HANDLE devTVObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_TVOUT);
	return Dx_Command(devTVObj, strCmd);
}
static BOOL Cmd_dx_hdmi(CHAR *strCmd)
{
	DX_HANDLE devHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_HDMIOUT);
	return Dx_Command(devHDMIObj, strCmd);
}
static BOOL Cmd_dx_lcd2(CHAR *strCmd)
{
	DX_HANDLE devLCD2Obj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD2);
	return Dx_Command(devLCD2Obj, strCmd);
}
static BOOL Cmd_dx_snr1(CHAR *strCmd)
{
	switch (*strCmd) {
#if (USE_VIO == ENABLE)
	case 'v':
		if (!strncmp(strCmd, "vdet", 4)) {
			char ts[10];
			UINT32 v1;
			sscanf_s(strCmd, "%s %d", ts, &v1); //fix for CID 44011
			DBG_IND("v1 = %d\r\n", v1);
			vio_setpin(VIO_SENSOR1_DET, v1);
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}
static BOOL Cmd_dx_snr2(CHAR *strCmd)
{
	switch (*strCmd) {
#if (USE_VIO == ENABLE)
	case 'v':
		if (!strncmp(strCmd, "vdet", 4)) {
			char ts[10];
			UINT32 v1;
			sscanf_s(strCmd, "%s %d", ts, &v1); //fix for CID 44012
			DBG_IND("v1 = %d\r\n", v1);
			vio_setpin(VIO_SENSOR2_DET, v1);
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}
static BOOL Cmd_dx_aout(CHAR *strCmd)
{
	DX_HANDLE devAudObj = Dx_GetObject(DX_CLASS_AUDIO_EXT);
	return Dx_Command(devAudObj, strCmd);
}

SXCMD_BEGIN(dx, "driver extern debug using")
SXCMD_ITEM("lcd %", Cmd_dx_lcd, "lcd relay command")
SXCMD_ITEM("tv %", Cmd_dx_tv, "tv relay command")
SXCMD_ITEM("hdmi %", Cmd_dx_hdmi, "hdmi relay command")
SXCMD_ITEM("lcd2 %", Cmd_dx_lcd2, "lcd2 relay command")
SXCMD_ITEM("snr1 %", Cmd_dx_snr1, "sensor1 relay command")
SXCMD_ITEM("snr2 %", Cmd_dx_snr2, "sensor2 relay command")
SXCMD_ITEM("aout %", Cmd_dx_aout, "audio out relay command")
SXCMD_END()

