#ifndef _NVT_DBG_H_
#define _NVT_DBG_H_

#define NVT_DBG_REMOTEDBG               0x00080000
#define NVT_DBG_REORDER                 0x00100000
#define NVT_DBG_TX                      0x00200000
#define NVT_DBG_FW                      0x00400000
#define NVT_DBG_CLEARVIEW               0x00800000
#define NVT_DBG_P2P                     0x01000000
#define NVT_DBG_CFG80211                0x02000000
#define NVT_DBG_DIAG                    0x04000000
#define NVT_DBG_USB                     0x08000000
#define NVT_DBG_SDIO                    0x10000000
#define NVT_DBG_INFO                    0x20000000
#define NVT_DBG_WARN                    0x40000000
#define NVT_DBG_ERROR                   0x80000000

#ifdef DEBUG
extern u32 nvt_dbg_level;

void _nvt_dbg(u32 level, const char *fmt, ...);
#define nvt_dbg(level, fmt, ...)                \
do {                                            \
        if (NVT_DBG_##level & nvt_dbg_level)    \
                pr_debug(fmt, ##__VA_ARGS__);   \
} while (0)
#else
#define nvt_dbg(fmt, ...)
#endif

#endif
