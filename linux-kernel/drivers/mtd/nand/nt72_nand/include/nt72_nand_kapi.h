#ifndef _NT72_NAND_KAPI_H_
#define _NT72_NAND_KAPI_H_

#include "nt72_nand_nfc.h"

void *nt72_nand_kapi_malloc(size_t size, gfp_t flags);

void nt72_nand_kapi_get_timestamp(u32 *s, u32 *us);

u32 nt72_nand_kapi_rand32(void);

void nt72_nand_kapi_cpu_relax(void);

unsigned long nt72_nand_kapi_timeout_init(void);
int nt72_nand_kapi_timeout_check(unsigned long stop_time);

void nt72_nand_kapi_lock_nfc(struct nt72_nand_info *info);
void nt72_nand_kapi_unlock_nfc(struct nt72_nand_info *info);

#endif
