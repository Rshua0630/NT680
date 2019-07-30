/**
    Header file of media fileformat writing library

    Exported header file of media fileformat writing library.

    @file       AVFile_MakerTS.h
    @ingroup    mIMEDIAWRITE
    @note       Nothing.
    @version    V1.00.000
    @date       2013/01/31

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _AVIFILEMAKERTS_H
#define _AVIFILEMAKERTS_H

#include "MOVLib.h"

#define TS_VIDEO_FRAMERATE        30
#define TS_AUDIO_SAMPLERATE    32000
#define TS_AUDIO_SAMPLEPERFAME  1024


#define TS_VIDEOPES_HEADERLENGTH  14  //(9+PTS 5bytes)
#define TS_AUDIOPES_HEADERLENGTH  14  //(9+PTS 5bytes)
#define TS_VIDEO_NAL_LENGTH        6
#define TS_AUDIO_ADTS_LENGTH       0
#define TS_PTS_LENGTH              5

#define TS_IDTYPE_VIDPATHID        2//video pathid
#define TS_IDTYPE_AUDPATHID        3//audio pathid

/**
    @addtogroup mIMEDIAWRITE
*/
//@{

/*
    Public funtions to use fileformat maker
*/
//@{

/**
    Get ts-container-maker.

    Get ts-container-maker to make a ts file.

    @param[in] void

    @return
     PCONTAINERMAKER container maker
*/
extern PCONTAINERMAKER ts_getContainerMaker(void);

/**
    Register callback for mov-container-maker.

    Register callback for mov-container-maker.

    @return
     - @b E_OK:     Register successfully.
     - @b E_NOSPT:  Wrong checkID.
*/






//extern ER MovWriteLib_RegisterObjCB(void *pobj);
extern void ts_ResetContainerMaker(void);
//extern ER MovWriteLib_ChangeWriteCB(void *pobj);

extern void TSWriteLib_UserMakerModelData(MOV_USER_MAKERINFO *pMaker);




//@}

#endif//_AVIFILEMAKERTS_H

//@}
