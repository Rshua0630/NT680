#ifndef GIC_H
#define GIC_H

#ifdef __cplusplus
extern "C" {
#endif

/** @name Distributor Interface Register Map
 *
 * Define the offsets from the base address for all Distributor registers of
 * the interrupt controller, some registers may be reserved in the hardware
 * device.
 * @{
 */
#define GIC_DIST_EN_OFFSET         0x00000000 /**< Distributor Enable Register */
#define GIC_IC_TYPE_OFFSET         0x00000004 /**< Interrupt Controller Type Register */
#define GIC_DIST_IDENT_OFFSET      0x00000008 /**< Implementor ID Register */
#define GIC_SECURITY_OFFSET        0x00000080 /**< Interrupt Security Register */
#define GIC_ENABLE_SET_OFFSET      0x00000100 /**< Enable Set Register */
#define GIC_DISABLE_OFFSET         0x00000180 /**< Enable Clear Register */
#define GIC_PENDING_SET_OFFSET     0x00000200 /**< Pending Set Register */
#define GIC_PENDING_CLR_OFFSET     0x00000280 /**< Pending Clea Register */
#define GIC_ACTIVE_OFFSET          0x00000300 /**< Active Status Register */
#define GIC_PRIORITY_OFFSET        0x00000400 /**< Priority Level Register */
#define GIC_SPI_TARGET_OFFSET      0x00000800 /**< SPI Targe Register 0x800-0x8FB */
#define GIC_INT_CFG_OFFSET         0x00000C00 /**< Interrupt Configuration Register 0xC00-0xCFC */
#define GIC_PPI_STAT_OFFSET        0x00000D00 /**< PPI Status Register */
#define GIC_SPI_STAT_OFFSET        0x00000D04 /**< SPI Status Register 0xd04-0xd7C */
#define GIC_AHB_CONFIG_OFFSET      0x00000D80 /**< AHB Configuration Register */
#define GIC_SFI_TRIG_OFFSET        0x00000F00 /**< Software Triggered Interrupt Register */
#define GIC_PERPHID_OFFSET         0x00000FD0 /**< Peripheral ID Reg */
#define GIC_PCELLID_OFFSET         0x00000FF0 /**< Pcell ID Register */
/* @} */

/** @name  Distributor Enable Register
 * Controls if the distributor response to external interrupt inputs.
 * @{
 */
#define GIC_EN_INT_MASK            0x00000001 /**< Interrupt In Enable */
/* @} */

/** @name  Interrupt Controller Type Register
 * @{
 */
#define GIC_LSPI_MASK              0x0000F800 /**< Number of Lockable Shared Peripheral Interrupts*/
#define GIC_DOMAIN_MASK            0x00000400 /**< Number os Security domains*/
#define GIC_CPU_NUM_MASK           0x000000E0 /**< Number of CPU Interfaces */
#define GIC_NUM_INT_MASK           0x0000001F /**< Number of Interrupt IDs */
/* @} */

/** @name  Implementor ID Register
 * Implementor and revision information.
 * @{
 */
#define GIC_REV_MASK               0x00FFF000 /**< Revision Number */
#define GIC_IMPL_MASK              0x00000FFF /**< Implementor */
/* @} */

/** @name  Interrupt Security Registers
 * Each bit controls the security level of an interrupt, either secure or non
 * secure. These registers can only be accessed using secure read and write.
 * There are registers for each of the CPU interfaces at offset 0x080.  A
 * register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 32 of these registers staring at location 0x084.
 * @{
 */
#define GIC_INT_NS_MASK            0x00000001 /**< Each bit corresponds to an INT_ID */
/* @} */

/** @name  Enable Set Register
 * Each bit controls the enabling of an interrupt, a 0 is disabled, a 1 is
 * enabled. Writing a 0 has no effect. Use the ENABLE_CLR register to set a
 * bit to 0.
 * There are registers for each of the CPU interfaces at offset 0x100. With up
 * to 8 registers aliased to the same address. A register set for the SPI
 * interrupts is available to all CPU interfaces.
 * There are up to 32 of these registers staring at location 0x104.
 * @{
 */
#define GIC_INT_EN_MASK            0x00000001 /**< Each bit corresponds to an INT_ID */
/* @} */

/** @name  Enable Clear Register
 * Each bit controls the disabling of an interrupt, a 0 is disabled, a 1 is
 * enabled. Writing a 0 has no effect. Writing a 1 disables an interrupt and
 * sets the corresponding bit to 0.
 * There are registers for each of the CPU interfaces at offset 0x180. With up
 * to 8 registers aliased to the same address.
 * A register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 32 of these registers staring at location 0x184.
 * @{
 */
#define GIC_INT_CLR_MASK           0x00000001 /**< Each bit corresponds to an INT_ID */
/* @} */

/** @name  Pending Set Register
 * Each bit controls the Pending or Active and Pending state of an interrupt, a
 * 0 is not pending, a 1 is pending. Writing a 0 has no effect. Writing a 1 sets
 * an interrupt to the pending state.
 * There are registers for each of the CPU interfaces at offset 0x200. With up
 * to 8 registers aliased to the same address.
 * A register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 32 of these registers staring at location 0x204.
 * @{
 */
#define GIC_PEND_SET_MASK          0x00000001 /**< Each bit corresponds to an INT_ID */
/* @} */

/** @name  Pending Clear Register
 * Each bit can clear the Pending or Active and Pending state of an interrupt, a
 * 0 is not pending, a 1 is pending. Writing a 0 has no effect. Writing a 1
 * clears the pending state of an interrupt.
 * There are registers for each of the CPU interfaces at offset 0x280. With up
 * to 8 registers aliased to the same address.
 * A register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 32 of these registers staring at location 0x284.
 * @{
 */
#define GIC_PEND_CLR_MASK          0x00000001 /**< Each bit corresponds to an INT_ID */
/* @} */

/** @name  Active Status Register
 * Each bit provides the Active status of an interrupt, a
 * 0 is not Active, a 1 is Active. This is a read only register.
 * There are registers for each of the CPU interfaces at offset 0x300. With up
 * to 8 registers aliased to each address.
 * A register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 32 of these registers staring at location 0x380.
 * @{
 */
#define GIC_ACTIVE_MASK            0x00000001 /**< Each bit corresponds to an INT_ID */
/* @} */

/** @name  Priority Level Register
 * Each byte in a Priority Level Register sets the priority level of an
 * interrupt. Reading the register provides the priority level of an interrupt.
 * There are registers for each of the CPU interfaces at offset 0x400 through
 * 0x41C. With up to 8 registers aliased to each address.
 * 0 is highest priority, 0xFF is lowest.
 * A register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 255 of these registers staring at location 0x420.
 * @{
 */
#define GIC_PRIORITY_MASK          0x000000FF /**< Each Byte corresponds to an INT_ID */
#define GIC_PRIORITY_MAX           0x000000FF /**< Highest value of a priority actually the lowest priority*/
/* @} */

/** @name  SPI Target Register 0x800-0x8FB
 * Each byte references a separate SPI and programs which of the up to 8 CPU
 * interfaces are sent a Pending interrupt.
 * There are registers for each of the CPU interfaces at offset 0x800 through
 * 0x81C. With up to 8 registers aliased to each address.
 * A register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 255 of these registers staring at location 0x820.
 *
 * This driver does not support multiple CPU interfaces. These are included
 * for complete documentation.
 * @{
 */
#define GIC_SPI_CPU7_MASK          0x00000080 /**< CPU 7 Mask*/
#define GIC_SPI_CPU6_MASK          0x00000040 /**< CPU 6 Mask*/
#define GIC_SPI_CPU5_MASK          0x00000020 /**< CPU 5 Mask*/
#define GIC_SPI_CPU4_MASK          0x00000010 /**< CPU 4 Mask*/
#define GIC_SPI_CPU3_MASK          0x00000008 /**< CPU 3 Mask*/
#define GIC_SPI_CPU2_MASK          0x00000004 /**< CPU 2 Mask*/
#define GIC_SPI_CPU1_MASK          0x00000002 /**< CPU 1 Mask*/
#define GIC_SPI_CPU0_MASK          0x00000001 /**< CPU 0 Mask*/
/* @} */

/** @name  Interrupt Configuration Register 0xC00-0xCFC
 * The interrupt configuration registers program an SFI to be active HIGH level
 * sensitive or rising edge sensitive.
 * Each bit pair describes the configuration for an INT_ID.
 * SFI    Read Only    b10 always
 * PPI    Read Only    depending on how the PPIs are configured.
 *                    b01    Active HIGH level sensitive
 *                    b11 Rising edge sensitive
 * SPI                LSB is read only.
 *                    b01    Active HIGH level sensitive
 *                    b11 Rising edge sensitive/
 * There are registers for each of the CPU interfaces at offset 0xC00 through
 * 0xC04. With up to 8 registers aliased to each address.
 * A register set for the SPI interrupts is available to all CPU interfaces.
 * There are up to 255 of these registers staring at location 0xC08.
 * @{
 */
#define GIC_INT_CFG_MASK           0x00000003    /**< */
/* @} */

/** @name  PPI Status Register
 * Enables an external AMBA master to access the status of the PPI inputs.
 * A CPU can only read the status of its local PPI signals and cannot read the
 * status for other CPUs.
 * This register is aliased for each CPU interface.
 * @{
 */
#define GIC_PPI_C15_MASK           0x00008000    /**< PPI Status */
#define GIC_PPI_C14_MASK           0x00004000    /**< PPI Status */
#define GIC_PPI_C13_MASK           0x00002000    /**< PPI Status */
#define GIC_PPI_C12_MASK           0x00001000    /**< PPI Status */
#define GIC_PPI_C11_MASK           0x00000800    /**< PPI Status */
#define GIC_PPI_C10_MASK           0x00000400    /**< PPI Status */
#define GIC_PPI_C09_MASK           0x00000200    /**< PPI Status */
#define GIC_PPI_C08_MASK           0x00000100    /**< PPI Status */
#define GIC_PPI_C07_MASK           0x00000080    /**< PPI Status */
#define GIC_PPI_C06_MASK           0x00000040    /**< PPI Status */
#define GIC_PPI_C05_MASK           0x00000020    /**< PPI Status */
#define GIC_PPI_C04_MASK           0x00000010    /**< PPI Status */
#define GIC_PPI_C03_MASK           0x00000008    /**< PPI Status */
#define GIC_PPI_C02_MASK           0x00000004    /**< PPI Status */
#define GIC_PPI_C01_MASK           0x00000002    /**< PPI Status */
#define GIC_PPI_C00_MASK           0x00000001    /**< PPI Status */
/* @} */

/** @name  SPI Status Register 0xd04-0xd7C
 * Enables an external AMBA master to access the status of the SPI inputs.
 * There are up to 63 registers if the maximum number of SPI inputs are
 * configured.
 * @{
 */
#define GIC_SPI_N_MASK             0x00000001    /**< Each bit corresponds to an SPI input */
/* @} */

/** @name  AHB Configuration Register
 * Provides the status of the CFGBIGEND input signal and allows the endianess
 * of the GIC to be set.
 * @{
 */
#define GIC_AHB_END_MASK           0x00000004    /**< 0-GIC uses little Endian, 1-GIC uses Big Endian */
#define GIC_AHB_ENDOVR_MASK        0x00000002    /**< 0-Uses CFGBIGEND control, 1-use the AHB_END bit */
#define GIC_AHB_TIE_OFF_MASK       0x00000001    /**< State of CFGBIGEND */

/* @} */

/** @name  Software Triggered Interrupt Register
 * Controls issueing of software interrupts.
 * @{
 */
#define GIC_SFI_SELFTRIG_MASK      0x02010000
#define GIC_SFI_TRIG_TRGFILT_MASK  0x03000000    /**< Target List filter
                                                            b00-Use the target List
                                                            b01-All CPUs except requester
                                                            b10-To Requester
                                                            b11-reserved */
#define GIC_SFI_TRIG_CPU_MASK      0x00FF0000    /**< CPU Target list */
#define GIC_SFI_TRIG_SATT_MASK     0x00008000    /**< 0= Use a secure interrupt */
#define GIC_SFI_TRIG_INTID_MASK    0x0000000F    /**< Set to the INTID
                                                        signaled to the CPU*/
/* @} */

/** @name CPU Interface Register Map
 *
 * Define the offsets from the base address for all CPU registers of the
 * interrupt controller, some registers may be reserved in the hardware device.
 * @{
 */
#define GIC_CONTROL_OFFSET         0x00000000 /**< CPU Interface Control Register */
#define GIC_CPU_PRIOR_OFFSET       0x00000004 /**< Priority Mask Reg */
#define GIC_BIN_PT_OFFSET          0x00000008 /**< Binary Point Register */
#define GIC_INT_ACK_OFFSET         0x0000000C /**< Interrupt ACK Reg */
#define GIC_EOI_OFFSET             0x00000010 /**< End of Interrupt Reg */
#define GIC_RUN_PRIOR_OFFSET       0x00000014 /**< Running Priority Reg */
#define GIC_HI_PEND_OFFSET         0x00000018 /**< Highest Pending Interrupt Register */
#define GIC_ALIAS_BIN_PT_OFFSET    0x0000001C /**< Aliased non-Secure Binary Point Register */

/**<  0x00000020 to 0x00000FBC are reserved and should not be read or written
 * to. */
/* @} */


/** @name Control Register
 * CPU Interface Control register definitions
 * All bits are defined here although some are not available in the non-secure
 * mode.
 * @{
 */
#define GIC_CNTR_SBPR_MASK         0x00000010    /**< Secure Binary Pointer,
                                                        0=separate registers,
                                                        1=both use bin_pt_s */
#define GIC_CNTR_FIQEN_MASK        0x00000008    /**< Use nFIQ_C for secure interrupts,
                                                        0= use IRQ for both,
                                                        1=Use FIQ for secure, IRQ for non*/
#define GIC_CNTR_ACKCTL_MASK       0x00000004    /**< Ack control for secure or non secure */
#define GIC_CNTR_EN_NS_MASK        0x00000002    /**< Non Secure enable */
#define GIC_CNTR_EN_S_MASK         0x00000001    /**< Secure enable, 0=Disabled, 1=Enabled */
/* @} */

/** @name Priority Mask Register
 * Priority Mask register definitions
 * The CPU interface does not send interrupt if the level of the interrupt is
 * lower than the level of the register.
 * @{
 */
#define GIC_PRIORITY_MASK          0x000000FF /**< All interrupts */
/* @} */

/** @name Binary Point Register
 * Binary Point register definitions
 * @{
 */

#define GIC_BIN_PT_MASK            0x00000007  /**< Binary point mask value
                        Value  Secure  Non-secure
                        b000    0xFE    0xFF
                        b001    0xFC    0xFE
                        b010    0xF8    0xFC
                        b011    0xF0    0xF8
                        b100    0xE0    0xF0
                        b101    0xC0    0xE0
                        b110    0x80    0xC0
                        b111    0x00    0x80
                        */
/*@}*/

/** @name Interrupt Acknowledge Register
 * Interrupt Acknowledge register definitions
 * Identifies the current Pending interrupt, and the CPU ID for software
 * interrupts.
 */
#define GIC_ACK_INTID_MASK         0x000003FF /**< Interrupt ID */
#define GIC_CPUID_MASK             0x00000C00 /**< CPU ID */
/* @} */

/** @name End of Interrupt Register
 * End of Interrupt register definitions
 * Allows the CPU to signal the GIC when it completes an interrupt service
 * routine.
 */
#define GIC_EOI_INTID_MASK         0x000003FF /**< Interrupt ID */

/* @} */

/** @name Running Priority Register
 * Running Priority register definitions
 * Identifies the interrupt priority level of the highest priority active
 * interrupt.
 */
#define GIC_RUN_PRIORITY_MASK      0x00000FF /**< Interrupt Priority */
/* @} */

/*
 * Highest Pending Interrupt register definitions
 * Identifies the interrupt priority of the highest priority pending interupt
 */
#define GIC_PEND_INTID_MASK        0x000003FF /**< Pending Interrupt ID */
#define GIC_CPUID_MASK             0x00000C00 /**< CPU ID */
/* @} */

#define DEFAULT_PRIORITY               0x0UL

/***************** Macros (Inline Functions) Definitions *********************/

#define GIC_INT_CFG_OFFSET_CALC(InterruptID) \
    (GIC_INT_CFG_OFFSET + ((InterruptID/16) * 4))

#define GIC_PRIORITY_OFFSET_CALC(InterruptID) \
    (GIC_PRIORITY_OFFSET + ((InterruptID/4) * 4))

#define GIC_SPI_TARGET_OFFSET_CALC(InterruptID) \
    (GIC_SPI_TARGET_OFFSET + ((InterruptID/4) * 4))

#define GIC_ENABLE_DISABLE_OFFSET_CALC(Register, InterruptID) \
    (Register + ((InterruptID/32) * 4))


#define GIC_SETREG(ofs,value)           OUTW(PERIPH_BASE+(ofs),(value))
#define GIC_GETREG(ofs)                 INW(PERIPH_BASE+(ofs))

#define gicd_read_ctlr()                GIC_GETREG(GICD_CTLR)
#define gicd_read_typer()               GIC_GETREG(GICD_TYPER)
#define gicd_read_sgir()                GIC_GETREG(GICD_SGIR)

#define gicd_write_ctlr(val)            GIC_SETREG(GICD_CTLR, val)
#define gicd_write_sgir(val)            GIC_SETREG(GICD_SGIR, val)

/*******************************************************************************
 * GIC CPU interface accessors for writing entire registers
 ******************************************************************************/
#define gicc_write_ctlr(val)            GIC_SETREG(GICC_CTLR, val)
#define gicc_write_pmr(val)             GIC_SETREG(GICC_PMR, val)
#define gicc_write_BPR(val)             GIC_SETREG(GICC_BPR, val)
#define gicc_write_IAR(val)             GIC_SETREG(GICC_IAR, val)
#define gicc_write_EOIR(val)            GIC_SETREG(GICC_EOIR, val)
#define gicc_write_hppir(val)           GIC_SETREG(GICC_HPPIR, val)
#define gicc_write_dir(val)             GIC_SETREG(GICC_DIR, val)


#define MAX_SPIS            480

#define MAX_PPIS            14
#define MAX_SGIS            16

#define MIN_SGI_ID          0
#define MIN_PPI_ID          16
#define MIN_SPI_ID          32

#define GRP0                0
#define GRP1                1

#define _GICD_              0x1000
#define _GICC_              0x2000
#define _GICH_              0x4000
#define _GICV_              0x6000

#define GIC_PRI_MASK                0xff
#define GIC_HIGHEST_SEC_PRIORITY    0
#define GIC_LOWEST_SEC_PRIORITY     127
#define GIC_HIGHEST_NS_PRIORITY     128
#define GIC_LOWEST_NS_PRIORITY      254 /* 255 would disable an interrupt */
#define GIC_SPURIOUS_INTERRUPT      1023
#define GIC_TARGET_CPU_MASK 0xff

/* Distributor interface definitions */
#define GICD_CTLR           _GICD_ + 0x0
#define GICD_TYPER          _GICD_ + 0x4
#define GICD_IGROUPR        _GICD_ + 0x80
#define GICD_ISENABLER      _GICD_ + 0x100
#define GICD_ICENABLER      _GICD_ + 0x180
#define GICD_ISPENDR        _GICD_ + 0x200
#define GICD_ICPENDR        _GICD_ + 0x280
#define GICD_ISACTIVER      _GICD_ + 0x300
#define GICD_ICACTIVER      _GICD_ + 0x380
#define GICD_IPRIORITYR     _GICD_ + 0x400
#define GICD_ITARGETSR      _GICD_ + 0x800
#define GICD_ICFGR          _GICD_ + 0xC00
#define GICD_SGIR           _GICD_ + 0xF00
#define GICD_CPENDSGIR      _GICD_ + 0xF10
#define GICD_SPENDSGIR      _GICD_ + 0xF20

#define IGROUPR_SHIFT       5
#define ISENABLER_SHIFT     5
#define ICENABLER_SHIFT     ISENABLER_SHIFT
#define ISPENDR_SHIFT       5
#define ICPENDR_SHIFT       ISPENDR_SHIFT
#define ISACTIVER_SHIFT     5
#define ICACTIVER_SHIFT     ISACTIVER_SHIFT
#define IPRIORITYR_SHIFT    2
#define ITARGETSR_SHIFT     2
#define ICFGR_SHIFT         4
#define CPENDSGIR_SHIFT     2
#define SPENDSGIR_SHIFT     CPENDSGIR_SHIFT

/* GICD_TYPER bit definitions */
#define IT_LINES_NO_MASK    0x1f

/* GICD_ICFGR bit definitions */
#define LEVEL_SENSITIVE     0x0
#define TRIGGER_SENSITIVE   0x1

/* Physical CPU Interface registers */
#define GICC_CTLR           _GICC_ + 0x0
#define GICC_PMR            _GICC_ + 0x4
#define GICC_BPR            _GICC_ + 0x8
#define GICC_IAR            _GICC_ + 0xC
#define GICC_EOIR           _GICC_ + 0x10
#define GICC_RPR            _GICC_ + 0x14
#define GICC_HPPIR          _GICC_ + 0x18
#define GICC_AHPPIR         _GICC_ + 0x28
#define GICC_IIDR           _GICC_ + 0xFC
#define GICC_DIR            _GICC_ + 0x1000
#define GICC_PRIODROP       _GICC_ + GICC_EOIR

/* GICC_CTLR bit definitions */
#define EOI_MODE_NS         (1 << 10)
#define EOI_MODE_S          (1 << 9)
#define IRQ_BYP_DIS_GRP1    (1 << 8)
#define FIQ_BYP_DIS_GRP1    (1 << 7)
#define IRQ_BYP_DIS_GRP0    (1 << 6)
#define FIQ_BYP_DIS_GRP0    (1 << 5)
#define CBPR                (1 << 4)
#define FIQ_EN              (1 << 3)
#define ACK_CTL             (1 << 2)
#define ENABLE_GRP1         (1 << 1)
#define ENABLE_GRP0         (1 << 0)

/* GICC_IIDR bit masks and shifts */
#define GICC_IIDR_PID_SHIFT     20
#define GICC_IIDR_ARCH_SHIFT    16
#define GICC_IIDR_REV_SHIFT     12
#define GICC_IIDR_IMP_SHIFT     0

#define GICC_IIDR_PID_MASK  0xfff
#define GICC_IIDR_ARCH_MASK 0xf
#define GICC_IIDR_REV_MASK  0xf
#define GICC_IIDR_IMP_MASK  0xfff

/* HYP view virtual CPU Interface registers */
#define GICH_CTL            0x0
#define GICH_VTR            0x4
#define GICH_ELRSR0         0x30
#define GICH_ELRSR1         0x34
#define GICH_APR0           0xF0
#define GICH_LR_BASE        0x100

/* Virtual CPU Interface registers */
#define GICV_CTL            0x0
#define GICV_PRIMASK        0x4
#define GICV_BP             0x8
#define GICV_INTACK         0xC
#define GICV_EOI            0x10
#define GICV_RUNNINGPRI     0x14
#define GICV_HIGHESTPEND    0x18
#define GICV_DEACTIVATE     0x1000

#ifdef __cplusplus
}
#endif

#endif
