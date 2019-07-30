/**
    ipl ext dx cam

    extend library common interface

    @file       ipl_ext_dx_cam.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

//#include "ipl_ext_lib.h"
#include "ipl_obj.h"
#include "ipl_obj_drv.h"

#define __MODULE__          DxIPLExt
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#include "DebugModule.h"

typedef IPL_EXT_LIB_TAB *(*IPL_GET_EXT_LIB)(void);

typedef struct _IPL_EXT_LIB_ITEM
{
	CHAR name[30];
	IPL_GET_EXT_LIB get_fp;
} IPL_EXT_LIB_ITEM;

static IPL_EXT_LIB_ITEM ipl_ext_lib_tab[IPL_MAX] = {
#if (!defined(_IPL1_IPL_FAKE_))
	{_IPL1_, _IPL1_EXT_LIB_TAB_},
#else
	{_IPL1_, NULL},
#endif

#if (!defined(_IPL2_IPL_FAKE_))
	{_IPL2_, _IPL2_EXT_LIB_TAB_},
#else
	{_IPL2_, NULL},
#endif

#if (!defined(_IPL3_IPL_FAKE_))
	{_IPL3_, _IPL3_EXT_LIB_TAB_},
#else
	{_IPL3_, NULL},
#endif

#if (!defined(_IPL4_IPL_FAKE_))
	{_IPL4_, _IPL4_EXT_LIB_TAB_},
#else
	{_IPL4_, NULL},
#endif

#if (!defined(_IPL5_IPL_FAKE_))
	{_IPL5_, _IPL5_EXT_LIB_TAB_},
#else
	{_IPL5_, NULL},
#endif

#if (!defined(_IPL6_IPL_FAKE_))
	{_IPL6_, _IPL6_EXT_LIB_TAB_},
#else
	{_IPL6_, NULL},
#endif

#if (!defined(_IPL7_IPL_FAKE_))
	{_IPL7_, _IPL7_EXT_LIB_TAB_},
#else
	{_IPL7_, NULL},
#endif

#if (!defined(_IPL8_IPL_FAKE_))
	{_IPL8_, _IPL8_EXT_LIB_TAB_},
#else
	{_IPL8_, NULL},
#endif

};


static IPL_EXT_LIB_ITEM ipl_ext_ext_lib_tab[] = {
#if (!defined(_IPL_EXT1_IPL_FAKE_))
	{_IPL_EXT1_, _IPL_EXT1_EXT_LIB_TAB_},
#else
	{_IPL_EXT1_, NULL},
#endif

#if (!defined(_IPL_EXT2_IPL_FAKE_))
	{_IPL_EXT2_, _IPL_EXT2_EXT_LIB_TAB_},
#else
	{_IPL_EXT2_, NULL},
#endif

#if (!defined(_IPL_EXT3_IPL_FAKE_))
	{_IPL_EXT3_, _IPL_EXT3_EXT_LIB_TAB_},
#else
	{_IPL_EXT3_, NULL},
#endif

#if (!defined(_IPL_EXT4_IPL_FAKE_))
	{_IPL_EXT4_, _IPL_EXT4_EXT_LIB_TAB_},
#else
	{_IPL_EXT4_, NULL},
#endif

	{"END", NULL},
};

#if 0
#endif
/************************************************************************/
/*                      common function                                 */
/************************************************************************/
static IPL_EXT_LIB_ITEM *ipl_ext_ext_lib_search(CHAR *search_name)
{
	UINT32 i;
	IPL_EXT_LIB_ITEM *ipl_ext_info;

	DBG_IND("search ipl ext name = %s\r\n", search_name);
	ipl_ext_info = ipl_ext_ext_lib_tab;

	for (i = 0; (strcmp(ipl_ext_info[i].name, "END") != 0); i ++) {
		if (strcmp(ipl_ext_info[i].name, search_name) == 0) {
			return &ipl_ext_info[i];
		}
	}
	return NULL;
}

BOOL ipl_ext_lib_repalce_lib(UINT32 id, CHAR *specified_name)
{
	IPL_EXT_LIB_ITEM *item;

	if (id >= IPL_MAX) {
		DBG_ERR("input parameter error (id = %d\r\n)", id);
		return FALSE;
	}
	DBG_IND("id = %d, name = %s\r\n", id, specified_name);

	item = ipl_ext_ext_lib_search(specified_name);
	if (item == NULL) {
		DBG_ERR("ipl ext lib search fail %s\r\n", specified_name);
		return FALSE;
	}

	ipl_ext_lib_tab[id] = *item;
	return TRUE;
}

BOOL ipl_ext_lib_init(UINT32 id, IPL_HANDLE *handle)
{
	if ((id >= IPL_MAX) || (handle == NULL)) {
		DBG_ERR("input parameter error (id = %d, handle = 0x%.8x\r\n)", id, handle);
		return FALSE;
	}

	if (strcmp(ipl_ext_lib_tab[id].name, "IPL_FAKE") != 0) {

		handle->ext_lib_tab = ipl_ext_lib_tab[id].get_fp();
		return TRUE;
	}

	return FALSE;
}

CHAR* ipl_ext_lib_get_name(UINT32 id)
{
	return ipl_ext_lib_tab[id].name;
}