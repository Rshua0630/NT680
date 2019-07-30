/**\file
 *  This file serves as the wrapper for the platform/OS dependent functions
 *  It is needed to modify these functions accordingly based on the platform and the
 *  OS. Whenever the synopsys GMAC driver ported on to different platform, this file
 *  should be handled at most care.
 *  The corresponding function definitions for non-inline functions are available in 
 *  synopGMAC_plat.c file.
 * \internal
 * -------------------------------------REVISION HISTORY---------------------------
 * Synopsys 				01/Aug/2007		 	   Created
 */
 
 
#ifndef SYNOP_GMAC_PLAT_H
#define SYNOP_GMAC_PLAT_H 1


#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/semaphore.h>

#define TR0(fmt, args...) /*printk(KERN_CRIT "SynopGMAC: " fmt, ##args)*/

//#define DEBUG	//jay hsu : open kernel debug message

#ifdef DEBUG
#undef TR
#  define TR(fmt, args...) printk(KERN_CRIT "SynopGMAC: " fmt, ##args)
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

#ifdef CONFIG_MACH_NT14M
#define MAC_STBC_RMII_GPIO_SEL_ADDR 0xFC04A424
#define MAC_STBC_RMII_GPIO_CTRL_ADDR_1 0xFC04043C
#define MAC_STBC_RMII_GPIO_CTRL_ADDR_2 0xFC040440
#endif /* ifdef CONFIG_MACH_NT14M */


// jay hsu : add new phy id for nt72682 demo board
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

struct Network_interface_data
{
	u32 unit;
	u32 addr;
	u32 data;
};


/**
  * These are the wrapper function prototypes for OS/platform related routines
  */ 

void * plat_alloc_memory(u32 );
void   plat_free_memory(void *);

#if 0
void * plat_alloc_consistent_dmaable_memory(struct pci_dev *, u32, u32 *);
void   plat_free_consistent_dmaable_memory (struct pci_dev *, u32, void *, u32);
#else
void *sys_plat_alloc_memory(u32 size);
void sys_plat_free_memory(u32 *addr);
#endif

void   plat_delay(u32);


/**
 * The Low level function to read register contents from Hardware.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * \return  Returns the register contents 
 */
static __inline__ u32 synopGMACReadReg(u32 *RegBase, u32 RegOffset)
{

  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  //TR("%s RegBase = 0x%08x RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, (u32)RegBase, RegOffset, data );
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
static __inline__ void synopGMACWriteReg(u32 *RegBase, u32 RegOffset, u32 RegData)
{

  u32 addr = (u32)RegBase + RegOffset;
//  TR("%s RegBase = 0x%08x RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__,(u32) RegBase, RegOffset, RegData );
  writel(RegData,(void *)addr);

  return;
}

/**
 * The Low level function to set bits of a register in Hardware.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1 
 * \return  void 
 */
static __inline__ void synopGMACSetBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data |= BitPos; 
//  TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  writel(data,(void *)addr);
//  TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  return;
}


/**
 * The Low level function to clear bits of a register in Hardware.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * @param[in] Bit mask to clear bits to logical 0 
 * \return  void 
 */
static __inline__ void synopGMACClearBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data &= (~BitPos); 
//  TR("%s !!!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  writel(data,(void *)addr);
//  TR("%s !!!!!!!!!!!!! RegOffset = 0x%08x RegData = 0x%08x\n", __FUNCTION__, RegOffset, data );
  return;
}

/**
 * The Low level function to Check the setting of the bits.
 * 
 * @param[in] pointer to the base of register map  
 * @param[in] Offset from the base
 * @param[in] Bit mask to set bits to logical 1 
 * \return  returns TRUE if set to '1' returns FALSE if set to '0'. Result undefined there are no bit set in the BitPos argument.
 * 
 */
static __inline__ bool synopGMACCheckBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data &= BitPos; 
  if(data)  return true;
  else	    return false;

}


#endif
