/***************************************************************************
	(C)Copyright 2013, NOVATEK MICROELECTRONICS CORP. STB all rights reserved.
 
	Product	: STB Firmware
	Date	 : 2013/10/22
    Purpose	 :
	Sources	 : def_type.h
	Customer : Novatek STB
***************************************************************************/
/*
 * Copyright (C) 2008 Novatek Inc, STB all right reserved.
 * This document is the CONFIDENTIAL property of Novatek Microelectronics Corp.
*/
/*
 * Brief: Common type definition.
 */

#ifndef __DEF_TYPE_H__
#define __DEF_TYPE_H__

#ifdef  __cplusplus
extern "C" {
#endif

#include <cyg/infra/cyg_type.h>
/******************************************************************************/

/*! \define
* This file provides the application's common defination.
*/

#ifndef TRUE
#define TRUE		1					/*!< Define TRUE 1 */
#endif

#ifndef ENABLE
#define ENABLE		1				/*!< Define ENABLE 1 */
#endif

#ifndef FALSE
#define FALSE		0					/*!< Define FALSE 0 */
#endif

#ifndef DISABLE
#define DISABLE		0				/*!< Define DISABLE 0 */
#endif

#ifndef NULL
#define NULL    	((void *)(0))			/*!< Define NULL (void *)(0) */
#endif

#ifndef SUCCESS
#define SUCCESS		0
#endif

#define INVALID_HANDLE 0xffffffff

#define ABS(a, b)	((a>b) ? (a-b) : (b-a)) /*!< Define ABS(a,b) */
#define PU8TOU16(pu8Ptr)	((((u16)((u8)*((u8*)(pu8Ptr)))) << 8)|((u16)((u8)*((u8*)(pu8Ptr+1)))))
#define MAKEWORD(low, high) ((u16)(((u8)(low))  | (((u16)((u8)(high))) << 8 ))) /*!< Define MAKEWORD(low, high) */
#define MAKELONG(low, high) ((u32)(((u16)(low)) | (((u32)((u16)(high))) << 16)))/*!< Define MAKELONG(low, high) */

#ifndef  _Pack_Struct_
#define  _Pack_Struct_		__attribute__ ((__packed__))
#endif
/******************************************************************************/

/*!
* This provides the application's variable types.
*/
typedef unsigned char	u8;		/*!< 1 byte, range: 0 to 255 */
typedef unsigned short	u16;	/*!< 2 bytes, range: 0 to 65536 */
typedef unsigned long	u32;	/*!< 4 bytes, range: 0 to 4G */
typedef unsigned long long  u64;/*!< 8 bytes */
typedef char			s8;		/*!< 1 byte, range: -128 to 127 */
typedef short			s16;	/*!< 2 bytes, range: -32768 to 32767 */
typedef long			s32;	/*!< 4 bytes, range: -2G to 2G */
typedef long long         s64;	/*!< 8 bytes */
//typedef unsigned char	bool;	/*!< 1 byte, range: TRUE or FALSE */
//typedef unsigned int    UINT;	/*!< 2 bytes, range: 0 to 65535 */
typedef unsigned short 	c16;	/**< 1 character 2 bytes, range: 0 to 65536 */
//typedef int				CT_STATUS;
//typedef int				CT_INT;
typedef volatile unsigned char 		vu8 ;
typedef volatile unsigned short 		vu16 ;
typedef volatile unsigned long 		vu32 ;

typedef u8 *                  pu8;
typedef u16 *                pu16;
typedef u32 *                pu32;

typedef float                f32;

typedef char c8;	
typedef struct
{
	s32   s32X;
	s32   s32Y;
	u32   u32W;
	u32   u32H;
} ST_RECT;


/* Driver device name type */
#define DRV_MAX_DEVICE_NAME 16  /* 15 characters plus '\0' */
typedef char DRV_DeviceNameType[DRV_MAX_DEVICE_NAME];

#if 0
/*!
* This provides the drivers return value.
*/
typedef enum _EN_DRV_RESULT
{
	DRV_OK,						/*!< Return type is OK. */
	DRVERR_NOTOK,				/*!< Return type is not OK. */
	DRVERR_NOTSUPPORTED,		/*!< Return type is not supported. */
	DRVERR_NOTINITIALISED,		/*!< Return type is not initial. */
	DRVERR_HARDWAREFAIL,		/*!< Return type is hardware fail. */
	DRVERR_OUTOFRANGE,			/*!< Return type is parameter out of range. */
	DRVERR_UNCOMPLETE,			/*!< Return type is not complete. */
	DRVERR_TIMEOUT=0x100,		/*!< Return type is time out. */
	DRVERR_NOT_ENOUGH_MEMORY,	/*!< Return type is memory error. */
	DRVERR_OS_TASK=0x110,		/*!< Return type is task error. */
	DRVERR_OS_QUEUE,			/*!< Return type is queue error. */
	DRVERR_OS_EVENT,			/*!< Return type is event error. */
	DRVERR_OS_SEMAPHORE,		/*!< Return type is semaphore error. */
	DRVERR_OS_TIMER,			/*!< Return type is timer error. */
	DRVERR_UNKNOWN=0xFFFF		/*!< Return type is error. */
} EN_DRV_RESULT;
#endif

//#include "dbgfuncs.h"

#ifdef  __cplusplus
    }
#endif

#endif /* __DEF_TYPE_H__ */
