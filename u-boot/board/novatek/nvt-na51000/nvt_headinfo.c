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
#include <nvt_type.h>
#include <asm/sections.h>
#include <modelext/bin_info.h>

#define __string(_x) #_x
#define __xstring(_x) __string(_x)

extern char uboot_bin_size[];

extern HEADINFO gHeadInfo __attribute__ ((section (".data.bininfo")));
HEADINFO gHeadInfo =
{
	(UINT32)&__image_copy_start, //<- fw CODE entry (4)
	{0}, ///<- reserved (4*19)
	"ub51000 ", //<- CHIP-NAME / TAG-NAME (8)
	"FFFFFFFF", //<- version (8)
	__xstring(_BUILD_DATE_), //<- releasedate (8)
	0xffffffff, //<- Bin File Length (4)
	0xffffffff, //<- Check Sum or CRC (4)
	0,///<- Length check for CRC (4)
	0,///<- where modelext data is. w by Ld / u-boot (4)
#ifdef _NVT_LINUX_SMP_ON_
	2,///<- Bin flag (4)
#else
	0,
#endif
	0,///<- Binary Tag for CRC (4)
};
