/*
 *  driver/mmc/nt72668_mmc.c
 *
 *  Author:	Alvin lin
 *  Created:	April 24, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#include <asm/io.h>
#include "common.h"
#include <errno.h>
#include <mmc.h>
#include <malloc.h>
#include <asm/arch/nt72668_stbc.h>
#include "nt72668_mmc.h"

//#define __MMC_DEBUG
//668 emmc HAL APIs
//=============================================================================================================
static int _emmc_host_select_clk_src(int src)
{
	if (src == 0) {
		nt72668_stbc_unlock_reg();
		EMMC_CLK_SRC_CTRL |= EMMC_FAST_CLK_SRC_ENABLE;
	} else if (src == 1) {
		nt72668_stbc_unlock_reg();
		EMMC_CLK_SRC_CTRL &= ~EMMC_FAST_CLK_SRC_ENABLE;
	} else
		return 0;

	/* 1 micro second at least */
	udelay(1);

	return 1;
}

static int nt72668_emmc_arch_host_preinit(void)
{
	nt72668_stbc_unlock_reg();
	*(volatile unsigned int *)(0xfc040200) &= ~(1<<4);
	udelay(1);

	nt72668_stbc_unlock_reg();
	udelay(1);
	*(volatile unsigned int *)(0xfc040200) |= (1<<5);
	udelay(1);

	nt72668_stbc_unlock_reg();
	*(volatile unsigned int *)(0xfc04021c) |= (1<<25);
	udelay(1);

	nt72668_stbc_unlock_reg();
	*(volatile unsigned int *)(0xfc04021c) &= ~(1<<25);
	udelay(1);

	_emmc_host_select_clk_src(0);
	EMMC_SECURE_LOCK_CTRL &= ~EMMC_SECURE_LOCK_ENABLE;

	return 0;
}

static unsigned long sdc_clk2div(unsigned long clk)
{
	unsigned long q = 0;
	unsigned long r = 0;
	unsigned long clk_src = 0;

extern unsigned long get_emmc_clk(void);
	/* if selected high clk src */ 
	if (EMMC_CLK_SRC_CTRL & EMMC_FAST_CLK_SRC_ENABLE)
		clk_src = get_emmc_clk();
	else
		clk_src = SDCLK_SOURCE_LOW;

	/* the max clk using this approach is clk_src divied by 8 */
	if (clk_src/8 < clk)
		return 0;

	q = (clk_src/(clk*4));
	r = (clk_src%(clk*4));
	q -= 2;

	if (r)
		q+=1;

	return q;
}

static unsigned long _emmc_init_clk(unsigned long rate)
{
	unsigned long divisor = 0;

#ifdef __MMC_DEBUG
	printf("emmc clk rate : %d\n", rate);
#endif
	if ((rate == 0) || (rate > SDCLK_MAX)) {
		printf("rate(%ld) is out of range", rate);
		return 0;
	}

	/* turn off clk */
	REG_SDC_CLK_CTRL = 0;


	if (divisor > SDC_MAX_CLK_DIV) {
		printf("can not get proper divisor");
		return 0;
	}
	/* if clk is higher than 1/4 of eMMC clk source, use 1/4 clk source, which is the max rate */
	if (rate >= CLK_100M) {
#ifdef __MMC_DEBUG
		printf("hs200 clk new @rate %ld\n", rate);
#endif
		REG_FCR_FUNC_CTRL &= ~FCR_FUNC_CTRL_SD_FLEXIBLE_CLK;
		REG_FCR_HS200_CTRL |= FCR_HS200_CTRL_FASTEST_CLK;
		REG_FCR_HS200_CTRL |= FCR_HS200_CTRL_HW_TRACK_EACH_BLK;
	} else {
		REG_FCR_HS200_CTRL &= ~FCR_HS200_CTRL_FASTEST_CLK;

		divisor = sdc_clk2div(rate);

		/* use NVT's clk scheme */
		REG_FCR_FUNC_CTRL |= FCR_FUNC_CTRL_SD_FLEXIBLE_CLK;
	} 
	/* turn on bus internal clk */
	REG_SDC_CLK_CTRL = SDC_CLK_CTRL_INCLK_ENABLE | SDC_CLK_CTRL_SDCLK_FREQ_SEL_EX(divisor);

	while (!(REG_SDC_CLK_CTRL & SDC_CLK_CTRL_INCLK_STABLE))
		printf("wait for emmc internal clk stable...\n");

	/* turn on bus clk */
	REG_SDC_CLK_CTRL |= SDC_CLK_CTRL_SDCLK_ENABLE;

	/* 1ms at least to wait external bus clk stable */
	mdelay(1);

	return divisor;
}

static int _emmc_bus_init(unsigned long voltage)
{
	if ((voltage != SDC_PW_CTRL_BUS_VOL_33V) &&
		(voltage != SDC_PW_CTRL_BUS_VOL_30V) &&
		(voltage != SDC_PW_CTRL_BUS_VOL_18V)) {
		printf("wrong bus voltage(0x%lx)", voltage);
		return 0;
	}

	/* we support bus power 1.8V and 3.3V */
	REG_SDC_PW_CTRL = SDC_PW_CTRL_BUS_PW_ON | voltage;

	/* 10ms for safety */
	mdelay(10);

	_emmc_init_clk(EMMC_INIT_BUS_CLK);

	return 1;
}

static int _emmc_set_bus_timing_mode(int clk,
                                   enum EMMC_DATA_LATCH data_latch)
{
	enum EMMC_SPEED_MODE speed_mode  = EMMC_NULL_SPEED_MODE;

	if(clk <= CLK_26M) 
		speed_mode = EMMC_LEGACY_SPEED;
	else if( clk <= CLK_52M)
		speed_mode = EMMC_HIGH_SPEED;
	else if( clk <= CLK_200M)
		speed_mode = EMMC_HS200_SPEED;
	else 
		printf("[MMC]error speed mode!!\n");

	if(speed_mode == EMMC_NULL_SPEED_MODE)
		return -1;
#ifdef __MMC_DEBUG	
	printf("[MMC] speed_mode = %d\n", speed_mode);
	printf("[MMC] latch = %d\n", data_latch);
#endif
        if ((speed_mode == EMMC_HIGH_SPEED) || (speed_mode == EMMC_HS200_SPEED)) {
		REG_SDC_HOST_CTRL |= SDC_HOST_CTRL_HIGH_SPEED;
		//mdelay(1);
                if (speed_mode == EMMC_HS200_SPEED) {
			REG_FCR_HS200_CTRL |= FCR_HS200_CTRL_DISABLE_CMD_CONFLICT;
			REG_FCR_HS200_CTRL &= ~FCR_HS200_OUTPUT_SELECT_MASK;
			REG_FCR_HS200_CTRL |= FCR_HS200_CTRL_ENABLE;
			REG_FCR_HS200_CTRL |= FCR_HS200_OUTPUT_SELECT_PHASE(0x1);
                } else {
			REG_FCR_HS200_CTRL &= ~FCR_HS200_CTRL_ENABLE;
			REG_FCR_HS200_CTRL &= ~FCR_HS200_CTRL_DISABLE_CMD_CONFLICT;
                }
        } else if (speed_mode == EMMC_LEGACY_SPEED) {
		REG_FCR_HS200_CTRL &= ~FCR_HS200_CTRL_DISABLE_CMD_CONFLICT;
		REG_FCR_HS200_CTRL &= ~ FCR_HS200_CTRL_ENABLE;
		REG_SDC_HOST_CTRL &=  ~SDC_HOST_CTRL_HIGH_SPEED;
	} else {
		printf("[MMC] not support host speed mode(%d)", speed_mode);
                return -1;
        }

	//mdelay(1);
        if (data_latch == EMMC_SINGLE_LATCH) {
                REG_FCR_CPBLT &= ~FCR_CPBLT_DUAL_DATA_RATE_ENABLE;
        } else if (data_latch == EMMC_DUAL_LATCH) {
                REG_FCR_CPBLT |= FCR_CPBLT_DUAL_DATA_RATE_ENABLE;
        } else {
                printf("cfg->data_latch(%d) is invalid.", data_latch); 
                return 0;
        }

	//udelay(1);

        return 0;
}
          
static int _emmc_host_switch_bus_width(enum EMMC_BUS_WIDTH bus_width)
{
	switch (bus_width) {
		case EMMC_BW_1BIT:
			REG_SDC_HOST_CTRL &= ~SDC_HOST_CTRL_4BIT;
			REG_FCR_FUNC_CTRL &= ~FCR_FUNC_CTRL_MMC_8BIT;
			REG_SDC_HOST_CTRL &= ~SDC_HOST_CTRL_8BIT;
			break;

		case EMMC_BW_4BIT:
			REG_SDC_HOST_CTRL |= SDC_HOST_CTRL_4BIT;
			REG_FCR_FUNC_CTRL &= ~FCR_FUNC_CTRL_MMC_8BIT;
			REG_SDC_HOST_CTRL &= ~SDC_HOST_CTRL_8BIT;
			break;

		case EMMC_BW_8BIT:
			REG_SDC_HOST_CTRL |= SDC_HOST_CTRL_4BIT;
			REG_FCR_FUNC_CTRL |= FCR_FUNC_CTRL_MMC_8BIT;
			REG_SDC_HOST_CTRL |= SDC_HOST_CTRL_8BIT;
			break;

		default:
			printf("invalid bus width(%d)", bus_width);	
			return 0;
	}
//	mdelay(1);
	return 1;
}

static void _emmc_emit_cmd_with_arg(unsigned short cmd_idx, unsigned long cmd_arg, unsigned long cmd_attr)
{
	REG_SDC_INT_STAT = -1;
	REG_SDC_ERR_INT_STAT = -1;
	REG_SDC_ARG = cmd_arg;
	REG_SDC_CMD = SDC_CMD_IDX(cmd_idx) | cmd_attr;
}

static inline void _emmc_wait_for_cmd_completed_or_err(void)
{
	while (!(REG_SDC_INT_STAT & (SDC_INT_CMD_COMPLETE | SDC_INT_ERR_INT)));
		//printf("wait for cmd completed...");
}

static int _emmc_err(void)
{
	if (REG_SDC_INT_STAT & SDC_INT_ERR_INT){
		printf("mmc err int status: %x \n", REG_SDC_INT_STAT);
	}
	
	return (REG_SDC_INT_STAT & SDC_INT_ERR_INT);
}

static int _emmc_timeout(void)
{
	return !(REG_SDC_INT_STAT & SDC_INT_CMD_COMPLETE);
}


static int _emmc_trans_complete(void)
{
	return (REG_SDC_INT_STAT & SDC_INT_TRAN_COMPLETE);
}

static int _emmc_dma_int(void)
{
	return (REG_SDC_INT_STAT & SDC_INT_DMA_INT);
}
//=============================================================================================================
/* send command to the mmc card and wait for results */
static int do_command(struct mmc *dev, struct mmc_cmd *cmd, u8 has_data)
{
	unsigned long cmd_attr = 0;

	//printf("cmd->resp_type = 0x%x\n", cmd->resp_type);

	if (!(cmd->resp_type & MMC_RSP_PRESENT)) {
		cmd_attr = 0;
	} else {
		

		if (cmd->resp_type & MMC_RSP_136) {
			cmd_attr |= SDC_CMD_RESP_TYPE_LEN_136;
		} else {
			cmd_attr |= SDC_CMD_RESP_TYPE_LEN_48;
		}

		if (cmd->resp_type & MMC_RSP_CRC) {
			cmd_attr |= SDC_CMD_CRC_CHK;
		}

		if (cmd->resp_type & MMC_RSP_BUSY) {
			cmd_attr |= SDC_CMD_RESP_TYPE_LEN_48_BUSY_CHK;
		}

		if (cmd->resp_type & MMC_RSP_OPCODE) {
			cmd_attr |= SDC_CMD_IDX_CHK;
		}
	}

	if (has_data) {
		cmd_attr |= SDC_CMD_DATA_PRESENT;
	}

	//kwinyee debug
	//printf("%s called ! cmd = %d, arg = %x, attr = 0x%x", __func__, cmd->cmdidx, cmd->cmdarg, cmd_attr);
	//kwinyee debug
	//printf("===1===\n");
	//getc();
	_emmc_emit_cmd_with_arg(cmd->cmdidx, cmd->cmdarg, cmd_attr);
	//kwinyee debug
	//printf("===2===\n");
	_emmc_wait_for_cmd_completed_or_err();

	//kwinyee debug
	//printf("===3===\n");
	if(_emmc_err()) {
		
		if (_emmc_timeout()) {
			printf("emmc timeout \n");
			return TIMEOUT;
		}
		printf("emmc error \n");
		return COMM_ERR;
	}

	if(cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136) {
			cmd->response[0] = REG_SDC_RESP0;
			cmd->response[1] = REG_SDC_RESP1;
			cmd->response[2] = REG_SDC_RESP2;
			cmd->response[3] = REG_SDC_RESP3;
			//kwinyee add
			unsigned int csd[4] = {};
			unsigned int raw_csd[4] = {};
			unsigned char *raw_ptr;
			unsigned char *csd_ptr;
			int idx;

			memcpy(raw_csd, cmd->response, sizeof(raw_csd));
			raw_ptr = (unsigned char*)raw_csd;
			csd_ptr = (unsigned char*)csd;
			//shift 8bit for R2
			csd_ptr++;
			for(idx=0;idx < 15;idx++)
				csd_ptr[idx]= raw_ptr[idx];

			cmd->response[0] = csd[3];
			cmd->response[1] = csd[2];
			cmd->response[2] = csd[1];
			cmd->response[3] = csd[0];
		} else
			cmd->response[0] = REG_SDC_RESP0;
	}

	//printf("cmd resp0 = 0x%x\n", cmd->response[0]);

	/* After CMD2 set RCA to a none zero value. */
	if (cmd->cmdidx == MMC_CMD_ALL_SEND_CID)
		dev->rca = 10;

	/* After CMD3 open drain is switched off and push pull is used. */
	//kwinyee note:how to do this on 668 ?
#if 0	
	if (cmd->cmdidx == MMC_CMD_SET_RELATIVE_ADDR) {
		u32 sdi_pwr = readl(&host->base->power) & ~SDI_PWR_OPD;
		writel(sdi_pwr, &host->base->power);
	}
#endif	
	//printf("HAL cmd done\n");
	return 0;
}

static int do_data_transfer(struct mmc *dev,
			    struct mmc_cmd *cmd,
			    struct mmc_data *data)
{
	u32 data_len = (u32) (data->blocks * data->blocksize);
	int error = 0;
	ulong start, timeout;

	timeout = 5 * CONFIG_SYS_HZ;

	if(((u32)(data->dest) % EMMC_DMA_ALIGNMENT)) {
		printf("emmc data buffer must %dbyte align !\n", EMMC_DMA_ALIGNMENT);
		error = -EINVAL;
		goto out;
	}

	while (REG_SDC_STAT & (SDC_STAT_CMD_INHIBIT_CMD | SDC_STAT_CMD_INHIBIT_DAT)) {
		printf("Wait for emmc bus idle !\n");
		mdelay(1);
	}

	/* SDMA */
	REG_SDC_HOST_CTRL &= (~(3<<3));

	REG_SDC_BLK_SIZE = SDC_BLK_SIZE_TRAN_BLK_SIZE(data->blocksize) | SDC_BLK_SIZE_DMA_BUFF_BND_512K;
	REG_SDC_BLK_COUNT = data->blocks;
	REG_SDC_DMA_ADDR = (u32)(data->dest);

	if (data->flags & MMC_DATA_READ) {

		if (data->blocks == 1) {
			REG_SDC_TRAN_MODE = SDC_TRAN_MODE_BLK_COUNT   |
					SDC_TRAN_MODE_READ        |
					SDC_TRAN_MODE_DMA;
		} else {
			REG_SDC_TRAN_MODE = SDC_TRAN_MODE_MULT_BLK    |
					SDC_TRAN_MODE_BLK_COUNT   |
					//SDC_TRAN_MODE_AUTO_CMD12  |
					SDC_TRAN_MODE_READ        |
					SDC_TRAN_MODE_DMA;
		}
		//No need to flush cache when read
		//flush_dcache_range((u32)(data->dest), data_len);

		error = do_command(dev, cmd, 1);
		if (error) {
			printf("emmc read op error !\n");
			goto out;
		}

		if(cmd->response[0] & EMMC_DEV_ANY_STATUS_ERR) {
			printf("emmc card status bad !\n");
			error = -EIO;
			goto out;
		}

		start = get_timer(0);
		while (!_emmc_trans_complete()) {
			if (_emmc_dma_int()) {
				REG_SDC_INT_STAT = SDC_INT_DMA_INT;
				REG_SDC_DMA_ADDR = REG_SDC_DMA_ADDR;
				continue;
			}

			if(_emmc_err()) {
				printf("emmc read dma transfer fail !\n");
				error = -EIO;
				goto out;
			}

			if(get_timer(start) > timeout) {
				printf("emmd read dma timeout !\n");
				error = -ETIMEDOUT;
				goto out;
			}
		}

		invalidate_dcache_range((u32)(data->dest), (u32)(data->dest)+ data_len);

	} else if (data->flags & MMC_DATA_WRITE) {

		flush_dcache_range((u32)(data->dest), (u32)(data->dest) + data_len);

		if(data->blocks > 1)
			REG_SDC_TRAN_MODE = SDC_TRAN_MODE_MULT_BLK | SDC_TRAN_MODE_BLK_COUNT | SDC_TRAN_MODE_DMA;
		else
			REG_SDC_TRAN_MODE = SDC_TRAN_MODE_DMA;

		error = do_command(dev, cmd, 1);
		if (error) {
			printf("emmc write op error !\n");
			goto out;
		}

		if(cmd->response[0] & EMMC_DEV_ANY_STATUS_ERR) {
			printf("emmc card status bad !\n");
			error = -EIO;
			goto out;
		}

		start = get_timer(0);

		while (!_emmc_trans_complete()) {
#if defined(CONFIG_TCL668)			
			nt72668_stbc_tcl_led_ctl(TCL_EU_LED_BLINK);
#endif			
			if (_emmc_dma_int()) {
				REG_SDC_INT_STAT = SDC_INT_DMA_INT;
				REG_SDC_DMA_ADDR = REG_SDC_DMA_ADDR;
				continue;
			}

			if(_emmc_err()) {
				printf("emmc write dma transfer fail !\n");
				error = -EIO;
				goto out;
			}

			if(get_timer(start) > timeout) {
				printf("emmc write dma timeout !\n");
				error = -ETIMEDOUT;
				goto out;
			}
		}

	}

out:
	return error;
}

static int host_request(struct mmc *dev,
			struct mmc_cmd *cmd,
			struct mmc_data *data)
{
	int result;

	//kwinyee debug
	//printf("%s called !\n",__func__);
	if (data)
		result = do_data_transfer(dev, cmd, data);
	else
		result = do_command(dev, cmd, 0);

	return result;
}

/* MMC uses open drain drivers in the enumeration phase */
static int mmc_host_reset(struct mmc *dev)
{

	//kwinyee debug
	//printf("%s called !\n",__func__);

#if 0	
	REG_SDC_SW_RESET = SDC_SW_RESET_ALL | SDC_SW_RESET_CMD_LINE | SDC_SW_RESET_DAT_LINE;

	while (REG_SDC_SW_RESET & (SDC_SW_RESET_ALL | SDC_SW_RESET_CMD_LINE | SDC_SW_RESET_DAT_LINE)) {
		printf("wait for sdc sw reset stable...");
	}

	udelay(1);
#endif
	return 0;
}

static void host_set_ios(struct mmc *dev)
{

	//kwinyee debug
	//printf("%s called !,clk = %d, bwidth = %d\n",__func__,dev->clock, dev->bus_width);

	/* set clock rate */
	if (dev->clock){ 
	static int cur_clk = 0;
		if(dev->clock > CLK_52M  ){
                        /* our controller has a bug, need to reset CMD&DATA after enabling HS200 speed mode */
                        /* 1. switch to higher clk */
                        _emmc_init_clk(13000000);
			cur_clk = 13000000;
                        /* 2. reset CMD&DATA */
                        REG_SDC_SW_RESET |= SDC_SW_RESET_CMD_LINE | SDC_SW_RESET_DAT_LINE;
                        /* OK, wait for reset done */
                        while (REG_SDC_SW_RESET & SDC_SW_RESET_CMD_LINE);
                        while (REG_SDC_SW_RESET & SDC_SW_RESET_DAT_LINE);
		}
		if(dev->clock != cur_clk)
		_emmc_init_clk(dev->clock);
		cur_clk = dev->clock;
	}

	/* Set the bus width */
	if (dev->bus_width) {

		switch (dev->bus_width) {
		case 1:
			_emmc_host_switch_bus_width(EMMC_BW_1BIT);
			break;
		case 4:
			_emmc_host_switch_bus_width(EMMC_BW_4BIT);
			break;
		case 8:
			_emmc_host_switch_bus_width(EMMC_BW_8BIT);
			break;
		default:
			printf("Invalid bus width: %d\n", dev->bus_width);
			break;
		}
	}

	/* check relation between ddr_mode and latch , remove it when data latch is not removed.
	*/	
	if(  (dev->ddr_mode == 1 && dev->data_latch != 2 ) || 
		(dev->ddr_mode == 0 && dev->data_latch != 1 )){
		printf("mmc error data struct ddr_mode & data_latch not matched... latch : %d ddr: %d \n", 
		dev->data_latch, dev->ddr_mode);
		while(1);
	}

	switch(dev->ddr_mode){
		case 0:
			_emmc_set_bus_timing_mode( dev->tran_speed, EMMC_SINGLE_LATCH);
			break;
		case 1:
			_emmc_set_bus_timing_mode( dev->tran_speed, EMMC_DUAL_LATCH);
			break;
		default : 
			//printf("Invalid data latch: %d\n", dev->data_latch);
			break;		
	
	}

}

static int mmc_host_getcd(struct mmc *mmc)
{
	//kwinyee debug
	//printf("%s called !\n",__func__);


	//emmc card always exist
	return 1;
}


static const struct mmc_ops nvt_hsmmc_ops = {
	.send_cmd = host_request,
	.set_ios = host_set_ios,
	.init = mmc_host_reset,
	.getcd = mmc_host_getcd,
};
static char dev_name[] = "NVT_EMMC";
/*
 * mmc_host_init - initialize the mmc controller.
 * Set initial clock and power for mmc slot.
 * Initialize mmc struct and register with mmc framework.
 */
int nt72668_mmc_init(void)
{
	struct mmc *dev;

	struct mmc_config *cfg = malloc(sizeof(struct mmc_config));		///new cfg
	//dev = malloc(sizeof(struct mmc));
	//if (!dev)
	//	return -ENOMEM;

	//memset(dev, 0, sizeof(struct mmc));
	
	
	//init host controler
	nt72668_emmc_arch_host_preinit();

	/* this register might be modified by STBC, need to set its default value again, or cmd might error */
	REG_FCR_CPBLT = 0xf8934ff;

	/* set DMA beat mode */
	REG_FCR_FUNC_CTRL = (EMMC_DMA_BEAT_16_8_4<<20) | 0xf3020;

	/* enable SW card detect function */
	REG_FCR_FUNC_CTRL |= FCR_FUNC_CTRL_SW_CDWP_ENABLE;

	/* force card always exists */
	REG_FCR_FUNC_CTRL &= ~FCR_FUNC_CTRL_SW_SD_CD;

	/* data bus is little endian */
	REG_FCR_FUNC_CTRL |= FCR_FUNC_CTRL_LITTLE_ENDIAN;

	/* do not sd clk bypass */
	REG_FCR_CPBLT &= ~FCR_CPBLT_SD_CLK_BYPASS;

	/* since NFC has nand and eMMC, select eMMC */
	REG_NFC_SYS_CTRL |= NFC_EMMC_SEL;

	/* SW reset */
	REG_SDC_SW_RESET = SDC_SW_RESET_ALL | SDC_SW_RESET_CMD_LINE | SDC_SW_RESET_DAT_LINE;

	while (REG_SDC_SW_RESET & (SDC_SW_RESET_ALL | SDC_SW_RESET_CMD_LINE | SDC_SW_RESET_DAT_LINE)) {
		printf("wait for sdc sw reset stable...");
	}

	udelay(1);

	/* how much time to timeout when waiting device's DATA[0:7] */
	REG_SDC_TIMEOUT_CTRL = SDC_TIMEOUT_CTRL_DATA_TIMEOUT(0xf);

	REG_SDC_INT_ENABLE = 0;         // mask all interrupt
	REG_SDC_INT_STAT_ENABLE = 0;    // disable all interrupt status
	REG_SDC_INT_STAT = -1;          // clear all interrupt status
	REG_SDC_INT_STAT_ENABLE = -1;   // enable all interrupt status

	/* delay latch data timing, clk will not delay for now */
	REG_FCR_FUNC_CTRL &= ~FCR_FUNC_CTRL_SD_SIG_DELAY_MASK;
	REG_FCR_FUNC_CTRL |= FCR_FUNC_CTRL_SD_SIG_DELAY(0); 

	/* host will read data x ns after latching, x is 0(0x00), 3(0x01), 6(0x02), or 9(0x03) on [28:29], now is 0 */
	REG_FCR_FUNC_CTRL &= ~FCR_FUNC_CTRL_READ_CLK_DELAY_MASK;
	REG_FCR_FUNC_CTRL |= FCR_FUNC_CTRL_READ_CLK_DELAY(0);

	/* disable all of HS200 functions for now, 
	 * make sure emmc clk is not used the fastest clk setting, which will force the bus clk = CLKSRC/3 */
	REG_FCR_HS200_CTRL = 0;

	/* must use 3.3V to set up...beacause our design issue... but do not worry, the bus power depends on external CKT :-) */
	if (!_emmc_bus_init(SDC_PW_CTRL_BUS_VOL_33V)) {
		printf("bus init failed");
		return -1;
	}

	/* before issue any cmd, wait at least 74 clk */		
	mdelay(1);

	//strncpy(dev->name, "nt72668 emmc", sizeof(dev->name));
	//dev->send_cmd = host_request;
	//dev->set_ios = host_set_ios;
	//dev->init = mmc_host_reset;
	//dev->getcd = mmc_host_getcd;
	//dev->host_caps = MMC_MODE_HS | MMC_MODE_HS_52MHz | MMC_MODE_HC | MMC_MODE_8BIT;
	//dev->voltages = MMC_VDD_27_28|MMC_VDD_28_29|MMC_VDD_29_30|MMC_VDD_30_31|MMC_VDD_31_32|MMC_VDD_32_33 | MMC_VDD_33_34;
	//dev->f_min = CONFIG_NT72668_EMMC_MIN_OPCLK;
	//dev->f_max = CONFIG_NT72668_EMMC_MAX_OPCLK;
	//dev->b_max = 0;
	//dev->version = MMC_VERSION_4_5;
	//mmc_register(dev);
	cfg->name = dev_name; 
	//cfg->ops = 

	cfg->voltages =  MMC_VDD_165_195 /*| MMC_VDD_32_33 | MMC_VDD_33_34*/;
	cfg->f_min = CONFIG_NT72668_EMMC_MIN_OPCLK;
	cfg->f_max = CONFIG_NT72668_EMMC_MAX_OPCLK;
	cfg->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;
	cfg->host_caps = MMC_MODE_8BIT | MMC_MODE_4BIT | MMC_MODE_HS_52MHz | MMC_MODE_HS |
			     MMC_MODE_HC | MMC_MODE_DDR_52MHz;
#ifdef  CONFIG_NVT_EMMC_HS200
	cfg->host_caps |=  MMC_MODE_HS200;
#endif
	cfg->ops = &nvt_hsmmc_ops;
	dev = mmc_create(cfg, NULL);
	dev->data_latch = 1; //temporial ..remove it later 
	debug("registered mmc interface number is:%d\n", dev->block_dev.dev);
	printf("registered mmc interface number is:%d\n", dev->block_dev.dev);
	printf("MMC DEBUG : %s done \n", __FUNCTION__);

	//while(1);
	return 0;
}
