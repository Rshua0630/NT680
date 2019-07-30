#ifndef _NVT_PRIV_H_
#define _NVT_PRIV_H_

#include <linux/types.h>
#include <linux/skbuff.h>
#include <net/ieee80211_radiotap.h>

#include "nvt_cfg80211.h"
#include "nvt_util_dbg.h"

/* Host Header Length */
#define NVT_TX_HOSTHEADER       8
#define NVT_RX_HOSTHEADER       44


/* Rx Iconfig/Event Offset Value */
#define WID_EVENT_OFFSET    42
#define WID_VALUE_OFFSET    4
#define WID_LENGTH_OFFSET   2

/* Rx Iconfig/Event Message Type Definition */
#define EVENT               0x45
#define NETWORK_INFO        0x4E
#define RESPONSE            0x52
#define INFORMATION         0x49

/* Reorder Parameters Define */
#define NUM_TIDS                    8
#define ASSOC_NUM                   8
#define MAX_SEQNO_BY_TWO            2048
#define SEQNO_MASK                  0x0FFF
#define LESSER_THAN_SEQLO           0
#define GREATER_THAN_SEQHI          1
#define BETWEEN_SEQLO_SEQHI         2
#define BA_RECOVERY_TIMEOUT         1000 /* in ms */

/* Default BA Window Size */
#define MAX_NUM_BA_BUFFERS          32 /* Maximum Size of BA Window Size */
#define DEFAULT_NUM_BA_BUFFERS       16
#define BA_QUEUE_BUFF_SIZE \
        ((g_num_ba_buffers * (sizeof(struct _bfd_rx_pkt_t))))

#define SEQNO_ADD(seq1, seq2) (((seq1) + (seq2)) & SEQNO_MASK)
#define SEQNO_SUB(seq1, seq2) (((seq1) - (seq2)) & SEQNO_MASK)
#define RX_REORDER_BUF_TIMEOUT (HZ / 10)

/* Tx Parameters Define */
#define PUBLIC_WEIGHT       70 /* means 70% */

struct _nvt_adapter;

/* Cipher Type */
enum _cipher_t {
        WEP40 = 0,
        TKIP = 1,
        CCMP = 2,
        NO_ENCRYP = 3,
        WEP104 = 4
};

struct _bfd_rx_pkt_t {
        u8 in_use;
        u16 seq_num;
        struct sk_buff *pkt;
        enum _cipher_t ct;
        unsigned long reorder_time;
};

struct _ba_alarm_data_t {
        u8  tid;
        struct _nvt_adapter *entry;
};

struct _ba_rx_struct_t {
        u8 sta_index; /* Transmitter Index */
        u16 buff_size; /* Block-Ack Buffer Size */
        u16 timeout; /* Session Timeout interval */
        u16 win_start;/* Seqno of the first un-Acked MSDU */
        u16 win_end; /* Seqno of the last MSDU that can be Rxd */
        u16 win_tail; /* Highest Seqno currently present in the Q */
        u16 buff_cnt; /* Num Frames currently in Reorder-Q        */
        struct _ba_alarm_data_t ba_data; /* Timeout Callback Function Data */
        struct _bfd_rx_pkt_t *buff_list; /* Rx-Reorder Queue */
        u8 **rx_pn_val; /* Last Received PN Number */
        struct timer_list timer; /* Session Timeout interval */
        struct timer_list reorder_timer; /* Internal Reorder Timer */
        u16 debug_seq; /* Debugging Purpose */
};

struct _ba_struct_t {
        struct _ba_rx_struct_t *ba_rx_ptr;
        /* below variables are static */
        struct _nvt_adapter *entry;
        struct work_struct ba_timeout_work;
        u8 sta_index;
        u8 tid;
        u8 dst_addr[6];
        spinlock_t ba_lock;
};

enum nvt_tx_fifo {
        NV_TX_FIFO_AC_VO = 0,
        NV_TX_FIFO_AC_VI,
        NV_TX_FIFO_AC_BE,
        NV_TX_FIFO_AC_BK,
        NV_TX_FIFO_PUBLIC,
        NV_TX_FIFO_COUNT
};

enum nvt_tx_test_mode {
        NVT_TX_TEST_DISABLE = 0x0000,
        NVT_TX_START_POINT = 0x0001,
        NVT_TX_ENQUE_POINT = 0x0002,
        NVT_TX_DEQUE_POINT = 0x0004,
        NVT_TX_DIRECT = 0x0008,
        NVT_TX_TEST_MAX
};

struct _nvt_tx_test {
        u16 tx_test_mode;
        u32 tx_total_cnt;
        u32 tx_enque_cnt;
        u32 rx_enque_drop_cnt;
        u32 tx_deque_cnt;
        u32 tx_deque_drop_cnt;
        unsigned long start_time;
        unsigned long end_time;
};

enum _nvt_data_mode {
        stream_mode = 0x1,
        data_mode = 0x2,
};

#if 1  /* tx statistic */
typedef struct tag_edca_stats {
    u32     en;                 /**< en-que stats */
    u32     out;                /**< de-que stats */
    u32     out_sz;
    u32     drop;               /**< en-que fail(drop) stats */
} EDCA_STATS_T;
#endif

struct _tx_info {
        struct _nvt_adapter *nvt_adapter;
        spinlock_t lock;
        struct work_struct tx_queue_work;
        struct sk_buff_head bk_queue_list;
        struct sk_buff_head be_queue_list;
        struct sk_buff_head vi_queue_list;
        struct sk_buff_head vo_queue_list;

        u32 fifo_enqpkt[NV_TX_FIFO_COUNT];
        int fifo_credit[NV_TX_FIFO_COUNT];
        int fifo_max_credit[NV_TX_FIFO_COUNT];
        u8 fifo_credit_update[NV_TX_FIFO_COUNT];
        u8 fifo_public_inuse[NV_TX_FIFO_COUNT - 1];
        u8 fifo_public_update[NV_TX_FIFO_COUNT - 1];

        #if 1  /* tx statistic */
        EDCA_STATS_T stats[NV_TX_FIFO_COUNT - 1];     /* point edca stats */
        #endif

        u32 fifo_credit_map;
        u32 fifo_txqueue_map;
        bool bus_flow_blocked;
        bool fcmode;
        bool is_netif_stop;
        #ifndef CONFIG_WIFI_TUNING_PHASE_I
        u32 fc_hi_watermark;
        u32 fc_low_watermark;
        #else
        u32 fc_hi_watermark[NV_TX_FIFO_COUNT - 1];
        u32 fc_low_watermark[NV_TX_FIFO_COUNT - 1];
        #endif
        u32 max_qcnt;
        u32 max_fqcnt;
        u8 is_credit_borrow;

        struct _nvt_tx_test nvt_tx_test;
};

struct _nvt_priv {
        /* Define Iconfig/Event structure */
        struct work_struct fw_evt_work;
        struct sk_buff_head fw_evt_list;
        /* Define Reorder structure */
        struct _ba_struct_t ba_rx[ASSOC_NUM * NUM_TIDS];
        /* per TID plus one for non-QoS frames */
        u16 last_seq_ctl[ASSOC_NUM * NUM_TIDS + ASSOC_NUM];
        /*This is used to record the PN for the specific
         *(STA_IDX, TID) pair*/
        u8 *pn_unicast[ASSOC_NUM * NUM_TIDS];
        u8 *pn_bcmc;
        /* Define Tx Structure */
        struct _tx_info *tx_info;
        /* netlink for remote log */
        struct sock *remote_fwlog_sock;
        u16 remote_debug_start;    /* 0: remotedebug util didn't connect
                                   1: remotedebug util didn't connect*/
        u16 remote_debug_pid;

        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        struct list_head sta_list_in_ap;   /* Station status in AP mode*/
        #endif
};

#ifdef CONFIG_WIFI_TUNING_PHASE_II
struct _tp_status {
    #if 1
        u8 mac_addr[6];
        u8 event_length;
        u8 rate_idx;
        u8 status_bitmap[2];
        u8 report_period[2];
        u8 ac_vo_frame_in_q;
        u8 ac_vi_frame_in_q;
        u8 ac_be_frame_in_q;
        u8 ac_bk_frane_in_q;
        u8 ac_vo_txsz[4];
        u8 ac_vi_txsz[4];
        u8 ac_be_txsz[4];
        u8 ac_bk_txsz[4];
        u8 ac_vo_dropsz[4];
        u8 ac_vi_dropsz[4];
        u8 ac_be_dropsz[4];
        u8 ac_bk_dropsz[4];
        u8 ac_vo_dropcnt[2];
        u8 ac_vi_dropcnt[2];
        u8 ac_be_dropcnt[2];
        u8 ac_bk_dropcnt[2];
        u8 ac_vo_txframe[4];
        u8 ac_vi_txframe[4];
        u8 ac_be_txframe[4];
        u8 ac_bk_txframe[4];
    #else
        u8 vo_inst_tp[4];
        u8 vi_inst_tp[4];
        u8 be_inst_tp[4];
        u8 bk_inst_tp[4];
        u8 vo_avg_tp[4];
        u8 vi_avg_tp[4];
        u8 be_avg_tp[4];
        u8 bk_avg_tp[4];
        u8 vo_inst_qmu[4];
        u8 vi_inst_qmu[4];
        u8 be_inst_qmu[4];
        u8 bk_inst_qmu[4];
        u8 vo_avg_qmu[4];
        u8 vi_avg_qmu[4];
        u8 be_avg_qmu[4];
        u8 bk_avg_qmu[4];
        /*8 stations each with MAC_ADDR+rate_index[7bytes]*/
        u8 assoc_mac_rate[56];
    #endif
} __attribute__((packed));

#define MAC_Status_Len          sizeof(struct _tp_status)
#define MAC_Sts_mac_off         offsetof(struct _tp_status, mac_addr)
#define MAC_Sts_phyr_off        offsetof(struct _tp_status, rate_idx)
#define MAC_Sts_votxsz_off      offsetof(struct _tp_status, ac_vo_txsz)
#define MAC_Sts_betxsz_off      offsetof(struct _tp_status, ac_be_txsz)
#define MAC_Sts_votxfrm_off     offsetof(struct _tp_status, ac_vo_txframe)
#define MAC_Sts_betxfrm_off     offsetof(struct _tp_status, ac_be_txframe)
#define MAC_Sts_vodrpsz_off     offsetof(struct _tp_status, ac_vo_dropsz)
#define MAC_Sts_bedrpsz_off     offsetof(struct _tp_status, ac_be_dropsz)
#define MAC_Sts_vodrpfrm_off    offsetof(struct _tp_status, ac_vo_dropcnt)
#define MAC_Sts_bedrpfrm_off    offsetof(struct _tp_status, ac_be_dropcnt)
#endif

struct _nvt_hwinfo_tx {
        u32 word0;
        struct {
                u8 swtaillen;
                u8 offset;
                u16 pktlen;
        } word1;
};


struct _nvt_hwinfo_rx {
        u32 word0;
        struct {
                u8 swtaillen;
                u8 offset;
                u16 pktlen;
        } word1;
        u32 word2;
        struct {
                u8 rx_silence_level;
                u8 snr;
                u8 datarate;
                u8 rssi;
        } word3;
        struct {
                u8 sta_idx;
                u8 vo_piggy;
                //buffer index in AMSDU
                u8 buff_num_curr_mpdu;
                //BIT0 == 1 --> AMSDU
                //BIT1 == 1 --> START AMSDU BUFFER
                u8 amsdu_flags;
        } word4;
        struct {
                u8 num_amsdu;
                u8 vi_piggy;
                u8 be_piggy;
                u8 bk_piggy;

        } word5;
        struct {
                u32 pn_high;
        } word6;
        union {
                u32 value;
                struct {
                        u32 pn_low:16;
                        u32 cipher_type:4;
                        u32 cur_seq:12;
                } field;
        } word7;
};

struct nvt_rx_radiotap_hdr {
        struct ieee80211_radiotap_header hdr;
        u8 flags;
        u8 rate;
        u8 antsignal;
} __packed;

extern void nvt_reset_ba_handle(struct _nvt_adapter *nvt_adapter,
        u8 sta_index);
extern bool nvt_ba_process(struct _nvt_if *nvt_if,
        struct _nvt_hwinfo_rx *info, struct sk_buff *pkt, u8 ba_idx);
extern s32 nvt_is_priv_pkt(struct _nvt_bus *, struct sk_buff *);
extern s32 nvt_process_priv_pkt(struct _nvt_bus *, struct sk_buff *);
extern s32 nvt_priv_init(struct _nvt_adapter *);
extern s32 nvt_priv_deinit(struct _nvt_adapter *);
extern s32 nvt_reset_fcmode(struct _nvt_if *, u8);
extern void nvt_flush_txq(struct _nvt_adapter *);
extern s32 get_ba_index_by_hosthdr(struct _nvt_hwinfo_rx *info,
                struct sk_buff *skb);
extern bool nvt_check_sec_rx_mpdu(struct _nvt_adapter *nvt_adapter,
        enum _cipher_t ct, struct sk_buff *skb, u8 *old_pn_val);
extern void nvt_reset_unicast_pn(struct _nvt_adapter *nvt_adapter,
                u8 sta_index);
#endif
