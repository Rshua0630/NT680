/*
 *  drivers/watchdog/nvt_wdt.c
 *
 *  Author:     Howard Chang
 *  Created:    May 15, 2015
 *  Copyright:  Novatek Inc.
 *
 */
#include <linux/bitops.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <mach/wdt_reg.h>
#include <linux/clk.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/stddef.h>
#include <linux/delay.h>

#define DRV_VERSION		"1.01.010"

/*The WDT range is from 1s to 89.478 s*/
#define NVT_WDT_DEFAULT_TIME		80 /*s*/
static bool     WDT_OPEN = false;
static spinlock_t wdt_clock_lock;

#if 1
unsigned int _WDT_REGIOBASE;
#define WDT_GETREG(ofs)				readl((void __iomem *)(_WDT_REGIOBASE + ofs))
#define WDT_SETREG(ofs, value)		writel(value, (void __iomem *)(_WDT_REGIOBASE + ofs))
#else
#define WDT_REG_ADDR(ofs)	(IOADDR_WDT_REG_BASE+(ofs))
#define WDT_GETREG(ofs)	INW(WDT_REG_ADDR(ofs))
#define WDT_SETREG(ofs, value)	OUTW(WDT_REG_ADDR(ofs), (value))
#endif
#define loc_cpu()		spin_lock_irq(&wdt_clock_lock)
#define unl_cpu()		spin_unlock_irq(&wdt_clock_lock)

static unsigned long nvt_wdt_busy;

static int wdt_time = NVT_WDT_DEFAULT_TIME;
module_param(wdt_time, int, 0);
MODULE_PARM_DESC(wdt_time, "wdt default time in sec");

#define NVTWDT_EXIT	0
#define NVTWDT_CLR	1
#define NVTWDT_EXITCLR	2
static int kick_daemon = 1;
module_param(kick_daemon, int, 0);
MODULE_PARM_DESC(kick_daemon, "Enable kick daemon as default, \
				set 0 to disable it");
struct task_struct *task_nvt_daemon = NULL;
volatile static unsigned int wdt_state = 0;

/*
 * Disable the watchdog.
 */
static inline void nvt_wdt_stop(void)
{
	union WDT_CTRL_REG ctrl_reg;
	union WDT_STS_REG status_reg;

	if (WDT_OPEN == false) {
		pr_err("WDT is not opened!\r\n");
		return;
	}

	/*Enter critical section*/
	loc_cpu();

	/*Enable WDT*/
	ctrl_reg.reg = WDT_GETREG(WDT_CTRL_REG_OFS);
	ctrl_reg.bit.enable = 0;
	ctrl_reg.bit.key_ctrl = WDT_KEY_VALUE;
	WDT_SETREG(WDT_CTRL_REG_OFS, ctrl_reg.reg);

	/*Wait for WDT is really enabled*/
	while (1) {
		status_reg.reg = WDT_GETREG(WDT_STS_REG_OFS);
		if (status_reg.bit.en_status == 0)
			break;
	}

	WDT_OPEN = false;

	/*Leave critical section*/
	unl_cpu();
}

/*
 * Enable and reset the watchdog.
 */
static inline void nvt_wdt_start(void)
{
	union WDT_CTRL_REG ctrl_reg;
	union WDT_STS_REG status_reg;


	if (WDT_OPEN == false) {
		pr_err("WDT is not opened!\r\n");
		return;
	}

	/*wait 1T time before set WDT_EN*/
	udelay(80);

	/*Enable WDT*/
	ctrl_reg.reg = WDT_GETREG(WDT_CTRL_REG_OFS);
	ctrl_reg.bit.enable = 1;
	ctrl_reg.bit.key_ctrl = WDT_KEY_VALUE;
	WDT_SETREG(WDT_CTRL_REG_OFS, ctrl_reg.reg);

	/*Wait for WDT is really enabled*/
	while (1) {
		status_reg.reg = WDT_GETREG(WDT_STS_REG_OFS);
		if (status_reg.bit.en_status == 1)
			break;
	}

}

/*
 * Reload the watchdog timer.  (ie, pat the watchdog)
 */
static inline void nvt_wdt_reload(void)
{
	union WDT_TRIG_REG trig_reg;

	if (WDT_OPEN == false) {
		pr_err("WDT is not opened!\r\n");
		return;
	}

	/*Wait for previous trigger done*/
	while (1) {
		trig_reg.reg = WDT_GETREG(WDT_TRIG_REG_OFS);
		if (trig_reg.bit.trigger == 0)
			break;
	}

	/*Trigger WDT*/
	trig_reg.bit.trigger = 1;
	WDT_SETREG(WDT_TRIG_REG_OFS, trig_reg.reg);

}

static int wdt_daemon(void *data)
{
	struct sched_param param = { .sched_priority = 99 };

	sched_setscheduler(current, SCHED_FIFO, &param);
	current->flags |= PF_NOFREEZE;

	set_current_state(TASK_INTERRUPTIBLE);

	while(wdt_state != NVTWDT_EXIT)
	{
		switch(wdt_state) {
			case NVTWDT_CLR:
				nvt_wdt_reload();
				break;
			case NVTWDT_EXITCLR:
				break;
			default:
				break;
		}
		schedule_timeout_interruptible(msecs_to_jiffies(wdt_time*1000/2)+1);
	}

	set_current_state(TASK_RUNNING);

	return 0;
}


/*
 * Watchdog device is opened
 */
static int nvt_wdt_open(struct inode *inode, struct file *file)
{
	int ret;
	if (test_and_set_bit(0, &nvt_wdt_busy))
		return -EBUSY;

	ret = nonseekable_open(inode, file);
	if (!ret)
		wdt_state = NVTWDT_EXITCLR;

	return ret;
}

/*
 * Close the watchdog device.
 */
static int nvt_wdt_close(struct inode *inode, struct file *file)
{
	clear_bit(0, &nvt_wdt_busy);
	return 0;
}

/*
 * Change the watchdog time interval.
 */
static int nvt_wdt_settimeout(int new_time)
{
	uint32_t clk_value;
	union WDT_CTRL_REG ctrl_reg;

	clk_value = (new_time * WDT_SOURCE_CLOCK) >> 12;
	if (clk_value > WDT_MSB_MAX) {
		pr_err("Invalid value (%d)\n", new_time);
		clk_value = WDT_MSB_MAX;
		return -EINVAL;
	}

	/*Enter critical section*/
	loc_cpu();

	ctrl_reg.reg = WDT_GETREG(WDT_CTRL_REG_OFS);
	ctrl_reg.bit.msb = clk_value;
	ctrl_reg.bit.key_ctrl = WDT_KEY_VALUE;
	WDT_SETREG(WDT_CTRL_REG_OFS, ctrl_reg.reg);

	/*Leave critical section*/
	unl_cpu();

	/* Set new watchdog time. It will be used when
	   nvt_wdt_start() is called. */
	wdt_time = new_time;

	return 0;
}

static struct watchdog_info nvt_wdt_info = {
	.identity	= "nvt watchdog",
	.options	= WDIOF_SETTIMEOUT,
};

/*
 * Handle commands from user-space.
 */
static long nvt_wdt_ioctl(struct file *file,
					unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int new_value;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &nvt_wdt_info,
				sizeof(nvt_wdt_info)) ? -EFAULT : 0;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);

	case WDIOC_SETOPTIONS:
		if (get_user(new_value, p))
			return -EFAULT;
		if (new_value & WDIOS_DISABLECARD)
			nvt_wdt_stop();

		if (new_value & WDIOS_ENABLECARD)
			nvt_wdt_start();
		return 0;

	case WDIOC_KEEPALIVE:
		nvt_wdt_reload();	/* pat the watchdog */
		return 0;

	case WDIOC_SETTIMEOUT:
		if (get_user(new_value, p))
			return -EFAULT;
		if (nvt_wdt_settimeout(new_value))
			return -EINVAL;
		/* Enable new time value */
		nvt_wdt_start();
		/* Return current value */
		return put_user(wdt_time, p);
	case WDIOC_GETTIMEOUT:
		return put_user(wdt_time, p);
	default:
		return -ENOTTY;
	}
}

/*
 * Pat the watchdog whenever device is written to.
 */
static ssize_t nvt_wdt_write(struct file *file, const char *data,
						size_t len, loff_t *ppos)
{
	nvt_wdt_reload();		/* pat the watchdog */
	return len;
}

/* ......................................................................... */

static const struct file_operations nvt_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.unlocked_ioctl	= nvt_wdt_ioctl,
	.open		= nvt_wdt_open,
	.release	= nvt_wdt_close,
	.write		= nvt_wdt_write,
};

static struct miscdevice nvt_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &nvt_wdt_fops,
};

static void nvt_wdt_init(void)
{
	union WDT_CTRL_REG ctrl_reg;
	wdt_state = NVTWDT_CLR;

	/*Enter critical section*/
	loc_cpu();

	/*Set the WDT as reset mode*/
	ctrl_reg.reg = 0;
	ctrl_reg.bit.mode       = WDT_MODE_RESET;
	ctrl_reg.bit.key_ctrl   = WDT_KEY_VALUE;
#ifdef CONFIG_NA51000_WATCHDOG_EXTERNAL_RESET
	ctrl_reg.bit.ext_reset  = 0x1;
#endif
	WDT_SETREG(WDT_CTRL_REG_OFS, ctrl_reg.reg);

	/*Leave critical section*/
	unl_cpu();

	if (nvt_wdt_settimeout(wdt_time))
		nvt_wdt_settimeout(NVT_WDT_DEFAULT_TIME);

	WDT_OPEN = true;

	if (wdt_time > 0) {
		nvt_wdt_start();

		if(kick_daemon) {
			struct task_struct *p_dog;
			p_dog = kthread_create(wdt_daemon, NULL, "nvt_wdt");
			if(IS_ERR(p_dog) <0) {
				pr_err("create wdt_daemon failed!\n");
			}
			wake_up_process(p_dog);
			task_nvt_daemon = p_dog;
		}
	}
}

static int nvt_wdt_probe(struct platform_device *pdev)
{
	int res;

	struct clk	*wdt_clk;

	struct resource *memres = NULL;

	if (pdev->resource->flags == IORESOURCE_MEM) {
        /* setup resource */
        memres = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        if (unlikely(!memres)) {
            printk("%s fails: platform_get_resource IORESOURCE_MEM not OK", __FUNCTION__);
			return -ENXIO;
        }
	}

	_WDT_REGIOBASE = (u32) ioremap_nocache(memres->start, resource_size(memres));
	if (unlikely(_WDT_REGIOBASE == 0)) {
		printk("%s fails: ioremap_nocache fail\n", __FUNCTION__);
		goto remap_err;
	}

	if (nvt_wdt_miscdev.parent)
		return -EBUSY;

	nvt_wdt_miscdev.parent = &pdev->dev;

	res = misc_register(&nvt_wdt_miscdev);
	if (res) {
		pr_err("misc_register fail, ret = %d\n", res);
		return res;
	}

	spin_lock_init(&wdt_clock_lock);

	wdt_clk = devm_clk_get(&pdev->dev, dev_name(&pdev->dev));
	if (!IS_ERR(wdt_clk)) {
		clk_prepare(wdt_clk);
		clk_enable(wdt_clk);
	} else
		pr_err("%s: wdt clk not found\n", __func__);

	nvt_wdt_init();

	return 0;

remap_err:
    release_mem_region(memres->start, resource_size(memres));
	return -1;

}

static int nvt_wdt_remove(struct platform_device *pdev)
{
	int res;

	wdt_state = NVTWDT_EXIT;

	if (pdev->resource->flags == IORESOURCE_MEM) {
		/* remove resource */
		iounmap((void __iomem *)_WDT_REGIOBASE);
		release_mem_region(_WDT_REGIOBASE, resource_size(pdev->resource));
	}

	res = misc_deregister(&nvt_wdt_miscdev);
	if (!res)
		nvt_wdt_miscdev.parent = NULL;

	nvt_wdt_stop();

	if(kick_daemon) {
		struct task_struct *p_dog = task_nvt_daemon;
		if(p_dog == NULL)
			return E_SYS;
		wake_up_process(p_dog);
		kthread_stop(p_dog);
		yield();
	}

	task_nvt_daemon = NULL;

	return res;
}

static void nvt_wdt_shutdown(struct platform_device *pdev)
{
	nvt_wdt_stop();
}

#ifdef CONFIG_PM

static int nvt_wdt_suspend(struct platform_device *pdev,
			      pm_message_t message)
{
	nvt_wdt_stop();
	return 0;
}

static int nvt_wdt_resume(struct platform_device *pdev)
{
	if (nvt_wdt_busy)
		nvt_wdt_start();
	return 0;
}

#else
#define nvt_wdt_suspend NULL
#define nvt_wdt_resume	NULL
#endif

#ifdef CONFIG_OF
static const struct of_device_id nvt_wdt_match[] = {
	{ .compatible = "nvt,nvt_wdt" },
	{},
};

MODULE_DEVICE_TABLE(of, nvt_wdt_match);
#endif

static struct platform_driver nvt_wdt_driver = {
	.probe		= nvt_wdt_probe,
	.remove		= nvt_wdt_remove,
	.shutdown	= nvt_wdt_shutdown,
	.suspend	= nvt_wdt_suspend,
	.resume		= nvt_wdt_resume,
	.driver		= {
		.name	= "nvt_wdt",
		.owner	= THIS_MODULE,
#ifdef CONFIG_OF
        .of_match_table = nvt_wdt_match,
#endif
	},
};

module_platform_driver(nvt_wdt_driver);

MODULE_AUTHOR("Howard Chang @ Novatek");
MODULE_DESCRIPTION("Watchdog driver for Novatek");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
MODULE_ALIAS("platform:nvt_wdt");

