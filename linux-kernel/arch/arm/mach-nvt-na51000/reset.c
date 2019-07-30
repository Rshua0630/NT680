/**
    NVT reset function
    This file will setup reset function interface
    @file       nvt-reset.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <linux/pm.h>
#include <linux/reboot.h>
#include <linux/delay.h>

#include <asm/io.h>

#include <mach/nvt-io.h>
#include <mach/nvt-ipc.h>
#include <mach/hardware.h>

static void nvt_ipc_pwr_ack(void)
{
	nvt_writel(0xffff0000, NVT_CC_BASE_VIRT + 0x118);
	// set NVTIPC_SYSCMD_POWEROFF_ACK
	nvt_writel(0xB, NVT_CC_BASE_VIRT + 0x11C);
	// clear AckFromCPU1 bit
	nvt_writel(0x10000, NVT_CC_BASE_VIRT + 0x24);
	// TriggerCPU1
	nvt_writel(0x1, NVT_CC_BASE_VIRT + 0x20);
}

static void nvt_ipc_reboot_req(void)
{
	NVTIPC_SYS_MSG ipcSysMsg = {0};
	ipcSysMsg.sysCmdID =NVTIPC_SYSCMD_REBOOT_REQ;
	nvt_ipc_msg_snd(0, NVTIPC_SENDTO_CORE1, &ipcSysMsg, sizeof(ipcSysMsg));
}

static void nvt_ipc_poweroff_req(void)
{
	NVTIPC_SYS_MSG ipcSysMsg = {0};
	ipcSysMsg.sysCmdID =NVTIPC_SYSCMD_POWEROFF_REQ;
	nvt_ipc_msg_snd(0, NVTIPC_SENDTO_CORE1, &ipcSysMsg, sizeof(ipcSysMsg));
}


static int nvt_notify_reboot(struct notifier_block *nb, unsigned long action, void *data)
{
	if (action == SYS_RESTART) {
		pr_info("restarting.....\n");
		nvt_ipc_reboot_req();
	}

	return NOTIFY_DONE;
}

static void nvt_power_off_prepare(void)
{
	nvt_ipc_poweroff_req();
}

static struct notifier_block nvt_reboot_notifier = {
	.notifier_call  = nvt_notify_reboot,
	.next       = NULL,
	.priority   = INT_MAX,
};

static void nvt_power_off_done(void)
{
	pr_info(" Power off \n");
	nvt_ipc_pwr_ack();
}

static int __init nvt_poweroff_setup(void)
{
	pr_info("*** %s \n", __func__);

	pm_power_off = nvt_power_off_done;
        pm_power_off_prepare = nvt_power_off_prepare;
	register_reboot_notifier(&nvt_reboot_notifier);

	return 0;
}

/* It will be executed when restart done */
void nvt_na51000_restart(enum reboot_mode mode, const char *cmd)
{
	pr_info("done\n");
	nvt_ipc_pwr_ack();
	/* Waiting for core0 rtc flow ready */
	mdelay(5000);
}

arch_initcall(nvt_poweroff_setup);
