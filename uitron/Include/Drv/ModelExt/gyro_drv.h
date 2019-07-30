/**
    Gyro Driver Header file.

    @file       gyro_drv.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _GYRO_DRV_H
#define _GYRO_DRV_H

#include "Type.h"
#include "spi.h"

typedef enum _GYRO_SENSOR_ID {
	GYRO_SENSOR_ID_1 = 0,              ///< gyro sensor id 0(SPI3
	GYRO_SENSOR_ID_2,                  ///< gyro sensor id 1(SPI1)
	GYRO_SENSOR_ID_MAX_NUM,
	ENUM_DUMMY4WORD(GYRO_SENSOR_ID)
} GYRO_SENSOR_ID;

typedef enum {
	GYRO_NOTIFY_SYNC_END = 0,          ///< End of one SIE sync. All data in this sync are completed.
	GYRO_NOTIFY_CHANGE_END,            ///< End of SIE VD after spi_changeGyro() is invoked. (Next VD will be new gyro setting)
	GYRO_NOTIFY_LAST_TRS,              ///< End of last transfer in a VD period
	GYRO_NOTIFY_MAX_NUM,
	ENUM_DUMMY4WORD(GYRO_NOTIFY_TYPE)
} GYRO_NOTIFY_TYPE;

typedef void (*FP_GYRO_CB)(GYRO_SENSOR_ID, GYRO_NOTIFY_TYPE);

#endif// _GYRO_DRV_H

