/**
    Header file of media naming rule (_FileDB) library

    Exported header file of media naming rule library.

    @file       NameRule_Empty.h
    @ingroup    mIMEDIAWRITE
    @note       1st version.
    @version    V1.00.001
    @date       2015/06/01

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _NAMERULEEMPTY_H
#define _NAMERULEEMPTY_H

/**
    @addtogroup mIMEDIAWRITE
*/
//@{

/*
    Public funtions to use fileformat maker
*/
//@{
#define NAMEHDL_EMPTY_CHECKID  0x656d7074 ///< 'empt' as checkID

/**
    Get Emtpy naming rule.

    Get Emtpy naming rule.

    @param[in] void

    @return
     PMEDIANAMINGRULE Emtpy naming rule
*/
extern PMEDIANAMINGRULE NameRule_getEmpty(void);

/**
    Open Emtpy naming rule debug msg.

    Open Emtpy naming rule debug msg.

    @param[in] on on/off

    @return void
*/
extern void NH_Empty_OpenMsg(UINT8 on);

//@}

#endif//_NAMERULEEMPTY_H

//@}


