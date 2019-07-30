#ifndef _EMB_PARTITION_INFO_H
#define _EMB_PARTITION_INFO_H

#define EMB_PARTITION_INFO_VER 0x16072117 ///< YYYY/MM/DD HH

/**
    Partition Infomation
    This is common header used between firmware of uITRON, eCos, Linux, DSP
    so !!!!!! DO NOT modify it !!!!!!
*/

#define EMB_PARTITION_INFO_COUNT  16

#define EMBTYPE_UNKNOWN     0x00
#define EMBTYPE_LOADER      0x01  /* loader must always put in partition[0] */
#define EMBTYPE_MODELEXT    0x02  /* modelext must always put in partition[1] */
#define EMBTYPE_UITRON      0x03
#define EMBTYPE_ECOS        0x04
#define EMBTYPE_UBOOT       0x05
#define EMBTYPE_LINUX       0x06
#define EMBTYPE_DSP         0x07
#define EMBTYPE_PSTORE      0x08
#define EMBTYPE_FAT         0x09
#define EMBTYPE_EXFAT       0x0A
#define EMBTYPE_ROOTFS      0x0B
#define EMBTYPE_RAMFS       0x0C
#define EMBTYPE_UENV        0x0D /* u-boot environment data */
#define EMBTYPE_MBR         0x0E /* for emmc partition, mbr always put in partition[0] instead of loader */

/**
    customer defined data partition format
*/
#define EMBTYPE_USER0       0x80
#define EMBTYPE_USER1       0x81
#define EMBTYPE_USER2       0x82
#define EMBTYPE_USER3       0x83
#define EMBTYPE_USER4       0x84
#define EMBTYPE_USER5       0x85
#define EMBTYPE_USER6       0x86
#define EMBTYPE_USER7       0x87


/* for reason of compatiable linux, we use original type to decalre */

typedef struct _EMB_PARTITION {
	unsigned short  EmbType;         /* EMBTYPE_ */
	unsigned short  OrderIdx;        /* Order index of the same EmbType based on '0' */
	unsigned int    PartitionOffset; /* Phyical offset of partition */
	unsigned int    PartitionSize;   /* Size of this partition */
	unsigned int    ReversedSize;    /* Reserved size for bad block */
} EMB_PARTITION, *PEMB_PARTITION;

#endif
