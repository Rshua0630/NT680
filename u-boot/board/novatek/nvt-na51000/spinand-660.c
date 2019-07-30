/*
 *  board/novatek/evb660/spinand-660.c
 *
 *  Author:	Howard Chang
 *  Created:	Feb 3, 2015
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <malloc.h>
#include <linux/err.h>
#if defined(CONFIG_CMD_NAND)
#include <spi.h>
#include <nand.h>
#include "spinand.h"
#include <linux/spi/RCWMacro.h>
#include <linux/mtd/nand.h>
#include <asm/arch/cache.h>

#define BUFSIZE (2048 + 64)
#define SPI_NAND_WAIT_TIMEOUT 22000000  	/*ns*/
static int enable_hw_ecc;
static int enable_read_hw_ecc;
static float cpu_freq_tick = 0;


static struct nand_ecclayout spinand_oob_64 = {
	.eccbytes = 32,
	.eccpos = {
		8, 9, 10, 11, 12, 13, 14, 15,
		24, 25, 26, 27, 28, 29, 30, 31,
		40, 41, 42, 43, 44, 45, 46, 47,
		56, 57, 58, 59, 60, 61, 62, 63},
	.oobavail = 30,
	.oobfree = {
		{.offset = 2,
			.length = 6},
		{.offset = 16,
			.length = 8},
		{.offset = 32,
			.length = 8},
		{.offset = 48,
			.length = 8},
	}
};

static struct nand_ecclayout spinand_oob_gd = {
#ifdef CONFIG_NVT_JFFS2_SUPPORT
        .eccbytes = 64,
        .eccpos = {
		64, 65, 66, 67, 68, 69, 70, 71,
		72, 73, 74, 75, 76, 77, 78, 79,
		80, 81, 82, 83, 84, 85, 86, 87,
		88, 89, 90, 91, 92, 93, 94, 95,
		96, 97, 98, 99, 100, 101, 102, 103,
		104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119,
		120, 121, 122, 123, 124, 125, 126, 127},
	.oobavail = 12,
	.oobfree = {
		{.offset = 16, .length = 4},
		{.offset = 32, .length = 4},
		{.offset = 48, .length = 4},
	}
#else
	.oobavail = 62,
	.oobfree = {
		{.offset = 2, .length = 62},
	}
#endif
};

static int spi_flash_read_write(struct spi_slave *spi,
				const u8 *cmd, size_t cmd_len,
				const u8 *data_out, u8 *data_in,
				size_t data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		printf("SF: Failed to send command (%zu bytes): %d\n",
				cmd_len, ret);
	} else if (data_len != 0) {
		ret = spi_xfer(spi, data_len * 8, data_out, data_in, SPI_XFER_END);
		if (ret)
			printf("SF: Failed to transfer %zu bytes of data: %d\n",
					data_len, ret);
	}

	return ret;
}


static void spinand_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	//Howard debug
//	printf("%s\n",__func__);
}

static int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, NULL, data, data_len);
}
/*
static int spi_flash_cmd_write(struct spi_slave *spi, const u8 *cmd, size_t cmd_len,
		const void *data, size_t data_len)
{
	return spi_flash_read_write(spi, cmd, cmd_len, data, NULL, data_len);
}
*/

static int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, size_t len)
{
	return spi_flash_cmd_read(spi, &cmd, 1, response, len);
}

static void spinand_read_id(struct spi_slave *spi, u8 *id)
{
	int ret;
	u8 id_buf[3];

	ret = spi_flash_cmd(spi, CMD_READ_ID, id_buf, sizeof(id_buf));
	if (ret)
	{
		spi_release_bus(spi);
		printf("spi_flash_cmd fail\n");
	}
	id[0] = id_buf[1];
	id[1] = id_buf[2];

	//printf("spi_flash_id[%02x][%02x]\n", id[0], id[1]);

}


/*
 * spinand_read_status- send command 0xf to the SPI Nand status register
 * Description:
 *    After read, write, or erase, the Nand device is expected to set the
 *    busy status.
 *    This function is to allow reading the status of the command: read,
 *    write, and erase.
 *    Once the status turns to be ready, the other status bits also are
 *    valid status bits.
 */
static int spinand_read_status(struct spi_slave *spi, uint8_t *status)
{
	struct spinand_cmd cmd = {0};
	int ret;

	cmd.n_addr = 2;
	cmd.addr[0] = CMD_READ_REG;
	cmd.addr[1] = REG_STATUS;
	cmd.n_rx = 1;
	cmd.rx_buf = status;

	ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_BEGIN);
	if (ret < 0)
		printf("err: %d read status register\n", ret);

	ret = spi_xfer(spi, cmd.n_rx * 8, NULL, cmd.rx_buf, SPI_XFER_END);
	if (ret < 0)
		printf("err: %d get status\n", ret);

	return ret;
}

static inline struct spinand_state *mtd_to_state(struct mtd_info *mtd)
{
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	struct spinand_state *state = (struct spinand_state *)info->priv;

	return state;
}

static int wait_till_ready(struct spi_slave *spi_nand)
{
	int retval;
	u8 stat = 0;
	u32 timeo = (CONFIG_SYS_HZ * 20) / 1000;
	u32 time_start;

	time_start = get_timer(0);

	/* wait until command is processed or timeout occures */
	while (get_timer(time_start) < timeo) {
		retval = spinand_read_status(spi_nand, &stat);
		if (retval < 0)
			return -1;
		else if (!(stat & 0x1))
			break;
	}

	if ((stat & 0x1) == 0)
		return 0;

	return -1;
}

static int spinand_unlockBP(struct spi_slave *spi)
{
	struct spinand_cmd cmd = {0};
	int ret = 0;

	if(!spi->BlockUnlocked)
	{
		cmd.cmd = CMD_WRITE_REG;
		cmd.n_addr = 2;
		cmd.addr[0] = REG_BLOCK_LOCK;
		cmd.addr[1] = 0x00;
		cmd.n_rx = 0;
		cmd.rx_buf = 0;


		ret = spi_xfer(spi, 8, &cmd.cmd, NULL, SPI_XFER_BEGIN);
		if (ret < 0)
			printf("err: %d send read command\n", ret);

		ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_END);
		if (ret < 0)
			printf("err: %d read status register\n", ret);

		if (wait_till_ready(spi))
		{
			printf("WAIT timedout!!!\n");
			ret = -ETIMEDOUT;
		}
		else
		{
			spi->BlockUnlocked = 1;
		}
	}
	return ret;
}

/**
 * spinand_get_otp- send command 0xf to read the SPI Nand OTP register
 * Description:
 *   There is one bit( bit 0x10 ) to set or to clear the internal ECC.
 *   Enable chip internal ECC, set the bit to 1
 *   Disable chip internal ECC, clear the bit to 0
 */
static int spinand_get_otp(struct spi_slave *spi, u8 *otp)
{
	struct spinand_cmd cmd = {0};
	int ret;

	cmd.n_addr = 2;
	cmd.addr[0] = CMD_READ_REG;
	cmd.addr[1] = REG_OTP;
	cmd.n_rx = 1;
	cmd.rx_buf = otp;

	ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_BEGIN);
	if (ret < 0)
		printf("err: %d send cache register\n", ret);

	ret = spi_xfer(spi, cmd.n_rx * 8, NULL, cmd.rx_buf, SPI_XFER_END);
	if (ret < 0)
		printf("err: %d get status\n", ret);

	return ret;
}

#ifndef CONFIG_NVT_SPINAND_HWECC
/**
 * spinand_set_otp- send command 0x1f to write the SPI Nand OTP register
 * Description:
 *   There is one bit( bit 0x10 ) to set or to clear the internal ECC.
 *   Enable chip internal ECC, set the bit to 1
 *   Disable chip internal ECC, clear the bit to 0
 */
static int spinand_set_otp(struct spi_slave *spi, u8 *otp)
{
	int retval;
	struct spinand_cmd cmd = {0};

	cmd.n_addr = 2;
	cmd.addr[0] = CMD_WRITE_REG;
	cmd.addr[1] = REG_OTP,
	cmd.n_tx = 1,
	cmd.tx_buf = otp,

	retval = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_BEGIN);
	if (retval < 0)
		printf("err: %d send cache register\n", retval);

	retval = spi_xfer(spi, cmd.n_tx * 8, cmd.tx_buf, NULL, SPI_XFER_END);
	if (retval < 0)
		printf("err: %d get status\n", retval);

	return retval;
}

static int spinand_disable_ecc(struct spi_slave *spi)
{
	int retval;
	u8 otp = 0;

	retval = spinand_get_otp(spi, &otp);
	if (retval < 0)
		return retval;

	if ((otp & OTP_ECC_MASK) == OTP_ECC_MASK) {
		otp &= ~OTP_ECC_MASK;
		retval = spinand_set_otp(spi, &otp);
		if (retval < 0)
			return retval;

		wait_till_ready(spi);
		retval = spinand_get_otp(spi, &otp);
		return retval;
	}

	return 0;
}
#endif

/**
 * spinand_write_enable- send command 0x06 to enable write or erase the
 * Nand cells
 * Description:
 *   Before write and erase the Nand cells, the write enable has to be set.
 *   After the write or erase, the write enable bit is automatically
 *   cleared (status register bit 2)
 *   Set the bit 2 of the status register has the same effect
 */
static int spinand_write_enable(struct spi_slave *spi_nand)
{
	u8 cmd = CMD_WR_ENABLE;

	return spi_flash_cmd(spi_nand, cmd, NULL, 0);
}

static int spinand_read_page_to_cache(struct spi_slave *spi, u16 page_id)
{
	struct spinand_cmd cmd = {0};
	u16 row;
	int ret;

	row = page_id;
	cmd.cmd = CMD_READ;
	cmd.n_addr = 3;
	cmd.addr[1] = (u8)((row & 0xff00) >> 8);
	cmd.addr[2] = (u8)(row & 0x00ff);

	ret = spi_xfer(spi, 8, &cmd.cmd, NULL, SPI_XFER_BEGIN);
	if (ret < 0)
		printf("err: %d send read command\n", ret);

	ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_END);
	if (ret < 0)
		printf("err: %d get status\n", ret);

	return ret;
}

/*
 * spinand_read_from_cache- send command 0x03 to read out the data from the
 * cache register(2112 bytes max)
 * Description:
 *   The read can specify 1 to 2112 bytes of data read at the corresponding
 *   locations.
 *   No tRd delay.
 */
static int spinand_read_from_cache(struct spi_slave *spi, u16 page_id,
		u16 byte_id, u16 len, u8 *rbuf)
{
	struct spinand_cmd cmd = {0};
	u16 column;
	int ret;

	column = byte_id;
	cmd.n_addr = 4;
	cmd.addr[0] = CMD_READ_RDM;
	cmd.addr[1] = (u8)((column & 0xff00) >> 8);
	cmd.addr[1] |= (u8)(((page_id >> 6) & 0x1) << 4);
	cmd.addr[2] = (u8)(column & 0x00ff);
	cmd.addr[3] = (u8)(0xff);
	cmd.n_dummy = 0;
	cmd.n_rx = len;
	cmd.rx_buf = rbuf;

	ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_BEGIN);
	if (ret < 0)
		printf("err: %d send cache register\n", ret);

	//ret = spi_xfer(spi, cmd.n_dummy * 8, &dummy, NULL, NULL);
	//if (ret < 0)
	//	printf("err: %d send dummy register\n", ret);

	ret = spi_xfer(spi, cmd.n_rx * 8, NULL, cmd.rx_buf, SPI_XFER_END);
	if (ret < 0)
		printf("err: %d get status\n", ret);

	return ret;
}

/*
 * spinand_read_page-to read a page with:
 * @page_id: the physical page number
 * @offset:  the location from 0 to 2111
 * @len:     number of bytes to read
 * @rbuf:    read buffer to hold @len bytes
 *
 * Description:
 *   The read includes two commands to the Nand: 0x13 and 0x03 commands
 *   Poll to read status to wait for tRD time.
 */

static int spinand_read_page(struct spi_slave *spi_nand, u16 page_id,
		u16 offset, u16 len, u8 *rbuf)
{
	int ret;
	u8 status = 0;

	ret = spinand_read_page_to_cache(spi_nand, page_id);
	if (ret < 0)
		return ret;
/*
	if (wait_till_ready(spi_nand))
		printf("WAIT timedout!!!\n");
*/
	while (1) {
		ret = spinand_read_status(spi_nand, &status);
		if (ret < 0) {
			printf("err %d read status register\n", ret);
			return ret;
		}

		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			if ((status & STATUS_ECC_MASK) == STATUS_ECC_ERROR) {
				printf("ecc error, page=%d\n", page_id);
			}
			break;
		}
	}

	//inv_dcache_range((uint32_t)rbuf, len);

	ret = spinand_read_from_cache(spi_nand, page_id, offset, len, rbuf);
	if (ret < 0) {
		printf("read from cache failed!!\n");
		return ret;
	}

	return ret;
}

/*
 * spinand_program_data_to_cache--to write a page to cache with:
 * @byte_id: the location to write to the cache
 * @len:     number of bytes to write
 * @rbuf:    read buffer to hold @len bytes
 *
 * Description:
 *   The write command used here is 0x84--indicating that the cache is
 *   not cleared first.
 *   Since it is writing the data to cache, there is no tPROG time.
 */
static int spinand_program_data_to_cache(struct spi_slave *spi,
		u16 page_id, u16 byte_id, u16 len, u8 *wbuf)
{
	struct spinand_cmd cmd = {0};
	u16 column;
	int ret;

	column = byte_id;
	cmd.n_addr = 3;
	cmd.addr[0] = CMD_PROG_PAGE_CLRCACHE;
	cmd.addr[1] = (u8)((column & 0xff00) >> 8);
	cmd.addr[1] |= (u8)(((page_id >> 6) & 0x1) << 4);
	cmd.addr[2] = (u8)(column & 0x00ff);
	cmd.n_tx = len;
	cmd.tx_buf = wbuf;

	ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_BEGIN);
	if (ret < 0)
		printf("err: %d send program data register\n", ret);

	ret = spi_xfer(spi, cmd.n_tx * 8, cmd.tx_buf, NULL, SPI_XFER_END);
	if (ret < 0)
		printf("err: %d get status\n", ret);

	return ret;
}

/**
 * spinand_program_execute--to write a page from cache to the Nand array with
 * @page_id: the physical page location to write the page.
 *
 * Description:
 *   The write command used here is 0x10--indicating the cache is writing to
 *   the Nand array.
 *   Need to wait for tPROG time to finish the transaction.
 */
static int spinand_program_execute(struct spi_slave *spi, u16 page_id)
{
	struct spinand_cmd cmd = {0};
	u16 row;
	int ret;

	row = page_id;
	cmd.cmd = CMD_PROG_PAGE_EXC;
	cmd.n_addr = 3;
	cmd.addr[1] = (u8)((row & 0xff00) >> 8);
	cmd.addr[2] = (u8)(row & 0x00ff);

	ret = spi_xfer(spi, 8, &cmd.cmd, NULL, SPI_XFER_BEGIN);
	if (ret < 0)
		printf("err: %d send program_execute command\n", ret);

	ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_END);
	if (ret < 0)
		printf("err: %d send cache register\n", ret);

	return ret;
}

/**
 * spinand_program_page--to write a page with:
 * @page_id: the physical page location to write the page.
 * @offset:  the location from the cache starting from 0 to 2111
 * @len:     the number of bytes to write
 * @wbuf:    the buffer to hold the number of bytes
 *
 * Description:
 *   The commands used here are 0x06, 0x84, and 0x10--indicating that
 *   the write enable is first sent, the write cache command, and the
 *   write execute command.
 *   Poll to wait for the tPROG time to finish the transaction.
 */
static int spinand_program_page(struct spi_slave *spi_nand,
		u16 page_id, u16 offset, u16 len, u8 *buf)
{
	int retval;
	u8 status = 0;
	uint8_t *wbuf;

	wbuf = buf;

	retval = spinand_write_enable(spi_nand);
	if (retval < 0) {
		printf("write enable failed!!\n");
		return retval;
	}
	if (wait_till_ready(spi_nand))
		printf("wait timedout!!!\n");

	//wb_dcache_range((uint32_t)wbuf, len);

	retval = spinand_program_data_to_cache(spi_nand, page_id,
			offset, len, wbuf);
	if (retval < 0)
		return retval;
	retval = spinand_program_execute(spi_nand, page_id);
	if (retval < 0)
		return retval;
	while (1) {
		retval = spinand_read_status(spi_nand, &status);
		if (retval < 0) {
			printf("error %d reading status register\n", retval);
			return retval;
		}

		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			if ((status & STATUS_P_FAIL_MASK) == STATUS_P_FAIL) {
				printf("program error, page %d\n", page_id);
				return -1;
			}
			break;
		}
	}

	return 0;
}

/**
 * spinand_erase_block_erase--to erase a page with:
 * @block_id: the physical block location to erase.
 *
 * Description:
 *   The command used here is 0xd8--indicating an erase command to erase
 *   one block--64 pages
 *   Need to wait for tERS.
 */
static int spinand_erase_block_erase(struct spi_slave *spi, u16 block_id)
{
	struct spinand_cmd cmd = {0};
	u16 row;
	int ret;

	row = block_id;
	cmd.cmd = CMD_ERASE_BLK;
	cmd.n_addr = 3;
	cmd.addr[1] = (u8)((row & 0xff00) >> 8);
	cmd.addr[2] = (u8)(row & 0x00ff);

	ret = spi_xfer(spi, 8, &cmd.cmd, NULL, SPI_XFER_BEGIN);
	if (ret < 0)
		printf("err: %d send erase block command\n", ret);

	ret = spi_xfer(spi, cmd.n_addr * 8, cmd.addr, NULL, SPI_XFER_END);
	if (ret < 0)
		printf("err: %d send erase register\n", ret);

	return ret;
}

/**
 * spinand_erase_block--to erase a page with:
 * @block_id: the physical block location to erase.
 *
 * Description:
 *   The commands used here are 0x06 and 0xd8--indicating an erase
 *   command to erase one block--64 pages
 *   It will first to enable the write enable bit (0x06 command),
 *   and then send the 0xd8 erase command
 *   Poll to wait for the tERS time to complete the tranaction.
 */
static int spinand_erase_block(struct spi_slave *spi_nand, u16 block_id)
{
	int retval;
	u8 status = 0;

	spinand_unlockBP(spi_nand);

	/* Send write enable command */
	retval = spinand_write_enable(spi_nand);
	if (wait_till_ready(spi_nand))
		printf("wait timedout!!!\n");

	retval = spinand_erase_block_erase(spi_nand, block_id);
	while (1) {
		retval = spinand_read_status(spi_nand, &status);
		if (retval < 0) {
			printf("error %d reading status register\n",(int) retval);
			return retval;
		}

		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			if ((status & STATUS_E_FAIL_MASK) == STATUS_E_FAIL) {
				printf("erase error, block %d\n", block_id);
				return -1;
			}
			break;
		}
	}
	return 0;
}

/*
 * spinand_reset- send RESET command "0xff" to the Nand device.
 */
static void spinand_reset(struct spi_slave *spi_nand)
{
	u8 cmd = CMD_RESET;

	if (spi_flash_cmd(spi_nand, cmd, NULL, 0) < 0)
		printf("spinand reset failed!\n");

	/* elapse 1ms before issuing any other command */
	udelay(1000);

	if (wait_till_ready(spi_nand))
		printf("wait timedout!\n");
}

static void spinand_cmdfunc(struct mtd_info *mtd, unsigned int command,
		int column, int page)
{
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct spinand_info *info = (struct spinand_info *)chip->priv;
	struct spinand_state *state = (struct spinand_state *)info->priv;

	switch (command) {
	/*
	 * READ0 - read in first  0x800 bytes
	 */
	case NAND_CMD_READ1:
	case NAND_CMD_READ0:
		state->buf_ptr = 0;
		
		spinand_read_page(info->spi, page, 0x0, 0x840, state->buf);
		
		break;
	/* READOOB reads only the OOB because no ECC is performed. */
	case NAND_CMD_READOOB:
		state->buf_ptr = 0;
		
		spinand_read_page(info->spi, page, 0x800, 0x40, state->buf);
		
		break;
	case NAND_CMD_RNDOUT:
		state->buf_ptr = column;
		break;
	case NAND_CMD_READID:
		state->buf_ptr = 0;
		
		spinand_read_id(info->spi, (u8 *)state->buf);
		
		break;
	case NAND_CMD_PARAM:
		state->buf_ptr = 0;
		break;
	/* ERASE1 stores the block and page address */
	case NAND_CMD_ERASE1:
		
		spinand_erase_block(info->spi, page);
		
		break;
	/* ERASE2 uses the block and page address from ERASE1 */
	case NAND_CMD_ERASE2:
		break;
	/* SEQIN sets up the addr buffer and all registers except the length */
	case NAND_CMD_SEQIN:
		state->col = column;
		state->row = page;
		state->buf_ptr = 0;
		break;
	/* PAGEPROG reuses all of the setup from SEQIN and adds the length */
	case NAND_CMD_PAGEPROG:
		
		spinand_program_page(info->spi, state->row, state->col,
				state->buf_ptr, state->buf);
		
		break;
	case NAND_CMD_STATUS:
		
		spinand_get_otp(info->spi, state->buf);
		if (!(state->buf[0] & 0x80))
			state->buf[0] = 0x80;
		state->buf_ptr = 0;
		
		break;
	/* RESET command */
	case NAND_CMD_RESET:
		
		spinand_reset(info->spi);
		
		break;
	default:
		printf("Unknown CMD: 0x%x\n", command);
	}
}

static void spinand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{

	struct spinand_state *state = mtd_to_state(mtd);


	
	memcpy(state->buf + state->buf_ptr, buf, len);
	state->buf_ptr += len;
	
}

static void spinand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct spinand_state *state = mtd_to_state(mtd);

	
	memcpy(buf, state->buf + state->buf_ptr, len);
	
	state->buf_ptr += len;
}

static uint8_t spinand_read_byte(struct mtd_info *mtd)
{
	struct spinand_state *state = mtd_to_state(mtd);
	u8 data;

	
	data = state->buf[state->buf_ptr];
	
	state->buf_ptr++;
	return data;
}

static int spinand_wait(struct mtd_info *mtd, struct nand_chip *chip)
{
	struct spinand_info *info = (struct spinand_info *)chip->priv;

	unsigned long timeo;
	u8 status;
	//u32 time_start;
#if 0
	if (state == FL_ERASING)
		timeo = (CONFIG_SYS_HZ * 400) / 1000;
	else
		timeo = (CONFIG_SYS_HZ * 20) / 1000;

	timeo = (timeo << 2);
#else
	timeo = (unsigned long) cpu_freq_tick;


#endif
	//chip->cmdfunc(mtd, NAND_CMD_STATUS, -1, -1);

	//time_start = get_timer(0);

	
	while (1) {
		if (get_timer(0) > timeo) {
			printf("Timeout!");
			return 0x01;
		}
 		//if (chip->read_byte(mtd) & NAND_STATUS_READY)
		//	break;

		// if (chip->dev_ready) {
			// if (chip->dev_ready(mtd))
				// break;
		// } else {
			// if (chip->read_byte(mtd) & NAND_STATUS_READY)
				// break;
		// }
		spinand_read_status(info->spi, &status);
		if ((status & NAND_STATUS_FAIL) == NAND_STATUS_FAIL) {
			
			return status;
		}
		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			
			return 0;
		}
	}

	
	return 0;
	//return chip->read_byte(mtd);
}


static int spinand_write_page_hwecc(struct mtd_info *mtd,
		struct nand_chip *chip, const uint8_t *buf)
{
	const uint8_t *p = buf;
	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;

	enable_hw_ecc = 1;
	chip->write_buf(mtd, p, eccsize * eccsteps);
	return 0;
}

static int spinand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
		uint8_t *buf, int page)
{
	u8 status;
	uint8_t *p = buf;
	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;
	struct spinand_info *info = (struct spinand_info *)chip->priv;

	enable_read_hw_ecc = 1;

	chip->read_buf(mtd, p, eccsize * eccsteps);
	
	while (1) {
		spinand_read_status(info->spi, &status);
		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			if ((status & STATUS_ECC_MASK) == STATUS_ECC_ERROR) {
				printf("spinand: ECC error\n");
				printf("page 0x%x\n", page);
				mtd->ecc_stats.failed++;
				
				return -EBADMSG;
			} else if ((status & STATUS_ECC_MASK) ==
					STATUS_ECC_1BIT_CORRECTED)
				mtd->ecc_stats.corrected++;
			break;
		}
	}
	
	return 0;

}

/*
static void nvt_scan_spinand_type(struct spinand_info *info, int32_t* id)
{
	int maf_id;
	int dev_id;
//	struct spinand_state *state = mtd_to_state(mtd);


	maf_id = (*id) & 0xFF;
	dev_id = (*id >> 8) & 0xFF;

	// switch(dev_id) {
		// case NAND_256M_2K:
			// f->page_per_block	= 64;
			// f->page_size		= 2048;
			// f->oob_size		= 64;
			// f->subpage_size		= 512;
			// f->flash_width		= 8;
			// f->nfc_width		= 8;
			// f->num_blocks		= 2048;
			// f->chip_id		= *id;
			// f->phy_page_ratio	= 4;
			// f->device_size		= 256;
			// f->chip_sel		= 0;
			// f->block_size		= 0x20000;
			// break;
		// default:
			// printf("Cannot find the parameter of dev_id 0x%x\n", dev_id);

	// }
//	info->flash_info = f;
}
*/

/*
static void emu_dspDumpData(uint32_t startAddress, uint32_t len)
{
    UINT32 index;
    UINT32 reg;
    printf("\r\n");
    printf("[0x%08x] ", startAddress);
    for(index = 0; index < len; index+=4)
    {
        if((index % 16) == 0 && index > 0)
        {
            printf("\r\n");
            printf("[0x%08x] ", startAddress + index);
        }
        reg = INW((startAddress + index));
        printf("0x%08x ", reg);
    }
}
*/
int nvt_spinand_board_nand_init(struct nand_chip *nand)
{
	int	rat0,rat1,rat2;
	int	pllRatio;
	//float	cpu_freq_tick;
	struct spinand_state *state;
	struct spi_slave * spi_dev;
	static struct spinand_info *info;

/*
	rat2 = *(volatile unsigned long *)0xF00211B0;
	rat1 = *(volatile unsigned long *)0xF00211AC;
	rat0 = *(volatile unsigned long *)0xF00211A8;
	pllRatio = 12 * (((rat2 << 16) | (rat1 << 8) | (rat0 << 0)) / 131072);


	cpu_freq_tick = (float)2000  / (float)pllRatio;

	printf(">CPU @ %u MHz, tick = %f\n", pllRatio, cpu_freq_tick);

	cpu_freq_tick = (SPI_NAND_WAIT_TIMEOUT / cpu_freq_tick);
*/
	cpu_freq_tick = 1000;
	
	spi_nt96680_init();
	
	info = malloc(sizeof(struct spinand_info));
	if (!info) {
		printf("alloc spinand_info failed!\n");
		return -ENOMEM;
	}

	nand->priv = info;

	state = malloc(sizeof(struct spinand_state));
	if (!state) {
		printf("alloc spinand_state failed!\n");
		goto fail_free_info;
	}

	spi_dev = malloc(sizeof(struct spi_slave));
	if (!spi_dev) {
		printf("alloc spi_dev failed!\n");
		goto fail_free_state;
	}

	spi_dev->BlockUnlocked = 0;


	info->priv = state;
	info->spi = spi_dev;
	state->buf_ptr = 0;
	state->buf = malloc(BUFSIZE);
	if (!state->buf) {
		printf("alloc buf failed\n");
		return -ENOMEM;
	}

	spinand_read_id(info->spi, state->buf);

	//nvt_scan_spinand_type(info, state->buf);
	/*
	for(test = 0; test < 0x840; test++)
	{
		state->buf[test] = 0x5a + test;
	}

	spinand_erase_block(info->spi, 0);
	spinand_program_page(info->spi, 0, 0, 0x840, state->buf);

	spinand_read_page(info->spi, 0, 0, 0x840, state->buf);
	emu_dspDumpData((uint32_t)state->buf, 256);
	*/
#ifdef CONFIG_NVT_SPINAND_HWECC
	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.size = 0x200;
	nand->ecc.bytes = 0x8;
	nand->ecc.steps = 0x4;
	nand->ecc.total	= nand->ecc.steps * nand->ecc.bytes;
	if (*state->buf == SPI_NAND_MFR_GD)
		nand->ecc.layout = &spinand_oob_gd;
	else
		nand->ecc.layout = &spinand_oob_64;

	nand->ecc.strength = 1;

	nand->ecc.read_page = spinand_read_page_hwecc;
	nand->ecc.write_page = spinand_write_page_hwecc;
//	nand->ecc.hwctl = drv_nand_ecc_hwctl;
//	nand->ecc.calculate = drv_nand_ecc_calculate;
//	nand->ecc.correct = drv_nand_ecc_correct;
#else
	nand->ecc.mode = NAND_ECC_SOFT;
	
	if (spinand_disable_ecc(info->spi) < 0)
		printf("disable ecc failed!\n");
	
#endif
	nand->waitfunc = spinand_wait;
	nand->read_byte = spinand_read_byte;

	nand->read_buf = spinand_read_buf;
	nand->write_buf = spinand_write_buf;

	nand->cmdfunc = spinand_cmdfunc;
	nand->cmd_ctrl = spinand_hwcontrol;
	nand->options = NAND_NO_SUBPAGE_WRITE;

	return 0;
fail_free_state:
	free(state);
fail_free_info:
	free(info);
	return -ENOMEM;
}


#endif

