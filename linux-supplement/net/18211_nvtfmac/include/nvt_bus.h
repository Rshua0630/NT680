#ifndef _NVT_BUS_H_
#define _NVT_BUS_H_

#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#include "nvt_diag.h"
#include "nvt_bus_usbif.h"
#include "nvt_bus_sdioif.h"
#include "nvt_bus_mailboxif.h"


/**
 *@NVT_BUS_STATE_DOWN: bus is not ready for transfering
 *@NVT_BUS_STATE_UP: bus is ready for transfering
 *
 */
enum nvt_bus_state {
        NVT_BUS_STATE_DOWN,
        NVT_BUS_STATE_UP,
};

struct _nvt_bus;

struct _nvt_wdev_bus_ops {
        s32 (*init)(struct _nvt_bus *);
        s32 (*stop)(struct _nvt_bus *);
        s32 (*tx_data)(struct _nvt_bus *, struct sk_buff *);
        s32 (*tx_ctrl)(struct _nvt_bus *, u8 *data, u32 data_len);
        s32 (*rx_ctrl)(struct _nvt_bus *, u8 *data, u32 data_len);
        s32 (*suspend)(struct _nvt_bus *);
        s32 (*resume)(struct _nvt_bus *);

};

struct _nvt_bus_statistic {
        u64 pkt_tx_cnt;
        u64 pkt_rx_cnt;
        u64 pkt_tx_err_cnt;
        u64 pkt_rx_err_cnt;

        u64 ctl_tx_cnt;
        u64 ctl_rx_cnt;
        u64 ctl_tx_err_cnt;
        u64 ctl_rx_err_cnt;
};

#ifdef BUS_UTILIZATION
enum nvt_bus_utilization_tx_mode {
        NVT_BUS_BU_TX_VARIABLE_LEN = 0,
        NVT_BUS_BU_TX_ALL_ONE,
        NVT_BUS_BU_TX_ALL_ZERO,
        NVT_BUS_BU_TX_SEQ,
        NVT_BUS_BU_TX_RANDOM,
        NVT_BUS_BU_TX_MAX
};

struct _nvt_bus_utilization {
        bool tx_enable;
        bool rx_enable;
        u32 tx_mode;
        s32 tx_total_cnt;
        s32 tx_runtime_cnt;
        s32 rx_total_cnt;
        s32 rx_runtime_cnt;
        u64 start_time;
        u64 end_time;
        u64 total_pkt_size_in_byte;
};
#endif

struct _nvt_bus {
        u32 chip;
        u32 chip_rev;
        union {
                struct _nvt_wdev_usb *nvt_wdev_usb;
                struct _nvt_wdev_sdio *nvt_wdev_sdio;
                struct _nvt_wdev_mailbox wdev_mailbox;
        } type;
        struct _nvt_wdev_bus_ops nvt_wdev_bus_ops;
        struct _nvt_diag nvt_diag;
        struct device *dev;
        struct _nvt_adapter *nvt_adapter;
        enum nvt_bus_state state;
        struct _nvt_bus_statistic statistic;
        struct _nvt_icfg_pkt_buff *icfg_pkg_buff;
        void (*tx_done_callback)(struct sk_buff *, s32 status_code);
        s32 fw_rdy_completed;
        wait_queue_head_t fw_rdy_wait;

#ifdef BUS_UTILIZATION
        struct _nvt_bus_utilization *nvt_bu;
#endif

};

extern s32 nvt_bus_txdata(struct _nvt_bus *nvt_bus, struct sk_buff *skb);
extern struct _nvt_bus *nvt_bus_attach(struct device *,
        struct _nvt_wdev_bus_ops);
extern s32 nvt_bus_detach(struct device *);
extern s32 nvt_bus_register_txdone_callback(struct _nvt_bus *nvt_bus,
        void (*txdone_cb)(struct sk_buff *, s32 status_code));

#ifdef BUS_UTILIZATION
extern s32 nvt_bus_bu_tx_on(struct _nvt_bus *nvt_bus, bool on,
        u32 pkt_cnt);
extern s32 nvt_bus_bu_rx_on(struct _nvt_bus *nvt_bus, bool on,
        u32 pkt_cnt);
extern void nvt_bus_bu_show_result(struct _nvt_bus *nvt_bus, bool is_tx);

#endif


#endif

