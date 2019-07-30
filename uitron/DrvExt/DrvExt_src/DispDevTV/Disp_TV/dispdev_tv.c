/*
    Display TV device object

    @file       dispdev_gsinf8bits.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "kernel.h"
#include "ide.h"
#include "pll.h"
#include "top.h"
#include "display.h"
#include "dispdev_ioctrl.h"
#include "dispdevctrl.h"

// Default debug level
#ifndef __DBGLVL__
#define __DBGLVL__  6       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
#define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"

static DISPDEV_IOCTRL pTvIoControl = NULL;

static ER dispdev_openTV(void);
static ER dispdev_closeTV(void);
static void dispdev_setTvIOCTRL(FP pIoCtrlFunc);
static ER dispdev_getTvSize(PDISPDEV_GET_PRESIZE tSize);
static BOOL dispdev_chgTvClkFreq(void);

static DISPDEV_OBJ DispDev_TvObj = { dispdev_openTV, dispdev_closeTV, NULL, NULL, NULL, dispdev_getTvSize, dispdev_setTvIOCTRL, {NULL, NULL}};

static UINT8 guiTvFilterTap[8] = {8, 8, 8, 8, 8, 8, 8, 8}; //0.74MHz
//static UINT8 guiTvFilterTap[8] = {0x1, 0x3, 0x5, 0x7, 0xA, 0xB, 0xD, 0xE}; //1.0MHz

static BOOL  bTvDispDevFreqChange = FALSE;
#if !_FPGA_EMULATION_
static UINT32 guiTvOrgFreq = 0;
#endif

#define DISPTV_DEBUG  DISABLE
#if DISPTV_DEBUG
#define disptv_debug(msg)       DBG_IND msg
#else
#define disptv_debug(msg)
#endif


#define TV_NTSC_Buf_width                       640
#define TV_NTSC_Buf_height                      448
#define TV_NTSC_Win_width                       640
#define TV_NTSC_Win_height                      448

#define TV_PAL_Buf_width                        640
#define TV_PAL_Buf_height                       528
#define TV_PAL_Win_width                        640
#define TV_PAL_Win_height                       528

#define TV_NTSC_Buf_width_full                  720
#define TV_NTSC_Buf_height_full                 480
#define TV_NTSC_Win_width_full                  720
#define TV_NTSC_Win_height_full                 480

#define TV_PAL_Buf_width_full                   720
//#define TV_PAL_Buf_width_full                   700
#define TV_PAL_Buf_height_full                  572
#define TV_PAL_Win_width_full                   720
//#define TV_PAL_Win_width_full                   700
#define TV_PAL_Win_height_full                  572

//Different FSADJ need different configuration
//HW FSADJ 3.6 K
#define TV_NTSC_BURST_LEVEL_3_6                 0x38
#define TV_NTSC_BLAND_LEVEL_3_6                 0x67
#define TV_NTSC_Y_SCALE_3_6                     0x78
#define TV_NTSC_CB_SCALE_3_6                    0x87
#define TV_NTSC_CR_SCALE_3_6                    0xBC

#define TV_PAL_BURST_LEVEL_3_6                  0x47
#define TV_PAL_BLAND_LEVEL_3_6                  0x6F
#define TV_PAL_Y_SCALE_3_6                      0x7D
#define TV_PAL_CB_SCALE_3_6                     0xA8
#define TV_PAL_CR_SCALE_3_6                     0xEC

//HW FSADJ 3.3 K
#define TV_NTSC_BURST_LEVEL_3_3                 0x38
#define TV_NTSC_BLAND_LEVEL_3_3                 0x74
#define TV_NTSC_Y_SCALE_3_3                     0x84//0x88
#define TV_NTSC_CB_SCALE_3_3                    0x76//0x77
#define TV_NTSC_CR_SCALE_3_3                    0xA8//0xAA

#define TV_PAL_BURST_LEVEL_3_3                  0x2e
#define TV_PAL_BLAND_LEVEL_3_3                  0x7a//0x7D
#define TV_PAL_Y_SCALE_3_3                      0x8e//0x92
#define TV_PAL_CB_SCALE_3_3                     0x80//0x7F
#define TV_PAL_CR_SCALE_3_3                     0xB5//0xB2

//HW FSADJ 3.0 K
#define TV_NTSC_BURST_LEVEL_3_0                 0x33
#define TV_NTSC_BLAND_LEVEL_3_0                 0x60
#define TV_NTSC_Y_SCALE_3_0                     0x76
#define TV_NTSC_CB_SCALE_3_0                    0x72
#define TV_NTSC_CR_SCALE_3_0                    0x9A

#define TV_PAL_BURST_LEVEL_3_0                  0x26
#define TV_PAL_BLAND_LEVEL_3_0                  0x65
#define TV_PAL_Y_SCALE_3_0                      0x75
#define TV_PAL_CB_SCALE_3_0                     0x76
#define TV_PAL_CR_SCALE_3_0                     0xA4

#define TV_PLL270     0x2D0000 //(0x2D0000*12)/131072 = 270MHz

#if 1
/*
    Display Device API of open TV Encoder

    Display Device API of open TV Encoder

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_OK:     TV Encoder Open done and success.
*/
static ER dispdev_openTV(void)
{
	UINT32                  uiBurstLevel = 0;
	UINT32                  uiBlandLevel = 0;
	UINT32                  uiYScale = 0, uiCbScale = 0, uiCrScale = 0;
	//UINT32                  uiClockDiv;
	//UINT32                  uiSrcClkFreq;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pTvIoControl;
	BOOL                    fullscreen;

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	disptv_debug(("DEVTV: open\r\n"));

	if (bTvDispDevFreqChange == FALSE) {
		if (dispdev_chgTvClkFreq() == TRUE) {
			bTvDispDevFreqChange = TRUE;
		}
	}

	if (tv_isOpened()) {
		tv_close();
	}

	tv_open();

	pDispDevControl(DISPDEV_IOCTRL_GET_ENABLE, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_ENABLE.bEn == TRUE) {
		DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
		pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
	}

#if 0
	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL1) {
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_480);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, 480);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_1) / 1000000;
		//uiSrcClkFreq = pll_getPLL1Freq()/1000000;
		//pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
		if (pll_getPLLEn(PLL_ID_1) != TRUE) {
			pll_setPLLEn(PLL_ID_1, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL2) {
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, PLL_SRC_ID_PLL2);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_2) / 1000000;
		//uiSrcClkFreq = pll_getPLL2Freq()/1000000;
		//pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
		if (pll_getPLLEn(PLL_ID_2) != TRUE) {
			pll_setPLLEn(PLL_ID_2, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL4) {
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL4);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, PLL_SRC_ID_PLL4);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_4) / 1000000;
		if (pll_getPLLEn(PLL_ID_4) != TRUE) {
			pll_setPLLEn(PLL_ID_4, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_PLL7) {
		pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL7);
//        pll_setConfig(PLL_CONFIG_ID_IDE_CLK_SRC, PLL_SRC_ID_PLL7);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_7) / 1000000;
		if (pll_getPLLEn(PLL_ID_7) != TRUE) {
			pll_setPLLEn(PLL_ID_7, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL1) {
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_480);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, 480);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_1) / 1000000;
		//uiSrcClkFreq = pll_getPLL1Freq()/1000000;
		//pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL1);
		if (pll_getPLLEn(PLL_ID_1) != TRUE) {
			pll_setPLLEn(PLL_ID_1, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL2) {
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL2);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, PLL_SRC_ID_PLL2);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_2) / 1000000;
		//uiSrcClkFreq = pll_getPLL2Freq()/1000000;
		//pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC, PLL_CLKSEL_IDE_CLKSRC_PLL2);
		if (pll_getPLLEn(PLL_ID_2) != TRUE) {
			pll_setPLLEn(PLL_ID_2, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL4) {
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL4);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, PLL_SRC_ID_PLL4);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_4) / 1000000;
		if (pll_getPLLEn(PLL_ID_4) != TRUE) {
			pll_setPLLEn(PLL_ID_4, TRUE);
		}
	} else if (DevIOCtrl.SEL.GET_SRCCLK.SrcClk == DISPCTRL_SRCCLK_IDE2_PLL7) {
		pll_setClockRate(PLL_CLKSEL_IDE2_CLKSRC, PLL_CLKSEL_IDE2_CLKSRC_PLL7);
//        pll_setConfig(PLL_CONFIG_ID_IDE2_CLK_SRC, PLL_SRC_ID_PLL7);
		uiSrcClkFreq = pll_getPLLFreq(PLL_ID_7) / 1000000;
		if (pll_getPLLEn(PLL_ID_7) != TRUE) {
			pll_setPLLEn(PLL_ID_7, TRUE);
		}
	} else {
		return E_SYS;
	}

	//set to 27MHz
	uiClockDiv = (float)uiSrcClkFreq / (float)27 / (float)2;
	if (uiClockDiv > 0) {
		uiClockDiv--;
	}

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
#else

	// set clock rate
	DevIOCtrl.SEL.SET_CLK_FREQ.uiFreq = 27 * 1000000;
	DevIOCtrl.SEL.SET_CLK_FREQ.bYCC8bit = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_FREQ, &DevIOCtrl);

	// enable clock
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = TRUE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);


#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);

	if ((DevIOCtrl.SEL.GET_SRCCLK.SrcClk < DISPCTRL_SRCCLK_IDE2_PLL1)) {
		if ((pinmux_getDispMode(PINMUX_FUNC_ID_TV)&PINMUX_TV_HDMI_CFG_MASK) == PINMUX_TV_HDMI_CFG_GPIO) {
			pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_GPIO);
		}
	} else {
		if ((pinmux_getDispMode(PINMUX_FUNC_ID_TV)&PINMUX_TV_HDMI_CFG_MASK) == PINMUX_TV_HDMI_CFG_GPIO) {
			pinmux_setPinmux(PINMUX_FUNC_ID_LCD2, PINMUX_LCD_SEL_GPIO);
		}
	}


	DevIOCtrl.SEL.SET_ICST_EN.bEn = FALSE;
	DevIOCtrl.SEL.SET_ICST_EN.Select = CST_RGB2YCBCR;
	pDispDevControl(DISPDEV_IOCTRL_SET_ICST_EN, &DevIOCtrl);

	DevIOCtrl.SEL.SET_DITHER_EN.bEn      = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_DITHER_EN, &DevIOCtrl);

	DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_EMBD_TV;
	pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);

#if 0
	// These parameters would not influence TV out
	ide_setPdir(IDE_PDIR_RGB);
	ide_setOdd(IDE_LCD_R);
	ide_setEven(IDE_LCD_G);
	ide_setHsInv(FALSE);
	ide_setVsInv(FALSE);
	ide_setHvldInv(FALSE);
	ide_setVvldInv(FALSE);
	ide_setClkInv(FALSE);
	ide_setFldInv(FALSE);
#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_TVFULL, &DevIOCtrl);
	fullscreen = DevIOCtrl.SEL.GET_TVFULL.bEnFull;

	pDispDevControl(DISPDEV_IOCTRL_GET_ACT_DEVICE, &DevIOCtrl);

	if (DevIOCtrl.SEL.GET_ACT_DEVICE.DevID == DISPDEV_ID_TVNTSC) {
		//NTSC
		ide_setTvMode(IDE_TV_NTSC_M);
		ide_setTvNtscSetupLevel(0x27);

		if (fullscreen == TRUE) {
			//ide_setHorTiming(0x7F, 0x6B3, 0x144, 0x69D);
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = 0x7F;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = 0x6B3;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = 0xEC;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = 0x68B;
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);

			//ide_set_ver_timing(0x05, 0x20C, 0x32, 0x205, 0x33, 0x206);
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = 0x05;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = 0x20C;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = 0x24;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = 0x203;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart = 0x25;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = 0x204;
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);
		} else {
			//ide_setHorTiming(0x7F, 0x6B3, 0x144, 0x69D);
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = 0x7F;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = 0x6B3;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = 0x13B;//(0x144);
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = 0x694;//(0x69D);
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);

			//ide_set_ver_timing(0x05, 0x20C, 0x32, 0x205, 0x33, 0x206);
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = 0x05;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = 0x20C;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = 0x32;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = 0x205;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart = 0x33;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = 0x206;
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);

		}

		pDispDevControl(DISPDEV_IOCTRL_GET_TVADJUST, &DevIOCtrl);
		if (DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_6_K) {
			uiBurstLevel    = TV_NTSC_BURST_LEVEL_3_6;
			uiBlandLevel    = TV_NTSC_BLAND_LEVEL_3_6;
			uiYScale        = TV_NTSC_Y_SCALE_3_6;
			uiCbScale       = TV_NTSC_CB_SCALE_3_6;
			uiCrScale       = TV_NTSC_CR_SCALE_3_6;
		} else if (DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_0_K) {
			uiBurstLevel    = TV_NTSC_BURST_LEVEL_3_0;
			uiBlandLevel    = TV_NTSC_BLAND_LEVEL_3_0;
			uiYScale        = TV_NTSC_Y_SCALE_3_0;
			uiCbScale       = TV_NTSC_CB_SCALE_3_0;
			uiCrScale       = TV_NTSC_CR_SCALE_3_0;
		} else {
			uiBurstLevel    = TV_NTSC_BURST_LEVEL_3_3;
			uiBlandLevel    = TV_NTSC_BLAND_LEVEL_3_3;
			uiYScale        = TV_NTSC_Y_SCALE_3_3;
			uiCbScale       = TV_NTSC_CB_SCALE_3_3;
			uiCrScale       = TV_NTSC_CR_SCALE_3_3;
		}

		pDispDevControl(DISPDEV_IOCTRL_GET_TVPAR, &DevIOCtrl);
		if (DevIOCtrl.SEL.GET_TVPAR.bEnUser == TRUE) {
			uiBurstLevel    = DevIOCtrl.SEL.GET_TVPAR.uiNTSC_BRL;
			uiBlandLevel    = DevIOCtrl.SEL.GET_TVPAR.uiNTSC_BLL;
			uiYScale        = DevIOCtrl.SEL.GET_TVPAR.uiNTSC_YScaling;
			uiCbScale       = DevIOCtrl.SEL.GET_TVPAR.uiNTSC_CbScaling;
			uiCrScale       = DevIOCtrl.SEL.GET_TVPAR.uiNTSC_CrScaling;
			ide_setTvNtscSetupLevel(DevIOCtrl.SEL.GET_TVPAR.uiNTSC_Setup);
		}
	} else {
		//PAL
		ide_setTvMode(IDE_TV_PAL_BDGHI);
		ide_setTvNtscSetupLevel(0x00);

		if (fullscreen == TRUE) {

			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync		 = 0x01;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal		 = 0x6BF;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart	 = 0x113;//0x113;700x//0x113;720x//0x108; //108 //112
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd 	 = 0x6B2;//0x68A;700x//0x6B2;720x//0x67F; //6A7 //6B1
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);
			
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync		 = 0x04;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal		 = 0x270;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = 0x2C;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd	 = 0x267;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart = 0x2d;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = 0x268;
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);

		}
		else {

			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHsync        = 0x01;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHtotal       = 0x6BF;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldStart    = 0x15C;
			DevIOCtrl.SEL.SET_WINDOW_H_TIMING.uiHvldEnd      = 0x69B;
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_H_TIMING, &DevIOCtrl);

			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVsync        = 0x04;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVtotal       = 0x270;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddStart = 0x040;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldOddEnd   = 0x267;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenStart = 0x041;
			DevIOCtrl.SEL.SET_WINDOW_V_TIMING.uiVvldEvenEnd  = 0x268;
			pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_V_TIMING, &DevIOCtrl);

		}

		pDispDevControl(DISPDEV_IOCTRL_GET_TVADJUST, &DevIOCtrl);
		if (DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_6_K) {
			uiBurstLevel    = TV_PAL_BURST_LEVEL_3_6;
			uiBlandLevel    = TV_PAL_BLAND_LEVEL_3_6;
			uiYScale        = TV_PAL_Y_SCALE_3_6;
			uiCbScale       = TV_PAL_CB_SCALE_3_6;
			uiCrScale       = TV_PAL_CR_SCALE_3_6;
		} else if (DevIOCtrl.SEL.GET_TVADJUST.TvAdjust == DISPDEV_TVADJUST_3_0_K) {
			uiBurstLevel    = TV_PAL_BURST_LEVEL_3_0;
			uiBlandLevel    = TV_PAL_BLAND_LEVEL_3_0;
			uiYScale        = TV_PAL_Y_SCALE_3_0;
			uiCbScale       = TV_PAL_CB_SCALE_3_0;
			uiCrScale       = TV_PAL_CR_SCALE_3_0;
		} else {
			uiBurstLevel    = TV_PAL_BURST_LEVEL_3_3;
			uiBlandLevel    = TV_PAL_BLAND_LEVEL_3_3;
			uiYScale        = TV_PAL_Y_SCALE_3_3;
			uiCbScale       = TV_PAL_CB_SCALE_3_3;
			uiCrScale       = TV_PAL_CR_SCALE_3_3;
		}

		pDispDevControl(DISPDEV_IOCTRL_GET_TVPAR, &DevIOCtrl);
		if (DevIOCtrl.SEL.GET_TVPAR.bEnUser == TRUE) {
			uiBurstLevel    = DevIOCtrl.SEL.GET_TVPAR.uiPAL_BRL;
			uiBlandLevel    = DevIOCtrl.SEL.GET_TVPAR.uiPAL_BLL;
			uiYScale        = DevIOCtrl.SEL.GET_TVPAR.uiPAL_YScaling;
			uiCbScale       = DevIOCtrl.SEL.GET_TVPAR.uiPAL_CbScaling;
			uiCrScale       = DevIOCtrl.SEL.GET_TVPAR.uiPAL_CrScaling;
		}
	}

	DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bInterlaced = TRUE;
	DevIOCtrl.SEL.SET_WINDOW_OUT_TYPE.bFieldOddSt = TRUE;
	pDispDevControl(DISPDEV_IOCTRL_SET_WINDOW_OUT_TYPE, &DevIOCtrl);


	DevIOCtrl.SEL.SET_CSB_EN.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CSB_EN, &DevIOCtrl);


	DevIOCtrl.SEL.SET_YC_EXCHG.bCbCrExchg = FALSE;
	DevIOCtrl.SEL.SET_YC_EXCHG.bYCExchg   = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_YC_EXCHG, &DevIOCtrl);

	DevIOCtrl.SEL.SET_SUBPIXEL.bOddR     = FALSE;
	DevIOCtrl.SEL.SET_SUBPIXEL.bOddG     = FALSE;
	DevIOCtrl.SEL.SET_SUBPIXEL.bOddB     = FALSE;
	DevIOCtrl.SEL.SET_SUBPIXEL.bEvenR    = FALSE;
	DevIOCtrl.SEL.SET_SUBPIXEL.bEvenG    = FALSE;
	DevIOCtrl.SEL.SET_SUBPIXEL.bEvenB    = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_SUBPIXEL, &DevIOCtrl);

	DevIOCtrl.SEL.SET_CLK1_2.bClk1_2     = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK1_2, &DevIOCtrl);


	//
	// TV Related APIs
	//
	ide_setTvBurst(0x00);
	//ide_setTvSlaveMode(TRUE);
	ide_setTvChromaFilter(TRUE, guiTvFilterTap);
	ide_setTvSearchEn(TRUE);
	ide_setTvCkpnPol(FALSE);//NEW DAC use positive edge
	ide_setTvBrl((UINT8)uiBurstLevel);
	ide_setTvBll((UINT8)uiBlandLevel);
	ide_setTvScale((UINT8)uiYScale, (UINT8)uiCbScale, (UINT8)uiCrScale);
	ide_setTvSela(0x3);
	ide_setTvPowerDown(FALSE);


	pDispDevControl(DISPDEV_IOCTRL_GET_ACT_DEVICE, &DevIOCtrl);

	if (fullscreen == TRUE) {
		if (DevIOCtrl.SEL.GET_ACT_DEVICE.DevID == DISPDEV_ID_TVNTSC) {
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = TV_NTSC_Buf_width_full;
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = TV_NTSC_Buf_height_full;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = TV_NTSC_Win_width_full;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = TV_NTSC_Win_height_full;
		} else {
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = TV_PAL_Buf_width_full;
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = TV_PAL_Buf_height_full;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = TV_PAL_Win_width_full;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = TV_PAL_Win_height_full;
		}
	} else {
		if (DevIOCtrl.SEL.GET_ACT_DEVICE.DevID == DISPDEV_ID_TVNTSC) {
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = TV_NTSC_Buf_width;
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = TV_NTSC_Buf_height;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = TV_NTSC_Win_width;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = TV_NTSC_Win_height;
		} else {
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = TV_PAL_Buf_width;
			DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = TV_PAL_Buf_height;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = TV_PAL_Win_width;
			DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = TV_PAL_Win_height;
		}
	}
	pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);

	disptv_debug(("DEVTV: init done\r\n"));
	return E_OK;
}

/*
    Display Device API of Close TV Encoder

    Display Device API of Close TV Encoder

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_OK:     TV Encoder Close done and success.
*/
static ER dispdev_closeTV(void)
{
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pTvIoControl;
#if !_FPGA_EMULATION_
	UINT32                  CurFreq;
	PLL_ID                  SrcPLL = PLL_ID_2;
#endif

	disptv_debug(("DEVTV: close\r\n"));

	// Disable IDE
	DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
	pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

	//pll_disableClock(IDE1_CLK);
	//pll_disableClock(IDE2_CLK);
	// Disable clock
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);

	ide_setTvPowerDown(TRUE);

#if !_FPGA_EMULATION_
	if (bTvDispDevFreqChange == TRUE) {
		pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);
		switch (DevIOCtrl.SEL.GET_SRCCLK.SrcClk) {
		case DISPCTRL_SRCCLK_PLL1:
		case DISPCTRL_SRCCLK_IDE2_PLL1: {
				SrcPLL = PLL_ID_1;
			}
			break;

		case DISPCTRL_SRCCLK_PLL2:
		case DISPCTRL_SRCCLK_IDE2_PLL2: {
				SrcPLL = PLL_ID_2;
			}
			break;

		case DISPCTRL_SRCCLK_PLL4:
		case DISPCTRL_SRCCLK_IDE2_PLL4: {
				SrcPLL = PLL_ID_4;
			}
			break;

		case DISPCTRL_SRCCLK_PLL14:
		case DISPCTRL_SRCCLK_IDE2_PLL14: {
				SrcPLL = PLL_ID_12;
			}
			break;

		default:
			break;
		}

		CurFreq = pll_getPLLFreq(SrcPLL) / 1000000;
		if ((CurFreq != guiTvOrgFreq)) {
			CurFreq = guiTvOrgFreq * 131072 / 12;

			pll_setPLLEn(SrcPLL,    FALSE);
			pll_setPLL(SrcPLL,      CurFreq);
			pll_setPLLEn(SrcPLL,    TRUE);
		}
		bTvDispDevFreqChange = FALSE;
	}
#endif

	return E_OK;
}

/*
    PreGet LCD size

    PreGet LCD size

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.
    @param[in] uiValue  Assigned LCD Register Value

    @return void
*/
static ER dispdev_getTvSize(DISPDEV_GET_PRESIZE *tSize)
{
	//UINT32                  i;
	//PINMUX_LCDINIT          LcdMode;
	//UINT32                  ModeNumber;
	//PINMUX_FUNC_ID          PinFuncID;
	//DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	DISPDEV_IOCTRL          pDispDevControl = pTvIoControl;

	disptv_debug(("DEVTV: getTvSize\r\n"));

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	if (tSize->uiDev == DISPDEV_ID_TVNTSC) {
		tSize->uiBufWidth = TV_NTSC_Buf_width;
		tSize->uiBufHeight = TV_NTSC_Buf_height;
	} else {
		tSize->uiBufWidth = TV_PAL_Buf_width;
		tSize->uiBufHeight = TV_PAL_Buf_height;
	}

	return E_OK;
}

static BOOL dispdev_chgTvClkFreq(void)
{
	//UINT32                  uiTargetFreq = 27000000;
#if !_FPGA_EMULATION_
	DISPDEV_IOCTRL          pDispDevControl = pTvIoControl;
	DISPDEV_IOCTRL_PARAM    DevIoCtrl;

	UINT32                  CurFreq;
	PLL_ID                  SrcPLL;
	BOOL                    bReturn;

	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIoCtrl);

	switch (DevIoCtrl.SEL.GET_SRCCLK.SrcClk) {
	case DISPCTRL_SRCCLK_PLL1:
	case DISPCTRL_SRCCLK_IDE2_PLL1: {
			SrcPLL = PLL_ID_1;
		}
		break;

	case DISPCTRL_SRCCLK_PLL2:
	case DISPCTRL_SRCCLK_IDE2_PLL2: {
			SrcPLL = PLL_ID_2;
		}
		break;

	case DISPCTRL_SRCCLK_PLL4:
	case DISPCTRL_SRCCLK_IDE2_PLL4: {
			SrcPLL = PLL_ID_4;
		}
		break;
#if 0
	case DISPCTRL_SRCCLK_PLL12:
	case DISPCTRL_SRCCLK_IDE2_PLL12: {
			SrcPLL = PLL_ID_12;
		}
		break;
#endif
	case DISPCTRL_SRCCLK_PLL14:
	case DISPCTRL_SRCCLK_IDE2_PLL14: {
			SrcPLL = PLL_ID_14;
		}
		break;

	default: {
			return FALSE;
		}

	}

	CurFreq              = pll_getPLLFreq(SrcPLL) / 1000000;
	disptv_debug(("DEVTV: Original SrcClk(%d)Mhz\r\n", CurFreq));

	guiTvOrgFreq = CurFreq;

	// Calculate PLL Ratio
	CurFreq = CurFreq * 131072 / 12;

	if ((CurFreq != TV_PLL270)) {
		pll_setPLLEn(SrcPLL,    FALSE);
		pll_setPLL(SrcPLL,      TV_PLL270);
		pll_setPLLEn(SrcPLL,    TRUE);
		bReturn = TRUE;
	} else {
		bReturn = FALSE;
	}

	disptv_debug(("DEVTV: Chg PLL to(%d)Mhz\r\n", pll_getPLLFreq(SrcPLL) / 1000000));


	return bReturn;

#else

	return FALSE;

#endif

}

/*
    Set TV Display Device IOCTRL function pointer
*/
static void dispdev_setTvIOCTRL(FP pIoCtrlFunc)
{
	pTvIoControl = (DISPDEV_IOCTRL)pIoCtrlFunc;
}

#endif

/**
    Get TV Display Device Object

    Get TV Display Device Object. This Object is used to control the TV Encoder.

    @return The pointer of TV Display Device object.
*/
PDISPDEV_OBJ dispdev_getTvDevObj(void)
{
	return &DispDev_TvObj;
}
