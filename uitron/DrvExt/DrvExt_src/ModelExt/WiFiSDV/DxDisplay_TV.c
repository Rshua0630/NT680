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

#include "Debug.h"

//driver
#include "display.h"
#include "stdio.h" //for sscanf


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

/*
	DISP_TVMODE_NTSC                = 0,          ///< TV MODE is NTSC (Video format is 640x448 i60 or p30)
	DISP_TVMODE_PAL                 = 1,          ///< TV MODE is PAL (Video format is 640x544 i50 or p25)
	DISP_TVMODE_NTSC_SD             = 2,          ///< TV MODE is SD NTSC (Video format is 720x480 i60 or p30)
	DISP_TVMODE_PAL_SD              = 3,          ///< TV MODE is SD PAL (Video format is 720x526 i50 or p25)
*/
//TV: NTSC = 640x448, PAL = 640x528
//#define TV_MAX_W                        640  //device w of DISP_TVMODE_xxx
//#define TV_MAX_H                        528  //device h of DISP_TVMODE_xxx
//SDTV: NTSC = 720x480, PAL = 720x576
#define TV_MAX_W                        720  //device w of DISP_TVMODE_xxx
#define TV_MAX_H                        576  //device h of DISP_TVMODE_xxx

static DISPDEV_PARAM   DispDev;
static DISPCTRL_PARAM  DispCtrl;
static PDISP_OBJ       pObj = 0;
static PDISPDEV_PARAM  pDev = &DispDev;
static PDISPCTRL_PARAM pCtrl = &DispCtrl;

static UINT32          g_TVEnable = FALSE; //not yet enable
// Exported Variables
static UINT32          g_TVDout = 0; //current dout
static UINT32          g_TVMode = 0; //current mode
static UINT32          g_TVLastMode = 0; //last mode

// Local Variables
static UINT32          g_TVSleep = FALSE; //not yet sleep
static UINT32          g_TVBacklightEn = FALSE;

// TV control object
//TV_DEVICE_OBJ   g_TVCtrlObj;

static BOOL g_bTVWaitTurnOnFinish = FALSE;

static UINT32 g_localMode = 0; //new mode
static USIZE  g_localSize = {0, 0}; //new size
static USIZE *pCurrentSize = 0; //return size

#if _FPGA_EMULATION_
extern void tc680_init(void);
extern ER   tc680_writeReg(UINT32 uiOffset, UINT32 uiValue);
extern ER   tc680_readReg(UINT32 uiOffset, UINT32 *puiValue);
extern void tc680_dump_all(void);
#endif

//public func
static UINT32 DrvTVGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
static UINT32 DrvTVSetcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
static UINT32 DrvTVInit(void *pInitParam); // Set Init Parameters
static UINT32 DrvTVOpen(void); // Common Constructor
static UINT32 DrvTVClose(void); // Common Destructor
static UINT32 DrvTVState(UINT32 StateID, UINT32 Value); // General Properties
static UINT32 DrvTVControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
static UINT32 DrvTVCommand(CHAR *pcCmdStr); //General Command Console

//internal func
static void DrvTV_TurnOn(void);
static void DrvTV_TurnOn_WaitFinish(void);
static void DrvTV_TurnOff(void);
static void DrvTV_SleepEnter(void);
static void DrvTV_SleepLeave(void);
static BOOL GPIOMap_DetTV(void);
static void GPIOMap_TurnOnTVBacklight(void);
static void GPIOMap_TurnOffTVBacklight(void);

//dx object
DX_OBJECT gDevTV = {
	DXFLAG_SIGN,
	DX_CLASS_DISPLAY_EXT | DX_TYPE_TVOUT,
	DISPLAY_VER,
	"Display_TV",
	0, 0, 0, 0,
	DrvTVGetcaps,
	DrvTVSetcfgs,
	DrvTVInit,
	DrvTVOpen,
	DrvTVClose,
	DrvTVState,
	DrvTVControl,
	DrvTVCommand,
	0,
};

static void _DrvTVHookDout(UINT32 id)
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
#if defined(_TV_ON_)
	{
		//static BOOL bTVHook[2] = {FALSE,FALSE};
		//if(!bTVHook[id])
		{
			pDev->SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_TVNTSC;
			pDev->SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getTvDevObj();
			pObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT, pDev);
			pDev->SEL.HOOK_DEVICE_OBJECT.DevID         = DISPDEV_ID_TVPAL;
			pDev->SEL.HOOK_DEVICE_OBJECT.pDispDevObj   = dispdev_getTvDevObj();
			pObj->devCtrl(DISPDEV_HOOK_DEVICE_OBJECT, pDev);
			//bTVHook[id] = TRUE;
		}
	}
#endif
}

static UINT32 DrvTVGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID & 0x0000ffff) {
	case DISPLAY_CAPS_BASE:
		//DBG_IND("get TV plug caps\r\n");
		v = DISPLAY_BF_BACKLIGHT | DISPLAY_BF_DETPLUG;
		break;
	case DISPLAY_CAPS_PLUG:
		//DBG_IND("det TV plug status\r\n");
		v = GPIOMap_DetTV();
		break;
	case DISPLAY_CAPS_DEFMODE:
		v = DISP_TVMODE_NTSC;
		break;
	case DISPLAY_CAPS_MODEINFO: {
			int mode = (CapID & 0xffff0000) >> 16;
			ISIZE *pModeInfo = (ISIZE *)Param1;
			DISPDEV_ID dev;
			_DrvTVHookDout(g_TVDout);
			if (mode == DISP_TVMODE_NTSC) {
				dev = DISPDEV_ID_TVNTSC; //NTSC: 640x448
			} else if (mode == DISP_TVMODE_PAL) {
				dev = DISPDEV_ID_TVPAL; //PAL: 640x528
			} else if (mode == DISP_TVMODE_NTSC_SD) {
				dev = DISPDEV_ID_TVNTSC;
			} else if (mode == DISP_TVMODE_PAL_SD) {
				dev = DISPDEV_ID_TVPAL;
			} else {
				DBG_ERR("mode=%d not support!\r\n", mode);
				break;
			}
			pDev->SEL.GET_PREDISPSIZE.DevID = dev;
			pObj->devCtrl(DISPDEV_GET_PREDISPSIZE, pDev);
			if (pModeInfo) {
				pModeInfo->w = pDev->SEL.GET_PREDISPSIZE.uiBufWidth; //640
				pModeInfo->h = pDev->SEL.GET_PREDISPSIZE.uiBufHeight; //448
			}
		}
		break;
	case DISPLAY_CAPS_MAXSIZE: {
			ISIZE *pSizeInfo = (ISIZE *)Param1;
			if (pSizeInfo) {
				pSizeInfo->w = TV_MAX_W;
				pSizeInfo->h = TV_MAX_H;
			}
		}
		break;
	case DISPLAY_CAPS_ASPECT: {
			ISIZE *pAspectInfo = (ISIZE *)Param1;
			if (pAspectInfo) {
				pAspectInfo->w = 4;
				pAspectInfo->h = 3;
			}
		}
		break;
	default:
		break;
	}
	return v;
}

static UINT32 DrvTVSetcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	switch (CfgID) {
	case DISPLAY_CFG_MODE:
		g_TVLastMode = Param1;
		break;
	case DISPLAY_CFG_DOUT:
		g_TVDout = Param1;
		break;
	case DISPLAY_CFG_INFOBUF:
		pCurrentSize = (USIZE *)Param1;
		break;
	default:
		break;
	}
	return DX_OK;
}

static UINT32 DrvTVInit(void *pInitParam) // Set Init Parameters
{
	DBG_FUNC_BEGIN("\r\n");

	g_TVLastMode = DrvTVGetcaps(DISPLAY_CAPS_DEFMODE, 0);

	return DX_OK;
}

static UINT32 DrvTVOpen(void) // Common Constructor
{
	DBG_FUNC_BEGIN("\r\n");

	_DrvTVHookDout(g_TVDout);
	DBG_IND("TV dout by IDE%d\r\n", g_TVDout + 1);
	////////////////////////////////////////////////////
#if (_FPGA_EMULATION_ == ENABLE)
	{
		DBG_WRN("Please make sure {PIN_FUNC_PWM, PIN_PWM_CFG_NONE}! and pick FPGA OSC to 27Mhz!\r\n");
		//emu_TC96650_DefaultConfig();
		//emu_TC96650_DumpRegister();
		DBG_DUMP("TC680 Init\r\n");
		tc680_init();
		tc680_writeReg(0x0000, 0x00000007);
		//tc680_dump_all();
	}
#else
	// TV need PLL2 for precisely CLK
	if (g_TVDout == 0) {
		pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_PLL2;
		pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
	}
	if (g_TVDout == 1) {
		pCtrl->SEL.SET_SRCCLK.SrcClk = DISPCTRL_SRCCLK_IDE2_PLL2;
		pObj->dispCtrl(DISPCTRL_SET_SRCCLK, pCtrl);
	}
#endif
	////////////////////////////////////////////////////
	pDev->SEL.SET_TVADJUST.TvAdjust = DISPDEV_TVADJUST_3_3_K;
	pObj->devCtrl(DISPDEV_SET_TVADJUST, pDev);
	////////////////////////////////////////////////////

	g_localMode = g_TVLastMode;
	DBG_IND("mode %08x\r\n", g_localMode);
	DrvTV_TurnOn();

	g_TVEnable = TRUE;

	return DX_OK;
}

static UINT32 DrvTVClose(void) // Common Destructor
{
	DBG_FUNC_BEGIN("\r\n");

	DrvTV_TurnOff();

	g_TVLastMode = g_TVMode;
	g_TVMode = 0;
	g_TVEnable = FALSE;

	return DX_OK;
}

static void DrvTV_TurnOn(void)
{
	BOOL bFull;
	DISPDEV_ID dev;
	DBG_FUNC_BEGIN("\r\n");
	if (g_localMode == DISP_TVMODE_NTSC) {
		bFull = FALSE;
		dev = DISPDEV_ID_TVNTSC;
	} else if (g_localMode == DISP_TVMODE_PAL) {
		bFull = FALSE;
		dev = DISPDEV_ID_TVPAL;
	} else if (g_localMode == DISP_TVMODE_NTSC_SD) {
		bFull = TRUE;
		dev = DISPDEV_ID_TVNTSC;
	} else if (g_localMode == DISP_TVMODE_PAL_SD) {
		bFull = TRUE;
		dev = DISPDEV_ID_TVPAL; //PAL: 640x528
	} else {
		DBG_ERR("mode=%d not support!\r\n", g_localMode);
		return;
	}

	// Init TV object
	/*
	pDev->SEL.GET_PREDISPSIZE.DevID = dev;
	pObj->devCtrl(DISPDEV_GET_PREDISPSIZE,pDev);
	g_localSize.w     = pDev->SEL.GET_PREDISPSIZE.uiBufWidth;
	g_localSize.h    = pDev->SEL.GET_PREDISPSIZE.uiBufHeight;
	*/
	//IMPORTANT! for FAST BOOT, Photo App will ask disp size very early~
	//Update size of first mode (before open)
	//DxDisplay_UpdateModeInfo(g_TVDout<<4, g_localMode, &g_localSize);

	pDev->SEL.SET_TVFULL.bEnFull   = bFull;
	pObj->devCtrl(DISPDEV_SET_TVFULL, pDev);
	pDev->SEL.OPEN_DEVICE.DevID = dev;
	pObj->devCtrl(DISPDEV_OPEN_DEVICE, pDev);
	g_TVBacklightEn = TRUE;

	pObj->devCtrl(DISPDEV_GET_DISPSIZE, pDev);
	g_localSize.w = pDev->SEL.GET_DISPSIZE.uiBufWidth;
	g_localSize.h = pDev->SEL.GET_DISPSIZE.uiBufHeight;
	if (pCurrentSize) {
		pCurrentSize[0] = g_localSize;
	}

	// Enable IDE
	pObj->waitFrmEnd();
	pCtrl->SEL.SET_ENABLE.bEn = TRUE;
	pObj->dispCtrl(DISPCTRL_SET_ENABLE, pCtrl);


#if 0
	{
		int i;
		// Delay to wait for IDE stable
		for (i = 6; i > 0; i--) {
			//ide_wait_frame_end();
			pObj->waitFrmEnd();
		}
	}
#endif

	g_TVMode = g_localMode;
}

static void DrvTV_TurnOn_WaitFinish(void)
{
	DBG_FUNC_BEGIN("\r\n");

	g_TVEnable = TRUE;
}

static void DrvTV_TurnOff(void)
{
	DBG_FUNC_BEGIN("\r\n");

#if 1
//#NT#2010/02/25#Corey Ke -begin
//#NT#Remove warning message
//    int i;
//#NT#2010/02/25#Corey Ke -end
	// Close TV
	pObj->devCtrl(DISPDEV_CLOSE_DEVICE, NULL);
	g_TVBacklightEn = FALSE;
	/*
	    // Disable TV DAC
	    //ide_set_tv_powerdown(0x01);

	    // Delay to wait for TV DAC stable
	    for (i=16; i>0; i--)
	    {
	        //ide_wait_frame_end();
	        pObj->waitFrmEnd();
	    }
	*/

	// Disable IDE
	//ide_set_en(FALSE);
	//ide_wait_frame_end();
	//ide_set_en(FALSE);
	pCtrl->SEL.SET_ENABLE.bEn = FALSE;
	pObj->dispCtrl(DISPCTRL_SET_ENABLE, pCtrl);
	pObj->waitFrmEnd();

	// wait for Power Stable
	//delay(10000);
#endif
	g_TVEnable = FALSE;
}

void DrvTV_SleepEnter(void)
{
	GPIOMap_TurnOffTVBacklight();

	// Disable IDE
	//ide_set_en(FALSE);
	pObj->waitFrmEnd();
	pCtrl->SEL.SET_ENABLE.bEn = FALSE;
	pObj->dispCtrl(DISPCTRL_SET_ENABLE, pCtrl);

	g_TVSleep = TRUE;
}

static void DrvTV_SleepLeave(void)
{
	// Enable IDE
	//ide_set_en(TRUE);
	pObj->waitFrmEnd();
	pCtrl->SEL.SET_ENABLE.bEn = TRUE;
	pObj->dispCtrl(DISPCTRL_SET_ENABLE, pCtrl);

	GPIOMap_TurnOnTVBacklight();

	g_TVSleep = FALSE;
}

static UINT32 DrvTVState(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_FUNC_BEGIN("\r\n");

	if (StateID & DXGET) {
		UINT32 rvalue =  0;
		StateID &= ~DXGET;
		switch (StateID) {
		case DRVDISP_STATE_ENBALE:
			//lost device? ---> cannot determine enable or not
			//under flow like: enable/disable
			rvalue = g_TVEnable;
			break;
		case DRVDISP_STATE_MODE:
			//lost device? ---> cannot determine mode
			//under flow like: change mode
			rvalue = g_TVMode;
			break;
		case DRVDISP_STATE_LASTMODE:
			rvalue =  g_TVLastMode;
			break;
		case DRVDISP_STATE_BACKLIGHT:
			rvalue = g_TVBacklightEn;
			break;
		case DRVDISP_STATE_SLEEP:
			rvalue = g_TVSleep;
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
				GPIOMap_TurnOffTVBacklight();
			} else { //if(value == DRVDISP_BACKLIGHT_ON)
				DBG_IND("DRVDISP_BACKLIGHT_ON\r\n");
				GPIOMap_TurnOnTVBacklight();
			}
			break;
		case DRVDISP_STATE_SLEEP:
			if (Value == TRUE) {
				DBG_IND("DRVDISP_CTRL_SLEEP 1\r\n");
				DrvTV_SleepEnter();
			} else if (Value == FALSE) {
				DBG_IND("DRVDISP_CTRL_SLEEP 0\r\n");
				DrvTV_SleepLeave();
			}
			break;
		default:
			DBG_WRN("set state=0x%02X not support!\r\n", StateID);
			break;
		}
	}
	return DX_OK;
}

static UINT32 DrvTVControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
	DBG_FUNC_BEGIN("\r\n");

	switch (CtrlID) {
	case DRVDISP_CTRL_MODE: {
			//DrvTV_TurnOff();
			g_localMode = Param1;
			//DrvTV_TurnOn();
			//g_bTVWaitTurnOnFinish = TRUE;
		}
		break;
	case DRVDISP_CTRL_WAITFINISH:
		if (g_bTVWaitTurnOnFinish == TRUE) {
			DrvTV_TurnOn_WaitFinish();
			g_bTVWaitTurnOnFinish = FALSE;
		}
		break;

	default:
		DBG_WRN("ctrl id=0x%02X not support!\r\n", CtrlID);
		break;
	}
	return DX_OK;
}

static UINT32 DrvTVCommand(CHAR *pcCmdStr) //General Command Console
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
			vio_setpin(VIO_TV_DET, v1);
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}


/**
  Turn on TV backlight

  Turn on TV backlight.

  @param void
  @return void
*/
static void GPIOMap_TurnOnTVBacklight(void)
{
	DBG_IND("GPIOMap_TurnOnTVBacklight\r\n");

	if (g_TVBacklightEn == FALSE) {
		// Enable TV DAC
		pDev->SEL.SET_POWERDOWN.bPowerDown = 0x00; //FALSE = power on
		pObj->devCtrl(DISPDEV_SET_POWERDOWN, pDev);
		g_TVBacklightEn = TRUE;
	}
}

/**
  Turn off TV backlight

  Turn off TV backlight.

  @param void
  @return void
*/
static void GPIOMap_TurnOffTVBacklight(void)
{
	DBG_IND("GPIOMap_TurnOffTVBacklight\r\n");

	if (g_TVBacklightEn == TRUE) {
		// Disable TV DAC
		pDev->SEL.SET_POWERDOWN.bPowerDown = 0x01; //TRUE = power off.
		pObj->devCtrl(DISPDEV_SET_POWERDOWN, pDev);
		g_TVBacklightEn = FALSE;
	}
}



#if (TV_DET_CTRL == TV_DET_BY_ADC)
static UINT32 VolDet_GetTVDetADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
	UINT32 uiADCValue;

	uiADCValue = adc_readData(ADC_CH_TV_PLUG);
	// One-Shot Mode, trigger one-shot
	adc_triggerOneShot(ADC_CH_TV_PLUG);

	return uiADCValue;
#else
	return adc_readData(ADC_CH_TV_PLUG);
#endif
}
#endif

/**
  Detect TV is plugging in or unplugged

  Detect TV is plugging in or unplugged.
  Return TRUE if TV is plugging in, FALSE if TV is unplugged.

  @param void
  @return BOOL: TRUE -> TV is plugging in, FALSE -> TV is unplugged
*/

static BOOL GPIOMap_DetTV(void)
{
    return FALSE;
#if (TV_DET_CTRL == TV_DET_DUMMY)
	return FALSE;
#endif
#if ((TV_DET_CTRL == TV_DET_BY_VIO) && (USE_VIO == ENABLE))
	return (vio_getpin(VIO_TV_DET) != 0);
#endif
#if (TV_DET_CTRL == TV_DET_BY_GPIO)
	return (gpio_getPin(GPIO_TV_PLUG) == 0 ? FALSE : TRUE);
#endif
#if (TV_DET_CTRL == TV_DET_BY_ADC)
	UINT32          uiTVDetADC;
	uiTVDetADC = VolDet_GetTVDetADC();
	DBG_IND("uiTVDetADC=%d\r\n", uiTVDetADC);
	return (uiTVDetADC < 500 ? TRUE : FALSE);
#endif
}

//@}
