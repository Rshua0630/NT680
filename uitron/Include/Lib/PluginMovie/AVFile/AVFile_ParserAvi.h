/**
    Header file of media fileformat reading library

    Exported header file of media fileformat reading library.

    @file       AVFile_ParserAvi.h
    @ingroup    mIMEDIAREAD
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _AVFILE_PARSERAVI_H
#define _AVFILE_PARSERAVI_H

/**
    @addtogroup mIMEDIAREAD
*/
//@{

/**
    @name   Media File Container Function Instances
*/
//@{

/**
    Get media MOV file container structure

    Get media file container function structure.

    @return CONTAINERPARSER pointer
*/
extern CONTAINERPARSER *MP_AviReadLib_GerFormatParser(void);


/**
    Parse 1st Video Information

    Parse 1st Video Information

    @param[in] hdrAddr    not used.
    @param[in] hdrSize    not used.
    @param[out] pobj      MEDIA_FIRST_INFO object

    @return ER
*/
extern ER MP_AVIReadLib_Parse1stVideo(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);

/**
    Register Callback function

    Register callback function for file read and output message.

    @param[out] pobj   CONTAINERPARSER object.

    @return ER
*/
extern ER MP_AVIReadLib_RegisterObjCB(void *pobj);


//@}

//@}

#endif //_AVFILE_PARSERAVI_H

