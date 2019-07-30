/**
    Public header file for capture callback event

    This file is the header file that define the API and data type for capture callback event.

    @file       ImgCapCBMsg.h
    @ingroup    mIAppCap
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IMG_CAP_CBMSG_H_
#define _IMG_CAP_CBMSG_H_
#include "Type.h"
#include "ipl_utility.h"

/** \addtogroup mIAppCap */
//@{

/**
     capture callback event
*/
typedef enum _IMG_CAP_CBMSG {
	IMG_CAP_CBMSG_GET_RAWEND = 0,       ///< get raw flow end(start capture -> one raw end -> one raw end ->.....-> raw end).
	IMG_CAP_CBMSG_GET_ONERAWEND,        ///< get one raw image end(start capture -> one raw end -> one raw end ->.....).
	IMG_CAP_CBMSG_QUICKVIEW,            ///< quick view CB command, data type IMG_CAP_QV_DATA.
	IMG_CAP_CBMSG_SET_EXIF_DATA,        ///< exif CB command, data type NULL.
	IMG_CAP_CBMSG_WRITE_DEBUG_DATA,     ///< write debug command, data IMG_CAP_DEBUG_INFO.
	IMG_CAP_CBMSG_JPEG_OK,              ///< jpeg ok CB command, data type NULL.
	IMG_CAP_CBMSG_CAPEND,               ///< capture end CB command(always notify after IMG_CAP_CBMSG_RET_PRV command, it doesn't mean save file ready), data type NULL.
	IMG_CAP_CBMSG_FSTOK,                ///< save one file ready command, data type IMG_CAP_FST_INFO
	IMG_CAP_CBMSG_RET_PRV,              ///< return to preview command, data type NULL
	IMG_CAP_CBMSG_GEN_DATASTAMP_PIC,    ///< gen data stamp image command, data type IMG_CAP_DATASTAMP_INFO
	IMG_CAP_CBMSG_GEN_DATASTAMP_STR,    ///< gen data stamp string command, data type NULL
	IMG_CAP_CBMSG_MFHDR,                ///< 3rd party lib process data type IMG_CAP_MFHDR_INFO
	IMG_CAP_CBMSG_MFANTISHAKE,          ///< 3rd party lib process data type IMG_CAP_MFANTISHAKE_INFO
	IMG_CAP_CBMSG_MAX_CNT,
	ENUM_DUMMY4WORD(IMG_CAP_CBMSG)
} IMG_CAP_CBMSG;

/************** IMG_CAP_MFHDR_INFO **************/
#define MF_HDR_MAX_NUM  4       ///< the maximunm image number for multi-frame hdr lib

/**
    multi-frame hdr callback information
*/
typedef struct _IMG_CAP_MFHDR_INFO {
	UINT32 WorkBufAddr;                             ///< working buffer address.
	UINT32 WorkBufSize;                             ///< working buffer size.
	UINT32 PicNum;                                  ///< image nubmer for hdr process
	IPL_YCC_IMG_INFO SrcImgInfo[MF_HDR_MAX_NUM];    ///< input image information
	IPL_YCC_IMG_INFO RstImgInfo;                    ///< result image information
} IMG_CAP_MFHDR_INFO;

/************** IMG_CAP_CBMSG_MFANTISHAKE **************/
#define MF_ANTISHAKE_MAX_NUM  4       ///< the maximunm image number for multi-frame anti-shakte lib

/**
    multi-frame anti-shake callback information
*/
typedef struct _IMG_CAP_MFANTISHAKE_INFO {
	UINT32 WorkBufAddr;                                 ///< working buffer address.
	UINT32 WorkBufSize;                                 ///< working buffer size.
	UINT32 PicNum;                                      ///< image nubmer for hdr process
	IPL_YCC_IMG_INFO SrcImgInfo[MF_ANTISHAKE_MAX_NUM];  ///< input image information
	IPL_YCC_IMG_INFO RstImgInfo;                        ///< result image information
} IMG_CAP_MFANTISHAKE_INFO;

/************** IMG_CAP_FST_INFO **************/
/**
    file system callback status
*/
typedef struct _IMG_CAP_FST_INFO {
	INT32 Status;           ///< file system status
} IMG_CAP_FST_INFO;

/************** IMG_CAP_CBMSG_QUICKVIEW **************/
/**
    quick view callback information
*/
typedef struct _IMG_CAP_QV_DATA {
	IPL_YCC_IMG_INFO ImgInfo;       ///< image information
} IMG_CAP_QV_DATA;

/************** IMG_CAP_CBMSG_WRITE_DEBUG_DATA **************/
/**
    debug message callback information
*/
typedef struct _IMG_CAP_DEBUG_INFO {
	UINT32 Addr;        ///< [out] buffer start address
	UINT32 TotalSize;   ///< [out] total buffer size for debug message
	UINT32 UsedSize;    ///< [in] used size
	UINT32 SensorId;
} IMG_CAP_DEBUG_INFO;

/************** IMG_CAP_CBMSG_WRITE_DEBUG_DATA **************/
/**
    data stamp callback event
*/
typedef enum _IMG_CAP_DATASTAMP_EVENT {
	CAP_DS_EVENT_THUMB = 0,         ///<thumbnail event
	CAP_DS_EVENT_QV,                ///<quick view event
	CAP_DS_EVENT_SCR,               ///<screennail event
	CAP_DS_EVENT_PRI,               ///<primary image event
} IMG_CAP_DATASTAMP_EVENT;

/**
    data stamp callback information
*/
typedef struct _IMG_CAP_DATASTAMP_INFO {
	IPL_YCC_IMG_INFO ImgInfo;                   ///< event image information
	IMG_CAP_DATASTAMP_EVENT event;
} IMG_CAP_DATASTAMP_INFO;

/**
    callback function prototype
*/
typedef void (*IMG_CAP_CBMSG_FP)(IMG_CAP_CBMSG Msg, void *Data);

//@}
#endif //_IMG_CAP_CBMSG_H_
