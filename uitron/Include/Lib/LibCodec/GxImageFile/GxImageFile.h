/**
    GxImageFile module.

    For JPEG file encoding or decoding.

    @file       GxImageFile.h
    @ingroup    mIGxImageFile

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _GX_IMAGEFILE_H_
#define _GX_IMAGEFILE_H_

#include "Type.h"
#include "GxImage.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mIGxImageFile
*/
//@{

/**
     @name Assign Q table.
*/
//@{
#define ASSIGN_QTABLE  0    ///< The parameter value for GxImgFile_EncodeByQuality().
//@}

/**
     @name Recommended file buffer size for parsing.
*/
//@{
#define RECOMMENDED_PARSING_FILE_SIZE        0x10000    ///< Recommanded size is 64KB.
//@}

/**
    Header information.
*/
typedef struct _GXIMG_HEADER_INFO {
	UINT32  uiImageWidth;            ///< [OUT]image width
	UINT32  uiImageHeight;           ///< [OUT]image height
	UINT32  uiLineoffsetY;           ///< [OUT]Y  line offset after scaling
	UINT32  uiLineoffsetUV;          ///< [OUT]UV line offset after scaling
	GX_IMAGE_PIXEL_FMT  fileformat;  ///< [OUT]file format, refer to GX_IMAGE_PIXEL_FMT
	UINT8   *pTagSOF;                ///< [OUT]start addr of SOF (image-width, image-height, image-format)
	void   *param[4];                ///< [OUT]reserved(param[0] is Q-table addr Y, param[1] is Q-table addr UV)
} GXIMG_HEADER_INFO, *PGXIMG_HEADER_INFO;

/**
    Scale down level.
*/
typedef enum _GXIMGDEC_SCALE_DOWN_LEVEL {
	GXIMGDEC_SCALE_DOWN_NONE = 0,      ///< no scaling
	GXIMGDEC_SCALE_DOWN_BOTH_1_2,      ///< 1/2 of width and 1/2 of height
	GXIMGDEC_SCALE_DOWN_BOTH_1_4,      ///< 1/4 of width and 1/4 of height
	GXIMGDEC_SCALE_DOWN_BOTH_1_8,      ///< 1/8 of width and 1/8 of height
	ENUM_DUMMY4WORD(GXIMGDEC_SCALE_DOWN_LEVEL)
} GXIMGDEC_SCALE_DOWN_LEVEL;

/**
    GxImageFile encoding configuration
*/
typedef enum _GX_IMGENC_CFG {
	IMGENC_CFG_QTABLE_Y = 0,            ///< Q table Y, only useful for GxImgFile_EncodeByQuality() with quality=ASSIGN_QTABLE.
	IMGENC_CFG_QTABLE_UV,               ///< Q table UV, Only useful for GxImgFile_EncodeByQuality() with quality=ASSIGN_QTABLE
	IMGENC_CFG_BRC_INITIAL_QF,          ///< initial QF value, 1 ~ 512, default is 128.
	IMGENC_CFG_BRC_LOWER_BOUND_RATIO,   ///< lower boundary ratio to target size, 1~100%, default is 15
	IMGENC_CFG_BRC_UPPER_BOUND_RATIO,   ///< upper boundary ratio to target size, 1~100%, default is 15
	IMGENC_CFG_BRC_MAX_RETRY_CNT,       ///< max re-try times, default is 5
	ENUM_DUMMY4WORD(GX_IMGENC_CFG)
} GX_IMGENC_CFG;


/**
    Parse image file to get respective JPEG information.

    @param[in] uiWholeFileSize JPEG file size.
    @param[in] pSrcFile Source file in buffer for parsing, recommended file size could refer to #RECOMMENDED_PARSING_FILE_SIZE.
    @param[out] pThumbnanilData Thumbnail data information, the offset from the start of JPEG file and the size of thumbnail image.
    @param[out] pPrimaryData Primary data information, the offset from the start of JPEG file and the size of primary image.
    @param[out] pScreennailData Screennail data information, the offset from the start of JPEG file and the size of screennail image.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_ParseJPG(UINT32 uiWholeFileSize, PMEM_RANGE pSrcFile, PMEM_RANGE pThumbnanilData, PMEM_RANGE pPrimaryData, PMEM_RANGE pScreennailData);

/**
    Parse JPEG data to get header information.

    @param[in] pSrcFile Source file in buffer for parsing.
    @param[out] pHeaderInfo Header information: #_GXIMG_HEADER_INFO.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_ParseHeader(PMEM_RANGE pSrcFile, PGXIMG_HEADER_INFO pHeaderInfo);

/**
    Allocate buffer for decoding.

    According to the available memory buffer to get proper decoding configuration.

    @param[out] pDstImgBuf Image buffer for available memory size.
    @param[in] pAvailableMem Available memory for decoding
    @param[in] pHeaderInfo Header information, from the result of GxImgFile_ParseHeader()
    @param[out] pScaleDownlevel Scale down level for decoding JPEG file into available memory.

    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_AllocDecBuf(PIMG_BUF pDstImgBuf, PMEM_RANGE pAvailableMem, PGXIMG_HEADER_INFO pHeaderInfo, GXIMGDEC_SCALE_DOWN_LEVEL *pScaleDownlevel);

/**
    Decode JPEG data.

    @param[in] pDstImgBuf Image buffer, from the result of GxImgFile_AllocDecBuf().
    @param[in] pSrcBitstream Source file in buffer for parsing.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_Decode(PIMG_BUF pDstImgBuf, PMEM_RANGE pSrcBitstream);

/**
    Decode JPEG data by down sampling.

    @param[in] pDstImgBuf Image buffer, from the result of GxImgFile_AllocDecBuf().
    @param[in] pSrcBitstream Source file in buffer for parsing.
    @param[in] ScaleDownlevel Scale down level.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_DecodeByDownsample(PIMG_BUF  pDstImgBuf, PMEM_RANGE pSrcBitstream, GXIMGDEC_SCALE_DOWN_LEVEL ScaleDownlevel); //support assign downsample level version


/**
    Config decoding/encodeing parameters.

    In order to change some default setting, one could uses this API.

    @param[in] Item Item to config: #_GX_IMGENC_CFG.
    @param[in] uiValue The value of the Item.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_Config(GX_IMGENC_CFG Item, UINT32 uiValue);

/**
    Encode image to JPEG data by the mechanism of constant quality.

    @param[in] pSrcImgBuf Image buffer for encoding.
    @param[in,out] pDstBitstream [in]Available memory for JPEG data, [out]real output JPEG size.
    @param[in] uiQuality Quality should be in the range of 1 to 100, and 100 is the best quality.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_EncodeByQuality(PIMG_BUF pSrcImgBuf, PMEM_RANGE pDstBitstream, UINT32 uiQuality);

/**
    Calculate compression ratio.

    @param[in] pSrcImgBuf Image buffer for encoding.
    @param[in] uiTargetSize Target size.
    @return Compression ratio, normalized to 100%
*/
extern UINT32 GxImgFile_CalcCompressedRatio(PIMG_BUF pSrcImgBuf, UINT32 uiTargetSize);

/**
    Encode image to JPEG data by the mechanism of constant bit rate.

    The default settings of bit rate control are the followings.\n
    Initail QF = 128.\n
    Lower boundary ratio = 15.\n
    Upper boundary ratio = 15.\n
    Max retried count = 5.

    @param[in] pSrcImgBuf Image buffer for encoding.
    @param[in,out] pDstBitstream [in]Available memory for JPEG data, [out]real output JPEG size.
    @param[in] uiCompressedRatio compression ratio, normalized to 100%.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_OBJ:  Reach BRC limitation or retry count, the bitstream might be correct in BRC mode.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_EncodeByBitrate(PIMG_BUF pSrcImgBuf, PMEM_RANGE pDstBitstream, UINT32 uiCompressedRatio);

/**
    Combine separated JPEG data, such as thumbnail, screennail, and primary, into a JPEG file

    @note This API will mainly use primary data to joint the other JPEG data,\n
    so user should reserve the exif data size prior to primary data, and the screennail size after primary data.
    @param[in] pExifData JPEG exif header, including thumbnail bitstream.
    @param[in] pPrimaryData Primary JPEG data.
    @param[in] pScreennailData Screennail JPEG data.
    @param[out] pDstJpgFile Combined JPEG file.
    @return
        - @b ER_OK:  Succeed.
        - @b ER_SYS:  Failed.
*/
extern ER GxImgFile_CombineJPG(PMEM_RANGE pExifData, PMEM_RANGE pPrimaryData, PMEM_RANGE pScreennailData, PMEM_RANGE pDstJpgFile);
//@}

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GX_IMAGEFILE_H_
