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

	//#NT#2012/09/05#Steven Wang -begin
	//#NT#0039432,Temp disable complete interrupt mask
	value3 = NAND_GETREG(info, NAND_INTMASK_REG_OFS);
	value3 &= ~NAND_COMP_INTREN;
	NAND_SETREG(info, NAND_INTMASK_REG_OFS, value3);
	//#NT#2012/09/05#Steven Wang -end

	if (pageCount > 0)
	{
		NAND_SETREG(info, NAND_PAGENUM_REG_OFS, pageCount);
		value = multiPageSelect;
	}
	//#NT#2012/09/05#Steven Wang -begin
	//#NT#0039432, Set single or multipage first
	value1 = NAND_GETREG(info, NAND_CTRL0_REG_OFS);
	NAND_SETREG(info, NAND_CTRL0_REG_OFS, value);
	//#NT#2012/09/05#Steven Wang -end

	// Set column address
	NAND_SETREG(info, NAND_COLADDR_REG_OFS, transParam->colAddr);

	// Set row address
	NAND_SETREG(info, NAND_ROWADDR_REG_OFS, transParam->rowAddr);

	// Set data length
	NAND_SETREG(info, NAND_DATALEN_REG_OFS, length);

	//#NT#2010/02/09#Steven Wang -begin
	//#NT#Only for NT96460
	if((value1 & _PROTECT1_EN) == _PROTECT1_EN)
	{
		value |= _PROTECT1_EN;
	}
	if((value1 & _PROTECT2_EN) == _PROTECT2_EN)
	{
		value |= _PROTECT2_EN;
	}


	value |= (transParam->type | transParam->uiCS);


	if(transParam->fifoDir == _FIFO_READ)
		value |= _NAND_WP_EN;

	NAND_SETREG(info, NAND_CTRL0_REG_OFS, value);

	//#NT#2012/09/05#Steven Wang -begin
	//#NT#1. Clear status
	//#NT#2. Re enable complete interrupt mask
	//#NT#0039432
	value3 = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
	value3 |= NAND_COMP_STS;
	NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, value3);
	//#NT#2012/09/05#Steven Wang -end

	// Set control0 register
	if (length && ((length > GET_NAND_SPARE_SIZE(f)) || \
					(length == 8) || \
					multiPageSelect == _MULTI_SPARE))
	{
		NAND_SETREG(info, NAND_FIFO_CTRL_REG_OFS, (transParam->fifoDir));

		NAND_SETREG(info, NAND_FIFO_CTRL_REG_OFS, (_FIFO_EN | \
							transParam->fifoDir | \
							transParam->transMode));
	}

	//#NT#0039432, re enable comp interrupt
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

    //#NT#2010/08/05#Steven Wang -begin
    //#NT#Add timeout enable
    RegCtrl0.Bit.TIMEOUT_EN = bTmOutEN;
    //#NT#2010/08/05#Steven Wang -end

    NAND_SETREG(info, NAND_CTRL0_REG_OFS, RegCtrl0.Reg);
}


/*
    NAND controller host receive data routing

    After read operation related command issued, called this function to reveive data

    @param[in]  buffer      buffer receive to
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
    uint8_t *eccbuf;

    eccbuf = buffer;

    if(transMode == _PIO_MODE)
    {
        //NAND_IND_MSG("SMHostReceiveData PIO mode\r\n");

        length = ((length+3)/4) * 4;

        while(length)
        {
            if(length < 64)
            {
                if((NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_CNT_MASK) == (length/4))
                {
                    for (index = length; index > 0; index -= 4)
                    {
                        *((uint32_t *)buffer) = NAND_GETREG(info, NAND_DATAPORT_REG_OFS);
                        buffer += 4;
                        length -= 4;
                    }
                }
            }
            else
            {
                if(NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_FULL)
                {
                    for (index = 64; index > 0; index -= 4)
                    {
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

    @param[in]  buffer      buffer transmit from
                length      transmit buffer length
                transMode   PIO/DMA mode usage

    @return
        - @b E_OK           transmit data success
        - @b E_SYS          error status
*/
int nand_host_transmit_data(struct drv_nand_dev_info *info, uint8_t *buffer, uint32_t length, uint32_t transMode)
{
    int32_t index;
    uint8_t *eccbuf;

    eccbuf = buffer;

    if(transMode == _PIO_MODE)
    {
        //NAND_IND_MSG("SMHostTransmitData PIO mode\r\n");

        length = ((length+3)/4) * 4;

        while(length)
        {
            if(length < 64)
            {
                if(NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_EMPTY)
                {
                    for (index = length; index > 0; index -= 4)
                    {
                        NAND_SETREG(info, NAND_DATAPORT_REG_OFS, *((uint32_t *)buffer));
                        buffer += 4;
                        length -= 4;
                    }
                }
            }
            else
            {
                if(NAND_GETREG(info, NAND_FIFO_STS_REG_OFS) & _FIFO_EMPTY)
                {
                    for (index = 64; index > 0; index -= 4)
                    {
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
    uint32_t i;

    i = 0;
    do
    {
        i++;
        //For slow card
        if (i > 0x48000)
        {
            printf("Status always busy\n\r");
//            return -EBUSY;
		return -1;
        }
    } while ((NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_SM_BUSY) == 0);
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
    {
        while((NAND_GETREG(info, NAND_FIFO_CTRL_REG_OFS) & _FIFO_EN) != 0);
    }
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

    //#NT#2011/01/03#Steven Wang -begin
    //#NT#Read register original value, or original value will be overwrite
    RegSramAcc.Reg = NAND_GETREG(info, NAND_SRAM_ACCESS_REG_OFS);
    //#NT#2011/01/03#Steven Wang -end

    //#NT#2011/01/03#Steven Wang -begin
    //#NT#parameter assign to bit directly
    #if 0
    if (bcpu_Acc == TRUE)
        RegSramAcc.Bit.ERROR_ACC = TRUE;

    else
        RegSramAcc.Bit.ERROR_ACC = FALSE ;
    #else
    RegSramAcc.Bit.ERROR_ACC = bcpu_Acc;
    #endif
    //#NT#2011/01/03#Steven Wang -end

    NAND_SETREG(info, NAND_SRAM_ACCESS_REG_OFS,RegSramAcc.Reg);

    //dummy read to delay 200ns for SRAM ready
    if(bcpu_Acc == TRUE)
    {
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

    //#NT#2011/01/03#Steven Wang -begin
    //#NT#Read register original value, or original value will be overwrite
    RegSramAcc.Reg = NAND_GETREG(info, NAND_SRAM_ACCESS_REG_OFS);
    //#NT#2011/01/03#Steven Wang -end

    //#NT#2011/01/03#Steven Wang -begin
    //#NT#parameter assign to bit directly
    #if 0
    if (bcpu_Acc == TRUE)
        RegSramAcc.Bit.SPARE_ACC = TRUE;

    else
        RegSramAcc.Bit.SPARE_ACC = FALSE ;
    #else
    RegSramAcc.Bit.SPARE_ACC = bcpu_Acc;
    #endif
    //#NT#2011/01/03#Steven Wang -end

    NAND_SETREG(info, NAND_SRAM_ACCESS_REG_OFS,RegSramAcc.Reg);

    //dummy read to delay 200ns for SRAM ready
    if(bcpu_Acc == TRUE)
    {
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

    for(i = 0; i < uiSection; i++){
        shifts = 4 * i;
        rs_err = (rs_sts >> shifts) & 0x7;
        if(rs_err == 0){
            continue;
        }else if(rs_err == 5){
            ret = E_CTX;
            continue;
        }

        for(j = 0; j < rs_err; j++){

            err_reg = NAND_SEC0_EADDR0_REG_OFS + (16 * i) + (4 * j);

            hrdata = NAND_GETREG(info, err_reg);
            err_addr = hrdata & 0xFFFF;

            //if(err_addr >= nand_getPageSize())
            if(err_addr >= _512BYTE)
            {
                nand_host_set_cpu_access_spare(info, TRUE);
                err_addr -= _512BYTE;
                err_addr += (uint32_t)((CONFIG_SYS_NAND_BASE + NAND_SPARE00_REG_OFS) + (i * 16));
            }
            err_bit = (hrdata >> 16) & 0xFF;

            mask_bit = (err_addr & 0x03) * 8;

            //#NT#2010/06/15#Steven Wang -begin
            //#NT#If ECC error in redundant area
            //if((hrdata & 0xFFFF) >= nand_getPageSize())
            if((hrdata & 0xFFFF) >= _512BYTE)
            {
                * (volatile uint32_t *) (err_addr & 0xFFFFFFFC) ^= (err_bit << mask_bit);
                nand_host_set_cpu_access_spare(info, FALSE);
            }
            else
            //#NT#2010/06/15#Steven Wang -end
            {
                Buf[err_addr+ i*_512BYTE] ^= (err_bit);   // correct bit
                #if 0
                *((uint32_t *) &Buf[(err_addr & 0xFFFC)+ i*_512BYTE]) ^= ((err_bit) << mask_bit);
                #endif
            }
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
    uint32_t uiTmpCS;
    union T_NAND_CTRL0_REG    uiNANDCtrl0;
    union T_NAND_MODULE0_REG  uiNANDMduCfg;

    uint32_t mask_bit;

    nand_host_set_cpu_access_err(info, TRUE);

    ham_sts = NAND_GETREG(info, NAND_SECONDARY_ECC_STS_REG_OFS);

    uiNANDCtrl0.Reg = NAND_GETREG(info, NAND_CTRL0_REG_OFS);

    uiTmpCS = uiNANDCtrl0.Bit.CHIP_EN;

    uiNANDMduCfg.Reg = NAND_GETREG(info, uiTmpCS * 4);

    for(i = 0; i < uiSection; i++){   // 8 sections
        shifts = 2 * i;
        ham_err = (ham_sts >> shifts) & 0x3;
        if(ham_err == 0){
            continue;
        }else if(ham_err == 2){
            ret = E_CTX;
            //debug_msg("^R Hamming uncorrect error\r\n");
            continue;
        }else if(ham_err == 3){
            ret = E_CTX;
            //debug_msg("^M Hamming code error\r\n");
            continue;
        }
        err_reg = NAND_SEC0_EADDR0_REG_OFS + (16 * i) + 4;

        hrdata = NAND_GETREG(info, err_reg);

        err_addr = hrdata & _ECC_SEC0_SECONDARY_ERRBYTE;
        err_bit = (hrdata & _ECC_SEC0_SECONDARY_ERRBIT)>>_ECC_SEC0_SECONDARY_BITSFT;

        if(err_addr >= _512BYTE)
        {
            nand_host_set_cpu_access_spare(info, TRUE);
            err_addr -= _512BYTE;
            err_addr += (uint32_t)(CONFIG_SYS_NAND_BASE + NAND_SPARE00_REG_OFS + i * 16);
            mask_bit = (err_addr & 0x03) * 8;

            //debug_msg("^G\033[1m  ===>  byte[%d] + bit[%d] err\r\n", (hrdata & _ECC_SEC0_SECONDARY_ERRBYTE), (err_bit) );

            * (volatile uint32_t *) (err_addr & 0xFFFFFFFC) ^= ((1<<err_bit) << mask_bit);

            nand_host_set_cpu_access_spare(info, FALSE);
        }
        else
        {
            printf("Secondary ECC should occurred > section size[0x%08x]\r\n", _512BYTE);
            ret = E_SYS;
            break;
        }
    }
    nand_host_set_cpu_access_err(info, FALSE);

    return ret;
}