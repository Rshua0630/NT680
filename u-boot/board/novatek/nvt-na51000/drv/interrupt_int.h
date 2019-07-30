/*
	Interrupt module internal header file

	Interrupt module internal header file

	@file	   interrupt_int.h
	@ingroup	mIDrvSys_Interrupt
	@note	   Nothing

	Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _INTERRUPT_INT_H
#define _INTERRUPT_INT_H

#include <asm/arch/nvt_common.h>

#define _ARM_GIC_USAGE_				ENABLE

/**
	@addtogroup mIDrvSys_Interrupt
*/
//@{
#define IOADDR_GIC_REG_BASE			0xf1500000

// Register default value
#define INT_DST0_REG_DEFAULT			0xFFFFFFFF
#define INT_DST1_REG_DEFAULT			0xFFFFFFFF
#define INT_DST2_REG_DEFAULT			0x00000000
#define INT_DST3_REG_DEFAULT			0x00000000
#define INT_DST4_REG_DEFAULT			0x00000000
#define INT_DST5_REG_DEFAULT			0x00000000
#define INT_FLAG0_REG_DEFAULT			0x00000000
#define INT_FLAG1_REG_DEFAULT			0x00000000
#define INT_FLAG2_REG_DEFAULT			0x00000000
#define INT_FLAG3_REG_DEFAULT			0x00000000
#define INT_FLAG4_REG_DEFAULT			0x00000000
#define INT_FLAG5_REG_DEFAULT			0x00000000
#define INT_EN0_REG_DEFAULT			0x00000000
#define INT_EN1_REG_DEFAULT			0x00000000
#define INT_EN2_REG_DEFAULT			0x00000000
#define INT_EN3_REG_DEFAULT			0x00000000
#define INT_EN4_REG_DEFAULT			0x00000000
#define INT_EN5_REG_DEFAULT			0x00000000
#define INT_ID0_REG_DEFAULT			0x00000000
#define INT_ID1_REG_DEFAULT			0x00000000
#define INT_ID2_REG_DEFAULT			0x00000000
#define INT_LAST_ID0_REG_DEFAULT		0x00000000
#define INT_LAST_ID1_REG_DEFAULT		0x00000000
#define INT_LAST_ID2_REG_DEFAULT		0x00000000

/*
	Interrupt register default value

	Interrupt register default value.
*/
typedef struct
{
	UINT32	uiOffset;
	UINT32	uiValue;
	CHAR	*pName;
} INT_REG_DEFAULT;

#if(_ARM_GIC_USAGE_ == ENABLE)

#define GIC_SETREG(ofs,value)			nvt_writel(value, IOADDR_GIC_REG_BASE+(ofs))
#define GIC_GETREG(ofs)				nvt_readl(IOADDR_GIC_REG_BASE+(ofs))

#define gicd_read_ctlr()			GIC_GETREG(GICD_CTLR)
#define gicd_read_typer()			GIC_GETREG(GICD_TYPER)
#define gicd_read_sgir()			GIC_GETREG(GICD_SGIR)

#define gicd_write_ctlr(val)			GIC_SETREG(GICD_CTLR, val)
#define gicd_write_sgir(val)			GIC_SETREG(GICD_SGIR, val)

/*******************************************************************************
 * GIC CPU interface accessors for writing entire registers
 ******************************************************************************/
#define gicc_write_ctlr(val)			GIC_SETREG(GICC_CTLR, val)
#define gicc_write_pmr(val)			GIC_SETREG(GICC_PMR, val)
#define gicc_write_BPR(val)			GIC_SETREG(GICC_BPR, val)
#define gicc_write_IAR(val)			GIC_SETREG(GICC_IAR, val)
#define gicc_write_EOIR(val)			GIC_SETREG(GICC_EOIR, val)
#define gicc_write_hppir(val)			GIC_SETREG(GICC_HPPIR, val)
#define gicc_write_dir(val)			GIC_SETREG(GICC_DIR, val)


#define MAX_SPIS				480

#define MAX_PPIS				14
#define MAX_SGIS				16

#define MIN_SGI_ID		  		0
#define MIN_PPI_ID		  		16
#define MIN_SPI_ID		  		32

#define GRP0					0
#define GRP1					1

#define _GICD_					0x1000
#define _GICC_					0x2000
#define _GICH_					0x4000
#define _GICV_					0x6000

#define GIC_PRI_MASK				0xff
#define GIC_HIGHEST_SEC_PRIORITY		0
#define GIC_LOWEST_SEC_PRIORITY			127
#define GIC_HIGHEST_NS_PRIORITY			128
#define GIC_LOWEST_NS_PRIORITY			254 /* 255 would disable an interrupt */
#define GIC_SPURIOUS_INTERRUPT			1023
#define GIC_TARGET_CPU_MASK			0xff

/* Distributor interface definitions */
#define GICD_CTLR				_GICD_ + 0x0
#define GICD_TYPER				_GICD_ + 0x4
#define GICD_IGROUPR				_GICD_ + 0x80
#define GICD_ISENABLER				_GICD_ + 0x100
#define GICD_ICENABLER				_GICD_ + 0x180
#define GICD_ISPENDR				_GICD_ + 0x200
#define GICD_ICPENDR				_GICD_ + 0x280
#define GICD_ISACTIVER				_GICD_ + 0x300
#define GICD_ICACTIVER				_GICD_ + 0x380
#define GICD_IPRIORITYR				_GICD_ + 0x400
#define GICD_ITARGETSR				_GICD_ + 0x800
#define GICD_ICFGR				_GICD_ + 0xC00
#define GICD_SGIR				_GICD_ + 0xF00
#define GICD_CPENDSGIR				_GICD_ + 0xF10
#define GICD_SPENDSGIR				_GICD_ + 0xF20

#define IGROUPR_SHIFT				5
#define ISENABLER_SHIFT				5
#define ICENABLER_SHIFT				ISENABLER_SHIFT
#define ISPENDR_SHIFT				5
#define ICPENDR_SHIFT				ISPENDR_SHIFT
#define ISACTIVER_SHIFT				5
#define ICACTIVER_SHIFT				ISACTIVER_SHIFT
#define IPRIORITYR_SHIFT			2
#define ITARGETSR_SHIFT				2
#define ICFGR_SHIFT				4
#define CPENDSGIR_SHIFT				2
#define SPENDSGIR_SHIFT				CPENDSGIR_SHIFT

/* GICD_TYPER bit definitions */
#define IT_LINES_NO_MASK			0x1f

/* GICD_ICFGR bit definitions */
#define LEVEL_SENSITIVE				0x0
#define TRIGGER_SENSITIVE			0x1

/* Physical CPU Interface registers */
#define GICC_CTLR				_GICC_ + 0x0
#define GICC_PMR				_GICC_ + 0x4
#define GICC_BPR				_GICC_ + 0x8
#define GICC_IAR				_GICC_ + 0xC
#define GICC_EOIR				_GICC_ + 0x10
#define GICC_RPR				_GICC_ + 0x14
#define GICC_HPPIR				_GICC_ + 0x18
#define GICC_AHPPIR				_GICC_ + 0x28
#define GICC_IIDR				_GICC_ + 0xFC
#define GICC_DIR				_GICC_ + 0x1000
#define GICC_PRIODROP				_GICC_ + GICC_EOIR

/* GICC_CTLR bit definitions */
#define EOI_MODE_NS				(1 << 10)
#define EOI_MODE_S				(1 << 9)
#define IRQ_BYP_DIS_GRP1			(1 << 8)
#define FIQ_BYP_DIS_GRP1			(1 << 7)
#define IRQ_BYP_DIS_GRP0			(1 << 6)
#define FIQ_BYP_DIS_GRP0			(1 << 5)
#define CBPR					(1 << 4)
#define FIQ_EN					(1 << 3)
#define ACK_CTL					(1 << 2)
#define ENABLE_GRP1				(1 << 1)
#define ENABLE_GRP0				(1 << 0)

/* GICC_IIDR bit masks and shifts */
#define GICC_IIDR_PID_SHIFT			20
#define GICC_IIDR_ARCH_SHIFT			16
#define GICC_IIDR_REV_SHIFT			12
#define GICC_IIDR_IMP_SHIFT			0

#define GICC_IIDR_PID_MASK 			0xfff
#define GICC_IIDR_ARCH_MASK			0xf
#define GICC_IIDR_REV_MASK 			0xf
#define GICC_IIDR_IMP_MASK 			0xfff

/* HYP view virtual CPU Interface registers */
#define GICH_CTL				0x0
#define GICH_VTR				0x4
#define GICH_ELRSR0				0x30
#define GICH_ELRSR1				0x34
#define GICH_APR0				0xF0
#define GICH_LR_BASE				0x100

/* Virtual CPU Interface registers */
#define GICV_CTL				0x0
#define GICV_PRIMASK				0x4
#define GICV_BP					0x8
#define GICV_INTACK				0xC
#define GICV_EOI				0x10
#define GICV_RUNNINGPRI				0x14
#define GICV_HIGHESTPEND			0x18
#define GICV_DEACTIVATE				0x1000
#endif

//@}

#endif
