/**
    Traffic sign recognition library.

    @file       tsr_lib.h
    @ingroup    mILibTSR

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef TSR_LIB_H
#define TSR_LIB_H

#include    "Type.h"
#include    "GxImage.h"
#include    "mIType.h"
#include    "fd_lib.h"

#define TSR_STA_OK   1
#define TSR_STA_ERR  -1

#define TSD_IMG_PART_CNT    1
#define TSD_FDE_MAX_TS_NUM  1023
#define TSD_MAX_DET_TS_NUM  (TSD_FDE_MAX_TS_NUM * TSD_IMG_PART_CNT)
#define TSD_MAX_WIDTH       1920
#define TSD_MAX_HEIGHT      1080
#define TSR_SPEED_ENUM_CNT  12

#define TSR_READ_TSD_MODEL_FROM_SDCARD DISABLE

#define FLGTSR_NEWRST   FLGPTN_BIT(2)       //0x00000100
#define FLGTSR_RSTDONE  FLGPTN_BIT(1)       //0x00000010

typedef enum _TSR_SPEED
{
    TSR_SPEED_NONE = 0,
    TSR_SPEED_20   = 1,
    TSR_SPEED_30,
    TSR_SPEED_40,
    TSR_SPEED_50,
    TSR_SPEED_60,
    TSR_SPEED_70,
    TSR_SPEED_80,
    TSR_SPEED_90,
    TSR_SPEED_100,
    TSR_SPEED_110   = 10,
    TSR_SPEED_120,
    TSR_SPEED_CNT,
    ENUM_DUMMY4WORD(TSR_SPEED)
}TSR_SPEED;

typedef enum
{
    TSR_TIME_FILTER_1 = 1,
    TSR_TIME_FILTER_2 = 2,
    TSR_TIME_FILTER_3 = 3,
    TSR_TIME_FILTER_4 = 4,
    TSR_TIME_FILTER_5 = 5
} TSR_TIME_FILTER_VAL;

typedef enum
{
    TSR_FILTER_MEAN,
    TSR_FILTER_SORT,
    TSR_FILTER_MAX_SORT,
} TSR_FILTER_TYPE;

#define TSR_USER_PARAM_FIELDS            \
    X(TSR_USER_ENABLE_TIME_FILTER, BOOL, TRUE) \
    X(TSR_USER_ENABLE_PRECIMODE, BOOL, TRUE) \
    X(TSR_USER_ENABLE_SCORE2, BOOL, FALSE) \
    X(TSR_USER_TIME_FITER_THRE, TSR_TIME_FILTER_VAL, TSR_TIME_FILTER_2) \
    X(TSR_USER_FILTER_TYPE, TSR_FILTER_TYPE, TSR_FILTER_MAX_SORT) \
    X(TSR_USER_FILTER_SORT_MAX_NUM, UINT32, 10) \
    X(TSR_USER_TSD_SC1_THRE, UINT32, 40) \
    X(TSR_USER_TSD_SC2_THRE, UINT32, 100) \
    X(TSR_USER_TSD_STD_THRE, UINT32, 30) \
    X(TSR_USER_TSD_FDE_MAX_SORT_FILTER_THRE, UINT32, 614)

typedef enum
{
#define X(name, type, value) name,
    TSR_USER_PARAM_FIELDS
#undef X
} TSR_USER_PARAM_TYPE;

ER TSR_SetUserParam(TSR_USER_PARAM_TYPE type, UINT32 value);

UINT32 TSR_GetUserParam(TSR_USER_PARAM_TYPE type);

/**
     Traffic sign detection input image ratio.

*/
typedef enum _TSR_IMAGERATIO
{
    TSR_IMAGERATIO_4_3 = 0,                     ///< input image ratio is 4:3
    TSR_IMAGERATIO_3_2,                         ///< input image ratio is 3:2
    TSR_IMAGERATIO_16_9,                        ///< input image ratio is 16:9
    TSR_IMAGERATIO_MAX_CNT,                     ///< total image ratio count
    ENUM_DUMMY4WORD(TSR_IMAGERATIO)
}TSR_IMAGERATIO;

typedef struct
{
    UINT32  uiStx;            ///< result start x
    UINT32  uiSty;            ///< result start y
    UINT32  uiWidth;
    UINT32  uiHeight;
    TSR_SPEED uiSpeedNum;
} TRAFFIC_SIGN;

/**
     Traffic sign detection result info.

*/
typedef struct _TSDMSG_INFO
{
    UINT32  TsDetectNum;                            ///<Detected total traffic sign number
    TRAFFIC_SIGN TsDetectRect[TSD_MAX_DET_TS_NUM];  ///<Detected traffic sign
} TSDMSG_INFO;

extern UINT32 _SECTION(".kercfg_data") g_uiFLG_ID_TS;

void TSR_InstallID(void);

UINT32 TSR_CalcCacheBuffSize(void);

UINT32 TSR_CalcBuffSize(void);

void TSR_InstallID(void) _SECTION(".kercfg_text");

INT32 TSR_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 ProcessRate);

INT32 TSR_UnInit(void);

BOOL TSR_ValidForProcess(VOID);

VOID TSR_PreProcess(IMG_BUF *pInputImg, TSR_IMAGERATIO imgRatio, UINT32 workBuffAddr, UINT32 cacheBuffAddr);

INT32 TSR_Process(VOID);

UINT32 TSR_GetRsltTS(TRAFFIC_SIGN *TS, URECT* TargetCoord);

void TSR_GetRstsMsg(IMG_BUF **pImg, TSDMSG_INFO **pMsg);

void TSR_Lock(BOOL bLock);

//#NT#2018/06/06#B_B Jiang -begin
BOOL TSR_GetLockStatus(VOID);
//#NT#2018/06/06#B_B Jiang -end

INT16 TSR_Speed2Num(TSR_SPEED speed);

BOOL TSR_SetTsrImgROI(URECT roi);

URECT TSR_GetTsrImgROI(VOID);

#endif
