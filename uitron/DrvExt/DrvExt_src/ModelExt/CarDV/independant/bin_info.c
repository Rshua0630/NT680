#include "modelext_info.h"
#include "bin_info.h"

BININFO ind_bin_info_data __attribute__((section("modelext_data.bin_info"))) = {
	//HEADINFO
	{
		_BOARD_UITRON_ADDR_ + CODE_ENTRY_OFFSET, //<- fw CODE entry (4)
		{0}, ///<- reserved (4*19)
		_CHIP_NAME_, //<- CHIP-NAME (8)
		"10000000", //<- version (8)
		"20160509", //<- releasedate (8)
		0xffffffff, //<- Bin File Length (4)
		0xffffffff, //<- Check Sum or CRC (4)
		0,///<- Length check for CRC (4)
		0,///<- reserved (4)
		0,///<- Bin flag (4)
		0,///<- Binary Tag for CRC (4)
	},
	//LDINFO
	{
		"LD:GS680        ", ///<- LD-NAME (16)
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
	//COMMINFO
	{
		"COMMINFO        ", ///< COMMINFO (16)
		{0},                ///< Resv (48)
	},
	//FWINFO
	{
		_BIN_NAME_, //BIN-NAME (16) to notify uboot update-name
		{0} ///<- (4*16)  --------- reserved for project Fw
	}
};

MODELEXT_HEADER ind_bin_info_header __attribute__((section("modelext_header.bin_info"))) = {
	.size = sizeof(ind_bin_info_data) + sizeof(MODELEXT_HEADER),
	.type = MODELEXT_TYPE_BIN_INFO,
	.number = 1,
	.version = BIN_INFO_VER,
};
