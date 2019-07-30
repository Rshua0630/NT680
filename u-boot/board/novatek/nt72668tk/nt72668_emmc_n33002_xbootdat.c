/*
 *  board/novatek/nt72668tk/nt72668_emmc_nvt_xbootdat.c
 *
 *  Author:	Alvin lin
 *  Created:	Jun 5, 2015
 *  Copyright:	Novatek Inc.
 *
 */
#include <version.h>

static int panel_xdat_help(struct _nvt_emmc_xbootdat_info *this, NVT_EMMC_XBOOTDAT_HELP_TYPE help_type);
static int panel_xdat_dump(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int panel_xdat_save(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int panel_xdat_erase(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int kercmd_xdat_help(struct _nvt_emmc_xbootdat_info *this, NVT_EMMC_XBOOTDAT_HELP_TYPE help_type);
static int kercmd_xdat_dump(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int kercmd_xdat_save(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int kercmd_xdat_erase(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int rcvcmd_xdat_help(struct _nvt_emmc_xbootdat_info *this, NVT_EMMC_XBOOTDAT_HELP_TYPE help_type);
static int rcvcmd_xdat_dump(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int rcvcmd_xdat_save(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
static int rcvcmd_xdat_erase(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);

#define CMDLINE_LEN     (1024)   

static  nvt_emmc_xbootdat_info xdat_info[] = {
	{
		//panel param
		.xdat_name = "panel",
		.xdathelp = panel_xdat_help,
		.xdatdump = panel_xdat_dump,
		.xdatsave = panel_xdat_save,
		.xdaterase = panel_xdat_erase,
		.start_sector = 0,
		.size = (PANEL_PARM_NUM + 1),
	},
	{
		//kernel command
		.xdat_name = "kercmd",
		.xdathelp = kercmd_xdat_help,
		.xdatdump = kercmd_xdat_dump,
		.xdatsave = kercmd_xdat_save,
		.xdaterase = kercmd_xdat_erase,
		.start_sector = (PANEL_PARM_NUM + 1),
		.size = 2,
	},
	{
		//recovery command
		.xdat_name = "rcvcmd",
		.xdathelp = rcvcmd_xdat_help,
		.xdatdump = rcvcmd_xdat_dump,
		.xdatsave = rcvcmd_xdat_save,
		.xdaterase = rcvcmd_xdat_erase,
		.start_sector = (PANEL_PARM_NUM + 3),
		.size = 2,
	},

};

static int panel_xdat_help(struct _nvt_emmc_xbootdat_info *this, NVT_EMMC_XBOOTDAT_HELP_TYPE help_type)
{
	int ret = 0;

	switch(help_type) {
		case XDAT_HELP_DUMP:
			printf("xdump panel - dump current panel select index and all panel settings\n");
			break;

		case XDAT_HELP_SAVE:
			printf("xsave panel index idx(0-9) - select panel config index for xboot use\n");
			printf("xsave panel config config_file_name - save panel config data into xbootdat\n");
			break;

		case XDAT_HELP_ERASE:
			printf("xerase panel - erase all panel settings and panel index, don't do this unless you know what you are doing\n");
			break;

		default:
			printf("unknown xbootdat panel help ?\n");
	}

	return ret;
}

static int panel_xdat_dump(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	panel_index_blk __attribute__((__aligned__(64))) pi_blk;
	panel_param_blk __attribute__((__aligned__(64))) pp_blk;
	int i;

	if(argc != 2) {
		printf("Usage: %s panel \n",argv[0]);
		ret = CMD_RET_USAGE;
		goto out;
	}

	//read index
	sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", &pi_blk, this->start_sector, 1);
	ret = run_command(cmd, 0);
	if(ret < 0) {
		printf("%s read panel index fail !\n", __func__);
		goto out;
	}

	if(memcmp(pi_blk.idx.sign,PANEL_INDEX_SIGN, strlen(PANEL_INDEX_SIGN)) != 0) {
		printf("panel index not valid !\n");
		goto out;
	}

	printf("panel param version : %d\n", pi_blk.idx.version);
	printf("panel param index : %d\n", pi_blk.idx.index);

	for(i=0; i < PANEL_PARM_NUM ;i++) {
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", &pp_blk, this->start_sector + 1 + i, 1);
		ret = run_command(cmd, 0);
		if(ret < 0) {
			printf("%s read panel param error !\n", __func__);
			goto out;
		}

		if(memcmp(pp_blk.p.sign, PANEL_PARAM_SIGN, strlen(PANEL_PARAM_SIGN)) != 0) 
			break;
		printf("=====panel param index : %d=====\n", i);
		printf("version: 			%d\n", pp_blk.p.version);
		printf("u8PanelID: 			%d\n", pp_blk.p.u8PanelID);
		printf("b8Mode50Hz: 			%d\n", pp_blk.p.b8Mode50Hz);
		printf("u8Panel: 			%d\n", pp_blk.p.u8Panel);
		printf("au8Name: 			%s\n", pp_blk.p.au8Name);
		printf("b8DualPixelOutput: 		%d\n", pp_blk.p.b8DualPixelOutput);
		printf("b8ABSwap: 			%d\n", pp_blk.p.b8ABSwap);
		printf("b8ChannelSwap: 			%d\n", pp_blk.p.b8ChannelSwap);
		printf("b8FrameSyncEnable: 		%d\n", pp_blk.p.b8FrameSyncEnable);
		printf("u8FrameSyncMode: 		%d\n", pp_blk.p.u8FrameSyncMode);
		printf("u8MISC: 			%d\n", pp_blk.p.u8MISC);
		printf("enLvdsFormat: 			%d\n", pp_blk.p.enLvdsFormat);
		printf("b8RBSwap: 			%d\n", pp_blk.p.b8RBSwap);
		printf("u8DitheringBit: 		%d\n", pp_blk.p.u8DitheringBit);
		printf("u16LVDSPhase: 			%d\n", pp_blk.p.u16LVDSPhase);
		printf("u16LVDSPhase_PortA: 		%d\n", pp_blk.p.u16LVDSPhase_PortA);
		printf("u16LVDSPhase_PortB: 		%d\n", pp_blk.p.u16LVDSPhase_PortB);
		printf("u16VFreqDiff_Min: 		%d\n", pp_blk.p.u16VFreqDiff_Min);
		printf("u16VFreqDiff_Max: 		%d\n", pp_blk.p.u16VFreqDiff_Max);
		printf("u16VTotalDiff_Min: 		%d\n", pp_blk.p.u16VTotalDiff_Min);
		printf("u16VTotalDiff_Max: 		%d\n", pp_blk.p.u16VTotalDiff_Max);
		printf("stMode50Hz.u16Width: 		%d\n", pp_blk.p.stMode50Hz.u16Width);
		printf("stMode50Hz.u16Height: 		%d\n", pp_blk.p.stMode50Hz.u16Height);
		printf("stMode50Hz.u16TypHTotal: 	%d\n", pp_blk.p.stMode50Hz.u16TypHTotal);
		printf("stMode50Hz.u8HSyncWidth: 	%d\n", pp_blk.p.stMode50Hz.u8HSyncWidth);
		printf("stMode50Hz.u16HSyncBackPorch: 	%d\n", pp_blk.p.stMode50Hz.u16HSyncBackPorch);
		printf("stMode50Hz.u16HSyncStart: 	%d\n", pp_blk.p.stMode50Hz.u16HSyncStart);
		printf("stMode50Hz.u16TypVTotal: 	%d\n", pp_blk.p.stMode50Hz.u16TypVTotal);
		printf("stMode50Hz.u8VSyncWidth: 	%d\n", pp_blk.p.stMode50Hz.u8VSyncWidth);
		printf("stMode50Hz.u16VSyncBackPorch: 	%d\n", pp_blk.p.stMode50Hz.u16VSyncBackPorch);
		printf("stMode50Hz.u16VSyncStart: 	%d\n", pp_blk.p.stMode50Hz.u16VSyncStart);
		printf("stMode50Hz.u16PLL: 		%d\n", pp_blk.p.stMode50Hz.u16PLL);
		printf("stMode60Hz.u16Width: 		%d\n", pp_blk.p.stMode60Hz.u16Width);
		printf("stMode60Hz.u16Height: 		%d\n", pp_blk.p.stMode60Hz.u16Height);
		printf("stMode60Hz.u16TypHTotal: 	%d\n", pp_blk.p.stMode60Hz.u16TypHTotal);
		printf("stMode60Hz.u8HSyncWidth: 	%d\n", pp_blk.p.stMode60Hz.u8HSyncWidth);
		printf("stMode60Hz.u16HSyncBackPorch: 	%d\n", pp_blk.p.stMode60Hz.u16HSyncBackPorch);
		printf("stMode60Hz.u16HSyncStart: 	%d\n", pp_blk.p.stMode60Hz.u16HSyncStart);
		printf("stMode60Hz.u16TypVTotal: 	%d\n", pp_blk.p.stMode60Hz.u16TypVTotal);
		printf("stMode60Hz.u8VSyncWidth: 	%d\n", pp_blk.p.stMode60Hz.u8VSyncWidth);
		printf("stMode60Hz.u16VSyncBackPorch: 	%d\n", pp_blk.p.stMode60Hz.u16VSyncBackPorch);
		printf("stMode60Hz.u16VSyncStart: 	%d\n", pp_blk.p.stMode60Hz.u16VSyncStart);
		printf("stMode60Hz.u16PLL: 		%d\n", pp_blk.p.stMode60Hz.u16PLL);
		printf("u16PanelPowerOnDelay1: 		%d\n", pp_blk.p.u16PanelPowerOnDelay1);
		printf("u16PanelPowerOnDelay2: 		%d\n", pp_blk.p.u16PanelPowerOnDelay2);
		printf("u16PanelPowerOFFDelay1: 	%d\n", pp_blk.p.u16PanelPowerOFFDelay1);
		printf("u16PanelPowerOFFDelay2: 	%d\n", pp_blk.p.u16PanelPowerOFFDelay2);
		printf("u16BackLightLevel: 		%d\n", pp_blk.p.u16BackLightLevel);
		printf("b8PWMDutyInv: 			%d\n", pp_blk.p.b8PWMDutyInv);
		printf("b8HFlip: 			%d\n", pp_blk.p.b8HFlip);
		printf("b8VFlip: 			%d\n", pp_blk.p.b8VFlip);
		printf("b8OutputPanelFHD: 		%d\n", pp_blk.p.b8OutputPanelFHD);
		printf("VX1: 				%d\n", pp_blk.p.VX1);
		printf("FRC: 				%d\n", pp_blk.p.FRC);
		printf("SEP: 				%d\n", pp_blk.p.SEP);
		printf("b8Blinking: 			%d\n", pp_blk.p.b8Blinking);
		printf("u8PanelType: 			%d\n", pp_blk.p.u8PanelType);
		printf("b8ForcePanelTiming2D: 		%d\n", pp_blk.p.b8ForcePanelTiming2D);
		printf("b8ForcePanelTiming2DSport: 	%d\n", pp_blk.p.b8ForcePanelTiming2DSport);
		printf("b8ForcePanelTiming3D: 		%d\n", pp_blk.p.b8ForcePanelTiming3D);
		printf("u16PanelTiming2D: 		%d\n", pp_blk.p.u16PanelTiming2D);
		printf("u16PanelTiming2DSport: 		%d\n", pp_blk.p.u16PanelTiming2DSport);
		printf("u16PanelTiming3D: 		%d\n", pp_blk.p.u16PanelTiming3D);
		printf("u16PWM2DSync: 			%d\n", pp_blk.p.u16PWM2DSync);
		printf("u16PWM3DSync: 			%d\n", pp_blk.p.u16PWM3DSync);
		printf("u16PDIMFreq2D50Hz: 		%d\n", pp_blk.p.u16PDIMFreq2D50Hz);
		printf("u16PDIMFreq2D60Hz: 		%d\n", pp_blk.p.u16PDIMFreq2D60Hz);
		printf("u16PDIMFreq2DSport50Hz: 	%d\n", pp_blk.p.u16PDIMFreq2DSport50Hz);
		printf("u16PDIMFreq2DSport60Hz: 	%d\n", pp_blk.p.u16PDIMFreq2DSport60Hz);
		printf("u16PDIMFreq3D50Hz: 		%d\n", pp_blk.p.u16PDIMFreq3D50Hz);
		printf("u16PDIMFreq3D60Hz: 		%d\n", pp_blk.p.u16PDIMFreq3D60Hz);
		printf("u16PDIMDelay2D50Hz: 		%d\n", pp_blk.p.u16PDIMDelay2D50Hz);
		printf("u16PDIMDelay2D60Hz: 		%d\n", pp_blk.p.u16PDIMDelay2D60Hz);
		printf("u16PDIMDelay2DSport50Hz: 	%d\n", pp_blk.p.u16PDIMDelay2DSport50Hz);
		printf("u16PDIMDelay2DSport60Hz: 	%d\n", pp_blk.p.u16PDIMDelay2DSport60Hz);
		printf("u16PDIMDelay3D50Hz: 		%d\n", pp_blk.p.u16PDIMDelay3D50Hz);
		printf("u16PDIMDelay3D60Hz: 		%d\n", pp_blk.p.u16PDIMDelay3D60Hz);
		printf("u16ADIMFreq2D50Hz: 		%d\n", pp_blk.p.u16ADIMFreq2D50Hz);
		printf("u16ADIMFreq2D60Hz: 		%d\n", pp_blk.p.u16ADIMFreq2D60Hz);
		printf("u16ADIMFreq2DSport50Hz: 	%d\n", pp_blk.p.u16ADIMFreq2DSport50Hz);
		printf("u16ADIMFreq2DSport60Hz: 	%d\n", pp_blk.p.u16ADIMFreq2DSport60Hz);
		printf("u16ADIMFreq3D50Hz: 		%d\n", pp_blk.p.u16ADIMFreq3D50Hz);
		printf("u16ADIMFreq3D60Hz: 		%d\n", pp_blk.p.u16ADIMFreq3D60Hz);
		printf("b8LocalDIMControl: 		%d\n", pp_blk.p.b8LocalDIMControl);
		printf("u16LRDelay3D: 			%d\n", pp_blk.p.u16LRDelay3D);
		printf("u16BTDelay3D: 			%d\n", pp_blk.p.u16BTDelay3D);
		printf("b8PDIMConByPanel3D: 		%d\n", pp_blk.p.b8PDIMConByPanel3D);
		printf("u8Tcon: 			%d\n", pp_blk.p.u8Tcon);
		printf("u8TconType: 			%d\n", pp_blk.p.u8TconType);
		printf("u16VX1PreEmphasisG1: 		%d\n", pp_blk.p.u16VX1PreEmphasisG1);
		printf("u16VX1PreEmphasisG2: 		%d\n", pp_blk.p.u16VX1PreEmphasisG2);
		printf("u16VX1PreEmphasisG3: 		%d\n", pp_blk.p.u16VX1PreEmphasisG3);
		printf("u16VX1SwingG1: 			%d\n", pp_blk.p.u16VX1SwingG1);
		printf("u16VX1SwingG2: 			%d\n", pp_blk.p.u16VX1SwingG2);
		printf("u16VX1SwingG3: 			%d\n", pp_blk.p.u16VX1SwingG3);
		printf("================================\n\n");
	}

out:
	return ret;
}

static int panel_xdat_save(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	panel_index_blk pi_blk;
	u32 idx = 0;

	if(argc != 4 || (strcmp(argv[2], "index") != 0 && strcmp(argv[2],"config") != 0)) {
		ret = CMD_RET_USAGE;
		goto out;
	}

	if (strcmp(argv[2], "index") == 0) {
		idx = simple_strtoul(argv[3], NULL, 10);
		if(idx >= PANEL_PARM_NUM) {
			printf("panel index must between 0~9\n");
			ret = -EINVAL;
			goto out;
		}

		memset(&pi_blk, 0, sizeof(pi_blk));

		memcpy(pi_blk.idx.sign, PANEL_INDEX_SIGN, strlen(PANEL_INDEX_SIGN));
		pi_blk.idx.version = PANEL_PARAM_VER;
		pi_blk.idx.index = idx;

		sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", &pi_blk, this->start_sector, 1);
		ret = run_command(cmd, 0);

		if(ret < 0) {
			printf("save panel index fail !\n");
			goto out;
		}
	} else if(strcmp(argv[2], "config") == 0) {
		run_command("usb start", 0);
		sprintf(cmd, "fatload usb 0 0x%x %s", CONFIG_SYS_FWUPDATE_BUF, argv[3]);
		ret = run_command(cmd, 0);

		if(ret != 0) {
			printf("load panel config %s fail !\n", argv[3]);
			goto out;
		}

		memset(cmd, 0, sizeof(cmd));

		sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF, this->start_sector, (PANEL_PARM_NUM + 1));
		ret = run_command(cmd, 0);

		if(ret < 0) {
			printf("write panel config to xbootdat fail !\n");
			goto out;
		}
	}

out:
	return ret;
}

static int panel_xdat_erase(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	char cmd[64] = {0};
	int ret = 0;

	memset(CONFIG_SYS_FWUPDATE_BUF, 0 ,(PANEL_PARM_NUM + 1) * PANEL_PARAM_SZ);
	sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF, this->start_sector, (PANEL_PARM_NUM));
	ret = run_command(cmd, 0);

	if(ret < 0) 
		printf("erase panel config fail !\n");

	return ret;
}


static int kercmd_xdat_help(struct _nvt_emmc_xbootdat_info *this, NVT_EMMC_XBOOTDAT_HELP_TYPE help_type)
{
	int ret = 0;

	switch(help_type) {
		case XDAT_HELP_DUMP:
			printf("xdump kercmd - dump current kernel command line\n");
			break;

		case XDAT_HELP_SAVE:
			printf("xsave kercmd - generate kernel command line by uboot variables and save to xbootdat\n");
			break;

		case XDAT_HELP_ERASE:
			printf("xerase kercmd - erase kernel command line save in xbootdat, don't do this unless you know what you are doing\n");
			break;

		default:
			printf("unknown xbootdat kercmd help ?\n");
	}

	return ret;

}


static int kercmd_xdat_dump(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	char __attribute__((__aligned__(64))) buf[CMDLINE_LEN] = {0};
	int i;

	if(argc != 2) {
		printf("Usage: %s kercmd \n",argv[0]);
		ret = CMD_RET_USAGE;
		goto out;
	}

	sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", buf, this->start_sector, this->size);
	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("read kernel command line fail !\n");
		goto out;
	}

	printf("kercmd : %s\n", buf);
out:
	return ret;
}


static int kercmd_xdat_save(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	char cmd[CMDLINE_LEN] = {0};
	char buf[255] = {0};
	int ret = 0,len;
	unsigned char *ker_load_addr = 0;
	unsigned char *ramdisk_load_addr = 0;
	struct bootloader_message *misc_info;
	nvt_emmc_image_info *pimg;
	int block_cnt = 0;
	struct boot_address addr;
	int cpus;

	if(NULL != getenv("console"))
		sprintf(cmd, "console=%s,115200 ", getenv("console"));
	else
		sprintf(cmd, "%s ", "console=ttyS0,115200");

	if (getenv("memstr") != NULL)
		sprintf(buf,"%s ",getenv("memstr"));
	else
		sprintf(buf,"%s ","mem=512M");

	strcat(cmd,buf);

	//for mali and ump
	if(getenv("mali_core") != NULL) {
		sprintf(buf,"mali.mali_active_cores=%s ",getenv("mali_core"));
		strcat(cmd,buf);
	}

	if (getenv("mali_start") != NULL && getenv("mali_size") != NULL) {
		sprintf(buf,"mali.mali_dedicated_mem_start=%s mali.mali_dedicated_mem_size=%s ",getenv("mali_start"),getenv("mali_size"));
		strcat(cmd,buf);
	}

	if(getenv("mali_share") != NULL) {
		sprintf(buf,"mali.mali_shared_mem_size=%s ",getenv("mali_share"));
		strcat(cmd, buf);
	}

	if (getenv("ump_start") != NULL && getenv("ump_size") != NULL) {
		sprintf(buf,"ump.ump_memory_address=%s ump.ump_memory_size=%s ",getenv("ump_start"),getenv("ump_size"));
		strcat(cmd,buf);
	}

	if(getenv("ump_backend") != NULL) {
		sprintf(buf,"ump.ump_backend=%s ",getenv("ump_backend"));
		strcat(cmd,buf);
	}

	if (getenv("fb_start") != NULL && getenv("fb_size") != NULL) {
		sprintf(buf,"mali.mali_fb_start=%s mali.mali_fb_size=%s ",getenv("fb_start"),getenv("fb_size"));
		strcat(cmd,buf);
	}

	if(getenv("lpj") != NULL) {
		sprintf(buf, "lpj=%s ",getenv("lpj"));
		strcat(cmd, buf);
	}

	if(getenv("dtv_log_addr") != NULL) {
		sprintf(buf, "dtv_log_addr=%s ",getenv("dtv_log_addr"));
		strcat(cmd,buf);
	}


	if (getenv("ethaddr") != NULL ) {
		sprintf(buf,"ethaddr=%s ",getenv("ethaddr"));
		strcat(cmd,buf);
	}
	
	sprintf(buf,"uboot_ver=%s ", U_BOOT_VERSION);
	strcat(cmd,buf);

	if(getenv("usb") != NULL){
		sprintf(buf,"root=%s rw ",getenv("usb"));
		setenv("ramdisk_loadaddr", NULL);
	}else {
    	ret = nvt_emmc_get_partition_id_by_name("fs0");
    	if (ret != INVAILD_USER_PART_ID) {
            sprintf(buf,"root=/dev/mmcblk0p%d ro ", ret);
            setenv("ramdisk_loadaddr", NULL);
        }
        else {
            sprintf(buf,"%s ","root=/dev/ram0 rw ");
        }
	}

	strcat(buf,"rootfstype=ext4 ");
	strcat(cmd,buf);
	
	ret = nvt_emmc_get_partition_id_by_name("ap0");
	if (ret != INVAILD_USER_PART_ID) {
        sprintf(buf,"apexe=/dev/mmcblk0p%d ", ret);
        strcat(cmd,buf);
	}

	if(getenv_yesno("quiet"))
		strcat(cmd,"quiet ");


	if(getenv("cpus") != NULL) {
		cpus = simple_strtoul(getenv("cpus"),NULL,10);
		if(cpus > 4 || cpus < 1)
			cpus = 4;
	} else {
		cpus = 4;
	}
	sprintf(buf, "maxcpus=%d ",cpus);
	strcat(cmd,buf);

	if(getenv("dump_restore") != NULL)
	{
		strcat(cmd,"rdinit=/linuxrc rootwait ");
	}
	else
	{
		strcat(cmd,"init=/init rootwait ");
	}

	if(getenv("kparam_addon") != NULL) {
		sprintf(buf," %s ", getenv("kparam_addon"));
		strcat(cmd, buf);
	}

	if(getenv("VX1") != NULL && strcmp(getenv("VX1"),"1") == 0) {
		sprintf(buf, "androidboot.VX1=1 " );
		strcat(cmd, buf);
	}

	if(getenv("FRC") != NULL && strcmp(getenv("FRC"),"1") == 0) {
		sprintf(buf, "androidboot.FRC=1 " );
		strcat(cmd, buf);
	}

	if(getenv("SEP") != NULL && strcmp(getenv("SEP"),"1") == 0) {
		sprintf(buf, "androidboot.SEP=1 " );
		strcat(cmd, buf);
	}

	if(getenv("PANEL") != NULL) {
		sprintf(buf, "androidboot.PANEL=%s ", getenv("PANEL"));
		strcat(cmd, buf);
	}


	setenv("bootargs",cmd);

#if defined(CONFIG_NT72668_TK_TZONE)
	pimg = nvt_emmc_get_img_by_name("secos");
	if(pimg == NULL) {	
		printf("%s get secos image fail !\n",__func__);
		goto out;
	}

	ret = pimg->read_img(pimg, SECOS_BUFFER_BLOCK, &len, NULL);
#endif	


	ker_load_addr = simple_strtoul(getenv("kernel_loadaddr"),NULL,16);

	if(getenv("p3") != NULL) {
		run_command("usb start", 0);
		setenv("ramdisk_loadaddr", NULL);
		sprintf(cmd,"fatload usb 0 0x%x p3.img",ker_load_addr);
		ret = run_command(cmd, 0);
		goto out;
	}

#if 0
	misc_info = ker_load_addr;
	block_cnt = (sizeof(struct bootloader_message)/SECTOR_IN_BYTES) + 1;
	memset(misc_info, 0 , block_cnt);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc image fail !\n",__func__);
		goto out;
	}

	ret = pimg->read_img(pimg, misc_info, &len, &block_cnt);
	if(ret < 0) {
		printf("load MISC from emmc fail !\n");
		goto out;
	}
	//printf("command=%s\n",misc_info->command);
#endif

	if(getenv("ramdisk_loadaddr") != NULL)
		ramdisk_load_addr = simple_strtoul(getenv("ramdisk_loadaddr"),NULL,16);
	else
		ramdisk_load_addr = 0;

	addr.kernel_addr = ker_load_addr;
	addr.ramdisk_addr = ramdisk_load_addr;

	if(!getenv_yesno("selinux"))
		strcat(cmd," androidboot.selinux=permissive ");

#if 0
	if(strncmp(misc_info->command, "boot-recovery", 13) == 0)
	{
		pimg = nvt_emmc_get_img_by_name("ker1");

		if(pimg == NULL) {
			printf("%s get recovery kernel image fail !\n");
			goto out;
		}
		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 1 from emmc fail !\n");
			goto out;
		}


	}
	else
#endif
	{
		pimg = nvt_emmc_get_img_by_name("ker0");

		if(pimg == NULL) {
			printf("%s get normal kernel image fail !\n");
			goto out;
		}


		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 0 from emmc fail !\n");
			goto out;
		}

	}

	if(len > 0 && addr.ramdisk_addr != 0) {
		//const image_header_t *hdr = (const image_header_t *)addr.ramdisk_addr;
		//sprintf(buf,"initrd=0x%x,0x%x", addr.ramdisk_addr + sizeof(image_header_t), image_get_data_size(hdr));
		strcat(cmd, "initrd=");
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "mmc write 0x%x 0x%x 0x%x", cmd, this->start_sector, this->size);
	ret = run_command(buf, 0);

	if(ret < 0) {
		printf("save kernel command to xbootdat fail !\n");
		goto out;
	}


out:

	return ret;

}


static int kercmd_xdat_erase(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	char buf[CMDLINE_LEN] = {0};
	int i;

	if(argc != 2) {
		printf("Usage: %s kercmd \n",argv[0]);
		ret = CMD_RET_USAGE;
		goto out;
	}

	sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", buf, this->start_sector, this->size);
	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("erase kernel command line fail !\n");
		goto out;
	}

out:
	return ret;

}

static int rcvcmd_xdat_help(struct _nvt_emmc_xbootdat_info *this, NVT_EMMC_XBOOTDAT_HELP_TYPE help_type)
{
	int ret = 0;

	switch(help_type) {
		case XDAT_HELP_DUMP:
			printf("xdump rcvcmd - dump current recovery command line\n");
			break;

		case XDAT_HELP_SAVE:
			printf("xsave rcvcmd - generate recovery command line by uboot variables and save to xbootdat\n");
			break;

		case XDAT_HELP_ERASE:
			printf("xerase rcvcmd - erase recovery command line save in xbootdat, don't do this unless you know what you are doing\n");
			break;

		default:
			printf("unknown xbootdat rcvcmd help ?\n");
	}

	return ret;

}


static int rcvcmd_xdat_dump(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	char __attribute__((__aligned__(64))) buf[CMDLINE_LEN] = {0};
	int i;

	if(argc != 2) {
		printf("Usage: %s rcvcmd \n",argv[0]);
		ret = CMD_RET_USAGE;
		goto out;
	}

	sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", buf, this->start_sector, this->size);
	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("read recovery command line fail !\n");
		goto out;
	}

	printf("rcvcmd : %s\n", buf);
out:
	return ret;
}


static int rcvcmd_xdat_save(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	char cmd[CMDLINE_LEN] = {0};
	char buf[255] = {0};
	int ret = 0,len;
	unsigned char *ker_load_addr = 0;
	unsigned char *ramdisk_load_addr = 0;
	struct bootloader_message *misc_info;
	nvt_emmc_image_info *pimg;
	int block_cnt = 0;
	struct boot_address addr;
	int cpus;

	if(NULL != getenv("console"))
		sprintf(cmd, "console=%s,115200 ", getenv("console"));
	else
		sprintf(cmd, "%s ", "console=ttyS0,115200");

	if (getenv("memstr") != NULL)
		sprintf(buf,"%s ",getenv("memstr"));
	else
		sprintf(buf,"%s ","mem=512M");

	strcat(cmd,buf);

	if(getenv("lpj") != NULL) {
		sprintf(buf, "lpj=%s ",getenv("lpj"));
		strcat(cmd, buf);
	}

	if(getenv("dtv_log_addr") != NULL) {
		sprintf(buf, "dtv_log_addr=%s ",getenv("dtv_log_addr"));
		strcat(cmd,buf);
	}


	if (getenv("ethaddr") != NULL ) {
		sprintf(buf,"ethaddr=%s ",getenv("ethaddr"));
		strcat(cmd,buf);
	}

	if(getenv("usb") != NULL){
		sprintf(buf,"root=%s rw ",getenv("usb"));
		setenv("ramdisk_loadaddr", NULL);
	}else {
		sprintf(buf,"%s ","root=/dev/ram0 rw ");
	}


	strcat(buf,"rootfstype=ext4 ");
	strcat(cmd,buf);

	if(getenv_yesno("quiet"))
		strcat(cmd,"quiet ");


	if(getenv("cpus") != NULL) {
		cpus = simple_strtoul(getenv("cpus"),NULL,10);
		if(cpus > 4 || cpus < 1)
			cpus = 4;
	} else {
		cpus = 4;
	}
	sprintf(buf, "maxcpus=%d ",cpus);
	strcat(cmd,buf);

	if(getenv("dump_restore") != NULL)
	{
		strcat(cmd,"rdinit=/linuxrc rootwait ");
	}
	else
	{
		strcat(cmd,"init=/init rootwait ");
	}

	if(getenv("kparam_addon") != NULL) {
		sprintf(buf," %s ", getenv("kparam_addon"));
		strcat(cmd, buf);
	}

	if(getenv("serialno") != NULL) {
		sprintf(buf, "androidboot.serialno=%s ", getenv("serialno"));
		strcat(cmd, buf);
	}

	if(getenv("VX1") != NULL && strcmp(getenv("VX1"),"1") == 0) {
		sprintf(buf, "androidboot.VX1=1 " );
		strcat(cmd, buf);
	}

	if(getenv("FRC") != NULL && strcmp(getenv("FRC"),"1") == 0) {
		sprintf(buf, "androidboot.FRC=1 " );
		strcat(cmd, buf);
	}

	if(getenv("SEP") != NULL && strcmp(getenv("SEP"),"1") == 0) {
		sprintf(buf, "androidboot.SEP=1 " );
		strcat(cmd, buf);
	}

	setenv("bootargs",cmd);


	ker_load_addr = simple_strtoul(getenv("kernel_loadaddr"),NULL,16);

	misc_info = ker_load_addr;
	block_cnt = (sizeof(struct bootloader_message)/SECTOR_IN_BYTES) + 1;
	memset(misc_info, 0 , block_cnt);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc image fail !\n",__func__);
		goto out;
	}

	ret = pimg->read_img(pimg, misc_info, &len, &block_cnt);
	if(ret < 0) {
		printf("load MISC from emmc fail !\n");
		goto out;
	}
	//printf("command=%s\n",misc_info->command);

	if(getenv("ramdisk_loadaddr") != NULL)
		ramdisk_load_addr = simple_strtoul(getenv("ramdisk_loadaddr"),NULL,16);
	else
		ramdisk_load_addr = 0;

	addr.kernel_addr = ker_load_addr;
	addr.ramdisk_addr = ramdisk_load_addr;

	if(!getenv_yesno("selinux"))
		strcat(cmd," androidboot.selinux=permissive ");

	if(len > 0 && addr.ramdisk_addr != 0) {
		//const image_header_t *hdr = (const image_header_t *)addr.ramdisk_addr;
		//sprintf(buf,"initrd=0x%x,0x%x", addr.ramdisk_addr + sizeof(image_header_t), image_get_data_size(hdr));
		strcat(cmd, "initrd=");
	}

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "mmc write 0x%x 0x%x 0x%x", cmd, this->start_sector, this->size);
	ret = run_command(buf, 0);

	if(ret < 0) {
		printf("save recovery command to xbootdat fail !\n");
		goto out;
	}


out:

	return ret;

}


static int rcvcmd_xdat_erase(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	char buf[CMDLINE_LEN] = {0};
	int i;

	if(argc != 2) {
		printf("Usage: %s rcvcmd \n",argv[0]);
		ret = CMD_RET_USAGE;
		goto out;
	}

	sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", buf, this->start_sector, this->size);
	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("erase recovery command line fail !\n");
		goto out;
	}

out:
	return ret;

}
