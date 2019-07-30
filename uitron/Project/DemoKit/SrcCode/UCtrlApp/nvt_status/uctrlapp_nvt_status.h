#ifndef _UCTRLAPP_NVT_STATUS_H
#define _UCTRLAPP_NVT_STATUS_H
#include "Type.h"
#include "uToken.h"

BOOL uctrlapp_nvt_status_init(UINT32 work_addr, UINT32 work_size);
UTOKEN_MODULE *uctrlapp_nvt_status_get_utoken_module(void);
UTOKEN_MODULE *ramoops_get_utoken_module(void);
void uctrlapp_nvt_status_installcmd(void);

#endif //_UCTRLAPP_NVT_STATUS_H
