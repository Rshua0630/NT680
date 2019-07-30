
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "UIFramework.h"
#include "UIFrameworkExt.h"
//#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "PlaybackTsk.h"
#include "Utility.h"
#include "Audio.h"
#include "UIFlowWndPlay.h"
#include "UIInfo.h"
#include "UIResource.h"
#include "DxPower.h"
#include "PowerDef.h"
#include "UIFlowPlayFuncs.h"
#include "UIFlow.h"
#include "DpofVendor.h"
#include "ExifVendor.h"
#include "DCF.h"
#include "GxVideoFile.h"
#include "AppDisp_MoviePlayView.h"
#include "SMediaPlayAPI.h"
#define MoviePlay_GetCurrPlaySecs(x)             SMediaPlay_GetCurrPlaySecs(x)

//#NT#2016/03/03#Ben Wang -begin
//#NT#Add debug message for script test.
#define MOVIE_PLAY_TEST_MIN_LEN  5 //sec
//#NT#2016/03/03#Ben Wang -end

/*
    DSCMode/Flash/EV Icon/Image size/WB/image quality/sharpness
    DZoom/Battery/Histogram/Protect/Date/Time
*/

static void FlowPB_IconDrawDSCMode(BOOL bShow)
{
	UINT32 uiFlag;
	UINT32 uiFileFmt = 0;
	PB_GetParam(PBPRMID_CURR_FILEFMT, &uiFileFmt);
	//hide icon
	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_DSCModeCtrl, FALSE);
		return;
	}
	//show icon
	if (uiFileFmt & (PBFMT_MOVMJPG | PBFMT_AVI | PBFMT_MP4 | PBFMT_TS)) {
		uiFlag = ICON_FILE_VIDEO;
		//#NT#2016/03/03#Ben Wang -begin
		//#NT#Add debug message for script test.
		if (MoviePlayExe_GetDataPlay(PLAYMOVIE_TOTAL_SECOND) > MOVIE_PLAY_TEST_MIN_LEN) {
			DBG_TEST("Movie Length = %d sec.\r\n", MoviePlayExe_GetDataPlay(PLAYMOVIE_TOTAL_SECOND));
		}
		//#NT#2016/03/03#Ben Wang -end
	} else {
		uiFlag = ICON_MODE_PLAYBACK;
	}
	UxStatic_SetData(&UIFlowWndPlay_StaticICN_DSCModeCtrl, STATIC_VALUE, uiFlag);
	UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_DSCModeCtrl, TRUE);
}

static void FlowPB_IconDrawFlash(BOOL bShow)
{
	EXIF_GETTAG exifTag;
	UINT16 uiExifFlash;
	BOOL bIsLittleEndian = TRUE;
	UINT32 uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_AUTO;
	UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_FlashCtrl, bShow);
	// Hide icon
	if (bShow == FALSE) {
		return;

	}
	//show icon
	EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
	exifTag.uiTagIfd = EXIF_IFD_EXIF;
	exifTag.uiTagId = TAG_ID_FLASH;
	if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
		uiExifFlash = Get16BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
		uiExifFlash &= ((1 << 6) + (1 << 4) + (1 << 3)); //only check bit 3,4,6
		switch (uiExifFlash) {
		case StrobeFlashAuto:
			uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_AUTO;
			break;
		case StrobeFlashForceON:
			uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_ON;
			break;
		case StrobeFlashForceOFF:
			uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_OFF;
			break;
		case StrobeFlashAutoRedEye:
			uiFlag = UIFlowWndPlay_StatusICN_Flash_ICON_FLASH_RED_EYE;
			break;
		default:
			UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_FlashCtrl, FALSE);
			break;
		}
		UxState_SetData(&UIFlowWndPlay_StatusICN_FlashCtrl, STATE_CURITEM, uiFlag);
	} else {
		UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_FlashCtrl, FALSE);
	}
}


void FlowPB_IconDrawEVIcon(BOOL bShow)
{
	//PJPGHEAD_DEC_CFG pJpgInfo;
	UINT32           uiFlag;
	//char             *str;
	CHAR str[5] = {0};
	EXIF_GETTAG exifTag;
	BOOL bIsLittleEndian = TRUE;
	UINT32 Data1, Data2;

	// Hide icon
	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_EVCtrl, FALSE);
		return;
	}
	//show icon
	//PB_GetParam(PBPRMID_INFO_IMG, (UINT32 *)&pJpgInfo);
	//str = (char *)pJpgInfo->pExifInfo->ExpBiasValue;

	EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));

	exifTag.uiTagIfd = EXIF_IFD_EXIF;
	exifTag.uiTagId = TAG_ID_EXPOSURE_BIAS_VALUE;
	if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
		Data1 = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
		Data2 = Get32BitsData(exifTag.uiTagDataAddr + 4, bIsLittleEndian);
		ExposureBiasToStr((INT32)Data1, (INT32)Data2, (UINT8 *)str);
	}
	if (!strncmp(str, "-2.0", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M2P0;
	} else if (!strncmp(str, "-5/3", 4) || !strncmp(str, "-1.7", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P6;
	} else if (!strncmp(str, "-4/3", 4) || !strncmp(str, "-1.3", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P3;
	} else if (!strncmp(str, "-1.0", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M1P0;
	} else if (!strncmp(str, "-2/3", 4) || !strncmp(str, "-0.7", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M0P6;
	} else if (!strncmp(str, "-1/3", 4) || !strncmp(str, "-0.3", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_M0P3;
	} else if (!strncmp(str, "+1/3", 4) || !strncmp(str, "+0.3", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P3;
	} else if (!strncmp(str, "+2/3", 4) || !strncmp(str, "+0.7", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P6;
	} else if (!strncmp(str, "+1.0", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P0;
	} else if (!strncmp(str, "+4/3", 4) || !strncmp(str, "+1.3", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P3;
	} else if (!strncmp(str, "+5/3", 4) || !strncmp(str, "+1.7", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P1P6;
	} else if (!strncmp(str, "+2.0", 4)) {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P2P0;
	} else {
		uiFlag = UIFlowWndPlay_StatusICN_EV_ICON_EV_P0P0;
	}

	UxState_SetData(&UIFlowWndPlay_StatusICN_EVCtrl, STATE_CURITEM, uiFlag);
	UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_EVCtrl, TRUE);
}

CHAR    gchaFullName[60] = { 0 };
static void FlowPB_IconDrawDCFFileID(BOOL bShow)
{
	static char item1_Buf[32];
#if USE_FILEDB
	UINT32 uiPBFileFmt = 0, rootFolderLength;
#endif


	//hide icon
	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_FilenameCtrl, FALSE);
		return;
	}

	//show icon
#if USE_FILEDB
	rootFolderLength = strlen(FILEDB_CARDV_ROOT);
	PB_GetParam(PBPRMID_CURR_FILEFMT, &uiPBFileFmt);
	PB_GetParam(PBPRMID_CURR_FILEPATH, (UINT32 *)&gchaFullName);

	//show icon

	if (uiPBFileFmt & PBFMT_AVI ||
		uiPBFileFmt & PBFMT_MOVMJPG ||
		uiPBFileFmt & PBFMT_MP4 ||
		uiPBFileFmt & PBFMT_TS) {

		if (uiPBFileFmt & PBFMT_READONLY) {
			strncpy(item1_Buf, (gchaFullName + rootFolderLength + 10 + 3), 20);
			item1_Buf[20] = '\0';
		} else {
			strncpy(item1_Buf, (gchaFullName + rootFolderLength + 10), 20);
			item1_Buf[20] = '\0';
		}
	} else {
		strncpy(item1_Buf, (gchaFullName + rootFolderLength + 10), 20);
		item1_Buf[20] = '\0';
	}
#else
	UINT32 uiDirID, uiFileID;
	PB_GetParam(PBPRMID_NAMEID_DIR, &uiDirID);
	PB_GetParam(PBPRMID_NAMEID_FILE, &uiFileID);
	snprintf(item1_Buf, 32, "%03ld-%04ld", uiDirID, uiFileID);
#endif

	//debug_msg("^GgchaFullName:%s,item1_Buf:%s\r\n",gchaFullName,item1_Buf);
	UxStatic_SetData(&UIFlowWndPlay_StaticTXT_FilenameCtrl, STATIC_VALUE, Txt_Pointer(item1_Buf));
	UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_FilenameCtrl, TRUE);
}

static void FlowPB_IconDrawImageSize(BOOL bShow)
{
	GXVIDEO_INFO MovieInfo;
	static char item1_Buf[32];
	UINT32 uiFileFmt = 0;
	JPGHEAD_DEC_CFG JPGInfo = {0};
	EXIF_GETTAG exifTag;
	BOOL bIsLittleEndian = TRUE;
	UINT32 OriImgWidth, OriImgHeight;
	PB_GetParam(PBPRMID_CURR_FILEFMT, &uiFileFmt);
	PB_GetParam(PBPRMID_INFO_IMG, (UINT32 *)&JPGInfo);
	EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
	exifTag.uiTagIfd = EXIF_IFD_EXIF;

	exifTag.uiTagId = TAG_ID_PIXEL_X_DIMENSION;
	if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
		OriImgWidth = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
	} else {
		OriImgWidth = (UINT32)JPGInfo.ori_imagewidth;
	}
	exifTag.uiTagId = TAG_ID_PIXEL_Y_DIMENSION;
	if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
		OriImgHeight = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
	} else {
		OriImgHeight = (UINT32)JPGInfo.ori_imageheight;
	}

	//hide icon
	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SizeCtrl, FALSE);
		return;
	}
	//show icon
	if (uiFileFmt & (PBFMT_MOVMJPG | PBFMT_AVI | PBFMT_MP4 | PBFMT_TS)) {
		PB_GetParam(PBPRMID_INFO_VDO, (UINT32 *)&MovieInfo);
		switch (MovieInfo.uiVidWidth) {
		case 3840: //3840x2160
			if (MovieInfo.uiVidRate == 50) {
				snprintf(item1_Buf, 32, "UHD P50");
			} else {
				snprintf(item1_Buf, 32, "UHD P30");
			}
			break;
		case 2880: //2880X2160
			if (MovieInfo.uiVidRate == 50) {
				snprintf(item1_Buf, 32, "UHD P50");
			} else {
				snprintf(item1_Buf, 32, "UHD P24");
			}
			break;
		case 2704: //2704x2032
			snprintf(item1_Buf, 32, "2.7K P60");
			break;
		case 2560: // 2560x1440
			if (MovieInfo.uiVidRate == 80) {
				snprintf(item1_Buf, 32, "QHD P80");
			} else if (MovieInfo.uiVidRate == 60) {
				snprintf(item1_Buf, 32, "QHD P60");
			} else {
				snprintf(item1_Buf, 32, "QHD P30");
			}
			break;
		case 2304: // 2304x1296P30
			snprintf(item1_Buf, 32, "3MHD P30");
			break;
		case 1920:
			if (MovieInfo.uiVidRate == 120) {
				snprintf(item1_Buf, 32, "FHD P120");
			} else if (MovieInfo.uiVidRate == 96) {
				snprintf(item1_Buf, 32, "FHD P96");
			} else if (MovieInfo.uiVidRate == 60) {
				snprintf(item1_Buf, 32, "FHD P60");
			} else {
				snprintf(item1_Buf, 32, "FHD P30");
			}
			break;
		case 1280:
			if (MovieInfo.uiVidRate == 240) {
				snprintf(item1_Buf, 32, "HD P240");
			} else if (MovieInfo.uiVidRate == 120) {
				snprintf(item1_Buf, 32, "HD P120");
			} else if (MovieInfo.uiVidRate == 60) {
				snprintf(item1_Buf, 32, "HD P60");
			} else {
				snprintf(item1_Buf, 32, "HD P30");
			}
			break;
		case 848:
			snprintf(item1_Buf, 32, "WVGA P30");
			break;
		case 640:
			if (MovieInfo.uiVidRate == 240) {
				snprintf(item1_Buf, 32, "VGA P240");
			} else {
				snprintf(item1_Buf, 32, "VGA P30");
			}
			break;
		case 320:
			snprintf(item1_Buf, 32, "QVGA P30");
			break;
		default:
			snprintf(item1_Buf, 32, "%lux%lu", OriImgWidth, OriImgHeight);
		}
	} else {
		snprintf(item1_Buf, 32, "%lux%lu", OriImgWidth, OriImgHeight);
	}


	UxStatic_SetData(&UIFlowWndPlay_StaticTXT_SizeCtrl, STATIC_VALUE, Txt_Pointer(item1_Buf));
	UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_SizeCtrl, TRUE);
}


static void FlowPB_IconDrawWB(BOOL bShow)
{
	//PJPGHEAD_DEC_CFG pJpgInfo;
	UINT32 uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_AUTO;
	EXIF_GETTAG exifTag;
	UINT16 uiExifWB;
	BOOL bIsLittleEndian = TRUE;
	UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_WBCtrl, bShow);
	//hide icon
	if (bShow == FALSE) {
		return;
	}
	//show icon
	EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
	exifTag.uiTagIfd = EXIF_IFD_EXIF;
	exifTag.uiTagId = TAG_ID_WHITE_BALANCE;
	if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
		uiExifWB = Get16BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
		switch (uiExifWB) {
		case WB_AUTO:
			uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_AUTO;
			break;
		case WB_DAYLIGHT:
			uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_DAYLIGHT;
			break;
		case WB_CLOUDY:
			uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_CLOUDY;
			break;
		case WB_TUNGSTEN:
			uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_TUNGSTEN;
			break;
		case WB_FLUORESCENT:
			uiFlag = UIFlowWndPlay_StatusICN_WB_ICON_WB_FLUORESCENT;
			break;
		default:
			UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_WBCtrl, FALSE);
			break;
		}
		UxState_SetData(&UIFlowWndPlay_StatusICN_WBCtrl, STATE_CURITEM, uiFlag);
	} else {
		UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_WBCtrl, FALSE);
	}
}

static void FlowPB_IconDrawImageQuality(BOOL bShow)
{
	UINT32 uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_FINE;
	UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_QualityCtrl, bShow);
	if (bShow == FALSE) {
		return;
	}
	switch (ExifVendor_GetData(EXIFVENDOR_QUALITY)) {
	case QUALITY_SUPER:
		uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_FINE;
		break;
	case QUALITY_FINE:
		uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_NORMAL;
		break;
	case QUALITY_NORMAL:
		uiFlag = UIFlowWndPlay_StatusICN_Quality_ICON_QUALITY_BASIC;
		break;
	default:
		UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_QualityCtrl, FALSE);
		break;
	}

	UxState_SetData(&UIFlowWndPlay_StatusICN_QualityCtrl, STATE_CURITEM, uiFlag);
}

static void FlowPB_IconDrawSharpness(BOOL bShow)
{
	UINT32 uiFlag = UIFlowWndPlay_StatusICN_Sharpness_ICON_SHARPNESS_MED;;
	EXIF_GETTAG exifTag;
	UINT16 uiSharpness;
	BOOL bIsLittleEndian = TRUE;
	UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_SharpnessCtrl, bShow);
	if (bShow == FALSE) {
		return;
	}
	EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
	exifTag.uiTagIfd = EXIF_IFD_EXIF;
	exifTag.uiTagId = TAG_ID_SHARPNESS;
	if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
		uiSharpness = Get16BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
		switch (uiSharpness) {
		case NORMAL_SHARPNESS:
			uiFlag = UIFlowWndPlay_StatusICN_Sharpness_ICON_SHARPNESS_MED;
			break;
		case SOFT_SHARPNESS:
			uiFlag = UIFlowWndPlay_StatusICN_Sharpness_ICON_SHARPNESS_LOW;
			break;
		case HARD_SHARPNESS:
			uiFlag = UIFlowWndPlay_StatusICN_Sharpness_ICON_SHARPNESS_HIGH;
			break;
		default:
			UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_SharpnessCtrl, FALSE);
			break;
		}
		UxState_SetData(&UIFlowWndPlay_StatusICN_SharpnessCtrl, STATE_CURITEM, uiFlag);
	} else {
		UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_SharpnessCtrl, FALSE);
	}
}

static void FlowPB_IconDrawStorage(BOOL bShow)
{
	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_StorageCtrl, FALSE);
		return;
	}
	switch (System_GetState(SYS_STATE_CARD)) {
	case CARD_REMOVED:
		UxState_SetData(&UIFlowWndPlay_StatusICN_StorageCtrl, STATE_CURITEM, UIFlowWndPlay_StatusICN_Storage_ICON_INTERNAL_FLASH);
		break;
	default:
		UxState_SetData(&UIFlowWndPlay_StatusICN_StorageCtrl, STATE_CURITEM, UIFlowWndPlay_StatusICN_Storage_ICON_SD_CARD);
		break;
	}

	UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_StorageCtrl, TRUE);
}

static void FlowPB_IconDrawBattery(BOOL bShow)
{

	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_BatteryCtrl, FALSE);
		return;
	}

	UxState_SetData(&UIFlowWndPlay_StatusICN_BatteryCtrl, STATE_CURITEM, GetBatteryLevel());
	UxCtrl_SetShow(&UIFlowWndPlay_StatusICN_BatteryCtrl, TRUE);
}




static void FlowPB_IconDrawFileLock(BOOL bShow)
{
	UINT32 uiLockStatus;
	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, FALSE);
		return;
	}
	PB_GetParam(PBPRMID_FILE_ATTR_LOCK, &uiLockStatus);
	if (uiLockStatus) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, TRUE);
		DBG_TEST("file locked\r\n");
		//#NT#2016/11/10#Niven Cho -begin
		//#NT#AUTO_TEST
		exam_msg("file locked\r\n");
		//#NT#2016/11/10#Niven Cho -end
	} else {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticICN_ProtectCtrl, FALSE);
	}
}



void FlowPB_IconDrawDate(BOOL bShow)
{
	UINT32  creDateTime[6], modDateTime[6] = {0};
	//UINT32  uiDirNum,uiFileNum,uiFileType,index;
//    CHAR    chaFullName[34] = { 0 };
	static char item1_Buf[34];

	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_DateCtrl, FALSE);
		return;
	}

	FileSys_WaitFinish();
#if 0
	PB_GetParam(PBPRMID_NAMEID_DIR, &uiDirNum);
	PB_GetParam(PBPRMID_NAMEID_FILE, &uiFileNum);
	index = DCF_GetIndexByID(uiDirNum, uiFileNum);
	DCF_GetObjInfo(index, &uiDirNum, &uiFileNum, &uiFileType);
	DCF_GetObjPath(index, uiFileType, chaFullName);
#endif
	PB_GetParam(PBPRMID_CURR_FILEPATH, (UINT32 *)&gchaFullName);
	if (FST_STA_OK == FileSys_GetDateTime(gchaFullName, creDateTime, modDateTime)) {
		modDateTime[0] %= 100;

		if (modDateTime[0] > 50) {
			modDateTime[0] = 50;
		}
		if (modDateTime[1] > 12) {
			modDateTime[0] = 12;
		}
		if (modDateTime[2] > 31) {
			modDateTime[0] = 31;
		}
		snprintf(item1_Buf, 20, "%02lu/%02lu/%02lu", modDateTime[0], modDateTime[1], modDateTime[2]);

		UxStatic_SetData(&UIFlowWndPlay_StaticTXT_DateCtrl, STATIC_VALUE, Txt_Pointer(item1_Buf));
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_DateCtrl, TRUE);
	}
}


static void FlowPB_IconDrawTime(BOOL bShow)
{
	UINT32  creDateTime[6], modDateTime[6] = {0};
	//UINT32  uiDirNum,uiFileNum,uiFileType,index;
//    CHAR    chaFullName[34] = { 0 };
	static char item1_Buf[34];

	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_TimeCtrl, FALSE);
		return;
	}

	FileSys_WaitFinish();
#if 0
	PB_GetParam(PBPRMID_NAMEID_DIR, &uiDirNum);
	PB_GetParam(PBPRMID_NAMEID_FILE, &uiFileNum);
	index = DCF_GetIndexByID(uiDirNum, uiFileNum);
	DCF_GetObjInfo(index, &uiDirNum, &uiFileNum, &uiFileType);
	DCF_GetObjPath(index, uiFileType, chaFullName);
#endif
	PB_GetParam(PBPRMID_CURR_FILEPATH, (UINT32 *)&gchaFullName);
	if (FST_STA_OK == FileSys_GetDateTime(gchaFullName, creDateTime, modDateTime)) {
		if (modDateTime[3] > 23) {
			modDateTime[3] = 0;
		}
		if (modDateTime[4] > 59) {
			modDateTime[4] = 0;
		}

		snprintf(item1_Buf, 20, "%02lu:%02lu", modDateTime[3], modDateTime[4]);

		UxStatic_SetData(&UIFlowWndPlay_StaticTXT_TimeCtrl, STATIC_VALUE, Txt_Pointer(item1_Buf));
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_TimeCtrl, TRUE);
	}
}


static void FlowPB_IconDrawThumbSeq(BOOL bShow)
{
	static char item1_Buf[32];
	UINT32  *pThumbSeqID;
	UINT32 uiFileNum = 0;
	UINT32 uiBrowseIdx, uiParamVal;
	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl, FALSE);
		return;
	}
	PB_GetParam(PBPRMID_THUMB_SEQ_ARRAY, &uiParamVal);
	pThumbSeqID = (UINT32 *)uiParamVal;
	PB_GetParam(PBPRMID_TOTAL_FILE_COUNT, &uiFileNum);
	PB_GetParam(PBPRMID_THUMB_CURR_IDX, &uiBrowseIdx);
	snprintf(item1_Buf, 32, "%lu/%lu", *(pThumbSeqID + uiBrowseIdx - 1), uiFileNum);

	UxStatic_SetData(&UIFlowWndPlayThumb_ThumbIDCtrl, STATIC_VALUE, Txt_Pointer(item1_Buf));
	UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl, TRUE);
}

void FlowPB_IconDrawLeftBtn(BOOL bShow)
{
	UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_LeftBtnCtrl, bShow);
	if (bShow == FALSE) {
		return;
	}
	if (g_PlbData.State == PLB_ST_PLAY_MOV) {
		UxStatic_SetData(&UIFlowWndPlay_StaticTXT_LeftBtnCtrl, STATIC_VALUE, STRID_STOP);
	} else if (g_PlbData.State == PLB_ST_MAGNIFY) {
		UxStatic_SetData(&UIFlowWndPlay_StaticTXT_LeftBtnCtrl, STATIC_VALUE, STRID_RETURN);
	} else {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_LeftBtnCtrl, FALSE);
	}
}

void FlowPB_IconDrawRightBtn(BOOL bShow)
{
	UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_RightBtnCtrl, bShow);
	if (bShow == FALSE) {
		return;
	}
	if (g_PlbData.State == PLB_ST_PLAY_MOV) {
		UxStatic_SetData(&UIFlowWndPlay_StaticTXT_RightBtnCtrl, STATIC_VALUE, STRID_PAUSE);
	} else if (g_PlbData.State == PLB_ST_PAUSE_MOV) {
		UxStatic_SetData(&UIFlowWndPlay_StaticTXT_RightBtnCtrl, STATIC_VALUE, STRID_RESUME);
	} else {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_RightBtnCtrl, FALSE);
	}
}

void FlowPB_IconDrawTabNavi(BOOL bShow)
{
	UxCtrl_SetShow(&UIFlowWndPlay_TabNaviCtrl, bShow);
}

void FlowPB_IconDrawMovPlayTime(BOOL bShow)
{
	static char item1_Buf[32];
	UINT32 uiPlayTimeSecs;

	if (bShow == FALSE) {
		UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, FALSE);
		return;
	}

#if (NMEDIAPLAY_FUNC == DISABLE)
	uiPlayTimeSecs = MoviePlay_GetCurrPlaySecs();
#else
	uiPlayTimeSecs = AppDisp_MoviePlayView_GetCurrPlaySecs();
#endif

	snprintf(item1_Buf, 32, "%02ld:%02ld:%02ld", uiPlayTimeSecs / 3600, (uiPlayTimeSecs % 3600) / 60, (uiPlayTimeSecs % 3600) % 60);

	UxStatic_SetData(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, STATIC_VALUE, Txt_Pointer(item1_Buf));

	UxCtrl_SetShow(&UIFlowWndPlay_StaticTXT_MovPlayTimeCtrl, TRUE);
}

void FlowPB_UpdateIcons(BOOL bShow)
{
	if (bShow == FALSE) {
		FlowPB_IconDrawDSCMode(FALSE);
		FlowPB_IconDrawFlash(FALSE);
		FlowPB_IconDrawEVIcon(FALSE);
		FlowPB_IconDrawDCFFileID(FALSE);
		FlowPB_IconDrawImageSize(FALSE);
		FlowPB_IconDrawWB(FALSE);
		FlowPB_IconDrawImageQuality(FALSE);
		FlowPB_IconDrawSharpness(FALSE);
		FlowPB_IconDrawStorage(FALSE);
		FlowPB_IconDrawBattery(FALSE);
		FlowPB_IconDrawFileLock(FALSE);
		FlowPB_IconDrawDate(FALSE);
		FlowPB_IconDrawTime(FALSE);
		FlowPB_IconDrawMovPlayTime(FALSE);
		FlowPB_IconDrawLeftBtn(FALSE);
		FlowPB_IconDrawRightBtn(FALSE);
		FlowPB_IconDrawTabNavi(FALSE);
	} else {
		UINT32 uiCurrMode;
		PB_GetParam(PBPRMID_PLAYBACK_MODE, &uiCurrMode);
		FlowPB_IconDrawDSCMode(TRUE);
		FlowPB_IconDrawDCFFileID(TRUE);
		FlowPB_IconDrawImageSize(TRUE);
		FlowPB_IconDrawStorage(TRUE);
		FlowPB_IconDrawBattery(TRUE);
		FlowPB_IconDrawFileLock(TRUE);
		FlowPB_IconDrawDate(TRUE);
		FlowPB_IconDrawTime(TRUE);
		FlowPB_IconDrawMovPlayTime(FALSE);
		FlowPB_IconDrawLeftBtn(FALSE);
		FlowPB_IconDrawRightBtn(FALSE);
		FlowPB_IconDrawTabNavi(TRUE);
		if (uiCurrMode == PLAYMODE_PRIMARY) {
			FlowPB_IconDrawWB(TRUE);
			FlowPB_IconDrawImageQuality(TRUE);
			FlowPB_IconDrawSharpness(TRUE);
			FlowPB_IconDrawFlash(TRUE);
			FlowPB_IconDrawEVIcon(TRUE);
		} else {
			FlowPB_IconDrawWB(FALSE);
			FlowPB_IconDrawImageQuality(FALSE);
			FlowPB_IconDrawSharpness(FALSE);
			FlowPB_IconDrawFlash(FALSE);
			FlowPB_IconDrawEVIcon(FALSE);
		}
	}
}

//#NT#2012/7/4#Philex - begin
void FlowPB_DrawThumbRect(BOOL bShow)
{
	UINT32 uiBrowseIdx;
	PB_GetParam(PBPRMID_THUMB_CURR_IDX, &uiBrowseIdx);
	switch (uiBrowseIdx) {
	//Playback fileIndex starts from 1.
	case 1:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel0Ctrl, bShow);
		break;
	case 2:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel1Ctrl, bShow);
		break;
	case 3:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel2Ctrl, bShow);
		break;
	case 4:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel3Ctrl, bShow);
		break;
	case 5:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel4Ctrl, bShow);
		break;
	case 6:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel5Ctrl, bShow);
		break;
	case 7:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel6Ctrl, bShow);
		break;
	case 8:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel7Ctrl, bShow);
		break;
	case 9:
		UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel8Ctrl, bShow);
		break;
	}
}


void FlowPB_ClearAllThumbIcon(void)
{
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel0Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel1Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel2Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel3Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel4Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel5Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel6Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel7Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_Panel8Ctrl, FALSE);

	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect0Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect1Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect2Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect3Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect4Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect5Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect6Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect7Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect8Ctrl, FALSE);

	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm0Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm1Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm2Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm3Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm4Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm5Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm6Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm7Ctrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm8Ctrl, FALSE);

	UxCtrl_SetShow(&UIFlowWndPlayThumb_ThumbIDCtrl, FALSE);
}


void FlowPB_ShowAllThumbIcon(void)
{
	UINT32  i, loopcnts, CurrFileFormat, uiParamVal;
	UINT16  *pCurrFileFormat;   // Hideo@0503: variable type should be UINT16

	FlowPB_DrawThumbRect(TRUE);

	FlowPB_IconDrawThumbSeq(TRUE);
	PB_GetParam(PBPRMID_THUMB_FMT_ARRAY, &uiParamVal);
	pCurrFileFormat = (UINT16 *)uiParamVal;

	PB_GetParam(PBPRMID_THUMB_CURR_NUM, &loopcnts);
	for (i = 0; i < loopcnts; i++) {
		CurrFileFormat  = *pCurrFileFormat++;

		if (CurrFileFormat & PBFMT_READONLY) {
			switch (i) {
			case 0:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect0Ctrl, TRUE);
				break;
			case 1:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect1Ctrl, TRUE);
				break;
			case 2:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect2Ctrl, TRUE);
				break;
			case 3:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect3Ctrl, TRUE);
				break;
			case 4:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect4Ctrl, TRUE);
				break;
			case 5:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect5Ctrl, TRUE);
				break;
			case 6:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect6Ctrl, TRUE);
				break;
			case 7:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect7Ctrl, TRUE);
				break;
			case 8:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_Protect8Ctrl, TRUE);
				break;
			}
		}

		if (CurrFileFormat & (PBFMT_MOVMJPG | PBFMT_AVI | PBFMT_MP4 | PBFMT_TS)) {
			switch (i) {
			case 0:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm0Ctrl, TRUE);
				break;
			case 1:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm1Ctrl, TRUE);
				break;
			case 2:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm2Ctrl, TRUE);
				break;
			case 3:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm3Ctrl, TRUE);
				break;
			case 4:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm4Ctrl, TRUE);
				break;
			case 5:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm5Ctrl, TRUE);
				break;
			case 6:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm6Ctrl, TRUE);
				break;
			case 7:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm7Ctrl, TRUE);
				break;
			case 8:
				UxCtrl_SetShow(&UIFlowWndPlayThumb_StaticICN_FileFilm8Ctrl, TRUE);
				break;
			}
		}
	}
}



