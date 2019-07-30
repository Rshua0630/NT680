#include <linux/of_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#include "include/nt72_nand_config.h"
#include "include/nt72_nand.h"
#include "include/nt72_nand_nfc.h"
#include "include/nt72_nand_nfc_init.h"
#include "include/nt72_nand_utils.h"
#include "include/nt72_nand_io.h"
#include "include/nt72_nand_ir.h"
#include "include/nt72_nand_kapi.h"
#include "include/nt72_nand_parts.h"
#ifdef USE_DT
#include "include/nt72_nand_dt.h"
#endif

void __init nt72_nand_mtd_params_print(struct nt72_nand_info *info)
{
#ifdef USE_NEW_MTD
	nfc_msg("nfc: mtd: size=%lluMB erasesize=%dKB writesize=%d\n",
		info->mtd->size / 1024 / 1024, info->mtd->erasesize / 1024,
		info->mtd->writesize);
	nfc_msg("nfc: mtd: oobsize=%d oobavail=%d ecc_strength=%d\n",
		info->mtd->oobsize, info->mtd->oobavail,
		info->mtd->ecc_strength);
	nfc_msg("nfc: mtd: bitflip_threshold=%d\n",
		info->mtd->bitflip_threshold);
#else
	nfc_msg("nfc: mtd: size=%lluMB erasesize=%dKB writesize=%d\n",
		info->mtd->size / 1024 / 1024, info->mtd->erasesize / 1024,
		info->mtd->writesize);
	nfc_msg("nfc: mtd: oobsize=%d oobavail=%d\n", info->mtd->oobsize,
		info->mtd->oobavail);
#endif
}

#ifdef MODULE
#define caller_is_module() (1)
#else
#define caller_is_module() \
	is_module_text_address((unsigned long)__builtin_return_address(0))
#endif

int __init nt72_nand_scan(struct mtd_info *mtd, int maxchips)
{
	/*
	 * This function should sync with nand_scan() from nand_base.c.
	 * What we need is to update info->nand_scanning variable to indicate
	 * that we are scanning bad blocks during boot, in order to:
	 *	1. avoid read retry
	 *	2. avoid printing some error messages caused by bad blocks
	 *	3. skip software fill blank page
	 * So we can optimize the boot process.
	 */
	int ret;
	struct nt72_nand_info *info = mtd->priv;

	/* Many callers got this wrong, so check for it for a while... */
	if (!mtd->owner && caller_is_module()) {
		nfc_err("nfc_err: NULL mtd->owner\n");
		BUG();
	}

	/* Scan NAND ID */
	ret = nand_scan_ident(mtd, maxchips, NULL);
	if (ret) {
		nfc_err("nfc_err: nand_scan_ident failed: %d\n", ret);
		return ret;
	}

	/* Init mtd, nand_chip and BBT */
	info->nand_scanning = 1;
	ret = nand_scan_tail(mtd);
	info->nand_scanning = 0;
	if (ret)
		nfc_err("nfc_err: try continue with BBT fail: %d\n", ret);

	return 0;
}

int __init nt72_nand_probe(struct platform_device *pdev)
{
	struct mtd_info *mtd = NULL;
	struct nt72_nand_info *info = NULL;
	int ret = 0;
	int irq = 0;
	int found_cmd_parts = 0;
	struct nt72_nand_spec *nand_spec = NULL;

#ifdef USE_DT
	struct resource *resource = NULL;
	const struct of_device_id *match;
#endif

	/* allocate memory for nand_info and mtd_info */
	info = nt72_nand_kapi_malloc(sizeof(*info) + sizeof(*mtd),
				     GFP_KERNEL | __GFP_ZERO);
	if (!info) {
		nfc_err("failed to allocate memory for nand_info and mtd_info\n");
		return -ENOMEM;
	}
	/* assign info[1] to mtd_info */
	info->mtd = mtd = (struct mtd_info *)&info[1];
	/*
	 * save all allocated memory to
	 * platform_device->device->device_private->driver_data
	 */
	platform_set_drvdata(pdev, info);

	/* init nand_info */
	info->pdev = pdev;
	info->dev = &(pdev->dev);
	sema_init(&info->sem_nfc, 1);
	/*
	 * init mtd_info:
	 * mtd->priv should point to nand_chip, which is also the first item of
	 * nand_info, so we can use priv to access both of them.
	 * */
	mtd->priv = info;
	mtd->owner = THIS_MODULE;

#ifdef USE_DT
	match = of_match_device(g_nt72_nand_match_table, &pdev->dev);
	if (!match) {
		nfc_err("nfc_err: of device not found\n");
		ret = -ENODEV;
		goto fail_exit;
	}

	resource = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!resource) {
		nfc_err("nfc_err: no IO memory resource defined\n");
		ret = -ENODEV;
		goto fail_exit;
	}

	resource = request_mem_region(resource->start, resource_size(resource),
				      pdev->name);
	if (!resource) {
		nfc_err("failed to request memory resource\n");
		ret = -EBUSY;
		goto fail_exit;
	}

	ret = nt72_nand_nfc_ioremap_all(resource);
	if (ret) {
		nfc_err("nfc ioremap failed\n");
		ret = -ENODEV;
		goto fail_exit;
	}
#endif

	/* for Linux device driver */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		nfc_err("no IRQ resource defined\n");
		ret = irq;
		goto fail_exit;
	}

	ret = request_irq(irq, nt72_nand_ir_isr, 0, pdev->name, info);
	if (ret) {
		nfc_err("fail to request IRQ\n");
		goto fail_exit;
	}

	/* Start getting up our NFC */
	nt72_nand_cmd_history_init();
	nt72_nand_bus_sniffer_init();

	ret = nt72_nand_nfc_detect_flash(info, &nand_spec);
	if (ret) {
		nfc_err("fail to detect flash\n");
		ret = -ENODEV;
		goto fail_exit;
	}

	nt72_nand_nfc_nand_params_init(info, nand_spec);
	nt72_nand_nfc_nand_params_print(info);
	nt72_nand_init_nand_chip(info);

	info->data_buff =
		dma_alloc_coherent(&pdev->dev, info->nand_params.dma_buf_size,
				   &info->dma_data_buff_pa, GFP_KERNEL);
	if (!info->data_buff) {
		nfc_err("fail to request dma buffer\n");
		ret = -ENOMEM;
		goto fail_exit;
	}
	check_cache_alignment(info, "dma buffer",
			      (unsigned char *)info->dma_data_buff_pa);

#ifdef FILL_BLANK_PAGE
	info->ff_pattern = nt72_nand_kapi_malloc(info->nand_params.subpage_size,
						 GFP_KERNEL);
	if (!info->ff_pattern) {
		nfc_err("failed to allocate memory for ff_pattern\n");
		ret = -ENOMEM;
		goto fail_exit;
	}
	memset(info->ff_pattern, 0xFF,
	       info->nand_params.subpage_size);
#endif

	ret = nt72_nand_scan(mtd, 1);
	if (ret) {
		nfc_err("nt72_nand_scan failed: %d\n", ret);
		goto fail_exit;
	}

	/* try to parse partitions from cmdline */
#ifdef USE_CMDLINE_PARTS
	{
		struct mtd_partition *partitions = NULL;
		int nr_partitions = 0;

		mtd->name = g_nt72_nand_partition_table_name;
		nr_partitions =
			parse_mtd_partitions(mtd, g_nt72_nand_part_probes,
					     &partitions,
					     0);
		if ((nr_partitions > 0) && (partitions != NULL)) {
			nfc_msg(
				"nfc: Create partition table from command line\n");
			ret = add_mtd_partitions(mtd, partitions,
						 nr_partitions);
			found_cmd_parts = 1;
		}
	}
#endif
	/* fallback to prebuilt partition table */
	if (!found_cmd_parts) {
		if (g_nr_nt72_nand_mtd_partitions > 1) {
			nfc_msg("nfc: Found the prebuilt partition table\n");
			ret = mtd_device_register(
				      mtd, g_nt72_nand_mtd_partitions,
				      g_nr_nt72_nand_mtd_partitions);
		} else
			nfc_err("nfc_err: partition table not found\n");
	}

	nt72_nand_mtd_params_print(info);
	nt72_nand_nfc_nand_params_selftest(info);

	return 0;

fail_exit:
	if (!info->data_buff) {
		dma_free_coherent(&pdev->dev,
				  info->nand_params.dma_buf_size,
				  info->data_buff,
				  info->dma_data_buff_pa);
	}
	if (irq >= 0)
		free_irq(irq, info);
#ifdef USE_DT
	if (resource)
		release_mem_region(resource->start, resource_size(resource));
#endif
	kfree(info);
	return ret;
}

int nt72_nand_remove(struct platform_device *pdev)
{
	struct nt72_nand_info *info = platform_get_drvdata(pdev);
	struct mtd_info *mtd = info->mtd;
	int irq;

	platform_set_drvdata(pdev, NULL);

	nand_release(mtd);
	irq = platform_get_irq(pdev, 0);
	if (irq >= 0)
		free_irq(irq, info);

	dma_free_coherent(&pdev->dev, info->nand_params.dma_buf_size,
			  info->data_buff, info->dma_data_buff_pa);
	kfree(info);
	return 0;
}
