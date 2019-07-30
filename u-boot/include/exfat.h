/*
 * R/O (V)FAT 12/16/32 filesystem implementation by Marcus Sundberg
 *
 * 2002-07-28 - rjones@nexus-tech.net - ported to ppcboot v1.1.6
 * 2003-03-10 - kharris@nexus-tech.net - ported to u-boot
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _EXFAT_H_
#define _EXFAT_H_

#include <asm/byteorder.h>

#define CONFIG_SUPPORT_VFAT
/* Maximum Long File Name length supported here is 128 UTF-16 code units */
#define EXFAT_MAXLEN_BYTES	256 /* Maximum LFN buffer in bytes */
#define VFAT_MAXSEQ		9   /* Up to 9 of 13 2-byte UTF-16 entries */
#define PREFETCH_BLOCKS		2

#ifndef CONFIG_FS_EXFAT_MAX_CLUSTSIZE
#define CONFIG_FS_EXFAT_MAX_CLUSTSIZE (256*1024)
#endif
#define MAX_EXCLUSTSIZE	CONFIG_FS_EXFAT_MAX_CLUSTSIZE

#define EXDIRENTSPERBLOCK	(mydata->sect_size / EXFAT_SIZE_PER_ENTRY)
#define EXDIRENTSPERCLUST	((mydata->clust_size * mydata->sect_size) / \
			 EXFAT_SIZE_PER_ENTRY)

#define FATBUFBLOCKS	6
#define FATBUFSIZE	(mydata->sect_size * FATBUFBLOCKS)
#define FAT12BUFSIZE	((FATBUFSIZE*2)/3)
#define FAT16BUFSIZE	(FATBUFSIZE/2)
#define FAT32BUFSIZE	(FATBUFSIZE/4)


/* Filesystem identifiers */
#define FAT12_SIGN	"FAT12   "
#define FAT16_SIGN	"FAT16   "
#define FAT32_SIGN	"FAT32   "
#define EXFAT_SIGN	"EXFAT   "
#define SIGNLEN		8

/* File attributes */
#define ATTR_RO	1
#define ATTR_HIDDEN	2
#define ATTR_SYS	4
#define ATTR_VOLUME	8
#define ATTR_DIR	16
#define ATTR_ARCH	32

#define ATTR_VFAT	(ATTR_RO | ATTR_HIDDEN | ATTR_SYS | ATTR_VOLUME)

#define DELETED_FLAG	((char)0xe5) /* Marks deleted files when in name[0] */
#define aRING		0x05	     /* Used as special character in name[0] */

/*
 * Indicates that the entry is the last long entry in a set of long
 * dir entries
 */
#define LAST_LONG_ENTRY_MASK	0x40

/* Flags telling whether we should read a file or list a directory */
#define LS_NO		0
#define LS_YES		1
#define LS_DIR		1
#define LS_ROOT		2

#define ISDIRDELIM(c)	((c) == '/' || (c) == '\\')

#define FSTYPE_NONE	(-1)

#if defined(__linux__) && defined(__KERNEL__)
#define FAT2CPU16	le16_to_cpu
#define FAT2CPU32	le32_to_cpu
#else
#if __LITTLE_ENDIAN
#define FAT2CPU16(x)	(x)
#define FAT2CPU32(x)	(x)
#else
#define FAT2CPU16(x)	((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8))
#define FAT2CPU32(x)	((((x) & 0x000000ff) << 24)  |	\
			 (((x) & 0x0000ff00) << 8)  |	\
			 (((x) & 0x00ff0000) >> 8)  |	\
			 (((x) & 0xff000000) >> 24))
#endif
#endif
/*
#define EXSTART(dent) (FAT2CPU32((dent)->stream_dentry.first_clus))

#define EXSIZE(dent) ((unsigned long long)FAT2CPU32((dent)->stream_dentry.valid_data_len[0]) \
					+ FAT2CPU32((unsigned long long)(dent)->stream_dentry.valid_data_len[1] << 32))
*/
#define EXSTART(strmptr) (FAT2CPU32((strmptr)->first_clus))

#define EXSIZE(strmptr) ((unsigned long long)FAT2CPU32((strmptr)->valid_data_len[0]) \
					+ FAT2CPU32((unsigned long long)(strmptr)->valid_data_len[1] << 32))

#define START(dent)	(FAT2CPU16((dent)->start) \
			+ (mydata->fatsize != 32 ? 0 : \
			  (FAT2CPU16((dent)->starthi) << 16)))
#define IS_LAST_CLUST(x, fatsize) ((x) >= ((fatsize) != 32 ? \
					((fatsize) != 16 ? 0xff8 : 0xfff8) : \
					0xffffff8))

#define IS_CONTIGUOUS(strmptr) ((strmptr)->flags & EXFAT_ENTRY_CONTIGUOUS ? 1 : 0)

#define CHECK_CLUST(x, fatsize) ((x) <= 1 || \
				(x) >= ((fatsize) != 32 ? \
					((fatsize) != 16 ? 0xff0 : 0xfff0) : \
					0xffffff0))
#define CHECK_EXCLUST(x, fatsize) ((x) <= 1 || (x) >= (0xffffff0))

typedef struct exfat_boot_sector {
	__u8	ignored[3];	/* Bootstrap code */
	char	file_system_name[8];	/* Name of fs */
	__u8	must_be_zero[53]; /* All 00h */
	__u32	partition_offset[2];
	__u32	volume_length[2];	/* Number of sectors (if sectors == 0) */
	__u32	fat_offset;
	__u32	fat_length;	/* Sectors/FAT */
	__u32	cluster_heap_offset;
	__u32	cluster_count;
	__u32	first_clus_of_root_dir;
	__u32	volume_serial_number;
	__u16	file_system_revision;
	__u16	volume_flags;
	__u8	byte_per_sector_shift;
	__u8	sector_per_clus_shift;
	__u8	number_of_fats;
} exfat_boot_sector;

typedef struct exfat_volume_info
{
	__u8 drive_number;	/* BIOS drive number */
	__u8 reserved;		/* Unused */
	__u8 ext_boot_sign;	/* 0x29 if fields below exist (DOS 3.3+) */
	__u8 volume_id[4];	/* Volume ID number */
	char volume_label[11];	/* Volume label */
	char fs_type[8];	/* Typically FAT12, FAT16, or FAT32 */
	/* Boot code comes next, all but 2 bytes to fill up sector */
	/* Boot sign comes last, 2 bytes */
} exfat_volume_info;

typedef struct exfat_dir_entry {
	char	name[8],ext[3];	/* Name and extension */
	__u8	attr;		/* Attribute bits */
	__u8	lcase;		/* Case for base and extension */
	__u8	ctime_ms;	/* Creation time, milliseconds */
	__u16	ctime;		/* Creation time */
	__u16	cdate;		/* Creation date */
	__u16	adate;		/* Last access date */
	__u16	starthi;	/* High 16 bits of cluster in FAT32 */
	__u16	time,date,start;/* Time, date and first cluster */
	__u32	size;		/* File size in bytes */
} exfat_dir_entry;

#define EXFAT_SIZE_PER_ENTRY		32
#define EXFAT_NAME_ENTRY_MAXCHAR	15 //max characters of each name entry (15)
#define EXFAT_NAME_ENTRY_MAXNUM		17 //the max number of name entries (17)
#define EXFAT_FILENAME_MAXLEN		EXFAT_NAME_ENTRY_MAXCHAR*EXFAT_NAME_ENTRY_MAXNUM //255

#define EXFAT_ENTRYTYPE_INUSE		0x80
#define EXFAT_ENTRY_CONTIGUOUS		0x2

#define EXFAT_ENTRY_TYPE_FILE		0x85
#define EXFAT_ENTRY_TYPE_STREAM		0xC0
#define EXFAT_ENTRY_TYPE_NAME		0xC1

typedef struct {
	__u8	entry_type;
	__u8	second_count;
	__u16	check_sum;
	__u16	attrib;
	__u16	reserved1;
	__u16	create_time;
	__u16	create_date;
	__u16	modify_time;
	__u16	modify_date;
	__u16	access_time;
	__u16	access_date;
	__u8	create_time_10ms;
	__u8	modify_time_10ms;
	__u8	create_utc_offset;
	__u8	modify_utc_offset;
	__u8	access_utc_offset;
	__u8	reserved2[7];
} exfat_file_dentry;

/* MS-DOS EXFAT stream extension directory entry (32 bytes) */
typedef struct {
	__u8	entry_type;
	__u8	flags;
	__u8	reserved1;
	__u8	name_len;
	__u16	name_hash;
	__u16	reserved2;
	__u32	valid_data_len[2];
	__u32	reserved3;
	__u32	first_clus;
	__u32	data_len[2];
} exfat_stream_dentry;

/* MS-DOS EXFAT file name directory entry (32 bytes) */
typedef struct {
	__u8	entry_type;
	__u8	flags;
	__u16	name[EXFAT_NAME_ENTRY_MAXCHAR];
} exfat_name_dentry;

typedef struct {
	exfat_file_dentry    file_dentry ;
	exfat_stream_dentry  stream_dentry ;
	exfat_name_dentry    name_dentry[EXFAT_NAME_ENTRY_MAXNUM];//although this is an array, but the valid number should be calculate from Stream_Entry
} exfat_entry_set;

typedef struct exdir_slot {
	__u8	id;		/* Sequence number for slot */
	__u8	name0_4[10];	/* First 5 characters in name */
	__u8	attr;		/* Attribute byte */
	__u8	reserved;	/* Unused */
	__u8	alias_checksum;/* Checksum for 8.3 alias */
	__u8	name5_10[12];	/* 6 more characters in name */
	__u16	start;		/* Unused */
	__u8	name11_12[4];	/* Last 2 characters in name */
} exdir_slot;

/*
 * Private filesystem parameters
 *
 * Note: FAT buffer has to be 32 bit aligned
 * (see FAT32 accesses)
 */
typedef struct {
	__u8	*fatbuf;	/* Current FAT buffer */
	int	fatsize;	/* Size of FAT in bits */
	__u32	fatlength;	/* Length of FAT in sectors */
	__u16	fat_sect;	/* Starting sector of the FAT */
	__u32	rootdir_sect;	/* Start sector of root directory */
	__u16	sect_size;	/* Size of sectors in bytes */
	__u16	clust_size;	/* Size of clusters in sectors */
	int	data_begin;	/* The sector of the first cluster, can be negative */
	int	fatbufnum;	/* Used by get_fatent, init to -1 */
} exfsdata;

typedef int	(file_detectfs_func)(void);
typedef int	(file_ls_func)(const char *dir);
typedef long	(file_read_func)(const char *filename, void *buffer,
				 unsigned long maxsize);
/*
struct filesystem {
	file_detectfs_func	*detect;
	file_ls_func		*ls;
	file_read_func		*read;
	const char		name[12];
};
*/

/* FAT tables */
file_detectfs_func	file_exfat_detectfs;
file_ls_func		file_exfat_ls;
file_read_func		file_exfat_read;

/* Currently this doesn't check if the dir exists or is valid... */
int file_cd(const char *path);
int file_exfat_detectfs(void);
int file_exfat_ls(const char *dir);
int exfat_exists(const char *filename);
long file_exfat_read_at(const char *filename, unsigned long pos, void *buffer,
		      unsigned long maxsize);
long file_exfat_read(const char *filename, void *buffer, unsigned long maxsize);
const char *file_getfsname(int idx);
int exfat_set_blk_dev(block_dev_desc_t *rbdd, disk_partition_t *info);
int exfat_register_device(block_dev_desc_t *dev_desc, int part_no);

int exfile_fat_write(const char *filename, void *buffer, unsigned long maxsize);
int exfat_read_file(const char *filename, void *buf, int offset, int len);
void exfat_close(void);
#endif /* _EXFAT_H_ */
