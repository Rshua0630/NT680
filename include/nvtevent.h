/**
    This header file of nvtevent api.

    nvtevent used to get event delivery from uitron.

    @file       nvtevent.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

*/

#ifndef _NVTEVENT_H
#define _NVTEVENT_H
#include <stdio.h>
#include <sys/time.h>
#include <nvtcfg_define.h>

/* ----------------------------------------------------------------- */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define EVENT_BUF_SIZE	64*1024 ///< the size of share memory for each event data

//for linux compatible
#ifndef IPCAM_EVENT_FD
#define IPCAM_EVENT_FD 0x01
#endif
#ifndef FD_NUM_MAX
#define FD_NUM_MAX     36
#endif
#ifndef ENOMSG
#define ENOMSG 35
#endif

#define IPCAM_EVENT_VQA		2
#define IPCAM_EVENT_BCMD	3
#define BC_MAX_NUM	1024
#define SET_FLAG(flag, val)     flag|=(1<<(val))
#define CLEAR_FLAG(flag, val)   flag&=(~(1<<(val)))
#define IS_FLAG(flag, val)      (((flag)&(1<<(val)))!=0)
#define CUSTOM_BC_USR_MAX_EVT_SIZE 120*90

typedef struct
{
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* microseconds */
} TIME_VAL ;

typedef struct {
	UINT32  x;      ///< X coordinate of the top-left point of the rectangle
	UINT32  y;      ///< Y coordinate of the top-left point of the rectangle
	UINT32  w;      ///< Rectangle width
	UINT32  h;      ///< Rectangle height
} EVENT_URECT, *EVENT_PURECT;

typedef struct {
	UINT32 targetnum;
	EVENT_URECT target[BC_MAX_NUM];
	UINT32 evt_width;
	UINT32 evt_height;
	UINT8 evt_map_addr[CUSTOM_BC_USR_MAX_EVT_SIZE];
} BC_RESULT;

typedef struct {
	char event;
	TIME_VAL TimeStamp;
	BC_RESULT result;
} IPCAM_BC_RESULT;

typedef struct {
	char Event;
	TIME_VAL TimeStamp;
	BOOL TooLight;
	BOOL TooDark;
	BOOL Blur;
} IPCAM_VQA_RESULT;

typedef void* NVTEVENT_HANDLE; ///< nvtevent handle for each api's operation

/**
     the event channel list. the mapping name corresponding to nvtcodec_event.h.
     for example, IPCAM_EVENT_VQA defined as 0x03 is mapping to (0x03-1) = event2

     for invoking nvtevent_subscribe(), the events are indicated which channles
     you want to get notification (multiple selections)

     for invoking nvtevent_wait(), indicate which channles have new message.
*/
typedef union _NVTEVENT_CHANNEL {
	unsigned int mask[4]; ///< 128 bits for 128 messages
	struct {
		unsigned char event0 : 1;
		unsigned char event1 : 1;
		unsigned char event2 : 1;
		unsigned char event3 : 1;
		unsigned char event4 : 1;
		unsigned char event5 : 1;
		unsigned char event6 : 1;
		unsigned char event7 : 1;
		unsigned char event8 : 1;
		unsigned char event9 : 1;
		unsigned char event10 : 1;
		unsigned char event11 : 1;
		unsigned char event12 : 1;
		unsigned char event13 : 1;
		unsigned char event14 : 1;
		unsigned char event15 : 1;
		unsigned char event16 : 1;
		unsigned char event17 : 1;
		unsigned char event18 : 1;
		unsigned char event19 : 1;
		unsigned char event20 : 1;
		unsigned char event21 : 1;
		unsigned char event22 : 1;
		unsigned char event23 : 1;
		unsigned char event24 : 1;
		unsigned char event25 : 1;
		unsigned char event26 : 1;
		unsigned char event27 : 1;
		unsigned char event28 : 1;
		unsigned char event29 : 1;
		unsigned char event30 : 1;
		unsigned char event31 : 1;
	};
} NVTEVENT_CHANNEL;

/**
     the description to get event data and its channel indication
*/
typedef struct _NVTEVENT_PACKAGE {
	NVTEVENT_CHANNEL channel;  ///< indicate the channel of this package
	void             *p_msg;   ///< the data along with channel
	unsigned int     msg_size; ///< the size of p_msg specific memory
} NVTEVENT_PACKAGE;

/**
     the description for nvtevent_wait() to setup the wait method you want.
*/
typedef struct _NVTEVENT_NOTIFY {
	NVTEVENT_PACKAGE package;    ///< [OUTPUT] package description.
	void             *p_tmp;     ///< [INPUT] a buffer to copy data along with p_package
	unsigned int     tmp_size;   ///< [INPUT] the size of p_tmp specific memory
	int              timeout_ms; ///< [INPUT] -1: wait infinite(better), >0: timeout in ms, 0: no wait
} NVTEVENT_NOTIFY;

/**
     initiate the nvtevent library

     before invoking nvtevent_initiate(), the nvteventd is required executed on
     system background.
     !! WARNING !! nvtevent_initiate() is for whole process, DO NOT USE BOTH 2
     THREADS TO INVOKE nvtevent_initiate(), RESPECTIVELY.

     @param[in] *hdl       the pointer of NVTEVENT_HANDLE.
     @return Description of data returned.
         - @b 0:  success
         - @b <0: failed.
     Example:
     @code
     {
        NVTEVENT_HANDLE hdl = NULL;
        int er = nvtevent_initiate(&hdl);
        if (er < 0) {
	        return -1;
	    }
     }
     @endcode
*/
extern int nvtevent_initiate(NVTEVENT_HANDLE *hdl);

/**
     subscribe channels

     after nvtevent_initiate(), use the handle to subscribe channels you want.
     system background.
     !! WARNING !! nvtevent_initiate() is for whole process, DO NOT USE BOTH 2
     THREADS TO INVOKE nvtevent_initiate(), RESPECTIVELY.

     @param[in] *hdl       the pointer of NVTEVENT_HANDLE.
     @return Description of data returned.
         - @b 0:  success
         - @b <0: failed.
     Example:
     @code
     {
        NVTEVENT_CHANNEL channel = {0};
        channel.event0 = 1;
        channel.event5 = 1;
        channel.event7 = 1;
        int er = nvtevent_subscribe(hdl, &channel);
	    if (er < 0) {
	        return -1;
	    }
     }
     @endcode
*/
extern int nvtevent_subscribe(NVTEVENT_HANDLE hdl, NVTEVENT_CHANNEL *p_channel);

/**
     unsubscribe whole channels

     if you just want to unsubscribe some of channels. you can invoke
     nvtevent_subscribe, and then invoke nvtevent_subscribe() again with your
     adjusted channels.

     @param[in] *hdl       the pointer of NVTEVENT_HANDLE.
     @return Description of data returned.
         - @b 0:   success
         - @b <0:  failed.
*/
extern int nvtevent_unsubscribe(NVTEVENT_HANDLE hdl);

/**
     wait new event for subscribed channels.

     the waited result located in NVTEVENT_NOTIFY::NVTEVENT_PACKAGE including
     data address and channel indication. user should provide enough temp buffer
     to copy event data.

     Example:
     @code
     {
        NVTEVENT_NOTIFY notify = {0};
        notify.p_tmp = temp buffer;
        notify.tmp_size = temp size;
        notify.timeout_ms = -1; // -1 for timeout infinite
        while (1) {
	        if ((er=nvtevent_wait(hdl, &notify)) < 0) {
	            break;
	        }
            if (er == ENOMSG) {
                    printf("timeout, no msg got.\n");
                    continue;
            }
        	if (notify.package.channel.event0) {
        		printf("got event0 \n");
        	}
        	if (notify.package.channel.event3) {
        		printf("got event3 \n");
        	}
        }
     }
     @endcode
*/
extern int nvtevent_wait(NVTEVENT_HANDLE hdl, NVTEVENT_NOTIFY *p_notify);

/**
     quit nvtevent library
*/
extern int nvtevent_exit(NVTEVENT_HANDLE hdl);

#ifdef __cplusplus
}
#endif
#endif
