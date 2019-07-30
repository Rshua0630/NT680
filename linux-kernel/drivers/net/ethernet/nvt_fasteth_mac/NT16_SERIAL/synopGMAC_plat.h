/**\file
 *  This file serves as the wrapper for the platform /OS
 * dependent functions
 *  It is needed to modify these functions accordingly
 * based on the platform and the
 *  OS. Whenever the synopsys GMAC driver ported on
 * to different platform, this file
 *  should be handled at most care.
 *  The corresponding function definitions for non-inline
 * functions are available in
 *  synopGMAC_plat.c file.
 * \internal
 * -------REVISION HISTORY---------------------------
 * Synopsys	01/Aug/2007	Created
 */

#ifndef SYNOP_GMAC_PLAT_H
#define SYNOP_GMAC_PLAT_H 1

#include <linux/kernel.h>
#include <linux/io.h>
/*#include <asm/io.h>*/
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/semaphore.h>

#define TR0(fmt, args...) /*printk(KERN_CRIT "SynopGMAC: " fmt, ##args)*/

/*open kernel debug message*/
/*#define DEBUG*/

#ifdef DEBUG
#undef TR
#  define TR(fmt, args...) pr_crit("SynopGMAC: " fmt, ##args)
#else
# define TR(fmt, args...) /* not debugging: nothing */
#endif

#define DEFAULT_DELAY_VARIABLE  10
#define DEFAULT_LOOP_VARIABLE   10000
#define AUTO_NEGO_WAITIG_LOOP_TIME   1000
#define NVT_PHY_TRY_COUNT            3000

/* There are platform related endian conversions
 *
 */

#define LE32_TO_CPU	__le32_to_cpu
#define BE32_TO_CPU	__be32_to_cpu
#define CPU_TO_LE32	__cpu_to_le32

#define SYNOP_GMAC_BASE_ADDR     0xFD160000
#define SYNOP_DMA_BASE_ADDR	0xFD161000

#define SYNOP_GMAC_REG_MEMMAP_LEN    0x00000FFC
#define SYNOP_DMA_REG_MEMMAP_LEN    0x00000054

#define MAC_RMII_MII_SEL_ADDR	0xFD160000
#define MAC_STBC_KEY_ADDR	0xFC040204
#define MAC_STBC_KEY_SET_ADDR	0xFC040208
#define MAC_STBC_CTRL_SWITCH_ADDR	0xFC040200
#define MAC_CLK_SRC_SWITCH_ADDR	0xFC040218
#define MAC_CLOCK_GPIO_ADDR		0xFC040BF0
#define MAC_CLOCK_INVERSE_ADDR		0xFC040BF4
#define MAC_AXI_CLK_RESET		0xFC04021C

#define MAC_STBC_RMII_GPIO_SEL_ADDR 0xFC04A424
#define MAC_STBC_RMII_GPIO_CTRL_ADDR_1 0xFC04043C
#define MAC_STBC_RMII_GPIO_CTRL_ADDR_2 0xFC040440

/*add new phy id for nt72682 demo board*/
#define MICREL_PHY_ID1		0x00221512
#define MICREL_PHY_ID2		0x00221513
#define RTL8201E_PHY_ID		0x001cc815

#define RTL8201F_PHY_ID         0x001cc816
#define NVT_INTERNAL_PHY_ID         0x1
#define ICPLUS101ALF_PHY_ID	0X02430C54
/* Error Codes */
#define ESYNOPGMACNOERR   0
#define ESYNOPGMACNOMEM   1
#define ESYNOPGMACPHYERR  2
#define ESYNOPGMACBUSY    3

struct Network_interface_data {
	u32 unit;
	u32 addr;
	u32 data;
};

/**
 * These are the wrapper function prototypes for OS/platform related routines
 */

void *plat_alloc_memory(unsigned long);
void   plat_free_memory(void *);

void *sys_plat_alloc_memory(unsigned long size);
void sys_plat_free_memory(unsigned long *addr);

void   plat_delay(u32);

/**
 * The Low level function to read register contents from Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * \return  Returns the register contents
 */
static inline u32 synopGMACReadReg(
	unsigned long *RegBase, unsigned long RegOffset)
{
	unsigned long addr = (unsigned long)RegBase + RegOffset;
	u32 data = readl((void *)addr);
	/*debug
	* TR("%s RegBase = 0x%08x RegOffset = 0x%08x
	* RegData = 0x%08x\n", __FUNCTION__,
	* (u32)RegBase, RegOffset, data );
	*/
	return data;
}

/**
 * The Low level function to write to a register in Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Data to be written
 * \return  void
 */
static inline void synopGMACWriteReg(
	unsigned long *RegBase, unsigned long RegOffset, unsigned long RegData)
{
	unsigned long addr = (unsigned long)RegBase + RegOffset;
	/* debug
	* TR("%s RegBase = 0x%08x RegOffset = 0x%08x
	* RegData = 0x%08x\n", __FUNCTION__,(u32) RegBase,
	* RegOffset, RegData );
	*/
	writel(RegData, (void *)addr);
}

/**
 * The Low level function to set bits of a register in Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1
 * \return  void
 */
static inline void synopGMACSetBits(
	unsigned long *RegBase, u32 RegOffset, u32 BitPos)
{
	unsigned long addr = (unsigned long)RegBase + RegOffset;
	unsigned long data = readl((void *)addr);

	data |= BitPos;
	/*debug
	* TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x
	* RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
	*/
	writel(data, (void *)addr);
	/*debug
	* TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x
	* RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
	*/
}

/**
 * The Low level function to clear bits of a register in Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Bit mask to clear bits to logical 0
 * \return  void
 */
static inline void synopGMACClearBits(
	unsigned long *RegBase, u32 RegOffset, u32 BitPos)
{
	unsigned long addr = (unsigned long)RegBase + RegOffset;
	unsigned long data = readl((void *)addr);

	data &= (~BitPos);
	/*debug
	* TR("%s !!!!!!!!!!!!!! RegOffset = 0x%08x
	* RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
	*/
	writel(data, (void *)addr);
	/*debug
	* TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x
	* RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
	*/
}

/**
 * The Low level function to Check the setting of the bits.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1
 * \return  returns TRUE if set to '1' returns FALSE if set to '0'.
 * Result undefined there are no bit set in the BitPos argument.
 *
 */
static inline bool synopGMACCheckBits(
		unsigned long *RegBase, u32 RegOffset, u32 BitPos)
{
	unsigned long addr = (unsigned long)RegBase + RegOffset;
	unsigned long data = readl((void *)addr);

	data &= BitPos;
	if (data)
		return true;
	else
		return false;
}

#endif
