/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIFlow.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Flow Functions
                This file is the user interface ( for interchange flow control).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UIFLOW_H
#define _UIFLOW_H

#include "SysCfg.h"
//UIApp
#include "SysCommon.h"
#include "AppCommon.h"
#include "UIAppMovie.h"
#include "UIAppNetwork.h"

#include "PrjCfg.h"
//System
#include "UICommon.h"

//UI Display
#if defined(_UI_STYLE_CARDV_)
#include "CARDV/Resource/BG_Images.h"
#include "CARDV/Resource/BG_Opening.h"
#elif defined(_UI_STYLE_TOUCH_)
#include "TOUCHUI/Resource/BG_Images.h"
#include "TOUCHUI/Resource/BG_Opening.h"
#else
#include "Resource/BG_Images.h"
#include "Resource/BG_Opening.h"
#endif
#include "UIGraphics.h"
#include "UIResource.h"

//UI Info
#include "UISetup.h"
#include "UIStorageCheck.h"
#include "UICommon.h"
#include "SoundData.h"
#include "UIInfo/UIInfo.h"

#include "UIAppWiFiCmd.h"


#include "UIFlow/UIFlowCommon/UIFlowWndWrnMsgAPI.h"

#if !defined(_UI_STYLE_IPCAM_)
//UIFlow common
#include "UIFlow/UIFlowCommon/UIFlowWndWrnMsg.h"
#include "UIFlow/UIFlowCommon/UIFlowWndWaitMoment.h"
#include "UIFlow/UIFlowCommon/UIFlowWndUSB.h"
#include "UIFlow/UIFlowCommon/UIFlowWndUSBAPI.h"
#endif
//UIFlow movie
#include "UIFlow/UIFlowMovie/UIFlowMovieFuncs.h"
#include "UIFlow/UIFlowMovie/UIFlowMovieIcons.h"
#include "UIFlow/UIFlowMovie/UIFlowWndMovie.h"

#if !defined(_UI_STYLE_IPCAM_)

//UIFlow photo
#include "UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.h"
#include "UIFlow/UIFlowPhoto/UIFlowPhotoIcons.h"
#include "UIFlow/UIFlowPhoto/UIFlowWndPhoto.h"

//UIFlow play
#include "UIFlow/UIFlowPlay/UIFlowWndPlay.h"
#include "UIFlow/UIFlowPlay/UIFlowPlayFuncs.h"
#include "UIFlow/UIFlowPlay/UIFlowPlayIcons.h"
#include "UIFlow/UIFlowPlay/UIFlowWndPlayThumb.h"

//UIFlow USB
#include "UIFlow/UIFlowCommon/UIFlowWndUSB.h"
#include "UIFlow/UIFlowCommon/UIFlowWndUSBAPI.h"

#endif
#include "UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.h"


//UIMenu Common
#include "UIMenu/UIMenuCommon/TabMenu.h"
#include "UIMenu/UIMenuCommon/MenuId.h"

#if defined(_UI_STYLE_CARDV_)
#include "UIMenu/UIMenuCommon/MenuCommonItem.h"   //Item
#include "UIMenu/UIMenuCommon/MenuCommonOption.h" //Option
#elif defined(_UI_STYLE_SPORTCAM_)
#include "UIMenu/UIMenuCommon/MenuCommonItem.h"   //Item
#include "UIMenu/UIMenuCommon/MenuCommonOption.h" //Option
#include "UIMenu/UIMenuCommon/MenuCommon.h" //Menu Common Page
#include "UIMenu/UIMenuCommon/MenuMode.h" //Mode page
#include "UIMenu/UIMenuCommon/MenuCommonConfirm.h"
#elif defined(_UI_STYLE_IPCAM_)

#elif defined(_UI_STYLE_TOUCH_)
#include "UIMenu/UIMenuCommon/MenuMode.h"
#include "UIMenu/UIMenuCommon/MenuCommonItem.h"
#include "UIMenu/UIMenuCommon/MenuCommonOption.h"
#include "UIMenu/UIMenuCommon/MenuCommonConfirm.h"
#endif

//UIMenu movie
#include "UIMenu/UIMenuMovie/MenuMovie.h"
#if !defined(_UI_STYLE_IPCAM_)

//UIMenu photo
#include "UIMenu/UIMenuPhoto/MenuPhoto.h"
#include "UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.h"
#include "UIMenu/UIMenuPhoto/UIMenuWndPhotoExposure.h"
#include "UIMenu/UIMenuPhoto/UIMenuWndPhotoQuickSetting.h"
#include "UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.h"


//UIMenu playback
#include "UIMenu/UIMenuPlay/MenuPlayback.h"
#include "UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.h"
#include "UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.h"
#include "UIMenu/UIMenuPlay/UIMenuWndPlayDel.h"
#include "UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.h"

//UIMenu USB
#include "UIMenu/UIMenuUSB/UIMenuWndUSB.h"


//UIMenu Setup
#include "UIMenu/UIMenuSetup/MenuSetup.h"
#if defined(_UI_STYLE_CARDV_)
#else
#include "UIMenu/UIMenuSetup/MenuExtSetup.h"
#endif
#include "UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.h"
#include "UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.h"
#include "UIMenu/UIMenuSetup/UIMenuWndSetupFormat.h"
#include "UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.h"
#include "UIMenu/UIMenuSetup/UIMenuWndSetupVersion.h"
#endif
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif
//UIMenu WiFi
#include "UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.h"
#include "UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.h"
#include "UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.h"


#include "Platform.h"
#include "Debug.h"
#include "DCF.h"


#define TIMER_HALF_SEC               500
#define TIMER_ONE_SEC               1000
#define TIMER_TWO_SEC               2000


// Hideo test: general macros
#define MAKE_WORD(l, h)     ((UINT16)(((UINT8)(l)) | ((UINT16)((UINT8)(h))) << 8))
#define MAKE_LONG(l, h)     ((UINT32)(((UINT16)(l)) | ((UINT32)((UINT16)(h))) << 16))
#define LO_LONG(x)          ((UINT32)(x))
#define HI_LONG(x)          ((UINT32)((x>>32) & ((1ULL<<32) - 1)))
#define LO_WORD(l)          ((UINT16)(l))
#define HI_WORD(l)          ((UINT16)(((UINT32)(l) >> 16) & 0xFFFF))
#define LO_BYTE(w)          ((UINT8)(w))
#define HI_BYTE(w)          ((UINT8)(((UINT16)(w) >> 8) & 0xFF))
#define BIT(b, x)           ( ((1 << (b)) & (x)) >> (b) )
#define BITS(s, e, x)       ( (((((1 << ((e)-(s) + 1)) - 1) << (s)) & (x)) >> (s)) )
#define MAKE_EVEN(x)        ((x) & ~1)
#define MAKE_QUAD(x)        ((x) & ~3)

#define LIMIT(var,min,max)  (var) = ((var) < (min)) ? (min) : \
									(((var) > (max)) ? (max) : (var))

#define LIMITR(var,min,max) (var) = ((var) > (max)) ? (min) : \
									(((var) < (min)) ? (max) : (var))

#define SWAP_BYTES(x)       (MAKE_WORD( HI_BYTE(x), LO_BYTE(x)))

// Swaps words and bytes.
// For example, SWAP_WORDS(0x33221100) = 0x00112233
#define SWAP_WORDS(x)       (MAKE_LONG( MAKE_WORD( HI_BYTE(HI_WORD(x)),   \
										LO_BYTE(HI_WORD(x))),  \
										MAKE_WORD( HI_BYTE(LO_WORD(x)),   \
												LO_BYTE(LO_WORD(x)))))

#define ELEMS_OF_ARRAY(x)   (sizeof(x) / sizeof(x[0]))

#endif
