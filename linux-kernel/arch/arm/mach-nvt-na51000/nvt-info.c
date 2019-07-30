/**
    NVT info for the platform related tools
    This file will provide the boot stage record the timestamp function
    @file       nvt-info.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <linux/string.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/gfp.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/memblock.h>
#include <generated/nvtversion.h>

#include <mach/nvt-bootts.h>
#include <mach/hardware.h>
#include <mach/nvt-io.h>

#define NVT_BOOTTS_MAX_LEN		15
#define NVT_BOOTTS_NAME_MAX_LEN		15
#define LOADER_TIME_OFFSET		0

#define NVT_TIMER_TM0_CNT		0x108
#define TM0_LIST_ONLY

struct bootts {
	unsigned long long time[NVT_BOOTTS_MAX_LEN];
	char    name[NVT_BOOTTS_MAX_LEN][NVT_BOOTTS_NAME_MAX_LEN];
};

static struct bootts boot_record;
static unsigned char	index;
struct proc_dir_entry   *nvt_info_dir_root = NULL;
EXPORT_SYMBOL(nvt_info_dir_root);

void nvt_bootts_add_ts(char *name)
{
	unsigned char name_len = strlen(name);

	if (name_len < NVT_BOOTTS_NAME_MAX_LEN && index < NVT_BOOTTS_MAX_LEN) {
		if (name != NULL)
			strncpy(boot_record.name[index], name, name_len);

		boot_record.time[index] = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
		index++;
	} else {
		pr_err("\n%s %s fail\n", __func__, name);
	}
}

EXPORT_SYMBOL_GPL(nvt_bootts_add_ts);

#ifdef CONFIG_PROC_FS
/* Support for	/proc/nvt_info/bootts */
/*		/proc/nvt_info/tm0 */
/*		/proc/nvt_info/memperf */

static int nvt_bootts_proc_show(struct seq_file *m, void *v)
{
	unsigned char i = 0, j = 0;
	unsigned int flag[NVT_BOOTTS_MAX_LEN] = {0}, offset = 0;
	char *pstr = NULL, tmp[16] = {0};
	unsigned long uboot_ts = 0, uboot_te = 0;

	seq_puts(m, "============== Boot time results ==============\n");
	/* seq_printf(m, "TM0 initial value: %llu\n", boot_record.time[0]); */
	#ifndef TM0_LIST_ONLY
	seq_puts(m, "Name\t\tTime(us)\tTM0\n");
	while (i < index) {
		seq_printf(m, "%-8.8s\t%-8.8llu\t%-8.8llu\n",
				boot_record.name[i],
				(i-1 < 0) ? (boot_record.time[i] - 0) :
				(boot_record.time[i] - boot_record.time[i-1]),
				boot_record.time[i]
				);
		i++;
	}
	seq_printf(m, "%-8.8s\t%-8.8llu\t\n",
			"Total",
			(boot_record.time[index - 1] - boot_record.time[0])
			);
	#else
	memset(flag, 0, sizeof(flag));
	seq_puts(m, "Name\t\tTM0\n");
	pstr = saved_command_line;
	while (*pstr != '\0' && pstr != NULL) {

		pstr = strpbrk(pstr, " ");
		if (pstr != NULL) {
			pstr++;
			if (!strncmp(pstr, "bootts=", strlen("bootts="))) {
				pstr += strlen("bootts=");
				offset = strlen(pstr) - strlen(strpbrk(pstr, ","));
				strncpy(tmp, pstr, offset);
				kstrtoul(tmp, 10, &uboot_ts);
				pstr += offset + 1;
				offset = strlen(pstr);
				if (strpbrk(pstr, " ") != NULL)
					offset -= strlen(strpbrk(pstr, " "));

				strncpy(tmp, pstr, offset);
				kstrtoul(tmp, 10, &uboot_te);
				seq_printf(m, "Uboot\t\t%lu\nUboot\t\t%lu\n\n",
						uboot_ts + LOADER_TIME_OFFSET,
						uboot_te + LOADER_TIME_OFFSET);
				break;
			}
		} else {
			break;
		}
	}
	while (i < index) {
		if (flag[i] == 0) {
			for (j = 0; j < index; j++) {
				if ((!strcmp(boot_record.name[i], boot_record.name[j])) &&
					(i != j) &&
					flag[j] == 0) {
					seq_printf(m, "%-8.8s\t%llu\n",
							boot_record.name[i],
							boot_record.time[i] + LOADER_TIME_OFFSET
							);
					flag[i] = 1;

					seq_printf(m, "%-8.8s\t%llu\n\n",
							boot_record.name[j],
							boot_record.time[j] + LOADER_TIME_OFFSET
							);
					flag[j] = 1;
					break;
				}
			}
		}
		i++;
	}
	#endif
	seq_puts(m, "================================================\n");
	return 0;
}


static int nvt_bootts_open(struct inode *inode, struct file *file)
{
	return single_open(file, nvt_bootts_proc_show, NULL);
}

static ssize_t nvt_bootts_write(struct file *file, const char __user *buffer,
					size_t count, loff_t *pos)
{
	char *buf = (char *) __get_free_page(GFP_USER);
	int res = 0;

	if (!buf)
		return -ENOMEM;

	res = -EINVAL;
	if (count >= NVT_BOOTTS_NAME_MAX_LEN)
		goto out;

	res = -EFAULT;
	if (copy_from_user(buf, buffer, count))
		goto out;

	buf[count-1] = '\0';

	nvt_bootts_add_ts(buf);
	res = count;
out:
	free_page((unsigned long)buf);
	return res;
}
static const struct file_operations nvt_bootts_fops = {
	.open = nvt_bootts_open,
	.read = seq_read,
	.write = nvt_bootts_write,
};

static int nvt_timer_tm0_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%u\n", nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT));

	return 0;
}

static int nvt_timer_tm0_open(struct inode *inode, struct file *file)
{
	return single_open(file, nvt_timer_tm0_proc_show, NULL);
}

static const struct file_operations nvt_timer_tm0_fops = {
	.open = nvt_timer_tm0_open,
	.read = seq_read,
	.llseek = seq_lseek,
};

static unsigned long nvt_test_memcpy_perf(size_t bufsize, unsigned long iters)
{
	unsigned int i = 0;
	char *ptr_buf1 = NULL;
	char *ptr_buf2 = NULL;
	unsigned long time_before = 0;
	unsigned long time_after = 0;

	ptr_buf1 = (char*)kzalloc(bufsize, GFP_KERNEL);
	ptr_buf2 = (char*)kzalloc(bufsize, GFP_KERNEL);

	time_before = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
	for (i = 0; i < iters; ++i) {
		memcpy(ptr_buf1, ptr_buf2, bufsize);
	}
	time_after = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
	kfree(ptr_buf1);
	kfree(ptr_buf2);
	return ((bufsize * iters) / (time_after - time_before));
}

static unsigned long nvt_test_memset_perf(size_t bufsize, unsigned long iters)
{
	unsigned int i = 0;
	char *ptr_buf1 = NULL;
	unsigned long time_before = 0;
	unsigned long time_after = 0;

	ptr_buf1 = (char*)kzalloc(bufsize, GFP_KERNEL);

	time_before = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
	for (i = 0; i < iters; ++i) {
		memset(ptr_buf1, i, bufsize);
	}
	time_after = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
	kfree(ptr_buf1);
	return ((bufsize * iters) / (time_after - time_before));
}

static unsigned long nvt_test_memcmp_perf(size_t bufsize, unsigned long iters)
{
	unsigned int i = 0;
	char *ptr_buf1 = NULL;
	char *ptr_buf2 = NULL;
	unsigned long time_before = 0;
	unsigned long time_after = 0;
	int ret = 0;
	
	ptr_buf1 = (char*)kzalloc(bufsize, GFP_KERNEL);
	ptr_buf2 = (char*)kzalloc(bufsize, GFP_KERNEL);
	memset(ptr_buf1, 0x55aa55aa, bufsize);
	memset(ptr_buf2, 0x55aa55aa, bufsize);

	time_before = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
	for (i = 0; i < iters; ++i) {
		ret = memcmp(ptr_buf1, ptr_buf2, bufsize);
		if (ret != 0)
			break;
	}
	time_after = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
	if (ret == 0)
		pr_info("Compare result: Same\n");
	else
		pr_info("Compare result: Different\n");

	kfree(ptr_buf1);
	kfree(ptr_buf2);
	return ((bufsize * iters) / (time_after - time_before));
}

static int nvt_memperf_proc_show(struct seq_file *m, void *v)
{
	seq_puts(m, "Memory performance testing results\n");
	seq_printf(m, "memcpy: %lu MB/Sec.\n", nvt_test_memcpy_perf(2000000, 1000));
	seq_printf(m, "memset: %lu MB/Sec.\n", nvt_test_memset_perf(2000000, 1000));
	seq_printf(m, "memcmp: %lu MB/Sec.\n", nvt_test_memcmp_perf(2000000, 1000));

	return 0;
}

static int nvt_memperf_open(struct inode *inode, struct file *file)
{
	return single_open(file, nvt_memperf_proc_show, NULL);
}

static const struct file_operations nvt_memperf_fops = {
	.open = nvt_memperf_open,
	.read = seq_read,
	.llseek = seq_lseek,
};

static ssize_t nvt_memhotplug_write(struct file *file, const char __user *buffer,
					size_t count, loff_t *pos)
{
	char *buf = (char *) __get_free_page(GFP_USER);
	char* const delim = "@";
	char *token = NULL;
	unsigned long mem_address = 0;
	unsigned long mem_size = 0;
	int res = 0;

	if (!buf)
		return -ENOMEM;

	res = -EFAULT;
	if (copy_from_user(buf, buffer, count))
		goto out;

	res = -EFAULT;
	while ((token = strsep(&buf, delim)) != NULL) {
		if (mem_size == 0) {
			if (kstrtoul(token, 16, &mem_size) < 0)
				goto out;
		} else {
			if (kstrtoul(token, 16, &mem_address) < 0)
				goto out;
			break;
		}
	}

	res = memblock_add(mem_address, mem_size);
	if (res != 0) {
		pr_info("Got failures during add memory region 0x%08lx@0x%08lx\n", mem_size, mem_address);
		res = -1;
	} else {
		pr_info("Add memory region 0x%08lx@0x%08lx\n", mem_size, mem_address);
		res = count;
	}

out:
	free_page((unsigned long)buf);
	return res;
}

static int nvt_memhotplug_proc_show(struct seq_file *m, void *v)
{
	seq_puts(m, "Memory hotplug usage\n");
	seq_puts(m, "\tAdd memory region:\n");
	seq_puts(m, "\t$ echo Phys_size@Phys_addr > /proc/nvt_info/memhotplug\n");
	seq_puts(m, "\tPrint memory region:\n");
	seq_puts(m, "\t$ cat /sys/kernel/debug/memblock/memory\n");

	return 0;
}

static int nvt_memhotplug_open(struct inode *inode, struct file *file)
{
	return single_open(file, nvt_memhotplug_proc_show, NULL);
}

static const struct file_operations nvt_memhotplug_fops = {
	.open = nvt_memhotplug_open,
	.read = seq_read,
	.write = nvt_memhotplug_write,
};

static int nvt_version_show(struct seq_file *m, void *v)
{
	seq_printf(m, "Version: %s \n", NVT_UTS_RELEASE);
	return 0;
}

static int nvt_version_open(struct inode *inode, struct file *file)
{
	return single_open(file, nvt_version_show, NULL);
}

static const struct file_operations nvt_version_fops = {
	.open = nvt_version_open,
	.read = seq_read,
};
#endif

static int nvt_bootts_init(void)
{
	memset(boot_record.name[0], 0,
		NVT_BOOTTS_MAX_LEN * NVT_BOOTTS_NAME_MAX_LEN);
	index = 1;
	boot_record.time[0] = nvt_readl(NVT_TIMER_BASE_VIRT + NVT_TIMER_TM0_CNT);
	strncpy(boot_record.name[0], "ker", strlen("ker"));

	pr_info("NVTBOOTTS: %s initial success\n", __func__);

	return 0;
}

#ifdef CONFIG_PROC_FS
static int __init nvt_bootts_proc_init(void)
{
	struct proc_dir_entry *entry = NULL;

	nvt_info_dir_root = proc_mkdir("nvt_info", NULL);
	if (!nvt_info_dir_root)
		return -ENOMEM;

	entry = proc_create("bootts", 0664, nvt_info_dir_root, &nvt_bootts_fops);
	if (!entry)
		return -ENOMEM;

	entry = proc_create("tm0", 0664, nvt_info_dir_root, &nvt_timer_tm0_fops);
	if (!entry)
		return -ENOMEM;

	entry = proc_create("memperf", 0664, nvt_info_dir_root, &nvt_memperf_fops);
	if (!entry)
		return -ENOMEM;

	entry = proc_create("memhotplug", 0664, nvt_info_dir_root, &nvt_memhotplug_fops);
	if (!entry)
		return -ENOMEM;

	entry = proc_create("version", 0664, nvt_info_dir_root, &nvt_version_fops);
	if (!entry)
		return -ENOMEM;

	nvt_bootts_init();
	pr_info("NVTBOOTTS: %s initial success\n", __func__);

	return 0;
}

core_initcall(nvt_bootts_proc_init);
#endif
