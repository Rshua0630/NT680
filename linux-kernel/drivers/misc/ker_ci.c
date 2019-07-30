/* ----------------------------------------------------------------------------
 *  Copyright (c) 2013 Novatek Microelectronics Corporation.
 *  All Rights Reserved.
 *
 *  ker_ci.c
 *  CI/CI+ PCMCIA interface control.
 *
 *  Novatek NT72668 Linux Driver Version 0.00.0
 * ----------------------------------------------------------------------------
 */
#include <linux/kernel.h>		/* ARRAY_SIZE */
#include <linux/init.h>			/* module_init/module_exit macros */
#include <linux/module.h>		/* MODULE_LICENSE */
#include <linux/delay.h>		/*! Dealy */
#include <linux/kthread.h>		/*! kthread */
#include <linux/jiffies.h>		/*! jiffies */

#include <linux/err.h>			/* IS_ERR */
#include <linux/interrupt.h>	/* irqreturn_t, tasklet */
#include <linux/version.h>		/* LINUX_VERSION_CODE */
#include <linux/types.h>		/* pid_t*/
#include <linux/mm.h>           /* struct vm_area_struct */
#include <linux/fs.h>           /* struct file_operations */
#include <linux/cdev.h>			/* cdev_ API */
#include <linux/proc_fs.h>		/* PROC_I */
#include <linux/wait.h>
#include <linux/kdev_t.h>		/*! MAJOR, MINOR, MKDEV */
#include <linux/slab.h>			/*! KMALLOC */
#include <linux/uaccess.h>		/*! copy_from_user, copy_to_user */

#include <linux/mm_types.h>       /*! io_remap_pfn_range,
				    remap_pfn_range, pgprot_noncached */
#include <linux/ioctl.h>
#include <linux/dma-mapping.h>
#include <linux/sysctl.h>


#include <linux/semaphore.h>

#include "ker_ce_syscalls.h"

#define SKYWORTH_TOOL


#include <linux/io.h>				/* virt_to_phys */


/*---------------------------------------------------------------------------*/
/* Local Constant Definitions                                                */
/*---------------------------------------------------------------------------*/
#define KER_NOT_POLLING_MODE	1

#ifndef K_OK
#define K_OK          (0)
#endif

#ifndef K_NOT_OK
#define K_NOT_OK      (-1)
#endif

#define KER_CI_SLOT_NUM			2

#define TURNKEY_DRV

#ifdef TURNKEY_DRV
struct task_struct *proc_ci_task;
#endif

#define CE_TRUE 1
#define CE_FALSE 0

/*--------------------------------------------------------------------------*/
/* Local Function Protype                                                   */
/*--------------------------------------------------------------------------*/
#if 0
#define KCE_ERR(msg...) printk(msg)
#define KCE_WRN(msg...) printk(msg)
#define KCE_MSG(msg...) printk(msg)
#define KCE_DBG(msg...) printk(msg)
#else
#define KCE_ERR(msg...)
#define KCE_WRN(msg...)
#define KCE_MSG(msg...)
#define KCE_DBG(msg...)
#endif

/*--------------------------------------------------------------------------*/
/* Local Global Variables                                                   */
/*--------------------------------------------------------------------------*/
char g_ker_ci_dbg;

#ifdef TURNKEY_DRV
#define KCE_NAME			"ntkcetool"
u32 gu32TKopen = 1;
#endif
#define CE_NAME			"ntkcetool"


struct memreq {
	unsigned int base;
	unsigned int size;
};

struct memory_property {
	unsigned int base;
	unsigned int size;
	unsigned int mode;
};

struct memory_node {
	const char *name;
	const char *type;

	struct memory_property property;

	void *data;
	unsigned long _flag;
	struct device cma_device;
	struct memory_node *allnext;
	struct device_node *dev_node;
};


struct nvt_ci_dev *gst_ci_dev;
static struct memreq g_encrypt_mem = {0};
static u32	gu32KerCIDetectEnable = CE_FALSE;

dma_addr_t ci_dma_handle;
static u32 gu32CIMemPhysAddr;
static void *gpu32CIMemVirtAddr;
static int CEmajor;
module_param(CEmajor, int, 0);

static int flag;
u32 *kci_reg_bak;

/*---------------------------------------------------------------------------*/
/* Extern Global Variables                                                   */
/*---------------------------------------------------------------------------*/
/*extern struct dvb_adapter stAdapter;*/

u32 gReleaseFlag;	/* 1: Release, 0: no Release */

/*---------------------------------------------------------------------------*/
/* Structure                                                      */
/*---------------------------------------------------------------------------*/

/* stores all private variables for communication with CI */
struct nvt_ci_dev {
	struct mutex ca_mutex;

	struct dvb_adapter *dvb_adapter;
	struct device *dev;

	void *priv;
	u8 current_irq_mode;
	int current_ci_flag;

	u8 databuf[32];
	int datalen;
	bool pbIsCardPresent;
};

#ifdef TURNKEY_DRV
struct kci_dev {
	u32					CI_INT_state;
	struct tasklet_struct ci_tasklet;
	struct semaphore	ci_sem;
	struct cdev			ci_cdev;
};

struct kci_dev *kci_device;

#endif

/*--------------------------------------------------------------------------*/
/*  Local Functions                                                         */
/*--------------------------------------------------------------------------*/
#if 1
#define K_INITREGISTER(initfn)				\
	static int __init __##initfn(void)		\
{ return initfn(); }					\
module_init(__##initfn)
#define K_EXITREGISTER(exitfn)				\
	static void __exit __##exitfn(void)		\
{ exitfn(); }							\
module_exit(__##exitfn)
#else
#define K_INITREGISTER(initfn)
#define K_EXITREGISTER(exitfn)
#endif

/*--------------------------------------------------------------------------*/
/* Semaphore Functions                                                      */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* Local Functions                                                          */
/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
/* Module Functions                                                         */
/*--------------------------------------------------------------------------*/
static int nvt_ci_phy_addr_procctl(struct ctl_table *ctl, int write,
		void __user *buffer, size_t *lenp, loff_t *fpos)
{
	return proc_dointvec(ctl, write, buffer, lenp, fpos);
}

static struct ctl_table_header *sysctls_root_table;
static u32 ker_ci_phy_addr = CONFIG_NVT_KER_CI_PHY_ADDR;

static struct ctl_table nvt_ci_phy_addr_sysctls[] = {
	{
		.procname	= "nvt_ci_phy_addr",
		.data		= &ker_ci_phy_addr,/* Data pointer and size.*/
		.maxlen		= sizeof(ker_ci_phy_addr),
		.mode		= 0444,	/* Mode, proc handler. */
		.proc_handler	= nvt_ci_phy_addr_procctl
	},
	{}
};

static struct ctl_table sysctls_root[] = {
	{
		.procname	= "nvt",
		.mode		= 0555,
		.child		= nvt_ci_phy_addr_sysctls
	},
	{}
};


/*---------------------------------------------------------------------------*/
/*   HW function                                                    */
/*---------------------------------------------------------------------------*/


#ifdef TURNKEY_DRV

int32_t Ker_CE_Close(void)
{

	proc_ci_task = NULL;

	kci_device->CI_INT_state = 0;
	flag = 1;


	return 0;
}

static long NTCE_Ioctl(struct vos_file *file, unsigned int cmd,
		unsigned long arg)
{
	int retval = 0;

	if (((void *)arg) == NULL)
		return -ENOTSUPP;

	switch (cmd) {
	case IOCTL_CI_GET_ENCRYPT_MEM:
			{
				struct CI_Ioctl_Get_Encrypt_Mem_t UserParams;

#ifndef SKYWORTH_TOOL
				UserParams.physAddr = g_encrypt_mem.base;
				UserParams.size = g_encrypt_mem.size;
#else
				UserParams.physAddr = ker_ci_phy_addr;
				UserParams.size = 0x40000;
#endif

				if (copy_to_user(
				(struct CI_Ioctl_Get_Encrypt_Mem_t *)arg,
				&UserParams,
				sizeof(struct CI_Ioctl_Get_Encrypt_Mem_t))) {
					/* Invalid user space address */
					retval = -EFAULT;

					goto fail;
				}

				break;
			}

	case IOCTL_CI_EN50221_RELEASE:
			{
				struct CI_Ioctl_EN50221_Release_t UserParams;

				if (copy_from_user(
				&UserParams,
				(struct CI_Ioctl_EN50221_Release_t *)arg,
				sizeof(struct CI_Ioctl_EN50221_Release_t))) {
					retval = -EFAULT;

					goto fail;
				}

				gReleaseFlag = UserParams.rlflag;

				break;
			}

	case IOCTL_CI_TERM:                  /* NTCI_Exit */
			{
				Ker_CE_Close();
				break;
			}

	default:
			break;
	}

	return 0;

fail:
	return retval;

}


static int NTCE_Open(struct inode *inode, struct vos_file *file)
{
	proc_ci_task = current;

	gReleaseFlag = 1;

	return 0;
}


static int NTCE_Release(struct inode *inode, struct vos_file *file)
{

	proc_ci_task = NULL;

	gReleaseFlag = 0;


	return 0;
}

static int crypto_mmap(struct file *filp, struct vm_area_struct *vma)
{

	int ret = 0;

	unsigned long vm_size = vma->vm_end - vma->vm_start;

	vma->vm_flags |= (VM_IO | VM_DONTDUMP);
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	if (io_remap_pfn_range(vma, vma->vm_start,
	ker_ci_phy_addr >> PAGE_SHIFT, vm_size, vma->vm_page_prot) < 0)	{
		printk("WARNING! remap failed!!\n");
		return -EAGAIN;
	}


	return ret;
}

const struct file_operations gci_fops = {
	.unlocked_ioctl		= NTCE_Ioctl,/* without Big Kernel Lock */
	.owner			= THIS_MODULE,
	.open			= NTCE_Open,
	.release		= NTCE_Release,
	.mmap			= crypto_mmap,
};

struct cdev gci_cdev = {
	.kobj	=   {.name = KCE_NAME, },
	.owner	= THIS_MODULE,
	.ops	= &gci_fops,
};

static struct class *g_ci_dev_class;

static char *CI_DevNode(struct device *dev, umode_t *mode)
{
	*mode = 0666;
	return NULL;
}

#endif	/* end TURNKEY_DRV */


static int nvt_ce_register(struct nvt_ci_dev *nvt_ci_dev, dev_t devno)
{
	long retval = 0;

	return retval;
}

static int nvt_ce_unregister(struct nvt_ci_dev *nvt_ci_dev)
{

	#ifdef TURNKEY_DRV
	gu32KerCIDetectEnable = CE_FALSE;
	#endif	/* End TRUNKEY_DRV */

	kfree(nvt_ci_dev);

	KCE_MSG(" Novatek CE Driver unloaded\n");
	KCE_MSG(KERN_INFO "NTKCE Device unregistered\n");

	return 0;
}

int __init KER_CE_DrvInit(void)
{
	int ret = 0;
	struct nvt_ci_dev *nvt_ci_dev;
	dev_t		devno = 0;

#ifdef TURNKEY_DRV
	long retval = 0;

	if (CEmajor == 0) {
		retval = alloc_chrdev_region(&devno, 0, 1, CE_NAME);
		CEmajor = MAJOR(devno);
	} else {
		devno = MKDEV(CEmajor, 0);
		retval = register_chrdev_region(devno, 1, CE_NAME);
	}

	if (retval) {
		KCE_MSG("can't get major %d\n", CEmajor);
		goto error;
	}
	kci_device = kmalloc(sizeof(struct kci_dev), GFP_KERNEL);

	if (!kci_device)
		return -ENOMEM;

	memset(kci_device, 0, sizeof(struct kci_dev));


	cdev_init(&kci_device->ci_cdev, &gci_fops);
	kci_device->ci_cdev.owner = THIS_MODULE;
	kci_device->ci_cdev.ops = &gci_fops;


	retval = cdev_add(&kci_device->ci_cdev, devno, 1);
	if (retval) {

		cdev_del(&kci_device->ci_cdev);
		unregister_chrdev_region(devno, 1);
		kfree(kci_device);
		KCE_ERR(" 0x%x adding /dev/%s", (int)retval, CE_NAME);
		goto error;
	}

	/* make & register device node */
	g_ci_dev_class = class_create(THIS_MODULE, CE_NAME);
	g_ci_dev_class->devnode = CI_DevNode;
	if (IS_ERR(g_ci_dev_class)) {
		retval = PTR_ERR(g_ci_dev_class);
	} else {
		struct device *mdev;

		mdev = device_create(g_ci_dev_class, NULL,
				devno, NULL, CE_NAME);
		if (mdev == NULL)
			KCE_ERR("creat kci device fail !\n");
	}

	sema_init(&kci_device->ci_sem, 1);

	kci_device->CI_INT_state = 0;

	gu32KerCIDetectEnable = CE_TRUE;

#endif	/* End TURNKEY_DRV */

	if (g_encrypt_mem.base == 0) {
#ifndef SKYWORTH_TOOL
		node = nvt_mem_find_node_by_name("encrypt");

		if (node == NULL)
			KCE_ERR("Set encrypt address Fail!\n");

		nvt_mem_alloc_from_contiguous(node, &g_encrypt_mem);
#else

		gpu32CIMemVirtAddr = (u32 *) dma_alloc_coherent
			(NULL, 0x40000, &ci_dma_handle,  GFP_KERNEL | GFP_DMA);
		if (gpu32CIMemVirtAddr != 0) {
			gu32CIMemPhysAddr = (u32)ci_dma_handle;

			KCE_DBG("CE mem phys addr 0x%08X,
			virt addr 0x%08X! \n", gu32CIMemPhysAddr,
			(u32)gpu32CIMemVirtAddr);
		} else
			KCE_ERR("CE allocate buffer fail\n");
#endif


	}
	KCE_DBG("LINE %d,FUNC %s mem base %x , size %d\r\n",
	__LINE__, __func__, g_encrypt_mem.base, g_encrypt_mem.size);
	/* Get physical address & size */

	/* Suspend/Resume */
	kci_reg_bak = kzalloc(sizeof(int)*7, GFP_KERNEL);
	/* only need to backup 7 register, each is 32 bit. */
	if (!kci_reg_bak)
		return -ENOMEM;

	nvt_ci_dev = kzalloc(sizeof(struct nvt_ci_dev), GFP_KERNEL);
	if (!nvt_ci_dev) {
		KCE_ERR(" [KER_CE_DrvInit] out of memory\n");
		return -ENOMEM;
	}
	gst_ci_dev = nvt_ci_dev;

	ret = nvt_ce_register(nvt_ci_dev, devno);
	if (ret < 0) {
		KCE_ERR(KERN_ERR "ntk_ce: failed %d while loading driver\n"
		, ret);
		return ret;
	}

	KCE_MSG(KERN_INFO
	"Novatek kernel ce device driver successfully loaded.\n");
	sysctls_root_table = register_sysctl_table(sysctls_root);

	return ret;

#ifdef TURNKEY_DRV
error:
	return retval;
#endif /* End TURNKEY_DRV */

}

void __init KER_CE_DrvExit(void)
{
	int ret = 0;
	struct nvt_ci_dev *nvt_ci_dev;

	#ifdef TURNKEY_DRV
	/*dev_t devno = VK_MKDEV(NTKCI_MAJOR, 0);*/
	dev_t devno = MKDEV(CEmajor, 0);


	cdev_del(&kci_device->ci_cdev);
	unregister_chrdev_region(devno, 1);

	#endif /* End TURNKEY_DRV */

	nvt_ci_dev = gst_ci_dev;
	if (nvt_ci_dev == NULL)
		return;

	ret = nvt_ce_unregister(nvt_ci_dev);
	if (ret < 0) {
		KCE_ERR(KERN_ERR
		"ntk_ce: failed %d while exit driver\n", ret);
		return;
	}

	#ifdef TURNKEY_DRV
		kfree(kci_device);
	#endif

	kfree(kci_reg_bak);

}



K_INITREGISTER(KER_CE_DrvInit);
K_EXITREGISTER(KER_CE_DrvExit);

