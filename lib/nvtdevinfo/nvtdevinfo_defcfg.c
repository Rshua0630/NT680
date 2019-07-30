/* =================================================================
 *
 *      nvtdevinfo_defcfg.c
 *
 *      Device config and inforation library for Novatek NT9850x
 *
 * =================================================================
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <nvtcfg_define.h>
#include "nvtdevinfo.h"

/* =================================================================
 *   Default Setting
 * =================================================================
 */

#define HOSTNAME_NAME  "nt9668Xevb"
#define INTERFACE_NAME "eth0"
#define DEVICE_IP      "192.168.0.3"
#define BROADCAST_IP   "239.255.255.250"
#define DNS_IP         "192.168.0.4"
#define GATEWAY_IP     "192.168.0.4"
#define NET_MASK       "255.255.255.0"
#define RTSP_SERVER_IP "192.168.0.2"
#define RTSP_MULTICAST_IP "224.2.1.1"
#define HTTP_PORT      80
#define HTTPS_PORT     443
#define RTSP_PORT      554//855

ONVIF_DEVICE_INFO       gDefaultDeviceInfo =
{ MANUFACTURER,
  MODEL_NAME,
  FIRMWARE_VERSION,
  "123456789",
  "0101010101"
};
ONVIF_USER              gDefaultUserList[USER_MAX_NUM] =
{
    {1,"admin","admin",tt__UserLevel__Administrator},
    {1,"operator","operator",tt__UserLevel__Operator},
    {1,"user","user",tt__UserLevel__User},
    {0},
    {0},
};
ONVIF_NETWORK_INFO      gDefaultNetworkInfo =
{
    HOSTNAME_NAME,       // hostname
    DNS_IP,              // DNS1
    "",                  // DNS2
    FALSE,               // DNSfromDHCP
    // interface
    {
        INTERFACE_NAME,      // interface name
        DEVICE_IP,           // ip
        BROADCAST_IP,        // broad ip
        "",                  // MAC
        NET_MASK,            // NetMask
        GATEWAY_IP,          // Gateway
#ifdef NVT_DEFAULT_NETWORK_BOOT_PROTOCOL_DHCP_SERVER		// DHCP Server
        FALSE,                // DHCPServerEnable
        TRUE,                // fromDHCP
#elif defined NVT_DEFAULT_NETWORK_BOOT_PROTOCOL_DHCP_CLIENT	// DHCP Client
        FALSE,               // DHCPServerEnable
        TRUE,                // fromDHCP
#else                  // Static IP
        FALSE,               // DHCPServerEnable
        FALSE,               // fromDHCP
#endif
        DEVICE_IP            // linklocal
    },
    // protocol
    {
        {TRUE,tt__NetworkProtocolType__HTTP,HTTP_PORT,   TRUE},  // tt__NetworkProtocolType__HTTP
        {TRUE,tt__NetworkProtocolType__HTTPS,HTTPS_PORT, TRUE},  // tt__NetworkProtocolType__HTTPS
        {TRUE,tt__NetworkProtocolType__RTSP,RTSP_PORT, TRUE}     // tt__NetworkProtocolType__RTSP
    },
    // NTP
    {
        FALSE, //                                            isfromDHCP;
        tt__NetworkHostType__IPv4,       //                  NetworkHostType;
        "192.168.0.4"                    //                  ServerAddress;
    },
    // auth
    {
        xsd__boolean__true_,
    }
};
ONVIF_DEVICE_DISCOVERY  gDefaultDeviceDiscovery=
{
 tt__DiscoveryMode__Discoverable,
 {
  {tt__ScopeDefinition__Fixed,"onvif://www.onvif.org/hardware/660_EVB"},
  {tt__ScopeDefinition__Fixed,"onvif://www.onvif.org/Profile/Streaming"},
  {tt__ScopeDefinition__Fixed,"onvif://www.onvif.org/name/ipcam"},
  {tt__ScopeDefinition__Fixed,"onvif://www.onvif.org/type/NetworkVideoTransmitter"},
  {tt__ScopeDefinition__Fixed,"onvif://www.onvif.org/type/Device"},
  {tt__ScopeDefinition__Configurable,""},
  {tt__ScopeDefinition__Configurable,""},
  {tt__ScopeDefinition__Configurable,""}
 },
  //"tds:Device",
  //"tds:Device dn:NetworkVideoTransmitter",
  "dn:NetworkVideoTransmitter",
};
ONVIF_EVENT_INFO        gDefaultEventInfo[EVENT_MAX_NUM]=
{
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}},
    {0,0,0,(time_t)0,{0},{0},{0}}
};
ONVIF_PTZ_INFO        gDefaultPTZInfo[PTZ_MAX_NUM]=
{
    {
        {{0.0f, 0.0f, tt__NSS},{0.0f, tt__NSS}},
        {
            {"","",{{0.0f, 0.0f, tt__NSS},{0.0f, tt__NSS}}},
            {"","",{{0.0f, 0.0f, tt__NSS},{0.0f, tt__NSS}}},
            {"","",{{0.0f, 0.0f, tt__NSS},{0.0f, tt__NSS}}},
        },
    }
};
ONVIF_MEDIA_INFO        gDefaultMediaInfo=
{
  // videoSourceConfigOptions
  {
    {1,1},{1,1},{1920,1920},{1080,1080},"VST_Token1",
  },
  // videoSourceConfig                                   // Total JPEG H264 MPEG
  {{"VS_Name1",1,"VS_Token1","VST_Token1",{1,1,1920,1080}, {1, 1, 0, 0},},
   {"VS_Name2",1,"VS_Token2","VST_Token1",{1,1,1920,1080}, {1, 0, 1, 0},},
   //{"VS_Name3",1,"VS_Token3","VST_Token1",{1,1,1920,1080}, {1, 0, 1, 0},},
  },
  // videoSource
  {{"VST_Token1",30,{1920,1080}},
  },
  // video encoder options
  {
   // jpeg options
   {{{2048,1536},{1920,1080},{1280,720}, {640,480}, {320,240}}, // ResolutionsAvailable
     //{848,480}, {640,480}, {640,360}}, // ResolutionsAvailable
     {1,30},     // FrameRateRange
     {1,1},       // EncodingIntervalRange
     {64,16384}, // BitRateRange, unit is kbps
    },
   // mpeg4 options
   {{{1280,720}}, // ResolutionsAvailable
     {2,120},     // GovLengthRange
     {1,30},      // FrameRateRange
     {1,1},       // EncodingIntervalRange
     {64,16384},     // BitRateRange, unit is kbps
    },
   // h264 options
   {{{2048,1536},{1920,1080},{1280,720}, {640,480}, {320,240}},   // ResolutionsAvailable
     {2,120},                 // GovLengthRange
     {1,30},                  // FrameRateRange
     {1,1},                   // EncodingIntervalRange
     {64,16384},              // BitRateRange, unit is kbps
    },
  },
  // videoEncoderConfig
  {                                                   //Resolution  //MaxResolution   //Quality   //rateControl                         //CropControl //MPEG4   //GOV                       //Multicast //SessionTimeout
   {"VE_Name1", 1,"VE_Token1", tt__VideoEncoding__H264, {1920,1080},  {2048,1536},      80,         {tt__RateControlMode__CBR,30,1,4000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name2", 1,"VE_Token2", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name3", 1,"VE_Token3", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name4", 1,"VE_Token4", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name5", 1,"VE_Token5", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name6", 1,"VE_Token6", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name7", 1,"VE_Token7", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name8", 1,"VE_Token8", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name9", 1,"VE_Token9", tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name10",1,"VE_Token10",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name11",1,"VE_Token11",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name12",1,"VE_Token12",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name13",1,"VE_Token13",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name14",1,"VE_Token14",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name15",1,"VE_Token15",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
   {"VE_Name16",1,"VE_Token16",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200},
  },
  // videoEncoderConfigExtra
  {                                                  //Resolution  //MaxResolution   //Quality   //rateControl                         //CropControl //MPEG4   //GOV                       //Multicast //SessionTimeout
   "VE_Name3",1,"VE_Token3",tt__VideoEncoding__H264, {640,480},    {640,480},        80,         {tt__RateControlMode__CBR,30,1,1000}, {FALSE,0,0},  {0},       {60,tt__H264Profile__High}, {0},        200,
  },
  // videoOutputConfig
  {
#ifdef NVT_DEFAULT_CVBS_TYPE_OFF
    tt__CVBSMode__OFF,
#elif defined NVT_DEFAULT_CVBS_TYPE_NTSC
    tt__CVBSMode__NTSC,
#else
    tt__CVBSMode__PAL,
#endif
  },
  // audioSourceConfigOptions
  {
    "AST_Token1",
  },
  // audioSourceConfig
  {{"AS_Name1",2,"AS_Token1","AST_Token1",8,tt__AudioDulpexMode__Full},
  },
  // audioSource
  {{"AST_Token1",2},
  },
  // audioEncoderConfigOptions
  {{tt__AudioEncoding__AAC, 2, {16*8,32*16}, 2, {16,32} },
   {tt__AudioEncoding__AAC, 1, {32*16,0}, 1, {32,0} },
   },

  //audioEncoderConfig
  {{"AE_Name1",1,"AE_Token1",tt__AudioEncoding__AAC,32*16, 32,{0},200},
   {"AE_Name2",1,"AE_Token2",tt__AudioEncoding__AAC,16*8,  16,{0},200},
   //{"AE_Name3",1,"AE_Token3",tt__AudioEncoding__AAC, 32*16, 32,{0},200},

   },
  // audioOutputConfigOptions
  {
    {"AO_Token1"},tt__SendPrimacy__Audio__Auto,{0, 100},
  },
  // audioOutputConfig
  {{"AO_Name1",1,"AO_Token1","AOT_Token1", tt__SendPrimacy__Audio__Auto, tt__AudioOutputTarget__LineOut, 50},
  },
  // audioOutput
  {{"AOT_Token1"},
  },
    //ptzConfigOptions
  {
   {
    {tt__PT_PositionGenericSpace,   {TRUE, 0.0f, 10.0f}, {TRUE, 0.0f, 10.0f}},   //BoundAbsolutePantTiltPositionSpace
    {tt__Z_PositionGenericSpace,    {TRUE, 0.0f, 10.0f}},                        //BoundAbsoluteZoomPositionSpace;
    {tt__PT_TranslationGenericSpace,{TRUE, 0.0f, 10.0f}, {TRUE, 0.0f, 10.0f}},   //BoundRelativePanTiltTranslationSpace;
    {tt__Z_TranslationGenericSpace, {TRUE, 0.0f, 10.0f}},                        //BoundRelativeZoomTranslationSpace;
    {tt__PT_VelocityGenericSpace,   {TRUE, 0.0f, 10.0f}, {TRUE, 0.0f, 10.0f}},   //BoundContinuousPanTiltVelocitySpace;
    {tt__Z_VelocityGenericSpace,    {TRUE, 0.0f, 10.0f}},                        //BoundContinuousZoomVelocitySpace;
    {tt__PT_GenericSpeedSpace,      {TRUE, 0.0f, 10.0f}},                        //BoundPTSpeed;
    {tt__Z_ZoomGenericSpeedSpace,   {TRUE, 0.0f, 10.0f}},                        //BoundZSpeed;
   }
  },
    //ptzConfig
  {
   {"PTZ_Node1", "PTZ_Token1", "PTZ_Name1", 1,
        tt__PT_PositionGenericSpace, //DefaultAbsolutePantTiltPositionSpace
        tt__Z_PositionGenericSpace, //DefaultAbsoluteZoomPositionSpace
        tt__PT_TranslationGenericSpace, //DefaultRelativePanTiltTranslationSpace
        tt__Z_TranslationGenericSpace, //DefaultRelativeZoomTranslationSpace
        tt__PT_VelocityGenericSpace, //DefaultContinuousPanTiltVelocitySpace
        tt__Z_VelocityGenericSpace, //DefaultContinuousZoomVelocitySpace
        {
         {tt__PT_GenericSpeedSpace, 1.0f, 1.0f}, //DefaultPTZSpeed
         {tt__Z_ZoomGenericSpeedSpace, 1.0f}//DefaultPTZSpeed
        },
        "PT10S", //DefaultPTZTimeout
        {{tt__PT_PositionGenericSpace, {TRUE, 0.0f, 10.0f}, {TRUE, 0.0f, 10.0f}}}, //PanTiltLimits
        {{tt__Z_PositionGenericSpace, {TRUE, 0.0f, 10.0f}}}, //ZoomLimits
        {2, 0, tt__PTZ_RS485_PROTOCOL_PELCO_D, tt__RS485_BAUDRATE_9600, tt__RS485_DATA_BIT_8, tt__RS485_STOP_BIT_1, tt__RS485_PARITY_BIT_NONE}, //RS485Config
        FALSE,
    },
  },
  //metadataConfig
  {{"MD_Name1",2,"MD_Token1",{0}, {0}, {0}, 200},
  },
  // media profiles
 {{"Profile_Name1","Profile_Token1",TRUE, "VE_Token1", "VS_Token1", "AE_Token1", "AS_Token1", "AO_Token1", "MD_Token1", "PTZ_Token1", {2048,1536}},
  {"Profile_Name2","Profile_Token2",TRUE, "VE_Token2", "VS_Token2", "AE_Token2", "AS_Token1", "AO_Token1", "MD_Token1", "PTZ_Token1", {640,480}},
  //{"Profile_Name3","Profile_Token3",TRUE, "VE_Token3", "VS_Token3", "AE_Token3", "AS_Token1", "AO_Token1", "MD_Token1", "PTZ_Token1", {640,480}},
  {0},
 },
  //osd config {user_text , text_position , font_size , show_datetime , show_user_text , font_color , datetime_format , width , height}
 {{{'I','P','C','a','m','_','0','1', 0, 0, 0, 0, 0, 0, 0}, tt__OsdUserTextPos__UpperLeft, 25, 1, 1, 0x00FFFF00, tt__OsdDateTimeFmt1, 1152, 100},
  {{'I','P','C','a','m','_','0','1', 0, 0, 0, 0, 0, 0, 0}, tt__OsdUserTextPos__UpperLeft, 25, 1, 1, 0x00FFFF00, tt__OsdDateTimeFmt1, 1152, 100},
  {{'I','P','C','a','m','_','0','1', 0, 0, 0, 0, 0, 0, 0}, tt__OsdUserTextPos__UpperLeft, 25, 1, 1, 0x00FFFF00, tt__OsdDateTimeFmt1, 1152, 100},
  {{'I','P','C','a','m','_','0','1', 0, 0, 0, 0, 0, 0, 0}, tt__OsdUserTextPos__UpperLeft, 25, 1, 1, 0x00FFFF00, tt__OsdDateTimeFmt1, 1152, 100},
 },
 "",//"Profile_Token1", // current profile token
};

ONVIF_STREAM_INFO       gDefaultStreamInfo=
{
    RTSP_SERVER_IP,
    RTSP_MULTICAST_IP
};


ONVIF_IMAGE_INFO gDefaultImageInfo =
{
    { //ONVIF_IMAGE_SETTINGDATA_INFO
        "Image_Setting_Name1",
        {//ONVIF_IMAGE_SETTING_INFO
            { xsd__boolean__true_, 4}, //ONVIF_IMAGE_SETTING_BACKLIGHTCOMPENSATION_INFO
            50, //brightness_float;
            50, //colorsaturation_float;
            50, //contrast_float;
            50, //sharpness_float;
            0,  //hue_float;
            xsd__boolean__false_, //mirror_bool;
            xsd__boolean__false_, //flip_bool;
            tt__ColorMode__Normal, //color_int;
            {ONVIF_IMAGE_EXPOSUREMODE_AUTO, ONVIF_IMAGE_EXPOSUREPRIORITY_LowNoise, ONVIF_IMAGE_EXPOSURESTRATEGY_HIGHLIGHT, ONVIF_IMAGE_EXPOSUREANTIFLICKER_60HZ,
             {0.0f,0.0f,0.0f,0.0f}, 12.0f, 4.0f, 3.0f, 0.0f, 100.0f, 33333.0f, 125.0f, 6400.0f, 0.0f, 0.0f, 16666.0f, 200.0f, 0.0f}, //ONVIF_IMAGE_SETTING_EXPOSURE_INFO
            {ONVIF_IMAGE_AUTOFOCUSMODE_AUTO, 0.0f, 0.0f, 0.0f,},  //ONVIF_IMAGE_SETTING_FOCUSCONFIGURATION_INFO
            {ONVIF_IMAGE_IRCUTFILTERMODE_AUTO, ONVIF_IMAGE_IRCUTAUTOSUBMODE_IMAGESENSOR, 18, 0, 6, 0}, //ONVIF_IMAGE_IRCUTFILTERMODE
            { xsd__boolean__false_, 0}, //ONVIF_IMAGE_SETTING_WIDEDYNAMICRANGE_INFO, WDR
            { xsd__boolean__false_, 0}, //ONVIF_IMAGE_SETTING_WIDEDYNAMICRANGE_INFO, HDR
            { ONVIF_IMAGE_WHITEBALANCE_AUTO, 4.0f, 10.0f, 400.0f, 400.0f}, //ONVIF_IMAGE_SETTING_WHITEBALANCE_INFO
            {50.0f, 1.0f},// ONVIF_IMAGE_SETTING_NOISEREDUCTION_INFO
            xsd__boolean__false_, //forcepersistence_bool;
            {//ONVIF_IMAGE_SETTING_EXT20_INFO
                {ONVIF_IMAGE_STABILIZATION_MODE_OFF, 0.0f}, //ONVIF_IMAGE_SETTING_IMAGESTABILIZATION_INFO
                {"", 0.0f, ""}, //ONVIF_IMAGE_SETTING_EXT202_INFO
            },
            NAR_16_9, // aspect_ratio
        }
    },
    {//ONVIF_IMAGE_STATUSDATA_INFO
        "Image_Status_Name1",
        {//ONVIF_IMAGE_FOCUS_STATUS_INFO
            0.0f,  //position_float;
            ONVIF_MOVESTATUS_IDLE, //ONVIF_MOVESTATUS
            "", //error_pchar;
        }
    },
    {//ONVIF_IMAGE_MOVEOPTIONS20DATA_INFO
        "Image_MoveOption_Name1",
        {//ONVIF_IMAGE_MOVEOPTIONS20_INFO
            {//ONVIF_IMAGE_MOVEOPTIONS_ABSOLUTE
               {xsd__boolean__true_, 1.0f, 2.0f}, //position_struct;
               {xsd__boolean__true_, 1.0f, 2.0f}, //speed_struct;
            },
            {//ONVIF_IMAGE_MOVEOPTIONS_RELATIVE
               {xsd__boolean__true_, 0.0f, 1.0f}, //distance_struct;
               {xsd__boolean__true_, 1.0f, 2.0f}, //speed_struct;
            },
            {
               {xsd__boolean__true_, 1.0f, 2.0f}, //speed_struct;
            }
        }
    },
    {//ONVIF_IMAGE_OPTIONSDATA_INFO
        "Image_Option_Name1",
        {//ONVIF_IMAGE_OPTIONS_INFO
            { //ONVIF_IMAGE_OPTIONS_BACKLIGHTCOMPENSATION
                2,
                {xsd__boolean__true_, xsd__boolean__false_}, //backlightcompensation_aint[IMAGE_SIZE_MAX_NUM];
                {xsd__boolean__true_, 0.0f, 10.0f},//lv_struct;
            },
            {xsd__boolean__true_, 0.0f, 100.0f},  //brightness_struct
            {xsd__boolean__true_, 0.0f, 100.0f},  //colorsaturation_struct;
            {xsd__boolean__true_, 0.0f, 100.0f},  //contrast_struct;
            {//ONVIF_IMAGE_OPTIONS_EXPOSURE
                {ONVIF_IMAGE_EXPOSUREMODE_AUTO, ONVIF_IMAGE_EXPOSUREMODE_MANUAL}, //mode_aint[IMAGE_SIZE_MAX_NUM];
                {ONVIF_IMAGE_EXPOSUREPRIORITY_LowNoise, ONVIF_IMAGE_EXPOSUREPRIORITY_FrameRate}, //priority_aint[IMAGE_SIZE_MAX_NUM];
                {xsd__boolean__true_, 100.0f, 133333.0f}, //maxexposuretime_struct;
                {xsd__boolean__true_, 100.0f, 33333.0f}, //minexposuretime_struct;
                {xsd__boolean__true_, 100.0f, 51200.0f}, //maxgain_struct;
                {xsd__boolean__true_, 100.0f, 3200.0f}, //mingain_struct;
                {xsd__boolean__true_, 0.0f, 0.0f}, //maxiris_struct;
                {xsd__boolean__true_, 0.0f, 0.0f}, //miniris_struct;
                {xsd__boolean__true_, 100.0f, 133333.0f}, //exposuretime_struct;
                {xsd__boolean__true_, 100.0f, 3200.0f}, //gain_struct;
                {xsd__boolean__true_, 0.0f, 0.0f}, //iris_struct;
            },
            {//ONVIF_IMAGE_OPTIONS_FOCUS
                {ONVIF_IMAGE_AUTOFOCUSMODE_AUTO, ONVIF_IMAGE_AUTOFOCUSMODE_MANUAL}, //autofocus_aint[IMAGE_SIZE_MAX_NUM];
                {xsd__boolean__true_, 0.0f, 0.0f}, //defaultspeed_struct;
                {xsd__boolean__true_, 0.0f, 0.0f}, //nearlimit_struct;
                {xsd__boolean__true_, 0.0f, 0.0f}, //farlimit_struct;
            },
            {ONVIF_IMAGE_IRCUTFILTERMODE_OFF, ONVIF_IMAGE_IRCUTFILTERMODE_ON, ONVIF_IMAGE_IRCUTFILTERMODE_AUTO}, //ircutfiltermodes_aint
            {xsd__boolean__true_, 0.0f, 100.0f}, //sharpness_struct;
            {//ONVIF_IMAGE_OPTIONS_WIDEDYNAMICRANGE
                2,
                {xsd__boolean__true_, xsd__boolean__false_},
                {xsd__boolean__true_, 0.0f, 10.0f},
            },
            {//ONVIF_IMAGE_OPTIONS_WHITEBALANCE
                //mode_aint[IMAGE_SIZE_MAX_NUM];
                {ONVIF_IMAGE_WHITEBALANCE_AUTO, ONVIF_IMAGE_WHITEBALANCE_MANUAL},
                {xsd__boolean__true_, 1.0f, 1023.0f}, //yrgain_struct;
                {xsd__boolean__true_, 1.0f, 1023.0f}, //ybgain_struct;
            },
            {//ONVIF_IMAGE_OPTIONS_EXT
                {//ONVIF_IMAGE_OPTIONS_STABILIZATION
                    {ONVIF_IMAGE_STABILIZATION_MODE_OFF, ONVIF_IMAGE_STABILIZATION_MODE_ON, ONVIF_IMAGE_STABILIZATION_MODE_AUTO, ONVIF_IMAGE_STABILIZATION_MODE_Extended},
                    {xsd__boolean__true_, 0.0f, 0.0f}, //lv_struct
                }
            }
        }
    },
    {//ONVIF_FOCUSMOVEDATA_INFO;
        "Image_FocusMove_Name1",
        {//ONVIF_FOCUSMOVE_INFO
            {0.0f, 0.0f},    //ONVIF_FOCUSMOVE_ABSOLUTE_INFO
            {0.0f, 0.0f},    //ONVIF_FOCUSMOVE_RELATIVE_INFO
            {0.0f}           //ONVIF_FOCUSMOVE_CONTINUOUS_INFO
        }
    }
};

ONVIF_DATETIME_INFO gDefaultDateTimeInfo =
{ //ONVIF_DATETIME_INFO
  tt__DataTimeType__Manual,  //enum tt__DataTimeType     dateTimeType;
  tt__NTPUpdateCycle__daily, //enum tt__NTPUpdateCycle NTPUpdateCycle;
  xsd__boolean__false_,      //enum xsd__boolean    daylightSavings;
  "TaipeiStandardTime-8",    //char                 timeZone[64];
  FAKE_DST_STRING,           //char                 dayLightSavingInfo[64];
};

ONVIF_DETECTION_INFO gDefaultDetectionInfo =
{ //ONVIF_DETECTION_INFO
  EE_DISABLE, //EVENT_ENABLE MD0_enable
  "5",         //char MD0_level
  "65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535,65535", //char MD0_wnd[256]
  EAOC_OPEN,  //EVENT_ALARM_OUTPUT_CFG
  EAIC_HIGH,   //EVENT_ALARM_INPUT_CFG
  EE_DISABLE, //EVENT_ENABLE DI0_enable
};

ONVIF_EVENT_CONFIG gDefaultEventConfig =
{ //ONVIF_EVENT_CONFIG
   0
};

ONVIF_RECORD_CONFIG gDefaultRecordConfig =
{ //ONVIF_RECORD_CONFIG
  RE_DISABLE, //RECORD_ENABLE enable
  RE_ENABLE, // RECORD_ENABLE schedule_always;
  {0,0,0,0,0,0,0}, //int schedule[7];
  1,//int video_len;
  NRT_SD_CARD,
  "",
  "",
};

