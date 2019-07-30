#ifndef _NT72_NAND_NFC_H_
#define _NT72_NAND_NFC_H_
#include <linux/mtd/nand.h>
#include <linux/semaphore.h>
#include <linux/completion.h>

#include "nt72_nand_config.h"
#include "nt72_nand_plat.h"

enum nt72_nand_ecc_type {
	RS,
	BCH,
};

struct nt72_nand_ecc_param {
	enum nt72_nand_ecc_type type;
	u16			nr_bits;
	u16			data_size;
	u16			extra_size;
};

enum {
	ERR_NONE = 0,
	ERR_ECC_FAIL = -1,
	ERR_TIMEOUT = -2,
	ERR_STATUS = -3,
	ERR_WERROR = -4,
	ERR_EERROR = -5,
};

struct nt72_nand_ecc_result {
	int	err_count;
	int	uncorrectable;
	int	blank;
	int	_err_count[MAX_NR_SUBPAGE];
	int	_uncorrectable[MAX_NR_SUBPAGE];
	int	_blank[MAX_NR_SUBPAGE];
};

struct nt72_nand_regs {
	u32	xd_cfg;

	u32	cfg0_read;
	u32	cfg0_prog;

	u32	cfg1;

	u32	sysctrl;

	u32	sysctrl1_read;
	u32	sysctrl1_prog;

	u32	sysctrl2_read;
	u32	sysctrl2_prog;

	u32	ftune;
};

struct nt72_nand_params {
	u32				chip_id;
	u16				flash_width;

	u32				page_size;
	u32				subpage_size;
	u32				oob_size;
	u32				suboob_size;

	u32				nr_blocks;
	u32				nr_page_per_block;
	u32				nr_subpage;

	u64				size;
	u32				dma_buf_size;

	u32				ic_id;
	u32				cache_line_size;
	u32				blank_0xff_threshold;
	u32				blank_non_0xff_threshold;
	u32				bitflip_threshold;
	u32				bbt_options;

	struct nt72_nand_ecc_param	ecc;
	struct nt72_nand_regs		regs;
};

struct nt72_nand_info {
	/* linux-specific data structures */
	struct nand_chip		chip;

	struct mtd_info			*mtd;
	struct device			*dev;
	struct platform_device		*pdev;

	dma_addr_t			dma_data_buff_pa;
	struct semaphore		sem_nfc;
	struct completion		nfc_completion;

	/* general data structures */
	int				target_chip;
	int				nand_scanning;

	int				buf_start;
	int				buf_count;

	unsigned char			*ff_pattern;
	unsigned char			*data_buff;

	int				seqin_column;
	int				seqin_page_addr;

	struct nt72_nand_params		nand_params;
	struct nt72_nand_ecc_result	ecc_result;

	int				retcode;
};


#ifndef DUMP_CONTROL_REGS
#define nt72_nand_nfc_dump_ctrl_regs(...)
#else
void nt72_nand_nfc_dump_ctrl_regs(const char *who);
#endif

#ifndef DUMP_REG_ON_ERROR
#define nt72_nand_nfc_dump_regs(...)
#else
void nt72_nand_nfc_dump_regs(struct nt72_nand_info *info, int page_addr);
#endif

#ifndef DUMP_DATA_ON_ERROR
#define nt72_nand_nfc_dump_data(...)
#else
void nt72_nand_nfc_dump_data(struct nt72_nand_info *info, int page_addr);
#endif

#ifndef DUMP_ECC_WHEN_UNCORRECTABLE
#define nt72_nand_nfc_dump_ecc(...)
#else
void nt72_nand_nfc_dump_ecc(struct nt72_nand_info *info);
#endif

#ifndef EXTRA_WAIT_NFC_READY
#define nt72_nand_nfc_wait_nfc_ready(...)
#else
void nt72_nand_nfc_wait_nfc_ready(const char *who);
#endif

void nt72_nand_nfc_ecc_workaround(struct nt72_nand_info *info);
int nt72_nand_nfc_is_bad_block(struct nt72_nand_info *info, int page_addr);
int nt72_nand_nfc_count_non_ff64(u64 *data);
void nt72_nand_nfc_fill_blank_subpage(struct nt72_nand_info *info,
				      int page_addr, int subpage_id,
				      int oob_only);
void nt72_nand_nfc_fill_blank_page(struct nt72_nand_info *info, int page_addr,
				   int oob_only);

void nt72_nand_nfc_hw_reset(void);
void nt72_nand_nfc_sw_reset(void);
void nt72_nand_nfc_ecc_reset(struct nt72_nand_info *info);
int nt72_nand_nfc_check_int_status(const char *who, struct nt72_nand_info *info,
				   int page_addr, int read, int oob_only);
void nt72_nand_nfc_prepare_pio_regs(struct nt72_nand_info *info);
void nt72_nand_nfc_prepare_rw_regs(struct nt72_nand_info *info, int write);
void nt72_nand_nfc_prepare_rw_dma_regs(struct nt72_nand_info *info, int column,
				       int page_addr,
				       int write);
void nt72_nand_nfc_send_piocmd(struct nt72_nand_info *info, const int busy_wait,
			       const u32 cmd);
void nt72_nand_nfc_send_rwcmd(struct nt72_nand_info *info, int column,
			      int page_addr,
			      int write);
void nt72_nand_nfc_get_ecc_result(struct nt72_nand_info *info);
void nt72_nand_nfc_read_id(struct nt72_nand_info *info, int column, u32 *id,
			   int probe);
void nt72_nand_nfc_read_status(struct nt72_nand_info *info, uint32_t *status);
void nt72_nand_nfc_read_page(struct nt72_nand_info *info, int column,
			     int page_addr, int use_ecc,
			     int oob_only);
void nt72_nand_nfc_erase_block(struct nt72_nand_info *info, int page_addr);
void nt72_nand_nfc_write_page(struct nt72_nand_info *info, int column,
			      int page_addr);

#endif /* #define _NT72_NAND_NFC_H_ */
