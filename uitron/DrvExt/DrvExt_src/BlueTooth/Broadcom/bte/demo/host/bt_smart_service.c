
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "bt_smart_service.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_rfcbte_gatt_db.h"
#include "BT_BrcmInt.h"

#define THIS_DBGLVL         1       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          BT_SmartService
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define debug_trace DBG_IND


//static uint8_t ntf[20] = { 0x00, 0x98, 0x05, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab };

static rcv_state_t recv_state = RCV_IDLE;
static UINT8 recv_cache[MAX_CMD_LENGTH];
static UINT8 * recv_cache_tail = recv_cache;// point to the first valid unused unit
static UINT32  package_len = 0;
//const start_package_header_p pStart_package_header = (start_package_header_p)recv_cache;
static UINT8 expected_sn = 0;

extern uint16_t  ble_conn_id;     /* attribute handle */

#if 1

#pragma pack(1)

#define MAX_SEND_BUFF_COUNT 32


typedef struct{
    UINT32      uiInIdx;
    UINT32      uiOutIdx;
    UINT8       data[MAX_SEND_BUFF_COUNT][CHAR_VALUE_LENGTH];
}send_buffer_t, *send_buffer_p;

#pragma pack()

static send_buffer_t send_cache={0};
#endif


static void dump_bytes_array(UINT8* data, int len)
{
    UINT16 i, j, offset = 0;
    /* Display incoming data */
    while (len > 0)
    {
        DBG_MSG("   %04X: ", offset);

        j = 16;
        if (len < 16)
            j = len;

        for (i=0; i<j; i++)
        {
            DBG_MSG("%02X ", ((UINT8 *)data)[offset+i]);
        }

        DBG_MSG("\r\n");
        offset += j;
        len -= j;
    }
}

#if 0
static const char* get_recv_state_disp(rcv_state_t state)
{
    switch (state)
    {
    case RCV_IDLE : return "RCV_IDLE";
    case RCV_START: return "RCV_START";
    default:
        return "UNKNOW";
    }
}
#endif

static int append_data(UINT8 *data, int length)
{
    memcpy(recv_cache_tail, data, length);
    recv_cache_tail += length;
    return 0;
}

static void enter_state(rcv_state_t state)
{
    //DBG_IND("recv state %s -> %s\r\n", get_recv_state_disp(recv_state), get_recv_state_disp(state));
    recv_state = state;
}

static void clear_cache(void)
{
    recv_cache_tail = recv_cache;
    expected_sn = 0;
    enter_state(RCV_IDLE);
}

static void notify_cmd_result(UINT8 * data, int length)
{
    BT_BLE_RCVDATA_CB  *bleRcvDataCB = BT_GetBleRcvData_CB() ;

    if (bleRcvDataCB)
        bleRcvDataCB(data, length);
}

// after receiving a complete command packate, its time to parse and take action
static int handle_command(UINT8 * data, int length)
{
    DBG_IND("get one commond\r\n");
    dump_bytes_array(data, length);
    notify_cmd_result(data, length);
    // last thing after handle command
    clear_cache();
    return 0;
}

static BOOL isRecvCompleted(void)
{
    DBG_IND("tail - start = %d, length=%d\r\n",recv_cache_tail - recv_cache, package_len);
    return ( (UINT32)(recv_cache_tail - recv_cache) >= package_len);
}

static int idle_handle_incoming_data(UINT8 * data, int length)
{
    if (data[0] != 0)
    {
        DBG_ERR("sn = 0x%02x\r\n", data[0]);
        return -1;
    }
    if (length < (int)sizeof(start_package_header_t))
    {
        DBG_ERR("sn = 0x%02x, length 0x%02x\r\n", data[0], length);
        return -1;
    }
    DBG_IND("sn=%d, type = 0x%x, length=%d\r\n",data[0],data[1],data[2]);
    // keep data length
    package_len = data[2];
    // skip header
    append_data(data+sizeof(start_package_header_t), length-sizeof(start_package_header_t));
    enter_state(RCV_START);
    return 0;
}

static int continue_handle_incoming_data(UINT8 * data, int length)
{
    if (expected_sn != data[0])
    {
        DBG_ERR("sn = 0x%02x, expected_sn 0x%02x\r\n", data[0], expected_sn);
        clear_cache();
        enter_state(RCV_IDLE);
        return -1;
    }
    if (length < (int)sizeof(start_package_header_t))
    {
        DBG_ERR("sn = 0x%02x, length 0x%02x\r\n", data[0], length);
        return -1;
    }
    DBG_IND("sn=%d\r\n",data[0]);
    // skip SN
    append_data(data+1, length-1);
    return 0;
}

static incoming_data_handle_t incoming_data_handler[RCV_INVALID] = {
    idle_handle_incoming_data,
    continue_handle_incoming_data,
};

int handle_incoming_data(UINT8 * data, int length)
{
    incoming_data_handler[recv_state](data, length);
    expected_sn++;

    if (isRecvCompleted())
    {
        handle_command(recv_cache, package_len);
    }
    return 0;
}

#if 1

static UINT32 bt_BleSndbufIsEmpty(send_buffer_t  *sndBuf)
{
    if (sndBuf->uiInIdx == sndBuf->uiOutIdx)
        return 1;
    else
        return 0;
}

static UINT32 bt_BleSndbufIsFull(send_buffer_t  *sndBuf)
{
    if (sndBuf->uiInIdx+1 == sndBuf->uiOutIdx)
        return 1;
    else
        return 0;
}

static void bt_BleSndbufEnQueue(send_buffer_t  *sndBuf, UINT8 *data)
{
    UINT32 InNextIdx;

    InNextIdx = sndBuf->uiInIdx+1;
    if (InNextIdx >= MAX_SEND_BUFF_COUNT)
    {
        InNextIdx -= MAX_SEND_BUFF_COUNT;
    }
    memcpy(&sndBuf->data[sndBuf->uiInIdx],data,CHAR_VALUE_LENGTH);
    sndBuf->uiInIdx = InNextIdx;
}

static void bt_BleSndbufDeQueue(send_buffer_t  *sndBuf, UINT8 *data)
{
    memcpy(data, &sndBuf->data[sndBuf->uiOutIdx],CHAR_VALUE_LENGTH);
    sndBuf->uiOutIdx++;
    if (sndBuf->uiOutIdx >= MAX_SEND_BUFF_COUNT)
    {
        sndBuf->uiOutIdx -= MAX_SEND_BUFF_COUNT;
    }
}

static int bt_BleGetHeaderLen(int sn)
{
    if (sn == 0)
        return sizeof(start_package_header_t);
    else
        return 1;
}

// device may send any length data to APP
// just put in cache
int bt_BleSndNotify_data(UINT8 cmdID, UINT8* data, int length)
{
    UINT8 * pInData = data;
    UINT8   tempData[CHAR_VALUE_LENGTH];
    send_buffer_p  p_send_cache = &send_cache;
    UINT8   *pBuf = tempData;
    int remains = length;
    int sn = 0,headerlen;

    if (data == NULL || length == 0)
    {
        return 0;
    }
    // package data
    while (remains > 0)
    {
        if (bt_BleSndbufIsFull(p_send_cache))
        {
            DBG_ERR("no more buffer\n");
            return -1;
        }
        else
        {
            // clean the buffer
            memset(pBuf,0x00,CHAR_VALUE_LENGTH);
            headerlen = bt_BleGetHeaderLen(sn);
            pBuf[0]=sn;
            DBG_IND("pInData=0x%x, remains=%d\r\n",pInData,remains);
            if (sn == 0)
            {
                pBuf[1]=cmdID;
                pBuf[2]=length;
            }
            if (remains > CHAR_VALUE_LENGTH-headerlen)
            {
                memcpy(&pBuf[headerlen], pInData,CHAR_VALUE_LENGTH-headerlen);
                remains -=(CHAR_VALUE_LENGTH-headerlen);
                pInData+=(CHAR_VALUE_LENGTH-headerlen);
            }
            else
            {
                memcpy(&pBuf[headerlen], pInData,remains);
                pInData+=remains;
                remains = 0;
            }
            sn++;
            bt_BleSndbufEnQueue(p_send_cache, pBuf);
        }
    }
    // send data
    DBG_IND("cmdId=0x%x, length = %d\r\n",cmdID,length);
    while (!bt_BleSndbufIsEmpty(p_send_cache))
    {
        bt_BleSndbufDeQueue(p_send_cache, pBuf);
        dump_bytes_array(pBuf, CHAR_VALUE_LENGTH);
        wiced_bt_gatt_send_notification(ble_conn_id, HDLC_NTF_VALUE, CHAR_VALUE_LENGTH, pBuf);

    }
    return 0;
}
#endif

