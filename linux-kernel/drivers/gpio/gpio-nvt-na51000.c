/**
    NVT GPIO driver
    To handle NVT GPIO interrupt extender and GPIO driver
    @file nvt-gpio.c
    @ingroup
    @note
    Copyright Novatek Microelectronics Corp. 2016. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/irqchip/chained_irq.h>

#include <mach/nvt-io.h>
#include <mach/nvt-gpio.h>
#include <mach/nvt-ipc.h>
#include <mach/cc.h>
/*
 * Interrupt number 256~287 will map to GPIO 0~31
 *	NT96680 GPIO document		===>	SW IRQ number mapping
 *============================================================================
 * 0	C_GPIO[3]	16 P_GPIO[36]	===> 256	C_GPIO[3]	272 P_GPIO[36]
 * 1	C_GPIO[9]	17 P_GPIO[37]	===> 257	C_GPIO[9]	273 P_GPIO[37]
 * 2	C_GPIO[15]	18 P_GPIO[41]	===> 258	C_GPIO[15]	274 P_GPIO[41]
 * 3	C_GPIO[21]	19 P_GPIO[20]	===> 259	C_GPIO[21]	275 P_GPIO[20]
 * 4	C_GPIO[26]	20 P_GPIO[21]	===> 260	C_GPIO[26]	276 P_GPIO[21]
 * 5	C_GPIO[27]	21 P_GPIO[43]	===> 261	C_GPIO[27]	277 P_GPIO[43]
 * 6	C_GPIO[28]	22 C_GPIO[25]	===> 262	C_GPIO[28]	278 C_GPIO[25]
 * 7	C_GPIO[33]	23 C_GPIO[31]	===> 263	C_GPIO[33]	279 C_GPIO[31]
 * 8	P_GPIO[3]	24 L_GPIO[14]	===> 264	P_GPIO[3]	280 L_GPIO[14]
 * 9	P_GPIO[6]	25 L_GPIO[15]	===> 265	P_GPIO[6]	281 L_GPIO[15]
 * 10	P_GPIO[11]	26 L_GPIO[20]	===> 266	P_GPIO[11]	282 L_GPIO[20]
 * 11	P_GPIO[15]	27 L_GPIO[21]	===> 267	P_GPIO[15]	283 L_GPIO[21]
 * 12	P_GPIO[19]	28 L_GPIO[22]	===> 268	P_GPIO[19]	284 L_GPIO[22]
 * 13	P_GPIO[29]	29 L_GPIO[26]	===> 269	P_GPIO[29]	285 L_GPIO[26]
 * 14	P_GPIO[31]	30 P_GPIO[44]	===> 270	P_GPIO[31]	286 P_GPIO[44]
 * 15	P_GPIO[35]	31 P_GPIO[47]	===> 271	P_GPIO[35]	287 P_GPIO[47]
 *
 */

/*
 * Interrupt number 288~300 will map to DGPIO 0~12
 *	NT96680 DGPIO document		===>	SW IRQ number mapping
 *============================================================================
 * 0	D_GPIO[0]			===>	288	D_GPIO[0]
 * 1	D_GPIO[1]			===>	289	D_GPIO[1]
 * 2	D_GPIO[2]			===>	290	D_GPIO[2]
 * 3	D_GPIO[3]			===>	291	D_GPIO[3]
 * 4	D_GPIO[4]			===>	292	D_GPIO[4]
 * 5	D_GPIO[5]			===>	293	D_GPIO[5]
 * 6	D_GPIO[6]			===>	294	D_GPIO[6]
 * 7	D_GPIO[7]			===>	295	D_GPIO[7]
 * 8	D_GPIO[8]			===>	296	D_GPIO[8]
 * 9	D_GPIO[9]			===>	297	D_GPIO[9]
 * 10	D_GPIO[10]			===>	298	D_GPIO[10]
 * 11	D_GPIO[11]			===>	299	D_GPIO[11]
 * 12	D_GPIO[12]			===>	300	D_GPIO[12]
 *
 */

#define DRV_VERSION		"1.04.000"

#define loc_cpu(lock, flags) {spin_lock_irqsave(lock, flags);cc_hwlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_GPIO);}
#define unl_cpu(lock, flags) {cc_hwunlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_GPIO);spin_unlock_irqrestore(lock, flags);}

static uint32_t GPIO_REG_BASE;
#define GPIO_GETREG(ofs)	readl((void __iomem *)(GPIO_REG_BASE + ofs))
#define GPIO_SETREG(ofs, value)	writel(value, (void __iomem *)(GPIO_REG_BASE + ofs))

/*GPIO number in na51000 SoC*/
#define GPIO_DATA_NUM		8
#define GPIO_IRQ_NUM		4
#define GPIO_IRQ_CON_NUM	3

struct nvt_gpio_pm_data {
	u32 gpio_data[GPIO_DATA_NUM];
	u32 gpio_dir[GPIO_DATA_NUM];
	u32 gpio_irq_en[GPIO_IRQ_NUM];
	u32 gpio_irq_config[GPIO_IRQ_CON_NUM];
	u32 gpio_irq_dir[GPIO_IRQ_NUM];
	u32 dgpio_irq_en[GPIO_IRQ_NUM];
	u32 dgpio_irq_config[GPIO_IRQ_CON_NUM];
	u32 dgpio_irq_dir[GPIO_IRQ_NUM];
};

struct nvt_gpio_chip {
	struct device		*dev;
	unsigned int		irq_base;		/* Shared IRQ */
	unsigned int		irq_summary;		/* Total IRQ numbers */
	unsigned int		irq_offset;		/* Extended offset */
	u64			irq_request_record;	/* To record int ID 0~44 */
	struct irq_chip		irq_chip;
	u64			int_status;		/* Interrupt status 64~108 gpio and dgpio */
	spinlock_t		lock;
	struct gpio_chip	gpio_chip;		/* To abstract gpio controller */
	struct nvt_gpio_pm_data	pm_data;		/* GPIO data used for power management */
};

static int nvt_gpio_irq_table[NVT_GPIO_INT_EXT_TOTAL] = {
	C_GPIO(3),	C_GPIO(9),	C_GPIO(15),	C_GPIO(21),	C_GPIO(26),	C_GPIO(27),
	C_GPIO(28),	C_GPIO(33),	P_GPIO(3),	P_GPIO(6),	P_GPIO(11),	P_GPIO(15),
	P_GPIO(19),	P_GPIO(29),	P_GPIO(31),	P_GPIO(35),	P_GPIO(36),	P_GPIO(37),
	P_GPIO(41),	P_GPIO(20),	P_GPIO(21),	P_GPIO(43),	C_GPIO(25),	C_GPIO(31),
	L_GPIO(14),	L_GPIO(15),	L_GPIO(20),	L_GPIO(21),	L_GPIO(22),	L_GPIO(26),
	P_GPIO(44),	P_GPIO(47),	D_GPIO(0),	D_GPIO(1),	D_GPIO(2),	D_GPIO(3),
	D_GPIO(4),	D_GPIO(5),	D_GPIO(6),	D_GPIO(7),	D_GPIO(8),	D_GPIO(9),
	D_GPIO(10),	D_GPIO(11),	D_GPIO(12)
};

static void nvt_gpio_int_pin_init(u8 irq_num, u8 type, u8 pol, u8 edge_type)
{
	u32 reg = 0;

	/* To configure the int type (Edge trigger=0 or Level trigger=1) */
	if (type) {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_INT_TYPE);
			reg |= (1<<irq_num);
			GPIO_SETREG(NVT_GPIO_INT_TYPE, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_INT_TYPE);
			reg |= (1<<(irq_num - 32));
			GPIO_SETREG(NVT_DGPIO_INT_TYPE, reg);
		}
	} else {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_INT_TYPE);
			reg &= ~(1<<irq_num);
			GPIO_SETREG(NVT_GPIO_INT_TYPE, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_INT_TYPE);
			reg &= ~(1<<(irq_num - 32));
			GPIO_SETREG(NVT_DGPIO_INT_TYPE, reg);
		}
	}

	/* To configure the polarity (Positive=0 or Negative=1) */
	if (pol) {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_INT_POL);
			reg |= (1<<irq_num);
			GPIO_SETREG(NVT_GPIO_INT_POL, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_INT_POL);
			reg |= (1<<(irq_num - 32));
			GPIO_SETREG(NVT_DGPIO_INT_POL, reg);
		}
	} else {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_INT_POL);
			reg &= ~(1<<irq_num);
			GPIO_SETREG(NVT_GPIO_INT_POL, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_INT_POL);
			reg &= ~(1<<(irq_num - 32));
			GPIO_SETREG(NVT_DGPIO_INT_POL, reg);
		}
	}


	/* To configure the polarity (Both edge) */
	if (edge_type) {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_EDGE_TYPE);
			reg |= (1<<irq_num);
			GPIO_SETREG(NVT_GPIO_EDGE_TYPE, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_EDGE_TYPE);
			reg |= (1<<(irq_num - 32));
			GPIO_SETREG(NVT_DGPIO_EDGE_TYPE, reg);
		}
	} else {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_EDGE_TYPE);
			reg &= ~(1<<irq_num);
			GPIO_SETREG(NVT_GPIO_EDGE_TYPE, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_EDGE_TYPE);
			reg &= ~(1<<(irq_num - 32));
			GPIO_SETREG(NVT_DGPIO_EDGE_TYPE, reg);
		}
	}

	/* To clear the interrupt status */
	if (irq_num < 32) {
		reg = GPIO_GETREG(NVT_GPIO_STS_CPU2);
		reg |= 1<<irq_num;
		GPIO_SETREG(NVT_GPIO_STS_CPU2, reg);
		reg = GPIO_GETREG(NVT_GPIO_STS_CPU1);
		reg |= 1<<irq_num;
		GPIO_SETREG(NVT_GPIO_STS_CPU1, reg);
	} else {
		reg = GPIO_GETREG(NVT_DGPIO_STS_CPU2);
		reg |= 1<<(irq_num - 32);
		GPIO_SETREG(NVT_DGPIO_STS_CPU2, reg);
		reg = GPIO_GETREG(NVT_DGPIO_STS_CPU1);
		reg |= 1<<(irq_num - 32);
		GPIO_SETREG(NVT_DGPIO_STS_CPU1, reg);
	}
	/* To assign the interrupt issue to CPU2 */
	if (irq_num < 32) {
		reg = GPIO_GETREG(NVT_GPIO_TO_CPU2);
		reg |= 1<<irq_num;
		GPIO_SETREG(NVT_GPIO_TO_CPU2, reg);
		reg = GPIO_GETREG(NVT_GPIO_TO_CPU1);
		reg &= ~(1<<irq_num);
		GPIO_SETREG(NVT_GPIO_TO_CPU1, reg);
	} else {
		reg = GPIO_GETREG(NVT_DGPIO_TO_CPU2);
		reg |= 1<<(irq_num - 32);
		GPIO_SETREG(NVT_DGPIO_TO_CPU2, reg);
		reg = GPIO_GETREG(NVT_DGPIO_TO_CPU1);
		reg &= ~(1<<(irq_num - 32));
		GPIO_SETREG(NVT_DGPIO_TO_CPU1, reg);
	}
}

static u64 nvt_gpio_int_get_status(struct nvt_gpio_chip *chip)
{
	u64 reg = 0;

	/* To read the CPU2's interrupt status */
	reg = GPIO_GETREG(NVT_GPIO_STS_CPU2);

	/* To read DGPIO interrupt status of the CPU2 side, only provide DGPIO0~DGPIO12 */
	reg |= ((u64)GPIO_GETREG(NVT_DGPIO_STS_CPU2) & 0x1fff) << 32;

	/* The irq_request_record will record request_threaded_irq applied irq
	number, the driver is only used to handle these number */
	reg &= (u64)chip->irq_request_record;

	return reg;
}

static void nvt_gpio_int_set_status(u64 reg)
{
	u32 tmp = (u32)(reg & 0xffffffff);
	// To check if it belongs to GPIO or DGPIO type
	if (tmp != 0)
		GPIO_SETREG(NVT_GPIO_STS_CPU2, tmp);
	else {
		tmp = (u32)(reg >> 32);
		GPIO_SETREG(NVT_DGPIO_STS_CPU2, tmp);
	}
}

static void nvt_gpio_int_pin_en(u8 irq_num, u8 en)
{
	u32 reg = 0;

	if (en) {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_INTEN_CPU2);
			reg |= 1<<irq_num;
			GPIO_SETREG(NVT_GPIO_INTEN_CPU2, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_INTEN_CPU2);
			reg |= 1<<(irq_num - 32);
			GPIO_SETREG(NVT_DGPIO_INTEN_CPU2, reg);
		}
	} else {
		if (irq_num < 32) {
			reg = GPIO_GETREG(NVT_GPIO_INTEN_CPU2);
			reg &= ~(1<<irq_num);
			GPIO_SETREG(NVT_GPIO_INTEN_CPU2, reg);
		} else {
			reg = GPIO_GETREG(NVT_DGPIO_INTEN_CPU2);
			reg &= ~(1<<(irq_num - 32));
			GPIO_SETREG(NVT_DGPIO_INTEN_CPU2, reg);
		}
	}
}

static void nvt_gpio_irq_mask(struct irq_data *d)
{
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = irq_data_get_irq_chip_data(d);
	unsigned int irq_num = d->irq - nvt_gpio_chip_ptr->irq_offset;
	unsigned long flags = 0;

	spin_lock_irqsave(&nvt_gpio_chip_ptr->lock, flags);
	/* To clear the irq id flag */
	nvt_gpio_chip_ptr->irq_request_record &= ~((u64)1<<irq_num);
	/* Disable IRQ */
	nvt_gpio_int_pin_en(d->irq - nvt_gpio_chip_ptr->irq_offset, 0);
	spin_unlock_irqrestore(&nvt_gpio_chip_ptr->lock, flags);
}

static void nvt_gpio_irq_unmask(struct irq_data *d)
{
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = irq_data_get_irq_chip_data(d);
	unsigned int irq_num = d->irq - nvt_gpio_chip_ptr->irq_offset;
	unsigned long flags = 0;

	spin_lock_irqsave(&nvt_gpio_chip_ptr->lock, flags);
	/* To set the irq id flag */
	nvt_gpio_chip_ptr->irq_request_record |= ((u64)1<<irq_num);
	/* Enable IRQ */
	nvt_gpio_int_pin_en(d->irq - nvt_gpio_chip_ptr->irq_offset, 1);
	spin_unlock_irqrestore(&nvt_gpio_chip_ptr->lock, flags);
}

static int nvt_gpio_irq_set_type(struct irq_data *d, unsigned int type)
{
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = irq_data_get_irq_chip_data(d);
	int irq_num = d->irq - nvt_gpio_chip_ptr->irq_offset;
	unsigned long flags = 0;

	if (type == IRQ_TYPE_LEVEL_LOW) {
		__irq_set_handler_locked(d->irq, handle_level_irq);
		loc_cpu(&nvt_gpio_chip_ptr->lock, flags);
		nvt_gpio_int_pin_init(irq_num, 1, 1, 0);
		unl_cpu(&nvt_gpio_chip_ptr->lock, flags);
		dev_info(nvt_gpio_chip_ptr->dev,
			"HWIRQ: %d IRQ: %d ### Level Low ###\n",
			irq_num, d->irq);
	} else if (type == IRQ_TYPE_LEVEL_HIGH) {
		__irq_set_handler_locked(d->irq, handle_level_irq);
		loc_cpu(&nvt_gpio_chip_ptr->lock, flags);
		nvt_gpio_int_pin_init(irq_num, 1, 0, 0);
		unl_cpu(&nvt_gpio_chip_ptr->lock, flags);
		dev_info(nvt_gpio_chip_ptr->dev,
			"HWIRQ: %d IRQ: %d ### Level High ###\n",
			irq_num, d->irq);
	} else if (type == IRQ_TYPE_EDGE_FALLING) {
		__irq_set_handler_locked(d->irq, handle_edge_irq);
		loc_cpu(&nvt_gpio_chip_ptr->lock, flags);
		nvt_gpio_int_pin_init(irq_num, 0, 1, 0);
		unl_cpu(&nvt_gpio_chip_ptr->lock, flags);
		dev_info(nvt_gpio_chip_ptr->dev,
			"HWIRQ: %d IRQ: %d ### Edge falling ###\n",
			irq_num, d->irq);
	} else if (type == IRQ_TYPE_EDGE_RISING) {
		__irq_set_handler_locked(d->irq, handle_edge_irq);
		loc_cpu(&nvt_gpio_chip_ptr->lock, flags);
		nvt_gpio_int_pin_init(irq_num, 0, 0, 0);
		unl_cpu(&nvt_gpio_chip_ptr->lock, flags);
		dev_info(nvt_gpio_chip_ptr->dev,
			"HWIRQ: %d IRQ: %d ### Edge rising ###\n",
			irq_num, d->irq);
	} else {
		__irq_set_handler_locked(d->irq, handle_edge_irq);
		loc_cpu(&nvt_gpio_chip_ptr->lock, flags);
		nvt_gpio_int_pin_init(irq_num, 0, 0, 1);
		unl_cpu(&nvt_gpio_chip_ptr->lock, flags);
		dev_info(nvt_gpio_chip_ptr->dev,
			"HWIRQ: %d IRQ: %d ### Both Edge ###\n",
			irq_num, d->irq);
	}

	return 0;
}

static void nvt_gpio_irq_ack(struct irq_data *d)
{
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = irq_data_get_irq_chip_data(d);
	int irq_num = d->irq - nvt_gpio_chip_ptr->irq_offset;
	u64 irq_ack_number = 0x1;

	irq_ack_number = irq_ack_number << irq_num;

	nvt_gpio_int_set_status(irq_ack_number);
}

static void nvt_summary_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	int n = 0;
	struct irq_chip *chip = irq_desc_get_chip(desc);
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = irq_get_handler_data(irq);

	/* To read the interrupt status to find which pin is trigger */
	nvt_gpio_chip_ptr->int_status =
			nvt_gpio_int_get_status(nvt_gpio_chip_ptr);

	/* mack irq */
	chained_irq_enter(chip, desc);

	/* To find which pin is trigger */
	for (n = 0; n < nvt_gpio_chip_ptr->irq_summary; n++) {
		if (nvt_gpio_chip_ptr->int_status & ((u64)1 << n))
			generic_handle_irq(n + nvt_gpio_chip_ptr->irq_offset);
	}

	/* unmask irq */
	chained_irq_exit(chip, desc);
}

static int nvt_install_irq_chip(struct nvt_gpio_chip *chip)
{
	int ret = 0;
	u16 n = 0;
	u32 irq_total_num = chip->irq_summary + chip->irq_offset;

	for (n = chip->irq_offset; n < irq_total_num; n++) {
		irq_set_chip_data(n, chip);
		irq_set_chip_and_handler(n, &chip->irq_chip, handle_edge_irq);
		irq_set_chip(n, &chip->irq_chip);
		irq_set_noprobe(n);
		irq_clear_status_flags(n, IRQ_NOREQUEST);
	}

	irq_set_chained_handler(chip->irq_base, nvt_summary_irq_handler);
	irq_set_handler_data(chip->irq_base, chip);
	return ret;
}

static inline struct nvt_gpio_chip *to_nvt_gpio(struct gpio_chip *chip)
{
	return container_of(chip, struct nvt_gpio_chip, gpio_chip);
}

static int gpio_validation(unsigned pin)
{
	if ((pin < C_GPIO(0)) || (pin > C_GPIO(33) && pin < P_GPIO(0)) || \
	(pin > P_GPIO(46) && pin < S_GPIO(0)) || \
	(pin > S_GPIO(10) && pin < L_GPIO(0)) || \
	(pin > L_GPIO(30) && pin < H_GPIO(0)) || \
	(pin > H_GPIO(23) && pin < D_GPIO(0)) || (pin > D_GPIO(17))) {
		pr_err("The gpio number is out of range\n");
		return -ENOENT;
	} else
		return 0;
}

static void gpio_pad_power_check(unsigned pin)
{
	u32 pad_power = (*(u32*) 0xFD030080) & 0x1;
	u32 config_pad_value = (*(u32*) 0xFD29001C) & 0x10;

	if ((pin >= C_GPIO(22)) && (pin <= C_GPIO(27))) {
		if (config_pad_value && (!pad_power))
			*(u32*) 0xFD030080 = 0x11;
		else if ((!config_pad_value) && pad_power)
			*(u32*) 0xFD030080 = 0x10;
	}
}

static int nvt_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	uint32_t tmp;
	uint32_t ofs = (offset >> 5) << 2;

	if (gpio_validation(offset) < 0)
		return -ENOENT;

	gpio_pad_power_check(offset);

	offset &= (32-1);
	tmp = (1<<offset);

	return (GPIO_GETREG(NVT_GPIO_STG_DATA_0 + ofs) & tmp) != 0;
}

static void nvt_gpio_set(struct gpio_chip *gc, unsigned offset, int value)
{
	uint32_t tmp;
	uint32_t ofs = (offset >> 5) << 2;

	if (gpio_validation(offset) == 0) {

		gpio_pad_power_check(offset);

		offset &= (32-1);
		tmp = (1<<offset);

		if (value)
			GPIO_SETREG(NVT_GPIO_STG_SET_0 + ofs, tmp);
		else
			GPIO_SETREG(NVT_GPIO_STG_CLR_0 + ofs, tmp);
	}

}

static int nvt_gpio_dir_input(struct gpio_chip *gc, unsigned offset)
{
	uint32_t reg_data;
	uint32_t ofs = (offset >> 5) << 2;
	unsigned long flags = 0;
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = to_nvt_gpio(gc);

	if (gpio_validation(offset) < 0)
		return -ENOENT;

	gpio_pad_power_check(offset);

	offset &= (32-1);

	/*race condition protect. enter critical section*/
	loc_cpu(&nvt_gpio_chip_ptr->lock, flags);

	//reg_data = GPIO_GETREG(GPIO_STRG_DIR0_REG_OFS + ofs);
	reg_data = GPIO_GETREG(NVT_GPIO_STG_DIR_0 + ofs);
	reg_data &= ~(1<<offset);   /*input*/
	GPIO_SETREG(NVT_GPIO_STG_DIR_0 + ofs, reg_data);
	//GPIO_SETREG(GPIO_STRG_DIR0_REG_OFS + ofs, reg_data);

	/*race condition protect. leave critical section*/
	unl_cpu(&nvt_gpio_chip_ptr->lock, flags);

	return 0;
}

static int nvt_gpio_dir_output(struct gpio_chip *gc,
					unsigned offset, int value)
{
	uint32_t reg_data, tmp;
	uint32_t ofs = (offset >> 5) << 2;
	unsigned long flags = 0;
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = to_nvt_gpio(gc);

	if (gpio_validation(offset) < 0)
		return -ENOENT;

	gpio_pad_power_check(offset);

	offset &= (32-1);
	tmp = (1<<offset);
	/*race condition protect. enter critical section*/
	//loc_cpu();
	loc_cpu(&nvt_gpio_chip_ptr->lock, flags);
	reg_data = GPIO_GETREG(NVT_GPIO_STG_DIR_0 + ofs);
	reg_data |= (1<<offset);    /*output*/
	GPIO_SETREG(NVT_GPIO_STG_DIR_0 + ofs, reg_data);

	/*race condition protect. leave critical section*/
	unl_cpu(&nvt_gpio_chip_ptr->lock, flags);

	if (value)
		GPIO_SETREG(NVT_GPIO_STG_SET_0 + ofs, tmp);
	else
		GPIO_SETREG(NVT_GPIO_STG_CLR_0 + ofs, tmp);

	return 0;
}

static int nvt_gpio_to_irq(struct gpio_chip *gc,
					unsigned offset)
{
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = to_nvt_gpio(gc);

	unsigned int i;

	for (i = 0; i < nvt_gpio_chip_ptr->irq_summary; i++) {
		if (nvt_gpio_irq_table[i] == offset)
			return i + nvt_gpio_chip_ptr->irq_offset;
	}

	return -ENOENT;
}

static int nvt_gpio_probe(struct platform_device *pdev)
{
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = NULL;
	struct resource *res = NULL;
	int ret = 0;

	nvt_gpio_chip_ptr = kzalloc(sizeof(struct nvt_gpio_chip), GFP_KERNEL);
	if (!nvt_gpio_chip_ptr)
		return -ENOMEM;

	if (pdev->resource->flags == IORESOURCE_MEM) {
	/* setup resource */
		res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		if (unlikely(!res)) {
			printk("%s, %s fails: platform_get_resource IORESOURCE_MEM not OK", pdev->name, __FUNCTION__);
			ret = -ENXIO;
			goto out_free;
		}
	}

	GPIO_REG_BASE = (u32) ioremap_nocache(res->start, resource_size(res));
	if (unlikely(GPIO_REG_BASE == 0)) {
		printk("%s fails: ioremap_nocache fail\n", __FUNCTION__);
		ret = -1;
		goto remap_err;
	}

	/* Basic data structure initialization */
	/* irq structure init. */
	nvt_gpio_chip_ptr->dev			 = &pdev->dev;
	nvt_gpio_chip_ptr->irq_base		 = platform_get_irq(pdev, 0);
	nvt_gpio_chip_ptr->irq_summary		 = NVT_GPIO_INT_EXT_TOTAL;
	nvt_gpio_chip_ptr->irq_chip.name	 = pdev->name;
	nvt_gpio_chip_ptr->irq_chip.irq_mask	 = nvt_gpio_irq_mask;
	nvt_gpio_chip_ptr->irq_chip.irq_unmask	 = nvt_gpio_irq_unmask;
	nvt_gpio_chip_ptr->irq_chip.irq_set_type = nvt_gpio_irq_set_type;
	nvt_gpio_chip_ptr->irq_chip.irq_ack	 = nvt_gpio_irq_ack;
	nvt_gpio_chip_ptr->irq_offset		 = NVT_IRQ_GPIO_EXT_START;
	nvt_gpio_chip_ptr->irq_request_record	 = 0;
	nvt_gpio_chip_ptr->int_status		 = 0;

	/* gpio structure init. */
	nvt_gpio_chip_ptr->gpio_chip.get = nvt_gpio_get;
	nvt_gpio_chip_ptr->gpio_chip.set = nvt_gpio_set;
	nvt_gpio_chip_ptr->gpio_chip.direction_input = nvt_gpio_dir_input;
	nvt_gpio_chip_ptr->gpio_chip.direction_output = nvt_gpio_dir_output;
	nvt_gpio_chip_ptr->gpio_chip.ngpio = NVT_GPIO_NUMBER;
	nvt_gpio_chip_ptr->gpio_chip.to_irq = nvt_gpio_to_irq;
	nvt_gpio_chip_ptr->gpio_chip.base = 0;
#ifdef CONFIG_OF_GPIO
	nvt_gpio_chip_ptr->gpio_chip.of_gpio_n_cells = 2;
	nvt_gpio_chip_ptr->gpio_chip.of_node = pdev->dev.of_node;
#endif

	ret = gpiochip_add(&nvt_gpio_chip_ptr->gpio_chip);
	if (ret < 0) {
		dev_err(&pdev->dev, "Could not register gpiochip, %d\n", ret);
		goto out_free;
	}

	spin_lock_init(&nvt_gpio_chip_ptr->lock);
	/* To install GPIO IRQ chip */
	ret = nvt_install_irq_chip(nvt_gpio_chip_ptr);
	if (ret < 0)
		goto out_free_irq;

	platform_set_drvdata(pdev, nvt_gpio_chip_ptr);

	dev_info(&pdev->dev, "Register %s successfully\n", __func__);
	return 0;

remap_err:
	release_mem_region(res->start, (res->end - res->start + 1));

out_free_irq:
	gpiochip_remove(&nvt_gpio_chip_ptr->gpio_chip);
out_free:
	kfree(nvt_gpio_chip_ptr);

	return ret;
}

static int nvt_gpio_remove(struct platform_device *pdev)
{
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = platform_get_drvdata(pdev);
	int n = 0;
	u32 irq_total_num = nvt_gpio_chip_ptr->irq_summary +
				nvt_gpio_chip_ptr->irq_offset;

	for (n = nvt_gpio_chip_ptr->irq_offset; n < irq_total_num; n++)
		irq_set_chip_and_handler(n, NULL, NULL);

	free_irq(nvt_gpio_chip_ptr->irq_base, nvt_gpio_chip_ptr);
	gpiochip_remove(&nvt_gpio_chip_ptr->gpio_chip);
	platform_set_drvdata(pdev, NULL);
	kfree(nvt_gpio_chip_ptr);
	return 0;
}

#ifdef CONFIG_PM
static int nvt_gpio_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = platform_get_drvdata(pdev);
	int i;

	disable_irq(nvt_gpio_chip_ptr->irq_base);

	for (i = 0; i < GPIO_DATA_NUM; i++) {
		nvt_gpio_chip_ptr->pm_data.gpio_dir[i] = \
			GPIO_GETREG(NVT_GPIO_STG_DIR_0 + i*0x4);

		nvt_gpio_chip_ptr->pm_data.gpio_data[i] = \
			GPIO_GETREG(NVT_GPIO_STG_DATA_0 + i*0x4);
	}

	for (i = 0; i < GPIO_IRQ_CON_NUM; i++) {
		nvt_gpio_chip_ptr->pm_data.gpio_irq_config[i] = \
			GPIO_GETREG(NVT_GPIO_INT_TYPE + i*0x4);

		nvt_gpio_chip_ptr->pm_data.dgpio_irq_config[i] = \
			GPIO_GETREG(NVT_DGPIO_INT_TYPE + i*0x4);
	}

	for (i = 0; i < GPIO_IRQ_NUM; i++) {
		nvt_gpio_chip_ptr->pm_data.gpio_irq_dir[i] = \
			GPIO_GETREG(NVT_GPIO_TO_CPU1 + i*0x4);

		nvt_gpio_chip_ptr->pm_data.dgpio_irq_dir[i] = \
			GPIO_GETREG(NVT_DGPIO_TO_CPU1 + i*0x4);

		nvt_gpio_chip_ptr->pm_data.gpio_irq_en[i] = \
			GPIO_GETREG(NVT_GPIO_INTEN_CPU1 + i*0x4);

		nvt_gpio_chip_ptr->pm_data.dgpio_irq_en[i] = \
			GPIO_GETREG(NVT_DGPIO_INTEN_CPU1 + i*0x4);
	}

	return 0;
}
static int nvt_gpio_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct nvt_gpio_chip *nvt_gpio_chip_ptr = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < GPIO_DATA_NUM; i++) {
		GPIO_SETREG(NVT_GPIO_STG_DIR_0 + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.gpio_dir[i]);

		GPIO_SETREG(NVT_GPIO_STG_DATA_0 + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.gpio_data[i]);
	}

	for (i = 0; i < GPIO_IRQ_CON_NUM; i++) {
		GPIO_SETREG(NVT_GPIO_INT_TYPE + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.gpio_irq_config[i]);

		GPIO_SETREG(NVT_DGPIO_INT_TYPE + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.dgpio_irq_config[i]);
	}

	for (i = 0; i < GPIO_IRQ_NUM; i++) {
		GPIO_SETREG(NVT_GPIO_TO_CPU1 + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.gpio_irq_dir[i]);

		GPIO_SETREG(NVT_DGPIO_TO_CPU1 + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.dgpio_irq_dir[i]);

		GPIO_SETREG(NVT_DGPIO_INTEN_CPU1 + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.dgpio_irq_en[i]);

		GPIO_SETREG(NVT_GPIO_INTEN_CPU1 + i*0x4, \
			nvt_gpio_chip_ptr->pm_data.gpio_irq_en[i]);
	}

	enable_irq(nvt_gpio_chip_ptr->irq_base);

	return 0;
}
static const struct dev_pm_ops nvt_gpio_pmops = {
	.suspend	= nvt_gpio_suspend,
	.resume		= nvt_gpio_resume,
};
#define NVT_GPIO_PMOPS &nvt_gpio_pmops
#else
#define NVT_GPIO_PMOPS NULL
#endif /* CONFIG_PM */

#ifdef CONFIG_OF
static const struct of_device_id nvt_gpio_pin_match[] = {
	{ .compatible = "nvt,nvt_gpio" },
	{},
};

MODULE_DEVICE_TABLE(of, nvt_gpio_pin_match);
#endif

static struct platform_driver nvt_gpio_drv = {
	.probe		= nvt_gpio_probe,
	.remove		= nvt_gpio_remove,
	.driver		= {
		.name	= "nvt_gpio",
		.owner	= THIS_MODULE,
		.pm = NVT_GPIO_PMOPS,
#ifdef CONFIG_OF
		.of_match_table = nvt_gpio_pin_match,
#endif
	},
};

static int __init nvt_gpio_init(void)
{
	return platform_driver_register(&nvt_gpio_drv);
}

static void __exit nvt_gpio_exit(void)
{
	platform_driver_unregister(&nvt_gpio_drv);
}

subsys_initcall(nvt_gpio_init);
module_exit(nvt_gpio_exit);

MODULE_AUTHOR("Novatek Microelectronics Corp.");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("GPIO driver for nvt na51000 SOC");
MODULE_VERSION(DRV_VERSION);
