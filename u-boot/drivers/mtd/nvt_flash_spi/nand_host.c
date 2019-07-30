/*
	@file       nand_host.c
	@ingroup    mIStgNAND

	@brief      NAND low layer driver which will access to NAND controller

	@note       Nothing

	Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

*/
#include "nand_int.h"
#include "nand_reg.h"

/*
	Configure nand host transfer register

	This function configure controller transfer related register

	@param[in]  transParam      Setup transfer configuration
	@param[in]  pageCount       Transfer page count
	@param[in]  length          Transfer length
	@param[in]  multiPageSelect multi page / multi spare / single page

	@return void
*/
void nand_host_setup_transfer(struct drv_nand_dev_info *info, \
				struct smc_setup_trans *transParam, \
				uint32_t pageCount, \
				uint32_t length, \
				uint32_t multiPageSelect)
{
	const struct nvt_nand_flash *f = info->flash_info;
	uint32_t value3 = 0;
	uint32_t value = 0;

	uint32_t value1 = 0;

	value3 = NAND_GETREG(info, NAND_INTMASK_REG_OFS);
	value3 &= ~NAND_COMP_INTREN;
	NAND_SETREG(info, NAND_INTMASK_REG_OFS, value3);

	if (pageCount > 0) {
		NAND_SETREG(info, NAND_PAGENUM_REG_OFS, pageCount);
		value = multiPageSelect;
	}

	value1 = NAND_GETREG(info, NAND_CTRL0_REG_OFS);

	NAND_SETREG(info, NAND_CTRL0_REG_OFS, value);


	// Set column address
	NAND_SETREG(info, NAND_COLADDR_REG_OFS, transParam->colAddr);

	// Set row address
	NAND_SETREG(info, NAND_ROWADDR_REG_OFS, transParam->rowAddr);

	// Set data length
	NAND_SETREG(info, NAND_DATALEN_REG_OFS, length);

	if((value1 & _PROTECT1_EN) == _PROTECT1_EN)
		value |= _PROTECT1_EN;

	if((value1 & _PROTECT2_EN) == _PROTECT2_EN)
		value |= _PROTECT2_EN;

	value |= (transParam->type | transParam->uiCS);

	if(transParam->fifoDir == _FIFO_READ)
		value |= _NAND_WP_EN;

	NAND_SETREG(info, NAND_CTRL0_REG_OFS, value);

	value3 = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
	value3 |= NAND_COMP_STS;
	NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, value3);

	// Set control0 register
	if (length && ((length > GET_NAND_SPARE_SIZE(f)) || \
					(length == 8) || (length == 4) || \
					multiPageSelect == _MULTI_SPARE)) {
		NAND_SETREG(info, NAND_FIFO_CTRL_REG_OFS, \
			(transParam->fifoDir | transParam->transMode));

		NAND_SETREG(info, NAND_FIFO_CTRL_REG_OFS, (_FIFO_EN | \
							transParam->fifoDir | \
							transParam->transMode));
	}

	if ((pageCount == 0) && (length == 0x40)) {
		NAND_SETREG(info, NAND_FIFO_CTRL_REG_OFS, \
			(transParam->fifoDir | transParam->transMode));

		NAND_GETREG(info, NAND_FIFO_CTRL_REG_OFS);

		NAND_SETREG(info, NAND_FIFO_CTRL_REG_OFS, (_FIFO_EN | \
							transParam->fifoDir | \
							transParam->transMode));
	}

	value3 = NAND_GETREG(info, NAND_INTMASK_REG_OFS);
	value3 |= NAND_COMP_INTREN;
	NAND_SETREG(info, NAND_INTMASK_REG_OFS, value3);
}


/*
	Configure SM transfer command

	Command send to NAND

	@param[in]  command     nand command
	@param[in]  bTmOutEN    time out enable / disable

	@return void
*/
void nand_host_send_command(struct drv_nand_dev_info *info, uint32_t command,
							uint32_t bTmOutEN)
{
	union T_NAND_CTRL0_REG RegCtrl0;

	//Set command
	NAND_SETREG(info, NAND_COMMAND_REG_OFS, command);

	RegCtrl0.Reg = NAND_GETREG(info, NAND_CTRL0_REG_OFS);

	RegCtrl0.Bit.OPER_EN = 1;

	RegCtrl0.Bit.TIMEOUT_EN = bTmOutEN;

	NAND_SETREG(info, NAND_CTRL0_REG_OFS, RegCtrl0.Reg);
}


/*
	NAND controller host receive data routing

	After read operation related command issued, called this function to reveive data

	@param[in]  	buffer      buffer receive to
			length      receive buffer length
			transMode   PIO/DMA mode usage

	@return
		- @b E_OK           receive data success
		- @b E_SYS          error status
*/
int nand_host_receive_data(struct drv_nand_dev_info *info, uint8_t *buffer,
					uint32_t length, uint32_t transMode)
{
	int32_t index;

	if(transMode == _PIO_MODE) {

		length = ((length+3)/4) * 4;

		while(length) {
			if(length < 128) {
				if((NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_CNT_MASK) == (length/4)) {
					for (index = length; index > 0; index -= 4) {
						*((uint32_t *)buffer) = NAND_GETREG(info, NAND_DATAPORT_REG_OFS);
						buffer += 4;
						length -= 4;
					}
				}
			} else {
				if(NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_FULL) {
					for (index = 128; index > 0; index -= 4) {
						*((uint32_t *)buffer) = NAND_GETREG(info, NAND_DATAPORT_REG_OFS);
						buffer += 4;
						length -= 4;
					}
				}
			}
		}
	}

	return nand_cmd_wait_complete(info);
}


/*
	NAND controller host transmit data routing

	After write operation related command issued, called this function to transmit data

	@param[in]  	buffer      buffer transmit from
			length      transmit buffer length
			transMode   PIO/DMA mode usage

	@return
		- @b E_OK           transmit data success
		- @b E_SYS          error status
*/
int nand_host_transmit_data(struct drv_nand_dev_info *info, uint8_t *buffer, uint32_t length, uint32_t transMode)
{
	int32_t index;

	if(transMode == _PIO_MODE) {

		length = ((length+3)/4) * 4;

		while(length) {
			if(length < 128) {
				if(NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_EMPTY) {
					for (index = length; index > 0; index -= 4) {
						NAND_SETREG(info, NAND_DATAPORT_REG_OFS, *((uint32_t *)buffer));
						buffer += 4;
						length -= 4;
					}
				}
			} else {
				if(NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_EMPTY) {
					for (index = 128; index > 0; index -= 4) {
						NAND_SETREG(info, NAND_DATAPORT_REG_OFS, *((uint32_t *)buffer));
						buffer += 4;
						length -= 4;
					}
				}
			}
		}
	}

	return nand_cmd_wait_complete(info);
}


/*
	Check NAND read ready bit and make sure status register is ready

	@return E_OK : ready
		E_TMOUT: busy
*/
int nand_host_check_ready(struct drv_nand_dev_info *info)
{
#if (CONFIG_NVT_NAND_TYPE == CONFIG_NAND_TYPE_ONFI)
	uint32_t i;

	i = 0;
	do {
		i++;
		//For slow card
		if (i > 0x48000) {
			printf("Status always busy\n\r");
			return -1;
		}
	} while ((NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_SM_BUSY) == 0);
#endif
	return E_OK;
}


/*
	Enable / Disable FIFO

	Enable and Disable FIFO of NAND controller

	@param[in]  uiEN     enable / disable

	@return void
*/
void nand_host_set_fifo_enable(struct drv_nand_dev_info *info, uint32_t uiEN)
{
	union T_NAND_FIFO_CTRL_REG RegFIFOCtrl;

	RegFIFOCtrl.Reg = NAND_GETREG(info, NAND_FIFO_CTRL_REG_OFS);

	RegFIFOCtrl.Bit.FIFO_EN = uiEN;

	NAND_SETREG(info, NAND_FIFO_CTRL_REG_OFS, RegFIFOCtrl.Reg);

	if(uiEN == FALSE)
		while((NAND_GETREG(info, NAND_FIFO_CTRL_REG_OFS) & _FIFO_EN) != 0);
}


/*
	Set NAND controller error correction SRAM access right(for CPU or NAND controller)

	Set register to switch error information SRAM access right to CPU or NAND controller

	@param[in]  bcpu_Acc
		- @b TRUE       switch error information sram area access right to CPU
		- @b FALSE      Switch error information sram area access right to NAND controller access

	@return
		- void
*/
void nand_host_set_cpu_access_err(struct drv_nand_dev_info *info, uint32_t bcpu_Acc)
{
	union T_NAND_SRAM_ACCESS_REG  RegSramAcc = {0x00000000};
	uint8_t i;

	RegSramAcc.Reg = NAND_GETREG(info, NAND_SRAM_ACCESS_REG_OFS);

	RegSramAcc.Bit.ERROR_ACC = bcpu_Acc;

	NAND_SETREG(info, NAND_SRAM_ACCESS_REG_OFS,RegSramAcc.Reg);

	//dummy read to delay 200ns for SRAM ready
	if(bcpu_Acc == TRUE) {
		for(i = 0; i < SRAMRDY_DELAY; i++)
			RegSramAcc.Reg = NAND_GETREG(info, NAND_SRAM_ACCESS_REG_OFS);
	}
}


/*
	Set NAND controller spare SRAM area access right(for CPU or NAND controller)

	Set register to switch spare area SRAM access right to CPU or NAND controller

	@param[in]  bcpu_Acc
		- @b TRUE       switch spare sram area access right to CPU
		- @b FALSE      switch spare sram area access right to NAND controller

	@return
		- void
*/
void nand_host_set_cpu_access_spare(struct drv_nand_dev_info *info,
							uint32_t bcpu_Acc)
{
	union T_NAND_SRAM_ACCESS_REG  RegSramAcc = {0x00000000};
	uint8_t i;

	RegSramAcc.Reg = NAND_GETREG(info, NAND_SRAM_ACCESS_REG_OFS);

	RegSramAcc.Bit.SPARE_ACC = bcpu_Acc;

	NAND_SETREG(info, NAND_SRAM_ACCESS_REG_OFS,RegSramAcc.Reg);

	//dummy read to delay 200ns for SRAM ready
	if(bcpu_Acc == TRUE) {
		for(i = 0; i < SRAMRDY_DELAY; i++)
			RegSramAcc.Reg = NAND_GETREG(info, NAND_SRAM_ACCESS_REG_OFS);
	}
}

/*
	NAND Reed Solomon ECC correction routing

	ECC correction flow by using reed solomon method,
	including correct error in spare area first 6 bytes

	@param[in]  Buf         Buffer to correct by RS correct routing
	@param[in]  section     section to be corrected

	@return
		- @b E_OK       success
		- @b E_CTX      uncorrect error
*/
int nand_host_correct_reedsolomon_ecc(struct drv_nand_dev_info *info,
					uint8_t * Buf, uint32_t uiSection)
{
	int32_t ret = 0;
	uint32_t i, j, shifts;
	uint32_t hrdata, rs_sts, rs_err, err_reg, err_addr, err_bit, mask_bit;

	nand_host_set_cpu_access_err(info, TRUE);

	rs_sts = NAND_GETREG(info, NAND_RSERR_STS0_REG_OFS);

	for(i = 0; i < uiSection; i++) {
		shifts = 4 * i;
		rs_err = (rs_sts >> shifts) & 0x7;
		if(rs_err == 0)
			continue;
		else if(rs_err == 5) {
			ret = E_CTX;
			continue;
		}

		for(j = 0; j < rs_err; j++) {

			err_reg = NAND_SEC0_EADDR0_REG_OFS + (16 * i) + (4 * j);

			hrdata = NAND_GETREG(info, err_reg);
			err_addr = hrdata & 0xFFFF;

			if(err_addr >= _512BYTE) {
				nand_host_set_cpu_access_spare(info, TRUE);
				err_addr -= _512BYTE;
				err_addr += (uint32_t)((CONFIG_SYS_NAND_BASE + NAND_SPARE00_REG_OFS) + (i * 16));
			}

			err_bit = (hrdata >> 16) & 0xFF;

			mask_bit = (err_addr & 0x03) * 8;

			if((hrdata & 0xFFFF) >= _512BYTE) {
				* (volatile uint32_t *) (err_addr & 0xFFFFFFFC) ^= (err_bit << mask_bit);
				nand_host_set_cpu_access_spare(info, FALSE);
			} else
				Buf[err_addr+ i*_512BYTE] ^= (err_bit);   // correct bit
		}
	}

	nand_host_set_cpu_access_err(info, FALSE);

	return ret;
}

/*
	NAND secondary ECC correction routing

	ECC correction flow by using secondary ECC method,

	@param[in]  Buf         Buffer to correct by RS correct routing
	@param[in]  section     section to be corrected

	@return
		- @b E_OK       success
		- @b E_CTX      uncorrect error
*/
int nand_host_correct_secondary_ecc(struct drv_nand_dev_info *info,
					uint32_t uiSection)
{
	int32_t ret = E_OK;
	uint32_t i, shifts;
	uint32_t hrdata, ham_sts, ham_err, err_reg, err_addr, err_bit;
	uint32_t mask_bit;

	nand_host_set_cpu_access_err(info, TRUE);

	ham_sts = NAND_GETREG(info, NAND_SECONDARY_ECC_STS_REG_OFS);

	for(i = 0; i < uiSection; i++) {   // 8 sections
		shifts = 2 * i;
		ham_err = (ham_sts >> shifts) & 0x3;
		if(ham_err == 0)
			continue;
		else if(ham_err == 2) {
			ret = E_CTX;
			continue;
		} else if(ham_err == 3) {
			ret = E_CTX;
			continue;
		}
		err_reg = NAND_SEC0_EADDR0_REG_OFS + (16 * i) + 4;

		hrdata = NAND_GETREG(info, err_reg);

		err_addr = hrdata & _ECC_SEC0_SECONDARY_ERRBYTE;
		err_bit = (hrdata & _ECC_SEC0_SECONDARY_ERRBIT)>>_ECC_SEC0_SECONDARY_BITSFT;

		if(err_addr >= _512BYTE) {
			nand_host_set_cpu_access_spare(info, TRUE);
			err_addr -= _512BYTE;
			err_addr += (uint32_t)(CONFIG_SYS_NAND_BASE + NAND_SPARE00_REG_OFS + i * 16);
			mask_bit = (err_addr & 0x03) * 8;

			* (volatile uint32_t *) (err_addr & 0xFFFFFFFC) ^= ((1<<err_bit) << mask_bit);

			nand_host_set_cpu_access_spare(info, FALSE);
		} else {
			printf("Secondary ECC should occurred > section size[0x%08x]\r\n", _512BYTE);
			ret = E_SYS;
			break;
		}
	}

	nand_host_set_cpu_access_err(info, FALSE);

	return ret;
}

/*
	Reset NAND dll delay module

	Issue SW reset of NAND dll delay module

	@return void
*/
void nand_dll_reset(void)
{
	union T_NAND_DLL_PHASE_DLY_REG0 RegDllCtrl0;

	RegDllCtrl0.Reg = NAND_GETREG(NULL, NAND_DLL_PHASE_DLY_REG0_OFS);

	RegDllCtrl0.Bit.PHY_DATA_DLY_SEL = 0x8;

	RegDllCtrl0.Bit.PHY_SW_RESET = 1;

	NAND_SETREG(NULL, NAND_DLL_PHASE_DLY_REG0_OFS, RegDllCtrl0.Reg);

	do {
		RegDllCtrl0.Reg = NAND_GETREG(NULL, NAND_DLL_PHASE_DLY_REG0_OFS);
	} while(RegDllCtrl0.Bit.PHY_SW_RESET == 1);
}

/*
	Configure nand host page size

	This function configure controller of SPI NAND page size

	@param[OUT]
	@return
		- @b_MULTI_PAGE
		- @b_SINGLE_PAGE

*/
UINT32 nand_hostGetMultipageSelect(void)
{
	union T_NAND_CTRL0_REG  RegCtrl = {0};

	RegCtrl.Reg = NAND_GETREG(info, NAND_CTRL0_REG_OFS);
	return (RegCtrl.Reg & 0x00030000);
}
/*
	set nand type

	set nand type

	@return
		- @b E_NOSPT    Not support
		- @b E_OK
*/
int nand_hostSetNandType(struct drv_nand_dev_info *info, NAND_TYPE_SEL uiNandType)
{
	union T_NAND_MODULE0_REG RegMdule0 = {0};
	int uRet = E_OK;

	RegMdule0.Reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);

	switch(uiNandType) {
	case NANDCTRL_ONFI_NAND_TYPE:
		RegMdule0.Bit.NAND_TYPE = 0;
		RegMdule0.Bit.SPI_FLASH_TYPE = 0;
	break;

	case NANDCTRL_SPI_NAND_TYPE:
		RegMdule0.Bit.NAND_TYPE = 1;
		RegMdule0.Bit.SPI_FLASH_TYPE = 0;
		RegMdule0.Bit.COL_ADDR = NAND_1COL_ADDR;
		RegMdule0.Bit.ROW_ADD = NAND_1ROW_ADDR;
	break;

	case NANDCTRL_SPI_NOR_TYPE:
		RegMdule0.Bit.NAND_TYPE = 1;
		RegMdule0.Bit.SPI_FLASH_TYPE = 1;
		RegMdule0.Bit.COL_ADDR = NAND_1COL_ADDR;
		RegMdule0.Bit.ROW_ADD = NAND_1ROW_ADDR;
	break;

	default:
		return E_NOSPT;
	break;
	}

	NAND_SETREG(info, NAND_MODULE0_REG_OFS, RegMdule0.Reg);

	return uRet;
}


/*
	Configure nand host row and column address cycle

	This function configure controller row & column cycle

	@param[in]  uiCS        chip select
	@param[in]  uiRow       row     cycle
	@param[in]  uiColumn    column  cycle

	@return
		- @b E_SYS      Status fail
		- @b E_OK       Operation success

*/
int nand_hostSetupAddressCycle(struct drv_nand_dev_info *info, NAND_ADDRESS_CYCLE_CNT uiRow, NAND_ADDRESS_CYCLE_CNT uiCol)
{
	union T_NAND_MODULE0_REG  RegMdule0 = {0};

	RegMdule0.Reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);

	if(uiCol > NAND_3_ADDRESS_CYCLE_CNT && uiCol != NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT)
		return E_SYS;

	if(uiCol != NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT)
		RegMdule0.Bit.COL_ADDR = uiCol;

	if(uiRow != NAND_NOT_CONFIGED_ADDRESS_CYCLE_CNT)
		RegMdule0.Bit.ROW_ADD = uiRow;

	NAND_SETREG(info, NAND_MODULE0_REG_OFS, RegMdule0.Reg);

	return E_OK;
}

/*
	Configure nand host page size

	This function configure controller of SPI NAND page size

	@param[in]  uiCS        chip select
	@param[in]  uiRow       row     cycle
	@param[in]  uiColumn    column  cycle

	@return
		- @b E_SYS      Status fail
		- @b E_OK       Operation success

*/
int nand_hostSetupPageSize(struct drv_nand_dev_info *info, NAND_PAGE_SIZE uiPageSize)
{
	union T_NAND_MODULE0_REG RegMdule0 = {0};

	RegMdule0.Reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);

	if(uiPageSize == NAND_PAGE_SIZE_512 || uiPageSize == NAND_PAGE_SIZE_2048 || uiPageSize == NAND_PAGE_SIZE_4096) {
		RegMdule0.Bit._PAGE_SIZE = uiPageSize;
		NAND_SETREG(info, NAND_MODULE0_REG_OFS, RegMdule0.Reg);
		return E_OK;
	} else
		return E_SYS;
}



/*
	NAND controller enable polling bit match operation

	Enable bit value compare function.
	After invoke spi_enBitMatch(), it will send uiCmd to SPI device
	and continously read data from SPI device.
	Once bit position specified by uiBitPosition of read data becomes bWaitValue,
	SPI module will stop checking and raise a interrupt.
	Caller of spi_enBitMatch() can use spi_waitBitMatch() to wait this checking complete.

	@param[in] uiStatusValue    The check value
	@param[in] uiStatusMask     Indicates which bit of status value you want to check. 1 for comparison and 0 for not comparison

	@return
		- @b E_OK: success
		- @b Else: fail
*/
int nand_hostSetupStatusCheckBit(struct drv_nand_dev_info *info, u8 uiStatusMask, u8 uiStatusValue)
{
	union T_NAND_STATUS_CHECK_REG RegStsChk = {0};

	RegStsChk.Bit.STATUS_VALUE = uiStatusValue;
	RegStsChk.Bit.STATUS_MASK = uiStatusMask;

	NAND_SETREG(info, NAND_STATUS_CHECK_REG_OFS, RegStsChk.Reg);
	return E_OK;
}

/*
	Configure nand host page size

	This function configure controller of SPI NAND page size

	@param[OUT]
	@return
		- @b_MULTI_PAGE
		- @b_SINGLE_PAGE

*/
ER nand_hostSetSPIIORegister(struct drv_nand_dev_info *info, NAND_SPI_CS_POLARITY uiCSPol, NAND_SPI_BUS_WIDTH uiBusWidth, NAND_SPI_IO_ORDER uiIOOrder)
{
	union T_NAND_SPI_IO_REG  RegIO = {0};

	RegIO.Reg = NAND_GETREG(info, NAND_SPI_IO_REG_OFS);

	RegIO.Bit.SPI_CS_POL    = uiCSPol;
	RegIO.Bit.SPI_BS_WIDTH  = uiBusWidth;
	RegIO.Bit.SPI_IO_ORDER  = uiIOOrder;

	NAND_SETREG(info, NAND_SPI_IO_REG_OFS, RegIO.Reg);
	return E_OK;
}

/*
	Configure nand host chip select manual mode

	Configure nand host chip select manual mode

	@param[in] bModeSel    manual mode or auto mode
		- NAND_SPI_CS_AUTO_MODE     : manual mode(CS configure by user)
		- NAND_SPI_CS_MANUAL_MODE   : auto mode(CS configure by controller)

*/
ER nand_hostConfigChipSelOperationMode(struct drv_nand_dev_info *info, NAND_SPI_CS_OP_MODE bModeSel)
{
	union T_NAND_SPI_CFG_REG  cfgReg = {0};

	cfgReg.Reg = NAND_GETREG(info, NAND_SPI_CFG_REG_OFS);
	cfgReg.Bit.SPI_OPERATION_MODE   = bModeSel;

	NAND_SETREG(info, NAND_SPI_CFG_REG_OFS, cfgReg.Reg);
	return E_OK;
}

/*
	Configure nand host chip select manual mode

	Configure nand host chip select manual mode

	@param[in] bCSLevel    CS (chip select) level
		- NAND_SPI_CS_LOW    : CS set low
		- NAND_SPI_CS_HIGH   : CS set high

*/
ER nand_hostSetCSActive(struct drv_nand_dev_info *info, NAND_SPI_CS_LEVEL bCSLevel)
{
	union T_NAND_SPI_CFG_REG  cfgReg = {0};

	cfgReg.Reg = NAND_GETREG(info, NAND_SPI_CFG_REG_OFS);
	cfgReg.Bit.SPI_NAND_CS   = bCSLevel;

	NAND_SETREG(info, NAND_SPI_CFG_REG_OFS, cfgReg.Reg);
	return E_OK;
}



/*
	Configure nand host timing 2 configuration

	Configure nand host timing 2 configuration

	@param[in] bCSLevel    CS (chip select) level
		- NAND_SPI_CS_LOW    : CS set low
		- NAND_SPI_CS_HIGH   : CS set high

*/
void nand_host_settiming2(struct drv_nand_dev_info *info, u32 time)
{
	union T_NAND_TIME2_REG  time_reg = {0};

	time_reg.Reg = time;

	if (time_reg.Bit.TSHCH < 0x4) {
		printf("SPI NAND TSHCH need >= 0x4, force config as 0x4\n");
		time_reg.Bit.TSHCH = 0x4;
	}

	NAND_SETREG(info, NAND_TIME2_REG_OFS, time_reg.Reg);
}
