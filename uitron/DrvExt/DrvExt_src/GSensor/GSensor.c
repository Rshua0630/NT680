

#include "Type.h"
#include "i2c.h"
#include "GSensor.h"

#define __MODULE__          GSensor
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 1        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

static GSENSOR_INFO g_GsensorInfo;

void GSensor_I2C_SetInfo(GSENSOR_INFO *pGSensorInfo)
{
	g_GsensorInfo.I2C_RegBytes    = pGSensorInfo->I2C_RegBytes;
	g_GsensorInfo.I2C_PinMux      = pGSensorInfo->I2C_PinMux;
	g_GsensorInfo.I2C_BusClock    = pGSensorInfo->I2C_BusClock;
	g_GsensorInfo.I2C_Slave_WAddr = pGSensorInfo->I2C_Slave_WAddr;
	g_GsensorInfo.I2C_Slave_RAddr = pGSensorInfo->I2C_Slave_RAddr;
}

GSENSOR_INFO *GSensor_I2C_GetInfo(void)
{
	return &g_GsensorInfo;
}


I2C_STS GSensor_I2C_Receive(UINT32 *value, BOOL bNACK, BOOL bStop)
{
	I2C_DATA I2cData;
	I2C_BYTE I2cByte[1];
	I2C_STS ret;

	I2cData.VersionInfo = DRV_VER_96650;
	I2cData.ByteCount = I2C_BYTE_CNT_1;
	I2cData.pByte = I2cByte;
	I2cByte[0].Param = I2C_BYTE_PARAM_NONE;
	if (bNACK == TRUE) {
		I2cByte[0].Param |= I2C_BYTE_PARAM_NACK;
	}
	if (bStop == TRUE) {
		I2cByte[0].Param |= I2C_BYTE_PARAM_STOP;
	}

	ret = i2c_receive(&I2cData);
	if (ret != I2C_STS_OK) {
		DBG_ERR("i2c ret = %02x!!\r\n", ret);
	}

	*value = I2cByte[0].uiValue;

	return ret;
}

I2C_STS GSensor_I2C_Transmit(UINT32 value, BOOL bStart, BOOL bStop)
{
	I2C_DATA I2cData;
	I2C_BYTE I2cByte[1];
	I2C_STS ret;

	I2cData.VersionInfo = DRV_VER_96650;
	I2cData.ByteCount = I2C_BYTE_CNT_1;
	I2cData.pByte = I2cByte;
	I2cByte[0].uiValue = value & 0xff;
	I2cByte[0].Param = I2C_BYTE_PARAM_NONE;
	if (bStart == TRUE) {
		I2cByte[0].Param |= I2C_BYTE_PARAM_START;
	}
	if (bStop == TRUE) {
		I2cByte[0].Param |= I2C_BYTE_PARAM_STOP;
	}

	DBG_IND("I2cData VersionInfo=0x%x, ByteCount =0x%x, uiValue=0x%x, parm=0x%x\r\n", I2cData.VersionInfo, I2cData.ByteCount, I2cByte[0].uiValue, I2cByte[0].Param);
	ret = i2c_transmit(&I2cData);
	if (ret != I2C_STS_OK) {
		DBG_ERR("i2c ret = %d!!\r\n", ret);
	}
	return ret;
}

/**
  Initialize Gsensor IIC bus

  Initialize Gsensor IIC bus

  @param void
  @return void
*/
BOOL GSensor_I2C_Init(GSENSOR_INFO GSensorInfo)
{
	UINT erReturn;

	g_GsensorInfo.I2C_RegBytes = GSensorInfo.I2C_RegBytes;
	g_GsensorInfo.I2C_PinMux = GSensorInfo.I2C_PinMux;
	g_GsensorInfo.I2C_BusClock = GSensorInfo.I2C_BusClock;
	g_GsensorInfo.I2C_Slave_WAddr = GSensorInfo.I2C_Slave_WAddr;
	g_GsensorInfo.I2C_Slave_RAddr = GSensorInfo.I2C_Slave_RAddr;

	if (g_GsensorInfo.I2C_RegBytes > GSENSOR_I2C_REGISTER_2BYTE) {
		DBG_ERR("Error register Byte Count !!\r\n");
		return FALSE;
	}

	if (g_GsensorInfo.I2C_PinMux > I2C_PINMUX_2ND) {
		DBG_ERR("Error I2C Pin mux !!\r\n");
		return FALSE;
	}

	if (g_GsensorInfo.I2C_BusClock > I2C_BUS_CLOCK_1MHZ) {
		DBG_ERR("Error I2C Bus Clock !!\r\n");
		return FALSE;
	}

	erReturn = i2c_open(&(g_GsensorInfo.I2C_Channel));
	if (erReturn != I2C_STS_OK) {
		DBG_ERR("Error open I2C driver!!\r\n");
		return FALSE;
	}

	i2c_setConfig(g_GsensorInfo.I2C_Channel, I2C_CONFIG_ID_PINMUX, g_GsensorInfo.I2C_PinMux); //select to control I2C 1st device
	i2c_setConfig(g_GsensorInfo.I2C_Channel, I2C_CONFIG_ID_BUSCLOCK, g_GsensorInfo.I2C_BusClock); //up to 400KHZ
#if _TODO
i2c_setConfig(g_GsensorInfo.I2C_Channel, I2C_CONFIG_ID_SCLTIMEOUT_CHECK, DISABLE); // disable SCL low timeout check
#endif

	return TRUE;
}

void GSensor_I2C_Exit(void)
{
	i2c_close(g_GsensorInfo.I2C_Channel);
}

void GSensor_I2C_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
	UINT        erReturn;
	UINT32      ulWriteAddr, ulReg1, ulData;
	UINT32      ulReg2 = 0;

	i2c_lock(g_GsensorInfo.I2C_Channel);

	ulWriteAddr = g_GsensorInfo.I2C_Slave_WAddr;
	if (g_GsensorInfo.I2C_RegBytes == GSENSOR_I2C_REGISTER_2BYTE) {
		ulReg1      = (uiAddr >> 8);
		ulReg2      = ((uiAddr) & (0x000000ff));
	} else {
		ulReg1      = ((uiAddr) & (0x000000ff));
	}
	ulData      = ((uiValue) & (0x000000ff));
	do {
		erReturn = GSensor_I2C_Transmit(ulWriteAddr, 1, 0);
		if (erReturn != I2C_STS_OK) {
			DBG_ERR("Gsensor Error transmit data (write addr)!!\r\n");
			break;
		}

		erReturn = GSensor_I2C_Transmit(ulReg1, 0, 0);
		if (erReturn != I2C_STS_OK) {
			DBG_ERR("Gsensor Error transmit data (reg) !!\r\n");
			break;
		}

		if (g_GsensorInfo.I2C_RegBytes == GSENSOR_I2C_REGISTER_2BYTE) {
			erReturn = GSensor_I2C_Transmit(ulReg2, 0, 0);
			if (erReturn != I2C_STS_OK) {
				DBG_ERR("Gsensor Error transmit data (reg) !!\r\n");
				break;
			}
		}

		erReturn = GSensor_I2C_Transmit(ulData, 0, 1);
		if (erReturn != I2C_STS_OK) {
			DBG_ERR("Gsensor Error transmit data (data1) !!\r\n");
			break;
		}
	} while (erReturn != I2C_STS_OK);

	i2c_unlock(g_GsensorInfo.I2C_Channel);
}

UINT32 GSensor_I2C_ReadReg(UINT32 uiAddr)
{
	UINT32      ulWriteAddr, ulReadAddr, ulReg1;
	static UINT32 ulData1;

	ulWriteAddr = g_GsensorInfo.I2C_Slave_WAddr;
	ulReadAddr  = g_GsensorInfo.I2C_Slave_RAddr;
	ulReg1      = (uiAddr & 0x000000ff);
	ulData1     = 0;

	if (i2c_lock(g_GsensorInfo.I2C_Channel) != E_OK) {
		DBG_ERR("GSensor: readReg I2C Lock Fail\r\n");
		return ulData1;
	}

	if (GSensor_I2C_Transmit(ulWriteAddr, 1, 0) != I2C_STS_OK) {
		DBG_ERR("Error transmit data1!!\r\n");
		i2c_unlock(g_GsensorInfo.I2C_Channel);
		return  ulData1;
	}

	if (GSensor_I2C_Transmit(ulReg1, 0, 0) != I2C_STS_OK) {
		DBG_ERR("Error transmit data2!!\r\n");
		i2c_unlock(g_GsensorInfo.I2C_Channel);
		return  ulData1;
	}

	if (GSensor_I2C_Transmit(ulReadAddr, 1, 0) != I2C_STS_OK) {
		DBG_ERR("Error transmit data3!!\r\n");
		i2c_unlock(g_GsensorInfo.I2C_Channel);
		return  ulData1;
	}

	if (GSensor_I2C_Receive(&ulData1, 1, 1) != I2C_STS_OK) {
		DBG_ERR("Error Receive data!!\r\n");
		i2c_unlock(g_GsensorInfo.I2C_Channel);
		return  ulData1;
	}

	if (i2c_unlock(g_GsensorInfo.I2C_Channel) != E_OK) {
		DBG_ERR("GSensor: readReg I2C UnLock Fail\r\n");
		return  ulData1;
	}

	return  ulData1;
}

