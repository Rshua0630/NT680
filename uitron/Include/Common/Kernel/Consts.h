/**
    uITRON constant values.

    @file       Consts.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _CONSTS_H
#define _CONSTS_H

#ifdef __ECOS
// eCos system

#include <cyg/infra/cyg_type.h>
#include <cyg/compat/uitron/uit_type.h>

#else
// uITRON system

/**
    @addtogroup mIKernelCore
*/
//@{


/* Some Services Related Constants */
/*---------------------------------*/


/* Process related constants */

#define TTS_RUN     ( 0x01 )        /* Running          */
#define TTS_RDY     ( 0x02 )        /* Ready            */
#define TTS_WAI     ( 0x04 )        /* Wait             */
#define TTS_SUS     ( 0x08 )        /* Suspend          */
#define TTS_WAS     ( 0x0c )        /* Wait+Suspend     */
#define TTS_DMT     ( 0x10 )        /* Dormant          */

#define TTS_KRN     ( 0x20 )
#define TTS_ZMB     ( 0x40 )
#define TTS_CHG     ( 0x80 )

/* Task related constants */

#define TA_ASM      ( 0x00 )
#define TA_HLNG     ( 0x01 )
#define TA_VERS     ( 0x08 )
#define TA_COP      ( 0x10 )
#define TA_COP0     ( 0x10 )
#define TA_COP1     ( 0x20 )
#define TA_COP2     ( 0x40 )
#define TA_COP3     ( 0x80 )
#define ALL_COP     ( TA_COP0 | TA_COP1 | TA_COP2 |TA_COP3 )    /* SR320    */

#define TSK_SELF    (0)
#define TSK_SLF     TSK_SELF

/* Priority related constants */

#define TPRI_INI    ((PRI)0)
#define TPRI_RUN    ((PRI)0)
#define INI_PRI     TPRI_INI

/* Timeout related constants */

#define TMO_POL     ((TMO)0)            /* Polling option           */
#define TMO_FEVR    ((TMO)-1)           /* Endless wait option      */

/* Event flags related constants */

#define TWF_ANDW    0x00                /* AND condition            */
#define TWF_ORW     0x02                /* OR condition             */
#define TWF_CLR     0x01                /* Clear condition          */


/* Alarm/cyclic handlers related constants */

#define TCYC_OFF    0x00                /* Inactive cyclic          */
#define TCYC_ON     0x01                /* Active cyclic            */
#define TCYC_INI    0x02                /* Initialize cyclic        */

#define NADR        ((VP)-1)
#define TA_INHERIT  ((ATR)0x80)

//@}

#endif

#endif
