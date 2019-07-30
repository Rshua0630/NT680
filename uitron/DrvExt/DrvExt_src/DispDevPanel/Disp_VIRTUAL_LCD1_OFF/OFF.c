#include "kernel.h"
#include "Debug.h"
#include "display.h"
#include "dispdev_ioctrl.h"

static ER       dispdev_openIF8Bits(void);
static ER       dispdev_closeIF8Bits(void)
{
	return E_OK;
}
static void     dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue) {}
static UINT32   dispdev_readLcdReg(UINT32 uiAddr)
{
	return 0;
}
static void     dispdev_setIF8bitsIOCTRL(FP pIoCtrlFunc);
static ER       dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot)
{
	return E_OK;
}
static ER       dispdev_getLcdSize(PDISPDEV_GET_PRESIZE tLCDSize);

static DISPDEV_IOCTRL pDispDevControl = NULL;

static DISPDEV_OBJ DispDev_IF8bitsObj = { dispdev_openIF8Bits, dispdev_closeIF8Bits, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdSize, dispdev_setIF8bitsIOCTRL, {NULL, NULL}};

static ER dispdev_openIF8Bits(void)
{
	DISPDEV_IOCTRL_PARAM DevIOCtrl = {0};
	DevIOCtrl.SEL.SET_DISPSIZE.uiBufWidth     = 80;
	DevIOCtrl.SEL.SET_DISPSIZE.uiBufHeight    = 60;
	DevIOCtrl.SEL.SET_DISPSIZE.uiWinWidth     = 80;
	DevIOCtrl.SEL.SET_DISPSIZE.uiWinHeight    = 60;
	pDispDevControl(DISPDEV_IOCTRL_SET_DISPSIZE, &DevIOCtrl);

	return E_OK;
}

static void dispdev_setIF8bitsIOCTRL(FP pIoCtrlFunc)
{
	pDispDevControl = (DISPDEV_IOCTRL)pIoCtrlFunc;
}

static ER dispdev_getLcdSize(PDISPDEV_GET_PRESIZE tLCDSize)
{
	tLCDSize->uiBufWidth = 80;
	tLCDSize->uiBufHeight = 60;
	return E_OK;
}

PDISPDEV_OBJ dispdev_getLcd1DevObj(void)
{
	return &DispDev_IF8bitsObj;
}