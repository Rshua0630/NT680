/*
    System Video Callback

    System Callback for Video Module.

    @file       SysVideo_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

/*

setting of single display: (LCD is fixed, but allow insert TV or HDMI to replace it)

[PrjCfg_XXX.h]
#define DISPLAY_CAPS_COUNT              1
#define DISPLAY_DEFAULT_ATTACH_DEVICE   LCD1_MASK
#define DISPLAY2_DEFAULT_ATTACH_DEVICE  0
#define DISPLAY_DEFAULT_ATTACH_MASK     DISPLAY_1
#define TV_INSERT_FUNCTION              ENABLE
#define HDMI_INSERT_FUNCTION            ENABLE
[UICfgDefault.h]
#define DEFAULT_DUALDISP                DUALDISP_OFF


setting of dual display: (LCD is fixed, HDMI is also fixed)

[PrjCfg_XXX.h]
#define DISPLAY_CAPS_COUNT              2
#define DISPLAY_DEFAULT_ATTACH_DEVICE   LCD1_MASK
#define DISPLAY2_DEFAULT_ATTACH_DEVICE  HDMI_MASK
#define DISPLAY_DEFAULT_ATTACH_MASK     (DISPLAY_1|DISPLAY_2)
#define TV_INSERT_FUNCTION              DISABLE
#define HDMI_INSERT_FUNCTION            DISABLE
[UICfgDefault.h]
#define DEFAULT_DUALDISP                DUALDISP_ON

and, must ensure (UI_GetData(FL_DualDisp) == DUALDISP_ON)!


setting of dual display: (LCD is fixed, allow insert TV as second display)

[PrjCfg_XXX.h]
#define DISPLAY_CAPS_COUNT              2
#define DISPLAY_DEFAULT_ATTACH_DEVICE   LCD1_MASK
#define DISPLAY2_DEFAULT_ATTACH_DEVICE  0
#define DISPLAY_DEFAULT_ATTACH_MASK     (DISPLAY_1)
#define TV_INSERT_FUNCTION              ENABLE
#define HDMI_INSERT_FUNCTION            DISABLE
[UICfgDefault.h]
#define DEFAULT_DUALDISP                DUALDISP_ON

and, must ensure (UI_GetData(FL_DualDisp) == DUALDISP_ON)!


setting of dual display: (LCD is fixed, allow insert TV or HDMI as second display)

[PrjCfg_XXX.h]
#define DISPLAY_CAPS_COUNT              2
#define DISPLAY_DEFAULT_ATTACH_DEVICE   LCD1_MASK
#define DISPLAY2_DEFAULT_ATTACH_DEVICE  0
#define DISPLAY_DEFAULT_ATTACH_MASK     (DISPLAY_1)
#define TV_INSERT_FUNCTION              ENABLE
#define HDMI_INSERT_FUNCTION            ENABLE
[UICfgDefault.h]
#define DEFAULT_DUALDISP                DUALDISP_ON

and, must ensure (UI_GetData(FL_DualDisp) == DUALDISP_ON)!
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"

#include "DxType.h"
#include "Dx.h"
#include "DxApi.h"

#include "DxDisplay.h"
#if(UI_FUNC==ENABLE)
#include "UIGraphics.h"
#endif
#include "GxSound.h"

#include "GxVideo.h"
#include "GxImageFile.h"
#include "ImageUnit_VdoOut.h"
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
#include "ImageUnit_ImagePipe.h"
#endif
//#NT#2016/09/20#Bob Huang -end
#if(IPCAM_FUNC==ENABLE)
#include "ImageApp_IPCam.h"
#else
#include "UIInfo.h"
#endif
//#NT#2014/03/25#KS Hung -begin
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
#include "UIView.h"
#endif
//#NT#2014/03/25#KS Hung -end

#include "AppDisp_View.h"
#include "SysExam_cmd.h"
#if (IPCAM_FUNC == ENABLE)
#include "UIAppIPCam_Param.h"
#include "UIModeIPCamPB.h"
#endif
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

//UINT32 _DBGLVL_SysVideoExe = 2;
//char _DBGFLT_SysVideoExe[256] = "*";

#if (DISPLAY2_FUNC == ENABLE)
#if ((DISPLAY_DEFAULT_ATTACH_DEVICE!=0) && (DISPLAY2_DEFAULT_ATTACH_DEVICE!=0))
BOOL g_bDualUI = TRUE;
#else
BOOL g_bDualUI = FALSE;
#endif
#else
#if ((DISPLAY_DEFAULT_ATTACH_DEVICE!=0))
BOOL g_bDualUI = FALSE;
#endif
#endif
UINT32 g_bDualUI_dev2_osd_mask = 0;
UINT32 g_DualVideo = DISPLAY_DEFAULT_ATTACH_MASK;


#if (LCD1_FUNC == ENABLE)
DX_HANDLE gDevLCDObj = 0;
#endif
#if (LCD2_FUNC == ENABLE)
DX_HANDLE gDevLCD2Obj = 0;
#endif
#if (TV_FUNC == ENABLE)
DX_HANDLE gDevTVObj = 0;
#endif
#if (HDMI_FUNC == ENABLE)
DX_HANDLE gDevHDMIObj = 0;
#endif
//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
BOOL g_bBootFromALM = FALSE;
#endif
//#NT#2016/03/07#KCHong -end



void Display_DetDispInsert(void);
void Display_DetStatus(void);
void Display_DetLCDDir(void);
int SX_TIMER_DET_TV_ID = -1;
int SX_TIMER_DET_LCDREG_ID = -1;
int SX_TIMER_DET_LCDDIR_ID = -1;
#if ((TV_INSERT_FUNCTION == ENABLE) || (HDMI_INSERT_FUNCTION == ENABLE) || (LCD2_INSERT_FUNCTION == ENABLE))
SX_TIMER_ITEM(Display_DetDispInsert, Display_DetDispInsert, 25, FALSE)
#endif
#if (LCD_AUTORESET_FUNCTION == ENABLE)
SX_TIMER_ITEM(Display_DetStatus, Display_DetStatus, 100, FALSE)
#endif
#if (LCD_ROTATE_FUNCTION == ENABLE)
SX_TIMER_ITEM(Display_DetLCDDir, Display_DetLCDDir, 100, FALSE)
#endif

static void UI_ShowJPG(UINT8 LayerID, UINT32 uiJPGData, UINT32 uiJPGSize, UINT32 uiTempBuf, UINT32 uiTempSize);

#if (LINE_INSERT_FUNCTION == ENABLE)
#else //others
void System_SetAudioOutput(void);
#endif

extern void Video_CB(UINT32 event, UINT32 param1, UINT32 param2);

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
extern BOOL gb3DNROut;
UINT32 gui3DNRMovieSize = MOVIE_SIZE_FRONT_1920x1080P30;
#endif
//#NT#2016/09/20#Bob Huang -end

/*
NOTE:

Total required VDO1 buffer size is optimized by config following settings in PrjCfg_XXXX.h,
User need to review these settings to get optimized buffer size.

1. device
  - LCD2_FUNC
  - TV_FUNC
  - HDMI_FUNC
  - DUALDISP_FUNC (for dual device LCD+LCD2, or LCD+TV, or LCD+HDMI)

2. device max buffer size
  - LCD_MAX_W and LCD_MAX_H
  - LCD2_MAX_W and LCD2_MAX_H
  - TV_MAX_W and TV_MAX_H
  - HDMI_MAX_W and HDMI_MAX_H

3. device need to rotate
  - VDO_USE_ROTATE_BUFFER (for LCD device)

Total required VDO1 buffer size =
  for single display : max_size(LCD,TV,HDMI) = maximum buffer area of LCD/TV/HDMI
    buffer size = VDO1_MAX_W*VDO1_MAX_H buffer x2
  for dual display : LCD + max_size(LCD,TV,HDMI)
    buffer size = LCD_MAX_W*LCD_MAX_H buffer x2 + VDO1_MAX_W*VDO1_MAX_H buffer x2
  for LCD device rotate:
    buffer size += LCD_MAX_W*LCD_MAX_H buffer x1

*/
USIZE Calc_VideoSize_LCD(void)
{
#if (LCD1_FUNC == ENABLE)
	DX_HANDLE cDispDev = 0;
#endif
	ISIZE sz = {0, 0};
	USIZE vdo1_lcd;
#if (LCD1_FUNC == ENABLE)
	cDispDev = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
	Dx_Getcaps(cDispDev, DISPLAY_CAPS_MAXSIZE, (UINT32)&sz); //query mode info
#endif
	vdo1_lcd.w = sz.w;
	vdo1_lcd.h = sz.h;
	return vdo1_lcd;
}
USIZE Calc_VideoSize_MAX(void)
{
	DX_HANDLE cDispDev = 0;
	ISIZE sz = {0, 0};
	USIZE vdo1_max;
#if (LCD1_FUNC == ENABLE)
	cDispDev = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
	Dx_Getcaps(cDispDev, DISPLAY_CAPS_MAXSIZE, (UINT32)&sz); //query mode info
#endif
	{
		vdo1_max.w = sz.w;
		vdo1_max.h = sz.h;
	}
#if( (LCD2_FUNC == ENABLE) \
      || (LCD2_FUNC_USERSTATUS == ENABLE) )
	cDispDev = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD2);
	Dx_Getcaps(cDispDev, DISPLAY_CAPS_MAXSIZE, (UINT32)&sz); //query mode info
	if ((vdo1_max.w * vdo1_max.h) < (UINT32)(sz.w * sz.h)) {
		vdo1_max.w = sz.w;
		vdo1_max.h = sz.h;
	}
#endif
#if (TV_FUNC == ENABLE)
	cDispDev = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_TVOUT);
	Dx_Getcaps(cDispDev, DISPLAY_CAPS_MAXSIZE, (UINT32)&sz); //query mode info
	if ((vdo1_max.w * vdo1_max.h) < (UINT32)(sz.w * sz.h)) {
		vdo1_max.w = sz.w;
		vdo1_max.h = sz.h;
	}
#endif
#if (HDMI_FUNC == ENABLE)
	cDispDev = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_HDMIOUT);
	Dx_Getcaps(cDispDev, DISPLAY_CAPS_MAXSIZE, (UINT32)&sz); //query mode info
	if ((vdo1_max.w * vdo1_max.h) < (UINT32)(sz.w * sz.h)) {
		vdo1_max.w = sz.w;
		vdo1_max.h = sz.h;
	}
#endif
	return vdo1_max;
}

void Check_VideoSize_BufferLimit(UINT32 DevID, DX_HANDLE cDispDev)
{
	ISIZE MaxBufSize = {0, 0};
	ISIZE DeviceSize = {0, 0};
	if (cDispDev != 0) {
		return;
	}
#if (DISPLAY2_FUNC != ENABLE)
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	DeviceSize = GxVideo_GetDeviceSize(DevID);
	Dx_Getcaps(cDispDev, DISPLAY_CAPS_MAXSIZE, (UINT32)&MaxBufSize);
	if ((DeviceSize.w * DeviceSize.h) > (MaxBufSize.w * MaxBufSize.h)) {
		int i;
		for (i = 0; i < 10; i++)
			DBG_ERR("*** Display [%d]: Device Size (%d,%d) > Max Buffer Size (%d,%d) !\r\n",
					DevID, DeviceSize.w, DeviceSize.h, MaxBufSize.w, MaxBufSize.h);
	}
}

void Video_CB_Fast(UINT32 event, UINT32 param1, UINT32 param2);

void System_OnVideoInit(void)
{
	DX_HANDLE cDispDev = 0;
	TM_BOOT_BEGIN("video", "init");
	TM_BOOT_BEGIN("video", "config");
	//PHASE-1 : Init & Open Drv or DrvExt
	{
		GxVideo_RegCB(Video_CB_Fast);         //Register CB function of GxVideo

#if (LCD1_FUNC == ENABLE)
		gDevLCDObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
		DBG_MSG("LCD = %08x\r\n", gDevLCDObj);
#endif
#if( (LCD2_FUNC == ENABLE) \
          || (LCD2_FUNC_USERSTATUS == ENABLE) )
		gDevLCD2Obj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD2);
		DBG_MSG("LCD2 = %08x\r\n", gDevLCD2Obj);
#endif
#if (TV_FUNC == ENABLE)
		gDevTVObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_TVOUT);
		DBG_MSG("TV = %08x\r\n", gDevTVObj);
#endif
#if (HDMI_FUNC == ENABLE)
		gDevHDMIObj = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_HDMIOUT);
		DBG_MSG("HDMI = %08x\r\n", gDevHDMIObj);
#endif

		// Init GxVideo
		GxVideo_InitDevice(DOUT1);
#if (DISPLAY2_FUNC == ENABLE)
		GxVideo_InitDevice(DOUT2);
#endif

		cDispDev = 0;
#if (LCD1_FUNC == ENABLE)
		cDispDev = gDevLCDObj;
		if (cDispDev) {
			Dx_Init(cDispDev, 0, 0, DISPLAY_VER);
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_MODE, LCDMODE);//set default LCD mode
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_DUAL, FALSE);
		}
#endif
#if( (LCD2_FUNC == ENABLE) \
          || (LCD2_FUNC_USERSTATUS == ENABLE) )
		cDispDev = gDevLCD2Obj;
		if (cDispDev) {
			Dx_Init(cDispDev, 0, 0, DISPLAY_VER);
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_MODE, LCD2MODE);//set default LCD2 mode
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_DUAL, FALSE);
		}
#endif
#if (TV_FUNC == ENABLE)
		cDispDev = gDevTVObj;
		if (cDispDev) {
			Dx_Init(cDispDev, 0, 0, DISPLAY_VER);
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_MODE, TVMODE);//set default TV mode
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_DUAL, FALSE);
		}
#endif
#if (HDMI_FUNC == ENABLE)
		cDispDev = gDevHDMIObj;
		if (cDispDev) {
			Dx_Init(cDispDev, 0, 0, DISPLAY_VER);
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_MODE, HDMIMODE);//set default HDMI mode
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_AUDIOFMT, AUD_HDMIMODE_32KHZ);
			Dx_Setconfig((DX_HANDLE)cDispDev, DISPLAY_CFG_DUAL, FALSE);
		}
#endif

		// Init GxDisplay
		GxDisplay_Init(DOUT1, 0, 0);
#if (DISPLAY2_FUNC == ENABLE)
		GxDisplay_Init(DOUT2, 0, 0);
#endif
#if (IPCAM_MODE == DISABLE && IPCAMPB_MODE == DISABLE && !defined(_UI_STYLE_USB_DISK_))
        AppDispView_Init();
#endif

		//5.註冊SxCmd服務 ---------> Cmd Function
	}
	//PHASE-2 : Init & Open Lib or LibExt
	{
		//Config SxJob,SxTimer,SxCmd
		//3.註冊SxJob服務 ---------> System Job
		//SX_JOB_DISPLAY = System_AddSxJob(Display_Change_Job);
		//SX_JOB_DISPMODE = System_AddSxJob(Display_ChangeMode_Job);
		//4.註冊SxTimer服務 ---------> Detect Job
#if ((TV_INSERT_FUNCTION == ENABLE) || (HDMI_INSERT_FUNCTION == ENABLE) || (LCD2_INSERT_FUNCTION == ENABLE))
		SX_TIMER_DET_TV_ID = SxTimer_AddItem(&Timer_Display_DetDispInsert);
#endif
#if (LCD_AUTORESET_FUNCTION == ENABLE)
		SX_TIMER_DET_LCDREG_ID = SxTimer_AddItem(&Timer_Display_DetStatus);
#endif
#if (LCD_ROTATE_FUNCTION == ENABLE)
		SX_TIMER_DET_LCDDIR_ID = SxTimer_AddItem(&Timer_Display_DetLCDDir);
#endif

		//5.註冊SxCmd服務 ---------> Cmd Function
	}
	TM_BOOT_END("video", "config");
}

void System_OnVideoExit(void)
{
	//PHASE-2 : Close Lib or LibExt
	{
		// Turn off LCD backlight
		GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
#if (DISPLAY2_FUNC == ENABLE)
#if( (LCD2_FUNC == ENABLE) \
          || (LCD2_FUNC_USERSTATUS == ENABLE) )
		GxVideo_SetDeviceCtrl(DOUT2, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
#endif
#endif
		// Exit GxVideo
		GxVideo_CloseDevice(DOUT1);
		GxVideo_ExitDevice(DOUT1);
#if (DISPLAY2_FUNC == ENABLE)
		GxVideo_CloseDevice(DOUT2);
		GxVideo_ExitDevice(DOUT2);
#endif
		Ux_PostEvent(NVTEVT_VIDEO_DETACH, 1, 0); // detach
	}
	//PHASE-1 : Close Drv or DrvExt
	{
	}
}

///////////////////////////////////////////////////////////////////////
void Display_DetDispInsert(void)
{
    if(!UI_IsForceLock()) {
#if ((TV_INSERT_FUNCTION == ENABLE) \
     && !(DISPLAY_DEFAULT_ATTACH_DEVICE == TV_MASK) \
     && !(DISPLAY2_DEFAULT_ATTACH_DEVICE == TV_MASK))
	//DBG_MSG("DetTV ");
	DX_HANDLE pDispDev1 = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_TVOUT);
	GxVideo_DetInsert((UINT32)pDispDev1, 0);
#endif
#if ((HDMI_INSERT_FUNCTION == ENABLE) \
     && !(DISPLAY_DEFAULT_ATTACH_DEVICE == HDMI_MASK) \
     && !(DISPLAY2_DEFAULT_ATTACH_DEVICE == HDMI_MASK))
	//DBG_MSG("DetHDMI ");
	DX_HANDLE pDispDev2 = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_HDMIOUT);
	GxVideo_DetInsert((UINT32)pDispDev2, 1);
#endif
#if ((LCD2_INSERT_FUNCTION == ENABLE) \
     && !(DISPLAY2_DEFAULT_ATTACH_DEVICE == LCD2_MASK))
	//DBG_MSG("DetLCD2 ");
	DX_HANDLE pDispDev3 = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD2);
	GxVideo_DetInsert((UINT32)pDispDev3, 2);
#endif
	//DBG_MSG("\r\n");
	}
}

#if (LCD_AUTORESET_FUNCTION == ENABLE)
void Display_DetStatus(void)
{
    if(!UI_IsForceLock()) {
	LCD_RegRestore();
	}
}
#endif

#if (LCD_ROTATE_FUNCTION == ENABLE)
void Display_DetLCDDir(void)
{
    if(!UI_IsForceLock()) {
	DX_HANDLE pDispDev1 = Dx_GetObject(DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD);
	GxVideo_DetDir((UINT32)pDispDev1, 0);
	}
}
#endif


///////////////////////////////////////////////////////////////////////

//TV flow
//close device
///Step3: [App] Pause with wait Finish
///Step4: [DispDrv] Stop
///Step5: [DispDrv] Set DispSrv Disp Buffer
//  1.disp src buffer size, fmt
//  2.disp dest window (x,y,w,h)
//  3.disp dest window dir
//  4.disp physical aspect ratio (sx,sy)
///Step6: [DispDrv] Play
///Step7: [App] Resume with wait Finish (need trigger one flip)
//open device
static void System_DisableVideo(UINT32 DevID)
{
#if (DISPLAY2_FUNC != ENABLE)
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	if (DevID == 0) {
		ImageUnit_Begin(&ISF_VdoOut1, 0);
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DEVICE, 0);
		//ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DISPDETACH, 0);
		ImageUnit_End();
	}
#if (DISPLAY2_FUNC == ENABLE)
	else if (DevID == 1) {
		ImageUnit_Begin(&ISF_VdoOut2, 0);
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DEVICE, 0);
		//ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DISPDETACH, 1);
		ImageUnit_End();
	}
#endif
}

static void System_EnableVideo(UINT32 id, UINT32 CurDevObj)
{

#if (DISPLAY2_FUNC != ENABLE)
	if (id == 1) {
		DBG_ERR("DOUT%d no support device\r\n", id+1);
		return;
	}
#endif

	if (id == 0) {
		ImageUnit_Begin(&ISF_VdoOut1, 0);
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DEVICE, (UINT32)CurDevObj);
		//ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DISPATTACH, 0);
		ImageUnit_End();


	}
#if (DISPLAY2_FUNC == ENABLE)
	else if (id == 1) {
		ImageUnit_Begin(&ISF_VdoOut2, 0);
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DEVICE, (UINT32)CurDevObj);
		//ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_DISPATTACH, 1);
		ImageUnit_End();
	}
#endif
}

static void UI_DisableVideo(UINT32 DevID)
{
#if(UI_FUNC==ENABLE)
#if (DISPLAY2_FUNC != ENABLE)
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	//TODO
	ISIZE DeviceSize = {0, 0};
	DeviceSize = GxVideo_GetDeviceSize(DevID);
	if ((DeviceSize.w == 80) && (DeviceSize.h == 60)) { //if dummy LCD?
		//dummy LCD will never load palette, need to force load palette again!
		UI_DirtyDisplayPalette(DevID);
	}
#endif
}

static void UI_EnableVideo(UINT32 DevID, UINT32 CurDevObj)
{
#if(UI_FUNC==ENABLE)
	UINT32 VideoMode;
	ISIZE DeviceSize;
	UINT id = _DD(DevID);
	//ISIZE BufferSize;
#if (DISPLAY2_FUNC != ENABLE)
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif

	//1.get last video mode
	VideoMode = GxVideo_QueryDeviceLastMode(CurDevObj);
	//2.get its size
	DeviceSize = GxVideo_QueryDeviceModeSize(DevID, CurDevObj, VideoMode);

	DBG_MSG("disp[%d] => device %08x\r\n", id, CurDevObj);
	DBG_MSG("-devicemode = %d\r\n", VideoMode);
	DBG_MSG("-devicesize = %dx%d\r\n", DeviceSize.w, DeviceSize.h);
	//#NT#2014/03/25#KS Hung -begin
#if ((LCD1_FUNC == ENABLE) && (OSD_USE_ROTATE_BUFFER == ENABLE))
	if ((id == 0) && (CurDevObj == (UINT32)gDevLCDObj)) {
		View_Window_ConfigAttr(id, OSD_ATTR_ROTATE, OSD_ROTATE_270_DEGREE); //enable rotate 270
	} else {
		View_Window_ConfigAttr(id, OSD_ATTR_ROTATE, OSD_ROTATE_0_DEGREE); //
	}
#endif
	//#NT#2014/03/25#KS Hung -end
	///Resize OSD
	UI_OnChangeSize(id, &DeviceSize);
#endif
}

extern void System_ResetPowerSaveCount(void);
#if (DISPLAY2_FUNC == ENABLE)

#if (LCD2_FUNC == ENABLE)
void System_OnVideoSetPrimaryDevice(UINT32 uiDispID)
{
	DISPSRV_CMD DispCmd = {0};

	DispCmd.Idx = DISPSRV_CMD_IDX_DISP_PRIMARY_AS_IDE_N;
	DispCmd.In.pData = &uiDispID; // 0 for ide0, 1 for ide1
	DispCmd.In.uiNumByte = sizeof(uiDispID);
	DispCmd.Prop.bExitCmdFinish = TRUE;
	DispSrv_Cmd(&DispCmd);
}
#endif
#endif
static DX_HANDLE SetModeDevObj = 0;
static BOOL uiDisplaySetMode = 0;
/*static*/ DX_HANDLE InsertDevObj = 0;
static DX_HANDLE RemoveDevObj = 0;
static UINT32 uiDisplayEnableState_fw = DISPLAY_DEFAULT_ATTACH_MASK; //fw attach display
static UINT32 uiDisplayEnableState_hw = DISPLAY_DEFAULT_ATTACH_MASK; //hw attach display (default value = fix attach display)
#if (DISPLAY2_FUNC == ENABLE)
#if ((DISPLAY_DEFAULT_ATTACH_DEVICE!=0) && (DISPLAY2_DEFAULT_ATTACH_DEVICE!=0))
static UINT32 uiDisplayEnableState_AllowDual = TRUE;
#else
static UINT32 uiDisplayEnableState_AllowDual = FALSE;
#endif
#else
#if ((DISPLAY_DEFAULT_ATTACH_DEVICE!=0))
static UINT32 uiDisplayEnableState_AllowDual = FALSE;
#endif
#endif
static void _System_ConnectDisplay(UINT32 DisplayMask)
{
	uiDisplayEnableState_hw |= DisplayMask;
}
static void _System_DisconnectDisplay(UINT32 DisplayMask)
{
	uiDisplayEnableState_hw &= ~DisplayMask;
}
UINT32 System_GetConnectDisplay(void)
{
	return uiDisplayEnableState_hw;
}

static void _System_AttachDisplay(UINT32 DevID, DX_HANDLE NewDevObj)
{
	UINT32 id = 0; //fix for CID 45087
	if (DevID == DOUT1) {
		id = 0;
	}
#if (DISPLAY2_FUNC == ENABLE)
	if (DevID == DOUT2) {
		id = 1;
	}
#else
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	Dx_Setconfig(NewDevObj, DISPLAY_CFG_DUAL, uiDisplayEnableState_AllowDual);
	GxVideo_OpenDevice(DevID, (UINT32)NewDevObj, DISP_LASTMODE); //slow API
	Check_VideoSize_BufferLimit(DevID, NewDevObj);
	GxVideo_SetDeviceCtrl(DevID, DISPLAY_DEVCTRL_BACKLIGHT, FALSE); //avoid TV/HDMI start output
	System_EnableVideo(id, (UINT32)NewDevObj);
	UI_EnableVideo(DevID, (UINT32)NewDevObj); //slow API
}
static void _System_AttachDisplay1(UINT32 DevID, DX_HANDLE NewDevObj)
{
#if (DISPLAY2_FUNC != ENABLE)
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	Dx_Setconfig(NewDevObj, DISPLAY_CFG_DUAL, uiDisplayEnableState_AllowDual);
	GxVideo_OpenDevice(DevID, (UINT32)NewDevObj, DISP_LASTMODE);
	Check_VideoSize_BufferLimit(DevID, NewDevObj);
	GxVideo_SetDeviceCtrl(DevID, DISPLAY_DEVCTRL_BACKLIGHT, FALSE); //avoid TV/HDMI start output
}
static void _System_AttachDisplay2(UINT32 DevID, DX_HANDLE NewDevObj)
{
	UINT32 id = 0; //fix for CID 45088
	if (DevID == DOUT1) {
		id = 0;
	}
#if (DISPLAY2_FUNC == ENABLE)
	if (DevID == DOUT2) {
		id = 1;
	}
#else
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	System_EnableVideo(id, (UINT32)NewDevObj);
	UI_EnableVideo(DevID, (UINT32)NewDevObj);
}
static void _System_DetachDisplay(UINT32 DevID)
{
	UINT32 id = 0; //fix for CID 45089
	if (DevID == DOUT1) {
		id = 0;
	}
#if (DISPLAY2_FUNC == ENABLE)
	if (DevID == DOUT2) {
		id = 1;
	}
#else
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	GxVideo_CloseDevice(DevID); //slow API
	System_DisableVideo(id);
	UI_DisableVideo(id);
}
static void _System_ShowDisplay(UINT32 DevID)
{
	//#NT#2016/03/07#KCHong -begin
	//#NT#Low power timelapse function
#if (DISPLAY2_FUNC != ENABLE)
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
	if (g_bBootFromALM == TRUE) {
		return;
	}
#endif
	//#NT#2016/03/07#KCHong -end
	if((DX_HANDLE)GxVideo_GetDevice(DevID))
		GxVideo_SetDeviceCtrl(DevID, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
}

static void _System_HideDisplay(UINT32 DevID)
{
#if (DISPLAY2_FUNC != ENABLE)
	if (DevID == DOUT2) {
		DBG_ERR("disp[%d] no support device\r\n", DevID);
		return;
	}
#endif
	if((DX_HANDLE)GxVideo_GetDevice(DevID))
		GxVideo_SetDeviceCtrl(DevID, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
}

void Video_CB_Fast(UINT32 event, UINT32 param1, UINT32 param2)
{
	switch (event) {
	case DISPLAY_CB_PLUG:
		//This CB will be trigger when TV or HDMI plug
		DBG_IND("DISPLAY_CB_PLUG\r\n");
		{
			InsertDevObj = (DX_HANDLE)param1;

			if (InsertDevObj == 0) {
				return;
			} else {
				char *pDevName = NULL;
				Dx_GetInfo(InsertDevObj, DX_INFO_NAME, &pDevName);
				DBG_DUMP("^Y===================================== video plug [%s]\r\n", pDevName);
			}

			if (InsertDevObj == gDevLCDObj) {
				_System_ConnectDisplay(0x01);
			}
#if (TV_FUNC == ENABLE)
			if (InsertDevObj == gDevTVObj) {
				_System_ConnectDisplay(0x02);
			}
#endif
#if (HDMI_FUNC == ENABLE)
			if (InsertDevObj == gDevHDMIObj) {
				_System_ConnectDisplay(0x04);
			}
#endif
#if (LCD2_FUNC == ENABLE)
			if (InsertDevObj == gDevLCD2Obj) {
				_System_ConnectDisplay(0x08);
			}
#endif
		}
		break;
	}
}

static DX_HANDLE gDispDev = 0;
#if (DISPLAY2_FUNC == ENABLE)
static DX_HANDLE gDispDev2 = 0;
#endif
void System_OnVideoInit2(void)
{
#if( ((LCD1_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == LCD1_MASK)) \
      || ((TV_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == TV_MASK)) \
      || ((HDMI_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == HDMI_MASK)) )
	DX_HANDLE cDispDev = 0;
#endif
#if( ((LCD2_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == LCD2_MASK)) \
      || ((TV_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == TV_MASK)) \
      || ((HDMI_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == HDMI_MASK)) \
      || (LCD2_FUNC_USERSTATUS == ENABLE) )
	DX_HANDLE cDispDev2 = 0;
#endif

	TM_BOOT_BEGIN("video", "open");
#if ((LCD1_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == LCD1_MASK))
	//open LCD
	{
		cDispDev = gDevLCDObj;
	}
#endif
#if ((TV_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == TV_MASK))
	//early detect and open TV
	//Display_DetDispInsert();
	//if((System_GetConnectDisplay() & 0x02) && (InsertDevObj == gDevTVObj))
	{
		InsertDevObj = 0;
		cDispDev = gDevTVObj;
	}
#endif
#if ((HDMI_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == HDMI_MASK))
	//early detect and open HDMI
	//Display_DetDispInsert();
	//if((System_GetConnectDisplay() & 0x04) && (InsertDevObj == gDevHDMIObj))
	{
		InsertDevObj = 0;
		cDispDev = gDevHDMIObj;
	}
#endif

#if ((LCD2_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == LCD2_MASK))
	//early detect and open LCD2
	//Display_DetDispInsert();
	//if((System_GetConnectDisplay() & 0x08) && (InsertDevObj == gDevLCD2Obj))
	{
		InsertDevObj = 0;
		cDispDev2 = gDevLCD2Obj;
	}
#endif
#if ((TV_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == TV_MASK))
	//early detect and open TV
	//Display_DetDispInsert();
	//if((System_GetConnectDisplay() & 0x02) && (InsertDevObj == gDevTVObj))
	{
		InsertDevObj = 0;
		cDispDev2 = gDevTVObj;
	}
#endif
#if ((HDMI_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == HDMI_MASK))
	//early detect and open HDMI
	//Display_DetDispInsert();
	//if((System_GetConnectDisplay() & 0x04) && (InsertDevObj == gDevHDMIObj))
	{
		InsertDevObj = 0;
		cDispDev2 = gDevHDMIObj;
	}
#endif
#if (LCD2_FUNC_USERSTATUS == ENABLE)
	{
		cDispDev2 = gDevLCD2Obj;
	}
#endif
#if( ((LCD1_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == LCD1_MASK)) \
      || ((TV_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == TV_MASK)) \
      || ((HDMI_FUNC == ENABLE) && (DISPLAY_DEFAULT_ATTACH_DEVICE == HDMI_MASK)) )
	if (cDispDev) {
		char *pDevName = NULL;
		Dx_GetInfo(cDispDev, DX_INFO_NAME, &pDevName);
		DBG_DUMP("^Y===================================== video fixed [%s]\r\n", pDevName);
		//DBG_MSG("attach fixed display\r\n");
		DBG_MSG("^Y===================================== video on - b\r\n");
		//config video Output
		_System_AttachDisplay1(DOUT1, cDispDev);
#if (LCD_ROTATE_FUNCTION == ENABLE)
		if (cDispDev == gDevLCDObj)
			//config video Direct
		{
			Display_DetLCDDir();
		}
#endif

#if (VDO_USE_ROTATE_BUFFER == ENABLE)
	if (cDispDev == gDevLCDObj) {
		GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_SWAPXY, 1);
	} else {
		GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_SWAPXY, 0);
	}
#endif

		gDispDev = cDispDev;
	}
#endif
#if (DISPLAY2_FUNC == ENABLE)
#if( ((LCD2_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == LCD2_MASK)) \
      || ((TV_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == TV_MASK)) \
      || ((HDMI_FUNC == ENABLE) && (DISPLAY2_DEFAULT_ATTACH_DEVICE == HDMI_MASK)) \
      || (LCD2_FUNC_USERSTATUS == ENABLE) )
	if (cDispDev2) {
		char *pDevName = NULL;
		Dx_GetInfo(cDispDev2, DX_INFO_NAME, &pDevName);
		DBG_DUMP("^Y===================================== video fixed [%s]\r\n", pDevName);
		//DBG_MSG("attach fixed display\r\n");
		DBG_MSG("^Y===================================== video on - b\r\n");
		//config video Output
		_System_AttachDisplay1(DOUT2, cDispDev2);
		gDispDev2 = cDispDev2;
	}
#endif
#endif
	if (__DBGLVL__ >= __DBGLVL__MSG) {
		GxVideo_DumpInfo();
	}
	TM_BOOT_END("video", "open");
	TM_BOOT_END("video", "init");
	TM_BOOT_BEGIN("video", "show_logo");
	{
		//#NT#2016/03/07#KCHong -begin
		//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
		if (MovieTLLPR_CheckHWRTStatus() == TL_HWRT_BOOT_ALARM) {
			g_bBootFromALM = TRUE;
		}
		if (g_bBootFromALM == FALSE) {
#endif
			//#NT#2016/03/07#KCHong -end
			//if(1st_attach)
			//  DISP: if(boot)->show boot logo/animation
#if (POWERONLOGO_FUNCTION == ENABLE)
			Display_ShowSplash(SPLASH_POWERON);
			Delay_DelayMs(30); // delay some time to avoid LCD flicker as power on
			GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
			DBG_MSG("^Y===================================== video on - e\r\n");
#endif
			//#NT#2016/03/07#KCHong -begin
			//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
		}
#endif
		//#NT#2016/03/07#KCHong -end
	}
	TM_BOOT_END("video", "show_logo");
}

void System_OnVideoInit3(void)
{
	DX_HANDLE cDispDev = gDispDev;
#if (DISPLAY2_FUNC == ENABLE)
	DX_HANDLE cDispDev2 = gDispDev2;
#endif
	TM_BOOT_BEGIN("video", "gfx");

	if (cDispDev) {
		DBG_MSG("attach fixed display\r\n");
		//config video Output
		_System_AttachDisplay2(DOUT1, cDispDev);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if (cDispDev2) {
		DBG_MSG("attach fixed display\r\n");
		//config video Output
		_System_AttachDisplay2(DOUT2, cDispDev2);
	}
#endif
	GxVideo_RegCB(Video_CB);         //Register CB function of GxVideo
#if ((TV_INSERT_FUNCTION == ENABLE) || (HDMI_INSERT_FUNCTION == ENABLE) || (LCD2_INSERT_FUNCTION == ENABLE))
#if (TV_INSERT_FUNCTION == ENABLE)
	DBG_MSG("DetTV insert\r\n");
#endif
#if (HDMI_INSERT_FUNCTION == ENABLE)
	DBG_MSG("DetHDMI insert\r\n");
#endif
#if (LCD2_INSERT_FUNCTION == ENABLE)
	DBG_MSG("DetLCD2 insert\r\n");
#endif
	SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID, TRUE);
#endif
#if (LCD_ROTATE_FUNCTION == ENABLE)
	SxTimer_SetFuncActive(SX_TIMER_DET_LCDDIR_ID, TRUE);
	DBG_MSG("DetLCD rotate\r\n");
#endif

#if(UI_FUNC==ENABLE)
	UI_GfxInit();
	UI_DispInit();
	UI_ControlShowInit();
#endif
	TM_BOOT_END("video", "gfx");
}


UINT32 System_GetEnableDisp(void)
{
	return uiDisplayEnableState_fw;
}
UINT32 System_GetEnableDispCount(void)
{
	UINT32 c = 0, i;
	UINT32 mask = 1;
	for (i = 0; i < 2; i++) {
		if (mask & uiDisplayEnableState_fw) {
			c++;
		}
		mask <<= 1;
	}
	return c;
}
extern void Display_PauseSplash(void);
extern void Display_ResumeSplash(void);

#if (DISPLAY2_FUNC == ENABLE)
#define NEWDEVOBJ(obj)	NewDevObj = (obj);
#define NEWDEV2OBJ(obj)	NewDev2Obj = (obj);
#else
#define NEWDEVOBJ(obj)	NewDevObj = (obj);
#define NEWDEV2OBJ(obj)
#endif
void System_EnableDisp(UINT32 DispMask)
{
	DX_HANDLE CurrDevObj;
	DX_HANDLE NewDevObj;
#if (DISPLAY2_FUNC == ENABLE)
	DX_HANDLE CurrDev2Obj;
	DX_HANDLE NewDev2Obj;
#endif
	UINT32 NewState;

	CurrDevObj = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
	NewDevObj = 0;
#if (DISPLAY2_FUNC == ENABLE)
	CurrDev2Obj = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
	NewDev2Obj = 0;
#endif
	uiDisplayEnableState_AllowDual = FALSE;
#if (DISPLAY2_FUNC == ENABLE)
#if (UI_FUNC == ENABLE)
	uiDisplayEnableState_AllowDual = (UI_GetData(FL_DualDisp) == DUALDISP_ON);
#else
	uiDisplayEnableState_AllowDual = TRUE;
#endif
#endif
#if (DISPLAY2_FUNC == ENABLE)
	//force dual display
	DispMask &= (DISPLAY_1 | DISPLAY_2);
#else
	//force single display
	DispMask &= (DISPLAY_1);
#endif
	if ((!uiDisplayEnableState_AllowDual) && (DispMask != DISPLAY_1)) {
		//force single display
		DispMask &= DISPLAY_1;
	}
#if (DUALDISP_FUNC == DISABLE)
	{
		//force single display
		DispMask &= DISPLAY_1;
	}
#endif

	if (__DBGLVL__ >= __DBGLVL__MSG) {
		DBGD(uiDisplayEnableState_AllowDual);
		DBGD(CurrDevObj);
#if (DISPLAY2_FUNC == ENABLE)
		DBGD(CurrDev2Obj);
#endif
		DBGD(uiDisplayEnableState_fw);
		DBGD(InsertDevObj);
		DBGD(RemoveDevObj);
		DBGD(DispMask);
	}

	if ((InsertDevObj == 0) && (RemoveDevObj == 0) && (SetModeDevObj != 0)) {
		//set mode only
	} else if ((InsertDevObj == 0) && (RemoveDevObj == 0) && (DispMask == uiDisplayEnableState_fw)) {
		//ignore!
		return;
	}

	NewState = uiDisplayEnableState_fw;
#if 1
	if ((uiDisplayEnableState_fw == 0) && (DispMask == DISPLAY_1)) { //current 0, want 1
#if (DISPLAY2_FUNC == ENABLE)
		if ((InsertDevObj != 0) && ((CurrDevObj == 0) && (CurrDev2Obj == 0))) { //just insert hw
#else
		if ((InsertDevObj != 0) && ((CurrDevObj == 0))) {
#endif
			if (InsertDevObj == gDevLCDObj)  {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#if (TV_FUNC == ENABLE)
			if (InsertDevObj == gDevTVObj)   {
				NEWDEVOBJ(gDevTVObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#endif
#if (HDMI_FUNC == ENABLE)
			if (InsertDevObj == gDevHDMIObj) {
				NEWDEVOBJ(gDevHDMIObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#endif
#if (LCD2_FUNC == ENABLE)
			if (InsertDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
			}
#endif
		}
	}
#endif
#if 1
#if (DISPLAY2_FUNC == ENABLE)
	else if (((uiDisplayEnableState_fw == DISPLAY_1) || (uiDisplayEnableState_fw == DISPLAY_2)) && (DispMask == 0)) { //current 1, want 0
#else
	else if (((uiDisplayEnableState_fw == DISPLAY_1)) && (DispMask == 0)) { //current 1, want 0
#endif
		if ((RemoveDevObj != 0)) { //just remove hw
			if (RemoveDevObj == gDevLCDObj)  {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(0);
				NewState = 0;
			}
#if (TV_FUNC == ENABLE)
			if (RemoveDevObj == gDevTVObj)   {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(0);
				NewState = 0;
			}
#endif
#if (HDMI_FUNC == ENABLE)
			if (RemoveDevObj == gDevHDMIObj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(0);
				NewState = 0;
			}
#endif
#if (LCD2_FUNC == ENABLE)
			if (RemoveDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(0);
				NewState = 0;
			}
#endif
		}
	}
#endif
#if 1
#if (DISPLAY2_FUNC == ENABLE)
	else if (((uiDisplayEnableState_fw == DISPLAY_1) || (uiDisplayEnableState_fw == DISPLAY_2)) && (DispMask == DISPLAY_1)) { //current 1, want 1
#else
	else if (((uiDisplayEnableState_fw == DISPLAY_1)) && (DispMask == DISPLAY_1)) { //current 1, want 1
#endif
		if ((InsertDevObj != 0)) { //just insert hw
			if (InsertDevObj == gDevLCDObj)  {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#if (TV_FUNC == ENABLE)
			if (InsertDevObj == gDevTVObj)   {
				NEWDEVOBJ(gDevTVObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#endif
#if (HDMI_FUNC == ENABLE)
			if (InsertDevObj == gDevHDMIObj) {
				NEWDEVOBJ(gDevHDMIObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#endif
#if (LCD2_FUNC == ENABLE)
			if (InsertDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
			}
#endif
		}
		else if ((RemoveDevObj != 0)) { //just remove hw
			DX_HANDLE gDev1Obj = gDevLCDObj;
			if (RemoveDevObj == gDevLCDObj)  {
				NEWDEVOBJ(gDev1Obj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#if (TV_FUNC == ENABLE)
			if (RemoveDevObj == gDevTVObj)   {
				NEWDEVOBJ(gDev1Obj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#endif
#if (HDMI_FUNC == ENABLE)
			if (RemoveDevObj == gDevHDMIObj) {
				NEWDEVOBJ(gDev1Obj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#endif
#if (LCD2_FUNC == ENABLE)
			if (RemoveDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(gDev1Obj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
			}
#endif
		}
	}
#endif
#if ((DISPLAY2_FUNC == ENABLE)&&(DUALDISP_FUNC == ENABLE))//CID 30276:only DUALDISP_FUNC enable,DispMask == (DISPLAY_1 | DISPLAY_2)
	else if (((uiDisplayEnableState_fw == DISPLAY_1) || (uiDisplayEnableState_fw == DISPLAY_2)) && (DispMask == (DISPLAY_1 | DISPLAY_2))) { //current 1, want 2
	    if ((InsertDevObj != 0) && ((CurrDevObj == gDevLCDObj) && (CurrDev2Obj == 0))) { //just insert hw
	        if (InsertDevObj == gDevLCDObj)  {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#if (TV_FUNC == ENABLE)
	        else if (InsertDevObj == gDevTVObj)   {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(gDevTVObj);
				NewState = (DISPLAY_1 | DISPLAY_2);
	        }
#endif
#if (HDMI_FUNC == ENABLE)
	        else if (InsertDevObj == gDevHDMIObj) {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(gDevHDMIObj);
				NewState = (DISPLAY_1 | DISPLAY_2);
	        }
#endif
#if (LCD2_FUNC == ENABLE)
	        else if (InsertDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = (DISPLAY_1 | DISPLAY_2);
	        }
#endif
	    }
#if (LCD2_FUNC == ENABLE)
	    else if ((InsertDevObj != 0) && ((CurrDevObj == 0) && (CurrDev2Obj == gDevLCD2Obj))) { //just insert hw
	        if (InsertDevObj == gDevLCDObj)  {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = (DISPLAY_1 | DISPLAY_2);
	        }
#if (TV_FUNC == ENABLE)
	        else if (InsertDevObj == gDevTVObj)   {
				NEWDEVOBJ(gDevTVObj);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = (DISPLAY_1 | DISPLAY_2);
	        }
#endif
#if (HDMI_FUNC == ENABLE)
	        else if (InsertDevObj == gDevHDMIObj) {
				NEWDEVOBJ(gDevHDMIObj);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = (DISPLAY_1 | DISPLAY_2);
	        }
#endif
#if (LCD2_FUNC == ENABLE)
	        else if (InsertDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#endif
	    }
#endif
	}
#endif
#if (DISPLAY2_FUNC == ENABLE)
 	else if ((uiDisplayEnableState_fw == (DISPLAY_1 | DISPLAY_2)) && (DispMask == DISPLAY_1)) { //current 2, want 1
	    if ((RemoveDevObj != 0) && (CurrDevObj == gDevLCDObj)) { //just remove hw
	        if (RemoveDevObj == gDevLCDObj)  {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#if (TV_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevTVObj)   {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#endif
#if (HDMI_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevHDMIObj) {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#endif
#if (LCD2_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#endif
	    }
#if (LCD2_FUNC == ENABLE)
	    else if ((RemoveDevObj != 0) && (CurrDev2Obj == gDevLCD2Obj)) { //just remove hw
	        if (RemoveDevObj == gDevLCDObj)  {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#if (TV_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevTVObj)   {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#endif
#if (HDMI_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevHDMIObj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#endif
#if (LCD2_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#endif
	    }
#endif
	}
#endif
#if ((DISPLAY2_FUNC == ENABLE)&&(DUALDISP_FUNC == ENABLE))//CID 30276:only DUALDISP_FUNC enable,DispMask == (DISPLAY_1 | DISPLAY_2)
	else if ((uiDisplayEnableState_fw == (DISPLAY_1 | DISPLAY_2)) && (DispMask == (DISPLAY_1 | DISPLAY_2))) { //current 2, want 2
	    if ((RemoveDevObj != 0) && (CurrDevObj == gDevLCDObj)) { //just remove hw
	        if (RemoveDevObj == gDevLCDObj)  {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#if (TV_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevTVObj)   {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#endif
#if (HDMI_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevHDMIObj) {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#endif
#if (LCD2_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(gDevLCDObj);
				NEWDEV2OBJ(0);
				NewState = DISPLAY_1;
	        }
#endif
	    }
#if (LCD2_FUNC == ENABLE)
	    else if ((RemoveDevObj != 0) && (CurrDev2Obj == gDevLCD2Obj)) { //just remove hw
	        if (RemoveDevObj == gDevLCDObj)  {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#if (TV_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevTVObj)   {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#endif
#if (HDMI_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevHDMIObj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#endif
#if (LCD2_FUNC == ENABLE)
	        else if (RemoveDevObj == gDevLCD2Obj) {
				NEWDEVOBJ(0);
				NEWDEV2OBJ(gDevLCD2Obj);
				NewState = DISPLAY_2;
	        }
#endif
	    }
#endif
	}
#endif

	if ((InsertDevObj == 0) && (RemoveDevObj == 0) && (SetModeDevObj != 0)) {
		//set mode only
		DBG_WRN("Set mode, force change!\r\n");
		NEWDEVOBJ(CurrDevObj);
		NEWDEV2OBJ(CurrDev2Obj);
		NewState = uiDisplayEnableState_fw;
	} else if ((InsertDevObj == 0) && (RemoveDevObj == 0) && (DispMask != uiDisplayEnableState_fw)) {
		//uiDisplayEnableState_hw mask is not support to request DispMask
		DBG_WRN("No insert/remove, ignore change!\r\n");
		return;
	} else {
		if (__DBGLVL__ >= __DBGLVL__MSG) {
			DBGD(NewDevObj);
#if (DISPLAY2_FUNC == ENABLE)
			DBGD(NewDev2Obj);
#endif
			DBGD(NewState);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if ((NewState == uiDisplayEnableState_fw)
			&& (NewDevObj == CurrDevObj)
			&& (NewDev2Obj == CurrDev2Obj)
		   )
#else
		if ((NewState == uiDisplayEnableState_fw)
			&& (NewDevObj == CurrDevObj)
		   )
#endif
		{
			if (InsertDevObj != 0) {
				DBG_ERR("Cannot handle this Insert event!\r\n");
			}
			if (RemoveDevObj != 0) {
				DBG_ERR("Cannot handle this Remove event!\r\n");
			}
			return;
		}
	}

	DBG_MSG("^Y===================================== video off - b\r\n");
	//--------------------------------------------------------------------------
#if (PWR_FUNC == ENABLE)
	if (InsertDevObj != 0) { //just insert hw
		//Relationship between TV and sleep:
		//    begin TV pluging flow ->  wakeup sleep, BUT do not turn on display and backlight
		//    end TV pluging flow ->  stop Sleep counting, turn on display and backlight
		if (GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) > 0) { // if sleep level > 0
			System_ChangeSleep(0); //enforce trigger system wakeup
		}
	}
#endif
	UI_LockEvent();
#if 1
	if (SetModeDevObj != 0) {
		if (CurrDevObj) {
			_System_HideDisplay(DOUT1);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if (CurrDev2Obj) {
			_System_HideDisplay(DOUT2);
		}
#endif
	}
#endif
#if (PWR_FUNC == ENABLE)
	if (InsertDevObj != 0) { //just insert hw
		//Relationship between TV and sleep:
		//    begin TV pluging flow ->  wakeup sleep, BUT do not turn on display and backlight
		//    end TV pluging flow ->  stop Sleep counting, turn on display and backlight
		System_ResetPowerSaveCount();
		UI_LockAutoSleep(); //stop sleep counting
		UI_LockAutoPWROff(); //stop power-save counting
	}
#endif
	if (__DBGLVL__ >= __DBGLVL__MSG) {
		GxVideo_DumpInfo();
	}
	if (CurrDevObj) {
		_System_DetachDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if (CurrDev2Obj) {
		_System_DetachDisplay(DOUT2);
	}
#endif
	Display_PauseSplash();
	DBG_MSG("^Y===================================== video off - e\r\n");
	//--------------------------------------------------------------------------
	if (SetModeDevObj != 0) {
		// set mode
		GxVideo_ConfigDeviceFirstMode((UINT32)SetModeDevObj, uiDisplaySetMode);
	}
	//--------------------------------------------------------------------------
	DBG_MSG("^Y===================================== video on - b\r\n");

#if (DISPLAY2_FUNC == ENABLE)
	UINT32 dev1_mask = GxDisplay_Get(DOUT1 | LAYER_OUTPUT, CTRL_STATE_ALLENABLE);
	UINT32 dev2_mask = GxDisplay_Get(DOUT2 | LAYER_OUTPUT, CTRL_STATE_ALLENABLE);
	UINT32 dev1_osd_mask = dev1_mask & (DISP_OSD1_EN|DISP_OSD2_EN);
	UINT32 dev2_osd_mask = dev2_mask & (DISP_OSD1_EN|DISP_OSD2_EN);
	UINT32 dev1_vdo_mask = dev1_mask & (DISP_VDO1_EN|DISP_VDO2_EN);
	UINT32 dev2_vdo_mask = dev2_mask & (DISP_VDO1_EN|DISP_VDO2_EN);
	if ((CurrDevObj != 0) && (CurrDev2Obj == 0) && (NewDevObj != 0) && (NewDev2Obj != 0)) {
		// single display (DOUT1) -> dual display (DOUT1+DOUT2)
		g_bDualUI = TRUE;
		dev2_osd_mask = dev1_osd_mask;
		g_DualVideo = VDOOUT_MASK;
		if (g_DualVideo == DISPLAY_1) {
			dev2_vdo_mask = 0;
		} else if (g_DualVideo == DISPLAY_2) {
			dev2_vdo_mask = dev1_vdo_mask;
			dev1_vdo_mask = 0;
		} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
			dev2_vdo_mask = dev1_vdo_mask;
		}
	} else if ((CurrDevObj == 0) && (CurrDev2Obj != 0) && (NewDevObj != 0) && (NewDev2Obj != 0)) {
		// single display (DOUT2) -> dual display (DOUT1+DOUT2)
		g_bDualUI = TRUE;
		dev1_osd_mask = dev2_osd_mask;
		g_DualVideo = VDOOUT_MASK;
		if (g_DualVideo == DISPLAY_1) {
			dev1_vdo_mask = dev2_vdo_mask;
			dev2_vdo_mask = 0;
		} else if (g_DualVideo == DISPLAY_2) {
			dev1_vdo_mask = 0;
		} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
			dev1_vdo_mask = dev2_vdo_mask;
		}
	} else if ((CurrDevObj != 0) && (CurrDev2Obj == 0) && (NewDevObj == 0) && (NewDev2Obj != 0)) {
		// single display (DOUT1 -> DOUT2)
		g_bDualUI = TRUE;
		dev2_osd_mask = dev1_osd_mask;
		dev1_osd_mask = 0;
		g_DualVideo = DISPLAY_2;
		dev2_vdo_mask = dev1_vdo_mask;
		dev1_vdo_mask = 0;
	} else if ((CurrDevObj == 0) && (CurrDev2Obj != 0) && (NewDevObj != 0) && (NewDev2Obj == 0)) {
		// single display (DOUT2 -> DOUT1)
		g_bDualUI = FALSE;
		dev1_osd_mask = dev2_osd_mask;
		dev2_osd_mask = 0;
		g_DualVideo = DISPLAY_1;
		dev1_vdo_mask = dev2_vdo_mask;
		dev2_vdo_mask = 0;
	} else if ((CurrDevObj != 0) && (CurrDev2Obj != 0) && (NewDevObj == 0) && (NewDev2Obj != 0)) {
		// dual display (DOUT1+DOUT2) -> single display (DOUT2)
		g_bDualUI = FALSE;
		g_DualVideo = DISPLAY_2;
		dev1_osd_mask = 0;
		dev1_vdo_mask = 0;
	} else {
		// dual display (DOUT1+DOUT2) -> single display (DOUT1)
		g_bDualUI = FALSE;
		g_DualVideo = DISPLAY_1;
		dev2_osd_mask = 0;
		dev2_vdo_mask = 0;
	}

	GxDisplay_Set(DOUT1 | LAYER_OUTPUT, CTRL_STATE_ALLENABLE, dev1_osd_mask | dev1_vdo_mask);
	if(g_bDualUI) {
		//not enable OSD layer,OSD buffer address not set, would be error
		GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_ALLENABLE,dev2_vdo_mask);
		//keep mask when View_Window set UIDisplay_Init enable
		g_bDualUI_dev2_osd_mask = dev2_osd_mask;
	} else 	{
		GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_ALLENABLE, dev2_osd_mask | dev2_vdo_mask);
	}


#else
	g_bDualUI = FALSE;
	g_DualVideo = DISPLAY_1;
#endif

	if (NewDevObj) {
		_System_AttachDisplay(DOUT1, NewDevObj);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if (NewDev2Obj) {
		_System_AttachDisplay(DOUT2, NewDev2Obj);
	}
#endif
	if (RemoveDevObj != 0) { //just remove hw
		//Relationship between TV and sleep:
		//    begin TV unplugin flow ->  wakeup sleep, BUT do not turn on display and backlight
		//    end TV unplugin flow ->  start Sleep counting, turn on display and backlight
		UI_UnlockAutoSleep(); //start sleep counting
		UI_UnlockAutoPWROff(); //start power-save counting
	}
	Display_ResumeSplash();
#if 1
	if (SetModeDevObj != 0) {
		if (NewDevObj) {
			_System_ShowDisplay(DOUT1);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if (NewDev2Obj) {
			_System_ShowDisplay(DOUT2);
		}
#endif
	}
#endif
	if (__DBGLVL__ >= __DBGLVL__MSG) {
		GxVideo_DumpInfo();
	}
	UI_UnlockEvent();
	DBG_MSG("^Y===================================== video on - e\r\n");
	//--------------------------------------------------------------------------
#if (LCD_ROTATE_FUNCTION == ENABLE)
	GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_FORCEDETDIR, 1);
	//config video Direct
	Display_DetLCDDir();
#endif

	if ((InsertDevObj == 0) && (RemoveDevObj == 0) && (SetModeDevObj != 0)) {
		//clear
		SetModeDevObj = 0;
		uiDisplaySetMode = 0;
		uiDisplayEnableState_fw = NewState;
	} else {
		//clear
		InsertDevObj = 0;
		RemoveDevObj = 0;
		uiDisplayEnableState_fw = NewState;
	}
}

INT32 System_OnVideoAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	static BOOL bFirst = TRUE;
	//  DISP: if(boot)->start disp srv
	int iCurrMode = System_GetState(SYS_STATE_NEXTMODE);
	DBG_IND("\r\n");
	DBG_DUMP("mode:%d\r\n",iCurrMode);
	if (paramNum == 1) { //end of attach
		//force enable backlight
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
			_System_ShowDisplay(DOUT1);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
			_System_ShowDisplay(DOUT2);
		}
#endif
#if(__DBGLVL__ >= __DBGLVL__MSG)
		DBGD(Perf_GetCurrent());
#endif
		return NVTEVT_CONSUME;
	}

	if (bFirst) {
		TM_BOOT_BEGIN("video", "attach");
	}

#if (PLAY_MODE == ENABLE)
	if (iCurrMode == PRIMARY_MODE_PLAYBACK) {
		System_EnableDisp(DISPLAY_ALL);
	} else
#endif
#if (PHOTO_MODE == ENABLE)
	if (iCurrMode == PRIMARY_MODE_PHOTO) {
		System_EnableDisp(DISPLAY_ALL);
	} else
#endif
#if (IPCAM_MODE == ENABLE)
	if (iCurrMode == PRIMARY_MODE_IPCAM) {
		System_EnableDisp(DISPLAY_ALL);
	} else
#endif
#if (IPCAMPB_MODE == ENABLE)
	if (iCurrMode == PRIMARY_MODE_IPCAMPB) {
		System_EnableDisp(DISPLAY_ALL);
	} else
#endif
#if (MOVIE_MODE == ENABLE)
	if (iCurrMode == PRIMARY_MODE_MOVIE) {
		System_EnableDisp(DISPLAY_ALL);
	} else
#endif
#if (USB_MODE == ENABLE)
	if (iCurrMode == PRIMARY_MODE_USBPCC) {
		System_EnableDisp(DISPLAY_ALL);
	} else
#endif
	{
		System_EnableDisp(DISPLAY_ALL);
	}

#if (LINE_INSERT_FUNCTION == ENABLE)
	//do nothing!
#else //others
	System_SetAudioOutput();
#endif

	if (System_GetState(SYS_STATE_PREVMODE) == SYS_MODE_UNKNOWN) {
#if (DISPLAY_FAST_SWITCH == DISABLE)
		//uItron+Linux boot is very slow, first time SetMode is delayed, but TV is already open
		//but TV is no signal

		//force enable backlight
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
			_System_ShowDisplay(DOUT1);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
			_System_ShowDisplay(DOUT2);
		}
#endif
#endif
		return NVTEVT_CONSUME;
	}

	if (bFirst) {
		TM_BOOT_END("video", "attach");
	}
	bFirst = FALSE;
	return NVTEVT_CONSUME;
}

INT32 System_OnVideoDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

INT32 System_OnVideoDir(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	/*
	此處旋轉作法有很多種可選擇, 就看 project 需求
	=============================================================================================
	(1) 要轉 Display OSD, 但不轉 IDE VDO, 也不轉 Record or Capture 方向
	=>  改 call GxDisplay_SetDestWindow() 去轉 OSD
	    這是靠 IDE 旋轉 OSD layer

	(2) 要轉 Display VDO, 但不轉 IDE OSD, 也不轉 Record or Capture 方向
	=>  改 call DisplayObj DISPLAYER_OP_SET_OUTDIR 去轉 VDO (或靠 DispSrvTsk)
	    這是靠 IDE 旋轉 VDO layer

	(3) 要轉 Display VDO & OSD, 不轉 Record or Capture 方向
	=>  call GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_DIRECT, MIRROR_X|MIRROR_Y); 去轉
	    這要靠 LCD driver 有支援旋轉才能使用

	(4) 要轉 Display VDO & OSD, 同時也轉 Record or Capture 方向
	=>  (API 要問 SENSOR DRIVER)
	    這要靠 sensor driver 有支援旋轉才能使用
	*/

#if ((LCD1_FUNC == ENABLE) && (LCD_ROTATE_FUNCTION == ENABLE))
	UINT32 CurDir = 0;
	UINT32 CurDevID = 0;
	DX_HANDLE VideoDevObj1;
	BOOL bDoRotate180 = FALSE;
	CurDevID = paramArray[0];
	CurDir = paramArray[1];
	DBG_MSG("CurDevID = %08x\r\n", CurDevID);
	DBG_MSG("CurDir = %d\r\n", CurDir);
	VideoDevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);

	if(CurDevID != 0) {
		return NVTEVT_CONSUME;
	}
	if (VideoDevObj1 != gDevLCDObj) {
		return NVTEVT_CONSUME;
	}

	switch (CurDir) {
	//Self-Capture 自拍
	case 0x00:
		bDoRotate180 = TRUE;
		break;
	//Open 打開
	case 0x01:
		bDoRotate180 = FALSE;
		break;
	//Normal 反收
	case 0x02:
		bDoRotate180 = TRUE;
		break;
	//Close 關閉
	case 0x03:
		bDoRotate180 = FALSE;
		break;
	}
	if (bDoRotate180) {
		GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_DIRECT, MIRROR_X | MIRROR_Y);
	} else {
		GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_DIRECT, MIRROR_DISCARD);
	}
#endif
	return NVTEVT_CONSUME;
}



INT32 System_OnVideoInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//IMPORTANT NOTE: to support HDMI, user must enable PIN_I2C_CFG_CH2 in PinmuxCfg.c
	if (paramNum != 1) {
		return NVTEVT_CONSUME;
	}

	InsertDevObj = (DX_HANDLE)(paramArray[0]);

	if (InsertDevObj == 0) {
		return NVTEVT_CONSUME;
	} else {
		char *pDevName = NULL;
		Dx_GetInfo(InsertDevObj, DX_INFO_NAME, &pDevName);
		DBG_DUMP("^Y===================================== video plug [%s]!\r\n", pDevName);
	}

	if (InsertDevObj == gDevLCDObj) {
		_System_ConnectDisplay(0x01);
	}
#if (TV_FUNC == ENABLE)
	if (InsertDevObj == gDevTVObj) {
		_System_ConnectDisplay(0x02);
		#if(IPCAM_FUNC == ENABLE)
			UIAPPIPCAM_VOUT_SETTING *pVoutInfo = UIAppIPCam_get_VoutInfo();
			if(pVoutInfo->bEnable == FALSE){
				DBG_WRN("TV DISABLE, can not attach display!!\r\n");
				return NVTEVT_CONSUME;
			}
		#endif
		#if TV_POWER_SAVE_MODE

			_System_ShowDisplay(DOUT1);
        	_System_AttachDisplay(DOUT1, InsertDevObj);
			#if (DISPLAY2_FUNC == ENABLE)
				_System_ShowDisplay(DOUT2);
        		_System_AttachDisplay(DOUT2, InsertDevObj);
			#endif
			return NVTEVT_CONSUME;
		#endif
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if (InsertDevObj == gDevHDMIObj) {
		//#NT#2016/09/20#Bob Huang -begin
		//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
		UINT32 nDisp = 0, nSensor = 0;
		nDisp = System_GetEnableDispCount();
		nSensor = ImageUnit_GetParam(ISF_VIN(0), ISF_CTRL, VDOIN_PARAM_SENSORCOUNT);

		if (nSensor == 1 && nDisp == 1 && System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
			gb3DNROut = TRUE;
			gui3DNRMovieSize = UI_GetData(FL_MOVIE_SIZE);
			UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_1920x1080P30);
			UI_SetData(FL_MOVIE_SIZE_MENU, MOVIE_SIZE_FRONT_1920x1080P30);
			DBG_DUMP("[3DNR] Insert HDMI and enable 3DNR, previous size index = %d\r\n", gui3DNRMovieSize);
		}
#endif
		//#NT#2016/09/20#Bob Huang -end
		_System_ConnectDisplay(0x04);
	}
#endif
#if (LCD2_FUNC == ENABLE)
	if (InsertDevObj == gDevLCD2Obj) {
		_System_ConnectDisplay(0x08);
	}
#endif

	if (System_GetState(SYS_STATE_CURRMODE) == SYS_MODE_UNKNOWN) {
		DBG_WRN("Unknown MODE, bypass change MODE!\r\n");
		return NVTEVT_CONSUME;
	}

#if (DISPLAY_FAST_SWITCH == DISABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_HideDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_HideDisplay(DOUT2);
	}
#endif

	// change to current UI mode again
	Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));

	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_ShowDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_ShowDisplay(DOUT2);
	}
#endif
#else
#if(__DBGLVL__ >= __DBGLVL__MSG)
	DBGD(Perf_GetCurrent());
#endif
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_HideDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_HideDisplay(DOUT2);
	}
#endif

	// change display device only
	//DBGD(Perf_GetCurrent());
	Ux_PostEvent(NVTEVT_VIDEO_INIT, 0);
	Ux_SendEvent(0, NVTEVT_VIDEO_ATTACH, 0);
	//DBGD(Perf_GetCurrent());
	Ux_PostEvent(NVTEVT_VIDEO_CHANGE, 0);
#endif

	return NVTEVT_CONSUME;
}

INT32 System_OnVideoRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum != 1) {
		return NVTEVT_CONSUME;
	}

	RemoveDevObj = (DX_HANDLE)(paramArray[0]);
	if (RemoveDevObj == 0) {
		return NVTEVT_CONSUME;
	} else {
		char *pDevName = NULL;
		Dx_GetInfo(RemoveDevObj, DX_INFO_NAME, &pDevName);
		DBG_DUMP("^Y===================================== video unplug [%s]!\r\n", pDevName);
	}

	if (RemoveDevObj == gDevLCDObj) {
		_System_DisconnectDisplay(0x01);
	}
#if (TV_FUNC == ENABLE)
	if (RemoveDevObj == gDevTVObj) {
		_System_DisconnectDisplay(0x02);
		#if TV_POWER_SAVE_MODE
        	_System_HideDisplay(DOUT1);
        	_System_DetachDisplay(DOUT1);
			#if (DISPLAY2_FUNC == ENABLE)
        		_System_HideDisplay(DOUT2);
        		_System_DetachDisplay(DOUT2);
			#endif
			return NVTEVT_CONSUME;
		#endif
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if (RemoveDevObj == gDevHDMIObj) {
		//#NT#2016/09/20#Bob Huang -begin
		//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
		if (gb3DNROut) {
			gb3DNROut = FALSE;
			UI_SetData(FL_MOVIE_SIZE, gui3DNRMovieSize);
			UI_SetData(FL_MOVIE_SIZE_MENU, gui3DNRMovieSize);
			DBG_DUMP("[3DNR] Remove HDMI and disable 3DNR, recover previous size index = %d\r\n", gui3DNRMovieSize);
		}
#endif
		//#NT#2016/09/20#Bob Huang -end
		_System_DisconnectDisplay(0x04);
	}
#endif
#if (LCD2_FUNC == ENABLE)
	if (RemoveDevObj == gDevLCD2Obj) {
		_System_DisconnectDisplay(0x08);
	}
#endif

	if (System_GetState(SYS_STATE_CURRMODE) == SYS_MODE_UNKNOWN) {
		DBG_WRN("Unknown MODE, bypass change MODE!\r\n");
		return NVTEVT_CONSUME;
	}

#if (DISPLAY_FAST_SWITCH == DISABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_HideDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_HideDisplay(DOUT2);
	}
#endif

	// change to current UI mode again
	Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));

	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_ShowDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_ShowDisplay(DOUT2);
	}
#endif
#else
#if(__DBGLVL__ >= __DBGLVL__MSG)
	DBGD(Perf_GetCurrent());
#endif
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_HideDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_HideDisplay(DOUT2);
	}
#endif

	// change display device only
	//DBGD(Perf_GetCurrent());
	Ux_PostEvent(NVTEVT_VIDEO_INIT, 0);
	Ux_SendEvent(0, NVTEVT_VIDEO_ATTACH, 0);
	//DBGD(Perf_GetCurrent());
	Ux_PostEvent(NVTEVT_VIDEO_CHANGE, 0);
#endif

	return NVTEVT_CONSUME;
}

INT32 System_OnVideoMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DX_HANDLE CurDevObj = 0;
#if (DISPLAY2_FUNC == ENABLE)
	DX_HANDLE CurDev2Obj = 0;
#endif
	DX_HANDLE NewDevObj = 0;
	UINT32 VideoMode = 0; //new display
	if (paramNum < 2) {
		return NVTEVT_CONSUME;
	}

	NewDevObj = (DX_HANDLE)(paramArray[0]);
	VideoMode = paramArray[1];
	if (NewDevObj == 0) {
		return NVTEVT_CONSUME;
	}

	CurDevObj = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
#if (DISPLAY2_FUNC == ENABLE)
	CurDev2Obj = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
#endif
	//check is current open device?
#if (DISPLAY2_FUNC == ENABLE)
	if ((CurDevObj != NewDevObj) && (CurDev2Obj != NewDevObj)) {
#else
	if ((CurDevObj != NewDevObj)) {
#endif
		char *pDevName = NULL;
		Dx_GetInfo(NewDevObj, DX_INFO_NAME, &pDevName);
		//not current device! => only config mode for next time.
		GxVideo_ConfigDeviceFirstMode((UINT32)NewDevObj, VideoMode);
		DBG_DUMP("^Yvideo config mode [%s] %d\r\n", pDevName, VideoMode);
		return NVTEVT_CONSUME;
	}

	{
		char *pDevName = NULL;
		Dx_GetInfo(NewDevObj, DX_INFO_NAME, &pDevName);
		DBG_DUMP("^Y===================================== video setmode [%s] %d\r\n", pDevName, VideoMode);
	}
	//it is current device => set mode immediately!
	SetModeDevObj = NewDevObj;
	uiDisplaySetMode = VideoMode;
#if 0
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_HideDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_HideDisplay(DOUT2);
	}
#endif
#endif
	// change to current UI mode again
	Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
#if 0
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
		_System_ShowDisplay(DOUT1);
	}
#if (DISPLAY2_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
		_System_ShowDisplay(DOUT2);
	}
#endif
#endif

	return NVTEVT_CONSUME;
}

/////////////////////////////////////////////////////////////////////////////
#if(IPCAM_FUNC==ENABLE)
#include "AppDisp_IPCamView.h"
#else
#if (PIP_VIEW_FUNC == ENABLE)
#include "AppDisp_PipView.h"
#endif
#if (SBS_VIEW_FUNC == ENABLE)
#include "AppDisp_SbsView.h"
#endif
#if (MOVIE_PLAY_VIEW_FUNC == ENABLE)
#include "AppDisp_MoviePlayView.h"
#endif
#endif

#if _TODO
static VIEW_SRC gDisp_src = {
	TRUE, //BYPASS DISP-SRV, DIRECT OUTPUT TO DISPLAY
	VIEW_HANDLE_PRIMARY,
	NULL,
};

void System_DispSourceByTarget(ISIZE *pImgSize, USIZE *pAspectRatio)
{
	if (gDisp_src.fpSetImgCb) {
		gDisp_src.fpSetImgCb(pImgSize, pAspectRatio);
	}
}
#else
void System_DispSourceByTarget(ISIZE *pImgSize, USIZE *pAspectRatio)
{
}
#endif

extern void VdoOut_CB(UINT32 event, UINT32 param1, UINT32 param2);

void System_DispSource(UINT32 SensorMask, UINT32 nSensor)
{
#if _TODO
	UINT32 nDisp;
#if(IPCAM_FUNC==DISABLE)
	UINT32 iSensorDisp = SensorMask;
#endif
#endif
	UINT32 CurDevObj1;
#if (DISPLAY2_FUNC == ENABLE)
#if (LCD2_FUNC == ENABLE || TV_FUNC == ENABLE || HDMI_FUNC == ENABLE)
	UINT32 CurDevObj2;
#endif
#endif

#if _TODO
#if (DISPLAY2_FUNC == ENABLE)
	nDisp = System_GetEnableDispCount();
#endif

	//config display view source
	gDisp_src.bDirectMode = FALSE;
	gDisp_src.fpDrawCb = NULL;
	gDisp_src.fpSetImgCb = NULL;
	gDisp_src.UserHandle = VIEW_HANDLE_PRIMARY;
#if (DISPLAY2_FUNC == ENABLE)
	if (nDisp > 1) {
		gDisp_src.UserHandle = VIEW_HANDLE_BOTH;
	}
#endif

#if(IPCAM_FUNC==ENABLE)
	{
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj) {
#if _TODO
			gDisp_src.bDirectMode = FALSE;
			gDisp_src.fpDrawCb = IPCamView_OnDraw;
			gDisp_src.fpSetImgCb = IPCamView_SetImg;
			//ImageApp_IPCam_Config(IPCAM_CFG_HDMI_OUT,FALSE);
#else
			gDisp_src.bDirectMode = TRUE;
			gDisp_src.fpDrawCb = NULL;
			gDisp_src.fpSetImgCb = NULL;
			//ImageApp_IPCam_Config(IPCAM_CFG_HDMI_OUT,TRUE);
#endif
		}
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevTVObj) {
#if _TODO
			gDisp_src.bDirectMode = FALSE;
			gDisp_src.fpDrawCb = IPCamView_OnDraw;
			gDisp_src.fpSetImgCb = IPCamView_SetImg;
			// ImageApp_IPCam_Config(IPCAM_CFG_HDMI_OUT,FALSE);
#else
			gDisp_src.bDirectMode = TRUE;
			gDisp_src.fpDrawCb = NULL;
			gDisp_src.fpSetImgCb = NULL;
			//ImageApp_IPCam_Config(IPCAM_CFG_HDMI_OUT,TRUE);
#endif
		}
#if (HDMI_FUNC == ENABLE)
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevHDMIObj) {
			gDisp_src.bDirectMode = TRUE;
			gDisp_src.fpDrawCb = NULL;
			gDisp_src.fpSetImgCb = NULL;
			//ImageApp_IPCam_Config(IPCAM_CFG_HDMI_OUT,TRUE);
		}
#endif
	}
#else
	// SingleSensor/ SingleDisplay
	//#NT#2016/09/20#Bob Huang -begin
	//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
	if (gb3DNROut) {
		gDisp_src.bDirectMode = FALSE;
		gDisp_src.fpDrawCb = NULL;
		gDisp_src.fpSetImgCb = NULL;
	} else
#endif
		//#NT#2016/09/20#Bob Huang -end
		if ((nSensor == 1) && (nDisp == 1)) {
			gDisp_src.bDirectMode = TRUE;
			gDisp_src.fpDrawCb = NULL;
			gDisp_src.fpSetImgCb = NULL;
		} else
			// SingleSensor/ DualDisplay
#if (DISPLAY2_FUNC == ENABLE)
			if ((nSensor == 1) && (nDisp == 2)) {
				gDisp_src.bDirectMode = TRUE;
				gDisp_src.fpDrawCb = NULL;
				gDisp_src.fpSetImgCb = NULL;
			} else
#endif
			// DualSensor/ SingleDisplay, IMAGEPIPE_PARAM_SENSORDISPLAY == SENSOR_1, must be bDirectMode = TRUE
			if ((nSensor == 2) && (nDisp == 1) && (iSensorDisp == SENSOR_1)) {
				gDisp_src.bDirectMode = TRUE;
				gDisp_src.fpDrawCb = NULL;
				gDisp_src.fpSetImgCb = NULL;
			} else
			// DualSensor/ SingleDisplay, IMAGEPIPE_PARAM_SENSORDISPLAY == SENSOR_2, must be bDirectMode = TRUE
			if ((nSensor == 2) && (nDisp == 1) && (iSensorDisp == SENSOR_2)) {
				gDisp_src.bDirectMode = TRUE;
				gDisp_src.fpDrawCb = NULL;
				gDisp_src.fpSetImgCb = NULL;
			} else
			// DualSensor/ SingleDisplay, IMAGEPIPE_PARAM_SENSORDISPLAY == (SENSOR_1|SENSOR_2), must be bDirectMode = FALSE
			if ((nSensor == 2) && (nDisp == 1) && (iSensorDisp == (SENSOR_1 | SENSOR_2))) {
#if (PIP_VIEW_FUNC == ENABLE)
				gDisp_src.bDirectMode = FALSE;
				gDisp_src.fpDrawCb = PipView_OnDraw;
				gDisp_src.fpSetImgCb = PipView_SetImg;
#elif (SBS_VIEW_FUNC == ENABLE)
				gDisp_src.bDirectMode = FALSE;
				gDisp_src.fpDrawCb = SbsView_OnDraw;
				gDisp_src.fpSetImgCb = SbsView_SetImg;
#else
				DBG_ERR("Not support PipView!\r\n");
				gDisp_src.bDirectMode = TRUE;
				gDisp_src.fpDrawCb = NULL;
				gDisp_src.fpSetImgCb = NULL;
#endif
			}
#if (DISPLAY2_FUNC == ENABLE)
			else
			// DualSensor/ DualDisplay, IMAGEPIPE_PARAM_SENSORDISPLAY == (SENSOR_1|SENSOR_2), must be bDirectMode = TRUE
			if ((nSensor == 2) && (nDisp == 2) && (iSensorDisp == (SENSOR_1 | SENSOR_2))) {
				gDisp_src.bDirectMode = TRUE;
				gDisp_src.fpDrawCb = NULL;
				gDisp_src.fpSetImgCb = NULL;
			}
#endif
	//#NT#2016/06/17#Brain Yen -begin
	//#NT#for special aspect panel and pipview
#if (PIP_VIEW_LR == ENABLE)
	gDisp_src.bDirectMode = FALSE;
	gDisp_src.fpDrawCb = PipView_OnDraw;
	gDisp_src.fpSetImgCb = PipView_SetImg;
#endif
	//#NT#2016/06/7#Brain Yen -end

#if ((PIP_VIEW_FUNC == ENABLE) || (SBS_VIEW_FUNC == ENABLE))
	if ((gDisp_src.bDirectMode == TRUE) && (gDisp_src.fpDrawCb == NULL)) //if not enable PIPVIEW or SBSVIEW, allow enable ROTATEVIEW
#endif
	{
		//fixed: if single sensor and enable LCD rotate, after TV/HDMI plugin, output screen will become horizontal patterns
#if ((LCD1_FUNC == ENABLE) && (VDO_USE_ROTATE_BUFFER == ENABLE))
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj) {
			gDisp_src.bDirectMode = FALSE;
			gDisp_src.fpDrawCb = RotateView_OnDraw;
			gDisp_src.fpSetImgCb = RotateView_SetImg;
		}
#endif
	}
#endif
	ImageUnit_Begin(&ISF_VdoOut1, 0);
	ImageUnit_SetParam(ISF_CTRL, VDOOUT_PARAM_VIEWSRC, (UINT32)&gDisp_src);
	ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_EVENTCB, (UINT32)VdoOut_CB);
	ImageUnit_End();
#if (DISPLAY2_FUNC == ENABLE)
	ImageUnit_Begin(&ISF_VdoOut2, 0);
	ImageUnit_SetParam(ISF_CTRL, VDOOUT_PARAM_VIEWSRC, (UINT32)&gDisp_src);
	ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_EVENTCB, (UINT32)VdoOut_CB);
	ImageUnit_End();
#endif
#else
	ImageUnit_Begin(&ISF_VdoOut1, 0);
	ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_EVENTCB, (UINT32)VdoOut_CB);
	ImageUnit_End();
#if (DISPLAY2_FUNC == ENABLE)
	ImageUnit_Begin(&ISF_VdoOut2, 0);
	ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_EVENTCB, (UINT32)VdoOut_CB);
	ImageUnit_End();
#endif
#endif

	///////////////////////////////////////////////////////////
	CurDevObj1 = (UINT32)GxVideo_GetDevice(DOUT1);
#if (DISPLAY2_FUNC == ENABLE)
#if (LCD2_FUNC == ENABLE || TV_FUNC == ENABLE || HDMI_FUNC == ENABLE)
	CurDevObj2 = (UINT32)GxVideo_GetDevice(DOUT2);
#if _TODO
	if ((nSensor == 2) && (nDisp == 1) && (gDisp_src.bDirectMode == FALSE)) { //use PIPVIEW to merge 2 sensor to 1 disp
		CurDevObj2 = CurDevObj1; //this will let display buffer format of 2nd sensor is equal to 1st sensor
	}
#endif
#endif
#endif
	ImageUnit_Begin(&ISF_VdoOut1, 0);
#if (LCD1_FUNC == ENABLE)
	if (CurDevObj1 == (UINT32)gDevLCDObj) { //case 1
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FORMAT, LCD_PREVIEW_VDO_FMT);
	}
#endif
#if (TV_FUNC == ENABLE)
	if (CurDevObj1 == (UINT32)gDevTVObj) { //case 2
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FORMAT, TV_PREVIEW_VDO_FMT);
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if (CurDevObj1 == (UINT32)gDevHDMIObj) { //case 3
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FORMAT, HDMI_PREVIEW_VDO_FMT);
	}
#endif
	ImageUnit_End();
#if (DISPLAY2_FUNC == ENABLE)
	ImageUnit_Begin(&ISF_VdoOut2, 0);
#if (LCD2_FUNC == ENABLE)
	if (CurDevObj2 == (UINT32)gDevLCD2Obj) { //case 1
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FORMAT, LCD2_PREVIEW_VDO_FMT);
	}
#endif
#if (TV_FUNC == ENABLE)
	if (CurDevObj2 == (UINT32)gDevTVObj) { //case 2
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FORMAT, TV_PREVIEW_VDO_FMT);
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if (CurDevObj2 == (UINT32)gDevHDMIObj) { //case 3
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FORMAT, HDMI_PREVIEW_VDO_FMT);
	}
#endif
	ImageUnit_End();
#endif
}


/////////////////////////////////////////////////////////////////////////////
#include "GxDisplay.h"
//#include "GxImageJPG.h"
#if(UI_FUNC==ENABLE)
#include "UIFlow.h"
#endif


extern UINT32   p_gLogoJPG;
extern UINT32   p_gLogoSize;

//extern BOOL gIsUSBChargePreCheck;
//DISPLAY_CFG disp_cfg = {0};


void Load_DispLogo(void)
{
	//Obsolete
}

static void UI_ShowJPG(UINT8 LayerID, UINT32 uiJPGData, UINT32 uiJPGSize, UINT32 uiTempBuf, UINT32 uiTempSize)
{
	UINT16 w = 0, h = 0, fmt = 0;
	UINT32 uiOutAddrY, uiOutAddrCb, uiOutAddrCr;
	ISIZE             DeviceSize;
	DeviceSize = GxVideo_GetDeviceSize(DOUT1);

#if 0 //(_FPGA_EMULATION_ == ENABLE)
	uiOutAddrY = uiTempBuf;
	uiOutAddrCb = uiTempBuf + (320 * 240);
	uiOutAddrCr = uiTempBuf + (320 * 240);
	w = 320;
	h = 240;
	fmt = DISP_PXLFMT_YUV420_PK;
#else
	GXIMGDEC_SCALE_DOWN_LEVEL ScaleDownlevel;
	MEM_RANGE DstMem;
	MEM_RANGE JpgData;
	//fix for CID 50701 - begin
	GXIMG_HEADER_INFO JpgInfo = {0};
	IMG_BUF DstImg = {0};
	//fix for CID 50701 - end

#if(VDO_USE_ROTATE_BUFFER == ENABLE)
	//GX_IMAGE_PIXEL_FMT GxImgFMT = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
	IMG_BUF   DispImg = {0};
	IMG_BUF   DispOutImg = {0};
	UINT32    uiPoolAddr, uiPoolSize;

	uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO2TEMP);
	uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_VDO2TEMP);
#endif

	JpgData.Addr = uiJPGData;
	JpgData.Size = uiJPGSize;

	GxImgFile_ParseHeader(&JpgData, &JpgInfo);

	DstMem.Addr = uiTempBuf;
	DstMem.Size = uiTempSize;
	DBG_MSG("Decode JPG %08x %08x\r\n", uiTempBuf, uiTempSize);

	if (GxImgFile_AllocDecBuf(&DstImg, &DstMem, &JpgInfo, &ScaleDownlevel) != E_OK) {
		DBG_ERR("target temp is too small\r\n");
		return;
	}

	if (GxImgFile_Decode(&DstImg, &JpgData) != E_OK) {
		DBG_ERR("decode error\r\n");
		return;
	}

	uiOutAddrY = DstImg.PxlAddr[0];
	uiOutAddrCb = DstImg.PxlAddr[1];
	uiOutAddrCr = DstImg.PxlAddr[2];
	w = DstImg.LineOffset[0];
	h = DstImg.Height;

	switch (DstImg.PxlFmt) {
	case GX_IMAGE_PIXEL_FMT_YUV422_PACKED:
		fmt = DISP_PXLFMT_YUV422_PK;
		break;
	case GX_IMAGE_PIXEL_FMT_YUV420_PACKED:
		fmt = DISP_PXLFMT_YUV420_PK;
		break;
	default:
		DBG_ERR("unknown format\r\n");
		break;
	}
#endif
#if 1
	if (LayerID == LAYER_VDO1) { //VDO1
#if(VDO_USE_ROTATE_BUFFER == ENABLE)
		DX_HANDLE CurDevObj;
		CurDevObj = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
		if (CurDevObj == (DX_HANDLE)gDevLCDObj) {
			if (fmt == DISP_PXLFMT_YUV422_PK) {
				GxImg_InitBuf(&DispImg, w, h, GX_IMAGE_PIXEL_FMT_YUV422, GXIMAGE_LINEOFFSET_ALIGN(16), uiTempBuf, w * h * 2);
			} else {
				GxImg_InitBuf(&DispImg, w, h, GX_IMAGE_PIXEL_FMT_YUV420, GXIMAGE_LINEOFFSET_ALIGN(16), uiTempBuf, w * h * 2);
			}
			//#NT#2014/03/25#KS Hung -begin
			GxImg_RotateData(&DispImg, uiPoolAddr, uiPoolSize, GX_IMAGE_ROTATE_270, &DispOutImg);
			//#NT#2014/03/25#KS Hung -end
			DBG_MSG("DispSize %d %d\r\n", DeviceSize.w, DeviceSize.h);
			DBG_MSG("Show YUV (%dx%d) %08x %08x %08x\r\n", h, w, DispOutImg.PxlAddr[0], DispOutImg.PxlAddr[1], DispOutImg.PxlAddr[2]);
			Display_SetBuffer(LAYER_VDO1, h, w, h, DISP_PXLFMT_YUV420_PK, DispOutImg.PxlAddr[0], DispOutImg.PxlAddr[1], DispOutImg.PxlAddr[2]);
			Display_SetWindow(LAYER_VDO1, 0, 0, DeviceSize.h, DeviceSize.w);
		} else {
			DBG_MSG("DispSize %d %d\r\n", DeviceSize.w, DeviceSize.h);
			DBG_MSG("Show YUV (%dx%d) %08x %08x %08x\r\n", w, h, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
			Display_SetBuffer(LAYER_VDO1, w, h, w, fmt, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
			Display_SetWindow(LAYER_VDO1, 0, 0, DeviceSize.w, DeviceSize.h);
		}
#else
		DBG_MSG("DispSize %d %d\r\n", DeviceSize.w, DeviceSize.h);
		DBG_MSG("Show YUV (%dx%d) %08x %08x %08x\r\n", w, h, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
		Display_SetBuffer(LAYER_VDO1, w, h, w, fmt, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
		Display_SetWindow(LAYER_VDO1, 0, 0, DeviceSize.w, DeviceSize.h);
#endif
		Display_SetEnable(LAYER_VDO1, TRUE);
		Display_SetEnable(LAYER_VDO2, FALSE);
	}
	if (LayerID == LAYER_VDO2) { //VDO2
		DBG_MSG("DispSize %d %d\r\n", DeviceSize.w, DeviceSize.h);
		DBG_MSG("Show YUV (%dx%d) %08x %08x %08x\r\n", w, h, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
		Display_SetBuffer(LAYER_VDO2, w, h, w, fmt, uiOutAddrY, uiOutAddrCb, uiOutAddrCr);
		Display_SetWindow(LAYER_VDO2, 0, 0, DeviceSize.w, DeviceSize.h);
		Display_SetEnable(LAYER_VDO1, FALSE);
		Display_SetEnable(LAYER_VDO2, TRUE);
	}
#endif
}

static BOOL gbShowSplash = FALSE;

void Display_ShowSplash(SPLASH_ID splash_id)
{
	UINT32  uiPoolAddr;
	UINT32  uiPoolSize;
	if (gbShowSplash) {
		return;
	}

	//get temp buffer
	uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO2);
	uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_VDO2);
	if (uiPoolAddr == 0) {
		return;
	}

#if 1
	//load default logo
	if (splash_id == SPLASH_POWERON) {
#if (POWERONLOGO_FUNCTION == ENABLE)
		p_gLogoJPG = (UINT32)g_ucBGOpening;
		p_gLogoSize = Logo_getBGOpening_size();
#endif
	} else {
#if (POWEROFFLOGO_FUNCTION == ENABLE)
		p_gLogoJPG = (UINT32)g_ucBGGoodbye;
		p_gLogoSize = Logo_getBGGoodbye_size();
#endif
	}
	//show logo
	DBG_IND("Show Logo\r\n");
	UI_ShowJPG(LOGO_DISP_LAYER, (UINT32)p_gLogoJPG, p_gLogoSize, uiPoolAddr, uiPoolSize);
	gbShowSplash = TRUE;
#endif
}

void Display_HideSplash(void)
{
	UINT32 LayerID = LOGO_DISP_LAYER;
	if (!gbShowSplash) {
		return;
	}
	DBG_IND("Hide Logo\r\n");
	if (LayerID == LAYER_VDO1) {
	}
	if (LayerID == LAYER_VDO2) {
		Display_SetEnable(LAYER_VDO2, FALSE);
	}
	gbShowSplash = FALSE;
}

void Display_ShowPreview(void)
{
	static BOOL bFirst = TRUE;
	//#NT#2016/03/07#KCHong -begin
	//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
	// Do not show preview if boot from alarm
	if (g_bBootFromALM == TRUE) {
		return;
	}
#endif
	//#NT#2016/03/07#KCHong -end

	if (bFirst) {
		TM_BOOT_BEGIN("video", "hide_logo");
	}
	DBG_IND("Show Video\r\n");
#if (DISPLAY2_FUNC == ENABLE)
	if (g_DualVideo & DISPLAY_1) {
		Display_SetEnable(LAYER_VDO1, TRUE);
	}
	if (g_DualVideo & DISPLAY_2) {
		Display_SetEnable(DOUT2 | LAYER_VDO1, TRUE);
	}
#else
	Display_SetEnable(LAYER_VDO1, TRUE);
#endif

#if (POWERONLOGO_FUNCTION == ENABLE)
	Display_HideSplash();
#else
	GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
#endif
	if (bFirst) {
		TM_BOOT_END("video", "hide_logo");
		SysExam_SetTimeTag(SYSEXAM_TMTAG_PREVIEW_RDY);
	}
	bFirst = FALSE;
}

void Display_PauseSplash(void)
{
	UINT32 LayerID = LOGO_DISP_LAYER;
	if (!gbShowSplash) {
		return;
	}
	if (LayerID == LAYER_VDO1) {
	}
	if (LayerID == LAYER_VDO2) {
		Display_SetEnable(LAYER_VDO2, FALSE);
	}
}

void Display_ResumeSplash(void)
{
	UINT32 LayerID = LOGO_DISP_LAYER;
	ISIZE             DeviceSize;
	DeviceSize = GxVideo_GetDeviceSize(DOUT1);
	if (!gbShowSplash) {
		return;
	}
	if (LayerID == LAYER_VDO1) {
	}
	if (LayerID == LAYER_VDO2) {
		DBG_MSG("%d %d\r\n", DeviceSize.w, DeviceSize.h);
		Display_SetWindow(LAYER_VDO2, 0, 0, DeviceSize.w, DeviceSize.h);
		Display_SetEnable(LAYER_VDO2, TRUE);
	}
}


void SysVideo_OnVideoInsert(void)
{

#if(IPCAM_FUNC == ENABLE)

	INT32 iDispPath=0;
	iDispPath= UIAppIPCam_get_DisplayPath();
	UIAPPIPCAM_VOUT_SETTING *pVoutInfo = UIAppIPCam_get_VoutInfo();
	if(iDispPath != -1){

    	ISF_PORT_STATE DispStatus = 0;
    	DispStatus =UIAppIPCam_Get_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT);
		if(DispStatus == ISF_PORT_STATE_RUN){
			////already open , no need open again
			return;
		}


		//InsertDevObj = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
    	_System_ConnectDisplay(0x02);
    	//_System_ShowDisplay(DOUT1);
		if(pVoutInfo->sMode <=UIAPPIPCAM_VOUT_AUTO){
			#if (HDMI_FUNC == ENABLE)
			_System_AttachDisplay(DOUT1, gDevHDMIObj);
			#endif

		}
		else if(pVoutInfo->sMode > UIAPPIPCAM_VOUT_AUTO &&
			        pVoutInfo->sMode < UIAPPIPCAM_VOUT_SETTING_MAX){
			DBG_DUMP("^YTV\r\n");
			#if (TV_FUNC == ENABLE)
    		_System_AttachDisplay(DOUT1, gDevTVObj);
			#endif
		}
		else{

			_System_AttachDisplay(DOUT1, gDevLCDObj);
		}

			//_System_AttachDisplay(DOUT1, 0);
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
			_System_ShowDisplay(DOUT1);
			ImageUnit_Begin(&ISF_VdoOut1, 0);
			USIZE DeviceRatio;
			DeviceRatio = GxVideo_GetDeviceAspect(0);
			ImageUnit_SetVdoAspectRatio(ISF_IN1, DeviceRatio.w, DeviceRatio.h);
			ImageUnit_End();
		}
		else{
			DBG_WRN("can not get Device!!\r\n");
		}
		UIAppIPCam_Set_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT,ISF_PORT_STATE_RUN);
		ImageApp_IPCam_VideoOut_set_status(iDispPath,ISF_PORT_STATE_RUN);

	}
#else
	DBG_ERR("not support!\r\n");
#endif


}
void SysVideo_OnVideoRemove(void)
{
#if (IPCAM_FUNC == ENABLE)
	INT32 iDispPath=0;
	iDispPath= UIAppIPCam_get_DisplayPath();

	if(iDispPath != -1){

   		ISF_PORT_STATE DispStatus = 0;
    	DispStatus =UIAppIPCam_Get_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT);
		if(DispStatus != ISF_PORT_STATE_RUN){
			////already remove , no need remove again
			return;
		}

		UIAppIPCam_Set_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT,ISF_PORT_STATE_OFF);
		ImageApp_IPCam_VideoOut_set_status(iDispPath,ISF_PORT_STATE_OFF);

    	//do disable TV device
    	_System_DisconnectDisplay(0x02);
    	_System_HideDisplay(DOUT1);
    	_System_DetachDisplay(DOUT1);

	}



#else
	DBG_ERR("not support!!\r\n");
#endif

}
UINT32 SysVideo_GetDir(void)
{
	DBG_ERR("not support replace by SysVideo_GetDirbyID\r\n");
	return ISF_VDO_DIR_ROTATE_0;

	#if VDO_USE_ROTATE_BUFFER
	// dual display case , only display video on HDMI
	if (System_GetEnableDispCount()==2)
		return ISF_VDO_DIR_ROTATE_0;
	else if (((DX_HANDLE)GxVideo_GetDevice(DOUT1)) == gDevLCDObj)
		return VDO_ROTATE_DIR;
	else
	#endif
		return ISF_VDO_DIR_ROTATE_0;
}
//DevID is DOUT1,DOUT2
UINT32 SysVideo_GetDirbyID(UINT32 DevID)
{
	#if VDO_USE_ROTATE_BUFFER
	if (((DX_HANDLE)GxVideo_GetDevice(DevID)) == gDevLCDObj)
		return VDO_ROTATE_DIR;
	else
	#endif
		return ISF_VDO_DIR_ROTATE_0;
}

