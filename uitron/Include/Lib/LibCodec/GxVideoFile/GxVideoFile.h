/**
    Header file of Video File Play API

    Exported API of Video 1st-Frame and Thumbnail Play.

    @file       GxVideoFile.h
    @ingroup    mILIBGXVIDFILE
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _GX_VIDEOFILE_H_
#define _GX_VIDEOFILE_H_

#include "Type.h"
#include "GxImage.h"
#include "MediaReadLib.h"
#include "AVFile_ParserMov.h"
#include "AVFile_ParserAvi.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mILIBGXVIDFILE
*/
//@{
/**
    @name Buffer size of video header parser

    Buffer size of video header parser

    @note Refrence by calling GxVidFile_Query1stFrameWkBufSize()
*/
//@{
#define GXVIDEO_PARSE_HEADER_BUFFER_SIZE     0x3E8000
//@}
/**
    @name SMedia Video Decoder H265 Nal Unit Type Max Buffer Size

    SMedia video decoder h265 nal unit type max buffer size
    (please see definition "DecNalBufSize" in h265dec_header.c)
*/
//@{
#define GXVIDEO_H265_NAL_MAXSIZE             512           ///< H265 Nal unit type max buffer size
//@}


#define GXVIDEO_PARSE_TS_THUMB_BUFFER_SIZE   0x250000

#define GXVIDEO_TSDEMUX_MEM_OFFSET           0x100000
#define GXVIDEO_TSDEMUX_THUMB_OFFSET         0x200000
/**
    Error Code definition

    @note Returned error code by GxVideoFile function API.
*/
typedef enum _GXVID_ER {
	GXVIDEO_PRSERR_OK               =     0,    ///< Parser OK
	GXVIDEO_PRSERR_SYS              = (-2),     ///< function error
	GXVIDEO_PRSERR_AVI_NOTRIFF      = (-5),     ///< Not four-character code id: FOURCC_RIFF (AVI Header)
	GXVIDEO_PRSERR_AVI_NOTAVI       = (-6),     ///< Not four-character code id: FOURCC_AVI (AVI Header)
	GXVIDEO_PRSERR_AVI_NOLIST       = (-7),     ///< No LIST chunk exist (AVI Header)
	GXVIDEO_PRSERR_AVI_NOxxrl       = (-8),     ///< No four-character code id: FOURCC_HDRL or FOURCC_STRL (AVI Header)
	GXVIDEO_PRSERR_AVI_NOstrf       = (-9),     ///< Not four-character code id: FOURCC_STRF (AVI Header)
	GXVIDEO_PRSERR_AVI_NOTavih      = (-10),    ///< Not four-character code id: FOURCC_avih (AVI Header)
	GXVIDEO_PRSERR_AVI_NOTstrh      = (-11),    ///< Not four-character code id: FOURCC_strh (AVI Header)
	GXVIDEO_PRSERR_AVI_NOTxxds      = (-12),    ///< Not four-character code id: FOURCC_VIDS or FOURCC_AUDS (AVI Header)
	GXVIDEO_PRSERR_AVI_vids         = (-13),    ///< Parse four-character code FOURCC_VIDS error (AVI Header)
	GXVIDEO_PRSERR_AVI_auds         = (-14),    ///< Parse four-character code FOURCC_AUDS error (AVI Header)
	GXVIDEO_PRSERR_AVI_NOmovi       = (-15),    ///< No movi chunk (AVI Header)
	GXVIDEO_PRSERR_AVI_NOidx1       = (-16),    ///< No idx1 chunk (AVI Header)
	ENUM_DUMMY4WORD(GXVID_ER)
} GXVID_ER;

/**
    Struture of Audio Stream information

    @note Returned by GxVidFile_ParseVideoInfo()
*/
typedef struct _GXVIDEO_AUDINFO {
	UINT32           uiSR;                ///<[out] Audio sample rate information
	UINT32           uiChs;               ///<[out] Audio channel information
	UINT32           uiType;              ///<[out] Audio codec type information
} GXVIDEO_AUDINFO, *PGXVIDEO_AUDINFO;

/**
    Structure of Video File information

    @note Returned by GxVidFile_ParseVideoInfo()
*/
typedef struct _GXVIDEO_INFO {
	UINT32           uiTotalFrames;          ///<[out] Total Video Frames
	UINT32           uiToltalSecs;           ///<[out] Total Seconds
	UINT32           uiVidWidth;             ///<[out] Video Width
	UINT32           uiVidHeight;            ///<[out] Video Height
	UINT32           uiVidRate;              ///<[out] Video Frame Rate
	UINT64           uiUserDataAddr;         ///<[out] User Data Position in File
	UINT32           uiUserDataSize;         ///<[out] User Data Size
	UINT64           uiFre1DataAddr;         ///<[out] Fre1 Data Position in File
	UINT32           uiFre1DataSize;         ///<[out] Fre1 Data Size
	UINT64           uiThumbOffset;          ///<[out] Thumbnail offset, from the start of video file
	UINT32           uiThumbSize;            ///<[out] Thumbnail size.
	UINT32           uiVidRotate;            ///<[out] Video Rotation information
	UINT32           uiVidType;              ///<[out] Video codec type information
	UINT32           uiH264DescAddr;         ///<[out] H264 SPS/PPS data address
	UINT32           uiH264DescSize;         ///<[out] H264 SPS/PPS data size
	UINT32           uiMovCreateTime;        ///<[out] Create time for MOV file
	UINT32           uiMovModificationTime;  ///<[out] Modification time for MOV file
	VOID            *pHeaderInfo;            ///<[out] Header information (MEDIA_AVIHEADER)
	GXVIDEO_AUDINFO  AudInfo;                ///<[out] Audio stream information
	UINT32           uiDispWidth;            ///<[out] Display Width
	UINT32           uiDispHeight;           ///<[out] Display Height
	UINT64           uiScraOffset;           ///<[out] Screennail offset, from the start of video file
	UINT32           uiScraSize;             ///<[out] Screennail size.
	UINT64           ui1stFramePos;          ///<[out] first video frame position
	UINT32           ui1stFrameSize;         ///<[out] first video frame size
} GXVIDEO_INFO, *PGXVIDEO_INFO;

/**
    @name   GxVideoFile Video Play API
*/
//@{

/**
    Callback Function Description

    The prototype of callback function for video file reading

    @param[in] pos       file read position
    @param[in] size      file read size
    @param[in] addr      read buffer address

    @return ER
*/
typedef  ER(* FPGXVIDEO_READCB)(UINT32 pos, UINT32 size, UINT32 addr);

typedef  ER(* FPGXVIDEO_READCB64)(UINT64 pos, UINT32 size, UINT32 addr);

//#NT#2012/08/21#Calvin Chang#Add for User Data & Thumbnail -begin
/**
    Callback Function Description

    The prototype of callback function for getting thumbnail frame image

    @param[in] userdata_buf    user-data buffer
    @param[in] userdata_size   user-data size
    @param[out] thumb_pos      returned thumbnail position in user-data buffer
    @param[out] thumb_size     returned thumbnail size

    @return ER

*/
typedef  ER(* FPGXVIDEO_GETHUMBCB)(UINT32 userdata_buf, UINT32 userdata_size, UINT64 *thumb_pos, UINT32 *thumb_size);
//#NT#2012/08/21#Calvin Chang -end

/**
    Query Video Buffer Needed Size

    Query the needed size of video buffer for decoding 1st video frame

    @param[in] uiTotalFileSize    the total file size of opened video file
    @param[out] uipBufferNeeded   the video buffer needed size

    @return
     - @b GXVIDEO_PRSERR_OK:     Query successfully.
*/
GXVID_ER GxVidFile_Query1stFrameWkBufSize(UINT32 *uipBufferNeeded, UINT64 uiTotalFileSize);

/**
    Decode 1st Video Frame

    Decode 1st video frame for video play mode, must call GxVidFile_Query1stFrameWkBufSize function first.

    @param[in] fpReadCB      the file read callback function pointer
    @param[in] pWorkBuf      the memory address and size
    @param[out] pDstImgBuf   the 1st video fram image

    @return
     - @b GXVIDEO_PRSERR_OK:     Decode 1st video frame successfully.
     - @b GXVIDEO_PRSERR_SYS:    Decode 1st video is failed. Have to check the content of input video file whether is correct.
*/
GXVID_ER GxVidFile_Decode1stFrame64(FPGXVIDEO_READCB64 fpReadCB, PMEM_RANGE pWorkBuf, PIMG_BUF pDstImgBuf);

GXVID_ER GxVidFile_Decode1stFrame(FPGXVIDEO_READCB fpReadCB, PMEM_RANGE pWorkBuf, PIMG_BUF pDstImgBuf);

GXVID_ER GxVidFile_Decode1stFrameImpl(FPGXVIDEO_READCB fpReadCB, FPGXVIDEO_READCB64 fpReadCB64, PMEM_RANGE pWorkBuf, PIMG_BUF pDstImgBuf);

//#NT#2012/08/21#Calvin Chang#Add for User Data & Thumbnail -begin
/**
    Register callback function for thumbnail

    Register callback function for getting thumbnail stream in user data

    @param[in] fpGetThumbCB    FPGXVIDEO_GETHUMBCB callback function pointer

    @return
     - @b GXVIDEO_PRSERR_OK:     Register successfully.
*/
GXVID_ER GxVidFile_RegisterGetThumbCB(FPGXVIDEO_GETHUMBCB fpGetThumbCB);
//#NT#2012/08/21#Calvin Chang -end

/**
    Query Video Buffer Needed Size

    Query the needed size of video buffer for decoding thumbnail frame

    @param[in] uiTotalFileSize    the total file size of opened video file
    @param[out] uipBufferNeeded   the video buffer needed size

    @return
     - @b GXVIDEO_PRSERR_OK:     Query successfully.
*/
GXVID_ER GxVidFile_QueryThumbWkBufSize(UINT32 *uipBufferNeeded, UINT64 uiTotalFileSize);

/**
    Decode Thumbnail Frame

    Decode thumbnail frame for video file play mode, must call GxVidFile_QueryThumbWkBufSize function first.

    @param[in] fpReadCB      the file read callback function pointer
    @param[in] pWorkBuf      the memory address and size
    @param[out] pDstImgBuf   the thumbnail fram image

    @return
     - @b GXVIDEO_PRSERR_OK:     Decode thumbnail frame successfully.
     - @b GXVIDEO_PRSERR_SYS:    Decode thumbnail frame is failed.
*/
GXVID_ER GxVidFile_DecodeThumbFrame(FPGXVIDEO_READCB64 fpReadCB, PMEM_RANGE pWorkBuf, PIMG_BUF pDstImgBuf);

/**
    Video Information Parser

    Parse the information of video file

    @param[in] fpReadCB       the file read callback function pointer
    @param[in] pWorkBuf       the memory address and size
    @param[in] uiTotalFileSize    the total file size of opened video file
    @param[out] pVideoInfo   the video file information
    @return
     - @b GXVIDEO_PRSERR_OK:     Parse video information successfully.
     - @b GXVID_ER:              Video Information Parser is failed. Reference ErrCode and check the content of input video file whether is correct.
*/
GXVID_ER GxVidFile_ParseVideoInfo64(FPGXVIDEO_READCB64 fpReadCB, PMEM_RANGE pWorkBuf, UINT64 uiTotalFileSize, PGXVIDEO_INFO pVideoInfo);

GXVID_ER GxVidFile_ParseVideoInfo(FPGXVIDEO_READCB fpReadCB, PMEM_RANGE pWorkBuf, UINT64 uiTotalFileSize, PGXVIDEO_INFO pVideoInfo);

GXVID_ER GxVidFile_ParseVideoInfoImpl(FPGXVIDEO_READCB fpReadCB, FPGXVIDEO_READCB64 fpReadCB64, PMEM_RANGE pWorkBuf, UINT64 uiTotalFileSize, PGXVIDEO_INFO pVideoInfo);

/**
    Video Information Parser

    Parse the information of video file

    @param[in] fpReadCB       the file read callback function pointer
    @param[in] pWorkBuf       the memory address and size
    @param[in] uiTotalFileSize    the total file size of opened video file
    @param[out] pVideoInfo   the video file information
    @return
     - @b GXVIDEO_PRSERR_OK:     Parse video information successfully.
     - @b GXVID_ER:              Video Information Parser is failed. Reference ErrCode and check the content of input video file whether is correct.
*/
GXVID_ER GxVidFile_ParseVideoInfoEntry(FPGXVIDEO_READCB64 fpReadCB, PMEM_RANGE pWorkBuf, UINT64 uiTotalFileSize, PGXVIDEO_INFO pVideoInfo);

/**
    Get I Frame Offset & Size.

    Get I Frame Offset & Size

    @param[in] uiGetCnt       Get count of I-Frames
    @param[in] uiGetNum       Get number of I-Frames
    @param[out] pOffset       I-Frame offset
    @param[out] pSize         I-Frame size
    @return
     - @b GXVIDEO_PRSERR_OK:     Parse video information successfully.
     - @b GXVID_ER:              Header Parser is failed. Reference ErrCode and check the content of input video file whether is correct.
*/
GXVID_ER GxVidFile_GetVideoIFrameOffsetSize(UINT32 uiGetCnt, UINT32 uiGetNum, UINT64 *pOffset, UINT32 *pSize);

/**
    Get thumbnail in TS file

    Get thumbnail in TS file

    @param[in] fpReadCB       the file read callback function pointer
    @param[in] pWorkBuf       the memory address and size
    @param[out] thumbAddr     the parsed thumbnail address in memory
    @param[out] thumbSize     the thumbnail size
    @return
     - @b GXVIDEO_PRSERR_OK:     Parse thumbnail successfully.
     - @b GXVID_ER:              Thumbnail Parser is failed. Reference ErrCode and check the content of input video file whether is correct.
*/
GXVID_ER GxVidFile_GetTsThumb64(FPGXVIDEO_READCB64 fpReadCB, PMEM_RANGE pWorkBuf, UINT32 *thumbAddr, UINT32 *thumbSize);

GXVID_ER GxVidFile_GetTsThumb(FPGXVIDEO_READCB fpReadCB, PMEM_RANGE pWorkBuf, UINT32 *thumbAddr, UINT32 *thumbSize);

GXVID_ER GxVidFile_GetTsThumbImpl(FPGXVIDEO_READCB fpReadCB, FPGXVIDEO_READCB64 fpReadCB64, PMEM_RANGE pWorkBuf, UINT32 *thumbAddr, UINT32 *thumbSize);

//@}

//@}

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GX_VIDEOFILE_H_
