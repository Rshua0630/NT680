/**
    NVT mmc function
    NVT mmc driver
    @file       na51000_mmc.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include "na51000_mmchost.h"
#include "na51000_mmcreg.h"
#include <linux/sysfs.h>
#include <linux/platform_device.h>
#include <linux/mmc/sd.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#endif
/* For archs that don't support NO_IRQ (such as mips), provide a dummy value */
#ifndef NO_IRQ
#define NO_IRQ 0
#endif


#define SDIO_TR             no_printk /*pr_info*/
static unsigned rw_threshold = 32;
module_param(rw_threshold, uint, S_IRUGO);
MODULE_PARM_DESC(rw_threshold,
		"Read/Write threshold. Default = 32");

static unsigned __initdata use_dma = 1;
module_param(use_dma, uint, 0);
MODULE_PARM_DESC(use_dma, "Whether to use DMA or not. Default = 1");

extern u32 boot_pad_driving[24];


static u32 default_pad_driving[SDIO_MAX_MODE_DRIVING] =
{PAD_DRIVING_20MA, PAD_DRIVING_15MA, PAD_DRIVING_15MA, \
PAD_DRIVING_20MA, PAD_DRIVING_15MA, PAD_DRIVING_15MA, \
PAD_DRIVING_30MA, PAD_DRIVING_25MA, PAD_DRIVING_25MA, \
PAD_DRIVING_40MA, PAD_DRIVING_30MA, PAD_DRIVING_30MA};

#if 1  // for Brcm drv
static struct mmc_nvt_host* nvt_host[] = {NULL, NULL, NULL, NULL};
void nvt_rescan_card(unsigned id, unsigned insert)
{
	struct mmc_nvt_host *host = NULL;
	printk("%s Enter: in \n", __FUNCTION__);
	BUG_ON(id > 3);
	// BUG_ON(nvt_host[id] == NULL);
	if (nvt_host[id] == NULL){
		printk("%s nvt_host[%d] is null \n", __FUNCTION__, id);
		return;
	}
	host = nvt_host[id];
	host->mmc->rescan_entered = 0;
	mmc_detect_change(host->mmc, 0);
	return;
}
EXPORT_SYMBOL_GPL(nvt_rescan_card);
#endif

/* PIO only */
static void nvt_mmc_sg_to_buf(struct mmc_nvt_host *host)
{
	host->buffer_bytes_left = sg_dma_len(host->sg);
	host->buffer = sg_virt(host->sg);
	if (host->buffer_bytes_left > host->bytes_left)
		host->buffer_bytes_left = host->bytes_left;
}

static void nvt_mmc_fifo_data_trans(struct mmc_nvt_host *host,
					unsigned int n)
{
	if (host->data_dir == SDIO_HOST_WRITE_DATA) {
		sdiohost_writeblock(host, host->id, (uint8_t *)host->buffer,
		host->buffer_bytes_left);
		host->bytes_left -= host->buffer_bytes_left;
	} else {
		sdiohost_readblock(host, host->id, (uint8_t *)host->buffer,
		host->buffer_bytes_left);
		host->bytes_left -= host->buffer_bytes_left;
	}
}

static void nvt_mmc_start_command(struct mmc_nvt_host *host,
		struct mmc_command *cmd)
{
	u32 cmd_reg = 0;
	char *s;

	switch (mmc_resp_type(cmd)) {
	case MMC_RSP_R1:
		s = ", R1";
		break;
	case MMC_RSP_R1B:
		s = ", R1b";
		break;
	case MMC_RSP_R2:
		s = ", R2";
		break;
	case MMC_RSP_R3:
		s = ", R3/R4";
		break;
	default:
		s = ", Rx";
		break;
	};

	SDIO_TR("SDIO%d CMD%d, arg 0x%08x %s\n", host->id, cmd->opcode, cmd->arg, s);

	host->cmd = cmd;

	switch (mmc_resp_type(cmd)) {
	case MMC_RSP_R1B:
		/* There's some spec confusion about when R1B is
		 * allowed, but if the card doesn't issue a BUSY
		 * then it's harmless for us to allow it.
		 */
		/*need to check card busy CARD_BUSY2READY bit or
		 *send _SDIO_SD_SEND_STATUS to check
		*/
		cmd_reg |= SDIO_CMD_REG_NEED_RSP;
		/* FALLTHROUGH */
	case MMC_RSP_R1:		/* 48 bits, CRC */
		cmd_reg |= SDIO_CMD_REG_NEED_RSP;
		break;
	case MMC_RSP_R2:		/* 136 bits, CRC */
		cmd_reg |= SDIO_CMD_REG_LONG_RSP;
		break;
	case MMC_RSP_R3:		/* 48 bits, no CRC */
		cmd_reg |= SDIO_CMD_REG_NEED_RSP;
		break;
	default:
		cmd_reg |= 0;
		dev_dbg(mmc_dev(host->mmc), "unknown resp_type %04x\n",
			mmc_resp_type(cmd));
		break;
	}

	/* Set command index */
	cmd_reg |= cmd->opcode;

	sdiohost_sendsdcmd(host, host->id, cmd_reg, cmd->arg);

}

/*----------------------------------------------------------------------*/
static void
nvt_mmc_prepare_data(struct mmc_nvt_host *host, struct mmc_request *req)
{
	/*int fifo_lev = (rw_threshold == 32) ? MMCFIFOCTL_FIFOLEV : 0;*/
	unsigned int timeout;
	struct mmc_data *data = req->data;
	int sg_len;

	host->data = data;
	host->data_early = 0;

	if (data == NULL)
		return;


	dev_dbg(mmc_dev(host->mmc), "%s %s, %d blocks of %d bytes\n",
		(data->flags & MMC_DATA_STREAM) ? "stream" : "block",
		(data->flags & MMC_DATA_WRITE) ? "write" : "read",
		data->blocks, data->blksz);
	dev_dbg(mmc_dev(host->mmc),
		"DTO %d cycles+ %d ns, %d ns_in_one_cycle\n",
		data->timeout_clks, data->timeout_ns, host->ns_in_one_cycle);

	timeout = data->timeout_clks +
		(data->timeout_ns / host->ns_in_one_cycle);


	sdiohost_setdatatimeout(host, host->id, timeout);
	sdiohost_setblksize(host, host->id, data->blksz);

	host->buffer = NULL;
	host->bytes_left = data->blocks * data->blksz;
	host->sg_len = data->sg_len;
	host->sg = host->data->sg;
	nvt_mmc_sg_to_buf(host);
	host->data_dir = ((data->flags & MMC_DATA_WRITE) ?
		SDIO_HOST_WRITE_DATA : SDIO_HOST_READ_DATA);

	if (host->use_dma) {
		#ifdef SDIO_SCATTER_DMA
		int i = 0;

		/*if (data->sg_len != 1)
		    printk("sg_len=%d\r\n", data->sg_len);*/

		memset((void *)&host->tmpdestable[0], 0,
			sizeof(struct STRG_SEG_DES) * SDIO_DES_TABLE_NUM);

		if (host->data_dir)
			sg_len = dma_map_sg(mmc_dev(host->mmc), data->sg,
					data->sg_len, DMA_FROM_DEVICE);
		else
			sg_len = dma_map_sg(mmc_dev(host->mmc), data->sg,
					data->sg_len, DMA_TO_DEVICE);

		for (i = 0; i < sg_len; i++) {
			host->tmpdestable[i].uisegaddr = sg_dma_address(&data->sg[i]);
			host->tmpdestable[i].uisegsize = sg_dma_len(&data->sg[i]);
		}

		/*if (host->data_dir)
			dma_sync_sg_for_device(mmc_dev(host->mmc), data->sg,
					sg_len, DMA_FROM_DEVICE);
		else
			dma_sync_sg_for_device(mmc_dev(host->mmc), data->sg,
					sg_len, DMA_TO_DEVICE);*/

		/*for (i = 0; i < data->sg_len; i++) {
			tmpdestable[i].uisegaddr =
			(uint32_t)sg_virt(&data->sg[i]);
			tmpdestable[i].uisegsize = sg_dma_len(&data->sg[i]);

		}*/
		sdiohost_setdestab(host->id, (uint32_t)host->tmpdestable, sg_len,
		(uint32_t *)host->vuisdio_destab);
		sdiohost_setdesen(host->id, 1);
		/*printk("tab addr 0x%x\r\n", (uint32_t)host->vuisdio_destab);*/
		/*printk("tab addr 0x%x, size %d\r\n", tmpdestable[0].uisegaddr,
			tmpdestable[0].uisegsize);*/
		#endif
		/*printk("dirty byte 0x%x\r\n", host->buffer[0x41]);*/

		host->do_dma = 1;
		sdiohost_setupdatatransferdma(host, host->id,
		(uint32_t)host->buffer, data->blocks * data->blksz,
		host->data_dir, (uint32_t *)host->vuisdio_destab);
		/* zero this to ensure we take no PIO paths */
		host->bytes_left = 0;
		/*printk("dma addr 0x%x, size %d\r\n", host->buffer,
			data->blocks * data->blksz);*/

	} else {
		host->do_dma = 0;
		/*printk("pio addr 0x%x, size %d\r\n", host->buffer,
			data->blocks * data->blksz);*/
		/*printk("dirty byte 0x%x\r\n", host->buffer[0x41]);*/

		sdiohost_setupdatatransferpio(host, host->id,
		(uint32_t)host->buffer,
		data->blocks * data->blksz, host->data_dir);
	}

}

static void nvt_mmc_request(struct mmc_host *mmc, struct mmc_request *req)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);

	unsigned long timeout = jiffies + msecs_to_jiffies(900);
	u32 mmcst1 = 0;

	if ((clk_get_phase(host->clk)) && (host->id == SDIO_HOST_ID_1) && \
		(req->cmd->opcode == SD_SWITCH_VOLTAGE))
		clk_set_phase(host->clk, 0);

	/* Card may still be sending BUSY after a previous operation,
	 * typically some kind of write.  If so, we can't proceed yet.
	 */
	if (req->data) {
		while (time_before(jiffies, timeout)) {
			mmcst1  = sdiohost_getrdy(host, host->id);
			if (mmcst1 == true)
				break;
			cpu_relax();
		}

		if (mmcst1 == false) {
			dev_err(mmc_dev(host->mmc), "still BUSY...\n");
			req->cmd->error = -ETIMEDOUT;
			mmc_request_done(mmc, req);
			return;
		}
	}

	nvt_mmc_prepare_data(host, req);
	nvt_mmc_start_command(host, req->cmd);

}

static void nvt_mmc_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);

	dev_dbg(mmc_dev(host->mmc),
		"set_ios clock %dHz busmode %d powermode %d Vdd %04x\n",
		ios->clock, ios->bus_mode, ios->power_mode,
		ios->vdd);

	/*The power state should be controlled by NVT instead of MMC flows*/
	if (ios->clock == 0)
		return;

	switch (ios->bus_width) {
	case MMC_BUS_WIDTH_8:
		dev_dbg(mmc_dev(host->mmc), "Enabling 8 bit mode\n");
		sdiohost_setbuswidth(host, host->id, SDIO_BUS_WIDTH8);
		break;
	case MMC_BUS_WIDTH_4:
		dev_dbg(mmc_dev(host->mmc), "Enabling 4 bit mode\n");
		sdiohost_setbuswidth(host, host->id, SDIO_BUS_WIDTH4);
		break;
	case MMC_BUS_WIDTH_1:
		dev_dbg(mmc_dev(host->mmc), "Enabling 1 bit mode\n");
		sdiohost_setbuswidth(host, host->id, SDIO_BUS_WIDTH1);
		break;
	}

	if (ios->clock > SDIO_MODE_SDR50) {
		sdiohost_setpaddriving(host, SDIO_MODE_SDR104);
		sdiohost_delay_chain(host, 0x10, SDIO_DLY_SDR104_DEFAULT);
	} else if (ios->clock > SDIO_MODE_HS) {
		sdiohost_setpaddriving(host, SDIO_MODE_SDR50);
		sdiohost_delay_chain(host, 0x200010, SDIO_DLY_SDR50_DEFAULT);
	} else if (ios->clock > SDIO_MODE_DS) {
		sdiohost_setpaddriving(host, SDIO_MODE_HS);
		sdiohost_delay_chain(host, 0x200000, SDIO_DLY_HS_DEFAULT);
	} else {
		sdiohost_setpaddriving(host, SDIO_MODE_DS);
		sdiohost_delay_chain(host, 0x200000, SDIO_DLY_DS_DEFAULT);
	}

	host->bus_mode = ios->bus_mode;
	sdiohost_setbusclk(host, host->id, ios->clock,
	(uint32_t *)&host->ns_in_one_cycle);
}

static void
nvt_mmc_xfer_done(struct mmc_nvt_host *host, struct mmc_data *data)
{
	host->data = NULL;

	if (!data->stop || (host->cmd && host->cmd->error)) {
		/*SDIO_TR("nvt_mmc_xfer_done: mmc_request_done\r\n");*/
		mmc_request_done(host->mmc, data->mrq);
	} else {
		/*SDIO_TR("nvt_mmc_xfer_done: start command\r\n");*/
		nvt_mmc_start_command(host, data->stop);
	}

	if (host->data_dir)
		dma_unmap_sg(mmc_dev(host->mmc), data->sg, data->sg_len,
				DMA_FROM_DEVICE);
	else
		dma_unmap_sg(mmc_dev(host->mmc), data->sg, data->sg_len,
				DMA_TO_DEVICE);

}

static void nvt_mmc_cmd_done(struct mmc_nvt_host *host,
				 struct mmc_command *cmd)
{
	struct mmc_data *data = host->data;
	host->cmd = NULL;

	if (cmd->flags & MMC_RSP_PRESENT) {
		if (cmd->flags & MMC_RSP_136) {
			/* response type 2 */
			sdiohost_getlongrsp(host, host->id,
			(uint32_t *)&cmd->resp[0], (uint32_t *)&cmd->resp[1],
			(uint32_t *)&cmd->resp[2], (uint32_t *)&cmd->resp[3]);
			SDIO_TR("SDIO%d LRsp %x %x %x %x\r\n", host->id,
			cmd->resp[0], cmd->resp[1], cmd->resp[2], cmd->resp[3]);
		} else {
			/* response types 1, 1b, 3, 4, 5, 6 */
			sdiohost_getshortrsp(host, host->id,
			(uint32_t *)&cmd->resp[0]);
			SDIO_TR("SDIO%d SRsp %x \r\n", host->id, cmd->resp[0]);
		}
	}

	if (host->data && host->data_early)
		/*SDIO_TR("nvt_mmc_cmd_done: data early\r\n");*/
		nvt_mmc_xfer_done(host, data);
	if (host->data == NULL)
		mmc_request_done(host->mmc, cmd->mrq);
	else if (cmd->error) {
		if ((cmd->error == -ETIMEDOUT)) {
			cmd->mrq->cmd->retries = 0;
			mmc_request_done(host->mmc, cmd->mrq);
		} else if ((cmd->error == -EILSEQ)) {
			cmd->mrq->cmd->retries = 0;
			if (host->data == NULL)
				mmc_request_done(host->mmc, cmd->mrq);
		}
	}
}

static inline void nvt_mmc_reset_ctrl(struct mmc_nvt_host *host)
{
	sdiohost_resetdata(host, host->id);
	sdiohost_reset(host, host->id);
}

static void
nvt_mmc_abort_data(struct mmc_nvt_host *host, struct mmc_data *data)
{
	pr_info("SDIO%d nvt_mmc_abort_data %c\r\n", host->id, \
		host->data_dir ? 'R':'W');

	nvt_mmc_reset_ctrl(host);
	dma_unmap_sg(mmc_dev(host->mmc), data->sg, data->sg_len,
				host->data_dir);
}

static irqreturn_t mmc_irq_cd(int irq, void *dev_id)
{
	struct mmc_nvt_host *host = (struct mmc_nvt_host *)dev_id;

	mmc_detect_change(host->mmc, msecs_to_jiffies(200));

	return IRQ_HANDLED;
}

static irqreturn_t nvt_mmc_irq(int irq, void *dev_id)
{
	struct mmc_nvt_host *host = (struct mmc_nvt_host *)dev_id;
	unsigned int status, qstatus;
	int end_command = 0;
	int end_transfer = 0;
	struct mmc_data *data = host->data;
	u32 tuning_cmd = sdiohost_getcmd(host);

	status = sdiohost_getstatus(host, host->id);
	qstatus = status;
	SDIO_TR("<6>isr %x\r\n", qstatus);

	if (sdiohost_getiointen(host, host->id) && (qstatus & MMCST_SDIO_INT)) {
		sdiohost_setstatus(host, host->id, MMCST_SDIO_INT);
		mmc_signal_sdio_irq(host->mmc);
		return IRQ_HANDLED;
	}

	/*pr_info("<6>isr %x\r\n",qstatus);*/

	/*writel(qstatus,host->base + NVT_MMCST0);*/
	/*sdiohost_setstatus(host, host->id, qstatus);*/




	if (qstatus & MMCST_RSP_TIMEOUT) {
		/* Command timeout */
		if (host->cmd) {
			dev_dbg(mmc_dev(host->mmc),
				"CMD%d timeout, status %x\n",
				host->cmd->opcode, qstatus);
			host->cmd->error = -ETIMEDOUT;
			if (data) {
				end_transfer = 1;
				end_command = 1;
				nvt_mmc_abort_data(host, data);
				data->bytes_xfered = 0;
				host->data->error = -ETIMEDOUT;
				/*printk("SDIO%d DATA MMCST_RSP_TIMEOUT\r\n", host->id);*/
			} else
				end_command = 1;
		}
		/*printk("SDIO%d MMCST_RSP_TIMEOUT\r\n", host->id);*/
		sdiohost_setstatus(host, host->id, MMCST_RSP_TIMEOUT);
	}

	if (qstatus & MMCST_DATA_TIMEOUT) {
		dev_dbg(mmc_dev(host->mmc),
			"data timeout (CMD%d), status %x\n",
			host->cmd->opcode, qstatus);
		if (host->data) {
			host->data->error = -ETIMEDOUT;
			end_transfer = 1;
			nvt_mmc_abort_data(host, data);
		}
		/*printk("SDIO%d MMCST_DATA_TIMEOUT\r\n", host->id);*/
		sdiohost_setstatus(host, host->id, MMCST_DATA_TIMEOUT);
	}

	if (qstatus & MMCST_RSP_CRC_FAIL) {
		/* Command CRC error */
		dev_dbg(mmc_dev(host->mmc), "Command CRC error\n");
		if (host->cmd) {
			host->cmd->error = -EILSEQ;
			end_command = 1;
		}
		/*printk("SDIO%d MMCST_RSP_CRC_FAIL\r\n", host->id);*/
		sdiohost_setstatus(host, host->id, MMCST_RSP_CRC_FAIL);
	}

	if (qstatus & MMCST_DATA_CRC_FAIL) {
		/* Data CRC error */
		end_transfer = 1;
		dev_dbg(mmc_dev(host->mmc), "data %s %s error\n",
			(data->flags & MMC_DATA_WRITE) ? "write" : "read",
			(data->error == -ETIMEDOUT) ? "timeout" : "CRC");

		nvt_mmc_abort_data(host, data);
		/*printk("SDIO%d MMCST_DATA_CRC_FAIL\r\n", host->id);*/
		sdiohost_setstatus(host, host->id, MMCST_DATA_CRC_FAIL);
	}

	if ((qstatus & MMCST_RSP_CRC_OK) || (qstatus & MMCST_CMD_SENT)) {
		/* End of command phase */
		if (data == NULL)
			end_command = (int) host->cmd;
		else {
			if ((host->do_dma == 0) && (host->bytes_left > 0)) {
				/* if datasize < rw_threshold
				 * no RX ints are generated
				 */
				nvt_mmc_fifo_data_trans(host, host->bytes_left);
			}
		}
		if (qstatus & MMCST_RSP_CRC_OK)
			sdiohost_setstatus(host, host->id, MMCST_RSP_CRC_OK);
		if (qstatus & MMCST_CMD_SENT)
			sdiohost_setstatus(host, host->id, MMCST_CMD_SENT);
	}

	if ((qstatus & MMCST_RSP_CRC_OK))
		end_command = (int) host->cmd;

	if ((qstatus & MMCST_DATA_END) || (qstatus & MMCST_DATA_CRC_OK)) {
		end_transfer = 1;
		if (tuning_cmd != MMC_SEND_TUNING_BLOCK)
			data->bytes_xfered = data->blocks * data->blksz;

		if (qstatus & MMCST_DATA_CRC_FAIL)
			data->bytes_xfered = 0;

		if (qstatus & MMCST_DATA_END) {
			sdiohost_setstatus(host, host->id, MMCST_DATA_END);
			if (tuning_cmd == MMC_SEND_TUNING_BLOCK)
				complete(&host->tuning_data_end);
		}

		if (qstatus & MMCST_DATA_CRC_OK)
			sdiohost_setstatus(host, host->id, MMCST_DATA_CRC_OK);

	}

	if (qstatus & (MMCST_VOL_SWITCH_END | MMCST_VOL_SWITCH_TIMEOUT)) {
		if (qstatus & MMCST_VOL_SWITCH_TIMEOUT) {
			host->voltage_switch_timeout = 1;
			sdiohost_setstatus(host, host->id, \
				MMCST_VOL_SWITCH_TIMEOUT);
		}

		if (qstatus & MMCST_VOL_SWITCH_END) {
			sdiohost_setstatus(host, host->id, \
				MMCST_VOL_SWITCH_END);
		}
		complete(&host->voltage_switch_complete);
	}

	if (end_command)
		nvt_mmc_cmd_done(host, host->cmd);

	if (end_transfer) {
		if (host->cmd) {
			/*
			 * Data managed to finish before the
			 * command completed. Make sure we do
			 * things in the proper order.
			 */
			host->data_early = 1;
			/*printk("<7>isr data early, end_c=%d, end_d=%d\r\n",
			end_command, end_transfer);*/

		} else {
			if (tuning_cmd != MMC_SEND_TUNING_BLOCK)
				nvt_mmc_xfer_done(host, data);
		}
	}

	return IRQ_HANDLED;

}

static int nvt_mmc_get_cd(struct mmc_host *mmc)
{
	struct platform_device *pdev = to_platform_device(mmc->parent);
	struct nvt_mmc_config *config = pdev->dev.platform_data;

	if (!config || !config->get_cd)
		return -ENOSYS;
	return config->get_cd(mmc);
}

static int nvt_mmc_get_ro(struct mmc_host *mmc)
{
	struct platform_device *pdev = to_platform_device(mmc->parent);
	struct nvt_mmc_config *config = pdev->dev.platform_data;

	if (!config || !config->get_ro)
		return -ENOSYS;
	return config->get_ro(mmc);
}

static void nvt_mmc_enable_sdio_irq(struct mmc_host *mmc, int enable)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);

	if (enable) {
		unsigned int status = sdiohost_getstatus(host, host->id);
		/*pr_info("%x\r\n",status);*/
		if (status & MMCST_SDIO_INT) {
			sdiohost_setstatus(host, host->id, MMCST_SDIO_INT);
			status = sdiohost_getstatus(host, host->id);
			sdiohost_setstatus(host, host->id,
				status & MMCST_SDIO_INT);
			mmc_signal_sdio_irq(host->mmc);
		} else {
			host->sdio_int = true;
			sdiohost_setiointen(host, host->id, TRUE);
			sdiohost_getiointen(host, host->id);
		}
	} else {
		host->sdio_int = false;
		sdiohost_setiointen(host, host->id, FALSE);
		sdiohost_getiointen(host, host->id);
	}
}

static int nvt_start_signal_voltage_switch(struct mmc_host *mmc,
	struct mmc_ios *ios)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);
	int ret;

	switch (ios->signal_voltage) {
	case MMC_SIGNAL_VOLTAGE_330:
		if (sdiohost_getpower(host) == SDIO_HOST_SETPOWER_VOL_3P3)
			return 0;

		sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_3P3);

		return 0;
	case MMC_SIGNAL_VOLTAGE_180:
		if (sdiohost_getpower(host) == SDIO_HOST_SETPOWER_VOL_1P8)
			return 0;

		sdiohost_disclockout(host, host->id);

		sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_1P8);

		mdelay(400);

		sdiohost_enclockout(host, host->id);
		mdelay(20);

		ret = sdiohost_set_voltage_switch(host);

		if (!(clk_get_phase(host->clk)) && (host->id == SDIO_HOST_ID_1))
			clk_set_phase(host->clk, 1);

		return ret;
	default:
		/* No signal voltage switch required */
		return 0;
	}
}

static int nvt_card_busy(struct mmc_host *mmc)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);
	u32 status = 0;
	/*
	 * Check the busy bit which is low when DAT[3:0]
	 * (the data lines) are 0000
	 */

	status = sdiohost_getdata_status(host, host->id);

	return !(status & SDIO_DATA_LVL_MASK);
}

static void nvt_mmc_check_edge(u32 ch, u32 *zeropos, u32 *zerolength)
{
	u32 mask = 2;
	u32 max_length = 0;
	u32 max_pos = 0;
	u32 this_length = 0;
	u32 this_pos = 0;
	u32 pos;

	for (pos = 1; pos < 16; pos++) {
		if ((ch & mask) == 0) {
			if (this_length == 0) {
				this_pos = pos;
			}
			this_length++;
		} else {
			if (this_length > max_length) {
				max_length = this_length;
				max_pos = this_pos;
			}
			this_length = 0;
		}
		mask = mask << 1;
	}

	if ((max_length == 0) || (this_length > max_length)) {
		max_pos = this_pos;
		max_length = this_length;
	}

	*zeropos = max_pos;
	*zerolength = max_length;
}


static int nvt_mmc_tuning(struct mmc_host *mmc, u32 opcode)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);
	int result;
	u32 detout, phase_sel, length, pos;

	sdiohost_setphyclrdetval(host);

	init_completion(&host->tuning_data_end);

	result = sdiohost_tuning_cmd(host, opcode);

	if (result) {
		pr_err("SDIO%d Tuning Cmd Fail\r\n", host->id);
		return result;
	}

	detout = sdiohost_getphydetout(host) & 0xFFFF;

	nvt_mmc_check_edge(detout, &pos, &length);

	if (length < 5) {
		clk_set_rate(host->clk, clk_get_rate(host->clk) >> 1);

		sdiohost_setphyclrdetval(host);

		result = sdiohost_tuning_cmd(host, opcode);

		if (result) {
			pr_err("SDIO%d Tuning Cmd Fail\r\n", host->id);
			return result;
		}

		detout = sdiohost_getphydetout(host) & 0xFFFF;

		nvt_mmc_check_edge(detout, &pos, &length);

		if (length < 5) {
			pr_warn("SDIO%d detect out zero count %d < 5\n",
					host->id, length);
			return E_OK;
		}
	}

	phase_sel = pos + (length >> 1);
	sdiohost_setdlyphase_sel(host, phase_sel);
	sdiohost_setphyphase_cmpen(host, (1 << phase_sel));

	return 0;
}

static struct mmc_host_ops nvt_mmc_ops = {
	.request	= nvt_mmc_request,
	.set_ios	= nvt_mmc_set_ios,
	.get_cd		= nvt_mmc_get_cd,
	.get_ro		= nvt_mmc_get_ro,
	.enable_sdio_irq = nvt_mmc_enable_sdio_irq,
	.start_signal_voltage_switch = nvt_start_signal_voltage_switch,
	.execute_tuning = nvt_mmc_tuning,
	.card_busy	= nvt_card_busy,
};

static struct mmc_host_ops nvt_fixed_mmc_ops = {
	.request	= nvt_mmc_request,
	.set_ios	= nvt_mmc_set_ios,
	.get_cd		= nvt_mmc_get_cd,
	.get_ro		= nvt_mmc_get_ro,
	.enable_sdio_irq = nvt_mmc_enable_sdio_irq,
};

/*----------------------------------------------------------------------*/


static inline int nvt_mmc_cpufreq_register(struct mmc_nvt_host *host)
{
	return 0;
}

static inline void nvt_mmc_cpufreq_deregister(struct mmc_nvt_host *host)
{
}

static void __init init_mmcsd_host(struct mmc_nvt_host *host, int voltage_switch)
{
	sdiohost_open(host, host->id, voltage_switch);
}

static int nvt_mmc_get_cd_polled(struct mmc_host *mmc)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);

	/*printk("<6>nvt_mmc_get_cd! 1\n");*/
	if (host->cd_gpio) {
		if (host->power_en) {
			if (gpio_get_value(host->power_en) != host->power_detect_edge) {
				sdiohost_power_switch(host, 0);
				return 0;
			} else
				sdiohost_power_switch(host, 1);
		}

		return (gpio_get_value(host->cd_gpio) == host->cd_detect_edge ? 1 : 0);
	} else
		return 1;
}

static int nvt_mmc_get_ro_polled(struct mmc_host *mmc)
{
	struct mmc_nvt_host *host = mmc_priv(mmc);

	/* printk("<6>nvt_mmc_get_ro! 0\n"); */
	if (host->ro_gpio)
		return (gpio_get_value(host->ro_gpio) == host->ro_detect_edge ? 1 : 0);
	else
		return 0;
}

/* rescan property */
static ssize_t nvtmmc_sysfs_set_rescan(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf,
				       size_t count)
{
	int is_rescan = 0;
	struct mmc_nvt_host *host = NULL;

	host = dev_get_drvdata(dev);
	if (!host)
		return -EINVAL;

	if (kstrtoint(buf, 10, &is_rescan) < 0)
		return -EINVAL;

	if (is_rescan < 2)
		nvt_rescan_card(host->id, is_rescan);
	else
		return -EINVAL;

	return count;
}

static DEVICE_ATTR(rescan, S_IWUSR | S_IRUGO,
		NULL, nvtmmc_sysfs_set_rescan);

static struct attribute *nvtmmc_sysfs_attributes[] = {
	&dev_attr_rescan.attr,
	NULL,
};

static const struct attribute_group nvtmmc_sysfs_attr_group = {
	.attrs = nvtmmc_sysfs_attributes,
};

static int nvt_mmc_sysfs_init(struct device *dev)
{
	return sysfs_create_group(&dev->kobj, &nvtmmc_sysfs_attr_group);
}

static struct nvt_mmc_config nvt_mmc_config_data = {
	.get_cd     = nvt_mmc_get_cd_polled,
	.get_ro     = nvt_mmc_get_ro_polled,
	.wires      = 4,
	.max_freq   = 96000000,
	.caps       = MMC_CAP_MMC_HIGHSPEED | MMC_CAP_SD_HIGHSPEED,
};

static struct nvt_mmc_config nvt_mmc2_config_data = {
	.get_cd     = nvt_mmc_get_cd_polled,
	.get_ro     = nvt_mmc_get_ro_polled,
	.wires      = 4,
	.max_freq   = 192000000,
	.caps       = MMC_CAP_MMC_HIGHSPEED | MMC_CAP_SD_HIGHSPEED,
};

static struct nvt_mmc_config nvt_mmc3_config_data = {
	.get_cd     = nvt_mmc_get_cd_polled,
	.get_ro     = nvt_mmc_get_ro_polled,
	.wires      = 4,
	.max_freq   = 48000000,
	.caps       = MMC_CAP_MMC_HIGHSPEED | MMC_CAP_SD_HIGHSPEED,
};

static const struct of_device_id nvt_mmcsd_of_dt_ids[] = {
	{ .compatible = "nvt,nvt_mmc", .data = &nvt_mmc_config_data },
	{ .compatible = "nvt,nvt_mmc2", .data = &nvt_mmc2_config_data },
	{ .compatible = "nvt,nvt_mmc3", .data = &nvt_mmc3_config_data },
	{},
};

MODULE_DEVICE_TABLE(of, nvt_mmcsd_of_dt_ids);

static int __init nvt_mmcsd_probe(struct platform_device *pdev)
{
	struct mmc_nvt_host *host = NULL;
	struct mmc_host *mmc = NULL;
	struct resource *r, *mem = NULL;
	int ret = 0, irq = 0, i;
	u32 cd_gpio[3] = {0}, ro_gpio[3] = {0}, power_en[2] = {0};
	u32 card_power_gpio[2] = {0};
	u32 pad_driving[SDIO_MAX_MODE_DRIVING] = {0};
	size_t mem_size;
	u32 voltage_switch = 0, max_voltage = 0;
	u32 config_pad_value = (*(u32*) 0xFD29001C) & 0x10;

#ifndef CONFIG_OF
	struct nvt_mmc_config *pdata = pdev->dev.platform_data;
#else
	struct nvt_mmc_config *pdata = NULL;
	const struct of_device_id *of_id;

	/* REVISIT:  when we're fully converted, fail if pdata is NULL */
	of_id = of_match_device(nvt_mmcsd_of_dt_ids, &pdev->dev);
	if (!of_id) {
		dev_err(&pdev->dev, "[NVT MMC] OF not found\n");
		return -EINVAL;
	}
	pdata = (struct nvt_mmc_config *) of_id->data;
	pdev->dev.platform_data = pdata;

	of_property_read_u32(pdev->dev.of_node, "voltage-switch",
				&voltage_switch);

	of_property_read_u32(pdev->dev.of_node, "max-voltage",
				&max_voltage);

	of_property_read_u32_array(pdev->dev.of_node, "cd_gpio", cd_gpio, 3);

	of_property_read_u32_array(pdev->dev.of_node, "ro_gpio", ro_gpio, 3);

	of_property_read_u32_array(pdev->dev.of_node, "power_en", power_en, 2);

	of_property_read_u32_array(pdev->dev.of_node, \
					"card_power_gpio", card_power_gpio, 2);
#endif

	ret = -ENODEV;
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!r || irq == NO_IRQ)
		goto out;

	ret = -EBUSY;
	mem_size = resource_size(r);
	mem = request_mem_region(r->start, mem_size, pdev->name);
	if (!mem)
		goto out;


	ret = -ENOMEM;
	mmc = mmc_alloc_host(sizeof(struct mmc_nvt_host), &pdev->dev);
	if (!mmc)
		goto out;

	host = mmc_priv(mmc);
	host->mmc = mmc;	/* Important */
	host->mem_res = mem;
	host->base = ioremap(mem->start, mem_size);/*mem->start;*/
	if (!host->base)
		goto out;

	if (host->base == NVT_SDIO1_BASE_VIRT) {
		host->id = SDIO_HOST_ID_1;
		if (max_voltage < VOLTAGE_3300)
			sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_1P8);
		else
			sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_3P3);
	} else if (host->base == NVT_SDIO2_BASE_VIRT) {
		host->id = SDIO_HOST_ID_2;
		if (max_voltage < VOLTAGE_3300) {
			sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_1P8);
			if (!(config_pad_value))
				pr_err("%s: Wrong SDIO2 pad power, "
						"force to 1800mV\n", __func__);
		} else {
			sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_3P3);
			if (config_pad_value)
				pr_err("%s: Wrong SDIO2 pad power, "
						"force to 3300mV\n", __func__);
		}
	} else
		host->id = SDIO_HOST_ID_3;

	/*printk("host id = %d\r\n", host->id);*/

	ret = -ENXIO;

	host->clk = clk_get(&pdev->dev, dev_name(&pdev->dev));
	if (!IS_ERR(host->clk)) {
		clk_prepare(host->clk);
		clk_enable(host->clk);
	} else {
		pr_err("%s: %s not found\n", __func__, dev_name(&pdev->dev));
		goto out;
	}

	for (i = 0; i < SDIO_MAX_MODE_DRIVING; i++) {
		pad_driving[i] = boot_pad_driving[i];
		if (pad_driving[0])
			host->pad_driving[i] = pad_driving[i];
		else
			host->pad_driving[i] = default_pad_driving[i];
	}

	init_mmcsd_host(host , voltage_switch);

	host->use_dma = use_dma;
	host->mmc_irq = irq;
	host->mmc_cd_irq = gpio_to_irq(cd_gpio[0]);
	host->cd_gpio = cd_gpio[0];
	host->cd_detect_edge = cd_gpio[1];
	host->ro_gpio = ro_gpio[0];
	host->ro_detect_edge = ro_gpio[1];
	host->power_en = power_en[0];
	host->power_detect_edge = power_en[1];
	host->cp_gpio = card_power_gpio[0];
	host->cp_gpio_value = card_power_gpio[1];
	host->voltage_switch = voltage_switch;

	/*Enable card power if card power was defined*/
	if (host->cp_gpio) {
		gpio_direction_output(host->cp_gpio, host->cp_gpio_value);
	}

	mmc->f_min = 312500;
	mmc->f_max = 24000000;
#ifndef CONFIG_OF
	if (pdata && (pdata->wires == 4 || pdata->wires == 0))
		mmc->caps |= MMC_CAP_4_BIT_DATA;

	if (pdata && (pdata->wires == 8))
		mmc->caps |= (MMC_CAP_4_BIT_DATA | MMC_CAP_8_BIT_DATA);

	if (pdata && pdata->max_freq)
		mmc->f_max = pdata->max_freq;
#else
	mmc_of_parse(host->mmc);
#endif

	/* REVISIT:  someday, support IRQ-driven card detection.  */
	/*mmc->caps |= MMC_CAP_NEEDS_POLL;*/

	/*mmc->caps |= MMC_CAP_WAIT_WHILE_BUSY;*/

	mmc->caps |= MMC_CAP_SDIO_IRQ;

	host->version = pdata->version;

	ret = nvt_mmc_sysfs_init(&pdev->dev);
	if (ret < 0) {
		pr_err("mmc sysfs can't be created.\n");
	}

	if (voltage_switch && (max_voltage > VOLTAGE_1800)) {
		mmc->ops = &nvt_mmc_ops;
		host->max_voltage = SDIO_HOST_SETPOWER_VOL_3P3;
	} else {
		mmc->ops = &nvt_fixed_mmc_ops;
		if (max_voltage > VOLTAGE_1800)
			host->max_voltage = SDIO_HOST_SETPOWER_VOL_3P3;
		else
			host->max_voltage = SDIO_HOST_SETPOWER_VOL_1P8;
	}

	if (pdata && pdata->caps)
		mmc->caps |= pdata->caps;
	mmc->ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
	mmc->ocr_avail_sdio = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;

	if(voltage_switch) {
		mmc->caps |= MMC_CAP_UHS_SDR12 | MMC_CAP_UHS_SDR25 | \
			MMC_CAP_UHS_SDR50;

		if (mmc->f_max > SDIO_MODE_SDR50)
			mmc->caps |= MMC_CAP_UHS_SDR104;
	}

#if SDIO_SCATTER_DMA
	mmc->max_segs		= SDIO_DES_TABLE_NUM;/*SDIO_DES_TABLE_NUM;*/
#else
	mmc->max_segs		= 1 + host->n_link;
#endif

	/*mmc->max_seg_size	= (32*1024);*/
	/* MMC/SD controller limits for multiblock requests */
	mmc->max_blk_size	= 2048;
	mmc->max_blk_count	= (32*1024);
	mmc->max_req_size	= mmc->max_blk_size * mmc->max_blk_count;
	mmc->max_seg_size	= mmc->max_req_size;
	dev_dbg(mmc_dev(host->mmc), "max_segs=%d\n", mmc->max_segs);
	dev_dbg(mmc_dev(host->mmc), "max_blk_size=%d\n", mmc->max_blk_size);
	dev_dbg(mmc_dev(host->mmc), "max_req_size=%d\n", mmc->max_req_size);
	dev_dbg(mmc_dev(host->mmc), "max_seg_size=%d\n", mmc->max_seg_size);
#ifdef CONFIG_NVT_WIFI_BRCM_PM
	if (host->id == SDIO_HOST_ID_2) {
		mmc->pm_caps |= MMC_PM_KEEP_POWER ;
		mmc->caps |= MMC_CAP_NONREMOVABLE;
	}
#endif
	if (cd_gpio[2]) {
		if (cd_gpio[0]) {
			if(gpio_is_valid(host->mmc_cd_irq)) {
				request_irq(host->mmc_cd_irq, mmc_irq_cd,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "mmc_cd", host);
			} else
				mmc->caps |= MMC_CAP_NEEDS_POLL;
		}
	} else
		mmc->caps |= MMC_CAP_NEEDS_POLL;

	platform_set_drvdata(pdev, host);
	#if 1  // for Brcm drv
	nvt_host[host->id] = host;
	#endif
	ret = nvt_mmc_cpufreq_register(host);
	if (ret) {
		dev_err(&pdev->dev, "failed to register cpufreq\n");
		goto cpu_freq_fail;
	}
	ret = mmc_add_host(mmc);
	if (ret < 0)
		goto out;

	ret = request_irq(irq, nvt_mmc_irq, 0, mmc_hostname(mmc), host);
	if (ret)
		goto out;

	if(voltage_switch)
		init_completion(&host->voltage_switch_complete);

	rename_region(mem, mmc_hostname(mmc));

	dev_info(mmc_dev(host->mmc), "Using %s, %d-bit mode\n",
		host->use_dma ? "DMA" : "PIO",
		(mmc->caps & MMC_CAP_8_BIT_DATA) ? 8 :
		(mmc->caps & MMC_CAP_4_BIT_DATA) ? 4 : 1);

	return 0;

out:
	nvt_mmc_cpufreq_deregister(host);
cpu_freq_fail:
	if (host) {
		if (host->clk) {
			clk_disable(host->clk);
			clk_put(host->clk);
		}

		if (host->base)
			iounmap(host->base);
	}

	#if 1 // for Brcm drv
	nvt_host[host->id] = NULL;
	#endif

	if (mmc)
		mmc_free_host(mmc);

	if (mem)
		release_mem_region(mem->start, mem_size);

	dev_dbg(&pdev->dev, "probe err %d\n", ret);

	return ret;
}

static int __exit nvt_mmcsd_remove(struct platform_device *pdev)
{
	struct mmc_nvt_host *host = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	if (host) {
		nvt_mmc_cpufreq_deregister(host);

		mmc_remove_host(host->mmc);
		free_irq(host->mmc_irq, host);

		if (gpio_is_valid(host->mmc_cd_irq))
			free_irq(gpio_to_irq(host->mmc_cd_irq), host);

		clk_disable(host->clk);
		clk_put(host->clk);

		iounmap(host->base);

		release_mem_region(host->mem_res->start, resource_size(host->mem_res));

		#if 1  // for Brcm drv
		nvt_host[host->id] = NULL;
		#endif

		mmc_free_host(host->mmc);
	}

	return 0;
}

#ifdef CONFIG_PM
static int nvt_mmcsd_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mmc_nvt_host *host = platform_get_drvdata(pdev);

	sdiohost_resetdata(host, host->id);
	clk_disable(host->clk);
	disable_irq(host->mmc_irq);
	sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_0);

	return 0;
}

static int nvt_mmcsd_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mmc_nvt_host *host = platform_get_drvdata(pdev);

	clk_enable(host->clk);
	msleep(50);
	sdiohost_setpower(host, host->max_voltage);
	init_mmcsd_host(host, host->voltage_switch);
	enable_irq(host->mmc_irq);

	return 0;
}

static const struct dev_pm_ops nvt_mmcsd_pm = {
	.suspend        = nvt_mmcsd_suspend,
	.resume         = nvt_mmcsd_resume,
};

#define nvt_mmcsd_pm_ops (&nvt_mmcsd_pm)
#else
#define nvt_mmcsd_pm_ops NULL
#endif

static struct platform_driver nvt_mmcsd_driver = {
	.probe		= nvt_mmcsd_probe,
	.remove		= __exit_p(nvt_mmcsd_remove),
	.driver		= {
		.name	= "nvt_mmc",
		.owner	= THIS_MODULE,
		.pm	= nvt_mmcsd_pm_ops,
#ifdef CONFIG_OF
		.of_match_table = nvt_mmcsd_of_dt_ids,
#endif
	},
};

static int __init nvt_mmcsd_init(void)
{
	return platform_driver_register(&nvt_mmcsd_driver);
}
module_init(nvt_mmcsd_init);

static void __exit nvt_mmcsd_exit(void)
{
	platform_driver_unregister(&nvt_mmcsd_driver);
}

module_exit(nvt_mmcsd_exit);

MODULE_AUTHOR("Novatek");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MMC/SD driver for NOVATEK MMC controller");
MODULE_ALIAS("nvt_mmc");
MODULE_VERSION("1.05.050");

