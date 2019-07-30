//    Common Type

#ifndef _GX_COMMON_H_
#define _GX_COMMON_H_

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef unsigned __int64    UINT64;
typedef __int64             INT64;
//typedef   unsigned long       UINT32;
//typedef   long                INT32;
typedef unsigned short      UINT16;
typedef short               INT16;
typedef unsigned char       UINT8;
typedef char                INT8;
typedef UINT8               UCHAR;
typedef INT8                CHAR;
//typedef UINT8               BOOL;
//#define TRUE                1
//#define FALSE               0

//#define _GX_ADDRESS_ALIGN_4     __declspec(align(4))
#define _GX_INLINE              _INLINE
#define GX_PACK 1
#define GX_PACK_STRUCT struct
/*
#pragma pack(1)
#pragma pack(push,1) // packing is now 1
#pragma pack(pop) // packing is 8
*/

#elif defined (__CC_ARM)

#include "Type.h"
#include <string.h> //for memset()

//#define _GX_ADDRESS_ALIGN_4     __align(4)
#define _GX_INLINE              _INLINE
#define GX_PACK 0
#define GX_PACK_STRUCT __packed struct

#else //other platform

#include "Type.h"
#include <string.h> //for memset()

//#define _GX_ADDRESS_ALIGN_4     __align(4)
#define _GX_INLINE              _INLINE
#define GX_PACK 0
#define GX_PACK_STRUCT struct

#endif


//--------------------------------------------------------------------------------------
//  type
//--------------------------------------------------------------------------------------
/** Event handler function prototype */
typedef void (*GX_CALLBACK_PTR)(UINT32 event, UINT32 param1, UINT32 param2);

//typedef void (*FPGXCallback)(UINT32 event, UINT32 param1, UINT32 param2); //general call back
//typedef void (*FPGXSysFunc)(UINT32 event); //GxSystem XXX_OnSystem()

//--------------------------------------------------------------------------------------
//  System Command
//--------------------------------------------------------------------------------------
#define SYSTEM_CB_CONFIG        0xFFFFFFFF

#define SYSTEM_UNKNOWN          ((INT32)-1)

//power flow
#define SYSTEM_CMD_POWERON      0x8000
#define SYSTEM_CMD_POWEROFF     0x8001
//sleep flow
#define SYSTEM_CMD_SLEEPENTER   0x8002
#define SYSTEM_CMD_SLEEPLEAVE   0x8003
//change-device flow
#define SYSTEM_CMD_INSERT       0x8007
#define SYSTEM_CMD_REMOVE       0x8008
//change-dma-clk flow
#define SYSTEM_CMD_PAUSEDMA     0x8004
#define SYSTEM_CMD_RESUMEDMA    0x8005
#define SYSTEM_CMD_STOPDMA      0x8006
//change-cpu-clk flow
//change-eng-clk flow

//--------------------------------------------------------------------------------------
//  debug
//--------------------------------------------------------------------------------------

//<type>

typedef UINT16  RESULT; //return value

//<enum>

//Gx error code

//OK
#define GX_OK                       0x00
//General
#define GX_NULL_POINTER             0x01
#define GX_NULL_BUF                 0x02
#define GX_ERROR_INITDEVICE         0x03
#define GX_NOTREADY                 0x04
#define GX_UNKNOWN_FORMAT           0x05
#define GX_OUTOF_MEMORY             0x06
#define GX_ERROR_LOADFILE           0x07
#define GX_STILLREADY               0x08
//GxGfx
#define GX_DRAW_FAILED              0x80
#define GX_ERROR_FORMAT             0x81
#define GX_ERROR_PARAM              0x82
#define GX_CANNOT_CONVERT           0x83
#define GX_ERROR_TYPE               0x84
#define GX_ERROR_SIZEALIGN          0x85
#define GX_ERROR_XYALIGN            0x86
#define GX_ERROR_SIZEZERO           0x87
#define GX_ERROR_SIZEDIFF           0x88
#define GX_ERROR_LOCK               0x89
#define GX_OUTOF_PLANE              0x8A
#define GX_EMPTY_RECT               0x8B
#define GX_ERROR_ADDRALIGN          0x8C
#define GX_ERROR_PITCHLARGE         0x8D
#define GX_NOTSUPPORT_ROP           0x8E
#define GX_NO_PALETTE               0x8F
#define GX_NO_IMAGETABLE            0x90
#define GX_NO_FONT                  0x91
#define GX_NO_STRINGTABLE           0x92
#define GX_OUTOF_IMAGEID            0x93
#define GX_OUTOF_STRID              0x94
#define GX_INVALID_IMAGEID          0x95
#define GX_INVALID_STRID            0x96
//GxDisplay
#define GX_OUTOF_BUFCOUNT           0xA0
#define GX_NOTENOUGH_BUFSIZE        0xA1
#define GX_FAIL_SWAPEFFECT          0xA2
#define GX_NULL_BUFMAN              0xA3
#define GX_NULL_DL                  0xA4
#define GX_NULL_ML                  0xA5
//GxPhoto
#define GX_DECODE_FAILED            0xB0
//GxSystem
#define GX_TIMER_FAILED             0xC0


#endif//_GX_COMMON_H_
