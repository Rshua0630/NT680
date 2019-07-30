#ifndef _UCTRLAPP_SYS_H
#define _UCTRLAPP_SYS_H
#include "Type.h"


void UCtrlAppSys_WaitNetInitFinish(void);
void UCtrlAppSys_InstallID(void) _SECTION(".kercfg_text");
void UCtrlAppSys_InstallCmd(void);

#endif //_UCTRLAPP_SYS_H