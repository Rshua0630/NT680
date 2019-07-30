#ifndef _UCTRLAPP_PHOTO_H
#define _UCTRLAPP_PHOTO_H
#include "Type.h"

extern void       UCtrlAppPhoto_InstallID(void) _SECTION(".kercfg_text");
extern void       UCtrlAppPhoto_CallBack(UINT32 callBackEvent);
extern void       UCtrlAppPhoto_Open(void);
extern void       UCtrlAppPhoto_Close(void);
#endif //_UCTRLAPP_PHOTO_H