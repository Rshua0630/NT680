#ifndef _DXSENSOR_H_
#define _DXSENSOR_H_
/**
    DxSensor.h

    Sensor common interface
    @file       DxSensor.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "sensor.h"
#include "sensor_if.h"

extern BOOL drv_sensor_repalce_drv(SENSOR_ID id, CHAR *specified_name);
extern ER drv_sensor_group_cfg(SENSOR_MULTI_ID* sensor_gp, SENSOR_MULTI_ID* sensor_subgp); // sensor_gp & sensor_subgp table size: SENSOR_ID_MAX_NUM
extern BOOL drv_sensor_first_cfg(void);
extern BOOL drv_sensor_init(SENSOR_ID id, SENSOR_HANDLE *handle);
extern void drv_sensor_power_turn_on(SENSOR_ID id);
extern void drv_sensor_power_turn_off(SENSOR_ID id);
extern void drv_sensor_power_save_off(SENSOR_ID id);
extern void drv_sensor_power_save_on(SENSOR_ID id);
extern BOOL drv_sensor_det_plug_in(SENSOR_ID id);
extern BOOL drv_sensor_plug_in_cb(SENSOR_ID id);

#if _FPGA_EMULATION_
extern void tc680_init(void);
extern ER   tc680_writeReg(UINT32 uiOffset, UINT32 uiValue);
extern ER   tc680_readReg(UINT32 uiOffset, UINT32 *puiValue);
extern void tc680_dump_all(void);
#endif

#endif //_DXSENSOR_H_

