//#include <linux/module.h>
//#include <linux/interrupt.h>
//#include <linux/platform_device.h>
//#include <linux/dma-mapping.h>
//#include <linux/delay.h>
//#include <linux/device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
//#include <linux/io.h>
//#include <linux/irq.h>
//#include <linux/slab.h>
#include "nand_reg.h"
#include "nand_int.h"

/*
    Reset NAND flash

    Send reset command to NAND flash

    @return
        - @b E_SYS      Status fail
        - @b E_TMOUT    Controller timeout
        - @b E_OK       Operation success

    @note for nand_CmdComplete()
*/
int nand_cmd_reset(struct drv_nand_dev_info *info, uint32_t chip_sel)
{
	struct smc_setup_trans transParam;

	transParam.colAddr      = 0;
	transParam.rowAddr      = 0;
	transParam.fifoDir      = _FIFO_READ;
	transParam.transMode    = _PIO_MODE;
	transParam.type         = _CMD;
//	transParam.uiCS         = chip_sel;
	transParam.uiCS         = NAND_CS0_EN;//Howard Modified

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, _RESET_CMD, 0);

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
int nand_cmd_read_status(struct drv_nand_dev_info *info)
{
	int ret;
	uint32_t status;
	struct smc_setup_trans transParam;

	transParam.colAddr      = 0;
	transParam.rowAddr      = 0;
	transParam.fifoDir      = _FIFO_READ;
	transParam.transMode    = _PIO_MODE;
	transParam.type         = _CMD_STS;
	transParam.uiCS         = info->flash_info->chip_sel;

	nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

	nand_host_send_command(info, _READ_ST_CMD, 0);

	ret = nand_cmd_wait_complete(info);

	if(ret == E_OK)
	{
		if(info->buf_count)
		{
			status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & 0xFF;
			/* Our NAND driver will disable WP automatically
			   => always return not protected
			*/
			status |= 0x80;
			memcpy(info->data_buff + info->buf_start, &status, 1);
		}
	}
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
    int erReturn;
    struct smc_setup_trans transParam;

    // Whether the memory card is ready or not?
    if(nand_host_check_ready(info) != E_OK)
    {
        return E_SYS;
    }

    transParam.colAddr      = 0;
    transParam.rowAddr      = block_address;
    transParam.fifoDir      = _FIFO_WRITE;
    transParam.transMode    = _PIO_MODE;
    transParam.type         = _BLK_ERSE;
    transParam.uiCS         = info->flash_info->chip_sel;

    // Once if not write operation => disable write protect EN
    nand_host_setup_transfer(info, &transParam, 0, 0, _SINGLE_PAGE);

    nand_host_send_command(info, _BLK_ERASE_CMD, TRUE);

    erReturn = nand_cmd_wait_complete(info);

    NAND_SETREG(info, NAND_CTRL0_REG_OFS, NAND_GETREG(info, NAND_CTRL0_REG_OFS) | (_NAND_WP_EN));

    if (erReturn != E_OK)
    {
        info->nand_int_status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
        printf("Erase fail sts = 0x%08x\r\n", info->nand_int_status);
        return erReturn;
    }

    if ((NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_PASS_BIT) || (!(NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_READY_BIT)))
    {

        NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_STS_FAIL_STS);

        return E_SYS;
    }
    else
    {
        return E_OK;
    }
}
/*
    Read device ID.

    @return Device ID
*/
int nand_cmd_read_id(uint8_t * card_id, struct drv_nand_dev_info *info)
{
    	struct smc_setup_trans transParam;
    	const struct nvt_nand_flash *f = info->flash_info;

	transParam.colAddr      = 0;
	transParam.rowAddr      = 0;
	transParam.fifoDir      = _FIFO_READ;
	transParam.transMode    = _PIO_MODE;
	transParam.type         = _READ_ID;
	// transParam.uiCS         = NAND_CHIP_SEL(f);
	transParam.uiCS		= NAND_CS0_EN; //Howard modified

	nand_host_setup_transfer(info, &transParam, 0, 8, _SINGLE_PAGE);

	nand_host_send_command(info, _READ_ID_CMD, FALSE);

    	return nand_host_receive_data(info, card_id, 8, _PIO_MODE);
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

    // Use interrupt to wait command.
#if 0//NAND_INT_DISABLE
	wait_for_completion(&info->cmd_complete);
	if(info->nand_int_status & NAND_STS_FAIL_STS)
	{
		dev_err(&info->pdev->dev, "nand status error\r\n");
		return -EINTR;
	}
	if(info->nand_int_status & NAND_TOUT_STS)
	{
		dev_err(&info->pdev->dev, "NAND cmd timeout\r\n");
		return -EIO;
	}
#else
	uint32_t i = 0;
	do
	{
		i++;
		//For slow card
		if (i > 0x48000) //The maximum of waiting time is 20ms (For super slow card)
		{
			printf("_COMP_INTR not set\n\r");
			return -EIO;
		}
	} while ((NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_COMP_STS) == 0);

	NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_COMP_STS);

	if(NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_STS_FAIL_STS)
	{
		NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_STS_FAIL_STS);
		return -EIO;
	}

	if(NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_TOUT_STS)
	{
		NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_TOUT_STS);
		printf("NAND cmd timeout\r\n");
		return -EIO;
	}
#endif
    	return nand_host_check_ready(info);
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
	uint32_t 	*pulBuf = (uint32_t *)buffer;
	int 		erReturn;
	int32_t 	i;
/*
	uint32_t  	bByteAlignment;
	int32_t   	page_cnt = 0;
	uint32_t  	tmpNumPage = 0;
*/
	uint32_t	has_eccerr = FALSE;
	int32_t 	iRetryIndex = -1;
	uint32_t	SpareAreaAddr;
	uint32_t	ii,jj;
	struct smc_setup_trans transParam;
	const struct nvt_nand_flash *f = info->flash_info;
	/*bByteAlignment = (uint32_t)pulBuf;

	bByteAlignment = bByteAlignment & 0x3;
	dev_info(&info->pdev->dev, "nand_cmd_read_operation:numPage = %d\n", numPage);
	*/
	// - Check parameter correctness -
	if (numPage == 0)                                   // Page number check
	{
		printf("nand_cmd_read_operation:numPage is zero\n");
		return E_PAR;
	}
	if (pageAddress % f->page_size)  // Address alignment check
	{
		printf("nand_cmd_read_operation:PageAddress is not dividable by page size(pageAddress:0x%x)\r\n", pageAddress);
		return E_PAR;
	}
	if ( pageAddress / f->block_size != (pageAddress + numPage * f->page_size - 1) / f->block_size)
	{
		printf("nand_cmd_read_operation:Read operation across a block is prohibition\r\n");
		return E_PAR;
	}
    	//Whether the memory card is ready or not?  (For slow card)
    	if(nand_host_check_ready(info) != E_OK)
        	return E_SYS;


	for (i = 0; i < (int32_t)numPage; i++)
	{
		NAND_SETREG(info, NAND_DMASTART_REG_OFS, rcw_get_phy_addr((uint32_t)pulBuf));
		nand_store_cache((void *)(pulBuf), (1 * f->page_size));

		nand_host_check_ready(info);

		transParam.colAddr      = pageAddress % f->page_size;
		transParam.rowAddr      = pageAddress / f->page_size;
		transParam.fifoDir      = _FIFO_READ;
		transParam.transMode    = _DMA_MODE;
		transParam.type         = NAND_HOST_USE_READ(f);
		transParam.uiCS         = f->chip_sel;

		nand_host_setup_transfer(info, &transParam, 1, f->page_size, _MULTI_PAGE);

		nand_host_send_command(info, NAND_CMD_READ(f, FALSE), TRUE);


		erReturn = nand_host_receive_data(info, (uint8_t *)pulBuf, f->page_size, _DMA_MODE);

		if (erReturn != E_OK)
		{
			return erReturn;
		}

		info->nand_int_status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
		if (info->nand_int_status) {
			NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, info->nand_int_status);
		}
		if (info->nand_int_status & NAND_PRI_ECC_STS)
		{
		        nand_store_cache((void *)pulBuf, (uint32_t)(f->page_size));
		        if(nand_host_correct_reedsolomon_ecc(info, (uint8_t *)&pulBuf[0], (f->page_size/0x200)) != E_OK)
		        {
		                if(iRetryIndex != i)
		                {
		                    printf("Retry again\r\n");
		                    iRetryIndex = i;
		                    i--;
		                    continue;
		                }
		                else
		                {
		                    iRetryIndex = -1;
		                }

		                has_eccerr = TRUE;
		        }
		        else
		        {
		                nand_store_cache((void *)pulBuf, (uint32_t)(f->page_size));
		        }
		}
		pulBuf += (f->page_size >> 2);
		pageAddress += f->page_size;
		SpareAreaAddr = (uint32_t)(info->mmio_base+ NAND_SPARE00_REG_OFS);
		ii = i * (f->page_size / 0x200) * 16;
		nand_host_set_cpu_access_spare(info, TRUE);
		for(jj = 0; jj < (f->page_size / 0x200); jj++)
		{
			*(volatile uint32_t *)(pulBuf + ii + 0) = *(volatile uint32_t *)(SpareAreaAddr + jj *16);
			*(volatile uint32_t *)(pulBuf + ii + 1) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x4);
			*(volatile uint32_t *)(pulBuf + ii + 2) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x8);
			*(volatile uint32_t *)(pulBuf + ii + 3) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0xC);

#if 0
				pr_info("ECC R [%d] [0x%08x] [0x%08x] [0x%08x] [0x%08x]\r\n", jj, *(volatile uint32_t *)(pulBuf + ii + 0),
                                                                                     *(volatile uint32_t *)(pulBuf + ii + 1),
                                                                                     *(volatile uint32_t *)(pulBuf + ii + 2),
                                                                                     *(volatile uint32_t *)(pulBuf + ii + 3));
#endif
			ii+=16>>2;
			pulBuf += (16>>2);

		}
		nand_host_set_cpu_access_spare(info, FALSE);

	}
	return E_OK;
}


/*
    Write pages.(single page operation)

    @param buffer      Buffer address
    @param pageAddress The address of the page. Only (n * g_pNandInfo->uiBytesPerPageData) will be valid.
            Beware that the max accessible size is 4GB.  One should use write_readOperation2() instead.
    @param numPage     How many pages
    @return E_OK or E_SYS
*/
int nand_cmd_write_operation_single(struct drv_nand_dev_info *info, int8_t * buffer, uint32_t pageAddress, uint32_t numPage)
{
	struct smc_setup_trans transParam;
	const struct nvt_nand_flash *f = info->flash_info;

	uint32_t *pulBuf = (uint32_t *)buffer;
	uint32_t i;
	uint32_t erReturn;

	uint32_t	SpareAreaAddr;
	uint32_t	ii,jj;
/*
    	dev_info(&info->pdev->dev, "nand_cmd_write_operation_single:numPage = %d\n", numPage);
*/
	// - Check parameter correctness -
	if (numPage == 0)                                   // Page number check
	{
		printf("nand_cmd_write_operation_single:numPage is zero\n");
		return E_PAR;
	}
	if (pageAddress % f->page_size)  // Address alignment check
	{
		printf("nand_cmd_write_operation_single:PageAddress is not dividable by page size(pageAddress:0x%x)\r\n", pageAddress);
		return E_PAR;
	}
	if ( pageAddress / f->block_size != (pageAddress + numPage * f->page_size - 1) / f->block_size)
	{
		printf("nand_cmd_write_operation_single:Read operation across a block is prohibition\r\n");
		return E_PAR;
	}
/*
    	//Whether the memory card is ready or not?  (For slow card)
    	if(nand_host_check_ready(info) != E_OK)
        	return E_SYS;
*/

	for (i = 0; i < numPage; i++)
    	{
		//-----------------------------------------------------
		// Start DMA
		//-----------------------------------------------------
		NAND_SETREG(info, NAND_DMASTART_REG_OFS, rcw_get_phy_addr((uint32_t)pulBuf));
		nand_store_cache((void *)(pulBuf), (1 * f->page_size));
		nand_host_check_ready(info);

	        transParam.colAddr      = pageAddress % f->page_size;
	        transParam.rowAddr      = pageAddress / f->page_size;
	        transParam.fifoDir      = _FIFO_WRITE;
        	transParam.transMode    = _DMA_MODE;
        	transParam.type         = _DATA_WR1;
        	transParam.uiCS         = f->chip_sel;
		SpareAreaAddr = (uint32_t)(info->mmio_base + NAND_SPARE00_REG_OFS);
		ii = i * (f->page_size / 0x200) * 16;
		nand_host_set_cpu_access_spare(info, TRUE);
		for(jj = 0; jj < (f->page_size / 0x200); jj++)
		{
			*(volatile uint32_t *)(SpareAreaAddr + jj *16) = *(volatile uint32_t *)(pulBuf + (f->page_size >> 2) + ii + 0);
			*(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x4) = *(volatile uint32_t *)(pulBuf + (f->page_size >> 2) + ii + 1);
			*(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x8) = *(volatile uint32_t *)(pulBuf + (f->page_size >> 2) + ii + 2);
			*(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0xC) = *(volatile uint32_t *)(pulBuf + (f->page_size >> 2) + ii + 3);

#if 0
			pr_info("ECC W [%d] [0x%08x] [0x%08x] [0x%08x] [0x%08x]\r\n", jj, *(volatile uint32_t *)(SpareAreaAddr + jj *16),
			                                                             *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x4),
			                                                             *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x8),
			                                                             *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0xC));


#endif

			ii+=16>>2;
		}
		nand_host_set_cpu_access_spare(info, FALSE);

	        nand_host_setup_transfer(info, &transParam, 1, f->page_size, _MULTI_PAGE);
	        nand_host_send_command(info, _WRITE_CMD, FALSE);

        	erReturn =  nand_host_transmit_data(info, (uint8_t *)pulBuf, f->page_size, transParam.transMode);


	        if (erReturn != E_OK)
	        {
	            return erReturn;
	        }

	        pulBuf += (f->page_size >> 2);
	        pageAddress += f->page_size;


		if ((NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_PASS_BIT) || (!(NAND_GETREG(info, NAND_CTRL_STS_REG_OFS) & NAND_READY_BIT))) //Check pass-bit when every page finished.
		{
			printf("write operation failure status 0x%08x\r\n", (uint32_t)NAND_GETREG(info, NAND_CTRL_STS_REG_OFS));

			NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, NAND_STS_FAIL_STS);

			//#enable nand write protect after write operation
			NAND_SETREG(info, NAND_CTRL0_REG_OFS, NAND_GETREG(info, NAND_CTRL0_REG_OFS) | (_NAND_WP_EN));
			return E_SYS;
		}
    	}
	NAND_SETREG(info, NAND_CTRL0_REG_OFS, NAND_GETREG(info, NAND_CTRL0_REG_OFS) | (_NAND_WP_EN));
	return E_OK;
}

void nand_store_cache(void *pbuf, u32 len)
{
	return;

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
	uint32_t		row_address;
	struct smc_setup_trans 	trans_param;
	uint32_t		SpareAreaAddr;
	uint32_t		ii,jj;
	uint32_t 		*pulBuf = (uint32_t *)buffer;

	const struct nvt_nand_flash *f = info->flash_info;
    	int erReturn;

    	info->retcode = E_OK;

	if (page_address % f->page_size)  // Address alignment check
	{
		printf("PageAddress is not dividable by page size(0x%x)\r\n", page_address);
		return E_PAR;
	}

    	row_address = page_address / f->page_size;

	trans_param.colAddr      = (f->page_size);
	trans_param.rowAddr      = row_address;
	trans_param.fifoDir      = _FIFO_READ;
	trans_param.transMode    = _PIO_MODE;
	trans_param.type         = NAND_HOST_USE_READ(f);
	trans_param.uiCS         = f->chip_sel;


    	nand_host_setup_transfer(info, &trans_param, 0, GET_NAND_SPARE_SIZE(f), _SINGLE_PAGE);

    	nand_host_send_command(info, NAND_CMD_READ(f, TRUE), FALSE);

    	erReturn = nand_cmd_wait_complete(info);

	if (erReturn != E_OK)
	{
		info->retcode = erReturn;
		return erReturn;
	}
	info->nand_int_status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
	if (info->nand_int_status) {
		NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, info->nand_int_status);
	}
	if (info->nand_int_status & NAND_SEC_ECC_STS)
	{
		if(nand_host_correct_secondary_ecc(info, (f->page_size/0x200)) != E_OK)
		{
		    NAND_SETREG(info, NAND_DMASTART_REG_OFS, (uint32_t)info->data_buff);
		    trans_param.colAddr      = page_address % f->page_size;
		    trans_param.rowAddr      = page_address / f->page_size;
		    trans_param.fifoDir      = _FIFO_READ;
		    trans_param.transMode    = _DMA_MODE;
		    trans_param.type         = NAND_HOST_USE_READ(f);
		    trans_param.uiCS         = f->chip_sel;

		    nand_host_setup_transfer(info, &trans_param, 1, f->page_size, _MULTI_PAGE);

		    nand_host_send_command(info, NAND_CMD_READ(f, FALSE), FALSE);

		    erReturn = nand_cmd_wait_complete(info);

		    if (erReturn != E_OK)
		    {
		    	info->retcode = erReturn;
		        return erReturn;
		    }
		    info->nand_int_status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);
		    if (info->nand_int_status) {
			NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, info->nand_int_status);
		    }
		    if (info->nand_int_status & NAND_PRI_ECC_STS)
		    {
		        nand_host_set_fifo_enable(info, FALSE);
		        //_dma_cache_inv((uint32_t)buffer, (f->page_size));
		        if(nand_host_correct_reedsolomon_ecc(info, (uint8_t *)&info->data_buff[0], (f->page_size/0x200)) != E_OK)
		        {
		            	SpareAreaAddr = (uint32_t)info->mmio_base+ NAND_SPARE00_REG_OFS;
				nand_host_set_cpu_access_spare(info, TRUE);
				ii = 0;
				for(jj = 0; jj < (f->page_size / 0x200); jj++)
				{
					*(volatile uint32_t *)(pulBuf + ii + 0) = *(volatile uint32_t *)(SpareAreaAddr + jj *16);
/*
					*(volatile uint32_t *)(pulBuf + ii + 1) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x4);
					*(volatile uint32_t *)(pulBuf + ii + 2) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x8);
					*(volatile uint32_t *)(pulBuf + ii + 3) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0xC);
*/

#if 0
					pr_info("ECC se [%d] [0x%08x] [0x%08x] [0x%08x] [0x%08x]\r\n", jj, *(volatile uint32_t *)(pulBuf + ii + 0),
				                                                             *(volatile uint32_t *)(pulBuf + ii + 1),
				                                                             *(volatile uint32_t *)(pulBuf + ii + 2),
				                                                             *(volatile uint32_t *)(pulBuf + ii + 3));
#endif
					ii+=16>>2;
					pulBuf += (16>>2);

				}
				nand_host_set_cpu_access_spare(info, FALSE);
		            	printf("read spare err block[%d]/page[%d]\r\n", page_address / (f->page_size * f->page_per_block), ((page_address / f->page_size) % f->page_per_block));
		            	return E_CTX;
		        }
		        else
		        {
//		            _dma_cache_inv((uint32_t)buffer, (f->page_size));
		        }
		    }
		}
	}
	SpareAreaAddr = (uint32_t)info->mmio_base + NAND_SPARE00_REG_OFS;
	nand_host_set_cpu_access_spare(info, TRUE);
	ii = 0;
	for(jj = 0; jj < (f->page_size / 0x200); jj++)
	{
		*(volatile uint32_t *)(pulBuf + ii + 0) = *(volatile uint32_t *)(SpareAreaAddr + jj *16);
/*
		*(volatile uint32_t *)(pulBuf + ii + 1) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x4);
		*(volatile uint32_t *)(pulBuf + ii + 2) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0x8);
		*(volatile uint32_t *)(pulBuf + ii + 3) = *(volatile uint32_t *)(SpareAreaAddr + jj *16 + 0xC);
*/

#if 0
		pr_info("[%d] [0x%08x] [0x%08x] [0x%08x] [0x%08x]\r\n", jj, *(volatile uint32_t *)(pulBuf + ii + 0),
                                                                     *(volatile uint32_t *)(pulBuf + ii + 1),
                                                                     *(volatile uint32_t *)(pulBuf + ii + 2),
                                                                     *(volatile uint32_t *)(pulBuf + ii + 3));
#endif
		ii+=16>>2;
		pulBuf += (16>>2);

	}
	nand_host_set_cpu_access_spare(info, FALSE);

	nand_cmd_dummy_read(info);

	return E_OK;
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
	const struct nvt_nand_flash *f = info->flash_info;
	struct smc_setup_trans 	trans_param;

	trans_param.colAddr      = 0;
	trans_param.rowAddr      = 0;
	trans_param.fifoDir      = _FIFO_READ;
	trans_param.transMode    = _PIO_MODE;
	trans_param.type         = NAND_HOST_USE_READ(f);
	trans_param.uiCS         = f->chip_sel;

	nand_host_setup_transfer(info, &trans_param, 0, 0, _SINGLE_PAGE);
	nand_host_send_command(info, NAND_CMD_READ(f, FALSE), TRUE);

    	return nand_cmd_wait_complete(info);
}