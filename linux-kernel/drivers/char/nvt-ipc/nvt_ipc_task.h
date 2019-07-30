/**

    The header file of nvt_ipc tasks APIs.

    @file       nvt_ipc_task.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#ifndef NVT_IPC_TASK_H
#define NVT_IPC_TASK_H
#include <mach/nvt_type.h>
#include <mach/nvt-ipc.h>



extern int  NvtIpc_Task_Init(void);
extern void NvtIpc_Task_Exit(void);
extern void NvtIpc_Task_SetErrCode(INT32 err, UINT32 CMD);
#endif
