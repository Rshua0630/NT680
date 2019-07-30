#ifndef _UI_RESORUCE_H_
#define _UI_RESORUCE_H_
#include "PrjCfg.h"
#include "GxGfx.h"

#include "DemoKit_String.h"
#include "DemoKit_Image.h"
#include "DemoKit_Font.h"
#include "DemoKit_Palette.h"
#include "Demo_BigFont.h"
#include "Demo_SmallFont.h"

// Update FW check byte
#define FW_UPDATE_OFFSET        0x57    // value should match with loader
// Update FW Tag
#define FW_UPDATE_TAG           0x55    // value should match with loader

extern const unsigned char gDemo_ARString[];
extern const unsigned char gDemo_PERString[];
extern FONT *UIFontTable[];


///////////////////////////////////////////////////////////////////////////////
//
//  Palette

#define _OSD_INDEX_TRANSPART             0
#define _OSD_INDEX_BLUE                  1
#define _OSD_INDEX_WHITE                 2
#define _OSD_INDEX_BLACK                 3
#define _OSD_INDEX_GRAY75                8
#define _OSD_INDEX_DARKGRAY              9
#define _OSD_INDEX_ORANGE                15
#define _OSD_INDEX_GREEN                 13
#define _OSD_INDEX_GRAY50                4
#define _OSD_INDEX_RED                   14
#define _OSD_INDEX_LIGHTGREEN            12
#define _OSD_INDEX_YELLOW                16
#define _OSD_INDEX_GRAY                  6
//#define _OSD_INDEX_PINK                  64
//#define _OSD_INDEX_PURPLE                62
#define _OSD_INDEX_BLACK50               0x3E
#define _OSD_INDEX_BLACK75               0x3F
#define _OSD_INDEX_WHITE50               0x40


///////////////////////////////////////////////////////////////////////////////
//
//  Font


///////////////////////////////////////////////////////////////////////////////
//
//  Image (JPG)



extern BOOL UIRes_ChkWriteIMG(void);
extern void UIRes_InitReadIMG(void);
extern void UIRes_DumpMemIMG(void);
extern void UIRes_DeleteIMG(void);

extern UINT32 Get_IMGTable(UINT32 uhIndex);

//#NT#2011/02/14Steven feng-begin

//Define the number of Slideshow WAV files
#define SSWAV_NUM 4

extern BOOL UIRes_ChkWriteSSWAV(void);
extern void UIRes_InitReadSSWAV(int SSWAV_id);
//#NT#2011/02/15Steven feng-end

//#NT#2011/02/16#Lily Kao -begin
extern UINT32 UIRes_ReadSSWAV(int SSWAV_id, UINT32 uiPoolAddr, UINT32 uiSize);
//#NT#2011/02/16#Lily Kao -end


///////////////////////////////////////////////////////////////////////////////
//
//  String (LANG)


extern void UIRes_InitReadLang(int lang_id);
extern BOOL UIRes_ChkWriteLang(void);
extern void UIRes_SetLang(int lang_id);
extern void UIRes_DumpMemLang(void);
extern void UIRes_DeleteLang(void);

extern UINT32 Get_LanguageTable(void);

///////////////////////////////////////////////////////////////////////////////
//
//  Sound


///////////////////////////////////////////////////////////////////////////////
//
//  Photo Frame (JPG)


extern BOOL UIRes_ChkWritePHFE(void);
extern void UIRes_InitReadPHFE(void);
extern void UIRes_DeletePHFE(void);

#if 1
//temp define for compile
#define ICONID_NULL_TMP             0xFFFF
#define ICON_MENU_HDR               ICON_HDR
#define ICON_VOL_3                  ICONID_NULL
#define ICON_VOL_4                  ICONID_NULL
#define ICON_VOL_5                  ICONID_NULL
#define ICON_VOL_6                  ICONID_NULL
#define ICON_VOL_7                  ICONID_NULL
#define ICON_VOL_8                  ICONID_NULL
#define ICON_MENU_SETUP_ON          ICONID_NULL
#endif



#define STRID_USER_START              0xF000

//MOVIE_SIZE_TAG
enum _STRID_USER_ {
	STRID_MCTF =                  STRID_USER_START,
	STRID_EDGE,
	STRID_NR,
	STRID_WIFI_ETH,
	STRID_ISO6400,
	STRID_ISO12800,
	STRID_FRONT_2880x2160P50,                   /// UHD P50 (DAR)
	STRID_FRONT_3840x2160P30,                   /// UHD P30
	STRID_FRONT_2880x2160P24,                   /// UHD P24 (DAR)
	STRID_FRONT_2704x2032P60,                   /// 2.7K P60
	STRID_FRONT_2560x1440P80,                   /// QHD P80
	STRID_FRONT_2560x1440P60,                   /// QHD P60
	STRID_FRONT_2560x1440P30,                   /// QHD P30
	STRID_FRONT_2304x1296P30,                   /// 3MHD P30
	STRID_FRONT_1920x1080P120,                  /// FHD P120
	STRID_FRONT_1920x1080P96,                   /// FHD P96
	STRID_FRONT_1920x1080P60,                   /// FHD P60
	STRID_FRONT_1920x1080P30,                   /// FHD P30
	STRID_FRONT_1280x720P240,                   /// HD P240
	STRID_FRONT_1280x720P120,                   /// HD P120
	STRID_FRONT_1280x720P60,                    /// HD P60
	STRID_FRONT_1280x720P30,                    /// HD P30
	STRID_FRONT_848x480P30,                     /// WVGA P30
	STRID_FRONT_640x480P240,                    /// VGA P240
	STRID_FRONT_640x480P30,                     /// VGA P30
	STRID_FRONT_320x240P30,                     /// QVGA P30
	STRID_DUAL_1080P30_1080P30,                 /// FHD FHD
	STRID_TRI_1080P30, 							/// FHD x 3
	STRID_QUAD_1080P30,							/// FHD x 4
	STRID_CLONE_1920x1080P30_1920x1080P30,      /// FHD FHD
	STRID_CLONE_1920x1080P30_1280x720P30,       /// FHD HD
	STRID_CLONE_2560x1440P30_848x480P30,        /// QHD WVGA
	STRID_CLONE_2304x1296P30_848x480P30,        /// 3MHD WVGA
	STRID_CLONE_1920x1080P60_848x480P30,        /// FHD WVGA
	STRID_CLONE_1920x1080P60_640x360P30,        /// FHD VGA
	STRID_CLONE_1920x1080P30_848x480P30,        /// FHD WVGA
	STRID_CLONE_2048x2048P30_480x480P30,
	STRID_BOTH2,
	STRID_CAP_SIDE,
    STRID_CAP_BURST_30,
    STRID_5MWXH_USR,
    STRID_CODEC,
    STRID_MJPG,
    STRID_H264,
    STRID_H265,
	STRID_USER_END
};

extern char *UIRes_GetUserString(UINT32 TxtId);

#endif //_UI_RESORUCE_H_
