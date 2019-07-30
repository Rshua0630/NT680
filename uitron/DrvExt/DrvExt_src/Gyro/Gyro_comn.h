/*
    Gyro common header

    @file       Gyro_comn.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _GYRO_COMN_H
#define _GYRO_COMN_H


#include "Type.h"

#define GDI2_GB_NUM             (32)
#define DATA_SIZE               (5)


typedef enum {
	GYRO_IDLE   = 0,        ///< idle
	GYRO_READY  = 1,        ///< ready for read/write
	GYRO_RUN    = 2,        ///< running
	ENUM_DUMMY4WORD(GYRO_STATUS)
} GYRO_STATUS;

typedef struct {
	UINT32  uiAddress;                      // Panel Register Address
	UINT32  uiValue;                        // Panel Register config value
} tGYRO_CMD;

typedef struct GYRO_INNEROBJ {
	SPI_ID          spiID;
	FP_GYRO_CB      gyroCB;        ///< gyro callback function
	SPI_VD_SRC      vdSrc;
	GYRO_OP_MODE    gyroOpMode;
	GYRO_STATUS     gyroStatus;
	UINT32          frPeriodUs;    ///< free run period in Us
	UINT32          frDataNum;     ///< gyro data number in a period
	UINT32          frTriggerIdx;  ///< the trigger index of gyro free run mode
	GYRO_FREERUN_TRIGGER_MODE     frTriggerMode;   ///< trigger mode
	UINT32          readFrequency;
	UINT32          writeFrequency;
	INT32           xOffset;
	INT32           yOffset;
	INT32           zOffset;
	UINT32          gyroBufInIdx;
	UINT32          gyroBufOutIdx;
	UINT32          gyroQueueCount;
	union {
		struct type1 {
			UINT32          gyro[GDI2_GB_NUM][DATA_SIZE];
			UINT32          accel[GDI2_GB_NUM][DATA_SIZE];
		} type1;
		struct type2 {
			UINT32          gyroX[GDI2_GB_NUM][DATA_SIZE];
			UINT32          gyroY[GDI2_GB_NUM][DATA_SIZE];
		} type3;
	} buf;
} GYRO_INNEROBJ;

#endif

