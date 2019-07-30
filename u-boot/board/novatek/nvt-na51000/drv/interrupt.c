/*
	Interrupt module driver.

	This file is the driver of Interrupt module.

	@file		interrupt.c
	@ingroup
	@note		Nothing.

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include <common.h>
#include <asm/io.h>
#include <modelext/interrupt.h>
#include "interrupt_int.h"

/**
	Configure interrupt controller

	Configure interrupt controller by upper layer

	@param[in] config_id	Configuration ID
	@param[in] config		Configuration value
	@return void
*/

void int_gic_set_target(unsigned int number, unsigned int target);

void int_set_config(INT_CONFIG_ID config_id, UINT32 config)
{
	UINT32  gic_int_cnt;
	UINT32  gic_int_num[4];
	UINT32  gic_int_group_cnt;
	UINT32  ca53_core_num;
	UINT32  bits;
	PINT_GIC_DST pint_dst = (PINT_GIC_DST)config;
	UINT32   addr[CC_CORE_CA53_CORE_NUM];

	switch (config_id) {
	case INT_CONFIG_ID_GIC_DESTINATION:
		addr[CC_CORE_CA53_CORE1] = (UINT32)&pint_dst->int_id_dst[CC_CORE_CA53_CORE1].Reg;
		addr[CC_CORE_CA53_CORE2] = (UINT32)&pint_dst->int_id_dst[CC_CORE_CA53_CORE2].Reg;

		gic_int_group_cnt = INT_ID_MAX / 32;

		if (INT_ID_MAX % 32 != 0) {
			gic_int_group_cnt++;
		}

		for (ca53_core_num = 0; ca53_core_num < CC_CORE_CA53_CORE_NUM; ca53_core_num++) {
			for (gic_int_cnt = 0; gic_int_cnt < gic_int_group_cnt; gic_int_cnt++) {
				gic_int_num[gic_int_cnt] = *(UINT32 *)(addr[ca53_core_num] + (gic_int_cnt << 2));
				//printf("	Core[%d] GIC Int Group Cnt[%d]addr[0x%08x]\r\n", (int)ca53_core_num, (int)gic_int_cnt, (unsigned int)gic_int_num[gic_int_cnt]);
				while (gic_int_num[gic_int_cnt]) {
					bits = __builtin_ctz(gic_int_num[gic_int_cnt]);
					if (ca53_core_num == CC_CORE_CA53_CORE1) {
						//printf("		Core[%d] Int[%d] set to target 1\r\n", (int)ca53_core_num, (int)(bits + gic_int_cnt * 32));
						int_gic_set_target(INT_GIC_SPI_START_ID + (bits + gic_int_cnt * 32), 1);
					} else {
						//printf("		Core[%d] Int[%d] set to target 2\r\n", (int)ca53_core_num, (int)(bits + gic_int_cnt * 32));
						int_gic_set_target(INT_GIC_SPI_START_ID + (bits + gic_int_cnt * 32), 2);
					}
					gic_int_num[gic_int_cnt] &= ~(1<<bits);
				}
			}
		}
		break;
	default:
		printf("Unknow interrupt set config ID%d\r\n", config_id);
		break;
	}
}

/*-----------------------------------------------------------------------------*/
/* GIC Interface Functions							*/
/*-----------------------------------------------------------------------------*/

void int_gic_set_target(unsigned int number, unsigned int target)
{
	unsigned int bank, tmp;

	target = target & 0xFF;
	bank = number/4;
	number   = number & 0x3;
	number   = number * 8;
	target = target << number;

	tmp = GIC_GETREG(GICD_ITARGETSR + (bank<<2));
	tmp = tmp & ~(0xFF << number);
	tmp = tmp | target;

	GIC_SETREG(GICD_ITARGETSR + (bank<<2), tmp);
}
