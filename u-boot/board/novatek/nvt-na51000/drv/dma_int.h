/*
    @file       dma_int.h

    @ingroup    mIDrvDMA_DMA

    @brief      NT96680 DMA internal header file

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

*/

#ifndef _DMA_INT_H
#define _DMA_INT_H

//#include "DrvCommon.h"
//#include "top.h"
#include "dma_reg.h"

#define DMA_MAX_MONITOR_TIME    (140)       // 140ms

// Use UINT32 to store accumulated size in recent 1 sec
// assert if UINT32 is not large enought to hold 2^28*SAMPLE_COUNT_IN_ONE_SEC
//STATIC_ASSERT(0x10000000ULL*(1000/DMA_MONITOR_TIME) <= ULONG_MAX);

//#define DMA_MONITOR_QUEUE_DEPTH (1000/DMA_MONITOR_TIME)

/*
    DMA IST event

    @note For ISR to IST notification
*/
typedef enum
{
    DMA_IST_EVENT_WPSET0 = 0x01,        // write protect set 0
    DMA_IST_EVENT_WPSET1 = 0x02,        // write protect set 1
    DMA_IST_EVENT_WPSET2 = 0x04,        // write protect set 2
    DMA_IST_EVENT_MONITOR = 0x100,      // DMA usage monitor

//    DMA_MONITOR_TYPE_COUNT,

    ENUM_DUMMY4WORD(DMA_IST_EVENT)
} DMA_IST_EVENT;

/*
    DMA controller monitor structure

*/
typedef struct
{
    UINT32 uiSizeLog;
    UINT32 uiReqLog;

} DMA_MONITOR_STRUCT, *PDMA_MONITOR_STRUCT;



#endif
//@}


