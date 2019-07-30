////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Photo.h"
#include "FileSysTsk.h"
#include "imgcapinfor.h"
#include <string.h>
#if(IPCAM_FUNC != ENABLE)
#include "UIPhotoMapping.h"
#else
#include "UIAppIPCam_Param.h"
#endif
#include "UIAppPhoto.h"
#include "DCF.h"
#include "ipl_cmd.h"
#include "PrjCfg.h"
#include "DxSensor.h"
#include "imgcaptureapi.h"
#include "display.h"
#include "ipl_alg_infor.h"

#define THIS_DBGLVL         2       // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          MsdcNvt_Photo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define CFG_MSDCPHOTO_DISP_ID DISP_1
#define CFG_MSDCPHOTO_IPL_ID 0

#if !defined(_IPL1_IPL_FAKE_)

//for MSDCVendorNVT_AddCallback_Bi
static void MsdcNvtCb_PhotoBegin(void *pData);
static void MsdcNvtCb_GetDisplaySettings2(void *pData);
static void MsdcNvtCb_GetDisplayCurrentBufSel(void *pData);
static void MsdcNvtCb_CaptureJpgWidthGetInfo(void *pData);
static void MsdcNvtCb_CaptureRawWidthGetInfo(void *pData);

MSDCNVT_REG_BI_BEGIN(m_MsdcNvtPhoto)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_PhotoBegin)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_GetDisplaySettings2)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_GetDisplayCurrentBufSel)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_CaptureJpgWidthGetInfo)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_CaptureRawWidthGetInfo)
MSDCNVT_REG_BI_END()

BOOL MsdcNvtRegBi_Photo(void)
{
	return MsdcNvt_AddCallback_Bi(m_MsdcNvtPhoto);
}


static void MsdcNvtCb_PhotoBegin(void *pData)
{
	tMSDCEXT_PARENT *pDesc = MSDCNVT_CAST(tMSDCEXT_PARENT, pData);
	if (pDesc == NULL) {
		return;
	}

	pDesc->bOK = FALSE;

	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PHOTO &&
		System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE) {
		INT32 nRetry = 20;
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
		//polling to wait finish
		while (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PHOTO &&
			   System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE &&
			   nRetry-- > 0) {
			Delay_DelayMs(500);
		}
	}

	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO ||
		System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
		pDesc->bOK = TRUE;
	}
}

static void MsdcNvtCb_GetDisplaySettings2(void *pData)
{
	DISPLAYER_PARAM lyr_param;
	DISP_OBJ *p_dispobj = disp_getDisplayObject(CFG_MSDCPHOTO_DISP_ID);
	tMSDCEXT_DISPLAY_SETTING2 *pDesc = MSDCNVT_CAST(tMSDCEXT_DISPLAY_SETTING2, pData);

	if (pDesc == NULL) {
		return;
	}

	pDesc->Parent.bOK = FALSE;

	pDesc->nValidFrm = 1;

	memset(&lyr_param, 0, sizeof(DISPLAYER_PARAM));
	p_dispobj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_GET_VDOBUFADDR, &lyr_param);
	pDesc->AddrY[0] = lyr_param.SEL.GET_VDOBUFADDR.uiAddrY0;
	pDesc->AddrU[0] = lyr_param.SEL.GET_VDOBUFADDR.uiAddrCb0;
	pDesc->AddrV[0] = lyr_param.SEL.GET_VDOBUFADDR.uiAddrCr0;

	memset(&lyr_param, 0, sizeof(DISPLAYER_PARAM));
	p_dispobj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_GET_MODE, &lyr_param);
	DISPBUFFORMAT BufFormat = lyr_param.SEL.GET_MODE.BufFormat;

	memset(&lyr_param, 0, sizeof(DISPLAYER_PARAM));
	p_dispobj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_GET_BUFSIZE, &lyr_param);

	pDesc->PitchY = lyr_param.SEL.GET_BUFSIZE.uiBufLineOfs;
	pDesc->PitchUV = pDesc->PitchY;
	pDesc->Width = lyr_param.SEL.GET_BUFSIZE.uiBufWidth;
	pDesc->HeightY = lyr_param.SEL.GET_BUFSIZE.uiBufHeight;

	switch (BufFormat) {
	case DISPBUFFORMAT_YUV422PACK:
		pDesc->HeightUV = pDesc->HeightY;
		break;
	case DISPBUFFORMAT_YUV420PACK:
		pDesc->HeightUV = pDesc->HeightY >> 1;
		break;
	default:
		DBG_ERR("unsupport display format: DISPBUFFORMAT_ %d\r\n", BufFormat);
	}

	pDesc->Parent.bOK = TRUE;
}

static void MsdcNvtCb_GetDisplayCurrentBufSel(void *pData)
{
	tMSDCEXT_DISPLYA_CURRENT *pDesc = MSDCNVT_CAST(tMSDCEXT_DISPLYA_CURRENT, pData);

	if (pDesc == NULL) {
		return;
	}

	pDesc->BUFNUM = 1;
	pDesc->OPTBUF = 0;
	pDesc->Parent.bOK = TRUE;
}

static void MsdcNvtCb_CaptureJpgWidthGetInfo(void *pData)
{
	tMSDCEXT_INFO_CAPTURE_JPG *pDesc = MSDCNVT_CAST(tMSDCEXT_INFO_CAPTURE_JPG, pData);
	if (pDesc == NULL) {
		return;
	}

	pDesc->Parent.bOK = FALSE;
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_PICNUM, 1);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_RAW_BUF_NUM, 1);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_BUF_NUM, 1);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_FILEFMT, SEL_FILEFMT_NONE);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPGFMT, SEL_JPGFMT_422);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_ENCRATIO, 100);
	#if (IPCAM_FUNC != ENABLE)
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE)));
	#else
		UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_H_SIZE, pEncodeConfig[0].resolution.w);
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_V_SIZE, pEncodeConfig[0].resolution.h);
	#endif
	ImgCap_StartCapture();
	ImgCap_WaitFinish();

	ImgCap_GetCurJpgInfo(&pDesc->JpgAddr, &pDesc->JpgFileSize);

	pDesc->Parent.bOK = TRUE;
}

static void MsdcNvtCb_CaptureRawWidthGetInfo(void *pData)
{
	UINT32 BufSize = 0;
	UINT32 uiPoolAddrEnd = 0;
#if (USE_DCF == ENABLE)
	UINT32 uiFolderId = 0;
	UINT32 uiFileId = 0;
#endif
	FST_FILE hFile = NULL;
	const SEL_RAW_BITDEPTH BitDepth = SEL_RAW_BITDEPTH_8;

	IPL_SIE_RAW_INFO IplInfo = {0};
	char path[DCF_FULL_FILE_PATH_LEN];

	tMSDCEXT_INFO_CAPTURE_RAW *pDesc = MSDCNVT_CAST(tMSDCEXT_INFO_CAPTURE_RAW, pData);

	if (pDesc == NULL) {
		return;
	}

	pDesc->Parent.bOK = FALSE;
#if (USE_DCF == ENABLE)
	DCF_GetNextID(&uiFolderId, &uiFileId);
	DCF_MakeObjPath(uiFolderId, uiFileId, DCF_FILE_TYPE_RAW, path);
#endif
	IPL_AlgSetUIInfo(IPL_PATH(0), IPL_SEL_RAW_BITDEPTH_CAP, BitDepth);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_PICNUM, 1);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_RAW_BUF_NUM, 1);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_BUF_NUM, 1);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_FILEFMT, SEL_FILEFMT_RAW);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPGFMT, SEL_JPGFMT_422);
	ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_ENCRATIO, 100);
	#if (IPCAM_FUNC != ENABLE)
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE)));
	#else
		UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_H_SIZE, pEncodeConfig[0].resolution.w);
		ImgCap_SetUIInfo(CFG_MSDCPHOTO_IPL_ID, CAP_SEL_JPG_IMG_V_SIZE, pEncodeConfig[0].resolution.h);
	#endif
	ImgCap_StartCapture();
	ImgCap_WaitFinish();

	uiPoolAddrEnd = OS_GetMempoolAddr(POOL_ID_APP);
	if (uiPoolAddrEnd == 0) {
		pDesc->Parent.bOK = FALSE;
		return;
	}
	uiPoolAddrEnd = uiPoolAddrEnd + POOL_SIZE_APP;

	IplInfo.id = IPL_PATH(0);
	IPL_GetCmd(IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR, &IplInfo);
	pDesc->RawWidth = IplInfo.img_line_ofs;
	pDesc->RawHeight = IplInfo.img_height;

	switch (BitDepth) {
	case SEL_RAW_BITDEPTH_8:
		pDesc->RawNbit = 8;
		break;
	case SEL_RAW_BITDEPTH_10:
		pDesc->RawNbit = 10;
		break;
	case SEL_RAW_BITDEPTH_12:
		pDesc->RawNbit = 12;
		break;
	case SEL_RAW_BITDEPTH_16:
		pDesc->RawNbit = 16;
		break;
	default:
		break;
	}

	pDesc->RawFileSize = pDesc->RawWidth * pDesc->RawHeight * ((pDesc->RawNbit + 7) >> 3);
	ImgCap_GetCurJpgInfo(&pDesc->RawAddr, &BufSize);
	pDesc->RawAddr = ALIGN_FLOOR_32(pDesc->RawAddr);
	BufSize = pDesc->RawFileSize;

	hFile = FileSys_OpenFile(path, FST_OPEN_READ);
	if (FileSys_ReadFile(hFile, (UINT8 *)pDesc->RawAddr, &BufSize, 0, NULL) != 0) {
		pDesc->Parent.bOK = FALSE;
		FileSys_CloseFile(hFile);
		return;
	}
	FileSys_CloseFile(hFile);

	//Delete Current File After Read To Memory
	if (FileSys_DeleteFile(path) != FST_STA_OK) {
		DBG_IND("Ignore deleting file.\r\n");
	}

	pDesc->Parent.bOK = TRUE;
}

#else

BOOL MsdcNvtRegBi_Photo(void)
{
	return TRUE;
}

#endif
