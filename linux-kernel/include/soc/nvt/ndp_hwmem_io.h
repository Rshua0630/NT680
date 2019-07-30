
#ifndef __NDP_HWMEM_IO_H__
#define __NDP_HWMEM_IO_H__

#include <linux/interrupt.h>

#define CMD_HWMEM_FLUSH		(0x23)
#define CMD_HWMEM_INV_RANGE	(0x28)
#define CMD_HWMEM_CLEAN_RANGE	(0x29)
#define CMD_HWMEM_FLUSH_RANGE	(0x2A)

struct ndp_rel_hwmem_t {
	unsigned long	phy_addr;
	unsigned long	vir_addr;
	size_t		size;
};

void ndp_hwmem_flush_all(void);


#endif /* __NDP_HWMEM_IO_H__ */
