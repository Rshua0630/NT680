#include "PinmuxCfg.h"
#include "top.h"
#include "DrvExt.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

void IO_InitPinmux(void)
{
	MODELEXT_HEADER *header;
	PIN_GROUP_CONFIG *vTopConfig;

	vTopConfig = (PIN_GROUP_CONFIG *)Dx_GetModelExtCfg(MODELEXT_TYPE_PINMUX_CFG, &header);
	if (!vTopConfig || !header) {
		DBG_FATAL("Modelext: pinmux is null\n");
		return;
	}

#if defined(_CPU2_LINUX_)
	pinmux_init_for_dualcore((PIN_GROUP_CONFIG *)vTopConfig, TRUE);
#else
	pinmux_init((PIN_GROUP_CONFIG *)vTopConfig);
#endif

}

