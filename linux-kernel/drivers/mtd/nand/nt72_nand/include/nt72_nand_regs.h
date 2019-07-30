#ifndef _NT72_NAND_REGS_H_
#define _NT72_NAND_REGS_H_

#include "nt72_nand_config.h"
#include "nt72_nand_io.h"

#define IRQ_NFC_NUM                     20

/* utilities */
#define nvt_bset(_var, val)              ((_var) |= (val))
#define nvt_bclear(_var, val)            ((_var) &= (~(val)))

/* CLK GEN */
#ifdef USE_NEW_MAPPING
#define REG_CLK_GEN_BASE                (0xfd020000)
#define REG_CLK_GEN_IC_ID               (REG_CLK_GEN_BASE + 0x100)
#else
#define REG_CLK_GEN_BASE                (0xbd020000)
#define REG_CLK_GEN_IC_ID               (REG_CLK_GEN_BASE + 0x100)
#endif /* USE_NEW_MAPPING */

/* STBC */
#ifdef USE_NEW_MAPPING
#define REG_STBC_BASE                   (0xfc040000)
#define REG_STBC_BOOTSTRAP              (0xfc04023c)
#define REG_STBC_1C                     (0xfc04021c)
#define STBC_1C_NFC_RST                 (1 << 25)
#else
#define REG_STBC_BASE                   (0xbc040000)
#define REG_STBC_BOOTSTRAP              (0xbc04023c)
#define REG_STBC_1C                     (0xbc04021c)
#define STBC_1C_NFC_RST                 (1 << 25)
#endif /* USE_NEW_MAPPING */

/* NFC registers */
#ifdef USE_NEW_MAPPING
/* ARM */
#define REG_NFC_BASE                    (0xfc048000)
#else
/* MIPS */
#define REG_NFC_BASE                    (0xbc048000)
#endif /* USE_NEW_MAPPING */

#define REG_NFC_CFG0            (REG_NFC_BASE + 0x00)
#define REG_NFC_CFG1            (REG_NFC_BASE + 0x04)
#define REG_NFC_CMD             (REG_NFC_BASE + 0x08)
#define REG_NFC_XFER            (REG_NFC_BASE + 0x0c)
#define REG_NFC_COLADR          (REG_NFC_BASE + 0x10)
#define REG_NFC_ROWADR          (REG_NFC_BASE + 0x14)
#define REG_NFC_SFTRST          (REG_NFC_BASE + 0x18)
#define REG_NFC_RNDACC          (REG_NFC_BASE + 0x1c)
#define REG_NFC_INTCTR          (REG_NFC_BASE + 0x20)
#define REG_NFC_INTSTS          (REG_NFC_BASE + 0x24)
#define REG_NFC_DATA            (REG_NFC_BASE + 0x28)
#define REG_NFC_RBOOTADDR       (REG_NFC_BASE + 0x2c)     /* for 8051 */

#define REG_NFC_STS             (REG_NFC_BASE + 0x44)
#define REG_NFC_SYSADR          (REG_NFC_BASE + 0x50)
#define REG_NFC_DMACTR          (REG_NFC_BASE + 0x54)
#define REG_NFC_SYSCTRL         (REG_NFC_BASE + 0x5c)
#define REG_NFC_RSECC0          (REG_NFC_BASE + 0x60)
#define REG_NFC_RSECC1          (REG_NFC_BASE + 0x64)
#define REG_NFC_RSECC2          (REG_NFC_BASE + 0x68)
#define REG_NFC_RSECC3          (REG_NFC_BASE + 0x6c)
#define REG_NFC_XD_CFG          (REG_NFC_BASE + 0x78)
#define REG_NFC_XTRA_ADDR       (REG_NFC_BASE + 0x104)
#define REG_NFC_MBOOTADDR       (REG_NFC_BASE + 0x108)    /* for mips */
#define REG_NFC_SYSCTRL1        (REG_NFC_BASE + 0x10c)
#define REG_NFC_FTUNE           (REG_NFC_BASE + 0x110)
#define REG_NFC_ERR_CNT0        (REG_NFC_BASE + 0x114)
#define REG_NFC_ERR_CNT1        (REG_NFC_BASE + 0x118)
#define REG_NFC_SYSCTRL2        (REG_NFC_BASE + 0x11C)    /* 568 new register */
#define REG_NFC_ERR_CNT2        (REG_NFC_BASE + 0x130)
#define REG_NFC_ERR_CNT3        (REG_NFC_BASE + 0x134)

#define REG_NFC_XTRA_DATA0      (REG_NFC_BASE + 0x30)
#define REG_NFC_XTRA_DATA1      (REG_NFC_BASE + 0x34)
#define REG_NFC_XTRA_DATA2      (REG_NFC_BASE + 0x38)
#define REG_NFC_XTRA_DATA3      (REG_NFC_BASE + 0x3c)

#define REG_NFC_XTRA_DATA4      (REG_NFC_BASE + 0x7c)
#define REG_NFC_XTRA_DATA5      (REG_NFC_BASE + 0x80)
#define REG_NFC_XTRA_DATA6      (REG_NFC_BASE + 0x84)
#define REG_NFC_XTRA_DATA7      (REG_NFC_BASE + 0x88)
#define REG_NFC_XTRA_DATA8      (REG_NFC_BASE + 0x8c)
#define REG_NFC_XTRA_DATA9      (REG_NFC_BASE + 0x90)
#define REG_NFC_XTRA_DATA10     (REG_NFC_BASE + 0x94)
#define REG_NFC_XTRA_DATA11     (REG_NFC_BASE + 0x98)
#define REG_NFC_XTRA_DATA12     (REG_NFC_BASE + 0x9c)
#define REG_NFC_XTRA_DATA13     (REG_NFC_BASE + 0xa0)
#define REG_NFC_XTRA_DATA14     (REG_NFC_BASE + 0xa4)
#define REG_NFC_XTRA_DATA15     (REG_NFC_BASE + 0xa8)
#define REG_NFC_XTRA_DATA16     (REG_NFC_BASE + 0xac)
#define REG_NFC_XTRA_DATA17     (REG_NFC_BASE + 0xb0)
#define REG_NFC_XTRA_DATA18     (REG_NFC_BASE + 0xb4)
#define REG_NFC_XTRA_DATA19     (REG_NFC_BASE + 0xb8)
#define REG_NFC_XTRA_DATA20     (REG_NFC_BASE + 0xbc)
#define REG_NFC_XTRA_DATA21     (REG_NFC_BASE + 0xc0)
#define REG_NFC_XTRA_DATA22     (REG_NFC_BASE + 0xc4)
#define REG_NFC_XTRA_DATA23     (REG_NFC_BASE + 0xc8)
#define REG_NFC_XTRA_DATA24     (REG_NFC_BASE + 0xcc)
#define REG_NFC_XTRA_DATA25     (REG_NFC_BASE + 0xd0)
#define REG_NFC_XTRA_DATA26     (REG_NFC_BASE + 0xd4)
#define REG_NFC_XTRA_DATA27     (REG_NFC_BASE + 0xd8)

/* REG_NFC_CFG1 */
#define NFC_CFG1_READY_TO_BUSY_TIMEOUT(n)   ((0xf & (n)) << 24)
#define NFC_CFG1_ECC_ENCODE_RESET           (0x1 << 23)
#define NFC_CFG1_ECC_DECODE_RESET           (0x1 << 22)
#define NFC_CFG1_DATA_INV                   (0x1 << 20)
#define NFC_CFG1_LITTLE_ENDIAN_XTRA         (0x1 << 19)
#define NFC_CFG1_LITTLE_ENDIAN              (0x1 << 18)
#define NFC_CFG1_BCH_EN                     (0x1 << 17)
#define NFC_CFG1_BUSY_TO_READY_TIMEOUT(n)   (0xffff & (n))
#define NFC_CFG1_ECC_RS                     ((0x0 << 21) | (0x0 << 16))
#define NFC_CFG1_ECC_BCH8_16                ((0x0 << 21) | (0x1 << 16))
#define NFC_CFG1_ECC_BCH8_32                ((0x1 << 21) | (0x1 << 16))
#define NFC_CFG1_ECC_BCH8_56                ((0x1 << 21) | (0x1 << 16))

#define NFC_CFG1_ECC_RS_NEW                 (0x1 << 16)

/* REG_NFC_CMD */
#define NFC_CMD_CE_IDX(n)                   ((7 & (n)) << 28)
#define NFC_CMD_CHIP_SEL(n)                 ((7 & (n)) << 28)
#define NFC_CMD_WP_NEG                      (0x1 << 27)
#define NFC_CMD_WP_KEEP                     (0x1 << 26)
#define NFC_CMD_DATA_XFER                   (0x1 << 23)
#define NFC_CMD_ADDR_CYCLE(n)               ((0x7 & (n)) << 20)
#define NFC_CMD_ADDR_CYCLE_NONE             NFC_CMD_ADDR_CYCLE(0)
#define NFC_CMD_ADDR_CYCLE_DUMMY            NFC_CMD_ADDR_CYCLE(1)
#define NFC_CMD_ADDR_CYCLE_COL              NFC_CMD_ADDR_CYCLE(2)
#define NFC_CMD_ADDR_CYCLE_ROW              NFC_CMD_ADDR_CYCLE(4)
#define NFC_CMD_ADDR_CYCLE_COL_ROW          NFC_CMD_ADDR_CYCLE(6)
#define NFC_CMD_ADDR_CYCLE_COL_OTHER        NFC_CMD_ADDR_CYCLE(7)
#define NFC_CMD_END_WAIT_READY              (0x1 << 19)
#define NFC_CMD_CYCLE(n)                    ((0x3 & (n)) << 16)
#define NFC_CMD_CYCLE_NONE                  NFC_CMD_CYCLE(0)
#define NFC_CMD_CYCLE_ONE                   NFC_CMD_CYCLE(1)
#define NFC_CMD_CYCLE_TWO                   NFC_CMD_CYCLE(2)
#define NFC_CMD_CODE1(n)                    ((0xff & (n)) << 8)
#define NFC_CMD_CODE0(n)                    (0xff & (n))

/* REG_NFC_INTCTR */
#define NFC_INTCTR_ENABLE                               (1 << 31)
#define NFC_INTCTR_FRUN_COMPLETE                        (1 << 21)
#define NFC_INTCTR_ERR_TIMEOUT                          (0xf7f << 8)
#define NFC_INTCTR_CMD_COMPLETE                         (1 << 0)

/* REG_NFC_INTSTS */
#define NFC_INTSTS_CMD_COMPLETE               (0x1 << 0)
#define NFC_INTSTS_DATA_XFER_TO_SRAM_COMPLETE (0x1 << 1)
#define NFC_INTSTS_SYSMEM_XFER_COMPLETE       (0x1 << 2)
#define NFC_INTSTS_ECC_DEC_COMPLETE           (0x1 << 6)
#define NFC_INTSTS_DMA_TO_DRAM_COMPLETE_ONCE  (0x1 << 7)
#define NFC_INTSTS_CMD_WAIT_READY_TIMEOUT     (0x1 << 8)
#define NFC_INTSTS_CMD_WAIT_BUSY_TIMEOUT      (0x1 << 9)
#define NFC_INTSTS_PIO_READ_ERR               (0x1 << 10)       /* unused */
#define NFC_INTSTS_PIO_WRITE_ERR              (0x1 << 11)       /* unused */
#define NFC_INTSTS_XFER_WAIT_READY_TIMEOUT    (0x1 << 12)
#define NFC_INTSTS_XFER_WAIT_BUSY_TIMEOUT     (0x1 << 13)
#define NFC_INTSTS_XFER_WAIT_READY2_TIMEOUT   (0x1 << 14)
#define NFC_INTSTS_SRAM_DMA_BUF_OK            (0x1 << 15)
#define NFC_INTSTS_DCR_WRITE_ERR              (0x1 << 16)       /* unused */
#define NFC_INTSTS_CMD_REG_WRITE_ERR          (0x1 << 17)
#define NFC_INTSTS_XFER_REG_WRITE_ERR         (0x1 << 18)
#define NFC_INTSTS_DMA_XFER_REG_WRITE_ERR     (0x1 << 19)
#define NFC_INTSTS_TIMEOUT_UNCORRECTABLE      (0x1 << 20)
#define NFC_INTSTS_FULL_PAGE_DMA_TERMINATED   (0x1 << 21)
#define NFC_INTSTS_HAS_BLANK_SUBPAGE          (0x1 << 22)
#define NFC_INTSTS_EMULATOR_TIMEOUT           (0x1 << 23)
#define NFC_INTSTS_READY_REQ                  (0x1 << 24)
#define NFC_INTSTS_ERRORS (\
			   NFC_INTSTS_PIO_READ_ERR | \
			   NFC_INTSTS_PIO_WRITE_ERR | \
			   NFC_INTSTS_DCR_WRITE_ERR | \
			   NFC_INTSTS_CMD_REG_WRITE_ERR | \
			   NFC_INTSTS_XFER_REG_WRITE_ERR | \
			   NFC_INTSTS_DMA_XFER_REG_WRITE_ERR)
#define NFC_INTSTS_TIMEOUTS (\
			     NFC_INTSTS_CMD_WAIT_READY_TIMEOUT | \
			     NFC_INTSTS_CMD_WAIT_BUSY_TIMEOUT | \
			     NFC_INTSTS_XFER_WAIT_READY_TIMEOUT | \
			     NFC_INTSTS_XFER_WAIT_BUSY_TIMEOUT | \
			     NFC_INTSTS_XFER_WAIT_READY2_TIMEOUT | \
			     NFC_INTSTS_EMULATOR_TIMEOUT)

/* REG_NFC_XFER */
#define NFC_XFER_KEEP_CE                         (0x1 << 31)
#define NFC_XFER_CHIP_SEL(n)                     ((0x7 & (n)) << 28)
#define NFC_XFER_RAND_DATA_ACC_CMD               (0x1 << 25)
#define NFC_XFER_RAND_DATA_ACC_CMD_CYCLE(n)      ((0x3 & (n)) << 24)
#define NFC_XFER_RAND_DATA_ACC_CMD_CYCLE_NONE \
	NFC_XFER_RAND_DATA_ACC_CMD_CYCLE(0)
#define NFC_XFER_RAND_DATA_ACC_CMD_CYCLE_ONE \
	NFC_XFER_RAND_DATA_ACC_CMD_CYCLE(2)
#define NFC_XFER_RAND_DATA_ACC_CMD_CYCLE_TWO \
	NFC_XFER_RAND_DATA_ACC_CMD_CYCLE(3)
#define NFC_XFER_XTRA_DATA_COUNT(n)              ((0x3 & (n)) << 22)
#define NFC_XFER_XTRA_DATA_COUNT_NONE            NFC_XFER_XTRA_DATA_COUNT(0)
#define NFC_XFER_XTRA_DATA_COUNT_8_LOW           NFC_XFER_XTRA_DATA_COUNT(1)
#define NFC_XFER_XTRA_DATA_COUNT_8_HIGH          NFC_XFER_XTRA_DATA_COUNT(2)
#define NFC_XFER_XTRA_DATA_COUNT_16              NFC_XFER_XTRA_DATA_COUNT(3)
#define NFC_XFER_END_WAIT_READY                  (0x1 << 21)
#define NFC_XFER_TRANSFER_BYTE_COUNT(n)          ((0x1fff & ((n) - 1)) << 8)
#define NFC_XFER_ECC_ENABLE                      (0x1 << 7)
#define NFC_XFER_ECC_RESET                       (0x1 << 6)
#define NFC_XFER_DATA_SEL(n)                     ((0x3 & (n)) << 4)
#define NFC_XFER_DATA_SEL_DATA_PORT              NFC_XFER_DATA_SEL(0)
#define NFC_XFER_DATA_SEL_DMA                    NFC_XFER_DATA_SEL(1)
#define NFC_XFER_DATA_SEL_XTRA                   NFC_XFER_DATA_SEL(2)
#define NFC_XFER_DATA_SEL_XTRA_ECC_CMB           NFC_XFER_DATA_SEL(3)
#define NFC_XFER_START_WAIT_RDY                  (0x1 << 2)
#define NFC_XFER_WRITE                           (0x1 << 1)

/* REG_NFC_STT */
#define NFC_STS_NAND_BUSY                       (0x1 << 31)
#define NFC_STS_RS_ECC_ERR(n)                  ((0x3 & (n)) << 20)
#define NFC_STS_RS_ECC_ERR_MASK                NFC_STS_RS_ECC_ERR(-1)
#define NFC_STS_RS_ECC_ERR_NONE                NFC_STS_RS_ECC_ERR(0)
#define NFC_STS_RS_ECC_ERR_CORRECTABLE         NFC_STS_RS_ECC_ERR(1)
#define NFC_STS_RS_ECC_ERR_NONCORRECTABLE      NFC_STS_RS_ECC_ERR(2)
#define NFC_STS_DMA_ACTIVE                     (1 << 5)
#define NFC_STS_CMD_ACTIVE                     (1 << 4)
#define NFC_STS_XFER_ACTIVE                    (1 << 3)
#define NFC_STS_DCTXFER_ACTIVE                 (1 << 2)
#define NFC_STS_ACTIVE                         (NFC_STS_DMA_ACTIVE | \
						NFC_STS_CMD_ACTIVE | \
						NFC_STS_XFER_ACTIVE | \
						NFC_STS_DCTXFER_ACTIVE)

/* REG_NFC_DMACTR */
#define NFC_DMACTR_WRITE_TO_DRAM                (0x1 << 16)
#define NFC_DMACTR_TRAN_BYTE(n)                 (0xffff & (n))

/* REG_NFC_RNDACC */
#define NFC_RNDACC_CODE1(n)               ((0xff & (n)) << 24)
#define NFC_RNDACC_CODE0(n)               ((0xff & (n)) << 16)
#define NFC_RNDACC_COL_ADDR(n)            (0xffff & (n))

/* REG_NFC_XD_CFG */
#define NFC_AHB_MASTER_MSK                      0x3000
#define NFC_AHB_MASTER_SINGLE                   0x0000
#define NFC_AHB_MASTER_4BEAT                    0x1000
#define NFC_AHB_MASTER_8_4BEAT                  0x2000
#define NFC_AHB_MASTER_16_8_4BEAT               0x3000

/* REG_NFC_SYSCTRL */
#define NFC_SYSCTRL_INIT_DONE                  (0x1 << 5)
#define NFC_SYSCTRL_PAGE_SIZE(n)               (((0x3 & (n)) << 24))
#define NFC_SYSCTRL_PAGE_2048                  NFC_SYSCTRL_PAGE_SIZE(0)
#define NFC_SYSCTRL_PAGE_4096                  NFC_SYSCTRL_PAGE_SIZE(1)
#define NFC_SYSCTRL_PAGE_8192                  NFC_SYSCTRL_PAGE_SIZE(2)
#define NFC_SYSCTRL_EXTRA_SIZE(n)              ((0x3f & (n)) << 8)
#define NFC_SYSCTRL_ECC_BCH                    (0x1 << 26)
#define NFC_SYSCTRL_SUBPAGE_SIZE(n)            (((0x1 & (n)) << 27))
#define NFC_SYSCTRL_SUBPAGE_512                NFC_SYSCTRL_SUBPAGE_SIZE(0)
#define NFC_SYSCTRL_SUBPAGE_1024               NFC_SYSCTRL_SUBPAGE_SIZE(1)
#define NFC_SYSCTRL_BLK_SIZE(n)                (((0x3 & (n))) << 28)
#define NFC_SYSCTRL_BLK_128K                   NFC_SYSCTRL_BLK_SIZE(0)
#define NFC_SYSCTRL_BLK_256K                   NFC_SYSCTRL_BLK_SIZE(1)
#define NFC_SYSCTRL_BLK_512K                   NFC_SYSCTRL_BLK_SIZE(2)
#define NFC_SYSCTRL_BLK_1M                     NFC_SYSCTRL_BLK_SIZE(3)
#define NFC_SYSCTRL_ONE_NAND                    (0x1 << 30)

/* REG_NFC_SYSCTRL1 */
#define NFC_SYSCTRL1_NEW_VERSION                  (0x1 << 0)
#define NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA           (0x1 << 2)
#define NFC_SYSCTRL1_MIPS_MUST_SET_TO_0           (0x1 << 3)
#define NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA_WRITE     (0x1 << 5)
#define NFC_SYSCTRL1_NO_REMOVE_EXTRA_2DUMMY_BYTE  (0x1 << 6)
#define NFC_SYSCTRL1_DECODE_FROM_BUFFER           (0x1 << 10)
#define NFC_SYSCTRL1_LATCH_READ_DELAY_1T          (0x1 << 11)
#define NFC_SYSCTRL1_LATCH_READ_DELAY_2T          (0x1 << 12)

/* REG_NFC_SYSCTRL2 */
#define NFC_SYSCTRL2_BLANK_CHECK_THRESHOLD_MASK   (0x3f << 0)
#define NFC_SYSCTRL2_BLANK_CHECK_THRESHOLD(n)     (((n) & 0x3f) << 0)
#define NFC_SYSCTRL2_BLANK_CHECK_EN               (0x01 << 6)
#define NFC_SYSCTRL2_AUTO_CORRECT_DISABLE         (0x01 << 7)
#define NFC_SYSCTRL2_XTRA_PTR_PARK                (0x01 << 10)
#define NFC_SYSCTRL2_DISABLE_FULLPAGE_DMA_WRITE_ECC (0x01 << 11)

#define NFC_SYSCTRL2_NEW_BLANK_CHECK_THRESHOLD_MASK             (0x3ff << 0)
#define NFC_SYSCTRL2_NEW_BLANK_CHECK_THRESHOLD(n)               (((n) & 0x3ff))
#define NFC_SYSCTRL2_NEW_BLANK_CHECK_EN                         (0x01 << 10)
#define NFC_SYSCTRL2_NEW_AUTO_CORRECT_DISABLE                   (0x01 << 11)
#define NFC_SYSCTRL2_NEW_XTRA_PTR_PARK                          (0x01 << 13)
#define NFC_SYSCTRL2_NEW_DISABLE_FULLPAGE_DMA_WRITE_ECC         (0x01 << 14)

/* REG_NFC_SFTRST */
#define NFC_SFTRST_CONTROLLER                     (0x1 << 0)
#define NFC_SFTRST_DMA                            (0x1 << 1)
#define NFC_SFTRST_BUFFER                         (0x1 << 2)

/* REG_NFC_FTUNE */
#define NFC_FTUNE_RESEL_ORIG    (0x0 << 6)
#define NFC_FTUNE_RESEL_DEC_HALF        (0x1 << 6)
#define NFC_FTUNE_RESEL_EXTEND_D05      (0x2 << 6)
#define NFC_FTUNE_RESEL_EXTEND_HALF     (0x3 << 6)

#define NFC_FTUNE_WESEL_ORIG    (0x0 << 14)
#define NFC_FTUNE_WESEL_DEC_HALF        (0x1 << 14)
#define NFC_FTUNE_WESEL_EXTEND_D05      (0x2 << 14)
#define NFC_FTUNE_WESEL_EXTEND_HALF     (0x3 << 14)

/* REG_NFC_ERR_CNT1~2 */
#define NFC_ERR_CNTX_IS_BLANK(n)          (0x1 << (20 + (n)))

#endif /* _NT72_NAND_REGS_H_ */
