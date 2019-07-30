/*
 * spi_na51000.c - Novatek SPI controller driver.
 *
 * Based on linux/drivers/spi/spi_na51000.c
 *
 * Copyright (C) 2017 Novatek
 *
 */
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/spi/spi.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/module.h>
#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>
#include <mach/spi_int.h>

#ifdef CONFIG_OF
#include <linux/of_device.h>
#endif

#define SPI_DMA_DIR_SPI2RAM                 (0)
#define SPI_DMA_DIR_RAM2SPI                 (1)

#define SPI_ACTIVE_CS                       (0)
#define SPI_INACTIVE_CS                     (1)

#define SPI_DMA_MAX_LENGTH                  ((1<<24)-1)

#define SPI_INT_MSK_DMAED                   (1<<3)

/*
 * HAL IO macros.
 */
#define HAL_READ_UINT32( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned int *)(_register_)))

#define HAL_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))

//ported from spi.h
/**
    SPI ID
*/
typedef enum
{
    SPI_ID_1,                   ///< SPI 1 (PIO, DMA)
    SPI_ID_2,                   ///< SPI 2 (PIO, DMA)
    SPI_ID_3,                   ///< SPI 3 (PIO)
    SPI_ID_4,                   ///< SPI 4 (PIO, Gyro)
    SPI_ID_5,                   ///< SPI 5 (PIO)

    SPI_ID_COUNT,               ///< SPI ID count
} SPI_ID;

struct nvt_spi {
	struct workqueue_struct	*workqueue;
	struct work_struct work;
	spinlock_t lock;	/* protect 'queue' */
	struct list_head queue;
	void __iomem *base;
	int baseclk;
	struct clk *clk;
	int irq;
	u32 max_speed_hz, min_speed_hz;
	int last_chipselect;
	int last_chipselect_val;
	atomic_t event;
	struct completion done;
};

static u32 spi_getreg(struct nvt_spi* spi_bus, u32 offset)
{
	u32 reg_value;

	reg_value = readl(spi_bus->base+offset);

	return reg_value;
}

/*
	Set SPI controller register.

	@param[in] spiID        SPI controller ID
	@param[in] offset       register offset in SPI controller (word alignment)
	@param[in] value        register value

	@return void
*/
static void spi_setreg(struct nvt_spi* spi_bus, u32 offset, u32 value)
{
	writel(value, spi_bus->base+offset);
}

static void nvt_spi_set_npcs(struct nvt_spi* spi_bus,int val)
{
	union SPI_CTRL_REG ctrl_reg;
	ctrl_reg.reg = spi_getreg(spi_bus, SPI_CTRL_REG_OFS);

	if (val == SPI_ACTIVE_CS)
		ctrl_reg.bit.spics_value = 0;
	else
		ctrl_reg.bit.spics_value = 1;

	spi_setreg(spi_bus, SPI_CTRL_REG_OFS, ctrl_reg.reg);

	// Due to outstanding feature
	// Polling to ensure register write is passed to controller
	while (1) {
		union SPI_CTRL_REG read_reg;

		read_reg.reg = spi_getreg(spi_bus, SPI_CTRL_REG_OFS);
		if (read_reg.bit.spics_value == ctrl_reg.bit.spics_value)
			break;
	}
}

static int nvt_spi_setup(struct spi_device *spi)
{
	struct spi_master *master = spi->master;
	struct nvt_spi *c = spi_master_get_devdata(master);

	union SPI_IO_REG io_reg = {0};
	union SPI_CTRL_REG ctrl_reg = {0};
	union SPI_DLY_CHAIN_REG dly_reg = {0};
	union SPI_CONFIG_REG config_reg = {0};
	unsigned int bits_per_word = spi->bits_per_word;

	if (spi->max_speed_hz < 100000) {
		printk("ERR: %s force %d Hz to 100 KHz\r\n", __func__, spi->max_speed_hz);
		spi->max_speed_hz = 100000;
	} else if (spi->max_speed_hz > 48000000) {
		printk("ERR: %s force %d Hz to 48 MHz\r\n", __func__, spi->max_speed_hz);
		spi->max_speed_hz = 48000000;
	}

	if (!IS_ERR(c->clk))
		clk_set_rate(c->clk, spi->max_speed_hz);

	switch (spi->mode & (SPI_CPOL | SPI_CPHA)) {
	case SPI_MODE_0:
		dly_reg.bit.latch_clk_edge = 1;
		break;
	case SPI_MODE_1:
		io_reg.bit.spi_cpha = 1;
		dly_reg.bit.latch_clk_shift = 1;
		break;
	case SPI_MODE_2:
		io_reg.bit.spi_cpol = 1;
		dly_reg.bit.latch_clk_edge = 1;
		break;
	default:
		io_reg.bit.spi_cpol = 1;
		io_reg.bit.spi_cpha = 1;
		dly_reg.bit.latch_clk_shift = 1;
		break;
	}

	io_reg.bit.spi_io_out_en = 1;
	io_reg.bit.spi_auto_io_out_en = 1;
	io_reg.bit.spi_io_order = 1;

	spi_setreg(c, SPI_IO_REG_OFS, io_reg.reg);

	spi_setreg(c, SPI_DLY_CHAIN_REG_OFS, dly_reg.reg);

	config_reg.reg = spi_getreg(c,SPI_CONFIG_REG_OFS);
	if (spi->mode&SPI_LSB_FIRST)
		config_reg.bit.spi_pkt_lsb_mode = 0x1;
	else
		config_reg.bit.spi_pkt_lsb_mode = 0x0;

	if (bits_per_word == 16)
		config_reg.bit.spi_pktlen = SPI_PKT_LEN_ENUM_2BYTES;
	else
		config_reg.bit.spi_pktlen = SPI_PKT_LEN_ENUM_1BYTE;

	spi_setreg(c, SPI_CONFIG_REG_OFS, config_reg.reg);

	do {
		ctrl_reg.bit.spi_en = 1;
		spi_setreg(c, SPI_CTRL_REG_OFS, ctrl_reg.reg);
		ctrl_reg.reg = spi_getreg(c,SPI_CTRL_REG_OFS);
	} while (ctrl_reg.bit.spi_en == 0);

	/* deselect chip */
	nvt_spi_set_npcs(c,SPI_INACTIVE_CS);

	return 0;
}

static void spi_set_packet_count(struct nvt_spi* spi_bus, u32 packets)
{
	union SPI_CONFIG_REG config_reg;

	config_reg.reg = spi_getreg(spi_bus, SPI_CONFIG_REG_OFS);

	switch (packets)
	{
	case 1:
		config_reg.bit.spi_pkt_cnt = SPI_PKT_CNT_ENUM_1PKT;
		break;
	case 2:
		config_reg.bit.spi_pkt_cnt = SPI_PKT_CNT_ENUM_2PKT;
		break;
	default:
		config_reg.bit.spi_pkt_cnt = SPI_PKT_CNT_ENUM_4PKT;
		break;
	}

	spi_setreg(spi_bus, SPI_CONFIG_REG_OFS, config_reg.reg);
}

static void spi_wait_tdre(struct nvt_spi* spi_bus)
{
	union SPI_STATUS_REG status_reg;

	while (1) {
		status_reg.reg = spi_getreg(spi_bus, SPI_STATUS_REG_OFS);
		if (status_reg.bit.spi_tdr_empty)
			break;
	}
}

static void spi_set_buffer(struct nvt_spi* spi_bus, u32 spi_buffer)
{
	union SPI_TDR_REG tdr_reg;

	tdr_reg.reg = spi_buffer;
	spi_setreg(spi_bus, SPI_TDR_REG_OFS, tdr_reg.reg);
}

static u32 spi_get_buffer(struct nvt_spi* spi_bus)
{
	return spi_getreg(spi_bus, SPI_RDR_REG_OFS);
}

static void spi_wait_rdrf(struct nvt_spi* spi_bus)
{
	union SPI_STATUS_REG status_reg;

	while (1) {
		status_reg.reg = spi_getreg(spi_bus, SPI_STATUS_REG_OFS);
		if (status_reg.bit.spi_rdr_full)
		break;
	}
}

/*
	Enable SPI DMA transfer

	This function can enable/disable SPI DMA function

	@param[in] dma_en   DMA mode selection
			- @b TRUE: enable DMA
			- @b FALSE: disable DMA

	@return void
*/
static void spi_enable_dma(struct nvt_spi *spi_bus, bool dma_en)
{
	union SPI_CTRL_REG ctrl_reg;
	union SPI_CONFIG_REG config_reg;
	union SPI_STATUS_REG status_reg = {0};

	config_reg.reg = spi_getreg(spi_bus, SPI_CONFIG_REG_OFS);

	if (dma_en) {
		union SPI_TIMING_REG timing_reg;

		timing_reg.reg = spi_getreg(spi_bus, SPI_TIMING_REG_OFS);

		if (timing_reg.bit.spi_post_cond_dly) {
			config_reg.bit.spi_pkt_burst_post_cond = 1;
			config_reg.bit.spi_pkt_burst_handshake_en = 1;
			spi_setreg(spi_bus, SPI_CONFIG_REG_OFS, config_reg.reg);
		}
	} else {
		config_reg.bit.spi_pkt_burst_post_cond = 0;
		config_reg.bit.spi_pkt_burst_handshake_en = 0;
		spi_setreg(spi_bus, SPI_CONFIG_REG_OFS, config_reg.reg);
	}

	status_reg.bit.dma_abort_sts = 1;
	spi_setreg(spi_bus, SPI_STATUS_REG_OFS, status_reg.reg);

	ctrl_reg.reg = spi_getreg(spi_bus, SPI_CTRL_REG_OFS);

	if (dma_en)
		ctrl_reg.bit.spi_dma_en = 1;
	else
		ctrl_reg.bit.spi_dma_dis = 1;

	spi_setreg(spi_bus, SPI_CTRL_REG_OFS, ctrl_reg.reg);
}

/*
	Set SPI DMA transfer count

	This function can set DMA transfer count.
	Count means needing to write SPI_TDR how many times.

	@param[in] count      transfer count. (unit: word)

	@return void
*/
static void spi_set_transfer_count(struct nvt_spi *spi_bus, u32 count)
{
	union SPI_DMA_BUFSIZE_REG dma_size_reg = {0};

	count<<=2;
	dma_size_reg.bit.spi_dma_bufsize = count;
	spi_setreg(spi_bus, SPI_DMA_BUFSIZE_REG_OFS, dma_size_reg.reg);
}

/*
	Set SPI DMA starting address

	This function can set DMA starting address.

	@param[in] addr       DMA starting address

	@return void
*/
static void spi_set_dma_address(struct nvt_spi *spi_bus, u32 addr)
{
	union SPI_DMA_STARTADDR_REG dma_addr_reg = {0};

	dma_addr_reg.bit.spi_dma_start_addr = virt_to_phys((u32*)addr);
	spi_setreg(spi_bus, SPI_DMA_STARTADDR_REG_OFS, dma_addr_reg.reg);
}

/*
	Set SPI DMA transfer direction

	This function can set DMA transfer direction (SPI->DRAM or DRAM->SPI).

	@param[in] dma_dir     DMA direction setting.
				- @b SPI_DMA_DIR_SPI2RAM
				- @b SPI_DMA_DIR_RAM2SPI
	@return void
*/
static void spi_set_dma_direction(struct nvt_spi *spi_bus, u32 dma_dir)
{
    union SPI_DMA_CTRL_REG dma_ctrl_reg = {0};

    dma_ctrl_reg.bit.dma_dir = dma_dir;
    spi_setreg(spi_bus, SPI_DMA_CTRL_REG_OFS, dma_ctrl_reg.reg);
}

/*
	Write data to dma

	@param[in] buffer    the dma buffer address
	@param[in] word_cnt  the transfer word count

	@return void
*/
static void spi_set_dma_write(struct nvt_spi *dev, u32 *buffer, u32 word_cnt)
{
	// Write-back cache to DRAM
	dma_addr_t dma_handle;

	dma_handle = dma_map_single(NULL, buffer, word_cnt*4, DMA_TO_DEVICE);
	dma_sync_single_for_device(NULL, dma_handle, word_cnt*4, DMA_TO_DEVICE);
	dma_unmap_single(NULL, dma_handle, word_cnt*4, DMA_TO_DEVICE);

	spi_enable_dma(dev, false);                   // disable DMA

	spi_set_transfer_count(dev, word_cnt);
	spi_set_dma_address(dev, (u32)dma_handle);
	spi_set_dma_direction(dev, SPI_DMA_DIR_RAM2SPI); // DRAM to SPI

	spi_enable_dma(dev, true);
}

/*
	Read data from dma

	@param[in] spiID        SPI controller ID
	@param[in] buffer    the dma buffer address
	@param[in] word_cnt  the transfer word count

	@return void
*/
static void spi_set_dma_read(struct nvt_spi *dev, u32 *buffer, u32 word_cnt)
{
	// Invalidate cache
	dma_addr_t dma_handle;

	dma_handle = dma_map_single(NULL, buffer, word_cnt*4, DMA_FROM_DEVICE);
	dma_sync_single_for_device(NULL, dma_handle, word_cnt*4, DMA_FROM_DEVICE);
	dma_unmap_single(NULL, dma_handle, word_cnt*4, DMA_FROM_DEVICE);

	spi_enable_dma(dev, false);                   // disable DMA

	spi_set_transfer_count(dev, word_cnt);
	spi_set_dma_address(dev, (u32)dma_handle);
	spi_set_dma_direction(dev, SPI_DMA_DIR_SPI2RAM); // SPI to DRAM

	spi_enable_dma(dev, true);
}

static void nvt_spi_transfer_polled(struct nvt_spi *dev,u32 count,
					const u8* tx_data, u8* rx_data)
{
	u32 val;

	spi_set_packet_count(dev, 4);

	for (; count >= 4; count-=4) {
		// Wait for transmit data register empty
		spi_wait_tdre(dev);

		// Send next word over the wire
		if (NULL != tx_data) {
			val = (tx_data[3]<<24) | (tx_data[2]<<16) \
				| (tx_data[1]<<8) | (tx_data[0]<<0);
			tx_data += 4;
		} else
			val = 0;

		spi_set_buffer(dev, val);

		// Wait for reveive data register full
		spi_wait_rdrf(dev);

		// Store received byte
		val = spi_get_buffer(dev);

		if (NULL != rx_data) {
			rx_data[0] = (val>>0) & 0xFF;
			rx_data[1] = (val>>8) & 0xFF;
			rx_data[2] = (val>>16) & 0xFF;
			rx_data[3] = (val>>24) & 0xFF;
			rx_data += 4;
		}
	}

	spi_set_packet_count(dev, 1);

	for (; count != 0; count--) {
		// Wait for transmit data register empty
		spi_wait_tdre(dev);

		// Send next word over the wire
		if (NULL != tx_data)
			val = *tx_data++;
		else
			val = 0;
		spi_set_buffer(dev, val);

		// Wait for reveive data register full
		spi_wait_rdrf(dev);

		// Store received byte
		val = spi_get_buffer(dev);

		if (NULL != rx_data)
			*rx_data++ = val;
	}
}

static irqreturn_t nvt_spi_irq(int irq, void *dev_id)
{
	struct nvt_spi *c = dev_id;

	u32 status_reg, inten_reg;

	/*Read the status register and check for transfer completition */
	HAL_READ_UINT32(c->base + SPI_STATUS_REG_OFS, status_reg);
	HAL_READ_UINT32(c->base + SPI_INTEN_REG_OFS, inten_reg);
	status_reg &= inten_reg;
	if (status_reg != 0) {
		/*Write 1 to clear interrupt status*/
		HAL_WRITE_UINT32(c->base + SPI_STATUS_REG_OFS, status_reg);
		HAL_READ_UINT32(c->base + SPI_STATUS_REG_OFS, status_reg);
		disable_irq_nosync (irq);
		complete(&c->done);
		return IRQ_HANDLED;
	} else
		return IRQ_NONE;
}



static void nvt_spi_transfer_dma(struct nvt_spi *dev, u32 count,
				const u8 *tx_data, u8 *rx_data)
{

	if(1) {
		nvt_spi_transfer_polled(dev, count, tx_data, rx_data);
		return;
	}

    /* 1. Handle heading non-word align parts */
	if (NULL != rx_data) {
		u32 byte_offset = ((u32)rx_data) & 0x03;

		if (byte_offset) {
			u32 length = sizeof(u32) - byte_offset;
			length = (length > count) ? count : length;

			nvt_spi_transfer_polled(dev, length, tx_data, rx_data);
			rx_data += length;
			count -= length;
		}
	} else {
		u32 byte_offset = ((u32)tx_data) & 0x03;

		if (byte_offset) {
			u32 length = sizeof(u32) - byte_offset;
			length = (length > count) ? count : length;

			nvt_spi_transfer_polled(dev, length, tx_data, rx_data);
			tx_data += length;
			count -= length;
		}
	}

	/* 2. Transfer (Address) Word align parts */

	spi_set_packet_count(dev, 4);
	while (count >= 16) {
		u32 tr_count = count > (SPI_DMA_MAX_LENGTH&(~0x03)) ?
				(SPI_DMA_MAX_LENGTH&(~0x03)) : (count&(~0x03));

		atomic_set(&dev->event, 1);

		if (NULL != rx_data)
			spi_set_dma_read(dev, (u32*)rx_data, tr_count/4);
		else //if (NULL != tx_data)
			spi_set_dma_write(dev, (u32*)tx_data, tr_count/4);


		/* Enable the SPI int events we are interested in*/
		if (tr_count > 4096) {

			/* Unmask the SPI int */
			enable_irq(dev->irq);

			/* Wait for its completition */
			wait_for_completion(&dev->done);
		} else {
			while (1) {
				u32 sts;
				HAL_READ_UINT32(dev->base + SPI_STATUS_REG_OFS, sts);
				if (sts & SPI_INT_MSK_DMAED) {
					HAL_WRITE_UINT32(dev->base + SPI_STATUS_REG_OFS,
						SPI_INT_MSK_DMAED);
					break;
				}
			}
		}

		// Disable the SPI interrupt
		/* Because SPI is shared between CPUs Disable SPI interrupt when idle */
		disable_irq(dev->irq);

		/* Adjust running variables */

		if (NULL != rx_data) {
			dma_addr_t dma_handle;

			dma_handle = dma_map_single(NULL, rx_data, tr_count, DMA_FROM_DEVICE);
			dma_sync_single_for_device(NULL, dma_handle, tr_count, DMA_FROM_DEVICE);
			dma_unmap_single(NULL, dma_handle, tr_count, DMA_FROM_DEVICE);

			rx_data += tr_count;
		}

		if (NULL != tx_data) {
			dma_addr_t dma_handle;

			dma_handle = dma_map_single(NULL, (void*)tx_data, tr_count, DMA_TO_DEVICE);
			dma_sync_single_for_device(NULL, dma_handle, tr_count, DMA_TO_DEVICE);
			dma_unmap_single(NULL, dma_handle, tr_count, DMA_TO_DEVICE);

			tx_data += tr_count;
		}

		count -= tr_count;
	}

	/* 3. Handle tailing non-word align parts */
	nvt_spi_transfer_polled(dev, count, tx_data, rx_data);
}


static void nvt_spi_work_one(struct nvt_spi *dev, struct spi_message *m)
{
	struct spi_device *spi = m->spi;
	struct spi_transfer *t;

	// Raise CS
	nvt_spi_set_npcs(dev, SPI_ACTIVE_CS);


	list_for_each_entry (t, &m->transfers, transfer_list) {
		const u8 *tx_data = (const u8*)t->tx_buf;
		u8 *rx_data = t->rx_buf;
		unsigned int count = t->len;
		u8 bits_per_word = t->bits_per_word ? : spi->bits_per_word;

		bits_per_word = bits_per_word ? : 8;

		if (count == 0)
			break;

		nvt_spi_transfer_dma(dev,count,tx_data,rx_data);
		m->actual_length += count;

		if(t->delay_usecs)
		udelay(t->delay_usecs);
	}

	m->status = 0; //TODO
	nvt_spi_set_npcs(dev,SPI_INACTIVE_CS);

	m->complete(m->context);
}

static void nvt_spi_work(struct work_struct *work)
{
	struct nvt_spi *c = container_of(work, struct nvt_spi, work);
	unsigned long flags;


	spin_lock_irqsave(&c->lock, flags);
	while (!list_empty(&c->queue)) {
		struct spi_message *m;

		m = container_of(c->queue.next, struct spi_message, queue);
		list_del_init(&m->queue);
		spin_unlock_irqrestore(&c->lock, flags);

		nvt_spi_work_one(c, m);

		spin_lock_irqsave(&c->lock, flags);
	}
	spin_unlock_irqrestore(&c->lock, flags);
}

static int nvt_spi_transfer(struct spi_device *spi, struct spi_message *m)
{
	struct spi_master *master = spi->master;
	struct nvt_spi *c = spi_master_get_devdata(master);
	struct spi_transfer *t;
	unsigned long flags;

	m->actual_length = 0;

	/* check each transfer's parameters */
	list_for_each_entry (t, &m->transfers, transfer_list) {
		u8 bits_per_word = t->bits_per_word ? : spi->bits_per_word;

		bits_per_word = bits_per_word ? : 8;
		if (!t->tx_buf && !t->rx_buf && t->len)
			return -EINVAL;

		if (bits_per_word != 8 && bits_per_word != 16)
			return -EINVAL;

		if (t->len & ((bits_per_word >> 3) - 1))
			return -EINVAL;
	}

	spin_lock_irqsave(&c->lock, flags);
	list_add_tail(&m->queue, &c->queue);
	queue_work(c->workqueue, &c->work);
	spin_unlock_irqrestore(&c->lock, flags);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id nvt_spi_of_dt_ids[] = {
	{ .compatible = "nvt,nvt_spi"},
	{},
};
MODULE_DEVICE_TABLE(of, nvt_spi_of_dt_ids);
#endif

static int nvt_spi_probe(struct platform_device *dev)
{
	struct spi_master *master;
	struct nvt_spi *c;
	int ret = -ENODEV;
	struct resource *res;
#ifdef CONFIG_OF
	u32 dev_id = 0;
#endif

	master = spi_alloc_master(&dev->dev, sizeof(*c));
	if (!master)
		return ret;

	c = spi_master_get_devdata(master);
	platform_set_drvdata(dev, master);


	INIT_WORK(&c->work, nvt_spi_work);
	spin_lock_init(&c->lock);
	INIT_LIST_HEAD(&c->queue);
	init_completion(&c->done);

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res) {
		goto exit;
	}
	c->base = devm_ioremap_resource(&dev->dev, res);
	if (IS_ERR(c->base)) {
		PTR_ERR(c->base);
		goto exit;
	}

	c->clk = devm_clk_get(&dev->dev, dev_name(&dev->dev));

	if (!IS_ERR(c->clk)) {
		clk_prepare(c->clk);
		clk_enable(c->clk);
	} else
		printk("%s: spi%d clk not found\n", __func__, dev->id);

	c->irq = platform_get_irq(dev, 0);
	if (c->irq < 0)
		goto exit_busy;

	ret = devm_request_irq(&dev->dev, c->irq, nvt_spi_irq,
			0, dev->name, c);
	disable_irq(c->irq);

	c->workqueue = create_singlethread_workqueue(dev_name(master->dev.parent));
	if (!c->workqueue)
		goto exit_busy;

	c->last_chipselect = -1;

	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CS_HIGH | SPI_CPOL | SPI_CPHA | SPI_LSB_FIRST;
#ifdef CONFIG_OF
	of_property_read_u32(dev->dev.of_node, "nvt-devname", &dev_id);
	master->bus_num = dev_id;
#else
	master->bus_num = dev->id;
#endif
	master->setup = nvt_spi_setup;
	master->transfer = nvt_spi_transfer;
	master->num_chipselect = (u16)UINT_MAX; /* any GPIO numbers */
#ifdef CONFIG_OF
	master->dev.of_node = dev->dev.of_node;
#endif

	ret = spi_register_master(master);

	if (ret)
		goto exit;

	return 0;
exit_busy:
	ret = -EBUSY;
exit:
	if (c->workqueue)
		destroy_workqueue(c->workqueue);
	//TODO: DISABLE CLOCK

	platform_set_drvdata(dev, NULL);
	spi_master_put(master);
	return ret;
}

static int __exit nvt_spi_remove(struct platform_device *dev)
{
	struct spi_master *master = spi_master_get(platform_get_drvdata(dev));
	struct nvt_spi *c = spi_master_get_devdata(master);

	spi_unregister_master(master);
	platform_set_drvdata(dev, NULL);
	destroy_workqueue(c->workqueue);

	//TODO: DISABLE CLOCK

	spi_master_put(master);
	return 0;
}

/* work with hotplug and coldplug */
MODULE_ALIAS("spi:nvt_spi");

static struct platform_driver nvt_spi_driver = {
	.probe = nvt_spi_probe,
	.remove = __exit_p(nvt_spi_remove),
	.driver = {
		.name = "nvt_spi",
		.owner = THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = nvt_spi_of_dt_ids,
#endif
	},
};

static int __init nvt_spi_init(void)
{
	int ret;

	ret = platform_driver_register(&nvt_spi_driver);
	if (ret < 0) {
		return ret;
	}
    return 0;
}
module_init(nvt_spi_init);

static void __exit nvt_spi_exit(void)
{
	platform_driver_unregister(&nvt_spi_driver);
}
module_exit(nvt_spi_exit);

MODULE_DESCRIPTION("NVT SPI Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.04.000");
