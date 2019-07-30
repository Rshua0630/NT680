
#ifndef _BT_API_H
#define _BT_API_H


/** BT events */
typedef enum
{
    BT_CONNECTION_STATUS_CONNECTED,       ///<  BT connection status connected
    BT_CONNECTION_STATUS_DISCONNECTED,    ///<  BT connection status disconnected
    ENUM_DUMMY4WORD(BT_EVENT)
} BT_EVENT;



typedef void  BT_LOADINFO_CB(UINT32 Addr, UINT32 Size);
typedef void  BT_SAVEINFO_CB(UINT32 Addr, UINT32 Size);
typedef void  BT_RCKEY_CB(UINT8* KeyData, UINT32 DataLen);
typedef void  BT_EVENT_CB(BT_EVENT event);
typedef void  BT_RFCOMM_RX_CB(UINT16 port_handle, void *p_data, UINT16 len);
typedef void  BT_BLE_RCVDATA_CB(void *p_data, UINT16 len);


typedef struct{
    BT_LOADINFO_CB  *loadInfo;            ///<  Callback function load BT info from storage
    BT_SAVEINFO_CB  *saveInfo;            ///<  Callback function save BT info to storage
    BT_RCKEY_CB     *rckeyCB;             ///<  Callback function of BT receive remote control key
    BT_EVENT_CB     *eventCB;             ///<  Callback function of BT event
    BT_RFCOMM_RX_CB *rfcommRxCB;          ///<  Callback function of BT rfcomm receive data
    BT_BLE_RCVDATA_CB *bleRcvDataCB;       ///<  Callback function of BT BLE receive url command
    UINT8           *BDAddress;           ///<  BD address 6 bytes
} BT_OPEN;

/**
     Install flag and semaphore id.

*/
extern void BT_InstallID(void) _SECTION(".kercfg_text");


extern ER BT_Open(BT_OPEN *pOpen);

extern ER BT_Close(void);


/**

    @param[in]  handle              : The connection handle returned by
    @param[in]  p_data              : Data to write
    @param[in]  max_len             : Byte count to write
    @param[out] p_len               : Bytes written
*/
extern INT32 BT_Rfcomm_WriteData(UINT16 port_handle, void *p_data, UINT16 max_len, UINT16 *p_len);



extern INT32 BT_BLE_SendNotifyData(UINT8 cmdID, UINT8* data, int length);

#endif
