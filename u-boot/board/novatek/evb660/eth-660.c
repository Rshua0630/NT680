/** \file
 * This is the network dependent layer to handle network related functionality.
 * This file is tightly coupled to neworking frame work of linux 2.6.xx kernel.
 * The functionality carried out in this file should be treated as an example only
 * if the underlying operating system is not Linux. 
 * 
 * \note Many of the functions other than the device specific functions
 *  changes for operating system other than Linux 2.6.xx
 * \internal 
 *-----------------------------REVISION HISTORY-----------------------------------
 * Synopsys			01/Aug/2007				Created
 */


#include <common.h>
#include <command.h>
#include <net.h>
#include <miiphy.h>
//#include <malloc.h>

//#include "nvt_eth.h"
#include "eth_dev-660.h"
//#include "nvt_plat.h"
static u8 Mac_Addr[6];		// The current mac address.

s32 eth_setup_tx_desc_queue(mac_device *gmacdev, u32 no_of_desc, u32 desc_mode)
{
	s32 i;

	DmaDesc *first_desc = NULL;
	DmaDesc *second_desc = NULL;
	//dma_addr_t dma_addr;
	gmacdev->TxDescCount = 0;

	if(desc_mode == RINGMODE)
	{
		TR("Total size of memory required for Tx Descriptors in Ring Mode = 0x%08x\n",((sizeof(DmaDesc) * no_of_desc)));
		//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc) * no_of_desc,&dma_addr);	// jay hsu
		
		// jay hsu : alloc memory in kernel
		first_desc = malloc(sizeof(DmaDesc)*no_of_desc);
		if(first_desc == NULL)
		{
			TR("Error in Tx Descriptors memory allocation\n");
			return -ESYNOPGMACNOMEM;
		}
		// jay hsu : clear tx descriptors
		memset((char *)first_desc, 0, sizeof(DmaDesc)*no_of_desc);
		
		gmacdev->TxDescCount = no_of_desc;
		gmacdev->TxDescBase      = first_desc;		// [Rich: assign the descriptor to be DMA]
		gmacdev->TxDescDma   = virt_to_phys(first_desc);	// Howard Chang define the start of all decriptors
		//gmacdev->TxDescDma   = (u32)first_desc;	// [Rich: define the start of all descriptors]
		//gmacdev->TxDescDma   = dma_addr;
		
		for(i=0; i < gmacdev->TxDescCount; i++)
		{
			eth_tx_desc_init_ring(gmacdev->TxDescBase + i, i == gmacdev->TxDescCount-1);
			//TR0("%02d %08x \n",i, (unsigned int)(gmacdev->TxDescBase + i) );
		}
		
		// jay hsu : sync tx init descriptors to dram
		//dma_cache_sync(first_desc,(sizeof(DmaDesc)*no_of_desc), DMA_TO_DEVICE);
		wb_inv_dcache_range((u32)first_desc, (sizeof(DmaDesc)*no_of_desc));

	}
	else
	{
		//Allocate the head descriptor
		//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
		
		// jay hsu : alloc memory in kernel
		first_desc = malloc(sizeof(DmaDesc));
		if(first_desc == NULL)
		{
			TR("Error in Tx Descriptor Memory allocation in Ring mode\n");
			return -ESYNOPGMACNOMEM;
		}
		gmacdev->TxDescBase       = first_desc;
		gmacdev->TxDescDma        = virt_to_phys(first_desc);	// Howard Chang define the start of all decriptors
		//gmacdev->TxDescDma    = (u32)first_desc;	// jay hsu
		//gmacdev->TxDescDma    = dma_addr;

		TR("Tx===================================================================Tx\n");
		first_desc->buffer2   = gmacdev->TxDescDma;
	 	first_desc->data2     = (u32)gmacdev->TxDescBase;

		gmacdev->TxDescCount = 1;
		
		for(i =0; i <(no_of_desc-1); i++)
		{
			//second_desc = plat_alloc_consistent_dmaable_memory(pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
			
			// jay hsu : alloc memory in kernel
			second_desc = malloc(sizeof(DmaDesc));
			if(second_desc == NULL)
			{	
				TR("Error in Tx Descriptor Memory allocation in Chain mode\n");
				return -ESYNOPGMACNOMEM;
			}
	//		first_desc->buffer2  = dma_addr;
			first_desc->data2    = (u32)second_desc;
			
			second_desc->buffer2 = gmacdev->TxDescDma;
			second_desc->data2   = (u32)gmacdev->TxDescBase;

		    eth_tx_desc_init_chain(first_desc);
			TR("%02d %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->TxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
			gmacdev->TxDescCount += 1;
			first_desc = second_desc;
		}
			
			eth_tx_desc_init_chain(first_desc);
			TR("%02d %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->TxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
			TR("Tx===================================================================Tx\n");
	}

	gmacdev->TxNextIndx = 0;
	gmacdev->TxBusyIndx = 0;
	gmacdev->TxNextDesc = gmacdev->TxDescBase;
	gmacdev->TxBusyDesc = gmacdev->TxDescBase;
	gmacdev->BusyTxDescNum  = 0; 

	return -ESYNOPGMACNOERR;
}



s32 eth_setup_rx_desc_queue(mac_device * gmacdev, u32 no_of_desc, u32 desc_mode)
{
	s32 i;

	DmaDesc *first_desc = NULL;
	DmaDesc *second_desc = NULL;
	//dma_addr_t dma_addr;
	gmacdev->RxDescCount = 0;

	if(desc_mode == RINGMODE)
	{
		TR("total size of memory required for Rx Descriptors in Ring Mode = 0x%08x\n",((sizeof(DmaDesc) * no_of_desc)));
		//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc) * no_of_desc, &dma_addr);	// jay hsu
		
		// jay hsu : alloc memory in kernel
		first_desc = malloc(sizeof(DmaDesc)*no_of_desc);
		// jay hsu : clear rx descriptors
		memset((char *)first_desc, 0, sizeof(DmaDesc) * no_of_desc);
		if(first_desc == NULL)
		{
			TR("Error in Rx Descriptor Memory allocation in Ring mode\n");
			return -ESYNOPGMACNOMEM;
		}
		gmacdev->RxDescCount = no_of_desc;
		gmacdev->RxDescBase  = first_desc;
		gmacdev->RxDescDma   = virt_to_phys(first_desc);	// Howard Chang
		//gmacdev->RxDescDma   = (u32)first_desc;	// jay hsu
		
		for(i =0; i < gmacdev->RxDescCount; i++)
		{
			eth_rx_desc_init_ring(gmacdev->RxDescBase + i, i == gmacdev->RxDescCount-1);
			//TR("%02d %08x \n",i, (unsigned int)(gmacdev->RxDescBase + i));
		}
		
		// jay hsu : sync rx init descriptors to dram
		//dma_cache_sync(first_desc,(sizeof(DmaDesc)*no_of_desc), DMA_TO_DEVICE);
		wb_inv_dcache_range((u32)first_desc, (sizeof(DmaDesc)*no_of_desc));
	}
	else
	{
		//Allocate the head descriptor
		//first_desc = plat_alloc_consistent_dmaable_memory (pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
		
		// jay hsu : alloc memory in kernel
		first_desc = malloc(sizeof(DmaDesc));	
		if(first_desc == NULL)
		{
			TR("Error in Rx Descriptor Memory allocation in Ring mode\n");
			return -ESYNOPGMACNOMEM;
		}
		gmacdev->RxDescBase       = first_desc;
		gmacdev->RxDescDma        = virt_to_phys(first_desc);	// Howard Chang
		//gmacdev->RxDescDma    = (u32)first_desc;	// jay hsu

		TR("Rx===================================================================Rx\n");
		first_desc->buffer2   = gmacdev->RxDescDma;
		first_desc->data2     = (u32) gmacdev->RxDescBase;

		gmacdev->RxDescCount = 1;
		for(i =0; i < (no_of_desc-1); i++)
		{
			//second_desc = plat_alloc_consistent_dmaable_memory(pcidev, sizeof(DmaDesc),&dma_addr);	// jay hsu
			
			// jay hsu : alloc memory in kernel
			second_desc = malloc(sizeof(DmaDesc));
			if(second_desc == NULL)
			{	
				TR("Error in Rx Descriptor Memory allocation in Chain mode\n");
				return -ESYNOPGMACNOMEM;
			}
	//		first_desc->buffer2  = dma_addr;
			first_desc->data2    = (u32)second_desc;
			
			second_desc->buffer2 = gmacdev->RxDescDma;
			second_desc->data2   = (u32)gmacdev->RxDescBase;

			eth_rx_desc_init_chain(first_desc);
			TR("%02d  %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->RxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
			gmacdev->RxDescCount += 1;
			first_desc = second_desc;
		}
	                eth_rx_desc_init_chain(first_desc);
			TR("%02d  %08x %08x %08x %08x %08x %08x %08x \n",gmacdev->RxDescCount, (u32)first_desc, first_desc->status, first_desc->length, first_desc->buffer1,first_desc->buffer2, first_desc->data1, first_desc->data2);
		TR("Rx===================================================================Rx\n");

	}

	gmacdev->RxNextIndx = 0;
	gmacdev->RxBusyIndx = 0;
	gmacdev->RxNextDesc = gmacdev->RxDescBase;
	gmacdev->RxBusyDesc = gmacdev->RxDescBase;
	gmacdev->BusyRxDescNum   = 0; 

	return -ESYNOPGMACNOERR;
}


void eth_giveup_rx_desc_queue(mac_device *gmacdev, u32 desc_mode)
{
	s32 i;

	if(desc_mode == RINGMODE)
	{
		for(i=0; i<gmacdev->RxDescCount; i++)
		{
			DmaDesc *pstDesc ;
			
			// free the buffer1 (data1), alllocated in eth_init
			pstDesc = gmacdev->RxDescBase + i ;
			// the allocated original buffer1 pointer was stored in data1
			if(pstDesc->data1)
				free(pstDesc->data1) ;
		}
		free((u32 *)gmacdev->RxDescBase);
		TR("Memory allocated %08x  for Rx Desriptors (ring) is given back\n",(u32)gmacdev->RxDescBase);
	}
	else
	{
		TR0("eth_giveup_rx_desc_queue: chain mode removed in uboot porting\n" );
	}
	gmacdev->RxDescBase    = NULL;
	gmacdev->RxDescDma = 0;
	return;
}


void eth_giveup_tx_desc_queue(mac_device *gmacdev, u32 desc_mode)
{
	if(desc_mode == RINGMODE)
	{
		// in uboot, we did not allocate buffer1 for tx desc
		// just free TxDescBase
		free((u32 *)gmacdev->TxDescBase);
		TR("Memory allocated %08x for Tx Desriptors (ring) is given back\n",(u32)gmacdev->TxDescBase);
	}
	else
	{
		TR0("eth_giveup_tx_desc_queue: chain mode removed in uboot porting\n" );
	}
	
	gmacdev->TxDescBase    = NULL;
	gmacdev->TxDescDma = 0;
	return;
}


s32 nt96660_eth_recv(struct eth_device *eth_dev)

{
	mac_device * gmacdev;
	s32 desc_index;
	
	u32 data1;
	u32 data2;
	u32 len;
	u32 status;
	u32 dma_addr1;
	u32 dma_addr2;

	u32 timeStart, timeDelta;

	gmacdev = eth_dev->priv;

	/*Handle the Receive Descriptors*/
	// process until no data in rx desc queue
	timeDelta = 1000; /* tick, 1 tick = 1ms */
	timeStart = get_timer(0);
	do
	{
		desc_index = eth_get_rx_qptr(gmacdev,&status,&dma_addr1,NULL, &data1,&dma_addr2,NULL,&data2);
		// jay hsu : debug
		TR("##Handle Received## desc_index %d, dma_addr1 0x%x(%p), data1 0x%x\n", desc_index, (u32)dma_addr1, &dma_addr1, data1);
		if(desc_index >= 0 && data1 != 0)
		{
			TR("Received Data at Rx Descriptor %d for skb 0x%08x whose status is %08x\n",desc_index,data1,status);
			/*At first step unmapped the dma address*/
			if(eth_is_rx_desc_valid(status))
			{
				len =  eth_get_rx_desc_frame_length(status) - 4; //Not interested in Ethernet CRC bytes
				// pass data to uboot proctocal layer
				NetReceive((u8 *)dma_addr1, len);
				// change the own of rx desc indexed by desc_index to DMA
				eth_set_owner_dma(gmacdev->RxDescBase + desc_index) ;
				wb_inv_dcache_range((u32)(gmacdev->RxDescBase + desc_index), (sizeof(DmaDesc)));
			}
			else
			{
				// show error message
				// do the error process
				TR0("something wrong in rx_desc\n");
			}
		}
		#if 0
		else
		{
			// show error message
			// do the error process
			// if init is OK, then code will not enter into this case!
			TR0("please check init setting\n") ;
			break ;
		}
		#endif
		if((get_timer(0) - timeStart) > timeDelta)
			break;
	}while(desc_index >= 0);

	return 0 ;
}

static int nt96660_eth_init_flag = false;

int nt96660_eth_init(struct eth_device *eth_dev, bd_t *bis)
{
	u8 mac_addr0[6] = DEFAULT_MAC_ADDRESS;
	s32 status = 0;
	s32 count;
	mac_device *gmacdev;
	u32 PHY_ID = 0;
	
	if (nt96660_eth_init_flag == true)
		return 0;

	gmacdev = (mac_device *)eth_dev->priv; 	// mac_dev already located in nvt682_eth_initialize
	
	/*Lets read the version of ip in to device structure*/	
	eth_read_version(gmacdev);
	
	// jay hsu : set mac address again [// Rich : why setting again ? the fisrt time is done by attach in nvt682_eth_initialize ]
	eth_set_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, mac_addr0);

	// set mac address to eth_dev
	//eth_get_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, eth_dev->enetaddr); // Rich : we can do this job by memcpy]
	memcpy(eth_dev->enetaddr,mac_addr0,6) ;
		
	/*Check for Phy initialization*/
	eth_set_mdc_clk_div(gmacdev,GmiiCsrClk1);
	gmacdev->ClockDivMdc = eth_get_mdc_clk_div(gmacdev);


	// Jay Hsu : reset phy
	if ( eth_reset_phy(gmacdev) == -ESYNOPGMACPHYERR )
	{
		TR("## reset RTL phy failed##\n");
		return -ESYNOPGMACPHYERR;
	}
	else
	{
		TR("## reset RTL phy OK ##\n");
	}
	
	// Howard Chang : check RTL phy id until phy reset procedure done
	count = 100;
	while (count--)
	{
		if ( (eth_get_phy_id(gmacdev) == RTL8201E_PHY_ID) ||  \
			(eth_get_phy_id(gmacdev) == RTL8201F_PHY_ID) )
		{
			PHY_ID = eth_get_phy_id(gmacdev);
			printf("The phy id is 0x%x\n", PHY_ID);
			TR("##Ethernet Phy ready##\n");
			break;
		}
		udelay(10);
	}

	if ( count < 0 )
	{
	 	TR0("##Not RTL phy##, can't open ethernet interface\n");
	 	return -ESYNOPGMACPHYERR;
	}
		
	// jay hsu : check phy init speed for our Micrel phy
	status = eth_check_phy_init_for_10_100M(gmacdev);
	
	// Howard Chang: specified setting for different RTL phy
	if (PHY_ID == RTL8201E_PHY_ID) {
		synopGMAC_get_phy_test_reg(gmacdev);
		synopGMAC_get_phy_LBREM_reg(gmacdev);
	} else if (PHY_ID == RTL8201F_PHY_ID) {
		synopGMAC_RTL8201F_set_phy_TXC_direction(gmacdev, 1);
		synopGMAC_RTL8201F_set_phy_adjust_RMII_RX_timing(gmacdev, 0xD);
	}	
	
	/*Set up the tx and rx descriptor queue/ring*/
	// jay hsu : no pci on 96660
	eth_setup_tx_desc_queue(gmacdev, TRANSMIT_DESC_SIZE, RINGMODE);	// [Rich:fn proto revised, pci dev removed]
	eth_init_tx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr

	// jay hsu : no pci on 96660
	eth_setup_rx_desc_queue(gmacdev, RECEIVE_DESC_SIZE, RINGMODE);	// [Rich:fn proto revised, pci dev removed]
	eth_init_rx_desc_base(gmacdev);	//Program the transmit descriptor base address in to DmaTxBase addr

	// jay hsu : use burst length 8
	//eth_dma_bus_mode_init(gmacdev, DmaBurstLength8 | DmaDescriptorSkip1);
	// eth_dma_bus_mode_init(gmacdev, DmaBurstLength8 | DmaDescriptorSkip2);	// jay hsu : skip 2x8 bytes to get next descriptors
	eth_dma_bus_mode_init(gmacdev, DmaBurstLength8 | DmaDescriptorSkip12);
	
	// jay hsu : don't use Operate on second frame
	eth_dma_control_init(gmacdev,DmaStoreAndForward |DmaRxThreshCtrl128);
	//eth_dma_control_init(gmacdev,DmaStoreAndForward |DmaTxSecondFrame|DmaRxThreshCtrl128);	

	/*Initialize the mac interface*/
	eth_mac_init(gmacdev);
	eth_pause_control(gmacdev); // This enables the pause control in Full duplex mode of operation
	
	#ifdef IPC_OFFLOAD
	/*IPC Checksum offloading is enabled for this driver. Should only be used if Full Ip checksumm offload engine is configured in the hardware*/
	eth_enable_rx_chksum_offload(gmacdev);		//Enable the offload engine in the receive path
	eth_rx_tcpip_chksum_drop_enable(gmacdev);	// This is default configuration, DMA drops the packets if error in encapsulated ethernet payload
												// The FEF bit in DMA control register is configured to 0 indicating DMA to drop the errored frames.
	#endif
	// [Rich: Prepare some rx buffers in advance ]
	
	 do
	 {
	 	u8	*pu8RxBuf ;

		pu8RxBuf = (u8 *)malloc(2000);	
		if(pu8RxBuf == NULL)
		{
			TR0("ERROR in buffer allocation in eth_init\n");
			break;
			//return -ESYNOPGMACNOMEM;
		}
		// let buffer1 aligned with 8
		// data1 to store the original buffer1 address that could be used during memory free
		// rx_desc.buffer1 = (aligned memory pointer)
		// rx_desc.length1 = 1992
		// rx_desc.data1 = original memory pointer
		// purpose: during receiving, no more memory allocation is needed
		// after rx_desc_queue setup /init, 
		//		each rx desc is empty in queue
		//		RxNextIndx = 0
		//		RxBusyIndx = 0
		// this loop will fill each rx_desc and move RxNextIndx/RxNextDesc back to the beginning
		status = eth_set_rx_qptr(gmacdev,((u32)pu8RxBuf)&(~0x7), 1992, pu8RxBuf,0,0,0);
		if(status < 0)
		{
			// no empty rx_desc, already back to the beginning of rx_desc ring
			free(pu8RxBuf);
		}
			
	}while(status >= 0);

	eth_clear_interrupt(gmacdev);
	/*
	Disable the interrupts generated by MMC and IPC counters.
	If these are not disabled ISR should be modified accordingly to handle these interrupts.
	*/	
	eth_disable_mmc_tx_interrupt(gmacdev, 0xFFFFFFFF);
	eth_disable_mmc_rx_interrupt(gmacdev, 0xFFFFFFFF);
	eth_disable_mmc_ipc_rx_interrupt(gmacdev, 0xFFFFFFFF);

	eth_enable_interrupt(gmacdev,DmaIntDisable);	// Rich : we do not use interrupt in uboot

	eth_enable_dma_rx(gmacdev);	// start rx engine
	eth_enable_dma_tx(gmacdev);	// start tx engine

	nt96660_eth_init_flag = true;
	
	return (0);	// non-negative return value means OK

}

void nt96660_eth_halt(struct eth_device *eth_dev)
{
#if 0
	mac_device *gmacdev;
	
	gmacdev = (mac_device *)eth_dev->priv;
	if(gmacdev == NULL)
	{
		TR0("OOPS gmacdev is null\n");
		return -ESYNOPGMACNOMEM;
	}

	/*Disable all the interrupts*/
	eth_disable_interrupt_all(gmacdev);
	TR("the eth interrupt has been disabled\n");

	/*Disable the reception*/	
	eth_disable_dma_rx(gmacdev);
	eth_take_desc_ownership_rx(gmacdev);
	TR("the eth Reception has been disabled\n");

	/*Disable the transmission*/
	eth_disable_dma_tx(gmacdev);
	eth_take_desc_ownership_tx(gmacdev);
	TR("the eth Transmission has been disabled\n");
		
	/*Free the Rx Descriptor contents*/
	TR("Now calling eth_giveup_rx_desc_queue \n");
	eth_giveup_rx_desc_queue(gmacdev, RINGMODE);
	
	TR("Now calling eth_giveup_tx_desc_queue \n");
	eth_giveup_tx_desc_queue(gmacdev, RINGMODE);
	
	return -ESYNOPGMACNOERR;
//	TR("%s called \n",__FUNCTION__);
#endif
}

s32 nt96660_eth_send(struct eth_device *eth_dev, volatile void *packet, int length)
{	
	s32 status = 0;
	u32 offload_needed = 0;
	mac_device *gmacdev;
	u32 dma_status_reg = 0 ;
	u32 send_packet_wait_loop = 0;

	gmacdev = (mac_device *)eth_dev->priv;
	if(gmacdev == NULL)
		return -1;

	status = eth_set_tx_qptr(gmacdev,(u32)packet,length,(u32)packet,0,0,0,offload_needed);
	if(status < 0)
	{
		TR("%s No More Free Tx Descriptors\n",__FUNCTION__);
		return -ESYNOPGMACNOMEM;
	}
	
	/*Now force the DMA to start transmission*/	
	eth_resume_dma_tx(gmacdev);		

#if 0
	//[Rich: wait for one packet completed, in polling mode]
	// check phy link status to see if wire is still linked or not
	// if not linked, return error
	status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_STATUS_REG, &data);
	if(!gmacdev->LinkState)
	{
		TR0("No Link: %08x\n",data);
		return -ESYNOPGMACPHYERR ;
	}
#endif

	while(1)
	{
		dma_status_reg = ethReadReg((u32 *)gmacdev->DmaBase, DmaStatus) ;
		if(dma_status_reg&DmaIntTxCompleted) {
			TR0("Txcomplete\n");
			break ;
		}
		// use timeout to prevent system trapped
		send_packet_wait_loop++ ;
		if(500000<send_packet_wait_loop)
		{
			TR0("gmac tx timeout\n");
			return -ESYNOPGMACBUSY ;
		}
	}
	
	//(gmacdev->TxDescBase + status)->length &= ~(DescSize1Mask | DescSize2Mask);

	return -ESYNOPGMACNOERR;
}

int nt96660_eth_write_hwaddr(struct eth_device *eth_dev)
{
	mac_device *gmacdev;
	gmacdev = (mac_device *)eth_dev->priv;
	if(gmacdev == NULL)
		return -1;

	memcpy(eth_dev->enetaddr, Mac_Addr, sizeof(eth_dev->enetaddr));
	return 0;
}

void SysPlat_enable_GMAC_CLK(void)
{
	u32 reg;
	
	reg = INW(0xC0020000 + 0x74);
	reg |= 1 << (61 & 0x1F);
	OUTW(0xC0020000 + 0x74, reg);
}

void SysPlat_asynchronous_reset(void)
{
	u32 reg;
	u32 uiRegOffset;
	/* The reset bit required to be set 0 -> 1*/
	uiRegOffset = (61 >> 5) << 2;
	reg = INW(0xC0020080 + uiRegOffset);
	reg &= ~(1 << (61 & 0x1F));
	reg = OUTW(0xC0020080 + uiRegOffset, reg);

	reg |= 1 << (61 & 0x1F);
	reg = OUTW(0xC0020080 + uiRegOffset, reg);
}

void SysPlat_Init_Ethernet_Phy(void)
{
	u32 clock_ctrl;

	/* RMII - MAC to Phy */
	clock_ctrl = INW(MAC_CLOCK_INVERSE_ADDR);
	clock_ctrl |= 1 << 14;
	OUTW(MAC_CLOCK_INVERSE_ADDR, clock_ctrl);
}

// Finally we need to initialize the value of mac address from flash or eeprom.
void SysPlat_Init_Mac_Addr(void)
{
	u8 default_mac_addr[6] = DEFAULT_MAC_ADDRESS;

	memcpy(Mac_Addr, default_mac_addr, sizeof(Mac_Addr));
	
	return;
}

int nt96660_eth_initialize(bd_t *bis)
{
	// >>> Following was copied from davinci
	struct eth_device	*eth_dev;
	mac_device		*gmacdev ;
	
	eth_dev = malloc(sizeof *eth_dev);

	if (eth_dev == NULL)
		return -1;

	memset(eth_dev, 0, sizeof *eth_dev);

	sprintf(eth_dev->name, "eth-660");
	eth_dev->init = nt96660_eth_init;
	eth_dev->halt = nt96660_eth_halt;
	eth_dev->send = nt96660_eth_send;
	eth_dev->recv = nt96660_eth_recv;
	eth_dev->write_hwaddr = nt96660_eth_write_hwaddr;
	
	eth_register(eth_dev);
	// <<< end of copied

	gmacdev    = NULL;
	/*Allocate Memory for the the GMACip structure*/
	gmacdev = (mac_device *)malloc(sizeof (mac_device));
	if(!gmacdev)
	{
		TR0("Error in Memory Allocataion #1\n");
		return -ESYNOPGMACNOMEM;
	}
	TR("## memory allocation at 0x%x\n", (u32)gmacdev);
	TR("## GMac Base Addr at 0x%x\n", (u32)eth_dev);

	eth_dev->priv = gmacdev;
	
	// Howard Chang: toggle ethernet reset before setting clock
	SysPlat_asynchronous_reset();

	// Howard Chang: Enable GMAC clock
	SysPlat_enable_GMAC_CLK();

	// Howard Chang: Init ethernet phy
	SysPlat_Init_Ethernet_Phy();
	
	// Howard Chang: Init MAC address
	SysPlat_Init_Mac_Addr();

	
	/*Attach the device to MAC struct. This will configure all the required base addresses
	  such as Mac base, configuration base, phy base address(out of 32 possible phys )*/
	eth_attach(gmacdev,(u32) ETH_BASE_ADDR + MACBASE,(u32) ETH_BASE_ADDR + DMABASE, DEFAULT_PHY_BASE);
	
	eth_reset(gmacdev);
	
	// Howard Chang: set up MAC address
	eth_set_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, Mac_Addr);
	
	memcpy(eth_dev->enetaddr,Mac_Addr,6);
	
 	return 0;	// 0&positive value means OK, negative means failed

}
