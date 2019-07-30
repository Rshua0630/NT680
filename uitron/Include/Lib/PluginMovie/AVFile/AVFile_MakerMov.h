/**
    Header file of media fileformat writing library

    Exported header file of media fileformat writing library.

    @file       AVFile_MakerMov.h
    @ingroup    mIMEDIAWRITE
    @note       Nothing.
    @version    V1.00.000
    @date       2013/01/31

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _AVIFILEMAKERMOV_H
#define _AVIFILEMAKERMOV_H

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
extern PCONTAINERMAKER mov_getContainerMaker(void);

/**
    Register callback for mov-container-maker.

    Register callback for mov-container-maker.

    @return
     - @b E_OK:     Register successfully.
     - @b E_NOSPT:  Wrong checkID.
*/
extern ER MovWriteLib_RegisterObjCB(void *pobj);
extern void mov_ResetContainerMaker(void);
extern ER MovWriteLib_ChangeWriteCB(void *pobj);

//@}

#endif//_AVIFILEMAKERMOV_H

//@}

