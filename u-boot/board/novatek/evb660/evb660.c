/*
 * (C) Copyright 2003
 * Thomas.Lange@corelatus.se
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <lcd.h>
#include <version.h>
#include <asm/mipsregs.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>

#include <asm/arch/nt96660_regs.h>
#include <asm/arch/nt96660_product_specific.h>

#include "nvt_type.h"
#include "PartitionInfo.h"
#include "NvtPackInfo.h"
#include "PartitionInfo.h"
#include "BinInfo.h"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) | ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))
#endif

#define UINT32_SWAP(data)           (((((UINT32)(data)) & 0x000000FF) << 24) | \
                                     ((((UINT32)(data)) & 0x0000FF00) << 8) | \
                                     ((((UINT32)(data)) & 0x00FF0000) >> 8) | \
                                     ((((UINT32)(data)) & 0xFF000000) >> 24))   ///< Swap [31:24] with [7:0] and [23:16] with [15:8].

#define dma_getNonCacheAddr(addr)       (((addr) & 0x1FFFFFFF)|0xA0000000)

static const GUID GUID_FW =        // {8827BE90-36CD-4FC2-A987-73A8484E84B1}
        { 0x8827be90, 0x36cd, 0x4fc2, { 0xa9, 0x87, 0x73, 0xa8, 0x48, 0x4e, 0x84, 0xb1 } };

extern BININFO gBinInfo;

typedef struct _HEADER_BFC{
    UINT32 uiFourCC;    ///< FourCC = BCL1
    UINT32 uiAlgorithm; ///< algorithm always is 9
    UINT32 uiSizeUnComp;///< big endian uncompressed size
    UINT32 uiSizeComp;  ///< big endian compressed size
}HEADER_BFC,*PHEADER_BFC;

static u32 nt96660_process_all_in_one(void);
u32 nt96660_is_need_upd_ubi(void);
int nt96660_nand_mtdpart_config(const EMB_PARTITION* pPtnTbl);
int nt96660_eth_initialize(bd_t *bis);

#if 0
#include "nt72568_stbc.h"
#include "nt72568_ddr_cfg.h"
#include "nt72568_regs.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f(void)
{
#if 0
	volatile unsigned long *pll    = (volatile unsigned long *)(0xbc12c120);
	volatile unsigned long *select = (volatile unsigned long *)(0xbc12c130);
	volatile unsigned long *clk    = (volatile unsigned long *)(0xbc12c140);

    * (volatile unsigned long *) 0xbc040204 = 0x72682;
    * (volatile unsigned long *) 0xbc040204 = 0x28627;
    * (volatile unsigned long *) 0xbc040208 = 1;
    * (volatile unsigned long *) 0xbd110020 = 0x8;

	/* cpu clock */
	switch (select[0])
	{
		case 0:
			clk[0] = pll[0] / 4;
			break;
		case 1:
			clk[0] = pll[0] / 3;
			break;
		case 2:
			clk[0] = pll[0] / 2;
			break;
		case 3:
			clk[0] = pll[0];
			break;
	}


	/* ahb clock */
	switch (select[2])
	{
		case 0:
			clk[2] = pll[0] / 6;
			break;
		case 1:
			clk[2] = pll[0] / 4;
			break;
		case 3:
			clk[2] = pll[3] / 2;
			break;

		case 4:
		default:
		clk[2] = 96000000;
			break;
	}

	/* axi clock */
	switch (select[3])
	{
		case 0:
			clk[3] = pll[0] / 6;
			break;
		case 1:
			clk[3] = pll[0] / 4;
			break;
		case 2:
			clk[3] = pll[3] / 2;
			break;
		case 3:
			clk[3] = pll[3] / 1;
			break;
	}

	clk[1] = pll[1];
	/*
	 * Nothing to do in this dummy implementation
	 */

#if defined(CONFIG_LCD)
	gd->fb_base = CONFIG_FB_ADDR;
#endif

	nt72568_stbc_enable_wdog(5);
#endif

	return 0;
}

phys_size_t initdram(int board_type)
{
	/* Sdram is setup by assembler code */
	/* If memory could be changed, we should return the true value here */
	return (CFG_MEM_SIZE * 1024 * 1024);
}

int checkboard (void)
{
#if 0
	volatile unsigned long *freq = (volatile unsigned long *)(0xbc12c140);

	printf("CPU  @ %lu.%lu MHz\n",
			freq[0]/1000000, freq[0]%1000000);
	printf("DDR  @ %lu.%lu MHz\n",
			freq[1]/1000000, freq[1]%1000000);
	printf("AHB  @ %lu.%lu MHz\n",
			freq[2]/1000000, freq[2]%1000000);
	printf("AXI  @ %lu.%lu MHz\n",
			freq[3]/1000000, freq[3]%1000000);
#else
	int	rat0,rat1,rat2;
	int	pllRatio;
	rat2 = *(volatile unsigned long *)0xC00211B0;
	rat1 = *(volatile unsigned long *)0xC00211AC;
	rat0 = *(volatile unsigned long *)0xC00211A8;
	pllRatio = 12 * (((rat2 << 16) | (rat1 << 8) | (rat0 << 0)) / 131072);
	printf(">CPU @ %u MHz\n", pllRatio);
#endif
	printf("\n");

	return 0;
}

int board_eth_init(bd_t *bis)
{
	int rc = 0;

#ifdef CONFIG_ETHNET
	rc = nt96660_eth_initialize(bis);
#endif

	return rc;
}

static void nt96650_set_default_env(void)
{
#if 0
	HW_DDR_CFG *hw_ddr_cfg;
#endif
	//unsigned char buf[64];
#if 0
	unsigned int ddr_cfg = REG_DDR_SETTING;
	unsigned int ddr_num;

	hw_ddr_cfg = nt72568_get_hw_ddr_cfg();
#endif
	//get emem setting
	if(getenv("emem") == NULL) {
#if 0
		if(hw_ddr_cfg == NULL)
			setenv("emem","0x7800000");
		else {
			if(hw_ddr_cfg->CFG_DDR_SIZE == 128)
				setenv("emem","0x7800000");
			else
				setenv("emem","0xFFC0000");
		}
#else
		setenv("emem","0x7800000");
#endif
	}

	//set memlow default

	if(getenv("memlow") == NULL) {
#if 0
		if(hw_ddr_cfg == NULL)
			setenv("memlow","90M");
		else {
			if(hw_ddr_cfg->CFG_DDR_SIZE == 128)
				setenv("memlow","54M");
			else
				setenv("memlow","90M");
		}
#else
		setenv("memlow","90M");
#endif
	}

	//set rmem env param
#if 0
	if(hw_ddr_cfg == NULL)
		setenv("rmem","128");
	else {
		sprintf(buf,"%d",hw_ddr_cfg->CFG_DDR_SIZE);
		setenv("rmem",buf);
	}
#else
	setenv("rmem","128");
#endif

#if 0
	if(ddr_cfg & DDR_SETTING_16BIT)
		ddr_num = 1;
	else
		ddr_num = 2;

	sprintf(buf,"%d",ddr_num);

	setenv("ddr_num", buf);

	//jtag disable ?
	if(getenv("jtagen")) {
		if(simple_strtoul(getenv("jtagen"),NULL,10) == 0)
			nt72568_jtag_disable();
	}
#endif
	//sprintf(buf, LOCAL_VERSION);
	//setenv("uboot_ver", buf);
}

#if 0
static void nt72568_reg_precfg(void)
{

	unsigned int reg_val;

	__REG(0xBD010024) = 0x33000000;

	//Set for EDID replace data when EDID authority switch to MIPS
	if((__REG(0xBC040220) & 0x200)==0x200)	// authority switch to MIPS
	{
        //standby mode support EDID

		//replace data function disable
		__REG(0xBC040C18) &= (~(0x0F<<17));

		//set replace data offset
		reg_val = (__REG(0xBC04011C)>>16) & 0xff;
		reg_val = (__REG(0xBC040D00) & 0xFFFFFF00) + reg_val;
		__REG(0xBC040D00) = reg_val;

		//set ID value for port 1~3
		reg_val = __REG(0xBC040124) & 0xFFFFFFFF;
		__REG(0xBC040D04) = reg_val;
		//set checksum value for port 1~3
		reg_val = __REG(0xBC040118) & 0x00FFFFFF;
		__REG(0xBC040D08) = reg_val;

		//replace data function enable
		__REG(0xBC040C18) |= (0x0F<<17);

        printf("ID=0x%x\n", __REG(0xBC040D04));
        printf("checksum=0x%x\n", __REG(0xBC040D08));
	}

    printf("ResReg2=0x%x\n", __REG(0xBC040120));
    //[0] - Enable logo.
    printf("Uparam=0x%x\n", ((__REG(0xBC040120)>>16) & 0xff));
}
#endif
int	misc_init_r   (void)
{
#if 0
	unsigned int boot_device;
	unsigned char cmd[64];
#endif
	int ret = -1;
	nt96650_set_default_env();
#if 0
	nt72568_dump_ddr_cfg_info();

	nt72568_reg_precfg();

	/* To check bootstrp setting and write to NAND */
	boot_device = (*(volatile unsigned long *)0xBC040210) & 0xff;
	if(boot_device == 0xa0 || boot_device == 0xb0)
	{
        nt72568_stbc_enable_wdog(0);

        memset(cmd, 0x00, 64);
		sprintf(cmd,"mux_burner");
	    ret = run_command(cmd, 0);
		if(ret < 0) {
			printf("Mux burning failed!\n");
		}else {
			printf("Completed mux burning. Please reboot.\n");
			while(1);
		}
	}


    if (((*(volatile unsigned char *) 0xbc04011c) & 0xff) == 0xaa) {

        nt72568_stbc_enable_wdog(0);

        memset(cmd, 0x00, 64);
        sprintf(cmd, "nvtupdate");
        ret = run_command(cmd, 0);
    	if(ret < 0) {
			printf("Mux burning failed!\n");
        }
        else
        {
            printf("USB update completed. Rebooting...\n");

            //2013.07.03 - The following API will let MCU go to idle mode in SRAM and reboot whole system.
            //This will avoid the system reset incomplete.
            //
            //nt72568_stbc_enable_wdog(1);
            nt72568_stbc_reboot_all();

            {
                unsigned long	tmo, out;
                unsigned char   count = 0;
                do
                {
                    tmo = get_timer(0);
                    out = tmo + (CONFIG_SYS_HZ) ; 	/*timeout 1 s*/

                    while(get_timer(0) < out) {
                        //waiting reboot
                    }
                    nt72568_stbc_reboot_all();
                    //printf("Rebooting...retry.\n");
                    printf("R");
                } while(++count < 5);
            }
        }
    }
#endif
    if(!nt96660_is_need_upd_ubi()) //no need to update
    {
        printf("nothing to update.\r\n");
	ret = nt96660_nand_mtdpart_config(NULL);
	if (ret) {
	    printf("%s: error %d\n", __func__, ret);
	    return ret;
	}
    } else {
        nt96660_process_all_in_one();
    }

    return ret;
}

u32 nt96660_is_need_upd_ubi(void)
{
    if(gBinInfo.fw.Resv[0]==0) //no need to update
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static u32 nt96660_process_all_in_one(void)
{
    u32 addr = gBinInfo.fw.Resv[0];
    u32 size = gBinInfo.fw.Resv[1];
    u32 addr_uncompressed_uitron_bin_info = gBinInfo.fw.Resv[2];
    u32 addr_fw_hdr = addr;
    u32 i = 0;
    char cmd[64];

    if(!IsEqualGUID(&GUID_FW,(void*)addr))
    {
        BININFO* pBinInfo = (BININFO*)addr_uncompressed_uitron_bin_info;
        //check if is uITRON FW?
        if(memcmp(pBinInfo->head.BinInfo_1,"NT9666",6)!=0)
        {
           printf("halt: wrong all in one bin(0). \r\n");
            while(1)
            {
                __asm__("nop");
            }
        }

        //check compressed uITRON
        HEADER_BFC* pBfc = (HEADER_BFC*)addr;

        if(pBfc->uiFourCC==MAKEFOURCC('B','C','L','1'))
        {
            printf("compressed uITRON\r\n");
            addr_fw_hdr +=  UINT32_SWAP(pBfc->uiSizeComp)+sizeof(HEADER_BFC);
        }
        else
        {
            printf("uncompressed uITRON\r\n");
            addr_fw_hdr += pBinInfo->head.BinLength;
        }

    }

    const EMB_PARTITION* pPtnTbl = (EMB_PARTITION*)(addr_uncompressed_uitron_bin_info+EMB_PARTITION_INFO_OFFSET);
    const HEADER_FW* pFw = (HEADER_FW*)addr_fw_hdr;

    nt96660_nand_mtdpart_config(pPtnTbl);
    if(!IsEqualGUID(&GUID_FW,(void*)pFw))
    {
        printf("halt: wrong all in one bin(1).\r\n");
        while(1)
        {
            __asm__("nop");
        }
    }

    // Verify if the partitions are set
    sprintf(cmd, "mtdparts");
    run_command(cmd, 0);

    HEADER_PARTITION* pPartition = (HEADER_PARTITION*)((UINT32)pFw + sizeof(HEADER_FW));
    for (i=0; i<pFw->TotalRes; i++)
    {
        HEADER_PARTITION* p = &pPartition[i];   // pointer to context descriptor
        UINT32 EmbType = pPtnTbl[p->PartitionID].EmbType;
        const UINT32 uiContextBuf = addr + p->Offset;

        if(EmbType!=EMBTYPE_UBIFS)
            continue;

        //uiContextBuf,p->Size
	sprintf(cmd, "nand erase.part rootfs");
        printf("%s\n", cmd);
        run_command(cmd, 0);

        // sprintf(cmd, "ubi part rootfs");
        // printf("%s\n", cmd);
        // run_command(cmd, 0);

	sprintf(cmd, "nand info");
        run_command(cmd, 0);

#if	defined(CONFIG_NVT_SQUASHFS_SUPPORT)
	sprintf(cmd, "nand write 0x%x rootfs 0x%x", uiContextBuf, p->Size);
	run_command(cmd, 0);
        printf("%s\n", cmd);
#elif	defined(CONFIG_NVT_JFFS2_SUPPORT)
	sprintf(cmd, "nand write.jffs2 0x%x rootfs 0x%x", uiContextBuf, p->Size);
	run_command(cmd, 0);
	printf("%s\n", cmd);
#else
        sprintf(cmd, "ubi part rootfs");
        run_command(cmd, 0);

	sprintf(cmd, "ubi create rootfs");
        run_command(cmd, 0);

	printf("ubifs size = 0x%08x\n", (uint32_t)p->Size);
	sprintf(cmd, "ubi write 0x%x rootfs 0x%x", (uint32_t)uiContextBuf, (uint32_t)p->Size);
	//sprintf(cmd, "nand write.trimffs 0x%x 0x%x 0x%x", uiContextBuf, pPtnTbl[p->PartitionID].PartitionOffset,p->Size);
	run_command(cmd, 0);
        sprintf(cmd, "ubi part rootfs");
        run_command(cmd, 0);
        sprintf(cmd, "ubifsmount ubi:rootfs");
        run_command(cmd, 0);
        sprintf(cmd, "ubifsls");
        run_command(cmd, 0);
        printf("%s\n", cmd);
#endif
    }

    printf("finish.\r\n");
    u32* pFinishByte = (u32*)dma_getNonCacheAddr((u32)&gBinInfo.fw.Resv[3]);
    *pFinishByte = 1; //finish, wait loader close u-boot

    while(1)
    {
        __asm__("nop");
    }
}
