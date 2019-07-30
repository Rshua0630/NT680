#include "Debug.h"
#include "utron_utils.h"
#include "bt_target.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "SysKer.h"
#include "Semaphore.h"
#include "stdio.h"
#include "embos.h"
#include "BT_BrcmInt.h"

#if BT_USE_TRACES == 1

UINT32  trace_mutex;

#if 0

#define trace_log_lock()    do{ wai_sem(trace_mutex); }while(0)
#define trace_log_unlock()  do{ sig_sem(trace_mutex); }while(0)

#else

#define trace_log_lock()
#define trace_log_unlock()
#endif

UINT8 wiced_log_enabled = 1;

char * bd2s(char * bda)
{
    static char buf[20];
    int    size=sizeof(buf);

    memset(buf, 0, sizeof(buf));
    snprintf(buf, size,"%2x:%2x:%2x:%2x:%2x:%2x", bda[0] & 0x0ff
                                          , bda[1] & 0x0ff
                                          , bda[2] & 0x0ff
                                          , bda[3] & 0x0ff
                                          , bda[4] & 0x0ff
                                          , bda[5] & 0x0ff);

    buf[size-1] = 0;
    return buf;
}

void debug_dump_hex(uint8 * data, int length)
{
    int i;

    wai_sem(trace_mutex);

    for (i = 0 ;i < length; i++) {
          if (i%16 == 0) debug_msg("\r\n");
          debug_msg("%02x ", data[i] & 0x0ff);
    }
    debug_msg("\r\n");

    trace_log_unlock();
}

void debug_trace(const char * fmt_str, ...)
{
#if 0
    static char buf[512];
    va_list ap;

    memset(buf, 0, sizeof(buf));
    va_start(ap, fmt_str);
    sprintf(buf, fmt_str, ap);
    va_end(ap);
    debug_msg("%s\r\n", buf);
#else
    va_list ap;
    trace_log_lock();

    //Due to eCos usage
    //coverity[returned_null]
    //coverity[var_assigned]
    va_start(ap, fmt_str);

    //Due to eCos usage
    //coverity[dereference]
    debug_vprintf( fmt_str, ap);

    va_end(ap);
    debug_msg("\r\n");
    trace_log_unlock();
#endif
}

void LogMsg(UINT32 trace_set_mask, const char *fmt_str, ...)
{
#if 0
    static char buf[512];

    va_list ap;

    if ((!wiced_log_enabled) && (TRACE_GET_TYPE(trace_set_mask) != TRACE_TYPE_ERROR))
    {
        /* If  wiced logging disabled, then only log errors */
        return;
    }

    memset(buf, 0, sizeof(buf));
    va_start(ap, fmt_str);
    sprintf(buf, fmt_str, ap);
    va_end(ap);
    debug_msg("%s\r\n", buf);
#else
    va_list ap;

    trace_log_lock();

    //Due to eCos usage
    //coverity[returned_null]
    //coverity[var_assigned]
    va_start(ap, fmt_str);

    //Due to eCos usage
    //coverity[dereference]
    debug_vprintf(fmt_str, ap);

    va_end(ap);
    debug_msg("\r\n");
    trace_log_unlock();
#endif
}

void ScrLog(UINT32 trace_set_mask, const char *fmt_str, ...)
{
#if 0
    static char buf[512];
    va_list ap;

    if ((!wiced_log_enabled) && (TRACE_GET_TYPE(trace_set_mask) != TRACE_TYPE_ERROR))
    {
        /* If  wiced logging disabled, then only log errors */
        return;
    }

    memset(buf, 0, sizeof(buf));

    va_start(ap, fmt_str);
    sprintf(buf, fmt_str, ap);
    va_end(ap);
    debug_msg("%s\r\n", buf);
#else
    va_list ap;

    trace_log_lock();

    //Due to eCos usage
    //coverity[returned_null]
    //coverity[var_assigned]
    va_start(ap, fmt_str);

    //Due to eCos usage
    //coverity[dereference]
    debug_vprintf( fmt_str, ap);

    va_end(ap);
    debug_msg("\r\n");
    trace_log_unlock();
#endif
}

/********************************************************************************
**
**    Function Name:   LogMsg_0
**
**    Purpose:  Encodes a trace message that has no parameter arguments
**
**    Input Parameters:  trace_set_mask: tester trace type.
**                       fmt_str: displayable string.
**    Returns:
**                      Nothing.
**
*********************************************************************************/
void LogMsg_0(UINT32 trace_set_mask, const char *fmt_str)
{
    LogMsg(trace_set_mask, fmt_str);
}

/********************************************************************************
**
**    Function Name:   LogMsg_1
**
**    Purpose:  Encodes a trace message that has one parameter argument
**
**    Input Parameters:  trace_set_mask: tester trace type.
**                       fmt_str: displayable string.
**    Returns:
**                      Nothing.
**
*********************************************************************************/
void LogMsg_1(UINT32 trace_set_mask, const char *fmt_str, UINTPTR p1)
{
    LogMsg(trace_set_mask, fmt_str, p1);
}

/********************************************************************************
**
**    Function Name:   LogMsg_2
**
**    Purpose:  Encodes a trace message that has two parameter arguments
**
**    Input Parameters:  trace_set_mask: tester trace type.
**                       fmt_str: displayable string.
**    Returns:
**                      Nothing.
**
*********************************************************************************/
void LogMsg_2(UINT32 trace_set_mask, const char *fmt_str, UINTPTR p1, UINTPTR p2)
{
    LogMsg(trace_set_mask, fmt_str, p1, p2);
}

/********************************************************************************
**
**    Function Name:   LogMsg_3
**
**    Purpose:  Encodes a trace message that has three parameter arguments
**
**    Input Parameters:  trace_set_mask: tester trace type.
**                       fmt_str: displayable string.
**    Returns:
**                      Nothing.
**
*********************************************************************************/
void LogMsg_3(UINT32 trace_set_mask, const char *fmt_str, UINTPTR p1, UINTPTR p2, UINTPTR p3)
{
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3);
}

/********************************************************************************
**
**    Function Name:   LogMsg_4
**
**    Purpose:  Encodes a trace message that has four parameter arguments
**
**    Input Parameters:  trace_set_mask: tester trace type.
**                       fmt_str: displayable string.
**    Returns:
**                      Nothing.
**
*********************************************************************************/
void LogMsg_4(UINT32 trace_set_mask, const char *fmt_str, UINTPTR p1, UINTPTR p2, UINTPTR p3, UINTPTR p4)
{
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4);
}

/********************************************************************************
**
**    Function Name:   LogMsg_5
**
**    Purpose:  Encodes a trace message that has five parameter arguments
**
**    Input Parameters:  trace_set_mask: tester trace type.
**                       fmt_str: displayable string.
**    Returns:
**                      Nothing.
**
*********************************************************************************/
void LogMsg_5(UINT32 trace_set_mask, const char *fmt_str, UINTPTR p1, UINTPTR p2, UINTPTR p3, UINTPTR p4, UINTPTR p5)
{
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4, p5);
}

/********************************************************************************
**
**    Function Name:   LogMsg_6
**
**    Purpose:  Encodes a trace message that has six parameter arguments
**
**    Input Parameters:  trace_set_mask: tester trace type.
**                       fmt_str: displayable string.
**    Returns:
**                      Nothing.
**
*********************************************************************************/
void LogMsg_6(UINT32 trace_set_mask, const char *fmt_str, UINTPTR p1, UINTPTR p2, UINTPTR p3, UINTPTR p4, UINTPTR p5, UINTPTR p6)
{
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4, p5, p6);
}
#else
void debug_trace(const char * fmt_str, ...){}
//wiced_result_t wiced_time_get_time(wiced_time_t* time_ptr){ return WICED_SUCCESS; }
#endif

char bt_nv_ram_store[1024*3]={0};
wiced_result_t wiced_dct_read_lock(void** info_ptr, int ptr_is_writable, int section, uint32_t offset, uint32_t size)
{
    *info_ptr = GKI_os_malloc(size);

    if ( *info_ptr == NULL)
    {
        return WICED_ERROR;
    }
    memcpy(*info_ptr, (void*)&bt_nv_ram_store[offset], size);
    return WICED_SUCCESS;
}

wiced_result_t wiced_dct_read_unlock(void* info_ptr, int ptr_is_writable)
{
    GKI_os_free(info_ptr);
    return WICED_SUCCESS;
}

wiced_result_t wiced_dct_write(const void* info_ptr, int section, uint32_t offset, uint32_t size)
{
    memcpy(&bt_nv_ram_store[offset], info_ptr, size);
    //#NT#2015/09/01#Lincy Lin -begin
    //#NT#
    BT_StoreInfo();
    //#NT#2015/09/01#Lincy Lin -end
    return WICED_SUCCESS;
}

BOOLEAN GKI_mutex_create(GKI_MUTEX *p_mutex)
{
    OS_CONFIG_SEMPHORE(p_mutex->p_platform_mutex, 0, 1, 1);
    debug_trace("GKI_mutex_create %d\r\n", p_mutex->p_platform_mutex);
    return TRUE;
}

/*******************************************************************************
 **
 ** Function         GKI_mutex_create
 **
 ** Description      Delete mutex
 **
 *******************************************************************************/
void GKI_mutex_delete(GKI_MUTEX *p_mutex)
{
    debug_trace("stub function %s\r\n", __func__);

    return ;
}

/*******************************************************************************
 **
 ** Function         GKI_mutex_lock
 **
 ** Description      Lock mutex
 **
 *******************************************************************************/
BOOLEAN GKI_mutex_lock(GKI_MUTEX *p_mutex)
{
    //debug_trace(" %s\r\n", __func__);
    wai_sem(p_mutex->p_platform_mutex);
    return TRUE;
}

/*******************************************************************************
 **
 ** Function         GKI_mutex_unlock
 **
 ** Description      Unlock mutex
 **
 *******************************************************************************/
BOOLEAN GKI_mutex_unlock(GKI_MUTEX *p_mutex)
{
    //debug_trace("%s\r\n", __func__);
    sig_sem(p_mutex->p_platform_mutex);
    return TRUE;
}


