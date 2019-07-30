/*
 *  board/novatek/evb668/nt72668_stbc.c
 *
 *  Author:	Alvin lin
 *  Created:	April 29, 2013
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <asm/arch/nt72668_stbc_regs.h>
#include <asm/arch/nt72668_stbc.h>
#include "nt72668_emmc_fwupdate.h"
#include <errno.h>

#define KEYPAD_LOCK_M
#define AUTO_CEC_M

FAC_STBC_DATA  __attribute__((__aligned__(64))) gstStbcData_power;
USR_STBC_DATA  __attribute__((__aligned__(64))) gstStbcData_usr;

EN_DRV_STBC_POWER_ON_EVENT genPowerOnEvent;

void nt72668_stbc_unlock_reg(void)
{
	if(!(REG_STBC_KEYPASS & STBC_KEY_PASS_EN)) {
		REG_STBC_PATT = STBC_PATT_0;
		REG_STBC_PATT = STBC_PATT_1;
	}
}

void nt72668_stbc_set_keypass(int enable)
{
	nt72668_stbc_unlock_reg();

	if(enable)
		REG_STBC_KEYPASS |= STBC_KEY_PASS_EN;
	else
		REG_STBC_KEYPASS &= ~STBC_KEY_PASS_EN;
}


static int nt72668_stbc_wait_response(void)
{
	unsigned long	tmo, out;

	tmo = get_timer(0);
	out = tmo + (1000/5) ; 	/*timeout 0.2 s*/

	while(get_timer(0) < out) {
		if(REG_STBC_INT_STAT & STBC_IPC_UPDATE_INT_STATUS) {
			REG_STBC_INT_STAT |= STBC_IPC_UPDATE_INT_STATUS;
			return 1;
		}
	}

	printf("%s timeout !\n",__func__);
	return 0;
}


void nt72668_stbc_stay_in_sram(void)
{
	unsigned int reg_val;

	nt72668_stbc_unlock_reg();

	REG_STBC_DOWNLOAD = (KER_STBC_IPC_CMD_SYS_STBC_IDLE_IN_SRAM << STBC_CMD_SHIFT);

	if(nt72668_stbc_wait_response() == 0) {
		printf("stbc stay in sram command timeout !\n");
		return;
	}

	reg_val = REG_STBC_UPLOAD;
}

//If singlely use this function, the system will be reseted.
//If use this function after nt72668_stbc_stay_in_sram, it not reset the system, only jump out of stay in sram
void nt72668_stbc_reset(void)
{
	nt72668_stbc_unlock_reg();

	REG_STBC_DOWNLOAD = (KER_STBC_IPC_CMD_RESET_STBC << STBC_CMD_SHIFT);

}

void nt72668_stbc_lock_keypad(void)
{
	nt72668_stbc_unlock_reg();
	REG_STBC_DOWNLOAD = (IPC_CMD_SYS_LOCK_KEYPAD << STBC_CMD_SHIFT);
	
	if(nt72668_stbc_wait_response() == 0) {
		printf("stbc lock keypad command timeout !\n");
		return;
	}
}

void nt72668_stbc_unlock_keypad(void)
{
	nt72668_stbc_unlock_reg();
	REG_STBC_DOWNLOAD = (IPC_CMD_SYS_UNLOCK_KEYPAD << STBC_CMD_SHIFT);

	if(nt72668_stbc_wait_response() == 0) {
		printf("stbc unlock keypad command timeout !\n");
		return;
	}
}

void nt72668_stbc_cec_auto_on(void)
{
	nt72668_stbc_unlock_reg();
	REG_STBC_DOWNLOAD = (IPC_CMD_SYS_CEC_AUTO_ON << STBC_CMD_SHIFT);
	
	if(nt72668_stbc_wait_response() == 0) {
		printf("stbc enable cec command timeout !\n");
		return;
	}
}

void nt72668_stbc_cec_auto_off(void)
{
	nt72668_stbc_unlock_reg();
	REG_STBC_DOWNLOAD = (IPC_CMD_SYS_CEC_AUTO_OFF << STBC_CMD_SHIFT);
	
	if(nt72668_stbc_wait_response() == 0) {
		printf("stbc disable cec command timeout !\n");
		return;
	}

}

void nt72668_stbc_shutdown_arm(void)
{
	nt72668_stbc_unlock_reg();
	REG_STBC_DOWNLOAD = (KER_STBC_IPC_CMD_POWER_OFF << STBC_CMD_SHIFT);

	if(nt72668_stbc_wait_response() == 0) {
		printf("stbc shutdown arm command timeout !\n");
		return;
	}
}

int nt72668_stbc_read_power_data(FAC_STBC_DATA* pData)
{
	int ret;
	char cmd[64];
	int partition = POWERON_MODE_PARTITON;

	sprintf(cmd,"ext4load mmc 0:%d 0x%x %s 0x%x", partition, pData, POWERON_MODE_NAME, sizeof(FAC_STBC_DATA));

	ret = run_command(cmd, 0);
	if(ret != 0) {
		printf("Read file error, use default settings!\n");
	}
	else
	{
		return 0;
	}

	//Default situation
	{
		memcpy(pData->Sign,STBCDATA_SIGN,4);
		pData->PowerOnMode = POWERON_MODE_LAST;
	}

	return 0;

}

int nt72668_stbc_read_usr_data(USR_STBC_DATA* pData)
{
	int ret;
	char cmd[64];
	int partition = USR_MODE_PARTITON;

	sprintf(cmd,"ext4load mmc 0:%d 0x%x %s 0x%x", partition, pData, USR_MODE_NAME, sizeof(USR_STBC_DATA));

	ret = run_command(cmd, 0);
	if(ret != 0) {
		printf("Read file error, use default settings!\n");
	}
	else
	{
		return 0;
	}

	//Default situation
	{
		memcpy(pData->Sign,STBCDATA_SIGN,4);
		pData->PowerStatus = STBC_PWR_ON;
		pData->BacklightOnFlag = BACKLIGHT_ON;
		pData->KeypadLockFlag = KEYPAD_UNLOCK;
		pData->CecAutoPowerOnMode = AUTO_POWER_ON;
	}

	return 0;

}

int nt72668_stbc_boot_check(void)
{
	nt72668_stbc_get_clock_divider();

	/* Get Power on event */
	genPowerOnEvent = nt72668_stbc_read_power_on_event();

	/* Get Power on mode */
	nt72668_stbc_read_power_data(&gstStbcData_power);
	nt72668_stbc_read_usr_data(&gstStbcData_usr);


#ifdef KEYPAD_LOCK_M
	if(gstStbcData_usr.KeypadLockFlag == KEYPAD_UNLOCK)
	{
		//send cmd to stbc
		//nt72668_stbc_unlock_keypad();
	}
	else if(gstStbcData_usr.KeypadLockFlag == KEYPAD_LOCK)
	{
		//send cmd to stbc
		nt72668_stbc_lock_keypad();
	}
	else
	{
		printf("wrong KeypadLockFlag!\n ");
	}
#endif

#ifdef AUTO_CEC_M
if(gstStbcData_usr.CecAutoPowerOnMode == AUTO_POWER_ON)
{
	//send cmd to stbc
	//nt72668_stbc_cec_auto_on();
}
else if(gstStbcData_usr.CecAutoPowerOnMode == AUTO_POWER_OFF)
{
	//send cmd to stbc
	nt72668_stbc_cec_auto_off();
}
else
{
	printf("wrong CecAutoPowerOnMode!\n ");
}
#endif

	if(genPowerOnEvent == EN_DRV_STBC_POWER_ON_EVENT_AC_POWER_TO_ON)
	{
		switch(gstStbcData_power.PowerOnMode) {
			case POWERON_MODE_ON:
				printf("Power on mode: Always on!\n");
				break;

			case POWERON_MODE_STB:
				printf("Power on mode: Always off!\n");
				nt72668_stbc_shutdown_arm();
				break;

			case POWERON_MODE_LAST:
			default:
				if(gstStbcData_usr.PowerStatus == STBC_PWR_STANDBY)
				{
					printf("Power on mode: last mode is Standby!\n");
					nt72668_stbc_shutdown_arm();
				}
				else
				{
					printf("Power on mode: last mode is On!\n");
				}
				break;
		}
	}

	return 0;

}


void nt72668_stbc_enable_wdog(unsigned short sec)
{
	unsigned short msec = sec * 1000;
	unsigned int reg_val;

	nt72668_stbc_unlock_reg();
	REG_STBC_INT_STAT |= STBC_IPC_UPDATE_INT_STATUS;
	REG_STBC_DOWNLOAD  = (KER_STBC_IPC_CMD_ENABLE_WATCHDOG_TIMER << STBC_CMD_SHIFT) | (msec << STBC_WDOG_TIME_SHIFT);

	if(nt72668_stbc_wait_response() == 0) {
		printf("enable wdog no response !\n");
		return;
	}

	reg_val = REG_STBC_UPLOAD;


}

void nt72668_stbc_disable_wdog(void)
{
	unsigned int reg_val;

	nt72668_stbc_unlock_reg();
	REG_STBC_INT_STAT |= STBC_IPC_UPDATE_INT_STATUS;
	REG_STBC_DOWNLOAD  = (KER_STBC_IPC_CMD_ENABLE_WATCHDOG_TIMER << STBC_CMD_SHIFT) | (0 << STBC_WDOG_TIME_SHIFT);

	if(nt72668_stbc_wait_response() == 0) {
		printf("disable wdog no response !\n");
		return;
	}

	reg_val = REG_STBC_UPLOAD;

}

unsigned int nt72668_stbc_read_power_on_event(void)
{
	unsigned int reg_val;

	nt72668_stbc_unlock_reg();

	REG_STBC_INT_STAT |= STBC_IPC_UPDATE_INT_STATUS;
	REG_STBC_DOWNLOAD = (KER_STBC_IPC_CMD_GET_POWER_ON_EVENT << STBC_CMD_SHIFT);

	if(nt72668_stbc_wait_response() == 0) {
		printf("disable wdog no response !\n");
		return 0xFF;
	}

	reg_val = REG_STBC_UPLOAD;

	reg_val >>= STBC_DATA_SHIFT;

	reg_val &= STBC_CMD_DATA_MASK;

	switch(reg_val)
	{
		case EN_DRV_STBC_POWER_ON_EVENT_AC_POWER_TO_ON:
			printf("Power on AC!\n");
			break;

		case EN_DRV_STBC_POWER_ON_EVENT_RC_PWR_KEY:
			printf("Power on IR!\n");
			break;

		case EN_DRV_STBC_POWER_ON_EVENT_FP_PWR_KEY:
			printf("Power on Key pad!\n");
			break;

		case EN_DRV_STBC_POWER_ON_EVENT_WAKE_UP_TIME_MATCH:
			printf("Power on Alarm time match!\n");
			break;

		case EN_DRV_STBC_POWER_ON_EVENT_CEC_POWER_ON:
			printf("Power on CEC!\n");
			break;

		case EN_DRV_STBC_POWER_ON_EVENT_SCART_PIN8_EXT1:
			printf("Power on SCART wake up!\n");
			break;

		case EN_DRV_STBC_POWER_ON_EVENT_WDT_RESET:
			printf("Power on watchdog time out!\n");
			break;

        case EN_DRV_STBC_POWER_ON_EVENT_RESET:
            printf("Power on reboot!\n");
            break;

        case EN_DRV_STBC_POWER_ON_EVENT_LONG_PRESS_KEYPAD_RESET:
            printf("Power on long press Key pad!\n");
            break;

		default:
			printf("Power on unknown event 0x%x!\n", reg_val);
			break;
	}

	return reg_val;
}

int do_nvt_reboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nt72668_stbc_reset();

	return 0;
}

U_BOOT_CMD(
	reboot, 3, 1, do_nvt_reboot,
	"reboot - reboot system",
	"       reboot system\n"
);


int do_nvt_halt(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nt72668_stbc_shutdown_arm();

	return 0;
}

U_BOOT_CMD(
	halt, 3, 1, do_nvt_halt,
	"halt - halt system",
	"       halt system\n"
);

void nt72668_stbc_sar_adc_init(void)
{
	//init sar adc0,1 as input
	nt72668_stbc_unlock_reg();
	REG_SAR_ADC_MUX |= (NVT_SAR_ADC0_SFT | NVT_SAR_ADC1_SFT);

	nt72668_stbc_unlock_reg();
	REG_SAR_ADC_DIR &= ~(NVT_SAR_ADC0_SFT | NVT_SAR_ADC1_SFT);
	
#if defined(CONFIG_TCL_CN_668) ||defined(CONFIG_TCL_EU_668) || defined(CONFIG_SRI668)
	(*((volatile u32 *) (0xFC040330))) = 0x3; //reset assert
	(*((volatile u32 *) (0xFC040330))) = 0x2; //reset de-assert 
	(*((volatile u32 *) (0xFC040308))) |= 0x01;
#endif
}

#if defined(CONFIG_TCL_CN_668) ||defined(CONFIG_TCL_EU_668) || defined(CONFIG_SRI668)      //change adc checking way
int nt72668_stbc_sar_adc_pressed(NVT_SAR_ADC_ID id)
{
	int not_pressed = 1;

	switch(id) {
		case NVT_SAR_ADC0:
			not_pressed = SARADC0_VAL_REG;
			break;

		case NVT_SAR_ADC1:
			not_pressed = SARADC1_VAL_REG;
			break;

		case NVT_SAR_ADC2:
			not_pressed = SARADC2_VAL_REG;
			break;

		case NVT_SAR_ADC3:
			not_pressed = SARADC3_VAL_REG;
			break;

		case NVT_SAR_ADC4:
			not_pressed = SARADC4_VAL_REG;
			break;

		case NVT_SAR_ADC5:
			not_pressed = SARADC5_VAL_REG;
			break;

		default:
			break;
	}
	//printf("not_pressed=%d\n",not_pressed);
	if((not_pressed == 0) ||((not_pressed<229) && (not_pressed>206)))
		return 1;
	else
		return 0;

}
#else
int nt72668_stbc_sar_adc_pressed(NVT_SAR_ADC_ID id)
{
	int not_pressed = 1;

	switch(id) {
		case NVT_SAR_ADC0:
			not_pressed = (REG_SAR_ADC_CTL & NVT_SAR_ADC0_SFT);
			break;

		case NVT_SAR_ADC1:
			not_pressed = (REG_SAR_ADC_CTL & NVT_SAR_ADC1_SFT);
			break;

		case NVT_SAR_ADC2:
			not_pressed = (REG_SAR_ADC_CTL & NVT_SAR_ADC2_SFT);
			break;

		case NVT_SAR_ADC3:
			not_pressed = (REG_SAR_ADC_CTL & NVT_SAR_ADC3_SFT);
			break;

		case NVT_SAR_ADC4:
			not_pressed = (REG_SAR_ADC_CTL & NVT_SAR_ADC4_SFT);
			break;

		case NVT_SAR_ADC5:
			not_pressed = (REG_SAR_ADC_CTL & NVT_SAR_ADC5_SFT);
			break;

		default:
			break;
	}

	if(not_pressed)
		return 0;
	else
		return 1;

}
#endif

#if defined(CONFIG_TCL668)

void nt72668_stbc_tcl_led_init(void)
{
	nt72668_stbc_unlock_reg();
	REG_STBC_GPIO_AUTHORITY |= TCL_EU_LED_AUTHORITY_SFT;

	nt72668_stbc_unlock_reg();
	REG_TCL_EU_LED_MUX |=  (TCL_EU_LED_SFT | TCL_EU_LED1_SFT);

	nt72668_stbc_unlock_reg();
	REG_TCL_EU_LED_DIR |= (TCL_EU_LED_SFT | TCL_EU_LED1_SFT);
}

void nt72668_stbc_tcl_bl_init(void)
{
	nt72668_stbc_unlock_reg();
	REG_STBC_GPIO_AUTHORITY |= TCL_EU_BL_AUTHORITY_SFT;

	nt72668_stbc_unlock_reg();
	REG_TCL_EU_LED_MUX |=  TCL_EU_BL_SFT;

	nt72668_stbc_unlock_reg();
	REG_TCL_EU_LED_DIR |= TCL_EU_BL_SFT;
}

void nt72668_stbc_tcl_led_ctl(TCL_EU_LED_STATE state)
{
	static int cnt = 0;
	static int toggle = 0;
	static const int cnt_max = 10;

	nt72668_stbc_unlock_reg();

	switch(state) {
		case TCL_EU_LED_ON:
			REG_TCL_EU_LED_CTL |= (TCL_EU_LED_SFT | TCL_EU_LED1_SFT);
			break;

		case TCL_EU_LED_OFF:
			REG_TCL_EU_LED_CTL &= ~(TCL_EU_LED_SFT | TCL_EU_LED1_SFT);
			break;

		case TCL_EU_LED_BLINK:
			if(cnt >= cnt_max) {
				if(toggle)
					REG_TCL_EU_LED_CTL |= (TCL_EU_LED_SFT | TCL_EU_LED1_SFT);
				else
					REG_TCL_EU_LED_CTL &= ~(TCL_EU_LED_SFT | TCL_EU_LED1_SFT);

				toggle = (toggle == 0) ? 1 : 0;
				cnt = 0;
			}
			cnt++;
			break;
	}

}
#endif

void nt72668_get_MAC()
{
	int ret;
	char cmd[64];
	char  __attribute__((__aligned__(64))) buf[64] = {0};
	char mac[20] = {0};
	int partition = POWERON_MODE_PARTITON;

	sprintf(cmd,"ext4load mmc 0:%d 0x%x %s 0x%x", partition, buf, MAC_FILE_NAME, 0);
	ret = run_command(cmd, 0);
	if(ret != 0) {
		return;
	}

	sprintf(mac, "%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",
			   buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

	setenv("ethaddr", mac);

	return;

}

unsigned int nt72668_stbc_get_clock_divider(void)
{
	unsigned int reg_val;

	nt72668_stbc_unlock_reg();

	REG_STBC_INT_STAT |= STBC_IPC_UPDATE_INT_STATUS;
	REG_STBC_DOWNLOAD = (KER_STBC_IPC_CMD_GET_STBC_REGISTER << STBC_CMD_SHIFT) | 0xF1BF00;

	if(nt72668_stbc_wait_response() == 0) {
		printf("disable wdog no response !\n");
		return 0xFF;
	}

	reg_val = REG_STBC_UPLOAD;

	reg_val >>= STBC_DATA_SHIFT;

	reg_val &= STBC_CMD_DATA_MASK;

	if(reg_val == EN_CLOCK_DIVIDER_12M)
		printf("\nSTBC clock is 12MHz\n");
	else if(reg_val == EN_CLOCK_DIVIDER_6M)
		printf("\nSTBC clock is 6MHz\n");
	else if(reg_val ==EN_CLOCK_DIVIDER_3M)
		printf("\nSTBC clock is 3MHz\n");

	return reg_val;

}

