/**
    Public header file for extend lib

    This file is the header file that define the API and data type for dis interface.

    @file       dis_api.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _DIS_API_H
#define _DIS_API_H

/**
    dis compensation flow

    @param[in] buf workinh buf
    @param[in] cachebuf catch buffer
    @param[in] Only_CalcVector only do vector calculation, do not compensate in ipl
    @return none
 */
extern void dis_compensation(MEM_RANGE *buf, MEM_RANGE *cachebuf, BOOL Only_CalcVector);
extern void DIS_InstallCmd(void);

//@}
#endif
