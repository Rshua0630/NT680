#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <malloc.h>
#include <asm/arch/nvt_types.h>
#include "nand_reg.h"
#include "nand_int.h"


u32 nvt_flash_ecc_err_pages[100] = {0};

#if defined(CONFIG_NVT_FW_UPDATE_LED) && !defined(CONFIG_NVT_PWM)
#define OP_ERASE 0
#define OP_PROGRAM 1
int RESTORE_GPIO_DIR = 0;

/*      LED function for FW update*/
static void led_set_gpio_high(int operation)
{
	u32 gpio_reg;
	int ofs = NVT_LED_PIN/32;
	int shift = NVT_LED_PIN & 0x1F;

	ofs = ofs*0x4;

	/*Set gpio as high*/
	gpio_reg = INW(IOADDR_GPIO_REG_BASE + 0x30 + ofs);

	if (gpio_reg & (1 << shift))
		RESTORE_GPIO_DIR = 1;
	else {
		gpio_reg |= (1 << shift);
		OUTW(IOADDR_GPIO_REG_BASE + 0x30 + ofs, gpio_reg);
	}

	OUTW(IOADDR_GPIO_REG_BASE + 0x50 + ofs, (1 << shift));

	/*Config duration*/
	if (operation)
		mdelay(NVT_LED_PROGRAM_DURATION);
	else
		mdelay(NVT_LED_ERASE_DURATION);
}

static void led_set_gpio_low(void)
{
	u32 gpio_reg;
	int ofs = NVT_LED_PIN/32;
	int shift = NVT_LED_PIN & 0x1F;

	ofs = ofs*0x4;
	/*Set gpio as low*/
	OUTW(IOADDR_GPIO_REG_BASE + 0x70 + ofs, (1 << shift));

	/*Force gpio direction as original config*/
	if (!(RESTORE_GPIO_DIR)) {
		gpio_reg = INW(IOADDR_GPIO_REG_BASE + 0x30 + ofs);
		gpio_reg &= ~(1 << shift);
		OUTW(IOADDR_GPIO_REG_BASE + 0x30 + ofs, gpio_reg);
		RESTORE_GPIO_DIR = 0;
	}
}

#endif
/*
	Unlock SPI NAND block protect
*/
static ER spi_nand_unlock_BP(struct drv_nand_dev_info *info)
{
	struct smc_setup_trans transParam;

	ER ret = E_OK;

	if (info->flash_info->spi_nand_status.bBlockUnlocked == FALSE) {

        /*
        ___|cmd(0x1F)|___|SR(0x01)(row byte0)|___|SR(0x0)(row byte1)|____
        */
		transParam.colAddr      = 0;
		transParam.rowAddr      = (SPINAND_FEATURE_BLOCK_LOCK << 8) | 0x00;
		transParam.fifoDir      = _FIFO_READ;
		transParam.transMode    = _PIO_MODE;
		transParam.type         = _CMD_ADDR;
		transParam.uiCS         = 0;

		nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

		nand_hostSetupAddressCycle(info, NAND_2_ADDRESS_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);

		nand_host_send_command(info, SPINAND_CMD_SET_FEATURE, FALSE);

		ret = nand_cmd_wait_complete(info);

		if (ret == E_OK)
			info->flash_info->spi_nand_status.bBlockUnlocked = TRUE;
	}

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
static ER spiNand_readPage(struct drv_nand_dev_info *info, uint32_t uiRowAddr, uint32_t uiColAddr, uint8_t* pBuf, uint32_t uiBufSize)
{
	struct smc_setup_trans  transParam;
	UINT8 ucStatus;
	UINT32 uiPageCnt;
	UINT32 uiSpareAreaAddr;
	UINT32 j, *pulBuf_spare;

	UINT32 *pulBuf = (UINT32 *)pBuf;

	union T_NAND_COMMAND_REG  uiCmdCycle= {0};
	union T_NAND_COMMAND2_REG uiCmd2Cycle= {0};

	UINT32  uiNandSectionCountPerPage = (info->flash_info->page_size / 0x200);

	ER ret = E_OK;
	static int ecc_page_count = 0;

	do {
		uiPageCnt = uiBufSize / info->flash_info->page_size;

		if (uiPageCnt == 1 && ((uiColAddr+uiBufSize) > (SPINAND_PAGE_SIZE+SPINAND_SPARE_SIZE))) {
			printf("col addr 0x%08x, buf size 0x%08x exceed page 2KB + 64B\r\n",
				(int32_t)uiColAddr, (int32_t)uiBufSize);
			ret = E_NOMEM;
			break;
		} else {
			if(uiBufSize % info->flash_info->page_size != 0 && uiColAddr != info->flash_info->page_size) {
				printf("uiBufSize[0x%08x] not multiple of page size[0x%08x]\r\n",
					uiBufSize, info->flash_info->page_size);
				ret = E_SYS;
				break;
			}
		}

		transParam.colAddr = uiColAddr;
		transParam.rowAddr = uiRowAddr;//0xXX0280 [xx] dummy bus --> [0xXX][0x02][0x80]
		transParam.fifoDir = _FIFO_READ;
		transParam.transMode = _DMA_MODE;
		transParam.type = _DATA_RD3;
		transParam.uiCS = 0;

		//Read spare data
		if(uiColAddr == info->flash_info->page_size) {
			u32 module0_reg;
			if(GET_NAND_SPARE_SIZE(info->flash_info) != uiBufSize) {
				printf("Spare size [%2d]!= Fill buffer size[%2d]\r\n", GET_NAND_SPARE_SIZE(info->flash_info), uiBufSize);
				break;
			}

			if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC) {
				module0_reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);
				module0_reg &= ~0x300;
				NAND_SETREG(info, NAND_MODULE0_REG_OFS, module0_reg | NAND_PRI_ECC_SPI_ON_DIE);
			}

			transParam.transMode    = _PIO_MODE;

			nand_host_setup_transfer(info, &transParam, 1, GET_NAND_SPARE_SIZE(info->flash_info), _SINGLE_PAGE);

			uiCmdCycle.Bit.CMD_CYC_1ST = SPINAND_CMD_PAGE_READ;

			uiCmdCycle.Bit.CMD_CYC_2ND = SPINAND_CMD_GET_FEATURE;

			uiCmdCycle.Bit.CMD_CYC_3RD = SPINAND_FEATURE_STATUS;

			uiCmdCycle.Bit.CMD_CYC_4TH = SPINAND_CMD_CACHE_READX4;

			nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);

			uiCmd2Cycle.Bit.CMD_CYC_5TH = SPINAND_CMD_GET_FEATURE;

			uiCmd2Cycle.Bit.CMD_CYC_6TH = SPINAND_FEATURE_STATUS;

			nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

			nand_hostSetupStatusCheckBit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			NAND_SETREG(info, NAND_COMMAND2_REG_OFS, uiCmd2Cycle.Reg);

			nand_host_send_command(info, uiCmdCycle.Reg, TRUE);

			ret = nand_cmd_wait_complete(info);

			nand_host_set_cpu_access_spare(info, TRUE);

			uiSpareAreaAddr = IOADDR_NAND_REG_BASE + NAND_SPARE00_REG_OFS;

			if(uiNandSectionCountPerPage > 1) {
				for(j = 0; j < uiNandSectionCountPerPage; j++) {

					*(pulBuf + j +  0) = *(UINT32 *)(uiSpareAreaAddr+0 + j * 4);
					*(pulBuf + j +  4) = *(UINT32 *)(uiSpareAreaAddr+16 + j * 4);
					*(pulBuf + j +  8) = *(UINT32 *)(uiSpareAreaAddr+32 + j * 4);
					*(pulBuf + j + 12) = *(UINT32 *)(uiSpareAreaAddr+48 + j * 4);
				}
			} else {
				*(pulBuf + 0) = *(UINT32 *)(uiSpareAreaAddr+0x0);
				*(pulBuf + 1) = *(UINT32 *)(uiSpareAreaAddr+0x4);
				*(pulBuf + 2) = *(UINT32 *)(uiSpareAreaAddr+0x8);
				*(pulBuf + 3) = *(UINT32 *)(uiSpareAreaAddr+0xC);
			}

			nand_host_set_cpu_access_spare(info, FALSE);

			if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC) {
				module0_reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);
				module0_reg &= ~0x300;
				NAND_SETREG(info, NAND_MODULE0_REG_OFS, module0_reg | NAND_PRI_ECC_RS);
			}

		} else {
			NAND_SETREG(info, NAND_DMASTART_REG_OFS, dma_getPhyAddr((UINT32)pulBuf));

			nand_host_setup_transfer(info, &transParam, 1, info->flash_info->page_size, _SINGLE_PAGE);

			invalidate_dcache_range((UINT32)pulBuf, (UINT32)(pulBuf + info->flash_info->page_size + info->flash_info->oob_size));

			uiCmdCycle.Bit.CMD_CYC_1ST = SPINAND_CMD_PAGE_READ;

			uiCmdCycle.Bit.CMD_CYC_2ND = SPINAND_CMD_GET_FEATURE;

			uiCmdCycle.Bit.CMD_CYC_3RD = SPINAND_FEATURE_STATUS;

			uiCmdCycle.Bit.CMD_CYC_4TH = SPINAND_CMD_CACHE_READX4;

			nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);

			uiCmd2Cycle.Bit.CMD_CYC_5TH = SPINAND_CMD_GET_FEATURE;

			uiCmd2Cycle.Bit.CMD_CYC_6TH = SPINAND_FEATURE_STATUS;

			nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

			nand_hostSetupStatusCheckBit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			NAND_SETREG(info, NAND_COMMAND2_REG_OFS, uiCmd2Cycle.Reg);

			nand_host_send_command(info, uiCmdCycle.Reg, TRUE);

			ret = nand_cmd_wait_complete(info);

			nand_host_set_cpu_access_spare(info, TRUE);

			uiSpareAreaAddr = IOADDR_NAND_REG_BASE + NAND_SPARE00_REG_OFS;

			pulBuf_spare = (UINT32 *)(pulBuf + info->flash_info->page_size/4);

			if(uiNandSectionCountPerPage > 1) {
				for(j = 0; j < uiNandSectionCountPerPage; j++) {
					*(pulBuf_spare + j +  0) = *(UINT32 *)(uiSpareAreaAddr+0 + j * 4);
					*(pulBuf_spare + j +  4) = *(UINT32 *)(uiSpareAreaAddr+16 + j * 4);
					*(pulBuf_spare + j +  8) = *(UINT32 *)(uiSpareAreaAddr+32 + j * 4);
					*(pulBuf_spare + j + 12) = *(UINT32 *)(uiSpareAreaAddr+48 + j * 4);
				}
			} else {
				*(pulBuf_spare + 0) = *(UINT32 *)(uiSpareAreaAddr+0x0);
				*(pulBuf_spare + 1) = *(UINT32 *)(uiSpareAreaAddr+0x4);
				*(pulBuf_spare + 2) = *(UINT32 *)(uiSpareAreaAddr+0x8);
				*(pulBuf_spare + 3) = *(UINT32 *)(uiSpareAreaAddr+0xC);
			}

			nand_host_set_cpu_access_spare(info, FALSE);

			ucStatus = (UINT8)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

			if (info->use_ecc == NANDCTRL_SPIFLASH_USE_ONDIE_ECC) {
				if(ucStatus & (SPINAND_FEATURE_STATUS_ECC_ERR | SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR)) {
					// ECC uncorrectable
					if((ucStatus & SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR) == SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR) {
						ret = E_CTX;
						printf("uncorrect ecc error sts 0x%08x, row Addr 0x%08x, col Addr 0x%08x\r\n", (uint32_t)SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR, (uint32_t)uiRowAddr, (uint32_t)uiColAddr);
						printf("uncorrect ecc error block 0x%08x, page 0x%x\n", (uint32_t)(uiRowAddr & 0xFFFFFF40) / 64, (uint32_t)(uiRowAddr & 0xFFFFFF40) * 0x800);
						nvt_flash_ecc_err_pages[ecc_page_count] = uiRowAddr * 0x800;
						ecc_page_count++;
						break;
					} else {
						ret = ECC_CORRECTED;
						debug("Bit errors were detected and corrected\n");
						debug("II sts 0x%08x, row Addr 0x%08x, col Addr 0x%08x\r\n", SPINAND_FEATURE_STATUS_ECC_ERR, (uint32_t)uiRowAddr, (uint32_t)uiColAddr);
					}
				}
			} else {
				if (ucStatus & NAND_PRI_ECC_STS) {
					printf("Primary RS ECC procedure\n");
					NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_PRI_ECC_STS);

					nand_host_set_fifo_enable(info, FALSE);

					if (nand_host_correct_reedsolomon_ecc(info, (u8*)pulBuf, info->flash_info->page_size / 0x200) != E_OK) {
						printf("uncorrect ecc error sts 0x%08x, row Addr 0x%08x, col Addr 0x%08x\r\n", (uint32_t)SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR, (uint32_t)uiRowAddr, (uint32_t)uiColAddr);
						ret = E_CTX;
					} else {
						invalidate_dcache_range((UINT32)pulBuf, (UINT32)(pulBuf + info->flash_info->page_size + info->flash_info->oob_size));
						ret = ECC_CORRECTED;
					}
				}
			}
		}
	} while (0);
	return ret;
}

/*
	SPI NAND program page

	@param[in] uiRowAddr        row address
	@param[in] uiColAddr        column address
	@param[in] pBuf             buffer to be programmed
	@param[in] uiBufSize        buffer size
	@param[in] pSpareBuf        buffer to be programmed on spare
	@param[in] uiSpareSize      buffer size of pSpareBuf

	@return
		- @b E_OK: success
		- @b E_OACV: already closed
		- @b E_NOMEM: program range exceed flash page
		- @b Else: fail
*/
static ER spiNand_programPage(struct drv_nand_dev_info *info, UINT32 uiRowAddr, UINT32 uiColAddr, UINT8* pBuf, UINT32 uiBufSize, UINT8* pSpareBuf, UINT32 uiSpareSize)
{
	struct smc_setup_trans transParam;
	UINT8 ucStatus;

	UINT32 *pulBuf = (UINT32 *)(pBuf + uiColAddr);

	union T_NAND_COMMAND_REG  uiCmdCycle= {0};
	union T_NAND_COMMAND2_REG uiCmd2Cycle= {0};


	ER ret = E_OK;

#if defined(CONFIG_NVT_FW_UPDATE_LED) && !defined(CONFIG_NVT_PWM)
	led_set_gpio_high(OP_PROGRAM);
#endif
	do {
		ret = spi_nand_unlock_BP(info);
		if (ret != E_OK)
			break;

		if (uiColAddr) {
			transParam.colAddr = 0;
			transParam.rowAddr = 0;
			transParam.fifoDir = _FIFO_READ;
			transParam.transMode = _PIO_MODE;
			transParam.type = _CMD;
			transParam.uiCS = NAND_CS0_EN;

			nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

			nand_host_send_command(info, SPINAND_CMD_WEN, FALSE);

			ret = nand_cmd_wait_complete(info);
			if (ret != E_OK)
				return ret;

			nand_hostSetNandType(info, NANDCTRL_SPI_NOR_TYPE);

			/*Disable clk_auto_gating before set cs_manual*/
			*(u32*) 0xF00200E0 &= ~0x80000;

			nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);

			nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_MANUAL_MODE);

			nand_hostSetCSActive(info, NAND_SPI_CS_LOW);

			uiCmdCycle.Bit.CMD_CYC_1ST = SPINAND_CMD_PROGRAM_LOAD;

			transParam.colAddr = 0;
			transParam.rowAddr = uiColAddr;
			transParam.fifoDir = _FIFO_READ;
			transParam.transMode = _PIO_MODE;
			transParam.type = _CMD_ADDR;
			transParam.uiCS = 0;

			nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

			nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

			nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_2_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);

			nand_host_send_command(info, uiCmdCycle.Reg, FALSE);

			ret = nand_cmd_wait_complete(info);

			if(ret != E_OK) {
				nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);
				nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);
				return ret;
			}

			nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);

			transParam.colAddr = 0;
			transParam.rowAddr = 0;
			transParam.fifoDir = _FIFO_WRITE;
			transParam.transMode = _PIO_MODE;
			transParam.type = _SPI_WRITE_N_BYTES;

			nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

			nand_host_setup_transfer(info, &transParam, 0, uiBufSize, _SINGLE_PAGE);

			nand_host_send_command(info, uiCmdCycle.Reg, FALSE);

			ret = nand_host_transmit_data(info, (u8*)pulBuf, uiBufSize, _PIO_MODE);

			if(ret != E_OK) {
				nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);
				nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);
				return ret;
			}

			nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);

			nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);

			/*Enable clk_auto_gating after set cs_manual*/
			*(u32*) 0xF00200E0 |= 0x80000;

			nand_hostSetNandType(info, NANDCTRL_SPI_NAND_TYPE);

			uiCmdCycle.Bit.CMD_CYC_1ST = SPINAND_CMD_PROGRAM_EXE;

			transParam.colAddr = 0;
			transParam.rowAddr = uiRowAddr;
			transParam.fifoDir = _FIFO_READ;
			transParam.transMode = _PIO_MODE;
			transParam.type = _CMD_ADDR;
			transParam.uiCS = 0;

			nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

			nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

			nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);

			nand_host_send_command(info, uiCmdCycle.Reg, FALSE);

			ret = nand_cmd_wait_complete(info);

			if (ret != E_OK)
				break;

			transParam.colAddr = 0;
			transParam.rowAddr = 0;
			transParam.type = _AUTO_STATUS;

			nand_host_setup_transfer(info, &transParam, 0, 1, _SINGLE_PAGE);

			nand_hostSetupStatusCheckBit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			nand_host_send_command(info, SPINAND_CMD_GET_FEATURE | (SPINAND_FEATURE_STATUS <<8) , 0);

			ret = nand_cmd_wait_complete(info);
		} else {
			// write enable
			transParam.colAddr = uiColAddr;
			transParam.rowAddr = uiRowAddr;
			transParam.fifoDir = _FIFO_WRITE;
			transParam.transMode = _DMA_MODE;
			transParam.type = _SPI_PROG_CMD;
			transParam.uiCS = 0;

			NAND_SETREG(info, NAND_DMASTART_REG_OFS, dma_getPhyAddr((UINT32)(pulBuf)));

			if (uiBufSize > info->flash_info->page_size) {
				nand_host_setup_transfer(info, &transParam, 1, info->flash_info->page_size, _SINGLE_PAGE);
				flush_dcache_range((UINT32)pulBuf, (UINT32)(pulBuf + info->flash_info->page_size));
			} else {
				nand_host_setup_transfer(info, &transParam, 1, uiBufSize, _SINGLE_PAGE);
				flush_dcache_range((UINT32)pulBuf, (UINT32)(pulBuf + uiBufSize));
			}

			uiCmdCycle.Bit.CMD_CYC_1ST = SPINAND_CMD_WEN;

			if (info->flash_info->spi_nand_status.bQuadProgram) {
				uiCmdCycle.Bit.CMD_CYC_2ND = SPINAND_CMD_PROGRAM_LOADX4;

				nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);
			} else {
				uiCmdCycle.Bit.CMD_CYC_2ND = SPINAND_CMD_PROGRAM_LOAD;

				nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);
			}

			uiCmdCycle.Bit.CMD_CYC_3RD = SPINAND_CMD_PROGRAM_EXE;

			uiCmdCycle.Bit.CMD_CYC_4TH= SPINAND_CMD_GET_FEATURE;

			uiCmd2Cycle.Bit.CMD_CYC_5TH = SPINAND_FEATURE_STATUS;

			nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

			nand_hostSetupStatusCheckBit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

			NAND_SETREG(info, NAND_COMMAND2_REG_OFS, uiCmd2Cycle.Reg);

			nand_host_send_command(info, uiCmdCycle.Reg, TRUE);

			ret = nand_cmd_wait_complete(info);

			if (ret != E_OK)
				break;
		}

		ucStatus = (UINT8)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

		if(ucStatus & SPINAND_FEATURE_STATUS_PROG_FAIL)
			ret = E_SYS;

	} while (0);

#if defined(CONFIG_NVT_FW_UPDATE_LED) && !defined(CONFIG_NVT_PWM)
	led_set_gpio_low();
#endif
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
	struct smc_setup_trans transParam;

	transParam.colAddr = 0;
	transParam.rowAddr = 0;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _CMD;
	transParam.uiCS = 0;

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

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
	struct smc_setup_trans transParam;
	uint32_t spi_cmd = 0;

	transParam.colAddr = 0;
	transParam.rowAddr = 0;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _CMD_STS;
	transParam.uiCS = info->flash_info->chip_sel;

	nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &transParam, 0, 1, _SINGLE_PAGE);

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
	struct smc_setup_trans transParam;
	uint32_t spi_cmd = 0;
	union T_NAND_COMMAND_REG  uiCmdCycle= {0};

	uiCmdCycle.Bit.CMD_CYC_1ST = SPINAND_CMD_SET_FEATURE;
	if (set == NAND_SPI_STS_FEATURE_1)
		spi_cmd = SPINAND_FEATURE_BLOCK_LOCK << 8;
	else if (set == NAND_SPI_STS_FEATURE_2)
		spi_cmd = SPINAND_FEATURE_OPT << 8;
	else
		spi_cmd = SPINAND_FEATURE_STATUS << 8;

	spi_cmd |= status;

	transParam.colAddr = 0;
	transParam.rowAddr = spi_cmd;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _CMD_ADDR;
	transParam.uiCS = 0;

	nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_2_CYCLE_CNT, NAND_COL_ADDRESS_2_CYCLE_CNT);

	nand_host_send_command(info, uiCmdCycle.Reg, 0);

	ret = nand_cmd_wait_complete(info);

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
	int ret;
	struct smc_setup_trans transParam;

	transParam.colAddr = 0;
	transParam.rowAddr = 0;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _CMD_STS;
	transParam.uiCS = 0;

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, config, 0);

	ret = nand_cmd_wait_complete(info);

	*(info->data_buff) = (UINT8)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

	return ret;
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
	struct smc_setup_trans transParam;
	UINT8 ucStatus;
	ER ret = E_OK;

#if defined(CONFIG_NVT_FW_UPDATE_LED) && !defined(CONFIG_NVT_PWM)
	led_set_gpio_high(OP_ERASE);
#endif

	do {
		ret = spi_nand_unlock_BP(info);
		if (ret != E_OK)
			break;

		// write enable
		// ___|cmd|___ = 0x07 command cycle only

		transParam.colAddr = 0;
		transParam.rowAddr = 0;
		transParam.fifoDir = _FIFO_READ;
		transParam.transMode = _PIO_MODE;
		transParam.type = _CMD;
		transParam.uiCS = 0;

		nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

		nand_host_send_command(info, SPINAND_CMD_WEN, FALSE);

		ret = nand_cmd_wait_complete(info);

		if (ret != E_OK)
			break;

		transParam.colAddr = 0;
		transParam.rowAddr = block_address;
		transParam.fifoDir = _FIFO_WRITE;
		transParam.transMode = _PIO_MODE;
		transParam.type = _BLK_ERSE;
		transParam.uiCS = 0;

		nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

		nand_hostSetupAddressCycle(info, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_COL_ADDRESS_CYCLE_CNT);

		nand_hostSetupStatusCheckBit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

		nand_host_send_command(info, SPINAND_CMD_BLK_ERASE | (SPINAND_CMD_GET_FEATURE << 8) | (SPINAND_FEATURE_STATUS << 16), TRUE);

		ret = nand_cmd_wait_complete(info);

		if (ret != E_OK)
			break;

		ucStatus = (UINT8)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

		if(ucStatus & SPINAND_FEATURE_STATUS_ERASE_FAIL) {
			ret = E_SYS;
			break;
		}

	} while (0);

#if defined(CONFIG_NVT_FW_UPDATE_LED) && !defined(CONFIG_NVT_PWM)
	led_set_gpio_low();
#endif

	return ret;
}
/*
	Read device ID.

	@return Device ID
*/
int nand_cmd_read_id(uint8_t * card_id, struct drv_nand_dev_info *info)
{
	struct smc_setup_trans transParam;

	transParam.colAddr = 0;
	transParam.rowAddr = 0;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _READ_ID;
	transParam.uiCS	= 0;

	nand_host_setup_transfer(info, &transParam, 0, 4, _SINGLE_PAGE);

	nand_host_send_command(info, SPINAND_CMD_JEDECID, FALSE);

	return nand_host_receive_data(info, card_id, 4, _PIO_MODE);
}

/*
    Read ECC corrected bits

    @return correct bits
*/

int nand_cmd_read_flash_ecc_corrected(struct drv_nand_dev_info *info)
{
    	struct smc_setup_trans transParam;
	uint8_t  ecc_bits[4];

	transParam.colAddr = 0;
	transParam.rowAddr = 0;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _READ_ID;
	transParam.uiCS	= 0;

	nand_host_setup_transfer(info, &transParam, 0, 4, _SINGLE_PAGE);

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
	unsigned int time_start, time_now;

	time_start = get_timer(0);

	while(1) {
		if (NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_COMP_STS)
			break;

		time_now = get_timer(0);
		if ((time_now - time_start) > 4500) {
			printf("_COMP_INTR not set\n\r");
			return -EIO;
		}
	}

	NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_COMP_STS);

	if ((NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_STS_FAIL_STS) && \
		!(NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & SPINAND_FEATURE_STATUS_ECC_UNCORR_ERR)) {
		NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_STS_FAIL_STS);
		printf("NAND status fail 0x%x\r\n", NAND_GETREG(info, NAND_CTRL_STS_REG_OFS));
		return -EIO;
	}

	if (NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_TOUT_STS) {
		NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_TOUT_STS);
		printf("NAND cmd timeout\r\n");
		return -EIO;
	}
    	return nand_host_check_ready(info);
}

/*
	Wait SPI flash ready

	Wait SPI flash returned to ready state

	@param[in] uiWaitMs     Timeout setting. (Unit: ms)

	@return
		- @b E_OK: success
		- @b Else: fail. Maybe timeout.
*/
static ER spiflash_wait_ready(struct drv_nand_dev_info *info, u8 * ucStatus)
{
	union T_NAND_COMMAND_REG uiCmdCycle= {0};
	struct smc_setup_trans transParam;
	ER ret = E_OK;

	do {
		transParam.colAddr = 0;
		transParam.rowAddr = 0;
		transParam.fifoDir = _FIFO_READ;
		transParam.transMode = _PIO_MODE;
		transParam.type = _AUTO_STATUS;
		transParam.uiCS = 0;

		nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

		uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_RDSR;

		nand_hostSetupStatusCheckBit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

		nand_host_send_command(info, uiCmdCycle.Reg, TRUE);

		ret = nand_cmd_wait_complete(info);

	} while (0);

	if(ucStatus)
		* ucStatus = (UINT8)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

	return ret;
}

static ER spiflash_disable_write_latch(struct drv_nand_dev_info *info)
{
	struct smc_setup_trans transParam;
	int  ret;

	transParam.colAddr = 0;
	transParam.rowAddr = 0;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _CMD;
	transParam.uiCS = 0;

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, FLASH_CMD_WRDI, FALSE);

	ret = nand_cmd_wait_complete(info);

	return ret;
}

static ER spiflash_enable_write_latch(struct drv_nand_dev_info *info)
{
	struct smc_setup_trans transParam;
	int ret;

	do {
		transParam.colAddr = 0;
		transParam.rowAddr = 0;
		transParam.fifoDir = _FIFO_READ;
		transParam.transMode = _PIO_MODE;
		transParam.type = _CMD;
		transParam.uiCS = 0;

		nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

		nand_host_send_command(info, FLASH_CMD_WREN, FALSE);

		ret = nand_cmd_wait_complete(info);

		if (ret != E_OK)
			break;
	} while(0);

	return ret;
}

/*
	Read pages.

	@param[out] buffer Buffer address
	@param pageAddress The address of the page. Only (n * g_pNandInfo->uiBytesPerPageData) will be valid.
			Beware that the max accessible size is 4GB.  One should use nand_readOperation2() instead.
	@param numPage How many pages
	@return
		- @b E_OK       read operation success
		- @b E_PAR      parameter error
		- @b E_SYS
*/
int nand_cmd_read_operation(struct drv_nand_dev_info *info, int8_t * buffer, uint32_t pageAddress, uint32_t numPage)
{
	const struct nvt_nand_flash *f = info->flash_info;

	return spiNand_readPage(info, pageAddress / f->page_size, 0, (uint8_t *)buffer, numPage*f->page_size);
}


/*
	Write pages.(single page operation)

	@param buffer      Buffer address
	@param pageAddress The address of the page. Only (n * g_pNandInfo->uiBytesPerPageData) will be valid.
		Beware that the max accessible size is 4GB.  One should use write_readOperation2() instead.
	@param numPage     How many pages
	@return E_OK or E_SYS
*/
int nand_cmd_write_operation_single(struct drv_nand_dev_info *info, int8_t * buffer, uint32_t pageAddress, uint32_t column)
{
	const struct nvt_nand_flash *f = info->flash_info;
	uint32_t buf_len = info->buf_count - column;

	return spiNand_programPage(info, pageAddress / f->page_size, column, (UINT8 *)buffer, buf_len, 0, 0);
}

void nand_store_cache(void *pbuf, u32 len)
{
	return;
}

void nand_cmd_write_spare_sram(struct drv_nand_dev_info *info)
{
	uint32_t spare_area_addr;
	uint32_t *pul_buf = (uint32_t *)(info->data_buff + info->flash_info->page_size);
	uint32_t section_count_per_page = (info->flash_info->page_size / 0x200);
	int j = 0;

	flush_dcache_range((UINT32)pul_buf, (UINT32)(pul_buf + info->flash_info->oob_size));

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
		*(uint32_t *)(spare_area_addr+0x0) = *(pul_buf + 0) ;
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
	@param pageAddress The address of the page
	@return
		- @b E_OK       read spare success
		- @b E_SYS      read spare operation fail(status fail)
		- @b E_CTX      read spare encounter ecc uncorrect error(Only if Reed solomon ecc usage)
*/
int nand_cmd_read_page_spare_data(struct drv_nand_dev_info *info, int8_t *buffer, uint32_t page_address)
{
	const struct nvt_nand_flash *f = info->flash_info;

	return spiNand_readPage(info, page_address / f->page_size, f->page_size, (uint8_t *)buffer, f->oob_size);
}

/*
    Read the largest number of ecc corrected bits
*/
int nand_cmd_read_ecc_corrected(struct drv_nand_dev_info *info)
{
	union T_NAND_RSERR_STS0_REG reg_correct = {0};
	u8 correct_count = 0;

	reg_correct.Reg = NAND_GETREG(info, NAND_RSERR_STS0_REG_OFS);

	/*Read the largest number of correct bits*/
	correct_count = reg_correct.Bit.SEC0_ERR_STS;
	if (reg_correct.Bit.SEC1_ERR_STS > correct_count)
		correct_count = reg_correct.Bit.SEC1_ERR_STS;

	if (reg_correct.Bit.SEC2_ERR_STS > correct_count)
		correct_count = reg_correct.Bit.SEC2_ERR_STS;

	if (reg_correct.Bit.SEC3_ERR_STS > correct_count)
		correct_count = reg_correct.Bit.SEC3_ERR_STS;

	return correct_count;
}

/*
	Dummy read command

	Dummy read command

	@return
		- @b E_SYS      Status fail
		- @b E_TMOUT    Controller timeout
		- @b E_OK       Operation success

	@note for nand_cmd_wait_complete()
*/
int nand_cmd_dummy_read(struct drv_nand_dev_info *info)
{
	return 0;
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
	union T_NAND_COMMAND_REG uiCmdCycle= {0};
	struct smc_setup_trans transParam;
	int ret = E_OK;

	/*Disable clk_auto_gating before set cs_manual*/
	*(u32*) 0xF00200E0 &= ~0x80000;

	nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);

	nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_MANUAL_MODE);

	nand_hostSetCSActive(info, NAND_SPI_CS_LOW);

	if (info->flash_info->nor_read_mode == SPI_NOR_QUAD_READ) {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_QUAD_READ_NORMAL_4BYTE;
		else
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_QUAD_READ_NORMAL;
	} else if (info->flash_info->nor_read_mode == SPI_NOR_DUAL_READ) {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_DUAL_READ_4BYTE;
		else
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_DUAL_READ;
	} else {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_READ_4BYTE;
		else
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_READ;
	}



	transParam.colAddr      = 0;
	transParam.rowAddr      = addr;
	transParam.fifoDir      = _FIFO_READ;
	transParam.transMode    = _PIO_MODE;
	transParam.type         = _CMD_ADDR;
	transParam.uiCS         = 0;

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
		nand_hostSetupAddressCycle(0, NAND_ROW_ADDRESS_4_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);
	else
		nand_hostSetupAddressCycle(0, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);

	nand_host_send_command(info, uiCmdCycle.Reg, FALSE);

	ret = nand_cmd_wait_complete(info);

	if(ret != E_OK) {
		nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);
		nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);
		return ret;
	}

	transParam.colAddr = 0;
	transParam.rowAddr = 0;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _DMA_MODE;
	transParam.type = _SPI_READ_N_BYTES;
	transParam.uiCS = 0;

	NAND_SETREG(info, NAND_DMASTART_REG_OFS, dma_getPhyAddr((UINT32)pbuf));

	if (info->flash_info->nor_read_mode != SPI_NOR_NORMAL_READ) {
		nand_host_setup_transfer(info, &transParam, 0, 1, _SINGLE_PAGE);

		nand_host_send_command(info, uiCmdCycle.Reg, 0);

		ret = nand_cmd_wait_complete(info);

		if(ret != E_OK) {
			nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);
			nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);
			return ret;
		}
	}

	if (info->flash_info->nor_read_mode == SPI_NOR_QUAD_READ)
		nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);
	else if (info->flash_info->nor_read_mode == SPI_NOR_DUAL_READ)
		nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_DUAL_MODE, NAND_SPI_ORDER_MODE_1);
	else
		nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &transParam, 0, byte_size, _SINGLE_PAGE);

	nand_host_send_command(info, uiCmdCycle.Reg, FALSE);

	ret = nand_cmd_wait_complete(info);

	nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);

	nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);

	if (info->flash_info->nor_read_mode != SPI_NOR_NORMAL_READ)
		nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	/*Enable clk_auto_gating after set cs_manual*/
	*(u32*) 0xF00200E0 |= 0x80000;

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
	union T_NAND_COMMAND_REG uiCmdCycle= {0};
	struct smc_setup_trans transParam;
	int ret;
	u8 ucSts;

	spiflash_enable_write_latch(info);

	/*Disable clk_auto_gating before set cs_manual*/
	*(u32*) 0xF00200E0 &= ~0x80000;

	nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);
	nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_MANUAL_MODE);

	nand_hostSetCSActive(info, NAND_SPI_CS_LOW);

	if (info->flash_info->nor_quad_support) {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_PAGEPROG_4X_4BYTE;
		else
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_PAGEPROG_4X;
	} else {
		if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_PAGEPROG_4BYTE;
		else
			uiCmdCycle.Bit.CMD_CYC_1ST = FLASH_CMD_PAGEPROG;
	}

	transParam.colAddr = 0;
	transParam.rowAddr = addr;
	transParam.fifoDir = _FIFO_READ;
	transParam.transMode = _PIO_MODE;
	transParam.type = _CMD_ADDR;
	transParam.uiCS = 0;

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
		nand_hostSetupAddressCycle(0, NAND_ROW_ADDRESS_4_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);
	else
		nand_hostSetupAddressCycle(0, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT);

	nand_host_send_command(info, uiCmdCycle.Reg, FALSE);

	ret = nand_cmd_wait_complete(info);

	if(ret != E_OK) {
		nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);
		nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);
		return ret;
	}

	transParam.fifoDir = _FIFO_WRITE;
	if (byte_size < SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)
		transParam.transMode = _PIO_MODE;
	else {
		transParam.transMode = _DMA_MODE;
		NAND_SETREG(info, NAND_DMASTART_REG_OFS, dma_getPhyAddr((UINT32)pbuf));
	}

	transParam.type = _SPI_WRITE_N_BYTES;

	if (info->flash_info->nor_quad_support)
		nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_QUAD_MODE, NAND_SPI_ORDER_MODE_1);
	else
		nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	nand_host_setup_transfer(info, &transParam, 0, byte_size, _SINGLE_PAGE);

	nand_host_send_command(info, uiCmdCycle.Reg, FALSE);

	if (byte_size < SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)
		ret = nand_host_transmit_data(info, (u8*)pbuf, byte_size, _PIO_MODE);
	else
		ret = nand_cmd_wait_complete(info);

	nand_hostSetCSActive(info, NAND_SPI_CS_HIGH);
	nand_hostConfigChipSelOperationMode(info, NAND_SPI_CS_AUTO_MODE);

	/*Enable clk_auto_gating after set cs_manual*/
	*(u32*) 0xF00200E0 |= 0x80000;

	nand_hostSetSPIIORegister(info, NAND_SPI_CS_ACTIVE_LOW, NAND_SPI_NORMAL_DUPLEX_MODE, NAND_SPI_ORDER_MODE_1);

	if(ret != E_OK)
		return ret;

	ret = spiflash_wait_ready(info, &ucSts);

	spiflash_disable_write_latch(info);

	return ret;
}

int spinor_erase_sector(struct drv_nand_dev_info *info, struct spi_flash *flash, u32 byte_addr)
{
	union T_NAND_COMMAND_REG cmd_cycle = {0};
	struct smc_setup_trans transParam;
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

	cmd_cycle.Bit.CMD_CYC_1ST = flash->erase_cmd;
	cmd_cycle.Bit.CMD_CYC_2ND = FLASH_CMD_RDSR;
	transParam.colAddr = 0;
	transParam.rowAddr = byte_addr;
	transParam.fifoDir = _FIFO_WRITE;
	transParam.transMode = _PIO_MODE;
	transParam.type = _BLK_ERSE;
	transParam.uiCS = 0;

	if (info->flash_info->device_size > SPI_FLASH_16MB_BOUN)
		nand_hostSetupAddressCycle(0, NAND_ROW_ADDRESS_4_CYCLE_CNT, NAND_NOT_CONFIGED_COL_ADDRESS_CYCLE_CNT);
	else
		nand_hostSetupAddressCycle(0, NAND_ROW_ADDRESS_3_CYCLE_CNT, NAND_NOT_CONFIGED_COL_ADDRESS_CYCLE_CNT);

	nand_hostSetupStatusCheckBit(info, NAND_STS_BIT_0, NAND_STS_BIT_0_EQUAL_0);

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, cmd_cycle.Reg, TRUE);

	ret = nand_cmd_wait_complete(info);

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
	int ret = E_SYS;
	read_cycle = byte_size / SPIFLASH_MAX_READ_BYTE_AT_ONCE;
	addr_index = byte_addr;
	remain_byte = byte_size % SPIFLASH_MAX_READ_BYTE_AT_ONCE;
	invalidate_dcache_range((u32)ALIGN_FLOOR((u32)pbuf,ARCH_DMA_MINALIGN), (u32)roundup((u32)(pbuf + byte_size), ARCH_DMA_MINALIGN));
	for(cycle_index = 0; cycle_index < read_cycle; cycle_index++, addr_index += SPIFLASH_MAX_READ_BYTE_AT_ONCE) {
		ret = spiflash_read_data(info, addr_index, SPIFLASH_MAX_READ_BYTE_AT_ONCE, (pbuf + (cycle_index * SPIFLASH_MAX_READ_BYTE_AT_ONCE)));
		if (ret != E_OK) {
			printf("%s: fail at flash address 0x%x\r\n", __func__, addr_index);
			break;
		} else
			debug("R");
	}

	if(remain_byte)
	{
		ret = spiflash_read_data(info, addr_index, remain_byte, (pbuf + (cycle_index * SPIFLASH_MAX_READ_BYTE_AT_ONCE)));
		if (ret != E_OK)
			printf("%s: fail at flash address 0x%x\r\n", __func__, addr_index);
		else
			debug("r");
	}

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
	int ret = E_SYS;

	program_cycle = sector_size / SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE;

	addr_index = byte_addr;

	flush_dcache_range((u32)pbuf, (u32)(pbuf + sector_size));

	for(cycle_index = 0; cycle_index < program_cycle; cycle_index++, addr_index += SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE) {
		ret = spiflash_page_program(info, addr_index, SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE, (pbuf + (cycle_index * SPIFLASH_MAX_PROGRAM_BYTE_AT_ONCE)));

		if (ret != E_OK) {
			printf("%s: fail at flash address 0x%x\r\n", __func__, addr_index);
			break;
		} else
			debug("W");

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

int spinor_read_status(struct drv_nand_dev_info *info, u8 status_set, u8* status)
{
	struct smc_setup_trans trans_param;
	u32 spi_cmd = 0;
	int ret = E_OK;

	do {
		trans_param.colAddr = 0;
		trans_param.rowAddr = 0;
		trans_param.fifoDir = _FIFO_READ;
		trans_param.transMode = _PIO_MODE;
		trans_param.type = _CMD_STS;
		trans_param.uiCS = 0;

		nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

		if(status_set == NAND_SPI_NOR_STS_RDSR_1)
			spi_cmd = FLASH_CMD_RDSR;
		else
			spi_cmd = FLASH_CMD_RDSR2;

		nand_host_send_command(info, spi_cmd, FALSE);

		ret = nand_cmd_wait_complete(info);

	} while (0);

	*status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

	return ret;
}

int spinor_write_status(struct drv_nand_dev_info *info, u8 status_set, u8 status)
{
	union T_NAND_COMMAND_REG cmd_cycle = {0};
	struct smc_setup_trans trans_param;
	u32 spi_cmd = 0;
	int ret = E_OK;
	u8 wait_status = 0;

	spiflash_enable_write_latch(info);

	do {
		//row addr = 0x44332211 --> 0x44 --> 0x33 --> 0x22 --> 0x11
		//                                            ___     ___________
		//use command + 1 cycle row address cycle ___|cmd|___|Row1stcycle|___

		if(status_set == NAND_SPI_NOR_STS_WRSR_1)
			cmd_cycle.Bit.CMD_CYC_1ST = FLASH_CMD_WRSR;
		else
			cmd_cycle.Bit.CMD_CYC_1ST = FLASH_CMD_WRSR2;

		spi_cmd = status;

		trans_param.colAddr = 0;
		trans_param.rowAddr = spi_cmd;
		trans_param.fifoDir = _FIFO_READ;
		trans_param.transMode = _PIO_MODE;
		trans_param.type = _CMD_ADDR;
		trans_param.uiCS = 0;

		nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);

		nand_hostSetupAddressCycle(0, NAND_ROW_ADDRESS_1_CYCLE_CNT, NAND_COL_ADDRESS_1_CYCLE_CNT);

		nand_host_send_command(info, cmd_cycle.Reg, FALSE);

		ret = nand_cmd_wait_complete(info);

		if (ret)
			return ret;

	} while (0);

	ret = spiflash_wait_ready(info, &wait_status);
	if (ret)
		printf("wait ready timeout with status 0x%x\n", wait_status);

	return ret;
}

int spiflash_boot_detect(struct drv_nand_dev_info *info)
{
	union T_NAND_SPI_IO_REG reg_io = {0};

	reg_io.Reg = NAND_GETREG(info, NAND_SPI_IO_REG_OFS);
	if (0 == reg_io.Bit.CPU1_INIT) {
		reg_io.Bit.CPU2_INIT = 1;
		NAND_SETREG(info, NAND_SPI_IO_REG_OFS, reg_io.Reg);
	}

	return reg_io.Bit.CPU1_INIT;
}
