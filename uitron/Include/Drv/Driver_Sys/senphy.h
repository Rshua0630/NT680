/*
    LVDS/MIPI-CSI/HiSPi Sensor PHY Configuration Driver Global header

    LVDS/MIPI-CSI/HiSPi Sensor PHY Configuration Driver Global header

    @file       senphy.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _SENPHY_H
#define _SENPHY_H

#include "Type.h"


/**
    Sensor PHY configuration Selection

    This is used in senphy_setConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	SENPHY_CONFIG_ID_ENO_DLY,       ///< (Clock) Lane Enable to Valid Output Delay Count in (60/80/120)MHz.

	SENPHY_CONFIG_ID_DLY_EN,        ///< Set new PHY Delay/DeSkew Function Enable/Disable.
	SENPHY_CONFIG_ID_DLY_CLK0,      ///< Clock Lane 0 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK1,      ///< Clock Lane 1 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK2,      ///< Clock Lane 2 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK3,      ///< Clock Lane 3 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK4,      ///< Clock Lane 4 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK5,      ///< Clock Lane 5 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK6,      ///< Clock Lane 6 analog Delay Unit select. Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_CLK7,      ///< Clock Lane 7 analog Delay Unit select. Valid value range from 0x0 to 0x7.

	SENPHY_CONFIG_ID_DLY_DAT0,      ///< Data Lane 0 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT1,      ///< Data Lane 1 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT2,      ///< Data Lane 2 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT3,      ///< Data Lane 3 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT4,      ///< Data Lane 4 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT5,      ///< Data Lane 5 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT6,      ///< Data Lane 6 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
	SENPHY_CONFIG_ID_DLY_DAT7,      ///< Data Lane 7 analog Delay Unit select.  Valid value range from 0x0 to 0x7.

	SENPHY_CONFIG_ID_IADJ,          ///< Analog Block Current Source adjustment.
	///< Set 0x0 is 20uA (Default value). Set 0x1 is 40uA. 0x2 is 30uA. 0x3 is 60uA.
	SENPHY_CONFIG_ID_CURRDIV2,      ///< Current Divide by 2 function ENABLE/DISABLE.

	SENPHY_CONFIG_ID_INV_DAT0,      ///< Data Lane 0 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT1,      ///< Data Lane 1 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT2,      ///< Data Lane 2 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT3,      ///< Data Lane 3 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT4,      ///< Data Lane 4 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT5,      ///< Data Lane 5 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT6,      ///< Data Lane 6 pad pin P/N Swap Enable/Disable.
	SENPHY_CONFIG_ID_INV_DAT7,      ///< Data Lane 7 pad pin P/N Swap Enable/Disable.

	SENPHY_CONFIG_ID_DBGMSG,        //   Sensor PHY Module Debug Message Enable/Disable. Default is disabled.
	ENUM_DUMMY4WORD(SENPHY_CONFIG_ID)
} SENPHY_CONFIG_ID;



extern ER       senphy_setConfig(SENPHY_CONFIG_ID ConfigID, UINT32 uiCfgValue);

#endif
