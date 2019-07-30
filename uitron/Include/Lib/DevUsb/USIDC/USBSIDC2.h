/**
    USB SIDC device class API header.

    @file       USBSIDC2.h
    @ingroup    mIUSIDC

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef _USBSIDC2_H
#define _USBSIDC2_H

/***********************************
*   Includes
************************************/
#include "Type.h"
#include "time.h"
/**
    @addtogroup mIUSIDC
*/
//@{

/**
     @name Minimum buffer size for the PTP command SEND_OBJECT.
*/
//@{
//modified for parsing AVI thumbnail
//#define MIN_BUFFER_SIZE_FOR_SENDOBJ     0x240000
#define MIN_BUFFER_SIZE_FOR_SENDOBJ     0x400000//GXVIDEO_PARSE_HEADER_BUFFER_SIZE
//@}

/**
     @name Can mount sxdc card, but can not access if this is set to be 1.
*/
//@{
#define _SDXC_MTP_MOUNT_NOTACCESS_          0
//@}
#if _SDXC_MTP_MOUNT_NOTACCESS_
#include "sdio.h"

/**
    SD card type call-back function
*/
typedef PSDIO_MISC_INFORMATION(*USIDC_GET_CARDTYPE_FUNC)(void);
#endif


/**
    Picture Bridge call-back function
*/
typedef void (*USIDCPTR_CB)(UINT8 cancel);

/**
    String Descriptor
*/
typedef  struct _SIDC_STRING_DESC {
	UINT8       bLength;                ///< size of String Descriptor
	UINT8       bDescriptorType;        ///< String Descriptor type
	UINT8       bString[254];           ///< UNICODE stirng
} SIDC_STRING_DESC;

/**
    USB Infomation Table for SIDC.
*/
typedef struct _USB_SIDC_INFO {
	SIDC_STRING_DESC     *pManuStringDesc;   ///< USB Manufacturer String Descriptor ptr
	SIDC_STRING_DESC     *pProdStringDesc;   ///< USB Product String Descriptor ptr
	SIDC_STRING_DESC     *pSerialStringDesc; ///< USB Serial Number String Descriptor ptr
	UINT16              VID;                ///< Customized USB Vendor ID
	UINT16              PID;                ///< Customized USB Product ID
	char                *pSIDCString;       ///< SIDC identification string
	UINT16              SIDCStringLen;      ///< SIDC identification string length
	UINT16              uiStrgCardLock;     ///< CardLock should be set when usidc_open()
	UINT32              uiSidcBufAddr;      ///< Buffer address for USB operation
	UINT32              uiSidcBufSize;      ///< Buffer size for USB operation
	char                *pPTPFWUpdateFilename;///< The file name for PTP FW update
	UINT32              uiMemoryAddr;       ///< Memory start address for PTP firmware download.
	UINT32              uiMemorySize;       ///< Memory size for PTP firmware download.
#if _SDXC_MTP_MOUNT_NOTACCESS_
	USIDC_GET_CARDTYPE_FUNC pGetCardTypeFunc;///< sd card type call-back function
#endif
	//#NT#2010/09/24#Lily Kao -begin
	//#Bus-Power or Self-Power is decided by project.Bus-Powered is default.
	UINT32              isSelfPwred; ///< Is self powerd or not.
	//#NT#2010/09/24#Lily Kao -end
} USB_SIDC_INFO;


/**
     Install SIDC task,flag and semaphore id

     This is a default setting API. Users can create their own task id, semphore id,
     flag id to adjust own stack size and task priority as the following:

     @code
     {
        #define PRI_USBSIDC             10
        #define PRI_USBSIDCEP3           8
        #define STKSIZE_USBSIDCVND      2048
        #define STKSIZE_USBSIDCEP3      2048

        UINT32 FLG_ID_USBCLASS_SIDC = 0;
        UINT32 FLG_ID_DPS = 0;
        UINT32 USBSIDCTSK_ID = 0;
        UINT32 USBSIDCEP3TSK_ID = 0;

        void Install_UserSidcID(void)
        {
            OS_CONFIG_FLAG(FLG_ID_USBCLASS_SIDC);
            OS_CONFIG_FLAG(FLG_ID_DPS);
            OS_CONFIG_TASK(USBSIDCTSK_ID,   PRI_USBSIDC,  STKSIZE_USBSIDCVND,   USBSIDCTsk);
            OS_CONFIG_TASK(USBSIDCEP3TSK_ID,   PRI_USBSIDCEP3,  STKSIZE_USBSIDCEP3,   USBSIDCEP3Tsk);
        }

     }
     @endcode
*/
extern void Sidc_InstallID(void) _SECTION(".kercfg_text");

/**
    USB SIDC service main task.
*/
extern void USBSIDCTsk(void);

/**
    USB SIDC EP3 Task.

    Processing of USB EP3.
*/
extern void USBSIDCEP3Tsk(void);

/**
    Open USB SIDC Task.

    @param[in] pClassInfo Pointer to the structure of USB SIDC class information: #_USB_SIDC_INFO.
    @return
        - @b E_OK:  Success.
        - @b E_SYS: USIDC module is opened already.
        - @b E_PAR: Parameter Error. For example, you don't assign SIDC buffer address and size.
*/
extern ER   Sidc_Open(USB_SIDC_INFO *pClassInfo);

/**
    Close USB SIDC Task.
*/
extern void Sidc_Close(void);

/**
    Set buffer address and size for the command SEND_OBJECT

    @param[in] addr The address of the buffer.
    @param[in] size The size of the buffer.
*/
extern void Sidc_SetSendObjBufAddrSize(UINT32 addr, UINT32 size);

/**
    Enable MTP function.

    @param[in] byEnable 1: Enable MTP module; 0: Disable MTP module.

*/
extern void MTP_Enable(UINT8 byEnable);

/**
    Check if MTP is enabled.

    @return 1: MTP module is enabled; 0: MTP module is disabled.
*/
extern UINT8 MTP_IsMTPEnabled(void);

/**
    Device friendly name to be sent to initiaotr when USIDCOp_GetDevicePropDesc()
    operation along with PROP_DEVICE_FRIENDLY_NAME property is received.

    @param[in] pDeviceFriendlyName Pointer to the string of Device friendly name.
               A constant string, such as "ABCDXXX". Max string lenght is 255.
*/
extern void MTP_SetDeviceFriendlyName(const char *pDeviceFriendlyName);

/**
    Device friendly name to be sent to initiaotr when USIDCOp_GetDevicePropDesc()
    operation along with PROP_DEVICE_FRIENDLY_NAME property is received.

    @return The string of device friendly name.
               A constant string, such as "ABCDXXX"
*/
extern char *MTP_GetDeviceFriendlyName(void);

/**
    Set device icon.

    @param[in] pIcon Pointer to icon data.
    @param[in] len The length of icon data.
    @return 0: succeed; 1: DeviceIcon is too big.
*/
extern UINT8 MTP_SetDeviceIcon(const UINT8 *pIcon, UINT32 len);

/**
    Get device icon.

    @param[in] pLen Icon data length.
    @return The pointer of icon data.
*/
extern UINT8 *MTP_GetDeviceIcon(UINT32 *pLen);

/**
    Set battery level.

    @param[in] batLevel Battery level.
*/
extern void PTP_SetBatteryLevel(UINT8 batLevel);

/**
    Get battery level.

    @return Battery level.
*/
extern UINT8 PTP_GetBatteryLevel(void);

/**
    Let MTP use specified device info or not.

    @param[in] isTrue Set TRUE for vendor specified device information
*/
extern void MTP_SetMTPVendorSpeciDevInfo(UINT8 isTrue);

/**
    The prototype of PTP device date time callback function
*/
typedef void (*PTPDevDateTime_CB)(time_t Time);

/**
    Register device date time callback.

    The callback is invoked when the host send the command SetDevicePropValue for DateTime.

    @param[in] pCBFunc Device date time callback function
*/
extern void PTP_RegistryDevDateTimeCallBack(PTPDevDateTime_CB pCBFunc);

/**
    The prototype of SIDC data transfer callback function.

    Start and Stop LED twinkle

    isStart = 0: stop shining;Otherwise start shinging.
*/
typedef void (*USIDCDataTxf_CB)(UINT8 isStart);

/**
    Register callback function for data transfer.

    @param[in] pCBFunc SIDC data transfer callback function.
*/
extern void Sidc_RegDataTxfCB(USIDCDataTxf_CB pCBFunc);

/**
    PTP storage type
*/
typedef enum _PTP_STORETYPE_ENUM {
	PTP_STORETYPE_FIX = 0,
	PTP_STORETYPE_REMOVABLE,
	PTP_STORETYPE_MAX,
	ENUM_DUMMY4WORD(PTP_STORETYPE_ENUM)
} PTP_STORETYPE_ENUM;

/**
    The prototype of PTP storage type callback function
*/
typedef UINT32(*PTPStorType_CB)(void);

/**
    Register callback function for storage type.

    The callback is invoked when the host issue the command GetStorageInfo.

    @note The return value of callback function should be eather PTP_STORETYPE_FIX or PTP_STORETYPE_REMOVABLE.
    @param[in] pCBFunc PTP storage type callback function.
*/
extern void PTP_RegistryStorTypeCB(PTPStorType_CB pCBFunc);

/**
    Set storage ID.

    @param[in] storeId storage ID
*/
extern void PTP_SetStorageId(UINT32 storeId);

//@}
#endif
