/*
 * Copyright Novatek Inc
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#ifndef __ARCH_NA51000_ADC_H
#define __ARCH_NA51000_ADC_H

void nvt_adc_open(void);
void nvt_adc_close(void);
u32 nvt_adc_get_value(u8 channel);

#endif
