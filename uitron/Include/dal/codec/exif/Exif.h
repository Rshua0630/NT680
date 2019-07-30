/**
    Exif module

    Parse or create Exif header.

    @file       Exif.h
    @ingroup    mIEXIF

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _EXIF_H //Exif.h
#define _EXIF_H

#include "Type.h"

/**
    @addtogroup mIEXIF
*/
//@{

/**
     @name The basic definition of Exif SPEC
*/
//@{
#define MAX_APP1_SIZE           0x10000 ///< maximum size of APP1 marker
#define MAX_APP2_SIZE           0x10000 ///< maximum size of APP2 marker
#define APP1_SIZE_OFFSET        4       ///< the offset of APP1 size, which is from the start of JPEG file (FF D8 FF E1 XX XX)
#define APP1_TIFF_OFFSET_BASE   12      ///< the offset of TIFF header, which is from the start of JPEG file
//@}

/**
   Exif error Code.
*/
typedef enum _EXIF_ER {
	EXIF_ER_OK                =     0, ///< E_OK, no error
	EXIF_ER_SYS               = (-5),  ///< E_SYS, system error
	EXIF_ER_SOI_NF            = (-6),  ///< SOI marker not found
	EXIF_ER_APP1_NF           = (-7),  ///< APP1 marker not found
	EXIF_ER_TAG_NUM_OV        = (-8),  ///< tag number overflow
	EXIF_ER_NO_EXIF_IFD       = (-9),  ///< EXIF IFD not found
	ENUM_DUMMY4WORD(EXIF_ER)
} EXIF_ER;

/**
    Command IFD index.

    Indexes for IFD information.
*/
typedef enum _EXIF_IFD {
	EXIF_IFD_0TH = 0,           ///< 0th IFD
	EXIF_IFD_EXIF,              ///< Exif IFD
	EXIF_IFD_GPS,               ///< GPS IFD
	EXIF_IFD_INTEROPERABILITY,  ///< Interoperability IFD
	EXIF_IFD_1ST,               ///< 1st IFD
	EXIF_IFD_MPO_INDEX,         ///< MP Index IFD
	EXIF_IFD_MPO_ATTR,          ///< MP Attribute IFD
	ENUM_DUMMY4WORD(EXIF_IFD)
} EXIF_IFD;

/**
    The necessary information for running a command via EXIF_SetTag().
*/
typedef struct _EXIF_SETTAG {
	EXIF_IFD uiTagIfd;          ///< [in] IFD index
	UINT16   uiTagId;           ///< [in] tag ID
	UINT16   uiTagType;         ///< [in] tag type
	UINT32   uiTagCount;        ///< [in] tag count
	UINT32   uiTagDataAddr;     ///< [in] tag data addr
	UINT32   uiTagDataLen;      ///< [in] tag data length(in bytes!!)
} EXIF_SETTAG, *PEXIF_SETTAG;

/**
    The necessary information for running a command via EXIF_GetTag().
*/
typedef struct _EXIF_GETTAG {
	EXIF_IFD uiTagIfd;      ///< [in] IFD index
	UINT16   uiTagId;       ///< [in] wanted exif id
	UINT32   uiTagDataAddr; ///< [out] tag data address
	UINT32   uiTagLen;      ///< [out] tag data length(in bytes!!)
	UINT32   uiTagOffset;   ///< [out] tag offset
} EXIF_GETTAG, *PEXIF_GETTAG;

/**
    Command MakerNote Info.
    The information of MakerNote which include screennail picture.
*/
typedef struct _MAKERNOTE_INFO {
	UINT32 uiMakerNoteAddr;    ///< MakerNote address
	UINT32 uiMakerNoteSize;    ///< MakerNote size
	UINT32 uiMakerNoteOffset;  ///< MakerNote offset, which is from the start of JPEG file
	UINT32 uiScreennailOffset; ///< Screennail offset, which is from the start of JPEG file
	UINT32 uiScreennailSize;   ///< Screennail size
} MAKERNOTE_INFO;

/**
    The necessary information for running a command via EXIF_GetInfo()/EXIF_UpdateInfo().
*/
typedef enum _EXIF_INFO {
	EXIFINFO_BYTEORDER = 0,     ///< Byte Order. (Out:BOOL, TRUE for little endian and FALSE for big endian)
	EXIFINFO_MPO_OFFSET_BASE,   ///< Offset base of MPO. (Out:UINT32)
	EXIFINFO_MAKERNOTE,         ///< MakerNote information.(Out:MAKERNOTE_INFO)
	EXIFINFO_EXIF_HEADER_SIZE,  ///< Exif header size including SOI, APP1, APP2.(Out:UINT32)
	EXIFINFO_TIFF_OFFSET_BASE,  ///< Offset base of TIFF. (Out:UINT32)
	EXIFINFO_APP1_SIZE_OFFSET,  ///< Offset of APP1 size. (Out:UINT32)
	ENUM_DUMMY4WORD(EXIF_INFO)
} EXIF_INFO;

/**
    The callback event for MakerNote process.
*/
typedef enum _EXIF_EVENT {
	CREATE_MAKERNOTE = 0,    ///<This callback event is invoked every time capture is performed.
	PARSE_MAKERNOTE,         ///<This callback event is invoked every time the image is play-backed.
	UPDATE_MAKERNOTE,        ///<This callback event is invoked every time capture is performed or the image is edited by playback.
	ENUM_DUMMY4WORD(EXIF_EVENT)
} EXIF_EVENT;

/**
    The prototype of callback function.
*/
typedef ER(*FPEXIFCB)(EXIF_EVENT event, PMEM_RANGE pBuffer, UINT32 uiRetParamNum, UINT32 *pParamArray);


/**
    EXIF handle ID.

    It supports more than one EXIF data be parsed or created simultaneously, E.g. dual sensor capturing with different EXIF setting.
*/
typedef enum _EXIF_HANDLE_ID {
	EXIF_HDL_ID_1 = 0,                       ///< EXIF handle ID 1
	EXIF_HDL_ID_2,                           ///< EXIF handle ID 2
	EXIF_HDL_ID_3,                           ///< EXIF handle ID 3
	EXIF_HDL_ID_4,                           ///< EXIF handle ID 4
	EXIF_HDL_ID_5,                           ///< EXIF handle ID 5
	EXIF_HDL_ID_6,                           ///< EXIF handle ID 6
	EXIF_HDL_ID_7,                           ///< EXIF handle ID 7
	EXIF_HDL_ID_8,                           ///< EXIF handle ID 8
	EXIF_HDL_MAX_NUM,
	ENUM_DUMMY4WORD(EXIF_HANDLE_ID)
} EXIF_HANDLE_ID;



/**
     Install EXIF semaphore id
*/
extern void EXIF_InstallID(void) _SECTION(".kercfg_text");

/**
    Initialize Exif lib.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
    @param[in] pWorkBuf Available working buffer for Exif lib, recommended size is 64KB.
    @param[in] fpExifCB Callback function to complete customized MakerNote content.

*/
extern void EXIF_Init(EXIF_HANDLE_ID HandleID, PMEM_RANGE pWorkBuf, FPEXIFCB fpExifCB);

/**
    Set Exif tag.

    Set specific fields in the Image EXIF header.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
    @param[in] pTag The information to set a Exif tag: #_EXIF_SETTAG.
    @return
        - @b EXIF_ER_OK: Succeed.
        - @b EXIF_ER_SYS: Failed.
*/
extern EXIF_ER EXIF_SetTag(EXIF_HANDLE_ID HandleID, PEXIF_SETTAG pTag);

/**
    Create Exif data.

    Afte user setting customized Exif tags, this function could create SOI and APP1 data.

    @note In most case, users do NOT have to use this function. EXIF_CreateExif() is invoked every time capture is performed.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
    @param[in,out] pExifData [in]Available buffer address and size, [out]the Exif data including SOI and APP1.
    @param[in] pThumbnail JPEG data of thumbnail.
    @return
        - @b EXIF_ER_OK:  Succeed.
        - @b EXIF_ER_SYS:  Failed.
*/
extern EXIF_ER EXIF_CreateExif(EXIF_HANDLE_ID HandleID, PMEM_RANGE pExifData, PMEM_RANGE pThumbnail);

/**
    Parse Exif data.

    @note In most case, users do NOT have to use this function. EXIF_ParseExif() is invoked every time the image is play-backed.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
    @param[in] pExifData The Exif data which is usually from the start of JPEG file to the end of APP1.
    @return
        - @b EXIF_ER_OK: Succeed.
        - @b EXIF_ER_SYS: Failed.
        - @b EXIF_ER_SOI_NF: SOI marker not found
        - @b EXIF_ER_APP1_NF: APP1 marker not found
        - @b EXIF_ER_TAG_NUM_OV: Tag number overflow
        - @b EXIF_ER_NO_EXIF_IFD: EXIF IFD not found
*/
extern EXIF_ER EXIF_ParseExif(EXIF_HANDLE_ID HandleID, PMEM_RANGE pExifData);

/**
    Get Exif tag.

    Get specific tag information.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
    @param[in] pTag The information to get a Exif tag: #_EXIF_GETTAG.
    @return
        - @b EXIF_ER_OK:  Succeed.
        - @b EXIF_ER_SYS:  Failed.
*/
extern EXIF_ER EXIF_GetTag(EXIF_HANDLE_ID HandleID, PEXIF_GETTAG pTag);

/**
    Dump the parsing result.

    @note Only for debug usage.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
*/
extern void EXIF_DumpData(EXIF_HANDLE_ID HandleID);

/**
    Get specific information of Exif.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
    @param[in] Info Exif information: #_EXIF_INFO.
    @param[out] pData Data address.
    @param[out] uiNumByte Data size.
    @return
        - @b EXIF_ER_OK:  Succeed.
        - @b EXIF_ER_SYS:  Failed.
*/
extern EXIF_ER EXIF_GetInfo(EXIF_HANDLE_ID HandleID, EXIF_INFO Info, VOID *pData, UINT32  uiNumByte);

/**
    Update specific information of Exif.

    @note In most case, users do NOT have to use this function.

    @param[in] HandleID EXIF handle ID: #_EXIF_HANDLE_ID.
    @param[in] Info Exif information: #_EXIF_INFO.
    @param[out] pData Data address.
    @param[out] uiNumByte Data size.
    @return
        - @b EXIF_ER_OK:  Succeed.
        - @b EXIF_ER_SYS:  Failed.
*/
extern EXIF_ER EXIF_UpdateInfo(EXIF_HANDLE_ID HandleID, EXIF_INFO Info, VOID *pData, UINT32  uiNumByte);



/**
    Get EXIF handle ID.

    @note In most case, users do NOT have to use this function.

    @param[in] ExifDataAddr EXIF data address.
    @return EXIF handle ID.
*/
extern EXIF_HANDLE_ID  EXIF_GetHandleID(UINT32 ExifDataAddr);



//@}
#endif //_EXIF_H

