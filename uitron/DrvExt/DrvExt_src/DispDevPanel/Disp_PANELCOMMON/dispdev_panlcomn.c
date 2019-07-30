/*
    Panel device open/close Common control.

    @file       dispdev_panlcomn.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "kernel.h"
#include "dispdev_panlcomn.h"
#include "display.h"
#include "ide.h"
#include "pll.h"
#include "Utility.h"


/*
    Set Display open

    Open the specified display engine with the specified engine parameters.
*/
ER dispdev_setDisplay(DISPDEV_IOCTRL pDispDevControl, tLCD_INF LcdInf, tLCD_PARAM *pMode, PINMUX_FUNC_ID PinFuncID)
{
	UINT32                  pinmux_value;
	tIDE_PARAM             *pIDE    = &pMode->IDE;
	tPANEL_PARAM           *pPanel  = &pMode->Panel;
	//UINT32                  uiSrcClkFreq;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	BOOL                    bYCC8bit;

	DBG_IND("dispdev_setDisplay START 1 [0x%08x] 0x%08x\r\n", pIDE->pinmux_select_lcd, *(UINT32 *)0xF0010008);

	pinmux_setPinmux(PinFuncID, pIDE->pinmux_select_lcd);

	DBG_IND("dispdev_setDisplay START 2 [0x%08x] 0x%08x\r\n", pIDE->pinmux_select_lcd, *(UINT32 *)0xF0010008);

	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
	/*if( DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_MI_ENGINE)
	{
	    // This path is for MI engine DRAM direct path display object
	    DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = pPanel->uiBufferWidth;
	    DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = pPanel->uiBufferHeight;
	    DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = pPanel->uiWindowWidth;
	    DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = pPanel->uiWindowHeight;
	    pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);
	    return E_OK;
	}*/

	// Make sure IDE Disable
	pDispDevControl(DISPDEV_IOCTRL_GET_ENABLE, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_ENABLE.bEn == TRUE) {
		DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
		pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
	}
#if 0
	// Set IDE Clock Source as specified
	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL1) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_1) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_480);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, 480);
		if (pll_getPLLEn(PLL_ID_1) != TRUE) {
			pll_setPLLEn(PLL_ID_1, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL2) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_2) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, PLL_SRC_ID_PLL2);
		if (pll_getPLLEn(PLL_ID_2) != TRUE) {
			pll_setPLLEn(PLL_ID_2, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL4) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_4) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL4);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, PLL_SRC_ID_PLL4);
		if (pll_getPLLEn(PLL_ID_4) != TRUE) {
			pll_setPLLEn(PLL_ID_4, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL7) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_7) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL7);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, PLL_SRC_ID_PLL7);
		if (pll_getPLLEn(PLL_ID_7) != TRUE) {
			pll_setPLLEn(PLL_ID_7, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL1) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_1) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_480);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, 480);
		if (pll_getPLLEn(PLL_ID_1) != TRUE) {
			pll_setPLLEn(PLL_ID_1, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL2) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_2) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL2);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, PLL_SRC_ID_PLL2);
		if (pll_getPLLEn(PLL_ID_2) != TRUE) {
			pll_setPLLEn(PLL_ID_2, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL4) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_4) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL4);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, PLL_SRC_ID_PLL4);
		if (pll_getPLLEn(PLL_ID_4) != TRUE) {
			pll_setPLLEn(PLL_ID_4, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL7) {
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_7) / 1000000;
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL7);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, PLL_SRC_ID_PLL7);
		if (pll_getPLLEn(PLL_ID_7) != TRUE) {
			pll_setPLLEn(PLL_ID_7, TRUE);
		}
	} else {
		dispanl_debug(("dispdev_setDisplay IDE SrcClk Error\r\n"));
		return E_SYS;
	}

	// Set IDE PIXEL clock frequency
	if (pPanel->bYCbCrFormat) {
		if ((pIDE->pinmux_select_lcd == PINMUX_LCD_SEL_CCIR601_16BITS) || (pIDE->pinmux_select_lcd == PINMUX_LCD_SEL_CCIR656_16BITS)) {
			uiClockDiv = (UINT8)((float)uiSrcClkFreq / (float)pPanel->fDCLK / (float)2.0);
		} else {
			uiClockDiv = (UINT8)((float)uiSrcClkFreq / (float)pPanel->fDCLK);
		}

		DevIOCtrl.SEL.SET_ICST_EN.bEn = FALSE;
		DevIOCtrl.SEL.SET_ICST_EN.Select = CST_RGB2YCBCR;
		pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);
	} else {
		uiClockDiv = (UINT8)((float)uiSrcClkFreq / (float)pPanel->fDCLK / (float)2.0);

		DevIOCtrl.SEL.SET_ICST_EN.bEn = TRUE;
		DevIOCtrl.SEL.SET_ICST_EN.Select = CST_YCBCR2RGB;
		pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);
	}

	//pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv - 1));

	// Enable IDE Clock
	//pll_enableClock(IDE1_CLK);
	//pll_enableClock(IDE2_CLK);*/

	if (uiClockDiv > 0) {
		uiClockDiv--;
	}


	// Set IDE Clock Source as specified
	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
	switch (DevIOCtrl.SEL.GET_SRCCLK.SrcClk) {
	case DISPCTRL_SRCCLK_PLL1:
	case DISPCTRL_SRCCLK_PLL2:
	case DISPCTRL_SRCCLK_PLL4:
	case DISPCTRL_SRCCLK_PLL7:
		pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV, PLL_IDE_CLKDIV(uiClockDiv));
		pll_enableClock(IDE1_CLK);
		break;
	case DISPCTRL_SRCCLK_IDE2_PLL1:
	case DISPCTRL_SRCCLK_IDE2_PLL2:
	case DISPCTRL_SRCCLK_IDE2_PLL4:
	case DISPCTRL_SRCCLK_IDE2_PLL7:
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKDIV, PLL_IDE2_CLKDIV(uiClockDiv));
		pll_enableClock(IDE2_CLK);
		break;
	default:
		break;
	}
#endif

	if (LcdInf == tLCD_INF_SERIAL_8BITS) {
		switch (pPanel->LCDMode) {
		case PINMUX_LCDMODE_RGB_SERIAL:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_SERIAL;
			break;
		case PINMUX_LCDMODE_YUV640:
		case PINMUX_LCDMODE_YUV720:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_YUV;
			break;
		case PINMUX_LCDMODE_RGBD360:
		case PINMUX_LCDMODE_RGBD320:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGBD;
			break;
		case PINMUX_LCDMODE_RGB_THROUGH:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_THROUGH;
			break;
		case PINMUX_LCDMODE_CCIR601:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_CCIR601_8BIT;
			break;
		case PINMUX_LCDMODE_CCIR656:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_CCIR656_8BIT;
			break;
		default:
			DBG_ERR("Invalid LCDMode %d LcdInf %d\r\n", pPanel->LCDMode, LcdInf);
			return E_NOSPT;
		}
		pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);

	} else if (LcdInf == tLCD_INF_MI) {
		if ((pPanel->LCDMode >= PINMUX_LCDMODE_MI_FMT0) && (pPanel->LCDMode <= PINMUX_LCDMODE_MI_SERIAL_SEP)) {
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_MI;
			pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);
		} else {
			DBG_ERR("Invalid LCDMode %d LcdInf %d\r\n", pPanel->LCDMode, LcdInf);
		}
	} else if (LcdInf == tLCD_INF_MIPI_DSI) {
		DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_EMBD_MIPIDSI;
		pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);
	} else if (LcdInf == tLCD_INF_PARALLEL_16BITS) {
		switch (pPanel->LCDMode) {
		case PINMUX_LCDMODE_RGB_PARALL:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_PARALL;
			break;
		case PINMUX_LCDMODE_RGB_PARALL666:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_PARALL;
			break;
		case PINMUX_LCDMODE_RGB_PARALL565:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_PARALL;
			break;
		case PINMUX_LCDMODE_RGB_PARALL_DELTA:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGBDELTA_16BIT;
			break;
		case PINMUX_LCDMODE_CCIR601:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_CCIR601_16BIT;
			break;
		case PINMUX_LCDMODE_CCIR656:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_CCIR656_16BIT;
			break;
		case PINMUX_LCDMODE_RGB_THROUGH:
			DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_THROUGH;
			break;
		default:
			DBG_ERR("Invalid LCDMode %d LcdInf %d\r\n", pPanel->LCDMode, LcdInf);
			return E_NOSPT;
		}
		pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);

	} else {
		DBG_ERR("LcdInf Has not implemented!(%d)\r\n", LcdInf);
		return E_NOSPT;
	}

	DBG_IND("YCbCrFormat %d pinmux_select_lcd = 0x%08x\r\n", pPanel->bYCbCrFormat, pIDE->pinmux_select_lcd);

	if (pPanel->bYCbCrFormat) {
		pinmux_value = pIDE->pinmux_select_lcd & ~(PINMUX_LCD_SEL_FEATURE_MSK);
		if ((pinmux_value == PINMUX_LCD_SEL_CCIR601_16BITS) || (pinmux_value == PINMUX_LCD_SEL_CCIR656_16BITS)) {
			bYCC8bit = FALSE;
		} else {
			bYCC8bit = TRUE;
		}
		// set ICST
		DevIOCtrl.SEL.SET_ICST_EN.bEn = FALSE;
		DevIOCtrl.SEL.SET_ICST_EN.Select = CST_RGB2YCBCR;
		pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);
	} else {
		bYCC8bit = FALSE;
		// set ICST
		DevIOCtrl.SEL.SET_ICST_EN.bEn = TRUE;
		DevIOCtrl.SEL.SET_ICST_EN.Select = CST_YCBCR2RGB;
		pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);
	}

	// set clock rate
	DBG_IND("set clock rate = %f Hz\r\n", pPanel->fDCLK * 1000000);
	DevIOCtrl.SEL.SET_CLK_FREQ.uiFreq = pPanel->fDCLK * 1000000;
	DevIOCtrl.SEL.SET_CLK_FREQ.bYCC8bit = bYCC8bit;
	DBG_IND("DevIOCtrl.SEL.SET_CLK_FREQ.bYCC8bit = %d\r\n", DevIOCtrl.SEL.SET_CLK_FREQ.bYCC8bit);
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_FREQ, &DevIOCtrl);

	// enable clock
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = TRUE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);


	DevIOCtrl.SEL.SET_SRGB_OUTORDER.PixOrder     = pIDE->pdir;
	DevIOCtrl.SEL.SET_SRGB_OUTORDER.OddStart     = pIDE->odd;
	DevIOCtrl.SEL.SET_SRGB_OUTORDER.EvenStart    = pIDE->even;
	pDispDevControl(DISPDEV_IOCTRL_SET_SRGB_OUTORDER, &DevIOCtrl);

	DevIOCtrl.SEL.SET_DITHER_EN.bEn      = pIDE->dithering[0];
	DevIOCtrl.SEL.SET_DITHER_EN.bFreeRun = pIDE->dithering[1];
	DevIOCtrl.SEL.SET_DITHER_EN.Rbits    = pIDE->DitherBits[0];
	DevIOCtrl.SEL.SET_DITHER_EN.Gbits    = pIDE->DitherBits[1];
	DevIOCtrl.SEL.SET_DITHER_EN.Bbits    = pIDE->DitherBits[2];
	pDispDevControl(DISPDEV_IOCTRL_SET_DITHER_EN, &DevIOCtrl);


	DevIOCtrl.SEL.SET_SYNC_INVERT.bHsInv  = pIDE->hsinv;
	DevIOCtrl.SEL.SET_SYNC_INVERT.bVsInv  = pIDE->vsinv;
	DevIOCtrl.SEL.SET_SYNC_INVERT.bClkInv = pIDE->clkinv;
	pDispDevControl(DISPDEV_IOCTRL_SET_SYNC_INVERT, &DevIOCtrl);


	DevIOCtrl.SEL.SET_VLD_INVERT.bHvldInv    = pIDE->hvldinv;
	DevIOCtrl.SEL.SET_VLD_INVERT.bVvldInv    = pIDE->vvldinv;
	DevIOCtrl.SEL.SET_VLD_INVERT.bFieldInv   = pIDE->fieldinv;
	DevIOCtrl.SEL.SET_VLD_INVERT.bDeInv      = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_VLD_INVERT, &DevIOCtrl);


	DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = pPanel->uiHSyncSYNCwidth;
	DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = pPanel->uiHSyncTotalPeriod - 1;
	DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = pPanel->uiHSyncBackPorch;
	DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = pPanel->uiHSyncActivePeriod + pPanel->uiHSyncBackPorch - 1;
	pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);


	if ((pIDE->pinmux_select_lcd == PINMUX_LCD_SEL_RGB_16BITS)) {
		DevIOCtrl.SEL.SET_CLK1_2.bClk1_2         = pIDE->clk1_2;
		pDispDevControl(DISPDEV_IOCTRL_SET_CLK1_2, &DevIOCtrl);
	} else {
		DevIOCtrl.SEL.SET_CLK1_2.bClk1_2         = FALSE;
		pDispDevControl(DISPDEV_IOCTRL_SET_CLK1_2, &DevIOCtrl);
	}



	DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = pPanel->uiVSyncSYNCwidth;
	DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = pPanel->uiVSyncTotalPeriod - 1;
	DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = pPanel->uiVSyncBackPorchOdd;
	DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = pPanel->uiVSyncActivePeriod + pPanel->uiVSyncBackPorchOdd - 1;
	DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart = pPanel->uiVSyncBackPorchEven;
	DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = pPanel->uiVSyncActivePeriod + pPanel->uiVSyncBackPorchEven - 1;
	pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);


	DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bInterlaced = pIDE->interlace;
	DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bFieldOddSt = TRUE;
	pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE, &DevIOCtrl);


	DevIOCtrl.SEL.SET_CSB_EN.bEn             = TRUE;
	DevIOCtrl.SEL.SET_CSB_EN.uiContrast      = pIDE->ctrst;
	DevIOCtrl.SEL.SET_CSB_EN.uiSaturation    = pIDE->cmults;
	DevIOCtrl.SEL.SET_CSB_EN.uiBrightness    = pIDE->brt;
	pDispDevControl(DISPDEV_IOCTRL_SET_CSB_EN, &DevIOCtrl);


	DevIOCtrl.SEL.SET_YC_EXCHG.bCbCrExchg    = pIDE->cex;
	DevIOCtrl.SEL.SET_YC_EXCHG.bYCExchg      = pIDE->YCex;
	pDispDevControl(DISPDEV_IOCTRL_SET_YC_EXCHG, &DevIOCtrl);

	DevIOCtrl.SEL.SET_CLAMP.uiClamp          = pIDE->clamp;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLAMP, &DevIOCtrl);

	ide_setTvPowerDown(pIDE->tv_powerdown);

	DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = pPanel->uiBufferWidth;
	DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = pPanel->uiBufferHeight;
	DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = pPanel->uiWindowWidth;
	DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = pPanel->uiWindowHeight;
	pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);

	DevIOCtrl.SEL.SET_ENABLE.bEn = TRUE;
	pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
	dispanl_debug(("dispdev_setDisplay Config Done\r\n"));

	//#NT#2012/03/22#Steven Wang -begin
	//#NT#Temp add for DSI
	if (LcdInf != tLCD_INF_MIPI_DSI) {
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
	}
	//#NT#2012/03/22#Temp add for DSI
	dispanl_debug(("dispdev_setDisplay IDE GO\r\n"));
	return E_OK;
}

#if 0
/*
    Display close

    Close the specified display engine.
*/
ER dispdev_closeDisplay(DISPDEV_IOCTRL pDispDevControl)
{
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	dispanl_debug(("DispDevClose\r\n"));

	DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
	pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

	/*
	// Disable IDE
	if(pDevIF->DispID == DISP_1)
	{
	    pDevIF->waitFrmEnd();
	    pll_disableClock(IDE1_CLK);
	    pll_disableClock(IDE2_CLK);
	}
	else if (pDevIF->DispID == DISP_2)
	{
	    Delay_DelayMs(40);
	}
	*/

	return E_OK;
}
#endif

