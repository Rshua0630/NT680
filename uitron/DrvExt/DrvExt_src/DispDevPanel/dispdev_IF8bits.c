/*
    Display device object Golden Sample for driving RGB/YUV 8bits panel

    @file       dispdev_gsinf8bits.c
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
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"


static void     DispDeviceInit(tLCD_PARAM *pMode);


static ER       dispdev_openIF8Bits(void);
static ER       dispdev_closeIF8Bits(void);
static void     dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue);
static UINT32   dispdev_readLcdReg(UINT32 uiAddr);
static void     dispdev_setIF8bitsIOCTRL(FP pIoCtrlFunc);
static ER       dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot);
static ER       dispdev_getLcdSize(PDISPDEV_GET_PRESIZE tLCDSize);



/*
    Exporting display device object
*/
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
static DISPDEV_OBJ     DispDev_IF8bitsObj = { dispdev_openIF8Bits, dispdev_closeIF8Bits, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdSize, dispdev_setIF8bitsIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIF8bitsIoControl1 = NULL;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
static DISPDEV_OBJ     DispDev_IF8bitsObj2 = { dispdev_openIF8Bits, dispdev_closeIF8Bits, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdSize, dispdev_setIF8bitsIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIF8bitsIoControl2 = NULL;
#endif




static void DispDeviceInit(tLCD_PARAM *pMode)
{
	UINT32                  i;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	tLCD_SIF_PARAM         *DevSIFParam;
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl2;
#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF) {
		sif_open(DevIOCtrl.SEL.GET_REG_IF.uiSifCh);

		if (dispdev_getLcdSifParam) {
			DevSIFParam = dispdev_getLcdSifParam();
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_BUSMODE,  DevSIFParam->uiSIFMode);
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_BUSCLOCK, DevSIFParam->uiBusClock);
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SENS,     DevSIFParam->uiSENS);
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SENH,     DevSIFParam->uiSENH);
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_LENGTH,   DevSIFParam->uiTransferLength);
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_DIR,      DevSIFParam->uiSIFDir);
		} else {
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_BUSMODE,  USED_SIF_MODE);
			// Bus clock = 3 MHz
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_BUSCLOCK, 3000000);
			// 16 half bus clock = 2666.66 ns => DMA related param
			// sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SEND,     2667);
			// 0 half bus clock = 0 ns
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SENS,     20);
			// 0 half bus clock = 0 ns
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_SENH,     0);
			// 16 bits per transmission
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_LENGTH,   24);
			// MSb shift out first
			sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_DIR,      SIF_DIR_MSB);
			// Delay between successive transmission => DMA related param
			//sif_setConfig((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, SIF_CONFIG_ID_DELAY,    2000);
		}
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
    Display Device API of open LCD with RGB/YUV Serial 8bits interface

    Display Device API of open LCD with RGB/YUV Serial 8bits interface.

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_NOSPT:  Driver without supporting the specified LCD Mode.
     - @b E_OK:     LCD Open done and success.
*/
static ER dispdev_openIF8Bits(void)
{
	UINT32                  i;
	tLCD_PARAM              *pMode      = NULL;
	tLCD_PARAM              *pConfigMode = NULL;
	PINMUX_LCDINIT          LcdMode;
	UINT32                  ModeNumber;
	PINMUX_FUNC_ID          PinFuncID;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl2;
#endif

	dispanl_debug(("openIF8Bits START\r\n"));

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	//
	//  Sortig the SysInit Config Mode support
	//
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif ((DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID) & (~PINMUX_LCDMODE_AUTO_PINMUX);

#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	pConfigMode = dispdev_getConfigMode(&ModeNumber);
#elif ((DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2))
	pConfigMode = dispdev_getConfigMode2(&ModeNumber);
#endif

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
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = 0x209;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = 0x12;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = 0x102;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = 0x119;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldStart  = 0x04;
		DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldEnd    = 0x10A;
		pDispDevControl(DISPDEV_IOCTRL_SET_CCIR656_SYNCCODE, &DevIOCtrl);
	}
#endif

	// Config & Enable Display physical engine
	dispdev_setDisplay(pDispDevControl, tLCD_INF_SERIAL_8BITS, pMode, PinFuncID);

	// Config & Enable display device
	DispDeviceInit(pMode);

	dispanl_debug(("openIF8Bits Done\r\n"));

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
static ER dispdev_closeIF8Bits(void)
{
	UINT32                  i, CmdNumber;
	tPANEL_CMD              *pStandby = NULL;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	PINMUX_FUNC_ID          PinFuncID;
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl2;
#endif

	dispanl_debug(("closeIF8Bits START\r\n"));

	// Close Display Engine First
	DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
	pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	//pll_disableClock(IDE1_CLK);
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	//pll_disableClock(IDE2_CLK);
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);
#endif

#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	pStandby = dispdev_getStandbyCmd(&CmdNumber);
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	pStandby = dispdev_getStandbyCmd2(&CmdNumber);
#endif

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
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif ((DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	if (pinmux_getDispMode(PinFuncID) & PINMUX_LCDMODE_AUTO_PINMUX) {
		pinmux_setPinmux(PinFuncID, PINMUX_LCD_SEL_GPIO);
	}

	dispanl_debug(("closeIF8Bits Done\r\n"));
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
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl2;
#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO) {
		dispdev_writeToLcdGpio(pDispDevControl, uiAddr, uiValue);
	} else if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF) {
		dispdev_writeToLcdSif(pDispDevControl, uiAddr, uiValue);
	}
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO) {
		dispdev_writeToLcdGpio2(pDispDevControl, uiAddr, uiValue);
	} else if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF) {
		dispdev_writeToLcdSif2(pDispDevControl, uiAddr, uiValue);
	}
#endif
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
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl2;
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
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif((DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID);
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	pConfigMode = dispdev_getConfigMode(&ModeNumber);
#elif((DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2))
	pConfigMode = dispdev_getConfigMode2(&ModeNumber);
#endif
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
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl2;
#endif

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO) {
		dispdev_readFromLcdGpio(pDispDevControl, uiAddr, &uiValue);
	} else {
		DBG_ERR("Error dispdev_RegRead Op!\r\n");
	}
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	if (DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO) {
		dispdev_readFromLcdGpio2(pDispDevControl, uiAddr, &uiValue);
	} else {
		DBG_ERR("Error dispdev_RegRead Op!\r\n");
	}
#endif

	return uiValue;
}

/*
    Set Display Device IOCTRL function pointer
*/
static void dispdev_setIF8bitsIOCTRL(FP pIoCtrlFunc)
{
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	pIF8bitsIoControl1 = (DISPDEV_IOCTRL)pIoCtrlFunc;
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	pIF8bitsIoControl2 = (DISPDEV_IOCTRL)pIoCtrlFunc;
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

#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
	pCmdRot = dispdev_getLcdRotateCmd(&ModeNo);
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
	pCmdRot = dispdev_getLcdRotateCmd2(&ModeNo);
#endif

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


#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
/**
    Get LCD 1 Device Object

    Get LCD 1 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd1DevObj(void)
{
	return &DispDev_IF8bitsObj;
}
#elif (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD2)
/**
    Get LCD 2 Device Object

    Get LCD 2 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd2DevObj(void)
{
	return &DispDev_IF8bitsObj2;
}

#endif

