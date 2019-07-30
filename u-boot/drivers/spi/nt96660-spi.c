/*
 *  driver/spi/nt96660-spi.c
 *  SPI driver for nt96660 platform
 *
 *  Author:	Howard Chang
 *  Created:	Feb  3, 2015
 *  Copyright:	Novatek Inc.
 *
 */

#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <linux/spi/RCWMacro.h>
#include <linux/spi/spi_int.h>
#include <linux/spi/spi_reg.h>

REGDEF_SETIOBASE(SPI_REG_BASE);
#define CFG_SPI_ID SPI_ID_1
//#define CFG_SPI_ID SPI_ID_2
#define CFG_SPI_IDX 0
//#define CFG_SPI_IDX 1

#if (CFG_SPI_IDX==0)
#define SPI_BASE_ADDR           0xF0230000
#define SPI_CLKEN_BIT_SHIFT     6
#define SPI_CLKDIV_ADDR         0xF0020044
#define SPI_CLKDIV_BIT_SHIFT    0
#elif (CFG_SPI_IDX==1)
#define SPI_BASE_ADDR           0xF0320000
#define SPI_CLKEN_BIT_SHIFT     7
#define SPI_CLKDIV_ADDR         0xF0020044
#define SPI_CLKDIV_BIT_SHIFT    16
#elif (CFG_SPI_IDX==2)
#define SPI_BASE_ADDR           0xF0340000
#define SPI_CLKEN_BIT_SHIFT     8
#define SPI_CLKDIV_ADDR         0xF0020048
#define SPI_CLKDIV_BIT_SHIFT    0
#elif (CFG_SPI_IDX==3)
#define SPI_BASE_ADDR           0xF0360000
#define SPI_CLKEN_BIT_SHIFT     15
#define SPI_CLKDIV_ADDR         0xF0020048
#define SPI_CLKDIV_BIT_SHIFT    16
#elif (CFG_SPI_IDX==4)
#define SPI_BASE_ADDR           0xF0390000
#define SPI_CLKEN_BIT_SHIFT     21
#define SPI_CLKDIV_ADDR         0xF002004C
#define SPI_CLKDIV_BIT_SHIFT    0
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
#define HAL_READ_UINT8( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned char *)(_register_)))

#define HAL_WRITE_UINT8( _register_, _value_ ) \
        (*((volatile unsigned char *)(_register_)) = (_value_))

#define HAL_READ_UINT16( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned short *)(_register_)))

#define HAL_WRITE_UINT16( _register_, _value_ ) \
        (*((volatile unsigned short *)(_register_)) = (_value_))

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


static REGVALUE spi_getReg(struct spi_slave *slave, UINT32 offset)
{
	REGVALUE regVal;

	regVal = readl(SPI_REG_BASE+offset);

	return regVal;
}

/*
    Set SPI controller register.

    @param[in] spiID        SPI controller ID
    @param[in] offset       register offset in SPI controller (word alignment)
    @param[in] value        register value

    @return void
*/
static void spi_setReg(struct spi_slave *slave, UINT32 offset, REGVALUE value)
{
	writel(value, SPI_REG_BASE+offset);
}

/*
    Enable SPI DMA transfer

    This function can enable/disable SPI DMA function

    @param[in] bDmaEn   DMA mode selection
                        - @b TRUE: enable DMA
                        - @b FALSE: disable DMA

    @return void
*/

static void spi_waitTDRE(struct spi_slave *slave)
{
	T_SPI_STATUS_REG stsReg;

	while (1)
	{
		stsReg.Reg = spi_getReg(slave, SPI_STATUS_REG_OFS);
		if (stsReg.Bit.spi_tdr_empty)
		{
			break;
		}
	}
}

static void spi_setBuffer(struct spi_slave *slave, u32 uiSPIBuffer)
{
	T_SPI_TDR_REG tdrReg;

	tdrReg.Reg = uiSPIBuffer;
	spi_setReg(slave, SPI_TDR_REG_OFS, tdrReg.Reg);
}

static u32 spi_getBuffer(struct spi_slave *slave)
{
	return spi_getReg(slave, SPI_RDR_REG_OFS);
}

static void spi_waitRDRF(struct spi_slave *slave)
{
	T_SPI_STATUS_REG stsReg;

	while (1)
	{
		stsReg.Reg = spi_getReg(slave, SPI_STATUS_REG_OFS);
		if (stsReg.Bit.spi_rdr_full)
		{
			break;
		}
	}
}

static void spi_waitDMAED(struct spi_slave *slave)
{
	T_SPI_STATUS_REG stsReg;

	while (1)
	{
		stsReg.Reg = spi_getReg(slave, SPI_STATUS_REG_OFS);
		if (stsReg.Bit.spi_dmaed)
		{
			spi_setReg(slave, SPI_STATUS_REG_OFS, (stsReg.Reg & (1<<3)));
			break;
		}
	}
}



//


static void spi_enableDma(struct spi_slave *slave,
                        int bDmaEn)
{
	T_SPI_CTRL_REG ctrlReg;
	T_SPI_CONFIG_REG configReg;
	T_SPI_STATUS_REG stsReg = {0};

	configReg.Reg = spi_getReg(slave, SPI_CONFIG_REG_OFS);
	if (bDmaEn)
	{
		T_SPI_TIMING_REG timingReg;

		timingReg.Reg = spi_getReg(slave, SPI_TIMING_REG_OFS);

		if (timingReg.Bit.spi_post_cond_dly)
		{
			configReg.Bit.spi_pkt_burst_post_cond = 1;
			configReg.Bit.spi_pkt_burst_handshake_en = 1;
			spi_setReg(slave, SPI_CONFIG_REG_OFS, configReg.Reg);
		}
	}
	else
	{
		configReg.Bit.spi_pkt_burst_post_cond = 0;
		configReg.Bit.spi_pkt_burst_handshake_en = 0;
		spi_setReg(slave, SPI_CONFIG_REG_OFS, configReg.Reg);
	}

	stsReg.Bit.dma_abort_sts = 1;
	spi_setReg(slave, SPI_STATUS_REG_OFS, stsReg.Reg);

	ctrlReg.Reg = spi_getReg(slave, SPI_CTRL_REG_OFS);
	if (bDmaEn)
	{
		ctrlReg.Bit.spi_dma_en = 1;
	}
	else
	{
		ctrlReg.Bit.spi_dma_dis = 1;
	}
	spi_setReg(slave, SPI_CTRL_REG_OFS, ctrlReg.Reg);
}

/*
    Set SPI DMA transfer count

    This function can set DMA transfer count.
    Count means needing to write SPI_TDR how many times.

    @param[in] uiCount      transfer count. (unit: word)

    @return void
*/
static void spi_setTransferCount(struct spi_slave *slave, u32 uiCount)
{

	T_SPI_DMA_BUFSIZE_REG dmaSizeReg = {0};

	uiCount<<=2;
	dmaSizeReg.Bit.spi_dma_bufsize = uiCount;
	spi_setReg(slave, SPI_DMA_BUFSIZE_REG_OFS, dmaSizeReg.Reg);
}

/*
    Set SPI DMA starting address

    This function can set DMA starting address.

    @param[in] uiAddr       DMA starting address

    @return void
*/
static void spi_setDmaStartingAddress(struct spi_slave *slave, u32 uiAddr)
{
	T_SPI_DMA_STARTADDR_REG dmaAddrReg = {0};

	dmaAddrReg.Bit.spi_dma_start_addr = dma_getPhyAddr(uiAddr);
	spi_setReg(slave, SPI_DMA_STARTADDR_REG_OFS, dmaAddrReg.Reg);
}

/*
    Set SPI DMA transfer direction

    This function can set DMA transfer direction (SPI->DRAM or DRAM->SPI).

    @param[in] uiDmaDir     DMA direction setting.
                            - @b SPI_DMA_DIR_SPI2RAM
                            - @b SPI_DMA_DIR_RAM2SPI
    @return void
*/
static void spi_setDmaDirection(struct spi_slave *slave, u32 uiDmaDir)
{
	T_SPI_DMA_CTRL_REG dmaCtrlReg = {0};

	dmaCtrlReg.Bit.dma_dir = uiDmaDir;
	spi_setReg(slave, SPI_DMA_CTRL_REG_OFS, dmaCtrlReg.Reg);
}

static void spi_setDmaWrite(struct spi_slave *slave,
                        u32 *puiBuffer,
                        u32 uiWordCount)
{
	// Write-back cache to DRAM
	//HAL_DCACHE_FLUSH((u32)puiBuffer, uiWordCount*4+1);
	//u32 page_begin = PFN_DOWN(virt_to_phys(puiBuffer));
	//u32 page_end = PFN_UP(virt_to_phys(puiBuffer)+uiWordCount*4);
	//u32 num_page = (page_end-page_begin);


	// while(num_page--)
	// {
		// flush_dcache_page(pfn_to_page(page_begin));
		// page_begin++;
	// }

	spi_enableDma(slave, false);                   // disable DMA

	spi_setTransferCount(slave, uiWordCount);
	spi_setDmaStartingAddress(slave, (u32)puiBuffer);
	spi_setDmaDirection(slave, SPI_DMA_DIR_RAM2SPI); // DRAM to SPI

	spi_enableDma(slave, true);
}

/*
    Read data from dma

    @param[in] spiID        SPI controller ID
    @param[in] puiBuffer    the dma buffer address
    @param[in] uiWordCount  the transfer word count

    @return void
*/
static void spi_setDmaRead(struct spi_slave *slave,
                        u32 *puiBuffer,
                        u32 uiWordCount)
{
	// Invalidate cache
	// HAL_DCACHE_FLUSH((u32)puiBuffer, uiWordCount*4+1);
	// eCos invalidate API does NOT handle head/tail non cache line align issue, temp use clean and invalidate
	//u32 page_begin = PFN_DOWN(virt_to_phys(puiBuffer));
	//u32 page_end = PFN_UP(virt_to_phys(puiBuffer)+uiWordCount*4);
	//u32 num_page = (page_end-page_begin);

	// while(num_page--)
	// {
		// flush_dcache_page(pfn_to_page(page_begin));
		// page_begin++;
	// }

	spi_enableDma(slave, false);                   // disable DMA

	spi_setTransferCount(slave, uiWordCount);
	spi_setDmaStartingAddress(slave, (u32)puiBuffer);
	spi_setDmaDirection(slave, SPI_DMA_DIR_SPI2RAM); // SPI to DRAM

	spi_enableDma(slave, true);
}

static void spi_setPacketCount(struct spi_slave *slave,
                                        u32 uiPackets)
{
	T_SPI_CONFIG_REG configReg;

	configReg.Reg = spi_getReg(slave, SPI_CONFIG_REG_OFS);
	switch (uiPackets)
	{
	case 1:
		configReg.Bit.spi_pkt_cnt = SPI_PKT_CNT_ENUM_1PKT;
		break;
	case 2:
		configReg.Bit.spi_pkt_cnt = SPI_PKT_CNT_ENUM_2PKT;
		break;
	default:
		configReg.Bit.spi_pkt_cnt = SPI_PKT_CNT_ENUM_4PKT;
		break;
	}

		spi_setReg(slave, SPI_CONFIG_REG_OFS, configReg.Reg);
}


static void spi_nt96660_transfer_polled(struct spi_slave *slave,u32 count,const u8* tx_data, u8* rx_data)
{
	u32 val;

	spi_setPacketCount(slave, 4);
	for (; count >= 4; count-=4)
	{
		// Wait for transmit data register empty
		spi_waitTDRE(slave);

		// Send next word over the wire
		if (NULL != tx_data)
		{
			val = (tx_data[3]<<24) | (tx_data[2]<<16) | (tx_data[1]<<8) | (tx_data[0]<<0);
			tx_data += 4;
		}
		else
		{
			val = 0;
		}
		spi_setBuffer(slave, val);

		// Wait for reveive data register full
		spi_waitRDRF(slave);

		// Store received byte
		val = spi_getBuffer(slave);

		if (NULL != rx_data)
		{
			rx_data[0] = (val>>0) & 0xFF;
			rx_data[1] = (val>>8) & 0xFF;
			rx_data[2] = (val>>16) & 0xFF;
			rx_data[3] = (val>>24) & 0xFF;
			rx_data += 4;
		}
	}

	spi_setPacketCount(slave, 1);
	for (; count != 0; count--)
	{
		// Wait for transmit data register empty
		spi_waitTDRE(slave);

		// Send next word over the wire
		if (NULL != tx_data)
		{
			val = *tx_data++;
		}
		else
		{
			val = 0;
		}

		spi_setBuffer(slave, val);

		// Wait for reveive data register full
		spi_waitRDRF(slave);

		// Store received byte
		val = spi_getBuffer(slave);

		if (NULL != rx_data)
		{
			*rx_data++ = val;
		}
	}
}
static void spi_nt96660_transfer_dma(struct spi_slave *slave,
                  u32             count,
                  const u8       *tx_data,
                  u8             *rx_data)
{
	// SPI h/w only support full duplex in PIO mode
	if (((NULL != rx_data) && (NULL != tx_data)) || // both rx_data, tx_data is not NULL
	((NULL == rx_data) && (NULL == tx_data)))   // both rx_data, tx_data is NULL
	{
		spi_nt96660_transfer_polled(slave, count, tx_data, rx_data);
		return;
	}

	//
	// 1. Handle heading non-word align parts
	//
	if (NULL != rx_data)
	{

		u32 uiByteOffset = ((u32)rx_data) & 0x03;

		if (uiByteOffset)
		{
			u32 uiLength = sizeof(u32) - uiByteOffset;
			uiLength = (uiLength > count) ? count : uiLength;

			spi_nt96660_transfer_polled(slave, uiLength, tx_data, rx_data);
			rx_data += uiLength;
			count -= uiLength;
		}
	}
	else //if (NULL != tx_data)
	{

		u32 uiByteOffset = ((u32)tx_data) & 0x03;

		if (uiByteOffset)
		{
			u32 uiLength = sizeof(u32) - uiByteOffset;
			uiLength = (uiLength > count) ? count : uiLength;

			spi_nt96660_transfer_polled(slave, uiLength, tx_data, rx_data);
			tx_data += uiLength;
			count -= uiLength;
		}
	}
	//
	// 2. Transfer (Address) Word align parts
	//

	spi_setPacketCount(slave, 4);

	while (count >= 16)
	{
		u32 tr_count = count > (SPI_DMA_MAX_LENGTH&(~0x03)) ?
				(SPI_DMA_MAX_LENGTH&(~0x03)) : (count&(~0x03));

		//atomic_set(&dev->event, 1);

		if (NULL != rx_data)
		{
			spi_setDmaRead(slave, (u32*)rx_data, tr_count/4);
		}
		else //if (NULL != tx_data)
		{
			spi_setDmaWrite(slave, (u32*)tx_data, tr_count/4);
		}

		spi_waitDMAED(slave);

		// Adjust running variables

		if (NULL != rx_data)
		{
			rx_data += tr_count;
		}
		if (NULL != tx_data)
		{
			tx_data += tr_count;
		}
		count   -= tr_count;
	}

	//
	// 3. Handle tailing non-word align parts
	//

	spi_nt96660_transfer_polled(slave, count, tx_data, rx_data);
}

static void spi_nt96660_set_npcs(struct spi_slave *slave,int val)
{
	T_SPI_CTRL_REG ctrlReg;
/*    SPI_ID spiID = spi_bus->spiID; */
	ctrlReg.Reg = spi_getReg(slave, SPI_CTRL_REG_OFS);

	if (val == SPI_ACTIVE_CS)
	{
		ctrlReg.Bit.spics_value = 0;
	}
	else
	{
		ctrlReg.Bit.spics_value = 1;
	}

	spi_setReg(slave, SPI_CTRL_REG_OFS, ctrlReg.Reg);
}

void spi_init()
{
	/*do nothing*/
}


void spi_nt96680_init(void)
{
	uint32_t reg, freq_reg;
	RCW_DEF(SPI_DLY_CHAIN_REG);
	RCW_DEF(SPI_CTRL_REG);
	RCW_DEF(SPI_IO_REG);

	RCW_LD(SPI_CTRL_REG);
	RCW_LD(SPI_IO_REG);
	RCW_LD(SPI_DLY_CHAIN_REG);
	reg = INW(0xF0020000 + 0x74);
	OUTW(0xF0020000 + 0x74, reg | 0x40);

	/* Need use PinCtrl framework */
	reg = INW(0xF0010000 + 0x4);
	reg &= ~0x1000;
	OUTW(0xF0010000 + 0x4, reg | 0x2000);

	reg = INW(0xF0010000 + 0xA0);
	reg &= ~(0x0000011F);
	OUTW(0xF0010000 + 0xA0, reg);

/*For FPGA*/
        reg = INW(0xF0020000 + 0x44);
	reg = 0x27;
	OUTW(0xF0020000 + 0x44, reg);
/*For FPGA*/

	freq_reg = INW(0xF0020000 + 0x44);
	freq_reg &= 0x1FFF;
	freq_reg = 480000000/(freq_reg + 1);

	if (freq_reg < 100000) {
		printf("ERR: force %d Hz to 100 KHz\n", freq_reg);
		freq_reg = 100000;
	} else if (freq_reg > 96000000) {
		printf("ERR: force %d Hz to 96 MHz\n", freq_reg);
		freq_reg = 96000000;
	}

	if (freq_reg > 60000000) {
		RCW_OF(SPI_DLY_CHAIN_REG).latch_clk_shift = SPI_LATCHDELAY_2T;
		RCW_OF(SPI_DLY_CHAIN_REG).latch_clk_edge = 0;
		RCW_OF(SPI_DLY_CHAIN_REG).dly_clk_sel = 4; // delay chain = 4
		RCW_OF(SPI_DLY_CHAIN_REG).dly_clk_src = 1; // clk from pad
		RCW_OF(SPI_DLY_CHAIN_REG).dly_clk_polarity = 1;
		RCW_OF(SPI_DLY_CHAIN_REG).latch_data_src = 1;  // data from delay chain
	} else if (freq_reg == 60000000) {
		RCW_OF(SPI_DLY_CHAIN_REG).latch_clk_shift = SPI_LATCHDELAY_1T;
		RCW_OF(SPI_DLY_CHAIN_REG).latch_clk_edge = 0;
		RCW_OF(SPI_DLY_CHAIN_REG).dly_clk_sel = 0; // delay chain = 0
		RCW_OF(SPI_DLY_CHAIN_REG).dly_clk_src = 1; // clk from pad
		RCW_OF(SPI_DLY_CHAIN_REG).dly_clk_polarity = 1;
		RCW_OF(SPI_DLY_CHAIN_REG).latch_data_src = 1;  // data from delay chain
		RCW_OF(SPI_DLY_CHAIN_REG).latch_clk_edge = 1;
	} else {
		RCW_OF(SPI_DLY_CHAIN_REG).latch_clk_shift = SPI_LATCHDELAY_NORMAL;
		RCW_OF(SPI_DLY_CHAIN_REG).latch_clk_edge = 1;
	}

	/*Set the delay chain*/
	RCW_ST(SPI_DLY_CHAIN_REG);

	RCW_OF(SPI_IO_REG).spi_io_out_en = 1;
	RCW_OF(SPI_IO_REG).spi_auto_io_out_en = 1;
	RCW_OF(SPI_IO_REG).spi_io_order = 1;

	RCW_ST(SPI_IO_REG);

	do{
		RCW_OF(SPI_CTRL_REG).spi_en = 1;
		RCW_OF(SPI_CTRL_REG).spics_value = 1;
		RCW_ST(SPI_CTRL_REG);
		RCW_LD(SPI_CTRL_REG);
	}while (RCW_OF(SPI_CTRL_REG).spi_en == 0);

}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct spi_slave	*as = NULL;

	T_SPI_IO_REG ioReg = {0};
	T_SPI_CTRL_REG ctrlReg = {0};

	uint32_t uiClkDiv;
	uint32_t spi_max_hz;

	printf("spi_setup_slave: bus:%i cs:%i max_hz:%i mode:%i\n",
			bus, cs, max_hz, mode);

	as = malloc(sizeof(struct spi_slave));
	if (!as)
		return NULL;

	as->bus = bus;
	as->cs = cs;
	spi_max_hz = max_hz;

	if (spi_max_hz < 1875000)
	{
		printf("ERR: %s force %d Hz to 1.875 MHz\r\n", __func__, spi_max_hz);
		spi_max_hz = 1875000;
	}
	else if (spi_max_hz > 96000000)
	{
		printf("ERR: %s force %d Hz to 96 MHz\r\n", __func__, spi_max_hz);
		spi_max_hz = 96000000;
	}


	uiClkDiv = (480000000 + spi_max_hz - 1) / spi_max_hz;
	if (uiClkDiv == 0)
	{
		uiClkDiv = 1;
	}
	uiClkDiv--;

	if ((480000000/(uiClkDiv+1)) != spi_max_hz)
	{
		printf("ERR: %s force %d Hz to %d Hz\r\n", __func__, spi_max_hz, (480000000/(uiClkDiv+1)));
		spi_max_hz = (480000000/(uiClkDiv+1));
	}

    // Store clock divider to CG register
//	if (c->clk != NULL) {
//		clk_set_rate(c->clk, spi_max_hz);
//	}


	if (mode&SPI_CPOL)
	{
		ioReg.Bit.spi_cpol = 1;
	}
	if (mode&SPI_CPHA)
	{
		ioReg.Bit.spi_cpha = 1;
	}

	ioReg.Bit.spi_io_out_en = 1;
	ioReg.Bit.spi_auto_io_out_en = 1;

	spi_setReg(as, SPI_IO_REG_OFS, ioReg.Reg);

	ctrlReg.Bit.spi_en = 1;
	spi_setReg(as, SPI_CTRL_REG_OFS, ctrlReg.Reg);

	/* deselect chip */
	spi_nt96660_set_npcs(as,SPI_INACTIVE_CS);

	return as;
}

void spi_free_slave(struct spi_slave *slave)
{
	/* do nothing*/
}

int spi_claim_bus(struct spi_slave *slave)
{
	/* do nothing*/
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{

	/* do nothing*/
}


int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
	     const void *dout, void *din, unsigned long flags)
{

	const u8 *tx_data = (const u8*)dout;
	u8 *rx_data = din;
	unsigned int count;


	if ((flags & SPI_XFER_BEGIN) == SPI_XFER_BEGIN)
		spi_nt96660_set_npcs(slave, SPI_ACTIVE_CS);

	if (bitlen % 8)
		return -1;

	count = bitlen / 8;

	spi_nt96660_transfer_dma(slave,count,tx_data,rx_data);

	if ((flags & SPI_XFER_END) == SPI_XFER_END)
		spi_nt96660_set_npcs(slave,SPI_INACTIVE_CS);


	return 0;
}

