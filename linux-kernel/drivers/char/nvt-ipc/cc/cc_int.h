/**

    CC module internal header file

    CC module internal header file

    @file       cc_int.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef _CC_INT_H
#define _CC_INT_H

//#include <stdio.h>
//#include <pthread.h>


#define DBG_ERR(fmtstr, args...) printk("ERR: " fmtstr, ##args)

#define DBG_WRN(fmtstr, args...) printk("WRN: " fmtstr, ##args)

#if 1
#define DBG_IND(fmtstr, args...)
#else
#define DBG_IND(fmtstr, args...) printk("%s(): " fmtstr, \
										__func__, ##args)
#endif

#define DBG_DUMP(fmtstr, args...) printk(fmtstr, ##args)
/**
    @addtogroup mIDrvSys_CC
*/
//@{

/*
    CC register default value

    CC register default value.
*/
typedef struct {
	UINT32  uiOffset;
	UINT32  uiValue;
	CHAR    *pName;
} CC_REG_DEFAULT;

#define CC_INT_EVENT_FROM_CPU1          0x00000001
#define CC_INT_EVENT_FROM_CPU2          0x00000002
#define CC_INT_EVENT_FROM_DSP           0x00000004
#define CC_INT_EVENT_FROM_DSP2          0x00000008

#define CC_INT_ACK_FROM_CPU1            0x00010000
#define CC_INT_ACK_FROM_CPU2            0x00020000
#define CC_INT_ACK_FROM_DSP             0x00040000
#define CC_INT_ACK_FROM_DSP2            0x00080000

#define SEM_HANDLE                  struct semaphore
#define SEM_CREATE(handle,init_cnt) sema_init(&handle,init_cnt)
#define SEM_SIGNAL(handle)          up(&handle)
#define SEM_WAIT(handle)            down_interruptible(&handle)//down(&handle)
#define SEM_DESTROY(handle)


//@}

#endif
