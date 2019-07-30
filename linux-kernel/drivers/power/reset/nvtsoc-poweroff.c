/*
 *  nvtsoc-poweroff.c
 *
 *  Author:	Alvin Lin
 *  Created:	Nov 23, 2016
 *  Copyright:	Novatek Inc.
 *
 *
 */

#include <linux/delay.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <linux/stat.h>
#include "../../soc/nvt/include/nvt_stbc.h"

static struct device *nvtsoc_power_off_device;

static void nvtsoc_power_off(void)
{
	nvt_soc_stbc_cmd(KER_STBC_IPC_CMD_POWER_OFF);
	while (1)
		;
}

static struct device *nvtsoc_restart_device;

static int nvtsoc_restart(struct notifier_block *this, unsigned long mode,
			     void *cmd)
{
	nvt_soc_stbc_cmd(KER_STBC_IPC_CMD_RESET);
	while (1)
		;

	return NOTIFY_DONE;
}

static struct notifier_block nvtsoc_restart_nb = {
	.notifier_call = nvtsoc_restart,
	.priority = 128,
};

static ssize_t nvtsoc_reset_active_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", nvtsoc_restart_device == dev);
}

static ssize_t nvtsoc_reset_active_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	long value;
	int err = kstrtol(buf, 0, &value);

	if (!err && value)
		nvtsoc_restart_device = dev;

	return err ? err : count;
}

DEVICE_ATTR(active, S_IRUGO | S_IWUSR, nvtsoc_reset_active_show,
		nvtsoc_reset_active_store);


enum nvtsoc_reset_func { FUNC_RESET, FUNC_SHUTDOWN, FUNC_REBOOT };

static const struct of_device_id nvtsoc_reset_of_match[] = {
	{
		.compatible = "nvt,nvtsoc-reset",
		.data = (void *)FUNC_RESET,
	}, {
		.compatible = "nvt,nvtsoc-shutdown",
		.data = (void *)FUNC_SHUTDOWN
	}, {
		.compatible = "nvt,nvtsoc-reboot",
		.data = (void *)FUNC_REBOOT
	},
	{}
};

static int _nvtsoc_register_restart_handler(struct device *dev)
{
	int err;

	nvtsoc_restart_device = dev;
	err = register_restart_handler(&nvtsoc_restart_nb);
	if (err) {
		dev_err(dev, "cannot register restart handler (err=%d)\n", err);
		return err;
	}
	device_create_file(dev, &dev_attr_active);

	return 0;
}

static int nvtsoc_reset_probe(struct platform_device *pdev)
{
	const struct of_device_id *match =
			of_match_device(nvtsoc_reset_of_match, &pdev->dev);
	int ret = 0;

	if (!match)
		return -EINVAL;

	switch ((enum nvtsoc_reset_func)match->data) {
	case FUNC_SHUTDOWN:
		nvtsoc_power_off_device = &pdev->dev;
		pm_power_off = nvtsoc_power_off;
		break;
	case FUNC_RESET:
		if (!nvtsoc_restart_device)
			ret = _nvtsoc_register_restart_handler(&pdev->dev);
		break;
	case FUNC_REBOOT:
		ret = _nvtsoc_register_restart_handler(&pdev->dev);
		break;
	};

	return ret;
}

static struct platform_device nvtsoc_reset_dev = {
	.name = "nvtsoc-reset",
	.id   = 0,
	.num_resources = 0,
};

static struct platform_driver nvtsoc_reset_driver = {
	.probe = nvtsoc_reset_probe,
	.driver = {
		.name = "nvtsoc-reset",
		.of_match_table = nvtsoc_reset_of_match,
	},
};

static int __init nvtsoc_reset_init(void)
{
	int ret = 0;

	ret = platform_device_register(&nvtsoc_reset_dev);
	if (ret) {
		pr_err("%s platform_device_register ret %d\n", __func__, ret);
		goto out;
	}

	ret = platform_driver_register(&nvtsoc_reset_driver);
	if (ret) {
		pr_err("%s platform_driver_register ret %d\n", __func__, ret);
		goto  device_unregister;
	}

	return ret;

device_unregister:
	platform_device_unregister(&nvtsoc_reset_dev);

out:
	return ret;
}
device_initcall(nvtsoc_reset_init);
