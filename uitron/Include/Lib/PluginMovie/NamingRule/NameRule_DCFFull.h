/**
    Header file of media naming rule (DCFFull) library

    Exported header file of media naming rule library.

    @file       NameRule_DCFFull.h
    @ingroup    mIMEDIAWRITE
    @note       1st version.
    @version    V1.00.001
    @date       2013/02/26

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _NAMERULEDCFFULL_H
#define _NAMERULEDCFFULL_H

/**
    @addtogroup mIMEDIAWRITE
*/
//@{

/*
    Public funtions to use fileformat maker
*/
//@{


/**
    Get DCFFull naming rule.

    Get DCFFull naming rule.

    @param[in] void

    @return
     PMEDIANAMINGRULE DCFFull naming rule
*/
extern PMEDIANAMINGRULE NameRule_getDCFFull(void);

/**
    Open namerule DCFFull debug msg. Default: off.

    @param[in] on  1 to open debug msg
    @return void
*/
extern void NameRuleDCFFull_OpenDumpMsg(UINT8 on);

//@}

#endif//_NAMERULEDCFFULL_H

//@}

