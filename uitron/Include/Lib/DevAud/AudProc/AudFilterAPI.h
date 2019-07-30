/**
    Header file of Audio Filter / Noise Reduction API

    Exported API of Audio Filter / Noise Reduction API functions.

    @file       AudFilterAPI.h
    @ingroup    mILIBAUDFILTER
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _AUDFILTERAPI_H
#define _AUDFILTERAPI_H

#include "Type.h"
#include "audFilt.h"
#include "AudNR.h"

/**
    @addtogroup mILIBAUDFILTER
*/
//@{

/**
    @name memory reserved size of audio filter

    memory reserved size of audio filter
*/

#define AUDFLTER_RESERVE_PROCSIZE     4096    ///< audio filter/noise reduction processing reserve bufffer size (4KB)

/**
     Install AudioFilter task,flag and semaphore id.

     Install audio filter task, flag and semaphore id.

     @return void
*/

extern void AudFilter_InstallID(void) _SECTION(".kercfg_text");

/**
    @name   Audio Filter Function API
*/
//@{

/**
    Audio Filter Callback Function Description

    The prototype of callback function for audio filter configuration

    @return void
*/
typedef void (*AUDFILTER_FILTER_CB)(void);

/**
    Setup specified audio filter set.

    Setup the configuration of specified audio filter set.

    @param[in] pAudFilterConfig  The configurations of the specified audio filter.
    @param[in] FilterType        Select which of the filter is selected for configuring.
    @param[in] bEnable           - @b TRUE:  Enable the specified auido filter.
                                 - @b FALSE: Disable the specified auido filter.

    @return
     - @b TRUE:  Setup successfully.
     - @b FALSE: Setup Operation fail.
*/
extern BOOL AudFilter_SetupParams(PAUDFILT_CONFIG pAudFilterConfig, AUDFILT_SELECTION FilterType, BOOL bEnable);

/**
    Setup audio noise reduction module.

    Setup the configuration of audio noise reduction moudle.

    @param[in] ConfigID     Configuration AUDNR_CONFIG_ID Selection ID.
    @param[in] uiCfgValue   Configuration Value.

    @return
     - @b TRUE:  Setup successfully.
     - @b FALSE: Setup Operation fail.
*/
extern BOOL AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID ConfigID, UINT32 uiCfgValue);

/*
    Open audio filter task.

    Open audio filter task for audio filter / noise reduction functions.

    @param[in] uiNumChannel    the number of audio channel.

    @return
     - @b E_OK:     open successfully.
     - @b E_SYS:    audio filter task is already open.
*/
extern ER   AudFilterTsk_Open(UINT32 uiNumChannel);

/**
    Close audio filter task.

    Close audio filter task.

    @return
     - @b E_OK:     close successfully.
     - @b E_SYS:    audio filter task is closed.
*/
extern ER   AudFilterTsk_Close(void);

/**
    Wait audio filter processing done

    Wait audio filter processing done when audio buffer queue changing.

    @return void
*/
extern void AudFilterTsk_WaitQueueDone(void);

/**
    Register callback function for audio filter/noise reduction configuration

    Register callback function for configuring audio filter and noise reduction modules.

    @param[in] fpAudPostProcess    AUDFILTER_FILTER_CB callback function pointer

    @return void
*/
extern void AudFilterTsk_RegFilter(AUDFILTER_FILTER_CB fpAudPostProcess);

/**
    Trigger audio filter and noise reduction process.

    Trigger audio filter and noise reduction process by timecode hit event in audio ISR.

    @return void
*/
extern void AudFilterTsk_TrigFiltering(void);

/**
    Trigger audio filter and noise reduction process.

    Trigger audio filter and noise reduction process by timecode hit event in audio ISR.

    @note   The audio queue pointer is changed to next queue.

    @return void
*/
extern void AudFilterTsk_TrigFilteringAsBufChange(void);

/**
    Set Audio Process Address

    Set Audio Process Address

    @param[in] uiAddr    The audio process data address.

    @return void
*/
extern void AudFilter_SetProcDataAddr(UINT32 uiAddr);

/**
    Get Audio Process Address

    Get Audio Process Address

    @return The audio process data address.
*/
extern UINT32 AudFilter_GetProcDataAddr(void);

/**
    Set Audio Queue Begin Address

    Set Audio Queue Begin Address

    @param[in] uiAddr    The audio queue begin address.

    @return void
*/
extern void AudFilter_SetQueueBeginAddr(UINT32 uiAddr);

/**
    Get Audio Queue Begin Address

    Get Audio Queue Begin Address

    @return The audio queue begin address.
*/
extern UINT32  AudFilter_GetQueueBeginAddr(void);

/**
    Set Audio Queue Size

    Set Audio Queue Size

    @param[in] uiSize    The audio queue size.

    @return void
*/
extern void AudFilter_SetQueueSize(UINT32 uiSize);

/*
    Get Audio Queue Size

    Get Audio Queue Size

    @return The audio queue size.
*/
extern UINT32  AudFilter_GetQueueSize(void);

/**
    Register callback function for audio filter process done.

    Register callback function for audio filter process done.

    @param[in] fpAudProcessDone    AUDFILTER_FILTER_CB callback function pointer

    @return void
*/
extern void AudFilterTsk_RegProcessDone(AUDFILTER_FILTER_CB fpAudProcessDone);

/**
    set audio filter pause

    set audio filter pause.

    @return void
*/
extern void AudFilterTsk_AudPause(void);

/**
    get applyAddr

    get applyAddr

    @return void
*/
extern UINT32  AudFilter_GetApplyAddr(void);

/**
    get applySize

    get applySize

    @return void
*/
extern UINT32  AudFilter_GetApplySize(void);

//@}
//@}


#endif // _AUDFILTERAPI_H
