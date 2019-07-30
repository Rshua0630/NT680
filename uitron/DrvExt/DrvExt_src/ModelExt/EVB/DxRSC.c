/**
    rsc ext dx cam

    extend library common interface

    @file       DxRSC.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/


#include "rsc_obj.h"
#include "rsc_obj_drv.h"

#define __MODULE__          DxRSCExt
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#include "DebugModule.h"

typedef RSC_EXT_LIB_TAB *(*RSC_GET_EXT_LIB)(void);

static CHAR rsc_name[30] = {
	_RSC_,
};

static RSC_GET_EXT_LIB rsc_ext_lib_tab = {

#if (!defined(_RSC_RSC_FAKE_))
	_RSC_EXT_LIB_TAB_,
#else
	NULL,
#endif

};

#if 0
#endif
/************************************************************************/
/*                      common function                                 */
/************************************************************************/
BOOL rsc_ext_lib_init(UINT32 id, RSC_HANDLE *handle)
{
	if ((id > RSC_SURRPORT_ID) || (handle == NULL)) {
		DBG_ERR("input parameter error (id = %d, handle = 0x%.8x\r\n)", id, handle);
		return FALSE;
	}

	if (strcmp(rsc_name, "RSC_FAKE") != 0) {
		handle->ext_lib_tab = rsc_ext_lib_tab();
		return TRUE;
	}

	return FALSE;
}