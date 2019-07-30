/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * Proximity Monitor Sample Application
 *
 */
#include "wiced_bt_dev.h"
#include "wiced_bt_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Constants
 ****************************************************************************/
#define BT_RFCOMM_SERVER_APP_SCN        1   /* Service Class Number */
#define BT_RFCOMM_SERVER_APP_MTU        500
/*****************************************************************************
 * Globals
 *****************************************************************************/
/* Stack and buffer pool configuration tables */
extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;
extern const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[];

extern const uint8_t wiced_bt_sdp_db[];
extern const UINT16 wiced_bt_sdp_db_size;

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif
