#ifndef _MSDCNVTCB_H
#define _MSDCNVTCB_H
#include "UMSDC.h"
#include "GxUSB.h"

typedef struct _MSDCNVTCB_OPEN {
	GXUSB_HDL_ID GxUsbHdlID;
	void (*fpUSBMakerInit)(USB_MSDC_INFO *pUSBMSDCInfo);
} MSDCNVTCB_OPEN, *PMSDCNVTCB_OPEN;

typedef struct _MSDCNVTCB_ATTACH {
	GXUSB_HDL_ID GxUsbHdlID;
	USB_MSDC_INFO *pInfo;
} MSDCNVTCB_ATTACH, *PMSDCNVTCB_ATTACH;

extern void MsdcNvtCb_Open(MSDCNVTCB_OPEN *pOpen);
extern void MsdcNvtCb_Close(void);
extern void MsdcNvtCb_Attach(MSDCNVTCB_ATTACH *pAttach);
extern void MsdcNvtCb_Net(BOOL bTurnOn);
extern GXUSB_HDL_ID MsdcNvtCb_GetConnectedID(void);

extern void IspSudoCb_Net(BOOL bTurnOn);

#endif
