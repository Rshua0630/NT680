/****************************************************************************
**
**  Name        data_types.h
**
**  Function    this file contains common data type definitions used
**              throughout the WIDCOMM Bluetooth code
**
**  Date       Modification
**  -----------------------
**  3/12/99    Create
**  07/27/00   Added nettohs macro for Little Endian
**                                                                         
**  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.
**  Proprietary and confidential.
**
*****************************************************************************/
//#include <itron.h>

#ifndef DATA_TYPES_H
#define DATA_TYPES_H



#include "rtos_type.h"

#if 0

#ifndef NULL
#define NULL     0
#endif

#ifndef FALSE
#define FALSE  0
#endif


typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef signed   long   INT32;
typedef signed   char   INT8;
typedef signed   short  INT16;
typedef unsigned char   BOOLEAN;
typedef UINT32          UINTPTR;
typedef UINT32          TIME_STAMP;


#ifndef TRUE
#define TRUE   (!FALSE)
#endif

#endif


typedef unsigned char   UBYTE;

#ifdef __arm
#define PACKED  __packed
#define INLINE  __inline
#else
#define PACKED
#define INLINE
#endif

#define BIG_ENDIAN FALSE

#define UINT16_LOW_BYTE(x)      ((x) & 0xff)
#define UINT16_HI_BYTE(x)       ((x) >> 8)

/* Timer list entry callback type
*/
typedef void (TIMER_CBACK)(void *p_tle);
#ifndef TIMER_PARAM_TYPE
#define TIMER_PARAM_TYPE    UINT32
#endif

// uitron porting added

#define BCM_STRNCPY_S(x1,x2,x3,x4)  strncpy((x1),(x3),(x4))
#define BT_MEMCPY(a,b,c)            memcpy( (a), (b), (c) )
#define BT_MEMCMP(a,b,c)            memcmp( (a), (b), (c) )


#endif
