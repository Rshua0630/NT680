#include "SysCommon.h"
#include "GxImage.h"
#include "UIInfo.h"
#include "NvtUctrlAPI.h"
#include "md_api.h"
#include "UIPhotoFuncInfo.h"
#include "UIAppIPCam.h"
#include "ImageUnit_ImagePipe.h"
#include "UIAppIPCam_Param.h"
#include "riff.h"

#if (IPCAM_DEPTH_FUNC == ENABLE)
#include "NvtDepth.h"

#endif

#if IPCAM_FD_FUNC
#include "fd_lib.h"
#define IPCAM_FD_FACESCALE	15
//#include "UIPhotoInfo.h"
#endif

//#if IPCAM_IN_DIS_FUNC || MOVIE_ODT_FUNC
#if IPCAM_DIS_MV_FUNC
#include "af_api.h"
#include "ipl_header_infor_raw.h"
#include "dis_alg.h"
#include "ipl_cmd.h"
#include "dis_api.h"
#endif

//#NT#2017/04/28#KCHong -begin
//#NT# CNN Object detection
#if (IPCAM_OD_FUNC == ENABLE)
#include "NvtNNFlow.h"
//#include "NvtCNN.h"
#include "Perf.h"
#include "GxImage.h"
#include "FileSysTsk.h"
#include "Type.h"
//static DX_HANDLE gDevHDMIObj = 0;
//static UINT32 g_hdmi_mode = 0;
#endif
//#NT#2017/04/28#KCHong -end


//#NT#2018/05/18#Dan Guo -begin
//#NT# Face recognition
#if (MOVIE_FR_FUNC == ENABLE)
#include "FR_lib.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "HwClock.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2018/05/18#Dan Guo -end



//#NT#2018/05/04#Bin Xiao -begin
//#NT# Face Detection by CNN
#if (MOVIE_FDCNN_FUNC == ENABLE)
#include "fdcnn_lib.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "HwClock.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2018/05/04#Bin Xiao -end

//#NT#2018/05/04#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#if (MOVIE_ODT_FUNC == ENABLE)
#include "odt_lib.h"
#include "ImageUnit_ImagePipe.h"
#include "GxImage.h"
#include "HwClock.h"
#include "UIInfo.h"
#include "displyr.h"
#include "display.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxDisplay.h"
#endif
//#NT#2018/05/04#Bin Xiao -end


#if (IPCAM_CDNN_FUNC || IPCAM_CDNN2_FUNC)
#include "alg_CDNN_PhotoFuncInfo.h"
#include "NvtCDNNDev.h"
#define CDNN_DEV    DISABLE
#endif

#if IPCAM_BC_FUNC
#include "bc_lib.h"
static BOOL gbBCStartFlag = FALSE;
#define IPCAM_BC_USE_ODT	DISABLE
#endif

//#NT#2018/03/19#Wayne Wei -begin
//#NT#Support License Plate Recognition function
#if IPCAM_LPR_FUNC
#include "ImageUnit_ImagePipe.h"
#include "FileSysTsk.h"
#include "LPR_API.h"
#endif
//#NT#2018/01/23#Wayne Wei -end

#if IPCAM_FUNC
#include "vqa_lib.h"
#endif

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppIPCam_PhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

static BOOL gMDStartFlag = FALSE;

typedef struct {
    PIMG_BUF                p_src_img[4];
	PIMG_BUF                p_dst_img;
    UINT32                  viewcnt;
} IPCAM_VIEW_DRAW;


//#NT#2017/08/08#Charlie Chang -begin
//#NT# support IPcam face detection
#if IPCAM_FD_FUNC
FD_IMAGERATIO IPCam_FDRatio(UINT32 w, UINT32 h){

    if(w==4 && h==3){
		return FD_IMAGERATIO_4_3;
	}
	else if(w==3 && h==2){
        return FD_IMAGERATIO_3_2;
	}
	else if(w==16 && h==9){
         return FD_IMAGERATIO_16_9;
	}
	else{
		DBG_ERR("w=%d h=%d not support!!\r\n",w,h);
	}

	return FD_IMAGERATIO_4_3;
}

void IPCam_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	FD_Config(FD_CFG_SCALE_ENG, FD_SCALE_ENG1);
	FD_Init(buf, cachebuf, MAX_FDNUM, IPCAM_FD_FACESCALE, 3);
}

void IPCam_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_SENSOR_INFO *pSensorInfo = UIAppIPCam_get_SensorInfo();
	UINT32 uiSensorIndex = pStreamInfo[15].sensor;
	UINT32 width=0;
	UINT32 height=0;
	IMG_BUF *pBuf=NULL;
	UINT32 faceNumber=0;


	double now_time = 0;
	static INT32 start_index = -1;
	static double old_time = 0;


	static UINT64 uiFDLastFrameTime =0;
	UINT32 LastTimeStamp_sec = 0;
	UINT32 LastTimeStamp_usec = 0;
	UINT32 TimeStamp_sec=0;
	UINT32 TimeStamp_usec =0;
	INT32 DiffTimeStamp_sec =0 ;
	INT32 DiffTimeStamp_usec = 0;
	static UINT64 uiFDDiffFrameTimeUS =0;
	UINT32 uiFDTimeOutUS = 33333;


	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		now_time = clock();
		if (now_time - old_time <= 2000000) { ///2secs
			return;
		}
		start_index++;
		DBG_DUMP("FD detection start!!\r\n");
	}

	pUnit = &ISF_UserProc;

	////path 15 is for ALG

	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
		//ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_BYPASS_IMM, 0);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

	if(!ImageUnit_IsAllowPull(pSrc)){
		DBG_WRN("ImageUnit_IsAllowPull fail!\r\n");
		return;
	}

	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		//#NT#2017/10/26#Charlie chang -begin
    	//#NT# flow control
		if (uiFDLastFrameTime == 0){
			uiFDLastFrameTime = isf_data.TimeStamp;
		}

		TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
		LastTimeStamp_sec = (uiFDLastFrameTime >> 32) & 0xFFFFFFFF;
		LastTimeStamp_usec = uiFDLastFrameTime & 0xFFFFFFFF;
		DiffTimeStamp_sec = (long)TimeStamp_sec - (long)LastTimeStamp_sec;
		DiffTimeStamp_usec = (long)TimeStamp_usec - (long)LastTimeStamp_usec;
		uiFDDiffFrameTimeUS = (UINT64)DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec + uiFDDiffFrameTimeUS;


		if (uiFDDiffFrameTimeUS > uiFDTimeOutUS) {

			pBuf =  (IMG_BUF *)&isf_data.Desc[0];
			width = pBuf->Width;
			height = pBuf->Height;
			FD_Process((IMG_BUF *)&isf_data.Desc[0], IPCam_FDRatio(pSensorInfo[uiSensorIndex].sRatio.w,pSensorInfo[uiSensorIndex].sRatio.h));
			uiFDDiffFrameTimeUS -= uiFDTimeOutUS;
		}
		uiFDLastFrameTime = isf_data.TimeStamp;
		//#NT#2017/10/26#Charlie Chang -end
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

	BOOL bUpdated = FD_GetRsltFlag();

	if(bUpdated == FALSE){
		return;
	}
	FD_FACE          FdInfo[FD_NUM_MAX]={0};
	URECT            FdDispCoord = {0, 0, 640, 480};
	IPCAM_FD_INFO    IPCamFdInfo={0};

	FdDispCoord.x = 0;
	FdDispCoord.y = 0;
	FdDispCoord.w = width;
	FdDispCoord.h = height;

	faceNumber = FD_GetRsltFace(FdInfo, &FdDispCoord);

	IPCamFdInfo.Event =IPCAM_EVENT_FD;
	IPCamFdInfo.uiFaceNumber = faceNumber;
	#if 1


	UINT32 i =0;
	//DBG_DUMP("facenumber=%d \r\n",IPCamFdInfo.uiFaceNumber);
	for (i = 0; i < faceNumber; i++) {
		IPCamFdInfo.info[i].x = FdInfo[i].x;
		IPCamFdInfo.info[i].y = FdInfo[i].y;
		IPCamFdInfo.info[i].w = FdInfo[i].w;
		IPCamFdInfo.info[i].h = FdInfo[i].h;
		#if 0
		DBG_DUMP("x:%d y:%d w:%d h:%d\r\n",
			IPCamFdInfo.info[i].x,
			IPCamFdInfo.info[i].y,
			IPCamFdInfo.info[i].w,
			IPCamFdInfo.info[i].h
		);
		#endif

	}
	#endif

	NvtUctrl_NotifyRaw((void *)&IPCamFdInfo, sizeof(IPCamFdInfo));

}

UINT32 IPCam_FDCalcBuffSize(void)
{
	FD_Config(FD_CFG_FACE_SCALE, IPCAM_FD_FACESCALE);
	return FD_CalcBuffSize();
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamfd = {
	{
		PHOTO_PRIORITY_M,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMFDProcess",
		IPCam_FDInit,              ///< init fp, only execute at first process
		IPCam_FDProcess,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		IPCam_FDCalcBuffSize,      ///< get working buffer fp
		FD_CalcCacheBuffSize,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2017/08/08#Charlie Chang -end

void Movie_MDLock(BOOL enable)
{

	gMDStartFlag = enable;
}

struct IPCAM_MOTION_INFO {

	BOOL set_no_trigger;


};

void EVENT_motion_detection_init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	struct IPCAM_MOTION_INFO *motion_info = (struct IPCAM_MOTION_INFO *)buf;
	motion_info->set_no_trigger = 0;

}
INT32 EVENT_motion_size(void)
{
	return sizeof(struct IPCAM_MOTION_INFO);

}
void EVENT_motion_detection(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if _TODO
	INT32 id = 0;
	BOOL trigger = FALSE;
	double now_time = 0;
	double time_out = 1000000;
	static INT32 start_index = -1;
	static double old_time = 0;

	struct IPCAM_MOTION_INFO *motion_info = (struct IPCAM_MOTION_INFO *)buf;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		now_time = clock();
		if (now_time - old_time <= 10000000) { ///10secs
			return;
		}

		start_index ++;
		DBG_DUMP("Motion detection start!!\r\n");
	}
	//#NT#2016/11/01#Charlie Chang -begin
	//#NT# start motion detection if flag enable
	if (gMDStartFlag == FALSE) {
		if (motion_info->set_no_trigger == 1) {
			DBG_DUMP("motion stop trigger\r\n");
			NvtUctrl_Notify(MOTION1_NOTRIGGER);
			motion_info->set_no_trigger = 0;
		}
		return;
	}
	//#NT#2016/11/01#Charlie Chang -end
	trigger = MD_Process(id);
	now_time = clock();
	if (trigger == TRUE) {

		if (motion_info->set_no_trigger == 1) {
			DBG_IND("motion still trigger\r\n");
		} else {

			DBG_DUMP("motion trigger\r\n");
			NvtUctrl_Notify(MOTION1_TRIGGER);
			motion_info->set_no_trigger = 1;

		}
		old_time = now_time;

	} else {
		if (((now_time - old_time) > time_out) && (motion_info->set_no_trigger == 1)) {

			DBG_DUMP("motion stop trigger\r\n");
			NvtUctrl_Notify(MOTION1_NOTRIGGER);
			motion_info->set_no_trigger = 0;

		}
	}
#endif

	return;
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_motion = {
	{
		PHOTO_PRIORITY_M,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"EVENT_motion_detection",
		EVENT_motion_detection_init,   /// < init fp, only execute at first process
		EVENT_motion_detection,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		NULL,                   ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL                    ///< point next Function Obj, last Function must be set NULL
};
//#NT#2017/08/09#Charlie Chang -begin//#NT# support IPCam DIS#if IPCAM_DISvoid IPCam_DIS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf){	dis_initialize(buf, cachebuf);	dis_setDisViewRatio(900);}void IPCam_DIS(MEM_RANGE *buf, MEM_RANGE *cachebuf){	IPL_SIE_CUR_FRAME_INFO Info = {0};	DISALG_VECTOR DIS_Offset = {0};	HEADER_RAW_DATA RawHeaderInfo = {0};    ISF_UNIT *pSensorList[IPL_ID_MAX_NUM] = {		&ISF_ImagePipe1, &ISF_ImagePipe2,		&ISF_ImagePipe3, &ISF_ImagePipe4,		&ISF_ImagePipe5, &ISF_ImagePipe6,		&ISF_ImagePipe7, &ISF_ImagePipe8	};	Info.id = IPL_PATH_1;	IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &Info);	if (Info.frame_cnt < 2) {		return;	}	//#NT#2016/11/15#Lincy Lin -begin	//#NT#Work around for FTG with DIS will have some error.	#if (MOVIE_FTG_FUNC == ENABLE)		IPL_HeaderRawClrChkStatus(IPL_PATH_1, (HEADER_RAW_STS_IS_OFS));	#else		if (ImageUnit_GetParam(pSensorList[IPL_PATH_1], ISF_CTRL, IPL_SEL_DISCOMP_IMM) == SEL_DISCOMP_ON) {			IPL_HeaderRawSetChkStatus(IPL_PATH_1, (HEADER_RAW_STS_IS_OFS));		} else {			IPL_HeaderRawClrChkStatus(IPL_PATH_1, (HEADER_RAW_STS_IS_OFS));		}	#endif	//#NT#2016/11/15#Lincy Lin -end	if (IPL_HeaderRawGetCurInfo_Dly1VD(IPL_PATH_1, &RawHeaderInfo) != E_OK) {		DBG_ERR("DIS: Get header error!!!\r\n");	}	if (ImageUnit_GetParam(pSensorList[IPL_PATH_1], ISF_CTRL, IPL_SEL_DISCOMP_IMM) == SEL_DISCOMP_ON) {		dis_process(buf, cachebuf);		dis_getFrameCorrVecPxl(&DIS_Offset, RawHeaderInfo.frame_cnt);		if (DIS_Offset.vector.iX % 2) {			DIS_Offset.vector.iX -= 1;		}		if (DIS_Offset.vector.iY % 2) {			DIS_Offset.vector.iY -= 1;		}        //DBG_ERR("cnt=%d,(%d,%d)%d %d\r\n",Info.frame_cnt,DIS_Offset.vector.iX,DIS_Offset.vector.iY,DIS_Offset.frame_cnt,RawHeaderInfo.frame_cnt);	}	else {		DIS_Offset.vector.iX = 0;		DIS_Offset.vector.iY = 0;	}	UINT32    rotate_dir;	ImageUnit_GetVdoDirection(pSensorList[IPL_PATH_1], ISF_IN1, &rotate_dir);	switch (rotate_dir) {	case ISF_VDO_DIR_MIRRORX:		DIS_Offset.vector.iX = 0 - DIS_Offset.vector.iX;		break;	case ISF_VDO_DIR_MIRRORY:		DIS_Offset.vector.iY = 0 - DIS_Offset.vector.iY;		break;	case (ISF_VDO_DIR_MIRRORX|ISF_VDO_DIR_MIRRORY):		DIS_Offset.vector.iX = 0 - DIS_Offset.vector.iX;		DIS_Offset.vector.iY = 0 - DIS_Offset.vector.iY;		break;	default:		break;	}	//#NT#2016/08/26#Lincy Lin -begin	//#NT# Add uctrl command for BC/MV/IPL buffer APIs .	if (ImageUnit_GetParam(pSensorList[IPL_PATH_1], ISF_CTRL, IPL_SEL_DISCOMP_IMM) == SEL_DISCOMP_ON)// && UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_ON)		//#NT#2016/08/26#Lincy Lin -end	{		IPL_HeaderRawSetInfo_Dly1VD(IPL_PATH_1, IPL_HEADER_RAW_IS_OFS, (void *)&DIS_Offset.vector);	}//#NT#2016/10/17#Bin Xiao -begin//#NT# Save DIS motion vecter result to FTG buffers.#if (MOVIE_FTG_FUNC == ENABLE)	#if 0	static UINT32 ftg_num1 = 0;	static UINT32 ftg_time1 = 0;	static UINT32 ftg_max1 = 0;	UINT32 t1 = Perf_GetCurrent();	ftg_savedisrslt();	UINT32 t2 = Perf_GetCurrent();	if (ftg_num1 < 1500 && ftg_num1 >= 1000) {		if (ftg_max1 < t2 - t1) {			ftg_max1 = t2 - t1;		}		ftg_time1 += t2 - t1;		ftg_num1++;	} else if (ftg_num1 >= 1000) {		DBG_ERR("500 ftg_savedisrslt uses %d us...max %d us\n\r", ftg_time1, ftg_max1);	} else {		ftg_num1++;	}	#else	if (!gFTGLock) {		ftg_savedisrslt();	}	#endif#endif//#NT#2016/10/17#Bin Xiao -end}PHOTO_FUNC_INFO PhotoFuncInfo_ipcamdis = {	{		PHOTO_PRIORITY_H,             ///< function hook process Id.		IPL_PATH_1,               ///< IPL Id.		//#NT#2016/05/04#Harry_Tsai -begin		//#NT# Change to VD		PHOTO_ISR_SIE_VD,       ///< isr tigger event		"IPCam_DIS",		//#NT#2016/05/04#Harry_Tsai -end		IPCam_DIS_Init,                   ///< init fp, only execute at first process		IPCam_DIS,           	///< process fp		NULL,                   ///< process end fp		dis_GetPrvMaxBuffer,        ///< get working buffer fp		NULL,                   ///< get working cache buffer fp	},	NULL  ///< point next Function Obj, last Function must be set NULL};#elsePHOTO_FUNC_INFO PhotoFuncInfo_IPCamdis = {0};#endif
//#NT#2017/08/09#Charlie Chang -begin
//#NT# support IPCam DIS
//#if IPCAM_IN_DIS_FUNC || MOVIE_ODT_FUNC
//#if 1
#if IPCAM_DIS_MV_FUNC

//#NT#2018/05/29#Charlie Chang -begin
//#NT# add SEMPHORE for dis
UINT32 DIS_SEM_ID = 0;
//#NT#2018/05/29#Charlie Chang -end

void IPCam_DIS_InstallID(void){
	OS_CONFIG_SEMPHORE(DIS_SEM_ID, 0, 1, 1);
}

void IPCam_DIS_WaitSem(void){

	wai_sem(DIS_SEM_ID);
}

void IPCam_DIS_SigSem(void){

	sig_sem(DIS_SEM_ID);
}

void IPCam_DIS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	dis_initialize(buf, cachebuf);
	//dis_setDisViewRatio(900); --> It should be set befor auto-gen dzoom table
    DIS_InstallCmd();

}

//#NT#2017/10/02#Harry_Tsai -begin
//#NT rearrange DIS structure
void IPCam_DIS(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	IPCam_DIS_WaitSem();
	
#if (IPCAM_DIS_COMP_FUNC == ENABLE)
	dis_compensation(buf,cachebuf,FALSE);
#else
    dis_compensation(buf,cachebuf,TRUE);
#endif

	
#if (MOVIE_FTG_FUNC == ENABLE)
    //dis_compensation(buf,cachebuf,TRUE);
    if (!gFTGLock) {
        ftg_savedisrslt();
    }
#elif (MOVIE_ODT_FUNC == ENABLE)
	//dis_compensation(buf,cachebuf,TRUE);
	odt_savedisrslt();
#else
    //dis_compensation(buf,cachebuf,FALSE);
#endif
	IPCam_DIS_SigSem();
}
//#NT#2017/10/02#Harry_Tsai -end

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamdis = {
	{
		PHOTO_PRIORITY_H,             ///< function hook process Id.
		IPL_PATH_1,               ///< IPL Id.
		//#NT#2016/05/04#Harry_Tsai -begin
		//#NT# Change to VD
		PHOTO_ISR_SIE_VD,       ///< isr tigger event
		"IPCam_DIS",
		//#NT#2016/05/04#Harry_Tsai -end
		IPCam_DIS_Init,                   ///< init fp, only execute at first process
		IPCam_DIS,           	///< process fp
		NULL,                   ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		dis_GetPrvMaxBuffer,        ///< get working buffer fp
		NULL,                   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#else
PHOTO_FUNC_INFO PhotoFuncInfo_IPCamdis = {0};
#endif


//#NT#2017/04/28#KCHong -begin
//#NT# CNN Object detection
#define DEBUG_DUMP_IMG           DISABLE
#if (IPCAM_OD_FUNC == ENABLE)
//#define OD_DRAW_RESULT    ENABLE
//#define OD_PUSH_RESULT    ENABLE

//static BOOL bODStart = FALSE;
//#if (OD_PUSH_RESULT == ENABLE)
//static UINT32 tsOD = 0;
//#endif
void IPCam_ODInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	NN_NetSelect(2);
	NN_Init(buf, cachebuf, 1);
}

void IPCam_ODProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
#if (DEBUG_DUMP_IMG == ENABLE)
	static UINT32    frame_cnt = 0;
	char             filename[50];
#endif

	while(((INT32)FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0) == 0))
    {
        debug_msg("FileSys_Init not ready\r\n");
    }
    static UINT32 is_first_load_model = 1;
	if ((is_first_load_model == 1) && ((INT32)FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0) == 1)) {
        is_first_load_model = 0;
		NN_NetSelect(2);
		NN_LoadModel();
	}

	#if (MOVIE_OD_USE_ODT && IPCAM_OD_FUNC)
    odt_rslt_loc                    TimeStamp = {0};
	static INT32					regist_falg = ODT_STA_ERROR;
	static UINT32					od_rslt_loc;
	static UINT32					od_num;
	static NN_Result				od_rslts[MAX_DETECTED_OBJECT];
	static odt_od_result 			od_odt_rslts[MAX_DETECTED_OBJECT];

	static odt_process_buf_param 	od_odt_buf_param;
	static odt_rslt_inf				od_buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				od_buf2;
	static odt_od_result 			od_buf3[MAX_DETECTED_OBJECT];

	od_odt_buf_param.detect_rslt_addr = (UINT32)od_buf3;
	od_odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&od_buf2);
	od_odt_buf_param.odt_rslt_buffer_addr = (UINT32)od_buf1;

	if (regist_falg == ODT_STA_ERROR) {
		regist_falg = odt_regist(&od_odt_buf_param);
		if (regist_falg == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 15, od_odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 1, od_odt_buf_param.process_id);
			odt_config(odt_cfg_scale_for_match, 15, od_odt_buf_param.process_id);
		}
	}
	#endif



	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if (MOVIE_OD_USE_ODT && IPCAM_OD_FUNC)
        TimeStamp.timestamp_sec  = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		TimeStamp.timestamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
		od_rslt_loc = (UINT32)&TimeStamp;
        //od_rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
		#endif


		NN_Process((IMG_BUF *)&isf_data.Desc[0], 1);
#if (DEBUG_DUMP_IMG == ENABLE)
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
#endif
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

	#if (MOVIE_OD_USE_ODT && IPCAM_OD_FUNC)
	UINT32 i;
	URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
	od_num = NN_GetResult(od_rslts, &targetcoord);
	for(i = 0; i < od_num; i++)
	{
		od_odt_rslts[i].x = od_rslts[i].x;
		od_odt_rslts[i].y = od_rslts[i].y;
		od_odt_rslts[i].w = od_rslts[i].w;
		od_odt_rslts[i].h = od_rslts[i].h;
		od_odt_rslts[i].score = od_rslts[i].score;
		od_odt_rslts[i].class = od_rslts[i].class;
	}
    odt_setodrslt_timestamp(od_num, od_odt_rslts, od_rslt_loc, od_odt_buf_param.process_id);
    //odt_setodrslt(od_num, od_odt_rslts, od_rslt_loc, od_odt_buf_param.process_id);
	#endif
/*
	ImageUnit_Begin(pUnit, 0);
	////path 2 is for OD
	ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	if(bODStart == FALSE)
	{
		ImageUnit_Begin(&ISF_VdoEnc, 0);
		////set SmartIQ
		ImageUnit_SetParam(ISF_OUT1, VDOENC_PARAM_WAIT_SMART_ROI, TRUE);
		ImageUnit_End();
		bODStart = TRUE;
	}


    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
#if (OD_PUSH_RESULT == ENABLE)
		tsOD = isf_data.TimeStamp;
#endif
        OD_Process((IMG_BUF *)&isf_data.Desc[0]);
#if (DEBUG_DUMP_IMG == ENABLE)
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
#endif
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
	*/

}
/*
void IPCam_ODProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	UINT32 ODNumber = 0;
	OD_Result od_result[MAX_DETECTED_OBJECT] = {0};
    #if 0  /// For CNN demo!
	URECT DispCord = {0, 0, 960, 240};
    #else
    URECT DispCord = {0, 0, 1920, 1080};
    #endif
    DISPLAYER_PARAM   DispLyr = {0} ;

    if (!g_hdmi_mode) {
            gDevHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
            if (gDevHDMIObj) {
                g_hdmi_mode = Dx_Getcaps(gDevHDMIObj, DISPLAY_CAPS_VIDMODE, 0);
            }
    }

    switch(g_hdmi_mode)
    {
        case HDMI_640X480P60:
            DispCord.w = 640;
            DispCord.h = 480;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 7;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 7;
            break;
        case HDMI_3840X2160P30:
            DispCord.w = 960;
            DispCord.h = 240;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 4;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 4;
            break;
        case HDMI_1920X1080I60:
            DispCord.w = 1920;
            DispCord.h = 1080;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 10;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 10;
            break;
        default:
            DispCord.w = 640;
            DispCord.h = 480;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = 7;
            DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = 7;
            debug_msg("xExamOD_odgetresult Error: Unknown HDMI mode (%d)\r\n", g_hdmi_mode);
            break;
    }

	ODNumber = OD_GetResult(od_result, &DispCord);

#if (OD_PUSH_RESULT == ENABLE)
	{
		ISF_PORT *pDest= ImageUnit_In(&ISF_VdoEnc, ISF_IN1);
		if(ImageUnit_IsAllowPush(pDest)) {
			ISF_DATA isf_data = {0};
			ISF_DATA* pData = &isf_data;
			UINT32 addr = 0;
			//new data
			addr = ImageUnit_NewData(sizeof(RIFF_CHUNK)+sizeof(VIDENC_SMART_ROI_INFO), pData);
	    		pData->TimeStamp = tsOD;
			if(addr != 0) {
				//assign data to "metadata"
				ISF_METADATA_BUF *pMetadata = (ISF_METADATA_BUF *)(pData->Desc);
				pMetadata->flag = MAKEFOURCC('M','E','T','A');
				pMetadata->addr = addr;
				{
					//fill riff chunk header
					RIFF_CHUNK* pChunk = (RIFF_CHUNK*)pMetadata->addr;
					pChunk->fourcc = MAKEFOURCC('O','D','R','L');
					pChunk->size = sizeof(RIFF_CHUNK)+sizeof(VIDENC_SMART_ROI_INFO);
					//fill riff chunk data
					VIDENC_SMART_ROI_INFO* pInfo = (VIDENC_SMART_ROI_INFO*)(((UINT8*)pChunk)+(pChunk->size));
					pInfo->uiRoiNum = ODNumber;
					DBGD(ODNumber);
					pInfo->uiRange = 64;
					memcpy(pInfo->RoiInfo, &DispCord, sizeof(OD_Result)*MAX_DETECTED_OBJECT);
			    		pInfo->TimeStamp = tsOD;
				}
		    		//push data
				ImageUnit_PushData(pDest, pData, 0);
				//release data
				ImageUnit_ReleaseData(pData);
			}
		}
	}
#endif

#if (OD_DRAW_RESULT == ENABLE)
	UINT32            i = 0;
	PDISP_OBJ         pDispObj = NULL;

	idec_setFDAllEn(0, 1);
	pDispObj = disp_getDisplayObject(DISP_1);
	if (ODNumber > 16) {
		ODNumber = 16;
	}
	for (i = 0; i < 16; i++) {
		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = FALSE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}

	for (i = 0; i < ODNumber; i++) {
		//DBG_DUMP("Object %d = [x=%d,y=%d,w=%d,h=%d]\r\n",i,od_result[i].x,od_result[i].y,od_result[i].w,od_result[i].h);
		DispLyr.SEL.SET_FDSIZE.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDSIZE.uiFDX = od_result[i].x;
		DispLyr.SEL.SET_FDSIZE.uiFDY = od_result[i].y;
		DispLyr.SEL.SET_FDSIZE.uiFDW = od_result[i].w;
		DispLyr.SEL.SET_FDSIZE.uiFDH = od_result[i].h;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDSIZE, &DispLyr);

		DispLyr.SEL.SET_FDCOLOR.FD_NUM = DISPFD_NUM0 << i;

		// Blue   : 0x1d/0xff/0x6b
		// White  : 0xff/0x80/0x80
		// Red    : 0x4c/0x54/0xff
		// Yellow : 0xff/0x00/0x94
		switch (od_result[i].class) {
		case 0:     // Bus (blue)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0x1d;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x6b;
			break;
		case 1:     // Car (white)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x80;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x80;
			break;
		case 2:     // Human (red)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0x4c;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x54;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0xff;
			break;
		default:    // Others (yellow)
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xff;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x00;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x94;
			break;
		}
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDCOLOR, &DispLyr);

		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = TRUE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}
	pDispObj->load(TRUE);
#endif
}
*/
PHOTO_FUNC_INFO PhotoFuncInfo_ipcamod = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"IPCam_ODProcess",
		IPCam_ODInit,                       ///< init OD, only execute at first process
		IPCam_ODProcess,                    ///< process OD
		NULL,                 				///< process end OD
		NULL,                               ///< exit fp, only execute at photo close
		OD_CalcBuffSize,                    ///< get working buffer OD
		NN_CalcCacheBuffSize,               ///< get working cache buffer OD
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2017/04/28#KCHong -end

//#NT#2018/05/18#Dan Guo -begin
//#NT#Face recognition
#if (MOVIE_FR_FUNC == ENABLE)
//#if (MOVIE_FR_REG_FLOW == ENABLE)
MEM_RANGE g_FRInitBuf;
MEM_RANGE g_FRInitCacheBuf;
static BOOL g_bFRRegFlowEnable = FALSE;

static BOOL CmdFRReg_Flow(CHAR* strCmd)
{
	UINT32  bFRRegFlow = 0;
	sscanf_s(strCmd, "%d", &bFRRegFlow);
	if(bFRRegFlow) {
		g_bFRRegFlowEnable = TRUE;
		DBG_DUMP("FR reg flow start.\r\n");
		FR_reg_set(1);
	}else {

		//re-init FR
		FR_Init(&g_FRInitBuf, &g_FRInitCacheBuf, 1,1);
		g_bFRRegFlowEnable = FALSE;
		FR_reg_set(0);
		DBG_DUMP("FR reg flow stop.\r\n");

	}
	return TRUE;
}
static BOOL CmdFRReg_Cap(CHAR* strCmd)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	static char name[32] = {0};
    UINT32 register_status = 0;
	if(FALSE == g_bFRRegFlowEnable) {
		DBG_ERR("run \"fr regflow 1\" first!\r\n");
		return TRUE;
	}
	sscanf_s(strCmd, "%s", name, sizeof(name));
	if(0 == strlen(name)) {
		DBG_ERR("no name!\r\n");
		return TRUE;
	}
	DBG_DUMP("FR reg name = %s\r\n",name);
	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		#if 0//dump register YUV
		char filename[64];
		IMG_BUF *pImgBuf = (IMG_BUF *)&isf_data.Desc[0];
		snprintf(filename, sizeof(filename), "A:\\%s.yuv", name);
		if(GX_IMAGE_PIXEL_FMT_YUV422_PACKED == pImgBuf->PxlFmt) {
			DBG_DUMP("Save YUV422 to %s, %dx%d \r\n", filename,pImgBuf->Width,pImgBuf->Height);
		}else {
			DBG_DUMP("Save YUV420 to %s, %dx%d \r\n", filename,pImgBuf->Width,pImgBuf->Height);
		}
		GxImg_DumpBuf(filename, pImgBuf, GX_IMAGE_DUMP_ALL);
		#endif
		//FR_Register((IMG_BUF *)&isf_data.Desc[0], name);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);

		register_status = FR_Register((IMG_BUF *)&isf_data.Desc[0], name);
		if(register_status == 0)
		{
			DBG_DUMP("FR_Register_OK\r\n");
		}
		else if(register_status == 1)
		{
			DBG_DUMP("no face\r\n");
		}
		else if(register_status == 2)
		{
			DBG_DUMP("more than one face ...\r\n");
		}


	}
	return TRUE;
}
SXCMD_BEGIN(fr, "FR command")
SXCMD_ITEM("regflow %", CmdFRReg_Flow, "FR register flow enable/disable.")
SXCMD_ITEM("regcap %", CmdFRReg_Cap, "FR register capture.")
SXCMD_END()
//#endif

void Movie_FRInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//DBG_ERR("FR position 1 ###########\r\n");
   // debug_msg("####FR init\r\n");
    FR_Init(buf, cachebuf, 1,1);

//#if (MOVIE_FR_REG_FLOW == ENABLE)
		SxCmd_AddTable(fr);
		g_FRInitBuf.Addr = buf->Addr;
		g_FRInitBuf.Size = buf->Size;
		g_FRInitCacheBuf.Addr = cachebuf->Addr;
		g_FRInitCacheBuf.Size = cachebuf->Size;
		//DBG_DUMP("buf=0x%X,0x%X	cachebuf=0x%X,0x%X\r\n",g_FRInitBuf.Addr,g_FRInitBuf.Size,g_FRInitCacheBuf.Addr,g_FRInitCacheBuf.Size);
//#endif

}
void Movie_FRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

    ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;

//#if (MOVIE_FR_REG_FLOW == ENABLE)
	if(g_bFRRegFlowEnable) {
		return;
	}
//#endif

    static UINT32 is_first = 1;
	if ((is_first == 1) && ((INT32)FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0) == 1)) {
        is_first = 0;

	    fdcnn_netselect(FDCNN_METHOD_1);
		fdcnn_loadmodel();

		//FR_NetSelect(0);
     //   FR_LoadModel(1);
	}

	if(is_first == 1) {
		return;
	}

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		FR_recognition((IMG_BUF *)&isf_data.Desc[0]);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}


}

PHOTO_FUNC_INFO PhotoFuncInfo_mvfr = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_FRProcess",
		Movie_FRInit,                       ///< init FR, only execute at first process
		Movie_FRProcess,                    ///< process FR
		NULL,                 ///< process end FR
		NULL,                      ///< exit fp, only execute at photo close
		FR_CalcBuffSize,                    ///< get working buffer FR
		FR_CalcCacheBuffSize,               ///< get working cache buffer FR
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2018/05/18#Dan Guo -end


//#NT#2018/05/04#Bin Xiao -begin
//#NT# Face Detection by CNN
#if (MOVIE_FDCNN_FUNC == ENABLE)
void Movie_FDCNNInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	fdcnn_lock(FALSE);
	fdcnn_init(buf, cachebuf);
	//fdcnn_config(FDCNN_CFG_SENSITIVITY, 110);

}


void Movie_FDCNNProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	odt_rslt_loc  TimeStamp = {0};

    static UINT32 is_first = 1;
	if ((is_first == 1) && ((INT32)FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0) == 1)) {
        is_first = 0;

		fdcnn_netselect(FDCNN_METHOD_1);
		fdcnn_loadmodel();

	}

	if(is_first == 1) {
		return;
	}

	#if (MOVIE_FDCNN_USE_ODT && MOVIE_ODT_FUNC)
	static INT32					regist_falg = ODT_STA_ERROR;
	static UINT32					fdcnn_rslt_loc;
	static UINT32					fdcnn_num;
	static fdcnn_result				fdcnn_rslts[FDCNN_MAX_OUTNUM];
	static odt_od_result 			fdcnn_odt_rslts[FDCNN_MAX_OUTNUM];

	static odt_process_buf_param 	fdcnn_odt_buf_param;
	static odt_rslt_inf				fdcnn_buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				fdcnn_buf2;
	static odt_od_result 			fdcnn_buf3[FDCNN_MAX_OUTNUM];

	fdcnn_odt_buf_param.detect_rslt_addr = (UINT32)fdcnn_buf3;
	fdcnn_odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&fdcnn_buf2);
	fdcnn_odt_buf_param.odt_rslt_buffer_addr = (UINT32)fdcnn_buf1;

	if (regist_falg == ODT_STA_ERROR) {
		regist_falg = odt_regist(&fdcnn_odt_buf_param);
		if (regist_falg == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 15, fdcnn_odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 1, fdcnn_odt_buf_param.process_id);
			odt_config(odt_cfg_scale_for_match, 15, fdcnn_odt_buf_param.process_id);
		}
	}
	#endif

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		#if (MOVIE_FDCNN_USE_ODT && MOVIE_ODT_FUNC)
		TimeStamp.timestamp_sec  = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		TimeStamp.timestamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
		fdcnn_rslt_loc = (UINT32)&TimeStamp;
		#endif
		
		#if 0

		#if (MOVIE_FDCNN_USE_ODT && MOVIE_ODT_FUNC)
		fdcnn_rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
		#endif

		
		UINT32 t1 = 0, t2 = 0;
		Perf_Open();
		t1 = Perf_GetCurrent();

		fdcnn_process((IMG_BUF *)&isf_data.Desc[0]);

		t2 = Perf_GetCurrent();
		Perf_Close();

		debug_msg("%d\r\n", t2-t1);
		#else
		fdcnn_process((IMG_BUF *)&isf_data.Desc[0]);
		#endif

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

	#if (MOVIE_FDCNN_USE_ODT && MOVIE_ODT_FUNC)
	UINT32 i;
    URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
	fdcnn_num = fdcnn_getresults(fdcnn_rslts, &targetcoord);

	for(i = 0; i < fdcnn_num; i++)
	{
		fdcnn_odt_rslts[i].x = fdcnn_rslts[i].x;
		fdcnn_odt_rslts[i].y = fdcnn_rslts[i].y;
		fdcnn_odt_rslts[i].w = fdcnn_rslts[i].w;
		fdcnn_odt_rslts[i].h = fdcnn_rslts[i].h;
		fdcnn_odt_rslts[i].score = fdcnn_rslts[i].score;
		fdcnn_odt_rslts[i].class = fdcnn_rslts[i].class;
	}
	odt_setodrslt_timestamp(fdcnn_num, fdcnn_odt_rslts, fdcnn_rslt_loc, fdcnn_odt_buf_param.process_id);
	#endif

}

PHOTO_FUNC_INFO PhotoFuncInfo_mvfdcnn = {
	{
		PHOTO_PRIORITY_M,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_FDCNNProcess",
		Movie_FDCNNInit,                       ///< init FDCNN, only execute at first process
		Movie_FDCNNProcess,                    ///< process FDCNN
		NULL,                 ///< process end FDCNN
		NULL,                      ///< exit fp, only execute at photo close
		fdcnn_calcbuffsize,                    ///< get working buffer FDCNN
		fdcnn_calccachebuffsize,               ///< get working cache buffer FDCNN
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2018/05/04#Bin Xiao -end

//#NT#2018/05/04#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#if (MOVIE_ODT_FUNC == ENABLE)
void Movie_ODTInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	odt_init(buf, cachebuf);
	odt_lock(FALSE);
}



void Movie_ODTProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	if (!odt_getinitstate()) {
		return;
	}

	odt_ot_process();

	//Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
}


void Movie_ODTODProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	if (!odt_getinitstate()) {
		return;
	}

	odt_od_process();


}



PHOTO_FUNC_INFO PhotoFuncInfo_mvodt = {
	{
		PHOTO_PRIORITY_H,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODTProcess",
		Movie_ODTInit,                       ///< init ODT, only execute at first process
		Movie_ODTProcess,                    ///< process ODT
		NULL,                 ///< process end ODT
		NULL,                      ///< exit fp, only execute at photo close
		odt_calcbuffsize,                    ///< get working buffer ODT
		odt_calccachebuffsize,               ///< get working cache buffer ODT
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

PHOTO_FUNC_INFO PhotoFuncInfo_mvodtod = {
	{
		PHOTO_PRIORITY_H,                   ///< function hook process Id.
		IPL_PATH_1,                         ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,              ///< isr tigger event
		"Movie_ODTODProcess",
		NULL,                       ///< init ODT, only execute at first process
		Movie_ODTODProcess,                    ///< process ODT
		NULL,                 ///< process end ODT
		NULL,                      ///< exit fp, only execute at photo close
		NULL,                    ///< get working buffer ODT
		NULL,               ///< get working cache buffer ODT
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2018/05/04#Bin Xiao -end




//#NT#2017/08/29#Charlie Chang -begin
//#NT# Support CEVA Deep Neural Network (CDNN)
#if IPCAM_CDNN_FUNC
#if CDNN_DEV
VOID IPCam_CDNNInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	cdnndev_app_init(ISF_OUT16);
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamcdnn = {
	{
		PHOTO_PRIORITY_M,               ///< priority
		IPL_PATH_1,                     ///< IPL Id
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"IPCam_CDNNProcess",            ///< process name
		IPCam_CDNNInit,                 ///< init fp, only execute at first process
		cdnndev_app_process,            ///< process fp
		NULL,                           ///< process end fp
		NULL,                           ///< exit fp, only execute at photo close
		cdnndev_app_calc_buf_size,      ///< get working buffer fp
		cdnndev_app_calc_cache_buf_size,///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#else // CDNN_DEV
PHOTO_FUNC_INFO PhotoFuncInfo_ipcamcdnn = {
	{
		PHOTO_PRIORITY_M,               ///< priority
		IPL_PATH_1,                     ///< IPL Id
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"IPCam_CDNNProcess",            ///< process name
		alg_CDNNInit,                   ///< init fp, only execute at first process
		alg_CDNNProcess,                ///< process fp
		NULL,                           ///< process end fp
		NULL,                           ///< exit fp, only execute at photo close
		alg_CDNNCalcBufSize,            ///< get working buffer fp
		alg_CDNNCalcCacheBufSize,       ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // CDNN_DEV
#endif // IPCAM_CDNN_FUNC

#if IPCAM_CDNN2_FUNC
#if CDNN_DEV
VOID IPCam_CDNN2Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	cdnndev2_app_init(ISF_OUT16);
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamcdnn2 = {
	{
		PHOTO_PRIORITY_L,               ///< priority
		IPL_PATH_1,                     ///< IPL Id
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"IPCam_CDNN2Process",           ///< process name
		IPCam_CDNN2Init,                ///< init fp, only execute at first process
		cdnndev2_app_process,           ///< process fp
		NULL,                           ///< process end fp
		NULL,                           ///< exit fp, only execute at photo close
		cdnndev2_app_calc_buf_size,     ///< get working buffer fp
		cdnndev2_app_calc_cache_buf_size,///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#else // CDNN_DEV
PHOTO_FUNC_INFO PhotoFuncInfo_ipcamcdnn2 = {
	{
		PHOTO_PRIORITY_L,               ///< priority
		IPL_PATH_1,                     ///< IPL Id
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"IPCam_CDNN2Process",           ///< process name
		alg_CDNN2Init,                  ///< init fp, only execute at first process
		alg_CDNN2Process,               ///< process fp
		NULL,                           ///< process end fp
		NULL,                           ///< exit fp, only execute at photo close
		alg_CDNN2CalcBufSize,           ///< get working buffer fp
		alg_CDNN2CalcCacheBufSize,      ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // CDNN_DEV
#endif // IPCAM_CDNN2_FUNC
//#NT#2017/08/29#Charlie Chang -end

/*****BCMD FUNC*****/
static BOOL gBCMDStartFlag = FALSE;
static UINT64 gBCFrameTimeUS = 33333; //200ms
static UINT64 gBCLastFrameTime = 0;
static UINT64 gBCDiffFrameTimeUS = 0;
static UINT8 gAlgBcDbgLevel = 0;
PIMG_BUF gpBCImgBuf = 0;
IPCAM_BC_RESULT *gBC_result;
static BCParam g_BcParam = {0};
static URECT gBC_usr_dim = {0};

static BOOL cmd_bc_dbg(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gAlgBcDbgLevel = value;
	DBG_DUMP("Alg BCMD dbglevel Mode(%u)\r\n", gAlgBcDbgLevel);
	return TRUE;
}
SXCMD_BEGIN(alg, "alg")
SXCMD_ITEM("bc %", cmd_bc_dbg, "set BCMD dbg level[0:disable, 1:enable]")
SXCMD_END()

void Movie_SetBcParam(BCParam *param)
{
	memcpy(&g_BcParam, param, sizeof(BCParam));
	gBCLastFrameTime = 0;
	gBCDiffFrameTimeUS = 0;
	DBG_DUMP("Bc param_info:fps=%d bc_mode=%d bc_deghost_en=%d bc_update_prob=%d\r\n", g_BcParam.fps, g_BcParam.bc_mode,
		g_BcParam.param_info.bc_deghost_en, g_BcParam.param_info.bc_update_prob);
	if (g_BcParam.fps == 0) {
		gBCMDStartFlag = FALSE;
	} else {
		bc_setprms(BC_RESET,              g_BcParam.param_info.bc_reset);
		bc_setprms(BC_RSLT_NEW,           g_BcParam.param_info.bc_rslt_new);
		bc_setprms(BC_UPDATE_PROB,        g_BcParam.param_info.bc_update_prob);
		bc_setprms(BC_UPDATE_NEIGHBOR_EN, g_BcParam.param_info.bc_update_neighbor_en);
		bc_setprms(BC_DEGHOST_EN,         g_BcParam.param_info.bc_deghost_en);
		bc_setprms(BC_MAX_FG_FRM_NUM,     g_BcParam.param_info.bc_max_fg_frm_num);
		bc_setprms(BC_MIN_OBJ_AREA_PCT,   g_BcParam.param_info.bc_min_obj_area_pct);
		bc_setprms(BC_TH_CROSS_FRM_NUM,   g_BcParam.param_info.bc_th_cross_frm_num);
		bc_setprms(BC_TH_FEATURE,         g_BcParam.param_info.bc_th_feature);
		bc_setprms(BC_SENSI,              g_BcParam.param_info.bc_sensi);
		gBC_usr_dim.x = 0;
		gBC_usr_dim.y = 0;
		gBC_usr_dim.w = g_BcParam.param_info.bc_width;
		gBC_usr_dim.h = g_BcParam.param_info.bc_height;
		gBCFrameTimeUS = 1000000 / g_BcParam.fps;
		gBCMDStartFlag = TRUE;
	}
}

BC_RESULT *Movie_BcGetResult(void)
{
	if(gBC_result == NULL){
		return NULL;
	}
	return &(gBC_result->result);
}

/********************************************************************/
#if IPCAM_BC_FUNC

void IPCam_BCLock(BOOL enable){
	gbBCStartFlag = enable;
}

void IPCam_BCInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	gBCMDStartFlag = FALSE;
	gBCFrameTimeUS = 33333;
	bc_init(buf->Addr, cachebuf->Addr);
	gBC_result = (IPCAM_BC_RESULT *)((UINT32)cachebuf->Addr + bc_getcachebufsize());
	memset(gBC_result, 0, sizeof(IPCAM_BC_RESULT));
	DBG_DUMP("IPCam_BC_Init buf(%#x %d) cachebuf(%#x %d)\r\n",
			buf->Addr, buf->Size, cachebuf->Addr, cachebuf->Size);
	DBG_DUMP("BC rst buf(%#x %d)\r\n",
			(UINT32)gBC_result, sizeof(IPCAM_BC_RESULT));
}

void IPCam_BCProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	//UINT32 width=0;
	//UINT32 height=0;
	IMG_BUF *pBuf=NULL;

	double now_time = 0;
	static INT32 start_index = -1;
	static double old_time = 0;


	static UINT64 uiBCLastFrameTime =0;
	UINT32 LastTimeStamp_sec = 0;
	UINT32 LastTimeStamp_usec = 0;
	UINT32 TimeStamp_sec=0;
	UINT32 TimeStamp_usec =0;
	UINT32 DiffTimeStamp_sec =0 ;
	UINT32 DiffTimeStamp_usec = 0;
	static UINT64 uiBCDiffFrameTimeUS =0;


	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		now_time = clock();
		if (now_time - old_time <= 2000000) { ///2secs
			return;
		}
		start_index++;
		DBG_DUMP("BC start!!\r\n");
	}

	if(gbBCStartFlag == FALSE && gBCMDStartFlag == FALSE){
		return;
	}

	#if (MOVIE_ODT_FUNC && IPCAM_BC_USE_ODT)
	static INT32					regist_flag = ODT_STA_ERROR;
	static UINT32					rslt_loc;
	static UINT32					num;
	static URECT					rslts[32];
	static odt_od_result 			odt_rslts[32];

	static odt_process_buf_param 	odt_buf_param;
	static odt_rslt_inf				buf1[ODT_MAX_DISRSLTBUF_NUM];
	static odt_rslt_inf				buf2;
	static odt_od_result 			buf3[32];

	odt_buf_param.detect_rslt_addr = (UINT32)buf3;
	odt_buf_param.odt_od_rslt_buffer_addr = (UINT32)(&buf2);
	odt_buf_param.odt_rslt_buffer_addr = (UINT32)buf1;

	if (regist_flag == ODT_STA_ERROR) {
		regist_flag = odt_regist(&odt_buf_param);
		if (regist_flag == ODT_STA_ERROR) {
			return;
		}else{
			odt_config(odt_cfg_max_obj_life1, 5, odt_buf_param.process_id);
			odt_config(odt_cfg_max_obj_life2, 0, odt_buf_param.process_id);
		}
	}
	#endif

	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

	if(!ImageUnit_IsAllowPull(pSrc)){
		DBG_WRN("ImageUnit_IsAllowPull fail!\r\n");
		return;
	}

	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		if (uiBCLastFrameTime == 0){
			uiBCLastFrameTime = isf_data.TimeStamp;
		}

		TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
		TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
		LastTimeStamp_sec = (uiBCLastFrameTime >> 32) & 0xFFFFFFFF;
		LastTimeStamp_usec = uiBCLastFrameTime & 0xFFFFFFFF;
		DiffTimeStamp_sec = TimeStamp_sec - LastTimeStamp_sec;
		DiffTimeStamp_usec = TimeStamp_usec - LastTimeStamp_usec;
		uiBCDiffFrameTimeUS = (UINT64)DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec + uiBCDiffFrameTimeUS;


		if (uiBCDiffFrameTimeUS > gBCFrameTimeUS) {

			#if (MOVIE_ODT_FUNC && IPCAM_BC_USE_ODT)
			rslt_loc = (odt_getsavedisrsltloc() + ODT_MAX_DISRSLTBUF_NUM - 1)%ODT_MAX_DISRSLTBUF_NUM;
			#endif

			pBuf =  (IMG_BUF *)&isf_data.Desc[0];
			bc_setimg(pBuf);
			bc_process();

			#if (MOVIE_ODT_FUNC && IPCAM_BC_USE_ODT)
			UINT32 i;
			URECT targetcoord = {0, 0, disHw_getCurDisInH(), disHw_getCurDisInV()};
			num = bc_gettarget(rslts, &targetcoord, 32);
			for(i = 0; i < num; i++)
			{
				odt_rslts[i].x = rslts[i].x;
				odt_rslts[i].y = rslts[i].y;
				odt_rslts[i].w = rslts[i].w;
				odt_rslts[i].h = rslts[i].h;
				odt_rslts[i].score = 0;
				odt_rslts[i].class = 1008;
			}
			odt_setodrslt(num, odt_rslts, rslt_loc, odt_buf_param.process_id);
			#endif

			if (gBCMDStartFlag) {
				bc_getforeground_v2(gBC_result->result.evt_map_addr, bc_getprms(BC_IMG_WIDTH),bc_getprms(BC_IMG_HEIGHT), 1, 0);
				gBC_result->result.evt_width = bc_getprms(BC_IMG_WIDTH);
				gBC_result->result.evt_height = bc_getprms(BC_IMG_HEIGHT);
			}
			uiBCDiffFrameTimeUS -= gBCFrameTimeUS;
			if (gBCMDStartFlag) {
				if (g_BcParam.bc_mode == 1) {
					if (gBC_usr_dim.w == 0 || gBC_usr_dim.h == 0)
						gBC_result->result.targetnum = bc_gettarget((URECT*)gBC_result->result.target, NULL, BC_MAX_NUM);
					else
						gBC_result->result.targetnum = bc_gettarget((URECT*)gBC_result->result.target, &gBC_usr_dim, BC_MAX_NUM);
					//DBG_DUMP("target number = %d\r\n", gBC_result->result.targetnum);
				}
			}
		}
		uiBCLastFrameTime = isf_data.TimeStamp;

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}

	if (gbBCStartFlag) {
		if ((BOOL)bc_getprms(BC_RSLT_NEW) == FALSE)
			return;
	}
	//check status
	#if 0
	static URECT     Info[1024];
	URECT            DispCoord = {0, 0, 640, 480};

	DispCoord.x = 0;
	DispCoord.y = 0;
	DispCoord.w = width;
	DispCoord.h = height;
	UINT32 number = bc_gettarget(Info, &DispCoord, 256);
	UINT32 pct = bc_gettdprms(BC_TAMPER_PCT_CUR);
	UINT32 frmnum = bc_gettdprms(BC_TAMPER_FRM_NUM_CUR);
	UINT32 trigger = bc_gettdprms(BC_TD_STATUS);
	bc_setprms(BC_RSLT_NEW, 0);
	UINT32 i =0;
	DBG_DUMP("number = %d, tamper_pct = %d, tamper_frmnum = %d trigger:%d\r\n", number, pct, frmnum, trigger);
	for (i = 0; i < number; i++) {
		DBG_DUMP("x = %d, y = %d, w = %d, h = %d\r\n", Info[i].x, Info[i].y, Info[i].w, Info[i].h);
	}
	#endif

/********Send BCMD result**********/
	if (gBCMDStartFlag == TRUE) {
		gBC_result->event = IPCAM_EVENT_BCMD;
		gBC_result->TimeStamp.tv_sec = TimeStamp_sec;
		gBC_result->TimeStamp.tv_usec = TimeStamp_usec;
#if 0
		//for test
		gBC_result->result.targetnum = 1;
		gBC_result->result.target[0].x = 10;
		gBC_result->result.target[0].y = 20;
		gBC_result->result.target[0].w = 30;
		gBC_result->result.target[0].h = 40;
		gBC_result->result.evt_map_addr[1] = 1;
		gBC_result->result.evt_map_addr[2] = 1;
		gBC_result->result.evt_map_addr[3] = 1;
		gBC_result->result.evt_width = 3;
		gBC_result->result.evt_height = 3;
		///
#endif
		if (gAlgBcDbgLevel == 1)
			DBG_DUMP("%s:sizeof(IPCAM_BC_RESULT)=%d targetnum=%d target[0]=(%d %d %d %d) evt_map_data:(0x%x 0x%x 0x%x 0x%x) evt_dim(%d %d)\r\n",
			__func__, sizeof(IPCAM_BC_RESULT), gBC_result->result.targetnum, gBC_result->result.target[0].x, gBC_result->result.target[0].y,
			gBC_result->result.target[0].w,gBC_result->result.target[0].h, gBC_result->result.evt_map_addr[0],
			gBC_result->result.evt_map_addr[1], gBC_result->result.evt_map_addr[2], gBC_result->result.evt_map_addr[3],
			gBC_result->result.evt_width, gBC_result->result.evt_height);
		NvtUctrl_NotifyRaw((void *)gBC_result, sizeof(IPCAM_BC_RESULT));
	}
/*******************************/
}

UINT32 IPCam_BCGetBufSize(void)
{
	return bc_getbufsize();
}

UINT32 IPCam_BCGetCacheBufSize(void)
{
	return bc_getcachebufsize() + sizeof(IPCAM_BC_RESULT);
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcambc = {
	{
		PHOTO_PRIORITY_M,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMBCProcess",
		IPCam_BCInit,              ///< init fp, only execute at first process
		IPCam_BCProcess,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		IPCam_BCGetBufSize,      ///< get working buffer fp
		IPCam_BCGetCacheBufSize,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

#endif
/***********************************/
static BOOL gVQAStartFlag = FALSE;
static BOOL gVQAStatusFlag = FALSE;
static UINT64 gVQAFrameTimeUS = 33333;
static UINT64 gVQALastFrameTime = 0;
static UINT64 gVQADiffFrameTimeUS = 0;
static UINT64 gVQABlockTimeUS = 0;

vqa_res_t gVQA_res;
void Movie_VqaSetFps(UINT32 fps)
{
	gVQALastFrameTime = 0;
	gVQADiffFrameTimeUS = 0;
	if (fps == 0) {
		gVQAStartFlag = FALSE;
	} else {
		gVQAFrameTimeUS = 1000000 / fps;
		gVQAStartFlag = TRUE;
	}
}

vqa_res_t Movie_VqaGetResult(void)
{
	return gVQA_res;
}
#if (IPCAM_BC_FUNC == ENABLE && IPCAM_FUNC == ENABLE)
BOOL Photo_VQA_param_init(vqa_param_t *vqa_param)
{
	// init VQA default param
	//enable
	vqa_param->enable_param.en_too_light = 1;
	vqa_param->enable_param.en_too_dark = 1;
	vqa_param->enable_param.en_blur = 1;
	vqa_param->enable_param.en_auto_adj_param = 1;
	vqa_param->enable_param.en_ref_md = 1;
	//global
	vqa_param->global_param.auto_adj_period = 5;
	vqa_param->global_param.g_alarm_frame_num = 20;
	//overexposure
	vqa_param->light_param.too_light_strength_th = 215;
	vqa_param->light_param.too_light_cover_th = 15;
	vqa_param->light_param.too_light_alarm_times = 5;
	//underexposure
	vqa_param->light_param.too_dark_strength_th = 50;
	vqa_param->light_param.too_dark_cover_th = 75;
	vqa_param->light_param.too_dark_alarm_times = 5;
	//blur
	vqa_param->contrast_param.blur_strength = 8;
	vqa_param->contrast_param.blur_cover_th = 85;
	vqa_param->contrast_param.blur_alarm_times = 5;
	vqa_set_param(0, vqa_param);
	return TRUE;
}
void Photo_VQA_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	vqa_param_t vqa_param;
	memset(&vqa_param, 0, sizeof(vqa_param_t));
	if (buf->Addr) {
		gVQAStartFlag = FALSE;
		gVQAFrameTimeUS = 33333;
		memset(&gVQA_res, 0, sizeof(vqa_res_t));
		//VQA TODO
		IPCam_BCLock(1);
		vqa_init(0, (UINT32)buf->Addr);
		Photo_VQA_param_init(&vqa_param);
		DBG_DUMP("Photo_VQA_Init buf(%#x %d)\r\n", buf->Addr, buf->Size);
	} else {
		DBG_ERR("Get vqa buf fail\n");
	}

}

void Photo_VQA_Process(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	static IPCAM_VQA_RESULT IPCamVQAResult = {0};
	static UINT32 TimeStamp_sec = 0, TimeStamp_usec = 0;
	static clock_t old_time = 0;
	static INT32 start_index = -1;
	ISF_DATA isf_data;
	clock_t t1, t2;
	ISF_UNIT            *pUnit;
	ISF_PORT            *pSrc;
    IMG_BUF             *InputImg;
	UINT8			    *y_map_ptr = 0;
	UINT32 LastTimeStamp_sec, LastTimeStamp_usec;
	INT32 DiffTimeStamp_sec, DiffTimeStamp_usec;

	if (old_time == 0) {
		old_time = clock();
	}
	//wait boot device finish
	if (start_index == -1) {
		if (clock() - old_time <= 1000000) { ///1secs
			return;
		}
		start_index++;
		DBG_DUMP("VQA detection start!!\r\n");
	}
	if (gVQAStartFlag == FALSE) {
		return;
	}
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);
	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_ALLOW_PULL_IMM, 1) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 1 Fail\r\n");
		return;
	}
	ImageUnit_End();
	if (ImageUnit_IsAllowPull(pSrc) == 0) {
		DBG_WRN("ImageUnit_IsAllowPull fail\r\n");
		return;
	}
	if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, gVQAFrameTimeUS) != ISF_OK) {
		DBG_WRN("ImageUnit_PullData fail\r\n");
		IPCamVQAResult.Event = IPCAM_EVENT_VQA;
		IPCamVQAResult.TimeStamp.tv_sec = TimeStamp_sec;
		IPCamVQAResult.TimeStamp.tv_usec = TimeStamp_usec;
		NvtUctrl_NotifyRaw((void *)&IPCamVQAResult, sizeof(IPCAM_VQA_RESULT));
		#if 0
		ImageUnit_Begin(&ISF_UserProc, 0);
		if (ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_ALLOW_PULL_IMM, 0) != ISF_OK) {
			DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
			return;
		}
		ImageUnit_End();
		#endif
		return;
	}
	#if 0
	ImageUnit_Begin(&ISF_UserProc, 0);
	if (ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_ALLOW_PULL_IMM, 0) != ISF_OK) {
		DBG_WRN("Set USERPROC_PARAM_ALLOW_PULL_IMM to 0 Fail\r\n");
		return;
	}
	ImageUnit_End();
	#endif
	if (gVQALastFrameTime == 0)
		gVQALastFrameTime = isf_data.TimeStamp;

	TimeStamp_sec = (isf_data.TimeStamp >> 32) & 0xFFFFFFFF;
	TimeStamp_usec = isf_data.TimeStamp & 0xFFFFFFFF;
	LastTimeStamp_sec = (gVQALastFrameTime >> 32) & 0xFFFFFFFF;
	LastTimeStamp_usec = gVQALastFrameTime & 0xFFFFFFFF;

	DiffTimeStamp_sec = (long)TimeStamp_sec - (long)LastTimeStamp_sec;
	DiffTimeStamp_usec = (long)TimeStamp_usec - (long)LastTimeStamp_usec;
	gVQADiffFrameTimeUS += DiffTimeStamp_sec * 1000000 + DiffTimeStamp_usec;
	gVQALastFrameTime = isf_data.TimeStamp;

	if (gVQADiffFrameTimeUS > gVQAFrameTimeUS) {
		gVQADiffFrameTimeUS -= gVQAFrameTimeUS;
		t1 = clock();
		InputImg =  (IMG_BUF *)&isf_data.Desc[0];
		y_map_ptr = (UINT8 *)InputImg->PxlAddr[0];
		vqa_run(0, y_map_ptr, InputImg->Width, InputImg->Height, (UINT8*)buf->Addr, &gVQA_res);
		if (gVQABlockTimeUS) {
			Delay_DelayUs(gVQABlockTimeUS);
			gVQABlockTimeUS = 0;
		}
		t2 = clock();
		DBG_IND("y_map_ptr(%p)\r\n", y_map_ptr);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);

		if ((t2 - t1) > 20000) {
			gVQAFrameTimeUS = 1000*1000;
			DBG_WRN("VQA ProcessTime=%ld us, adjust FrameTime=%llu us\r\n", (t2 - t1), gVQAFrameTimeUS);
		}
	} else {
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
		return;
	}

	if (__DBGLVL__ >= 5 || gVQAStatusFlag)
		DBG_DUMP("vqa_res light(%d) dark(%d) blur(%d)\r\n",
			 gVQA_res.res_too_light, gVQA_res.res_too_dark,
			 gVQA_res.res_blur);

	IPCamVQAResult.Event = IPCAM_EVENT_VQA;
	IPCamVQAResult.TimeStamp.tv_sec = TimeStamp_sec;
	IPCamVQAResult.TimeStamp.tv_usec = TimeStamp_usec;
	IPCamVQAResult.TooLight = gVQA_res.res_too_light;
	IPCamVQAResult.TooDark = gVQA_res.res_too_dark;
	IPCamVQAResult.Blur = gVQA_res.res_blur;
	NvtUctrl_NotifyRaw((void *)&IPCamVQAResult, sizeof(IPCAM_VQA_RESULT));
}

UINT32 Photo_VQA_GetBuf(void)
{
	//TODO
		return vqa_calc_buf_size();
	//return 16*1024;
}

PHOTO_FUNC_INFO PhotoFuncInfo_vqa = {{
		PHOTO_PRIORITY_M, ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_SIE_VD,       ///< isr tigger event
		"VQA", Photo_VQA_Init, ///< init fp, only execute at first process
		Photo_VQA_Process,     ///< process fp
		NULL,  			///< process end fp
		NULL,            ///< exit fp, only execute at photo close
		Photo_VQA_GetBuf,      ///< get working buffer fp
		NULL,    		///< get working cache buffer fp
	},
	NULL ///< point next Function Obj, last Function must be set NULL
};
#endif

static BOOL cmd_vqa_case(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	if (value == 1) {
		gVQA_res.res_blur = !gVQA_res.res_blur;
	}
	return TRUE;
}

static BOOL cmd_vqa_block(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gVQABlockTimeUS = (UINT64)value * 1000;
	DBG_DUMP("Set VQA BlockTimeUS(%llu)\r\n", gVQABlockTimeUS);
	return TRUE;
}

static BOOL cmd_vqa_set(CHAR *strCmd)
{
	UINT32 value = 0;
	sscanf_s(strCmd, "%d", &value);
	gVQALastFrameTime = 0;
	gVQADiffFrameTimeUS = 0;
	if (value == 0) {
		gVQAStartFlag = FALSE;
	} else {
		gVQAFrameTimeUS = 1000000 / value;
		gVQAStartFlag = TRUE;
	}
	DBG_DUMP("VQA FrameTimeUS(%llu), StartFlag(%d) ResultFlag(%d)\r\n", gVQAFrameTimeUS, gVQAStartFlag, gVQAStatusFlag);
	return TRUE;
}

static BOOL cmd_vqa_status(CHAR *strCmd)
{
	UINT32 value = 0;
	DBG_DUMP("VQA FrameTimeUS(%llu), StartFlag(%d) ResultFlag(%d)\r\n", gVQAFrameTimeUS, gVQAStartFlag, gVQAStatusFlag);
	sscanf_s(strCmd, "%d", &value);
	if (value == 0) {
		gVQAStatusFlag = FALSE;
	} else {
		gVQAStatusFlag = TRUE;
	}
	return TRUE;
}

SXCMD_BEGIN(vqa, "vqa")
SXCMD_ITEM("case %", cmd_vqa_case, "send test result to linux [1:light, 2:dark, 3:occlusion, 4:blur, 5:scene change]")
SXCMD_ITEM("block %", cmd_vqa_block, "set md block time (in msec)")
SXCMD_ITEM("set %", cmd_vqa_set, "set fps, fps=0 to stop")
SXCMD_ITEM("status %", cmd_vqa_status, "vqa status, 1=dump")
SXCMD_END()
void Photo_InstallCmd(void)
{
	SxCmd_AddTable(vqa);
	SxCmd_AddTable(alg);
}
/********************************************************/
//#NT#2018/03/19#Wayne Wei -begin
//#NT# Support License Plate Recognition (LPR)
#if IPCAM_LPR_FUNC
#define LPR_SENSOR_FLOW 1 //1: sensor mode, 0: file mode

//#define _DEBUG_LPR_USER_
#define LPR_IMG_PATH "A:\\LPRLib\\DI\\imgData\\"
#define LPR_MAX_FILE_PATH              100
UINT32 lpr_test_idx = 0;
char filename[64];

//////////////////////////////////////////////////////////////////////////
// Voting function
//////////////////////////////////////////////////////////////////////////
#define MAX_VOTE_NUM   5
#define MAX_NAME_SIZE  15
char g_vote_name[MAX_VOTE_NUM][MAX_NAME_SIZE];

void Movie_LPRVote_init(void)
{
    int idx,i,j;

    for(i = 0; i < MAX_VOTE_NUM; i++) {
        for(j = 0; j < MAX_NAME_SIZE; j++) {
                g_vote_name[i][j] = 0;
        }
    }

    for(idx = 0; idx < MAX_VOTE_NUM; idx++) {
            snprintf(&g_vote_name[idx][0], MAX_NAME_SIZE, "...");
    }
}

int Movie_LPRVote_lp_name(char* input_lp_name)
{
    int      i, j, cmp;
    int      max_idx, max_value;
    int      vote_sum[MAX_VOTE_NUM] = {};
    // delete old lp name
    for(i = 0;i < MAX_VOTE_NUM - 1;i++) {
            snprintf(&g_vote_name[i][0], MAX_NAME_SIZE, "%s", &g_vote_name[i+1][0]);
    }
    // get new lp name
    snprintf(&g_vote_name[MAX_VOTE_NUM - 1][0], MAX_NAME_SIZE, "%s", input_lp_name);
    // voting
    for(i = 0;i < MAX_VOTE_NUM; i++) {
            for(j = 0;j < MAX_VOTE_NUM; j++) {
                    cmp = strcmp(&g_vote_name[i][0], &g_vote_name[j][0]);
                    if(cmp == 0){
                        vote_sum[i]++;
                    }
            }
    }
    max_value = -1;
    max_idx = 0;
    for(i = MAX_VOTE_NUM - 1; i >= 0; i--) {
            if(vote_sum[i] >= max_value) {
                max_idx = i;
                max_value = vote_sum[i];
        }
    }
    // output the result with max voting
    if(max_value != 0) {
        DBG_DUMP("[LPR]Vote: %s\r\n", &g_vote_name[max_idx][0]);
    }

    return 0;
}

INT32 Movie_LPRCheckVer(void)
{
    INT32 ret;
    LPR_SYS_PRMS sys;

    sys = lpr_getsystem();

    if ((sys.ver.major != LPR_VERSION_MAJOR) || (sys.ver.minor != LPR_VERSION_MINOR) ||
        (sys.ver.build != LPR_VERSION_BUILD)) {
            DBG_ERR("[LPR]Error, Not match version, Please check LPR_API.h(%d:%d:%d) and LPR.a(%d:%d:%d)\r\n",
                       LPR_VERSION_MAJOR, LPR_VERSION_MINOR, LPR_VERSION_BUILD,
                    sys.ver.major, sys.ver.minor, sys.ver.build);
            ret = LPR_STA_ERR;
    } else {
            DBG_DUMP("[LPR] Version Check OK:  LPR_API.h(%d:%d:%d) and LPR.a(%d:%d:%d)\r\n",
                    LPR_VERSION_MAJOR, LPR_VERSION_MINOR, LPR_VERSION_BUILD,
                    sys.ver.major, sys.ver.minor, sys.ver.build);
            ret = LPR_STA_OK;
    }

    return ret;
}
void IPCam_LPRInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	////init after file system is ready
	return;
}

void IPCam_LPRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA            isf_data;
	ISF_UNIT            *pUnit;
	ISF_PORT            *pSrc;
	static UINT32       check_fs_status = 0;
        IMG_BUF             *InputImg;
	LPR_RESULT          lpr_result;
	UINT32              p_idx;

#if (DEBUG_DUMP_IMG == ENABLE)
	static UINT32   frame_cnt = 0;
	char            filename[50];
#endif

	if (check_fs_status == 0) {

		check_fs_status = FileSys_GetParam(FST_PARM_TASK_INIT_RDY, 0);
		if (check_fs_status == 0) {
			//DBG_DUMP("####FS is not ready####\r\n");
			return;
		} else {
			DBG_DUMP("####FS is ready####\r\n");
		}

		LPR_INIT_PRMS      init_prms = {0};
		UINT32 uiPool;
		UINT32 uiPoolSize;

		if (Movie_LPRCheckVer() != LPR_STA_OK) {
			DBG_ERR("[LPR]Error, Check version failed.\r\n");
			check_fs_status = 0;
			return;
		}
		Movie_LPRVote_init();

		uiPoolSize = LPR_CalcBuffSize();
		uiPool=SxCmd_GetTempMem(uiPoolSize);

		buf->Addr = uiPool;
		buf->Size = uiPoolSize;
		cachebuf->Addr = 0;
		cachebuf->Size = 0;

		init_prms.cp_type    = CN_TYPE;
		init_prms.src_w      = 720;
		init_prms.src_h      = 480;
		init_prms.crop_x     = 0;
		init_prms.crop_y     = 0;
		init_prms.crop_w     = 600;
		init_prms.crop_h     = 300;
		init_prms.detect_w   = 600;
		init_prms.detect_h   = 300;
		init_prms.reg_w      = 600;
		init_prms.reg_h      = 300;
		snprintf(init_prms.cp_model_path, sizeof(init_prms.cp_model_path), "A:\\LPRLib\\model");
		init_prms.buf        = buf;
		init_prms.cache_buf  = cachebuf;

		lpr_ap_init(&init_prms);
    	}

	pUnit = &ISF_UserProc;
	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

    	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		//IMG_BUF *pISF_buf = NULL;
		//pISF_buf = (IMG_BUF *)&isf_data.Desc[0];
		//DBG_IND("send DSP YUV w=%d h=%d\r\n", pISF_buf->Width, pISF_buf->Height);
		InputImg = (IMG_BUF *)&isf_data.Desc[0];

#if (DEBUG_DUMP_IMG == ENABLE)
		frame_cnt++;
		if (!(frame_cnt % 100)) {
			snprintf(filename, 50, "A:\\test%05d.yuv", frame_cnt);
			DBG_DUMP("Save yuv file %s\r\n", filename);
			GxImg_DumpBuf(filename, (IMG_BUF *)&isf_data.Desc[0], GX_IMAGE_DUMP_ALL);
		}
#endif

#if 0
		if (MovRec_IsRecording()) {
                	DBG_DUMP("####[Rec]Sensor inputting width=%d height=%d\r\n", InputImg->Width, InputImg->Height);
        	} else {
                	DBG_DUMP("####[Preview]Sensor inputting width=%d height=%d\r\n", InputImg->Width, InputImg->Height);
        	}
#endif

#if (LPR_SENSOR_FLOW == 1)
		static UINT32 is_first = 1;
		LPR_SENSOR_PRMS sensor_prms = {0};

		DBG_DUMP("[LPR]Sensor mode\r\n");
		sensor_prms.data = InputImg;

		if (is_first == 1) {
			//Run-time to set user parameter
			lpr_setprms(LPR_ROI_LEFT_E, 5);
			lpr_setprms(LPR_ROI_RIGHT_E, 5);
			lpr_setprms(LPR_ROI_TOP_E, 5);
			lpr_setprms(LPR_ROI_BOTTOM_E, 5);
			lpr_setprms(LPR_FEA_MAX_WIDTH_E, 650);
			lpr_setprms(LPR_FEA_MIN_WIDTH_E, 100);
			lpr_setprms(LPR_FEA_MAX_HEIGHT_E, 150);
			lpr_setprms(LPR_FEA_MIN_HEIGHT_E, 25);
			lpr_setprms(LPR_REG_THRESHOLD_E, 400);

			is_first = 0;
		}

#ifdef _DEBUG_LPR_USER_
		DBG_DUMP("[LPR]Get parameter, roi:(%d %d %d %d) fea:(%d %d %d %d) reg:(%d)\r\n",
			lpr_getprms(LPR_ROI_LEFT_E), lpr_getprms(LPR_ROI_RIGHT_E),
			lpr_getprms(LPR_ROI_TOP_E), lpr_getprms(LPR_ROI_BOTTOM_E),
			lpr_getprms(LPR_FEA_MAX_WIDTH_E), lpr_getprms(LPR_FEA_MIN_WIDTH_E),
			lpr_getprms(LPR_FEA_MAX_HEIGHT_E), lpr_getprms(LPR_FEA_MIN_HEIGHT_E),
			lpr_getprms(LPR_REG_THRESHOLD_E));
#endif

		lpr_proc_sensor(&sensor_prms);
		lpr_get_result(&lpr_result);

		for(p_idx = 0;p_idx < lpr_result.max_rslt_num; p_idx++){
			DBG_DUMP("[LPR]LPName_Top%d: %s num=%d\r\n", p_idx+1, lpr_result.rslt[p_idx].lp_name, lpr_result.rslt[p_idx].ch_num);
		}
		Movie_LPRVote_lp_name((char *)lpr_result.rslt[0].lp_name);

#endif

		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}


#if (LPR_SENSOR_FLOW == 0)
	//DBG_DUMP("####Sensor not inputting, please enter \"ks2\" to enable record.\r\n");
        DBG_DUMP("[LPR]File mode\r\n");
        LPR_FILE_PRMS file_prms = {0};
        UINT32 test_num;
        UINT32 test_max_num;

        static UINT32 is_first = 1;
	if (is_first == 1) {
		//Run-time to set user parameter
		lpr_setprms(LPR_ROI_LEFT_E, 5);
		lpr_setprms(LPR_ROI_RIGHT_E, 5);
		lpr_setprms(LPR_ROI_TOP_E, 5);
		lpr_setprms(LPR_ROI_BOTTOM_E, 5);
		lpr_setprms(LPR_FEA_MAX_WIDTH_E, 650);
		lpr_setprms(LPR_FEA_MIN_WIDTH_E, 100);
		lpr_setprms(LPR_FEA_MAX_HEIGHT_E, 150);
		lpr_setprms(LPR_FEA_MIN_HEIGHT_E, 25);
		lpr_setprms(LPR_REG_THRESHOLD_E, 10000);

		is_first = 0;
	}

#ifdef _DEBUG_LPR_USER_
                DBG_DUMP("[LPR]Get parameter, roi:(%d %d %d %d) fea:(%d %d %d %d) reg:(%d)\r\n",
                        lpr_getprms(LPR_ROI_LEFT_E), lpr_getprms(LPR_ROI_RIGHT_E),
                        lpr_getprms(LPR_ROI_TOP_E), lpr_getprms(LPR_ROI_BOTTOM_E),
                        lpr_getprms(LPR_FEA_MAX_WIDTH_E), lpr_getprms(LPR_FEA_MIN_WIDTH_E),
                        lpr_getprms(LPR_FEA_MAX_HEIGHT_E), lpr_getprms(LPR_FEA_MIN_HEIGHT_E),
                        lpr_getprms(LPR_REG_THRESHOLD_E));
#endif

        test_num = 10;
        test_max_num = 10;
        DBG_DUMP("[LPR]test_num=%d\r\n", test_num);

        lpr_test_idx++;
        if (lpr_test_idx > test_max_num) {
                lpr_test_idx = 1;
        }

        sprintf((char *)filename, "%strain_o_000%02d.jpg", LPR_IMG_PATH, lpr_test_idx);

        file_prms.filename = filename;

        DBG_DUMP("[LPR][DET]filename=%s\r\n", file_prms.filename);

        lpr_proc_file(&file_prms);

        DBG_DUMP("[LPR]####test license plate idx=%d \r\n", lpr_test_idx);

        lpr_get_result(&lpr_result);

        for(p_idx = 0;p_idx < lpr_result.max_rslt_num; p_idx++){
                DBG_DUMP("[TestKit][LPR]LPName_Top%d: %s num=%d\r\n", p_idx+1, lpr_result.rslt[p_idx].lp_name, lpr_result.rslt[p_idx].ch_num);
        }

#endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamlpr = {
	{
		PHOTO_PRIORITY_M,               ///< priority
		IPL_PATH_1,                     ///< IPL Id
		PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
		"IPCam_LPRProcess",             ///< process name
		IPCam_LPRInit,                  ///< init fp, only execute at first process
		IPCam_LPRProcess,               ///< process fp
		NULL,                           ///< process end fp
		NULL,                           ///< exit fp, only execute at photo close
		LPR_CalcBuffSize,               ///< get working buffer fp
		NULL,                           ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // IPCAM_LPR_FUNC
//#NT#2018/03/19#Wayne Wei -end


////////////////////
////////////////////User PullData 1
void IPCam_Pull1Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull1(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT1, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];
		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 1 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull1 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull1",
		IPCam_Pull1Init,              ///< init fp, only execute at first process
		IPCam_Pull1,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 2
void IPCam_Pull2Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull2(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT2, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT2);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 2 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull2 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull2",
		IPCam_Pull2Init,              ///< init fp, only execute at first process
		IPCam_Pull2,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};
////////////////////
////////////////////User PullData 3
void IPCam_Pull3Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull3(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT3, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT3);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 3 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull3 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull3",
		IPCam_Pull3Init,              ///< init fp, only execute at first process
		IPCam_Pull3,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};


////////////////////
////////////////////User PullData 4
void IPCam_Pull4Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull4(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT4, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT4);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 4 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull4 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull4",
		IPCam_Pull4Init,              ///< init fp, only execute at first process
		IPCam_Pull4,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 5
void IPCam_Pull5Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull5(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT5, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT5);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 5 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull5 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull5",
		IPCam_Pull5Init,              ///< init fp, only execute at first process
		IPCam_Pull5,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};


////////////////////
////////////////////User PullData 6
void IPCam_Pull6Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull6(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT6, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT6);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 6 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull6 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull6",
		IPCam_Pull6Init,              ///< init fp, only execute at first process
		IPCam_Pull6,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 7
void IPCam_Pull7Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull7(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT7, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT7);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 1 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull7 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull7",
		IPCam_Pull7Init,              ///< init fp, only execute at first process
		IPCam_Pull7,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 8
void IPCam_Pull8Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull8(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT8, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT8);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 8 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull8 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull8",
		IPCam_Pull8Init,              ///< init fp, only execute at first process
		IPCam_Pull8,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 9
void IPCam_Pull9Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull9(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT9, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT9);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 9 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull9 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull9",
		IPCam_Pull9Init,              ///< init fp, only execute at first process
		IPCam_Pull9,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 10
void IPCam_Pull10Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull10(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT10, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT10);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 10 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull10 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull10",
		IPCam_Pull10Init,              ///< init fp, only execute at first process
		IPCam_Pull10,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 11
void IPCam_Pull11Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull11(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT11, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT11);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 11 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull11 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull11",
		IPCam_Pull11Init,              ///< init fp, only execute at first process
		IPCam_Pull11,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 12
void IPCam_Pull12Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull12(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT12, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT12);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 12 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull12 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull12",
		IPCam_Pull12Init,              ///< init fp, only execute at first process
		IPCam_Pull12,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 13
void IPCam_Pull13Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull13(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT13, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT13);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 13 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull13 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull13",
		IPCam_Pull13Init,              ///< init fp, only execute at first process
		IPCam_Pull13,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 14
void IPCam_Pull14Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull14(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT14, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT14);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 14 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull14 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull14",
		IPCam_Pull14Init,              ///< init fp, only execute at first process
		IPCam_Pull14,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 15
void IPCam_Pull15Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull15(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT15, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT15);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 15 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull15 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull15",
		IPCam_Pull15Init,              ///< init fp, only execute at first process
		IPCam_Pull15,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};

////////////////////
////////////////////User PullData 16
void IPCam_Pull16Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{


}

void IPCam_Pull16(MEM_RANGE *buf, MEM_RANGE *cachebuf){

	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	ISF_DATA         isf_data;
	pUnit = &ISF_UserProc;

	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(ISF_OUT16, USERPROC_PARAM_BYPASS_IMM, FALSE);
	ImageUnit_End();

	pSrc = ImageUnit_Out(pUnit, ISF_OUT16);

    if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
		IMG_BUF 		*pISF_buf = NULL;
		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];

		////do what you want~~~
		static UINT32 test_count=0;
		if(test_count ==0){
			DBG_DUMP("Pull 16 data w:%d h:%d\r\n",pISF_buf->Width, pISF_buf->Height);
			test_count=33;
		}
		else{
			test_count--;
		}

		ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_data, 0);
		ImageUnit_ReleaseData(&isf_data);
    }

}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamPull16 = {
	{
		PHOTO_PRIORITY_H,          ///< function hook process Id.
		IPL_PATH_1,                ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,     ///< isr tigger event
		"Photo_IPCAMPull16",
		IPCam_Pull16Init,              ///< init fp, only execute at first process
		IPCam_Pull16,           ///< process fp
		NULL,                      ///< process end fp
		#if (PHOTO_EXIT_CB == 1)
		NULL,                      ///< exit fp, only execute at photo close
		#endif
		NULL,      ///< get working buffer fp
		NULL,      ///< get working cache buffer fp
	},
	NULL                           ///< point next Function Obj, last Function must be set NULL
};



#if (IPCAM_DEPTH_FUNC == ENABLE)


#ifdef IPCAM_DEPTH_DEBUG
static BOOL sbs_mode(CHAR *strCmd)
{
	if(strCmd == NULL){
		DBG_WRN("Please Set sbs mode!\r\n");
		return FALSE;	}
		DBG_DUMP("set sbs mode:%d\r\n",atoi(strCmd));
		UIAPPIPCAM_DEPTH_CONFIG *pDepthInfo =  UIAppIPCam_get_DepthInfo();
		pDepthInfo->bSBSView = atoi(strCmd);
		return TRUE;
}

SXCMD_BEGIN(ipcam_depth, "ipcam_depth")
SXCMD_ITEM("sbs_mode %", sbs_mode, "set right view or depth view")
SXCMD_END()
#endif
static void IPCam_DepthInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	#ifdef IPCAM_DEPTH_DEBUG
	SxCmd_AddTable(ipcam_depth);
	#endif

	//debug_msg("^YPhoto_DepthInit\r\n");
	//debug_msg("WorkBuf=0x%x, size=0x%x\r\n", buf->Addr, buf->Size);
	UIAPPIPCAM_DEPTH_CONFIG *pDepthInfo =  UIAppIPCam_get_DepthInfo();
	DEPTH_INIT_PARAM  depth_initobj;
	memset((void *)&depth_initobj, 0, sizeof(DEPTH_INIT_PARAM));
	depth_initobj.buf = *buf;
	depth_initobj.cachebuf = *cachebuf;
	depth_initobj.disp_mode = pDepthInfo->sDepthInfo.disp_mode;
	depth_initobj.ntr_on = pDepthInfo->sDepthInfo.ntr_on;
	depth_initobj.inval_show = pDepthInfo->sDepthInfo.inval_show;
	Depth_Init(&depth_initobj);
}

static UINT32 IPCam_DepthCalcBuffSize(void)
{
	UINT32 size = 0;
	size = Depth_CalcBuffSize(640, 480, MAX_DISP_D128);
	//size = Depth_GetBuffSize();
	return size;
}

static UINT32 IPCam_DepthCalcCacheBuffSize(void)
{
	UINT32 size = 0;
	size = Depth_CalcCacheBuffSize(640, 480, MAX_DISP_D128);
	return size;
}



static INT32 IPCam_DepthSBS(IPCAM_VIEW_DRAW *pDraw)
{

	IRECT     dst_region;



    GxImg_FillData(pDraw->p_dst_img, REGION_MATCH_IMG, COLOR_YUV_BLACK);
	// img1
	dst_region.x = 0;
	dst_region.y = 0;
	dst_region.w = pDraw->p_dst_img->Width / 2;
	dst_region.h = pDraw->p_dst_img->Height;
	GxImg_ScaleData(pDraw->p_src_img[0], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

    // img2
    dst_region.x = pDraw->p_dst_img->Width / 2;
	dst_region.y = 0;

	GxImg_SetParm(GXIMG_PARM_SCALE_METHOD, GXIMG_SCALE_NEAREST);
	GxImg_ScaleData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);
	GxImg_SetParm(GXIMG_PARM_SCALE_METHOD, GXIMG_SCALE_AUTO);


	return E_OK;
}

static void IPCam_DepthProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	ISF_DATA         isf_data1, isf_data2;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc1, *pSrc2;
	pUnit = &ISF_UserProc;
	UIAPPIPCAM_DEPTH_CONFIG *pDepthInfo =  UIAppIPCam_get_DepthInfo();
	if(pDepthInfo->bEnable == FALSE){
		return;
	}
	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(pDepthInfo->sInPath1+ISF_OUT_BASE, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
		ImageUnit_SetParam(pDepthInfo->sInPath2+ISF_OUT_BASE, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc1 = ImageUnit_Out(pUnit, pDepthInfo->sInPath1+ISF_OUT_BASE);
	pSrc2 = ImageUnit_Out(pUnit, pDepthInfo->sInPath2+ISF_OUT_BASE);
	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_IsAllowPull(pSrc2)) {
		if (ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) != ISF_OK) {
			return;
		}
		if (ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) != ISF_OK) {
			ImageUnit_ReleaseData(&isf_data1);
			return;
		}
		/////first input --> left , second input -->right
		Depth_Process((IMG_BUF *)&isf_data2.Desc[0], (IMG_BUF *)&isf_data1.Desc[0]);


		ISF_DATA            isf_new_data;
		ISF_PORT            *pDispSrc;
		IMG_BUF             dst_img;
		UINT32              img_buf_size , buff_addr, dst_w, dst_h, dst_pxlfmt;
		IPCAM_VIEW_DRAW   appdisp_draw;
		IPCAM_VIEW_DRAW   *pdraw = &appdisp_draw;
		STEREO_DISP_RESULT img_depth;
		img_depth.show_case = pDepthInfo->bSBSView;
		Depth_GetDispResult(&img_depth);

        pDispSrc = ImageUnit_Out(pUnit, ISF_OUT1 + pDepthInfo->sOutPath);
		if(pDispSrc == NULL){
			ImageUnit_ReleaseData((ISF_DATA *)&isf_data1);
			ImageUnit_ReleaseData((ISF_DATA *)&isf_data2);
			return;
		}
		ImageUnit_Begin(pUnit, 0);
			ImageUnit_SetParam(pDepthInfo->sOutPath+ISF_OUT_BASE, USERPROC_PARAM_BYPASS_IMM, FALSE);
		ImageUnit_End();
		ImageUnit_GetVdoImgSize(pUnit, pDispSrc->oPort,&dst_w, &dst_h);
	    dst_pxlfmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
		img_buf_size = GxImg_CalcRequireSize(dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16));
		buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
		if (buff_addr == 0) {
			DBG_ERR("Get buffer fail\r\n");
			ImageUnit_ReleaseData((ISF_DATA *)&isf_data1);
			ImageUnit_ReleaseData((ISF_DATA *)&isf_data2);
			return;
		}
		GxImg_InitBuf(&dst_img, dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
		memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));
		////need set left
		pdraw->p_src_img[0] = (IMG_BUF *)&isf_data2.Desc[0];
		pdraw->p_src_img[1] = &img_depth.disp_out;
		pdraw->p_dst_img = &dst_img;
		IPCam_DepthSBS(pdraw);
		ImageUnit_PushData(pDispSrc, (ISF_DATA *)&isf_new_data, 0);
		ImageUnit_ReleaseData(&isf_new_data);

		// process end , release buffer
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data1);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data2);
		//DBG_WRN("charlie finish!\r\n");
	}
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcamdepth = {
	{
		PHOTO_PRIORITY_H,       ///< function hook process Id.
		IPL_PATH_2,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"IPCam_DepthProcess",
		IPCam_DepthInit,           ///< init fp, only execute at first process
		IPCam_DepthProcess,        ///< process fp
		NULL,                      ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		IPCam_DepthCalcBuffSize,        ///< get working buffer fp
		IPCam_DepthCalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif


#if(IPCAM_RAW_DATA == ENABLE)

static void IPCam_RAWInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	return;
}

static void IPCam_RAWProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc1;
	INT32 iRawPath = UIAppIPCam_get_RAWPath();
	IMG_BUF 		*pISF_buf = NULL;
	pUnit = &ISF_UserProc;
	if(iRawPath < 0){

		DBG_ERR("Rawpath:%d not enable\r\n",iRawPath);
		return;
	}

	ImageUnit_Begin(pUnit, 0);
		ImageUnit_SetParam(iRawPath+ISF_OUT_BASE, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc1 = ImageUnit_Out(pUnit, iRawPath+ISF_OUT_BASE);
	if (ImageUnit_IsAllowPull(pSrc1)) {
		if (ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data, 0) != ISF_OK) {
			return;
		}

		pISF_buf = (IMG_BUF *)&isf_data.Desc[0];
	/*
		width = raw width
		height = raw height
		PxlFmt = IPCAM_FMT_RAW12,IPCAM_FMT_RAW8,IPCAM_FMT_RAW10,IPCAM_FMT_RAW16
		PxlAddr[0] = raw address
		LineOffset[0] = raw line offset
		PxlAddr[1] = start_pixel (_IPL_RAW_PIX)

	*/
		char *tmpBuf = (char *)pISF_buf->PxlAddr[0];
		static int count =0;
		DBG_DUMP("count:%d w:%d h:%d fmt:%d addr:%x %x %x %x lineoffset:%x start pixel:%x\r\n",
			count,pISF_buf->Width,pISF_buf->Height,pISF_buf->PxlFmt,pISF_buf->PxlAddr[0],
		*(tmpBuf),*(tmpBuf+1),*(tmpBuf+2),
		pISF_buf->LineOffset[0],pISF_buf->PxlAddr[1]);

		if(count == 10){
			UINT32 buf_size = (ALIGN_CEIL_8(pISF_buf->Width*12) * pISF_buf->Height / 8);
			DBG_DUMP("start write to file size:%d lineoff*h=%d\r\n", buf_size, pISF_buf->LineOffset[0]*pISF_buf->Height);
			FST_FILE     filehdl = NULL;
			char path[64]={0};
			sprintf(path,"A:\\raw\\raw_data%d",count);

			filehdl = FileSys_OpenFile(path, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
			if (filehdl) {
				FileSys_WriteFile(filehdl, (UINT8 *)tmpBuf, &buf_size, 0, NULL);

				FileSys_CloseFile(filehdl);

			} else {
				DBG_ERR("file open error!!\r\n");
			}
		}
		count ++;
		ImageUnit_ReleaseData(&isf_data);

	}
}
PHOTO_FUNC_INFO PhotoFuncInfo_ipcamraw = {
	{
		PHOTO_PRIORITY_H,       ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"IPCam_RAWProcess",
		IPCam_RAWInit,           ///< init fp, only execute at first process
		IPCam_RAWProcess,        ///< process fp
		NULL,                      ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		NULL,        ///< get working buffer fp
		NULL,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif


void IPCam_InitPhotoFunc(void)
{
	extern PHOTO_FUNC_INFO PhotoFuncInfo_dummy;
	//extern PHOTO_FUNC_INFO PhotoFuncInfo_dis;

	PHOTO_FUNC_INFO *phead = NULL;
	//
	// Setup IPL information.
	//
	phead = &PhotoFuncInfo_dummy;
	phead->pNext = NULL; //mark end

//#if IPCAM_IN_DIS_FUNC
#if IPCAM_DIS_MV_FUNC
	{
		// DIS function
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamdis;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}


#endif


#if IPCAM_FD_FUNC
	{

		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamfd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

	}
	//IP cam will unlock FD by CPU_CORE2
	FD_Lock(FALSE);

#endif


	//#NT#2017/04/28#KCHong -begin
	//#NT# CNN Object detection
#if (IPCAM_OD_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamod;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    /// find last func
		}
		pcurr->pNext = pfunc;                                           /// append this func
		pfunc->pNext = NULL;                                            /// mark end
		//PhotoFuncInfo_ipcamod.Info.IPLId = IPL_PATH_1;
	}
#endif
	//#NT#2017/04/28#KCHong -end

	//#NT#2018/05/04#Bin Xiao -begin
	//#NT# Face Detection by CNN
#if (MOVIE_FDCNN_FUNC == ENABLE)
		{
			PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvfdcnn;
			PHOTO_FUNC_INFO *pcurr = 0;
			pcurr = phead;
			while (pcurr->pNext) {
				pcurr = pcurr->pNext;	 /// find last func
			}
			pcurr->pNext = pfunc;											/// append this func
			pfunc->pNext = NULL;											/// mark end
			//PhotoFuncInfo_mvfdcnn.Info.IPLId = IPL_PATH_1;
		}
#endif
	//#NT#2018/05/04#Bin Xiao -end


	//#NT#2018/05/18#Dan Guo -begin
	//#NT# Face recogniton
#if (MOVIE_FR_FUNC == ENABLE)
		{
			PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvfr;
			PHOTO_FUNC_INFO *pcurr = 0;
			pcurr = phead;
			while (pcurr->pNext) {
				pcurr = pcurr->pNext;	 /// find last func
			}
			pcurr->pNext = pfunc;											/// append this func
			pfunc->pNext = NULL;											/// mark end
			//PhotoFuncInfo_mvfr.Info.IPLId = IPL_PATH_1;
		}

#endif
	//#NT#2018/05/18#Dan Guo -end



	//#NT#2018/05/04#Bin Xiao -begin
	//#NT# Object detection&DIS Tracking
#if (MOVIE_ODT_FUNC == ENABLE)
		{
			PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvodt;
			PHOTO_FUNC_INFO *pcurr = 0;
			pcurr = phead;
			while (pcurr->pNext) {
				pcurr = pcurr->pNext;	 /// find last func
			}
			pcurr->pNext = pfunc;											/// append this func
			pfunc->pNext = NULL;											/// mark end
			//PhotoFuncInfo_mvodt.Info.IPLId = IPL_PATH_1;
		}

		{
			PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvodtod;
			PHOTO_FUNC_INFO *pcurr = 0;
			pcurr = phead;
			while (pcurr->pNext) {
				pcurr = pcurr->pNext;	 /// find last func
			}
			pcurr->pNext = pfunc;											/// append this func
			pfunc->pNext = NULL;											/// mark end
			//PhotoFuncInfo_mvodtod.Info.IPLId = IPL_PATH_1;
		}

#endif
	//#NT#2018/05/04#Bin Xiao -end




	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcam_motion;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}


	//Support object tracking function
#if MOVIE_OT_FUNC
	{
		//UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvot;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end
		//#NT#2018/01/22#Charlie Chang -begin
       	//#NT# please set it in the PhotoFuncInfo_mvot table
#if 0
		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvot.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvot.Info.IPLId = IPL_PATH_2;
		}
#endif
		//#NT#2018/01/22#Charlie Chang -end
	}
#endif
	//Support Dis dsp function

	//#NT#2017/10/05#Charlie Chang -begin
	//#NT# Support CEVA Deep Neural Network (CDNN)
#if (IPCAM_CDNN_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamcdnn;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   // find last func
		}
		pcurr->pNext = pfunc;       // append this func
		pfunc->pNext = NULL;        // mark end

	}
#endif

#if (IPCAM_CDNN2_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamcdnn2;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;   // find last func
		}
		pcurr->pNext = pfunc;       // append this func
		pfunc->pNext = NULL;        // mark end

	}
#endif
	//#NT#2017/10/05#Charlie Chang -end

	//#NT#2016/10/27#Omega Yu -begin
	//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
	{
		//UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_mvscd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end
		//#NT#2018/01/22#Charlie Chang -begin
       	//#NT# please set it in the PhotoFuncInfo_mvscd table
#if 0
		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_2;
		}
#endif
	}

#endif
	//#NT#2016/10/27#Omega Yu -end


#if IPCAM_BC_FUNC
	{
//		UINT32		myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcambc;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end
		//#NT#2018/01/22#Charlie Chang -begin
       	//#NT# please set it in the PhotoFuncInfo_ipcambc table
#if 0
		myDualCam = Photo_getDualCamSetting();
		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_ipcambc.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_ipcambc.Info.IPLId = IPL_PATH_2;
		}
#endif
	}

#if (IPCAM_BC_FUNC == ENABLE && IPCAM_FUNC == ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_vqa;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
	}
#endif


#if (IPCAM_DEPTH_FUNC == ENABLE)

	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamdepth;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end
	}
#endif

#endif

	//#NT#2018/03/19#Wayne Wei -begin
	//#NT# Support License Plate Recognition (LPR)
#if (IPCAM_LPR_FUNC==ENABLE)
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamlpr;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

	}
#endif
	//#NT#2018/03/19#Wayne Wei -end


#if (IPCAM_USER_PULL_DATA == ENABLE)
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_USER_PULL_DATA *pPullDataInfo = UIAppIPCam_get_UserPullDataInfo();
	UINT32 uiALGPath = UIAppIPCam_get_ALGPath();
	UINT32 i=0;
	PHOTO_FUNC_INFO *PullList[IPCAM_PATH_MAX_ID] ={
		&PhotoFuncInfo_ipcamPull1,
		&PhotoFuncInfo_ipcamPull2,
		&PhotoFuncInfo_ipcamPull3,
		&PhotoFuncInfo_ipcamPull4,
		&PhotoFuncInfo_ipcamPull5,
		&PhotoFuncInfo_ipcamPull6,
		&PhotoFuncInfo_ipcamPull7,
		&PhotoFuncInfo_ipcamPull8,
		&PhotoFuncInfo_ipcamPull9,
		&PhotoFuncInfo_ipcamPull10,
		&PhotoFuncInfo_ipcamPull11,
		&PhotoFuncInfo_ipcamPull12,
		&PhotoFuncInfo_ipcamPull13,
		&PhotoFuncInfo_ipcamPull14,
		&PhotoFuncInfo_ipcamPull15,
		&PhotoFuncInfo_ipcamPull16
	};
	for(i=0; i < IPCAM_PATH_MAX_ID ; i++){

		if((i== uiALGPath) || (pStreamInfo[i].enable == IPCAM_STATE_DISABLE)){
			continue;
		}
		if(pPullDataInfo[i].bPullData == TRUE){
			PHOTO_FUNC_INFO *pfunc = PullList[i];
			PHOTO_FUNC_INFO *pcurr = 0;
			pcurr = phead;
			while (pcurr->pNext) {
				pcurr = pcurr->pNext;	 //find last func
			}
			pcurr->pNext = pfunc; //append this func
			pfunc->pNext = NULL; // mark end
		}

	}
#endif

#if (IPCAM_DEPTH_FUNC == ENABLE)

	{
//		UINT32		myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamdepth;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

	}
#endif

#if(IPCAM_RAW_DATA == ENABLE)

	{
//		UINT32		myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_ipcamraw;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;	 //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; // mark end

	}
#endif

	UIPhotoFunc_Open(phead);
}
