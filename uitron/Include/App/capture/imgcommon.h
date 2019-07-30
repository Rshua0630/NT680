/**
    Public header file for capture common information

    This file is the header file that define the API and data type for capture common information.

    @file       imgcommon.h
    @ingroup    mIAppCap
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IMG_COMMON_H_
#define _IMG_COMMON_H_

/** \addtogroup mIAppCap */
//@{

/**
     get flag ID for customized event flow

     @return
         - @b UINT32:   flag ID
*/
UINT32 ImgCap_GetFlag(void);

/**
    write file format for #IMG_CAP_FSYS_FP function pointer
*/
typedef enum _IMGCAP_FSYS_TYPE {
	IMGCAP_FSYS_JPG = 0,                ///< jpeg format
	IMGCAP_FSYS_RAW,                    ///< raw format
	ENUM_DUMMY4WORD(IMGCAP_FSYS_TYPE)
} IMGCAP_FSYS_TYPE;

/**
    capture event message
*/
typedef enum _IMGCAP_EVTMSG {
	IMGCAP_EVT_ERR = 0,             ///< capture process fail
	IMGCAP_EVT_NULL,                ///< capture table null
	IMGCAP_EVT_OK = 100,            ///< capture process ok
	ENUM_DUMMY4WORD(IMGCAP_EVTMSG)
} IMGCAP_EVTMSG;


typedef enum _IMGCAP_DBGMSG {
	IMGCAP_DBG_ALL = 0,             ///< capture debug all masssage
	IMGCAP_DBG_PM,                  ///< capture debug primacy set
	IMGCAP_DBG_UIPAM,               ///< capture debug primacy set
	IMGCAP_DBG_MAX,
	ENUM_DUMMY4WORD(IMGCAP_DBGMSG)
} IMGCAP_DBGMSG;

typedef struct _CAP_RAW_BASE_INFO {
	IPL_GET_CAP_RAW_YOUT_INFO y_out_info;
} CAP_RAW_BASE_INFO;

typedef struct _CAP_HEADER_DATA {
	UINT32 start_tag;
	UINT32 Proc_id;
	UINT32 Width;           ///< image width(pixel)
	UINT32 Height;          ///< image height(pixel)
	UINT32 LineOffset;      ///< image line offset(byte)
	IPL_RAW_BIT Bit;        ///< bits /pixel
	IPL_RAW_PIX STPix;      ///< image start pixel
	UINT32 rsc_addr;
	UINT32 RawType;
	UINT32 end_tag;
	CAP_RAW_BASE_INFO base_info;
} CAP_HEADER_DATA;

#define SENSOR_LIST_CNT 8
extern UINT32 DRE_Proc_Fmt;
/********************* common used flag *********************/
typedef enum {
	IMGCAP_COMMON_FLAG_BIT_00 = 0,
	IMGCAP_COMMON_FLAG_BIT_01,
	IMGCAP_COMMON_FLAG_BIT_02,
	IMGCAP_COMMON_FLAG_BIT_03,
	IMGCAP_COMMON_FLAG_BIT_04,
	IMGCAP_COMMON_FLAG_BIT_05,
	IMGCAP_COMMON_FLAG_BIT_06,
	IMGCAP_COMMON_FLAG_BIT_07,
	IMGCAP_COMMON_FLAG_BIT_08,
	IMGCAP_COMMON_FLAG_BIT_09,
	IMGCAP_COMMON_FLAG_BIT_10,
	IMGCAP_COMMON_FLAG_BIT_11,
	IMGCAP_COMMON_FLAG_BIT_12,
	IMGCAP_COMMON_FLAG_BIT_13,
	IMGCAP_COMMON_FLAG_BIT_14,
	IMGCAP_COMMON_FLAG_BIT_15,
	IMGCAP_COMMON_FLAG_BIT_16,
	IMGCAP_COMMON_FLAG_BIT_17,
	IMGCAP_COMMON_FLAG_BIT_18,
	IMGCAP_COMMON_FLAG_BIT_19,
	IMGCAP_COMMON_FLAG_BIT_20,
	IMGCAP_COMMON_FLAG_NUM,
	ENUM_DUMMY4WORD(IMGCAP_COMMON_FLAG)
} IMGCAP_COMMON_FLAG;

//global flag for flow control
#define FLG_CAPTURE_START_1          FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_00)
#define FLG_CAPTURE_START_2          FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_01)
#define FLG_CAPTURE_DONE             FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_02)
//common flag for img cap task
#define FLG_CAPTSK_IMGCAP_IDLE     FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_03)
#define FLG_CAPTSK_IMGCAP_START    FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_04)
#define FLG_CAPTSK_IMGCAP_END      FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_05)

//common flag for img proc task
#define FLG_CAPTSK_IMGPROC_IDLE    FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_06)
#define FLG_CAPTSK_IMGPROC_START   FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_07)
#define FLG_CAPTSK_IMGPROC_END     FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_08)

//common flag for img proc2 task
#define FLG_CAPTSK_IMGPROC2_IDLE    FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_09)
#define FLG_CAPTSK_IMGPROC2_START   FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_10)
#define FLG_CAPTSK_IMGPROC2_END     FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_11)

//common flag for img enc task
#define FLG_CAPTSK_IMGENC_IDLE    FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_12)
#define FLG_CAPTSK_IMGENC_START   FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_13)
#define FLG_CAPTSK_IMGENC_END     FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_14)

//common flag for img write file task
#define FLG_CAPTSK_IMGWRITEFILE_IDLE   FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_15)
#define FLG_CAPTSK_IMGWRITEFILE_START  FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_16)
#define FLG_CAPTSK_IMGWRITEFILE_END    FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_17)

//common flag for background task
#define FLG_CAPTSK_BACKGROUND_IDLE   FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_18)
#define FLG_CAPTSK_BACKGROUND_START  FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_19)
#define FLG_CAPTSK_BACKGROUND_END    FLGPTN_BIT(IMGCAP_COMMON_FLAG_BIT_20)
//
#define FLG_CAPTSK_ALL (FLGPTN)(~(FLG_CAPTSK_IMGCAP_IDLE|FLG_CAPTSK_IMGPROC_IDLE|FLG_CAPTSK_IMGPROC2_IDLE|FLG_CAPTSK_IMGENC_IDLE|FLG_CAPTSK_IMGWRITEFILE_IDLE|FLG_CAPTSK_BACKGROUND_IDLE|FLG_CAPTURE_DONE))
//@}

extern BOOL Cap_ShowDebugMsg(IMGCAP_DBGMSG Sel);
extern void Cap_SetDebugMsgEn(IMGCAP_DBGMSG Sel, BOOL Enable);

#endif //_IMG_COMMON_H_
