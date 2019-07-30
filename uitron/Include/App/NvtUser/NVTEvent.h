/**
    Header file of user event task

    Declare system events of user task.

    @file       NVTEvent.h
    @ingroup    mINvtUser
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef NVT_EVENT_H
#define NVT_EVENT_H

/**
    @addtogroup mINvtUser
*/

//@{

/**
    @name Class of events

    @note For #NVTEVT.
*/
//@{
//Class --------------------------------------------
#define MAJOR_EVENT_MASK                0xf0000000 ///< Mask for class of event
#define CLASS_EVENT_MASK                0xffff0000 ///< Mask for sub-class of event

#define APP_EVENT_MASK                  0x10000000 ///< Application class
#define SYS_EVENT_MASK                  0x10000000 ///<  System sub-class
#define DEV_EVENT_MASK                  0x11000000 ///<  Device sub-class
#define LIB_EVENT_MASK                  0x12000000 ///<  Library sub-class
#define APPUSER_EVENT_MASK              0x14000000 ///<  AppUser sub-class
#define UCTRL_EVENT_MASK                0x20000000 ///< Uctrl class
#define USER_EVENT_MASK                 0x80000000 ///< User-defined
//@}

/**
     Check is device class event.

     Check is device class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_DEV_EVENT(e)                 (((e) & CLASS_EVENT_MASK) == DEV_EVENT_MASK)

/**
     Check is library class event.

     Check is library class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_LIB_EVENT(e)                 (((e) & CLASS_EVENT_MASK) == LIB_EVENT_MASK)

/**
     Check is application class event.

     Check is application class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_APP_EVENT(e)                 (((e) & MAJOR_EVENT_MASK) == APP_EVENT_MASK)

/**
     Check is system class event.

     Check is system class event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_SYS_EVENT(e)                 (((e) & CLASS_EVENT_MASK) == SYS_EVENT_MASK)

/**
     Check is user-defined event.

     Check is user-defined event.
     @note Nothing.

     @param[in] e             event value.
*/
#define IS_USER_EVENT(e)                (((e) & CLASS_EVENT_MASK) == USER_EVENT_MASK)


/**
    @name Type of events

    @note For #NVTEVT.
*/
//@{
//Type ---------------------------------------------

#define UI_EVENT_BASE                   0x00008000 ///< UI general event
#define UI_WND_EVENT_BASE               0x00008100 ///< UI window event
#define UI_CTRL_EVENT_BASE              0x00008200 ///< UI control event

#define SYS_FLOW_EVENT_BASE             0x10010000 ///< System flow event event
#define SYS_MODE_EVENT_BASE             0x10020000 ///< System mode event event

#define DEV_SYS_EVENT_BASE              0x11000000 ///< System event event
#define DEV_STRG_EVENT_BASE             0x11010000 ///< Storage device event
#define DEV_VOUT_EVENT_BASE             0x11020000 ///< Display device event
#define DEV_AOUT_EVENT_BASE             0x11030000 ///< Audio-play device event
#define DEV_VIN_EVENT_BASE              0x11040000 ///< Sensor device event
#define DEV_USB_EVENT_BASE              0x11050000 ///< Usb device event
#define DEV_KEY_EVENT_BASE              0x11060000 ///< Key-input device event
#define DEV_TOUCH_EVENT_BASE            0x11070000 ///< Touch-panel device event
#define DEV_POWER_EVENT_BASE            0x11080000 ///< Power-provider device event
#define DEV_LENS_EVENT_BASE             0x11090000 ///< Lens device event
#define DEV_AIN_EVENT_BASE              0x110a0000 ///< Audio-record device event

#define LIB_TIMER_EVENT_BASE            0x12000000 ///< Timer library event
#define LIB_FILE_EVENT_BASE             0x12010000 ///< File library event
#define LIB_DISP_EVENT_BASE             0x12020000 ///< Display library event
#define LIB_SOUND_EVENT_BASE            0x12030000 ///< Sound library event
#define LIB_IPL_EVENT_BASE              0x12040000 ///< Image Pipe-line library event
//@}

/**
     Check is this type of event.

     Check is this type of event.
     @note Nothing.

     @param[in] e             event value.
     @param[in] base          event type.
*/
#define IS_TYPE_EVENT(e, base)          ( ((e)>=(base)) && ((e)<((base)+0x10000)) )


//Data ---------------------------------------------
/**
    @brief NVT event data type
*/
typedef UINT32 NVTEVT;


//Value --------------------------------------------
/**
    @brief NVT event values
*/
enum {
	NVTEVT_NULL                         = 0,   ///< NULL
};


/**
    @brief Define the maximum number of parameters carried with a event
*/
#define MAX_MESSAGE_PARAM_NUM      3


//@}

#endif //NVT_EVENT_H
