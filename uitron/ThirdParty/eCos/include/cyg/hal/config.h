/**
 * @file .../nt171/include/config.h
 *
 * @brief config file for nt72171
 *
 * @author Novatek SP/KSW
 *
 * Copyright 2016 Novatek Microelectronics Corp., Ltd.
 */


#ifndef __CONFIG_NVT72171__

#define __CONFIG_NVT72171__


#define WATCHDOG_ADDR		(0xfd0c0000)
#define S_ROM_CODE_ENABLE   (0)
#define M_ROM_CODE_ENABLE   (0x1 << S_ROM_CODE_ENABLE)
#define ENABLE_ROM_ADDR		(0xfd060100)
#define ENABLE_ROM    		((*((volatile unsigned int *)ENABLE_ROM_ADDR)) & M_ROM_CODE_ENABLE)
#define BYPASS_ROM    		(!ENABLE_ROM)
#define S_SECURE_BOOT   	(9)
#define M_SECURE_BOOT   	(0x1 << S_SECURE_BOOT)
#define SECURE_BOOTING  	((*((volatile unsigned int *)0xfc04023c)) & M_SECURE_BOOT)
#define PERIPH_MEM_BASE     (0xffd00000)
#define AUTH_ENGINE_BASE	(0xeffd4400)
#define SRAM_SHA_TMP_BUF	(0xeffdbf00) // 256B, 0xeffdbf00 ~ 0xeffdc000
#define SRAM_SHA_RESULT_BUF	(0xeffdc000) // 256B, 0xeffdc000 ~ 0xeffdc100
#define DRAM_SHA_TMP_BUF	(0x1ffd1000)
#define DRAM_SHA_RESULT_BUF	(0x1ffd1400)
#define L2_MEM_BASE			(0xffe00000)
#define TAG_RAM_SETUP_LATENCY	(1)
#define TAG_RAM_READ_LATENCY	(1)
#define TAG_RAM_WRITE_LATENCY	(1)
#define DATA_RAM_SETUP_LATENCY	(1)
#define DATA_RAM_READ_LATENCY	(1)
#define DATA_RAM_WRITE_LATENCY	(1)
#define SYS_CLK_REG_BASE		(0xfd020000)
#define MAILBOX_REG_BASE		(0xfd020124)
/*
#define TZASC_MAU0_MEM_BASE		(0xfd700000)
#define TZASC_MAU1_MEM_BASE		(0xfd710000)
#define TZASC_ACP_MEM_BASE  	(0xfd1a0000)
*/

#define S_DC_ADDR_MASK			(0xc0000000)
#define S_DC_START_ADDR			(0x00000000)
#define S_DUC_START_ADDR		(0x40000000)


#define MAILBOX1				(*((volatile unsigned int *)(MAILBOX_REG_BASE + 0x4)))
#define MAILBOX2				(*((volatile unsigned int *)(MAILBOX_REG_BASE + 0x8)))
#define MAILBOX3				(*((volatile unsigned int *)(MAILBOX_REG_BASE + 0xc)))

#define CONFIG_MAU0_BASE	0x00000000
#define CONFIG_MAU0_SIZE	0x40000000
#define CONFIG_MAU1_BASE	0x80000000
#define CONFIG_MAU1_SIZE	0x40000000
#define CONFIG_SOC_PERIPHERAL_AHB_BASE 0xfa000000
#define CONFIG_SOC_PERIPHERAL_AHB_SIZE 0x03000000
#define CONFIG_SOC_PERIPHERAL_APB_BASE 0xfd000000
#define CONFIG_SOC_PERIPHERAL_APB_SIZE 0x02000000

#define CONFIG_SECURE_SRAM_BASE	0xeffd0000
#define CONFIG_SECURE_SRAM_SIZE	0x00010000
#define CONFIG_IMVQ_SRAM_BASE	0xfc130000
#define CONFIG_IMVQ_SRAM_SIZE	0x00010000
#define CONFIG_CPU_PERIPHERAL_BASE	PERIPH_MEM_BASE



#define Security_Check_Bit    15


#define eMMC_Bus_Width_Set_To_8_Bits    5


#define eMMC_ADMA_Support

#define BOOTSTRAP_ADDR	0xfc04023c
#define BOOTSTRAP_REG	*((volatile unsigned int *)BOOTSTRAP_ADDR)
#define BOOTSTRAP_VAL	((BOOTSTRAP_REG >> 6) & 0x7)

#define CHIPID_ADDR		(0xfd020100)
#define CHIPID_REG		(*((volatile unsigned int *)CHIPID_ADDR))
#define CHIPID_VAL		(((CHIPID_REG & 0x0f000000) >> 8) | (CHIPID_REG & 0x0000ffff))

#define UART2_GPIO_ADDR	(0xfd110020)
#define UART2_GPIO_BIT	(11)

#if 0   //CA9
#define S_DC_SETS	8
#define S_DC_W		2
#define S_DC_LS		5

#define _DC_SETS	(1 << S_DC_SETS)	/* 256 sets */
#define _DC_W 		(1 << S_DC_W)		/* 4 way */
#define _DC_LS 		(1 << S_DC_LS)		/* 32byte line size */

#define S_IC_SETS	8
#define S_IC_W		2
#define S_IC_LS		5

#define _IC_SETS	(1 << S_IC_SETS)	/* 256 sets */
#define _IC_W 		(1 << S_IC_W)		/* 4way */
#define _IC_LS 		(1 << S_IC_LS)		/* 32 byte line size */

#define S_SC_SETS	10
#define S_SC_W		4
#define S_SC_LS		5

#define _SC_SETS	(1 << S_SC_SETS)	/* 1024 sets */
#define _SC_W 		(1 << S_SC_W)		/* 16 way */
#define _SC_LS 		(1 << S_SC_LS)		/* 32 byte line size */
#else   //CA53
#define S_DC_SETS	7
#define S_DC_W		2
#define S_DC_LS		6

#define _DC_SETS	(1 << S_DC_SETS)	/* 128 sets */
#define _DC_W 		(1 << S_DC_W)		/* 4 way */
#define _DC_LS 		(1 << S_DC_LS)		/* 64byte line size */

#define S_IC_SETS	8
#define S_IC_W		1
#define S_IC_LS		6

#define _IC_SETS	(1 << S_IC_SETS)	/* 256 sets */
#define _IC_W 		(1 << S_IC_W)		/* 2way */
#define _IC_LS 		(1 << S_IC_LS)		/* 64 byte line size */


#define LEVEL_1_DCACHE  0
#define LEVEL_1_ICACHE  1
#define LEVEL_2_DCACHE  2

#if 0
#define S_SC_SETS	10
#define S_SC_W		4
#define S_SC_LS		5

#define _SC_SETS	(1 << S_SC_SETS)	/* 1024 sets */
#define _SC_W 		(1 << S_SC_W)		/* 16 way */
#define _SC_LS 		(1 << S_SC_LS)		/* 32 byte line size */
#endif

#define S_DC_W_L2   4                   /* L2 way shift*/
#define S_CCSIDR_WT     31
#define M_CCSIDR_WT     (0x1  << S_CCSIDR_WT)       /* Support write-through */
#define S_CCSIDR_WB     30
#define M_CCSIDR_WB     (0x1  << S_CCSIDR_WB)       /* Support write-back */
#define S_CCSIDR_RA     29
#define M_CCSIDR_RA     (0x1  << S_CCSIDR_RA)       /* Support read-allocation */
#define S_CCSIDR_WA     28
#define M_CCSIDR_WA     (0x1  << S_CCSIDR_WA)       /* Support write-allocation */
#define S_CCSIDR_SETS   13
#define M_CCSIDR_SETS   (0x7fff  << S_CCSIDR_SETS)  /* Number of sets */
#define S_CCSIDR_A      3
#define M_CCSIDR_A      (0x3ff  << S_CCSIDR_A)      /* Number of associatiovity */
#define S_CCSIDR_LS     0
#define M_CCSIDR_LS     (0x7  << S_CCSIDR_LS)       /* Cache line size */


#define LEVEL_1 (1 - 1)
#define LEVEL_2 (2 - 1)



#endif


/*
 * UART
 */
#define CFG_UART_0_PORT		0
#define CFG_UART_1_PORT		1
#define CFG_UART_2_PORT		2

/*
 * backup region for SECOS
 */
#define DRAM_UC_START		(0x40000000)	/* address 0 - uncacheable */
#define DRAM_UC_LOC			(0xefedc200)	/* backup buffer - uncachedable */
#define DRAM_UC_LOC_SIZE	256

#endif //__CONFIG_NVT72171__

