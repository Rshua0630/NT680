/*
 *  board/novatek/evb668/nt72668_utils.c
 *
 *  Author:	Alvin lin
 *  Created:	June 21, 2013
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <asm/arch/nt72668_stbc.h>
#include "nt72668_emmc_fwupdate.h"
#include "../../../drivers/mmc/nt72668_mmc.h"
#include "nt72668_utils.h"
//#include "nt72668_fb.h"
#include "nt72668_emmc_partition.h"
//#include "nvt_usb.h"

#if defined(CONFIG_TCL_CN_668) || defined(CONFIG_TCL_AU_668)

#define OSD_BUFF_ADDR 		(0x33F80000)  //According to memory map setting in tv code file hal_mem_668_setting.h
#define kernel_loadaddr     (0x02000000)
int nt72668_fwupdate_image(void)
{
	int ret = 0;
    int i = 0;
    int k =0;
	unsigned long u32TimgFlag;
	char fwupdate_buf[55];
	unsigned long const_timg_flag = 1196247380; //TIMG:474d4954
    char *str[]={"CN","EU","AU"};

    typedef struct _pimg_hdr{
        unsigned char sign[4];
        unsigned long flen;
        unsigned char md5sum[16];
        unsigned char img_name[16];
        unsigned char dev_path[20];
        unsigned long dev_ofs;
        unsigned long startpos;
    }nvtfw_pimg_hdr;

	nt72668_stbc_disable_wdog();
    //sleep 800ms after usb power on
    mdelay(2000);

	setenv("usb_port","0");

	ret = run_command("usb start",0);
	if(ret < 0)
	{
		printf("can't start usb 0 ! \n");
		return -1;
	}
    for (i = 0; i < 3; i++)
    {

	   sprintf(fwupdate_buf,"fatload usb 0 0x%x V8-NT667%s.bin 100 0", CONFIG_SYS_FWUPDATE_BUF, str[i]);
	   ret = run_command(fwupdate_buf,0);
	   u32TimgFlag = (*(volatile unsigned long*)CONFIG_SYS_FWUPDATE_BUF);

       if (u32TimgFlag == const_timg_flag)
       {
           break;
       }
       else
       {
           continue;
       }
    }

	if(u32TimgFlag != const_timg_flag)
	{

	    setenv("usb_port","1");

	    ret = run_command("usb start",0);
	    if(ret < 0)
	    {
		   printf("can't start usb port 1 ! \n");
		   return -1;
	    }

        for (i = 0; i < 3; i++)
        {
	        sprintf(fwupdate_buf,"fatload usb 0 0x%x V8-NT667%s.bin 100 0", CONFIG_SYS_FWUPDATE_BUF, str[i]);
	        ret = run_command(fwupdate_buf,0);
	        u32TimgFlag = (*(volatile unsigned long*)CONFIG_SYS_FWUPDATE_BUF);

            if (u32TimgFlag == const_timg_flag)
            {
               break;
            }
            else
            {
               continue;
            }
        }
	}

    if(u32TimgFlag != const_timg_flag)
    {
        printf("can't get update package from usb, please check it!\n");
        //led on,if can't load update package
		__REG(0xFC04041c) |= 0x1;
		__REG(0xFC040438) |= 0x40;

		__REG(0xFC04042C) |= 0x40;

		__REG(0xFC040420) &= 0xBF;
        return -1;
    }

    nvtfw_pimg_hdr stPerimgInfo[3];

    for (k = 0; k < 3; k++)
    {
        memset(&stPerimgInfo[k], 0, sizeof(nvtfw_pimg_hdr));
    }

    for (k = 0; k < 3; k++)
    {   //break whole img header len = 24
        memcpy(&stPerimgInfo[k],(char *)(0x3200018+k*sizeof(nvtfw_pimg_hdr)),sizeof(nvtfw_pimg_hdr));
    }

    // show brun img bmp
	sprintf(fwupdate_buf,"fatload usb 0 0x%x V8-NT667%s.bin %x %x",OSD_BUFF_ADDR, str[i], stPerimgInfo[0].flen, stPerimgInfo[0].startpos<<12);
	run_command(fwupdate_buf,0);
	nt72668_show_logo_fwupdate();

    ret = strcmp((const char*)stPerimgInfo[1].img_name, "kerdisk");
    if (ret == 0)
	{
        //update ptable not exit
	    sprintf(fwupdate_buf,"fatload usb 0 0x%x V8-NT667%s.bin %x %x", kernel_loadaddr, str[i], stPerimgInfo[1].flen, stPerimgInfo[1].startpos<<12);
	    run_command(fwupdate_buf,0);
		setenv("ramdisk_loadaddr", NULL);
        sprintf(fwupdate_buf,"bootm 0x%x", kernel_loadaddr);
        run_command(fwupdate_buf,0);
	}
	else
	{
        //update ptable if exit
	    sprintf(fwupdate_buf,"fatload usb 0 0x4000000 V8-NT667%s.bin %x %x", str[i], stPerimgInfo[1].flen, stPerimgInfo[1].startpos<<12);
	    run_command(fwupdate_buf,0);
		nvt_ewritepart(0x4000000,stPerimgInfo[1].flen);
        sprintf(fwupdate_buf,"epart write");
        run_command(fwupdate_buf,0);

	    sprintf(fwupdate_buf,"fatload usb 0 0x%x V8-NT667%s.bin %x %x", kernel_loadaddr, str[i], stPerimgInfo[2].flen, stPerimgInfo[2].startpos<<12);
	    run_command(fwupdate_buf,0);
		setenv("ramdisk_loadaddr", NULL);
	    sprintf(fwupdate_buf,"bootm 0x%x", kernel_loadaddr);
		run_command(fwupdate_buf,0);
	}
	return 0;
}
#endif

int do_nvt_boot_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char cmd[512] = {0};
	char buf[255] = {0};
	int ret = 0,len;
	unsigned char *ker_load_addr = 0;
	unsigned char *ramdisk_load_addr = 0;
	struct bootloader_message *misc_info;
	nvt_emmc_image_info *pimg;
	int block_cnt = 0;
	struct boot_address addr;

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

#if defined(CONFIG_FLASH_P3)
	if(getenv("p3") != NULL) {
		sprintf(buf,"%s ","root=/dev/mmcblk0p7 rw ");
		setenv("ramdisk_loadaddr", NULL);
	}else
#endif
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

#if defined(NT72668_REV_B)
	strcat(cmd, "vmalloc=560M ");

	{
		int cpus;

		if(getenv("cpus") != NULL) {
			cpus = simple_strtoul(getenv("cpus"),NULL,10);
			if(cpus > 4 || cpus < 1)
				cpus = 4;
		} else {
			cpus = 4;
		}
		sprintf(buf, "maxcpus=%d ",cpus);
		strcat(cmd,buf);
	}
#endif

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

	setenv("bootargs",cmd);

#if defined(CONFIG_NT72668_TK_TZONE)
	pimg = nvt_emmc_get_img_by_name("secos");
	if(pimg == NULL) {	
		printf("%s get secos image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, SECOS_BUFFER_BLOCK, &len, NULL);
#endif	


	ker_load_addr = simple_strtoul(getenv("kernel_loadaddr"),NULL,16);

#if ! defined(CONFIG_FLASH_P3)
	if(getenv("p3") != NULL) {
		setenv("ramdisk_loadaddr", NULL);
		sprintf(cmd,"nvtload 0x%x p3.img",ker_load_addr);
		ret = run_command(cmd, 0);
		goto out;
	}
#endif

	misc_info = ker_load_addr;
	block_cnt = (sizeof(struct bootloader_message)/SECTOR_IN_BYTES) + 1;
	memset(misc_info, 0 , block_cnt);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, misc_info, &len, &block_cnt);
	if(ret < 0) {
		printf("load MISC from emmc fail !\n");
		goto loop;
	}
	printf("command=%s\n",misc_info->command);

	if(getenv("ramdisk_loadaddr") != NULL)
		ramdisk_load_addr = simple_strtoul(getenv("ramdisk_loadaddr"),NULL,16);
	else
		ramdisk_load_addr = 0;

	addr.kernel_addr = ker_load_addr;
	addr.ramdisk_addr = ramdisk_load_addr;

	if(strncmp(misc_info->command, "boot-recovery", 13) == 0)
	{
		pimg = nvt_emmc_get_img_by_name("ker1");

		if(pimg == NULL) {
			printf("%s get recovery kernel image fail !\n");
			goto loop;
		}
		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 1 from emmc fail !\n");
			goto loop;
		}


	} else {
		pimg = nvt_emmc_get_img_by_name("ker0");

		if(pimg == NULL) {
			printf("%s get normal kernel image fail !\n");
			goto loop;
		}


		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 0 from emmc fail !\n");
			goto loop;
		}

	}

	if(len > 0 && addr.ramdisk_addr != 0) {
		const image_header_t *hdr = (const image_header_t *)addr.ramdisk_addr;
		sprintf(buf,"initrd=0x%x,0x%x", addr.ramdisk_addr + sizeof(image_header_t), image_get_data_size(hdr));
		strcat(cmd, buf);
		setenv("bootargs",cmd);
	}
out:
	usb_power_off();

	return ret;

loop:
	while(1);
}

U_BOOT_CMD(
		nvt_boot,	2,	0,	do_nvt_boot_cmd,
		"",""
	  );

#if defined(CONFIG_N62001)
int do_n62001_boot_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char cmd[512] = {0};
	char buf[255] = {0};
	int ret = 0,len;
	unsigned char *ker_load_addr = 0;
	unsigned char *ramdisk_load_addr = 0;
	struct bootloader_message *misc_info;
	nvt_emmc_image_info *pimg;
	int block_cnt = 0;
	struct boot_address addr;

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

#if defined(CONFIG_FLASH_P3)
	if(getenv("p3") != NULL) {
		sprintf(buf,"%s ","root=/dev/mmcblk0p7 rw ");
		setenv("ramdisk_loadaddr", NULL);
	}else
#endif
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

#if defined(NT72668_REV_B)
	strcat(cmd, "vmalloc=560M ");

	{
		int cpus;

		if(getenv("cpus") != NULL) {
			cpus = simple_strtoul(getenv("cpus"),NULL,10);
			if(cpus > 4 || cpus < 1)
				cpus = 4;
		} else {
			cpus = 4;
		}
		sprintf(buf, "maxcpus=%d ",cpus);
		strcat(cmd,buf);
	}
#endif

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

	setenv("bootargs",cmd);

#if defined(CONFIG_NT72668_TK_TZONE)
	pimg = nvt_emmc_get_img_by_name("secos");
	if(pimg == NULL) {	
		printf("%s get secos image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, SECOS_BUFFER_BLOCK, &len, NULL);
#endif	


	ker_load_addr = simple_strtoul(getenv("kernel_loadaddr"),NULL,16);

#if ! defined(CONFIG_FLASH_P3)
	if(getenv("p3") != NULL) {
		setenv("ramdisk_loadaddr", NULL);
		sprintf(cmd,"nvtload 0x%x p3.img",ker_load_addr);
		ret = run_command(cmd, 0);
		goto out;
	}
#endif

	misc_info = ker_load_addr;
	block_cnt = (sizeof(struct bootloader_message)/SECTOR_IN_BYTES) + 1;
	memset(misc_info, 0 , block_cnt);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, misc_info, &len, &block_cnt);
	if(ret < 0) {
		printf("load MISC from emmc fail !\n");
		goto loop;
	}
	printf("command=%s\n",misc_info->command);

	if(getenv("ramdisk_loadaddr") != NULL)
		ramdisk_load_addr = simple_strtoul(getenv("ramdisk_loadaddr"),NULL,16);
	else
		ramdisk_load_addr = 0;

	addr.kernel_addr = ker_load_addr;
	addr.ramdisk_addr = ramdisk_load_addr;

	if(strncmp(misc_info->command, "boot-recovery", 13) == 0)
	{
		pimg = nvt_emmc_get_img_by_name("ker1");

		if(pimg == NULL) {
			printf("%s get recovery kernel image fail !\n");
			goto loop;
		}
		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 1 from emmc fail !\n");
			goto loop;
		}

		if(getenv("wdt") != NULL) {
			unsigned short en_wdt = 0;
		
			en_wdt = simple_strtoul(getenv("wdt"), NULL, 10);
			if(en_wdt != 0)
				nt72668_stbc_disable_wdog();
		}

	} else {
		pimg = nvt_emmc_get_img_by_name("ker0");

		if(pimg == NULL) {
			printf("%s get normal kernel image fail !\n");
			goto loop;
		}


		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 0 from emmc fail !\n");
			goto loop;
		}

	}

	if(len > 0 && addr.ramdisk_addr != 0) {
		const image_header_t *hdr = (const image_header_t *)addr.ramdisk_addr;
		sprintf(buf,"initrd=0x%x,0x%x", addr.ramdisk_addr + sizeof(image_header_t), image_get_data_size(hdr));
		strcat(cmd, buf);
		setenv("bootargs",cmd);
	}
out:
	usb_power_off();

	return ret;

loop:
	while(1);
}


U_BOOT_CMD(
		n62001_boot,	2,	0,	do_n62001_boot_cmd,
		"",""
	  );
#endif


#if defined(CONFIG_TCL_CN_668)
int do_tclcn_boot_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char cmd[512] = {0};
	char buf[255] = {0};
	int ret = 0,len;
	unsigned char *ker_load_addr = 0;
	unsigned char *ramdisk_load_addr = 0;
	struct bootloader_message *misc_info;
	nvt_emmc_image_info *pimg;
	int block_cnt = 0;
	struct boot_address addr;
	tclcn_fwupdate_flag __attribute__((__aligned__(64))) uflag = {};

	if(nt72668_stbc_sar_adc_pressed(NVT_SAR_ADC1))
	{
		nt72668_fwupdate_image();
        //when can't get update package from usb, normal boot need set osd
	    nt72668_show_boot_logo();
	}

	ret = tclcn668_read_update_flag(&uflag);
	if(0 == ret)
	{
		printf("boot fwupdate uflag.fwupdate_flag = 0x%x \n",uflag.fwupdate_flag);
		if(uflag.fwupdate_flag == 0x8001)
		{
			nt72668_stbc_disable_wdog();
			nt72668_fwupdate_image();
		}
	}
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

	if (getenv("mali_fb_start") != NULL && getenv("mali_fb_start") != NULL) {
		sprintf(buf,"mali.mali_fb_start=%s mali.mali_fb_size=%s ",getenv("mali_fb_start"),getenv("mali_fb_start"));
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

#if defined(NT72668_REV_B)
	strcat(cmd, "vmalloc=560M ");

	{
		int cpus;

		if(getenv("cpus") != NULL) {
			cpus = simple_strtoul(getenv("cpus"),NULL,10);
			if(cpus > 4 || cpus < 1)
				cpus = 4;
		} else {
			cpus = 4;
		}
		sprintf(buf, "maxcpus=%d ",cpus);
		strcat(cmd,buf);
	}
#endif

	if(getenv("dump_restore") != NULL)
	{
		strcat(cmd,"rdinit=/linuxrc rootwait");
	}
	else
	{
		strcat(cmd,"init=/init rootwait ");
	}

	if(getenv("kparam_addon") != NULL) {
		sprintf(buf," %s ", getenv("kparam_addon"));
		strcat(cmd, buf);
	}

	setenv("bootargs",cmd);

#if defined(CONFIG_NT72668_TK_TZONE)
	pimg = nvt_emmc_get_img_by_name("secos");
	if(pimg == NULL) {	
		printf("%s get secos image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, SECOS_BUFFER_BLOCK, &len, NULL);
#endif	

	ker_load_addr = simple_strtoul(getenv("kernel_loadaddr"),NULL,16);

	if(getenv("p3") != NULL) {
		setenv("ramdisk_loadaddr", NULL);
		sprintf(cmd,"nvtload 0x%x p3.img",ker_load_addr);
		ret = run_command(cmd, 0);
		goto out;
	}

	misc_info = ker_load_addr;
	block_cnt = (sizeof(struct bootloader_message)/SECTOR_IN_BYTES) + 1;
	memset(misc_info, 0 , block_cnt);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, misc_info, &len, &block_cnt);
	if(ret < 0) {
		printf("load MISC from emmc fail !\n");
		goto loop;
	}
	printf("command=%s\n",misc_info->command);

	if(getenv("ramdisk_loadaddr") != NULL)
		ramdisk_load_addr = simple_strtoul(getenv("ramdisk_loadaddr"),NULL,16);
	else
		ramdisk_load_addr = 0;

	addr.kernel_addr = ker_load_addr;
	addr.ramdisk_addr = ramdisk_load_addr;

	if(strncmp(misc_info->command, "boot-recovery", 13) == 0)
	{
        /* Disable watchdog when boot recovery */
        nt72668_stbc_disable_wdog();

		pimg = nvt_emmc_get_img_by_name("ker1");

		if(pimg == NULL) {
			printf("%s get recovery kernel image fail !\n");
			goto loop;
		}
		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 1 from emmc fail !\n");
			goto loop;
		}


	} else {
		pimg = nvt_emmc_get_img_by_name("ker0");

		if(pimg == NULL) {
			printf("%s get normal kernel image fail !\n");
			goto loop;
		}

		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 0 from emmc fail !\n");
			goto loop;
		}

	}

	if(len > 0 && addr.ramdisk_addr != 0) {
		const image_header_t *hdr = (const image_header_t *)addr.ramdisk_addr;
		sprintf(buf,"initrd=0x%x,0x%x", addr.ramdisk_addr + sizeof(image_header_t), image_get_data_size(hdr));
		strcat(cmd, buf);
		setenv("bootargs",cmd);
	}

out:
	usb_power_off();

	return ret;

loop:
	while(1);
}

U_BOOT_CMD(
		tclcn_boot,	2,	0,	do_tclcn_boot_cmd,
		"",""
	  );

#endif

#if defined(CONFIG_TCL_EU_668)

int do_tcleu_boot_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char cmd[512] = {0};
	char buf[255] = {0};
	int ret = 0,len;
	unsigned char *ker_load_addr = 0;
	unsigned char *ramdisk_load_addr = 0;
	struct bootloader_message *misc_info;
	update_flag __attribute__((__aligned__(64))) uflag = {};
	boot_flag __attribute__((__aligned__(64))) bflag = {};
	nvt_emmc_image_info *pimg;
	unsigned int hotkey_update = 0;

	struct boot_address addr;

	hotkey_update = nt72668_stbc_sar_adc_pressed(NVT_SAR_ADC1);

	if(hotkey_update) {
		run_command("usb start", 0);
		sprintf(cmd,"fatload usb 0 0x%x %s 4", CONFIG_FWUP_RAM_START, CONFIG_TCL_EU_UPDATE_FNAME);
		if(run_command(cmd, 0))
		{
			hotkey_update = 0;
			
			//when can't get update package from usb, normal boot need set osd
			nt72668_show_boot_logo();
		}
	}

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

	if (getenv("mali_fb_start") != NULL && getenv("mali_fb_size") != NULL) {
		sprintf(buf,"mali.mali_fb_start=%s mali.mali_fb_size=%s ",getenv("mali_fb_start"),getenv("mali_fb_size"));
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

	ret = nt72668_read_boot_flag(&bflag);

	if(ret < 0) {
		bflag.uboot_idx = 0;
		bflag.kernel_idx = 0;
		bflag.fs_idx = 0;
	}

	if(getenv("usb") != NULL){
		sprintf(buf,"root=%s rw ",getenv("usb"));
		setenv("ramdisk_loadaddr", NULL);
	} else if(hotkey_update) {
		sprintf(buf, "root=/dev/ram0 rw ");
		run_command("emiscmd HOTKEY_UPDATE", 0);
	} else {
		ret = nt72668_read_update_flag(&uflag);

		if(ret == 0 && uflag.update_ptable_flag == FW_UPDATE_PTBL)
			run_command("epart write", 0);

		sprintf(buf,"root=/dev/mmcblk0p%d ro ",nvt_emmc_get_partition_id_by_name("system"));
	}


	strcat(buf,"rootfstype=ext4 ");
	strcat(cmd,buf);

	if(getenv_yesno("quiet"))
		strcat(cmd,"quiet ");

#if defined(NT72668_REV_B)
	strcat(cmd, "vmalloc=560M ");

	{
		int cpus;

		if(getenv("cpus") != NULL) {
			cpus = simple_strtoul(getenv("cpus"),NULL,10);
			if(cpus > 4 || cpus < 1)
				cpus = 4;
		} else {
			cpus = 4;
		}
		sprintf(buf, "maxcpus=%d ",cpus);
		strcat(cmd,buf);
	}
#endif

	if(hotkey_update)
		strcat(cmd,"rdinit=/linuxrc rootwait ");
	else
		strcat(cmd,"init=/init rootwait");

	if(getenv("kparam_addon") != NULL) {
		sprintf(buf," %s ", getenv("kparam_addon"));
		strcat(cmd, buf);
	}

	setenv("bootargs",cmd);

#if defined(CONFIG_NT72668_TK_TZONE)
	pimg = nvt_emmc_get_img_by_name("secos");
	if(pimg == NULL) {	
		printf("%s get secos image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, SECOS_BUFFER_BLOCK, &len, NULL);
#endif	

	ker_load_addr = simple_strtoul(getenv("kernel_loadaddr"),NULL,16);

	addr.kernel_addr = ker_load_addr;
	if(hotkey_update)
		addr.ramdisk_addr = simple_strtoul(getenv("ramdisk_loadaddr"),NULL,16);
	else
		addr.ramdisk_addr = NULL;

	if(getenv("p3") != NULL) {
		setenv("ramdisk_loadaddr", NULL);
		sprintf(cmd,"nvtload 0x%x p3.img",ker_load_addr);
		ret = run_command(cmd, 0);
		goto out;
	}

	if(bflag.kernel_idx == 0)
		pimg = nvt_emmc_get_img_by_name("ker0");
	else
		pimg = nvt_emmc_get_img_by_name("ker1");

	if(pimg == NULL) {
		printf("%s get normal kernel image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);

	if(ret < 0) {
		printf("load kernel0 from emmc fail !\n");
		goto loop;
	}

	if(!hotkey_update)
		setenv("ramdisk_loadaddr", NULL);
	else {
		if(len > 0 && addr.ramdisk_addr != 0) {
			const image_header_t *hdr = (const image_header_t *)addr.ramdisk_addr;
			sprintf(buf,"initrd=0x%x,0x%x", addr.ramdisk_addr + sizeof(image_header_t), image_get_data_size(hdr));
			strcat(cmd, buf);
			setenv("bootargs",cmd);
		}
	}
out:
	usb_power_off();

	return ret;

loop:
	while(1);
}

U_BOOT_CMD(
		tcleu_boot,	2,	0,	do_tcleu_boot_cmd,
		"",""
	  );


#endif

#if defined(CONFIG_TCL_AU_668)

int do_tclau_boot_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char cmd[512] = {0};
	char buf[255] = {0};
	int ret = 0,len;
	unsigned char *ker_load_addr = 0;
	unsigned char *ramdisk_load_addr = 0;
	struct bootloader_message *misc_info;
	nvt_emmc_image_info *pimg;
	int block_cnt = 0;
	struct boot_address addr;
	tclcn_fwupdate_flag __attribute__((__aligned__(64))) uflag = {};

	if(nt72668_stbc_sar_adc_pressed(NVT_SAR_ADC1))
	{
		nt72668_fwupdate_image();		
        //when can't get update package from usb, normal boot need set osd
	    nt72668_show_boot_logo();
	}

	ret = tclcn668_read_update_flag(&uflag);
	if(0 == ret)
	{
		printf("boot fwupdate uflag.fwupdate_flag = 0x%x \n",uflag.fwupdate_flag);
		if(uflag.fwupdate_flag == 0x8001)
		{
			nt72668_stbc_disable_wdog();
			nt72668_fwupdate_image();
		}
	}

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

	if (getenv("mali_fb_start") != NULL && getenv("mali_fb_start") != NULL) {
		sprintf(buf,"mali.mali_fb_start=%s mali.mali_fb_size=%s ",getenv("mali_fb_start"),getenv("mali_fb_start"));
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

#if defined(NT72668_REV_B)
	strcat(cmd, "vmalloc=560M ");

	{
		int cpus;

		if(getenv("cpus") != NULL) {
			cpus = simple_strtoul(getenv("cpus"),NULL,10);
			if(cpus > 4 || cpus < 1)
				cpus = 4;
		} else {
			cpus = 4;
		}
		sprintf(buf, "maxcpus=%d ",cpus);
		strcat(cmd,buf);
	}
#endif

	if(getenv("dump_restore") != NULL)
	{
		strcat(cmd,"rdinit=/linuxrc rootwait");
	}
	else
	{
		strcat(cmd,"init=/init rootwait ");
	}

	if(getenv("kparam_addon") != NULL) {
		sprintf(buf," %s ", getenv("kparam_addon"));
		strcat(cmd, buf);
	}

	setenv("bootargs",cmd);

#if defined(CONFIG_NT72668_TK_TZONE)
	pimg = nvt_emmc_get_img_by_name("secos");
	if(pimg == NULL) {	
		printf("%s get secos image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, SECOS_BUFFER_BLOCK, &len, NULL);
#endif	


	ker_load_addr = simple_strtoul(getenv("kernel_loadaddr"),NULL,16);

	if(getenv("p3") != NULL) {
		setenv("ramdisk_loadaddr", NULL);
		sprintf(cmd,"nvtload 0x%x p3.img",ker_load_addr);
		ret = run_command(cmd, 0);
		goto out;
	}

	misc_info = ker_load_addr;
	block_cnt = (sizeof(struct bootloader_message)/SECTOR_IN_BYTES) + 1;
	memset(misc_info, 0 , block_cnt);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc image fail !\n",__func__);
		goto loop;
	}

	ret = pimg->read_img(pimg, misc_info, &len, &block_cnt);
	if(ret < 0) {
		printf("load MISC from emmc fail !\n");
		goto loop;
	}
	printf("command=%s\n",misc_info->command);

	if(getenv("ramdisk_loadaddr") != NULL)
		ramdisk_load_addr = simple_strtoul(getenv("ramdisk_loadaddr"),NULL,16);
	else
		ramdisk_load_addr = 0;

	addr.kernel_addr = ker_load_addr;
	addr.ramdisk_addr = ramdisk_load_addr;

	if(strncmp(misc_info->command, "boot-recovery", 13) == 0)
	{
        /* Disable watchdog when boot recovery */
        nt72668_stbc_disable_wdog();

		pimg = nvt_emmc_get_img_by_name("ker1");

		if(pimg == NULL) {
			printf("%s get recovery kernel image fail !\n");
			goto loop;
		}
		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 1 from emmc fail !\n");
			goto loop;
		}


	} else {
		pimg = nvt_emmc_get_img_by_name("ker0");

		if(pimg == NULL) {
			printf("%s get normal kernel image fail !\n");
			goto loop;
		}

		ret = pimg->read_img(pimg, ker_load_addr, &len, &addr);
		if(ret < 0) {
			printf("load kernel 0 from emmc fail !\n");
			goto loop;
		}

	}

	if(len > 0 && addr.ramdisk_addr != 0) {
		const image_header_t *hdr = (const image_header_t *)addr.ramdisk_addr;
		sprintf(buf,"initrd=0x%x,0x%x", addr.ramdisk_addr + sizeof(image_header_t), image_get_data_size(hdr));
		strcat(cmd, buf);
		setenv("bootargs",cmd);
	}

out:
	usb_power_off();

	return ret;

loop:
	while(1);
}

U_BOOT_CMD(
		tclau_boot,	2,	0,	do_tclau_boot_cmd,
		"",""
	  );

#endif

