/**

    The header file of nvt_ipc signal APIs.

    @file       nvt_ipc_sig.h
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#ifndef NVT_IPC_SIG_H
#define NVT_IPC_SIG_H
#include <mach/nvt_type.h>
#include <mach/nvt-ipc.h>

#define NVTIPC_SIG_MAX_WAIT_NUM              32
#define NVTIPC_SIG_SUSPEND_TIMEOUT_SEC       10


/**
   NVTIPC signal.

*/
typedef enum _NVTIPC_SIG{
    NVTIPC_SIG_SUSPEND = 1,                ///< suspend signal
    NVTIPC_SIG_RESUME,                     ///< resume  signal
    NVTIPC_SIG_MAX = NVTIPC_SIG_RESUME,    ///< Max value
   ENUM_DUMMY4WORD(NVTIPC_SIG)
}NVTIPC_SIG;


typedef struct {
	wait_queue_head_t wait_sig_q;
	wait_queue_head_t wait_ack_q;
	int               sig_event;
	unsigned int      wait_pid_list[NVTIPC_SIG_MAX_WAIT_NUM];
	unsigned int      ack_pid_list[NVTIPC_SIG_MAX][NVTIPC_SIG_MAX_WAIT_NUM];
	bool              is_waiting_timeout;
} NVTIPC_SIG_CTRL, *PNVTIPC_SIG_CTRL;



extern void       nvt_ipc_sig_init(NVTIPC_SIG_CTRL  *sig_ctrl);
extern NVTIPC_I32 nvt_ipc_sig_send(int sig_event);
extern NVTIPC_I32 nvt_ipc_sig_wait_all_ack(NVTIPC_SIG sig, NVTIPC_U32 timeout_sec);
extern NVTIPC_I32 nvt_ipc_sig_wait(void);
extern NVTIPC_I32 nvt_ipc_sig_ack(NVTIPC_SIG sig);
extern void       nvt_ipc_sig_rm_pid_from_wait_list(pid_t pid);


#endif
