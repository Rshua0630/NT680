/****************************************************************************
**
**  Name        gki_int.h
**
**  Function    This file contains GKI private definitions
**
**
**  Copyright (c) 1999-2004, WIDCOMM Inc., All Rights Reserved.
**  Proprietary and confidential.
**
*****************************************************************************/
#ifndef GKI_INT_H
#define GKI_INT_H

#include <kernel.h>
#include "gki_common.h"
#if 0
#if defined (__CC_NORCROFT)     /* Norcroft C : ARM SDT8*/
#include    <norti3.h>          /* NORTi3 Standart Header*/
#elif defined (GAIO)            /* XCCV*/
#include    <gosv3.h>
#elif defined (USE_RTOS_RX850)
#include <stdrx850.h>
//#include <sit.h>
#else
/* #error  COMPILER TYPE            Error compiler type*/
#include <h2000.h>
#endif
#endif

#ifndef USE_CRE_TSK
#define USE_CRE_TSK
#endif
#ifndef USE_SEM_FOR_LOCK
#define USE_SEM_FOR_LOCK
#endif
/**********************************************************************
** uItron specific definitions
*/
typedef struct
{
#ifdef USE_CRE_TSK
    T_CTSK  ctsk_main[GKI_MAX_TASKS];
    ID      osids[GKI_MAX_TASKS];
    UINT32  wait_flag[GKI_MAX_TASKS];

#else
    int    ctsk_main[GKI_MAX_TASKS];
#endif
} tGKI_OS;

#define FLAG_FULL_MASK 0xFFFFFFFF
#define FLAG_WAKE_UP    FLGPTN_BIT(2)

/* Contains common control block as well as OS specific variables */
typedef struct
{
    tGKI_OS     os;
    tGKI_COM_CB com;
} tGKI_CB;


#ifdef __cplusplus
extern "C" {
#endif

#if GKI_DYNAMIC_MEMORY == FALSE
GKI_API extern tGKI_CB  _SECTION(".kercfg_data")  gki_cb;
#else
GKI_API extern tGKI_CB *gki_cb_ptr;
#define gki_cb (*gki_cb_ptr)
#endif

#ifdef __cplusplus
}
#endif

#endif
