/** \file
 * This file defines the synopsys GMAC device dependent functions.
 * Most of the operations on the GMAC device are available in this file.
 * Functions for initiliasing and accessing MAC/DMA/PHY registers
 * and the DMA descriptors
 * are encapsulated in this file.
 * The functions are platform/host/OS independent.
 * These functions in turn use the
 * low level device dependent (HAL) functions to
 * access the register space.
 * \internal
 * ------------------------REVISION HISTORY---------------------------------
 * Synopsys                 01/Aug/2007                              Created
 */
#include "synopGMAC_Dev.h"
#include <linux/delay.h>
#include <linux/dma-mapping.h>

#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include "m_internal_Phy.h"

unsigned long g_MDC_CLOCK;

/*lock for read/write PHY by MAC register,in order to avoid race condition*/
struct semaphore sem_phy_rw;

s32 ioremap_write(u32 address, u32 value)
{
	unsigned long *remap_address;

	remap_address = ioremap(address, 0x4);

	if (!remap_address)
		return -1;

	writel(value, remap_address);
	iounmap(remap_address);
	return 0;
}

s32 ioremap_AND_value(u32 address, u32 and_value)
{
	unsigned long *remap_address;
	unsigned long value;

	remap_address = ioremap(address, 0x4);

	if (!remap_address)
		return -1;

	value = readl(remap_address);
	value &= and_value;
	writel(value, remap_address);
	iounmap(remap_address);

	return 0;
}

s32 ioremap_OR_value(u32 address, u32 or_value)
{
	unsigned long *remap_address;
	unsigned long value;

	remap_address = ioremap(address, 0x4);

	if (!remap_address)
		return -1;

	value = readl(remap_address);
	value |= or_value;
	writel(value, remap_address);
	iounmap(remap_address);

	return 0;
}

/**
 * Function to set the MDC clock for mdio transactiona
 *
 * @param[in] pointer to device structure.
 * @param[in] clk divider value.
 * \return Reuturns 0 on success else return the error value.
 */
s32 synopGMAC_set_mdc_clk_div(struct synop_device *gmacdev, u32 clk_div_val)
{
	u32 orig_data;

	/*set the mdc clock to the user defined value*/
	orig_data = synopGMACReadReg((unsigned long *)gmacdev->MacBase,
						GmacGmiiAddr);

	orig_data = orig_data & (~(u32)GmiiCsrClkMask);
	orig_data |= clk_div_val;

	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
						GmacGmiiAddr, orig_data);
	return 0;
}

/**
 * Returns the current MDC divider value programmed in the ip.
 *
 * @param[in] pointer to device structure.
 * @param[in] clk divider value.
 * \return Returns the MDC divider value read.
 */
u32 synopGMAC_get_mdc_clk_div(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase,
							GmacGmiiAddr);
	data &= GmiiCsrClkMask;

	return data;
}

/**
 * Function to read the Phy register. The access to phy register
 * is a slow process as the data is moved accross MDI/MDO interface
 * @param[in] pointer to Register Base (It is the mac base in our case) .
 * @param[in] PhyBase register is the index of one of supported 32 PHY devices.
 * @param[in] Register offset is the index of one of the 32 phy register.
 * @param[out] u16 data read from the respective phy register
 * (only valid iff return value is 0).
 * \return Returns 0 on success else return the error status.
 */
s32 synopGMAC_read_phy_reg(unsigned long *RegBase,
							u32 PhyBase,
							u32 RegOffset,
							u16 *data)
{
	u32 loop_variable;
	unsigned long addr;

	addr =
		((PhyBase << GmiiDevShift) & GmiiDevMask) |
		((RegOffset << GmiiRegShift) & GmiiRegMask) |
		g_MDC_CLOCK;

	addr = addr | GmiiBusy; /*Gmii busy bit*/

	/*Critical Zone of R/W PHY by MAC register - Lock*/
	if (down_interruptible(&sem_phy_rw))
		return -EINTR;

	/*write the address from where the data to be read in*/
	/*GmiiGmiiAddr register of synopGMAC ip*/
	synopGMACWriteReg(RegBase, GmacGmiiAddr, addr);

	/*Wait till the busy bit gets cleared with in a certain amount of time*/
	for (loop_variable = 0;
			loop_variable < DEFAULT_LOOP_VARIABLE;
			loop_variable++){
		if (!(synopGMACReadReg(RegBase, GmacGmiiAddr) & GmiiBusy))
			break;
		plat_delay(DEFAULT_DELAY_VARIABLE);
	}

	if (loop_variable < DEFAULT_LOOP_VARIABLE) {
		*data = (u16)(synopGMACReadReg(RegBase, GmacGmiiData) & 0xFFFF);
	} else {
		TR("Error:PHY not respond. Busy bit isn't cleared\n");
		/*Critical Zone of R/W PHY by MAC register - unlock*/
		up(&sem_phy_rw);
		return -ESYNOPGMACPHYERR;
	}
	/*Critical Zone of R/W PHY by MAC register - unlock*/
	up(&sem_phy_rw);
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
s32 synopGMAC_write_phy_reg(unsigned long *RegBase,
							u32 PhyBase,
							u32 RegOffset,
							u16 data)
{
	u32 loop_variable;
	unsigned long addr;

	/*Critical Zone of R/W PHY by MAC register - Lock*/
	if (down_interruptible(&sem_phy_rw))
		return -EINTR;

	/*write the data in to GmacGmiiData register of synopGMAC ip*/
	synopGMACWriteReg(RegBase, GmacGmiiData, data);

	addr = ((PhyBase << GmiiDevShift) & GmiiDevMask) |
		((RegOffset << GmiiRegShift) & GmiiRegMask) |
		GmiiWrite | g_MDC_CLOCK;

	addr = addr | GmiiBusy ; /*set Gmii clk to 20-35 Mhz and Gmii busy bit*/

	synopGMACWriteReg(RegBase, GmacGmiiAddr, addr);
	for (loop_variable = 0;
			loop_variable < DEFAULT_LOOP_VARIABLE;
			loop_variable++){
		if (!(synopGMACReadReg(RegBase, GmacGmiiAddr) & GmiiBusy))
			break;
		plat_delay(DEFAULT_DELAY_VARIABLE);
	}

	/*Critical Zone of R/W PHY by MAC register - unlock*/
	up(&sem_phy_rw);

	if (loop_variable < DEFAULT_LOOP_VARIABLE)
		return -ESYNOPGMACNOERR;

	TR("Error:PHY not respond. Busy bit isn't cleared\n");
	return -ESYNOPGMACPHYERR;
}

/**
 * Function to configure the phy in loopback mode.
 *
 * @param[in] pointer to struct synop_device.
 * @param[in] enable or disable the loopback.
 * \return 0 on success else return the error status.
 * \note Don't get confused with mac loop-back
 * synopGMAC_loopback_on(struct synop_device *)
 * and synopGMAC_loopback_off(struct synop_device *) functions.
 */
s32 synopGMAC_phy_loopback(struct synop_device *gmacdev, bool loopback)
{
	s32 status = -ESYNOPGMACNOERR;
	u16 data;

	if (loopback) {
		synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
				       gmacdev->PhyBase,
				       PHY_CONTROL_REG, &data);
		data = data | Mii_Loopback;

		status = synopGMAC_write_phy_reg(
					(unsigned long *)gmacdev->MacBase,
					 gmacdev->PhyBase,
					 PHY_CONTROL_REG, data);
	} else {
		synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
				       gmacdev->PhyBase,
				       PHY_CONTROL_REG, &data);
		data = data & (u16)(~Mii_Loopback);

		status = synopGMAC_write_phy_reg(
					(unsigned long *)gmacdev->MacBase,
					 gmacdev->PhyBase,
					 PHY_CONTROL_REG,
					 data);
	}

	return status;
}

/**
 * Function to read the GMAC IP Version and populates the same in device data structure.
 * @param[in] pointer to struct synop_device.
 * \return Always return 0.
 */

s32 synopGMAC_read_version(struct synop_device *gmacdev)
{
	u32 data = 0;

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase,
							GmacVersion);
	gmacdev->Version = data;

	TR("The data read from %08x is %08x\n",
	   (gmacdev->MacBase + GmacVersion), data);
	return 0;
}

/**
 * Function to reset the GMAC core.
 * This reests the DMA and GMAC core.
 * After reset all the registers holds their respective reset value
 * @param[in] pointer to struct synop_device.
 * \return 0 on success else return the error status.
 */
s32 synopGMAC_reset(struct synop_device *gmacdev)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaBusMode, DmaResetOn);

	plat_delay(DEFAULT_LOOP_VARIABLE);

	synopGMACReadReg((unsigned long *)gmacdev->DmaBase,
						DmaBusMode);

	return 0;
}

/**
 * Function to program DMA bus mode register.
 *
 * The Bus Mode register is programmed with the value given.
 * The bits to be set are
 * bit wise or'ed and sent as the second argument to this function.
 * @param[in] pointer to struct synop_device.
 * @param[in] the data to be programmed.
 * \return 0 on success else return the error status.
 */
s32 synopGMAC_dma_bus_mode_init(struct synop_device *gmacdev, u32 init_value)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaBusMode, init_value);
	return 0;
}

/**
 * Function to program DMA Control register.
 *
 * The Dma Control register is programmed with the value given.
 * The bits to be set are
 * bit wise or'ed and sent as the second argument to this function.
 * @param[in] pointer to struct synop_device.
 * @param[in] the data to be programmed.
 * \return 0 on success else return the error status.
 */
s32 synopGMAC_dma_control_init(struct synop_device *gmacdev, u32 init_value)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaControl, init_value);
	return 0;
}

/*Gmac configuration functions*/

/**
 * Enable the watchdog timer on the receiver.
 * When enabled, Gmac enables Watchdog timer, and GMAC allows no more than
 * 2048 bytes of data (10,240 if Jumbo frame enabled).
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_wd_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacWatchdog);
}

/**
 * Disable the watchdog timer on the receiver.
 * When disabled, Gmac disabled watchdog timer, and can receive frames up to
 * 16,384 bytes.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_wd_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacWatchdog);
}

/**
 * Enables the Jabber frame support.
 * When enabled, GMAC disabled the jabber timer,
 * and can transfer 16,384 byte frames.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_jab_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacJabber);
}

/**
 * Disables the Jabber frame support.
 * When disabled, GMAC enables jabber timer.
 * It cuts of transmitter if application
 * sends more than 2048 bytes of data (10240 if Jumbo frame enabled).
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_jab_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacJabber);
}

/**
 * Enables Frame bursting (Only in Half Duplex Mode).
 * When enabled, GMAC allows frame bursting in GMII Half Duplex mode.
 * Reserved in 10/100 and Full-Duplex configurations.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_frame_burst_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacFrameBurst);
}

/**
 * Disables Frame bursting.
 * When Disabled, frame bursting is not supported.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_frame_burst_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacFrameBurst);
}

/**
 * Enable Jumbo frame support.
 * When Enabled GMAC supports jumbo frames of 9018/9022(VLAN tagged).
 * Giant frame error is not reported in receive frame status.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_jumbo_frame_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacJumboFrame);
}

/**
 * Disable Jumbo frame support.
 * When Disabled GMAC does not supports jumbo frames.
 * Giant frame error is reported in receive frame status.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_jumbo_frame_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacJumboFrame);
}

/**
 * Disable Carrier sense.
 * When Disabled GMAC ignores CRS signal during frame transmission
 * in half duplex mode.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_disable_crs(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacDisableCrs);
}

/**
 * Selects the GMII port.
 * When called GMII (1000Mbps) port is selected
 * (programmable only in 10/100/1000 Mbps configuration).
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_select_gmii(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacMiiGmii);
}

/**
 * Selects the MII port.
 * When called MII (10/100Mbps) port is selected
 * (programmable only in 10/100/1000 Mbps configuration).
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_select_mii(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacMiiGmii);
}

/**
 * Enables Receive Own bit (Only in Half Duplex Mode).
 * When enaled GMAC receives
 * all the packets given by phy while transmitting.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_rx_own_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacRxOwn);
}

/**
 * Disables Receive Own bit (Only in Half Duplex Mode).
 * When enaled GMAC disables the reception of
 * frames when gmii_txen_o is asserted.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_rx_own_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacRxOwn);
}

/**
 * Sets the GMAC in loopback mode.
 * When on GMAC operates in loop-back mode at GMII/MII.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 * \note (G)MII Receive clock is required for loopback to work properly,
 * as transmit clock is
 * not looped back internally.
 */
void synopGMAC_loopback_on(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacLoopback);
}

/**
 * Sets the GMAC in Normal mode.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_loopback_off(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacLoopback);
}

/**
 * Sets the GMAC core in Full-Duplex mode.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_set_full_duplex(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacDuplex);
}

/**
 * Sets the GMAC core in Half-Duplex mode.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_set_half_duplex(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacDuplex);
}

/**
 * GMAC tries retransmission (Only in Half Duplex mode).
 * If collision occurs on the GMII/MII, GMAC attempt retries based on the
 * back off limit configured.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 * \note This function is tightly coupled with
 * synopGMAC_back_off_limit(synopGMACdev *, u32).
 */
void synopGMAC_retry_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacRetry);
}

/**
 * GMAC tries only one transmission (Only in Half Duplex mode).
 * If collision occurs on the GMII/MII, GMAC will ignore the current frami
 * transmission and report a frame abort
 * with excessive collision in tranmit frame status.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_retry_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacRetry);
}

/**
 * GMAC strips the Pad/FCS field of incoming frames.
 * This is true only if the length field value is less than or equal to
 * 1500 bytes. All received frames with length field greater than or equal to
 * 1501 bytes are passed to the application
 * without stripping the Pad/FCS field.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_pad_crc_strip_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacPadCrcStrip);
}

/**
 * GMAC doesnot strips the Pad/FCS field of incoming frames.
 * GMAC will pass all the incoming frames to Host unmodified.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_pad_crc_strip_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacPadCrcStrip);
}

/**
 * GMAC programmed with the back off limit value.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 * \note This function is tightly coupled with
 * synopGMAC_retry_enable(struct synop_device *gmacdev)
 */
void synopGMAC_back_off_limit(struct synop_device *gmacdev, u32 value)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase, GmacConfig);
	data &= (unsigned long)(~GmacBackoffLimit);
	data |= value;
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase, GmacConfig, data);
}

/**
 * Enables the Deferral check in GMAC (Only in Half Duplex mode)
 * GMAC issues a Frame Abort Status,
 * along with the excessive deferral error bit set in the
 * transmit frame status
 * when transmit state machine is deferred for more than
 * -24,288 bit times in 10/100Mbps mode
 * -155,680 bit times in 1000Mbps mode or Jumbo frame mode
 * in 10/100Mbps operation.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 * \note Deferral begins when transmitter is ready to transmit,
 * but is prevented because  of
 * an active CRS (carrier sense)
 */
void synopGMAC_deferral_check_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacDeferralCheck);
}

/**
 * Disables the Deferral check in GMAC (Only in Half Duplex mode).
 * GMAC defers until the CRS signal goes inactive.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_deferral_check_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacDeferralCheck);
}

/**
 * Enable the reception of frames on GMII/MII.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_rx_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase, GmacConfig, GmacRx);
}

/**
 * Disable the reception of frames on GMII/MII.
 * GMAC receive state machine is disabled
 * after completion of reception of current frame.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_rx_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacRx);
}

/**
 * Enable the transmission of frames on GMII/MII.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_tx_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase, GmacConfig, GmacTx);
}

/**
 * Disable the transmission of frames on GMII/MII.
 * GMAC transmit state machine is
 * disabled after completion of transmission of current frame.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_tx_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacConfig, GmacTx);
}

/*Receive frame filter configuration functions*/

/**
 * Enables reception of all the frames to application.
 * GMAC passes all the frames received to
 * application irrespective of whether they
 * pass SA/DA address filtering or not.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_frame_filter_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
						GmacFrameFilter, GmacFilter);
}

/**
 * Disables reception of all the frames to application.
 * GMAC passes only those received frames to application which
 * pass SA/DA address filtering.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_frame_filter_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
						GmacFrameFilter, GmacFilter);
}

void synopGMAC_mac_addr_setFilter_enable(struct synop_device *gmacdev)
{
	u32 data;

	/*GmacAddr1High*/
	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase, 0x0048);

	data |= 0xC0000000; /*AE: Address Enable, and SA: Source Address*/

	synopGMACWriteReg((unsigned long *)gmacdev->MacBase, 0x0048, data);
}

void synopGMAC_mac_addr_setFilter_disable(struct synop_device *gmacdev)
{
	u32 data;

	/*GmacAddr1High*/
	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase, 0x0048);

	data &= ~(0xC0000000); /*AE: Address Enable, and SA: Source Address*/

	synopGMACWriteReg((unsigned long *)gmacdev->MacBase, 0x0048, data);
}

/**
 * Populates the Hash High register with the data supplied.
 * This function is called when the Hash filtering is to be enabled.
 * @param[in] pointer to struct synop_device.
 * @param[in] data to be written to hash table high register.
 * \return void.
 */
void synopGMAC_write_hash_table_high(struct synop_device *gmacdev, u32 data)
{
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
						GmacHashHigh, data);
}

/**
 * Populates the Hash Low register with the data supplied.
 * This function is called when the Hash filtering is to be enabled.
 * @param[in] pointer to struct synop_device.
 * @param[in] data to be written to hash table low register.
 * \return void.
 */
void synopGMAC_write_hash_table_low(struct synop_device *gmacdev, u32 data)
{
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase, GmacHashLow, data);
}

/**
 * Enables Hash or Perfect filter (only if Hash filter is enabled in H/W).
 * Only frames matching either perfect filtering or
 * Hash Filtering as per HMC and HUC
 * configuration are sent to application.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_hash_perfect_filter_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacHashPerfectFilter);
}

/**
 * Enables only Hash(only if Hash filter is enabled in H/W).
 * Only frames matching Hash Filtering as per HMC and HUC
 * configuration are sent to application.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_Hash_filter_only_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacHashPerfectFilter);
}

/**
 * Enables Source address filtering.
 * When enabled source address filtering is performed.
 * Only frames matching SA filtering are passed  to application with
 * SAMatch bit of RxStatus is set. GMAC drops failed frames.
 * @param[in] pointer to struct synop_device.
 * \return void.
 * \note This function is overridden
 * by synopGMAC_frame_filter_disable(struct synop_device *)
 */
void synopGMAC_src_addr_filter_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacSrcAddrFilter);
}

/**
 * Disables Source address filtering.
 * When disabled GMAC forwards the received frames
 * with updated SAMatch bit in RxStatus.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_src_addr_filter_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacSrcAddrFilter);
}

/**
 * Enables Inverse Source address filtering.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_src_addr_filter_inverse(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacSrcInvaAddrFilter);
}

/**
 * Enables Normal Source address filtering.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_src_addr_filter_normal(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacSrcInvaAddrFilter);
}

/**
 * Enables Inverse Destination address filtering.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_dst_addr_filter_inverse(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacDestAddrFilterNor);
}

/**
 * Enables the normal Destination address filtering.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_dst_addr_filter_normal(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacDestAddrFilterNor);
}

/**
 * Enables forwarding of control frames.
 * When set forwards all the control frames
 * (incl. unicast and multicast PAUSE frames).
 * @param[in] pointer to struct synop_device.
 * \return void.
 * \note Depends on RFE of FlowControlRegister[2]
 */
void synopGMAC_set_pass_control(struct synop_device *gmacdev, u32 passcontrol)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase,
							GmacFrameFilter);

	data &= (unsigned long)(~GmacPassControl);
	data |= passcontrol;

	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
						GmacFrameFilter, data);
}

/**
 * Enables Broadcast frames.
 * When enabled Address filtering module passes all incoming broadcast frames.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_broadcast_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacBroadcast);
}

/**
 * Disable Broadcast frames.
 * When disabled Address filtering module filters
 * all incoming broadcast frames.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_broadcast_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacBroadcast);
}

/**
 * Enables Multicast frames.
 * When enabled all multicast frames are passed.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_multicast_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacMulticastFilter);
}

/**
 * Disable Multicast frames.
 * When disabled multicast frame filtering depends on HMC bit.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_multicast_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacMulticastFilter);
}

/**
 * Enables multicast hash filtering.
 * When enabled GMAC performs the destination address filtering
 * according to the hash table.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_multicast_hash_filter_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacMcastHashFilter);
}

/**
 * Disables multicast hash filtering.
 * When disabled GMAC performs perfect destination address
 * filtering for multicast frames, it compares
 * DA field with the value programmed in DA register.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_multicast_hash_filter_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacMcastHashFilter);
}

/**
 * Enables promiscous mode.
 * When enabled Address filter modules pass all incoming frames
 * regardless of their Destination
 * and source addresses.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_promisc_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacPromiscuousMode);
}

/**
 * Clears promiscous mode.
 * When called the GMAC falls back to normal operation from promiscous mode.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_promisc_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacPromiscuousMode);
}

/**
 * Enables unicast hash filtering.
 * When enabled GMAC performs the destination address
 * filtering of unicast frames according to the hash table.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_unicast_hash_filter_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter, GmacUcastHashFilter);
}

/**
 * Disables multicast hash filtering.
 * When disabled GMAC performs perfect destination address
 * filtering for unicast frames, it compares
 * DA field with the value programmed in DA register.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_unicast_hash_filter_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFrameFilter,
			GmacUcastHashFilter);
}

/*Flow control configuration functions*/

/**
 * Enables detection of pause frames with stations unicast address.
 * When enabled GMAC detects the pause frames
 * with stations unicast address in addition to the
 * detection of pause frames with unique multicast address.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_unicast_pause_frame_detect_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFlowControl,
			GmacUnicastPauseFrame);
}

/**
 * Disables detection of pause frames with stations unicast address.
 * When disabled GMAC only detects with the unique multicast address (802.3x).
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_unicast_pause_frame_detect_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFlowControl,
			GmacUnicastPauseFrame);
}

/**
 * Rx flow control enable.
 * When Enabled GMAC will decode the rx pause frame
 * and disable the tx for a specified time.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_rx_flow_control_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFlowControl,
			GmacRxFlowControl);
}

/**
 * Rx flow control disable.
 * When disabled GMAC will not decode pause frame.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_rx_flow_control_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFlowControl,
			GmacRxFlowControl);
}

/**
 * Tx flow control enable.
 * When Enabled
 * -In full duplex GMAC
 * enables flow control operation to transmit pause frames.
 * -In Half duplex GMAC enables the back pressure operation
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_tx_flow_control_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFlowControl, GmacTxFlowControl);
}

/**
 * Tx flow control disable.
 * When Disabled
 *-In full duplex GMAC will not transmit any pause frames.
 *-In Half duplex GMAC disables the back pressure feature.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_tx_flow_control_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits(
			(unsigned long *)gmacdev->MacBase,
			GmacFlowControl, GmacTxFlowControl);
}

/**
 * Initiate Flowcontrol operation.
 * When Set
 * -In full duplex GMAC initiates pause control frame.
 * -In Half duplex GMAC initiates back pressure function.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_tx_activate_flow_control(struct synop_device *gmacdev)
{
	/*In case of full duplex check for this bit to b'0. */
	/*If it is read as b'1 indicates that */
	/*control frame transmission is in progress.*/
	if (gmacdev->Speed == FULLDUPLEX) {
		if (!synopGMACCheckBits(
					(unsigned long *)gmacdev->MacBase,
					GmacFlowControl,
					GmacFlowControlBackPressure))
			synopGMACSetBits(
					(unsigned long *)gmacdev->MacBase,
					GmacFlowControl,
					GmacFlowControlBackPressure);
	} else {
		/*if half duplex mode*/
		synopGMACSetBits(
				(unsigned long *)gmacdev->MacBase,
				GmacFlowControl,
				GmacFlowControlBackPressure);
	}
}

/**
 * stops Flowcontrol operation.
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_tx_deactivate_flow_control(struct synop_device *gmacdev)
{
	/*In full duplex this bit is automatically*/
	/*cleared after transmitting a pause control frame.*/
	if (gmacdev->Speed == HALFDUPLEX) {
		synopGMACSetBits(
				(unsigned long *)gmacdev->MacBase,
				GmacFlowControl,
				GmacFlowControlBackPressure);
	}
}

/**
 * This enables the pause frame generation after programming
 * the appropriate registers.
 * presently activation is set at 3k and deactivation set at 4k.
 * These may have to tweaked
 * if found any issues
 * @param[in] pointer to struct synop_device.
 * \return void.
 */
void synopGMAC_pause_control(struct synop_device *gmacdev)
{
	u32 omr_reg;
	u32 mac_flow_control_reg;

	omr_reg  = synopGMACReadReg((unsigned long *)gmacdev->DmaBase,
								DmaControl);
	omr_reg |= DmaRxFlowCtrlAct4K | DmaRxFlowCtrlDeact5K | DmaEnHwFlowCtrl;

	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaControl, omr_reg);

	mac_flow_control_reg = synopGMACReadReg(
			(unsigned long *)gmacdev->MacBase, GmacFlowControl);
	mac_flow_control_reg |=
		GmacRxFlowControl | GmacTxFlowControl | 0xFFFF0000;

	synopGMACWriteReg(
			(unsigned long *)gmacdev->MacBase,
			GmacFlowControl, mac_flow_control_reg);
}

/*Set fast ethernet 10M or 100M*/
void synopGMAC_fast_ethernet_speed(struct synop_device *gmacdev, u32 speed)
{
	if (speed == SPEED100) {
		synopGMACSetBits(
				(unsigned long *)gmacdev->MacBase,
				GmacConfig, GmacFESpeed100);
	} else {
		synopGMACClearBits(
				(unsigned long *)gmacdev->MacBase,
				GmacConfig, GmacFESpeed100);
	}
}

/**
 * Example mac initialization sequence.
 * This function calls the initialization routines
 * to initialize the GMAC register.
 * One can change the functions invoked here
 * to have different configuration as per the requirement
 * @param[in] pointer to struct synop_device.
 * \return Returns 0 on success.
 */
s32 synopGMAC_mac_init(struct synop_device *gmacdev, u32 promisc)
{
	u32 PHYreg;

	pr_crit("[NTKETHMAC] MAC init duplex=%d, speed=%d\n",
		gmacdev->DuplexMode, gmacdev->Speed);
	if (gmacdev->DuplexMode == FULLDUPLEX) {
		synopGMAC_wd_enable(gmacdev);
		synopGMAC_jab_enable(gmacdev);
		synopGMAC_frame_burst_enable(gmacdev);
		synopGMAC_jumbo_frame_disable(gmacdev);
		synopGMAC_rx_own_enable(gmacdev);
		synopGMAC_loopback_off(gmacdev);
		synopGMAC_set_full_duplex(gmacdev);
		synopGMAC_retry_enable(gmacdev);
		synopGMAC_pad_crc_strip_disable(gmacdev);
		synopGMAC_back_off_limit(gmacdev, GmacBackoffLimit0);
		synopGMAC_deferral_check_disable(gmacdev);
		synopGMAC_tx_enable(gmacdev);
		synopGMAC_rx_enable(gmacdev);

		if (gmacdev->Speed == SPEED1000)
			synopGMAC_select_gmii(gmacdev);
		else
			synopGMAC_select_mii(gmacdev);

		/*set fast ethernet speed*/
		synopGMAC_fast_ethernet_speed(gmacdev, gmacdev->Speed);

		/*Frame Filter Configuration*/
		synopGMAC_set_pass_control(gmacdev, GmacPassControl0);
		synopGMAC_broadcast_enable(gmacdev);
		synopGMAC_multicast_enable(gmacdev);
		synopGMAC_dst_addr_filter_normal(gmacdev);
		synopGMAC_multicast_hash_filter_disable(gmacdev);
		synopGMAC_unicast_hash_filter_disable(gmacdev);

		/*Flow Control Configuration*/
		synopGMAC_unicast_pause_frame_detect_disable(gmacdev);
		synopGMAC_rx_flow_control_enable(gmacdev);
		synopGMAC_tx_flow_control_enable(gmacdev);
	} else {
		/*for Half Duplex configuration*/
		synopGMAC_wd_enable(gmacdev);
		synopGMAC_jab_enable(gmacdev);
		synopGMAC_frame_burst_enable(gmacdev);
		synopGMAC_jumbo_frame_disable(gmacdev);
		synopGMAC_rx_own_enable(gmacdev);
		synopGMAC_loopback_off(gmacdev);
		synopGMAC_set_half_duplex(gmacdev);
		synopGMAC_retry_enable(gmacdev);
		synopGMAC_pad_crc_strip_disable(gmacdev);
		synopGMAC_back_off_limit(gmacdev, GmacBackoffLimit0);
		synopGMAC_deferral_check_disable(gmacdev);
		synopGMAC_tx_enable(gmacdev);
		synopGMAC_rx_enable(gmacdev);

		if (gmacdev->Speed == SPEED1000)
			synopGMAC_select_gmii(gmacdev);
		else
			synopGMAC_select_mii(gmacdev);

		/*set fast ethernet speed*/
		synopGMAC_fast_ethernet_speed(gmacdev, gmacdev->Speed);

		/*Frame Filter Configuration*/
		synopGMAC_set_pass_control(gmacdev, GmacPassControl0);
		synopGMAC_broadcast_enable(gmacdev);
		synopGMAC_multicast_enable(gmacdev);
		synopGMAC_dst_addr_filter_normal(gmacdev);
		synopGMAC_multicast_hash_filter_disable(gmacdev);
		synopGMAC_unicast_hash_filter_disable(gmacdev);

		/*Flow Control Configuration*/
		synopGMAC_unicast_pause_frame_detect_disable(gmacdev);
		synopGMAC_rx_flow_control_disable(gmacdev);
		synopGMAC_tx_flow_control_disable(gmacdev);

		/*To set PHY register to enable CRS on Transmit*/
		synopGMACWriteReg(
				(unsigned long *)gmacdev->MacBase,
				GmacGmiiAddr, GmiiBusy | 0x00000408);
		PHYreg = synopGMACReadReg((unsigned long *)gmacdev->MacBase,
					  GmacGmiiData);
		synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
				  GmacGmiiData, PHYreg | 0x00000800);
		synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
				  GmacGmiiAddr, GmiiBusy | 0x0000040a);
	}

	synopGMAC_promisc_disable(gmacdev);

	if (promisc) {
		synopGMAC_frame_filter_disable(gmacdev);
		synopGMAC_src_addr_filter_enable(gmacdev);
		synopGMAC_src_addr_filter_inverse(gmacdev);
		synopGMAC_mac_addr_setFilter_enable(gmacdev);
		pr_crit("[NTKETHMAC]: Promiscuous Mode is enabled.\n");
	} else {
		synopGMAC_frame_filter_enable(gmacdev);
		synopGMAC_src_addr_filter_disable(gmacdev);
		synopGMAC_src_addr_filter_normal(gmacdev);
		synopGMAC_mac_addr_setFilter_disable(gmacdev);
		pr_crit("[NTKETHMAC]: Promiscuous Mode is disabled.\n");
	}
	return 0;
}

/*check Micrel KSZ8041NL phy id*/
u32 synopGMAC_get_phy_id(struct synop_device *gmacdev)
{
	u16 data;
	u32 phy_id = 0;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_ID_HI_REG,
			&data);

	phy_id = (u32)(data << 16);

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_ID_LOW_REG,
			&data);

	phy_id += data;

	gmacdev->PHY_ID = phy_id;
	return phy_id;
}

void synopGMAC_restart_auto_nego(struct synop_device *gmacdev)
{
	u16 data = 0;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_CONTROL_REG,
			&data);

	data |= 0x0200;

	synopGMAC_write_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_CONTROL_REG,
			data);
}

#ifdef CONFIG_NVT_RTL_PHY_SUPPORT
u16 synopGMAC_get_phy_ctrl_reg(struct synop_device *gmacdev)
{
	u16 data = 0;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_CONTROL_REG, &data);
	return data;
}

s32 synopGMAC_set_phy_page_reg(struct synop_device *gmacdev, u8 page)
{
	u16 data;
	s32 status = -ESYNOPGMACNOERR;

	data =  page;
	status = synopGMAC_write_phy_reg(
			(u32 *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_PAGE_REG, data);
	return status;
}

/* For RTL8201 EL */
s32 synopGMAC_get_phy_test_reg(struct synop_device *gmacdev)
{
	u16 data;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_TEST_REG, &data);
	pr_crit("PHY Test Reg = 0x%8x\n", data);

	return 0;
}

s32 synopGMAC_get_phy_rxer_reg(struct synop_device *gmacdev)
{
	u16 data;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_RXER_REG, &data);

	pr_crit("PHY RX_ER Reg = 0x%8x\n", data);

	return 0;
}

void  synopGMAC_get_phy_LBREM_reg(struct synop_device *gmacdev)
{
	u16 data;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_LBREM_REG, &data);
	pr_crit("PHY LBREM Reg = 0x%8x\n", data);
}

/* For RTL8201 EL */

/* For RTL8201 F */
u16 synopGMAC_RTL8201F_get_phy_RMII_mode_setting_reg(
	struct synop_device *gmacdev)
{
	u16 data = 0;

	synopGMAC_set_phy_page_reg(gmacdev, 7);
	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			&data);
	synopGMAC_set_phy_page_reg(gmacdev, 0);
	return data;
}

s32 synopGMAC_RTL8201F_set_phy_TXC_direction(
		struct synop_device *gmacdev, u8 input)
{
	/*original
	* synopGMAC_RTL8201F_set_phy_TXC_direction
	*/
	u16 data = 0;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_set_phy_page_reg(gmacdev, 7);

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			&data);

	if (input)
		data |= (u16)0x1000;
	else
		data &= (u16)(~(0x1000));

	status = synopGMAC_write_phy_reg(
			(u32 *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			data);

	synopGMAC_set_phy_page_reg(gmacdev, 0);
	return status;
}

s32 synopGMAC_RTL8201F_set_phy_adjust_RMII_TX_timing(
		struct synop_device *gmacdev, u8 timing)
{
	u16 data = 0;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_set_phy_page_reg(gmacdev, 7);
	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			&data);

	data &= 0xf0ff;
	data = data | (u16)(timing << 8);

	status = synopGMAC_write_phy_reg(
			(u32 *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			data);

	synopGMAC_set_phy_page_reg(gmacdev, 0);
	return status;
}

s32 synopgmac_rtl8201f_set_phy_rmii_rx_timing(
		struct synop_device *gmacdev, u8 timing)
{
	/*original naming:
	* synopGMAC_RTL8201F_set_phy_adjust_RMII_RX_timing
	*/
	u16 data = 0;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_set_phy_page_reg(gmacdev, 7);
	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			&data);

	data &= 0xff0f;
	data = data | (u16)(timing << 4);

	status = synopGMAC_write_phy_reg(
			(u32 *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			data);

	synopGMAC_set_phy_page_reg(gmacdev, 0);
	return status;
}

s32 synopGMAC_RTL8201F_set_phy_CRS_RXDV_pin(
		struct synop_device *gmacdev,
		u8 pin_CRS)
{
	u16 data = 0;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_set_phy_page_reg(gmacdev, 7);
	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			&data);

	if (pin_CRS)
		data &= (u16)(~(0x0004));
	else
		data |= (u16)(0x1004);

	status = synopGMAC_write_phy_reg(
			(u32 *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_RMII_MODE_SET_REG,
			data);

	synopGMAC_set_phy_page_reg(gmacdev, 0);

	return status;
}

u16 synopGMAC_RTL8201F_get_phy_fibermode_loopback_reg(
	struct synop_device *gmacdev)
{
	u16 data = 0;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_FIBER_MODE_LOOPBACK_REG,
			&data);

	data &= (u16)(~(0x0004));

	synopGMAC_write_phy_reg(
			(u32 *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_FIBER_MODE_LOOPBACK_REG,
			data);

	return data;
}

u16 synopGMAC_RTL8201F_get_phy_interrupt_status(struct synop_device *gmacdev)
{
	u16 data = 0;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_RTL8201F_INT_SNR_REG,
			&data);

	return data;
}

/* For RTL8201 F */
#endif

/*force in our speed*/
s32 synopGMAC_set_phy_speed(struct synop_device *gmacdev, u8 speed10)
{
	u16 data;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_CONTROL_REG, &data);

	pr_crit("PHY CTRL Reg = %04x\n", data);

	if (speed10 == 0) {
		pr_crit("Set 100 M\n");
		data = 0x2100;
	} else {
		pr_crit("Set 10 M\n");
		data = 0x0100;
	}
	status = synopGMAC_write_phy_reg((unsigned long *)gmacdev->MacBase,
					 gmacdev->PhyBase,
					 PHY_CONTROL_REG, data);
	if (status)
		return status;

	if (speed10 == 0)
		gmacdev->Speed = SPEED100;
	else
		gmacdev->Speed = SPEED10;

	gmacdev->DuplexMode = FULLDUPLEX;
	gmacdev->LinkState = LINKUP;

	status = synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase,
			PHY_STATUS_REG, &data);

	pr_info("PHY STATUS Reg = %04x\n", data);

	if ((data & Mii_Link) == 0)
		pr_crit("Ethernet Phy No Link\n");
	else
		pr_crit("Ethernet Phy Link UP\n");

	return -ESYNOPGMACNOERR;
}

/*force Micrel KSZ8041NL phy in loopback mode*/
s32 synopGMAC_reset_phy(struct synop_device *gmacdev)
{
	u16 data;
	s32 status = -ESYNOPGMACNOERR;

	synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
			       gmacdev->PhyBase, PHY_CONTROL_REG, &data);

	data |= Mii_reset;
	status = synopGMAC_write_phy_reg((unsigned long *)gmacdev->MacBase,
					 gmacdev->PhyBase,
					 PHY_CONTROL_REG, data);

	if (status)
		return status;

	msleep(100);

	synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
			       gmacdev->PhyBase, PHY_CONTROL_REG, &data);

	if ((data & Mii_reset) != 0)
		return -ESYNOPGMACPHYERR;

	return -ESYNOPGMACNOERR;
}

/*check 10/100M phy status*/
s32 synopGMAC_check_phy_init_for_10_100M(struct synop_device *gmacdev)
{
	u16 data;
	s32 status = -ESYNOPGMACNOERR;
	s32 loop_count = AUTO_NEGO_WAITIG_LOOP_TIME;

	status = synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_CONTROL_REG, &data);

	if (status)
		return status;

	gmacdev->AutoNgFail = 0;
	if (data & Mii_En_Auto_Neg) {
		/*check if auto-negotiation process completed*/
		while (loop_count-- > 0) {
			status = synopGMAC_read_phy_reg(
					(unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					PHY_STATUS_REG,
					&data);

			if (status)
				return status;

			if ((data & Mii_AutoNegCmplt) != 0) {
				pr_crit("[NTKETHMAC] Autonegotiation Complete\n");
				break;
			}
			/*Total autonegotiation time : 10000 x 30us = 300ms*/
			/*udelay(1000);*/
			/*currently, this function is called by
			* init and link detect (work queue)
			* it is not atomic, usleep is OK
			*/
			usleep_range(1000, 1100);
		}
	}

	/*currently, this function is called by
	* init and link detect (work queue)
	* it is not atomic, usleep is OK
	*/
	/*udelay(1000);*/
	usleep_range(1000, 1100);

	/*check link status*/
	status = synopGMAC_read_phy_reg(
			(unsigned long *)gmacdev->MacBase,
			gmacdev->PhyBase, PHY_STATUS_REG, &data);

	if (status)
		return status;

	if (data & Mii_Link) {
		gmacdev->LinkState = LINKUP;
		pr_crit("[NTKETHMAC] Link UP\n");
	} else {
		pr_crit("\033[1;31m[NTKETHMAC] No Link\033[0m\n");
		gmacdev->LinkState = LINKDOWN;
		gmacdev->Speed = SPEED100;
		gmacdev->DuplexMode = FULLDUPLEX;
		return -ESYNOPGMACPHYERR;
	}

	/*check auto-negotiation result*/
	if (loop_count < 0) {
		gmacdev->LinkState = LINKDOWN;
		gmacdev->Speed = SPEED100;
		gmacdev->DuplexMode = FULLDUPLEX;
		gmacdev->AutoNgFail = 1;
		pr_crit("\033[1;31m[NTKETHMAC_ERROR]: Ethernet Phy Auto-Negotiation Failed\033[0m\n");
		return -ESYNOPGMACPHYERR;
	}

#if defined(CONFIG_NVT_RTL_PHY_SUPPORT) || \
	defined(CONFIG_NVT_ICPLUS_PHY_SUPPORT) || \
	defined(CONFIG_NVT_INTERNAL_PHY)

	status = nvt_nternal_PHY_display_status(gmacdev);
	if (status)
		return status;

#elif defined(CONFIG_NVT_MICREL_PHY_SUPPORT)
	/*the method of Micrel phy,
	 * why can't get the correct result on reg 0 bit13 and bit8.
	 */
	status = synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					PHY_CONTRL_2_REG, &data);

	if (status)
		return status;

	if ((data >> 2 & 0x0007) == OP_MODE_AUTO_NEGOTIATION) {
		TR("phy still in auto-negotiation\n");
		return -ESYNOPGMACPHYERR;
	} else if ((data >> 2 & 0x0007) == OP_MODE_10BASE_T_HALF) {
		gmacdev->DuplexMode = HALFDUPLEX;
		gmacdev->Speed = SPEED10;
		TR("Link is up in 10M half-duplex mode\n");
	} else if ((data >> 2 & 0x0007) == OP_MODE_100BASE_TX_HALF) {
		gmacdev->DuplexMode = HALFDUPLEX;
		gmacdev->Speed = SPEED100;
		TR("Link is up in 100M half-duplex mode\n");
	} else if ((data >> 2 & 0x0007) == OP_MODE_10BASE_T_FULL) {
		gmacdev->DuplexMode = FULLDUPLEX;
		gmacdev->Speed = SPEED10;
		TR("Link is up in 10M full-duplex mode\n");
	} else if ((data >> 2 & 0x0007) == OP_MODE_100BASE_TX_FULL) {
		gmacdev->DuplexMode = FULLDUPLEX;
		gmacdev->Speed = SPEED100;
		TR("Link is up in 100M full-duplex mode\n");
	}
#endif
	return -ESYNOPGMACNOERR;
}

/**
 * Checks and initialze phy.
 * This function checks whether the phy initialization is complete.
 * @param[in] pointer to struct synop_device.
 * \return 0 if success else returns the error number.
 */

s32 synopGMAC_check_phy_init(struct synop_device *gmacdev)
{
	u16 data;
	s32 status = -ESYNOPGMACNOERR;
	s32 loop_count;

	loop_count = DEFAULT_LOOP_VARIABLE;
	while (loop_count-- > 0) {
		status = synopGMAC_read_phy_reg(
				(unsigned long *)gmacdev->MacBase,
				gmacdev->PhyBase,
				PHY_STATUS_REG, &data);

		if (status)
			return status;

		if ((data & Mii_AutoNegCmplt) != 0) {
			TR("Autonegotiation Complete\n");
			break;
		}
	}

	status = synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					PHY_SPECIFIC_STATUS_REG, &data);
	if (status)
		return status;

	if ((data & Mii_phy_status_link_up) == 0) {
		TR("No Link\n");
		gmacdev->LinkState = LINKDOWN;
		return -ESYNOPGMACPHYERR;
	}
	gmacdev->LinkState = LINKUP;
	TR("Link UP\n");

	status = synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					PHY_SPECIFIC_STATUS_REG, &data);
	if (status)
		return status;

	gmacdev->DuplexMode =
		(data & Mii_phy_status_full_duplex) ? FULLDUPLEX : HALFDUPLEX;
	TR("Link is up in %s mode\n",
	   (gmacdev->DuplexMode == FULLDUPLEX) ?
			"FULL DUPLEX" : "HALF DUPLEX");

	/* if not set to Master configuration in case of Half duplex mode*/
	/*set it manually as Master*/
	if (gmacdev->DuplexMode == HALFDUPLEX) {
		status = synopGMAC_read_phy_reg(
					(unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					PHY_CONTROL_REG, &data);

		if (status)
			return status;

		status = synopGMAC_write_phy_reg(
					(unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					PHY_CONTROL_REG,
					data | Mii_Manual_Master_Config);

		if (status)
			return status;
	}

	status = synopGMAC_read_phy_reg((unsigned long *)gmacdev->MacBase,
					gmacdev->PhyBase,
					PHY_SPECIFIC_STATUS_REG,
					&data);
	if (status)
		return status;

	if (data & Mii_phy_status_speed_1000)
		gmacdev->Speed = SPEED1000;
	else if (data & Mii_phy_status_speed_100)
		gmacdev->Speed = SPEED100;
	else
		gmacdev->Speed = SPEED10;

#ifdef DEBUG
	if (gmacdev->Speed == SPEED1000)
		TR("Link is with 1000M Speed\n");
	if (gmacdev->Speed == SPEED100)
		TR("Link is with 100M Speed\n");
	if (gmacdev->Speed == SPEED10)
		TR("Link is with 10M Speed\n");
#endif
	return -ESYNOPGMACNOERR;
}

/**
 * Sets the Mac address in to GMAC register.
 * This function sets the MAC address to the MAC register in question.
 * @param[in] pointer to struct synop_device
 * to populate mac dma and phy addresses.
 * @param[in] Register offset for Mac address high
 * @param[in] Register offset for Mac address low
 * @param[in] buffer containing mac address to be programmed.
 * \return 0 upon success. Error code upon failure.
 */
s32 synopGMAC_set_mac_addr(struct synop_device *gmacdev,
			   u32 MacHigh, u32 MacLow, u8 *MacAddr)
{
	u32 data;

	data = MacAddr[5];
	data = (data << 8) | MacAddr[4];
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase, MacHigh, data);

	data = MacAddr[3];
	data = (data << 8) | MacAddr[2];
	data = (data << 8) | MacAddr[1];
	data = (data << 8) | MacAddr[0];
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase, MacLow, data);
	return 0;
}

/**
* Get the Mac address in to the address specified.
* The mac register contents are read and written to buffer passed.
* @param[in] pointer to struct synop_device
* to populate mac dma and phy addresses.
* @param[in] Register offset for Mac address high
* @param[in] Register offset for Mac address low
* @param[out] buffer containing the device mac address.
* \return 0 upon success. Error code upon failure.
*/
s32 synopGMAC_get_mac_addr(struct synop_device *gmacdev,
			   u32 MacHigh, u32 MacLow, u8 *MacAddr)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase, MacHigh);
	MacAddr[5] = (u8)((data >> 8) & 0xff);
	MacAddr[4] = (u8)((data) & 0xff);

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase, MacLow);
	MacAddr[3] = (u8)((data >> 24) & 0xff);
	MacAddr[2] = (u8)((data >> 16) & 0xff);
	MacAddr[1] = (u8)((data >> 8) & 0xff);
	MacAddr[0] = (u8)((data) & 0xff);

	return 0;
}

/**
 * Attaches the synopGMAC device structure to the hardware.
 * Device structure is populated with MAC/DMA and PHY base addresses.
 * @param[in] pointer to struct synop_device
 * to populate mac dma and phy addresses.
 * @param[in] GMAC IP mac base address.
 * @param[in] GMAC IP dma base address.
 * @param[in] GMAC IP phy base address.
 * \return 0 upon success. Error code upon failure.
 * \note This is important function. No kernel api provided by Synopsys
 */

s32 synopGMAC_attach(struct synop_device *gmacdev,
		     u32 macBase, u32 dmaBase, u32 phyBase)
{
#ifdef USE_DEVICE_TREE
	struct device_node *node = NULL;
#endif
	/*Make sure the Device data strucure is cleared*/
	/*before we proceed further*/
	memset((void *)gmacdev, 0, sizeof(struct synop_device));
	/*Populate the mac and dma base addresses*/
	gmacdev->PhyBase = phyBase;

	gmacdev->rxDesc_invaild_count = 0;

#ifdef USE_DEVICE_TREE
	node = of_find_compatible_node(NULL, NULL, "nvt,synopsys-mac");
	if (node) {
		gmacdev->MacBase = of_iomap(node, 0);
		gmacdev->RmiiModeCtrlReg = gmacdev->MacBase;
		gmacdev->DmaBase = of_iomap(node, 1);
		gmacdev->GpioMuxReg = of_iomap(node, 2);
		gmacdev->RxPadinReg = of_iomap(node, 3);
		gmacdev->ClockCtrlReg = gmacdev->RxPadinReg + 0x4;
		gmacdev->IRQNum = irq_of_parse_and_map(node, 0);
	} else {
		of_node_put(node);
		return -ENODEV;
	}
#endif

	return 0;
}

/**
 * Initialize the rx descriptors for ring or chain mode operation.
 * -Status field is initialized to 0.
 * -EndOfRing set for the last descriptor.
 * -buffer1 and buffer2 set to 0 for ring mode of operation. (note)
 * -data1 and data2 set to 0. (note)
 * @param[in] pointer to struct dma_desc structure.
 * @param[in] whether end of ring
 * \return void.
 * \note Initialization of the buffer1, buffer2,
 * data1,data2 and status are not done here.
 * This only initializes whether one wants to use this descriptor
 * in chain mode or ring mode. For chain mode of operation
 * the buffer2 and data2 are programmed before calling this function.
 */
void synopGMAC_rx_desc_init_ring(struct dma_desc *desc, bool last_ring_desc)
{
	desc->status = 0;
	desc->length = last_ring_desc ? RxDescEndOfRing : 0;
	desc->buffer1 = 0;
	desc->buffer2 = 0;
	desc->data1 = 0;
	desc->data2 = 0;
}

/**
* Initialize the tx descriptors for ring or chain mode operation.
*- Status field is initialized to 0.
*- EndOfRing set for the last descriptor.
*- buffer1 and buffer2 set to 0 for ring mode of operation. (note)
*- data1 and data2 set to 0. (note)
* @param[in] pointer to struct dma_desc structure.
* @param[in] whether end of ring
* \return void.
* \note Initialization of the buffer1, buffer2, data1,data2
* and status are not done here.
* This only initializes whether one wants to use this descriptor
* in chain mode or ring mode.
* For chain mode of operation the buffer2 and data2
* are programmed before calling this function.
*/
void synopGMAC_tx_desc_init_ring(struct dma_desc *desc, bool last_ring_desc)
{
#ifdef ENH_DESC
	desc->status = last_ring_desc ? TxDescEndOfRing : 0;
	desc->length = 0;
#else
	desc->length = last_ring_desc ? TxDescEndOfRing : 0;
#endif
	desc->buffer1 = 0;
	desc->buffer2 = 0;
	desc->data1 = 0;
	desc->data2 = 0;
}

/**
 * Initialize the rx descriptors for chain mode of operation.
 *- Status field is initialized to 0.
 *- EndOfRing set for the last descriptor.
 *- buffer1 and buffer2 set to 0.
 *- data1 and data2 set to 0.
 * @param[in] pointer to struct dma_desc structure.
 * @param[in] whether end of ring
 * \return void.
 */

void synopGMAC_rx_desc_init_chain(struct dma_desc *desc)
{
	desc->status = 0;
	desc->length = RxDescChain;
	desc->buffer1 = 0;
	desc->data1 = 0;
}

/**
 * Initialize the rx descriptors for chain mode of operation.
 *- Status field is initialized to 0.
 *- EndOfRing set for the last descriptor.
 *- buffer1 and buffer2 set to 0.
 *- data1 and data2 set to 0.
 * @param[in] pointer to struct dma_desc structure.
 * @param[in] whether end of ring
 * \return void.
 */
void synopGMAC_tx_desc_init_chain(struct dma_desc *desc)
{
#ifdef ENH_DESC
	desc->status = TxDescChain;
	desc->length = 0;
#else
	desc->length = TxDescChain;
#endif
	desc->buffer1 = 0;
	desc->data1 = 0;
}

s32 synopGMAC_init_tx_rx_desc_queue(struct synop_device *gmacdev)
{
	u32 i;

	for (i = 0; i < gmacdev->TxDescCount; i++) {
		synopGMAC_tx_desc_init_ring(gmacdev->TxDesc + i,
					    i == gmacdev->TxDescCount - 1);
	}
	TR("At line %d\n", __LINE__);
	for (i = 0; i < gmacdev->RxDescCount; i++) {
		synopGMAC_rx_desc_init_ring(gmacdev->RxDesc + i,
					    i == gmacdev->RxDescCount - 1);
	}

	gmacdev->TxNext = 0;
	gmacdev->TxBusy = 0;
	gmacdev->RxNext = 0;
	gmacdev->RxBusy = 0;

	return -ESYNOPGMACNOERR;
}

/**
 * Programs the DmaRxBaseAddress with the Rx descriptor base address.
 * Rx Descriptor's base address is available in the gmacdev structure.
 * This function progrms the
 * Dma Rx Base address with the starting address
 * of the descriptor ring or chain.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_init_rx_desc_base(struct synop_device *gmacdev)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
			  DmaRxBaseAddr, (u32)gmacdev->RxDescDma);
}

/**
 * Programs the DmaTxBaseAddress with the Tx descriptor base address.
 * Tx Descriptor's base address is available in the gmacdev structure.
 * This function progrms the
 * Dma Tx Base address with the starting address
 * of the descriptor ring or chain.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_init_tx_desc_base(struct synop_device *gmacdev)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
			  DmaTxBaseAddr,
			(u32)gmacdev->TxDescDma);
}

/**
 * Makes the Dma as owner for this descriptor.
 * This function sets the own bit of status field of the DMA descriptor,
 * indicating the DMA is the owner for this descriptor.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns void.
 */
void synopGMAC_set_owner_dma(struct dma_desc *desc)
{
	desc->status |=  DescOwnByDma;
}

/**
 * set tx descriptor to indicate SOF.
 * This Descriptor contains the start of ethernet frame.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns void.
 */
void synopGMAC_set_desc_sof(struct dma_desc *desc)
{
#ifdef ENH_DESC
	desc->status |= DescTxFirst; /*ENH_DESC*/
#else
	desc->length |= DescTxFirst;
#endif
}

/**
 * set tx descriptor to indicate EOF.
 * This descriptor contains the End of ethernet frame.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns void.
 */
void synopGMAC_set_desc_eof(struct dma_desc *desc)
{
#ifdef ENH_DESC
	desc->status |= DescTxLast; /*ENH_DESC*/
#else
	desc->length |= DescTxLast;
#endif
}

/**
 * checks whether this descriptor contains start of frame.
 * This function is to check whether the descriptor's data buffer
 * contains a fresh ethernet frame?
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if SOF in current descriptor, else returns fail.
 */
bool synopGMAC_is_sof_in_rx_desc(struct dma_desc *desc)
{
	return ((desc->status & DescRxFirst) == DescRxFirst);
}

/**
 * checks whether this descriptor contains end of frame.
 * This function is to check whether the descriptor's data buffer
 * contains end of ethernet frame?
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if SOF in current descriptor, else returns fail.
 */
bool synopGMAC_is_eof_in_rx_desc(struct dma_desc *desc)
{
	return ((desc->status & DescRxLast) == DescRxLast);
}

/**
 * checks whether destination address filter failed in the rx frame.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if Failed, false if not.
 */
bool synopGMAC_is_da_filter_failed(struct dma_desc *desc)
{
	return ((desc->status & DescDAFilterFail) == DescDAFilterFail);
}

/**
 * checks whether source address filter failed in the rx frame.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if Failed, false if not.
 */
bool synopGMAC_is_sa_filter_failed(struct dma_desc *desc)
{
	return ((desc->status & DescSAFilterFail) == DescSAFilterFail);
}

/**
 * Checks whether the descriptor is owned by DMA.
 * If descriptor is owned by DMA then the OWN bit is set to 1.
 * This API is same for both ring and chain mode.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if Dma owns descriptor and false if not.
 */
bool synopGMAC_is_desc_owned_by_dma(struct dma_desc *desc)
{
	if (!desc) {
		pr_crit("%s: desc is NULL pointer !\n", __func__);
		return -1;
	}
	return ((desc->status & DescOwnByDma) == DescOwnByDma);
}

/**
 * returns the byte length of received frame including CRC.
 * This returns the no of bytes received
 * in the received ethernet frame including CRC(FCS).
 * @param[in] pointer to struct dma_desc structure.
 * \return returns the length of received frame lengths in bytes.
 */
u32 synopGMAC_get_rx_desc_frame_length(u32 status)
{
	return ((status & DescFrameLengthMask) >> DescFrameLengthShift);
}

/**
 * Checks whether the descriptor is valid
 * if no errors such as CRC/Receive Error/Watchdog
 * Timeout/Late collision/Giant Frame/Overflow/Descriptor
 * error the descritpor is said to be a valid descriptor.
 * @param[in] pointer to struct dma_desc structure.
 * \return True if desc valid. false if error.
 */
bool synopGMAC_is_desc_valid(unsigned long status)
{
	return ((status & DescError) == 0);
}

/**
 * Checks whether the descriptor is empty.
 * If the buffer1 and buffer2 lengths
 * are zero in ring mode descriptor is empty.
 * In chain mode buffer2 length is 0 but buffer2 itself
 * contains the next descriptor address.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if descriptor is empty, false if not empty.
 */
bool synopGMAC_is_desc_empty(struct dma_desc *desc)
{
	if (!desc) {
		pr_crit("%s: desc is NULL pointer !\n", __func__);
		return -1;
	}

	/*if both the buffer1 length and buffer2 length are zero desc is empty*/
	return(((desc->length & DescSize1Mask) == 0) &&
	       ((desc->length  & DescSize2Mask) == 0));
}

/**
 * Checks whether the rx descriptor is valid.
 * if rx descripor is not in error and complete frame is available
 * in the same descriptor
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if no error and first and last desc bits are set,
 * otherwise it returns false.
 */
bool synopGMAC_is_rx_desc_valid(u32 status)
{
	return ((status & DescError) == 0) &&
		((status & DescRxFirst) == DescRxFirst) &&
		((status & DescRxLast) == DescRxLast);
}

/**
 * Checks whether the tx is aborted due to collisions.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if collisions, else returns false.
 */
bool synopGMAC_is_tx_aborted(u32 status)
{
	return (
			((status & DescTxLateCollision) ==
			DescTxLateCollision) |
			((status & DescTxExcCollisions) ==
			DescTxExcCollisions));
}

/**
 * Checks whether the tx carrier error.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if carrier error occurred, else returns falser.
 */
bool synopGMAC_is_tx_carrier_error(u32 status)
{
	return (((status & DescTxLostCarrier) == DescTxLostCarrier)
			| ((status & DescTxNoCarrier) == DescTxNoCarrier));
}

/**
 * Gives the transmission collision count.
 * returns the transmission collision count indicating number
 * of collisions occurred before the frame was transmitted.
 * Make sure to check excessive collision didnot happen
 * to ensure the count is valid.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns the count value of collision.
 */
u32 synopGMAC_get_tx_collision_count(u32 status)
{
	return ((status & DescTxCollMask) >> DescTxCollShift);
}

u32 synopGMAC_is_exc_tx_collisions(u32 status)
{
	return ((status & DescTxExcCollisions) == DescTxExcCollisions);
}

/**
 * Check for damaged frame due to overflow or collision.
 * Retruns true if rx frame was damaged due
 * to buffer overflow in MTL or late collision in half duplex mode.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if error else returns false.
 */
bool synopGMAC_is_rx_frame_damaged(u32 status)
{
	return (((status & DescRxDamaged) == DescRxDamaged)
			| ((status & DescRxCollision) == DescRxCollision));
}

/**
 * Check for damaged frame due to collision.
 * Retruns true if rx frame was damaged due
 * to late collision in half duplex mode.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if error else returns false.
 */
bool synopGMAC_is_rx_frame_collision(u32 status)
{
	return ((status & DescRxCollision) == DescRxCollision);
}

/**
 * Check for receive CRC error.
 * Retruns true if rx frame CRC error occurred.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if error else returns false.
 */
bool synopGMAC_is_rx_crc(u32 status)
{
	return ((status & DescRxCrc) == DescRxCrc);
}

/**
 * Indicates rx frame has non integer multiple of bytes. (odd nibbles).
 * Retruns true if dribbling error in rx frame.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if error else returns false.
 */
bool synopGMAC_is_frame_dribbling_errors(u32 status)
{
	return ((status & DescRxDribbling) == DescRxDribbling);
}

/**
 * Indicates error in rx frame length.
 * Retruns true if received frame length doesnot
 * match with the length field
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if error else returns false.
 */
bool synopGMAC_is_rx_frame_length_errors(u32 status)
{
	return((status & DescRxLengthError) == DescRxLengthError);
}

/**
 * Checks whether this rx descriptor is last rx descriptor.
 * This returns true if it is last descriptor
 * either in ring mode or in chain mode.
 * @param[in] pointer to devic structure.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if it is last descriptor, false if not.
 * \note This function should not be called
 * before initializing the descriptor using synopGMAC_desc_init().
 */
bool synopGMAC_is_last_rx_desc(
	struct synop_device *gmacdev, struct dma_desc *desc)
{
	return (((desc->length & RxDescEndOfRing) == RxDescEndOfRing) ||
		((unsigned long)gmacdev->RxDesc == desc->data2));
}

/**
 * Checks whether this tx descriptor is last tx descriptor.
 * This returns true if it is last descriptor
 * either in ring mode or in chain mode.
 * @param[in] pointer to devic structure.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if it is last descriptor, false if not.
 * \note This function should not be called
 * before initializing the descriptor using synopGMAC_desc_init().
 */
bool synopGMAC_is_last_tx_desc(
	struct synop_device *gmacdev, struct dma_desc *desc)
{
#ifdef ENH_DESC
	return (((desc->status & TxDescEndOfRing) == TxDescEndOfRing) ||
		((u32)gmacdev->TxDesc == desc->data2));
#else
	return (((desc->length & TxDescEndOfRing) == TxDescEndOfRing) ||
		((unsigned long)gmacdev->TxDesc == desc->data2));
#endif
}

/**
 * Checks whether this rx descriptor is in chain mode.
 * This returns true if it is this descriptor is in chain mode.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if chain mode is set, false if not.
 */
bool synopGMAC_is_rx_desc_chained(struct dma_desc *desc)
{
	return((desc->length & RxDescChain) == RxDescChain);
}

/**
 * Checks whether this tx descriptor is in chain mode.
 * This returns true if it is this descriptor is in chain mode.
 * @param[in] pointer to struct dma_desc structure.
 * \return returns true if chain mode is set, false if not.
 */
bool synopGMAC_is_tx_desc_chained(struct dma_desc *desc)
{
#ifdef ENH_DESC
	return((desc->status & TxDescChain) == TxDescChain);
#else
	return((desc->length & TxDescChain) == TxDescChain);
#endif
}

/**
 * Driver Api to get the descriptor field information.
 * This returns the status, dma-able address of
 * buffer1, the length of buffer1, virtual address of buffer1
 * dma-able address of buffer2, length of buffer2
 * virtural adddress of buffer2.
 * @param[in]  pointer to struct dma_desc structure.
 * @param[out] pointer to status field fo descriptor.
 * @param[out] dma-able address of buffer1.
 * @param[out] length of buffer1.
 * @param[out] virtual address of buffer1.
 * @param[out] dma-able address of buffer2.
 * @param[out] length of buffer2.
 * @param[out] virtual address of buffer2.
 * \return returns void.
 */
void synopGMAC_get_desc_data(struct dma_desc *desc,
			     u32 *Status,
			     u32 *Buffer1, u32 *Length1,
			     unsigned long *Data1,
			     u32 *Buffer2, u32 *Length2,
			     unsigned long *Data2)
{
	if (Status != 0)
		*Status = desc->status;

	if (Buffer1 != 0)
		*Buffer1 = desc->buffer1;
	if (Length1 != 0)
		*Length1 = (desc->length & DescSize1Mask) >> DescSize1Shift;
	if (Data1 != 0)
		*Data1 = desc->data1;

	if (Buffer2 != 0)
		*Buffer2 = desc->buffer2;
	if (Length2 != 0)
		*Length2 = (desc->length & DescSize2Mask) >> DescSize2Shift;
	if (Data1 != 0)
		*Data2 = desc->data2;
}

#ifdef ENH_DESC_8W
/**
 * This function is defined two times.
 * Once when the code is compiled
 * for ENHANCED DESCRIPTOR SUPPORT and Once for Normal descriptor
 * Get the index and address of Tx desc.
 * This api is same for both ring mode and chain mode.
 * This function tracks the tx descriptor
 * the DMA just closed after the transmission
 * of data from this descriptor is
 * over. This returns the descriptor fields to the caller.
 * @param[in] pointer to struct synop_device.
 * @param[out] status field of the descriptor.
 * @param[out] Dma-able buffer1 pointer.
 * @param[out] length of buffer1 (Max is 2048).
 * @param[out] virtual pointer for buffer1.
 * @param[out] Dma-able buffer2 pointer.
 * @param[out] length of buffer2 (Max is 2048).
 * @param[out] virtual pointer for buffer2.
 * @param[out] u32 data indicating whether
 * the descriptor is in ring mode or chain mode.
 * \return returns present tx descriptor index on success.
 * Negative value if error.
 */
s8 synopGMAC_get_tx_qptr(struct synop_device *gmacdev,
			 u32 *Status, u32 *Buffer1, u32 *Length1, u32 *Data1,
		u32 *Buffer2, u32 *Length2, u32 *Data2,
		u32 *Ext_Status, u32 *Time_Stamp_High, u32 *Time_Stamp_Low,
		u32 *now_desc)
{
	u32  txover      = gmacdev->TxBusy;
	struct dma_desc *txdesc = gmacdev->TxBusyDesc;

	if (synopGMAC_is_desc_owned_by_dma(txdesc))
		return -1;
	if (synopGMAC_is_desc_empty(txdesc))
		return -1;

	/*busy tx descriptor is reduced by one*/
	/*as it will be handed over to Processor now*/
	(gmacdev->BusyTxDesc)--;
	if (Status != 0)
		*Status = txdesc->status;

	if (Ext_Status != 0)
		*Ext_Status = txdesc->extstatus;
	if (Time_Stamp_High != 0)
		*Time_Stamp_High = txdesc->timestamphigh;
	if (Time_Stamp_Low != 0)
		*Time_Stamp_High = txdesc->timestamplow;

	if (Buffer1 != 0)
		*Buffer1 = txdesc->buffer1;
	if (Length1 != 0)
		*Length1 = (txdesc->length & DescSize1Mask) >> DescSize1Shift;
	if (Data1 != 0)
		*Data1 = txdesc->data1;

	if (Buffer2 != 0)
		*Buffer2 = txdesc->buffer2;
	if (Length2 != 0)
		*Length2 = (txdesc->length & DescSize2Mask) >> DescSize2Shift;
	if (Data1 != 0)
		*Data2 = txdesc->data2;

	gmacdev->TxBusy =
		synopGMAC_is_last_tx_desc(gmacdev, txdesc) ? 0 : txover + 1;

	if (synopGMAC_is_tx_desc_chained(txdesc)) {
		gmacdev->TxBusyDesc = (struct dma_desc *)txdesc->data2;
		synopGMAC_tx_desc_init_chain(txdesc);
	} else {
		gmacdev->TxBusyDesc =
			synopGMAC_is_last_tx_desc(gmacdev, txdesc)
			? gmacdev->TxDesc : (txdesc + 1);
		synopGMAC_tx_desc_init_ring(txdesc,
					    synopGMAC_is_last_tx_desc(gmacdev,
								      txdesc));
	}
	TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",
	   txover, (u32)txdesc, txdesc->status, txdesc->length,
			txdesc->buffer1, txdesc->buffer2, txdesc->data1,
			txdesc->data2);

	*now_desc = txover;
	return 0;
}
#else

/**
 * Get the index and address of Tx desc.
 * This api is same for both ring mode and chain mode.
 * This function tracks the tx descriptor the
 * DMA just closed after the transmission of data from this descriptor is
 * over. This returns the descriptor fields to the caller.
 * @param[in] pointer to struct synop_device.
 * @param[out] status field of the descriptor.
 * @param[out] Dma-able buffer1 pointer.
 * @param[out] length of buffer1 (Max is 2048).
 * @param[out] virtual pointer for buffer1.
 * @param[out] Dma-able buffer2 pointer.
 * @param[out] length of buffer2 (Max is 2048).
 * @param[out] virtual pointer for buffer2.
 * @param[out] u32 data indicating
 * whether the descriptor is in ring mode or chain mode.
 * \return returns present tx descriptor index on success.
 * Negative value if error.
 */
s8 synopGMAC_get_tx_qptr(
		struct synop_device *gmacdev,
		u32 *Status,
		u32 *Buffer1, u32 *Length1,
		unsigned long *Data1,
		u32 *Buffer2, u32 *Length2,
		unsigned long *Data2,
		unsigned long *now_desc)
{
	u32  txover      = gmacdev->TxBusy;
	struct dma_desc *txdesc = gmacdev->TxBusyDesc;

	if (!txdesc) {
		pr_crit("%s: txdesc is NULL\n", __func__);
		return -1;
	}

	/*do cache sync, dram sync to cpu cache*/
	dma_sync_single_for_cpu(NULL,
				virt_to_phys(txdesc),
			sizeof(struct dma_desc),
			DMA_FROM_DEVICE);

	TR("==============Get TX descriptor 0x%x==============\n", (u32)txdesc);

	if (synopGMAC_is_desc_owned_by_dma(txdesc)) {
		TR("Get TX Qptr Fail at <<synopGMAC_is_desc_owned_by_dma>>\n");
		return -1;
	}
	if (synopGMAC_is_desc_empty(txdesc)) {
		TR("Get TX Qptr Fail at <<synopGMAC_is_desc_empty>>\n");
		return -1;
	}

	/*busy tx descriptor is reduced by one */
	/*as it will be handed over to Processor now*/
	(gmacdev->BusyTxDesc)--;

	if (Status != 0)
		*Status = txdesc->status;

	if (Buffer1 != 0)
		*Buffer1 = txdesc->buffer1;
	if (Length1 != 0) {
		*Length1 =
			(txdesc->length & DescSize1Mask) >> DescSize1Shift;
	}
	if (Data1 != 0)
		*Data1 = txdesc->data1;

	if (Buffer2 != 0)
		*Buffer2 = txdesc->buffer2;
	if (Length2 != 0) {
		*Length2 =
			(txdesc->length & DescSize2Mask) >> DescSize2Shift;
	}
	if (Data1 != 0)
		*Data2 = txdesc->data2;

	gmacdev->TxBusy =
		synopGMAC_is_last_tx_desc(gmacdev, txdesc) ? 0 : txover + 1;

	if (synopGMAC_is_tx_desc_chained(txdesc)) {
		gmacdev->TxBusyDesc = (struct dma_desc *)txdesc->data2;
		synopGMAC_tx_desc_init_chain(txdesc);
	} else {
		gmacdev->TxBusyDesc =
			synopGMAC_is_last_tx_desc(gmacdev, txdesc)
			? gmacdev->TxDesc : (txdesc + 1);
		synopGMAC_tx_desc_init_ring(txdesc,
					    synopGMAC_is_last_tx_desc(gmacdev,
								      txdesc));
	}

	TR("Get TX Qptr : %02d %08x %08x %08x %08x %08x %08x %08x\n",
	   txover, (u32)txdesc, txdesc->status, txdesc->length,
			txdesc->buffer1, txdesc->buffer2, txdesc->data1,
			txdesc->data2);

	/*do cache sync, cache sync to dram*/
	dma_sync_single_for_device(
			NULL,
			virt_to_phys(txdesc),
			sizeof(struct dma_desc),
			DMA_TO_DEVICE);

	*now_desc = txover;
	return 0;
}
#endif

/**
 * Populate the tx desc structure with the buffer address.
 * Once the driver has a packet ready to be transmitted,
 * this function is called with the
 * valid dma-able buffer addresses and their lengths.
 * This function populates the descriptor
 * and make the DMA the owner for the descriptor.
 * This function also controls whetther Checksum
 * offloading to be done in hardware or not.
 * This api is same for both ring mode and chain mode.
 * @param[in] pointer to struct synop_device.
 * @param[in] Dma-able buffer1 pointer.
 * @param[in] length of buffer1 (Max is 2048).
 * @param[in] virtual pointer for buffer1.
 * @param[in] Dma-able buffer2 pointer.
 * @param[in] length of buffer2 (Max is 2048).
 * @param[in] virtual pointer for buffer2.
 * @param[in] u32 data indicating
 * whether the descriptor is in ring mode or chain mode.
 * @param[in] u32 indicating
 * whether the checksum offloading in HW/SW.
 * \return returns present tx descriptor index on success.
 * Negative value if error.
 */
s8 synopGMAC_set_tx_qptr(
		struct synop_device *gmacdev,
		u32 Buffer1, u32 Length1,
		unsigned long Data1,
		u32 Buffer2, u32 Length2,
		unsigned long Data2,
		u32 offload_needed,
		unsigned long *now_desc)
{
	u32  txnext      = gmacdev->TxNext;
	struct dma_desc *txdesc = gmacdev->TxNextDesc;

	if (!txdesc) {
		pr_crit("%s: desc is NULL pointer !\n", __func__);
		return -1;
	}

	dma_sync_single_for_cpu(
			NULL,
			virt_to_phys(txdesc),
			sizeof(struct dma_desc),
			DMA_FROM_DEVICE);

	if (!synopGMAC_is_desc_empty(txdesc))
		return -1;

	/*busy tx descriptor is reduced by one*/
	/*as it will be handed over to Processor now*/
	(gmacdev->BusyTxDesc)++;

	if (synopGMAC_is_tx_desc_chained(txdesc)) {
		txdesc->length |= ((Length1 << DescSize1Shift) & DescSize1Mask);
#ifdef ENH_DESC
		txdesc->status |=  (DescTxFirst | DescTxLast | DescTxIntEnable);
#else
		txdesc->length |=  (DescTxFirst | DescTxLast | DescTxIntEnable);
#endif

		txdesc->buffer1 = Buffer1;
		txdesc->data1 = Data1;

		if (offload_needed) {
			/* Comment
			 * Make sure that the OS you are running supports
			 * the IP and TCP checkusm offloaidng,
			 * before calling any of the functions given below.
			 */
			synopGMAC_tx_checksum_offload_ipv4hdr(gmacdev, txdesc);
			synopGMAC_tx_checksum_offload_tcponly(gmacdev, txdesc);
		}
#ifdef ENH_DESC
		txdesc->status |= DescOwnByDma;
#else
		txdesc->status = DescOwnByDma;
#endif

		gmacdev->TxNext =
			synopGMAC_is_last_tx_desc(gmacdev, txdesc) ?
			0 : txnext + 1;

		gmacdev->TxNextDesc = (struct dma_desc *)txdesc->data2;
	} else {
		txdesc->length |=
			(((Length1 << DescSize1Shift) & DescSize1Mask) |
			 ((Length2 << DescSize2Shift) & DescSize2Mask));
#ifdef ENH_DESC
		txdesc->status |=  (DescTxFirst | DescTxLast | DescTxIntEnable);
#else
		/* Its always assumed
		 * that complete data will fit in to one descriptor
		 */
		txdesc->length |=  (DescTxFirst | DescTxLast | DescTxIntEnable);
#endif

		txdesc->buffer1 = Buffer1;
		txdesc->data1 = Data1;

		txdesc->buffer2 = Buffer2;
		txdesc->data2 = Data2;

		if (offload_needed) {
			/* Make sure that the OS you are running supports the IP
			 * and TCP checkusm offloaidng,
			 * before calling any of the functions given below.
			 */
			synopGMAC_tx_checksum_offload_ipv4hdr(gmacdev, txdesc);
			synopGMAC_tx_checksum_offload_tcponly(gmacdev, txdesc);
		}
#ifdef ENH_DESC
		txdesc->status |= DescOwnByDma;
#else
		txdesc->status = DescOwnByDma;
#endif

		gmacdev->TxNext =
			synopGMAC_is_last_tx_desc(gmacdev, txdesc) ?
			0 : txnext + 1;
		gmacdev->TxNextDesc =
			synopGMAC_is_last_tx_desc(gmacdev, txdesc) ?
			gmacdev->TxDesc : (txdesc + 1);
	}

	/*do cache sync, cpu cache sync to dram*/
	dma_sync_single_for_device(
			NULL,
			Buffer1,
			Length1,
			DMA_TO_DEVICE);

	dma_sync_single_for_device(
			NULL,
			virt_to_phys(txdesc),
			sizeof(struct dma_desc),
			DMA_TO_DEVICE);

	TR("Set TX Qptr : %02d %08x %08x %08x %08x %08x %08x %08x\n",
	   txnext, (u32)txdesc, txdesc->status, txdesc->length,
			txdesc->buffer1, txdesc->buffer2,
			txdesc->data1, txdesc->data2);

	*now_desc = txnext;

	return 0;
}

#ifdef ENH_DESC_8W
/**
 * Prepares the descriptor to receive packets.
 * The descriptor is allocated with the valid buffer addresses
 * (sk_buff address)and the length fields
 * and handed over to DMA by setting the ownership.
 * After successful return from this function the
 * descriptor is added to the receive descriptor pool/queue.
 * This api is same for both ring mode and chain mode.
 * @param[in] pointer to struct synop_device.
 * @param[in] Dma-able buffer1 pointer.
 * @param[in] length of buffer1 (Max is 2048).
 * @param[in] Dma-able buffer2 pointer.
 * @param[in] length of buffer2 (Max is 2048).
 * @param[in] u32 data indicating
 * whether the descriptor is in ring mode or chain mode.
 * \return returns present rx descriptor index on success.
 * Negative value if error.
 */
s8 synopGMAC_set_rx_qptr(struct synop_device *gmacdev,
		u32 Buffer1, u32 Length1, u32 Data1,
		u32 Buffer2, u32 Length2, u32 Data2,
		u32 *now_desc)
{
	u32  rxnext      = gmacdev->RxNext;
	struct dma_desc *rxdesc = gmacdev->RxNextDesc;

	if (!synopGMAC_is_desc_empty(rxdesc))
		return -1;

	if (synopGMAC_is_rx_desc_chained(rxdesc)) {
		rxdesc->length |= ((Length1 << DescSize1Shift) & DescSize1Mask);

		rxdesc->buffer1 = Buffer1;
		rxdesc->data1 = Data1;

		rxdesc->extstatus = 0;
		rxdesc->reserved1 = 0;
		rxdesc->timestamplow = 0;
		rxdesc->timestamphigh = 0;

#if (MODULO_INTERRUPT > 1)
		if ((rxnext % MODULO_INTERRUPT) != 0)
			rxdesc->length |= RxDisIntCompl;
#endif

		rxdesc->status = DescOwnByDma;

		gmacdev->RxNext =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			0 : rxnext + 1;

		gmacdev->RxNextDesc = (struct dma_desc *)rxdesc->data2;
	} else {
		rxdesc->length |=
			(((Length1 << DescSize1Shift) & DescSize1Mask) |
			 ((Length2 << DescSize2Shift) & DescSize2Mask));

		rxdesc->buffer1 = Buffer1;
		rxdesc->data1 = Data1;

		rxdesc->extstatus = 0;
		rxdesc->reserved1 = 0;
		rxdesc->timestamplow = 0;
		rxdesc->timestamphigh = 0;

		rxdesc->buffer2 = Buffer2;
		rxdesc->data2 = Data2;

#if (MODULO_INTERRUPT > 1)
		if ((rxnext % MODULO_INTERRUPT) != 0)
			rxdesc->length |= RxDisIntCompl;
#endif

		rxdesc->status = DescOwnByDma;

		gmacdev->RxNext =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			0 : rxnext + 1;

		gmacdev->RxNextDesc =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			gmacdev->RxDesc : (rxdesc + 1);
	}
	TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",
	   rxnext, (u32)rxdesc, rxdesc->status, rxdesc->length,
			rxdesc->buffer1, rxdesc->buffer2, rxdesc->data1,
			rxdesc->data2);

	/*One descriptor will be given to Hardware.
	 * So busy count incremented by one
	 */
	(gmacdev->BusyRxDesc)++;
	*now_desc = rxnext;
	return 0;
}

#else
/**
 * Prepares the descriptor to receive packets.
 * The descriptor is allocated with the valid buffer addresses
 * (sk_buff address) and the length fields
 * and handed over to DMA by setting the ownership.
 * After successful return from this function the
 * descriptor is added to the receive descriptor pool/queue.
 * This api is same for both ring mode and chain mode.
 * @param[in] pointer to struct synop_device.
 * @param[in] Dma-able buffer1 pointer.
 * @param[in] length of buffer1 (Max is 2048).
 * @param[in] Dma-able buffer2 pointer.
 * @param[in] length of buffer2 (Max is 2048).
 * @param[in] u32 data indicating
 * whether the descriptor is in ring mode or chain mode.
 * \return returns present rx descriptor index on success.
 * Negative value if error.
 */
s8 synopGMAC_set_rx_qptr(struct synop_device *gmacdev,
			u32 Buffer1, u32 Length1,
			unsigned long Data1,
			u32 Buffer2, u32 Length2,
			unsigned long Data2,
			unsigned long *now_desc)
{
	u32  rxnext      = gmacdev->RxNext;
	struct dma_desc *rxdesc = gmacdev->RxNextDesc;

	if (!rxdesc) {
		pr_crit("%s: desc is NULL pointer !\n", __func__);
		return -1;
	}

	dma_sync_single_for_cpu(
			NULL,
			virt_to_phys(rxdesc),
			sizeof(struct dma_desc),
			DMA_FROM_DEVICE);

	if (!synopGMAC_is_desc_empty(rxdesc))
		return -1;

	dma_sync_single_for_device(
			NULL,
			Buffer1,
			Length1,
			DMA_TO_DEVICE);

	if (synopGMAC_is_rx_desc_chained(rxdesc)) {
		rxdesc->length |= ((Length1 << DescSize1Shift) & DescSize1Mask);

		rxdesc->buffer1 = Buffer1;
		rxdesc->data1 = Data1;

#if (MODULO_INTERRUPT > 1)
		if ((rxnext % MODULO_INTERRUPT) != 0)
			rxdesc->length |= RxDisIntCompl;
#endif

		rxdesc->status = DescOwnByDma;

		gmacdev->RxNext =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			0 : rxnext + 1;

		gmacdev->RxNextDesc = (struct dma_desc *)rxdesc->data2;
	} else {
		rxdesc->length |=
			(((Length1 << DescSize1Shift) & DescSize1Mask) |
			 ((Length2 << DescSize2Shift) & DescSize2Mask));

		rxdesc->buffer1 = Buffer1;
		rxdesc->data1 = Data1;

		rxdesc->buffer2 = Buffer2;
		rxdesc->data2 = Data2;

#if (MODULO_INTERRUPT > 1)
		if ((rxnext % MODULO_INTERRUPT) != 0)
			rxdesc->length |= RxDisIntCompl;
#endif

		rxdesc->status = DescOwnByDma;

		gmacdev->RxNext =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			0 : rxnext + 1;

		gmacdev->RxNextDesc =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			gmacdev->RxDesc : (rxdesc + 1);
	}

	/*do cache sync, cpu cache sync to dram*/
	dma_sync_single_for_device(
			NULL,
			virt_to_phys(rxdesc),
			sizeof(struct dma_desc),
			DMA_TO_DEVICE);

	TR("Set Rx Qptr %02d %08x %08x %08x %08x %08x %08x %08x\n",
	   rxnext, (u32)rxdesc, rxdesc->status, rxdesc->length,
			rxdesc->buffer1, rxdesc->buffer2,
			rxdesc->data1, rxdesc->data2);
	/*One descriptor will be given to Hardware.
	 * So busy count incremented by one
	 */
	(gmacdev->BusyRxDesc)++;
	*now_desc = rxnext;
	return 0;
}

#endif
#ifdef ENH_DESC_8W
/**
 * This function is defined two times.
 * Once when the code is compiled for ENHANCED DESCRIPTOR SUPPORT
 * and Once for Normal descriptor
 * Get back the descriptor from DMA after data has been received.
 * When the DMA indicates that the data is received (interrupt is generated),
 * this function should be
 * called to get the descriptor and hence the data buffers received.
 * With successful return from this
 * function caller gets the descriptor fields for processing.
 * check the parameters to understand the
 * fields returned.`
 * @param[in] pointer to struct synop_device.
 * @param[out] pointer to hold the status of DMA.
 * @param[out] Dma-able buffer1 pointer.
 * @param[out] pointer to hold length of buffer1 (Max is 2048).
 * @param[out] virtual pointer for buffer1.
 * @param[out] Dma-able buffer2 pointer.
 * @param[out] pointer to hold length of buffer2 (Max is 2048).
 * @param[out] virtual pointer for buffer2.
 * \return returns present rx descriptor index on success.
 * Negative value if error.
 */
s8 synopGMAC_get_rx_qptr(struct synop_device *gmacdev,
			 u32 *Status, u32 *Buffer1, u32 *Length1, u32 *Data1,
		u32 *Buffer2, u32 *Length2, u32 *Data2,
		u32 *Ext_Status, u32 *Time_Stamp_High,
		u32 *Time_Stamp_Low, u32 *now_desc)
{
	/*index of descriptor the DMA just
	 * completed. May be useful when data
	 */
	u32 rxnext = gmacdev->RxBusy;
	/*is spread over multiple buffers/descriptors*/
	struct dma_desc *rxdesc = gmacdev->RxBusyDesc;

	if (synopGMAC_is_desc_owned_by_dma(rxdesc))
		return -1;
	if (synopGMAC_is_desc_empty(rxdesc))
		return -1;

	if (Status != 0)
		*Status = rxdesc->status; /*send the status of this descriptor*/

	if (Ext_Status != 0)
		*Ext_Status = rxdesc->extstatus;
	if (Time_Stamp_High != 0)
		*Time_Stamp_High = rxdesc->timestamphigh;
	if (Time_Stamp_Low != 0)
		*Time_Stamp_Low = rxdesc->timestamplow;

	if (Length1 != 0)
		*Length1 = (rxdesc->length & DescSize1Mask) >> DescSize1Shift;
	if (Buffer1 != 0)
		*Buffer1 = rxdesc->buffer1;
	if (Data1 != 0)
		*Data1 = rxdesc->data1;

	if (Length2 != 0)
		*Length2 = (rxdesc->length & DescSize2Mask) >> DescSize2Shift;
	if (Buffer2 != 0)
		*Buffer2 = rxdesc->buffer2;
	if (Data1 != 0)
		*Data2 = rxdesc->data2;

	gmacdev->RxBusy =
		synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
		0 : rxnext + 1;

	if (synopGMAC_is_rx_desc_chained(rxdesc)) {
		gmacdev->RxBusyDesc = (struct dma_desc *)rxdesc->data2;
		synopGMAC_rx_desc_init_chain(rxdesc);
	} else {
		gmacdev->RxBusyDesc =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			gmacdev->RxDesc : (rxdesc + 1);
		synopGMAC_rx_desc_init_ring(
				rxdesc,
				synopGMAC_is_last_rx_desc(gmacdev, rxdesc));
	}
	TR("%02d %08x %08x %08x %08x %08x %08x %08x\n",
	   rxnext, (u32)rxdesc, rxdesc->status, rxdesc->length,
			rxdesc->buffer1, rxdesc->buffer2, rxdesc->data1,
			rxdesc->data2);

	/*busy tx descriptor is reduced
	 * by one as it will be handed over to Processor now
	 */
	(gmacdev->BusyRxDesc)--;

	*now_desc = rxnext;
	return 0;
}
#else

/**
 * Get back the descriptor from DMA after data has been received.
 * When the DMA indicates that the data
 * is received (interrupt is generated), this function should be
 * called to get the descriptor and hence the data buffers received.
 * With successful return from this
 * function caller gets the descriptor fields for processing.
 * check the parameters to understand the
 * fields returned.`
 * @param[in] pointer to struct synop_device.
 * @param[out] pointer to hold the status of DMA.
 * @param[out] Dma-able buffer1 pointer.
 * @param[out] pointer to hold length of buffer1 (Max is 2048).
 * @param[out] virtual pointer for buffer1.
 * @param[out] Dma-able buffer2 pointer.
 * @param[out] pointer to hold length of buffer2 (Max is 2048).
 * @param[out] virtual pointer for buffer2.
 * \return returns present rx descriptor index on success.
 * Negative value if error.
 */
s8 synopGMAC_get_rx_qptr(struct synop_device *gmacdev,
			u32 *Status,
			u32 *Buffer1, u32 *Length1,
			unsigned long *Data1,
			u32 *Buffer2, u32 *Length2,
			unsigned long *Data2,
			unsigned long *now_desc)
{
	/*index of descriptor the DMA just completed. May be useful when data*/
	u32 rxnext       = gmacdev->RxBusy;
	/*is spread over multiple buffers/descriptors*/
	struct dma_desc *rxdesc = gmacdev->RxBusyDesc;

	if (!rxdesc) {
		pr_crit("%s: rxdesc is NULL\n", __func__);
		return -1;
	}

	/*do cache sync, dram sync to cpu cache*/
	dma_sync_single_for_cpu(
			NULL,
			virt_to_phys(rxdesc),
			sizeof(struct dma_desc),
			DMA_FROM_DEVICE);

	TR("==============Get RX descriptor 0x%x==============\n", (u32)rxdesc);

	if (synopGMAC_is_desc_owned_by_dma(rxdesc)) {
		TR("Get RX Qptr Fail at <<synopGMAC_is_desc_owned_by_dma>>\n");
		return -1;
	}
	if (synopGMAC_is_desc_empty(rxdesc)) {
		TR("Get RX Qptr Fail at <<synopGMAC_is_desc_empty>>\n");
		return -1;
	}

	if (Status) {
		/*send the status of this descriptor*/
		*Status = rxdesc->status;
	} else {
		pr_crit("%s,%d DESC =0x%p status is NULL!\n",
			__func__, rxnext, rxdesc);
	}

	if (Length1 != 0)
		*Length1 = (rxdesc->length & DescSize1Mask) >> DescSize1Shift;
	if (Buffer1 != 0)
		*Buffer1 = rxdesc->buffer1;
	if (Data1 != 0)
		*Data1 = rxdesc->data1;

	TR("jay hsu : debug Buffer1 0x%x, length %d\n",
	   rxdesc->buffer1,
			((rxdesc->length & DescSize1Mask) >> DescSize1Shift));
	/*do cache sync, dram sync to cpu cache*/
	dma_sync_single_for_cpu(
			NULL,
			rxdesc->buffer1,
			((rxdesc->length & DescSize1Mask) >> DescSize1Shift),
			DMA_FROM_DEVICE);

	if (Length2 != 0)
		*Length2 = (rxdesc->length & DescSize2Mask) >> DescSize2Shift;
	if (Buffer2 != 0)
		*Buffer2 = rxdesc->buffer2;
	if (Data1 != 0)
		*Data2 = rxdesc->data2;

	gmacdev->RxBusy =
		synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
		0 : rxnext + 1;

	if (synopGMAC_is_rx_desc_chained(rxdesc)) {
		gmacdev->RxBusyDesc = (struct dma_desc *)rxdesc->data2;
		synopGMAC_rx_desc_init_chain(rxdesc);
	} else {
		gmacdev->RxBusyDesc =
			synopGMAC_is_last_rx_desc(gmacdev, rxdesc) ?
			gmacdev->RxDesc : (rxdesc + 1);
		synopGMAC_rx_desc_init_ring(rxdesc,
					    synopGMAC_is_last_rx_desc(gmacdev,
								      rxdesc));
	}
	dma_sync_single_for_device(
			NULL,
			virt_to_phys(rxdesc),
			sizeof(struct dma_desc),
			DMA_TO_DEVICE);

	TR("Get RX Qptr %02d %08x %08x %08x %08x %08x %08x %08x\n",
	   rxnext, (u32)rxdesc, rxdesc->status, rxdesc->length,
			rxdesc->buffer1, rxdesc->buffer2, rxdesc->data1,
			rxdesc->data2);
	/*This returns one descriptor to processor.
	 * So busy count will be decremented by one
	 */
	(gmacdev->BusyRxDesc)--;

	*now_desc = rxnext;
	return 0;
}

#endif

/**
 * Clears all the pending interrupts.
 * If the Dma status register is read then all the interrupts gets cleared
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_clear_interrupt(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase, DmaStatus);
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase, DmaStatus, data);
}

/**
 * Returns the all unmasked interrupt status after reading the DmaStatus register.
 * @param[in] pointer to struct synop_device.
 * \return 0 upon success. Error code upon failure.
 */
u32 synopGMAC_get_interrupt_type(struct synop_device *gmacdev)
{
	u32 data;
	u32 interrupts = 0;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase, DmaStatus);
	/* I think this is the appropriate location to clear the interrupts*/
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase, DmaStatus, data);
	TR("DMA status reg is %08x\n", data);
	if (data & DmaIntErrorMask)
		interrupts |= synopGMACDmaError;
	if (data & DmaIntRxNormMask)
		interrupts |= synopGMACDmaRxNormal;
	if (data & DmaIntRxAbnMask)
		interrupts |= synopGMACDmaRxAbnormal;
	if (data & DmaIntRxStoppedMask)
		interrupts |= synopGMACDmaRxStopped;
	if (data & DmaIntTxNormMask)
		interrupts |= synopGMACDmaTxNormal;
	if (data & DmaIntTxAbnMask)
		interrupts |= synopGMACDmaTxAbnormal;
	if (data & DmaIntTxStoppedMask)
		interrupts |= synopGMACDmaTxStopped;

	return interrupts;
}

/**
 * Returns the interrupt mask.
 * @param[in] pointer to struct synop_device.
 * \return 0 upon success. Error code upon failure.
 */
u32 synopGMAC_get_interrupt_mask(struct synop_device *gmacdev)
{
	return synopGMACReadReg((unsigned long *)gmacdev->DmaBase,
							DmaInterrupt);
}

/**
 * Enable all the interrupts.
 * Enables the DMA interrupt as specified by the bit mask.
 * @param[in] pointer to struct synop_device.
 * @param[in] bit mask of interrupts to be enabled.
 * \return returns void.
 */
void synopGMAC_enable_interrupt(struct synop_device *gmacdev, u32 interrupts)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaInterrupt, interrupts);
}

/**
 * Disable all the interrupts.
 * Disables all DMA interrupts.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 * \note This function disabled all the interrupts,
 * if you want to disable a particular interrupt then
 *  use synopGMAC_disable_interrupt().
 */
void synopGMAC_disable_interrupt_all(struct synop_device *gmacdev)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaInterrupt, DmaIntDisable);
}

/**
 * Disable interrupt according to the bitfield supplied.
 * Disables only those interrupts specified in the bit mask in second argument.
 * @param[in] pointer to struct synop_device.
 * @param[in] bit mask for interrupts to be disabled.
 * \return returns void.
 */
void synopGMAC_disable_interrupt(struct synop_device *gmacdev, u32 interrupts)
{
	synopGMACClearBits((unsigned long *)gmacdev->DmaBase,
						DmaInterrupt, interrupts);
}

/**
 * Enable the DMA Reception.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_enable_dma_rx(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase, DmaControl);
	data |= DmaRxStart;
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase, DmaControl, data);
}

/**
 * Enable the DMA Transmission.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_enable_dma_tx(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase, DmaControl);
	data |= DmaTxStart;
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase, DmaControl, data);
}

/**
 * Resumes the DMA Transmission.
 * the DmaTxPollDemand is written. (the data writeen could be anything).
 * This forces the DMA to resume transmission.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_resume_dma_tx(struct synop_device *gmacdev)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaTxPollDemand, 0);
	/*flush read/write register commands*/
	/*wbflush_ahb();*/
}

/**
 * Resumes the DMA Reception.
 * the DmaRxPollDemand is written. (the data writeen could be anything).
 * This forces the DMA to resume reception.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_resume_dma_rx(struct synop_device *gmacdev)
{
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase,
						DmaRxPollDemand, 0);
}

/**
 * Take ownership of this Descriptor.
 * The function is same for both the ring mode
 * and the chain mode DMA structures.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_take_desc_ownership(struct dma_desc *desc)
{
	if (desc) {
		/*Clear the DMA own bit*/
		desc->status &= ~DescOwnByDma;
	}
}

/**
 * Take ownership of all the rx Descriptors.
 * This function is called when there is fatal error
 * in DMA transmission.
 * When called it takes the ownership of
 * all the rx descriptor in rx descriptor pool/queue from DMA.
 * The function is same for both the ring mode
 * and the chain mode DMA structures.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 * \note Make sure to disable the transmission
 * before calling this function,
 * otherwise may result in racing situation.
 */
void synopGMAC_take_desc_ownership_rx(struct synop_device *gmacdev)
{
	u32 i;
	struct dma_desc *desc;

	if (!gmacdev)
		return;

	if (!gmacdev->RxDesc)
		return;

	desc = gmacdev->RxDesc;
	for (i = 0; i < gmacdev->RxDescCount; i++) {
		if (synopGMAC_is_rx_desc_chained(desc)) {
			/*This descriptor is in chain mode*/
			synopGMAC_take_desc_ownership(desc);
			desc = (struct dma_desc *)desc->data2;
		} else {
			synopGMAC_take_desc_ownership(desc + i);
		}
	}
}

/**
 * Take ownership of all the rx Descriptors.
 * This function is called when there is
 * fatal error in DMA transmission.
 * When called it takes the ownership of
 * all the tx descriptor in tx descriptor pool/queue from DMA.
 * The function is same for both the ring mode
 * and the chain mode DMA structures.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 * \note Make sure to disable the transmission before calling this function,
 * otherwise may result in racing situation.
 */
void synopGMAC_take_desc_ownership_tx(struct synop_device *gmacdev)
{
	u32 i;
	struct dma_desc *desc;

	if (!gmacdev)
		return;

	if (!gmacdev->TxDesc)
		return;

	desc = gmacdev->TxDesc;
	for (i = 0; i < gmacdev->TxDescCount; i++) {
		if (synopGMAC_is_tx_desc_chained(desc)) {
			/*This descriptor is in chain mode*/
			synopGMAC_take_desc_ownership(desc);
			desc = (struct dma_desc *)desc->data2;
		} else {
			synopGMAC_take_desc_ownership(desc + i);
		}
	}
}

/**
 * Disable the DMA for Transmission.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */

void synopGMAC_disable_dma_tx(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase, DmaControl);
	data &= (u32)(~DmaTxStart);
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase, DmaControl, data);
}

/**
 * Disable the DMA for Reception.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_disable_dma_rx(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase, DmaControl);
	data &= (u32)(~DmaRxStart);
	synopGMACWriteReg((unsigned long *)gmacdev->DmaBase, DmaControl, data);
}

u32 synopGMAC_get_dma_current_TX_desc(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase,
							DmaTxCurrDesc);
	return data;
}

u32 synopGMAC_get_dma_current_RX_desc(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->DmaBase,
							DmaRxCurrDesc);
	return data;
}

/*******************PMT APIs***************************************/
/**
 * Enables the assertion of PMT interrupt.
 * This enables the assertion of PMT interrupt due to Magic Pkt or Wakeup frame
 * reception.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_pmt_int_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacInterruptMask, GmacPmtIntMask);
}

/**
 * Disables the assertion of PMT interrupt.
 * This disables the assertion of PMT interrupt due to Magic Pkt or Wakeup frame
 * reception.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_pmt_int_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacInterruptMask, GmacPmtIntMask);
}

/**
 * Enables the power down mode of GMAC.
 * This function puts the Gmac in power down mode.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_power_down_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacPmtCtrlStatus, GmacPmtPowerDown);
}

/**
 * Disables the powerd down setting of GMAC.
 * If the driver wants to bring up the GMAC from powerdown mode,
 * even though the magic packet or the
 * wake up frames received from the network,
 * this function should be called.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_power_down_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacPmtCtrlStatus, GmacPmtPowerDown);
}

/**
 * Enables the pmt interrupt generation in powerdown mode.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_enable_pmt_interrupt(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacInterruptMask, GmacPmtIntMask);
}

/**
 * Disables the pmt interrupt generation in powerdown mode.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_disable_pmt_interrupt(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacInterruptMask, GmacPmtIntMask);
}

/**
 * Enables GMAC to look for Magic packet.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_magic_packet_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacPmtCtrlStatus, GmacPmtMagicPktEnable);
}

/**
 * Enables GMAC to look for wake up frame.
 * Wake up frame is defined by the user.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_wakeup_frame_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacPmtCtrlStatus, GmacPmtWakeupFrameEnable);
}

/**
 * Enables wake-up frame filter to handle unicast packets.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_pmt_unicast_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacPmtCtrlStatus, GmacPmtGlobalUnicast);
}

/**
 * Checks whether the packet received is a magic packet?.
 * @param[in] pointer to struct synop_device.
 * \return returns True if magic packet received else returns false.
 */
bool synopGMAC_is_magic_packet_received(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase,
							GmacPmtCtrlStatus);

	return((data & GmacPmtMagicPktReceived) == GmacPmtMagicPktReceived);
}

/**
 * Checks whether the packet received is a wakeup frame?.
 * @param[in] pointer to struct synop_device.
 * \return returns true if wakeup frame received else returns false.
 */
bool synopGMAC_is_wakeup_frame_received(struct synop_device *gmacdev)
{
	u32 data;

	data = synopGMACReadReg((unsigned long *)gmacdev->MacBase,
							GmacPmtCtrlStatus);

	return((data & GmacPmtWakeupFrameReceived) ==
			GmacPmtWakeupFrameReceived);
}

/**
 * Populates the remote wakeup frame registers.
 * Consecutive 8 writes to GmacWakeupAddr
 * writes the wakeup frame filter registers.
 * Before commensing a new write,
 * frame filter pointer is reset to 0x0000.
 * A small delay is introduced
 * to allow frame filter pointer reset operation.
 * @param[in] pointer to struct synop_device.
 * @param[in] pointer to frame filter contents array.
 * \return returns void.
 */
void synopGMAC_write_wakeup_frame_register(struct synop_device *gmacdev,
					   u32 *filter_contents)
{
	s32 i;

	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacPmtCtrlStatus, GmacPmtFrmFilterPtrReset);
	plat_delay(10);
	for (i = 0; i < WAKEUP_REG_LENGTH; i++) {
		synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
				  GmacWakeupAddr, *(filter_contents + i));
	}
}

/*******************PMT APIs***************************************/
/*******************MMC APIs***************************************/

/**
 * Freezes the MMC counters.
 * This function call freezes the MMC counters.
 * None of the MMC counters are updated
 * due to any tx or rx frames until
 * synopGMAC_mmc_counters_resume is called.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_mmc_counters_stop(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacMmcCntrl, GmacMmcCounterFreeze);
}

/**
 * Resumes the MMC counter updation.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_mmc_counters_resume(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacMmcCntrl, GmacMmcCounterFreeze);
}

/**
 * Configures the MMC in Self clearing mode.
 * Programs MMC interface so that counters
 * are cleared when the counters are read.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_mmc_counters_set_selfclear(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacMmcCntrl, GmacMmcCounterResetOnRead);
}

/**
 * Configures the MMC in non-Self clearing mode.
 * Programs MMC interface so that counters
 * are cleared when the counters are read.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_mmc_counters_reset_selfclear(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacMmcCntrl, GmacMmcCounterResetOnRead);
}

/**
 * Configures the MMC to stop rollover.
 * Programs MMC interface so that counters
 * will not rollover after reaching maximum value.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_mmc_counters_disable_rollover(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacMmcCntrl, GmacMmcCounterStopRollover);
}

/**
 * Configures the MMC to rollover.
 * Programs MMC interface so that counters
 * will rollover after reaching maximum value.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_mmc_counters_enable_rollover(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacMmcCntrl, GmacMmcCounterStopRollover);
}

/**
 * Read the MMC Counter.
 * @param[in] pointer to struct synop_device.
 * @param[in] the counter to be read.
 * \return returns the read count value.
 */
u32 synopGMAC_read_mmc_counter(struct synop_device *gmacdev, u32 counter)
{
	return synopGMACReadReg((unsigned long *)gmacdev->MacBase, counter);
}

/**
 * Read the MMC Rx interrupt status.
 * @param[in] pointer to struct synop_device.
 * \return returns the Rx interrupt status.
 */
u32 synopGMAC_read_mmc_rx_int_status(struct synop_device *gmacdev)
{
	return synopGMACReadReg((unsigned long *)gmacdev->MacBase,
							GmacMmcIntrRx);
}

/**
 * Read the MMC Tx interrupt status.
 * @param[in] pointer to struct synop_device.
 * \return returns the Tx interrupt status.
 */
u32 synopGMAC_read_mmc_tx_int_status(struct synop_device *gmacdev)
{
	return synopGMACReadReg((unsigned long *)gmacdev->MacBase,
							GmacMmcIntrTx);
}

/**
 * Disable the MMC Tx interrupt.
 * The MMC tx interrupts are masked out as per the mask specified.
 * @param[in] pointer to struct synop_device.
 * @param[in] tx interrupt bit mask for
 * which interrupts needs to be disabled.
 * \return returns void.
 */
void synopGMAC_disable_mmc_tx_interrupt(struct synop_device *gmacdev, u32 mask)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacMmcIntrMaskTx, mask);
}

/**
 * Enable the MMC Tx interrupt.
 * The MMC tx interrupts are enabled as per the mask specified.
 * @param[in] pointer to struct synop_device.
 * @param[in] tx interrupt bit mask for
 * which interrupts needs to be enabled.
 * \return returns void.
 */
void synopGMAC_enable_mmc_tx_interrupt(struct synop_device *gmacdev, u32 mask)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacMmcIntrMaskTx, mask);
}

/**
 * Disable the MMC Rx interrupt.
 * The MMC rx interrupts are masked out as per the mask specified.
 * @param[in] pointer to struct synop_device.
 * @param[in] rx interrupt bit mask
 * for which interrupts needs to be disabled.
 * \return returns void.
 */
void synopGMAC_disable_mmc_rx_interrupt(struct synop_device *gmacdev, u32 mask)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacMmcIntrMaskRx, mask);
}

/**
 * Enable the MMC Rx interrupt.
 * The MMC rx interrupts are enabled as per the mask specified.
 * @param[in] pointer to struct synop_device.
 * @param[in] rx interrupt bit mask for which interrupts needs to be enabled.
 * \return returns void.
 */
void synopGMAC_enable_mmc_rx_interrupt(struct synop_device *gmacdev, u32 mask)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacMmcIntrMaskRx, mask);
}

/**
 * Disable the MMC ipc rx checksum offload interrupt.
 * The MMC ipc rx checksum offload interrupts
 * are masked out as per the mask specified.
 * @param[in] pointer to struct synop_device.
 * @param[in] rx interrupt bit mask for which interrupts needs to be disabled.
 * \return returns void.
 */
void synopGMAC_disable_mmc_ipc_rx_interrupt(
	struct synop_device *gmacdev, u32 mask)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacMmcRxIpcIntrMask, mask);
}

/**
 * Enable the MMC ipc rx checksum offload interrupt.
 * The MMC ipc rx checksum offload interrupts
 * are enabled as per the mask specified.
 * @param[in] pointer to struct synop_device.
 * @param[in] rx interrupt bit mask
 * for which interrupts needs to be enabled.
 * \return returns void.
 */
void synopGMAC_enable_mmc_ipc_rx_interrupt(struct synop_device *gmacdev,
					   u32 mask)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacMmcRxIpcIntrMask, mask);
}

/*******************MMC APIs***************************************/
/*******************Ip checksum offloading APIs***************************************/

/**
 * Enables the ip checksum offloading in receive path.
 * When set GMAC calculates 16 bit 1's complement
 * of all received ethernet frame payload.
 * It also checks IPv4 Header checksum is correct.
 * GMAC core appends the 16 bit checksum calculated
 * for payload of IP datagram and appends
 * it to Ethernet frame transferred to the application.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_enable_rx_chksum_offload(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacConfig, GmacRxIpcOffload);
}

/**
 * Disable the ip checksum offloading in receive path.
 * Ip checksum offloading is disabled in the receive path.
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_disable_rx_Ipchecksum_offload(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacConfig, GmacRxIpcOffload);
}

/**
 * Instruct the DMA to drop the packets fails tcp ip checksum.
 * This is to instruct the receive DMA engine
 * to drop the recevied packet if they
 * fails the tcp/ip checksum in hardware.
 * Valid only when full checksum offloading is enabled(type-2).
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_rx_tcpip_chksum_drop_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->DmaBase,
			   DmaControl, DmaDisableDropTcpCs);
}

/**
 * Instruct the DMA not to drop the packets even if it fails tcp ip checksum.
 * This is to instruct the receive DMA engine
 * to allow the packets even if recevied packet
 * fails the tcp/ip checksum in hardware.
 * Valid only when full checksum offloading is enabled(type-2).
 * @param[in] pointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_rx_tcpip_chksum_drop_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->DmaBase,
			 DmaControl, DmaDisableDropTcpCs);
}

/**
 * When the Enhanced Descriptor is enabled
 * then the bit 0 of RDES0 indicates whether the
 * Extended Status is available (RDES4).
 * Time Stamp feature and the Checksum Offload Engine2
 * makes use of this extended status
 * to provide the status of the received packet.
 * @param[in] pointer to struct synop_device
 * \return returns TRUE or FALSE
 */
#ifdef ENH_DESC_8W

/**
 * This function indicates whether extended status is available in the RDES0.
 * Any function which accesses the fields
 * of extended status register must ensure a check on this has been made
 * This is valid only for Enhanced Descriptor.
 * @param[in] pointer to struct synop_device.
 * @param[in] u32 status field of the corresponding descriptor.
 * \return returns TRUE or FALSE.
 */
bool synopGMAC_is_ext_status(struct synop_device *gmacdev, u32 status)
{
	/*extended status present indicates that the RDES4 need to be probed*/
	/* if extstatus set then it returns 1*/
	return((status & DescRxEXTsts) != 0);
}

/**
 * This function returns true if the IP header checksum bit is set in the extended status.
 * Valid only when enhanced status available is set in RDES0 bit 0.
 * This is valid only for Enhanced Descriptor.
 * @param[in] pointer to struct synop_device.
 * @param[in] u32 status field of the corresponding descriptor.
 * \return returns TRUE or FALSE.
 */
bool synopGMAC_ES_is_IP_header_error(struct synop_device *gmacdev,
				     u32 ext_status)
{
	/*IP header (IPV4) checksum error*/
	/* if IPV4 header error return 1 */
	return((ext_status & DescRxIpHeaderError) != 0);
}

/**
 * This function returns true if the Checksum is bypassed in the hardware.
 * Valid only when enhanced status available is set in RDES0 bit 0.
 * This is valid only for Enhanced Descriptor.
 * @param[in] pointer to struct synop_device.
 * @param[in] u32 status field of the corresponding descriptor.
 * \return returns TRUE or FALSE.
 */
bool synopGMAC_ES_is_rx_checksum_bypassed(struct synop_device *gmacdev,
					  u32 ext_status)
{
	/*Hardware engine bypassed the checksum computation/checking*/
	/* if checksum offloading bypassed return 1*/
	return((ext_status & DescRxChkSumBypass) != 0);
}

/**
 * This function returns true if payload checksum error
 * is set in the extended status.
 * Valid only when enhanced status available is set in RDES0 bit 0.
 * This is valid only for Enhanced Descriptor.
 * @param[in] pointer to struct synop_device.
 * @param[in] u32 status field of the corresponding descriptor.
 * \return returns TRUE or FALSE.
 */
bool synopGMAC_ES_is_IP_payload_error(struct synop_device *gmacdev,
				      u32 ext_status)
{
	/*IP payload checksum is in error (UDP/TCP/ICMP checksum error)*/
	/* if IP payload error return 1*/
	return((ext_status & DescRxIpPayloadError) != 0);
}
#endif

/**
 * Decodes the Rx Descriptor status to various checksum error conditions.
 * @param[in] pointer to struct synop_device.
 * @param[in] u32 status field of the corresponding descriptor.
 * \return returns decoded enum (u32) indicating the status.
 */
u32 synopGMAC_is_rx_checksum_error(struct synop_device *gmacdev,
				   u32 status)
{
	if (((status & DescRxChkBit5) == 0) &&
	    ((status & DescRxChkBit7) == 0) &&
			((status & DescRxChkBit0) == 0))
		return RxLenLT600;
	else if (((status & DescRxChkBit5) == 0) &&
		 ((status & DescRxChkBit7) == 0) &&
			((status & DescRxChkBit0) != 0))
		return RxIpHdrPayLoadChkBypass;
	else if (((status & DescRxChkBit5) == 0) &&
		 ((status & DescRxChkBit7) != 0) &&
			((status & DescRxChkBit0) != 0))
		return RxChkBypass;
	else if (((status & DescRxChkBit5) != 0) &&
		 ((status & DescRxChkBit7) == 0) &&
			((status & DescRxChkBit0) == 0))
		return RxNoChkError;
	else if (((status & DescRxChkBit5) != 0) &&
		 ((status & DescRxChkBit7) == 0) &&
			((status & DescRxChkBit0) != 0))
		return RxPayLoadChkError;
	else if (((status & DescRxChkBit5) != 0) &&
		 ((status & DescRxChkBit7) != 0) &&
			((status & DescRxChkBit0) == 0))
		return RxIpHdrChkError;
	else if (((status & DescRxChkBit5) != 0) &&
		 ((status & DescRxChkBit7) != 0) &&
			((status & DescRxChkBit0) != 0))
		return RxIpHdrPayLoadChkError;
	else
		return RxIpHdrPayLoadRes;
}

/**
 * Checks if any Ipv4 header checksum error in the frame just transmitted.
 * This serves as indication that error occureed
 * in the IPv4 header checksum insertion.
 * The sent out frame doesnot carry any ipv4 header checksum inserted
 * by the hardware.
 * @param[in] pointer to struct synop_device.
 * @param[in] u32 status field of the corresponding descriptor.
 * \return returns true if error in ipv4 header checksum, else returns false.
 */
bool synopGMAC_is_tx_ipv4header_checksum_error(
		struct synop_device *gmacdev, u32 status)
{
	return((status & DescTxIpv4ChkError) == DescTxIpv4ChkError);
}

/**
 * Checks if any payload checksum error
 * in the frame just transmitted.
 * This serves as indication that error occureed
 * in the payload checksum insertion.
 * The sent out frame doesnot carry
 * any payload checksum inserted by the hardware.
 * @param[in] pointer to struct synop_device.
 * @param[in] u32 status field of the corresponding descriptor.
 * \return returns true if error in ipv4 header checksum,
 * else returns false.
 */
bool synopGMAC_is_tx_payload_checksum_error(
		struct synop_device *gmacdev, u32 status)
{
	return((status & DescTxPayChkError) == DescTxPayChkError);
}

/**
 * The check summ offload engine is bypassed in the tx path.
 * Checksum is not computed in the Hardware.
 * @param[in] pointer to struct synop_device.
 * @param[in] Pointer to tx descriptor
 * for which ointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_tx_checksum_offload_bypass(
		struct synop_device *gmacdev, struct dma_desc *desc)
{
#ifdef ENH_DESC
	desc->status = (desc->length & (u32)(~DescTxCisMask));
#else
	desc->length = (desc->length & (u32)(~DescTxCisMask));
#endif
}

/**
 * The check summ offload engine is enabled
 * to do only IPV4 header checksum.
 * IPV4 header Checksum is computed in the Hardware.
 * @param[in] pointer to struct synop_device.
 * @param[in] Pointer to tx descriptor for which
 * ointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_tx_checksum_offload_ipv4hdr(
		struct synop_device *gmacdev, struct dma_desc *desc)
{
#ifdef ENH_DESC
	desc->status =
		((desc->status & (u32)(~DescTxCisMask)) |
		 DescTxCisIpv4HdrCs);
#else
	desc->length =
		((desc->length & (u32)(~DescTxCisMask)) |
		 DescTxCisIpv4HdrCs);
#endif
}

/**
 * The check summ offload engine is enabled
 * to do TCPIP checsum assuming Pseudo header is available.
 * Hardware computes the tcp ip checksum
 * assuming pseudo header checksum is computed in software.
 * Ipv4 header checksum is also inserted.
 * @param[in] pointer to struct synop_device.
 * @param[in] Pointer to tx descriptor for which
 * ointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_tx_checksum_offload_tcponly(
		struct synop_device *gmacdev, struct dma_desc *desc)
{
#ifdef ENH_DESC
	desc->status =
		((desc->status & (u32)(~DescTxCisMask)) |
		 DescTxCisTcpOnlyCs);
#else
	desc->length =
		((desc->length & (u32)(~DescTxCisMask)) |
		 DescTxCisTcpOnlyCs);
#endif
}

/**
 * The check summ offload engine
 * is enabled to do complete checksum computation.
 * Hardware computes the tcp ip
 * checksum including the pseudo header checksum.
 * Here the tcp payload checksum field should be set to 0000.
 * Ipv4 header checksum is also inserted.
 * @param[in] pointer to struct synop_device.
 * @param[in] Pointer to tx descriptor for which
 * ointer to struct synop_device.
 * \return returns void.
 */
void synopGMAC_tx_checksum_offload_tcp_pseudo(struct synop_device *gmacdev,
					      struct dma_desc *desc)
{
#ifdef ENH_DESC
	desc->status =
		((desc->length & (u32)(~DescTxCisMask)) |
		 DescTxCisTcpPseudoCs);
#else
	desc->length =
		((desc->length & (u32)(~DescTxCisMask)) |
		 DescTxCisTcpPseudoCs);
#endif
}

/***Ip checksum offloading APIs*****************************/
/***IEEE 1588 Timestamping API*****************************/

/* Comment
 * At this time the driver supports the IEEE time stamping feature
 * when the Enhanced Descriptors are enabled.
 * For normal descriptor and the IEEE time stamp (version 1),
 * driver support is not proviced
 * Please make sure you have enabled the Advanced timestamp
 * feature in the hardware and the driver should
 * be compiled with the ADV_TME_STAMP feature.
 * Some of the APIs provided here may not be valid for all configurations.
 * Please make sure you call the
 * API with due care.
 */

/**
 * This function enables the timestamping.
 * This enables the timestamping for transmit and receive frames.
 * When disabled timestamp is not added
 * to tx and receive frames and timestamp generator is suspended.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSENA);
}

/**
 * This function disables the timestamping.
 * When disabled timestamp is not added to tx and receive
 * frames and timestamp generator is suspended.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacInterruptMask, GmacTSIntMask);
}

/**
 * Enable the interrupt to get timestamping interrupt.
 * This enables the host to get the interrupt when
 * (1) system time is greater or equal to the
 * target time high and low register or
 * (2) there is a overflow in th esecond register.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_int_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacInterruptMask, GmacPmtIntMask);
}

/**
 * Disable the interrupt to get timestamping interrupt.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_int_disable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacInterruptMask, GmacPmtIntMask);
}

/**
 * Enable MAC address for PTP frame filtering.
 * When enabled, uses MAC address (apart from MAC address 0)
 * to filter the PTP frames when
 * PTP is sent directly over Ethernet.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_mac_addr_filt_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSENMACADDR);
}

/**
 * Disables MAC address for PTP frame filtering.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_mac_addr_filt_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSENMACADDR);
}

/**
 * Selet the type of clock mode for PTP.
 * Please note to use one of the follwoing as the clk_type argument.
 * GmacTSOrdClk = 0x00000000, 00=> Ordinary clock
 * GmacTSBouClk = 0x00010000,	01=> Boundary clock
 * GmacTSEtoEClk = 0x00020000, 10=> End-to-End transparent clock
 * GmacTSPtoPClk = 0x00030000, 11=> P-to-P transparent clock
 * @param[in] pointer to struct synop_device
 * @param[in] u32 value representing one of the above clk value
 * \return returns void
 */
void synopGMAC_TS_set_clk_type(
		struct synop_device *gmacdev, u32 clk_type)
{
	u32 clkval;

	/*set the mdc clock to the user defined value*/
	clkval = synopGMACReadReg(
			(unsigned long *)gmacdev->MacBase, GmacTSControl);
	clkval = clkval | clk_type;

	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSControl, clkval);
}

/**
 * Enable Snapshot for messages relevant to Master.
 * When enabled, snapshot is taken for messages relevant
 * to master mode only, else snapshot is taken for messages relevant
 * to slave node.
 * Valid only for Ordinary clock and Boundary clock
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_master_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSMSTRENA);
}

/**
 * Disable Snapshot for messages relevant to Master.
 * When disabled, snapshot is taken for messages relevant
 * to slave node.
 * Valid only for Ordinary clock and Boundary clock
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_master_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSMSTRENA);
}

/**
 * Enable Snapshot for Event messages.
 * When enabled, snapshot is taken for event messages only
 * (SYNC, Delay_Req, Pdelay_Req or Pdelay_Resp)
 * When disabled, snapshot is taken for all messages
 * except Announce, Management and Signaling.
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_event_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSEVNTENA);
}

/**
 * Disable Snapshot for Event messages.
 * When disabled, snapshot is taken for all messages
 * except Announce, Management and Signaling.
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_event_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSEVNTENA);
}

/**
 * Enable time stamp snapshot for IPV4 frames.
 * When enabled, time stamp snapshot is taken for IPV4 frames
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_IPV4_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSIPV4ENA);
}

/**
 * Disable time stamp snapshot for IPV4 frames.
 * When disabled, time stamp snapshot is not taken for IPV4 frames
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_IPV4_disable(struct synop_device *gmacdev)
{
	/*Only for "Advanced Time Stamp"*/
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSIPV4ENA);
}

/**
 * Enable time stamp snapshot for IPV6 frames.
 * When enabled, time stamp snapshot is taken for IPV6 frames
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_IPV6_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSIPV6ENA);
}

/**
 * Disable time stamp snapshot for IPV6 frames.
 * When disabled, time stamp snapshot is not taken for IPV6 frames
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_IPV6_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSIPV6ENA);
}

/**
 * Enable time stamp snapshot for PTP over Ethernet frames.
 * When enabled, time stamp snapshot is taken for PTP over Ethernet frames
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_ptp_over_ethernet_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSIPENA);
}

/**
 * Disable time stamp snapshot for PTP over Ethernet frames.
 * When disabled, time stamp snapshot is not taken
 * for PTP over Ethernet frames
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_ptp_over_ethernet_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSIPENA);
}

/**
 * Snoop PTP packet for version 2 format
 * When set the PTP packets are snooped using the version 2 format.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_pkt_snoop_ver2(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSVER2ENA);
}

/**
 * Snoop PTP packet for version 2 format
 * When set the PTP packets are snooped using the version 2 format.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_pkt_snoop_ver1(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSVER2ENA);
}

/**
 * Timestamp digital rollover
 * When set the timestamp low register rolls over after 0x3B9A_C9FF value.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_digital_rollover_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSCTRLSSR);
}

/**
 * Timestamp binary rollover
 * When set the timestamp low register rolls
 * over after 0x7FFF_FFFF value.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_binary_rollover_enable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSCTRLSSR);
}

/**
 * Enable Time Stamp for All frames
 * When set the timestamp snap shot is enabled
 * for all frames received by the core.
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_all_frames_enable(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSENALL);
}

/**
 * Disable Time Stamp for All frames
 * When reset the timestamp snap shot
 * is not enabled for all frames received by the core.
 * Reserved when "Advanced Time Stamp" is not selected
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_all_frames_disable(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSENALL);
}

/**
 * Addend Register Update
 * This function loads the contents of Time stamp
 * addend register with the supplied 32 value.
 * This is reserved function when only coarse correction option is selected
 * @param[in] pointer to struct synop_device
 * @param[in] 32 bit addend value
 * \return returns 0 for Success or else Failure
 */
s32 synopGMAC_TS_addend_update(struct synop_device *gmacdev, u32 addend_value)
{
	u32 loop_variable;

	/* Load the addend_value in to Addend register*/
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSAddend, addend_value);

	/*Wait till the busy bit gets cleared with in a certain amount of time*/
	for (loop_variable = 0;
			loop_variable < DEFAULT_LOOP_VARIABLE;
			loop_variable++) {
		/*if it is cleared then break*/
		if (!((synopGMACReadReg((unsigned long *)gmacdev->MacBase,
					GmacTSControl)) & GmacTSADDREG)){
			break;
		}
		plat_delay(DEFAULT_DELAY_VARIABLE);
	}
	if (loop_variable < DEFAULT_LOOP_VARIABLE)
		synopGMACSetBits((unsigned long *)gmacdev->MacBase,
				 GmacTSControl, GmacTSADDREG);
	else{
		TR("Error:The TSADDREG bit isn't cleared!\n");
		return -ESYNOPGMACPHYERR;
	}
	return -ESYNOPGMACNOERR;
}

/**
 * time stamp Update
 * This function updates (adds/subtracts)
 * with the value specified in the Timestamp High Update and
 * Timestamp Low Update register.
 * @param[in] pointer to struct synop_device
 * @param[in] Timestamp High Update value
 * @param[in] Timestamp Low Update value
 * \return returns 0 for Success or else Failure
 */
s32 synopGMAC_TS_timestamp_update(
		struct synop_device *gmacdev, u32 high_value, u32 low_value)
{
	u32 loop_variable;

	/* Load the high value to Timestamp High register*/
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSHighUpdate, high_value);

	/* Load the high value to Timestamp High register*/
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSLowUpdate, low_value);

	/*Wait till the busy bit gets cleared with in a certain amount of time*/
	for (loop_variable = 0;
			loop_variable < DEFAULT_LOOP_VARIABLE;
			loop_variable++) {
		/* if it is cleared then break*/
		if (!((synopGMACReadReg((unsigned long *)gmacdev->MacBase,
					GmacTSControl)) & GmacTSUPDT)) {
			break;
		}
		plat_delay(DEFAULT_DELAY_VARIABLE);
	}
	if (loop_variable < DEFAULT_LOOP_VARIABLE) {
		synopGMACSetBits((unsigned long *)gmacdev->MacBase,
				 GmacTSControl, GmacTSUPDT);
	} else {
		TR("Error: The TSADDREG bit isn't cleared !\n");
		return -ESYNOPGMACPHYERR;
	}
	return -ESYNOPGMACNOERR;
}

/**
 * time stamp Initialize
 * This function Loads/Initializes h the value
 * specified in the Timestamp High Update and
 * Timestamp Low Update register.
 * @param[in] pointer to struct synop_device
 * @param[in] Timestamp High Load value
 * @param[in] Timestamp Low Load value
 * \return returns 0 for Success or else Failure
 */
s32 synopGMAC_TS_timestamp_init(
		struct synop_device *gmacdev, u32 high_value, u32 low_value)
{
	u32 loop_variable;

	/*Load the high value to Timestamp High register*/
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSHighUpdate, high_value);

	/*Load the high value to Timestamp High register*/
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSLowUpdate, low_value);

	/*Wait till the busy bit gets cleared with in a certain amount of time*/
	for (loop_variable = 0;
			loop_variable < DEFAULT_LOOP_VARIABLE;
			loop_variable++) {
		/*if it is cleared then break*/
		if (!((synopGMACReadReg((unsigned long *)gmacdev->MacBase,
					GmacTSControl)) & GmacTSINT)) {
			break;
		}
		plat_delay(DEFAULT_DELAY_VARIABLE);
	}
	if (loop_variable < DEFAULT_LOOP_VARIABLE) {
		synopGMACSetBits((unsigned long *)gmacdev->MacBase,
				 GmacTSControl, GmacTSINT);
	} else {
		TR("Error: The TSADDREG bit isn't cleared !\n");
		return -ESYNOPGMACPHYERR;
	}
	return -ESYNOPGMACNOERR;
}

/**
 * Time Stamp Update Coarse
 * When reset the timestamp update is done using coarse method.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_coarse_update(struct synop_device *gmacdev)
{
	synopGMACClearBits((unsigned long *)gmacdev->MacBase,
			   GmacTSControl, GmacTSCFUPDT);
}

/**
 * Time Stamp Update Fine
 * When reset the timestamp update is done using Fine method.
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_fine_update(struct synop_device *gmacdev)
{
	synopGMACSetBits((unsigned long *)gmacdev->MacBase,
			 GmacTSControl, GmacTSCFUPDT);
}

/**
 * Load the Sub Second Increment value in to Sub Second increment register
 * @param[in] pointer to struct synop_device
 * \return returns void
 */
void synopGMAC_TS_subsecond_init(struct synop_device *gmacdev,
				 u32 sub_sec_inc_value)
{
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSSubSecIncr, (sub_sec_inc_value & GmacSSINCMsk));
}

/**
 * Reads the time stamp contents in to the respective pointers
 * These registers are readonly.
 * This function returns the 48 bit time stamp assuming
 * Version 2 timestamp with higher word is selected.
 * @param[in] pointer to struct synop_device
 * @param[in] pointer to hold 16 higher bit second register contents
 * @param[in] pointer to hold 32 bit second register contents
 * @param[in] pointer to hold 32 bit subnanosecond register contents
 * \return returns void
 * \note Please note that since the atomic
 * access to the timestamp registers is not possible,
 *  the contents read may be different from the actual time stamp.
 */
void synopGMAC_TS_read_timestamp(struct synop_device *gmacdev,
				 u16 *higher_sec_val,
				 u32 *sec_val, u32 *sub_sec_val)
{
	*higher_sec_val =
		(u16)(synopGMACReadReg((unsigned long *)gmacdev->MacBase,
			GmacTSHighWord) & GmacTSHighWordMask);
	*sec_val =
		synopGMACReadReg((unsigned long *)gmacdev->MacBase, GmacTSHigh);

	*sub_sec_val =
		synopGMACReadReg((unsigned long *)gmacdev->MacBase, GmacTSLow);
}

/**
 * Loads the time stamp higher sec value from the value supplied
 * @param[in] pointer to struct synop_device
 * @param[in] 16 higher bit second register contents passed as 32 bit value
 * \return returns void
 */
void synopGMAC_TS_load_timestamp_higher_val(struct synop_device *gmacdev,
					    u32 higher_sec_val)
{
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSHighWord, (higher_sec_val &
			GmacTSHighWordMask));
}

/**
 * Reads the time stamp higher sec value to respective pointers
 * @param[in] pointer to struct synop_device
 * @param[in] pointer to hold 16 higher bit second register contents
 * \return returns void
 */
void synopGMAC_TS_read_timestamp_higher_val(struct synop_device *gmacdev,
					    u16 *higher_sec_val)
{
	*higher_sec_val =
		(u16)(synopGMACReadReg((unsigned long *)gmacdev->MacBase,
		GmacTSHighWord) & GmacTSHighWordMask);
}

/**
 * Load the Target time stamp registers
 * This function Loads the target time stamp registers
 * with the values proviced
 * @param[in] pointer to struct synop_device
 * @param[in] target Timestamp High value
 * @param[in] target Timestamp Low  value
 * \return returns 0 for Success or else Failure
 */
void synopGMAC_TS_load_target_timestamp(struct synop_device *gmacdev,
					u32 sec_val, u32 sub_sec_val)
{
	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSTargetTimeHigh, sec_val);

	synopGMACWriteReg((unsigned long *)gmacdev->MacBase,
			  GmacTSTargetTimeLow, sub_sec_val);
}

/**
 * Reads the Target time stamp registers
 * This function Loads the target time stamp
 * registers with the values proviced
 * @param[in] pointer to struct synop_device
 * @param[in] pointer to hold target Timestamp High value
 * @param[in] pointer to hold target Timestamp Low  value
 * \return returns 0 for Success or else Failure
 */
void synopGMAC_TS_read_target_timestamp(struct synop_device *gmacdev,
					u32 *sec_val, u32 *sub_sec_val)
{
	*sec_val =
		synopGMACReadReg((unsigned long *)gmacdev->MacBase,
				 GmacTSTargetTimeHigh);
	*sub_sec_val =
		synopGMACReadReg((unsigned long *)gmacdev->MacBase,
				 GmacTSTargetTimeLow);
}

