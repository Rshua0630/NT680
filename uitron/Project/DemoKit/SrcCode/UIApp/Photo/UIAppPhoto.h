#ifndef _UIAPP_PHOTO_H_
#define _UIAPP_PHOTO_H_

#include "AppLib.h"

#include "UIAppCommon.h"
#if(IPCAM_FUNC !=ENABLE)
#include "UIPhotoInfo.h"
#endif
#include "UIAppPhoto_Param.h"

#include "fd_lib.h"

//#include "sd_lib.h"

#include "ae_api.h"

#include "awb_api.h"

extern UINT32 gPhotoFrameBuf;

extern UINT32 PhotoExe_GetMaxDzoomIdx(void);
extern UINT32 PhotoExe_GetDZoomIdx(void);
extern UINT32 PhotoExe_GetMinOzoomIdx(void);
extern UINT32 PhotoExe_GetMaxOzoomIdx(void);
extern UINT32 PhotoExe_GetCurOzoomIdx(void);
extern UINT32 PhotoExe_GetDZoomRatio(void);
extern void   PhotoExe_RSC_SetSwitch(UINT32 index, UINT32 value);

#define DZOOM_IDX_MIN()         (0)
//#define UI_DZOOM_IDX_MIN        0

#define DZOOM_IDX_MAX()         PhotoExe_GetMaxDzoomIdx()
#define DZOOM_IDX_GET()         PhotoExe_GetDZoomIdx()

#define OZOOM_IDX_MIN()         PhotoExe_GetMinOzoomIdx()
#define OZOOM_IDX_MAX()         PhotoExe_GetMaxOzoomIdx()
#define OZOOM_IDX_GET()         PhotoExe_GetCurOzoomIdx()

#define CURR_EV()               AE_GetPrvEVValue()

// Zoom Control
#define UI_ZOOM_CTRL_STOP               0
#define UI_ZOOM_CTRL_IN                 1
#define UI_ZOOM_CTRL_OUT                2
#define UI_ZOOM_CTRL_RESET_DZOOM        3
#define UI_ZOOM_CTRL_RESET_OZOOM        4

#define PHOTO_FRAME_COUNT               3


// APP event class
typedef enum {
	NVTEVT_PHOTO_EVT_START      = APPUSER_PHOTO_BASE, ///< Min value = 0x14002000
	//Flow and basic parameter
	NVTEVT_EXE_PREVIEW          = 0x14002000,
	NVTEVT_EXE_CAPTURE_START    = 0x14002001,
	NVTEVT_EXE_CAPTURE_STOP     = 0x14002002,
	NVTEVT_EXE_CAPTURE_END      = 0x14002003,
	NVTEVT_EXE_START_FUNC       = 0x14002004,
	NVTEVT_EXE_STOP_FUNC        = 0x14002005,
	NVTEVT_EXE_DUALCAM          = 0x14002006,
	NVTEVT_EXE_ZOOM             = 0x14002007,
	NVTEVT_EXE_DIGITAL_ZOOM     = 0x14002008,
	NVTEVT_EXE_CAPTURE_SIZE     = 0x14002009,
	NVTEVT_EXE_IMAGE_RATIO      = 0x1400200a,
	NVTEVT_EXE_QUALITY          = 0x1400200b,
	/* INSERT NEW EVENT HRER */
	//Preview AE,AWB,AF,Color,IQ,Stable,Distortion
	NVTEVT_EXE_ISO              = 0x14002100,
	NVTEVT_EXE_METERING         = 0x14002101,
	NVTEVT_EXE_EV               = 0x14002102,
	NVTEVT_EXE_FLASH            = 0x14002103,
	NVTEVT_EXE_WB               = 0x14002104,
	NVTEVT_EXE_MACRO            = 0x14002105,
	NVTEVT_EXE_AFWINDOW         = 0x14002106,
	NVTEVT_EXE_AFBEAM           = 0x14002107,
	NVTEVT_EXE_CONTAF           = 0x14002108,
	NVTEVT_EXE_AF_PROCESS       = 0x14002109,
	NVTEVT_EXE_AF_RELEASE       = 0x1400210a,
	NVTEVT_EXE_AF_WAITEND       = 0x1400210b,
	NVTEVT_EXE_COLOR            = 0x1400210c,
	NVTEVT_EXE_SCENEMODE        = 0x1400210d,
	NVTEVT_EXE_SHARPNESS        = 0x1400210e,
	NVTEVT_EXE_SATURATION       = 0x1400210f,
	NVTEVT_EXE_EDGE             = 0x14002110,
	NVTEVT_EXE_NR               = 0x14002111,
	NVTEVT_EXE_SHDR             = 0x14002112,
	NVTEVT_EXE_WDR              = 0x14002113,
	NVTEVT_EXE_ANTISHAKING      = 0x14002114,
	NVTEVT_EXE_RSC              = 0x14002115,
	NVTEVT_EXE_GDC              = 0x14002116,
	NVTEVT_EXE_DEFOG              = 0x14002117,

	/* INSERT NEW EVENT HRER */
	//Capture Drive and Effect
	NVTEVT_EXE_SELFTIMER        = 0x14002200,
	NVTEVT_EXE_CAPTURE_MODE     = 0x14002201,
	NVTEVT_EXE_BSHOOT           = 0x14002202,
	NVTEVT_EXE_CONTSHOT         = 0x14002203,
	NVTEVT_EXE_PAN              = 0x14002204,
	//NVTEVT_EXE_CONTINUESHOT     = 0x14002205,//duplicate with NVTEVT_EXE_CONTSHOT
	NVTEVT_EXE_CAPTURE_SMILE    = 0x14002206,
	NVTEVT_EXE_FD               = 0x14002207,
	NVTEVT_EXE_FDEND            = 0x14002208,
	NVTEVT_EXE_SDEND            = 0x14002209,
	NVTEVT_EXE_INIT_DATE_BUF    = 0x1400220a,
	NVTEVT_EXE_DATE_PRINT       = 0x1400220b,
	NVTEVT_EXE_GEN_DATE_STR     = 0x1400220c,
	NVTEVT_EXE_GEN_DATE_PIC     = 0x1400220d,
	NVTEVT_EXE_PLAY_SHUTTER_SOUND = 0x1400220e,
	/* INSERT NEW EVENT HRER */
	//Event post general
	NVTEVT_CALLBACK             = 0x14002f00, ///< General callback event of photo mode
	//Event post from IPLTask CB
	NVTEVT_ALGMSG_PREVIEW       = 0x14002f01, ///< preview ready
	NVTEVT_ALGMSG_PREVIEW_STABLE = 0x14002f02, ///< preview stable with 2A
	NVTEVT_ALGMSG_DZOOMSTEP     = 0x14002f03, ///< Digital Zoom moving change one step.
	NVTEVT_ALGMSG_MWBCALEND     = 0x14002f04, ///< calculate manual WB end
	NVTEVT_ALGMSG_FOCUSEND      = 0x14002f05, ///< auto focus move end
	NVTEVT_CB_ZOOM              = 0x14002f06,  ///< Optical or Digital Zoom moving change step.
	/* INSERT NEW EVENT HRER */
	//Event post from CaptureTask CB
	NVTEVT_ALGMSG_FLASH         = 0x14002f10, ///< capture flash trigger end
	NVTEVT_ALGMSG_QVSTART       = 0x14002f11, ///< capture quick view image generated
	NVTEVT_ALGMSG_JPGOK         = 0x14002f12, ///< capture image compress JPG OK
	NVTEVT_ALGMSG_CAPFSTOK      = 0x14002f13, ///< capture image save to file System OK
	NVTEVT_ALGMSG_CAPTUREEND    = 0x14002f14, ///< capture command end
	NVTEVT_ALGMSG_SLOWSHUTTER   = 0x14002f15, ///< slow shutter
	NVTEVT_ALGMSG_PDEND         = 0x14002f16, ///< pan capture detection end
	NVTEVT_ALGMSG_PROGRESSEND   = 0x14002f17, ///< pan capture progress end
	/* INSERT NEW EVENT HRER */
	//Event post from PhotoTask CB or capture
	UIAPPPHOTO_CB_NULL          = 0x14002f20,
	UIAPPPHOTO_CB_PREVIEW_STABLE = 0x14002f21,
	UIAPPPHOTO_CB_FDEND         = 0x14002f22,
	UIAPPPHOTO_CB_SDEND         = 0x14002f23,
	UIAPPPHOTO_CB_ASCEND        = 0x14002f24,
	UIAPPPHOTO_CB_FOCUSEND      = 0x14002f25,
	UIAPPPHOTO_CB_CAPSTART      = 0x14002f26,
	UIAPPPHOTO_CB_SLOWSHUTTER   = 0x14002f27,
	UIAPPPHOTO_CB_STOP_CONTSHOT = 0x14002f28,
	UIAPPPHOTO_CB_FLASH         = 0x14002f29,
	UIAPPPHOTO_CB_QVSTART       = 0x14002f2a,
	UIAPPPHOTO_CB_JPGOK         = 0x14002f2b,
	UIAPPPHOTO_CB_FSTOK         = 0x14002f2c,
	UIAPPPHOTO_CB_CAPTUREEND    = 0x14002f2d,
	//#NT#2016/03/02#Lincy Lin -begin
	//#NT#Support object tracking function
	UIAPPPHOTO_CB_OTEND         = 0x14002f2e, // object tracking process on frame track end
	//#NT#2016/03/02#Lincy Lin -end
	//#NT#2016/05/25#David Tsai -begin
	//#NT#Support tampering detection function
	UIAPPPHOTO_CB_TDEND         = 0x14002f2f, // tampering detection process on frame track end
	//#NT#2016/05/25#David Tsai -end
	//#NT#2016/10/17#Bin Xiao -begin
	//#NT# Support Face Tracking Grading(FTG)
	UIAPPPHOTO_CB_FTGEND         = 0x14002f30,
	//#NT#2016/10/17#Bin Xiao -end

	//#NT#2016/10/14#Yuzhe Bian -begin
	//#NT# Support trip-wire detection & trip-zone detection function
	UIAPPPHOTO_CB_TWDEND        = 0x14002f31, // trip wire detection process on frame track end
	UIAPPPHOTO_CB_TZDEND        = 0x14002f32, // trip zone detection process on frame track end
	//#NT#2016/10/14#Yuzhe Bian -end
	//#NT#2016/06/08#Lincy Lin -begin
	//#NT#Implement generic OSD and video drawing mechanism for ALG function
	UIAPPPHOTO_CB_ALGEND        = 0x14002f33, // alg process on end
	//#NT#2016/06/08#Lincy Lin -end

	//#NT#2017/09/27#Bin Xiao -begin
	//#NT# Object detection&DIS Tracking
	UIAPPPHOTO_CB_ODTEND 		= 0x14002f34,
	//#NT#2017/09/27#Bin Xiao -end

    //#NT#2017/11/3#Ming Yang -begin
    //#NT# Object detection
    UIAPPPHOTO_CB_ODEND		    = 0x14002f35,
    //#NT#2017/11/3#Ming Yang -end


	
	/* INSERT NEW EVENT HRER */
	//Event post from WifTask CB
	UIAPPPHOTO_CB_UCTRL_UPDATE_UI = 0x14002f40,
	/* INSERT NEW EVENT HRER */
	NVTEVT_PHOTO_EVT_END        = APPUSER_PHOTO_BASE + 0x1000 - 1, ///< Max value = 0x14002fff
}
CUSTOM_PHOTO_EVENT;

#define UIAPPPHOTO_CB_MAX   UIAPPPHOTO_CB_NULL

#if(PHOTO_MODE==ENABLE)

typedef enum {
	ON_CAPTURE_SINGLE = 0,
	ON_CAPTURE_CONT,
	ON_CAPTURE_SD_CAPTURE,
	ON_CAPTURE_MAX
} PHOTO_ON_CAPTURE_PARM;


typedef enum {
	SD_STOP = 0,
	SD_START,
	SD_CONTINUE,
	SD_MAX
} SD_PARM;


typedef enum {
	OBJTRACK_STOP = 0,
	OBJTRACK_START,
	OBJTRACK_MAX
} OBJTRACK_PARM;


//  param NVTEVT_EXE_LOCK
#define UIAPP_PHOTO_AE    0x00000001
#define UIAPP_PHOTO_AWB   0x00000002
#define UIAPP_PHOTO_FD    0x00000004
#define UIAPP_PHOTO_AF    0x00000008
#define UIAPP_PHOTO_ASC   0x00000010


#define UIAPP_PHOTO_WAITIDLE     TRUE
#define UIAPP_PHOTO_NOWAITIDLE   FALSE



typedef enum {
	UIAPPPHOTO_Attr_FD = 0,
	UIAPPPHOTO_Attr_SD,
	UIAPPPHOTO_Attr_ASCN,
	UIAPPPHOTO_Attr_MAX
} UIAPPPHOTO_Attr;

typedef enum {
	UIAPPPHOTO_FRAME_CMD_LOADPRV_ADDR = 0,
	UIAPPPHOTO_FRAME_CMD_LOADPRV,
	UIAPPPHOTO_FRAME_CMD_LOADCAP,
	UIAPPPHOTO_FRAME_CMD_EN,
	UIAPPPHOTO_FRAME_CMD_DIS,
	UIAPPPHOTO_FRAME_CMD_MAX
} UIAPPPHOTO_FRAME_CMD;

typedef struct _PhotoFrameDataType {
	UINT32        offset;
	UINT32        size;
} PhotoFrameDataType;

typedef struct _PhotoFrameInfoType {
	UINT32             TotalLen;
	UINT32             Ver;
	UINT32             Header[2];
	PhotoFrameDataType Data[PHOTO_FRAME_COUNT * 2];
} PhotoFrameInfoType;


typedef struct _UIAppPhotoExeInfoType {
	UINT32                     FreePicNum;
	BOOL                       isFolderFull;
	BOOL                       isCardFull;
	BOOL                       isFocusEnd;
	BOOL                       isStartCapture;
	BOOL                       isContShotStateSetOn;
	BOOL                       isDoingContShot;
	BOOL                       isStopingContShot;
	BOOL                       isStartSmileDetect;       //
	UINT32                     uiTakePicNum;
	BOOL                       isAscnOn;                 //
	BOOL                       isCafOn;                  //
	BOOL                       isSdOn;                   //
	BOOL                       isFdOn;                   //
	BOOL                       IsJPGok;                  //
	BOOL                       IsAFProcess;              //
	BOOL                       BDstatus;
	BOOL                       IsCallBack;
	UINT32                     CallBackEvent;
	BOOL                       isPhotoFrameOn;           //
	BOOL                       isFlashTrigger;           //
	BOOL                       isSingleViewOpen;         //
	BOOL                       isPrvPostProcess;
	BOOL                       isAFBeam;
	UINT32                     uiMaxImageSize;
	BOOL                       isDzoomStart;
	UINT32                     ZoomCtrl;
	BOOL                       isStartHighSpeedTone;
	URECT                      FdDispCoord;
	UINT32                     sensorCount;
	UINT32                     DualCam;
} UIAppPhotoExeInfoType;

extern UIAppPhotoExeInfoType *pPhotoExeInfo;
extern PhotoFrameInfoType     gPFInfo;
extern UINT32 PhotoExe_GetFreePicNum(void);
extern void   PhotoExe_SetData(UINT32 attribute, UINT32 value);
extern UINT32 PhotoExe_GetFdFaceNum(void);
extern void PhotoExe_DZoomReset(void);
extern void PhotoExe_DZoomInBK(UINT32 *pIsDzoomStop);
extern void PhotoExe_DZoomOutBK(UINT32 *pIsDzoomStop);

//Photo Init
extern INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

//Photo Exe
extern INT32 PhotoExe_OnAFWindow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnPreview(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnDatePrint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnAFBeam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 PhotoExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);


//Photo CB
extern void PhotoExe_DateImprint(void);
extern void Photo_setS2Status(BOOL isPressed);
extern void Photo_RegCB(void);
extern VControl CustomPhotoObjCtrl;

extern void PhotoExe_OpenHttpLiveView(void);
extern void PhotoExe_CloseHttpLiveView(void);
extern void PhotoExe_InitNetHttp(void);
extern BOOL FlowPhoto_CheckReOpenItem(void);
#endif

#endif //_UIAPP_PHOTO_H_
