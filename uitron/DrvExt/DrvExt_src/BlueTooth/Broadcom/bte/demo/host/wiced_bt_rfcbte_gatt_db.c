/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * Proximity Reporter Sample Application (GATT Server database)
 *
 */

#include "wiced_bt_gatt.h"
#include "gattdefs.h"
#include "sdpdefs.h"
#include "wiced_bt_cfg.h"
#include "bt_le_reporter.h"
#include "wiced_bt_rfcbte_gatt_db.h"

/* GATT database */
const uint8_t gatt_db[] =
{
    // Generic Attribute service
    PRIMARY_SERVICE_UUID16 (HDLS_GENERIC_ATTRIBUTE, UUID_SERVCLASS_GATT_SERVER),

    CHARACTERISTIC_UUID16 (HDLC_GENERIC_ATTRIBUTE_SERVICE_CHANGED,
                           HDLC_GENERIC_ATTRIBUTE_SERVICE_CHANGED_VALUE,
                           GATT_UUID_GATT_SRV_CHGD,
                           LEGATTDB_CHAR_PROP_NOTIFY,
                           LEGATTDB_PERM_NONE),

    // Generic Access service
    PRIMARY_SERVICE_UUID16 (HDLS_GENERIC_ACCESS, UUID_SERVCLASS_GAP_SERVER),

    CHARACTERISTIC_UUID16 (HDLC_GENERIC_ACCESS_DEVICE_NAME,
                               HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE,
                               GATT_UUID_GAP_DEVICE_NAME,
                               LEGATTDB_CHAR_PROP_READ,
                               LEGATTDB_PERM_READABLE),

    CHARACTERISTIC_UUID16 (HDLC_GENERIC_ACCESS_APPEARANCE,
                           HDLC_GENERIC_ACCESS_APPEARANCE_VALUE,
                           GATT_UUID_GAP_ICON,
                           LEGATTDB_CHAR_PROP_READ,
                           LEGATTDB_PERM_READABLE),

#ifdef ENABLE_PROXIMITY
    // Link Loss service
    PRIMARY_SERVICE_UUID16 (HDLS_LINK_LOSS, UUID_SERVCLASS_LINKLOSS),

    CHARACTERISTIC_UUID16_WRITABLE (HDLC_LINK_LOSS_ALERT_LEVEL,
                                    HDLC_LINK_LOSS_ALERT_LEVEL_VALUE,
                                    GATT_UUID_ALERT_LEVEL,
                                    LEGATTDB_CHAR_PROP_READ | LEGATTDB_CHAR_PROP_WRITE,
                                    LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ),

    // Immediate alert service
    PRIMARY_SERVICE_UUID16 (HDLS_IMMEDIATE_ALERT, UUID_SERVCLASS_IMMEDIATE_ALERT),

    CHARACTERISTIC_UUID16_WRITABLE (HDLC_IMMEDIATE_ALERT_LEVEL,
                                    HDLC_IMMEDIATE_ALERT_LEVEL_VALUE,
                                    GATT_UUID_ALERT_LEVEL,
                                    LEGATTDB_CHAR_PROP_WRITE_NO_RESPONSE,
                                    LEGATTDB_PERM_WRITE_CMD),

    // TX Power service
    PRIMARY_SERVICE_UUID16 (HDLS_TX_POWER, UUID_SERVCLASS_TX_POWER),

    CHARACTERISTIC_UUID16 (HDLC_TX_POWER_LEVEL,
                           HDLC_TX_POWER_LEVEL_VALUE,
                           GATT_UUID_TX_POWER_LEVEL,
                           LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE),

#endif

    // command service
    PRIMARY_SERVICE_UUID16(HDLS_CMD, UUID_SERVCLASS_COMMAND),
    CHARACTERISTIC_UUID16_WRITABLE(HDLC_CMD,
                           HDLC_CMD_VALUE,
                           UUID_SERVCLASS_COMMAND_CHAR,
                           LEGATTDB_CHAR_PROP_WRITE_NO_RESPONSE,
                           LEGATTDB_PERM_WRITE_CMD),

   CHARACTERISTIC_UUID16(HDLC_NTF,
                           HDLC_NTF_VALUE,
                           UUID_SERVCLASS_NOTIFY_CHAR,
    LEGATTDB_CHAR_PROP_NOTIFY, //LEGATTDB_CHAR_PROP_NOTIFY | LEGATTDB_CHAR_PROP_READ,
                           LEGATTDB_PERM_READABLE),
    CHAR_DESCRIPTOR_UUID16_WRITABLE(HDLC_NTF_DISC, UUID_SERVCLASS_NOTIFY_DESC, LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ),
};

const uint16_t gatt_db_size = sizeof(gatt_db);