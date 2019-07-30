#include "include/nt72_nand_config.h"
#include "include/nt72_nand_kapi.h"
#include "include/nt72_nand_utils.h"

#ifndef EXTRA_CHECK_CACHE_ALIGNMENT
#define check_cache_alignment(...)
#else
void check_cache_alignment(struct nt72_nand_info *info, const char *who,
			   unsigned char *addr)
{
	if ((u32)addr % info->nand_params.cache_line_size)
		nfc_err("nfc_err: %s is not aligned to cache\n", who);
}
#endif

#ifdef ENABLE_CMD_HISTORY
struct nt72_nand_cmd_history cmd_history;
#endif

#ifdef ENABLE_CMD_HISTORY
u64 nt72_nand_cmd_history_get_timestamp_diff(int i)
{
	u32 start_s = cmd_history.records[i].start_s;
	u32 start_us = cmd_history.records[i].start_us;
	u32 end_s = cmd_history.records[i].end_s;
	u32 end_us = cmd_history.records[i].end_us;

	if (likely(start_s == end_s))
		return (u64)end_us - start_us;

	return (u64)((u64)end_s * 1000000 + end_us) - ((u64)start_s * 1000000 +
						       start_us);
}
#endif

#ifdef ENABLE_CMD_HISTORY
void nt72_nand_cmd_history_init(void)
{
	int i;

	memset(&cmd_history, 0, sizeof(cmd_history));
	for (i = 0; i < CMD_HISTORY_SIZE; i++)
		strcpy(cmd_history.records[i].command, "xxx");
	cmd_history.head = 0;
	cmd_history.tail = CMD_HISTORY_SIZE - 1;
}
#endif

#ifdef ENABLE_CMD_HISTORY
void nt72_nand_cmd_history_add(const char *cmd)
{
	cmd_history.head++;
	if (cmd_history.head == CMD_HISTORY_SIZE)
		cmd_history.head = 0;
	cmd_history.tail++;
	if (cmd_history.tail == CMD_HISTORY_SIZE)
		cmd_history.tail = 0;
	strncpy(cmd_history.records[cmd_history.tail].command, cmd,
		sizeof(cmd_history.records[cmd_history.tail].command));
	cmd_history.records[cmd_history.tail].data_valid = 0;
}
#endif

#ifdef ENABLE_CMD_HISTORY
void nt72_nand_cmd_history_save_start_timestamp(void)
{
	nt72_nand_kapi_get_timestamp(
		&(cmd_history.records[cmd_history.tail].start_s),
		&(cmd_history.records[cmd_history.tail].start_us));
}
#endif

#ifdef ENABLE_CMD_HISTORY
void nt72_nand_cmd_history_save_end_timestamp(void)
{
	nt72_nand_kapi_get_timestamp(
		&(cmd_history.records[cmd_history.tail].end_s),
		&(cmd_history.records[cmd_history.tail].end_us));
}
#endif

#ifdef ENABLE_CMD_HISTORY
void nt72_nand_cmd_history_add_data(u32 data)
{
	cmd_history.records[cmd_history.tail].data = data;
	cmd_history.records[cmd_history.tail].data_valid = 1;
}
#endif

#ifdef ENABLE_CMD_HISTORY
void nt72_nand_cmd_history_dump(void)
{
	int i;

	nfc_dbg("nfc_dbg: start dump cmd history\n");
	for (i = cmd_history.head;; i++) {
		if (i == CMD_HISTORY_SIZE)
			i = 0;
		if (cmd_history.records[i].data_valid) {
			nfc_dbg("command: %s\t%llu us\t0x%02X\n",
				cmd_history.records[i].command,
				nt72_nand_cmd_history_get_timestamp_diff(i),
				cmd_history.records[i].data);
		} else {
			nfc_dbg("command: %s\t%llu us\n",
				cmd_history.records[i].command,
				nt72_nand_cmd_history_get_timestamp_diff(i));
		}
		if (i == cmd_history.tail)
			break;
	}
	nfc_dbg("nfc_dbg: dump cmd history end\n");
}
#endif

#ifdef ENABLE_BUS_SNIFFER
struct nt72_nand_bus_sniffer sniffer;
#endif

#ifdef ENABLE_BUS_SNIFFER
void nt72_nand_bus_sniffer_init(void)
{
	int i;

	memset(&sniffer, 0, sizeof(sniffer));
	for (i = 0; i < BUS_SNIFFER_SIZE; i++) {
		strcpy(sniffer.records[i].op, "xxx");
		sniffer.records[i].addr = 0;
		sniffer.records[i].val = 0;
	}
	sniffer.head = 0;
	sniffer.tail = BUS_SNIFFER_SIZE - 1;
}
#endif

#ifdef ENABLE_BUS_SNIFFER
void nt72_nand_bus_sniffer_add(const char *op, u32 addr, u32 val)
{
	sniffer.head++;
	if (sniffer.head == BUS_SNIFFER_SIZE)
		sniffer.head = 0;
	sniffer.tail++;
	if (sniffer.tail == BUS_SNIFFER_SIZE)
		sniffer.tail = 0;
	strncpy(sniffer.records[sniffer.tail].op, op,
		sizeof(sniffer.records[sniffer.tail].op));
	sniffer.records[sniffer.tail].addr = (u16)addr;
	sniffer.records[sniffer.tail].val = val;
}
#endif

#ifdef ENABLE_BUS_SNIFFER
void nt72_nand_bus_sniffer_dump(void)
{
	int i;

	nfc_dbg("nfc_dbg: start dump bus sniffer\n");
	for (i = sniffer.head;; i++) {
		if (i == BUS_SNIFFER_SIZE)
			i = 0;
		pr_cont("%s:%04X=%08X ",
			sniffer.records[i].op,
			sniffer.records[i].addr,
			sniffer.records[i].val);
		if (i == sniffer.tail)
			break;
	}
	nfc_dbg("nfc_dbg: dump bus sniffer end\n");
}
#endif
