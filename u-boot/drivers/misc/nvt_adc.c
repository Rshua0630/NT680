/*
 * Copyright (C) 2016 Novatek MicroElectronics Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <asm/io.h>

#include <asm/arch/adc.h>
#include <asm/arch/rcw_macro.h>
#include <asm/arch/IOAddress.h>

#define CALI_VDDADC 2500
#define CALI_OFFSET 0

#define ADC_GETREG(ofs)        INW(IOADDR_ADC_REG_BASE+(ofs))
#define ADC_SETREG(ofs, value) OUTW(IOADDR_ADC_REG_BASE +(ofs),(value))

#define CG_GETREG(ofs)        INW(IOADDR_CG_REG_BASE+(ofs))
#define CG_SETREG(ofs, value) OUTW(IOADDR_CG_REG_BASE +(ofs),(value))

#define ADC_CTRL_REG_OFS          0x00
#define ADC_INTCTRL_REG_OFS       0x08
#define ADC_AIN0_DATA_REG_OFS     0x30

#define CG_ENABLE_REG_OFS     0x84
#define CG_RESET_REG_OFS     0xA4
#define ADC_BIT (1 << 13)

#define ADC_CHANNEL_NR 8

void nvt_adc_open(void)
{
	u32  reg_data;

	/*Enable ADC CG*/
	reg_data = CG_GETREG(CG_ENABLE_REG_OFS);
	reg_data |= ADC_BIT;
	CG_SETREG(CG_ENABLE_REG_OFS, reg_data);

	/*Release ADC Reset*/
	reg_data = CG_GETREG(CG_RESET_REG_OFS);
	reg_data |= ADC_BIT;
	CG_SETREG(CG_RESET_REG_OFS, reg_data);

	/*Diable interrupt*/
	ADC_SETREG(ADC_INTCTRL_REG_OFS, 0x0);

	/*Set continuous mode and trigger enable*/
	reg_data = ADC_GETREG(ADC_CTRL_REG_OFS);
	reg_data |= 0x8000FF;
	ADC_SETREG(ADC_CTRL_REG_OFS, reg_data);

	/*Delay for data stable*/
	mdelay(1);
}

u32 nvt_adc_get_value(u8 channel)
{
	u16 adc_data;

	if (channel < ADC_CHANNEL_NR) {
		adc_data = ADC_GETREG(ADC_AIN0_DATA_REG_OFS + channel*4);
		return (adc_data+CALI_OFFSET)*CALI_VDDADC/511;
	} else {
		printf("Not support channel %d\n", channel);
		return -E_NOSPT;
	}
}

void nvt_adc_close(void)
{
	u32  reg_data;

	/*Disable ADC CG*/
	reg_data = CG_GETREG(CG_ENABLE_REG_OFS);
	reg_data &= ~ADC_BIT;
	CG_SETREG(CG_ENABLE_REG_OFS, reg_data);

	/*Pull ADC Reset*/
	reg_data = CG_GETREG(CG_RESET_REG_OFS);
	reg_data &= ~ADC_BIT;
	CG_SETREG(CG_RESET_REG_OFS, reg_data);

	/*Release ADC Reset*/
	reg_data = CG_GETREG(CG_RESET_REG_OFS);
	reg_data |= ADC_BIT;
	CG_SETREG(CG_RESET_REG_OFS, reg_data);
}