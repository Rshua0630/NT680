/**\file
 *  To distinguish different manufacture
 * \internal
 * ----------------------------REVISION HISTORY-----------------------------
 * NVT			15/Apr/2007			Created
 */

#ifndef M_INTERNAL_PHY_H
#define M_INTERNAL_PHY_H 1

/*	Comment
 *	SynopGMAC can support up to 32 phys
 */
enum GMACPhyBase {
	PHY0  = 0, /*The device can support 32 phys, but we use first phy only*/
	PHY1  = 1,
	PHY2  = 2,
	PHY3  = 3,
	PHY31 = 31,
};

/*	Comment
 *	Name and default base of PHY
 */
#ifdef CONFIG_NVT_MICREL_PHY_SUPPORT
#define NVT_PHY_VENDOR  "MICREL"
#define DEFAULT_PHY_BASE PHY0		/*Micrel Phy address on EV Board*/
#elif defined(CONFIG_NVT_RTL_PHY_SUPPORT)
#define NVT_PHY_VENDOR  "Realtek 8201"
#define DEFAULT_PHY_BASE PHY3		/*Realtek Phy address*/
#elif defined(CONFIG_NVT_ICPLUS_PHY_SUPPORT)
#define NVT_PHY_VENDOR  "IC Plus 101 ALF"
#define DEFAULT_PHY_BASE PHY1           /*IC Plus Phy address*/
#elif defined(CONFIG_NVT_INTERNAL_PHY)
#define NVT_PHY_VENDOR  "NVT Internal PHY"
#define DEFAULT_PHY_BASE PHY0		/*NVT Phy address*/
#endif

extern raw_spinlock_t clock_gen_lock;

/*Form synopGMAC_Dev.h*/
void synopGMAC_set_nvt_phy_mii(struct synop_device *gmacdev);
s32 synopGMAC_nvt_phy_disable_isolate(struct synop_device *gmacdev);
s32 synopGMAC_nvt_phy_power_down(struct synop_device *gmacdev);
s32 synopGMAC_nvt_phy_power_on(struct synop_device *gmacdev);
void synopGMAC_nvt_phy_reset_RX_statemach(struct synop_device *gmacdev);
u16 synopGMAC_get_nvt_phy_link_trigger_level(struct synop_device *gmacdev);
void synopGMAC_set_nvt_phy_link_trigger_level(struct synop_device *gmacdev);

void synopGMAC_eq_reset_function(u32);
void synopGMAC_eq_set_function(void);
void nvt_internal_PHY_inverse_clock(struct synop_device *gmacdev);
void nvt_internal_PHY_iounmap_all(void);
void nvt_internal_PHY_reset(struct synop_device *gmacdev);
int nvt_internal_PHY_ioremap_all(void);
int nvt_nternal_PHY_display_status(struct synop_device *gmacdev);

void SysPlat_Init_Ethernet_Phy(struct synop_device *gmacdev, int Init_boot);
#endif
