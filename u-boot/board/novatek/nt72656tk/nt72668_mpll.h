/*
 *  board/novatek/evb668/nt72668_mpll.h
 *
 *  Author:	Alvin lin
 *  Created:	Jun 3, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef __NT72668_MPLL_H__
#define __NT72668_MPLL_H__

#define SYS_CLK_REG_BASE                (0xfd020000)

#define _SYS_MPLL_PAGE_B_EN	0x00000001
#define _SYS_MPLL_PAGE_0_EN	0x00000002

#define AHB_CLOCK_MASK1		*((volatile unsigned long *) (SYS_CLK_REG_BASE + 0x0000))
#define AHB_CLK_TESTPBUS	(1UL << 8)

/* System MPLL register */
#define SYS_CLOCK_SELECT	*((volatile unsigned long *) (SYS_CLK_REG_BASE + 0x0030))
#define SYS_CLOCK_SELECT2	*((volatile unsigned long *) (SYS_CLK_REG_BASE + 0x0034))
#define AXI_CLK_SEL			((SYS_CLOCK_SELECT 	>> 8) & 0x3)
#define AHB_CLK_SEL			((SYS_CLOCK_SELECT 	>> 5) & 0x3)
#define DSP_CLK_SEL			((SYS_CLOCK_SELECT 	>> 2) & 0x7)
#define MIPS_CLK_SEL		((SYS_CLOCK_SELECT	>> 0) & 0x3)
#define AHB_HALF_SEL		((SYS_CLOCK_SELECT2 >> 2) & 0x1)

#define _CPU_PLL_OFFSET	0x62	
#define _AXI_PLL_OFFSET	0x6a
#define _DDR_PLL_OFFSET	0x82
#define _MPG_PLL_OFFSET	0x9a
#define _AHB_PLL_OFFSET	0x9d
#define _ETH_PLL_OFFSET	0xaa
#define _MMC_PLL_OFFSET	0xc4

/**
 * @hideinitializer
 */
#define MPLL_BASE_ADDR			(0xfd670000)
#define APLL_BASE_ADDR			(0xfd720000)
#define SYS_MPLL_PAGE_EN	*((volatile unsigned long *)(SYS_CLK_REG_BASE + 0xBC))
#define SYS_APLL_PAGE_EN	*((volatile unsigned long *)(0xfc0f0c18))

#define _AHB_CLK_SEL (((*((volatile unsigned long *)(SYS_CLK_REG_BASE + 0x30))) >> 5) & 0x3)
#define _AXI_CLK_SEL (((*((volatile unsigned long *)(SYS_CLK_REG_BASE + 0x30))) >> 8) & 0x3) 

#define _MPLL_SetData(u8Offset, u8Value) \
({ \
    (*((volatile unsigned long *)(MPLL_BASE_ADDR + (u8Offset * 4)))) = u8Value; \
 })

#define _MPLL_GetData(u8Offset) \
({ \
    (*((volatile unsigned long *)(MPLL_BASE_ADDR + (u8Offset * 4)))); \
 })

#define _APLL_SetData(u8Offset, u8Value) \
({ \
    (*((volatile unsigned long *)(APLL_BASE_ADDR + (u8Offset * 4)))) = u8Value; \
 })

#define _APLL_GetData(u8Offset) \
({ \
    (*((volatile unsigned long *)(APLL_BASE_ADDR + (u8Offset * 4)))); \
 })

/**
 * @fn void _MPLL_EnablePageB(void)
 * @brief Enable MPLL Page B 
 */
#define _MPLL_EnablePageB() \
    SYS_MPLL_PAGE_EN = _SYS_MPLL_PAGE_B_EN;

/**
 * @fn void _APLL_EnablePage0(void)
 * @brief Enable MPLL Page 0
 */
#define _APLL_EnablePage0() \
    SYS_APLL_PAGE_EN = 0x1;

typedef enum _MPLL_OFF_e
{
	EN_MPLL_OFF_ARM		= 0x62,	///< ARM
	EN_MPLL_OFF_AHB		= 0x9d,	///< AHB
	EN_MPLL_OFF_AXI		= 0x6a,	///< AXI
	EN_MPLL_OFF_ETH		= 0xaa,	///< Ethernet
	EN_MPLL_OFF_USB_20	= 0x8a,	///< USB 2.0
	EN_MPLL_OFF_USB_30	= 0x15,	///< USB 3.0
	EN_MPLL_OFF_EMMC	= 0xc4,	///< EMMC
	EN_MPLL_OFF_DDR		= 0x82,	///< DDR
} MPLL_OFF;

/**
 * @internal
 * @ingroup type
 *
 * @defgroup AHBPLL Enumeration of AHB clock clock source
 * @{
 *
 * @typedef EN_SYS_AHB_CLK_SRC
 *
 * A enumeration for AHB clock source select 
 */
typedef enum _EN_SYS_AHB_CLK_SRC
{
	///< AHB clock source select
    EN_SYS_AHB_CLK_SRC_REF_96M	= 0b00,	///< 2'b00: refserence clock 96M 
    EN_SYS_AHB_CLK_SRC_ARM_D8	= 0b01,	///< 2'b01: ARM PLL div 8
    EN_SYS_AHB_CLK_SRC_AHB		= 0b10,	///< 2'b1x: AHB PLL
} EN_SYS_AHB_CLK_SRC;

/**
 * @internal
 * @ingroup type
 *
 * @defgroup AXIPLL Enumeration of AXI clock clock source
 * @{
 *
 * @typedef EN_SYS_AXI_CLK_SRC
 *
 * A enumeration for AXI clock source select 
 */
typedef enum _EN_SYS_AXI_CLK_SRC
{
	///< AHB clock source select
    EN_SYS_AXI_CLK_SRC_ARM_D8	= 0b00,	///< 2'b00: ARM PLL div 8 
    EN_SYS_AXI_CLK_SRC_DDR_D2	= 0b01,	///< 2'b01: DDR PLL div 2
    EN_SYS_AXI_CLK_SRC_AXI_D2	= 0b10,	///< 2'b1x: AXI PLL div 2
    EN_SYS_AXI_CLK_SRC_AXI		= 0b11,	///< 2'b1x: AXI PLL
} EN_SYS_AXI_CLK_SRC;

/**
 * @fn void set_arm_clk(unsigned long freq)
 * @brief Initial and set ARMPLL by specified frequecy  
 *
 * @param freq Specified frequency (MHz)
 */
void set_arm_clk(unsigned long freq);

/**
 * @fn unsigned long get_cpu_clk(void)
 * @brief Get frequency of ARM 
 *
 * @return Frequency (KHz) of ARM
 */
unsigned long get_cpu_clk(void);

/**
 * @fn unsigned long get_sys_mpll(unsigned long off)
 * @brief Get frequency of specified MPLL by beginning offset of MPLL 
 *
 * @param off Beginning offset of MPLL page B
 * @return Frequency (kHz) of specified MPLL
 */
unsigned long get_sys_mpll(unsigned long off);

/**
 * @fn void set_sys_mpll(unsigned long off, unsigned long val)
 * @brief Set specified frequency of specified MPLL by beginning offset of MPLL 
 *
 * @param off Beginning offset of MPLL page B
 * @param val Specified frequecy (MHz)
 */
void set_sys_mpll(unsigned long off, unsigned long val);

/**
 * @brief Get frequency of AHB clock
 *
 * @return Frequency (Hz) of AHB clock
 */
unsigned long get_ahb_clk(void);


unsigned long get_emmc_clk(void);

unsigned long get_axi_clk(void);

unsigned long get_ddr_clk(void);
#endif

