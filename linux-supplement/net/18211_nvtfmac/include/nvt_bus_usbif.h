#ifndef _NVT_USB_H_
#define _NVT_USB_H_

#include <linux/usb.h>

#define NVT_USB_VENDOR_ID       0x0603
#define NVT_USB_DEVICE_ID       0x471a
#define NVT_RX_WATERMARK_L              32
#define NVT_RXQ_NUM                     (NVT_RX_WATERMARK_L << 1)
#define NVT_TX_WATERMARK_L              16
#define NVT_TX_WATERMARK_H              (16 * 3)
#define NVT_TXQ_NUM                     (NVT_TX_WATERMARK_L << 2)

#define NVT_USB_MTU                     1632

#define IF_PTR(usb, idx)                usb->actconfig->interface[idx]
#define IF_ALT_PTR(usb, idx, alt_idx)   IF_PTR(usb, idx)->altsetting[alt_idx]
#define IF_DSCR(usb, idx, alt_idx)      IF_ALT_PTR(usb, idx, alt_idx).desc
#define IF_EP_DSCR(usb, idx, alt_idx, ep)       \
                IF_ALT_PTR(usb, idx, alt_idx).endpoint[ep].desc

#define INTR_IN_EP_DSCR_SEQ             0
#define BULK_IN_EP_DSCR_SEQ             1
#define BULK_OUT_EP_DSCR_SEQ            2

#define NVT_CTRL_URB_TIMEOUT    3000

struct _nvt_wdev_usb;

enum nvt_usb_state {
        NVT_USB_STATE_DOWN,
        NVT_USB_STATE_FW_FAIL,
        NVT_USB_STATE_FW_RDY,
        NVT_USB_STATE_UP,
        NVT_USB_STATE_SLEEP
};

struct _usb_statistic {
        u32 tx_pkt_cnt;
        u32 tx_pkt_err_cnt;
        u32 rx_pkt_cnt;
        u32 rx_pkt_err_cnt;

        u32 tx_ctrlpkt_cnt;
        u32 tx_ctrlpkt_err_cnt;
        u32 rx_ctrlpkt_cnt;
        u32 rx_ctrlpkt_err_cnt;
};

struct _nvt_usb_reqs  {
        struct _nvt_wdev_usb *nvt_wdev_usb;
        struct list_head list;
        struct urb *urb;
        struct sk_buff  *skb;
};

struct _nvt_wdev_usb {
        struct device *dev;
        struct usb_device *usbdev;

        struct list_head rx_free_q;

        struct _usb_statistic usb_statistic;
        struct _nvt_usb_reqs *rx_reqs;
        u32 rx_ctrl_pipe;
        u32 tx_ctrl_pipe;
        u32 rx_bulk_pipe;
        u32 tx_bulk_pipe;

        struct urb *ctrl_urb;
        struct usb_ctrlrequest ctl_write;
        struct usb_ctrlrequest ctl_read;
        s32 ctl_urb_status;
        s32 ctl_completed;
        wait_queue_head_t ctl_resp_wait;
        ulong ctl_op;

        u32 rxq_num;
        u32 rx_wm_l;
        u32 txq_num;
        u32 tx_wm_l;
        u32 tx_wm_h;
        u32 mtu;
        u16 vendor_id;
        u16 product_id;

        enum nvt_usb_state state;
        u8 tx_blkurb_inuse_count;
        spinlock_t txflow_lock;
        spinlock_t qlock;

        //20150918 nash: work for rx data
        struct work_struct bulkrx_work;
        struct sk_buff_head skb_list;


};

extern void nvtfmac_usb_init(void);
extern void nvtfmac_usb_exit(void);
#endif
