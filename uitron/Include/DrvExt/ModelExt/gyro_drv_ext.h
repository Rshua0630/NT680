/**
    Gyro Driver Header file.

    @file       gyro_drv_ext.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _GYRO_DRV_EXT_H
#define _GYRO_DRV_EXT_H

#include "Type.h"
#include "spi.h"
#include "gyro_drv.h"

typedef enum {
	GYRO_OP_MODE_SINGLE_ACCESS,       ///< gyro in single access mode, read/write register one by one
	GYRO_OP_MODE_FREE_RUN,            ///< gyro in free run mode, read register after periodic trigger automatically
	ENUM_DUMMY4WORD(GYRO_OP_MODE)
} GYRO_OP_MODE;


typedef enum {
	GYRO_FREERUN_SIE_SYNC,             ///< SIE sync mode
	GYRO_FREERUN_FWTRIGGER,            ///< FW trigger mode
	ENUM_DUMMY4WORD(GYRO_FREERUN_TRIGGER_MODE)
} GYRO_FREERUN_TRIGGER_MODE;

typedef struct GYRO_FREE_RUN_PARAM {
	UINT32                        uiPeriodUs;    ///< free run period in Us
	UINT32                        uiDataNum;     ///< gyro data number in a period
	UINT32                        uiTriggerIdx;  ///< the trigger index of gyro free run mode
	GYRO_FREERUN_TRIGGER_MODE     triggerMode;   ///< trigger mode
} GYRO_FREE_RUN_PARAM;

typedef struct _GYRO_CFGINFO {
	UINT32            AxisSelec[3];              ///< Gyro data axis
	UINT32            DirSelec[3];               ///< Gyro data direction
} GYRO_CFGINFO;

typedef struct GYRO_OPENOBJ {
	SPI_VD_SRC vd_src;
	FP_GYRO_CB gyro_cb; ///< gyro callback function
} GYRO_OPENOBJ;

typedef struct GYRO_STARTOBJ {
	SPI_ID SpiID;
	SPI_GYRO_INFO *pSPIGyroInfo;
} GYRO_STARTOBJ;

typedef struct GYRO_RSCINFOINIT {
	UINT32            gdFormat;       ///< Gyro data format
	UINT32            AxisSelec[3];   ///< Gyro data axis
	UINT32            DirSelec[3];    ///< Gyro data direction
	UINT32            uiDataUnit;     ///< Gyro data unit, in degree/65536sec
	UINT32            uiCalGain[3];   ///< calibration gain, in 1/1024, recommended fixed to 1024
	INT32             iCalOfs[3];     ///< Calibration offset, recommended fixed to 0
} GYRO_RSCINFOINIT;

extern void gyro_cfg(GYRO_SENSOR_ID gID, GYRO_CFGINFO *cfgInfo) _SECTION(".kercfg_text");
extern ER gyro_open(GYRO_SENSOR_ID gID, GYRO_OPENOBJ *openObj);
extern ER gyro_close(GYRO_SENSOR_ID gID);
extern ER gyro_shutdown(void);
extern ER gyro_readReg(GYRO_SENSOR_ID gID, UINT32 uiAddr, UINT32 *puiData);
extern ER gyro_writeReg(GYRO_SENSOR_ID gID, UINT32 uiAddr, UINT32 uiData);
extern ER gyro_setFreeRunParam(GYRO_SENSOR_ID gID, GYRO_FREE_RUN_PARAM *frParam);
extern ER gyro_setMode(GYRO_SENSOR_ID gID, GYRO_OP_MODE opMode);
extern ER gyro_getSingleData(GYRO_SENSOR_ID gID, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ);
extern ER gyro_getFreeRunData(GYRO_SENSOR_ID gID, UINT32 *puiNum, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ);
extern void gyro_setCalZeroOffset(GYRO_SENSOR_ID gID, INT32 xOffset, INT32 yOffset, INT32 zOffset);

extern void gyro_FreeRunStart(GYRO_SENSOR_ID gID, GYRO_STARTOBJ *pgyroobj);
extern void gyro_RSCInfoInit(GYRO_SENSOR_ID gID, GYRO_RSCINFOINIT *pgyrorscinit);
extern UINT32 gyro_getLSB(GYRO_SENSOR_ID gID);

extern ER   gyro_getGsData(GYRO_SENSOR_ID gID, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ);  // get Gsensor data

#endif// _GYRO_DRV_EXT_H

