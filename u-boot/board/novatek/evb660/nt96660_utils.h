/*
 *  board/nvt/evb660/nt96660_utils.h
 *
 *  Author:	Alvin lin
 *  Created:	Dec 18, 2011
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NT96660_UTILS_H__
#define __NT96660_UTILS_H__

typedef enum {
	BOOT_SPI,
	BOOT_NAND,
	BOOT_EMMC,
	BOOT_RAM,	//Use LD96660 to pre-load
	BOOT_MAX,
}NT96660_BOOT_TYPE;

#if 0
typedef enum {
	MPLL_PAGE_B,	
	MPLL_PAGE_0
}NT72568_MPLL_PAGE_TYPE;
#endif

NT96660_BOOT_TYPE nt96660_get_boot_type(void);

#if 0
void nt72568_pll_set(NT72568_MPLL_PAGE_TYPE type,u32 addr, u32 val);

void nt72568_jtag_disable(void);

void nt72568_adc_fwupdate(void);
#endif

#endif
