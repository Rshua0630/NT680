/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-KSW <SP_KSW_MailGrp@novatek.com.tw>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include "../../../drivers/mmc/nt72668_mmc.h"


DECLARE_GLOBAL_DATA_PTR;

const char *boardinfo[] = {
	"Board: NT72656 DTV Turnkey board\n"
};

/**
 * @brief board_init
 *
 * @return 0
 */
int board_init(void)
{
	gd->bd->bi_arch_number = 0x104d;
	gd->bd->bi_boot_params = 0x06408000;
	usb_power_on();
	return 0;
}

int board_mmc_init(bd_t *bis)
{
	nt72668_mmc_init();
	return 0;
}

#if 0 
int board_nand_init(bd_t *bis)
{
	return 0;
}
#endif
int board_eth_init(bd_t *bis)
{
	return 0;
}

/*
* Routine: get_board_revision
* Description: Detect if we are running on a NT72656 Turnkey board.
*/
int get_board_revision(void)
{
	int board_id;
	return board_id;
}

/**
 * @brief misc_init_r - Configure Panda board specific configurations
 * such as power configurations, ethernet initialization as phase2 of
 * boot sequence
 *
 * @return 0
 */
int misc_init_r(void)
{
	get_board_revision();
	return 0;
}

/*
 * get_board_rev() - get board revision
 */
u32 get_board_rev(void)
{
	return 0;
}

void nt72668_reg_preinit(void)
{
    //TK project always close external phy clk
    nt72668_stbc_set_keypass(1);
    __REG(0xFC040904) &= ~(1 << 17);
}

#define DDR_SIGN "DDRC"
#define DDR_TOOL_VERSION 3000
#define DDR_SRAM_START   0xFC150000

typedef struct {
	char ddr_sign[4];
	char ddr_fname[64];
	unsigned short ddr_sys_sz;
	unsigned short ddr_sys_cpus;
	unsigned int ddr_bin_max_sz;
	unsigned int ddr_gpio_reg;
	unsigned int ddr_gpio_val;
	unsigned short ddr_cmd_cnt;
	unsigned short ddr_tool_ver;
} nt72668_ddr_hdr;



void nt72668_print_sys_info(void)
{
	nt72668_ddr_hdr *pddr_hdr = (nt72668_ddr_hdr*)DDR_SRAM_START;

	if(memcmp(pddr_hdr->ddr_sign,DDR_SIGN,sizeof(pddr_hdr->ddr_sign)) == 0) {
		printf("\nDDRF: %s\n",pddr_hdr->ddr_fname);
		printf("DDRS: %d MB\n",pddr_hdr->ddr_sys_sz);
		if(pddr_hdr->ddr_gpio_reg != 0) {
			printf("DDR GPIO REG: 0X%X\n",pddr_hdr->ddr_gpio_reg);
			printf("DDR GPIO VAL: 0X%X\n",pddr_hdr->ddr_gpio_val);
		}
	}

	if(pddr_hdr->ddr_sys_cpus >= CPU_NUM_MAX || pddr_hdr->ddr_sys_cpus < 1)
		pddr_hdr->ddr_sys_cpus = CPU_NUM_MAX;

	printf("\nCPU:  %d MHz x %d COREs\n",get_arm_clk()/1000, pddr_hdr->ddr_sys_cpus);
	printf("AXI:  %d MHz\n",get_axi_clk()/1000000);
	printf("AHB:  %d MHz\n",get_ahb_clk()/1000000);
	printf("DDR:  %d MHz\n",get_ddr_clk());
	printf("EMMC: %d MHz\n",get_emmc_clk()/1000000);

}

#if defined(CONFIG_EVB_668)

void nt72668_tk_burn_image(void)
{
	int ret;
	u32 reg_val, tmp_val;

	reg_val = __REG(0xFC040700);

	tmp_val = (reg_val >> 14) & 1;

	setenv("usb_port","3");

	if (tmp_val == 0)
		run_command("mmc bootbus 0 2 0 2", 0);
	else
		run_command("mmc bootbus 0 2 0 1", 0);

	ret = run_command("ewrite xboot", 0);
	if(ret < 0) {
		printf("write xboot fail ...!\n");
		goto out;
	}
	printf("write xboot done ...\n");

	run_command("sf erase 0x0 0x100000", 0);

	ret = run_command("swrite stbc", 0);

	if(ret < 0) {
		printf("write stbc  fail ...!\n");
		goto out;
	}
	printf("write stbc done ...\n");


	ret = run_command("ewrite stbc  stbc_emmc.bin", 0);

	if(ret < 0) {
		printf("write stbc emmc fail ...!\n");
		goto out;
	}
	printf("write emmc stbc done ...\n");


	ret = run_command("ewrite ddrcfg", 0);

	if(ret < 0) {
		printf("write ddrcfg fail !\n");
		goto out;
	}

	ret = run_command("ewrite uboot", 0);
	if(ret < 0) {
		printf("write U-Boot fail ...!\n");
		goto out;
	}
	printf("write U-Boot done...\n");

	ret = run_command("xsave panel config xbootdat.bin", 0);
	if(ret < 0) {
		printf("write panel config fail !\n");
		goto out;
	}

	ret = run_command("ewrite ker0", 0);
	if(ret < 0) {
		printf("write kernel fail ...!\n");
		goto out;
	}
	printf("write kernel done...\n");

	ret = run_command("ewrite ker1", 0);
	if(ret < 0) {
		printf("write recovery fail ...!\n");
		goto out;
	}
	printf("write recovery done...\n");


	ret = run_command("epart write", 0);
	if(ret < 0) {
		printf("write partition fail ...!\n");
		goto out;
	}
	printf("write partition done ...\n");

	ret = run_command("ewrite system ", 0);
	if(ret < 0) {
		printf("write android image fail ...!\n");
		goto out;
	}

	run_command("env default -a", 0);

	run_command("emiscmd \"\"", 0);

	ret = run_command("xsave kercmd", 0);
	if(ret < 0) {
		printf("generate kernel command line fail !\n");
		goto out;
	}

	ret = run_command("xsave rcvcmd", 0);
	if(ret < 0) {
		printf("generate recovery command line fail !\n");
		goto out;
	}

	printf("Burning image process complete, please reboot this device.\n");

	//auto reboot
	nt72668_stbc_set_keypass(1);

	//switch authority
	__REG(0xFC04041C) |= ((1 << 2) | (1 << 0));

	//set STBC_GPIO0,2 set as input
	__REG(0xFC040438) |= (1 << 6);
	__REG(0xFC04042C) &= ~(1 << 6);
	__REG(0xFC040438) |= (1 << 8);
	__REG(0xFC04042C) &= ~(1 << 8);

	__REG(0xFC040210) = 0xF0;

	while(1);

out:
	return;
}

int do_tk_burn(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nt72668_tk_burn_image();
	return 0;
}

U_BOOT_CMD(
	tkburn, 2, 1, do_tk_burn,
	"run tk empty burner",
	""
);
#endif

#if defined(CONFIG_SRI668)

void nt72668_sri_burn_image(void)
{
	int ret;
	u32 reg_val, tmp_val;

	reg_val = __REG(0xFC040700);

	tmp_val = (reg_val >> 14) & 1;

	setenv("usb_port","0");

	if (tmp_val == 0)
		run_command("mmc bootbus 0 2 0 2", 0);
	else
		run_command("mmc bootbus 0 2 0 1", 0);

	ret = run_command("ewrite xboot", 0);
	if(ret < 0) {
		printf("write xboot fail ...!\n");
		goto out;
	}
	printf("write xboot done ...\n");


	ret = run_command("ewrite stbc  stbc_emmc.bin", 0);

	if(ret < 0) {
		printf("write stbc emmc fail ...!\n");
		goto out;
	}
	printf("write emmc stbc done ...\n");


	ret = run_command("ewrite ddrcfg", 0);

	if(ret < 0) {
		printf("write ddrcfg fail !\n");
		goto out;
	}

	ret = run_command("ewrite uboot", 0);
	if(ret < 0) {
		printf("write U-Boot fail ...!\n");
		goto out;
	}
	printf("write U-Boot done...\n");

	ret = run_command("xsave panel config xbootdat.bin", 0);
	if(ret < 0) {
		printf("write panel config fail !\n");
		goto out;
	}

	ret = run_command("ewrite ker0", 0);
	if(ret < 0) {
		printf("write kernel fail ...!\n");
		goto out;
	}
	printf("write kernel done...\n");

	ret = run_command("ewrite ker1", 0);
	if(ret < 0) {
		printf("write recovery fail ...!\n");
		goto out;
	}
	printf("write recovery done...\n");

	ret = run_command("ewrite secos", 0);
	if(ret < 0) {
		printf("write secos fail....!\n");
		goto out;
	}
	printf("write secos done...\n");


	ret = run_command("epart write", 0);
	if(ret < 0) {
		printf("write partition fail ...!\n");
		goto out;
	}
	printf("write partition done ...\n");

	ret = run_command("ewrite fs0 ", 0);
	if(ret < 0) {
		printf("write fs0 image fail ...!\n");
		goto out;
	}

	run_command("env default -a", 0);

	run_command("emiscmd \"\"", 0);

	ret = run_command("xsave kercmd", 0);
	if(ret < 0) {
		printf("generate kernel command line fail !\n");
		goto out;
	}

	ret = run_command("xsave rcvcmd", 0);
	if(ret < 0) {
		printf("generate recovery command line fail !\n");
		goto out;
	}

	printf("Burning image process complete, please reboot this device.\n");

	//auto reboot
	nt72668_stbc_set_keypass(1);

	//switch authority
	__REG(0xFC04041C) |= ((1 << 2) | (1 << 0));

	//set STBC_GPIO0,2 set as input
	__REG(0xFC040438) |= (1 << 6);
	__REG(0xFC04042C) &= ~(1 << 6);
	__REG(0xFC040438) |= (1 << 8);
	__REG(0xFC04042C) &= ~(1 << 8);

	__REG(0xFC040210) = 0xF0;

	while(1);

out:
	return;
}


#endif

#if defined(CONFIG_N62001)
void nt72668_n62001_burn_image(void)
{
	int ret;
	u32 reg_val, tmp_val;

	reg_val = __REG(0xFC040700);

	tmp_val = (reg_val >> 14) & 1;

	setenv("usb_port", CONFIG_USB_PORT);

	if (tmp_val == 0)
		run_command("mmc bootbus 0 2 0 2", 0);
	else
		run_command("mmc bootbus 0 2 0 1", 0);

#if defined(CONFIG_CMD_NVTUPDATE)
	ret = run_command("nvtupdate", 0);
	if (ret == 0) {
	    goto out_ok;
    }
	printf("run script update fail ...!\n");
#endif //CONFIG_CMD_NVTUPDATE

	ret = run_command("ewrite xboot", 0);
	if(ret < 0) {
		printf("write xboot fail ...!\n");
		goto out;
	}
	printf("write xboot done ...\n");

	run_command("sf erase 0x0 0x100000", 0);

	ret = run_command("swrite stbc", 0);

	if(ret < 0) {
		printf("write stbc  fail ...!\n");
		goto out;
	}
	printf("write stbc done ...\n");


	ret = run_command("ewrite stbc  stbc_emmc.bin", 0);

	if(ret < 0) {
		printf("write stbc emmc fail ...!\n");
		goto out;
	}
	printf("write emmc stbc done ...\n");


	ret = run_command("ewrite ddrcfg", 0);

	if(ret < 0) {
		printf("write ddrcfg fail !\n");
		goto out;
	}

	ret = run_command("ewrite uboot", 0);
	if(ret < 0) {
		printf("write U-Boot fail ...!\n");
		goto out;
	}
	printf("write U-Boot done...\n");

	ret = run_command("xsave panel config xbootdat.bin", 0);
	if(ret < 0) {
		printf("write panel config fail !\n");
		goto out;
	}

	ret = run_command("ewrite ker0", 0);
	if(ret < 0) {
		printf("write kernel fail ...!\n");
		goto out;
	}
	printf("write kernel done...\n");

	ret = run_command("ewrite ker1", 0);
	if(ret < 0) {
		printf("write recovery fail ...!\n");
		goto out;
	}
	printf("write recovery done...\n");


	ret = run_command("epart write", 0);
	if(ret < 0) {
		printf("write partition fail ...!\n");
		goto out;
	}
	printf("write partition done ...\n");

	ret = run_command("ewrite system ", 0);
	if(ret < 0) {
		printf("write android image fail ...!\n");
		goto out;
	}
	
	run_command("env default -a", 0);

	run_command("emiscmd \"\"", 0);

	ret = run_command("xsave kercmd", 0);
	if(ret < 0) {
		printf("generate kernel command line fail !\n");
		goto out;
	}

	ret = run_command("xsave rcvcmd", 0);
	if(ret < 0) {
		printf("generate recovery command line fail !\n");
		goto out;
	}

out_ok:
	printf("Burning image process complete, please reboot this device.\n");

	//auto reboot
	nt72668_stbc_set_keypass(1);

	//switch authority
	__REG(0xFC04041C) |= ((1 << 2) | (1 << 0));

	//set STBC_GPIO0,2 set as input
	__REG(0xFC040438) |= (1 << 6);
	__REG(0xFC04042C) &= ~(1 << 6);
	__REG(0xFC040438) |= (1 << 8);
	__REG(0xFC04042C) &= ~(1 << 8);

	__REG(0xFC040210) = 0xF0;

	while(1);

out:
	return;
}
#endif /* CONFIG_N62001 */

#if defined(CONFIG_N33002)
void nt72668_n33002_burn_image(void)
{
	int ret;
	u32 reg_val, tmp_val;

	reg_val = __REG(0xFC040700);

	tmp_val = (reg_val >> 14) & 1;

	setenv("usb_port", CONFIG_USB_PORT);

	if (tmp_val == 0)
		run_command("mmc bootbus 0 2 0 2", 0);
	else
		run_command("mmc bootbus 0 2 0 1", 0);

#if defined(CONFIG_CMD_NVTUPDATE)
	ret = run_command("nvtupdate", 0);
	if (ret == 0) {
	    goto out_ok;
    }
	printf("run script update fail ...!\n");
#endif //CONFIG_CMD_NVTUPDATE

	ret = run_command("ewrite xboot", 0);
	if(ret < 0) {
		printf("write xboot fail ...!\n");
		goto out;
	}
	printf("write xboot done ...\n");

	run_command("sf erase 0x0 0x100000", 0);

	ret = run_command("swrite stbc", 0);

	if(ret < 0) {
		printf("write stbc  fail ...!\n");
		goto out;
	}
	printf("write stbc done ...\n");


	ret = run_command("ewrite stbc  stbc_emmc.bin", 0);

	if(ret < 0) {
		printf("write stbc emmc fail ...!\n");
		goto out;
	}
	printf("write emmc stbc done ...\n");


	ret = run_command("ewrite ddrcfg", 0);

	if(ret < 0) {
		printf("write ddrcfg fail !\n");
		goto out;
	}

	ret = run_command("ewrite uboot", 0);
	if(ret < 0) {
		printf("write U-Boot fail ...!\n");
		goto out;
	}
	printf("write U-Boot done...\n");

	ret = run_command("xsave panel config xbootdat.bin", 0);
	if(ret < 0) {
		printf("write panel config fail !\n");
		goto out;
	}

	ret = run_command("ewrite ker", 0);
	if(ret < 0) {
		printf("write kernel fail ...!\n");
		goto out;
	}
	printf("write kernel done...\n");

	ret = run_command("epart write", 0);
	if(ret < 0) {
		printf("write partition fail ...!\n");
		goto out;
	}
	printf("write partition done ...\n");

	ret = run_command("ewrite fs ", 0);
	if(ret < 0) {
		printf("write android image fail ...!\n");
		goto out;
	}
	
	run_command("env default -a", 0);

	run_command("emiscmd \"\"", 0);

	ret = run_command("xsave kercmd", 0);
	if(ret < 0) {
		printf("generate kernel command line fail !\n");
		goto out;
	}

	ret = run_command("xsave rcvcmd", 0);
	if(ret < 0) {
		printf("generate recovery command line fail !\n");
		goto out;
	}

out_ok:
	printf("Burning image process complete, please reboot this device.\n");

	//auto reboot
	nt72668_stbc_set_keypass(1);

	//switch authority
	__REG(0xFC04041C) |= ((1 << 2) | (1 << 0));

	//set STBC_GPIO0,2 set as input
	__REG(0xFC040438) |= (1 << 6);
	__REG(0xFC04042C) &= ~(1 << 6);
	__REG(0xFC040438) |= (1 << 8);
	__REG(0xFC04042C) &= ~(1 << 8);

	__REG(0xFC040210) = 0xF0;

	while(1);

out:
	return;
}
#endif /* CONFIG_N33002 */

enum {
	STORAGE_NONE,
	STORAGE_NAND,
	STORAGE_EMMC,
};

int do_nvt_bootstrap_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	u32 reg_val;
	int i,tmp_val;
	int storage_type = STORAGE_NONE;

	reg_val = __REG(0xFC040700);
	tmp_val = reg_val;
	reg_val >>= 3;

	for(i = 3;i <= 20;i++) {
		printf("BS%d : %01x\n",i,(reg_val & 0x1));
		reg_val >>= 1;
	}

	reg_val = tmp_val;

	//interpret BS3,4
	tmp_val = (reg_val >> 3) & 3;

	switch(tmp_val) {
		case 0:
			printf("Boot from STBC, Using STBC MaskROM\n");
			break;

		case 1: 
			printf("Boot from CPU, Using STBC MaskROM\n");
			break;

		case 2:
			printf("Boot from STBC, Bypass STBC MaskROM\n");
			break;

		case 3:
			printf("Boot from CPU, Bypass STBC MaskROM\n");
			break;
	}

	//interpret BS 5
	tmp_val = (reg_val >> 5) & 1;

	if(tmp_val == 0)
		printf("Disable STBC watchdog\n");
	else
		printf("Enable STBC watchdog\n");

	//interpret BS 6,7,8
	tmp_val = (reg_val >> 6) & 7;

	switch(tmp_val) {
		case 0:
			printf("STBC boot from SPI, CPU boot from NANS (NAND 2 Row adderess cycles)\n");
			storage_type = STORAGE_NAND;
			break;

		case 4:
			printf("STBC boot from SPI, CPU boot from NAND (NAND 3 Row address cycles)\n");
			storage_type = STORAGE_NAND;
			break;

		case 2:
			printf("STBC & CPU boot from SPI, CPU SPI use 12MHz OSC\n");
			break;

		case 6:
			printf("STBC & CPU boot from SPI, CPU SPI use 24MHz MPLL(need to bypass STBC)\n");
			break;

		case 1:
			printf("STBC & CPU boot from NAND (NAND 2 Row address cycles)\n");
			storage_type = STORAGE_NAND;
			break;

		case 5:
			printf("STBC boot from SPI, CPU boot from eMMC\n");
			storage_type = STORAGE_EMMC;
			break;

		case 3:
			printf("STBC & CPU boot from NAND (NAND 3 Row address cycles)\n");
			storage_type = STORAGE_NAND;
			break;

		case 7:
			printf("STBC & CPU boot from eMMC\n");
			storage_type = STORAGE_EMMC;
			break;
	}

	//interpret BS 9
	tmp_val = (reg_val >> 9) & 1;

	if(tmp_val == 0)
		printf("internal boot strap disable\n");
	else
		printf("internal boot strap enable\n");

	//interpret BS 10,11
	tmp_val = (reg_val >> 10) & 3;

	switch(storage_type) {
		case STORAGE_NAND:
			if((reg_val >> 20) & 1) {
				printf("NAND Page size : 512 bytes");
			} else {
				switch(tmp_val) {
					case 0:
						printf("NAND Page size : 2048 bytes");
						break;

					case 2:
						printf("NAND Page size : 4096 bytes");
						break;

					case 1:
						printf("NAND Page size : 8192 bytes");
						break;
				}
			}
			break;

		case STORAGE_EMMC:
			switch(tmp_val) {
				case 0:
					printf("EMMC Bus width is 1 bit\n");
					break;

				case 2:
					printf("EMMC Bus width is 4 bit\n");
					break;

				case 1:
					printf("EMMC Bus width is 8 bit\n");
					break;
			}
			break;

		default:
			break;	
	}

	//interpret BS 12,13,14
	
	switch(storage_type) {
		case STORAGE_NAND:
			tmp_val = (reg_val >> 12) & 7;
			switch (tmp_val) {
				case 0:
					printf("NAND Block size : 128 Kbytes\n");
					break;

				case 4:
					printf("NAND Block size : 256 Kbytes\n");
					break;

				case 2:
					printf("NAND Block size : 512 Kbytes\n");
					break;

				case 6:
					printf("NAND Block size : 1 Mbytes\n");
					break;

				case 1:
					printf("NAND Block size : 2 Mbytes\n");
					break;

			}
			break;

		case STORAGE_EMMC:
			tmp_val = (reg_val >> 13) & 1;
			if(tmp_val == 1)
				printf("EMMC_REG in 5K mode\n");

			tmp_val = (reg_val >> 14) & 1;
			if(tmp_val == 0)
				printf("EMMC Data rate in ddr mode\n");
			else
				printf("EMMC Data rate in sdr mode\n");
			break;

		default:
			break;
	}


	//interpret BS 15
	tmp_val = (reg_val >> 15) & 1;

	switch(storage_type) {
		case STORAGE_NAND:
			if(tmp_val == 0)
				printf("NAND boot flow : boot from ROM then emulator\n");
			else
				printf("NAND boot flow : boot from ROM then SRAM\n");
			break;

		case STORAGE_EMMC:
			if(tmp_val == 0)
				printf("EMMC Clock in 13 MHz\n");
			else
				printf("EMMC Clock in 26 MHz\n");
			break;

		default:
			break;
	}

	//interpret BS 16
	tmp_val = (reg_val >> 16) & 1;
	if(tmp_val == 0)
		printf("ss security boot disable \n");
	else
		printf("ss security boot enable \n");

	//interpret BS 17
	tmp_val = (reg_val >> 17) & 1;
	if(tmp_val == 0)
		printf("Scramble disable\n");
	else
		printf("Scramble enable\n");

	//interpret BS 18
	tmp_val = (reg_val >> 18) & 1;
	if(tmp_val == 0)
		printf("NAND ECC mode : RS\n");
	else
		printf("NAND ECC mode : BCH\n");

	//interpret BS 19
	tmp_val = (reg_val >> 19) & 1;
	if(tmp_val == 0)
		printf("tk security boot disable \n");
	else
		printf("tk security boot enable \n");

	//interpret BS 20
	tmp_val = (reg_val >> 20) & 1;
	if(tmp_val == 0)
		printf("NAND 512 page size support disable\n");
	else
		printf("NAND 512 page size support enable\n");

	return 0;
	
}

U_BOOT_CMD(
	bstrap,	3,	0,	do_nvt_bootstrap_cmd,
	"bstarp    - print nt72668 bootstrap info\n",
	""
);



int board_late_init(void)
{
	unsigned int bootstrap;

	nt72668_reg_preinit();

	nt72668_stbc_read_power_on_event();
	if(getenv("wdog") != NULL) {
		unsigned short wdog_sec = 0;

		wdog_sec = simple_strtoul(getenv("wdog"), NULL, 10);
		if(wdog_sec > 60)
			wdog_sec = 60;

		nt72668_stbc_enable_wdog(wdog_sec);
	}
	nvt_emmc_part_init();


	bootstrap = (*(volatile unsigned long *)(0xFC040210));
	if(bootstrap == 0x000000c0)
	{
		/* Disable watchdog when burn image */
		nt72668_stbc_disable_wdog();
#if defined(CONFIG_EVB_668)		
		nt72668_tk_burn_image();
#elif defined(CONFIG_SRI668)
		nt72668_sri_burn_image();
#elif defined(CONFIG_N62001)
        nt72668_n62001_burn_image();
#elif defined(CONFIG_N33002)
        nt72668_n33002_burn_image();
#else
#error "You should defined your burn image function !\n"		
#endif		
	}

	/* To check power on status */
	nt72668_stbc_boot_check();

	nt72668_print_sys_info();

#if defined(CONFIG_USB_UPDATE)  /* Logic add for usb update */
	if(0xaa == ((*((volatile u32 *) (0xFC04011C))) & 0xFF))
	{
		printf("USB update!---------\n");
    #if defined(CONFIG_N33002)
        if (run_command("nvtupdate", 0) != 0)
    #else
		if (run_command("cu", 0) != 0)
    #endif
		{		
            printf("reboot...\n");
		    run_command("reboot", 0);
		}
	}
#endif

	return 0;
}


