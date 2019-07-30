/*
 *  board/novatek/evb668/nt72668_mpll.c
 *
 *  Author:	Alvin lin
 *  Created:	Jun 3, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#include <common.h>
#include <command.h>
#include "nt72668_mpll.h"

#define FREQ_CPU		500000

unsigned long aclk = 0;	// ARM frequecy
unsigned long emmc_clk = 0;	// emmc frequecy (Hz)

/**
 * @fn unsigned long get_arm_clk(void)
 * @brief Get frequency of ARM 
 *
 * @return Frequency (Hz) of ARM
 */
unsigned long get_arm_clk(void)
{
	aclk = get_sys_mpll(_CPU_PLL_OFFSET);

	_APLL_EnablePage0();
	if(_APLL_GetData(0x00) & 0x1)
		aclk *= 8;

	return ((aclk) ? (aclk *1000):(FREQ_CPU * 1000));
}
#if 0
/**
 * @fn unsigned long get_sys_mpll(unsigned long off)
 * @brief Get frequency of specified MPLL by beginning offset of MPLL 
 *
 * @param off Beginning offset of MPLL page B
 * @return Frequency (MHz) of specified MPLL
 */
unsigned long get_sys_mpll(unsigned long off)
{
	unsigned long rVal;

	_MPLL_EnablePageB();
	rVal = _MPLL_GetData(off);
	rVal |= (_MPLL_GetData((off + 1)) << 8);
	rVal |= (_MPLL_GetData((off + 2)) << 16);

	rVal *= 12;
	rVal += ((1UL << 17) - 1);
	rVal >>= 17;

	return (unsigned long)rVal;
}

/**
 * @fn void set_sys_mpll(unsigned long off, unsigned long val)
 * @brief Set specified frequency of specified MPLL by beginning offset of MPLL 
 *
 * @param off Beginning offset of MPLL page B
 * @param val Specified frequecy (MHz)
 */
void set_sys_mpll(unsigned long off, unsigned long val)
{
	val <<= 17;
	val += (12 - 1);
	val /=12;

	_MPLL_EnablePageB();
	_MPLL_SetData((off + 0), ((val >> 0) & 0xff));
	_MPLL_SetData((off + 1), ((val >> 8) & 0xff));
	_MPLL_SetData((off + 2), ((val >> 16) & 0xff));
}
#endif
/**
 * @brief Get frequency of AHB clock
 *
 * @return Frequency (Hz) of AHB clock
 */
unsigned long get_ahb_clk(void)
{
	unsigned long ratio = AHB_CLK_SEL;
	unsigned long hclk;

  
	if((ratio == 0))
	{	
		//!< Source is OSC16X/2
		hclk = 96000000;	//!< 3'bx00: OSC16X/2
	}
	else if(ratio == 1)
	{
		//!< Source is ARM_D8CK
		hclk = get_arm_clk();
		hclk *= 1000;
		hclk /= 8;
	}
	else //!< if((ratio == 2) || (ratio == 3))
	{
		//!< Source is AHB_CK
		hclk = get_sys_mpll(_AHB_PLL_OFFSET);
		hclk *= 1000000;
	}

	return hclk;
}

/**
 * @brief Get frequency of emmc clock
 *
 * @return Frequency (Hz) of emmc clock
 */
unsigned long get_emmc_clk(void)
{
	emmc_clk = get_sys_mpll(_MMC_PLL_OFFSET);
	emmc_clk *= 1000000;
	emmc_clk *= 4;	///< emmc MPLL = emmc MPLL ratio X 4

	return emmc_clk;
}

static unsigned long axi_clk, arm_clk ,ddr_clk;

#if 0
static void compute_ahb_clk(void)
{
	if(get_sys_mpll(0x6c))	///< Real Chip
	{
		switch(_AHB_CLK_SEL)
		{
			case EN_SYS_AHB_CLK_SRC_REF_96M:	///< OSC16X/2
				ahb_clk = 96000000;

				break;

			case EN_SYS_AHB_CLK_SRC_ARM_D8:		///< ARM_D8CK
				ahb_clk = (arm_clk / 8);

				break;

			case EN_SYS_AHB_CLK_SRC_AHB:		///< AHB_CK
			default:
				ahb_clk = SYS_CLK_GetMpll(EN_MPLL_OFF_AHB);
				ahb_clk *= 1000000;

				break;
		}
	}
	else
	{
		ahb_clk = 12000000;
	}
}
#endif

static void compute_axi_clk(void)
{
	if(get_sys_mpll(0x6c))	///< Real Chip
	{
		switch(_AXI_CLK_SEL)
		{
			case EN_SYS_AXI_CLK_SRC_ARM_D8:	///< ARM_D8CK
				axi_clk = (arm_clk / 8);

				break;

			case EN_SYS_AXI_CLK_SRC_DDR_D2:	///< DDR_D2CK
				axi_clk = (get_sys_mpll(EN_MPLL_OFF_DDR) / 2);;
				axi_clk *= 1000000;

				break;

			case EN_SYS_AXI_CLK_SRC_AXI_D2:	///< AXI_CLK/2
			case EN_SYS_AXI_CLK_SRC_AXI: 	///< AXI_CLK
				axi_clk = (get_sys_mpll(EN_MPLL_OFF_AXI) / (4 - _AXI_CLK_SEL));
				axi_clk *= 1000000;

				break;

			default:
				printf("Invalid AXI clock selection\n");
				break;
		}
	}
	else
	{
		axi_clk = 27000000;
	}
}


static void compute_cpu_clk(void)
{

	if(get_sys_mpll(0x6c))	///< Real Chip
	{
		/**
		 * Get ratio of ARM PLL
		 */
		arm_clk = get_sys_mpll(EN_MPLL_OFF_ARM);

		/**
		 * Check MUX
		 */
		_APLL_EnablePage0();
		if((_APLL_GetData(0x00) & 0x1))	///< Select local PLL
		{
			printf("\tSelect local PLL\n");
			arm_clk *= 8;
		}
		else
		{
			printf("\tSelect MPLL\n");
		}

		arm_clk *= 1000000;
	}
	else
	{
		arm_clk = 55000000;
	}
}


unsigned long get_axi_clk(void)
{
	if(axi_clk == 0)
		compute_axi_clk();

	return axi_clk;
}

unsigned long get_ddr_clk(void)
{
	if (ddr_clk == 0)
		ddr_clk = get_sys_mpll(_DDR_PLL_OFFSET);

	return (ddr_clk << 2);
}
