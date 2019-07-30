/*
    nvt_ipc module internal debug header

    @file       nvt_ipc_debug.h
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/



//@{

#ifndef _NVT_IPC_DEBUG_H
#define _NVT_IPC_DEBUG_H

#define CHKPNT    printk("\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__)
#define DBGD(x)   printk("\033[0;35m%s=%d\033[0m\r\n",#x,x)
#define DBGH(x)   printk("\033[0;35m%s=0x%08X\033[0m\r\n",#x,x)

#ifndef DBG_DUMP
#define DBG_DUMP(fmtstr, args...) printk(fmtstr, ##args)
#define DBG_ERR(fmtstr, args...)  printk("\033[0;31mERR:%s() \033[0m" fmtstr, __func__, ##args)
#define DBG_WRN(fmtstr, args...)  printk("\033[0;33mWRN:%s() \033[0m" fmtstr, __func__, ##args)
#endif

#if 0
#define DBG_IND(fmtstr, args...) printk("%s(): " fmtstr,__func__, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif
//@}
#endif

