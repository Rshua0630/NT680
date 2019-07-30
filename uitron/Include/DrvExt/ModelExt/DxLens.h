/**
    DxLens.h

    Lens common interface
    @file       DxLens.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DXLENS_H_
#define _DXLENS_H_

/**
    @name lens operation error code.

     Error code to indicate condition.
*/
#define ERR_OK                     0
#define ERR_LENS_INIT_FAIL         (-5)
#define ERR_OPERATION_FAIL         (-1)
#define ERR_FUNC_PTR_NULL          (-2)
#define ERR_OPERATION_INTERRUPT    (-3)

/**
    @name shutter control input parameter.

     macro for shutter open/close usage.
*/
#define OPEN                    1
#define CLOSE                   0

/**
    @name zoom in/out control parameter.

     macro for zoom in/out control.
*/
#define ZOOM_IN                 1
#define ZOOM_OUT                0

/**
    @name focus tele/wide control parameter.

     macro for focus tele/wide control.
*/
#define FOCUS_TELE              1
#define FOCUS_WIDE              0

#define ZOOM_SECTION_WIDE           1
#define ZOOM_SECTION_TELE           11
#define ZOOM_SECTION_NUM            (ZOOM_SECTION_TELE + 1)

#define FOCUS_DISTANCE_TABLE_NUM    12


/**
     GPIOArray index.

     Index used to map Lens GPIO. Used in DrvLens_GetDevice().
*/
typedef enum {
    LENS_IO_MODE,

    LENS_IO_IN_0,
    LENS_IO_IN_1,
    LENS_IO_IN_2,
    LENS_IO_IN_3,
    LENS_IO_IN_4,
    LENS_IO_IN_5,
    LENS_IO_IN_6,
    LENS_IO_IN_7,

    LENS_IO_PWR,
    LENS_IO_RESET,
    LENS_IO_ZOOM_PI,
    LENS_IO_FOCUS_PI,
    LENS_IO_ZOOM_PIINT,
    LENS_IO_FOCUS_PIINT,
    LENS_IO_ZOOM_PR,
    LENS_IO_FOCUS_PR,
    LENS_IO_IRCUT_PR,

    LENS_IO_MOT_EXT1,
    LENS_IO_MOT_EXT2,
    LENS_IO_MOT_EXT3,
    LENS_IO_MAX,
    ENUM_DUMMY4WORD(LENS_MODULE_IO_MAP)
} LENS_MODULE_IO_MAP;

/**
    Lens speed configuration.
*/
typedef enum {
    LENS_SPEED_VERY_LOW  = 20,    // lens speed very low
    LENS_SPEED_LOW       = 40,    // lens speed low
    LENS_SPEED_MEDIUM    = 60,    // lens speed medium
    LENS_SPEED_HIGH      = 80,    // lens speed hight
    LENS_SPEED_VERY_HIGH = 100,   // lens speed very hight
    ENUM_DUMMY4WORD(LENS_SPEED_CATEGORY)
} LENS_SPEED_CATEGORY;

/**
     Enumeration to get af searching range.
*/
typedef enum {
    FOCUS_RANGE_START,            ///< start of searching range.
    FOCUS_RANGE_END,              ///< end of searching range.
    FOCUS_RANGE_BOTH,             ///< start & end of searching range.
    ENUM_DUMMY4WORD(FOCUS_RANGE)
} FOCUS_RANGE;

typedef enum {
    POS_GARAGE,                   ///< retract focus to garage position
    POS_HOME,                     ///< retract focus to home position
    ENUM_DUMMY4WORD(FOCUS_RETRACT_POSITION)
} FOCUS_RETRACT_POSITION;

/**
     Index for lens related IO control.
*/
typedef enum {
    MD_SIGNAL_STANDBY,            ///< used to power on/off motor driver
    MD_SIGNAL_ZPI,                ///< used to set/get ZPI signal
    MD_SIGNAL_ZPR,                ///< used to set/get ZPR signal
    MD_SIGNAL_FPI,                ///< used to set/get FPI signal
    ENUM_DUMMY4WORD(MD_SIGNAL)
} MD_SIGNAL;

/**
     Index for zoom control.
*/
typedef enum {
    MOTOR_ZOOM_BRAKE,             ///< used to break zoom
    MOTOR_ZOOM_FWD,               ///< used to forward zoom
    MOTOR_ZOOM_BWD,               ///< used to backward zoom
    MOTOR_ZOOM_OFF,               ///< used to turn off power for zoom
    MOTOR_ZOOM_SETSPEED,          ///< used to set zoom moving speed
    ENUM_DUMMY4WORD(MOTOR_ZOOM_ACT)
} MOTOR_ZOOM_ACT;

/**
     Index for zoom function.
*/
typedef enum {
    LENS_ZOOM_INIT,
    LENS_ZOOM_GO2_SECTION,
    LENS_ZOOM_GO2_POSITION,
    LENS_ZOOM_IN,
    LENS_ZOOM_OUT,
    LENS_ZOOM_RETRACT,
    LENS_ZOOM_FUNC_UNKNOWN,
} LENS_ZOOM_FUNC;

/**
     Index for focus control.
*/
typedef enum {
    MOTOR_FOCUS_FWD,              ///< used to forward focus
    MOTOR_FOCUS_BWD,              ///< used to backward focus
    MOTOR_FOCUS_EXCITE_ON,        ///< used to pre-excite on focus motor
    MOTOR_FOCUS_EXCITE_OFF,       ///< used to excite off focus motor
    MOTOR_FOCUS_CHECK_EXT,        ///< used to check excite on/off
    MOTOR_FOCUS_SETSPEED,         ///< used to set focus moving speed
    ENUM_DUMMY4WORD(MOTOR_FOCUS_ACT)
} MOTOR_FOCUS_ACT;

/**
     Index for focus function.
*/
typedef enum {
    LENS_FOCUS_INIT,
    LENS_FOCUS_GO2_POSITION,
    LENS_FOCUS_TELE,
    LENS_FOCUS_WIDE,
    LENS_FOCUS_RETRACT,
    LENS_FOCUS_FUNC_UNKNOWN,
} LENS_FOCUS_FUNC;

/**
     Index for aperture control.
*/
typedef enum {
    MOTOR_APERTURE_NORMAL,        ///< reserved
    MOTOR_APERTURE_DUMMYLOAD,     ///< reserved
    MOTOR_APERTURE_FWD,           ///< used to forward aperture
    MOTOR_APERTURE_BWD,           ///< used to backward aperture
    MOTOR_APERTURE_EXCITE_ON,     ///< used to pre-excite on aperture motor
    MOTOR_APERTURE_EXCITE_OFF,    ///< used to excite off aperture motor
    MOTOR_APERTURE_RESET,         ///< used to reset aperture position
    ENUM_DUMMY4WORD(MOTOR_APERTURE_ACT)
} MOTOR_APERTURE_ACT;

typedef enum {
    APERTURE_DUMMYLOAD_START,     ///< reserved
    APERTURE_DUMMYLOAD_END,       ///< reserved
    ENUM_DUMMY4WORD(APERTURE_DUMMYLOAD_ACT)
} APERTURE_DUMMYLOAD_ACT;

typedef enum {
    IRIS_POS_BIG  = 0,
    IRIS_POS_F1_8 = 0,            ///< Fno. 1.8
    IRIS_POS_F2_0,                ///< Fno. 2.0
    IRIS_POS_F2_8,                ///< Fno. 2.8
    IRIS_POS_F4_0,                ///< Fno. 4.0
    IRIS_POS_F5_6,                ///< Fno. 5.6
    IRIS_POS_F8_0,                ///< Fno. 8.0
    IRIS_POS_SMALL = IRIS_POS_F8_0,
    IRIS_POS_CLOSE,               ///< reserved
    IRIS_POS_RESET,               ///< IRIS is in reset position
    IRIS_POS_MAX,
    ENUM_DUMMY4WORD(IRIS_POS)
} IRIS_POS;

/**
     Index for shutter control.
*/
typedef enum {
    MOTOR_SHUTTER_NORMAL,         ///< normal opeartion for shutter
    MOTOR_SHUTTER_CAP,            ///< capture operation for shutter
    MOTOR_SHUTTER_DUMMYLOAD,      ///< reserved
    MOTOR_SHUTTER_PSEUDO_OPEN,    ///< reserved
    MOTOR_SHUTTER_PSEUDO_CLOSE,   ///< reserved
    ENUM_DUMMY4WORD(MOTOR_SHUTTER_ACT)
} MOTOR_SHUTTER_ACT;

typedef enum {
    SHUTTER_DUMMYLOAD_START,      ///< reserved
    SHUTTER_DUMMYLOAD_END,        ///< reserved
    ENUM_DUMMY4WORD(SHUTTER_DUMMYLOAD_ACT)
} SHUTTER_DUMMYLOAD_ACT;

typedef enum {
    SHUTTER_POS_CLOSE = 0,        ///< used to close shutter
    SHUTTER_POS_OPEN,             ///< used to open shutter
    SHUTTER_POS_MAX,
    ENUM_DUMMY4WORD(SHUTTER_POS)
} SHUTTER_POS;

/**
     Index for IRCUT control.
*/
typedef enum {
    MOTOR_IRCUT_OPEN,
    MOTOR_IRCUT_CLOSE,
    ENUM_DUMMY4WORD(MOTOR_IRCUT_ACT)
} MOTOR_IRCUT_ACT;

typedef enum {
    IRCUT_POS_OPEN = 0,           ///< used to open IRCUT
    IRCUT_POS_CLOSE,              ///< used to close IRCUT
    IRCUT_POS_MAX,
    ENUM_DUMMY4WORD(IRCUT_POS)
} IRCUT_POS;

/**
     Index for lens support function.
*/
typedef enum {
    LENS_SUPPORT_NONE     = 0x00000000,
    LENS_SUPPORT_FOCUS    = 0x00000001,
    LENS_SUPPORT_ZOOM     = 0x00000002,
    LENS_SUPPORT_APERTURE = 0x00000004,
    LENS_SUPPORT_SHUTTER  = 0x00000008,
    LENS_SUPPORT_IRCUT    = 0x00000010,
} LENS_SUPPORT_PROPERTY;

typedef struct {
    INT32 iZoomPosition;
    INT32 iFocusMin;
    INT32 iFocusMax;
} LENS_ZOOM_FOCUS_TAB;

typedef struct {
    UINT32 uiTabIdx;
    LENS_ZOOM_FOCUS_TAB ZoomFocusTab;
} LENS_ZOOM_FOCUS_ITEM;

typedef struct {
    INT32 iLensPosition[FOCUS_DISTANCE_TABLE_NUM];
} LENS_FOCUS_DISTANCE_TAB;

/**
    The callback function of lens
*/
typedef void (*FPLENS_CALLBACK)(UINT32 msg_id, UINT32 *param);


//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------
/**
    Lens device object.
*/
typedef struct {
    UINT32 *pGPIOArray;           ///< GPIO pin array for HW connection between NT9666x and lens driver
} LENS_DEVICE_OBJ, *PLENS_DEVICE_OBJ;

/**
    The configuration object for motor.
*/
typedef struct {
    // common
    void   (*init)(PLENS_DEVICE_OBJ);
    void   (*setSignal)(MD_SIGNAL, UINT32);                         ///< set signal
    UINT32 (*getSignal)(MD_SIGNAL);                                 ///< get signal
    UINT32 (*getBusyState)(void);                                   ///< zoom & focus motor action
    // zoom
    UINT32 (*zoom_getSpeed)(void);                                  ///< get zoom speed
    void   (*zoom_setState)(MOTOR_ZOOM_ACT, UINT32);                ///< zoom action
    // focus
    UINT32 (*focus_getSpeed)(void);                                 ///< get focus speed
    BOOL   (*focus_setState)(MOTOR_FOCUS_ACT, UINT32);              ///< focus action
    // aperture
    void   (*aperture_setState)(MOTOR_APERTURE_ACT, UINT32);        ///< aperture action
    //shutter
    void   (*shutter_setState)(MOTOR_SHUTTER_ACT, UINT32);          ///< shutter action
    // IR cut
    void   (*ircut_setState)(MOTOR_IRCUT_ACT, UINT32);              ///< IR cut action
} MOTOR_TAB, *PMOTOR_TAB;

/**
    For DrvExt layer APIs to invoke LibExt layer APIs(LensCtrl_Open).
*/
typedef struct {
    void (*lenstsk_setSignal)(MD_SIGNAL, BOOL);
    void (*lenstsk_zoom_setFunc)(LENS_ZOOM_FUNC, UINT32);
    void (*lenstsk_focus_setFunc)(LENS_FOCUS_FUNC, UINT32);
    void (*lenstsk_aperture_setState)(MOTOR_APERTURE_ACT, UINT32);
    void (*lenstsk_shutter_setState)(MOTOR_SHUTTER_ACT, UINT32);
    void (*lenstsk_ircut_setState)(MOTOR_IRCUT_ACT, UINT32);
} LENSCTRL_TASK_API, *PLENSCTRL_TASK_API;

/**
    The configuration object for lens.
*/
typedef struct {
    // common
    void     (*init)(PLENS_DEVICE_OBJ);                             ///< init lens and mount IO device object
    void     (*setSignal)(MD_SIGNAL, UINT32);                       ///< set signal
    UINT32   (*getSignal)(MD_SIGNAL);                               ///< get signal
    UINT32   (*getBusyState)(void);                                 ///< zoom & focus motor state
    // zoom
    INT32    (*zoom_init)(UINT32);                                  ///< init zoom
    UINT32   (*zoom_getSpeed)(void);                                ///< get zoom speed
    void     (*zoom_setSpeed)(LENS_SPEED_CATEGORY);                 ///< set zoom speed
    UINT32   (*zoom_getMinSection)(void);                           ///< get minimum zoom section
    UINT32   (*zoom_getMaxSection)(void);                           ///< get maximum zoom section
    UINT32   (*zoom_getSection)(void);                              ///< get current zoom section
    INT32    (*zoom_getSectionPosition)(void);                      ///< get current zoom section step
    INT32    (*zoom_getPosition)(void);                             ///< get current zoom position
    void     (*zoom_setPostion)(INT32, INT32);                      ///< set current zoom position to desired one
    INT32    (*zoom_go2Section)(UINT32);                            ///< go to specific zoom section
    INT32    (*zoom_go2Position)(INT32);                            ///< go to specific zoom position
    INT32    (*zoom_press)(UINT32);                                 ///< zoom in/out
    UINT32   (*zoom_release)(void);                                 ///< zoom stop
    INT32    (*zoom_retract)(void);                                 ///< zoom retract
    // focus
    INT32    (*focus_init)(void);                                   ///< init focus
    UINT32   (*focus_getSpeed)(void);                               ///< get focus speed
    void     (*focus_setSpeed)(LENS_SPEED_CATEGORY);                ///< set focus speed
    UINT32   (*focus_getFocalLength)(UINT32);                       ///< get focal length
    void     (*focus_setFocalLength)(UINT32, UINT32);               ///< set focal length
    UINT32   (*focus_getDistTable)(UINT32);                         ///< get table of object distance
    INT32    (*focus_getDefaultTable)(UINT32, UINT32);              ///< get default table of focus step for each zoom section
    INT32    (*focus_getTable)(UINT32, UINT32);                     ///< get table of focus step for each zoom section
    INT32    (*focus_getRange)(FOCUS_RANGE);                        ///< get focus search range for each zoom section
    void     (*focus_doExc)(BOOL, UINT32);                          ///< focus with pre-excitation or post-excitation
    INT32    (*focus_getPostion)(void);                             ///< get current focus position
    void     (*focus_setPostion)(INT32, INT32);                     ///< set current focus position to desired one
    INT32    (*focus_go2Position)(INT32, BOOL);                     ///< go to specific focus position
    INT32    (*focus_press)(UINT32);                                ///< focus tele/wide
    UINT32   (*focus_release)(void);                                ///< focus stop
    INT32    (*focus_retract)(FOCUS_RETRACT_POSITION);              ///< focus retract
    // aperture
    void     (*aperture_init)(void);                                ///< init aperture
    UINT32   (*aperture_getFNo)(UINT32, UINT32);                    ///< get f number
    IRIS_POS (*aperture_getPosition)(void);                         ///< get current aperture position
    void     (*aperture_go2Position)(IRIS_POS);                     ///< go to specific aperture position
    // shutter
    void     (*shutter_setState)(MOTOR_SHUTTER_ACT, UINT32);        ///< set shutter state
    // IR cut
    void     (*ircut_setState)(MOTOR_IRCUT_ACT, UINT32);            ///< set IR cut state
    // misc
    INT32    (*lensRetract)(void);                                  ///< lens retract
    INT32    (*lensReset)(void);                                    ///< lens reset
    INT32    (*lensPowerOff)(void);                                 ///< turn off lens power
    UINT32   (*lensGetInfo)(void);                                  ///< get lens related info
    void     (*makeAFTable)(void);                                  ///< calculate AF table
    INT32    (*setCaliData)(INT16 *, UINT32);                       ///< load focus calibration data
    UINT32   (*getZoomFocusTable)(UINT32, LENS_ZOOM_FOCUS_TAB *);   ///< get zoom vs focus_range table
    UINT32   (*setZoomFocusTable)(UINT32, LENS_ZOOM_FOCUS_TAB *);   ///< set zoom vs focus_range table

    PMOTOR_TAB pMotor;                                              ///< motor object pointer
    PLENSCTRL_TASK_API pLensCtrlTsk;                                ///< for DrvExt layer APIs to invoke LibExt layer APIs(LensCtrl_Open).
} LENS_TAB, *PLENS_TAB;

/**
    The configuration object for Lens Control Object (Lens_ModuleInit()).
*/
typedef struct {
    PLENS_TAB        pLens;                                         ///< mount lens object
    PLENS_DEVICE_OBJ pLensDev;                                      ///< mount lens IO object
    void (*TaskCB)(UINT32 MessageID, UINT32 *Parameter);            ///< callback to notify zoom section was changed
    void (*APICB) (UINT32 MessageID, UINT32 *Parameter);            ///< callback to notify UI current state of lens
    UINT32 semid;                                                   ///< semaphore used for lens module
} LENSCTRL_OBJ, *PLENSCTRL_OBJ;


/**
    Power on lens

    @return void
*/
void DrvLens_PowerOn(void);

/**
    Power off lens

    @return void
*/
void DrvLens_PowerOff(void);

/**
    To retrieve lens object.

    @return PLENS_TAB    Point to lens object
*/
extern PLENS_TAB DrvLens_GetLens(void);

/**
    To retrieve motor object.

    @return PMOTOR_TAB    Point to motor object
*/
extern PMOTOR_TAB DrvLens_GetMotor(void);

/**
    To retrieve IO device object.

    @return PLENS_DEVICE_OBJ    Point to IO device object
*/
PLENS_DEVICE_OBJ DrvLens_GetDevice(void);

/**
    To retrieve lens control object.

    @return PLENSCTRL_OBJ    Point to lens control object
*/
extern PLENSCTRL_OBJ DrvLens_GetLensCtrl(void);

extern void DrvLens_RegLensCB(void *plens_task_cb, void *plens_api_cb);


#endif    // _DXLENS_H_


