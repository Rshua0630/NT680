/*
    System Sensor Callback

    System Callback for Sensor Module.

    @file       SysSensor_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
#include "UIAppPhoto.h" //for CURR_EV()
#include "MenuSetupLCDBrt.h" //for MenuSetup_GetV2BacklightValue()
#endif
#include "sensor.h"
#include "DxSensor.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_VdoIn.h"
#include "PStore.h"
#include "GxFlash.h"
#include "ipl_obj.h"
#include "ipl_api.h"
#include "rsc_api.h"
#if (CALIBRATION_FUNC == ENABLE)
#include "ipl_calibration.h"
#include "calibration_item.h"
#include "ipl_cal_api.h"
#include "EngineerMode.h"
#endif
#if(WIFI_FUNC==ENABLE)
//#NT#2016/06/23#Isiah Chang -begin
//#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
#include "WifiAppCmd.h"
#include "UIModeWifi.h"
//#NT#2016/06/23#Isiah Chang -end
#endif
#include "SysSensor.h"
#include "bin_info.h"
#include "DrvExt.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysSensorExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

BOOL bEnableAE = FALSE;
BOOL bStableAE = FALSE;

#if (VDOIN_MAX_NUM == 8)
INT32     uiSensorMapping[VDOIN_MAX_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1};
ISF_UNIT *gVdoIn[VDOIN_MAX_NUM] = {
	&ISF_VdoIn1,
	&ISF_VdoIn2,
	&ISF_VdoIn3,
	&ISF_VdoIn4,
	&ISF_VdoIn5,
	&ISF_VdoIn6,
	&ISF_VdoIn7,
	&ISF_VdoIn8,
};

ISF_UNIT *gImagePipe[VDOIN_MAX_NUM] = {
	&ISF_ImagePipe1,
	&ISF_ImagePipe2,
	&ISF_ImagePipe3,
	&ISF_ImagePipe4,
	&ISF_ImagePipe5,
	&ISF_ImagePipe6,
	&ISF_ImagePipe7,
	&ISF_ImagePipe8,
};
#endif
#if (VDOIN_MAX_NUM == 2)
INT32 uiSensorMapping[VDOIN_MAX_NUM] = {-1, -1};
ISF_UNIT *gVdoIn[VDOIN_MAX_NUM] = {
	&ISF_VdoIn1,
	&ISF_VdoIn2,
};
ISF_UNIT *gImagePipe[VDOIN_MAX_NUM] = {
	&ISF_ImagePipe1,
	&ISF_ImagePipe2,
};
#endif
static BOOL bFirst = TRUE;
#if (SENSOR_CAPS_COUNT == 1)
static INT32 gSensor_iLastMode = -1; //keep last UI mode
#endif

#if (SENSOR_INSERT_FUNCTION == ENABLE)
void DetSensor(void);
int SX_TIMER_DET_SENSOR_ID = -1;
SX_TIMER_ITEM(DetSensor, DetSensor, 25, FALSE)
#endif

#if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
int SX_TIMER_DET_EV_ID = -1;
void Sensor_DetEV(void);
SX_TIMER_ITEM(Sensor_DetEV, Sensor_DetEV, 50, TRUE)
#endif

#if (FLASHLIGHT_FUNCTION == ENABLE)
int SX_TIMER_DET_FLASH_ID = -1;
int SX_TIMER_DET_RECHARGE_ID = -1;
SX_TIMER_ITEM(GxFlash_DetFlash, GxFlash_DetFlash, 5, TRUE)
SX_TIMER_ITEM(GxFlash_DetReCharge, GxFlash_DetReCharge, 50, TRUE)
#endif

void System_EnableSensor(UINT32 SensorMask);
static void _System_MapSensor(void);

#if 0//(CALIBRATION_FUNC == ENABLE) // [cal-mark]
static UINT32 CalRwStoreOp_OnOpen(char *pSecName, CAL_RW_OPEN_SEL OpenSel);
static ER CalRwStoreOp_OnClose(UINT32 pSectionHandleAddr);
static ER CalRwStoreOp_OnRead(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr);
static ER CalRwStoreOp_OnWrite(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr);
static ER CalRwStoreOp_OnDelete(char *pSecName);
static CAL_RW_STORE_OP m_CalRwStoreOp = {CalRwStoreOp_OnOpen, CalRwStoreOp_OnClose, CalRwStoreOp_OnRead, CalRwStoreOp_OnWrite, CalRwStoreOp_OnDelete};

static UINT32 CalRwStoreOp_OnOpen(char *pSecName, CAL_RW_OPEN_SEL OpenSel)
{
	UINT32 hHandle = 0;
#if (PST_FUNC == ENABLE)
	UINT32 RWOperation = 0;
	RWOperation |= (OpenSel & CAL_RD_ONLY) ? PS_RDONLY : 0;
	RWOperation |= (OpenSel & CAL_WR_ONLY) ? PS_WRONLY : 0;
	RWOperation |= (OpenSel & CAL_CREATE) ? PS_CREATE : 0;
	hHandle = (UINT32)PStore_OpenSection(pSecName, RWOperation);
#endif
	return hHandle;
}

static ER CalRwStoreOp_OnClose(UINT32 pSectionHandleAddr)
{
	ER er = E_SYS;
#if (PST_FUNC == ENABLE)
	er = PStore_CloseSection((PSTORE_SECTION_HANDLE *) pSectionHandleAddr);
#endif
	return er;
}

static ER CalRwStoreOp_OnRead(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr)
{
	ER er = E_SYS;
#if (PST_FUNC == ENABLE)
	er = PStore_ReadSection(pcBuffer, ulStartAddress, ulDataLength, (PSTORE_SECTION_HANDLE *) pSectionHandleAddr);
#endif
	return er;
}

static ER CalRwStoreOp_OnWrite(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, UINT32 pSectionHandleAddr)
{
	ER er = E_SYS;
#if (PST_FUNC == ENABLE)
	er = PStore_WriteSection(pcBuffer, ulStartAddress, ulDataLength, (PSTORE_SECTION_HANDLE *) pSectionHandleAddr);
#endif
	return er;
}

static ER CalRwStoreOp_OnDelete(char *pSecName)
{
	ER er = E_SYS;
#if (PST_FUNC == ENABLE)
	er = PStore_DeleteSection(pSecName);
#endif
	return er;
}
#endif

#if (SENSOR_GP_FUNC == ENABLE)
static void System_SensorGroupCfg(void)
{
	SENSOR_MULTI_ID sensor_gp[SENSOR_ID_MAX_NUM], sensor_subgp[SENSOR_ID_MAX_NUM];

	#if (SENSOR_ID_MAX_NUM == 8)
	sensor_gp[0] = SENSOR_GP_1;
	sensor_gp[1] = SENSOR_GP_2;
	sensor_gp[2] = SENSOR_GP_3;
	sensor_gp[3] = SENSOR_GP_4;
	sensor_gp[4] = SENSOR_GP_5;
	sensor_gp[5] = SENSOR_GP_6;
	sensor_gp[6] = SENSOR_GP_7;
	sensor_gp[7] = SENSOR_GP_8;

	sensor_subgp[0] = SENSOR_SUBGP_1;
	sensor_subgp[1] = SENSOR_SUBGP_2;
	sensor_subgp[2] = SENSOR_SUBGP_3;
	sensor_subgp[3] = SENSOR_SUBGP_4;
	sensor_subgp[4] = SENSOR_SUBGP_5;
	sensor_subgp[5] = SENSOR_SUBGP_6;
	sensor_subgp[6] = SENSOR_SUBGP_7;
	sensor_subgp[7] = SENSOR_SUBGP_8;
	#else
	sensor_gp[0] = SENSOR_GP_1;
	sensor_gp[1] = SENSOR_GP_2;
	sensor_subgp[0] = SENSOR_SUBGP_1;
	sensor_subgp[1] = SENSOR_SUBGP_2;
	#endif
	drv_sensor_group_cfg(sensor_gp, sensor_subgp);
}
#endif

#if 0
void System_DynamicSensorInit(void)
{
	MODELEXT_HEADER *header = NULL;
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
	if (!pBinInfo || !header) {
		DBG_ERR("bininfo is null\r\n");
		return;
	}
	DBGD(pBinInfo->fw.Resv[0]);
	_System_MapSensor();
	//System_ChangeSensorName();
}
#endif

void System_OnSensorInit(void)
{
	TM_BOOT_BEGIN("sensor", "init");
	//PHASE-1 : Init & Open Drv or DrvExt
	{
#if 0//(CALIBRATION_FUNC == ENABLE) // [cal-mark]
		CAL_DMA_RST rst = {0};
		CAL_RegRWFunc(&m_CalRwStoreOp);
		CAL_RW()->fpSectionAllRead(OS_GetMempoolAddr(POOL_ID_CAL), POOL_SIZE_CAL);
		if (rst.RemainSize > POOL_SIZE_CAL) {
			DBG_ERR("POOL_SIZE_CAL is not enough. \r\n");
		}
#endif
	}
	#if (SENSOR_GP_FUNC == ENABLE)
	System_SensorGroupCfg();
	#endif
	// Dynamic change sensor
	#if 0
	System_DynamicSensorInit();
	#endif
	// init sensor clock
	drv_sensor_first_cfg();
	TM_BOOT_END("sensor", "init");
}
extern void _System_InitSensor(void);
extern UINT32 System_GetConnectSensor(void);
void System_EnableSensor_FastDevice(void);
void System_EnableSensor_FastUnit(void);
void System_OnSensorInit2(void)
{
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL) {
		INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
		if (iCurrMode == PRIMARY_MODE_PHOTO) {
			System_EnableSensor_FastDevice(); //enable 1 sensors
		} else
#endif
#if (MOVIE_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_MOVIE) {
				System_EnableSensor_FastDevice(); //enable 1 sensors
			}
#endif
#if(IPCAM_MODE ==ENABLE)
		if (iCurrMode == PRIMARY_MODE_IPCAM) {
			System_EnableSensor_FastDevice(); //enable 1 sensors
		}
#endif
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
		if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
			INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_PHOTO) {
				System_EnableSensor_FastDevice(); //enable 1 sensors
			} else
#endif
#if (MOVIE_MODE==ENABLE)
				if (iCurrMode == PRIMARY_MODE_MOVIE) {
					System_EnableSensor_FastDevice(); //enable 1 sensors
				}
#endif
#if(IPCAM_MODE == ENABLE)
			if (iCurrMode == PRIMARY_MODE_IPCAM) {
				System_EnableSensor_FastDevice(); //enable 1 sensors
			}
#endif
		}
#endif
#if (SENSOR_CAPS_COUNT == 4)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
		if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)) {
			INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_PHOTO) {
				System_EnableSensor_FastDevice(); //enable 1 sensors
			} else
#endif
#if (MOVIE_MODE==ENABLE)
				if (iCurrMode == PRIMARY_MODE_MOVIE) {
					System_EnableSensor_FastDevice(); //enable 1 sensors
				}
#endif
#if (IPCAM_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_IPCAM) {
				System_EnableSensor_FastDevice(); //enable 1 sensors
			}
#endif
		}
#endif
#if (SENSOR_CAPS_COUNT == 8)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
		if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4 | SENSOR_5 | SENSOR_6 | SENSOR_7 | SENSOR_8)) {
			INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_PHOTO) {
				System_EnableSensor_FastDevice(); //enable 1 sensors
			} else
#endif
#if (MOVIE_MODE==ENABLE)
				if (iCurrMode == PRIMARY_MODE_MOVIE) {
					System_EnableSensor_FastDevice(); //enable 1 sensors
				}
#endif
#if (IPCAM_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_IPCAM) {
				System_EnableSensor_FastDevice(); //enable 1 sensors
			}
#endif
		}
#endif
}
INT32 System_GetFirstSensorMode(INT32 iCurrMode)
{
#if (SENSOR_CAPS_COUNT == 1)
	_System_InitSensor();
	if (System_GetConnectSensor() == 0) {
		gSensor_iLastMode = iCurrMode; //keep last mode
		iCurrMode = PRIMARY_MODE_MAIN;
	}
	return iCurrMode;
#elif (SENSOR_CAPS_COUNT == 2)
	_System_InitSensor();
	if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
		//keep this mode
	}
	return iCurrMode;
#elif (SENSOR_CAPS_COUNT == 4)
	_System_InitSensor();
	if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)) {
		//keep this mode
	}
	return iCurrMode;
#elif (SENSOR_CAPS_COUNT == 8)
	_System_InitSensor();
	if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4 | SENSOR_5 | SENSOR_6 | SENSOR_7 | SENSOR_8)) {
		//keep this mode
	}
	return iCurrMode;
#else
	return iCurrMode;
#endif
}
void System_OnSensorInit3(void)
{
#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL) {
		INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
		if (iCurrMode == PRIMARY_MODE_PHOTO) {
			System_EnableSensor_FastUnit(); //enable 1 sensors
		} else
#endif
#if (MOVIE_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_MOVIE) {
				System_EnableSensor_FastUnit(); //enable 1 sensors
			}
#endif
#if (IPCAM_MODE==ENABLE)
		if (iCurrMode == PRIMARY_MODE_IPCAM) {
			System_EnableSensor_FastUnit(); //enable 1 sensors
		}
#endif
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
		if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
			INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_PHOTO) {
				System_EnableSensor_FastUnit(); //enable 1 sensors
			} else
#endif
#if (MOVIE_MODE==ENABLE)
				if (iCurrMode == PRIMARY_MODE_MOVIE) {
					System_EnableSensor_FastUnit(); //enable 1 sensors
				}
#endif
#if (IPCAM_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_IPCAM) {
				System_EnableSensor_FastUnit(); //enable 1 sensors
			}
#endif
		}
#endif
#if (SENSOR_CAPS_COUNT == 4)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
		if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)) {
			INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_PHOTO) {
				System_EnableSensor_FastUnit(); //enable 1 sensors
			} else
#endif
#if (MOVIE_MODE==ENABLE)
				if (iCurrMode == PRIMARY_MODE_MOVIE) {
					System_EnableSensor_FastUnit(); //enable 1 sensors
				}
#endif
#if(IPCAM_MODE == ENABLE)
			if (iCurrMode == PRIMARY_MODE_IPCAM) {
				System_EnableSensor_FastUnit(); //enable 1 sensors
			}
#endif
		}
#endif
#if (SENSOR_CAPS_COUNT == 8)
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
		if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4 | SENSOR_5 | SENSOR_6 | SENSOR_7 | SENSOR_8)) {
			INT32 iCurrMode = System_GetBootFirstMode();
#if (PHOTO_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_PHOTO) {
				System_EnableSensor_FastUnit(); //enable 1 sensors
			} else
#endif
#if (MOVIE_MODE==ENABLE)
				if (iCurrMode == PRIMARY_MODE_MOVIE) {
					System_EnableSensor_FastUnit(); //enable 1 sensors
				}
#endif
#if(IPCAM_MODE == ENABLE)
			if (iCurrMode == PRIMARY_MODE_IPCAM) {
				System_EnableSensor_FastUnit(); //enable 1 sensors
			}
#endif
		}
#endif
	//PHASE-2 : Init & Open Lib or LibExt
	{
#if (SENSOR_INSERT_FUNCTION == ENABLE)
		SX_TIMER_DET_SENSOR_ID = SxTimer_AddItem(&Timer_DetSensor);
#endif
#if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
		SX_TIMER_DET_EV_ID = SxTimer_AddItem(&Timer_Sensor_DetEV);
#endif
#if (FLASHLIGHT_FUNCTION == ENABLE)
		SX_TIMER_DET_FLASH_ID = SxTimer_AddItem(&Timer_GxFlash_DetFlash);
		SX_TIMER_DET_RECHARGE_ID = SxTimer_AddItem(&Timer_GxFlash_DetReCharge);
#endif
#if ((SENSOR_INSERT_FUNCTION == ENABLE))
		SxTimer_SetFuncActive(SX_TIMER_DET_SENSOR_ID, TRUE);
#endif
	}
}

void System_OnSensorExit(void)
{
	//PHASE-2 : Close Lib or LibExt
	{
		System_EnableSensor(0); //disable all sensors
	}
	//PHASE-1 : Close Drv or DrvExt
	{
	}
}

///////////////////////////////////////////////////////////////////////
#if (SENSOR_INSERT_FUNCTION == ENABLE)
void DetSensor(void)
{
#if (SENSOR_INSERT_MASK == SENSOR_1)
	static BOOL cp = 0;
	BOOL p = drv_sensor_det_plug_in(IPL_PATH(0));
	if ((cp == 0) && (p == 1)) {
		Ux_PostEvent(NVTEVT_SENSOR_INSERT, 1, 0); //SENSOR 1
	} else if ((cp == 1) && (p == 0)) {
		Ux_PostEvent(NVTEVT_SENSOR_REMOVE, 1, 0); //SENSOR 1
	}
	cp = p;
#endif
#if (SENSOR_INSERT_MASK == SENSOR_2)
	static BOOL cp = 0;
	BOOL p = drv_sensor_det_plug_in(IPL_PATH(1));
	if ((cp == 0) && (p == 1)) {
		Ux_PostEvent(NVTEVT_SENSOR_INSERT, 1, 1); //SENSOR 2
	} else if ((cp == 1) && (p == 0)) {
		Ux_PostEvent(NVTEVT_SENSOR_REMOVE, 1, 1); //SENSOR 2
	}
	cp = p;
#endif
}
#endif

#if (LCD_AUTOBACKLIGHT_FUNCTION == ENABLE)
extern BOOL g_bIsInitSystemFinish;

void Sensor_DetEV(void)
{
	//check is enable to AUTO adjust?
	if (UI_GetData(FL_BrightAdjIndex) !=  LCDBRT_ADJ_AUTO) {
		return;
	}
	//Sleep not control backlight
	if (GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 0) {
		return;
	}
	//Playback mode can not get LV value
	if (UI_GetData(FL_ModeIndex) == DSC_MODE_PLAYBACK) {
		return;
	}

	{
		UINT32 uiLV;
		uiLV = CURR_EV() / 10;
		if (g_bIsInitSystemFinish) {
			//DetBLcount++;
			//if (DetBLcount > 2)
			{
				//debug_msg("CURR_EV()=%d\r\n", CURR_EV());
				//debug_msg("LV=%d\r\n", uiLV);
				//debug_msg(" set bright=%d\r\n",MenuSetup_GetV2BacklightValue(uiLV));
				Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_BRIGHTLVL, 1, MenuSetup_GetV2BacklightValue(uiLV));
			}
		}
	}
}
#endif

static UINT32 uiSensorEnableState_fw = 0; //fw attach sensor
static UINT32 uiSensorEnableState_hw = SENSOR_DEFAULT_ATTACH_MASK; //hw attach sensor (default value = fix attach sensors)
static UINT32 uiSensorDispState = SENSOR_DEFAULT_DISPLAY_MASK; //display sensor
static BOOL g_bBypassImageUnit = FALSE;

void _System_InitSensor(void)
{
#if (SENSOR_INSERT_MASK == SENSOR_1)
	if (drv_sensor_det_plug_in(IPL_PATH(0))) {
		uiSensorEnableState_hw |= SENSOR_1;
	}
#endif
#if (SENSOR_INSERT_MASK == SENSOR_2)
	if (drv_sensor_det_plug_in(IPL_PATH(1))) {
		uiSensorEnableState_hw |= SENSOR_2;
	}
#endif
}

static void _System_MapSensor(void)
{
	UINT32 i;
	#if (VDOIN_MAX_NUM == 8)
	uiSensorMapping[0] = SENSOR_MAP_1;
	uiSensorMapping[1] = SENSOR_MAP_2;
	uiSensorMapping[2] = SENSOR_MAP_3;
	uiSensorMapping[3] = SENSOR_MAP_4;
	uiSensorMapping[4] = SENSOR_MAP_5;
	uiSensorMapping[5] = SENSOR_MAP_6;
	uiSensorMapping[6] = SENSOR_MAP_7;
	uiSensorMapping[7] = SENSOR_MAP_8;
	#endif
	#if (VDOIN_MAX_NUM == 2)
	uiSensorMapping[0] = SENSOR_MAP_1;
	uiSensorMapping[1] = SENSOR_MAP_2;
	#endif
	for (i=0;i<VDOIN_MAX_NUM;i++){
		if (uiSensorMapping[i] !=-1){
			uiIsfIPLMapping[i] = gImagePipe[uiSensorMapping[i]];
			uiIsfVInMapping[i] = gVdoIn[uiSensorMapping[i]];
		}
	}
	DBG_MSG("Sensor Mapping\r\n");
}

INT32 System_MapSensorID(INT32 i)
{
	return uiSensorMapping[i];
}



static void _System_AttachSensor(UINT32 phy_SensorID)
{
	SENSOR_HANDLE sen_handle = {0};
	if (phy_SensorID >= IPL_PATH_MAX_NUM) {
		DBG_ERR("phy_SensorID %08x\r\n", phy_SensorID);
		return;
	}
	//turn on sensor power
	DBG_MSG("Sensor PowerOn [%d]\r\n", phy_SensorID);
	if (bFirst) {
		TM_BOOT_BEGIN("sensor", "poweron");
	}
	drv_sensor_power_turn_on(phy_SensorID);
	if (drv_sensor_init(phy_SensorID, &sen_handle) == FALSE){
		DBG_ERR("sensor_init fail\r\n");
		return;
	}
	if (bFirst) {
		TM_BOOT_END("sensor", "poweron");
	}
	//open sensor
	DBG_MSG("Sensor Open [%d]\r\n", phy_SensorID);
	if (bFirst) {
		TM_BOOT_BEGIN("sensor", "open");
	}
	sensor_open(phy_SensorID, &sen_handle);
	ipl_ext_init(phy_SensorID);

#if (CALIBRATION_FUNC == ENABLE)
	cal_system_init();
#endif
	rsc_ext_init(phy_SensorID);
	if (bFirst) {
		TM_BOOT_END("sensor", "open");
	}
	if (!g_bBypassImageUnit) {
		//attach sensor to ImagePipe
		ImageUnit_Begin(gVdoIn[phy_SensorID], 0);
		ImageUnit_SetParam(ISF_CTRL, VDOIN_PARAM_SENSORATTACH, phy_SensorID);
		ImageUnit_End();
	}
}

//
static void _System_DetachSensor(UINT32 phy_SensorID)
{
	if (phy_SensorID >= IPL_PATH_MAX_NUM) {
		DBG_ERR("phy_SensorID %08x\r\n", phy_SensorID);
		return;
	}
	//detach sensor from ImagePipe
	ImageUnit_Begin(gVdoIn[phy_SensorID], 0);
	ImageUnit_SetParam(ISF_CTRL, VDOIN_PARAM_SENSORDETACH, phy_SensorID);
	ImageUnit_End();
#if 1
	//close sensor
	DBG_MSG("Sensor Close [%d]\r\n", phy_SensorID);
	ipl_ext_uninit(phy_SensorID);
#if (CALIBRATION_FUNC == ENABLE)
	cal_system_uninit();
#endif
	sensor_close(phy_SensorID);
	//turn off sensor power
	DBG_MSG("Sensor PowerOff [%d]\r\n", phy_SensorID);
	drv_sensor_power_turn_off(phy_SensorID);
#endif
}
static void _System_ConnectSensor(UINT32 SensorMask)
{
	uiSensorEnableState_hw |= SensorMask;
}
static void _System_DisconnectSensor(UINT32 SensorMask)
{
	uiSensorEnableState_hw &= ~SensorMask;
}
UINT32 System_GetConnectSensor(void)
{
	return uiSensorEnableState_hw;
}

extern
void System_DispSource(UINT32 SensorMask, UINT32 nSensor);

void System_DispSensor(UINT32 SensorMask)
{
	UINT32 nSensor;
	nSensor = ImageUnit_GetParam(ISF_VIN(0), ISF_CTRL, VDOIN_PARAM_SENSORCOUNT); //return 0/1/2/4

	//config display sensor
	ImageUnit_Begin(ISF_VIN(0), 0);
	ImageUnit_SetParam(ISF_CTRL, VDOIN_PARAM_SENSORDISPLAY, SensorMask);
	ImageUnit_End();

	//config display source
	System_DispSource(SensorMask, nSensor);
}

UINT32 System_GetEnableSensor(void)
{
	return uiSensorEnableState_fw;
}
UINT32 System_GetEnableSensorCount(void)
{
	UINT32 c = 0, i;
	UINT32 mask = 1;
	for (i = 0; i < VDOIN_MAX_NUM; i++) {
		if (mask & uiSensorEnableState_fw) {
			c++;
		}
		mask <<= 1;
	}
	return c;
}

void System_EnableSensor(UINT32 SensorMask)
{
	static UINT32 uiSensorEnable = 0;
	static UINT32 uiSensorDisable = 0;
	if (SensorMask == SENSOR_ALL) {
		SensorMask = System_GetConnectSensor();
	}

	DBG_IND("enable mask=%08x\r\n", SensorMask);
	DBG_MSG("fw=%08x\r\n", uiSensorEnableState_fw);
	DBG_MSG("hw=%08x\r\n", uiSensorEnableState_hw);
	if (SensorMask & (~uiSensorEnableState_hw)) {
		DBG_ERR("Some sensor(s) [%08x] are NOT exist!\r\n", SensorMask & (~uiSensorEnableState_hw));
		return;
	}
	uiSensorEnable = (SensorMask & (~uiSensorEnableState_fw));
	DBG_MSG("en=%08x\r\n", uiSensorEnable);
	uiSensorDisable = ((~SensorMask) & (uiSensorEnableState_fw));
	DBG_MSG("de=%08x\r\n", uiSensorDisable);
	if ((uiSensorDisable & SENSOR_1) && (uiSensorEnableState_fw & SENSOR_1)) {
		_System_DetachSensor(IPL_PATH(0));
		uiSensorEnableState_fw &= ~SENSOR_1;
		uiSensorDisable &= ~SENSOR_1;
	}
	if ((uiSensorEnable & SENSOR_1) && !(uiSensorEnableState_fw & SENSOR_1)) {
		_System_AttachSensor(IPL_PATH(0));
		uiSensorEnableState_fw |= SENSOR_1;
		uiSensorEnable &= ~SENSOR_1;
	}
	if ((uiSensorDisable & SENSOR_2) && (uiSensorEnableState_fw & SENSOR_2)) {
		_System_DetachSensor(IPL_PATH(1));
		uiSensorEnableState_fw &= ~SENSOR_2;
		uiSensorDisable &= ~SENSOR_2;
	}
	if ((uiSensorEnable & SENSOR_2) && !(uiSensorEnableState_fw & SENSOR_2)) {
		_System_AttachSensor(IPL_PATH(1));
		uiSensorEnableState_fw |= SENSOR_2;
		uiSensorEnable &= ~SENSOR_2;
	}
#if SENSOR_CAPS_COUNT > 2
	if ((uiSensorDisable & SENSOR_3) && (uiSensorEnableState_fw & SENSOR_3)) {
		_System_DetachSensor(IPL_PATH(2));
		uiSensorEnableState_fw &= ~SENSOR_3;
		uiSensorDisable &= ~SENSOR_3;
	}
	if ((uiSensorEnable & SENSOR_3) && !(uiSensorEnableState_fw & SENSOR_3)) {
		_System_AttachSensor(IPL_PATH(2));
		uiSensorEnableState_fw |= SENSOR_3;
		uiSensorEnable &= ~SENSOR_3;
	}
#endif
#if SENSOR_CAPS_COUNT > 3
	if ((uiSensorDisable & SENSOR_4) && (uiSensorEnableState_fw & SENSOR_4)) {
		_System_DetachSensor(IPL_PATH(3));
		uiSensorEnableState_fw &= ~SENSOR_4;
		uiSensorDisable &= ~SENSOR_4;
	}
	if ((uiSensorEnable & SENSOR_4) && !(uiSensorEnableState_fw & SENSOR_4)) {
		_System_AttachSensor(IPL_PATH(3));
		uiSensorEnableState_fw |= SENSOR_4;
		uiSensorEnable &= ~SENSOR_4;
	}
#endif
}
void System_EnableSensor_FastDevice(void)
{
	g_bBypassImageUnit = TRUE;
	if (bFirst) {
		_System_MapSensor();
	}
#if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_1)
	System_EnableSensor(SENSOR_1);
#endif
#if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_2)
	System_EnableSensor(SENSOR_2);
#endif
	g_bBypassImageUnit = FALSE;
}
void System_EnableSensor_FastUnit(void)
{
#if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_1)
	ImageUnit_Begin(gVdoIn[IPL_PATH(0)], 0);
	ImageUnit_SetParam(ISF_CTRL, VDOIN_PARAM_SENSORATTACH, IPL_PATH(0));
	ImageUnit_End();
#endif
#if (SENSOR_DEFAULT_ATTACH_MASK & SENSOR_2)
	ImageUnit_Begin(gVdoIn[IPL_PATH(1)], 0);
	ImageUnit_SetParam(ISF_CTRL, VDOIN_PARAM_SENSORATTACH, IPL_PATH(1));
	ImageUnit_End();
#endif
}

#if defined(_BSP_NA51000_)
static UINT32 uiSensorLUT[4] = {
#else
static UINT32 uiSensorLUT[2] = {
#endif
	SENSOR_1,
	SENSOR_2,
#if defined(_BSP_NA51000_)
	SENSOR_3,
	SENSOR_4,
#endif
};

INT32 System_OnSensorInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 new_device_id; //sensor id
	if (paramNum != 1) {
		return NVTEVT_CONSUME;
	}
	new_device_id = (UINT32)(paramArray[0]); //IPL_PATH_1 ~ IPL_PATH_4
	DBG_DUMP("^Y===================================== sensor plug [SENSOR_%d]\r\n", new_device_id + 1);

	// connect sensor IPL_PATH_n
	_System_ConnectSensor(uiSensorLUT[new_device_id]);

#if 0
	//check is current open device?
	if (System_GetEnableSensor() == 0) {
		//not current device! => only config display for next time.
		return NVTEVT_CONSUME;
	}
#endif

#if(WIFI_FUNC==ENABLE)
//#NT#2016/06/23#Isiah Chang -begin
//#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE), SYS_SUBMODE_WIFI);

		Delay_DelayMs(500); // Delay to ensure preview is ready. Might use other sync. mechanism.

#if _TODO
		WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_SENSOR_NUM_CHANGED);
#endif

		return NVTEVT_CONSUME;
	}
//#NT#2016/06/23#Isiah Chang -end
#endif

	//fix for CID 44954 - begin
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_1))
	if (uiSensorLUT[new_device_id] == SENSOR_1) {
		INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		// exit main mode, return to last mode
		if ((iCurrMode == PRIMARY_MODE_MAIN) && (gSensor_iLastMode != -1)) {
			Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, gSensor_iLastMode);
			gSensor_iLastMode = -1; //clear last mode
		}
	} else
#endif
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_2))
		if (uiSensorLUT[new_device_id] == SENSOR_2) {
			INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
			// exit main mode, return to last mode
			if ((iCurrMode == PRIMARY_MODE_MAIN) && (gSensor_iLastMode != -1)) {
				Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, gSensor_iLastMode);
				gSensor_iLastMode = -1; //clear last mode
			}
		} else
#endif
#if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_1))
			if (uiSensorLUT[new_device_id] == SENSOR_1) {
				// change to current mode again
				Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
			} else
#endif
#if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_2))
				if (uiSensorLUT[new_device_id] == SENSOR_2) {
					// change to current mode again
					Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
				} else
#endif
					//fix for CID 44954 - end
				{
					DBG_ERR("Not support this insert event!!!!!\r\n");
				}

	return NVTEVT_CONSUME;
}

INT32 System_OnSensorRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 old_device_id; //sensor id
	if (paramNum != 1) {
		return NVTEVT_CONSUME;
	}
	old_device_id = (UINT32)(paramArray[0]); //IPL_PATH_1 ~ IPL_PATH_4
	DBG_DUMP("^Y===================================== sensor unplug [SENSOR_%d]\r\n", old_device_id + 1);

	// disconnect sensor IPL_PATH_n
	_System_DisconnectSensor(uiSensorLUT[old_device_id]);

#if 0
	//check is current open device?
	if (System_GetEnableSensor() == 0) {
		//not current device! => only config display for next time.
		return NVTEVT_CONSUME;
	}
#endif

#if(WIFI_FUNC==ENABLE)
//#NT#2016/06/23#Isiah Chang -begin
//#NT#Notify APP to restart RTSP live view becaues of sensor plug/unplug event.
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE), SYS_SUBMODE_WIFI);

		Delay_DelayMs(500); // Delay to ensure preview is ready. Might use other sync. mechanism.

#if _TODO
		WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_SENSOR_NUM_CHANGED);
#endif

		return NVTEVT_CONSUME;
	}
//#NT#2016/06/23#Isiah Chang -end
#endif

	//fix for CID 44955 - begin
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_1))
	if (uiSensorLUT[old_device_id] == SENSOR_1) {
		INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		// exit current mode, enter to main mode
#if (PHOTO_MODE==ENABLE)
		if ((iCurrMode == PRIMARY_MODE_PHOTO) || (iCurrMode == PRIMARY_MODE_MOVIE))
#else
		if (iCurrMode == PRIMARY_MODE_MOVIE)
#endif
		{
			gSensor_iLastMode = iCurrMode; //keep last mode
			Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
		}
	} else
#endif
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_INSERT_MASK == SENSOR_2))
		if (uiSensorLUT[old_device_id] == SENSOR_2) {
			INT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
			// exit current mode, enter to main mode
#if (PHOTO_MODE==ENABLE)
			if ((iCurrMode == PRIMARY_MODE_PHOTO) || (iCurrMode == PRIMARY_MODE_MOVIE))
#else
			if (iCurrMode == PRIMARY_MODE_MOVIE)
#endif
			{
				gSensor_iLastMode = iCurrMode; //keep last mode
				Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
			}
		} else
#endif
#if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_1))
			if (uiSensorLUT[old_device_id] == SENSOR_1) {
				// change to current mode again
				Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
			} else
#endif
#if ((SENSOR_CAPS_COUNT == 2) && (SENSOR_INSERT_MASK == SENSOR_2))
				if (uiSensorLUT[old_device_id] == SENSOR_2) {
					// change to current mode again
					Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
				} else
#endif
					//fix for CID 44955 - end
				{
					DBG_ERR("Not support this remove event!!!!!\r\n");
				}

	return NVTEVT_CONSUME;
}

INT32 System_OnSensorDisplay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 new_display_mask; //sensor id
	if (paramNum != 1) {
		return NVTEVT_CONSUME;
	}

	new_display_mask = (UINT32)(paramArray[0]); //SENSOR_1/2/3/4/ALL
	new_display_mask &= (SENSOR_CAPS_MASK);
	DBG_DUMP("^Y===================================== sensor display [MASK=%08x]->[MASK=%08x]\r\n", uiSensorDispState, new_display_mask);

#if (SENSOR_CAPS_COUNT == 1)
	DBG_ERR("single sensor, not support!\r\n");
	uiSensorDispState = SENSOR_1;
#endif
#if (SENSOR_CAPS_COUNT == 2)

	//check is current open device?
	if (System_GetEnableSensor() == 0) {
		if (uiSensorDispState != new_display_mask) {
			// change sensor display state
			uiSensorDispState = new_display_mask;
		}
		//not current device! => only config display for next time.
		DBG_WRN("dual sensor, but ImagePipe is not enabled, ignore!\r\n");
		return NVTEVT_CONSUME;
	}

	if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
		if (uiSensorDispState != new_display_mask) {
			// change sensor display state
			uiSensorDispState = new_display_mask;
		}
		DBG_WRN("dual sensor, but only connect single now, ignore!\r\n");
		return NVTEVT_CONSUME;
	}
#if _TODO
#if (PIP_VIEW_FASTSWITCH==ENABLE)
	{
		//if recording file => change mode
		//if NOT recording file => NOT change mode ... this is the FAST SWITCH path
		if (MovRec_IsRecording() && (uiSensorDispState != new_display_mask)) {
			// change sensor display state
			uiSensorDispState = new_display_mask;
			// change to current UI mode again
			Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
		}
	}
#else //(PIP_VIEW_FASTSWITCH==DISABLE)
	{
		//always change mode
		if (uiSensorDispState != new_display_mask) {
			// change sensor display state
			uiSensorDispState = new_display_mask;
			// change to current UI mode again
			Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
		}
	}
#endif
#endif

#endif
#if (SENSOR_CAPS_COUNT == 4)
	uiSensorDispState = SENSOR_1;
#endif

	return NVTEVT_CONSUME;
}

INT32 System_OnSensorAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	int iCurrMode = System_GetState(SYS_STATE_NEXTMODE);
	DBG_IND("\r\n");
	if (bFirst) {
		TM_BOOT_BEGIN("sensor", "attach");
	}
	if (bFirst) {
		_System_MapSensor();
	}
	DBG_DUMP("mode:%d\r\n",iCurrMode);

#if (PLAY_MODE==ENABLE)
	if (iCurrMode == PRIMARY_MODE_PLAYBACK) {
		System_EnableSensor(0); //disable all sensors
		System_DispSensor(0); //not display
	} else
#endif
#if (PHOTO_MODE==ENABLE)
		if (iCurrMode == PRIMARY_MODE_PHOTO) {
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
			System_EnableSensor(SENSOR_1); //enable 1 sensors
			System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
			System_EnableSensor(SENSOR_2); //enable 1 sensors
			System_DispSensor(SENSOR_2); //display 1 sensors
#endif
#if (SENSOR_CAPS_COUNT == 2)
			if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
				System_EnableSensor(SENSOR_1); //enable 1 sensors
				System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
				System_EnableSensor(SENSOR_2); //enable 1 sensors
				System_DispSensor(SENSOR_2); //display 1 sensors
#endif
			} else {
				System_EnableSensor((SENSOR_1 | SENSOR_2)); //enable dual sensors
				System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
			}
#endif
#if (SENSOR_CAPS_COUNT == 4)
			{
				System_EnableSensor((SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)); //enable all connected sensors
				//System_EnableSensor(System_GetConnectSensor()); //enable all connected sensors
				System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
			}
#endif
		} else
#endif
#if (IPCAM_MODE==ENABLE)
			if (iCurrMode == PRIMARY_MODE_IPCAM) {
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
				System_EnableSensor(SENSOR_1); //enable 1 sensors
				System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
				System_EnableSensor(SENSOR_2); //enable 1 sensors
				System_DispSensor(SENSOR_2); //display 1 sensors
#endif
#if (DEMO_SENSOR6_CCIR == ENABLE)
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_6))
				System_EnableSensor(SENSOR_6); //enable 1 sensors
				System_DispSensor(SENSOR_6); //display 1 sensors
#endif
#endif
#if (SENSOR_CAPS_COUNT == 2)
				if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
					System_EnableSensor(SENSOR_1); //enable 1 sensors
					System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
					System_EnableSensor(SENSOR_2); //enable 1 sensors
					System_DispSensor(SENSOR_2); //display 1 sensors
#endif
				} else {
					System_EnableSensor((SENSOR_1 | SENSOR_2)); //enable dual sensors
					System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
				}
#endif
#if (SENSOR_CAPS_COUNT == 4)
				{
					System_EnableSensor((SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)); //enable all connected sensors
					//System_EnableSensor(System_GetConnectSensor()); //enable all connected sensors
					System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
				}
#endif
			} else
#endif
#if (MOVIE_MODE==ENABLE)
				if (iCurrMode == PRIMARY_MODE_MOVIE) {
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
					System_EnableSensor(SENSOR_1); //enable 1 sensors
					System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
					System_EnableSensor(SENSOR_2); //enable 1 sensors
					System_DispSensor(SENSOR_2); //display 1 sensors
#endif
#if (SENSOR_CAPS_COUNT == 2)
					if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
						System_EnableSensor(SENSOR_1); //enable 1 sensors
						System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
						System_EnableSensor(SENSOR_2); //enable 1 sensors
						System_DispSensor(SENSOR_2); //display 1 sensors
#endif
					} else {
						System_EnableSensor((SENSOR_1 | SENSOR_2)); //enable dual sensors
						System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
					}
#endif
#if (SENSOR_CAPS_COUNT == 4)
					{
						System_EnableSensor((SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)); //enable all connected sensors
						//System_EnableSensor(System_GetConnectSensor()); //enable all connected sensors
						System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
					}
#endif
#if (CALIBRATION_FUNC == ENABLE)
				} else if ((iCurrMode == PRIMARY_MODE_MAIN) && (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL)) {
					if (EngineerMode_CheckEng()) {
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
						System_EnableSensor(SENSOR_1); //enable 1 sensors
						System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
						System_EnableSensor(SENSOR_2); //enable 1 sensors
						System_DispSensor(SENSOR_2); //display 1 sensors
#endif
#if (SENSOR_CAPS_COUNT == 2)
						if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
							System_EnableSensor(SENSOR_1); //enable 1 sensors
							System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
							System_EnableSensor(SENSOR_2); //enable 1 sensors
							System_DispSensor(SENSOR_2); //display 1 sensors
#endif
						} else {
							System_EnableSensor((SENSOR_1 | SENSOR_2)); //enable dual sensors
							System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
						}
#endif
#if (SENSOR_CAPS_COUNT == 4)
						{
							System_EnableSensor((SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)); //enable all connected sensors
							//System_EnableSensor(System_GetConnectSensor()); //enable all connected sensors
							System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
						}
#endif
					}
#endif // #if (CALIBRATION_FUNC == ENABLE)
				} else
#endif
#if (USB_MODE==ENABLE)
					if (iCurrMode == PRIMARY_MODE_USBPCC) {
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_1))
						System_EnableSensor(SENSOR_1); //enable 1 sensors
						System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if ((SENSOR_CAPS_COUNT == 1) && (SENSOR_CAPS_MASK == SENSOR_2))
						System_EnableSensor(SENSOR_2); //enable 1 sensors
						System_DispSensor(SENSOR_2); //display 1 sensors
#endif
#if (SENSOR_CAPS_COUNT == 2)
						if (System_GetConnectSensor() != (SENSOR_1 | SENSOR_2)) {
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_1)
							System_EnableSensor(SENSOR_1); //enable 1 sensors
							System_DispSensor(SENSOR_1); //display 1 sensors
#endif
#if (SENSOR_DEFAULT_ATTACH_MASK == SENSOR_2)
							System_EnableSensor(SENSOR_2); //enable 1 sensors
							System_DispSensor(SENSOR_2); //display 1 sensors
#endif
						} else {
							System_EnableSensor((SENSOR_1 | SENSOR_2)); //enable dual sensors
							System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
						}
#endif
#if (SENSOR_CAPS_COUNT == 4)
						{
							System_EnableSensor((SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)); //enable all connected sensors
							//System_EnableSensor(System_GetConnectSensor()); //enable all connected sensors
							System_DispSensor(uiSensorDispState); //display selected sensors by SENSOR_DISPLAY
						}
#endif
					} else
#endif
					{
						System_EnableSensor(0); //disable all sensors
						System_DispSensor(0); //not display
					}
	if (bFirst) {
		TM_BOOT_END("sensor", "attach");
	}
	bFirst = FALSE;
	return NVTEVT_CONSUME;
}

INT32 System_OnSensorDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

ER System_ChangeSensorName(SENSOR_ID id, CHAR *repalce_name)
{
	UINT32 phy_id = IPL_PATH(id);

	DBG_IND("id = %d, repalce_name = %s\r\n",id,repalce_name);
	//_System_DetachSensor(phy_id);
	if (drv_sensor_repalce_drv(phy_id,repalce_name) == FALSE)
		return E_PAR;
	drv_sensor_first_cfg();
	//_System_AttachSensor(phy_id);
	return E_OK;
}

