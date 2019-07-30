/*
 *  arch/mips/include/asm/arch-nt96660/nt96660_product_specific.h
 *
 *  Author:	Alvin lin
 *  Created:	Jun 11, 2012
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NT96660_PRODUCT_SPECIFIC_FUNCTIONS_H__
#define __NT96660_PRODUCT_SPECIFIC_FUNCTIONS_H__

#if defined(SKYWORTH)

typedef enum {
	SKYWORTH_LED_RED,
	SKYWORTH_LED_GREEN,
	SKYWORTH_LED_TOGGLE
}skyworth_led_state;

void skyworth_led_init(void);

void skyworth_led_ctrl(skyworth_led_state state);

#endif


#endif
