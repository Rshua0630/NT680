/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * RFCOMM Server Sample Application
 *
 * Features demonstrated
 *  - WICED BT RFCOMM APIs
 *
 * On startup this demo:
 *  - Initializes the Bluetooth sub system
 *  - Initialize RFCOMM server (using PSM=1), and wait for a connection
 *  - On receiving data over RFCOMM, echo back to remote device
 *
 * Application Instructions
 *   Connect a PC terminal to the serial port of the WICED Eval board,
 *   then build and download the application as described in the WICED
 *   Quick Start Guide
 *
 */

#include <string.h>
#include <stdio.h>
#include "wiced.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_sdp.h"
#include "wiced_bt_rfcomm.h"
#include "wiced_bt_cfg.h"
#include "bt_rfc_server.h"
#include "BT_BrcmInt.h"

#if 0 //#ifdef SPP_ENABLE
/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
/* Mask of RFCOMM events handled by app callback */
#define BT_RFCOMM_SERVER_EVT_MASK   ((wiced_bt_rfcomm_port_event_t)(PORT_EV_FC | PORT_EV_FCS | PORT_EV_RXCHAR | \
                                            PORT_EV_TXEMPTY | PORT_EV_CTS | PORT_EV_DSR | \
                                            PORT_EV_RING | PORT_EV_CTSS | PORT_EV_DSRS))

/******************************************************
*               Function Declarations
******************************************************/
//static void bt_rfcomm_server_log_data(uint8_t *p_data, uint16_t len);
void bt_rfcomm_server_init(void);

#include "wiced_bt_dev.h"

/******************************************************
 *               Variable Definitions
 ******************************************************/
#define RFCOMM_SERVER_CONNECTION_COUNT 1
uint16_t bt_rfcomm_server_handle[RFCOMM_SERVER_CONNECTION_COUNT];

/* RFCOMM connection management callback callback */
static void bt_rfcomm_server_conn_cback(wiced_bt_rfcomm_result_t code, uint16_t port_handle)
{
    if (code == WICED_BT_RFCOMM_SUCCESS)
    {
        /* RFCOMM connection established. Send test string to remote device */
        WPRINT_BT_APP_INFO(("RFCOMM connection established.\n"));
    }
    else if (code == WICED_BT_RFCOMM_CLOSED)
    {
        WPRINT_BT_APP_INFO(("RFCOMM connection closed.\n"));
    }
    else
    {
        WPRINT_BT_APP_INFO(("%s unhandled code=0x%x\n", __FUNCTION__, code));
    }
}

/* RFCOMM port event callback */
static void bt_rfcomm_server_evt_cback(wiced_bt_rfcomm_port_event_t event, uint16_t port_handle)
{
    WPRINT_BT_APP_INFO(("bt_rfcomm_server_evt_cback event mask=0x%04X\n", (int)event));
}

/* RFCOMM Data RX callback */
static int bt_rfcomm_server_data_cback(uint16_t port_handle, void *p_data, uint16_t len)
{
    //#NT#2015/09/01#Lincy Lin -begin
    //#NT#
    #if 0
    uint16_t len_sent;
    WPRINT_BT_APP_INFO(("RFCOMM RX (len=%i)\n", len));
    bt_rfcomm_server_log_data (p_data, len);

    /* Echo back to client */
    WPRINT_BT_APP_INFO(("RFCOMM TX (len=%i)\n", len));
    bt_rfcomm_server_log_data (p_data, len);

    wiced_bt_rfcomm_write_data(port_handle, (char *)p_data, len, &len_sent);
    #else
    {
        BT_RFCOMM_RX_CB  *rfcommRxCB;
        rfcommRxCB = BT_GetRfcomm_Rx_CB();
        if (rfcommRxCB)
        {
            rfcommRxCB(port_handle, p_data, len);
        }
    }
    #endif
    //#NT#2015/09/01#Lincy Lin -end
    return 0;
}

#if 0
/* Log data */
static void bt_rfcomm_server_log_data (uint8_t *p_data, uint16_t len)
{
    uint16_t i, j, offset = 0;

    /* Display incoming data */
    while (len > 0)
    {
        WPRINT_BT_APP_INFO(("   %04X: ", offset));

        j = 16;
        if (len < 16)
            j = len;

        for (i=0; i<j; i++)
        {
            WPRINT_BT_APP_INFO(("%02X ", ((uint8_t *)p_data)[offset+i]));
        }

        WPRINT_BT_APP_INFO(("\n"));
        offset += j;
        len -= j;
    }
}
#endif

/* Initialize the RFCOMM server */
void bt_rfcomm_server_init(void)
{
    wiced_bt_device_address_t bd_addr;
    int i = 0;
    /* Initialize SDP server database for rfcble_app */
    wiced_bt_sdp_db_init((UINT8 *)wiced_bt_sdp_db, wiced_bt_sdp_db_size);
    //wiced_bt_dev_set_sspmode(0);
    for (i = 0; i < RFCOMM_SERVER_CONNECTION_COUNT; i++)
    {
        /* Create RFCOMM server connection */
        wiced_bt_rfcomm_create_connection(UUID_SERVCLASS_SERIAL_PORT,
            BT_RFCOMM_SERVER_APP_SCN,
            TRUE,
            BT_RFCOMM_SERVER_APP_MTU,
            bd_addr,
            &bt_rfcomm_server_handle[i],
            bt_rfcomm_server_conn_cback);

        /* Set data callback RFCOMM */
        wiced_bt_rfcomm_set_data_callback(bt_rfcomm_server_handle[i], bt_rfcomm_server_data_cback);

        /* Set event callback RFCOMM, and events to be notified of */
        wiced_bt_rfcomm_set_event_mask(bt_rfcomm_server_handle[i], BT_RFCOMM_SERVER_EVT_MASK);
        wiced_bt_rfcomm_set_event_callback(bt_rfcomm_server_handle[i], bt_rfcomm_server_evt_cback);
    }


    /* Enable connectability (use default connectability window/interval) */
    wiced_bt_dev_set_connectability (BTM_CONNECTABLE, 0, 0);

    /* Enable discoverability (use default discoverability window/interval) */
    wiced_bt_dev_set_discoverability (BTM_GENERAL_DISCOVERABLE, 0, 0);

    WPRINT_BT_APP_INFO(("Waiting for RFCOMM connection (scn=%i)...\r\n", BT_RFCOMM_SERVER_APP_SCN));

}
#endif