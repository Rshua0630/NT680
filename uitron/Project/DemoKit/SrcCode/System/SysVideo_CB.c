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
#define __MODULE__          SysVideoExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#include "SysKer.h"
#include "Debug.h"
#include "NvtUser.h"
#include "SysCommon.h"

//#include "NvtSystem.h"
#include "GxVideo.h"
#include "GxSystem.h"
#include "AppControl.h"
#include "ImageUnit_VdoOut.h"

/////////////////////////////////////////////////////////////////////////////
// DISPLAY
#include "GxDisplay.h"
#include "alg_VdoOut.h"
#include "nvtmpp.h"


extern DX_HANDLE InsertDevObj;

UINT32 g_Disp_Device1 = 0;
UINT32 g_Disp_Device2 = 0;

void Video_CB(UINT32 event, UINT32 param1, UINT32 param2);

void Video_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	switch (event) {
// part-1
///////////////////////////////////////////////////////
// part-3
	case DISPLAY_CB_PLUG:
		//This CB will be trigger when TV or HDMI plug
		DBG_IND("DISPLAY_CB_PLUG\r\n");
		{
			Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, param1);
		}
		break;
	case DISPLAY_CB_UNPLUG:
		//This CB will be trigger when TV or HDMI unplug
		DBG_IND("DISPLAY_CB_UNPLUG\r\n");
		{
			Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, param1);
		}
		break;
	case DISPLAY_CB_DIR:
		//This CB will be trigger when LCD rotate
		DBG_IND("DISPLAY_CB_DIR\r\n");
		{
			Ux_PostEvent(NVTEVT_VIDEO_DIR, 2, param1, param2);
		}
		break;
	}
}

extern BOOL bEnableAE;
extern BOOL bStableAE;
BOOL bEnableVdo = FALSE;
BOOL bShowPreview = FALSE;

#if (VDO_USE_COPY_BUFFER == ENABLE)
static BOOL g_is_skip_alloc_temp = FALSE;

static ISF_UNIT *ISF_VdoOutList[2] = {
	&ISF_VdoOut1,
	&ISF_VdoOut2,
};

static UINT32 g_tempbuf[2] = {
	0,
	0,
};



static ISF_RV temp_lock(UINT64 module, UINT32 hData){return ISF_OK;}
static ISF_RV temp_unlock(UINT64 module, UINT32 hData){return ISF_OK;}


void VdoOut_SetSkipAllocTemp(UINT32 is_alloc)
{
	g_is_skip_alloc_temp = is_alloc;
}


static int VdoOut_AllocTempBuf(UINT32 *buf_addr, UINT32 need_buf_size)
{
	NVTMPP_VB_POOL vb_temp_pool;
	NVTMPP_VB_BLK  blk;
	UINT32         blk_size;

	blk_size = need_buf_size;
	vb_temp_pool = nvtmpp_vb_create_pool(NVTMPP_TEMP_POOL_NAME, blk_size, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == vb_temp_pool) {
		DBG_ERR("create pool err\r\n");
		return -1;
	}
	blk = nvtmpp_vb_get_block(0, vb_temp_pool, blk_size, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_BLK == blk) {
		DBG_ERR("get vb block err\r\n");
		if (nvtmpp_vb_destroy_pool(vb_temp_pool) != NVTMPP_ER_OK)
			DBG_ERR("free pool\r\n");
		return -1;
	}
	*buf_addr = nvtmpp_vb_block2addr(blk);
	DBG_IND("temp buffer =0x%x\r\n",*buf_addr);
	return 0;
}

UINT32 VdoOut_NewTempData(UINT32 id, ISF_DATA* pData)
{
	UINT32 w = 0,h = 0,fmt = 0;
	UINT32 bufsize, bufaddr = 0;

	if (InsertDevObj != 0) {
		DBG_DUMP("Change Display\r\n");
		return 0;
	}
	//query image size, format
	ImageUnit_GetVdoImgSize(ISF_VdoOutList[id], ISF_IN1, &w, &h);
	ImageUnit_GetVdoImgFormat(ISF_VdoOutList[id], ISF_IN1, &fmt);
	//calculate buffer size
	DBG_DUMP("VdoOut%d New blk size==0, w %d, h %d\r\n", id,w, h);
	bufsize = GxImg_CalcRequireSize(w, h, fmt, GXIMAGE_LINEOFFSET_ALIGN(16));
	if(bufsize == 0) {
		DBG_ERR("VdoOut%d New blk size==0, invalid fmt?, FAILED!\r\n", id+1);
		return 0;
	}
	//new buffer
	#if 1
	if (!g_is_skip_alloc_temp){
		VdoOut_AllocTempBuf(&bufaddr,bufsize);
		g_tempbuf[id] = bufaddr;
	}
	else{
		bufaddr= g_tempbuf[id];
	}
	pData->Sign = ISF_SIGN_DATA;
	pData->pLockCB = temp_lock; //fake lock
	pData->pUnlockCB = temp_unlock; //fake unlock
	pData->hData = bufaddr; //fill addr if ok
	#else
	UINT32 maxbufsize = OS_GetMempoolSize(POOL_ID_DISP_VDO1);
	if(bufsize <= maxbufsize) {
		bufaddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
		pData->Sign = ISF_SIGN_DATA;
		pData->pLockCB = temp_lock; //fake lock
		pData->pUnlockCB = temp_unlock; //fake unlock
		pData->hData = bufaddr; //fill addr if ok
	} else {
		pData->hData = 0; //fill 0 if fail
	}
	if(bufaddr) {
		DBG_MSG("^YVdoOut%d New blk size=%d, OK! addr=%08x\r\n", id+1, bufsize, bufaddr);
	} else {
		DBG_ERR("VdoOut%d New blk size=%d > max=%d, FAILED!\r\n", id+1, bufsize, maxbufsize);
	}
	#endif

	if(bufaddr) {
		ISF_VIDEO_RAW_BUF* pImg = (ISF_VIDEO_RAW_BUF *)(pData->Desc);
		GxImg_InitBuf(pImg, w, h, fmt, GXIMAGE_LINEOFFSET_ALIGN(16), bufaddr, bufsize);
	}
	return bufaddr;
}
#endif

void VdoOut_CB(UINT32 event, UINT32 param1, UINT32 param2);

void VdoOut_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	#if 0 //(__DBGLVL__ >= 6)
	ISF_VIDEO_RAW_BUF *pImg;
	#endif
	switch (event) {
	case VDOOUT_EVENT_NEW:    //param1 = device id, param2 = (ISF_DATA *)
		//This CB will be trigger before close of VdoOut
		{
			DBG_MSG("^YVdoOut%d New!\r\n", param1 + 1);
			#if (VDO_USE_COPY_BUFFER == ENABLE)
			VdoOut_NewTempData(param1, (ISF_DATA *)(param2));
			//nvtmpp_dump_mem_range(debug_msg);
			#endif

		}
		break;
	case VDOOUT_EVENT_PUSH:    //param1 = device id, param2 = (ISF_DATA *)
		//This CB will be trigger before change config of VdoOut
		{
			ISF_VIDEO_RAW_BUF *pImg = (ISF_VIDEO_RAW_BUF *)(((ISF_DATA *)param2)->Desc);
			DBG_MSG("^YVdoOut%d Push! -- Vdo: %dx%d %d %08x %08x %d %d\r\n", param1 + 1, pImg->Width, pImg->Height, pImg->PxlFmt, pImg->PxlAddr[0], pImg->PxlAddr[1], pImg->LineOffset[0], pImg->LineOffset[1]);
			alg_VdoOut_Draw(pImg);
		}
		break;
	case VDOOUT_EVENT_VSYNC: //param1 = device id
		//This CB will be trigger after change config of VdoOut (every time)
		//DBG_MSG("^YVdoOut%d Sync!\r\n", param1 + 1);
		break;
	case VDOOUT_EVENT_VSYNC_FIRST: //param1 = device id
		//This CB will be trigger after change config of VdoOut (first time only)
		DBG_MSG("^YVdoOut%d Sync First!\r\n", param1 + 1);
		bEnableVdo = TRUE;
		if(!bEnableAE) {
			if((DX_HANDLE)GxVideo_GetDevice(DOUT1) && !bShowPreview) {
				bShowPreview = TRUE;
				Display_ShowPreview(); //show preview immediately (VdoOut should be last one)
				DBG_DUMP("^YShow Preview! (IPL > AE off > Video VSync)\r\n");
			}
		} else if(bEnableAE && bStableAE) {
			if((DX_HANDLE)GxVideo_GetDevice(DOUT1) && !bShowPreview) {
				bShowPreview = TRUE;
				Display_ShowPreview(); //show preview immediately (VdoOut should be last one)
				DBG_DUMP("^YShow Preview! (IPL > AE stable > Video VSync)\r\n");
			}
		}
#if (DISPLAY_FAST_SWITCH == ENABLE)
		Ux_SendEvent(0, NVTEVT_VIDEO_ATTACH, 1, 0); //end of video attach
		//DBGD(Perf_GetCurrent());
#endif
		break;
	case VDOOUT_EVENT_RELEASE:    //param1 = device id, param2 = (ISF_DATA *)
		//This CB will be trigger after change config of VdoOut
		#if 0 //(__DBGLVL__ >= 6)
		pImg = (ISF_VIDEO_RAW_BUF *)(param2);
		DBG_MSG("^YVdoOut%d Release! -- Vdo: %dx%d %d %08x %08x %d %d\r\n", param1 + 1, pImg->Width, pImg->Height, pImg->PxlFmt, pImg->PxlAddr[0], pImg->PxlAddr[1], pImg->LineOffset[0], pImg->LineOffset[1]);
		#endif
		break;
	}
}

