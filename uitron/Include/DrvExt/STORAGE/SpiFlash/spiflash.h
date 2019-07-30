#ifndef _SPIFLASH_DEF_H
#define _SPIFLASH_DEF_H

#include "spi.h"
#include "StrgDef.h"
#include "nand.h"
#if 0
/*
    SPI Flash Wide Bus Capability
*/
typedef enum {
	SPI_FLASH_BUSWIDTH_NORMAL = 0,  //< Normal 1bit full duplex flash
	SPI_FLASH_BUSWIDTH_DUAL = 0x01, //< Support dual read (0x3B)
	SPI_FLASH_BUSWIDTH_QUAD_TYPE1 = 0x10,   //< Support quad read (0xEB)
	//< But QE (Quad Enable) bit is in Status Register[bit 6]
	//<  and 0xEB command requires 4 dummy clocks
	SPI_FLASH_BUSWIDTH_QUAD_TYPE2 = 0x20,   //< Support quad read (0xEB)
	//< But QE (Quad Enable) bit is in Status Register[bit 9]
	//< and  and 0xEB command requires 4 dummy clocks
	SPI_FLASH_BUSWIDTH_QUAD_TYPE3 = 0x40,   //< Support quad read (0xEB)
	//< But not require QE (Quad Enable) bit and 0xEB command requires 8 dummy clocks
	SPI_FLASH_BUSWIDTH_QUAD_TYPE4 = 0x80,   ///< Support quad read, QE(Quad Enable) bit  is in Status Register[bit 9] and 0xEB command requires 4 dummy clocks.
	///< But QE should be modified by 0x31 command

	ENUM_DUMMY4WORD(SPI_FLASH_BUSWIDTH)
} SPI_FLASH_BUSWIDTH;


/**
    SPI flash identification structure

    @note For SPIFLASH_IDENTIFY_CB
*/
typedef struct {
	UINT32      uiTotalSize;        ///< total size (unit: byte)
	UINT32      uiSectorSize;       ///< sector size (unit: byte)
	BOOL        bSupportEWSR;       ///< support EWSR command
	BOOL        bSupportAAI;        ///< support AAI command
	BOOL        bSupportSecErase;   ///< support SECTOR_ERASE command (0x20)
	UINT32      uiSectorEraseTime;  ///< Sector erase timeout (unit: ms)
	UINT32      uiPageProgramTime;  ///< page program timeout (unit: ms)
	SPI_FLASH_BUSWIDTH  flashWidth; //< Wide bus (dual/quad) supported by this flash
} SPIFLASH_IDENTIFY, *PSPIFLASH_IDENTIFY;
#endif

/**
    SPIFLASH operation

    @note For SPIFLASH_INVALID_WRITE_CB()
*/
typedef enum {
	SPIFLASH_OPERATION_READ,        ///< read operation
	SPIFLASH_OPERATION_WRITE,       ///< write operation
	SPIFLASH_OPERATION_ERASE,       ///< erase operation

	ENUM_DUMMY4WORD(SPIFLASH_OPERATION)
} SPIFLASH_OPERATION;

/**
    SPIFLASH invalid write callback

    Callback routine to be invoked when invalid erase/write access is detected by driver.

    @note STRG_EXT_CMD_SPI_INVALID_WRITE_CB

    @param[in] first parameter  Operating type
                                - @b SPIFLASH_OPERATION_ERASE: erase
                                - @b SPIFLASH_OPERATION_WRITE: write
    @param[in] second parameter Starting address
    @param[in] third parameter  Access length

    @return void
*/
typedef void (*SPIFLASH_INVALID_WRITE_CB)(SPIFLASH_OPERATION, UINT32, UINT32);

/**
    SPIFLASH identify callback

    Callback routine to be invoked after JEDEC ID is read from spi flash.
    Callback routine should check if read ID is supported.

    @note STRG_EXT_CMD_SPI_IDENTIFY_CB

    @param[in] first parameter  (JEDEC) manufacturer ID read from spi flash
    @param[in] second parameter (JEDEC) type ID read from spi flash
    @param[in] third parameter  (JEDEC) capacity ID read from spi flash
    @param[out] forth parameter flash identification returned to spi flash driver

    @return
        - @b TRUE: call back will handle identification of this flash. and PSPI_IDENTIFY will fill identifed information
        - @b FALSE: input ID is NOT supported/identified by call back
*/
//typedef BOOL (*SPIFLASH_IDENTIFY_CB)(UINT32, UINT32, UINT32, PSPIFLASH_IDENTIFY);


extern void spiflash_InstallID(void) _SECTION(".kercfg_text");
extern PSTORAGE_OBJ spiflash_getStorageObject(STRG_OBJ_ID strgObjID);

#endif

