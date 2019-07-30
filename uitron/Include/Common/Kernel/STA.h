/*
    uITRON state related header file.

    @file       STA.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _STA_H
#define _STA_H

#ifdef __ECOS
// eCos system

#else
// uITRON system

/*
 *  General Code Bits
*/
#define CNEXST  ( 0x80 )                /* Code not existent                */
#define CDORMT  ( 0x40 )                /* Code non-existant                */
#define CZOMBI  ( CNEXST+CDORMT )       /* Code dormant                     */
#define CSUSPD  0x20                    /* Code suspended                   */
#define CINKER  0x10                    /* Code in kernel                   */

/*
 *  Ready & State Changing Code
*/
#define CREADY  0x08                    /* Code ready                       */
#define CCHANG  0x00                    /* Code state changing              */

/*
 *  Waiting State Codes
*/
#define CWFSEM  0x01                    /* Code wait for sem                */
#define CWFFLG  0x02                    /* Code wait for flg                */
#define CWFMBX  0x03                    /* Code wait for mbx                */
#define CWFFPL  0x04                    /* Code wait for fpl                */
#define CWFVPL  0x05                    /* Code wait for vpl                */

#define CWFWUP  ( CREADY+0x03 )         /* Code wait for wup                */

/*
 *  Waiting  Mask
*/
#define CWAITG  0x07                    /* Waiting mask                     */

/*
 *  The following states are assigned directly
*/
#define SNEXST  CNEXST                  /* State not existent               */
#define SDORMT  CDORMT                  /* State dormant                    */
#define SZOMBI  CZOMBI                  /* State zombie                     */
#define SSUSPD  CSUSPD                  /* State suspended                  */

/*
 *  Ready States - keep CINKER bit
*/
#define SUREDY  CREADY                  /* User-ready                       */
#define SKREDY  ( CINKER+CREADY )       /* Kernel-ready                     */

/*
 *  State Changing States - keep CINKER bit
*/
#define SUCHNG  CCHANG                  /* User-state-changing              */
#define SKCHNG  ( CINKER+CCHANG )       /* Kernel-state-changing            */

/*
 *  Waiting States - have CINKER+CWAITG bits
*/
#define SKWWUP  ( CINKER+CWFWUP )       /* State wait for wup               */

/*
 *  Types Of Kernel Queues
*/
#define TYP_RDY CREADY                  /* Ready queue type                 */
#define TYP_SEM ( CINKER+CWFSEM )       /* Semaphore queue type             */
#define TYP_FLG ( CINKER+CWFFLG )       /* Event flag queue type            */
#define TYP_MBX ( CINKER+CWFMBX )       /* Mailbox queue type               */
#define TYP_FPL ( CINKER+CWFFPL )       /* Fixpool queue type               */
#define TYP_VPL ( CINKER+CWFVPL )       /* Varpool queue type               */

#endif

#endif
