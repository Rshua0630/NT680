/*
 *  nvt_stbc.c
 *
 *  Author:	Z Y Chai
 *  Created:	Oct 10, 2013
 *  Copyright:	Novatek Inc.
 *
 *  Refine:
 *  Alvin Lin, modify for dts driver style
 *  Nov 16, 2016
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sysctl.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_device.h>
#include <linux/atomic.h>
#include <nvt_stbc_regs.h>
#include <nvt_stbc.h>

static struct nvt_stbc_hwplat stbc_hwplat;
static struct ctl_table_header *sysctls_root_table;
static atomic_t nvt_stbc_lock_spi = ATOMIC_INIT(0);

static int user_atoi(char __user *ubuf, size_t len)
{
	char buf[16];
	unsigned long res;
	int ret;

	if (len > 15)
		return -EINVAL;

	if (copy_from_user(buf, ubuf, len))
		return -EFAULT;

	buf[len] = 0;
	ret = kstrtoul(buf, 10, &res);
	if (res > INT_MAX || ret != 0)
		return -ERANGE;
	return res;
}


static int nvt_stbc_lock_spi_procctl(struct ctl_table *ctl, int write,
			 void __user *buffer, size_t *lenp, loff_t *fpos)
{
	int lock_stat;

	if (!write)
		return proc_dointvec(ctl, write, buffer, lenp, fpos);

	lock_stat = user_atoi(buffer, *lenp);

	if (lock_stat != 0) {
		nvt_stbc_stay_in_sram();
		atomic_set(&nvt_stbc_lock_spi, 1);
	} else {
		atomic_set(&nvt_stbc_lock_spi, 0);
		nvt_stbc_exit_idle();
	}

	return *lenp;

}


static struct ctl_table nvt_stbc_sysctls[] = {
	{
		.procname	= "nvt_stbc_lock_spi",
		/* Data pointer and size. */
		.data		= &nvt_stbc_lock_spi,
		.maxlen		= sizeof(nvt_stbc_lock_spi),
		.mode		= 0644,	/* Mode, proc handler. */
		.proc_handler	= nvt_stbc_lock_spi_procctl
	},
	{}
};

static struct ctl_table sysctls_root[] = {
	{
		.procname	= "nvt",
		.mode		= 0555,
		.child		= nvt_stbc_sysctls
	},
	{}
};


static int nvt_soc_stbc_in_eburn_mode(void)
{
	int ret = 0;
	u32 reg_val;

	reg_val = readl(stbc_hwplat.stbc_bootstrap_reg);

	if (reg_val == 0xA0 || reg_val == 0xB0 || reg_val == 0xC0)
		ret = 1;

	return ret;

}
static int nvt_soc_stbc_wait_response(void)
{
	int cnt = 0;

	for (cnt = 0; cnt < 10; cnt++) {
		if(readl(stbc_hwplat.stbc_int_stat) & STBC_IPC_UPDATE_INT_STATUS)
			return 1;
		
		msleep(50);
	}
	return 0;
}

static void nvt_soc_stbc_clear_intr(void)
{
	writel((readl(stbc_hwplat.stbc_int_stat) |
	STBC_IPC_UPDATE_INT_STATUS), stbc_hwplat.stbc_int_stat);
}

void nvt_soc_stbc_unlock_reg(void)
{
	if(!(readl(stbc_hwplat.stbc_keypass) & STBC_KEY_PASS_EN)) {
		writel(STBC_PATT_START, stbc_hwplat.stbc_pattern);
		writel(STBC_PATT_END, stbc_hwplat.stbc_pattern);
	}
}

int  nvt_soc_stbc_cmd(int cmd)
{
	u32 reg_val;
	nvt_soc_stbc_unlock_reg();

	writel(cmd << STBC_CMD_SHIFT, stbc_hwplat.stbc_download);

	if (nvt_soc_stbc_wait_response() == 0)
		printk("%s wait stbc response fail !\n",__func__);
	
	reg_val = readl(stbc_hwplat.stbc_upload);

	return reg_val;

}

int nvt_stbc_stay_in_sram(void)
{
	int ret;
	int retry = 0;
	int resp_val = (KER_STBC_IPC_CMD_ACK_CMD << STBC_CMD_SHIFT) |
		(KER_STBC_IPC_CMD_SYS_STBC_IDLE_IN_SRAM << STBC_DATA_SHIFT);

	if (nvt_soc_stbc_in_eburn_mode())
		return 0;

	if (atomic_read(&nvt_stbc_lock_spi))
		return 0;

	do {
		ret =  nvt_soc_stbc_cmd(KER_STBC_IPC_CMD_SYS_STBC_IDLE_IN_SRAM);
		retry++;
		if (retry > 30)
			break;
	} while (ret != resp_val);

	return ret;
}

void nvt_stbc_exit_idle(void)
{
	if (atomic_read(&nvt_stbc_lock_spi))
		return;

	nvt_soc_stbc_unlock_reg();
	writel((KER_STBC_IPC_CMD_SYS_STBC_EXIT_IDLE
	<< STBC_CMD_SHIFT), stbc_hwplat.stbc_download);
}

void nvt_stbc_reset(void)
{
	nvt_soc_stbc_unlock_reg();
	writel((KER_STBC_IPC_CMD_RESET_STBC
	<< STBC_CMD_SHIFT), stbc_hwplat.stbc_download);
}


#define WAIT (10000)
static int checkbit(unsigned long addr,
		unsigned long mask, unsigned long value)
{
	int i = 0;

	for (i = 0 ; i < WAIT ; i++) {
		if ((addr & mask) == value)
			return 0;
	}
	msleep(50);

	return -1;
}

u8 nvt_stbc_read_reg(unsigned short reg)
{
	u8 reg_val = 0;
	int i, ipc_cmd, ipc_ack, retry_cnt = 500;

	nvt_soc_stbc_unlock_reg();

	ipc_cmd = KER_STBC_IPC_CMD_READ_REGISTER << STBC_CMD_SHIFT |
		reg << STBC_DATA1_SHIFT;
	ipc_ack = KER_STBC_IPC_CMD_ACK_READ_REG;
	writel(ipc_cmd, stbc_hwplat.stbc_download);

	if (nvt_soc_stbc_wait_response() == 0)
		pr_err("%s wait stbc response fail !\n", __func__);

	for (i = 0; i < retry_cnt; i++) {
		if (!checkbit(readl(stbc_hwplat.stbc_upload),
					0xff000000, ipc_ack))
			break;
	}

	if (i == retry_cnt)
		pr_err("%s  0x%x fail !\n", __func__, reg);
	else
		reg_val = (u8)(readl(stbc_hwplat.stbc_upload)
			>> STBC_DATA_SHIFT);

	nvt_soc_stbc_clear_intr();
	return reg_val;
}

int nvt_stbc_write_reg(u8 reg_val, unsigned short reg)
{
	int i, ipc_cmd, ipc_ack, retry_cnt = 500;

	nvt_soc_stbc_unlock_reg();

	ipc_cmd = KER_STBC_IPC_CMD_WRITE_REGISTER << STBC_CMD_SHIFT |
		reg << STBC_DATA1_SHIFT | reg_val;

	ipc_ack = KER_STBC_IPC_CMD_ACK_WRITE_REG;

	writel(ipc_cmd, stbc_hwplat.stbc_download);

	if (nvt_soc_stbc_wait_response() == 0)
		pr_err("%s wait stbc response fail !\n", __func__);

	for (i = 0; i < retry_cnt; i++) {
		if (!checkbit(readl(
			stbc_hwplat.stbc_upload), 0xffff0000, ipc_ack))
			break;
	}

	if (i == retry_cnt)
		pr_info("%s  0x%x fail !\n", __func__, reg);
	nvt_soc_stbc_clear_intr();

	return 0;
}

#define LEAPYEAR(year) (!((year) % 4) && (((year) % 100) || !((year) % 400)))

static int nvt_soc_os_gmtime(u32 total_sec, struct rtc_time *tm)
{
	int ret = 0;
	u32 daysecond;
	u32 daycount;
	u32 year = 1970;
	u32 DayOfMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	u32 SECS_DAY = 24 * 60 * 60;

	if(tm == NULL) {
		ret = -EINVAL;
		printk("%s tm is NULL !\n",__func__);
		goto out;
	}

	daysecond = total_sec % SECS_DAY;
	daycount = total_sec / SECS_DAY;

	tm->tm_sec = daysecond % 60;
	daysecond /= 60;
	tm->tm_min = daysecond % 60;
	tm->tm_hour = daysecond / 60;
	tm->tm_wday = (daycount + 4) % 7;//day count 0 is Thursday

	while(daycount >= (LEAPYEAR(year) ? 366 : 365)){
		daycount -= (LEAPYEAR(year) ? 366 : 365);
		year++;
	}

	tm->tm_year = year - 1900;
	tm->tm_yday = daycount;
	tm->tm_mon = 0;
	DayOfMonth[1] += LEAPYEAR(year) ? 1 : 0;

	while(daycount >= DayOfMonth[tm->tm_mon]) {
		daycount -= DayOfMonth[tm->tm_mon];
		tm->tm_mon++;
	}

	tm->tm_mday = daycount + 1;
	tm->tm_isdst = 0;

out:
	return ret;
}

static int nvt_soc_os_tm_OutOfRange(struct rtc_time *tm)
{
	if(tm->tm_year < 70 || tm->tm_year > 206)
		return 1;

	if(tm->tm_year == 206) {
		if(tm->tm_mon > 1)
			return 1;
		else if(tm->tm_mon == 1) {
			if(tm->tm_mday > 7) {
				return 1;
			} else if (tm->tm_mday == 7) {
				if(tm->tm_hour > 6)
					return 1;
				else if (tm->tm_hour == 6) {
					if(tm->tm_min > 28 || (tm->tm_min == 28 && tm->tm_sec >15))
						return 1;
				}
			}
		}
	}

	return 0;
}

static int nvt_soc_os_timegm(u32 *total_sec, struct rtc_time *tm)
{
	int ret = 0;
	u32 year, yeardiff;
	u32 month = 0;
	u32 daycount = 0;
	u32 DayOfMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if(total_sec == NULL || tm == NULL) {
		printk("%s invaild parameters !\n",__func__);
		ret = -EINVAL;
		goto out;
	}

	if (nvt_soc_os_tm_OutOfRange(tm)) {
		pr_err("%s tm struct out of range !\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	year = tm->tm_year + 1900;
	yeardiff = year - 1970;
	daycount = yeardiff * 365;
	/*1972 is the first leap year since 1970*/
	daycount += ((yeardiff + 1)/4);	daycount -= ((yeardiff + 69)/100);
	daycount += ((yeardiff + 369)/400);

	DayOfMonth[1] += LEAPYEAR(year) ? 1 : 0;
	while (month < tm->tm_mon) {
		daycount += DayOfMonth[month];
		++month;
	}

	daycount += (tm->tm_mday - 1);

	*total_sec = (((daycount * 24 + tm->tm_hour) * 60) + tm->tm_min) * 60
		+ tm->tm_sec;

out:
	return ret;
}

int nvt_soc_stbc_get_time(struct rtc_time *tm)
{
	u32 reg_val;
	u32 total_seconds;
	int ret = 0;

	if(tm == NULL) {
		ret = -EINVAL;
		pr_err("%s tm is NULL !\n", __func__);
		goto out;
	}

	reg_val = readl(stbc_hwplat.stbc_rtc_base_reg);

	total_seconds = ((readl(stbc_hwplat.stbc_rtc_sec_reg) >> STBC_RTC_SEC_SFT)  & STBC_RTC_SEC_MSK);

	total_seconds += (((readl(stbc_hwplat.stbc_rtc_sec_reg) >> STBC_RTC_MIN_SFT) & STBC_RTC_MIN_MSK) * 60);

	total_seconds += (((readl(stbc_hwplat.stbc_rtc_sec_reg) >> STBC_RTC_HOUR_SFT) & STBC_RTC_HOUR_MSK) * 3600);

	total_seconds += readl(stbc_hwplat.stbc_rtc_day_reg) * 86400;

	total_seconds += reg_val;


	if (nvt_soc_os_gmtime(total_seconds, tm)) {
		pr_err("%s second to date convert fail !\n", __func__);
		ret = -EINVAL;
		goto out;
	}


	//kwinyee debug
#if 0	
	printk("year = %d, mon = %d, yday = %d, hour = %d, min = %d, sec = %d\n", 
			tm->tm_year, tm->tm_mon, tm->tm_yday, tm->tm_hour, tm->tm_min, 
			tm->tm_sec);
#endif	

out:
	return ret;
}
EXPORT_SYMBOL(nvt_soc_stbc_get_time);

int nvt_soc_stbc_set_time(struct rtc_time *tm)
{
	int ret = 0;
	u32 total_sec;

	if(tm == NULL) {
		pr_err("%s invalid tm strunct !\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	if (nvt_soc_os_timegm(&total_sec, tm)) {
		pr_err("%s convert second fail !\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	nvt_soc_stbc_unlock_reg();

	writel((readl(stbc_hwplat.stbc_rtc_switch_reg) |
	(1 << STBC_RTC_SWITCH_ARM_SFT)), stbc_hwplat.stbc_rtc_switch_reg);

	nvt_soc_stbc_unlock_reg();

	writel(total_sec, stbc_hwplat.stbc_rtc_base_reg);

	nvt_soc_stbc_unlock_reg();

	writel(0, stbc_hwplat.stbc_rtc_ctl_reg);

	nvt_soc_stbc_unlock_reg();

	writel((readl(stbc_hwplat.stbc_rtc_ctl_reg) | STBC_RTC_RESET)
		, stbc_hwplat.stbc_rtc_ctl_reg);

out:
	return ret;
}
EXPORT_SYMBOL(nvt_soc_stbc_set_time);


void nvt_soc_stbc_enable_wdog(unsigned short sec)
{
	unsigned short msec = sec * 1000;
	u32 reg_val;

	nvt_soc_stbc_unlock_reg();

	writel(((KER_STBC_IPC_CMD_ENABLE_WATCHDOG_TIMER << STBC_CMD_SHIFT)
	| (msec << STBC_WDOG_TIME_SHIFT)), stbc_hwplat.stbc_download);

	if (nvt_soc_stbc_wait_response() == 0) {
		//printk("%s wait stbc response fail !\n",__func__);
		reg_val = readl(stbc_hwplat.stbc_upload);
		return;
	}

	reg_val = readl(stbc_hwplat.stbc_upload);
}

void nvt_soc_stbc_kick_wdog(void)
{
	u32 reg_val;
	
	nvt_soc_stbc_unlock_reg();
	
	writel((KER_STBC_IPC_CMD_RESET_WATCHDOG << STBC_CMD_SHIFT)
	, stbc_hwplat.stbc_download);

	if (nvt_soc_stbc_wait_response() == 0) {
		//printk("%s wait stbc response fail !\n",__func__);
		reg_val = readl(stbc_hwplat.stbc_upload);
		return;
	}
	
	reg_val = readl(stbc_hwplat.stbc_upload);
}

static const struct of_device_id nvt_stbc_of_match[] = {
	{ .compatible = "novatek,nvt-stbc", },
	{ /* DO NOT REMOVE */ },
};


static int nvt_stbc_probe(struct platform_device *pdev)
{
	const struct of_device_id *of_id;
	int ret = 0;
	struct resource *r;
	struct device_node *np = pdev->dev.of_node;
	u32 offset;

	of_id = of_match_device(nvt_stbc_of_match, &pdev->dev);

	if (!of_id) {
		ret = -EINVAL;
		goto out;
	}

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!r) {
		pr_err("%s get io resource fail !\n", __func__);
		ret = -ENXIO;
		goto out;
	}

	stbc_hwplat.stbc_vbase = ioremap_nocache(r->start, resource_size(r));
	if (!stbc_hwplat.stbc_vbase) {
		pr_err("%s remap stbc memory fail!\n", __func__);
		ret = -ENOMEM;
		goto out;
	}


	of_property_read_u32(np, "download_reg_ofs", &offset);
	stbc_hwplat.stbc_download = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "upload_reg_ofs", &offset);
	stbc_hwplat.stbc_upload = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "int_stat_reg_ofs", &offset);
	stbc_hwplat.stbc_int_stat = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "stbc_patt_reg_ofs", &offset);
	stbc_hwplat.stbc_pattern = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "stbc_key_pass_reg_ofs", &offset);
	stbc_hwplat.stbc_keypass = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "rtc_base_reg_ofs", &offset);
	stbc_hwplat.stbc_rtc_base_reg = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "rtc_sec_reg_ofs", &offset);
	stbc_hwplat.stbc_rtc_sec_reg = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "rtc_day_reg_ofs", &offset);
	stbc_hwplat.stbc_rtc_day_reg = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "rtc_ctrl_reg_ofs", &offset);
	stbc_hwplat.stbc_rtc_ctl_reg = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "rtc_switch_reg_ofs", &offset);
	stbc_hwplat.stbc_rtc_switch_reg = stbc_hwplat.stbc_vbase + offset;

	of_property_read_u32(np, "bootstrap_reg_ofs", &offset);
	stbc_hwplat.stbc_bootstrap_reg = stbc_hwplat.stbc_vbase + offset;

	sysctls_root_table = register_sysctl_table(sysctls_root);

out:
	return ret;

}

static int nvt_stbc_remove(struct platform_device *pdev)
{
	struct resource *r;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (r)
		iounmap(stbc_hwplat.stbc_vbase);

	if (sysctls_root_table)
		unregister_sysctl_table(sysctls_root_table);
	return 0;
}

static struct platform_device nvt_stbc_dev = {
	.name = "nvt-stbc",
	.id   = 0,
	.num_resources = 0,
};

static struct platform_driver nvt_stbc_driver = {
	.driver = {
		.name = "nvt-stbc",
		.of_match_table = of_match_ptr(nvt_stbc_of_match),
	},
	.probe = nvt_stbc_probe,
	.remove = nvt_stbc_remove,
};

static int __init nvt_stbc_init(void)
{
	int ret = 0;

	ret = platform_device_register(&nvt_stbc_dev);
	if (ret) {
		pr_err("%s platform_device_register ret %d\n", __func__, ret);
		goto out;
	}

	ret = platform_driver_register(&nvt_stbc_driver);
	if (ret) {
		pr_err("%s platform_driver_register ret %d\n", __func__, ret);
		goto  device_unregister;
	}

	return ret;

device_unregister:
	platform_device_unregister(&nvt_stbc_dev);

out:
	return ret;
}

postcore_initcall(nvt_stbc_init);
