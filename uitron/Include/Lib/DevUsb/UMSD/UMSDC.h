/**
    Header file for USB MSDC Task APIs

    This is the public header file for the USB Mass-Storage-Device-Class task.
    Before using these APIs in the header file, the user must use Msdc_InstallID()
    to retrieve the system task ID and the flag ID for MSDC Task usage to keep the task normal working.

    @file       UMSDC.h
    @ingroup    mILibUsbMSDC
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/

#ifndef _UMSDC_H
#define _UMSDC_H

#include "Type.h"
#include "Dx.h"
#include "usb_desc.h"
#include "usb_define.h"

/**
    @addtogroup mILibUsbMSDC
*/
//@{


/**
    The Minimum required DRAM buffer size for the MSDC Task
*/
//@{
#define MSDC_MIN_BUFFER_SIZE        0x044040                       	///< [USB2/3]: Minimunm required buffer size is (256K+64) Bytes + 16KB for USB3.
#define MSDC_GOOD_BUFFER_SIZE      	0x084040						///< [USB3]:   Buffer size to make MSDC get good performance
#define MSDC_MAX_BUFFER_SIZE      	0x104040						///< [USB3]:   Maximum Buffer size to make MSDC get good performance

#define MSDC_WCACHE_MIN_BUFFER_SIZE 0x100000						///< [USB2/3]: Minimum Write Cache Buffer Size.
																	///< [USB3]:   This size can up to 4MB ot 8MB to make every SDCARD get very good write speed for USB3 controller.
																	///< [USB2]:   For USB2.0 controller, use MSDC_WCACHE_MIN_BUFFER_SIZE is enough.
//@}


/**
    The Maximum supported LUN number of the MSDC Task
*/
//@{
#define MAX_LUN                     6                               ///< MSDC Task maximum Logical-Unit-Number supported
//@}

/**
    Structure used for MSDC Vendor Command Callback API

    This structure is used in the callback prototype MSDC_Vendor_CB to specify the needed parameters for vendor command processing.
*/
typedef struct {
	UINT32    OutDataBufLen;                                        ///< Data Buffer Length for "Host to MSDC-task" transaction.

	UINT32    InDataBuf;                                            ///< Data Buffer Address for "MSDC-task to Host" transaction. MSDC-task would prepare this buffer for callback function.
	UINT32    InDataBufLen;                                         ///< Host assigned "MSDC-task to Host" transaction length in byte count.

	UINT32    VendorCmdBuf;                                         ///< Vendor CBW buffer that device will receive from host for CBW buffer. Fixed 31 bytes.
	UINT32    VendorCSWBuf;                                         ///< Vendor CSW buffer that device will return to host for CSW buffer, MSDC task should prepare this buffer for callback function, Fixed 13 bytes.
} MSDCVendorParam, *PMSDCVendorParam;

/**
    Callback functions prototype that MSDC task used

    These definitions are used in the structure USB_MSDC_INFO and would be used during Msdc_Open() to specify the
    MSDC_Task needed information.
*/
//@{
typedef void (*MSDC_Vendor_CB)(PMSDCVendorParam pBuf);              ///< Callback for Vendor Command processing
typedef BOOL (*MSDC_Verify_CB)(UINT32 pCmdBuf, UINT32 *pDataBuf);   ///< Callback for verify the Vendor Command is supported or not
typedef BOOL (*MSDC_StorageDet_CB)(void);                           ///< Callback for detecting the storage card is plugging or not.
typedef BOOL (*MSDC_StrgLockDet_CB)(void);                          ///< Callback for detecting the storage card lock status.
typedef void (*MSDC_Led_CB)(void);                                  ///< Callback for flashing the LED when MSDC under R/W function.
//@}

/**
    MSDC Task Current Status Flag

    These definitions are used as the return value identification for API Msdc_GetStatus().
*/
typedef enum {
	MSDC_COMMANDOK,                                                 ///< Previous command that MSDC task processed is done and OK.
	MSDC_CLOSED_STATE,                                              ///< MSDC task has not opened yet.
	MSDC_USBRESETED,                                                ///< MSDC task got the RESET signaling from Host.
	MSDC_PHASEERROR,                                                ///< MSDC commands handshake phase error.
	MSDC_READ_BUSY_STATE,                                           ///< MSDC is still reading data from storage.
	MSDC_WRITE_BUSY_STATE,                                          ///< MSDC is still writing data to storage.

	ENUM_DUMMY4WORD(MSDC_TASK_STS)
} MSDC_TASK_STS;

/**
    Mass Storage Logical Unit Type

    This definition is used USB_MSDC_INFO to specify each the MSDC LUN would behave as STORAGE or CDROM during Msdc_Open().
*/
typedef enum {
	MSDC_STRG = 0,                                                  ///< Set the specified LUN as storage device.
	MSDC_CDROM,                                                     ///< Set the specified LUN as CDROM read only device.

	ENUM_DUMMY4WORD(MSDC_TYPE)
} MSDC_TYPE;

/**
    MSDC Device Power Source

    This definition is used in Msdc_SetConfig(USBMSDC_CONFIG_ID_SELECT_POWER) to select the MSDC Device power source.
    This feature is used to change the descriptors only to notify the MSDC Host the power source of the device.
*/
typedef enum {
	USBMSDC_POW_SELFPOWER,                                          ///< USB Device is Self-Powered.
	USBMSDC_POW_BUSPOWER,                                           ///< USB Device is Bus-Powered.

	ENUM_DUMMY4WORD(USBMSDC_POWER)
} USBMSDC_POWER;

/**
    MSDC Configuration identifier

    This definition is used in Msdc_SetConfig() to assgin new configuration to specified ID function.
*/
typedef enum {
	USBMSDC_CONFIG_ID_COMPOSITE,                                    ///< Select composite device support
																	///< Context can be ORed of:
																	///< - @b FALSE (Defalut value): disable composite device support
																	///< - @b TRUE: enable composite device support
	USBMSDC_CONFIG_ID_CHGVENINBUGADDR,                              ///< Change Vendor command IN data buffer address
	USBMSDC_CONFIG_ID_SELECT_POWER,                                 ///< Selct Device Power Source from USBMSDC_POW_SELFPOWER or USBMSDC_POW_BUSPOWER.
																	///< The Default value is Self-Powered (USBMSDC_POW_SELFPOWER).
	USBMSDC_CONFIG_ID_SERIALNO_STRING_EN,                           ///< Set serial number string descriptor enable/disable. Default value is enable.
	USBMSDC_CONFIG_ID_RC_DISABLESTALL,                              ///< When non of card inserted, this can send dummy capacity data without STALL
																	///< EP for the ReadCapacity command.
	USBMSDC_CONFIG_ID_SPEED,                                        ///< Specified USB highest Speed for MSDC operation. Please use U3DEV_SPEED as input parameter.
	USBMSDC_CONFIG_ID_WCACHE_ADDR,									///< Assign Write Cache buffer Address. Default is NULL
	USBMSDC_CONFIG_ID_WCACHE_SIZE,									///< Assign Write Cache buffer Size. Default is 0. Minimum size is MSDC_WCACHE_MIN_BUFFER_SIZE.
	USBMSDC_CONFIG_ID_WCACHE_SLICE,									///< Default is 128. Lower value can make MSDC write speed better in small size write. But induce more interrupts.

	USBMSDC_CONFIG_ID_RESERVED,                                     //   Novatek internal usage. Change EP number mapping.
	ENUM_DUMMY4WORD(USBMSDC_CONFIG_ID)
} USBMSDC_CONFIG_ID;

/**
    MSDC Logical Unit Configuration identifier

    This definition is used in Msdc_SetLunConfig() to assgin new configuration to specified ID function.
*/
typedef enum {
	USBMSDC_LUN_CONFIG_ID_FORCE_EJECT,                              ///< Force LUN to eject. TURE: Force Eject. FALSE: NOT forcing eject.
	USBMSDC_LUN_CONFIG_ID_WRITEPROTECT,                             ///< Force LUN WriteProtect. TRUE: Force WriteProtect. FALSE: NOT forcing WriteProtect.

	ENUM_DUMMY4WORD(USBMSDC_LUN_CONFIG_ID)
} USBMSDC_LUN_CONFIG_ID;

/**
    MSDC Storgae Information

    MSDC Storgae Information including storage object, storage detect/lock callback and also the sd info callback if the
    storage is the SD card. This API is used in Msdc_ChgStrgObject() to allow the user change the storage device dynamically.

*/
typedef struct {
	DX_HANDLE           StrgHandle;                                 ///< Pointer of each LUN's Dx storage handle.
	MSDC_StorageDet_CB  msdc_storage_detCB;                         ///< Callback function to detect storage plug status. If there is none, please set to NULL
	MSDC_StrgLockDet_CB msdc_strgLock_detCB;                        ///< Callback function to detect storage lock status. If there is none, please set to NULL

} USB_MSDC_STRG_INFO, *PUSB_MSDC_STRG_INFO;


/**
    Information needed for USB MSDC Task

    This structure is used in Msdc_Open() to specify the msdc task needed information for normal working
    and user customization callback, including task working buffer address/size and logical unit number and type.
*/
typedef struct {
	MSDC_TYPE           msdc_type[MAX_LUN];                         ///< Select specified LUN act as STORAGE or CDROM device.

	UINT32              uiMsdcBufAddr;                              ///< Buffer address for MSDC-Task operation.
	UINT32              uiMsdcBufSize;                              ///< Buffer size for MSDC-Task operation. The minimum requirement is 0x40040 Bytes.

	USB_STRING_DESC     *pManuStringDesc;                           ///< USB Manufacturer String Descriptor pointer.
	USB_STRING_DESC     *pProdStringDesc;                           ///< USB Product String Descriptor pointer.
	USB_STRING_DESC     *pSerialStringDesc;                         ///< USB Serial Number String Descriptor pointer.
	UINT8               *pInquiryData;                              ///< SCSI Inquire data
	UINT16              VID;                                        ///< Customized USB Vendor ID
	UINT16              PID;                                        ///< Customized USB Product ID

	MSDC_Verify_CB      msdc_check_cb;                              ///< Callback function to check if the command is a supported vendor command.
	MSDC_Vendor_CB      msdc_vendor_cb;                             ///< Vendor command processing callback function

	UINT8               LUNs;                                       ///< Number of LUNs supported by MSDC Task.

	DX_HANDLE           pStrgHandle[MAX_LUN];                       ///< Pointer of each LUN's Dx storage Handle.
	MSDC_StorageDet_CB  msdc_storage_detCB[MAX_LUN];                ///< Callback function to detect storage plug status. If there is none, please set to NULL
	MSDC_StrgLockDet_CB msdc_strgLock_detCB[MAX_LUN];               ///< Callback function to detect storage lock status. If there is none, please set to NULL

	// Add Customization backward compatible
	MSDC_Led_CB         msdc_RW_Led_CB;                             ///< Callback when RW storage is running.
	MSDC_Led_CB         msdc_StopUnit_Led_CB;                       ///< Callback when host issue StopUnit cmd.
	MSDC_Led_CB         msdc_UsbSuspend_Led_CB;                     ///< Callback when host issue usb suspend.

} USB_MSDC_INFO, *PUSB_MSDC_INFO;



/**
    MSDC Object ID

    This is used by the MSDC Get Object API MSDC_getObject() to get the specific MSDC control object.
*/
typedef enum {
	MSDC_ID_USB20,	///< USB 2.0 Port MSDC Object
	MSDC_ID_USB30,	///< USB 3.0 Port MSDC Object

	MSDC_ID_MAX,
	ENUM_DUMMY4WORD(MSDC_ID)
} MSDC_ID;




/**
    Mass Storage Object Definitions
*/
typedef struct {
	MSDC_ID			MsdcID;

	ER				(*Open)(PUSB_MSDC_INFO pClassInfo);
	void			(*Close)(void);
	ER 				(*SetConfig)(USBMSDC_CONFIG_ID ConfigID, UINT32 uiCfgValue);
	UINT32 			(*GetConfig)(USBMSDC_CONFIG_ID ConfigID);
	ER 				(*SetLunConfig)(UINT32 uiLun, USBMSDC_LUN_CONFIG_ID ConfigID, UINT32 uiCfgValue);
	UINT32 			(*GetLunConfig)(UINT32 uiLun, USBMSDC_LUN_CONFIG_ID ConfigID);
	void    		(*ChgStrgObject)(UINT32 uiLun, PUSB_MSDC_STRG_INFO pStrgInfo);
	MSDC_TASK_STS   (*GetStatus)(void);
	BOOL            (*CheckIdle)(void);

} MSDC_OBJ, *PMSDC_OBJ;


//
//  I2C Object APIs
//
extern PMSDC_OBJ         	Msdc_getObject(MSDC_ID MsdcID);


//
//  MSDC Task Export APIs
//
extern void             	Msdc_InstallID(void)   _SECTION(".kercfg_text");
extern void             	U2Msdc_InstallID(void) _SECTION(".kercfg_text");

extern ER               	U3Msdc_Open(PUSB_MSDC_INFO pClassInfo);
#define Msdc_Open 			U3Msdc_Open

extern void             	U3Msdc_Close(void);
#define Msdc_Close 			U3Msdc_Close

extern ER               	U3Msdc_SetConfig(USBMSDC_CONFIG_ID ConfigID, UINT32 uiCfgValue);
#define Msdc_SetConfig 		U3Msdc_SetConfig

extern UINT32           	U3Msdc_GetConfig(USBMSDC_CONFIG_ID ConfigID);
#define Msdc_GetConfig 		U3Msdc_GetConfig

extern ER               	U3Msdc_SetLunConfig(UINT32 uiLun, USBMSDC_LUN_CONFIG_ID ConfigID, UINT32 uiCfgValue);
#define Msdc_SetLunConfig 	U3Msdc_SetLunConfig

extern UINT32           	U3Msdc_GetLunConfig(UINT32 uiLun, USBMSDC_LUN_CONFIG_ID ConfigID);
#define Msdc_GetLunConfig 	U3Msdc_GetLunConfig

extern void             	U3Msdc_ChgStrgObject(UINT32 uiLun, PUSB_MSDC_STRG_INFO pStrgInfo);
#define Msdc_ChgStrgObject 	U3Msdc_ChgStrgObject

extern MSDC_TASK_STS    	U3Msdc_GetStatus(void);
#define Msdc_GetStatus 		U3Msdc_GetStatus

extern BOOL             	U3Msdc_CheckIdle(void);
#define Msdc_CheckIdle 		U3Msdc_CheckIdle


//@}

#endif
