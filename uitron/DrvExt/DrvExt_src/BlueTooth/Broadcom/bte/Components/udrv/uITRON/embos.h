#pragma once

#if 1

#include "data_types.h"
#include "gki_common.h"
#include "wiced_result.h"
#include "bt_types.h"

#define UNUSED_PARAMETER(x) (void) x
/*
typedef enum
{
    WICED_FALSE = 0,
    WICED_TRUE = 1
} wiced_bool_t;
*/

#define WICED_FALSE 0
#define WICED_TRUE  1

#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        long
#define portSHORT       short
#define portSTACK_TYPE  unsigned portLONG
#define portBASE_TYPE   long

typedef void (*timer_handler_t)( void* arg );


#define RESULT_ENUM( prefix, name, value )  prefix ## name = (value)

/**
* DCT section
*/
typedef enum
{
    DCT_APP_SECTION,
    DCT_SECURITY_SECTION,
    DCT_MFG_INFO_SECTION,
    DCT_WIFI_CONFIG_SECTION,
    DCT_INTERNAL_SECTION, /* Do not use in apps */
} dct_section_t;

#define GKI_TIMER_INTERVAL      (100)
#define WICED_NEVER_TIMEOUT   (0xFFFFFFFF)
#define WICED_WAIT_FOREVER    (0xFFFFFFFF)
#define WICED_NO_WAIT         (0)

typedef struct
{
    char year[4];        /**< Year         */
    char dash1;          /**< Dash1        */
    char month[2];       /**< Month        */
    char dash2;          /**< Dash2        */
    char day[2];         /**< Day          */
    char T;              /**< T            */
    char hour[2];        /**< Hour         */
    char colon1;         /**< Colon1       */
    char minute[2];      /**< Minute       */
    char colon2;         /**< Colon2       */
    char second[2];      /**< Second       */
    char decimal;        /**< Decimal      */
    char sub_second[6];  /**< Sub-second   */
    char Z;              /**< UTC timezone */
} wiced_iso8601_time_t;

typedef struct
{
    uint8_t*  buffer;
    uint32_t  size;
    uint32_t  head;
    uint32_t  tail;
} wiced_ring_buffer_t;

#ifdef _DEBUG
#define WICED_BREAK_IF_DEBUG( ) wc_assert("assert", __FILE__, __LINE__)
#else
#define WICED_BREAK_IF_DEBUG( )
#endif

#define GKI_ASSERT() wc_assert("assert", __FILE__, __LINE__)

#define WPRINT_MACRO(args) do {debug_trace args;} while(0==1)

#define WPRINT_ENABLE_APP_INFO
#define WPRINT_ENABLE_APP_DEBUG
#define WPRINT_ENABLE_APP_ERROR
#define WPRINT_ENABLE_LIB_INFO

#ifdef WPRINT_ENABLE_APP_INFO
#define WPRINT_APP_INFO(args) WPRINT_MACRO(args)
#else
#define WPRINT_APP_INFO(args)
#endif
#ifdef WPRINT_ENABLE_APP_DEBUG
#define WPRINT_APP_DEBUG(args) WPRINT_MACRO(args)
#else
#define WPRINT_APP_DEBUG(args)
#endif
#ifdef WPRINT_ENABLE_APP_ERROR
#define WPRINT_APP_ERROR(args) { WPRINT_MACRO(args); WICED_BREAK_IF_DEBUG(); }
#else
#define WPRINT_APP_ERROR(args) { WICED_BREAK_IF_DEBUG(); }
#endif
#ifdef WPRINT_ENABLE_LIB_INFO
#define WPRINT_LIB_INFO(args) WPRINT_MACRO(args)
#else
#define WPRINT_LIB_INFO(args)
#endif

/* Wiced base priority for GKI_TASK_ID 0  */
//#define GKI_WICED_BASE_PRIORITY     160
//#define GKI_TICKS_PRIORITY          200

//#define READ_THREAD_PRI             2   /* test: lower than player task */

typedef enum wiced_button_event
{
    /* Denotes the events for multifunctional buttons. Differntiates on
    * the time-gap between a press-release cycle */
    EVT_SHORT,
    EVT_LONG,
    EVT_VERYLONG,
    EVT_EXTENDED,
    EVT_MAX,
    EVT_INVALID = EVT_MAX,

}wiced_button_event_t;

#define taskENTER_CRITICAL() do{ OS_EnterRegion();}while(0)
#define  taskEXIT_CRITICAL() do{ OS_LeaveRegion(); }while(0)
#define REFERENCE_DEBUG_ONLY_VARIABLE(x)  ( (void)(x) )

void *GKI_igetpoolbuf(UINT8 pool_id);
BOOLEAN gki_timers_is_timer_running(void);
BOOLEAN gki_timers_is_timer_running(void);
#if 0
wiced_result_t wiced_rtos_init_mutex(wiced_mutex_t* mutex);
wiced_result_t wiced_rtos_lock_mutex(wiced_mutex_t* mutex);
wiced_result_t wiced_rtos_unlock_mutex(wiced_mutex_t* mutex);
wiced_result_t wiced_rtos_deinit_mutex(wiced_mutex_t* mutex);
wiced_result_t wiced_time_get_iso8601_time(wiced_iso8601_time_t* iso8601_time);
wiced_result_t wiced_rtos_init_queue(wiced_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages, pGKI_OS pCtx, int index);
wiced_result_t wiced_rtos_deinit_queue(wiced_queue_t* queue, pGKI_OS pCtx, int index);
wiced_result_t wiced_rtos_is_queue_empty(wiced_queue_t* queue);
wiced_result_t wiced_rtos_is_queue_full(wiced_queue_t* queue);
wiced_result_t wiced_rtos_push_to_queue(wiced_queue_t* queue, void* message, uint32_t timeout_ms);
wiced_result_t wiced_rtos_pop_from_queue(wiced_queue_t* queue, void* message, uint32_t timeout_ms);
wiced_result_t wiced_rtos_is_current_thread(wiced_thread_t* thread);
wiced_result_t wiced_rtos_delete_thread(wiced_thread_t* thread);
wiced_result_t wiced_time_get_time(wiced_time_t* time_ptr);
#endif
void LogMsg(UINT32 trace_set_mask, const char *fmt_str, ...);
void ScrLog(UINT32 trace_set_mask, const char *fmt_str, ...);
void log2udp(char * msg);
//void userial_read_thread(void);
void debug_trace(const char * fmt_str, ...);
void dump_array(const char * tag, const UINT8* data, int len);
char * get_windows_time(void);
#ifdef WIN32
#define wiced_assert( error_string, assertion ) do{ if(!assertion) { debug_trace(error_string); assert(0); };} while(0)
char * get_windows_time();
wiced_result_t wiced_load_nv_content();
#else
#define wiced_assert( error_string, assertion ) do{ if(!assertion) { debug_trace(error_string);OS_Error(-1); };} while(0)
#endif
wiced_result_t wiced_dct_read_lock(void** info_ptr, int ptr_is_writable, int section, uint32_t offset, uint32_t size);
wiced_result_t wiced_dct_read_unlock(void* info_ptr, int ptr_is_writable);
wiced_result_t wiced_dct_write(const void* info_ptr, int section, uint32_t offset, uint32_t size);
void DispSmpMsg(BT_HDR *p_buf, BOOLEAN is_recv);

void wiced_bt_power_lock(void);
void wiced_bt_power_unlock(void);
void set_uart1_rts(int state);

// this will be stored in NV
#define BLUETOOTH_INFO_START_NSEC        (1024 * 900 / 4096)
#define BLUETOOTH_HCD_NSEC               (BLUETOOTH_INFO_START_NSEC + 1)  // from this sector, HCD content is stored
//#define BLUETOOTH_BOND_DEV_INFO_NSEC     (BLUETOOTH_INFO_START_NSEC + 1)
#define FLASH_SECTOR_SIZE 4096

typedef struct {
    char signature[16]; // should be "BRCM4774NV"
    char bda[8];  // bdaddress
    char adv_name[20];
    char peer_devices[2048];
}bte_cfg_nv_t, *bte_cfg_nv_p ;

extern bte_cfg_nv_p bluetooth_info;

void load_bt_info_from_sflash(void);

#endif