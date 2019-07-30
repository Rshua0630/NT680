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

#include "Type.h"
#include "Platform.h"
#include "PrjCfg.h"
#include "bin_info.h"
#include "NvtVerInfo.h"

HEADINFO gHeadInfo = {
	_BOARD_UITRON_ADDR_ + CODE_ENTRY_OFFSET, //<- fw CODE entry (4)
	{0}, ///<- reserved (4*19)
	_CHIP_NAME_, //<- CHIP-NAME (8)
	"FFFFFFFF",  //<- version (8)
	__xstring(_BUILD_DATE_), //<- releasedate (8)
	0xffffffff, //<- Bin File Length (4)
	0xffffffff, //<- Check Sum or CRC (4)
	0,///<- Length check for CRC (4)
	0,///<- where modelext data is. w by Ld / u-boot (4)
	0,///<- Bin flag (4)
	0,///<- Binary Tag for CRC (4)
};
