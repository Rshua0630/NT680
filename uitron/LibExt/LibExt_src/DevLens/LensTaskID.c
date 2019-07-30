/**
 * Novatek lens task install.
 * 
 * Lens task install.
 * 
 * @file LensTaskID.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */
 
#include "SysKer.h"
#include "LensAPI.h"


#define PRI_LENS_CTRL              10    // must be the same with PRI_ISPSUDO
#define STKSIZE_LENS_CTRL          3072
#define PRI_LENS_FOCUS_CTRL        10    // must be the same with PRI_ISPSUDO
#define STKSIZE_LENS_FOCUS_CTRL    1024


UINT32 g_uiLensTaskID = 0, g_uiLensFocusTaskID = 0, g_uiLensFlagID = 0, g_uiLensFocusFlagID = 0;
UINT32 g_uiLensCtrlSemaID = LENS_SEMAID_INIT_VALUE;


void LensCtrl_InstallID(void)
{
    OS_CONFIG_TASK(g_uiLensTaskID, PRI_LENS_CTRL, STKSIZE_LENS_CTRL, LensCtrl_Tsk);
    OS_CONFIG_TASK(g_uiLensFocusTaskID, PRI_LENS_FOCUS_CTRL, STKSIZE_LENS_FOCUS_CTRL, LensFocusCtrl_Tsk);

    OS_CONFIG_FLAG(g_uiLensFlagID);
    OS_CONFIG_FLAG(g_uiLensFocusFlagID);

    OS_CONFIG_SEMPHORE(g_uiLensCtrlSemaID, 0, 1, 1);
}


