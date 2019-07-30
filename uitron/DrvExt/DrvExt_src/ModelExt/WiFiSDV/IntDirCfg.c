#include "IntDirCfg.h"
#include "interrupt.h"
#include "DrvExt.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

void IO_InitIntDir(void)
{
	MODELEXT_HEADER *header;
	INT_GIC_DST  *uiIntDirTbl;

	uiIntDirTbl = (INT_GIC_DST *)Dx_GetModelExtCfg(MODELEXT_TYPE_INTDIR_CFG, &header);
	if (!uiIntDirTbl || !header) {
		DBG_FATAL("Modelext: Interrupt direction setting is null\n");
		return;
	}


	int_setConfig(INT_CONFIG_ID_GIC_DESTINATION, (UINT32)uiIntDirTbl);
}


