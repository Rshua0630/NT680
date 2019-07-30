/**
    Header file of video encoding codec library

    Exported header file of video encoding codec library.

    @file       VideoEncode.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _VIDEOENCODE_H
#define _VIDEOENCODE_H

#include "dal_vdoenc.h"

#define MP_VDOENC_SHOW_MSG 			1

/**
    Each bitstream buffer size for N frames encoding simultaneously
*/
#define MP_VDOENC_BSBUFSIZE  				(1024*1024) 	///< 1MB for BS temp buffer as simultaneously encode
#define MP_VDOENC_REFSIZE    				(2*1024*1024) 	///< 2MB for MV, RcRef, SideInfo ... (max enc limit: 5120*5120)


#define MP_VDOENC_QP_MIN 					DAL_VDOENC_QP_MIN
#define MP_VDOENC_QP_MAX 					DAL_VDOENC_QP_MAX
#define MP_VDOENC_ROI_NUM_MAX				DAL_VDOENC_ROI_NUM_MAX
#define MP_VDOENC_BSTR_VF_NUM_MAX  			60  							///< input video bitstream test pattern max frame numbers
#define MP_VDOENC_FIXED_YUV_NUM_MAX    		8   							///< max number of fixed YUV test patterns
#define MP_VDOENC_MD_INFO_SIZE				DAL_VDOENC_MD_INFO_SIZE

typedef void (*MEDIAREC_3DNRCB)(UINT32 pathID, UINT32 config);

/**
    Video Encoder ID
*/
typedef enum {
	MP_VDOENC_ID_1,                			///< video encoder ID 1
	MP_VDOENC_ID_2,                			///< video encoder ID 2
	MP_VDOENC_ID_3,                			///< video encoder ID 3
	MP_VDOENC_ID_4,                			///< video encoder ID 4
	MP_VDOENC_ID_5,                			///< video encoder ID 5
	MP_VDOENC_ID_6,                			///< video encoder ID 6
	MP_VDOENC_ID_7,                			///< video encoder ID 7
	MP_VDOENC_ID_8,                			///< video encoder ID 8
	MP_VDOENC_ID_9,                			///< video encoder ID 9
	MP_VDOENC_ID_10,               			///< video encoder ID 10
	MP_VDOENC_ID_11,               			///< video encoder ID 11
	MP_VDOENC_ID_12,               			///< video encoder ID 12
	MP_VDOENC_ID_13,               			///< video encoder ID 13
	MP_VDOENC_ID_14,               			///< video encoder ID 14
	MP_VDOENC_ID_15,               			///< video encoder ID 15
	MP_VDOENC_ID_16,               			///< video encoder ID 16
	MP_VDOENC_ID_MAX,              			///< video encoder ID maximum
	ENUM_DUMMY4WORD(MP_VDOENC_ID)
} MP_VDOENC_ID;

/**
    Video Display Aspect Ratio
*/
typedef enum {
	MP_VDOENC_DAR_DEFAULT,         			///< default video display aspect ratio (the same as encoded image)
	MP_VDOENC_DAR_16_9,            			///< 16:9 video display aspect ratio
	MP_VDOENC_DAR_MAX,             			///< maximum video display aspect ratio
	ENUM_DUMMY4WORD(MP_VDOENC_DAR)
} MP_VDOENC_DAR;

/**
    Video Encoding Path
*/
typedef enum {
	MP_VDOENC_PATH_D2D,            			///< encoding path: D2D
	MP_VDOENC_PATH_IMEDIRECT,      			///< encoding path: IME/Direct
	MP_VDOENC_PATH_ALLDIRECT,      			///< encoding path: All direct
	MP_VDOENC_PATH_MAX,            			///< maximum encoding path value
	ENUM_DUMMY4WORD(MP_VDOENC_PATH)
} MP_VDOENC_PATH;

/**
    Video Encoding Profile
*/
typedef enum {
	MP_VDOENC_PROFILE_BASELINE,
	MP_VDOENC_PROFILE_MAIN,
	MP_VDOENC_PROFILE_HIGH,
	MP_VDOENC_PROFILE_MAX,
	ENUM_DUMMY4WORD(MP_VDOENC_PROFILE)
} MP_VDOENC_PROFILE;

/**
    SVC settings
*/
typedef enum {
	MP_VDOENC_SVC_DISABLE,         			///< disable SVC
	MP_VDOENC_SVC_LAYER1,          			///< enable SVC with GOP number 15
	MP_VDOENC_SVC_LAYER2,          			///< enable SVC with GOP number 15
	MP_VDOENC_SVC_MAX,             			///< SVC setting maximum
	ENUM_DUMMY4WORD(MP_VDOENC_SVC)
} MP_VDOENC_SVC;

/**
    type of getting info from video encoding codec library.
    format: ER (*GetInfo)(MP_VDOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
*/
typedef enum {
	MP_VDOENC_GETINFO_MEM_SIZE, 			///< get codec needed size
	MP_VDOENC_GETINFO_DESC, 				///< get header description
	MP_VDOENC_GETINFO_ISIFRAME, 			///< check whether is I frame
	MP_VDOENC_GETINFO_GOPNUM, 				///< get gop number
	MP_VDOENC_GETINFO_CBR, 					///< get CBR info
	MP_VDOENC_GETINFO_EVBR, 				///< get EVBR info
	MP_VDOENC_GETINFO_VBR, 					///< get VBR info
	MP_VDOENC_GETINFO_FIXQP, 				///< get FIXQP info
	MP_VDOENC_GETINFO_AQ, 					///< get AQ info
	MP_VDOENC_GETINFO_RC, 					///< get RC info
	MP_VDOENC_GETINFO_3DNR, 				///< get 3DNR info
	ENUM_DUMMY4WORD(MP_VDOENC_GETINFO_TYPE)
} MP_VDOENC_GETINFO_TYPE;

/**
    type of setting info to video encoding codec library.
    format: ER (*SetInfo)(MP_VDOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
*/
typedef enum {
	MP_VDOENC_SETINFO_CBR, 					///< set CBR info
	MP_VDOENC_SETINFO_EVBR, 				///< set EVBR info
	MP_VDOENC_SETINFO_VBR, 					///< set VBR info
	MP_VDOENC_SETINFO_FIXQP, 				///< set FIXQP info
	MP_VDOENC_SETINFO_ROWRC, 				///< set ROWRC info
	MP_VDOENC_SETINFO_QPMAP, 				///< set QP Map
	MP_VDOENC_SETINFO_AQ, 					///< set AQ info
	MP_VDOENC_SETINFO_3DNR, 				///< set 3DNR info
	MP_VDOENC_SETINFO_3DNRCB, 				///< set 3DNR setting callback
	MP_VDOENC_SETINFO_ROI, 					///< set ROI info
	MP_VDOENC_SETINFO_SMART_ROI, 			///< set Smart ROI info (from OD)
	MP_VDOENC_SETINFO_MD, 					///< set MD info
	MP_VDOENC_SETINFO_SLICESPLIT,			///< set Slice Split info
	MP_VDOENC_SETINFO_RESET_IFRAME, 		///< reset I frame
	MP_VDOENC_SETINFO_DUMPINFO, 			///< enable/disable dump info (e.x. brc ...)
	MP_VDOENC_SETINFO_RESET, 				///< reset settings when encoding h.264/h.265 simultaneously
	ENUM_DUMMY4WORD(MP_VDOENC_SETINFO_TYPE)
} MP_VDOENC_SETINFO_TYPE;

typedef enum {
	MP_VDOENC_ROTATE_DISABLE,				///< no rotate
	MP_VDOENC_ROTATE_CCW, 					///< rotate counter clockwise
	MP_VDOENC_ROTATE_CW, 					///< rotate clockwise
	ENUM_DUMMY4WORD(MP_VDOENC_ROTATE_TYPE)
} MP_VDOENC_ROTATE_TYPE;

typedef DAL_VDOENC_3DNR_INFO MP_VDOENC_3DNR_INFO;

typedef DAL_VDOENC_3DNRCB MP_VDOENC_3DNRCB;

typedef DAL_VDOENC_AQ_INFO MP_VDOENC_AQ_INFO;

typedef DAL_VDOENC_BUF MP_VDOENC_BUF;

typedef DAL_VDOENC_CBR_INFO MP_VDOENC_CBR_INFO;

typedef DAL_VDOENC_EVBR_INFO MP_VDOENC_EVBR_INFO;

typedef DAL_VDOENC_FIXQP_INFO MP_VDOENC_FIXQP_INFO;

typedef DAL_VDOENC_LTR_INFO MP_VDOENC_LTR_INFO;

typedef DAL_VDOENC_MD_INFO MP_VDOENC_MD_INFO;

typedef DAL_VDOENC_MEM_INFO MP_VDOENC_MEM_INFO;

typedef DAL_VDOENC_QPMAP_INFO MP_VDOENC_QPMAP_INFO;

typedef DAL_VDOENC_RC_INFO MP_VDOENC_RC_INFO;

typedef DAL_VDOENC_ROWRC_INFO MP_VDOENC_ROWRC_INFO;

typedef DAL_VDOENC_ROI_INFO MP_VDOENC_ROI_INFO;

typedef DAL_VDOENC_SAR MP_VDOENC_SAR;

typedef DAL_VDOENC_SMART_ROI_INFO MP_VDOENC_SMART_ROI_INFO;

typedef DAL_VDOENC_SRC_COMPRESS_INFO MP_VDOENC_SRC_COMPRESS_INFO;

typedef DAL_VDOENC_VBR_INFO MP_VDOENC_VBR_INFO;

typedef DAL_VDOENC_YUV_SRC MP_VDOENC_YUV_SRC;

/**
    Init structure
*/
typedef DAL_VDOENC_INIT MP_VDOENC_INIT;

/**
    Encode structure
*/
typedef DAL_VDOENC_PARAM MP_VDOENC_PARAM;

/**
    Structure of video encoder
*/
typedef struct {
	ER(*Initialize)(MP_VDOENC_INIT *ptr);  													///< initialize encoder
	ER(*Close)(void);   																			///< close encoder
	ER(*GetInfo)(MP_VDOENC_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);   ///< get info from codec library
	ER(*SetInfo)(MP_VDOENC_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);  		///< set parameters to codec library
	ER(*EncodeOne)(UINT32 type, UINT32 outputAddr, UINT32 *pSize, MP_VDOENC_PARAM *ptr);  			///< encoding one video frame and wait ready
	ER(*TriggerEnc)(MP_VDOENC_PARAM *ptr);  														///< trigger encoding one video frame but not wait ready (only for direct path)
	ER(*WaitEncReady)(MP_VDOENC_PARAM *ptr);  														///< wait for encoding ready
	ER(*AdjustBPS)(UINT32 type, UINT32 param1, UINT32 param2, UINT32 param3);  						///< adjusting BPS
	ER(*CustomizeFunc)(UINT32 type, void *pobj);  													///< codec customize function
	UINT32 checkID;     																			///< check ID
} MP_VDOENC_ENCODER, *PMP_VDOENC_ENCODER;

#endif
