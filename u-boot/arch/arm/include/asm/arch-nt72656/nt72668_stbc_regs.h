/*
 *  arch/arm/include/asm/arch-nt72668/nt72668_stbc_regs.h
 *
 *  Author:	Alvin lin
 *  Created:	April 26, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef __NT72668_STBC_REGS_H__
#define __NT72668_STBC_REGS_H__
#include <asm/types.h>
#include <asm/arch/hardware.h>

#define REG_STBC_BASE			0xFC040000
#define REG_STBC_DOWNLOAD		__REG(REG_STBC_BASE)
#define REG_STBC_UPLOAD		__REG((REG_STBC_BASE + 4))
#define REG_STBC_INT_STAT		__REG((REG_STBC_BASE + 8))
#define REG_STBC_RTC_BASE_REG		__REG((REG_STBC_BASE + 0x100))
#define REG_STBC_RTC_SEC_REG 		__REG((REG_STBC_BASE + 0x104))
#define REG_STBC_RTC_DAY_REG 		__REG((REG_STBC_BASE + 0x108))
#define REG_STBC_RTC_CTRL_REG 		__REG((REG_STBC_BASE + 0x114))

#define STBC_CMD_SHIFT			24
#define STBC_DATA_SHIFT		16
#define STBC_WDOG_TIME_SHIFT	8
#define STBC_CMD_DATA_MASK	0xFF

#define KER_STBC_IPC_CMD_GET_POWER_ON_EVENT 	0x21
#define KER_STBC_IPC_CMD_POWER_OFF				0x31
#define KER_STBC_IPC_CMD_POWER_OFF_BY_KEYPAD	0xB0
#define KER_STBC_IPC_CMD_REPLY_POWER_ON_EVENT   0x31
#define KER_STBC_IPC_CMD_GET_STBC_VERSION		0xB5
#define KER_STBC_IPC_CMD_REPLY_STBC_VERSION		0xB6
#define KER_STBC_IPC_CMD_RESET_STBC 			0xC4

#define IPC_CMD_SYS_LOCK_KEYPAD   (0xA2)       //2014.07.22 for TCL to lock keypad
#define IPC_CMD_SYS_UNLOCK_KEYPAD (0xA3)

#define IPC_CMD_SYS_CEC_AUTO_ON   (0xA6)       //2014.08.12 for TCL CEC auto on/off
#define IPC_CMD_SYS_CEC_AUTO_OFF (0xA7)


#define KER_STBC_IPC_CMD_SAVE_WATCHDOG_SETTING          (0xAD)          //2012.0305
#define KER_STBC_IPC_CMD_ENABLE_WATCHDOG_TIMER          (0xAE)          //2012.0305
#define KER_STBC_IPC_CMD_RESET_WATCHDOG                         (0xAF)          //2012.0305

#define KER_STBC_IPC_CMD_GET_POWERKEY_IDX               0xE5
#define KER_STBC_IPC_CMD_REPLY_POWERKEY_IDX             0xE6
#define KER_STBC_IPC_CMD_SET_POWERKEY                   0xE7
#define KER_STBC_IPC_CMD_REPLY_SET_POWERKEY             0xE8
#define KER_STBC_IPC_CMD_SYS_STBC_IDLE_IN_SRAM          0xE9

#define KER_STBC_IPC_CMD_GET_STBC_REGISTER		0x41

#define STBC_IPC_UPDATE_INT_STATUS			(0x00000100)

#define REG_STBC_PATT                   __REG(0xFC040204)
#define STBC_PATT_0                             0x72682
#define STBC_PATT_1                             0x28627

#define REG_STBC_KEYPASS 		__REG(0xFC040208)
#define STBC_KEY_PASS_EN		(1 << 0)

#define REG_STBC_SWITCH         __REG(0xFC040200)
#define STBC_NFC_OLD_TYPE                       (1 << 4)
#define STBC_STORAGE_CTRL_MODE  (1 << 5)

#define REG_STBC_SPI_MODE      __REG(0xFC040288)
#define SPI_NEW_DESIGN				(1 << 0)

#define GPIO_SWITCH_CTRL			__REG(0xFC040220)
#define GPIO_LED_CTRL_DIR			__REG(0xFC040430)
#define GPIO_LED_CTRL_LEVEL		__REG(0xFC040424)
#define GPIO_LED_CTRL_SWITCH		__REG(0xFC04043C)
//#define GPIO_GROUP0_SWITCH_TO_MIPS		(1 << 9)
#define GPIO_GROUP1_SWITCH_TO_MIPS		(1 << 10)
#define LEDR_DIR_OUT						(1 << 12)
#define LEDR_TURN_ON						(1 << 12)
#define LEDR_SWITCH_GPIO					(1 << 12)
#define LEDG_DIR_OUT						(1 << 22)
#define LEDG_TURN_ON						(1 << 22)
#define LEDG_SWITCH_GPIO					(1 << 22)
#define LED_DIR_OUT						(1 << 26)
#define LED_TURN_ON						(1 << 26)
#define LED_SWITCH_GPIO					(1 << 26)

#define SARADC0_VAL_REG			__REG(0xFC04030C)
#define SARADC1_VAL_REG			__REG(0xFC040310)
#define SARADC2_VAL_REG			__REG(0xFC040314)
#define SARADC3_VAL_REG			__REG(0xFC040318)
#define SARADC4_VAL_REG			__REG(0xFC04031C)
#define SARADC5_VAL_REG			__REG(0xFC040320)
#define SARADC6_VAL_REG			__REG(0xFC040324)
#define SARADC7_VAL_REG			__REG(0xFC040328)

#define STBC_RTC_RESET 		(1 << 0)
#define STBC_RTC_SEC_MSK 	0x3F
#define STBC_RTC_SEC_SFT 	0
#define STBC_RTC_MIN_MSK 	0x3F
#define STBC_RTC_MIN_SFT 	8
#define STBC_RTC_HOUR_MSK 	0xF
#define STBC_RTC_HOUR_SFT 	16

#define REG_SAR_ADC_MUX 		__REG(0xFC04043C)
#define REG_SAR_ADC_DIR 		__REG(0xFC040430)
#define REG_SAR_ADC_CTL 		__REG(0xFC040424)
#define NVT_SAR_ADC0_SFT 		(1 << 8)
#define NVT_SAR_ADC1_SFT 		(1 << 9)
#define NVT_SAR_ADC2_SFT 		(1 << 10)
#define NVT_SAR_ADC3_SFT 		(1 << 11)
#define NVT_SAR_ADC4_SFT 		(1 << 12)
#define NVT_SAR_ADC5_SFT 		(1 << 13)
#if defined(CONFIG_TCL668)
#define REG_STBC_GPIO_AUTHORITY 	__REG(0xFC04041C)
#define REG_TCL_EU_LED_MUX 		__REG(0xFC040438)
#define REG_TCL_EU_LED_DIR 		__REG(0xFC04042C)
#define REG_TCL_EU_LED_CTL 		__REG(0xFC040420)
#define TCL_EU_LED_SFT 			(1 << 6)
#define TCL_EU_LED1_SFT 		(1 << 18)
#define TCL_EU_LED_AUTHORITY_SFT 	(1 << 0)
#define TCL_EU_BL_SFT 			(1 << 8)
#define TCL_EU_BL_AUTHORITY_SFT 	(1 << 2)
#endif

#endif
