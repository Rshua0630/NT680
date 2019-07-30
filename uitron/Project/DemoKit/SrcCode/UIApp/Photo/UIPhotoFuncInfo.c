#include "SysCommon.h"
#include "GxImage.h"
#include "UIInfo.h"
#include "nvtmpp.h"
#include "dma.h"
#include "ImageUnit_ImagePipe.h"
#include "UIPhotoFuncInfo.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIPhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"



static ISF_IMAGEPIPE_EVENT_CB Photo_EventCB = {
	.ISR_CB_FP[IPL_ISR_SIE1] = Photo_ISR_EventSIE,
	.ISR_CB_FP[IPL_ISR_SIE2] = NULL,
#if defined(_BSP_NA51000_)
	.ISR_CB_FP[IPL_ISR_SIE3] = NULL,
	.ISR_CB_FP[IPL_ISR_SIE4] = NULL,
	.ISR_CB_FP[IPL_ISR_SIE5] = NULL,
	.ISR_CB_FP[IPL_ISR_SIE6] = NULL,
	.ISR_CB_FP[IPL_ISR_SIE7] = NULL,
	.ISR_CB_FP[IPL_ISR_SIE8] = NULL,
#endif
	.ISR_CB_FP[IPL_ISR_IFE] = NULL,
	.ISR_CB_FP[IPL_ISR_DCE] = NULL,
	.ISR_CB_FP[IPL_ISR_IPE] = Photo_ISR_EventIPE,
	.ISR_CB_FP[IPL_ISR_IME] = Photo_ISR_EventIME,
	.ISR_CB_FP[IPL_ISR_IFE2] = NULL,
	.ISR_CB_FP[IPL_ISR_RHE] = NULL
};


PHOTO_FUNC_INFO PhotoFuncInfo_dummy = {
	{
		PHOTO_PRIORITY_H,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_SIE_VD,       ///< isr tigger event
		"dummy",
		NULL,                   ///< init fp, only execute at first process
		NULL,                   ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

UINT32 Photo_getDualCamSetting(void)
{
	UINT32           myDualCam = DUALCAM_FRONT;

#if (SENSOR_CAPS_COUNT == 1)
	if (System_GetEnableSensor() == (SENSOR_1)) {
		myDualCam = DUALCAM_FRONT;
	} else {
		myDualCam = DUALCAM_BEHIND;
	}
#endif
#if (SENSOR_CAPS_COUNT == 2)
	if (System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2)) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			myDualCam = DUALCAM_FRONT;
		} else {
			myDualCam = DUALCAM_BEHIND;
		}
	} else {
		myDualCam = UI_GetData(FL_DUAL_CAM);
	}
#endif
	return myDualCam;
}

static void _PhotoFunc_CB(PHOTO_CB_EVENT event, CHAR *func_name, void *in_info, void *out_info)
{
	NVTMPP_VB_POOL pool;
	NVTMPP_VB_BLK  blk;
	UINT32         blk_size = *(UINT32 *)in_info, addr;
	switch (event) {

	case PHOTO_CB_GET_BUF:
		pool = nvtmpp_vb_create_pool(func_name, blk_size, 1, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_POOL == pool) {
			DBG_ERR("%s create private pool err\r\n", func_name);
			return;
		}
		blk = nvtmpp_vb_get_block(0, pool, blk_size, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_BLK == blk) {
			DBG_ERR("get vb block err\r\n");
			break;
		}
		addr = nvtmpp_vb_block2addr(blk);
		*(UINT32 *)out_info = addr;
		DBG_IND("%s get noncache buf addr = 0x%08X, size=0x%08X\r\n", func_name, addr, blk_size);
		break;
	case PHOTO_CB_GET_CACHE_BUF:
		pool = nvtmpp_vb_create_pool(func_name, blk_size, 1, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_POOL == pool) {
			DBG_ERR("%s create private pool err\r\n", func_name);
			return;
		}
		blk = nvtmpp_vb_get_block(0, pool, blk_size, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_BLK == blk) {
			DBG_ERR("get vb block err\r\n");
			break;
		}
		addr = nvtmpp_vb_block2addr(blk);
		*(UINT32 *)out_info = dma_getCacheAddr(addr);
		DBG_IND("%s get cache buf addr = 0x%08X, size=0x%08X\r\n", func_name, addr, blk_size);
		break;
	case PHOTO_CB_REL_BUF:
	case PHOTO_CB_REL_CACHE_BUF:
        addr = *(UINT32 *)out_info;
        DBG_IND("%s release buf addr = 0x%08X, size=0x%08X\r\n", func_name, addr, blk_size);
		pool = nvtmpp_vb_addr2pool(addr);
		if (pool == NVTMPP_VB_INVALID_POOL){
			DBG_ERR("invalid block addr 0x%08X\r\n",addr);
		}
		else {
			if (nvtmpp_vb_destroy_pool(pool) != NVTMPP_ER_OK)
				DBG_ERR("free pool\r\n");
		}
        break;
	default:
		break;
	}
}

void UIPhotoFunc_Open(PHOTO_FUNC_INFO *phead)
{
	ImageUnit_Begin(ISF_IPL(0), 0);
	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_EVENT_CB, (UINT32)&Photo_EventCB);
	ImageUnit_End();
#if (SENSOR_CAPS_COUNT > 1)
	ImageUnit_Begin(ISF_IPL(1), 0);
	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_EVENT_CB, (UINT32)&Photo_EventCB);
	ImageUnit_End();
#endif
#if (SENSOR_CAPS_COUNT > 2)
	ImageUnit_Begin(ISF_IPL(2), 0);
	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_EVENT_CB, (UINT32)&Photo_EventCB);
	ImageUnit_End();
#endif
#if (SENSOR_CAPS_COUNT > 3)
	ImageUnit_Begin(ISF_IPL(3), 0);
	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_EVENT_CB, (UINT32)&Photo_EventCB);
	ImageUnit_End();
#endif
	if (Photo_Open(phead, _PhotoFunc_CB) != E_OK) {
		DBG_ERR("\r\n");
	}
}

void UIPhotoFunc_Close(void)
{
	if (Photo_Close() != E_OK) {
		DBG_ERR("\r\n");
	}
}

