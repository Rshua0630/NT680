/**
    Change clock APIs header file

    Change clock APIs header.

    @file       clock.h
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _CLOCK_H
#define _CLOCK_H

#include "Type.h"

/**
    @addtogroup mIDrvSys_CG
*/
//@{


/**
    DMA frequency level

    @note For clk_changeDMA()
*/
typedef enum {
	CLK_DMA_FREQ_LEVEL_0,               ///< level 0 (normal frequency)
	CLK_DMA_FREQ_LEVEL_1,               ///< level 1 (obsolete)


	CLK_DMA_FREQ_CNT,
	ENUM_DUMMY4WORD(CLK_DMA_FREQ)
} CLK_DMA_FREQ;


/**
    Power Down mode definition

    @note This definition is used for clk_powerdown() & clk_changeCPUAHB().
*/
typedef enum {
	CLK_PDN_MODE_CLKSCALING,            ///< Clock scaling mode
	CLK_PDN_MODE_SLEEP1,                ///< Sleep mode 1
	CLK_PDN_MODE_SLEEP2,                ///< Sleep mode 2
	CLK_PDN_MODE_SLEEP3,                ///< Sleep mode 3

    CLK_PDN_MODE_SYSOFF_DRAMON,         ///< power off but DRAM keep self refresh
	CLK_PDN_MODE_POWEROFF,              ///< Sleep mode with ALL power off

	ENUM_DUMMY4WORD(CLK_PDN_MODE)
} CLK_PDN_MODE;

/**
    CLKGEN configuration identifier

    @note For clk_setConfig()
*/
typedef enum {
	CLK_CONFIG_ID_ADC_WAKEUP,               ///< Enable ADC as CLKGEN wakeup source
	///< Context can be:
	///< - @b TRUE: Enable ADC as CLKGEN wakeup source (Default value)
	///< - @b FALSE: Disable ADC as CLKGEN wakeup source
	CLK_CONFIG_ID_GPIO_WAKEUP,              ///< Enable GPIO as CLKGEN wakeup source
	///< Context can be:
	///< - @b TRUE: Enable GPIO as CLKGEN wakeup source (Default value)
	///< - @b FALSE: Disable GPIO as CLKGEN wakeup source
	CLK_CONFIG_ID_RM_WAKEUP,                ///< Enable RM as CLKGEN wakeup source
	///< Context can be:
	///< - @b TRUE: Enable RM as CLKGEN wakeup source (Default value)
	///< - @b FALSE: Disable RM as CLKGEN wakeup source
	CLK_CONFIG_ID_RTC_WAKEUP,               ///< Enable RTC as CLKGEN wakeup source
	///< Context can be:
	///< - @b TRUE: Enable RTC as CLKGEN wakeup source (Default value)
	///< - @b FALSE: Disable RTC as CLKGEN wakeup source
	CLK_CONFIG_ID_USB_WAKEUP,               ///< Enable USB as CLKGEN wakeup source
	///< Context can be:
	///< - @b TRUE: Enable USB as CLKGEN wakeup source (Default value)
	///< - @b FALSE: Disable USB as CLKGEN wakeup source

	CLK_CONFIG_ID_CNT,
	ENUM_DUMMY4WORD(CLK_CONFIG_ID)
} CLK_CONFIG_ID;


extern ER       	clk_open(void);
extern ER       	clk_close(void);

extern BOOL     	clk_changeDMA(CLK_DMA_FREQ clkDmaFreq);
extern BOOL     	clk_changeCPUAHB(UINT32 uiCPUClk, UINT32 uiAHBClk);
extern BOOL     	clk_changeAPB(UINT32 uiAPBClk);
extern BOOL         clk_pre_powerdown(CLK_PDN_MODE uiMode);
extern BOOL     	clk_powerdown(CLK_PDN_MODE uiMode);
extern void     	clk_waitFlag(void);
extern void     	clk_clearFlag(void);
extern ER       	clk_setConfig(CLK_CONFIG_ID configID, UINT32 configContext);
extern UINT32   	clk_getDMAFreq(CLK_DMA_FREQ clkDmaFreq);
extern void 		clk_set_reload_fw_addr(UINT32 fw_addr, UINT32 fw_size);
extern ER 			clk_set_reload_apb_register(UINT32 register_addr, UINT32 *register_size);
extern void 		clk_get_reload_apb_register(UINT32 *register_addr, UINT32 *register_size);
extern void 		clk_reload_apb_register(UINT32 reg_addr, UINT32 reg_size);

extern CLK_DMA_FREQ clk_getCurrDMALevel(void);
//@}

#endif
