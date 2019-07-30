/**
    Header file of video decoding codec library

    Exported header file of video decoding codec library.

    @file       VideoDecode.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _VIDEODECODE_H
#define _VIDEODECODE_H


#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"
#include "dal_vdodec.h"

#define MP_VDODEC_SHOW_MSG 				1

/**
    Definition of video pre-decode frame buffer number.
*/
//@{
#define MP_VDODEC_PREDEC_FRMNUM			3			///< Video Pre-decode Frame Number
//@}

/**
    Video Decoder ID
*/
typedef enum {
	MP_VDODEC_ID_1,                			///< video decoder ID 1
	MP_VDODEC_ID_MAX,              			///< video decoder ID maximum
	ENUM_DUMMY4WORD(MP_VDODEC_ID)
} MP_VDODEC_ID;

/**
    type for CustomizeFunc() in MP_VDODEC_DECODER.
*/
//@{
typedef enum {
	MP_VDODEC_CUSTOM_DECODE1ST,						///< p1:(MP_VDODEC_1STV_INFO *) object(out)
	MP_VDODEC_CUSTOM_STREAMINIT,					///< p1:(MP_VDODEC_STREAM_INIT *) object(in)
	MP_VDODEC_CUSTOM_STREAMDECODE,					///< p1:(MP_VDODEC_STREAM_INFO *) object(in)
	MP_VDODEC_CUSTOM_STREAMWAITDONE,				///< p1:(UINT32 *) ER_Code(out)
	ENUM_DUMMY4WORD(MP_VDODEC_CUSTOM_TYPE)
} MP_VDODEC_CUSTOM_TYPE;
//@}

/**
    Type of getting information from video decoding codec library.
*/
typedef enum {
	MP_VDODEC_GETINFO_NOWYCBCR,						///< p1:y addr(out), p2:cb addr(out), p3: cr addr(out)
	MP_VDODEC_GETINFO_PREVYCBCR,					///< p1:prev y addr(out), p2: prev cb addr(out), p3: prev cr addr(out)
	MP_VDODEC_GETINFO_PREDECFRAMENUM,				///< p1:Pre-Decode Frame Numeber(out)
	MP_VDODEC_GETINFO_IFREAMENUMINSEC,				///< p1:I-frame numeber in sec(out)
	MP_VDODEC_GETINFO_CURRDISPFRMNUM,				///< p1: Current display frame number(out)
	MP_VDODEC_GETINFO_CURRGOPNUM,					///< p1:Current GOP number(out)
	MP_VDODEC_GETINFO_OUTPUTYCBCR,					///< p1:y addr(out), p2:cb addr(out), p3: cr addr(out)
	ENUM_DUMMY4WORD(MP_VDODEC_GETINFO_TYPE)
} MP_VDODEC_GETINFO_TYPE;

/**
    Type of setting information to video decoding codec library.
*/
typedef enum {
	MP_VDODEC_SETINFO_BUF_ADDR,						///< p1:address for H264 description (in)
	MP_VDODEC_SETINFO_YUV_ADDR,						///< p1:y addr(in), p2:UV addr(in)
	MP_VDODEC_SETINFO_CLOSEBUS,						///< No parameters, stop decoder.
	MP_VDODEC_SETINFO_FRAMERATE,					///< p1:frame rate for reorder queue
	MP_VDODEC_SETINFO_PREDECODE,					///< p1:y memory address(in), p2:memeory size(in), p3: pre-decode frame number, pre-decode image for H.264 with B-frame
	MP_VDODEC_SETINFO_PLAYCHANGE,					///< No parameters, inform Media Plug-in the Display frame order is changed.
	MP_VDODEC_SETINFO_DECODEONE_WAITDONE,			///< No parameters, decode One Frame and Wait Done
	MP_VDODEC_SETINFO_REFRESH_IMGBUF,				///< No parameters, refresh H264 Image Buffer Queue
	MP_VDODEC_SETINFO_PREDECFRAMENUM,				///< p1: pre decode frame number(in)
	MP_VDODEC_SETINFO_OUTPUTRAWMODE,				///< p1:enable/disable output RAW mode
	MP_VDODEC_SETINFO_RAWDATAADDR,					///< p1:RAW Y addr(in), p2:RAW UV addr(in)
	MP_VDODEC_SETINFO_H264GOPNUM,					///< p1:h264 GOP number(in)
	MP_VDODEC_SETINFO_H265GOPNUM,					///< p1:h265 GOP number(in)
	ENUM_DUMMY4WORD(MP_VDODEC_SETINFO_TYPE)
} MP_VDODEC_SETINFO_TYPE;


/**
    Type of 1st video frame decode information.
*/
typedef struct {
	UINT32 addr;									///< [in] Video frame addr
	UINT32 size;									///< [in] Video frame size
	UINT32 decodeBuf;								///< [in] Output raw addr
	UINT32 decodeBufSize;							///< [in] Max size for outputing raw
	UINT32 y_Addr;									///< [out] Output Y addr
	UINT32 cb_Addr;									///< [out] Output cb addr
	UINT32 cr_Addr;									///< [out] Output cr addr
	UINT32 width;									///< [out] Video frame width
	UINT32 height;									///< [out] Video frame height
	UINT32 DescAddr;								///< [in] Description addr
	UINT32 DescLen;									///< [in] Description length
	UINT32 jpgType;									///< [out] Jpg type, JPG_FMT_YUV420 or others
	UINT32 decfrmnum;								///< [in] Decode Frame numeber
	UINT32 frmaddr[MP_VDODEC_PREDEC_FRMNUM];		///< [in] Video frames position
	UINT32 frmsize[MP_VDODEC_PREDEC_FRMNUM];		///< [in] Video frames size
} MP_VDODEC_1STV_INFO;

/**
    Type defintion struture of video frame data YCbCr buffer information.
*/
typedef struct {
	UINT32 yAddr;									///< Y address
	UINT32 cbAddr;									///< CB address
	UINT32 crAddr;									///< CR address
} MP_VDODEC_YCBCR_INFO;

/**
    Type defintion struture of video display setting information.
*/
typedef struct {
	UINT32 firstFramePos;							///<[in] First video frame position
	UINT32 firstFrameSize;							///<[in] First video frame size
	UINT32 rawAddr;									///<[in] After decoded, Y addr (cb, cr counted)
	UINT32 rawSize;									///<[in] Size for YCbCr
	UINT32 rawWidth;								///<[in] Raw width
	UINT32 rawHeight;								///<[in] Raw height
	UINT32 rawType;									///<[in] Raw type, (jdcfg.fileformat)
	UINT32 rawCbAddr;								///<[out] After decoded, cb addr
	UINT32 rawCrAddr;								///<[out] After decoded, cr addr
	UINT32 DescAddr;								///<[in] Description addr
	UINT32 DescLen;									///<[in] Description length
	UINT32 jpgType;									///<[in] Jpg type
} MP_VDODEC_DISPLAY_INFO;

/**
	Type defintion struture of video stream decoder init parameter.
*/
typedef struct {
	UINT32 workbuf;									///<[in] workbuf Addr for codec H/W internal use
	UINT32 workbufsize;								///<[in] workbuf size for codec H/W internal use
	UINT32 rawWidth;								///<[in] Raw width
	UINT32 rawHeight;								///<[in] Raw height
	UINT32 DescAddr;								///<[in] Description addr
	UINT32 DescLen;									///<[in] Description length
} MP_VDODEC_STREAM_INIT;

/**
	Type defintion struture of video stream decode parameter.
*/
typedef struct {
	UINT32 bs_addr;									///<[in] bitstream addr
	UINT32 bs_size;									///<[in] bitstream size
	UINT32 y_addr;									///<[out] After decoded, Y addr
	UINT32 uv_addr;									///<[out] After decoded, UV addr
	UINT32 frameType;								///<[in] slice type for current decode frame
} MP_VDODEC_STREAM_INFO;

/**
    Init structure
*/
typedef DAL_VDODEC_INIT MP_VDODEC_INIT;

/**
    Decode structure
*/
typedef DAL_VDODEC_PARAM MP_VDODEC_PARAM;

/**
	Type defintion struture of video decoder object functions.
*/
typedef struct {
	ER(*Initialize)(UINT32 fileAddr, MP_VDODEC_DISPLAY_INFO *pobj);									///< Initialize decoder
	ER(*GetInfo)(MP_VDODEC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);	///< Get information from decoder
	ER(*SetInfo)(MP_VDODEC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);			///< Set information to decoder
	ER(*DecodeOne)(UINT32 type, UINT32 fileAddr, UINT32 p2, UINT32 *p3);							///< Decode one frame, p1: frame addr(in), p2: frame size(in), p3: frame number for H264 (in)
	ER(*WaitDecodeDone)(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);							///< Wait decoding finished, p1: E_OK or not(out)
	ER(*CustomizeFunc)(MP_VDODEC_CUSTOM_TYPE type, void *pobj);										///< customize function, if needed
	UINT32 checkID;																					///< check ID for decoders
} MP_VDODEC_DECODER, *PMP_VDODEC_DECODER;


//@}

#endif //_VIDEODECODE_H

