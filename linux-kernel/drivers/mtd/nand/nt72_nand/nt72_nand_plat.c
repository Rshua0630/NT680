#include <linux/init.h>
#include <linux/module.h>

#include <linux/mtd/physmap.h>
#include <linux/platform_device.h>

#include <mtd/mtd-abi.h>

#include "include/nt72_nand_config.h"
#include "include/nt72_nand_kernel.h"
#include "include/nt72_nand_plat.h"
#include "include/nt72_nand_regs.h"

#ifndef USE_DT

static struct resource nt72_nand_resources[] = {
	{
		.start = REG_NFC_BASE,
		.end = REG_NFC_BASE + 0x1000,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = IRQ_NFC_NUM,
		.end = IRQ_NFC_NUM,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device nt72_nand_device = {
	.name		= "nt72_nand",
	.id		= -1,
	.resource	= nt72_nand_resources,
	.num_resources	= ARRAY_SIZE(nt72_nand_resources),
};

static int __init nvt_init_mtd_nand(void)
{
	return platform_device_register(&nt72_nand_device);
}

arch_initcall(nvt_init_mtd_nand);

static struct platform_driver nt72_nand_driver = {
	.driver		= {
		.name	= "nt72_nand",
	},
	.probe		= nt72_nand_probe,
	.remove		= nt72_nand_remove,
};

static int __init nt72_nand_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&nt72_nand_driver);
	return ret;
}

module_init(nt72_nand_init);

static void __exit nt72_nand_exit(void)
{
	platform_driver_unregister(&nt72_nand_driver);
}

module_exit(nt72_nand_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Novatek NT72xxx NAND Flash Controller driver");

#endif /* USE_DT */
