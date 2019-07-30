
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxDisplay.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "DispDef.h"

//driver
#include "display.h"
#include "sif.h"
#include "dma.h"

//drvier ext
#include "Utility.h"
#include "Debug.h"
#include "pll.h"
#include "string.h"
#include "stdio.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if defined(_Disp_VIRTUAL_LCD1_OFF_)
#define LCD_MAX_W                       ALIGN_CEIL_16(80)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(60)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    4
#define LCD_ASPECT_H                    3
#elif defined(_Disp_IF8B_LCD1_AUCN01_)
#define LCD_MAX_W                       ALIGN_CEIL_16(320)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(240)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    4
#define LCD_ASPECT_H                    3
#elif defined(_Disp_IF8B_LCD1_D200N9309V3_ST7789V_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(240) //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(320)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    3
#define LCD_ASPECT_H                    4
#elif defined(_Disp_IF8B_LCD1_ILI9341_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(320)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(720)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    3
#define LCD_ASPECT_H                    4
#elif defined(_Disp_IF8B_LCD1_ILI9342C_) 
#define LCD_MAX_W                       ALIGN_CEIL_16(960)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(240)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    4
#define LCD_ASPECT_H                    3
#elif defined(_Disp_IF8B_LCD1_PW27P05_ILI8961_)
#define LCD_MAX_W                       ALIGN_CEIL_16(320)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(240)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    16
#define LCD_ASPECT_H                    9
#elif defined(_Disp_IF8B_LCD1_PW35P00_HX8238D_)
#define LCD_MAX_W                       ALIGN_CEIL_16(960)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(240)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    4
#define LCD_ASPECT_H                    3
#elif defined(_Disp_IF8B_LCD1_S2003T26_ST7789_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(240) //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(320)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    3
#define LCD_ASPECT_H                    4
#elif defined(_Disp_IFDSI_LCD1_ILI9806E_ZH233G2402A_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(480) //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(720)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    9
#define LCD_ASPECT_H                    16
#elif defined(_Disp_IFDSI_LCD1_NT35510_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(240)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(320)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    3
#define LCD_ASPECT_H                    4
#elif defined(_Disp_IFDSI_LCD1_NT35521_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(720)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(1280)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    9
#define LCD_ASPECT_H                    16
#elif defined(_Disp_IFDSI_LCD1_S2003T26_ST7789_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(240) //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(320)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    3
#define LCD_ASPECT_H                    4
#elif defined(_Disp_IFDSI_LCD1_TG078UW006A0_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(400) //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(1280)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    5
#define LCD_ASPECT_H                    16
#elif defined(_Disp_IFDSI_LCD1_UT35067A0_ILI9488_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(320) //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(480)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    9
#define LCD_ASPECT_H                    16
#elif defined(_Disp_IFPA_LCD1_ST7796S_) // ROTATE panel !!!
#define LCD_MAX_W                       ALIGN_CEIL_16(320) //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(480)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    2
#define LCD_ASPECT_H                    3
#else
#define LCD_MAX_W                       ALIGN_CEIL_16(320)  //device w of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_MAX_H                       ALIGN_CEIL_16(240)  //device h of DISP_LCDMODE_xxx, to support rotate, it must align to 16
#define LCD_ASPECT_W                    4
#define LCD_ASPECT_H                    3
#endif
#define DISP_DUAL       DISABLE

static DISPDEV_PARAM   DispDev;
static PDISP_OBJ       pObj = 0;
static PDISPDEV_PARAM  pDev = &DispDev;

#if 1 //(_FPGA_EMULATION_ != ENABLE)
static DISPCTRL_PARAM  DispCtrl;
static PDISPCTRL_PARAM pCtrl = &DispCtrl;
#endif

#define LCDMODE                     0

//Configure
#define CFG_IDE_DMA_HIGH DISABLE  //Enable will cause IPE bandwith not enough

//MIPS TODO : FPGA phase these three param are useless
#define PAD_DS_LCD8         PAD_DS_LGPIO8
//#define PAD_DS_LCD          PAD_DS_DGPIO2
#define PAD_DRIVINGSINK_8MA PAD_DRIVINGSINK_10MA
#define PAD_DRIVINGSINK_2MA PAD_DRIVINGSINK_2P5MA
#define PAD_DRIVINGSINK_4MA PAD_DRIVINGSINK_5MA
#define PAD_DRIVINGSINK_6MA PAD_DRIVINGSINK_7P5MA

// Local Variables
static UINT32   g_LCDDout = 0; //current dout
static UINT32   g_LCDMode = 0; //current mode
static UINT32   g_LCDLastMode = 0; //current mode
static UINT32   g_LCDEnable = FALSE; //not yet enable
static UINT32   g_LCDSleep = FALSE;
#define DRVLCD_BRT_LEVEL_MAX      10
#define DRVLCD_BRT_LEVEL_INIT     5
static BOOL     g_LCDBacklightEn = FALSE;
static INT32    g_LCDBacklightLvl = DRVLCD_BRT_LEVEL_INIT;

static UINT32   g_LCDDual = FALSE;

// LCD control object
static BOOL g_bLCDWaitTurnOnFinish = FALSE;

static UINT32 g_localMode = 0; //new mode
static USIZE  g_localSize = {0, 0}; //new size
static USIZE *pCurrentSize = 0; //return size

//public func
static UINT32 DrvLCDGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
static UINT32 DrvLCDSetcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
static UINT32 DrvLCDInit(void *pInitParam); // Set Init Parameters
static UINT32 DrvLCDOpen(void); // Common Constructor
static UINT32 DrvLCDClose(void); // Common Destructor
static UINT32 DrvLCDState(UINT32 StateID, UINT32 Value); // General Properties
static UINT32 DrvLCDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
static UINT32 DrvLCDCommand(CHAR *pcCmdStr); //General Command Console

//internal func
static void DrvLCD_TurnOn(void);
static void DrvLCD_TurnOn_WaitFinish(void);
static void DrvLCD_TurnOff(void);
static void DrvLCD_Dump(void);
static void DrvLCD_SleepEnter(void);
static void DrvLCD_SleepLeave(void);

static void     GPIOMap_TurnOnLCDBacklight(void);
static void     GPIOMap_TurnOffLCDBacklight(void);
static void     GPIOMap_SetLCDBacklightBrightLevel(INT32 uiLevel);
static void     GPIOMap_AdjustLCDBacklight(UINT32 uiAdjValue);
static BOOL     GPIOMap_IsLCDBacklightOn(void);
static void     GPIOMap_DumpBacklight(void);

static void     GPIOMap_TurnOnLCDPower(void);
static void     GPIOMap_TurnOffLCDPower(void);
static void     GPIOMap_LCDReset(void);
//static BOOL     GPIOMap_IsLCDPowerOn(void);

//dx object
DX_OBJECT gDevLCD = {
	DXFLAG_SIGN,
	DX_CLASS_DISPLAY_EXT | DX_TYPE_LCD,
	DISPLAY_VER,
	"Display_LCD",
	0, 0, 0, 0,
	DrvLCDGetcaps,
	DrvLCDSetcfgs,
	DrvLCDInit,
	DrvLCDOpen,
	DrvLCDClose,
	DrvLCDState,
	DrvLCDControl,
	DrvLCDCommand,
	0,
};

static int _DrvLCDHookDout(UINT32 id)
{
	pObj = 0;
	if (id == 0) {
		pObj = disp_getDisplayObject(DISP_1);
	}
	if (id == 1) {
		//pObj = disp_getDisplayObject(DISP_2);
		DBG_ERR("LCD1 not support DOUT2!\r\n");
		return 0;
	}
	if (!pObj) {
		DBG_ERR("invalid display id = %d!\r\n", id);
		return 0;
	}
	////////////////////////////////////////////////////
	if (g_LCDDout == 0) {
		static BOOL bLCDHook = FALSE;
		if (!bLCDHook) {
			pDev->SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_PANEL;
			pDev->SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getLcd1DevObj();
			pObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT, pDev);
			bLCDHook = TRUE;
		}
	}
	////////////////////////////////////////////////////
	return 1;
}

static UINT32 DrvLCDGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID & 0x0000ffff) {
	case DISPLAY_CAPS_BASE:
		v = DISPLAY_BF_BACKLIGHT;
		break;
	case DISPLAY_CAPS_DEFMODE:
		v = LCDMODE;
		break;
	case DISPLAY_CAPS_MODEINFO: {
			int mode = (CapID & 0xffff0000) >> 16;
			ISIZE *pModeInfo = (ISIZE *)Param1;
			_DrvLCDHookDout(g_LCDDout);
			pDev->SEL.SET_LCDMODE.Mode = mode;
			pObj->devCtrl(DISPDEV_SET_LCDMODE, pDev);
			pDev->SEL.GET_PREDISPSIZE.DevID = DISPDEV_ID_PANEL;
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
				pSizeInfo->w = LCD_MAX_W;
				pSizeInfo->h = LCD_MAX_H;
			}
		}
		break;
	case DISPLAY_CAPS_ASPECT: {
			ISIZE *pAspectInfo = (ISIZE *)Param1;
			if (pAspectInfo) {
				pAspectInfo->w = LCD_ASPECT_W;
				pAspectInfo->h = LCD_ASPECT_H;
			}
		}
		break;
	default:
		break;
	}
	return v;
}

static UINT32 DrvLCDSetcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	switch (CfgID) {
	case DISPLAY_CFG_MODE:
		DBG_IND("set mode %08x\r\n", Param1);
		g_LCDLastMode = Param1;
		break;
	case DISPLAY_CFG_DOUT:
		g_LCDDout = Param1;
		break;
	case DISPLAY_CFG_DUAL:
		g_LCDDual = Param1;
		break;
	case DISPLAY_CFG_INFOBUF:
		pCurrentSize = (USIZE *)Param1;
		break;
	default:
		break;
	}
	return DX_OK;
}

static UINT32 DrvLCDInit(void *pInitParam) // Set Init Parameters
{
	DBG_FUNC_BEGIN("\r\n");

	g_LCDLastMode = DrvLCDGetcaps(DISPLAY_CAPS_DEFMODE, 0);

	return DX_OK;
}

static UINT32 DrvLCDOpen(void) // Common Constructor
{
	DBG_FUNC_BEGIN("\r\n");

	if (!_DrvLCDHookDout(g_LCDDout)) {
		return DX_NOT_SUPPORT;
	}
	DBG_IND("LCD dout by IDE%d\r\n", g_LCDDout + 1);
	////////////////////////////////////////////////////
	// LCD default use PLL2 for precisely CLK, but if enable dual, it can use PLL1.
#if (_FPGA_EMULATION_ == ENABLE)
	pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL2;
	pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
#else
	if (g_LCDDual == FALSE) {
		DBG_IND("^YLCD Dual=0\r\n");
		// NOTE: Our LCD device driver will always try to change PLL clock setting for its "target frame rate"
		// Solution: select PLL1(fixed at 480Mhz) or PLL2(used by TV/HDMI) or PLL4(support SpreadSpectrum)
		if (g_LCDDout == 0) {
			pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL2;
			pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
		}
	} else {
		DBG_IND("^YLCD Dual=1\r\n");
		// NOTE: Under "Dual Display" condition, PLL2 clock must be fixed and reserved for TV/HDMI device!
		// Solution: Do NOT select PLL2(used by TV/HDMI)! Just use PLL1(fixed at 480Mhz) or PLL4(support SpreadSpectrum)
		if (g_LCDDout == 0) {
			pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL1;
			pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
		}
	}
#endif
	////////////////////////////////////////////////////
	//Panel have 3 types: NORMAL(8BIT or PARAL),DSI(MIPI), MI(MEMORY-INTERFACE)
	if (g_LCDDout == 0) {
#if (LCD_COMM_CTRL == LCD_COMM_BY_GPIO)
		pDev->SEL.SET_REG_IF.LcdCtrl     = DISPDEV_LCDCTRL_GPIO;
		pDev->SEL.SET_REG_IF.uiSifCh     = 0;
		pDev->SEL.SET_REG_IF.uiGpioSen   = GPIO_LCD_SIF_SEN;
		pDev->SEL.SET_REG_IF.uiGpioClk   = GPIO_LCD_SIF_SCK;
		pDev->SEL.SET_REG_IF.uiGpioData  = GPIO_LCD_SIF_SDA;
		pObj->devCtrl(DISPDEV_SET_REG_IF, pDev);
#endif
#if (LCD_COMM_CTRL == LCD_COMM_BY_SIF)
		pDev->SEL.SET_REG_IF.LcdCtrl     = DISPDEV_LCDCTRL_SIF;
		pDev->SEL.SET_REG_IF.uiSifCh     = SIF_LCD_CH;
		pDev->SEL.SET_REG_IF.uiGpioSen   = 0;
		pDev->SEL.SET_REG_IF.uiGpioClk   = 0;
		pDev->SEL.SET_REG_IF.uiGpioData  = 0;
		pObj->devCtrl(DISPDEV_SET_REG_IF, pDev);
		//Configure SIF pins to default state
		pad_setPullUpDown(PAD_LCD_SIF_SEN, DIR_LCD_SIF_SEN);
		pad_setPullUpDown(PAD_LCD_SIF_SCK, DIR_LCD_SIF_SCK);
		pad_setPullUpDown(PAD_LCD_SIF_SDA, DIR_LCD_SIF_SDA);
#endif
		pad_setDrivingSink(PAD_DS_LCD8, PAD_DRIVINGSINK_7P5MA);
	}

	////////////////////////////////////////////////////

	GPIOMap_TurnOnLCDPower();
	GPIOMap_LCDReset();

	g_localMode = g_LCDLastMode;
	DBG_IND("open mode %08x\r\n", g_localMode);
	DrvLCD_TurnOn();

	g_LCDEnable = TRUE;

	return DX_OK;
}

static UINT32 DrvLCDClose(void) // Common Destructor
{
	DBG_FUNC_BEGIN("\r\n");

	DrvLCD_TurnOff();

	// Panel enter sleep mode
	GPIOMap_TurnOffLCDPower();

	g_LCDLastMode = g_LCDMode;
	g_LCDMode = 0;
	g_LCDEnable = FALSE;

	return DX_OK;
}

static void DrvLCD_TurnOn(void)
{
	DBG_FUNC_BEGIN("\r\n");

	// Turn on LCD power

	if (g_localMode < DISPDEV_LCDMODE_NO_CHANGE) { //must be LCD
		pDev->SEL.SET_LCDMODE.Mode = g_localMode;
		pObj->devCtrl(DISPDEV_SET_LCDMODE, pDev);
		/*
		pDev->SEL.GET_PREDISPSIZE.DevID = DISPDEV_ID_PANEL;
		pObj->devCtrl(DISPDEV_GET_PREDISPSIZE,pDev);
		g_localSize.w     = pDev->SEL.GET_PREDISPSIZE.uiBufWidth;
		g_localSize.h    = pDev->SEL.GET_PREDISPSIZE.uiBufHeight;
		*/
	} else {
		DBG_ERR("mode=%d not support!\r\n", g_localMode);
		return;
	}

	//IMPORTANT! for FAST BOOT, Photo App will ask disp size very early~
	//Update size of first mode (before open)
	//DxDisplay_UpdateModeInfo(g_LCDDout<<4, g_localMode, &g_localSize);

	//LCD panel open
	pDev->SEL.OPEN_DEVICE.DevID = DISPDEV_ID_PANEL;
	pObj->devCtrl(DISPDEV_OPEN_DEVICE, pDev);

	//////////////////////////////////////////////////////////////////////
	// special operation for this project!
	//
	//pDev->SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_180;
	//pObj->devCtrl(DISPDEV_SET_ROTATE,pDev);
	//////////////////////////////////////////////////////////////////////

	pObj->devCtrl(DISPDEV_GET_DISPSIZE, pDev);
	g_localSize.w = pDev->SEL.GET_DISPSIZE.uiBufWidth;
	g_localSize.h = pDev->SEL.GET_DISPSIZE.uiBufHeight;
	if (pCurrentSize) {
		pCurrentSize[0] = g_localSize;
	}

	g_LCDMode = g_localMode;
}

static void DrvLCD_TurnOn_WaitFinish(void)
{
	DBG_FUNC_BEGIN("\r\n");

	g_LCDEnable = TRUE;
}


static void DrvLCD_TurnOff(void)
{
	DBG_FUNC_BEGIN("\r\n");

	//NOTE: VERY VERY VERY VERY VERY IMPORTANT!!!!!
	//上層要確保SleepEnter()跟TurnOff()不會重複發生!!!
	//if DrvLCD_SleepEnter() is called ClosePanel, then here DrvLCD_TurnOff() also call ClosePanel again,
	//=> it will cause sif_close() is call twice by LCD driver internal,
	//=> then ALL sif_send() command of other devices will become abnormally!!!
	//=> 可能現象1: 造成CCD無法wakeup (TG會用到sif)
	//=> 可能現象2: 造成CCD拍出壞照片 (TG會用到sif)

	//LCD panel close
	pObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);

	// Turn off LCD power
	g_LCDEnable = FALSE;
}

static void DrvLCD_SleepEnter(void)
{
	// LCD backlight
	GPIOMap_TurnOffLCDBacklight();

	// LCD panel close
	pObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);

	GPIOMap_LCDReset();

	g_LCDSleep = TRUE;
}

static void DrvLCD_SleepLeave(void)
{
	// LCD panel open
	pDev->SEL.OPEN_DEVICE.DevID = DISPDEV_ID_PANEL;
	pObj->devCtrl(DISPDEV_OPEN_DEVICE, pDev);

	//////////////////////////////////////////////////////////////////////
	// special operation for this project!
	//
	//pDev->SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_180;
	//pObj->devCtrl(DISPDEV_SET_ROTATE,pDev);
	//////////////////////////////////////////////////////////////////////

	// LCD backlight
	GPIOMap_TurnOnLCDBacklight();

	g_LCDSleep = FALSE;
}


static UINT32 DrvLCDState(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_FUNC_BEGIN("\r\n");

	if (StateID & DXGET) {
		UINT32 rvalue =  0;
		StateID &= ~DXGET;
		DBG_IND("get %08x\r\n", StateID);
		switch (StateID) {
		case DRVDISP_STATE_ENBALE:
			//lost device? ---> cannot determine enable or not
			//under flow like: enable/disable
			rvalue =  g_LCDEnable;
			break;
		case DRVDISP_STATE_MODE:
			//lost device? ---> cannot determine mode
			//under flow like: change mode
			rvalue =  g_LCDMode;
			break;
		case DRVDISP_STATE_LASTMODE:
			rvalue =  g_LCDLastMode;
			break;
		case DRVDISP_STATE_SLEEP:
			rvalue =  g_LCDSleep;
			break;
		case DRVDISP_STATE_BACKLIGHT:
			rvalue =  GPIOMap_IsLCDBacklightOn() ? DRVDISP_BACKLIGHT_ON : DRVDISP_BACKLIGHT_OFF;
			break;
		case DRVDISP_STATE_BRIGHTLVL:
			rvalue =  g_LCDBacklightLvl;
			break;
		default:
			DBG_WRN("get state=0x%02X not support!\r\n", StateID);
			break;
		}
		return rvalue;
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		DBG_IND("set %08x\r\n", StateID);
		switch (StateID) {
		case DRVDISP_STATE_BACKLIGHT:
			if (Value == DRVDISP_BACKLIGHT_OFF) {
				DBG_IND("DRVDISP_BACKLIGHT_OFF\r\n");
				GPIOMap_TurnOffLCDBacklight();
			} else { //if(value == DRVDISP_BACKLIGHT_ON)
				DBG_IND("DRVDISP_BACKLIGHT_ON\r\n");
				GPIOMap_TurnOnLCDBacklight();
			}
			break;
		case DRVDISP_STATE_BRIGHTLVL: {
				DBG_IND("DRVDISP_BRIGHTLVL=%d\r\n", Value);
				GPIOMap_SetLCDBacklightBrightLevel((INT32)Value);
			}
			break;
		default:
			DBG_WRN("set state=0x%02X not support!\r\n", StateID);
			break;
		}
	}
	return DX_OK;
}

static UINT32 DrvLCDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
	DBG_FUNC_BEGIN("\r\n");
	DBG_IND("ctrl %08x\r\n", CtrlID);

	switch (CtrlID) {
	case DRVDISP_CTRL_MODE: {
			//DrvLCD_TurnOff();
			g_localMode = Param1;
			//DrvLCD_TurnOn();
			//g_bLCDWaitTurnOnFinish = TRUE;
		}
		break;
	case DRVDISP_CTRL_SLEEP:
		if (Param1 == TRUE) {
			DBG_IND("DRVDISP_CTRL_SLEEP 1\r\n");
			DrvLCD_SleepEnter();
		} else if (Param1 == FALSE) {
			DBG_IND("DRVDISP_CTRL_SLEEP 0\r\n");
			//g_localMode = g_LCDCtrlObj.uiLCDMode; //current mode
			pObj->devCtrl(DISPDEV_GET_LCDMODE, pDev);
			g_localMode = pDev->SEL.GET_LCDMODE.Mode; //current mode
			DrvLCD_SleepLeave();
		}
		break;
	case DRVDISP_CTRL_WAITFINISH:
		if (g_bLCDWaitTurnOnFinish == TRUE) {
			DrvLCD_TurnOn_WaitFinish();
			g_bLCDWaitTurnOnFinish = FALSE;
		}
		break;

	default:
		DBG_WRN("ctrl id=0x%02X not support!\r\n", CtrlID);
		break;
	}
	return DX_OK;
}

static UINT32 DrvLCDCommand(CHAR *pcCmdStr) //General Command Console
{
	switch (*pcCmdStr) {
	case 'd':
		if (!strncmp(pcCmdStr, "disp lcd dump", 13)) {
			DrvLCD_Dump();
			return TRUE;
		}
		break;
	}
	return FALSE;
}

static void DrvLCD_Dump(void)
{
	GPIOMap_DumpBacklight();
}


////////////////////////////////////////////////////////////////////////////////
// GPIO related
static void GPIOMap_LCDReset(void)
{
	// Reset LCD
#if defined(_Disp_IF8B_LCD1_ILI9341_)
	gpio_clearPin(GPIO_LCD_RESET);
	SwTimer_DelayMs(5);
	gpio_setPin(GPIO_LCD_RESET);
#elif defined(_Disp_IFDSI_LCD1_BPB5017BFW_ILI9806E_)
	gpio_clearPin(GPIO_LCD_RESET);
	SwTimer_DelayMs(10);
	gpio_setPin(GPIO_LCD_RESET);
#endif
}

/**
  Turn on LCD power

  Turn on LCD power.

  @param void
  @return void
*/
static void GPIOMap_TurnOnLCDPower(void)
{
}

/**
  Turn off LCD power

  Turn off LCD power.

  @param void
  @return void
*/
static void GPIOMap_TurnOffLCDPower(void)
{

}

/**
  Check whether LCD power is on or not

  Check whether LCD power is on or not.
  Return TRUE if LCD power is on, return FALSE if LCD power is off.

  @param void
  @return BOOL
*/
//static BOOL GPIOMap_IsLCDPowerOn(void)
//{
//    return TRUE;
//}

////////////////////////////////////////////////////////////////////////

//#NT#2009/12/15#Lincy Lin -begin
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
static PWM_CFG g_LCDBacklightPWMInfo = {1, 60, 0, 60, 0, 0};
static UINT32 g_LCDBacklightLvlPWMDuty[DRVLCD_BRT_LEVEL_MAX] = {
	59,//48,// level 0
	44,// level 1
	35,// level 2
	28,// level 3
	25,// level 4
	22,// level 5
	20,// level 6
	18,// level 7
	17,// level 8
	5, //10 // level 9
};
#endif


/**
  Turn on LCD backlight

  Turn on LCD backlight.

  @param void
  @return void
*/
static void GPIOMap_TurnOnLCDBacklight(void)
{
	DBG_IND("GPIOMap_TurnOnLCDBacklight: Adjust value %d\r\n", g_LCDBacklightLvl);

	if (g_LCDBacklightEn == FALSE) {
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
		g_LCDBacklightPWMInfo.uiRise = g_LCDBacklightLvlPWMDuty[g_LCDBacklightLvl];
		DBG_IND("PWM: rise=%d\r\n", g_LCDBacklightPWMInfo.uiRise);
		pwm_open(PWMID_LCD_BLG_PCTL);
		pwm_pwmConfig(PWMID_LCD_BLG_PCTL, &g_LCDBacklightPWMInfo);
		pwm_pwmEnable(PWMID_LCD_BLG_PCTL);
#elif (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
		gpio_setPin(GPIO_LCD_BLG_PCTL);
#endif
		g_LCDBacklightEn = TRUE;
	}
}

static void GPIOMap_SetLCDBacklightBrightLevel(INT32 uiLevel)
{
	g_LCDBacklightLvl = uiLevel;
	if (g_LCDBacklightLvl < 0) {
		DBG_WRN("DrvLCDSetCtrl Warning! brightlvl=0x%02X out of range\r\n", g_LCDBacklightLvl);
		g_LCDBacklightLvl = 0;
	}
	if (g_LCDBacklightLvl >= DRVLCD_BRT_LEVEL_MAX) {
		DBG_WRN("DrvLCDSetCtrl Warning! brightlvl=0x%02X out of range\r\n", g_LCDBacklightLvl);
		g_LCDBacklightLvl = DRVLCD_BRT_LEVEL_MAX - 1;
	}

	if (!g_LCDBacklightEn) {
		return;
	}

	GPIOMap_AdjustLCDBacklight(g_LCDBacklightLvl);
}

/**
  Turn off LCD backlight

  Turn off LCD backlight.

  @param void
  @return void
*/
static void GPIOMap_TurnOffLCDBacklight(void)
{
	DBG_IND("GPIOMap_TurnOffLCDBacklight: Adjust value %d\r\n", g_LCDBacklightLvl);

	if (g_LCDBacklightEn == TRUE) {
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
		pwm_pwmDisable(PWMID_LCD_BLG_PCTL);
		pwm_close(PWMID_BLG_PCTL, TRUE);
#elif (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
		gpio_clearPin(GPIO_LCD_BLG_PCTL);
#endif
		g_LCDBacklightEn = FALSE;
	}
}

/**
  Adjust LCD backlight level

  Adjust LCD backlight level.

  @param UINT32 uiAdjValue
  @return void
*/
static void GPIOMap_AdjustLCDBacklight(UINT32 uiAdjValue)
{
	DBG_IND("GPIOMap_AdjustLCDBacklight: Adjust value %d\r\n", uiAdjValue);

	/* Re-start PWM */
	if (g_LCDBacklightEn) {
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
		g_LCDBacklightPWMInfo.uiRise = g_LCDBacklightLvlPWMDuty[g_LCDBacklightLvl];
		DBG_IND("PWM: rise=%d\r\n", g_LCDBacklightPWMInfo.uiRise);
		pwm_pwmDisable(PWMID_LCD_BLG_PCTL);
		pwm_pwmConfig(PWMID_LCD_BLG_PCTL, &g_LCDBacklightPWMInfo);
		pwm_pwmEnable(PWMID_LCD_BLG_PCTL);
#endif
	}
}

/**
  Check whether LCD backlight is on or not

  Check whether LCD backlight is on or not.
  Return TRUE if LCD backlight is on, return FALSE if LCD backlight is off.

  @param void
  @return BOOL
*/
static BOOL GPIOMap_IsLCDBacklightOn(void)
{
	return g_LCDBacklightEn;
}

static void GPIOMap_DumpBacklight(void)
{
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
	DBG_DUMP("BACKLIGHT clk :\t\t%d MHz\r\n", 48 / (g_LCDBacklightPWMInfo.uiDiv + 1));
	DBG_DUMP("BACKLIGHT Duty :\t%d %\r\n", (g_LCDBacklightPWMInfo.uiFall - g_LCDBacklightPWMInfo.uiRise) * 100 / g_LCDBacklightPWMInfo.uiPrd);
#endif
}

