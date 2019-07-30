#ifndef _USBOTG987_H_
#define _USBOTG987_H_


//---------------------------------------------------------------------
//    Set/Get all host register information
//---------------------------------------------------------------------
// Host Controller Capability Registers
#define REG_HC_CAPLENG                  1
//#define REG_HC_VERSION                2
#define REG_HC_HCSPARAMS                3
#define REG_HC_HCCPARAMS                4
#define REG_HC_HCSPPORTROUTE            5   // do not support
// Host Controller Operational Registers
#define REG_HC_USBCMMD                  6
#define REG_HC_USBSTS                   7
#define REG_HC_USBINTR                  8
#define REG_HC_FRAME_INDEX              9
#define REG_HC_CTRLDSSEGMENT            10  // do not support
#define REG_HC_PERD_BASEADDR            11
#define REG_HC_CURR_LISTADDR            12
#define REG_HC_CONFIGFLAG               13
#define REG_HC_PORTSTATUS               14

#define REG_HC_PHYSUSP                  15
#define REG_HC_EOF1TIME                 16
#define REG_HC_EOF2TIME                 17
#define REG_HC_ASYNCSCH_SLEEPTIME       18

//---------------------------------------------------------------------
//    Set/Get all otg register information
//---------------------------------------------------------------------
#define REG_OTG_CTRL_SETBIT             1
#define REG_OTG_CTRL_CLRBIT             2
#define REG_OTG_STATUS                  3
#define REG_OTG_CURR_ROLE               4
#define REG_OTG_CURR_ID                 5
#define REG_OTG_HOST_SPEED              6

//---------------------------------------------------------------------
//    0x40    HC Misc. Register
//---------------------------------------------------------------------
#define USBHC_HOSTPHY_SUSP                  0x00000040      // Host transceiver suspend mode
//---------------------------------------------------------------------
//    0x80    OTG Control/Status Register
//---------------------------------------------------------------------
#define USBOTG_CTRL_B_BUS_REQ               0x00000001      // B-device bus request
#define USBOTG_CTRL_B_HNP_EN                0x00000002      // Inform B-device that it has been enabled to perform HNP
#define USBOTG_CTRL_B_DSCHRG_VBUS           0x00000004      // B-device discharge Vbus
#define USBOTG_CTRL_A_BUS_REQ               0x00000010      // A-device bus request
#define USBOTG_CTRL_A_BUS_DROP              0x00000020      // A-device bus drop
#define USBOTG_CTRL_A_SET_B_HNP_EN          0x00000040      // Indicates to A-device that the HNP function of B-device had been enabled
#define USBOTG_CTRL_A_SRP_DET_EN            0x00000080      // A-device SRP detection enable
#define USBOTG_CTRL_A_SRP_RESP_MASK         0x00000100      // SRP response type
#define USBOTG_CTRL_A_SRP_RESP_VBUS         0x00000000      // 0: A-device responds to Vbus pulsing
#define USBOTG_CTRL_A_SRP_RESP_DATALINE     0x00000100      // 1: A-device responds to data-line pulsing
#define USBOTG_CTRL_ID_FLTSEL_MASK          0x00000200      // Select a timer to filter out noise of ID from UTMI+
#define USBOTG_CTRL_ID_FLTSEL_3MS           0x00000000      // 0: approximated to 3ms
#define USBOTG_CTRL_ID_FLTSEL_4MS           0x00000200      // 1: approximated to 4ms
#define USBOTG_CTRL_Vbus_FLTSEL_MASK        0x00000400      // Select a timer to filter out noise of Vbus_VLD from UTMI+
#define USBOTG_CTRL_Vbus_FLTSEL_135S        0x00000000      // 0: approximated to 135 s
#define USBOTG_CTRL_Vbus_FLTSEL_472S        0x00000400      // 1: approximated to 472 s
#define USBOTG_CTRL_HDISCON_FLTSEL_MASK     0x00000800      // Select a timer to filter out noise of HDISCON from UTMI+
#define USBOTG_CTRL_HDISCON_FLTSEL_135S     0x00000000      // 0: approximated to 135 s
#define USBOTG_CTRL_HDISCON_FLTSEL_270S     0x00000800      // 1: approximated to 270 s

#define USBOTG_STAT_B_SESS_END              0x00010000      // B-device session end
#define USBOTG_STAT_B_SESS_VLD              0x00020000      // B-device session valid
#define USBOTG_STAT_A_SESS_VLD              0x00040000      // A-device session valid
#define USBOTG_STAT_A_VBUS_VLD              0x00080000      // A-device Vbus valid
#define USBOTG_STAT_CROLE_HOST              0               // 0: host
#define USBOTG_STAT_CROLE_DEV               1               // 1: device
#define USBOTG_STAT_ID_A_DEV                0               // 0: A-device
#define USBOTG_STAT_ID_B_DEV                1               // 1: B-device
#define USBOTG_STAT_HOST_SPD_FS             0               // 00: FS
#define USBOTG_STAT_HOST_SPD_LS             1               // 01: LS
#define USBOTG_STAT_HOST_SPD_HS             2               // 10: HS

#define USBOTG_CTRL_EOF_LEGACY              0x80000000      // EOF legacy behavior(0: patched behavior, 1: legacy behavior)
//---------------------------------------------------------------------
//  0x84~0x88    OTG Interrupt status\enable Register
//---------------------------------------------------------------------
#define USBOTG_INT_B_SRP_DN                 0x00000001
#define USBOTG_INT_A_SRP_DET                0x00000010
#define USBOTG_INT_A_VBUS_ERR               0x00000020
#define USBOTG_INT_B_SESS_END               0x00000040
#define USBOTG_INT_RLCHG                    0x00000100
#define USBOTG_INT_IDCHG                    0x00000200
#define USBOTG_INT_OVC                      0x00000400
#define USBOTG_INT_BPLGRMV                  0x00000800
#define USBOTG_INT_APLGRMV                  0x00001000
//---------------------------------------------------------------------
//  0xC0~0xC4    Global Mask of HC/OTG/DEV Interrupt Mask\Status
//---------------------------------------------------------------------
#define USB_INTTYP_DEVG                     0x00000001
#define USB_INTTYP_OTG                      0x00000002
#define USB_INTTYP_HOST                     0x00000004

#define USB_INTTYP_DEV_G0                   0x00000001
#define USB_INTTYP_DEV_G1                   0x00000002
#define USB_INTTYP_DEV_G2                   0x00000004

#define USB_INT_POLARITY_ACTHIGH            0x00000000
#define USB_INT_POLARITY_ACTLOW             0x00000008
//---------------------------------------------------------------------
//  0x100    Device Main Control Register
//---------------------------------------------------------------------
#define USBDEV_MAINCTL_HALF_SPEED           0x00000002      // Half speed enable





/*-------------------------------------------------
** USBOTG API
---------------------------------------------------*/
// USB OTG host APIs
extern  void    usbhostc_setReg(UINT32 uiCmmdType, UINT32 uiSetData);
extern  UINT32  usbhostc_getReg(UINT32 uiGetInfo);
// USB OTG controller APIs
extern  void    usbotgc_setReg(UINT32 uiCmmdType, UINT32 uiSetData);
extern  UINT32  usbotgc_getReg(UINT32 uiGetInfo);
// USB OTG device APIs

extern  void    usb_intisr(void);

extern  UINT32  usb_getHostIntStatus(void);
extern  void    usb_clrHostIntStatus(UINT32);
#endif
