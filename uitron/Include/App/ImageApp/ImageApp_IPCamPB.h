/*
    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.

    @file       ImageApp_IPCamPB.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2018/04/16
*/

#ifndef IMAGEAPP_IPCAMPB_H
#define IMAGEAPP_IPCAMPB_H

#include "GxLib.h"
#include "ImageStream.h"


///< related units
#include "ImageUnit_StreamReceiver.h"
#include "ImageUnit_AudDec.h"
#include "ImageUnit_BsDemux.h"
#include "ImageUnit_VdoDec.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_AudOut.h"

extern void ImageApp_IPCamPB_Open(void);

extern void ImageApp_IPCamPB_Close(void);

extern void ImageApp_IPCamPB_AllStatusSet(ISF_PORT_STATE status);



#endif//IMAGEAPP_IPCAMPB_H