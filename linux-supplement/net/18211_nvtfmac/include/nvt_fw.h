#ifndef _NVT_FW_H_
#define _NVT_FW_H_
#include "nvt_wlan_linux.h"

#define NVT_FW_NAME_STA "nvt/dsw_18211_sta.img"
#define NVT_FW_NAME_AP  "nvt/dsw_18211_ap.img"
#define NVT_FW_NAME_HUT  "nvt/dsw_18211_hut.img"
#define NVT_FW_CONFIG_PATCH_FILE_NAME "nvt/cpf_linux.cpf"

#define NVT_FW_CFG_DOWNLOAD_BY_CHIPID    0

#define ELF_SEEK_SET    1
#define NVT_FW_IMAGE_HEADER_LEN 32
#define NVT_FW_FM_GO_ADDR 0xE413F800

#define NVT_CONFIG_PATCH_FILE_HEADER_LEN 64
#define NVT_CONFIG_IMG_SECTOR_REG_COUNT 2
#define NVT_CONFIG_IMG_SECTOR_MASK 1
#define NVT_CONFIG_IMG_SECTOR_VAL 1
#define NVT_CONFIG_IMG_SECTOR_READ_MAX_LEN 1024

enum nvt_fw_mode {
        NVT_FW_STA = 0,
        NVT_FW_AP,
        NVT_FW_HUT,
        NVT_FW_NUM
};

enum nvt_fw_type {
        NVT_FW_TYPE_BOOTLOADER = 0,
        NVT_FW_TYPE_STA,
        NVT_FW_TYPE_AP,
        NVT_FW_TYPE_MAX
};

/* AT define MAC FW Capability */
#define FW_MAC_CAP_1_INTERNAL_SUPP_CAP (1 << 3)
#define FW_MAC_CAP_2_PMF_CAP (1 << 2)
#define FW_MAC_CAP_2_WAPI_CAP (1 << 3)

/* define EXTRA FW Capability_1 */
#define FW_CAPABILITY_SEND_MAC_STATUS (1 << 3)
#define FW_CAPABILITY_AP_STA_CONCURRENT (1 << 4)
#define FW_CAPABILITY_STREAMING_MODE (1 << 5)
#define FW_CAPABILITY_WOW (1 << 6)

//20150709 nash:
#define NVT_FW_DEFAULT_FW_MODE  NVT_FW_STA

struct _nvt_bus;
struct _nvt_if;

struct _nvt_config_patch_file_header {
        u16 project_code;
        u16 reserved1;
        u32 total_length;
        u64 reserved2;
        u8 version[16];
        u8 build_date[16];
        u8 reserved_section3[15];
        u8 crc;
};

struct _nvt_config_img_sector_header {
        u16 sector_type;
        u16 offset;
        u32 total_length;
        u32 write_addr;
};

#if (NVT_FW_CFG_DOWNLOAD_BY_CHIPID == 1)
struct _nvt_fw_name {
        s32 chip_id;
        enum nvt_fw_mode mode;
        s8 *filename;
};
#endif


extern s32 nvt_fw_download(struct _nvt_bus *, enum nvt_fw_mode);
extern s32 nvt_fw_change_opmode(struct _nvt_if *, enum nvt_fw_mode);
extern s32 nvt_fw_cold_boot(struct _nvt_if *);
extern s32 nvt_fw_get_ver(struct _nvt_bus *nvt_bus, u8 *ver, u32 *ver_len);
extern s32 nvt_fw_deep_sleep_resume(struct _nvt_bus *bus);
extern s32 nvt_fw_request_cpf(struct _nvt_bus *bus);
extern void nvt_set_firmware_capability(struct _nvt_adapter *nvt_adapter);
#endif
