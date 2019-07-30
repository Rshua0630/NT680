#ifndef __SCU_DEFS_H__
#define __SCU_DEFS_H__

#include <cyg/hal/config.h>

#define SCU_MEM_BASE	(PERIPH_MEM_BASE + 0x000)

#define SCU_CTRL					(*((volatile unsigned long *)(SCU_MEM_BASE + 0x00)))	/* SCU Control register */
#define SCU_CONF					(*((volatile unsigned long *)(SCU_MEM_BASE + 0x04)))	/* SCU Configuration register */
#define SCU_POWER_ST				(*((volatile unsigned long *)(SCU_MEM_BASE + 0x08)))	/* SCU CPU power status register */
#define SCU_INV_ALL					(*((volatile unsigned long *)(SCU_MEM_BASE + 0x0c)))	/* SCU Invalidate all registers in Seccure state */
#define SCU_ADDR_FILTERING_START	(*((volatile unsigned long *)(SCU_MEM_BASE + 0x40)))	/* Filtering start address register */
#define SCU_ADDR_FILTERING_END		(*((volatile unsigned long *)(SCU_MEM_BASE + 0x44)))	/* Filtering end address register */
#define SCU_SAC						(*((volatile unsigned long *)(SCU_MEM_BASE + 0x50)))	/* SCU access control (SAC) register */
#define SCU_SNSAC					(*((volatile unsigned long *)(SCU_MEM_BASE + 0x54)))	/* SCU non-secure access control (SNSAC) register */

#define S_SCU_CTRL_IC_STANDBY_EN		(6)
#define M_SCU_CTRL_IC_STANDBY_EN		(0x1 << S_SCU_CTRL_IC_STANDBY_EN)
#define S_SCU_CTRL_SCU_STANDBY_EN		(5)
#define M_SCU_CTRL_SCU_STANDBY_EN		(0x1 << S_SCU_CTRL_SCU_STANDBY_EN)
#define S_SCU_CTRL_FORCE_DEV_PORT0_EN	(4)
#define M_SCU_CTRL_FORCE_DEV_PORT0_EN	(0x1 << S_SCU_CTRL_FORCE_DEV_PORT0_EN)
#define S_SCU_CTRL_SPECULATIVE_LF_EN	(3)
#define M_SCU_CTRL_SPECULATIVE_LF_EN	(0x1 << S_SCU_CTRL_SPECULATIVE_LF_EN)
#define S_SCU_CTRL_RAM_PAR_EN			(2)
#define M_SCU_CTRL_RAM_PAR_EN			(0x1 << S_SCU_CTRL_RAM_PAR_EN
#define S_SCU_CTRL_ADDR_FLLTERING_EN	(1)
#define M_SCU_CTRL_ADDR_FLLTERING_EN	(0x1 << S_SCU_CTRL_ADDR_FLLTERING_EN)
#define S_SCU_CTRL_SCU_EN				(0)
#define M_SCU_CTRL_SCU_EN				(0x1 << S_SCU_CTRL_SCU_EN)

#define K_SCU_CTRL_SCU_EN_ON			(1)
#define K_SCU_CTRL_SCU_EN_OFF			(0)

#define S_SCU_CONF_TAG_RAM_SIZE			(8)
#define M_SCU_CONF_TAG_RAM_SIZE			(0xff << S_SCU_CONF_TAG_RAM_SIZE)
#define S_SCU_CONF_CPUS_SMP				(4)
#define M_SCU_CONF_CPUS_SMP				(0xf << S_SCU_CONF_CPUS_SMP)
#define S_SCU_CONF_NO_CPUS				(0)
#define M_SCU_CONF_NO_CPUS				(0x3 << S_SCU_CONF_NO_CPUS)

#define S_SCU_POWER_ST_CPU3_ST			(24)
#define M_SCU_POWER_ST_CPU3_ST			(0x3 << S_SCU_POWER_ST_CPU3_ST)
#define S_SCU_POWER_ST_CPU2_ST			(16)
#define M_SCU_POWER_ST_CPU2_ST			(0x3 << S_SCU_POWER_ST_CPU2_ST)
#define S_SCU_POWER_ST_CPU1_ST			(8)
#define M_SCU_POWER_ST_CPU1_ST			(0x3 << S_SCU_POWER_ST_CPU1_ST)
#define S_SCU_POWER_ST_CPU0_ST			(0)
#define M_SCU_POWER_ST_CPU0_ST			(0x3 << S_SCU_POWER_ST_CPU0_ST)

#define K_SCU_POWER_ST_NORMAL			(0)
#define K_SCU_POWER_ST_DORMANT			(2)
#define K_SCU_POWER_ST_POWER_OFF		(3)

#define S_SCU_INV_ALL_CPU3_WAYS			(15)
#define M_SCU_INV_ALL_CPU3_WAYS			(0xf << S_SCU_INV_ALL_CPU3_WAYS)
#define S_SCU_INV_ALL_CPU2_WAYS			(8)
#define M_SCU_INV_ALL_CPU2_WAYS			(0xf << S_SCU_INV_ALL_CPU2_WAYS)
#define S_SCU_INV_ALL_CPU1_WAYS			(4)
#define M_SCU_INV_ALL_CPU1_WAYS			(0xf << S_SCU_INV_ALL_CPU1_WAYS)
#define S_SCU_INV_ALL_CPU0_WAYS			(0)
#define M_SCU_INV_ALL_CPU0_WAYS			(0xf << S_SCU_INV_ALL_CPU0_WAYS)

#endif /* __SCU_DEFS_H__ */

