#ifndef _UCTR_MAIN_H
#define _UCTR_MAIN_H
#include "Type.h"
#include "NVTEvent.h"

extern void       UCtrlMain_InstallID(void) _SECTION(".kercfg_text");
extern void       UctrlMain_Init(void);
extern void       UctrlMain_EventEnd(int ret, int Event);
extern void       UctrlMain_SendEvent(NVTEVT evt, UINT32 paramNum, ...);
extern void       UctrlMain_Lock(void);
extern void       UctrlMain_UnLock(void);
#endif //_UCTR_MAIN_H