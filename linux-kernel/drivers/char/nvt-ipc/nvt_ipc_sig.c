/**

    This file contains the nvt_ipc kernel thread init/exit and thread main fuction.

    @file       nvt_ipc_task.c
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/bootmem.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/syscalls.h>
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/hrtimer.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <mach/nvt_type.h>
#include "nvt_ipc_debug.h"
#include "nvt_ipc_sig.h"
#include "NvtIpcInt.h"


static NVTIPC_SIG_CTRL   *p_sig_ctrl = NULL;
static DEFINE_SPINLOCK(nvt_sig_lock);


static NVTIPC_I32 nvt_ipc_sig_add_pid_to_list(unsigned int *p_list, pid_t pid)
{
	int i,ret = 0;
	unsigned long flags;

	spin_lock_irqsave(&nvt_sig_lock, flags);
	for (i=0;i<NVTIPC_SIG_MAX_WAIT_NUM;i++){
		if (p_list[i] == pid){
			break;
		}
		if (p_list[i] == 0){
			p_list[i] = pid;
			break;
		}
	}
	if (i == NVTIPC_SIG_MAX_WAIT_NUM){
		DBG_ERR("Exceed max wait num %d\r\n",i);
		ret = -1;
	}
	spin_unlock_irqrestore(&nvt_sig_lock, flags);
	return ret;
}

static bool nvt_ipc_sig_chk_pid_in_list(unsigned int *p_list, pid_t pid)
{
	int i;

	for (i=0;i<NVTIPC_SIG_MAX_WAIT_NUM;i++){
		if (p_list[i] == pid){
			DBG_IND("pid = %d in list\r\n",pid);
			return true;
		}
	}
	DBG_IND("pid = %d not in list\r\n",pid);
	return false;
}

static bool nvt_ipc_sig_chk_all_pid_ack(unsigned int *p_wait_list,unsigned int *p_ack_list)
{
	int i,j;

	for (i=0;i<NVTIPC_SIG_MAX_WAIT_NUM;i++){
		if (p_wait_list[i] == 0)
			continue;
		for (j=0;j<NVTIPC_SIG_MAX_WAIT_NUM;j++){
			if (p_ack_list[j] ==0 || p_wait_list[i] == p_ack_list[j]){
				break;
			}
		}
		if (j==NVTIPC_SIG_MAX_WAIT_NUM || p_ack_list[j] == 0 ){
			DBG_IND("pid = %d not ack\r\n",p_wait_list[i]);
			return false;
		}
	}
	DBG_IND("all ack\r\n");
	return true;
}

static unsigned int *nvt_ipc_sig_get_acklist_by_sig(NVTIPC_SIG sig)
{
	unsigned int *p_ack_list;

	p_ack_list = p_sig_ctrl->ack_pid_list[sig-1];
	return p_ack_list;
}


static void nvt_ipc_sig_dump_list(NVTIPC_SIG sig)
{
	int           i;
	unsigned int *p_ack_list;


	DBG_DUMP("ipc signal wait pid list");
	for (i=0;i<NVTIPC_SIG_MAX_WAIT_NUM;i++){
		if (p_sig_ctrl->wait_pid_list[i] !=0)
			DBG_DUMP(" %d,",p_sig_ctrl->wait_pid_list[i]);
	}
	DBG_DUMP("\r\nipc signal ack pid list");
	p_ack_list = nvt_ipc_sig_get_acklist_by_sig(sig);
	for (i=0;i<NVTIPC_SIG_MAX_WAIT_NUM;i++){
		if (p_ack_list[i] !=0)
			DBG_DUMP(" %d,",p_ack_list[i]);
	}
	DBG_DUMP("\r\n");
}

static void nvt_ipc_sig_clear_status(void)
{
	unsigned long flags;

	spin_lock_irqsave(&nvt_sig_lock, flags);
	p_sig_ctrl->sig_event = 0;
	p_sig_ctrl->is_waiting_timeout = 0;
	memset(p_sig_ctrl->ack_pid_list,0x00,sizeof(p_sig_ctrl->ack_pid_list));
	spin_unlock_irqrestore(&nvt_sig_lock, flags);
}


void nvt_ipc_sig_init(NVTIPC_SIG_CTRL  *sig_ctrl)
{
	p_sig_ctrl = sig_ctrl;
	memset(sig_ctrl,0x00,sizeof(NVTIPC_SIG_CTRL));
	init_waitqueue_head(&p_sig_ctrl->wait_sig_q);
	init_waitqueue_head(&p_sig_ctrl->wait_ack_q);

}

NVTIPC_I32 nvt_ipc_sig_send(int sig_event)
{
	if (p_sig_ctrl == NULL){
		DBG_ERR("ipc sig not init\r\n");
		return -1;
	}
	if (sig_event < NVTIPC_SIG_SUSPEND || sig_event > NVTIPC_SIG_MAX){
		DBG_ERR("invalid sig %d\r\n",sig_event);
		return -1;
	}
	DBG_IND("sig_event = %d\r\n",sig_event);
	nvt_ipc_sig_clear_status();
	p_sig_ctrl->sig_event = sig_event;
	wake_up_interruptible(&p_sig_ctrl->wait_sig_q);
	return 0;
}


static enum hrtimer_restart  nvt_ipc_sig_timer_cb(struct hrtimer *timer)
{
	DBG_WRN("ipc signal suspend Timeout\r\n");
	p_sig_ctrl->is_waiting_timeout = 1;
	wake_up_interruptible(&p_sig_ctrl->wait_ack_q);
	return HRTIMER_NORESTART;
}

static void nvt_ipc_sig_create_timer(struct  hrtimer *p_htimer, NVTIPC_U32 timeout_sec)
{
	ktime_t kt_periode;

	kt_periode = ktime_set(timeout_sec, 0); //seconds,nanoseconds
	hrtimer_init(p_htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	p_htimer->function = nvt_ipc_sig_timer_cb;
	hrtimer_start(p_htimer, kt_periode, HRTIMER_MODE_REL);
}

static void nvt_ipc_sig_delete_timer(struct  hrtimer *p_htimer)
{
	hrtimer_cancel(p_htimer);
}


NVTIPC_I32 nvt_ipc_sig_wait_all_ack(NVTIPC_SIG sig, NVTIPC_U32 timeout_sec)
{
	wait_queue_head_t    *pWaitq;
	int                   ret;
	struct                hrtimer htimer;
	unsigned int          *p_ack_list;

	if (p_sig_ctrl == NULL){
		DBG_ERR("ipc sig not init\r\n");
		return -1;
	}
	if (sig < NVTIPC_SIG_SUSPEND || sig > NVTIPC_SIG_MAX){
		DBG_ERR("invalid sig %d\r\n",sig);
		return -1;
	}
	p_ack_list = nvt_ipc_sig_get_acklist_by_sig(sig);
	DBG_IND("begin\r\n");
	nvt_ipc_sig_create_timer(&htimer,timeout_sec);
	pWaitq = &p_sig_ctrl->wait_ack_q;
	while (1) {
		if ((ret = wait_event_interruptible(*pWaitq, nvt_ipc_sig_chk_all_pid_ack(p_sig_ctrl->wait_pid_list,p_ack_list)==true || p_sig_ctrl->is_waiting_timeout == true))) {
			// wake up by process signal not ipc signal
			DBG_IND("ret = %d\r\n",ret);
			nvt_ipc_sig_delete_timer(&htimer);
			return -ERESTARTSYS;
		}
		DBG_IND("ret = %d\r\n",ret);
		break;
	}
	nvt_ipc_sig_delete_timer(&htimer);
	DBG_IND("ret = %d\r\n",ret);
	nvt_ipc_sig_dump_list(sig);
	nvt_ipc_sig_clear_status();
	DBG_IND("end\r\n");
	return 0;
}


NVTIPC_I32 nvt_ipc_sig_wait(void)
{
	wait_queue_head_t    *pWaitq;
	int                   ret;

	if (p_sig_ctrl == NULL){
		DBG_ERR("ipc sig not init\r\n");
		return -1;
	}
	pWaitq = &p_sig_ctrl->wait_sig_q;
	DBG_IND("add wait pid = %d\r\n",current->pid);
	nvt_ipc_sig_add_pid_to_list(p_sig_ctrl->wait_pid_list,current->pid);
	while (1) {
		if (wait_event_interruptible(*pWaitq, ((p_sig_ctrl->sig_event !=0) && (nvt_ipc_sig_chk_pid_in_list(nvt_ipc_sig_get_acklist_by_sig(p_sig_ctrl->sig_event),current->pid) == false)))) {
			// wake up by process signal not ipc signal
			return -ERESTARTSYS;
		}
		break;
	}
	ret = p_sig_ctrl->sig_event;
	// resume case, need to auto ack
	if (p_sig_ctrl->sig_event == NVTIPC_SIG_RESUME){
		unsigned int *p_ack_list;

		p_ack_list = nvt_ipc_sig_get_acklist_by_sig(p_sig_ctrl->sig_event);
		nvt_ipc_sig_add_pid_to_list(p_ack_list,current->pid);
		if (nvt_ipc_sig_chk_all_pid_ack(p_sig_ctrl->wait_pid_list,p_ack_list) == true){
			nvt_ipc_sig_dump_list(p_sig_ctrl->sig_event);
			nvt_ipc_sig_clear_status();
		}
	}
	DBG_IND("wait end pid = %d\r\n",current->pid);
	return ret;
}

NVTIPC_I32 nvt_ipc_sig_ack(NVTIPC_SIG sig)
{
	unsigned int *p_ack_list;
	if (p_sig_ctrl == NULL){
		DBG_ERR("ipc sig not init\r\n");
		return -1;
	}
	if (sig < NVTIPC_SIG_SUSPEND || sig > NVTIPC_SIG_MAX){
		DBG_ERR("invalid sig %d\r\n",sig);
		return -1;
	}
	p_ack_list = nvt_ipc_sig_get_acklist_by_sig(sig);
	nvt_ipc_sig_add_pid_to_list(p_ack_list,current->pid);
	DBG_IND("add ack pid = %d\r\n",current->pid);
	wake_up_interruptible(&p_sig_ctrl->wait_ack_q);
	return 0;
}


void nvt_ipc_sig_rm_pid_from_wait_list(pid_t pid)
{
	int           i;
	unsigned long flags;
	unsigned int *p_wait_list = p_sig_ctrl->wait_pid_list;
	bool          is_find = false;

	spin_lock_irqsave(&nvt_sig_lock, flags);
	for (i=0;i<NVTIPC_SIG_MAX_WAIT_NUM;i++){
		if (p_wait_list[i] == pid){
			p_wait_list[i] = 0;
			is_find = true;
			break;
		}
	}
	spin_unlock_irqrestore(&nvt_sig_lock, flags);
	if (is_find){
		DBG_DUMP("NvtIPC Rel pid %d from wait list\r\n", pid);
		wake_up_interruptible(&p_sig_ctrl->wait_ack_q);
	}
}

