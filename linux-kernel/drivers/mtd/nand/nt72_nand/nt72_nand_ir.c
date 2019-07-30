#include <linux/completion.h>
#include "include/nt72_nand_config.h"
#include "include/nt72_nand_io.h"
#include "include/nt72_nand_ir.h"
#include "include/nt72_nand_kapi.h"
#include "include/nt72_nand_regs.h"

void nt72_nand_ir_init(struct nt72_nand_info *info, const int busy_wait,
		       const u32 what)
{
	nfc_write32(REG_NFC_INTCTR, 0);
	/* REG_NFC_INITSTS: write 1 to clear */
	nfc_write32(REG_NFC_INTSTS, 0xFFFFFFFF);
#ifdef USE_INTERRUPT
	init_completion(&info->nfc_completion);
	if (!busy_wait)
		nfc_write32(REG_NFC_INTCTR, what);
#endif
}

void nt72_nand_ir_wait(struct nt72_nand_info *info, const char *who,
		       const u32 what)
{
#ifdef USE_INTERRUPT
	int ret;

	ret = wait_for_completion_timeout(&info->nfc_completion,
					  INTERRUPT_TIMEOUT);
	if (!ret)
		nfc_err("nfc_err: %s wait interrupt timeout\n", who);
#else
	nt72_nand_ir_force_busy_wait(info, who, what);
#endif
}

void nt72_nand_ir_force_busy_wait(struct nt72_nand_info *info, const char *who,
				  const u32 what)
{
	unsigned long stop_time = nt72_nand_kapi_timeout_init();

#ifdef SELFTEST_REG_POLLING_TIMEOUT
	int counter = 1;
#endif

	while (!(nfc_read32(REG_NFC_INTSTS) & what)) {
#ifdef SELFTEST_REG_POLLING_TIMEOUT
		if ((counter % 1000) == 0)
			ssleep(REG_POLLING_TIMEOUT / HZ * 2);
#endif
		if (nt72_nand_kapi_timeout_check(stop_time)) {
			nfc_err("nfc_err: %s polling int status timeout\n",
				who);
			break;
		}
		nt72_nand_kapi_cpu_relax();
	}
}

irqreturn_t nt72_nand_ir_isr(int irq, void *devid)
{
#ifdef USE_INTERRUPT
	struct nt72_nand_info *info = (struct nt72_nand_info *)devid;

	nfc_write32(REG_NFC_INTCTR, 0);
	complete(&info->nfc_completion);
#endif
	return IRQ_HANDLED;
}
