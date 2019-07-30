#ifndef _USBHOST_EHCI_REG_H
#define _USBHOST_EHCI_REG_H

//#include    "constant.h"

// USB Command
#define     EHCI_USBCMD_RUN                     0x00000001      // Run/Stop. When set to a "1", the host controller proceeds with execution of the schedule
#define     EHCI_USBCMD_RESET                   0x00000002      // Host controller reset. This control bit is used by the software to reset the host controller
#define     EHCI_USBCMD_PSCH_EN                 0x00000010      // Periodic schedule enable. This bit controls whether the host controller skips the periodic schedule processing
#define     EHCI_USBCMD_ASCH_EN                 0x00000020      // Asynchronous schedule enable. This bit controls whether the host controller skips the asynchronous schedule processing
#define     EHCI_USBCMD_INT_OAAD                0x00000040      // Interrupt on asynchronous advance doorbell. This bit is used as a doorbell by the software to instruct the host controller to issue an interrupt the next time it advances the asynchronous schedule
#define     EHCI_USBCMD_ASYNC_PK_EN             0x00000800      // Asynchronous schedule park mode enable. Software uses this bit to enable or disable the park mode. When this bit is set to "1", the park mode is enabled

#define     EHCI_USBCMD_FRLSIZE_1024            0               // 00: 1024 elements (4096 bytes, default value)
#define     EHCI_USBCMD_FRLSIZE_512             1               // 01: 512 elements (2048 bytes)
#define     EHCI_USBCMD_FRLSIZE_256             2               // 10: 256 elements (1024 bytes)
// USB Status
#define     EHCI_USBSTA_USB_INT                 0x00000001      // The host controller sets this bit to "1" upon completion of an USB transaction
#define     EHCI_USBSTA_USBERR_INT              0x00000002      // The host controller sets this bit to "1" when completion of an USB transaction results in an error
#define     EHCI_USBSTA_PO_CHG_DET              0x00000004      // The host controller sets this bit to "1" when any port has a change bit transition from "0" to "1"
#define     EHCI_USBSTA_FRL_ROL                 0x00000008      // The host controller sets this bit to "1" when the frame list index rolls over from its maximum value to zero
#define     EHCI_USBSTA_H_SYSERR                0x00000010      // The host controller sets this bit to "1" when a serious error occurs during host system access involving the host controller module
#define     EHCI_USBSTA_INT_OAA                 0x00000020      // This status bit indicates the assertion of the interrupt on async advance doorbell
#define     EHCI_USBSTA_HALTED                  0x00001000      // This bit is "0" whenever the run/stop bit is "1". The host controller sets this bit to "1" after it has stopped executing as a result of the run/stop bit being set to 0
#define     EHCI_USBSTA_RECLAMATION             0x00002000      // This is a read only status bit, which is used to detect an empty asynchronous schedule
#define     EHCI_USBSTA_PSCH                    0x00004000      // Periodic schedule status. This bit reports the actual status of the periodic schedule
#define     EHCI_USBSTA_ASCH                    0x00008000      // Asynchronous schedule status. This bit reports the actual status of the asynchronous schedule
// USB Interrupt Enable Register
#define     EHCI_USBINT_USBCOMPLT               0x00000001      // The host controller sets this bit to "1" upon completion of an USB transaction
#define     EHCI_USBINT_USBERR                  0x00000002      // The host controller sets this bit to "1" when completion of an USB transaction results in an error
#define     EHCI_USBINT_PO_CHG_DET              0x00000004      // The host controller sets this bit to "1" when any port has a change bit transition from "0" to "1"
#define     EHCI_USBINT_FRL_ROL                 0x00000008      // The host controller sets this bit to "1" when the frame list index rolls over from its maximum value to zero
#define     EHCI_USBINT_H_SYSERR                0x00000010      // The host controller sets this bit to "1" when a serious error occurs during host system access involving the host controller module
#define     EHCI_USBINT_OAA                     0x00000020      // This status bit indicates the assertion of the interrupt on async advance doorbell
// Port Status and Control Register
#define     EHCI_PORTSC_CONN_STS                0x00000001      // Current connect status
#define     EHCI_PORTSC_CONN_CHG                0x00000002      // Connect status change
#define     EHCI_PORTSC_PO_EN                   0x00000004      // Port enable/disable
#define     EHCI_PORTSC_PO_EN_CHG               0x00000008      // Port enable/disable change
#define     EHCI_PORTSC_F_PO_RESM               0x00000040      // Force port resume
#define     EHCI_PORTSC_PO_SUSP                 0x00000080      // Fort suspend
#define     EHCI_PORTSC_PO_RESET                0x00000100      // port reset

#define     EHCI_PORTSC_PORTTST_DISABLE         0               // 0000    Test mode is not enable
#define     EHCI_PORTSC_PORTTST_J_STA           1               // 0001    Test J-state
#define     EHCI_PORTSC_PORTTST_K_STA           2               // 0010    Test K-sate
#define     EHCI_PORTSC_PORTTST_SE0_NAK         3               // 0011    Test SE0_NAK
#define     EHCI_PORTSC_PORTTST_PACKET          4               // 0100    Test Packet
#define     EHCI_PORTSC_PORTTST_ENABLE          5               // 0101    Test FORCE_ENABLE

#define     EHCI_PORTSC_PORTTST_PKDONE          0x00100000      // TEST PACKET DONE



extern  UINT32  usbehci_getCAPLENGTH(void);
extern  UINT32  usbehci_getHCIVERSION(void);
extern  UINT32  usbehci_getHCSPARAMS_NPorts(void);
extern  UINT32  usbehci_getHCCPARAMS_Async_Sch_Park_En(void);
extern  UINT32  usbehci_getHCCPARAMS_Prog_FRListFlag_En(void);

extern  void    usbehci_setUSBCMD(UINT32 uiRegData);
extern  UINT32  usbehci_getUSBCMD(void);
extern  void    usbehci_setUSBCMD_RunStop(UINT32 uiIfRun);
extern  void    usbehci_setUSBCMD_Reset(UINT32 uiIfReset);
extern  void    usbehci_setUSBCMD_FRListSize(UINT32 uiFRListSize);
extern  UINT32  usbehci_getUSBCMD_FRListSize(void);
extern  void    usbehci_setUSBCMD_PeriSch_En(UINT32 uiIfEnable);
extern  void    usbehci_setUSBCMD_AsyncSch_En(UINT32 uiIfEnable);
extern  void    usbehci_setUSBCMD_IntOnAsyncDoorbell_En(UINT32 uiIfEnable);
extern  void    usbehci_setUSBCMD_AsyncSchParkCnt(UINT32 uiParkCnt);
extern  UINT32  usbehci_getUSBCMD_AsyncSchParkCnt(void);
extern  void    usbehci_setUSBCMD_AsyncSchPark_En(UINT32 uiIfEnable);
extern  void    usbehci_setUSBCMD_IntThreshold(UINT32 uiParkCnt);
extern  UINT32  usbehci_getUSBCMD_IntThreshold(void);

extern  UINT32  usbehci_getUSBSTS(void);
extern  void    usbehci_clrUSBSTS(UINT32 uiStatus);
extern  UINT32  usbehci_getUSBINTEnable(void);
extern  void    usbehci_setUSBINTEnable(UINT32 uiIntEn);
extern  void    usbehci_clrUSBINTEnable(UINT32 uiIntEn);

extern  void    usbehci_setFRINDEX(UINT32 uiFRIndex);
extern  UINT32  usbehci_getFRINDEX(void);
extern  void    usbehci_setPeriFRListBase(UINT32 uiBaseAddr);
extern  UINT32  usbehci_getPeriFRListBase(void);
extern  void    usbehci_setAsyncListAddr(UINT32 uiListAddr);
extern  UINT32  usbehci_getAsyncListAddr(void);

extern  UINT32  usbehci_getPORTSC(void);
extern  void    usbehci_setPORTSC(UINT32 uiSetBits);
extern  void    usbehci_clrPORTSC(UINT32 uiClrBits);
extern  UINT32  usbehci_getPORTSC_LINESTS(void);
extern  void    usbehci_setPORTSC_PORTTST(UINT32 uiTstMode);
extern  UINT32  usbehci_getPORTSC_PORTTST(void);

extern  void    usbehci_setMISC_PhySuspend(UINT32 uiIfSusp);
extern  void    usbehci_setMISC_EOF1Time(UINT32 uiTime);
extern  void    usbehci_setMISC_EOF2Time(UINT32 uiTime);

extern  void     USBHostPowerOnInit(char *pBuf);

#endif  //_USBHOST_EHCI_REG_H
