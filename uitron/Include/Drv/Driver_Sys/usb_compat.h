/*
    Header file for USB driver backward compatibility

    Header file for USB driver backward compatibility.

    @file       usb_compat.h
    @ingroup    mIDrvUSB_Device
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _USB_COMPAT_H
#define _USB_COMPAT_H

#include "Type.h"

/**
    @addtogroup mIDrvUSB_Device
*/
//@{

//-------------------------------------------------
// USB Compatible driver API
//-------------------------------------------------
#define USBStateChange()            usb_StateChange()
#define USBPowerOnInit(bHighSpeed)  usb_PowerOnInit(bHighSpeed)

#define usb_setEP0Stall()           usb_setEPStall(USB_EP0)
#define usb_setEPinStall(x)         usb_setEPStall((x))
#define usb_setEPoutStall(x)        usb_setEPStall((x))

//@}
#endif //_USB_COMPAT_H
