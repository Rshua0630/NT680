#ifndef _NVTCFG_DEFINE_H
#define _NVTCFG_DEFINE_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtcfg_define.h

    @version

    @date
*/
#include <stdio.h>
#include <time.h>
#include "nvt_type.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _FIRMWARE_VERSION_
#define FIRMWARE_VERSION _FIRMWARE_VERSION_
#else
#define FIRMWARE_VERSION     "v1.01.001"
#endif

#ifdef _MANUFACTURER_
#define MANUFACTURER _MANUFACTURER_
#else
#define MANUFACTURER         "Novatek"
#endif

#ifdef _MODEL_NAME_
#define MODEL_NAME _MODEL_NAME_
#else
#define MODEL_NAME           "9668X_IPCAM"
#endif

#if 1

#define LONG64 long long
/* xsd:boolean */
enum xsd__boolean {xsd__boolean__false_ = 0, xsd__boolean__true_ = 1};

/* tt:FactoryDefaultType */
enum tt__FactoryDefaultType {tt__FactoryDefaultType__Hard = 0, tt__FactoryDefaultType__Soft = 1};

/* tt:NetworkProtocolType */
enum tt__NetworkProtocolType {tt__NetworkProtocolType__HTTP = 0, tt__NetworkProtocolType__HTTPS = 1, tt__NetworkProtocolType__RTSP = 2};

/* tt:ScopeDefinition */
enum tt__ScopeDefinition {tt__ScopeDefinition__Fixed = 0, tt__ScopeDefinition__Configurable = 1};

/* tt:DiscoveryMode */
enum tt__DiscoveryMode {tt__DiscoveryMode__NonDiscoverable = 0, tt__DiscoveryMode__Discoverable = 1};

/* tt:VideoEncoding */
enum tt__VideoEncoding {tt__VideoEncoding__JPEG = 0, tt__VideoEncoding__MPEG4 = 1, tt__VideoEncoding__H264 = 2, tt__VideoEncoding__H265 = 3};

/* tt:RateControlMode */
enum tt__RateControlMode {tt__RateControlMode__CBR = 0, tt__RateControlMode__VBR = 1};

/* tt:Mpeg4Profile */
enum tt__Mpeg4Profile {tt__Mpeg4Profile__SP = 0, tt__Mpeg4Profile__ASP = 1};

/* tt:H264Profile */
enum tt__H264Profile {tt__H264Profile__Baseline = 0, tt__H264Profile__Main = 1, tt__H264Profile__Extended = 2, tt__H264Profile__High = 3};

/* tt:AudioDulpexMode */
enum tt__AudioDulpexMode {tt__AudioDulpexMode__Full = 0, tt__AudioDulpexMode__Half = 1};

/* tt:AudioEncoding */
enum tt__AudioEncoding {tt__AudioEncoding__G711 = 0, tt__AudioEncoding__G726 = 1, tt__AudioEncoding__AAC = 2, tt__AudioEncoding__G711alaw = 3, tt__AudioEncoding__PCM = 4, tt__AudioEncoding__PPCM = 5};


/* tt:SendPrimacy */
enum tt__SendPrimacy {tt__SendPrimacy__Audio__Server = 0, tt__SendPrimacy__Audio__Client = 1, tt__SendPrimacy__Audio__Auto = 2};

/* tt:AudioOutputTarget */
enum tt__AudioOutputTarget {tt__AudioOutputTarget__LineOut = 0, tt__AudioOutputTarget__Speaker = 1};

/* tt:NetworkHostType */
enum tt__NetworkHostType {tt__NetworkHostType__IPv4 = 0, tt__NetworkHostType__IPv6 = 1, tt__NetworkHostType__DNS = 2};

/* tt:DataTimeType */
enum tt__DataTimeType {tt__DataTimeType__Manual = 0, tt__DataTimeType__NTP = 1};

/* tt:NTPUpdatePeriod */
enum tt__NTPUpdateCycle {tt__NTPUpdateCycle__hourly = 0, tt__NTPUpdateCycle__daily = 1, tt__NTPUpdateCycle__weekly = 2, tt__NTPUpdateCycle__monthly = 3, tt__NTPUpdateCycle__max = 4};

/* tt:ColorMode */
enum tt__ColorMode {tt__ColorMode__Normal = 0, tt__ColorMode__Gray = 1};

/* tt:OsdUserTextPos */
enum tt__OsdUserTextPos {tt__OsdUserTextPos__UpperLeft = 0, tt__OsdUserTextPos__UpperRight = 1, tt__OsdUserTextPos__LowerLeft = 2, tt__OsdUserTextPos__LowerRight = 3, };

/* tt:OsdDateTimeFmt */
enum tt__OsdDateTimeFmt {tt__OsdDateTimeFmt1 = 0, //YYYY/MM/DD - HH:MM:SS
                         tt__OsdDateTimeFmt2 = 1, //MM/DD/YYYY - HH:MM:SS
                         tt__OsdDateTimeFmt3 = 2, //HH:MM:SS - YYYY/MM/DD
                         tt__OsdDateTimeFmt4 = 3, //HH:MM:SS - MM/DD/YYYY
                        };

enum tt__UserLevel
{
    tt__UserLevel__Anonymous = 1,	///< xs:string value="Anonymous"
	tt__UserLevel__User,	        ///< xs:string value="User"
	tt__UserLevel__Operator,	    ///< xs:string value="Operator"
	tt__UserLevel__Administrator,	///< xs:string value="Administrator"
	tt__UserLevel__Extended,	    ///< xs:string value="Extended"
};

/* tt:VideoResolution */
struct tt__VideoResolution
{
    int Width;  /* required element of type xsd:int */
    int Height; /* required element of type xsd:int */
};

/* tt:IntRange */
struct tt__IntRange
{
    int Min;    /* required element of type xsd:int */
    int Max;    /* required element of type xsd:int */
};

/* tt:VideoRateControl */
struct tt__VideoRateControl
{
    enum tt__RateControlMode RateControlMode; /* required element of type tt:RateControlMode */
    int FrameRateLimit; /* required element of type xsd:int */
    int EncodingInterval;   /* required element of type xsd:int */
    int BitrateLimit;   /* required element of type xsd:int */
};

/* tt:VideoCropControl */
struct tt__VideoCropControl
{
    enum xsd__boolean Enable; /* required element of type xsd:boolean */
    int CoordinateX;   /* required element of type xsd:int */
    int CoordinateY;   /* required element of type xsd:int */
};

/* tt:Mpeg4Configuration */
struct tt__Mpeg4Configuration
{
    int GovLength;  /* required element of type xsd:int */
    enum tt__Mpeg4Profile Mpeg4Profile; /* required element of type tt:Mpeg4Profile */
};

/* tt:H264Configuration */
struct tt__H264Configuration
{
    int GovLength;  /* required element of type xsd:int */
    enum tt__H264Profile H264Profile;   /* required element of type tt:H264Profile */
};

struct tt__IntRectangle
{
    int x;  /* required attribute of type xsd:int */
    int y;  /* required attribute of type xsd:int */
    int width;  /* required attribute of type xsd:int */
    int height; /* required attribute of type xsd:int */
};

struct tt__PTZFilter
{
    enum xsd__boolean Status;   /* required element of type xsd:boolean */
    enum xsd__boolean Position; /* required element of type xsd:boolean */
    char *__anyAttribute;   /* optional attribute of type xsd:anyType */
};

struct tt__EventSubscription
{
    struct wsnt__FilterType *Filter;    /* optional element of type wsnt:FilterType */
    struct _tt__EventSubscription_SubscriptionPolicy *SubscriptionPolicy;   /* optional element of type tt:EventSubscription-SubscriptionPolicy */
    int __size; /* sequence of elements <-any> */
    char **__any;
    char *__anyAttribute;   /* optional attribute of type xsd:anyType */
};
#endif


#define SMALL_INFO_LENGTH    20
#define IP_LENGTH            20
#define MAC_LENGTH            6
#define MACH_ADDR_LENGTH     32
#define INFO_LENGTH          100
#define LARGE_INFO_LENGTH    100

#define ONVIF_VERSION_MAJOR  2
#define ONVIF_VERSION_MINOR  4

#define TRUE 1
#define FALSE 0

// user
#define USER_MAX_NUM          5
#define USER_NAME_MAX_LEN     32
#define USER_PWD_MAX_LEN      32
#define USER_NAME_MIN_LEN     5
#define USER_PWD_MIN_LEN      8

// discovery
#define SCOPE_MAX_NUM          8
#define SCOPE_ITEM_MAX_LEN    64
#define PROBE_TYPES_MAX_LEN   64

// network
#define HOSTNAME_MAX_LEN       253   // RFC 1123 section 2.1
#define HOSTNAMECONFIGFILE      "/etc/hostname"
#define DNS_MAX_COUNT            4
#define INTERFACE_NAME_MAX_LEN  10
#define SEARCH_DOMAIN_MAX_LEN   64
#define DNSCONFIGFILE           "/etc/resolv.conf"
#define DNSKEYWORD              "nameserver"
#define DNSSEARCH               "search"
#define PROTOCOL_COUNT          3

// event
#define EVENT_FILTER_TYPE_TOPIC       0
#define EVENT_FILTER_TYPE_MESSAGE     1
#define EVENT_MAX_NOTIFY_PRODUCERS    6
#define EVENT_MAX_PULL_POINTS         6
#define EVENT_NOTIFY_START_IDX        0
#define EVENT_NOTIFY_END_IDX          EVENT_MAX_NOTIFY_PRODUCERS-1
#define EVENT_PULLPOINT_START_IDX     EVENT_MAX_NOTIFY_PRODUCERS
#define EVENT_PULLPOINT_END_IDX       EVENT_MAX_NOTIFY_PRODUCERS+EVENT_MAX_PULL_POINTS-1
#define EVENT_START_IDX               0
#define EVENT_END_IDX                 EVENT_MAX_NOTIFY_PRODUCERS+EVENT_MAX_PULL_POINTS-1
#define EVENT_MAX_NUM                 EVENT_MAX_NOTIFY_PRODUCERS+EVENT_MAX_PULL_POINTS
#define EVENT_DEFAULT_TIMEOUT         60
#define EVENT_MAX_MESSAGE_LIMIT       (3)//temp
#define EVENT_MAX_MESSAGE_TIMEOUT     (120)//Shall at least support a Timeout of one minute.

#define TOPIC_0_NAME  "tns1:Device/Trigger/DigitalInput"
#define TOPIC_0_SOURCE_NUM    1
#define TOPIC_0_SOURCE_0_NAME  "InputToken"
#define TOPIC_0_SOURCE_0_NAME_VALUE  "DI_Name1"
#define TOPIC_0_SOURCE_0_TYPE  "tt:ReferenceToken"
#define TOPIC_0_KEY_NUM     0
#define TOPIC_0_KEY_0_NAME  "Topic0KeyName"
#define TOPIC_0_KEY_0_TYPE  "Topic0KeyType"
#define TOPIC_0_DATA_NUM     1
#define TOPIC_0_DATA_0_NAME  "LogicalState"
#define TOPIC_0_DATA_0_TYPE  "xs:boolean"

#define TOPIC_1_NAME  "tns1:VideoSource/MotionAlarm"
#define TOPIC_1_SOURCE_NUM    1
#define TOPIC_1_SOURCE_0_NAME  "Source"
#define TOPIC_1_SOURCE_0_NAME_VALUE  "MD_Name1"
#define TOPIC_1_SOURCE_0_TYPE  "tt:ReferenceToken"
#define TOPIC_1_KEY_NUM     0
#define TOPIC_1_KEY_0_NAME  "Topic0KeyName"
#define TOPIC_1_KEY_0_TYPE  "Topic0KeyType"
#define TOPIC_1_DATA_NUM     1
#define TOPIC_1_DATA_0_NAME  "State"
#define TOPIC_1_DATA_0_TYPE  "xs:boolean"

#define EVENT_TOPIC_MOTION 1
#define EVENT_TOPIC_ALARM_INPUT 2


// media service
#define MEDIA_VIDEO_SOURCE_CONFIG_MAX_NUM    2
#define MEDIA_VIDEO_SOURCE_MAX_NUM           1
#define MEDIA_VIDEO_ENCODER_CONFIG_MAX_NUM   16

#define MEDIA_AUDIO_SOURCE_CONFIG_MAX_NUM    1
#define MEDIA_AUDIO_SOURCE_MAX_NUM           1
#define MEDIA_AUDIO_ENCODER_CONFIG_OPTIONS_MAX_NUM   2
#define MEDIA_AUDIO_ENCODER_CONFIG_MAX_NUM   2
#define MEDIA_AUDIO_BITRATE_MAX_NUM          2
#define MEDIA_AUDIO_SAMPLERATE_MAX_NUM       2
#define MEDIA_AUDIO_OUTPUT_CONFIG_MAX_NUM    1
#define MEDIA_AUDIO_OUTPUT_MAX_NUM           1


#define MEDIA_METADATA_CONFIG_MAX_NUM        1
#define MEDIA_PROFILE_MAX_NUM                3
#define MEDIA_NAME_MAX_LEN                  64
#define MEDIA_TOKEN_MAX_LEN                 64

#define VIDEO_SOURCE_TOKEN_AVAILABLE_NUM     1
#define AUDIO_SOURCE_TOKEN_AVAILABLE_NUM     1
#define AUDIO_OUTPUT_TOKEN_AVAILABLE_NUM     1

#define JPEG_RESOLUTION_MAX_NUM       5
#define MPEG4_RESOLUTION_MAX_NUM      1
#define H264_RESOLUTION_MAX_NUM       5

#define OSD_USER_TEXT_MAX_NUM         16
#define OSD_MAX_CHANNEL               (MEDIA_PROFILE_MAX_NUM + 1) //+HDMI
#define OSD_HDMI_CHANNEL              (OSD_MAX_CHANNEL - 1) //+HDMI

// ptz service
#define PTZ_TOKEN_MAX_LEN                   64
#define MEDIA_PTZ_CONFIG_MAX_NUM            1
#define PTZ_PRESET_MAX_NUM                  3
#define PTZ_MAX_NUM                         1

// date time
#define FAKE_DST_STRING "M1.1.0,M1.1.1"

typedef struct
{
    int  isValid;
    char Username[USER_NAME_MAX_LEN+1]; /* required element of type xsd:string */
    char Password[USER_PWD_MAX_LEN+1];  /* optional element of type xsd:string */
    enum tt__UserLevel UserLevel;       /* required element of type tt:UserLevel */
}ONVIF_USER;

typedef struct
{
     enum tt__ScopeDefinition ScopeDef;       /* required element of type tt:ScopeDefinition */
     char ScopeItem[SCOPE_ITEM_MAX_LEN+1];    /* required element of type xsd:anyURI */
}ONVIF_SCOPE;


typedef struct
{
     enum  tt__DiscoveryMode DiscoveryMode; //{tt__DiscoveryMode__Discoverable
     ONVIF_SCOPE             scopeList[SCOPE_MAX_NUM];
     char                    ProbeTypes[PROBE_TYPES_MAX_LEN];
}ONVIF_DEVICE_DISCOVERY;


typedef struct
{
     char Manufacturer[33];
     char Model[33];
     char FirmwareVersion[11];
     char SerialNumber[17];
     char HardwareId[17];
}ONVIF_DEVICE_INFO;


typedef struct
{
     char name[INTERFACE_NAME_MAX_LEN+1];
     char IP[IP_LENGTH+1];
     char BroadIP[IP_LENGTH+1];
     unsigned char MAC[MAC_LENGTH+1];
     char Mask[IP_LENGTH+1];
     char GW[IP_LENGTH+1];
     enum xsd__boolean   DHCPServerEnable;
     enum xsd__boolean   fromDHCP;
     char linkLocal[IP_LENGTH+1];
}ONVIF_NETWORK_INTERFACE_INFO;


typedef struct
{
     int                            isSupport;
     enum tt__NetworkProtocolType   Type;
     int                            port;
     int                            Enable;
}ONVIF_NETWORK_PROTOCOL_INFO;


typedef struct
{
    enum xsd__boolean               rtspAuth;
}ONVIF_NETWORK_AUTH_INFO;


typedef struct
{
     int                            isfromDHCP;
     enum tt__NetworkHostType       NetworkHostType;
     char                           ServerAddress[32];
}ONVIF_NTP_INFO;


typedef struct
{
     char                          hostname[HOSTNAME_MAX_LEN+1];
     char                          DNS1[IP_LENGTH+1];
     char                          DNS2[IP_LENGTH+1];
     int                           DNSfromDHCP;
     ONVIF_NETWORK_INTERFACE_INFO  interfaceInfo;
     ONVIF_NETWORK_PROTOCOL_INFO   protocolInfo[PROTOCOL_COUNT];
     ONVIF_NTP_INFO                NTPInfo;
     ONVIF_NETWORK_AUTH_INFO       authInfo;
}ONVIF_NETWORK_INFO;


typedef struct
{
     int                           subscribed;
     int                           triggered;
     int                           initTimeout;
     time_t                        expiretime;
     char                          topicFilter[INFO_LENGTH+1];
     char                          messageFilter[INFO_LENGTH+1];
     char                          consumerEndPoint[INFO_LENGTH+1];
}ONVIF_EVENT_INFO;



typedef struct
{
    struct tt__VideoResolution        ResolutionsAvailable[JPEG_RESOLUTION_MAX_NUM];
    struct tt__IntRange               FrameRateRange;
    struct tt__IntRange               EncodingIntervalRange;
    struct tt__IntRange               BitRateRange;
}ONVIF_VIDEO_ENCODER_JPEG_OPTIONS;

typedef struct
{
    struct tt__VideoResolution        ResolutionsAvailable[MPEG4_RESOLUTION_MAX_NUM];
    struct tt__IntRange               GovLengthRange;
    struct tt__IntRange               FrameRateRange;
    struct tt__IntRange               EncodingIntervalRange;
    struct tt__IntRange               BitRateRange;

}ONVIF_VIDEO_ENCODER_MPEG4_OPTIONS;

typedef struct
{
    struct tt__VideoResolution        ResolutionsAvailable[H264_RESOLUTION_MAX_NUM];
    struct tt__IntRange               GovLengthRange;
    struct tt__IntRange               FrameRateRange;
    struct tt__IntRange               EncodingIntervalRange;
    struct tt__IntRange               BitRateRange;
}ONVIF_VIDEO_ENCODER_H264_OPTIONS;

typedef struct
{
    ONVIF_VIDEO_ENCODER_JPEG_OPTIONS   jpegOptions;
    ONVIF_VIDEO_ENCODER_MPEG4_OPTIONS  mpeg4Options;
    ONVIF_VIDEO_ENCODER_H264_OPTIONS   h264Options;
}ONVIF_VIDEO_ENCODER_CONFIG_OPTIONS;

typedef struct
{
    char                               Address[IP_LENGTH+1];
    int                                Port;
    int                                TTL;
    enum xsd__boolean                  AutoStart;
}ONVIF_MEDIA_MULTICAST_CONFIG;

typedef struct
{
    char                                 Name[MEDIA_NAME_MAX_LEN+1];
    int                                  UseCount;
    char                                 token[MEDIA_TOKEN_MAX_LEN+1];
    enum   tt__VideoEncoding             Encoding;
    struct tt__VideoResolution           Resolution;
    struct tt__VideoResolution           MaxResolution;
    float                                Quality;
    struct tt__VideoRateControl          RateControl;
    struct tt__VideoCropControl          CropControl;
    struct tt__Mpeg4Configuration        MPEG4;
    struct tt__H264Configuration         H264;
    ONVIF_MEDIA_MULTICAST_CONFIG         Multicast;
    LONG64                               SessionTimeout;
}ONVIF_VIDEO_ENCODER_CONFIG;

typedef struct
{
    int                               TotalNumber;
    int                               JPEG;
    int                               H264;
    int                               MPEG4;
}ONVIF_VIDEO_ENCODE_INSTANCE_NUM;


typedef struct
{
    struct tt__IntRange               XRange;
    struct tt__IntRange               YRange;
    struct tt__IntRange               WidthRange;
    struct tt__IntRange               HeightRange;
    char                              VideoSourceTokensAvailable[VIDEO_SOURCE_TOKEN_AVAILABLE_NUM][MEDIA_TOKEN_MAX_LEN+1];
}ONVIF_VIDEO_SOURCE_CONFIG_OPTIONS;



typedef struct
{
    char                              Name[MEDIA_NAME_MAX_LEN+1];
    int                               UseCount;
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    char                              SourceToken[MEDIA_TOKEN_MAX_LEN+1];
    struct tt__IntRectangle           Bounds;
    ONVIF_VIDEO_ENCODE_INSTANCE_NUM   VideoEncodeInstanceNum;
}ONVIF_VIDEO_SOURCE_CONFIG;


typedef struct
{
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    float                             Framerate;
    struct tt__VideoResolution        Resolution;
}ONVIF_VIDEO_SOURCE;

/* tt:CVBSMode */
enum tt__CVBSMode {tt__CVBSMode__OFF = 0, tt__CVBSMode__NTSC = 1, tt__CVBSMode__PAL = 2};

typedef struct
{
    enum tt__CVBSMode CVBSMode;
}CVBS_OUTPUT;

typedef struct
{
    CVBS_OUTPUT CVBSOutput;
}ONVIF_VIDEO_OUTPUT;

typedef struct
{
    char                              InputTokensAvailable[AUDIO_SOURCE_TOKEN_AVAILABLE_NUM][MEDIA_TOKEN_MAX_LEN+1];
}ONVIF_AUDIO_SOURCE_CONFIG_OPTIONS;


typedef struct
{
    char                              Name[MEDIA_NAME_MAX_LEN+1];
    int                               UseCount;
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    char                              SourceToken[MEDIA_TOKEN_MAX_LEN+1];
    int                               Volume;
    enum tt__AudioDulpexMode          DulpexMode;
}ONVIF_AUDIO_SOURCE_CONFIG;


typedef struct
{
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    int                               Channels;
}ONVIF_AUDIO_SOURCE;


typedef struct
{
    enum tt__AudioEncoding            Encoding;
    int                               SizeofBitrate;
    int                               BitrateList[MEDIA_AUDIO_BITRATE_MAX_NUM];        // kbps
    int                               SizeofSampleRate;
    int                               SampleRateList[MEDIA_AUDIO_SAMPLERATE_MAX_NUM];  // kHz
}ONVIF_AUDIO_ENCODER_CONFIG_OPTIONS;

typedef struct
{
    char                              Name[MEDIA_NAME_MAX_LEN+1];
    int                               UseCount;
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    enum tt__AudioEncoding            Encoding;
    int                               Bitrate;    // kbps
    float                             SampleRate; // kHz
    ONVIF_MEDIA_MULTICAST_CONFIG      Multicast;
    LONG64                            SessionTimeout;
}ONVIF_AUDIO_ENCODER_CONFIG;


typedef struct
{
    char                              TokensAvailable[AUDIO_OUTPUT_TOKEN_AVAILABLE_NUM][MEDIA_TOKEN_MAX_LEN+1];
    enum tt__SendPrimacy              SendPrimacy;
	struct tt__IntRange               outputlevelrange;// Range of values greater equal Min value and less equal Max value.
}ONVIF_AUDIO_OUTPUT_CONFIG_OPTIONS;


typedef struct
{
    char                              Name[MEDIA_NAME_MAX_LEN+1];
    int                               UseCount;
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    char                              OutputToken[MEDIA_TOKEN_MAX_LEN+1];
    enum tt__SendPrimacy              SendPrimacy;
    enum tt__AudioOutputTarget        OutputTarget;
    int                               OutputLevel;
}ONVIF_AUDIO_OUTPUT_CONFIG;


typedef struct
{
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
}ONVIF_AUDIO_OUTPUT;

typedef struct
{
    char                              Name[MEDIA_NAME_MAX_LEN+1];
    int                               UseCount;
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    struct tt__PTZFilter              PTZStatus;
    struct tt__EventSubscription      Events;
    ONVIF_MEDIA_MULTICAST_CONFIG      Multicast;
    LONG64                            SessionTimeout;
}ONVIF_METADATA_CONFIG;

typedef struct
{
    char                              Name[MEDIA_NAME_MAX_LEN+1];
    char                              token[MEDIA_TOKEN_MAX_LEN+1];
    enum xsd__boolean                 fixed;
    char                              VideoEncoderConfigToken[MEDIA_TOKEN_MAX_LEN+1];
    char                              VideoSourceConfigToken[MEDIA_TOKEN_MAX_LEN+1];
    char                              AudioEncoderConfigToken[MEDIA_TOKEN_MAX_LEN+1];
    char                              AudioSourceConfigToken[MEDIA_TOKEN_MAX_LEN+1];
    char                              AudioOutputConfigToken[MEDIA_TOKEN_MAX_LEN+1];
    char                              MetadataConfigToken[MEDIA_TOKEN_MAX_LEN+1];
    char                              PtzConfigToken[MEDIA_TOKEN_MAX_LEN+1];
    struct tt__VideoResolution        MaxResolution;
}ONVIF_MEDIA_PROFILE;

typedef struct
{
    unsigned long                     UserText[OSD_USER_TEXT_MAX_NUM+1];
    enum tt__OsdUserTextPos           OsdUserTextPos;
    int                               FontSize;
    int                               DateTime_On;
    int                               UserText_On;
    unsigned int                      FontColor;
    enum tt__OsdDateTimeFmt           OsdDateTimeFmt;
    int                               ImgWidth;
    int                               ImgHeight;
}OSD_CONFIG;

typedef struct
{
    char                                     rtspServerIP[IP_LENGTH+1];
    char                                     rtspMulticastIP[IP_LENGTH+1];
}
ONVIF_STREAM_INFO;


//////////////////////////////////////////////////////////////
//
// PTZ PART
//
/////////////////////////////////////////////////////////////
/* tt:PTZSpace */
enum tt__PTZSpace
{
	tt__NSS = 0, //not support space
	tt__PT_PositionGenericSpace = 1,
	tt__PT_TranslationGenericSpace = 2,
	tt__PT_SphericalTranslationSpaceDegrees = 3,
	tt__PT_TranslationSpaceFov = 4,
	tt__PT_VelocityGenericSpace = 5,
	tt__PT_GenericSpeedSpace = 6,
	tt__PT_SphericalPositionSpace = 7,
	tt__Z_PositionGenericSpace = 8,
	tt__Z_TranslationGenericSpace = 9,
	tt__Z_VelocityGenericSpace = 10,
	tt__Z_ZoomGenericSpeedSpace = 11
};

enum tt__PTZ_TOUR_DIRECTION
{
	tt__PTZ_TOUR_DIRECTION_UNUSED = 0,
	tt__PTZ_TOUR_DIRECTION_FORWARD,
	tt__PTZ_TOUR_DIRECTION_BACKWARD,
	tt__PTZ_TOUR_DIRECTION_EXTENEDED
}
;

enum tt__PTZ_ELIIP
{
	tt__PTZ_ELIIP_UNUSED = 0,
	tt__PTZ_ELIIP_OFF,
	tt__PTZ_ELIIP_ON,
	tt__PTZ_ELIIP_EXTENEDED,
}
;

enum tt__PTZ_REVERSE
{
	tt__PTZ_REVERSE_UNUSED = 0,
	tt__PTZ_REVERSE_OFF,
	tt__PTZ_REVERSE_ON,
	tt__PTZ_REVERSE_AUTO,
	tt__PTZ_REVERSE_EXTENEDED,
}
;

enum tt__PTZ_RS485_PROTOCOL
{
    tt__PTZ_RS485_PROTOCOL_PELCO_D,
    tt__PTZ_RS485_PROTOCOL_PELCO_P,
    tt__PTZ_RS485_PROTOCOL_PELCO_MAX,
}
;

enum tt__RS485_BAUDRATE
{
    tt__RS485_BAUDRATE_110 = 110,
    tt__RS485_BAUDRATE_300 = 300,
    tt__RS485_BAUDRATE_600 = 600,
    tt__RS485_BAUDRATE_1200 = 1200,
    tt__RS485_BAUDRATE_2400 = 2400,
    tt__RS485_BAUDRATE_4800 = 4800,
    tt__RS485_BAUDRATE_9600 = 9600,
    tt__RS485_BAUDRATE_19200 = 19200,
    tt__RS485_BAUDRATE_38400 = 38400,
    tt__RS485_BAUDRATE_57600 = 57600,
    tt__RS485_BAUDRATE_115200 = 115200,
    tt__RS485_BAUDRATE_MAX,
}
;

enum tt__RS485_DATA_BIT
{
    tt__RS485_DATA_BIT_5 = 5,
    tt__RS485_DATA_BIT_6 = 6,
    tt__RS485_DATA_BIT_7 = 7,
    tt__RS485_DATA_BIT_8 = 8,
    tt__RS485_DATA_BIT_MAX,
}
;

enum tt__RS485_PARITY_BIT
{
    tt__RS485_PARITY_BIT_NONE,
    tt__RS485_PARITY_BIT_ODD,
    tt__RS485_PARITY_BIT_EVEN,
    tt__RS485_PARITY_BIT_SPACE,
    tt__RS485_PARITY_BIT_MAX,
}
;

enum tt__RS485_STOP_BIT
{
    tt__RS485_STOP_BIT_1 = 1,
    tt__RS485_STOP_BIT_2 = 2,
    tt__RS485_DATABIT_MAX,
}
;

/* tt:FloatRange */
typedef struct
{
    int   bUsed;
    float Min;    /* required element of type xsd:float */
    float Max;    /* required element of type xsd:float */
}
tt__FloatRange;

typedef struct _ONVIF_VECTOR_2D
{
	enum tt__PTZSpace Space;
	float x_float;
	float y_float;
}
ONVIF_VECTOR_2D;

typedef struct _ONVIF_VECTOR_1D
{
	enum tt__PTZSpace Space;
	float x_float;
}
ONVIF_VECTOR_1D;

typedef struct
{
	ONVIF_VECTOR_2D PT_pVector;
	ONVIF_VECTOR_1D Z_pVector;
}
ONVIF_PTZ_SPEED;

typedef struct
{
	enum tt__PTZSpace anySpace;			// A URI of coordinate systems, [1~1]
	tt__FloatRange xrange;		// A range of x-axis, [1~1]
	tt__FloatRange yrange;		// A range of y-axis, [1~1]
}
ONVIF_SPACE2D_DESC;

typedef struct
{
	enum tt__PTZSpace anySpace;			// A URI of coordinate systems, [1~1]
	tt__FloatRange xrange;		// A range of x-axis, [1~1]
}
ONVIF_SPACE1D_DESC;

typedef struct
{
	ONVIF_SPACE2D_DESC range;	// A range of pan tilt limits, [1~1]
}
ONVIF_PT_LIMIT;

typedef struct
{
	ONVIF_SPACE1D_DESC range;	// A range of zoom limit, [1~1]
}
ONVIF_Z_LIMIT;

typedef struct
{
	char min[10];					// Range of duration greater equal Min duration and less equal Max duration, [1~1]
	char max[10];					// Range of duration greater equal Min duration and less equal Max duration, [1~1]
}
ONVIF_PTZ_DURATION_RANGE;

#define PTZ_COUNT   16
typedef struct
{
	enum tt__PTZ_ELIIP EflipMode; // config just only index = 0
	enum tt__PTZ_REVERSE ReverseMode; // config just only index = 0
}
ONVIF_PTZ_PTCTRL_DIRECTION;

typedef struct
{
	ONVIF_PTZ_PTCTRL_DIRECTION PTDirect;
}
ONVIF_PTZ_EXTENSION;

typedef struct
{
	ONVIF_SPACE2D_DESC BoundAbsolutePantTiltPositionSpace;
	ONVIF_SPACE1D_DESC BoundAbsoluteZoomPositionSpace;
	ONVIF_SPACE2D_DESC BoundRelativePanTiltTranslationSpace;
	ONVIF_SPACE1D_DESC BoundRelativeZoomTranslationSpace;
	ONVIF_SPACE2D_DESC BoundContinuousPanTiltVelocitySpace;
	ONVIF_SPACE1D_DESC BoundContinuousZoomVelocitySpace;
	ONVIF_SPACE1D_DESC BoundPTSpeed;
	ONVIF_SPACE1D_DESC BoundZSpeed;
}
ONVIF_PTZ_SPACE_BOUND;

typedef struct
{
    ONVIF_PTZ_SPACE_BOUND           PTZSpaceBound;
#if 0
    ONVIF_PTZ_DURATION_RANGE        PTZTimeout;
    ONVIF_PTZ_PTCTRL_DIRECTION      PTDirect;
#endif
}
ONVIF_PTZ_CONFIG_OPTIONS;

typedef struct
{
    unsigned int                    ttyNumber;
    unsigned int                    deviceID;
    enum tt__PTZ_RS485_PROTOCOL     protocol;
    enum tt__RS485_BAUDRATE         baudrate;
    enum tt__RS485_DATA_BIT         dataBit;
    enum tt__RS485_STOP_BIT         stopBit;
    enum tt__RS485_PARITY_BIT       parityBit;
}
ONVIF_RS485_CONFIG;

typedef struct
{
    char                            nodeToken[PTZ_TOKEN_MAX_LEN+1];
    char                            token[PTZ_TOKEN_MAX_LEN+1];
    char                            Name[PTZ_TOKEN_MAX_LEN+1];
    int                             UseCount;
    enum   tt__PTZSpace             DefaultAbsolutePantTiltPositionSpace; //if supports absolute Pan/Tilt movements
    enum   tt__PTZSpace             DefaultAbsoluteZoomPositionSpace; //if supports absolute zoom movements
    enum   tt__PTZSpace             DefaultRelativePanTiltTranslationSpace; //if supports relative Pan/Tilt movements
    enum   tt__PTZSpace             DefaultRelativeZoomTranslationSpace; //if supports relative zoom movements
    enum   tt__PTZSpace             DefaultContinuousPanTiltVelocitySpace; //if supports continuous Pan/Tilt movements
    enum   tt__PTZSpace             DefaultContinuousZoomVelocitySpace; //if supports continuous zoom movements
    ONVIF_PTZ_SPEED                 DefaultPTZSpeed; //if supports absolute or relative PTZ movements
    char                            DefaultPTZTimeout[10]; //if supports continuous movements
    ONVIF_PT_LIMIT                  PanTiltLimits;//if supports an absolute Pan/Tilt
    ONVIF_Z_LIMIT                   ZoomLimits; //if supports absolute zoom
    ONVIF_RS485_CONFIG              RS485Config;
#if 0
	ONVIF_PTZ_EXTENSION             Extension;
#endif
	int                             isForcePersistence;
}
ONVIF_PTZ_CONFIG;

typedef struct
{
	ONVIF_VECTOR_2D PT_pVector;
	ONVIF_VECTOR_1D Z_pVector;
}
ONVIF_PTZ_POSITION;

typedef struct
{
	char                            token[PTZ_TOKEN_MAX_LEN+1];
	char                            Name[PTZ_TOKEN_MAX_LEN+1];
    ONVIF_PTZ_POSITION              pos;
}
ONVIF_PTZ_PRESET;

typedef struct
{
    ONVIF_PTZ_SPEED                 speed;
    ONVIF_PTZ_PRESET                preset[PTZ_PRESET_MAX_NUM];
}
ONVIF_PTZ_INFO;

//////////////////////////////////////////////////////////////
//
// IMAGE PART
//
/////////////////////////////////////////////////////////////
#define IMAGE_SETTING_UPDATE_FLAG_ALL                   0xffffffff
#define IMAGE_SETTING_UPDATE_FLAG_BACKLIGHTCOMPENSATION 0x00000001
#define IMAGE_SETTING_UPDATE_FLAG_BRIGHTNESS            0x00000002
#define IMAGE_SETTING_UPDATE_FLAG_COLORSATURATION       0x00000004
#define IMAGE_SETTING_UPDATE_FLAG_CONTRAST              0x00000008
#define IMAGE_SETTING_UPDATE_FLAG_EXPOSURE              0x00000010
#define IMAGE_SETTING_UPDATE_FLAG_EXPOSURE_WINDOW       0x00000020
#define IMAGE_SETTING_UPDATE_FLAG_FOCUS                 0x00000040
#define IMAGE_SETTING_UPDATE_FLAG_IRCUT                 0x00000080
#define IMAGE_SETTING_UPDATE_FLAG_WDR                   0x00000100
#define IMAGE_SETTING_UPDATE_FLAG_WHITEBALANCE          0x00000200
#define IMAGE_SETTING_UPDATE_FLAG_IMAGESTABILIZATION    0x00000400
#define IMAGE_SETTING_UPDATE_FLAG_SHARPNESS             0x00000800

#define IMAGE_FOCUSMOVE_EXE_FLAG_ABSOLUTE               0x00000001
#define IMAGE_FOCUSMOVE_EXE_FLAG_RELATIVE               0x00000002
#define IMAGE_FOCUSMOVE_EXE_FLAG_CONTINUOUS             0x00000004
#define IMAGE_FOCUSMOVE_EXE_FLAG_STOP                   0x00000008


#define IMAGE_CHECK_RANGE_OK            0
#define IMAGE_CHECK_RANGE_NOTSUPPORT   -1
#define IMAGE_CHECK_RANGE_OUTRANGE     -2

#define IMAGE_SIZE_MAX_NUM 2

typedef enum
{
    ONVIF_IMAGE_EXPOSUREMODE_AUTO = 0,
    ONVIF_IMAGE_EXPOSUREMODE_MANUAL = 1,
    ONVIF_IMAGE_EXPOSUREMODE_MAXNUM
} ONVIF_IMAGE_EXPOSUREMODE;

typedef enum
{
    ONVIF_IMAGE_EXPOSUREPRIORITY_LowNoise = 0,
    ONVIF_IMAGE_EXPOSUREPRIORITY_FrameRate = 1,
    ONVIF_IMAGE_EXPOSUREPRIORITY_MAXNUM
} ONVIF_IMAGE_EXPOSUREPRIORITY;

typedef enum
{
    ONVIF_IMAGE_EXPOSURESTRATEGY_LOWLIGHT = 0,
    ONVIF_IMAGE_EXPOSURESTRATEGY_HIGHLIGHT = 1,
    ONVIF_IMAGE_EXPOSURESTRATEGY_MAXNUM
} ONVIF_IMAGE_EXPOSURESTRATEGY;

typedef enum
{
    ONVIF_IMAGE_EXPOSUREANTIFLICKER_50HZ = 0,
    ONVIF_IMAGE_EXPOSUREANTIFLICKER_60HZ = 1,
    ONVIF_IMAGE_EXPOSUREANTIFLICKER_AUTO = 2,
    ONVIF_IMAGE_EXPOSUREANTIFLICKER_NONE = 3,
    ONVIF_IMAGE_EXPOSUREANTIFLICKER_MAXNUM
} ONVIF_IMAGE_EXPOSUREANTIFLICKER;

typedef enum
{
    ONVIF_IMAGE_AUTOFOCUSMODE_AUTO = 0,
    ONVIF_IMAGE_AUTOFOCUSMODE_MANUAL = 1,
    ONVIF_IMAGE_AUTOFOCUSMODE_MAXNUM
} ONVIF_IMAGE_AUTOFOCUSMODE;

typedef enum
{
    ONVIF_IMAGE_STABILIZATION_MODE_OFF = 0,
    ONVIF_IMAGE_STABILIZATION_MODE_ON = 1,
    ONVIF_IMAGE_STABILIZATION_MODE_AUTO,
    ONVIF_IMAGE_STABILIZATION_MODE_Extended,
    ONVIF_IMAGE_STABILIZATION_MODE_MAXNUM
} ONVIF_IMAGE_STABILIZATION_MODE;

typedef enum
{
    ONVIF_IMAGE_IRCUTFILTERMODE_OFF = 0,
    ONVIF_IMAGE_IRCUTFILTERMODE_ON = 1,
    ONVIF_IMAGE_IRCUTFILTERMODE_AUTO,
    ONVIF_IMAGE_IRCUTFILTERMODE_MAXNUM
} ONVIF_IMAGE_IRCUTFILTERMODE;

typedef enum
{
    ONVIF_IMAGE_IRCUTAUTOSUBMODE_IMAGESENSOR,
    ONVIF_IMAGE_IRCUTAUTOSUBMODE_SCHEDULE,
    ONVIF_IMAGE_IRCUTAUTOSUBMODE_DI,
    ONVIF_IMAGE_IRCUTAUTOSUBMODE_MAXNUM,
} ONVIF_IMAGE_IRCUTAUTOSUBMODE;

typedef struct
{
    ONVIF_IMAGE_IRCUTFILTERMODE ircutfiltermodes_int;
    ONVIF_IMAGE_IRCUTAUTOSUBMODE ircutautosubmodes_int;
    unsigned int daytonighttimehour_uint;
    unsigned int daytonighttimeminute_uint;
    unsigned int nighttodaytimehour_uint;
    unsigned int nighttodaytimeminute_uint;
} ONVIF_IMAGE_IRCUT_INFO;

typedef enum
{
    ONVIF_IMAGE_WHITEBALANCE_AUTO = 0,
    ONVIF_IMAGE_WHITEBALANCE_MANUAL = 1,
    ONVIF_IMAGE_WHITEBALANCE_MAXNUM
} ONVIF_IMAGE_WHITEBALANCE;

typedef struct
{
    enum xsd__boolean  backlightcompensation_int;
    float lv_float;
} ONVIF_IMAGE_SETTING_BACKLIGHTCOMPENSATION_INFO;

typedef struct
{
    float bottom_float;
    float top_float;
    float right_float;
    float left_float;
} ONVIF_IMAGE_EXPOSURE_WINRECT;


typedef struct
{
    ONVIF_IMAGE_EXPOSUREMODE mode_int;
    ONVIF_IMAGE_EXPOSUREPRIORITY priority_int;
    ONVIF_IMAGE_EXPOSURESTRATEGY strategy_int;
    ONVIF_IMAGE_EXPOSUREANTIFLICKER antiflicker_int;
    ONVIF_IMAGE_EXPOSURE_WINRECT window_struct;
    float ev_float;
    float convergencespeed_float;
    float tolerance_float;
    float delay_float;
    float minexposuretime_float;
    float maxexposuretime_float;
    float mingain_float ;
    float maxgain_float ;
    float miniris_float ;
    float maxiris_float ;
    float exposuretime_float;
    float gain_float;
    float iris_float;
} ONVIF_IMAGE_SETTING_EXPOSURE_INFO;

typedef struct
{
    ONVIF_IMAGE_AUTOFOCUSMODE autofocus_int;
    float defaultspeed_float;
    float nearlimit_float;
    float farlimit_float;
} ONVIF_IMAGE_SETTING_FOCUSCONFIGURATION_INFO;

typedef struct
{
    enum xsd__boolean  widedynamicmode_int;
    float lv_float;
} ONVIF_IMAGE_SETTING_WIDEDYNAMICRANGE_INFO;

typedef struct
{
    ONVIF_IMAGE_WHITEBALANCE mode_int;
    float convergencespeed_float;
    float convergencestep_float;
    float crgain_float;
    float cbgain_float;
} ONVIF_IMAGE_SETTING_WHITEBALANCE_INFO;

typedef struct
{
    float nr2Dstrength_float;
    float nr3Dstrength_float;
} ONVIF_IMAGE_SETTING_NOISEREDUCTION_INFO;

typedef struct
{
    ONVIF_IMAGE_STABILIZATION_MODE mode_int;
    float level_float;
} ONVIF_IMAGE_SETTING_IMAGESTABILIZATION_INFO;

typedef struct
{
    char boundarytype_achar[MEDIA_TOKEN_MAX_LEN+1];
    float boundaryoffset_float;
    char responsetime_achar[MEDIA_TOKEN_MAX_LEN+1];
} ONVIF_IMAGE_SETTING_EXT202_INFO;

typedef struct
{
    ONVIF_IMAGE_SETTING_IMAGESTABILIZATION_INFO ImageStabilization_struct;
    ONVIF_IMAGE_SETTING_EXT202_INFO extension_struct;

} ONVIF_IMAGE_SETTING_EXT20_INFO;

typedef enum {
    NAR_9_16,
    NAR_2_3,
    NAR_3_4,
    NAR_1_1,
    NAR_4_3,
    NAR_3_2,
    NAR_16_9,
} NVT_ASPECT_RATIO;

typedef struct
{
    ONVIF_IMAGE_SETTING_BACKLIGHTCOMPENSATION_INFO BacklightCompensation_struct;
    float brightness_float;
    float colorsaturation_float;
    float contrast_float;
    float sharpness_float;
    float hue_float;
    enum xsd__boolean  mirror_bool;
    enum xsd__boolean  flip_bool;
    enum tt__ColorMode color_int;
    ONVIF_IMAGE_SETTING_EXPOSURE_INFO  exposure_struct;
    ONVIF_IMAGE_SETTING_FOCUSCONFIGURATION_INFO focus_struct;
    ONVIF_IMAGE_IRCUT_INFO ircutfilter_struct;
    ONVIF_IMAGE_SETTING_WIDEDYNAMICRANGE_INFO   widedynamicRange_struct;
    ONVIF_IMAGE_SETTING_WIDEDYNAMICRANGE_INFO   highdynamicRange_struct;
    ONVIF_IMAGE_SETTING_WHITEBALANCE_INFO    whitebalance_struct;
    ONVIF_IMAGE_SETTING_NOISEREDUCTION_INFO noisereduction_struct;
    enum xsd__boolean  forcepersistence_bool;
    ONVIF_IMAGE_SETTING_EXT20_INFO extension_struct;
    NVT_ASPECT_RATIO aspect_ratio;
}ONVIF_IMAGE_SETTING_INFO;

typedef enum
{
    ONVIF_MOVESTATUS_IDLE,
    ONVIF_MOVESTATUS_MOVING,
    ONVIF_MOVESTATUS_UNKNOWN
} ONVIF_MOVESTATUS;

typedef struct
{
    float position_float;
    ONVIF_MOVESTATUS movestatus_int;
    char error_pchar[64];
} ONVIF_IMAGE_FOCUS_STATUS_INFO;

typedef struct
{
    char toke_achar[MEDIA_TOKEN_MAX_LEN+1];
    ONVIF_IMAGE_FOCUS_STATUS_INFO  FocusStatus;
}ONVIF_IMAGE_STATUSDATA_INFO;

typedef struct
{
    enum xsd__boolean  used_int;
    float min_float;
    float max_float;
} ONVIF_FLOAT_RANGE;

typedef struct
{
   ONVIF_FLOAT_RANGE position_struct;
   ONVIF_FLOAT_RANGE speed_struct;
} ONVIF_IMAGE_MOVEOPTIONS_ABSOLUTE;

typedef struct
{
   ONVIF_FLOAT_RANGE distance_struct;
   ONVIF_FLOAT_RANGE speed_struct;
}  ONVIF_IMAGE_MOVEOPTIONS_RELATIVE;

typedef struct
{
   ONVIF_FLOAT_RANGE speed_struct;
} ONVIF_IMAGE_MOVEOPTIONS_CONTINUOUS;

typedef struct
{
   ONVIF_IMAGE_MOVEOPTIONS_ABSOLUTE   absolute_struct;
   ONVIF_IMAGE_MOVEOPTIONS_RELATIVE   relative_struct;
   ONVIF_IMAGE_MOVEOPTIONS_CONTINUOUS continuous_struct;
}  ONVIF_IMAGE_MOVEOPTIONS20_INFO;

typedef struct
{
    int sizeofbacklightcompensation_int;
    enum xsd__boolean  backlightcompensation_aint[IMAGE_SIZE_MAX_NUM];
    ONVIF_FLOAT_RANGE lv_struct;
} ONVIF_IMAGE_OPTIONS_BACKLIGHTCOMPENSATION;

typedef struct
{
    ONVIF_IMAGE_EXPOSUREMODE mode_aint[ONVIF_IMAGE_EXPOSUREMODE_MAXNUM];
    ONVIF_IMAGE_EXPOSUREPRIORITY priority_aint[ONVIF_IMAGE_EXPOSUREPRIORITY_MAXNUM];
    ONVIF_FLOAT_RANGE maxexposuretime_struct;
    ONVIF_FLOAT_RANGE minexposuretime_struct;
    ONVIF_FLOAT_RANGE maxgain_struct;
    ONVIF_FLOAT_RANGE mingain_struct;
    ONVIF_FLOAT_RANGE maxiris_struct;
    ONVIF_FLOAT_RANGE miniris_struct;
    ONVIF_FLOAT_RANGE exposuretime_struct;
    ONVIF_FLOAT_RANGE gain_struct;
    ONVIF_FLOAT_RANGE iris_struct;
} ONVIF_IMAGE_OPTIONS_EXPOSURE;

typedef struct
{
    ONVIF_IMAGE_AUTOFOCUSMODE autofocus_aint[ONVIF_IMAGE_AUTOFOCUSMODE_MAXNUM];
    ONVIF_FLOAT_RANGE defaultspeed_struct;
    ONVIF_FLOAT_RANGE nearlimit_struct;
    ONVIF_FLOAT_RANGE farlimit_struct;
} ONVIF_IMAGE_OPTIONS_FOCUS;

typedef struct
{
    int sizeofwidedynamicmode_int;
    enum xsd__boolean  widedynamicmode_aint[IMAGE_SIZE_MAX_NUM];
    ONVIF_FLOAT_RANGE lv_struct;
} ONVIF_IMAGE_OPTIONS_WIDEDYNAMICRANGE;


typedef struct
{
    ONVIF_IMAGE_WHITEBALANCE mode_aint[ONVIF_IMAGE_WHITEBALANCE_MAXNUM];
    ONVIF_FLOAT_RANGE yrgain_struct;
    ONVIF_FLOAT_RANGE ybgain_struct;
} ONVIF_IMAGE_OPTIONS_WHITEBALANCE;

typedef struct
{
    ONVIF_IMAGE_STABILIZATION_MODE mode_aint[ONVIF_IMAGE_STABILIZATION_MODE_MAXNUM];
    ONVIF_FLOAT_RANGE lv_struct;
} ONVIF_IMAGE_OPTIONS_STABILIZATION;

typedef struct
{
    ONVIF_IMAGE_OPTIONS_STABILIZATION imagestabilization_struct;
} ONVIF_IMAGE_OPTIONS_EXT;

typedef struct
{
    ONVIF_IMAGE_OPTIONS_BACKLIGHTCOMPENSATION backlight_struct;
    ONVIF_FLOAT_RANGE brightness_struct;
    ONVIF_FLOAT_RANGE colorsaturation_struct;
    ONVIF_FLOAT_RANGE contrast_struct;
    ONVIF_IMAGE_OPTIONS_EXPOSURE  exposure_struct;
    ONVIF_IMAGE_OPTIONS_FOCUS   focus_struct;
    ONVIF_IMAGE_IRCUTFILTERMODE ircutfiltermodes_aint[ONVIF_IMAGE_IRCUTFILTERMODE_MAXNUM];
    ONVIF_FLOAT_RANGE sharpness_struct;
    ONVIF_IMAGE_OPTIONS_WIDEDYNAMICRANGE  widedynamicrange_struct;
    ONVIF_IMAGE_OPTIONS_WHITEBALANCE  whitebalance_struct;
    ONVIF_IMAGE_OPTIONS_EXT ext_sturct;
} ONVIF_IMAGE_OPTIONS_INFO ;

typedef struct
{
    char token[64];
    ONVIF_IMAGE_OPTIONS_INFO opts_struct;
} ONVIF_IMAGE_OPTIONSDATA_INFO ;

typedef struct
{
   char token[64];
   ONVIF_IMAGE_MOVEOPTIONS20_INFO focus_struct;
} ONVIF_IMAGE_MOVEOPTIONS20DATA_INFO ;

typedef struct
{
   char token[64];
   ONVIF_IMAGE_SETTING_INFO set_struct;
} ONVIF_IMAGE_SETTINGDATA_INFO ;



#if 0
typedef enum
{
    OnvifFocusMove_CmdType_Stop = 0,
    OnvifFocusMove_CmdType_Absolute,
    OnvifFocusMove_CmdType_Relative,
    OnvifFocusMove_CmdType_Continuous
} ONVIF_FOCUSMOVE_CMDTYPE;
#endif

typedef struct
{
    float position_float;
    float speed_float;
} ONVIF_FOCUSMOVE_ABSOLUTE_INFO;

typedef struct
{
    float distance_float;
    float speed_float;
} ONVIF_FOCUSMOVE_RELATIVE_INFO;

typedef struct
{
    float speed_float;
} ONVIF_FOCUSMOVE_CONTINUOUS_INFO;

typedef struct
{
    ONVIF_FOCUSMOVE_ABSOLUTE_INFO   absolute_struct;        // Parameters for the absolute focus control, [0~1]
    ONVIF_FOCUSMOVE_RELATIVE_INFO   relative_struct;        // Parameters for the relative focus control, [0~1]
    ONVIF_FOCUSMOVE_CONTINUOUS_INFO continuous_struct;    // Parameter for the continuous focus control, [0~1]
} ONVIF_FOCUSMOVE_INFO;

typedef struct
{
    char token_achar[64];
    ONVIF_FOCUSMOVE_INFO focus_struct;
} ONVIF_FOCUSMOVEDATA_INFO;

typedef struct
{
    ONVIF_IMAGE_SETTINGDATA_INFO ImageSettingInfo;
    ONVIF_IMAGE_STATUSDATA_INFO  ImageStatusInfo;
    ONVIF_IMAGE_MOVEOPTIONS20DATA_INFO ImageMoveOptions;
    ONVIF_IMAGE_OPTIONSDATA_INFO ImageOptions;
    ONVIF_FOCUSMOVEDATA_INFO     ImageFocusMove;
}ONVIF_IMAGE_INFO;

//////////////////////////////////////////////////////////////
//
// TOTAL INFO
//
/////////////////////////////////////////////////////////////


typedef struct
{
    ONVIF_VIDEO_SOURCE_CONFIG_OPTIONS        videoSourceConfigOptions;
    ONVIF_VIDEO_SOURCE_CONFIG                videoSourceConfig[MEDIA_VIDEO_SOURCE_CONFIG_MAX_NUM];
    ONVIF_VIDEO_SOURCE                       videoSource[MEDIA_VIDEO_SOURCE_MAX_NUM];
    ONVIF_VIDEO_ENCODER_CONFIG_OPTIONS       videoEncoderConfigOptions;
    ONVIF_VIDEO_ENCODER_CONFIG               videoEncoderConfig[MEDIA_VIDEO_ENCODER_CONFIG_MAX_NUM];
    ONVIF_VIDEO_ENCODER_CONFIG               videoEncoderConfigExtra;
    ONVIF_VIDEO_OUTPUT                       videoOutputConfig;
    ONVIF_AUDIO_SOURCE_CONFIG_OPTIONS        audioSourceConfigOptions;
    ONVIF_AUDIO_SOURCE_CONFIG                audioSourceConfig[MEDIA_AUDIO_SOURCE_CONFIG_MAX_NUM];
    ONVIF_AUDIO_SOURCE                       audioSource[MEDIA_AUDIO_SOURCE_MAX_NUM];
    ONVIF_AUDIO_ENCODER_CONFIG_OPTIONS       audioEncoderConfigOptions[MEDIA_AUDIO_ENCODER_CONFIG_OPTIONS_MAX_NUM];
    ONVIF_AUDIO_ENCODER_CONFIG               audioEncoderConfig[MEDIA_AUDIO_ENCODER_CONFIG_MAX_NUM];
    ONVIF_AUDIO_OUTPUT_CONFIG_OPTIONS        audioOutputConfigOptions;
    ONVIF_AUDIO_OUTPUT_CONFIG                audioOutputConfig[MEDIA_AUDIO_OUTPUT_CONFIG_MAX_NUM];
    ONVIF_AUDIO_OUTPUT                       audioOutput[MEDIA_AUDIO_OUTPUT_MAX_NUM];
    ONVIF_PTZ_CONFIG_OPTIONS                 ptzConfigOptions;
    ONVIF_PTZ_CONFIG                         ptzConfig[MEDIA_PTZ_CONFIG_MAX_NUM];
    ONVIF_METADATA_CONFIG                    metadataConfig[MEDIA_METADATA_CONFIG_MAX_NUM];
    ONVIF_MEDIA_PROFILE                      mediaProfile[MEDIA_PROFILE_MAX_NUM];
    OSD_CONFIG                               osdConfig[OSD_MAX_CHANNEL];
    char                                     currentProfiletoken_achar[MEDIA_TOKEN_MAX_LEN+1];
}ONVIF_MEDIA_INFO;


typedef struct
{
    enum tt__DataTimeType     dateTimeType;
    enum tt__NTPUpdateCycle   NTPUpdateCycle;
    enum xsd__boolean         daylightSavings;
    char                      timeZone[64];
    char                      dayLightSavingInfo[64];
}ONVIF_DATETIME_INFO;

typedef enum _EVENT_ENABLE {EE_DISABLE, EE_ENABLE}EVENT_ENABLE;
typedef enum _RECORD_ENABLE {RE_DISABLE, RE_ENABLE}RECORD_ENABLE;
typedef enum _EVENT_ALARM_OUTPUT_CFG{EAOC_OPEN, EAOC_GROUND}EVENT_ALARM_OUTPUT_CFG;
typedef enum _EVENT_ALARM_INPUT_CFG{EAIC_HIGH, EAIC_LOW}EVENT_ALARM_INPUT_CFG;

#define BUF_MAX_NAME_SIZE 128
#define RECV_MAX_SIZE 256
#define EVT_FTP_BUF_SIZE_128 128
#define BUF_ALARM_CFG_SIZE 64
#define EVT_SMTP_BUF_SIZE_128 128
#define EVT_SD_BUF_SIZE_128 128
#define EVT_HTTP_BUF_SIZE_128 128
#define EVT_ISCSI_BUF_SIZE_128 128
#define EVT_MAX_EVENT_NUM 5
#define EVT_MAX_FTP_NUM 3
#define EVT_MAX_SMTP_NUM 3
#define EVT_MAX_SD_NUM 3
#define EVT_MAX_HTTP_NUM 3
#define EVT_MAX_ISCSI_NUM 1
/*
alarm input : ES_HIGH, ES_LOW
motion      : ES_NO_TRIGGER, ES_TRIGGER
network loss: ES_NETWORK_LOSS , ES_NETWORK_NORMAL
*/
typedef enum {ES_HIGH, ES_LOW ,ES_NO_TRIGGER, ES_TRIGGER, ES_NETWORK_LOSS, ES_NETWORK_NORMAL, ES_ERROR} EVT_STATUS;
typedef enum {CC_DI0, CC_MD0, CC_NL0, CC_SYNC, CC_PULL_CMD, CC_ERROR} CURRENT_CHANGE;

typedef enum {MD_TYPE, DI_TYPE, NL_TYPE } EVENT_TYPE;  //motion detection type, digital input type, network loss type

typedef enum {ALL_EVENT, MD_EVENT, DI_EVENT, NL_EVENT} REGISTER_TYPE;

typedef enum {AOS_GROUNDED, AOS_OPEN}ALARM_OUTPUT_STATUS;

typedef enum {

    EVENT_ER_OK               = 0, // ok
    EVENT_ER_GRR              = -1, //general error
    EVENT_ER_TIME_OUT         = -2,   // time out
    EVENT_ER_NAME_ERROR       = -3,    //name exist or name error
    EVENT_ER_LIMITATION_ERROR = -4   ////out of limitation
}EVENT_ER;


typedef struct _EVENT_INFO{

    ///CC_DI0: notify cmd,is DI0 changed      CC_MD0: notify cmd,is MD0 change CC_NL0: notify cmd, is NL0 changed   CC_PULL_CMD: is pull cmd
    CURRENT_CHANGE current_changed;
    EVT_STATUS DI0;//digital input
    EVT_STATUS MD0;//motion detect
    EVT_STATUS NL0;//network loss

}EVENT_INFO;


typedef struct _EVENT_CFG{

    int enable;
    char name[BUF_MAX_NAME_SIZE];
    int schedule_always;
    int schedule[7];//for Monday ~Sunday, using 24 bits to set 24 hours
    EVENT_TYPE event_type;
    int action_type;   //first bit:FTP  second bit:SMTP  third bit: HTTP 4th bit: SD card  5th:DIgital output 6th: ISCSI  value:0~63
    int triggered;  // 0: no trigger   1: trigger
    time_t trigger_time; ///the time when last event trigger
    int delay_time; // the event interval time
    char ftp_name[BUF_MAX_NAME_SIZE]; //ftp name
    char smtp_name[BUF_MAX_NAME_SIZE]; //SMTP name
    char http_name[BUF_MAX_NAME_SIZE]; //HTTP name
    char iscsi_name[BUF_MAX_NAME_SIZE]; //ISCSI name
    char sd_name[BUF_MAX_NAME_SIZE]; //SD card name
    char action[BUF_MAX_NAME_SIZE]; //value: log, snapshot, video
    int video_presecond; // if action == "video", set presecond  vaule:0~10 sec
    int video_postsecond; // if action == "video", set postsecond  value: 1~30 sec

} EVENT_CFG;

typedef struct _MOTION_CFG{


    char level[RECV_MAX_SIZE]; //value: "High,9,8,7,6,5,4,3,2,1,Low"
    char wnd[RECV_MAX_SIZE]; //value: "0~65535,0~65535,0~65535,0~65535,0~65535,0~65535,0~65535,0~65535,0~65535,0~65535,0~65535,0~65535"
    char enable[RECV_MAX_SIZE]; //value:"on"  or "off"

}MOTION_CFG;

typedef struct _EVENT_FTP_CFG{

    char ftp_name[EVT_FTP_BUF_SIZE_128];
    int port;
    char ftp_addr[EVT_FTP_BUF_SIZE_128];
    char user_name[EVT_FTP_BUF_SIZE_128];
    char password[EVT_FTP_BUF_SIZE_128];
    char folder_name[EVT_FTP_BUF_SIZE_128];
}EVENT_FTP_CFG;

typedef struct _EVENT_ALARMINPUT_CFG{

    char enable[BUF_ALARM_CFG_SIZE];//value :"on" or "off"  , now not using this parameter
    char cfg[BUF_ALARM_CFG_SIZE];//value : "HIGH" or "LOW"

}EVENT_ALARMINPUT_CFG;

typedef struct _EVENT_SMTP_CFG{

    char smtp_name[EVT_SMTP_BUF_SIZE_128];
    int server_port;
    char sender_addr[EVT_SMTP_BUF_SIZE_128];
    char recipient_addr[EVT_SMTP_BUF_SIZE_128];
    char server_addr[EVT_SMTP_BUF_SIZE_128];
    char user_name[EVT_SMTP_BUF_SIZE_128];
    char password[EVT_SMTP_BUF_SIZE_128];
    int ssl;//0 : STARTTLS   1: SSL

}EVENT_SMTP_CFG;


typedef struct _EVENT_SD_CFG{

    char sd_name[EVT_SD_BUF_SIZE_128];
    char sd_path[EVT_SD_BUF_SIZE_128];

}EVENT_SD_CFG;

typedef struct _EVENT_HTTP_CFG{

    char http_name[EVT_HTTP_BUF_SIZE_128];
    char http_url[EVT_HTTP_BUF_SIZE_128];
    char http_user[EVT_HTTP_BUF_SIZE_128];
    char http_password[EVT_HTTP_BUF_SIZE_128];

}EVENT_HTTP_CFG;

typedef struct _EVENT_ISCSI_CFG{

    char iscsi_name[EVT_ISCSI_BUF_SIZE_128];
    char iscsi_ip[EVT_ISCSI_BUF_SIZE_128];
    char iscsi_target_name[EVT_ISCSI_BUF_SIZE_128];
    char iscsi_user[EVT_ISCSI_BUF_SIZE_128];
    char iscsi_password[EVT_ISCSI_BUF_SIZE_128];
    int  iscsi_bmount;////0:not mount on ip cam   1: mount on ip cam
}EVENT_ISCSI_CFG;


typedef struct
{
    EVENT_ENABLE MD0_enable;
    char MD0_level[32];
    char MD0_wnd[256];
    EVENT_ALARM_OUTPUT_CFG DO0_CFG;
    EVENT_ALARM_INPUT_CFG DI0_CFG;
    EVENT_ENABLE DI0_enable;
}ONVIF_DETECTION_INFO;


typedef struct
{
    EVENT_CFG event_list[EVT_MAX_EVENT_NUM];
    int event_list_num;
    EVENT_SMTP_CFG smtp_list[EVT_MAX_SMTP_NUM];
    int smtp_list_num;
    EVENT_SD_CFG sd_list[EVT_MAX_SD_NUM];
    int sd_list_num;
    EVENT_HTTP_CFG http_list[EVT_MAX_HTTP_NUM];
    int http_list_num;
    EVENT_ISCSI_CFG iscsi_list[EVT_MAX_ISCSI_NUM];
    int iscsi_list_num;
    EVENT_FTP_CFG ftp_list[EVT_MAX_FTP_NUM];
    int ftp_list_num;
}ONVIF_EVENT_CONFIG;

typedef enum { NRT_SD_CARD, NRT_ISCSI} NVT_RECORD_TYPE;

typedef struct
{
    RECORD_ENABLE enable;
    RECORD_ENABLE schedule_always;
    int schedule[7];////for Monday ~Sunday, using 24 bits to set 24 hours
    int video_len;
    NVT_RECORD_TYPE record_type;
    char sd_name[128];
    char iscsi_name[128];
}ONVIF_RECORD_CONFIG;

typedef struct
{
    ONVIF_DEVICE_INFO       deviceInfo;
    ONVIF_USER              userList[USER_MAX_NUM];
    ONVIF_NETWORK_INFO      networkInfo;
    ONVIF_DEVICE_DISCOVERY  deviceDiscovery;
    ONVIF_EVENT_INFO        eventInfo[EVENT_MAX_NUM];
    ONVIF_MEDIA_INFO        mediaInfo;
    ONVIF_STREAM_INFO       streamInfo;
    ONVIF_PTZ_INFO          ptzInfo;
    ONVIF_IMAGE_INFO        imageInfo;
    ONVIF_DATETIME_INFO     datetimeInfo;
    ONVIF_DETECTION_INFO    detectionInfo;
    ONVIF_EVENT_CONFIG      eventConfig;
    ONVIF_RECORD_CONFIG     recordConfig;
}ONVIF_INFO;

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTDEVINFO_DEFINE_H  */
