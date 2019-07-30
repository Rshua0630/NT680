#ifndef __ARCH_DEFS_H__
#define __ARCH_DEFS_H__

#define _ISB() \
	__asm__ __volatile__("isb\n\t");

#define _DSB() \
	__asm__ __volatile__("dsb\n\t");

#define _DMB() \
	__asm__ __volatile__("dmb\n\t");

/*
 ************************************************************************
 *    					Program status register                 		*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |N|Z|C|V|Q|Res|J|   R   |  GE   |     R     |E|A|I|F|T|    M    |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define S_CPSR_N	31
#define M_CPSR_N	(0x1 << S_CPSR_N)
#define S_CPSR_Z	30
#define M_CPSR_Z	(0x1 << S_CPSR_Z)
#define S_CPSR_C	29
#define M_CPSR_C	(0x1 << S_CPSR_C)
#define S_CPSR_V	28
#define M_CPSR_V	(0x1 << S_CPSR_V)
#define S_CPSR_Q	27
#define M_CPSR_Q	(0x1 << S_CPSR_Q)
#define S_CPSR_J	24
#define M_CPSR_J	(0x1 << S_CPSR_J)
#define S_CPSR_GE	16
#define M_CPSR_GE	(0xf << S_CPSR_GE)
#define S_CPSR_E	9
#define M_CPSR_E	(0x1 << S_CPSR_E)
#define S_CPSR_A	8
#define M_CPSR_A	(0x1 << S_CPSR_A)	/* Disable Abort */
#define S_CPSR_I	7
#define M_CPSR_I	(0x1 << S_CPSR_I)	/* Disable IRQ */
#define S_CPSR_F	6
#define M_CPSR_F	(0x1 << S_CPSR_F)	/* Disable FIQ */
#define S_CPSR_T	5
#define M_CPSR_T	(0x1 << S_CPSR_T)
#define S_CPSR_M	0
#define M_CPSR_M	(0x1f << S_CPSR_M)

#define K_CPSR_M_USR	0x10	/* User mode */
#define K_CPSR_M_FIQ	0x11	/* FIQ mode */
#define K_CPSR_M_IRQ	0x12	/* IRQ mode */
#define K_CPSR_M_SVC 	0x13	/* Supervisor modew */
#define	K_CPSR_M_MON	0x16	/* Monitor mode, Security extensions, secure only */
#define K_CPSR_M_ABT	0x17	/* Abort mode */
#define K_CPSR_M_HYP	0x1a	/* Virtualization extensions, non-secure only */
#define K_CPSR_M_UND	0x1b	/* Undifined mode */
#define K_CPSR_M_SYS	0x1f	/* System mode */

/*
 ************************************************************************
 *    			Main ID register, MIDR (cp15, 0, c0, c0, 0)             *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  Implementer  |  Var  | Arch  |   Primary part number   | Rev |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_MIDR_Impl		24
#define M_MIDR_Impl		(0xff << S_MIDR_Impl)
#define S_MIDR_Var		20
#define M_MIDR_Var		(0xf << S_MIDR_Var)
#define S_MIDR_Arch		16
#define M_MIDR_Arch		(0xf << S_MIDR_Arch)
#define S_MIDR_PartNo	3
#define M_MIDR_PartNO	(0xfff << S_MIDR_PartNO)
#define S_MIDR_Rev		0
#define M_MIDR_Rev		(0xf << S_MIDR_Rev)

#define K_MIDR_Impl_A	0x41	/* ARM Limited */
#define K_MIDR_Impl_D	0x44
#define K_MIDR_Impl_M	0x4d
#define K_MIDR_Impl_Q	0x51
#define K_MIDR_Impl_V	0x56
#define K_MIDR_Impl_i	0x69

#define K_MIDR_Arch_ARMv4		0x1
#define K_MIDR_Arch_ARMv4T		0x2
#define K_MIDR_Arch_ARMv5		0x3
#define K_MIDR_Arch_ARMv5T		0x4
#define K_MIDR_Arch_ARMv5TE		0x5
#define K_MIDR_Arch_ARMv5TEJ	0x6
#define K_MIDR_Arch_ARMv6		0x7
#define K_MIDR_Arch_DefbyCPUID	0xf	/* ARMv7*/

/*
 ************************************************************************
 *    Multiprocessor Affubuty register, MPIDR (cp15, 0, c0, c0, 5)      *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |1|U|Reserved                           |Cluster|SBZ        |CPU|
 * | | |                                   |ID     |           |ID |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_MPIDR_U				30
#define M_MPIDR_U				(0x1 << S_MPIDR_U)
#define S_MPIDR_CLUSTER_ID		8
#define M_MPIDR_CLUSTER_ID		(0xf << S_MPIDR_CLUSTER_ID)
#define S_MPIDR_CPU_ID			0
#define M_MPIDR_CPU_ID			(0x3 << S_MPIDR_CPU_ID)

/*
 ************************************************************************
 *    	Processor Feature Register0, ID_PFR0 (cp15, 0, c0, c1, 0)       *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    	Processor Feature Register1, ID_PFR1 (cp15, 0, c0, c1, 1)       *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    	Debug Feature Register0, ID_DFR0 (cp15, 0, c0, c1, 2)       	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    	Auxiliary Feature Register0, ID_AFR0 (cp15, 0, c0, c1, 3)       *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    Memory Model Feature Register0, ID_MMFR0 (cp15, 0, c0, c1, 4)     *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    Memory Model Feature Register1, ID_MMFR1 (cp15, 0, c0, c1, 5)     *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    Memory Model Feature Register2, ID_MMFR0 (cp15, 0, c0, c1, 6)     *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    Memory Model Feature Register3, ID_MMFR1 (cp15, 0, c0, c1, 7)     *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    		ISA Feature Register0, ID_ISAR0 (cp15, 0, c0, c2, 0)     	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    		ISA Feature Register1, ID_ISAR1 (cp15, 0, c0, c2, 1)	    *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    		ISA Feature Register2, ID_ISAR2 (cp15, 0, c0, c2, 2)     	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    		ISA Feature Register3, ID_ISAR1 (cp15, 0, c0, c2, 3)     	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    		ISA Feature Register4, ID_ISAR4 (cp15, 0, c0, c2, 4)     	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    		ISA Feature Register5, ID_ISAR5 (cp15, 0, c0, c2, 5)     	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved       |Aff2           |Aff1           |Aff0           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

/*
 ************************************************************************
 *    		Cache Size ID Register, CCSIDR (cp15, 1, c0, c0, 0)       	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |W|W|R|W|NumSets			 			 |Associativity      |L    |
 * |T|B|A|A|                             |                   |S    |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_CCSIDR_WT		31
#define M_CCSIDR_WT		(0x1  << S_CCSIDR_WT)		/* Support write-through */
#define S_CCSIDR_WB		30
#define M_CCSIDR_WB		(0x1  << S_CCSIDR_WB)		/* Support write-back */
#define S_CCSIDR_RA		29
#define M_CCSIDR_RA		(0x1  << S_CCSIDR_RA)		/* Support read-allocation */
#define S_CCSIDR_WA		28
#define M_CCSIDR_WA		(0x1  << S_CCSIDR_WA)		/* Support write-allocation */
#define S_CCSIDR_SETS	13
#define M_CCSIDR_SETS	(0x7fff  << S_CCSIDR_SETS)	/* Number of sets */
#define S_CCSIDR_A		3
#define M_CCSIDR_A		(0x3ff  << S_CCSIDR_A)		/* Number of associatiovity */
#define S_CCSIDR_LS		0
#define M_CCSIDR_LS		(0x7  << S_CCSIDR_LS)		/* Cache line size */

/*
 ************************************************************************
 *    		Cache Level ID Register, CLIDR (cp15, 1, c0, c0, 1)       	*
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |0|0|LoUU |LoC  |LoUIS|CT7  |CT6  |CT5  |CT4  |CT3  |CT2  |CT1  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_CLIDR_LoUU	27
#define M_CLIDR_LoUU	(0x7 << S_CLIDR_LoUU)
#define S_CLIDR_LoC		24
#define M_CLIDR_LoC		(0x7 << S_CLIDR_LoC)	/* Cache cohernecy level */
#define S_CLIDR_LoUIS	21
#define M_CLIDR_LoUIS	(0x7 << S_CLIDR_LoUIS)
#define S_CLIDR_Ctype7	18
#define M_CLIDR_Ctype7	(0x7 << S_CLIDR_Ctype7)	/* Cache type of level 7 cache */
#define S_CLIDR_Ctype6	15
#define M_CLIDR_Ctype6	(0x7 << S_CLIDR_Ctype6)	/* Cache type of level 6 cache */
#define S_CLIDR_Ctype5	12
#define M_CLIDR_Ctype5	(0x7 << S_CLIDR_Ctype5)	/* Cache type of level 5 cache */
#define S_CLIDR_Ctype4	9
#define M_CLIDR_Ctype4	(0x7 << S_CLIDR_Ctype4)	/* Cache type of level 4 cache */
#define S_CLIDR_Ctype3	6
#define M_CLIDR_Ctype3	(0x7 << S_CLIDR_Ctype3)	/* Cache type of level 3 cache */
#define S_CLIDR_Ctype2	3
#define M_CLIDR_Ctype2	(0x7 << S_CLIDR_Ctype2)	/* Cache type of level 2 cache */
#define S_CLIDR_Ctype1	0
#define M_CLIDR_Ctype1	(0x7 << S_CLIDR_Ctype1)	/* Cache type of level 1 cache */

#define K_CLIDR_Ctype_NO	0	/* No cache */
#define K_CLIDR_Ctype_IC	1	/* Instruction cache only */
#define K_CLIDR_Ctype_DC	2	/* Data cache only */
#define K_CLIDR_Ctype_ID	3	/* Seperate instruction and data caches, Harvard cache */
#define K_CLIDR_Ctype_U		4	/* Unified cache, von Neumann cacge */

/*
 ************************************************************************
 *    	Cache Size Selection Register, CCSELR (cp15, 2, c0, c0, 0)      *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |Reserved                                               |Level|I|
 * |                                                       |     |n|
 * |                                                       |     |D|
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_CCSELR_Level	1
#define M_CCSELR_Level	(0x7  << S_CCSELR_Level)	/* Cache level */
#define S_CCSELR_InD	0
#define M_CCSELR_InD	(0x1  << S_CCSELR_InD)		/* I$ or D$ */

/*
 ************************************************************************
 *    		System Control Register, SCTLR (cp15, 0, c1, c0, 0)         *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |R|T|A|T|N|0|E|V|1|U|F|U|W|1|H|1|0|R|V|I|Z|S|0|0|B|1|B|1|1|C|A|M|
 * | |E|F|R|M| |E|E| | |I|W|X| |A| | | | | | |W| | | | |E| | | | | |
 * | | |E|E|F| | | | | | |X|N| | | | | | | | | | | | | |N| | | | | |
 * | | | | |I| | | | | | |N| | | | | | | | | | | | | | | | | | | | |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_SCTLR_TE		30
#define M_SCTLR_TE		(0x1 << S_SCTLR_TE)			/* Thumb exception enable */
#define S_SCTLR_AFE		29
#define M_SCTLR_AFE		(0x1 << S_SCTLR_AFE)		/* Access flag enable */
#define S_SCTLR_TRE		28
#define M_SCTLR_TRE		(0x1 << S_SCTLR_TRE)		/* TXE remap enable */
#define S_SCTLR_NMFI	27
#define M_SCTLR_NMFI	(0x1 << S_SCTLR_NMFI)		/* None-maskable FIQ enbale (RO) */
#define S_SCTLR_EE		25
#define M_SCTLR_EE		(0x1 << S_SCTLR_EE)			/* Exception Endianness: big-endian */
#define S_SCTLR_VE		24
#define M_SCTLR_VE		(0x1 << S_SCTLR_VE)			/* Interrupt Vector Enable */
#define S_SCTLR_FI		21
#define M_SCTLR_FI		(0x1 << S_SCTLR_FI)			/* Fast interrupt configuraiont enable */
#define S_SCTLR_UWXN	20
#define M_SCTLR_UWXN	(0x1 << S_SCTLR_UWXN)		/* */
#define S_SCTLR_WXN		19
#define M_SCTLR_WXN		(0x1 << S_SCTLR_WXN)
#define S_SCTLR_HA		17
#define M_SCTLR_HA		(0x1 << S_SCTLR_HA)			/* Hardware access flag enable */
#define S_SCTLR_RR		14
#define M_SCTLR_RR		(0x1 << S_SCTLR_RR)			/* Round-robin select */
#define S_SCTLR_V		13
#define M_SCTLR_V		(0x1 << S_SCTLR_V)			/* Vector bits */
#define S_SCTLR_I		12
#define M_SCTLR_I		(0x1 << S_SCTLR_I)			/* Instruction cache enable */
#define S_SCTLR_Z		11
#define M_SCTLR_Z		(0x1 << S_SCTLR_Z)			/* Branch prediction enable */
#define S_SCTLR_SW		10
#define M_SCTLR_SW		(0x1 << S_SCTLR_SW)			/* SW and SWP enable */
#define S_SCTLR_CP15BEN	5
#define M_SCTLR_CP15BEN	(0x1 << S_SCTLR_CP15BEN)	/* CP15 barrier support */
#define S_SCTLR_C		2
#define M_SCTLR_C		(0x1 << S_SCTLR_C)			/* Data and Unidied cache enable */
#define S_SCTLR_A		1
#define M_SCTLR_A		(0x1 << S_SCTLR_A)			/* Alignment fault checking enable */
#define S_SCTLR_M		0
#define M_SCTLR_M		(0x1 << S_SCTLR_M)			/* PL1&0 stage MMU enable */

/************************************************************************
 *    		Auxiliary Control Register, ACTLR (cp15, 0, c1, c0, 1)      *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |UNP/SBZP                                   |P|A|E|S|RAZ|W|L|L|F|
 * |                                           |a|l|X|M|   | |1|2|W|
 * |                                           |r|l|C|P|   |f| | | |
 * |                                           | |o|L| |   |l|p|p| |
 * |                                           |o|c| | |   | |r|r| |
 * |                                           |n| | | |   |z|e|e| |
 * |                                           | |o| | |   |e|f|f| |
 * |                                           | |n| | |   |r| | | |
 * |                                           | |e| | |   |o|e|e| |
 * |                                           | | | | |   |s|n|n| |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_ACTLR_PAR_ON		9
#define M_ACTLR_PAR_ON		(0x1 << S_ACTLR_PAR_ON)		/* Parity on */
#define S_ACTLR_ALLOC_ONE	8
#define M_ACTLR_ALLOC_ONE	(0x1 << S_ACTLR_ALLOC_ONE)	/* Alloc in one way */
#define S_ACTLR_EXCL		7
#define M_ACTLR_EXCL		(0x1 << S_ACTLR_EXCL)		/* Exclusive cache */
#define S_ACTLR_SMP			6
#define M_ACTLR_SMP			(0x1 << S_ACTLR_SMP)		/* SMP */
#define S_ACTLR_W_FL_ZEROS	3
#define M_ACTLR_W_FL_ZEROS	(0x1 << S_ACTLR_W_FL_ZEROS)	/* Write full line of zeros mode */
#define S_ACTLR_L1_PERF_EN	2
#define M_ACTLR_L1_PERF_EN	(0x1 << S_ACTLR_L1_PERF_EN)	/* Dside prefetch enable */
#define S_ACTLR_L2_PERF_EN	1
#define M_ACTLR_L2_PERF_EN	(0x1 << S_ACTLR_L2_PERF_EN)	/* L2 prefetch enable */
#define S_ACTLR_FW			0
#define M_ACTLR_FW			(0x1 << S_ACTLR_FW)			/* Cache and TLB maintenance boardcast */

/*
 ************************************************************************
 *    	Secure Configuration Register, SCR (cp15, 0, c1, c1, 0)         *
 ************************************************************************
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                           |S|H|S|n|A|F|E|F|I|N|
 * |                                           |I|C|C|E|W|W|A|I|R|S|
 * |                                           |F|E|D|T| | | |Q|Q| |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define S_SCR_SIF			9
#define M_SCR_SIF			(0x1 << S_SCR_SIF)
#define S_SCR_HCE			8
#define M_SCR_HCE			(0x1 << S_SCR_HCE)
#define S_SCR_SCD			7
#define M_SCR_SCD			(0x1 << S_SCR_SCD)
#define S_SCR_NET			6
#define M_SCR_NET			(0x1 << S_SCR_NET)
#define S_SCR_AW			5
#define M_SCR_AW			(0x1 << S_SCR_AW)
#define S_SCR_FW			4
#define M_SCR_FW			(0x1 << S_SCR_FW)
#define S_SCR_EA			3
#define M_SCR_EA			(0x1 << S_SCR_EA)
#define S_SCR_FIQ			2
#define M_SCR_FIQ			(0x1 << S_SCR_FIQ)
#define S_SCR_IRQ			1
#define M_SCR_IRQ			(0x1 << S_SCR_IRQ)
#define S_SCR_NS			0
#define M_SCR_NS			(0x1 << S_SCR_NS)

#endif /* __ARCH_DEFS_H__ */

