#ifndef _PARTITIONINFO_H
#define _PARTITIONINFO_H

/**
    Partition Infomation

    this structure should be addressing to fixed on address '0x80000700' ~
    '0x80000800'

    This is common header used between firmware of uITRON, eCos, Linux, DSP
    so !!!!!! DO NOT modify it !!!!!!
*/

#define EMB_PARTITION_INFO_OFFSET 0x00000700
#define EMB_PARTITION_INFO_COUNT  16

#define EMBTYPE_UNKNOWN 0x00
#define EMBTYPE_UITRON  0x01
#define EMBTYPE_ECOS    0x02
#define EMBTYPE_UBOOT   0x03
#define EMBTYPE_LINUX   0x04
#define EMBTYPE_DSP     0x05
#define EMBTYPE_PSTORE  0x06
#define EMBTYPE_FAT     0x07
#define EMBTYPE_EXFAT   0x08
#define EMBTYPE_UBIFS   0x09
#define EMBTYPE_RAMFS   0x0A
#define EMBTYPE_UENV    0x0B /* u-boot environment data */

/* for reason of compatiable linux, we use original type to decalre */

typedef struct _EMB_PARTITION{
    unsigned short  EmbType;         /* EMBTYPE_ */
    unsigned short  OrderIdx;        /* Order index of the same EmbType based on '0' */
    unsigned int    PartitionOffset; /* Phyical offset of partition */
    unsigned int    PartitionSize;   /* Size of this partition */
    unsigned int    ReversedSize;    /* Reserved size for bad block */
}EMB_PARTITION, *PEMB_PARTITION;

#endif
