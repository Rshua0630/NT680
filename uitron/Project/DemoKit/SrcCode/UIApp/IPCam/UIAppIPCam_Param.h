#ifndef _UIAPPIPCAM_Param_H
#define _UIAPPIPCAM_Param_H

#include "PrjCfg.h"
#include "ipl_ctrl_flow.h"
#include "Type.h"
#include "GxImage.h"
#include "ImageApp_IPCam.h"
#include "nvtmpp.h"
#include "NvtDepth.h"
#include "VideoEncode.h"
#include "NMediaRecAudEnc.h"

#define IPCAM_FMT_420 	GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define IPCAM_FMT_422 	GX_IMAGE_PIXEL_FMT_YUV422_PACKED
#define IPCAM_FMT_444 	GX_IMAGE_PIXEL_FMT_YUV444_PLANAR
#define IPCAM_FMT_Y   	GX_IMAGE_PIXEL_FMT_Y_ONLY

#define IPCAM_FMT_RAW8	GX_IMAGE_PIXEL_FMT_RAW8
#define IPCAM_FMT_RAW10	GX_IMAGE_PIXEL_FMT_RAW10
#define IPCAM_FMT_RAW12	GX_IMAGE_PIXEL_FMT_RAW12
#define IPCAM_FMT_RAW16	GX_IMAGE_PIXEL_FMT_RAW16

#if defined(_BSP_NA51000_)
#define IPCAM_FMT_ODRL (0xff+0)
#endif
#if defined(_BSP_NA51023_)
#define IPCAM_FMT_420_MD 	(0x01ff+GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
#define IPCAM_FMT_422_MD 	(0x01ff+GX_IMAGE_PIXEL_FMT_YUV422_PACKED)
#endif

typedef enum {
    IPCAMCODEC_START =1,   //mapping  MEDIAVIDENC_MJPG, MEDIAVIDENC_H264, MEDIAVIDENC_H265
	IPCAMCODEC_MJPG =IPCAMCODEC_START,
	IPCAMCODEC_H264 ,
	IPCAMCODEC_H265 ,
	IPCAMCODEC_YUV  ,
	IPCAMCODEC_MAX  ,
	ENUM_DUMMY4WORD(UIAPPIPCAM_CODEC)

}UIAPPIPCAM_CODEC;


typedef enum {
	UIAPPIPCAM_SENSOR1 = 0,
	UIAPPIPCAM_SENSOR2,
#if defined(_BSP_NA51000_)
	UIAPPIPCAM_SENSOR3,
	UIAPPIPCAM_SENSOR4,
	UIAPPIPCAM_SENSOR5,
	UIAPPIPCAM_SENSOR6,
	UIAPPIPCAM_SENSOR7,
	UIAPPIPCAM_SENSOR8,
#endif
	UIAPPIPCAM_SENSOR_MAX,
	ENUM_DUMMY4WORD(UIAPPIPCAM_SENSOR)

} UIAPPIPCAM_SENSOR;

typedef enum{

	UIAPPIPCAM_VDOOUT_UNIT,
	UIAPPIPCAM_MAX_UNIT,
	ENUM_DUMMY4WORD(UIAPPIPCAM_UNIT)
}UIAPPIPCAM_UNIT;

typedef enum {
	UIAPPIPCAM_PATH_ID_0 = 0,
	UIAPPIPCAM_PATH_ID_1 = 1,
	UIAPPIPCAM_PATH_ID_2 = 2,
	UIAPPIPCAM_PATH_ID_3 = 3,
	UIAPPIPCAM_PATH_ID_4 = 4,
	UIAPPIPCAM_PATH_ID_5 = 5,
	UIAPPIPCAM_PATH_ID_6 = 6,
	UIAPPIPCAM_PATH_ID_7 = 7,
#if defined(_BSP_NA51000_)
	UIAPPIPCAM_PATH_ID_8 = 8,
	UIAPPIPCAM_PATH_ID_9 = 9,
	UIAPPIPCAM_PATH_ID_10 = 10,
	UIAPPIPCAM_PATH_ID_11 = 11,
	UIAPPIPCAM_PATH_ID_12 = 12,
	UIAPPIPCAM_PATH_ID_13 = 13,
	UIAPPIPCAM_PATH_ID_14 = 14,
	UIAPPIPCAM_PATH_ID_15 = 15,
#endif
	UIAPPIPCAM_PATH_MAX_ID,
	ENUM_DUMMY4WORD(UIAPPIPCAM_PATH_ID)
} UIAPPIPCAM_PATH_ID;


typedef struct {

	UINT32 uiScaleWidth;
	UINT32 uiScaleHeight;
	URECT  sCropRECT;
}UIAPPIPCAM_CROPINFO;
typedef struct {
    UIAPPIPCAM_SENSOR      sensor;

	UIAPPIPCAM_CROPINFO	   sCropInfo[5];
}UIAPPIPCAM_CROP_INFO;

typedef struct {
	UIAPPIPCAM_SENSOR      sensor;
	UINT32              uiFrontFPS;
	UINT32              uiIMEFPS[5];
	ISIZE               sRatio;
	FLIP_TYPE           eFlipType;
	BOOL                bIME3DNR;
	GX_IMAGE_PIXEL_FMT  eVideoFormat[5];
	BOOL                bIME1Compress;
	BOOL				bCropEnable[5];

} UIAPPIPCAM_SENSOR_INFO;

typedef struct {

	UINT32    uiWDRMode;
	UINT32    uiWDRLevel;

}UIAPPIPCAM_WDR_INFO;

typedef struct{


	UINT32 uiHDRMode;
	UINT32 uiFrameNum;

}UIAPPIPCAM_HDR_INFO;

typedef struct{

	SEL_DEFOG eDeFogMode;
	UINT32 uiLevel;

}UIAPPIPCAM_DEFOG_INFO;

typedef struct {

	UIAPPIPCAM_SENSOR      sensor;
	UIAPPIPCAM_HDR_INFO	   sHDR;
	UINT32				   uiDIS;
	UIAPPIPCAM_WDR_INFO    sWDR;
	UIAPPIPCAM_DEFOG_INFO  sDefog;

} UIAPPIPCAM_IPL_FUNC;



typedef enum {
	UIAPPIPCAM_BRC_NONE,
	UIAPPIPCAM_BRC_CBR,
	UIAPPIPCAM_BRC_VBR,
	UIAPPIPCAM_BRC_FIXQP,
	UIAPPIPCAM_BRC_EVBR,
	UIAPPIPCAM_BRC_MAX,
	ENUM_DUMMY4WORD(UIAPPIPCAM_BRC_TYPE)
} UIAPPIPCAM_BRC_TYPE;

typedef enum{
	UIAPPIPCAM_RT_NONE,			// no rotate
	UIAPPIPCAM_RT_BUILDIN_90,    //for ISF_VdoEnc rotate 90 Clockwise
	UIAPPIPCAM_RT_BUILDIN_270,   //for ISF_VdoEnc rotate 270 Clockwise
	UIAPPIPCAM_RT_EXTEND_90,     //for ISF_ImgTrans rotate 90 Clockwise
	UIAPPIPCAM_RT_EXTEND_180,	//for ISF_ImgTrans rotate 180 Clockwise
	UIAPPIPCAM_RT_EXTEND_270,	//for ISF_ImgTrans rotate 270 Clockwise
	UIAPPIPCAM_RT_EXTEND_MAX,
	ENUM_DUMMY4WORD(UIAPPIPCAM_RT_TYPE)
}UIAPPIPCAM_RT_TYPE;

typedef struct {

    BOOL bEnable;

}UIAPPIPCAM_CODEC_NR_SETTING;




typedef struct {

    BOOL bEnable;

}UIAPPIPCAM_CODEC_SEI_SETTING;



typedef struct {

	UIAPPIPCAM_PATH_ID      		path;
	ISIZE                   		resolution;
	UIAPPIPCAM_CODEC                codec;
	UIAPPIPCAM_CODEC_NR_SETTING   	sNRSetting; //encoder 3DNR
	UIAPPIPCAM_BRC_TYPE     		eBrcType;
	UIAPPIPCAM_RT_TYPE      		eRotateType;
	MP_VDOENC_SVC					eSVCType;
	MP_VDOENC_LTR_INFO				sLTRInfo;
	MP_VDOENC_PROFILE               eProfile;
	BOOL							bSmartROI;
	UIAPPIPCAM_CODEC_SEI_SETTING	sSEISetting;
} UIAPPIPCAM_ENCODE_CONFIG;

typedef struct {

	UIAPPIPCAM_PATH_ID		path;
	UINT32 					uiDeltaQp;		// 0 = all fixed qp, 1 = all delta qp
	BOOL    				bSmartROI;      // 0: Disable SmartROI    1: Enable SmartROI  (if Enable, Should Close all ROIConfig to disable)
	_DAL_VDOENC_ROI_INFO 	roi[MP_VDOENC_ROI_NUM_MAX];
}UIAPPIPCAM_ROI_CONFIG;

typedef struct {

	UIAPPIPCAM_PATH_ID      path;
	MP_VDOENC_CBR_INFO     cbr_info;
} UIAPPIPCAM_CBR_CONFIG;

typedef struct {

	UIAPPIPCAM_PATH_ID      path;
	MP_VDOENC_VBR_INFO     vbr_info;
} UIAPPIPCAM_VBR_CONFIG;

typedef struct {

	UIAPPIPCAM_PATH_ID      path;
	MP_VDOENC_EVBR_INFO     evbr_info;
} UIAPPIPCAM_EVBR_CONFIG;

typedef struct {

	UIAPPIPCAM_PATH_ID      ePath;
	MP_VDOENC_FIXQP_INFO		sFixQPInfo;

}UIAPPIPCAM_FIXQP_CONFIG;

typedef struct {

	UIAPPIPCAM_PATH_ID      path;
	UINT32    uiFrameRate;
	UINT32    uiGOP;
	UINT32    uiTargetByterate;
} UIAPPIPCAM_BRC_NONE_CONFIG;


typedef struct {

	UIAPPIPCAM_PATH_ID      path;
	MP_VDOENC_AQ_INFO			sAQInfo;

}UIAPPIPCAM_AQ_CONFIG;

typedef struct {

	UINT32 codec;
	UINT32 channel;
	UINT32 SampleRate;
	UINT32 volume;


} UIAPPIPCAM_AUDIO_INFO;


typedef struct {
	BOOL isEnabled;
	ISIZE sResolution;
	UINT32 count;
	NVTMPP_DDR ddr;
	BOOL bCompress;
	UINT32 defaultFormat;
} UIAPPIPCAM_BLOCK_BUFFER_INFO;



typedef enum {
	UIAPPIPCAM_VOUT_4K2KP30,
	UIAPPIPCAM_VOUT_4K2KP25,
	UIAPPIPCAM_VOUT_4K2KP24,
	UIAPPIPCAM_VOUT_UHDP30,
	UIAPPIPCAM_VOUT_UHDP25,
	UIAPPIPCAM_VOUT_UHDP24,
	UIAPPIPCAM_VOUT_FHDI60,
	UIAPPIPCAM_VOUT_FHDP30,
	UIAPPIPCAM_VOUT_HD,
	UIAPPIPCAM_VOUT_AUTO,
	UIAPPIPCAM_VOUT_CVBS_NTSC,
	UIAPPIPCAM_VOUT_CVBS_PAL,
	UIAPPIPCAM_VOUT_SETTING_MAX,
	ENUM_DUMMY4WORD(UIAPPIPCAM_VOUT_MODE)
} UIAPPIPCAM_VOUT_MODE;

typedef struct{

	UIAPPIPCAM_VOUT_MODE sMode;
	BOOL					bEnable;

} UIAPPIPCAM_VOUT_SETTING;

typedef struct{

	UIAPPIPCAM_PATH_ID   path;
	UIAPPIPCAM_CODEC     sMaxCodec;
	UINT32     uiMaxWidth;
	UINT32     uiMaxHeight;
	UINT32 	   uiMaxByteRate;
	UINT32     uiMaxBufMs;
	UINT32     uiMaxRecFormat;
	UINT32     uiMaxSVCLayer;
	UINT32     uiLTRInterval;
	UINT32     uiRotate;
	BOOL       bAllocSnapshotBuf;
}UIAPPIPCAM_VIDEO_MAXCONFIG;

typedef struct{
	UIAPPIPCAM_SENSOR      eSensor;
	UINT32				   uiRatioThreshold; ///< ime scale ratio threshold [31:16]/[15:0](src:bit[31:16], div:bit[15:0])
	IPL_IME_SCALER		   eMathodLarge;
	IPL_IME_SCALER		   eMathodSmall;
}UIAPPIPCAM_IME_SCALE_TYPE;

typedef struct{

	UIAPPIPCAM_PATH_ID   sPath;
	BOOL				 bPullData;

}UIAPPIPCAM_USER_PULL_DATA;


typedef struct{

	BOOL				 bEnable;
	UIAPPIPCAM_PATH_ID   sInPath1;
	UIAPPIPCAM_PATH_ID	 sInPath2;
	UIAPPIPCAM_PATH_ID   sOutPath;
	DEPTH_PARAM			 sDepthInfo;
	BOOL				 bSBSView;

}UIAPPIPCAM_DEPTH_CONFIG;

extern void UIAppIPCam_set_SensorInfo(UIAPPIPCAM_SENSOR_INFO *SensorInfo);

extern UIAPPIPCAM_SENSOR_INFO *UIAppIPCam_get_SensorInfo(void);

extern void UIAppIPCam_set_EncodeConfig(UIAPPIPCAM_ENCODE_CONFIG *EncodeConfig);

extern UIAPPIPCAM_ENCODE_CONFIG *UIAppIPCam_get_EncodeConfig(void);

extern IPCAM_STREAM_INFO *UIAppIPCam_get_streamInfo(void);

extern UIAPPIPCAM_BLOCK_BUFFER_INFO *UIAppIPCam_get_BlockBufferInfo(void);

extern void UIAppIPCam_set_StreamInfo(IPCAM_STREAM_INFO *streamInfo);

extern void UIAppIPCam_set_DisplayPath(INT32 path);

extern INT32 UIAppIPCam_get_DisplayPath(void);

extern INT32 UIAppIPCam_get_ALGPath(void);

extern void UIAppIPCam_set_ALGPath(INT32 path);

extern UIAPPIPCAM_AUDIO_INFO *UIAppIPCam_get_AudioInfo(void);

extern void UIAppIPCam_set_AudioInfo(UIAPPIPCAM_AUDIO_INFO *pAudioInfo);

extern UIAPPIPCAM_AUDIO_INFO *UIAppIPCam_get_AudioOutInfo(void);

extern void UIAppIPCam_set_AudioOutInfo(UIAPPIPCAM_AUDIO_INFO *pAudioOutInfo);

extern void UIAppIPCam_set_VoutInfo(UIAPPIPCAM_VOUT_SETTING *pVoutInfo);

extern UIAPPIPCAM_VOUT_SETTING *UIAppIPCam_get_VoutInfo(void);

extern UIAPPIPCAM_CBR_CONFIG *UIAppIPCam_get_CBRInfo(void);

extern UIAPPIPCAM_VBR_CONFIG *UIAppIPCam_get_VBRInfo(void);

extern UIAPPIPCAM_BRC_NONE_CONFIG *UIAppIPCam_get_BRCNoneInfo(void);

extern void UIAppIPCam_set_VBRInfo(UIAPPIPCAM_VBR_CONFIG *pVBRInfo);

extern void UIAppIPCam_set_CBRInfo(UIAPPIPCAM_CBR_CONFIG *pCBRInfo);

extern void UIAppIPCam_set_BRCNoneInfo(UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneInfo);

extern void UIAppIPCam_set_CropInfo(UIAPPIPCAM_CROP_INFO *pCropInfo);

extern UIAPPIPCAM_CROP_INFO *UIAppIPCam_get_CropInfo(void);

extern void UIAppIPCam_set_FixQPInfo(UIAPPIPCAM_FIXQP_CONFIG *pFixQPInfo);

extern UIAPPIPCAM_FIXQP_CONFIG *UIAppIPCam_get_FixQPInfo(void);

extern void UIAppIPCam_set_AQInfo(UIAPPIPCAM_AQ_CONFIG *pAQInfo);

extern UIAPPIPCAM_AQ_CONFIG *UIAppIPCam_get_AQInfo(void);

extern void UIAppIPCam_set_IPLFuncInfo(UIAPPIPCAM_IPL_FUNC *pIPLFuncInfo);

extern UIAPPIPCAM_IPL_FUNC *UIAppIPCam_get_IPLFuncInfo(void);

extern UIAPPIPCAM_VIDEO_MAXCONFIG *UIAppIPCam_get_VdoMaxConfig(void);

extern void UIAppIPCam_Set_Unit_Status(UIAPPIPCAM_UNIT unit, ISF_PORT_STATE status);

extern ISF_PORT_STATE UIAppIPCam_Get_Unit_Status(UIAPPIPCAM_UNIT unit);

extern void UIAppIPCam_set_EVBRInfo(UIAPPIPCAM_EVBR_CONFIG *pEVBRInfo);

extern UIAPPIPCAM_EVBR_CONFIG *UIAppIPCam_get_EVBRInfo(void);

extern INT32 UIAppIPCam_get_UVACPath(void);

extern void UIAppIPCam_set_UVACPath(INT32 path);

extern void UIAppIPCam_set_UserPullDataInfo(UIAPPIPCAM_USER_PULL_DATA *pPullDataInfo);

extern UIAPPIPCAM_USER_PULL_DATA *UIAppIPCam_get_UserPullDataInfo(void);

extern WAVSTUD_INFO_SET * UIAppIPCam_get_AudioInMaxInfo(void);

extern NMI_AUDENC_MAX_MEM_INFO * UIAppIPCam_get_AudioEncMaxInfo(void);

extern WAVSTUD_INFO_SET * UIAppIPCam_get_AudioOutMaxInfo(void);

extern NMI_AUDDEC_MAX_MEM_INFO * UIAppIPCam_get_AudioDecMaxInfo(void);

extern STMRCV_MAX_MEM_INFO * UIAppIPCam_get_StreamReceiverMaxInfo(void);

extern void UIAppIPCam_set_IMEScaleInfo(UIAPPIPCAM_IME_SCALE_TYPE *pIMEInfo);

extern UIAPPIPCAM_IME_SCALE_TYPE * UIAppIPCam_get_IMEScaleInfo(void);

extern void UIAppIPCam_set_DepthInfo(UIAPPIPCAM_DEPTH_CONFIG *pDepthInfo);

extern UIAPPIPCAM_DEPTH_CONFIG * UIAppIPCam_get_DepthInfo(void);

extern INT32 UIAppIPCam_get_YUVPath(void);

extern void UIAppIPCam_set_YUVPath(INT32 path);

extern void UIAppIPCam_set_ROIInfo(UIAPPIPCAM_ROI_CONFIG *pROIInfo);

extern UIAPPIPCAM_ROI_CONFIG *UIAppIPCam_get_ROIInfo(void);

extern void UIAppIPCam_set_RAWPath(INT32 path);

extern INT32 UIAppIPCam_get_RAWPath(void);

#endif

