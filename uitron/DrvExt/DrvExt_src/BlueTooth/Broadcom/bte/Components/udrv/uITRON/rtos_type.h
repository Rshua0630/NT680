#ifndef RTOS_UITRON
#define RTOS_UITRON

#include "Type.h"
#include "mIType.h"

typedef unsigned char                           UCHAR;
typedef long                                    LONG;
typedef unsigned long                           ULONG;
typedef short                                   SHORT;
typedef unsigned short                          USHORT;
typedef unsigned char   BOOLEAN;
typedef UINT32          UINTPTR;
typedef UINT32          TIME_STAMP;


typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef unsigned char       uint8;
typedef signed   char       int8;
typedef unsigned short      uint16;
typedef signed   short      int16;
typedef unsigned long       uint32;
typedef signed   long       int32;
typedef UINT32      uint32_t;
typedef INT32       int32_t;
typedef UINT8       uint8_t;
typedef INT8        int8_t;
typedef UINT16      uint16_t;
typedef INT16       int16_t;
typedef UCHAR       wiced_bool_t;

#define BT_OCTET16_LEN    16                /**<  length: 16 */
typedef uint8_t BT_OCTET16[BT_OCTET16_LEN]; /**< octet array: size 16 */

#define BT_OCTET32_LEN    32
typedef uint8_t BT_OCTET32[BT_OCTET32_LEN];   /* octet array: size 32 */

#define WICED_FALSE 0
#define WICED_TRUE  1
//#define NULL     0
#endif