/**\file
 *  To distinguish different manufacture
 * \internal
 * ----------------------------REVISION HISTORY-----------------------------
 * NVT			15/Apr/2007			Created
 */


#ifndef M_INTERNAL_PHY_H
#define M_INTERNAL_PHY_H 1

/*
 *	SynopGMAC can support up to 32 phys
 */
enum GMACPhyBase
{
	PHY0  = 0,			//The device can support 32 phys, but we use first phy only
	PHY1  = 1,
	PHY2  = 2,
	PHY3  = 3,
	PHY31 = 31,
};

/*
 *	Name and default base of PHY
 */
#ifdef CONFIG_NVT_MICREL_PHY_SUPPORT
#define NVT_PHY_VENDOR  "MICREL"
#define DEFAULT_PHY_BASE PHY0		// Micrel Phy address on EV Board
#elif defined(CONFIG_NVT_RTL_PHY_SUPPORT)
#define NVT_PHY_VENDOR  "Realtek 8201"
#define DEFAULT_PHY_BASE PHY3		// Realtek Phy address
#elif defined(CONFIG_NVT_ICPLUS_PHY_SUPPORT)
#define NVT_PHY_VENDOR  "IC Plus 101 ALF"
#define DEFAULT_PHY_BASE PHY1           // IC Plus Phy address
#elif defined(CONFIG_NVT_INTERNAL_PHY)
#define NVT_PHY_VENDOR  "NVT Internal PHY"
#define DEFAULT_PHY_BASE PHY0		// NVT Phy address
#endif

//Form synopGMAC_Dev.h
void synopGMAC_set_nvt_phy_mii(synopGMACdevice * gmacdev);
s32 synopGMAC_nvt_phy_disable_isolate(synopGMACdevice * gmacdev);
s32 synopGMAC_nvt_phy_power_down(synopGMACdevice * gmacdev);
s32 synopGMAC_nvt_phy_power_on(synopGMACdevice * gmacdev);
void synopGMAC_nvt_phy_reset_RX_statemach(synopGMACdevice * gmacdev);
u16 synopGMAC_get_nvt_phy_link_trigger_level(synopGMACdevice * gmacdev);
void synopGMAC_set_nvt_phy_link_trigger_level(synopGMACdevice * gmacdev);


void synopGMAC_eq_reset_function(u32);
void synopGMAC_eq_set_function(void);
void nvt_internal_PHY_inverse_clock(synopGMACdevice * gmacdev);
void nvt_internal_PHY_iounmap_all(void);
void nvt_internal_PHY_reset(synopGMACdevice * gmacdev);
int nvt_internal_PHY_ioremap_all(void);
int nvt_nternal_PHY_display_status(synopGMACdevice * gmacdev);

void SysPlat_Init_Ethernet_Phy(synopGMACdevice * gmacdev, int Init_boot);
u32 ioremap_read(u32 address);

#ifdef CONFIG_NVT_ETH_PHY_POWER_DOWN
void nvt_internal_PHY_power_done(void);
s32 nvt_internal_PHY_check_power_status(void);
#endif
#endif

/* Form m_internal_Phy.h */
extern u32 NVT_Phy_Init_Done;
extern u32 NVT_Phy_Init_STR;
extern struct timer_list NVT_Phy_InitTimer, NVT_Phy_InitTimer2;
