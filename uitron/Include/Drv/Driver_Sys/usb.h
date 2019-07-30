/**
    Header file for USB device

    This file is the header file for USB device module driver

    @file       usb.h
    @ingroup    mIDrvUSB_Device
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#ifndef _USB_H
#define _USB_H

#include <Type.h>

#include "Driver.h"
#include "usb_desc.h"
#include "usb_define.h"
#include "usb_compat.h"

/**
    @addtogroup mIDrvUSB_Device
*/
//@{

/**
    USB physical max DMA length
*/
//@{
#define USB_MAX_DMA_LENGTH      0x7fffff         ///< Max DMA length: 8MB-1
//@}

/**
    USB controller mode
*/
typedef enum {
	USB_CTRL_MODE_DEVICE,                       ///< USB device mode
	USB_CTRL_MODE_HOST,                         ///< USB host mode

	ENUM_DUMMY4WORD(USB_CTRL_MODE)
} USB_CTRL_MODE;


/**
    Endpoint Block number used

    Describe endpoint receive/transmit data with manner of single/ping-pong/tripple buffer

    @note For BlkNum of USB_ENP_CONFIG
*/
typedef enum {
	BLKNUM_NOT_USE,                             ///< block not used
	BLKNUM_SINGLE,                              ///< use single block
	BLKNUM_DOUBLE,                              ///< use double block
	BLKNUM_TRIPLE,                              ///< use triple block
	ENUM_DUMMY4WORD(USB_EP_BLKNUM)
} USB_EP_BLKNUM;

/**
    Endpoint direction

    @note For Direction of USB_ENP_CONFIG
*/
typedef enum {
	EP_DIR_NOT_USE,                             ///< Endpoint direction not set
	EP_DIR_IN,                                  ///< Endpoint direction is IN
	EP_DIR_OUT,                                 ///< Endpoint direction is OUT
	ENUM_DUMMY4WORD(USB_EP_DIR)
} USB_EP_DIR;

/**
    Endpoint transfer type

    @note For TrnsferType of USB_ENP_CONFIG
*/
typedef enum {
	EP_TYPE_NOT_USE,                            ///< Endpoint transfer type not set
	EP_TYPE_ISOCHRONOUS,                        ///< Endpoint transfer type is isochronous
	EP_TYPE_BULK,                               ///< Endpoint transfer type is bulk
	EP_TYPE_INTERRUPT,                          ///< Endpoint transfer type is interrupt
	ENUM_DUMMY4WORD(USB_EP_TYPE)
} USB_EP_TYPE;

/**
    transaction number for high bandwidth endpoint

    @note For HighBandWidth of USB_ENP_CONFIG
*/
typedef enum {
	HBW_NOT,                                    ///< not high bandwidth
	HBW_1,                                      ///< one transaction
	HBW_2,                                      ///< two transaction
	HBW_3,                                      ///< three transaction
	ENUM_DUMMY4WORD(USB_EP_HBW)
} USB_EP_HBW;

/**
    USB controller state

    @note For usb_setControllerState(), usb_getControllerState()
*/
typedef enum {
	USB_CONTROLLER_STATE_NONE,                  ///< USB controller state is none
	USB_CONTROLLER_STATE_RESET,                 ///< USB controller state is reset
	USB_CONTROLLER_STATE_SUSPEND,               ///< USB controller state is suspend
	ENUM_DUMMY4WORD(USB_CONTROLLER_STATE)
} USB_CONTROLLER_STATE;



/**
    Endpoint configuration

    @note For EPxConfig_HS and EPxConfig_FS of USB_MNG
*/
typedef  struct Endpoint_Config {
	BOOL           Enable;                      ///< enable this endpoint or not
												///< - @b TRUE: this endpoint descriptor is enabled
												///< - @b FALSE: this endpoint descriptor is disabled
	UINT32         BlkSize;                     ///< block size : inform usb controller allocate 512/1024 bytes block for this endpoint
												///< - @b 512: MaxPktSize <= 512 bytes
												///< - @b 1024: MaxPktSize > 512 bytes
	USB_EP_BLKNUM  BlkNum;                      ///< block number : single/double/triple
	USB_EP_DIR     Direction;                   ///< IN or OUT drection
	USB_EP_TYPE    TrnsferType;                 ///< bulk/interrupt/isochronous
	UINT32         MaxPktSize;                  ///< max packet size (unit: byte)
	USB_EP_HBW     HighBandWidth;               ///< transzction number for high bandwidth endpoint
} USB_ENP_CONFIG;


/**
    USB Management Infomation Table

    This is used in usb_initManagement() / usb_setManagement() to apply usb configurations before usb_open().
*/
typedef struct {
	UINT32           uiMagicNo;                     ///< No not modify. this value is assigned by usb_initManagement().
	USB_DEV_TYPE     USBType;                       ///< USB_MSD, USB_SIDC, USB_PCC

	USB_DEVICE_DESC  *pDevDesc;                     ///< USB Device Descriptor ptr
	USB_CONFIG_DESC  *pConfigDesc;                  ///< USB Config Descriptor ptr
	USB_CONFIG_DESC  *pConfigOthersDesc;            ///< USB Config Descriptor ptr
	USB_CONFIG_DESC  *pConfigDescHS;                ///< USB HighSpeed Descriptor
	USB_CONFIG_DESC  *pConfigDescFS;                ///< USB FullSpeed Descriptor
	USB_CONFIG_DESC  *pConfigDescFSOther;           ///< USB HighSpeed Descriptor
	USB_CONFIG_DESC  *pConfigDescHSOther;           ///< USB FullSpeed Descriptor
	USB_ENP_CONFIG   EPxConfig_HS[15];              ///< set Highspeed endpoint configuration
	USB_ENP_CONFIG   EPxConfig_FS[15];              ///< set Fullspeed endpoint configuration
	USB_STRING_DESC  *pStringDesc[4];               ///< USB String Descriptor ptr
	USB_DEVICE_DESC  *pDevQualiDesc;                ///< USB Qualifier ptr; High speed vs low speed.
	UINT16           bNumConfigurations;            ///< configuration number
	UINT16           bNumStrings;                   ///< string number

	void (*fpOpen_Needed_FIFO)(void);               ///< callback function to enable/disable FIFO
	void (*fpEvent_CB)(UINT32 uEvent);              ///< ISR callback function
	FP               fUSB_SuspendFun;               ///< callback function pointer for suspend

} USB_MNG;


/**
    control transfer

    @note For global variable ControlData which pass control transfer information between usb driver and class library
*/
typedef struct {
	UINT8               *pData;                  ///< data pointer to CX IN data
	USB_DEVICE_REQUEST  DeviceRequest;           ///< device request
	UINT16              wLength;                 ///< length
	UINT16              reserved;                ///< reserved
} CONTROL_XFER;

/**
    USB configuration identifier

    @note For usb_setConfig(), usb_getConfig()
*/
typedef enum {
	USB_CONFIG_ID_STANDARD_CHARGER,             ///< Select standard charger support
												///<   Context can be:
												///<   - @b TRUE: USB standard charger (Default value)
												///<   - @b FALSE: non-standard charger

	USB_CONFIG_ID_SQUELCH_LEVEL,                ///< Get squelch level.
	USB_CONFIG_ID_HS_SWING,                     ///< Get high speed TX Swing
	USB_CONFIG_ID_REF_CURRENT,                  ///< Get reference current

	USB_CONFIG_ID_USE_SHARE_FIFO,               ///< Allow the USB Endpoint use the Bi-Directional share FIFO or not.
												///<   Context can be:
												///<   - @b TRUE:  Allow to use share FIFO for IN/OUT endpoints.
												///<   - @b FALSE: Always use independent FIFO for all endpoints (Default value)
	USB_CONFIG_ID_ENABLE,						///< Check the controller is started or not


	/* Reserved for internal usage only */
	USB_CONFIG_ID_FIFO_START_OFS,               ///<  (Engineer Usage) Select FIFO Mapping Start Offset

	ENUM_DUMMY4WORD(USB_CONFIG_ID)
} USB_CONFIG_ID;

/**
    USB call back list

    @note For usb_setCallBack()
*/
typedef enum {
	USB_CALLBACK_CHARGING_EVENT,            ///< usb charging event call back (Designed for project layer)
											///< (SUGGESTED to be invoked in system init phase)
											///< The installed call back can receive following event:
											///< - @b USB_CHARGE_EVENT_2P5MA
											///< - @b USB_CHARGE_EVENT_100MA
											///< - @b USB_CHARGE_EVENT_500MA
											///< - @b USB_CHARGE_EVENT_5A
	USB_CALLBACK_CX_CLASS_REQUEST,          ///< usb control pipe class request call back (Designed for usb class)
											///< The installed call back doesn't need to receive parameter
	USB_CALLBACK_SET_INTERFACE,             ///< usb set interface call back (Designed for project/class layer)
											///< (SUGGESTED to be invoked in class init phase)
											///< The installed call back can receive USB_SETINTERFACE_EVENT
	USB_CALLBACK_CX_VENDOR_REQUEST,         ///< usb control pipe vendor request call back (Designed for usb class)
											///< The installed call back should process ControlData by itself
											///< The installed call back will be invoked when vendor request is received


	ENUM_DUMMY4WORD(USB_CALLBACK)
} USB_CALLBACK;


/**
    USB Endpoint Configuration Select ID

    This is used in usb_setEpConfig()/usb_getEpConfig() to select which Endpoint Config is chosen to assign new configuration.
*/
typedef enum {
	USB_EPCFG_ID_AUTOHDR_EN,            ///< Set Auto-Header function Enable/Disable. TRUE is enable and FALSE is disable.
	USB_EPCFG_ID_AUTOHDR_LEN,           ///< Set Auto-Header Length. The configuration value must be 2 or 12. Others are not supported.

	USB_EPCFG_ID_AUTOHDR_START,         ///< If setting START to TRUE/ENABLE, the next DMA Trigger start would toggle FID bit in the header field.
										///< If setting START to FALSE/DISABLE, the next DMA Trigger start would keep current FID bit in the header field.
	USB_EPCFG_ID_AUTOHDR_STOP,          ///< If setting STOP to TRUE/ENABLE, the controller would set EOF to 1 in the header field at the next DMA ending SOF slice.
	USB_EPCFG_ID_AUTOHDR_RSTCOUNTER,    ///< Set TRUE to reset internal 32bits counter value to 0 for PTS[31:0] and SCR[31:0].

	/* Get Only */
	USB_EPCFG_ID_AUTOHDR_FID_VAL,       ///< Get current FID bit value in the auto header field.
	USB_EPCFG_ID_AUTOHDR_EOF_VAL,       ///< Get current EOF bit value in the auto header field.
	USB_EPCFG_ID_AUTOHDR_PTS_VAL,       ///< Get current PTS[31:0] value in the auto header field.
	USB_EPCFG_ID_AUTOHDR_SCRLSB_VAL,    ///< Get current SCR[31:0] value in the auto header field.
	USB_EPCFG_ID_AUTOHDR_SCRMSB_VAL,    ///< Get current SCR[47:32] value in the auto header field.


	/* Below Config IDs are reserved for internal usage only */
	USB_EPCFG_ID_AUTOHDR_HWFID_EN,      // Novatek internal usage only
	USB_EPCFG_ID_AUTOHDR_SWFID,         // Novatek internal usage only
	USB_EPCFG_ID_AUTOHDR_HWPTS_EN,      // Novatek internal usage only
	USB_EPCFG_ID_AUTOHDR_SWPTS,         // Novatek internal usage only

	ENUM_DUMMY4WORD(USB_EPCFG_ID)
} USB_EPCFG_ID;


/**
    ControlData

    Control Transfer information

    - If SETUP token is received, usb driver may pass ControlData to class library for vendor control transfer
    - If class library needs return CX_IN data, it should fill ControlData then invoke usb_RetSetupData()
*/
extern CONTROL_XFER ControlData;


//
// Normal USB Driver API
//

/**
    Initialize USB Management Infomation Table Data Structure

    Initialize USB Management Infomation Table Data Structure.
    User can allocate local parameter of USB_MNG and apply usb_initManagement to it.
    And then start configure the management table.

    @param[in] pMng USB Management Infomation Table pointer

    @return void
*/
extern void                 usb_initManagement(USB_MNG *pMng);

/**
    Apply USB Management Infomation Table

    Apply USB Management Infomation Table to USB Driver.

    @param[in] pMng USB Management Infomation Table pointer

    @return
     - @b E_OK:  Apply management info success.
     - @b E_PAR: Input parameter is NULL pointer.
     - @b E_CTX: No Init. Please apply usb_initManagement() to pMng first.
*/
extern ER                   usb_setManagement(USB_MNG *pMng);

/**
    Get Current USB Management Infomation Table

    Get Current  USB Management Infomation Table to USB Driver.

    @param[out] pMng USB Management Infomation Table pointer

    @return
     - @b E_OK:  Apply management info success.
     - @b E_PAR: Input parameter is NULL pointer.
     - @b E_CTX: No Init. Please apply usb_initManagement() to pMng first.
*/
extern ER                   usb_getManagement(USB_MNG *pMng);

/**
    Open USB driver

    Initialize USB module and enable interrupt, start running
    control & setup task for ISR etc...

    @return
        - @b E_OK: open success
        - @b E_OACV: USB driver is already opened
        - @b E_ID: Outside semaphore ID number range
        - @b E_NOEXS: Semaphore does not yet exist
        - @b E_SYS: USB has been unplug, so ignore this request...
*/
extern ER                   usb_open(void);

/**
    Check usb charger

    Check if connect with a usb charger

    @param[in] uiDebounce       debounce time to detect usb charger, unit: 10ms

    @return
        - @b USB_CHARGER_STS_NONE: PC or not charger detected
        - @b USB_CHARGER_STS_CHARGING_DOWNSTREAM_PORT: charging downstream port
        - @b USB_CHARGER_STS_CHARGER: charger detected
*/
extern USB_CHARGER_STS      usb_chkCharger(UINT32 uiDebounce);

/**
    USB Power on initialization.

    @note This API SHOULD be invoked in system init phase. Else USB may be malfunctioned.

    @param[in] bHighSpeed   set USB in high/full speed mode
                            - @b TRUE: high speed mode
                            - @b FALSE: full speed mode
    @return void
*/
extern void                 usb_PowerOnInit(BOOL bHighSpeed);

/**
    USB Change state

    Calling this function to get the current plug status.
    Application should remember the plug/unplug state to avoid calling
    open/close function multiple!

    @return
        - @b FALSE: Unplug State.
        - @b TRUE: Plug State.
*/
extern BOOL                 usb_StateChange(void);

/**
    Close USB device driver access.

    This function will stop USB DMA, terminate control and setup task,
    disable interrupt and USB clock etc...

    @return
        - @b E_OK: open success
        - @b E_OACV: USB driver is not opened
        - @b E_ID: Outside semaphore ID number range
        - @b E_NOEXS: Semaphore does not yet exist
        - @b E_QOVR: Semaphore's counter error, maximum counter < counter
*/
extern ER                   usb_close(void);

/**
    Set VBUSI state

    Force VBUSI state for USB controller
    This API should be invoked when upper layer want to force USB connect without usb_StateChange()

    @param[in] bPlug    state of current VBUSI
                        - @b TRUE: VBUSI is HIGH
                        - @b FALSE: VBUSI is LOW

    @return void
*/
extern void                 usb_setVbusi(BOOL bPlug);

/**
    Set the callback function when wakeup from standby mode

    User can use this API to register call back when system is waked up from power down/standby mode.
    The installed call back will be invoked if system is waked up by USB (resume/reset).

    @param[in] fpIsr    User defined ISR for wakeup interrupt

    @return void
*/
extern void                 usb_SetStandByWakeup(DRV_CB fpIsr);

/**
    get USB controller state.

    @return    USB controller state
        - @b USB_CONTROLLER_STATE_NONE: USB is not used
        - @b USB_CONTROLLER_STATE_RESET: USB is in reset state
        - @b USB_CONTROLLER_STATE_SUSPEND: USB is in suspend state
*/
extern USB_CONTROLLER_STATE usb_getControllerState(void);

/**
    Check set configuration result

    This API is provided to upper layer to check if USB is under set configuration state.

    @return
        - @b TRUE: under set configuration
        - @b FALSE: not under set configuration
*/
extern BOOL                 usb_chkConfiguration(void);

/**
    Set USB controller configuration

    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
*/
extern ER                   usb_setConfig(USB_CONFIG_ID configID, UINT32 configContext);

/**
    Get USB controller configuration

    @param[in] configID         configuration identifier

    @return configuration context for configID
*/
extern UINT32               usb_getConfig(USB_CONFIG_ID configID);

/**
    Set usb call back routine

    This function provides a facility for upper layer to install callback routine.

    @param[in] callBackID   callback routine identifier
    @param[in] pCallBack    callback routine to be installed

    @return
        - @b E_OK: install callback success
        - @b E_NOSPT: callBackID is not valid
*/
extern ER                   usb_setCallBack(USB_CALLBACK callBackID, USB_GENERIC_CB pCallBack);

/**
    Get the current setting of USB speed mode.

    @return
        - @b TRUE: device is in High speed
        - @b FALSE: device is in full speed
*/
extern BOOL                 usb_isHighSpeedEnabled(void);

//
// USB read /write functions
//

/**
    Return setup data to host.

    Class library can use this API to return CX IN data after receive a setup packet.

    @note Before invoke this API, ControlData.pData should point to return data\n
            and ControlData.wLength should be filled with length of return data.

    @return void
*/
extern void                 usb_RetSetupData(void);

/**
    Read the buffer content from FIFO

    This function is blocking mode. It will return until DMA done or
    some error occured. Check the return ER and pDMALen for actual
    transfered length.

    @param[in] EPn          endpoint number (USB_EP0~USB_EP15)
    @param[in] pBuffer      buffer pointer
    @param[in,out] pDMALen  input length to read, output actual transfered length

    @return
        - @b E_OACV: USB driver is not opened
        - @b E_PAR: input DMA length or EP number incorrect
        - @b E_OBJ: FIFO is busy
        - @b E_SYS: USB is SUSPENDED
        - @b E_OK: read done (maybe aborted, check return pDMALen)
*/
extern ER                   usb_readEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Write the buffer content to FIFO

    This function is blocking mode. It will return until DMA done or
    some error occured. Check the return ER and pDMALen for actual
    transfered length.

    @param[in] EPn          endpoint number (USB_EP0~USB_EP15)
    @param[in] pBuffer      buffer pointer
    @param[in,out] pDMALen  input length to write, output actual transfered length

    @return
        - @b E_OACV: USB driver is not opened
        - @b E_PAR: input DMA length or EP number incorrect
        - @b E_OBJ: FIFO is busy
        - @b E_SYS: USB is SUSPENDED
        - @b E_OK: read done (maybe aborted, check return pDMALen)
*/
extern ER                   usb_writeEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Abort the endpoint transfer

    @param[in] EPn  endpoint number (1~15)

    @return
        - @b E_OK: abort success
        - @b Else: abort fail
*/
extern ER                   usb_abortEndpoint(USB_EP EPn);

/**
    Read the buffer content from FIFO

    This function is non-blocking mode and will return immediately.
    Used with usb_waitEPDone() to wait for DMA complete.

    @param[in] EPn      endpoint number (USB_EP0~USB_EP15)
    @param[in] pBuffer  buffer pointer
    @param[in] pDMALen  DMA length to be read.
                        Valid length 0x1 to 0x40 for EP0.
                        Valid length 0x1 to 0x7fffff for EP1~EP15.

    @return
        - @b E_OACV: USB driver is not opened
        - @b E_PAR: input DMA length or EP number incorrect
        - @b E_OBJ: FIFO is busy
        - @b E_SYS: USB is SUSPENDED
        - @b E_OK: start to transfer
*/
extern ER                   usb_setEPRead(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Write the buffer content to FIFO

    This function is non-blocking mode and will return immediately.
    Used with usb_waitEPDone() to wait for DMA complete.

    @param[in] EPn      endpoint number (USB_EP0~USB_EP15)
    @param[in] pBuffer  buffer pointer
    @param[in] pDMALen  DMA length to be written.
                        Valid length 0x1 to 0x40 for EP0.
                        Valid length 0x1 to 0x7fffff for EP1~EP15.

    @return
        - @b E_OACV: USB driver is not opened
        - @b E_PAR: input DMA length or EP number incorrect
        - @b E_OBJ: FIFO is busy
        - @b E_SYS: USB is SUSPENDED
        - @b E_OK: start to transfer
*/
extern ER                   usb_setEPWrite(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Wait for endpoint DMA done

    This function is used with usb_setEPRead() or usb_setEPWrite().
    It will Wait until DMA done and return actual transfered length

    @param[in] EPn          endpoint number (USB_EP0~USB_EP8)
    @param[in,out] pDMALen  input DMA length (1~0x7fffff), output actual transfered length

    @return
        - @b E_OACV: USB driver is not opened
        - @b E_OK: DMA done (maybe aborted, check return pDMALen)
*/
extern ER                   usb_waitEPDone(USB_EP EPn, UINT32 *pDMALen);

//
// USB endpoint and FIFO setting functions
//

/**
    Set Configuration value to dedicate Endpoint

    Set Configuration value to dedicate Endpoint

    @param[in] EPn          Endpoint number.
    @param[in] CfgID        Configuration Select ID.
    @param[in] uiCfgValue   Configuration value.

    @return void
*/
extern void                 usb_setEpConfig(USB_EP EPn, USB_EPCFG_ID CfgID, UINT32 uiCfgValue);

/**
    Get Configuration value from dedicate Endpoint

    Get Configuration value from dedicate Endpoint

    @param[in] EPn      Endpoint number.
    @param[in] CfgID    Configuration Select ID.

    @return Configuration value
*/
extern UINT32               usb_getEpConfig(USB_EP EPn, USB_EPCFG_ID CfgID);

/**
    Mask(Disable) the endpoint interrupt

    @param[in] EPn  endpoint number (USB_EP0~USB_EP8)

    @return void
*/
extern void                 usb_maskEPINT(USB_EP EPn);

/**
    Unmask(Enable) the endpoint interrupt

    @param[in] EPn  endpoint number (USB_EP0~USB_EP8)

    @return void
*/
extern void                 usb_unmaskEPINT(USB_EP EPn);

/**
    Set endpoint0 CX_DONE

    When class library completes transfers of CX IN/OUT, it should invoke this API to enter status stage.

    @return void
*/
extern void                 usb_setEP0Done(void);

/**
    Set endpoint stall

    When class library finds somethins wrong, it can use this API to make a function stall for this endpoint.

    @param[in] EPn  endpoint number (USB_EP0~USB_EP15)

    @return void
*/
extern void                 usb_setEPStall(USB_EP EPn);

/**
    Clear Endpoint FIFO

    Clear endpoint related FIFO.

    @param[in] EPn  endpoint number (USB_EP0~USB_EP15)
    @return void
*/
extern void                 usb_clrEPFIFO(USB_EP EPn);

/**
    clear FIFO

    @return void
*/
extern void                 usb_clrFIFO(void);

/**
    Set the endpoint to send a 0 byte data

    @param[in] EPn      endpoint number (USB_EP1~USB_EP15)

    @return void
*/
extern void                 usb_setTX0Byte(USB_EP EPn);

/**
    Get endpoint byte count.

    Some application may not know the exactly transfering size at
    the beginning. Use this function to get the FIFO byte counts.
    This API should be used after OUT interrupt issued.

    @param[in] EPn      endpoint number (USB_EP0~USB_EP15)

    @return endpoint byte count
*/
extern UINT32               usb_getEPByteCount(USB_EP EPn);

/**
    Check endpoint FIFO empty

    @param[in] EPn      endpoint number (USB_EP1~USB_EP8)

    @return
        - @b TRUE: FIFO for this endpoint is empty
        - @b FALSE: FIFO for this endpoint is NOT empty
*/
extern BOOL                 usb_chkEPEmpty(USB_EP EPn);

/**
    Check endpoint is Busy or not.

    @param[in] EPn      endpoint number (USB_EP1~USB_EP15)

    @return
        - @b FALSE:      FIFO for this endpoint is empty and DMA is not working.
        - @b TRUE:     FIFO for this endpoint is busy.
*/
extern BOOL                 usb_chkEPBusy(USB_EP EPn);

/**
    Get SOF

    Return current SOF 11bits value

    @return SOF value
*/
extern UINT32               usb_getSOF(void);

//
// USB misc functions
//

/**
    Set the release number in USB device descriptor

    @param[in] Speed        Set high speed or full speed value
                            - @b TRUE: High speed
                            - @b FALSE: Full speed
    @param[in] RelNum       Set the release number value
                            - @b USB_HS_DEV_RELEASE_NUMBER: USB 2.0
                            - @b USB_FS_DEV_RELEASE_NUMBER: USB 1.1

    @return void
*/
extern void                 usb_SetDevDescUSBVerRelNum(BOOL Speed, UINT16 RelNum);

extern void             	usb_installID(void) _SECTION(".kercfg_text");

//@}
#endif // _USB_H
