#ifndef __NAND_H__
#define __NAND_H__
#ifdef __cplusplus
    extern "C" {
#endif  // __cplusplus

// =================================================================================================
#define REG_STBC_BOOTSTRAP              (*(volatile unsigned long *)(0xFC04023C))
// =================================================================================================
#define NVT_REG_STBC_BASE               (0xFC040000)
#define NVT_REG_PHY_KEY_CTRL			(0x204) 
#define NVT_REG_PHY_KEY_ENABLE			(0x208)

#define NVT_STBC_CTRL_KEY1				(0x72682)
#define NVT_STBC_CTRL_KEY2				(0x28627)
#define NVT_STBC_ENABLE                 (1)

#define NVT_REG_STBC_GPF_G1             (0x824)
#define NVT_REG_STBC_GPF_G2             (0x828)
#define NVT_REG_STBC_GPF_G3             (0x82C)

#define NVT_REG_STBC_GPF_NAND_D0        (1 << 20)
#define NVT_REG_STBC_GPF_NAND_D1        (1 << 28)
#define NVT_REG_STBC_GPF1_NAND_D2       (1 << 4)
#define NVT_REG_STBC_GPF1_NAND_D3       (1 << 12)
#define NVT_REG_STBC_GPF1_NAND_D4       (1 << 20)
#define NVT_REG_STBC_GPF1_NAND_D5       (1 << 28)
#define NVT_REG_STBC_GPF2_NAND_D6       (1 << 4)
#define NVT_REG_STBC_GPF2_NAND_D7       (1 << 12)

#define NVT_REG_PHY_NFC_ARBITOR_CTRL	(0x200)
#define NVT_NFC_STBC_ENABLE				(1<<4)
#define NVT_NFC_MAINCHIP_ENABLE			(1<<5)

#define NVT_REG_PHY_SECURE_LOCK_CTRL    (0xFD650000)
#define NVT_EMMC_SECURE_LOCK_ENABLE     (1<<4)

#define NVT_REG_MAU_W64BIT_MASTER0 		(0xFD010258)
#define NVT_MAU_W64BIT_MASTER0			(0xFFFFFFFF)

#define NVT_REG_MAU_R64BIT_MASTER0		(0xFD010260)
#define NVT_MAU_R64BIT_MASTER0 			(0xFFFFFFFF)

#define NVT_REG_MAU_ARBITER 			(0xFD150008)
#define NVT_MAU_ARBITER 				(0x00FFFF00)

// =================================================================================================
#define REG_NFC_BASE                    (0xFC048000) //For 72656
// -------------------------------------------------------------------------------------------------
#define REG_NFC_CFG0                    (0x00)
#define REG_NFC_CFG1                    (0x04)
#define REG_NFC_CMD                     (0x08)
#define REG_NFC_TRAN_MODE               (0x0c)
#define REG_NFC_COL_ADDR                (0x10)
#define REG_NFC_ROW_ADDR                (0x14)
#define REG_NFC_SW_RESET                (0x18)
#define REG_NFC_RAND_ACC_CMD            (0x1c)
#define REG_NFC_INT_ENABLE              (0x20)
#define REG_NFC_INT_STAT                (0x24)
#define REG_NFC_DATA_PORT               (0x28)

#define REG_NFC_DIRCTRL                 (0x40)
#define REG_NFC_STAT                    (0x44)
#define REG_NFC_DMA_ADDR                (0x50)
#define REG_NFC_DMA_CTRL                (0x54)
#define REG_NFC_SYSCTRL                 (0x5c)

#define REG_NFC_RS_ECC(n)               (0x60 + 4 * ( 3 & (n)))
#define REG_NFC_RS_ECC0                 REG_NFC_RS_ECC(0)
#define REG_NFC_RS_ECC1                 REG_NFC_RS_ECC(1)
#define REG_NFC_RS_ECC2                 REG_NFC_RS_ECC(2)
#define REG_NFC_RS_ECC3                 REG_NFC_RS_ECC(3)

#define REG_NFC_BCH_ERR_SET1(n)         (0x60 + 4 * (3 & (n)))
#define REG_NFC_BCH_ERR_SET2(n)         (0xdc + 4 * (7 & (n)))

#define REG_NFC_BCH_ERR_1_2             (0x60)
#define REG_NFC_BCH_ERR_3_4             (0x64)
#define REG_NFC_BCH_ERR_5_6             (0x68)
#define REG_NFC_BCH_ERR_7_8             (0x6c)
#define REG_NFC_BCH_ERR_9_10            (0xdc)
#define REG_NFC_BCH_ERR_11_12           (0xe0)
#define REG_NFC_BCH_ERR_13_14           (0xe4)
#define REG_NFC_BCH_ERR_15_16           (0xe8)
#define REG_NFC_BCH_ERR_17_18           (0xec)
#define REG_NFC_BCH_ERR_19_20           (0xf0)
#define REG_NFC_BCH_ERR_21_22           (0xf4)
#define REG_NFC_BCH_ERR_23_24           (0xf8)

#define REG_NFC_XTRA_DATA0              (0x30)
#define REG_NFC_XTRA_DATA1              (0x34)
#define REG_NFC_XTRA_DATA2              (0x38)
#define REG_NFC_XTRA_DATA3              (0x3c)
#define REG_NFC_XTRA_DATA4              (0x7c)
#define REG_NFC_XTRA_DATA5              (0x80)
#define REG_NFC_XTRA_DATA6              (0x84)
#define REG_NFC_XTRA_DATA7              (0x88)
#define REG_NFC_XTRA_DATA8              (0x8c)
#define REG_NFC_XTRA_DATA9              (0x90)
#define REG_NFC_XTRA_DATA10             (0x94)
#define REG_NFC_XTRA_DATA11             (0x98)
#define REG_NFC_XTRA_DATA12             (0x9c)
#define REG_NFC_XTRA_DATA13             (0xa0)
#define REG_NFC_XTRA_DATA14             (0xa4)
#define REG_NFC_XTRA_DATA15             (0xa8)
#define REG_NFC_XTRA_DATA16             (0xac)
#define REG_NFC_XTRA_DATA17             (0xb0)
#define REG_NFC_XTRA_DATA18             (0xb4)
#define REG_NFC_XTRA_DATA19             (0xb8)
#define REG_NFC_XTRA_DATA20             (0xbc)
#define REG_NFC_XTRA_DATA21             (0xc0)
#define REG_NFC_XTRA_DATA22             (0xc4)
#define REG_NFC_XTRA_DATA23             (0xc8)
#define REG_NFC_XTRA_DATA24             (0xcc)
#define REG_NFC_XTRA_DATA25             (0xd0)
#define REG_NFC_XTRA_DATA26             (0xd4)
#define REG_NFC_XTRA_DATA27             (0xd8)

#define REG_NFC_AHB_BURST_SIZE          (0x78)

#define REG_NFC_XTRA_ADDR               (0x104)
#define REG_NFC_SYSCTRL1                (0x10C)
#define REG_NFC_Fine_Tune               (0x110)
#define NFC_ERR_CNT0                    (0x114)
#define NFC_ERR_CNT1                    (0x118)
#define REG_NFC_SYSCTRL2                (0x11C)


// -------------------------------------------------------------------------------------------------
// REG_NFC_CFG0
#define NFC_CFG0_TWH(n)                 ((0xf & (n)) << 28)
#define NFC_CFG0_TWL(n)                 ((0xf & (n)) << 24)
#define NFC_CFG0_TRH(n)                 ((0xf & (n)) << 20)
#define NFC_CFG0_TRL(n)                 ((0xf & (n)) << 16)
#define NFC_CFG0_TWW(n)                 ((0xf & (n)) << 12)
#define NFC_CFG0_TAR(n)                 ((0xf & (n)) << 8)
#define NFC_CFG0_TCLS_TCS_TALS_TALH(n)  ((0xf & (n)) << 4)
#define NFC_CFG0_DATAWIDTH_16           (1 << 2)

#define NFC_CFG0_ROW_ADDR_MASK          (1 << 1)
#define NFC_CFG0_COL_ADDR_MASK          (1 << 0)
#define NFC_CFG0_ROW_ADDR_3CYCLES       (1 << 1)
#define NFC_CFG0_ROW_ADDR_2CYCLES       (0 << 1)
#define NFC_CFG0_COL_ADDR_2CYCLES       (1 << 0)
#define NFC_CFG0_COL_ADDR_1CYCLES       (0 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_CFG1
#define NFC_CFG1_READY_TO_BUSY_TIMEOUT(n)   ((0xf & (n)) << 24)
#define NFC_CFG1_ECC_ENCODE_RESET           (1 << 23)
#define NFC_CFG1_ECC_DECODE_RESET           (1 << 22)
#define NFC_CFG1_BCH_ECC_ENABLE             (1 << 21)
#define NFC_CFG1_LITTLE_ENDIAN_XTRA         (1 << 19)
#define NFC_CFG1_LITTLE_ENDIAN              (1 << 18)
#define NFC_CFG1_BCH_ENABLE_PL              (1 << 17)
#define NFC_CFG1_RS_ECC_ENABLE              (1 << 16)
#define NFC_CFG1_BUSY_TO_READY_TIMEOUT(n)   (0xffff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_CMD
#define NFC_CMD_CE_IDX(n)                   ((7 & (n)) << 28)
#define NFC_CMD_WP_DISABLE                  (1 << 27)
#define NFC_CMD_WP_NEG                      (1 << 27)
#define NFC_CMD_WP_POS                      (0 << 27)
#define NFC_CMD_WP_KEEP                     (1 << 26)
#define NFC_CMD_FREERUN                     (1 << 24)
#define NFC_CMD_DATA_PRESENT                (1 << 23)
#define NFC_CMD_ADDR_CYCLE(n)               ((7 & (n)) << 20)
#define NFC_CMD_ADDR_CYCLE_NONE             NFC_CMD_ADDR_CYCLE(0)
#define NFC_CMD_ADDR_CYCLE_DUMMY            NFC_CMD_ADDR_CYCLE(1)
#define NFC_CMD_ADDR_CYCLE_COL              NFC_CMD_ADDR_CYCLE(2)
#define NFC_CMD_ADDR_CYCLE_ROW              NFC_CMD_ADDR_CYCLE(4)
#define NFC_CMD_ADDR_CYCLE_COL_ROW          NFC_CMD_ADDR_CYCLE(6)
#define NFC_CMD_ADDR_CYCLE_COL_OTHER        NFC_CMD_ADDR_CYCLE(7)
#define NFC_CMD_END_WAIT_BUSY_TO_RDY        (1 << 19)
#define NFC_CMD_CYCLE(n)                    ((3 & (n)) << 16)
#define NFC_CMD_CYCLE_NONE                  NFC_CMD_CYCLE(0)
#define NFC_CMD_CYCLE_ONE                   NFC_CMD_CYCLE(1)
#define NFC_CMD_CYCLE_TWO                   NFC_CMD_CYCLE(2)
#define NFC_CMD_CODE1(n)                    ((0xff & (n)) << 8)
#define NFC_CMD_CODE0(n)                    (0xff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_TRAN_MODE
#define NFC_TRAN_MODE_KEEP_CE               (1 << 31)
#define NFC_TRAN_MODE_CE_IDX(n)             ((7 & (n)) << 28)

#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(n)     ((3 & (n)) << 24)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_NONE   NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(0)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE    NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(2)
#define NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO    NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE(3)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT(n)        ((3 & (n)) << 22)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_NONE      NFC_TRAN_MODE_XTRA_DATA_COUNT(0)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_8_LOW     NFC_TRAN_MODE_XTRA_DATA_COUNT(1)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_8_HIGH    NFC_TRAN_MODE_XTRA_DATA_COUNT(2)
#define NFC_TRAN_MODE_XTRA_DATA_COUNT_16        NFC_TRAN_MODE_XTRA_DATA_COUNT(3)
#define NFC_TRAN_MODE_END_WAIT_READY            (1 << 21)

#define NFC_TRAN_MODE_BLK_SIZE(n)               ((0x1fff & ((n) - 1)) << 8)

#define NFC_TRAN_MODE_TRANSFER_BYTE_COUNT(n)    ((0x1fff & ((n) - 1)) << 8)

#define NFC_TRAN_MODE_ECC_ENABLE                (1 << 7)
#define NFC_TRAN_MODE_ECC_RESET                 (1 << 6)
#define NFC_TRAN_MODE_DATA_SEL(n)               ((3 & (n)) << 4)
#define NFC_TRAN_MODE_DATA_SEL_DATA_PORT        NFC_TRAN_MODE_DATA_SEL(0)
#define NFC_TRAN_MODE_DATA_SEL_DMA              NFC_TRAN_MODE_DATA_SEL(1)
#define NFC_TRAN_MODE_DATA_SEL_XTRA             NFC_TRAN_MODE_DATA_SEL(2)
#define NFC_TRAN_MODE_DATA_SEL_XTRA_ECC_CMB     NFC_TRAN_MODE_DATA_SEL(3)
#define NFC_TRAN_MODE_START_WAIT_RDY            (1 << 2)
#define NFC_TRAN_MODE_WRITE                     (1 << 1)

// -------------------------------------------------------------------------------------------------
// REG_NFC_RAND_ACC_CMD
#define NFC_RAND_ACC_CMD_CODE1(n)               ((0xff & (n)) << 24)
#define NFC_RAND_ACC_CMD_CODE0(n)               ((0xff & (n)) << 16)
#define NFC_RAND_ACC_CMD_COL_ADDR(n)            (0xffff & (n))

// -------------------------------------------------------------------------------------------------
// REG_NFC_INT_ENABLE, REG_NFC_INT_STAT
/*
#if (!(NT72682))
#define NFC_INT_READY_REQUEST                   (1 << 24)
#endif
*/
#define NFC_INT_ALL                             (0xFFFFFFFF)
#define NFC_INT_ENABLE                          (1 << 31)
#define NFC_INT_FREE_RUN_COMPLETE               (1 << 21)
#define NFC_INT_FR_UNCORECTABLE                 (1 << 20)
#define NFC_INT_ERR                             ((0xf7f << 8))
#define NFC_INT_ERR_RW                          ((0xe51 << 8))
#define NFC_INT_BCH_READYSIGNAL                 (1 << 15)
#define NFC_INT_MMEMP_COMPLETE                  (1 << 7)
#define NFC_INT_ECC_DEC_COMPLETE                (1 << 6)
#define NFC_INT_PIO_READY                       (1 << 3)
#define NFC_INT_MEM_TRAN_COMPLETE               (1 << 2)
#define NFC_INT_DATA_TRAN_COMPLETE              (1 << 1)
#define NFC_INT_CMD_COMPLETE                    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_NFC_STAT
#define NFC_STAT_RB                             (1 << 31)
#define NFC_STAT_RS_ECC_ERR(n)                  ((3 & (n)) << 20)
#define NFC_STAT_RS_ECC_ERR_MASK                NFC_STAT_RS_ECC_ERR(-1)
#define NFC_STAT_RS_ECC_ERR_NONE                NFC_STAT_RS_ECC_ERR(0)
#define NFC_STAT_RS_ECC_ERR_CORRECTABLE         NFC_STAT_RS_ECC_ERR(1)
#define NFC_STAT_RS_ECC_ERR_NONCORRECTABLE      NFC_STAT_RS_ECC_ERR(2)
#define NFC_STAT_GET_RS_ECC_COUNT(n)            (((n) >> 16) & 0x7)

// -------------------------------------------------------------------------------------------------
// REG_NFC_DMA_CTRL
#define NFC_DMA_CTRL_READ                       (1 << 16)

// -------------------------------------------------------------------------------------------------
// REG_NFC_DMA_CTRL
#define NFC_DMA_CTRL_TRAN_BYTE_COUNT(n)         (0xffff & ((n) - 1))

//REG_NFC_AHB_BURST_SIZE
#define NFC_AHB_MASTER_MSK        0x3000
#define NFC_AHB_MASTER_SINGLE     0x0000
#define NFC_AHB_MASTER_4BEAT      0x1000
#define NFC_AHB_MASTER_8_4BEAT    0x2000
#define NFC_AHB_MASTER_16_8_4BEAT 0x3000

// -------------------------------------------------------------------------------------------------
// REG_NFC_SYSCTRL
#define NFC_SYS_CTRL_FREE_RUN_EN                (1<<3)
#define NFC_SYS_CTRL_SUBPAGE_SIZE(n)            (((1 & (n)) << 27))
#define NFC_SYS_CTRL_SUBPAGE_512                NFC_SYS_CTRL_SUBPAGE_SIZE(0)
#define NFC_SYS_CTRL_SUBPAGE_1024               NFC_SYS_CTRL_SUBPAGE_SIZE(1)
#define NFC_SYS_CTRL_PAGE_SIZE(n)               (((3 & (n)) << 24))
#define NFC_SYS_CTRL_PAGE_2048                  NFC_SYS_CTRL_PAGE_SIZE(0)
#define NFC_SYS_CTRL_PAGE_4096                  NFC_SYS_CTRL_PAGE_SIZE(1)
#define NFC_SYS_CTRL_PAGE_8192                  NFC_SYS_CTRL_PAGE_SIZE(2)
#define NFC_SYS_CTRL_EXTRA_SIZE(n)              ((0x3F & (n)) << 8)
#define NFC_SYS_CTRL_ECC_SEL(n)                 ((1 & (n)) << 26)
#define NFC_SYS_CTRL_ECC_RS                     NFC_SYS_CTRL_ECC_SEL(0)
#define NFC_SYS_CTRL_ECC_BCH                    NFC_SYS_CTRL_ECC_SEL(1)
#define NFC_SYS_CTRL_BLK_SIZE(n)                (((3 & (n))) << 28)
#define NFC_SYS_CTRL_BLK_128K                   NFC_SYS_CTRL_BLK_SIZE(0)
#define NFC_SYS_CTRL_BLK_256K                   NFC_SYS_CTRL_BLK_SIZE(1)
#define NFC_SYS_CTRL_BLK_512K                   NFC_SYS_CTRL_BLK_SIZE(2)
#define NFC_SYS_CTRL_BLK_1M                     NFC_SYS_CTRL_BLK_SIZE(3)
#define NFC_SYS_CTRL_CONTROLLER_SEL(n)          ((1 & (n)) << 30)
#define NFC_SYS_CTRL_16_BIT_NAND                (1 << 31)
#define NFC_SYS_CTRL_8_BIT_NAND                 (0 << 31)


// REG_NFC_SYSCTRL1
#define NFC_SYS_CTRL1_NEW_VERSION                  (0x1)
#define NFC_SYS_CTRL1_FREE_RUN                     (0x1 << 2)
#define NFC_SYS_CTRL1_FREE_RUN_WRITE               (0x1 << 5)
#define NFC_SYS_CTRL1_REMOVE_EXTRA_2DUMMY_BYTE_DIS (0x1 << 6)
#define NFC_SYS_CTRL1_BCH16_ENABLE_NEW_ARCH        (0x1 << 10)
#define NFC_SYS_CTRL1_DELAY_LATCH_1T               (0x1 << 11)
#define NFC_SYS_CTRL1_DELAY_LATCH_2T               (0x1 << 12)
#define NFC_SYS_CTRL1_DELAY_LATCH_MSK              (0x3 << 11)

// REG_NFC_SYSCTRL2
#define NFC_SYS_CTRL2_BLANK_THRESHOLD_MASK          0x3F
#define NFC_SYS_CTRL2_BLANK_ENABLE                  0x40
#define NFC_SYS_CTRL2_DISABLE_AUTOECC               (0x1 << 7)
#define NFC_SYS_CTRL2_XTRA_REG_SET1                 (0x1 << 10)
#define NFC_SYS_CTRL2_DISABLE_FREE_WRITE_ECC_ENCODE (0x1 << 11)


#ifdef __cplusplus
    }
#endif  // __cplusplus
#endif  // __NAND_H__
