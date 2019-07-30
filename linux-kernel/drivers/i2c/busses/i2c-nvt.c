/*
 * Novatek NT96680 I2C adapter driver.
 *
 * Copyright (C) 2015 Novatek MicroElectronics Corp.
 *
 * Updated by Howard Chang Aug 2015
 *
 * ----------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ----------------------------------------------------------------------------
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/cpufreq.h>
#include <linux/gpio.h>
#include <linux/semaphore.h>

#include "mach/i2c_reg.h"
#define DRV_VERSION         "1.00.004"
#define	NVTIM_I2C_TIMEOUT (msecs_to_jiffies(1000))

static struct semaphore i2c_sem;
#define loc_cpu()		down(&i2c_sem);
#define unl_cpu()		up(&i2c_sem);

struct nvtim_i2c_dev {
	struct device       *dev;
	void __iomem        *base;
	struct completion   cmd_complete;
	struct clk          *clk;
	u8                  *buf;
	size_t              buf_len;
	u8                  addr;
	int                 irq;
	int                 stop;
	int                 flags;
	u32                 current_bytes;
	u32                 remain_bytes;
	int                 berr;
	int                 bal;
	struct i2c_adapter  adapter;
	struct nvtim_i2c_platform_data *pdata;
};

/* default platform data to use if not supplied in the platform_device */
static struct nvtim_i2c_platform_data nvtim_i2c_platform_data_default = {
	.bus_freq	= I2C_BUS_CLOCK_100KHZ,
	.bus_delay	= 0,
};

static inline void nvtim_i2c_write_reg(struct nvtim_i2c_dev *i2c_dev,
					 int reg, u32 val)
{
	__raw_writel(val, i2c_dev->base + reg);
}

static inline u32 nvtim_i2c_read_reg(struct nvtim_i2c_dev *i2c_dev, int reg)
{
	return __raw_readl(i2c_dev->base + reg);
}

static u32 nvtim_i2c_get_busclock(u32* gsr, u32* clkh, u32* clkl)
{
	return (I2C_SOURCE_CLOCK / (*clkl + *clkh + *gsr + 4));

}

static void nvtim_transceive_bytes(struct nvtim_i2c_dev *dev)
{
	dev->current_bytes = (dev->remain_bytes > I2C_BYTE_CNT_4) \
		? I2C_BYTE_CNT_4 : dev->remain_bytes;

	dev->remain_bytes = (dev->remain_bytes > I2C_BYTE_CNT_4) \
		? (dev->remain_bytes - I2C_BYTE_CNT_4): 0;
}

static void nvtim_calc_clk_dividers(struct nvtim_i2c_dev *dev)
{
	u32 gsr, tsr;
	u32 clkh, min_clkh;
	u32 clkl, min_clkl;
	u32 input_clock = dev->pdata->bus_freq;

	if ((input_clock < I2C_BUS_CLOCK_50KHZ) || \
		(input_clock > I2C_BUS_CLOCK_1MHZ)) {
		dev_err(dev->dev, "invalid value %d\n", input_clock);
	}

	gsr = I2C_DEFAULT_GSR;
	clkl = clkh = ((I2C_SOURCE_CLOCK / input_clock) - gsr) >> 1;
	while (nvtim_i2c_get_busclock(&gsr, &clkl, &clkh) > input_clock)
		clkl++;

	/*Standard mode (bus clock <= 100 KHz)*/
	if (input_clock <= I2C_BUS_CLOCK_100KHZ) {
		/*if Clock low period must >= 4.7 us,*/
		/*clock high period must >= 4 us*/
		min_clkl = (u32)(((float)I2C_SOURCE_CLOCK / (float)1000000000) \
			* (float)4700) + 1;
		min_clkh = (u32)(((float)I2C_SOURCE_CLOCK / (float)1000000000) \
			* (float)4000) + 1;
	} else if (input_clock <= I2C_BUS_CLOCK_400KHZ) {
		/*Fast mode (bus clock <= 400 KHz)*/
		/*if Clock low period must >= 1.3 us,*/
		/*clock high period must >= 0.6 us*/
		min_clkl = (u32)(((float)I2C_SOURCE_CLOCK / (float)1000000000) \
			* (float)1300) + 1;
		min_clkh = (u32)(((float)I2C_SOURCE_CLOCK / (float)1000000000) \
			* (float)600) + 1;
	} else {
		/*Fast mode plus (bus clock <= 1 MHz)*/
		/*if Clock low period must >= 0.5 us,*/
		/*clock high period must >= 0.26 us*/
		min_clkl = (u32)(((float)I2C_SOURCE_CLOCK / (float)1000000000) \
			* (float)500) + 1;
		min_clkh = (u32)(((float)I2C_SOURCE_CLOCK / (float)1000000000) \
			* (float)260) + 1;
	}

	/*Find correct clock low period*/
	if (clkl < min_clkl) {
		clkh -= (min_clkl - clkl);
		clkl  = min_clkl;
	}

	if (clkl > I2C_CLKLOW_MAX)
		clkl = I2C_CLKLOW_MAX;

	/*Find correct clock high period*/
	if (clkh < min_clkh)
		clkh = min_clkh;

	if (clkh > I2C_CLKHIGH_MAX)
		clkh = I2C_CLKHIGH_MAX;

	/*Adjust to real register settings*/
	clkl -= 1;
	if(clkl < I2C_CLKLOW_MIN)
		clkl = I2C_CLKLOW_MIN;

	clkh -= 3;
	if(clkh < I2C_CLKHIGH_MIN)
		clkh = I2C_CLKHIGH_MIN;

	while (nvtim_i2c_get_busclock(&gsr, &clkl, &clkh) > input_clock)
		clkl++;

	/*Fix TSR as 0x1. About 0.1us.*/
	tsr = 1;


	/*Clock low counter must > (4 + GSR + TSR)*/
	if (tsr >= (clkl - 4 - gsr))
		tsr = clkl - 5 - gsr;

	/*TSR: 1 ~ 1023*/
	if (tsr < I2C_TSR_MIN)
		tsr = I2C_TSR_MIN;
	else if (tsr > I2C_TSR_MAX)
		tsr = I2C_TSR_MAX;

	nvtim_i2c_write_reg(dev, I2C_BUSCLK_REG, \
		nvtim_i2c_read_reg(dev, I2C_BUSCLK_REG) | \
		clkl | (clkh << I2C_BUSCLK_HIGH_COUNTER));

	nvtim_i2c_write_reg(dev, I2C_TIMING_REG, \
		nvtim_i2c_read_reg(dev, I2C_TIMING_REG) | \
		tsr | (gsr << I2C_TIMING_GSR));

	dev_dbg(dev->dev, "input_clock = %d\n", input_clock);
}

/*This function configures I2C init conditions*/
static int i2c_nvtim_init(struct nvtim_i2c_dev *dev)
{
	/*put I2C into reset*/
	clk_disable(dev->clk);
	clk_unprepare(dev->clk);
	clk_prepare(dev->clk);
	clk_enable(dev->clk);

	/*compute clock dividers*/
	nvtim_calc_clk_dividers(dev);

	/*Set default timeout*/
	nvtim_i2c_write_reg(dev, I2C_TIMING_REG, \
		nvtim_i2c_read_reg(dev, I2C_TIMING_REG)| \
		I2C_DEFAULT_SCL_TIMEOUT << I2C_TIMING_SCLTIMEOUT);

	/*Enable interrupts*/
	nvtim_i2c_write_reg(dev, I2C_CTRL_REG, I2C_INTR_ALL);

	return 0;
}

/*Waiting for bus not busy*/
static int i2c_nvtim_wait_bus_not_busy(struct nvtim_i2c_dev *dev,
					 char allow_sleep)
{
	u32 timeout = I2C_POLLING_TIMEOUT;
	u32 busfree, busbusy;

	do
	{
		timeout--;
		busbusy = nvtim_i2c_read_reg(dev, I2C_STS_REG);
		busfree = busbusy >> I2C_STS_BUSFREE;
		busbusy &= BIT(I2C_STS_BUSBUSY);

		if (allow_sleep)
			schedule_timeout(1);
	} while (((busfree == 0) || (busbusy == 1)) && timeout);

	if (timeout)
		return 0;
	else
		return -EBUSY;
}

static void i2c_nvtim_handle_nack(struct nvtim_i2c_dev *dev)
{
	u32  timeout, reg;

	/*Configure controller to send STOP condition*/
	nvtim_i2c_write_reg(dev, I2C_CONFIG_REG, ~I2C_ALL_BIT | \
		BIT(I2C_CONFIG_STOP_GEN0)|BIT(I2C_CONFIG_PIO_DATASIZE));

	nvtim_i2c_write_reg(dev, I2C_CTRL_REG, \
		nvtim_i2c_read_reg(dev, I2C_CTRL_REG) | BIT(I2C_CTRL_TB_EN));

	/*Wait for STOP condition sent, controller back to idle*/
	timeout = I2C_POLLING_TIMEOUT;
	do
	{
		timeout--;
		reg = nvtim_i2c_read_reg(dev, I2C_STS_REG);
		reg = (reg >> I2C_STS_BUSY)& ~(I2C_STS_BUSBUSY >> I2C_STS_BUSY);
	} while ((reg == 1) && timeout);

	if (timeout == 0)
		dev_err(dev->dev, "Receive NACK, send STOP, \
			wait for ready timeout!\r\n");

}

static void nvtim_set_data(struct nvtim_i2c_dev *dev)
{
	u32	byte_shift, reg, config_reg;

	config_reg = ~I2C_ALL_BIT;
	reg = I2C_DATA_REG_DEFAULT;

    if (dev->buf_len != 0) {
        config_reg |= (dev->current_bytes << I2C_CONFIG_PIO_DATASIZE);
        byte_shift = 0;
        if ((dev->remain_bytes + dev->current_bytes) == (dev->buf_len+1)) {
            reg = (dev->addr << 1);
            config_reg |= BIT(I2C_START_GEN_SHIFT);
            byte_shift++;
        }

        for (; byte_shift < dev->current_bytes; byte_shift++) {
            reg |= (*dev->buf & I2C_DATA_MASK) << (I2C_DATA_SHIFT * byte_shift);
            dev->buf++;
        }

        /*Set the stop byte, note that stop byte count from 0*/
        if ((dev->remain_bytes == 0) && (dev->stop) && (dev->buf_len!=0)) {
            config_reg |= BIT(I2C_STOP_GEN_SHIFT + (dev->current_bytes - 1));
        }
    } else {  /* support i2cdetect (1 byte addr scan), but it will write additional 1 byte data(0x00) */
        config_reg |= ((dev->current_bytes+1) << I2C_CONFIG_PIO_DATASIZE);
        reg = (dev->addr << 1);
        config_reg |= BIT(I2C_START_GEN_SHIFT);

        reg |= ((0 & I2C_DATA_MASK) << I2C_DATA_SHIFT);
        config_reg |= BIT(I2C_STOP_GEN_SHIFT + 1);
    }

	/*Config controller*/
	nvtim_i2c_write_reg(dev, I2C_CONFIG_REG, config_reg);

	/*Write data*/
	nvtim_i2c_write_reg(dev, I2C_DATA_REG, reg);

	/*Control bits*/
	nvtim_i2c_write_reg(dev, I2C_CTRL_REG, \
		nvtim_i2c_read_reg(dev, I2C_CTRL_REG) | BIT(I2C_CTRL_TB_EN));
}

static void nvtim_set_rxconfig(struct nvtim_i2c_dev *dev)
{
	u32	config_reg, reg;

	/*Set default value*/
	config_reg = ~I2C_ALL_BIT;

	/*Set data size*/
	config_reg |= dev->current_bytes << I2C_CONFIG_PIO_DATASIZE;

	if ((dev->remain_bytes + dev->current_bytes) == (dev->buf_len+1)) {
		reg = (dev->addr << 1) | I2C_READ_BIT;
		config_reg |= BIT(I2C_START_GEN_SHIFT);
		/*Write data*/
		nvtim_i2c_write_reg(dev, I2C_DATA_REG, reg);
	}

	/*Set the stop byte, note that stop byte count from 0*/
	if ((dev->remain_bytes == 0) && (dev->stop)) {
		config_reg |= BIT(I2C_STOP_GEN_SHIFT+(dev->current_bytes - 1)) \
			| BIT(I2C_NACK_GEN_SHIFT + (dev->current_bytes - 1));
	}

	/*Config controller*/
	nvtim_i2c_write_reg(dev, I2C_CONFIG_REG, config_reg);

	/*Control bits*/
	nvtim_i2c_write_reg(dev, I2C_CTRL_REG, \
		nvtim_i2c_read_reg(dev, I2C_CTRL_REG) | BIT(I2C_CTRL_TB_EN));
}

static void nvtim_get_rxdata(struct nvtim_i2c_dev *dev)
{
	u32	byte_shift, reg;

	byte_shift = 0;
	if ((dev->remain_bytes + dev->current_bytes) == (dev->buf_len+1)) {
		byte_shift++;
	}
	for (; byte_shift < dev->current_bytes; byte_shift++) {
		reg = nvtim_i2c_read_reg(dev, I2C_DATA_REG);
		*dev->buf = (reg >> (I2C_DATA_SHIFT * byte_shift)) & I2C_DATA_MASK;
		dev->buf++;
	}
}

/*
 * Low level master read/write transaction. This function is called
 * from i2c_nvtim_xfer.
 */
static int
i2c_nvtim_xfer_msg(struct i2c_adapter *adap, struct i2c_msg *msg, int stop)
{
	struct nvtim_i2c_dev *dev = i2c_get_adapdata(adap);
	struct nvtim_i2c_platform_data *pdata = dev->pdata;
	int ret;

	/* Introduce a delay, required for some boards (e.g nvtim EVM) */
	if (pdata->bus_delay)
		udelay(pdata->bus_delay);

	dev->addr    = msg->addr;
	dev->buf     = msg->buf;
	dev->buf_len = msg->len;
	dev->flags   = msg->flags;
	dev->berr    = 0;
	dev->bal     = 0;
	dev->stop    = stop;

	reinit_completion(&dev->cmd_complete);

	dev->remain_bytes = dev->buf_len+1;
	do {
		nvtim_transceive_bytes(dev);

		if(msg->flags & I2C_M_RD)
			nvtim_set_rxconfig(dev);
		else
			nvtim_set_data(dev);

		ret = wait_for_completion_timeout(&dev->cmd_complete, dev->adapter.timeout);

		if (dev->berr) {
			//dev_err(dev->dev, "%s(%d) NACK!\n",__func__,__LINE__);
			i2c_nvtim_handle_nack(dev);
			i2c_nvtim_init(dev);
			return -EREMOTEIO;
		}

		if (dev->bal) {
			//dev_err(dev->dev, "%s(%d) arbitration lost!\n",__func__,__LINE__);
			i2c_nvtim_init(dev);
			return -EREMOTEIO;
		}
	} while(dev->remain_bytes > 0);

	/*Disable Controller*/
	nvtim_i2c_write_reg(dev, I2C_CTRL_REG, \
		nvtim_i2c_read_reg(dev, I2C_CTRL_REG) & ~BIT(I2C_CTRL_TB_EN));

	if (!ret) {
		dev_err(dev->dev, "i2c transfer timed out(%d)\n",__LINE__);
		i2c_nvtim_init(dev);
		dev->buf_len = 0;
		return -ETIMEDOUT;
	}

	if (ret < 0)
		return ret;
	else
		return msg->len;

	dev_err(dev->dev, "i2c transfer failed\n");

	return -EIO;
}

/*
 * Prepare controller for a transaction and call i2c_nvtim_xfer_msg
 */
static int
i2c_nvtim_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[], int num)
{
	struct nvtim_i2c_dev *dev = i2c_get_adapdata(adap);
	int i;
	int ret;

	dev_dbg(dev->dev, "%s: msgs: %d\n", __func__, num);
	ret = i2c_nvtim_wait_bus_not_busy(dev, 1);
	if (ret < 0) {
		dev_warn(dev->dev, "timeout waiting for bus ready\n");
		i2c_nvtim_init(dev);
		return ret;
	}

	for (i = 0; i < num; i++) {
		loc_cpu();
		ret = i2c_nvtim_xfer_msg(adap, &msgs[i], (i == (num - 1)));
		unl_cpu();
		dev_dbg(dev->dev, "%s [%d/%d] ret: %d\n", __func__, i + 1, num, ret);
		if (ret < 0)
			return ret;
	}

	return num;
}

static u32 i2c_nvtim_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

/*
 * Interrupt service routine. This gets called whenever an I2C interrupt
 * occurs.
 */
static irqreturn_t i2c_nvtim_isr(int this_irq, void *dev_id)
{
	struct nvtim_i2c_dev *dev = dev_id;
	u32 sts_reg;
	sts_reg = nvtim_i2c_read_reg(dev, I2C_STS_REG);

	/*Handle enabled interrupt status*/
	sts_reg &= nvtim_i2c_read_reg(dev, I2C_CTRL_REG) & I2C_CTRL_INTEN_MASK;

	if (sts_reg != 0) {
		/*Clear interrupt status*/
		nvtim_i2c_write_reg(dev, I2C_STS_REG, sts_reg);

		if (sts_reg & BIT(I2C_STS_DR)) {
			nvtim_get_rxdata(dev);
		}

		if (sts_reg & (BIT(I2C_STS_DT) | BIT(I2C_STS_DR))) {
			complete(&dev->cmd_complete);
		}

		if (sts_reg & BIT(I2C_STS_BERR)){
			dev->berr = 1;
			complete(&dev->cmd_complete);
		}

		if (sts_reg & BIT(I2C_STS_AL)){
			dev->bal = 1;
			complete(&dev->cmd_complete);
		}
	}

	return IRQ_HANDLED;
}

static struct i2c_algorithm i2c_nvtim_algo = {
	.master_xfer	= i2c_nvtim_xfer,
	.functionality	= i2c_nvtim_func,
};

#ifdef CONFIG_OF
static int nvtim_i2c_of_probe(struct platform_device *pdev, struct nvtim_i2c_dev *i2c)
{
	struct device_node *np = pdev->dev.of_node;
	u32 clock_frequency;

	if(of_property_read_u32(np, "clock-frequency", &clock_frequency)) {
	    dev_err(&pdev->dev, "Missing required parameter 'clock-frequency'\n");
	    return -ENODEV;
	}
	(i2c->pdata)->bus_freq = clock_frequency;

	return 0;
}
#else
#define nvtim_i2c_of_probe(pdev, i2c) -ENODEV
#endif

static int nvtim_i2c_probe(struct platform_device *pdev)
{
	struct nvtim_i2c_dev *dev;
	struct i2c_adapter *adap;
	struct resource *mem, *irq;
	int r;

	/* NOTE: driver uses the static register mapping */
	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem) {
		dev_err(&pdev->dev, "no mem resource?\n");
		return -ENODEV;
	}

	irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!irq) {
		dev_err(&pdev->dev, "no irq resource?\n");
		return -ENODEV;
	}

	dev = devm_kzalloc(&pdev->dev, sizeof(struct nvtim_i2c_dev),
			GFP_KERNEL);
	if (!dev) {
		dev_err(&pdev->dev, "Memory allocation failed\n");
		return -ENOMEM;
	}

	init_completion(&dev->cmd_complete);
	sema_init(&i2c_sem, 1);

	dev->dev   = &pdev->dev;
	dev->irq   = irq->start;
	dev->pdata = dev->dev->platform_data;
	platform_set_drvdata(pdev, dev);

	if (!dev->pdata) {
		dev->pdata = devm_kzalloc(&pdev->dev, sizeof(struct nvtim_i2c_platform_data), GFP_KERNEL);
		if (!dev->pdata)
			return -ENOMEM;

        if(nvtim_i2c_of_probe(pdev, dev) != 0) {
		    memcpy(dev->pdata, &nvtim_i2c_platform_data_default, sizeof(struct nvtim_i2c_platform_data));
	    }
	}

	dev->clk = devm_clk_get(&pdev->dev, dev_name(&pdev->dev));
	if (IS_ERR(dev->clk)) {
		dev_err(&pdev->dev, "can't find clock %s\n", dev_name(&pdev->dev));
		dev->clk = NULL;
	}
	else {
		clk_prepare(dev->clk);
		clk_enable(dev->clk);
	}

	dev->base = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(dev->base)) {
		r = PTR_ERR(dev->base);
		goto err_unuse_clocks;
	}

	i2c_nvtim_init(dev);

	r = devm_request_irq(&pdev->dev, dev->irq, i2c_nvtim_isr, 0,
			pdev->name, dev);
	if (r) {
		dev_err(&pdev->dev, "failure requesting irq %i\n", dev->irq);
		goto err_unuse_clocks;
	}

	adap = &dev->adapter;
	i2c_set_adapdata(adap, dev);
	adap->owner = THIS_MODULE;
	adap->class = I2C_CLASS_HWMON;
	strlcpy(adap->name, "nvtim I2C adapter", sizeof(adap->name));
	adap->algo = &i2c_nvtim_algo;
	adap->dev.parent = &pdev->dev;
	adap->timeout = NVTIM_I2C_TIMEOUT;

	adap->nr = pdev->id;
#ifdef CONFIG_OF
	adap->dev.of_node = pdev->dev.of_node;
#endif

	r = i2c_add_numbered_adapter(adap);
	if (r) {
		dev_err(&pdev->dev, "failure adding adapter\n");
		goto err_unuse_clocks;
	}

	dev_info(&pdev->dev, "Register %s successfully\n", __func__);
	return 0;

err_unuse_clocks:
	clk_disable_unprepare(dev->clk);
	dev->clk = NULL;
	return r;
}

static int nvtim_i2c_remove(struct platform_device *pdev)
{
	struct nvtim_i2c_dev *dev = platform_get_drvdata(pdev);

	i2c_del_adapter(&dev->adapter);

	clk_disable_unprepare(dev->clk);
	dev->clk = NULL;

	nvtim_i2c_write_reg(dev, I2C_CTRL_REG, 0);

	return 0;
}

#ifdef CONFIG_PM
static int nvtim_i2c_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct nvtim_i2c_dev *i2c_dev = platform_get_drvdata(pdev);

	clk_disable_unprepare(i2c_dev->clk);

	return 0;
}

static int nvtim_i2c_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct nvtim_i2c_dev *i2c_dev = platform_get_drvdata(pdev);

	clk_prepare_enable(i2c_dev->clk);

	return 0;
}

static const struct dev_pm_ops nvtim_i2c_pm = {
	.suspend        = nvtim_i2c_suspend,
	.resume         = nvtim_i2c_resume,
};

#define nvtim_i2c_pm_ops (&nvtim_i2c_pm)
#else
#define nvtim_i2c_pm_ops NULL
#endif

#ifdef CONFIG_OF
static const struct of_device_id nvtim_i2c_ids[] = {
    { .compatible = "nvt,nvt_i2c" },
    {},
};
MODULE_DEVICE_TABLE(of, nvtim_i2c_ids);
#else
#define nvtim_i2c_ids     NULL
#endif

static struct platform_driver nvtim_i2c_driver = {
	.probe		= nvtim_i2c_probe,
	.remove		= nvtim_i2c_remove,
	.driver		= {
		.name	        = "nvt_i2c",
		.owner	        = THIS_MODULE,
		.pm	            = nvtim_i2c_pm_ops,
		.of_match_table = of_match_ptr(nvtim_i2c_ids),
	},
};

/* I2C may be needed to bring up other drivers */
static int __init nvtim_i2c_init_driver(void)
{
	return platform_driver_register(&nvtim_i2c_driver);
}
subsys_initcall(nvtim_i2c_init_driver);

static void __exit nvtim_i2c_exit_driver(void)
{
	platform_driver_unregister(&nvtim_i2c_driver);
}
module_exit(nvtim_i2c_exit_driver);

MODULE_AUTHOR("Novatek");
MODULE_VERSION(DRV_VERSION);
MODULE_DESCRIPTION("NT96680 I2C bus adapter");
MODULE_LICENSE("GPL");
