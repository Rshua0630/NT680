/**
    Partial Load.

    Partial load module.

    @file       PartLoad.h
    @ingroup    mPARTLOAD

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _PARTLOAD_H
#define _PARTLOAD_H

#include "Dx.h"

/**
    @addtogroup mPARTLOAD
*/
//@{

#define PARTLOAD_API_VERSION 0x13020616U ///< API version

/**
     Data Type

     The type of loaded data. (eg. not loaded data is compressed data by Lz)
*/
typedef enum _PARTLOAD_DATA_TYPE {
	PARTLOAD_DATA_TYPE_UNCOMPRESS,    ///< uncompress data
	PARTLOAD_DATA_TYPE_COMPRESS_LZ,   ///< data compressed by lz
	PARTLOAD_DATA_TYPE_COMPRESS_GZ,   ///< data compressed by gz
	ENUM_DUMMY4WORD(PARTLOAD_DATA_TYPE)
} PARTLOAD_DATA_TYPE;


/**
     Initial structure

     Partial initial structure.
*/
typedef struct _PARTLOAD_INIT {
	UINT32 uiApiVer;        ///< have to be PARTLOAD_API_VERSION
	DX_HANDLE hStrg;        ///< firmware stroage object handle
	PARTLOAD_DATA_TYPE DataType; ///< data type
	UINT32 uiAddrBegin;     ///< Part-1 finish address, set it to 0 means full load
	UINT32 uiWorkingAddr;   ///< Working Buffer Address (have to non-cachable)
	UINT32 uiWorkingSize;   ///< Working Buffer Size (equal to a block size, if nand used, it maybe 16384 or 131072 bytes)
} PARTLOAD_INIT, *PPARTLOAD_INIT;

/**
     PartLoad_IsLoaded output structure

     Indicate the part was loaded.
*/
typedef struct _PARTLOAD_ISLOADED {
	BOOL bLoaded;       ///< inidcate the address is loaded
	UINT32 uiPart;      ///< the input address belong to which part index
} PARTLOAD_ISLOADED, *PPARTLOAD_ISLOADED;

/**
     Initial Structure

     This module have to initial when system startup and just set once.

     @param[in] pInit      initial structure
     @return Description of data returned.
         - @b E_OK:   Success
         - @b E_SYS:  Failed to initial
*/
ER PartLoad_Init(PARTLOAD_INIT *pInit);

/**
     Load a part

     Given a part index to load a part.

     @param[in] uiPart part index
     @return Description of data returned.
         - @b E_OK:   Success
         - @b E_SYS:  Failed to load
*/
typedef void (*PARTLOAD_CB_LOAD_FINISH)(const UINT32 Idx);
ER PartLoad_Load(UINT32 uiPart, PARTLOAD_CB_LOAD_FINISH fpFinish);

/**
     Indicate an address is loaded

     Given an calling address to check it whether is loaded

     @param[in] uiAddr address
     @param[out] pOut inidcate the address is loaded and belongs to which part
     @return Description of data returned.
         - @b E_OK:   Success
         - @b E_SYS:  Failed to check, maybe partial load function not inited.
         - @b E_PAR:  The uiAddr is out of range
         - @b E_CTX:  this funciton does not support to full loading
*/
ER PartLoad_IsLoaded(PARTLOAD_ISLOADED *pOut, UINT32 uiAddr);

/**
     Get Initial Structure

     Given an empty structure to return initial settings

     @param[out] pInit output a structure which is initial structure
     @return Description of data returned.
         - @b E_OK:   Success
         - @b E_SYS:  Failed to get
*/
ER PartLoad_GetInit(PARTLOAD_INIT *pInit);

/**
	Install PartLoad task,flag and semaphore id
*/
extern void PartLoad_InstallID(void) _SECTION(".kercfg_text");

/**
    PartLoad partial uncompressing task.
*/
extern VOID PartLoadTsk(VOID);


//@}
#endif //_PARTLOAD_H
