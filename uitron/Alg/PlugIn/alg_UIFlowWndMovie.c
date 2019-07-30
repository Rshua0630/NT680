#include "SysCommon.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "display.h"
#include "UIDisplay.h"
#include "alg_MovieExe.h"

//Support object tracking function
#if MOVIE_OT_FUNC
#include "NvtOt.h"
#endif

//Object detection
#if MOVIE_OD_FUNC
#include "NvtNNFlow.h"
#endif

//Object detection&DIS Tracking
#if MOVIE_ODT_FUNC
#include "odt_lib.h"
#endif

//Face recognition by CNN
#if MOVIE_FR_FUNC
#include "FR_lib.h"
#endif


//Face Detection by CNN
#if MOVIE_FDCNN_FUNC
#include "fdcnn_lib.h"
#endif

// pipe version of OD
#if MOVIE_ODP_FUNC
#include "NvtNNFlow.h"
#endif

//Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC || MOVIE_BC_MD
#include "bc_lib.h"
#endif

//Support trip-wire detection & trip-zone detection function
#if MOVIE_TWD_FUNC
#include "TWD_lib.h"
#endif

#if MOVIE_TZD_FUNC
#include "TZD_lib.h"
#endif

//Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#include "DDD_lib.h"
#include "MovieStamp.h"
#endif

#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#endif

#if MOVIE_TSR_FUNC_
#include "tsr_lib.h"
#include "TSD_DSP_lib.h"
#endif

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          alg_UIFlowWndMovie
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if MOVIE_FD_FUNC_
INT32 UIFlowWndMovie_FD_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern    URECT     gMovieFdDispCord;
#if (TV_FUNC == ENABLE)
	extern    DX_HANDLE gDevTVObj;
#endif
#if VDO_USE_ROTATE_BUFFER
	extern    DX_HANDLE gDevLCDObj;
	UINT32            isLCD = 0;
#endif
	UINT32            uiRectColor = _OSD_INDEX_WHITE;
	UINT32            i, Fdnumber;
	static FD_FACE    FdInfo[MAX_FDNUM];
	PDISP_OBJ         pDispObj = NULL;
	DISPLAYER_PARAM   DispLyr = {0} ;
	URECT             DispCord;
	UINT32            BorderW = 3, BorderH = 3;
	ISIZE             dev1size;
	USIZE             dev1Ratio;
	UINT32            ExpandRatio = 1;

	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == NULL) {
		return NVTEVT_CONSUME;
	}
#if _TODO
	idec_setFDExpEn(0, 1);
#endif
	DispCord = gMovieFdDispCord;
#if (TV_FUNC == ENABLE)
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == gDevTVObj) {
		ExpandRatio = 2;
	}
#endif
#if VDO_USE_ROTATE_BUFFER
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == gDevLCDObj) {
		isLCD = 1;
	}
#endif
	dev1size = GxVideo_GetDeviceSize(DOUT1);
	dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
	BorderW = dev1size.w / dev1Ratio.w * dev1Ratio.h * BorderH * ExpandRatio * 10 / dev1size.h;
	BorderW = ALIGN_ROUND(BorderW, 10) / 10;
	DBG_IND("[fd] dev1size w=%d,h=%d dev1Ratio w=%d,h=%d, BorderW=%d\r\n", dev1size.w, dev1size.h, dev1Ratio.w, dev1Ratio.h, BorderW);
	DBG_IND("[fd] DispCord x=%d,y=%d, w=%d,h=%d \r\n", DispCord.x, DispCord.y, DispCord.w, DispCord.h);
	Fdnumber = FD_GetRsltFace(FdInfo, &DispCord);


	if (Fdnumber > MAX_FDNUM) {
		Fdnumber = MAX_FDNUM;
	}
	if (0) { //SysGetFlag(FL_FD)==FD_OFF)
		uiRectColor = _OSD_INDEX_TRANSPART;
	} else {
		uiRectColor = _OSD_INDEX_WHITE;
	}

	DBG_IND("[fd]FDFrame_OnDraw color=%d, \r\n", uiRectColor);

	pDispObj = disp_getDisplayObject(DISP_1);
	//#NT#2016/03/02#Lincy Lin -begin
	//#NT#Support object tracking function
	for (i = 0; i < MAX_FDNUM; i++)
		//#NT#2016/03/02#Lincy Lin -end
	{
		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = FALSE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}
	if (uiRectColor != _OSD_INDEX_TRANSPART) {
		for (i = 0; i < Fdnumber; i++) {
			// draw FD frame
			DBG_IND("[fd] %d ( %3d , %3d ; %3d , %3d )\r\n",
					i,
					FdInfo[i].x,
					FdInfo[i].y,
					FdInfo[i].w,
					FdInfo[i].h);

			pDispObj = disp_getDisplayObject(DISP_1);
			DispLyr.SEL.SET_FDSIZE.FD_NUM = DISPFD_NUM0 << i;
#if VDO_USE_ROTATE_BUFFER
			if (isLCD) {
				UINT32              in_hsz, in_vsz;
				in_hsz = dev1size.h;
				in_vsz = dev1size.w;
				DispLyr.SEL.SET_FDSIZE.uiFDX     = FdInfo[i].y;
				DispLyr.SEL.SET_FDSIZE.uiFDY     = in_vsz - FdInfo[i].w - FdInfo[i].x;
				DispLyr.SEL.SET_FDSIZE.uiFDW     = FdInfo[i].h;
				DispLyr.SEL.SET_FDSIZE.uiFDH     = FdInfo[i].w;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = BorderH;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = BorderW;


				DBG_IND("[fd] rotate %d( %3d , %3d ; %3d , %3d )\r\n",
						i,
						DispLyr.SEL.SET_FDSIZE.uiFDX,
						DispLyr.SEL.SET_FDSIZE.uiFDY,
						DispLyr.SEL.SET_FDSIZE.uiFDW,
						DispLyr.SEL.SET_FDSIZE.uiFDH);
			} else
#endif
			{
				DispLyr.SEL.SET_FDSIZE.uiFDX = FdInfo[i].x * ExpandRatio;
				DispLyr.SEL.SET_FDSIZE.uiFDY = FdInfo[i].y;
				DispLyr.SEL.SET_FDSIZE.uiFDW = FdInfo[i].w * ExpandRatio;
				DispLyr.SEL.SET_FDSIZE.uiFDH = FdInfo[i].h;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = BorderW;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = BorderH;

			}
			DBG_IND("[fd] BorderW %d BorderH %d \r\n", DispLyr.SEL.SET_FDSIZE.uiFDBORD_W, DispLyr.SEL.SET_FDSIZE.uiFDBORD_H);
			pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDSIZE, &DispLyr);

			DispLyr.SEL.SET_FDCOLOR.FD_NUM = DISPFD_NUM0 << i;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xFF;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x80;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x80;
			pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDCOLOR, &DispLyr);

			DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
			DispLyr.SEL.SET_FDEN.bEn = TRUE;
			pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
		}
	}
	pDispObj->load(TRUE);
	return NVTEVT_CONSUME;
}
#endif

#if MOVIE_OT_FUNC
INT32 UIFlowWndMovie_OT_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern    URECT   gMovieFdDispCord;
//#NT#2016/10/18#Jeah Yen -begin
//#NT#fix compile error
#if (TV_FUNC == ENABLE)
	extern    DX_HANDLE gDevTVObj;
#endif
//#NT#2016/10/18#Jeah Yen -end
#if VDO_USE_ROTATE_BUFFER
	extern    DX_HANDLE gDevLCDObj;
	UINT32            isLCD = 0;
#endif
	//UINT32            uiRectColor = _OSD_INDEX_WHITE;
	UINT32            OTnumber = 0;
	URECT             OTInfo;
	PDISP_OBJ         pDispObj = NULL;
	DISPLAYER_PARAM   DispLyr = {0} ;
	URECT             DispCord = {0, 0, 640, 360};
	UINT32            BorderW = 3, BorderH = 3;
	ISIZE             dev1size;
	USIZE             dev1Ratio;
	UINT32            ExpandRatio = 1;
	UINT32            dispFdID;

	//#NT#2016/08/18#Lincy Lin -begin
	//#NT#Fix enter sleep mode will have divide by zero exception because of GxVideo_GetDevice return NULL.
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == NULL) {
		return NVTEVT_CONSUME;
	}
	//#NT#2016/08/18#Lincy Lin -end

#if _TODO
	idec_setFDExpEn(0, 1);
#endif
	DispCord = gMovieFdDispCord;
#if (TV_FUNC == ENABLE)
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == gDevTVObj) {
		ExpandRatio = 2;
	}
#endif
#if VDO_USE_ROTATE_BUFFER
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == gDevLCDObj) {
		isLCD = 1;
	}
#endif
	dev1size = GxVideo_GetDeviceSize(DOUT1);
	dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
	BorderW = dev1size.w / dev1Ratio.w * dev1Ratio.h * BorderH * ExpandRatio * 10 / dev1size.h;
	BorderW = ALIGN_ROUND(BorderW, 10) / 10;
	DBG_IND("[fd] dev1size w=%d,h=%d dev1Ratio w=%d,h=%d, BorderW=%d\r\n", dev1size.w, dev1size.h, dev1Ratio.w, dev1Ratio.h, BorderW);
	DBG_IND("[fd] DispCord x=%d,y=%d, w=%d,h=%d \r\n", DispCord.x, DispCord.y, DispCord.w, DispCord.h);
// Only when movie recording will have Object tracking
	if ((gMovData.State == MOV_ST_REC)&&(!OT_PROCESS_STA())) {
		OTnumber = OT_GetTarget(&OTInfo, &DispCord);
	}
	//uiRectColor = _OSD_INDEX_WHITE;
	//DBG_IND("[fd]FDFrame_OnDraw color=%d, \r\n", uiRectColor);
	pDispObj = disp_getDisplayObject(DISP_1);

	// use last DispFD
	dispFdID = DISPFD_NUM0;
	DispLyr.SEL.SET_FDEN.FD_NUM = dispFdID;
	DispLyr.SEL.SET_FDEN.bEn = FALSE;
	pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);

	if (OTnumber) {
		// draw FD frame
		DBG_IND("[ot]( %3d , %3d ; %3d , %3d )\r\n",
				OTInfo.x,
				OTInfo.y,
				OTInfo.w,
				OTInfo.h);

#if VDO_USE_ROTATE_BUFFER
		if (isLCD) {
			UINT32              in_hsz, in_vsz;
			in_hsz = dev1size.h;
			in_vsz = dev1size.w;
			DispLyr.SEL.SET_FDSIZE.uiFDX     = OTInfo.y;
			DispLyr.SEL.SET_FDSIZE.uiFDY     = in_vsz - OTInfo.w - OTInfo.x;
			DispLyr.SEL.SET_FDSIZE.uiFDW     = OTInfo.h;
			DispLyr.SEL.SET_FDSIZE.uiFDH     = OTInfo.w;
			DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = BorderH;
			DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = BorderW;


			DBG_IND("[ot] rotate ( %3d , %3d ; %3d , %3d )\r\n",
					DispLyr.SEL.SET_FDSIZE.uiFDX,
					DispLyr.SEL.SET_FDSIZE.uiFDY,
					DispLyr.SEL.SET_FDSIZE.uiFDW,
					DispLyr.SEL.SET_FDSIZE.uiFDH);
		} else
#endif
		{
			DispLyr.SEL.SET_FDSIZE.uiFDX = OTInfo.x * ExpandRatio;
			DispLyr.SEL.SET_FDSIZE.uiFDY = OTInfo.y;
			DispLyr.SEL.SET_FDSIZE.uiFDW = OTInfo.w * ExpandRatio;
			DispLyr.SEL.SET_FDSIZE.uiFDH = OTInfo.h;
			DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = BorderW;
			DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = BorderH;

		}
		DBG_IND("[ot] BorderW %d BorderH %d \r\n", DispLyr.SEL.SET_FDSIZE.uiFDBORD_W, DispLyr.SEL.SET_FDSIZE.uiFDBORD_H);
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDSIZE, &DispLyr);

		DispLyr.SEL.SET_FDCOLOR.FD_NUM = dispFdID;
		DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xFF;
		DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x80;
		DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x80;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDCOLOR, &DispLyr);

		DispLyr.SEL.SET_FDEN.FD_NUM = dispFdID;
		DispLyr.SEL.SET_FDEN.bEn = TRUE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}
	pDispObj->load(FALSE);
	return NVTEVT_CONSUME;
}
#endif

#if MOVIE_OD_FUNC
INT32 UIFlowWndMovie_OD_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if MOVIE_OD_DRAW_VIDEO
	extern  URECT      gMovieOsdDispCord;
	URECT              DispCord = {0, 0, OSD_W, OSD_H};
    NN_Result          od_result[MAX_DETECTED_OBJECT] = {0};
	UIScreen           ScreenObj = *paramArray;
	INT32              i, ODNumber = 0;
	URECT              rect = {0};

	DispCord = gMovieOsdDispCord;
	ODNumber = NN_GetResult(od_result, &DispCord); ///need to modify position

#if 1
		GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
		GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);

		GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
		GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);
		GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);
        DBG_DUMP("ODNumber = %d\r\n",ODNumber);
		for (i = 0; i < ODNumber; i++) {
			rect.x = od_result[i].x*1920/640;
			rect.y = od_result[i].y*1080/360;
			rect.w = od_result[i].w*1920/640;
			rect.h = od_result[i].h*1080/360;
            //DBG_DUMP("Object %d = [x=%d,y=%d,w=%d,h=%d]\r\n",i,od_result[i].x,od_result[i].y,od_result[i].w,od_result[i].h);
			switch (od_result[i].class) {
			case 0: 	// Bus (blue)
				GxGfx_SetShapeColor(CLRID_IDX_BLUE, CLRID_IDX_BLUE, NULL);
				GxGfx_SetTextColor(CLRID_IDX_BLUE, NULL, NULL);
				break;
			case 1: 	// Car (white)
				GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);
				GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);
				break;
			case 2: 	// Human (red)
				GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
				break;
			default:	// Others (yellow)
				GxGfx_SetShapeColor(CLRID_IDX_YELLOW, CLRID_IDX_YELLOW, NULL);
				GxGfx_SetTextColor(CLRID_IDX_YELLOW, NULL, NULL);
				break;
			}


			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x + rect.w, rect.y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x + rect.w,  rect.y,
					   rect.x + rect.w, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y + rect.h,
					   rect.x + rect.w, rect.y + rect.h);

			//sprintf(string, "%d", id);

			//GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], (INT32)(rect.x + (rect.w >> 1)), (INT32)(rect.y - 5),
			//				string);
		}

#endif
#endif

	return NVTEVT_CONSUME;


}
#endif

#if MOVIE_ODT_FUNC
INT32 UIFlowWndMovie_ODT_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if MOVIE_ODT_DRAW_VIDEO

	extern  URECT      	gMovieOsdDispCord;
	URECT              	DispCord = {0, 0, OSD_W, OSD_H};
	static odt_obj		odt_result[ODT_NUM_MAX] = {0};
	UIScreen           	ScreenObj = *paramArray;
	INT32             	i, j, ODTNumber = 0;
	UINT32             	id;
	URECT              	rect = {0};
	char               	string[10];

	

	for(j = 0; j < ODT_PROCESS_NUM_MAX; j++)
	{
		DispCord = gMovieOsdDispCord;
		ODTNumber = odt_getrsltobj_for_draw(odt_result, j, &DispCord);

		#if 1
		GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
		GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);

		GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
		GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);
		GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);

		for (i = 0; i < ODTNumber; i++) {
			rect.x = odt_result[i].obj_rect.x;
			rect.y = odt_result[i].obj_rect.y;
			rect.w = odt_result[i].obj_rect.w;
			rect.h = odt_result[i].obj_rect.h;
			id	   = j;
			//debug_msg("\n\r*** %d, %d, %d, %d\n\r", rect.x, rect.y, rect.w, rect.h);


			switch (odt_result[i].class) {
			case 0: 	// Bus (blue)
				GxGfx_SetShapeColor(CLRID_IDX_BLUE, CLRID_IDX_BLUE, NULL);
				GxGfx_SetTextColor(CLRID_IDX_BLUE, NULL, NULL);
				break;
			case 1: 	// Car (white)
				GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);
				GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);
				break;
			case 2: 	// Human (red)
				GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
				break;
			default:	// Others (yellow)
				GxGfx_SetShapeColor(CLRID_IDX_YELLOW, CLRID_IDX_YELLOW, NULL);
				GxGfx_SetTextColor(CLRID_IDX_YELLOW, NULL, NULL);
				break;
			}


			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x + rect.w, rect.y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x + rect.w,  rect.y,
					   rect.x + rect.w, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y + rect.h,
					   rect.x + rect.w, rect.y + rect.h);

			sprintf(string, "%d", id);

			GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], (INT32)(rect.x + (rect.w >> 1)), (INT32)(rect.y - 5),
							string);
		}

		#endif
	}

#endif

	return NVTEVT_CONSUME;


}
#endif


#if MOVIE_FR_FUNC
INT32 UIFlowWndMovie_FR_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{


        //debug_msg("draw FR");
		extern	URECT	   gMovieOsdDispCord;
		static FRCNN_result	 FRresult[MAX_NUM]= {0};
		static URECT			   DispCord = {0, 0, OSD_W, OSD_H};
	

		UIScreen		   ScreenObj = *paramArray;
		INT32			   i, FDNumber = 0;
		static URECT	   rect = {0};
		static char		   string[100];
		DispCord = gMovieOsdDispCord;
		FDNumber = FR_getrsltobj_for_draw(FRresult,&DispCord);
		UINT32 FR_state = 0;
		FR_state = FR_reg_get();
       // debug_msg("FR_state %d %d\r\n",FR_state,FDNumber);
#if 1
		GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
		GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);

		GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
		GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);
		GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);

		if(FR_state)
		{

				FDNumber = 0;
			    rect.x = 735/6;rect.y = 345/4.5;
			    rect.w = 450/6; rect.h = 390/4.5;
				
				sprintf(string, "keep face in the box");
				
				GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
						   rect.x + rect.w, rect.y);
				GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
						   rect.x, rect.y + rect.h);
				GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x + rect.w,  rect.y,
						   rect.x + rect.w, rect.y + rect.h);
				GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y + rect.h,
						   rect.x + rect.w, rect.y + rect.h);
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], rect.x, rect.y-10,string);
		}
       for (i = 0; i < FDNumber; i++) 
		{
			rect.x = FRresult[i].x;
			rect.y = FRresult[i].y;
			rect.w = FRresult[i].w;
			rect.h = FRresult[i].h;

		 	strcpy(string,FRresult[i].name);

			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x + rect.w, rect.y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x + rect.w,  rect.y,
					   rect.x + rect.w, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y + rect.h,
					   rect.x + rect.w, rect.y + rect.h);

            GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], rect.x, rect.y,string);

		}
#endif

	return NVTEVT_CONSUME;


}
#endif


#if MOVIE_FDCNN_FUNC
INT32 UIFlowWndMovie_FDCNN_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

#if MOVIE_FDCNN_DRAW_VIDEO
	extern  URECT      	gMovieOsdDispCord;
	URECT              	DispCord = {0, 0, OSD_W, OSD_H};
	static fdcnn_result FDCNN_result[FDCNN_MAX_OUTNUM] = {0};
	UIScreen           	ScreenObj = *paramArray;
	INT32              	i, FDCNNNumber = 0;
	URECT              	rect = {0};

	DispCord = gMovieOsdDispCord;
	FDCNNNumber = fdcnn_getresults(FDCNN_result, &DispCord);

#if 1
		GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
		GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);

		GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
		GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);
		GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);

		for (i = 0; i < FDCNNNumber; i++) {
			rect.x = FDCNN_result[i].x;
			rect.y = FDCNN_result[i].y;
			rect.w = FDCNN_result[i].w;
			rect.h = FDCNN_result[i].h;


			switch (FDCNN_result[i].class) {
			case 1: 	// Face (white)
				GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);
				GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);
				break;
			default:	// Others (yellow)
				GxGfx_SetShapeColor(CLRID_IDX_YELLOW, CLRID_IDX_YELLOW, NULL);
				GxGfx_SetTextColor(CLRID_IDX_YELLOW, NULL, NULL);
				break;
			}


			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x + rect.w, rect.y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x + rect.w,  rect.y,
					   rect.x + rect.w, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y + rect.h,
					   rect.x + rect.w, rect.y + rect.h);

		}

#endif

#endif
	return NVTEVT_CONSUME;


}
#endif


//#NT#2017/12/06#WY -begin
//#NT#
#if MOVIE_ODP_FUNC
INT32 UIFlowWndMovie_ODP_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern  URECT      gMovieOsdDispCord;
	URECT              DispCord = {0, 0, OSD_W, OSD_H};
    NN_Result          od_result[MAX_DETECTED_OBJECT] = {0};
	UIScreen           ScreenObj = *paramArray;
	INT32              i, ODNumber = 0;
	URECT              rect = {0};

	DispCord = gMovieOsdDispCord;
	ODNumber = NN_GetResult(od_result, &DispCord); ///need to modify position

#if 1
		GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
		GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);

		GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
		GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);
		GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);
        DBG_DUMP("ODNumber = %d\r\n",ODNumber);
		for (i = 0; i < ODNumber; i++) {
			rect.x = od_result[i].x*1920/640;
			rect.y = od_result[i].y*1080/360;
			rect.w = od_result[i].w*1920/640;
			rect.h = od_result[i].h*1080/360;
            //DBG_DUMP("Object %d = [x=%d,y=%d,w=%d,h=%d]\r\n",i,od_result[i].x,od_result[i].y,od_result[i].w,od_result[i].h);
			switch (od_result[i].class) {
			case 0: 	// Bus (blue)
				GxGfx_SetShapeColor(CLRID_IDX_BLUE, CLRID_IDX_BLUE, NULL);
				GxGfx_SetTextColor(CLRID_IDX_BLUE, NULL, NULL);
				break;
			case 1: 	// Car (white)
				GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);
				GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);
				break;
			case 2: 	// Human (red)
				GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
				break;
			default:	// Others (yellow)
				GxGfx_SetShapeColor(CLRID_IDX_YELLOW, CLRID_IDX_YELLOW, NULL);
				GxGfx_SetTextColor(CLRID_IDX_YELLOW, NULL, NULL);
				break;
			}


			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x + rect.w, rect.y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y,
					   rect.x, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x + rect.w,  rect.y,
					   rect.x + rect.w, rect.y + rect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,	rect.y + rect.h,
					   rect.x + rect.w, rect.y + rect.h);

			//sprintf(string, "%d", id);

			//GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], (INT32)(rect.x + (rect.w >> 1)), (INT32)(rect.y - 5),
			//				string);
		}

#endif


	return NVTEVT_CONSUME;


}
#endif
//#NT#2017/12/06#WY -end



//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
//#NT#2016/11/10#Brain Yen -begin
//#NT#Add for PTL668 UI bug
_INLINE TCHAR_VALUE String_GetChar(const TCHAR *pszDest, INT16 n)
{
	return (TCHAR_VALUE)(pszDest[n]);
}

UINT16 UIFlowWndMovie_GetStringLength(TCHAR *pszDest)
{
	TCHAR_VALUE c;
	ISIZE szFont;
	INT16 iCount = 0;
	IVALUE id;
	UINT16 Length = 0;
	c = String_GetChar(pszDest, iCount);
	while (c) {
		id = c;
		GxGfx_GetFontCharSize(id, &szFont);
		Length += szFont.w;
		//DBG_ERR("szFont=%d\r\n",szFont);
		iCount++;
		c = String_GetChar(pszDest, iCount);
	}
	//DBG_ERR("Length=%d\r\n",Length);
	return Length;
}
//#NT#2016/11/10#Brain Yen -end

extern    URECT   gMovieOsdDispCord;
static void UIFlowWndMovie_DDD_Draw(UINT32 *paramArray)
{
	URECT       DispCord = {0, 0, OSD_W, OSD_H};
	UIScreen    ScreenObj = *paramArray;
	INT32       i, Dddnumber;
	DDD_SHAPE   TotalShape;
	DDD_SHAPE   PartShape;
	INT32       iPoseRay[8];
	INT32       ret;
	URECT       trackRect;
	DDD_WARN    dowsyState;

	DispCord = gMovieOsdDispCord;
	//DBG_DUMP("x=%d, y=%d, w=%d, h=%d\r\n", DispCord.x, DispCord.y, DispCord.w, DispCord.h);
	Dddnumber = DDD_GetRsltLandmark(&TotalShape, &DispCord);
	//DBG_ERR("Dddnumber=%d\r\n",Dddnumber);

	if (Dddnumber > 0) {
		GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
		GxGfx_SetShapeColor(CLRID_IDX_GREEN, CLRID_IDX_GREEN, NULL);
		for (i = 0; i < Dddnumber; i++) {
			GxGfx_Point(((DC **)ScreenObj)[GxGfx_OSD], TotalShape.pt[i * 2], TotalShape.pt[i * 2 + 1]);
		}

		ret = DDD_GetRsltPoseRay(iPoseRay, 100, &DispCord);
		if (ret) {
			GxGfx_SetShapeColor(CLRID_IDX_GREEN, CLRID_IDX_GREEN, NULL);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  iPoseRay[0],  iPoseRay[1], iPoseRay[2], iPoseRay[3]);
			GxGfx_SetShapeColor(CLRID_IDX_BLUE, CLRID_IDX_BLUE, NULL);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  iPoseRay[0],  iPoseRay[1], iPoseRay[4], iPoseRay[5]);
			GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  iPoseRay[0],  iPoseRay[1], iPoseRay[6], iPoseRay[7]);
		}

		if (DDD_GetFacialState(DDD_FACIAL_TYPE_LEFT_EYE) == DDD_EYE_CLOSED) {
			PartShape = DDD_GetFacialPts(&TotalShape, DDD_FACIAL_TYPE_LEFT_EYE);
			GxGfx_SetShapeColor(CLRID_IDX_BLUE, CLRID_IDX_BLUE, NULL);
			for (i = 0; i < PartShape.nL; i++) {
				GxGfx_Point(((DC **)ScreenObj)[GxGfx_OSD], PartShape.pt[i * 2], PartShape.pt[i * 2 + 1]);
			}
		} else if (DDD_GetFacialState(DDD_FACIAL_TYPE_LEFT_EYE) == DDD_EYE_UNCERTAIN) {
			PartShape = DDD_GetFacialPts(&TotalShape, DDD_FACIAL_TYPE_LEFT_EYE);
			GxGfx_SetShapeColor(CLRID_IDX_ORANGE, CLRID_IDX_GREEN, NULL);
			for (i = 0; i < PartShape.nL; i++) {
				GxGfx_Point(((DC **)ScreenObj)[GxGfx_OSD], PartShape.pt[i * 2], PartShape.pt[i * 2 + 1]);
			}
		}

		if (DDD_GetFacialState(DDD_FACIAL_TYPE_RIGHT_EYE) == DDD_EYE_CLOSED) {
			PartShape = DDD_GetFacialPts(&TotalShape, DDD_FACIAL_TYPE_RIGHT_EYE);
			GxGfx_SetShapeColor(CLRID_IDX_BLUE, CLRID_IDX_BLUE, NULL);
			for (i = 0; i < PartShape.nL; i++) {
				GxGfx_Point(((DC **)ScreenObj)[GxGfx_OSD], PartShape.pt[i * 2], PartShape.pt[i * 2 + 1]);
			}
		} else if (DDD_GetFacialState(DDD_FACIAL_TYPE_RIGHT_EYE) == DDD_EYE_UNCERTAIN) {
			PartShape = DDD_GetFacialPts(&TotalShape, DDD_FACIAL_TYPE_RIGHT_EYE);
			GxGfx_SetShapeColor(CLRID_IDX_ORANGE, CLRID_IDX_GREEN, NULL);
			for (i = 0; i < PartShape.nL; i++) {
				GxGfx_Point(((DC **)ScreenObj)[GxGfx_OSD], PartShape.pt[i * 2], PartShape.pt[i * 2 + 1]);
			}
		}

		if (DDD_GetFacialState(DDD_FACIAL_TYPE_MOUTH) == DDD_MOUTH_YAWN) {
			PartShape = DDD_GetFacialPts(&TotalShape, DDD_FACIAL_TYPE_MOUTH);
			GxGfx_SetShapeColor(CLRID_IDX_SKYBLUE, NULL, NULL);
			for (i = 0; i < PartShape.nL; i++) {
				GxGfx_Point(((DC **)ScreenObj)[GxGfx_OSD], PartShape.pt[i * 2], PartShape.pt[i * 2 + 1]);
			}
		}

		ret = DDD_GetFacialState(DDD_FACIAL_TYPE_HEAD);
		if (ret != DDD_HEAD_STATE_NORMAL) {
			PartShape = DDD_GetFacialPts(&TotalShape, DDD_FACIAL_TYPE_HEAD);
			if (ret == DDD_HEAD_STATE_NODING) {
				GxGfx_SetShapeColor(CLRID_IDX_ORANGE, CLRID_IDX_GREEN, NULL);
			} else { // distribute
				GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
			}
			for (i = 0; i < PartShape.nL; i++) {
				GxGfx_Point(((DC **)ScreenObj)[GxGfx_OSD], PartShape.pt[i * 2], PartShape.pt[i * 2 + 1]);
			}
		}

		ret = DDD_GetRsltTrackingRect(&trackRect, &DispCord);
		if (ret) {
			GxGfx_SetShapeColor(CLRID_IDX_PURPLE, CLRID_IDX_PURPLE, NULL);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  trackRect.x,  trackRect.y,
					   trackRect.x + trackRect.w, trackRect.y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  trackRect.x,  trackRect.y,
					   trackRect.x, trackRect.y + trackRect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  trackRect.x + trackRect.w,  trackRect.y,
					   trackRect.x + trackRect.w, trackRect.y + trackRect.h);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  trackRect.x,  trackRect.y + trackRect.h,
					   trackRect.x + trackRect.w, trackRect.y + trackRect.h);
		}
	}


#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
	g_DDD_DowsyState = 0;//Add by ML Cui@2016-4-25
#endif
#if 1
//#NT#2016/11/21#ML Cui -begin
//#NT#Add for PTL user defined warning
	dowsyState = DDD_GetUserWarning();
	if (dowsyState) {
		UINT32 dowsybinary = 0;

		GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
		GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);

		if (dowsyState & (DDD_UWRN_NOFACE | DDD_UWRN_NOFACE_DURA)) {
			GxGfx_SetTextColor(CLRID_IDX_CYAN, NULL, NULL);

			sprintf(strTmp, "NOFACE Warning %d%d",
					(dowsyState & DDD_UWRN_NOFACE_DURA) > 0,
					(dowsyState & DDD_UWRN_NOFACE) > 0);

			GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], 5, 40,
							strTmp);

			dowsybinary += (dowsyState & (DDD_UWRN_NOFACE |
										  DDD_UWRN_NOFACE_DURA))
						   / DDD_UWRN_NOFACE;

		} else {
			dowsybinary = 0;

			if (dowsyState & (DDD_UWRN_BLINK)) {
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);

				sprintf(strTmp, "Blink Warning %d",
						(dowsyState & DDD_UWRN_BLINK) > 0);
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 60,
								strTmp);
				dowsybinary += 0x10 * ((dowsyState & (DDD_UWRN_BLINK))
									   / DDD_UWRN_BLINK);
			}

			if (dowsyState & (DDD_UWRN_YAWN | DDD_UWRN_YAWN_DURA)) {
				GxGfx_SetTextColor(CLRID_IDX_ORANGE, CLRID_IDX_GREEN, NULL);
				//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 80,
				//                "Remind~ driver yawn too frequently");
				sprintf(strTmp, "Yawn Warning %d%d",
						(dowsyState & DDD_UWRN_YAWN_DURA) > 0,
						(dowsyState & DDD_UWRN_YAWN) > 0);
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 80,
								strTmp);

				dowsybinary += 0x100 * ((dowsyState & (DDD_UWRN_YAWN | DDD_UWRN_YAWN_DURA))
										/ DDD_UWRN_YAWN);
			}

			if (dowsyState & (DDD_UWRN_DIS | DDD_UWRN_DIS_DURA)) {
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
				//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 100,
				//                "Serious Warning! Driver distraction!");

				sprintf(strTmp, "Distraction Warning %d%d",
						(dowsyState & DDD_UWRN_DIS_DURA) > 0,
						(dowsyState & DDD_UWRN_DIS) > 0);
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 120,
								strTmp);
				dowsybinary += 0x1000 * ((dowsyState & (DDD_UWRN_DIS | DDD_UWRN_DIS_DURA))
										 / DDD_UWRN_DIS);
			}

			if (dowsyState & (DDD_UWRN_NOD | DDD_UWRN_NOD_DURA)) {
				GxGfx_SetTextColor(CLRID_IDX_ORANGE, NULL, NULL);
				//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 120,
				//                "Annotation! Driver is a little tired!");
				sprintf(strTmp, "Nod Warning %d%d",
						(dowsyState & DDD_UWRN_NOD_DURA) > 0,
						(dowsyState & DDD_UWRN_NOD) > 0);
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 140,
								strTmp);

				dowsybinary += 0x10000 * ((dowsyState & (DDD_UWRN_NOD | DDD_UWRN_NOD_DURA))
										  / DDD_UWRN_NOD);
			}
			//#NT#2016/06/07#ML Cui -Begin
			//#NT# Add Continues eye closed warning
			if (dowsyState & (DDD_UWRN_EYE | DDD_UWRN_EYE_DURA)) {
				//GxGfx_SetTextColor(CLRID_IDX_ORANGE, NULL, NULL);
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
				//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 140,
				//                "Warning! Driver's eye closed too long!");
				sprintf(strTmp, "Eye Warning %d%d",
						(dowsyState & DDD_UWRN_EYE_DURA) > 0,
						(dowsyState & DDD_UWRN_EYE) > 0);
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 180,
								strTmp);
				dowsybinary += 0x100000 * ((dowsyState & (DDD_UWRN_EYE | DDD_UWRN_EYE_DURA))
										   / DDD_UWRN_EYE) ;
			}
		}
		//#NT#2016/06/07#ML Cui -End
		//debug_msg("dowsybinary = %06x\r\n", dowsybinary);
#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
		g_DDD_DowsyState = dowsybinary;
#endif
	}
//#NT#2016/11/21#ML Cui -end
#else
	dowsyState = DDD_GetWarning();
	if (dowsyState) {
		UINT32 dowsybinary = 0;

		GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
		GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);

		if (dowsyState & DDD_WARN_NONEFACE) {
			GxGfx_SetTextColor(CLRID_IDX_CYAN, NULL, NULL);
			GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], 5, 40,
							"Could not find face!");
			dowsybinary = 1;
		} else {
			dowsybinary = 0;

			if (dowsyState & DDD_WARN_PERCLOS) {
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 60,
//                "Warning! Driver drossy!!");
				sprintf(strTmp, "Warning! Driver drossy!!");
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 60,
								strTmp);
				dowsybinary += 0x10;
			}

			if (dowsyState & DDD_WARN_YAWN) {
				GxGfx_SetTextColor(CLRID_IDX_ORANGE, CLRID_IDX_GREEN, NULL);
//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 80,
//                "Remind~ driver yawn too frequently");
				sprintf(strTmp, "Remind~ driver yawn too frequently");
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 80,
								strTmp);

				dowsybinary += 0x100;
			}

			if (dowsyState & DDD_WARN_DIS) {
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 100,
//                "Serious Warning! Driver distraction!");
				sprintf(strTmp, "Serious Warning!");
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 100,
								strTmp);
				sprintf(strTmp, "Driver distraction!");
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 120,
								strTmp);
				dowsybinary += 0x1000;
			}

			if (dowsyState & DDD_WARN_NODE) {
				GxGfx_SetTextColor(CLRID_IDX_ORANGE, NULL, NULL);
//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 120,
//                "Annotation! Driver is a little tired!");
				sprintf(strTmp, "Annotation! Driver is a little tired!");
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 140,
								strTmp);

				dowsybinary += 0x10000;
			}
//#NT#2016/06/07#ML Cui -Begin
//#NT# Add Continues eye closed warning
			if (dowsyState & DDD_WARN_EYE) {
//GxGfx_SetTextColor(CLRID_IDX_ORANGE, NULL, NULL);
				GxGfx_SetTextColor(CLRID_IDX_RED, NULL, NULL);
//GxGfx_TextPrint(((DC**)ScreenObj)[GxGfx_OSD], 5, 140,
//                "Warning! Driver's eye closed too long!");
				sprintf(strTmp, "Warning!");
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 160,
								strTmp);
				sprintf(strTmp, "Driver's eye closed too long!");
				GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], ((TOOL_LAYOUT_W - UIFlowWndMovie_GetStringLength(strTmp)) / 2), 180,
								strTmp);
				dowsybinary += 0x100000;
			}
//#NT#2016/06/07#ML Cui -End
		}

#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
		g_DDD_DowsyState = dowsybinary;
#endif
	}
#endif

}

#endif

//#NT#2017/02/07#XQ Liu -begin
//#NT# Support Background Construction/Tamper Detection
#if MOVIE_BC_FUNC
INT32 UIFlowWndMovie_BC_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern    URECT   gMovieOsdDispCord;
	URECT       DispCord = {0, 0, OSD_W, OSD_H};
	static URECT     Info[1024];
	UIScreen    ScreenObj = *paramArray;
	INT32       i, number = 0;
	URECT *rect;

	DispCord = gMovieOsdDispCord;
	number = bc_gettarget(Info, &DispCord, 64);
	//number = bc_getblktarget(Info, &DispCord, 1024);

#if 1
	GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
	if (bc_gettdprms(BC_TD_STATUS)) {
		GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
	} else {
		GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);
	}
	for (i = 0; i < number; i++) {
		rect = Info + i;

		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y,
				   rect->x + rect->w, rect->y);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y,
				   rect->x, rect->y + rect->h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x + rect->w,  rect->y,
				   rect->x + rect->w, rect->y + rect->h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y + rect->h,
				   rect->x + rect->w, rect->y + rect->h);
	}
#endif
	return NVTEVT_CONSUME;
}
#endif
//#NT#2017/02/07#XQ Liu -end


//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection function
#if MOVIE_TWD_FUNC
INT32 UIFlowWndMovie_TWD_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern URECT     gMovieOsdDispCord;
	URECT            DispCord = {0, 0, OSD_W, OSD_H};
	static URECT     TWDInfo[twd_max_objects_number];
	static BOOL      TWDState[twd_max_objects_number];
	static UPOINT    TWDLINEInfo[twd_max_nodes_number];
	UIScreen         ScreenObj = *paramArray;
	INT32            i, Tdnumber_rect = 0, Tdnumber_point = 0;
	URECT            *rect;
	UPOINT           TWDStart, TWDEnd;

	DispCord = gMovieOsdDispCord;
	//DBG_DUMP("x=%d, y=%d, w=%d, h=%d\r\n", DispCord.x, DispCord.y, DispCord.w, DispCord.h);
	Tdnumber_point = twd_getpointsintripwire(TWDLINEInfo, &DispCord);
	Tdnumber_rect = twd_gettargetswithwarn(TWDInfo, TWDState, &DispCord);

#if 1
	GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
	GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);

	for (i = 0; i < Tdnumber_point - 1; i++) {
		TWDStart.x = TWDLINEInfo[i].x;
		TWDStart.y = TWDLINEInfo[i].y;
		TWDEnd.x = TWDLINEInfo[i + 1].x;
		TWDEnd.y = TWDLINEInfo[i + 1].y;
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  TWDStart.x,  TWDStart.y,
				   TWDEnd.x, TWDEnd.y);
	}
	for (i = 0; i < Tdnumber_rect; i++) {
		rect = TWDInfo + i;

		if (TWDState[i] == FALSE) {
			GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);
		} else {
			GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
		}

		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y,
				   rect->x + rect->w, rect->y);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y,
				   rect->x, rect->y + rect->h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x + rect->w,  rect->y,
				   rect->x + rect->w, rect->y + rect->h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y + rect->h,
				   rect->x + rect->w, rect->y + rect->h);
	}
#endif
	return NVTEVT_CONSUME;
}
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection function
#if MOVIE_TZD_FUNC
INT32 UIFlowWndMovie_TZD_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern URECT     gMovieOsdDispCord;
	URECT            DispCord = {0, 0, OSD_W, OSD_H};
	static URECT     TZDInfo[tzd_max_objects_number];
	static BOOL      TZDState[tzd_max_objects_number];
	static UPOINT    TZDLINEInfo[tzd_max_nodes_number];
	UIScreen         ScreenObj = *paramArray;
	INT32            i, Tdnumber_rect = 0, Tdnumber_point = 0;
	URECT            *rect;
	UPOINT           TZDStart, TZDEnd;

	DispCord = gMovieOsdDispCord;
	//DBG_ERR("x=%d, y=%d, w=%d, h=%d\r\n", DispCord.x, DispCord.y, DispCord.w, DispCord.h);
	Tdnumber_point = tzd_getpointsintripzone(TZDLINEInfo, &DispCord);
	Tdnumber_rect = tzd_gettargetswithwarn(TZDInfo, TZDState, &DispCord);

#if 1
	GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
	GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);

	for (i = 0; i < Tdnumber_point - 1; i++) {
		TZDStart.x = TZDLINEInfo[i].x;
		TZDStart.y = TZDLINEInfo[i].y;
		TZDEnd.x = TZDLINEInfo[i + 1].x;
		TZDEnd.y = TZDLINEInfo[i + 1].y;
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  TZDStart.x,  TZDStart.y,
				   TZDEnd.x, TZDEnd.y);
	}
	for (i = 0; i < Tdnumber_rect; i++) {
		rect = TZDInfo + i;

		if (TZDState[i] == FALSE) {
			GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);
		} else {
			GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL);
		}

		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y,
				   rect->x + rect->w, rect->y);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y,
				   rect->x, rect->y + rect->h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x + rect->w,  rect->y,
				   rect->x + rect->w, rect->y + rect->h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect->x,  rect->y + rect->h,
				   rect->x + rect->w, rect->y + rect->h);
	}
#endif
	return NVTEVT_CONSUME;
}
#endif
//#NT#2016/10/14#Yuzhe Bian -end

#if (_ADAS_FUNC_ == ENABLE)
INT32 UIFlowWndMovie_ADAS_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UIScreen ScreenObj = *paramArray;
	ADAS_APPS_RESULT_INFO *pAdasRlt = alg_MovieExe_GetAdasRltOSD();
	CHAR InfoStr[20] = {0};
	BVALUE LineColor = 0;

	if ((UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_OFF) && (UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_OFF)) {
		return NVTEVT_CONSUME;
	}

	// Draw FC distance and speed info
#if (GPS_FUNCTION == ENABLE)
	sprintf(InfoStr, "%dm/%dkm", ADAS_GetAppsStatus() ? ADAS_GetFcwsCurrentDist() : 0, (UINT32)g_CurSpeed);
#else   // #if (GPS_FUNCTION == ENABLE)
	sprintf(InfoStr, "%dm/0km", ADAS_GetAppsStatus() ? ADAS_GetFcwsCurrentDist() : 0);
#endif  // #if (GPS_FUNCTION == ENABLE)

	GxGfx_SetTextStroke((const FONT *)&gDemoKit_Font, FONTEFFECT_HIGHLIGHT2, SCALE_1X);
	GxGfx_Text(((DC **)ScreenObj)[GxGfx_OSD], 180, 150, (CHAR *)InfoStr);

	if (ADAS_GetAppsStatus() == FALSE) {
		return NVTEVT_CONSUME;
	}

	GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
	GxGfx_SetShapeColor(CLRID_IDX_GREEN, CLRID_IDX_GREEN, NULL);;

	// SnG ROI
#if (_SNG_FUNC_ == ENABLE)
#if (_DRAW_SNG_ROI_ == ENABLE)
	if (1)
#else   // #if (_DRAW_SNG_ROI_ == ENABLE)
	if (ADAS_IsAdasDebug())
#endif  // #if (_DRAW_SNG_ROI_ == ENABLE)
	{
		ADAS_SNG_ROI_INFO *pSnGROI = alg_MovieExe_GetSnGROIOSD();

		if (pSnGROI) {
			LineColor = (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID) ? CLRID_IDX_GREEN : CLRID_IDX_RED;
			GxGfx_SetShapeColor(LineColor, LineColor, NULL);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pSnGROI->uiP1x, pSnGROI->uiP1y, pSnGROI->uiP2x, pSnGROI->uiP2y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pSnGROI->uiP2x, pSnGROI->uiP2y, pSnGROI->uiP3x, pSnGROI->uiP3y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pSnGROI->uiP3x, pSnGROI->uiP3y, pSnGROI->uiP4x, pSnGROI->uiP4y);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pSnGROI->uiP4x, pSnGROI->uiP4y, pSnGROI->uiP1x, pSnGROI->uiP1y);
		}
	}
#endif  // #if (_SNG_FUNC_ == ENABLE)


	// Only for test
	#if 0
	pAdasRlt->LdwsRsltInfo.uiLx1 = 100;
	pAdasRlt->LdwsRsltInfo.uiLy1 = 100;
	pAdasRlt->LdwsRsltInfo.uiLx2 = 50;
	pAdasRlt->LdwsRsltInfo.uiLy2 = 200;

	pAdasRlt->LdwsRsltInfo.uiRx1 = 200;
	pAdasRlt->LdwsRsltInfo.uiRy1 = 100;
	pAdasRlt->LdwsRsltInfo.uiRx2 = 250;
	pAdasRlt->LdwsRsltInfo.uiRy2 = 200;

	pAdasRlt->FcwsRsltInfo.uiLx = 100;
	pAdasRlt->FcwsRsltInfo.uiLy = 100;
	pAdasRlt->FcwsRsltInfo.uiRx = 200;
	pAdasRlt->FcwsRsltInfo.uiRy = 100;
	#endif

	// draw lane detection result
	if (pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) {
#if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
		if (1)
#else   // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
		if (ADAS_IsAdasDebug())
#endif  // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
		{
			LineColor = (pAdasRlt->LdwsRsltInfo.DepartureDirVideo == LDWS_DEPARTURE_LEFT) ? CLRID_IDX_RED : CLRID_IDX_GREEN;
			GxGfx_SetShapeColor(LineColor, LineColor, NULL);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pAdasRlt->LdwsRsltInfo.uiLx1, pAdasRlt->LdwsRsltInfo.uiLy1, pAdasRlt->LdwsRsltInfo.uiLx2, pAdasRlt->LdwsRsltInfo.uiLy2); // left lane

			LineColor = (pAdasRlt->LdwsRsltInfo.DepartureDirVideo == LDWS_DEPARTURE_RIGHT) ? CLRID_IDX_RED : CLRID_IDX_GREEN;
			GxGfx_SetShapeColor(LineColor, LineColor, NULL);
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pAdasRlt->LdwsRsltInfo.uiRx1, pAdasRlt->LdwsRsltInfo.uiRy1, pAdasRlt->LdwsRsltInfo.uiRx2, pAdasRlt->LdwsRsltInfo.uiRy2); // right lane
		}

		// draw car location
#if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
		if (1)
#else   // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
		if (ADAS_IsAdasDebug())
#endif  // #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
		{
			if ((pAdasRlt->FcwsRsltInfo.uiLx != pAdasRlt->FcwsRsltInfo.uiRx) || (pAdasRlt->FcwsRsltInfo.uiLy != pAdasRlt->FcwsRsltInfo.uiRy)) {
				LineColor = (pAdasRlt->FcwsRsltInfo.FCVideoWarning) ? CLRID_IDX_RED : CLRID_IDX_GREEN;
				GxGfx_SetShapeColor(LineColor, LineColor, NULL);
				GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pAdasRlt->FcwsRsltInfo.uiLx, pAdasRlt->FcwsRsltInfo.uiLy, pAdasRlt->FcwsRsltInfo.uiRx, pAdasRlt->FcwsRsltInfo.uiRy);
			}
		}
	}

	LineColor = (pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) ? CLRID_IDX_BLUE : CLRID_IDX_RED;
	GxGfx_SetShapeColor(LineColor, LineColor, NULL);
#if (_AUTOVP_FUNC_ == ENABLE)
	GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x - 10, pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y, pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x + 10, pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y);
	GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x, pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y - 10, pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x, pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y + 10);
#else   // #if (_AUTOVP_FUNC_ == ENABLE)
	GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], OSD_W / 2 - 10, OSD_H / 2, OSD_W / 2 + 10, OSD_H / 2);
	GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], OSD_W / 2, OSD_H / 2 - 10, OSD_W / 2, OSD_H / 2 + 10);
#endif  // #if (_AUTOVP_FUNC_ == ENABLE)

	// Show SnG & stop detection debug info
	if (ADAS_IsAdasDebug()) {
		UINT32 y;

#if (_SNG_FUNC_ == ENABLE)
		// SnG debug info
		if (pAdasRlt->SnGInfo.uiMovingStatus == STOPNGO_MOVING) {
			GxGfx_SetShapeColor(CLRID_IDX_GREEN, CLRID_IDX_GREEN, NULL); // green
		}
		if (pAdasRlt->SnGInfo.uiStopStatus == STOPNGO_STOP) {
			GxGfx_SetShapeColor(CLRID_IDX_YELLOW, CLRID_IDX_YELLOW, NULL); // yellow
		}
		if (pAdasRlt->SnGInfo.uiGoStatus == STOPNGO_GO) {
			GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL); // red
		}
		for (y = 0; y < 7; y++) {
			GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], OSD_W * 8 / 10, OSD_H * 5 / 8 + y, OSD_W * 8 / 10 + 20, OSD_H * 5 / 8 + y);
		}
#endif  // #if (_SNG_FUNC_ == ENABLE)

		// Stop detection debug info
		if (ADAS_GetStopDectRslt() == STOPDECT_STOP_TRUE) {
			GxGfx_SetShapeColor(CLRID_IDX_RED, CLRID_IDX_RED, NULL); // red
			for (y = 0; y < 7; y++) {
				GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], OSD_W * 9 / 10, OSD_H * 5 / 8 + y, OSD_W * 9 / 10 + 20, OSD_H * 5 / 8 + y);
			}
		}
	}
	return NVTEVT_CONSUME;
}
#endif  // #if (_ADAS_FUNC_ == ENABLE)

//#NT#2016/07/06#CC Chao -begin
#if MOVIE_TSR_FUNC_
INT32 UIFlowWndMovie_TSR_OSD_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern URECT gMovieOsdDispCord;
	UIScreen ScreenObj = *paramArray;
	URECT DispCord;
	TRAFFIC_SIGN TsrInfo[TSD_FDE_MAX_TS_NUM];
	UINT32 Tsrnumber = 0, idx = 0;

	//debug_msg("^M%s: ScreenObj = 0x%x\r\n", __func__, ScreenObj);
	#if 0
	if (gMovData.State != MOV_ST_REC) {
		return NVTEVT_CONSUME;
	}
	#endif

	DispCord = gMovieOsdDispCord;
	//pAdasOsdCoord = (ADAS_OSD_COORDINATE *)FlowMovie_GetAdasOsdIno();
#if 0
	Tsrnumber = TSR_GetRsltTS(TsrInfo, &DispCord);
	GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
	GxGfx_SetShapeColor(CLRID_IDX_BLACK, CLRID_IDX_BLACK, NULL);;
	for (idx = 0; idx < Tsrnumber; idx++) {
		UINT32 ltPtx = TsrInfo[idx].uiStx;
		UINT32 ltPty = TsrInfo[idx].uiSty;
		UINT32 rbPtx = TsrInfo[idx].uiStx + TsrInfo[idx].uiWidth;
		UINT32 rbPty = TsrInfo[idx].uiSty + TsrInfo[idx].uiHeight;

		//if( (ltPtx < (DispCord.w>>1)) || (rbPtx < (DispCord.w>>1)) )
		//{ continue; }

		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], ltPtx, ltPty, ltPtx, rbPty);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], ltPtx, ltPty, rbPtx, ltPty);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], rbPtx, ltPty, rbPtx, rbPty);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], ltPtx, rbPty, rbPtx, rbPty);
	}
#else
	Tsrnumber = NvtCv_TSDDSP_GetRsltTS(TsrInfo, &DispCord);
	GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
	GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);;
	for (idx = 0; idx < Tsrnumber; idx++) {
		UINT32 ltPtx = TsrInfo[idx].uiStx;
		UINT32 ltPty = TsrInfo[idx].uiSty;
		UINT32 rbPtx = TsrInfo[idx].uiStx + TsrInfo[idx].uiWidth;
		UINT32 rbPty = TsrInfo[idx].uiSty + TsrInfo[idx].uiHeight;

		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], ltPtx, ltPty, ltPtx, rbPty);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], ltPtx, ltPty, rbPtx, ltPty);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], rbPtx, ltPty, rbPtx, rbPty);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD], ltPtx, rbPty, rbPtx, rbPty);

		// Draw Speed Number
		CHAR InfoStr[20] = {0};
		sprintf(InfoStr, "%d", TSR_Speed2Num(TsrInfo[idx].uiSpeedNum));
		GxGfx_SetTextStroke((const FONT *)&gDemoKit_Font, FONTEFFECT_HIGHLIGHT2, SCALE_1X);
		GxGfx_Text(((DC **)ScreenObj)[GxGfx_OSD], ltPtx, rbPty, (CHAR *)InfoStr);
	}
#endif

	return NVTEVT_CONSUME;
}

INT32 UIFlowWndMovie_TSR_FD_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern    URECT     gMovieFdDispCord;
	//#NT#2016/10/18#Jeah Yen -begin
	//#NT#fix compile error
#if (TV_FUNC == ENABLE)
	extern    DX_HANDLE gDevTVObj;
#endif
//#NT#2016/10/18#Jeah Yen -end
#if VDO_USE_ROTATE_BUFFER
	extern    DX_HANDLE gDevLCDObj;
	UINT32            isLCD = 0;
#endif
	UINT32            uiRectColor = _OSD_INDEX_WHITE;
	UINT32            i, Tsrnumber;
	TRAFFIC_SIGN      TsrInfo[TSD_FDE_MAX_TS_NUM];
	PDISP_OBJ         pDispObj = NULL;
	DISPLAYER_PARAM   DispLyr = {0} ;
	URECT             DispCord;
	UINT32            BorderW = 3, BorderH = 3;
	ISIZE             dev1size;
	USIZE             dev1Ratio;
	UINT32            ExpandRatio = 1;

	//#NT#2016/08/18#Lincy Lin -begin
	//#NT#Fix enter sleep mode will have divide by zero exception because of GxVideo_GetDevice return NULL.
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == NULL) {
		return NVTEVT_CONSUME;
	}
	//#NT#2016/08/18#Lincy Lin -end

#if _TODO
	idec_setFDExpEn(0, 1);
#endif
	DispCord = gMovieFdDispCord;
#if (TV_FUNC == ENABLE)
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == gDevTVObj) {
		ExpandRatio = 2;
	}
#endif
#if VDO_USE_ROTATE_BUFFER
	if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == gDevLCDObj) {
		isLCD = 1;
	}
#endif
	dev1size = GxVideo_GetDeviceSize(DOUT1);
	dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
	BorderW = dev1size.w / dev1Ratio.w * dev1Ratio.h * BorderH * ExpandRatio * 10 / dev1size.h;
	BorderW = ALIGN_ROUND(BorderW, 10) / 10;
	DBG_IND("[tsr] dev1size w=%d,h=%d dev1Ratio w=%d,h=%d, BorderW=%d\r\n", dev1size.w, dev1size.h, dev1Ratio.w, dev1Ratio.h, BorderW);
	DBG_IND("[tsr] DispCord x=%d,y=%d, w=%d,h=%d \r\n", DispCord.x, DispCord.y, DispCord.w, DispCord.h);

	//Tsrnumber = TSR_GetRsltTS (TsrInfo,&DispCord);
	Tsrnumber = NvtCv_TSDDSP_GetRsltTS(TsrInfo, &DispCord);

	if (Tsrnumber > MAX_FDNUM) {
		Tsrnumber = MAX_FDNUM;
	}
	if (0) { //SysGetFlag(FL_FD)==FD_OFF)
		uiRectColor = _OSD_INDEX_TRANSPART;
	} else {
		uiRectColor = _OSD_INDEX_WHITE;
	}

	DBG_IND("[tsr]FDFrame_OnDraw color=%d, \r\n", uiRectColor);

	pDispObj = disp_getDisplayObject(DISP_1);

	for (i = 0; i < MAX_FDNUM; i++) {
		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = FALSE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}
	if (uiRectColor != _OSD_INDEX_TRANSPART) {
		for (i = 0; i < Tsrnumber; i++) {
			// draw FD frame
			DBG_IND("[tsr] %d ( %3d , %3d ; %3d , %3d )\r\n",
					i,
					TsrInfo[i].uiStx,
					TsrInfo[i].uiSty,
					TsrInfo[i].uiWidth,
					TsrInfo[i].uiHeight);

			pDispObj = disp_getDisplayObject(DISP_1);
			DispLyr.SEL.SET_FDSIZE.FD_NUM = DISPFD_NUM0 << i;
#if VDO_USE_ROTATE_BUFFER
			if (isLCD) {
				UINT32              in_hsz, in_vsz;
				in_hsz = dev1size.h;
				in_vsz = dev1size.w;
				DispLyr.SEL.SET_FDSIZE.uiFDX     = TsrInfo[i].uiSty;
				DispLyr.SEL.SET_FDSIZE.uiFDY     = in_vsz - TsrInfo[i].uiWidth - TsrInfo[i].uiStx;
				DispLyr.SEL.SET_FDSIZE.uiFDW     = TsrInfo[i].uiHeight;
				DispLyr.SEL.SET_FDSIZE.uiFDH     = TsrInfo[i].uiWidth;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = BorderH;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = BorderW;


				DBG_IND("[tsr] rotate %d( %3d , %3d ; %3d , %3d )\r\n",
						i,
						DispLyr.SEL.SET_FDSIZE.uiFDX,
						DispLyr.SEL.SET_FDSIZE.uiFDY,
						DispLyr.SEL.SET_FDSIZE.uiFDW,
						DispLyr.SEL.SET_FDSIZE.uiFDH);
			} else
#endif
			{
				DispLyr.SEL.SET_FDSIZE.uiFDX = TsrInfo[i].uiStx * ExpandRatio;
				DispLyr.SEL.SET_FDSIZE.uiFDY = TsrInfo[i].uiSty;
				DispLyr.SEL.SET_FDSIZE.uiFDW = TsrInfo[i].uiWidth * ExpandRatio;
				DispLyr.SEL.SET_FDSIZE.uiFDH = TsrInfo[i].uiHeight;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_W = BorderW;
				DispLyr.SEL.SET_FDSIZE.uiFDBORD_H = BorderH;

			}
			DBG_IND("[fd] BorderW %d BorderH %d \r\n", DispLyr.SEL.SET_FDSIZE.uiFDBORD_W, DispLyr.SEL.SET_FDSIZE.uiFDBORD_H);
			pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDSIZE, &DispLyr);

			DispLyr.SEL.SET_FDCOLOR.FD_NUM = DISPFD_NUM0 << i;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRY = 0xFF;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCb = 0x80;
			DispLyr.SEL.SET_FDCOLOR.uiFDCRCr = 0x80;
			pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDCOLOR, &DispLyr);

			DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
			DispLyr.SEL.SET_FDEN.bEn = TRUE;
			pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
		}
	}
	pDispObj->load(TRUE);
	return NVTEVT_CONSUME;
}
#endif
//#NT#2016/07/06#CC Chao -end



//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
INT32 UIFlowWndMovie_FTG_Draw(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	extern  URECT      gMovieOsdDispCord;
	URECT              DispCord = {0, 0, OSD_W, OSD_H};
	static  ftg_face   FTGInfo[ftg_num_max];
	UIScreen           ScreenObj = *paramArray;
	INT32              i, FTGnumber = 0;
	UINT32             id;
	URECT              rect = {0};
	char               string[10];

	DispCord = gMovieOsdDispCord;
	//DBG_DUMP("x=%d, y=%d, w=%d, h=%d\r\n", DispCord.x, DispCord.y, DispCord.w, DispCord.h);
	FTGnumber = ftg_getrsltface(FTGInfo, &DispCord);
	//DBG_ERR("FTGnumber = %d\r\n", FTGnumber);

#if 1
	GxGfx_SetShapeStroke(LINEBRUSH_SQUARE | LINEWEIGHT(1), FILLSTYLE_FILL);
	GxGfx_SetShapeColor(CLRID_IDX_WHITE, CLRID_IDX_WHITE, NULL);

	GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, TEXTFONTSTYLE_DEFAULT, TEXTFONTSIZE_DEFAULT);
	GxGfx_SetTextLayout(TEXTLAYOUT_DEFAULT, TEXTALIGN_DEFAULT, TEXTLINEHEIGHT_DEFAULT, TEXTLETTERSPACE_DEFAULT, TEXTINDENTSPACE_DEFAULT);
	GxGfx_SetTextColor(CLRID_IDX_WHITE, NULL, NULL);

	for (i = 0; i < FTGnumber; i++) {
		rect.x = FTGInfo[i].stx;
		rect.y = FTGInfo[i].sty;
		rect.w = FTGInfo[i].width;
		rect.h = FTGInfo[i].height;
		id      = FTGInfo[i].id;

		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,  rect.y,
				   rect.x + rect.w, rect.y);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,  rect.y,
				   rect.x, rect.y + rect.h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x + rect.w,  rect.y,
				   rect.x + rect.w, rect.y + rect.h);
		GxGfx_Line(((DC **)ScreenObj)[GxGfx_OSD],  rect.x,  rect.y + rect.h,
				   rect.x + rect.w, rect.y + rect.h);

		sprintf(string, "%d", id);

		GxGfx_TextPrint(((DC **)ScreenObj)[GxGfx_OSD], (INT32)(rect.x + (rect.w >> 1)), (INT32)(rect.y - 5),
						string);
	}

#endif
	return NVTEVT_CONSUME;
}
#endif
//#NT#2016/10/17#Bin Xiao -end


INT32 UIFlowWndMovie_ALG_Draw_OSD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (MOVIE_FD_FUNC_== ENABLE) && (MOVIE_DDD_FUNC == DISABLE)
	UIFlowWndMovie_FD_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_OT_FUNC == ENABLE)
	UIFlowWndMovie_OT_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_OD_FUNC == ENABLE)
	UIFlowWndMovie_OD_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_ODT_FUNC == ENABLE)
	UIFlowWndMovie_ODT_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_FR_FUNC == ENABLE)
	UIFlowWndMovie_FR_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_FDCNN_FUNC == ENABLE)
	UIFlowWndMovie_FDCNN_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_ODP_FUNC == ENABLE)
	UIFlowWndMovie_ODP_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_DDD_FUNC == ENABLE)
	UIFlowWndMovie_DDD_Draw(paramArray);
#endif
#if (MOVIE_BC_FUNC == ENABLE)
	UIFlowWndMovie_BC_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_TWD_FUNC == ENABLE)
	UIFlowWndMovie_TWD_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_TZD_FUNC == ENABLE)
	UIFlowWndMovie_TZD_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_FTG_FUNC == ENABLE)
	UIFlowWndMovie_FTG_Draw(pCtrl, paramNum, paramArray);
#endif
#if (_ADAS_FUNC_ == ENABLE)
	UIFlowWndMovie_ADAS_Draw(pCtrl, paramNum, paramArray);
#endif
#if (MOVIE_TSR_FUNC_ == ENABLE)
	UIFlowWndMovie_TSR_OSD_Draw(pCtrl, paramNum, paramArray);
#endif
	return NVTEVT_CONSUME;
}

#if MOVIE_FD_FUNC_ || MOVIE_OT_FUNC || MOVIE_BC_FUNC || MOVIE_FTG_FUNC || MOVIE_ODT_FUNC || MOVIE_OD_FUNC || MOVIE_ODP_FUNC || MOVIE_FDCNN_FUNC || MOVIE_FR_FUNC
static void UIFlowWndMovie_ClrFDRect(void)
{
	UINT32 i;
	DISPLAYER_PARAM   DispLyr = {0} ;
	PDISP_OBJ         pDispObj = NULL;

	pDispObj = disp_getDisplayObject(DISP_1);
	for (i = 0; i < MAX_FDNUM; i++) {
		DispLyr.SEL.SET_FDEN.FD_NUM = DISPFD_NUM0 << i;
		DispLyr.SEL.SET_FDEN.bEn = FALSE;
		pDispObj->dispLyrCtrl(DISPLAYER_FD, DISPLAYER_OP_SET_FDEN, &DispLyr);
	}
	pDispObj->load(TRUE);
}
#endif

void UIFlowWndMovie_ALG_Clear_OSD(void)
{
#if MOVIE_FD_FUNC_ || MOVIE_OT_FUNC || MOVIE_BC_FUNC|| MOVIE_FTG_FUNC || MOVIE_ODT_FUNC || MOVIE_OD_FUNC || MOVIE_ODP_FUNC || MOVIE_FDCNN_FUNC || MOVIE_FR_FUNC
	UIFlowWndMovie_ClrFDRect();
#endif
}

