////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"
#include "UIAppPhoto.h"
#include "ExifVendor.h"
#include "GxSound.h"
#include "af_api.h"
#include "Flashlight_api.h"
#include "GxFlash.h"
#include "ImageApp_Photo.h"

#include "dma.h"
#include "KeyDef.h"


#include "ImageUnit_VdoOut.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiPhotoCB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

extern BOOL _g_bFirstPhoto;
extern UINT32 System_GetEnableSensor(void);

void Photo_GenDateStrCB(void)
{
	// update the date time str
	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_GEN_DATE_STR, 0);
}

void Photo_GenDatePicCB(IMG_CAP_DATASTAMP_INFO  *dateInfo)
{
	// Generate the date time picture
	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_GEN_DATE_PIC, 1, (UINT32)dateInfo);
}

void Photo_WriteExifCB(UINT32 *SensorID)

{
	// Write Exif
	EXIF_HANDLE_ID HandleID;

	HandleID = *SensorID;
	DBG_IND("HandleID = %d\r\n", HandleID);
	ExifVendor_Write0thIFD(HandleID);
	ExifVendor_WriteExifIFD(HandleID);
	ExifVendor_Write0thIntIFD(HandleID);
}

void Photo_WriteDebugCB(IMG_CAP_DEBUG_INFO *DebugInfo)
{
	UINT32 Addr;

	Addr = DebugInfo->Addr;
#if !defined(_IPL1_IPL_FAKE_)
	//if (System_GetEnableSensor() == (SENSOR_2) )
	if (DebugInfo->SensorId == 1) {
		Addr = AE_WriteDebugInfo(1, Addr);
		Addr = AWB_WriteDebugInfo(1, Addr);
	} else {
		Addr = AE_WriteDebugInfo(0, Addr);
		Addr = AWB_WriteDebugInfo(0, Addr);
	}
#if _TODO
	Addr = Flash_WriteDebugInfo(Addr);
#endif
#if (LENS_FUNCTION == ENABLE)
	Addr = AF_WriteDebugInfo(Addr);
#endif
#endif
	DebugInfo->UsedSize = Addr - DebugInfo->Addr;
	if (Addr - DebugInfo->Addr > DebugInfo->TotalSize) {
		DBG_ERR("Debug message of JPEG end is overflow!!\r\n");
	}
}

void Photo_ShutterSoundCB(void)
{
}

void Photo_AFCB(AF_CBMSG uiMsgID, void *Data)
{

	switch (uiMsgID) {
	case AF_CBMSG_AF_START:
		DBG_IND("===AF Start! - ===\r\n");
#if (FLASHLIGHT_FUNCTION == ENABLE)
		GxFlash_SuspendCharge();
#endif
		break;
	case AF_CBMSG_AF_DONE:
		DBG_IND("===AF Done! - ===\r\n");
#if (FLASHLIGHT_FUNCTION == ENABLE)
		GxFlash_ResumeCharge();
#endif
		Ux_PostEvent(NVTEVT_ALGMSG_FOCUSEND, 1, NVTEVT_ALGMSG_FOCUSEND);
		break;
	default:
		break;
	}
}





static IMG_CAP_QV_DATA gImgQvData = {0};


//-------------------------------------------------------------------------------------------------
//
// Callback function for IPL events
//
//-------------------------------------------------------------------------------------------------
extern BOOL bEnableAE;
extern BOOL bStableAE;
extern BOOL bEnableVdo;
extern BOOL bShowPreview;

void Photo_IPLCB(IPL_PROC_ID Id, IPL_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {
	case IPL_CBMSG_PREVIEW:
		DBG_IND("===IPL_CBMSG_PREVIEW===\r\n");
		if(!bEnableAE)
			bEnableAE = ae_isopen(AE_ID_1);
		if (_g_bFirstPhoto) {
			TM_BOOT_BEGIN("sensor", "photo_2a_stable");
		}
		Ux_PostEvent(NVTEVT_CALLBACK, 1, NVTEVT_ALGMSG_PREVIEW);
		break;
	case IPL_CBMSG_DZOOM:
		DBG_IND("===IPL_CBMSG_DZOOM===\r\n");
		// post event for update UI
		Ux_PostEvent(NVTEVT_CB_ZOOM, 0);
		break;

	case IPL_CBMSG_SHUTTER_SOUND:
		Photo_ShutterSoundCB();
		break;
	default:
		break;
	}
}

#include "ae_cb_msg.h"
static void Photo_AE_CB(AE_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {

	case AE_CBMSG_PREVIEWSTABLE:
		DBG_IND("===AE_CBMSG_PREVIEWSTABLE===\r\n");
		if (_g_bFirstPhoto) {
			TM_BOOT_END("sensor", "photo_2a_stable");
		}
		if (_g_bFirstPhoto) {
			_g_bFirstPhoto = FALSE;
			bStableAE = TRUE;
			if(bEnableVdo) {
				if(!bShowPreview) {
					bShowPreview = TRUE;
					Display_ShowPreview(); //show preview immediately (AE should be last one)
					DBG_DUMP("^YShow Preview! (IPL > Video VSync > AE stable)\r\n");
				}
			}
		}
		Ux_PostEvent(NVTEVT_CALLBACK, 1, NVTEVT_ALGMSG_PREVIEW_STABLE);
	default:
		break;
	}
}


//-------------------------------------------------------------------------------------------------
//
// Callback function for Capture events
//
//-------------------------------------------------------------------------------------------------
void Photo_CaptureCB(IMG_CAP_CBMSG Msg, void *Data)
{
	switch (Msg) {
	case IMG_CAP_CBMSG_GET_RAWEND:
		DxSys_DumpCaptureDrvInfo();
		break;
	case IMG_CAP_CBMSG_GET_ONERAWEND:
		DBG_IND("===GET_ONERAWEND===\r\n");
		break;
	case IMG_CAP_CBMSG_JPEG_OK:
		//Data[0] = PriBS_Addr,Data[1] = PriBS_Size; Data[2] = ScrBS_Addr,Data[3] = ScrBS_Size; Data[4] = ThumbBS_Addr,Data[5] = ThumbBS_Size
		DBG_IND("===JPEGOK===\r\n");
		Ux_PostEvent(NVTEVT_CALLBACK, 1, NVTEVT_ALGMSG_JPGOK);
		break;
	case IMG_CAP_CBMSG_QUICKVIEW:
		DBG_IND("===QVSTART===\r\n");
		{
			memcpy(&gImgQvData, Data, sizeof(gImgQvData));
			Ux_PostEvent(NVTEVT_CALLBACK, 2, NVTEVT_ALGMSG_QVSTART, (UINT32)&gImgQvData);
		}
		break;
	case IMG_CAP_CBMSG_CAPEND:
		DBG_IND("===CAPTUREEND===\r\n");
		Ux_PostEvent(NVTEVT_CALLBACK, 1, NVTEVT_ALGMSG_CAPTUREEND);
		break;
	case IMG_CAP_CBMSG_FSTOK:
		DBG_IND("===CAPFSTOK===\r\n");
		{
			IMG_CAP_FST_INFO    *pFstInfo;
			pFstInfo = (IMG_CAP_FST_INFO *)Data;
			//#NT#2016/10/04#Lincy Lin -begin
			//#NT#Support SD hot plug function
			if (pFstInfo->Status != FST_STA_OK) {
				Photo_setS2Status(FALSE);
			}
			//#NT#2016/10/04#Lincy Lin -end
			Ux_PostEvent(NVTEVT_CALLBACK, 2, NVTEVT_ALGMSG_CAPFSTOK, pFstInfo->Status);
		}
		break;
	case IMG_CAP_CBMSG_GEN_DATASTAMP_STR:
		DBG_IND("===Gen DataStamp str===\r\n");
		Photo_GenDateStrCB();
		break;
	case IMG_CAP_CBMSG_GEN_DATASTAMP_PIC:
		DBG_IND("===Gen DataStamp Pic===\r\n");
		Photo_GenDatePicCB(Data);
		break;
	case IMG_CAP_CBMSG_RET_PRV:
		//Before change to preview
		DBG_IND("===Return Preview===\r\n");
		break;
	case IMG_CAP_CBMSG_SET_EXIF_DATA:
		DBG_IND("===Set Exif data===\r\n");
		Photo_WriteExifCB((UINT32 *)Data);
		break;
	case IMG_CAP_CBMSG_WRITE_DEBUG_DATA:
		DBG_IND("===Write debug data===\r\n");
		Photo_WriteDebugCB((IMG_CAP_DEBUG_INFO *)Data);
		break;
	case IMG_CAP_CBMSG_MFHDR:
		DBG_IND("=== HDR ===\r\n");
		break;
	case IMG_CAP_CBMSG_MFANTISHAKE:
		DBG_IND("=== Stab ===\r\n");
		break;
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for FD,SD events
//
//-------------------------------------------------------------------------------------------------
#if _FD_FUNC_
void Photo_FdCB(FD_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {
	case FD_CBMSG_DETEND:
		DBG_IND("===FDEND===\r\n");
		Ux_PostEvent(NVTEVT_EXE_FDEND, 1, NVTEVT_EXE_FDEND);
		break;


	default:
		break;
	}
}
#endif


//-------------------------------------------------------------------------------------------------
//
// Callback function for S2 key detect
//
//-------------------------------------------------------------------------------------------------
BOOL isS2Pressed = TRUE;

void Photo_setS2Status(BOOL isPressed)
{
	isS2Pressed = isPressed;
}


static BOOL Photo_DetS2CB(void)
{
#if 1
	DBG_IND("isS2Pressed=%d\r\n", isS2Pressed);
	return isS2Pressed;
#else
	if ((GxKey_GetData(GXKEY_NORMAL_KEY) & FLGKEY_SHUTTER2)) {
		return TRUE;
	} else if ((GxKey_GetData(GXKEY_NORMAL_KEY) & FLGKEY_LEFT)) {
		return TRUE;
	}
	return FALSE;
#endif
}


void Photo_RegCB(void)
{
#if _FD_FUNC_
	FD_RegisterCB(Photo_FdCB);
#endif
#if !defined(_IPL1_IPL_FAKE_)
	// Register AF callback func
	AF_RegisterCB(Photo_AFCB);
#endif
	if (System_GetEnableSensor() == (SENSOR_2)) {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)Photo_IPLCB);
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(1), Photo_AE_CB);
	} else {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)Photo_IPLCB);
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(0), Photo_AE_CB);
	}
	ImageUnit_Begin(&ISF_Cap, 0);
	ImageUnit_SetParam(ISF_CTRL, CAP_PARAM_CAP_CB, (UINT32)Photo_CaptureCB);
	ImageUnit_SetParam(ISF_CTRL, CAP_PARAM_DETS2_CB, (UINT32)Photo_DetS2CB);
	ImageUnit_End();
}

