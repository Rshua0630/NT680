#ifndef _BT_EXTBRCM_INT_H
#define _BT_EXTBRCM_INT_H
#include "Type.h"
#include "BT_API.h"

extern UINT8 bt_bda[6];

// flag define
#define FLG_BT_BLE_OFF       0x00000001

extern BT_RCKEY_CB        *BT_GetRCKEY_CB(void);
extern BT_EVENT_CB        *BT_GetEvent_CB(void);
extern BT_RFCOMM_RX_CB    *BT_GetRfcomm_Rx_CB(void);
extern BT_BLE_RCVDATA_CB  *BT_GetBleRcvData_CB(void);
extern void                BT_StoreInfo(void);
extern void                BT_BLE_SetScanOff(void);
#endif //_BT_EXTBRCM_INT_H