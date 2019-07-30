/*
 * NT660/cache.c
 *
 * Driver for NT96660 MIPS cache
 *
 * Copyright 2010 Novatek Microelectronics Corp., Ltd.
 *
 */

#include <asm/arch-nt96660/nt96660_regs.h>
#include <asm/arch/cache.h>
#include "ArchDefs.h"

void inv_icache_all(void)
{
	unsigned long base=0x80000000;
	unsigned int cfg1, icache_size;

	cfg1 = MMFC0(R_C0_Config1, R_C0_SelConfig1);
	icache_size = (2 << ((cfg1 & M_Config1IL) >> S_Config1IL)) * (64 << ((cfg1 & M_Config1IS) >> S_Config1IS)) * (((cfg1 & M_Config1IA) >> S_Config1IA) + 1);
	for(;base < (0x80000000 + icache_size); base+=1024)	{
		cache32_unroll32(base, ICIndexInv);
	}
}

void wb_inv_dcache_all(void)
{
	unsigned long base=0x80000000;
	unsigned int cfg1, dcache_size;

	cfg1 = MMFC0(R_C0_Config1, R_C0_SelConfig1);
	dcache_size = (2 << ((cfg1 & M_Config1DL) >> S_Config1DL)) * (64 << ((cfg1 & M_Config1DS) >> S_Config1DS)) * (((cfg1 & M_Config1DA) >> S_Config1DA) + 1);
	for(;base < (0x80000000 + dcache_size); base+=1024) {
		cache32_unroll32(base, DCIndexWBInv);
	}
	iob();
}

void wb_inv_dcache_range(unsigned long addr, unsigned long len)
{
	unsigned long adr;

	for( adr=addr&0xffffffe0; adr < ((addr + len + 0x1f)&0xffffffe0);adr+=32) {
		cache_op(DCHitWBInv, adr);
	}
	wb_inv_scache_range(addr, len);
}

void inv_dcache_range(unsigned long addr,unsigned long len)
{
	unsigned long adr;

	for(adr=addr&0xffffffe0; adr < ((addr + len + 0x1f)&0xffffffe0); adr+=32) {
		cache_op(DCHitInv, adr);
	}
	inv_scache_range(addr, len);
}

void wb_dcache_range(unsigned long addr,unsigned long len)
{
	unsigned long adr;

	for( adr=addr&0xffffffe0; adr < ((addr + len + 0x1f)&0xffffffe0); adr+=32) {
		cache_op(DCHitWB, adr);
	}
	wb_scache_range(addr, len);
}

void wb_inv_scache_range(unsigned long addr,unsigned long len)
{
	unsigned long adr;
	unsigned int cfg2, scache_linesize, cache_line_mask;

	cfg2 = MMFC0(R_C0_Config2, R_C0_SelConfig2);
	scache_linesize = (2 << ((cfg2 & M_Config2SL) >> S_Config2SL));

	if (scache_linesize == 0)
		return;

	cache_line_mask = ~(scache_linesize - 1);
	for( adr=addr&cache_line_mask; adr < ((addr + len + (scache_linesize -1))&cache_line_mask); adr+=scache_linesize) {
		cache_op(SCHitWBInv, adr);
	}
	iob();
}

void inv_scache_range(unsigned long addr,unsigned long len)
{
	unsigned long adr;
	unsigned int cfg2, scache_linesize, cache_line_mask;

	cfg2 = MMFC0(R_C0_Config2, R_C0_SelConfig2);
	scache_linesize = (2 << ((cfg2 & M_Config2SL) >> S_Config2SL));

	if (scache_linesize == 0)
		return;

	cache_line_mask = ~(scache_linesize - 1);
	for( adr=addr&cache_line_mask; adr < ((addr + len + (scache_linesize -1))&cache_line_mask); adr+=scache_linesize) {
		cache_op(SCHitInv, adr);
	}
}

void wb_scache_range(unsigned long addr,unsigned long len)
{
	unsigned long adr;
	unsigned int cfg2, scache_linesize, cache_line_mask;

	cfg2 = MMFC0(R_C0_Config2, R_C0_SelConfig2);
	scache_linesize = (2 << ((cfg2 & M_Config2SL) >> S_Config2SL));

	if (scache_linesize == 0)
		return;

	cache_line_mask = ~(scache_linesize - 1);
	for( adr=addr&cache_line_mask; adr < ((addr + len + (scache_linesize -1))&cache_line_mask); adr+=scache_linesize) {
		cache_op(SCHitWB, adr);
	}
	iob();
}
