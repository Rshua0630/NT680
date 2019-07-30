/*
 *  gpio/na51000_gpio.c
 *
 *  Author:	Howard Chang
 *  Created:	July 3, 2018
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>

#include <asm/arch/rcw_macro.h>
#include <asm/arch/IOAddress.h>

#define GPIO_GETREG(ofs)	INW(IOADDR_GPIO_REG_BASE+(ofs))
#define GPIO_SETREG(ofs, value)	OUTW(IOADDR_GPIO_REG_BASE +(ofs),(value))

#define NVT_GPIO_STG_DATA_0		(0)
#define NVT_GPIO_STG_DIR_0		(0x30)
#define NVT_GPIO_STG_SET_0		(0x50)
#define NVT_GPIO_STG_CLR_0		(0x70)

static int gpio_validation(unsigned pin)
{
	if ((pin < C_GPIO(0)) || (pin > C_GPIO(33) && pin < P_GPIO(0)) || \
	(pin > P_GPIO(46) && pin < S_GPIO(0)) || \
	(pin > S_GPIO(10) && pin < L_GPIO(0)) || \
	(pin > L_GPIO(30) && pin < H_GPIO(0)) || \
	(pin > H_GPIO(23) && pin < D_GPIO(0)) || (pin > D_GPIO(17))) {
		printf("The gpio number is out of range\n");
		return -E_NOSPT;
	} else
		return 0;
}

int gpio_request(unsigned offset, const char *label)
{
	return gpio_validation(offset);
}

int gpio_free(unsigned offset)
{
	return 0;
}

int gpio_direction_input(unsigned offset)
{
	u32 reg_data;
	u32 ofs = (offset >> 5) << 2;

	if (gpio_validation(offset) < 0)
		return -E_NOSPT;

	offset &= (32-1);

	reg_data = GPIO_GETREG(NVT_GPIO_STG_DIR_0 + ofs);

	reg_data &= ~(1<<offset);   /*input*/

	GPIO_SETREG(NVT_GPIO_STG_DIR_0 + ofs, reg_data);

	return 0;
}

int gpio_direction_output(unsigned offset, int value)
{
	u32 reg_data, tmp;
	u32 ofs = (offset >> 5) << 2;

	if (gpio_validation(offset) < 0)
		return -E_NOSPT;

	offset &= (32-1);

	tmp = (1<<offset);

	reg_data = GPIO_GETREG(NVT_GPIO_STG_DIR_0 + ofs);

	reg_data |= (1<<offset);    /*output*/

	GPIO_SETREG(NVT_GPIO_STG_DIR_0 + ofs, reg_data);

	if (value)
		GPIO_SETREG(NVT_GPIO_STG_SET_0 + ofs, tmp);
	else
		GPIO_SETREG(NVT_GPIO_STG_CLR_0 + ofs, tmp);

	return 0;
}

int gpio_get_value(unsigned offset)
{
	u32 tmp;
	u32 ofs = (offset >> 5) << 2;

	if (gpio_validation(offset) < 0)
		return -E_NOSPT;

	offset &= (32-1);

	tmp = (1<<offset);

	return (GPIO_GETREG(NVT_GPIO_STG_DATA_0 + ofs) & tmp) != 0;
}

int gpio_set_value(unsigned offset, int value)
{
	u32 tmp;
	u32 ofs = (offset >> 5) << 2;

	if (gpio_validation(offset) < 0)
		return -E_NOSPT;

	offset &= (32-1);

	tmp = (1<<offset);

	if (value)
		GPIO_SETREG(NVT_GPIO_STG_SET_0 + ofs, tmp);
	else
		GPIO_SETREG(NVT_GPIO_STG_CLR_0 + ofs, tmp);

	return 0;
}
