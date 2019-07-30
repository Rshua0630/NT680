/**
    JPEG header information.

    @file       JpgHeader.h
    @ingroup    mIUserJPEG

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

*/
#ifndef _JPEHEADER_H
#define _JPEHEADER_H

#include "Type.h"

/**
    @addtogroup mIUserJPEG
*/
//@{

/**
     @name JPEG header size.
*/
//@{
#define JPG_HEADER_SIZE    sizeof(JPG_HEADER) ///< Refer to #_JPG_HEADER.
//@}

/**
    Huffman table DC
*/
typedef _PACKED_BEGIN struct _HUFF_TBL_DC {
	UINT8    TblIndex;          ///< table index
	UINT8    symbol_nums[16];   ///< symbol numbers
	UINT8    symbol_data[12];   ///< symbol data
} _PACKED_END HUFF_TBL_DC;

/**
    Huffman table AC
*/
typedef _PACKED_BEGIN struct _HUFF_TBL_AC {
	UINT8    TblIndex;          ///< table index
	UINT8    symbol_nums[16];   ///< symbol numbers
	UINT8    symbol_data[162];  ///< symbol data
} _PACKED_END HUFF_TBL_AC;

/**
    quantization table
*/
typedef _PACKED_BEGIN struct _Q_TABLE {
	UINT8    TblIndex;          ///< table index
	UINT8    q_data[64];        ///< quantization data
} _PACKED_END Q_TABLE;

/**
    component info of SOF
*/
typedef _PACKED_BEGIN struct _SOF_COMPINFO {
	UINT8    ComponentID;        ///< component ID
	UINT8    SampleHVRate;       ///< Horizontal & Vertical rate
	UINT8    ComponentQTblIndex; ///< Q-table index
} _PACKED_END SOF_COMPINFO;

/**
    component info of SOS
*/
typedef _PACKED_BEGIN struct _SOS_COMPINFO {
	UINT8    ComponentID;             ///< component ID
	UINT8    ComponentHufTblIndex;    ///< AC & DC Huffman table index
} _PACKED_END SOS_COMPINFO;

/**
    DHT tag
*/
typedef _PACKED_BEGIN struct _TAG_DHT {
	UINT8           TagID[2];         ///< tag ID
	UINT8           TagLength[2];     ///< tag length
	HUFF_TBL_DC    HuffDC_0th;        ///< 00->0th DC Huffman table (Y)
	HUFF_TBL_AC    HuffAC_0th;        ///< 10->0th AC Huffman table (Y)
	HUFF_TBL_DC    HuffDC_1th;        ///< 01->1th DC Huffman table (UV)
	HUFF_TBL_AC    HuffAC_1th;        ///< 11->1th AC Huffman table (UV)
} _PACKED_END TAG_DHT;    // Huffman tables

/**
    DQT tag
*/
typedef _PACKED_BEGIN struct _TAG_DQT {
	UINT8           TagID[2];       ///< tag ID
	UINT8           TagLength[2];   ///< tag length
	Q_TABLE        Qtbl_0th;        ///< 00: 0th Q-table
	Q_TABLE        Qtbl_1th;        ///< 01: 1th Q-table
} _PACKED_END TAG_DQT;    // Quantization tables

/**
    SOF tag
*/
typedef _PACKED_BEGIN struct _TAG_SOF {
	UINT8           TagID[2];       ///< tag ID
	UINT8           TagLength[2];   ///< tag length
	UINT8           BitsPerPixel;   ///< bits per pixel per color component
	UINT8           ImgHeight[2];   ///< image height
	UINT8           ImgWidth[2];    ///< image width
	UINT8           ComponentNums;  ///< number of color components
	SOF_COMPINFO    Component1;     ///< component 1
	SOF_COMPINFO    Component2;     ///< component 2
	SOF_COMPINFO    Component3;     ///< component 3
} _PACKED_END TAG_SOF;    // Start of Frame

/**
    SOS tag
*/
typedef _PACKED_BEGIN struct _TAG_SOS {
	UINT8           TagID[2];       ///< tag ID
	UINT8           TagLength[2];   ///< tag length
	UINT8           ComponentNums;  ///< number of color components
	SOS_COMPINFO    Component1;     ///< component 1
	SOS_COMPINFO    Component2;     ///< component 2
	SOS_COMPINFO    Component3;     ///< component 3
	UINT8           OtherData[3];
} _PACKED_END TAG_SOS;    // Start of Scan

/**
    JPEG header
*/
typedef _PACKED_BEGIN struct _JPG_HEADER {
	UINT16        SOI;              ///< SOI marker
	TAG_DQT       MarkDQT;          ///< DQT marker
	TAG_SOF       MarkSOF;          ///< SOF marker
	TAG_DHT       MarkDHT;          ///< DHT marker
	TAG_SOS       MarkSOS;          ///< SOS marker
} _PACKED_END JPG_HEADER, *PJPG_HEADER;

/**
    YUV format
*/
typedef enum _JPG_YUV_FORMAT {
	JPG_FMT_YUV211  = 0,   ///< YCbCr 2h11
	JPG_FMT_YUV422  = 0,   ///< YCbCr 2h11
	JPG_FMT_YUV420  = 1,   ///< YCbCr 411
	JPG_FMT_YUV444  = 2,   ///< YCbCr 411
	JPG_FMT_YUV211V = 3,   ///< YCbCr 2v11
	JPG_FMT_YUV100  = 4,   ///< YCbCr 100 (Y plane only)
	ENUM_DUMMY4WORD(JPG_YUV_FORMAT)
} JPG_YUV_FORMAT;

/**
    pixel layout
*/
typedef enum _JPG_PXL_LAYOUT {
	JPG_PXL_UVPACKED = 0,   ///< UV packed
	JPG_PXL_PLANAR,         ///< YUV planar
	ENUM_DUMMY4WORD(JPG_PXL_LAYOUT)
} JPG_PXL_LAYOUT;


/**
    Decode JPEG type.

    One jpeg file contains one primary image, one 160x120 thumbnail image,
    and one screennail image.
    For MJPEG AVI file, the frame inside also a JPEG file.
*/
typedef enum _JPG_DEC_TYPE {
	DEC_PRIMARY,        ///< decode primary image
	DEC_THUMBNAIL,      ///< decode thumbnail, or decode primary image
	DEC_HIDDEN,         ///< decode hidden thumb, or decode primary image
	DEC_THUMB_ONLY,     ///< decode thumbnail only
	DEC_MPO             ///< decode MPO
} JPG_DEC_TYPE;

/**
   Error Code.

   Error code for parsing JPG header.
*/
typedef enum _JPG_HEADER_ER {
	JPG_HEADER_ER_OK        =   0x0000,      ///< JPEG decode OK
	JPG_HEADER_ER_MARKER    =   0xFFFF,      ///< invalid or reserved marker
	JPG_HEADER_ER_SOI_NF    =   0xD8FF,      ///< SOI marker not found
	JPG_HEADER_ER_DQT_LEN   =   0xDB00,      ///< invalid DQT segment length
	JPG_HEADER_ER_DQT_TYPE  =   0xDB01,      ///< unknown quant table type
	JPG_HEADER_ER_SOF_NBL   =   0xC1CF,      ///< encoding mode is not baseline
	JPG_HEADER_ER_SOF_P     =   0xC004,      ///< sample precision != 8
	JPG_HEADER_ER_SOF_NC    =   0xC005,      ///< num components/frame != 3
	JPG_HEADER_ER_SOF_SFY   =   0xC005,      ///< MCU format != YUV420/422
	JPG_HEADER_ER_DHT_LEN   =   0xC400,      ///< invalid DHT segment length
	JPG_HEADER_ER_DHT_TYPE  =   0xC401,      ///< unknown huffman table type
	JPG_HEADER_ER_SOS_NF    =   0xDAFF,      ///< SOS marker not found
	JPG_HEADER_ER_RST_NF    =   0xD0FF,      ///< RST marker not found
	JPG_HEADER_ER_EOI_NF    =   0xD9FF,      ///< EOI marker not found
	JPG_HEADER_ER_APP1      =   0xE1FF,      ///< EOI marker not found
	JPG_HEADER_ER_AVI_NF    =   0x5249,      ///< AVI marker not found
	JPG_HEADER_ER_APP2      =   0xE2FF,      ///< APP2 error
	ENUM_DUMMY4WORD(JPG_HEADER_ER)
} JPG_HEADER_ER;


/**
    JPEG decode configuration.
*/
typedef struct _JPGHEAD_DEC_CFG {
	UINT32  headerlen;          ///< [out]length of header
	UINT8   *inbuf;             ///< [in]input buffer start address
	UINT8   *pQTabY;            ///< [out]start addr of 0th-Quantization table
	UINT8   *pQTabUV;           ///< [out]start addr of 1st-Quantization table
	UINT8   *pQTabUV2;          ///< [out]start addr of 2nd-Quantization table
	UINT8   *pHuffDC0th;        ///< [out]start addr of 0th-Huff-DC table
	UINT8   *pHuffDC1th;        ///< [out]start addr of 1st-Huff-DC table
	UINT8   *pHuffAC0th;        ///< [out]start addr of 0th-Huff-AC table
	UINT8   *pHuffAC1th;        ///< [out]start addr of 1st-Huff-AC table
	UINT8   *pTagSOF;           ///< [out]start addr of SOF (image-width, image-height, image-format)
	UINT16  imageheight;        ///< [out]image height
	UINT16  imagewidth;         ///< [out]image width
	UINT16  numcomp;            ///< [out]number of components
	UINT16  scanfreq[3][2];     ///< [out]h and v freqs for each of 3 components
	UINT16  qtype[3];           ///< [out]quantization table type for each component
	JPG_YUV_FORMAT  fileformat; ///< [out]file format, refer to JPG_YUV_FORMAT
	JPG_PXL_LAYOUT  PxlLayout;  ///< [out]raw data layout, refer to JPG_PXL_LAYOUT
	UINT16  rstinterval;        ///< [out]restart interval (initially set to 0)
	UINT16  lineoffsetY;        ///< [out]Y  line offset after scaling
	UINT16  lineoffsetUV;       ///< [out]UV line offset after scaling
	UINT16  ori_imageheight;    ///< [out]ori-image height before scaling
	UINT16  ori_imagewidth;     ///< [out]ori-image width  before scaling
	BOOL    bIfSpeedUpSN;       ///< [in]speed up Screennail
} JPGHEAD_DEC_CFG, *PJPGHEAD_DEC_CFG;

/**
    Parse JPEG header and get information from JPEG header.

    @param[in,out] jdcfg_p JPEG decoding info: #_JPGHEAD_DEC_CFG.
    @param[in] DecType JPEG decoding type: #_JPG_DEC_TYPE.
    @return JPEG decoding error code: #_JPG_HEADER_ER.
*/
extern JPG_HEADER_ER Jpg_ParseHeader(JPGHEAD_DEC_CFG *jdcfg_p, JPG_DEC_TYPE DecType);


/**
    Check if the image format supported.

    @param[in] usNumComp - Component number
    @param[in] usScanFreq[3][2] - Component format
    @param[out] pusFileFormat - Supported format
    @return result
        - @b FALSE - Format not support
        - @b TRUE - Format support
*/
extern BOOL Jpg_IsSupportedFormat(UINT16 usNumComp, UINT16 usScanFreq[3][2], JPG_YUV_FORMAT *pusFileFormat);


/**
    Get pixel layout format.

    @return Pixel layout format: #_JPG_PXL_LAYOUT.
*/
extern JPG_PXL_LAYOUT Jpg_GetPxlLayout(void);
//@}
#endif//_JPEHEADER_H
