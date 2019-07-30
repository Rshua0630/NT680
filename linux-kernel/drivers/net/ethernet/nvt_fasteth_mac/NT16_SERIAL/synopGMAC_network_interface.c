/** \file
 * This is the network dependent layer
 * to handle network related functionality.
 * This file is tightly coupled to neworking frame work
 * of linux 2.6.xx kernel.
 * The functionality carried out in this file should be treated
 * as an example only
 * if the underlying operating system is not Linux.
 *
 * \note Many of the functions other
 * than the device specific functions
 *  changes for operating system other than Linux 2.6.xx
 * \internal
 *-------------REVISION HISTORY-------------------
 * Synopsys		01/Aug/2007		Created
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/delay.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include <linux/io.h>

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

/*for socket ioctl lists*/
#include <linux/sockios.h>
#include <linux/mii.h>

#include <linux/spinlock.h>
#include <linux/interrupt.h>

/*USE_DEVICE_TREE*/
#include <linux/of_platform.h>
#include <linux/platform_device.h>

#include "synopGMAC_Host.h"
#include "synopGMAC_plat.h"
#include "synopGMAC_network_interface.h"
#include "synopGMAC_Dev.h"
#include "m_internal_Phy.h"

#define DEBUG_READ_PHY_REG      0

#define NVT_PHY_RESET_GPIO_ADDR 0xfd100400
#define NVT_PHY_RESET_GPIO_BIT	21

#define IOCTL_READ_REGISTER		(SIOCDEVPRIVATE + 1)
#define IOCTL_WRITE_REGISTER	(SIOCDEVPRIVATE + 2)
#define IOCTL_READ_IPSTRUCT		(SIOCDEVPRIVATE + 3)
#define IOCTL_READ_RXDESC		(SIOCDEVPRIVATE + 4)
#define IOCTL_READ_TXDESC		(SIOCDEVPRIVATE + 5)
#define IOCTL_POWER_DOWN		(SIOCDEVPRIVATE + 6)
#define IOCTL_CHK_CONN			(SIOCDEVPRIVATE + 7)
#define IOCTL_PHY_LOOPBACK_SETUP		(SIOCDEVPRIVATE + 8)
#define IOCTL_SS_SELF_TEST_MODE_ON_OFF	(SIOCDEVPRIVATE + 9)
#define IOCTL_SS_SELF_TEST_SEND			(SIOCDEVPRIVATE + 10)
#define IOCTL_SS_SELF_TEST_GET_RESULT	(SIOCDEVPRIVATE + 11)
#define IOCTL_SS_SELF_TEST_SET_FRAME	(SIOCDEVPRIVATE + 12)
#define IOCTL_RESET_PHY					(SIOCDEVPRIVATE + 13)
#define IOCTL_RESET_MAC					(SIOCDEVPRIVATE + 14)
#define IOCTL_SHOW_DMA_STATUS			(SIOCDEVPRIVATE + 15)
#define GMAC_IRQ	74

struct net_device          *synopGMACnetdev;

static struct timer_list synopGMAC_cable_unplug_timer;

/*This global variable is used to indicate the ISR
* whether the interrupts occurred in the process
* of powering down the mac or not
*/
static u32 GMAC_Power_down;

/*checkpatch.pl, do not initialise statics to 0 or NULL*/
static u32 do_suspend;

#if DEBUG_READ_PHY_REG
u32 *internal_PHY_OFFSET_0120;
u32 *internal_PHY_OFFSET_012C;
u32 *internal_PHY_OFFSET_0130;
#endif

/*Save the current mac address*/
static u8 Mac_Addr[6];

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
#define NVT_NAPI_WEIGHT 64
/*1 -> napi poll running, otherwise stop*/
atomic_t napi_poll = ATOMIC_INIT(0);
static struct napi_struct nvt_napi;
#endif

static void nvt_wq_detect_linkstatus(struct work_struct *ignored);
static DECLARE_WORK(priv_work, nvt_wq_detect_linkstatus);
static struct workqueue_struct *plugDetect_Wq;

/*Sample Wake-up frame filter configurations*/
u32 synopGMAC_wakeup_filter_config0[] = {
	/* For Filter0 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/* For Filter1 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/* For Filter2 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/* For Filter3 CRC is based on 0,1,2,3,4,6,8,9,10,11,12,
	* 14,16,17,18,19,20,22,24,25,26,27,28,30 bytes from offset
	*/
	0x5F5F5F5F,
	/* Filter 0,1,2 are disabled, Filter3 is enabled
	* and filtering applies to only multicast packets
	*/
	0x09000000,
	/* Filter 0,1,2 (no significance), filter 3 offset
	* is 28 bytes from start of Destination MAC address
	*/
	0x1C000000,
	/* No significance of CRC for Filter0 and Filter1*/
	0x00000000,
	/* No significance of CRC for Filter2, Filter3 CRC is 0xBDCC*/
	0xBDCC0000
};

u32 synopGMAC_wakeup_filter_config1[] = {
	/*For Filter0 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*For Filter1 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*For Filter2 CRC is based on 1,3,4,5,6,9,11,12,13,14,17,
	* 19,20,21,25,27,28,29,30 bytes from offset
	*/
	0x7A7A7A7A,
	/*For Filter3 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*Filter 0,1,3 are disabled, Filter2 is enabled and filtering
	* applies to only unicast packets
	*/
	0x00010000,
	/*Filter 0,1,3 (no significance),
	* filter 2 offset is 16 bytes from start of Destination MAC address
	*/
	0x00100000,
	/*No significance of CRC for Filter0 and Filter1*/
	0x00000000,
	/*No significance of CRC for Filter3, Filter2 CRC is 0xA0FE*/
	0x0000A0FE
};

u32 synopGMAC_wakeup_filter_config2[] = {
	/*For Filter0 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*For Filter1 CRC is computed on 0,1,2,3,4,5,6,7 bytes from offset*/
	0x000000FF,
	/*For Filter2 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*For Filter3 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*Filter 0,2,3 are disabled, Filter 1 is enabled
	* and filtering applies to only unicast packets
	*/
	0x00000100,
	/*Filter 0,2,3 (no significance),
	* filter 1 offset is 223 bytes from start
	* of Destination MAC address
	*/
	0x0000DF00,
	/*No significance of CRC for Filter0, Filter1 CRC is 0xDB9E*/
	0xDB9E0000,
	/*No significance of CRC for Filter2 and Filter3*/
	0x00000000
};

/**
* The synopGMAC_wakeup_filter_config3[]
* is a sample configuration for wake up filter.
* Filter1 is used here
* Filter1 offset is programmed to 50 (0x32)
* Filter1 mask is set to 0x000000FF,
* indicating First 8 bytes are used by the filter
* Filter1 CRC= 0x7EED this is the CRC computed on data
* 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55
* Refer accompanied software DWC_gmac_crc_example.c
* for CRC16 generation and how to use the same.
*/

u32 synopGMAC_wakeup_filter_config3[] = {
	/*For Filter0 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*For Filter1 CRC is computed on 0,1,2,3,4,5,6,7 bytes from offset*/
	0x000000FF,
	/*For Filter2 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*For Filter3 CRC is not computed may be it is 0x0000*/
	0x00000000,
	/*Filter 0,2,3 are disabled, Filter 1 is enabled
	* and filtering applies to only unicast packets
	*/
	0x00000100,
	/*Filter 0,2,3 (no significance),
	* filter 1 offset is 50 bytes from start
	* of Destination MAC address
	*/
	0x00003200,
	/*No significance of CRC for Filter0, Filter1 CRC is 0x7EED,*/
	0x7eED0000,
	/*No significance of CRC for Filter2 and Filter3*/
	0x00000000
};

/* Buffer point for SS self loopback test */
void *SS_selftest_frame;
dma_addr_t SS_selftest_frame_phys;
u32 *external_PHY_reset_dir;
u32 *external_PHY_reset_pull_low;
u32 *external_PHY_reset_pull_high;
u32 inRESET;

/**
* Checkpatch not allow to print error message, but it is important.
*/
void show_skb_alloc_error(void)
{
	pr_crit("SKB memory allocation failed\n");
}

/**
 * Function used to detect the cable plugging and unplugging.
 * This function gets scheduled once in every second and polls
 * the PHY register for network cable plug/unplug. Once the
 * connection is back the GMAC device is configured as per
 * new Duplex mode and Speed of the connection.
 * @param[in] u32 type but is not used currently.
 * \return returns void.
 * \note This function is tightly coupled with Linux 2.6.xx.
 * \callgraph
 */

static void synopGMAC_linux_cable_unplug_function(u32 notused)
{
	struct synop_adapter_struct *adapter =
		(struct synop_adapter_struct *)
		synopGMAC_cable_unplug_timer.data;

	if (NVT_Phy_Init_Done > 0)
		queue_work(plugDetect_Wq, &priv_work);

	if (timer_pending(&synopGMAC_cable_unplug_timer)) {
		del_timer(&synopGMAC_cable_unplug_timer);
		pr_crit("[NTKETHMAC]: Error-Timer is pending.\n");
	}

	init_timer(&synopGMAC_cable_unplug_timer);

	synopGMAC_cable_unplug_timer.function =
		(void *)synopGMAC_linux_cable_unplug_function;

	synopGMAC_cable_unplug_timer.data = (unsigned long)adapter;
	synopGMAC_cable_unplug_timer.expires = jiffies + (HZ / 2);

	add_timer(&synopGMAC_cable_unplug_timer);
}

static void synopGMAC_linux_powerdown_mac(struct synop_device *gmacdev)
{
	TR0("Put the GMAC to power down mode..\n");
	/*Disable the Dma engines in tx path*/

	/*Let ISR know that Mac is going to be in the power down mode*/
	GMAC_Power_down = 1;
	synopGMAC_disable_dma_tx(gmacdev);

	/*allow any pending transmission to complete*/
	plat_delay(10000);

	/* Disable the Mac for both tx and rx*/
	synopGMAC_tx_disable(gmacdev);
	synopGMAC_rx_disable(gmacdev);

	/*Allow any pending buffer to be read by host*/
	plat_delay(10000);

	/*Disable the Dma in rx path*/
	synopGMAC_disable_dma_rx(gmacdev);

	/*prepare the gmac for magic packet reception
	* and wake up frame reception
	*/
	synopGMAC_magic_packet_enable(gmacdev);
	synopGMAC_write_wakeup_frame_register(
		gmacdev,
		synopGMAC_wakeup_filter_config3);

	synopGMAC_wakeup_frame_enable(gmacdev);

	/*gate the application and transmit clock inputs to the code.
	* This is not done in this driver :).
	*/

	/*enable the Mac for reception*/
	synopGMAC_rx_enable(gmacdev);

	/*Enable the assertion of PMT interrupt*/
	synopGMAC_pmt_int_enable(gmacdev);
	/*enter the power down mode*/
	synopGMAC_power_down_enable(gmacdev);
}

static void synopGMAC_linux_powerup_mac(struct synop_device *gmacdev)
{
	/* Let ISR know that MAC is out of power down now*/
	GMAC_Power_down = 0;
#ifdef DEBUG
	if (synopGMAC_is_magic_packet_received(gmacdev))
		TR("GMAC wokeup due to Magic Pkt Received\n");
	if (synopGMAC_is_wakeup_frame_received(gmacdev))
		TR("GMAC wokeup due to Wakeup Frame Received\n");
#endif
	/*Disable the assertion of PMT interrupt*/
	synopGMAC_pmt_int_disable(gmacdev);

	/*Enable the mac and Dma rx and tx paths*/
	synopGMAC_rx_enable(gmacdev);
	synopGMAC_enable_dma_rx(gmacdev);

	synopGMAC_tx_enable(gmacdev);
	synopGMAC_enable_dma_tx(gmacdev);
}

/* Detect links tatus by work queue */
static void nvt_wq_detect_linkstatus(struct work_struct *ignored)
{
	u16 data;
	struct net_device *netdev = synopGMACadapter->synopGMACnetdev;
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;

	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	gmacdev = (struct synop_device *)adapter->synopGMACdev;

	/* check phy status */
	synopGMAC_read_phy_reg(
	(unsigned long *)gmacdev->MacBase,
	gmacdev->PhyBase, PHY_STATUS_REG, &data);

	if ((data & Mii_Link) == 0)	{
		if (gmacdev->LinkState == 1) {
			pr_crit("[NTKETHMAC] Eth Unplug is detected : %08x\n",
				data);

#ifdef CONFIG_NVT_INTERNAL_PHY
			if (inRESET == 0) {
				/*WQ, sleep is allowed*/
				synopGMAC_eq_set_function();
				msleep(100);
				synopGMAC_eq_reset_function(1);
			}
#endif
		}
		gmacdev->LinkState = 0;
		gmacdev->DuplexMode = 0;
		gmacdev->Speed = 0;
		gmacdev->LoopBackMode = 0;
#ifdef CONFIG_NVT_INTERNAL_PHY
		synopGMAC_set_nvt_phy_link_trigger_level(gmacdev);
#endif
		netif_carrier_off(synopGMACadapter->synopGMACnetdev);
	} else {
		if (gmacdev->LinkState == 0)
			pr_crit("[NTKETHMAC] Eth Plug is detected : %08x\n",
				data);

		if (!gmacdev->LinkState) {
			gmacdev->LinkState = 1;

			if (gmacdev->AutoNgFail)
				synopGMAC_restart_auto_nego(gmacdev);

			synopGMAC_check_phy_init_for_10_100M(gmacdev);

#ifdef CONFIG_NVT_RTL_PHY_SUPPORT
			if (gmacdev->PHY_ID == RTL8201F_PHY_ID) {
				synopGMAC_RTL8201F_set_phy_TXC_direction(
				gmacdev, 1);
				synopgmac_rtl8201f_set_phy_rmii_rx_timing(
				gmacdev, 0xD);
			}
#endif
			if (netdev->flags & IFF_PROMISC)
				synopGMAC_mac_init(gmacdev, 1);
			else
				synopGMAC_mac_init(gmacdev, 0);

			if (do_suspend == 1) {
				synopGMAC_enable_interrupt(
					gmacdev, DmaIntEnable);
				synopGMAC_enable_dma_rx(gmacdev);
				synopGMAC_enable_dma_tx(gmacdev);
				do_suspend = 0;
			}

			netif_carrier_on(synopGMACadapter->synopGMACnetdev);
		}
	}
}

/**
* This sets up the transmit Descriptor queue in ring or chain mode.
* This function is tightly coupled to the platform and operating system
* Device is interested only after the descriptors are setup.
* Therefore this function
* is not included in the device driver API.
* This function should be treated as an
* example code to design the descriptor structures for ring mode or chain mode.
* This function depends on the pcidev structure
* for allocation consistent dma-able memory in case of linux.
* This limitation is due to the fact that linux uses pci structure
* to allocate a dmable memory
*-Allocates the memory for the descriptors.
*-Initialize the Busy and Next descriptors indices to 0
* (Indicating first descriptor).
*-Initialize the Busy and Next descriptors to first descriptor address.
*-Initialize the last descriptor with the endof ring in case of ring mode.
*-Initialize the descriptors in chain mode.
* @param[in] pointer to struct synop_device.
* @param[in] pointer to pci_device structure.
* @param[in] number of descriptor expected in tx descriptor queue.
* @param[in] whether descriptors to be created in RING mode or CHAIN mode.
* \return 0 upon success. Error code upon failure.
* \note This function fails if allocation fails
* for required number of descriptors in Ring mode, but in chain mode
* function returns -ESYNOPGMACNOMEM
* in the process of descriptor chain creation. once returned from this function
* user should for gmacdev->TxDescCount to see how
* many descriptors are there in the chain. Should continue further
* only if the number of descriptors in the chain meets the requirements
*/

static s32 synopGMAC_setup_tx_desc_queue(
	struct synop_device *gmacdev,
	struct pci_dev *pcidev, u32 no_of_desc, u32 desc_mode)
{
	u32 i;
	struct dma_desc *first_desc = NULL;
	struct dma_desc *second_desc = NULL;
	dma_addr_t dma_addr;

	gmacdev->TxDescCount = 0;

	if (desc_mode == RINGMODE) {
		TR("Total size required for Tx Des in Ring = 0x%08x\n",
		   ((sizeof(struct dma_desc) * no_of_desc)));

		/*alloc memory in kernel*/
		first_desc =
		sys_plat_alloc_memory(sizeof(struct dma_desc) * no_of_desc);

		/* use cache virtual address instead of dma address */
		dma_addr = (dma_addr_t)first_desc;

		if (!first_desc) {
			TR("Error in Tx Descriptors memory allocation\n");
			return -ESYNOPGMACNOMEM;
		}

		/*clear tx descriptors*/
		memset((char *)first_desc, 0,
		       sizeof(struct dma_desc) * no_of_desc);

		gmacdev->TxDescCount = no_of_desc;
		gmacdev->TxDesc      = first_desc;
		/*save the dma address*/
		gmacdev->TxDescDma   = virt_to_phys((unsigned long *)dma_addr);

		for (i = 0; i < gmacdev->TxDescCount; i++) {
			synopGMAC_tx_desc_init_ring(
				gmacdev->TxDesc + i,
				i == gmacdev->TxDescCount - 1);
			TR("%02d %08x\n",
			   i, (unsigned int)(gmacdev->TxDesc + i));
		}

		/*sync tx init descriptors to dram*/
		dma_sync_single_for_device(
			NULL,
			virt_to_phys(first_desc),
			(sizeof(struct dma_desc) * no_of_desc),
			DMA_TO_DEVICE);

	} else {
		/*alloc memory in kernel*/
		first_desc = sys_plat_alloc_memory(sizeof(struct dma_desc));

		/*use cache virtual address instead of dma address*/
		dma_addr = (dma_addr_t)first_desc;

		if (!first_desc) {
			TR("Error in Tx Des. allocation in Ring\n");
			return -ESYNOPGMACNOMEM;
		}

		gmacdev->TxDesc       = first_desc;
		/*save the dma address*/
		gmacdev->TxDescDma    = virt_to_phys((unsigned long *)dma_addr);

		TR("Tx=======================Tx\n");
		first_desc->buffer2   = gmacdev->TxDescDma;
		first_desc->data2     = (unsigned long)gmacdev->TxDesc;

		gmacdev->TxDescCount = 1;

		for (i = 0; i < (no_of_desc - 1); i++) {
			/*alloc memory in kernel*/
			second_desc =
			sys_plat_alloc_memory(sizeof(struct dma_desc));

			if (!second_desc) {
				TR("Error in Tx Des. allocation in Chain\n");
				return -ESYNOPGMACNOMEM;
			}
			first_desc->buffer2  =
				virt_to_phys((unsigned long *)dma_addr);

			first_desc->data2    = (unsigned long)second_desc;

			second_desc->buffer2 = gmacdev->TxDescDma;
			second_desc->data2   = (unsigned long)gmacdev->TxDesc;

			synopGMAC_tx_desc_init_chain(first_desc);

			TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",
			   gmacdev->TxDescCount,
			   (unsigned long)first_desc,
				first_desc->status, first_desc->length,
				first_desc->buffer1, first_desc->buffer2,
				first_desc->data1, first_desc->data2);

			gmacdev->TxDescCount += 1;
			first_desc = second_desc;
		}

		synopGMAC_tx_desc_init_chain(first_desc);
		TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",
		   gmacdev->TxDescCount, (unsigned long)first_desc,
			first_desc->status, first_desc->length,
			first_desc->buffer1, first_desc->buffer2,
			first_desc->data1, first_desc->data2);
		TR("Tx=======================Tx\n");
	}

	gmacdev->TxNext = 0;
	gmacdev->TxBusy = 0;
	gmacdev->TxNextDesc = gmacdev->TxDesc;
	gmacdev->TxBusyDesc = gmacdev->TxDesc;
	gmacdev->BusyTxDesc  = 0;

	return -ESYNOPGMACNOERR;
}

/**
* This sets up the receive Descriptor queue in ring or chain mode.
* This function is tightly coupled to the platform and operating system
* Device is interested only after the descriptors are setup.
* Therefore this function
* is not included in the device driver API.
* This function should be treated as an
* example code to design the descriptor structures
* in ring mode or chain mode.
* This function depends on the pcidev structure for allocation
* of consistent dma-able memory in case of linux.
* This limitation is due to the fact that linux uses pci structure
* to allocate a dmable memory
*- Allocates the memory for the descriptors.
*- Initialize the Busy and Next descriptors indices to 0
* (Indicating first descriptor).
*- Initialize the Busy and Next descriptors to first descriptor address.
*- Initialize the last descriptor with the endof ring in case of ring mode.
*- Initialize the descriptors in chain mode.
* @param[in] pointer to struct synop_device.
* @param[in] pointer to pci_device structure.
* @param[in] number of descriptor expected in rx descriptor queue.
* @param[in] whether descriptors to be created
* in RING mode or CHAIN mode.
* \return 0 upon success. Error code upon failure.
* \note This function fails if allocation fails
* for required number of descriptors in Ring mode, but in chain mode
*  function returns -ESYNOPGMACNOMEM in the process
* of descriptor chain creation. once returned from this function
*  user should for gmacdev->RxDescCount
* to see how many descriptors are there in the chain.
* Should continue further
*  only if the number of descriptors in
* the chain meets the requirements
*/
static s32 synopGMAC_setup_rx_desc_queue(
	struct synop_device *gmacdev,
	struct pci_dev *pcidev, u32 no_of_desc, u32 desc_mode)
{
	u32 i;
	struct dma_desc *first_desc = NULL;
	struct dma_desc *second_desc = NULL;
	dma_addr_t dma_addr;

	gmacdev->RxDescCount = 0;

	if (desc_mode == RINGMODE) {
		TR("total size required for Rx Des. in Ring Mode = 0x%08x\n",
		   ((sizeof(struct dma_desc) * no_of_desc)));

		/*alloc memory in kernel*/
		first_desc =
		sys_plat_alloc_memory(sizeof(struct dma_desc) * no_of_desc);

		if (!first_desc) {
			TR("Error in Rx Des. memory allocation in Ring mode\n");
			return -ESYNOPGMACNOMEM;
		}

		/* use cache virtual address instead of dma address*/
		dma_addr = (dma_addr_t)first_desc;

		memset((char *)first_desc, 0,
		       sizeof(struct dma_desc) * no_of_desc);

		gmacdev->RxDescCount = no_of_desc;
		gmacdev->RxDesc      = first_desc;
		/*save the dma address*/
		gmacdev->RxDescDma   =
				 virt_to_phys((unsigned long *)dma_addr);

		for (i = 0; i < gmacdev->RxDescCount; i++) {
			synopGMAC_rx_desc_init_ring(
			gmacdev->RxDesc + i,
			i == gmacdev->RxDescCount - 1);
			TR("%02d %08x\n",
			   i, (unsigned int)(gmacdev->RxDesc + i));
		}

		/*sync rx init descriptors to dram*/
		dma_sync_single_for_device(
			NULL,
			virt_to_phys(first_desc),
			(sizeof(struct dma_desc) * no_of_desc),
			DMA_TO_DEVICE);

	} else {
		/*alloc memory in kernel*/
		first_desc = sys_plat_alloc_memory(sizeof(struct dma_desc));
		/*use cache virtual address instead of dma address*/
		dma_addr = (dma_addr_t)first_desc;

		if (!first_desc) {
			TR("Error in Rx Des. Memory allocation in Ring mode\n");
			return -ESYNOPGMACNOMEM;
		}
		gmacdev->RxDesc       = first_desc;
		/*save the dma address*/
		gmacdev->RxDescDma    = virt_to_phys((unsigned long *)dma_addr);

		TR("Rx========================Rx\n");
		first_desc->buffer2   = gmacdev->RxDescDma;
		first_desc->data2     = (unsigned long)gmacdev->RxDesc;

		gmacdev->RxDescCount = 1;

		for (i = 0; i < (no_of_desc - 1); i++) {
			/*alloc memory in kernel*/
			second_desc =
			sys_plat_alloc_memory(sizeof(struct dma_desc));

			if (!second_desc) {
				TR("Error in Rx Des allocation in Chain\n");
				return -ESYNOPGMACNOMEM;
			}

			first_desc->buffer2  =
				virt_to_phys((unsigned long *)dma_addr);

			first_desc->data2    = (unsigned long)second_desc;

			second_desc->buffer2 = gmacdev->RxDescDma;
			second_desc->data2   = (unsigned long)gmacdev->RxDesc;

			synopGMAC_rx_desc_init_chain(first_desc);
			TR("%02d  %08x %08x %08x %08x %08x %08x %08x\n",
			   gmacdev->RxDescCount, (u32)first_desc,
				first_desc->status, first_desc->length,
				first_desc->buffer1, first_desc->buffer2,
				first_desc->data1, first_desc->data2);
			gmacdev->RxDescCount += 1;
			first_desc = second_desc;
		}
		synopGMAC_rx_desc_init_chain(first_desc);
		TR("%02d  %08x %08x %08x %08x %08x %08x %08x\n",
		   gmacdev->RxDescCount, (unsigned long)first_desc,
			first_desc->status, first_desc->length,
			first_desc->buffer1, first_desc->buffer2,
			first_desc->data1, first_desc->data2);
		TR("Rx========================Rx\n");
	}

	gmacdev->RxNext = 0;
	gmacdev->RxBusy = 0;
	gmacdev->RxNextDesc = gmacdev->RxDesc;
	gmacdev->RxBusyDesc = gmacdev->RxDesc;

	gmacdev->BusyRxDesc   = 0;

	return -ESYNOPGMACNOERR;
}

/**
 * This gives up the receive Descriptor queue in ring or chain mode.
 * This function is tightly coupled to
 * the platform and operating system
 * Once device's Dma is stopped the memory descriptor memory
 * and the buffer memory deallocation,
 * is completely handled by the operating system,
 * this call is kept outside the device driver Api.
 * This function should be treated as an example code
 * to de-allocate the descriptor structures in ring mode or chain mode
 * and network buffer deallocation.
 * This function depends on the pcidev structure for dma-able memory
 * deallocation for both descriptor memory and the
 * network buffer memory under linux.
 * The responsibility of this function is to
 *- Free the network buffer memory if any.
 *- Fee the memory allocated for the descriptors.
 * @param[in] pointer to struct synop_device.
 * @param[in] pointer to pci_device structure.
 * @param[in] number of descriptor expected in rx descriptor queue.
 * @param[in] whether descriptors to be created in RING mode or CHAIN mode.
 * \return 0 upon success. Error code upon failure.
 * \note No referece should be made to descriptors once this function is called.
 * This function is invoked when the device is closed.
 */
static void synopGMAC_giveup_rx_desc_queue(
	struct synop_device *gmacdev,
	struct pci_dev *pcidev, u32 desc_mode)
{
	u32 i;
	u32 status;
	u32 dma_addr1, dma_addr2;
	u32 length1, length2;
	unsigned long data1, data2;
	struct dma_desc *first_desc = NULL;

	if (desc_mode == RINGMODE) {
		for (i = 0; i < gmacdev->RxDescCount; i++) {
			synopGMAC_get_desc_data(
				gmacdev->RxDesc + i,
				&status, &dma_addr1, &length1, &data1,
				&dma_addr2, &length2, &data2);
			if ((length1 != 0) && (data1 != 0)) {
				/* free buffer1*/
				dev_kfree_skb_any((struct sk_buff *)data1);
				/*debug
				* TR("(Ring mode) rx buffer1 %08x of size
				* %d from %d rx descriptor is given back\n",
				*data1, length1, i);
				*/
			}
			if ((length2 != 0) && (data2 != 0)) {
				/*free buffer2*/
				dev_kfree_skb_any((struct sk_buff *)data2);
				/*debug
				*TR("(Ring mode) rx buffer2 %08x of size
				* %d from %d rx descriptor is given back\n",
				* data2, length2, i);
				*/
			}
		}
		/*free descriptors memory*/
		sys_plat_free_memory((unsigned long *)gmacdev->RxDesc);

		TR("Alloc %08x for Rx Des.(ring) is given back\n",
		   (unsigned long)gmacdev->RxDesc);
	} else {
		TR("rx-----------------------------------rx\n");
		first_desc          = gmacdev->RxDesc;
		for (i = 0; i < gmacdev->RxDescCount; i++) {
			synopGMAC_get_desc_data(
				first_desc, &status,
				&dma_addr1, &length1, &data1,
				&dma_addr2, &length2, &data2);

			TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",
			   i, (unsigned long)first_desc, first_desc->status,
			first_desc->length, first_desc->buffer1,
			first_desc->buffer2, first_desc->data1,
			first_desc->data2);

			if ((length1 != 0) && (data1 != 0)) {
				/*free buffer1*/
				dev_kfree_skb_any((struct sk_buff *)data1);
				TR("r_buf1 %08x size%d from %d rxdesc. back\n",
				   data1, length1, i);
			}
			/*free descriptors memory*/
			sys_plat_free_memory((unsigned long *)first_desc);

			TR("alloc %08x for RxDes. at %d is back\n",
			   data2, i);

			first_desc = (struct dma_desc *)data2;
		}

		TR("rx-----------------------------------rx\n");
	}
	gmacdev->RxDesc    = NULL;
	gmacdev->RxDescDma = 0;
}

/**
 * This gives up the transmit Descriptor queue in ring or chain mode.
 * This function is tightly coupled to the platform and operating system
 * Once device's Dma is stopped the memory
 * descriptor memory and the buffer memory deallocation,
 * is completely handled by the operating system,
 * this call is kept outside the device driver Api.
 * This function should be treated as an example
 * code to de-allocate the descriptor structures in ring mode or chain mode
 * and network buffer deallocation.
 * This function depends on the pcidev structure
 * for dma-able memory deallocation for both descriptor memory and the
 * network buffer memory under linux.
 * The responsibility of this function is to
 *     - Free the network buffer memory if any.
 *	- Fee the memory allocated for the descriptors.
 * @param[in] pointer to struct synop_device.
 * @param[in] pointer to pci_device structure.
 * @param[in] number of descriptor expected in tx descriptor queue.
 * @param[in] whether descriptors
 * to be created in RING mode or CHAIN mode.
 * \return 0 upon success. Error code upon failure.
 * \note No reference should be made to
 * descriptors once this function is called.
 * This function is invoked when the device is closed.
 */
static void synopGMAC_giveup_tx_desc_queue(
	struct synop_device *gmacdev,
	struct pci_dev *pcidev, u32 desc_mode)
{
	u32 i;
	u32 status;
	u32 length1, length2;
	u32 dma_addr1, dma_addr2;
	unsigned long data1, data2;
	struct dma_desc *first_desc = NULL;

	if (desc_mode == RINGMODE) {
		for (i = 0; i < gmacdev->TxDescCount; i++) {
			synopGMAC_get_desc_data(
				gmacdev->TxDesc + i, &status,
				&dma_addr1, &length1, &data1,
				&dma_addr2, &length2, &data2);

			if ((length1 != 0) && (data1 != 0)) {
				/*  free buffer1 */
				dev_kfree_skb_any((struct sk_buff *)data1);
			}
			if ((length2 != 0) && (data2 != 0)) {
				/* free buffer2 */
				dev_kfree_skb_any((struct sk_buff *)data2);
			}
		}
		/*Free descriptors */
		sys_plat_free_memory((unsigned long *)gmacdev->TxDesc);

		/*debug
		* TR("Memory allocated %08x for Tx Descriptors
		* (ring) is given back\n",(u32)gmacdev->TxDesc);
		*/
	} else {
		TR("tx----------------------------tx\n");
		first_desc          = gmacdev->TxDesc;
		for (i = 0; i < gmacdev->TxDescCount; i++) {
			synopGMAC_get_desc_data(
				first_desc, &status, &dma_addr1,
				&length1, &data1, &dma_addr2, &length2, &data2);

			TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",
			   i, (u32)first_desc, first_desc->status,
				first_desc->length, first_desc->buffer1,
				first_desc->buffer2, first_desc->data1,
				first_desc->data2);
			if ((length1 != 0) && (data1 != 0)) {
				/*pci_unmap_single
				* (pcidev,dma_addr1,0,PCI_DMA_TODEVICE);
				*/
				/*  free buffer1 */
				dev_kfree_skb_any((struct sk_buff *)data2);
				/*debug
				* TR("(Chain) tx buff1 %08x of size %d
				* from %d rx desc is given back\n",
				* data1, length1, i);
				*/
			}
			/*  jay hsu : free descriptors */
			sys_plat_free_memory((unsigned long *)first_desc);

			TR("Memory allc %08x for Tx Des. at %d is given back\n",
			   data2, i);

			first_desc = (struct dma_desc *)data2;
		}
		TR("tx----------------------------tx\n");
	}
	gmacdev->TxDesc    = NULL;
	gmacdev->TxDescDma = 0;
}

static void synopGMAC_linux_reset_mac(struct net_device *netdev,
				      struct synop_device *gmacdev)
{
	s8 status;
	s32 skblen;
	unsigned long desc_index;

	struct sk_buff *skb;

	inRESET = 1;
	/*Disable all the interrupts*/
	synopGMAC_disable_interrupt_all(gmacdev);
	TR("the synopGMAC interrupt has been disabled\n");

	/*Disable the reception*/
	synopGMAC_disable_dma_rx(gmacdev);
	synopGMAC_take_desc_ownership_rx(gmacdev);
	TR("the synopGMAC Reception has been disabled\n");

	/*Disable the transmission*/
	synopGMAC_disable_dma_tx(gmacdev);
	synopGMAC_take_desc_ownership_tx(gmacdev);

	netif_stop_queue(netdev);

	synopGMAC_giveup_rx_desc_queue(gmacdev, NULL, RINGMODE);
	synopGMAC_giveup_tx_desc_queue(gmacdev, NULL, RINGMODE);

	synopGMAC_setup_tx_desc_queue(
		gmacdev, NULL, TRANSMIT_DESC_SIZE, RINGMODE);
	/* Program the transmit descriptor
	* base address in to DmaTxBase addr
	*/
	synopGMAC_init_tx_desc_base(gmacdev);

	synopGMAC_setup_rx_desc_queue(
		gmacdev, NULL, RECEIVE_DESC_SIZE, RINGMODE);
	/* Program the transmit descriptor
	* base address in to DmaTxBase addr
	*/
	synopGMAC_init_rx_desc_base(gmacdev);

	do {
		skb = alloc_skb(
			netdev->mtu + ETHERNET_HEADER + ETHERNET_CRC,
			GFP_ATOMIC);

		if (!skb) {
			TR0("ERROR in skb buffer allocation\n");
			break;
		}

		skblen = skb_tailroom(skb);
		if (skblen > 0) {
			status = synopGMAC_set_rx_qptr(
				gmacdev, virt_to_phys(skb->data),
				(u32)skblen,
				(unsigned long)skb, 0, 0, 0,
				&desc_index);
		} else {
			status = -1;
		}
		if (status < 0)
			dev_kfree_skb_any(skb);
	} while (status == 0);

#ifndef CONFIG_NVT_INTERNAL_PHY
	/* External ethernet phy reset */
	*external_PHY_reset_dir |= 1UL << NVT_PHY_RESET_GPIO_BIT;
	*external_PHY_reset_pull_low = 1UL << NVT_PHY_RESET_GPIO_BIT;
	mdelay(1);
	*external_PHY_reset_pull_high = 1UL << NVT_PHY_RESET_GPIO_BIT;
#else
	/* nvt_internal_PHY_reset(gmacdev); */
#endif /* CONFIG_NVT_INTERNAL_PHY */

#ifdef CONFIG_NVT_RTL_PHY_SUPPORT
	if (gmacdev->PHY_ID == RTL8201F_PHY_ID) {
		synopGMAC_RTL8201F_set_phy_TXC_direction(gmacdev, 1);
		synopgmac_rtl8201f_set_phy_rmii_rx_timing(gmacdev, 0xD);
	}
#endif
	synopGMAC_clear_interrupt(gmacdev);
	/***
	* Disable the interrupts generated by MMC and IPC counters.
	* If these are not disabled ISR should be
	* modified accordingly to handle these interrupts.
	*/
	synopGMAC_disable_mmc_tx_interrupt(gmacdev, 0xFFFFFFFF);
	synopGMAC_disable_mmc_rx_interrupt(gmacdev, 0xFFFFFFFF);
	synopGMAC_disable_mmc_ipc_rx_interrupt(gmacdev, 0xFFFFFFFF);

	synopGMAC_enable_interrupt(gmacdev, DmaIntEnable);
	synopGMAC_enable_dma_rx(gmacdev);
	synopGMAC_enable_dma_tx(gmacdev);
	netif_wake_queue(netdev);
	inRESET = 0;
}

/**
 * Function to transmit a SS self test packet on the wire.
 * The function prepares the descriptor and enables/resumes the transmission.
 * \return Returns 0 on success and Error code on failure.
 * \note
 */
static s32 synopGMAC_SS_self_test_xmit_frames(struct synop_device *gmacdev)
{
	s8 status = 0;
	u32 offload_needed = 0;
	/* Add tx lock support */
	unsigned long desc_index = 0;
	unsigned long irq_flags;

	if (!gmacdev) {
		pr_crit("synopGMAC_SS_self_test_xmit_frames gmacdev == NULL\n");
		return -1;
	}

	if (!gmacdev->ssLoopbackEnable) {
		pr_crit("synopGMAC_SS_self_test_xmit_frames ssLoopbackEnable == 0\n");
		return -1;
	}

	/*  start spin_lock protection */
	spin_lock_irqsave(&gmacdev->tx_lock, irq_flags);
	status = synopGMAC_set_tx_qptr(gmacdev,
				       SS_selftest_frame_phys,
		gmacdev->ssLoopbackFrameSize,
		(unsigned long)SS_selftest_frame,
		0, 0, 0, offload_needed, &desc_index);

	if (status < 0) {
		pr_crit("synopGMAC_SS_self_test_xmit_frames status < 0\n");
		/* End spin_lock protection */
		spin_unlock_irqrestore(&gmacdev->tx_lock, irq_flags);
		return -1;
	}

	/*Now force the DMA to start transmission*/
	synopGMAC_resume_dma_tx(gmacdev);
	/*end spin_lock protection */
	spin_unlock_irqrestore(&gmacdev->tx_lock, irq_flags);

	return -ESYNOPGMACNOERR;
}

/**
 * Function to Receive a packet from the interface in SS self test mode.
 * @param[in]
 * \return void.
 * \note This function runs in interrupt context.
 */
static void synopGMAC_SS_self_test_recv_frames(struct synop_device *gmacdev)
{
	u32 len;
	u32 status;
	u32 dma_addr1;
	u32 dma_addr2;
	unsigned long desc_index;
	unsigned long data1, data2;

#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 time_stamp_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif

	u32 i = 0;
	s32 skblen;
	/* This is the pointer to hold the received data */
	s8 ret;
	struct sk_buff *skb;

	/*Handle the Receive Descriptors*/
	do {
#ifdef ENH_DESC_8W
		ret = synopGMAC_get_rx_qptr(
			gmacdev, &status, &dma_addr1, NULL,
			&data1, &dma_addr2, NULL, &data2,
			&ext_status, &time_stamp_high, &time_stamp_low,
			&desc_index);
		if (ret == 0) {
			synopGMAC_TS_read_timestamp_higher_val(
				gmacdev, &time_stamp_higher);
			/*debug
			* TR("S:%08x ES:%08x DA1:%08x d1:%08x DA2:%08x
			* d2:%08x TSH:%08x TSL:%08x TSHW:%08x\n",status,
			* ext_status,dma_addr1, data1,dma_addr2,data2,
			* time_stamp_high,time_stamp_low,time_stamp_higher);
			*/
			printk_info("synopGMAC_SS_self_test_recv_frames desc_index >0\n");
		}
#else
		ret = synopGMAC_get_rx_qptr(
			gmacdev, &status, &dma_addr1, NULL,
			&data1, &dma_addr2, NULL, &data2, &desc_index);
		/* debug
		* TR("##Handle Received## desc_index %d, dma_addr1 0x%x,
		* data1 0x%x\n", desc_index, (u32)dma_addr1, data1);
		*/
#endif
		if (ret == 0 && data1 != 0) {
			/*debug only
			* TR("Received Data at Rx Descriptor %d
			* for skb 0x%08x whose status is
			* %08x\n",desc_index,data1,status);
			*/
			/*At first step unmapped the dma address*/
			skb = (struct sk_buff *)data1;
			mb();/*memory barrier without comment*/

			if (synopGMAC_is_rx_desc_valid(status)) {
				/* Not interested in Ethernet CRC bytes */
				len =
				synopGMAC_get_rx_desc_frame_length(status) - 4;

				skb_put(skb, len);

				for (i = 0;
					i < gmacdev->ssLoopbackFrameSize; i++) {
					if (
						(char)*((char *)skb->data + i) ^
						(*((char *)(SS_selftest_frame) +
						i))) {
						break;
					}
				}

				if (i < gmacdev->ssLoopbackFrameSize)
					gmacdev->ssLoopbackErrorPkt++;
				else
					gmacdev->ssLoopbackPassPkt++;

			} else {
				/*Now the present skb should be set free*/
				gmacdev->ssLoopbackErrorPkt++;
				pr_crit("SS self test rx desc is not valid  status = %08x\n",
					status);
			}
			dev_kfree_skb_any(skb);

			/* Now lets allocate the skb
			* for the emptied descriptor
			*/
			skb = dev_alloc_skb(
			MAX_ETHERNET_PAYLOAD + ETHERNET_PACKET_EXTRA);
			if (!skb) {
				show_skb_alloc_error();
				return;
			}

			skblen = skb_tailroom(skb);
			if (skblen > 0) {
				ret = synopGMAC_set_rx_qptr(
					gmacdev, virt_to_phys(skb->data),
					(u32)skblen,
					(unsigned long)skb, 0, 0, 0,
					&desc_index);
			} else {
				ret = -1;
			}
			if (ret < 0) {
				pr_crit("Cannot set Rx Descriptor for skb %lu\n",
					(unsigned long)skb);
				dev_kfree_skb_any(skb);
			}
		}
	} while (ret == 0);
}

/**
 * Function to handle housekeeping after a packet is transmitted over the wire.
 * After the transmission of a packet DMA generates corresponding interrupt
 * (if it is enabled). It takes care of returning the sk_buff to the linux
 * kernel, updating the networking statistics and tracking the descriptors.
 * @param[in] pointer to net_device structure.
 * \return void.
 * \note This function runs in interrupt context
 */
static void synopGMAC_SS_self_test_transmit_over(struct synop_device *gmacdev)
{
	u32 status;
	u32 length1, length2;
	u32 dma_addr1, dma_addr2;
	unsigned long desc_index;
	unsigned long data1, data2;

#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 time_stamp_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif

	unsigned long irq_flags;
	s8 ret;

	/*Handle the transmit Descriptors*/
	do {
		spin_lock_irqsave(&gmacdev->tx_lock, irq_flags);
#ifdef ENH_DESC_8W
		ret = synopGMAC_get_tx_qptr(
		gmacdev, &status, &dma_addr1, &length1,
		&data1, &dma_addr2, &length2, &data2,
		&ext_status, &time_stamp_high, &time_stamp_low,
		&desc_index);
		synopGMAC_TS_read_timestamp_higher_val(
			gmacdev, &time_stamp_higher);
#else
		ret = synopGMAC_get_tx_qptr(
		gmacdev, &status, &dma_addr1, &length1,
		&data1, &dma_addr2, &length2,
		&data2, &desc_index);
#endif
		spin_unlock_irqrestore(&gmacdev->tx_lock, irq_flags);

	} while (ret == 0);
}

/**
 * Function to handle housekeeping after a packet is transmitted over the wire.
 * After the transmission of a packet DMA generates corresponding interrupt
 * (if it is enabled). It takes care of returning the sk_buff to the linux
 * kernel, updating the networking statistics and tracking the descriptors.
 * @param[in] pointer to net_device structure.
 * \return void.
 * \note This function runs in interrupt context
 */
static void synop_handle_transmit_over(struct net_device *netdev)
{
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;
	/*struct pci_dev *pcidev;*/
	u32 status;
	u32 length1, length2;
	u32 dma_addr1, dma_addr2;
	unsigned long desc_index;
	unsigned long data1, data2;

#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 time_stamp_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif

	unsigned long irq_flags;
	s8 ret;

	TR("%s called\n", __func__);

	adapter = netdev_priv(netdev);
	if (!adapter) {
		TR("Unknown Device\n");
		return;
	}

	gmacdev = adapter->synopGMACdev;
	if (!gmacdev) {
		TR("GMAC device structure is missing\n");
		return;
	}

	if (gmacdev->ssLoopbackEnable) {
		synopGMAC_SS_self_test_transmit_over(gmacdev);
		return;
	}

	/*Handle the transmit Descriptors*/
	do {
		spin_lock_irqsave(&gmacdev->tx_lock, irq_flags);
#ifdef ENH_DESC_8W
		ret = synopGMAC_get_tx_qptr(
		gmacdev, &status, &dma_addr1,
		&length1, &data1, &dma_addr2,
		&length2, &data2, &ext_status,
		&time_stamp_high, &time_stamp_low,
		&desc_index);
		synopGMAC_TS_read_timestamp_higher_val(
			gmacdev, &time_stamp_higher);
#else
		ret = synopGMAC_get_tx_qptr(
		gmacdev, &status, &dma_addr1,
		&length1, &data1, &dma_addr2,
		&length2, &data2, &desc_index);
#endif
		spin_unlock_irqrestore(&gmacdev->tx_lock, irq_flags);

		if (ret == 0 && data1 != 0) {
			/*debug only
			* TR("Finished Transmit at Tx Descriptor %d
			*for skb 0x%08x and buffer = %08x whose
			*status is %08x\n", desc_index,
			*data1,dma_addr1,status);
			*/
#ifdef	IPC_OFFLOAD
			if (synopGMAC_is_tx_ipv4header_checksum_error(
				gmacdev, status)) {
				TR("HW Fail to Insert IPV4 hdr CRC\n");
			}
			if (synopGMAC_is_tx_payload_checksum_error(
				gmacdev, status)) {
				TR("HW Fail to Insert Payload CRC\n");
			}
#endif
			dev_kfree_skb_any((struct sk_buff *)data1);

			if (synopGMAC_is_desc_valid(status)) {
				adapter->synop_stats.tx_bytes += length1;
				adapter->synop_stats.tx_packets++;
				TR("<<tx_bytes>> 0x%lx\n",
				   adapter->synop_stats.tx_bytes);
				TR("<<tx_packets>> %ld\n",
				   adapter->synop_stats.tx_packets);
			} else {
				pr_crit("%s: Error in Status %08x\n",
					__func__, status);
				adapter->synop_stats.tx_errors++;
				adapter->synop_stats.tx_aborted_errors +=
					synopGMAC_is_tx_aborted(status);
				adapter->synop_stats.tx_carrier_errors +=
					synopGMAC_is_tx_carrier_error(status);
			}

			/*driver bug fixed. Only count the collision
			* when desc_index isn't -1.
			*/
			adapter->synop_stats.collisions +=
			synopGMAC_get_tx_collision_count(status);
		}

	} while (ret == 0);

	netif_wake_queue(netdev);
}

/**
 * Function to Receive a packet from the interface.
 * After Receiving a packet, DMA transfers
 * the received packet to the system memory
 * and generates corresponding interrupt
 * (if it is enabled). This function prepares
 * the sk_buff for received packet
 * after removing the ethernet CRC, and hands it over
 * to linux networking stack.
 *- Updataes the networking interface statistics
 *- Keeps track of the rx descriptors
 * @param[in] pointer to net_device structure.
 * \return void.
 * \note This function runs in interrupt context.
 */

static void synop_handle_received_data(struct net_device *netdev)
{
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;
	/*struct pci_dev *pcidev;*/

	s8 ret;
	u32 skblen;
	u32 len;
	u32 status;
	u32 dma_addr1, dma_addr2;
	unsigned long desc_index;
	unsigned long data1, data2;

#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 time_stamp_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif

	/* This is the pointer to hold the received data */
	struct sk_buff *skb;

	TR("%s\n", __func__);

	adapter = netdev_priv(netdev);
	if (!adapter) {
		TR("Unknown Device\n");
		return;
	}

	gmacdev = adapter->synopGMACdev;
	if (!gmacdev) {
		TR("GMAC device structure is missing\n");
		return;
	}

	if (gmacdev->ssLoopbackEnable) {
		synopGMAC_SS_self_test_recv_frames(gmacdev);
		return;
	}

	/* Handle the Receive Descriptors*/
	do {
#ifdef ENH_DESC_8W
		ret = synopGMAC_get_rx_qptr(
			gmacdev,
			&status, &dma_addr1, NULL, &data1,
			&dma_addr2, NULL, &data2,
			&ext_status, &time_stamp_high,
			&time_stamp_low, &desc_index);
		if (ret == 0) {
			synopGMAC_TS_read_timestamp_higher_val(
				gmacdev, &time_stamp_higher);
			/*For debug only
			* TR("S:%08x ES:%08x DA1:%08x
			* d1:%08x DA2:%08x d2:%08x TSH:%08x
			* TSL:%08x TSHW:%08x\n",status,
			* ext_status,dma_addr1, data1,dma_addr2,
			* data2, time_stamp_high,
			* time_stamp_low,time_stamp_higher);
			*/
		}
#else
		ret = synopGMAC_get_rx_qptr(
				gmacdev,
				&status,
				&dma_addr1,
				NULL,
				&data1,
				&dma_addr2,
				NULL,
				&data2,
				&desc_index);
		/*For debug only
		* TR("##Handle Received## desc_index %d,
		* dma_addr1 0x%x, data1 0x%x\n",
		* desc_index, (u32)dma_addr1, data1);
		*/
#endif

		if (ret == 0 && data1 != 0) {
			/*For debug only
			* TR("Received Data at Rx Descriptor %d
			* for skb 0x%08x whose status is %08x\n",
			* desc_index,data1,status);
			*/
			skb = (struct sk_buff *)data1;

			if (status & DescSAFilterFail) {
				dev_kfree_skb_any(skb);

				/*  Allocate the skb for
				* the emptied descriptor.
				*/
				skb = dev_alloc_skb(
					netdev->mtu + ETHERNET_PACKET_EXTRA);
				if (!skb) {
					show_skb_alloc_error();
					return;
				}

				adapter->synop_stats.rx_dropped++;

				skblen = skb_tailroom(skb);
				if (skblen > 0) {
					ret = synopGMAC_set_rx_qptr(
						gmacdev,
						virt_to_phys(skb->data),
						(u32)skblen,
						(unsigned long)skb, 0, 0, 0,
						&desc_index);
				} else {
					ret = -1;
				}

				if (ret < 0) {
					pr_crit("Cannot set Rx Descriptor for skb %lu\n",
						(unsigned long)skb);
					dev_kfree_skb_any(skb);
				}
				return;
			}

			if (synopGMAC_is_rx_desc_valid(status)) {
				/* Not interested in EthCRC bytes */
				len =
				synopGMAC_get_rx_desc_frame_length(status) - 4;

				skb_put(skb, len);
#ifdef IPC_OFFLOAD
				/*lets check for the IPC offloading */
				/*Since we have enabled the
				* checksum offloading in hardware
				* lets inform the kernel
				* not to perform the checksum
				* computation on the incoming packet.
				* Note that ip header checksum will be
				* computed by the kernel immaterial
				* of what we inform.
				* Similarly TCP / UDP / ICMP pseudo
				* header checksum will be computed
				* by the stack.
				* What we can inform is not
				* to perform payload checksum.
				* When CHECKSUM_UNNECESSARY
				* is set kernel bypasses the
				* checksum computation.
				*/

				TR("Checksum Offloading done now\n");
				skb->ip_summed =
					CHECKSUM_UNNECESSARY;

#ifdef ENH_DESC_8W
				if (synopGMAC_is_ext_status(
					gmacdev, status)) {
					/* extended status present indicates
					* that the RDES4 need to be probed
					*/
					TR("Extended Status present\n");
					if (synopGMAC_ES_is_IP_header_error(
						gmacdev, ext_status)) {
						/*  IP header checksum error */
						/* Linux Kernel
						* doesnot care for ipv4
						* header checksum.
						* So we will simply proceed
						* by printing a warning ....
						*/
						TR("(ES)IP header error\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
				if (synopGMAC_ES_is_rx_checksum_bypassed(
					gmacdev, ext_status)) {
					/*  Hardware engine bypassed
					* the checksum computation
					* and checking
					*/
					TR("(ES)HW bypassed CRC computation\n");
					skb->ip_summed = CHECKSUM_NONE;
				}
					if (synopGMAC_ES_is_IP_payload_error(
						gmacdev, ext_status)) {
						/*  IP payload checksum is
						* in error (UDP / TCP / ICMP
						* checksum error)
						*/
						TR("(ES) Error EP payload\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
				} else {
					/* No extended status.
					* So relevant information is available
					*in the status itself
					*/
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxNoChkError) {
						TR("RxNoChkError\n");
						skb->ip_summed =
							CHECKSUM_UNNECESSARY;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxIpHdrChkError) {
						TR("RxIpHdrChkError\n");
						skb->ip_summed =
							CHECKSUM_UNNECESSARY;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxLenLT600) {
						TR("RxLenLT600\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxIpHdrPayLoadChkBypass) {
						TR("RxIpHdrPayLoadChkBypass\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxChkBypass) {
						TR("RxChkBypass\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxPayLoadChkError) {
						TR("RxPayLoadChkError\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error
						(gmacdev, status) ==
						RxIpHdrChkError) {
						TR("RxIpHdrChkError\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
				}
#else
				if (synopGMAC_is_rx_checksum_error(
					gmacdev, status) ==
					RxNoChkError) {
					/*Ip header and TCP / UDP
					* payload checksum Bypassed
					*/
					TR("RxNoChkError\n");
					skb->ip_summed =
					CHECKSUM_UNNECESSARY;
				}
				if (synopGMAC_is_rx_checksum_error(
					gmacdev, status) ==
					RxIpHdrChkError) {
					/* Linux Kernel
					* doesnot care for ipv4 header checksum.
					* So we will simply proceed
					* by printing a warning ....
					*/
					/*Error in 16bit IPV4 Header CRC*/
					TR("RxIpHdrChkError\n");

					/* Let Kernel bypass the TCP / UDP
					* checksum computation
					*/
					skb->ip_summed =
					CHECKSUM_UNNECESSARY;
				}
				if (synopGMAC_is_rx_checksum_error(
					gmacdev, status) ==
					RxLenLT600) {
					TR("RxLenLT600\n");
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(
					gmacdev, status) ==
					RxIpHdrPayLoadChkBypass) {
					/*Ip header and TCP / UDP payload
					* checksum Bypassed
					*/
					TR("RxIpHdrPayLoadChkBypass\n");
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(
					gmacdev, status) ==
					RxChkBypass) {
					/*Ip header and TCP / UDP payload
					* checksum Bypassed
					*/
					TR("RxChkBypass\n");
					/* Let Kernel compute the CRC*/
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(
					gmacdev, status) ==
					RxPayLoadChkError) {
					/*TCP / UDP payload CRC Error*/
					TR("RxPayLoadChkError\n");
					/* Let Kernel compute the CRC */
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(
					gmacdev, status) ==
					RxIpHdrChkError) {
					/* Linux Kernel
					* doesnot care for ipv4 header checksum.
					* So we will simply proceed
					* by printing a warning ....
					*/
					/*Both IP header and
					* Payload Checksum Error
					*/
					TR("RxIpHdrChkError\n");
					/* Let Kernel compute the CRC */
					skb->ip_summed = CHECKSUM_NONE;
				}

#endif
#endif /* IPC_OFFLOAD */
				skb->dev = netdev;
				skb->protocol = eth_type_trans(skb, netdev);
				netif_rx(skb);
				netdev->last_rx = jiffies;
				adapter->synop_stats.rx_packets++;
				adapter->synop_stats.rx_bytes += len;
			} else {
				/*Now the present skb should be set free*/
				pr_crit("\033[1;31m[NTKETHMAC_ERROR] %s: Descriptor is not valid status = %08x, next Desc index=%lu\033[0m\n",
					__func__, status, desc_index);

				dev_kfree_skb_any(skb);

				adapter->synop_stats.rx_errors++;
				if (synopGMAC_is_rx_frame_collision(status))
					adapter->synop_stats.collisions++;
				if (synopGMAC_is_rx_crc(status))
					adapter->synop_stats.rx_crc_errors++;
				if (synopGMAC_is_frame_dribbling_errors(status))
					adapter->synop_stats.rx_frame_errors++;
				if (synopGMAC_is_rx_frame_length_errors(status))
					adapter->synop_stats.rx_length_errors++;
			}

			/* Now lets allocate the skb
			* for the emptied descriptor
			*/
			skb = dev_alloc_skb(
				netdev->mtu + ETHERNET_PACKET_EXTRA);
			if (!skb) {
				show_skb_alloc_error();
				adapter->synop_stats.rx_dropped++;
				return;
			}

			skblen = skb_tailroom(skb);
			if (skblen > 0) {
				ret = synopGMAC_set_rx_qptr(gmacdev,
						virt_to_phys(skb->data),
						(u32)skblen,
						(unsigned long)skb, 0, 0, 0,
						&desc_index);
			} else {
				ret = -1;
			}

			if (ret < 0) {
				TR("Cannot set Rx Descriptor for skb %lu\n",
				   (unsigned long)skb);
				pr_crit("Cannot set Rx Descriptor for skb %lu\n",
					(unsigned long)skb);
				dev_kfree_skb_any(skb);
			}
		}
	} while (ret == 0);
}

/**
 * Interrupt service routing.
 * This is the function registered as ISR for device interrupts.
 * @param[in] interrupt number.
 * @param[in] void pointer to device unique structure
 * (Required for shared interrupts in Linux).
 * @param[in] pointer to pt_regs (not used).
 * \return Returns IRQ_NONE if not device interrupts IRQ_HANDLED
 * for device interrupts.
 * \note This function runs in interrupt context
 *
 */

static irqreturn_t synopGMAC_intr_handler(int irq, void *dev_id)
{
	/*Kernels passes the netdev structure in the dev_id. So grab it*/
	struct net_device *netdev;
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;

	s8 status;
	s32 skblen;
	u32 interrupt, dma_status_reg;
	unsigned long desc_index;

	TR("%s called\n", __func__);

	netdev  = (struct net_device *)dev_id;
	if (!netdev) {
		TR("Unknown Device\n");
		return -1;
	}
	adapter  = netdev_priv(netdev);
	if (!adapter) {
		TR("Adapter Structure Missing\n");
		return -1;
	}

	gmacdev = adapter->synopGMACdev;
	if (!gmacdev) {
		TR("GMAC device structure Missing\n");
		return -1;
	}
	/*Read the Dma interrupt status to
	* know whether the interrupt
	* got generated by our device or not
	*/
	dma_status_reg =
		synopGMACReadReg((unsigned long *)gmacdev->DmaBase, DmaStatus);

	if (dma_status_reg == 0)
		return IRQ_NONE;

	synopGMAC_disable_interrupt_all(gmacdev);

	TR("%s:Dma Status Reg: 0x%08x\n", __func__, dma_status_reg);

	if (dma_status_reg & GmacPmtIntr) {
		TR("%s:: Interrupt due to PMT module\n", __func__);
		synopGMAC_linux_powerup_mac(gmacdev);
	}

	if (dma_status_reg & GmacMmcIntr) {
		TR("%s:: Interrupt due to MMC module\n", __func__);
		TR("%s:: synopGMAC_rx_int_status = %08x\n",
		   __func__, synopGMAC_read_mmc_rx_int_status(gmacdev));
		TR("%s:: synopGMAC_tx_int_status = %08x\n",
		   __func__, synopGMAC_read_mmc_tx_int_status(gmacdev));
	}

	if (dma_status_reg & GmacLineIntfIntr)
		TR("%s:: Interrupt due to GMAC LINE module\n", __func__);

	/*Now lets handle the DMA interrupts*/
	interrupt = synopGMAC_get_interrupt_type(gmacdev);
	TR("%s:Interrupts to be handled: 0x%08x\n",
	   __func__, interrupt);

	if (interrupt & synopGMACDmaError) {
		TR("%s::Fatal Bus Error Inetrrupt Seen\n", __func__);
		synopGMAC_disable_dma_tx(gmacdev);
		synopGMAC_disable_dma_rx(gmacdev);

		synopGMAC_take_desc_ownership_tx(gmacdev);
		synopGMAC_take_desc_ownership_rx(gmacdev);

		synopGMAC_init_tx_rx_desc_queue(gmacdev);

		/* reset the DMA engine and the GMAC ip */
		synopGMAC_reset(gmacdev);

		synopGMAC_set_mac_addr(gmacdev,
				       GmacAddr0High, GmacAddr0Low, Mac_Addr);
		synopGMAC_set_mac_addr(gmacdev,
				       GmacAddr1High, GmacAddr1Low, Mac_Addr);

		synopGMAC_dma_bus_mode_init(gmacdev,
					    DmaFixedBurstEnable |
						DmaBurstLength8 |
						DmaDescriptorSkip6);
		synopGMAC_dma_control_init(gmacdev, DmaStoreAndForward);
		synopGMAC_init_rx_desc_base(gmacdev);
		synopGMAC_init_tx_desc_base(gmacdev);

		if (netdev->flags & IFF_PROMISC)
			synopGMAC_mac_init(gmacdev, 1);
		else
			synopGMAC_mac_init(gmacdev, 0);

		synopGMAC_enable_dma_rx(gmacdev);
		synopGMAC_enable_dma_tx(gmacdev);
	}

	if (interrupt & synopGMACDmaRxNormal) {
		TR("%s:: Rx Normal\n", __func__);

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
		if (likely(napi_schedule_prep(&nvt_napi))) {
			atomic_set(&napi_poll, 1);
			__napi_schedule(&nvt_napi);
		}
#else
		synop_handle_received_data(netdev);
#endif
	}

	if (interrupt & synopGMACDmaRxAbnormal) {
		TR("%s::Abnormal Rx Interrupt Seen\n", __func__);
		pr_crit("[NTKETHMAC] : Abnormal Rx, Do Flow Control!!\n");

		/*need deactive flow control in half-duplex*/
		synopGMAC_tx_activate_flow_control(gmacdev);
	}

	if (interrupt & synopGMACDmaRxStopped) {
		/* Receiver gone in to stopped state */
		TR("%s::Receiver stopped seeing Rx interrupts\n", __func__);
#if 1
		if (GMAC_Power_down == 0) {
			/*  If Mac is not in powerdown */
			adapter->synop_stats.rx_over_errors++;
			do {
				struct sk_buff *skb =
					alloc_skb((netdev->mtu +
					ETHERNET_HEADER + ETHERNET_CRC),
					GFP_ATOMIC);

				if (!skb) {
					/*Better Luck Next time*/
					TR("%s::ERROR in skb allocation",
					   __func__);
					break;
				}

				skblen = skb_tailroom(skb);
				if (skblen > 0) {
					status =
					synopGMAC_set_rx_qptr(gmacdev,
							      virt_to_phys(
							      skb->data),
						(u32)skblen,
						(unsigned long)skb,
						0, 0, 0, &desc_index);
				} else {
					status = -1;
				}
				TR("%s Set Rx Des. #%08x for skb %08x\n",
				   __func__, desc_index, (unsigned long)skb);

				if (status < 0) {
					/* changed from dev_free_skb.
					* If problem check this again--manju
					*/
					dev_kfree_skb_any(skb);
				}

			} while (status == 0);

			synopGMAC_enable_dma_rx(gmacdev);
		}
#endif
	}

	if (interrupt & synopGMACDmaTxNormal) {
		/* xmit function has done its job */
		TR("%s::Finished Normal Transmission\n", __func__);
		/* Do whatever you want after the transmission is over */
		synop_handle_transmit_over(netdev);
	}

	if (interrupt & synopGMACDmaTxAbnormal) {
		TR("%s::Abnormal Tx Interrupt Seen\n", __func__);
		pr_crit("%s::Abnormal Tx Interrupt Seen\n", __func__);
#if 1
		if (GMAC_Power_down == 0) {
			/*  If Mac is not in powerdown */
			synop_handle_transmit_over(netdev);
		}
#endif
	}

	if (interrupt & synopGMACDmaTxStopped) {
		TR("%s::Transmitter stopped sending the packets\n",
		   __func__);
		pr_crit("%s::Transmitter stopped sending the packets\n",
			__func__);
#if 1
		if (GMAC_Power_down == 0) {
			/*  If Mac is not in powerdown */
			synopGMAC_disable_dma_tx(gmacdev);
			synopGMAC_take_desc_ownership_tx(gmacdev);

			synopGMAC_enable_dma_tx(gmacdev);
			netif_wake_queue(netdev);
			TR("%s::Transmission Resumed\n", __func__);
		}
#endif
	}

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
	if (atomic_read(&napi_poll) == 1) {
		/*  jay hsu : ignore the rx complete */
		synopGMAC_enable_interrupt(gmacdev,
					   DmaIntEnable & (~DmaIntRxNormMask));
	} else {
		/* Enable the interrupt before returning from ISR*/
		synopGMAC_enable_interrupt(gmacdev, DmaIntEnable);
	}
#else
	/* Enable the interrupt before returning from ISR*/
	synopGMAC_enable_interrupt(gmacdev, DmaIntEnable);
#endif

	return IRQ_HANDLED;
}

/**
 * Function used when the interface is opened for use.
 * We register synopGMAC_linux_open function to linux open().
 * Basically this
 * function prepares the the device for operation .
 * This function is called whenever ifconfig (in Linux)
 * activates the device (for example "ifconfig eth0 up").
 * This function registers
 * system resources needed
 *- Attaches device to device specific structure
 *- Programs the MDC clock for PHY configuration
 *- Check and initialize the PHY interface
 *- ISR registration
 *- Setup and initialize Tx and Rx descriptors
 *- Initialize MAC and DMA
 *- Allocate Memory for RX descriptors (The should be DMAable)
 *- Initialize one second timer to detect cable plug/unplug
 *- Configure and Enable Interrupts
 *- Enable Tx and Rx
 *- start the Linux network queue interface
 * @param[in] pointer to net_device structure.
 * \return Returns 0 on success and error status upon failure.
 * \callgraph
 */

s32 synopGMAC_linux_open(struct net_device *netdev)
{
	/* u8 mac_addr0[6] = DEFAULT_MAC_ADDRESS; */
	s8 status = 0;
	u32 Phy_Init_try_count = NVT_PHY_TRY_COUNT;
	s32 retval = 0;
	s32 ijk;
	s32 count, skblen;
	unsigned long desc_index = 0;

#ifdef USE_DEVICE_TREE
#ifndef CONFIG_NVT_INTERNAL_PHY
	u32 sel_RMII_reg_val;
#endif
#endif /*  USE_DEVICE_TREE */

#if DEBUG_READ_PHY_REG
	s32 i;
#endif

	struct sk_buff *skb;
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;

	TR0("%s called\n", __func__);
	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	gmacdev = (struct synop_device *)adapter->synopGMACdev;

	/*Lets read the version of ip in to device structure*/
	synopGMAC_read_version(gmacdev);

	/*  jay hsu : set mac address again */
	synopGMAC_set_mac_addr(gmacdev,
			       GmacAddr0High, GmacAddr0Low, Mac_Addr);
	synopGMAC_set_mac_addr(gmacdev,
			       GmacAddr1High, GmacAddr1Low, Mac_Addr);
	synopGMAC_get_mac_addr(gmacdev,
			       GmacAddr0High, GmacAddr0Low, netdev->dev_addr);

	/*Now set the broadcast address*/
	for (ijk = 0; ijk < 6; ijk++)
		netdev->broadcast[ijk] = 0xff;

	for (ijk = 0; ijk < 6; ijk++) {
		TR("dev_addr[%d]=%02x",
		   ijk, netdev->dev_addr[ijk]);
	}
	for (ijk = 0; ijk < 6; ijk++) {
		TR("broadcast[%d]=%02x\n",
		   ijk, netdev->broadcast[ijk]);
	}

	/*Check for Phy initialization*/
	synopGMAC_set_mdc_clk_div(gmacdev, g_MDC_CLOCK);
	gmacdev->ClockDivMdc = synopGMAC_get_mdc_clk_div(gmacdev);

	gmacdev->ssLoopbackEnable = 0;

#ifndef CONFIG_NVT_INTERNAL_PHY

#ifdef USE_DEVICE_TREE
	sel_RMII_reg_val = readl(gmacdev->RmiiModeCtrlReg);
	sel_RMII_reg_val  |= 0x80000000;
	writel(sel_RMII_reg_val, gmacdev->RmiiModeCtrlReg);
#else /* not USE_DEVICE_TREE */
	ioremap_OR_value(MAC_RMII_MII_SEL_ADDR, 0x80000000);

	/* External ethernet phy reset */
	ioremap_OR_value(NVT_PHY_RESET_GPIO_ADDR + 0x8,
			 (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));

	ioremap_write(NVT_PHY_RESET_GPIO_ADDR,
		      (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));

	mdelay(1);

	ioremap_write(NVT_PHY_RESET_GPIO_ADDR + 0x4,
		      (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));

	mdelay(100);
#endif /* USE_DEVICE_TREE */

#endif /* CONFIG_NVT_INTERNAL_PHY */

	while (Phy_Init_try_count-- > 0) {
		if (NVT_Phy_Init_Done)
			break;

		/*msleep(1);
		* checkpatch.pl suggest usleep_rang
		*/
		usleep_range(1 * 1000, 2 * 1000);
	}
	if (Phy_Init_try_count <= 0)
		pr_crit("\033[1;31m[NTKETHMAC_ERROR]:Reset Digital Phy is Timeout!\033[0m\n");

	/*  Jay Hsu : check phy id until phy reset procedure done */
	count = 100;
	while (count--) {
#ifdef CONFIG_NVT_MICREL_PHY_SUPPORT
		if ((synopGMAC_get_phy_id(gmacdev) == MICREL_PHY_ID1) ||
		    (synopGMAC_get_phy_id(gmacdev) == MICREL_PHY_ID2)) {
#elif defined CONFIG_NVT_RTL_PHY_SUPPORT
		if ((synopGMAC_get_phy_id(gmacdev) == RTL8201E_PHY_ID) ||
		    (synopGMAC_get_phy_id(gmacdev) == RTL8201F_PHY_ID)) {
#elif defined CONFIG_NVT_ICPLUS_PHY_SUPPORT
		if (synopGMAC_get_phy_id(gmacdev) == ICPLUS101ALF_PHY_ID) {
#elif defined CONFIG_NVT_INTERNAL_PHY
		if (synopGMAC_get_phy_id(gmacdev) == NVT_INTERNAL_PHY_ID) {
#endif
			pr_crit("[NTKETHMAC] : %s Ethernet PHY ready\n",
				NVT_PHY_VENDOR);
			break;
		}
		mdelay(10);
	}

	if (count < 0) {
		pr_crit("\033[1;31m[NTKETHMAC_ERROR]: Not %s PHY can't initialize PHY ID = %08x\033[0m\n",
			NVT_PHY_VENDOR, gmacdev->PHY_ID);

#if DEBUG_READ_PHY_REG
		*internal_PHY_OFFSET_0120 = 0x00000001;

		for (i = 0; i <= 100; i++) {
			if (((*internal_PHY_OFFSET_0120 >> 4) & 0x1) == 1) {
				pr_crit("[NTKETHMAC_DEBUG]:PHY reg_120=%08x\n",
					*internal_PHY_OFFSET_0120);
				break;
			}
			mdelay(1);
		}

		mdelay(10);
		pr_crit("[NTKETHMAC_DEBUG]:PHY reg_12C=%08x\n",
			*internal_PHY_OFFSET_012C);
		pr_crit("[NTKETHMAC_DEBUG]:PHY reg_130=%08x\n",
			*internal_PHY_OFFSET_0130);
#endif

		return -ESYNOPGMACPHYERR;
	}

	/*  jay hsu : check phy init speed for our Micrel phy */
	synopGMAC_check_phy_init_for_10_100M(gmacdev);

#ifdef CONFIG_NVT_RTL_PHY_SUPPORT
	if (gmacdev->PHY_ID == RTL8201E_PHY_ID) {
		synopGMAC_get_phy_test_reg(gmacdev);
		synopGMAC_get_phy_LBREM_reg(gmacdev);
	} else if (gmacdev->PHY_ID == RTL8201F_PHY_ID) {
		synopGMAC_RTL8201F_set_phy_TXC_direction(gmacdev, 1);
		synopgmac_rtl8201f_set_phy_rmii_rx_timing(gmacdev, 0xD);
	}
#endif

	/*  use new IRQ definition @ 20100520 */
#ifdef USE_DEVICE_TREE
	if (request_irq(gmacdev->IRQNum, synopGMAC_intr_handler,
			0, netdev->name, netdev)) {
#else
	if (request_irq(GMAC_IRQ, synopGMAC_intr_handler,
			0, netdev->name, netdev)) {
#endif
		TR0("Error in request_irq\n");
		goto error_in_irq;
	}

	/*Set up the tx and rx descriptor queue/ring*/
	synopGMAC_setup_tx_desc_queue(gmacdev,
				      NULL, TRANSMIT_DESC_SIZE, RINGMODE);

	/* Program the transmit descriptor
	* base address in to DmaTxBase addr
	*/
	synopGMAC_init_tx_desc_base(gmacdev);

	synopGMAC_setup_rx_desc_queue(gmacdev,
				      NULL, RECEIVE_DESC_SIZE, RINGMODE);

	/* Program the transmit descriptor
	* base address in to DmaTxBase addr
	*/
	synopGMAC_init_rx_desc_base(gmacdev);

	/*  jay hsu : use burst length 8 */
	/*  jay hsu : skip 6x8 bytes to get next descriptors */
	synopGMAC_dma_bus_mode_init(gmacdev,
				    DmaBurstLength8 | DmaDescriptorSkip6);

	/*  jay hsu : don't use Operate on second frame */
	synopGMAC_dma_control_init(gmacdev,
				   DmaStoreAndForward | DmaRxThreshCtrl128);

	if (netdev->flags & IFF_PROMISC)
		synopGMAC_mac_init(gmacdev, 1);
	else
		synopGMAC_mac_init(gmacdev, 0);

	/*This enables the pause control in Full duplex mode of operation */
	synopGMAC_pause_control(gmacdev);

#ifdef IPC_OFFLOAD
	/*IPC Checksum offloading is enabled for this driver.
	* Should only be used if Full Ip checksumm offload engine
	* is configured in the hardware
	*/

	/* Enable the offload engine in the receive path */
	synopGMAC_enable_rx_chksum_offload(gmacdev);

	/*This is default configuration,
	* DMA drops the packets if error
	*in encapsulated ethernet payload
	*/
	synopGMAC_rx_tcpip_chksum_drop_enable(gmacdev);

	/* The FEF bit in DMA control register is configured to 0
	* indicating DMA to drop the errored frames.
	*/
	/*Inform the Linux Networking stack about
	* the hardware capability of checksum offloading
	*/
	netdev->features = NETIF_F_HW_CSUM;
#endif

	do {
		skb =
			alloc_skb(netdev->mtu + ETHERNET_HEADER + ETHERNET_CRC,
				  GFP_KERNEL);

		if (!skb) {
			TR0("ERROR in skb buffer allocation\n");
			synopGMAC_giveup_rx_desc_queue(gmacdev, NULL, RINGMODE);
			synopGMAC_giveup_tx_desc_queue(gmacdev, NULL, RINGMODE);
			return -ESYNOPGMACNOMEM;
		}

		skblen = skb_tailroom(skb);
		if (skblen > 0) {
			status =
				synopGMAC_set_rx_qptr(gmacdev,
						      virt_to_phys(skb->data),
						      (u32)skblen,
				(unsigned long)skb, 0, 0, 0,
				&desc_index);
		} else {
			pr_crit("Set RX descriptor fail\n");
			status = -1;
		}

		if (status < 0)
			dev_kfree_skb_any(skb);

	} while (status == 0);

	TR("Setting up the cable unplug timer\n");
	init_timer(&synopGMAC_cable_unplug_timer);
	synopGMAC_cable_unplug_timer.function =
		(void *)synopGMAC_linux_cable_unplug_function;
	synopGMAC_cable_unplug_timer.data = (unsigned long)adapter;
	synopGMAC_cable_unplug_timer.expires = jiffies + (HZ / 2);
	add_timer(&synopGMAC_cable_unplug_timer);

	synopGMAC_clear_interrupt(gmacdev);
	/*Disable the interrupts generated by MMC and IPC counters.
	* If these are not disabled ISR should be modified accordingly
	* to handle these interrupts.
	*/
	synopGMAC_disable_mmc_tx_interrupt(gmacdev, 0xFFFFFFFF);
	synopGMAC_disable_mmc_rx_interrupt(gmacdev, 0xFFFFFFFF);
	synopGMAC_disable_mmc_ipc_rx_interrupt(gmacdev, 0xFFFFFFFF);

	synopGMAC_enable_interrupt(gmacdev, DmaIntEnable);
	synopGMAC_enable_dma_rx(gmacdev);
	synopGMAC_enable_dma_tx(gmacdev);

	/* Power up MAC */
	synopGMAC_power_down_disable(gmacdev);

	if (gmacdev->LinkState == 1)
		netif_carrier_on(netdev);
	else
		netif_carrier_off(netdev);

	netif_start_queue(netdev);

#ifdef	CONFIG_NVT_FASTETH_MAC_NAPI
	napi_enable(&nvt_napi);
#endif

	return retval;

error_in_irq:
	/*Lets free the allocated memory*/
	plat_free_memory(gmacdev);
	return -ESYNOPGMACBUSY;
}

/**
 * Function used when the interface is closed.
 *
 * This function is registered to linux stop() function. This function is
 * called whenever ifconfig (in Linux) closes the device
 * (for example "ifconfig eth0 down").
 * This releases all the system resources
 * allocated during open call.
 * system resources int needs
 *- Disable the device interrupts
 *- Stop the receiver and get back
 * all the rx descriptors from the DMA
 *- Stop the transmitter and get back
 * all the tx descriptors from the DMA
 *- Stop the Linux network queue interface
 *- Free the irq
 * (ISR registered is removed from the kernel)
 *- Release the TX and RX descripor memory
 *- De-initialize one second timer rgistered
 * for cable plug/unplug tracking
 * @param[in] pointer to net_device structure.
 * \return Returns 0 on success and error status upon failure.
 * \callgraph
 */

s32 synopGMAC_linux_close(struct net_device *netdev)
{
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;

	TR0("%s\n", __func__);
	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	if (!adapter) {
		TR0("OOPS adapter is null\n");
		return -1;
	}

	gmacdev = (struct synop_device *)adapter->synopGMACdev;
	if (!gmacdev) {
		TR0("OOPS gmacdev is null\n");
		return -1;
	}

	/*Disable all the interrupts*/
	synopGMAC_disable_interrupt_all(gmacdev);
	TR("the synopGMAC interrupt has been disabled\n");

	/*Disable the reception*/
	synopGMAC_disable_dma_rx(gmacdev);
	synopGMAC_take_desc_ownership_rx(gmacdev);
	TR("the synopGMAC Reception has been disabled\n");

	/*Disable the transmission*/
	synopGMAC_disable_dma_tx(gmacdev);
	synopGMAC_take_desc_ownership_tx(gmacdev);

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
	napi_disable(&nvt_napi);
#endif

	TR("the synopGMAC Transmission has been disabled\n");
	netif_stop_queue(netdev);

	/*  jay hsu : free GMAC irq */
#ifdef USE_DEVICE_TREE
	free_irq(gmacdev->IRQNum, netdev);
#else
	free_irq(GMAC_IRQ, netdev);
#endif
	TR("the synopGMAC interrupt handler has been removed\n");

	/*Free the Rx Descriptor contents*/
	TR("Now calling synopGMAC_giveup_rx_desc_queue\n");
	synopGMAC_giveup_rx_desc_queue(gmacdev, NULL, RINGMODE);

	TR("Now calling synopGMAC_giveup_tx_desc_queue\n");
	synopGMAC_giveup_tx_desc_queue(gmacdev, NULL, RINGMODE);

	TR("Freeing the cable unplug timer\n");

	del_timer(&NVT_Phy_InitTimer);

	del_timer(&synopGMAC_cable_unplug_timer);

	return -ESYNOPGMACNOERR;
}

/**
 * Function to transmit a given packet on the wire.
 * Whenever Linux Kernel has a packet ready
 * to be transmitted, this function is called.
 * The function prepares a packet
 * and prepares the descriptor and
 * enables/resumes the transmission.
 * @param[in] pointer to sk_buff structure.
 * @param[in] pointer to net_device structure.
 * \return Returns 0 on success and Error code on failure.
 * \note structure sk_buff is used
 * to hold packet in Linux networking stacks.
 */
s32 synopGMAC_linux_xmit_frames(struct sk_buff *skb, struct net_device *netdev)
{
	s8 status = 0;
	unsigned long desc_index;
	u32 offload_needed = 0;
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;

	unsigned long irq_flags;/*  jay hsu : add tx lock support */

	TR("%s called\n", __func__);
	if (!skb) {
		TR0("skb is NULL What happened to Linux Kernel?\n ");
		return -1;
	}

	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	if (!adapter)
		return -1;

	gmacdev = (struct synop_device *)adapter->synopGMACdev;
	if (!gmacdev)
		return -1;

	if (gmacdev->ssLoopbackEnable)
		return NETDEV_TX_BUSY;

	/*2.6.30 use UDP-lite,
	* then skb->ip_summed will set by CHECKSUM_PARTIAL
	*/
	if (skb->ip_summed == CHECKSUM_COMPLETE ||
	    skb->ip_summed == CHECKSUM_PARTIAL) {
		/* In Linux networking, if kernel indicates
		* skb->ip_summed = CHECKSUM_HW,
		* then only checksum offloading should be performed
		* Make sure that the OS on which this code
		* runs have proper support to enable offloading.
		*/
		offload_needed = 0x00000001;
	}

	/*  jay hsu : debug */
	TR("##send frame## skb data 0x%x, len %d\n",
	   (unsigned long)skb->data, skb->len);
	TR("offload needed %d\n", offload_needed);

	/*  jay hsu @ 20100727 : start spin_lock protection */
	spin_lock_irqsave(&gmacdev->tx_lock, irq_flags);
	status =
		synopGMAC_set_tx_qptr(gmacdev,
				      virt_to_phys(skb->data),
				      skb->len,
				      (unsigned long)skb,
		0, 0, 0, offload_needed, &desc_index);

	if (status < 0) {
		pr_crit("%s No More Free Tx Descriptors\n", __func__);

		/*  jay hsu : fix the errno @ 20100727 */
		spin_unlock_irqrestore(&gmacdev->tx_lock, irq_flags);
		synopGMAC_linux_reset_mac(netdev, gmacdev);
		return NETDEV_TX_BUSY;
	}

	/*Now force the DMA to start transmission*/
	synopGMAC_resume_dma_tx(gmacdev);
	spin_unlock_irqrestore(&gmacdev->tx_lock, irq_flags);
	netdev->trans_start = jiffies;

	return -ESYNOPGMACNOERR;
}

/**
 * Function provides the network interface statistics.
 * Function is registered to linux get_stats() function. This function is
 * called whenever ifconfig (in Linux) asks for networkig statistics
 * (for example "ifconfig eth0").
 * @param[in] pointer to net_device structure.
 * \return Returns pointer to net_device_stats structure.
 * \callgraph
 */
struct net_device_stats *synopGMAC_linux_get_stats(struct net_device *netdev)
{
	TR("%s called\n", __func__);
	return(&(((struct synop_adapter_struct *)(netdev_priv(netdev)))->
		synop_stats));
}

/**
 * Function to set multicast and promiscous mode.
 * @param[in] pointer to net_device structure.
 * \return returns void.
 */
void synopGMAC_linux_set_multicast_list(struct net_device *netdev)
{
	TR("%s called\n", __func__);
	/* todo Function not yet implemented. */
}

/**
 * Function to set ethernet address of the NIC.
 * @param[in] pointer to net_device structure.
 * @param[in] pointer to an address structure.
 * \return Returns 0 on success Errorcode on failure.
 */
s32 synopGMAC_linux_set_mac_address(struct net_device *netdev, void *macaddr)
{
	struct synop_adapter_struct *adapter = NULL;
	struct synop_device *gmacdev = NULL;
	struct sockaddr *addr = macaddr;

	/* adapter = (struct synop_adapter_struct *) netdev->priv; */
	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	if (!adapter)
		return -1;

	gmacdev = adapter->synopGMACdev;
	if (!gmacdev)
		return -1;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	/* Only set mac address when net device is down. */
	if (netif_running(netdev))
		return -EBUSY;

	/* Save current mac address in dram */
	memcpy(Mac_Addr, addr->sa_data, sizeof(Mac_Addr));

	synopGMAC_set_mac_addr(gmacdev,
			       GmacAddr0High, GmacAddr0Low, addr->sa_data);
	synopGMAC_set_mac_addr(gmacdev,
			       GmacAddr1High, GmacAddr1Low, addr->sa_data);
	synopGMAC_get_mac_addr(gmacdev,
			       GmacAddr0High, GmacAddr0Low, netdev->dev_addr);

	TR("%s called\n", __func__);
	return 0;
}

/**
 * Function to change the Maximum Transfer Unit.
 * @param[in] pointer to net_device structure.
 * @param[in] New value for maximum frame size.
 * \return Returns 0 on success Errorcode on failure.
 */
s32 synopGMAC_linux_change_mtu(struct net_device *netdev, s32 newmtu)
{
	TR("%s called\n", __func__);
	/*todo Function not yet implemented.*/
	return 0;
}

/**
 * IOCTL interface.
 * This function is mainly for debugging purpose.
 * This provides hooks for Register read write, Retrieve descriptor status
 * and Retreiving Device structure information.
 * @param[in] pointer to net_device structure.
 * @param[in] pointer to ifreq structure.
 * @param[in] ioctl command.
 * \return Returns 0 on success Error code on failure.
 */
s32 synopGMAC_linux_do_ioctl(struct net_device *netdev,
			     struct ifreq *ifr, s32 cmd)
{
	s32 retval = 0;
	u32 currdescaddr;
	u16 temp_data = 0;
	struct synop_adapter_struct *adapter = NULL;
	struct synop_device *gmacdev = NULL;

	struct ifr_data_struct {
		u32 unit;
		unsigned long addr;
		u32 data;
		u32 len;
	} *req;

	/* For mii ioctl command*/
	struct mii_ioctl_data *mii_data;
	struct dma_desc *currdesc;

	if (!netdev)
		return -1;
	if (!ifr)
		return -1;

	req = (struct ifr_data_struct *)ifr->ifr_data;

	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	if (!adapter)
		return -1;

	gmacdev = adapter->synopGMACdev;
	if	(!gmacdev)
		return -1;

	/*check if the net device is up.*/
	if (!netif_running(netdev))
		return -ENODEV;

	switch (cmd) {
	/*IOCTL for reading IP registers : Read Registers*/
	case IOCTL_READ_REGISTER:
		if (req->unit == 0) {
			/* Read Mac Register*/
			req->data =
			synopGMACReadReg((unsigned long *)gmacdev->MacBase,
					 req->addr);
		} else if (req->unit == 1) {
			/* Read DMA Register*/
			req->data =
			synopGMACReadReg((unsigned long *)gmacdev->DmaBase,
					 req->addr);
		} else if (req->unit == 2) {
			/*Read Phy Register*/
			retval = synopGMAC_read_phy_reg(
					(unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					req->addr, &temp_data);

			req->data = (u32)temp_data;
#ifdef DEBUG
			if (retval != -ESYNOPGMACNOERR)
				TR("ERROR in Phy read\n");
#endif
		}
		break;

	/*IOCTL for reading IP registers : Read Registers*/
	case IOCTL_WRITE_REGISTER:
		if (req->unit == 0) {
			/* Write Mac Register*/
			synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
					  req->addr, req->data);
		} else if (req->unit == 1) {
			/*Write DMA Register*/
			pr_info("GMAC driver : addr 0x%lu, data 0x%x\n",
				req->addr, req->data);

			synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
					  req->addr, req->data);

		} else if (req->unit == 2) {
			/*Write Phy Register*/
			retval =
				synopGMAC_write_phy_reg(
				(unsigned long *)gmacdev->MacBase,
				gmacdev->PhyBase, req->addr,
				(u16)(req->data & 0xffff));

#ifdef DEBUG
			if (retval != -ESYNOPGMACNOERR)
				TR("ERROR in Phy read\n");
#endif
		}
		break;

	/*IOCTL for reading GMAC DEVICE IP private structure*/
	case IOCTL_READ_IPSTRUCT:
		memcpy(ifr->ifr_data,
		       gmacdev, sizeof(struct synop_device));
		break;

	/*IOCTL for Reading Rx DMA DESCRIPTOR*/
	case IOCTL_READ_RXDESC:
		memcpy(ifr->ifr_data,
		       gmacdev->RxDesc +
			((struct dma_desc *)(ifr->ifr_data))->data1,
			sizeof(struct dma_desc));
		break;

	/*IOCTL for Reading Tx DMA DESCRIPTOR*/
	case IOCTL_READ_TXDESC:
		memcpy(ifr->ifr_data,
		       gmacdev->TxDesc +
			((struct dma_desc *)(ifr->ifr_data))->data1,
			sizeof(struct dma_desc));
		break;

	case IOCTL_POWER_DOWN:
		if (req->unit == 1) {
			/* power down the mac */
			TR("=I will Power down the MAC now=\n");
			pr_crit("=I will Power down the MAC now=\n");
			/* If it is already in power down
			* don't power down again
			*/
			retval = 0;
			if (((synopGMACReadReg(
				(unsigned long *)gmacdev->MacBase,
				GmacPmtCtrlStatus)) & GmacPmtPowerDown) !=
				GmacPmtPowerDown) {
				synopGMAC_linux_powerdown_mac(gmacdev);
				retval = 0;
			}
		}
		if (req->unit == 2) {
			/* Disable the power down
			* and wake up the Mac locally
			*/
			TR("=I will Power up the MAC now =\n");
			pr_crit("=I will Power up the MAC now =\n");
			/* If already powered down
			* then only try to wake up
			*/
			retval = -1;
			if (((synopGMACReadReg(
				(unsigned long *)gmacdev->MacBase,
				GmacPmtCtrlStatus)) & GmacPmtPowerDown) ==
				GmacPmtPowerDown) {
				synopGMAC_power_down_disable(gmacdev);
				synopGMAC_linux_powerup_mac(gmacdev);
				retval = 0;
			}
		}
		break;

	case IOCTL_CHK_CONN:
		pr_info("link status %d\n", gmacdev->LinkState);
		memcpy(ifr->ifr_data, &gmacdev->LinkState,
		       sizeof(gmacdev->LinkState));
		break;

	case IOCTL_PHY_LOOPBACK_SETUP:
		if (!req) {
			pr_crit("req is NULL\n");
			break;
		}

		if (req->unit == 0)
			synopGMAC_phy_loopback(gmacdev, 0);
		else
			synopGMAC_phy_loopback(gmacdev, 1);
		break;

	case IOCTL_SS_SELF_TEST_MODE_ON_OFF:
		if (!req) {
			pr_crit("req is NULL\n");
			break;
		}

		if (req->unit == 1) {
			pr_crit("please plug the test tool in Ethernet port\n");

			synopGMAC_frame_filter_disable(gmacdev);
			synopGMAC_rx_tcpip_chksum_drop_disable(gmacdev);

			SS_selftest_frame = dma_alloc_coherent(
			NULL,
			MAX_ETHERNET_PAYLOAD + ETHERNET_PACKET_EXTRA,
			&SS_selftest_frame_phys, GFP_KERNEL);

			gmacdev->ssLoopbackEnable = 1;
			pr_crit("SS Loopback Mode on\n");
		} else if (req->unit == 0) {
			pr_crit("SS Loopback Mode off\n");
			gmacdev->ssLoopbackEnable = 0;
			synopGMAC_frame_filter_enable(gmacdev);
			synopGMAC_rx_tcpip_chksum_drop_enable(gmacdev);

			dma_free_coherent(NULL,
					  (MAX_ETHERNET_PAYLOAD +
					  ETHERNET_PACKET_EXTRA),
			SS_selftest_frame, SS_selftest_frame_phys);
		}
		gmacdev->ssLoopbackErrorPkt = 0;
		gmacdev->ssLoopbackPassPkt = 0;
		break;

	case IOCTL_SS_SELF_TEST_SEND:
		gmacdev->ssLoopbackErrorPkt = 0;
		gmacdev->ssLoopbackPassPkt = 0;
		synopGMAC_SS_self_test_xmit_frames(gmacdev);
		break;

	case IOCTL_SS_SELF_TEST_GET_RESULT:
		if (!req) {
			pr_crit("req is NULL\n");
			break;
		}

		if (gmacdev->ssLoopbackPassPkt)
			req->unit = 1;
		else if (gmacdev->ssLoopbackErrorPkt)
			req->unit = 2;

		break;

	case IOCTL_SS_SELF_TEST_SET_FRAME:
		if (!req) {
			pr_crit("req is NULL\n");
			break;
		}

		if (req->len > MAX_ETHERNET_PAYLOAD)
			retval = -1;

		memset(SS_selftest_frame, 0,
		       MAX_ETHERNET_PAYLOAD + ETHERNET_PACKET_EXTRA);
		copy_from_user((void *)SS_selftest_frame,
			       (const void __user *)(req->addr),
			(unsigned long)req->len);
		gmacdev->ssLoopbackFrameSize = req->len;
		break;

	case IOCTL_RESET_PHY:
		pr_crit("MAC driver do PHY Reset\n");
#if (!defined(CONFIG_NVT_INTERNAL_PHY))
		ioremap_OR_value(NVT_PHY_RESET_GPIO_ADDR + 0x8,
				 (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));
		ioremap_write(NVT_PHY_RESET_GPIO_ADDR,
			      (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));
		mdelay(1);
		ioremap_write(NVT_PHY_RESET_GPIO_ADDR + 0x4,
			      (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));
#endif
		break;

	case IOCTL_RESET_MAC:
		pr_crit("MAC driver do MAC Reset\n");
		synopGMAC_reset(gmacdev);
		synopGMAC_linux_close(netdev);
		synopGMAC_linux_open(netdev);
		break;

	case IOCTL_SHOW_DMA_STATUS:
		currdescaddr =
			synopGMAC_get_dma_current_TX_desc(gmacdev);
		pr_info("Current description address = 0x%08x\n",
			currdescaddr);
		currdesc = (struct dma_desc *)phys_to_virt(currdescaddr);
		pr_info("Current des. status=0x%08x, length=0x%08x\n",
			currdesc->status, currdesc->length);
		break;

	case SIOCGMIIPHY:
		mii_data = if_mii(ifr);
		mii_data->phy_id = gmacdev->PhyBase & 0x1f;
		break;

	case SIOCGMIIREG:
		mii_data = if_mii(ifr);
		/*check return value is necessary*/
		retval = synopGMAC_read_phy_reg(
		(unsigned long *)gmacdev->MacBase,
		gmacdev->PhyBase, mii_data->reg_num,
		&mii_data->val_out);
		break;

	case SIOCSMIIREG:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		mii_data = if_mii(ifr);
		/*check return value is necessary*/
		retval = synopGMAC_write_phy_reg(
		(unsigned long *)gmacdev->MacBase,
		gmacdev->PhyBase, mii_data->reg_num,
		mii_data->val_in);
		break;
	default:
		retval = -1;
	}

	return retval;
}

/**
 * Function to handle a Tx Hang.
 * This is a software hook (Linux) to handle transmitter hang if any.
 * We get transmitter hang in the device interrupt status, and is handled
 * in ISR. This function is here as a place holder.
 * @param[in] pointer to net_device structure
 * \return void.
 */
void synopGMAC_linux_tx_timeout(struct net_device *netdev)
{
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev = NULL;

	pr_crit("%s called\n", __func__);

	adapter = netdev_priv(netdev);
	if (!adapter) {
		TR("Unknown Device\n");
		return;
	}

	gmacdev = adapter->synopGMACdev;
	if (!gmacdev) {
		TR("GMAC device structure is missing\n");
		return;
	}

	synopGMAC_linux_reset_mac(netdev, gmacdev);

	adapter->synop_stats.tx_errors++;
}

void synopGMAC_linux_change_rx_flags(struct net_device *netdev,
				     int flags)
{
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev = NULL;

	adapter = netdev_priv(netdev);
	if (!adapter) {
		TR("Unknown Device\n");
		return;
	}

	gmacdev = adapter->synopGMACdev;
	if (!gmacdev) {
		TR("GMAC device structure is missing\n");
		return;
	}

	if (netdev->flags & IFF_PROMISC) {
		synopGMAC_frame_filter_disable(gmacdev);
		synopGMAC_src_addr_filter_enable(gmacdev);
		synopGMAC_src_addr_filter_inverse(gmacdev);
		pr_crit("[NTKETHMAC]: Promiscuous Mode is enabled.\n");
	} else {
		synopGMAC_frame_filter_enable(gmacdev);
		synopGMAC_src_addr_filter_disable(gmacdev);
		synopGMAC_src_addr_filter_normal(gmacdev);
		pr_crit("[NTKETHMAC]: Promiscuous Mode is disabled.\n");
	}
}

/*polling function*/
#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
int synopGMAC_linux_poll(struct napi_struct *napi, int budget)
{
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;
	struct net_device *netdev = napi->dev;
	u32 desc_index;
	s8 ret;
	u32 data1;
	u32 data2;
	u32 len;
	s32 skblen;
	u32 status;
	u32 dma_addr1;
	u32 dma_addr2;
#ifdef ENH_DESC_8W
	u32 ext_status;
	u16 ts_higher;
	u32 time_stamp_high;
	u32 time_stamp_low;
#endif
	u32 polling_packets = 0;

	/* This is the pointer to hold the received data */
	struct sk_buff *skb;

	TR("%s\n", __func__);

	adapter = netdev_priv(netdev);
	if (!adapter) {
		TR("Unknown Device\n");
		return 1;
	}

	gmacdev = adapter->synopGMACdev;
	if (!gmacdev) {
		TR("GMAC device structure is missing\n");
		return 1;
	}

	/*Handle the Receive Descriptors*/
	do {
#ifdef ENH_DESC_8W
		ret = synopGMAC_get_rx_qptr(gmacdev,
					    &status, &dma_addr1, NULL, &data1,
			&dma_addr2, NULL, &data2, &ext_status,
			&time_stamp_high, &time_stamp_low,
			&desc_index);
		if (ret == 0) {
			synopGMAC_TS_read_timestamp_higher_val(gmacdev,
							       &ts_higher);
			/* debug only
			* TR("S:%08x ES:%08x DA1:%08x d1:%08x "
			* "DA2:%08x d2:%08x TSH:%08x "
			* "TSL:%08x TSHW:%08x\n",
			* status, ext_status, dma_addr1,
			* data1, dma_addr2, data2,
			* time_stamp_high, time_stamp_low,
			* ts_higher);
			*/
		}
#else
		ret = synopGMAC_get_rx_qptr(gmacdev,
					    &status, &dma_addr1, NULL, &data1,
			&dma_addr2, NULL, &data2, &desc_index);
			/*debug only
			* TR("##Handle Received## desc_index %d,"
			* "dma_addr1 0x%x, data1 0x%x\n",
			* desc_index, (u32)dma_addr1, data1);
			*/
#endif

		if (ret == 0 && data1 != 0) {
			/* For debug only:
			* TR("Received Data at Rx Descriptor %d "
			* "for skb 0x%08x whose status is %08x\n",
			* desc_index, data1, status);
			*/

			skb = (struct sk_buff *)data1;
			if (synopGMAC_is_rx_desc_valid(status)) {
				/* Not interested in Ethernet CRC bytes */
				len =
				synopGMAC_get_rx_desc_frame_length(status) - 4;
				skb_put(skb, len);
#ifdef IPC_OFFLOAD
		/* Now lets check for the IPC offloading */
		/* Since we have enabled the checksum offloading
		* in hardware, lets inform the kernel
		* not to perform the checksum computation
		* on the incoming packet. Note that ip header checksum
		* will be computed by the kernel immaterial of what we inform.
		* Similarly TCP / UDP / ICMP
		* pseudo header checksum will be computed by the stack.
		* What we can inform is not to perform
		* payload checksum.
		* When CHECKSUM_UNNECESSARY is set
		* kernel bypasses the checksum computation.
		*/

				TR("Checksum Offloading will be done now\n");
				skb->ip_summed = CHECKSUM_UNNECESSARY;

#ifdef ENH_DESC_8W
				if (synopGMAC_is_ext_status(gmacdev, status)) {
					/*  extended status present indicates
					* that the RDES4 need to be probed
					*/
					TR("Extended Status present\n");
					if (synopGMAC_ES_is_IP_header_error(
						gmacdev, ext_status)) {
						/*  IP header (IPV4)
						* checksum error
						*/
						/* Linux Kernel doesnot care
						* for ipv4 header checksum.
						* So we will simply proceed
						* by printing a warning ....
						*/
						TR("(ES)IP hdr error\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
				/*checkpatch, over 80 chars*/
				if (synopGMAC_ES_is_rx_checksum_bypassed(
					gmacdev, ext_status)) {
					/*  Hardware engine bypassed
					* the checksum
					* computation/checking
					*/
					TR("(ES)HW Bypass CRC\n");
					skb->ip_summed = CHECKSUM_NONE;
				}
					if (synopGMAC_ES_is_IP_payload_error(
						gmacdev, ext_status)) {
						/*  IP payload checksum is
						* in error
						* (UDP/TCP/ICMP checksum error)
						*/
						TR("(ES)EP payload err.\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
				} else {
					/*  No extended status.
					* So relevant information is available
					* in the status itself
					*/
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxNoChkError) {
						TR("RxNoChkError\n");
						TR("<Chk Status = 4>\n");
						skb->ip_summed =
							CHECKSUM_UNNECESSARY;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxIpHdrChkError) {
						/* Linux Kernel doesnot care for
						* ipv4 header checksum.
						* So we will simply proceed
						* by printing a warning ....
						*/
						TR("RxIpHdrChkError\n");
						TR("<Chk Status = 6>\n");
						skb->ip_summed =
							CHECKSUM_UNNECESSARY;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxLenLT600) {
						TR("RxLenLT600\n");
						TR("<Chk Status = 0>\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxIpHdrPayLoadChkBypass) {
						TR("RxIpHdrPayLoadChkBypass\n");
						TR("<Chk Status = 1>\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxChkBypass) {
						TR("RxChkBypass\n");
						TR("<Chk Status = 3>\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxPayLoadChkError) {
						TR("RxPayLoadChkError\n");
						TR("<Chk Status = 5>\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
					if (synopGMAC_is_rx_checksum_error(
						gmacdev, status) ==
						RxIpHdrChkError) {
						/* Linux Kernel doesnot care for
						* ipv4 header checksum.
						* So we will simply proceed
						* by printing a warning ....
						*/
						TR("RxIpHdrChkError\n");
						TR("<Chk Status = 7>\n");
						skb->ip_summed = CHECKSUM_NONE;
					}
				}
#else
				if (synopGMAC_is_rx_checksum_error(gmacdev,
								   status) ==
					RxNoChkError) {
					/*Ip hdr and TCP/UDP
					* payload CRC Bypassed
					*/
					TR("RxNoChkError\n");
					TR("<Chk Status = 4>\n");
					/* Let Kernel bypass computing
					* the Checksum
					*/
					skb->ip_summed = CHECKSUM_UNNECESSARY;
				}
				if (synopGMAC_is_rx_checksum_error(gmacdev,
								   status) ==
					RxIpHdrChkError) {
					/* Linux Kernel doesnot care
					* for ipv4 header checksum.
					* So we will simply proceed
					* by printing a warning ....
					*/
					/*Error in 16bit IPV4 Header CRC*/
					TR("RxIpHdrChkError\n");
					TR("<Chk Status = 6>\n");
					/* Let Kernel bypass the TCP/UDP
					* checksum computation
					*/
					skb->ip_summed = CHECKSUM_UNNECESSARY;
				}
				if (synopGMAC_is_rx_checksum_error(gmacdev,
								   status) ==
					RxLenLT600) {
					/*802.3 type frame with LEN field
					* less than 0x0600
					*/
					TR("RxLenLT600\n");
					TR("<Chk Status = 0>\n");
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(gmacdev,
								   status) ==
					RxIpHdrPayLoadChkBypass) {
					/*Ip hdr and TCP/UDP
					* payload CRC Bypassed
					*/
					TR("RxIpHdrPayLoadChkBypass\n");
					TR("<Chk Status = 1>\n");
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(gmacdev,
								   status) ==
					RxChkBypass) {
					/*IP hdr and TCP/UDP
					* payload CRC Bypassed
					*/
					TR("RxChkBypass\n");
					TR("<Chk Status = 3>\n");
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(gmacdev,
								   status) ==
					RxPayLoadChkError) {
					/*TCP/UDP payload CRC Error*/
					TR("RxPayLoadChkError\n");
					TR("<Chk Status = 5>\n");
					/* Let Kernel compute the Checksum */
					skb->ip_summed = CHECKSUM_NONE;
				}
				if (synopGMAC_is_rx_checksum_error(gmacdev,
								   status) ==
					RxIpHdrChkError) {
					/* Linux Kernel doesnot care
					* for ipv4 header checksum.
					* So we will simply proceed
					* by printing a warning ....
					*/
					/*IP hdr and Payload CRC Error*/
					TR("RxIpHdrChkError\n");
					TR("<Chk Status = 7>\n");
					/* Let Kernel compute the Checksum */
					skb->ip_summed = CHECKSUM_NONE;
				}

#endif
#endif /*IPC_OFFLOAD*/
				skb->dev = netdev;
				skb->protocol = eth_type_trans(skb, netdev);
				netif_receive_skb(skb);

				netdev->last_rx = jiffies;
				adapter->synop_stats.rx_packets++;
				adapter->synop_stats.rx_bytes += len;

				/* increase packet counters*/
				polling_packets++;
			} else {
				/*Now the present skb should be set free*/
				dev_kfree_skb_any(skb);
				pr_crit("status : %08x\n", status);
				adapter->synop_stats.rx_errors++;
				adapter->synop_stats.collisions +=
					synopGMAC_is_rx_frame_collision(status);
				adapter->synop_stats.rx_crc_errors +=
					synopGMAC_is_rx_crc(status);
				adapter->synop_stats.rx_frame_errors +=
				synopGMAC_is_frame_dribbling_errors(status);
				adapter->synop_stats.rx_length_errors +=
				synopGMAC_is_rx_frame_length_errors(status);
			}

			/*Now lets allocate the skb for the emptied descriptor*/
			skb =
				dev_alloc_skb(netdev->mtu +
				ETHERNET_PACKET_EXTRA);

			if (!skb) {
				TR("SKB memory allocation failed\n");
				show_skb_alloc_error();
				adapter->synop_stats.rx_dropped++;
				return 1;
			}

			skblen = skb_tailroom(skb);
			if (skblen > 0) {
				ret =
				synopGMAC_set_rx_qptr(gmacdev,
						      virt_to_phys(skb->data),
					(u32)skblen,
					(unsigned long)skb, 0, 0, 0,
					&desc_index);
			} else {
				ret = -1;
			}
			if (ret < 0) {
				TR("Cannot set Rx Descriptor for skb %lu\n",
				   (unsigned long)skb);
				pr_crit("Cannot set Rx Descriptor for skb %lu\n",
					(unsigned long)skb);
				dev_kfree_skb_any(skb);
			}
		}
	} while (ret == 0 && (polling_packets < (budget - 1)));

	napi_complete(napi);
	atomic_set(&napi_poll, 0);
	synopGMAC_enable_interrupt(gmacdev,
				   synopGMAC_get_interrupt_mask(gmacdev) |
				   DmaIntRxNormMask);

	return polling_packets;
}
#endif

static const struct of_device_id of_platform_synopGMAC_table[];
static int synopGMAC_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;

	pr_crit("[NTKETHMAC]: %s.\n", __func__);

	match = of_match_device(of_platform_synopGMAC_table, &pdev->dev);
	if (!match)
		return -EINVAL;

	return 0;
}

static int synopGMAC_suspend(struct device *pdev)
{
	struct net_device *netdev = synopGMACadapter->synopGMACnetdev;
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;

	pr_crit("%s: START\n", __func__);

	if (!netdev) {
		pr_crit("%s: OOPS net dev is null\n", __func__);
		return 0;
	}

	/* use netdev_priv() @ 20100520*/
	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	if (!adapter) {
		pr_crit("%s: OOPS adapter is null\n", __func__);
		return 0;
	}

	gmacdev = (struct synop_device *)adapter->synopGMACdev;
	if (!gmacdev) {
		pr_crit("%s: OOPS gmacdev is null\n", __func__);
		return 0;
	}

	if (netif_running(netdev)) {
		netif_device_detach(netdev);

#ifdef USE_DEVICE_TREE
		disable_irq(gmacdev->IRQNum);
#else
		disable_irq(GMAC_IRQ);
#endif

		flush_workqueue(plugDetect_Wq);

		del_timer(&NVT_Phy_InitTimer);

		del_timer(&synopGMAC_cable_unplug_timer);

		/*Disable all the interrupts*/
		synopGMAC_disable_interrupt_all(gmacdev);

		/*Disable the reception*/
		synopGMAC_disable_dma_rx(gmacdev);
		synopGMAC_take_desc_ownership_rx(gmacdev);

		/*Disable the transmission*/
		synopGMAC_disable_dma_tx(gmacdev);
		synopGMAC_take_desc_ownership_tx(gmacdev);

		/*Free the TX Rx Descriptor contents*/
		synopGMAC_giveup_rx_desc_queue(gmacdev, NULL, RINGMODE);
		synopGMAC_giveup_tx_desc_queue(gmacdev, NULL, RINGMODE);

		synopGMAC_pmt_int_enable(gmacdev);
		synopGMAC_magic_packet_enable(gmacdev);

		/* Power off MAC */
		synopGMAC_power_down_enable(gmacdev);
	}

	/* Set AXI and AHB RESET of MAC */
	ioremap_OR_value(MAC_AXI_CLK_RESET, 0x10000000);
	ioremap_AND_value(MAC_STBC_CTRL_SWITCH_ADDR, ~(0x00000100));
	ioremap_AND_value(MAC_CLK_SRC_SWITCH_ADDR,   ~(0x0000000C));
	ioremap_AND_value(MAC_AXI_CLK_RESET, (u32)(~(0x10000000)));

	pr_crit("%s: END\n", __func__);
	return 0;
}

static int synopGMAC_resume(struct device *pdev)
{
	s8 status;
	s32 skblen;
	unsigned long desc_index;

#ifdef USE_DEVICE_TREE
#ifndef CONFIG_NVT_INTERNAL_PHY
	u32 sel_RMII_reg_val;
#endif
#endif
	struct sk_buff *skb;
	struct synop_adapter_struct *adapter;
	struct synop_device *gmacdev;
	struct net_device *netdev = synopGMACadapter->synopGMACnetdev;

	if (!netdev) {
		pr_crit("%s: OOPS net dev is null\n", __func__);
		return 0;
	}

	/*use netdev_priv() @ 2010520*/
	adapter = (struct synop_adapter_struct *)netdev_priv(netdev);
	if (!adapter) {
		pr_crit("%s:OOPS adapter is null\n", __func__);
		return 0;
	}
	gmacdev = (struct synop_device *)adapter->synopGMACdev;
	if (!gmacdev) {
		pr_crit("%s: OOPS gmacdev is null\n", __func__);
		return 0;
	}

	/* Set AXI and AHB RESET of MAC */
	ioremap_OR_value(MAC_STBC_CTRL_SWITCH_ADDR,  0x00000100);
	ioremap_OR_value(MAC_CLK_SRC_SWITCH_ADDR,    0x0000000C);
	ioremap_AND_value(MAC_AXI_CLK_RESET, (u32)(~(0x20000000)));

	if (netif_running(netdev)) {
		SysPlat_Init_Ethernet_Phy(gmacdev, 1);

		synopGMAC_set_mac_addr(gmacdev,
				       GmacAddr0High, GmacAddr0Low, Mac_Addr);
		synopGMAC_set_mac_addr(gmacdev,
				       GmacAddr1High, GmacAddr1Low, Mac_Addr);

		synopGMAC_set_mdc_clk_div(gmacdev, g_MDC_CLOCK);

		synopGMAC_setup_tx_desc_queue(gmacdev,
					      NULL,
					      TRANSMIT_DESC_SIZE,
					      RINGMODE);

		/*Program the transmit descriptor base address
		* in to DmaTxBase addr
		*/
		synopGMAC_init_tx_desc_base(gmacdev);
		synopGMAC_setup_rx_desc_queue(gmacdev,
					      NULL,
					      RECEIVE_DESC_SIZE,
					      RINGMODE);
		/*Program the transmit descriptor base address
		* in to DmaTxBase addr
		*/
		synopGMAC_init_rx_desc_base(gmacdev);

		/*skip 6x8 bytes to get next descriptors*/
		synopGMAC_dma_bus_mode_init(gmacdev,
					    DmaBurstLength8 |
					    DmaDescriptorSkip6);
		synopGMAC_dma_control_init(gmacdev,
					   DmaStoreAndForward |
					   DmaRxThreshCtrl128);

		if (netdev->flags & IFF_PROMISC)
			synopGMAC_mac_init(gmacdev, 1);
		else
			synopGMAC_mac_init(gmacdev, 0);

		/* This enables the pause control
		* in Full duplex mode of operation
		*/
		synopGMAC_pause_control(gmacdev);

		do {
			skb =
				alloc_skb(
					(netdev->mtu +
					ETHERNET_HEADER +
					ETHERNET_CRC),
					GFP_ATOMIC);

			if (!skb) {
				TR0("ERROR in skb buffer allocation\n");
				break;
			}

			skblen = skb_tailroom(skb);
			if (skblen > 0) {
				status =
				synopGMAC_set_rx_qptr(gmacdev,
						      virt_to_phys(skb->data),
					(u32)skblen,
					(unsigned long)skb, 0, 0, 0,
					&desc_index);
			} else {
				status = -1;
			}
			if (status < 0)
				dev_kfree_skb_any(skb);
		} while (status == 0);

#ifndef CONFIG_NVT_INTERNAL_PHY

#ifdef USE_DEVICE_TREE
		sel_RMII_reg_val = readl(gmacdev->RmiiModeCtrlReg);
		sel_RMII_reg_val  |= 0x80000000;
		writel(sel_RMII_reg_val, gmacdev->RmiiModeCtrlReg);
#else /*not USE_DEVICE_TREE*/
		ioremap_OR_value(MAC_RMII_MII_SEL_ADDR, 0x80000000);

		/* External ethernet phy reset */
		ioremap_OR_value(NVT_PHY_RESET_GPIO_ADDR + 0x8,
				 (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));
		ioremap_write(NVT_PHY_RESET_GPIO_ADDR,
			      (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));
		mdelay(1);
		ioremap_write(NVT_PHY_RESET_GPIO_ADDR + 0x4,
			      (u32)(1UL << NVT_PHY_RESET_GPIO_BIT));

#endif /*USE_DEVICE_TREE*/
#endif /*CONFIG_NVT_INTERNAL_PHY*/
		gmacdev->LinkState = 0;
		do_suspend = 1;

#ifdef CONFIG_NVT_RTL_PHY_SUPPORT
		if (gmacdev->PHY_ID == RTL8201F_PHY_ID) {
			synopGMAC_RTL8201F_set_phy_TXC_direction(gmacdev, 1);
			synopgmac_rtl8201f_set_phy_rmii_rx_timing(
				gmacdev, 0xD);
		}
#endif
		synopGMAC_clear_interrupt(gmacdev);
		/*Disable the interrupts generated by MMC and IPC counters.
		* If these are not disabled ISR should be modified
		* accordingly to handle these interrupts.
		*/
		synopGMAC_disable_mmc_tx_interrupt(gmacdev, 0xFFFFFFFF);
		synopGMAC_disable_mmc_rx_interrupt(gmacdev, 0xFFFFFFFF);
		synopGMAC_disable_mmc_ipc_rx_interrupt(gmacdev, 0xFFFFFFFF);

		/* Power up MAC */
		synopGMAC_power_down_disable(gmacdev);

		netif_device_attach(netdev);

#ifdef USE_DEVICE_TREE
		enable_irq(gmacdev->IRQNum);
#else
		enable_irq(GMAC_IRQ);
#endif

		init_timer(&synopGMAC_cable_unplug_timer);

		synopGMAC_cable_unplug_timer.function =
			(void *)synopGMAC_linux_cable_unplug_function;

		synopGMAC_cable_unplug_timer.data = (unsigned long)adapter;
		synopGMAC_cable_unplug_timer.expires = jiffies + (HZ / 2);
		add_timer(&synopGMAC_cable_unplug_timer);
	}

	pr_crit("Ethernet resume complete....\n");
	return 0;
}

static const struct of_device_id of_platform_synopGMAC_table[] = {
	{ .compatible = "nvt,synopsys-mac" },
	{},
};

MODULE_DEVICE_TABLE(of, of_platform_synopGMAC_table);

static const struct dev_pm_ops synopGMAC_platform_pm_ops = {
	.suspend        = synopGMAC_suspend,
	.resume         = synopGMAC_resume,
};

static struct platform_driver synopGMAC_plat_driver = {
	.driver = {
		.name = "NT72658-synopGMAC",
		.owner = THIS_MODULE,
		.pm = &synopGMAC_platform_pm_ops,
		.bus = &platform_bus_type,
		.of_match_table = of_match_ptr(of_platform_synopGMAC_table),
	},
	.probe   = synopGMAC_probe,
};

/*add netdev_ops*/
static const struct net_device_ops synopGMAC_Netdev_Ops = {
	.ndo_open				= synopGMAC_linux_open,
	.ndo_stop				= synopGMAC_linux_close,
	.ndo_start_xmit			= synopGMAC_linux_xmit_frames,
	.ndo_get_stats			= synopGMAC_linux_get_stats,
	.ndo_set_mac_address	= synopGMAC_linux_set_mac_address,
	.ndo_change_mtu			= synopGMAC_linux_change_mtu,
	.ndo_do_ioctl			= synopGMAC_linux_do_ioctl,
	.ndo_tx_timeout			= synopGMAC_linux_tx_timeout,
	.ndo_change_rx_flags	= synopGMAC_linux_change_rx_flags,
};

/**
 * Function to initialize the Linux network interface.
 *
 * Linux dependent Network interface is setup here. This provides
 * an example to handle the network dependent functionality.
 *
 * \return Returns 0 on success and Error code on failure.
 */
s32 __init synopGMAC_init_network_interface(void)
{
	s32 err;
	struct net_device *netdev;

	TR("Call register_netdev to register the network interface\n");
	/*Lets allocate and set up an ethernet device,
	* it takes the sizeof the private structure.
	* This is mandatory as a 32 byte
	* allignment is required for the private data structure.
	*/
	netdev = alloc_etherdev(sizeof(struct synop_adapter_struct));
	if (!netdev) {
		err = -ESYNOPGMACNOMEM;
		goto err_alloc_etherdev;
	}

	synopGMACadapter = netdev_priv(netdev);
	synopGMACadapter->synopGMACnetdev = netdev;
	synopGMACadapter->synopGMACdev    = NULL;

	/*Allocate Memory for the the GMACip structure*/
	synopGMACadapter->synopGMACdev =
		(struct synop_device *)plat_alloc_memory(
		sizeof(struct synop_device));

	if (!synopGMACadapter->synopGMACdev)
		TR0("Error in Memory Allocataion #1\n");

	TR("## memory allocation at 0x%x\n",
	   (u32)synopGMACadapter->synopGMACdev);

	/*Attach the device to MAC struct
	* This will configure all the required base addresses
	* such as Mac base, configuration base,
	* phy base address(out of 32 possible phys)
	*/
#ifdef USE_DEVICE_TREE
	synopGMAC_attach(synopGMACadapter->synopGMACdev,
			 0, 0, DEFAULT_PHY_BASE);
#else
	synopGMAC_attach(synopGMACadapter->synopGMACdev,
			 SYNOP_GMAC_BASE_ADDR,
		SYNOP_DMA_BASE_ADDR,
		DEFAULT_PHY_BASE);
#endif

	/* Init lock for read/write PHY*/
	sema_init(&sem_phy_rw, 1);

#ifdef CONFIG_NVT_INTERNAL_PHY
	if (nvt_internal_PHY_ioremap_all())
		pr_crit("[NTKETHMAC] ioremap PHY address fail\n");

#if DEBUG_READ_PHY_REG
	internal_PHY_OFFSET_0120 = ioremap(0xFD130120, 0x4);
	if (!internal_PHY_OFFSET_0120)
		return -ESYNOPGMACNOMEM;

	internal_PHY_OFFSET_012C = ioremap(0xFD13012C, 0x4);
	if (!internal_PHY_OFFSET_012C)
		return -ESYNOPGMACNOMEM;

	internal_PHY_OFFSET_0130 = ioremap(0xFD130130, 0x4);
	if (!internal_PHY_OFFSET_0130)
		return -ESYNOPGMACNOMEM;
#endif

#else
	external_PHY_reset_dir = ioremap(NVT_PHY_RESET_GPIO_ADDR + 0x8, 0x4);
	if (!external_PHY_reset_dir)
		return -ESYNOPGMACNOMEM;

	external_PHY_reset_pull_low = ioremap(NVT_PHY_RESET_GPIO_ADDR, 0x4);
	if (!external_PHY_reset_pull_low)
		return -ESYNOPGMACNOMEM;

	external_PHY_reset_pull_high =
		ioremap(NVT_PHY_RESET_GPIO_ADDR + 0x4, 0x4);

	if (!external_PHY_reset_pull_high)
		return -ESYNOPGMACNOMEM;
#endif

	plugDetect_Wq = create_singlethread_workqueue("Eth_plugDetect");

	/*initialize the ethernet phy*/
	SysPlat_Init_Ethernet_Phy(synopGMACadapter->synopGMACdev, 1);

	/*to initialize the mac address*/
	SysPlat_Init_Mac_Addr();

	synopGMAC_reset(synopGMACadapter->synopGMACdev);

	/*refer e100.c net driver*/
	netdev->netdev_ops = &synopGMAC_Netdev_Ops;
	netdev->watchdog_timeo = 5 * HZ;

	/*init tx spin_lock*/
	spin_lock_init(&synopGMACadapter->synopGMACdev->tx_lock);

	/*set MDC clock by ahb auto-detection function*/
	g_MDC_CLOCK = GmiiCsrClk1;

	err = platform_driver_register(&synopGMAC_plat_driver);
	if (err) {
		pr_crit("[NTKETHMAC] Register platform driver fail\n");
		return err;
	}

	/*Now start the network interface*/
	pr_crit("[NTKETHMAC] : Now Registering the netdevice\n");
	TR("Now Registering the netdevice\n");

	err = register_netdev(netdev);

	if (err) {
		TR0("Error in Registering netdevice\n");
		return err;
	}

#ifdef CONFIG_NVT_FASTETH_MAC_NAPI
	pr_crit("[NTKETHMAC] : Init with NAPI support\n");
	netif_napi_add(netdev, &nvt_napi,
		       synopGMAC_linux_poll, NVT_NAPI_WEIGHT);

#endif

	/* Set up MAC address */
	synopGMAC_set_mac_addr(synopGMACadapter->synopGMACdev,
			       GmacAddr0High, GmacAddr0Low, Mac_Addr);
	synopGMAC_set_mac_addr(synopGMACadapter->synopGMACdev,
			       GmacAddr1High, GmacAddr1Low, Mac_Addr);
	synopGMAC_get_mac_addr(synopGMACadapter->synopGMACdev,
			       GmacAddr0High, GmacAddr0Low, netdev->dev_addr);

	return 0;
err_alloc_etherdev:
	TR0("Problem in alloc_etherdev()..Take Necessary action\n");
	return err;
}

/**
 * Function to initialize the Linux network interface.
 * Linux dependent Network interface is setup here. This provides
 * an example to handle the network dependent functionality.
 * \return Returns 0 on success and Error code on failure.
 */
void __exit synopGMAC_exit_network_interface(void)
{
	TR0("Now Calling network_unregister\n");
	unregister_netdev(synopGMACadapter->synopGMACnetdev);

	platform_driver_unregister(&synopGMAC_plat_driver);

#ifdef CONFIG_NVT_INTERNAL_PHY
	nvt_internal_PHY_iounmap_all();
#else
	iounmap(external_PHY_reset_dir);
	iounmap(external_PHY_reset_pull_low);
	iounmap(external_PHY_reset_pull_high);
#endif

	flush_workqueue(plugDetect_Wq);
	destroy_workqueue(plugDetect_Wq);
}

/*Finally we need to initialize the value
* of mac address from flash or eeprom.
*/
void SysPlat_Init_Mac_Addr(void)
{
#define DEFAULT_MAC_ADDRESS {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}

	u8 default_mac_addr[6] = DEFAULT_MAC_ADDRESS;

	memcpy(Mac_Addr, default_mac_addr, sizeof(Mac_Addr));
}

