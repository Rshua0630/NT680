/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       DetTV.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect USB is plugging in or unplugged
                Detect USB is plugging in or unplugged

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxDisplay.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "DispDef.h"

//driver
#include "display.h"
#include "Audio.h"
#include "pll.h"
#include "stdio.h" //for sscanf

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define HDMI_SUPPORT_AUTO       ENABLE //DISABLE
#if !(defined(_SENSOR1_CMOS_OFF_) && defined(_SENSOR2_CMOS_OFF_) && defined(_SENSOR3_CMOS_OFF_) && defined(_SENSOR4_CMOS_OFF_))
#define HDMI_SUPPORT_UHD        DISABLE //3840X2160
#define HDMI_SUPPORT_4K2K       DISABLE //4096X2160
#define HDMI_SUPPORT_14B        DISABLE //HDMI 1.4b
#else
#define HDMI_SUPPORT_UHD        ENABLE //3840X2160
#define HDMI_SUPPORT_4K2K       DISABLE //4096X2160
#define HDMI_SUPPORT_14B        ENABLE //HDMI 1.4b
#endif

#if (_FPGA_EMULATION_ == ENABLE)
//do not replace video mode!
#undef HDMI_SUPPORT_AUTO
#define HDMI_SUPPORT_AUTO       DISABLE
#endif

#define HDMI_MAX_W              ALIGN_CEIL_16(1920)  //device w of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16
#define HDMI_MAX_H              ALIGN_CEIL_16(1080)  //device h of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16
#define HDMI_FORMAT_DEFAULT     HDMI_1920X1080I60

#if (HDMI_SUPPORT_UHD == ENABLE)
#undef HDMI_MAX_W
#undef HDMI_MAX_H
#undef HDMI_FORMAT_DEFAULT
#define HDMI_MAX_W              ALIGN_CEIL_16(3840)  //device w of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16
#define HDMI_MAX_H              ALIGN_CEIL_16(2160)  //device h of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16
#define HDMI_FORMAT_DEFAULT     HDMI_3840X2160P30

#if (HDMI_SUPPORT_4K2K == ENABLE)
#undef HDMI_MAX_W
#undef HDMI_MAX_H
#undef HDMI_FORMAT_DEFAULT
#define HDMI_MAX_W              ALIGN_CEIL_16(4096)  //device w of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16
#define HDMI_MAX_H              ALIGN_CEIL_16(2160)  //device h of DISP_HDMIMODE_xxx, for H264 decoder, it must align to 16
#define HDMI_FORMAT_DEFAULT     HDMI_4096X2160P30
#endif
#endif

#if (HDMI_SUPPORT_AUTO == ENABLE)
#undef HDMI_FORMAT_DEFAULT
#define HDMI_FORMAT_DEFAULT     DISP_HDMIMODE_MAX
#endif

static DISPDEV_PARAM   DispDev;
static DISPCTRL_PARAM  DispCtrl;
static PDISP_OBJ       pObj = 0;
static PDISPDEV_PARAM  pDev = &DispDev;
static PDISPCTRL_PARAM pCtrl = &DispCtrl;

static UINT32          g_HDMIEnable = FALSE; //not yet enable
// Exported Variables
static UINT32          g_HDMIDout = 0; //current dout
static UINT32          g_HDMIAudioMode = HDMI_AUDIO48KHZ; //current audio mode
static UINT32          g_HDMIMode = 0; //current mode
static UINT32          g_HDMILastMode = 0; //current mode

// Local Variables
static UINT32          g_HDMISleep = FALSE; //not yet sleep
static UINT32          g_HDMIBacklightEn = FALSE;

static BOOL g_bHDMIWaitTurnOnFinish = FALSE;

static UINT32 g_localMode = 0; //new mode
static USIZE  g_localSize = {0, 0}; //new size
static USIZE *pCurrentSize = 0; //return size

typedef struct _HDMI_FORMAT {
	UINT32          uiIndex;
	char            str[16];
	HDMI_VIDEOID    VideoID;
	UINT32          uiWidth;
	UINT32          uiHeight;

} HDMI_FORMAT;

HDMI_FORMAT g_HDMIFormat[] = {
	{ 1,    {"640x480p60"},     HDMI_640X480P60,    640,    480},
	{ 2,    {"720x480p60"},     HDMI_720X480P60,    720,    480},
	{ 3,    {"720x576p50"},     HDMI_720X576P50,    720,    576},
	{ 4,    {"1280x720p60"},    HDMI_1280X720P60,   1280,   720},
	{ 5,    {"1920x1080p30"},   HDMI_1920X1080P30,  1920,   1080},
	{ 6,    {"1920x1080i50"},   HDMI_1920X1080I50,  1920,   1080},
	{ 7,    {"1920x1080i60"},   HDMI_1920X1080I60,  1920,   1080},
	{ 8,    {"1920x1080p60"},   HDMI_1920X1080P60,  1920,   1080},
#if (HDMI_SUPPORT_UHD == ENABLE)
#if (HDMI_SUPPORT_14B == ENABLE)
	{ 9,    {"3840x2160p24-14B"},   HDMI14B_3840X2160P24,  3840,   2160},
	{ 10,    {"3840X2160p25-14B"},   HDMI14B_3840X2160P24,  3840,   2160},
	{ 11,   {"3840x2160p30-14B"},   HDMI14B_3840X2160P30,  3840,   2160},
#endif
	{ 12,   {"3840x2160p24"},   HDMI_3840X2160P24,  3840,   2160},
	{ 13,   {"3840x2160p25"},   HDMI_3840X2160P25,  3840,   2160},
	{ 14,   {"3840x2160p30"},   HDMI_3840X2160P30,  3840,   2160},
#if (HDMI_SUPPORT_4K2K == ENABLE)
#if (HDMI_SUPPORT_14B == ENABLE)
	{ 15,   {"4096x2160p24-14B"},   HDMI14B_4096X2160P24,  4096,   2160},
#endif
	{ 16,   {"4096x2160p24"},   HDMI_4096X2160P24,  4096,   2160},
	{ 17,   {"4096x2160p25"},   HDMI_4096X2160P25,  4096,   2160},
	{ 18,   {"4096x2160p30"},   HDMI_4096X2160P30,  4096,   2160},
#endif
#endif
};


#define HDMI_FORMAT_MAX     (sizeof(g_HDMIFormat)/ sizeof(HDMI_FORMAT))


//public func
static UINT32 DrvHDMIGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
static UINT32 DrvHDMISetcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
static UINT32 DrvHDMIInit(void *pInitParam); // Set Init Parameters
static UINT32 DrvHDMIOpen(void); // Common Constructor
static UINT32 DrvHDMIClose(void); // Common Destructor
static UINT32 DrvHDMIState(UINT32 StateID, UINT32 Value); // General Properties
static UINT32 DrvHDMIControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
static UINT32 DrvHDMICommand(CHAR *pcCmdStr); //General Command Console

//internal func
static void DrvHDMI_TurnOn(void);
static void DrvHDMI_TurnOn_WaitFinish(void);
static void DrvHDMI_TurnOff(void);
static void DrvHDMI_SleepEnter(void);
static void DrvHDMI_SleepLeave(void);
static BOOL GPIOMap_DetHDMI(void);
static void GPIOMap_TurnOnHDMIBacklight(void);
static void GPIOMap_TurnOffHDMIBacklight(void);


//dx object
DX_OBJECT gDevHDMI = {
	DXFLAG_SIGN,
	DX_CLASS_DISPLAY_EXT | DX_TYPE_HDMIOUT,
	DISPLAY_VER,
	"Display_HDMI",
	0, 0, 0, 0,
	DrvHDMIGetcaps,
	DrvHDMISetcfgs,
	DrvHDMIInit,
	DrvHDMIOpen,
	DrvHDMIClose,
	DrvHDMIState,
	DrvHDMIControl,
	DrvHDMICommand,
	0,
};

static void _DrvHDMIHookDout(UINT32 id)
{
	pObj = 0;
	if (id == 0) {
		pObj = disp_getDisplayObject(DISP_1);
	}
	if (id == 1) {
		pObj = disp_getDisplayObject(DISP_2);
	}
	if (!pObj) {
		DBG_ERR("invalid display id = %d!\r\n", id);
		return;
	}
#if defined(_HDMI_ON_)
	{
		//static BOOL bHDMIHook[2] = {FALSE,FALSE};
		//if(!bHDMIHook[id])
		{
			pDev->SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_TVHDMI;
			pDev->SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getHdmiDevObj();
			pObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT, pDev);
			//bHDMIHook[id] = TRUE;
		}
	}
#endif
}

static UINT32 DrvHDMIGetVideoMode(void)
{
	UINT32  i, j;
	UINT32  uiEDID;
	UINT32  uiLen;
	UINT32  uiVideoMode = HDMI_FORMAT_DEFAULT;
	HDMI_VDOABI VideoAbility[32] = {0}; // EDID max is 32
	HDMI_FORMAT *pHDMIFormat = NULL;

	//force set to 4K2K mode for demo!
	//return HDMI_3840X2160P30;

	hdmitx_open();

	uiEDID = hdmitx_getEdidInfo();
	if (uiEDID) {
		DBG_DUMP("HDMI get EDID = %08x\r\n", uiEDID);

		hdmitx_getVideoAbility(&uiLen, VideoAbility);

		for (i = 0; i < uiLen; i++) {
			DBG_DUMP("HDMI Video Ability %d: %d\r\n", i, VideoAbility[i].VideoID);
			for (j = 0; j < HDMI_FORMAT_MAX; j++) {
				if ((g_HDMIFormat[j].VideoID == VideoAbility[i].VideoID)
					&& (g_HDMIFormat[j].uiWidth <= HDMI_MAX_W)
					&& (g_HDMIFormat[j].uiHeight <= HDMI_MAX_H)) {
					if (pHDMIFormat == NULL) {
						pHDMIFormat = &g_HDMIFormat[j];
						DBG_DUMP(" -match: %d %s\r\n", j, pHDMIFormat->str);
					} else if (g_HDMIFormat[j].uiIndex > pHDMIFormat->uiIndex) {
						pHDMIFormat = &g_HDMIFormat[j];
						DBG_DUMP(" -match: %d %s\r\n", j, pHDMIFormat->str);
					}
				}
			}
		}

		if ((pHDMIFormat == NULL) || (pHDMIFormat->VideoID < HDMI_3840X2160P24)) {
#if (HDMI_SUPPORT_UHD == ENABLE)
			j = 9;
			if ((uiEDID & HDMITX_DTD3840X2160P30)
				&& (g_HDMIFormat[j].uiWidth <= HDMI_MAX_W)
				&& (g_HDMIFormat[j].uiHeight <= HDMI_MAX_H)) {
				pHDMIFormat = &g_HDMIFormat[j];
			}
#if (HDMI_SUPPORT_4K2K == ENABLE)
			j = 12;
			if ((uiEDID & HDMITX_DTD4096X2160P30)
				&& (g_HDMIFormat[j].uiWidth <= HDMI_MAX_W)
				&& (g_HDMIFormat[j].uiHeight <= HDMI_MAX_H)) {
				pHDMIFormat = &g_HDMIFormat[j];
			}
#endif
#endif
		}

		if (pHDMIFormat == NULL) {
			DBG_ERR("cannot match HDMI format!\r\n");
		} else {
			DBG_DUMP(" -match HDMI format: %s\r\n", pHDMIFormat->str);
			uiVideoMode = pHDMIFormat->VideoID;
		}
	} else {
		DBG_ERR("No EDID, is DVI?\r\n");
	}

	hdmitx_close();

	return uiVideoMode;
}

static UINT32 DrvHDMIGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID & 0x0000ffff) {
	case DISPLAY_CAPS_BASE:
		//DBG_IND("get HDMI plug caps\r\n");
		v = DISPLAY_BF_BACKLIGHT | DISPLAY_BF_DETPLUG;
		break;
	case DISPLAY_CAPS_PLUG:
		//DBG_IND("det HDMI plug status\r\n");
		v = GPIOMap_DetHDMI();
		break;
	case DISPLAY_CAPS_DEFMODE:
		v = HDMI_FORMAT_DEFAULT;
		break;
	case DISPLAY_CAPS_MODEINFO: {
			int mode = (CapID & 0xffff0000) >> 16;
			ISIZE *pModeInfo = (ISIZE *)Param1;
			_DrvHDMIHookDout(g_HDMIDout);
			pDev->SEL.SET_HDMIMODE.VideoID = mode;
			pDev->SEL.SET_HDMIMODE.AudioID = HDMI_AUDIO_NO_CHANGE;
			pObj->devCtrl(DISPDEV_SET_HDMIMODE, pDev);
			pDev->SEL.GET_PREDISPSIZE.DevID = DISPDEV_ID_TVHDMI;
			pObj->devCtrl(DISPDEV_GET_PREDISPSIZE, pDev);
			if (pModeInfo) {
				pModeInfo->w = pDev->SEL.GET_PREDISPSIZE.uiBufWidth;
				pModeInfo->h = pDev->SEL.GET_PREDISPSIZE.uiBufHeight;
			}
		}
		break;
	case DISPLAY_CAPS_MAXSIZE: {
			ISIZE *pSizeInfo = (ISIZE *)Param1;
			if (pSizeInfo) {
				pSizeInfo->w = HDMI_MAX_W;
				pSizeInfo->h = HDMI_MAX_H;
			}
		}
		break;
	case DISPLAY_CAPS_ASPECT: {
			ISIZE *pAspectInfo = (ISIZE *)Param1;
			if (pAspectInfo) {
				pAspectInfo->w = 16;
				pAspectInfo->h = 9;
			}
		}
		break;
	case DISPLAY_CAPS_VIDMODE:
#if (HDMI_SUPPORT_AUTO == ENABLE)
		v = DrvHDMIGetVideoMode();
#else
		v = HDMI_FORMAT_DEFAULT;
#endif
		break;
	default:
		break;
	}
	return v;
}

static UINT32 DrvHDMISetcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	switch (CfgID) {
	case DISPLAY_CFG_MODE:
		g_HDMILastMode = Param1;
		break;
	case DISPLAY_CFG_DOUT:
		g_HDMIDout = Param1;
		break;
	case DISPLAY_CFG_INFOBUF:
		pCurrentSize = (USIZE *)Param1;
		break;
	case DISPLAY_CFG_AUDIOFMT:
		/*
		NT96660 HDMI 有支援的是: 32 kHz, 44.1 kHz, 48 kHz 三種
		>> 沒設定過的話, default 為 48 kHz
		>> 如果設定沒支援的值, 會 return DX_NOT_SUPPORT, 並保持現況
		>> 如果設定是某兩個值的中間值, 也會 return DX_NOT_SUPPORT, 並保持現況
		>> 如果 call API 時間點是 HDMI open 的狀況, 會 return DX_OK, 會把設定立刻生效
		>> 如果 call API 時間點是 HDMI close 的狀況, 會 return DX_NOT_OPEN, 會把設定 keep 下來, 待下次 HDMI open 時同時生效
		*/
		{
			//ER retV = E_OK;
			//UINT32 openAud = FALSE;
			//UINT32 aud_sample = 0;
#if(__DBGLVL__ > 2)
			HDMI_AUDIOFMT hdmi_currAudioMode;
#endif
			switch (Param1) {
			case HDMI_AUDIO32KHZ:
				//aud_sample = AUDIO_SR_32000;
				break;
			case HDMI_AUDIO44_1KHZ:
				//aud_sample = AUDIO_SR_44100;
				break;
			case HDMI_AUDIO48KHZ:
				//aud_sample = AUDIO_SR_48000;
				break;
			default:
				return DX_NOT_SUPPORT;
			}

			g_HDMIAudioMode = Param1; //keep HDMI audio mode

			if (g_HDMIEnable == FALSE) {
				DBG_IND("keep!\r\n");
				return DX_NOT_OPEN;
			}

#if(__DBGLVL__ > 2)
			hdmi_currAudioMode = hdmitx_getConfig(HDMI_CONFIG_ID_AUDIO);
			DBG_IND("old HDMI audio mode %d\r\n", hdmi_currAudioMode);
			DBG_IND("new HDMI audio mode %d\r\n", g_HDMIAudioMode);
#endif

			//change HDMI audio mode NOW!
			DBG_IND("change NOW!\r\n");
			pDev->SEL.SET_HDMIMODE.VideoID    = HDMI_VID_NO_CHANGE;
			pDev->SEL.SET_HDMIMODE.AudioID    = g_HDMIAudioMode;
			pObj->devCtrl(DISPDEV_SET_HDMIMODE, pDev);
		}
		break;
	default:
		break;
	}
	return DX_OK;
}

static UINT32 DrvHDMIInit(void *pInitParam) // Set Init Parameters
{
	DBG_FUNC_BEGIN("\r\n");

	g_HDMILastMode = DrvHDMIGetcaps(DISPLAY_CAPS_DEFMODE, 0);

	return DX_OK;
}

static UINT32 DrvHDMIOpen(void) // Common Constructor
{
	DBG_FUNC_BEGIN("\r\n");

	_DrvHDMIHookDout(g_HDMIDout);
	DBG_IND("HDMI dout by IDE%d\r\n", g_HDMIDout + 1);
	////////////////////////////////////////////////////
#if (_FPGA_EMULATION_ == ENABLE)
	DBG_WRN("Please make sure {PIN_FUNC_I2C, PIN_I2C_CFG_CH5}! and pick FPGA OSC to 27Mhz!\r\n");
	// FPGA version: FS_SA1 10/24 或 10/25 的版本
	// power supply => 7V 1.2A

	pll_setPLLEn(PLL_ID_2, ENABLE);

	// HDMI need PLL2 for precisely CLK
	if (g_HDMIDout == 0) {
		pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL2;
		pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
	}
	if (g_HDMIDout == 1) {
		pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_IDE2_PLL2;
		pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
	}
#else
	// HDMI need PLL2 for precisely CLK
	if (g_HDMIDout == 0) {
		pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL2;
		pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
	}
	if (g_HDMIDout == 1) {
		pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_IDE2_PLL2;
		pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
	}
#endif
	////////////////////////////////////////////////////

	DBG_DUMP(" -mode %08x\r\n", g_HDMILastMode);
#if (HDMI_SUPPORT_AUTO == ENABLE)
	if (g_HDMILastMode == DISP_HDMIMODE_MAX) {
		DBG_DUMP(" -HDMI auto mode - begin\r\n");
		g_HDMILastMode = DrvHDMIGetcaps(DISPLAY_CAPS_VIDMODE, 0); // get HDMI video mode ability
		DBG_DUMP(" -HDMI auto mode - end\r\n");
	}
#else
	DBG_DUMP(" -HDMI user mode\r\n");
#endif

	g_localMode = g_HDMILastMode;
	DBG_IND("HDMI set mode %08x\r\n", g_localMode);
	DrvHDMI_TurnOn();

	g_HDMIEnable = TRUE;

	return DX_OK;
}

static UINT32 DrvHDMIClose(void) // Common Destructor
{
	DBG_FUNC_BEGIN("\r\n");

	DrvHDMI_TurnOff();

	g_HDMILastMode = g_HDMIMode;
	g_HDMIMode = 0;
	g_HDMIEnable = FALSE;

	return DX_OK;
}

static void DrvHDMI_TurnOn(void)
{
	DBG_FUNC_BEGIN("\r\n");
	// Init HDMI object
	if (g_localMode < DISP_HDMIMODE_MAX) { //must be HDMI
		pDev->SEL.SET_HDMIMODE.VideoID    = g_localMode;
		pDev->SEL.SET_HDMIMODE.AudioID    = g_HDMIAudioMode;
		pObj->devCtrl(DISPDEV_SET_HDMIMODE, pDev);
		/*
		pDev->SEL.GET_PREDISPSIZE.DevID = DISPDEV_ID_TVHDMI;
		pObj->devCtrl(DISPDEV_GET_PREDISPSIZE,pDev);
		g_localSize.w    = pDev->SEL.GET_PREDISPSIZE.uiBufWidth;
		g_localSize.h    = pDev->SEL.GET_PREDISPSIZE.uiBufHeight;
		*/
	} else {
		DBG_ERR("HDMI fmt = %d is incorrect!\r\n", g_localMode);
		return;
	}

	//IMPORTANT! for FAST BOOT, Photo App will ask disp size very early~
	//Update size of first mode (before open)
	//DxDisplay_UpdateModeInfo(g_HDMIDout<<4, g_localMode, &g_localSize);

	pDev->SEL.OPEN_DEVICE.DevID = DISPDEV_ID_TVHDMI;
	pObj->devCtrl(DISPDEV_OPEN_DEVICE, pDev);

	pObj->devCtrl(DISPDEV_GET_DISPSIZE, pDev);
	g_localSize.w = pDev->SEL.GET_DISPSIZE.uiBufWidth;
	g_localSize.h = pDev->SEL.GET_DISPSIZE.uiBufHeight;
	if (pCurrentSize) {
		pCurrentSize[0] = g_localSize;
	}

	g_HDMIBacklightEn = TRUE;

	g_HDMIMode = g_localMode;
}

static void DrvHDMI_TurnOn_WaitFinish(void)
{
	DBG_FUNC_BEGIN("\r\n");

	g_HDMIEnable = TRUE;
}


static void DrvHDMI_TurnOff(void)
{
	DBG_FUNC_BEGIN("\r\n");

#if 1
	pObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);
	g_HDMIBacklightEn = FALSE;
#endif

	g_HDMIEnable = FALSE;
}

static void DrvHDMI_SleepEnter(void)
{
	GPIOMap_TurnOffHDMIBacklight();

	// Disable IDE
	//ide_set_en(FALSE);
	pObj->waitFrmEnd();
	pCtrl->SEL.SET_ENABLE.bEn = FALSE;
	pObj->dispCtrl(DISPCTRL_SET_ENABLE, pCtrl);

	g_HDMISleep = TRUE;
}

static void DrvHDMI_SleepLeave(void)
{
	// Enable IDE
	//ide_set_en(TRUE);
	pObj->waitFrmEnd();
	pCtrl->SEL.SET_ENABLE.bEn = TRUE;
	pObj->dispCtrl(DISPCTRL_SET_ENABLE, pCtrl);

	GPIOMap_TurnOnHDMIBacklight();
	g_HDMISleep = FALSE;
}

static UINT32 DrvHDMIState(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_FUNC_BEGIN("\r\n");

	if (StateID & DXGET) {
		UINT32 rvalue =  0;
		StateID &= ~DXGET;
		switch (StateID) {
		case DRVDISP_STATE_ENBALE:
			//lost device? ---> cannot determine enable or not
			//under flow like: enable/disable
			rvalue = g_HDMIEnable;
			break;
		case DRVDISP_STATE_MODE:
			//lost device? ---> cannot determine mode
			//under flow like: change mode
			rvalue = g_HDMIMode;
			break;
		case DRVDISP_STATE_LASTMODE:
			rvalue =  g_HDMILastMode;
			break;
		case DRVDISP_STATE_BACKLIGHT:
			rvalue = g_HDMIBacklightEn;
			break;
		case DRVDISP_STATE_SLEEP:
			rvalue = g_HDMISleep;
			break;
		default:
			DBG_WRN("get state=0x%02X not support!\r\n", StateID);
			break;
		}
		return rvalue;
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		switch (StateID) {
		case DRVDISP_STATE_BACKLIGHT:
			if (Value == DRVDISP_BACKLIGHT_OFF) {
				DBG_IND("DRVDISP_BACKLIGHT_OFF\r\n");
				GPIOMap_TurnOffHDMIBacklight();
			} else { //if(value == DRVDISP_BACKLIGHT_ON)
				DBG_IND("DRVDISP_BACKLIGHT_ON\r\n");
				GPIOMap_TurnOnHDMIBacklight();
			}
			break;
		case DRVDISP_STATE_SLEEP:
			if (Value == TRUE) {
				DBG_IND("DRVDISP_CTRL_SLEEP 1\r\n");
				DrvHDMI_SleepEnter();
			} else if (Value == FALSE) {
				DBG_IND("DRVDISP_CTRL_SLEEP 0\r\n");
				DrvHDMI_SleepLeave();
			}
			break;
		default:
			DBG_WRN("set state=0x%02X not support!\r\n", StateID);
			break;
		}
	}
	return DX_OK;
}

static UINT32 DrvHDMIControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
	DBG_FUNC_BEGIN("\r\n");

	switch (CtrlID) {
	case DRVDISP_CTRL_MODE: {
			//DrvHDMI_TurnOff();
			g_localMode = Param1;
			//DrvHDMI_TurnOn();
			//g_bHDMIWaitTurnOnFinish = TRUE;
		}
		break;
	case DRVDISP_CTRL_WAITFINISH:
		if (g_bHDMIWaitTurnOnFinish == TRUE) {
			DrvHDMI_TurnOn_WaitFinish();
			g_bHDMIWaitTurnOnFinish = FALSE;
		}
		break;

	default:
		DBG_WRN("ctrl id=0x%02X not support!\r\n", CtrlID);
		break;
	}
	return DX_OK;
}

static UINT32 DrvHDMICommand(CHAR *pcCmdStr) //General Command Console
{
	switch (*pcCmdStr) {
	case 'd':
		/*if (!strncmp(pcCmdStr, "disp tv dump", 12))
		{
		    ...
		    return TRUE;
		}*/
		break;
#if (USE_VIO == ENABLE)
	case 'v':
		if (!strncmp(pcCmdStr, "vdet", 4)) {
			char ts[10];
			UINT32 v1;
			sscanf_s(pcCmdStr, "%s %lu", ts, sizeof(ts), &v1); //fix for CID 44013
			DBG_IND("v1 = %d\r\n", v1);
			vio_setpin(VIO_HDMI_DET, v1);
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}


/**
  Turn on HDMI backlight

  Turn on HDMI backlight.

  @param void
  @return void
*/
static void GPIOMap_TurnOnHDMIBacklight(void)
{
	DBG_IND("GPIOMap_TurnOnHDMIBacklight\r\n");

	if (g_HDMIBacklightEn == FALSE) {
		// HDMI AV ON
		pDev->SEL.SET_POWERDOWN.bPowerDown = 0x00; //FALSE = power on
		pObj->devCtrl(DISPDEV_SET_POWERDOWN, pDev);
		g_HDMIBacklightEn = TRUE;
	}
}

/**
  Turn off HDMI backlight

  Turn off HDMI backlight.

  @param void
  @return void
*/
static void GPIOMap_TurnOffHDMIBacklight(void)
{
	DBG_IND("GPIOMap_TurnOffHDMIBacklight\r\n");

	if (g_HDMIBacklightEn == TRUE) {
		// HDMI AV MUTE
		pDev->SEL.SET_POWERDOWN.bPowerDown = 0x01; //TRUE = power off.
		pObj->devCtrl(DISPDEV_SET_POWERDOWN, pDev);
		g_HDMIBacklightEn = FALSE;
	}
}

/**
  Detect HDMI is plugging in or unplugged

  Detect HDMI is plugging in or unplugged.
  Return TRUE if HDMI is plugging in, FALSE if HDMI is unplugged.

  @param void
  @return BOOL: TRUE -> HDMI is plugging in, FALSE -> HDMI is unplugged
*/
static BOOL GPIOMap_DetHDMI(void)
{
#if (HDMI_DET_CTRL == HDMI_DET_DUMMY)
	return TRUE;
#endif
#if ((HDMI_DET_CTRL == HDMI_DET_BY_VIO) && (USE_VIO == ENABLE))
	return (vio_getpin(VIO_HDMI_DET) != 0);
#endif
#if (HDMI_DET_CTRL == HDMI_DET_BY_HOTPLUG)
	return hdmitx_checkHotplug();
#else
	return 0;
#endif
}

//@}
