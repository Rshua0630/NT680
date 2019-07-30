/*
 * $ Copyright Broadcom Corporation $
 */
#pragma once

#ifdef NOVATEK
#include "buildcfg_novatek.h"
#elif defined(WIN32)
#include "buildcfg _win32.h"
#else

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////
// project settings quick config
//#define BT_TRACE_VERBOSE      FALSE
//#define BT_USE_TRACES         TRUE
//#define BT_TRACE_PROTOCOL     FALSE
//#define USE_BUILD_IN_HCD      TRUE
//#define BCM_CHIP_ID           CHIP_ID_4339
//#define ENABLE_CONSOLE        TRUE
//#define GKI_DEBUG             FALSE

// keep disabled when debugging via JLINK
#define ENABLE_SLEEP FALSE

/////////////////////////////////////////////////////////////////

#define BTEWICED                        TRUE

#define BTM_INCLUDED                    TRUE                # Makefile only
#define BTM_CMD_POOL_ID                 GKI_POOL_ID_1
#define BTM_CLB_INCLUDED                FALSE
#define BTM_CLB_RX_INCLUDED             FALSE
#define BTM_TBFC_INCLUDED               FALSE
#define BTM_SCO_INCLUDED                FALSE
#define BTM_INQ_DB_INCLUDED             FALSE
#define BTM_BUSY_LEVEL_CHANGE_INCLUDED  FALSE
#define BTM_ALLOW_CONN_IF_NONDISCOVER   TRUE
#define BTM_MAX_REM_BD_NAME_LEN         10
#define BTM_DUMO_ADDR_CENTRAL_ENABLED   FALSE
#define BTM_APP_DEV_INIT                bte_post_reset
#define BTM_INTERNAL_LINKKEY_STORAGE_INCLUDED TRUE
#define BTM_BLE_PRIVACY_SPT             TRUE
#define BTM_USE_CONTROLLER_PRIVATE_ADDRESS  FALSE

#define BTU_INCLUDED                    TRUE                # Makefile only
#define BTUTHIN_INCLUDED                FALSE               # Makefile only
#define BTU_BTC_SNK_INCLUDED            FALSE
#define BTU_STACK_LITE_ENABLED          FALSE
#define BTU_DYNAMIC_CB_INCLUDED         TRUE
#define BTU_MUTEX_INCLUDED              TRUE

#define L2CAP_INCLUDED                  TRUE
#define L2CAP_CMD_POOL_ID               GKI_POOL_ID_1
#define L2CAP_FCR_INCLUDED              FALSE
#define L2CAP_UCD_INCLUDED              FALSE
#define L2CAP_WAKE_PARKED_LINK          FALSE
#define L2CAP_NON_FLUSHABLE_PB_INCLUDED FALSE
#define L2CAP_ROUND_ROBIN_CHANNEL_SERVICE   FALSE
#define L2CAP_MTU_SIZE                  ((UINT16)(HCI_ACL_POOL_BUF_SIZE - BT_HDR_SIZE - 8))     /* ACL bufsize minus BT_HDR, and l2cap/hci packet headers */
#define L2CAP_LE_COC_INCLUDED           TRUE

#define RFCOMM_INCLUDED                 FALSE
#define RFCOMM_USE_EXTERNAL_SCN         FALSE
#define RFCOMM_CMD_POOL_ID              GKI_POOL_ID_1
#define RFCOMM_DATA_POOL_ID             GKI_POOL_ID_2
#define MAX_RFC_PORTS                   (p_btm_cfg_settings->rfcomm_cfg.max_ports)
#define MAX_BD_CONNECTIONS              (p_btm_cfg_settings->rfcomm_cfg.max_links)
#define PORT_RX_CRITICAL_WM             ((UINT32)(L2CAP_MTU_SIZE-L2CAP_MIN_OFFSET-RFCOMM_DATA_OVERHEAD)*PORT_RX_BUF_CRITICAL_WM)
#define PORT_RX_LOW_WM                  ((UINT32)(L2CAP_MTU_SIZE-L2CAP_MIN_OFFSET-RFCOMM_DATA_OVERHEAD)*PORT_RX_BUF_LOW_WM)
#define PORT_RX_HIGH_WM                 ((UINT32)(L2CAP_MTU_SIZE-L2CAP_MIN_OFFSET-RFCOMM_DATA_OVERHEAD)*PORT_RX_BUF_HIGH_WM)
#define PORT_RX_BUF_LOW_WM              8
#define PORT_RX_BUF_HIGH_WM             16
#define PORT_RX_BUF_CRITICAL_WM         20
#define PORT_TX_HIGH_WM                 ((UINT32)(L2CAP_MTU_SIZE-L2CAP_MIN_OFFSET-RFCOMM_DATA_OVERHEAD)*PORT_TX_BUF_HIGH_WM)
#define PORT_TX_CRITICAL_WM             ((UINT32)(L2CAP_MTU_SIZE-L2CAP_MIN_OFFSET-RFCOMM_DATA_OVERHEAD)*PORT_TX_BUF_CRITICAL_WM)
#define PORT_TX_BUF_HIGH_WM             3
#define PORT_TX_BUF_CRITICAL_WM         5
#define PORT_CREDIT_RX_LOW              2
#define PORT_CREDIT_RX_MAX              3

/* HID definitions */
#define HID_DEV_INCLUDED                FALSE
#define HID_DEV_MAX_DESCRIPTOR_SIZE     200
#define HID_DEV_SET_CONN_MODE           FALSE

/* AVDT/A2DP/AVRC definitions */
#define A2D_INCLUDED                    FALSE
#define A2D_SBC_INCLUDED                FALSE
#define A2D_M12_INCLUDED                FALSE
#define A2D_M24_INCLUDED                FALSE
#define AVDT_INCLUDED                   FALSE
#define AVDT_REPORTING                  FALSE
#define AVDT_MULTIPLEXING               FALSE
#define AVDT_NUM_LINKS                  (p_btm_cfg_settings->avdt_cfg.max_links)
#define AVDT_CMD_POOL_ID                GKI_POOL_ID_1
#define AVDT_DATA_POOL_ID               GKI_POOL_ID_3
#define AVDT_DATA_POOL_SIZE             GKI_BUF3_SIZE

#define AVRC_INCLUDED                   FALSE
#define AVCT_INCLUDED                   FALSE
#define AVCT_NUM_LINKS                  (p_btm_cfg_settings->avrc_cfg.max_links)
#define AVCT_NUM_CONN                   (avct_cb.num_conn)
#define AVRC_SEC_MASK                   (p_btm_cfg_settings->security_requirement_mask)
#define AVRC_CONTROL_MTU                (L2CAP_MTU_SIZE)
#define AVRC_BROWSE_MTU                 (L2CAP_MTU_SIZE)

#define GATT_FIXED_DB                   TRUE
#define GATTS_APPU_USE_GATT_TRACE       TRUE
#define GATT_MAX_APPS                   3
#define GATT_MAX_SR_PROFILES            3
#define GATT_MAX_PHY_CHANNEL            (GATT_CL_MAX_LCB + GATT_MAX_SCCB)
#define GATT_MAX_ATTR_LEN               (p_btm_cfg_settings->gatt_cfg.max_attr_len)
#define GATT_MAX_MTU_SIZE               517
#define GATT_CL_MAX_LCB                 (p_btm_cfg_settings->gatt_cfg.client_max_links)
#define GATT_MAX_SCCB                   (p_btm_cfg_settings->gatt_cfg.server_max_links)
#define GATTP_TRANSPORT_SUPPORTED       GATT_TRANSPORT_LE_BR_EDR
#define GATTC_NOTIF_TIMEOUT             3

#define SIM_ACCESS_INCLUDED             FALSE
#define SAP_SERVER_INCLUDED             FALSE
#define SAP_CLIENT_INCLUDED             FALSE
#define BLE_INCLUDED                    TRUE
#define SMP_INCLUDED                    TRUE
#define GAP_INCLUDED                    FALSE
#define SMP_HOST_ENCRYPT_INCLUDED       TRUE
#define SMP_LE_SC_INCLUDED              FALSE
#define SMP_LE_SC_OOB_INCLUDED          FALSE
#define ATT_DEBUG                       FALSE
#define SMP_DEBUG                       FALSE
#define BLE_BRCM_INCLUDED               TRUE
#define BLE_DATA_LEN_EXT_INCLUDED       TRUE

#define AMP_INCLUDED                    FALSE
#define GPS_INCLUDED                    FALSE

#define GKI_NUM_FIXED_BUF_POOLS         WICED_BT_CFG_NUM_BUF_POOLS
#define GKI_NUM_TOTAL_BUF_POOLS         WICED_BT_CFG_NUM_BUF_POOLS
#define GKI_BUF0_SIZE                   (p_btm_cfg_buf_pools[GKI_POOL_ID_0].buf_size)
#define GKI_BUF0_MAX                    (p_btm_cfg_buf_pools[GKI_POOL_ID_0].buf_count)
#define GKI_BUF1_SIZE                   (p_btm_cfg_buf_pools[GKI_POOL_ID_1].buf_size)
#define GKI_BUF1_MAX                    (p_btm_cfg_buf_pools[GKI_POOL_ID_1].buf_count)
#define GKI_BUF2_SIZE                   (p_btm_cfg_buf_pools[GKI_POOL_ID_2].buf_size)
#define GKI_BUF2_MAX                    (p_btm_cfg_buf_pools[GKI_POOL_ID_2].buf_count)
#define GKI_BUF3_SIZE                   (p_btm_cfg_buf_pools[GKI_POOL_ID_3].buf_size)
#define GKI_BUF3_MAX                    (p_btm_cfg_buf_pools[GKI_POOL_ID_3].buf_count)
#define GKI_DYNAMIC_POOL_CFG            TRUE
#define GKI_DYNAMIC_MEMORY              FALSE
#define GKI_USE_DYNAMIC_BUFFERS         TRUE



#define HCIC_INCLUDED                   TRUE
#define HCI_CMD_POOL_ID                 GKI_POOL_ID_1
#define HCI_CMD_POOL_BUF_SIZE           (p_btm_cfg_buf_pools[HCI_CMD_POOL_ID].buf_size)
#define HCI_ACL_POOL_ID                 GKI_POOL_ID_2
#define HCI_ACL_POOL_BUF_SIZE           (p_btm_cfg_buf_pools[HCI_ACL_POOL_ID].buf_size)
#define HCI_SCO_POOL_ID                 GKI_POOL_ID_2
#define HCI_USE_VARIABLE_SIZE_CMD_BUF   TRUE

#define HCISU_H4_INCLUDED               TRUE

#ifdef WIN32
#define HCILP_INCLUDED                  FALSE
#else
#define HCILP_INCLUDED                  TRUE
#define HCILP_BT_WAKE_POLARITY          1
#define HCILP_HOST_WAKE_POLARITY        1
#define HCILP_BT_WAKE_IDLE_TIMEOUT      1000
#endif

#define H4IBSS_INCLUDED                 FALSE
#define H4IBSS_DEBUG                    FALSE

#define SLIP_INCLUDED                   FALSE
#define SLIP_STATIS_INCLUDED            FALSE
#define SLIP_SW_FLOW_CTRL               TRUE
#define BT_TRACE_SLIP                   FALSE
#define SLIP_SLEEP_TO                   5000
#define SLIP_HOST_SLIDING_WINDOW_SIZE   7

#define BTM_INQ_DB_SIZE                 1
#define BTM_SEC_MAX_DEVICE_RECORDS      (p_btm_cfg_settings->max_simultaneous_links)
#if !defined(BTM_USE_CONTROLLER_PRIVATE_ADDRESS) || (BTM_USE_CONTROLLER_PRIVATE_ADDRESS == FALSE)
#define BTM_SEC_HOST_PRIVACY_ADDR_RESOLUTION_TABLE_SIZE    3
#endif
#define BTM_SEC_MAX_SERVICE_RECORDS     4
#define BTM_SEC_SERVICE_NAME_LEN        0
#define BTM_MAX_LOC_BD_NAME_LEN         0
#define BTM_MAX_PM_RECORDS              1
#define BTM_MAX_VSE_CALLBACKS           1
#define BTM_BLE_MAX_BG_CONN_DEV_NUM     2
#define BTM_OOB_INCLUDED                FALSE
#define BTM_BR_SC_INCLUDED              FALSE
#define BTM_CROSS_TRANSP_KEY_DERIVATION FALSE
#define BTM_PWR_MGR_INCLUDED            FALSE
#define BT_BRCM_VS_INCLUDED             TRUE

#define BTTRC_INCLUDED                  FALSE
#define BTTRC_PARSER_INCLUDED           FALSE
#define MAX_TRACE_RAM_SIZE              10

#define SDP_CLIENT_ENABLED              FALSE
#define SDP_POOL_ID                     GKI_POOL_ID_2
#define SDP_MAX_CONNECTIONS             1
#define SDP_MAX_RECORDS                 3
#define SDP_MAX_REC_ATTR                8
#define SDP_MAX_UUID_FILTERS            3
#define SDP_MAX_ATTR_FILTERS            12
#define SDP_MAX_PROTOCOL_PARAMS         2
#define SDP_RAW_DATA_SERVER             FALSE

#if (defined(BTU_DYNAMIC_CB_INCLUDED)  && (BTU_DYNAMIC_CB_INCLUDED == TRUE))
#define MAX_L2CAP_CLIENTS               (btu_cb.l2c_cfg_max_clients)
#define MAX_L2CAP_LINKS                 (btu_cb.l2c_cfg_max_links)
#define MAX_L2CAP_CHANNELS              (btu_cb.l2c_cfg_max_channels)

/* Connection Oriented Channel configuration */
#define MAX_L2CAP_BLE_CLIENTS           (p_btm_cfg_settings->l2cap_application.max_le_psm)
#define MAX_L2CAP_BLE_CHANNELS          (p_btm_cfg_settings->l2cap_application.max_le_channels)

#else /* BTU_DYNAMIC_CB_INCLUDED  */
#define MAX_L2CAP_CLIENTS               3
#define MAX_L2CAP_LINKS                 1
#define MAX_L2CAP_CHANNELS              4
#endif /* BTU_DYNAMIC_CB_INCLUDED */

#define GAP_CONN_INCLUDED               FALSE

#define HCISU_TASK                      0
#define BTU_TASK                        1
#define BTE_APPL_TASK                   2
#define TICKS_TASK                      3

#define GKI_MAX_TASKS                   4

#define BTE_IDLE_TASK_INCLUDED          FALSE

#define BTM_PRE_LISBON_INCLUDED         FALSE
#define USE_BUILD_IN_HCD                TRUE
#ifndef USE_BUILD_IN_HCD
#define USE_BUILD_IN_HCD                FALSE
#endif

// default enable debug console
#ifndef ENABLE_CONSOLE
#define ENABLE_CONSOLE                  TRUE
#endif

#ifdef WIN32
#ifdef _MAX_PATH
#undef _MAX_PATH
#endif
#endif
/* Miscellaneous application configuration */
#define _MAX_PATH                       16
#define TICKS_PER_SEC                   1000
#define USERIAL_HCI_PORT                USERIAL_PORT_1
#define GKI_SHUTDOWN_EVT                APPL_EVT_7
#define THREAD_EVT_QUEUE_MSG_SIZE       4
#define THREAD_EVT_QUEUE_NUM_MSG        10

#ifndef BT_TRACE_PROTOCOL
#define BT_TRACE_PROTOCOL FALSE
#endif

#ifndef BT_USE_TRACES
#define BT_USE_TRACES   FALSE
#endif

#define CHIP_ID_43438 1
#define CHIP_ID_43341 2
#define CHIP_ID_4354  3
#define CHIP_ID_20710  4
#define CHIP_ID_4339 5  // Nexus 5 patchram

#ifndef WIN32
#define GPIO_BT_RST         6//8 // 1
#define GPIO_BT_WAKE        7 // 0
#define GPIO_HOST_WAKE      41 //21 //8 // 1
#define GPIO_BT_REG         42 // 21
#endif

// default chip id unless specified in compiler project settings
#ifdef WIN32
#ifndef BCM_CHIP_ID
#define BCM_CHIP_ID CHIP_ID_43438
#endif
#else
#ifndef BCM_CHIP_ID
#define BCM_CHIP_ID CHIP_ID_43438
#endif
#endif

//#define  GKI_DEBUG  TRUE

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif
