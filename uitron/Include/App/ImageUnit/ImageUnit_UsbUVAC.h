/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_UsbUVAC.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_USBUVAC_H
#define IMAGEUNIT_USBUVAC_H

/*
Support these input port(s): IN1,IN2

Support these output port(s):

Support these port config API(s):

============================================================
port
------------------------------------------------------------
ImageUnit_CfgImgSize()
ImageUnit_CfgImgAspect()
ImageUnit_CfgImgWindow()
ImageUnit_CfgImgDirect()
============================================================
*/

#include "Type.h"
#include "ImageStream.h"



extern ISF_UNIT ISF_UsbUVAC;


/**
    Supported video format.

    @note For IMGUNIT_UVAC_CFG_VID_FMT_TYPE.
*/
typedef enum _IMGUNIT_UVAC_VIDEO_FORMAT_TYPE {
	IMGUNIT_UVAC_VIDEO_FORMAT_H264_MJPEG,          ///< Support H264 and MJPEG.
	IMGUNIT_UVAC_VIDEO_FORMAT_H264_ONLY,           ///< Support H264 only.
	IMGUNIT_UVAC_VIDEO_FORMAT_MJPEG_ONLY,          ///< Support MJPEG only.
	ENUM_DUMMY4WORD(IMGUNIT_UVAC_VIDEO_FORMAT_TYPE)
} IMGUNIT_UVAC_VIDEO_FORMAT_TYPE;


/**
    Parameters for ISF_UsbUVAC.
*/
enum {
	UVAC_PARAM_START = 0x00003000,
	UVAC_PARAM_UVAC_INFO,           ///< Information needed for UVAC module.
	UVAC_PARAM_UVAC_VEND_DEV_DESC,  ///< Device descriptor, refering to #_UVAC_VEND_DEV_DESC.
	UVAC_PARAM_VID_RESO_ARY,        ///< Video resolution array, refering to #_UVAC_VID_RESO_ARY.
	UVAC_PARAM_AUD_SAMPLERATE_ARY,  ///< Audio sample array, refering to #_UVAC_AUD_SAMPLERATE_ARY_.
	UVAC_PARAM_UVAC_TBR_MJPG,       ///< Motion JPEG stream target size(Byte/Sec). Default value is 0x300000.
	UVAC_PARAM_UVAC_TBR_H264,       ///< H.264 stream target size(Byte/Sec). Default value is 0x100000.
	UVAC_PARAM_BS_START,            ///< Resume  sending stream to USB host.
	UVAC_PARAM_BS_STOP,             ///< Suspend sending stream to USB host.
	UVAC_PARAM_UVAC_CAP_M3,         ///< Enable UVC capture method 3.
	UVAC_PARAM_EU_VENDCMDCB_ID01,   ///> Extension unit callback with CS=1.
	UVAC_PARAM_EU_VENDCMDCB_ID02,   ///> Extension unit callback with CS=2.
	UVAC_PARAM_EU_VENDCMDCB_ID03,   ///> Extension unit callback with CS=3.
	UVAC_PARAM_EU_VENDCMDCB_ID04,   ///> Extension unit callback with CS=4.
	UVAC_PARAM_EU_VENDCMDCB_ID05,   ///> Extension unit callback with CS=5.
	UVAC_PARAM_EU_VENDCMDCB_ID06,   ///> Extension unit callback with CS=6.
	UVAC_PARAM_EU_VENDCMDCB_ID07,   ///> Extension unit callback with CS=7.
	UVAC_PARAM_EU_VENDCMDCB_ID08,   ///> Extension unit callback with CS=8.
	UVAC_PARAM_WINUSB_ENABLE,       ///< Enable WINUSB.
	UVAC_PARAM_WINUSB_CB,           ///< The callback function for WINUSB
	UVAC_PARAM_BULK_DATA,           ///< Obsolete
	UVAC_PARAM_VID_FMT_TYPE,        ///< Supported video format by UVC device.
	UVAC_PARAM_CDC_ENABLE,          ///< Enable CDC ADM.
	UVAC_PARAM_CDC_PSTN_REQUEST_CB, ///< The callback function for CDC PSTN subclass specific request
	UVAC_PARAM_AUD_FMT_TYPE,        ///< Audio format, refering to #_UVAC_AUD_FMT_TYPE
	UVAC_PARAM_MAX_FRAME_SIZE,      ///< Set higher frame size in MJPEG format.
	UVAC_PARAM_SUSPEND_CLOSE_IMM,	///< Suspend close unit.
	UVAC_PARAM_EU_CTRL,				///< UVC extension unit capability.
	UVAC_PARAM_MAX
};

extern ISF_UNIT ISF_UsbUVAC;

#endif //IMAGEUNIT_USBUVAC_H

