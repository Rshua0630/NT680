/**
    Header file of Movie Stamp API

    Exported API of Movie Stamp functions.

    @file       MovieStampAPI.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef _MOVIESTAMPAPI_H
#define _MOVIESTAMPAPI_H

#include "Type.h"

/**
    @addtogroup
*/
//@{

/**
     Install movie stamp task, flag and semaphore id.

     Install movie stamp task, flag and semaphore id.

     @return void
*/
extern void MovieStamp_InstallID(void) _SECTION(".kercfg_text");

/**
    @name   Movie Stamp Function API
*/
//@{

/**
    Movie Stamp Callback Function

    The prototype of callback function for movie stamp data update

    @return void
*/
typedef void (*MOVIESTAMP_UPDATE_CB)(void);

/*
    Open movie stamp task.

    Open movie stamp task.

    @param[in] void

    @return
     - @b E_OK:     open successfully.
     - @b E_SYS:    movie stamp task is already open.
*/
extern ER   MovieStampTsk_Open(void);

/**
    Close movie stamp task.

    Close movie stamp task.

    @return
     - @b E_OK:     close successfully.
     - @b E_SYS:    movie stamp task is closed.
*/
extern ER   MovieStampTsk_Close(void);

/**
    Trigger movie stamp update.

    Trigger movie stamp update.

    @return void
*/
extern void MovieStampTsk_TrigUpdate(void);

/**
    To register movie stamp update callback function.

    To register movie stamp update callback function.

    @return void
*/
extern void MovieStampTsk_RegUpdateCB(MOVIESTAMP_UPDATE_CB fpMovieStampUpdate);

//@}
//@}
extern BOOL MovieStampTsk_IsOpen(void);


#endif
