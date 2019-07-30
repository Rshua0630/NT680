#include <linux/module.h>
#include "include/nt72_nand.h"
#include "include/nt72_nand_config.h"
#include "include/nt72_nand_dt.h"
#include "include/nt72_nand_kernel.h"

#ifdef USE_DT

enum nvt_nfc_type {
	NFC,
	NFC_453
};

struct platform_device_id nt72_nand_devtype[] = {
	{
		.name = "nvt_nfc",
		.driver_data = NFC
	},
	{
		.name = "nvt_nfc_453",
		.driver_data = NFC_453
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, nt72_nand_devtype);

const struct of_device_id g_nt72_nand_match_table[] = {
	{
		.compatible = "nvt,nfc",
		.data = &nt72_nand_devtype[NFC]
	},
	{
		.compatible = "nvt,nfc-453",
		.data = &nt72_nand_devtype[NFC_453]
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, g_nt72_nand_match_table);

struct platform_driver nt72_nand_driver = {
	.driver			= {
		.name		= "nt72_nand_controller",
		.owner		= THIS_MODULE,
		.of_match_table = g_nt72_nand_match_table,
	},
	.probe			= nt72_nand_probe,
	.remove			= nt72_nand_remove,
};
module_platform_driver(nt72_nand_driver);

#endif /* USE_DT */
