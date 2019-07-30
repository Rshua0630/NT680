////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "DeviceCtrl.h"
#include "NvtIpcAPI.h"
#include "display.h"
#include "displyr.h"

#if (IPCAM_FUNC != ENABLE)
#include "UISetupMapping.h"
#include "UIStorageCheck.h"
#endif
#include "UIAppNetwork.h"
#if(WIFI_FUNC==ENABLE)
#include "WifiCmdParser.h"
#endif
#if (IPCAM_FUNC != ENABLE)
#include "ImageApp_Photo.h"
#include "ImageApp_Movie.h"
#endif
#include "HwClock.h"
#include "MovieStamp.h"
#if(WIFI_FUNC==ENABLE)
static UINT32 bExeWiFiStart = FALSE;
#endif
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UISetupExe
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if(UI_FUNC==ENABLE)


#if(WIFI_FUNC==ENABLE)

// To store Wi-Fi HotSopt SSID and passphrase for station mode.
static char gHotSpot_SSID[NVT_WSC_MAX_SSID_LEN] = {0};
static char gHotSpot_PASSPHRASE[NVT_MAX_WEP_KEY_LEN] = {0};
#endif

///////////////////////////////////////////////////////////////////////////////

UINT32 Get_VolumeValue(UINT32 uhIndex)
{
	//#NT#2013/03/05#Lily Kao -begin
	//#NT# new volume range 0 ~ 100
	//#NT# previous setting should not use "AUDIO_GAIN_x"
#if 1
	switch (uhIndex) {
	case BEEP_VOL_1:
		return 25;
	case BEEP_VOL_2:
		return 50;
	case BEEP_VOL_3:
		return 75;
	case BEEP_VOL_MUTE:
		return 0;
	default:
		return 50;
	}
#else
	switch (uhIndex) {
	case BEEP_VOL_1:
#if BEEP_VOLUME_LOUDER
		return AUDIO_GAIN_4;
#else
		return AUDIO_GAIN_3;
#endif
	case BEEP_VOL_2:
#if BEEP_VOLUME_LOUDER
		return AUDIO_GAIN_6;
#else
		return AUDIO_GAIN_5;
#endif
	case BEEP_VOL_3:
#if BEEP_VOLUME_SMALLER
		return AUDIO_GAIN_6;
#else
		return AUDIO_GAIN_7;
#endif
	case BEEP_VOL_MUTE:
		return AUDIO_GAIN_MUTE;
	default:
		return AUDIO_GAIN_5;
	}
#endif
	//#NT#2013/03/05#Lily Kao -end
}

///////////////////////////////////////////////////////////////////////////
//#NT#2016/06/27#Lincy Lin -begin
//#NT#Reduce IPCAM used memory pool size
#if USE_FILEDB
#include "FileDB.h"
#include "NameRule_FileDB.h"
static FILEDB_INIT_OBJ gSetupFDBInitObj = {
	"A:\\",  //rootPath
	NULL,  //defaultfolder
	NULL,  //filterfolder
	TRUE,  //bIsRecursive
	TRUE,  //bIsCyclic
	TRUE,  //bIsMoveToLastFile
	TRUE, //bIsSupportLongName
	FALSE, //bIsDCFFileOnly
	TRUE,  //bIsFilterOutSameDCFNumFolder
	{'N', 'V', 'T', 'I', 'M'}, //OurDCFDirName[5]
	{'I', 'M', 'A', 'G'}, //OurDCFFileName[4]
	FALSE,  //bIsFilterOutSameDCFNumFile
	TRUE, //bIsChkHasFile
	60,    //u32MaxFilePathLen
	10000,  //u32MaxFileNum
	(FILEDB_FMT_JPG | FILEDB_FMT_AVI | FILEDB_FMT_MOV | FILEDB_FMT_MP4 | FILEDB_FMT_TS), //fileFilter
	0,     //u32MemAddr
	0,     //u32MemSize
	NULL   //fpChkAbort

};

#endif
//#NT#2016/06/27#Lincy Lin -end
BOOL SetupExe_IsNoFile(void)
{
#if (USE_FILEDB == ENABLE)
	PFILEDB_INIT_OBJ   pFDBInitObj = &gSetupFDBInitObj;
	FILEDB_HANDLE      FileDBHandle = 0;
	UINT32 fileCount = 0;

	pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);

	if (!pFDBInitObj->u32MemAddr) {
		return TRUE;
	}

	pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
	FileDBHandle = FileDB_Create(pFDBInitObj);
	fileCount = FileDB_GetTotalFileNum(FileDBHandle);
	//FileDB_DumpInfo(FileDBHandle);

	FileDB_Release(FileDBHandle);
    SxCmd_RelTempMem(pFDBInitObj->u32MemAddr);

	if (!fileCount) {
		return TRUE;
	} else {
		return FALSE;
	}
#else
	return FALSE;
#endif
}


INT32 SetupExe_OnFILEDB_FILEID_RESET(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (USE_FILEDB == ENABLE)
	PFILEDB_FILE_ATTR  pFileAttr = NULL;
	int curfilenameLen;
	CHAR FileIndex[8] = {0};
	INT32 uiTotalFileNum, i;

	FileDB_Refresh(0);
	uiTotalFileNum = FileDB_GetTotalFileNum(0);
	if (uiTotalFileNum) {
		// search Movie file name
		for (i = uiTotalFileNum - 1; i >= 0; i--) {
			pFileAttr = FileDB_SearhFile(0, i);
			if (pFileAttr == NULL) {
				DBG_ERR("pFileAttr is NULL, i=%d\r\n", i);
				NH_FileDB_SetFileID(0);
				NH_Custom_SetFileID(0);
				return NVTEVT_CONSUME;
			}
			//DBG_DUMP("file name:%s\r\n",pFileAttr->filename);
			if ((pFileAttr->fileType == FILEDB_FMT_MP4) || (pFileAttr->fileType == FILEDB_FMT_MOV) ||
				(pFileAttr->fileType == FILEDB_FMT_TS)  || (pFileAttr->fileType == FILEDB_FMT_JPG)) {
				break;
			}
		}
		//pFileAttr = FileDB_CurrFile(0);
		curfilenameLen = strlen(pFileAttr->filename);
		strncpy(FileIndex, pFileAttr->filename + curfilenameLen - 7, 3);
		DBG_IND("fileid:%d\r\n", atoi(FileIndex));
		NH_FileDB_SetFileID(atoi(FileIndex));
		NH_Custom_SetFileID(atoi(FileIndex));
	} else {
		DBG_IND("fileid:%d\r\n", 0);
		NH_FileDB_SetFileID(0);
		NH_Custom_SetFileID(0);
	}
#endif
	return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////////////////
#if(UI_FUNC==ENABLE)
#include "UIBackgroundObj.h"
#endif
INT32 SetupExe_OnFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 1
	DBG_IND("Do Format storage\r\n");

#if (PHOTO_MODE==ENABLE)
	UIStorageCheck_ResetFileReachMax();
#endif
#if(UI_FUNC==ENABLE)
	BKG_PostEvent(NVTEVT_BKW_FORMAT);
#endif
#else
	Ux_PostEvent(NVTEVT_BACKGROUND_DONE, 1, NVTEVT_BKW_FORMAT);

#endif
	return NVTEVT_CONSUME;
}

///////////////////////////////////////////////////////////////////

INT32 SetupExe_OnBeepVolume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_BeepVolumnIndex, uhSelect);
	//#Set volumn
	GxSound_SetVolume(Get_VolumeValue(uhSelect));

	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnBeepKey(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_BEEP, uhSelect);

	return NVTEVT_CONSUME;
}

///////////////////////////////////////////////////////////////////
#if(UI_FUNC==ENABLE)
#include "UIResource.h"
#include "UIGraphics.h"
#endif
///////////////////////////////////////////////////////////////////
INT32 SetupExe_OnLanguage(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_LANGUAGE, uhSelect);
#if(UI_FUNC==ENABLE)
	GxGfx_SetStringTable((const STRING_TABLE *)Get_LanguageValue(uhSelect));
#endif

	return NVTEVT_CONSUME;
}

///////////////////////////////////////////////////////////////////
#if (PHOTO_MODE==ENABLE)
extern void PhotoExe_AE_SetUIInfo(UINT32 index, UINT32 value);
#endif
extern void MovieExe_AE_SetUIInfo(UINT32 index, UINT32 value);

INT32 SetupExe_OnFreq(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);
	UI_SetData(FL_FREQUENCY, uhSelect);
#if (PHOTO_MODE==ENABLE)
	PhotoExe_AE_SetUIInfo(AE_UI_FREQUENCY, uhSelect);
#endif
	MovieExe_AE_SetUIInfo(AE_UI_FREQUENCY, uhSelect);

	return NVTEVT_CONSUME;
}
///////////////////////////////////////////////////////////////////

INT32 SetupExe_OnChangeDSCMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiCurMode, uiChangeTo = 0;
	UINT32 uiType = DSCMODE_CHGTO_NEXT;

	if (paramNum > 0) {
		uiType = paramArray[0];
	}

	DBG_IND("uiType %d \r\n", uiType);

	uiCurMode = UI_GetData(FL_NextMode);

	switch (uiType) {
	case DSCMODE_CHGTO_NEXT:
#if (PHOTO_MODE==ENABLE)
		if (PRIMARY_MODE_PHOTO == (INT32)uiCurMode) {
			uiChangeTo = PRIMARY_MODE_PLAYBACK;
		} else
#endif
			if (PRIMARY_MODE_MOVIE == (INT32)uiCurMode) {
#if (PHOTO_MODE==ENABLE)
				uiChangeTo = PRIMARY_MODE_PHOTO;
#endif
			} else
#if (PLAY_MODE==ENABLE)
				if (PRIMARY_MODE_PLAYBACK == (INT32)uiCurMode) {
					uiChangeTo = PRIMARY_MODE_MOVIE;
				} else
#endif
				{
					DBG_ERR("UISystemObj: Unknown current DSC mode 0x%x\r\n", uiCurMode);
				}
		break;
	case DSCMODE_CHGTO_PREV:
		uiChangeTo = UI_GetData(FL_PreMode);
		break;
	case DSCMODE_CHGTO_CURR:
		uiChangeTo = uiCurMode;
		break;
	}

	if (1) { //(KeyScan_IsHDMIPlugIn()==FALSE)
		UI_SetData(FL_PreMode, uiCurMode);
		UI_SetData(FL_NextMode, uiChangeTo);
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, uiChangeTo);
	}


	return NVTEVT_CONSUME;
}

///////////////////////////////////////////////////////////////////
INT32 SetupExe_OnForceToLivedMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiCurrMode;
#if(UI_FUNC==ENABLE)
	VControl *pCloseWnd = NULL;
#endif

	uiCurrMode = UI_GetData(FL_NextMode);

#if (PHOTO_MODE==ENABLE)
	if ((INT32)uiCurrMode == PRIMARY_MODE_PHOTO) {
		/* Close all the window upper than root */
		Ux_GetWindowByIndex(&pCloseWnd, UX_2NDLAYER_WND_IDX);
		if (pCloseWnd) {
			Ux_CloseWindow(pCloseWnd, 0);
		}
	} else
#endif
#if (MOVIE_MODE==ENABLE)
		if ((INT32)uiCurrMode == PRIMARY_MODE_MOVIE) {
#if(UI_FUNC==ENABLE)
			/* Close all the window upper than root */
			Ux_GetWindowByIndex(&pCloseWnd, UX_2NDLAYER_WND_IDX);
			if (pCloseWnd) {
				Ux_CloseWindow(pCloseWnd, 0);
			}
#endif
		} else
#endif
#if (PLAY_MODE==ENABLE)
			if ((INT32)uiCurrMode == PRIMARY_MODE_PLAYBACK) {
				/* Switch to the previous liveview mode before playback mode */
				/* switch to the previous mode only unpluging HDMI */
				if (1) { //(KeyScan_IsHDMIPlugIn()==FALSE)
					UI_SetData(FL_PreMode, PRIMARY_MODE_PLAYBACK);
					UI_SetData(FL_NextMode, PRIMARY_MODE_MOVIE);
					Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
				}
			} else
#endif
			{
				DBG_ERR("UISetupObj: Unknown current DSC mode 0x%x\r\n", uiCurrMode);
			}
	return NVTEVT_CONSUME;
}
///////////////////////////////////////////////////////////////////
INT32 SetupExe_OnForceToPlaybackMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (PLAY_MODE==ENABLE)
	UINT32 uiCurrMode;

	uiCurrMode = UI_GetData(FL_NextMode);

	if ((INT32)uiCurrMode != PRIMARY_MODE_PLAYBACK) {
		UI_SetData(FL_PreMode, uiCurrMode);
		UI_SetData(FL_NextMode, PRIMARY_MODE_PLAYBACK);
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
	}
#endif
	return NVTEVT_CONSUME;
}
///////////////////////////////////////////////////////////////////
INT32 SetupExe_OnLCDOff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_LCD_OFF, uhSelect);

	switch (uhSelect) {
	case LCDOFF_ON:
		GxPower_SetControl(GXPWR_CTRL_SLEEP_EN, FALSE);
		break;
#if (AUTOSLEEP_FUNCTION == ENABLE)
	case LCDOFF_1MIN:
		GxPower_SetControl(GXPWR_CTRL_SLEEP_EN, TRUE);
		//GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 10);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 60);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME, 60);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_D, 60);
		break;
	case LCDOFF_3MIN:
		GxPower_SetControl(GXPWR_CTRL_SLEEP_EN, TRUE);
		//GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 10);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 180);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME, 180);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_D, 180);
		break;
	case LCDOFF_5MIN:
		GxPower_SetControl(GXPWR_CTRL_SLEEP_EN, TRUE);
		//GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 10);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 300);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME, 300);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_D, 300);
		break;
	case LCDOFF_10MIN:
		GxPower_SetControl(GXPWR_CTRL_SLEEP_EN, TRUE);
		//GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 10);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_L, 600);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME, 600);
		GxPower_SetControl(GXPWR_CTRL_SLEEP_TIME_D, 600);
		break;

#endif
	default:
		DBG_ERR("Error parameter (%d) with SetupExe_OnLCDOff\r\n", uhSelect);
		break;
	}


	return NVTEVT_CONSUME;
}
///////////////////////////////////////////////////////////////////
#include "GxPower.h"
INT32 SetupExe_OnPowerOff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_AUTO_POWER_OFF, uhSelect);

	switch (uhSelect) {
	case POWER_ON:
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_EN, FALSE);
		break;
#if (AUTOPWROFF_FUNCTION == ENABLE)
	case POWER_1MIN:
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_EN, TRUE);
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_TIME, 60);
		break;
	case POWER_2MIN:
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_EN, TRUE);
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_TIME, 120);
		break;
	case POWER_3MIN:
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_EN, TRUE);
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_TIME, 180);
		break;
	case POWER_5MIN:
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_EN, TRUE);
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_TIME, 300);
		break;
	case POWER_10MIN:
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_EN, TRUE);
		GxPower_SetControl(GXPWR_CTRL_AUTOPOWEROFF_TIME, 600);
		break;
#endif
	default:
		DBG_ERR("Error parameter (%d) with SetupExe_OnPowerOff\r\n", uhSelect);
		break;
	}

	return NVTEVT_CONSUME;
}

///////////////////////////////////////////////////////////////////

INT32 SetupExe_OnSysReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("Do SysReset\r\n");
#if(UI_FUNC==ENABLE)
#if (PST_FUNC == ENABLE)
	AppBKW_SetData(BKW_WAIT_TIME, 1000);//show wait window for 2 second
	BKG_PostEvent(NVTEVT_BKW_DUMMY_WAIT);
	//#NT#For menu reset flag,for FlowPhoto_InitSettingParams
	UI_SetData(FL_ResetFlag, TRUE);


	//#NT#2010/09/10#Lincy Lin -begin
	//#NT#For special key reset function
#if SPECIAL_KEY_RESET_FUNC
	if ((GxKey_GetData(GXKEY_NORMAL_KEY) & FLGKEY_ZOOMIN)) {
		UI_SetData(FL_TakePictCnt, 0);
		UI_SetData(FL_ModeIndex, 0);
		//System_PowerOff(SYS_POWEROFF_NORMAL);
	}
#endif
	//#NT#2010/09/10#Lincy Lin -end

	Reset_MenuInfo();
	//save to nand
	Save_MenuInfo();
#else
	Ux_PostEvent(NVTEVT_BACKGROUND_DONE, 1, NVTEVT_BKW_DUMMY_WAIT);
#endif
#endif
	return NVTEVT_CONSUME;
}
INT32 SetupExe_OnSysResetNoWin(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("Do SysReset No Win\r\n");
	Reset_MenuInfo();
	Save_MenuInfo();
    exam_msg("SysReset ret=%d\r\n", 0);

	return NVTEVT_CONSUME;
}

///////////////////////////////////////////////////////////////////

INT32 SetupExe_OnNumReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if(UI_FUNC==ENABLE)
	BKG_PostEvent(NVTEVT_BKW_NUM_RESET);
#endif

	return NVTEVT_CONSUME;
}


///////////////////////////////////////////////////////////////////
#include "GxDisplay.h"
INT32 SetupExe_OnBrightAdj(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_BrightAdjIndex, uhSelect);

	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnBrightLvl(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BRIGHTLVL, uhSelect);

	UI_SetData(FL_LCD_BRIGHTNESS, uhSelect);

	return NVTEVT_CONSUME;
}


///////////////////////////////////////////////////////////////////
INT32 SetupExe_OnUSBMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_USB_MODE, uhSelect);

	return NVTEVT_CONSUME;
}



///////////////////////////////////////////////////////////////////

#if (TV_FUNC == ENABLE)
extern UINT32 gDevTVObj;

INT32 SetupExe_OnTVFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_TV_MODE, uhSelect);

	switch (uhSelect) {
	case VIDEOOUT_NTSC:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_NTSC); //change to NTSC mode
		break;
	case VIDEOOUT_PAL:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_PAL); //change to PAL mode
		break;
	default:
		DBG_ERR("Error parameter (%d) with SetupExe_OnTVFormat\r\n", uhSelect);
		break;
	}

	return NVTEVT_CONSUME;
}
#endif

#if (HDMI_FUNC == ENABLE)
extern UINT32 gDevHDMIObj;

INT32 SetupExe_OnHDMIFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_HDMI_MODE, uhSelect);

	switch (uhSelect) {
	case VIDEOOUT2_AUTO:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_MAX); //change to max resoluition
		break;
	case VIDEOOUT2_4K2KP30:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P30); //change to 4K2K mode
		break;
	case VIDEOOUT2_4K2KP25:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P25); //change to 4K2K mode
		break;
	case VIDEOOUT2_4K2KP24:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P24); //change to 4K2K mode
		break;
	case VIDEOOUT2_UHDP30:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P30); //change to UHD mode
		break;
	case VIDEOOUT2_UHDP25:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P25); //change to UHD mode
		break;
	case VIDEOOUT2_UHDP24:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P24); //change to UHD mode
		break;
	case VIDEOOUT2_FHDI60:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_1920X1080I60); //change to FHD mode
		break;
	case VIDEOOUT2_FHDP30:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_1920X1080P30); //change to FHD mode
		break;
	case VIDEOOUT2_HD:
		Ux_PostEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_720X480P60); //change to HD mode
		break;
	default:
		DBG_ERR("Error parameter (%d) with SetupExe_OnHDMIFormat\r\n", uhSelect);
		break;
	}

	return NVTEVT_CONSUME;
}
#endif

INT32 SetupExe_OnDualDisp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_DualDisp, uhSelect);

	return NVTEVT_CONSUME;
}


#if(WIFI_FUNC==ENABLE)
INT32 SetupExe_OnWifiMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	UIMenuStoreInfo *ptMenuStoreInfo = UI_GetMenuInfo();

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_DUMP("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_NetWorkMode, uhSelect);

	if (UI_GetData(FL_NetWorkMode) == NET_AP_MODE || UI_GetData(FL_NetWorkMode) == NET_WPS_AP_PBC_MODE) {
		if (ptMenuStoreInfo->strSSID[0]) {
			UINet_SetSSID(ptMenuStoreInfo->strSSID, strlen(ptMenuStoreInfo->strSSID));           // SSID of AP
		} else {
			UINet_SetSSID(UINet_GetDefaultAPSSID(), strlen(UINet_GetDefaultAPSSID())); // SSID of AP
		}

		if (ptMenuStoreInfo->strPASSPHRASE[0]) {
			UINet_SetPASSPHRASE(ptMenuStoreInfo->strPASSPHRASE, strlen(ptMenuStoreInfo->strPASSPHRASE));            // passwd of AP
		} else {
			UINet_SetPASSPHRASE(UINet_GetDefaultAPPASSPHRASE(), strlen(UINet_GetDefaultAPPASSPHRASE()));  // passwd of AP
		}
	}

	return NVTEVT_CONSUME;
}

#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
INT32 SetupExe_OnWifiStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 result = 0;

	if (bExeWiFiStart) {
		DBG_WRN("is start\r\n");
		return NVTEVT_CONSUME;
	}
#if (defined(_CPU2_ECOS_) && (POWERON_FAST_CPU2_BOOT != ENABLE))
	//UINet_CPU2_Init();
	System_CPU2_Start(); //try to download FW of CPU2, and start CPU2
	System_CPU2_WaitReady();
#endif
	UINet_AppIpc_Init();

//#if defined(_CPU2_ECOS_) || (defined(_CPU2_LINUX_) && WIFI_AP_FUNC==ENABLE)
//#if !defined(_NVT_SDIO_WIFI_NONE_)
	DBG_DUMP("Network mod:%d \r\n", UI_GetData(FL_NetWorkMode));

	//#NT#2016/07/14#Charlie Chang -begin
	//#NT# support 4g module
#if (NETWORK_4G_MODULE == ENABLE)
	UI_SetData(FL_NetWorkMode, NET_STATION_MODE);
	UINet_SetAuthType(NET_AUTH_WPA2);
	UINet_IotP2PInit();
	UINet_HFSInit();
#if ONVIF_FUNC
	UINet_OnvifInit();
#endif
	UI_SetData(FL_WIFI_LINK, WIFI_LINK_OK);
	SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID, FALSE);

	bExeWiFiStart = TRUE;
	return NVTEVT_CONSUME;
#endif
	//#NT#2016/07/14#Charlie Chang -end
	if (UI_GetData(FL_NetWorkMode) == NET_AP_MODE || UI_GetData(FL_NetWorkMode) == NET_WPS_AP_PBC_MODE) {
		//#NT#2016/07/05#Charlie Chang -begin
		//#NT# reset ssid name if IOT_P2P_FUNC enable
#if (IOT_P2P_FUNC == ENABLE)
		char ssid_buf[NVT_WSC_MAX_SSID_LEN] = {0};
		sprintf(ssid_buf, "BRONCI_AP_10001");
		UINet_SetSSID(ssid_buf, strlen(ssid_buf));
#endif
		//#NT#2016/06/20#Charlie Chang -end
		UINet_SetAuthType(NET_AUTH_WPA2);
	} else if (UI_GetData(FL_NetWorkMode) == NET_STATION_MODE) { // Station Mode.
#if 0
		UINet_SetSSID("MyAP", strlen("MyAP"));       // SSID of AP
		UINet_SetAuthType(NET_AUTH_WPA2);
		UINet_SetPASSPHRASE("123456780", strlen("12345678")); // Password of AP
		UI_SetData(FL_NetWorkMode, NETWORK_STATION_MODE);
#else
		if (gHotSpot_SSID[0] && gHotSpot_PASSPHRASE[0]) {
			UINet_SetSSID(gHotSpot_SSID, strlen(gHotSpot_SSID));        // SSID of AP

			UINet_SetAuthType(NET_AUTH_WPA2);

			UINet_SetPASSPHRASE(gHotSpot_PASSPHRASE, strlen(gHotSpot_PASSPHRASE)); // Password of AP
		} else {
			DBG_ERR("SSID or passphrase error!\r\n");
		}
#endif
	} else {
		DBG_ERR("Neither Wi-Fi AP nor Station mode!\r\n");
	}
//#elif defined(_NETWORK_SPI_EVB_ETHERNET_)
//    UI_SetData(FL_NetWorkMode,NET_AP_MODE);
//#endif
	UINet_SetAuthType(NET_AUTH_WPA2);
	result = UINet_WifiInit(UI_GetData(FL_NetWorkMode), UINet_GetAuthType());
//#else
//    UINet_HFSInit();
//#endif
#if ONVIF_FUNC
	UINet_OnvifInit();
#endif
	if (result == 0) {
		UI_SetData(FL_WIFI_LINK, WIFI_LINK_OK);
		SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID, FALSE);
#if !defined(_NVT_SDIO_WIFI_NONE_)
		if (UINet_GetMode() == NET_STATION_MODE) {
			UINet_CliReConnect(1);
		}
#endif
	} else {
		UI_SetData(FL_WIFI_LINK, WIFI_LINK_NG);
	}

	//DBG_DUMP("BackgroundWiFiOn %d result %d\r\n", UI_GetData(FL_NetWorkMode), result);
	bExeWiFiStart = TRUE;
	return NVTEVT_CONSUME;
}

void System_WifiStart(void)
{
	SetupExe_OnWifiStart(0, 0, 0);
}

INT32 SetupExe_OnWifiStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (!bExeWiFiStart) {
		DBG_WRN("not start\r\n");
		return NVTEVT_CONSUME;
	}
	UINet_WifiUnInit(UI_GetData(FL_NetWorkMode));
#if ONVIF_FUNC
	UINet_OnvifUnInit();
#endif
	SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID, TRUE);
	UINet_AppIpc_UnInit();
	UI_SetData(FL_COMMON_LOCAL, WIFI_OFF);

#if (defined(_CPU2_ECOS_) && (POWERON_FAST_CPU2_BOOT != ENABLE))
	System_CPU2_Stop();
#endif
	bExeWiFiStart = FALSE;

	UI_SetData(FL_WIFI_LINK, WIFI_LINK_NG);
	if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON) {
          MovieStamp_Disable();
	}
	return NVTEVT_CONSUME;
}
#endif

INT32 SetupExe_OnWifiSetSSID(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	char *Data = 0;
	UIMenuStoreInfo *ptMenuStoreInfo = UI_GetMenuInfo();

	if (paramNum) {
		Data = (char *)paramArray[0];
		DBG_IND(" %s %d\r\n", Data, strlen(Data));
		UINet_SetSSID(Data, strlen(Data));
		if (strlen(Data) <= NVT_WSC_MAX_SSID_LEN - 1) {
			strncpy(ptMenuStoreInfo->strSSID, UINet_GetSSID(), (NVT_WSC_MAX_SSID_LEN - 1)); // Save Wi-Fi SSID.
			ptMenuStoreInfo->strSSID[strlen(ptMenuStoreInfo->strSSID)] = '\0';
		}
	}
	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnWifiSetPassPhrase(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	char *Data = 0;
	UIMenuStoreInfo *ptMenuStoreInfo = UI_GetMenuInfo();

	if (paramNum) {
		Data = (char *)paramArray[0];
		DBG_IND(" %s %d\r\n", Data, strlen(Data));
		UINet_SetPASSPHRASE(Data, strlen(Data));
		if (strlen(Data) <= NVT_MAX_WEP_KEY_LEN - 1) {
			strncpy(ptMenuStoreInfo->strPASSPHRASE, UINet_GetPASSPHRASE(), (NVT_MAX_WEP_KEY_LEN - 1)); // Save Wi-Fi PASSWORD.
			ptMenuStoreInfo->strPASSPHRASE[strlen(ptMenuStoreInfo->strPASSPHRASE)] = '\0';
		}
	}
	return NVTEVT_CONSUME;
}
INT32 SetupExe_OnWifiSetAuthType(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum) {
		DBG_IND("uhSelect %d \r\n", paramArray[0]);
		UINet_SetAuthType(paramArray[0]);
	}
	return NVTEVT_CONSUME;
}
INT32 SetupExe_OnDHCPStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum) {
		DBG_IND("uhSelect %d \r\n", paramArray[0]);
		UINet_DHCP_Start(paramArray[0]);
	}
	return NVTEVT_CONSUME;
}
INT32 SetupExe_OnWifiReInit(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINet_WifiP2PReInit();
	return NVTEVT_CONSUME;
}
INT32 SetupExe_OnWifiBack2Dev(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINet_WifiBack2Dev();
	return NVTEVT_CONSUME;
}


INT32 SetupExe_OnWifiSendSSIDPassPhrase(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	char *Data = 0;
	UIMenuStoreInfo *ptMenuStoreInfo = UI_GetMenuInfo();

	DBG_IND("%s %s\r\n", __FUNCTION__, paramArray[0]);
	if (paramNum) {
		Data = (char *)paramArray[0];
		DBG_DUMP("%d\r\n", strlen(Data));

		if (2 == sscanf_s(Data, "%[^:]:%s", gHotSpot_SSID, gHotSpot_PASSPHRASE, sizeof(gHotSpot_SSID) + sizeof(gHotSpot_PASSPHRASE))) {
			WifiCmd_UnlockString(Data);//setDate cost time,unlock string before rtc_setDate
			DBG_DUMP("SSID:%s, Passphrase:%s\r\n", gHotSpot_SSID, gHotSpot_PASSPHRASE);

			if (strlen(gHotSpot_SSID) <= NVT_WSC_MAX_SSID_LEN - 1) {
				strncpy(ptMenuStoreInfo->strSSID_hotspot_1, gHotSpot_SSID, (NVT_WSC_MAX_SSID_LEN - 1)); // Save Wi-Fi SSID for station mode.
				ptMenuStoreInfo->strSSID_hotspot_1[strlen(ptMenuStoreInfo->strSSID_hotspot_1)] = '\0';
			}

			if (strlen(gHotSpot_PASSPHRASE) <= NVT_MAX_WEP_KEY_LEN - 1) {
				strncpy(ptMenuStoreInfo->strPASSPHRASE_hotspot_1, gHotSpot_PASSPHRASE, (NVT_MAX_WEP_KEY_LEN - 1)); // Save Wi-Fi passwd for station mode.
				ptMenuStoreInfo->strPASSPHRASE_hotspot_1[strlen(ptMenuStoreInfo->strPASSPHRASE_hotspot_1)] = '\0';
			}
		} else {
			WifiCmd_UnlockString(Data);
		}
	}

	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnWifiReqQRDecode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	INT32            ret;
	NVTIPC_SYS_MSG   sysMsg;
    DISPLAYER_PARAM  lyr_param;
	DISP_OBJ         *p_dispobj = disp_getDisplayObject(DISP_1);
	char             cmd[64] = "nvtqrcode_test /mnt/sd/ide1v1b0.bin 960 180 &";

    SxCmd_DoCommand("gxdisp dumpbuf 0 2");

    p_dispobj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_GET_BUFSIZE, &lyr_param);

    snprintf((char *)cmd, sizeof(cmd) - 1, "nvtqrcode_test /mnt/sd/ide1v1b0.bin %d %d &",
					 lyr_param.SEL.GET_BUFSIZE.uiBufLineOfs, lyr_param.SEL.GET_BUFSIZE.uiBufHeight);

    DBG_IND("cmd:%s\r\n", cmd);

    Delay_DelayMs(100);

	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)&cmd[0];
	sysMsg.DataSize = strlen(cmd) + 1;
	if ((ret = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg))) < 0) {
		DBG_ERR("NvtIPC_MsgSnd %d\r\n", ret);
		//return -1;
	}

	return NVTEVT_CONSUME;
}
#endif

///////////////////////////////////////////////////////////////////
INT32 SetupExe_OnDateFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_DateFormatIndex, uhSelect);

	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnTimeFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("uhSelect %d \r\n", uhSelect);

	UI_SetData(FL_TimeFormatIndex, uhSelect);

	return NVTEVT_CONSUME;
}
INT32 SetupExe_OnDate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	if (paramNum == 3) {

        struct tm Curr_DateTime;

		DBG_IND("%d %d %d\r\n", paramArray[0], paramArray[1], paramArray[2]);

        Curr_DateTime = HwClock_GetTime(TIME_ID_CURRENT);
        Curr_DateTime.tm_year = paramArray[0];
        Curr_DateTime.tm_mon = paramArray[1];
        Curr_DateTime.tm_mday = paramArray[2];

        HwClock_SetTime(TIME_ID_CURRENT, Curr_DateTime, 0);

	}
	return NVTEVT_CONSUME;

}
INT32 SetupExe_OnTime(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum == 3) {

        struct tm Curr_DateTime;

		DBG_IND("%d %d %d\r\n", paramArray[0], paramArray[1], paramArray[2]);

        Curr_DateTime = HwClock_GetTime(TIME_ID_CURRENT);
        Curr_DateTime.tm_hour = paramArray[0];
        Curr_DateTime.tm_min  = paramArray[1];
        Curr_DateTime.tm_sec  = paramArray[2];

        HwClock_SetTime(TIME_ID_CURRENT, Curr_DateTime, 0);

	}
	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnFWUpdate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("Do FW Update\r\n");

#if (IPCAM_FUNC != ENABLE)
#if 0
#if(UI_FUNC==ENABLE)
	BKG_PostEvent(NVTEVT_BKW_FW_UPDATE);
#endif
#else // use UI task update for uctrl
	System_OnStrg_UploadFW(0);
#endif
#else
	debug_msg("_TODO -> %s:%d, task(%x,%s) caller(%pF)\r\n", __FUNCTION__, __LINE__,
			  OS_GetTaskID(), OS_GetTaskName(), __builtin_return_address(0));

#endif

	return NVTEVT_CONSUME;
}
///////////////////////////////////////////////////////////////////
INT32 SetupExe_OnPreviewMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (PLAY_MODE==ENABLE)
	int curMode = System_GetState(SYS_STATE_CURRMODE);
	DBG_IND("Cur:%d, Previous:%d\r\n", curMode, UI_GetData(FL_PreMode));
#if 0
	UI_Switch_DscMode(UI_GetData(FL_PreMode), DSCMODE_SWITCH_FORCE, DSCMODE_SWITCHDIR_DONT_CARE);
#else
	if (curMode == PRIMARY_MODE_PLAYBACK) {
		UI_Switch_DscMode(UI_GetData(FL_PreMode), DSCMODE_SWITCH_FORCE, DSCMODE_SWITCHDIR_DONT_CARE);
	} else
#endif
	{
		VControl *previewWnd;
		Ux_GetWindowByIndex(&previewWnd, 1);
		//close wind untill root window
		Ux_CloseWindow(previewWnd, 0);
	}
#endif
	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnCopy2Card(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("Do copy to card\r\n");

#if(UI_FUNC==ENABLE)
	BKG_PostEvent(NVTEVT_BKW_COPY2CARD);
#endif

	return NVTEVT_CONSUME;
}

INT32 SetupExe_OnPbRetractLens(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (IPCAM_FUNC != ENABLE)
	DevCtrl_Lens_Playback_Retract();
#endif
	return NVTEVT_CONSUME;
}

#endif

EVENT_ENTRY UISetupObjCmdMap[] = {
//Mode
#if(UI_FUNC==ENABLE)
	{NVTEVT_EXE_CHANGEDSCMODE,       SetupExe_OnChangeDSCMode     },
	{NVTEVT_FORCETO_LIVEVIEW_MODE,   SetupExe_OnForceToLivedMode  },
	{NVTEVT_FORCETO_PLAYBACK_MODE,   SetupExe_OnForceToPlaybackMode},
	{NVTEVT_FILEID_RESET,            SetupExe_OnFILEDB_FILEID_RESET},
	{NVTEVT_PREVIEWMODE,             SetupExe_OnPreviewMode       },

//Status
	{NVTEVT_EXE_FWUPDATE,            SetupExe_OnFWUpdate          },
	{NVTEVT_EXE_SYSRESET,            SetupExe_OnSysReset          },
	{NVTEVT_EXE_SYSRESET_NO_WIN,     SetupExe_OnSysResetNoWin     },
//Time
	{NVTEVT_EXE_DATEFORMAT,          SetupExe_OnDateFormat        },
	{NVTEVT_EXE_TIMEFORMAT,          SetupExe_OnTimeFormat        },
	{NVTEVT_EXE_DATE,                SetupExe_OnDate              },
	{NVTEVT_EXE_TIME,                SetupExe_OnTime              },
//Power
	{NVTEVT_EXE_LCDOFF,              SetupExe_OnLCDOff            },
	{NVTEVT_EXE_POWEROFF,            SetupExe_OnPowerOff          },
//Sound
	{NVTEVT_EXE_BEEPVOLUME,          SetupExe_OnBeepVolume        },
//Display
#if (TV_FUNC == ENABLE)
	{NVTEVT_EXE_TVFORMAT,            SetupExe_OnTVFormat          },
#endif
#if (HDMI_FUNC == ENABLE)
	{NVTEVT_EXE_HDMIFORMAT,          SetupExe_OnHDMIFormat        },
#endif
	{NVTEVT_EXE_DUALDISP,            SetupExe_OnDualDisp          },
#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
#if(WIFI_AP_FUNC==ENABLE)
	{NVTEVT_EXE_WIFI_MODE,           SetupExe_OnWifiMode          },
	{NVTEVT_EXE_WIFI_START,          SetupExe_OnWifiStart         },
	{NVTEVT_EXE_WIFI_STOP,           SetupExe_OnWifiStop          },
	{NVTEVT_EXE_WIFI_SET_SSID,       SetupExe_OnWifiSetSSID       },
	{NVTEVT_EXE_WIFI_SET_PASSPHRASE, SetupExe_OnWifiSetPassPhrase },
	{NVTEVT_EXE_WIFI_SET_AUTHTYPE,   SetupExe_OnWifiSetAuthType   },
	{NVTEVT_EXE_WIFI_DHCP_START,     SetupExe_OnDHCPStart         },
	{NVTEVT_EXE_WIFI_REINIT,         SetupExe_OnWifiReInit        },
	{NVTEVT_EXE_WIFI_BACK2DEV,       SetupExe_OnWifiBack2Dev      },
	{NVTEVT_EXE_WIFI_SEND_SSID_PASSPHRASE,  SetupExe_OnWifiSendSSIDPassPhrase   },
    {NVTEVT_EXE_WIFI_REQ_QR_DECODE,  SetupExe_OnWifiReqQRDecode},
// for IPCam
#else
	{NVTEVT_EXE_WIFI_START,          SetupExe_OnWifiStart         },
	{NVTEVT_EXE_WIFI_STOP,           SetupExe_OnWifiStop          },
#endif
#endif
	{NVTEVT_EXE_BRIGHTADJ,           SetupExe_OnBrightAdj         },
	{NVTEVT_EXE_BRIGHTLVL,           SetupExe_OnBrightLvl         },
//FileSys
	{NVTEVT_EXE_FORMAT,              SetupExe_OnFormat            },
	{NVTEVT_EXE_SETUPCOPY2CARD,      SetupExe_OnCopy2Card         },
//USB
	{NVTEVT_EXE_USBMODE,             SetupExe_OnUSBMode           },
//Lens
	{NVTEVT_EXE_PB_RETRACT_LENS,     SetupExe_OnPbRetractLens     },
//DCF manage
	{NVTEVT_EXE_NUMRESET,            SetupExe_OnNumReset          },
	{NVTEVT_EXE_BEEPKEY,             SetupExe_OnBeepKey           },
	{NVTEVT_EXE_LANGUAGE,            SetupExe_OnLanguage          },
//Photo
	{NVTEVT_EXE_FREQ,                SetupExe_OnFreq              },
#if (PHOTO_MODE==ENABLE)
	{NVTEVT_EXE_AFWINDOW,            PhotoExe_OnAFWindow          },
	{NVTEVT_EXE_AFBEAM,              PhotoExe_OnAFBeam            },
	{NVTEVT_EXE_CONTAF,              PhotoExe_OnContAF            },
	{NVTEVT_EXE_METERING,            PhotoExe_OnMetering          },
	{NVTEVT_EXE_PREVIEW,             PhotoExe_OnPreview           },
	{NVTEVT_EXE_DIGITAL_ZOOM,        PhotoExe_OnDigitalZoom       },
	{NVTEVT_EXE_DATE_PRINT,          PhotoExe_OnDatePrint         },
#endif

//Movie
#if(MOVIE_MODE==ENABLE)
	{NVTEVT_EXE_MOVIEDIS,            MovieExe_OnDis               },
	{NVTEVT_EXE_MOVIEGDC,            MovieExe_OnGdc               },
	{NVTEVT_EXE_MOVIESMEAR,          MovieExe_OnSmear             },
#endif

#endif
	{NVTEVT_NULL,                    0},  //End of Command Map
};
#if(WIFI_AP_FUNC==ENABLE)
CREATE_APP(UISetupObj, APP_WIFICMD)
#else
CREATE_APP(UISetupObj, APP_BASE)
#endif
