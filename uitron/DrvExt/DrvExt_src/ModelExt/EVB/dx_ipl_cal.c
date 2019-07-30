/**
    ipl cal ext dx cam

    ipl calibration extend common interface

    @file       dx_ipl_cal.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#include "ipl_cal_obj.h"
#include "ipl_cal_obj_drv.h"

#define __MODULE__          DxIPLCALExt
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#include "DebugModule.h"

typedef IPL_CAL_EXT_LIB_TAB *(*IPL_CAL_GET_EXT_LIB)(void);

static CHAR ipl_cal_name[IPL_CAL_MAX][30] = {
	_IPL_CAL1_,
	_IPL_CAL2_,
	_IPL_CAL3_,
	_IPL_CAL4_,
	_IPL_CAL5_,
	_IPL_CAL6_,
	_IPL_CAL7_,
	_IPL_CAL8_,
};

static IPL_CAL_GET_EXT_LIB ipl_cal_ext_lib_tab[IPL_CAL_MAX] = {

#if (!defined(_IPL_CAL1_IPL_CAL_FAKE_))
	_IPL_CAL1_EXT_LIB_TAB_,
#else
	NULL,
#endif

#if (!defined(_IPL_CAL2_IPL_CAL_FAKE_))
	_IPL_CAL2_EXT_LIB_TAB_,
#else
	NULL,
#endif

#if (!defined(_IPL_CAL3_IPL_CAL_FAKE_))
	_IPL_CAL3_EXT_LIB_TAB_,
#else
	NULL,
#endif

#if (!defined(_IPL_CAL4_IPL_CAL_FAKE_))
	_IPL_CAL4_EXT_LIB_TAB_,
#else
	NULL,
#endif

#if (!defined(_IPL_CAL5_IPL_CAL_FAKE_))
	_IPL_CAL5_EXT_LIB_TAB_,
#else
	NULL,
#endif

#if (!defined(_IPL_CAL6_IPL_CAL_FAKE_))
	_IPL_CAL6_EXT_LIB_TAB_,
#else
	NULL,
#endif

#if (!defined(_IPL_CAL7_IPL_CAL_FAKE_))
	_IPL_CAL7_EXT_LIB_TAB_,
#else
	NULL,
#endif

#if (!defined(_IPL_CAL8_IPL_CAL_FAKE_))
	_IPL_CAL8_EXT_LIB_TAB_,
#else
	NULL,
#endif
};

#if 0
#endif
/************************************************************************/
/*                      common function                                 */
/************************************************************************/
BOOL ipl_cal_ext_lib_init(UINT32 id, IPL_CAL_HANDLE *handle)
{
	if ((id >= IPL_CAL_MAX) || (handle == NULL)) {
		DBG_ERR("input parameter error (id = %d, handle = 0x%.8x\r\n)", id, handle);
		return FALSE;
	}

	if (strcmp(ipl_cal_name[id], "IPL_CAL_FAKE") != 0) {
		handle->cal_ext_lib_tab = ipl_cal_ext_lib_tab[id]();
		return TRUE;
	}

	return FALSE;
}
