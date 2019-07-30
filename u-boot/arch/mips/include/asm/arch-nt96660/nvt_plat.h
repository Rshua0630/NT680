/**\file
 *  This file serves as the wrapper for the platform/OS dependent functions
 *  It is needed to modify these functions accordingly based on the platform and the
 *  OS. Whenever the synopsys GMAC driver ported on to different platform, this file
 *  should be handled at most care.
 *  The corresponding function definitions for non-inline functions are available in
 *  nvt_plat.c file.
 * \internal
 * -------------------------------------REVISION HISTORY---------------------------
 * Synopsys 				01/Aug/2007		 	   Created
 */


#ifndef NVT_PLAT_H
#define NVT_PLAT_H 1

#include <common.h>
#include <asm/io.h>
#include <spi.h>
#include <malloc.h>
#include <asm/addrspace.h>

#include <asm/arch/nt96660_regs.h>

#define __sync()								\
	__asm__ __volatile__(						\
		".set	push\n\t"						\
		".set	noreorder\n\t"					\
		".set	mips2\n\t"						\
		"lw $0, %0\n\t"							\
		"sync\n\t"								\
		".set	pop"							\
		: /* no output */						\
		: "m" (*(int*)(0xa0000000))				\
		: "memory")
#define wbflush_ahb()	__sync()

#define mdelay(n) ({unsigned long msec=(n); while (msec--) udelay(1000);})

#define KERN_CRIT
#ifdef DEBUG
#define TR0(fmt, args...) printf(KERN_CRIT "SynopGMAC: " fmt, ##args)
#undef TR
#define TR(fmt, args...) printf(KERN_CRIT "SynopGMAC: " fmt, ##args)
#else
#define TR0(fmt, args...)
#define TR(fmt, args...) /* not debugging: nothing */
#endif

//typedef int	bool;
#define	true 	1
#define false 	0
#define DEFAULT_DELAY_VARIABLE  10
#define DEFAULT_LOOP_VARIABLE   10000

typedef enum {FALSE, TRUE} BOOL;				/* AO added	*/
#define ON		1
#define OFF		0

/* There are platform related endian conversions
 *
 */

#define LE32_TO_CPU	__le32_to_cpu
#define BE32_TO_CPU	__be32_to_cpu
#define CPU_TO_LE32	__cpu_to_le32


#define MAC_CLOCK_INVERSE_ADDR		0xC0020024

#define ETH_BASE_ADDR	0xC02B0000
#define MICREL_PHY_ID1	0x00221512
#define MICREL_PHY_ID2	0x00221513
#define RTL8201E_PHY_ID	0x001cc815
#define RTL8201F_PHY_ID	0x001cc816
#define NVT_IM_INTERNAL_PHY_ID	0x1



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

void   plat_delay(u32);

/**
 * The Low level function to read register contents from Hardware.
 *
 * @param[in] pointer to the base of register map
 * @param[in] Offset from the base
 * \return  Returns the register contents
 */

static u32 __inline__ ethReadReg(u32 *RegBase, u32 RegOffset)
{

  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);		// 2010.0114, Rich: readl, writel were defined in uboot/include/asm-mips/io.h
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
static void  __inline__ ethWriteReg(u32 *RegBase, u32 RegOffset, u32 RegData)
{

  u32 addr = (u32)RegBase + RegOffset;
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
static void __inline__ ethSetBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data |= BitPos;
  writel(data,(void *)addr);
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
static void __inline__ ethClearBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data &= (~BitPos);
  writel(data,(void *)addr);
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
static bool __inline__ ethCheckBits(u32 *RegBase, u32 RegOffset, u32 BitPos)
{
  u32 addr = (u32)RegBase + RegOffset;
  u32 data = readl((void *)addr);
  data &= BitPos;
  if(data)
	  return true;
  else
	  return false;

}

#endif
