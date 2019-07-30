#ifndef _NVTCODEC_H
#define _NVTCODEC_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtcodec.h

    @version

    @date
*/
#include <stdio.h>
#include <nvt_type.h>
#include <sys/time.h>
#include "nvtevent.h"
#include <nvtvideosprite_protected.h>
#ifdef __cplusplus
extern "C" {
#endif

#define VENC_META_SEI   0 // 1 SEI header in metadata, 0 SEI header in vstream
#define VIDEO_ENCODER_CHANNEL_MAX   16
#define VIDEO_OUTPUT_CHANNEL_MAX    1
#define AUDIO_INPUT_CHANNEL_MAX     16
#define VENC_STREAM_PACK_MAX        4

#define VPS_LENGTH_MAX 256
#define SPS_LENGTH_MAX 256
#define PPS_LENGTH_MAX 256
#define SEI_LENGTH_MAX 256
#define BC_MAX_OBJ_NUM 256
#define BC_MAX_BLK_NUM  32*32
#define MAX_PLANE_NUM 3
typedef enum {
	H264_NALU_TYPE_SLICE = 1,       ///< Coded slice (P-Frame)
	H264_NALU_TYPE_DPA,             ///< Coded data partition A
	H264_NALU_TYPE_DPB,             ///< Coded data partition B
	H264_NALU_TYPE_DPC,             ///< Coded data partition C
	H264_NALU_TYPE_IDR = 5,         ///< Instantaneous decoder refresh (I-Frame)
	H264_NALU_TYPE_SEI,             ///< Supplement Enhancement Information
	H264_NALU_TYPE_SPS = 7,         ///< Sequence parameter sets
	H264_NALU_TYPE_PPS = 8,         ///< Picture parameter sets
	H264_NALU_TYPE_AUD,             ///< AUD
	H264_NALU_TYPE_EOSEQ,           ///< End sequence
	H264_NALU_TYPE_EOSTREAM,        ///< End stream
	H264_NALU_TYPE_FILL,            ///< Filler data
	H264_NALU_TYPE_SPSEXT,          ///< SPS extension
	H264_NALU_TYPE_AUXILIARY,       ///< Auxiliary slice
	H264_NALU_TYPE_MAX,
	ENUM_DUMMY4WORD(H264_NALU_TYPE)
} H264_NALU_TYPE;

typedef enum {
	H265_NALU_TYPE_SLICE = 1,
	H265_NALU_TYPE_IDR = 19,
	H265_NALU_TYPE_VPS = 32,
	H265_NALU_TYPE_SPS = 33,
	H265_NALU_TYPE_PPS = 34,
	H265_NALU_TYPE_AUD = 35,
	H265_NALU_TYPE_EOS_NUT = 36,
	H265_NALU_TYPE_EOB_NUT = 37,
	H265_NALU_TYPE_SEI_PREFIX = 39,
	H265_NALU_TYPE_SEI_SUFFIX = 40,
	ENUM_DUMMY4WORD(H265_NALU_TYPE)
} H265_NALU_TYPE;

typedef enum {
	JPEG_PACK_TYPE_PIC,             ///< JPEG picture data
	JPEG_PACK_TYPE_MAX,
	ENUM_DUMMY4WORD(JPEG_PACK_TYPE)
} JPEG_PACK_TYPE;

typedef union {
	H264_NALU_TYPE eH264Type;           ///< H.264 pack type
	JPEG_PACK_TYPE eJPEGType;           ///< JPEG pack type
	H265_NALU_TYPE eH265Type;           ///< H.265 pack type
} CodecVencPackType;

typedef enum {
	CODEC_VENC_INPUT_SOURCE_SENSOR1,
	CODEC_VENC_INPUT_SOURCE_SENSOR2,
	CODEC_VENC_INPUT_SOURCE_SENSOR3,
	CODEC_VENC_INPUT_SOURCE_SENSOR4,
	CODEC_VENC_INPUT_SOURCE_UVC,
	CODEC_VENC_INPUT_SOURCE_MAX,
	ENUM_DUMMY4WORD(CodecVencInputSource)
} CodecVencInputSource;

typedef enum {
	CODEC_VENC_FORMAT_MJPEG,
	CODEC_VENC_FORMAT_H264,
	CODEC_VENC_FORMAT_H265,
	CODEC_VENC_FORMAT_YUV,
	CODEC_VENC_FORMAT_MAX,
	ENUM_DUMMY4WORD(CodecVencFormat)
} CodecVencFormat;

typedef enum {
	CODEC_VENC_PROFILE_BASELINE,
	CODEC_VENC_PROFILE_MAIN,
	CODEC_VENC_PROFILE_HIGH,
	CODEC_VENC_PROFILE_MAX,
	ENUM_DUMMY4WORD(CodecVencProfile)
} CodecVencProfile;

typedef struct {

	BOOL bROIEnable;            ///< ROI enable or disable
	UINT32 uiX;                 ///< X coordinate of top-left point of ROI window
	UINT32 uiY;                 ///< Y coordinate of top-left point of ROI window
	UINT32 uiWidth;             ///< Width of ROI window
	UINT32 uiHeight;            ///< Height of ROI window
	INT32 iQP;             ///< ROI QP,uiDeltaQp=0,  Range: 1~51.   uiDeltaQp=1, Range: -32~31
} CodecVencROI;

typedef struct {

    UINT32 	uiDeltaQp;		// 0 = all fixed qp, 1 = all delta qp
    BOOL    bSmartROI;      // 0: Disable SmartROI    1: Enable SmartROI  (if Enable, Should Close all ROIConfig to disable)
	CodecVencROI sROIConfig[10];    ///< support 10 index to contrl ROI

} CodecVencROISetting;

typedef enum {
	CODEC_VENC_SVCT_DISABLE,    ///< SVC-T disable
	CODEC_VENC_SVCT_LAYER_1,    ///< SVC-T enable with 1 layers
	CODEC_VENC_SVCT_LAYER_2,    ///< SVC-T enable with 2 layers
	CODEC_VENC_SVCT_MAX,
	ENUM_DUMMY4WORD(CodecVencSVCT)
} CodecVencSVCT;

typedef struct {
	CodecVencSVCT eSVCTemporalLayer; ///< SVC temporal layer control
} CodecVencSVCSetting;

typedef struct {
	UINT32 uiEnable;            ///<
	UINT32 uiStaticTime;        ///< Range: 0~20
	UINT32 uiByteRate;          ///< Target byte rate
	UINT32 uiFrameRate;         ///<
	UINT32 uiGOP;               ///< Group of pictures for H.264/265. Range: 1~4096 (Unit: frame)
        UINT32 uiInitIQp;           ///< Initial I-frame QP value, Range: 0~51, 0 is best quality
	UINT32 uiMinIQp;            ///< Minimum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxIQp;            ///< Maximum I-frame QP value. Range: 0~51, 0 is best quality
        UINT32 uiInitPQp;           ///< Initial P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMinPQp;            ///< Minimum P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxPQp;            ///< Maximum P-frame QP value. Range: 0~51, 0 is best quality
	INT32  iIPWeight;           ///< Range. -100~100
	UINT32 uiChangePos;         ///< Range: 0~100
	UINT32 uiRowRcEnalbe;       ///<
	UINT32 uiRowRcQpRange;      ///<
	UINT32 uiRowRcQpStep;       ///<
} CodecVencVBR;

typedef struct {
	UINT32 uiEnable;            ///<
	UINT32 uiStaticTime;        ///< Range: 0~20
	UINT32 uiByteRate;          ///< Target byte rate
	UINT32 uiFrameRate;         ///<
	UINT32 uiGOP;               ///< Group of pictures for H.264/265. Range: 1~4096 (Unit: frame)
	UINT32 uiKeyPPeriod;        ///< base on frame rate   Range: 0~4096
	UINT32 uiInitIQp;           ///< Initial I-frame QP value, Range: 0~51, 0 is best quality
	UINT32 uiMinIQp;            ///< Minimum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxIQp;            ///< Maximum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiInitPQp;           ///< Initial P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMinPQp;            ///< Minimum P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxPQp;            ///< Maximum P-frame QP value. Range: 0~51, 0 is best quality
	INT32  iIPWeight;           ///< Range: -100~100
	INT32  iKeyPWeight;         ///< Range: -100~100
	INT32  iMotionAQStrength;   ///< Range: -20~20
	UINT32 uiRowRcEnalbe;       ///< 0 :disable RowRc,  1:enable RowRc
	UINT32 uiRowRcQpRange;      ///<
	UINT32 uiRowRcQpStep;       ///<
	UINT32 uiStillFrameCnd;     ///< Range: 1~Unlimited
	UINT32 uiMotionRatioThd;    ///< Range: 1~100
	UINT32 uiIPsnrCnd;          ///< Range: 0~1000
	UINT32 uiPPsnrCnd;          ///< Range: 0~1000
	UINT32 uiKeyPPsnrCnd;       ///< Range: 0~1000
} CodecVencEVBR;

typedef struct {
	UINT32 uiEnable;            ///<
	UINT32 uiStaticTime;        ///< Range: 0~20
	UINT32 uiByteRate;          ///< Target byte rate
	UINT32 uiFrameRate;         ///<
	UINT32 uiGOP;               ///< Group of pictures for H.264/265. Range: 1~4096 (Unit: frame)
        UINT32 uiInitIQp;           ///< Initial I-frame QP value, Range: 0~51, 0 is best quality
	UINT32 uiMinIQp;            ///< Minimum I-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxIQp;            ///< Maximum I-frame QP value. Range: 0~51, 0 is best quality
        UINT32 uiInitPQp;           ///< Initial P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMinPQp;            ///< Minimum P-frame QP value. Range: 0~51, 0 is best quality
	UINT32 uiMaxPQp;            ///< Maximum P-frame QP value. Range: 0~51, 0 is best quality
	INT32  iIPWeight;           ///< Range. -100~100
	UINT32 uiRowRcEnalbe;       ///<
	UINT32 uiRowRcQpRange;      ///<
	UINT32 uiRowRcQpStep;       ///<
} CodecVencCBR;

typedef struct {
	UINT32    uiFrameRate;      ///<
	UINT32    uiGOP;            ///< Group of pictures for H.264/265. Range: 1~4096 (Unit: frame)
	UINT32    uiByteRate;       ///< Target byte rate
} CodecVencBRCNoneInfo;

typedef enum {
	CODEC_VENC_BRC_NONE,
	CODEC_VENC_BRC_CBR,
	CODEC_VENC_BRC_VBR,
	CODEC_VENC_BRC_FIXQP,
	CODEC_VENC_BRC_EVBR,
	CODEC_VENC_BRC_MAX,
	ENUM_DUMMY4WORD(CODEC_VENC_BRC_TYPE)
} CODEC_VENC_BRC_TYPE;

typedef enum {
	CODEC_VENC_RT_NONE,         ///< no rotate
	CODEC_VENC_RT_BUILDIN_90,   ///< for codec rotate 90 Clockwise
	CODEC_VENC_RT_BUILDIN_270,  ///< for codec rotate 270 Clockwise
	CODEC_VENC_RT_EXTEND_90,    ///< for extend unit rotate 90 Clockwise
	CODEC_VENC_RT_EXTEND_180,   ///< for extend rotate 180 Clockwise
	CODEC_VENC_RT_EXTEND_270,   ///< for extend rotate 270 Clockwise
	CODEC_VENC_RT_EXTEND_MAX,
	ENUM_DUMMY4WORD(CODEC_VENC_RT_TYPE)
} CodecVencRT;

typedef struct {
	UINT32 uiByteRate;  ///< Target byte rate
	UINT32 uiFrameRate;
	UINT32 uiGOP;       ///< Group of pictures for H.264. Range: 2~120 (Unit: frame)
	UINT32 uiEnable;
	UINT32 uiIFixQP;        ///< fix I frame QP value
	UINT32 uiPFixQP;        ///< fix P frame QP value
} CodecVencFIXQP;



typedef struct {

	INT32  aq_enable;         ///< 0 = aq off, 1 = aq on
	UINT32 aq_str;            ///< aq strength: 1~8 (680);
	INT32  sraq_init_aslog2;  ///< initial i_ASlog2 for sraq (680):@1st Frame; 0~63 (default: 36) #0
	INT32  max_sraq;          ///< max ssim aq: 0~ 15 (default:  8)
	INT32  min_sraq;          ///< min ssim aq: 0~-15 (default: -8)
        INT32  sraq_const_aslog2; // 0:update aslog2 by each frame, 1:use sraq_init_aslog2 for each frame, default: 0
} CodecVencAQ;

typedef struct {
	UINT32 uiLTRInterval;   ///< long-term reference frame interval. range : 0 ~ 4095
	UINT32 uiLTRPreRef;     ///< long-term reference setting. 0 : all long-term reference to IDR frame, 1: reference latest long-term reference frame.
} CodecVencLTR;

typedef struct {

    BOOL bEnable;                 ///< enable/disable codec NR
} CodecVencNRSetting;

typedef struct {
	BOOL bEnable;				///< enable SEI header
} CodecVencSEISetting;	
/**
    CodecVencSetting
    video lineoffset need alignment 64
*/
typedef struct {
	CodecVencInputSource eVencInputSource;  ///< Input video source
	CodecVencFormat eVencFormat;            ///< Video encoded format
	CodecVencProfile eVencProfile;          ///< H.264 profile
	UINT32 uiTargetWidth;                   ///< Target width of encoded data Range: 144 ~ (width of input source)
	UINT32 uiTargetHeight;                  ///< Target height of encoded data Range: 128 ~ (height of input source)
	CODEC_VENC_BRC_TYPE eBRCType;          ///< Bit rate control type)
	CodecVencVBR sVencVBRConfig;            ///< VBR configuration     (for H26x)
	CodecVencCBR sVencCBRConfig;            ///< CBR configuration     (for H26x)
        CodecVencFIXQP sVencFixQPConfig;        ///< Fix QP configuration   (for H26x)
        CodecVencEVBR  sVencEVBRConfig;         ///< EVBR configuration (for H26x)
	CodecVencBRCNoneInfo sVencBRCNoneInfo;  ///< No-BRC configuration  (for mjpeg only)
	CodecVencSVCSetting sSVCSetting;        ///< SVC setting
	UINT32 uiBufNum;                        ///< Encoder buffer number (This parameter is not supported.)
        CodecVencNRSetting sNRSetting;          ///< NR setting
	CodecVencRT eVencRotateType;            ///< Rotate type (#CodecVencRT)
        CodecVencAQ sAQSetting;                 ///< adaptive quantization setting
        CodecVencLTR sLTRInfo;                  ///< long-term reference
	CodecVencSEISetting	sSEISetting;		///< SEI setting 
} CodecVencSetting;

typedef struct {
	UINT32 uiDatatLength;                   ///< Data length of snapshot
	UINT32 uiSnapshotData;                  ///< Pointer to snapshot data
} CodecVencSnapshot;

typedef struct {
	UINT32 uiVencChannelID;                 ///< channel ID
	UINT32 uiWidth;                         ///< scaling width
	UINT32 uiHeight;                        ///< scaling height
} CodecVencSnapshotConfig;

typedef struct {
	BOOL bStart;                            ///< Video encoder status. NVT_TRUE means video encoder already start.
	UINT32 uiMaxWidth;                      ///<
	UINT32 uiMaxHeight;                     ///<
} CodecVencStatus;

typedef struct {
	CodecVencPackType uPackType;            ///< Pack Type
	UINT32 uiDataLength;                    ///< Length of encoded data
	UINT8 *puiStreamPayloadData;            ///< Pointer to encoded data
	BOOL bLastPack;                         ///< Flag of frame end. NVT_TRUE means the last pack of one frame.
} CodecVencPack;

typedef struct {
	CodecVencFormat eVencFormat;            ///< Encoded format of video frame
	UINT32 uiPackNum;                       ///< Pack number in video frame
	UINT32 uiSeqNum;                        ///< Video frame sequence number
	struct timeval sTimeStamp;              ///< Encoded timestamp
    UINT32 uiFrameType;                    ///<  Video Frame type 0:P frame, 1:B frame, 2:I frame , 3:IDR, 4:key p frame
	UINT32 uiSVCTemporalLayerID;            ///< SVC temporal layer ID
	CodecVencPack sVencPack[VENC_STREAM_PACK_MAX];  ///< Pack array of encoded data
} CodecVencStream;

typedef struct {
	UINT8 VPSLength;                        ///< VPS length
	UINT8 VPS[VPS_LENGTH_MAX];              ///< VPS data
	UINT8 SPSLength;                        ///< SPS length
	UINT8 SPS[SPS_LENGTH_MAX];              ///< SPS data
	UINT8 PPSLength;                        ///< PPS length
	UINT8 PPS[PPS_LENGTH_MAX];              ///< PPS data
#if VENC_META_SEI
	UINT8 SEIPrLength;						///< SEI prefix length
	UINT8 SEIPr[SEI_LENGTH_MAX];            ///< SEI prefix data
	UINT8 SEISuLength;                      ///< SEI suffix length
    UINT8 SEISu[SEI_LENGTH_MAX];           ///< SEI suffix data
#endif
} CodecVencParamSet;

typedef enum {
	CODEC_AIN_SOURCE_LINEIN,                ///< Line-in audio source
	CODEC_AIN_SOURCE_MICIN,                 ///< Mic-in audio source
	CODEC_AIN_SOURCE_MAX,
	ENUM_DUMMY4WORD(CodecAinSource)
} CodecAinSource;

typedef enum {
	CODEC_AIO_SAMPLE_RATE_8000 = 8000,
	CODEC_AIO_SAMPLE_RATE_11025 = 11025,
	CODEC_AIO_SAMPLE_RATE_12000 = 12000,
	CODEC_AIO_SAMPLE_RATE_16000 = 16000,
	CODEC_AIO_SAMPLE_RATE_22050 = 22050,
	CODEC_AIO_SAMPLE_RATE_24000 = 24000,
	CODEC_AIO_SAMPLE_RATE_32000 = 32000,
	CODEC_AIO_SAMPLE_RATE_44100 = 44100,
	CODEC_AIO_SAMPLE_RATE_48000 = 48000,
	CODEC_AIO_SAMPLE_RATE_MAX,
	ENUM_DUMMY4WORD(CodecAioSampleRate)
} CodecAioSampleRate;

typedef enum {
	CODEC_AIO_BIT_WIDTH_8BIT,               ///< 8 bits sampling precision
	CODEC_AIO_BIT_WIDTH_16BIT,              ///< 16 bits sampling precision
	CODEC_AIO_BIT_WIDTH_32BIT,              ///< 32 bits sampling precision
	CODEC_AIO_BIT_WIDTH_MAX,
	ENUM_DUMMY4WORD(CodecAioBitWidth)
} CodecAioBitWidth;

typedef enum {
	CODEC_AIO_WORK_MODE_I2S,
	CODEC_AIO_WORK_MODE_AC97,
	CODEC_AIO_WORK_MODE_MAX,
	ENUM_DUMMY4WORD(CodecAioWorkMode)
} CodecAioWorkMode;

typedef enum {
	CODEC_AIO_DUPLEX_MODE_HALF,             ///< Half duplex, audio input will be set to mute when audio output start
	CODEC_AIO_DUPLEX_MODE_FULL,             ///< Full duplex
	CODEC_AIO_DUPLEX_MODE_MAX,
	ENUM_DUMMY4WORD(CodecAioDuplexMode)
} CodecAioDuplexMode;

typedef enum {
	CODEC_AIO_SOUND_MODE_MONO,              ///< Mono mode. This parameter is not supported. (Please select #CODEC_AIO_SOUND_MODE_LEFT or #CODEC_AIO_SOUND_MODE_RIGHT)
	CODEC_AIO_SOUND_MODE_STEREO,            ///< Stereo mode
	CODEC_AIO_SOUND_MODE_LEFT,              ///< Duplicate left channel to right channel
	CODEC_AIO_SOUND_MODE_RIGHT,             ///< Duplicate right channel to left channel
	CODEC_AIO_SOUND_MODE_MAX,
	ENUM_DUMMY4WORD(CodecAioSoundMode)
} CodecAioSoundMode;

typedef enum {
	CODEC_AENC_FORMAT_PCM,                  ///< PCM format
	CODEC_AENC_FORMAT_AAC,                  ///< AAC format
	CODEC_AENC_FORMAT_ULAW,                 ///< G.711 ulaw format. This parameter is not supported.
	CODEC_AENC_FORMAT_ALAW,                 ///< G.711 alaw format. This parameter is not supported.
	CODEC_AENC_FORMAT_MAX,
	ENUM_DUMMY4WORD(CodecAencFormat)
} CodecAencFormat;

typedef struct {
	UINT8 bNREnable;                ///< Audio noise reduction disable/enable
	INT32 iNRdB;                   ///< Maximum suppression level of noise
	INT32 iHPFCutOffFreq;          ///< Cut-off frequency of HPF pre-filtering
	INT32 iBiasSensitive;          ///< Bias sensitive
}CodecANRSetting;

typedef struct {
	BOOL bFiltEnable;
}CodecAudFiltSetting;

typedef struct {
	CodecAinSource eAinSource;              ///< Audio input source (This parameter is not supported.)
	UINT32 uiAinVolume;                     ///< Audio input volume. Range: 0~8, 0 means mute
	CodecAioSampleRate eAinSampleRate;      ///< Sample rate of audio input
	CodecAioBitWidth eAinBitWidth;          ///< Sampling precision. This parameter is not supported (Fixed to #CODEC_AIO_BIT_WIDTH_16BIT).
	CodecAioDuplexMode eAinDuplexMode;      ///< Audio input/output duplex mode
	CodecAioSoundMode eAinSoundMode;        ///< Sound channel mode
	CodecAencFormat eAinFormat;             ///< Audio encoded format
	UINT32 uiAinBitRate;                    ///< Encoded bitrate for AAC. Range: 16~192 (Unit: Kbps). This parameter is not supported (Fixed in 192Kbps)
	CodecANRSetting sAinNRSetting;          ///< audio noise reduction settings
	CodecAudFiltSetting sAinAudFiltSetting; ///< audio filter settings
} CodecAinSetting;

typedef struct {
	CodecAencFormat eAinFormat;             ///< Encoded format of audio frame
	UINT32 uiDataLength;                    ///< Audio frame data length
	UINT32 uiSeqNum;                        ///< Audio frame sequence number
	struct timeval sTimeStamp;              ///< Encoded timestamp
	UINT8 *puiStreamPayloadData;            ///< Pointer to payload data of audio frame
} CodecAencStream;

typedef enum {
	CODEC_AOUT_TARGET_LINEOUT,              ///< Audio output to line-out
	CODEC_AOUT_TARGET_SPEAKER,              ///< Audio output to speaker
	CODEC_AOUT_TARGET_MAX,
	ENUM_DUMMY4WORD(CodecAoutTarget)
} CodecAoutTarget;

typedef struct {
	UINT8 bAECEnable;                 ///< Enable/Disable the AEC
	UINT8 bLeakEstEnable;             ///< Enable/Disable the AEC leak estimate.
	INT32 iLeakEstValue;              ///< Initial Condition of the leak estimate. Value range 25 ~ 99.
	INT32 iNoiseCancelLevel;          ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.
	INT32 iEchoCancelLevel;           ///< Defualt is -50dB. Suggest value range -30 ~ -60. Unit in dB.
	UINT32 uiFilterLen;               ///< Set AEC internal Filter Length. Set to 0 is using default value. Default is 0.
	UINT32 uiFrameSize;               ///< Set AEC internal Frame Size. Set to 0 is using default value. Default is 0.
	UINT32 uiNotchRadius;  	          ///< Set the Notch filter Radius. Value range 0 ~ 1000.
}CodecAECSetting;

/**
    CodecAoutSetting

    @note Because AEC is processed by CPU1, the recommended maximum sample rate is 32KHz. \n
    Please setup audio input format as PCM when AEC is enabled.
*/
typedef struct {
	CodecAoutTarget eAoutTarget;            ///< Audio output target
	CodecAioSampleRate eAoutSampleRate;     ///< Audio output sample rate. This parameter is not supported (It is controlled by audio input).
	UINT32 uiAoutVolume;                    ///< Audio output volume. Range: 0~64, 0 means mute
	CodecAioBitWidth eAoutBitWidth;         ///< Sampling precision. This parameter is not supported (Fixed to #CODEC_AIO_BIT_WIDTH_16BIT).
	CodecAioSoundMode eAoutSoundMode;       ///< Sound channel mode
	CodecAencFormat eAoutFormat;            ///< Audio output data encoded format. (Only support PCM now)
	CodecAECSetting sAoutAECSetting;        ///< Audio AEC Setting structure
} CodecAoutSetting;

typedef struct {
	UINT32 uiDataLength;                    ///< Payload data length
	UINT8 *puiStreamPayloadData;            ///< Pointer to audio output data
} CodecAoutStream;

typedef struct {
	BOOL bStart;                            ///< Audio output to line-out
} CodecAoutStatus;


/**
    CodecVencSetting

    @note HDMI output is only supported in two stream mode(setup in uItron)
*/
typedef enum {
	CODEC_VOUT_INTERFACE_CVBS_NTSC,             ///< CVBS with NTSC
	CODEC_VOUT_INTERFACE_CVBS_PAL,              ///< CVBS with PAL
	CODEC_VOUT_INTERFACE_LCD,                   ///<
	//CODEC_VOUT_INTERFACE_HDMI_720x480P60,     ///< HDMI with 720x480p60
	CODEC_VOUT_INTERFACE_HDMI_1280x720P30,      ///< HDMI with 1280x720P30
	CODEC_VOUT_INTERFACE_HDMI_1920x1080P30,     ///< HDMI with 1920x1080P30
	CODEC_VOUT_INTERFACE_HDMI_1920x1080I60,     ///< HDMI with 1920x1080i60
	CODEC_VOUT_INTERFACE_HDMI_3840x2160P30,     ///< HDMI with 3840x2160P30
	CODEC_VOUT_INTERFACE_HDMI_3840x2160P25,     ///< HDMI with 3840x2160P25
	CODEC_VOUT_INTERFACE_HDMI_3840x2160P24,     ///< HDMI with 3840x2160P24
	CODEC_VOUT_INTERFACE_HDMI_4096x2160P30,     ///< HDMI with 4096x2160P30
	CODEC_VOUT_INTERFACE_HDMI_4096x2160P25,     ///< HDMI with 4096x2160P25
	CODEC_VOUT_INTERFACE_HDMI_4096x2160P24,     ///< HDMI with 4096x2160P24
	CODEC_VOUT_INTERFACE_AUTO,             ///<  monitor auto detection
	CODEC_VOUT_INTERFACE_MAX,
	ENUM_DUMMY4WORD(CodecVoutInterface)
} CodecVoutInterface;
/*
typedef enum {
    CODEC_VOUT_INTERFACE_CVBS_NTSC,
    CODEC_VOUT_INTERFACE_CVBS_PAL,
    CODEC_VOUT_INTERFACE_HDMI_720x480P60,
    CODEC_VOUT_INTERFACE_HDMI_1920x1080I60,
    CODEC_VOUT_INTERFACE_HDMI_AUTO,
    CODEC_VOUT_INTERFACE_MAX,
    ENUM_DUMMY4WORD(CodecVoutInterface)
} CodecVoutInterface;
*/
typedef struct {
	CodecVoutInterface eVoutInterface;      ///< Video output enable
	INT32 iChannel;                         ///< Video output interface
//	BOOL  bEnable;                         ///< Video output Enable/ Disable
} CodecVoutSetting;

typedef enum {
	Codec_OSD_TEXT_POSITION_UPPER_LEFT,
	Codec_OSD_TEXT_POSITION_UPPER_RIGHT,
	Codec_OSD_TEXT_POSITION_LOWER_LEFT,
	Codec_OSD_TEXT_POSITION_LOWER_RIGHT,
	Codec_OSD_TEXT_POSITION_MAX,
	ENUM_DUMMY4WORD(CodecOSDTextPosition)
} CodecOSDTextPosition;

#if 0
typedef struct {
	UINT32 uiSrcVideoID;    ///< The source video channel, the blending channel. Range: 0~2
	UINT32 uiDesVideoID;    ///< The destination video channel, copy data to the blending channel
	BOOL bEnable;           ///< Enable / disable blending function. NVT_TRUE: enable, NVT_FALSE: disable
	UINT32 uiAlpha;         ///< A: is the source image. B: is alpha value. C: is the destination image. The copy blending weight is (B/256)*A + ((256-B)/256)*C. Range: 0~256
} CodecBlendSetting;
#endif


typedef struct {

	UINT32 uiPCT;      ///< Foreground area threshold in tampering detection.
                           ///< 50 means that tampering detection timer is start if foreground is over 50%.
                           ///< Range: 1~100 (unit is percentage)

	UINT32 uiFrameNum; ///< Foreground keep time threshold in tampering detection.
                           ///< 30 means tampering is detected if foreground area is over uiPCT
                           ///< and keep over 30 video detected frames. Range: 0~65535 (unit is detected frame)

}CodecTDSetting;


typedef struct {

	BOOL   bStatus;              ///< TD trigger or not, 0 : not trigger,  1: trigger
	UINT32 uiPCT_Cur;            ///< currently Foreground area threshold
	UINT32 uiFrameNum_Cur;       ///< currently frame which tampering is detected

}CodecTDStatus;

typedef struct {
        UINT32 uiMaxFaceNum;      ///< Maximum number of tracking faces. range : 1~36
        UINT32 uiFaceScale;       ///< The ratio of minimum face size to maximum image size.  range 4~48
        UINT32 uiProcessRate;     ///< Process rate of face tracking. range >=1
} CodecFDSetting;

typedef struct {
        UINT32  x;      ///< X coordinate of the top-left point of the rectangle
        UINT32  y;      ///< Y coordinate of the top-left point of the rectangle
        UINT32  w;      ///< Rectangle width
        UINT32  h;      ///< Rectangle height
} FDURECT;


typedef struct {
        char Event;                  ///<  face detection event type
        UINT32 uiFaceNumber;         ///< the number of object which are detected
        FDURECT info[FD_NUM_MAX];    ///< the result about face detction information
} CodecFDData;


typedef struct{

	UINT32	uiUpdateProb;          ///< background update frequency, default value:0 Range: 0~255
	BOOL	bUpdateNeighborEn;     ///< Neighborhood update enable. 1 means that background
                                       ///< update will also handle neighbor area. It can improve accuracy,
                                       ///<  but foreground will be updated to background gradually.
	BOOL	bDeghostEn;            ///<  1: enable removing ghost object
	UINT32  uiMaxFGFrameNum;       ///< Maximum foreground life time, foreground will be update to background after uiMaxFGFrameNum frames.
                                       ///< Range: 0~ 65535
	UINT32  uiMinOBJAreaPCT;       ///< Minimum object area percentage of the whole image. Range:0~10000
	UINT32  uiTHCrossFrameNum;     ///< Range: 0 ~ 65535
	UINT32  uiTHFeature;           ///< Range: 0 ~ 255
	UINT32  uiSensi;               ///< Foreground sensitivity. The smaller value means the more sensitive detection. Range: 25~200
	UINT32  uiBlkNumH;             ///< Block number in horizontal direction. Range: 1~32
	UINT32  uiBlkNumW;             ///< Block number in vertical direction.   Range: 1~32
	UINT32  MinOBJBlkNum;          ///< Minimum block number of the detected object. Range: 0 ~ 32*32
	UINT32  FGPCTperBlk;           ///< Foreground threshold. If the percentage of foreground pixels in one
                                       ///< block is larger than uiFGPerBlkPCT %, this block is considered as foreground. Range: 0~100

}CodecBCSetting;

typedef struct {
	UINT32 uiCoordinateX;
	UINT32 uiCoordinateY;
	UINT32 uiWidth;
	UINT32 uiHeight;
} CodecObjData;

typedef struct {
	BOOL         bUpdated;
	UINT32       uiObjectNum;
	CodecObjData sObjectData[BC_MAX_OBJ_NUM];
} CodecBCData;


typedef struct {
	BOOL       bUpdated;
	UINT32     uiObjectNum;
	CodecObjData sObjectData[BC_MAX_BLK_NUM];
} CodecBCBLKData;

typedef enum {
	CODEC_PIXEL_FMT_YUV422_PLANAR  = 0x00,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h), and V(w/2,h)
	CODEC_PIXEL_FMT_YUV420_PLANAR  = 0x01,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h/2), and V(w/2,h/2)
	CODEC_PIXEL_FMT_YUV422_PACKED  = 0x02,    ///< 2 color plane, pixel=Y(w,h), UV(w,h)
	CODEC_PIXEL_FMT_YUV420_PACKED  = 0x03,    ///< 2 color plane, pixel=Y(w,h), UV(w,h/2)
	CODEC_PIXEL_FMT_Y_ONLY         = 0x04,    ///< 1 color plane, pixel=Y(w,h),
	CODEC_PIXEL_FMT_ARGB565        = 0x05,    ///< 2 color plane, pixel=A8(w,h),RGB565(2w,h)
	CODEC_PIXEL_FMT_RGB888_PLANAR  = 0x06,    ///< 3 color plane, pixel=R(w,h), G(w,h), B(w,h)
	CODEC_PIXEL_FMT_ARGB888_PACKED = 0x07,    ///< 1 color plane, pixel=ARGB(4w,h)
	CODEC_PIXEL_FMT_MAX,
	ENUM_DUMMY4WORD(CodecPixelFormat)
} CodecPixelFormat;


typedef struct {
	UINT32              uiWidth;                     ///< image width
	UINT32              uiHeight;                    ///< image height
	CodecPixelFormat    ePxlFmt;                     ///< pixel format
	UINT32              uiPxlAddr[MAX_PLANE_NUM];    ///< pixel address
	UINT32              uiLineOffset[MAX_PLANE_NUM]; ///< pixel lineoffset
} CodecImageBuf;

typedef struct {
	UINT32 uiBcWidth;
	UINT32 uiBcHeight;
	UINT8  uBcReset;
	UINT8  uBcRsltNew;
	UINT8  uBcUpdateProb;
	UINT8  uBcUpdateNeighborEn;
	UINT8  uBcDeghostEn;
	UINT32 uiBcMaxFgFrmNum;
	UINT32 uiBcMinObjAreaPct;
	UINT32 uiBcThCrossFrmNum;
	UINT8  uBcThFeature;
	UINT8  uBcSensi;
	UINT32 uiReserved[6];
} BC_USR_PARAM;

typedef struct {
	UINT32 uiFps; //fps=0 means turn off
	UINT32 uiBcMode;
	BC_USR_PARAM sParamInfo;
	UINT32  reserved[6];
} BC_PARAM_INFO;

typedef struct {
	UINT32	EventFlag;
	UINT32	NotifyFlag;
	UINT32	SerialNumber;
	char EventData[EVENT_BUF_SIZE];
} CodecEventInfo;

typedef struct {
	UINT32 en_auto_adj_param;
	UINT32 en_ref_md;
	UINT32 en_too_light;
	UINT32 en_too_dark;
	UINT32 en_blur;
} VQAEnable;

typedef struct {
	UINT32 width;
	UINT32 height;
	UINT32 mb_x_size;
	UINT32 mb_y_size;
	UINT32 auto_adj_period;
	UINT32 g_alarm_frame_num;
} VQAGlobal;

typedef struct {
	UINT32 too_light_strength_th;
	UINT32 too_light_cover_th;
	UINT32 too_light_alarm_times;
	UINT32 too_dark_strength_th;
	UINT32 too_dark_cover_th;
	UINT32 too_dark_alarm_times;
} VQALight;

typedef struct {
	UINT32 blur_strength;
	UINT32 blur_cover_th;
	UINT32 blur_alarm_times;
} VQAContrast;

typedef struct {
	UINT32	fps; //fps=0 means turn off
	VQAEnable enable_param;
	VQAGlobal global_param;
	VQALight light_param;
	VQAContrast contrast_param;
} IVSVQAParam;
// video encoder
/**
    Get current video encoder setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencSetting   Pointer to the data structure of video encoder setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencSetting(UINT32 uiVencChannelID, CodecVencSetting *psVencSetting);

/**
    Set new video encoder setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVencSetting    Pointer to the data structure of video encoder setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVencSetting(UINT32 uiVencChannelID, CodecVencSetting *psVencSetting);

/**
    Get current video encoder ROI setting

    @param[in] uiVencChannelID      Video encoder channel ID
    @param[out] psVencROISetting    Pointer to the data structure of video encoder ROI setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencROISetting(UINT32 uiVencChannelID, CodecVencROISetting *psVencROISetting);

/**
    Set new video encoder ROI setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVencROISetting Pointer to the data structure of video encoder ROI setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVencROISetting(UINT32 uiVencChannelID, CodecVencROISetting *psVencROISetting);

/**
    Start video encoder

    @param[in] uiVencChannelID  Video encoder channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StartVenc(UINT32 uiVencChannelID);

/**
    Stop video encoder

    @param[in] uiVencChannelID  Video encoder channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StopVenc(UINT32 uiVencChannelID);

/**
    Request next frame encoded as I-frame

    @param[in] uiVencChannelID  Video encoder channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_RequestVencIFrame(UINT32 uiVencChannelID);

/**
    Get video encoded data

    @note
    -# This function must be used after video encoder start
    -# In order to keep encoder flow non-blocking, encoded buffer not be locked even if this buffer is being gotten by user. \n
    The encoded buffer always is refreshed in its turn. If need to use encoded buffer more than 5 seconds, \n
    it's recommended to set bCopyBuf as NVT_TRUE and get stream from indirect memory buffer. \n
    This way has additional memory copy effort, but can ensure that payload data not be refreshed.

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencStream    Pointer to the data structure of video encoded data
    @param[in] uiTimeOutMs      Function time out, 0 means block mode(Unit is millisecond)
    @param[in] bCopyBuf         Payload data handle method, NVT_TRUE means copy data into additional memory buffer *

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencStream(UINT32 uiVencChannelID, CodecVencStream *psVencStream, UINT32 uiTimeOutMs, BOOL bCopyBuf);

/**
    Release video encoded data (This function must be used after video encoder start.)

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVencStream     Pointer to the data structure of video encoded data

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_ReleaseVencStream(UINT32 uiVencChannelID, CodecVencStream *psVencStream);

/**
    Get parameter set for H.264

    @note
    -# This function must be used after video encoder start

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencParamSet  Pointer to the data structure of parameter set

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_RequestVencParamSet(UINT32 uiVencChannelID, CodecVencParamSet *psVencParamSet);

/**
    Get one snapshot that is encoded to JPEG

    @note
    -# This function must be used after video encoder start

    @param[in] psVencSnapshotConfig Pointer to the data structure of JPEG snapshot configuration
    @param[out] psVencSnapshot      Pointer to the data structure of JPEG snapshot

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_RequestVencJPEG(CodecVencSnapshotConfig *psVencSnapshotConfig, CodecVencSnapshot *psVencSnapshot);

/**
    Release the JPEG snapshot resource

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_ReleaseVencJPEG(void);

/**
    Get video encoder status

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVencStatus    Pointer to the data structure of encoder status

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVencStatus(UINT32 uiVencChannelID, CodecVencStatus *psVencStatus);

// video output
/**
    Get current video output setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[out] psVoutSetting   Pointer to the data structure of video output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetVoutSetting(UINT32 uiVoutChannelID, CodecVoutSetting *psVoutSetting);

/**
    Set new video output setting

    @param[in] uiVencChannelID  Video encoder channel ID
    @param[in] psVoutSetting    Pointer to the data structure of video output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVoutSetting(UINT32 uiVoutChannelID, CodecVoutSetting *psVoutSetting);

/**
    set display Enable / Disable

    @param[in] bEnable  enable/disable disaplay

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetVoutEnable(BOOL bEnable);

// audio input
/**
    Get current audio input setting

    @param[in] uiAinChannelID   Audio input channel ID
    @param[out] psAinSetting    Pointer to the data structure of audio input setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern NVTRET NvtCodec_GetAinSetting(UINT32 uiAinChannelID, CodecAinSetting *psAinSetting);

/**
    Set new audio input setting

    @param[in] uiAinChannelID   Audio input channel ID
    @param[in] psAinSetting     Pointer to the data structure of audio input setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetAinSetting(UINT32 uiAinChannelID, CodecAinSetting *psAinSetting);

/**
    Start audio input

    @param[in] uiAinChannelID   Audio input channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StartAin(UINT32 uiAinChannelID);

/**
    Stop audio input

    @param[in] uiAinChannelID   Audio input channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StopAin(UINT32 uiAinChannelID);

/**
    Get audio encoded data

    @note
    -# This function must be used after audio input start

    @param[in] uiAinChannelID   Audio input channel ID
    @param[out] psAencStream    Pointer to the data structure of audio encoded data
    @param[in] uiTimeOutMs      Function time out, 0 means block mode(Unit is millisecond)
    @param[in] bCopyBuf         Payload data handle method, NVT_TRUE means copy data into additional memory buffer *

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetAinStream(UINT32 uiAinChannelID, CodecAencStream *psAencStream, UINT32 uiTimeOutMs, BOOL bCopyBuf);

/**
    Release audio encoded data

    @note
    -# This function must be used after audio input start

    @param[in] uiAinChannelID   Audio input channel ID
    @param[in] psAencStream     Pointer to the data structure of audio encoded data

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_ReleaseAinStream(UINT32 uiAinChannelID, CodecAencStream *psAencStream);

// audio output
/**
    Get current audio output setting.

    @param[in] uiAoutChannelID  Audio output channel ID
    @param[out] psAoutSetting   Pointer to the data structure of audio output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetAoutSetting(UINT32 uiAoutChannelID, CodecAoutSetting *psAoutSetting);

/**
    Set new audio output setting.

    @param[in] uiAoutChannelID  Audio output channel ID
    @param[in] psAoutSetting    Pointer to the data structure of audio output setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SetAoutSetting(UINT32 uiAoutChannelID, CodecAoutSetting *psAoutSetting);

/**
    Start audio output.

    @param[in] uiAoutChannelID  Audio output channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StartAout(UINT32 uiAoutChannelID);

/**
    Stop audio output.

    @param[in] uiAoutChannelID  Audio output channel ID

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_StopAout(UINT32 uiAoutChannelID);

/**
    Play audio data

    @note
    -# This function must be used after audio output start.

    @param[in] uiAoutChannelID      Audio output channel ID
    @param[in] psAoutStream         Pointer to the data structure of audio output data
    @param[out] puiSendStreamSize   The size of audio data that is send to play buffer

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_SendAoutStream(UINT32 uiAoutChannelID, CodecAoutStream *psAoutStream, UINT32 *puiSendStreamSize);

/**
    Get audio output status

    @param[in] uiAoutChannelID  Audio output channel ID
    @param[out] psAoutStatus    Pointer to the data structure of audio output status

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtCodec_GetAoutStatus(UINT32 uiAoutChannelID, CodecAoutStatus *psAoutStatus);

// overlay
/**
    application-wide initialization

    @param[in] uiMaxSize  max osd size : 2 * max width * max height

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_InitVds(unsigned int uiMaxSize);

/**
    application-wide un-initialization

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_UninitVds(void);

/**
    application-wide ping-pong buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] width  max osd width for image #id
    @param[in] height max osd height for image #id

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocOSDBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

/**
    add/update an mask

    @param[in] image mask configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AddVdsMask(CodecOSDIMAGE *image);

/**
    add/update an osd

    @param[in] image osd configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AddVdsOSD(CodecOSDIMAGE *image);

/**
    draw ext mask for the specified stream

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsExtMask(int vid);

/**
    draw ps mask for the specified ipl

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsPsMask(int vid);

/**
    draw btn ext osd for the specified stream

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsBtnExtOSD(int vid);

/**
    draw ps osd for the specified stream

    @param[in] vid stream number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SwapVdsPsOSD(int vid);

/**
    clear all osd and mask

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_ClearVds(void);

/**
    free ping pong buffer

    @param[in] if actually return ping pong buffer to system

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_FreeBuffer(int release);

/**
    defrag ping pong buffer

    @param[in] void

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_Defragment(void);

/**
    delete an osd

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_DelVdsOSD(VDS_PHASE phase, int id);

/**
    Get information of images stored in videosprite

    @param[in]      phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in]      mask   requery mask(1) or osd(0)
    @param[out]     image  information of images stored in videosprite
    @param[in,out]  num    input:number of input image ; output:number of images stored in videosprite

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetImages(VDS_PHASE phase, int mask, CodecOSDIMAGE *image, int *num);

/**
    Get free ping pong buffer of an osd

    @param[in]      phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in]      id     id of osd
    @param[out]     addr   address of the ping pong buffer
    @param[out]     size   size of the ping pong buffer

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetLockPingPong(VDS_PHASE phase, int id, unsigned int *addr, unsigned int *size);

/**
    Release resuorce locked by NvtCodec_GetLockPingPong()

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_UnlockPingPong(void);

/**
    application-wide ping-pong buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] width  max osd width for image #id
    @param[in] height max osd height for image #id

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocPingPongBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

/**
    application-wide scale buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] PreW   max width of input osd for image #id
    @param[in] PreH   max height of intpu osd for image #id
    @param[in] PostW  max width of output osd for image #id after scaling
    @param[in] PostH  max height of output osd for image #id after scaling

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocScaleBuf(VDS_PHASE phase, int id, unsigned int PreW, unsigned int PreH, unsigned int PostW, unsigned int PostH);

/**
    application-wide single buffer configuration

    @param[in] phase  VDS_PHASE_BTN VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] id     serial number
    @param[in] width  max osd width for image #id
    @param[in] height max osd height for image #id

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocSingleBuf(VDS_PHASE phase, int id, unsigned int width, unsigned int height);

/**
    add/update an osd of scale buffer with a specified scale ratio

    @param[in] image osd configuration
    @param[in] ratio scale up ratio : 1 2 4 8 16

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AddVdsScaleOSD(CodecOSDIMAGE *image, int ratio);

/**
    allocate mosaic's buffer from uitron for VDS_PHASE_EXT

    @param[in] uiMaxSize how many bytes to allocate

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_InitMosaicBuf(unsigned int uiMaxSize);

/**
    application-wide mosaic buffer configuration

    @param[in] phase  only VDS_PHASE_EXT is valid because VDS_PHASE_PS allocates mosaic buffer in hardware driver
    @param[in] vid    streaming number
    @param[in] width  streaming width
    @param[in] height streaming height

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_AllocMosaicBuf(VDS_PHASE phase, int vid, unsigned int width, unsigned int height);

/**
    free mosaic buffer

    @param[in] release if actually return mosaic buffer to system

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_FreeMosaicBuffer(int release);

/**
    set mosaic block size

    @param[in] phase  VDS_PHASE_EXT VDS_PHASE_PS
    @param[in] vid    for VDS_PHASE_EXT it's vid ; for VDS_PHASE_PS it's ipl_id
    @param[in] blksz  8/16/32/64

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetVdsMosaicBlksz(VDS_PHASE phase, int vid, unsigned int blksz);

/**
    set motion info for alg

    @param[in] Background Construction param

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetBcCfg(BC_PARAM_INFO *pBcInfo);

/*
    Wait BC event and get BC result
*/
extern ER NvtCodec_GetBcData(IPCAM_BC_RESULT *bc_data, int iTimeOutMillisec);

/*
	Exit wait BC event 
*/
extern ER NvtCodec_ExitBc(void);

/**
    Start face detection from uITRON

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/

/**
    set VQA parameters for alg

    @param[in] vqa param

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetVqaParam(IVSVQAParam *sIVSVQAParam);

/**
    get VQA result

    @param[in] pointer of vqa result
    @param[in] timeout in millisec

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetVqaResult(IPCAM_VQA_RESULT *pVqaRes, int iTimeOutMillisec);

/*
	Exit wait VQA event
*/
extern ER NvtCodec_ExitVqa(void);

/**
    set PD parameters for alg

    @param[in] pd param

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_StartFD(UINT32 uiFDIdx);

/**
    Stop face detection from uITRON

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_StopFD(UINT32 uiFDIdx);

/**
    Get current face detection setting

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)
    @param[in] psFDSetting  Pointer to the data structure of face detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetFDSetting(UINT32 uiFDIdx, CodecFDSetting *psFDSetting);

/**
    Set new face detection setting

    @param[in] uiFDIdx      Index of face detection window (Only supports one detection window)
    @param[in] psFDSetting  Pointer to the data structure of face detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_SetFDSetting(UINT32 uiFDIdx, CodecFDSetting *psFDSetting);


/**
    Get face detection result

    @note
    -# This function must be used after face detection start

    @param[in] uiFDIdx          Index of face detection window (Only supports one detection window)
    @param[in] fd_data         Pointer to the data structure of face detection result

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_GetFDData(char *fd_data, int timeout_millisec);

/*
        If linux do not using NvtCodec_GetFDData anymore, please call NvtCodec_ExitFd to release resource
*/
extern ER NvtCodec_ExitFd(void);



/**
    Disable/Eanble Tamper detection, in uitron, enable flag is the same with BC

     @param[in] bEnable         0: disable Tamper detection, 1: enable tamper detection

     @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_TDStart(BOOL bEnable);



/**
     get tamper detection config function

    @param[in] psTDSetting  result Pointer to the data structure of tamper detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_GetTDSetting(CodecTDSetting *psTDSetting);


/**
     set tamper detection config function

    @param[in] psTDSetting  Pointer to the data structure of tamper detection setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_SetTDSetting(CodecTDSetting *psTDSetting);


/**
     get tamper detection result function

    @param[in] psTDStatus  Pointer to the data structure of tamper detection status

    @return NvtErrorCode (Refer to nvt_type.h)
*/



extern ER NvtCodec_GetTDStatus(CodecTDStatus *psTDStatus);


/**
    Disable/Eanble backgound construction, in uitron, Enable flag is the same with Tamper detection

     @param[in] bEnable         0: disable BC, 1: enable BC

     @return NvtErrorCode (Refer to nvt_type.h)
*/


extern ER NvtCodec_BCStart(BOOL bEnable);


/**
     get background constructure config function

    @param[in] psBCSetting  result Pointer to the data structure of BC setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_GetBCSetting(CodecBCSetting *psBCSetting);


/**
     set background constructure config function

    @param[in] psBCSetting  Pointer to the data structure of BC setting

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_SetBCSetting(CodecBCSetting *psBCSetting);


/**
     reset background constructure

     @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_ResetBC(void);


/**
     Get foreground object result

     @param[in] psBCData   Pointer to the data structure of CodecBCData
     @param[in] psTargetCoord   Mapping coordinate of background construction result

      @return NvtErrorCode (Refer to nvt_type.h)

*/
extern ER NvtCodec_GetBCResult(CodecBCData *psBCData, CodecObjData *psTargetCoord);

/**
     Get foreground block result

     @param[in] psBCBLKData   Pointer to the data structure of CodecBCBLKData
     @param[in] psTargetCoord   Mapping coordinate of background construction result

      @return NvtErrorCode (Refer to nvt_type.h)

*/
extern ER NvtCodec_GetBCBLKResult(CodecBCBLKData *psBCBLKData, CodecObjData *psTargetCoord);


/**

     Get foreground image

     @param[in] psImageBuf   Pointer to the data structure of foreground image

     @return NvtErrorCode (Refer to nvt_type.h)

*/

extern ER NvtCodec_GetBCForeground(CodecImageBuf *psImageBuf);


/**

     Get background image

     @param[in] psImageBuf   Pointer to the data structure of background image

     @return NvtErrorCode (Refer to nvt_type.h)

*/

extern ER NvtCodec_GetBCBackground(CodecImageBuf *psImageBuf);



/**
    release foreground image

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_ReleaseBCForeground(void);


/**
    release background image

    @return NvtErrorCode (Refer to nvt_type.h)
*/

extern ER NvtCodec_ReleaseBCBackground(void);

/**
    start event handler by using nvtevent

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_EventHandler_Init(void);

/**
    stop event handler by using nvtevent

    @return NvtErrorCode (Refer to nvt_type.h)
*/
extern ER NvtCodec_EventHandler_Exit(void);

//blending
/**
    Get the current blending information

    @param[in] uiBlendChannelID     Blending channel ID (Reserved now)
    @param[out] psBlendSetting      Pointer to the data structure of Blending configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtCodec_GetBlendSetting(UINT32 uiBlendChannelID, CodecBlendSetting *psBlendSetting);

/**
    Set the blending information

    @param[in] uiBlendChannelID     Blending channel ID (Reserved now)
    @param[in] psBlendSetting       Pointer to the data structure of Blending configuration

    @return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtCodec_SetBlendSetting(UINT32 uiBlendChannelID, CodecBlendSetting *psBlendSetting);





#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTCODEC_H  */
