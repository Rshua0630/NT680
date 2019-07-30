/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 */


#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <asm/byteorder.h>
#include <nand.h>
#include <part.h>
//#include <asm/hardware.h>
//#include <asm/arch/NT78320.h>
#define NAND_SPARE_SIZE_IN_A_BLOCK 64 * 4 * 16
#define NAND_SPARE_SIZE_IN_A_PAGE  4 * 16
#define NAND_SPARE_MARK            0x4D4EFF

#define NAND_PARAMETER_BLOCK 2
#define NAND_CMD_DEBUG       1

#if NAND_CMD_DEBUG
#define NandDBG(fmt,args...)    printf (fmt ,##args)
#else
#define NandDBG(fmt,args...)
#endif

unsigned long ulSpareValue[NAND_SPARE_SIZE_IN_A_BLOCK / 4];

u32 nand_get_bad_block_size(u32 offset, u32 length)
{
	nand_info_t *nand;
	u32 offend, i ,bbcnt;

	nand = &nand_info[nand_curr_device];

	if ((offset + length) > nand->size)
		offend = nand->size;
	else
		offend = offset+length;

	bbcnt = 0;

	for(i=offset; i < offend;i += nand->erasesize)
		if (nand_block_isbad(nand, i))
			bbcnt++;

	return (bbcnt * nand->erasesize);
}


int do_nand_flash_read(int argc, char * const argv[])
{
    nand_info_t* info = &nand_info[nand_curr_device];
//    void *buf;
    unsigned long ulDesAddr = 0xFFFFFFFF;
    unsigned int uiOffset = 0xFFFFFFFF;
    unsigned int uiLen;
    unsigned int uiLimitSpace;
//    unsigned long ulBlockIndexByByte = 0;
    unsigned long ulBlockSize = info->erasesize;
//    unsigned long ulRealLen;
    unsigned int  uiRealDataLen;
    int           ret;
    char*         endp;

    debug("%s\r\n", __func__);

    if(argc != 5)   {
        printf("Usage: nandtest offset(hex) length(hex) rounds(dec)\n");
        goto ParaError;
    }

    ulDesAddr = simple_strtoul(argv[1], &endp, 16);
    if (*argv[1] == 0 || *endp != 0)
        goto ParaError;
    uiOffset = simple_strtoul(argv[2], &endp, 16);
    if (*argv[2] == 0 || *endp != 0)
        goto ParaError;
    uiLen = simple_strtoul(argv[3], &endp, 16);
    if (*argv[3] == 0 || *endp != 0)
        goto ParaError;
    uiLimitSpace = simple_strtoul(argv[4], &endp, 16);
    if (*argv[4] == 0 || *endp != 0)
        goto ParaError;

    if((uiOffset % ulBlockSize) != 0)
    {
        goto ParaError;
    }

    uiRealDataLen = (uiLen / ulBlockSize) * ulBlockSize;
    if((uiLen % ulBlockSize) > 0)
        uiRealDataLen += ulBlockSize;

//    printf("ulDesAddr:0x%x ulOffset:0x%x ulLen:0x%x\r\n", ulDesAddr, ulOffset, ulLen);

//    ulLen = KERNEL_NAND_LEN;

    debug("ulDesAddr:0x%lx ulOffset:0x%x ulLen:0x%x limit:0x%x\r\n", ulDesAddr, uiOffset, uiRealDataLen, uiLimitSpace);

//    nand_select_read_function(info, NAND_ECC_BCH_512_16);
//    nand_select_write_function(info, NAND_ECC_BCH_512_16);

    ret = nand_read_skip_bad(info, uiOffset, &uiRealDataLen, NULL, uiLimitSpace, (unsigned char*)ulDesAddr);
    if (ret != 0) {
        printf("NAND flash %s failed\n", argv[0]);
        goto NANDError;
    }


    //Dump data
#if 0
    {
        unsigned long DataIndex = 0;

        for(DataIndex = 0 ; DataIndex < ulLen; DataIndex++)
        {
            if((DataIndex % 16) == 0)
                printf("\r\n");

            printf("%2x ", *((unsigned char*)ulDesAddr + DataIndex));

        }

    }
#endif

    debug("done\n");
    return 0;

ParaError:
    printf("parameter error\r\n");
    printf("ulSourceAddr:0x%lx ulOffset:0x%x ulLen:0x%x\r\n", ulDesAddr, uiOffset, uiRealDataLen);

NANDError:
    return 1;

}

int do_nand_flash_write(int argc, char * const argv[])
{
    nand_info_t* info = &nand_info[nand_curr_device];
    unsigned long ulSourceAddr = 0;
    unsigned int  uiOffset = 0;
    unsigned int  uiLen = 0;
    unsigned int  uiRealDataLen;
    unsigned int  uiBufferIndex;
    unsigned int  uiFreeSpace = 0;
    unsigned long ulBlockIndexByByte = 0;
    unsigned long ulBlockSize = info->erasesize;
    unsigned int  uiRealLen;
	size_t        Actual;

    char *endp;

    debug("%s\r\n", __func__);

    if(argc != 5)   {
        printf("Usage: nandtest offset(hex) length(hex) rounds(dec)\n");
        goto ParaError;
    }

    if(ulBlockSize == 0)
    {
        printf("Block size is zero\n");
        goto ParaError;
    }

    ulSourceAddr = simple_strtoul(argv[1], &endp, 16);
    if (*argv[1] == 0 || *endp != 0)
        goto ParaError;
    uiOffset = simple_strtoul(argv[2], &endp, 16);
    if (*argv[2] == 0 || *endp != 0)
        goto ParaError;
    uiLen = simple_strtoul(argv[3], &endp, 16);
    if (*argv[3] == 0 || *endp != 0)
        goto ParaError;
    uiFreeSpace = simple_strtoul(argv[4], &endp, 16);
    if (*argv[4] == 0 || *endp != 0)
        goto ParaError;

    if((uiOffset % ulBlockSize) != 0)
    {
        goto ParaError;
    }

    if((uiFreeSpace % ulBlockSize) != 0)
    {
        goto ParaError;
    }

    uiRealDataLen = (uiLen / ulBlockSize) * ulBlockSize;
    if((uiLen % ulBlockSize) > 0)
        uiRealDataLen += ulBlockSize;

    NandDBG("ulSourceAddr:0x%lx ulOffset:%d ulLen:%d uiRealLen:%d ulFreeSpace:%d\r\n", ulSourceAddr, uiOffset, uiLen, uiRealDataLen, uiFreeSpace);

    //Dump data

#if 0
    {
        unsigned long DataIndex = 0;

        for(DataIndex = 0 ; DataIndex < ulLen; DataIndex++)
        {
            if((DataIndex % 16) == 0)
                printf("\r\n");

            printf("%2x ", *((unsigned char*)ulSourceAddr + DataIndex));

        }

    }
#endif

    for(ulBlockIndexByByte = uiOffset, uiBufferIndex = 0 ; uiBufferIndex < uiRealDataLen ; ulBlockIndexByByte += ulBlockSize, ulSourceAddr += ulBlockSize, uiBufferIndex += ulBlockSize)
    {
        debug("Write data to NAND flash 0x%lx\r\n", ulBlockIndexByByte);

        if((ulBlockIndexByByte - uiOffset) >= uiFreeSpace)
        {
            printf("No more free space to retain data, Maybe the NAND flash is damaged\n");
            while(1);
        }

        uiRealLen = ulBlockSize;

        if(nand_write_skip_bad(info, ulBlockIndexByByte, &uiRealLen, &Actual, ulBlockSize, (unsigned char*)ulSourceAddr, 0) != 0)
        {
            printf("write data to nand fail !\n");
            goto NANDError;
        }



        while(nand_get_bad_block_size(ulBlockIndexByByte, info->erasesize))
        {
            ulBlockIndexByByte += nand_get_bad_block_size(ulBlockIndexByByte, info->erasesize);
        }

        printf(".");
    }

    printf("done\n");

    return 0;

ParaError:
    printf("parameter error\r\n");
    debug("ulAddr:%lx ulOffset:%x ulLen:%x ulFreeSpace:%x\r\n", ulSourceAddr, uiOffset, uiLen, uiFreeSpace);

NANDError:
    return 1;
}

int do_nand_flash_erase(int argc, char * const argv[])
{
    nand_info_t * info = &nand_info[nand_curr_device];
//    size_t len;
    nand_erase_options_t nand_erase_options;

    unsigned long ulOffset;
    unsigned long ulLen;
    unsigned long ulBlockIndex;
    unsigned long ulBlockSize = info->erasesize;
    unsigned long ulTotalLen;

    char *endp;

    debug("%s\r\n", __func__);

    if (argc < 3)
        goto usage;


    ulOffset = simple_strtoul(argv[1], &endp, 16);

    if (*argv[1] == 0 || *endp != 0)
        goto usage;

    ulLen = simple_strtoul(argv[2], &endp, 16);

    if (*argv[2] == 0 || *endp != 0)
        goto usage;

    debug("Nand flash %s command: offset:0x%lx len:0x%lx\n", argv[0], ulOffset, ulLen);

    if((ulOffset % ulBlockSize) != 0)
    {
        debug("Offset can't divide by block size\r\n");
        goto usage;
    }

    debug("1:%ld 2:%ld\n", ulLen / ulBlockSize, ulLen % ulBlockSize);

    if((ulLen > info->size) || ((ulLen + ulOffset) > info->size))
    {
        ulTotalLen = info->size;
    }
    else
        ulTotalLen = ulOffset + ulLen;

    if((ulTotalLen % ulBlockSize) != 0)
    {
        printf("Len can't divide by block size\r\n");
        goto usage;
    }

    debug("BlockStart:%ld NumBlock:%ld Blocksize:%ld\n", ulOffset / ulBlockSize, ulLen / ulBlockSize, ulBlockSize);

    for(ulBlockIndex = ulOffset ; ulBlockIndex < ulTotalLen ; ulBlockIndex += ulBlockSize)
    {
        NandDBG("\rBlock %ld erasing... \n", ulBlockIndex / ulBlockSize);
        nand_erase_options.jffs2 = 0;
        nand_erase_options.length = ulBlockSize;
        nand_erase_options.offset = (ulong)ulBlockIndex;
        nand_erase_options.quiet = 1;
        nand_erase_options.scrub = 0;
        if(nand_erase_opts(info, &nand_erase_options))
        {
            printf("erase fail, mark bad........\n");
            mtd_block_markbad(info, ulBlockIndex);
  //          return 1;
        }
    }

    printf("done\n");

    return 0;

usage:
    printf("Nand erase error\n");
    return 1;
}

int do_nand_flash(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    const char *cmd;
    cmd = argv[1];


#if NAND_CMD_DEBUG
    int        iComIndex;
    for(iComIndex = 0 ; iComIndex < argc ; iComIndex++)
    {
        printf("Argc:%d-%s\r\n", iComIndex, argv[iComIndex]);
    }
#endif
    NandDBG("%s\r\n", __func__);

    if (strcmp(cmd, "write") == 0)
    {
        /* need at least two arguments */
        if (argc < 6)
            goto usage;
        return do_nand_flash_write(argc - 1, argv + 1);
    }

    if (strcmp(cmd, "read") == 0)
    {
        /* need at least two arguments */
        if (argc < 6)
            goto usage;
        return do_nand_flash_read(argc - 1, argv + 1);
    }

    if (strcmp(cmd, "erase") == 0)
    {
        /* need at least two arguments */
        if (argc < 4)
            goto usage;
        return do_nand_flash_erase(argc - 1, argv + 1);
    }

usage:
    cmd_usage(cmdtp);
    return 1;
}

U_BOOT_CMD(
    nvt_nand, 6,  1,  do_nand_flash,
    "NAND flash sub-system",
	"nvt_nand read addr offset len  totalFreeSpace	- read `len' bytes starting at `offset' to memory at `addr'\n"
	"                                               - totalFreeSpare:How many free blocks(include replaced block) used for the read operation'\n"
	"nvt_nand write addr offset len totalFreeSpace	- write `len' bytes from memory at `addr' to flash at `offset\n"
	"                                               - totalFreeSpare:How many free blocks(include replaced block) used for the write operation'\n"
	"nvt_nand erase offset [+]len		- erase `len' bytes from `offset'\n"
	"				  `+len' round up `len' to block size\n");
