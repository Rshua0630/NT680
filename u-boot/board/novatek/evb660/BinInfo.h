/**
    Bin File Information

    Bin file layout

    -----------------------------------------------------

     BININFO: 0x00 ~ 0x180 = 384 bytes = 96 WORDS          (see BinInfo_MIPS.c)

     including:

        HEADINFO  0x00 ~ 0x80 = 128 bytes = 32 WORDS
        LDINFO    0x80 ~ 0xc0 = 64 bytes = 16 WORDS
        DRAMINFO  0xc0 ~ 0x100 = 64 bytes = 16 WORDS
        STRGINFO  0x100 ~ 0x140 = 64 bytes = 16 WORDS
        FWINFO    0x140 ~ 0x180 = 64 bytes = 16 WORDS

    -----------------------------------------------------

     EXCEPTION_TABLE: 0x180 ~ 0x200 = 128 bytes = 32 WORDS (see exception_MIPS.s)

    -----------------------------------------------------

     VECTOR_TABLE: 0x200 ~ 0x2f0 = 240 bytes = 60 WORDS    (see isr_MIPS.s)

    -----------------------------------------------------

     Code Info: 0x2f0 ~ 0x400 = 272 bytes = 68 WORDS       (see CodeInfo_MIPS.s)

    -----------------------------------------------------

     Code Entry: 0x400                                     (see Loader_MIPS.s)

        :

       PART-1
       PART-2
       PART-3
         :
       PART-N

    -----------------------------------------------------

    @file       BinInfo.h
    @ingroup    mGXSYSTEM
    @note       THESE STRUCTS ARE VERY VERY IMPORTANT FORMAT DEFINITION OF SYSTEM,
                DO NOT MODIFY ANY ITEM OR INSERT/REMOVE ANY ITEM OF THESE STRUCTS!!!

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _BININFO_H
#define _BININFO_H

#define BIN_VER             0x00012100 ///< BinInfo v1.21

#define BIN_INFO_OFFSET_UITRON 0x00000000 ///< uITRON BinInfo.o attached to offset '0'
#define BIN_INFO_OFFSET_ECOS   0x00000400 ///< eCos BinInfo.o attached to offset '0x400'
#define BIN_INFO_OFFSET_UBOOT  0x00000400 ///< uboot BinInfo.o attached to offset '0x400'

//Ep: Encryption Program (CheckSum version)
//Epcrc: Encryption Program (CRC version)
//Bfc: Bin File Compress
//Ld: Loader
//Fw: Firmware

//Head control flag for HEADINFO.BinCtrl
#define HDCF_LZCOMPRESS_EN  0x0000001 ///< BIT 0.compressed enable (0=no,1=yes)

/**
     @name Ld control flag for LDINFO.LdCtrl
*/
//@{
#define LDCF_PARTLOAD_EN    0x0000001 ///< BIT 0.PARTLOAD_EN (0=no,1=yes)
//@}

/**
     @name Ld control flag for LDINFO.LdCtrl2
*/
//@{
#define LDCF_UPDATE_FW      0x0000001 ///< BIT 0.UPDATE_FW (0=no,1=yes)
#define LDCF_UPDATE_LD      0x0000002 ///< BIT 1.UPDATE_LD (0=no,1=yes)
#define LDCF_BOOT_CARD      0x0000004 ///< BIT 2.BOOT_CARD (0=no,1=yes)
#define LDCF_BOOT_FLASH     0x0000008 ///< BIT 3.BOOT_FLASH (0=no,1=yes)
#define LDCF_UPDATE_CAL     0x0000010 ///< BIT 4.UPDATE_CAL (0=no,1=yes)
#define LDCF_UPDATE_FW_DONE 0x0000100 ///< BIT 8.UPDATE_FW_DONE (0=no,1=yes)
//@}

/**
     Header information

     0x00 ~ 0x80 = 128 bytes = 32 WORDS
*/
typedef struct HEADINFO
{
    UINT32 CodeEntry;   ///< [0x00] fw CODE entry (4) ----- r by Ld
    UINT32 Resv1[19];   ///< [0x04~0x50] reserved (4*19) -- reserved, its mem value will filled by Ld
    char BinInfo_1[8];  ///< [0x50~0x58] CHIP-NAME (8) ---- r by Ep
    char BinInfo_2[8];  ///< [0x58~0x60] SDK version (8)
    char BinInfo_3[8];  ///< [0x60~0x68] SDK releasedate (8)
    UINT32 BinLength;   ///< [0x68] Bin File Length (4) --- w by Ep/bfc
    UINT32 Checksum;    ///< [0x6c] Check Sum or CRC (4) ----- w by Ep/Epcrc
    UINT32 CRCLenCheck; ///< [0x70~0x74] Length check for CRC (4) ----- w by Epcrc (total len ^ 0xAA55)
    UINT32 Resv2;       ///< [0x74~0x78] reserved (4) --- reserved for other bin tools
    UINT32 BinCtrl;     ///< [0x78~0x7C] Bin flag (4) --- w by bfc
                        ///<             BIT 0.compressed enable (w by bfc)
    UINT32 CRCBinaryTag;///< [0x7C~0x80] Binary Tag for CRC (4) ----- w by Epcrc (0xAA55 + "NT")
}
HEADINFO;

STATIC_ASSERT(sizeof(HEADINFO) == 128);

/**
     Loader information

     0x80 ~ 0xc0 = 64 bytes = 16 WORDS
*/
typedef struct _LDINFO
{
    char LdInfo_1[16];  ///< [0x80~0x90] LD-NAME(16) ------ w by Ld
    UINT32 LdCtrl;      ///< [0x90] Fw flag (4) ----------- r by Ld
                        ///<        BIT 0.enable part-load (0=full load,1=part load)
    UINT32 LdCtrl2;     ///< [0x94] Ld flag (4) ----------- w by Ld
                        ///<        BIT 0.UPDATE_FW (0=no,1=yes)
                        ///<        BIT 1.UPDATE_LOADER (0=no,1=yes)
                        ///<        BIT 2.BOOT_CARD (0=no,1=yes)
                        ///<        BIT 3.BOOT_FLASH (0=no,1=yes)
                        ///<        BIT 4.UPDATE_CAL (0=no,1=yes)
                        ///<        BIT 8.UPDATE_FW_DONE (0=no,1=yes)
    UINT32 LdLoadSize;  ///< [0x98] Ld load size (4) ------ w by Ld (NOTE: this value must be block size align)
    UINT32 LdLoadTime;  ///< [0x9c] Ld exec time(us) (4) -- w by Ld
    UINT32 LdResvSize;  ///< [0xa0] Ld size (by bytes, reserved size in partition) (4) ------ w by Ld
    UINT32 FWResvSize;  ///< [0xa4] FW reserved size (4) ------ w by Ld
    UINT16 LdPackage;   ///< [0xa8] IC package expected by Ld (0xFF: ES, 0: 660, 3: 663, 5: 665, etc...)
    UINT16 LdStorage;   ///< [0xaa] Internal storage expected by Ld (0: unkown, 1: nand, 2: spi nand, 3: spi nor)
    UINT32 Resv[5];     ///< [0xac~0xc0] (4*5) ------------ reserved for project Ld
}
LDINFO;

STATIC_ASSERT(sizeof(LDINFO) == 64);

/**
     DRAM information

     0xc0 ~ 0x100 = 64 bytes = 16 WORDS
*/
typedef struct _DRAMINFO
{
    char DramInfo_1[16];///< [0xc0~0xd0] DRAMINFO (16)
    UINT32 DramCtrl;    ///< [0xd0] dram ctrl (4)
    UINT32 DramCtrl2;   ///< [0xd4] dram ctrl2 (4)
    UINT32 DramInfo[10];///< [0xd8~0x100] dram partition (4*10)
                        ///< [0xd8] TOTAL offset (4)
                        ///< [0xdc] TOTAL size (4)
                        ///< [0xe0] CODE offset (4)
                        ///< [0xe4] CODE size (4)
                        ///< [0xe8] STACK offset (4)
                        ///< [0xec] STACK size (4)
                        ///< [0xf0] HEAP offset (4)
                        ///< [0xf4] HEAP size (4)
                        ///< [0xf8] RESV offset (4)
                        ///< [0xfc] RESV size (4)
}
DRAMINFO;

STATIC_ASSERT(sizeof(DRAMINFO) == 64);

/**
     Storage information

     0x100 ~ 0x140 = 64 bytes = 16 WORDS
*/
typedef struct _STRGINFO
{
    char StrgInfo_1[16];///< [0x100~0x110] STRGINFO (16)
    UINT32 StrgBlkSz;   ///< [0x110] nand block size (4) ------ w by Ld (project Ld)
    UINT32 Resv2;       ///< [0x114] (4) ---------- reserved for project nand
    UINT32 StrgInfo[10];///< [0x118~0x130] nand partition (4*10)
                        ///< [0x118] UITRON offset (4) (must be 0)
                        ///< [0x11c] UITRON size (4)(w by FW, but will compare to LDINFO.FWResvSize)
                        ///< [0x120] ECOS / UBOOT offset (4)
                        ///< [0x124] ECOS / UBOOT size (4)
                        ///< [0x128] LINUX offset (4)
                        ///< [0x12c] LINUX size (4)
                        ///< [0x130] ROOTFS offset (4)
                        ///< [0x134] ROOTFS size (4)
                        ///< [0x138] DSP offset (4)
                        ///< [0x13c] DSP size (4)
}
STRGINFO;

STATIC_ASSERT(sizeof(STRGINFO) == 64);

/**
     Firmware information

     0x140 ~ 0x180 = 64 bytes = 16 WORDS
*/
typedef struct _FWINFO
{
    char FwInfo_1[16];  ///< [0x140~0x150] PRJ-NAME (16)
    UINT32 Resv[12];    ///< [0x150~0x180] (4*12) --------- reserved for project Fw
}
FWINFO;

STATIC_ASSERT(sizeof(FWINFO) == 64);

/**
     Binary file information

     0x00 ~ 0x180 = 384 bytes = 96 WORDS
*/
typedef struct _BININFO
{
    HEADINFO head;      ///< 0x00 ~ 0x80 = 128 bytes = 32 WORDS, header information
    LDINFO ld;          ///< 0x80 ~ 0xc0 = 64 bytes = 16 WORDS, loader information
    DRAMINFO dram;      ///< 0xc0 ~ 0x100 = 64 bytes = 16 WORDS, DRAM information
    STRGINFO strg;      ///< 0x100 ~ 0x140 = 64 bytes = 16 WORDS, storage information
    FWINFO fw;          ///< 0x140 ~ 0x180 = 64 bytes = 16 WORDS, firmware information
}
BININFO;

STATIC_ASSERT(sizeof(BININFO) == 384);

///////////////////////////////////////////////////////////////////////////////
//Code Info: 0x2f0 ~ 0x400

#define ZI_SECTION_OFFSET   (0x2f0+0x10) ///< ZI area information of starting offset
#define CODE_SECTION_OFFSET (0x2f0+0x18) ///< code section information of starting offset

///////////////////////////////////////////////////////////////////////////////
//Code Entry: 0x400 ~
#define CODE_ENTRY_OFFSET   (0x400)  ///< code entry starting offset

/*
bfc syntex

bfc.exe p1 p2 p3 p4 p5 p6 p7 p8 p9

p1: compress/decompress
p2: compress method
p3: input file name
p4: output file name
p5: partial load flag ('1' means partial load)
p6: partial load file start [locate offset of bin file]
p7: output binary file length [locate offset of bin file]
p8: partial compress flag [locate offset of bin file]
p9: NAND block size (option)

NOTE: p6~p9 must be hex format (0x****)

@$(BFC) c lz $(BIN_R) tmp 1 0x310 0x68 0x78 $(EMBMEM_BLK_SIZE)
*/

#endif
