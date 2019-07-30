/*
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _NT96660_H_
#define _NT96660_H_

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

#define HAL_EDID_REG_CTRL           0xcc040214UL
/*********************************************************/
//#define PanelON
//#define MusicON
#define NTREG_BASE_ATV	   		0xCE000000UL
#if 0 // jj TODO: these regs are changed in NT72568
#define NTREG_BASE_SYS_A   	0xCC0C0000UL // ATV System base address
#define NTREG_BASE_SYS_D   	0xCC0D0000UL // DTV System base address
#define NTREG_BASE_MPLL_A  	0xCC0C1000UL // MPLL base address (page A)
#define NTREG_BASE_GPIO       0xCD0D0000UL
#endif // ~jj
#define NTREG_BASE_OSD        0xCE000E00UL
#define NTREG_BASE_LBM        0xCE000900UL
#define NTREG_BASE_DP			0xCE000F00UL


//OSD Register
#define REG_OSD_ENABLE								*((volatile unsigned int*)0xbe000e00)
#define REG_OSD_P2_CONTROL						*((volatile unsigned int*)0xbe000e08)
#define REG_OSD_UPDATE								*((volatile unsigned int*)0xbe000e0c)
#define REG_OSD_P2_DISPLAY_POSITION				*((volatile unsigned int*)0xbe000e98)
#define REG_OSD_P2_LINE_OFFSET					*((volatile unsigned int*)0xbe000e9c)
#define REG_OSD_P2_DISPLAY_SIZE					*((volatile unsigned int*)0xbe000ea0)
#define REG_OSD_P2_H_SCALING_FACTOR				*((volatile unsigned int*)0xbe000ea4)
#define REG_OSD_P2_V_SCALING_FACTOR				*((volatile unsigned int*)0xbe000ea8)

//LBM register
#define REG_LBM_OSD_START_ADDR					*((volatile unsigned int*)0xbe00091c)
#define REG_LBM_ATV_FREEZE						*((volatile unsigned int*)0xbe000984)

//ATV interrupt register
#define REG_ATV_INT_CONFIG						*((volatile unsigned int*)0xbe000100)
#define REG_ATV_INT_FLG0							*((volatile unsigned int*)0xbe000104)
#define REG_ATV_INT_FLG1							*((volatile unsigned int*)0xbe000108)
#define REG_ATV_INT_FLG2							*((volatile unsigned int*)0xbe00010C)
#define REG_AHB_PRI								*((volatile unsigned int*)0xbC060000)

//MUSIC register
#define NTREG_BASE_SIF			0xBE110000UL
#define NTREG_BASE_AUDIOCLK	0xBE130000UL
#define NTREG_BASE_DAC			0xBE180000UL
#define NTREG_BASE_IIS			0xBE181000UL
#define NTREG_BASE_IISCLK		0xBE181300UL

//IIC register
#define NTREG_BASE_I2CMST        0xBD000000UL //MB
/*********************************************************/
/*
 * DMA control register
 */
#define HAL_DMA_REG_BASE			0xcc0a0080UL

#define HAL_DMA_CHA_SRC_ADDR		(HAL_DMA_REG_BASE + 0x00)
#define HAL_DMA_CHA_DEST_ADDR		(HAL_DMA_REG_BASE + 0x04)
#define HAL_DMA_CHA_COUNT			(HAL_DMA_REG_BASE + 0x08)
#define HAL_DMA_CHA_CMD				(HAL_DMA_REG_BASE + 0x0c)

#define HAL_DMA_CHB_SRC_ADDR		(HAL_DMA_REG_BASE + 0x10)
#define HAL_DMA_CHB_DEST_ADDR		(HAL_DMA_REG_BASE + 0x14)
#define HAL_DMA_CHB_COUNT			(HAL_DMA_REG_BASE + 0x18)
#define HAL_DMA_CHB_CMD				(HAL_DMA_REG_BASE + 0x1c)

#define HAL_DMA_CHC_SRC_ADDR		(HAL_DMA_REG_BASE + 0x20)
#define HAL_DMA_CHC_DEST_ADDR		(HAL_DMA_REG_BASE + 0x24)
#define HAL_DMA_CHC_COUNT			(HAL_DMA_REG_BASE + 0x28)
#define HAL_DMA_CHC_CMD				(HAL_DMA_REG_BASE + 0x2c)

#define HAL_DMA_CHD_SRC_ADDR		(HAL_DMA_REG_BASE + 0x30)
#define HAL_DMA_CHD_DEST_ADDR		(HAL_DMA_REG_BASE + 0x34)
#define HAL_DMA_CHD_COUNT			(HAL_DMA_REG_BASE + 0x38)
#define HAL_DMA_CHD_CMD				(HAL_DMA_REG_BASE + 0x3c)

#define HAL_DMA_ENABLE				0x00000001UL
#define HAL_DMA_FINISH_STATUS		0x00000002UL
#define HAL_DMA_FINISH_INTR_ENABLE	0x00000004UL
#define HAL_DMA_BURST_MODE			0x00000008UL
#define HAL_DMA_ERR_RSP_STATUS		0x00000010UL
#define HAL_DMA_ERR_RSP_INTR_ENABLE	0x00000020UL
#define HAL_DMA_SRC_ADDR_BUS		0x00000040UL
#define HAL_DMA_DEST_ADDR_BUS		0x00000080UL
#define HAL_DMA_SRC_ADDR_INCR		0x00000700UL
#define HAL_DMA_DEST_ADDR_INCR		0x00007000UL
#define HAL_DMA_DATA_WIDTH			0x00300000UL
#define HAL_DMA_REQ_SIGNAL_SEL		0x0f000000UL

#define DMA_ENABLE					0x00000001UL
#define DMA_DISABLE					0x00000000UL

#define DMA_SRC_BURST_NO_INC		0x00000000UL
#define DMA_SRC_BURST_INC_1			0x00000100UL
#define DMA_SRC_BURST_INC_2			0x00000200UL
#define DMA_SRC_BURST_INC_4			0x00000300UL
#define DMA_SRC_BURST_DEC_1			0x00000500UL
#define DMA_SRC_BURST_DEC_2			0x00000600UL
#define DMA_SRC_BURST_DEC_4			0x00000700UL

#define DMA_DST_BURST_NO_INC		0x00000000UL
#define DMA_DST_BURST_INC_1			0x00001000UL
#define DMA_DST_BURST_INC_2			0x00002000UL
#define DMA_DST_BURST_INC_4			0x00003000UL
#define DMA_DST_BURST_DEC_1			0x00005000UL
#define DMA_DST_BURST_DEC_2			0x00006000UL
#define DMA_DST_BURST_DEC_4			0x00007000UL

#define DMA_SRC_ADDR_APB_BUS		0x00000000UL
#define DMA_SRC_ADDR_AHB_BUS		0x00000040UL
#define DMA_DEST_ADDR_APB_BUS		0x00000000UL
#define DMA_DEST_ADDR_AHB_BUS		0x00000080UL

#define DMA_DATA_WIDTH_WORD			0x00000000UL
#define DMA_DATA_WIDTH_HWORD		0x00100000UL
#define DMA_DATA_WIDTH_BYTE			0x00200000UL

#define DMA_REQ_SIGNAL_NO			0x00000000UL
#define DMA_REQ_SIGNAL_PB			0x01000000UL
#define DMA_REQ_SIGNAL_SDMMC		0x02000000UL
#define DMA_REQ_SIGNAL_CF			0x03000000UL
#define DMA_REQ_SIGNAL_SMC			0x04000000UL
#define DMA_REQ_SIGNAL_USB			0x05000000UL
#define DMA_REQ_SIGNAL_SDIO			0x05000000UL
#define DMA_REQ_SIGNAL_AUDIO		0x06000000UL
#define DMA_REQ_SIGNAL_SPDIF		0x06000000UL
#define DMA_REQ_SIGNAL_MSPRO		0x07000000UL
#define DMA_REQ_SIGNAL_SPI			0x09000000UL

/*
 * spi flash register
 */
#define HAL_SPI_REG_BASE			0xbd081000UL

#define HAL_SPI_CTRL				(HAL_SPI_REG_BASE + 0x00)

#define HAL_SPI_CMD					(HAL_SPI_REG_BASE + 0x04)
#define HAL_SPI_ADDR				(HAL_SPI_REG_BASE + 0x04)

#define HAL_SPI_ACCESS_CTRL			(HAL_SPI_REG_BASE + 0x08)
#define HAL_SPI_ACCESS_DATA0		(HAL_SPI_REG_BASE + 0x10)
#define HAL_SPI_ACCESS_DATA1		(HAL_SPI_REG_BASE + 0x14)
#define HAL_SPI_ACCESS_DATA2		(HAL_SPI_REG_BASE + 0x18)
#define HAL_SPI_ACCESS_DATA3		(HAL_SPI_REG_BASE + 0x1c)

#define HAL_SPI_DMA_CTRL			(HAL_SPI_REG_BASE + 0x20)
#define HAL_SPI_DMA_DATAPORT		(HAL_SPI_REG_BASE + 0x24)

#define HAL_SPI_INTERRUPT			(HAL_SPI_REG_BASE + 0x30)

#define HAL_SPI_TRANSFORM_INDEX		(HAL_SPI_REG_BASE + 0x40)
#define HAL_SPI_TRANSFORM_ROWDATA	(HAL_SPI_REG_BASE + 0x44)
#define HAL_SPI_TRANSFORM_DATA		(HAL_SPI_REG_BASE + 0x48)

/* bit mapping of spi control register */
#define HAL_SPI_CTRL_DIR			0x00000001UL
#define HAL_SPI_CTRL_LOCK			0x00000002UL
#define HAL_SPI_CTRL_ADDR_EN		0x00000004UL
#define HAL_SPI_CTRL_DUMMY_EN		0x00000008UL
#define HAL_SPI_CTRL_DUMMY_CNT		0x000001f0UL
#define HAL_SPI_CTRL_WIDTH_SELECT	0x00003000UL

/* bit mapping of spi command register */
#define HAL_SPI_CMD_MASK			0x000000ffUL
#define HAL_SPI_ADDR_MASK			0xffffff00UL

/* bit mapping of spi access control register */
#define HAL_SPI_ACCESS_CTRL_START	0x00000001UL
#define HAL_SPI_ACCESS_CTRL_ENABLE	0x00000008UL
#define HAL_SPI_ACCESS_CTRL_COUNT	0x000000f0UL

/* bit mapping of spi dma control register */
#define HAL_SPI_DMA_DATA_COUNT		0x0003ffffUL
#define HAL_SPI_DMA_START			0x01000000UL

/* bit mapping of spi interrupt register */
#define HAL_SPI_INTERRUPT_ENABLE	0x00000001UL
#define HAL_SPI_INTERRUPT_FINISH	0x00000100UL

/* GPIO setting related to SPI */
/* nvt 72568 GPIO register */
#define HAL_GPIO_REG_BASE                       0xbc040400
#define HAL_GPIO_REG_DIRECTION                  (HAL_GPIO_REG_BASE + 0x2C)
#define HAL_GPIO_REG_HIGH_LOW                   (HAL_GPIO_REG_BASE + 0x20)
#define HAL_GPIO_REG_FUNCTION                   (HAL_GPIO_REG_BASE + 0x38)
#define HAL_GPIO_REG_SPI_CE0                    (1 << 14)
#define HAL_SPI_CLOCK_SOURCE                    0xbc040218
#define HAL_SPI_CLOCK_SOURCE_MSK                0x1
#define HAL_SPI_CLOCK_SOURCE_OSC16X             0x1
#define HAL_SPI_CLOCK_DIVIDER                   0xbc040240 //(SPI clock = 12M * 16 / (divider + 1))
#define HAL_SPI_ADDRESS_MODE                    0xbc040284 // 1:interleave mode 0:normal

/*
 * uart register
 */
#define HAL_UART_REG_BASE			0xC0290000UL

#define HAL_UARTX_CLKMHZ(_which_)	((_which_==0)?HAL_UART0_CLKMHZ:HAL_UART1_CLKMHZ)// (HAL_UART0_CLKMHZ + (_which_)*24000000)
#define HAL_UARTX_BASE(_which_)		(HAL_UART_REG_BASE + (_which_)*0x70000)

#define HAL_UARTx_THR(_which_)		(HAL_UARTX_BASE(_which_) + 0x00)
#define HAL_UARTx_RBR(_which_)		(HAL_UARTX_BASE(_which_) + 0x00)
#define HAL_UARTx_DLABLO(_which_)	(HAL_UARTX_BASE(_which_) + 0x00)
#define HAL_UARTx_DLABHI(_which_)	(HAL_UARTX_BASE(_which_) + 0x04)
#define HAL_UARTx_IER(_which_)		(HAL_UARTX_BASE(_which_) + 0x04)
#define HAL_UARTx_IIR(_which_)		(HAL_UARTX_BASE(_which_) + 0x08)
#define HAL_UARTx_IFCR(_which_)		(HAL_UARTX_BASE(_which_) + 0x08)
#define HAL_UARTx_LCR(_which_)		(HAL_UARTX_BASE(_which_) + 0x0c)
#define HAL_UARTx_LSR(_which_)		(HAL_UARTX_BASE(_which_) + 0x14)
#define HAL_UARTx_SCRATCH(_which_)	(HAL_UARTX_BASE(_which_) + 0x1c)

#define HAL_UART0_CLKMHZ		    24000000
#define HAL_UART0_BASE				HAL_UARTX_BASE(0)

#define HAL_UART0_THR				HAL_UARTx_THR(0)
#define HAL_UART0_RBR				HAL_UARTx_RBR(0)
#define HAL_UART0_DLABLO			HAL_UARTx_DLABLO(0)
#define HAL_UART0_DLABHI			HAL_UARTx_DLABHI(0)
#define HAL_UART0_IER				HAL_UARTx_IER(0)
#define HAL_UART0_IIR				HAL_UARTx_IIR(0)
#define HAL_UART0_IFCR				HAL_UARTx_IFCR(0)
#define HAL_UART0_LCR				HAL_UARTx_LCR(0)
#define HAL_UART0_LSR				HAL_UARTx_LSR(0)
#define HAL_UART0_SCRATCH			HAL_UARTx_SCRATCH(0)

#ifdef CONFIG_FPGA_EMULATION
#define HAL_UART1_CLKMHZ		    24000000
#else
#define HAL_UART1_CLKMHZ		    48000000
#endif
#define HAL_UART1_BASE				HAL_UARTX_BASE(1)

#define HAL_UART1_THR				HAL_UARTx_THR(1)
#define HAL_UART1_RBR				HAL_UARTx_RBR(1)
#define HAL_UART1_DLABLO			HAL_UARTx_DLABLO(1)
#define HAL_UART1_DLABHI			HAL_UARTx_DLABHI(1)
#define HAL_UART1_IER				HAL_UARTx_IER(1)
#define HAL_UART1_IIR				HAL_UARTx_IIR(1)
#define HAL_UART1_IFCR				HAL_UARTx_IFCR(1)
#define HAL_UART1_LCR				HAL_UARTx_LCR(1)
#define HAL_UART1_LSR				HAL_UARTx_LSR(1)
#define HAL_UART1_SCRATCH			HAL_UARTx_SCRATCH(1)

/* bit mapping of IER register */
#define HAL_UART_IER_RXDATA			0x00000001UL
#define HAL_UART_IER_THREMPTY		0x00000002UL
#define HAL_UART_IER_LINESTATUS		0x00000004UL
#define HAL_UART_IER_INTMASK		0x00000007UL

/* bit mapping of IIR register */
#define HAL_UART_IIR_INT_PENDING	0x00000001UL
#define HAL_UART_IIR_INT_THREMPTY	0x00000002UL
#define HAL_UART_IIR_INT_RXDATA		0x00000004UL
#define HAL_UART_IIR_INT_LINESTATUS	0x00000006UL
#define HAL_UART_IIR_INT_TIMEOUT	0x0000000cUL
#define HAL_UART_IIR_INT_ID			0x0000000fUL

/* bit mapping of FCR register */
#define HAL_UART_FCR_FIFO_ENABLE	0x00000001UL
#define HAL_UART_FCR_RXFIFO_RST		0x00000002UL
#define HAL_UART_FCR_TXFIFO_RST		0x00000004UL
#define HAL_UART_FCR_MODE_DMA		0x00000008UL
#define HAL_UART_FCR_RXTRIGGER_LV	0x000000c0UL

#define FCR_RX_SHIFBIT			6
#define FCR_RXTRIGGER_LV1		(0x00 << FCR_RX_SHIFBIT)
#define FCR_RXTRIGGER_LV4		(0x01 << FCR_RX_SHIFBIT)
#define FCR_RXTRIGGER_LV8		(0x02 << FCR_RX_SHIFBIT)
#define FCR_RXTRIGGER_LV14		(0x03 << FCR_RX_SHIFBIT)

/* bit mapping of LCR register */
#define HAL_UART_LCR_CHARLENGTH		0x00000003UL
#define HAL_UART_LCR_STOPBIT		0x00000004UL
#define HAL_UART_LCR_PARITY_ENABLE	0x00000008UL
#define HAL_UART_LCR_PARITY_EVEN	0x00000010UL
#define HAL_UART_LCR_PARITY_STICK	0x00000020UL
#define HAL_UART_LCR_MODE_LOOPBACK	0x00000040UL
#define HAL_UART_LCR_DLAB			0x00000080UL

#define LCR_CHAR_LEN5		0x00000000UL
#define LCR_CHAR_LEN6		0x00000001UL
#define LCR_CHAR_LEN7		0x00000002UL
#define LCR_CHAR_LEN8		0x00000003UL

#define LCR_STOP1			0x00000000UL
#define LCR_STOP2			0x00000004UL

#define LCR_PARITY_DISABLE	0x00000000UL
#define LCR_PARITY_ENABLE	0x00000008UL

#define LCR_PARITY_STICK_DISABLE	0x00000000UL
#define LCR_PARITY_STICK_ENABLE		0x00000004UL

#define LCR_EVEN_PARITY		0x00000010
#define LCR_STICK_PARITY	0x00000020

/* bit mapping of LSR register */
#define HAL_UART_LSR_DATAREADY		0x00000001UL
#define HAL_UART_LSR_ERR_OVERRUN	0x00000002UL
#define HAL_UART_LSR_ERR_PARITY		0x00000004UL
#define HAL_UART_LSR_ERR_FRAMING	0x00000008UL
#define HAL_UART_LSR_THREMPTY		0x00000020UL
#define HAL_UART_LSR_TXEMPTY		0x00000040UL
#define HAL_UART_LSR_ERR_RXFIFO		0x00000080UL

#define CPU_PLL_T		0x62
#define AXI_PLL_T		0x6a
#define DDR_PLL_T		0x82

/* DDR config register */
#define REG_DDR_SETTING		*((volatile unsigned int*)0xBD010008)
#define DDR_SETTING_16BIT		(1 << 28)
#endif /* _NT72568_H_ */
