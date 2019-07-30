#ifndef _NT72_NAND_UTILS_H_
#define _NT72_NAND_UTILS_H_

#include "nt72_nand_config.h"
#include "nt72_nand_nfc.h"

#ifdef ENABLE_CMD_HISTORY
struct nt72_nand_cmd_history_record {
	char	command[8];
	int	data_valid;
	u32	data;
	u32	start_s;
	u32	start_us;
	u32	end_s;
	u32	end_us;
};
#endif

#ifdef ENABLE_CMD_HISTORY
struct nt72_nand_cmd_history {
	struct nt72_nand_cmd_history_record	records[CMD_HISTORY_SIZE];
	int					head;
	int					tail;
};
#endif

#ifdef ENABLE_BUS_SNIFFER
struct nt72_nand_bus_sniffer_record {
	char	op[8];
	u16	addr;
	u32	val;
};
#endif

#ifdef ENABLE_BUS_SNIFFER
struct nt72_nand_bus_sniffer {
	struct nt72_nand_bus_sniffer_record	records[BUS_SNIFFER_SIZE];
	int					head;
	int					tail;
};
#endif

#ifndef EXTRA_CHECK_CACHE_ALIGNMENT
#define check_cache_alignment(...)
#else
void check_cache_alignment(struct nt72_nand_info *info, const char *who,
			   unsigned char *addr);
#endif

#ifndef ENABLE_CMD_HISTORY
#define nt72_nand_cmd_history_get_timestamp(...)
#else
u64 nt72_nand_cmd_history_get_timestamp_diff(int i);
#endif

#ifndef ENABLE_CMD_HISTORY
#define nt72_nand_cmd_history_init(...)
#else
void nt72_nand_cmd_history_init(void);
#endif

#ifndef ENABLE_CMD_HISTORY
#define nt72_nand_cmd_history_add(...)
#else
void nt72_nand_cmd_history_add(const char *cmd);
#endif

#ifndef ENABLE_CMD_HISTORY
#define nt72_nand_cmd_history_save_start_timestamp(...)
#else
void nt72_nand_cmd_history_save_start_timestamp(void);
#endif

#ifndef ENABLE_CMD_HISTORY
#define nt72_nand_cmd_history_save_end_timestamp(...)
#else
void nt72_nand_cmd_history_save_end_timestamp(void);
#endif

#ifndef ENABLE_CMD_HISTORY
#define nt72_nand_cmd_history_add_data(...)
#else
void nt72_nand_cmd_history_add_data(u32 data);
#endif

#ifndef ENABLE_CMD_HISTORY
#define nt72_nand_cmd_history_dump(...)
#else
void nt72_nand_cmd_history_dump(void);
#endif

#ifndef ENABLE_BUS_SNIFFER
#define nt72_nand_bus_sniffer_init(...)
#else
void nt72_nand_bus_sniffer_init(void);
#endif

#ifndef ENABLE_BUS_SNIFFER
#define nt72_nand_bus_sniffer_add(...)
#else
void nt72_nand_bus_sniffer_add(const char *op, u32 addr, u32 val);
#endif

#ifndef ENABLE_BUS_SNIFFER
#define nt72_nand_bus_sniffer_dump(...)
#else
void nt72_nand_bus_sniffer_dump(void);
#endif

#endif /* _NT72_NAND_UTILS_H_ */
