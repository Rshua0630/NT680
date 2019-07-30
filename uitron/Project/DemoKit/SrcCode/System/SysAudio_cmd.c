//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysAudCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


static BOOL Cmd_sys_aout(CHAR *strCmd)
{
	char str1[32];

	sscanf_s(strCmd, "%s", str1, sizeof(str1));

	if (strncmp(str1, "off", 31) == 0) {
		DBG_DUMP("(OFF)!\r\n");
		//TBD
	}
	if (strncmp(str1, "spk", 31) == 0) {
		DBG_DUMP("SPK!\r\n");
		SxCmd_DoCommand("dx aout vdet 0");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
	if (strncmp(str1, "line", 31) == 0) {
		DBG_DUMP("LINE!\r\n");
		SxCmd_DoCommand("dx aout vdet 1");
		SwTimer_DelayMs(500); //wait for insert/remove detect
	}
	return TRUE;
}

SXCMD_BEGIN(sysaud, "audio command")
SXCMD_ITEM("aout %", Cmd_sys_aout, "change aout setting")
SXCMD_END()

