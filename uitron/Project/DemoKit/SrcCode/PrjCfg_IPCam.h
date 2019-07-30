#ifndef _PRJCFG_IPCAM_H_
#define _PRJCFG_IPCAM_H_
#include "Type.h"
#include "SysKer.h"


//==============================================================================
//   MODE and Major Feature (Maintain these in \Kernel, \System, \Mode folder)
//==============================================================================
#define IPCAM_DEMO				  ENABLE

#define IPCAM_BUFFER_ALL             3 //(for MAIN-DISP-SUB1, its required buffer count= 3-4-3.)
                                     //DATA FLOW: ImagePipe is triggered by itself.
                                     //DATA USAGE: protected by nvtmpp new/release.
                                     //PRO: full function.
                                     //CON: need 3 buffer for each stream (at least)

#define IPCAM_BUFFER_TWO             2 //(for MAIN-DISP-SUB1, its required buffer count= 2-3-3.)
                                     //DATA FLOW: ImagePipe is triggered by Encoder or Display.
                                     //DATA USAGE: protected by recycle queue.
                                     //PRO: need 2 buffer for each stream.
                                     //CON: cannot support IPCAM_STREAM_EXT, ImgTrans Crop/Scale/Rotate, UserProc ByPass+AllowPull.

#define IPCAM_BUFFER_ONE             1 //(for MAIN-DISP-SUB1, its required buffer count= 1-1-3.)
                                     //DATA FLOW: ImagePipe is triggered by itself.
                                     //DATA USAGE: NOT protected.
                                     //PRO: need 1 buffer only for each stream.
                                     //CON: support ImagePipe+encode/display steps only, NOT support any other pre-encode process.
                                     //CON: if the performance of any frame is slower then 1 sensor VD, streaming/screen will appear some tearing effect.

	#define IPCAM_BUFFER_LEVEL       IPCAM_BUFFER_ALL
	#define	IPCAM_DISP_PATH_NO       UIAPPIPCAM_PATH_ID_1

//Custom product config selection
#define IPCAM_UHD_512MB                 1	//one sensor
#define IPCAM_FHDX4_512MB               2	//four sensor
#define IPCAM_2MP                       3	//one sensor
#define IPCAM_PRODUCT                   IPCAM_UHD_512MB

//vout(liveview): IPCAM_PATH_ID_1
#if defined(_TV_OFF_) && defined(_HDMI_OFF_) && defined(_Disp_VIRTUAL_LCD1_OFF_) && defined(_Disp_VIRTUAL_LCD2_OFF_)
	#define IPCAM_DISP_ON_OFF      0
#else
	#define IPCAM_DISP_ON_OFF      1
#endif

#define IPCAM_VDOENC_MAX_BUF_MS   		2700    //length of encoder output buffer
#define IPCAM_VDOENC_RESERVED_BUF_MS 	0    	//reserved length of encoder output buffer
#define IPCAM_VDOENC_MIN_I_RATIO		150		//the ratio of I frame min size to bitrate
#define IPCAM_VDOENC_MIN_P_RATIO		100		//the ratio of P frame min size to bitrate

//==============================================================================
//   MODE and Major Feature (Maintain these in \Kernel, \System, \Mode folder)
//==============================================================================
#define PLAY_MODE                       DISABLE
#define PHOTO_MODE                      DISABLE
#define MOVIE_MODE                      DISABLE
#define USB_MODE                        DISABLE
#define SLEEP_MODE                      DISABLE
#define ENG_MODE                        DISABLE
#define IPCAM_MODE                      ENABLE
#define UPDFW_MODE                      DISABLE
#define IPCAMPB_MODE					ENABLE

#define IPCAM_FUNC                      ENABLE
#define STAMP_FUNC                      DISABLE
#define UI_FUNC                         DISABLE
#define WIFI_FUNC                       DISABLE

#if (IPCAM_DEMO == ENABLE)
#define FORCE_UNPLUG_RTSP               ENABLE
#else
#define FORCE_UNPLUG_RTSP               ENABLE
#endif

#define DEMO_SENSOR6_CCIR 				DISABLE

#define GPROF_FUNC					DISABLE

//==============================================================================
//   GENERAL CONFIG
//==============================================================================
//..............................................................................
#if defined(_FW_PARTIAL_LOAD_ON_)
#else
#endif

//..............................................................................
// FW version and name
#define FW_UPDATE_NAME                  "A:\\FW96680Z.bin"
#define FW_VERSION_RELEASE              DISABLE //ENABLE
#define FW_VERSION_NUM                  "REGULAR_0402_001"

//..............................................................................
/**
     The definition of EmbMem is the storage without any card detection,
     which means the end-user cannot take that storage out. The EmbMem can also
     be a combination of partition such as partition 0 is spi-uITRON,
     partition 1 is ram-FAT these depend on what you designed on DxStorage_Mem.c
     Normally, in SPI case, we design the ram-disk as internal FAT

*/
// Internal Storage Mapping to DxStorage
#define USER_DX_TYPE_EMBMEM_LOADER      DX_TYPE_EMBMEM0 //STRG_OBJ_FW_RSV1
#define USER_DX_TYPE_EMBMEM_MODELEXT    DX_TYPE_EMBMEM1 //STRG_OBJ_FW_RSV2
#define USER_DX_TYPE_EMBMEM_UITRON      DX_TYPE_EMBMEM2 //STRG_OBJ_FW_RSV3
#define USER_DX_TYPE_EMBMEM_UBOOT       DX_TYPE_EMBMEM3 //STRG_OBJ_FW_RSV4
#define USER_DX_TYPE_EMBMEM_LINUX       DX_TYPE_EMBMEM4 //STRG_OBJ_FW_RSV5
#define USER_DX_TYPE_EMBMEM_DSP         DX_TYPE_EMBMEM5 //STRG_OBJ_FW_RSV6
#define USER_DX_TYPE_EMBMEM_DSP2        DX_TYPE_EMBMEM6 //STRG_OBJ_FW_RSV7
#define USER_DX_TYPE_EMBMEM_PSTORE      DX_TYPE_EMBMEM7 //STRG_OBJ_PSTORE
#define USER_DX_TYPE_EMBMEM_FAT         DX_TYPE_EMBMEM8 //STRG_OBJ_FAT1


//==============================================================================
//   SYSTEM FUNCTIONS
//==============================================================================
//..............................................................................
// Boot
#define POWERON_PERF                    ENABLE  //enable performance benchmark
#define POWERON_TRACE                   DISABLE //stop and wait for user enter cmd: "dsc boot"
#define POWERON_FAST_BOOT               DISABLE
#define POWERON_FAST_CPU2_BOOT          ENABLE
#define POWERON_FAST_RECORD             DISABLE
#define POWERON_FAST_WIFI               DISABLE //NOTE: need to enable POWERON_FAST_CPU2_BOOT too
#define POWERON_WAIT_FS_READY           DISABLE
#define WAITPHOTO_FUNCTION              ENABLE
#define POWERONLOGO_FUNCTION            DISABLE
#define POWEROFFLOGO_FUNCTION           DISABLE //ENABLE
#if ((POWERON_FAST_BOOT == ENABLE) || (POWERON_FAST_RECORD == ENABLE))
#define POWERONSOUND_FUNCTION           DISABLE
#else
#define POWERONSOUND_FUNCTION           ENABLE
#endif
#define POWEROFFSOUND_FUNCTION          ENABLE
#define _LOGO_                          _LOGO_NOVATEK_
#if (defined(_LCDTYPE_ILI9341_IF8B_) || \
     defined(_LCDTYPE_TG078UW006A0_DSI_)|| \
     defined(_LCDTYPE_UT35067A0_ILI9488_DSI_))
#define LOGO_DISP_LAYER                 LAYER_VDO1 // VDO2 logo has no rotation
#else
#define LOGO_DISP_LAYER                 LAYER_VDO2
#endif
#define LENSERROFF_FUNCTION             DISABLE

#if (POWERON_PERF == ENABLE)
#define TM_BOOT_ARRAY(n)                static TIMEMARK tmbuf[(n)] = {0}
#define TM_BOOT_INIT(n)                 TM_Init(tmbuf, n)
#define TM_BOOT_BEGIN(a,b)              {static BOOL isFirst=TRUE; \
                                         if (isFirst) TM_Begin(a,b);\
										 	isFirst = FALSE;}
#define TM_BOOT_END(a,b)                {static BOOL isFirst=TRUE; \
                                         if (isFirst) TM_End(a,b);\
										 	isFirst = FALSE;}
#else
#define TM_BOOT_ARRAY(n)
#define TM_BOOT_INIT(n)
#define TM_BOOT_BEGIN(a,b)
#define TM_BOOT_END(a,b)
#endif

//#NT#2016/07/12#Niven Cho -begin
//#NT#LINUX_LOAD_BY_UBOOT
//#NT#2016/07/12#Niven Cho -end

//..............................................................................
// Storage Config
#define FWS_FUNC                        DISABLE

#if defined(_EMBMEM_NONE_)
#define PST_FUNC                        DISABLE
#else
#if(IPCAM_DEMO ==ENABLE)
#define PST_FUNC                        DISABLE // IPCAM does not  need pstore
#define PST_VER                         2
#else
#define PST_FUNC                        DISABLE
#endif
#endif
//#NT#2016/12/14#Niven Cho -begin
//#NT#EMMC_AS_PSTORE
// only for (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
#define PST_DEV_ID 4 //used for gxstrg to mount linux emmc-pstore partition
#define PST_FS_DRIVE "E:\\"
//#NT#2016/12/14#Niven Cho -end

//..............................................................................
// Input Config
#define INPUT_FUNC                      DISABLE
//key
#define POWERKEY_FUNCTION               ENABLE //Auto Detect
#define NORMALKEY_FUNCTION              ENABLE //Auto Detect
#define STATUSKEY_FUNCTION              DISABLE //Auto Detect
#define MODEKEY_FUNCTION                DISABLE //Auto Detect

//..............................................................................
// Output Config
#define OUTPUT_FUNC                     DISABLE
//LED
#define LED_FUNCTION                    DISABLE //Auto Detect

//..............................................................................
// Power-Saving Config
#define PWR_FUNC                        DISABLE
//battery level
//#NT#2016/12/15#Niven Cho -begin
//#NT#FIX DEMO1 with USB HOST, get low power
#if defined(_MODEL_DEMO1_) && (!defined(_USB1_HOST_))
#define PWRLEVEL_FUNCTION               ENABLE //Auto Detect
#else
#define PWRLEVEL_FUNCTION               DISABLE //Auto Detect
#endif
//#NT#2016/12/15#Niven Cho -end
//auto power-off when battery level = empty
#define EMPTYPWROFF_FUNCTION            DISABLE
//auto sleep
#define AUTOSLEEP_FUNCTION              DISABLE //Auto Detect
//auto power-off
#define AUTOPWROFF_FUNCTION             DISABLE //Auto Detect

//..............................................................................
// Date-Time Config
#define DEF_YEAR                        2015
#define DEF_MONTH                       1
#define DEF_DAY                         1
#define MIN_YEAR                        2000
#define MAX_YEAR                        2050


//..............................................................................
// Sensor Config (for 1,2,4 devices)

#define SENSOR_CAPS_COUNT               1
#define SENSOR_CAPS_MASK                (SENSOR_1)
#define SENSOR_MAP_1                    SENSOR_ID_1
#define SENSOR_MAP_2                    NA
#define SENSOR_MAP_3                    NA
#define SENSOR_MAP_4                    NA
#define SENSOR_MAP_5                    NA
#define SENSOR_MAP_6                    NA
#define SENSOR_MAP_7                    NA
#define SENSOR_MAP_8                    NA

#define SENSOR_GP_FUNC                  DISABLE

// Sensor group
#define SENSOR_GP_1                     SENSOR_MULTI_ID_NONE /*ex: SENSOR_MULTI_ID_1|SENSOR_MULTI_ID_2*/
#define SENSOR_GP_2                     SENSOR_MULTI_ID_NONE /*ex: SENSOR_MULTI_ID_3|SENSOR_MULTI_ID_4*/
#define SENSOR_GP_3                     SENSOR_MULTI_ID_NONE
#define SENSOR_GP_4                     SENSOR_MULTI_ID_NONE
#define SENSOR_GP_5                     SENSOR_MULTI_ID_NONE
#define SENSOR_GP_6                     SENSOR_MULTI_ID_NONE
#define SENSOR_GP_7                     SENSOR_MULTI_ID_NONE
#define SENSOR_GP_8                     SENSOR_MULTI_ID_NONE

// Sensor sub group

#define SENSOR_SUBGP_1                  SENSOR_MULTI_ID_NONE
#define SENSOR_SUBGP_2                  SENSOR_MULTI_ID_NONE
#define SENSOR_SUBGP_3                  SENSOR_MULTI_ID_NONE
#define SENSOR_SUBGP_4                  SENSOR_MULTI_ID_NONE
#define SENSOR_SUBGP_5                  SENSOR_MULTI_ID_NONE
#define SENSOR_SUBGP_6                  SENSOR_MULTI_ID_NONE
#define SENSOR_SUBGP_7                  SENSOR_MULTI_ID_NONE
#define SENSOR_SUBGP_8                  SENSOR_MULTI_ID_NONE


// Sensor Insert Mask
//#define SENSOR_INSERT_MASK              (SENSOR_1) // support insert sensor
//#define SENSOR_INSERT_MASK              (SENSOR_2) // support insert sensor
#define SENSOR_INSERT_MASK              (0) // if NOT support insert sensor, please choose this

#define SENSOR_DEFAULT_ATTACH_MASK      (SENSOR_CAPS_MASK & ~(SENSOR_INSERT_MASK)) // hw default attach sensors

#define SENSOR_INSERT_FUNCTION          ((SENSOR_CAPS_MASK == SENSOR_DEFAULT_ATTACH_MASK)?(DISABLE):(ENABLE)) //Auto Detect (Dynamic attach)

#define SENSOR_DEFAULT_DISPLAY_MASK     (SENSOR_1)


//..............................................................................
// Flashlight Config

#define FLASHLIGHT_FUNCTION             DISABLE //DISABLE


//..............................................................................
// Lens Config
#define LENS_FUNCTION                   DISABLE
#define LENS_FOCAL_LENGTH               2880    // unit:um


//..............................................................................
// Display Config (for LCD, LCD2, TV and HDMI device)

#define DISPLAY2_FUNC                   ENABLE

#define LCD2_FUNC_USERSTATUS            DISABLE //use LCD2 as status display. (not for video display)

//NOTE: LCD1 output from DOUT1 only
#if defined(_Disp_VIRTUAL_LCD1_OFF_)
#define LCD1_FUNC                       DISABLE
#define LCD1_MASK                       0x00
#else
#define LCD1_FUNC                       ENABLE
#define LCD1_MASK                       0x01
#endif
//NOTE: TV output from DOUT1 or DOUT2
#if defined(_TV_OFF_)
#define TV_FUNC                         DISABLE
#define TV_MASK                         0x00
#else
#define TV_FUNC                         ENABLE
#define TV_MASK                         0x02
#endif
//NOTE: HDMI output from DOUT1 or DOUT2
#if defined(_HDMI_OFF_)
#define HDMI_FUNC                       DISABLE
#define HDMI_MASK                       0x00
#else
#define HDMI_FUNC                       ENABLE
#define HDMI_MASK                       0x04
#endif
#define LCD2_FUNC                       DISABLE
#define LCD2_MASK                       0x00

#define DISPLAY_CAPS_COUNT              1 //1: only single display, 2: allow single display or dual display
#define DUALDISP_FUNC                   ((DISPLAY_CAPS_COUNT == 2)?ENABLE:DISABLE)

#define DISPLAY_DEFAULT_ATTACH_MASK     DISPLAY_1 // hw default attach display

#define DISPLAY_DEFAULT_ATTACH_DEVICE   LCD1_MASK //LCD1_MASK or HDMI_MASK or TV_MASK
#define DISPLAY2_DEFAULT_ATTACH_DEVICE  0 //LCD2_MASK or TV_MASK or HDMI_MASK

//LCD related
#define LCD_AUTORESET_FUNCTION          DISABLE //Auto Reset - LCD register
#define LCD_AUTOBACKLIGHT_FUNCTION      DISABLE //Auto Adjust Backlight - with AE LV
#define LCD_ROTATE_FUNCTION             DISABLE //Auto Detect - LCD rotate (enable to support LCD body mirror or flip)
//TV related
#define TV_INSERT_FUNCTION              TV_FUNC //Auto Detect - TV cable insert
//HDMI related
#define HDMI_INSERT_FUNCTION            HDMI_FUNC //Auto Detect - HDMI cable insert
//LCD2 related
#define LCD2_INSERT_FUNCTION            LCD2_FUNC //Auto Detect - LCD2 cable insert

#define TV_POWER_SAVE_MODE			DISABLE // if no TV insert, close IDE device

// Display Config (for UI window, FD window)

// OSD size and format
#if defined(_Disp_IFDSI_LCD1_TG078UW006A0_)
#define DISPLAY_OSD_W                   768//320//640
#define DISPLAY_OSD_H                   240//240
#else
#define DISPLAY_OSD_W                   320//640
#define DISPLAY_OSD_H                   240
#endif
#define DISPLAY_OSD_FMT                 DISP_PXLFMT_INDEX8 // 256 colors
//#define DISPLAY_OSD_FMT                 DISP_PXLFMT_RGBA5658_PK // 16bpp-colors

//UI tool layout size setting
#if defined(_Disp_IFDSI_LCD1_TG078UW006A0_)
#define TOOL_LAYOUT_W                   768//320//640
#define TOOL_LAYOUT_H                   240//240//480
#else
#define TOOL_LAYOUT_W                   320//640
#define TOOL_LAYOUT_H                   240//480
#endif
#if (defined(_Disp_IF8B_LCD1_ILI9341_) || \
     defined(_Disp_IFDSI_LCD1_TG078UW006A0_) || \
     defined(_Disp_IFDSI_LCD1_UT35067A0_ILI9488_))
#define OSD_USE_ROTATE_BUFFER           ENABLE  //use rotate buffer (enable to support LCD with stripe-type subpixel)
#else
#define OSD_USE_ROTATE_BUFFER           DISABLE //use rotate buffer (enable to support LCD with stripe-type subpixel)
#endif

#define OSD2_FUNC						DISABLE
#define OSD2_USE_DOUBLE_BUFFER          ENABLE //use double buffer


// Display Config (for UI background, App quickview, photo frame, ...)

// VDO size and format
#if defined(_Disp_IFDSI_LCD1_TG078UW006A0_)
#define DISPLAY_VDO_W                   1280//640
#define DISPLAY_VDO_H                   400//480
#else
#define DISPLAY_VDO_W                   640
#define DISPLAY_VDO_H                   480
#endif
#define DISPLAY_VDO_FMT                 DISP_PXLFMT_YUV422_PK

#define VDO_USE_DOUBLE_BUFFER           DISABLE //use double buffer
#define VDO2_USE_DOUBLE_BUFFER          DISABLE
#define VDO_USE_PHOTO_SOURCE            ENABLE  //display photo video streaming
#if (defined(_Disp_IF8B_LCD1_ILI9341_) || \
     defined(_Disp_IFDSI_LCD1_TG078UW006A0_)|| \
     defined(_Disp_IFDSI_LCD1_UT35067A0_ILI9488_))
#define VDO_USE_ROTATE_BUFFER           ENABLE  //use rotate buffer (enable to support LCD with stripe-type subpixel)
#else
#define VDO_USE_ROTATE_BUFFER           DISABLE //use rotate buffer (enable to support LCD with stripe-type subpixel)
#endif

#define VDO_USE_COPY_BUFFER             DISABLE //use copy buffer to avoid black between UI mode

#if (DISPLAY2_FUNC == ENABLE)
//#define VDOOUT_MASK     				(DISPLAY_1) // app's video will output to which display
//#define VDOOUT_MASK     				(DISPLAY_2) // app's video will output to which display
#define VDOOUT_MASK     				(DISPLAY_1|DISPLAY_2) // app's video will output to which display
#else
#define VDOOUT_MASK     				(DISPLAY_1) // app's video will output to which display
#endif

#define LCD_PREVIEW_VDO_FMT             GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define LCD2_PREVIEW_VDO_FMT            GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define TV_PREVIEW_VDO_FMT              GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define HDMI_PREVIEW_VDO_FMT            GX_IMAGE_PIXEL_FMT_YUV420_PACKED

// Display Config (for DispSrv; Photo App, Movie App and Play App)
// VDO1 size and format (for LCD device)
//LCD1:
#if defined(_Disp_VIRTUAL_LCD1_OFF_)
#define LCDMODE                         DISABLE
#elif defined(_Disp_IF8B_LCD1_AUCN01_)
#define LCDMODE                         DISP_LCDMODE_RGBD320 //DISP_LCDMODE_YUV640
#elif defined(_Disp_IF8B_LCD1_PW35P00_HX8238D_) || \
      defined(_Disp_IF8B_LCD1_ILI9341_)
#define LCDMODE                         DISP_LCDMODE_RGB_SERIAL
#elif (defined(_Disp_IFDSI_LCD1_NT35510_) \
    || defined(_Disp_IFDSI_LCD1_UT35067A0_ILI9488_))
#define LCDMODE                         DISP_LCDMODE_MIPI
#else
#error "Unknown _LCD_"
#endif

#define LCD2MODE                        DISABLE

// VDO1 size and format (for TV device)
//TV: DISP_TVMODE_PAL mode
#define TVMODE                          DISP_TVMODE_NTSC_SD

// VDO1 size and format (for HDMI device)
//HDMI: DISP_HDMIMODE_1920X1080I60 mode
//#define HDMIMODE                        DISP_HDMIMODE_1920X1080I60
#define HDMIMODE                        DISP_HDMIMODE_MAX


#define DUALCAM_PIP_BEHIND_FLIP         DISABLE  //Flip 2nd cam image in PIP view. Need extra 1/2 VDO buffer



//..............................................................................
// External Audio Config (for LineOut device)

//LINE related

//..............................................................................
// External Storage Config
#if defined(_CPU2_LINUX_)
#define SDINSERT_FUNCTION               DISABLE //Auto Detect by Linux
#else
#define SDINSERT_FUNCTION               ENABLE //Auto Detect by uItron
#endif
#define SDHOTPLUG_FUNCTION              DISABLE
//..............................................................................
// FILEDB Config
#define USE_FILEDB                      DISABLE
#define FILEDB_CARDV_ROOT               "Novatek"   // the max length should not exceeds 16
// DCF Config
#define USE_DCF                         DISABLE
#define DCF_SUPPORT_FORMAT              (DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI|DCF_FILE_TYPE_MOV|DCF_FILE_TYPE_MP4)
//..............................................................................
// EXIF Config
// End string must be '\0', total char must be 31
// End string must be '\0', total char must be 31
#define EXIF_MODEL_NAME                 "96680"
// End string must be '\0', total char must be 31
#define EXIF_IMAGE_DESCROPTOIN          "NOVATEK CAMERA"


//..............................................................................
// Movie Maker/Model name Config
// End string must be '\0', total char must plus 1
#define MOVAPP_COMP_MANU_STRING         "NOVATEK"
// End string must be '\0', total char must plus 1
#define MOVAPP_COMP_NAME_STRING         "DEMO1"
//..............................................................................
// Usb Config

#define USBINSERT_FUNCTION              DISABLE //Auto Detect

#define USB_VID                         0x0603
#define USB_PID_PCCAM                   0x8612 // not support pc cam
#define USB_PID_PRINT                   0x8613
#define USB_PID_MSDC                    0x8611

#define USB_PRODUCT_REVISION            '1', '.', '0', '0'
#define USB_VENDER_DESC_STRING          'N', 0x00,'O', 0x00,'V', 0x00,'A', 0x00,'T', 0x00,'E', 0x00,'K', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_VENDER_DESC_STRING_LEN      0x09
#define USB_PRODUCT_DESC_STRING         'D', 0x00,'E', 0x00,'M', 0x00,'O', 0x00,'1', 0x00, 0x20, 0x00, 0x00, 0x00 // NULL
#define USB_PRODUCT_DESC_STRING_LEN     0x07
#define USB_PRODUCT_STRING              'N','v','t','-','D','S','C'
#define USB_SIDC_DESC_STRING            'D', 0x00,'E', 0x00,'M', 0x00,'O', 0x00,'1', 0x00, 0x20, 0x00, 0x00, 0x00 // NULL
#define USB_SIDC_DESC_STRING_LEN        0x07
#define USB_MTP_FRIENDNAME_STRING       'D','E','M','O','1',0x00  //maximum length is 253 bytes
#define USB_MTP_FRIENDNAME_STRING_LEN   0x06                      //maximum length is 253 bytes
#define USB_VENDER_STRING               'N','O','V','A','T','E','K'
#define USB_VENDER_SIDC_DESC_STRING     'N', 0x00,'O', 0x00,'V', 0x00,'A', 0x00,'T', 0x00,'E', 0x00,'K', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_VENDER_SIDC_DESC_STRING_LEN 0x09
#define USB_SERIAL_NUM_STRING           '9', '6', '6', '8', '0','0', '0', '0', '0', '0','0', '0', '1', '0', '0'

//..............................................................................
// Other Config

#define GSENSOR_FUNCTION                DISABLE  //G-sensor
#define GPS_FUNCTION                    DISABLE

#define SHOW_ADC_ON_SCREEN              DISABLE
#define CALIBRATION_FUNC                DISABLE  // Enable/Disable Calibration
#define WATERLOGO_FUNCTION              DISABLE  // Enable/Disable waterlogo function

// Important Notice:
//    If enable MULTILINGUAL_FUNCTION, remember to update font table and make sure the font height is not exceed the buffer size!!
#define MULTILINGUAL_FUNCTION           DISABLE  // Enable/Disable Multi-Lingual movie stamp function

#define TIMELAPSE_LPR_FUNCTION          DISABLE   // Enable/Disable Timelapse Low Power Record function

//---------------------------------------------------------------------------
// For Video Debug Info
//---------------------------------------------------------------------------
#define _MOVIE_IQLOG_                   DISABLE     // Enable/Disable Video IQLOG, Spark Chou

//==============================================================================
//   PLAYBACK FUNCTIONS
//==============================================================================
#define SLIDESHOW_MUSIC_OPTION          DISABLE //longer no maintain
// copy card to card function
#define COPYCARD2CARD_FUNCTION          DISABLE

//==============================================================================
//   PHOTO FUNCTIONS
//==============================================================================

#define ASSIST_BEAM_FUNC                DISABLE
#define GDC_FUNC                        DISABLE
#define AF_BEAM_EV_THRESHOLD            540
#define _FD_FUNC_                       ENABLE
#define _TSR_FUNC_                      DISABLE
#define _ODP_FUNC_                      DISABLE
#define PIP_VIEW_FUNC                   DISABLE  // PipView (ENABLE: display 2 sensor by PipView, DISABLE: display user selected sensor)
#define PIP_VIEW_FASTSWITCH             DISABLE  // display single sensor method (ENABLE: always display by Pipview, DISABLE: display by direct path)
#define SBS_VIEW_FUNC                   DISABLE // SbsView (ENABLE: display 2 sensor by SbsView, DISABLE: display user selected sensor)
#define SHDR_FUNC                       DISABLE
#define WDR_FUNC                        DISABLE
#define DEFOG_FUNC                      DISABLE // ENABLE

#if !defined(_Gyro_None_)
#define RSC_FUNC                        ENABLE
#else
#define RSC_FUNC                        DISABLE
#endif
//..............................................................................
#define _SCREENNAIL_SIZE_               _SCREENNAIL_SIZE_VGA_
#define _QUICKVIEW_SIZE_                _QUICKVIEW_SIZE_SUIT_FOR_DISP

//..............................................................................
// Photo Frame Pstore Version Config
#define PF_PSTORE_VER                   0x00000105

//==============================================================================
//   MOVIE FUNCTIONS
//==============================================================================
#if !defined(_Gyro_None_)
#define MOVIE_RSC                       ENABLE
#else
#define MOVIE_RSC                       DISABLE
#endif
#define MOVIE_PIM_CAPTURE               DISABLE // picture in movie method 1 (capture full resolution image)
#define MOVIE_GDC_FUNC                  DISABLE
#define MOVIE_SMEAR_R_FUNC              DISABLE
#define MOVIE_ENSURE_SD_CLASS4          DISABLE
#define MOVIE_ENSURE_SD_32KCLUSTER      DISABLE
#define MOVIE_MULTI_RECORD_FUNC         ENABLE
#define MOVIE_REC_YUVMERGE              DISABLE // slowly timelapse record mode, frame period >= 1S
#define MOVIE_AUTOREC_ACPLUG            ENABLE
#define MOVIE_NET_USE_STREAM_1          DISABLE
#define MOVIE_UVAC_FUNC                 DISABLE
#define MOVIE_CDNN_FUNC                 DISABLE


//#define MOVIE_DIS                       DISABLE //sould be replaced by MOVIE_DIS_MV_FUNC & MOVIE_DIS_COMP_FUNC
#define MOVIE_DIS_MV_FUNC				DISABLE //Digital Image Stabilzier: calculate motion vector
#define MOVIE_DIS_COMP_FUNC				DISABLE //DIS compensation: if set MOVIE_DIS_COMP_FUNC = enable, it should set MOVIE_DIS_MV_FUNC = TRUE, too
#define MOVIE_FD_FUNC_                  DISABLE
#define MOVIE_FD_DRAW_VIDEO             DISABLE
#define MOVIE_BC_FUNC                   DISABLE
#define MOVIE_TD_FUNC                   DISABLE // Depend on MOVIE_BC_FUNC
#define MOVIE_TD_DRAW_VIDEO             DISABLE
#define MOVIE_TWD_FUNC                  DISABLE // Depend on MOVIE_BC_FUNC
#define MOVIE_TWD_DRAW_VIDEO            DISABLE
#define MOVIE_TZD_FUNC                  DISABLE // Depend on MOVIE_BC_FUNC
#define MOVIE_TZD_DRAW_VIDEO            DISABLE

#define UCTRL_APP_MOVIE_FEATURE_SETGET  DISABLE
#define MEDIA_VASTREAM                  DISABLE
#define IR_CUT_FUNC                     DISABLE
#define LOGFILE_FUNC                    DISABLE
#define _ADAS_FUNC_                     DISABLE
//..............................................................................
// ADAS subsetting (The following sub-functions are available only if _ADAS_FUNC_ = ENABLE
#define _AUTOVP_FUNC_                   ENABLE
#define _SNG_FUNC_                      DISABLE
#define _DRAW_LDFCINFO_ON_OSD_          ENABLE
#define _DRAW_SNG_ROI_                  DISABLE
#define _DRAW_LDWSINFO_ON_VIDEO_        DISABLE     // This function is only valid if data path is D2D!!

#define MOVIE_TSR_FUNC_                 DISABLE
#define MOVIE_TSR_DRAW_VIDEO            DISABLE
#define TSR_MODEL_IN_PSTORE             DISABLE
#define MOVIE_LPR_FUNC                  DISABLE

#define CROP_SOURCE_W                   3840
#define CROP_SOURCE_H                   2160


//==============================================================================
//   MOVIE PLAY FUNCTIONS
//==============================================================================
#define MOVIE_PLAY_VIEW_FUNC            ENABLE // MoviePlayView

//==============================================================================
//   USBCAM FUNCTIONS
//==============================================================================
#define UVAC_MODE_2_PATH                DISABLE
#if (UVAC_MODE_2_PATH==ENABLE)
#undef MOVIE_MULTI_RECORD_FUNC
#define MOVIE_MULTI_RECORD_FUNC         ENABLE
#endif
//==============================================================================
//   USBCHARGE FUNCTIONS
//==============================================================================
#define USB_CHARGE_FUNCTION             DISABLE
#define USB_CHARGE_VERIFY               DISABLE

//==============================================================================
//   USBDISK FUNCTIONS
//==============================================================================

//==============================================================================
//   MSDCVENDOR_NVT, is required for PC tools to transmit commands and data via USB / NET
//==============================================================================
#if(IPCAM_DEMO ==ENABLE)
#define MSDCVENDOR_NVT                  ENABLE  //for via NET, just ENABLE it, for via USB, depend on USBINSERT_FUNCTION and _USB3_INT_CORE1_
#else
#define MSDCVENDOR_NVT                  DISABLE //for via NET, just ENABLE it, for via USB, depend on USBINSERT_FUNCTION and _USB3_INT_CORE1_
#endif
#define MSDCVENDOR_UPDFW                DISABLE //enable it will cause disalbing MSDC and MTP/PTP/UAVC Mode, MSDCVENDOR_NVT must enable
#define MSDCVENDOR_IDENTITY_FILE        "A:\\MSDCNVT"
#if defined(_MODEL_IPCAM1_EVB_)
#define MSDCVENDOR_NVT_EXAM             MSDCVENDOR_NVT  //only for novatek internal use, and depend on MSDCVENDOR_NVT ENABLE
#else
#define MSDCVENDOR_NVT_EXAM             DISABLE //for customer, disable it to reduce memory
#endif
//==============================================================================
//   USBPRINT FUNCTIONS
//==============================================================================

//==============================================================================
//   USBPRINT FUNCTIONS
//==============================================================================

//==============================================================================
//   UI FUNCTIONS
//==============================================================================
//..............................................................................
// UI Resource Storage Config
#define _LANG_STORE_                    _INSIDECODE_
#define _JPG_STORE_                     _INSIDECODE_
#define SPECIAL_KEY_RESET_FUNC          DISABLE
#define EXTRA_MENU                      DISABLE

//==============================================================================
//   IPCam FUNCTION
//==============================================================================
//streamsender
#define IPCAM_STEAMSENDER_A_SLOT		30      //streamsender audio slot number
#define IPCAM_STEAMSENDER_V_SLOT		30      //streamsender video slot number

//VIDEO flow control
#define IPCAM_IPL_DIRECT 				ENABLE //ImagePipe Direct Input Mode. (Even ENABLE, IPL may still run DIRECT_MODE off due to >2112 with two stripe)
#define IPCAM_IPL_ONERAW 				DISABLE //ImagePipe One-Raw Input Mode.
#define IPCAM_RESET_FPS     			ENABLE  /// reset encode fps by dynamic ipl fps

#if (IPCAM_BUFFER_LEVEL == IPCAM_BUFFER_ALL) //buffer count: 3-4-3

#define IPCAM_MAIN_ONE_BUFFER			DISABLE //1st Encoder Path is running in Single-Buffer-Mode
#define IPCAM_DISP_ONE_BUFFER			DISABLE //Display Path is running in Single-Buffer-Mode
#define IPCAM_MAIN_TRIGGER				IPCAM_TRIGGER_SRC //1st Encoder Path trigger mode
#define IPCAM_DISP_TRIGGER				IPCAM_TRIGGER_SRC //Display Path trigger mode
#define IPCAM_MAIN_WAIT				ENABLE //1st Encoder Path will waiting for EncodeFinish
#define IPCAM_DISP_WAIT				ENABLE //Display Path will waiting for FlipFinish

#elif (IPCAM_BUFFER_LEVEL == IPCAM_BUFFER_TWO) //buffer count: 2-3-3

#define IPCAM_MAIN_ONE_BUFFER			DISABLE //1st Encoder Path is running in Single-Buffer-Mode
#define IPCAM_DISP_ONE_BUFFER			DISABLE //Display Path is running in Single-Buffer-Mode
#define IPCAM_MAIN_TRIGGER				IPCAM_TRIGGER_DEST //1st Encoder Path trigger mode
#define IPCAM_DISP_TRIGGER				IPCAM_TRIGGER_DEST //Display Path trigger mode
#define IPCAM_MAIN_WAIT				ENABLE //1st Encoder Path will waiting for EncodeFinish
#define IPCAM_DISP_WAIT				ENABLE //Display Path will waiting for FlipFinish

#elif (IPCAM_BUFFER_LEVEL == IPCAM_BUFFER_ONE) //buffer count: 1-1-3

#define IPCAM_MAIN_ONE_BUFFER			ENABLE //1st Encoder Path is running in Single-Buffer-Mode
#define IPCAM_DISP_ONE_BUFFER			ENABLE //Display Path is running in Single-Buffer-Mode
#define IPCAM_MAIN_TRIGGER				IPCAM_TRIGGER_SRC //1st Encoder Path trigger mode
#define IPCAM_DISP_TRIGGER				IPCAM_TRIGGER_SRC //Display Path trigger mode
#define IPCAM_MAIN_WAIT				DISABLE //1st Encoder Path will waiting for EncodeFinish
#define IPCAM_DISP_WAIT				DISABLE //Display Path will waiting for FlipFinish

#endif

//..............................................................................
//VIDEO functions
#if (DEMO_SENSOR6_CCIR == ENABLE)
//#define IPCAM_IN_DIS_FUNC               DISABLE
#define IPCAM_DIS_MV_FUNC			DISABLE //Digital Image Stabilzier: calculate motion vector
#define IPCAM_DIS_COMP_FUNC			DISABLE //DIS compensation: if set IPCAM_DIS_COMP_FUNC = enable, it should set IPCAM_DIS_MV_FUNC = TRUE, too
#else
//#define IPCAM_IN_DIS_FUNC               ENABLE
#define IPCAM_DIS_MV_FUNC			ENABLE //Digital Image Stabilzier: calculate motion vector
#define IPCAM_DIS_COMP_FUNC			ENABLE //DIS compensation: if set IPCAM_DIS_COMP_FUNC = enable, it should set IPCAM_DIS_MV_FUNC = TRUE, too
#endif

#define IPCAM_LCE					ENABLE
#define IPCAM_DEFOG_FUNC			ENABLE
#define IPCAM_SHDR_FUNC				DISABLE
#define IPCAM_SHDR_MAX_FRAME		2
#define IPCAM_SQUARE_3DNR				ENABLE
#define IPCAM_FD_FUNC                   ENABLE//Face Detection (if enable this, the block count of UIAPPIPCAM_BLOCK_BUFFER_INFO of this path, should add 1 to prevent get_blk fail)
#define MAX_FDNUM     				10      // param of IPCAM_FD_FUNC, max face number
#define IPCAM_FD_DRAW_VIDEO			DISABLE // param of IPCAM_FD_FUNC, draw face frame (just for demo!)
#define IPCAM_OD_FUNC                   DISABLE //OD
#define IPCAM_OD_DRAW_VIDEO				DISABLE // param of IPCAM_OD_FUNC

#define IPCAM_CDNN_FUNC                 DISABLE
#define IPCAM_CDNN2_FUNC                DISABLE
#define IPCAM_WDR_FUNC                  ENABLE // include LCE (Local Contrast Enhancement)

#define IPCAM_BC_FUNC					ENABLE //BC

#define IPCAM_LPR_FUNC                  DISABLE

#define IPCAM_DEPTH_FUNC				DISABLE
/////pull data to encode
#define IPCAM_USER_PULL_DATA            DISABLE
#define VDOENC_MULTI_TEMPORARY_LAYER ENABLE //Multi Temporary Layer
#define YUV_TV_RANGE 				DISABLE //YUV color space in tv range
#define VIDEO_FUNC_MJPG            	ENABLE
#define VIDEO_FUNC_H264            	ENABLE
#define VIDEO_FUNC_H265            	ENABLE
#define IPCAM_RAW_DATA				DISABLE

//..............................................................................
//AUDIO flow control
#define IPCAM_AUDIO_IN_FUNC			ENABLE //Audio Input
#define IPCAM_AUDIO_OUT_FUNC 			ENABLE //Audio Output

//..............................................................................
//AUDIO functions
#define AUDIO_FUNC_PCM                  ENABLE  //PCM
#define AUDIO_FUNC_AAC                  ENABLE //AAC
#define AUDIO_FUNC_PPCM                 DISABLE //PPCM
#define AUDIO_FUNC_ULAW                 ENABLE	//U-LAW
#define AUDIO_FUNC_ALAW                 ENABLE	//A-LAW

#define IPCAM_AEC_FUNC                  ENABLE //Audio Echo Cancellation
#define IPCAM_ANR_FUNC                  ENABLE //Audio Noise Reduction
#define IPCAM_AUDFILT_FUNC              DISABLE

//==============================================================================
//   NET FUNCTIONS
//==============================================================================
#if (!defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_))
#define _NETWORK_DEVICE_
#define _NETWORK_ON_WIFI_
#endif
#if !defined(_NVT_USB_4G_NONE_)
#define _NETWORK_DEVICE_
#define _NETWORK_ON_4G_
#endif
#if !defined(_NVT_ETHERNET_NONE_)
#define _NETWORK_DEVICE_
#define _NETWORK_ON_ETH_
#endif

#define MAC_APPEN_SSID                  ENABLE
#define ACL_TIME                        15000 //minmum 15 sec
#define WIFI_APP_MANUFACTURER           "NVT"
#define WIFI_APP_MODLE                  "CarDV_WiFi"
#define ONVIF_PROFILE_S                 DISABLE  //Enable it to cause NO CARD DETECTED.
#define ECOS_POWER_OFF_FLOW             DISABLE

//#NT#2016/10/21#YongChang Qui -begin
//#NT#wifi modules on EVB can't be powered off by default
#define WIFI_POWER_OFF_FLOW             DISABLE //Enabled only if ECOS_POWER_OFF_FLOW == DISABLE and DrvWiFi_PowerOff() actually powers off wifi module
//#NT#2016/10/21#YongChang Qui -end

#if (IPCAM_DEMO == ENABLE)
#define HFS_FUNC                        ENABLE
#else
#define HFS_FUNC                        DISABLE
#endif

#define HTTP_LIVEVIEW_FUNC              DISABLE
#define WIFI_AP_FUNC                    DISABLE
#define WIFI_FTP_FUNC                   DISABLE

#define HTTP_MJPG_W_4                   640
#define HTTP_MJPG_H_3                   480

#define HTTP_MJPG_W_16                  640
#define HTTP_MJPG_H_9                   360

#define AUTO_WIFI                       DISABLE

#define HTTP_LIVEVIEW                   1
#define RTSP_LIVEVIEW                   2
#define MOVIE_LIVEVIEW                  RTSP_LIVEVIEW

#if(WIFI_AP_FUNC==ENABLE)
#undef HTTP_LIVEVIEW_FUNC
#define HTTP_LIVEVIEW_FUNC              ENABLE
#define MOVIE_MULTI_RECORD_FUNC         ENABLE


#undef HTTP_MJPG_W_4
#undef HTTP_MJPG_H_3
#undef HTTP_MJPG_W_16
#undef HTTP_MJPG_H_9
#define HTTP_MJPG_W_4                   640
#define HTTP_MJPG_H_3                   480
#define HTTP_MJPG_W_16                  640
#define HTTP_MJPG_H_9                   360
#endif

#define WIFI_APP_MANUFACTURER           "NVT"
#define WIFI_APP_MODLE                  "CarDV_WiFi"
#define WIFI_APP_DOWNLOAD_URL           "http://115.29.201.46:8020/download/filedesc.xml"


#if defined(_CPU2_LINUX_)
#define UCTRL_FUNC                      ENABLE
#else
#define UCTRL_FUNC                      DISABLE
#endif
#define ONVIF_FUNC                      DISABLE  //CarDV-Linux is no using of ONVIF


#define WIFI_FINALCAM_APP_STYLE         DISABLE
#define WIFI_BG_COLOR                   14

#define WIFI_STA_RECONNECT_MAX_COUNT 5
#define WIFI_STA_RECONNECT_INTERVAL 300

#define YOUKU_SDK                       DISABLE
#define IOT_P2P_FUNC                    DISABLE
#define NETWORK_4G_MODULE               DISABLE

#if (YOUKU_SDK == ENABLE)
#define WIFI_MOVIE_FOLDER               "MOVIE_WIFI"
#define CUT_MOVIE_FOLDER                "MOVIE_CUT"
//the following two functions should be exclusive
#define SAVE_DUAL_STREAM_FUNC           ENABLE
#define CUT_MOVIE_FUNC                  DISABLE
#endif

#define MOVIE_OT_FUNC                   DISABLE
#define MOVIE_OT_DRAW_VIDEO             DISABLE



#define CURL_FUNC                       DISABLE

#define _DDD_FUNC_                      DISABLE
#define MOVIE_DDD_FUNC                  DISABLE
#define MOVIE_DDD_FD_FUNC               DISABLE
#define MOVIE_DDD_DRAW_VIDEO            DISABLE
#define MOVIE_DDD_SAVE_LOG              DISABLE
#define DDD_MODEL_IN_PSTORE             DISABLE

#define MOVIE_FTG_FUNC                  DISABLE
#define MOVIE_FTG_DRAW_VIDEO            DISABLE

//#NT#2017/04/28#KCHong -begin
//#NT# CNN Object detection
#define _OD_FUNC_                       DISABLE
//#NT#2017/04/28#KCHong -end

//#NT#2017/11/3#Ming Yang -begin
//#NT# Object detection
#define MOVIE_OD_FUNC                   DISABLE
#define MOVIE_OD_DRAW_VIDEO             DISABLE
#define MOVIE_OD_USE_ODT                DISABLE
//#NT#2017/11/3#Ming Yang -end

//#NT#2017/09/27#Bin Xiao -begin
//#NT# Object detection&DIS Tracking
#define MOVIE_ODT_FUNC                  DISABLE
#define MOVIE_ODT_DRAW_VIDEO            DISABLE
//#NT#2017/09/27#Bin Xiao -end

//#NT#2017/12/04#WY -begin
//#NT# OD PIPE
#define MOVIE_ODP_FUNC                  DISABLE
#define MOVIE_ODP_DRAW_VIDEO            DISABLE
//#NT#2017/12/04#WY -end

//#NT#2018/06/08#WY -begin
//#NT# OD TEST
#define MOVIE_ODP_TEST_FUNC             DISABLE
//#NT#2018/06/08#WY -end

//#NT#2018/03/12#Dan Guo -begin
//#NT# Face recognition
#define MOVIE_FR_FUNC                   DISABLE
#define MOVIE_FR_DRAW_VIDEO             DISABLE
//#NT#2018/03/12#Dan Guo -end

#define MOVIE_FDCNN_FUNC                DISABLE
#define MOVIE_FDCNN_DRAW_VIDEO          DISABLE
#define MOVIE_FDCNN_USE_ODT             DISABLE

#define USERLOG_FUNC                    DISABLE


// fileSize align function
#define FILESIZE_ALIGN_FUNC             DISABLE
#define PHOTO_FILESIZE_ALIGN_VALUE      2*1024*1024   //  2MB
#define FS_ALIGN_RESERVED_SIZE          (2 * MOVIE_FILESIZE_ALIGN_VALUE)

// mount sd-2 as "B:\\"
// To mount sd-2 as "B:\\" please do the following steps
// 1. set .int_id_dst[CC_CORE_MIPS1].Bit.bInt_ID_SDIO2 to TRUE
// 2. enable FS_DX_TYPE_DRIVE_B as DX_TYPE_CARD2
// 3. enable FS_MULTI_STRG_FUNC
// 4. NETWORK = NETWORK_NONE (Both uITRON and eCos)
#define FS_MULTI_STRG_FUNC              DISABLE
//MULTI_DRIVE
#define FS_DX_TYPE_DRIVE_A              DX_TYPE_CARD1
#define FS_DX_TYPE_DRIVE_B              USER_DX_TYPE_EMBMEM_FAT //FS_MULTI_STRG_FUNC must enable for FS_DX_TYPE_DRIVE_B

#define _3DNROUT_FUNC                   DISABLE

#define RTC_FUNC                        ENABLE

#if(IPCAM_MODE == ENABLE)
#define BOOT_RESET_MENU_INFO    DISABLE
#endif

#define DISPLAY_FAST_SWITCH DISABLE //fast swicth display device
#define DISPLAY_OSD2_FMT DISP_PXLFMT_INDEX8 // 256 colors
#define SPK_MASK 0x01
#define LOUT_MASK 0x02
#define LINE_INSERT_FUNCTION DISABLE //Auto Detect - LINE cable insert (audio out)
#define USE_DPOF DISABLE
#define BT_FUNC DISABLE
#define ECOS_DBG_MSG_FORCE_UART1_DIRECT DISABLE
#define USOCKET_CLIENT DISABLE
#define MOVIE_DISDSP_FUNC DISABLE
#define WIFI_UI_VER_1_0 0
#define WIFI_UI_VER_2_0 1
#define WIFI_UI_FLOW_VER WIFI_UI_VER_1_0
#define LOG_DBGINFO_IN_GPS_SECTION DISABLE
#define MOVIE_TD_SAVE_LOG DISABLE
#define MOVIE_SCD_FUNC DISABLE
#define UVC_MULTIMEDIA_FUNC DISABLE
#define PIP_VIEW_LR DISABLE   //(ENABLE: display 2 sensor by PipView in left, right view, side by side, DISABLE: normal pipview)
#define IQTOOL_FUNC DISABLE
#define GXGFX_ADVANCED_STRING_FUNC ENABLE
#define SMALL_CLONE_MOVIE DISABLE
#define CUSTOM_NAMING_RULE DISABLE
#define LINUX_CVHOST_FUNC DISABLE
#define AUDIO_PLAY_TEST DISABLE
#define FTG_SNAPSHOT_FUN DISABLE
#define FTG_MAX_SNAPSHOT_NUM 5
#define FTG_OVERRALL_MAX_BUF_SIZE 250*1024
#define FTG_FACE_MAX_BUF_SIZE 100*1024
#define FTG_MAX_SNAPSHOT_SIZE 1024*1024*25
#define FTG_MAX_FACE_PIC_NUM 3
#define RTSP_AUDIO_ENABLE DISABLE
#define AUDIO_CMD_UCTRL_ENABLE ENABLE
#define FACE_GRADE_TEST DISABLE
#define IPCAM_UVC_FUNC ENABLE
#define VIDEOSPRITEIPC ENABLE
#define HOT_PLUG_REMAIN_SIZE 0
#define _CVHOST_EX_                     DISABLE
#define _CVHOST_EX_ENDSP_               DISABLE
#define _NVTCV_EX_                      DISABLE
#define _NVTCV_EX_ENDSP_                DISABLE
#define MOVIE_MAX_CHANNEL				16
#define LINUX_MSDC DISABLE
#define THUMB_STAMP_FUNC				DISABLE



#endif //_PRJCFG_IPCAM_H_
