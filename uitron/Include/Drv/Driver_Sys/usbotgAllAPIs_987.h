#ifndef __USBOTGAllAPIS987_H
#define __USBOTGAllAPIS987_H

#include    "StrgDef.h"
#include     "usbotg987.h"
#include     "usbhost_EHCI_reg.h"

#define OTGC_Get(cmmd)                              usbotgc_getReg(cmmd)
#define OTGC_Set(cmmd,data)                       usbotgc_setReg(cmmd,data)
#define Host20_Get(cmmd)                          usbhostc_getReg(cmmd)
#define Host20_Set(cmmd,data)                     usbhostc_setReg(cmmd,data)

//-------------------------------------------------------------------------
// OTG APIs define
//-------------------------------------------------------------------------
//    0x80    OTG Control/Status Register
#define mdwOTGC_Control_CROLE_Rd()                  OTGC_Get(REG_OTG_CURR_ROLE)     //0:Host 1:Peripheral
#define mdwOTGC_Control_ID_Rd()                      OTGC_Get(REG_OTG_CURR_ID)       //0:A-Device 1:B-Device
#define mdwOTGC_Control_Rd()                      OTGC_Get(REG_OTG_STATUS)
#define mdwOTGC_Control_Speed_Rd()                  OTGC_Get(REG_OTG_HOST_SPEED)

#define mdwOTGC_Control_Get()                      OTGC_Get(REG_OTG_STATUS)
#define mdwOTGC_Control_Set(bit)                  OTGC_Set(REG_OTG_CTRL_SETBIT,bit)
#define mdwOTGC_Control_Clr(bit)                  OTGC_Set(REG_OTG_CTRL_CLRBIT,bit)

//~B Function
#define mdwOTGC_Control_B_BUS_REQ_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_B_BUS_REQ)
#define mdwOTGC_Control_B_BUS_REQ_Set()            mdwOTGC_Control_Set(USBOTG_CTRL_B_BUS_REQ)
#define mdwOTGC_Control_B_BUS_REQ_Clr()            mdwOTGC_Control_Clr(USBOTG_CTRL_B_BUS_REQ)
#define mdwOTGC_Control_B_HNP_EN_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_B_HNP_EN)
#define mdwOTGC_Control_B_HNP_EN_Set()              mdwOTGC_Control_Set(USBOTG_CTRL_B_HNP_EN)
#define mdwOTGC_Control_B_HNP_EN_Clr()            mdwOTGC_Control_Clr(USBOTG_CTRL_B_HNP_EN)
#define mdwOTGC_Control_B_DSCHG_VBUS_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_B_DSCHRG_VBUS)
#define mdwOTGC_Control_B_DSCHG_VBUS_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_B_DSCHRG_VBUS)
#define mdwOTGC_Control_B_DSCHG_VBUS_Clr()           mdwOTGC_Control_Clr(USBOTG_CTRL_B_DSCHRG_VBUS)
//~A Function
#define mdwOTGC_Control_A_BUS_REQ_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_A_BUS_REQ)
#define mdwOTGC_Control_A_BUS_REQ_Set()              mdwOTGC_Control_Set(USBOTG_CTRL_A_BUS_REQ)
#define mdwOTGC_Control_A_BUS_REQ_Clr()              mdwOTGC_Control_Clr(USBOTG_CTRL_A_BUS_REQ)
#define mdwOTGC_Control_A_BUS_DROP_Rd()              (mdwOTGC_Control_Get() & USBOTG_CTRL_A_BUS_DROP)
#define mdwOTGC_Control_A_BUS_DROP_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_A_BUS_DROP)
#define mdwOTGC_Control_A_BUS_DROP_Clr()          mdwOTGC_Control_Clr(USBOTG_CTRL_A_BUS_DROP)
#define mdwOTGC_Control_A_SET_B_HNP_EN_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_A_SET_B_HNP_EN)
#define mdwOTGC_Control_A_SET_B_HNP_EN_Set()      mdwOTGC_Control_Set(USBOTG_CTRL_A_SET_B_HNP_EN)
#define mdwOTGC_Control_A_SET_B_HNP_EN_Clr()      mdwOTGC_Control_Clr(USBOTG_CTRL_A_SET_B_HNP_EN)
#define mdwOTGC_Control_A_SRP_DET_EN_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_A_SRP_DET_EN)
#define mdwOTGC_Control_A_SRP_DET_EN_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_A_SRP_DET_EN)
#define mdwOTGC_Control_A_SRP_DET_EN_Clr()          mdwOTGC_Control_Clr(USBOTG_CTRL_A_SRP_DET_EN)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Rd()      (mdwOTGC_Control_Get() & USBOTG_CTRL_A_SRP_RESP_MASK)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Set(b)      mdwOTGC_Control_Set(USBOTG_CTRL_A_SRP_RESP_MASK)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Clr()      mdwOTGC_Control_Clr(USBOTG_CTRL_A_SRP_RESP_MASK)
#define mdwOTGC_Control_VBUS_FLT_SEL_Rd()          (mdwOTGC_Control_Get() & USBOTG_CTRL_Vbus_FLTSEL_MASK)
#define mdwOTGC_Control_VBUS_FLT_SEL_Set()          mdwOTGC_Control_Set(USBOTG_CTRL_Vbus_FLTSEL_MASK)
#define mdwOTGC_Control_VBUS_FLT_SEL_Clr()          mdwOTGC_Control_Clr(USBOTG_CTRL_Vbus_FLTSEL_MASK)
#define mdwOTGC_Control_EOF_Legacy_Set()            mdwOTGC_Control_Set(USBOTG_CTRL_EOF_LEGACY)
#define mdwOTGC_Control_EOF_Legacy_Clr()            mdwOTGC_Control_Clr(USBOTG_CTRL_EOF_LEGACY)

#define mdwOTGC_Control_B_SESS_END_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_B_SESS_END)
#define mdwOTGC_Control_B_SESS_VLD_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_B_SESS_VLD)
#define mdwOTGC_Control_A_SESS_VLD_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_A_SESS_VLD)
#define mdwOTGC_Control_A_VBUS_VLD_Rd()              (mdwOTGC_Control_Get() & USBOTG_STAT_A_VBUS_VLD)

#define A_SRP_RESP_TYPE_VBUS                      USBOTG_CTRL_A_SRP_RESP_VBUS
#define A_SRP_RESP_TYPE_DATA_LINE                 USBOTG_CTRL_A_SRP_RESP_DATALINE

#define mwOTG20_Control_HOST_SPD_TYP_Rd()          mdwOTGC_Control_Speed_Rd()
#define mwOTG20_Control_A_BUS_REQ_Set()              mdwOTGC_Control_Set(USBOTG_CTRL_A_BUS_REQ)


//--------------------------------------------------------------------------
//  0x84~0x88     OTG Interrupt Enable & Status Register
//--------------------------------------------------------------------------
#define OTGC_INT_A_TYPE        (USBOTG_INT_A_SRP_DET|USBOTG_INT_A_VBUS_ERR|\
								USBOTG_INT_OVC|USBOTG_INT_RLCHG|USBOTG_INT_IDCHG|\
								USBOTG_INT_BPLGRMV|USBOTG_INT_APLGRMV)
#define OTGC_INT_B_TYPE     (USBOTG_INT_B_SRP_DN|USBOTG_INT_A_VBUS_ERR|\
							 USBOTG_INT_OVC|USBOTG_INT_RLCHG|USBOTG_INT_IDCHG)

//-------------------------------------------------------------------------
// Host APIs define
//-------------------------------------------------------------------------
//  0x00    HC Capability Register
#define mwHost20_HCIVersion_Rd()                            usbehci_getHCIVERSION()
#define mwHost20_CapLength_Rd()                                usbehci_getCAPLENGTH()
//    0x04    HC Structural parameters
#define mwHost20_NumPorts_Rd()                                usbehci_getHCSPARAMS_NPorts()
//    0x08    HC Capability parameters
#define mbHost20_ProgrammableFrameListFlag_Rd()                usbehci_getHCCPARAMS_Prog_FRListFlag_En()
//  0x10    HC USB Command Register
#define mwHost20_USBCMD_Get()                                usbehci_getUSBCMD()
#define mwHost20_USBCMD_IntThreshold_Rd()                    usbehci_getUSBCMD_IntThreshold()
#define mbHost20_USBCMD_IntThreshold_Set(bValue)            usbehci_setUSBCMD_IntThreshold(bValue)

#define mbHost20_USBCMD_ParkMode_Rd()                       (mwHost20_USBCMD_Get() & EHCI_USBCMD_ASYNC_PK_EN)
#define mbHost20_USBCMD_ParkMode_Set()                         usbehci_setUSBCMD_AsyncSchPark_En(TRUE)
#define mbHost20_USBCMD_ParkMode_Clr()                        usbehci_setUSBCMD_AsyncSchPark_En(FALSE)
#define mbHost20_USBCMD_ParkMode_CNT_Rd()                   usbehci_getUSBCMD_AsyncSchParkCnt()
#define mbHost20_USBCMD_ParkMode_CNT_Set(bValue)             usbehci_setUSBCMD_AsyncSchParkCnt(bValue)
#define mbHost20_USBCMD_InterruptOnAsync_Rd()               (mwHost20_USBCMD_Get() & EHCI_USBCMD_INT_OAAD)
#define mbHost20_USBCMD_InterruptOnAsync_Set()                 usbehci_setUSBCMD_IntOnAsyncDoorbell_En(TRUE)
#define mbHost20_USBCMD_InterruptOnAsync_Clr()                usbehci_setUSBCMD_IntOnAsyncDoorbell_En(FALSE)
#define mbHost20_USBCMD_AsynchronousEnable_Rd()             (mwHost20_USBCMD_Get() & EHCI_USBCMD_ASCH_EN)
#define mbHost20_USBCMD_AsynchronousEnable_Set()             usbehci_setUSBCMD_AsyncSch_En(TRUE)
#define mbHost20_USBCMD_AsynchronousEnable_Clr()            usbehci_setUSBCMD_AsyncSch_En(FALSE)
#define mbHost20_USBCMD_PeriodicEnable_Rd()                 (mwHost20_USBCMD_Get() & EHCI_USBCMD_PSCH_EN)
#define mbHost20_USBCMD_PeriodicEnable_Set()                 usbehci_setUSBCMD_PeriSch_En(TRUE)
#define mbHost20_USBCMD_PeriodicEnable_Clr()                usbehci_setUSBCMD_PeriSch_En(FALSE)

#define mbHost20_USBCMD_FrameListSize_Rd()                    usbehci_getUSBCMD_FRListSize()
#define mbHost20_USBCMD_FrameListSize_Set(bValue)             usbehci_setUSBCMD_FRListSize(bValue)
#define HOST20_USBCMD_FrameListSize_1024                    EHCI_USBCMD_FRLSIZE_1024
#define HOST20_USBCMD_FrameListSize_512                     EHCI_USBCMD_FRLSIZE_512
#define HOST20_USBCMD_FrameListSize_256                     EHCI_USBCMD_FRLSIZE_256
#define mbHost20_USBCMD_HCReset_Rd()                        (mwHost20_USBCMD_Get() & EHCI_USBCMD_RESET)
#define mbHost20_USBCMD_HCReset_Set()                        usbehci_setUSBCMD_Reset(TRUE)
#define mbHost20_USBCMD_RunStop_Rd()                        (mwHost20_USBCMD_Get() & EHCI_USBCMD_RUN)
#define mbHost20_USBCMD_RunStop_Set()                        usbehci_setUSBCMD_RunStop(TRUE)
#define mbHost20_USBCMD_RunStop_Clr()                        usbehci_setUSBCMD_RunStop(FALSE)
#define HOST20_Enable                                       0x01
#define HOST20_Disable                                      0x00
//  0x14    HC USB Status Register
#define mwHost20_USBSTS_Rd()                                usbehci_getUSBSTS()
#define mwHost20_USBSTS_Set(wValue)                            usbehci_clrUSBSTS(wValue)
#define mwHost20_USBSTS_Clr(wValue)                            usbehci_clrUSBSTS(wValue)
#define mwHost20_USBSTS_AsynchronousStatus_Rd()                (mwHost20_USBSTS_Rd() & EHCI_USBSTA_ASCH)
#define mwHost20_USBSTS_PeriodicStatus_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_PSCH)
#define mwHost20_USBSTS_Reclamation_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_RECLAMATION)
#define mwHost20_USBSTS_HCHalted_Rd()                        (mwHost20_USBSTS_Rd() & EHCI_USBSTA_HALTED)
#define mwHost20_USBSTS_IntOnAsyncAdvance_Rd()                (mwHost20_USBSTS_Rd() & EHCI_USBSTA_INT_OAA)
#define mwHost20_USBSTS_IntOnAsyncAdvance_Set()                mwHost20_USBSTS_Set(EHCI_USBSTA_INT_OAA)
#define mwHost20_USBSTS_SystemError_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_H_SYSERR)
#define mwHost20_USBSTS_SystemError_Set()                    mwHost20_USBSTS_Set(EHCI_USBSTA_H_SYSERR)
#define mwHost20_USBSTS_FrameRollover_Rd()                    (mwHost20_USBSTS_Rd() & EHCI_USBSTA_FRL_ROL)
#define mwHost20_USBSTS_FrameRollover_Set()                    mwHost20_USBSTS_Set(EHCI_USBSTA_FRL_ROL)
#define mwHost20_USBSTS_PortChangeDetect_Rd()                (mwHost20_USBSTS_Rd() & EHCI_USBSTA_PO_CHG_DET)
#define mwHost20_USBSTS_PortChangeDetect_Set()                mwHost20_USBSTS_Set(EHCI_USBSTA_PO_CHG_DET)
#define mwHost20_USBSTS_USBError_Rd()                        (mwHost20_USBSTS_Rd() & EHCI_USBSTA_USBERR_INT)
#define mwHost20_USBSTS_USBError_Set()                        mwHost20_USBSTS_Set(EHCI_USBSTA_USBERR_INT)
#define mwHost20_USBSTS_CompletionOfTransaction_Rd()        (mwHost20_USBSTS_Rd() & EHCI_USBSTA_USB_INT)
#define mwHost20_USBSTS_CompletionOfTransaction_Set()        mwHost20_USBSTS_Set(EHCI_USBSTA_USB_INT)
//  0x18    HC USB Interrupt Enable Register
#define HOST20_USBINTR_IntOnAsyncAdvance                    EHCI_USBINT_OAA
#define HOST20_USBINTR_SystemError                          EHCI_USBINT_H_SYSERR
#define HOST20_USBINTR_FrameRollover                        EHCI_USBINT_FRL_ROL
#define HOST20_USBINTR_PortChangeDetect                     EHCI_USBINT_PO_CHG_DET
#define HOST20_USBINTR_USBError                             EHCI_USBINT_USBERR
#define HOST20_USBINTR_CompletionOfTransaction              EHCI_USBINT_USBCOMPLT

#define Host20_EnableInt(EnInt)                             usbehci_setUSBINTEnable(EnInt)
#define Host20_DisableInt(DisInt)                           usbehci_clrUSBINTEnable(DisInt)
#define mwHost20_USBINTR_Rd()                                usbehci_getUSBINTEnable()
#define mwHost20_USBINTR_Set(bValue)                        Host20_EnableInt(bValue)

#define mwHost20_USBINTR_IntOnAsyncAdvance_Rd()                (mwHost20_USBINTR_Rd() & HOST20_USBINTR_IntOnAsyncAdvance)
#define mwHost20_USBINTR_IntOnAsyncAdvance_Set()            Host20_EnableInt(HOST20_USBINTR_IntOnAsyncAdvance)
#define mwHost20_USBINTR_IntOnAsyncAdvance_Clr()            Host20_DisableInt(HOST20_USBINTR_IntOnAsyncAdvance)
#define mwHost20_USBINTR_SystemError_Rd()                    (mwHost20_USBINTR_Rd() & HOST20_USBINTR_SystemError)
#define mwHost20_USBINTR_SystemError_Set()                    Host20_EnableInt(HOST20_USBINTR_SystemError)
#define mwHost20_USBINTR_SystemError_Clr()                    Host20_DisableInt(HOST20_USBINTR_SystemError)
#define mwHost20_USBINTR_FrameRollover_Rd()                    (mwHost20_USBINTR_Rd() & HOST20_USBINTR_FrameRollover)
#define mwHost20_USBINTR_FrameRollover_Set()                Host20_EnableInt(HOST20_USBINTR_FrameRollover)
#define mwHost20_USBINTR_FrameRollover_Clr()                Host20_DisableInt(HOST20_USBINTR_FrameRollover)
#define mwHost20_USBINTR_PortChangeDetect_Rd()                (mwHost20_USBINTR_Rd() & HOST20_USBINTR_PortChangeDetect)
#define mwHost20_USBINTR_PortChangeDetect_Set()                Host20_EnableInt(HOST20_USBINTR_PortChangeDetect)
#define mwHost20_USBINTR_PortChangeDetect_Clr()                Host20_DisableInt(HOST20_USBINTR_PortChangeDetect)
#define mwHost20_USBINTR_USBError_Rd()                        (mwHost20_USBINTR_Rd() & HOST20_USBINTR_USBError)
#define mwHost20_USBINTR_USBError_Set()                        Host20_EnableInt(HOST20_USBINTR_USBError)
#define mwHost20_USBINTR_USBError_Clr()                        Host20_DisableInt(HOST20_USBINTR_USBError)
#define mwHost20_USBINTR_CompletionOfTransaction_Rd()        (mwHost20_USBINTR_Rd() & HOST20_USBINTR_CompletionOfTransaction)
#define mwHost20_USBINTR_CompletionOfTransaction_Set()        Host20_EnableInt(HOST20_USBINTR_CompletionOfTransaction)
#define mwHost20_USBINTR_CompletionOfTransaction_Clr()        Host20_DisableInt(HOST20_USBINTR_CompletionOfTransaction)

//  0x1c    HC Frame Index Register
#define mwHost20_FrameIndex_Rd()                            (usbehci_getFRINDEX() & 0x00001FFF)     //Only Read Bit0~Bit12(Skip Bit 13)
#define mwHost20_FrameIndex14Bit_Rd()                       usbehci_getFRINDEX()
#define mwHost20_FrameIndex_Set(wValue)                        usbehci_setFRINDEX(wValue)
//  0x24    HC Periodic Frame List Base Address Register
#define mwHost20_PeriodicBaseAddr_Rd()                        usbehci_getPeriFRListBase()
#define mwHost20_PeriodicBaseAddr_Set(wValue)                usbehci_setPeriFRListBase(wValue)
//  0x28    HC Current Asynchronous List Address Register
#define mwHost20_CurrentAsynchronousAddr_Rd()                usbehci_getAsyncListAddr()
#define mwHost20_CurrentAsynchronousAddr_Set(wValue)        usbehci_setAsyncListAddr(wValue)
//  0x30    HC Port Status and Control Register
#define mwHost20_PORTSC_Rd()                                usbehci_getPORTSC()
#define mwHost20_PORTSC_Set(bit)                            usbehci_setPORTSC(bit)
#define mwHost20_PORTSC_Clr(bit)                            usbehci_clrPORTSC(bit)

#define mwHost20_PORTSC_LineStatus_Rd()                        usbehci_getPORTSC_LINESTS()
#define mwHost20_Misc_LineStatus_Rd()                        usbehci_getPORTSC_LINESTS()
#define mwHost20_PORTSC_PortReset_Rd()                        (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_RESET)
#define mwHost20_PORTSC_PortReset_Set()                        mwHost20_PORTSC_Set(EHCI_PORTSC_PO_RESET)
#define mwHost20_PORTSC_PortReset_Clr()                        mwHost20_PORTSC_Clr(EHCI_PORTSC_PO_RESET)
#define mwHost20_PORTSC_ForceSuspend_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_SUSP)
#define mwHost20_PORTSC_ForceSuspend_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_PO_SUSP)
#define mwHost20_PORTSC_ForceResume_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_F_PO_RESM)
#define mwHost20_PORTSC_ForceResume_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_F_PO_RESM)
#define mwHost20_PORTSC_ForceResume_Clr()                    mwHost20_PORTSC_Clr(EHCI_PORTSC_F_PO_RESM)
#define mwHost20_PORTSC_EnableDisableChange_Rd()            (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_EN_CHG)
#define mwHost20_PORTSC_EnableDisableChange_Set()            mwHost20_PORTSC_Set(EHCI_PORTSC_PO_EN_CHG)
#define mwHost20_PORTSC_EnableDisable_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PO_EN)
#define mwHost20_PORTSC_EnableDisable_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_PO_EN)
#define mwHost20_PORTSC_EnableDisable_Clr()                    mwHost20_PORTSC_Clr(EHCI_PORTSC_PO_EN)
#define mwHost20_PORTSC_ConnectChange_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_CONN_CHG)
#define mwHost20_PORTSC_ConnectChange_Set()                    mwHost20_PORTSC_Set(EHCI_PORTSC_CONN_CHG)
#define mwHost20_PORTSC_ConnectStatus_Rd()                    (mwHost20_PORTSC_Rd() & EHCI_PORTSC_CONN_STS)
#define mwHost20_PORTSC_PortTest_TestPkt()                  (mwHost20_PORTSC_Rd() & (EHCI_PORTSC_PORTTST_PACKET<<16))
#define mwHost20_PORTSC_PortTest_TestPktDone()              (mwHost20_PORTSC_Rd() & EHCI_PORTSC_PORTTST_PKDONE)
//    0x40    HC Misc. Register
#define mwHost20_Misc_AsynSleepTime_Set(bValue)                  Host20_Set(REG_HC_ASYNCSCH_SLEEPTIME,bValue)
#define mwHost20_Misc_EOF1Time_Set(bValue)                    Host20_Set(REG_HC_EOF1TIME,bValue)
#define mwHost20_Misc_EOF2Time_Set(bValue)                    Host20_Set(REG_HC_EOF2TIME,bValue)
#define mwHost20_Misc_Physuspend_Rd()                        Host20_Get(REG_HC_PHYSUSP)
#define mwHost20_Misc_Physuspend_Set()                        Host20_Set(REG_HC_PHYSUSP,TRUE)
#define mwHost20_Misc_Physuspend_Clr()                        Host20_Set(REG_HC_PHYSUSP,FALSE)

#endif
