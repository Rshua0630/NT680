/**
    Header file of media fileformat writing library

    Exported header file of media fileformat writing library.

    @file       AVFile_MakerAvi.h
    @ingroup    mIMEDIAWRITE
    @note       Nothing.
    @version    V1.00.000
    @date       2013/01/31

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _AVIFILEMAKERAVI_H
#define _AVIFILEMAKERAVI_H

/**
    @addtogroup mIMEDIAWRITE
*/
//@{

/*
    Public funtions to use fileformat maker
*/
//@{

/**
    Get mov-container-maker.

    Get mov-container-maker to make a mov file.

    @param[in] void

    @return
     PCONTAINERMAKER container maker
*/
extern PCONTAINERMAKER avi_getContainerMaker(void);
extern void avi_ResetContainerMaker(void);

/**
    Register callback for mov-container-maker.

    Register callback for mov-container-maker.

    @return
     - @b E_OK:     Register successfully.
     - @b E_NOSPT:  Wrong checkID.
*/
extern ER MP_AviWriteLib_RegisterObjCB(void *pobj);
extern ER MP_AviWriteLib_ChangeWriteCB(void *pobj);
//@}

#endif//_AVIFILEMAKERAVI_H

//@}

