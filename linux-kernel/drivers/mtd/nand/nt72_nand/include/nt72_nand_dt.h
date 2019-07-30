#ifndef _NT72_NAND_DT_H_
#define _NT72_NAND_DT_H_

#include <linux/of_device.h>
#include "nt72_nand_config.h"

#ifdef USE_DT

extern const struct of_device_id g_nt72_nand_match_table[];

#endif  /* USE_DT */
#endif  /* _NT72_NAND_DTS_H_ */
