/*
 *  drivers/nvtsoc/nt98700/rw-mem.c
 *
 *  Interface for user to read/write memory(register)
 *  Copyright (C) Novatek Microelectronics Corp, Jades Shih, 2009/5
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/pid.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>

struct rw_memory_t {
	unsigned int	addr;
	unsigned int	size;
	unsigned int	value;
	pid_t           pid;
};


#define MEMORY_IOC_MAGIC	0xEC38

#define MEMORY_IOC_READ		_IOW(MEMORY_IOC_MAGIC, 0, struct rw_memory_t)
#define MEMORY_IOC_WRITE	_IOW(MEMORY_IOC_MAGIC, 1, struct rw_memory_t)
#define WATCH_IOC_SET	_IOW(MEMORY_IOC_MAGIC, 2, struct rw_memory_t)
#define WATCH_IOC_CLEAR	_IOW(MEMORY_IOC_MAGIC, 3, struct rw_memory_t)
#define MAU_IOC_SET	_IOW(MEMORY_IOC_MAGIC, 4, struct mau_memory_t)
#define MAU_IOC_CLEAR	_IOW(MEMORY_IOC_MAGIC, 5, struct mau_memory_t)


#define NVTDBG_NAME		"nvtdbg"
#define NVTDBG_MINOR		99

#if 0
static int bad_address(void *p)
{
	unsigned long dummy;

	return probe_kernel_address((unsigned long *)p, dummy);
}

static unsigned long usr_v2p(struct mau_memory_t *pmau)
{
#if 0
	struct task_struct *ptask = get_pid_task(
		find_get_pid(pmau->pid), PIDTYPE_PID);

	pgd_t *pgd = pgd_offset(current->mm, pmau->vaddr);
	pmd_t *pmd;
	pte_t *pte;

	struct page *pg;
	unsigned long paddr = 0;

	if (bad_address(pgd)) {
		pr_info("[nskk] Alert: bad address of pgd %p\n", pgd);
		goto bad;
	}

	if (!pgd_present(*pgd)) {
		pr_info("[nskk] Alert: pgd not present %lu\n", *pgd);
		goto out;
	}

	pmd = pmd_offset(pgd, pmau->vaddr);
	if (bad_address(pmd)) {
		pr_info("[nskk] Alert: bad address of pmd %p\n", pmd);
		goto bad;
	}
	if (!pmd_present(*pmd)) {
		pr_info("[nskk] Alert: pmd not present %lu\n", *pmd);
		goto out;
	}

	pte = pte_offset(pmd, pmau->vaddr);
	if (bad_address(pte)) {
		pr_info("[nskk] Alert: bad address of pte %p\n", pte);
		goto bad;
	}
	if (!pte_present(*pte)) {
		pr_info("[nskk] Alert: pte not present %lu\n", *pte);
		goto out;
	}

	pg = pte_page(*pte);
	paddr = page_address(pg);

out:
	return paddr;

bad:
	pr_info("[nskk] Alert: Bad address\n");
	return 0;
#else
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *ptep, pte;
	unsigned long paddr = 0;
	unsigned long offset = 0;

	offset = pmau->vaddr & (PAGE_MASK - 1);
	pmau->vaddr &= PAGE_MASK;

	pgd = pgd_offset(current->mm, pmau->vaddr);
	if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
		goto out;

	pud = pud_offset(pgd, pmau->vaddr);
	if (pud_none(*pud) || unlikely(pud_bad(*pud)))
		goto out;

	pmd = pmd_offset(pud, pmau->vaddr);
	if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
		goto out;

	ptep = pte_offset(pmd, pmau->vaddr);
	if (!ptep)
		goto out;

	paddr = page_address(pte_page(*ptep));

	return (paddr + offset);

out:
	return 0;
#endif

}
#endif

static struct perf_event * __percpu *watch_event_hbp;

static void watch_event_handler(struct perf_event *bp,
			       struct perf_sample_data *data,
			       struct pt_regs *regs)
{
	show_regs(regs);
	panic("watch point triggered !\n");
}

int nvt_add_watch(unsigned int addr)
{
	struct perf_event_attr attr;
	int ret = 0;

	hw_breakpoint_init(&attr);
	attr.bp_addr = addr;
	attr.bp_len = HW_BREAKPOINT_LEN_4;
	attr.bp_type = HW_BREAKPOINT_W;

	watch_event_hbp =
	register_wide_hw_breakpoint(&attr, watch_event_handler, NULL);
	if (IS_ERR((void __force *)watch_event_hbp))
		ret = PTR_ERR((void __force *)watch_event_hbp);

	return ret;

}
EXPORT_SYMBOL(nvt_add_watch);

void nvt_remove_watch(void)
{
	unregister_wide_hw_breakpoint(watch_event_hbp);
}
EXPORT_SYMBOL(nvt_remove_watch);

int nvtdbg_ioctl(struct inode *inode, unsigned int cmd, unsigned long arg)
{
	int i, j, ret;
	struct rw_memory_t rw_mem_t;
	struct perf_event_attr attr;

	switch (cmd) {
	case WATCH_IOC_SET:
		if (copy_from_user(&rw_mem_t, (struct rw_memory_t *)arg
		, sizeof(rw_mem_t))) {
			pr_info("Invalid WATCH_IOC_SET parameter\n");
			return -EFAULT;
		}
		hw_breakpoint_init(&attr);
		attr.bp_addr = rw_mem_t.addr;
		attr.bp_len = HW_BREAKPOINT_LEN_4;
		attr.bp_type = HW_BREAKPOINT_W;

		watch_event_hbp = register_user_hw_breakpoint(&attr
		, watch_event_handler, NULL, find_task_by_vpid(rw_mem_t.pid));
		if (IS_ERR((void __force *)watch_event_hbp)) {
			ret = PTR_ERR((void __force *)watch_event_hbp);
			pr_info("set watchpoint fail !\n");
			return -EFAULT;
		}

		pr_info("set watchpoint to addr 0x%x\n", rw_mem_t.addr);
	break;

	case WATCH_IOC_CLEAR:
		unregister_hw_breakpoint(watch_event_hbp);
	break;
#if 0
	case MAU_IOC_SET:
		if (copy_from_user(&mau
		, (struct mau_memory_t *)arg, sizeof(mau))) {
			pr_info("Invalid MAU_IOC_SET parameter\n");
			return -EFAULT;
		}
		nvtmpu_add_protect(mau.paddr, mau.len, mau.mau_id, mau.mon_id);
	break;

	case MAU_IOC_CLEAR:
		if (copy_from_user(&mau,
		(struct mau_memory_t *)arg, sizeof(mau))) {
			pr_info("Invalid MAU_IOC_SET parameter\n");
			return -EFAULT;
		}
		nvtmpu_clear_protect(mau.mau_id, mau.mon_id);
	break;
#endif
	default:
		pr_info(" Invalid command 0x%08x\n", cmd);
		return -EINVAL;
	}
	return 0;
}

int nvtdbg_open(struct inode *inode, struct file *filp)
{
	return 0;
}

int nvtdbg_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations nvtdbg_fops = {
	.owner	= THIS_MODULE,
	.unlocked_ioctl	= nvtdbg_ioctl,
	.open	= nvtdbg_open,
	.release  = nvtdbg_release,
};

static struct miscdevice nvtdbg_dev = {
	.minor	= NVTDBG_MINOR,
	.name	= NVTDBG_NAME,
	.fops	= &nvtdbg_fops,
};

static int __init nvtdbg_init(void)
{
	return misc_register(&nvtdbg_dev);
}

static void __exit nvtdbg_exit(void)
{
	misc_deregister(&nvtdbg_dev);
}

module_init(nvtdbg_init);
module_exit(nvtdbg_exit);
