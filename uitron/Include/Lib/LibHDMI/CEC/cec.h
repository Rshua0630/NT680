/**
    Header file for CEC Task

    Header file for CEC Task.

    @file       cec.h
    @ingroup    mIDISPHDMITx
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef  _CEC_H
#define  _CEC_H

/**
    @addtogroup mIDISPHDMITx
*/
//@{

/**
    HDMI CEC Supported Command Identification

    This definition is used to configure each of the CEC command's supporting abilities due to customization.
    This is used in Cec_SetCommandSupport() as the input parameter to assign the CEC command is supported
    by the CEC Task. Each of the CEC command supporting options must be filled carefully and matching exactly with the
    HDMI CEC Compliance Test Capability Descriptor File(CDF).
*/
typedef enum {
	CECCMD_SUP_ACTIVE_SOURCE,               ///< Default value is  TX_ONLY.
	CECCMD_SUP_IMAGE_VIEWON,                ///< Default value is  TX_ONLY.
	CECCMD_SUP_TEXT_VIEWON,                 ///< Default value is NO-SUPPORT.
	CECCMD_SUP_INACTIVE_SOURCE,             ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REQUEST_ACTIVE_SOURCE,       ///< Default value is   RX_ONLY.
	CECCMD_SUP_ROUTING_CHANGE,              ///< Default value is NO-SUPPORT.
	CECCMD_SUP_ROUTING_INFO,                ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_STREAM_PATH,             ///< Default value is NO-SUPPORT.
	CECCMD_SUP_STANDBY,                     ///< Default value is   RX_ONLY.
	CECCMD_SUP_RECORD_OFF,                  ///< Default value is NO-SUPPORT.
	CECCMD_SUP_RECORD_ON,                   ///< Default value is NO-SUPPORT.
	CECCMD_SUP_RECORD_STATUS,               ///< Default value is NO-SUPPORT.
	CECCMD_SUP_RECORD_TVSCREEN,             ///< Default value is NO-SUPPORT.
	CECCMD_SUP_CLEAR_ANALOG_TIMER,          ///< Default value is NO-SUPPORT.
	CECCMD_SUP_CLEAR_DIGITAL_TIMER,         ///< Default value is NO-SUPPORT.
	CECCMD_SUP_CLEAR_EXTERNAL_TIMER,        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_ANALOG_TIMER,            ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_DIGITAL_TIMER,           ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_EXTERNAL_TIMER,          ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_TIMER_PROGRAM_TITLE,     ///< Default value is NO-SUPPORT.
	CECCMD_SUP_TIMER_CLEARED_STATUS,        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_TIMER_STATUS,                ///< Default value is NO-SUPPORT.
	CECCMD_SUP_CEC_VERSION,                 ///< Default value is NO-SUPPORT.
	CECCMD_SUP_GET_CEC_VERSION,             ///< Default value is NO-SUPPORT.
	CECCMD_SUP_GIVE_PHYSICAL_ADDR,          ///< Default value is   RX_ONLY.
	CECCMD_SUP_GET_MENU_LANGUAGE,           ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REPORT_PHY_ADDR,             ///< Default value is  TX_ONLY.
	CECCMD_SUP_SET_MENU_LANGUAGE,           ///< Default value is NO-SUPPORT.
	CECCMD_SUP_DECK_CONTROL,                ///< Default value is NO-SUPPORT.
	CECCMD_SUP_DECK_STATUS,                 ///< Default value is NO-SUPPORT.
	CECCMD_SUP_GIVE_DECK_STATUS,            ///< Default value is NO-SUPPORT.
	CECCMD_SUP_PLAY,                        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_GIVE_TUNER_DEVICE_STATUS,    ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SELECT_ANALOG_SERVICE,       ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SELECT_DIGITAL_SERVICE,      ///< Default value is NO-SUPPORT.
	CECCMD_SUP_TUNER_DEVICE_STATUS,         ///< Default value is NO-SUPPORT.
	CECCMD_SUP_TUNER_STEP_DECREMENT,        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_TUNER_STEP_INCREMENT,        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_DEVICE_VENDORID,             ///< Default value is NO-SUPPORT.
	CECCMD_SUP_GIVE_DEVICE_VENDORID,        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_VENDOR_COMMAND,              ///< Default value is NO-SUPPORT.
	CECCMD_SUP_VENDOR_COMMAND_WITH_ID,      ///< Default value is NO-SUPPORT.
	CECCMD_SUP_VENDOR_RM_BUTTON_DOWN,       ///< Default value is NO-SUPPORT.
	CECCMD_SUP_VENDOR_RM_BUTTON_UP,         ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_OSD_STRING,              ///< Default value is NO-SUPPORT.
	CECCMD_SUP_GIVE_OSD_NAME,               ///< Default value is   RX_ONLY.
	CECCMD_SUP_SET_OSD_NAME,                ///< Default value is  TX_ONLY.
	CECCMD_SUP_MENU_REQUEST,                ///< Default value is NO-SUPPORT.
	CECCMD_SUP_MENU_STATUS,                 ///< Default value is  TX_ONLY.
	CECCMD_SUP_USERCTL_PRESSED,             ///< Default value is   RX_ONLY.
	CECCMD_SUP_USERCTL_RELEASED,            ///< Default value is   RX_ONLY.
	CECCMD_SUP_GIVE_DEVICE_POWER_STATUS,    ///< Default value is   RX_ONLY.
	CECCMD_SUP_REPORT_POWER_STATUS,         ///< Default value is  TX_ONLY.
	CECCMD_SUP_FEATURE_ABORT,               ///< Default value is    SUPPORT.
	CECCMD_SUP_ABORT,                       ///< Default value is   RX_ONLY.  **
	CECCMD_SUP_GIVE_AUDIO_STATUS,           ///< Default value is NO-SUPPORT.
	CECCMD_SUP_GIVE_SYS_AUDIO_MODE_STS,     ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REPORT_AUDIO_STATUS,         ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REPORT_SHORT_AUD_DESCRI,     ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REQUEST_SHORT_AUD_DESCRI,    ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_SYS_AUDIO_MODE,          ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SYS_AUD_MODE_REQUEST,        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SYS_AUD_MODE_STATUS,         ///< Default value is NO-SUPPORT.
	CECCMD_SUP_SET_AUDIO_RATE,              ///< Default value is NO-SUPPORT.
	CECCMD_SUP_INITIATE_ARC,                ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REPORT_ARC_INITIATED,        ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REPORT_ARC_TERMINATED,       ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REQUEST_ARC_INITIATION,      ///< Default value is NO-SUPPORT.
	CECCMD_SUP_REQUEST_ARC_TERMINATION,     ///< Default value is NO-SUPPORT.
	CECCMD_SUP_TERMINATE_ARC,               ///< Default value is NO-SUPPORT.
	CECCMD_SUP_CDC,                         ///< Default value is NO-SUPPORT.

	CECCMD_SUP_NUM,                         // Total Number of CEC Commands.
	ENUM_DUMMY4WORD(CECCMD_SUPPORT_ID)
} CECCMD_SUPPORT_ID;

/**
    CEC Command Supporting Options

    This definition is used in Cec_SetCommandSupport() as the input parameter to assign the CEC command is supported
    by the CEC Task.
*/
typedef enum {
	CECCMD_SUP_TYPE_SUPPORT         = 0x00, ///< The specified CEC command is supported by the CEC Task for both the TX/RX directions.
	CECCMD_SUP_TYPE_RX_ONLY         = 0x20, ///< The specified CEC command is supported at receiving only. Transmitting is not supported.
	CECCMD_SUP_TYPE_TX_ONLY         = 0x08, ///< The specified CEC command is supported at transmitting only. Receiving is not supported.
	CECCMD_SUP_TYPE_NOSUPPORT       = 0x10, ///< Both the transmitting and receiving behavior is not supported by the CEC Task.

	CECCMD_SUP_TYPE_ALL             = 0x38,
	ENUM_DUMMY4WORD(CECCMD_SUPPORT_TYPE)
} CECCMD_SUPPORT_TYPE;


/**
    CEC Remote Control Pass Through Feature Command Code

    CEC User Control Codes Define
*/
typedef enum {
	CECKEY_SELECT       = 0x00, ///< CEC Key 0x00: Select
	CECKEY_UP,                  ///< CEC Key 0x01: Up
	CECKEY_DOWN,                ///< CEC Key 0x02: Down
	CECKEY_LEFT,                ///< CEC Key 0x03: Left
	CECKEY_RIGHT,               ///< CEC Key 0x04: Right
	CECKEY_RIGHTUP,             ///< CEC Key 0x05: Right-Up
	CECKEY_RIGHTDOWN,           ///< CEC Key 0x06: Right-Down
	CECKEY_LEFTUP,              ///< CEC Key 0x07: Left-Up
	CECKEY_LEFTDOWN,            ///< CEC Key 0x08: Left-Down
	CECKEY_ROOT_MENU,           ///< CEC Key 0x09: Root Menu
	CECKEY_SETUP_MENU,          ///< CEC Key 0x0A: Setup Menu
	CECKEY_CONTENT_MENU,        ///< CEC Key 0x0B: Content Menu
	CECKEY_FAVORITE_MENU,       ///< CEC Key 0x0C: Favorite Menu
	CECKEY_EXIT,                ///< CEC Key 0x0D: Exit
	CECKEY_MEDIA_TOPMENU = 0x10, ///< CEC Key 0x10: Media Top Menu
	CECKEY_MEDIA_CONTEXT,       ///< CEC Key 0x11: Media Context - Sensitive Menu
	CECKEY_NUMBERENTRYMODE = 0x1D, ///< CEC Key 0x1D: Number Entry Mode
	CECKEY_NUMBER_11,           ///< CEC Key 0x1E: Number 11
	CECKEY_NUMBER_12,           ///< CEC Key 0x1F: Number 12
	CECKEY_NUMBER_0,            ///< CEC Key 0x20: Number 0 or Number 10
	CECKEY_NUMBER_1,            ///< CEC Key 0x21: Number 1
	CECKEY_NUMBER_2,            ///< CEC Key 0x22: Number 2
	CECKEY_NUMBER_3,            ///< CEC Key 0x23: Number 3
	CECKEY_NUMBER_4,            ///< CEC Key 0x24: Number 4
	CECKEY_NUMBER_5,            ///< CEC Key 0x25: Number 5
	CECKEY_NUMBER_6,            ///< CEC Key 0x26: Number 6
	CECKEY_NUMBER_7,            ///< CEC Key 0x27: Number 7
	CECKEY_NUMBER_8,            ///< CEC Key 0x28: Number 8
	CECKEY_NUMBER_9,            ///< CEC Key 0x29: Number 9
	CECKEY_DOT,                 ///< CEC Key 0x2A: Dot
	CECKEY_ENTER,               ///< CEC Key 0x2B: Enter
	CECKEY_CLEAR,               ///< CEC Key 0x2C: Clear
	CECKEY_NEXT_FAVORITE = 0x2F, ///< CEC Key 0x2F: Next Favorite
	CECKEY_CHANNEL_UP,          ///< CEC Key 0x30: Channel Up
	CECKEY_CHANNEL_DOWN,        ///< CEC Key 0x31: Channel Down
	CECKEY_PREVIOUS_CHANNEL,    ///< CEC Key 0x32: Previous Channel
	CECKEY_SOUND_SELECT,        ///< CEC Key 0x33: Sound Select
	CECKEY_INPUT_SELECT,        ///< CEC Key 0x34: Input Select
	CECKEY_DISPLAY_INFO,        ///< CEC Key 0x35: Display Information
	CECKEY_HELP,                ///< CEC Key 0x36: Help
	CECKEY_PAGE_UP,             ///< CEC Key 0x37: Page Up
	CECKEY_PAGE_DOWN,           ///< CEC Key 0x38: Page Down
	CECKEY_POWER = 0x40,        ///< CEC Key 0x40: Power
	CECKEY_VOLUME_UP,           ///< CEC Key 0x41: Volume Up
	CECKEY_VOLUME_DOWN,         ///< CEC Key 0x42: Volume Down
	CECKEY_MUTE,                ///< CEC Key 0x43: Mute
	CECKEY_PLAY,                ///< CEC Key 0x44: Play
	CECKEY_STOP,                ///< CEC Key 0x45: Stop
	CECKEY_PAUSE,               ///< CEC Key 0x46: Pause
	CECKEY_RECORD,              ///< CEC Key 0x47: Record
	CECKEY_REWIND,              ///< CEC Key 0x48: Rewind
	CECKEY_FAST_FORWARD,        ///< CEC Key 0x49: Fast Forward
	CECKEY_EJECT,               ///< CEC Key 0x4A: Eject
	CECKEY_FORWARD,             ///< CEC Key 0x4B: Forward
	CECKEY_BACKWARD,            ///< CEC Key 0x4C: Backward
	CECKEY_STOP_RECORD,         ///< CEC Key 0x4D: Stop Record
	CECKEY_PAUSE_RECORD,        ///< CEC Key 0x4E: Pause Record
	CECKEY_ANGLE = 0x50,        ///< CEC Key 0x50: Angle
	CECKEY_SUB_PICTURE,         ///< CEC Key 0x51: SUB-Picture
	CECKEY_VIDEO_ON_DEMAND,     ///< CEC Key 0x52: Video On Demand
	CECKEY_ELECTRO_PROG_GUIDE,  ///< CEC Key 0x53: Electronic Program Guide
	CECKEY_TIMER_PROGRAMMING,   ///< CEC Key 0x54: Timer Programming
	CECKEY_INITIAL_CONFIG,      ///< CEC Key 0x55: Initial Configuration
	CECKEY_SEL_BROADCAST_TYPE,  ///< CEC Key 0x56: Select Broadcast Type
	CECKEY_SEL_SOUND_PRESENT,   ///< CEC Key 0x57: Select Sound Presentation
	CECKEY_PLAY_FUNC = 0x60,    ///< CEC Key 0x60: Play Function
	CECKEY_PAUSE_PLAY_FUNC,     ///< CEC Key 0x61: Pause Play Function
	CECKEY_RECORD_FUNC,         ///< CEC Key 0x62: Record Function
	CECKEY_PAUSE_RECORD_FUNC,   ///< CEC Key 0x63: Pause-Record Function
	CECKEY_STOP_FUNC,           ///< CEC Key 0x64: Stop Function
	CECKEY_MUTE_FUNC,           ///< CEC Key 0x65: Mute Function
	CECKEY_RESTORE_VOLUME_FUNC, ///< CEC Key 0x66: Restore Volume Function
	CECKEY_TUNE_FUNC,           ///< CEC Key 0x67: Tune Function
	CECKEY_SEL_MEDIA_FUNC,      ///< CEC Key 0x68: Select Media Function
	CECKEY_SEL_AV_IN_FUNC,      ///< CEC Key 0x69: Select AV Input Function
	CECKEY_SEL_AUDIO_IN_FUNC,   ///< CEC Key 0x6A: Select Audio Input Function
	CECKEY_POWER_TOGGLE_FUNC,   ///< CEC Key 0x6B: Power Toggle Function
	CECKEY_POWER_OFF_FUNC,      ///< CEC Key 0x6C: Power OFF Function
	CECKEY_POWER_ON_FUNC,       ///< CEC Key 0x6D: Power ON Function
	CECKEY_F1 = 0x71,           ///< CEC Key 0x71: F1(Blue)
	CECKEY_F2,                  ///< CEC Key 0x72: F2(Red)
	CECKEY_F3,                  ///< CEC Key 0x73: F3(Green)
	CECKEY_F4,                  ///< CEC Key 0x74: F4(Yellow)
	CECKEY_F5,                  ///< CEC Key 0x75: F5
	CECKEY_DATA,                ///< CEC Key 0x76: Data

	ENUM_DUMMY4WORD(CECKEY)
} CECKEY;



/**
    CEC Remote callback function prototype
*/
//@{
typedef ER(*CecRemoteKeyCB)(CECKEY Key, BOOL Press);
//@}

/**
    CEC Vendor Key event callback function prototype
*/
//@{
typedef ER(*CecVendorKeyCB)(UINT32 Key, BOOL Press);
//@}

/**
    CEC Standby callback function prototype
*/
//@{
typedef ER(*CecStandbyCB)(void);
//@}

/**
    CEC Standby callback function prototype
*/
//@{
typedef ER(*CecActiveSrcCB)(BOOL bActive);
//@}

/**
    CEC Customized Callback ID

    This enumeration is used in Cec_HookSpecificCallBack() to specify which of the CB function is assigned.
    The Callback function should be assigned before or after the Cec_Open() and would be cleared to NULL inside
    Cec_Close(). So the user must re-assign the Callback function during Cec_Open() again each time.
    The default value for each callback funtion is NULL.
*/
typedef enum {
	CEC_CUSTOMCB_ID_STANDBY,    ///< Assign Standby CB function.
	///< This CB would be triggered when CEC receive STANDBY command from Tx.
	///< Please use CecStandbyCB as callback function prototype.
	CEC_CUSTOMCB_ID_ACTIVESRC,  ///< Assign Active-Source CB function.
	///< This CB would be triggered when the Active Source Status is changed for us.
	///< Please use CecActiveSrcCB as callback function prototype.

	ENUM_DUMMY4WORD(CEC_CUSTOMCB_ID)
} CEC_CUSTOMCB_ID;



/**
    CEC Transmit command types

    This API is used in Cec_ApplyFeature() to apply the cec transmit feature.
*/
typedef enum {
	//   Must not use the ID 0x00. Reserved for internal usage.
	CECFEATURE_ONE_TOUCH_PLAY = 0x01, ///< One Touch Play.
	CECFEATURE_GET_VENDORID,        ///< Get the Device/TV's Vendor ID.
	CECFEATURE_INACTIVE_SOURCE,     ///< Notify the TV that the active source inactivate.
	CECFEATURE_SET_OSD_STRING,      ///< Set OSD string on the TV's screen.
	CECFEATURE_MENUSTATUS,          ///< Send Menu Status command. Input parameter can use TRUE/FALSE to indicate ON/OFF.

	ENUM_DUMMY4WORD(CECFEATURE)
} CECFEATURE;



/**
    CEC Vendor ID

    This API is used in Cec_SetDeviceVendorID() to assign the device vendor id.
*/
typedef enum {
	CECVENDOR_NONE       = 0x000000,    ///< None of the Vendor ID.
	CECVENDOR_SONYBRAVIA = 0x080046,    ///< SONY BRAVIA Vendor ID.

	ENUM_DUMMY4WORD(CECVENDOR)
} CECVENDOR;



/**
    CEC Task Configuration Options.

    Currently, these definitions are used in Cec_GetConfig() to get the current CEC Task information.
*/
typedef enum {
	CECTSK_CFG_ID_CURACTSRC_LOGADDR,    ///< Get Current Active Source Logical Address.  Valid value from 0x0~0xF.
	CECTSK_CFG_ID_CURACTSRC_PHYADDR,    ///< Get Current Active Source Physical Address. Valid value from 0x0000~0xFFFF.

	ENUM_DUMMY4WORD(CECTSK_CFG_ID)
} CECTSK_CFG_ID;


//
//  CEC Exporting APIs
//
extern void     Cec_InstallID(void) _SECTION(".kercfg_text");

extern ER       Cec_Open(void);
extern ER       Cec_Close(void);

extern void     Cec_SetCommandSupport(CECCMD_SUPPORT_ID CmdID, CECCMD_SUPPORT_TYPE SuprtType);
extern ER       Cec_ApplyFeature(CECFEATURE Feature, UINT32 uiParam);

extern void     Cec_HookRemoteCallBack(CecRemoteKeyCB BasicCallBack, CecVendorKeyCB VendorCallBack);
extern void     Cec_HookSpecificCallBack(CEC_CUSTOMCB_ID CbSelect, VP func);

extern BOOL     Cec_ChkTvCecExist(void);
extern void     Cec_SetDeviceVendorID(CECVENDOR uiVendorID);
extern void     Cec_ChgOsdName(UINT32 Length, CHAR *pName);
extern UINT32   Cec_GetCecStatus(void);
extern UINT32   Cec_GetConfig(CECTSK_CFG_ID ConfigID);
//@}

#endif
