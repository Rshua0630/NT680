#include "Type.h"
#include "DrvExt.h"
#include "Dx.h"
#include "DxCommon.h"
#include "modelext_parser.h"
#include "bin_info.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//IO
/*
[PinmuxCfg.c]
pinmux

[GpioMapping.c]
cgpio x32 (Storage)
pgpio x64 (Peripheral)
sgpio x30 (SENSOR)
lgpio x28 (LCD)
dgpio x9 (Dedicated)
pwm x20
pwm_ccnt x3

[VoltageDet.c]
adc x8

[SerialComm.c]
i2c x2 x2
sif x1 x6
csi x2 x4
lvds x1 x10
spi x3
uart x2
*/

extern DX_OBJECT gDevLCD;
extern DX_OBJECT gDevLCD2;
extern DX_OBJECT gDevTV;
extern DX_OBJECT gDevHDMI;
extern DX_OBJECT gDevEmbMem0;
extern DX_OBJECT gDevEmbMem1;
extern DX_OBJECT gDevEmbMem2;
extern DX_OBJECT gDevEmbMem3;
extern DX_OBJECT gDevEmbMem4;
extern DX_OBJECT gDevEmbMem5;
extern DX_OBJECT gDevEmbMem6;
extern DX_OBJECT gDevEmbMem7;
extern DX_OBJECT gDevEmbMem8;
extern DX_OBJECT gDevCARD1;
extern DX_OBJECT gDevCARD2;
extern DX_OBJECT gDevUSB;
extern DX_OBJECT gDevSnd;
extern DX_OBJECT gDevPowerBATT;
extern DX_OBJECT gDevPowerDC;
extern DX_OBJECT gDevLED;

DX_HANDLE Dx_GetObject(UINT32 DxClassType)  // Query device object
{
	UINT32 hDevice = 0;

	switch (DxClassType & DX_CLASS_MASK) {
	//external device
	case DX_CLASS_DISPLAY_EXT:
		if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_LCD) {
			hDevice = (UINT32)(&gDevLCD);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_TVOUT) {
			hDevice = (UINT32)(&gDevTV);
		}
#if !defined(_BSP_NA51023_)
		else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_HDMIOUT) {
			hDevice = (UINT32)(&gDevHDMI);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_LCD2) {
			hDevice = (UINT32)(&gDevLCD2);
		}
#endif
		break;
	case DX_CLASS_AUDIO_EXT:
		hDevice = (UINT32)(&gDevSnd);
		break;
	case DX_CLASS_SENSOR_EXT:
		break;
	case DX_CLASS_STORAGE_EXT:
		if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM0) {
			hDevice = (UINT32)(&gDevEmbMem0);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM1) {
			hDevice = (UINT32)(&gDevEmbMem1);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM2) {
			hDevice = (UINT32)(&gDevEmbMem2);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM3) {
			hDevice = (UINT32)(&gDevEmbMem3);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM4) {
			hDevice = (UINT32)(&gDevEmbMem4);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM5) {
			hDevice = (UINT32)(&gDevEmbMem5);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM6) {
			hDevice = (UINT32)(&gDevEmbMem6);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM7) {
			hDevice = (UINT32)(&gDevEmbMem7);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_EMBMEM8) {
			hDevice = (UINT32)(&gDevEmbMem8);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_CARD1) {
			hDevice = (UINT32)(&gDevCARD1);
		} else if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_CARD3) {
			hDevice = (UINT32)(&gDevCARD2);
		}
		break;
	case DX_CLASS_USB_EXT:
		hDevice = (UINT32)(&gDevUSB);
		break;
	case DX_CLASS_POWER_EXT:
		#if defined(_CLOCK_RTC_)
		hDevice = (UINT32)(&gDevPowerBATT);
		#elif defined(_CLOCK_DRTC_)
		hDevice = (UINT32)(&gDevPowerDC);
		#else
		#warning Unknown CLOCK?
		#warning Please assign your "CLOCK" in include "ModelConfig.txt"
		#error (see above)
		#endif
		break;
	case DX_CLASS_INPUT_EXT:
		break;
	case DX_CLASS_STATUS_EXT:
		if ((DxClassType & DX_TYPE_MASK) == DX_TYPE_OUTLED) {
			hDevice = (UINT32)(&gDevLED);
		}
		break;
	}

	if (hDevice == 0) {
		DBG_ERR("[Dx] GetObject %08x failed\r\n", DxClassType);
	}
	return (DX_HANDLE)hDevice;
}

UINT8 *Dx_GetModelExtCfg(MODELEXT_TYPE type, MODELEXT_HEADER **header)
{
#if defined(_MODELEXT_BUILT_IN_ON_)
#define LABEL_NAME(_name_) _name_
	extern char LABEL_NAME(_section_modelext_info_addr)[];
	extern char LABEL_NAME(_section_modelext_info_size)[];
	static BOOL bInit = FALSE;
	if (!bInit) {
		MODELEXT_HEADER *header;
		MODELEXT_INFO *pData;
		pData = (MODELEXT_INFO *)modelext_get_cfg((unsigned char *)_section_modelext_info_addr, MODELEXT_TYPE_INFO, &header);
		if (pData == NULL) {
			DBG_FATAL("MODELEXT_INFO is null, call by = 0x%08X\n", __CALL__ - 8);
			return NULL;
		}
		pData->ext_bin_length = (UINT32)_section_modelext_info_size;
		bInit = TRUE;
	}
	UINT8 *p_modelext = (UINT8 *)modelext_get_cfg((unsigned char *)_section_modelext_info_addr, type, header); //_TODO: VOS need nvt_mem converting address
#else
	UINT8 *p_modelext = (UINT8 *)modelext_get_cfg((unsigned char *)_BOARD_IPC_ADDR_, type, header); //_TODO: VOS need nvt_mem converting address
#endif

	if (p_modelext == NULL || *header == NULL) {
		DBG_FATAL("MODELEXT_TYPE = %d is null, call by = 0x%08X\n", type, __CALL__ - 8);
		return NULL;
	}

	return p_modelext;
}

void Install_DrvExt(void)
{

}

