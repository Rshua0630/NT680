/* =========================================================================
 * The Synopsys DWC ETHER QOS Software Driver and documentation (hereinafter
 * "Software") is an unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto.  Permission is hereby granted,
 * free of charge, to any person obtaining a copy of this software annotated
 * with this license and the Software, to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================= */

/*!@file: DWC_ETH_QOS_mdio.c
 * @brief: Driver functions.
 */
#include "DWC_ETH_QOS_yheader.h"

/*!
* \brief read MII PHY register, function called by the driver alone
*
* \details Read MII registers through the API read_phy_reg where the
* related MAC registers can be configured.
*
* \param[in] pdata - pointer to driver private data structure.
* \param[in] phyaddr - the phy address to read
* \param[in] phyreg - the phy regiester id to read
* \param[out] phydata - pointer to the value that is read from the phy registers
*
* \return int
*
* \retval  0 - successfully read data from register
* \retval -1 - error occurred
* \retval  1 - if the feature is not defined.
*/

INT DWC_ETH_QOS_mdio_read_direct(struct DWC_ETH_QOS_prv_data *pdata,
				 int phyaddr, int phyreg, int *phydata)
{
	struct hw_if_struct *hw_if = &(pdata->hw_if);
	int phy_reg_read_status;

	DBGPR_MDIO("--> DWC_ETH_QOS_mdio_read_direct\n");

	if (hw_if->read_phy_regs) {
		phy_reg_read_status =
		    hw_if->read_phy_regs(phyaddr, phyreg, phydata);
	} else {
		phy_reg_read_status = 1;
		printk(KERN_ALERT "%s: hw_if->read_phy_regs not defined",
		       DEV_NAME);
	}

	DBGPR_MDIO("<-- DWC_ETH_QOS_mdio_read_direct\n");

	return phy_reg_read_status;
}

/*!
* \brief write MII PHY register, function called by the driver alone
*
* \details Writes MII registers through the API write_phy_reg where the
* related MAC registers can be configured.
*
* \param[in] pdata - pointer to driver private data structure.
* \param[in] phyaddr - the phy address to write
* \param[in] phyreg - the phy regiester id to write
* \param[out] phydata - actual data to be written into the phy registers
*
* \return void
*
* \retval  0 - successfully read data from register
* \retval -1 - error occurred
* \retval  1 - if the feature is not defined.
*/

INT DWC_ETH_QOS_mdio_write_direct(struct DWC_ETH_QOS_prv_data *pdata,
				  int phyaddr, int phyreg, int phydata)
{
	struct hw_if_struct *hw_if = &(pdata->hw_if);
	int phy_reg_write_status;

	DBGPR_MDIO("--> DWC_ETH_QOS_mdio_write_direct\n");

	if (hw_if->write_phy_regs) {
		phy_reg_write_status =
		    hw_if->write_phy_regs(phyaddr, phyreg, phydata);
	} else {
		phy_reg_write_status = 1;
		printk(KERN_ALERT "%s: hw_if->write_phy_regs not defined",
		       DEV_NAME);
	}

	DBGPR_MDIO("<-- DWC_ETH_QOS_mdio_write_direct\n");

	return phy_reg_write_status;
}

/*!
* \brief read MII PHY register.
*
* \details Read MII registers through the API read_phy_reg where the
* related MAC registers can be configured.
*
* \param[in] bus - points to the mii_bus structure
* \param[in] phyaddr - the phy address to write
* \param[in] phyreg - the phy register offset to write
*
* \return int
*
* \retval  - value read from given phy register
*/

static INT DWC_ETH_QOS_mdio_read(struct mii_bus *bus, int phyaddr, int phyreg)
{
	struct net_device *dev = bus->priv;
	struct DWC_ETH_QOS_prv_data *pdata = netdev_priv(dev);
	struct hw_if_struct *hw_if = &(pdata->hw_if);
	int phydata;

	DBGPR_MDIO("--> DWC_ETH_QOS_mdio_read: phyaddr = %d, phyreg = %d\n",
	      phyaddr, phyreg);

	if (hw_if->read_phy_regs) {
		hw_if->read_phy_regs(phyaddr, phyreg, &phydata);
	} else {
		printk(KERN_ALERT "%s: hw_if->read_phy_regs not defined",
		       DEV_NAME);
	}

	DBGPR_MDIO("<-- DWC_ETH_QOS_mdio_read: phydata = %#x\n", phydata);

	return phydata;
}

/*!
* \brief API to write MII PHY register
*
* \details This API is expected to write MII registers with the value being
* passed as the last argument which is done in write_phy_regs API
* called by this function.
*
* \param[in] bus - points to the mii_bus structure
* \param[in] phyaddr - the phy address to write
* \param[in] phyreg - the phy register offset to write
* \param[in] phydata - the register value to write with
*
* \return 0 on success and -ve number on failure.
*/

static INT DWC_ETH_QOS_mdio_write(struct mii_bus *bus, int phyaddr, int phyreg,
				  u16 phydata)
{
	struct net_device *dev = bus->priv;
	struct DWC_ETH_QOS_prv_data *pdata = netdev_priv(dev);
	struct hw_if_struct *hw_if = &(pdata->hw_if);
	INT ret = Y_SUCCESS;

	DBGPR_MDIO("--> DWC_ETH_QOS_mdio_write\n");

	if (hw_if->write_phy_regs) {
		hw_if->write_phy_regs(phyaddr, phyreg, phydata);
	} else {
		ret = -1;
		printk(KERN_ALERT "%s: hw_if->write_phy_regs not defined",
		       DEV_NAME);
	}

	DBGPR_MDIO("<-- DWC_ETH_QOS_mdio_write\n");

	return ret;
}


/*!
* \brief API to reset PHY
*
* \details This API is issue soft reset to PHY core and waits
* until soft reset completes.
*
* \param[in] bus - points to the mii_bus structure
*
* \return 0 on success and -ve number on failure.
*/

static INT DWC_ETH_QOS_mdio_reset(struct mii_bus *bus)
{
	struct net_device *dev = bus->priv;
	struct DWC_ETH_QOS_prv_data *pdata = netdev_priv(dev);
	struct hw_if_struct *hw_if = &(pdata->hw_if);
	INT phydata;

	DBGPR_MDIO("-->DWC_ETH_QOS_mdio_reset: phyaddr : %d\n", pdata->phyaddr);

#if 0 //def DWC_ETH_QOS_CONFIG_PGTEST
	printk(KERN_ALERT "PHY Programming for Autoneg disable\n");
	hw_if->read_phy_regs(pdata->phyaddr, MII_BMCR, &phydata);
	phydata &= ~(1 << 12);
	hw_if->write_phy_regs(pdata->phyaddr, MII_BMCR, phydata);
#endif

	hw_if->read_phy_regs(pdata->phyaddr, MII_BMCR, &phydata);

	if (phydata < 0)
		return 0;

	/* issue soft reset to PHY */
	phydata |= BMCR_RESET;
	hw_if->write_phy_regs(pdata->phyaddr, MII_BMCR, phydata);

	/* wait until software reset completes */
	do {
		hw_if->read_phy_regs(pdata->phyaddr, MII_BMCR, &phydata);
	} while ((phydata >= 0) && (phydata & BMCR_RESET));

#if 0 //def DWC_ETH_QOS_CONFIG_PGTEST
	printk(KERN_ALERT "PHY Programming for Loopback\n");
	hw_if->read_phy_regs(pdata->phyaddr, MII_BMCR, &phydata);
	phydata |= (1 << 14);
	hw_if->write_phy_regs(pdata->phyaddr, MII_BMCR, phydata);
#endif

	DBGPR_MDIO("<--DWC_ETH_QOS_mdio_reset\n");

	return 0;
}

#ifdef YDEBUG
/*!
 * \details This function is invoked by other functions to get the PHY register
 * dump. This function is used during development phase for debug purpose.
 *
 * \param[in] pdata ??pointer to private data structure.
 *
 * \return 0
 */

void dump_phy_registers(struct DWC_ETH_QOS_prv_data *pdata)
{
	int phydata = 0;

	printk(KERN_ALERT
	       "\n************* PHY Reg dump *************************\n");
	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_BMCR, &phydata);
	printk(KERN_ALERT
	       "Phy Control Reg(Basic Mode Control Reg) (%#x) = %#x\n",
	       MII_BMCR, phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_BMSR, &phydata);
	printk(KERN_ALERT "Phy Status Reg(Basic Mode Status Reg) (%#x) = %#x\n",
	       MII_BMSR, phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_PHYSID1,
	    &phydata);
	printk(KERN_ALERT "Phy Id (PHYS ID 1) (%#x)= %#x\n", MII_PHYSID1,
	    phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_PHYSID2,
	    &phydata);
	printk(KERN_ALERT "Phy Id (PHYS ID 2) (%#x)= %#x\n", MII_PHYSID2,
	    phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_ADVERTISE,
	    &phydata);
	printk(KERN_ALERT "Auto-nego Adv (Advertisement Control Reg)"\
	    " (%#x) = %#x\n", MII_ADVERTISE, phydata);

	/* read Phy Control Reg */
	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_LPA,
	    &phydata);
	printk(KERN_ALERT "Auto-nego Lap (Link Partner Ability Reg)"\
	    " (%#x)= %#x\n", MII_LPA, phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_EXPANSION,
	    &phydata);
	printk(KERN_ALERT "Auto-nego Exp (Extension Reg)"\
	    "(%#x) = %#x\n", MII_EXPANSION, phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr,
	    DWC_ETH_QOS_AUTO_NEGO_NP, &phydata);
	printk(KERN_ALERT "Auto-nego Np (%#x) = %#x\n",
	    DWC_ETH_QOS_AUTO_NEGO_NP, phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_ESTATUS,
				     &phydata);
	printk(KERN_ALERT "Extended Status Reg (%#x) = %#x\n", MII_ESTATUS,
	       phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_CTRL1000,
	    &phydata);
	printk(KERN_ALERT "1000 Ctl Reg (1000BASE-T Control Reg)"\
	    "(%#x) = %#x\n", MII_CTRL1000, phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, MII_STAT1000,
	    &phydata);
	printk(KERN_ALERT "1000 Sts Reg (1000BASE-T Status)(%#x) = %#x\n",
	       MII_STAT1000, phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr, DWC_ETH_QOS_PHY_CTL,
			&phydata);
	printk(KERN_ALERT "PHY Ctl Reg (%#x) = %#x\n", DWC_ETH_QOS_PHY_CTL,
	    phydata);

	DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr,
	    DWC_ETH_QOS_PHY_STS, &phydata);
	printk(KERN_ALERT "PHY Sts Reg (%#x) = %#x\n", DWC_ETH_QOS_PHY_STS,
	    phydata);

	printk(KERN_ALERT
	       "\n****************************************************\n");
}
#endif

/*!
* \brief API to adjust link parameters.
*
* \details This function will be called by PAL to inform the driver
* about various link parameters like duplex and speed. This function
* will configure the MAC based on link parameters.
*
* \param[in] dev - pointer to net_device structure
*
* \return void
*/

static void DWC_ETH_QOS_adjust_link(struct net_device *dev)
{
	struct DWC_ETH_QOS_prv_data *pdata = netdev_priv(dev);
	struct hw_if_struct *hw_if = &(pdata->hw_if);
	struct phy_device *phydev = pdata->phydev;
	unsigned long flags;
	int new_state = 0;

	if (phydev == NULL)
		return;

	DBGPR_MDIO("-->DWC_ETH_QOS_adjust_link. address %d link %d\n", phydev->addr,
	      phydev->link);

	spin_lock_irqsave(&pdata->lock, flags);

	if (phydev->link) {
		/* Now we make sure that we can be in full duplex mode.
		 * If not, we operate in half-duplex mode */
		if (phydev->duplex != pdata->oldduplex) {
			new_state = 1;
			if (phydev->duplex)
				hw_if->set_full_duplex();
			else {
				hw_if->set_half_duplex();
#ifdef DWC_ETH_QOS_CERTIFICATION_PKTBURSTCNT_HALFDUPLEX
				/* For Synopsys testing and debugging only */
				{
					UINT phydata;

					/* setting 'Assert CRS on transmit' */
					phydata = 0;
					DWC_ETH_QOS_mdio_read_direct(pdata, pdata->phyaddr,
						DWC_ETH_QOS_PHY_CTL, &phydata);
					phydata |= (1 << 11);
					DWC_ETH_QOS_mdio_write_direct(pdata, pdata->phyaddr,
						DWC_ETH_QOS_PHY_CTL, phydata);
				}
#endif
			}
			pdata->oldduplex = phydev->duplex;
		}

		/* FLOW ctrl operation */
		if (phydev->pause || phydev->asym_pause) {
			if (pdata->flow_ctrl != pdata->oldflow_ctrl)
				DWC_ETH_QOS_configure_flow_ctrl(pdata);
		}

		if (phydev->speed != pdata->speed) {
			new_state = 1;
			switch (phydev->speed) {
			case SPEED_1000:
				hw_if->set_gmii_speed();
				break;
			case SPEED_100:
				hw_if->set_mii_speed_100();
				break;
			case SPEED_10:
				hw_if->set_mii_speed_10();
				break;
			}
			pdata->speed = phydev->speed;
		}

		if (!pdata->oldlink) {
			new_state = 1;
			pdata->oldlink = 1;
		}
	} else if (pdata->oldlink) {
		new_state = 1;
		pdata->oldlink = 0;
		pdata->speed = 0;
		pdata->oldduplex = -1;
	}

	if (new_state)
		phy_print_status(phydev);

#ifdef DWC_ETH_QOS_CONFIG_EEE
	/* At this stage, it could be need to setup the EEE or adjust some
	 * MAC related HW registers.
	 * */
	pdata->eee_enabled = DWC_ETH_QOS_eee_init(pdata);
#endif

	spin_unlock_irqrestore(&pdata->lock, flags);

	DBGPR_MDIO("<--DWC_ETH_QOS_adjust_link\n");
}

/*!
* \brief API to initialize PHY.
*
* \details This function will initializes the driver's PHY state and attaches
* the PHY to the MAC driver.
*
* \param[in] dev - pointer to net_device structure
*
* \return integer
*
* \retval 0 on success & negative number on failure.
*/

static int DWC_ETH_QOS_init_phy(struct net_device *dev)
{
	struct DWC_ETH_QOS_prv_data *pdata = netdev_priv(dev);
	struct phy_device *phydev = NULL;
	char phy_id_fmt[MII_BUS_ID_SIZE + 3];
	char bus_id[MII_BUS_ID_SIZE];

	DBGPR_MDIO("-->DWC_ETH_QOS_init_phy\n");

	pdata->oldlink = 0;
	pdata->speed = 0;
	pdata->oldduplex = -1;

	snprintf(bus_id, MII_BUS_ID_SIZE, "dwc_phy-%x", pdata->bus_id);

	snprintf(phy_id_fmt, MII_BUS_ID_SIZE + 3, PHY_ID_FMT, bus_id,
		 pdata->phyaddr);

	DBGPR_MDIO("trying to attach to %s\n", phy_id_fmt);

	phydev = phy_connect(dev, phy_id_fmt, &DWC_ETH_QOS_adjust_link, 
			     pdata->interface);

	if (IS_ERR(phydev)) {
		printk(KERN_ALERT "%s: Could not attach to PHY\n", dev->name);
		return PTR_ERR(phydev);
	}

	if (phydev->phy_id == 0) {
		phy_disconnect(phydev);
		return -ENODEV;
	}

	if (pdata->interface == PHY_INTERFACE_MODE_GMII) {
		phydev->supported = PHY_GBIT_FEATURES;
#ifdef DWC_ETH_QOS_CERTIFICATION_PKTBURSTCNT_HALFDUPLEX
		phydev->supported &= ~SUPPORTED_1000baseT_Full;
#endif
	} else if ((pdata->interface == PHY_INTERFACE_MODE_MII) ||
		(pdata->interface == PHY_INTERFACE_MODE_RMII)) {
		phydev->supported = PHY_BASIC_FEATURES;
	}

#ifndef DWC_ETH_QOS_CONFIG_PGTEST
	phydev->supported |= (SUPPORTED_Pause | SUPPORTED_Asym_Pause);
#endif

    /* Lets Make the code support for both 100M and Giga bit */
//#ifdef DWC_ETH_QOS_CONFIG_PGTEST
//	phydev->supported = PHY_BASIC_FEATURES;
//#endif

	phydev->advertising = phydev->supported;

	DBGPR_MDIO("%s: attached to PHY (UID 0x%x) Link = %d\n", dev->name,
	      phydev->phy_id, phydev->link);

	pdata->phydev = phydev;

	phy_start(pdata->phydev);

	DBGPR_MDIO("<--DWC_ETH_QOS_init_phy\n");

	return 0;
}

/*!
* \brief API to register mdio.
*
* \details This function will allocate mdio bus and register it
* phy layer.
*
* \param[in] dev - pointer to net_device structure
*
* \return 0 on success and -ve on failure.
*/
int DWC_ETH_QOS_mdio_register(struct net_device *dev)
{
	struct DWC_ETH_QOS_prv_data *pdata = netdev_priv(dev);
	struct mii_bus *new_bus = NULL;
	int phyaddr = 0;
	unsigned short phy_detected = 0;
	int ret = Y_SUCCESS;

	DBGPR_MDIO("-->DWC_ETH_QOS_mdio_register\n");

	/* find the phy ID or phy address which is connected to our MAC */
	for (phyaddr = 0; phyaddr < 32; phyaddr++) {
		int phy_reg_read_status, mii_status;

		phy_reg_read_status =
		    DWC_ETH_QOS_mdio_read_direct(pdata, phyaddr, MII_BMSR,
			&mii_status);
		if (phy_reg_read_status == 0) {
			if (mii_status != 0x0000 && mii_status != 0xffff) {
				printk(KERN_ALERT "%s: Phy detected at"\
				    " ID/ADDR %d\n", DEV_NAME, phyaddr);
				phy_detected = 1;
				break;
			}
		} else if (phy_reg_read_status < 0) {
			printk(KERN_ALERT "%s: Error reading the phy register"\
			    " MII_BMSR for phy ID/ADDR %d\n",
			    DEV_NAME, phyaddr);
		}
	}
	if (!phy_detected) {
		printk(KERN_ALERT "%s: No phy could be detected\n", DEV_NAME);
		return -ENOLINK;
	}
	pdata->phyaddr = phyaddr;
	pdata->bus_id = 0x1;

	DBGPHY_REGS(pdata);

	new_bus = mdiobus_alloc();
	if (new_bus == NULL) {
		printk(KERN_ALERT "Unable to allocate mdio bus\n");
		return -ENOMEM;
	}

	new_bus->name = "dwc_phy";
	new_bus->read = DWC_ETH_QOS_mdio_read;
	new_bus->write = DWC_ETH_QOS_mdio_write;
	new_bus->reset = DWC_ETH_QOS_mdio_reset;
	snprintf(new_bus->id, MII_BUS_ID_SIZE, "%s-%x", new_bus->name,
		 pdata->bus_id);
	new_bus->priv = dev;
	new_bus->phy_mask = 0;
	new_bus->parent = &pdata->pdev->dev;
	ret = mdiobus_register(new_bus);
	if (ret != 0) {
		printk(KERN_ALERT "%s: Cannot register as MDIO bus\n",
		    new_bus->name);
		mdiobus_free(new_bus);
		return ret;
	}
	pdata->mii = new_bus;

	ret = DWC_ETH_QOS_init_phy(dev);
	if (unlikely(ret)) {
		printk(KERN_ALERT "Cannot attach to PHY (error: %d)\n", ret);
		goto err_out_phy_connect;
	}

	DBGPR_MDIO("<--DWC_ETH_QOS_mdio_register\n");

	return ret;

 err_out_phy_connect:
	DWC_ETH_QOS_mdio_unregister(dev);
	return ret;
}

/*!
* \brief API to unregister mdio.
*
* \details This function will unregister mdio bus and free's the memory
* allocated to it.
*
* \param[in] dev - pointer to net_device structure
*
* \return void
*/

void DWC_ETH_QOS_mdio_unregister(struct net_device *dev)
{
	struct DWC_ETH_QOS_prv_data *pdata = netdev_priv(dev);

	DBGPR_MDIO("-->DWC_ETH_QOS_mdio_unregister\n");

	if (pdata->phydev) {
		phy_stop(pdata->phydev);
		phy_disconnect(pdata->phydev);
		pdata->phydev = NULL;
	}

	mdiobus_unregister(pdata->mii);
	pdata->mii->priv = NULL;
	mdiobus_free(pdata->mii);
	pdata->mii = NULL;

	DBGPR_MDIO("<--DWC_ETH_QOS_mdio_unregister\n");
}
