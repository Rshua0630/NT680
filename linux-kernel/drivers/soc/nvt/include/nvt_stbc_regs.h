/*
 *  arch/arm/include/asm/mach-nvt72668/nvt72668_stbc_regs.h
 *
 *  Author:	Z Y Chai
 *  Created:	Oct 10, 2013
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NVT72668_STBC_REGS_H__
#define __NVT72668_STBC_REGS_H__

#define __REG(x)  (x)

#define REG_STBC_BASE			0xFC040000
#define REG_STBC_DOWNLOAD	__REG(REG_STBC_BASE)
#define REG_STBC_UPLOAD		__REG((REG_STBC_BASE + 4))
#define REG_STBC_INT_STAT		__REG((REG_STBC_BASE + 8))

#define REG_STBC_RTC_BASE_REG		__REG((REG_STBC_BASE + 0x100))
#define REG_STBC_RTC_SEC_REG		__REG((REG_STBC_BASE + 0x104))
#define REG_STBC_RTC_DAY_REG		__REG((REG_STBC_BASE + 0x108))
#define REG_STBC_RTC_CTRL_REG		__REG((REG_STBC_BASE + 0x114))
#define REG_STBC_RTC_SWITCH_REG		__REG((REG_STBC_BASE + 0x200))
#define REG_STBC_BOOTSTAP_REG		__REG((REG_STBC_BASE + 0x210))

#define REG_STBC_END			0xFC040288

#define REG_STBC_SRAM_BASE_REG		__REG((REG_STBC_BASE + 0x1000))
#define STBC_SRAM_SIZE				0x2000

#define STBC_CMD_SHIFT			24
#define STBC_DATA_SHIFT			16
#define STBC_DATA1_SHIFT		8
#define STBC_WDOG_TIME_SHIFT		8
#define STBC_CMD_DATA_MASK		0xFF
#define STBC_TIME_HOUR_SHIFT		16
#define STBC_TIME_MIN_SHIFT		8
#define STBC_TIME_SEC_SHIFT		0

#define KER_STBC_IPC_CMD_SET_CURRENT_TIME                (0x01)
#define KER_STBC_IPC_CMD_GET_CURRENT_TIME                (0x02)
#define KER_STBC_IPC_CMD_SET_WAKE_UP_TIME                (0x05)
#define KER_STBC_IPC_CMD_GET_WAKE_UP_TIME                (0x06)
#define KER_STBC_IPC_CMD_SET_POWER_OFF_TIME              (0x07)
#define KER_STBC_IPC_CMD_GET_POWER_OFF_TIME              (0x08)
#define KER_STBC_IPC_CMD_SET_CURRENT_DAY                 (0x09)
#define KER_STBC_IPC_CMD_GET_CURRENT_DAY                 (0x0A)
#define KER_STBC_IPC_CMD_GET_POWER_ON_EVENT			0x21
#define KER_STBC_IPC_CMD_POWER_OFF				0x31
#define KER_STBC_IPC_CMD_REPLY_POWER_ON_EVENT   0x31
#define KER_STBC_IPC_CMD_RESET                           (0x33)
#define KER_STBC_IPC_CMD_RESET_STBC                      (0xC4)
#define KER_STBC_IPC_CMD_SYS_STBC_IDLE_IN_SRAM          (0xE9)
#define KER_STBC_IPC_CMD_SYS_STBC_EXIT_IDLE		(0xEA)

#define KER_STBC_IPC_CMD_SAVE_WATCHDOG_SETTING          (0xAD)
#define KER_STBC_IPC_CMD_ENABLE_WATCHDOG_TIMER          (0xAE)
#define KER_STBC_IPC_CMD_RESET_WATCHDOG                         (0xAF)

#define KER_STBC_IPC_CMD_REPLY_CURRENT_TIME              (0x01)
#define KER_STBC_IPC_CMD_REPLY_WAKE_UP_TIME              (0x03)
#define KER_STBC_IPC_CMD_REPLY_POWER_OFF_TIME            (0x04)
#define KER_STBC_IPC_CMD_REPLY_CURRENT_DAY               (0x05)
#define KER_STBC_IPC_CMD_REPLY_POWER_OFF_REMINDER_TIME   (0x06)
#define KER_STBC_IPC_CMD_REPLY_WAKE_UP_DATE              (0x09)
#define KER_STBC_IPC_CMD_READ_REGISTER (0x41)
#define KER_STBC_IPC_CMD_WRITE_REGISTER (0x42)

#define KER_STBC_IPC_CMD_ACK_CMD                         (0xF1)
#define KER_STBC_IPC_CMD_ACK_READ_REG                    (0x41000000)
#define KER_STBC_IPC_CMD_ACK_WRITE_REG                   (0xF1420000)

#define STBC_IPC_UPDATE_INT_STATUS			(0x00000100)


#define REG_STBC_PATTERN			__REG((REG_STBC_BASE + 0x204))
#define STBC_PATT_START					0x72682
#define STBC_PATT_END						0x28627

#define REG_STBC_KEYPASS			__REG((REG_STBC_BASE + 0x208))
#define STBC_KEY_PASS_EN					(1 << 0)

#define STBC_RTC_RESET				(1 << 0)
#define STBC_RTC_SEC_MSK			0x3F
#define STBC_RTC_SEC_SFT			0
#define STBC_RTC_MIN_MSK			0x3F
#define STBC_RTC_MIN_SFT			8
#define STBC_RTC_HOUR_MSK			0x1F
#define STBC_RTC_HOUR_SFT			16
#define STBC_RTC_SWITCH_ARM_SFT 7

#if 0
#define REG_STBC_SWITCH         __REG(0xBC040200)
#define STBC_NFC_OLD_TYPE                       (1 << 4)
#define STBC_STORAGE_CTRL_MODE  (1 << 5)

#define REG_STBC_SPI_MODE      __REG(0xBC040288)
#define SPI_NEW_DESIGN				(1 << 0)

#endif

#endif
