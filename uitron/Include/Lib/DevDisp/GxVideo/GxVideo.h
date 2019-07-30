/**
    Video device control.

    Video device control.

    @file       GxVideo.h
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _GX_VIDEO_H_
#define _GX_VIDEO_H_

#include "GxCommon.h"
#include "DispDef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
     @name DISPLAY device id
*/
//@{
#ifndef DOUT1
#define DOUT1                       0x00    ///< 1st device
#define DOUT2                       0x10    ///< 2nd device
#define DOUT_MASK                   0xf0    ///< output device mask
#endif
//@}


//--------------------------------------------------------------------------------------
//  device
//--------------------------------------------------------------------------------------

/**
     Initialize device.

     Initialize device.
     @note Nothing
*/
extern void GxVideo_InitDevice(UINT32 DevID);

/**
     Uninitialize device.

     Uninitialize device.
     @note Nothing
*/
extern void GxVideo_ExitDevice(UINT32 DevID);


/**
     @name Callback id
*/
//@{
#define DISPLAY_CB_PLUG             1   ///< device inserted
#define DISPLAY_CB_UNPLUG           2   ///< device removed
#define DISPLAY_CB_INITOK           4   ///< device initialize ok
#define DISPLAY_CB_DIR              8   ///< device change direction
//@}

/**
     Register callback of this lib.

     Register callback of this lib.
     @note Nothing

     @param[in] fpDispCB    Callback function pointer
*/
extern void GxVideo_RegCB(GX_CALLBACK_PTR fpDispCB);


/**
     Dump status of device.

     Dump status of device.
     @note Nothing
*/
extern void GxVideo_DumpInfo(void);

/**
     Open device.

     Open device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] NewDevObj       device object.
     @param[in] mode            device mode.
*/
extern void GxVideo_OpenDevice(UINT32 DevID, UINT32 NewDevObj, UINT32 mode);

/**
     Close device.

     Close device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
*/
extern void GxVideo_CloseDevice(UINT32 DevID);

/**
     Get current device object.

     Get current device object.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @return                    Device object
*/
extern UINT32 GxVideo_GetDevice(UINT32 DevID);

/**
     Get current device size of current mode.

     Get current device size of current mode, it will affect by ROTATE state!
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @return                    Device size
*/
extern ISIZE GxVideo_GetDeviceSize(UINT32 DevID);

/**
     Get current device aspect ratio

     Get current device aspect ratio, it will affect by ROTATE state!
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @return                    Aspect ratio
*/
extern USIZE GxVideo_GetDeviceAspect(UINT32 DevID);

/**
     Get given device first mode.

     Get given device first mode.
     @note Nothing

     @param[in] NewDevObj       Device object
     @param[in] mode            First mode
*/
void GxVideo_ConfigDeviceFirstMode(UINT32 NewDevObj, UINT32 mode);

/**
     Get given device last mode.

     Get given device last mode.
     @note Nothing

     @param[in] NewDevObj       Device object
     @return                    Last mode
*/
extern UINT32 GxVideo_QueryDeviceLastMode(UINT32 NewDevObj);

/**
     Get given device size of given mode.

     Get given device size of given mode.
     @note Nothing

     @param[in] NewDevObj       Device object
     @param[in] mode            Given mode
     @return                    Device size
*/
extern ISIZE GxVideo_QueryDeviceModeSize(UINT32 DevID, UINT32 NewDevObj, UINT32 mode);


/**
     @name Device control id
*/
//@{
#define DISPLAY_DEVCTRL_MODE        0  ///< get MODE, set MODE
#define DISPLAY_DEVCTRL_SLEEP       1  ///< get SLEEP, set SLEEP
#define DISPLAY_DEVCTRL_ACTIVE      2  ///< get ACTIVE, set ACTIVE
#define DISPLAY_DEVCTRL_BACKLIGHT   3  ///< get backlight, set backlight
#define DISPLAY_DEVCTRL_BRIGHTLVL   4  ///< get brightness, set brightness
#define DISPLAY_DEVCTRL_FORCEDETINS 5  ///< get force-detect insert, set force-detect insert
#define DISPLAY_DEVCTRL_FORCEDETDIR 6  ///< get force-detect dir, set force-detect dir
#define DISPLAY_DEVCTRL_DIRECT      7  ///< get rotate direct, set rotate direct
#define DISPLAY_DEVCTRL_SWAPXY      8  ///< get enable swap x and y, set enable swap x and y
//@}

/**
     @name State of DISPLAY_DEVCTRL_ACTIVE
*/
//@{
#define DISPLAY_ACTIVE_STOP         0 ///< stop
#define DISPLAY_ACTIVE_PLAY         1 ///< play
#define DISPLAY_ACTIVE_PAUSE        2 ///< pause
//@}

/**
     Set control to device.

     Set control to device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] data            Control state
     @param[in] value           Control value
*/
extern void GxVideo_SetDeviceCtrl(UINT32 DevID, UINT32 data, UINT32 value);

/**
     Get control to device.

     Get control to device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] data            Control state
     @return                    Control value
*/
extern UINT32 GxVideo_GetDeviceCtrl(UINT32 DevID, UINT32 data);

/**
     Detect insert of device.

     Detect insert of device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] context         Context for detect process
*/
extern void GxVideo_DetInsert(UINT32 DevID, UINT32 context);

/**
     Detect direct of device.

     Detect direct of device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] context         Context for detect process
*/
extern void GxVideo_DetDir(UINT32 DevID, UINT32 context);



#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_VIDEO_H_
