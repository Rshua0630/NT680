/**
    DSP module driver.

    This file is the driver of DSP module.

    @file       DSP.h
    @ingroup    mIDrvSys_DSP
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _DSP_H
#define _DSP_H

//#include "Type.h"

/**
    DSP ID

*/
//@{
typedef enum
{
    DSP_ID_1,                          ///< DSP Controller
    DSP_ID_2,                          ///< DSP2 Controller

    ENUM_DUMMY4WORD(DSP_ID)
} DSP_ID;
//@}

/**
    DSP BOOT mode
*/
//@{
typedef enum
{
    DSP_BOOTMODE_RESET_SEQ,           ///< DSP BOOT mode: reset sequence
    DSP_BOOTMODE_BOOT_SEQ,            ///< DSP BOOT mode: boot sequence

    ENUM_DUMMY4WORD(DSP_BOOT_MODE)
} DSP_BOOT_MODE;



typedef enum
{
    DSP_CONFIG_ID_BOOTVECTOR,         ///< Configure boot vector
    DSP_CONFIG_ID_BOOTMODE,           ///< Configure boot mode

    DSP_CONFIG_ID_DERESET,            ///< Configure de-reset (core, sys, ocem, global)


    DSP_CONFIG_ID_DEBUGPORTEN,        ///< Coinfig debug port enable
    DSP_CONFIG_ID_DEBUGPORTSEL,       ///< Coinfig debug port selection

	DSP_CONFIG_ID_EPP_POSTD,          ///< Config EPP POSTD
	DSP_CONFIG_ID_EPP_PAWBA_MODE,     ///< Config EPP PAWBA mode
	DSP_CONFIG_ID_EPP_AWBA_SET,       ///< Config EPP AWBA
	DSP_CONFIG_ID_EPP_PPRE_RD_OFF,    ///< Config EPP PPRE_RD_OFF

	DSP_CONFIG_ID_EDP_POSTD,          ///< Config EDP POSTD
	DSP_CONFIG_ID_EDP_PAWBA_MODE,     ///< Config EDP PAWBA mode
	DSP_CONFIG_ID_EDP_AWBA_SET,       ///< Config EDP AWBA
	DSP_CONFIG_ID_EDP_PPRE_RD_OFF,    ///< Config EDP PPRE_RD_OFF

    ENUM_DUMMY4WORD(DSP_CONFIG_ID)
}DSP_CONFIG_ID;


extern ER dsp_open(DSP_ID id);
extern ER dsp_close(DSP_ID id);
extern BOOL dsp_isOpened(DSP_ID id);
extern ER dsp_setConfig(DSP_ID id, DSP_CONFIG_ID ConfigID, UINT32 uiCfgValue);




#endif
