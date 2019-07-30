////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_UpdFw.h"
#if (FWS_FUNC == ENABLE)
#include "FwSrvApi.h"
#endif
#include "FileSysTsk.h"
#include "PartLoad.h"
#include "HwPower.h"
#include "AppControl.h"
#include "SysMain.h"
#include "UMSDC.h"
#include "cpu2util_api.h"
#include "emb_partition_info.h"
#include "bin_info.h"
#include <string.h>

/*
#if defined(_MODEL_EVB_)
#define CFG_SAFE_POWER_OFF DISABLE //because EVB no rtc power
#else
#define CFG_SAFE_POWER_OFF ENABLE //if disalbe it, use watch dog to reset system, but it may cause damage on peripheral.
#endif
*/

#define THIS_DBGLVL         2       // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          MsdcNvt_UpdFw
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//for MSDCVendorNVT_AddCallback_Bi
static void MsdcNvtCb_UpdFwGetWorkMem(void *pData);
static void MsdcNvtCb_UpdFwAllInOne_Step1(void *pData);
static void MsdcNvtCb_UpdFwAllInOne_Step2(void *pData);

MSDCNVT_REG_BI_BEGIN(m_MsdcNvtUpdfw)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwGetWorkMem)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwAllInOne_Step1)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwAllInOne_Step2)
MSDCNVT_REG_BI_END()

BOOL MsdcNvtRegBi_UpdFw(void)
{
	return MsdcNvt_AddCallback_Bi(m_MsdcNvtUpdfw);
}

static void MsdcNvtCb_UpdFwGetWorkMem(void *pData)
{
	PARTLOAD_INIT Init = {0};
	tMSDCEXT_UPDFW_GET_WORK_MEM *pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFW_GET_WORK_MEM, pData);
	if (pDesc == NULL) {
		return;
	}

	PartLoad_GetInit(&Init);
	pDesc->uiAddr = Init.uiWorkingAddr;
	pDesc->uiSize = Init.uiWorkingSize;

	pDesc->tParent.bOK = TRUE;
}

static void MsdcNvtCb_UpdFwAllInOne_Step1(void *pData)
{
#if (FWS_FUNC == ENABLE)
	tMSDCEXT_UPDFWALLINONE_STEP1 *pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFWALLINONE_STEP1, pData);
	if (pDesc == NULL) {
		return;
	}

#if (USB_MODE == ENABLE)
	//Change Mode to Main for Free Memory
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_USBMSDC) {
		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
			INT32 nRetry = 20;
			Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
			//polling to wait finish
			while (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN && nRetry-- > 0) {
				Delay_DelayMs(500);
			}
		}

		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
			pDesc->tParent.bOK = FALSE;
			DBG_ERR("cannot change mode to MAIN.\r\n");
			return;
		}

		//Delete Firmware in Cards
		DBG_DUMP("\r\nDelete firmware in A:\\ \r\n");
		if (FileSys_DeleteFile("A:\\"_BIN_NAME_T_".BIN") != FST_STA_OK) {
			DBG_IND("Ignore deleting file.\r\n");
		}

		if (FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN") != FST_STA_OK) {
			DBG_IND("Ignore deleting file.\r\n");
		}

		//#NT#2016/08/29#Niven Cho -begin
		//#NT#ETH-Update FW
		//System_CPU2_Stop();
		//#NT#2016/08/29#Niven Cho -end
	} else {
		//because we need to kill old fw in SD, so don't want pc accessing sd card.
		MSDC_OBJ *pMsdc = MsdcNvt_GetMsdcObj();
		if (pMsdc != NULL) {
			pMsdc->SetLunConfig(0, USBMSDC_LUN_CONFIG_ID_WRITEPROTECT, TRUE);
		}
		//Mount SD by uITRON
		// coverity[var_deref_model]
		GxStrg_SetConfig(FILE_CFG_FS_TYPE, FileSys_GetOPS_uITRON());
		if (GxStrg_OpenDevice(0, Dx_GetObject(DX_CLASS_STORAGE_EXT | DX_TYPE_CARD1)) == TRUE) {
			FileSys_WaitFinish();
		}
		//No need to stop cpu2, because it has closed whiling changing to msdc mode.

		//Delete Firmware in Cards
		DBG_DUMP("\r\nDelete firmware in A:\\ \r\n");
		if (FileSys_DeleteFile("A:\\"_BIN_NAME_T_".BIN") != FST_STA_OK) {
			DBG_IND("Ignore deleting file.\r\n");
		}

		if (FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN") != FST_STA_OK) {
			DBG_IND("Ignore deleting file.\r\n");
		}

	}
#else
	{
		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
			INT32 nRetry = 20;
			Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
			//polling to wait finish
			while (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN && nRetry-- > 0) {
				Delay_DelayMs(500);
			}
		}

		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
			pDesc->tParent.bOK = FALSE;
			DBG_ERR("cannot change mode to MAIN.\r\n");
			return;
		}

		//Delete Firmware in Cards
		DBG_DUMP("\r\nDelete firmware in A:\\ \r\n");
		if (FileSys_DeleteFile("A:\\"_BIN_NAME_T_".BIN") != FST_STA_OK) {
			DBG_IND("Ignore deleting file.\r\n");
		}

		if (FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN") != FST_STA_OK) {
			DBG_IND("Ignore deleting file.\r\n");
		}
		//#NT#2016/08/29#Niven Cho -begin
		//#NT#ETH-Update FW
		//System_CPU2_Stop();
		//#NT#2016/08/29#Niven Cho -end
	}
#endif

	pDesc->uiAddr = OS_GetMempoolAddr(POOL_ID_APP) + MSDC_MIN_BUFFER_SIZE;
	pDesc->uiSize = OS_GetMempoolSize(POOL_ID_APP) - MSDC_MIN_BUFFER_SIZE;

	pDesc->tParent.bOK = TRUE;
#endif
}

#if (FWS_FUNC == ENABLE)
static void system_reboot(void)
{
	DBG_IND("system_reboot\r\n");
/*
#if (CFG_SAFE_POWER_OFF == ENABLE)
	GxSystem_EnableHWReset(0);
#else
	GxSystem_EnableSWReset(0);
	DBG_WRN("Using SW Reset, it might be not safe for peripheral.\r\n");
#endif
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
*/
#if defined(_CLOCK_RTC_)
	GxSystem_EnableHWReset(0);
#elif defined(_CLOCK_DRTC_)
	GxSystem_EnableSWReset(0); //it may cause damage on peripheral, cause their power are not be reset here.
	DBG_WRN("Using SW Reset, it might be not safe for peripheral.\r\n");
#else
	#error (unknown CLOCK setting?)
#endif
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}
#endif

static void MsdcNvtCb_UpdFwAllInOne_Step2(void *pData)
{
#if (FWS_FUNC == ENABLE)
	FWSRV_CMD Cmd = {0};
	tMSDCEXT_UPDFWALLINONE_STEP2 *pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFWALLINONE_STEP2, pData);

	if (pDesc == NULL) {
		return;
	}

	if (pDesc->uiKey != MAKEFOURCC('N', 'O', 'V', 'A')) {
		debug_msg("MsdcNvtCb_UpdFwSetBlock: Invalid Key\r\n!");
		return;
	}

	UINT32 uiSrcAddr = OS_GetMempoolAddr(POOL_ID_APP) + MSDC_MIN_BUFFER_SIZE;
	UINT32 uiSrcSize = pDesc->EffectDataSize;
	UINT32 uiWorkAddr = uiSrcAddr + uiSrcSize;
	UINT32 uiWorkSize = OS_GetMempoolSize(POOL_ID_APP) - MSDC_MIN_BUFFER_SIZE - uiSrcSize;


	// verify firmware
	FWSRV_VERIFY_FW Verify = {0};
	Verify.Buf.uiAddr = uiSrcAddr;
	Verify.Buf.uiSize = uiSrcSize;
	Cmd.Idx = FWSRV_CMD_IDX_VERIFY_FW;
	Cmd.In.pData = &Verify;
	Cmd.In.uiNumByte = sizeof(Verify);
	Cmd.Prop.bExitCmdFinish = TRUE;
	if (FwSrv_Cmd(&Cmd) != FWSRV_ER_OK) {
		DBG_ERR("check sum failed.\r\n");
		pDesc->tParent.bOK = FALSE;
		return;
	}

	//#NT#2016/08/29#Niven Cho -begin
	//#NT#ETH-Update FW
	//must close CPU2 because root-fs updated for condition CPU2 OFF.
	System_CPU2_Stop();
	//#NT#2016/08/29#Niven Cho -end

	// load uboot partition
	FWSRV_LOAD_PARTITION Load = {0};
	Load.EmbType = EMBTYPE_UBOOT;
	Load.OrderIdx = 0;
	Load.WorkBuf.uiAddr = uiWorkAddr;
	Load.WorkBuf.uiSize = uiWorkSize;

	Cmd.Idx = FWSRV_CMD_IDX_LOAD_PARTITION;
	Cmd.In.pData = &Load;
	Cmd.In.uiNumByte = sizeof(Load);
	Cmd.Prop.bExitCmdFinish = TRUE;
	if (FwSrv_Cmd(&Cmd) != FWSRV_ER_OK) {
		DBG_ERR("load uboot failed.\r\n");
		pDesc->tParent.bOK = FALSE;
		return;
	}

	// start update
	HEADINFO *p_hdr = (HEADINFO *)(_BOARD_UBOOT_ADDR_ + BIN_INFO_OFFSET_UBOOT);
	CPU2UTIL_UPDFW upd_fw = {0};
	upd_fw.uboot_addr = p_hdr->CodeEntry;
	upd_fw.uboot_size = p_hdr->BinLength;
	upd_fw.fw_addr = uiSrcAddr;
	upd_fw.fw_size = uiSrcSize;
	if (cpu2util_updfw(&upd_fw) != 0) {
		DBG_ERR("update failed - 1.\r\n");
		pDesc->tParent.bOK = FALSE;
		return;
	}

	if (cpu2util_wait_updfw_done(5) != 0) {
		DBG_ERR("update failed - 2.\r\n");
		pDesc->tParent.bOK = FALSE;
		return;
	}

	cpu2util_terminate();

	system_reboot();

	pDesc->tParent.bOK = TRUE;
#endif
}

