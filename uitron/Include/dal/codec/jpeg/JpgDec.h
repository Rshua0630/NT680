/**
    Decode JPEG file.

    @file       JpgDec.h
    @ingroup    mIUserJPEG
    @brief      Jpeg decoder

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

*/

#ifndef _JPGDEC_H
#define _JPGDEC_H

#include "SysKer.h"
#include "JpgHeader.h"
#include "Type.h"

/**
    @addtogroup mIUserJPEG
*/
//@{

/**
     @name Needed data size for parsing JPEG header(including EXIF info)
*/
//@{
#define FST_READ_THUMB_BUF_SIZE         0x10000
//@}


/**
     @name Alignment for decoded image address
*/
//@{
#define JPGDEC_IMG_ADDR_ALIGN(a)           ALIGN_CEIL_4(a)
//@}

/**
    Jpeg decoder descriptor
*/
typedef struct _JPG_DEC_INFO {
	INT32(*pJPGScalarHandler)(PJPGHEAD_DEC_CFG, UINT32 *);              ///< [in]The callback function for JPEG scale handling.
	UINT8               *pSrcAddr;               ///< [in]Source bitstream address.
	UINT8               *pDstAddr;               ///< [out]Destination YUV raw image buffer address.
	UINT32              JPGFileSize;             ///< [in]Source bitstream size.
	UINT32              AVIFileSize;             ///< [in]AVI source bitstream size.
	PJPGHEAD_DEC_CFG    jdcfg_p;                 ///< [out]JPEG decode configuration parameters.
	JPG_PXL_LAYOUT      PxlLayout;               ///< [out]Raw data layout.
	UINT32              uiOutAddrY;              ///< [out]Output raw image Y buffer address in pDstAddr.
	UINT32              uiOutAddrCb;             ///< [out]Output raw image UV buffer address in pDstAddr.
	UINT32              uiOutAddrCr;             ///< reserved
	JPG_DEC_TYPE        DecodeType;              ///< [in]Decoding type, refer to JPG_DEC_TYPE.
	BOOL                bIfSpeedUpSN;            ///< [in]Speed up Screennail.
	BOOL                bEnableTimeOut;          ///< [in]Enable time out mechanism.
	void (*pJPGTimerStart)(void);                ///< [in]Timer start callback function.
	void (*pJPGTimerPause)(void);                ///< [in]Timer pause callback function.
	void (*pJPGTimerWait)(void);                 ///< [in]Timer wait callback function.
	BOOL                bEnableCrop;             ///< [in]Enable crop function.
	UINT32              uiCropStartX;            ///< [in]Coordinate x of crop area.
	UINT32              uiCropStartY;            ///< [in]Coordinate y of crop area.
	UINT32              uiCropWidth;             ///< [in]Width of crop area.
	UINT32              uiCropHeight;            ///< [in]Height of crop area.
} JPG_DEC_INFO, *PJPG_DEC_INFO;

/**
    Transform size to fit JPEG SPEC or limitation

    @param[in,out] pWidth [in]Original image width, [out]output image width acording to the prarmeter fileformat and ScalarLevel.
    @param[in,out] pHeight [in]Original image height, [out]output image height acording to the prarmeter fileformat and ScalarLevel.
    @param[in,out] pLineOftY [in]Original image lineoffset Y, [out]output image lineoffse Y acording to the prarmeter fileformat and ScalarLevel.
    @param[in,out] pLineOftUV [in]Original image lineoffset UV, [out]output image lineoffset UV acording to the prarmeter fileformat and ScalarLevel.
    @param[in] fileformat File format: #_JPG_YUV_FORMAT.
    @param[in] ScalarLevel Scale down level, 0 for non-scaled, 1 for 1/2, 2 for 1/4 and 3 for 1/8.
*/
extern void Jpg_DecTransSize(UINT16 *pWidth, UINT16 *pHeight, UINT16 *pLineOftY, UINT16 *pLineOftUV, JPG_YUV_FORMAT  fileformat, UINT32 ScalarLevel);

/**
    Decode one JPEG bitstream to image raw data.

    @note It's a synchronous API.
    @param[in] pDecJPG Decoding info: #_JPG_DEC_INFO.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_DecOneJPG(PJPG_DEC_INFO pDecJPG);

/**
    Open JPEG engine in asynchronous decoding mode.

    @note It's an asynchronous API.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncDecOpen(void);

/**
    Start to decode in asynchronous decoding mode.

    @note It's an asynchronous API.
    @param[in] pDecJPG decoding info: #_JPG_DEC_INFO.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncDecStart(PJPG_DEC_INFO pDecJPG);

/**
    Wait for JPEG to finish in asynchronous decoding mode.

    @note It's an asynchronous API.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncDecWaitDone(void);

/**
    Close JPEG engine in asynchronous decoding mode.

    @note It's an asynchronous API.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER Jpg_ASyncDecClose(void);

//@}
#endif // _JPGDEC_H
