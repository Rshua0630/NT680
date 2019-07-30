/*
 *  driver/rtc/rtc-nvt668.c
 *
 *  Author:	Alvin lin
 *  Created:	Dec 20, 2012
 *  Copyright:	Novatek Inc.
 *
 */


#include <linux/module.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <linux/bcd.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/of.h>
#include "../soc/nvt/include/nvt_stbc.h"

#define DRV_VERSION		"1.0"

struct nvt_soc_rtc_priv {
	struct rtc_device *rtc;
};

/*add by Y_S WU 2014.5.22  CTS default time*/
static struct rtc_time default_tm = {
	.tm_year = (2008 - 1900), /*year 2014*/
	.tm_mon = (12 - 1),       /*month 10*/
	.tm_mday = 1,             /*day 1*/
	.tm_hour = 12,
	.tm_min = 0,
	.tm_sec = 0
};



#ifdef CONFIG_RTC_INTF_DEV

static int nvt_soc_rtc_ioctl(struct device *dev,
		unsigned int cmd, unsigned long arg)
{

	switch (cmd) {
		case RTC_AIE_ON:
			pr_info("RTC_AIE_ON\n");
			break;
		case RTC_AIE_OFF:
			pr_info("RTC_AIE_OFF\n");
			break;
		case RTC_UIE_ON:
			pr_info("RTC_UIE_ON\n");
			break;
		case RTC_UIE_OFF:
			pr_info("RTC_UIE_OFF\n");
			break;
		case RTC_WIE_ON:
			pr_info("RTC_WIE_ON\n");
			break;
		case RTC_WIE_OFF:
			pr_info("RTC_WIE_OFF\n");
			break;
		case RTC_ALM_SET:
			pr_info("RTC_ALM_SET\n");
			break;
		case RTC_ALM_READ:
			pr_info("RTC_ALM_READ\n");
			break;
		case RTC_RD_TIME:
			pr_info("RTC_RD_TIME\n");
			break;
		case RTC_SET_TIME:
			pr_info("RTC_SET_TIME\n");
			break;
		case RTC_IRQP_READ:
			pr_info("RTC_IRQP_READ\n");
			break;
		case RTC_IRQP_SET:
			pr_info("RTC_IRQP_SET\n");
			break;
		case RTC_EPOCH_READ:
			pr_info("RTC_EPOCH_READ\n");
			break;
		case RTC_EPOCH_SET:
			pr_info("RTC_EPOCH_SET\n");
			break;
		case RTC_WKALM_SET:
			pr_info("RTC_WKALM_SET\n");
			break;
		case RTC_WKALM_RD:
			pr_info("RTC_WKALM_RD\n");
			break;
		case RTC_PLL_SET:
			pr_info("RTC_PLL_SET\n");
			break;
		case RTC_PLL_GET:
			pr_info("RTC_PLL_GET\n");
			break;
		default:
			pr_info("unknown rtc ioctl :0X%X\n", cmd);
	}
	return 0;
}

#else
#define nvt_soc_rtc_ioctl    NULL
#endif

#ifdef CONFIG_PROC_FS

static int nvt_soc_rtc_proc(struct device *dev, struct seq_file *seq)
{
	return 0;
}

#else
#define nvt_soc_rtc_proc     NULL
#endif

static int nvt_soc_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	int ret;

	ret = nvt_soc_stbc_get_time(tm);

	if(ret < 0)
		goto out;
#if 0
	//kwinyee debug
	printk("read time: sec = %d, min = %d, hour = %d, mday = %d,"	\
	"mon = %d, year = %d, wday = %d, yday = %d," \
	"\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday, \
	tm->tm_mon,tm->tm_year,tm->tm_wday,tm->tm_yday);
#endif


	ret = rtc_valid_tm(tm);

out:
	return ret;
}

static int nvt_soc_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	int ret;

#if 0
	//kwinyee debug
	printk("set time: sec = %d, min = %d, hour = %d, mday = %d,"  \
	"mon = %d, year = %d, wday = %d, yday = %d," \
	"\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday, \
	tm->tm_mon,tm->tm_year,tm->tm_wday,tm->tm_yday);
#endif
	ret = nvt_soc_stbc_set_time(tm);

	return ret;
}

static int nvt_soc_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	return 0;
}

static int nvt_soc_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	return 0;
}

static const struct rtc_class_ops nvt_soc_rtc_ops = {
	.ioctl		= nvt_soc_rtc_ioctl,
	.proc		= nvt_soc_rtc_proc,
	.read_time	= nvt_soc_rtc_read_time,
	.set_time	= nvt_soc_rtc_set_time,
	.read_alarm	= nvt_soc_rtc_read_alarm,
	.set_alarm	= nvt_soc_rtc_set_alarm,
};

static int nvt_soc_rtc_probe(struct platform_device *pdev)
{
	struct rtc_device *rtc;
	struct nvt_soc_rtc_priv *priv;
	int ret = 0;
	priv = kzalloc(sizeof(struct nvt_soc_rtc_priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	platform_set_drvdata(pdev, priv);
	rtc = rtc_device_register("nvt_soc", &pdev->dev,
				  &nvt_soc_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc)) {
		ret = PTR_ERR(rtc);
		goto out;
	}
	priv->rtc = rtc;
#if 0  //set default system time  --add by y s wu
	nvt_soc_rtc_set_time(NULL, &default_tm);
#endif
	return 0;

out:
	if (priv->rtc)
		rtc_device_unregister(priv->rtc);
	kfree(priv);
	return ret;
}

static int nvt_soc_rtc_remove(struct platform_device *pdev)
{
	struct nvt_soc_rtc_priv *priv = platform_get_drvdata(pdev);

	rtc_device_unregister(priv->rtc);
	kfree(priv);
	return 0;
}

static const struct of_device_id nvt_soc_rtc_of_match[] = {
	{ .compatible = "novatek,nvtsoc-rtc", },
	{ /* DO NOT REMOVE */ },
};

MODULE_DEVICE_TABLE(of, nvt_soc_rtc_of_match);

static struct platform_device nvt_soc_rtc_platform_dev = {
	.name = "nvt_rtc",
	.id   = 0,
	.num_resources = 0,
};

static struct platform_driver nvt_soc_rtc_platform_driver = {
	.driver		= {
		.name	= "nvt_rtc",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(nvt_soc_rtc_of_match),
	},
	.probe		= nvt_soc_rtc_probe,
	.remove		= nvt_soc_rtc_remove,
};

static int __init nvt_soc_rtc_init(void)
{
	int ret = 0;

	ret = platform_device_register(&nvt_soc_rtc_platform_dev);
	if (ret) {
		pr_err("%s platform_device_register ret %d\n", __func__, ret);
		goto out;
	}
	ret = platform_driver_register(&nvt_soc_rtc_platform_driver);
	if (ret) {
		pr_err("%s platform_driver_register ret %d\n", __func__, ret);
		goto device_unregister;
	}
	return ret;

device_unregister:
	platform_device_unregister(&nvt_soc_rtc_platform_dev);
out:
	return ret;
}

static void __exit nvt_soc_rtc_exit(void)
{
	platform_driver_unregister(&nvt_soc_rtc_platform_driver);
	platform_device_unregister(&nvt_soc_rtc_platform_dev);
}

module_init(nvt_soc_rtc_init);
module_exit(nvt_soc_rtc_exit);

MODULE_AUTHOR("Novatek");
MODULE_DESCRIPTION("Novatek SOC RTC driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS("platform:rtc-nvt_soc");
