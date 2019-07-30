/*
 *  drivers/watchdog/nt72668_wdt.c
 *
 *  Author:     Alvin lin
 *  Created:    Mar 7, 2012
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
#include <nvt_stbc.h>

#define NT72668_WDT_DEFAULT_TIME		10	/* seconds */
#define NT72668_WDT_MAX_TIME			64	/* seconds */
#define NT72668_WDT_MIN_TIME			5	/* seconds */

static int wdt_time = NT72668_WDT_DEFAULT_TIME;
static unsigned long nt72668wdt_busy;

/*
 * Disable the watchdog.
 */
static inline void nt72668_wdt_stop(void)
{
	nt72668_stbc_enable_wdog(0);
}

/*
 * Enable and reset the watchdog.
 */
static inline void nt72668_wdt_start(void)
{
	nt72668_stbc_enable_wdog(wdt_time);
}

/*
 * Reload the watchdog timer.  (ie, pat the watchdog)
 */
static inline void nt72668_wdt_reload(void)
{
	nt72668_stbc_kick_wdog();
}

/* ......................................................................... */

/*
 * Watchdog device is opened
 */
static int nt72668_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &nt72668wdt_busy))
		return -EBUSY;

	return nonseekable_open(inode, file);
}

/*
 * Close the watchdog device.
 */
static int nt72668_wdt_close(struct inode *inode, struct file *file)
{

	clear_bit(0, &nt72668wdt_busy);
	return 0;
}

/*
 * Change the watchdog time interval.
 */
static int nt72668_wdt_settimeout(int new_time)
{
	if (new_time != 0 && ((new_time < NT72668_WDT_MIN_TIME) || (new_time > NT72668_WDT_MAX_TIME)))
		return -EINVAL;

	/* Set new watchdog time. It will be used when
	   nt72668_wdt_start() is called. */
	wdt_time = new_time;
	return 0;
}

static struct watchdog_info nt72668_wdt_info = {
	.identity	= "nt72668 watchdog",
	.options	= WDIOF_SETTIMEOUT,
};

/*
 * Handle commands from user-space.
 */
static long nt72668_wdt_ioctl(struct file *file,
					unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int new_value;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &nt72668_wdt_info,
				sizeof(nt72668_wdt_info)) ? -EFAULT : 0;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);

	case WDIOC_SETOPTIONS:
		if (get_user(new_value, p))
			return -EFAULT;
		if (new_value & WDIOS_DISABLECARD)
			nt72668_wdt_stop();

		if (new_value & WDIOS_ENABLECARD)
			nt72668_wdt_start();
		return 0;

	case WDIOC_KEEPALIVE:
		nt72668_wdt_reload();	/* pat the watchdog */
		return 0;

	case WDIOC_SETTIMEOUT:
		if (get_user(new_value, p))
			return -EFAULT;
		if (nt72668_wdt_settimeout(new_value))
			return -EINVAL;
		/* Enable new time value */
		nt72668_wdt_start();
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
static ssize_t nt72668_wdt_write(struct file *file, const char *data,
						size_t len, loff_t *ppos)
{
	nt72668_wdt_reload();		/* pat the watchdog */
	return len;
}

/* ......................................................................... */

static const struct file_operations nt72668wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.unlocked_ioctl	= nt72668_wdt_ioctl,
	.open		= nt72668_wdt_open,
	.release	= nt72668_wdt_close,
	.write		= nt72668_wdt_write,
};

static struct miscdevice nt72668wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &nt72668wdt_fops,
};

static void nt72668_wdt_init(void)
{
	char *wdog_str;
	char *tmp_str;
	char buf[64];
	unsigned short sec;

	wdog_str = strstr(boot_command_line, "wdog");

	if (wdog_str != NULL) {
		memset(buf, 0, sizeof(buf));
		tmp_str = strchr(wdog_str, ' ');
		memcpy(buf, wdog_str, tmp_str - wdog_str);
		tmp_str = strchr(buf, '=');
		tmp_str++;

		sec = simple_strtoul(tmp_str, NULL, 10);
		printk("wdog timeout = %d sec\n", sec);
		wdt_time = sec;
	} else {
		wdt_time = 0;
		printk("wdog disabled !\n");
	}

	/*
	 * Check that the heartbeat value is within range;
	 * if not reset to the default
	 */
	if (nt72668_wdt_settimeout(wdt_time)) {
		nt72668_wdt_settimeout(NT72668_WDT_DEFAULT_TIME);
		pr_info("nt72668_wdt: wdt_time value must be %d <= wdt_time <= %d, using %d\n", NT72668_WDT_MIN_TIME, NT72668_WDT_MAX_TIME, wdt_time);
	}

	if (wdt_time > 0)
		nt72668_wdt_start();
}

static int nt72668wdt_probe(struct platform_device *pdev)
{
	int res;

	if (nt72668wdt_miscdev.parent)
		return -EBUSY;
	nt72668wdt_miscdev.parent = &pdev->dev;

	res = misc_register(&nt72668wdt_miscdev);
	if (res) {
		printk("misc_register fail, ret = %d\n",res);
		return res;
	}

	if(wdt_time > 0)
		printk(KERN_INFO "NT72668 Watchdog Timer enabled (%d seconds)\n",
				wdt_time);

	nt72668_wdt_init();

	return 0;
}

static int nt72668wdt_remove(struct platform_device *pdev)
{
	int res;

	res = misc_deregister(&nt72668wdt_miscdev);
	if (!res)
		nt72668wdt_miscdev.parent = NULL;

	return res;
}

static void nt72668wdt_shutdown(struct platform_device *pdev)
{
	nt72668_wdt_stop();
}

#ifdef CONFIG_PM

static int nt72668wdt_suspend(struct platform_device *pdev, pm_message_t message)
{
	nt72668_wdt_stop();
	return 0;
}

static int nt72668wdt_resume(struct platform_device *pdev)
{
	if (nt72668wdt_busy)
		nt72668_wdt_start();
	return 0;
}

#else
#define nt72668wdt_suspend NULL
#define nt72668wdt_resume	NULL
#endif

static const struct of_device_id nt72668_wdt_of_match[] = {
	{ .compatible = "novatek,nvt72668-wdt", },
	{ /* DO NOT REMOVE */ },
};

MODULE_DEVICE_TABLE(of, nt72668_wdt_of_match);

static struct platform_driver nt72668wdt_driver = {
	.probe		= nt72668wdt_probe,
	.remove		= nt72668wdt_remove,
	.shutdown	= nt72668wdt_shutdown,
	.suspend	= nt72668wdt_suspend,
	.resume		= nt72668wdt_resume,
	.driver		= {
		.name	= "nt72668_wdt",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(nt72668_wdt_of_match),
	},
};

module_platform_driver(nt72668wdt_driver);

MODULE_AUTHOR("Alvin lin @ Novatek");
MODULE_DESCRIPTION("Watchdog driver for Novatek NT72668");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
MODULE_ALIAS("platform:nt72668_wdt");

