//#include "UIFlow.h"
#include "UIFlowPlayFuncs.h"
#include "Type.h"
#include "Audio.h"
#include "Platform.h"
#include "UIFlow.h"
#include "PlaybackTsk.h"
#include "UIAppCommon.h"
#include "ImageUnit_VdoOut.h"

PLB_TASK_DATA g_PlbData = {0};

//#NT#2012/8/13#Philex - begin
//Add Thumbnail play data structure
static PLAY_BROWSER_OBJ     FlowPBBrowserObj;
PURECT   g_pFlowPBThumbRect;
URECT    g_FlowPBThumbRect02[FLOWPB_THUMB_H_NUM2 * FLOWPB_THUMB_V_NUM2] =

// For display size 320x240, 3x3 thumbnail mode
#if (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_3)
{
	{  8, 12, 96, 64}, {112, 12, 96, 64}, {216, 12, 96, 64},
	{  8, 88, 96, 64}, {112, 88, 96, 64}, {216, 88, 96, 64},
	{  8, 164, 96, 64}, {112, 164, 96, 64}, {216, 164, 96, 64},
};
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_2)
// For display size 320x240, 3x2 thumbnail mode
{
	{  8, 38, 96, 64}, {112, 38, 96, 64}, {216, 38, 96, 64},
	{  8, 138, 96, 64}, {112, 138, 96, 64}, {216, 138, 96, 64},
};
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_5_5)
// For display size 320x240, 5x5 thumbnail mode
{
	{  8,  8, 56, 40}, {70,  8, 56, 40}, {132,  8, 56, 40}, {194,  8, 56, 40}, {256,  8, 56, 40},
	{  8, 54, 56, 40}, {70, 54, 56, 40}, {132, 54, 56, 40}, {194, 54, 56, 40}, {256, 54, 56, 40},
	{  8, 100, 56, 40}, {70, 100, 56, 40}, {132, 100, 56, 40}, {194, 100, 56, 40}, {256, 100, 56, 40},
	{  8, 146, 56, 40}, {70, 146, 56, 40}, {132, 146, 56, 40}, {194, 146, 56, 40}, {256, 146, 56, 40},
	{  8, 192, 56, 40}, {70, 192, 56, 40}, {132, 192, 56, 40}, {194, 192, 56, 40}, {256, 192, 56, 40},
};
#endif
//#NT#2012/8/13#Philex - end

UINT8 FlowPB_GetPlbDataState(void)
{
	return g_PlbData.State;
}

void FlowPB_PlayVideo(void)
{
	//#NT#2012/7/4#Philex - begin
	//KeyScan_EnableAutoPoweroff(FALSE); //disable auto poweroff,while playing video
	//KeyScan_EnableUSBDet(FALSE);       //disable USB detect,while playing video
	//#NT#2012/7/4#Philex - end
	g_PlbData.State = PLB_ST_PLAY_MOV;
	g_PlbData.VolCount = AUDIO_VOL_63;  //AUDIO_VOL_63
	//#NT#2012/7/4#Philex - begin
	//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_AUD_VOLUME,1,g_PlbData.VolCount);
	//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVPLAY,1,UIFlowMoviePlayCBFunc);
	//#NT#2012/7/4#Philex - end
}

void FlowPB_PauseVideoPlaying(void)
{
	Ux_FlushEventByRange(NVTEVT_KEY_EVT_START, NVTEVT_KEY_EVT_END);
	//#NT#2012/7/4#Philex - begin
	//KeyScan_EnableAutoPoweroff(FALSE); //disable auto poweroff,while playing video
	//KeyScan_EnableUSBDet(FALSE);       //disable USB detect,while playing video
	//#NT#2012/7/4#Philex - end
	g_PlbData.State = PLB_ST_PAUSE_MOV;
	//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVPAUSE,0);
}

void FlowPB_ResumeVideoPlaying(void)
{
	Ux_FlushEventByRange(NVTEVT_KEY_EVT_START, NVTEVT_KEY_EVT_END);
	//#NT#2012/7/4#Philex - begin
	//KeyScan_EnableAutoPoweroff(FALSE); //disable auto poweroff,while playing video
	//KeyScan_EnableUSBDet(FALSE);       //disable USB detect,while playing video
	//#NT#2012/7/4#Philex - end
	g_PlbData.State = PLB_ST_PLAY_MOV;
	//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVRESUME,0);
}

void FlowPB_StopVideoPlaying(void)
{
	g_PlbData.State = PLB_ST_FULL;
	g_PlbData.VideoPBSpeed = PLB_FWD_MOV_1x;
	//#NT#2012/7/4#Philex - begin
	//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_MOVSTOP,0);
	//KeyScan_EnableAutoPoweroff(TRUE);
	//KeyScan_EnableUSBDet(TRUE);
	//#NT#2012/7/4#Philex - end
}

extern UINT32 g_DualVideo;

//#NT#2012/8/13#Philex - begin
// add Thumbnail play functions
static BOOL FlowPB_ThumbConfig(THUMBNAIL_CONFIG *pThumbCfg)
{
	UINT32  i;
	UINT32  uiDispW, uiDispH;
	UINT16  wThumbWTotal, wThumbHTotal;
	UINT16  wThumbW, wThumbH;
	USIZE bufsize;
	UINT32  rotdir;

	if (g_DualVideo & DISPLAY_2) {
		ImageUnit_GetDisp_BufSize(&ISF_VdoOut2, ISF_IN1, &bufsize);
		rotdir = SysVideo_GetDirbyID(DOUT2);
	} else {
		ImageUnit_GetDisp_BufSize(&ISF_VdoOut1, ISF_IN1, &bufsize);
		rotdir = SysVideo_GetDirbyID(DOUT1);
	}

	if (rotdir == ISF_VDO_DIR_ROTATE_270 || rotdir == ISF_VDO_DIR_ROTATE_90) {
		uiDispW = bufsize.h;
		uiDispH = bufsize.w;
	} else {
		uiDispW = bufsize.w;
		uiDispH = bufsize.h;
	}

	debug_msg("Display w = %d, h = %d\r\n", uiDispW, uiDispH);

	// support maximum 5 x 5 thumbnails
	if (pThumbCfg->wThumbPerLine > 5 || pThumbCfg->wThumbPerColumn > 5) {
		return (FALSE);
	}

	// at least we need 4 pixels for thumbnail frame
	if (pThumbCfg->wFirstHGap < 4) {
		pThumbCfg->wFirstHGap = 4;
	}
	if (pThumbCfg->wHGap < 4) {
		pThumbCfg->wHGap = 4;
	}
	if (pThumbCfg->wLastHGap < 4) {
		pThumbCfg->wLastHGap = 4;
	}
	if (pThumbCfg->wFirstVGap < 4) {
		pThumbCfg->wFirstVGap = 4;
	}
	if (pThumbCfg->wVGap < 4) {
		pThumbCfg->wVGap = 4;
	}
	if (pThumbCfg->wLastVGap < 4) {
		pThumbCfg->wLastVGap = 4;
	}

	wThumbWTotal = pThumbCfg->wFirstHGap + pThumbCfg->wLastHGap +
				   pThumbCfg->wHGap * (pThumbCfg->wThumbPerLine - 1);

	if (wThumbWTotal >= uiDispW) {
		debug_err(("Thumb setting error! wThumbWTotal=%d > uiDispW=%d\r\n", wThumbWTotal, uiDispW));
		return (FALSE);
	}

	wThumbHTotal = pThumbCfg->wFirstVGap + pThumbCfg->wLastVGap +
				   pThumbCfg->wVGap * (pThumbCfg->wThumbPerColumn - 1);

	if (wThumbHTotal >= uiDispH) {
		debug_err(("Thumb setting error! wThumbHTotal=%d > uiDispH=%d\r\n", wThumbHTotal, uiDispH));
		return (FALSE);
	}

	wThumbW = (uiDispW - wThumbWTotal) / pThumbCfg->wThumbPerLine;
	wThumbH = (uiDispH - wThumbHTotal) / pThumbCfg->wThumbPerColumn;

	for (i = 0; i < (UINT32)(pThumbCfg->wThumbPerLine * pThumbCfg->wThumbPerColumn); i++) {
		g_FlowPBThumbRect02[i].x = pThumbCfg->wFirstHGap +
								   (i % pThumbCfg->wThumbPerLine) * (wThumbW + pThumbCfg->wHGap);
		g_FlowPBThumbRect02[i].y = pThumbCfg->wFirstVGap +
								   (i / pThumbCfg->wThumbPerLine) * (wThumbH + pThumbCfg->wVGap);
		g_FlowPBThumbRect02[i].w = wThumbW;
		g_FlowPBThumbRect02[i].h = wThumbH;
	}

	FlowPBBrowserObj.HorNums = pThumbCfg->wThumbPerLine;
	FlowPBBrowserObj.VerNums = pThumbCfg->wThumbPerColumn;
	//uiThumbFrameColor = pThumbCfg->wFrColor;

	return (TRUE);
}

PURECT FlowPB_ThumbDisplay(void)
{
	THUMBNAIL_CONFIG ThumbCfg;


	if (KeyScan_GetPlugDev() == PLUG_TV) {
		//#For LCD panel with 16:9 ratio (but 4:3 pixel resolution)
		if (SysGetFlag(FL_TV_MODE) == TV_MODE_NTSC) {
			ThumbCfg.wFirstHGap         = 22 * (640) / (320);
			ThumbCfg.wHGap              = 22 * (640) / (320);
			ThumbCfg.wLastHGap          = 24 * (640) / (320);
			ThumbCfg.wFirstVGap         = 6 * (448) / (240);
			ThumbCfg.wVGap              = 6 * (448) / (240);
			ThumbCfg.wLastVGap          = 30 * (448) / (240);
		} else {
			ThumbCfg.wFirstHGap         = 22 * (640) / (320);
			ThumbCfg.wHGap              = 22 * (640) / (320);
			ThumbCfg.wLastHGap          = 22 * (640) / (320);
			ThumbCfg.wFirstVGap         = 7 * (528) / (240);
			ThumbCfg.wVGap              = 6 * (528) / (240);
			ThumbCfg.wLastVGap          = 30 * (528) / (240);
		}
	} else {
		if (KeyScan_GetPlugDev() == PLUG_HDMI) {
			//HDMI
			ThumbCfg.wFirstHGap         = 20 * (1920) / (320);
			ThumbCfg.wHGap              = 22 * (1920) / (320);
			ThumbCfg.wLastHGap          = 20 * (1920) / (320);
			ThumbCfg.wFirstVGap         = 6 * (1080) / (240);
			ThumbCfg.wVGap              = 6 * (1080) / (240);
			ThumbCfg.wLastVGap          = 30 * (1080) / (240);
		} else {
			//LCD
#if (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_3)
			ThumbCfg.wFirstHGap         = 18;//16;
			ThumbCfg.wHGap              = 17;//16;
			ThumbCfg.wLastHGap          = 18;//32;
			ThumbCfg.wFirstVGap         = 6;
			ThumbCfg.wVGap              = 6;
			ThumbCfg.wLastVGap          = 30;
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_3_2)
			ThumbCfg.wFirstHGap         = 18;
			ThumbCfg.wHGap              = 16;
			ThumbCfg.wLastHGap          = 18;
			ThumbCfg.wFirstVGap         = 50;
			ThumbCfg.wVGap              = 12;
			ThumbCfg.wLastVGap          = 50;
#elif (FLOWPB_THUMB_MODE == FLOWPB_THUMB_MODE_5_5)
			//Todo: fine tune 5x5 location
			ThumbCfg.wFirstHGap         = 18;
			ThumbCfg.wHGap              = 16;
			ThumbCfg.wLastHGap          = 18;
			ThumbCfg.wFirstVGap         = 6;
			ThumbCfg.wVGap              = 6;
			ThumbCfg.wLastVGap          = 30;
#endif
		}
	}

	ThumbCfg.wThumbPerLine          = FLOWPB_THUMB_H_NUM2;
	ThumbCfg.wThumbPerColumn        = FLOWPB_THUMB_V_NUM2;
	//ThumbCfg.wFrColor               = SHOWOSD_COLOR_WHITE;//SHOWOSD_COLOR_RED;
	FlowPB_ThumbConfig(&ThumbCfg);
//#NT#2012/8/13#Philex - begin
//mark code
#if 0
	FlowPB_ShowThumbBG();
	FlowPBBrowserObj.BrowserCommand = PB_BROWSER_CURR;
	FlowPBBrowserObj.MoveImgMode    = SLIDE_EFFECT_NONE;
	FlowPBBrowserObj.JumpOffset     = 0;
	PB_PlayBrowserMode(&FlowPBBrowserObj);
	g_pFlowPBThumbRect = &g_FlowPBThumbRect02[0];
	PB_SetThumbRect(g_pFlowPBThumbRect);
#endif
//#NT#2012/8/13#Philex - end
	return &g_FlowPBThumbRect02[0];
}


//#NT#2012/8/13#Philex - end
