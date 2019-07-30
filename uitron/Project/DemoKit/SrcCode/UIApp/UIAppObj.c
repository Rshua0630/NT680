#include "SysKer.h"
#include "SysCfg.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "Debug.h"
#include "AppLib.h"
#include "UIAppCommon.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppComm
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


extern VControl SystemObjCtrl;
extern VControl UISetupObjCtrl;

//the order of this array should be the same as APP_TYPE
VControl *gAppCtrlTypeList[APP_TYPE_MAX - APP_TYPE_MIN] = {
	&SystemObjCtrl,      //APP_BASE
	&UISetupObjCtrl,     //APP_SETUP
	0,                   //APP_PLAY
	0,                   //APP_PHOTO
	0,                   //APP_MOVIEPLAY
	0,                   //APP_MOVIEREC
	0,                   //APP_PRINT
	0,                   //APP_PCC,
	0,                   //APP_MSDC
#if(WIFI_AP_FUNC==ENABLE)
	0,                   //APP_WIFICMD
#endif
};

/*
#define APPMEM_MAX_NUM   10
MEM_RANGE gAppMem_All = {0};
UINT32 gAppMem_ID = 0;
MEM_RANGE gAppMem_List[APPMEM_MAX_NUM] = {0};
char* gAppMem_Name[APPMEM_MAX_NUM] = {0};
*/

void AppMem_Open(void)
{
	DBG_ERR("AppMem is not support!");
	/*
	    gAppMem_All.Addr = OS_GetMempoolAddr(POOL_ID_APP);
	    gAppMem_All.Size = OS_GetMempoolSize(POOL_ID_APP);
	    gAppMem_ID = 0;
	    DBG_IND("AppMem all = {%08x, %08x}\r\n", gAppMem_All.Addr, gAppMem_All.Size);
	*/
}

MEM_RANGE AppMem_Alloc(char *name, UINT32 size)
{
	MEM_RANGE n = {0};
	DBG_ERR("AppMem is not support!");
	/*
	    if(gAppMem_ID==APPMEM_MAX_NUM-1)
	    {
	        DBG_ERR("AppMem count %d is not enough!\r\n", gAppMem_ID);
	        return n;
	    }
	    if(size == APPMEM_ALL)
	    {
	        size = gAppMem_All.Size;
	    }
	    if((gAppMem_All.Size < size) || (gAppMem_All.Size == 0))
	    {
	        DBG_ERR("AppMem size %08x is not enough for %s %08x!\r\n", gAppMem_All.Size, name, size);
	        return n;
	    }
	    gAppMem_List[gAppMem_ID].Addr = gAppMem_All.Addr;
	    gAppMem_List[gAppMem_ID].Size = size;
	    gAppMem_Name[gAppMem_ID] = name;
	    DBG_IND("AppMem alloc %d %s = {%08x, %08x}\r\n", gAppMem_ID, name, gAppMem_List[gAppMem_ID].Addr, gAppMem_List[gAppMem_ID].Size);
	    gAppMem_All.Addr = gAppMem_All.Addr + size;
	    gAppMem_All.Size = gAppMem_All.Size - size;
	    n = gAppMem_List[gAppMem_ID];
	    gAppMem_ID++;
	*/
	return n;
}

void AppMem_Dump(void)
{
	DBG_ERR("AppMem is not support!");
	/*
	    UINT32 i;
	    for(i=0;i<gAppMem_ID;i++)
	    {
	    UINT32 addr = gAppMem_List[i].Addr;
	    UINT32 size = gAppMem_List[i].Size;
	    DBG_DUMP("   DyncBuf-%.2ld: Range[0x%08X~0x%08X] Size=0x%08X, POOL_ID_APP for [%s]\r\n",
	        i,
	        addr - 0x20000000,
	        addr - 0x20000000+size,
	        size,
	        gAppMem_Name[i]
	        );
	    }
	*/
}
void AppMem_Close(void)
{
	DBG_ERR("AppMem is not support!");
	/*
	    gAppMem_ID = 0;
	*/
}

