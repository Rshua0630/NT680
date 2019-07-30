#ifndef _UCTRLAPP_ONVIF_H
#define _UCTRLAPP_ONVIF_H
#include "Type.h"

extern void       UCtrlAppOnvif_InstallID(void) _SECTION(".kercfg_text");
extern void       UCtrlAppOnvif_Open(void);
extern void       UCtrlAppOnvif_Close(void);
extern void       UCtrlAppOnvif_InstallCmd(void);
#endif //_UCTRLAPP_ONVIF_H