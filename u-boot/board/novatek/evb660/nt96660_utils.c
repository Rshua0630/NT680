/*
 *  board/nvt/evb568/nt96650_utils.c
 *
 *  Author:	Alvin lin
 *  Created:	Dec 18, 2011
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <asm/arch/hardware.h>
#include <lcd.h>
#if defined(CONFIG_SBOOT)
#include <sha1.h>
#include "rsa/SBKN_CC_API.h"
#endif
#include "nt96660_utils.h"
#include <image.h>

#include "nvt_type.h"
#include "CoreInfo.h"
#include "NvtLinuxInfo.h"
#include "PartitionInfo.h"

#define CODE_INFO_ADDR 0x800003C0

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) | ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))
#endif

#if defined(CONFIG_RAMOOPS_LOG)
#define RAMOOPS_BUF_SIZE (32 * 1024)
#define RAMOOPS_REC_SIZE (16 * 1024)
#endif

CORE_INFO* m_pCoreInfo = (CORE_INFO*)CODE_INFO_ADDR;
extern char CONFIG_SYS_TEXT_BASE[];

/* This function will be called by common/cmd_nand.c "nand init" */
int nt96660_nand_mtdpart_config(const EMB_PARTITION* pPtnTbl)
{
    char cmd[512];
    char buf[40];
    uint32_t i;
    int ret = 0;

    /* Get from 0x8000000 */
    if (pPtnTbl == NULL)
	pPtnTbl = (EMB_PARTITION*)(0x80000000 + EMB_PARTITION_INFO_OFFSET);

    sprintf(cmd, "nand0=spi_nand.0");
    ret = setenv("mtdids",cmd);
    if (ret) {
	printf("%s: error set\n", __func__);
	return ret;
    }

    sprintf(cmd,"mtdparts=spi_nand.0:");

    /* To parse mtdparts for rootfs partition table */
    for(i=0;i<EMB_PARTITION_INFO_COUNT;i++)
    {
        const EMB_PARTITION* p = pPtnTbl + i;
        switch(p->EmbType)
        {
        case EMBTYPE_UITRON:
            sprintf(buf, "0x%x@0x%x(uitron),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_UBOOT:
            sprintf(buf, "0x%x@0x%x(uboot),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_UENV:
            sprintf(buf, "0x%x@0x%x(udev),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_LINUX:
            sprintf(buf, "0x%x@0x%x(linux),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_RAMFS:
            sprintf(buf, "0x%x@0x%x(ramfs),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_UBIFS:
            sprintf(buf, "0x%x@0x%x(rootfs),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_FAT:
            sprintf(buf, "0x%x@0x%x(fat),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_DSP:
            sprintf(buf, "0x%x@0x%x(dsp),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        case EMBTYPE_PSTORE:
            sprintf(buf, "0x%x@0x%x(pstore),", p->PartitionSize, p->PartitionOffset);
	    strcat(cmd, buf);
            break;
        default:
            break;
        }
    }

    cmd[strlen(cmd) - 1] = '\0';
    ret = setenv("mtdparts",cmd);

    return ret;
}


int do_nvt_boot_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    NVTLINUXINFO* pLinuxInfo = (NVTLINUXINFO*)m_pCoreInfo->LinuxAddr;
    const EMB_PARTITION* pPtnTbl = (EMB_PARTITION*)(0x80000000 + EMB_PARTITION_INFO_OFFSET);
    uint32_t i,j;
    uint32_t part_num;

    char cmd_line[1024] = {0};
    char buf[256];

    if((u32)CONFIG_SYS_TEXT_BASE + (CFG_MEM_SIZE<<20) > m_pCoreInfo->LinuxAddr+m_pCoreInfo->LinuxSize)
    {
        printf("u-boot mem lower-bound(%08X) > linux memory boundary (%08X)\r\n",(u32)CONFIG_SYS_TEXT_BASE + (CFG_MEM_SIZE<<20),(u32)m_pCoreInfo->LinuxAddr+(u32)m_pCoreInfo->LinuxSize);
        printf("may CFG_MEM_SIZE need modify.\r\n");
        goto loop;
    }

    if(pLinuxInfo->InfoFourCC != MAKEFOURCC('L','I','F','O'))
    {
        printf("ERR: NVTLINUXINFO FourCC\r\n");
        goto loop;
    }

    if(pLinuxInfo->InfoSize != sizeof(NVTLINUXINFO))
    {
        printf("ERR: NVTLINUXINFO Size uBoot(%d),uITRON(%d)\r\n",sizeof(NVTLINUXINFO),pLinuxInfo->InfoSize);
        goto loop;
    }

    ulong addr = m_pCoreInfo->RootFsAddr - image_get_header_size ();

    if(m_pCoreInfo->RootFsAddr!=0)
    {
        puts ("Verifying Root-FS Checksum ... ");
	    if (!image_check_dcrc ((image_header_t *)addr))
        {
	    	printf ("Bad Data CRC\n");
		    goto loop;
    	}
        puts ("OK.\n");
    }

    sprintf(cmd_line,"%08X",(u32)m_pCoreInfo->uImageAddr);
    setenv("kernel_loadaddr", cmd_line);


	//sprintf(cmd_line,"%s ",getenv("bootargs"));
    cmd_line[0] = 0;
    strcat(cmd_line,pLinuxInfo->CmdLine);
    strcat(cmd_line," ");

    j = 0;
    part_num = 0;
    /* To parse mtdparts for rootfs partition table */
    strcat(cmd_line,"mtdparts=spi_nand.0:");
    for(i=0;i<EMB_PARTITION_INFO_COUNT;i++)
    {
	const EMB_PARTITION* p = pPtnTbl + i;
	switch(p->EmbType)
	{
	case EMBTYPE_UITRON:
		sprintf(buf,"0x%x@0x%x(uitron)ro,", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		j++;
	    break;
	case EMBTYPE_UBOOT:
		sprintf(buf,"0x%x@0x%x(uboot)ro,", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		j++;
	    break;
	case EMBTYPE_UENV:
		sprintf(buf,"0x%x@0x%x(uenv),", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		j++;
	    break;
	case EMBTYPE_LINUX:
		sprintf(buf,"0x%x@0x%x(linux)ro,", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		j++;
	    break;
	case EMBTYPE_RAMFS:
		sprintf(buf,"0x%x@0x%x(ramfs),", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		j++;
	    break;
	case EMBTYPE_UBIFS:
		sprintf(buf,"0x%x@0x%x(rootfs),", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		part_num = j;
	    break;
        case EMBTYPE_FAT:
                sprintf(buf,"0x%x@0x%x(fat),", p->PartitionSize, p->PartitionOffset);
                strcat(cmd_line, buf);
                j++;
            break;
	case EMBTYPE_DSP:
		sprintf(buf,"0x%x@0x%x(dsp)ro,", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		j++;
	    break;
	case EMBTYPE_PSTORE:
		sprintf(buf,"0x%x@0x%x(pstore)ro,", p->PartitionSize, p->PartitionOffset);
		strcat(cmd_line, buf);
		j++;
	    break;
	default:
	    break;
	}
    }

    cmd_line[strlen(cmd_line) - 1] = ' ';

#if defined(CONFIG_NVT_SQUASHFS_SUPPORT)
    sprintf(buf," root=/dev/mtdblock%d rootfstype=squashfs ro ", part_num);
    strcat(cmd_line, buf);
#elif defined(CONFIG_NVT_JFFS2_SUPPORT)
    sprintf(buf," root=/dev/mtdblock%d rootfstype=jffs2 rw rootwait", part_num);
    strcat(cmd_line, buf);
#else
    ////////////////////////////////////////////////////////////////////////////
    // set initframfs related info: rd_start, rd_size, root, rootfstype
    ////////////////////////////////////////////////////////////////////////////
    if(m_pCoreInfo->RootFsAddr!=0)
    {
        sprintf(buf,"rd_start=0x%08X ",(u32)m_pCoreInfo->RootFsAddr);
        strcat(cmd_line,buf);

        sprintf(buf,"rd_size=0x%08X ",(u32)image_get_size((image_header_t *)addr));
        strcat(cmd_line,buf);

        strcat(cmd_line,"root=/dev/ram0 ");
        strcat(cmd_line,"rootfstype=squashfs ");
    }
    else //run on UBI-fs
    {
        strcat(cmd_line,"root=ubi0:rootfs rootfstype=ubifs rootwait rw ubi.fm_autoconvert=1 ");
    }

    sprintf(buf," ubi.mtd=%d ", part_num);
    strcat(cmd_line, buf);
#endif

#ifdef	CONFIG_BOOTARGS
    strcat(cmd_line, CONFIG_BOOTARGS);
    strcat(cmd_line, " ");
#endif

    sprintf(buf,"uboot_ver=%s ",__DATE__);
    strcat(cmd_line,buf);

#if defined(CONFIG_RAMOOPS_LOG)
    sprintf(buf,"mem=0x%08X@0x%08X ", (u32)(m_pCoreInfo->uImageAddr - m_pCoreInfo->LinuxAddr - RAMOOPS_BUF_SIZE), 
        (u32)(m_pCoreInfo->LinuxAddr - 0x80000000));
    strcat(cmd_line,buf);
    sprintf(buf,"mem=0x%08X@0x%08X ", (u32)(m_pCoreInfo->LinuxAddr + m_pCoreInfo->LinuxSize - m_pCoreInfo->uImageAddr), (u32)(m_pCoreInfo->uImageAddr - 0x80000000));
    strcat(cmd_line,buf);
    sprintf(buf,"memmap=0x%08X@0x%08X ", RAMOOPS_BUF_SIZE, 
        (u32)(m_pCoreInfo->uImageAddr - 0x80000000 - RAMOOPS_BUF_SIZE));
    strcat(cmd_line,buf);
    if (m_pCoreInfo->DspAddr != 0) {
	sprintf(buf,"memmap=0x%08X@0x%08X ", (u32)m_pCoreInfo->DspSize, (u32)(m_pCoreInfo->DspAddr - 0x80000000));
	strcat(cmd_line,buf);
    }
    sprintf(buf,"memmap=0x%08X@0 ", (u32)(m_pCoreInfo->LinuxAddr - 0x80000000));
    strcat(cmd_line, buf); 

    sprintf(buf,"ramoops.record_size=%d ", RAMOOPS_REC_SIZE);
    strcat(cmd_line,buf);
    sprintf(buf,"ramoops.mem_address=0x%08X ", (u32)(m_pCoreInfo->uImageAddr - 0x80000000 - RAMOOPS_BUF_SIZE));
    strcat(cmd_line,buf);
    sprintf(buf,"ramoops.mem_size=%d ", RAMOOPS_BUF_SIZE);
    strcat(cmd_line,buf);
#else
    sprintf(buf,"mem=0x%08X@0x%08X ", m_pCoreInfo->LinuxSize, (m_pCoreInfo->LinuxAddr - 0x80000000));
    strcat(cmd_line,buf);
    sprintf(buf,"memmap=0x%08X@0 ", (m_pCoreInfo->LinuxAddr - 0x80000000));
    strcat(cmd_line,buf);
    if (m_pCoreInfo->DspAddr != 0) {
       sprintf(buf,"memmap=0x%08X@0x%08X ", m_pCoreInfo->DspSize, (m_pCoreInfo->DspAddr - 0x80000000));
       strcat(cmd_line,buf);
    }
#endif

    strcat(cmd_line,"init=/init ");

    setenv("bootargs",cmd_line);
    setenv("verify","n"); //check crc32 of linux has been moved to loader.

	return 0;

loop:
	while(1);
}

U_BOOT_CMD(
	nvt_boot,	2,	0,	do_nvt_boot_cmd,
	"To implement nvt platform boot initialization","\n"
);

#if 0
#define REG_MPLL_PAGE_SELECT		__REG(0xCD0200BC)
#define MPLL_PAGE_B_EN				(1 << 0)
#define MPLL_PAGE_0_EN				(1 << 1)

#define REG_MPLL_ADDR				__REG(0xCD0200B0)
#define REG_MPLL_DATA				__REG(0xCD0200B4)

void nt72568_pll_set(NT72568_MPLL_PAGE_TYPE type,u32 addr, u32 val)
{
	if (type == MPLL_PAGE_0)
		REG_MPLL_PAGE_SELECT = MPLL_PAGE_0_EN;
	else
		REG_MPLL_PAGE_SELECT = MPLL_PAGE_B_EN;

	REG_MPLL_ADDR = addr;

	REG_MPLL_DATA = val;
}

void nt72568_jtag_disable(void)
{
	//mips jtag disable
	(*(volatile unsigned int*)0xbd100420) |= (1<<3);
	(*(volatile unsigned int*)0xbd100408) |= (0x1F800000);
	(*(volatile unsigned int*)0xbd100400) = (1<<23)|(1<<28);
	(*(volatile unsigned int*)0xbd100404) = (1<<24)|(1<<25)|(1<<26)|(1<<27);

	//audio dsp jtag disable
	(*(volatile unsigned int*)0xbd110020) &= (~1);
	(*(volatile unsigned int*)0xbd110008) |= (0x0000001F);
	(*(volatile unsigned int*)0xbd110004) &= ~(0x0000001F);
	(*(volatile unsigned int*)0xbd110000) |= (0x0000001F);


}
#endif

