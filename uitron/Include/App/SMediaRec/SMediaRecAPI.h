/**
    Header file of SMedia Recorder Task

    Exported header file of media recorder task.

    @file       SMediaRecAPI.h
    @ingroup    mIAPPSMEDIAREC
    @note       add gSMediaRecPwrOffPT

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _SMEDIARECAPI_H
#define _SMEDIARECAPI_H

#include "MediaWriteLib.h"
#include "AVFile_MakerMov.h"
#include "AVFile_MakerAvi.h"
#include "MediaReadLib.h"
#include "AVFile_ParserMov.h"
#include "AVFile_ParserAvi.h"
#include "VideoEncode.h"
#include "movieinterface_def.h"

/**
    @addtogroup mIAPPSMEDIAREC
*/
//@{

/**
    Stop type for SMediaRec_Stop()
*/
typedef enum {
	SMEDIAREC_NO_WAIT  = 0x0,  ///< stop recording and no wait
	SMEDIAREC_WAIT_END = 0x1,   ///< stop recording and wait this action finished
	ENUM_DUMMY4WORD(SMEDIAREC_STOP_TYPE)
} SMEDIAREC_STOP_TYPE;

//#NT#2010/05/17#Meg Lin -begin
//#NT#add restriction type

/**
    check type for SMediaRec_SetWriteCard()
*/
typedef enum {
	SMEDIAREC_WRITE_PATH1  = 0x0,  ///< write path 1 to card
	SMEDIAREC_WRITE_PATH2  = 0x1,  ///< write path 2 to card
	ENUM_DUMMY4WORD(SMEDIAREC_WRITECARD_TYPE)
} SMEDIAREC_WRITECARD_TYPE;


typedef struct {
	UINT32 bsAddr;
	UINT32 bsSize;
	UINT32 iskey;    //is key-frame or not
} SMEDIAREC_BS_FROMSENSOR;


//#NT#2012/08/13#Hideo Lin -begin
//#NT#Integrate media recorder test mode
typedef struct {
	UINT32 inBStrAddr;          			///< input bitstream addr
	UINT32 inBStrSize;          			///< input bitstream size
} MP_VDOENC_BSTR_ENTRY;

typedef struct {
	UINT32 inBStrDescAddr;  				///< input description addr
	UINT32 inBStrDescSize;  				///< input description size
} MP_VDOENC_BSTR_DESC;

typedef struct {
	UINT32                    totalBStrVFN;                   		///< [in] total input video bitstream frame numbers
	MP_VDOENC_BSTR_DESC       bstrDesc;                       		///< [in] description data
	MP_VDOENC_BSTR_ENTRY      bstrVF[MP_VDOENC_BSTR_VF_NUM_MAX]; 	///< [in] bitstream frames
} MP_VDOENC_BSTR_INFO;

typedef struct {
	UINT32                  uiTotalNum;                         	///< [in] total YUV pattern number
	MP_VDOENC_YUV_SRC    	YUVSrc[MP_VDOENC_FIXED_YUV_NUM_MAX];   	///< [in] YUV source info
} MP_VDOENC_YUV_INFO;

typedef struct {
	BOOL    bEnable;                        ///< Enable test mode
	UINT32  uiVidSrcType;                   ///< recording video source type, MEDIAREC_SRC_NORMAL, MEDIAREC_SRC_FIXED_YUV, MEDIAREC_SRC_BS_ONLY
	UINT32  uiAudSrcType;                   ///< recording audio source type, MEDIAREC_SRC_NORMAL, MEDIAREC_SRC_FIXED_AUDIO
	MP_VDOENC_BSTR_INFO   BStrInfo;   ///< input video bitstream test pattern info
	MP_VDOENC_YUV_INFO    YUVInfo;    ///< fixed YUV test pattern information
} MEDIAREC_TESTMODE;
//#NT#2012/08/13#Hideo Lin -end


/**
    @name SMedia record event ID

    Callback ID of Smedia recorder.
*/
//@{
#define     SMEDIAREC_EVENT_RESULT_NORMAL        MOVREC_EVENT_RESULT_NORMAL ///< Finish normally
#define     SMEDIAREC_EVENT_RESULT_HW_ERR        MOVREC_EVENT_RESULT_HW_ERR ///< Hardware error
#define     SMEDIAREC_EVENT_RESULT_FULL          MOVREC_EVENT_RESULT_FULL   ///< Card full
#define     SMEDIAREC_EVENT_RESULT_SLOW          MOVREC_EVENT_RESULT_SLOW   ///< Write card too slowly
#define     SMEDIAREC_EVENT_RESULT_NOTREADY      MOVREC_EVENT_RESULT_NOTREADY///< Not ready
#define     SMEDIAREC_EVENT_THRESHOLD            MOVREC_EVENT_THRESHOLD     ///< Threshold meets
#define     SMEDIAREC_EVENT_RESULT_OVERTIME      MOVREC_EVENT_RESULT_OVERTIME///< Overtime
#define     SMEDIAREC_EVENT_RESULT_WRITE_ERR     MOVREC_EVENT_RESULT_WRITE_ERR///< Filesystem write fails
#define     SMEDIAREC_EVENT_STOPPED              MOVREC_EVENT_STOPPED       ///< Callback when stops totally
#define     SMEDIAREC_EVENT_FILECOMPLETE         MOVREC_EVENT_FILECOMPLETE  ///< Write file completely
#define     SMEDIAREC_EVENT_RESULT_CUT_FAIL      MOVREC_EVENT_RESULT_CUT_FAIL///< Cut, overlap fail (write slowly)
#define     SMEDIAREC_EVENT_CUSTOM_UDATA         MOVREC_EVENT_CUSTOM_UDATA  ///< Set customized user data
#define     SMEDIAREC_EVENT_ENCONE_OK            MOVREC_EVENT_ENCONE_OK     ///< enc one frame

#define     SMEDIAREC_EVENT_H264BSCB_P0          MOVREC_EVENT_VIDEO_BS_CB   ///< path1 video enc ok
#define     SMEDIAREC_EVENT_H264BSCB_P1          MOVREC_EVENT_H264BSCB_P1   ///< path2 video enc ok
#define     SMEDIAREC_EVENT_RESULT_FLASHSTOP     MOVREC_EVENT_RESULT_FLASHSTOP///< 30 sec after flash stop
#define     SMEDIAREC_EVENT_AUDBSCB              MOVREC_EVENT_AUDBSCB       ///< audio enc ok
#define     SMEDIAREC_EVENT_LOOPREC_FULL         MOVREC_EVENT_LOOPREC_FULL  ///< no space for seamless recording
#define     SMEDIAREC_EVENT_START                MOVREC_EVENT_START         ///< start recording
#define     SMEDIAREC_EVENT_TIMELAPSE_ONESHOT    MOVREC_EVENT_TIMELAPSE_ONESHOT///< timelapse oneshot
#define     SMEDIAREC_EVENT_SLOWSTART            MOVREC_EVENT_SLOWSTART     ///< slow stop begins
#define     SMEDIAREC_EVENT_ADDONEVIDBS          MOVREC_EVENT_ADDONEVIDBS   ///< add one video BS
#define     SMEDIAREC_EVENT_RESULT_PARAMERR      MOVREC_EVENT_RESULT_PARAMERR ///< paramters error
#define     SMEDIAREC_EVENT_RESULT_YUVQ_FULL     MOVREC_EVENT_RESULT_YUVQ_FULL ///< yuv queue full
#define     SMEDIAREC_EVENT_GETNAME_PATH0        MOVREC_EVENT_GETNAME_PATH0 ///< add one video BS
#define     SMEDIAREC_EVENT_GETNAME_EMRPATH      MOVREC_EVENT_GETNAME_EMRPATH///< add one video BS
#define     SMEDIAREC_EVENT_EMR_FILECOMPLETE     MOVREC_EVENT_EMR_FILECOMPLETE///< enc one frame
#define     SMEDIAREC_EVENT_VIDINFO_READY        MOVREC_EVENT_VIDEO_INFO_READY///< sps ready for h264, param=pathid

//@}

//#NT#2012/09/12#Meg Lin -begin
/**
    @name SMedia ImmediatelyProc callback ID

    Callback ID of Smedia ImmediatelyProc.
*/
//@{
#define     SMEDIAREC_IMMPROC_CUTFILE_END        MOVREC_IMMPROC_CUTFILE_END ///< end of one cut file, no parameter
#define     SMEDIAREC_IMMPROC_DELETE_FILE        MOVREC_IMMPROC_DELETE_FILE ///< end of deleting file, p1: directory ID, p2: file ID
#define     SMEDIAREC_IMMPROC_ENC_SETTING        MOVREC_IMMPROC_ENC_SETTING ///< special encoding parameters setting, such as H.264 3DNR
//@}
//#NT#2012/09/12#Meg Lin -end

/**
    @name SMedia record parameters

    Media recording changeable parameters.
*/
//@{
#define SMEDIAREC_RECPARAM_WIDTH             MOVREC_RECPARAM_WIDTH   ///< width, p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_HEIGHT            MOVREC_RECPARAM_HEIGHT  ///< height, p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_FRAMERATE         MOVREC_RECPARAM_FRAMERATE///< frameRate, p1 valid
#define SMEDIAREC_RECPARAM_QUALITY           MOVREC_RECPARAM_QUALITY ///< quality, p1 valid
#define SMEDIAREC_RECPARAM_TARGETRATE        MOVREC_RECPARAM_TARGETRATE ///< target rate (bytes per second), p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_GOLFSHOT_ON       MOVREC_RECPARAM_GOLFSHOT_ON///< change recFormat to golfshot, p1 0 or 1
#define SMEDIAREC_RECPARAM_CUTSEC            MOVREC_RECPARAM_CUTSEC  ///< seamless cut sec
#define SMEDIAREC_RECPARAM_CUTSIZE           MOVREC_RECPARAM_CUTSIZE ///< seamless cut size
#define SMEDIAREC_RECPARAM_ENDTYPE           MOVREC_RECPARAM_ENDTYPE ///< end type
#define SMEDIAREC_RECPARAM_FLASH_ON          MOVREC_RECPARAM_FLASH_ON///< flash recording on/off, p1 0 or 1
#define SMEDIAREC_RECPARAM_PWROFFPT          MOVREC_RECPARAM_PWROFFPT///< enable/disable poweroff_protection, default: on
#define SMEDIAREC_RECPARAM_GPSON             MOVREC_RECPARAM_GPSON   ///< enable/disable gps info on, default: off
#define SMEDIAREC_RECPARAM_TIMELAPSE         MOVREC_RECPARAM_TIMELAPSE///< set time lapse recording mode (ON/OFF)
#define SMEDIAREC_RECPARAM_TIMELAPSE_TIME    MOVREC_RECPARAM_TIMELAPSE_TIME///< set time lapse recording time interval (in unit of ms, should be larger than 1/30 sec)
#define SMEDIAREC_RECPARAM_THUMB_DATA        MOVREC_RECPARAM_THUMB_DATA///< movie thumbnail settings, p1:on/off, p2:width, p3:height
#define SMEDIAREC_RECPARAM_AUD_SAMPLERATE    MOVREC_RECPARAM_AUD_SAMPLERATE///< audio sample rate, AUDIO_SR_8000 or others
#define SMEDIAREC_RECPARAM_AUD_CHS           MOVREC_RECPARAM_AUD_CHS ///< audio channels, 1 or 2
#define SMEDIAREC_RECPARAM_AUD_SRC           MOVREC_RECPARAM_AUD_SRC ///< audio source, MEDIAREC_AUDTYPE_LEFT or others
#define SMEDIAREC_RECPARAM_AUD_CODEC         MOVREC_RECPARAM_AUD_CODEC///< audio codec type, MEDIAREC_ENC_PCM or others
#define SMEDIAREC_RECPARAM_RECFORMAT         MOVREC_RECPARAM_RECFORMAT///< recording format, MEDIAREC_AUD_VID_BOTH or others
#define SMEDIAREC_RECPARAM_EN_CUSTOMUDATA    MOVREC_RECPARAM_EN_CUSTOMUDATA///< enable custom user data or not
#define SMEDIAREC_RECPARAM_VIDEOCODEC        MOVREC_RECPARAM_VIDEOCODEC///< video codec type, MEDIAREC_ENC_H264 or others
#define SMEDIAREC_RECPARAM_H264GOPTYPE       MOVREC_RECPARAM_H264GOPTYPE///< h264 GOP type, MEDIAREC_GOP_IPONLY or others
#define SMEDIAREC_RECPARAM_H2643DNRLEVEL     MOVREC_RECPARAM_H2643DNRLEVEL///< h264 3DNR level
#define SMEDIAREC_RECPARAM_RAWENC_Q          MOVREC_RECPARAM_RAWENC_Q///< Q value for raw-encoding
#define SMEDIAREC_RECPARAM_EN_AUDFILTER      MOVREC_RECPARAM_EN_AUDFILTER///< enable audio filter or not
#define SMEDIAREC_RECPARAM_DROP_FRAME_NUM    MOVREC_RECPARAM_DROP_FRAME_NUM///< drop frame number as start recording (for AV sync)
#define SMEDIAREC_RECPARAM_H264QPLEVEL       MOVREC_RECPARAM_H264QPLEVEL///< h264 QP initial level
//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
#define SMEDIAREC_RECPARAM_MOV_ROTATE        MOVREC_RECPARAM_MOV_ROTATE///< MOV/MP4 file rotation, MEDIAREC_MOV_ROTATE_0
//#NT#2013/04/17#Calvin Chang -end
#define SMEDIAREC_RECPARAM_FILETYPE_2        MOVREC_RECPARAM_FILETYPE_2///< file1 type, MEDIAREC_AVI as default
#define SMEDIAREC_RECPARAM_MERGEYUV          MOVREC_RECPARAM_MERGEYUV
#define SMEDIAREC_RECPARAM_FILETYPE          MOVREC_RECPARAM_FILETYPE ///< file type, MEDIAREC_MOV, MEDIAREC_AVI, MEDIAREC_MP4
#define SMEDIAREC_RECPARAM_JPG_YUVFORMAT     MOVREC_RECPARAM_JPG_YUVFORMAT///< set jpeg yuv format
#define SMEDIAREC_RECPARAM_MULTIREC_ON       MOVREC_RECPARAM_MULTIREC_ON
#define SMEDIAREC_RECPARAM_LIVEVIEW          MOVREC_RECPARAM_LIVEVIEW ///< set MEDIAREC_LIVEVIEW recformat 2013/10/02 Meg
#define SMEDIAREC_RECPARAM_DAR               MOVREC_RECPARAM_DAR      ///< video display aspect ratio
#define SMEDIAREC_RECPARAM_PRE_ENCODE        MOVREC_RECPARAM_PRE_ENCODE///< video pre-encode (for applying 3DNR to 1st frame)
#define SMEDIAREC_RECPARAM_SENSOR_FPS        MOVREC_RECPARAM_SENSOR_FPS///< video sensor fps
#define SMEDIAREC_RECPARAM_SENSOR_NUM        MOVREC_RECPARAM_SENSOR_NUM///< input sensor number (for encoding path judgement)
#define SMEDIAREC_RECPARAM_SVC               MOVREC_RECPARAM_SVC      ///< set SVC disable/enable and GOP number
#define SMEDIAREC_RECPARAM_TINYMODE_ON       MOVREC_RECPARAM_TINYMODE_ON///< tiny mode on/off, p1 0 or 1
#define SMEDIAREC_RECPARAM_RAWENC_ON         MOVREC_RECPARAM_RAWENC_ON///< tiny mode on/off, p1 0 or 1
#define SMEDIAREC_RECPARAM_Y_LINEOFT         MOVREC_RECPARAM_Y_LINEOFT///< y lineoffset
#define SMEDIAREC_RECPARAM_UV_LINEOFT        MOVREC_RECPARAM_UV_LINEOFT///< UV lineoffset
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#define SMEDIAREC_RECPARAM_H264_3DNROUT      MOVREC_RECPARAM_H264_3DNROUT///< enable/disable h264 3DNR Out
//#NT#2016/09/20#Bob Huang -end
//#NT#2016/10/05#Bob Huang -begin
//#NT#Support Multi 2P Enc
#define SMEDIAREC_RECPARAM_MULTI_2PENC_ON    MOVREC_RECPARAM_MULTI_2PENC_ON///< enable/disable multi 2p enc
//#NT#2016/10/05#Bob Huang -end
#define SMEDIAREC_RECPARAM_H264ROTATE        MOVREC_RECPARAM_H264ROTATE///< h264 rotate

//@}

/**
    @name Valid video frame rate

    Valid video frame rate.
*/
//@{
#define SMEDIAREC_VRATE_2            2  ///< 2 fps, only for MEDIAREC_FAST_FWD
#define SMEDIAREC_VRATE_10           10 ///< 10 fps
#define SMEDIAREC_VRATE_15           15 ///< 15 fps
#define SMEDIAREC_VRATE_20           20 ///< 20 fps
#define SMEDIAREC_VRATE_24           24 ///< 24 fps
#define SMEDIAREC_VRATE_25           25 ///< 25 fps
#define SMEDIAREC_VRATE_28           28 ///< 28 fps
#define SMEDIAREC_VRATE_30           30 ///< 30 fps
#define SMEDIAREC_VRATE_48           48 ///< 48 fps, only for 960p/720p/VGA/QVGA
#define SMEDIAREC_VRATE_50           50 ///< 50 fps, only for 960p/720p/VGA/QVGA
#define SMEDIAREC_VRATE_55           55 ///< 55 fps
#define SMEDIAREC_VRATE_60           60 ///< 60 fps, only for 720p/VGA/QVGA
#define SMEDIAREC_VRATE_120          120 ///< 120 fps, only for VGA/QVGA
#define SMEDIAREC_VRATE_240          240 ///< 240 fps, only for QVGA
//@}

/**
    @name Valid file type

    Valid file type.
*/
//@{
#define SMEDIAREC_AVI    MEDIA_FILEFORMAT_AVI    ///< .avi file
#define SMEDIAREC_MOV    MEDIA_FILEFORMAT_MOV    ///< .mov file
#define SMEDIAREC_MP4    MEDIA_FILEFORMAT_MP4    ///< .mp4 file
#define SMEDIAREC_TS     MEDIA_FILEFORMAT_TS     ///< .ts file
//@}

/**
    @name Valid video encode codec

    Valid video encode codec.
*/
//@{
#define SMEDIAREC_ENC_JPG               MEDIAVIDENC_MJPG            ///< motion JPG
#define SMEDIAREC_ENC_H264              MEDIAVIDENC_H264            ///< H.264
#define SMEDIAREC_ENC_H265              MEDIAVIDENC_H265            ///< H.265
//@}

/**
    @name Valid audio encode codec

    Valid audio encode codec.
*/
//@{
#define SMEDIAREC_ENC_PCM    MOVAUDENC_PCM   ///< PCM
#define SMEDIAREC_ENC_AAC    MOVAUDENC_AAC   ///< AAC
#define SMEDIAREC_ENC_PPCM   MOVAUDENC_PPCM  ///< PPCM
#define SMEDIAREC_ENC_ULAW   MOVAUDENC_ULAW  ///< PCM uLaw
#define SMEDIAREC_ENC_ALAW   MOVAUDENC_ALAW  ///< PCM aLaw
//@}

/**
    @name Valid video input YCbCr format

    Valid video input YCbCr format.
*/
//@{
#define SMEDIAREC_JPEG_FORMAT_420    MOVREC_JPEG_FORMAT_420  ///< Y:Cb:Cr = 4:1:1
#define SMEDIAREC_JPEG_FORMAT_422    MOVREC_JPEG_FORMAT_422  ///< Y:Cb:Cr = 2:1:1
#define SMEDIAREC_JPEG_FORMAT_444    MOVREC_JPEG_FORMAT_444  ///< Y:Cb:Cr = 1:1:1
#define SMEDIAREC_JPEG_FORMAT_100    MOVREC_JPEG_FORMAT_100  ///< Y:Cb:Cr = 4:1:1
//@}

/**
    @name Valid counting type

    Valid counting type for MediaRec_Disk2SecondWithType.
*/
//@{
#define SMEDIAREC_COUNTTYPE_FS    MOVREC_COUNTTYPE_FS   ///< get freespace in storage
#define SMEDIAREC_COUNTTYPE_4G    MOVREC_COUNTTYPE_4G   ///< using 4G to calculate
#define SMEDIAREC_COUNTTYPE_USER  MOVREC_COUNTTYPE_USER ///< using given size to calculate
//@}

/**
    @name Valid end type

    Valid recording end type. Normal end, cut overlap, or cut till card full.
*/
//@{
#define SMEDIAREC_ENDTYPE_NORMAL             MOVREC_ENDTYPE_NORMAL ///< normal end, such as press key to stop recording
#define SMEDIAREC_ENDTYPE_CUTOVERLAP         MOVREC_ENDTYPE_CUTOVERLAP///< cut file and continue recording automatically; overlap the old files (cyclic recording)
#define SMEDIAREC_ENDTYPE_CUT_TILLCARDFULL   MOVREC_ENDTYPE_CUT_TILLCARDFULL///< cut file and continue recording automatically till card full
#define SMEDIAREC_ENDTYPE_MAX                MOVREC_ENDTYPE_MAX    ///< maximum number of recording end type
//@}

//#NT#2012/01/09#Hideo Lin -begin
//#NT#Add media recording source type (for test mode)
/**
    @name Media recording source type

    Media recording video source type: normal, fixed YUV pattern, bitstream;
    Media recording audio source type: normal, fixed PCM pattern.
*/
//@{
#define SMEDIAREC_SRC_NORMAL         MOVREC_SRC_NORMAL     ///< normal source (video: sensor, audio: mic-in)
#define SMEDIAREC_SRC_FIXED_YUV      MOVREC_SRC_FIXED_YUV  ///< fixed YUV pattern (for video)
#define SMEDIAREC_SRC_BS_ONLY        MOVREC_SRC_BS_ONLY    ///< bitstream only (for video)
#define SMEDIAREC_SRC_FIXED_PCM      MOVREC_SRC_FIXED_PCM  ///< fixed PCM pattern (for audio)
//@}
//#NT#2012/01/09#Hideo Lin -end

/**
    @name IPL mode change callback event

    Event ID for changing IPL mode callback function.
*/
//@{
#define SMEDIAREC_IPLCHG_PREVIEW     MOVREC_IPLCHG_PREVIEW    ///< Set IPL to preview mode
#define SMEDIAREC_IPLCHG_VIDEO       MOVREC_IPLCHG_VIDEO      ///< Set IPL to video mode
#define SMEDIAREC_IPLCHG_INITBS      MOVREC_IPLCHG_INITBS     ///< initbs
//@}

/**
    @name Get IPL ready buffer callback event

    Event ID for getting IPL ready buffer callback function.
*/
//@{
#define SMEDIAREC_IPLGET_READYBUF    MOVREC_IPLGET_READYBUF   ///< Get IPL ready buffer
//@}

/**
    @name Smedia record information

    SMedia recording getting information.
*/
//@{
#define     SMEDIAREC_GETINFO_WIDTH_HEIGHT   MOVREC_GETINFO_WIDTH_HEIGHT    ///< p1: width (out), p2: height (out)
#define     SMEDIAREC_GETINFO_VIDEO_CODEC    MOVREC_GETINFO_VIDEO_CODEC     ///< p1: videocodec type(out), MEDIAREC_ENC_JPG or others//2012/10/15 Meg
#define     SMEDIAREC_GETINFO_AUDIO_SR       MOVREC_GETINFO_AUDIO_SR        ///< p1: audio samplerate(out), AUDIO_SR_32000 or others//2013/01/11 Meg
#define     SMEDIAREC_GETINFO_AUDIO_CHS      MOVREC_GETINFO_AUDIO_CHS       ///< p1: audio channels(out), 1 or 2//2013/01/11 Meg
#define     SMEDIAREC_GETINFO_AUDIO_BITS     MOVREC_GETINFO_AUDIO_BITS      ///< p1: audio bits per sample, 16
#define     SMEDIAREC_GETINFO_H264_SPS       MOVREC_GETINFO_H264_SPS        ///< p1: addr (out), p2: size (out), p3: num(0 or 1)
#define     SMEDIAREC_GETINFO_VIDEO_FPS      MOVREC_GETINFO_VIDEO_FPS       ///< p1: v fps (out)
#define     SMEDIAREC_GETINFO_WIDTH_HEIGHT_P2 MOVREC_GETINFO_WIDTH_HEIGHT_P2  ///< p1: width of Path2 (out), p2: height of Path2(out)
#define     SMEDIAREC_GETINFO_AUDIO_CODEC    MOVREC_GETINFO_AUDIO_CODEC     ///< p1: audio codec type of 1stpath(out), MEDIAREC_ENC_PCM or others
//@}

/**
    @name Smedia audio type information

    SMedia recording audio type information.
*/
//@{
#define     SMEDIAREC_AUDTYPE_LEFT   MOVREC_AUDTYPE_LEFT   ///< left
#define     SMEDIAREC_AUDTYPE_RIGHT  MOVREC_AUDTYPE_RIGHT  ///< right
#define     SMEDIAREC_AUDTYPE_STEREO MOVREC_AUDTYPE_STEREO ///< stereo
#define     SMEDIAREC_AUDTYPE_MONO   MOVREC_AUDTYPE_MONO   ///< mono
//@}

//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
/**
    @name Smedia rotation angle information

    SMedia recording roation angle information.
*/
//@{
#define     SMEDIAREC_MOV_ROTATE_0       MOVREC_MOV_ROTATE_0   ///< angle 0
#define     SMEDIAREC_MOV_ROTATE_90      MOVREC_MOV_ROTATE_90   ///< angle 90
#define     SMEDIAREC_MOV_ROTATE_180     MOVREC_MOV_ROTATE_180   ///< angle 180
#define     SMEDIAREC_MOV_ROTATE_270     MOVREC_MOV_ROTATE_270   ///< angle 270
//@}
//#NT#2013/04/17#Calvin Chang -end
#define     SMEDIAREC_STRGID_1ST      0   ///< 1st storage
#define     SMEDIAREC_STRGID_2ND      1   ///< 2nd storage


typedef enum {
	MPFSCTRL_GETINFO_CLUSTERSIZE = 0x01,    ///< get cluster size (p1)
	ENUM_DUMMY4WORD(MP_FSCONTROLLER_GETINFO_TYPE)
} MP_FSCONTROLLER_GETINFO_TYPE;

typedef enum {
	MPFSCTRL_GETINFO64_FREESPACE    = 0x01,    ///< get freespace
	MPFSCTRL_GETINFO64_NOWLASTING   = 0x02,    ///< get nowlasting
	ENUM_DUMMY4WORD(MP_FSCTRL_GETINFO_TYPE64)
} MP_FSCTRL_GETINFO_TYPE64;

typedef enum {
	MPFSCTRL_SETINFO_FILEHDL      = 0x01, ///< set fileHDL (p1: hdl, p2: fstskid)
	MPFSCTRL_SETINFO_MINUSSIZE    = 0x02, ///< p1: fstskid, p2:minusSize
	ENUM_DUMMY4WORD(MP_FSCONTROLLER_SETINFO_TYPE)
} MP_FSCONTROLLER_SETINFO_TYPE;

typedef enum {
	MPFSCTRL_SETINFO64_INITSIZE     = 0x01, ///< p1:size
	ENUM_DUMMY4WORD(MP_FSCONTROLLER_SETINFO_TYPE64)
} MP_FSCONTROLLER_SETINFO_TYPE64;

typedef enum {
	MPFSCTRL_OLDFSFX_ALLOCFILE   = 0x01, ///< alloc fileHDL (p1: fstskid, p2: pos_L, p3: pos_h)
	MPFSCTRL_OLDFSFX_DELETE      = 0x02, ///< delete (p1: pPath)
	MPFSCTRL_OLDFSFX_FLUSH       = 0x03, ///< flush fileHDL (p1: fstskid)
	MPFSCTRL_OLDFSFX_SETATTR     = 0x04, ///< FileSys_SetAttrib (p1: pPath, p2: attr, p3:false)
	MPFSCTRL_OLDFSFX_MOVE        = 0x05, ///< MoveFile (p1: pOldPath, p2: pNewPath)
	MPFSCTRL_OLDFSFX_SEEK        = 0x06, ///< SeekFile (p1: fstskid, p2: pos, p3:pos_H)
	MPFSCTRL_OLDFSFX_WRITE       = 0x07, ///< Write (p1: fstskid, p2: buf, p3:size)
	ENUM_DUMMY4WORD(MP_FSCONTROLLER_OLDFSFX_TYPE)
} MP_FSCONTROLLER_OLDFSFX_TYPE;

/**
    Status type for SMediaRec_GetStatus()
*/
typedef enum {
	SMEDIAREC_STATUS_NOT_OPENED        = 0x1,  ///< media recorder task is not open
	SMEDIAREC_STATUS_OPENED_NOT_RECORD = 0x2,  ///< media recorder task is open, not recording
	SMEDIAREC_STATUS_RECORDING         = 0x3,  ///< media recorder task is recording
	ENUM_DUMMY4WORD(SMEDIAREC_STATUS_TYPE)
} SMEDIAREC_STATUS_TYPE;

/**
    Video ready buffer information for movie recording.

*/
typedef struct {
	UINT32      y;              ///<[in]: y address
	UINT32      cb;             ///<[in]: cb address
	UINT32      cr;             ///<[in]: cr address
	UINT32      y_lot;          ///<[in]: y line offset
	UINT32      uv_lot;         ///<[in]: uv line offset
	UINT32      uiBufID;        ///<[in]: IME ready buffer ID
} SMEDIAREC_READYBUF_INFO;

/**
     Install Task, Sem, Flag ID for SMediaRec.

*/
extern void SMediaRec_InstallID(void) _SECTION(".kercfg_text");

/*
    Public Smedia recording functions
*/
//MediaRecTsk.c
/**
    Open Smedia recorder task.

    Open Smedia recorder task to prepare for recording.

    @param[in] param    the media recorder object

    @return
        - @b E_OK:  open Smedia recorder task successfully
        - @b E_SYS: Smedia recorder task is already opened
*/
extern ER       SMediaRec_Open(MEDIAREC_OBJ *param);

/**
    Close Smedia recorder task.

    Close Smedia recorder related tasks and stop recording if necessary.

    @return
        - @b E_OK:  terminate media recorder task successfully
        - @b E_SYS: Smedia recorder task is not opened
*/
extern ER       SMediaRec_Close(void);

/**
    Start Smedia recording.

    @return
        - @b E_OK:  start recording successfully
        - @b E_SYS: Smedia recorder is recording or not ready
        - @b E_PAR: recorder setting error (such as recording format, video codec type, ...)
*/
extern ER       SMediaRec_Record(void);

/**
    Stop Smedia recording.

    @param[in] waitType waiting type as media recording stop, MEDIAREC_NO_WAIT or MEDIAREC_WAIT_END

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       SMediaRec_Stop(SMEDIAREC_STOP_TYPE waitType);

/**
    Change Smedia recorder parameters.

    Change Smedia recorder parameters, such as width, height, frame rate, target bit rate, ..., and so on.

    @note

    @param[in] type parameter type
    @param[in] p1   parameter1
    @param[in] p2   parameter2
    @param[in] p3   parameter3

    @return
        - @b E_OK:  change successfully
        - @b E_PAR: parameter value error
*/
extern ER       SMediaRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Get Smedia recorder parameter settings.

    Get Smedia recorder parameter settings, only support getting width and height information currently.

    @note

    @param[in]  type    parameter type, only support SMEDIAREC_GETINFO_WIDTH_HEIGHT currently
    @param[out] p1      parameter1 value
    @param[out] p2      parameter2 value
    @param[out] p3      parameter3 value

    @return
        - @b E_OK:  get parameter successfully
        - @b E_SYS: parameter address error
        - @b E_PAR: parameter type error
*/
extern ER       SMediaRec_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);

/**
    Set user data.

    Set user data for media container.

    @note The user data is in 'JUNK' chunk for AVI file, and in 'udta' atom for MOV/MP4 file.

    @param[in] addr user data starting address
    @param[in] size user data size

    @return void
*/
extern void     SMediaRec_SetUserData(UINT32 addr, UINT32 size);
extern void     SMediaRec_SetCustomDataWithID(UINT32 id, UINT32 tag, UINT32 addr, UINT32 size);

/**
    Estimate remaining time for media recording.

    Estimate remaining time in seconds according to storage device space with different counting type.

    @note This function should be called before Smedia recording.

    @param[in] type counting type, SMEDIAREC_COUNTTYPE_FS (according to storage free space),
                    SMEDIAREC_COUNTTYPE_4G (FAT file size limitation), or MEDIAREC_COUNTTYPE_USER
                    (user defined space size, for special purpose)
    @param[in] size user defined space size, just for SMEDIAREC_COUNTTYPE_USER type

    @return Estimated remaining time in seconds.
*/
extern UINT32   SMediaRec_Disk2SecondWithType(UINT32 type, UINT32 size);//2010/01/25 Meg

/**
    Set restriction for recording time.

    Set restriction to calculate remaining time for Smedia recording.

    @note

    @param[in] type     restriction type, SMEDIAREC_RESTR_INDEXBUF or SMEDIAREC_RESTR_FSLIMIT
    @param[in] value    set FALSE to disable, and others to enable the related restriction

    @return void
*/
extern void     SMediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value);//2010/05/17 Meg Lin

/**
    Set free user data.

    Set free user data for special purpose.

    @note It's only valid for MOV/MP4 file. The data will be put in 'fre1' atom.

    @param[in] addr free data starting address
    @param[in] size free data size

    @return void
*/
extern void     SMediaRec_SetFre1Data(UINT32 addr, UINT32 size);

/**
    Re-allocate buffer to Smedia recorder.

    Re-allocate buffer for Smedia video and audio recording usage.

    @note The buffer should be re-allocated before media recording.

    @param[in] memStart buffer starting address
    @param[in] memSize  buffer size

    @return
        - @b E_OK:  re-allocate buffer successfully
        - @b E_SYS: re-allocate buffer failed (memory is not enough)
*/
extern ER       SMediaRec_ReAllocBuf2VA(UINT32 memStart, UINT32 memSize);

/**
    Stop Smedia recording and set read-only.

    Stop Smedia recording and set previous file, current file and next file to be read-only.

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       SMediaRec_StopAndReadOnly(void);//2012/02/17 Meg

/**
    To get Smedia recorder status.

    @return
        - @b SMEDIAREC_STATUS_NOT_OPENED:        Smedia recorder is not opened
        - @b SMEDIAREC_STATUS_OPENED_NOT_RECORD: Smedia recorder is opened but not recording
        - @b SMEDIAREC_STATUS_RECORDING:         Smedia recorder is recording
*/
extern SMEDIAREC_STATUS_TYPE   SMediaRec_GetStatus(void);

/**
    Set audio volume for SMedia Recorder.

    @param[in] percent  the percentage of audio gain level (0% - 100%)
    @return
        - @b E_OK:  set successfully
*/
extern ER SMediaRec_SetAudioVolume(UINT32 percent);


extern void     SMediaRec_Sleep(void);
extern void     SMediaRec_Wakeup(void);
extern void     SMediaRec_PauseDMA(void);
extern void     SMediaRec_ResumeDMA(void);
extern void     SMediaRec_ChangeDisplaySize(UINT32 uiDisp_w, UINT32 uiDisp_h);


//SMediaVideoRecTsk.c
/**
    Set ready buffer for Smedia recorder.

    Set ready buffer information, such as Y, Cb, Cr starting address and line offset for media recorder.

    @note This function is usually used in IPLGetReadyCB (get IPL ready buffer callback) of media recorder object.
          In the callback, get IPL ready buffer from IPL driver, then use this API to set to media recorder.

    @param[in] pInfo    ready buffer information for media recorder

    @return void
*/
extern void     SMediaRec_ResetReadyBuf(void);
extern void     SMediaRec_PutReadyBuf(MP_VDOENC_YUV_SRC *pInfo);
extern void     SMediaRec_GetReadyBuf(MP_VDOENC_YUV_SRC *pInfo);
extern void     SMediaRec_SetReadyBuf(MP_VDOENC_YUV_SRC *pInfo);//2012/08/30 Meg
extern ER       SMediaRec_PutReadyBufWithID(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);
extern void     SMediaRec_GetReadyBufWithID(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);
extern void     SMediaRec_SetReadyBufWithID(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);//2012/08/30 Meg
extern void     SMediaRec_SetReadyBuf4RawEnc(UINT32 fileid, MP_VDOENC_YUV_SRC *pInfo);


//SMediaAudioRecTsk.c
extern void     SMediaRec_Pause(void);
extern void     SMediaRec_Resume(void);

//SMediaRecFSTsk.c
/**
    Set GPS data.

    Set GPS data for Smedia container.

    @note It's only valid for AVI file currently. The GPS data is put in 'JUNK' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     SMediaRec_SetGPSData(UINT32 addr, UINT32 size);//2011/06/23 Meg

/**
    Set crash event.

    Set current recording file to be renamed and read-only.

    @note

    @return void
*/
extern void     SMediaRec_SetCrash(void);//2011/06/27 Meg

/**
    Cancel crash event.

    Cancel crash event.

    @note

    @return void
*/
extern void     SMediaRec_CancelCrash(void);//2013/11/18 Meg

/**
    Set event.

    Set current recording file to be renamed and read-only.

    @param[in] pathid 0 or 1
    @param[in] eventid MEDIAREC_CRASHTYPE_EV1 or others

    @return void
*/
extern void SMediaRec_SetEvent(UINT32 pathid, UINT32 eventid);

/**
    To use same crash DID or not.

    To use same or different DCF directory ID for crash event.

    @note Set TRUE to use same directory ID, FALSE to use different directory ID.

    @param[in] on   turn on/off same crash DID function

    @return void
*/
extern void     SMediaRec_SetTheSameCrashDid(UINT8 on);//2011/09/15 Meg

/**
    Set crash folder name.

    Set DCF folder name for crash event.

    @note It should follow the DCF naming rule, that is, 5 characters in 'A'~'Z', 'a'~'z', '0'~'9', and '_'.

    @param[in] pChar    pointer to the folder name

    @return void
*/
extern void     SMediaRec_SetCrashFolderName(char *pChar);//2011/09/15 Meg

/**
    Set naming rule for SMedia Recorder.

    @param[in] pHdl    pointer to the naming rule
    @return
        - @b E_OK:  set successfully
*/
extern ER SMediaRec_SetFSNamingHandler(MEDIANAMINGRULE *pHdl);


extern void SMediaRecCmdTsk(void);
extern void SMediaRecTsk(void);
extern void SMediaVideoRecTsk(void);
extern void SMRawEncTsk(void);
extern void SMediaAudioEncTsk(void);
extern void SMediaRecFSTsk(void);
extern void SMediaRecVideoStrmTsk(void);
extern void SMediaRecMdatTsk(void);
extern void SMediaRecVidTrigD2DTsk(void);
extern void SMediaRecVidTrigDirectTsk(void);
extern void SMediaRecTSTsk(void);
extern void SMediaRecFSStrmTsk(void);


extern void SMediaRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
extern void SMediaRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB);
extern void SMediaRec_VideoDoEnc(void);
extern void SMediaRec_GetEncInfo(UINT32 puiParam[7]);
extern void SMediaRec_SetFileHandle(FST_FILE fhdl, UINT32 fileid);
extern void SMediaRecFS_SetWorkingID(UINT32 id);
extern UINT32   SMediaRec_GetNowVideoFrames(void);

/**
    Open Smedia recording error msg.

    @param[in] on  1 to open error msg
    @return void
*/
extern void SMediaRec_OpenErrMsg(UINT8 on);

/**
    Reset Smedia recorder containerMaker.

    @return void
*/
extern void SMediaRec_ResetConMaker(void);

/**
    Open Smedia recording warning msg.

    @param[in] on  1 to open warning msg
    @return void
*/
extern void SMediaRec_OpenWrnMsg(UINT8 on);

/**
    Open media recording seamless recording msg.

    @param[in] on  1 to open seamless recording msg
    @return void
*/
extern void SMediaRec_OpenCutMsg(UINT8 on);

/**
    Open media recording must msg.

    @param[in] on  1 to open must msg
    @return void
*/
extern void SMediaRec_OpenMustMsg(UINT8 on);

/**
    Open media recording buf msg.

    @param[in] on  1 to open buf msg
    @return void
*/
extern void SMediaRec_OpenBufMsg(UINT8 on);

/**
    Check Smedia recorder if recording.

    @param[in] void
    @return
        - @b TRUE:  recording
        - @b FALSE: not recording
*/
extern BOOL SMediaRec_IsRecording(void);

/**
    Set memory addr and size for MergeYUV.

    @param[in] addr memory address
    @param[in] size memory size
    @return void
*/
extern void SMediaRec_SetGiveYUVBuf(UINT32 addr, UINT32 size);

/**
    Set readyYUV address for MergeYUV.

    @param[in] ptr    pointer to YUV info
    @return void
*/
extern void SMediaRec_GiveYUV(MP_VDOENC_YUV_SRC *ptr);

/**
    Set Extend crash.
    If seamless recording, this will set Crash after 30 sec.
    Max 3 times. If busy, return E_SYS.

    @param[in] void
    @return
        - @b E_OK: ok
        - @b E_SYS: busy
*/
extern ER SMediaRec_SetExtendCrash(void);//2013/09/04 Meg

/**
    Dump MediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32 SMediaRec_DumpDataStatus(UINT32 *pData);//2013/08/15

/**
    Get lastest video bitstream addr,size.

    @param[in] frameBuf    pointer to video frame
    @return 0
*/
extern UINT32 SMediaRec_GetLastV_BS(UINT32 pathID, MEDIAREC_MEM_RANGE *frameBuf);

/**
    Force SMediaRec to check FS_DiskInfo.

    @param[in] void
    @return void
*/
extern void SMediaRec_CheckFSDiskInfo(void);

/**
    Open Smedia recording FSObj msg.

    @param[in] on  1 to open FSObj msg
    @return void
*/
extern void SMediaRec_OpenFSOBJMsg(UINT8 on);

/**
    Open Smedia recording file msg.

    @param[in] on  1 to open file msg
    @return void
*/
extern void SMediaRec_OpenFileMsg(UINT8 on);

/**
    Set path2 write card or not.

    @param[in] on  1 to write path2 to card, if path2 enable
    @return void
*/
extern void SMediaRec_SetPath2WriteCard(UINT32 on);


/**
    Set path2 IPL get ready buffer callback.

    @param[in] IPLCb  path2 IPL get ready buffer callback
    @return void
*/
extern void SMediaRec_RegisterCB2(MediaRecIPLGetReadyCB IPLCb);

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @return void
*/
extern void SMediaRec_MinusFilesize(UINT32 size);

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @param[in] filesavetype , SMEDIAREC_FILESAVE_PART1 or others
    @return void
*/
extern void SMediaRec_MinusFilesizeWithStrg(UINT32 size, UINT32 filesavetype);

extern void SMediaRec_StopPreRecordStartWriting(void);

/**
    Get H264 sps/pps for each video path.

    @param[in] fileid video path, 0 or 1
    @param[in] pAddr address to put sps/pps
    @param[in] pSize size of sps/pps
    @return void
*/
extern void     SMediaRec_VidGetDesc(UINT32 fileid, UINT32 *pAddr, UINT32 *pSize);

/**
    Get minimum memory size for liveview mode.

    @param[in] void
    @return memory size
*/
extern UINT32 SMediaRec_GetLiveViewMinMemSize(void);

/**
    Update setting to calculate disk2second.

    @param[in] void
    @return void
*/
extern void SMediaRec_UpdateSetting2CaluSec(void);

/**
    Set Customized Data. (Put end of file with specific tag)

    @param[in] tag abcd=0x61626364
    @param[in] addr data address
    @param[in] size data size
    @return void
*/
extern void SMediaRec_SetCustomData(UINT32 tag, UINT32 addr, UINT32 size);

/**
    Set read-only percentage.

    @param[in] percent min=30, max=50
    @return void
*/
extern void SMediaRec_SetROPercent(UINT32 percent);

/**
    Enable end-char for single recording.

    @param[in] value on/off
    @return void
*/
extern void SMediaRec_EnableEndChar4SingleRec(UINT8 value);

/**
    Enable end-char for multiple recording.

    @param[in] value on/off
    @return void
*/
extern void SMediaRec_EnableEndChar4MultiRec(UINT8 value);

/**
    Get lastest audio bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void SMediaRec_GetLastA_BS(MEDIAREC_MEM_RANGE *frameBuf);

/**
    Get lastest movie filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void SMediaRec_GetLastFilePath(char *pPath);

/**
    Get lastest EMR filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void SMediaRec_GetLastEMRFilePath(char *pPath);
/**
    Enable Check and delete read-only file when seamless recoring.

    @param[in] value on/off, default: on
    @return void
*/
extern void SMediaRec_EnableCheckReadonly(UINT8 value);
extern void SMediaRec_VidReady2Get(UINT32 fileid);//2014/01/09

/**
    Get lastest delete filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void SMediaRec_GetLastDelFilePath(char *pPath);

/**
    Input settings to calculate recording seconds.

    @param[in] pSetting input recording setting
    @return recording seconds
*/
extern UINT32 SMediaRec_CalcSecBySetting(MEDIAREC_CALCSEC_SETTING *pSetting);

/**
    Set max customdata size. Card capacity will minus this size when start recording.

    @param[in] fileID
    @param[in] maxSize
    @return void
*/
extern void SMediaRec_SetCustomDataMaxSize(UINT32 fileid, UINT32 maxSize);

/**
    Set frameinfo size. Default: 4MB.

    @param[in] size
    @return void
*/
extern void SMediaRec_SetFrameInfoSize(UINT32 size);

/**
    Set this and last file as read-only.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            this and last both read-only
        - @b MOVREC_SETCRASH_NOLASTFILE:    this will be read-only (later), last not exist
        - @b MOVREC_SETCRASH_LASTFILERO:    this will be read-only (later), last has been RO already
*/
extern UINT32 SMediaRec_SetReadonly_ThisandLastfile(void);

/**
    Set end char for each path. 'A' and 'B' as default value.

    @param[in] endchar A~Z, a~z
    @param[in] fileid  0 as path1, 1 as path2...
    @return
        - @b E_OK:  OK
        - @b E_SYS: repeat as others
*/
extern ER SMediaRec_SetEndChar(char endchar, UINT32 fileid);

/**
    Delete files (including RO). It SHOULD NOT be executed when recording.

    @param[in] roDelete delete read-only files or not
    @param[in] roPercent  read-only max percent (30~50)
    @return void
*/
extern void SMediaRec_Clean4Recording(UINT32 roDelete, UINT32 roPercent);

/**
    Set deleting read-only files or not.

    @param[in] value 0: No delete RO files, 1: as default
    @return void
*/
extern void SMediaRec_SetDeleteReadonly(UINT32 value);

/**
    Get read-only files percent. (including freespace)

    @param[in] void
    @return RO percent
*/
extern UINT32 SMediaRec_GetROPercent(void);

/**
    Check and get H.264 encoding path after some settings (such as resolution, frame rate, ...).

    @param[in] void
    @return void
*/
extern void SMediaRec_CheckEncPath(void);

/**
    Enable/disable card writing.

    @param[in] pathid   0 or 1
    @param[in] on       0: not save, 1: as default
    @return void
*/
extern void SMediaRec_SetWriteCard(UINT32 pathid, UINT32 on);

/**
    Enable/disable EmergencyRecording for path0/1
    (If EMR is enable, it needs more memory!!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @param[in] on       0: disable 1: enable
    @return void
*/
extern void SMediaRec_SetEmergencyRecording(UINT32 pathid, UINT32 on);

/**
    Enable/disable EMR loop recording for path0/1
    (1st file rollback, but others just overlap 1 sec)
    (of course, EMR should be enable)

    @param[in] pathid   0 or 1(if path1 is enable)
    @param[in] on       0: disable 1: enable
    @return void
*/
extern void SMediaRec_SetEMR_loop(UINT32 pathid, UINT32 on);


/**
    Stop EmergencyRecording for path0/1
    (If EMR is disable, it doesn't work!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @return void
*/
extern ER SMediaRec_StopFlash(UINT32 pathid);

/**
    Stop TS EmergencyRecording for path0/1
    (If EMR is disable, it doesn't work!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @return void
*/
extern ER SMediaRec_StopFlashTS(UINT32 pathid);

/**
    Set H.264 Max Qp level.

    @param[in] level VIDENC_MAX_QP_LEVEL_0 as default
    @return void
*/
extern void SMediaRec_SetH264MaxQp(UINT32 pathid, UINT32 level);

/**
    Set H.264 GOP number.

    @param[in] gopnum 15 as default. Max: 120
    @return void
*/
extern void SMediaRec_SetH264GopNum(UINT32 gopnum);

/**
    Set H.264 GOP number by path.

    @param[in] gopnum 15 as default. Max: 120
    @param[in] pathID 0 as default. Max: 1
    @return void
*/
extern void SMediaRec_SetH264GopNumByPath(UINT32 gopnum, UINT32 pathID);

/**
    Get SMedia recording parameters.

    @param[in] type MOVREC_GETRECPARAM_TARGETBR or others

    @return ER
        - @b E_OK:  OK
        - @b E_SYS: params error
*/
extern ER SMediaRec_GetParam(UINT32 type, UINT32 *pParam1, UINT32 *pParam2, UINT32 *pParam3);

/**
    Set SMedia Force no write card

    @param[in] value 1:no write

    @return void
*/
extern void SMediaRec_ForceNoWriteCard(UINT32 value);

/**
    Set SMedia max stopflash rollback sec

    @param[in] value 5~30 is valid, default: 30

    @return void
*/
extern void SMediaRec_SetMaxStopFlashSec(UINT32 value);

/**
    Set SMedia max keep sec

    @param[in] value 5~30 is valid, default: 30

    @return void
*/
extern void SMediaRec_SetMaxKeepSec(UINT32 value);

/**
    Enable rollback or not

    @param[in] enable: 0 no overlap, 1 overlap 1sec

    @return void
*/
extern void SMediaRec_EnableOverlap(UINT32 enable);

/**
    Enable/disable ExtendCrash

    @param[in] enable: 0 no extend crash (1 as default)

    @return void
*/
extern void SMediaRec_Enable_ExtendCrash(UINT32 value);

/**
    Enable/disable Emergency recording with one shot.

    @param[in] enable: 0 no rawenc (0 as default)

    @return void
*/
extern void     SMediaRec_Enable_EMRwithRawEnc(UINT32 value);

/**
    Set NextSec to call StopFlash.

    @param[in] value 5 is default (smallest : 5)

    @return void
*/
extern void     SMediaRec_SetStopFlash_NextSec(UINT32 value);

/**
    Wait Raw-encode ok

    @param[in] pathid (0 as path1)

    @return void
*/
extern void SMRawEnc_WaitJpgEnc(UINT32 pathid);

/**
    Set RawEnc write JPG file or not.

    @param[in] value (default: 1)

    @return void
*/
extern void SMRawEnc_EnableWriteCard(UINT32 enable);

/**
    Force size of all files align to a special size.
    Only 16M/32M is valid. (0x1000000  0x2000000)

    @param[in] size (0x1000000 or 0x2000000)

    @return void
*/
extern void SMediaRec_Enable_FileSizeAlign(UINT32 size);

/**
    According to now setting, calculate suggested memory size.

    @param[in] void

    @return Size in bytes
*/
extern UINT32 SMediaRec_GetSuggestMem(void);

/**
    Set GPS block size. Max:100KB, default: 32KB

    @param[in] void

    @return Size in bytes
*/
extern void SMediaRec_SetGpsBlkSize(UINT32 value);

/**
    Get if vidPath is writing to card

    @param[in] vidPathID 0 as 1st path, 1 as 2nd path

    @return 0: not write
*/
extern UINT32 SMediaRec_IsWriting(UINT32 vidPathID);

/**
    Set lastfile as read-only and move to RO folder.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            this and last both read-only
        - @b MOVREC_SETCRASH_NOLASTFILE:    last not exist
        - @b MOVREC_SETCRASH_LASTFILERO:    last has been RO already
*/
extern UINT32 SMediaRec_SetCrash_Lastfile(void);

/**
    Set nextfile as read-only and move to RO folder.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            next file will be read-only
*/
extern UINT32 SMediaRec_SetCrash_Nextfile(void);

/**
    Cut two-path EMR recordings at the same time!
    @param[in] void
    @return
        - @b E_OK: both OK
        - @b E_SYS: other errors
*/
extern ER SMediaRec_MultiStopFlash(void);

/**
    Enable/disable overwrite YUV test. default off

    @param[in] on: 1 to open test

    @return void
*/
extern void SMediaRec_overwriteYUVtest(UINT32 on);

/**
    Cut now to end last file, start new file.

    @param[in] void

    @return ER E_OK or others
*/
extern ER SMediaRec_SetCutNOW(void);

extern void SMediaRec_SetExtendCrashUnit(MOVREC_EXTENDCRASH_UNIT value);

extern void SMediaRec_SetValidVidBS(SMEDIAREC_BS_FROMSENSOR *pBS);
extern UINT32 SMediaRecVidTrig_GetOutputAddr(void);
extern void SMediaRec_VtrigOK(void);
extern void SMediaRec_IPLOK_GoNext(void);
extern MP_VDOENC_PATH SMediaRec_GetEncPath(UINT32 uiVidEncId);
extern void SMediaRec_Vtrig_NoKickH264(UINT32 value);
extern void SMediaRec_Set3DNROutAddr(UINT32 pathID, UINT32 uiFrameId, UINT32 uiYAddr, UINT32 uiUVAddr);
extern void SMediaRec_Trigger3DNROut(UINT32 pathID);
extern UINT32 SMediaRec_GetFreeRecSec(void);
extern void SMediaRec_RegisterCB_Start(MediaRecCB_Start IPLCb);
extern void SMediaRec_RegisterCB_Get(MediaRecCB_Get IPLCb);
extern void SMediaRec_SetGolfshot_FrameRate(UINT32 value);
extern ER   SMRawEnc_IsReadyToCapture(void);
extern UINT32 SMediaRec_GetSuggestMem(void);
extern UINT32 SMediaRec_GetNowVidYuvAddr(UINT32 pathID);
extern void   SMediaRec_Register3DNRCb(UINT32 pathID, MEDIAREC_3DNRCB CB);
extern UINT32 SMediaRec_GetFreeRecSecByPath(UINT32 first, UINT32 second);
extern void   SMediaRec_GetLastA_PCM(MEDIAREC_MEM_RANGE *frameBuf);

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
/*
    Get Now Display Frame YUV Address

    @param  [in] pathid: Path ID
    @param  [in] py: Y plane address pointer
    @param  [in] pu: U plane address pointer
    @param  [in] pv: V plane address pointer

    @return void
*/
extern void SMediaRec_GetNowDisplayFrame(UINT32 pathid, UINT32 *py, UINT32 *pu, UINT32 *pv);
//#NT#2016/09/20#Bob Huang -end

extern void SMediaRec_SetMdatWriteBlock(UINT32 value);

//#NT#2016/10/05#Bob Huang -begin
//#NT#Support Multi 2P Enc
extern BOOL SMediaRecUti_GetMulti2PEncOn(void);
//#NT#2016/10/05#Bob Huang -end
void SMediaRecUti_DumpBufUsage(void);

extern void Cmd_sm_CreateB(UINT32 adr, UINT32 size);
extern void SMediaRec_EMRsaveType(UINT32 pathid, UINT32 filesavetype);
extern void SMediaRec_EnUnlimitedLowTBR(UINT32 en);
#endif//_SMEDIARECAPI_H

//@}
