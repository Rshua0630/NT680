#ifndef _NT72_NAND_IR_H_
#define _NT72_NAND_IR_H_

#include <linux/interrupt.h>
#include "nt72_nand_nfc.h"

void nt72_nand_ir_init(struct nt72_nand_info *info, const int busy_wait,
		       const u32 what);

void nt72_nand_ir_wait(struct nt72_nand_info *info, const char *who,
		       const u32 what);
void nt72_nand_ir_force_busy_wait(struct nt72_nand_info *info, const char *who,
				  const u32 what);

irqreturn_t nt72_nand_ir_isr(int irq, void *devid);

#endif /* _NT72_NAND_IR_H_ */
