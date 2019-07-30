/**
    Header file for USB device descriptor

    This file is the header file for USB device descriptor

    @file       usb_desc.h
    @ingroup    mIDrvUSB_Device
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _USBDESC_H
#define _USBDESC_H

/**
    @addtogroup mIDrvUSB_Device
*/
//@{


/**
    USB Device Descriptor Data Structure
*/
typedef  struct Device_Descriptor {
	UINT8       bLength;                ///< size of Device Descriptor
	UINT8       bDescriptorType;        ///< Device Dscriptor type
	UINT16      bcdUSB;                 ///< number of USB specifications
	UINT8       bDeviceClass;           ///< class code
	UINT8       bDeviceSubClass;        ///< sub class code
	UINT8       bDeviceProtocol;        ///< protocol code
	UINT8       bMaxPacketSize0;        ///< max packt size of endpoint0
	UINT16      idVendor;               ///< Vendor id
	UINT16      idProduct;              ///< Protocol id
	UINT16      bcdDevice;              ///< Device nmber
	UINT8       iManufacturer;          ///< index of string Desc(maker)
	UINT8       iProduct;               ///< index of string Desc(products)
	UINT8       iSerialNumber;          ///< index of string Desc(serial number)
	UINT8       bNumConfigurations;     ///< number for configration
} USB_DEVICE_DESC, *PUSB_DEVICE_DESC;

/**
    USB Configuration Descriptor Data Structure
*/
typedef  struct Configuration_Descriptor {
	UINT8       bLength;                ///< size of Configuration Descriptor
	UINT8       bDescriptorType;        ///< Configuration Descriptor type
	UINT16      wTotalLength;           ///< all length of data
	UINT8       bNumInterfaces;         ///< number of interface
	UINT8       bConfigurationValue;    ///< value of argument
	UINT8       iConfiguration;         ///< index of string Descriptor
	UINT8       bmAttributes;           ///< characteristic of composition
	UINT8       MaxPower;               ///< max power consumption
} USB_CONFIG_DESC, *PUSB_CONFIG_DESC;

/**
    USB Interface Descriptor Data Structure
*/
typedef struct {
	UINT8       bLength;                ///< size of Configuration Descriptor
	UINT8       bDescriptorType;        ///< Configuration Descriptor type
	UINT8       bInterfaceNumber;       ///< Number of this interface
	UINT8       bAlternateSetting;      ///< Value used to select this alternate setting for the interface.
	UINT8       bNumEndpoints;          ///< Number of endpoints used by this interface.
	UINT8       bInterfaceClass;        ///< Class Code
	UINT8       bInterfaceSubClass;     ///< SubClass Code
	UINT8       bInterfaceProtocol;     ///< Protocol Code
	UINT8       iInterface;             ///< index of string descriptor
} USB_INTF_DESC, *PUSB_INTF_DESC;

/**
    USB Endpoint Descriptor Data Structure
*/
typedef struct {
	UINT8       bLength;                ///< size of Configuration Descriptor
	UINT8       bDescriptorType;        ///< Configuration Descriptor type
	UINT8       bEndpointAddress;       ///< The Address of this endpoint
	UINT8       bmAttributes;           ///< Endpoint Attributes bitmap
	UINT16      wMaxPacketSize;         ///< Maximum Packet Size
	UINT8       bInterval;              ///< Minimum transaction Interval
} USB_ENDPOINT_DESC, *PUSB_ENDPOINT_DESC;

/**

*/
typedef struct {
	UINT8       bLength;                ///< size of SS Endpoint Companion Descriptor
	UINT8       bDescriptorType;        ///< SS Endpoint Companion Descriptor type
	UINT8       bMaxBurst;              ///< Max Burst Number. Valid Value from 0-15.
	UINT8       bmAttrubutes;           ///< For BULK, [4:0] is MaxStreams. [7:5] is reserved to 0.
	UINT16      wBytesPerInterval;      ///<
} USB_EPCOMPANION_DESC, *PUSB_EPCOMPANION_DESC;



/**
    String Descriptor

    @note For pStringDesc of USB_MNG
*/
typedef  struct String_Descriptor {
	UINT8       bLength;                ///< size of String Descriptor
	UINT8       bDescriptorType;        ///< String Descriptor type
	UINT8       bString[254];           ///< UNICODE stirng
} USB_STRING_DESC, *PUSB_STRING_DESC;


/**
    @name   USB device release number

    @note For usb_SetDevDescUSBVerRelNum()
*/
//@{
#define USB_HS_DEV_RELEASE_NUMBER       0x0200  ///< spec. release number
#define USB_FS_DEV_RELEASE_NUMBER       0x0110  ///< spec. release number
//@}


//@}

#endif  //_USBDESC_H

