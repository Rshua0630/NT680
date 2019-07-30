/**
    Video-by-One(Vx1) HS Controller global header

    Video-by-One(Vx1) HS and Sublink Controller global header.

    @file       vx1.h
    @ingroup    mIDrvIO_Vx1
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#ifndef _VX1_H
#define _VX1_H

#include "Type.h"

/**
    @addtogroup mIDrvIO_Vx1
*/
//@{

/**
	Vx1 Driver Object ID

	This field is used at vx1_getDrvObject(VX1_ID) to get the related controller control object.
*/
typedef enum {
	VX1_ID_1,	///< Vx1_1 Controller. (This is up to 2 lanes Vx1 controller.)
	VX1_ID_2,	///< Vx1_2 Controller. (This is 1 lane Vx1 controller.)

	VX1_ID_MAX,
	ENUM_DUMMY4WORD(VX1_ID)
} VX1_ID;

/**
    Vx1 controller general configuration Selection

    This is used in pVx1Obj->setConfig()/pVx1Obj->getConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	VX1_CONFIG_ID_OPMODE,           ///< Operation Mode selection. Please use VX1_OPMODE as input.
									///< Default value is VX1_OPMODE_NRML_1LANE which is compatible with NT96663.
	VX1_CONFIG_ID_RXMODE,           ///< Select Vx1 receive mode. Please use VX1_RXMODE as input parameter.
	VX1_CONFIG_ID_PIXSWAP,          ///< If RXMODE is VX1_RXMODE_MUX24_TO_2X12/VX1_RXMODE_MUX32_TO_2X12,
									///< this feature can set the pixel order swap.
	VX1_CONFIG_ID_BITSWAP,          ///< Set Bit-wise swap for each pixel data.
	VX1_CONFIG_ID_HDMUX,            ///< Select HD signal to SIE is from HSYNC or DE from Vx1 PHY.
									///< Please use VX1_HDMUX as input parameter.
	VX1_CONFIG_ID_LOCKSTS_ALN,      ///< This can only be used in pVx1Obj->getConfig() to get the LOCK-Status of Vx1 Alignment.
	VX1_CONFIG_ID_LOCKSTS_CDR,      ///< This can only be used in pVx1Obj->getConfig() to get the LOCK-Status of Vx1 CDR.
	VX1_CONFIG_ID_LOCKSTS_CALLBACK, ///< IST Callback function when Vx1 Locked or UnLocked. Please use DRV_CB as function prototype.
									///< When Locked, the notify event would be VX1_INTERRUPT_LOCKED. When UnLocked, the notify event would be VX1_INTERRUPT_UNLOCKED.
	VX1_CONFIG_ID_COLOR_MODE,       ///< This can only be used in pVx1Obj->getConfig() to get the PHY's 3/4/5byte mode. 0x1 is 3byte mode. 0x2 is 4byte mode.0x3 is 5byte mode.
	VX1_CONFIG_ID_FIFO_EN,          ///< This can only be used in pVx1Obj->getConfig() to get the Digital FIFO is enable or disable.
	VX1_CONFIG_ID_SPEED,            ///< This can only be used in pVx1Obj->getConfig() to get the PHY's operation speed in Mbps.

	VX1_CONFIG_ID_HDRMODE,          ///< When VX1_CONFIG_ID_OPMODE is set to VX1_OPMODE_HDR_1LANE/VX1_OPMODE_HDR_2LANE,
									///< This config id selects the HDR mode options. Please use VX1_HDRMODE as input parameter.
	VX1_CONFIG_ID_VSYNC_ISSUE_MULTI,///< When Vx1 is used to connect SONY DOL HDR MIPI Sensor, this shall be set to ENABLE. Default is DISABLE.
	VX1_CONFIG_ID_WIDTH,			///< Set the receive Image Width in pixel count.
	VX1_CONFIG_ID_CHID1,			///< Image's Channel ID value. This image would send to SIE1. Valid value range can be 0x0~0xF.
	VX1_CONFIG_ID_CHID2,			///< Image's Channel ID value. This image would send to SIE2. Valid value range can be 0x0~0xF.
	VX1_CONFIG_ID_CHID3,			///< Image's Channel ID value. This image would send to SIE3. Valid value range can be 0x0~0xF.
	VX1_CONFIG_ID_CHID4,			///< Image's Channel ID value. This image would send to SIE4. Valid value range can be 0x0~0xF.
	VX1_CONFIG_ID_CHID_BIT0,		///< Channel ID bit-0 location offset. This is used in SONY DOL Sensor.
	VX1_CONFIG_ID_CHID_BIT1,		///< Channel ID bit-1 location offset. This is used in SONY DOL Sensor.
	VX1_CONFIG_ID_CHID_BIT2,		///< Channel ID bit-2 location offset. This is used in SONY DOL Sensor.
	VX1_CONFIG_ID_CHID_BIT3,		///< Channel ID bit-3 location offset. This is used in SONY DOL Sensor.

	VX1_CONFIG_ID_VSYNC_INV,		///< Set VSYNC pulse active edge. Default is active LOW. Set ENABLE to make it active HIGH.
	VX1_CONFIG_ID_HSYNC_INV,		///< Set HSYNC pulse active edge. Default is active LOW. Set ENABLE to make it active HIGH.

	VX1_CONFIG_ID_SETPATCH,			///< Set 680B ECO patch after SIE start.
	VX1_CONFIG_ID_TIMEOUT_PERIOD,	///< waitInterrupt timeout period. Unit in ms. Set to 0 means no timeout. Default is 1000 ms.
	VX1_CONFIG_ID_TXINIT_VAL,		///< Get initTransmitter 's init mode value.

	VX1_CONFIG_ID_RESERVED_1,		// Novatek internal test usgae. Shall not using this.
	VX1_CONFIG_ID_RESERVED_2,		// Novatek internal test usgae. Shall not using this.
	ENUM_DUMMY4WORD(VX1_CONFIG_ID)
} VX1_CONFIG_ID;


/**
    Vx1 interrupt ID

    This is used in pVx1Obj->waitInterrupt() to specify which interrupt is waiting for.
*/
typedef enum {
	VX1_INTERRUPT_LOCKED    = 0x004,    ///< Vx1 ALN Locked.
	VX1_INTERRUPT_UNLOCKED  = 0x008,    ///< Vx1 ALN UnLocked.
	VX1_INTERRUPT_SUBLINK   = 0x020,    //   Vx1 Sublink. Driver internal usages.
	VX1_INTERRUPT_VD        = 0x100,    ///< Vx1 Vsync to SIE1.
	VX1_INTERRUPT_VD2       = 0x200,    ///< Vx1 Vsync to SIE2.
	VX1_INTERRUPT_VD3       = 0x400,    ///< Vx1 Vsync to SIE3.
	VX1_INTERRUPT_VD4       = 0x800,    ///< Vx1 Vsync to SIE4.

	VX1_INTERRUPT_ABORT     = 0x80000000,
	ENUM_DUMMY4WORD(VX1_INTERRUPT)
} VX1_INTERRUPT;

/**
	Vx1 Operation Mode

	This is used at pVx1Obj->setConfig(VX1_CONFIG_ID_OPMODE) to set the Vx1 opeation mode.
*/
typedef enum {
	VX1_OPMODE_NRML_1LANE,	///< Normal 1 lane mode.
	VX1_OPMODE_NRML_2LANE,	///< Normal 2 lanes mode.
	VX1_OPMODE_HDR_1LANE,	///< 1 lane  HDR mode.
	VX1_OPMODE_HDR_2LANE,	///< 2 lanes HDR mode.

	ENUM_DUMMY4WORD(VX1_OPMODE)
} VX1_OPMODE;

/**
	HDR Mode Setting

	When VX1_CONFIG_ID_OPMODE is set to VX1_OPMODE_HDR_1LANE or VX1_OPMODE_HDR_2LANE,
	This field is set the HDR config mode.
*/
typedef enum {
	VX1_HDRMODE_SF_VC,	///< Virtual Channel HDR mode. This is used for OV/ON-Semi HDR sensors.
	VX1_HDRMODE_SF_LI,	///< LineInformation HDR mode. This is used for SONY DOL HDR sensors.

	ENUM_DUMMY4WORD(VX1_HDRMODE)
} VX1_HDRMODE;


/**
    Vx1 Receive Mode Selection

    This is used in pVx1Obj->setConfig(VX1_CONFIG_ID_RXMODE) to assign the Vx1 Rx Mode.
*/
typedef enum {
	VX1_RXMODE_MUX11_0,         ///< Select Vx1 PHY-D[11:0]  as pixel data to SIE.
	VX1_RXMODE_MUX23_12,        ///< Select Vx1 PHY-D[23:12] as pixel data to SIE.
	VX1_RXMODE_MUX24_TO_2X12,   ///< Select Vx1 PHY-D[11:0] as first pixel and PHY-D[23:12] as second pixel data to SIE.
	VX1_RXMODE_MUX24_TO_16,     ///< Select Vx1 PHY-D[23:16][11:4] as 16bits pixel data to SIE.
	VX1_RXMODE_MUX32_TO_2X12,   ///< Select Vx1 PHY-D[11:0] as first pixel and PHY-D[27:16] as second pixel data to SIE.
	VX1_RXMODE_MUX24_TO_16V2,   ///< Select Vx1 PHY-D[23:16][7:0] as 16bits pixel data to SIE.
	VX1_RXMODE_MUX32_TO_2X16,   ///< Select Vx1 PHY-D[31:16][15:0] as 2 x 16bits-pixel data to SIE.

	ENUM_DUMMY4WORD(VX1_RXMODE)
} VX1_RXMODE;

/**
    Vx1 HD Mux

    This selects HD(Hsync) signal to SIE is from Vx1-PHY's DE or Hsync signal.
*/
typedef enum {
	VX1_HDMUX_DE,               ///< HD to SIE is from Vx1 PHY's DE signal.
	VX1_HDMUX_HSYNC,            ///< HD to SIE is from Vx1 PHY's Hsync signal.

	ENUM_DUMMY4WORD(VX1_HDMUX)
} VX1_HDMUX;

/**
    Vx1 sublink general configuration Selection

    This is used in pVx1Obj->setSublinkConfig()/pVx1Obj->getSublinkConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	VX1SBL_CONFIG_ID_REGADDR_BYTECOUNT,     ///< Set i2c device (Sensor) 's register address is 1 to 4 bytes.
	VX1SBL_CONFIG_ID_DATA_BYTECOUNT,        ///< Set i2c device (Sensor) 's register data is 1 to 4 bytes.
	VX1SBL_CONFIG_ID_I2C_SPEED,             ///< Set i2c device (Sensor) 's I2C Bus speed as 50KHz/100KHz/200KHz/400KHz.
	VX1SBL_CONFIG_ID_NACK_CHECK,            ///< Enable/Disable i2c nack check in vx1_writeI2cDevice/vx1_readI2cDevice. Default is enabled.
	VX1SBL_CONFIG_ID_TXMODULE,              ///< Set the Vx1 transmitter module. Please use VX1_TXMODULE as input parameter.
	VX1SBL_CONFIG_ID_TX_LANE_NUMBER,        ///< Set Vx1 transmitter module lane number. Only 1 or 2 are valid.
											///< Default value is 1 lane.
	VX1SBL_CONFIG_ID_TXMIPI_LANE_NUMBER,    ///< Set the THCV241 transmitter's mipi input lane number. Only 1/2/4 are valid.

	VX1SBL_CONFIG_ID_TARGET_MCLK,           ///< Config the Sensor (on transmitter board) target MCLK freq in Hertz.
											///< Default value is 24000000(Hz).
	VX1SBL_CONFIG_ID_REAL_MCLK,             ///< Config the Sensor (on transmitter board) real operating MCLK freq in Hertz.
											///< Default value is 24000000(Hz). This would change the HSDATAO_DELAY value.
	VX1SBL_CONFIG_ID_HSDATAO_DELAY,         ///< Assign Vx1 Transmitter(THCV241) HSDATAO_DELAY directly. Valid value range is 0x1~0xFF.
	VX1SBL_CONFIG_ID_CKO_OUTOUT,			///< Enable/Disable the CKO signal on the THCV241.
	VX1SBL_CONFIG_ID_SENSOR_CKSPEED_BPS,	///< When using THCV241 is PLL auto calculation path.
											///< The user must assign the sensor clock speed in bits per second to make calculation tool working.
											///< For example, if the sensor clock speed is 445Mbps, please set 445000000 (bps) to this field. default is 0.
	VX1SBL_CONFIG_ID_FREQ_CALC_EXCEED,		///< Valid value is 0 ~ 3. default is 1. This assign the auto calculation frequency min value in % than optimal freq.



	VX1SBL_CONFIG_ID_EMULATION_0,           // novatek internal usage.
	ENUM_DUMMY4WORD(VX1SBL_CONFIG_ID)
} VX1SBL_CONFIG_ID;

/**
    Vx1 transmitter Module select

    Supported Vx1 transmitter Module select. Currently we support THCV231 and THCV235.
    These select options are used in pVx1Obj->initTransmitter()/pVx1Obj->cfgTransmitterGPIO() to initialize
    transmitter operation mode and configure the transmitter's GPIO function.
*/
typedef enum {
	VX1_TXMODULE_THCV235,       ///< Transmitter module is THCV235.
								///< The THCV235 only supports the Vx1 4-Bytes Mode.(Inlcude MUX function)
	VX1_TXMODULE_THCV231,       ///< Transmitter module is THCV235.
								///< The THCV231 supports the Vx1 4-Bytes and 3-Bytes Mode.(Inlcude MUX function)
	VX1_TXMODULE_THCV241,       ///< Transmitter module is THCV241.
								///< The THCV241 supports MIPI sensor and sensor HDR mode.

	ENUM_DUMMY4WORD(VX1_TXMODULE)
} VX1_TXMODULE;

/**
    Vx1 Transmitter operation mode select

    These options are used in pVx1Obj->initTransmitter() to initialize the operation mode.
    Different operation mode maps the transmiiter to different operation bus frerquency.
*/
typedef enum {
	VX1_TXINIT_3BYTEMODE,       		///< 3-Bytes Mode.
										///< The Bus frequency would be Sensor (pixel-clock x 30)Hz.
	VX1_TXINIT_3BYTEMODE_MUX,   		///< 3-Bytes Mode with MUX function enabled.
										///< The Bus frequency would be Sensor (pixel-clock x 15)Hz.
	VX1_TXINIT_4BYTEMODE,       		///< 4-Bytes Mode.
										///< The Bus frequency would be Sensor (pixel-clock x 40)Hz.
	VX1_TXINIT_4BYTEMODE_MUX,   		///< 4-Bytes Mode with MUX function enabled.
										///< The Bus frequency would be Sensor (pixel-clock x 20)Hz.

	VX1_TXINIT_THCV241_YUV422_MAP1,     ///< THCV241 Format for YUV422_MAP1. Please refer to THCV241 datasheets for details.
	VX1_TXINIT_THCV241_RAW12HF2_MAP2,   ///< THCV241 Format for RAW12.3BYTE-MODE with MUX. Please refer to THCV241 datasheets for details.
	VX1_TXINIT_THCV241_RAW10HF2_MAP3,   ///< THCV241 Format for RAW10.3BYTE-MODE with MUX. Please refer to THCV241 datasheets for details.

	VX1_TXINIT_THCV241_RAW12HF2_MAP1,   ///< THCV241 Format for RAW12.3BYTE-MODE with MUX. Please refer to THCV241 datasheets for details.
										///< Virtual Channel HDR Mode must be using this mode.
	VX1_TXINIT_THCV241_RAW10HF2_MAP1,   ///< THCV241 Format for RAW10.3BYTE-MODE with MUX. Please refer to THCV241 datasheets for details.
										///< Virtual Channel HDR Mode must be using this mode.

	VX1_TXINIT_MAX,
	ENUM_DUMMY4WORD(VX1_TXINIT)
} VX1_TXINIT;

/**
    Vx1 Sublink I2C command transfer status

    The i2c command transfer result through the Vx1 sublink to sensor board on the Vx1 transmitter.
    This is used in pVx1Obj->writeI2cDevice()/pVx1Obj->readI2cDevice() as return parameter.
*/
typedef enum {
	VX1_I2CSTS_OK,              ///< I2C command transfer done and success. (Acked)
	VX1_I2CSTS_NACK,            ///< I2C command transfer done but got NACK.
	VX1_I2CSTS_ERROR,           ///< I2C command transfer not complete due to some errors,
								///< such as vx1 cable-unplug, driver no open, ...etc.

	ENUM_DUMMY4WORD(VX1_I2CSTS)
} VX1_I2CSTS;

/**
    Vx1 I2C bus speed on transmitter module

    This is used in pVx1Obj->setSublinkConfig(VX1SBL_CONFIG_ID_I2C_SPEED) to configure the I2C bus speed
    to sensor borad on the Vx1 transmitter module. Default value is 100KHz.
*/
typedef enum {
	VX1_I2CSPEED_50KHz,         ///< I2C Bus speed is 50KHz.
	VX1_I2CSPEED_100KHz,        ///< I2C Bus speed is 100KHz. (Default)
	VX1_I2CSPEED_200KHz,        ///< I2C Bus speed is 200KHz.
	VX1_I2CSPEED_400KHz,        ///< I2C Bus speed is 400KHz.

	ENUM_DUMMY4WORD(VX1_I2CSPEED)
} VX1_I2CSPEED;

/**
    GPIO pin select on the Vx1 transmitter module

    The Vx1 transmitter board provides some GPIO pins for input/output usages.
    This is used in pVx1Obj->cfgTransmitterGPIO() to assign dedicate GPIO pin for IO.
*/
typedef enum {
	VX1_TXGPIO_0,               ///< Select Vx1 transmitter's GPIO_0. Valid for THCV241.
	VX1_TXGPIO_1,               ///< Select Vx1 transmitter's GPIO_1. Valid for THCV241.
	VX1_TXGPIO_2,               ///< Select Vx1 transmitter's GPIO_2. Valid for THCV241.
	VX1_TXGPIO_3,               ///< Select Vx1 transmitter's GPIO_3. Valid for THCV241/231/235.
	VX1_TXGPIO_4,               ///< Select Vx1 transmitter's GPIO_4. Valid for THCV231/235.

	ENUM_DUMMY4WORD(VX1_TXGPIO)
} VX1_TXGPIO;

/**
    GPIO Input/Output Direction select on the Vx1 transmitter module

    GPIO Input/Output Direction select on the Vx1 transmitter module
    This is used in pVx1Obj->cfgTransmitterGPIO() to assign dedicate GPIO direction.
*/
typedef enum {
	VX1_GPIODIR_INPUT,          ///< Configure GPIO to input direction.
	VX1_GPIODIR_OUTPUT,         ///< Configure GPIO to output direction.

	ENUM_DUMMY4WORD(VX1_GPIODIR)
} VX1_GPIODIR;

/**
    Vx1 Driver Object Definotions

    This is used by the Vx1 Get Object API vx1_getDrvObject()
    to get the specific Vx1 module control object.
*/
typedef struct {
	VX1_ID          Vx1ID;

	/**
	    Open Vx1 (video mainlink) module

	    Open Vx1 (video mainlink) module.
	    This API would open Vx1 module necessary Clock/Power/interrupt.

	    @return E_OK means open done and success.
	*/
	ER(*open)(void);

	/**
	    Close Vx1 module

	    Close Vx1 module include both the mainlink and sublink.
	    This API would also close the allocated system resource such as Clock/Power/interrupt.

	    @return E_OK means close done and success.
	*/
	ER(*close)(void);

	/**
	    Check if Vx1 is opened or not.

	    Check if Vx1 is opened or not.

	    @return
	     - @b TRUE:     Vx1 is already opened.
	     - @b FALSE:    Vx1 is not opened.
	*/
	BOOL (*isOpened)(void);

	/**
	    Enable/Disable Vx1 Mainlink Video Streaming

	    Enable/Disable Vx1 Mainlink Video Streaming.
	    After ENABLE, the sublink would start to notify the transmitter the hotplug event.

	    @param[in] bEn
	     - @b TRUE:     Enable Vx1 Mainlink Video Streaming
	     - @b FALSE:    Disable Vx1 Mainlink Video Streaming

	    @return void
	*/
	void (*setEnable)(BOOL bEn);

	/**
	    Check if the Vx1 Mainlink is enabled or not

	    Check if the Vx1 Mainlink is enabled or not

	    @return
	     - @b TRUE:     Vx1 is already enabled.
	     - @b FALSE:    Vx1 is disabled.
	*/
	BOOL (*getEnable)(void);

	/**
	    Set Vx1 Configurations

	    Set Vx1 Configurations, such as Mode/Pixel-Swap/Bit-Swap/HD-Mux settings.

	    @param[in]  ConfigID    Configuration selection.
	    @param[in]  uiCfgValue  Configuration Values.

	    @return void
	*/
	void (*setConfig)(VX1_CONFIG_ID ConfigID, UINT32 uiCfgValue);

	/**
	    Get Vx1 Configurations

	    Get Vx1 current Configurations, such as Mode/Pixel-Swap/Bit-Swap/HD-Mux settings.

	    @param[in]  ConfigID    Configuration selection.

	    @return Configuration Values
	*/
	UINT32(*getConfig)(VX1_CONFIG_ID ConfigID);

	/**
	    Wait Vx1 interrupt event

	    Wait Vx1 interrupt event

	    @param[in]  WaitedFlag  Please refer to VX1_INTERRUPT for waiting flag details.

	    @return waited flag.
	*/
	VX1_INTERRUPT(*waitInterrupt)(VX1_INTERRUPT WaitedFlag);

	/**
	    Open Vx1 sublink function

	    Open Vx1 sublink function. This can only be called if the Vx1 Transmitter has Sublink function.
	    This API would initialize the Sublink master functional block.
	    This API must be called after calling vx1_open() first.

	    @return
	     - @b E_OK:     Sublink function open done and success.
	     - @b E_OACV:   vx1_open() should be called first before openSublink.
	*/
	ER(*openSublink)(void);

	/**
	    Close Vx1 sublink function

	    Close Vx1 sublink function

	    @return E_OK means close done and success.
	*/
	ER(*closeSublink)(void);

	/**
	    Check if Vx1 sublink is opened or not.

	    Check if Vx1 sublink is opened or not.

	    @return
	     - @b TRUE:     sublink  is already opened.
	     - @b FALSE:    sublink  is not opened.
	*/
	BOOL (*isSublinkOpened)(void);

	/**
	    Set Vx1 sublink Configurations

	    Set Vx1 sublink Configurations, such as i2c device address/data field byte count.

	    @param[in]  ConfigID    Configuration selection.
	    @param[in]  uiCfgValue  Configuration Values.

	    @return void
	*/
	void (*setSublinkConfig)(VX1SBL_CONFIG_ID ConfigID, UINT32 uiCfgValue);

	/**
	    Get Vx1 sublink Configurations

	    Get Vx1 sublink Configurations, such as i2c device address/data field byte count.

	    @param[in]  ConfigID    Configuration selection.

	    @return Configuration Values.
	*/
	UINT32(*getSublinkConfig)(VX1SBL_CONFIG_ID ConfigID);

	/**
	    Write I2C command to sublink's i2c-device

	    Write I2C command to sublink's i2c-device. The i2c-device is attached on the Vx1 transmitter.

	    @param[in]  uiDevAddr   7-bit I2C device address.(Exclude the 1bit R/W flag.)
	    @param[in]  uiRegAddr   I2C Device Register Address value
	    @param[in]  uiValue     I2C Device Register Data value

	    @return VX1_I2CSTS
	     - @b VX1_I2CSTS_OK:    I2C command transfer done and OK.
	     - @b VX1_I2CSTS_NACK:  I2C command transfer NACKed.
	     - @b VX1_I2CSTS_ERROR: I2C command transfer fail due to some errors.
	                            Such as driver no open, cable-unplug, uiDevAddr exceed-7bits, ...ect.
	*/
	VX1_I2CSTS(*writeI2cDevice)(UINT32 uiDevAddr, UINT32 uiRegAddr, UINT32 uiValue);

	/**
	    Write I2C sequential command to sublink's i2c-device

	    Write I2C sequential command to sublink's i2c-device. The i2c-device is attached on the Vx1 transmitter.

	    @param[in]  uiDevAddr   	7-bit I2C device address.(Exclude the 1bit R/W flag.)
	    @param[in]  uiRegStartAddr  I2C Device sequential Register Starting Address
	    @param[in]  uiLength     	Total Write length in bytes count.
	    @param[in]  uiBufferAddr    Write Data Buffer address

	    @return VX1_I2CSTS
	     - @b VX1_I2CSTS_OK:    I2C command transfer done and OK.
	     - @b VX1_I2CSTS_NACK:  I2C command transfer NACKed.
	     - @b VX1_I2CSTS_ERROR: I2C command transfer fail due to some errors.
	                            Such as driver no open, cable-unplug, uiDevAddr exceed-7bits, ...ect.
	*/
	VX1_I2CSTS (*writeI2cDeviceSequential)(UINT32 uiDevAddr,UINT32 uiRegStartAddr, UINT32 uiLength, UINT32 uiBufferAddr);

	/**
	    Read sublink i2c-device's register value

	    Read sublink i2c-device's register value. The i2c-device is attached on the Vx1 transmitter.

	    @param[in]  uiDevAddr   7-bit I2C device address.(Exclude the 1bit R/W flag.)
	    @param[in]  uiRegAddr   I2C Device Register Address value
	    @param[out] puiData     I2C Device Register Data value

	    @return VX1_I2CSTS
	     - @b VX1_I2CSTS_OK:    I2C command transfer done and OK.
	     - @b VX1_I2CSTS_NACK:  I2C command transfer NACKed.
	     - @b VX1_I2CSTS_ERROR: I2C command transfer fail due to some errors.
	                            Such as driver no open, cable-unplug, uiDevAddr exceed-7bits, ...ect.
	*/
	VX1_I2CSTS(*readI2cDevice)(UINT32 uiDevAddr, UINT32 uiRegAddr, UINT32 *puiData);

	/**
	    Initialize Vx1 transmitter

	    Initialize Vx1 transmitter's operation mode, such as 3/4-Byte Mode
	    and enable the MUX function or not.

	    @param[in] TxModule     Transmitter module select.
	    @param[in] Init         Operation mode select

	    @return
	     - @b E_OK:     Done and success.
	     - @b E_OACV:   Sublink driver not opened.
	     - @b E_NOSPT:  No supported TxModule or Init.
	*/
	ER(*initTransmitter)(VX1_TXMODULE TxModule, VX1_TXINIT Init);

	/**
	    Check Sublink is ready.

	    This API would check the sublink connection is ready on the bus.
	    The application layer may use this as vx1-cable plug/unplug event.
	    After calling this api the vx1/sublink module would be switch to power always on,
	    i.e. driver would not be closed even if calling to vx1_close/vx1_closeSublink.

	    @return
	     - @b TRUE:  Sublink transfer is ready. / Vx1 cable is plugged.
	     - @b FALSE: Sublink transfer is not ready. / Vx1 cable is unplugged.
	*/
	BOOL (*checkSublinkReady)(void);

	/**
	    Configure the GPIO on the Vx1 transmitter

	    Configure the dedicate GPIO on the Vx1 transmitter. Both the GPIO Input/Output are supported.

	    @param[in] TxModule     Transmitter module select.
	    @param[in] TxGpio       Transmitter GPIO pin select.
	    @param[in] GpioDir      Select GPIO input or output Direction.
	    @param[in,out] pbLevel  Input/Output GPIO Level according to GpioDir setup.

	    @return
	     - @b E_OK:     Done and success
	     - @b E_NOSPT:  No supported TxModule.
	*/
	ER(*cfgTransmitterGPIO)(VX1_TXMODULE TxModule, VX1_TXGPIO TxGpio, VX1_GPIODIR GpioDir, BOOL *pbLevel);

	/**
	    Set Vx1 Transmitter THCV241's PLL Settings

	    Set Vx1 Transmitter THCV241's PLL Settings.
	    The input parameter shall contain 8 entries of the PLL settings for THCV241's register
	    [0x1011][0x1012][0x1013][0x1014][0x1015][0x1016][0x1036][0x1037] in fixed order.
	    The PLL Setting values shall be generated from the THINE provided calculation tool.

	    @param[in] pPLL  8 entries of PLL setting value.

	    @return void
	*/
	void (*setThcv241PLL)(UINT8 *pPLL);

	/**
	    Dump Vx1 Mainlink PHY Register Settings

	    Dump Vx1 Mainlink PHY Register Settings. Only the page-0x0 and page-0xc is valid.

	    @param[in]  uiPage  Only the 0x0 and 0xC is meaningful. Others is meaningless.

	    @return void
	*/
	void (*dumpMainlink)(UINT32 uiPage);

	/**
	    Dump Vx1 Sublink Master Register Settings

	    Dump Vx1 Sublink-Master(Embedded in NT96660) Register Settings.

	    @return void
	*/
	void (*dumpSublinkMaster)(void);

	/**
	    Dump Vx1 Sublink Slave Register Settings

	    Dump Vx1 Sublink-Slave(Vx1 Transmitter, such as THCV231/235) Register Settings.

	    @return void
	*/
	void (*dumpSublinkSlave)(void);

	/**
		Bit Error Test (BET)

		Perform specified time for BET test.
		The BET is valid only for Vx1 lane-0. The lane-1(Vx1_2) cannot support BET test.

		@param[in] uiTestTime Specified BET test time.

		@return
	     - @b TRUE:		FAILED.
	     - @b FALSE:  	PASS.
	*/
	BOOL (*bet)(UINT32 uiTestTime);
} VX1OBJ, *PVX1OBJ;



//
//  Extern APIs
//
extern PVX1OBJ          vx1_getDrvObject(VX1_ID Vx1ID);
extern ER 				vx1_cabletest(VX1_ID Vx1ID, BOOL bManual);

//@}

#endif
