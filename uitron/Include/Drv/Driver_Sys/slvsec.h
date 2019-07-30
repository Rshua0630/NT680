/*
    SLVS-EC Controller internal header

    SLVS-EC Controller internal header

    @file       slvsec_int.h
    @ingroup    mIDrvIO_SLVSEC
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _SLVSEC_H
#define _SLVSEC_H

#include "Type.h"

/**
    @addtogroup mIDrvIO_SLVSEC
*/
//@{


/**
	SLVSEC Controller Configuration Options

	This is used at slvsec_setConfig(SLVSEC_CFG_ID)/slvsec_getConfig(SLVSEC_CFG_ID) to assign configurations to HW engine.
*/
typedef enum {
	SLVSEC_CFG_ID_PIXEL_DEPTH,      ///< Set new configuration to Pixel Depth.
									///< Please use "SLVSEC_PIXDEPTH" as input parameter.
	SLVSEC_CFG_ID_DLANE_NUMBER,     ///< Set new configuration to Data Lane Number.
									///< Please use "SLVSEC_DATLANE_NO" as input parameter.
	SLVSEC_CFG_ID_ROUND_ENABLE,     ///< Set rounding function enable/disable. This is valid only when pixel depth larger than 12bits.
									///< Please use ENABLE / DISABLE as input parameter.
	SLVSEC_CFG_ID_VSYNC_ISSUE_MULTI,///< ENABLE/DISABLE the VSYNC issue to SIE1/2 when any Vsync detected.
									///< This function shall be enabled for sensor DOL mode.
	SLVSEC_CFG_ID_DISABLE_SOURCE,   ///< This selects the SLVS-EC DISABLE is depending on the Frame_End_1 or Frame_End_2 event.
									///< TRUE is Frame_End_2 and FALSE is Frame_End_1. The Frame_End_2 should only be used
									///< at Sensor HDR mode. The default value is FLASE (Frame_End_1).
	SLVSEC_CFG_ID_DATAID,           ///< Specified the DATAID of the image to the SIE1 engine. Valid value from 0x0 ~ 0xF.
	SLVSEC_CFG_ID_DATAID2,          ///< Specified the DATAID of the image to the SIE2 engine. Valid value from 0x0 ~ 0xF.
	SLVSEC_CFG_ID_VALID_WIDTH,      ///< Set new configuration to image valid width in pixel count.
									///< Valid value range is from 1 to 32767.
	SLVSEC_CFG_ID_VALID_HEIGHT,     ///< Set new configuration to image valid height for SIE1 path.
									///< Valid value range is from 1 to 65535.
	SLVSEC_CFG_ID_VALID_HEIGHT2,    ///< Set new configuration to image valid height for SIE2 path.
									///< Valid value range is from 1 to 65535.
	SLVSEC_CFG_ID_DATLANE0_SEL,     ///< Select SLVS-EC controller internal data lane 0 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DATLANE1_SEL,     ///< Select SLVS-EC controller internal data lane 1 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DATLANE2_SEL,     ///< Select SLVS-EC controller internal data lane 2 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DATLANE3_SEL,     ///< Select SLVS-EC controller internal data lane 3 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DATLANE4_SEL,     ///< Select SLVS-EC controller internal data lane 4 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DATLANE5_SEL,     ///< Select SLVS-EC controller internal data lane 5 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DATLANE6_SEL,     ///< Select SLVS-EC controller internal data lane 6 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DATLANE7_SEL,     ///< Select SLVS-EC controller internal data lane 7 is from which of the data lane pad pins.
									///< Please use "SLVSEC_PADPIN" as input parameter.
	SLVSEC_CFG_ID_DESKEWCODE,		///< Sensor DeSkew Code setting. Please do not modify this in normal operations.
	SLVSEC_CFG_ID_HALFSPEED,		///< Set ENABLE if the sensor traget outout speed is 1152Mbps. Default is DISABLE means 2304Mbps.
	SLVSEC_CFG_ID_TIMEOUT_PERIOD,	///< slvsec_waitInterrupt() timeout period. Unit in ms. Default is 0 means no timeout.
	SLVSEC_CFG_ID_FORCE_EN,			///< Set controller force enable/disable. Default is DISABLE means wait frame-end to DISABLE.

	SLVSEC_CFG_ID_PATGEN_EN,		// Novatek internal usage only. Pattern Gen enable.
	SLVSEC_CFG_ID_PATGEN_MODE,		// Novatek internal usage only. Pattern Gen mode.
	SLVSEC_CFG_ID_PATGEN_VAL,		// Novatek internal usage only. Pattern Gen value.
	SLVSEC_CFG_ID_MCLKSPEED,        // FPGA Emulation only. Useless for real-chip This is used to calculate internal init parameter.
	SLVSEC_CFG_ID_TEST0,            // novatek internal test only.

	ENUM_DUMMY4WORD(SLVSEC_CFG_ID)
} SLVSEC_CFG_ID;

/**
    SLVS-EC Pixel Depth

    This definition is used in slvsec_setConfig(SLVSEC_CFG_ID_PIXEL_DEPTH)
    to specify the pixel depth of the sensor output.
    Please refer to the driver appilication note for the detail introduction.
    If the used pixel depth is greater than 12 bits, we can enable the controller's
    rounding function to round the bit13 to bit12 by slvsec_setConfig(SLVSEC_CFG_ID_ROUND_ENABLE, ENABLE).
*/
typedef enum {
	SLVSEC_PIXDEPTH_8BIT,         ///< Pixel depth is  8 bits per pixel.
	SLVSEC_PIXDEPTH_10BIT,        ///< Pixel depth is 10 bits per pixel.
	SLVSEC_PIXDEPTH_12BIT,        ///< Pixel depth is 12 bits per pixel.
	SLVSEC_PIXDEPTH_14BIT,        ///< Pixel depth is 14 bits per pixel.
	SLVSEC_PIXDEPTH_16BIT,        ///< Pixel depth is 16 bits per pixel.

	ENUM_DUMMY4WORD(SLVSEC_PIXDEPTH)
} SLVSEC_PIXDEPTH;

/**
    SLVS-EC Data Lane Number

    This definition is used in slvsec_setConfig(SLVSEC_CFG_ID_DLANE_NUMBER)
    to specify the data lane number are used for the SLVS-EC controller.
*/
typedef enum {
	SLVSEC_DATLANE_NO_1,    ///< SLVS-EC use 1 data lane.
	SLVSEC_DATLANE_NO_2,    ///< SLVS-EC use 2 data lane2.
	SLVSEC_DATLANE_NO_4,    ///< SLVS-EC use 4 data lane2.
	SLVSEC_DATLANE_NO_6,    ///< SLVS-EC use 6 data lane2.
	SLVSEC_DATLANE_NO_8,    ///< SLVS-EC use 8 data lane2.

	ENUM_DUMMY4WORD(SLVSEC_DATLANE_NO)
} SLVSEC_DATLANE_NO;


/**
	SLVSEC input pad pin name

	This is used at slvsec_setConfig(SLVSEC_CFG_ID_DATLANEx_SEL) to assign data lane is from which
	pad pin location. This location mapping depends on PCB board circuit layout.
*/
typedef enum {
	SLVSEC_PADPIN_D0,	///< HSI_D0-P/N
	SLVSEC_PADPIN_D1,	///< HSI_D1-P/N
	SLVSEC_PADPIN_D2,	///< HSI_D2-P/N
	SLVSEC_PADPIN_D3,	///< HSI_D3-P/N
	SLVSEC_PADPIN_D4,	///< HSI_D4-P/N
	SLVSEC_PADPIN_D5,	///< HSI_D5-P/N
	SLVSEC_PADPIN_D6,	///< HSI_D6-P/N
	SLVSEC_PADPIN_D7,	///< HSI_D7-P/N

	SLVSEC_PADPIN_MAX,
	ENUM_DUMMY4WORD(SLVSEC_PADPIN)
} SLVSEC_PADPIN;


/**
    SLVSEC Interrupt identification

    This identification can be used in the SLVSEC API slvsec_waitInterrupt() to wait the interrupt is triggered.
    The user must make sure that the driver is opened and set enabled before using slvsec_waitInterrupt().
*/
typedef enum {
	SLVSEC_INTERRUPT_VD             = 0x00000001,       ///< The interrupt is triggered when Vsync event from sensor is got.
	SLVSEC_INTERRUPT_HD             = 0x00000002,       ///< The interrupt is triggered when Hsync event from sensor is got.
	SLVSEC_INTERRUPT_FRMEND         = 0x00000004,       ///< The interrupt is triggered when the final line of the current frame is got from sensor.

	SLVSEC_INTERRUPT_VD2            = 0x00010000,       ///< The interrupt is triggered when Vsync event from sensor is got.
	SLVSEC_INTERRUPT_HD2            = 0x00020000,       ///< The interrupt is triggered when Hsync event from sensor is got.
	SLVSEC_INTERRUPT_FRMEND2        = 0x00040000,       ///< The interrupt is triggered when the final line of the current frame is got from sensor.

	SLVSEC_INTERRUPT_ABORT			= 0x80000000,
	SLVSEC_INTERRUPT_DISABLE_DONE   = 0xFFFFFFFF,		///< After set controller DISABLE, this is used to wait controller stopped.

	SLVSEC_INTERRUPT_ALL            = 0x00070007,
	ENUM_DUMMY4WORD(SLVSEC_INTERRUPT)
} SLVSEC_INTERRUPT;



//
//  SLVS-EC Exporting APIs
//

/**
    Open SLVS-EC driver

    Open the SLVS-EC driver and initialize the related resource, such as source clock, analog logic power,
    and the OS resource. The user must use this API first for SLVS-EC engine.

    @return
     - @b E_OK: SLVS-EC driver open done and success.
*/
extern ER               slvsec_open(void);

/**
    Close SLVS-EC Driver

    Close the SLVS-EC driver and also close the related resource, such as source clock, analog logic power,
    and the OS resource. The user must also remember to disable the SLVS-EC controller before closing
    the SLVS-EC driver by this API to insure the sensor interface is terminated in normal operation.

    @return
     - @b E_OK: SLVS-EC close done and success.
*/
extern ER               slvsec_close(void);

/**
    Check if the SLVS-EC driver is opened or not.

    Check if the SLVS-EC driver is opened or not.

    @return
     - @b TRUE:  SLVS-EC driver is already opened.
     - @b FALSE: SLVS-EC driver has not opened.
*/
extern BOOL             slvsec_isOpened(void);

/**
	SLVSEC Post Initialization

	This API shall be invoked after sensor streaming enabled.
	This would enable the error handling interrupt in the HW engine.

    @return void
*/
extern void             slvsec_postInit(void);


/**
    Set SLVS-EC Engine Enable/Disable

    This api is used to start or stop the SLVS-EC engine.
    After disabling the SLVS-EC engine, the LVDS would be stopped during this frame receiving end
    and the enable bit would be cleared at this frame end.
    After disabling the SLVS-EC engine, the user can use slvsec_waitInterrupt(SLVSEC_INTERRUPT_FRMEND)
    to make sure the engine is disabled.

    @param[in] bEn
     - @b TRUE:   Enable the SLVS-EC engine immediately.
     - @b FALSE:  Disable the SLVS-EC engine at the current frame end.

    @return
     - @b E_OK:  Done and success.
     - @b E_SYS: SLVS-EC driver has not opened.
*/
extern ER               slvsec_setEnable(BOOL bEn);

/**
    Check the SLVS-EC engine is enabled or not

    Check the SLVS-EC engine is enabled or not.

    @return
     - @b TRUE:  The SLVS-EC engine is already enabled.
     - @b FALSE: The SLVS-EC engine is disabled.
*/
extern BOOL             slvsec_getEnable(void);

/**
    Set SLVS-EC engine general configurations

    This api is used to configure the general function of the SLVS-EC engine,
    such as DataLane number, pixel format, ...etc.
    This API can only be used under SLVS-EC engine disabled,
    and should not be changed dynamically under engine working state.

    @param[in] ConfigID     The configuration selection ID. Please refer to SLVSEC_CONFIG_ID for details.
    @param[in] uiCfgValue   The configuration parameter according to the ConfigID.

    @return void
*/
extern void             slvsec_setConfig(SLVSEC_CFG_ID ConfigID, UINT32 uiCfgValue);

/**
    Get SLVS-EC engine general configurations

    This api is used to get the general function of the SLVS-EC engine,
    such as DataLane number, pixel format, ...etc.
    This API can only be used under SLVS-EC engine disabled,
    and should not be changed dynamically under engine working state.

    @param[in] ConfigID     The configuration selection ID. Please refer to SLVSEC_CONFIG_ID for details.

    @return void
*/
extern UINT32           slvsec_getConfig(SLVSEC_CFG_ID ConfigID);

/**
    Wait the SLVSEC interrupt event

    Wait the SLVSEC specified interrupt event. When the interrupt event is triggered and match the wanted events,
    the waited event flag would be returned.

    @param[in] WaitedFlag  The bit-wise OR of the waited interrupt events.

    @return The waited interrupt events.
*/
extern SLVSEC_INTERRUPT slvsec_waitInterrupt(SLVSEC_INTERRUPT WaitFlag);

/*
	Dump the PHY configuration to console

	This API would dump the SLVSEC PHY config register values to UART console.

	@return void
*/
extern void             slvsec_dumpPHY(void);

//@}
#endif
