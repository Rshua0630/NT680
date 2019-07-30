//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCfg.h"
#include "SysCommon.h"

#include "AppLib.h"

#include "UIFramework.h"
#include "NVTUserCommand.h"

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysSensorCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

static BOOL Cmd_sys_sensor(CHAR *strCmd)
{
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1)); //fix for CID 44999

	if (strncmp(str1, "off", 31) == 0) {
		DBG_DUMP("NO SENSOR!\r\n");
		SxCmd_DoCommand("dx snr1 vdet 0");
		SxCmd_DoCommand("dx snr2 vdet 0");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
	if (strncmp(str1, "s1", 31) == 0) {
		DBG_DUMP("SENSOR1!\r\n");
		SxCmd_DoCommand("dx snr1 vdet 1");
		SxCmd_DoCommand("dx snr2 vdet 0");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
	if (strncmp(str1, "s2", 31) == 0) {
		DBG_DUMP("SENSOR2!\r\n");
		SxCmd_DoCommand("dx snr1 vdet 0");
		SxCmd_DoCommand("dx snr2 vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
	if (strncmp(str1, "s1s2", 31) == 0) {
		DBG_DUMP("SENSOR1+SENSOR2!\r\n");
		SxCmd_DoCommand("dx snr1 vdet 1");
		SxCmd_DoCommand("dx snr2 vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}

	if (strncmp(str1, "d1", 31) == 0) {
		DBG_DUMP("display SENSOR1!\r\n");
		Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_1);
	}
	if (strncmp(str1, "d2", 31) == 0) {
		DBG_DUMP("display SENSOR2!\r\n");
		Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_2);
	}
#if defined(_BSP_NA51000_)
	if (strncmp(str1, "d3", 31) == 0) {
		DBG_DUMP("display SENSOR3!\r\n");
		Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_3);
	}
	if (strncmp(str1, "d4", 31) == 0) {
		DBG_DUMP("display SENSOR4!\r\n");
		Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_4);
	}
#endif
	if (strncmp(str1, "d12", 31) == 0) {
		DBG_DUMP("display SENSOR 1 & 2!\r\n");
		Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1 | SENSOR_2));
	}
	if (strncmp(str1, "dall", 31) == 0) {
		DBG_DUMP("display SENSOR all!\r\n");
		Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_ALL);
	}
	return TRUE;
}

static BOOL Cmd_sys_change_sensor(CHAR *strCmd)
{
	UINT32 sensor_id;
	char   sensor_name[32];
	sscanf_s(strCmd, "%d %s", &sensor_id, sensor_name,sizeof(sensor_name));
	DBG_DUMP("dynamic change sensor %d %s\r\n",sensor_id,sensor_name);
	if (System_ChangeSensorName(sensor_id,sensor_name) == E_OK)
		DBG_DUMP("change ok\r\n");
	else
		DBG_DUMP("change fail\r\n");
	return TRUE;

}

SXCMD_BEGIN(syssnr, "sensor command")
SXCMD_ITEM("sensor %", Cmd_sys_sensor, "change sensor setting")
SXCMD_ITEM("ch %",     Cmd_sys_change_sensor, "change sensor name")
SXCMD_END()


void System_InstallSensorcmd(void)
{
	SxCmd_AddTable(syssnr);
}



