/**\file
 * This file defines the function prototypes for the Synopsys GMAC device and the
 * Marvell 88E1011/88E1011S integrated 10/100/1000 Gigabit Ethernet Transceiver.
 * Since the phy register mapping are standardised, the phy register map and the
 * bit definitions remain the same for other phy as well.
 * This also defines some of the Ethernet related parmeters.
 * \internal
 * ----------------- REVISION HISTORY -----------------------
 * Synopsys			   01/Aug/2007				Created
 */

#ifndef SYNOP_GMAC_DEV_H
#define SYNOP_GMAC_DEV_H 1

#include "synopGMAC_plat.h"

#define USE_DEVICE_TREE		1

#define MACBASE			0x0000 /* Mac Base address */
#define DMABASE			0x1000 /* Dma base address */
#define MDC_CLOCK_DIV	GmiiCsrClk1 /* for ahb 48MHz */

/* Tx/Rx Descriptors needed in the Descriptor pool/queue */
#define TRANSMIT_DESC_SIZE		256
#define RECEIVE_DESC_SIZE		256

#define VLAN_TAG				4
/* Maximum length when received data is copied on to a new skb */
#define ETHERNET_PACKET_COPY	250
/* Preallocated length for the rx packets is MTU + ETHERNET_PACKET_EXTRA */
#define ETHERNET_PACKET_EXTRA	18
#define MIN_ETHERNET_PAYLOAD	46   /* Minimum Ethernet payload size*/
#define MAX_ETHERNET_PAYLOAD	1500 /* Maximum Ethernet payload size */
/* 6 byte Dest addr, 6 byte Src addr, 2 byte length/type */
#define ETHERNET_HEADER			14
#define ETHERNET_CRC			4    /* Ethernet CRC */

#define TX_BUF_SIZE	\
	(ETHERNET_HEADER + ETHERNET_CRC + MAX_ETHERNET_PAYLOAD + VLAN_TAG)

/* DMA Descriptor Structure
*  The structure is common for both receive and transmit descriptors
*  The descriptor is of 4 words, but our structrue contains 6 words
*  where last two words are to hold the virtual address of the network
*  buffer pointers for driver's use.
*
*  From the GMAC core release 3.50a onwards, the Enhanced Descriptor
*  structure got changed. The descriptor (both transmit and receive) are of
*  8 words each rather the 4 words of normal descriptor structure.
*  Whenever IEEE 1588 Timestamping is enabled TX/RX DESC6 provides the lower
*  32 bits of Timestamp value and TX/RX DESC7 provides the upper 32 bits of
*  Timestamp value. In addition to this whenever extended status bit is set
*  (RX DESC0 bit 0), RX DESC4 contains the extended status information.
*/

/* If set to 1, interrupt is available for all the descriptors or
*  else interrupt is available only for descriptor whose
*  index%MODULO_INTERRUPT is zero.
*/
#define MODULO_INTERRUPT	1

#ifdef ENH_DESC_8W
struct dma_desc {
	u32 status; /* Status */
	u32 length; /* Buffer 1  and Buffer 2 length */
	u32 buffer1;
	u32 buffer2;

/* This data below is used only by driver */
	u32 extstatus; /* Extended status of a Rx Descriptor */
	u32 reserved1; /* Reserved word */

/* Lower and Higher 32 bits of the 64 bit timestamp value */
	u32 timestamplow;
	u32 timestamphigh;

/* This holds virtual address of buffer1 and buffer2, not used by DMA */
	u32 data1;
	u32 data2;
};

#else

struct dma_desc {
	u32 status; /* Status */
	u32 length; /* Buffer 1  and Buffer 2 length */

/* Network Buffer 1 pointer (Dma-able) */
	u32 buffer1;
/* Network Buffer 2 pointer or next descriptor pointer in chain structure */
	u32 buffer2;

/* This data below is used only by driver,
*  holds virtual address of buffer1 and buffer2, not used by DMA.
*/
	unsigned long data1;
	unsigned long data2;

	#ifndef CONFIG_ARM64
	/* add 8 bytes for cache alignment (32 bytes) */
	u32 dummy1;
	u32 dummy2;
	#endif

/* add 32 dummy bytes for cache alignment (64 bytes) */
	u8 dummy_array[32];
};
#endif

enum DescMode {
	RINGMODE	= 0x00000001,
	CHAINMODE	= 0x00000002,
};

enum BufferMode {
	SINGLEBUF	= 0x00000001,
	DUALBUF		= 0x00000002,
};

/* synopGMAC device data */
struct synop_device {
#ifdef USE_DEVICE_TREE
	void __iomem *MacBase;
	void __iomem *DmaBase;
	void __iomem *RmiiModeCtrlReg;
	void __iomem  *ClockCtrlReg;
	void __iomem  *GpioMuxReg;
	void __iomem *RxPadinReg;

#else
	unsigned long *MacBase; /* base address of MAC registers */
	unsigned long *DmaBase; /* base address of DMA registers */
#endif

	u32 PhyBase; /* PHY device address on MII interface */
	u32 Version; /* Gmac Revision version */
	u32 IRQNum;

/* Dma-able address of first Tx/Rx desc either in ring or chain mode,
*  this is used by the GMAC device.
*/
	dma_addr_t TxDescDma;
	dma_addr_t RxDescDma;

/* start address of Tx/Rx desc ring or chain, this is used by the driver  */
	struct dma_desc *TxDesc;
	struct dma_desc *RxDesc;

/* Number of Tx/Rx Descriptors owned by DMA at any given time */
	u32 BusyTxDesc;
	u32 BusyRxDesc;

/* number of Rx/Tx desc in the Rx/Tx descriptor queue/pool */
	u32  RxDescCount;
	u32  TxDescCount;

/* index of the tx desc owned by DMA, obtained by synopGMAC_get_tx_qptr() */
	u32  TxBusy;
/* index of the tx desc next available with driver,
*  given to DMA by synopGMAC_set_tx_qptr()
*/
	u32  TxNext;
/* index of the rx desc owned by DMA, obtained by synopGMAC_get_rx_qptr() */
	u32  RxBusy;
/* index of the rx desc next available with driver,
*  given to DMA by synopGMAC_set_rx_qptr()
*/
	u32  RxNext;

/* Tx/Rx Descriptor address corresponding to the index */
	struct dma_desc *TxBusyDesc; /* TxBusy */
	struct dma_desc *TxNextDesc; /* TxNext */
	struct dma_desc *RxBusyDesc; /* RxBusy */
	struct dma_desc *RxNextDesc; /* RxNext */

/*Phy related stuff*/
	u32 ClockDivMdc; /* Clock divider value programmed in HW */

/* The status of the link */
	u32 LinkState;		/* Link status as reported by Phy */
	u32 DuplexMode;		/* Duplex mode of the Phy */
	u32 Speed;			/* Speed of the Phy */
	u32 LoopBackMode;	/* Loopback status of the Phy */
	u32 AutoNgFail;
	u32 PHY_ID;			/* PHY ID */

	spinlock_t	tx_lock; /* tx lock support */

	u32 ssLoopbackEnable;
	u32 ssLoopbackFrameSize;
	u32 ssLoopbackPassPkt;
	u32 ssLoopbackErrorPkt;
	u32 ssLoopbackRecvPkt;
	u32 rxDesc_invaild_count;
};

/* 88E1011/88E1011S Integrated 10/100/1000 Gigabit Ethernet
*  Transceiver Register and their layouts.
*  This Phy has been used in the Dot Aster GMAC Phy daughter.
*  Since the Phy register map is standard, this map hardly changes
* to a different Phy.
*/
enum MiiRegisters {
	PHY_CONTROL_REG	= 0x0000, /* Control Register */
	PHY_STATUS_REG	= 0x0001, /* Status Register */

	PHY_ID_HI_REG	= 0x0002, /* PHY Id High Register */
	PHY_ID_LOW_REG	= 0x0003, /* PHY Id  Low Register */
	PHY_AN_ADV_REG	= 0x0004, /* AN Advertisement Register */
/* Link Partner Ability Register (Base Page) */
	PHY_LNK_PART_ABl_REG	= 0x0005,
/* Auto-Negotiation Expansion Register */
	PHY_AN_EXP_REG			= 0x0006,

/* Next Page Transmit Register */
	PHY_AN_NXT_PAGE_TX_REG    = 0x0007,
/* Link Partner Next Page Registe */
	PHY_LNK_PART_NXT_PAGE_REG = 0x0008,
/* 1000BASE-T Control Register */
	PHY_1000BT_CTRL_REG       = 0x0009,
/* 1000BASE-T Status Register */
	PHY_1000BT_STATUS_REG     = 0x000a,

/* Phy specific control register */
	PHY_SPECIFIC_CTRL_REG     = 0x0010,
/* Phy specific status register */
	PHY_SPECIFIC_STATUS_REG   = 0x0011,
/* Phy interrupt enable register */
	PHY_INTERRUPT_ENABLE_REG  = 0x0012,
/* Phy interrupt status register */
	PHY_INTERRUPT_STATUS_REG  = 0x0013,
/* Extended Phy specific control */
	PHY_EXT_PHY_SPC_CTRL	= 0x0014,
/* Receive Error Counter */
	PHY_RX_ERR_COUNTER		= 0x0015,

/* Extended address for cable diagnostic register */
	PHY_EXT_ADDR_CBL_DIAG	= 0x0016,
	PHY_LED_CONTROL			= 0x0018, /* LED Control */
	PHY_MAN_LED_OVERIDE		= 0x0019, /* Manual LED override */
	PHY_EXT_PHY_SPC_CTRL2	= 0x001a, /* Extended Phy specific control 2 */
	PHY_EXT_PHY_SPC_STATUS	= 0x001b, /* Extended Phy specific status */
	PHY_CBL_DIAG_REG	= 0x001c, /* Cable diagnostic registers */
	PHY_CONTRL_1_REG	= 0x001e, /* Micrel 10/100Mbps phy */
	PHY_CONTRL_2_REG	= 0x001f, /* Micrel 10/100Mbps phy */
	PHY_PAGE_REG		= 0x001f, /* RTL page register */
	PHY_TEST_REG		= 0x0019, /* RTL8201E Test register */
	PHY_LBREM_REG		= 0x0011, /* RTL8201E LBREM register */
	PHY_RXER_REG		= 0x0012, /* RTL8201E RX ERROR register */
};

enum RTL8201F_Registers {
	PHY_RTL8201F_RMII_MODE_SET_REG			= 16,
	PHY_RTL8201F_FIBER_MODE_LOOPBACK_REG	= 28,
	PHY_RTL8201F_INT_SNR_REG				= 30,
};

enum NVT_INTERNAL_PHY_Registers_D {
	NVT_PHY_REG_0			= 0xfd130000,
	NVT_PHY_REG_IREF		= 0xfd130080,
	NVT_PHY_REG_EQ_RESET	= 0xfd130018,
};

/* Control register layout. Control register is of 16 bit wide. */
enum Mii_GEN_CTRL {
	Mii_reset		= 0x8000,

	Mii_Speed_10	= 0x0000, /* 6:13, 10   Mbps */
	Mii_Speed_100	= 0x2000, /* 6:13, 100  Mbps */
	Mii_Speed_1000	= 0x0040, /* 6:13, 1000 Mbps */

	Mii_Duplex		= 0x0100, /* 8, Full Duplex mode */

/* 11, Manual Master Config */
	Mii_Manual_Master_Config = 0x0800,

	Mii_Loopback	= 0x4000, /* 14, Enable Loop back */
	Mii_NoLoopback	= 0x0000,

	Mii_En_Auto_Neg	= 0x1000,
};

enum Mii_Phy_Status {
	Mii_phy_status_speed_10		= 0x0000,
	Mii_phy_status_speed_100	= 0x4000,
	Mii_phy_status_speed_1000	= 0x8000,

	Mii_phy_status_full_duplex	= 0x2000,
	Mii_phy_status_half_duplex	= 0x0000,

	Mii_phy_status_link_up		= 0x0400,
};

/* Status register layout. Status register is of 16 bit wide. */
enum Mii_GEN_STATUS {
	Mii_100BaseT4		= 0x8000, /* T4 capable */
	Mii_100BaseTX_Full	= 0x4000, /* 100M full */
	Mii_100BaseTX_Half	= 0x2000, /* 100M half */
	Mii_10BaseT_Full	= 0x1000, /* 10M full */
	Mii_10BaseT_Half	= 0x0800, /* 10M half */

	Mii_AutoNegCmplt	= 0x0020, /* 5, AN completed */
	Mii_Link			= 0x0004, /* 2, Link status */
};

enum Mii_Link_Status {
	LINKDOWN	= 0,
	LINKUP		= 1,
};

enum Mii_Duplex_Mode {
	HALFDUPLEX = 1,
	FULLDUPLEX = 2,
};

enum Mii_Link_Speed {
	SPEED10     = 1,
	SPEED100    = 2,
	SPEED1000   = 3,
};

enum Mii_Loop_Back {
	NOLOOPBACK  = 0,
	LOOPBACK    = 1,
};

/* PHY_CONTRL_2_REG */
enum Mii_Phy_Control_2 {
	OP_MODE_AUTO_NEGOTIATION	= 0x0,
	OP_MODE_10BASE_T_HALF		= 0x1,
	OP_MODE_100BASE_TX_HALF		= 0x2,
	OP_MODE_10BASE_T_FULL		= 0x5,
	OP_MODE_100BASE_TX_FULL		= 0x6,
};

/* GMAC registers Map
*  For Pci based system address is BARx + GmacRegisterBase
*  For any other system translation is done accordingly
*/
enum GmacRegisters {
	GmacConfig		= 0x0000, /* Mac config Register */
	GmacFrameFilter	= 0x0004, /* Mac frame filtering controls */
	GmacHashHigh	= 0x0008, /* Multi-cast hash table high */
	GmacHashLow		= 0x000C, /* Multi-cast hash table low */
	GmacGmiiAddr	= 0x0010, /* GMII address Register(ext. Phy) */
	GmacGmiiData	= 0x0014, /* GMII data Register(ext. Phy) */
	GmacFlowControl	= 0x0018, /* Flow control Register */
	GmacVlan		= 0x001C, /* VLAN tag Register (IEEE 802.1Q) */

/* GMAC Core Version Register */
	GmacVersion			= 0x0020,
/* GMAC Wake-up Frame Filter adrress Register */
	GmacWakeupAddr		= 0x0028,
/* PMT Control and Status Register */
	GmacPmtCtrlStatus	= 0x002C,

/* Mac Interrupt ststus Reg */
	GmacInterruptStatus	= 0x0038,
/* Mac Interrupt Mask Reg */
	GmacInterruptMask	= 0x003C,

	GmacAddr0High	= 0x0040, /* Mac address  0 Register */
	GmacAddr0Low	= 0x0044,
	GmacAddr1High	= 0x0048, /* Mac address  1 Register */
	GmacAddr1Low	= 0x004C,
	GmacAddr2High	= 0x0050, /* Mac address  2 Register */
	GmacAddr2Low	= 0x0054,
	GmacAddr3High	= 0x0058, /* Mac address  3 Register */
	GmacAddr3Low	= 0x005C,
	GmacAddr4High	= 0x0060, /* Mac address  4 Register */
	GmacAddr4Low	= 0x0064,
	GmacAddr5High	= 0x0068, /* Mac address  5 Register */
	GmacAddr5Low	= 0x006C,
	GmacAddr6High	= 0x0070, /* Mac address  6 Register */
	GmacAddr6Low	= 0x0074,
	GmacAddr7High	= 0x0078, /* Mac address  7 Register */
	GmacAddr7Low	= 0x007C,
	GmacAddr8High	= 0x0080, /* Mac address  8 Register */
	GmacAddr8Low	= 0x0084,
	GmacAddr9High	= 0x0088, /* Mac address  9 Register */
	GmacAddr9Low	= 0x008C,
	GmacAddr10High	= 0x0090, /* Mac address 10 Register */
	GmacAddr10Low	= 0x0094,
	GmacAddr11High	= 0x0098, /* Mac address 11 Register */
	GmacAddr11Low	= 0x009C,
	GmacAddr12High	= 0x00A0, /* Mac address 12 Register */
	GmacAddr12Low	= 0x00A4,
	GmacAddr13High	= 0x00A8, /* Mac address 13 Register */
	GmacAddr13Low	= 0x00AC,
	GmacAddr14High	= 0x00B0, /* Mac address 14 Register */
	GmacAddr14Low	= 0x00B4,
	GmacAddr15High	= 0x00B8, /* Mac address 15 Register */
	GmacAddr15Low	= 0x00BC,

/* Time Stamp Register Map,
*  only when IEEE 1588 time stamping without external timestamp input
*/
/* Controls the Timestamp update logic */
	GmacTSControl		= 0x0700,

/* 8 bit value by which sub second register is incremented */
	GmacTSSubSecIncr	= 0x0704,

	GmacTSHigh	= 0x0708, /* 32 bit seconds(MS) */
	GmacTSLow	= 0x070C, /* 32 bit nano seconds(MS) */

/* 32 bit seconds(MS) to be written/added/subtracted */
	GmacTSHighUpdate	= 0x0710,
/* 32 bit nano seconds(MS) to be writeen/added/subtracted */
	GmacTSLowUpdate		= 0x0714,

/* Used by Software to readjust the clock frequency linearly */
	GmacTSAddend		= 0x0718,

/* 32 bit seconds(MS) to be compared with system time */
	GmacTSTargetTimeHigh	= 0x071C,
/* 32 bit nano seconds(MS) to be compared with system time */
	GmacTSTargetTimeLow		= 0x0720,

/* Time Stamp Higher Word Register (Version 2 only);
*  only lower 16 bits are valid.
*/
	GmacTSHighWord	= 0x0724,

	GmacTSStatus	= 0x0728, /* Time Stamp Status Register */
};

/* GMAC Network interface registers
*  This explains the Register's Layout
*  FES is Read only by default and is enabled only when Tx
*  Config Parameter is enabled for RGMII/SGMII interface
*  during CoreKit Config.
*  DM is Read only with value 1'b1 in Full duplex only Config
*/

/* GmacConfig	= 0x0000, Mac config Register Layout */
enum GmacConfigReg {
	GmacWatchdog		= 0x00800000,
/* 23, (WD)Disable watchdog timer on Rx */
	GmacWatchdogDisable	= 0x00800000,
/* 0, Enable watchdog timer */
	GmacWatchdogEnable	= 0x00000000,

	GmacJabber			= 0x00400000,
/* 22, (JD)Disable jabber timer on Tx */
	GmacJabberDisable	= 0x00400000,
/* 0, Enable jabber timer */
	GmacJabberEnable	= 0x00000000,

	GmacFrameBurst			= 0x00200000,
/* 21, (BE)Enable frame bursting during Tx */
	GmacFrameBurstEnable	= 0x00200000,
/* 0, Disable frame bursting */
	GmacFrameBurstDisable	= 0x00000000,

	GmacJumboFrame           = 0x00100000,
/* 20, (JE)Enable jumbo frame for Tx */
	GmacJumboFrameEnable     = 0x00100000,
/* 0, Disable jumbo frame */
	GmacJumboFrameDisable    = 0x00000000,

/* 19:17, (IFG) Config - bit times */
	GmacInterFrameGap7	= 0x000E0000, /*  7 - 40 bit times */
	GmacInterFrameGap6	= 0x000C0000, /*  6 - 48 bit times */
	GmacInterFrameGap5	= 0x000A0000, /*  5 - 56 bit times */
	GmacInterFrameGap4	= 0x00080000, /*  4 - 64 bit times */
	GmacInterFrameGap3	= 0x00040000, /*  3 - 72 bit times */
	GmacInterFrameGap2	= 0x00020000, /*  2 - 80 bit times */
	GmacInterFrameGap1	= 0x00010000, /*  1 - 88 bit times */
	GmacInterFrameGap0	= 0x00000000, /*  0 - 96 bit times */

	GmacDisableCrs	= 0x00010000,
	GmacMiiGmii		= 0x00008000,
/* 15, (PS)Port Select-MII mode */
	GmacSelectMii	= 0x00008000,
	GmacSelectGmii	= 0x00000000, /* GMII mode */

/* 14, (FES)Fast Ethernet speed 100Mbps */
	GmacFESpeed100	= 0x00004000,
	GmacFESpeed10	= 0x00000000, /* 0, 10Mbps */

	GmacRxOwn			= 0x00002000,
/* 13, (DO)Disable receive own packets */
	GmacDisableRxOwn	= 0x00002000,
/* 0, Enable receive own packets */
	GmacEnableRxOwn		= 0x00000000,

	GmacLoopback	= 0x00001000,
	GmacLoopbackOn	= 0x00001000, /* 12, Loopback mode for GMII/MII */
	GmacLoopbackOff	= 0x00000000, /*  0, Normal mode */

	GmacDuplex		= 0x00000800,
	GmacFullDuplex	= 0x00000800, /* 11, (DM)Full duplex mode */
	GmacHalfDuplex	= 0x00000000, /*  0,  Half duplex mode */

/* 10, IPC checksum offload */
	GmacRxIpcOffload	= 0x00000400,

	GmacRetry			= 0x00000200,
/* 9, (DR)Disable Retry */
	GmacRetryDisable	= 0x00000200,
/* 0, Enable retransmission as per BL */
	GmacRetryEnable		= 0x00000000,

	GmacLinkUp			= 0x00000100, /* 8, Link UP */
	GmacLinkDown		= 0x00000100, /* 0, Link Down */

	GmacPadCrcStrip			= 0x00000080,
/* 7, (ACS) Automatic Pad/Crc strip enable */
	GmacPadCrcStripEnable	= 0x00000080,
/* 0, Automatic Pad/Crc stripping disable */
	GmacPadCrcStripDisable	= 0x00000000,

	GmacBackoffLimit	= 0x00000060,
/* 6:5, (BL)Back-off limit in HD mode */
	GmacBackoffLimit3	= 0x00000060,
	GmacBackoffLimit2	= 0x00000040,
	GmacBackoffLimit1	= 0x00000020,
	GmacBackoffLimit0	= 0x00000000,

	GmacDeferralCheck			= 0x00000010,
/* 4, (DC)Deferral check enable in HD mode */
	GmacDeferralCheckEnable		= 0x00000010,
/* 0, Deferral check disable */
	GmacDeferralCheckDisable	= 0x00000000,

	GmacTx			= 0x00000008,
	GmacTxEnable	= 0x00000008, /* 3, (TE)Transmitter enable */
	GmacTxDisable	= 0x00000000, /* 0, Transmitter disable */

	GmacRx			= 0x00000004,
	GmacRxEnable	= 0x00000004, /* 2, (RE)Receiver enable */
	GmacRxDisable	= 0x00000000, /* 0, Receiver disable */
};

/* GmacFrameFilter	= 0x0004, Mac frame filtering controls Register Layout*/
enum GmacFrameFilterReg {
	GmacFilter		= 0x80000000,
/* 31, (RA)Receive all incoming packets. */
	GmacFilterOff	= 0x80000000,
/* 0, Receive filtered packets only. */
	GmacFilterOn	= 0x00000000,

/* 10, Hash or Perfect Filter enable */
	GmacHashPerfectFilter		= 0x00000400,

	GmacSrcAddrFilter			= 0x00000200,
/* 9, (SAF)Source Address Filter enable */
	GmacSrcAddrFilterEnable		= 0x00000200,
	GmacSrcAddrFilterDisable	= 0x00000000,

	GmacSrcInvaAddrFilter    = 0x00000100,
/* 8, (SAIF)Inv Src Addr Filter enable */
	GmacSrcInvAddrFilterEn   = 0x00000100,
	GmacSrcInvAddrFilterDis  = 0x00000000,

	GmacPassControl		= 0x000000C0,
/* 7:6, (PCS)Forwards ctrl frms that pass AF */
	GmacPassControl3	= 0x000000C0,
	GmacPassControl2	= 0x00000080, /* Forwards all control frames */
	GmacPassControl1	= 0x00000040, /* Does not pass control frames */
	GmacPassControl0	= 0x00000000, /* Does not pass control frames */

	GmacBroadcast			= 0x00000020,
/* 5, DBF)Disable Rx of broadcast frames */
	GmacBroadcastDisable	= 0x00000020,
/* 0, Enable broadcast frames */
	GmacBroadcastEnable		= 0x00000000,

	GmacMulticastFilter		= 0x00000010,
/* 4, (PM) Pass all multicast packets */
	GmacMulticastFilterOff	= 0x00000010,
/* 0, Pass filtered multicast packets */
	GmacMulticastFilterOn	= 0x00000000,

	GmacDestAddrFilter       = 0x00000008,
/* 3, (DAIF)Inverse filtering for DA */
	GmacDestAddrFilterInv    = 0x00000008,
/* 0, Normal filtering for DA */
	GmacDestAddrFilterNor    = 0x00000000,

	GmacMcastHashFilter      = 0x00000004,
/* 2, (HMC)perfom multicast hash filtering */
	GmacMcastHashFilterOn    = 0x00000004,
/* 0, Pperfect filtering only */
	GmacMcastHashFilterOff   = 0x00000000,

	GmacUcastHashFilter      = 0x00000002,
/* 1, (HUC)Unicast Hash filtering only */
	GmacUcastHashFilterOn    = 0x00000002,
/* 0, Perfect filtering only */
	GmacUcastHashFilterOff   = 0x00000000,

	GmacPromiscuousMode      = 0x00000001,
/* 0, Receive all frames */
	GmacPromiscuousModeOn    = 0x00000001,
/* 0, Receive filtered packets only */
	GmacPromiscuousModeOff   = 0x00000000,
};

/* GmacGmiiAddr	= 0x0010, GMII address Register(ext. Phy) Layout */
enum GmacGmiiAddrReg {
/* 15:11, (PA)GMII device address */
	GmiiDevMask		= 0x0000F800,
	GmiiDevShift	= 11,

/* 10:6, (GR)GMII register in selected Phy */
	GmiiRegMask		= 0x000007C0,
	GmiiRegShift	= 6,

/* 4:2, CSR Clock bit Mask */
	GmiiCsrClkMask	= 0x0000001C,
/* 4:2, (CR)CSR Clock Range  */
	GmiiCsrClk5		= 0x00000014, /* 250-300 MHz */
	GmiiCsrClk4		= 0x00000010, /* 150-250 MHz */
	GmiiCsrClk3		= 0x0000000C, /*   35-60 MHz */
	GmiiCsrClk2		= 0x00000008, /*   20-35 MHz */
	GmiiCsrClk1		= 0x00000004, /* 100-150 MHz */
	GmiiCsrClk0		= 0x00000000, /*  60-100 MHz */

	GmiiWrite		= 0x00000002, /* 1, (GW)Write to register */
	GmiiRead		= 0x00000000, /* 0, Read from register */

/* 0, (GB)GMII interface is busy */
	GmiiBusy		= 0x00000001,
};

/* GmacGmiiData	= 0x0014, GMII data Register(ext. Phy) Layout */
enum GmacGmiiDataReg {
	GmiiDataMask	= 0x0000FFFF, /* 15:0, (GD)GMII Data  RW */
};

/* GmacFlowControl	= 0x0018, Flow control Register   Layout */
enum GmacFlowControlReg {
/* 31:16, (PT) PAUSE TIME field in the control frame  RW */
	GmacPauseTimeMask	= 0xFFFF0000,
	GmacPauseTimeShift	= 16,

/* 5:4, (PLT)thresh for pause tmr slot time  RW */
	GmacPauseLowThresh	= 0x00000030,
	GmacPauseLowThresh3	= 0x00000030, /*  56 slot time */
	GmacPauseLowThresh2	= 0x00000020, /* 144 slot time */
	GmacPauseLowThresh1	= 0x00000010, /*  28 slot time */
	GmacPauseLowThresh0	= 0x00000000, /*   4 slot time */

	GmacUnicastPauseFrame    = 0x00000008,
/* 3, (UP)Detect pause frame with unicast addr. */
	GmacUnicastPauseFrameOn  = 0x00000008,
/* 0, Detect only pause frame with multicast addr. */
	GmacUnicastPauseFrameOff = 0x00000000,

	GmacRxFlowControl			= 0x00000004,
/* 2, (RFE)Enable Rx flow control */
	GmacRxFlowControlEnable		= 0x00000004,
/* 0, Disable Rx flow control */
	GmacRxFlowControlDisable	= 0x00000000,

	GmacTxFlowControl		= 0x00000002,
/* 1, (TFE)Enable Tx flow control */
	GmacTxFlowControlEnable	= 0x00000002,
/* 0, Disable flow control */
	GmacTxFlowControlDisable = 0x00000000,

	GmacFlowControlBackPressure	= 0x00000001,
/* 0, (FCB/PBA)send pause frm/Apply back pressure */
	GmacSendPauseFrame			= 0x00000001,
};

/*  GmacInterruptStatus	= 0x0038, Mac Interrupt ststus register */
enum GmacInterruptStatusBitDefinition {
/* Set if int generated due to TS,
* (Read Time Stamp Status Register to know details).
*/
	GmacTSIntSts		= 0x00000200,
/* set if int generated in MMC RX CHECKSUM OFFLOAD int register */
	GmacMmcRxChksumOffload	= 0x00000080,
/* set if int generated in MMC TX Int register */
	GmacMmcTxIntSts		= 0x00000040,
/* set if int generated in MMC RX Int register */
	GmacMmcRxIntSts		= 0x00000020,
/* set if any of the above bit [7:5] is set */
	GmacMmcIntSts		= 0x00000010,
/* set whenever magic pkt/wake-on-lan frame is received */
	GmacPmtIntSts		= 0x00000008,
/* set when AN is complete in TBI/RTBI/SGMIII phy interface */
	GmacPcsAnComplete	= 0x00000004,
/* set if any lnk status change in TBI/RTBI/SGMII interface */
	GmacPcsLnkStsChange	= 0x00000002,
/* set if any change in lnk status of RGMII interface */
	GmacRgmiiIntSts		= 0x00000001,
};

/* GmacInterruptMask	= 0x003C, Mac Interrupt Mask register */
enum GmacInterruptMaskBitDefinition {
/* Disables the time stamp interrupt generation */
	GmacTSIntMask			= 0x00000200,
/* Disables the assertion of PMT interrupt */
	GmacPmtIntMask			= 0x00000008,
/* Disables the assertion of PCS AN complete interrupt */
	GmacPcsAnIntMask		= 0x00000004,
/* Disables the assertion of PCS lnk status change interrupt */
	GmacPcsLnkStsIntMask	= 0x00000002,
/* Disables the assertion of RGMII int */
	GmacRgmiiIntMask		= 0x00000001,
};

/* GMAC DMA registers
* For Pci based system address is BARx + GmaDmaBase
* For any other system translation is done accordingly.
*/
enum DmaRegisters {
/* CSR0 - Bus Mode Register */
	DmaBusMode        = 0x0000,
/* CSR1 - Transmit Poll Demand Register */
	DmaTxPollDemand   = 0x0004,
/* CSR2 - Receive Poll Demand Register */
	DmaRxPollDemand   = 0x0008,
/* CSR3 - Receive Descriptor list base address */
	DmaRxBaseAddr     = 0x000C,

/* CSR4 - Transmit Descriptor list base address */
	DmaTxBaseAddr     = 0x0010,
/* CSR5 - Dma status Register */
	DmaStatus         = 0x0014,
/* CSR6 - Dma Operation Mode Register */
	DmaControl        = 0x0018,
/* CSR7 - Interrupt enable */
	DmaInterrupt      = 0x001C,

/* CSR8 - Missed Frame & Buffer overflow Counter */
	DmaMissedFr       = 0x0020,

/*      - Current host Tx Desc Register */
	DmaTxCurrDesc     = 0x0048,
/*      - Current host Rx Desc Register */
	DmaRxCurrDesc     = 0x004C,
/* CSR20 - Current host transmit buffer address */
	DmaTxCurrAddr     = 0x0050,
/* CSR21 - Current host receive buffer address */
	DmaRxCurrAddr     = 0x0054,
};

/**********************************************************
 * DMA Engine registers Layout
 **********************************************************/

/* DmaBusMode	= 0x0000, CSR0 - Bus Mode */
enum DmaBusModeReg {
 /* 16, (FB)Fixed Burst = SINGLE, INCR4, INCR8, or INCR16 */
	DmaFixedBurstEnable     = 0x00010000,
	DmaFixedBurstDisable    = 0x00000000, /* SINGLE, INCR */

/* 15:14, (PR)TX:RX DMA priority ratio */
	DmaTxPriorityRatio11    = 0x00000000, /* ratio 1:1 */
	DmaTxPriorityRatio21    = 0x00004000, /* ratio 2:1 */
	DmaTxPriorityRatio31    = 0x00008000, /* ratio 3:1 */
	DmaTxPriorityRatio41    = 0x0000C000, /* ratio 4:1 */

/* 24, When set mutiplies the PBL by 8 */
	DmaBurstLengthx8        = 0x01000000,

/* [24]:13:8, (DmaBurstLengthx8 | DmaBurstLength32) = 256 */
	DmaBurstLength256	= 0x01002000,
/* [24]:13:8, (DmaBurstLengthx8 | DmaBurstLength16) = 128 */
	DmaBurstLength128	= 0x01001000,
/* [24]:13:8, (DmaBurstLengthx8 | DmaBurstLength8) = 64 */
	DmaBurstLength64	= 0x01000800,

/* 13:8, (PBL) programmable Dma burst length = 32 */
	DmaBurstLength32	= 0x00002000,
	DmaBurstLength16	= 0x00001000, /* length = 16 */
	DmaBurstLength8		= 0x00000800, /* length = 8 */
	DmaBurstLength4		= 0x00000400, /* length = 4 */
	DmaBurstLength2		= 0x00000200, /* length = 2 */
	DmaBurstLength1		= 0x00000100, /* length = 1 */
	DmaBurstLength0		= 0x00000000, /* length = 0 */

/* 7, Enh Descriptor works  1=> 8 word descriptor */
	DmaDescriptor8Words	= 0x00000080,
/* 7, Enh Descriptor works  0=> 4 word descriptor */
	DmaDescriptor4Words	= 0x00000000,

/* 6:2, (DSL)Descriptor skip length (no.of dwords)  RW */
	DmaDescriptorSkip16	= 0x00000040,
	DmaDescriptorSkip8	= 0x00000020,
	DmaDescriptorSkip6	= 0x00000018,
	DmaDescriptorSkip4	= 0x00000010,
	DmaDescriptorSkip2	= 0x00000008,
	DmaDescriptorSkip1	= 0x00000004,
	DmaDescriptorSkip0	= 0x00000000,

	DmaArbitPr	= 0x00000002, /* Rx has priority over Tx */
	DmaArbitRr	= 0x00000000, /* (DA) DMA RR arbitration */

	DmaResetOn	= 0x00000001, /* (SWR)Software Reset DMA engine */
	DmaResetOff	= 0x00000000,
};

/* DmaStatus	= 0x0014, CSR5 - Dma status Register */
enum DmaStatusReg {
/* Bit 28:26 indicate whether the interrupt due to PMT GMACMMC or
*  GMAC LINE Remaining bits are DMA interrupts.
*/
/* 28, (GPI)Gmac subsystem interrupt RO */
	GmacPmtIntr			= 0x10000000,
/* 27, (GMI)Gmac MMC subsystem interrupt RO */
	GmacMmcIntr			= 0x08000000,
/* 26, Line interface interrupt RO */
	GmacLineIntfIntr	= 0x04000000,

/* 25, (EB)Error bits 0-data buffer, 1-desc. access RO */
	DmaErrorBit2	= 0x02000000,
/* 24, (EB)Error bits 0-write trnsf, 1-read transfr RO */
	DmaErrorBit1	= 0x01000000,
/* 23, (EB)Error bits 0-Rx DMA, 1-Tx DMA RO */
	DmaErrorBit0	= 0x00800000,

/* 22:20, (TS)Transmit process state  RO */
	DmaTxState		= 0x00700000,
/* Stopped - Reset or Stop Tx Command issued */
	DmaTxStopped	= 0x00000000,
/* Running - fetching the Tx descriptor */
	DmaTxFetching	= 0x00100000,
/* Running - waiting for status */
	DmaTxWaiting	= 0x00200000,
/* Running - reading the data from host memory */
	DmaTxReading	= 0x00300000,
/* Suspended - Tx Descriptor unavailabe */
	DmaTxSuspended	= 0x00600000,
/* Running - closing Rx descriptor */
	DmaTxClosing	= 0x00700000,

/* 19:17, (RS)Receive process state  RO */
	DmaRxState		= 0x000E0000,
/* Stopped - Reset or Stop Rx Command issued */
	DmaRxStopped	= 0x00000000,
/* Running - fetching the Rx descriptor */
	DmaRxFetching	= 0x00020000,
/* Running - waiting for packet */
	DmaRxWaiting	= 0x00060000,
/* Suspended - Rx Descriptor unavailable */
	DmaRxSuspended	= 0x00080000,
/* Running - closing descriptor */
	DmaRxClosing	= 0x000A0000,
/* Running - queuing the receive frame into host memory */
	DmaRxQueuing	= 0x000E0000,

/* (NIS)Normal interrupt summary */
	DmaIntNormal		= 0x00010000,

/* (AIS)Abnormal interrupt summary */
	DmaIntAbnormal		= 0x00008000,
/* Early receive interrupt (Normal) */
	DmaIntEarlyRx		= 0x00004000,
/* Fatal bus error (Abnormal) */
	DmaIntBusError		= 0x00002000,
/* Early transmit interrupt (Abnormal) */
	DmaIntEarlyTx		= 0x00000400,
/* Receive Watchdog Timeout (Abnormal) */
	DmaIntRxWdogTO		= 0x00000200,
/* Receive process stopped (Abnormal) */
	DmaIntRxStopped		= 0x00000100,

/* Receive buffer unavailable (Abnormal) */
	DmaIntRxNoBuffer	= 0x00000080,
/* Completion of frame reception (Normal) */
	DmaIntRxCompleted	= 0x00000040,
/* Transmit underflow (Abnormal) */
	DmaIntTxUnderflow	= 0x00000020,
/* Receive Buffer overflow interrupt */
	DmaIntRcvOverflow	= 0x00000010,

/* Transmit Jabber Timeout (Abnormal) */
	DmaIntTxJabberTO	= 0x00000008,
/* Transmit buffer unavailable (Normal) */
	DmaIntTxNoBuffer	= 0x00000004,
/* Transmit process stopped (Abnormal) */
	DmaIntTxStopped		= 0x00000002,
/* Transmit completed (Normal) */
	DmaIntTxCompleted	= 0x00000001,
};

/* DmaControl	= 0x0018, CSR6 - Dma Operation Mode Register */
enum DmaControlReg {
/* 26, (DT) Dis. drop. of tcp/ip CS error frames */
	DmaDisableDropTcpCs	= 0x04000000,

/* 21, (SF)Store and forward */
	DmaStoreAndForward	= 0x00200000,
/* 20, (FTF)Tx FIFO controller is reset to default */
	DmaFlushTxFifo		= 0x00100000,

/* 16:14, (TTC)Controls the Threh of MTL tx Fifo RW */
	DmaTxThreshCtrl		= 0x0001C000,
	DmaTxThreshCtrl16	= 0x0001C000, /* tx Fifo  16 */
	DmaTxThreshCtrl24	= 0x00018000, /* tx Fifo  24 */
	DmaTxThreshCtrl32	= 0x00014000, /* tx Fifo  32 */
	DmaTxThreshCtrl40	= 0x00010000, /* tx Fifo  40 */
	DmaTxThreshCtrl256	= 0x0000c000, /* tx Fifo 256 */
	DmaTxThreshCtrl192	= 0x00008000, /* tx Fifo 192 */
	DmaTxThreshCtrl128	= 0x00004000, /* tx Fifo 128 */
	DmaTxThreshCtrl64	= 0x00000000, /* tx Fifo  64 */

	DmaTxStart	= 0x00002000, /* 13, (ST)Start/Stop transmission */

/* [22]:12:11, (RFD)Rx flow control deact. threhold   RW */
	DmaRxFlowCtrlDeact      = 0x00401800,
	DmaRxFlowCtrlDeact1K    = 0x00000000, /* threhold (1kbytes) */
	DmaRxFlowCtrlDeact2K    = 0x00000800, /* threhold (2kbytes) */
	DmaRxFlowCtrlDeact3K    = 0x00001000, /* threhold (3kbytes) */
	DmaRxFlowCtrlDeact4K    = 0x00001800, /* threhold (4kbytes) */
	DmaRxFlowCtrlDeact5K    = 0x00400000, /* threhold (4kbytes) */
	DmaRxFlowCtrlDeact6K    = 0x00400800, /* threhold (4kbytes) */
	DmaRxFlowCtrlDeact7K    = 0x00401000, /* threhold (4kbytes) */

/* [23]:10:09, (RFA)Rx flow control Act. threhold   RW*/
	DmaRxFlowCtrlAct	= 0x00800600,
	DmaRxFlowCtrlAct1K	= 0x00000000, /* threhold (1kbytes) */
	DmaRxFlowCtrlAct2K	= 0x00000200, /* threhold (2kbytes) */
	DmaRxFlowCtrlAct3K	= 0x00000400, /* threhold (3kbytes) */
	DmaRxFlowCtrlAct4K	= 0x00000600, /* threhold (4kbytes) */
	DmaRxFlowCtrlAct5K	= 0x00800000, /* threhold (5kbytes) */
	DmaRxFlowCtrlAct6K	= 0x00800200, /* threhold (6kbytes) */
	DmaRxFlowCtrlAct7K	= 0x00800400, /* threhold (7kbytes) */

/* 4:3, (RTC)Controls thre Threh of MTL tx Fifo	  RW */
	DmaRxThreshCtrl		= 0x00000018, /* rx Fifo     */
	DmaRxThreshCtrl64	= 0x00000000, /* tx Fifo  64 */
	DmaRxThreshCtrl32	= 0x00000008, /* tx Fifo  32 */
	DmaRxThreshCtrl96	= 0x00000010, /* tx Fifo  96 */
	DmaRxThreshCtrl128	= 0x00000018, /* tx Fifo 128 */

/* 8, (EFC)Enable HW flow control		RW	*/
	DmaEnHwFlowCtrl		= 0x00000100,
/* 0, Disable HW flow control				*/
	DmaDisHwFlowCtrl	= 0x00000000,

/* 7, (FEF)Forward error frames			RW	*/
	DmaFwdErrorFrames	= 0x00000080,
/* 6, (FUF)Forward undersize frames		RW	*/
	DmaFwdUnderSzFrames	= 0x00000040,

/* 3, (OSF)Operate on second frame		RW	*/
	DmaTxSecondFrame	= 0x00000004,
/* 1, (SR)Start/Stop reception			RW	*/
	DmaRxStart			= 0x00000002,
};

/* DmaInterrupt	= 0x001C, CSR7 - Interrupt enable Register Layout */
enum  DmaInterruptReg {
/* Normal interrupt enable                 RW	0 */
	DmaIeNormal			= DmaIntNormal,
/* Abnormal interrupt enable               RW	0 */
	DmaIeAbnormal		= DmaIntAbnormal,

/* Early receive interrupt enable          RW	0 */
	DmaIeEarlyRx		= DmaIntEarlyRx,
/* Fatal bus error enable                  RW	0 */
	DmaIeBusError		= DmaIntBusError,
/* Early transmit interrupt enable         RW	0 */
	DmaIeEarlyTx		= DmaIntEarlyTx,
/* Receive Watchdog Timeout enable         RW	0 */
	DmaIeRxWdogTO		= DmaIntRxWdogTO,
/* Receive process stopped enable          RW	0 */
	DmaIeRxStopped		= DmaIntRxStopped,
/* Receive buffer unavailable enable       RW	0 */
	DmaIeRxNoBuffer		= DmaIntRxNoBuffer,
/* Completion of frame reception enable    RW	0 */
	DmaIeRxCompleted	= DmaIntRxCompleted,
/* Transmit underflow enable               RW	0 */
	DmaIeTxUnderflow	= DmaIntTxUnderflow,

/* Receive Buffer overflow interrupt       RW	0 */
	DmaIeRxOverflow		= DmaIntRcvOverflow,
/* Transmit Jabber Timeout enable          RW	0 */
	DmaIeTxJabberTO		= DmaIntTxJabberTO,
/* Transmit buffer unavailable enable      RW	0 */
	DmaIeTxNoBuffer		= DmaIntTxNoBuffer,
/* Transmit process stopped enable         RW	0 */
	DmaIeTxStopped		= DmaIntTxStopped,
/* Transmit completed enable               RW	0 */
	DmaIeTxCompleted	= DmaIntTxCompleted,
};

/*******************************************************
 * DMA Engine descriptors
 *******************************************************/

#ifdef ENH_DESC
/***** Enhanced Descritpor structure to support 8K buffer per buffer ******
* DmaRxBaseAddr	= 0x000C, CSR3 - Receive Descriptor list base address
* DmaRxBaseAddr is the pointer to the first Rx Descriptors.
* The Descriptor format in Little endian with a 32 bit Data bus is
* as shown below
* -----------------------------------------------------------------------------
* RDES0	| OWN (31)| Status                                                    |
* -----------------------------------------------------------------------------
* RDES1	| Ctrl | Res | Byte Count Buffer 2 | Ctrl | Res | Byte Count Buffer 1 |
* -----------------------------------------------------------------------------
* RDES2	| Buffer 1 Address                                                    |
* -----------------------------------------------------------------------------
* RDES3	| Buffer 2 Address / Next Descriptor Address                          |
* -----------------------------------------------------------------------------
*
* Similarly,
* DmaTxBaseAddr	= 0x0010, CSR4 - Transmit Descriptor list base address
* DmaTxBaseAddr is the pointer to the first Rx Descriptors.
* The Descriptor format in Little endian with a 32 bit Data bus is
* as shown below
* -----------------------------------------------------------------------------
* TDES0 | OWN (31)| Ctrl | Res | Ctrl | Res | Status                          |
* -----------------------------------------------------------------------------
* TDES1 | Res | Byte Count Buffer 2 | Res |      Byte Count Buffer 1          |
* -----------------------------------------------------------------------------
* TDES2 | Buffer 1 Address                                                    |
* -----------------------------------------------------------------------------
* TDES3 | Buffer 2 Address / Next Descriptor Address                          |
* -----------------------------------------------------------------------------
*/

/* status word of DMA descriptor */
enum DmaDescriptorStatus {
	DescOwnByDma	= 0x80000000, /* 31, Descriptor is owned by DMA */

/* 30, (AFM)Rx - DA Filter Fail for the rx frame */
	DescDAFilterFail	= 0x40000000,

/* 29:16, (FL)Receive descriptor frame length */
	DescFrameLengthMask   = 0x3FFF0000,
	DescFrameLengthShift  = 16,

	DescError	= 0x00008000, /* 15, (ES)Error Summary */

/*  DE || OE || IPC || LC || RWT || RE || CE */
/* 14, (DE)Rx - no more descriptors for receive frame */
	DescRxTruncated		= 0x00004000,
/* 13, (SAF)Rx - SA Filter Fail for the received frame */
	DescSAFilterFail	= 0x00002000,
/* 12, (LE)Rx - frm size not matching with len field */
	DescRxLengthError	= 0x00001000,

/* 11, (OE)Rx - frm was damaged due to buffer overflow */
	DescRxDamaged		= 0x00000800,
/* 10, (VLAN)Rx - received frame is a VLAN frame */
	DescRxVLANTag		= 0x00000400,
/* 9, (FS)Rx - first descriptor of the frame */
	DescRxFirst			= 0x00000200,
/* 8, (LS)Rx - last descriptor of the frame */
	DescRxLast			= 0x00000100,

/* 7, (Giant Frame)Rx - frame is longer than 1518/1522 */
	DescRxLongFrame		= 0x00000080,
/* 6, (LC)Rx - late collision occurred during reception */
	DescRxCollision		= 0x00000040,
/* 5, (FT)Rx - Frame type - Ethernet, otherwise 802.3 */
	DescRxFrameEther	= 0x00000020,
/* 4, (RWT)Rx - watchdog timer expired during reception */
	DescRxWatchdog		= 0x00000010,
/* 3, (RE)Rx - error reported by MII interface */
	DescRxMiiError		= 0x00000008,
/* 2, (DE)Rx - frame contains non int multiple of 8 bits */
	DescRxDribbling		= 0x00000004,
/* 1, (CE)Rx - CRC error */
	DescRxCrc			= 0x00000002,

	DescRxEXTsts	= 0x00000001, /* 0, Extended Status Available (RDES4) */

/* 30, (IC)Tx - interrupt on completion */
	DescTxIntEnable	= 0x40000000,
/* 29, (LS)Tx - Last segment of the frame */
	DescTxLast		= 0x20000000,
/* 28, (FS)Tx - First segment of the frame */
	DescTxFirst		= 0x10000000,
/* 27, (DC)Tx - Add CRC disabled (first segment only) */
	DescTxDisableCrc	= 0x08000000,
	DescTxDisablePadd	= 0x04000000, /* 26, (DP)disable padding */

/* 23:22, Tx checksum offloading control mask */
	DescTxCisMask		= 0x00c00000,
/* TCP/UDP/ICMP checksum. Pseudo header checksum is assumed to be present */
	DescTxCisTcpOnlyCs	= 0x00800000,
	DescTxCisIpv4HdrCs	= 0x00400000, /* IPv4 header checksum */
	DescTxCisBypass		= 0x00000000, /* Checksum bypass */

/* TCP/UDP/ICMP checksum fully in hardware including pseudo header */
	DescTxCisTcpPseudoCs	= 0x00c00000,

/* 21, (TER)End of descriptors ring */
	TxDescEndOfRing	= 0x00200000,
/* 20, (TCH)Second buffer address is chain address */
	TxDescChain		= 0x00100000,

/* 7, (IPC CS ERROR)Rx - Ipv4 header checksum error */
	DescRxChkBit7	= 0x00000080,
/* 5, (FT)Rx - Frame type - Ethernet, otherwise 802.3 */
	DescRxChkBit5	= 0x00000020,
	DescRxChkBit0	= 0x00000001, /*0, Rx - Rx Payload Checksum Error */

	DescRxTSavail	= 0x00000080, /* 7, Time stamp available */
/* 5, (FT)Rx - Frame type - Ethernet, otherwise 802.3 */
	DescRxFrameType	= 0x00000020,

	DescTxIpv4ChkError	= 0x00010000, /* 16, (IHE) Tx Ip header error */

/* 14, (JT)Tx - Transmit jabber timeout */
	DescTxTimeout		= 0x00004000,
/* 13, (FF)Tx - DMA/MTL flushed the frame due to SW flush */
	DescTxFrameFlushed	= 0x00002000,
/* 12, (PCE) Tx Payload checksum Error */
	DescTxPayChkError	= 0x00001000,

/* 11, (LC)Tx - carrier lost during tramsmission */
	DescTxLostCarrier	= 0x00000800,
/* 10, (NC)Tx - no carrier signal from the tranceiver */
	DescTxNoCarrier		= 0x00000400,
/* 9, (LC)Tx - transmission aborted due to collision */
	DescTxLateCollision	= 0x00000200,
/* 8, (EC)Tx - tx aborted after 16 collisions */
	DescTxExcCollisions	= 0x00000100,

/* 7, (VF)Tx - VLAN-type frame */
	DescTxVLANFrame       = 0x00000080,

/* 6:3, (CC)Tx - Collision count */
	DescTxCollMask		= 0x00000078,
	DescTxCollShift		= 3,

/* 2, (ED)Tx - excessive deferral */
	DescTxExcDeferral	= 0x00000004,
/* 1, (UF)Tx - late data arrival from the memory */
	DescTxUnderflow		= 0x00000002,
/* 0, (DB)Tx - frame tx deferred */
	DescTxDeferred		= 0x00000001,

/* This explains the RDES1/TDES1 bits layout
*  ------------------------------------------------------------------------
*  RDES1/TDES1 | Control Bits | Byte Count Buffer 2 | Byte Count Buffer 1 |
*  ------------------------------------------------------------------------
*/

	RxDisIntCompl	= 0x80000000, /* 31, Disable Rx int on completion */

	RxDescEndOfRing	= 0x00008000, /* 15, (TER)End of descriptors ring */
/* 14, (TCH)Second buffer address is chain address */
	RxDescChain		= 0x00004000,

	DescSize2Mask	= 0x1FFF0000, /* 28:16, (TBS2) Buffer 2 size */
	DescSize2Shift	= 16,
	DescSize1Mask	= 0x00001FFF, /* 12:0,  (TBS1) Buffer 1 size */
	DescSize1Shift	= 0,

/* This explains the RDES4 Extended Status bits layout
*  --------------------------------------------
*  RDES4    |       Extended Status           |
*  --------------------------------------------
*/
	DescRxPtpAvail	= 0x00004000, /* 14, PTP snapshot available */
/* 13, When set indicates IEEE1584 Version 2 (else Ver1) */
	DescRxPtpVer	= 0x00002000,
/* 12, PTP frame type Indicates PTP sent over ethernet */
	DescRxPtpFrameType		= 0x00001000,

	DescRxPtpMessageType	= 0x00000F00, /* 11:8, Message Type */

/* 0000 => No PTP message received */
	DescRxPtpNo			= 0x00000000,
/* 0001 => Sync (all clock types) received */
	DescRxPtpSync		= 0x00000100,
/* 0010 => Follow_Up (all clock types) received */
	DescRxPtpFollowUp	= 0x00000200,
/* 0011 => Delay_Req (all clock types) received */
	DescRxPtpDelayReq	= 0x00000300,
/* 0100 => Delay_Resp (all clock types) received */
	DescRxPtpDelayResp	= 0x00000400,
/* 0101 => Pdelay_Req (in P to P tras clk) or
*          Announce in Ord and Bound clk
*/
	DescRxPtpPdelayReq	= 0x00000500,
/* 0110 => Pdealy_Resp(in P to P trans clk) or
*          Management in Ord and Bound clk
*/
	DescRxPtpPdelayResp	= 0x00000600,
/* 0111 => Pdealy_Resp_Follow_Up (in P to P trans clk) or
*          Signaling in Ord and Bound clk
*/
	DescRxPtpPdelayRespFP = 0x00000700,

	DescRxPtpIPV6	= 0x00000080, /* 7, Rx Packet is in IPV6 Pkt */
	DescRxPtpIPV4	= 0x00000040, /* 6, Rx Packet is in IPV4 Pkt */
/* 5, Indicates checksum offload engine is bypassed */
	DescRxChkSumBypass		= 0x00000020,
/* 4, Indicates 16bit IP payload CS is in error */
	DescRxIpPayloadError	= 0x00000010,

/* 3, When set indicates 16bit IPV4 header CS is in error or IP datagram
*     version is not consistent with Ethernet type value
*/
	DescRxIpHeaderError   = 0x00000008,

/* 2:0, Type of payload encapsulated in IPdatagram processed by COE (Rx) */
	DescRxIpPayloadType   = 0x00000007,
	DescRxIpPayloadICMP   = 0x00000003, /* ICMP */
	DescRxIpPayloadTCP    = 0x00000002, /* TCP */
	DescRxIpPayloadUDP    = 0x00000001, /* UDP */

/* Unknown or didnot process IP payload */
	DescRxIpPayloadUnknown	= 0x00000000,
};

#else
/****** Default Descritpor structure ************
* DmaRxBaseAddr	= 0x000C, CSR3 - Receive Descriptor list base address.
* DmaRxBaseAddr is the pointer to the first Rx Descriptors.
* The Descriptor format in Little endian with a 8 32 bit Data bus is
* as shown below
*
* Similarly,
* DmaTxBaseAddr	= 0x0010, CSR4 - Transmit Descriptor list base address.
* DmaTxBaseAddr is the pointer to the first Rx Descriptors.
* The Descriptor format in Little endian with a 32 bit Data bus is
* as shown below
* -------------------------------------------------------------------------
* RDES0/TDES0  |OWN (31)| Status                                          |
* -------------------------------------------------------------------------
* RDES1/TDES1  | Control Bits | Byte Count Buffer 2 | Byte Count Buffer 1 |
* -------------------------------------------------------------------------
* RDES2/TDES2  |  Buffer 1 Address                                        |
* -------------------------------------------------------------------------
* RDES3/TDES3  |  Buffer 2 Address / Next Descriptor Address              |
* -------------------------------------------------------------------------
*/

/* Status word of DMA descriptor */
enum DmaDescriptorStatus {
	DescOwnByDma	= 0x80000000, /* 31, Descriptor is owned by DMA */

/* 30, (AFM)Rx - DA Filter Fail for the rx frame */
	DescDAFilterFail	= 0x40000000,

/* 29:16, (FL)Receive descriptor frame length */
	DescFrameLengthMask		= 0x3FFF0000,
	DescFrameLengthShift	= 16,

	DescError	= 0x00008000, /* 15, (ES)Error Summary */

/* DE || OE || IPC || LC || RWT || RE || CE */
/* 14, (DE)Rx - no more descriptors for receive frame */
	DescRxTruncated		= 0x00004000,
/* 13, (SAF)Rx - SA Filter Fail for the received frame */
	DescSAFilterFail	= 0x00002000,
/* 12, (LE)Rx - frm size not matching with len field */
	DescRxLengthError	= 0x00001000,

/* 11, (OE)Rx - frm was damaged due to buffer overflow */
	DescRxDamaged	= 0x00000800,
/* 10, (VLAN)Rx - received frame is a VLAN frame */
	DescRxVLANTag	= 0x00000400,
	DescRxFirst		= 0x00000200, /* 9, (FS)Rx - first descriptor */
	DescRxLast		= 0x00000100, /* 8, (LS)Rx - last descriptor */

/* 7, (Giant Frame)Rx - frame is longer than 1518/1522 */
	DescRxLongFrame		= 0x00000080,
/* 6, (LC)Rx - late collision occurred during reception */
	DescRxCollision		= 0x00000040,
/* 5, (FT)Rx - Frame type - Ethernet, otherwise 802.3 */
	DescRxFrameEther	= 0x00000020,
/* 4, (RWT)Rx - watchdog timer expired during reception */
	DescRxWatchdog		= 0x00000010,

/* 3, (RE)Rx - error reported by MII interface */
	DescRxMiiError	= 0x00000008,
/* 2, (DE)Rx - frame contains non int multiple of 8 */
	DescRxDribbling	= 0x00000004,
	DescRxCrc		= 0x00000002, /* 1, (CE)Rx - CRC error */
	DescRxMacMatch	= 0x00000001, /* 0, mac address reg(1~15)match */

	DescRxChkBit0	= 0x00000001, /* 0, Rx Payload Checksum Error */
	DescRxChkBit7	= 0x00000080, /* 7, Ipv4 header checksum error */
/* 5, (FT)Rx - Frame type - Ethernet, otherwise 802.3 */
	DescRxChkBit5	= 0x00000020,

	DescTxIpv4ChkError    = 0x00010000, /* 16, (IHE) Tx Ip header error */

/* 14, (JT)Tx - Transmit jabber timeout */
	DescTxTimeout         = 0x00004000,
/* 13, (FF)Tx - DMA/MTL flushed the frame due to SW flush */
	DescTxFrameFlushed    = 0x00002000,
/* 12, (PCE) Tx Payload checksum Error */
	DescTxPayChkError     = 0x00001000,

/* 11, (LC)Tx - carrier lost during tramsmission */
	DescTxLostCarrier     = 0x00000800,
/* 10, (NC)Tx - no carrier signal from the tranceiver */
	DescTxNoCarrier       = 0x00000400,
/* 9, (LC)Tx - transmission aborted due to collision */
	DescTxLateCollision   = 0x00000200,
/* 8, (EC)Tx - transmission aborted after 16 collisions */
	DescTxExcCollisions   = 0x00000100,

	DescTxVLANFrame       = 0x00000080, /* 7, (VF)Tx - VLAN-type frame */

	DescTxCollMask        = 0x00000078, /* 6:3, (CC)Tx - Collision count */
	DescTxCollShift       = 3,

/* 2, (ED)Tx - excessive deferral */
	DescTxExcDeferral     = 0x00000004,
/* 1, (UF)Tx - late data arrival from the memory */
	DescTxUnderflow       = 0x00000002,
/* 0, (DB)Tx - frame transmision deferred */
	DescTxDeferred        = 0x00000001,

/* This explains the RDES1/TDES1 bits layout
* ------------------------------------------------------------------------
* RDES1/TDES1 | Control Bits | Byte Count Buffer 2 | Byte Count Buffer 1 |
* ------------------------------------------------------------------------
*/

/* 31, (IC)Tx - interrupt on completion */
	DescTxIntEnable		= 0x80000000,
/* 30, (LS)Tx - Last segment of the frame */
	DescTxLast			= 0x40000000,
/* 29, (FS)Tx - First segment of the frame */
	DescTxFirst			= 0x20000000,
/* 26, (DC)Tx - Add CRC disabled (first segment only) */
	DescTxDisableCrc	= 0x04000000,

	RxDisIntCompl	= 0x80000000, /* 31, (Disable Rx int on completion) */
	RxDescEndOfRing	= 0x02000000, /* 25, (TER)End of descriptors ring */
/* 24, (TCH)Second buffer address is chain address */
	RxDescChain		= 0x01000000,

	DescTxDisablePadd	= 0x00800000, /* 23, (DP)disable padding */

	TxDescEndOfRing	= 0x02000000, /* (TER)End of descriptors ring */
/* 24, (TCH)Second buffer address is chain address */
	TxDescChain		= 0x01000000,

	DescSize2Mask	= 0x003FF800, /* 21:11, (TBS2) Buffer 2 size */
	DescSize2Shift	= 11,
	DescSize1Mask	= 0x000007FF, /* 10:0,  (TBS1) Buffer 1 size */
	DescSize1Shift	= 0,

/* 28:27, Tx checksum offloading control mask */
	DescTxCisMask			= 0x18000000,
	DescTxCisBypass			= 0x00000000, /* Checksum bypass */
	DescTxCisIpv4HdrCs		= 0x08000000, /* IPv4 header checksum */
/* TCP/UDP/ICMP checksum. Pseudo header checksum is assumed to be present */
	DescTxCisTcpOnlyCs		= 0x10000000,
/* TCP/UDP/ICMP checksum fully in hardware including pseudo header */
	DescTxCisTcpPseudoCs	= 0x18000000,
};
#endif

/* Rx Descriptor COE type2 encoding */
enum RxDescCOEEncode {
/* 0, IEEE 802.3 type frame Length field is Lessthan 0x0600 */
	RxLenLT600				= 0,
/* 1, Payload & Ip header checksum bypassed (unsuppported payload) */
	RxIpHdrPayLoadChkBypass	= 1,
	RxIpHdrPayLoadRes		= 2, /* 2, Reserved */
	RxChkBypass				= 3, /* 3, Checksum bypassed */
/* 4, No IPv4/IPv6 Checksum error detected */
	RxNoChkError			= 4,
/* 5, Payload checksum error detected for Ipv4/v6 frames */
	RxPayLoadChkError		= 5,
/* 6, Ip header checksum error detected for Ipv4 frames */
	RxIpHdrChkError			= 6,
/* 7, Payload & Ip header checksum error detected for Ipv4/v6 frames */
	RxIpHdrPayLoadChkError	= 7,
};

/******************************************************
 * DMA engine interrupt handling functions
 ******************************************************/
/* Intrerrupt types */
enum synopGMACDmaIntEnum {
	synopGMACDmaRxNormal   = 0x01, /* Rx normal */
	synopGMACDmaRxAbnormal = 0x02, /* Rx abnormal */
	synopGMACDmaRxStopped  = 0x04, /* Rx stopped  */
	synopGMACDmaTxNormal   = 0x08, /* Tx normal */
	synopGMACDmaTxAbnormal = 0x10, /* Tx abnormal */
	synopGMACDmaTxStopped  = 0x20, /* Tx stopped  */
	synopGMACDmaError      = 0x80, /* Dma error */
};

/******************************************************
 * Initial register values
 ******************************************************/
enum InitialRegisters {
	/* Full-duplex mode with perfect filter on */
	GmacConfigInitFdx1000 = GmacWatchdogEnable |
						GmacJabberEnable |
						GmacFrameBurstEnable |
						GmacJumboFrameDisable |
						GmacSelectGmii |
						GmacEnableRxOwn |
						GmacLoopbackOff |
						GmacFullDuplex |
						GmacRetryEnable |
						GmacPadCrcStripDisable |
						GmacBackoffLimit0 |
						GmacDeferralCheckDisable |
						GmacTxEnable | GmacRxEnable,

	/* Full-duplex mode with perfect filter on */
	GmacConfigInitFdx110 = GmacWatchdogEnable |
						GmacJabberEnable |
						GmacFrameBurstEnable |
						GmacJumboFrameDisable |
						GmacSelectMii |
						GmacEnableRxOwn |
						GmacLoopbackOff |
						GmacFullDuplex |
						GmacRetryEnable |
						GmacPadCrcStripDisable |
						GmacBackoffLimit0 |
						GmacDeferralCheckDisable |
						GmacTxEnable | GmacRxEnable,

/* Full-duplex mode:
*  CHANGED: Pass control config, dest addr filter normal, added
*  source address filter, multicast & unicast Hash filter.
*/
	GmacFrameFilterInitFdx = GmacFilterOn | GmacPassControl0 |
						GmacBroadcastEnable |
						GmacSrcAddrFilterDisable |
						GmacMulticastFilterOn |
						GmacDestAddrFilterNor |
						GmacMcastHashFilterOff |
						GmacPromiscuousModeOff |
						GmacUcastHashFilterOff,

	/* Full-duplex mode */
	GmacFlowControlInitFdx = GmacUnicastPauseFrameOff |
						GmacRxFlowControlEnable |
						GmacTxFlowControlEnable,

	/* Full-duplex mode */
	GmacGmiiAddrInitFdx    = MDC_CLOCK_DIV,

	/* Half-duplex mode with perfect filter on */
	GmacConfigInitHdx1000 = GmacWatchdogEnable |
						GmacJabberEnable |
						GmacFrameBurstEnable |
						GmacJumboFrameDisable |
						GmacSelectGmii |
						GmacDisableRxOwn |
						GmacLoopbackOff |
						GmacHalfDuplex |
						GmacRetryEnable |
						GmacPadCrcStripDisable |
						GmacBackoffLimit0 |
						GmacDeferralCheckDisable |
						GmacTxEnable |
						GmacRxEnable,

	/* Half-duplex mode with perfect filter on */
	GmacConfigInitHdx110 = GmacWatchdogEnable |
						GmacJabberEnable |
						GmacFrameBurstEnable |
						GmacJumboFrameDisable |
						GmacSelectMii |
						GmacDisableRxOwn |
						GmacLoopbackOff |
						GmacHalfDuplex |
						GmacRetryEnable |
						GmacPadCrcStripDisable |
						GmacBackoffLimit0 |
						GmacDeferralCheckDisable |
						GmacTxEnable |
						GmacRxEnable,

	/* Half-duplex mode */
	GmacFrameFilterInitHdx = GmacFilterOn |
						GmacPassControl0 |
						GmacBroadcastEnable |
						GmacSrcAddrFilterDisable |
						GmacMulticastFilterOn |
						GmacDestAddrFilterNor |
						GmacMcastHashFilterOff |
						GmacUcastHashFilterOff |
						GmacPromiscuousModeOff,

	/* Half-duplex mode */
	GmacFlowControlInitHdx = GmacUnicastPauseFrameOff |
						GmacRxFlowControlDisable |
						GmacTxFlowControlDisable,

	/* Half-duplex mode */
	GmacGmiiAddrInitHdx    = MDC_CLOCK_DIV,

/**********************************************
*DMA configurations
**********************************************/

	DmaBusModeInit = DmaFixedBurstEnable |
					DmaBurstLength8 |
					DmaDescriptorSkip6 |
					DmaResetOff,

	/* 1000 Mb/s mode */
	DmaControlInit1000 = DmaStoreAndForward,

	/* 100 Mb/s mode */
	DmaControlInit100  = DmaStoreAndForward,

	/* 10 Mb/s mode */
	DmaControlInit10   = DmaStoreAndForward,

	/* Interrupt groups */
	DmaIntErrorMask     = DmaIntBusError,    /* Error */
	DmaIntRxAbnMask     = DmaIntRxNoBuffer,  /* Rx abnormal */
	DmaIntRxNormMask    = DmaIntRxCompleted, /* Rx normal   */
	DmaIntRxStoppedMask = DmaIntRxStopped,   /* Rx stopped  */
	DmaIntTxAbnMask     = DmaIntTxUnderflow, /* Tx abnormal */
	DmaIntTxNormMask    = DmaIntTxCompleted, /* Tx normal   */
	DmaIntTxStoppedMask = DmaIntTxStopped,   /* Tx stopped  */

	DmaIntEnable  = DmaIeNormal |
					DmaIeAbnormal |
					DmaIntErrorMask |
					DmaIntRxAbnMask |
					DmaIntRxNormMask |
					DmaIntRxStoppedMask |
					DmaIntTxAbnMask |
					DmaIntTxNormMask |
					DmaIntTxStoppedMask,

	DmaIntDisable	= 0,
};

/******************************************************
 * Mac Management Counters (MMC)
 ******************************************************/
enum MMC_ENABLE {
	GmacMmcCntrl			= 0x0100,
	GmacMmcIntrRx			= 0x0104,
	GmacMmcIntrTx			= 0x0108,
	GmacMmcIntrMaskRx		= 0x010C,
	GmacMmcIntrMaskTx		= 0x0110,
};

enum MMC_TX {
	GmacMmcTxOctetCountGb	= 0x0114,
	GmacMmcTxFrameCountGb	= 0x0118,
	GmacMmcTxBcFramesG		= 0x011C,
	GmacMmcTxMcFramesG		= 0x0120,

	GmacMmcTx64OctetsGb			= 0x0124,
	GmacMmcTx65To127OctetsGb	= 0x0128,
	GmacMmcTx128To255OctetsGb	= 0x012C,
	GmacMmcTx256To511OctetsGb	= 0x0130,
	GmacMmcTx512To1023OctetsGb	= 0x0134,
	GmacMmcTx1024ToMaxOctetsGb	= 0x0138,

	GmacMmcTxUcFramesGb		= 0x013C,
	GmacMmcTxMcFramesGb		= 0x0140,
	GmacMmcTxBcFramesGb		= 0x0144,
	GmacMmcTxUnderFlowError	= 0x0148,
	GmacMmcTxSingleColG		= 0x014C,
	GmacMmcTxMultiColG		= 0x0150,
	GmacMmcTxDeferred		= 0x0154,
	GmacMmcTxLateCol		= 0x0158,
	GmacMmcTxExessCol		= 0x015C,
	GmacMmcTxCarrierError	= 0x0160,
	GmacMmcTxOctetCountG	= 0x0164,
	GmacMmcTxFrameCountG	= 0x0168,
	GmacMmcTxExessDef		= 0x016C,

	GmacMmcTxPauseFrames	= 0x0170,
	GmacMmcTxVlanFramesG	= 0x0174,
};

enum MMC_RX {
	GmacMmcRxFrameCountGb	= 0x0180,
	GmacMmcRxOctetCountGb	= 0x0184,
	GmacMmcRxOctetCountG	= 0x0188,
	GmacMmcRxBcFramesG		= 0x018C,
	GmacMmcRxMcFramesG		= 0x0190,

	GmacMmcRxCrcError		= 0x0194,
	GmacMmcRxAlignError		= 0x0198,
	GmacMmcRxRuntError		= 0x019C,
	GmacMmcRxJabberError	= 0x01A0,
	GmacMmcRxUnderSizeG		= 0x01A4,
	GmacMmcRxOverSizeG		= 0x01A8,

	GmacMmcRx64OctetsGb		= 0x01AC,
	GmacMmcRx65To127OctetsGb	= 0x01B0,
	GmacMmcRx128To255OctetsGb	= 0x01B4,
	GmacMmcRx256To511OctetsGb	= 0x01B8,
	GmacMmcRx512To1023OctetsGb	= 0x01BC,
	GmacMmcRx1024ToMaxOctetsGb	= 0x01C0,

	GmacMmcRxUcFramesG		= 0x01C4,
	GmacMmcRxLengthError	= 0x01C8,
	GmacMmcRxOutOfRangeType	= 0x01CC,

	GmacMmcRxPauseFrames	= 0x01D0,
	GmacMmcRxFifoOverFlow	= 0x01D4,
	GmacMmcRxVlanFramesGb	= 0x01D8,

	GmacMmcRxWatchdobError	= 0x01DC,
};

enum MMC_IP_RELATED {
	GmacMmcRxIpcIntrMask	= 0x0200,
	GmacMmcRxIpcIntr		= 0x0208,

	GmacMmcRxIpV4FramesG		= 0x0210,
	GmacMmcRxIpV4HdrErrFrames	= 0x0214,
	GmacMmcRxIpV4NoPayFrames	= 0x0218,
	GmacMmcRxIpV4FragFrames		= 0x021C,
	GmacMmcRxIpV4UdpChkDsblFrames	= 0x0220,

	GmacMmcRxIpV6FramesG		= 0x0224,
	GmacMmcRxIpV6HdrErrFrames	= 0x0228,
	GmacMmcRxIpV6NoPayFrames	= 0x022C,

	GmacMmcRxUdpFramesG			= 0x0230,
	GmacMmcRxUdpErrorFrames		= 0x0234,
	GmacMmcRxTcpFramesG			= 0x0238,
	GmacMmcRxTcpErrorFrames		= 0x023C,

	GmacMmcRxIcmpFramesG		= 0x0240,
	GmacMmcRxIcmpErrorFrames	= 0x0244,

	GmacMmcRxIpV4OctetsG		= 0x0250,
	GmacMmcRxIpV4HdrErrorOctets	= 0x0254,
	GmacMmcRxIpV4NoPayOctets	= 0x0258,
	GmacMmcRxIpV4FragOctets		= 0x025C,
	GmacMmcRxIpV4UdpChkDsblOctets	= 0x0260,

	GmacMmcRxIpV6OctetsG		= 0x0264,
	GmacMmcRxIpV6HdrErrorOctets	= 0x0268,
	GmacMmcRxIpV6NoPayOctets	= 0x026C,

	GmacMmcRxUdpOctetsG			= 0x0270,
	GmacMmcRxUdpErrorOctets		= 0x0274,
	GmacMmcRxTcpOctetsG			= 0x0278,
	GmacMmcRxTcpErrorOctets		= 0x027C,

	GmacMmcRxIcmpOctetsG		= 0x0280,
	GmacMmcRxIcmpErrorOctets	= 0x0284,
};

enum MMC_CNTRL_REG_BIT_DESCRIPTIONS {
	GmacMmcCounterFreeze		= 0x00000008,
	GmacMmcCounterResetOnRead	= 0x00000004,
	GmacMmcCounterStopRollover	= 0x00000002,
	GmacMmcCounterReset		= 0x00000001,

};

enum MMC_RX_INTR_MASK_AND_STATUS_BIT_DESCRIPTIONS {
	GmacMmcRxWDInt			= 0x00800000,
	GmacMmcRxVlanInt		= 0x00400000,
	GmacMmcRxFifoOverFlowInt	= 0x00200000,
	GmacMmcRxPauseFrameInt	= 0x00100000,
	GmacMmcRxOutOfRangeInt	= 0x00080000,
	GmacMmcRxLengthErrorInt	= 0x00040000,
	GmacMmcRxUcFramesInt	= 0x00020000,
	GmacMmcRx1024OctInt		= 0x00010000,
	GmacMmcRx512OctInt		= 0x00008000,
	GmacMmcRx256OctInt		= 0x00004000,
	GmacMmcRx128OctInt		= 0x00002000,
	GmacMmcRx65OctInt		= 0x00001000,
	GmacMmcRx64OctInt		= 0x00000800,
	GmacMmcRxOverSizeInt	= 0x00000400,
	GmacMmcRxUnderSizeInt	= 0x00000200,
	GmacMmcRxJabberErrorInt	= 0x00000100,
	GmacMmcRxRuntErrorInt	= 0x00000080,
	GmacMmcRxAlignErrorInt	= 0x00000040,
	GmacMmcRxCrcErrorInt	= 0x00000020,
	GmacMmcRxMcFramesInt	= 0x00000010,
	GmacMmcRxBcFramesInt	= 0x00000008,
	GmacMmcRxOctetGInt		= 0x00000004,
	GmacMmcRxOctetGbInt		= 0x00000002,
	GmacMmcRxFrameInt		= 0x00000001,
};

enum MMC_TX_INTR_MASK_AND_STATUS_BIT_DESCRIPTIONS {
	GmacMmcTxVlanInt			= 0x01000000,
	GmacMmcTxPauseFrameInt		= 0x00800000,
	GmacMmcTxExessDefInt		= 0x00400000,
	GmacMmcTxFrameInt			= 0x00200000,
	GmacMmcTxOctetInt			= 0x00100000,
	GmacMmcTxCarrierErrorInt	= 0x00080000,
	GmacMmcTxExessColInt		= 0x00040000,
	GmacMmcTxLateColInt			= 0x00020000,
	GmacMmcTxDeferredInt		= 0x00010000,
	GmacMmcTxMultiColInt		= 0x00008000,
	GmacMmcTxSingleCol			= 0x00004000,
	GmacMmcTxUnderFlowErrorInt	= 0x00002000,
	GmacMmcTxBcFramesGbInt		= 0x00001000,
	GmacMmcTxMcFramesGbInt		= 0x00000800,
	GmacMmcTxUcFramesInt		= 0x00000400,
	GmacMmcTx1024OctInt			= 0x00000200,
	GmacMmcTx512OctInt			= 0x00000100,
	GmacMmcTx256OctInt			= 0x00000080,
	GmacMmcTx128OctInt			= 0x00000040,
	GmacMmcTx65OctInt			= 0x00000020,
	GmacMmcTx64OctInt			= 0x00000010,
	GmacMmcTxMcFramesInt		= 0x00000008,
	GmacMmcTxBcFramesInt		= 0x00000004,
	GmacMmcTxFrameGbInt			= 0x00000002,
	GmacMmcTxOctetGbInt			= 0x00000001,
};

/**********************************************************
 * Power Management (PMT) Block
 **********************************************************/

/* PMT supports the reception of network (remote) wake-up frames and
* Magic packet frames. It generates interrupts for wake-up frames and
* Magic packets received by GMAC.
* PMT sits in Rx path and is enabled with remote wake-up frame enable and
* Magic packet enable. These enable are in PMT control and Status register
* and are programmed by apllication.
*
* When power down mode is enabled in PMT, all rx frames are dropped by the
* core. Core comes out of power down mode only when either Magic packe or a
* Remote wake-up frame is received and the corresponding detection is enabled.
*
* Driver need not be modified to support this feature. Only Api to put the
* device in to power down mode is sufficient.
*/

#define WAKEUP_REG_LENGTH	8	/* Length for wake up reg. */

enum GmacPmtCtrlStatusBitDefinition {
/* when set remote wake-up frame filter register pointer to 3'b000 */
	GmacPmtFrmFilterPtrReset	= 0x80000000,
/* Enables any unicast packet to be a wake-up frame */
	GmacPmtGlobalUnicast		= 0x00000200,
	GmacPmtWakeupFrameReceived	= 0x00000040, /* Rx Wake up frame */
	GmacPmtMagicPktReceived		= 0x00000020, /* Rx Magic Packet */
	GmacPmtWakeupFrameEnable	= 0x00000004, /* Wake-up frame enable */
	GmacPmtMagicPktEnable		= 0x00000002, /* Magic packet enable */
	GmacPmtPowerDown			= 0x00000001, /* Power Down */
};

/**********************************************************
 * IEEE 1588-2008 Precision Time Protocol (PTP) Support
 **********************************************************/
enum PTPMessageType {
	SYNC		= 0x0,
	Delay_Req	= 0x1,
	Pdelay_Req	= 0x2,
	Pdelay_Resp	= 0x3,
	Follow_up	= 0x8,
	Delay_Resp	= 0x9,
	Pdelay_Resp_Follow_Up	= 0xA,
	Announce	= 0xB,
	Signaling	= 0xC,
	Management	= 0xD,
};

struct time_stamp_s {
	u32 TSversion;      /* PTP Version 1 or PTP version2 */
	u32 TSmessagetype;  /* Message type associated with this time stamp */

	u16 TShighest16; /* Highest 16 bit time stamp value */
	u16 padding;     /* Padding for alignment */
	u32 TSupper32;   /* Most significant 32 bit time stamp value */
	u32 TSlower32;   /* Least Significat 32 bit time stamp value */
};

/* IEEE 1588-2008 is the optional module to support Ethernet frame
* time stamping. Sixty four (+16) bit time stamps are given in each
* frames transmit and receive status.
* The driver assumes the following IEEE 1588
* 1. "Time Stamping" "TIME_STAMPING"is ENABLED
* 2. "External Time Stamp Input Enable" "EXT_TIME_STAMPING" is DISABLED
* 3. "Advanced Time Stamp support" "ADV_TIME_STAMPING" is ENABLED
* 4. "Higher Word Register Enable" "ADV_TIME_HIGH_WORD" is ENABLED
*/

/* GmacTSControl = 0x0700, Controls the Timestamp update logic: only when
*  IEEE 1588 time stamping is enabled in corekit.
*/
enum GmacTSControlReg {
	GmacTSENMACADDR = 0x00040000, /* Enable Mac Addr for PTP filtering */

	GmacTSCLKTYPE   = 0x00030000, /* Select the type of clock node */

/*TSCLKTYPE TSMSTRENA TSEVNTENA  Messages for wihich TS snapshot is taken
* 00/01         X        0        SYNC, FOLLOW_UP, DELAY_REQ, DELAY_RESP
* 00/01         1        0        DELAY_REQ
* 00/01         0        1        SYNC
* 10            NA       0        SYNC, FOLLOW_UP, DELAY_REQ, DELAY_RESP
* 10            NA       1        SYNC, FOLLOW_UP
* 11            NA       0        SYNC, FOLLOW_UP, DELAY_REQ, DELAY_RESP,
*                                       PDELAY_REQ, PDELAY_RESP
* 11            NA       1        SYNC, PDELAY_REQ, PDELAY_RESP
*/
	GmacTSOrdClk  = 0x00000000, /* 00=> Ordinary clock */
	GmacTSBouClk  = 0x00010000, /* 01=> Boundary clock */
	GmacTSEtoEClk = 0x00020000, /* 10=> End-to-End transparent clock */
	GmacTSPtoPClk = 0x00030000, /* 11=> P-to-P transparent clock */

	GmacTSMSTRENA = 0x00008000, /* Ena TS Snapshot for Master Messages */
	GmacTSEVNTENA = 0x00004000, /* Ena TS Snapshot for Event Messages */
	GmacTSIPV4ENA = 0x00002000, /* Ena TS snapshot for IPv4 */
	GmacTSIPV6ENA = 0x00001000, /* Ena TS snapshot for IPv6 */
	GmacTSIPENA   = 0x00000800, /* Ena TS snapshot for PTP over E'net */
	GmacTSVER2ENA = 0x00000400, /* Ena PTP snooping for version 2 */

	GmacTSCTRLSSR = 0x00000200, /* Digital or Binary Rollover */

	GmacTSENALL   = 0x00000100, /* Enable TS fro all frames (Ver2 only) */

	GmacTSADDREG  = 0x00000020, /* Addend Register Update */
	GmacTSUPDT    = 0x00000008, /* Time Stamp Update */
	GmacTSINT     = 0x00000004, /* Time Atamp Initialize */

	GmacTSTRIG    = 0x00000010, /* Time stamp interrupt Trigger Enable */

	GmacTSCFUPDT      = 0x00000002, /* Time Stamp Fine/Coarse */
	GmacTSCUPDTCoarse = 0x00000000, /* Time Stamp update method is coarse */
	GmacTSCUPDTFine   = 0x00000002, /* Time Stamp update method is fine */

	GmacTSENA = 0x00000001, /* Time Stamp Enable */
};

/* GmacTSSubSecIncr = 0x0704, 8 bit value by which sub second register is
*  incremented: only when IEEE 1588 time stamping without
*  external timestamp input.
*/
enum GmacTSSubSecIncrReg {
	GmacSSINCMsk = 0x000000FF, /* Only Lower 8 bits are valid bits */
};

/* GmacTSLow = 0x070C, Indicates whether the timestamp low count is
*  positive or negative; for Adv timestamp it is always zero.
*/
enum GmacTSSign {
	GmacTSSign     = 0x80000000, /* PSNT */
	GmacTSPositive = 0x00000000,
	GmacTSNegative = 0x80000000,
};

/* GmacTargetTimeLow = 0x0718, 32 bit nano seconds(MS) to be compared
*  with system time: only when IEEE 1588 time stamping without
*  external timestamp input.
*/
enum GmacTSLowReg {
/* When TSCTRLSSR is set the max value for GmacTargetTimeLowReg and
*  GmacTimeStampLow register is 0x3B9AC9FF at 1ns precision.
*/
	GmacTSDecThr = 0x3B9AC9FF,
};

/* GmacTSHighWord = 0x0724, Time Stamp Higher Word Register. */
enum GmacTSHighWordReg {
	GmacTSHighWordMask = 0x0000FFFF, /* Has only lower 16 bits valid. */
};

/* GmacTSStatus	= 0x0728, Time Stamp Status Register. */
enum GmacTSStatusReg {
	GmacTSTargTimeReached = 0x00000002, /* Target Time Reached. */
	GmacTSSecondsOverflow = 0x00000001, /* Seconds Overflow. */
};

/**************************************
 * Time stamp related functions
 **************************************/
void synopGMAC_TS_enable(struct synop_device *gmacdev);
void synopGMAC_TS_disable(struct synop_device *gmacdev);

void synopGMAC_TS_int_enable(struct synop_device *gmacdev);
void synopGMAC_TS_int_disable(struct synop_device *gmacdev);

void synopGMAC_TS_mac_addr_filt_enable(struct synop_device *gmacdev);
void synopGMAC_TS_mac_addr_filt_disable(struct synop_device *gmacdev);
void synopGMAC_TS_set_clk_type(struct synop_device *gmacdev, u32 clk_type);
void synopGMAC_TS_master_enable(struct synop_device *gmacdev);
void synopGMAC_TS_master_disable(struct synop_device *gmacdev);
void synopGMAC_TS_event_enable(struct synop_device *gmacdev);
void synopGMAC_TS_event_disable(struct synop_device *gmacdev);
void synopGMAC_TS_IPV4_enable(struct synop_device *gmacdev);
void synopGMAC_TS_IPV4_disable(struct synop_device *gmacdev);
void synopGMAC_TS_IPV6_enable(struct synop_device *gmacdev);
void synopGMAC_TS_IPV6_disable(struct synop_device *gmacdev);
void synopGMAC_TS_ptp_over_ethernet_enable(struct synop_device *gmacdev);
void synopGMAC_TS_ptp_over_ethernet_disable(struct synop_device *gmacdev);
void synopGMAC_TS_pkt_snoop_ver2(struct synop_device *gmacdev);
void synopGMAC_TS_pkt_snoop_ver1(struct synop_device *gmacdev);

void synopGMAC_TS_digital_rollover_enable(struct synop_device *gmacdev);
void synopGMAC_TS_binary_rollover_enable(struct synop_device *gmacdev);
void synopGMAC_TS_all_frames_enable(struct synop_device *gmacdev);
void synopGMAC_TS_all_frames_disable(struct synop_device *gmacdev);

s32 synopGMAC_TS_addend_update(struct synop_device *gmacdev, u32 addend_value);
s32 synopGMAC_TS_timestamp_update(struct synop_device *gmacdev,
				  u32 high_value, u32 low_value);
s32 synopGMAC_TS_timestamp_init(struct synop_device *gmacdev,
				u32 high_value, u32 low_value);

void synopGMAC_TS_coarse_update(struct synop_device *gmacdev);
void synopGMAC_TS_fine_update(struct synop_device *gmacdev);

void synopGMAC_TS_subsecond_init(struct synop_device *gmacdev,
				 u32 sub_sec_inc_val);
void synopGMAC_TS_read_timestamp(
	struct synop_device *gmacdev, u16 *higher_sec_val,
	u32 *sec_val, u32 *sub_sec_val);
void synopGMAC_TS_load_target_timestamp(struct synop_device *gmacdev,
					u32 sec_val, u32 sub_sec_val);

void synopGMAC_TS_load_timestamp_higher_val(struct synop_device *gmacdev,
					    u32 higher_sec_val);
void synopGMAC_TS_read_timestamp_higher_val(struct synop_device *gmacdev,
					    u16 *higher_sec_val);
void synopGMAC_TS_read_target_timestamp(struct synop_device *gmacdev,
					u32 *sec_val, u32 *sub_sec_val);

/**********************************************************
 * Common functions
 **********************************************************/
/* s32 ioremap_read(u32 address); */
s32 ioremap_write(u32 address, u32 value);
s32 ioremap_AND_value(u32 address, u32 and_value);
s32 ioremap_OR_value(u32 address, u32 or_value);
s32 synopGMAC_set_mdc_clk_div(struct synop_device *gmacdev, u32 clk_div_val);
u32 synopGMAC_get_mdc_clk_div(struct synop_device *gmacdev);
s32 synopGMAC_read_phy_reg(unsigned long *RegBase,
					u32 PhyBase, u32 RegOffset, u16 *data);
s32 synopGMAC_write_phy_reg(unsigned long *RegBase,
					u32 PhyBase, u32 RegOffset, u16 data);
s32 synopGMAC_phy_loopback(struct synop_device *gmacdev, bool loopback);
s32 synopGMAC_read_version(struct synop_device *gmacdev);
s32 synopGMAC_reset(struct synop_device *gmacdev);
s32 synopGMAC_dma_bus_mode_init(struct synop_device *gmacdev, u32 init_value);
s32 synopGMAC_dma_control_init(struct synop_device *gmacdev, u32 init_value);
void synopGMAC_wd_enable(struct synop_device *gmacdev);
void synopGMAC_wd_disable(struct synop_device *gmacdev);
void synopGMAC_jab_enable(struct synop_device *gmacdev);
void synopGMAC_jab_disable(struct synop_device *gmacdev);
void synopGMAC_frame_burst_enable(struct synop_device *gmacdev);
void synopGMAC_frame_burst_disable(struct synop_device *gmacdev);
void synopGMAC_jumbo_frame_enable(struct synop_device *gmacdev);
void synopGMAC_jumbo_frame_disable(struct synop_device *gmacdev);
void synopGMAC_disable_crs(struct synop_device *gmacdev);
void synopGMAC_select_gmii(struct synop_device *gmacdev);
void synopGMAC_select_mii(struct synop_device *gmacdev);
void synopGMAC_rx_own_enable(struct synop_device *gmacdev);
void synopGMAC_rx_own_disable(struct synop_device *gmacdev);
void synopGMAC_loopback_on(struct synop_device *gmacdev);
void synopGMAC_loopback_off(struct synop_device *gmacdev);
void synopGMAC_set_full_duplex(struct synop_device *gmacdev);
void synopGMAC_set_half_duplex(struct synop_device *gmacdev);
void synopGMAC_retry_enable(struct synop_device *gmacdev);
void synopGMAC_retry_disable(struct synop_device *gmacdev);
void synopGMAC_pad_crc_strip_enable(struct synop_device *gmacdev);
void synopGMAC_pad_crc_strip_disable(struct synop_device *gmacdev);
void synopGMAC_back_off_limit(struct synop_device *gmacdev, u32 value);
void synopGMAC_deferral_check_enable(struct synop_device *gmacdev);
void synopGMAC_deferral_check_disable(struct synop_device *gmacdev);
void synopGMAC_rx_enable(struct synop_device *gmacdev);
void synopGMAC_rx_disable(struct synop_device *gmacdev);
void synopGMAC_tx_enable(struct synop_device *gmacdev);
void synopGMAC_tx_disable(struct synop_device *gmacdev);
void synopGMAC_frame_filter_enable(struct synop_device *gmacdev);
void synopGMAC_frame_filter_disable(struct synop_device *gmacdev);

void synopGMAC_mac_addr_setFilter_enable(struct synop_device *gmacdev);
void synopGMAC_mac_addr_setFilter_disable(struct synop_device *gmacdev);

void synopGMAC_write_hash_table_high(struct synop_device *gmacdev, u32 data);
void synopGMAC_write_hash_table_low(struct synop_device *gmacdev, u32 data);
void synopGMAC_hash_perfect_filter_enable(struct synop_device *gmacdev);
void synopGMAC_Hash_filter_only_enable(struct synop_device *gmacdev);
void synopGMAC_src_addr_filter_enable(struct synop_device *gmacdev);
void synopGMAC_src_addr_filter_disable(struct synop_device *gmacdev);

void synopGMAC_src_addr_filter_inverse(struct synop_device *gmacdev);
void synopGMAC_src_addr_filter_normal(struct synop_device *gmacdev);

void synopGMAC_dst_addr_filter_inverse(struct synop_device *gmacdev);
void synopGMAC_dst_addr_filter_normal(struct synop_device *gmacdev);
void synopGMAC_set_pass_control(struct synop_device *gmacdev, u32 passcontrol);
void synopGMAC_broadcast_enable(struct synop_device *gmacdev);
void synopGMAC_broadcast_disable(struct synop_device *gmacdev);
void synopGMAC_multicast_enable(struct synop_device *gmacdev);
void synopGMAC_multicast_disable(struct synop_device *gmacdev);
void synopGMAC_multicast_hash_filter_enable(struct synop_device *gmacdev);
void synopGMAC_multicast_hash_filter_disable(struct synop_device *gmacdev);
void synopGMAC_promisc_enable(struct synop_device *gmacdev);
void synopGMAC_promisc_disable(struct synop_device *gmacdev);
void synopGMAC_unicast_hash_filter_enable(struct synop_device *gmacdev);
void synopGMAC_unicast_hash_filter_disable(struct synop_device *gmacdev);
void synopGMAC_unicast_pause_frame_detect_enable(struct synop_device *gmacdev);
void synopGMAC_unicast_pause_frame_detect_disable(struct synop_device *gmacdev);
void synopGMAC_rx_flow_control_enable(struct synop_device *gmacdev);
void synopGMAC_rx_flow_control_disable(struct synop_device *gmacdev);
void synopGMAC_tx_flow_control_enable(struct synop_device *gmacdev);
void synopGMAC_tx_flow_control_disable(struct synop_device *gmacdev);
void synopGMAC_tx_activate_flow_control(struct synop_device *gmacdev);
void synopGMAC_tx_deactivate_flow_control(struct synop_device *gmacdev);
void synopGMAC_pause_control(struct synop_device *gmacdev);
void synopGMAC_fast_ethernet_speed(struct synop_device *gmacdev, u32 speed);

s32 synopGMAC_mac_init(struct synop_device *gmacdev, u32 promisc);

s32 synopGMAC_check_phy_init(struct synop_device *gmacdev);
s32 synopGMAC_set_mac_addr(struct synop_device *gmacdev,
			   u32 MacHigh, u32 MacLow, u8 *MacAddr);
s32 synopGMAC_get_mac_addr(struct synop_device *gmacdev,
			   u32 MacHigh, u32 MacLow, u8 *MacAddr);
s32 synopGMAC_attach(struct synop_device *gmacdev,
		     u32 macBase, u32 dmaBase, u32 phyBase);

void synopGMAC_rx_desc_init_ring(struct dma_desc *desc, bool last_ring_desc);
void synopGMAC_tx_desc_init_ring(struct dma_desc *desc, bool last_ring_desc);
void synopGMAC_rx_desc_init_chain(struct dma_desc *desc);
void synopGMAC_tx_desc_init_chain(struct dma_desc *desc);
s32 synopGMAC_init_tx_rx_desc_queue(struct synop_device *gmacdev);
void synopGMAC_init_rx_desc_base(struct synop_device *gmacdev);
void synopGMAC_init_tx_desc_base(struct synop_device *gmacdev);
void synopGMAC_set_owner_dma(struct dma_desc *desc);
void synopGMAC_set_desc_sof(struct dma_desc *desc);
void synopGMAC_set_desc_eof(struct dma_desc *desc);
bool synopGMAC_is_sof_in_rx_desc(struct dma_desc *desc);
bool synopGMAC_is_eof_in_rx_desc(struct dma_desc *desc);
bool synopGMAC_is_da_filter_failed(struct dma_desc *desc);
bool synopGMAC_is_sa_filter_failed(struct dma_desc *desc);
bool synopGMAC_is_desc_owned_by_dma(struct dma_desc *desc);
u32 synopGMAC_get_rx_desc_frame_length(u32 status);
bool synopGMAC_is_desc_valid(unsigned long status);
bool synopGMAC_is_desc_empty(struct dma_desc *desc);
bool synopGMAC_is_rx_desc_valid(u32 status);
bool synopGMAC_is_tx_aborted(u32 status);
bool synopGMAC_is_tx_carrier_error(u32 status);
u32 synopGMAC_get_tx_collision_count(u32 status);
u32 synopGMAC_is_exc_tx_collisions(u32 status);
bool synopGMAC_is_rx_frame_damaged(u32 status);
bool synopGMAC_is_rx_frame_collision(u32 status);
bool synopGMAC_is_rx_crc(u32 status);
bool synopGMAC_is_frame_dribbling_errors(u32 status);
bool synopGMAC_is_rx_frame_length_errors(u32 status);
bool synopGMAC_is_last_rx_desc(
	struct synop_device *gmacdev, struct dma_desc *desc);
bool synopGMAC_is_last_tx_desc(
	struct synop_device *gmacdev, struct dma_desc *desc);
bool synopGMAC_is_rx_desc_chained(struct dma_desc *desc);
bool synopGMAC_is_tx_desc_chained(struct dma_desc *desc);

#ifndef CONFIG_VD_RELEASE
void synopGMAC_dbg_print_rxdes_addr(struct synop_device *gmacdev, u32 index);
void synopGMAC_dbg_print_txdes_addr(struct synop_device *gmacdev, u32 index);
#endif

void synopGMAC_get_desc_data(struct dma_desc *desc,
		u32 *Status,
		u32 *Buffer1, u32 *Length1,
		unsigned long *Data1,
		u32 *Buffer2, u32 *Length2,
		unsigned long *Data2);

#ifdef ENH_DESC_8W
s8 synopGMAC_get_tx_qptr(struct synop_device *gmacdev, u32 *Status,
			 u32 *Buffer1, u32 *Length1, u32 *Data1,
	u32 *Buffer2, u32 *Length2, u32 *Data2,
	u32 *Ext_Status,
	u32 *Time_Stamp_High, u32 *Time_Stamp_low,
	u32 *now_desc);
#else
s8 synopGMAC_get_tx_qptr(struct synop_device *gmacdev,
			u32 *Status,
			u32 *Buffer1, u32 *Length1,
			unsigned long *Data1,
			u32 *Buffer2, u32 *Length2,
			unsigned long *Data2,
			unsigned long *now_desc);
#endif

s8 synopGMAC_set_tx_qptr(struct synop_device *gmacdev,
			u32 Buffer1, u32 Length1,
			unsigned long Data1,
			u32 Buffer2, u32 Length2,
			unsigned long Data2,
			u32 offload_needed,
			unsigned long *now_desc);

s8 synopGMAC_set_rx_qptr(struct synop_device *gmacdev,
			u32 Buffer1, u32 Length1,
			unsigned long Data1,
			u32 Buffer2, u32 Length2,
			unsigned long Data2,
			unsigned long *now_desc);

#ifdef ENH_DESC_8W
s8 synopGMAC_get_rx_qptr(struct synop_device *gmacdev, u32 *Status,
			 u32 *Buffer1, u32 *Length1, u32 *Data1,
	u32 *Buffer2, u32 *Length2, u32 *Data2,
	u32 *Ext_Status,
	u32 *Time_Stamp_High, u32 *Time_Stamp_low,
	u32 *now_desc);
#else
s8 synopGMAC_get_rx_qptr(struct synop_device *gmacdev,
			u32 *Status,
			u32 *Buffer1, u32 *Length1,
			unsigned long *Data1,
			u32 *Buffer2, u32 *Length2,
			unsigned long *Data2,
			unsigned long *now_desc);
#endif

void synopGMAC_clear_interrupt(struct synop_device *gmacdev);
u32 synopGMAC_get_interrupt_type(struct synop_device *gmacdev);
u32 synopGMAC_get_interrupt_mask(struct synop_device *gmacdev);
void synopGMAC_enable_interrupt(struct synop_device *gmacdev, u32 interrupts);
void synopGMAC_disable_interrupt_all(struct synop_device *gmacdev);
void synopGMAC_disable_interrupt(struct synop_device *gmacdev, u32 interrupts);
void synopGMAC_enable_dma_rx(struct synop_device *gmacdev);
void synopGMAC_enable_dma_tx(struct synop_device *gmacdev);
void synopGMAC_resume_dma_tx(struct synop_device *gmacdev);
void synopGMAC_resume_dma_rx(struct synop_device *gmacdev);
void synopGMAC_take_desc_ownership(struct dma_desc *desc);
void synopGMAC_take_desc_ownership_rx(struct synop_device *gmacdev);
void synopGMAC_take_desc_ownership_tx(struct synop_device *gmacdev);
void synopGMAC_disable_dma_tx(struct synop_device *gmacdev);
void synopGMAC_disable_dma_rx(struct synop_device *gmacdev);
u32 synopGMAC_get_dma_current_TX_desc(struct synop_device *gmacdev);
u32 synopGMAC_get_dma_current_RX_desc(struct synop_device *gmacdev);

/* Following APIs only for Enhanced Descriptor from 3.50a release onwards. */
bool synopGMAC_is_ext_status(struct synop_device *gmacdev, u32 status);
bool synopGMAC_ES_is_IP_header_error(
	struct synop_device *gmacdev, u32 ext_status);
bool synopGMAC_ES_is_rx_checksum_bypassed(struct synop_device *gmacdev,
					  u32 ext_status);
bool synopGMAC_ES_is_IP_payload_error(
	struct synop_device *gmacdev, u32 ext_status);

/* PMT APIs. */
void synopGMAC_pmt_int_enable(struct synop_device *gmacdev);
void synopGMAC_pmt_int_disable(struct synop_device *gmacdev);
void synopGMAC_power_down_enable(struct synop_device *gmacdev);
void synopGMAC_power_down_disable(struct synop_device *gmacdev);
void synopGMAC_enable_pmt_interrupt(struct synop_device *gmacdev);
void synopGMAC_disable_pmt_interrupt(struct synop_device *gmacdev);
void synopGMAC_magic_packet_enable(struct synop_device *gmacdev);
void synopGMAC_wakeup_frame_enable(struct synop_device *gmacdev);
void synopGMAC_pmt_unicast_enable(struct synop_device *gmacdev);
bool synopGMAC_is_magic_packet_received(struct synop_device *gmacdev);
bool synopGMAC_is_wakeup_frame_received(struct synop_device *gmacdev);
void synopGMAC_write_wakeup_frame_register(struct synop_device *gmacdev,
					   u32 *filter_contents);

/* MMC APIs. */
void synopGMAC_mmc_counters_stop(struct synop_device *gmacdev);
void synopGMAC_mmc_counters_resume(struct synop_device *gmacdev);
void synopGMAC_mmc_counters_set_selfclear(struct synop_device *gmacdev);
void synopGMAC_mmc_counters_reset_selfclear(struct synop_device *gmacdev);
void synopGMAC_mmc_counters_disable_rollover(struct synop_device *gmacdev);
void synopGMAC_mmc_counters_enable_rollover(struct synop_device *gmacdev);
u32 synopGMAC_read_mmc_counter(struct synop_device *gmacdev, u32 counter);
u32 synopGMAC_read_mmc_rx_int_status(struct synop_device *gmacdev);
u32 synopGMAC_read_mmc_tx_int_status(struct synop_device *gmacdev);
void synopGMAC_disable_mmc_tx_interrupt(struct synop_device *gmacdev, u32 mask);
void synopGMAC_enable_mmc_tx_interrupt(struct synop_device *gmacdev, u32 mask);
void synopGMAC_disable_mmc_rx_interrupt(struct synop_device *gmacdev, u32 mask);
void synopGMAC_enable_mmc_rx_interrupt(struct synop_device *gmacdev, u32 mask);
void synopGMAC_enable_mmc_ipc_rx_interrupt(
	struct synop_device *gmacdev, u32 mask);
void synopGMAC_disable_mmc_ipc_rx_interrupt(
	struct synop_device *gmacdev, u32 mask);

/* IP checksum offloading APIs. */
void synopGMAC_enable_rx_chksum_offload(struct synop_device *gmacdev);
void synopGMAC_disable_rx_Ipchecksum_offload(struct synop_device *gmacdev);
void synopGMAC_rx_tcpip_chksum_drop_enable(struct synop_device *gmacdev);
void synopGMAC_rx_tcpip_chksum_drop_disable(struct synop_device *gmacdev);
u32  synopGMAC_is_rx_checksum_error(struct synop_device *gmacdev, u32 status);
bool synopGMAC_is_tx_ipv4header_checksum_error(struct synop_device *gmacdev,
					       u32 status);
bool synopGMAC_is_tx_payload_checksum_error(struct synop_device *gmacdev,
					    u32 status);
void synopGMAC_tx_checksum_offload_bypass(struct synop_device *gmacdev,
					  struct dma_desc *desc);
void synopGMAC_tx_checksum_offload_ipv4hdr(struct synop_device *gmacdev,
					   struct dma_desc *desc);
void synopGMAC_tx_checksum_offload_tcponly(struct synop_device *gmacdev,
					   struct dma_desc *desc);
void synopGMAC_tx_checksum_offload_tcp_pseudo(struct synop_device *gmacdev,
					      struct dma_desc *desc);

/* System board dependent functions. */
s32 synopGMAC_reset_phy(struct synop_device *gmacdev);
u32 synopGMAC_get_phy_id(struct synop_device *gmacdev);
void synopGMAC_restart_auto_nego(struct synop_device *gmacdev);

#ifdef CONFIG_NVT_RTL_PHY_SUPPORT
/* For 8201 EL */
s32 synopGMAC_get_phy_test_reg(struct synop_device *gmacdev);
s32 synopGMAC_get_phy_rxer_reg(struct synop_device *gmacdev);
void synopGMAC_get_phy_LBREM_reg(struct synop_device *gmacdev);

u16 synopGMAC_get_phy_ctrl_reg(struct synop_device *gmacdev);
s32 synopGMAC_set_phy_page_reg(struct synop_device *gmacdev, u8 page);

/* For 8201 F */
u16 synopGMAC_RTL8201F_get_phy_RMII_mode_setting_reg(
	struct synop_device *gmacdev);
s32 synopGMAC_RTL8201F_set_phy_TXC_direction(struct synop_device *gmacdev,
					     u8 input);
s32 synopGMAC_RTL8201F_set_phy_CRS_RXDV_pin(struct synop_device *gmacdev,
					    u8 pin_CRS);
u16 synopGMAC_RTL8201F_get_phy_fibermode_loopback_reg(
	struct synop_device *gmacdev);
u16 synopGMAC_RTL8201F_get_phy_interrupt_status(struct synop_device *gmacdev);
s32 synopGMAC_RTL8201F_set_phy_adjust_RMII_TX_timing(
	struct synop_device *gmacdev,
	u8 timing);
s32 synopgmac_rtl8201f_set_phy_rmii_rx_timing(struct synop_device *gmacdev,
					      u8 timing);
#endif

s32 synopGMAC_set_phy_speed(struct synop_device *gmacdev, u8 speed10);
s32 synopGMAC_check_phy_init_for_10_100M(struct synop_device *gmacdev);

/* for phy debug only */
s32 synopGMAC_set_phy_in_100M(struct synop_device *gmacdev);
s32 synopGMAC_set_rtl_phy_in_100M(struct synop_device *gmacdev);

extern unsigned long g_MDC_CLOCK;

#endif /* End of file */
