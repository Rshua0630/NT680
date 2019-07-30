/*
	io module driver.

	This file is the driver of io module.

	@file		iocfg.c
	@ingroup
	@note		Nothing.

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include <common.h>
#include <asm/io.h>
#include <modelext/gpio_info.h>
#include "iocfg_int.h"


static ER pad_reg_lookup(u32 pin, u32 *dw_ofs, u32 *bit_ofs)
{
	u32 i, len;

	len = sizeof(pad_pullupdown_table) / sizeof(struct pad_pullupdown_pin);
	for (i = 0; i < len; i++) {
		if (pad_pullupdown_table[i].pin == pin) {
			*dw_ofs = pad_pullupdown_table[i].pup;
			*bit_ofs = pad_pullupdown_table[i].bit_ofs;
			return E_OK;
		}
	}

	*dw_ofs   = 0;
	*bit_ofs  = 0;

	return E_SYS;
}

static int pad_set_pull(u32 pin, PAD_PULL pulltype)
{
	int ret;
	u32 dw_ofs, bit_ofs, reg_data;

	ret = pad_reg_lookup(pin, &dw_ofs, &bit_ofs);
	if (ret == E_OK) {
		reg_data = PAD_GETREG(PAD_PUPD0_REG_OFS + (dw_ofs << 2));
		reg_data &= ~(3 << bit_ofs);
		reg_data |= (pulltype << bit_ofs);
		PAD_SETREG(PAD_PUPD0_REG_OFS + (dw_ofs << 2), reg_data);
		return E_OK;
	} else {
		return E_SYS;
	}
}

static void gpio_set_dir(u32 pin, GPIO_DIR dir)
{
	u32 reg_data;
	u32 ofs = (pin >> 5) << 2;

	pin &= (32 - 1);

	reg_data = GPIO_GETREG(GPIO_STRG_DIR_REG_OFS + ofs);

	if (dir) {
		reg_data |= (1 << pin);    //output
	} else {
		reg_data &= ~(1 << pin);    //input
	}

	GPIO_SETREG(GPIO_STRG_DIR_REG_OFS + ofs, reg_data);
}

static void gpio_set_pin(u32 pin)
{
	u32 tmp;
	u32 ofs = (pin >> 5) << 2;

	pin &= (32 - 1);
	tmp = (1 << pin);

	GPIO_SETREG(GPIO_STRG_SET0_REG_OFS + ofs, tmp);
}

static void gpio_clear_pin(u32 pin)
{
	u32 tmp;
	u32 ofs = (pin >> 5) << 2;

	pin &= (32 - 1);
	tmp = (1 << pin);

	GPIO_SETREG(GPIO_STRG_CLR0_REG_OFS + ofs, tmp);
}

static void IO_InitNet(void)
{
#ifdef ETH_PHY_25MHz_ENABLE
	*(uint32_t*) 0xF0020000 |= (0x01<<4);

	*(uint32_t*) 0xF0021318 = 0x00;
	*(uint32_t*) 0xF002131C = 0x00;
	*(uint32_t*) 0xF0021320 = 0x32;


	*(uint32_t*) 0xF0020024 &=  ~0x4300;
	*(uint32_t*) 0xF0020024 |= (0x01<<8);
	//*(uint32_t*) 0xF0020024 |= (0x01<<14);

	*(uint32_t*) 0xF002003C &=  ~0xFF;
	*(uint32_t*) 0xF002003C |= 0x0B;

	*(uint32_t*) 0xF0020080 |= (0x01<<12);
#endif

#ifdef ETH_PHY_HW_RESET
	gpio_set_dir(NVT_PHY_RST_PIN, GPIO_DIR_OUTPUT);
	gpio_clear_pin(NVT_PHY_RST_PIN);
	mdelay(20);
	gpio_set_pin(NVT_PHY_RST_PIN);
	mdelay(50);
#endif
}

void iocfg_init(GPIO_INIT_OBJ *gpio_map_initable, u32 gpio_count)
{
	u32 value;

	for (value = 0 ; value < gpio_count ; value++) {
		if (gpio_map_initable[value].GpioDir == GPIO_DIR_INPUT) {
			gpio_set_dir(gpio_map_initable[value].GpioPin, GPIO_DIR_INPUT);
			pad_set_pull(gpio_map_initable[value].PadPin, gpio_map_initable[value].PadDir);
		} else {
			gpio_set_dir(gpio_map_initable[value].GpioPin, GPIO_DIR_OUTPUT);
			if (gpio_map_initable[value].PadDir == GPIO_SET_OUTPUT_HI) {
				gpio_set_pin(gpio_map_initable[value].GpioPin);
			} else {
				gpio_clear_pin(gpio_map_initable[value].GpioPin);
			}
		}
	}

	IO_InitNet();

}
