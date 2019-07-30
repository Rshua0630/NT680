#ifndef _USBDEVDEF_H
#define _USBDEVDEF_H

/**
     @name USB connection type
*/
//@{
#define USB_CONNECT_NONE                     0   ///< not connected
#define USB_CONNECT_PC                       1   ///< connect to PC
#define USB_CONNECT_CHARGER                  2   ///< connect to charger
#define USB_CONNECT_CHARGING_PC              3   ///< connect to PC which is charger capable
#define USB_CONNECT_UNKNOWN         0xffffffff   ///< connect to an unknown device
//@}

#endif //_USBDEVDEF_H