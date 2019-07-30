/**
 * Novatek Lens API function header.
 * 
 * Lens API header.
 * 
 * @file LensAPI.h
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _LENS_API_H_
#define _LENS_API_H_

#include "Lens.h"


#define FLGLENS_ZOOM_SETSTATE           FLGPTN_BIT(0)     ///< set zoom state
#define FLGLENS_FOCUS_SETSTATE          FLGPTN_BIT(1)     ///< set focus state
#define FLGLENS_APERTURE_SETSTATE       FLGPTN_BIT(2)     ///< set aperture state
#define FLGLENS_SHUTTER_SETSTATE        FLGPTN_BIT(3)     ///< set shutter state
#define FLGLENS_IRCUT_SETSTATE          FLGPTN_BIT(4)     ///< set IRCUT state
#define FLGLENS_IDLE                    FLGPTN_BIT(31)    ///< LensCtrlTsk is idle
#define FLGLENS_ALL                     0xFFFFFFFF        ///< flag mask

#define LENS_SEMAID_INIT_VALUE          0xFFFFFFFF


extern ER LensCtrl_Open(PLENSCTRL_OBJ plens_ctrl);
extern ER LensCtrl_Close(void);
extern void LensCtrl_Tsk(void);
extern void LensFocusCtrl_Tsk(void);


extern UINT32 _SECTION(".kercfg_data") g_uiLensTaskID;
extern UINT32 _SECTION(".kercfg_data") g_uiLensFlagID;
extern UINT32 _SECTION(".kercfg_data") g_uiLensFocusTaskID;
extern UINT32 _SECTION(".kercfg_data") g_uiLensFocusFlagID;

extern UINT32 _SECTION(".kercfg_data") g_uiLensCtrlSemaID;


extern void Lens_InstallCmd(void);


#endif    // _LENS_API_H_


