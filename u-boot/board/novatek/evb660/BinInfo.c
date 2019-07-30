/*;   Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
 *;
 *;   @file       BinInfo.c
 *;   @ingroup
 *;
 *;   @brief      Binary file information
 *;               Binary file information
 *;
 *;   @version    V1.20.000
 *;   @author     Novatek
 *;   @date       2012/07/19
 */
#include "nvt_type.h"
#include "BinInfo.h"

extern char uboot_bin_size[];
extern char CONFIG_SYS_TEXT_BASE[];

extern BININFO gBinInfo __attribute__ ((section (".data.bininfo")));
BININFO gBinInfo =
{
    //HEADINFO
    {
    (UINT32)CONFIG_SYS_TEXT_BASE, //<- fw CODE entry (4)
    {0}, ///<- reserved (4*19)
    "NT96660 ", //<- CHIP-NAME (8)
    "10000000", //<- version (8)
    "20100701", //<- releasedate (8)
    (UINT32)uboot_bin_size, //<- Bin File Length (4)
    0xffffffff, //<- Check Sum or CRC (4)
    0,///<- Length check for CRC (4)
    0,///<- reserved (4)
    0,///<- Bin flag (4)
    0,///<- Binary Tag for CRC (4)
    },
    //LDINFO
    {
    "                ", ///<- LD-NAME (16)
    0x00000000, ///<- Fw flag (4)
    0xffffffff, ///<- Ld flag (4)
    0xffffffff, ///<- Ld load size (4)
    0, ///<- Ld exec time (4)
    0, ///<- Ld size (n blocks) (4)
    0, ///<- FW reserved size (4)
    0, ///< LdPackage(2)
    0, ///< LdStorage(2)
    {0} ///<- (4*5) ---------- reserved 5 WORDs for project Ld
    },
    //DRAMINFO
    {
    "DRAMINFO        ", //DRAMINFO (16)
    0xffffffff, //dram ctrl (4)
    0xffffffff, //dram ctrl2 (4)
    {
    0, 0, //Total
    0, 0, //Code Region
    0, 0, //Stack Region
    0, 0, //Heap Region
    0, 0, //Reserved Region
    }
    },
    //STRGINFO
    {
    "STRGINFO        ", //STRGINFO (16)
    0, //nand block size (4)
    0, //Resv2 (4)
    {
    0x00000000,0x00000000, //uITRON Region
    0x00000000,0x00000000, //eCos / uBoot Region
    0x00000000,0x00000000, //Linux Region
    0x00000000,0x00000000, //RootFs Region
    0x00000000,0x00000000, //DSP Region
    },
    },
    //FWINFO
    {
    "UBOOT_001_001   ", //PRJ-NAME (16)
    {0} ///<- (4*16)  --------- reserved for project Fw
    }
};

extern char gBinEnd[4] __attribute__ ((section (".BinEnd")));
char gBinEnd[4] =
{'_','E','N','D'};

