#ifndef _UIVIEWMOVIE_H_
#define _UIVIEWMOVIE_H_

#include "AppLib.h"
#include "UIAppCommon.h"
#if (IPCAM_FUNC != ENABLE)
#include "UIMovieInfo.h"
#endif
#if(UI_FUNC==ENABLE)
#include "UIBackgroundObj.h"
#endif
#include "NVTEvent.h"

#define MOVOBJ_REC_JPEG_FORMAT444      0
#define MOVOBJ_REC_JPEG_FORMAT422      1
#define MOVOBJ_REC_JPEG_FORMAT420      2
#define MOVOBJ_REC_AUD_VID_BOTH        0
#define MOVOBJ_REC_VID_ONLY            1
#define MOVOBJ_REC_QUALITY_BEST        0
#define MOVOBJ_REC_QUALITY_NORMAL      1
#define MOVOBJ_REC_QUALITY_DRAFT       2

//----- record -----
#define MOVREC_REC_STATUS_NOT_OPENED                1
#define MOVREC_REC_STATUS_NOT_RECORD                2
#define MOVREC_REC_STATUS_NOT_RECORDING             3

#define MOVREC_EVENT_RESULT_NORMAL                  1        //1    ///< Finish normally
#define MOVREC_EVENT_RESULT_HW_ERR                  2        //2    ///< Hardware error
#define MOVREC_EVENT_RESULT_FULL                    3          //3    ///< Card full
#define MOVREC_EVENT_RESULT_SLOW                    4          //4    ///< Write card too slowly
#define MOVREC_EVENT_RESULT_NOTREADY                5      //5    ///< Not ready
#define MOVREC_EVENT_ONE_SECOND                     6           //6    ///< One second arrives
#define MOVREC_EVENT_RESULT_OVERTIME                7      //7    ///< Overtime
#define MOVREC_EVENT_RESULT_OVERWRITE               8     //8    ///< Buffer overwrite
#define MOVREC_EVENT_RESULT_INVALIDFILE             9   //9    ///< SAVE INVALIDFILE
#define MOVREC_EVENT_RESULT_WRITE_ERR               10     //10

typedef enum {
	RECMOVIE_AUD_OFF  =   0,  //   0%
	RECMOVIE_AUD_VOL1 =  13,  //  13%
	RECMOVIE_AUD_VOL2 =  25,  //  25%
	RECMOVIE_AUD_VOL3 =  37,  //  37%
	RECMOVIE_AUD_VOL4 =  50,  //  50%
	RECMOVIE_AUD_VOL5 =  63,  //  63%
	RECMOVIE_AUD_VOL6 =  75,  //  75%
	RECMOVIE_AUD_VOL7 =  87,  //  87%
	RECMOVIE_AUD_ON   = 100,  // 100%
	RECMOVIE_AUD_MAX
} RECMOVIE_AUD_TYPE;


//------------------------------------------------------------

typedef enum {
	RECMOVIE_MAXSECOND,
	RECMOVIE_REC_STATUS,
	RECMOVIE_SAVING_STATUS,
	//#NT#2011/02/10#Photon Lin -begin
	//#Add file access
	RECMOVIE_QUEUE_STATUS,
	//#NT#2011/02/10#Photon Lin -end
	RECMOVIE_GET_DATATYPE_MAX
} RECMOVIE_GET_DATATYPE;


#include "GPS.h"

// APP event class

typedef enum {
	NVTEVT_MOVIE_EVT_START      = APPUSER_MOVIE_BASE, ///< Min value = 0x14001000
	//Flow and basic parameter
	NVTEVT_EXE_MOVIE_REC_START  = 0x14001000,
	NVTEVT_EXE_MOVIE_REC_STEP   = 0x14001001,
	NVTEVT_EXE_MOVIE_REC_STOP   = 0x14001002,
	NVTEVT_EXE_MOVIE_REC_PIM    = 0x14001003, // picture in movie (still image is full resolution)
	NVTEVT_EXE_MOVIE_REC_RAWENC = 0x14001004, // raw encode (still image size is equal to or smaller than video size)
	NVTEVT_EXE_MOVIESIZE        = 0x14001005,
	NVTEVT_EXE_MOVIEQUALITY     = 0x14001006,
	NVTEVT_EXE_MOVIEDZOOM       = 0x14001007,
	NVTEVT_EXE_MOVIE_AUDIO      = 0x14001008,
	NVTEVT_EXE_MOVIE_AUDIO_VOL  = 0x14001009,
	NVTEVT_EXE_DUAL_REC         = 0x14001010,
	NVTEVT_EXE_MOVIE_AUDIO_REC  = 0x14001011,
    NVTEVT_EXE_MOVIE_STRM_START	= 0x14001012,
    NVTEVT_EXE_MOVIE_STRM_STOP	= 0x14001013,
	/* INSERT NEW EVENT HRER */
	//Preview AE,AWB,AF,Color,IQ,Stable,Distortion
	NVTEVT_EXE_MOVIE_EV         = 0x14001100,
	NVTEVT_EXE_MOVIECONTAF      = 0x14001101,
	NVTEVT_EXE_MOVIEMETERING    = 0x14001102,
	NVTEVT_EXE_MOVIECOLOR       = 0x14001103,
	NVTEVT_EXE_MOVIE_MCTF       = 0x14001104,
	NVTEVT_EXE_MOVIE_EDGE       = 0x14001105,
	NVTEVT_EXE_MOVIE_NR         = 0x14001106,
	NVTEVT_EXE_MOVIE_RSC        = 0x14001107,
	NVTEVT_EXE_MOVIE_HDR        = 0x14001108,
	NVTEVT_EXE_MOVIE_WDR        = 0x14001109,
	NVTEVT_EXE_MOVIEGDC         = 0x1400110a,
	NVTEVT_EXE_MOVIESMEAR       = 0x1400110b,
	NVTEVT_EXE_MOVIEDIS         = 0x1400110c,
	NVTEVT_EXE_MOVIEDIS_ENABLE  = 0x1400110d,
	NVTEVT_EXE_MOVIE_IR_CUT     = 0x1400110e,
	NVTEVT_EXE_MOVIE_SENSOR_ROTATE = 0x14001110,  // Move to System obj!!!
	//#NT#2016/06/03#Charlie Chang -begin
	//#NT# addcontrast AudioIn,flip mirror, quality set
	NVTEVT_EXE_MOVIE_CONTRAST   = 0x14001111,
	NVTEVT_EXE_MOVIE_AUDIOIN    = 0x14001112,
	NVTEVT_EXE_MOVIE_AUDIOIN_SR = 0x14001113,
	NVTEVT_EXE_MOVIE_FLIP_MIRROR = 0x14001114,
	NVTEVT_EXE_MOVIE_QUALITY_SET = 0x14001115,
	//#NT#2016/06/03#Charlie Chang -end
	/* INSERT NEW EVENT HRER */
	//Record func and Effect
	NVTEVT_EXE_MOVIE_FDEND      = 0x14001200,
	NVTEVT_EXE_MOVIE_AUTO_REC   = 0x14001201,
	NVTEVT_EXE_CYCLIC_REC       = 0x14001202,
	NVTEVT_EXE_MOTION_DET       = 0x14001203,
	NVTEVT_EXE_GSENSOR          = 0x14001204,
	NVTEVT_EXE_DATEIMPRINT       = 0x14001205,
	NVTEVT_EXE_MOVIE_DATE_IMPRINT   = 0x14001206,
	NVTEVT_EXE_MOVIE_PROTECT_AUTO   = 0x14001207,
	NVTEVT_EXE_MOVIE_PROTECT_MANUAL = 0x14001208,
	NVTEVT_EXE_MOVIE_PTZ        = 0x14001209,
	NVTEVT_EXE_MOVIE_LDWS       = 0x1400120a,
	NVTEVT_EXE_MOVIE_FCW        = 0x1400120b,
	NVTEVT_EXE_VIDEO_PAUSE_RESUME = 0x1400120c,
	//#NT#2016/03/02#Lincy Lin -begin
	//#NT#Support object tracking function
	NVTEVT_EXE_MOVIE_OTEND      = 0x1400120d,   // object tracking process on frame track end
	//#NT#2016/03/02#Lincy Lin -end
	//#NT#2016/05/25#David Tsai -begin
	//#NT#Support tampering detection function
	NVTEVT_EXE_MOVIE_TDEND      = 0x1400120e,   // tampering detection process on frame track end
	//#NT#2016/05/25#David Tsai -end
	//#NT#2016/10/17#Bin Xiao -begin
	//#NT# Support Face Tracking Grading(FTG)
	NVTEVT_EXE_MOVIE_FTGEND     = 0x1400120f,
	//#NT#2016/10/17#Bin Xiao -end

	//#NT#2016/10/14#Yuzhe Bian -begin
	//#NT# Support trip-wire detection & trip-zone detection function
	NVTEVT_EXE_MOVIE_TWDEND     = 0x14001210,   // trip-wire detection process on frame track end
	NVTEVT_EXE_MOVIE_TZDEND     = 0x14001211,   // trip-zone detection process on frame track end
	//#NT#2016/10/14#Yuzhe Bian -end
	//#NT#2016/06/08#Lincy Lin -begin
	//#NT#Implement generic OSD and video drawing mechanism for ALG function
	NVTEVT_EXE_MOVIE_ALGEND     = 0x14001212,   // alg process on end
	//#NT#2016/06/08#Lincy Lin -end
	NVTEVT_EXE_MOTION_DET_RUN   = 0x14001213,
	//#NT#2016/03/25#KCHong -begin

	//#NT#2017/09/27#Bin Xiao -begin
	//#NT# Object detection&DIS Tracking
	NVTEVT_EXE_MOVIE_ODTEND		= 0x14001214,
	//#NT#2017/09/27#Bin Xiao -end

    //#NT#2017/11/3#Ming Yang -begin
    //#NT# Object detection
    NVTEVT_EXE_MOVIE_ODEND		= 0x14001215,
    //#NT#2017/11/3#Ming Yang -end
    NVTEVT_EXE_MOVIE_CODEC		= 0x14001216,


	//#NT#New ADAS
	// Evnet for ADAS
	NVTEVT_CB_ADAS_SETCROPWIN   = 0x14001300,
	NVTEVT_CB_ADAS_SHOWALARM    = 0x14001301,
	//#NT#2016/03/25#KCHong -end
	//#NT#2016/07/20#Brain Yen -begin
	//#NT#Event for DDD alarm
	NVTEVT_CB_DDD_SHOWALARM    = 0x14001302,
	//#NT#2016/07/20#Brain Yen -end

	/* INSERT NEW EVENT HRER */
	//Event post from IPLTsk
	NVTEVT_CB_PREVIEWSTABLE     = 0x14001f00,
	NVTEVT_CB_OZOOMSTEPCHG      = 0x14001f01,
	NVTEVT_CB_DZOOMSTEPCHG      = 0x14001f02,
	/* INSERT NEW EVENT HRER */
	//Event post from MediaTsk
	NVTEVT_CB_MOVIE_REC_ONE_SEC = 0x14001f10,
	NVTEVT_CB_MOVIE_REC_FINISH  = 0x14001f11,
	NVTEVT_CB_MOVIE_FULL        = 0x14001f12,
	NVTEVT_CB_MOVIE_SLOW        = 0x14001f13,
	NVTEVT_CB_MOVIE_SLOWMEDIA   = 0x14001f14,  ///< slow media
	NVTEVT_CB_MOVIE_FILEACCESS  = 0x14001f15, ///< file access with case of card full
	NVTEVT_CB_MOVIE_OVERTIME    = 0x14001f16,
	NVTEVT_CB_MOVIE_PIM_READY   = 0x14001f17,
	NVTEVT_CB_MOVIE_WR_ERROR    = 0x14001f18,
	NVTEVT_CB_MOVIE_LOOPREC_FULL = 0x14001f19,
	NVTEVT_CB_MOVIE_CONTINUOUS_RECORD = 0x14001f1a,
	NVTEVT_CB_EMR_COMPLETED     = 0x14001f1b,
	NVTEVT_CB_MOVIE_VEDIO_READY = 0x14001f1c,
	/* INSERT NEW EVENT HRER */
	//Event post from MediaTsk RawEncode
	NVTEVT_CB_RAWENC_OK         = 0x14001ff0,
	NVTEVT_CB_RAWENC_ERR        = 0x14001ff1,
	NVTEVT_CB_RAWENC_WRITE_ERR  = 0x14001ff2,
	NVTEVT_CB_RAWENC_DCF_FULL   = 0x14001ff3,
	/* INSERT NEW EVENT HRER */
	NVTEVT_MOVIE_EVT_END        = APPUSER_MOVIE_BASE + 0x1000 - 1, ///< Max value = 0x14001fff
} CUSTOM_MOVIE_EVENT;

//#NT#2013/2/6#Philex -begin
typedef struct {
	char            IQVer[16];
	char            IQBuildDate[16];
	RMCINFO         rmcinfo;

//#NT#2011/10/14#Spark Chou -begin
//#NT# Video IQ debug info
#if _MOVIE_IQLOG_
	char    IQDebugInfo[5120];
#endif
//#NT#2011/10/14#Spark Chou -end
} GPSDATA, *pGPSDATA;


//#NT#2016/03/25#KCHong -begin
//#NT#New ADAS
typedef enum _ADAS_ALARM {
	ADAS_ALARM_FC = 0,
	ADAS_ALARM_LD = 1,
	ADAS_ALARM_STOP = 2,
	ADAS_ALARM_GO = 3,
	ADAS_ALARM_MAX_CNT,
	ENUM_DUMMY4WORD(_ADAS_ALARM)
} ADAS_ALARM;
//#NT#2016/03/25#KCHong -end
//#NT#2016/07/20#Brain Yen -begin
//#NT#For DDD alarm
typedef enum _DDD_ALARM {
	DDD_ALARM_PERCLOS = 0,
	DDD_ALARM_YAWN = 1,
	DDD_ALARM_DIS = 2,
	DDD_ALARM_NODE = 3,
	DDD_ALARM_EYE = 4,
	ENUM_DUMMY4WORD(_DDD_ALARM)
} DDD_ALARM;
//#NT#2016/07/20#Brain Yen -end
//Movie Config
extern ISIZE Movie_GetBufferSize(void);

//Movie Exe
extern INT32 MovieExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnDis(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern INT32 MovieExe_OnSmear(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

extern void Movie_SetUserData(void);

extern UINT32 MovieExe_GetMaxRecSec(void);
extern UINT32 MovieExe_GetFreeRecSec(void);
extern void   MovieExe_RSC_SetSwitch(UINT32 index, UINT32 value);
extern void   MovieExe_IPL_SetIInfo(UINT32 index, UINT32 value);

extern void Movie_RegCB(void);

extern void UIMovie_IRCutCtrl(BOOL isON);

extern VControl CustomMovieObjCtrl;

extern void Movie_SetSDSlow(BOOL IsSlow);

#if MOVIE_MAPPING_MULTIREC
extern UINT32 Movie_GetSensorRecMask(void);
extern void Movie_SetSensorRecMask(UINT32 mask);
extern UINT32 Movie_GetMovieRecMask(void);
extern UINT32 Movie_GetCloneRecMask(void);
extern void Movie_SetResvSize(void);
extern UINT32 Movie_GetFreeSec(void);
#endif

// For UCTRL use
extern UINT32 Movie_GetAudChannel(void);
extern void   Movie_SetAudChannel(UINT32 uiAudChannel);
extern UINT32 Movie_GetAudSampleRate(void);
extern void   Movie_SetAudSampleRate(UINT32 uiAudSampleRate);
extern UINT32 Movie_GetAudCodec(void);
extern void   Movie_SetAudCodec(UINT32 uiAudCodec);

//#NT#2013/10/29#Isiah Chang -begin
//#Implement YUV merge mode of recording func.
extern void   FlowMovie_RecSetYUVMergeMode(BOOL bEnable);
extern BOOL   FlowMovie_RecGetYUVMergeMode(void);
extern void   FlowMovie_RecSetYUVMergeRecInterval(UINT32 uiSec);
extern UINT32 FlowMovie_RecGetYUVMergeRecInterval(void);
extern void   FlowMovie_RecSetYUVMergeRecCounter(UINT32 uiCount);
extern UINT32 FlowMovie_RecGetYUVMergeRecCounter(void);
extern void   FlowMovie_RecYUVMergeCounterInc(void);
//#NT#2013/10/29#Isiah Chang -end
extern void   FlowMovie_FileDBCreate(void);
extern void   FlowMovie_FileDBCreate_Fast(void);
extern BOOL   FlowMovie_CheckReOpenItem(void);

//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
#define TIMELAPSE_FROM_HWRT         0
#define TIMELAPSE_FROM_UI           1
#define TIMELAPSE_FROM_TIMER        2
#define TIMELAPSE_FROM_PWRKEY       3

#define TL_FLOW_MOVIE               0x00
#define TL_FLOW_ORG                 0x10
#define TL_FLOW_LPR                 0x20

#define TL_STATE_DONTCARE           0x00
#define TL_STATE_STOP               0x01
#define TL_STATE_RECORD             0x02
#define TL_STATE_BUSY               0x03

#define TL_FLOW_MASK                0xf0
#define TL_STATE_MASK               0x0f

#define TL_HWRT_BOOT_NORMAL         0
#define TL_HWRT_BOOT_ALARM          1
#define TL_HWRT_WORKING             2
#define TL_HWRT_TIMEUP              3

#define TL_BUFFER_SIZE              ALIGN_CEIL_32(0x200000)

#define TL_LPR_TIME_MIN_PERIOD      MOVIE_TIMELAPSEREC_30SEC

extern void MovieTLLPR_SetMem(UINT32 uiAddr, UINT32 uiSize);
extern UINT32 MovieTLLPR_Process(INT32 CmdFrom);
extern UINT32 MovieTLLPR_CheckHWRTStatus(void);
extern UINT32 MovieTLLPR_GetStatus(void);
#endif
//#NT#2016/03/07#KCHong -end

#endif //_UIVIEWMOVIE_H_
