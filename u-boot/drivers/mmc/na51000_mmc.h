/*
 *  driver/mmc/nt96660_mmc.h
 *
 *  Author:	Howard Chang
 *  Created:	April 8, 2016
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef __NA51000_MMC_H__
#define __NA51000_MMC_H__

#include "na51000_mmcreg.h"

int nvt_mmc_init(int id);
#define CLK_26M	(26000000)
#define CLK_52M (52000000)
#define CLK_200M (200000000)
#define CLK_100M (100000000)

#define BIT(nr)			(1UL << (nr))
#define PAD_DS_REG_OFS              (IOADDR_PAD_REG_BASE+0x40)

#define PAD_DS_CGPIO_BASE      0
#define PAD_DS_GROUP5_40        0x8000
#define PAD_DS_CGPIO16         ((PAD_DS_CGPIO_BASE + 16) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO17         ((PAD_DS_CGPIO_BASE + 17) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO18         ((PAD_DS_CGPIO_BASE + 18) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO19         ((PAD_DS_CGPIO_BASE + 19) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO20         ((PAD_DS_CGPIO_BASE + 20) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO21         ((PAD_DS_CGPIO_BASE + 21) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO22         ((PAD_DS_CGPIO_BASE + 22) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO23         ((PAD_DS_CGPIO_BASE + 23) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO24         ((PAD_DS_CGPIO_BASE + 24) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO25         ((PAD_DS_CGPIO_BASE + 25) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO26         ((PAD_DS_CGPIO_BASE + 26) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO27         ((PAD_DS_CGPIO_BASE + 27) | PAD_DS_GROUP5_40)

#define PAD_DRIVINGSINK_2P5MA    0x01
#define PAD_DRIVINGSINK_5MA      0x02
#define PAD_DRIVINGSINK_7P5MA    0x04
#define PAD_DRIVINGSINK_10MA     0x08
#define PAD_DRIVINGSINK_12P5MA   0x10
#define PAD_DRIVINGSINK_15MA     0x20
#define PAD_DRIVINGSINK_17P5MA   0x40
#define PAD_DRIVINGSINK_20MA     0x80
#define PAD_DRIVINGSINK_25MA    0x100
#define PAD_DRIVINGSINK_30MA    0x200
#define PAD_DRIVINGSINK_35MA    0x400
#define PAD_DRIVINGSINK_40MA    0x800

#define SDIO_HOST_WRITE_DATA                (FALSE)
#define SDIO_HOST_READ_DATA                 (TRUE)

struct mmc_nvt_host {
	struct mmc_cmd *cmd;
	struct mmc_data *data;
	unsigned int mmc_input_clk;
	void __iomem *base;
	struct resource *mem_res;
	int id;
	unsigned char bus_mode;
	unsigned char data_dir;
	unsigned char suspended;

	/* buffer is used during PIO of one scatterlist segment, and
	 * is updated along with buffer_bytes_left.  bytes_left applies
	 * to all N blocks of the PIO transfer.
	 */
	u8 *buffer;
	u32 buffer_bytes_left;
	u32 bytes_left;
	/*early data*/
	bool data_early;

	/* Version of the MMC/SD controller */
	u8 version;
	/* for ns in one cycle calculation */
	unsigned ns_in_one_cycle;
};


/*
    SDIO send command execution result.

    Encoding of sdiohost_sendcmd() result.
*/

#define SDIO_HOST_CMD_OK                    (0)     /* command execution OK*/
#define SDIO_HOST_RSP_TIMEOUT               (-1)    /* response timeout*/
#define SDIO_HOST_RSP_CRCFAIL               (-2)    /* response CRC fail*/
#define SDIO_HOST_CMD_FAIL                  (-3)    /* command fail*/


/*
    SDIO data transfer result.

    Encoding of sdiohost_waitdataend() result.
*/

#define SDIO_HOST_DATA_OK                   (0)     /* data transfer OK*/
#define SDIO_HOST_DATA_TIMEOUT              (-1)    /* data block timeout*/
#define SDIO_HOST_DATA_CRCFAIL              (-2)    /* data block CRC fail*/
#define SDIO_HOST_DATA_FAIL                 (-3)    /* data transfer fail*/


#define SDIO_HOST_BOOT_ACK_OK               (0)
#define SDIO_HOST_BOOT_ACK_TIMEOUT          (-1)
#define SDIO_HOST_BOOT_ACK_ERROR            (-2)
#define SDIO_HOST_BOOT_END                  (0)
#define SDIO_HOST_BOOT_END_ERROR            (-1)

/*
    SDIO response type

    @note for sdiohost_sendcmd()
*/
typedef enum {
	SDIO_HOST_RSP_NONE,         /* No response*/
	SDIO_HOST_RSP_SHORT,        /* Short response*/
	SDIO_HOST_RSP_LONG,         /* Long response*/
	SDIO_HOST_RSP_SHORT_TYPE2,  /* Short response timeout is 5 bus clock*/
	SDIO_HOST_RSP_LONG_TYPE2,   /* Long response timeout is 5 bus clock*/
	SDIO_HOST_RSP_VOLT_DETECT,  /* voltage detect response*/
	ENUM_DUMMY4WORD(SDIO_HOST_RESPONSE)
} SDIO_HOST_RESPONSE;

#define SDIO_HOST_MAX_VOLT_TIMER (0xFFF) /* max value of voltage switch timer*/

#define SDIO_DES_TABLE_NUM        (128)
#define SDIO_DES_WORD_SIZE        (3)   /*descriptor 3 word*/
#define SDIO_HOST_MAX_DATA_LENGTH (64*1024*1024)
#define SDIO_HOST_DATA_FIFO_DEPTH (16)

/*sdio_protocol.h*/
#define SDIO_HOST_WRITE_DATA                (FALSE)
#define SDIO_HOST_READ_DATA                 (TRUE)


/* Command Register Bit*/
#define SDIO_CMD_REG_INDEX                  0x0000003F  /* bit 5..0*/
#define SDIO_CMD_REG_NEED_RSP               0x00000040  /* bit 6*/
#define SDIO_CMD_REG_LONG_RSP               0x000000C0  /* bit 7*/
#define SDIO_CMD_REG_RSP_TYPE2              0x00000100  /* bit 8*/
#define SDIO_CMD_REG_APP_CMD                0x00000000  /* bit x*/
#define SDIO_CMD_REG_ABORT                  0x00000800  /* bit 11*/
#define SDIO_CMD_REG_VOLTAGE_SWITCH_DETECT  0x00001000  /* bit 12*/


/* Status/Interrupt Mask Register Bit*/
#define SDIO_STATUS_REG_RSP_CRC_FAIL        0x00000001  /* bit 0*/
#define SDIO_STATUS_REG_DATA_CRC_FAIL       0x00000002  /* bit 1*/
#define SDIO_STATUS_REG_RSP_TIMEOUT         0x00000004  /* bit 2*/
#define SDIO_STATUS_REG_DATA_TIMEOUT        0x00000008  /* bit 3*/
#define SDIO_STATUS_REG_RSP_CRC_OK          0x00000010  /* bit 4*/
#define SDIO_STATUS_REG_DATA_CRC_OK         0x00000020  /* bit 5*/
#define SDIO_STATUS_REG_CMD_SEND            0x00000040  /* bit 6*/
#define SDIO_STATUS_REG_DATA_END            0x00000080  /* bit 7*/
#define SDIO_STATUS_REG_INT                 0x00000100  /* bit 8*/
#define SDIO_STATUS_REG_READWAIT            0x00000200  /* bit 9*/
#define SDIO_STATUS_REG_EMMC_BOOTACKREV     0x00008000  /* bit 15*/
#define SDIO_STATUS_REG_EMMC_BOOTACKTOUT    0x00010000  /* bit 16*/
#define SDIO_STATUS_REG_EMMC_BOOTEND        0x00020000  /* bit 17*/
#define SDIO_STATUS_REG_EMMC_BOOTACKERR     0x00040000  /* bit 18*/
#define SDIO_STATUS_REG_DMA_ERR             0x00080000  /* bit 19*/
#define SDIO_INTMASK_ALL                    0x000003FF  /* bit 9..0*/

/* Bus Width Register bit definition*/
#define SDIO_BUS_WIDTH1                     0x0  /* bit 1..0*/
#define SDIO_BUS_WIDTH4                     0x1  /* bit 1..0*/
#define SDIO_BUS_WIDTH8                     0x2  /* bit 1..0*/

#define   MMCST_RSP_CRC_FAIL                  BIT(0)
#define   MMCST_DATA_CRC_FAIL                 BIT(1)
#define   MMCST_RSP_TIMEOUT                   BIT(2)
#define   MMCST_DATA_TIMEOUT                  BIT(3)
#define   MMCST_RSP_CRC_OK                    BIT(4)
#define   MMCST_DATA_CRC_OK                   BIT(5)
#define   MMCST_CMD_SENT                      BIT(6)
#define   MMCST_DATA_END                      BIT(7)
#define   MMCST_SDIO_INT                      BIT(8)
#define   MMCST_READ_WAIT                     BIT(9)
#define   MMCST_CARD_BUSY2READY               BIT(10)
#define   MMCST_VOL_SWITCH_END                BIT(11)
#define   MMCST_VOL_SWITCH_TIMEOUT            BIT(12)
#define   MMCST_RSP_VOL_SWITCH_FAIL           BIT(13)
#define   MMCST_VOL_SWITCH_GLITCH             BIT(14)
#define   MMCST_EMMC_BOOT_ACK_RECEIVE         BIT(15)
#define   MMCST_EMMC_BOOT_ACK_TIMEOUT         BIT(16)
#define   MMCST_EMMC_BOOT_END                 BIT(17)
#define   MMCST_EMMC_BOOT_ACK_ERROR           BIT(18)
#define   MMCST_DMA_ERROR                     BIT(19)

#define SDIO_HOST_ID_1 0
#define SDIO_HOST_ID_2 1
#define SDIO_HOST_ID_3 2

typedef enum {
	SDIO_MODE_DS = 25000000,
	SDIO_MODE_HS = 50000000,
	SDIO_MODE_SDR50 = 100000000,
	SDIO_MODE_SDR104 = 208000000,
	ENUM_DUMMY4WORD(SDIO_SPEED_MODE)
} SDIO_SPEED_MODE;

#define SDIO_DLY_PHASE_UNIT_SFT	4
#define SDIO_DLY_CMD_PHASE_SFT	0
#define SDIO_DLY_DATA_SFT	20
#define SDIO_DLY_SAMPEDGE_DATA_SFT	12
#define SDIO_DLY_SAMPEDGE_DATA_POS	(0<<SDIO_DLY_SAMPEDGE_DATA_SFT)
#define SDIO_DLY_SAMPEDGE_DATA_NEG	(1<<SDIO_DLY_SAMPEDGE_DATA_SFT)
#define SDIO_DLY_CLK_SRC	13
#define SDIO_DLY_DET_SEL	8
#define SDIO_DLY_DET_AUTO	19

#define SDIO_DLY_DS_DEFAULT             (SDIO_DLY_SAMPEDGE_DATA_POS) + \
					(0<<SDIO_DLY_PHASE_UNIT_SFT) + \
					(0<<SDIO_DLY_CMD_PHASE_SFT) + \
					(0<<SDIO_DLY_DATA_SFT) + \
					(1<<SDIO_DLY_CLK_SRC) + \
					(1<<SDIO_DLY_DET_SEL) + \
					(1<<SDIO_DLY_DET_AUTO)
#define SDIO_DLY_HS_DEFAULT             (SDIO_DLY_SAMPEDGE_DATA_POS) + \
					(0<<SDIO_DLY_PHASE_UNIT_SFT) + \
					(0<<SDIO_DLY_CMD_PHASE_SFT) + \
					(0<<SDIO_DLY_DATA_SFT) + \
					(1<<SDIO_DLY_CLK_SRC) + \
					(1<<SDIO_DLY_DET_SEL) + \
					(1<<SDIO_DLY_DET_AUTO)
#define SDIO_DLY_SDR50_DEFAULT      	(SDIO_DLY_SAMPEDGE_DATA_POS) + \
					(2<<SDIO_DLY_PHASE_UNIT_SFT) + \
					(6<<SDIO_DLY_CMD_PHASE_SFT) + \
					(0<<SDIO_DLY_DATA_SFT) + \
					(1<<SDIO_DLY_CLK_SRC) + \
					(1<<SDIO_DLY_DET_SEL) + \
					(1<<SDIO_DLY_DET_AUTO)
#define SDIO_DLY_SDR104_DEFAULT		(SDIO_DLY_SAMPEDGE_DATA_POS) + \
					(2<<SDIO_DLY_PHASE_UNIT_SFT) + \
					(8<<SDIO_DLY_CMD_PHASE_SFT) + \
					(0<<SDIO_DLY_DATA_SFT) + \
					(1<<SDIO_DLY_CLK_SRC) + \
					(1<<SDIO_DLY_DET_SEL) + \
					(1<<SDIO_DLY_DET_AUTO)

#endif
