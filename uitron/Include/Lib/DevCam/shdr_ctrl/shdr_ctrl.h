/*
    Sensor HDR Ctrl

    This file is the API of the SHDR control.

    @file       SHDRCtrl.h
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _SENSOR_HDR_CTRL_H_
#define _SENSOR_HDR_CTRL_H_

#include "SensorHDRLib.h"

extern ER SHDRCtrl_Reset(UINT32 ipl_id);
extern ER SHDRCtrl_Init(UINT32 ipl_id, SHDR_OPEN_INFO *pHdrOpenInfo);
extern UINT32 SHDRCtrl_GetParam(UINT32 ipl_id, INT32 frame_idx);
extern UINT32 SHDRCtrl_GetUpdateBits(void);
ER SHDRCtrl_Proc(UINT32 ipl_id, SHDR_CAL_INFO *pHdrCalInfo, UINT32 frameIdx);

#endif  //_SENSOR_HDR_CTRL_H_

