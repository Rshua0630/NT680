#ifndef _DPOF_VENDOR_H
#define _DPOF_VENDOR_H
#include "Type.h"


typedef enum {
	DPOF_OP_SET_CURR,
	DPOF_OP_SET_ALL,
	ENUM_DUMMY4WORD(DPOF_OP_MODE)
} DPOF_OP_MODE;
typedef enum {
	DPOF_INFO_CURR_QTY,
	DPOF_INFO_CURR_DATE_STATUS,
	DPOF_INFO_IF_ALL_IMG_SET_TO_DPOF,
	ENUM_DUMMY4WORD(DPOF_INFO)
} DPOF_INFO;

extern void DpofVendor_Open(UINT32 buffAddr);
extern void DpofVendor_Close(void);
extern void DpofVendor_Update(void);
extern void DpofVendor_SetDPOFQty(DPOF_OP_MODE DpofOPMode, UINT32 DpofPrtNum, BOOL DpofDateOn);
extern UINT32 DpofVendor_GetDPOFInfo(DPOF_INFO DpofInfo);
extern void DpofVendor_DPOFSetCreatorName(char *pRetVal, UINT32 length, BOOL bUpdateNow);


#endif//_DPOF_VENDOR_H
