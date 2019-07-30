/**
    Header file of media fileformat reading library

    Exported header file of media fileformat reading library.

    @file       AVFile_ParserMov.h
    @ingroup    mIMEDIAREAD
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _AVFILE_PARSERMOV_H
#define _AVFILE_PARSERMOV_H

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
extern CONTAINERPARSER *MP_MovReadLib_GerFormatParser(void);

/**
    Parse 1st Video Information

    Parse 1st Video Information

    @param[in] hdrAddr    not used.
    @param[in] hdrSize    not used.
    @param[in] pobj       MEDIA_FIRST_INFO object

    @return ER
*/
extern ER MovReadLib_Parse1stVideo(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);

/**
    Parse Thumbnail Information

    Parse Thumbnail Information

    @param[in] hdrAddr    not used.
    @param[in] hdrSize    not used.
    @param[in] pobj       MEDIA_FIRST_INFO object

    @return ER
*/
extern ER MovReadLib_ParseThumbnail(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);


/**
    Parse Video Information

    Parse Video Information

    @param[in] hdrAddr    not used.
    @param[in] hdrSize    not used.
    @param[in] pobj       MEDIA_FIRST_INFO object

    @return ER
*/
extern ER MovReadLib_ParseVideoInfo(UINT32 hdrAddr, UINT32 hdrSize, void *pobj);
/**
    Register Callback function

    Register callback function for file read and output message.

    @param[out] pobj   CONTAINERPARSER object.

    @return ER
*/
extern ER MovReadLib_RegisterObjCB(void *pobj);//pContainer = (CONTAINERPARSER *)pobj;

//@}

//@}

#endif //_AVFILE_PARSERMOV_H
