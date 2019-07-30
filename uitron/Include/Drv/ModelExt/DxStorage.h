#ifndef _DXSTORAGE_H_
#define _DXSTORAGE_H_

#include "Dx.h"

typedef enum _DXSTRG_ID {
	DXSTRG_ID_FW_RSV1,                  ///< Firmware reserved area 1
	DXSTRG_ID_FW_RSV2,                  ///< Firmware reserved area 2
	DXSTRG_ID_FW_RSV3,                  ///< Firmware reserved area 3

	DXSTRG_ID_FAT1 = 0x10,              ///< FAT area 1
	DXSTRG_ID_FAT2,                     ///< FAT area 2
	DXSTRG_ID_FAT3,                     ///< FAT area 3

	DXSTRG_ID_PSTORE1 = 0x20,           ///< Pstore area 1
	DXSTRG_ID_PSTORE2,                  ///< Pstore area 2
	DXSTRG_ID_PSTORE3,                  ///< Pstore area 2
	DXSTRG_ID_PSTORE4,                  ///< Pstore area 2
	DXSTRG_ID_PSTORE5,                  ///< Pstore area 2
	ENUM_DUMMY4WORD(DXSTRG_ID)
} DXSTRG_ID;

typedef struct _DXSTRG_PARTITION {
	UINT32     uiDxClassType;
	UINT32     uiPhyAddr;
	UINT32     uiPhySize;
	UINT32     uiResvSize;
} DXSTRG_PARTITION, *PDXSTRG_PARTITION;

typedef struct _DXSTRG_INIT {
	MEM_RANGE        buf;
	DXSTRG_PARTITION prt; //only for NAND now
} DXSTRG_INIT, *PDXSTRG_INIT;

//ver
#define STORAGE_VER                 0x00010000

//caps base flag
#define STORAGE_BF_DETPLUG          0x00000001 //support STORAGE_STATE_INSERT
#define STORAGE_BF_DETWP            0x00000002 //support STORAGE_STATE_LOCK
#define STORAGE_BF_SKIPCLOSE        0x00000004 //skip CLOSE for special card

//caps id
#define STORAGE_CAPS_BASE           0x00000001
#define STORAGE_CAPS_PLUG           0x00000002
#define STORAGE_CAPS_HANDLE         0x00000003

//state id
#define STORAGE_STATE_ENBALE        0   //
#define STORAGE_STATE_MODE          1   //
#define STORAGE_STATE_SLEEP         2   //
#define STORAGE_STATE_INSERT        3   //detect card insert
#define STORAGE_STATE_LOCK          4   //detect card write-protected
#define STORAGE_STATE_TYPE          5   //get card type

//control id
#define STORAGE_CTRL_POWER          0   //power control
#define STORAGE_CTRL_MODE           1   //mode control (for change mode)
#define STORAGE_CTRL_SLEEP          2   //sleep control (for enter/leave sleep mode)
#define STORAGE_CTRL_WAITFINISH     5   //wait enable finish (for change device)

//nand Device Code
#define NAND_128M_2K                    0xF1
#define NAND_256M_2K                    0xDA


extern void DrvCARD_SetInsert(UINT32 status);
extern void DrvCARD_DetRestart(void);

extern void DrvCARD2_SetInsert(UINT32 status);
extern void DrvCARD2_DetRestart(void);

#endif //_DXSTORAGE_H_

