#pragma once

#include "data_types.h"
#include "gki_common.h"
#include "wiced_result.h"
#include "stddef.h"
#include "Delay.h"

extern UINT32 _SECTION(".kercfg_data")  trace_mutex;
extern char   bt_nv_ram_store[1024*3];

void __assert_func(const char *fileName, int line, const char *funcName, const char *condition) __attribute__ ((__noreturn__));
//#define ASSERT(__e) ((__e) ? (void)0 : __assert_func (__FILE__, __LINE__, __FUNCTION__, #__e))

void debug_trace(const char * fmt_str, ...)  _SECTION(".kercfg_text");
//#define debug_trace   debug_msg

void debug_dump_hex(uint8 * data, int length);

void vPortFree(void *pv);
void *pvPortMalloc(size_t xWantedSize);
char * bd2s(char * bda);
void application_start( void );
void application_stop( void );


void USERIAL_ReadThread_create( void ) _SECTION(".kercfg_text");
void bte_create_tasks(void ) _SECTION(".kercfg_text");

