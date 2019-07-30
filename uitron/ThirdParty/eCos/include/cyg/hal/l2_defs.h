#ifndef __L2_DEF_H__
#define __L2_DEF_H__

#define L2_REG0_BASE	(L2_MEM_BASE + 0x000)	/* Cache ID and Cache Type */
#define L2_REG1_BASE	(L2_MEM_BASE + 0x100)	/* Control */
#define L2_REG2_BASE	(L2_MEM_BASE + 0X200)	/* Interrupt and Counter Control Registers */
#define L2_REG7_BASE	(L2_MEM_BASE + 0x700)	/* Cache Maintenance Operations */
#define L2_REG9_BASE	(L2_MEM_BASE + 0x900)	/* Cache Lockdown */
#define L2_REG12_BASE	(L2_MEM_BASE + 0xC00)	/* Address Filtering */
#define L2_REG15_BASE	(L2_MEM_BASE + 0xF00)	/* Debug, Prefetch and Power */

/**
 * Cache ID and Cache Type
 */
#define L2_REG0_CACHE_ID				(*((volatile unsigned long *)(L2_REG0_BASE + 0x00)))
#define L2_REG0_CACHE_TYPE				(*((volatile unsigned long *)(L2_REG0_BASE + 0x04)))

#define S_L2_REG0_CACHE_TYPE_DB			(31)
#define M_L2_REG0_CACHE_TYPE_DB			(0xf << S_L2_REG0_CACHE_TYPE_DB)
#define S_L2_REG0_CACHE_TYPE_CTYPE		(25)
#define M_L2_REG0_CACHE_TYPE_CTYPE		(0xf << S_L2_REG0_CACHE_TYPE_CTYPE)
#define S_L2_REG0_CACHE_TYPE_H			(24)
#define M_L2_REG0_CACHE_TYPE_H			(0x1 << S_L2_REG0_CACHE_TYPE_H)
#define S_L2_REG0_CACHE_TYPE_DWS		(20)
#define M_L2_REG0_CACHE_TYPE_DWS		(0x7 << S_L2_REG0_CACHE_TYPE_DWS)
#define S_L2_REG0_CACHE_TYPE_DA			(18)
#define M_L2_REG0_CACHE_TYPE_DA			(0x1 << S_L2_REG0_CACHE_TYPE_DA)
#define S_L2_REG0_CACHE_TYPE_DLS		(12)
#define M_L2_REG0_CACHE_TYPE_DLS		(0x3 << S_L2_REG0_CACHE_TYPE_DLS)
#define S_L2_REG0_CACHE_TYPE_IWS		(8)
#define M_L2_REG0_CACHE_TYPE_IWS		(0x7 << S_L2_REG0_CACHE_TYPE_IWS)
#define S_L2_REG0_CACHE_TYPE_IA			(6)
#define M_L2_REG0_CACHE_TYPE_IA			(0x1 << S_L2_REG0_CACHE_TYPE_IA)
#define S_L2_REG0_CACHE_TYPE_ILS		(0)
#define M_L2_REG0_CACHE_TYPE_ILS		(0x3 << S_L2_REG0_CACHE_TYPE_ILS)

#define K_L2_REG0_CACHE_TYPE_DA_16WAY	(1)
#define K_L2_REG0_CACHE_TYPE_DA_8WAY	(0)

/**
 * Control
 */
#define L2_REG1_CONTROL					(*((volatile unsigned long *)(L2_REG1_BASE + 0x00)))
#define L2_REG1_AUX_CTRL				(*((volatile unsigned long *)(L2_REG1_BASE + 0x04)))
#define L2_REG1_TAG_RAM_CTRL			(*((volatile unsigned long *)(L2_REG1_BASE + 0x08)))
#define L2_REG1_DATA_RAM_CTRL			(*((volatile unsigned long *)(L2_REG1_BASE + 0x0C)))

#define S_L2_REG1_CONTROL_EN	(0)
#define M_L2_REG1_CONTROL_EN	(0x1 << S_L2_REG1_CONTROL_EN)

#define K_L2_REG1_CONTROL_EN_ON		1
#define K_L2_REG1_CONTROL_EN_OFF	0

#define S_L2_REG1_AUX_CTRL_BRESP						(30)
#define M_L2_REG1_AUX_CTRL_BRESP						(0x1 << S_L2_REG1_AUX_CTRL_BRESP)
#define S_L2_REG1_AUX_CTRL_INSTR_PREF					(29)
#define M_L2_REG1_AUX_CTRL_INSTR_PREF					(0x1 << S_L2_REG1_AUX_CTRL_INSTR_PREF)
#define S_L2_REG1_AUX_CTRL_DATA_PERF					(28)
#define M_L2_REG1_AUX_CTRL_DATA_PERF					(0x1 << S_L2_REG1_AUX_CTRL_DATA_PERF)
#define S_L2_REG1_AUX_CTRL_NS_INT_CTRL					(27)
#define M_L2_REG1_AUX_CTRL_NS_INT_CTRL					(0x1 << S_L2_REG1_AUX_CTRL_NS_INT_CTRL)
#define S_L2_REG1_AUX_CTRL_NS_LOCK_EN					(26)
#define M_L2_REG1_AUX_CTRL_NS_LOCK_EN					(0x1 << S_L2_REG1_AUX_CTRL_NS_LOCK_EN)
#define S_L2_REG1_AUX_CTRL_CACHE_POLICY					(25)
#define M_L2_REG1_AUX_CTRL_CACHE_POLICY					(0x1 << S_L2_REG1_AUX_CTRL_CACHE_POLICY)
#define S_L2_REG1_AUX_CTRL_FORCE_WA						(23)
#define M_L2_REG1_AUX_CTRL_FORCE_WA						(0x3 << S_L2_REG1_AUX_CTRL_FORCE_WA)
#define S_L2_REG1_AUX_CTRL_SHARED_OVERRIDE_EN			(22)
#define M_L2_REG1_AUX_CTRL_SHARED_OVERRIDE_EN			(0x1 << S_L2_REG1_AUX_CTRL_SHARED_OVERRIDE_EN)
#define S_L2_REG1_AUX_CTRL_PARITY_EN					(21)
#define M_L2_REG1_AUX_CTRL_PARITY_EN					(0x1 << S_L2_REG1_AUX_CTRL_PARITY_EN)
#define S_L2_REG1_AUX_CTRL_EVENT_MON_BUD_EN				(20)
#define M_L2_REG1_AUX_CTRL_EVENT_MON_BUD_EN				(0x1 << S_L2_REG1_AUX_CTRL_EVENT_MON_BUD_EN)
#define S_L2_REG1_AUX_CTRL_WAT_SIZE						(17)
#define M_L2_REG1_AUX_CTRL_WAT_SIZE						(0x7 << S_L2_REG1_AUX_CTRL_WAT_SIZE)
#define S_L2_REG1_AUX_CTRL_ASSOCIATIVITY				(16)
#define M_L2_REG1_AUX_CTRL_ASSOCIATIVITY				(0x1 << S_L2_REG1_AUX_CTRL_ASSOCIATIVITY)
#define S_L2_REG1_AUX_CTRL_SHARED_INV_EN				(13)
#define M_L2_REG1_AUX_CTRL_SHARED_INV_EN				(0x1 << S_L2_REG1_AUX_CTRL_SHARED_INV_EN)
#define S_L2_REG1_AUX_CTRL_EXCLUSIVE_CACHE_CONF			(12)
#define M_L2_REG1_AUX_CTRL_EXCLUSIVE_CACHE_CONF			(0x1 << S_L2_REG1_AUX_CTRL_EXCLUSIVE_CACHE_CONF)
#define S_L2_REG1_AUX_CTRL_STORE_BUD_DEV_LIMIT_EN		(11)
#define M_L2_REG1_AUX_CTRL_STORE_BUD_DEV_LIMIT_EN		(0x1 << S_L2_REG1_AUX_CTRL_STORE_BUD_DEV_LIMIT_EN)
#define S_L2_REG1_AUX_CTRL_HIGH_PRIO_SO_DEV_READS_EN	(10)
#define M_L2_REG1_AUX_CTRL_HIGH_PRIO_SO_DEV_READS_EN	(0x1 << S_L2_REG1_AUX_CTRL_HIGH_PRIO_SO_DEV_READS_EN)
#define S_L2_REG1_AUX_CTRL_FULL_LINE_Z_EN				(0)
#define M_L2_REG1_AUX_CTRL_FULL_LINE_Z_EN				(0x1 << S_L2_REG1_AUX_CTRL_FULL_LINE_Z_EN)

/**
 * Interrupt and Counter Control Registers
 */
#define L2_REG2_EV_CNT_CTRL				(*((volatile unsigned long *)(L2_REG2_BASE + 0x00)))
#define L2_REG2_EV_CNT1_CFG				(*((volatile unsigned long *)(L2_REG2_BASE + 0x04)))
#define L2_REG2_EV_CNT0_CFG				(*((volatile unsigned long *)(L2_REG2_BASE + 0x08)))
#define L2_REG2_EV_CNT1					(*((volatile unsigned long *)(L2_REG2_BASE + 0x0C)))
#define L2_REG2_EV_CNT0					(*((volatile unsigned long *)(L2_REG2_BASE + 0x10)))
#define L2_REG2_INT_MASK				(*((volatile unsigned long *)(L2_REG2_BASE + 0x14)))
#define L2_REG2_INT_MASK_STATUS			(*((volatile unsigned long *)(L2_REG2_BASE + 0x18)))
#define L2_REG2_INT_RAW_STATUS			(*((volatile unsigned long *)(L2_REG2_BASE + 0x1C)))
#define L2_REG2_INT_CLEAR				(*((volatile unsigned long *)(L2_REG2_BASE + 0x20)))

#define S_L2_REG2_INT_DECERR	(8)
#define M_L2_REG2_INT_DECERR	(0x1 << S_L2_REG2_INT_DECERR)	/* Decode error */
#define S_L2_REG2_INT_SLVERR	(7)
#define M_L2_REG2_INT_SLVERR	(0x1 << S_L2_REG2_INT_SLVERR)	/* Slave error */
#define S_L2_REG2_INT_ERRRD		(6)
#define M_L2_REG2_INT_ERRRD		(0x1 << S_L2_REG2_INT_ERRRD)	/* Data RAM read error */
#define S_L2_REG2_INT_ERRRT		(5)
#define M_L2_REG2_INT_ERRRT		(0x1 << S_L2_REG2_INT_ERRRT)	/* Tag RAM read error */
#define S_L2_REG2_INT_ERRWD		(4)
#define M_L2_REG2_INT_ERRWD		(0x1 << S_L2_REG2_INT_ERRWD)	/* Data RAM write error */
#define S_L2_REG2_INT_ERRWT		(3)
#define M_L2_REG2_INT_ERRWT		(0x1 << S_L2_REG2_INT_ERRWT)	/* Tag RAM write error */
#define S_L2_REG2_INT_PARRD		(2)
#define M_L2_REG2_INT_PARRD		(0x1 << S_L2_REG2_INT_PARRD)	/* Parity error on data RAM read */
#define S_L2_REG2_INT_PARRT		(1)
#define M_L2_REG2_INT_PARRT		(0x1 << S_L2_REG2_INT_PARRT)	/* Pariry error on tag RAM read */
#define S_L2_REG2_INT_ECNTR		(0)
#define M_L2_REG2_INT_ECNTR		(0x1 << S_L2_REG2_INT_ECNTR)	/* Event counter1/0 overflow increment */

/**
 * Cache Maintenance Operations
 */
#define L2_REG7_CACHE_SYNC				(*((volatile unsigned long *)(L2_REG7_BASE + 0x30)))
#define L2_REG7_INV_PA					(*((volatile unsigned long *)(L2_REG7_BASE + 0x70)))
#define L2_REG7_INV_WAY					(*((volatile unsigned long *)(L2_REG7_BASE + 0x7C)))
#define L2_REG7_CLEAN_PA				(*((volatile unsigned long *)(L2_REG7_BASE + 0xB0)))
#define L2_REG7_CLEAN_INDEX				(*((volatile unsigned long *)(L2_REG7_BASE + 0xB8)))
#define L2_REG7_CLEAN_WAY				(*((volatile unsigned long *)(L2_REG7_BASE + 0xBC)))
#define L2_REG7_CLEAN_INV_PA			(*((volatile unsigned long *)(L2_REG7_BASE + 0xF0)))
#define L2_REG7_CLEAN_INV_INDEX			(*((volatile unsigned long *)(L2_REG7_BASE + 0xF8)))
#define L2_REG7_CLEAN_INV_WAY			(*((volatile unsigned long *)(L2_REG7_BASE + 0xFC)))


#define K_L2_REG7_CACHE_SYNC_C			(0x1)

#define K_L2_REG7_INV_WAY_8WAY			(0x00ff)
#define K_L2_REG7_INV_WAY_16WAY			(0xffff)

/**
 * Cache Lockdown
 */
#define L2_REG9_D_LOCKDOWN0				(*((volatile unsigned long *)(L2_REG9_BASE + 0x00)))
#define L2_REG9_I_LOCKDOWN0				(*((volatile unsigned long *)(L2_REG9_BASE + 0x04)))
#define L2_REG9_D_LOCKDOWN1				(*((volatile unsigned long *)(L2_REG9_BASE + 0x08)))
#define L2_REG9_I_LOCKDOWN1				(*((volatile unsigned long *)(L2_REG9_BASE + 0x0C)))
#define L2_REG9_D_LOCKDOWN2				(*((volatile unsigned long *)(L2_REG9_BASE + 0x10)))
#define L2_REG9_I_LOCKDOWN2				(*((volatile unsigned long *)(L2_REG9_BASE + 0x14)))
#define L2_REG9_D_LOCKDOWN3				(*((volatile unsigned long *)(L2_REG9_BASE + 0x18)))
#define L2_REG9_I_LOCKDOWN3				(*((volatile unsigned long *)(L2_REG9_BASE + 0x1C)))
#define L2_REG9_D_LOCKDOWN4				(*((volatile unsigned long *)(L2_REG9_BASE + 0x20)))
#define L2_REG9_I_LOCKDOWN4				(*((volatile unsigned long *)(L2_REG9_BASE + 0x24)))
#define L2_REG9_D_LOCKDOWN5				(*((volatile unsigned long *)(L2_REG9_BASE + 0x28)))
#define L2_REG9_I_LOCKDOWN5				(*((volatile unsigned long *)(L2_REG9_BASE + 0x2C)))
#define L2_REG9_D_LOCKDOWN6				(*((volatile unsigned long *)(L2_REG9_BASE + 0x30)))
#define L2_REG9_I_LOCKDOWN6				(*((volatile unsigned long *)(L2_REG9_BASE + 0x34)))
#define L2_REG9_D_LOCKDOWN7				(*((volatile unsigned long *)(L2_REG9_BASE + 0x38)))
#define L2_REG9_I_LOCKDOWN7				(*((volatile unsigned long *)(L2_REG9_BASE + 0x3C)))
#define L2_REG9_LOCK_LINE_EN			(*((volatile unsigned long *)(L2_REG9_BASE + 0x50)))
#define L2_REG9_UNLOCK_WAY				(*((volatile unsigned long *)(L2_REG9_BASE + 0x54)))

/**
 * Address Filtering
 */
#define L2_REG12_ADDR_FILTERING_START	(*((volatile unsigned long *)(L2_REG12_BASE + 0x00)))
#define L2_REG12_ADDR_FILTERING_END		(*((volatile unsigned long *)(L2_REG12_BASE + 0x04)))

/**
 * Debug, Prefetch and Power
 */
#define L2_REG15_DEBUG_CTRL				(*((volatile unsigned long *)(L2_REG15_BASE + 0x40)))
#define L2_REG15_PREF_CTRL				(*((volatile unsigned long *)(L2_REG15_BASE + 0x60)))
#define L2_REG15_POWER_CTRL				(*((volatile unsigned long *)(L2_REG15_BASE + 0x80)))

#define S_L2_REG15_DEBUG_CTRL_SPNIDEN	(2)
#define M_L2_REG15_DEBUG_CTRL_SPNIDEN	(0x1 << S_L2_REG15_DEBUG_CTRL_SPNIDEN)
#define S_L2_REG15_DEBUG_CTRL_DWB		(1)
#define M_L2_REG15_DEBUG_CTRL_DWB		(0x1 << S_L2_REG15_DEBUG_CTRL_DWB)
#define S_L2_REG15_DEBUG_CTRL_DCL		(0)
#define M_L2_REG15_DEBUG_CTRL_DCL		(0x1 << S_L2_REG15_DEBUG_CTRL_DCL)

#define S_L2_REG15_PREF_CTRL_DL_FILL_EN					(30)
#define M_L2_REG15_PREF_CTRL_DL_FILL_EN					(0x1 << S_L2_REG15_PREF_CTRL_DL_FILL_EN)
#define S_L2_REG15_PREF_CTRL_INST_PREF_EN				(29)
#define M_L2_REG15_PREF_CTRL_INST_PREF_EN				(0x1 << S_L2_REG15_PREF_CTRL_INST_PREF_EN)
#define S_L2_REG15_PREF_CTRL_DATA_PREF_EN				(28)
#define M_L2_REG15_PREF_CTRL_DATA_PREF_EN				(0x1 << S_L2_REG15_PREF_CTRL_DATA_PREF_EN)
#define S_L2_REG15_PREF_CTRL_DL_WRAP_READ_DIS			(27)
#define M_L2_REG15_PREF_CTRL_DL_WRAP_READ_DIS			(0x1 << S_L2_REG15_PREF_CTRL_DL_WRAP_READ_DIS)
#define S_L2_REG15_PREF_CTRL_PREF_DROP_EN				(24)
#define M_L2_REG15_PREF_CTRL_PREF_DROP_EN				(0x1 << S_L2_REG15_PREF_CTRL_PREF_DROP+EN)
#define S_L2_REG15_PREF_CTRL_INCR_DL_FILL_EN			(23)
#define M_L2_REG15_PREF_CTRL_INCR_DL_FILL_EN			(0x1 << S_L2_REG15_PREF_CTRL_INCR_DL_FILL_EN)
#define S_L2_REG15_PREF_CTRL_NOT_SAME_ID_EXCLU_SEQ_EN	(21)
#define M_L2_REG15_PREF_CTRL_NOT_SAME_ID_EXCLU_SEQ_EN	(0x1 << S_L2_REG15_PREF_CTRL_NOT_SAME_ID_EXCLU_SEQ_EN)
#define S_L2_REG15_PREF_CTRL_PREF_OFF					(0)
#define M_L2_REG15_PREF_CTRL_PREF_OFF					(0xf << S_L2_REG15_PREF_CTRL_PREF_OFF)

#endif

