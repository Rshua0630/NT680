/**
    DxInput module

    Physical layer to detect USB connection status. User should implement these APIs before using GxUSB.

    @file       DxUSB.h
    @ingroup    mIDxUSB

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _DXUSB_H_
#define _DXUSB_H_

#include "Type.h"
#include "Dx.h"

//ver
#define DETUSB_VER                 0x00010000

//caps id
#define DETUSB_CAPS_BASE           0x00000001
#define DETUSB_CAPS_PLUG           0x00000002
#define DETUSB_CAPS_CONN_TYPE      0x00000003

//caps base flag
#define DETUSB_BF_DETPLUG          0x00000001
#define DETUSB_BF_CONNTYPE         0x00000002

//config ID
#define DETUSB_CFG_STANDARD_CHARGER  	0x00000001
//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection 
#define DETUSB_CFG_CHARGER_EVT_CALLBACK  0x00000002
//#NT#2018/01/12#Ben Wang -end
//@}
#endif //_DXUSB_H_
