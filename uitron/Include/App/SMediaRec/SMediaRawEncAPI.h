/**
    SRaw encoder applications.

    To encode YUV raw data to JPEG during movie recording.

    @file       RawEncAPI.h
    @ingroup    mIAPPSMEDIAREC
    @note

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _SMRAWRECAPI_H
#define _SMRAWRECAPI_H

/**
    @addtogroup mIAPPSMEDIAREC
*/
//@{


/**
    @name raw encoding EVENT ID
    @note callback ID of raw encoder
*/
//@{
#define SMRAWENC_EVENT_RESULT_OK          MOVREC_RAWENC_EVENT_RESULT_OK    ///< finish normally
#define SMRAWENC_EVENT_RESULT_ERR         MOVREC_RAWENC_EVENT_RESULT_ERR   ///< encode error
#define SMRAWENC_EVENT_RESULT_WRITE_ERR   MOVREC_RAWENC_EVENT_RESULT_WRITE_ERR///< filesystem write fails
#define SMRAWENC_EVENT_RESULT_DCF_FULL    MOVREC_RAWENC_EVENT_RESULT_DCF_FULL///< DCF ID full
#define SMRAWENC_EVENT_RESULT_JPG_OK      MOVREC_RAWENC_EVENT_RESULT_JPG_OK///< jpg encoding ok
#define SMRAWENC_EVENT_EN_IMEOUT          MOVREC_RAWENC_EVENT_EN_IMEOUT    ///< enable IME output
#define SMRAWENC_EVENT_DIS_IMEOUT         MOVREC_RAWENC_EVENT_DIS_IMEOUT   ///< disable IME output
//@}

/**
    @name raw encoding results
*/
//@{
#define SMRAWENC_OK                       MOVREC_RAWENC_OK           ///< encoding successfully
#define SMRAWENC_ERR_NOT_OPEN             MOVREC_RAWENC_ERR_NOT_OPEN ///< raw encode task is not opened
#define SMRAWENC_ERR_BUSY                 MOVREC_RAWENC_ERR_BUSY     ///< raw encode task is busy
#define SMRAWENC_ERR_IMG_NOT_READY        MOVREC_RAWENC_ERR_IMG_NOT_READY///< YUV raw image is not ready
#define SMRAWENC_ERR_UNSAFE_TO_WRITE      MOVREC_RAWENC_ERR_UNSAFE_TO_WRITE///< memory may be not enough for writing JPEG file
//@}

/**
    Changeable parameters of raw encoder.

    Changeable parameters of raw encoder.
    @note
*/
typedef enum {
	SMRAWENC_JPEG_WIDTH   = 0x0,  ///< output JPEG width, p1: width(in), p2: fileid (in)
	SMRAWENC_JPEG_HEIGHT  = 0x1,  ///< output JPEG height, p1: height(in), p2: fileid (in)
	SMRAWENC_Q            = 0x2,  ///< input encoding quality, p1: Q(in, 0~100), p2: fileid (in)
	SMRAWENC_SRC_WIDTH    = 0x3,  ///< source width, p1: width(in), p2: fileid (in)
	SMRAWENC_SRC_HEIGHT   = 0x4,  ///< source height, p1: width(in), p2: fileid (in)
	ENUM_DUMMY4WORD(SMRAW_CHANGE_TYPE)
} SMRAW_CHANGE_TYPE;

typedef enum {
	SMRAW_FORMAT_PATH1 = 0x0,   ///< fileid 0
	SMRAW_FORMAT_PATH2 = 0x1,   ///< fileid 1
	ENUM_DUMMY4WORD(SMRAW_TRIGENC_PATH)
} SMRAW_TRIGENC_PATH;

typedef enum {
	SMRAW_FORMAT_ONEPATH = 0x0,   ///< with fileid SMRAW_FORMAT_PATH1/SMRAW_FORMAT_PATH2
	SMRAW_FORMAT_ALLPATH = 0x1,   ///< valid, if multi-recording
	ENUM_DUMMY4WORD(SMRAW_TRIGENC_TYPE)
} SMRAW_TRIGENC_TYPE;

/**
    Export parameters of raw encoder.

    Export parameters of raw encoder.
    @note
*/
typedef struct {
	//BOOL                bEnable;            ///< enable raw encode or not
	//UINT32              uiMemAddr;          ///< starting address for recording
	//UINT32              uiMemSize;          ///< size for recording
	//UINT32              uiMaxWidth;         ///< max encode pic width
	//UINT32              uiMaxHeight;        ///< max encode pic height
	//UINT32              uiJpegWidth;        ///< output JPEG width
	//UINT32              uiJpegHeight;       ///< output JPEG height
	UINT32              uiEncQ;             ///< input encoding Q
	//RAW_FORMAT_TYPE     uiRawFormat;        ///< raw format, RAW_FORMAT_444 or others
	//RawEncCallbackType  *CallBackFunc;      ///< event inform callback, remove 2012/08/30
} SMRAW_ENC_DATA;

typedef struct {
	UINT32              uiWidth;            ///< image width
	UINT32              uiHeight;           ///< image height
	UINT32              uiYAddr;            ///< image Y address
	UINT32              uiUVAddr;           ///< image UV address
	UINT32              uiYLineOffset;      ///< image Y line-offset
	UINT32              uiUVLineOffset;     ///< image UV line-offset
} SMRAW_ENC_YUV;

//
//  Export APIs
//

/**
    Change raw encoding parameters.

    Change raw encoding parameters, such as width, height, encoding Q value.
    @note

    @param[in] type parameter type
    @param[in] p1 parameter1
    @param[in] p2 parameter2
    @param[in] p3 parameter3
    @return
        - @b E_OK:  change successfully.
*/
extern ER SMRawEnc_ChangeParameter(SMRAW_CHANGE_TYPE type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Trigger raw encoding.

    Trigger YUV raw encoding to output JPEG file.
    @note

    @return
        - @b SMRAWENC_OK:                     encoding successfully
        - @b SMRAWENC_ERR_NOT_OPEN:           raw encode task is not opened
        - @b SMRAWENC_ERR_BUSY:               raw encode task is busy
        - @b SMRAWENC_ERR_IMG_NOT_READY:      YUV raw image is not ready
        - @b SMRAWENC_ERR_UNSAFE_TO_WRITE:    memory may be not enough for writing JPEG file

    Example:
    @code
    {
        // Example for applying raw encoding function
        SMRawEnc_ChangeParameter(RAWENC_JPEG_WIDTH,  1920, 0, 0);
        SMRawEnc_ChangeParameter(RAWENC_JPEG_HEIGHT, 1080, 0, 0);
        SMRawEnc_TriggerEncode();
    }
    @endcode
*/
extern INT32 SMRawEnc_TriggerEncode(SMRAW_TRIGENC_TYPE type, UINT32 pathID);

/**
    Get last rawencoding JPG data.

    @param[in] puiAddr output JPG addr
    @param[in] puiSize output JPG size
    @param[in] fileID 0:path0, 1:path1
*/
extern void SMRawEnc_GetJpgData(UINT32 *puiAddr, UINT32 *puiSize, UINT32 fileid);

//extern void     SMRawEnc_GetScreenYUV(SMRAW_ENC_YUV *pYuv);

//@}
#endif //_SMRAWRECAPI_H
