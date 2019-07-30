/** \file
 * This file defines the synopsys GMAC device dependent functions.
 * Most of the operations on the GMAC device are available in this file.
 * Functions for initiliasing and accessing MAC/DMA/PHY registers and the DMA descriptors
 * are encapsulated in this file. The functions are platform/host/OS independent.
 * These functions in turn use the low level device dependent (HAL) functions to 
 * access the register space.
 * \internal
 * ------------------------REVISION HISTORY---------------------------------
 * Synopsys                 01/Aug/2007                              Created
 */

#include <common.h>
#include <spi.h>
//#include <malloc.h>
#include <asm/addrspace.h>

#include <asm/arch/nt96660_regs.h>
#include <asm/arch/nvt_plat.h>
#include <asm/arch/cache.h>
#include "eth_dev-660.h"

/**
 * Function to set the MDC clock for mdio transactiona
 *
 * @param[in] pointer to device structure.
 * @param[in] clk divider value.
 * \return Reuturns 0 on success else return the error value.
 */
s32 eth_set_mdc_clk_div(mac_device *gmacdev,u32 clk_div_val)
{
	u32 orig_data;
	orig_data = ethReadReg((u32 *)gmacdev->MacBase,GmacGmiiAddr); //set the mdc clock to the user defined value
	orig_data &= (~ GmiiCsrClkMask);	   
	orig_data |= clk_div_val;
	ethWriteReg((u32 *)gmacdev->MacBase, GmacGmiiAddr ,orig_data);
	return 0;
}

/**
 * Returns the current MDC divider value programmed in the ip.
 *
 * @param[in] pointer to device structure.
 * @param[in] clk divider value.
 * \return Returns the MDC divider value read.
 */
u32 eth_get_mdc_clk_div(mac_device *gmacdev)
{
	u32 data;
	data = ethReadReg((u32 *)gmacdev->MacBase,GmacGmiiAddr);
	data &= GmiiCsrClkMask;
	return data;
}

/**
 * Function to read the Phy register. The access to phy register
 * is a slow process as the data is moved accross MDI/MDO interface
 * @param[in] pointer to Register Base (It is the mac base in our case) .
 * @param[in] PhyBase register is the index of one of supported 32 PHY devices.
 * @param[in] Register offset is the index of one of the 32 phy register.
 * @param[out] u16 data read from the respective phy register (only valid iff return value is 0).
 * \return Returns 0 on success else return the error status.
 */
s32 eth_read_phy_reg(u32 *RegBase,u32 PhyBase, u32 RegOffset, u16 * data)
{
	u32 addr;
	u32 loop_variable;

	//addr = ((PhyBase << GmiiDevShift) & GmiiDevMask) | ((RegOffset << GmiiRegShift) & GmiiRegMask);
	// jay hsu : set mdc clock divider together
	addr = ((PhyBase << GmiiDevShift) & GmiiDevMask) | ((RegOffset << GmiiRegShift) & GmiiRegMask) | MDC_CLOCK_DIV;

	addr = addr | GmiiBusy ; //Gmii busy bit
	ethWriteReg(RegBase,GmacGmiiAddr,addr); //write the address from where the data to be read in GmiiGmiiAddr register of eth ip

	for(loop_variable = 0; loop_variable < DEFAULT_LOOP_VARIABLE; loop_variable++)
	{ //Wait till the busy bit gets cleared with in a certain amount of time
		plat_delay(DEFAULT_DELAY_VARIABLE);

		if (!(ethReadReg(RegBase,GmacGmiiAddr) & GmiiBusy))
		{
			break;
		}
	}
	if(loop_variable < DEFAULT_LOOP_VARIABLE)
		* data = (u16)(ethReadReg(RegBase,GmacGmiiData) & 0xFFFF);
	else
	{
		TR("Error::: PHY not responding Busy bit didnot get cleared !!!!!!\n");
		return -ESYNOPGMACPHYERR;
	}
	return -ESYNOPGMACNOERR;
}

/**
 * Function to write to the Phy register. The access to phy register
 * is a slow process as the data is moved accross MDI/MDO interface
 * @param[in] pointer to Register Base (It is the mac base in our case) .
 * @param[in] PhyBase register is the index of one of supported 32 PHY devices.
 * @param[in] Register offset is the index of one of the 32 phy register.
 * @param[in] data to be written to the respective phy register.
 * \return Returns 0 on success else return the error status.
 */
s32 eth_write_phy_reg(u32 *RegBase, u32 PhyBase, u32 RegOffset, u16 data)
{
	u32 addr;
	u32 loop_variable;

	ethWriteReg(RegBase,GmacGmiiData,data); // write the data in to GmacGmiiData register of eth ip

	//addr = ((PhyBase << GmiiDevShift) & GmiiDevMask) | ((RegOffset << GmiiRegShift) & GmiiRegMask) | GmiiWrite;
	// jay hsu : set mdc clock divider together
	addr = ((PhyBase << GmiiDevShift) & GmiiDevMask) | ((RegOffset << GmiiRegShift) & GmiiRegMask) | GmiiWrite | MDC_CLOCK_DIV;

	addr = addr | GmiiBusy ; //set Gmii clk to 20-35 Mhz and Gmii busy bit

	ethWriteReg(RegBase,GmacGmiiAddr,addr);
	for(loop_variable = 0; loop_variable < DEFAULT_LOOP_VARIABLE; loop_variable++)
	{
		if (!(ethReadReg(RegBase,GmacGmiiAddr) & GmiiBusy))
		{
			break;
		}
		plat_delay(DEFAULT_DELAY_VARIABLE);
	}

	if(loop_variable < DEFAULT_LOOP_VARIABLE)
	{
		return -ESYNOPGMACNOERR;
	}
	else
	{
		TR("Error::: PHY not responding Busy bit didnot get cleared !!!!!!\n");
		return -ESYNOPGMACPHYERR;
	}
}

/**
 * Function to configure the phy in loopback mode. 
 *
 * @param[in] pointer to mac_device.
 * @param[in] enable or disable the loopback.
 * \return 0 on success else return the error status.
 * \note Don't get confused with mac loop-back eth_loopback_on(mac_device *) 
 * and eth_loopback_off(mac_device *) functions.
 */
s32 eth_phy_loopback(mac_device *gmacdev, bool loopback)
{
	s32 status = -ESYNOPGMACNOERR;
	if(loopback)
		status = eth_write_phy_reg((u32 *)gmacdev->MacBase, gmacdev->PhyBase, PHY_CONTROL_REG, Mii_Loopback);
	else
		status = eth_write_phy_reg((u32 *)gmacdev->MacBase, gmacdev->PhyBase, PHY_CONTROL_REG, Mii_NoLoopback);

	return status;
}

/**
 * Function to read the GMAC IP Version and populates the same in device data structure.
 * @param[in] pointer to mac_device.
 * \return Always return 0.
 */

s32 eth_read_version (mac_device * gmacdev)
{	
	u32 data = 0;
	data = ethReadReg((u32 *)gmacdev->MacBase, GmacVersion );
	gmacdev->Version = data;
	TR("The data read from %08x is %08x\n",(gmacdev->MacBase+GmacVersion),data);
	return 0;
}


/**
 * Function to reset the GMAC core. 
 * This reests the DMA and GMAC core. After reset all the registers holds their respective reset value
 * @param[in] pointer to mac_device.
 * \return 0 on success else return the error status.
 */
s32 eth_reset (mac_device *gmacdev )
{	
	u32 data = 0;

	ethWriteReg((u32 *)gmacdev->DmaBase, DmaBusMode ,DmaResetOn);
	plat_delay(DEFAULT_LOOP_VARIABLE);
	data = ethReadReg((u32 *)gmacdev->DmaBase, DmaBusMode);
	TR("DATA after Reset = %08x\n",data);

	return 0;	
}


/**
 * Function to program DMA bus mode register. 
 * 
 * The Bus Mode register is programmed with the value given. The bits to be set are
 * bit wise or'ed and sent as the second argument to this function.
 * @param[in] pointer to mac_device.
 * @param[in] the data to be programmed.
 * \return 0 on success else return the error status.
 */
s32 eth_dma_bus_mode_init(mac_device * gmacdev, u32 init_value )
{
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaBusMode ,init_value);
	return 0;

}

/**
 * Function to program DMA Control register. 
 * 
 * The Dma Control register is programmed with the value given. The bits to be set are
 * bit wise or'ed and sent as the second argument to this function.
 * @param[in] pointer to mac_device.
 * @param[in] the data to be programmed.
 * \return 0 on success else return the error status.
 */
s32 eth_dma_control_init(mac_device * gmacdev, u32 init_value)
{
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaControl, init_value);
	return 0;
}


/*Gmac configuration functions*/

/**
 * Enable the watchdog timer on the receiver. 
 * When enabled, Gmac enables Watchdog timer, and GMAC allows no more than
 * 2048 bytes of data (10,240 if Jumbo frame enabled).
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_wd_enable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacWatchdog);
	return;
}
/**
 * Disable the watchdog timer on the receiver. 
 * When disabled, Gmac disabled watchdog timer, and can receive frames up to
 * 16,384 bytes.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_wd_disable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacWatchdog);
	return;
}

/**
 * Enables the Jabber frame support. 
 * When enabled, GMAC disabled the jabber timer, and can transfer 16,384 byte frames.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_jab_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacJabber);
	return;
}
/**
 * Disables the Jabber frame support. 
 * When disabled, GMAC enables jabber timer. It cuts of transmitter if application 
 * sends more than 2048 bytes of data (10240 if Jumbo frame enabled).
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_jab_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacJabber);
	return;
}

/**
 * Enables Frame bursting (Only in Half Duplex Mode). 
 * When enabled, GMAC allows frame bursting in GMII Half Duplex mode.
 * Reserved in 10/100 and Full-Duplex configurations.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_frame_burst_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacFrameBurst);
	return;
}
/**
 * Disables Frame bursting. 
 * When Disabled, frame bursting is not supported.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_frame_burst_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacFrameBurst);
	return;
}

/**
 * Enable Jumbo frame support. 
 * When Enabled GMAC supports jumbo frames of 9018/9022(VLAN tagged).
 * Giant frame error is not reported in receive frame status.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_jumbo_frame_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacJumboFrame);
	return;
}
/**
 * Disable Jumbo frame support. 
 * When Disabled GMAC does not supports jumbo frames.
 * Giant frame error is reported in receive frame status.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_jumbo_frame_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacJumboFrame);
	return;
}

/**
 * Disable Carrier sense. 
 * When Disabled GMAC ignores CRS signal during frame transmission
 * in half duplex mode.
 * @param[in] pointer to mac_device.
 * \return void.
 */

void eth_disable_crs(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacDisableCrs);
	return;
}



/**
 * Selects the GMII port. 
 * When called GMII (1000Mbps) port is selected (programmable only in 10/100/1000 Mbps configuration).
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_select_gmii(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacMiiGmii);
	return;
}
/**
 * Selects the MII port. 
 * When called MII (10/100Mbps) port is selected (programmable only in 10/100/1000 Mbps configuration).
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_select_mii(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacMiiGmii);
	return;
}

/**
 * Enables Receive Own bit (Only in Half Duplex Mode). 
 * When enaled GMAC receives all the packets given by phy while transmitting.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_rx_own_enable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacRxOwn);
	return;
}
/**
 * Disables Receive Own bit (Only in Half Duplex Mode). 
 * When enaled GMAC disables the reception of frames when gmii_txen_o is asserted.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_rx_own_disable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacRxOwn);
	return;
}

/**
 * Sets the GMAC in loopback mode. 
 * When on GMAC operates in loop-back mode at GMII/MII.
 * @param[in] pointer to mac_device.
 * \return returns void.
 * \note (G)MII Receive clock is required for loopback to work properly, as transmit clock is
 * not looped back internally.
 */
void eth_loopback_on(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacLoopback);
	return;
}
/**
 * Sets the GMAC in Normal mode. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_loopback_off(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacLoopback);
	return;
}

/**
 * Sets the GMAC core in Full-Duplex mode. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_set_full_duplex(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacDuplex);
	return;
}
/**
 * Sets the GMAC core in Half-Duplex mode. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_set_half_duplex(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacDuplex);
	return;
}

/**
 * GMAC tries retransmission (Only in Half Duplex mode).
 * If collision occurs on the GMII/MII, GMAC attempt retries based on the 
 * back off limit configured. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 * \note This function is tightly coupled with eth_back_off_limit(macdev *, u32).
 */
void eth_retry_enable(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacRetry);
	return;
}
/**
 * GMAC tries only one transmission (Only in Half Duplex mode).
 * If collision occurs on the GMII/MII, GMAC will ignore the current frami
 * transmission and report a frame abort with excessive collision in tranmit frame status. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_retry_disable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacRetry);
	return;
}

/**
 * GMAC strips the Pad/FCS field of incoming frames.
 * This is true only if the length field value is less than or equal to
 * 1500 bytes. All received frames with length field greater than or equal to
 * 1501 bytes are passed to the application without stripping the Pad/FCS field. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_pad_crc_strip_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacPadCrcStrip);
	return;
}
/**
 * GMAC doesnot strips the Pad/FCS field of incoming frames.
 * GMAC will pass all the incoming frames to Host unmodified. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_pad_crc_strip_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacPadCrcStrip);
	return;
}
/**
 * GMAC programmed with the back off limit value.
 * @param[in] pointer to mac_device.
 * \return returns void.
 * \note This function is tightly coupled with eth_retry_enable(mac_device * gmacdev)
 */
void eth_back_off_limit(mac_device * gmacdev, u32 value)
{
	u32 data;
	data = ethReadReg((u32 *)gmacdev->MacBase, GmacConfig);
	data &= (~GmacBackoffLimit);
	data |= value;
	ethWriteReg((u32 *)gmacdev->MacBase, GmacConfig,data);
	return;
}

/**
 * Enables the Deferral check in GMAC (Only in Half Duplex mode)
 * GMAC issues a Frame Abort Status, along with the excessive deferral error bit set in the 
 * transmit frame status when transmit state machine is deferred for more than
 * 	- 24,288 bit times in 10/100Mbps mode
 * 	- 155,680 bit times in 1000Mbps mode or Jumbo frame mode in 10/100Mbps operation. 
 * @param[in] pointer to mac_device.
 * \return returns void.
 * \note Deferral begins when transmitter is ready to transmit, but is prevented because  of
 * an active CRS (carrier sense) 
 */
void eth_deferral_check_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacDeferralCheck);
	return;
}
/**
 * Disables the Deferral check in GMAC (Only in Half Duplex mode).
 * GMAC defers until the CRS signal goes inactive.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_deferral_check_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacDeferralCheck);
	return;
}
/**
 * Enable the reception of frames on GMII/MII.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_rx_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacRx);
	return;
}
/**
 * Disable the reception of frames on GMII/MII.
 * GMAC receive state machine is disabled after completion of reception of current frame.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_rx_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacRx);
	return;
}
/**
 * Enable the transmission of frames on GMII/MII.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_tx_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacTx);
	return;
}
/**
 * Disable the transmission of frames on GMII/MII.
 * GMAC transmit state machine is disabled after completion of transmission of current frame.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_tx_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacTx);
	return;
}


/*Receive frame filter configuration functions*/

/**
 * Enables reception of all the frames to application.
 * GMAC passes all the frames received to application irrespective of whether they
 * pass SA/DA address filtering or not.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_frame_filter_enable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacFilter);
	return;
}
/**
 * Disables reception of all the frames to application.
 * GMAC passes only those received frames to application which 
 * pass SA/DA address filtering.
 * @param[in] pointer to mac_device.
 * \return void. 
 */
void eth_frame_filter_disable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacFilter);
	return;
}

/**
 * Populates the Hash High register with the data supplied.
 * This function is called when the Hash filtering is to be enabled.
 * @param[in] pointer to mac_device.
 * @param[in] data to be written to hash table high register.
 * \return void. 
 */
void eth_write_hash_table_high(mac_device * gmacdev, u32 data)
{
	ethWriteReg((u32 *)gmacdev->MacBase,GmacHashHigh,data);
	return;
}

/**
 * Populates the Hash Low register with the data supplied.
 * This function is called when the Hash filtering is to be enabled.
 * @param[in] pointer to mac_device.
 * @param[in] data to be written to hash table low register.
 * \return void. 
 */
void eth_write_hash_table_low(mac_device * gmacdev, u32 data)
{
	ethWriteReg((u32 *)gmacdev->MacBase,GmacHashLow,data);
	return;
}

/**
 * Enables Hash or Perfect filter (only if Hash filter is enabled in H/W).
 * Only frames matching either perfect filtering or Hash Filtering as per HMC and HUC 
 * configuration are sent to application.
 * @param[in] pointer to mac_device.
 * \return void. 
 */
void eth_hash_perfect_filter_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacHashPerfectFilter);
	return;
}

/**
 * Enables only Hash(only if Hash filter is enabled in H/W).
 * Only frames matching Hash Filtering as per HMC and HUC 
 * configuration are sent to application.
 * @param[in] pointer to mac_device.
 * \return void. 
 */
void eth_Hash_filter_only_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacHashPerfectFilter);
	return;
}

/**
 * Enables Source address filtering.
 * When enabled source address filtering is performed. Only frames matching SA filtering are passed  to application with 
 * SAMatch bit of RxStatus is set. GMAC drops failed frames. 
 * @param[in] pointer to mac_device.
 * \return void.
 * \note This function is overriden by eth_frame_filter_disable(mac_device *) 
 */
void eth_src_addr_filter_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacSrcAddrFilter);
	return;
}
/**
 * Disables Source address filtering.
 * When disabled GMAC forwards the received frames with updated SAMatch bit in RxStatus. 
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_src_addr_filter_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacSrcAddrFilter);
	return;
}
/**
 * Enables Inverse Destination address filtering.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_dst_addr_filter_inverse(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacDestAddrFilterNor);
	return;
}
/**
 * Enables the normal Destination address filtering.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_dst_addr_filter_normal(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacDestAddrFilterNor);
	return;
}

/**
 * Enables forwarding of control frames.
 * When set forwards all the control frames (incl. unicast and multicast PAUSE frames).
 * @param[in] pointer to mac_device.
 * \return void.
 * \note Depends on RFE of FlowControlRegister[2]
 */
void eth_set_pass_control(mac_device * gmacdev,u32 passcontrol)
{	
	u32 data;
	data = ethReadReg((u32 *)gmacdev->MacBase, GmacFrameFilter);
	data &= (~GmacPassControl);
	data |= passcontrol;
	ethWriteReg((u32 *)gmacdev->MacBase,GmacFrameFilter,data);
	return;
}

/**
 * Enables Broadcast frames.
 * When enabled Address filtering module passes all incoming broadcast frames.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_broadcast_enable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacBroadcast);
	return;
}
/**
 * Disable Broadcast frames.
 * When disabled Address filtering module filters all incoming broadcast frames.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_broadcast_disable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacBroadcast);
	return;
}

/**
 * Enables Multicast frames.
 * When enabled all multicast frames are passed.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_multicast_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacMulticastFilter);
	return;
}
/**
 * Disable Multicast frames.
 * When disabled multicast frame filtering depends on HMC bit.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_multicast_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacMulticastFilter);
	return;
}

/**
 * Enables multicast hash filtering.
 * When enabled GMAC performs teh destination address filtering according to the hash table.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_multicast_hash_filter_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacMcastHashFilter);
	return;
}
/**
 * Disables multicast hash filtering.
 * When disabled GMAC performs perfect destination address filtering for multicast frames, it compares 
 * DA field with the value programmed in DA register.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_multicast_hash_filter_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacMcastHashFilter);
	return;
}

/**
 * Enables promiscous mode.
 * When enabled Address filter modules pass all incoming frames regardless of their Destination
 * and source addresses.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_promisc_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacPromiscuousMode);
	return;
}
/**
 * Clears promiscous mode.
 * When called the GMAC falls back to normal operation from promiscous mode.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_promisc_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacPromiscuousMode);
	return;
}


/**
 * Enables unicast hash filtering.
 * When enabled GMAC performs the destination address filtering of unicast frames according to the hash table.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_unicast_hash_filter_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacUcastHashFilter);
	return;
}
/**
 * Disables multicast hash filtering.
 * When disabled GMAC performs perfect destination address filtering for unicast frames, it compares 
 * DA field with the value programmed in DA register.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_unicast_hash_filter_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFrameFilter, GmacUcastHashFilter);
	return;
}

/*Flow control configuration functions*/

/**
 * Enables detection of pause frames with stations unicast address.
 * When enabled GMAC detects the pause frames with stations unicast address in addition to the
 * detection of pause frames with unique multicast address.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_unicast_pause_frame_detect_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacUnicastPauseFrame);
	return;
}
/**
 * Disables detection of pause frames with stations unicast address.
 * When disabled GMAC only detects with the unique multicast address (802.3x).
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_unicast_pause_frame_detect_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacUnicastPauseFrame);
	return;
}
/**
 * Rx flow control enable.
 * When Enabled GMAC will decode the rx pause frame and disable the tx for a specified time.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_rx_flow_control_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacRxFlowControl);
	return;
}
/**
 * Rx flow control disable.
 * When disabled GMAC will not decode pause frame.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_rx_flow_control_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacRxFlowControl);
	return;
}
/**
 * Tx flow control enable.
 * When Enabled 
 * 	- In full duplex GMAC enables flow control operation to transmit pause frames.
 *	- In Half duplex GMAC enables the back pressure operation
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_tx_flow_control_enable(mac_device * gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacTxFlowControl);
	return;
}

/**
 * Tx flow control disable.
 * When Disabled 
 * 	- In full duplex GMAC will not transmit any pause frames.
 *	- In Half duplex GMAC disables the back pressure feature.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_tx_flow_control_disable(mac_device * gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacTxFlowControl);
	return;
}

/**
 * Initiate Flowcontrol operation.
 * When Set
 * 	- In full duplex GMAC initiates pause control frame.
 *	- In Half duplex GMAC initiates back pressure function.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_tx_activate_flow_control(mac_device * gmacdev)
{
	//In case of full duplex check for this bit to b'0. if it is read as b'1 indicates that
	//control frame transmission is in progress.
	if(gmacdev->Speed == FULLDUPLEX)
	{
		if(!ethCheckBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacFlowControlBackPressure))
			ethSetBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacFlowControlBackPressure);
	}
	else
	{ //if half duplex mode

		ethSetBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacFlowControlBackPressure);
	}

	return;
}

/**
 * stops Flowcontrol operation.
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_tx_deactivate_flow_control(mac_device * gmacdev)
{
	//In full duplex this bit is automatically cleared after transmitting a pause control frame.
	if(gmacdev->Speed == HALFDUPLEX)
	{
		ethSetBits((u32 *)gmacdev->MacBase, GmacFlowControl, GmacFlowControlBackPressure);
	}
	return;
}

/**
 * This enables the pause frame generation after programming the appropriate registers.
 * presently activation is set at 3k and deactivation set at 4k. These may have to tweaked
 * if found any issues
 * @param[in] pointer to mac_device.
 * \return void.
 */
void eth_pause_control(mac_device *gmacdev)
{
	u32 omr_reg;
	u32 mac_flow_control_reg;
	omr_reg = ethReadReg((u32 *)gmacdev->DmaBase,DmaControl);
	omr_reg |= DmaRxFlowCtrlAct4K | DmaRxFlowCtrlDeact5K |DmaEnHwFlowCtrl;
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaControl, omr_reg);

	mac_flow_control_reg = ethReadReg((u32 *)gmacdev->MacBase,GmacFlowControl);
	mac_flow_control_reg |= GmacRxFlowControl | GmacTxFlowControl | 0xFFFF0000;
	ethWriteReg((u32 *)gmacdev->MacBase,GmacFlowControl,mac_flow_control_reg);

	return;

}

// jay hsu : set fast ethernet 10M or 100M
void eth_fast_ethernet_speed(mac_device *gmacdev, u32 speed)
{
	if ( speed == SPEED100 )
		ethSetBits((u32 *)gmacdev->MacBase, GmacConfig, GmacFESpeed100);
	else
		ethClearBits((u32 *)gmacdev->MacBase, GmacConfig, GmacFESpeed100);

	return;	
}

/**
 * Example mac initialization sequence.
 * This function calls the initialization routines to initialize the GMAC register.
 * One can change the functions invoked here to have different configuration as per the requirement
 * @param[in] pointer to mac_device.
 * \return Returns 0 on success.
 */
s32 eth_mac_init(mac_device * gmacdev)
{
	u32 PHYreg;

	if(gmacdev->DuplexMode == FULLDUPLEX)
	{
		eth_wd_enable(gmacdev);
		eth_jab_enable(gmacdev);
		eth_frame_burst_enable(gmacdev);
		eth_jumbo_frame_disable(gmacdev);
		eth_rx_own_enable(gmacdev);
		eth_loopback_off(gmacdev);
		eth_set_full_duplex(gmacdev);
		eth_retry_enable(gmacdev);
		eth_pad_crc_strip_disable(gmacdev);
		eth_back_off_limit(gmacdev,GmacBackoffLimit0);
		eth_deferral_check_disable(gmacdev);
		eth_tx_enable(gmacdev);	
		eth_rx_enable(gmacdev);

		if(gmacdev->Speed == SPEED1000)
			eth_select_gmii(gmacdev);
		else
			eth_select_mii(gmacdev);

		// jay hsu : set fast ethernet speed
		eth_fast_ethernet_speed(gmacdev, gmacdev->Speed);			

		/*Frame Filter Configuration*/
		eth_frame_filter_enable(gmacdev);
		eth_set_pass_control(gmacdev,GmacPassControl0);
		eth_broadcast_enable(gmacdev);
		eth_src_addr_filter_disable(gmacdev);
		eth_multicast_disable(gmacdev);
		eth_dst_addr_filter_normal(gmacdev);
		eth_multicast_hash_filter_disable(gmacdev);
		eth_promisc_disable(gmacdev);
		eth_unicast_hash_filter_disable(gmacdev);

		/*Flow Control Configuration*/
		eth_unicast_pause_frame_detect_disable(gmacdev);
		eth_rx_flow_control_enable(gmacdev);
		eth_tx_flow_control_enable(gmacdev);
	}
	else
	{//for Half Duplex configuration

		eth_wd_enable(gmacdev);
		eth_jab_enable(gmacdev);
		eth_frame_burst_enable(gmacdev);
		eth_jumbo_frame_disable(gmacdev);
		eth_rx_own_enable(gmacdev);
		eth_loopback_off(gmacdev);
		eth_set_half_duplex(gmacdev);
		eth_retry_enable(gmacdev);
		eth_pad_crc_strip_disable(gmacdev);
		eth_back_off_limit(gmacdev,GmacBackoffLimit0);
		eth_deferral_check_disable(gmacdev);
		eth_tx_enable(gmacdev);	
		eth_rx_enable(gmacdev);

		if(gmacdev->Speed == SPEED1000)
			eth_select_gmii(gmacdev);
		else
			eth_select_mii(gmacdev);

		// jay hsu : set fast ethernet speed
		eth_fast_ethernet_speed(gmacdev, gmacdev->Speed);			

		/*Frame Filter Configuration*/
		eth_frame_filter_enable(gmacdev);
		eth_set_pass_control(gmacdev,GmacPassControl0);
		eth_broadcast_enable(gmacdev);
		eth_src_addr_filter_disable(gmacdev);
		eth_multicast_disable(gmacdev);
		eth_dst_addr_filter_normal(gmacdev);
		eth_multicast_hash_filter_disable(gmacdev);
		eth_promisc_disable(gmacdev);
		eth_unicast_hash_filter_disable(gmacdev);

		/*Flow Control Configuration*/
		eth_unicast_pause_frame_detect_disable(gmacdev);
		eth_rx_flow_control_disable(gmacdev);
		eth_tx_flow_control_disable(gmacdev);

		/*To set PHY register to enable CRS on Transmit*/
		ethWriteReg((u32 *)gmacdev->MacBase, GmacGmiiAddr, GmiiBusy | 0x00000408);
		PHYreg = ethReadReg((u32 *)gmacdev->MacBase,GmacGmiiData);
		ethWriteReg((u32 *)gmacdev->MacBase, GmacGmiiData, PHYreg   | 0x00000800);
		ethWriteReg((u32 *)gmacdev->MacBase, GmacGmiiAddr, GmiiBusy | 0x0000040a);
	}
	return 0;
}

// check Micrel KSZ8041NL phy id
s32 eth_get_phy_id(mac_device * gmacdev)
{
	u16 data;
	s32 phy_id=0;
	
	printf("The mac base is 0x%x, the phy base is 0x%x\n", gmacdev->MacBase, gmacdev->PhyBase);
	
	eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_ID_HI_REG, &data);
	phy_id = data << 16;
	eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_ID_LOW_REG, &data);
	phy_id += data;

	return phy_id;
}

// jay hsu : force Micrel KSZ8041NL phy in loopback mode
s32 eth_reset_phy(mac_device * gmacdev)
{
	u16 data;

	s32 status = -ESYNOPGMACNOERR;
	status = eth_write_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTROL_REG, 	Mii_reset );
	if (status)
		return status;

	//msleep(100);
	mdelay(100);

	eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTROL_REG, &data);
	if ( (data & Mii_reset) != 0 )
		return -ESYNOPGMACPHYERR;	

	return -ESYNOPGMACNOERR;			
}


// jay hsu : check 10/100M phy status
s32 eth_check_phy_init_for_10_100M(mac_device * gmacdev)
{
	u8 auto_negotiation = 0;
	u16 data;
	s32 status = -ESYNOPGMACNOERR;
	s32 loop_count;

	loop_count = DEFAULT_LOOP_VARIABLE;
	while(loop_count-- > 0)
	{

		status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_STATUS_REG, &data);
		if(status) {
			TR("$$ $$ check phy data value 0x%x status 0x%x\n", data, status);
			return status;
		}

		if((data & Mii_AutoNegCmplt) != 0){
			TR("Autonegotiation Complete\n");
			break;
		}
		// jay hsu : add delay 20091021. Total autonegotiation time : 10000 x 30us = 300ms
		udelay(1000);
		//plat_delay(10000);
	}

	// jay hsu : check phy whether auto-negotiation or not
	if ( loop_count < 0 )
	{
		TR("Autonegotiation not complete\n");
		auto_negotiation = 0;
	}
	else
		auto_negotiation = 1;

	// use MII reg1 to check link status
	TR("check phy data value 0x%x\n", data);
	if((data & Mii_Link) == 0)
	{
		TR("No Link\n");
		gmacdev->LinkState = LINKDOWN; 
		return -ESYNOPGMACPHYERR;
	}
	else
	{
		gmacdev->LinkState = LINKUP; 
		TR("Link UP\n");
	}
	
	// Howard Chang: Check 10/100M Phy status
	status = eth_read_phy_reg((u32 *) gmacdev->MacBase,
					gmacdev->PhyBase, PHY_CONTROL_REG,
					&data);
	if (status) {
		TR("Read Phy Register Fail in\n");
		return status;
	}

	
	if (data & Mii_En_Auto_Neg) {
		// Check if Auto-Negotiation process completed.
		while (loop_count-- > 0) {
			status =
			    eth_read_phy_reg((u32 *) gmacdev->MacBase,
						   gmacdev->PhyBase,
						   PHY_STATUS_REG, &data);
			if (status) {
				TR("Read Phy Register Fail\n");
				return status;
			}

			if ((data & Mii_AutoNegCmplt) != 0) {
				TR("Autonegotiation Complete\n");
				break;
			}
			// Add delay, and Total autonegotiation time : 10000 x 30us = 300ms
			udelay(1000);
		}
	}

	TR("PHY_STATUS_REG Reg= 0x%04x\n", data);

	// Check Auto-Negotiation result
	if (loop_count < 0) {
		gmacdev->LinkState = LINKDOWN;
		gmacdev->Speed = SPEED100;
		gmacdev->DuplexMode = FULLDUPLEX;

		TR("Ethernet Phy Auto-Negotiation Failed\n");

		return -ESYNOPGMACPHYERR;
	} else
		auto_negotiation = 0;

		
	// jay hsu : force GMac in 10Mbps @ 20090601
	if ( auto_negotiation == 0 )
	{
		// check phy speed decided by boot strapping pin
		status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTROL_REG, &data);
		if(status)	
			return status;

		if ( data & Mii_Speed_100 )
			gmacdev->Speed = SPEED100;
		else
			gmacdev->Speed = SPEED10;

		if ( data & Mii_Duplex )
			gmacdev->DuplexMode = FULLDUPLEX;
		else
			gmacdev->DuplexMode = HALFDUPLEX;

		TR("##Phy No Auto-Negoatiation## %s %s\n", (gmacdev->Speed==SPEED100)?"100M":"10M", (gmacdev->DuplexMode==FULLDUPLEX)?"full-duplex":"half-duplex");

		//		gmacdev->DuplexMode = FULLDUPLEX;
		//		gmacdev->Speed = SPEED10;
		//		printf("phy no auto-negotiation, force Mac in 10M full-duplex mode\n");
		
		status = eth_read_phy_reg((u32 *) gmacdev->MacBase,
						gmacdev->PhyBase,
						PHY_AN_ADV_REG, &data);
		TR("PHY reg ANAR (0x4) = 0x%04x\n", data);

		status = eth_read_phy_reg((u32 *) gmacdev->MacBase,
						gmacdev->PhyBase,
						PHY_LNK_PART_ABl_REG, &data);
		TR("PHY reg ANLPAR (0x5) = 0x%04x\n", data);

		status = eth_read_phy_reg((u32 *) gmacdev->MacBase,
						gmacdev->PhyBase,
						PHY_AN_EXP_REG, &data);
		TR("PHY reg ANER (0x6) = 0x%04x\n", data);

		TR("Set PHY into rising edge output\n");
	}
	else
	{
		status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTRL_2_REG, &data);
		if(status)	
			return status;

		// jay hsu : check phy operation mode @ 20090601
		if ( (data>>2 & 0x0007) == OP_MODE_AUTO_NEGOTIATION )
		{
			TR("phy still in auto-negotiation\n");
			return ESYNOPGMACPHYERR;
		}
		else if ( (data>>2 & 0x0007) == OP_MODE_10BASE_T_HALF )
		{
			gmacdev->DuplexMode = HALFDUPLEX;
			gmacdev->Speed = SPEED10;
			TR("Link is up in 10M half-duplex mode\n");
		}
		else if ( (data>>2 & 0x0007) == OP_MODE_100BASE_TX_HALF )
		{
			gmacdev->DuplexMode = HALFDUPLEX;
			gmacdev->Speed = SPEED100;
			TR("Link is up in 100M half-duplex mode\n");
		}
		else if ( (data>>2 & 0x0007) == OP_MODE_10BASE_T_FULL )
		{
			gmacdev->DuplexMode = FULLDUPLEX;
			gmacdev->Speed = SPEED10;
			TR("Link is up in 10M full-duplex mode\n");			
		}
		else if ( (data>>2 & 0x0007) == OP_MODE_100BASE_TX_FULL )
		{
			gmacdev->DuplexMode = FULLDUPLEX;
			gmacdev->Speed = SPEED100;
			TR("Link is up in 100M full-duplex mode\n");			
		}

	}
	// jay hsu : why set phy in power down mode in half duplex mode
	/*if not set to Master configuration in case of Half duplex mode set it manually as Master*/
	if(gmacdev->DuplexMode == HALFDUPLEX){
		status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTROL_REG, &data);
		if(status)
			return status;

		status = eth_write_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTROL_REG, data | Mii_Manual_Master_Config );
		if(status)
			return status;		
	}

	return -ESYNOPGMACNOERR;
}

/**
 * Checks and initialze phy.
 * This function checks whether the phy initialization is complete. 
 * @param[in] pointer to mac_device.
 * \return 0 if success else returns the error number.
 */

s32 eth_check_phy_init (mac_device * gmacdev)
{	
	//u32 addr;  
	u16 data;
	s32 status = -ESYNOPGMACNOERR;
	s32 loop_count;

	loop_count = DEFAULT_LOOP_VARIABLE;
	while(loop_count-- > 0)
	{

		status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_STATUS_REG, &data);
		if(status)	
			return status;

		if((data & Mii_AutoNegCmplt) != 0){
			TR("Autonegotiation Complete\n");
			break;
		}
	}

	status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_SPECIFIC_STATUS_REG, &data);
	if(status)
		return status;

	if((data & Mii_phy_status_link_up) == 0)
	{
		TR("No Link\n");
		gmacdev->LinkState = LINKDOWN; 
		return -ESYNOPGMACPHYERR;
	}
	else
	{
		gmacdev->LinkState = LINKUP; 
		TR("Link UP\n");
	}

	status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_SPECIFIC_STATUS_REG, &data);
	if(status)
		return status;



	gmacdev->DuplexMode = (data & Mii_phy_status_full_duplex)  ? FULLDUPLEX: HALFDUPLEX ;
	TR("Link is up in %s mode\n",(gmacdev->DuplexMode == FULLDUPLEX) ? "FULL DUPLEX": "HALF DUPLEX");

	/*if not set to Master configuration in case of Half duplex mode set it manually as Master*/
	if(gmacdev->DuplexMode == HALFDUPLEX)
	{
		status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTROL_REG, &data);
		if(status)
			return status;

		status = eth_write_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_CONTROL_REG, data | Mii_Manual_Master_Config );
		if(status)
			return status;		
	}

	status = eth_read_phy_reg((u32 *)gmacdev->MacBase,gmacdev->PhyBase,PHY_SPECIFIC_STATUS_REG, &data);
	if(status)
		return status;
	if(data & Mii_phy_status_speed_1000)
		gmacdev->Speed      =   SPEED1000;
	else if(data & Mii_phy_status_speed_100)
		gmacdev->Speed      =   SPEED100;
	else
		gmacdev->Speed      =   SPEED10;

	if(gmacdev->Speed == SPEED1000)	
		TR("Link is with 1000M Speed \n");
	if(gmacdev->Speed == SPEED100)	
		TR("Link is with 100M Speed \n");
	if(gmacdev->Speed == SPEED10)	
		TR("Link is with 10M Speed \n");

	return -ESYNOPGMACNOERR;
}

/**
 * Sets the Mac address in to GMAC register.
 * This function sets the MAC address to the MAC register in question.
 * @param[in] pointer to mac_device to populate mac dma and phy addresses.
 * @param[in] Register offset for Mac address high
 * @param[in] Register offset for Mac address low
 * @param[in] buffer containing mac address to be programmed.
 * \return 0 upon success. Error code upon failure.
 */
s32 eth_set_mac_addr(mac_device *gmacdev, u32 MacHigh, u32 MacLow, u8 *MacAddr)
{
	u32 data;

	data = (MacAddr[5] << 8) | MacAddr[4];
	ethWriteReg((u32 *)gmacdev->MacBase,MacHigh,data);
	data = (MacAddr[3] << 24) | (MacAddr[2] << 16) | (MacAddr[1] << 8) | MacAddr[0] ;
	ethWriteReg((u32 *)gmacdev->MacBase,MacLow,data);
	return 0;
}


/**
 * Get the Mac address in to the address specified.
 * The mac register contents are read and written to buffer passed.
 * @param[in] pointer to mac_device to populate mac dma and phy addresses.
 * @param[in] Register offset for Mac address high
 * @param[in] Register offset for Mac address low
 * @param[out] buffer containing the device mac address.
 * \return 0 upon success. Error code upon failure.
 */
s32 eth_get_mac_addr(mac_device *gmacdev, u32 MacHigh, u32 MacLow, u8 *MacAddr)
{
	u32 data;

	data = ethReadReg((u32 *)gmacdev->MacBase,MacHigh);
	MacAddr[5] = (data >> 8) & 0xff;
	MacAddr[4] = (data)        & 0xff;

	data = ethReadReg((u32 *)gmacdev->MacBase,MacLow);
	MacAddr[3] = (data >> 24) & 0xff;
	MacAddr[2] = (data >> 16) & 0xff;
	MacAddr[1] = (data >> 8 ) & 0xff;
	MacAddr[0] = (data )      & 0xff;

	TR("The mac address is 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", MacAddr[0], MacAddr[1], MacAddr[2], MacAddr[3], MacAddr[4], MacAddr[5]);
	
	return 0;
}


/**
 * Attaches the eth device structure to the hardware.
 * Device structure is populated with MAC/DMA and PHY base addresses.
 * @param[in] pointer to mac_device to populate mac dma and phy addresses.
 * @param[in] GMAC IP mac base address.
 * @param[in] GMAC IP dma base address.
 * @param[in] GMAC IP phy base address.
 * \return 0 upon success. Error code upon failure.
 * \note This is important function. No kernel api provided by Synopsys 
 */

s32 eth_attach (mac_device *gmacdev, u32 macBase, u32 dmaBase, u32 phyBase)
{
	u8 mac_addr0[6] = DEFAULT_MAC_ADDRESS;

	/*Make sure the Device data strucure is cleared before we proceed further*/
	//memset((void *) gmacdev,0,sizeof(mac_device));
	// <<< Rich: seems something wrong, the previous setting will be cleared !!! >>>

	/*Populate the mac and dma base addresses*/
	gmacdev->MacBase = macBase;
	gmacdev->DmaBase = dmaBase;
	gmacdev->PhyBase = phyBase;

	/* Program/flash in the station/IP's Mac address */
	eth_set_mac_addr(gmacdev,GmacAddr0High,GmacAddr0Low, mac_addr0); 

	return 0;	
}




/**
 * Initialize the rx descriptors for ring or chain mode operation.
 * 	- Status field is initialized to 0.
 *	- EndOfRing set for the last descriptor.
 *	- buffer1 and buffer2 set to 0 for ring mode of operation. (note)
 *	- data1 and data2 set to 0. (note)
 * @param[in] pointer to DmaDesc structure.
 * @param[in] whether end of ring
 * \return void.
 * \note Initialization of the buffer1, buffer2, data1,data2 and status are not done here. This only initializes whether one wants to use this descriptor
 * in chain mode or ring mode. For chain mode of operation the buffer2 and data2 are programmed before calling this function.
 */
void eth_rx_desc_init_ring(DmaDesc *desc, bool last_ring_desc)
{
	desc->status = 0;
	desc->length = last_ring_desc ? RxDescEndOfRing : 0;
	desc->buffer1 = 0;
	desc->buffer2 = 0;
	desc->data1 = 0;
	desc->data2 = 0;
	return;
}
/**
 * Initialize the tx descriptors for ring or chain mode operation.
 * 	- Status field is initialized to 0.
 *	- EndOfRing set for the last descriptor.
 *	- buffer1 and buffer2 set to 0 for ring mode of operation. (note)
 *	- data1 and data2 set to 0. (note)
 * @param[in] pointer to DmaDesc structure.
 * @param[in] whether end of ring
 * \return void.
 * \note Initialization of the buffer1, buffer2, data1,data2 and status are not done here. This only initializes whether one wants to use this descriptor
 * in chain mode or ring mode. For chain mode of operation the buffer2 and data2 are programmed before calling this function.
 */
void eth_tx_desc_init_ring(DmaDesc *desc, bool last_ring_desc)
{
#ifdef ENH_DESC
	desc->status = last_ring_desc? TxDescEndOfRing : 0;
	desc->length = 0; 
#else
	desc->length = last_ring_desc? TxDescEndOfRing : 0;
#endif
	desc->buffer1 = 0;
	desc->buffer2 = 0;
	desc->data1 = 0;
	desc->data2 = 0;
	return;
}



/**
 * Initialize the rx descriptors for chain mode of operation.
 * 	- Status field is initialized to 0.
 *	- EndOfRing set for the last descriptor.
 *	- buffer1 and buffer2 set to 0.
 *	- data1 and data2 set to 0.
 * @param[in] pointer to DmaDesc structure.
 * @param[in] whether end of ring
 * \return void.
 */

void eth_rx_desc_init_chain(DmaDesc * desc)
{
	desc->status = 0;
	desc->length = RxDescChain;
	desc->buffer1 = 0;
	desc->data1 = 0;
	return;
}
/**
 * Initialize the rx descriptors for chain mode of operation.
 * 	- Status field is initialized to 0.
 *	- EndOfRing set for the last descriptor.
 *	- buffer1 and buffer2 set to 0.
 *	- data1 and data2 set to 0.
 * @param[in] pointer to DmaDesc structure.
 * @param[in] whether end of ring
 * \return void.
 */
void eth_tx_desc_init_chain(DmaDesc * desc)
{
#ifdef ENH_DESC
	desc->status = TxDescChain;
	desc->length = 0;
#else
	desc->length = TxDescChain;
#endif
	desc->buffer1 = 0;
	desc->data1 = 0;
	return;
}


s32 eth_init_tx_rx_desc_queue(mac_device *gmacdev)
{
	s32 i;
	for(i =0; i < gmacdev -> TxDescCount; i++)
	{
		eth_tx_desc_init_ring(gmacdev->TxDescBase + i, i == gmacdev->TxDescCount-1);
	}
	TR("At line %d\n",__LINE__);
	for(i =0; i < gmacdev -> RxDescCount; i++)
	{
		eth_rx_desc_init_ring(gmacdev->RxDescBase + i, i == gmacdev->RxDescCount-1);
	}

	gmacdev->TxNextIndx = 0;
	gmacdev->TxBusyIndx = 0;
	gmacdev->RxNextIndx = 0;
	gmacdev->RxBusyIndx = 0;

	return -ESYNOPGMACNOERR;
}
/**
 * Programs the DmaRxBaseAddress with the Rx descriptor base address.
 * Rx Descriptor's base address is available in the gmacdev structure. This function progrms the 
 * Dma Rx Base address with the starting address of the descriptor ring or chain.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_init_rx_desc_base(mac_device *gmacdev)
{
	ethWriteReg((u32 *)gmacdev->DmaBase,DmaRxBaseAddr,(u32)gmacdev->RxDescDma);
	return;
}

/**
 * Programs the DmaTxBaseAddress with the Tx descriptor base address.
 * Tx Descriptor's base address is available in the gmacdev structure. This function progrms the 
 * Dma Tx Base address with the starting address of the descriptor ring or chain.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_init_tx_desc_base(mac_device *gmacdev)
{
	ethWriteReg((u32 *)gmacdev->DmaBase,DmaTxBaseAddr,(u32)gmacdev->TxDescDma);
	return;
}


/** 
 * Makes the Dma as owner for this descriptor.
 * This function sets the own bit of status field of the DMA descriptor,
 * indicating the DMA is the owner for this descriptor. 
 * @param[in] pointer to DmaDesc structure.
 * \return returns void.
 */
void eth_set_owner_dma(DmaDesc *desc)
{
	desc->status |=  DescOwnByDma;
}

/** 
 * set tx descriptor to indicate SOF.
 * This Descriptor contains the start of ethernet frame.
 * @param[in] pointer to DmaDesc structure.
 * \return returns void.
 */
void eth_set_desc_sof(DmaDesc *desc)
{
#ifdef ENH_DESC
	desc->status |= DescTxFirst;//ENH_DESC
#else
	desc->length |= DescTxFirst;
#endif

}

/** 
 * set tx descriptor to indicate EOF.
 * This descriptor contains the End of ethernet frame.
 * @param[in] pointer to DmaDesc structure.
 * \return returns void.
 */
void eth_set_desc_eof(DmaDesc *desc)
{
#ifdef ENH_DESC
	desc->status |= DescTxLast;//ENH_DESC
#else
	desc->length |= DescTxLast;
#endif
}


/** 
 * checks whether this descriptor contains start of frame.
 * This function is to check whether the descriptor's data buffer 
 * contains a fresh ethernet frame?
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if SOF in current descriptor, else returns fail.
 */
bool eth_is_sof_in_rx_desc(DmaDesc *desc)
{
	return ((desc->status & DescRxFirst) == DescRxFirst);                      
}

/** 
 * checks whether this descriptor contains end of frame.
 * This function is to check whether the descriptor's data buffer 
 * contains end of ethernet frame?
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if SOF in current descriptor, else returns fail.
 */
bool eth_is_eof_in_rx_desc(DmaDesc *desc)
{
	return ((desc->status & DescRxLast) == DescRxLast);                      
}

/** 
 * checks whether destination address filter failed in the rx frame.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if Failed, false if not.
 */
bool eth_is_da_filter_failed(DmaDesc *desc)
{
	return ((desc->status & DescDAFilterFail) == DescDAFilterFail);                      
}

/** 
 * checks whether source address filter failed in the rx frame.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if Failed, false if not.
 */
bool eth_is_sa_filter_failed(DmaDesc *desc)
{
	return ((desc->status & DescSAFilterFail) == DescSAFilterFail);                      
}

/** 
 * Checks whether the descriptor is owned by DMA.
 * If descriptor is owned by DMA then the OWN bit is set to 1. This API is same for both ring and chain mode.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if Dma owns descriptor and false if not.
 */
bool eth_is_desc_owned_by_dma(DmaDesc *desc)
{
	return ((desc->status & DescOwnByDma) == DescOwnByDma );
}

/**
 * returns the byte length of received frame including CRC.
 * This returns the no of bytes received in the received ethernet frame including CRC(FCS).
 * @param[in] pointer to DmaDesc structure.
 * \return returns the length of received frame lengths in bytes.
 */
u32 eth_get_rx_desc_frame_length(u32 status)
{
	return ((status & DescFrameLengthMask) >> DescFrameLengthShift);
}

/**
 * Checks whether the descriptor is valid
 * if no errors such as CRC/Receive Error/Watchdog Timeout/Late collision/Giant Frame/Overflow/Descriptor
 * error the descritpor is said to be a valid descriptor.
 * @param[in] pointer to DmaDesc structure.
 * \return True if desc valid. false if error.
 */
bool eth_is_desc_valid(u32 status)
{
	return ((status & DescError) == 0);
}

/**
 * Checks whether the descriptor is empty.
 * If the buffer1 and buffer2 lengths are zero in ring mode descriptor is empty.
 * In chain mode buffer2 length is 0 but buffer2 itself contains the next descriptor address.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if descriptor is empty, false if not empty.
 */
bool eth_is_desc_empty(DmaDesc *desc)
{
	//if both the buffer1 length and buffer2 length are zero desc is empty
	return(((desc->length  & DescSize1Mask) == 0) && ((desc->length  & DescSize2Mask) == 0) );
}


/**
 * Checks whether the rx descriptor is valid.
 * if rx descripor is not in error and complete frame is available in the same descriptor
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if no error and first and last desc bits are set, otherwise it returns false.
 */
bool eth_is_rx_desc_valid(u32 status)
{
	return ((status & DescError) == 0) && ((status & DescRxFirst) == DescRxFirst) && ((status & DescRxLast) == DescRxLast);
}

/**
 * Checks whether the tx is aborted due to collisions.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if collisions, else returns false.
 */
bool eth_is_tx_aborted(u32 status)
{
	return (((status & DescTxLateCollision) == DescTxLateCollision) | ((status & DescTxExcCollisions) == DescTxExcCollisions));

}

/**
 * Checks whether the tx carrier error.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if carrier error occured, else returns falser.
 */
bool eth_is_tx_carrier_error(u32 status)
{
	return (((status & DescTxLostCarrier) == DescTxLostCarrier)  | ((status & DescTxNoCarrier) == DescTxNoCarrier));
}


/**
 * Gives the transmission collision count.
 * returns the transmission collision count indicating number of collisions occured before the frame was transmitted.
 * Make sure to check excessive collision didnot happen to ensure the count is valid.
 * @param[in] pointer to DmaDesc structure.
 * \return returns the count value of collision.
 */
u32 eth_get_tx_collision_count(u32 status)
{
	return ((status & DescTxCollMask) >> DescTxCollShift);
}
u32 eth_is_exc_tx_collisions(u32 status)
{
	return ((status & DescTxExcCollisions) == DescTxExcCollisions);
}


/**
 * Check for damaged frame due to overflow or collision.
 * Retruns true if rx frame was damaged due to buffer overflow in MTL or late collision in half duplex mode.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if error else returns false.
 */
bool eth_is_rx_frame_damaged(u32 status)
{
	//bool eth_dma_rx_collisions(u32 status)
	return (((status & DescRxDamaged) == DescRxDamaged) | ((status & DescRxCollision) == DescRxCollision));
}

/**
 * Check for damaged frame due to collision.
 * Retruns true if rx frame was damaged due to late collision in half duplex mode.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if error else returns false.
 */
bool eth_is_rx_frame_collision(u32 status)
{
	//bool eth_dma_rx_collisions(u32 status)
	return ((status & DescRxCollision) == DescRxCollision);
}

/**
 * Check for receive CRC error.
 * Retruns true if rx frame CRC error occured.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if error else returns false.
 */
bool eth_is_rx_crc(u32 status)
{
	//u32 eth_dma_rx_crc(u32 status)
	return ((status & DescRxCrc) == DescRxCrc);
}

/**
 * Indicates rx frame has non integer multiple of bytes. (odd nibbles).
 * Retruns true if dribbling error in rx frame.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if error else returns false.
 */
bool eth_is_frame_dribbling_errors(u32 status)
{
	//u32 eth_dma_rx_frame_errors(u32 status)
	return ((status & DescRxDribbling) == DescRxDribbling);
}

/**
 * Indicates error in rx frame length.
 * Retruns true if received frame length doesnot match with the length field
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if error else returns false.
 */
bool eth_is_rx_frame_length_errors(u32 status)
{
	//u32 eth_dma_rx_length_errors(u32 status)
	return((status & DescRxLengthError) == DescRxLengthError);
}

/**
 * Checks whether this rx descriptor is last rx descriptor.
 * This returns true if it is last descriptor either in ring mode or in chain mode.
 * @param[in] pointer to devic structure.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if it is last descriptor, false if not.
 * \note This function should not be called before initializing the descriptor using eth_desc_init().
 */
bool eth_is_last_rx_desc(mac_device * gmacdev,DmaDesc *desc)
{
	//bool eth_is_last_desc(DmaDesc *desc)
	return (((desc->length & RxDescEndOfRing) == RxDescEndOfRing) || ((u32)gmacdev->RxDescBase == desc->data2));
}

/**
 * Checks whether this tx descriptor is last tx descriptor.
 * This returns true if it is last descriptor either in ring mode or in chain mode.
 * @param[in] pointer to devic structure.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if it is last descriptor, false if not.
 * \note This function should not be called before initializing the descriptor using eth_desc_init().
 */
bool eth_is_last_tx_desc(mac_device * gmacdev,DmaDesc *desc)
{
	//bool eth_is_last_desc(DmaDesc *desc)
#ifdef ENH_DESC
	return (((desc->status & TxDescEndOfRing) == TxDescEndOfRing) || ((u32)gmacdev->TxDescBase == desc->data2));
#else
	return (((desc->length & TxDescEndOfRing) == TxDescEndOfRing) || ((u32)gmacdev->TxDescBase == desc->data2));
#endif
}

/**
 * Checks whether this rx descriptor is in chain mode.
 * This returns true if it is this descriptor is in chain mode.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if chain mode is set, false if not.
 */
bool eth_is_rx_desc_chained(DmaDesc * desc)
{
	return((desc->length & RxDescChain) == RxDescChain);             
}

/**
 * Checks whether this tx descriptor is in chain mode.
 * This returns true if it is this descriptor is in chain mode.
 * @param[in] pointer to DmaDesc structure.
 * \return returns true if chain mode is set, false if not.
 */
bool eth_is_tx_desc_chained(DmaDesc * desc)
{
#ifdef ENH_DESC
	return((desc->status & TxDescChain) == TxDescChain);             
#else
	return((desc->length & TxDescChain) == TxDescChain);             
#endif
}

/**
 * Driver Api to get the descriptor field information.
 * This returns the status, dma-able address of buffer1, the length of buffer1, virtual address of buffer1
 * dma-able address of buffer2, length of buffer2, virtural adddress of buffer2.
 * @param[in]  pointer to DmaDesc structure.
 * @param[out] pointer to status field fo descriptor.
 * @param[out] dma-able address of buffer1.
 * @param[out] length of buffer1.
 * @param[out] virtual address of buffer1.
 * @param[out] dma-able address of buffer2.
 * @param[out] length of buffer2.
 * @param[out] virtual address of buffer2.
 * \return returns void.
 */
void eth_get_desc_data(DmaDesc *desc, u32 *Status, u32 *Buffer1, u32 *Length1, u32 *Data1, u32 *Buffer2, u32 *Length2, u32 *Data2)
{

	if(Status != 0)   
		*Status = desc->status;

	if(Buffer1 != 0)
		*Buffer1 = desc->buffer1;

	if(Length1 != 0)
		*Length1 = (desc->length & DescSize1Mask) >> DescSize1Shift;

	if(Data1 != 0)
		*Data1 = desc->data1;

	if(Buffer2 != 0)
		*Buffer2 = desc->buffer2;

	if(Length2 != 0)
		*Length2 = (desc->length & DescSize2Mask) >> DescSize2Shift;

	if(Data1 != 0)
		*Data2 = desc->data2;

	return;

}

/**
 * Get the index and address of Tx desc.
 * This api is same for both ring mode and chain mode.
 * This function tracks the tx descriptor the DMA just closed after the transmission of data from this descriptor is 
 * over. This returns the descriptor fields to the caller.
 * @param[in] pointer to mac_device.
 * @param[out] status field of the descriptor.
 * @param[out] Dma-able buffer1 pointer.
 * @param[out] length of buffer1 (Max is 2048).
 * @param[out] virtual pointer for buffer1.
 * @param[out] Dma-able buffer2 pointer.
 * @param[out] length of buffer2 (Max is 2048).
 * @param[out] virtual pointer for buffer2.
 * @param[out] u32 data indicating whether the descriptor is in ring mode or chain mode.
 * \return returns present tx descriptor index on success. Negative value if error.
 */
s32 eth_get_tx_qptr(mac_device * gmacdev, u32 * Status, u32 * Buffer1, u32 * Length1, u32 * Data1, u32 * Buffer2, u32 * Length2, u32 * Data2 )
{
	u32  txover      = gmacdev->TxBusyIndx;
	DmaDesc * txdesc = gmacdev->TxBusyDesc;

	// jay hsu : do cache sync, dram sync to cpu cache
	//dma_cache_sync(txdesc,sizeof(DmaDesc), DMA_FROM_DEVICE);
	inv_dcache_range((u32)txdesc, (u32)sizeof(DmaDesc));

	TR("==============Get TX descriptor 0x%x==============\n", (u32)txdesc);

	if(eth_is_desc_owned_by_dma(txdesc))
	{
		TR("Get TX Qptr Fail at <<eth_is_desc_owned_by_dma>>\n");
		return -1;
	}
	if(eth_is_desc_empty(txdesc))
	{
		TR("Get TX Qptr Fail at <<eth_is_desc_empty>>\n");
		return -1;
	}

	(gmacdev->BusyTxDescNum)--; //busy tx descriptor is reduced by one as it will be handed over to Processor now

	if(Status != 0)   
		*Status = txdesc->status;

	if(Buffer1 != 0)
		*Buffer1 = txdesc->buffer1;
	if(Length1 != 0)
		*Length1 = (txdesc->length & DescSize1Mask) >> DescSize1Shift;
	if(Data1 != 0)
		*Data1 = txdesc->data1;

	if(Buffer2 != 0)
		*Buffer2 = txdesc->buffer2;
	if(Length2 != 0)
		*Length2 = (txdesc->length & DescSize2Mask) >> DescSize2Shift;
	if(Data1 != 0)
		*Data2 = txdesc->data2;

	gmacdev->TxBusyIndx = eth_is_last_tx_desc(gmacdev,txdesc) ? 0 : txover + 1;

	if(eth_is_tx_desc_chained(txdesc)){
		gmacdev->TxBusyDesc = (DmaDesc *)txdesc->data2;
		eth_tx_desc_init_chain(txdesc);
	}
	else{
		gmacdev->TxBusyDesc = eth_is_last_tx_desc(gmacdev,txdesc) ? gmacdev->TxDescBase : (txdesc + 1);
		eth_tx_desc_init_ring(txdesc, eth_is_last_tx_desc(gmacdev,txdesc));
	}
	TR("Get TX Qptr : %02d %08x %08x %08x %08x %08x %08x %08x\n",txover,(u32)txdesc,txdesc->status,txdesc->length,txdesc->buffer1,txdesc->buffer2,txdesc->data1,txdesc->data2);

	wb_dcache_range((u32)txdesc, (u32)sizeof(DmaDesc));

	return txover;	
}

/**
 * Populate the tx desc structure with the buffer address.
 * Once the driver has a packet ready to be transmitted, this function is called with the 
 * valid dma-able buffer addresses and their lengths. This function populates the descriptor
 * and make the DMA the owner for the descriptor. This function also controls whetther Checksum
 * offloading to be done in hardware or not. 
 * This api is same for both ring mode and chain mode.
 * @param[in] pointer to mac_device.
 * @param[in] Dma-able buffer1 pointer.
 * @param[in] length of buffer1 (Max is 2048).
 * @param[in] virtual pointer for buffer1.
 * @param[in] Dma-able buffer2 pointer.
 * @param[in] length of buffer2 (Max is 2048).
 * @param[in] virtual pointer for buffer2.
 * @param[in] u32 data indicating whether the descriptor is in ring mode or chain mode.
 * @param[in] u32 indicating whether the checksum offloading in HW/SW.
 * \return returns present tx descriptor index on success. Negative value if error.
 */
s32 eth_set_tx_qptr(mac_device *gmacdev, u32 Buffer1, u32 Length1, u32 Data1, u32 Buffer2, u32 Length2, u32 Data2, u32 offload_needed)
{
	u32  txnext	= gmacdev->TxNextIndx;
	DmaDesc *txdesc = gmacdev->TxNextDesc;

	//if(!eth_is_desc_empty(txdesc))
	//	return -1;

	(gmacdev->BusyTxDescNum)++; //busy tx descriptor is reduced by one as it will be handed over to Processor now

	if(eth_is_tx_desc_chained(txdesc))
	{
		txdesc->length |= ((Length1 <<DescSize1Shift) & DescSize1Mask);
#ifdef ENH_DESC
		txdesc->status |=  (DescTxFirst | DescTxLast | DescTxIntEnable); //ENH_DESC
#else
		txdesc->length |=  (DescTxFirst | DescTxLast | DescTxIntEnable); //Its always assumed that complete data will fit in to one descriptor
#endif

		txdesc->buffer1 = Buffer1;
		txdesc->data1 = Data1;

		if(offload_needed)
		{
			/*
				 Make sure that the OS you are running supports the IP and TCP checkusm offloaidng,
				 before calling any of the functions given below.		 
			 */
			eth_tx_checksum_offload_ipv4hdr(gmacdev, txdesc);
			eth_tx_checksum_offload_tcponly(gmacdev, txdesc);
		}
#ifdef ENH_DESC
		txdesc->status |= DescOwnByDma;//ENH_DESC
#else
		txdesc->status = DescOwnByDma;
#endif

		gmacdev->TxNextIndx = eth_is_last_tx_desc(gmacdev,txdesc) ? 0 : txnext + 1;
		gmacdev->TxNextDesc = (DmaDesc *)txdesc->data2;
	}
	else
	{
		txdesc->length |= (((Length1 <<DescSize1Shift) & DescSize1Mask) | ((Length2 <<DescSize2Shift) & DescSize2Mask));
#ifdef ENH_DESC
		txdesc->status |=  (DescTxFirst | DescTxLast | DescTxIntEnable); //ENH_DESC
#else
		txdesc->length |=  (DescTxFirst | DescTxLast | DescTxIntEnable); //Its always assumed that complete data will fit in to one descriptor
#endif

		txdesc->buffer1 = Buffer1;	//[Rich: should we convert the buffer1 address into physical address for MAC HW? ]
		txdesc->data1 = Data1;

		txdesc->buffer2 = Buffer2;
		txdesc->data2 = Data2;

		if(offload_needed)
		{
			/*
				 Make sure that the OS you are running supports the IP and TCP checkusm offloaidng,
				 before calling any of the functions given below.		 
			 */
			eth_tx_checksum_offload_ipv4hdr(gmacdev, txdesc);
			eth_tx_checksum_offload_tcponly(gmacdev, txdesc);
		}
#ifdef ENH_DESC	
		txdesc->status |= DescOwnByDma;//ENH_DESC
#else
		txdesc->status = DescOwnByDma;
#endif

		gmacdev->TxNextIndx = eth_is_last_tx_desc(gmacdev,txdesc) ? 0 : txnext + 1;
		gmacdev->TxNextDesc = eth_is_last_tx_desc(gmacdev,txdesc) ? gmacdev->TxDescBase : (txdesc + 1);
	}

	// jay hsu : do cache sync, cpu cache sync to dram
	//dma_cache_sync((u32 *)Buffer1,Length1, DMA_TO_DEVICE);
	wb_inv_dcache_range((u32)Buffer1, Length1);

	//dma_cache_sync(txdesc,sizeof(DmaDesc), DMA_TO_DEVICE);
	wb_inv_dcache_range((u32)txdesc, (u32)sizeof(DmaDesc));

	TR("Set TX Qptr : %02d %08x %08x %08x %08x %08x %08x %08x\n",txnext,(u32)txdesc,txdesc->status,txdesc->length,txdesc->buffer1,txdesc->buffer2,txdesc->data1,txdesc->data2);
	return txnext;	
}

/**
 * Prepares the descriptor to receive packets.
 * The descriptor is allocated with the valid buffer addresses (sk_buff address) and the length fields
 * and handed over to DMA by setting the ownership. After successful return from this function the
 * descriptor is added to the receive descriptor pool/queue.
 * This api is same for both ring mode and chain mode.
 * @param[in] pointer to mac_device.
 * @param[in] Dma-able buffer1 pointer.
 * @param[in] length of buffer1 (Max is 2048).
 * @param[in] Dma-able buffer2 pointer.
 * @param[in] length of buffer2 (Max is 2048).
 * @param[in] u32 data indicating whether the descriptor is in ring mode or chain mode.
 * \return returns present rx descriptor index on success. Negative value if error.
 */
s32 eth_set_rx_qptr(mac_device *gmacdev, u32 Buffer1, u32 Length1, u32 Data1, u32 Buffer2, u32 Length2, u32 Data2)
{
	u32  rxnext		= gmacdev->RxNextIndx;
	DmaDesc *rxdesc = gmacdev->RxNextDesc;

	// jay hsu : do cache sync, dram sync to cpu cache @ 20090825 test
	//dma_cache_sync(rxdesc,sizeof(DmaDesc), DMA_FROM_DEVICE);
	//invalidate_dcache_range((u32)rxdesc,(u32)rxdesc+sizeof(DmaDesc));
	if(!eth_is_desc_empty(rxdesc))
		return -1;

	if(eth_is_rx_desc_chained(rxdesc))
	{
		//rxdesc->length &= ~(DescSize1Mask);
		rxdesc->length |= ((Length1 <<DescSize1Shift) & DescSize1Mask);

		rxdesc->buffer1 = Buffer1;
		rxdesc->data1 = Data1;

		if((rxnext % MODULO_INTERRUPT) !=0)
			rxdesc->length |= RxDisIntCompl;		

		rxdesc->status = DescOwnByDma;

		gmacdev->RxNextIndx     = eth_is_last_rx_desc(gmacdev,rxdesc) ? 0 : rxnext + 1;
		gmacdev->RxNextDesc = (DmaDesc *)rxdesc->data2;
	}
	else
	{
		//rxdesc->length &= ~(DescSize1Mask | DescSize2Mask);
		rxdesc->length |= (((Length1 <<DescSize1Shift) & DescSize1Mask) | ((Length2 << DescSize2Shift) & DescSize2Mask));

		rxdesc->buffer1 = Buffer1;
		rxdesc->data1 = Data1;

		rxdesc->buffer2 = Buffer2;
		rxdesc->data2 = Data2;

		if((rxnext % MODULO_INTERRUPT) !=0)
			rxdesc->length |= RxDisIntCompl;		
		// [Rich: do we need rx interrupt in uboot? ]

		rxdesc->status = DescOwnByDma;

		gmacdev->RxNextIndx     = eth_is_last_rx_desc(gmacdev,rxdesc) ? 0 : rxnext + 1;
		gmacdev->RxNextDesc = eth_is_last_rx_desc(gmacdev,rxdesc) ? gmacdev->RxDescBase : (rxdesc + 1);
	}

	// jay hsu : do cache sync, cpu cache sync to dram
	//dma_cache_sync((u32 *)Buffer1,Length1, DMA_TO_DEVICE);
	//flush_dcache_range((u32 *)Buffer1, (u32 *)Buffer1+Length1) ; /* Omit */

	//dma_cache_sync(rxdesc,sizeof(DmaDesc), DMA_TO_DEVICE);
	wb_inv_dcache_range((u32)rxdesc, (u32)sizeof(DmaDesc));
	//printk("Set Rxdesc 0x%08x\n", (u32)rxdesc);	// jay hsu : test debug it
	TR("Set Rx Qptr %02d %08x %08x %08x %08x %08x %08x %08x\n",rxnext,(u32)rxdesc,rxdesc->status,rxdesc->length,rxdesc->buffer1,rxdesc->buffer2,rxdesc->data1,rxdesc->data2);
	(gmacdev->BusyRxDescNum)++; //One descriptor will be given to Hardware. So busy count incremented by one
	return rxnext;
}

/**
 * Get back the descriptor from DMA after data has been received.
 * When the DMA indicates that the data is received (interrupt is generated), this function should be
 * called to get the descriptor and hence the data buffers received. With successful return from this
 * function caller gets the descriptor fields for processing. check the parameters to understand the 
 * fields returned.`
 * @param[in] pointer to mac_device.
 * @param[out] pointer to hold the status of DMA.
 * @param[out] Dma-able buffer1 pointer.
 * @param[out] pointer to hold length of buffer1 (Max is 2048).
 * @param[out] virtual pointer for buffer1.
 * @param[out] Dma-able buffer2 pointer.
 * @param[out] pointer to hold length of buffer2 (Max is 2048).
 * @param[out] virtual pointer for buffer2.
 * \return returns present rx descriptor index on success. Negative value if error.
 */
s32 eth_get_rx_qptr(mac_device *gmacdev, u32 *Status, u32 *Buffer1, u32 *Length1, u32 *Data1, u32 *Buffer2, u32 *Length2, u32 *Data2)
{
	u32 rxnext       = gmacdev->RxBusyIndx;	// index of descriptor the DMA just completed. May be useful when data 
	// is spread over multiple buffers/descriptors
	DmaDesc *rxdesc = gmacdev->RxBusyDesc;

	// jay hsu : do cache sync, dram sync to cpu cache
	//dma_cache_sync(rxdesc, sizeof(DmaDesc), DMA_FROM_DEVICE);
	inv_dcache_range((u32)rxdesc, (u32)sizeof(DmaDesc));

	TR("==============Get RX descriptor 0x%x==============\n", (u32)rxdesc);

	if(eth_is_desc_owned_by_dma(rxdesc))
	{
		TR("Get RX Qptr Fail at <<eth_is_desc_owned_by_dma>>\n");
		return -1;
	}
	if(eth_is_desc_empty(rxdesc))
	{
		TR("Get RX Qptr Fail at <<eth_is_desc_empty>>\n");
		return -1;
	}

	if(Status != 0)
		*Status = rxdesc->status;// send the status of this descriptor

	if(Length1 != 0)
		*Length1 = (rxdesc->length & DescSize1Mask) >> DescSize1Shift;

	if(Buffer1 != 0)
		*Buffer1 = rxdesc->buffer1;

	if(Data1 != 0)
		*Data1 = rxdesc->data1;	

	TR("Buffer1 0x%x, length %d\n", (u32)*Buffer1, ((rxdesc->length & DescSize1Mask) >> DescSize1Shift));
	// jay hsu : do cache sync, dram sync to cpu cache
	//dma_cache_sync(Buffer1,((rxdesc->length & DescSize1Mask) >> DescSize1Shift), DMA_FROM_DEVICE);
	inv_dcache_range((u32)*Buffer1, ((rxdesc->length & DescSize1Mask) >> DescSize1Shift));

	if(Length2 != 0)
		*Length2 = (rxdesc->length & DescSize2Mask) >> DescSize2Shift;

	if(Buffer2 != 0)
		*Buffer2 = rxdesc->buffer2;

	if(Data1 != 0)
		*Data2 = rxdesc->data2;

	gmacdev->RxBusyIndx = eth_is_last_rx_desc(gmacdev,rxdesc) ? 0 : rxnext + 1;

	if(eth_is_rx_desc_chained(rxdesc))
	{
		gmacdev->RxBusyDesc = (DmaDesc *)rxdesc->data2;
		eth_rx_desc_init_chain(rxdesc);
	}
	else
	{
		gmacdev->RxBusyDesc = eth_is_last_rx_desc(gmacdev,rxdesc) ? gmacdev->RxDescBase : (rxdesc + 1);
		//eth_rx_desc_init_ring(rxdesc, eth_is_last_rx_desc(gmacdev,rxdesc));

		// [Rich: we would like to keep using those buffer1s allocated in open/init function]
		// just change the own ship in rxdesc
		// and this change is better after received data is processed by upper layer function
	}

	TR("Get RX Qptr %02d %08x %08x %08x %08x %08x %08x %08x\n",rxnext,(u32)rxdesc,rxdesc->status,rxdesc->length,rxdesc->buffer1,rxdesc->buffer2,rxdesc->data1,rxdesc->data2);
	(gmacdev->BusyRxDescNum)--; //This returns one descriptor to processor. So busy count will be decremented by one

	return(rxnext);

}

/**
 * Clears all the pending interrupts.
 * If the Dma status register is read then all the interrupts gets cleared
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_clear_interrupt(mac_device *gmacdev)
{
	u32 data;
	data = ethReadReg((u32 *)gmacdev->DmaBase, DmaStatus);
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaStatus ,data);
}

/**
 * Returns the all unmasked interrupt status after reading the DmaStatus register.
 * @param[in] pointer to mac_device.
 * \return 0 upon success. Error code upon failure.
 */
u32 eth_get_interrupt_type(mac_device *gmacdev)
{
	u32 data;
	u32 interrupts = 0;
	data = ethReadReg((u32 *)gmacdev->DmaBase, DmaStatus);
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaStatus ,data); //manju: I think this is the appropriate location to clear the interrupts
	TR("DMA status reg is  %08x\n",data);
	if(data & DmaIntErrorMask)	interrupts |= ethDmaError;
	if(data & DmaIntRxNormMask)	interrupts |= ethDmaRxNormal;
	if(data & DmaIntRxAbnMask)	interrupts |= ethDmaRxAbnormal;
	if(data & DmaIntRxStoppedMask)	interrupts |= ethDmaRxStopped;
	if(data & DmaIntTxNormMask)	interrupts |= ethDmaTxNormal;
	if(data & DmaIntTxAbnMask)	interrupts |= ethDmaTxAbnormal;
	if(data & DmaIntTxStoppedMask)	interrupts |= ethDmaTxStopped;

	return interrupts;
}

/**
 * Returns the interrupt mask.
 * @param[in] pointer to mac_device.
 * \return 0 upon success. Error code upon failure.
 */
u32 eth_get_interrupt_mask(mac_device *gmacdev)
{
	return(ethReadReg((u32 *)gmacdev->DmaBase, DmaInterrupt));
}

/**
 * Enable all the interrupts.
 * Enables the DMA interrupt as specified by the bit mask.
 * @param[in] pointer to mac_device.
 * @param[in] bit mask of interrupts to be enabled.
 * \return returns void.
 */
void eth_enable_interrupt(mac_device *gmacdev, u32 interrupts)
{
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaInterrupt, interrupts);
	return;
}


/**
 * Disable all the interrupts.
 * Disables all DMA interrupts.
 * @param[in] pointer to mac_device.
 * \return returns void.
 * \note This function disabled all the interrupts, if you want to disable a particular interrupt then
 *  use eth_disable_interrupt().
 */
void eth_disable_interrupt_all(mac_device *gmacdev)
{
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaInterrupt, DmaIntDisable);
	return;
}

/**
 * Disable interrupt according to the bitfield supplied.
 * Disables only those interrupts specified in the bit mask in second argument.
 * @param[in] pointer to mac_device.
 * @param[in] bit mask for interrupts to be disabled.
 * \return returns void.
 */
void eth_disable_interrupt(mac_device *gmacdev, u32 interrupts)
{
	ethClearBits((u32 *)gmacdev->DmaBase, DmaInterrupt, interrupts);
	return;
}
/**
 * Enable the DMA Reception.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_enable_dma_rx(mac_device * gmacdev)
{
	//	ethSetBits((u32 *)gmacdev->DmaBase, DmaControl, DmaRxStart);
	u32 data;
	data = ethReadReg((u32 *)gmacdev->DmaBase, DmaControl);
	data |= DmaRxStart; 
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaControl ,data);

}

/**
 * Enable the DMA Transmission.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_enable_dma_tx(mac_device * gmacdev)
{
	//	ethSetBits((u32 *)gmacdev->DmaBase, DmaControl, DmaTxStart);
	u32 data;
	data = ethReadReg((u32 *)gmacdev->DmaBase, DmaControl);
	data |= DmaTxStart; 
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaControl ,data);

}

/**
 * Resumes the DMA Transmission.
 * the DmaTxPollDemand is written. (the data writeen could be anything).
 * This forces the DMA to resume transmission.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_resume_dma_tx(mac_device *gmacdev)
{
	wbflush_ahb();		// jay hsu : flush read/write register commands
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaTxPollDemand, 0);

}
/**
 * Resumes the DMA Reception.
 * the DmaRxPollDemand is written. (the data writeen could be anything).
 * This forces the DMA to resume reception.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_resume_dma_rx(mac_device * gmacdev)
{
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaRxPollDemand, 0);

}
/**
 * Take ownership of this Descriptor.
 * The function is same for both the ring mode and the chain mode DMA structures.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_take_desc_ownership(DmaDesc * desc)
{
	if(desc)
	{
		desc->status &= ~DescOwnByDma;  //Clear the DMA own bit
	}
}

/**
 * Take ownership of all the rx Descriptors.
 * This function is called when there is fatal error in DMA transmission.
 * When called it takes the ownership of all the rx descriptor in rx descriptor pool/queue from DMA.
 * The function is same for both the ring mode and the chain mode DMA structures.
 * @param[in] pointer to mac_device.
 * \return returns void.
 * \note Make sure to disable the transmission before calling this function, otherwise may result in racing situation.
 */
void eth_take_desc_ownership_rx(mac_device * gmacdev)
{
	s32 i;
	DmaDesc *desc;
	desc = gmacdev->RxDescBase;
	for(i = 0; i < gmacdev->RxDescCount; i++)
	{
		if(eth_is_rx_desc_chained(desc))
		{	//This descriptor is in chain mode

			eth_take_desc_ownership(desc);
			desc = (DmaDesc *)desc->data2;
		}
		else
		{
			eth_take_desc_ownership(desc + i);
		}
	}
}

/**
 * Take ownership of all the rx Descriptors.
 * This function is called when there is fatal error in DMA transmission.
 * When called it takes the ownership of all the tx descriptor in tx descriptor pool/queue from DMA.
 * The function is same for both the ring mode and the chain mode DMA structures.
 * @param[in] pointer to mac_device.
 * \return returns void.
 * \note Make sure to disable the transmission before calling this function, otherwise may result in racing situation.
 */
void eth_take_desc_ownership_tx(mac_device * gmacdev)
{
	s32 i;
	DmaDesc *desc;
	desc = gmacdev->TxDescBase;
	for(i = 0; i < gmacdev->TxDescCount; i++)
	{
		if(eth_is_tx_desc_chained(desc))
		{	//This descriptor is in chain mode
			eth_take_desc_ownership(desc);
			desc = (DmaDesc *)desc->data2;
		}
		else
		{
			eth_take_desc_ownership(desc + i);
		}
	}

}

/**
 * Disable the DMA for Transmission.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */

void eth_disable_dma_tx(mac_device * gmacdev)
{	
	//	ethClearBits((u32 *)gmacdev->DmaBase, DmaControl, DmaTxStart);
	u32 data;
	data = ethReadReg((u32 *)gmacdev->DmaBase, DmaControl);
	data &= (~DmaTxStart); 
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaControl ,data);
}
/**
 * Disable the DMA for Reception.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_disable_dma_rx(mac_device * gmacdev)
{	
	//	ethClearBits((u32 *)gmacdev->DmaBase, DmaControl, DmaRxStart);
	u32 data;
	data = ethReadReg((u32 *)gmacdev->DmaBase, DmaControl);
	data &= (~DmaRxStart); 
	ethWriteReg((u32 *)gmacdev->DmaBase, DmaControl ,data);
}

/* RTL phy register operation*/
s32 synopGMAC_set_phy_page_reg(mac_device * gmacdev, u8 page)
{
	u16 data;
	s32 status = -ESYNOPGMACNOERR;

	data = page;
	status =
	    eth_write_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
				    PHY_PAGE_REG, data);
	return status;
}

/* For RTL8201 EL */
s32 synopGMAC_get_phy_test_reg(mac_device * gmacdev)
{
	u16 data;

	eth_read_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
			       PHY_TEST_REG, &data);
	data &= ~(1 << 13);
	eth_write_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
				PHY_TEST_REG, data);
	TR("[NTKETHMAC] RTL8201-EL: PHY Test Reg = 0x%8x\n", data);

	return 0;
}

s32 synopGMAC_set_phy_test_reg(mac_device * gmacdev, u16 data)
{
	eth_write_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
				PHY_TEST_REG, data);
	TR("[NTKETHMAC] RTL8201-EL: Set PHY Test Reg = 0x%8x\n", data);

	return 0;
}

void synopGMAC_get_phy_LBREM_reg(mac_device * gmacdev)
{
	u16 data;

	eth_read_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
			       PHY_LBREM_REG, &data);
	TR("[NTKETHMAC] RTL8201-EL: PHY LBREM Reg = 0x%8x\n", data);

	return;
}

/* For RTL8201F */
s32 synopGMAC_RTL8201F_set_phy_TXC_direction(mac_device * gmacdev,
					     u8 input)
{
	u16 data = 0;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_set_phy_page_reg(gmacdev, 7);
	eth_read_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
			       PHY_RTL8201F_RMII_MODE_SET_REG, &data);

	if (input) {
		data |= (u16) 0x1000;
	} else {
		data &= (u16) (~(0x1000));
	}

	status =
	    eth_write_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
				    PHY_RTL8201F_RMII_MODE_SET_REG, data);
	synopGMAC_set_phy_page_reg(gmacdev, 0);

	return status;
}

s32 synopGMAC_RTL8201F_set_phy_adjust_RMII_TX_timing(mac_device * gmacdev,
						     u8 timing)
{
	u16 data = 0;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_set_phy_page_reg(gmacdev, 7);

	eth_read_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
			       PHY_RTL8201F_RMII_MODE_SET_REG, &data);
	data &= 0xf0ff;
	data = data | (u16) (timing << 8);
	status =
	    eth_write_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
				    PHY_RTL8201F_RMII_MODE_SET_REG, data);
	synopGMAC_set_phy_page_reg(gmacdev, 0);

	return status;
}

s32 synopGMAC_RTL8201F_set_phy_adjust_RMII_RX_timing(mac_device * gmacdev,
						     u8 timing)
{
	u16 data = 0;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_set_phy_page_reg(gmacdev, 7);

	eth_read_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
			       PHY_RTL8201F_RMII_MODE_SET_REG, &data);
	data &= 0xff0f;
	data = data | (u16) (timing << 4);
	status =
	    eth_write_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
				    PHY_RTL8201F_RMII_MODE_SET_REG, data);
	synopGMAC_set_phy_page_reg(gmacdev, 0);

	return status;
}


s32 synopGMAC_get_phy_rxer_reg(mac_device * gmacdev)
{
	u16 data;

	eth_read_phy_reg((u32 *) gmacdev->MacBase, gmacdev->PhyBase,
			       PHY_RXER_REG, &data);

	TR("PHY RX_ER Reg = 0x%8x\n", data);

	return 0;
}


/*******************PMT APIs***************************************/
/**
 * Enables the assertion of PMT interrupt.
 * This enables the assertion of PMT interrupt due to Magic Pkt or Wakeup frame
 * reception.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_pmt_int_enable(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacInterruptMask,GmacPmtIntMask); 
	return;
}
/**
 * Disables the assertion of PMT interrupt.
 * This disables the assertion of PMT interrupt due to Magic Pkt or Wakeup frame
 * reception.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_pmt_int_disable(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacInterruptMask,GmacPmtIntMask); 
	return;
}
/**
 * Enables the power down mode of GMAC.
 * This function puts the Gmac in power down mode.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_power_down_enable(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus,GmacPmtPowerDown);	
	return;
}
/**
 * Disables the powerd down setting of GMAC.
 * If the driver wants to bring up the GMAC from powerdown mode, even though the magic packet or the
 * wake up frames received from the network, this function should be called.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_power_down_disable(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus,GmacPmtPowerDown);	
	return;
}
/**
 * Enables the pmt interrupt generation in powerdown mode.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_enable_pmt_interrupt(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacInterruptMask,GmacPmtIntMask);	
}
/**
 * Disables the pmt interrupt generation in powerdown mode.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_disable_pmt_interrupt(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacInterruptMask,GmacPmtIntMask);	
}
/**
 * Enables GMAC to look for Magic packet.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_magic_packet_enable(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus,GmacPmtMagicPktEnable);	
	return;
}

/**
 * Enables GMAC to look for wake up frame. 
 * Wake up frame is defined by the user.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_wakeup_frame_enable(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus,GmacPmtWakeupFrameEnable);	
	return;
}

/**
 * Enables wake-up frame filter to handle unicast packets.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_pmt_unicast_enable(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus,GmacPmtGlobalUnicast);	
	return;
}
/**
 * Checks whether the packet received is a magic packet?.
 * @param[in] pointer to mac_device.
 * \return returns True if magic packet received else returns false.
 */
bool eth_is_magic_packet_received(mac_device *gmacdev)
{
	u32 data;
	data = 	ethReadReg((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus);	
	return((data & GmacPmtMagicPktReceived) == GmacPmtMagicPktReceived);
}
/**
 * Checks whether the packet received is a wakeup frame?.
 * @param[in] pointer to mac_device.
 * \return returns true if wakeup frame received else returns false.
 */
bool eth_is_wakeup_frame_received(mac_device *gmacdev)
{
	u32 data;
	data = 	ethReadReg((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus);	
	return((data & GmacPmtWakeupFrameReceived) == GmacPmtWakeupFrameReceived);
}

/**
 * Populates the remote wakeup frame registers.
 * Consecutive 8 writes to GmacWakeupAddr writes the wakeup frame filter registers.
 * Before commensing a new write, frame filter pointer is reset to 0x0000.
 * A small delay is introduced to allow frame filter pointer reset operation.
 * @param[in] pointer to mac_device.
 * @param[in] pointer to frame filter contents array.
 * \return returns void.
 */
void eth_write_wakeup_frame_register(mac_device *gmacdev, u32 * filter_contents)
{
	s32 i;
	ethSetBits((u32 *)gmacdev->MacBase,GmacPmtCtrlStatus,GmacPmtFrmFilterPtrReset);
	plat_delay(10);
	for(i =0; i<WAKEUP_REG_LENGTH; i++)
		ethWriteReg((u32 *)gmacdev->MacBase, GmacWakeupAddr,  *(filter_contents + i));
	return;

}
/*******************PMT APIs***************************************/
/*******************MMC APIs***************************************/

/**
 * Freezes the MMC counters.
 * This function call freezes the MMC counters. None of the MMC counters are updated
 * due to any tx or rx frames until eth_mmc_counters_resume is called.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_mmc_counters_stop(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacMmcCntrl,GmacMmcCounterFreeze);
	return;
}
/**
 * Resumes the MMC counter updation.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_mmc_counters_resume(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacMmcCntrl,GmacMmcCounterFreeze);
	return;
}
/**
 * Configures the MMC in Self clearing mode.
 * Programs MMC interface so that counters are cleared when the counters are read.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_mmc_counters_set_selfclear(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacMmcCntrl,GmacMmcCounterResetOnRead);
	return;
}
/**
 * Configures the MMC in non-Self clearing mode.
 * Programs MMC interface so that counters are cleared when the counters are read.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_mmc_counters_reset_selfclear(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacMmcCntrl,GmacMmcCounterResetOnRead);
	return;
}
/**
 * Configures the MMC to stop rollover.
 * Programs MMC interface so that counters will not rollover after reaching maximum value.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_mmc_counters_disable_rollover(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacMmcCntrl,GmacMmcCounterStopRollover);
	return;
}
/**
 * Configures the MMC to rollover.
 * Programs MMC interface so that counters will rollover after reaching maximum value.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_mmc_counters_enable_rollover(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacMmcCntrl,GmacMmcCounterStopRollover);
	return;
}

/**
 * Read the MMC Counter.
 * @param[in] pointer to mac_device.
 * @param[in] the counter to be read.
 * \return returns the read count value.
 */
u32 eth_read_mmc_counter(mac_device *gmacdev, u32 counter)
{
	return(	ethReadReg((u32 *)gmacdev->MacBase,counter));
}
/**
 * Read the MMC Rx interrupt status.
 * @param[in] pointer to mac_device.
 * \return returns the Rx interrupt status.
 */
u32 eth_read_mmc_rx_int_status(mac_device *gmacdev)
{
	return(	ethReadReg((u32 *)gmacdev->MacBase,GmacMmcIntrRx));
}
/**
 * Read the MMC Tx interrupt status.
 * @param[in] pointer to mac_device.
 * \return returns the Tx interrupt status.
 */
u32 eth_read_mmc_tx_int_status(mac_device *gmacdev)
{
	return(	ethReadReg((u32 *)gmacdev->MacBase,GmacMmcIntrTx));
}
/**
 * Disable the MMC Tx interrupt.
 * The MMC tx interrupts are masked out as per the mask specified.
 * @param[in] pointer to mac_device.
 * @param[in] tx interrupt bit mask for which interrupts needs to be disabled.
 * \return returns void.
 */
void eth_disable_mmc_tx_interrupt(mac_device *gmacdev, u32 mask)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacMmcIntrMaskTx,mask);
	return;
}
/**
 * Enable the MMC Tx interrupt.
 * The MMC tx interrupts are enabled as per the mask specified.
 * @param[in] pointer to mac_device.
 * @param[in] tx interrupt bit mask for which interrupts needs to be enabled.
 * \return returns void.
 */
void eth_enable_mmc_tx_interrupt(mac_device *gmacdev, u32 mask)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacMmcIntrMaskTx,mask);
}
/**
 * Disable the MMC Rx interrupt.
 * The MMC rx interrupts are masked out as per the mask specified.
 * @param[in] pointer to mac_device.
 * @param[in] rx interrupt bit mask for which interrupts needs to be disabled.
 * \return returns void.
 */
void eth_disable_mmc_rx_interrupt(mac_device *gmacdev, u32 mask)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacMmcIntrMaskRx,mask);
	return;
}
/**
 * Enable the MMC Rx interrupt.
 * The MMC rx interrupts are enabled as per the mask specified.
 * @param[in] pointer to mac_device.
 * @param[in] rx interrupt bit mask for which interrupts needs to be enabled.
 * \return returns void.
 */
void eth_enable_mmc_rx_interrupt(mac_device *gmacdev, u32 mask)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacMmcIntrMaskRx,mask);
	return;
}
/**
 * Disable the MMC ipc rx checksum offload interrupt.
 * The MMC ipc rx checksum offload interrupts are masked out as per the mask specified.
 * @param[in] pointer to mac_device.
 * @param[in] rx interrupt bit mask for which interrupts needs to be disabled.
 * \return returns void.
 */
void eth_disable_mmc_ipc_rx_interrupt(mac_device *gmacdev, u32 mask)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacMmcRxIpcIntrMask,mask);
	return;
}
/**
 * Enable the MMC ipc rx checksum offload interrupt.
 * The MMC ipc rx checksum offload interrupts are enabled as per the mask specified.
 * @param[in] pointer to mac_device.
 * @param[in] rx interrupt bit mask for which interrupts needs to be enabled.
 * \return returns void.
 */
void eth_enable_mmc_ipc_rx_interrupt(mac_device *gmacdev, u32 mask)
{
	ethClearBits((u32 *)gmacdev->MacBase,GmacMmcRxIpcIntrMask,mask);
	return;
}
/*******************MMC APIs***************************************/
/*******************Ip checksum offloading APIs***************************************/

/**
 * Enables the ip checksum offloading in receive path.
 * When set GMAC calculates 16 bit 1's complement of all received ethernet frame payload.
 * It also checks IPv4 Header checksum is correct. GMAC core appends the 16 bit checksum calculated
 * for payload of IP datagram and appends it to Ethernet frame transferred to the application.
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_enable_rx_chksum_offload(mac_device *gmacdev)
{
	ethSetBits((u32 *)gmacdev->MacBase,GmacConfig,GmacRxIpcOffload);
	return;
}

/**
 * Instruct the DMA to drop the packets fails tcp ip checksum.
 * This is to instruct the receive DMA engine to drop the recevied packet if they 
 * fails the tcp/ip checksum in hardware. Valid only when full checksum offloading is enabled(type-2).
 * @param[in] pointer to mac_device.
 * \return returns void.
 */
void eth_rx_tcpip_chksum_drop_enable(mac_device *gmacdev)
{
	ethClearBits((u32 *)gmacdev->DmaBase,DmaControl,DmaDisableDropTcpCs);
	return;
}


/**
 * The check summ offload engine is enabled to do only IPV4 header checksum.
 * IPV4 header Checksum is computed in the Hardware.
 * @param[in] pointer to mac_device.
 * @param[in] Pointer to tx descriptor for which  ointer to mac_device.
 * \return returns void.
 */
void eth_tx_checksum_offload_ipv4hdr(mac_device *gmacdev, DmaDesc *desc)
{
#ifdef ENH_DESC
	desc->status = ((desc->status & (~DescTxCisMask)) | DescTxCisIpv4HdrCs);//ENH_DESC
#else
	desc->length = ((desc->length & (~DescTxCisMask)) | DescTxCisIpv4HdrCs);
#endif

}

/**
 * The check summ offload engine is enabled to do TCPIP checsum assuming Pseudo header is available.
 * Hardware computes the tcp ip checksum assuming pseudo header checksum is computed in software.
 * Ipv4 header checksum is also inserted.
 * @param[in] pointer to mac_device.
 * @param[in] Pointer to tx descriptor for which  ointer to mac_device.
 * \return returns void.
 */
void eth_tx_checksum_offload_tcponly(mac_device *gmacdev, DmaDesc *desc)
{
#ifdef ENH_DESC
	desc->status = ((desc->status & (~DescTxCisMask)) | DescTxCisTcpOnlyCs);//ENH_DESC
#else
	desc->length = ((desc->length & (~DescTxCisMask)) | DescTxCisTcpOnlyCs);
#endif

}

/*******************Ip checksum offloading APIs***************************************/




