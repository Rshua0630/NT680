/*
    Display device object Golden Sample for driving RGB/YUV Paral panel

    @file       dispdev_gsinfParal.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "kernel.h"
#include "ide.h"
#include "top.h"
#include "gpio.h"
#include "pll.h"
#include "sif.h"
#include "Utility.h"
#include "display.h"
#include "dispdev_IFParal.h"
#include "dispdev_panlcomn.h"


static void     DispDeviceInit(tLCD_PARAM *pMode);


static ER       dispdev_openIFParal(void);
static ER       dispdev_closeIFParal(void);
static void     dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue);
static UINT32   dispdev_readLcdReg(UINT32 uiAddr);
static void     dispdev_setIFParalIOCTRL(FP pIoCtrlFunc);
static ER       dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot);
static ER       dispdev_getLcdSize(PDISPDEV_GET_PRESIZE tLCDSize);



/*
    Exporting display device object
*/
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
static DISPDEV_OBJ     DispDev_IFParalObj = { dispdev_openIFParal, dispdev_closeIFParal, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdSize, dispdev_setIFParalIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIFParalIoControl1 = NULL;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
static DISPDEV_OBJ     DispDev_IFParalObj2 = { dispdev_openIFParal, dispdev_closeIFParal, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdSize, dispdev_setIFParalIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIFParalIoControl2 = NULL;
#endif




static void DispDeviceInit(tLCD_PARAM *pMode)
{
	UINT32                  i;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl1;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl2;
#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF) {
		sif_open(DevIOCtrl.SEL.GET_REG_IF.uiSifCh);

		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_BUSMODE,  USED_SIF_MODE);
		// Bus clock = 3 MHz
		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_BUSCLOCK, 3000000);
		// 16 half bus clock = 2666.66 ns
		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SEND,     2667);
		// 0 half bus clock = 0 ns
		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SENS,     0);
		// 0 half bus clock = 0 ns
		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SENH,     0);
		// 16 bits per transmission
		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_LENGTH,   16);
		// MSb shift out first
		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_DIR,      SIF_DIR_MSB);
		// Delay between successive transmission
		sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_DELAY,    2000);
	}

	pDispDevControl(DISPDEV_IOCTRL_GET_PANEL_ADJUST, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_PANEL_ADJUST.pfpAdjust != NULL) {
		DevIOCtrl.SEL.GET_PANEL_ADJUST.pfpAdjust();
	} else if (pMode->pCmdQueue != NULL) {
		for (i = 0; i < pMode->nCmd; i++) {
			if (pMode->pCmdQueue[i].uiAddress == CMDDELAY_MS) {
				Delay_DelayMs(pMode->pCmdQueue[i].uiValue);
			} else if (pMode->pCmdQueue[i].uiAddress == CMDDELAY_US) {
				Delay_DelayUs(pMode->pCmdQueue[i].uiValue);
			} else {
				dispdev_writeLcdReg(pMode->pCmdQueue[i].uiAddress, pMode->pCmdQueue[i].uiValue);
			}
		}
	}

}

#if 1
/*
    Display Device API of open LCD with RGB/YUV Paral  interface

    Display Device API of open LCD with RGB/YUV Paral  interface.

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_NOSPT:  Driver without supporting the specified LCD Mode.
     - @b E_OK:     LCD Open done and success.
*/
static ER dispdev_openIFParal(void)
{
	UINT32                  i;
	tLCD_PARAM              *pMode      = NULL;
	tLCD_PARAM              *pConfigMode = NULL;
	PINMUX_LCDINIT          LcdMode;
	UINT32                  ModeNumber;
	PINMUX_FUNC_ID          PinFuncID;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl1;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl2;
#endif

	DBG_IND("openIFParal START\r\n");

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	//
	//  Sortig the SysInit Config Mode support
	//
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif ((DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID) & (~PINMUX_LCDMODE_AUTO_PINMUX);
	pConfigMode = dispdev_getConfigModeParal(&ModeNumber);
	if ((pConfigMode != NULL) && (ModeNumber > 0)) {
		for (i = 0; i < ModeNumber; i++) {
			if (pConfigMode[i].Panel.LCDMode == LcdMode) {
				pMode = (tLCD_PARAM *)&pConfigMode[i];
				break;
			}
		}
	}

	if (pMode == NULL) {
		DBG_ERR("LCDMode=%d not support\r\n", LcdMode);
		return E_NOSPT;
	}

#if 1
	if (LcdMode == PINMUX_LCDMODE_CCIR656) {
		UINT32 uiFP, uiFieldEnd, uiOddStart;
		/*DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = 0x20C;  //mismatch setting
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = 0x16;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = 0x106;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = 0x11C;
		*/
		/*DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = 0x000; //IDE odd,even = 0x2c, 0x2d
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = 0x016;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = 0x106;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = 0x11D;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldStart  = 0x04;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldEnd    = 0x10A;*/

		DBG_IND("Field0_VValidST<->uiOddEnd---------->[0x%02x]\r\n", (pMode->Panel.uiVSyncBackPorchOdd >> 1));
		DBG_IND("uiEvenStart[CEVENBLKST]------------->[0x%04x]\r\n", (pMode->Panel.uiVSyncActivePeriod >> 1) + (pMode->Panel.uiVSyncBackPorchOdd >> 1));
		DBG_IND("CEVENBLKED[uiEvenEnd]--------------->[0x%04x]\r\n", (pMode->Panel.uiVSyncActivePeriod >> 1) + (pMode->Panel.uiVSyncBackPorchOdd >> 1) + ((pMode->Panel.uiVSyncTotalPeriod - pMode->Panel.uiVSyncActivePeriod + 1) >> 1));
		DBG_IND("Field1_VValidEnd-------------------->[0x%04x]\r\n", (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchOdd) - 1);

		uiFP = pMode->Panel.uiVSyncTotalPeriod - pMode->Panel.uiVSyncActivePeriod - pMode->Panel.uiVSyncBackPorchOdd;
		if (uiFP % 2) {
			uiFP = ((uiFP + 1) >> 1);
		}
		uiFieldEnd = (pMode->Panel.uiVSyncBackPorchOdd >> 1) + (pMode->Panel.uiVSyncActivePeriod >> 1) + uiFP + (pMode->Panel.uiVSyncSYNCwidth >> 1);
		DBG_IND("uiFieldEnd-------------------------->[0x%04x]\r\n", uiFieldEnd);
		uiOddStart = (pMode->Panel.uiVSyncActivePeriod >> 1) + (pMode->Panel.uiVSyncBackPorchOdd >> 1) + ((pMode->Panel.uiVSyncTotalPeriod - pMode->Panel.uiVSyncActivePeriod + 1) >> 1) + (pMode->Panel.uiVSyncActivePeriod >> 1);
		DBG_IND("uiOddStart-------------------------->[0x%04x]\r\n", uiOddStart);
#if 1
#if 0
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = 0x209;   //IDE odd,even = 0x24, 0x25
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = 0x12;    //=pMode.Panel.uiVSyncBackPorchOdd / 2
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = 0x102;   //=pMode.Panel.uiVSyncActivePeriod / 2 + pMode.Panel.uiVSyncBackPorchOdd / 2
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = 0x119;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldStart  = 0x04;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldEnd    = 0x10A;
#else
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = uiOddStart;  //IDE odd,even = 0x24, 0x25
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = (pMode->Panel.uiVSyncBackPorchOdd >> 1); //=pMode.Panel.uiVSyncBackPorchOdd / 2
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = (pMode->Panel.uiVSyncActivePeriod >> 1) + (pMode->Panel.uiVSyncBackPorchOdd >> 1);   //=pMode.Panel.uiVSyncActivePeriod / 2 + pMode.Panel.uiVSyncBackPorchOdd / 2
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = (pMode->Panel.uiVSyncActivePeriod >> 1) + (pMode->Panel.uiVSyncBackPorchOdd >> 1) + ((pMode->Panel.uiVSyncTotalPeriod - pMode->Panel.uiVSyncActivePeriod + 1) >> 1);
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldStart  = (pMode->Panel.uiVSyncSYNCwidth >> 1);
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldEnd    = uiFieldEnd;
#endif
#else
		//1080I
		//ide_setDigitalTiming(0x464, (0x015), (0x231), (0x248), 0, (564));
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = 0x464;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = 0x015;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = 0x231;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = 0x248;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldStart  = 0x000;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldEnd    = 0x234;
#endif

		pDispDevControl(DISPDEV_IOCTRL_SET_CCIR656_SYNCCODE, &DevIOCtrl);
	}
#endif

	// Config & Enable Display physical engine
	dispdev_setDisplay(pDispDevControl, tLCD_INF_PARALLEL_16BITS, pMode, PinFuncID);

	// Config & Enable display device
	DispDeviceInit(pMode);

	DBG_IND("openIFParal Done mode[%d]\r\n", pMode);

#if 0
	// Test Panel Frame Rate
	{
		UINT32 i;

		Perf_Open();
		Perf_Mark();
		for (i = 0; i < 1000; i++) {
			pDevIF->waitFrmEnd();
		}
		DBG_ERR("Time tick = %d us\r\n", Perf_GetDuration());
		Perf_Close();
	}
#endif

	return E_OK;
}

/*
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @param[in] pDevIF Please refer to dispdevctrl.h for details.

    @return
     - @b E_OK:     LCD Close done and success.
*/
static ER dispdev_closeIFParal(void)
{
	UINT32                  i, CmdNumber;
	tPANEL_CMD              *pStandby = NULL;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	PINMUX_FUNC_ID          PinFuncID;
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl1;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl2;
#endif

	dispanl_debug(("closeIFParal START\r\n"));

	// Close Display Engine First
	DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
	pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	//pll_disableClock(IDE1_CLK);
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	//pll_disableClock(IDE2_CLK);
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);
#endif

	pStandby = dispdev_getStandbyCmdParal(&CmdNumber);

	if (pStandby != NULL) {
		// Set display device into Standby or powerdown.
		for (i = 0; i < CmdNumber; i++) {
			dispdev_writeLcdReg(pStandby[i].uiAddress, pStandby[i].uiValue);
		}
	}

	// Close SIF if needed.
	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF) {
		sif_close(DevIOCtrl.SEL.GET_REG_IF.uiSifCh);
	}

	// close pinmux if needed.
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif ((DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	if (pinmux_getDispMode(PinFuncID) & PINMUX_LCDMODE_AUTO_PINMUX) {
		pinmux_setPinmux(PinFuncID, PINMUX_LCD_SEL_GPIO);
	}

	dispanl_debug(("closeIFParal Done\r\n"));
	return E_OK;
}

/*
    Write LCD Register

    Write LCD Register

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.
    @param[in] uiValue  Assigned LCD Register Value

    @return void
*/
static void dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue)
{
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl1;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl2;
#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO) {
		dispdev_writeToLcdGpioParal(pDispDevControl, uiAddr, uiValue);
	} else if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF) {
		dispdev_writeToLcdSifParal(pDispDevControl, uiAddr, uiValue);
	}
	//return E_OK;
}

/*
    PreGet LCD size

    PreGet LCD size

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.
    @param[in] uiValue  Assigned LCD Register Value

    @return void
*/
static ER dispdev_getLcdSize(DISPDEV_GET_PRESIZE *tSize)
{
	UINT32                  i;
	tLCD_PARAM              *pMode      = NULL;
	tLCD_PARAM              *pConfigMode = NULL;
	PINMUX_LCDINIT          LcdMode;
	UINT32                  ModeNumber;
	PINMUX_FUNC_ID          PinFuncID;
	//DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl1;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl2;
#endif

	dispanl_debug(("getLcdSize START\r\n"));

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	tSize->uiBufWidth = 0;
	tSize->uiBufHeight = 0;

	//
	//  Sortig the SysInit Config Mode support
	//
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif((DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID);
	pConfigMode = dispdev_getConfigModeParal(&ModeNumber);
	if ((pConfigMode != NULL) && (ModeNumber > 0)) {
		for (i = 0; i < ModeNumber; i++) {
			if (pConfigMode[i].Panel.LCDMode == LcdMode) {
				pMode = (tLCD_PARAM *)&pConfigMode[i];
				break;
			}
		}
	}

	if (pMode == NULL) {
		DBG_ERR("LCDMode=%d not support\r\n", LcdMode);
		return E_NOSPT;
	} else {
		tSize->uiBufWidth = pMode->Panel.uiBufferWidth;
		tSize->uiBufHeight = pMode->Panel.uiBufferHeight;
	}
	return E_OK;
}




/*
    Read LCD Register

    Read LCD Register

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.

    @return Read back LCD Register Value
*/
static UINT32  dispdev_readLcdReg(UINT32 uiAddr)
{
	UINT32                  uiValue = 0x0;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl1;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFParalIoControl2;
#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO) {
		dispdev_readFromLcdGpioParal(pDispDevControl, uiAddr, &uiValue);
	} else {
		DBG_ERR("Error dispdev_RegRead Op!\r\n");
	}

	return uiValue;
}

/*
    Set Display Device IOCTRL function pointer
*/
static void dispdev_setIFParalIOCTRL(FP pIoCtrlFunc)
{
#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
	pIFParalIoControl1 = (DISPDEV_IOCTRL)pIoCtrlFunc;
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
	pIFParalIoControl2 = (DISPDEV_IOCTRL)pIoCtrlFunc;
#endif
}

/*
    Set LCD Rotation
*/
static ER dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot)
{
	UINT32          ModeNo, index, i;
	tLCD_ROT        *pCmdRot = NULL;
	BOOL            bValid = FALSE;

	pCmdRot = dispdev_getLcdRotateCmdParal(&ModeNo);

	// Search Valid Rotattion Config table index
	for (index = 0; index < ModeNo; index++) {
		if (pCmdRot[index].Rotation == Rot) {
			bValid = TRUE;
			break;
		}
	}

	if (bValid == FALSE) {
		DBG_ERR("DispDev NO valid rotate!\r\n\r\n");
		return E_PAR;
	}


	if (pCmdRot[index].pCmdQueue != NULL) {
		for (i = 0; i < pCmdRot[index].nCmd; i++) {
			if (pCmdRot[index].pCmdQueue[i].uiAddress == CMDDELAY_MS) {
				Delay_DelayMs(pCmdRot[index].pCmdQueue[i].uiValue);
			} else if (pCmdRot[index].pCmdQueue[i].uiAddress == CMDDELAY_US) {
				Delay_DelayUs(pCmdRot[index].pCmdQueue[i].uiValue);
			} else {
				dispdev_writeLcdReg(pCmdRot[index].pCmdQueue[i].uiAddress, pCmdRot[index].pCmdQueue[i].uiValue);
			}
		}
	}

	return E_OK;
}

#endif


#if (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD1)
/**
    Get LCD 1 Device Object

    Get LCD 1 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd1DevObj(void)
{
	return &DispDev_IFParalObj;
}
#elif (DISPLCDSEL_IFPARAL_TYPE == DISPLCDSEL_IFPARAL_LCD2)
/**
    Get LCD 2 Device Object

    Get LCD 2 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd2DevObj(void)
{
	return &DispDev_IFParalObj2;
}

#endif

