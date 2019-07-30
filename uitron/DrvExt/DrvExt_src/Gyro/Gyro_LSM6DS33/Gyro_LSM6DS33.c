/*
    Gyro Driver InvenSense LSM6DS33.

    @file       GyroDrv_LSM6DS33.c
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "ErrorNo.h"
#include "gyro_drv_ext.h"
#include "spi.h"
#include "Delay.h"
#include "time.h"
#include "Gyro_comn.h"

#define THIS_DBGLVL         2   // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          GyroDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define MAX_GYRO_NUM            (2)


static GYRO_CFGINFO  _SECTION(".kercfg_data") g_GyroCfgInfo[MAX_GYRO_NUM] = {{
		0, //RSC_GYRO_AXIS_X;
		1, //RSC_GYRO_AXIS_Y;
		2, //RSC_GYRO_AXIS_Z;
		0, //RSC_DIR_POS
		0, //RSC_DIR_POS
		0
	}, {
		0, //RSC_GYRO_AXIS_X;
		1, //RSC_GYRO_AXIS_Y;
		2, //RSC_GYRO_AXIS_Z;
		0, //RSC_DIR_POS
		0, //RSC_DIR_POS
		0
	}
};//RSC_DIR_POS

static GYRO_INNEROBJ gyroObj[MAX_GYRO_NUM];



#define REG_ACCEL_XOUT_L            (0x28)
#define REG_ACCEL_XOUT_H            (0x29)
#define REG_ACCEL_YOUT_L            (0x2A)
#define REG_ACCEL_YOUT_H            (0x2B)
#define REG_ACCEL_ZOUT_L            (0x2C)
#define REG_ACCEL_ZOUT_H            (0x2D)

#define REG_GYRO_XOUT_L             (0x22)
#define REG_GYRO_XOUT_H             (0x23)
#define REG_GYRO_YOUT_L             (0x24)
#define REG_GYRO_YOUT_H             (0x25)
#define REG_GYRO_ZOUT_L             (0x26)
#define REG_GYRO_ZOUT_H             (0x27)


#define REG_WHO_AM_I                (0x0F)
#define REG_WRITE                   (0x00)
#define REG_READ                    (0x80)

#define REG_DELAY                   (0xFF)

#define GYRO_ID                     (0x69)

tGYRO_CMD cmd_gyro_init[] = {
	0x12,    0x05,      // Reset
	0xFF,    0x01,      // delay 1 ms
	0x10,    0x37,      // Accelerometer Configuration +-16g, 50Hz
	0x11,    0x80,      // Gyro Configuration        245 dps, 1.66KHz
	0x18,    0x38,      // Enable Accelerometer output
	0x19,    0x38,      // Enable Gyro output


};

void gyro_cfg(GYRO_SENSOR_ID gID, GYRO_CFGINFO *pCfgInfo)
{
	memcpy((void *)&g_GyroCfgInfo[gID], (void *)pCfgInfo, sizeof(GYRO_CFGINFO));
	DBG_IND("Axis= %d, %d, %d\r\n", g_GyroCfgInfo[gID].AxisSelec[0], g_GyroCfgInfo[gID].AxisSelec[1], g_GyroCfgInfo[gID].AxisSelec[2]);
	DBG_IND("Dir= %d, %d, %d\r\n", g_GyroCfgInfo[gID].DirSelec[0], g_GyroCfgInfo[gID].DirSelec[1], g_GyroCfgInfo[gID].DirSelec[2]);
}

ER gyro_open(GYRO_SENSOR_ID gID, GYRO_OPENOBJ *openObj)
{

	DBG_IND("openObj =0x%x!!\r\n", openObj);

	if (!openObj) {
		DBG_ERR("no openObj\r\n");
		return E_PAR;
	}

	// state check: only while idle
	if (gyroObj[gID].gyroStatus != GYRO_IDLE) {
		//DBG_WRN("already opened , gyroObj[gID].gyroStatus=%d!!\r\n", gyroObj[gID].gyroStatus);
		return E_OK;
	}

	memset((void *)&gyroObj[gID], 0, sizeof(GYRO_INNEROBJ));

	gyroObj[gID].readFrequency = 1000000; // 1 MHz for read
	gyroObj[gID].writeFrequency = 1000000; // 1 MHz for write

	if (gID == GYRO_SENSOR_ID_1) {
		gyroObj[gID].spiID = SPI_ID_3;
	} else {
		gyroObj[gID].spiID = SPI_ID_1;
	}

	// parameter check
	if (openObj->gyro_cb) {
		gyroObj[gID].gyroCB = openObj->gyro_cb;
	}

	gyroObj[gID].vdSrc = openObj->vd_src;

	// power on gyro
	// always power-on

	// initializa gyro
	{
		UINT32 recv1 = 0;

		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_ENG_GYRO_UNIT, FALSE);  // normal mode

		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_FREQ, gyroObj[gID].readFrequency);

		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_BUSMODE, SPI_MODE_3);
		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_MSB_LSB, SPI_MSB);

		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_VD_SRC, gyroObj[gID].vdSrc);
		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_CS_ACT_LEVEL, SPI_CS_ACT_LEVEL_LOW);
		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_CS_CK_DLY, 1);

		// Read gyro ID
		spi_open(gyroObj[gID].spiID);
		spi_setCSActive(gyroObj[gID].spiID, TRUE);
		spi_setTransferLen(gyroObj[gID].spiID, SPI_TRANSFER_LEN_2BYTES);
		spi_writeReadSingle(gyroObj[gID].spiID, (REG_READ | REG_WHO_AM_I) << 8, &recv1);
		spi_setCSActive(gyroObj[gID].spiID, FALSE);
		spi_close(gyroObj[gID].spiID);

		if ((recv1 & 0xFF) != GYRO_ID) {
			DBG_ERR("recv1 = 0x%x\r\n", recv1);
			return E_SYS;
		}
	}
	// set initial value
	{
		UINT32 i;
		spi_open(gyroObj[gID].spiID);
		spi_setTransferLen(gyroObj[gID].spiID, SPI_TRANSFER_LEN_1BYTE);
		for (i = 0; i < sizeof(cmd_gyro_init) / sizeof(tGYRO_CMD); i++) {
			if (REG_DELAY == cmd_gyro_init[i].uiAddress) {
				Delay_DelayMs(cmd_gyro_init[i].uiValue);
				continue;
			}
			spi_setCSActive(gyroObj[gID].spiID, TRUE);
			spi_writeSingle(gyroObj[gID].spiID, REG_WRITE | (cmd_gyro_init[i].uiAddress & 0x7f));
			spi_writeSingle(gyroObj[gID].spiID, cmd_gyro_init[i].uiValue);
			spi_setCSActive(gyroObj[gID].spiID, FALSE);
		}
		spi_close(gyroObj[gID].spiID);
	}

	// state change: ready for gyro
	gyroObj[gID].gyroStatus = GYRO_READY;

	return E_OK;
}


ER gyro_close(GYRO_SENSOR_ID gID)
{

	DBG_IND("\r\n");
	// state check: only while idle
	if (gyroObj[gID].gyroOpMode == GYRO_OP_MODE_FREE_RUN) {
		// disable free run
		spi_stopGyro(gyroObj[gID].spiID);
		spi_close(gyroObj[gID].spiID);

		gyroObj[gID].gyroOpMode = GYRO_OP_MODE_SINGLE_ACCESS;
		gyroObj[gID].gyroStatus = GYRO_READY;
	}

	// stop/pause gyro
	// not-yet

	// power off gyro
	// always power-on

	// state change: ready for gyro
	gyroObj[gID].gyroStatus = GYRO_IDLE;

	return E_OK;
}

ER gyro_shutdown(void)
{
	return E_OK;
}

ER gyro_readReg(GYRO_SENSOR_ID gID, UINT32 uiAddr, UINT32 *puiData)
{
	// state check: only while ready
	if (gyroObj[gID].gyroStatus != GYRO_READY) {
		DBG_ERR("GYRO ERR: gyro_readReg, gyroObj[gID].gyroStatus=%d!!\r\n", gyroObj[gID].gyroStatus);
		return E_SYS;
	}

	// state change: ready -> run
	gyroObj[gID].gyroStatus = GYRO_RUN;

	// read register
	//normal job
	{
		UINT32 recv1 = 0;//, recv2;
		spi_open(gyroObj[gID].spiID);

		spi_setCSActive(gyroObj[gID].spiID, TRUE);
		spi_setTransferLen(gyroObj[gID].spiID, SPI_TRANSFER_LEN_1BYTE);

		spi_writeSingle(gyroObj[gID].spiID, (REG_READ) | (uiAddr & 0x7f));
		spi_setTransferLen(gyroObj[gID].spiID, SPI_TRANSFER_LEN_2BYTES);
		spi_writeReadSingle(gyroObj[gID].spiID, 0x00, &recv1);
		spi_setCSActive(gyroObj[gID].spiID, FALSE);
		spi_close(gyroObj[gID].spiID);

		*puiData = (recv1 & 0xFFFF);
	}

	// state change: run -> ready
	gyroObj[gID].gyroStatus = GYRO_READY;

	return E_OK;
}

ER gyro_writeReg(GYRO_SENSOR_ID gID, UINT32 uiAddr, UINT32 uiData)
{
	// state check: only while ready
	if (gyroObj[gID].gyroStatus != GYRO_READY) {
		DBG_ERR("GYRO ERR: gyro_readReg, gyroObj[gID].gyroStatus=%d!!\r\n", gyroObj[gID].gyroStatus);
		return E_SYS;
	}

	// state change: ready -> run
	gyroObj[gID].gyroStatus = GYRO_RUN;

	// write register
	{
		UINT32 recv1;//, recv2;

		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_FREQ, gyroObj[gID].writeFrequency);

		spi_open(gyroObj[gID].spiID);

		spi_setCSActive(gyroObj[gID].spiID, TRUE);
		spi_setTransferLen(gyroObj[gID].spiID, SPI_TRANSFER_LEN_1BYTE);
		spi_writeSingle(gyroObj[gID].spiID, (REG_WRITE) | (uiAddr & 0x7f));
		spi_setTransferLen(gyroObj[gID].spiID, SPI_TRANSFER_LEN_2BYTES);
		spi_writeReadSingle(gyroObj[gID].spiID, uiData, &recv1);
		spi_setCSActive(gyroObj[gID].spiID, FALSE);

		spi_close(gyroObj[gID].spiID);

		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_FREQ, gyroObj[gID].readFrequency);

	}

	// state change: run -> ready
	gyroObj[gID].gyroStatus = GYRO_READY;

	return E_OK;
}


ER gyro_setFreeRunParam(GYRO_SENSOR_ID gID, GYRO_FREE_RUN_PARAM *frParam)
{
	if (!frParam) {
		DBG_ERR("frParam is NULL\r\n");
		return E_PAR;
	}
	if (frParam->uiDataNum > GDI2_GB_NUM) {
		DBG_ERR("uiDataNum over limit %d\r\n", GDI2_GB_NUM);
		return E_PAR;
	}
	if (frParam->uiTriggerIdx >= frParam->uiDataNum) {
		DBG_ERR("uiTriggerIdx %d, uiDataNum  %d\r\n", frParam->uiTriggerIdx, frParam->uiDataNum);
		return E_PAR;
	}
	DBG_IND("uiPeriodUs %d, uiDataNum %d, triggerMode %d, triggerIdx %d\r\n", frParam->uiPeriodUs, frParam->uiDataNum, frParam->triggerMode, frParam->uiTriggerIdx);

	gyroObj[gID].frPeriodUs = frParam->uiPeriodUs;
	gyroObj[gID].frDataNum = frParam->uiDataNum;
	gyroObj[gID].frTriggerIdx = frParam->uiTriggerIdx;
	gyroObj[gID].frTriggerMode = frParam->triggerMode;

	return E_OK;
}
static void gyro_syncEnd(GYRO_INNEROBJ *pGyroObj)
{
	UINT32 i, j, k;
	GYRO_BUF_QUEUE gyroData;
	UINT32 *gyroBufInIdx = &(pGyroObj->gyroBufInIdx);
	UINT32 *gyroQueueCount = &(pGyroObj->gyroQueueCount);


	j = *gyroBufInIdx;

	if (*gyroBufInIdx == 0) {
		*gyroQueueCount = ((pGyroObj->frDataNum - pGyroObj->frTriggerIdx) << 1);
	} else {
		*gyroQueueCount = (pGyroObj->frDataNum << 1);
	}

	for (i = 0; i < *gyroQueueCount; i++) {
		if (spi_getGyroData(pGyroObj->spiID, &gyroData) != E_OK) {
			return;
		}

		k = j % GDI2_GB_NUM;
		if (i % 2 == 0) {
			pGyroObj->buf.type1.gyro[k][0] = gyroData.uiFrameID;
			pGyroObj->buf.type1.gyro[k][1] = gyroData.uiDataID;

			//  x =  byte 1 | byte2 (byte 0 is address, we ignore it)
			pGyroObj->buf.type1.gyro[k][2] = ((gyroData.vRecvWord[0] & 0x0000FF00) >> 8)  | ((gyroData.vRecvWord[0] & 0x00FF0000) >> 8);
			//  y =  byte 3 | byte4
			pGyroObj->buf.type1.gyro[k][3] = ((gyroData.vRecvWord[0] & 0xFF000000) >> 24) | ((gyroData.vRecvWord[1] & 0x000000FF) << 8);
			//  z =  byte 5 | byte6
			pGyroObj->buf.type1.gyro[k][4] = ((gyroData.vRecvWord[1] & 0x0000FF00) >> 8)  | ((gyroData.vRecvWord[1] & 0x00FF0000) >> 8);
		} else {
			pGyroObj->buf.type1.accel[k][0] = gyroData.uiFrameID;
			pGyroObj->buf.type1.accel[k][1] = gyroData.uiDataID;

			//  x =  byte 1 | byte2 (byte 0 is address, we ignore it)
			pGyroObj->buf.type1.accel[k][2] = ((gyroData.vRecvWord[0] & 0x0000FF00) >> 8)  | ((gyroData.vRecvWord[0] & 0x00FF0000) >> 8);
			//  y =  byte 3 | byte4
			pGyroObj->buf.type1.accel[k][3] = ((gyroData.vRecvWord[0] & 0xFF000000) >> 24) | ((gyroData.vRecvWord[1] & 0x000000FF) << 8);
			//  z =  byte 5 | byte6
			pGyroObj->buf.type1.accel[k][4] = ((gyroData.vRecvWord[1] & 0x0000FF00) >> 8)  | ((gyroData.vRecvWord[1] & 0x00FF0000) >> 8);

			//DBG_IND("k = %d, gsX = %d, gsY = %d, gsZ = %d\r\n",k,buf.type1.accel[k][2],buf.type1.accel[k][2],buf.type1.accel[k][4]);
			j++;
		}

	}
	*gyroBufInIdx += pGyroObj->frDataNum;
	DBG_IND("Sync End = %d ms, *gyroQueueCount = %d *gyroBufInIdx %d\r\n", clock() / 1000, *gyroQueueCount, *gyroBufInIdx);

}

static void gyro_HandleGyroEvent_1(SPI_GYRO_INT gyroInt)
{
	GYRO_SENSOR_ID gID = GYRO_SENSOR_ID_1;

	switch (gyroInt) {
	case SPI_GYRO_INT_SYNC_END:
		gyro_syncEnd(&gyroObj[gID]);
		if (gyroObj[gID].gyroCB != NULL) {
			gyroObj[gID].gyroCB(gID, GYRO_NOTIFY_SYNC_END);
		}
		break;
	case SPI_GYRO_INT_CHANGE_END:
		DBG_DUMP("SPI_GYRO_INT_CHANGE_END!!\r\n");
		if (gyroObj[gID].gyroCB != NULL) {
			gyroObj[gID].gyroCB(gID, GYRO_NOTIFY_CHANGE_END);
		}
		break;
	case SPI_GYRO_INT_LAST_TRS:
		if (gyroObj[gID].gyroCB != NULL) {
			gyroObj[gID].gyroCB(gID, GYRO_NOTIFY_LAST_TRS);
		}
		break;
	case SPI_GYRO_INT_OVERRUN:
		DBG_ERR("SPI_GYRO_INT_OVERRUN!!\r\n");
		break;
	case SPI_GYRO_INT_SEQ_ERR:

		DBG_ERR("SPI_GYRO_INT_SEQ_ERR!! %d ms, count=%d\r\n", clock() / 1000, spi_getGyroQueueCount(gyroObj[gID].spiID));
		break;

	case SPI_GYRO_INT_QUEUE_THRESHOLD:
		DBG_ERR("SPI_GYRO_INT_QUEUE_THRESHOLD!!\r\n");
		break;
	case SPI_GYRO_INT_QUEUE_OVERRUN:
		DBG_ERR("SPI_GYRO_INT_QUEUE_OVERRUN!!\r\n");
		break;
	default:
		DBG_ERR("%d\r\n", gyroInt);
		break;
	}
}

static void gyro_HandleGyroEvent_2(SPI_GYRO_INT gyroInt)
{
	GYRO_SENSOR_ID gID = GYRO_SENSOR_ID_2;

	switch (gyroInt) {
	case SPI_GYRO_INT_SYNC_END:
		gyro_syncEnd(&gyroObj[gID]);
		if (gyroObj[gID].gyroCB != NULL) {
			gyroObj[gID].gyroCB(gID, GYRO_NOTIFY_SYNC_END);
		}
		break;
	case SPI_GYRO_INT_CHANGE_END:
		DBG_DUMP("SPI_GYRO_INT_CHANGE_END!!\r\n");
		if (gyroObj[gID].gyroCB != NULL) {
			gyroObj[gID].gyroCB(gID, GYRO_NOTIFY_CHANGE_END);
		}
		break;
	case SPI_GYRO_INT_LAST_TRS:
		if (gyroObj[gID].gyroCB != NULL) {
			gyroObj[gID].gyroCB(gID, GYRO_NOTIFY_LAST_TRS);
		}
		break;
	case SPI_GYRO_INT_OVERRUN:
		DBG_ERR("SPI_GYRO_INT_OVERRUN!!\r\n");
		break;
	case SPI_GYRO_INT_SEQ_ERR:

		DBG_ERR("SPI_GYRO_INT_SEQ_ERR!! %d ms, count=%d\r\n", clock() / 1000, spi_getGyroQueueCount(gyroObj[gID].spiID));
		break;

	case SPI_GYRO_INT_QUEUE_THRESHOLD:
		DBG_ERR("SPI_GYRO_INT_QUEUE_THRESHOLD!!\r\n");
		break;
	case SPI_GYRO_INT_QUEUE_OVERRUN:
		DBG_ERR("SPI_GYRO_INT_QUEUE_OVERRUN!!\r\n");
		break;
	default:
		DBG_ERR("%d\r\n", gyroInt);
		break;
	}
}

static ER gyro_setFreeRun(GYRO_SENSOR_ID gID, BOOL bUpdated)
{
	UINT32 i;
	SPI_GYRO_INFO gyroInfo = {0};

	//
	// 1. Prepare data buffer to store polled gyroscope data
	//
	//useless in new driver//uiDumpCount = 0;
	//useless in new driver//pNextBuf = (UINT32*)uiDumpBufAddr;

	//
	// 2. Start SPI gyro polling
	//
	if (gyroObj[gID].frTriggerMode == GYRO_FREERUN_SIE_SYNC) {
		gyroInfo.gyroMode = SPI_GYRO_MODE_SIE_SYNC;
	} else {
		gyroInfo.gyroMode = SPI_GYRO_MODE_FREE_RUN;
	}

	gyroInfo.uiTransferLen = 2;
	gyroInfo.uiTransferCount = gyroObj[gID].frDataNum;
	gyroInfo.uiOpInterval       = 1;
	// reserve 200 us for last transfer
	gyroInfo.uiTransferInterval = (gyroObj[gID].frPeriodUs - 200) / gyroObj[gID].frDataNum;
	gyroInfo.uiOp0Length = 7;
	DBG_IND("TLen = %d TCount= %d  OpInt =%d, TInt= %d Op0Len=%d\r\n", gyroInfo.uiTransferLen, gyroInfo.uiTransferCount, gyroInfo.uiOpInterval, gyroInfo.uiTransferInterval, gyroInfo.uiOp0Length);
	gyroInfo.vOp0OutData[0] = REG_READ | REG_GYRO_XOUT_L;
	for (i = 1; i < gyroInfo.uiOp0Length; i++) {
		gyroInfo.vOp0OutData[i] = 0;
	}
	gyroInfo.uiOp1Length = 7;
	gyroInfo.vOp1OutData[0] = REG_READ | REG_ACCEL_XOUT_L;
	for (i = 1; i < gyroInfo.uiOp1Length; i++) {
		gyroInfo.vOp1OutData[i] = 0;
	}

	if (gID == GYRO_SENSOR_ID_1) {
		gyroInfo.pEventHandler = gyro_HandleGyroEvent_1;
	} else {
		gyroInfo.pEventHandler = gyro_HandleGyroEvent_2;
	}

	if (bUpdated == FALSE) {
		spi_open(gyroObj[gID].spiID);

		spi_setConfig(gyroObj[gID].spiID, SPI_CONFIG_ID_GYRO_SYNC_END_OFFSET, gyroObj[gID].frTriggerIdx);

		spi_setTransferLen(gyroObj[gID].spiID, SPI_TRANSFER_LEN_1BYTE);

		spi_startGyro(gyroObj[gID].spiID, &gyroInfo);
	} else {
		spi_changeGyro(gyroObj[gID].spiID, &gyroInfo, gyroObj[gID].frTriggerIdx);
	}


	return E_OK;
}

UINT32 gyro_getLSB(GYRO_SENSOR_ID gID)
{
	return 245;
}


void gyro_RSCInfoInit(GYRO_SENSOR_ID gID, GYRO_RSCINFOINIT *pRSCInfoInit)
{
	// please refer to IPL_Ctrl_RSC_Int.h file for the GDFmt/FDDir definition
	pRSCInfoInit->gdFormat = 0;//_INT32
	pRSCInfoInit->AxisSelec[0] = g_GyroCfgInfo[gID].AxisSelec[0];
	pRSCInfoInit->AxisSelec[1] = g_GyroCfgInfo[gID].AxisSelec[1];
	pRSCInfoInit->AxisSelec[2] = g_GyroCfgInfo[gID].AxisSelec[2];
	pRSCInfoInit->DirSelec[0] = g_GyroCfgInfo[gID].DirSelec[0];
	pRSCInfoInit->DirSelec[1] = g_GyroCfgInfo[gID].DirSelec[1];
	pRSCInfoInit->DirSelec[2] = g_GyroCfgInfo[gID].DirSelec[2];
	pRSCInfoInit->uiDataUnit = (65536 / gyro_getLSB(gID));
	pRSCInfoInit->uiCalGain[0] = 1024;
	pRSCInfoInit->uiCalGain[1] = 1024;
	pRSCInfoInit->uiCalGain[2] = 1024;
	pRSCInfoInit->iCalOfs[0] = 0;
	pRSCInfoInit->iCalOfs[1] = 0;
	pRSCInfoInit->iCalOfs[2] = 0;
}


ER gyro_setMode(GYRO_SENSOR_ID gID, GYRO_OP_MODE opMode)
{
	DBG_IND("opMode %d\r\n", opMode);

	if (gyroObj[gID].gyroStatus == GYRO_IDLE) {
		//DBG_WRN("Gyro is not opened\r\n");
		return E_SYS;
	}
	// state check: only while idle
	if ((gyroObj[gID].gyroStatus != GYRO_READY) && (gyroObj[gID].gyroOpMode != GYRO_OP_MODE_FREE_RUN)) {
		DBG_ERR("gyro_setMode, gyroObj[gID].gyroOpMode = %d, gyroObj[gID].gyroStatus=%d!!\r\n", gyroObj[gID].gyroOpMode, gyroObj[gID].gyroStatus);
		return E_SYS;
	}
	if (opMode == GYRO_OP_MODE_SINGLE_ACCESS) {
		if (gyroObj[gID].gyroOpMode == GYRO_OP_MODE_SINGLE_ACCESS) {
			//DBG_WRN("gyro_setMode, already single access mode!!\r\n");
			return E_OK;
		} else if (gyroObj[gID].gyroOpMode == GYRO_OP_MODE_FREE_RUN) {
			// disable free run
			spi_stopGyro(gyroObj[gID].spiID);
			spi_close(gyroObj[gID].spiID);

			gyroObj[gID].gyroOpMode = GYRO_OP_MODE_SINGLE_ACCESS;
			gyroObj[gID].gyroStatus = GYRO_READY;
			gyroObj[gID].gyroBufInIdx = 0;
			gyroObj[gID].gyroBufOutIdx = 0;
		} else {
			DBG_ERR("gyro_setMode, gyroObj[gID].gyroOpMode=%d!!\r\n", opMode);
			return E_SYS;
		}
	} else if (opMode == GYRO_OP_MODE_FREE_RUN) {
		if (gyroObj[gID].gyroOpMode == GYRO_OP_MODE_FREE_RUN) {
			//DBG_WRN("gyro_setMode, already free run mode!!\r\n");
			// apply new free run parameters
			gyro_setFreeRun(gID, TRUE);

			return E_OK;
		} else if (gyroObj[gID].gyroOpMode == GYRO_OP_MODE_SINGLE_ACCESS) {
			if (gyroObj[gID].frPeriodUs == 0) {
				DBG_ERR("gyro_setMode, run mode param is not set!!\r\n");
				return E_SYS;
			}
			// enable free run
			gyro_setFreeRun(gID, FALSE);

			gyroObj[gID].gyroOpMode = GYRO_OP_MODE_FREE_RUN;
			gyroObj[gID].gyroStatus = GYRO_RUN;
		} else {
			DBG_ERR("gyro_setMode, gyroObj[gID].gyroOpMode=%d!!\r\n", opMode);
			return E_SYS;
		}
	} else {
		DBG_ERR("gyro_setMode, opMode=%d!!\r\n", opMode);
		return E_SYS;
	}
	return E_OK;
}

static void gyro_AdjustDataOffset(GYRO_SENSOR_ID gID, INT32 *gx, INT32 *gy, INT32 *gz)
{
	INT32  tempX, tempY, tempZ;

	//DBG_IND("gx=%ld, gy=%ld, g_xOffset = %ld, g_yOffset = %ld\r\n",*gx,*gy,g_xOffset,g_yOffset);
	tempX = (INT16) * gx;
	if (tempX + gyroObj[gID].xOffset > 32767) {
		tempX = 32767;
	} else if (tempX + gyroObj[gID].xOffset < -32768) {
		tempX = -32768;
	} else {
		tempX += gyroObj[gID].xOffset;
	}

	tempY = (INT16) * gy;
	if (tempY + gyroObj[gID].yOffset > 32767) {
		tempY = 32767;
	} else if (tempY + gyroObj[gID].yOffset < -32768) {
		tempY = -32768;
	} else {
		tempY += gyroObj[gID].yOffset;
	}

	tempZ = (INT16) * gz;
	if (tempZ + gyroObj[gID].zOffset > 32767) {
		tempZ = 32767;
	} else if (tempZ + gyroObj[gID].zOffset < -32768) {
		tempZ = -32768;
	} else {
		tempZ += gyroObj[gID].zOffset;
	}

	*gx = tempX;
	*gy = tempY;
	*gz = tempZ;

	//DBG_IND("gx=%ld, gy=%ld \r\n",*gx,*gy);
}

ER gyro_getSingleData(GYRO_SENSOR_ID gID, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
	UINT32  tempData = 0;

	if (!puiDatX) {
		DBG_ERR("puiDatX is NULL\r\n");
		return E_PAR;
	}
	if (!puiDatY) {
		DBG_ERR("puiDatY is NULL\r\n");
		return E_PAR;
	}
	if (!puiDatZ) {
		DBG_ERR("puiDatZ is NULL\r\n");
		return E_PAR;
	}
	gyro_readReg(gID, REG_GYRO_XOUT_L, &tempData);
	*(UINT32 *)puiDatX = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
	gyro_readReg(gID, REG_GYRO_YOUT_L, &tempData);
	*(UINT32 *)puiDatY = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
	gyro_readReg(gID, REG_GYRO_ZOUT_L, &tempData);
	*(UINT32 *)puiDatZ = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
	return E_OK;
}




ER gyro_getFreeRunData(GYRO_SENSOR_ID gID, UINT32 *puiNum, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{
	UINT32 i, j;
	UINT32 *uiDataNum = &(gyroObj[gID].frDataNum);
	UINT32 *gyroBufInIdx = &(gyroObj[gID].gyroBufInIdx);
	UINT32 *gyroBufOutIdx = &(gyroObj[gID].gyroBufOutIdx);
	UINT32 *gyroQueueCount = &(gyroObj[gID].gyroQueueCount);

	// state check: only while run
	if (gyroObj[gID].gyroStatus != GYRO_RUN) {
		DBG_ERR("gyro_getGyroData, gyroObj[gID].gyroStatus=%d!!\r\n", gyroObj[gID].gyroStatus);
		return E_SYS;
	}

	// op mode check: only while free run mode
	if (gyroObj[gID].gyroOpMode != GYRO_OP_MODE_FREE_RUN) {
		DBG_ERR("gyro_getGyroData, not in free run mode!!\r\n", gyroObj[gID].gyroOpMode);
		return E_SYS;
	}
	// normal job
	if ((*gyroBufOutIdx + *uiDataNum) == *gyroBufInIdx) {
		j = *gyroBufOutIdx;
		for (i = 0; i < *uiDataNum; i++) {
			UINT32 k;
			k = j % GDI2_GB_NUM;
			*(puiDatX + i) = gyroObj[gID].buf.type1.gyro[k][2];
			*(puiDatY + i) = gyroObj[gID].buf.type1.gyro[k][3];
			*(puiDatZ + i) = gyroObj[gID].buf.type1.gyro[k][4];
			gyro_AdjustDataOffset(gID, puiDatX + i, puiDatY + i, puiDatZ + i);
			j++;
			if (i == 0) {
				DBG_IND("Count = %d,gyro x=%5d, y=%5d, z=%5d\r\n", *gyroQueueCount >> 1, *puiDatX, *puiDatY, *puiDatZ);
				DBG_IND("accel x=%5d, y=%5d, z=%5d\r\n", gyroObj[gID].buf.type1.accel[k][2], gyroObj[gID].buf.type1.accel[k][3], gyroObj[gID].buf.type1.accel[k][4]);
			}
		}

		*gyroBufOutIdx = j;
		*puiNum = (*gyroQueueCount >> 1);

	} else if ((*gyroBufOutIdx + *uiDataNum) > *gyroBufInIdx) {
		DBG_ERR("asking for future data!!  *gyroBufOutIdx %d *uiDataNum %d *gyroBufInIdx %d\r\n", *gyroBufOutIdx, *uiDataNum, *gyroBufInIdx);
		memset((void *)puiDatX, 0, *uiDataNum * sizeof(UINT32));
		memset((void *)puiDatY, 0, *uiDataNum * sizeof(UINT32));
		memset((void *)puiDatZ, 0, *uiDataNum * sizeof(UINT32));
		*puiNum = 0;
	} else if ((*gyroBufOutIdx + *uiDataNum) < *gyroBufInIdx) {
		DBG_ERR("asking for past data!!\r\n");
		*gyroBufOutIdx =  *gyroBufInIdx;
		memset((void *)puiDatX, 0, *uiDataNum * sizeof(UINT32));
		memset((void *)puiDatY, 0, *uiDataNum * sizeof(UINT32));
		memset((void *)puiDatZ, 0, *uiDataNum * sizeof(UINT32));
		//*puiNum = *uiDataNum;
		*puiNum = (*gyroQueueCount >> 1);
	}
	// state change: run -> run
	gyroObj[gID].gyroStatus = GYRO_RUN;

	return E_OK;
}

void gyro_setCalZeroOffset(GYRO_SENSOR_ID gID, INT32 xOffset, INT32 yOffset, INT32 zOffset)
{
	gyroObj[gID].xOffset = xOffset;
	gyroObj[gID].yOffset = yOffset;
	gyroObj[gID].zOffset = zOffset;
	DBG_IND("xOffset = %d, yOffset = %d, zOffset = %d\r\n", xOffset, yOffset, zOffset);
}

static void gyro_AdjustGsData(GYRO_SENSOR_ID gID, INT32 *gx, INT32 *gy, INT32 *gz)
{
	INT32  tempX, tempY, tempZ;

	//DBG_IND("gx=%ld, gy=%ld, gz=%ld\r\n",*gx,*gy,*gz);

	tempX = (INT16) * gx;
	tempY = (INT16) * gy;
	tempZ = (INT16) * gz;
	*gx = tempX;
	*gy = tempY;
	*gz = tempZ;

	//DBG_IND("gx=%ld, gy=%ld, gz=%ld\r\n",*gx,*gy,*gz);
}

ER gyro_getGsData(GYRO_SENSOR_ID gID, INT32 *puiDatX, INT32 *puiDatY, INT32 *puiDatZ)
{

	UINT32 *gyroBufInIdx = &(gyroObj[gID].gyroBufInIdx);

	if (!puiDatX) {
		DBG_ERR("puiDatX is NULL\r\n");
		return E_PAR;
	}
	if (!puiDatY) {
		DBG_ERR("puiDatY is NULL\r\n");
		return E_PAR;
	}
	if (!puiDatZ) {
		DBG_ERR("puiDatZ is NULL\r\n");
		return E_PAR;
	}
	if (gyroObj[gID].gyroOpMode == GYRO_OP_MODE_SINGLE_ACCESS) {
		UINT32 tempData = 0;

		gyro_readReg(gID, REG_ACCEL_XOUT_L, &tempData);
		*(UINT32 *)puiDatX = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
		gyro_readReg(gID, REG_ACCEL_YOUT_L, &tempData);
		*(UINT32 *)puiDatY = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
		gyro_readReg(gID, REG_ACCEL_ZOUT_L, &tempData);
		*(UINT32 *)puiDatZ = ((tempData & 0x0000FF00) >> 8) | ((tempData & 0x000000FF) << 8);
	} else {
		UINT32 k;
		if (*gyroBufInIdx > 0) {
			k = (*gyroBufInIdx - 1) % GDI2_GB_NUM;
		} else {
			k = *gyroBufInIdx % GDI2_GB_NUM;
		}
		*puiDatX = gyroObj[gID].buf.type1.accel[k][2];
		*puiDatY = gyroObj[gID].buf.type1.accel[k][3];
		*puiDatZ = gyroObj[gID].buf.type1.accel[k][4];
	}

	gyro_AdjustGsData(gID, puiDatX, puiDatY, puiDatZ);
	return E_OK;
}



