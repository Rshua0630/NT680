
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/memblock.h>
#include <linux/jiffies.h>
#include <linux/vmalloc.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/reboot.h>

#include <asm/memory.h> /*alloc_page_node*/
#include <linux/uaccess.h> /*copy_from_user*/
#include <asm-generic/mman.h> /*copy_from_user*/
#include <asm/cacheflush.h> /* copy_from_user*/
#include <asm/outercache.h>

#include <soc/nvt/ndp_hwmem_io.h>

#undef DEBUG_NDP_HWMEM
/*#define DEBUG_NDP_HWMEM 1*/

#ifdef DEBUG_NDP_HWMEM
#define DPRINT_NDP_HWMEM(fmt, args...) pr_info("[%s]" fmt, __func__, ##args)
#else
#define DPRINT_NDP_HWMEM(fmt, args...)
#endif

#define PRINT_HWMEM_ERR(fmt, args...) pr_err("[%s]" fmt, __func__, ##args)

#define NDP_HWMEM_NAME		"ndp_hwmem"
#define NDP_HWMEM_MINOR		192

void ndp_hwmem_flush_all(void)
{
	unsigned long flag;

	raw_local_irq_save(flag);

#ifdef CONFIG_SMP
	raw_local_irq_restore(flag);

	smp_call_function((smp_call_func_t) __cpuc_flush_kern_louis, NULL, 1);

	raw_local_irq_save(flag);
#endif

	flush_cache_all();

	outer_flush_all();

	raw_local_irq_restore(flag);

	DPRINT_NDP_HWMEM("FLUSH %d\n", __LINE__);

}
EXPORT_SYMBOL(ndp_hwmem_flush_all);

static inline void
ndp_hwmem_inv_range(const void *vir_addr, const unsigned long phy_addr,
					const size_t size)
{
	dmac_map_area(vir_addr, size, DMA_FROM_DEVICE);

	outer_inv_range(phy_addr, (unsigned long)(phy_addr+size));
}

static inline void
ndp_hwmem_clean_range(const void *vir_addr, const unsigned long phy_addr,
						const size_t size)
{
	dmac_map_area(vir_addr, size, DMA_TO_DEVICE);

	outer_clean_range(phy_addr, (unsigned long)(phy_addr+size));
}

static inline void
ndp_hwmem_flush_range(const void *vir_addr, const unsigned long phy_addr,
						const size_t size)
{
	dmac_flush_range(vir_addr, (const void *)((u32)vir_addr+size));

	outer_flush_range(phy_addr, (unsigned long)(phy_addr+size));
}

static const struct vm_operations_struct mmap_mem_ops = {
	.open = NULL,
#ifdef CONFIG_HAVE_IOREMAP_PROT
	.access = generic_access_phys
#endif
};

static inline int __intersects(phys_addr_t addr, phys_addr_t size,
						struct memblock_region *reg)
{
	if (addr >= reg->base + reg->size)
		return 1;
	else if (addr + size <= reg->base)
		return -1;
	return 0;
}

static bool memblock_intersects(struct memblock_type *type, phys_addr_t addr,
						phys_addr_t size)
{
	unsigned int left = 0, right = type->cnt;

	do {
		unsigned int mid = (right + left) / 2;

		int intersect_res = __intersects(addr, size,
							&type->regions[mid]);

		/* left side */
		if (intersect_res < 0)
			right = mid;
		/* right side */
		else if (intersect_res > 0)
			left = mid + 1;
		/* found intersection */
		if (intersect_res == 0)
			return true;

	} while (left < right);

	return false;
}

static int ndp_hwmem_mmap(struct file *file, struct vm_area_struct *vma)
{
	size_t size = vma->vm_end - vma->vm_start;

	if (memblock_intersects(&memblock.memory,
			(phys_addr_t) (vma->vm_pgoff) << PAGE_SHIFT,
			(phys_addr_t) size)) {
		return -EFAULT;
	}

	if (file->f_flags & O_SYNC)
#ifdef CONFIG_ARM_ERRATA_821423
		vma->vm_page_prot = __pgprot_modify(vma->vm_page_prot,
				L_PTE_MT_MASK, L_PTE_MT_UNCACHED | L_PTE_XN);
#else
#ifdef CONFIG_64BIT
		vma->vm_page_prot = __pgprot_modify(vma->vm_page_prot,
				PTE_ATTRINDX_MASK, PTE_ATTRINDX(1) | PTE_UXN);
#else
		vma->vm_page_prot = __pgprot_modify(vma->vm_page_prot,
				L_PTE_MT_MASK, L_PTE_MT_BUFFERABLE | L_PTE_XN);
#endif
#endif

	vma->vm_ops = &mmap_mem_ops;

	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	return remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
					size, vma->vm_page_prot);
}

static int
ndp_hwmem_cache(unsigned int cmd, struct ndp_rel_hwmem_t *p_hwmem_info)
{
	int ret_val = 0;
	struct ndp_rel_hwmem_t hwmem_info;

	ret_val = (int) copy_from_user(&hwmem_info, p_hwmem_info,
						sizeof(struct ndp_rel_hwmem_t));

	if (ret_val) {
		pr_err("get hwmem info failed\n");
		return -EINVAL;
	}

	/*debug message*/
	DPRINT_NDP_HWMEM("cmd case %d\n", cmd);
	DPRINT_NDP_HWMEM("vir_addr 0x%08lX\n", hwmem_info.vir_addr);
	DPRINT_NDP_HWMEM("phy_addr 0x%08lX\n", hwmem_info.phy_addr);
	DPRINT_NDP_HWMEM("size %d\n", (int)hwmem_info.size);

	switch (cmd) {
	case (CMD_HWMEM_INV_RANGE):
		ndp_hwmem_inv_range((const void *)hwmem_info.vir_addr,
				(const unsigned long)hwmem_info.phy_addr,
				hwmem_info.size);
		break;
	case (CMD_HWMEM_CLEAN_RANGE):
		ndp_hwmem_clean_range((const void *)hwmem_info.vir_addr,
				(const unsigned long)hwmem_info.phy_addr,
				hwmem_info.size);
		break;
	case (CMD_HWMEM_FLUSH_RANGE):
		ndp_hwmem_flush_range((const void *)hwmem_info.vir_addr,
				(const unsigned long)hwmem_info.phy_addr,
				hwmem_info.size);
		break;
	default:
		break;
	}

	return 0;
}

static long
ndp_hwmem_ioctl(struct file *file, unsigned int cmd, unsigned long args)
{
	int ret_val = 0;

	switch (cmd) {
	case (CMD_HWMEM_FLUSH):
		ndp_hwmem_flush_all();
		break;
	case (CMD_HWMEM_INV_RANGE):
	case (CMD_HWMEM_CLEAN_RANGE):
	case (CMD_HWMEM_FLUSH_RANGE):
		ret_val = ndp_hwmem_cache(cmd,
					(struct ndp_rel_hwmem_t *) args);
		break;
	default:
		break;
	}

	DPRINT_NDP_HWMEM("ioctl Done\n");

	return ret_val;
}

static int ndp_hwmem_open(struct inode *inode, struct file *file)
{
	DPRINT_NDP_HWMEM("open\n");
	return 0;
}

static int ndp_hwmem_release(struct inode *inode, struct file *file)
{
	DPRINT_NDP_HWMEM("release\n");
	return 0;
}

static const struct file_operations ndp_hwmem_fops = {
	.owner = THIS_MODULE,
	.open  = ndp_hwmem_open,
	.release = ndp_hwmem_release,
	.unlocked_ioctl = ndp_hwmem_ioctl,
	.mmap = ndp_hwmem_mmap,
};

static struct miscdevice ndp_hwmem_dev = {
	.minor = NDP_HWMEM_MINOR,
	.name = NDP_HWMEM_NAME,
	.fops = &ndp_hwmem_fops,
	.mode = 0600,
};

static int __init ndp_hwmem_init(void)
{
	int ret_val = 0;

	ret_val = misc_register(&ndp_hwmem_dev);

	if (ret_val)
		pr_err("[ERR]%s: misc register failed\n", NDP_HWMEM_NAME);
	else
		pr_info("[NDP_HWMEM] %s initialized\n", NDP_HWMEM_NAME);

	return ret_val;
}

static void __exit ndp_hwmem_exit(void)
{
	misc_deregister(&ndp_hwmem_dev);
}

module_init(ndp_hwmem_init);
module_exit(ndp_hwmem_exit);

MODULE_AUTHOR("ChunYen_Wang@novatek.com.tw");
MODULE_DESCRIPTION("Driver for NDP HW memory allocation");
MODULE_LICENSE("Proprietary");
