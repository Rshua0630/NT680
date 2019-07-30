#ifndef _NVT_SDIO_H_
#define _NVT_SDIO_H_

#include <linux/types.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_func.h>

#include "nvt_bus_sdioif_18202reg.h"

//20160222 nash:
#define NVT_SDIO_CFG_AUTO_PKT_DONE             1
#define NVT_SDIO_GET_AVAI_SPACE_BEFORE_TX      0

#define NVT_SDIO_VENDOR_ID              0x4950
#define NVT_SDIO_DEVICE_ID_18202        0x0202

#define NVT_SDIO_F1_BLOCKSIZE           256

#define NVT_SDIO_FUNC0                  0
#define NVT_SDIO_FUNC1                  1

#define NVT_SDIO_ACCESS_RETRY_LIMIT     10
#define NVT_SDIO_ACCESS_SLEEP_US        1000

#define NVT_SDIO_CTRL_MSG_TIMEOUT    3000

#define NVT_SDIO_RX_BRUST_NUM           15
enum nvt_sdio_rw_type {
        NVT_SDIO_CHUNK_DATA = 0,
        NVT_SDIO_FIFO_DATA,
        NVT_SDIO_RW_TYPE_MAX
};

struct _nvt_wdev_sdio {
        //from brcmf_sdio_dev
        struct device *dev;
        struct sdio_func *func[SDIO_MAX_FUNCS];
        u8 num_funcs;   /* Supported funcs on client */
        u8 state;
        u8 flowcontrol; /* per prio flow control bitmask */
        u8 reserved1;   /* for align */

        atomic_t suspend;               /* suspend flag */
        wait_queue_head_t req_byte_wait;
        wait_queue_head_t req_word_wait;
        wait_queue_head_t req_buffer_wait;

        wait_queue_head_t ctrl_msg_resp_wait;
        s32 ctrl_msg_resp_rdy;
        //from brcmf_sdio_dev:end


        //nash:TODO??
        //struct chip_info *ci;   /* Chip info struct */

        //u32 ramsize;            /* Size of RAM in SOCRAM (bytes) */

        //u32 hostintmask;        /* Copy of Host Interrupt Mask */
        atomic_t intstatus;     /* Intstatus bits (events) pending */
        atomic_t fcstate;       /* State of dongle flow-control */

        u32 blocksize;         /* Block size of SDIO transfers */
        //uint roundup;           /* Max roundup limit */

        //struct pktq txq;        /* Queue length used for flow-control */

        //u8 tx_seq;              /* Transmit sequence number (next) */
        //u8 tx_max;              /* Maximum transmit sequence allowed */

        //u8 hdrbuf[MAX_HDR_READ + BRCMF_SDALIGN];
        //u8 *rxhdr;              /* Header of current rx frame (in hdrbuf) */
        //u8 rx_seq;              /* Receive sequence number (expected) */
        //struct brcmf_sdio_hdrinfo cur_read;
                                /* info of current read frame */
        //bool rxskip;            /* Skip receive (awaiting NAK ACK) */
        bool rxpending;         /* Data frame pending in dongle */

        //uint rxbound;           /* Rx frames to read before resched */
        //uint txbound;           /* Tx frames to send before resched */
        u32 txminmax;

        //struct sk_buff *glomd;  /* Packet containing glomming descriptor */
        //struct sk_buff_head glom; /* Packet list for glommed superframe */
        //uint glomerr;           /* Glom packet read errors */

        u32 pkt_len;    /* The length of incoming paekt from air*/
        //u8 *rxbuf;              /* Buffer for receiving control packets */
        //uint rxblen;            /* Allocated length of rxbuf */
        //u8 *rxctl;              /* Aligned pointer into rxbuf */
        //u8 *rxctl_orig;         /* pointer for freeing rxctl */
        //uint rxlen;             /* Length of valid data in buffer */
        //spinlock_t rxctl_lock;  /* protection lock for ctrl frame resources */

        //u8 sdpcm_ver;   /* Bus protocol reported by dongle */

        //bool intr;              /* Use interrupts */
        //bool poll;              /* Use polling */
        bool is_polling;        /* true: use polling, false: use interrupts */
        atomic_t int_pending;   /* Device interrupt is pending */
        //uint spurious;          /* Count of spurious interrupts */
        u32 pollrate;          /* Ticks between device polls */
        u32 polltick;          /* Tick counter */

        //u32 clkstate;          /* State of sd and backplane clock(s) */
        bool activity;          /* Activity flag for clock down */
        s32 idletime;           /* Control for activity timeout */
        s32 idlecount;  /* Activity timeout counter */
        //s32 idleclock;  /* How to set bus driver when idle */
        //bool rxflow_mode;       /* Rx flow control mode */
        //bool rxflow;            /* Is rx flow control on */
        //bool alp_only;          /* Don't use HT clock (ALP only) */

        //u8 *ctrl_frame_buf;
        //u32 ctrl_frame_len;
        //bool ctrl_frame_stat;


        spinlock_t txqlock;

        //wait_queue_head_t ctrl_wait;
        //wait_queue_head_t dcmd_resp_wait;
        //wait_queue_head_t ctrl_resp_wait;

        //nash:do we need watch dog??
        struct timer_list timer;
        struct completion watchdog_wait;
        struct task_struct *watchdog_tsk;
        bool wd_timer_valid;
        uint save_ms;

        //struct workqueue_struct *brcmf_wq;
        struct work_struct datahdl_work;
        atomic_t datahdl_tskcnt;
        struct sk_buff_head tx_skb_list;
        struct sk_buff_head rx_skb_list;


        //nash: if no TX queue, we do not need it
        bool txoff;             /* Transmit flow-controlled */

        //nash:??
        bool sr_enabled; /* SaveRestore enabled */

        //u8 tx_hdrlen;           /* sdio bus header length for tx packet */

        //statistic counter
        u32 rx_int_cnt;

};

extern void nvtfmac_sdio_init(void);
extern void nvtfmac_sdio_exit(void);

//nash:work around because nt96660 mmc host driver suspend/resume not ready yet
extern s32 nvt_sdio_wa_suspend(struct _nvt_bus *nvt_bus);
extern s32 nvt_sdio_wa_resume(struct _nvt_bus *nvt_bus);

#endif
