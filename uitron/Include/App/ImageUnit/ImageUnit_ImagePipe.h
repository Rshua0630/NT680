/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_ImagePipe.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_IMAGEPIPE_H
#define IMAGEUNIT_IMAGEPIPE_H
/*
Note: NT96680
  each IPL support 5 outputs
     Output path 1: support raw encode for H264/H265 only
  Dzoom | SIE1~8
  AE/AWB | SIE1~8
  AF | SIE1 only
  for CCIR sensor, IPL also support Dzoom/SensorRatio/Crop/5-Output!
*/

/*
For ImageUnit_ImagePipe1~8

Support these input port(s): IN1

Support these output port(s): OUT1,OUT2,OUT3,OUT4,OUT5,OUTC

Support these port config API(s):

============================================================
port                        IN1         OUT1,OUT2,OUT3,OUT4,OUT5
------------------------------------------------------------
ImageUnit_CfgImgSize()                  [Y]
ImageUnit_CfgImgAspect()    [Y]
ImageUnit_CfgImgWindow()    [Y](*)      [Y](**)
ImageUnit_CfgImgDirect()    [Y]
============================================================
(*) set SIE input crop window
(**) set IME output crop window

*/

#include "Type.h"
#include "ImageStream.h"
#include "ipl_cmd.h" //for imagepipe parameters
#include "ipl_alg_infor.h" //for imagepipe parameters
#include "ipl_utility.h" //for imagepipe parameters
#include "ipl_cb_msg.h" //for imagepipe parameters

#if defined(_BSP_NA51000_)
#define IMAGEPIPE_MAX_NUM	8
#endif
#if defined(_BSP_NA51023_)
#define IMAGEPIPE_MAX_NUM	2
#endif

enum {
	IMAGEPIPE_PARAM_START = 0x80001010,
	IMAGEPIPE_PARAM_MODE = IMAGEPIPE_PARAM_START, ///< ISF_CTRL: Set operation mode of an ImagePipe device. #IMAGEPIPE_MODE
	IMAGEPIPE_PARAM_VPORT_START, ///< ISF_CTRL: begin / Set virtual port source of an ImagePipe device. #IMAGEPIPE_PARAM_VPORT
	IMAGEPIPE_PARAM_VPORT_END = IMAGEPIPE_PARAM_VPORT_START + 10 - 1, ///< ISF_CTRL: end / Set virtual port source of an ImagePipe device. #IMAGEPIPE_PARAM_VPORT
	IMAGEPIPE_PARAM_IPL_CB, ///< ISF_CTRL: Set general callback of an ImagePipe device.
	IMAGEPIPE_PARAM_EVENT_CB, ///< ISF_CTRL: Set event callback of an ImagePipe device.
	IMAGEPIPE_PARAM_VIDEOMASK_CB, ///< ISF_CTRL: Set video mask callback of an ImagePipe device. 
	IMAGEPIPE_PARAM_VIDEOOSD_CB, ///< ISF_CTRL: Set video OSD callback of an ImagePipe device.
	IMAGEPIPE_PARAM_FUNC, ///< ISF_CTRL: Enable basic function of an ImagePipe device. #IMAGEPIPE_FUNC
	IMAGEPIPE_PARAM_FUNC2, ///< ISF_CTRL: Enable advance function of an ImagePipe device. #IMAGEPIPE_FUNC2
	IMAGEPIPE_PARAM_MAX_FUNC, ///< ISF_CTRL: Prepare buffer for enable basic function of an ImagePipe device. #IMAGEPIPE_FUNC
	IMAGEPIPE_PARAM_MAX_FUNC2, ///< ISF_CTRL: Prepare buffer for enable advance function of an ImagePipe device. #IMAGEPIPE_FUNC2
	IMAGEPIPE_PARAM_SCALE_QUALITY_RATIO, ///< ISF_CTRL: Set ratio to decide small scaling or large scaling, threshold format = [31:16]/[15:0]
	IMAGEPIPE_PARAM_SCALE_MATHOD_SMALL, ///< ISF_CTRL: Set scaling mathod for small scaling. #IPL_IME_SCALER
	IMAGEPIPE_PARAM_SCALE_MATHOD_LARGE, ///< ISF_CTRL: Set scaling mathod for large scaling. #IPL_IME_SCALER
	IMAGEPIPE_PARAM_NVX_CODEC, ///< ISF_CTRL: Set target codec for yuv compress mode of an ImagePipe device. #MAKEFOURCC()
	IMAGEPIPE_PARAM_SNAPSHOT_IMM,    ///< ISF_OUTx: Do snapshot of an ImagePipe device.
	IMAGEPIPE_PARAM_SENSORID_IMM,  ///< ISF_IN1: Get sensor map (RO) of an ImagePipe device.
	IMAGEPIPE_PARAM_ACTUAL_INSIZE_IMM,    ///< ISF_IN1: Get real input size (RO) of an ImagePipe device.
	IMAGEPIPE_PARAM_ACTUAL_INFPS_IMM,    ///< ISF_IN1: Get real input fps (RO) of an ImagePipe device.
	IMAGEPIPE_PARAM_DEFAULT_INFPS_IMM,  ///< ISF_IN1: Get default input fps (RO) of an ImagePipe device.
	IMAGEPIPE_PARAM_LIVE_INFPS_IMM,  ///< ISF_IN1: Get live input fps (RO) of an ImagePipe device.
#if defined(_BSP_NA51023_)
	IMAGEPIPE_PARAM_MOTIONDETECT_CB, ///< ISF_CTRL: Set motion-detect callback of an ImagePipe device.
#endif
	IMAGEPIPE_PARAM_CODEC, ///< ISF_OUTx: Set target codec for this output of an ImagePipe device. #MAKEFOURCC()
#if defined(_BSP_NA51023_)
	IMAGEPIPE_PARAM_VIEWTRACKING_CB, ///< ISF_CTRL: Set general view-tracking callback of an ImagePipe device.
#endif
	IMAGEPIPE_PARAM_PROC_CB, ///< ISF_CTRL: Set proc state callback of an ImagePipe device.
#if defined(_BSP_NA51023_)
	IMAGEPIPE_PARAM_COLOR_SPACE,  ///< ISF_CTRL: Set color space of an ImagePipe device. #MAKEFOURCC()
#endif
#if _TODO
	IMAGEPIPE_PARAM_DIRECT2H264SNAPSHOT,    // Start snapshot for DIRECT2H264
	IMAGEPIPE_PARAM_DIRECT2H264SNAPSHOT_STOP,    // Stop snapshot for DIRECT2H264
	IMAGEPIPE_PARAM_SBSVIEW_EN, // side-by-side view for VR-CAM & 360-CAM
	IMAGEPIPE_PARAM_AUTOIPLOFF_EN, // auto off another IPL for reduce DMA B/W (NOTE: FastPipView swicth to "Both" will not work)
#endif
	IMAGEPIPE_PARAM_SIZEFACTOR, ///< ISF_OUT: enable size scale factor for View Tracking
};


#define IMAGEPIPE_MODE_UNKNOWN  0xff    ///< IPL unknown.
#define IMAGEPIPE_MODE_OFF      0       ///< IPL off mode.
#define IMAGEPIPE_MODE_PREVIEW  2       ///< IPL photo preview mode.
#define IMAGEPIPE_MODE_VIDEO    3       ///< IPL movie preview mode.
#define IMAGEPIPE_MODE_VIDEOREC 4       ///< IPL movie recording mode.
#define IMAGEPIPE_MODE_DIRTY    0x80000000  ///< dirty IPL mode (force set mode).

//used from IMAGEPIPE_PARAM_VPORT_START to IMAGEPIPE_PARAM_VPORT_END
#define IMAGEPIPE_PARAM_VPORT_SRC(oPort)      (IMAGEPIPE_PARAM_VPORT_START+(oPort))       ///< virtual port: set share from src oPort

//used by IMAGEPIPE_PARAM_FUNC
#if defined(_BSP_NA51000_)
#define IMAGEPIPE_FUNC_SQUARE           IPL_FUNC_IME_SQUARE //3DNR effect
#endif
#if defined(_BSP_NA51023_)
#define IMAGEPIPE_FUNC_TMNR			IPL_FUNC_IME_TMNR //3DNR effect
#define IMAGEPIPE_FUNC_MD				IPL_FUNC_MD //motion detect
#define IMAGEPIPE_FUNC_WDR     		IPL_FUNC_WDR //wdr effect, (conflict with IMAGEPIPE_FUNC_SHDR and IMAGEPIPE_FUNC_STITCH)
#define IMAGEPIPE_FUNC_SHDR     		IPL_FUNC_SHDR //sensor HDR effect, (conflict with IMAGEPIPE_FUNC_WDR and IMAGEPIPE_FUNC_STITCH)
#define IMAGEPIPE_FUNC_STITCH     		IPL_FUNC_STITCH //stitch effect, (conflict with IMAGEPIPE_FUNC_WDR and IMAGEPIPE_FUNC_SHDR)
#define IMAGEPIPE_FUNC_DEFOG     		IPL_FUNC_DEFOG //defog effect, (conflict with IMAGEPIPE_FUNC_STITCH) 
#endif

//used by IMAGEPIPE_PARAM_FUNC2
#define IMAGEPIPE_FUNC2_YUVCOMPRESS		0x00000001 //enable YUV Compress of ImagePipe output 1
#define IMAGEPIPE_FUNC2_TRIGGERSINGLE 	0x00000002
#define IMAGEPIPE_FUNC2_MOTIONDETECT1	0x00000010 //enable Motion Detect of ImagePipe output 1
#define IMAGEPIPE_FUNC2_MOTIONDETECT2	0x00000020 //enable Motion Detect of ImagePipe output 2
#define IMAGEPIPE_FUNC2_MOTIONDETECT3	0x00000040 //enable Motion Detect of ImagePipe output 3
#if defined(_BSP_NA51023_)
#define IMAGEPIPE_FUNC2_VIEWTRACKING1	0x00000100 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#define IMAGEPIPE_FUNC2_VIEWTRACKING2	0x00000200 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#define IMAGEPIPE_FUNC2_VIEWTRACKING3	0x00000400 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#define IMAGEPIPE_FUNC2_VIEWTRACKING4	0x00000800 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#endif
#if defined(_BSP_NA51000_)
#define IMAGEPIPE_FUNC2_VIEWTRACKING1	0x00000100 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#define IMAGEPIPE_FUNC2_VIEWTRACKING2	0x00000200 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#define IMAGEPIPE_FUNC2_VIEWTRACKING3	0x00000400 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#define IMAGEPIPE_FUNC2_VIEWTRACKING4	0x00000800 //enable View Tracking of ImagePipe output 1,2,3,4 for DIS effect
#endif
#define IMAGEPIPE_FUNC2_ONERAW			0x00001000 //enable one raw buffer
#if defined(_BSP_NA51023_)
#define IMAGEPIPE_FUNC2_DIRECT			0x00008000 //enable direct input from VdoIn to ImagePipe (zero raw buffer)
#define IMAGEPIPE_FUNC2_DROPOLDSIZE		0x00010000 //enable drop old-size of ImagePipe output
#define IMAGEPIPE_FUNC2_TMNRSHARE		0x00020000 //enable TMNR share buffer (NOTE: only work if one buffer mode!)
#endif
#if defined(_BSP_NA51000_)
#define IMAGEPIPE_FUNC2_RSCBUF			0x00100000 //enable additional buffer for RSC effect
#define IMAGEPIPE_FUNC2_SQUARE_PAD		0x80000000 //enable padding for 3DNR effect
#endif

extern ISF_UNIT ISF_ImagePipe1;
#if (IMAGEPIPE_MAX_NUM >= 2)
extern ISF_UNIT ISF_ImagePipe2;
#endif
#if (IMAGEPIPE_MAX_NUM >= 3)
extern ISF_UNIT ISF_ImagePipe3;
#endif
#if (IMAGEPIPE_MAX_NUM >= 4)
extern ISF_UNIT ISF_ImagePipe4;
#endif
#if (IMAGEPIPE_MAX_NUM >= 5)
extern ISF_UNIT ISF_ImagePipe5;
#endif
#if (IMAGEPIPE_MAX_NUM >= 6)
extern ISF_UNIT ISF_ImagePipe6;
#endif
#if (IMAGEPIPE_MAX_NUM >= 7)
extern ISF_UNIT ISF_ImagePipe7;
#endif
#if (IMAGEPIPE_MAX_NUM >= 8)
extern ISF_UNIT ISF_ImagePipe8;
#endif


typedef void (*IPL_CB_FP)(IPL_CBMSG uiMsgID, void *Data);

#if defined(_BSP_NA51023_)
typedef UINT32 (*IPL_VIEWTRACKING_CB)(UINT32 id, UINT32 frame_cnt, INT32* pOffx, INT32* pOffy);
#endif

typedef IPL_POST_PROC_FUNC_EN ISF_IMAGEPIPE_FUNC;

typedef struct _ISF_IMAGEPIPE_EVENT_CB {
	IPL_ISR_EVENT_FP ISR_CB_FP[IPL_ISR_MAX_CNT];    ///< ISR CB for photo mode
}
ISF_IMAGEPIPE_EVENT_CB;

typedef UINT32 (*IPL_PROC_CB)(UINT32 id, UINT32 state);

extern ISF_UNIT *uiIsfIPLMapping[IMAGEPIPE_MAX_NUM];
#define ISF_IPL(id)  uiIsfIPLMapping[id]

#endif //IMAGEUNIT_IMAGEPIPE_H

