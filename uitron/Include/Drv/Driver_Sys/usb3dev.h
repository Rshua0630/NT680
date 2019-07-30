/**
    Header file for USB 3.0 Device Controller

    This file is the header file for USB 3.0 device module driver

    @file       usb3dev.h
    @ingroup    mIDrvUSB3_Device
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _USB3DEV_H
#define _USB3DEV_H

#include "Driver.h"

#include "usb_desc.h"
#include "usb_define.h"

/**
	USB3 Device Driver needed minimum buffer size

	The user shall use usb3dev_setConfig(U3DEV_CONFIG_ID_BUFFER_ADDR) / usb3dev_setConfig(U3DEV_CONFIG_ID_BUFFER_SIZE)
	to assign driver needed buffer address and buffer size. The size shall not smaller than U3DEV_MIN_BUFFER_SIZE.
*/
//@{
#define U3DEV_MIN_BUFFER_SIZE   (0x4000)		///< 16KB for usb3 driver internal usages.
//@}

/**
	USB3 Device Controller Total TXFIFO Size

	USB3 Device Controller Total TXFIFO Size.
*/
//@{
#define U3DEV_TOTAL_TXFIFO_SIZE		16768		///< default is 2096 each for EP1-8.
//@}

/**
	USB3 Device controller configuration identifier

	This is used in usb3dev_setConfig() to assign the configuration parameters.
*/
typedef enum {
	U3DEV_CONFIG_ID_BUFFER_ADDR,		///< Driver internal needed buffer address.
	U3DEV_CONFIG_ID_BUFFER_SIZE,		///< Driver internal needed buffer size. Must not less than U3DEV_MIN_BUFFER_SIZE.

	U3DEV_CONFIG_ID_SPEED,				///< Set Maximum allowed usb speed. Please use U3DEV_SPEED as input value.
	U3DEV_CONFIG_ID_ISO_LATENCY,		///< Set Isochronous latency after trigger data start to transfer to its real transmission start.
										///< The default value is 4. (4x125us = 500us)
	U3DEV_CONFIG_ID_INTERFACE_ALTERNATE,///< Set interface altermating number would act as default ON setting.

	U3DEV_CONFIG_ID_STANDARD_CHARGER,	///< Set TRUE if detecting charger is standard charger. Set FALSE to support non-standard charger.
	U3DEV_CONFIG_ID_DYNAMIC_PM,			///< When SuperSpeed is connected, the controller would allow U2 power state request if enabled.
										///< The U1 power state is not supported for NT96680. Default is ENABLE.
	U3DEV_CONFIG_ID_ALLOW_COMPLIANCE,	///< Allow ENABLE/DISABLE entering Compliance test mode. Default is DISABLE.
	U3DEV_CONFIG_ID_SET_TXFIFO,			///< Re-Assign IN Endpoints FIFO Mapping. Please use USB3DEV_INEP_TXFIFO(InEPn,FiFoSz) as input.
										///< This is only allowed to use at Callback U3DEV_CALLBACK_ID_TXFIFO_REMAP.

	ENUM_DUMMY4WORD(U3DEV_CONFIG_ID)
} U3DEV_CONFIG_ID;


/**
	Used in usb3dev_setConfig(U3DEV_CONFIG_ID_INTERFACE_ALTERNATE, USB3DEV_INTF_ALT(intf,alt))
*/
//@{
#define USB3DEV_INTF_ALT(intf,alt)      (((intf)<<16)+(alt))
//@}

/**
	Used in usb3dev_setConfig(U3DEV_CONFIG_ID_SET_TXFIFO, USB3DEV_INEP_TXFIFO(InEPn,FiFoSz))
*/
//@{
#define USB3DEV_INEP_TXFIFO(InEPn,FiFoSz)      (((InEPn)<<16)+(FiFoSz))
//@}


/**
	Used in usb3dev_getEpInfo() to get the Endpoint information
*/
typedef enum {
	U3DEV_EPINFO_ID_EVENT,  ///< This can be used in the callback to get exactly EP's event value.

	ENUM_DUMMY4WORD(U3DEV_EPINFO_ID)
} U3DEV_EPINFO_ID;

/**
	Used in usb3dev_getDevInfo() to get the Endpoint information
*/
typedef enum {
	U3DEV_DEVINFO_ID_SPEED,     	///< Please use U3DEV_SPEED as parameter
	U3DEV_DEVINFO_ID_SQUELCH_LEVEL, ///< Get squelch level.
	U3DEV_DEVINFO_ID_HS_SWING,      ///< Get high speed TX Swing
	U3DEV_DEVINFO_ID_REF_CURRENT,   ///< Get reference current
	U3DEV_DEVINFO_ID_RTRIM,   		///< Get RTRIM value
	U3DEV_DEVINFO_ID_STANDARD_CHARGER,///< Get if the config is the Standard Charger or not
	U3DEV_DEVINFO_ID_ENABLE,		///< Get RUNSTOP bit is set or not

	ENUM_DUMMY4WORD(U3DEV_DEVINFO_ID)
} U3DEV_DEVINFO_ID;

/**
	USB Speed Difinitions.

	Used in usb3dev_setConfig(U3DEV_CONFIG_ID_SPEED, U3DEV_SPEED) to assign allowed maximum usb speed.
	or used in usb3dev_getDevInfo(U3DEV_DEVINFO_ID_SPEED) to get actual running speed.
*/
typedef enum {
	U3DEV_SPEED_SS,		///< Super-Speed. (5Gbps.)
	U3DEV_SPEED_HS,		///< High-Speed.  (480Mbps.)
	U3DEV_SPEED_FS,		///< Full-Speed.  (12Mbps.)

	U3DEV_SPEED_UNKNOWN,
	ENUM_DUMMY4WORD(U3DEV_SPEED)
} U3DEV_SPEED;

/**
	Callback Routine Identifier

	This is used in usb3dev_setCallBack() to assign callback routine function.
*/
typedef enum {
	U3DEV_CALLBACK_ID_CX_VENDOR_REQUEST,	///< Vendor Request Callback.
	U3DEV_CALLBACK_ID_CX_VENDOR_REQDONE,	///< Vendor Request Data Phase done Callback. (Status Phase not done)
	U3DEV_CALLBACK_ID_CX_CLASS_REQUEST,		///< Class Request Callback.
	U3DEV_CALLBACK_ID_CX_CLASS_REQDONE,		///< Class Request Data Phase done Callback. (Status Phase not done)

	U3DEV_CALLBACK_ID_EVENT_NOTIFY,			///< Event Notification Callback. The notify event ID is USB3_EVENT.
	U3DEV_CALLBACK_ID_CHARGING_EVENT,		///< Charger Control Callback funtion. The notify event ID is USB_CHARGE_EVENT.
	U3DEV_CALLBACK_ID_SET_INTERFACE,		///< Callback function when set_interface command received.
	U3DEV_CALLBACK_ID_TXFIFO_REMAP,			///< Callback function for TXFIFO Remapping.

	U3DEV_CALLBACK_ID_NO,
	ENUM_DUMMY4WORD(U3DEV_CALLBACK_ID)
} U3DEV_CALLBACK_ID;

/**
	Set Descriptor identification

	This is used in usb3dev_setDescriptor() to assign USB descriptors for host requested.
*/
typedef enum {
	U3DEV_DESC_ID_SS_DEVICE,			///< Super Speed Device Descriptor
	U3DEV_DESC_ID_SS_CONFIG,			///< Super Speed Configuration Descriptor
	U3DEV_DESC_ID_SS_BOS,				///< Super Speed BOS Descriptor
	U3DEV_DESC_ID_SS_STRING0,			///< Super Speed String Descriptor 0
	U3DEV_DESC_ID_SS_STRING1,			///< Super Speed String Descriptor 1
	U3DEV_DESC_ID_SS_STRING2,			///< Super Speed String Descriptor 2
	U3DEV_DESC_ID_SS_STRING3,			///< Super Speed String Descriptor 3
	U3DEV_DESC_ID_SS_STRING4,			///< Super Speed String Descriptor 4
	U3DEV_DESC_ID_SS_STRING5,			///< Super Speed String Descriptor 5
	U3DEV_DESC_ID_SS_STRING6,			///< Super Speed String Descriptor 6
	U3DEV_DESC_ID_SS_STRING7,			///< Super Speed String Descriptor 7

	U3DEV_DESC_ID_HS_DEVICE,			///< High Speed Device Descriptor
	U3DEV_DESC_ID_HS_DEVICEQUALIFIER,	///< High Speed Device Qualifier Descriptor
	U3DEV_DESC_ID_HS_CONFIG,			///< High Speed Configuration Descriptor
	U3DEV_DESC_ID_HS_OTHERSPEED,		///< High Speed Other Speed Descriptor
	U3DEV_DESC_ID_HS_BOS,				///< High Speed BOS Descriptor
	U3DEV_DESC_ID_HS_STRING0,			///< High Speed String Descriptor 0
	U3DEV_DESC_ID_HS_STRING1,			///< High Speed String Descriptor 1
	U3DEV_DESC_ID_HS_STRING2,			///< High Speed String Descriptor 2
	U3DEV_DESC_ID_HS_STRING3,			///< High Speed String Descriptor 3
	U3DEV_DESC_ID_HS_STRING4,			///< High Speed String Descriptor 4
	U3DEV_DESC_ID_HS_STRING5,			///< High Speed String Descriptor 5
	U3DEV_DESC_ID_HS_STRING6,			///< High Speed String Descriptor 6
	U3DEV_DESC_ID_HS_STRING7,			///< High Speed String Descriptor 7

	U3DEV_DESC_ID_FS_DEVICE,			///< Full Speed Device Descriptor
	U3DEV_DESC_ID_FS_DEVICEQUALIFIER,	///< Full Speed Device Qualifier Descriptor
	U3DEV_DESC_ID_FS_CONFIG,			///< Full Speed Configuration Descriptor
	U3DEV_DESC_ID_FS_OTHERSPEED,		///< Full Speed Other Speed Descriptor
	U3DEV_DESC_ID_FS_STRING0,			///< Full Speed String Descriptor 0
	U3DEV_DESC_ID_FS_STRING1,			///< Full Speed String Descriptor 1
	U3DEV_DESC_ID_FS_STRING2,			///< Full Speed String Descriptor 2
	U3DEV_DESC_ID_FS_STRING3,			///< Full Speed String Descriptor 3
	U3DEV_DESC_ID_FS_STRING4,			///< Full Speed String Descriptor 4
	U3DEV_DESC_ID_FS_STRING5,			///< Full Speed String Descriptor 5
	U3DEV_DESC_ID_FS_STRING6,			///< Full Speed String Descriptor 6
	U3DEV_DESC_ID_FS_STRING7,			///< Full Speed String Descriptor 7

	U3DEV_DESC_ID_NO,
	ENUM_DUMMY4WORD(U3DEV_DESC_ID)
} U3DEV_DESC_ID;


/**
	USB Endpoint Direction Definition
*/
typedef enum {
	U3DEV_EPDIR_OUT,	///< OUT. Data transfer is from HOST to Device(96680).
	U3DEV_EPDIR_IN,		///< IN. Data transfer is from Device(96680) to HOST.

	ENUM_DUMMY4WORD(U3DEV_EPDIR)
} U3DEV_EPDIR;

/**
	ISOCHRONOUS Packet Data Structure

	This is used in U3DEV_ISODATA to construct the packet stream structure.
*/
typedef struct {
	UINT32  uiBufAddr;                  ///< Transfer Buffer Address
	UINT16  uiSize;                     ///< Target Transfer Size
	UINT16  uiRemainedSize;             ///< Not Transmitted/Recieved Size
} U3DEV_ISOPKT, *PU3DEV_ISOPKT;

/**
	ISOCHRONOUS Endpoint Data Packet Stream Data Structure

	This is used in ISOCHRONOUS transfer API to assign ISOCHRONOUS data buffer.
	Such as usb3dev_writeIsoEndpoint/usb3dev_readIsoEndpoint/ ...
*/
typedef struct {
	UINT32          uiPacketNumber;     ///< Number of ISO packets
	PU3DEV_ISOPKT   pIsoPacket;         ///< U3DEV_ISOPKT Array Pointer

	UINT32          uiNotFinishedSize;	///< Not Transmitted/Recieved Size
} U3DEV_ISODATA, *PU3DEV_ISODATA;


//
//	Exporting API definitions and descriptions
//


/**
    Open USB3 Device Controller Driver

    Initialize USB3 Device module and enable interrupt, start running
    controller to connect with usb host controller.

    @return
        - @b E_OK: 		open success
        - @b E_NOMEM: 	The USB Driver needed buffer is not assigned or not enough buffer size.
        				Please use usb3dev_setConfig(U3DEV_CONFIG_ID_BUFFER_ADDR / U3DEV_CONFIG_ID_BUFFER_SIZE)
        				to assign this buffer address/size. The minimum size is U3DEV_MIN_BUFFER_SIZE.
*/
extern ER               usb3dev_open(void);

/**
    Close USB3 Device Controller Driver access.

    This function will stop USB3 controller, terminate the unfinished USB transfer,
    disable interrupt and USB clock etc...

    @return
        - @b E_OK: Close USB driver success
*/
extern ER               usb3dev_close(void);

/**
    Check if USB3 Device Controller Driver is already opened or not

    Check if USB3 Device Controller Driver is already opened or not

    @return
        - @b TRUE:   Current is Opend.
        - @b FALSE:  Current is Closed.
*/
extern BOOL             usb3dev_isOpened(void);

/**
	Set the SETUP(CONTROL) transfer result

	This API shall be used inside the Vendor or ClassRequest SETUP/CONTROL Callback to return
	the result of the SETUP 8B command. If the SETUP command is not valid, just set bStall to TRUE
	and the uiRetBufAddr/uiRetSize would be dont care. If the bStall is FALSE and Data Phase exists,
	the user shall fill the IN/OUT data buffer address and size.

    @param[in] bStall    	STALL to the SETUP command or not.
    @param[in] uiRetBufAddr Data Phase buffer address
    @param[in] uiRetSize    Data Phase buffer size

    @return void
*/
extern void             usb3dev_handleSetupResult(BOOL bStall, UINT32 uiRetBufAddr, UINT32 uiRetSize);

/**
    Set usb call back routine

    This function provides a facility for upper layer to install callback routine in the usb driver.

    @param[in] U3CallBackID   callback routine identifier
    @param[in] pCallBack      callback routine to be installed

    @return
        - @b E_OK: 		install callback success
        - @b E_NOSPT: 	callBackID is not valid
*/
extern ER               usb3dev_setCallBack(U3DEV_CALLBACK_ID U3CallBackID, USB_GENERIC_CB pCallBack);

/**
	Set Device Mode Descriptors

	These descriptors must be declared in the fixed mapping dram address.
	Must not use the STACK buffer! Because usb driver would use it anytime after usb3dev_open().
	The upper layer can only reuse the descriptor buffer after usb3dev_close().
	These descriptor would be return to host/pc by the host request, the user must setup all the needed
	descriptors before usb3dev_open(). If the host requested descriptor is not assigned, the device driver
	would STALL the request automatically.
	All the assigned descriptors would be auto cleared after usb3dev_close(), the user shall re-assign the
	descriptors before next usb3dev_open().

    @param[in] U3DescID    	Descriptor ID.
    @param[in] DescBuf    	Descriptor data buffer address.
    						The size shall be correct filled in the descriptor content.

    @return E_OK means assigned done and success.
*/
extern ER               usb3dev_setDescriptor(U3DEV_DESC_ID U3DescID, void *DescBuf);

/**
    Set USB3 Device controller configurations

    @param[in] CfgID         	configuration identifier
    @param[in] uiConfig    		configuration context for configID

    @return
        - @b E_OK: 		set configuration success
        - @b E_NOSPT: 	configID is not supported
*/
extern ER               usb3dev_setConfig(U3DEV_CONFIG_ID CfgID, UINT32 uiConfig);

/**
    Get USB3 Device Controller Information

    @param[in] InfoID  Information select identifier. Please refer to U3DEV_DEVINFO_ID for more details.

    @return The information result for InfoID
*/
extern UINT32           usb3dev_getDevInfo(U3DEV_DEVINFO_ID InfoID);

/**
    Get USB3 Device Controller's Endpoint Information

    @param[in] EPn  	Specified Endpoint number. Please use USB_EP.
    @param[in] EpDir  	Specified Endpoint direction. Please use U3DEV_EPDIR_OUT or U3DEV_EPDIR_IN.
    @param[in] InfoID  	Information select identifier. Please refer to U3DEV_EPINFO_ID for more details.

    @return The information result for selected EP's InfoID
*/
extern UINT32           usb3dev_getEpInfo(USB_EP EPn, U3DEV_EPDIR EpDir, U3DEV_EPINFO_ID InfoID);

/**
    Check usb charger

    Check if connect with a usb charger

    @param[in] uiDebounce   debounce time to detect usb charger.
    						Useless in current driver implementation. Backward compatible usages.
    @return
        - @b USB_CHARGER_STS_NONE: PC or not charger detected
        - @b USB_CHARGER_STS_CHARGING_DOWNSTREAM_PORT: charging downstream port
        - @b USB_CHARGER_STS_CHARGER: charger detected
        - @b USB_CHARGER_STS_DISCONNECT: cable is disconnected.
*/
extern USB_CHARGER_STS  usb3dev_chkCharger(UINT32 uiDebounce);

/**
    USB3 Device Change state

    Calling this function to get the current plug status.
    After getting the plug state is TRUE, the user can invoke to usb3dev_open() to start
    usb device class driver.

    @return
        - @b FALSE: Unplug State.
        - @b TRUE:  Plug State.
*/
extern BOOL             usb3dev_StateChange(void);

/**
    Check endpoint is Busy or not.

    @param[in] EPn      endpoint number (USB_EP1~USB_EP8)
    @param[in] EpDir    endpoint Direction.

    @return
        - @b FALSE:     Idle.
        - @b TRUE:      busy.
*/
extern BOOL             usb3dev_chkEPBusy(USB_EP EPn, U3DEV_EPDIR EpDir);

/**
    Get SOF

    Return current SOF value

    @return SOF/ITP value
*/
extern UINT32 			usb3dev_getSOF(void);

/**
    Write USB IN endpoint data

    This function is blocking operation.
    It will return until all the data size transfer done or
    some error occured such as disconnecting.
    Check the return ER and pDMALen for actual transfered length.

    @param[in] EPn          endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pBuffer      buffer pointer
    @param[in,out] pDMALen  input length to write, output actual transfered length

    @return
        - @b E_OACV: EPn number not valid
        - @b E_PAR:  input DMA length or DMA address error
        - @b E_OBJ:  Endpoint is busy
        - @b E_OK:   write done (maybe aborted, please check return pDMALen)
*/
extern ER               usb3dev_writeEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Write USB IN endpoint data

    This function is non-blocking mode and will return immediately.
    Please use usb3dev_waitWriteDone() to wait for transfer done
    and get the returned real transfer size.

    @param[in] EPn          endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pBuffer      buffer pointer
    @param[in,out] pDMALen  input length to write, output actual transfered length

    @return
        - @b E_OACV: EPn number not valid
        - @b E_PAR:  input DMA length or DMA address error
        - @b E_OBJ:  Endpoint is busy
        - @b E_OK:   Start to transfer
*/
extern ER               usb3dev_setEPWrite(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Write USB ISOCHRONOUS IN endpoint data

    This function is blocking operation.
    It will return until all the data size transfer done or
    some error occured such as short packet received or disconnecting.
    Check the return ER and pIsoData data structure for actual transfered length.

    @param[in] EPn          ISOCHRONOUS endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pIsoData     ISOCHRONOUS transfer data information. Please refer to PU3DEV_ISODATA for details.

    @return
        - @b E_OACV: EPn number not valid or not ISOCHRONOUS endpoint.
        - @b E_OK:   Start to transfer
*/
extern ER               usb3dev_writeIsoEndpoint(USB_EP EPn, PU3DEV_ISODATA pIsoData);

/**
    Write USB ISOCHRONOUS IN endpoint data

    This function is non-blocking mode and will return immediately.
    Please use usb3dev_waitWriteDone() to wait for transfer done
    and get the returned real transfer size.

    @param[in] EPn          ISOCHRONOUS endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pIsoData     ISOCHRONOUS transfer data information. Please refer to PU3DEV_ISODATA for details.

    @return
        - @b E_OACV: EPn number not valid or not ISOCHRONOUS endpoint.
        - @b E_OK:   Start to transfer
*/
extern ER               usb3dev_setIsoEPWrite(USB_EP EPn, PU3DEV_ISODATA pIsoData);

/**
    Wait for IN endpoint transfer done

    This function is used after usb3dev_setIsoEPWrite() or usb3dev_setEPWrite().
    It will Wait until transfer done and return actual transfered length.

    @param[in] EPn          endpoint number (USB_EP0~USB_EP8)
    @param[in,out] pDMALen  input DMA length (1~0x7fffff), output actual transfered length

    @return
        - @b E_OACV: USB driver is not opened
        - @b E_OK: DMA done (maybe aborted, check return pDMALen)
*/
extern ER               usb3dev_waitWriteDone(USB_EP EPn, UINT32 *pDMALen);

/**
    Read USB OUT endpoint data

    This function is blocking operation.
    It will return until all the data size transfer done or
    some error occured such as short packet received or disconnecting.
    Check the return ER and pDMALen for actual transfered length.

    @param[in] EPn          endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pBuffer      buffer pointer
    @param[in,out] pDMALen  input length to read, output actual transfered length

    @return
        - @b E_OACV: EPn number not valid
        - @b E_PAR:  input DMA length or DMA address error
        - @b E_OBJ:  Endpoint is busy
        - @b E_OK:   read done (maybe aborted, please check return pDMALen)
*/
extern ER               usb3dev_readEndpoint(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Read USB OUT endpoint data

    This function is non-blocking mode and will return immediately.
    Please use usb3dev_waitReadDone() to wait for transfer done
    and get the returned real transfer size.

    @param[in] EPn          endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pBuffer      buffer pointer
    @param[in,out] pDMALen  input length to write, output actual transfered length

    @return
        - @b E_OACV: EPn number not valid
        - @b E_PAR:  input DMA length or DMA address error
        - @b E_OBJ:  Endpoint is busy
        - @b E_OK:   Start to transfer
*/
extern ER               usb3dev_setEPRead(USB_EP EPn, UINT8 *pBuffer, UINT32 *pDMALen);

/**
    Read USB ISOCHRONOUS OUT endpoint data

    This function is blocking operation.
    It will return until all the data size transfer done or
    some error occured such as short packet received or disconnecting.
    Check the return ER and pIsoData data structure for actual transfered length.

    @param[in] EPn          ISOCHRONOUS endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pIsoData     ISOCHRONOUS transfer data information. Please refer to PU3DEV_ISODATA for details.

    @return
        - @b E_OACV: EPn number not valid or not ISOCHRONOUS endpoint.
        - @b E_OK:   Start to transfer
*/
extern ER               usb3dev_readIsoEndpoint(USB_EP EPn, PU3DEV_ISODATA pIsoData);

/**
    Read USB ISOCHRONOUS OUT endpoint data

    This function is non-blocking mode and will return immediately.
    Please use usb3dev_waitReadDone() to wait for transfer done
    and get the returned real transfer size.

    @param[in] EPn          ISOCHRONOUS endpoint number. Only allow (USB_EP1 ~ USB_EP8)
    @param[in] pIsoData     ISOCHRONOUS transfer data information. Please refer to PU3DEV_ISODATA for details.

    @return
        - @b E_OACV: EPn number not valid or not ISOCHRONOUS endpoint.
        - @b E_OK:   Start to transfer
*/
extern ER               usb3dev_setIsoEPRead(USB_EP EPn, PU3DEV_ISODATA pIsoData);

/**
    Wait for Out endpoint transfer done

    This function is used after usb3dev_setIsoEPRead() or usb3dev_setEPRead().
    It will Wait until transfer done and return actual transfered length.

    @param[in] EPn          endpoint number (USB_EP0~USB_EP8)
    @param[in,out] pDMALen  input DMA length (1~0x7fffff), output actual transfered length

    @return
        - @b E_OACV: USB driver is not opened
        - @b E_OK: DMA done (maybe aborted, check return pDMALen)
*/
extern ER               usb3dev_waitReadDone(USB_EP EPn, UINT32 *pDMALen);

/**
	Set USB IN direction Endpoint STALL

	Set USB IN direction Endpoint STALL.

    @param[in] EPn IN endpoint number (USB_EP0~USB_EP8)

    @return void
*/
extern void             usb3dev_setEPinStall(USB_EP EPn);

/**
	Set USB OUT direction Endpoint STALL

	Set USB OUT direction Endpoint STALL.

    @param[in] EPn OUT endpoint number (USB_EP0~USB_EP8)

    @return void
*/
extern void             usb3dev_setEPoutStall(USB_EP EPn);

extern void             usb3dev_installID(void) _SECTION(".kercfg_text");
#endif
