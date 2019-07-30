/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * BLE Proximity Reporter Sample Application
 *
 * Features demonstrated
 *  - WICED BT GATT server APIs
 *
 * On startup this demo:
 *  - Initializes the GATT subsystem
 *  - Begins advertising
 *  - Waits for GATT clients to connect
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
#include "bt_target.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced_bt_rfcbte_gatt_db.h"
#include "bt_le_reporter.h"
#include "wiced_bt_l2c.h"
#include "bt_smart_service.h"

/******************************************************
 *               Variable Definitions
 ******************************************************/

/* Proximity reporter attribute values */
uint8_t     proximity_immediate_alert_level;
uint8_t     proximity_link_loss_alert_level;
int8_t      proximity_tx_power_level;
uint16_t    ble_conn_id;

/* GATT attrIbute values */
uint32_t    proximity_gatt_attribute_service_changed = 0;
uint16_t    proximity_gatt_generic_access_appearance = 0;
//static uint8_t ntf[20] = { 0x10, 0xaa, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab };
static uint8_t ntf[20] = { 0x00, 0x91, 0x04, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab };
static uint8_t ntf_desc[20] = {0, 0};
wiced_bt_gatt_status_t bt_le_proximity_gatt_write_request(wiced_bt_gatt_write_t *p_write_request);
wiced_bt_gatt_status_t bt_le_proximity_gatt_read_request(wiced_bt_gatt_read_t *p_read_request);
wiced_bt_gatt_status_t bt_le_gatt_server_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
wiced_bt_gatt_status_t bt_le_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
void bt_le_proximity_reporter_init(void);

/* TX Power report handler */
#ifdef ENABLE_PROXIMITY
static void bt_ble_proximity_tx_power_cback(wiced_bt_tx_power_result_t *p_tx_power)
{
    if ((p_tx_power->status == WICED_BT_SUCCESS) && (p_tx_power->hci_status == HCI_SUCCESS))
    {
        WPRINT_BT_APP_INFO(("Local TX power: %i\n", p_tx_power->tx_power));
        proximity_tx_power_level = p_tx_power->tx_power;
    }
    else
    {
        WPRINT_BT_APP_INFO(("Unable to read Local TX power. (btm_status=0x%x, hci_status=0x%x)\n", p_tx_power->status, p_tx_power->hci_status));
        proximity_tx_power_level = 0;
    }
}
#endif

/* Handler for attrubute write requests */
wiced_bt_gatt_status_t bt_le_proximity_gatt_write_request(wiced_bt_gatt_write_t *p_write_request)
{
#ifdef ENABLE_PROXIMITY
    uint8_t attribute_value = *(uint8_t *)p_write_request->p_val;
#endif
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;


    switch (p_write_request->handle)
    {
#ifdef ENABLE_PROXIMITY
    case HDLC_LINK_LOSS_ALERT_LEVEL_VALUE:
        proximity_link_loss_alert_level = attribute_value;
        WPRINT_BT_APP_INFO(("Link loss alert level changed to: %i\r\n", attribute_value));
        break;

    case HDLC_IMMEDIATE_ALERT_LEVEL_VALUE:
        proximity_immediate_alert_level = attribute_value;
        WPRINT_BT_APP_INFO(("Proximity alert (level: %i)\r\n", attribute_value));
        break;
#endif
    case HDLC_CMD_VALUE:
        handle_incoming_data(p_write_request->p_val, p_write_request->val_len);
        break;

    case HDLC_NTF_DISC:
        WPRINT_BT_APP_INFO(("***************** HDLC_NTF_DISC write (%d) ***************\r\n", p_write_request->val_len));
        memcpy(ntf_desc, p_write_request->p_val, p_write_request->val_len);
        break;
    default:
        WPRINT_BT_APP_INFO(("Write request to invalid handle: 0x%x\n", p_write_request->handle));
        status = WICED_BT_GATT_WRITE_NOT_PERMIT;
        break;
    }

    return (status);
}

/* Handler for attrubute read requests */
wiced_bt_gatt_status_t bt_le_proximity_gatt_read_request(wiced_bt_gatt_read_t *p_read_request)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;
    void *p_attribute_value_source;
    uint16_t attribute_value_length = 0;

    switch (p_read_request->handle)
    {
    case HDLC_GENERIC_ATTRIBUTE_SERVICE_CHANGED_VALUE:
        p_attribute_value_source    = &proximity_gatt_attribute_service_changed;
        attribute_value_length      = sizeof(proximity_gatt_attribute_service_changed);
        break;

    case HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE:
        p_attribute_value_source    = (void *)wiced_bt_cfg_settings.device_name;
        attribute_value_length      = strlen((char *)wiced_bt_cfg_settings.device_name);
        break;

    case HDLC_GENERIC_ACCESS_APPEARANCE_VALUE:
        p_attribute_value_source    = &proximity_gatt_generic_access_appearance;
        attribute_value_length      = sizeof(proximity_gatt_generic_access_appearance);
        break;

#ifdef ENABLE_PROXIMITY
    case HDLC_TX_POWER_LEVEL_VALUE:
        p_attribute_value_source    = &proximity_tx_power_level;
        attribute_value_length      = sizeof(proximity_tx_power_level);
        break;

    case HDLC_LINK_LOSS_ALERT_LEVEL_VALUE:
        p_attribute_value_source    = &proximity_link_loss_alert_level;
        attribute_value_length      = sizeof(proximity_link_loss_alert_level);
        break;
#endif

    case HDLC_NTF_VALUE:
        p_attribute_value_source = ntf;
        attribute_value_length = sizeof(ntf);
        WPRINT_BT_APP_INFO(("***************** HDLC_NTF_VALUE read ***************\r\n"));
        break;
    case HDLC_NTF_DISC:
        p_attribute_value_source = ntf_desc;
        attribute_value_length = sizeof(ntf_desc);
        WPRINT_BT_APP_INFO(("***************** HDLC_NTF_DISC read ***************\r\n"));
        break;
    default:
        status = WICED_BT_GATT_READ_NOT_PERMIT;
        WPRINT_BT_APP_INFO(("Read request to invalid handle: 0x%x\r\n", p_read_request->handle));
        break;
    }

    /* Validate destination buffer size */
    if (attribute_value_length > *p_read_request->p_val_len)
    {
        *p_read_request->p_val_len = attribute_value_length;
    }

    /* Copy the attribute value */
    if (attribute_value_length)
    {
        memcpy(p_read_request->p_val, p_attribute_value_source, attribute_value_length);
    }

    /* Indicate number of bytes copied */
    *p_read_request->p_val_len = attribute_value_length;

    return (status);
}



/* GATT event handler */
wiced_bt_gatt_status_t bt_le_gatt_server_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;
    uint8_t *bda;

    switch (event)
{
    case GATT_CONNECTION_STATUS_EVT:
        /* GATT connection status change */
        bda = p_event_data->connection_status.bd_addr;
        WPRINT_BT_APP_INFO(("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] %s.\r\n",  bda[0], bda[1], bda[2], bda[3], bda[4], bda[5],
            (p_event_data->connection_status.connected ? "established" : "released")));
        if (p_event_data->connection_status.connected)
        {
#ifdef ENABLE_PROXIMITY
            /* Connection established. Get current TX power  (required for setting TX power attribute in GATT database) */
                wiced_bt_dev_read_tx_power (p_event_data->connection_status.bd_addr,
                                            p_event_data->connection_status.transport,
                                            (wiced_bt_dev_cmpl_cback_t *)bt_ble_proximity_tx_power_cback);
#endif
            ble_conn_id = p_event_data->connection_status.conn_id;
            /* Disable connectability. */
            wiced_bt_start_advertisements (BTM_BLE_ADVERT_OFF, 0, NULL);

            wiced_bt_l2cap_update_ble_conn_params(p_event_data->connection_status.bd_addr, 80, 400, 0, 700);
        }
        else
        {
            /* Connection released. Re-enable BLE connectability. */
            wiced_bt_start_advertisements (BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
            /* test code for directed adv */
            /* wiced_bt_start_advertisements (BTM_BLE_ADVERT_DIRECTED_HIGH, 0, p_event_data->connection_status.bd_addr); */
            memset(ntf_desc, 0, sizeof(ntf_desc));
            WPRINT_BT_APP_INFO(("Waiting for proximity monitor to connect...\r\n"));
        }
        break;

    default:
        break;
    }

    return (status);
}

extern char device_name[20];
/* Initialize Proximity Reporter */
void bt_le_proximity_reporter_init(void)
{
    wiced_bt_ble_advert_data_t adv_data;
    /* Set advertising data: device name and discoverable flag */
    adv_data.flag = 0x5 ; //0x06;
    adv_data.tx_power = 4;

    strncpy(device_name, "NvtBT", sizeof(device_name));
    device_name[sizeof(device_name)-1]=0;
    wiced_bt_ble_set_advertisement_data(BTM_BLE_ADVERT_BIT_FLAGS | BTM_BLE_ADVERT_BIT_DEV_NAME , &adv_data);

    /* Enable privacy */
    //wiced_bt_ble_enable_privacy (TRUE);

    /* Register for gatt event notifications */
    //wiced_bt_gatt_register(&bt_le_proximity_gatt_cback);

    /* Initialize GATT database */
    wiced_bt_gatt_db_init ((uint8_t *)gatt_db, gatt_db_size);

    /* Enable Bluetooth LE advertising and connectability */

    /* start LE advertising */
    wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
    //WPRINT_BT_APP_INFO(("Waiting for proximity monitor to connect...\r\n"));
}