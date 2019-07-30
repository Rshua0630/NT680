/**
    Binary format utility.

    The utilities to get binary data.

    @file       BinaryFormat.h
    @ingroup    mIUtilBinFormat
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _BINFMT_H
#define _BINFMT_H

#include "Type.h"

/**
    @addtogroup mIUtilBinFormat
*/
//@{


/**
    @name  Get Raw Data (Bytes Are In Little Endian Order)
*/
//@{
#define UINT32_Byte1(data)      ((UINT8)((((UINT32)(data))) & 0x000000FF))      ///< Get a byte data which is [7:0] of a 32-bit data.
#define UINT32_Byte2(data)      ((UINT8)((((UINT32)(data))>>8) & 0x000000FF))   ///< Get a byte data which is [15:8] of a 32-bit data.
#define UINT32_Byte3(data)      ((UINT8)((((UINT32)(data))>>16) & 0x000000FF))  ///< Get a byte data which is [23:16] of a 32-bit data.
#define UINT32_Byte4(data)      ((UINT8)((((UINT32)(data))>>24) & 0x000000FF))  ///< Get a byte data which is [31:24] of a 32-bit data.
#define UINT32_LoWord(data)     ((UINT16)((((UINT32)(data))) & 0x0000FFFF))     ///< Get a 16-bit data which is [15:0] of a 32-bit data.
#define UINT32_HiWord(data)     ((UINT16)((((UINT32)(data))>>16) & 0x0000FFFF)) ///< Get a 16-bit data which is [31:16] of a 32-bit data.
#define UINT16_LoByte(data)     ((UINT8)((((UINT32)(data))) & 0x000000FF))      ///< Get a byte data which is [7:0] of a 16-bit data.
#define UINT16_HiByte(data)     ((UINT8)((((UINT32)(data))>>8) & 0x000000FF))   ///< Get a byte data which is [15:8] of a 16-bit data.
#define HiWord(data)            UINT32_HiWord(data)     ///< Refer to UINT32_HiWord().
#define LoWord(data)            UINT32_LoWord(data)     ///< Refer to UINT32_LoWord().
#define HiByte(data)            UINT16_HiByte(data)     ///< Refer to UINT16_HiByte().
#define LoByte(data)            UINT16_LoByte(data)     ///< Refer to UINT16_LoByte().
//@}




/**
    @name  Get Swap Raw Data (Bytes Are In Big Endian Order)
*/
//@{
#define UINT32_SWAP(data)           (((((UINT32)(data)) & 0x000000FF) << 24) | \
									 ((((UINT32)(data)) & 0x0000FF00) << 8) | \
									 ((((UINT32)(data)) & 0x00FF0000) >> 8) | \
									 ((((UINT32)(data)) & 0xFF000000) >> 24))   ///< Swap [31:24] with [7:0] and [23:16] with [15:8].
#define UINT32_LoWord_SWAP(data)    ((((UINT32)(data) & 0x000000FF) << 8) | \
									 (((UINT32)(data) & 0x0000FF00) >> 8))      ///< Swap [15:8] with [7:0] and mask [31:24].
#define UINT32_HiWord_SWAP(data)    ((((UINT32)(data) & 0x00FF0000) >> 8) | \
									 (((UINT32)(data) & 0xFF000000) >> 24))     ///< Equals to UINT32_SWAP(data&0xFFFF0000)
#define UINT16_SWAP(data)       ((UINT16)UINT32_LoWord_SWAP(data))              ///< Refer to UINT32_LoWord_SWAP().
//@}

/**
    @name  Data format transform (Bytes Are In Little Endian Order)
*/
//@{
#define DESC_WORD(x)   \
	UINT32_Byte1(x),   \
	UINT32_Byte2(x)         ///< Transform the least significant 16-bit data to two bytes data in little endian order.

#define DESC_DWORD(x)   \
	UINT32_Byte1(x),    \
	UINT32_Byte2(x),    \
	UINT32_Byte3(x),    \
	UINT32_Byte4(x)         ///< Transform a 32-bit data to four bytes data in little endian order.
//@}

extern UINT32 Get32BitsData(UINT32 Addr, BOOL bIsLittleEndian);
extern UINT16 Get16BitsData(UINT32 Addr, BOOL bIsLittleEndian);
extern void   Set32BitsData(UINT32 Addr, UINT32 Value, BOOL bIsLittleEndian);
extern void   Set16BitsData(UINT32 Addr, UINT16 Value, BOOL bIsLittleEndian);
extern UINT8 GetCurByte(UINT8 **);
extern UINT16 GetCurSHORT(UINT8 **, BOOL);
extern UINT32 GetCurUINT(UINT8 **, BOOL);

//@}

#endif

