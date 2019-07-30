#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include "nvt_flash_spi_int.h"
#include "nvt_flash_spi_reg.h"
#include <mach/cc.h>

static void dma_flushwritecache(void *pbuf, u32 len)
{
	__cpuc_flush_dcache_area((void *)pbuf, len);
}

static void dma_flushreadcache(void *pbuf, u32 len)
{
	__cpuc_flush_dcache_area((void *)pbuf, len);
}

void spiflash_disable_irq(struct drv_nand_dev_info *info)
{
	spin_lock_irq(&info->intc_lock);
	cc_hwlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_INTC);
#ifndef CONFIG_SMP
	*(uint32_t*) 0xFE501184 = 0x20000000;
	*(uint32_t*) 0xFE50183C &= ~0xFF00;
	*(uint32_t*) 0xFE50183C |= 0x100;
#endif
	cc_hwunlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_INTC);
	spin_unlock_irq(&info->intc_lock);
}

void spiflash_enable_irq(struct drv_nand_dev_info *info)
{
	spin_lock_irq(&info->intc_lock);
	cc_hwlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_INTC);
#ifndef CONFIG_SMP
	*(uint32_t*) 0xFE50183C &= ~0xFF00;
	*(uint32_t*) 0xFE50183C |= 0x200;
	*(uint32_t*) 0xFE501104 = 0x20000000;
#endif
	cc_hwunlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_INTC);
	spin_unlock_irq(&info->intc_lock);
}


static int spiflash_disable_write_latch(struct drv_nand_dev_info *info)
{
	struct smc_setup_trans  trans_param;
	int  ret;

	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _CMD;
	trans_param.cs = 0;

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, FLASH_CMD_WRDI, FALSE);

	ret = nand_cmd_wait_complete(info);

	return ret;
}

static int spiflash_enable_write_latch(struct drv_nand_dev_info *info)
{
#if 0
	u8 word = 0;
#endif
	struct smc_setup_trans  trans_param;
	int  ret;
	do
	{
#if 0
		if (g_pNandInfo->bSupportEWSR== TRUE) {
			word = nand_cmd_read_status(info);

			word &= ~(FLASH_STATUS_WP_BITS | FLASH_STATUS_WEL_BITS);

			trans_param.col_addr = 0;
			trans_param.row_addr = 0;
			trans_param.fifo_dir = _FIFO_READ;
			trans_param.trans_mode = _PIO_MODE;
			trans_param.type = _CMD;
			trans_param.cs = info->flash_info->chip_sel;

			nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

			nand_host_send_command(info, FLASH_CMD_EWSR, FALSE);

			ret = nand_cmd_wait_complete(info);

			if (ret != E_OK)
				break;

			ret = nand_cmd_write_status(info, word);

			if (ret != E_OK)
				break;
		}
#endif
		trans_param.col_addr = 0;
		trans_param.row_addr = 0;
		trans_param.fifo_dir = _FIFO_READ;
		trans_param.trans_mode  = _PIO_MODE;
		trans_param.type  = _CMD;
		trans_param.cs = info->flash_info->chip_sel;

		nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

		nand_host_send_command(info, FLASH_CMD_WREN, FALSE);

		ret = nand_cmd_wait_complete(info);

		if (ret != E_OK)
			break;
	}while(0);

	return ret;
}


/*
    SPI NAND read page(SPI Single page read)

    @return
        - @b E_OK: success
        - @b E_OACV: already closed
        - @b E_NOMEM: read range exceed flash page
        - @b Else: fail
*/
static int spinand_read_page(struct drv_nand_dev_info *info, uint32_t row_addr, uint32_t col_addr, uint8_t* pbuf, uint32_t buf_size)
{
	struct smc_setup_trans  trans_param;
	uint8_t status;
	uint32_t  total_page_cnt,spare_area_addr, j, *pul_buf_spare;
	uint32_t *pul_buf = (uint32_t *)pbuf;
	uint32_t  section_count_per_page = (info->flash_info->page_size / 0x200);
	int ret = E_OK;

	union NAND_COMMAND_REG  reg_cmd_cycle= {0};
	union NAND_COMMAND2_REG reg_cmd2_cycle= {0};

	do
	{
		total_page_cnt = buf_size / info->flash_info->page_size;

		if (total_page_cnt == 1 && ((col_addr+buf_size) > (SPINAND_PAGE_SIZE+SPINAND_SPARE_SIZE))) {
			pr_info("col addr 0x%08x, buf size 0x%08x exceed page 2KB + 64B\r\n",
				(int32_t)col_addr, (int32_t)buf_size);
			ret = E_NOMEM;
			break;
		} else {
			if(buf_size % info->flash_info->page_size != 0 && col_addr != info->flash_info->page_size) {
				pr_info("buf_size[0x%08x] not multiple of page size[0x%08x]\r\n",
					buf_size, info->flash_info->page_size);
				ret = E_SYS;
				break;
			}

		}

		trans_param.col_addr = col_addr;
		trans_param.row_addr = row_addr;
		trans_param.fifo_dir = _FIFO_READ;
		trans_param.trans_mode = _DMA_MODE;
		trans_param.type = _DATA_RD3;
		trans_param.cs = NAND_CS0_EN;

		//Read spare data
		if(col_addr == info->flash_info->page_size)
		{
			u32 module0_reg;
			if(GET_NAND_SPARE_SIZE(info->flash_info) != buf_size) {
				pr_info("Spare size [%2d]!= Fill buffer size[%2d]\r\n", GET_NAND_SPARE_SIZE(info->flash_info), buf_size);
				break;
			}

			if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC) {
				module0_reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);
				module0_reg &= ~0x300;
				NAND_SETREG(info, NAND_MODULE0_REG_OFS, module0_reg | NAND_PRI_ECC_SPI_ON_DIE);
			}

			trans_param.trans_mode = _PIO_MODE;

			nand_host_setup_transfer(info, &trans_param, 1, GET_NAND_SPARE_SIZE(info->flash_info), _SINGLE_PAGE);

			reg_cmd_cycle.bit.cmd_cyc_1st = SPINAND_CMD_PAGE_READ;

			reg_cmd_cycle.bit.cmd_cyc_2nd = SPINAND_CMD_GET_FEATURE;

			reg_cmd_cycle.bit.cmd_cyc_3rd = SPINAND_FEATURE_STATUS;

			reg_cmd_cycle.bit.cmd_cyc_4th = SPINAND_CMD_CACHE_READX4;

			nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);

			reg_cmd2_cycle.bit.cmd_cyc_5th = SPINAND_CMD_GET_FEATURE;

			reg_cmd2_cycle.bit.cmd_cyc_6th = SPINAND_FEATURE_STATUS;

			nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

			nand_host_setup_status_check_bit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			NAND_SETREG(info, NAND_COMMAND2_REG_OFS, reg_cmd2_cycle.reg);

			nand_host_send_command(info, reg_cmd_cycle.reg, TRUE);

			ret = nand_cmd_wait_complete(info);

			nand_host_set_cpu_access_spare(info, TRUE);

			spare_area_addr = (uint32_t) info->mmio_base + NAND_SPARE00_REG_OFS;

			if(section_count_per_page > 1) {
				for(j = 0; j < section_count_per_page; j++) {
					*(pul_buf + j +  0) = *(uint32_t *)(spare_area_addr+0 + j * 4);
					*(pul_buf + j +  4) = *(uint32_t *)(spare_area_addr+16 + j * 4);
					*(pul_buf + j +  8) = *(uint32_t *)(spare_area_addr+32 + j * 4);
					*(pul_buf + j + 12) = *(uint32_t *)(spare_area_addr+48 + j * 4);
				}
			} else {
				*(pul_buf + 0) = *(uint32_t *)(spare_area_addr+0x0);
				*(pul_buf + 1) = *(uint32_t *)(spare_area_addr+0x4);
				*(pul_buf + 2) = *(uint32_t *)(spare_area_addr+0x8);
				*(pul_buf + 3) = *(uint32_t *)(spare_area_addr+0xC);
			}
			nand_host_set_cpu_access_spare(info, FALSE);

			if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC) {
				module0_reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);
				module0_reg &= ~0x300;
				NAND_SETREG(info, NAND_MODULE0_REG_OFS, module0_reg | NAND_PRI_ECC_RS);
			}
		} else {
			NAND_SETREG(info, NAND_DMASTART_REG_OFS, virt_to_phys(pul_buf));

			nand_host_setup_transfer(info, &trans_param, 1, info->flash_info->page_size, _SINGLE_PAGE);

			dma_flushreadcache((uint32_t*)(pul_buf), (info->flash_info->page_size + info->flash_info->oob_size));

			reg_cmd_cycle.bit.cmd_cyc_1st = SPINAND_CMD_PAGE_READ;

			reg_cmd_cycle.bit.cmd_cyc_2nd = SPINAND_CMD_GET_FEATURE;

			reg_cmd_cycle.bit.cmd_cyc_3rd = SPINAND_FEATURE_STATUS;

			reg_cmd_cycle.bit.cmd_cyc_4th = SPINAND_CMD_CACHE_READX4;

			nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);

			reg_cmd2_cycle.bit.cmd_cyc_5th = SPINAND_CMD_GET_FEATURE;

			reg_cmd2_cycle.bit.cmd_cyc_6th = SPINAND_FEATURE_STATUS;

			nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

			nand_host_setup_status_check_bit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			NAND_SETREG(info, NAND_COMMAND2_REG_OFS, reg_cmd2_cycle.reg);

			nand_host_send_command(info, reg_cmd_cycle.reg, TRUE);

			ret = nand_cmd_wait_complete(info);

			nand_host_set_cpu_access_spare(info, TRUE);

			spare_area_addr = (uint32_t) info->mmio_base + NAND_SPARE00_REG_OFS;

			pul_buf_spare = (uint32_t *)(pul_buf + info->flash_info->page_size/4);

			if(section_count_per_page > 1) {
				for(j = 0; j < section_count_per_page; j++) {
					*(pul_buf_spare + j +  0) = *(uint32_t *)(spare_area_addr+0 + j * 4);
					*(pul_buf_spare + j +  4) = *(uint32_t *)(spare_area_addr+16 + j * 4);
					*(pul_buf_spare + j +  8) = *(uint32_t *)(spare_area_addr+32 + j * 4);
					*(pul_buf_spare + j + 12) = *(uint32_t *)(spare_area_addr+48 + j * 4);
				}
			} else {
				*(pul_buf_spare + 0) = *(uint32_t *)(spare_area_addr+0x0);
				*(pul_buf_spare + 1) = *(uint32_t *)(spare_area_addr+0x4);
				*(pul_buf_spare + 2) = *(uint32_t *)(spare_area_addr+0x8);
				*(pul_buf_spare + 3) = *(uint32_t *)(spare_area_addr+0xC);
			}

			nand_host_set_cpu_access_spare(info, FALSE);

			status = (uint8_t)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

			if (info->use_ecc == NANDCTRL_SPIFLASH_USE_ONDIE_ECC) {
				if(status & (SPINAND_FEATURE_STATUS_ECC_ERR | SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR)) {
					// ECC uncorrectable
					if((status & SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR) == SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR)
					{
						ret = E_CTX;
						pr_err("uncorrect ecc error sts 0x%08x, row Addr 0x%08x, col Addr 0x%08x\r\n", (uint32_t)SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR, (uint32_t)row_addr, (uint32_t)col_addr);
						break;
					} else {
						ret = ECC_CORRECTED;
						pr_debug("Bit errors were detected and corrected\n");
						pr_debug("II sts 0x%08x, row Addr 0x%08x, col Addr 0x%08x\r\n", SPINAND_FEATURE_STATUS_ECC_ERR, (uint32_t)row_addr, (uint32_t)col_addr);
						break;
					}
				}
			} else {
				if (status & NAND_PRI_ECC_STS) {
					pr_debug("Primary RS ECC procedure\n");
					NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_PRI_ECC_STS);

					nand_host_set_fifo_enable(info, FALSE);

					if (nand_host_correct_reedsolomon_ecc(info, (u8*)pul_buf, info->flash_info->page_size / 0x200) != E_OK) {
						pr_err("uncorrect ecc error sts 0x%08x, row Addr 0x%08x, col Addr 0x%08x\r\n", (uint32_t)SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR, (uint32_t)row_addr, (uint32_t)col_addr);
						ret = E_CTX;
					} else {
						dma_flushreadcache((uint32_t*)(pul_buf), (info->flash_info->page_size + info->flash_info->oob_size));
						ret = ECC_CORRECTED;
					}
				}
			}
		}
	} while (0);

    return ret;
}


/*
    Unlock SPI NAND block protect
*/
int spi_nand_unlock_bp(struct drv_nand_dev_info *info)
{
	struct smc_setup_trans trans_param;

	int ret = E_OK;

	if (info->flash_info->spi_nand_status.block_unlocked == FALSE) {
		trans_param.col_addr = 0;
		trans_param.row_addr = (SPINAND_FEATURE_BLOCK_LOCK << 8) | 0x00;
		trans_param.fifo_dir = _FIFO_READ;
		trans_param.trans_mode = _PIO_MODE;
		trans_param.type = _CMD_ADDR;
		trans_param.cs = NAND_CS0_EN;

		nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

		nand_host_setup_address_cycle(info, NAND_2_ADDRESS_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);

		nand_host_send_command(info, SPINAND_CMD_SET_FEATURE, FALSE);

		ret = nand_cmd_wait_complete(info);

		if (ret == E_OK)
			info->flash_info->spi_nand_status.block_unlocked = TRUE;
	}

	return ret;
}

/*
    SPI NAND program page

    @param[in] row_addr        row address
    @param[in] col_addr        column address
    @param[in] pbuf             buffer to be programmed
    @param[in] buf_size        buffer size
    @param[in] pspare_buf        buffer to be programmed on spare
    @param[in] spare_size      buffer size of pspare_buf

    @return
        - @b E_OK: success
        - @b E_OACV: already closed
        - @b E_NOMEM: program range exceed flash page
        - @b Else: fail
*/
static int spinand_program_page(struct drv_nand_dev_info *info,
	uint32_t row_addr, uint32_t col_addr, uint32_t buf_size,
	uint8_t* pspare_buf, uint32_t spare_size)
{
	struct smc_setup_trans  trans_param;
	uint8_t status;
	int ret = E_OK;
	union NAND_COMMAND_REG  reg_cmd_cycle= {0};
	union NAND_COMMAND2_REG reg_cmd2_cycle= {0};
	uint32_t *buf_start = (uint32_t *)(info->data_buff + col_addr);

	do {
		ret = spi_nand_unlock_bp(info);
		if (ret != E_OK)
			break;

		if (col_addr) {
			trans_param.col_addr = 0;
			trans_param.row_addr = 0;
			trans_param.fifo_dir = _FIFO_READ;
			trans_param.trans_mode = _PIO_MODE;
			trans_param.type = _CMD;
			trans_param.cs = NAND_CS0_EN;

			nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

			nand_host_send_command(info, SPINAND_CMD_WEN, FALSE);

			ret = nand_cmd_wait_complete(info);
			if (ret != E_OK)
				return ret;

			nand_host_set_nand_type(info, NANDCTRL_SPI_NOR_TYPE);

			if (clk_get_phase(info->clk))
				clk_set_phase(info->clk, 0);

			nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);

			nand_host_config_cs_opmode(info, NAND_SPI_CS_MANUAL_MODE);

			nand_host_set_cs_active(info, NAND_SPI_CS_LOW);

			reg_cmd_cycle.bit.cmd_cyc_1st = SPINAND_CMD_PROGRAM_LOAD;

			trans_param.col_addr = 0;
			trans_param.row_addr = col_addr;
			trans_param.fifo_dir = _FIFO_READ;
			trans_param.trans_mode = _PIO_MODE;
			trans_param.type = _CMD_ADDR;
			trans_param.cs = 0;

			nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

			nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

			nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_2_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);

			nand_host_send_command(info, reg_cmd_cycle.reg, FALSE);

			ret = nand_cmd_wait_complete(info);

			if(ret != E_OK) {
				nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);
				nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);
				return ret;
			}

			nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);

			trans_param.col_addr = 0;
			trans_param.row_addr = 0;
			trans_param.fifo_dir = _FIFO_WRITE;
			trans_param.trans_mode = _PIO_MODE;
			trans_param.type = _SPI_WRITE_N_BYTES;

			nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

			nand_host_setup_transfer(info, &trans_param, 0, buf_size, _SINGLE_PAGE);

			nand_host_send_command(info, reg_cmd_cycle.reg, FALSE);

			ret = nand_host_transmit_data(info, (u8*)buf_start, buf_size, _PIO_MODE);

			if(ret != E_OK) {
				nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);
				nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);
				return ret;
			}

			nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);

			nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);

			if (!(clk_get_phase(info->clk)))
				clk_set_phase(info->clk, 1);

			nand_host_set_nand_type(info, NANDCTRL_SPI_NAND_TYPE);

			reg_cmd_cycle.bit.cmd_cyc_1st = SPINAND_CMD_PROGRAM_EXE;

			trans_param.col_addr = 0;
			trans_param.row_addr = row_addr;
			trans_param.fifo_dir = _FIFO_READ;
			trans_param.trans_mode = _PIO_MODE;
			trans_param.type = _CMD_ADDR;
			trans_param.cs = 0;

			nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

			nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

			nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);

			nand_host_send_command(info, reg_cmd_cycle.reg, FALSE);

			ret = nand_cmd_wait_complete(info);

			if (ret != E_OK)
				break;

			trans_param.col_addr = 0;
			trans_param.row_addr = 0;
			trans_param.type = _AUTO_STATUS;

			nand_host_setup_transfer(info, &trans_param, 0, 1, _SINGLE_PAGE);

			nand_host_setup_status_check_bit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			nand_host_send_command(info, SPINAND_CMD_GET_FEATURE | (SPINAND_FEATURE_STATUS <<8) , 0);

			ret = nand_cmd_wait_complete(info);
		} else {
			// write enable
			trans_param.col_addr = col_addr;
			trans_param.row_addr = row_addr;
			trans_param.fifo_dir = _FIFO_WRITE;
			trans_param.trans_mode = _DMA_MODE;
			trans_param.type = _SPI_PROG_CMD;
			trans_param.cs = NAND_CS0_EN;

			NAND_SETREG(info, NAND_DMASTART_REG_OFS, virt_to_phys(info->data_buff));

			nand_host_setup_transfer(info, &trans_param, 1, info->flash_info->page_size, _SINGLE_PAGE);

			dma_flushwritecache(info->data_buff, info->flash_info->page_size);

			reg_cmd_cycle.bit.cmd_cyc_1st = SPINAND_CMD_WEN;

			if (info->flash_info->spi_nand_status.quad_program) {
				reg_cmd_cycle.bit.cmd_cyc_2nd = SPINAND_CMD_PROGRAM_LOADX4;

				nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);
			} else {
				reg_cmd_cycle.bit.cmd_cyc_2nd = SPINAND_CMD_PROGRAM_LOAD;

				nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);
			}

			reg_cmd_cycle.bit.cmd_cyc_3rd = SPINAND_CMD_PROGRAM_EXE;

			reg_cmd_cycle.bit.cmd_cyc_4th= SPINAND_CMD_GET_FEATURE;

			reg_cmd2_cycle.bit.cmd_cyc_5th = SPINAND_FEATURE_STATUS;

			nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

			nand_host_setup_status_check_bit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			NAND_SETREG(info, NAND_COMMAND2_REG_OFS, reg_cmd2_cycle.reg);

			nand_host_send_command(info, reg_cmd_cycle.reg, TRUE);

			ret = nand_cmd_wait_complete(info);

			if (ret != E_OK)
				break;
		}

		status = (uint8_t)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

		if(status & SPINAND_FEATURE_STATUS_PROG_FAIL)
			ret = E_SYS;
	} while (0);

	return ret;
}



/*
    Reset NAND flash

    Send reset command to NAND flash

    @return
        - @b E_SYS      Status fail
        - @b E_TMOUT    Controller timeout
        - @b E_OK       Operation success

    @note for nand_CmdComplete()
*/
int nand_cmd_reset(struct drv_nand_dev_info *info)
{
	struct smc_setup_trans trans_param;

	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _CMD;
	trans_param.cs = NAND_CS0_EN;

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, SPINAND_CMD_RESET, 0);

	return nand_cmd_wait_complete(info);
}


/*
    Read status command

    Send read status command to NAND flash

    @return
        - @b E_SYS      Status fail
        - @b E_TMOUT    Controller timeout
        - @b E_OK       Operation success

    @note for nand_CmdComplete()
*/
int nand_cmd_read_status(struct drv_nand_dev_info *info, uint32_t set)
{
	int ret;
	uint32_t status = 0;
	struct smc_setup_trans trans_param;
	uint32_t spi_cmd;

	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _CMD_STS;
	trans_param.cs = 0;

	nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &trans_param, 0, 1, _SINGLE_PAGE);

	spi_cmd = SPINAND_CMD_GET_FEATURE;

	if (set == NAND_SPI_STS_FEATURE_1)
		spi_cmd |= (SPINAND_FEATURE_BLOCK_LOCK << 8);
	else if (set == NAND_SPI_STS_FEATURE_2)
		spi_cmd |= (SPINAND_FEATURE_OPT << 8);
	else if (set == NAND_SPI_STS_FEATURE_4)
		spi_cmd |= (SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR << 8);
	else
		spi_cmd |= (SPINAND_FEATURE_STATUS << 8);

	nand_host_send_command(info, spi_cmd, 0);

	ret = nand_cmd_wait_complete(info);

	if(ret == E_OK)
	{
		status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & 0xFF;
		memcpy(info->data_buff + info->buf_start, &status, 1);
	}
	return status;
}

/*
    Write status command

    Send write status command to NAND flash

    @return
        - @b E_SYS      Status fail
        - @b E_TMOUT    Controller timeout
        - @b E_OK       Operation success

    @note for nand_CmdComplete()
*/
int nand_cmd_write_status(struct drv_nand_dev_info *info, u32 set, u32 status)
{
	int ret;
	struct smc_setup_trans trans_param;
	uint32_t spi_cmd = 0;
	union NAND_COMMAND_REG  cmd_cycle= {0};

	cmd_cycle.bit.cmd_cyc_1st = SPINAND_CMD_SET_FEATURE;
	if (set == NAND_SPI_STS_FEATURE_1)
		spi_cmd = SPINAND_FEATURE_BLOCK_LOCK << 8;
	else if (set == NAND_SPI_STS_FEATURE_2)
		spi_cmd = SPINAND_FEATURE_OPT << 8;
	else
		spi_cmd = SPINAND_FEATURE_STATUS << 8;

	spi_cmd |= status;

	trans_param.col_addr = 0;
	trans_param.row_addr = spi_cmd;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _CMD_ADDR;
	trans_param.cs = info->flash_info->chip_sel;

	nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_2_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

	nand_host_send_command(info, cmd_cycle.reg, 0);

	ret =  nand_cmd_wait_complete(info);

	return ret;
}

/*
    read config command

    Send read config command to NAND flash

    @return
        - @b E_SYS      Status fail
        - @b E_TMOUT    Controller timeout
        - @b E_OK       Operation success

    @note for nand_CmdComplete()
*/
int nand_cmd_read_config(struct drv_nand_dev_info *info, u32 config)
{
	struct smc_setup_trans trans_param;
	int ret;

	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _CMD_STS;
	trans_param.cs = 0;

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, config, 0);

	ret = nand_cmd_wait_complete(info);
        if (ret != E_OK)
		return ret;

	return (u8)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
}

/*
     Erase block command.

     Issue erase block command of NAND controller
     @note block_num is the physical block number instead of the address.

     @param[in] block_num   which physical block number want to erase
     @return success or fail
         - @b E_OK:     Erase operation success
         - @b E_TMOUT   Time out
         - @b E_SYS:    Erase operation fail
*/
int nand_cmd_erase_block(struct drv_nand_dev_info *info, uint32_t block_address)
{
	int err_return = E_OK, status;
	struct smc_setup_trans trans_param;

	if(spi_nand_unlock_bp(info) != E_OK)
		return E_SYS;

        trans_param.col_addr = 0;
        trans_param.row_addr = 0;
        trans_param.fifo_dir = _FIFO_READ;
        trans_param.trans_mode = _PIO_MODE;
        trans_param.type = _CMD;
        trans_param.cs = NAND_CS0_EN;

        nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

        nand_host_send_command(info, SPINAND_CMD_WEN, FALSE);

        err_return = nand_cmd_wait_complete(info);
        if (err_return != E_OK)
		return err_return;

	trans_param.col_addr = 0;
	trans_param.row_addr = block_address;
	trans_param.fifo_dir = _FIFO_WRITE;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _BLK_ERSE;
	trans_param.cs = NAND_CS0_EN;

	// Once if not write operation => disable write protect EN
	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

        nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_COL_ADDRESS_CYCLE_CNT);

        nand_host_setup_status_check_bit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

	nand_host_send_command(info, SPINAND_CMD_BLK_ERASE | (SPINAND_CMD_GET_FEATURE << 8) | (SPINAND_FEATURE_STATUS << 16), TRUE);

	err_return = nand_cmd_wait_complete(info);

	if (err_return != E_OK) {
		dev_err(&info->pdev->dev, "Erase fail sts = 0x%08x\r\n", info->nand_int_status);
		return err_return;
	}

	status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
        if(status & SPINAND_FEATURE_STATUS_ERASE_FAIL)
        {
            dev_err(&info->pdev->dev,"Erase block fail 0x%08x\r\n", block_address);
            err_return = E_SYS;
        }

	return err_return;
}
/*
    Read device ID.

    @return Device ID
*/

int nand_cmd_read_id(uint8_t * card_id, struct drv_nand_dev_info *info)
{
    	struct smc_setup_trans trans_param;

	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _READ_ID;
	trans_param.cs = NAND_CS0_EN;

	nand_host_setup_transfer(info, &trans_param, 0, 4, _SINGLE_PAGE);

	nand_host_send_command(info, SPINAND_CMD_JEDECID, FALSE);

    	return nand_host_receive_data(info, card_id, 4, _PIO_MODE);
}

/*
    Read ECC corrected bits

    @return correct bits
*/

int nand_cmd_read_flash_ecc_corrected(struct drv_nand_dev_info *info)
{
    	struct smc_setup_trans trans_param;
	uint8_t  ecc_bits[4];

	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _READ_ID;
	trans_param.cs = NAND_CS0_EN;

	nand_host_setup_transfer(info, &trans_param, 0, 4, _SINGLE_PAGE);

	nand_host_send_command(info, SPINAND_CMD_MXIC_READ_ECC, FALSE);

	nand_host_receive_data(info, &ecc_bits[0], 4, _PIO_MODE);

	return ecc_bits[0] & 0xF;
}

/*
    nand_cmd_wait_complete

    Check NAND operation command complete.

    @return
            - @b E_SYS      Status fail
            - @b E_OK       Operation success

*/
int nand_cmd_wait_complete(struct drv_nand_dev_info *info)
{
	u32 ret = 0;
	// Use interrupt to wait command.
	ret = wait_for_completion_timeout(&info->cmd_complete, 4*HZ);

	if (ret == 0) {
		ret = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
		dev_err(&info->pdev->dev, "wait timeout with status 0x%x\n", \
					ret);
		return -EINTR;
	}

	if ((info->nand_int_status & NAND_STS_FAIL_STS) && \
		!(info->nand_int_status & SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR)) {
		dev_err(&info->pdev->dev, "nand status error\r\n");
		return -EINTR;
	}

	if(info->nand_int_status & NAND_TOUT_STS) {
		dev_err(&info->pdev->dev, "NAND cmd timeout\r\n");
		return -EIO;
	}
	return E_OK;
}

/*
    Wait SPI flash ready

    Wait SPI flash returned to ready state

    @param[in] uiWaitMs     Timeout setting. (Unit: ms)

    @return
        - @b E_OK: success
        - @b Else: fail. Maybe timeout.
*/
static int spiflash_wait_ready(struct drv_nand_dev_info *info, u8 * status)
{
	union NAND_COMMAND_REG  cmd_cycle= {0};
	struct smc_setup_trans  trans_param;
	int ret = E_OK;
	do
	{
	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _AUTO_STATUS;
	trans_param.cs = 0;

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_RDSR;

	nand_host_setup_status_check_bit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

	nand_host_send_command(info, cmd_cycle.reg, TRUE);

	ret = nand_cmd_wait_complete(info);

	} while (0);

	if(status)
		*status = (u8)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

	return ret;
}

/*
    Read pages.

    @param[out] buffer Buffer address
    @param page_address The address of the page. Only (n * g_pNandInfo->uiBytesPerPageData) will be valid.
                       Beware that the max accessible size is 4GB.  One should use nand_readOperation2() instead.
    @param num_page How many pages
    @return
        - @b E_OK       read operation success
        - @b E_PAR      parameter error
        - @b E_SYS
*/
int nand_cmd_read_operation(struct drv_nand_dev_info *info, int8_t * buffer, uint32_t page_address, uint32_t num_page)
{
	const struct nvt_nand_flash *f = info->flash_info;

	return spinand_read_page(info, page_address / f->page_size, 0, \
				(uint8_t*) buffer, num_page*f->page_size);
}


/*
    Write pages.(single page operation)

    @param buffer      Buffer address
    @param page_address The address of the page. Only (n * g_pNandInfo->uiBytesPerPageData) will be valid.
            Beware that the max accessible size is 4GB.  One should use write_readOperation2() instead.
    @param num_page     How many pages
    @return E_OK or E_SYS
*/
int nand_cmd_write_operation_single(struct drv_nand_dev_info *info, uint32_t page_address, uint32_t column)
{
	const struct nvt_nand_flash *f = info->flash_info;
	uint32_t buf_len = info->buf_count - column;

	return spinand_program_page(info, page_address / f->page_size, \
		column, buf_len, 0, 0);
}

void nand_cmd_write_spare_sram(struct drv_nand_dev_info *info)
{
	uint32_t spare_area_addr;
	uint32_t *pul_buf = (uint32_t *)(info->data_buff + info->flash_info->page_size);
	uint32_t  section_count_per_page = (info->flash_info->page_size / 0x200);
	int j = 0;

	dma_flushwritecache(pul_buf, info->flash_info->oob_size);

	nand_host_set_cpu_access_spare(info, TRUE);
	spare_area_addr = (uint32_t) info->mmio_base + NAND_SPARE00_REG_OFS;

	if(section_count_per_page > 1) {
		for(j = 0; j < section_count_per_page; j++) {
			*(uint32_t *)(spare_area_addr+0 + j * 4) = *(pul_buf + j +  0);
			*(uint32_t *)(spare_area_addr+16 + j * 4) = *(pul_buf + j +  4);
			*(uint32_t *)(spare_area_addr+32 + j * 4) = *(pul_buf + j +  8);
			*(uint32_t *)(spare_area_addr+48 + j * 4) = *(pul_buf + j + 12);
		}
	} else {
		*(uint32_t *)(spare_area_addr+0x0) = *(pul_buf + 0);
		*(uint32_t *)(spare_area_addr+0x4) = *(pul_buf + 1);
		*(uint32_t *)(spare_area_addr+0x8) = *(pul_buf + 2);
		*(uint32_t *)(spare_area_addr+0xC) = *(pul_buf + 3);
	}

	nand_host_set_cpu_access_spare(info, FALSE);
}

/*
    Read the spare data from a page for debug.

    @param[out] spare0  The value of Spare Area Read Data Register 0
    @param[out] spare1  The value of Spare Area Read Data Register 1
    @param[out] spare2  The value of Spare Area Read Data Register 2
    @param[out] spare3  The value of Spare Area Read Data Register 3
    @param page_address The address of the page
    @return
            - @b E_OK       read spare success
            - @b E_SYS      read spare operation fail(status fail)
            - @b E_CTX      read spare encounter ecc uncorrect error(Only if Reed solomon ecc usage)
*/
int nand_cmd_read_page_spare_data(struct drv_nand_dev_info *info, int8_t *buffer, uint32_t page_address)
{
	const struct nvt_nand_flash *f = info->flash_info;

	return spinand_read_page(info, page_address / f->page_size, \
			f->page_size, (uint8_t *)buffer, f->oob_size);
}

/*
    Read the largest number of ecc corrected bits
*/
int nand_cmd_read_ecc_corrected(struct drv_nand_dev_info *info)
{
	union NAND_RSERR_STS0_REG reg_correct = {0};
	u8 correct_count = 0;

	reg_correct.reg = NAND_GETREG(info, NAND_RSERR_STS0_REG_OFS);

	/*Read the largest number of correct bits*/
	correct_count = reg_correct.bit.sec0_err_sts;
	if (reg_correct.bit.sec1_err_sts > correct_count)
		correct_count = reg_correct.bit.sec1_err_sts;

	if (reg_correct.bit.sec2_err_sts > correct_count)
		correct_count = reg_correct.bit.sec2_err_sts;

	if (reg_correct.bit.sec3_err_sts > correct_count)
		correct_count = reg_correct.bit.sec3_err_sts;

	return correct_count;
}

/*
	Read SPI flash

	@param[in] addr	Byte address of SPI flash
	@param[in] byte_size	Byte count of read data. Should be word alignment (i.e. multiple of 4).
	@param[out] pbuf	Pointer point to store read data

	@return void
*/
static int spiflash_read_data(struct drv_nand_dev_info *info, u32 addr, u32 byte_size, u8 *pbuf)
{
	union NAND_COMMAND_REG  cmd_cycle= {0};
	struct smc_setup_trans trans_param;
	int ret = E_OK;

	if (clk_get_phase(info->clk))
		clk_set_phase(info->clk, 0);

	nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);

	nand_host_config_cs_opmode(info, NAND_SPI_CS_MANUAL_MODE);

	nand_host_set_cs_active(info, NAND_SPI_CS_LOW);

	if (info->flash_info->nor_read_mode == SPI_NOR_QUAD_READ) {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_QUAD_READ_NORMAL_4BYTE;
		else
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_QUAD_READ_NORMAL;
	} else if (info->flash_info->nor_read_mode == SPI_NOR_DUAL_READ) {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_DUAL_READ_4BYTE;
		else
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_DUAL_READ;
	} else {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_READ_4BYTE;
		else
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_READ;
	}

	trans_param.col_addr = 0;
	trans_param.row_addr = addr;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _CMD_ADDR;
	trans_param.cs = 0;

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
		nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_4_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);
	else
		nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);

	nand_host_send_command(info, cmd_cycle.reg, FALSE);

	ret = nand_cmd_wait_complete(info);

	if(ret != E_OK) {
		nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);
		nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);
		return ret;
	}

	trans_param.col_addr = 0;
	trans_param.row_addr = 0;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _DMA_MODE;
	trans_param.type = _SPI_READ_N_BYTES;
	trans_param.cs = 0;

	NAND_SETREG(info, NAND_DMASTART_REG_OFS, virt_to_phys((uint32_t*)pbuf));

	if (info->flash_info->nor_read_mode != SPI_NOR_NORMAL_READ) {
		nand_host_setup_transfer(info, &trans_param, 0, 1, _SINGLE_PAGE);

		nand_host_send_command(info, cmd_cycle.reg, FALSE);

		ret = nand_cmd_wait_complete(info);

		if(ret != E_OK) {
			nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);
			nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);
			return ret;
		}
	}

	if (info->flash_info->nor_read_mode == SPI_NOR_QUAD_READ)
		nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);
	else if (info->flash_info->nor_read_mode == SPI_NOR_DUAL_READ)
		nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_DUAL_MODE, NAND_SPI_ORDER_MODE_1);
	else
		nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &trans_param, 0, byte_size, _SINGLE_PAGE);

	nand_host_send_command(info, cmd_cycle.reg, FALSE);

	ret = nand_cmd_wait_complete(info);

	nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);

	nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);

	if (info->flash_info->nor_read_mode != SPI_NOR_NORMAL_READ)
		nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	if (!(clk_get_phase(info->clk)))
		clk_set_phase(info->clk, 1);

	return ret;
}

/*
    Page program

    Program a page of SPI flash

    @param[in] addr        Byte address of SPI flash
    @param[in] byte_size       Byte count of written data. Should be <= 256 and word alignment (i.e. multiple of 4).
    @param[out] pbuf            Pointer point to store read data

    @return
        - @b E_OK: program success
        - @b FALSE: program fail. Maybe timeout.
*/
static int spiflash_page_program(struct drv_nand_dev_info *info, u32 addr, u32 byte_size, u8* pbuf)
{
	union NAND_COMMAND_REG  cmd_cycle = {0};
	struct smc_setup_trans trans_param;
	int  ret;
	u8   status;

	spiflash_enable_write_latch(info);

	if (clk_get_phase(info->clk))
		clk_set_phase(info->clk, 0);

	nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);

	nand_host_config_cs_opmode(info, NAND_SPI_CS_MANUAL_MODE);

	nand_host_set_cs_active(info, NAND_SPI_CS_LOW);

	if (info->flash_info->nor_quad_support) {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_PAGEPROG_4X_4BYTE;
		else
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_PAGEPROG_4X;
	} else {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_PAGEPROG_4BYTE;
		else
			cmd_cycle.bit.cmd_cyc_1st = FLASH_CMD_PAGEPROG;
	}

	trans_param.col_addr = 0;
	trans_param.row_addr = addr;
	trans_param.fifo_dir = _FIFO_READ;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _CMD_ADDR;
	trans_param.cs = 0;

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
		nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_4_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);
	else
		nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);

	nand_host_send_command(info, cmd_cycle.reg, FALSE);

	ret = nand_cmd_wait_complete(info);

	if(ret != E_OK) {
		nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);
		nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);
		return ret;
	}

	trans_param.fifo_dir = _FIFO_WRITE;

	if (byte_size < SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)
		trans_param.trans_mode = _PIO_MODE;
	else {
		trans_param.trans_mode = _DMA_MODE;
		NAND_SETREG(info, NAND_DMASTART_REG_OFS, virt_to_phys((uint32_t*)pbuf));
	}

	trans_param.type = _SPI_WRITE_N_BYTES;

	if (info->flash_info->nor_quad_support)
		nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);
	else
		nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &trans_param, 0, byte_size, _SINGLE_PAGE);

	nand_host_send_command(info, cmd_cycle.reg, FALSE);

	if (byte_size < SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)
		ret = nand_host_transmit_data(info, (u8*)pbuf, byte_size, _PIO_MODE);
	else
		ret = nand_cmd_wait_complete(info);

	nand_host_set_cs_active(info, NAND_SPI_CS_HIGH);

	nand_host_config_cs_opmode(info, NAND_SPI_CS_AUTO_MODE);

	if (!(clk_get_phase(info->clk)))
		clk_set_phase(info->clk, 1);

	nand_host_set_spi_io(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	if(ret != E_OK)
		return ret;

	ret = spiflash_wait_ready(info, &status);

	spiflash_disable_write_latch(info);

	return ret;
}

/*
    Read SPI flash data

    Read data from SPI flash.

    @param[in] byte_addr   Byte address of flash.
    @param[in] byte_size   Byte size of read data.
    @param[out] pbuf        Pointer point to store read data

    @return
        - @b E_OK: erase success
        - @b E_CTX: driver not opened
        - @b Else: read fail
*/
int spinor_read_operation(struct drv_nand_dev_info *info, u32 byte_addr, u32 byte_size, u8 *pbuf)
{
	u32 remain_byte;
	u32 read_cycle;
	u32 addr_index;
	u32 cycle_index;
	int ret = E_CTX;

	read_cycle = byte_size / SPIFLASH_MAX_READ_BYTE_AT_ONCE;
	addr_index = byte_addr;
	remain_byte = byte_size % SPIFLASH_MAX_READ_BYTE_AT_ONCE;
	dma_flushreadcache((uint32_t*)pbuf, byte_size);
	for(cycle_index = 0; cycle_index < read_cycle; cycle_index++, addr_index += SPIFLASH_MAX_READ_BYTE_AT_ONCE) {
		ret = spiflash_read_data(info, addr_index, SPIFLASH_MAX_READ_BYTE_AT_ONCE, (pbuf + (cycle_index * SPIFLASH_MAX_READ_BYTE_AT_ONCE)));
		if (ret != E_OK) {
			dev_err(&info->pdev->dev,"fail at flash address 0x%x\n", addr_index);
			break;
		} else
			pr_debug("R");
	}

	if(remain_byte)
	{
		ret = spiflash_read_data(info, addr_index, remain_byte, (pbuf + (cycle_index * SPIFLASH_MAX_READ_BYTE_AT_ONCE)));
		if (ret != E_OK)
			dev_err(&info->pdev->dev,"fail at flash address 0x%x\r\n", addr_index);
		else
			pr_debug("r");
	}

	dma_flushreadcache((uint32_t*)pbuf, byte_size);

	return ret;
}

/*
    Program a sector

    Program a sector of SPI flash.

    @param[in] byte_addr   Byte address of programed sector. Should be sector size alignment.
    @param[in] sector_size Byte size of one secotr. Should be sector size.
    @param[in] pbuf         pointer point to written data

    @return
        - @b E_OK: erase success
        - @b E_CTX: driver not opened
        - @b E_MACV: byte_addr is set into s/w write protect detect region
        - @b E_TMOUT: flash ready timeout
        - @b E_PAR: byte_addr is not sector size alignment
        - @b Else: program fail
*/
int spinor_program_operation(struct drv_nand_dev_info *info, u32 byte_addr, u32 sector_size, u8* pbuf)
{
	u32 program_cycle;
	u32 addr_index;
	u32 cycle_index;
	int ret = E_CTX;

	program_cycle = sector_size / SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE;

	addr_index = byte_addr;

	dma_flushwritecache((uint32_t*)pbuf, sector_size);

	for(cycle_index = 0; cycle_index < program_cycle; cycle_index++, addr_index += SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE) {
		ret = spiflash_page_program(info, addr_index, SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE, (pbuf + (cycle_index * SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)));

		if (ret != E_OK) {
			dev_err(&info->pdev->dev, "fail at flash address 0x%x\r\n", addr_index);
			break;
		} else
			pr_debug("W");

		sector_size -= SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE;
	}

	if (sector_size) {

		/*Minimal transmitting size should be word align*/
		if (sector_size % WORD_ALIGN_OFFSET)
			sector_size += WORD_ALIGN_OFFSET - (sector_size % WORD_ALIGN_OFFSET);

		ret = spiflash_page_program(info, addr_index, sector_size, (pbuf + (cycle_index * SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)));
	}

	return ret;
}


/*
    Erase a sector

    Program a sector of SPI flash.

    @param[in] byte_addr   Byte address of programed sector. Should be sector size alignment.
    @param[in] sector_size Byte size of one secotr. Should be sector size.
    @param[in] pbuf         pointer point to written data

    @return
        - @b E_OK: erase success
        - @b E_CTX: driver not opened
        - @b E_MACV: byte_addr is set into s/w write protect detect region
        - @b E_TMOUT: flash ready timeout
        - @b E_PAR: byte_addr is not sector size alignment
        - @b Else: program fail
*/
int spinor_erase_sector(struct drv_nand_dev_info *info, u8 cmd, u32 addr)
{
	struct smc_setup_trans trans_param;
	union NAND_COMMAND_REG  cmd_cycle = {0};
	u8 status;
	int ret;

	spiflash_wait_ready(info, &status);

	spiflash_enable_write_latch(info);

	//row addr = 0x44332211 --> 0x44 --> 0x33 --> 0x22 --> 0x11
	//                                            ___     ___________
	//use command + 1 cycle row address cycle ___|cmd|___|Row1stcycle|___

	//use block erase cycle
	//   ____    ______    ______    ______
	//___|cmd|___|addr1|___|addr2|___|addr3|___| ... |___|cmd2|___ ...
	//                                                            ____
	//                                                            |sts|__ ...

	cmd_cycle.bit.cmd_cyc_1st = cmd;
	cmd_cycle.bit.cmd_cyc_2nd = FLASH_CMD_RDSR;
	trans_param.col_addr = 0;
	trans_param.row_addr = addr;
	trans_param.fifo_dir = _FIFO_WRITE;
	trans_param.trans_mode = _PIO_MODE;
	trans_param.type = _BLK_ERSE;
	trans_param.cs = 0;

	if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
		nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_4_CYCLE_CNT, NAND_NOT_CONFIGED_COL_ADDRESS_CYCLE_CNT);
	else
		nand_host_setup_address_cycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_COL_ADDRESS_CYCLE_CNT);

	nand_host_setup_status_check_bit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, cmd_cycle.reg, TRUE);

	ret = nand_cmd_wait_complete(info);

	spiflash_disable_write_latch(info);

	return ret;
}

int spiflash_boot_detect(struct drv_nand_dev_info *info)
{
	union NAND_SPI_IO_REG reg_io = {0};

	reg_io.reg = NAND_GETREG(info, NAND_SPI_IO_REG_OFS);
	if (0 == reg_io.bit.cpu1_init) {
		reg_io.bit.cpu2_init = 1;
		NAND_SETREG(info, NAND_SPI_IO_REG_OFS, reg_io.reg);
	}

	return reg_io.bit.cpu1_init;
}
