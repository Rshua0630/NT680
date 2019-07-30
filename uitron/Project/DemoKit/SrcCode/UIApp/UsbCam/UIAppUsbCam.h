/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIAppUsbCam.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of UIAppUsbCam_Exe.c
                Header file of UIAppUsbCam_Exe.c

    @note       Nothing.

    @version    V1.00.000
    @author     Chris Hsu
    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _UIAPPUSBCAM_H
#define _UIAPPUSBCAM_H

#include "NVTEvent.h"

#include "AppLib.h"
#include "UIAppCommon.h"


#define NVT_UI_UVAC_RESO_CNT                            4
#define NVT_UI_UVAC_RESO_IDX_DEF                        0    //1080P 30fps
#define NVT_UI_UVAC_RESO_IDX_DUALSNESOR_DEF             2    //848x480P 30fps
//#define NVT_UI_UVAC_RESO_IDX_DEF                      1    //720P 30fps
#define NVT_UI_UVAC_AUD_SAMPLERATE_CNT                  1
#define NVT_UI_UVAC_VENDCMD_CURCMD                      0x01
#define NVT_UI_UVAC_RESO_INTERNAL                       DISABLE


#define NVT_UI_FREQUENCY_16K                    0x003E80   //16k
#define NVT_UI_FREQUENCY_32K                    0x007D00   //32k
#define NVT_UI_FREQUENCY_48K                    0x00BB80   //48k


extern VControl CustomUSBPCCObjCtrl;

#endif
//@}
