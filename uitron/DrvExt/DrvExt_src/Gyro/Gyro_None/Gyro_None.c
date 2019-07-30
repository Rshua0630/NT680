/*
    Gyro Driver InvenSense IDG2020.

    @file       GyroDrv_IDG2020.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#include <stdio.h>
#include <string.h>
#include "Debug.h"
#include "Type.h"
#include "ErrorNo.h"

#include "Debug.h"
#include "cache.h"
#include "dma.h"
#include "gyro_drv_ext.h"
#include "spi.h"
#include "SysKer.h"
#include "Delay.h"

#define THIS_DBGLVL         6   // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          GyroDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
void gyro_cfg(GYRO_SENSOR_ID gID, GYRO_CFGINFO *cfgInfo)
{
}

ER gyro_open(GYRO_SENSOR_ID gID, GYRO_OPENOBJ *openObj)
{
	return E_SYS;
}


ER gyro_close(GYRO_SENSOR_ID gID)
{
	return E_OK;
}

ER gyro_shutdown(void)
{
	return E_OK;
}

ER gyro_readReg(GYRO_SENSOR_ID gID, UINT32 uiAddr, UINT32 *puiData)
{
	return E_OK;
}

ER gyro_writeReg(GYRO_SENSOR_ID gID, UINT32 uiAddr, UINT32 uiData)
{
	return E_OK;
}


ER gyro_setFreeRunParam(GYRO_SENSOR_ID gID, GYRO_FREE_RUN_PARAM *frParam)
{
	return E_OK;
}


ER gyro_setMode(GYRO_SENSOR_ID gID, GYRO_OP_MODE opMode)
{
	return E_OK;
}


ER gyro_getFreeRunData(GYRO_SENSOR_ID gID, UINT32 *puiNum, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
	return E_OK;
}

void gyro_setCalZeroOffset(GYRO_SENSOR_ID gID, INT32 xOffset, INT32 yOffset, INT32 zOffset)
{
}

ER gyro_getSingleData(GYRO_SENSOR_ID gID, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
	return E_OK;
}

UINT32 gyro_getLSB(GYRO_SENSOR_ID gID)
{
	return 50;
}

void gyro_RSCInfoInit(GYRO_SENSOR_ID gID, GYRO_RSCINFOINIT *pRSCInfoInfo)
{

}

ER gyro_getGsData(GYRO_SENSOR_ID gID, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
	return E_OK;
}


