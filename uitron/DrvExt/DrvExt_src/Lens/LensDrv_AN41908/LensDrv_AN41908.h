/**
 * Novatek PANASONIC AN41908 lens driver header.
 * 
 * Reference AN41908 lens driver header.
 * 
 * @file LensDrv_AN41908.h
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _LENSDRV_AN41908_H_
#define _LENSDRV_AN41908_H_

#include "Type.h"
#include "Lens_AN41908.h"


//=============================================================================
// constant define
//=============================================================================


//=============================================================================
// struct & flag definition
//=============================================================================
typedef struct
{
    LENS_TAB LensDrv;

    LENSCTRL_TASK_API LensCtrlTsk;

    UINT32 uiFocalLengthMin;    // unit: um
    UINT32 uiFocalLengthMax;    // unit: um

    UINT32 uiZoomSecPos;
    INT32 iFocusCaliOfst[ZOOM_SECTION_NUM];
    IRIS_POS eAperturePos;
    MOTOR_SHUTTER_ACT eShutterState;
    MOTOR_IRCUT_ACT eIrCutState;
} LENSDRV_INFO;


//=============================================================================
// lens driver parameters
//=============================================================================
const UINT32 g_uiFNumberMapping[] = {18, 20, 28, 40, 56, 80};

const UINT32 g_uiFocusDistanceTbl[FOCUS_DISTANCE_TABLE_NUM] = {5, 10, 15, 20, 30, 50, 70, 100, 140, 200, 300, 9999};    // unit: cm


#endif    // _LENSDRV_AN41908_H_


