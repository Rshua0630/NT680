/*
 * drivers/staging/android/ion/ntk/ntk_ion.c
 *
 * Copyright (C) 2011 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/idr.h>
#include "../ion.h"
#include "../ion_priv.h"

#define ION_GRALLOC_HEAP_NAME	"gralloc"
#define ION_VIDEO_HEAP_NAME	"video"
#define NTK_ION_HEAP_NUM 2

static struct idr ntk_idr;

enum ion_heap_ids {
	INVALID_HEAP_ID = -1,
	ION_GRALLOC_HEAP_ID = ION_HEAP_TYPE_SYSTEM,
	ION_VIDEO_HEAP_ID = ION_HEAP_TYPE_DMA,
	ION_HEAP_ID_RESERVED = 31 /** Bit reserved for ION_FLAG_SECURE flag */
};

static u64 msm_dmamask = DMA_BIT_MASK(32);

static struct platform_device ion_cma_device = {
	.name = "ion-cma-device",
	.id = -1,
	.dev = {
	.dma_mask = &msm_dmamask,
	.coherent_dma_mask = DMA_BIT_MASK(32),
	}
};

/**
 * These heaps are listed in the order they will be allocated.
 */
struct ion_platform_heap ntk_heaps[] = {
		{
			.id	= ION_GRALLOC_HEAP_ID,
			.type	= ION_HEAP_TYPE_SYSTEM,
			.name	= ION_GRALLOC_HEAP_NAME,
		},

		{
			.id	= ION_VIDEO_HEAP_ID,
			.type	= ION_HEAP_TYPE_DMA,
			.name	= ION_VIDEO_HEAP_NAME,
			.priv    = (void *)&ion_cma_device.dev,
		},
};

static struct ion_platform_data ion_pdata = {
	.nr = NTK_ION_HEAP_NUM,
	.heaps = ntk_heaps,
};

static struct platform_device ion_dev = {
	.name = "ion-ntk",
	.id = 1,
	.dev = { .platform_data = &ion_pdata },
};

static struct ion_device *idev;
static int num_heaps;
static struct ion_heap **heaps;

static int ntk_ion_init(struct platform_device *pdev)
{
	struct ion_platform_data *pdata = pdev->dev.platform_data;
	int err;
	int i;

	num_heaps = pdata->nr;

	/*heaps = kzalloc(sizeof(struct ion_heap *) * pdata->nr, GFP_KERNEL);*/
	heaps = kcalloc(pdata->nr, sizeof(struct ion_heap *), GFP_KERNEL);

	idev = ion_device_create(NULL);
	if (IS_ERR_OR_NULL(idev)) {
		kfree(heaps);
		return PTR_ERR(idev);
	}

	/* create the heaps as specified in the board file */
	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &pdata->heaps[i];

		heaps[i] = ion_heap_create(heap_data);
		if (IS_ERR_OR_NULL(heaps[i])) {
			err = PTR_ERR(heaps[i]);
			goto err;
		}
		ion_device_add_heap(idev, heaps[i]);
	}

	idr_init(&ntk_idr);
	platform_set_drvdata(pdev, idev);
	return 0;
err:
	for (i = 0; i < num_heaps; i++) {
		if (heaps[i])
			ion_heap_destroy(heaps[i]);
	}
	kfree(heaps);
	return err;
}

static int ntk_ion_close(struct platform_device *pdev)
{
	struct ion_device *idev = platform_get_drvdata(pdev);
	int i;

	ion_device_destroy(idev);
	idr_destroy(&ntk_idr);
	for (i = 0; i < num_heaps; i++)
		ion_heap_destroy(heaps[i]);
	kfree(heaps);
	return 0;
}

struct idr *ntk_getidr(void)
{
	return &ntk_idr;
}

static int __init ion_init(void)
{
	return ntk_ion_init(&ion_dev);
}

static void __exit ion_exit(void)
{
	ntk_ion_close(&ion_dev);
}

module_init(ion_init);
module_exit(ion_exit);

