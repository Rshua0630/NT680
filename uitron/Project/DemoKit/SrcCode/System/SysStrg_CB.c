/**
    DevMan, Service command function implementation

    @file       DeviceUsbMan.c
    @ingroup    mDEVMAN

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysStrgExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#include "SysKer.h"
#include "Debug.h"
#include "NvtUser.h"
//#NT#2016/09/19#Niven Cho -begin
//#NT#SD_HOTPLUG
#include "NVTUserCommand.h"
//#NT#2016/09/19#Niven Cho -end

//#include "NvtSystem.h"
#include "GxSystem.h"
#include "AppControl.h"
#include "SysMain.h"
#include "Delay.h"

/////////////////////////////////////////////////////////////////////////////
// CARD

#include "GxStrg.h"
#include "FileSysTsk.h"

void Strg_CB(UINT32 event, UINT32 param1, UINT32 param2);

void Strg_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	UINT32 status;
	switch (event) {
// part-1
	case SYSTEM_CB_CONFIG:
		//2.設定CB值,
		//3.註冊SxJob服務 ---------> System Job
		//4.註冊SxTimer服務 ---------> Detect Job
		//5.註冊SxCmd服務 ---------> Cmd Function
		//System_AddSxCmd(Storage_OnCommand); //GxStorage
		break;
	case STRG_CB_INSERTED: //call after card inserted
		DBG_IND("CARD-%d inserted\r\n", param1 + 1);
		//#NT#2016/03/02#Niven Cho -begin
		//#NT#Fix FAST_BOOT + LINUX + without card, boot failed.
		Ux_PostEvent(NVTEVT_STRG_INSERT, 1, param1);
		Ux_PostEvent(NVTEVT_STRG_CHANGE, 1, param1);

		//#NT#2016/03/02#Niven Cho -end
		break;
	case STRG_CB_REMOVED: //call after card removed
		DBG_IND("CARD-%d removed\r\n", param1 + 1);
		#if (FS_MULTI_STRG_FUNC == DISABLE)
		// ignore card2, card3 removed event when FS_MULTI_STRG_FUNC is disable
		if (param1 > 0) {
			break;
		}
		#endif
		Ux_PostEvent(NVTEVT_STRG_CHANGE, 1, param1);
		Ux_PostEvent(NVTEVT_STRG_REMOVE, 1, param1);
		break;
	case STRG_CB_UNKNOWN: //call if card status is unknown
		DBG_IND("CARD-%d unknown\r\n", param1 + 1);

		DBG_IND("File abort\r\n");
		{
			// abort file system
			///FileSys_SetState(FST_STATE_ABORT,FST_CMD_CLOSE_FILE);
			///FileSys_SetState(FST_STATE_ABORT,FST_CMD_DELETE_FILE);
			///FileSys_SetState(FST_STATE_ABORT,FST_CMD_FORMAT);
		}

		DBG_ERR("^RCard unknown\r\n");
		{
			//System_PowerOff(SYS_POWEROFF_NORMAL);
		}
		break;
	case STRG_CB_UNMOUNT_FINISH:
		status = param2;
		switch (status) {
		case FST_STA_OK:
			DBG_MSG("card-%d unmount OK\r\n", param1 + 1);
			break;
		default:
			DBG_MSG("^Rcard-%d unmount FAIL\r\n", param1 + 1);
			break;
		}
		Ux_PostEvent(NVTEVT_STRG_DETACH, 2, param1, status);
		break;
	case STRG_CB_MOUNT_FINISH: //after finish change storage
		status = param2;
		switch (status) {
		case FST_STA_OK:
			DBG_MSG("card-%d mount OK\r\n", param1 + 1);
			break;
		case FST_STA_DISK_UNFORMAT:
			DBG_MSG("^Rcard-%d mount FAIL: Unformat\r\n", param1 + 1);
			break;
		case FST_STA_DISK_UNKNOWN_FORMAT:
			DBG_MSG("^Rcard-%d mount FAIL: Unknown Format\r\n", param1 + 1);
			break;
		case FST_STA_CARD_ERR:
			DBG_MSG("^Rcard-%d mount FAIL: Card Error\r\n", param1 + 1);
			break;
#if 0
		case FST_STA_OUT_OF_ID:
			DBG_MSG("^Rcard-%d mount FAIL: Out of ID\r\n", stg_id + 1);
			break;
#endif
		default:
			DBG_MSG("^Rcard-%d mount FAIL: ErrID=%d\r\n", param1 + 1, status);
		}
		{
			static BOOL bFirst = TRUE;
			if (bFirst) {
				TM_BOOT_END("sdio", "mount_fs");
			}
			bFirst = FALSE;
		}
#if (POWERON_FAST_BOOT == ENABLE)
		INIT_SETFLAG(FLGINIT_MOUNTFS);
#endif
		Ux_PostEvent(NVTEVT_STRG_ATTACH, 2, param1, status);
		break;

	default:
		DBG_ERR("Strg_CB Err event(%d)", event);
		break;
	}
}


