/*
 * This header provides constants for binding novatek,na*-gpio.
 *
 * The first cell in Novetek's GPIO specifier is the GPIO ID. The macros below
 * provide names for this.
 *
 * The second cell contains standard flag values specified in gpio.h.
 */

#ifndef _DT_BINDINGS_GPIO_NVT_GPIO_H
#define _DT_BINDINGS_GPIO_NVT_GPIO_H

#include <dt-bindings/gpio/gpio.h>

#define C_GPIO(pin)			(pin)
#define P_GPIO(pin)			(pin + 0x40)
#define S_GPIO(pin)			(pin + 0x80)
#define L_GPIO(pin)			(pin + 0xA0)
#define H_GPIO(pin)			(pin + 0xC0)
#define D_GPIO(pin)			(pin + 0xE0)

#define GPIO_RISING_EDGE		0x1
#define GPIO_FALLING_EDGE		0x0

#define GPIO_INTERRUPT		0x1
#define GPIO_POLLING		0x0

#define GPIO_HIGH		0x1
#define GPIO_LOW		0x0

#endif
