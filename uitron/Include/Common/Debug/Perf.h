/**
    Performance time mesasure

    Here provide 3 kinds of measure time. First is single measure, just measure
    an item cost time. Second that allows many items is to record each begin and
    end time. Third that also allows many items is to calculate total coverage time
    per item.

    @file       Perf.h
    @ingroup    mISYSDebug

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.
*/

#ifndef _PERF_H
#define _PERF_H

#include "Type.h"
#include "Debug.h"

/**
     @name Single item measure time
     @note only can one item be measure.
*/
//@{
#define PERF_PER_SEC 1000000Lb ///< the mesaure/PERF_PER_SEC equals to second unit
#define PERF_PER_MS  1000L     ///< the mesaure/PERF_PER_MS equals to milli unit

/**
    Open and start a performance measurement.

    This function open and start a timer for performance measurement.

    @note Becasue this performance measurement will use one hardware timer after Perf_Open(),
        be sure to close by calling Perf_Close() in order to release the allocated timer
        for others.

    @return E_OK if success, E_SYS if already opened or timer is not available.
*/
extern ER       Perf_Open(void);

/**
    Check if performance measurement is opended or not.

    Check if performance measurement is opended or not.

    @return TRUE: Opened. FALSE: Closed.
*/
extern BOOL     Perf_IsOpened(void);

/**
    Close and stop a performance measurement.

    This function close the timer used for performance measurement,

    @return E_OK if success, E_SYS if performance timer is not opened.
*/
extern ER       Perf_Close(void);

/**
    Mark current tick.

    This function mark current tick as a performance check point.
    Current tick start from Perf_Open() is also returned.

    @return current tick count from Perf_Open() to return. In unit of us.
*/
extern UINT32   Perf_Mark(void);

/**
    Get tick duration from last mark.

    This function get the difference ticks between current tick time and last time
    Perf_Mark() called.

    @return tick count difference from last Perf_Mark() to return. In unit of us.
*/
extern UINT32   Perf_GetDuration(void);

/**
    Get current tick from start.

    This function get the difference ticks between current tick time and last time
    Perf_Open() called.

    @return tick count difference from last Perf_Open() to return. In unit of us.
*/
extern UINT32   Perf_GetCurrent(void);
//@}

/**
     @name TM API global timemark record - used to recording system flow timing
     @note \n
        Pros: \n
            1.No index allot problem, using string name, simplfy Add() process \n
            2.using 1st name as begin time, 2nd name as end time, these 2 time can be sorting at Dump() post-process \n
        Cons: \n
            1.only can record each mark once \n
*/
//@{


/**
     Time mark element buffer

     For each item with begin and end time, there are 2 TIMEMARKs are required.
     If n items should be measured, user should prepare TIMEMARK TmBuf[n*2];
     These buffer have to initilize via TM_Init.

     @code
     static TIMEMARK tmbuf[4*2]; //total 4 items seprate to 2 modules
     TM_Init(tmbuf,4*2);
     TM_Begin("moduleA","Item1");
     TM_Begin("moduleA","Item2");
     TM_Begin("moduleB","Item1");
     TM_Begin("moduleB","Item2");
     //do something
     TM_End("moduleA","Item1");
     TM_End("moduleA","Item2");
     TM_End("moduleB","Item1");
     TM_End("moduleB","Item2");
     @endcode
*/
typedef struct _TIMEMARK {
	CHAR   *pSzModule;  ///< moudle name
	CHAR   *pSzName;    ///< item name
	UINT32  cTime;      ///< item time of begin or end.
} TIMEMARK;

/**
    Init TIMEMARK buffer

    Before any TM_ APIs start, the TIMEMARK buffer has to be initial. User just
    declare TIMEMARK array, and send it to TM_Init.

    @param[in] buffer TIMEMARK array
    @param[in] count the TIMEMARK buffer counts
*/
extern void TM_Init(TIMEMARK *buffer, UINT32 count);

/**
    TM item begin

    Set a module name and a item name to notify TM record a starting time.

    @param[in] pStrModule a module name string
    @param[in] pStrName a item name string
*/
extern void TM_Begin(char *pStrModule, char *pStrName);

/**
    TM item end

    Set a module name and a item name to notify TM record a end time.

    @param[in] pStrModule a module name string
    @param[in] pStrName a item name string
*/
extern void TM_End(char *pStrModule, char *pStrName);

/**
    TM clear

    Clean all records saved.
*/
extern void TM_Clear(void);

/**
    TM Dump

    dump all item result of a module and show it to UART.
    @param[in] pStrModule a module name string
*/
extern void TM_Dump(char *pStrModule);



/**
    TM Value

    get TM record value.
    @return TM record value. In unit of us.
*/
extern UINT32 TM_Value(void);



#define TMPNT     debug_msg("\033[37mTM: %d, %s --- %d\033[0m\r\n",__LINE__,__func__, TM_Value()) ///< Show a color sting of line count and function name in your insert codes



//@}

/**
     @name BM API : local benchmark record - used to calculate module flow speed
     @note \n
        Pros: \n
            1.can record each mark over 2 times \n
            2.can auto calculate average speed \n
        Cons: \n
            1.need provide variable for each BM \n
*/
//@{

/**
     BM operating structure

     All BM operation based on this structure handle. User need to initialize
     this structure.
*/
typedef struct _BENCHMARK {
	CHAR   *pSzModule;  ///< Module Name, should be initialized by user
	CHAR   *pSzName;    ///< Item Name, should be initialized by user
	UINT32  nCount;     ///< internal data, just set it to 0 by user
	UINT32  nTotal;     ///< internal data, just set it to 0 by user
	UINT32  cTime;      ///< internal data, just set it to 0 by user
} BENCHMARK;

/**
    BM item begin

    Start record time at the begin program.

    @param[in] pBM BM handle.
*/
extern void BM_Begin(BENCHMARK *pBM);

/**
    BM item begin

    Stop record time at the begin program.

    @param[in] pBM BM handle.
*/
extern void BM_End(BENCHMARK *pBM);

/**
    BM item clean

    Clean all recorded data.

    @param[in] pBM BM handle.
*/
extern void BM_Clear(BENCHMARK *pBM);

/**
    BM show header

    Show a report header to uart as follow.
    @code
        >Total-Time,   Count,  Avg-Time, [Module::Func()]
        >------------------------------------------------------------
    @endcode
*/
extern void BM_Dump_Header(void);

/**
    BM Dump

    Dump the coverage time report to uart.

    @param[in] pBM BM handle.
*/
extern void BM_Dump(BENCHMARK *pBM);

//@}

#endif //  _PERF_H
