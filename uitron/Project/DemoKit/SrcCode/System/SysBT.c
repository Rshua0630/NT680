/*
    System Main Callback

    System Callback for System Module.

    @file       SysMain_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#if(UI_FUNC==ENABLE)
#include "UIBackgroundObj.h"
#endif
#include "UICommon.h"
#include "AppLib.h"


//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
#include "PStore.h"
#if (BT_FUNC == ENABLE)
#include "BT_API.h"
#endif

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysBT
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


#if (BT_FUNC == ENABLE)
static void BT_LoadInfo(UINT32 Addr, UINT32 Size)
{
}

static void BT_SaveInfo(UINT32 Addr, UINT32 Size)
{
}

static void  BT_RcKeyCB(UINT8 *KeyData, UINT32 DataLen)
{
	UINT32 i;
	DBG_IND("DataLen = %d, KeyCode", DataLen);
	for (i = 0; i < DataLen; i++) {
		DBG_MSG(" 0x%02x", KeyData[i]);
	}
}

static void BT_Print_Rx_Data(UINT8 *p_data, UINT16 len)
{
	UINT16 i, j, offset = 0;

	/* Display incoming data */
	while (len > 0) {
		DBG_MSG("   %04X: ", offset);

		j = 16;
		if (len < 16) {
			j = len;
		}

		for (i = 0; i < j; i++) {
			DBG_MSG("%02X ", ((UINT8 *)p_data)[offset + i]);
		}

		DBG_MSG("\r\n");
		offset += j;
		len -= j;
	}
}
void BT_RfcommRxCB(UINT16 port_handle, void *p_data, UINT16 len)
{
	UINT16 byteWriten = 0;

	BT_Print_Rx_Data(p_data, len);

	BT_Rfcomm_WriteData(port_handle, p_data, len, &byteWriten);
}

#endif
void System_OnBTInit(void)
{
	DBG_IND("\r\n");
#if (BT_FUNC == ENABLE)
	BT_OPEN      btOpen = {0};
	btOpen.loadInfo = BT_LoadInfo;
	btOpen.saveInfo = BT_SaveInfo;
	btOpen.rckeyCB = BT_RcKeyCB;
	btOpen.rfcommRxCB = BT_RfcommRxCB;
	BT_Open(&btOpen);
#endif
}


void System_OnBTExit(void)
{
#if (BT_FUNC == ENABLE)
	BT_Close();
#endif
}

