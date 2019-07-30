/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppCommon.h
    @ingroup    mIPRJAPUIFlow

    @brief      App Common Interfaces
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _APPCOMMON_H
#define _APPCOMMON_H

//Built-in App Object
#include "AppLib.h"         //include EVENT of Common Obj

//Mode
#include "UIMode.h"

//M (Info)

#include "ProjectInfo.h"
#if(MOVIE_MODE==ENABLE)
#include "UIMovieInfo.h"
#include "UIMovieMapping.h"
#endif
#if(PHOTO_MODE==ENABLE)
#include "UIPhotoInfo.h"
#include "UIPhotoMapping.h"
#endif
#if(PLAY_MODE==ENABLE)
#include "UIPlayInfo.h"
#include "UIPlayMapping.h"
#endif
#if(IPCAM_FUNC!= ENABLE)
#include "DateTimeInfo.h"
#include "GSInfo.h"
#include "UIInfo.h"
#endif

//V (App)
#if(MOVIE_MODE==ENABLE)
#include "UIAppMovie.h"      //include EVENT of Movie App
#endif
#if(PHOTO_MODE==ENABLE)
#include "UIAppPhoto.h"      //include EVENT of Photo App
#endif
#if(PLAY_MODE==ENABLE)
#include "UIAppPlay.h"       //include EVENT of Play App
#include "UIAppMoviePlay.h"       //include EVENT of Play App
#endif
#if(USB_MODE==ENABLE)
#include "UIAppUsbCam.h"
#include "UIAppUsbCharge.h"
#include "UIAppUsbDisk.h"
#include "UIAppUsbPrint.h"   //include EVENT of Print App
#endif
#if(UI_FUNC==ENABLE)
#include "UIBackgroundObj.h" //include EVENT of Worker Task
#endif
#include "UISetup.h"         //include EVENT of System Setup

#endif //_APPCOMMON_H_

