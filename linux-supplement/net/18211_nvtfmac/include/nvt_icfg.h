#ifndef _NVT_ICFG_H_
#define _NVT_ICFG_H_

#include <linux/mutex.h>

#define NVT_ICFG_MAX_MSG_BODY   1024
#define NVT_ICFG_MSG_HDR_LEN             4
#define NVT_ICFG_WID_SIZE 2

#define NVT_ICFG_CHAR_CMD_LEN 4
#define NVT_ICFG_SHORT_CMD_LEN 5
#define NVT_ICFG_INT_CMD_LEN 7
#define NVT_ICFG_STR_SET_CMD_LEN 3
#define NVT_ICFG_STR_GET_CMD_LEN 2
#define NVT_ICFG_BIN_SET_CMD_LEN 5
#define NVT_ICFG_BIN_GET_CMD_LEN 2

#define NVT_ICFG_SHORT_WID_START 0x1000
#define NVT_ICFG_INT_WID_START 0x2000
#define NVT_ICFG_STR_WID_START 0x3000
#define NVT_ICFG_BIN_WID_START 0x4000
#define NVT_ICFG_MAX_WID_START 0x5000

#define NVT_ICFG_CHAR_WID_LEN 1
#define NVT_ICFG_SHORT_WID_LEN 2
#define NVT_ICFG_INT_WID_LEN 4

#define NVT_ICFG_BIN_WID_BIN_LEN 2

struct _nvt_bus;

enum nvt_icfg_dir {
        NVT_ICFG_GET = 0,
        NVT_ICFG_SET,
        NVT_ICFG_DIR_MAX,
};

struct _nvt_icfg_pkt_buff {

        u8     msg_type;
        u8     seq;
        u8     msg_len[2];
        u8     msg_body[NVT_ICFG_MAX_MSG_BODY];
        u16    cur_index;
        struct mutex iconfig_mutex;
};

extern s32 nvt_icfg_lock(struct _nvt_bus *nvt_bus);
extern s32 nvt_icfg_unlock(struct _nvt_bus *nvt_bus);

extern s32 nvt_icfg_reset(struct _nvt_bus *nvt_bus);
extern s32 nvt_icfg_add(struct _nvt_bus *nvt_bus, enum nvt_icfg_dir dir,
        u32 wid, u8 *wid_val, u16 size);
extern s32 nvt_icfg_send_and_get_resp(struct _nvt_bus *nvt_bus,
        u8 *resp, u16 resp_buf_size);
extern s32 nvt_icfg_get_wid_value(u8*, u32, u8*);
extern s32 nvt_get_wid_value(struct _nvt_bus *nvt_bus, u32 wid, u8 *resp,
        u32 buff_size, u8 *wid_value);
#endif
