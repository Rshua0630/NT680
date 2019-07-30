#include <stdio.h>
#include <string.h>
#include "Color.h"
#include "SysCfg.h"
#include "GxImage.h"
#include "MovieStamp.h"
#include "PrjCfg.h"
//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
#include "display.h"
#include "UIFlow.h"
#include "NvtOt.h"
#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2017/11/3#Ming Yang -begin
//#NT# Object detection
#if (MOVIE_OD_FUNC || IPCAM_OD_FUNC)
//#include "display.h"
//#include "UIFlow.h"
//#include "NvtCNN.h"
#include "NvtNNFlow.h"
#endif
//#NT#2017/11/3#Ming Yang -end

//#NT#2017/09/27#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#if MOVIE_ODT_FUNC
#include "display.h"
//#include "UIFlow.h"
#include "odt_lib.h"
#endif
//#NT#2017/09/27#Bin Xiao -end

//#NT#2018/03/12#Dan Guo -begin
//#NT# Face recognition
#if MOVIE_FR_FUNC
#include "display.h"
//#include "UIFlow.h"
#include "FR_lib.h"
#endif



//#NT#2018/01/29#Bin Xiao -begin
//#NT# Face Detection by CNN
#if MOVIE_FDCNN_FUNC
#include "display.h"
//#include "UIFlow.h"
#include "fdcnn_lib.h"
#endif
//#NT#2018/01/29#Bin Xiao -end

//#NT#2017/12/06#WY -begin
//#NT#
#if MOVIE_ODP_FUNC
#include "display.h"
#include "UIFlow.h"
#include "NvtNNFlow.h"
#endif
//#NT#2017/12/06#WY -end


#if MOVIE_TWD_FUNC
#include "TWD_lib.h"
#endif

#if MOVIE_TZD_FUNC
#include "TZD_lib.h"
#endif

#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
#include "DDD_lib.h"
#endif

#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#endif

#if (MOVIE_TSR_FUNC_)
#include "tsr_lib.h"
#include "TSD_DSP_lib.h"
#include "Color.h"
#endif

#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#endif



#define __MODULE__              alg_MovieStamp
#define __DBGLVL__              2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__              "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#if (MOVIE_FD_FUNC_ == ENABLE || IPCAM_FD_FUNC == ENABLE)
void MovieStamp_DrawFD(PIMG_BUF pDstImg)
{

#if (MOVIE_FD_DRAW_VIDEO == ENABLE || IPCAM_FD_DRAW_VIDEO == ENABLE)

	UINT32            Fdnumber, i;
	static FD_FACE    FdInfo[MAX_FDNUM];
	IRECT             DrawRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_WHITE;
	URECT             DispCord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	Fdnumber = FD_GetRsltFace(FdInfo, &DispCord);
	if (Fdnumber > MAX_FDNUM) {
		Fdnumber = MAX_FDNUM;
	}
	if (Fdnumber) {
		for (i = 0; i < Fdnumber; i++) {
			DrawRect.x = FdInfo[i].x;
			DrawRect.y = FdInfo[i].y;
			DrawRect.w = FdInfo[i].w;
			DrawRect.h = FdInfo[i].h;
			MovieStamp_DrawRect(pDstImg, &DrawRect, drawColor, border);
		}
	}
#endif
}
#endif

#if MOVIE_OT_FUNC
void MovieStamp_DrawOT(PIMG_BUF pDstImg)
{
#if MOVIE_OT_DRAW_VIDEO
	UINT32            OTnumber;
	URECT             OTInfo;
	IRECT             OTRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_WHITE;
	URECT             DispCord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	OTnumber = OT_GetTarget(&OTInfo, &DispCord);
	if (OTnumber) {
		OTRect.x = OTInfo.x;
		OTRect.y = OTInfo.y;
		OTRect.w = OTInfo.w;
		OTRect.h = OTInfo.h;
		MovieStamp_DrawRect(pDstImg, &OTRect, drawColor, border);
	}
#endif
}
#endif

//#NT#2017/11/3#Ming Yang -begin
//#NT# Object detection
#if (MOVIE_OD_FUNC || IPCAM_OD_FUNC)
void MovieStamp_DrawOD(PIMG_BUF pDstImg)
{
#if (MOVIE_OD_DRAW_VIDEO || IPCAM_OD_DRAW_VIDEO)
	UINT32            ODnumber;
	static NN_Result  od_result[MAX_DETECTED_OBJECT] = {0};
	IRECT             ODRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_YELLOW;
	URECT             DispCord;
	UINT32            i = 0;
	//UINT32            isODFirst;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
    //isODFirst = NN_GetODFirstFlag();
    //DBG_DUMP("isODFirst = %d\r\n",isODFirst);
    //if(isODFirst==0)
    //{
    	ODnumber = NN_GetResult(od_result, &DispCord);
        DBG_DUMP("ODNumber = %d\r\n",ODnumber);
    	for (i = 0; i < ODnumber; i++) {
    		ODRect.x = od_result[i].x;
    		ODRect.y = od_result[i].y;
    		ODRect.w = od_result[i].w;
    		ODRect.h = od_result[i].h;

            switch (od_result[i].class) {
    			case 0: 	// Bus (blue)
                    drawColor = COLOR_YUV_BLUE;
    				break;
    			case 1: 	// Car (white)
                    drawColor = COLOR_YUV_WHITE;
    				break;
    			case 2: 	// Human (red)
                    drawColor = COLOR_YUV_RED;
    				break;
    			default:	// Others (yellow)
    			    drawColor = COLOR_YUV_YELLOW;
    				break;
    		}

    		MovieStamp_DrawRect(pDstImg, &ODRect, drawColor, border);
    	}
    //}
#endif
}
#endif
//#NT#2017/11/3#Ming Yang -end

//#NT#2017/09/27#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#if MOVIE_ODT_FUNC
void MovieStamp_DrawODT(PIMG_BUF pDstImg)
{
#if MOVIE_ODT_DRAW_VIDEO
	UINT32            ODTnumber;
	static odt_obj    ODTInfo[ODT_NUM_MAX];
	IRECT             ODTRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_WHITE;
	URECT             DispCord;
	UINT32            i = 0, j = 0;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	for(j = 0; j < ODT_PROCESS_NUM_MAX; j++)
	{
		ODTnumber = odt_getrsltobj_for_draw(ODTInfo, j, &DispCord);
		for (i = 0; i < ODTnumber; i++) {
			ODTRect.x = ODTInfo[i].obj_rect.x;
			ODTRect.y = ODTInfo[i].obj_rect.y;
			ODTRect.w = ODTInfo[i].obj_rect.w;
			ODTRect.h = ODTInfo[i].obj_rect.h;
			MovieStamp_DrawRect(pDstImg, &ODTRect, drawColor, border);
		}
	}

#endif
}
#endif
//#NT#2017/09/27#Bin Xiao -end


//#NT#2018/03/12#Dan Guo -begin
//#NT# Face recognition
#if MOVIE_FR_FUNC
void MovieStamp_DrawFR(PIMG_BUF pDstImg)
{

#if MOVIE_FR_DRAW_VIDEO

    //debug_msg("MovieStamp_DrawFR");
	UINT32            FDnumber;
	static FRCNN_result     FRresult[MAX_NUM]= {0};
	static IRECT            FDRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_WHITE;
	UINT32            i = 0;
	static char 	   string[100];


    UPOINT Pos={0};


	static URECT			  DispCord ;
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	if (pDstImg->Width < 1280) {
		border = 2;
	}

	FDnumber = FR_getrsltobj_for_draw(FRresult,&DispCord);

	UINT32 FR_state = 0;
    FR_state = FR_reg_get();

	if(FR_state)
	{

			FDnumber = 0;
		    FDRect.x = 735;FDRect.y = 345;
		    //FDRect.w = 450;FDRect.h = 390;
		    FDRect.w = 390;FDRect.h = 450;
			
			//MovieStamp_DrawRect(pDstImg, &FDRect, drawColor, border);

			Pos.x = FDRect.x;
		    Pos.y = FDRect.y;
		    sprintf(string, "keep face in the box");
			MovieStamp_DrawRect(pDstImg, &FDRect, drawColor, border);
			MovieStamp_DrawFaceName(pDstImg,0,&Pos,string);
			

	}
				
	for (i = 0; i < FDnumber; i++) {
		FDRect.x = FRresult[i].x;
		FDRect.y = FRresult[i].y;
		FDRect.w = FRresult[i].w;
		FDRect.h = FRresult[i].h;
		Pos.x = FRresult[i].x;
		Pos.y = FRresult[i].y;
		sprintf(string, FRresult[i].name);
		MovieStamp_DrawRect(pDstImg, &FDRect, drawColor, border);
        MovieStamp_DrawFaceName(pDstImg,0,&Pos,string);
		
	}

#endif

}
#endif
//#NT#2018/03/12#Dan Guo -end


//#NT#2018/01/29#Bin Xiao -begin
//#NT# Face Detection by CNN
#if MOVIE_FDCNN_FUNC
void MovieStamp_DrawFDCNN(PIMG_BUF pDstImg)
{
#if MOVIE_FDCNN_DRAW_VIDEO
	UINT32            	FDCNNnumber;
	static fdcnn_result FDCNNInfo[FDCNN_MAX_OUTNUM];
	IRECT             	FDCNNRect;
	UINT32            	border = 4; // border can not smaller than 2
	UINT32            	drawColor = COLOR_YUV_WHITE;
	URECT             	DispCord;
	UINT32            	i = 0;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	FDCNNnumber = fdcnn_getresults(FDCNNInfo, &DispCord);
	for (i = 0; i < FDCNNnumber; i++) {
		FDCNNRect.x = FDCNNInfo[i].x;
		FDCNNRect.y = FDCNNInfo[i].y;
		FDCNNRect.w = FDCNNInfo[i].w;
		FDCNNRect.h = FDCNNInfo[i].h;
		MovieStamp_DrawRect(pDstImg, &FDCNNRect, drawColor, border);
	}
#endif
}
#endif
//#NT#2018/01/29#Bin Xiao -end

//#NT#2017/12/06#WY -begin
//#NT#
#if MOVIE_ODP_FUNC
void MovieStamp_DrawODP(PIMG_BUF pDstImg)
{
#if MOVIE_ODP_DRAW_VIDEO
	UINT32            ODnumber;
	NN_Result         od_result[MAX_DETECTED_OBJECT] = {0};
	IRECT             ODRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_WHITE;
	URECT             DispCord;
	UINT32            i = 0;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	ODnumber = NN_GetResult(od_result, &DispCord);
	for (i = 0; i < ODnumber; i++) {
		ODRect.x = od_result[i].x;
		ODRect.y = od_result[i].y;
		ODRect.w = od_result[i].w;
		ODRect.h = od_result[i].h;
		MovieStamp_DrawRect(pDstImg, &ODRect, drawColor, border);
	}
#endif
}
#endif
//#NT#2017/12/06#WY -end





#if MOVIE_DDD_FUNC
void MovieStamp_DrawDDD(UINT32 uiVidEncId, PIMG_BUF pDstImg)
{
#if MOVIE_DDD_DRAW_VIDEO
	//draw variables
	URECT DispCord;
	UINT32 ColorKey;

	//DDD variables
	DDD_SHAPE Shape_Landmark;
	URECT TrackRect;
	UINT32 Dddnumber;
	INT32 iPoseRay[8];
	UINT32 Ret_DDD;

	//other variables
	UINT32 i;
	UINT32 t1, t2;

	//#NT#2016/09/20#ML Cui -begin
	//#NT# DDD need not to draw on front channel.
	//#NT# It was not worked as wish in PTL668 latest HW.
#if 1
	if (uiVidEncId == 0) {
		return;
	}
#endif
	//#NT#2016/09/20#ML Cui -end

	t1 = Perf_GetCurrent();
	//-------------------------
	if (pDstImg->PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
		DBG_ERR("Not YUV420\r\n");
		return;
	}

	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;

	//draw landmark
	Dddnumber = DDD_GetRsltLandmark(&Shape_Landmark, &DispCord);
	if (Dddnumber > 0) {
		//landmark: head
		Ret_DDD = DDD_GetFacialState(DDD_FACIAL_TYPE_HEAD);
		if (DDD_HEAD_STATE_NODING == Ret_DDD) {
			ColorKey = COLOR_YUV_ORANGE;
		} else if (DDD_HEAD_STATE_DISTRIBUTE == Ret_DDD) {
			ColorKey = COLOR_YUV_RED;
		} else {
			ColorKey = COLOR_YUV_GREEN;
		}

		for (i = 0; i < 17; i++) {
			MovieStamp_DrawPointUV(Shape_Landmark.pt[i * 2], Shape_Landmark.pt[i * 2 + 1], ColorKey, pDstImg);
		}

		//landmark: others
		ColorKey = COLOR_YUV_GREEN;

		for (i = 17; i < 36; i++) {
			MovieStamp_DrawPointUV(Shape_Landmark.pt[i * 2], Shape_Landmark.pt[i * 2 + 1], ColorKey, pDstImg);
		}

		//landmark: left eye
		if (DDD_GetFacialState(DDD_FACIAL_TYPE_LEFT_EYE) == DDD_EYE_CLOSED) {
			ColorKey = COLOR_YUV_BLUE;
		} else {
			ColorKey = COLOR_YUV_GREEN;
		}

		for (i = 36; i < 42; i++) {
			MovieStamp_DrawPointUV(Shape_Landmark.pt[i * 2], Shape_Landmark.pt[i * 2 + 1], ColorKey, pDstImg);
		}

		//landmark: right eye
		if (DDD_GetFacialState(DDD_FACIAL_TYPE_RIGHT_EYE) == DDD_EYE_CLOSED) {
			ColorKey = COLOR_YUV_BLUE;
		} else {
			ColorKey = COLOR_YUV_GREEN;
		}

		for (i = 42; i < 48; i++) {
			MovieStamp_DrawPointUV(Shape_Landmark.pt[i * 2], Shape_Landmark.pt[i * 2 + 1], ColorKey, pDstImg);
		}

		//landmark: mouse
		if (DDD_GetFacialState(DDD_FACIAL_TYPE_MOUTH) == DDD_MOUTH_YAWN) {
			ColorKey = COLOR_YUV_CYAN;
		} else {
			ColorKey = COLOR_YUV_GREEN;
		}

		for (i = 48; i < 68; i++) {
			MovieStamp_DrawPointUV(Shape_Landmark.pt[i * 2], Shape_Landmark.pt[i * 2 + 1], ColorKey, pDstImg);
		}

		//poseray
		if (DDD_GetRsltPoseRay(iPoseRay, 60, &DispCord)) {
			MovieStamp_DrawLineUV(iPoseRay[0], iPoseRay[1], iPoseRay[2], iPoseRay[3], COLOR_YUV_GREEN, pDstImg);
			MovieStamp_DrawLineUV(iPoseRay[0], iPoseRay[1], iPoseRay[4], iPoseRay[5], COLOR_YUV_BLUE, pDstImg);
			MovieStamp_DrawLineUV(iPoseRay[0], iPoseRay[1], iPoseRay[6], iPoseRay[7], COLOR_YUV_RED, pDstImg);
		}

		//tracking rect
		if (DDD_GetRsltTrackingRect(&TrackRect, &DispCord)) {
			MovieStamp_DrawRectUV((IRECT *)&TrackRect, COLOR_YUV_PURPLE, pDstImg);
		}
	}

	//-------------------------
	t2 = Perf_GetCurrent();
	DBG_IND("DrawDDD = %d ms\r\n", (t2 - t1) / 1000);
#endif
}
#endif


#if (_ADAS_FUNC_ == ENABLE)
void MovieStamp_DrawADAS(UINT32 uiVidEncId, PIMG_BUF pDstImg)
{
#if (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
	ADAS_APPS_RESULT_INFO *pAdasRlt = alg_MovieExe_GetAdasRltVideo();

	if ((uiVidEncId != 0) || (ADAS_GetAppsStatus() == FALSE)) {
		return;
	}

	// draw lane
	MovieStamp_DrawLineUV(pAdasRlt->LdwsRsltInfo.uiLx1, pAdasRlt->LdwsRsltInfo.uiLy1,
						  pAdasRlt->LdwsRsltInfo.uiLx2, pAdasRlt->LdwsRsltInfo.uiLy2,
						  (pAdasRlt->LdwsRsltInfo.DepartureDirVideo == LDWS_DEPARTURE_LEFT) ? COLOR_YUV_RED : COLOR_YUV_GREEN, pDstImg);
	MovieStamp_DrawLineUV(pAdasRlt->LdwsRsltInfo.uiRx1, pAdasRlt->LdwsRsltInfo.uiRy1,
						  pAdasRlt->LdwsRsltInfo.uiRx2, pAdasRlt->LdwsRsltInfo.uiRy2,
						  (pAdasRlt->LdwsRsltInfo.DepartureDirVideo == LDWS_DEPARTURE_RIGHT) ? COLOR_YUV_RED : COLOR_YUV_GREEN, pDstImg);

	// draw car underline
	if ((pAdasRlt->FcwsRsltInfo.uiLx != pAdasRlt->FcwsRsltInfo.uiRx) || (pAdasRlt->FcwsRsltInfo.uiLy != pAdasRlt->FcwsRsltInfo.uiRy)) {
		MovieStamp_DrawLineUV(pAdasRlt->FcwsRsltInfo.uiLx, pAdasRlt->FcwsRsltInfo.uiLy,
							  pAdasRlt->FcwsRsltInfo.uiRx, pAdasRlt->FcwsRsltInfo.uiRy,
							  (pAdasRlt->FcwsRsltInfo.FCVideoWarning == TRUE) ? COLOR_YUV_RED : COLOR_YUV_GREEN, pDstImg);
	}

	// draw Auto-VP result
	MovieStamp_DrawLineUV((pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x - 30), pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y,
						  (pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x + 30), pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y,
						  (pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) ? COLOR_YUV_BLUE : COLOR_YUV_RED, pDstImg);
	MovieStamp_DrawLineUV(pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x, (pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y - 30),
						  pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x, (pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y + 30),
						  (pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) ? COLOR_YUV_BLUE : COLOR_YUV_RED, pDstImg);

#if _TODO   // [ADAS_TODO] (_SNG_FUNC_ == ENABLE)
	// draw SnGROI
	ADAS_SNG_ROI_INFO *pSnGROI = alg_MovieExe_GetSnGROIVideo();

	if (pSnGROI) {
		MovieStamp_DrawLineUV(pSnGROI->uiP1x, pSnGROI->uiP1y, pSnGROI->uiP2x, pSnGROI->uiP2y,
							  (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID) ? COLOR_YUV_GREEN : COLOR_YUV_RED, pDstImg);
		MovieStamp_DrawLineUV(pSnGROI->uiP2x, pSnGROI->uiP2y, pSnGROI->uiP3x, pSnGROI->uiP3y,
							  (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID) ? COLOR_YUV_GREEN : COLOR_YUV_RED, pDstImg);
		MovieStamp_DrawLineUV(pSnGROI->uiP3x, pSnGROI->uiP3y, pSnGROI->uiP4x, pSnGROI->uiP4y,
							  (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID) ? COLOR_YUV_GREEN : COLOR_YUV_RED, pDstImg);
		MovieStamp_DrawLineUV(pSnGROI->uiP4x, pSnGROI->uiP4y, pSnGROI->uiP1x, pSnGROI->uiP1y,
							  (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID) ? COLOR_YUV_GREEN : COLOR_YUV_RED, pDstImg);
	}
#endif  // #if (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
#endif  // #if (_ADAS_FUNC_ == ENABLE)
}
#endif


//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection function
#if MOVIE_TWD_FUNC
void MovieStamp_DrawTWD(PIMG_BUF pDstImg)
{
#if MOVIE_TWD_DRAW_VIDEO
	UINT32            TWDRECTnumber, TWDPOINTnumber, i;
	URECT             TWDInfo[twd_max_objects_number];
	BOOL              TWDState[twd_max_objects_number];
	UPOINT            TWDLINEInfo[twd_max_nodes_number];
	UPOINT TWDStart, TWDEnd;
	IRECT  TWDRect;
	UINT32 border = 4; // border can not smaller than 2
	UINT32 drawColor = COLOR_YUV_RED;
	URECT  DispCord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;

	if ((TWDPOINTnumber = twd_getpointsintripwire(TWDLINEInfo, &DispCord)) > 1) {
		for (i = 0; i < TWDPOINTnumber - 1; i++) {
			TWDStart.x = TWDLINEInfo[i].x;
			TWDStart.y = TWDLINEInfo[i].y;
			TWDEnd.x = TWDLINEInfo[i + 1].x;
			TWDEnd.y = TWDLINEInfo[i + 1].y;
			MovieStamp_DrawLineUV(TWDStart.x, TWDStart.y, TWDEnd.x, TWDEnd.y, drawColor, pDstImg);
		}
	}

	if ((TWDRECTnumber = twd_gettargetswithwarn(TWDInfo, TWDState, &DispCord)) > 0) {
		for (i = 0; i < TWDRECTnumber; i++) {
			TWDRect.x = TWDInfo[i].x;
			TWDRect.y = TWDInfo[i].y;
			TWDRect.w = TWDInfo[i].w;
			TWDRect.h = TWDInfo[i].h;

			if (TWDState[i] == FALSE) {
				drawColor = COLOR_YUV_WHITE;
			} else {
				drawColor = COLOR_YUV_RED;
			}

			MovieStamp_DrawRect(pDstImg, &TWDRect, drawColor, border);
		}
	}
#endif
}
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-zone detection function
#if MOVIE_TZD_FUNC
void MovieStamp_DrawTZD(PIMG_BUF pDstImg)
{
#if MOVIE_TZD_DRAW_VIDEO
	UINT32            TZDRECTnumber, TZDPOINTnumber, i;
	URECT             TZDInfo[tzd_max_objects_number];
	BOOL              TZDState[tzd_max_objects_number];
	UPOINT            TZDLINEInfo[tzd_max_nodes_number];
	UPOINT TZDStart, TZDEnd;
	IRECT  TZDRect;
	UINT32 border = 4; // border can not smaller than 2
	UINT32 drawColor = COLOR_YUV_RED;
	URECT  DispCord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;

	if ((TZDPOINTnumber = tzd_getpointsintripzone(TZDLINEInfo, &DispCord)) > 1) {
		for (i = 0; i < TZDPOINTnumber - 1; i++) {
			TZDStart.x = TZDLINEInfo[i].x;
			TZDStart.y = TZDLINEInfo[i].y;
			TZDEnd.x = TZDLINEInfo[i + 1].x;
			TZDEnd.y = TZDLINEInfo[i + 1].y;
			MovieStamp_DrawLineUV(TZDStart.x, TZDStart.y, TZDEnd.x, TZDEnd.y, drawColor, pDstImg);
		}
	}

	if ((TZDRECTnumber = tzd_gettargetswithwarn(TZDInfo, TZDState, &DispCord)) > 0) {
		for (i = 0; i < TZDRECTnumber; i++) {
			TZDRect.x = TZDInfo[i].x;
			TZDRect.y = TZDInfo[i].y;
			TZDRect.w = TZDInfo[i].w;
			TZDRect.h = TZDInfo[i].h;

			if (TZDState[i] == FALSE) {
				drawColor = COLOR_YUV_WHITE;
			} else {
				drawColor = COLOR_YUV_RED;
			}

			MovieStamp_DrawRect(pDstImg, &TZDRect, drawColor, border);
		}
	}
#endif
}
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/06/16#CCChao -begin
//#NT# Support traffic sign recognition function
#if MOVIE_TSR_FUNC_
void MovieStamp_DrawTSR(PIMG_BUF pDstImg)
{
#if MOVIE_TSR_DRAW_VIDEO
	UINT32              TSRnumber, i;
	static TRAFFIC_SIGN TSRInfo[TSD_FDE_MAX_TS_NUM];
	//UINT32              border = 2; // border can not smaller than 2
	URECT               DispCord;

#if 0
	UINT32              drawColor = COLOR_YUV_WHITE;
	TSRnumber = TSR_GetRsltTS(TSRInfo, &DispCord);
	drawColor = COLOR_YUV_BLACK;
	for (i = 0; i < TSRnumber; i++) {
		IRECT DrawRect;
		//if( (TSRInfo[i].uiStx+TSRInfo[i].uiWidth) < (pDstImg->Width>>1) )
		//{ continue; }

		DrawRect.x = TSRInfo[i].uiStx;
		DrawRect.y = TSRInfo[i].uiSty;
		DrawRect.w = TSRInfo[i].uiWidth;
		DrawRect.h = TSRInfo[i].uiHeight;
		MovieStamp_DrawRect(pDstImg, &DrawRect, drawColor, border);
	}
#else
	TSRnumber = NvtCv_TSDDSP_GetRsltTS(TSRInfo, &DispCord);
	for (i = 0; i < TSRnumber; i++) {
#if 0
		IRECT DrawRect;
		DrawRect.x = TSRInfo[i].uiStx;
		DrawRect.y = TSRInfo[i].uiSty;
		DrawRect.w = TSRInfo[i].uiWidth;
		DrawRect.h = TSRInfo[i].uiHeight;
		MovieStamp_DrawRect(pDstImg, &DrawRect, drawColor, border);
#else
		MovieStamp_DrawLineUV(TSRInfo[i].uiStx, TSRInfo[i].uiSty + TSRInfo[i].uiHeight,
							  TSRInfo[i].uiStx + TSRInfo[i].uiWidth, TSRInfo[i].uiSty + TSRInfo[i].uiHeight,
							  COLOR_YUV_GREEN, pDstImg);
#endif
	}
#endif
#endif
}
#endif
//#NT#2016/06/16#CCChao -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC


//#NT#2016/12/07#Charlie Chang -begin
//#NT# get/set moviestatmp FTG config

void MovieStamp_set_ftg_draw_config(UINT32 type, UINT32 value)
{

	switch (type) {
	case moviestatmp_ftg_draw_delay_count:
		g_ftg_draw_delay_count = value;
		break;
	case moviestatmp_ftp_rlst_type:

		if (value >= ftg_rlst_max_cnt) {
			DBG_ERR("value(%d) error\r\n", value);
			return;
		}
		g_ftg_rlst_type = value;
		break;
	default:
		DBG_ERR("type(%d) error\r\n", type);
	}
}

UINT32 MovieStamp_get_ftg_draw_config(UINT32 type)
{

	switch (type) {
	case moviestatmp_ftg_draw_delay_count:
		return g_ftg_draw_delay_count;

	case moviestatmp_ftp_rlst_type:

		return g_ftg_rlst_type;

	default:
		DBG_ERR("type(%d) error\r\n", type);

	}
	return 0;
}


//#NT#2016/12/07#Charlie Chang -end

void MovieStamp_DrawFTG(PIMG_BUF pDstImg, UINT32 uiVidEncId)
{
#if MOVIE_FTG_DRAW_VIDEO

#if 1

	UINT32              i, id;
	static  ftg_face    FTGInfo[3][ftg_num_max];
	static UINT32       FTGnumber[3] = {0};
	IRECT               DrawRect;
	UINT32              border = 4; // border can not smaller than 2
	UINT32              drawColor = COLOR_YUV_WHITE;
	URECT               DispCord;
	static UINT32       draw_count_index[3] = {0};

	if (Movie_FTGGetLockSts()) {
		return;
	}

	if (draw_count_index[uiVidEncId] < g_ftg_draw_delay_count) {
		draw_count_index[uiVidEncId] ++;
	} else {
		draw_count_index[uiVidEncId] = 0;
	}

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	if (draw_count_index[uiVidEncId] == 0) { ////update information
		FTGnumber[uiVidEncId] = ftg_getrsltface2(FTGInfo[uiVidEncId], &DispCord, g_ftg_rlst_type);
	}
	for (i = 0; i < FTGnumber[uiVidEncId]; i++) {
		DrawRect.x = FTGInfo[uiVidEncId][i].stx;
		DrawRect.y = FTGInfo[uiVidEncId][i].sty;
		DrawRect.w = FTGInfo[uiVidEncId][i].width;
		DrawRect.h = FTGInfo[uiVidEncId][i].height;


		MovieStamp_DrawRect(pDstImg, &DrawRect, drawColor, border);

		id = FTGInfo[uiVidEncId][i].id;
	}
#endif
#endif
}
#endif
//#NT#2016/10/17#Bin Xiao -end
void MovieStamp_DrawFN(PIMG_BUF pDstImg, UINT32 uiVidEncId)
{
    UINT16 i;

    UPOINT Pos[5]={
        {10,203},
        {120,20},
        {1000,903},
        {1500,5},
        {400,400},
        };
    char *Name[5]={"NOVATEK","ANDERSON CHEN","STARBUCKS GOD","CASIOCASIOCASIO","VIEWSONIC LIN"};

    for(i=0;i<5;i++){
        if(strlen(Name[i])){
            MovieStamp_DrawFaceName(pDstImg, i, &Pos[i], Name[i]);
        }
    }
}
void MovieStamp_DrawALG(UINT32 uiVidEncId, PIMG_BUF pDstImg)
{
#if (((MOVIE_FD_FUNC_== ENABLE) && (MOVIE_DDD_FUNC == DISABLE))|| (IPCAM_FD_FUNC==ENABLE))
	MovieStamp_DrawFD(pDstImg);
	MovieStamp_DrawFN(pDstImg, uiVidEncId);
#endif
#if (MOVIE_OT_FUNC == ENABLE)
	MovieStamp_DrawOT(pDstImg);
#endif
#if ((MOVIE_OD_FUNC == ENABLE)||(IPCAM_OD_FUNC==ENABLE))
	MovieStamp_DrawOD(pDstImg);
#endif
#if (MOVIE_ODT_FUNC == ENABLE)
	MovieStamp_DrawODT(pDstImg);
#endif
#if (MOVIE_FR_FUNC == ENABLE)
	MovieStamp_DrawFR(pDstImg);
#endif
#if (MOVIE_FDCNN_FUNC == ENABLE)
	MovieStamp_DrawFDCNN(pDstImg);
#endif
#if (MOVIE_ODP_FUNC == ENABLE)
	MovieStamp_DrawODP(pDstImg);
#endif
#if (MOVIE_DDD_FUNC == ENABLE)
	MovieStamp_DrawDDD(uiVidEncId, pDstImg);
#endif
#if (MOVIE_TWD_FUNC == ENABLE)
	MovieStamp_DrawTWD(pDstImg);
#endif
#if (MOVIE_TZD_FUNC == ENABLE)
	MovieStamp_DrawTZD(pDstImg);
#endif
#if (_ADAS_FUNC_ == ENABLE)
	MovieStamp_DrawADAS(uiVidEncId, pDstImg);
#endif
#if (MOVIE_TSR_FUNC_ == ENABLE)
	//MovieStamp_DrawTSR(pDstImg);
#endif
#if (MOVIE_FTG_FUNC == ENABLE)
	MovieStamp_DrawFTG(pDstImg, uiVidEncId);
#endif
}

//#NT#2016/06/08#Lincy Lin -end
