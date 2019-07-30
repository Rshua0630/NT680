/* Flash opcodes. */
#define	OPCODE_WREN			0x06	/* Write enable */
#define	OPCODE_RDSR			0x05	/* Read status register_1 */
#define OPCODE_RDSR_2		0x35    /* Read status register_2 */
#define OPCODE_RDSR_3		0x15	/* Read status register_3 */
#define	OPCODE_WRSR			0x01	/* Write status register_1 */
#define OPCODE_WRSR_2		0x31    /* Write status register_2 */
#define OPCODE_WRSR_3		0x11	/* Write status register_3 */
#define	OPCODE_NORM_READ	0x03	/* Read data bytes (low frequency) */
#define	OPCODE_FAST_READ	0x0b	/* Read data bytes (high frequency) */
#define OPCODE_QIO_READ		0xeb	/* Read data bytes (4-bit high freq) */
/* Page program (up to 256 bytes) */
#define	OPCODE_PP			0x02
#define	OPCODE_BE_4K		0x20	/* Erase 4KiB block */
#define	OPCODE_BE_32K		0x52	/* Erase 32KiB block */
#define	OPCODE_CHIP_ERASE	0xc7	/* Erase whole flash chip */
/* Sector erase (usually 64KiB) */
#define	OPCODE_SE			0xd8
#define	OPCODE_RDID			0x9f	/* Read JEDEC ID */

/* Used for SST flashes only. */
#define	OPCODE_BP			0x02	/* Byte program */
#define	OPCODE_WRDI			0x04	/* Write disable */
/* Auto address increment word program */
#define	OPCODE_AAI_WP		0xad

/* Used for Macronix flashes only. */
#define	OPCODE_EN4B			0xb7	/* Enter 4-byte mode */
#define	OPCODE_EX4B			0xe9	/* Exit 4-byte mode */

/* Used for Spansion flashes only. */
#define	OPCODE_BRWR			0x17	/* Bank register write */

/* Status Register bits. */
#define	SR_WIP				1		/* Write in progress */
#define	SR_WEL				2		/* Write enable latch */
/* meaning of other SR_* bits may differ between vendors */
#define	SR_BP0				4		/* Block protect 0 */
#define	SR_BP1				8		/* Block protect 1 */
#define	SR_BP2				0x10	/* Block protect 2 */
#define	SR_SRWD				0x80	/* SR write protect */
#define SR_QE				0x2

/* Define max times to check status register before we give up. */

/* M25P16 specs 40s max chip erase */
#define	MAX_READY_WAIT_JIFFIES	(40 * HZ)
#define	MAX_CMD_SIZE		5

#define JEDEC_MFR(_jedec_id)	((_jedec_id) >> 16)

/*STBC switch*/
#define NVT_REG_STBC_BASE		0xFC040000
#define NVT_REG_MODE_OPTION		0x284

/* clk setting */
#define NVT_REG_CLK_SET_BASE    0xFD020000
#define NVT_REG_MPLL_BUS_BASE   0xFD670000

#define CLK_SET_OFFSET			0xbc

#define NVT_MPLL_ADDR1			0x6d
#define NVT_MPLL_ADDR2			0x6e
#define NVT_MPLL_ADDR3			0x6f

#define NVT_MPLL_INDEX			0x98

#define SPI_CLOCK_SOURCE		0x218
#define SPI_CLOCK_SOURCE_PAGEB	0x1
#define SPI_CLOCK_SOURCE_PAGE0	0x2
#define SPI_CLOCK_SOURCE_OSC16X 0x1
/* (SPI clock = 12M * 16 / (divider + 1)) */
#define SPI_CLOCK_DIVIDER		0x240

#define GPIO_REG_DIRECTION		0x42C
#define GPIO_REG_HIGH_LOW		0x420
#define GPIO_REG_FUNCTION		0x438
#define GPIO_REG_AGPIO_SW		0x220
#define GPIO_REG_SPI_WP			(1 << 18)
#define GPIO_REG_SPI_ARM		(1 << 14)

#define GPIO_ENABLE_SPI_FUNC	0xfc000

#define ADDR_MUTI_BIT_MODE_ENABLE	0x1

#define SPI_PREFORMANCE_TEST		0x0

/* ioremap */
#define HAL_WRITE_UINT32(Addr, val)    iowrite32(val, Addr)
#define HAL_READ_UINT32(Addr, Store)   (Store = ioread32(Addr))
#define RREG_ADDR(Addr)                  ioread32(Addr)
