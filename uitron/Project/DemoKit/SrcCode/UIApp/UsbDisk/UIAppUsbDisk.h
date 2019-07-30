//#NT#2010/07/29#Lily Kao -begin
#ifndef _UIAPPUSBDISK_H_
#define _UIAPPUSBDISK_H_

#include "UMSDC.h"
#include "NVTEvent.h"

#include "AppLib.h"
#include "UIAppCommon.h"


extern void USBMakerInit_UMSD(USB_MSDC_INFO *pUSBMSDCInfo);
//#NT#2010/12/13#Lily Kao -begin
extern ER AppInit_ModeUSBMSDC(void);
//#NT#2010/12/13#Lily Kao -end

extern VControl CustomMSDCObjCtrl;

#endif
//#NT#2010/07/29#Lily Kao -end
