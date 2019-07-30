/*
    Display device object Golden Sample for driving Memory interface panel

    @file       dispdev_gsinfmi.c
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
#include "dispdev_IFmi.h"
#include "dispdev_panlcomn.h"
#include "mi.h"

static void     DispDeviceInit(tLCD_PARAM *pMode);

static ER       dispdev_openIFMi(void);
static ER       dispdev_closeIFMi(void);
static ER       dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot);
static void     dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue);
static UINT32   dispdev_readLcdReg(UINT32 uiAddr);
static void     dispdev_setIFMiIOCTRL(FP pIoCtrlFunc);
static ER       dispdev_getLcdMiSize(PDISPDEV_GET_PRESIZE tLCDSize);


/*
    Exporting display device object
*/
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
static DISPDEV_OBJ     DispDev_IFMiObj_Lcd1 = { dispdev_openIFMi, dispdev_closeIFMi, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdMiSize, dispdev_setIFMiIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIFMiIoControl1 = NULL;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
static DISPDEV_OBJ     DispDev_IFMiObj_Lcd2 = { dispdev_openIFMi, dispdev_closeIFMi, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdMiSize, dispdev_setIFMiIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIFMiIoControl2 = NULL;
#endif

static void DispDeviceInit(tLCD_PARAM *pMode)
{
	UINT32                  i;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl1;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl2;
#endif

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
    Display Device API of open LCD with MI interface

    Display Device API of open LCD with MI interface

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_NOSPT:  Driver without supporting the specified LCD Mode.
     - @b E_OK:     LCD Open done and success.
*/
static ER dispdev_openIFMi(void)
{
	UINT32                  i;
	tLCD_PARAM              *pMode      = NULL;
	tLCD_PARAM              *pConfigMode = NULL;
	PINMUX_LCDINIT          LcdMode;
	UINT32                  ModeNumber;
	tMI_CONFIG              MiConfig;
	PINMUX_FUNC_ID          PinFuncID;
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl1;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl2;
#endif

	dispanl_debug(("openIFMi START\r\n"));

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	//
	//  Sortig the SysInit Config Mode support
	//
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID) & (~PINMUX_LCDMODE_AUTO_PINMUX);
	pConfigMode = dispdev_getConfigModeMI(&ModeNumber);
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

	if (mi_isOpened()) {
		mi_close();
	}

	mi_open();

	switch (LcdMode) {
	case PINMUX_LCDMODE_MI_FMT0:
		MiConfig.DataFmt = MI_DATAFMT0;
		break;
	case PINMUX_LCDMODE_MI_FMT1:
		MiConfig.DataFmt = MI_DATAFMT1;
		break;
	case PINMUX_LCDMODE_MI_FMT2:
		MiConfig.DataFmt = MI_DATAFMT2;
		break;
	case PINMUX_LCDMODE_MI_FMT3:
		MiConfig.DataFmt = MI_DATAFMT3;
		break;
	case PINMUX_LCDMODE_MI_FMT4:
		MiConfig.DataFmt = MI_DATAFMT4;
		break;
	case PINMUX_LCDMODE_MI_FMT5:
		MiConfig.DataFmt = MI_DATAFMT5;
		break;
	case PINMUX_LCDMODE_MI_FMT6:
		MiConfig.DataFmt = MI_DATAFMT6;
		break;
	case PINMUX_LCDMODE_MI_FMT7:
		MiConfig.DataFmt = MI_DATAFMT7;
		break;
	case PINMUX_LCDMODE_MI_FMT8:
		MiConfig.DataFmt = MI_DATAFMT8;
		break;
	case PINMUX_LCDMODE_MI_FMT9:
		MiConfig.DataFmt = MI_DATAFMT9;
		break;
	case PINMUX_LCDMODE_MI_FMT10:
		MiConfig.DataFmt = MI_DATAFMT10;
		break;
	case PINMUX_LCDMODE_MI_FMT11:
		MiConfig.DataFmt = MI_DATAFMT11;
		break;
	case PINMUX_LCDMODE_MI_FMT12:
		MiConfig.DataFmt = MI_DATAFMT12;
		break;
	case PINMUX_LCDMODE_MI_FMT13:
		MiConfig.DataFmt = MI_DATAFMT13;
		break;
	case PINMUX_LCDMODE_MI_FMT14:
		MiConfig.DataFmt = MI_DATAFMT14;
		break;
	case PINMUX_LCDMODE_MI_SERIAL_BI:
		MiConfig.DataFmt = MI_DATAFMT1;
		break;

	default:
		MiConfig.DataFmt = MI_DATAFMT0;
		DBG_ERR("openIFMi not supported mode! \r\n");
		break;
	}

#if _FPGA_EMULATION_
	MiConfig.fMiSrcClk = 24.0;
#else
	MiConfig.fMiSrcClk = 240.0;
#endif
	dispdev_setMiConfig(&MiConfig);

	mi_setManual(TRUE);

	// Config & Enable Display physical engine
	dispdev_setDisplay(pDispDevControl, tLCD_INF_MI, pMode, PinFuncID);

	// Config & Enable display device
	DispDeviceInit(pMode);

	dispdev_setMiAutoCmd();

	mi_setManual(FALSE);
	mi_setEn(TRUE);

	dispanl_debug(("openIFMi Done\r\n"));
#if 0
	// Test Panel Frame Rate
	{
		UINT32 i;

		Perf_Open();
		Perf_Mark();
		for (i = 0; i < 1000; i++) {
			pDevIF->waitFrmEnd();
		}
		debug_err(("Time tick = %d us\r\n", Perf_GetDuration()));
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
static ER dispdev_closeIFMi(void)
{
	UINT32                  i, CmdNumber;
	tPANEL_CMD              *pStandby = NULL;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	PINMUX_FUNC_ID          PinFuncID;
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl1;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl2;
#endif
	MI_TYPE     Type = PARALLEL_I80;
	//#NT#2011/11/15#Klins Chen -begin
	MI_AUTO_FMT             AutoType = MI_AUTOFMT0;
	MI_DATAFMT              DataFMT = MI_DATAFMT0;
	BOOL                    bRGBSWAP = FALSE;
	BOOL                    bBITSWAP = FALSE;
	BOOL                    bSync = FALSE;
	BOOL                    bPINMUX = FALSE;

	dispanl_debug(("closeIFMi START\r\n"));

	mi_getCtrl(&Type, &AutoType, &DataFMT, &bRGBSWAP, &bBITSWAP, &bSync, &bPINMUX);

	if (bSync == FALSE) {
		mi_setEn(FALSE);
	}

	DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
	pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

	if (bSync == TRUE) {
		mi_setEn(FALSE);
	}

#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	//pll_disableClock(IDE1_CLK);
	//pll_disableClock(IDE2_CLK);
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);
#endif

	mi_setManual(TRUE);

	mi_setCtrl(Type,   AutoType,  DataFMT,  bRGBSWAP,  bBITSWAP,  FALSE,  bPINMUX);
	//#NT#2011/11/15#Klins Chen -end

	pStandby = dispdev_getStandbyCmdMI(&CmdNumber);
	if (pStandby != NULL) {
		// Set display device into Standby or powerdown.
		for (i = 0; i < CmdNumber; i++) {

			if (pStandby[i].uiAddress == CMDDELAY_MS) {
				Delay_DelayMs(pStandby[i].uiValue);
			} else if (pStandby[i].uiAddress == CMDDELAY_US) {
				Delay_DelayUs(pStandby[i].uiValue);
			} else {
				dispdev_writeLcdReg(pStandby[i].uiAddress, pStandby[i].uiValue);
			}
		}
	}

	mi_close();

	// close pinmux if needed.
#if (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif ((DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	if (pinmux_getDispMode(PinFuncID) & PINMUX_LCDMODE_AUTO_PINMUX) {
		pinmux_setPinmux(PinFuncID, PINMUX_LCD_SEL_GPIO);
	}

	dispanl_debug(("closeIFMi Done\r\n"));
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
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl1;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl2;
#endif
	dispdev_writeToLcdMi(pDispDevControl, uiAddr, uiValue);
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
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl1;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl2;
#endif

	dispdev_readFromLcdMi(pDispDevControl, uiAddr, &uiValue);
	return uiValue;
}

/*
    PreGet LCD size

    PreGet LCD size

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.
    @param[in] uiValue  Assigned LCD Register Value

    @return void
*/
static ER dispdev_getLcdMiSize(DISPDEV_GET_PRESIZE *tSize)
{
	UINT32                  i;
	tLCD_PARAM              *pMode      = NULL;
	tLCD_PARAM              *pConfigMode = NULL;
	PINMUX_LCDINIT          LcdMode;
	UINT32                  ModeNumber;
	PINMUX_FUNC_ID          PinFuncID;
	//DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl1;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFMiIoControl2;
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
#if (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif ((DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID);
	pConfigMode = dispdev_getConfigModeMI(&ModeNumber);
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
    Set TV Display Device IOCTRL function pointer
*/
static void dispdev_setIFMiIOCTRL(FP pIoCtrlFunc)
{
#if (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
	pIFMiIoControl1 = (DISPDEV_IOCTRL)pIoCtrlFunc;
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
	pIFMiIoControl2 = (DISPDEV_IOCTRL)pIoCtrlFunc;
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

	pCmdRot = dispdev_getLcdRotateMiCmd(&ModeNo);

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

	mi_setEn(FALSE);
	Delay_DelayMs(40);
	mi_setManual(TRUE);

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

	dispdev_setMiAutoCmd();

	mi_setManual(FALSE);
	mi_setEn(TRUE);

	return E_OK;
}


#endif

#if   (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD1)
/**
    Get LCD 1 Device Object

    Get LCD 1 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd1DevObj(void)
{
	return &DispDev_IFMiObj_Lcd1;
}
#elif (DISPLCDSEL_IFMI_TYPE == DISPLCDSEL_IFMI_LCD2)
/**
    Get LCD 2 Device Object

    Get LCD 2 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd2DevObj(void)
{
	return &DispDev_IFMiObj_Lcd2;
}
#endif

