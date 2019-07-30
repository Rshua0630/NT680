#ifndef _NT72_NAND_PARTS_H_
#define _NT72_NAND_PARTS_H_

#include <linux/mtd/partitions.h>
#include "nt72_nand_config.h"

extern struct mtd_partition g_nt72_nand_mtd_partitions[];
extern const unsigned int g_nr_nt72_nand_mtd_partitions;

extern const char *g_nt72_nand_part_probes[];
extern const char g_nt72_nand_partition_table_name[];

/* these two functions are not defined in linux/mtd/partitions.h, fixed in newer
 * kernel */
int add_mtd_partitions(struct mtd_info *, const struct mtd_partition *, int);
int parse_mtd_partitions(struct mtd_info *master, const char **types,
			 struct mtd_partition **pparts,
			 unsigned long origin);

#endif /* _NT72_NAND_PARTS_H_ */
