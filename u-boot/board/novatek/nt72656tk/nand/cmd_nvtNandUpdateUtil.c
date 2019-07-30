/*
 * (C) Copyright 2011
 * Tom Lin, Novatek Software Engineering, Tom_Lin@novatek.com.tw.
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
#include <serial.h>
#include <asm-generic/errno.h>
#include "nand_partition_layout.h"

#define  NVT_FLASH_UPDATE_IMAGE_FROM_USB     1

unsigned char nand_dataCompare(unsigned char* pucOrigData, unsigned char* pucCompareData, unsigned long ulDataAddr, unsigned long ulDataLen, unsigned long ulLimitSpaceLen)
{
    unsigned long ulDataIndex = 0;
    int  cmd_ret;
    char cmd[256];


    printf("Start to comapre data\n\n");

    //Read back for data verification
    memset(cmd, '\0', 256);
    sprintf(cmd, "nvt_nand read 0x%lx 0x%lx 0x%lx 0x%lx", (unsigned long)pucCompareData, ulDataAddr, ulDataLen, ulLimitSpaceLen);
    cmd_ret = run_command (cmd, 0);
    if(cmd_ret != 0) {
        printf("read failed\n");
        return -1;
    }
	
    for(ulDataIndex = 0 ; ulDataIndex < ulDataLen ; ulDataIndex++)
    {
        if(pucOrigData[ulDataIndex] != pucCompareData[ulDataIndex])
//        if(memcmp(pucOrigData, pucCompareData, ulDataLen) != 0)
        {
            unsigned long DataIndexBase = ((ulDataIndex / 512) * 512);
            unsigned long DataIndexLimit = 0;
            unsigned long DataIndexStart = 0;

            printf("Comapre error at:%ld (Len:%ld)\r\n", ulDataIndex, ulDataLen);
            if((DataIndexBase + 4096) < ulDataLen)
                DataIndexLimit = DataIndexBase + 4096;
            else
                DataIndexLimit = ulDataLen;

            printf("Dump data from:0x%lx to 0x%lx\r\n", ulDataIndex, DataIndexLimit);

            printf("Orig\n\n");
            for(DataIndexStart = DataIndexBase ; DataIndexStart < DataIndexLimit ; DataIndexStart++)
            {
                if((DataIndexStart % 16) == 0)
                    printf("\r\n");

                if((DataIndexStart % 512) == 0)
                    printf("\r\nOffset:%ld\r\n", DataIndexStart);


                printf("%2x ", *(pucOrigData + DataIndexStart));

            }

            printf("Nand\n\n");
            for(DataIndexStart = DataIndexBase ; DataIndexStart < DataIndexLimit ; DataIndexStart++)
            {
                if((DataIndexStart % 16) == 0)
                    printf("\r\n");
                if((DataIndexStart % 512) == 0)
                    printf("\r\nOffset:%ld\r\n", DataIndexStart);

                printf("%2x ", *(pucCompareData + DataIndexStart));

            }
            printf("Data compare failed.....\n\n");
            return -1;
        }
    }
    return 0;
}

static int nand_sub_menu(NAND_UPDATE_COMMAND_TYPE ucCommand)
{
    int  cmd_ret;
    unsigned long ulFileSize = 0;
//    unsigned char *base;
#if (NVT_FLASH_UPDATE_IMAGE_FROM_USB == 1)
    char* FileSize_str;
#endif
	unsigned char ucCMDIndex;
    char select_opt;
    char cmd[256];

    do
    {
        if (ucCommand == NVT_FLASH_UPDATE_SHOW_MENU)
        {
            printf("*******    Novatek Flash update Menu   *******\n");
            printf("* %c.    Update onboot                        *\n", NVT_FLASH_UPDATE_ONBOOT);
            printf("* %c.    Update U-boot                        *\n", NVT_FLASH_UPDATE_UBOOT);
            printf("* %c.    Update DeviceTree(.dtb)              *\n", NVT_FLASH_UPDATE_DT);
            printf("* %c.    Update Kernel                        *\n", NVT_FLASH_UPDATE_KERNEL);
            printf("* %c.    Update Rootfs                        *\n", NVT_FLASH_UPDATE_ROOTFS);
			printf("* %c.    Exit                                 *\n", NVT_FLASH_UPDATE_EXIT);
            printf("**********************************************\n");
            printf("Please select one option?");
            select_opt = serial_getc();
        }
        else
        {
            select_opt = ucCommand;
        }

        switch (select_opt)
        {
            case NVT_FLASH_UPDATE_UBOOT:
			case NVT_FLASH_UPDATE_ONBOOT:
			case NVT_FLASH_UPDATE_DT:
			case NVT_FLASH_UPDATE_KERNEL:
			case NVT_FLASH_UPDATE_ROOTFS:
				
				ucCMDIndex = select_opt - '0';
				if((ucCMDIndex < 0))
				{
					printf("command error\r\n");
					return -1;
				}

				printf("Command type:%c(%d)\r\n", select_opt, ucCMDIndex);
#if (NVT_FLASH_UPDATE_IMAGE_FROM_USB == 1)
                memset(cmd, '\0', 256);
                memset((unsigned char *)NVT_update_parameter_table[ucCMDIndex].ulSourceBuffer, 0xFF, NVT_update_parameter_table[ucCMDIndex].ulSourceLen);

				sprintf(cmd, "fatload usb 0 0x%lx %s", NVT_update_parameter_table[ucCMDIndex].ulSourceBuffer, NVT_update_parameter_table[ucCMDIndex].ucImageFileName);

//				sprintf(cmd, "fatload usb 0 0x%x %s 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF,
//						 img_name, NVT_FWUPDATE_MAX_WRITE_BYTES, (read_blks << BYTE_TO_SECTOR_SFT));
				//printf("ewrite fatcmd: %s\n", cmd);
//				ret = run_command(cmd, 0);


				printf("Command:%s\r\n", cmd);
                cmd_ret = run_command (cmd, 0);
                if(cmd_ret != 0) {
                    printf("load %s from usb fail !\n", NVT_update_parameter_table[ucCMDIndex].ucImageFileName);
                    goto err_out;
                }

                FileSize_str = getenv("filesize");
                if (FileSize_str != NULL)
                {
                    ulFileSize = simple_strtoul (FileSize_str, NULL, 16);
                }
                if(ulFileSize == 0)
                {
                    printf("file size is zero\n");
                    goto err_out;
                }	
#else
				ulFileSize = NVT_update_parameter_table[ucCMDIndex].ulSourceLen;
#endif

                memset(cmd, '\0', 256);
                sprintf(cmd, "nvt_nand erase 0x%lx 0x%lx", NVT_update_parameter_table[ucCMDIndex].ulFlashAddr, NVT_update_parameter_table[ucCMDIndex].ulFlashSpace);
                cmd_ret = run_command (cmd, 0);
                if(cmd_ret != 0) {
                    printf("erase failed\n");
                    goto err_out;
                }

                memset(cmd, '\0', 256);
                sprintf(cmd, "nvt_nand write 0x%lx 0x%lx 0x%lx 0x%lx", NVT_update_parameter_table[ucCMDIndex].ulSourceBuffer, NVT_update_parameter_table[ucCMDIndex].ulFlashAddr, ulFileSize, NVT_update_parameter_table[ucCMDIndex].ulFlashSpace);
                cmd_ret = run_command (cmd, 0);
                if(cmd_ret != 0) {
                    printf("Program failed\n");
                    goto err_out;
                }
/*
				if(nand_dataCompare((unsigned char*)NVT_update_parameter_table[ucCMDIndex].ulSourceBuffer, (unsigned char*)NVT_FLASH_UPDATE_VERIFY_BUFFER_ADDR, NVT_update_parameter_table[ucCMDIndex].ulFlashAddr, ulFileSize, NVT_update_parameter_table[ucCMDIndex].ulFlashSpace) != 0)
				{
					printf("Data are mismatch and update procedure is failure.\r\n");
				}
                else
*/
					printf("Flash Update Completed....\n\n");

                break;

            case NVT_FLASH_UPDATE_EXIT:
                printf("\n\n");
                return 0;
                break;

            default:
                printf("Unknow command type:0x%x", ucCommand);
                break;
        }
    } while(ucCommand == NVT_FLASH_UPDATE_SHOW_MENU);

err_out:
    return -1;
}

static int nand_updateAll(void)
{
    nand_sub_menu(NVT_FLASH_UPDATE_ONBOOT);
    nand_sub_menu(NVT_FLASH_UPDATE_UBOOT);
    nand_sub_menu(NVT_FLASH_UPDATE_DT);
    nand_sub_menu(NVT_FLASH_UPDATE_KERNEL);
    nand_sub_menu(NVT_FLASH_UPDATE_ROOTFS);
    return 0;
}

int do_update_menu (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char select_opt;
	int ret = 0;
    char cmd[256];

	printf("argc:%d arg0:%s  arg1:%s\r\n", argc, argv[0], argv[1]);

    if (argc > 2)
        goto usage;

#if (NVT_FLASH_UPDATE_IMAGE_FROM_USB == 1)
    memset(cmd, '\0', 256);
    sprintf(cmd, "usb start");
    ret = run_command (cmd, 0);
	if(ret != 0) 
	{
		printf("%s init usb fail !\n",__func__);
		ret = -EIO;
		goto ERROR_EXIT;
	}
#endif

    if (argc == 2)
    {
		if (strcmp (argv[1], "nand") == 0)
        {
            while(1)
            {
                printf("***    Novatek Flash Update Main Menu   ***\n");
                printf("* 1. Update All                           *\n");
                printf("* 2. Update Partial                       *\n");
				printf("* %c. Erase All block			  		   *\n", NVT_FLASH_UPDATE_ERASE_ALL);
                printf("* 0. Exit                                 *\n");
                printf("*******************************************\n");
                printf("Please select one option?");
                select_opt = serial_getc();
                printf(" %c\n", select_opt);
                switch (select_opt)
                {
                    case '1':
                        ret = nand_updateAll();
                        break;
                    case '2':
                        printf("\n\n");
                        ret = nand_sub_menu(NVT_FLASH_UPDATE_SHOW_MENU);
                        break;
		            case NVT_FLASH_UPDATE_ERASE_ALL:
		                memset(cmd, '\0', 256);
		                sprintf(cmd, "nvt_nand erase 0x%x 0x%x", 0, 0xFFFFFFFF);
		                ret = run_command (cmd, 0);
		                if(ret != 0)
		                {
		                    printf("erase failed\n");
		                }
		                break;						

					case '0':
                        return 0;
                }
            }
        }
    }

usage:
    printf("usage: update <device(nand)>\n");

ERROR_EXIT:	
    return ret;
}

U_BOOT_CMD(
    update, 2, 0,   do_update_menu,
    "show the update menu",
    NULL
);


