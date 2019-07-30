/*
    Display object object Golden Sample for driving RGB 8bits panel

    @file       dispdev_gsinf8bits.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DISPDEV_GSINF8BITS_H
#define _DISPDEV_GSINF8BITS_H

#include "sif.h"
#include "dispdev_panlcomn.h"
#include "gpio.h"
#include "Utility.h"

// Default debug level
#ifndef __DBGLVL__
#define __DBGLVL__  2       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
#define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"

/* Command code for adding delay between commands */
#define CMDDELAY_MS 0x80000000
#define CMDDELAY_US 0x40000000


//
//  GPIO SIF Mode Select
//
//@{
/* Check SIF of Settings */

#define OPTION_SIF_0000  0x0   ///< SIF bus mode 0000
#define OPTION_SIF_0001  0x1   ///< SIF bus mode 0001
#define OPTION_SIF_0010  0x2   ///< SIF bus mode 0010
#define OPTION_SIF_0011  0x3   ///< SIF bus mode 0011
#define OPTION_SIF_0100  0x4   ///< SIF bus mode 0100
#define OPTION_SIF_0101  0x5   ///< SIF bus mode 0101
#define OPTION_SIF_0110  0x6   ///< SIF bus mode 0110
#define OPTION_SIF_0111  0x7   ///< SIF bus mode 0111
#define OPTION_SIF_1000  0x8   ///< SIF bus mode 1000
#define OPTION_SIF_1001  0x9   ///< SIF bus mode 1001
#define OPTION_SIF_1010  0xA   ///< SIF bus mode 1010
#define OPTION_SIF_1011  0xB   ///< SIF bus mode 1011
#define OPTION_SIF_1100  0xC   ///< SIF bus mode 1100
#define OPTION_SIF_1101  0xD   ///< SIF bus mode 1101
#define OPTION_SIF_1110  0xE   ///< SIF bus mode 1110
#define OPTION_SIF_1111  0xF   ///< SIF bus mode 1111

#if     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUCN01)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLY99000543_636)
#define SETTING_SIF_MODE                    OPTION_SIF_0100
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUA027DN01)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUA027DN03)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_AUCN02)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_PS20P00)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_RZWT27P02)
#define SETTING_SIF_MODE                    OPTION_SIF_0100
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLY99000734)
#define SETTING_SIF_MODE                    OPTION_SIF_0100
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLYA3AA)
#define SETTING_SIF_MODE                    OPTION_SIF_0100
#elif     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLYTD025)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TOPPOLYTD030WHEA1)
#define SETTING_SIF_MODE                    OPTION_SIF_0100
#elif     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TXDT240C)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TXDT270M)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif     (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_WMF4823V47FLW)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_TXDT270CDR_2V7)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_ILI9341)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#elif   (DISPDEV_IF8BITS_TYPE == DISPDEV_IF8BITS_D200N9309V3_ST7789V)
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#else
#define SETTING_SIF_MODE                    OPTION_SIF_0000
#endif






#if (SETTING_SIF_MODE==OPTION_SIF_0000)     //Confirmed
#define USED_SIF_MODE     SIF_MODE_00
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0001)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_01
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0010)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_02
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0011)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_03
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0100)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_04
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0101)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_05
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0110)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_06
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0111)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_07
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_1000)   //Confirmed
#define USED_SIF_MODE     SIF_MODE_08
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#else
#error("This SIF Mode has not implement!");
#endif
//@}







tLCD_PARAM     *dispdev_getConfigMode(UINT32 *ModeNumber);
tPANEL_CMD     *dispdev_getStandbyCmd(UINT32 *CmdNumber);

void            dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
void            dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
void            dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *value);

tLCD_ROT       *dispdev_getLcdRotateCmd(UINT32 *ModeNumber);
tLCD_SIF_PARAM *dispdev_getLcdSifParam(void) __attribute__((weak));

tLCD_PARAM     *dispdev_getConfigMode2(UINT32 *ModeNumber);
tPANEL_CMD     *dispdev_getStandbyCmd2(UINT32 *CmdNumber);

void            dispdev_writeToLcdSif2(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
void            dispdev_writeToLcdGpio2(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
void            dispdev_readFromLcdGpio2(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *value);

tLCD_ROT       *dispdev_getLcdRotateCmd2(UINT32 *ModeNumber);
tLCD_SIF_PARAM *dispdev_getLcdSifParam2(void) __attribute__((weak));


#endif
