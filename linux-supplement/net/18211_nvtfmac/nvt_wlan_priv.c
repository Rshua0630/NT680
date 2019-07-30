#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <net/sock.h>
#include <net/netlink.h>
#include "nvt_wlan_linux.h"
#include "nvt_wlan_priv.h"

#ifdef CONFIG_WIFI_TUNING_PHASE_II
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(a) ((a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5])

#define QP_turning_point_1          (70 * 8 * 1000)      /* TBR: 70 KBps */
#define QP_turning_point_2          (50 * 8 * 1000)      /* TBR: 50 KBps */
#define QP_turning_point_3          (20 * 8 * 1000)      /* TBR: 20 KBps */

//#define VCLEAR_INCR_TP_THRES        4
//#define VCLEAR_MACBEDROP_THRES      3

//#define VCLEAR_MAX_TP         (300 * 8 * 1024)       /* TBR: 2.4 Mbps */
#define VCLEAR_MAX_TP         (125 * 8 * 1000)       /* TBR: 125 KBps (1Mbps) */
#define VCLEAR_MIN_TP         (10 * 8 * 1000)        /* TBR: 10 KBps */

#define TPUT_IN_PHY_1_0       (1000 * 1000 / 2)      /* TBR: 62.5 KBps */
#define TPUT_IN_PHY_2_0       (81 * 8 * 1000)        /* TBR: 81 KBps */

#define TP_MONITOR_PHYRATE_TBL_SZ   28
// {x, y} : x means the value from fw, y is phyrate being round down to the
// nearest whole value.
u32 tp_monitor_phyrate_map_tbl[TP_MONITOR_PHYRATE_TBL_SZ][2] = {
    // b mode
    {0x04,  1000}, {0x01,  2000}, {0x02,  5500}, {0x03, 11000},
    // g mode
    {0x0b,  6000}, {0x0f,  9000}, {0x0a, 12000}, {0x0e, 18000}, {0x09, 24000},
    {0x0d, 36000}, {0x08, 48000}, {0x0c, 54000},
    // n mode with SGI
    {0xc0,  7200}, {0xc1, 14400}, {0xc2, 21700}, {0xc3, 28900}, {0xc4, 43300},
    {0xc5, 57800}, {0xc6, 65000}, {0xc7, 72200},
    // n mode with LGI
    {0x80,  6500}, {0x81, 13000}, {0x82, 19500}, {0x83, 26000}, {0x84, 39000},
    {0x85, 52000}, {0x86, 58500}, {0x87, 65000}
};
#endif

u16 g_num_ba_buffers = DEFAULT_NUM_BA_BUFFERS;

static void update_piggyback_fifocredit(struct _nvt_if *nvt_if,
        struct _nvt_hwinfo_rx *info);


/* This function checks whether seq1 is less than or equal to seq2 */
static inline bool nvt_seqno_leq(u16 seq1, u16 seq2)
{
        if (((seq1 <= seq2) && ((seq2 - seq1) < MAX_SEQNO_BY_TWO)) ||
           ((seq1 > seq2) && ((seq1 - seq2) > MAX_SEQNO_BY_TWO))) {
                return true;
        }
        return false;
}

/* This function checks whether seq1 is greater than or equal to seq2 */
static inline bool nvt_seqno_geq(u16 seq1, u16 seq2)
{
        return nvt_seqno_leq(seq2, seq1);

}

static inline bool nvt_seqno_lt(u16 seq1, u16 seq2)
{
        if (((seq1 < seq2) && ((seq2 - seq1) < MAX_SEQNO_BY_TWO)) ||
            ((seq1 > seq2) && ((seq1 - seq2) > MAX_SEQNO_BY_TWO))) {
                return true;
        }
        return false;
}

static inline bool nvt_seqno_gt(u16 seq1, u16 seq2)
{
    return nvt_seqno_lt(seq2, seq1);
}

/*
 * This function compares the given sequence number with the specified
 * upper and lower bounds and returns its position relative to them.
*/
static inline u8 nvt_seqno_bound_chk(u16 seq_lo, u16 seq_hi, u16 seq)
{
        bool lo_chk = nvt_seqno_leq(seq_lo, seq);
        bool hi_chk = nvt_seqno_leq(seq, seq_hi);
        u8 chk_res = 0;

        if ((true == lo_chk) && (true == hi_chk)) {
                chk_res = BETWEEN_SEQLO_SEQHI;
        } else if (true == lo_chk) {
                chk_res = GREATER_THAN_SEQHI;
        } else {
                chk_res = LESSER_THAN_SEQLO;
        }
        return chk_res;
}

static bool nvt_cmp_pn_val(u8 *pn_val_old, struct sk_buff *skb)
{
        struct _nvt_hwinfo_rx *info = NULL;
        bool ret_val = false;
        u32 old_val_low = 0;
        u32 old_val_high = 0;
        u32 rx_val_low = 0;
        u32 rx_val_high = 0;
        u8 pn_val_rx[6];

        info = (struct _nvt_hwinfo_rx *)skb->data;
        rx_val_low = (ntohl(info->word7.value) & 0xffff0000) >> 16;
        rx_val_high = ntohl(info->word6.pn_high);
        memcpy(&pn_val_rx[0], (u8 *)&rx_val_low, 2);
        memcpy(&pn_val_rx[2], (u8 *)&rx_val_high, 4);

        old_val_low = pn_val_old[0] | (pn_val_old[1] << 8) |
                (pn_val_old[2] << 16) | (pn_val_old[3] << 24);
        old_val_high = pn_val_old[4] | (pn_val_old[5] << 8);

        rx_val_low = pn_val_rx[0] | (pn_val_rx[1] << 8) |
                (pn_val_rx[2] << 16) | (pn_val_rx[3] << 24);
        rx_val_high = pn_val_rx[4] | (pn_val_rx[5] << 8);

        nvt_dbg(REORDER, "New PN = 0x%x_%x\n", rx_val_high, rx_val_low);
        nvt_dbg(REORDER, "Old PN = 0x%x_%x\n", old_val_high, old_val_low);

        if (rx_val_low == 0 && rx_val_high == 0) {
                nvt_dbg(REORDER, "PN is all zero, do not check PN value!\n");
                return true;
        }

        if (rx_val_high > 0x0000f0ff) {
                nvt_dbg(REORDER, "PN wrap-around, don't check PN/reset PN\n");
                memset(pn_val_old, 0, 6);
                return true;
        }

        /*
         * If the received PN Value is newer, that the old value is updated
         * and successful comparision is returned.
        */
        if (((old_val_high == rx_val_high) && (old_val_low < rx_val_low)) ||
            (old_val_high < rx_val_high)) {
                nvt_dbg(REORDER, "After PN check, New > Old\n");
                memcpy(pn_val_old, pn_val_rx, 6);
                ret_val = true;
        }
        return ret_val;
}

bool nvt_check_sec_rx_mpdu(struct _nvt_adapter *nvt_adapter,
        enum _cipher_t ct, struct sk_buff *skb, u8 *old_pn_val)
{
        bool error = false;

        nvt_dbg(REORDER, "%s, ct=%d\n", __func__, ct);

        /* Perform replay detection. */
        if ((ct == TKIP) || (ct == CCMP)) {
                nvt_dbg(REORDER, "skb buffer is ciphered by TKIP/CCMP\n");
                if (nvt_cmp_pn_val(old_pn_val, skb) != true) {
                        error = true;
                }
        } else {
                nvt_dbg(REORDER, "not ciphered by TKIP/CCMP, don't Check PN\n");
        }
        return error;
}

static void nvt_process_ba_rx_buff_frame(struct _ba_rx_struct_t *ba,
        struct _bfd_rx_pkt_t *pkt_ptr, u8 tid)
{
        struct _nvt_adapter *nvt_adapter = ba->ba_data.entry;
        struct _nvt_if *nvt_if = NULL;
        struct _nvt_hwinfo_rx *info = NULL;
        u8 ifidx = 0;

        nvt_dbg(REORDER, "%s\n", __func__);

        /* Get nvt_if pointer */
        nvt_if = nvt_get_if_by_index(nvt_adapter, ifidx);
        if (!nvt_if) {
                nvt_dbg(REORDER,
                        "ifidx = %d, nvt_if not match, Drop it\n", ifidx);
                dev_kfree_skb_any(pkt_ptr->pkt);
                return;
        }

        nvt_dbg(REORDER, "ifidx=%d, nvt_if=%p, nvt_adapter=%p",
                ifidx, nvt_if, nvt_adapter);

        /* PN Check */
        if (nvt_check_sec_rx_mpdu(nvt_adapter, pkt_ptr->ct,
                pkt_ptr->pkt, *(ba->rx_pn_val)) == true) {
                nvt_dbg(REORDER, "PN Check Fail, Drop it!!!!!!!!!\n");
                nvt_dbg(REORDER, "seq=%d,tid=%d\n", pkt_ptr->seq_num, tid);
                /* Drop it */
                dev_kfree_skb_any(pkt_ptr->pkt);
        } else {
                nvt_dbg(REORDER, "PN Check Success, Send it to kernel\n");
                nvt_dbg(REORDER, "seq=%d,tid=%d\n", pkt_ptr->seq_num, tid);

                /* Debugging Purpose */
                if ((nvt_seqno_gt(ba->debug_seq, pkt_ptr->seq_num) == true) ||
                    (ba->debug_seq == pkt_ptr->seq_num)) {
                        nvt_dbg(REORDER, "ERROR:tid=%d,prev=%d,current=%d\n",
                                tid, ba->debug_seq, pkt_ptr->seq_num);
                }
                if (((ba->debug_seq + 1) % 4096) != pkt_ptr->seq_num) {
                        nvt_dbg(REORDER, "Hole:tid=%d,prev=%d,current=%d\n",
                                tid, ba->debug_seq, pkt_ptr->seq_num);
                }
                ba->debug_seq = pkt_ptr->seq_num;

                info = (struct _nvt_hwinfo_rx *)pkt_ptr->pkt->data;
                /* check this skb is AMSDU or not, and send to kernel */
                if ((ntohl(info->word0) & 0x08000000) == 0) {
                        nvt_netif_rx(nvt_if, pkt_ptr->pkt);
                } else {
                        nvt_deaggr_amsdu(pkt_ptr->pkt, info, nvt_if);
                }
        }
}

static struct _bfd_rx_pkt_t *
nvt_remove_frame_from_reorder_q(struct _ba_rx_struct_t *ba,
        u16 seq_num)
{
        u16 idx = (seq_num & (g_num_ba_buffers - 1));
        struct _bfd_rx_pkt_t *buff = &(ba->buff_list[idx]);

        nvt_dbg(REORDER, "%s\n", __func__);
        nvt_dbg(REORDER, "seq_num=%d, buff->in_use=%d, buff->seq_num=%d\n",
                seq_num, buff->in_use, buff->seq_num);

        if ((buff->in_use == 0) || (buff->seq_num != seq_num)) {
                return NULL;
        }
        buff->in_use = 0;
        return buff;
}

static u16 nvt_send_frames_in_order(struct _ba_rx_struct_t *ba, u8 tid)
{
        struct _bfd_rx_pkt_t *pkt_ptr = NULL;
        u16 seq_num = ba->win_start;

        nvt_dbg(REORDER, "%s, win_start=%d\n", __func__, ba->win_start);

        while (1) {
                pkt_ptr = nvt_remove_frame_from_reorder_q(ba, seq_num);
                if (pkt_ptr != NULL) {
                        nvt_process_ba_rx_buff_frame(ba, pkt_ptr, tid);
                        ba->buff_cnt--;
                } else {
                        break;
                }
                seq_num = SEQNO_ADD(seq_num, 1);
        }
        return seq_num;
}

static u16 nvt_send_frames_with_gap(struct _ba_rx_struct_t *ba,
        u8 tid, u16 last_seqno)
{
        struct _bfd_rx_pkt_t *pkt_ptr  = NULL;
        u8 num_frms = 0;
        u16 seq_num = ba->win_start;

        nvt_dbg(REORDER, "%s\n", __func__);
        while (seq_num != last_seqno) {
                pkt_ptr = nvt_remove_frame_from_reorder_q(ba, seq_num);
                if (pkt_ptr != NULL) {
                        nvt_process_ba_rx_buff_frame(ba, pkt_ptr, tid);
                        num_frms++;
                        ba->buff_cnt--;
                } else {
                        /* Do nothing */
                }
                seq_num = SEQNO_ADD(seq_num, 1);
        }
        return num_frms;
}

static void nvt_flush_reorder_q(struct _ba_rx_struct_t *ba)
{
        u16 idx = 0;
        struct _bfd_rx_pkt_t *buff = NULL;

        nvt_dbg(REORDER, "%s\n", __func__);

        for (idx = 0; idx < g_num_ba_buffers; idx++) {
                buff = &(ba->buff_list[idx]);

                if (buff->in_use == 1) {
                        nvt_dbg(REORDER, "buff->seq_num=%d\n", buff->seq_num);
                        buff->in_use = 0;
                }
        }
}

static struct _bfd_rx_pkt_t *
nvt_buffer_frame_in_reorder_q(struct _ba_rx_struct_t *ba,
        u16 seq_num, bool *reuse)
{
        u16 idx = (seq_num & (g_num_ba_buffers - 1));
        struct _bfd_rx_pkt_t *buff = &(ba->buff_list[idx]);

        nvt_dbg(REORDER, "%s, idx=%d, seq_num=%d\n", __func__, idx, seq_num);

        *reuse = false;
        if (buff->in_use == 1) {
                nvt_dbg(REORDER, "buff->in_use == 1, check it later\n");
                nvt_dbg(REORDER, "buff->seq_num=%d, seq_num=%d\n",
                        buff->seq_num, seq_num);
                *reuse = true;
        }
        buff->in_use = 1;
        return buff;
}

static s32 nvt_buffer_ba_rx_frame(struct _ba_rx_struct_t *ba,
        struct sk_buff *pkt, u16 seq_num, enum _cipher_t ct, bool *reuse)
{
        struct _bfd_rx_pkt_t *pkt_ptr = NULL;

        nvt_dbg(REORDER, "%s\n", __func__);

        /* Get the pointer to the buffered packet */
        pkt_ptr = nvt_buffer_frame_in_reorder_q(ba, seq_num, reuse);

        if ((*reuse) == false) {
                /* Update the buffered receive packet details */
                pkt_ptr->seq_num   = seq_num;
                pkt_ptr->pkt       = pkt;
                pkt_ptr->ct        = ct;
                pkt_ptr->reorder_time = jiffies;
                nvt_dbg(REORDER, "jiffies_now=%lu, buff->reorder_time=%lu\n",
                        jiffies, pkt_ptr->reorder_time);
                ba->buff_cnt++;
        } else {
                if (seq_num == pkt_ptr->seq_num) {
                        return -1;
                }
        }
        return 0;
}

static void nvt_release_reorder_frame(struct _ba_rx_struct_t *ba,
                                        u16 idx, u8 tid)
{
        struct _bfd_rx_pkt_t *buff = &(ba->buff_list[idx]);

        nvt_dbg(REORDER, "%s, idx=%d, buff->seq_num=%d\n",
                                        __func__, idx, buff->seq_num);
        nvt_dbg(REORDER, "ba->win_start=%d, ba->win_end=%d, ba->buff_cnt=%d\n",
                        ba->win_start, ba->win_end, ba->buff_cnt);
        if (buff->in_use == 0) {
                nvt_dbg(REORDER, "Come here is werid, ERROR!!\n");
                return;
        }
        buff->in_use = 0;
        nvt_dbg(REORDER, "jiffies_now=%lu, buff->reorder_time=%lu\n",
                        jiffies, buff->reorder_time);
        nvt_process_ba_rx_buff_frame(ba, buff, tid);
        ba->buff_cnt--;
        ba->win_start = SEQNO_ADD(ba->win_start, 1);
        ba->win_end = SEQNO_ADD(ba->win_end, 1);
}

static bool nvt_rx_reorder_ready(struct _ba_rx_struct_t *ba, u16 idx)
{
        struct _bfd_rx_pkt_t *buff = &(ba->buff_list[idx]);

        if (buff->in_use == 1) {
                return true;
        } else {
                return false;
        }
}

static void nvt_reorder_release(struct _ba_rx_struct_t *ba,
                                                u8 tid, bool do_check)
{
        u32 index, j;
        u32 skipped = 0;
        struct _bfd_rx_pkt_t *buff = NULL;

        nvt_dbg(REORDER, "%s, ba->win_start=%d\n", __func__, ba->win_start);
        /* release the buffer until next missing frame */
        index = (ba->win_start & (g_num_ba_buffers - 1));
        if (!nvt_rx_reorder_ready(ba, (u16)index) && (ba->buff_cnt > 0) &&
                                                        (do_check == true)) {
                /* No buffers ready to be released, but check whether any
                * frames in the reorder buffer have timed out */
                skipped = 1;
                for (j = (index + 1) % ba->buff_size; j != index;
                        j = (j + 1) % ba->buff_size) {
                        if (!nvt_rx_reorder_ready(ba, (u16)j)) {
                                skipped++;
                                continue;
                        }
                        buff = &(ba->buff_list[(u16)j]);
                        if (skipped &&
                                !time_after(jiffies, buff->reorder_time +
                                RX_REORDER_BUF_TIMEOUT)) {
                                goto set_release_timer;
                        }
                        nvt_dbg(REORDER, "release an RX reorder frame due to timeout on earlier frames\n");
                        nvt_release_reorder_frame(ba, (u16)j, tid);
                        /* Increment the head seq also for the skipped slots */
                        ba->win_start = SEQNO_ADD(ba->win_start, skipped);
                        ba->win_end = SEQNO_ADD(ba->win_end, skipped);
                        skipped = 0;
                }
        }

        if (ba->buff_cnt > 0) {
                j = index = (ba->win_start & (g_num_ba_buffers - 1));

                for (; j != (index - 1) % ba->buff_size;
                        j = (j + 1) % ba->buff_size) {
                        if (nvt_rx_reorder_ready(ba, (u16)j)) {
                                break;
                        }
                }
set_release_timer:
                nvt_dbg(REORDER, "start reorder release timer\n");
                buff = &(ba->buff_list[(u16)j]);
                nvt_dbg(REORDER,
                        "idx=%d, buff->seq_num=%d, buff->reorder_time=%lu\n",
                        j, buff->seq_num, buff->reorder_time);
                mod_timer(&(ba->reorder_timer), buff->reorder_time + 1 +
                                RX_REORDER_BUF_TIMEOUT);
        } else {
                nvt_dbg(REORDER, "stop reorder release timer\n");
                if (timer_pending(&(ba->reorder_timer))) {
                        del_timer(&(ba->reorder_timer));
                }

        }

}

static void nvt_reorder_ba_rx_buffer_data(struct _ba_rx_struct_t *ba,
        struct sk_buff *pkt, u8 tid, u16 seq_num, enum _cipher_t ct, bool reuse)
{
        u8 seqno_pos = 0;
        u16 temp_winend;
        u16 temp_winstart;
        bool do_check = false;

        nvt_dbg(REORDER, "%s\n", __func__);
        nvt_dbg(REORDER, "win_start=%d, win_end=%d, seq_num=%d, reuse=%d\n",
                ba->win_start, ba->win_end, seq_num, reuse);

        if (ba->win_start == seq_num) {
                /* do nothing */
        } else {
                do_check = true;
        }
        seqno_pos = nvt_seqno_bound_chk(ba->win_start, ba->win_end, seq_num);

        if (BETWEEN_SEQLO_SEQHI == seqno_pos) {
                nvt_dbg(REORDER, "BETWEEN_SEQLO_SEQHI\n");

                ba->win_start = nvt_send_frames_in_order(ba, tid);
                ba->win_end = SEQNO_ADD(ba->win_start, (ba->buff_size - 1));
        } else if (GREATER_THAN_SEQHI == seqno_pos) {
                nvt_dbg(REORDER, "GREATER_THAN_SEQHI\n");

                temp_winend = seq_num;
                temp_winstart = SEQNO_SUB(temp_winend, (ba->buff_size - 1));
                nvt_send_frames_with_gap(ba, tid, temp_winstart);
                if (reuse == true) {
                        if (nvt_buffer_ba_rx_frame(ba, pkt,
                                seq_num, ct, &reuse) < 0) {
                                dev_kfree_skb_any(pkt);
                                nvt_dbg(REORDER, "Shouldn't be here, ERROR!\n");
                        }
                }
                ba->win_start = temp_winstart;
                ba->win_start = nvt_send_frames_in_order(ba, tid);
                ba->win_end = SEQNO_ADD(ba->win_start, (ba->buff_size - 1));
        } else {
                nvt_dbg(REORDER, "Come here is illegal, ERROR!!!!!!!!!\n");
                /* Do Nothing */
        }

        /* handle reorder timer release */
        nvt_reorder_release(ba, tid, do_check);
}

static void nvt_free_ba_handle(void *ba_handle)
{
        if (ba_handle != NULL) {
                nvt_dbg(REORDER, "%s, (*ba) != NULL, free them\n", __func__);
                /* buff_list element is freed */
                kfree(((struct _ba_rx_struct_t *)ba_handle)->buff_list);
                /* Block ACK handle is freed */
                kfree(ba_handle);
                return;
        }
        nvt_dbg(REORDER, "nvt_free_ba_handle, ba == NULL, weird!!!!!\n");
}

static void nvt_ba_handle(struct _ba_rx_struct_t *ba, int cmd,
        struct _bfd_rx_pkt_t *pkt_cur, struct _nvt_if *nvt_if)
{
        struct _nvt_hwinfo_rx *info = NULL;
        bool reuse = false;
        u8 tid = 0;
        u16 temp_seqno = 0;

        nvt_dbg(REORDER, "%s\n", __func__);
        if (ba == NULL) {
                nvt_dbg(REORDER, "%s,ba==NULL, check it out!!\n", __func__);
                /* Exception */
                return;
        }
        nvt_dbg(REORDER, "ba=%p\n", ba);
        nvt_dbg(REORDER, "ba->win_start=%d, ba->win_end=%d, cmd=%d\n",
                ba->win_start, ba->win_end, cmd);

        tid = ba->ba_data.tid;
        nvt_dbg(REORDER, "tid=%d\n", tid);
        if (cmd == 1) {
                temp_seqno = SEQNO_ADD(ba->win_tail, 1);
                nvt_send_frames_with_gap(ba, tid, temp_seqno);
                ba->win_start = temp_seqno;
                ba->win_end =
                        SEQNO_ADD(ba->win_start, (ba->buff_size - 1));
        } else if (cmd == 2) {
                ba->win_start = SEQNO_ADD(ba->win_start, 1);
                ba->win_end = SEQNO_ADD(ba->win_end, 1);

                if (nvt_check_sec_rx_mpdu(nvt_if->nvt_adapter, pkt_cur->ct,
                        pkt_cur->pkt, *(ba->rx_pn_val)) == true) {
                        //pkt_cur->pkt, pkt_cur->rx_pn_val) == true) {
                        nvt_dbg(REORDER, "PN Check Fail, Drop it!!!!!!!!!\n");
                        nvt_dbg(REORDER,
                                "seq=%d,tid=%d\n", pkt_cur->seq_num, tid);
                        dev_kfree_skb_any(pkt_cur->pkt);
                } else {
                        nvt_dbg(REORDER, "PN Check Success, Send to kernel\n");
                        nvt_dbg(REORDER,
                                "seq=%d,tid=%d\n", pkt_cur->seq_num, tid);
                        /* Debugging Purpose */
                        if ((nvt_seqno_gt(ba->debug_seq,
                                pkt_cur->seq_num) == true) ||
                            (ba->debug_seq == pkt_cur->seq_num)) {
                                nvt_dbg(REORDER,
                                        "ERROR:tid=%d,prev=%d,current=%d\n",
                                        tid, ba->debug_seq,
                                        pkt_cur->seq_num);
                        }
                        if (((ba->debug_seq + 1) % 4096) !=
                                pkt_cur->seq_num) {
                                nvt_dbg(REORDER,
                                        "Hole:tid=%d,prev=%d,current=%d\n",
                                        tid, ba->debug_seq,
                                        pkt_cur->seq_num);
                        }
                        ba->debug_seq = pkt_cur->seq_num;

                        info = (struct _nvt_hwinfo_rx *)pkt_cur->pkt->data;
                        /* check this skb is AMSDU or not, send to kernel */
                        if ((ntohl(info->word0) & 0x08000000) == 0) {
                                nvt_netif_rx(nvt_if, pkt_cur->pkt);
                        } else {
                                nvt_deaggr_amsdu(pkt_cur->pkt, info, nvt_if);
                        }
                }
        } else if (cmd == 3) {
                /* Buffer the new MSDU */
                if (nvt_buffer_ba_rx_frame(ba, pkt_cur->pkt, pkt_cur->seq_num,
                        pkt_cur->ct, &reuse) < 0) {
                        dev_kfree_skb_any(pkt_cur->pkt);
                        nvt_dbg(REORDER, "Receive duplicate frame drop it!!\n");
                        return;
                }

                /* Pass the reordered MSDUs up the stack */
                nvt_reorder_ba_rx_buffer_data(ba, pkt_cur->pkt, tid,
                        pkt_cur->seq_num, pkt_cur->ct, reuse);
        }
        return;
}

static void nvt_reset_rx_ba_handle(struct _ba_struct_t *ba_ctxt)
{
        struct _ba_rx_struct_t *ba = NULL;
        ulong flags;
        //u8 idx;
        //AT:comment out unused variables
        //struct _nvt_adapter *nvt_adapter = ba_ctxt->entry;
        //u8 *pn_start_ptr;
        //u8 *bcmc_pn_ptr;
        nvt_dbg(REORDER, "%s\n", __func__);

        spin_lock_irqsave(&ba_ctxt->ba_lock, flags);

        ba = ba_ctxt->ba_rx_ptr;
        if (ba == NULL) {
                nvt_dbg(REORDER, "%s,ba==NULL, check it!!\n", __func__);
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                return;
        }
        del_timer(&(ba->timer));
        del_timer(&(ba->reorder_timer));

        nvt_dbg(REORDER, "Before reset,ba->buff_cnt=%d, ba->win_tail=%d\n",
                ba->buff_cnt, ba->win_tail);
        if (ba->buff_cnt > 0) {
                nvt_ba_handle(ba, 1, NULL, NULL);
        }
        nvt_dbg(REORDER, "After reset,ba->buff_cnt=%d\n", ba->buff_cnt);

        /* Flush out the BA-Rx Queue */
        nvt_flush_reorder_q(ba);

        nvt_free_ba_handle(ba);
        ba_ctxt->ba_rx_ptr = NULL;
        //AT:should not free PN in reset ba handle
        //idx = (ba_ctxt->sta_index - 1) * NUM_TIDS + ba_ctxt->tid;
        //pn_start_ptr = nvt_adapter->nvt_priv.pn_unicast[idx];
        //bcmc_pn_ptr = nvt_adapter->nvt_priv.pn_bcmc;
        //if (pn_start_ptr != NULL) {
        //        kfree(pn_start_ptr);
        //        nvt_adapter->nvt_priv.pn_unicast[idx] = 0;
        //}
        //if (bcmc_pn_ptr != NULL) {
        //        kfree(bcmc_pn_ptr);
        //        nvt_adapter->nvt_priv.pn_bcmc = 0;
        //}

        spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
}

/**
 * nvt_reset_ba_handle - clean ba handles for one STA or one AP
 * @nvt_adapter: adapter structrue
 * @ba: ba(block ack) handles
 *
 * this function will reset the 11e BA handles
 *
 * Return: NULL
 */
void nvt_reset_ba_handle(struct _nvt_adapter *nvt_adapter, u8 sta_index)
{
        struct _ba_struct_t *ba_ctxt = NULL;
        struct _nvt_priv nvt_priv = nvt_adapter->nvt_priv;
        u8 idx;
        u8 i = 0;
        nvt_dbg(REORDER, "%s\n", __func__);
        nvt_dbg(REORDER,
                "nvt_adapter=%p, sta_index=%d\n", nvt_adapter, sta_index);

        for (i = 0; i < NUM_TIDS; i++) {
                idx = (sta_index - 1) * NUM_TIDS + i;
                ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
                nvt_reset_rx_ba_handle(ba_ctxt);
                nvt_priv.last_seq_ctl[idx] = 0xFFFF;
        }
        nvt_priv.last_seq_ctl[ASSOC_NUM * NUM_TIDS + sta_index - 1] = 0xFFFF;
}

void nvt_reset_unicast_pn(struct _nvt_adapter *nvt_adapter,
                u8 sta_index)
{
        u8 idx;
        u8 i = 0;
        u8 *pn_start_ptr;
        ulong flags;
        struct _ba_struct_t *ba_ctxt = NULL;
        struct _ba_rx_struct_t *ba;
        for (i = 0; i < NUM_TIDS; i++) {
                idx = (sta_index - 1) * NUM_TIDS + i;
                ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
                spin_lock_irqsave(&ba_ctxt->ba_lock, flags);
                /*check ba ring buffer*/
                ba = ba_ctxt->ba_rx_ptr;
                if (ba != NULL && ba->buff_cnt > 0) {
                        nvt_ba_handle(ba, 1, NULL, NULL);
                }
                pn_start_ptr = nvt_adapter->nvt_priv.pn_unicast[idx];
                if (pn_start_ptr != NULL) {
                        kfree(pn_start_ptr);
                        nvt_adapter->nvt_priv.pn_unicast[idx] = 0;
                }
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
        }
}

static void nvt_ba_timeout_fn(ulong arg)
{
        struct _ba_struct_t *ba_ctxt = (struct _ba_struct_t *)arg;
        struct _ba_rx_struct_t *ba = NULL;
        ulong flags;

        nvt_dbg(REORDER, "%s, sta_index=%d, tid=%d\n",
                __func__, ba_ctxt->sta_index, ba_ctxt->tid);
        nvt_dbg(REORDER, "is_in_interrupt=%lu\n", in_interrupt());
        if (spin_is_locked(&ba_ctxt->ba_lock)) {
                return;
        }
        spin_lock_irqsave(&ba_ctxt->ba_lock, flags);

        ba = ba_ctxt->ba_rx_ptr;
        /* Check whether the Block-Ack session is still valid */
        if (ba == NULL) {
                nvt_dbg(REORDER, "ba == NULL, return\n");
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                return;
        }
        nvt_dbg(REORDER, "ba->buff_cnt=%d, ba->win_tail=%d\n",
                ba->buff_cnt, ba->win_tail);

        if (ba->buff_cnt > 0) {
                nvt_ba_handle(ba, 1, NULL, NULL);
        }
        nvt_dbg(REORDER, "ba->buff_cnt=%d\n", ba->buff_cnt);

        if (ba->timeout != 0) {
                nvt_dbg(REORDER,
                        "Timeout!=0, call workqueue to send Delba/reset ba\n");
                schedule_work(&ba_ctxt->ba_timeout_work);
        }
        spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
}

static void nvt_ba_timeout_worker(struct work_struct *work)
{
        struct _ba_struct_t *ba_ctxt = container_of(work,
                struct _ba_struct_t, ba_timeout_work);
        struct _nvt_adapter *nvt_adapter = NULL;
        s32 ret = 0;
        u16 index = 0;
        u8 buf[64] = {0};

        nvt_dbg(REORDER, "%s, ba_ctxt=%p\n", __func__, ba_ctxt);
        nvt_dbg(REORDER, "is_in_interrupt=%lu\n", in_interrupt());

        nvt_adapter = ba_ctxt->entry;
        nvt_dbg(REORDER, "nvt_adapter=%p, sta_index=%d, tid=%d\n",
                nvt_adapter, ba_ctxt->sta_index, ba_ctxt->tid);

        /* Prepare DELBA Command to FW & Reset BA-Session */
        index = 1;
        /* Block Ack Category */
        buf[index++] = 3;
        /* MLME_DELBA_REQ_TYPE */
        buf[index++] = 2;
        memcpy(&buf[index], ba_ctxt->dst_addr, ETH_ALEN);
        index += ETH_ALEN;
        buf[index++] = ba_ctxt->tid;
        /* Recipient */
        buf[index++] = 0;
        /* QSTA_TIMEOUT */
        buf[index++] = 39;
        buf[0] = index;

        nvt_reset_rx_ba_handle(ba_ctxt);

        /* Send DELBA Command */
        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_11N_P_ACTION_REQ, (u8 *)&buf, index);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                nvt_dbg(REORDER, "WID_SET(DELBA) has problems\n");
        } else {
        }
}

static void nvt_reorder_timeout_fn(ulong arg)
{
        struct _ba_struct_t *ba_ctxt = (struct _ba_struct_t *)arg;
        struct _ba_rx_struct_t *ba = NULL;
        u8 tid = 0;
        ulong flags;

        nvt_dbg(REORDER, "%s, sta_index=%d, tid=%d\n",
                __func__, ba_ctxt->sta_index, ba_ctxt->tid);
        nvt_dbg(REORDER, "is_in_interrupt=%lu\n", in_interrupt());
        if (spin_is_locked(&ba_ctxt->ba_lock)) {
                return;
        }
        spin_lock_irqsave(&ba_ctxt->ba_lock, flags);

        ba = ba_ctxt->ba_rx_ptr;
        /* Check whether the Block-Ack session is still valid */
        if (ba == NULL) {
                nvt_dbg(REORDER, "ba == NULL, return\n");
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                return;
        }
        nvt_dbg(REORDER, "before win: start=%d, end=%d, buff_cnt=%d, tail=%d\n",
                        ba->win_start, ba->win_end, ba->buff_cnt, ba->win_tail);

        if (ba->buff_cnt > 0) {
                tid = ba->ba_data.tid;
                nvt_reorder_release(ba, tid, true);
        }

        nvt_dbg(REORDER, "after win: start=%d, end=%d, buff_cnt=%d, tail=%d\n",
                        ba->win_start, ba->win_end, ba->buff_cnt, ba->win_tail);
        spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
}

/*
 * This function reorders the Reciver buffer and sends frames to the higher
 * layer on reception of a Block-Ack-Request frame. It also updates the
 * receiver buffer window.
*/
static void nvt_reorder_ba_rx_buffer_bar(struct _ba_rx_struct_t *ba,
        u8 tid, u16 start_seq_num)
{
        nvt_dbg(REORDER, "%s\n", __func__);
        nvt_dbg(REORDER, "start_seq_num=%d, tid=%d, ba->win_start=%d\n",
                start_seq_num, tid, ba->win_start);

        if (true == nvt_seqno_gt(start_seq_num, ba->win_start)) {
                nvt_send_frames_with_gap(ba, tid, start_seq_num);
                ba->win_start = start_seq_num;
                ba->win_start = nvt_send_frames_in_order(ba, tid);
                ba->win_end = SEQNO_ADD(ba->win_start, (ba->buff_size - 1));
        }
}

s32 get_ba_index_by_hosthdr(struct _nvt_hwinfo_rx *info,
        struct sk_buff *pkt)
{

        u8 idx;
        u8 sta_index = info->word4.sta_idx;
        u8 tid = (u8)(ntohl(info->word0) & 0x000000F0) >> 4;
        idx = (sta_index - 1) * NUM_TIDS + (tid);
        if ((sta_index == 0) || (sta_index > 8)) {
                nvt_dbg(REORDER, "Error Sta_index not available, drop\n");
                nvt_dbg(REORDER, "info->word0 = 0x%08x\n", ntohl(info->word0));
                dev_kfree_skb_any(pkt);
                return -1;
        }
        if (tid > 7) {
                nvt_dbg(REORDER, "Error tid not available, drop\n");
                nvt_dbg(REORDER, "info->word0 = 0x%08x\n", ntohl(info->word0));
                dev_kfree_skb_any(pkt);
                return -1;
        }
        nvt_dbg(REORDER, "sta_index=%d, tid=%d\n", sta_index, tid);
        return idx;
}

/**
 * nvt_ba_process - this function checks whether the received frame
 *                  should be filtered based on the Ack-Policy.
 * @nvt_if    : interface structrue
 * @info      : rx host header structure
 * @pkt       : rx skb buffer
 * @ba_index  : this is an array idx used to access the ba_ctxt
 * If the rxd frame is under a block-ack session, and is out-of-order,
 * then it is buffered. Otherwise, it is allowed to be passed to the kernel.
 *
 * Return: true - under ba session, need to reorder.
 *         false - pass to the kernel directly.
 */
bool nvt_ba_process(struct _nvt_if *nvt_if,
        struct _nvt_hwinfo_rx *info, struct sk_buff *pkt, u8 idx)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _ba_struct_t *ba_ctxt = NULL;
        struct _ba_rx_struct_t *ba = NULL;
        enum _cipher_t ct;
        struct _bfd_rx_pkt_t pkt_cur;
        ulong flags;
        u8 tid;
        u8 sta_index;
        //u8 idx;
        u8 *pn_start_ptr;
        u16 seq_num;
        u8 is_BC;
        u8 is_MC;
        u8 *sa = NULL;
        u32 tmp;
        u32 i = 0;

        /* Get the station ID entry & TID & Seq. & cipher, sanity check */
        sta_index = info->word4.sta_idx;
        tid = (u8)(ntohl(info->word0) & 0x000000F0) >> 4;
        //idx = (sta_index - 1) * NUM_TIDS + tid;
        seq_num = (u16)(ntohl(info->word7.value) & 0x00000fff);
        /*if ((sta_index == 0) || (sta_index > 8)) {
                nvt_dbg(REORDER, "Error Sta_index not available, drop\n");
                nvt_dbg(REORDER, "info->word0 = 0x%08x\n", ntohl(info->word0));
                dev_kfree_skb_any(pkt);
                return true;
        }
        if (tid > 7) {
                nvt_dbg(REORDER, "Error tid not available, drop\n");
                nvt_dbg(REORDER, "info->word0 = 0x%08x\n", ntohl(info->word0));
                dev_kfree_skb_any(pkt);
                return true;
        }*/

        nvt_dbg(REORDER, "%s\n", __func__);
        nvt_dbg(REORDER, "is_in_interrupt=%lu\n", in_interrupt());
        nvt_dbg(REORDER, "nvt_if=%p, nvt_adapter=%p\n", nvt_if, nvt_adapter);
        //nvt_dbg(REORDER, "sta_index=%d, seq_num=%d, tid=%d\n",
        //        sta_index, seq_num, tid);
        is_BC = (ntohl(info->word0) & 0x10000000) >> 28;
        is_MC = (ntohl(info->word0) & 0x20000000) >> 29;
        ct = (enum _cipher_t)((ntohl(info->word7.value) & 0x0000f000) >> 12);
        update_piggyback_fifocredit(nvt_if, info);

        sa = (u8 *)(pkt->data);
        tmp = info->word1.offset;
        nvt_dbg(REORDER, "SA address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                sa[tmp + 6], sa[tmp + 7], sa[tmp + 8], sa[tmp + 9],
                sa[tmp + 10], sa[tmp + 11]);
        nvt_dbg(REORDER, "info->word0 = 0x%08x\n", ntohl(info->word0));

        if ((sa[tmp + 12] == 0x88) && (sa[tmp + 13] == 0x8e)) {
                nvt_dbg(REORDER, "This is Rx EAPOL\n");
                for (i = 0; i < pkt->len; i++) {
                        //printk("%02x:", sa[tmp + i]);
                }
                nvt_dbg(REORDER, "\n");
        }

        ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
        spin_lock_irqsave(&ba_ctxt->ba_lock, flags);

        ba = ba_ctxt->ba_rx_ptr;
        pn_start_ptr = nvt_adapter->nvt_priv.pn_unicast[idx];
        nvt_dbg(REORDER, "ba_ctxt=%p, ba=%p\n", ba_ctxt, ba);

        /* Check if block ACK is enabled for current TID */
        if (ba == NULL) {
                nvt_dbg(REORDER, "Block ACK is NOT enabled ba == NULL\n");
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                return false;
        }

        pn_start_ptr = nvt_adapter->nvt_priv.pn_unicast[idx];
        if (pn_start_ptr == NULL) {
                pn_start_ptr = kzalloc(6, GFP_ATOMIC);
                nvt_adapter->nvt_priv.pn_unicast[idx] =
                         pn_start_ptr;
        }
        nvt_dbg(REORDER, "ba_ctxt=%p, ba=%p\n", ba_ctxt, ba);


        nvt_dbg(REORDER, "timeout=%d\n", ba->timeout);
        nvt_dbg(REORDER, "seq_num=%d, win_start=%d, win_tail=%d\n",
                seq_num, ba->win_start, ba->win_tail);
        nvt_dbg(REORDER, "buff_cnt=%d\n", ba->buff_cnt);
        nvt_dbg(REORDER, "Is_BC=%d, Is_MC=%d, cipher=%d\n", is_BC, is_MC, ct);

        /*
         * Only consider for Uni-cast Frame based on Address one
         * is_BC & is_MC is based on Address three
        */
        if (((is_BC == 1) || (is_MC == 1)) && (nvt_if->mode == NVT_FW_STA)) {
                nvt_dbg(REORDER, "Not Unicast frame(According Address one)\n");
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                return false;
        }

        if (true == nvt_seqno_geq(seq_num, ba->win_start)) {

                /*
                 * The BA-Timer is restarted each time a frame under the
                 * BA-session is successfully received.
                */
                if (ba->timeout > 0) {
                        nvt_dbg(REORDER,
                                "start ba timer = %d ms\n", ba->timeout);
                        mod_timer(&(ba->timer),
                                (jiffies + ((ba->timeout) * HZ) / 1000));
                }

                if (nvt_seqno_geq(seq_num, ba->win_tail) == true) {
                        ba->win_tail = seq_num;
                }

                /*
                 * If the sequence number of the frame is same as Win-Start,
                 * and the reorder queue is empty, then the new packet need
                 * not be buffered in the reordering queue.
                 * For this type of frames just update the window parameters
                 * and pass it up to the host.
                */
                if ((seq_num == ba->win_start) && (ba->buff_cnt == 0)) {
                        pkt_cur.ct = ct;
                        pkt_cur.pkt = (struct sk_buff *)pkt;
                        pkt_cur.seq_num = seq_num;
                        nvt_ba_handle(ba, 2, &pkt_cur, nvt_if);
                } else {
                        pkt_cur.pkt = pkt;
                        pkt_cur.ct = ct;
                        pkt_cur.seq_num = seq_num;
                        nvt_ba_handle(ba, 3, &pkt_cur, nvt_if);
/*
                        if (ba->timeout == 0) {
                                if (ba->buff_cnt > 0) {
                                        nvt_dbg(REORDER, "start timer 1sec\n");
                                        mod_timer(&(ba->timer),
                                                (jiffies +
                                                (BA_RECOVERY_TIMEOUT * HZ) /
                                                1000));
                                } else {
                                        nvt_dbg(REORDER, "stop timer\n");
                                        if (timer_pending(&(ba->timer))) {
                                                del_timer(&(ba->timer));
                                        }
                                }
                        }
*/
                }
        } else {
                dev_kfree_skb_any(pkt);
                nvt_dbg(REORDER,
                        "seq_num is not within valid space, Discard it!!!\n");
        }

        /* Check Sync loss and flush the reorder queue when one is detected */
        if ((ba->win_tail == SEQNO_SUB(ba->win_start, 1)) &&
            (ba->buff_cnt > 0)) {
                nvt_dbg(REORDER, "Sync loss happenned, check it out, ERROR!\n");
                nvt_dbg(REORDER, "win_start=%d, buff_cnt=%d\n",
                        ba->win_start, ba->buff_cnt);
                nvt_flush_reorder_q(ba);
                ba->buff_cnt = 0;
        }
        spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
        return true;
}

static s32 nvt_handle_addba_req_evt(struct _nvt_if *nvt_if,
        const u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _ba_struct_t *ba_ctxt = NULL;
        struct _ba_rx_struct_t *ba = NULL;
        u8 *pn_start_ptr;
        gfp_t alloc_flag = GFP_ATOMIC;
        ulong flags;
        u16 buf_size;
        u8 tid;
        u8 sta_index;
        u8 idx;
        u8 err = 0;

        nvt_dbg(REORDER, "%s\n", __func__);
        nvt_dbg(REORDER, "is_in_interrupt=%lu\n", in_interrupt());

        /* Get the station ID entry & TID & buff_size */
        sta_index = data[2];
        tid = data[3];
        idx = (sta_index - 1) * NUM_TIDS + tid;
        nvt_dbg(REORDER, "nvt_if=%p, nvt_adapter=%p\n", nvt_if, nvt_adapter);
        nvt_dbg(REORDER, "sta_index=%d, tid=%d\n", sta_index, tid);
        if ((sta_index == 0) || (sta_index > 8)) {
                nvt_dbg(REORDER, "Error Sta_index not available, drop\n");
                return -EINVAL;
        }
        if (tid > 7) {
                nvt_dbg(REORDER, "Error tid not available, drop\n");
                return -EINVAL;
        }
        ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
        spin_lock_irqsave(&ba_ctxt->ba_lock, flags);

        ba = ba_ctxt->ba_rx_ptr;
        pn_start_ptr = nvt_adapter->nvt_priv.pn_unicast[idx];
        if (ba == NULL) {
                nvt_dbg(REORDER, "ba == NULL, new BA is build\n");
                ba = kzalloc(sizeof(struct _ba_rx_struct_t), alloc_flag);
                /*Allocate 6 byte PN for WPA*/
                if (ba == NULL) {
                        nvt_dbg(REORDER, "_ba_rx_struct_t allocate fail\n");
                        err = 1;
                        goto fail;
                }
                ba_ctxt->ba_rx_ptr = ba;
                /*Allocate 6 byte PN for WPA*/
                if (pn_start_ptr == NULL) {
                        pn_start_ptr = kzalloc(6, alloc_flag);
                }

                if (pn_start_ptr == NULL) {
                        nvt_dbg(ERROR, "PN allocate fail\n");
                        err = 2;
                        goto fail;
                }
                ba->rx_pn_val = &(nvt_adapter->nvt_priv.pn_unicast[idx]);
                nvt_adapter->nvt_priv.pn_unicast[idx] = pn_start_ptr;
                buf_size = data[14] | (data[15] << 8);
                if ((buf_size > MAX_NUM_BA_BUFFERS) || (buf_size == 0)) {
                        nvt_dbg(REORDER, "buff_size is out of range, fail\n");
                        err = 2;
                        goto fail;
                }
                ba->buff_size = data[14] | (data[15] << 8);
                g_num_ba_buffers = ba->buff_size;
                ba->buff_list = kzalloc(BA_QUEUE_BUFF_SIZE, alloc_flag);
                if ((ba->buff_list) == NULL) {
                        nvt_dbg(REORDER, "_bfd_rx_pkt_t allocate fail\n");
                        err = 2;
                        goto fail;
                }
        } else {
                nvt_dbg(REORDER, "The TID already build, tid=%d\n", tid);
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                /*
                 * if same TID BA is re-build by peer device,
                 * we will clean old and build new one.
                */
                nvt_reset_rx_ba_handle(ba_ctxt);
                spin_lock_irqsave(&ba_ctxt->ba_lock, flags);

                ba = kzalloc(sizeof(struct _ba_rx_struct_t), alloc_flag);
                if (ba == NULL) {
                        nvt_dbg(REORDER, "_ba_rx_struct_t allocate fail\n");
                        err = 1;
                        goto fail;
                }
                ba_ctxt->ba_rx_ptr = ba;
                if (pn_start_ptr == NULL) {
                        nvt_dbg(ERROR, "PN should not be NULL\n");
                        pn_start_ptr = kzalloc(6, alloc_flag);
                        nvt_adapter->nvt_priv.pn_unicast[idx] = pn_start_ptr;
                }
                if (pn_start_ptr == NULL) {
                        nvt_dbg(ERROR, "PN allocate fail\n");
                        err = 1;
                        goto fail;

                }
                buf_size = data[14] | (data[15] << 8);
                if ((buf_size > MAX_NUM_BA_BUFFERS) || (buf_size == 0)) {
                        nvt_dbg(REORDER, "buff_size is out of range, fail\n");
                        err = 2;
                        goto fail;
                }
                ba->buff_size = data[14] | (data[15] << 8);
                g_num_ba_buffers = ba->buff_size;
                ba->buff_list = kzalloc(BA_QUEUE_BUFF_SIZE, alloc_flag);
                if ((ba->buff_list) == NULL) {
                        nvt_dbg(REORDER, "_bfd_rx_pkt_t allocate fail\n");
                        err = 2;
                        goto fail;
                }
                ba->rx_pn_val = &(nvt_adapter->nvt_priv.pn_unicast[idx]);
        }

        ba->sta_index = sta_index;
        memcpy(ba_ctxt->dst_addr, &data[4], ETH_ALEN);
        ba->ba_data.tid = tid;
        ba->ba_data.entry = nvt_adapter;
        ba_ctxt->entry = nvt_adapter;
        /* in TUs = 1024us = 1.024ms */
        ba->timeout = data[10] | (data[11] << 8);
        ba->win_start = data[12] | (data[13] << 8);
        ba->win_end = SEQNO_ADD(ba->win_start, (ba->buff_size - 1));
        ba->win_tail = SEQNO_SUB(ba->win_start, 1);

        nvt_dbg(REORDER, "ba->timeout=%d, ba->win_start=%d\n",
                ba->timeout, ba->win_start);
        nvt_dbg(REORDER, "ba->win_end=%d, ba->win_tail=%d\n",
                ba->win_end, ba->win_tail);

        /* Initial Session Timer */
        init_timer(&(ba->timer));
        ba->timer.function = nvt_ba_timeout_fn;
        ba->timer.data = (unsigned long)ba_ctxt;

        /* Initial Reorder Timer */
        init_timer(&(ba->reorder_timer));
        ba->reorder_timer.function = nvt_reorder_timeout_fn;
        ba->reorder_timer.data = (unsigned long)ba_ctxt;

        nvt_dbg(REORDER, "tid=%d, ba_ctxt=%p, ba=%p\n", tid, ba_ctxt, ba);
        spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);

        return 0;
fail:
        switch (err) {
        case 2:
                kfree(ba_ctxt->ba_rx_ptr);
                ba_ctxt->ba_rx_ptr = NULL;
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                break;
        default:
                /* Do nothing */
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                break;
        }
        return -ENOMEM;
}

static s32 nvt_handle_delba_evt(struct _nvt_if *nvt_if,
        const u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _ba_struct_t *ba_ctxt = NULL;
        u8 tid = 0;
        u8 sta_index = 0;
        u8 idx = 0;

        nvt_dbg(REORDER, "%s\n", __func__);

        /* Get the station ID entry & TID */
        sta_index = data[2];
        tid = data[3];
        idx = (sta_index - 1) * NUM_TIDS + tid;

        nvt_dbg(REORDER, "nvt_if=%p, nvt_adapter=%p\n", nvt_if, nvt_adapter);
        nvt_dbg(REORDER, "sta_index=%d, tid=%d\n", sta_index, tid);
        if ((sta_index == 0) || (sta_index > 8)) {
                nvt_dbg(REORDER, "Error Sta_index not available, drop\n");
                return -EINVAL;
        }
        if (tid > 7) {
                nvt_dbg(REORDER, "Error tid not available, drop\n");
                return -EINVAL;
        }

        ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);

        nvt_reset_rx_ba_handle(ba_ctxt);

        return 0;
}

static s32 nvt_handle_bar_evt(struct _nvt_if *nvt_if,
        const u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _ba_struct_t *ba_ctxt = NULL;
        struct _ba_rx_struct_t *ba = NULL;
        u16 seq_num;
        ulong flags;
        u8 tid;
        u8 sta_index;
        u8 idx;

        nvt_dbg(REORDER, "%s\n", __func__);

        /* Get the station ID entry & TID & SSN */
        sta_index = data[2];
        tid = data[3];
        idx = (sta_index - 1) * NUM_TIDS + tid;
        seq_num = data[4] | (data[5] << 8);
        nvt_dbg(REORDER, "nvt_if=%p, nvt_adapter=%p\n", nvt_if, nvt_adapter);
        nvt_dbg(REORDER, "sta_index=%d, tid=%d, seq_num=%d\n",
                sta_index, tid, seq_num);
        if ((sta_index == 0) || (sta_index > 8)) {
                nvt_dbg(REORDER, "Error Sta_index not available, drop\n");
                return -EINVAL;
        }
        if (tid > 7) {
                nvt_dbg(REORDER, "Error tid not available, drop\n");
                return -EINVAL;
        }
        ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
        spin_lock_irqsave(&ba_ctxt->ba_lock, flags);

        ba = ba_ctxt->ba_rx_ptr;
        if (ba == NULL) {
                nvt_dbg(REORDER, "ba is NULL, return\n");
                spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);
                return 0;
        }

        /* Reorder the BA-Rx Buffer */
        nvt_reorder_ba_rx_buffer_bar(ba, tid, seq_num);
        spin_unlock_irqrestore(&ba_ctxt->ba_lock, flags);

        return 0;

}

#ifdef CONFIG_WIFI_TUNING_PHASE_II
static s32 nvt_notify_tp_monitor(struct _nvt_if *nvt_if,
        const u16 msg_len, u8 *data)
{
       struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
       struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;

       struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;

       u8 i, j, zero_mac[ETH_ALEN] = {0}, found = 0, phyrate_found;
       u8 win_sz = 8;    /* Note: Max. Sliding window size is 8 */
       u8 *monitor_info_p = (((u8 *)data) + 2);
       u8 *p;
       u16 monitor_info_p_len = (msg_len-2);       /* discard of WID */

       /*       // debug
       nvt_dbg(CLEARVIEW, "%s: monitor_info_p_len:%d\n",
               __func__, monitor_info_p_len);

       for (i=0; i< monitor_info_p_len; i++) {
           nvt_dbg(CLEARVIEW, " %x ", *(monitor_info_p+i));
           //if(i%16==0)   nvt_dbg(CLEARVIEW, "\n");
       }
       nvt_dbg(CLEARVIEW,"monitor_info_p:%p\n", monitor_info_p);

       nvt_dbg(CLEARVIEW, "MAC_Sts_mac_off:%d\n", MAC_Sts_mac_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_phyr_off:%d\n", MAC_Sts_phyr_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_votxsz_off:%d\n", MAC_Sts_votxsz_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_betxsz_off:%d\n", MAC_Sts_betxsz_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_votxfrm_off:%d\n", MAC_Sts_votxfrm_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_betxfrm_off:%d\n", MAC_Sts_betxfrm_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_vodrpsz_off:%d\n", MAC_Sts_vodrpsz_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_bedrpsz_off:%d\n", MAC_Sts_bedrpsz_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_vodrpfrm_off:%d\n", MAC_Sts_vodrpfrm_off);
       nvt_dbg(CLEARVIEW, "MAC_Sts_bedrpfrm_off:%d\n", MAC_Sts_bedrpfrm_off);

       //*/
       u16 wid = *((u8 *)data) | (*(((u8 *)data) + 1) << 8);

       /* driver layer */
       u32 outq_num_vo, outq_num_be, drop_num_be;
       u32 outq_sz_vo, outq_sz_be;

       /* MAC layer */
       u16 mac_vodropframe, mac_bedropframe;
       u32 phyrate = 0;
       u32 mac_votxsz, mac_betxsz, mac_votxframe, mac_betxframe;
       u32 mac_vodropsz, mac_bedropsz;
       struct _sta_sts_in_ap *sta_sts_in_ap = NULL;
       ulong flags = 0;

       //WMM_EDCA_Q_T *hQue_be = (WMM_EDCA_Q_T *)pDrv->edca_q[EDCA_Q_BE];
       //WMM_EDCA_Q_T *hQue_vo = (WMM_EDCA_Q_T *)pDrv->edca_q[EDCA_Q_VO];

       if (wid != WID_EVENT_TP_MONITOR) {
           // it will be freed by the caller
           nvt_dbg(ERROR, "%s: Wrong WID: 0x%x isn't 0x000A ", __func__, wid);
       } else {
           if (monitor_info_p_len >= MAC_Status_Len) {
               // Clear once MAC report becuase we don't want to handle
               // the case of overflow in outq_num and outq_sz.

               // Driver VO statistic
               outq_num_vo = tx_info->stats[NV_TX_FIFO_AC_VO].out;
               outq_sz_vo = tx_info->stats[NV_TX_FIFO_AC_VO].out_sz;
               spin_lock_irqsave(&tx_info->lock, flags);
               tx_info->stats[NV_TX_FIFO_AC_VO].out = 0;
               tx_info->stats[NV_TX_FIFO_AC_VO].out_sz = 0;
               spin_unlock_irqrestore(&tx_info->lock, flags);

               // Driver BE statistic
               outq_num_be = tx_info->stats[NV_TX_FIFO_AC_BE].out;
               outq_sz_be = tx_info->stats[NV_TX_FIFO_AC_BE].out_sz;
               spin_lock_irqsave(&tx_info->lock, flags);
               tx_info->stats[NV_TX_FIFO_AC_BE].out = 0;
               tx_info->stats[NV_TX_FIFO_AC_BE].out_sz = 0;
               spin_unlock_irqrestore(&tx_info->lock, flags);

               drop_num_be = tx_info->stats[NV_TX_FIFO_AC_BE].drop;
               spin_lock_irqsave(&tx_info->lock, flags);
               tx_info->stats[NV_TX_FIFO_AC_BE].drop = 0;
               spin_unlock_irqrestore(&tx_info->lock, flags);

               i = 0;
               do {
                   found = 0;
                   p = (u8 *)(monitor_info_p + i);

                   //nvt_dbg(CLEARVIEW, "i:%d\n", i);
                   //nvt_dbg(CLEARVIEW, "p:%p\n", p);

                   // sta mac is not zero
                  if (memcmp((u8 *)(p + MAC_Sts_mac_off), zero_mac, ETH_ALEN)) {
                       list_for_each_entry(sta_sts_in_ap,
                                        &nvt_priv->sta_list_in_ap, list) {
                           /*       // debug
                           nvt_dbg(CLEARVIEW, "sta:" MACSTR "\n",
                                  MAC2STR(sta_sts_in_ap->mac_addr));
                           nvt_dbg(CLEARVIEW, "MAC_Sts_mac_off:%d\n",
                                  MAC_Sts_mac_off);
                           //*/

                           if (!memcmp((u8 *)(p + MAC_Sts_mac_off),
                               sta_sts_in_ap->mac_addr, ETH_ALEN)) {
                               found = 1;
                               phyrate = mac_votxsz = mac_betxsz = 0;
                               mac_votxframe = mac_betxframe = 0;
                               mac_vodropsz = mac_bedropsz = 0;
                               mac_vodropframe = mac_bedropsz = 0;

                               phyrate = *((p + MAC_Sts_phyr_off));

                    #if 1
                               mac_votxsz = (
                                 (*((u8 *)(p + MAC_Sts_votxsz_off)) << 24) |
                                 (*((u8 *)(p + MAC_Sts_votxsz_off + 1)) << 16) |
                                 (*((u8 *)(p + MAC_Sts_votxsz_off + 2)) << 8) |
                                  (*((u8 *)(p + MAC_Sts_votxsz_off + 3))));
                               mac_betxsz = (
                                 (*((u8 *)(p + MAC_Sts_betxsz_off)) << 24) |
                                 (*((u8 *)(p + MAC_Sts_betxsz_off + 1)) << 16) |
                                 (*((u8 *)(p + MAC_Sts_betxsz_off + 2)) << 8) |
                                  (*((u8 *)(p + MAC_Sts_betxsz_off + 3))));
                               mac_votxframe = (
                                (*((u8 *)(p + MAC_Sts_votxfrm_off)) << 24) |
                                (*((u8 *)(p + MAC_Sts_votxfrm_off + 1)) << 16) |
                                (*((u8 *)(p + MAC_Sts_votxfrm_off + 2)) << 8) |
                                  (*((u8 *)(p + MAC_Sts_votxfrm_off + 3))));
                               mac_betxframe = (
                                (*((u8 *)(p + MAC_Sts_betxfrm_off)) << 24) |
                                (*((u8 *)(p + MAC_Sts_betxfrm_off + 1)) << 16) |
                                (*((u8 *)(p + MAC_Sts_betxfrm_off + 2)) << 8) |
                                (*((u8 *)(p + MAC_Sts_betxfrm_off + 3))));
                               mac_vodropsz = (
                                (*((u8 *)(p + MAC_Sts_vodrpsz_off)) << 24) |
                                (*((u8 *)(p + MAC_Sts_vodrpsz_off + 1)) << 16) |
                                (*((u8 *)(p + MAC_Sts_vodrpsz_off + 2)) << 8) |
                                (*((u8 *)(p + MAC_Sts_vodrpsz_off + 3))));
                               mac_bedropsz = (
                                (*((u8 *)(p + MAC_Sts_bedrpsz_off)) << 24) |
                                (*((u8 *)(p + MAC_Sts_bedrpsz_off + 1)) << 16) |
                                (*((u8 *)(p + MAC_Sts_bedrpsz_off + 2)) << 8) |
                                (*((u8 *)(p + MAC_Sts_bedrpsz_off + 3))));
                               mac_vodropframe = (
                                  (*((u8 *)(p + MAC_Sts_vodrpfrm_off)) << 8) |
                                  (*((u8 *)(p + MAC_Sts_vodrpfrm_off + 1))));
                               mac_bedropframe = (
                                  (*((u8 *)(p + MAC_Sts_bedrpfrm_off)) << 8) |
                                  (*((u8 *)(p + MAC_Sts_bedrpfrm_off + 1))));
                    #else
                               memcpy(&mac_votxsz,
                                   (u8 *)(monitor_info_p+i+MAC_Sts_votxsz_off),
                                   sizeof(u32));
                               mac_votxsz = endian_swap_u32(mac_votxsz);

                               memcpy(&mac_betxsz,
                                   (u8 *)(monitor_info_p+i+MAC_Sts_betxsz_off),
                                   sizeof(u32));
                               mac_betxsz = endian_swap_u32(mac_betxsz);

                               memcpy(&mac_votxframe,
                                   (u8 *)(monitor_info_p+i+MAC_Sts_votxfrm_off),
                                   sizeof(u32));
                               mac_votxframe = endian_swap_u32(mac_votxframe);

                               memcpy(&mac_betxframe,
                                   (u8 *)(monitor_info_p+i+MAC_Sts_betxfrm_off),
                                   sizeof(u32));
                               mac_betxframe = endian_swap_u32(mac_betxframe);

                               memcpy(&mac_vodropsz,
                                   (u8 *)(monitor_info_p+i+MAC_Sts_vodrpsz_off),
                                   sizeof(u32));
                               mac_vodropsz = endian_swap_u32(mac_vodropsz);

                               memcpy(&mac_bedropsz,
                                   (u8 *)(monitor_info_p+i+MAC_Sts_bedrpsz_off),
                                   sizeof(u32));
                               mac_bedropsz = endian_swap_u32(mac_bedropsz);

                               memcpy(&mac_vodropframe,
                                  (u8 *)(monitor_info_p+i+MAC_Sts_vodrpfrm_off),
                                  sizeof(u16));
                             mac_vodropframe = endian_swap_u16(mac_vodropframe);

                               memcpy(&mac_bedropframe,
                                  (u8 *)(monitor_info_p+i+MAC_Sts_bedrpfrm_off),
                                   sizeof(u16));
                             mac_bedropframe = endian_swap_u16(mac_bedropframe);
                    #endif
                               break;
                           }
                       }

                       if (found == 0) {
                           nvt_dbg(ERROR,
                           "%s: There's T-Put sts repored from MAC which ",
                           __func__);
                           nvt_dbg(ERROR, "doesn't join this AP ");
                           nvt_dbg(ERROR, "addr: %x:%x:%x:%x:%x:%x\n",
                               *((u8 *)(p + MAC_Sts_mac_off)),
                               *((u8 *)(p + MAC_Sts_mac_off + 1)),
                               *((u8 *)(p + MAC_Sts_mac_off + 2)),
                               *((u8 *)(p + MAC_Sts_mac_off + 3)),
                               *((u8 *)(p + MAC_Sts_mac_off + 4)),
                               *((u8 *)(p + MAC_Sts_mac_off + 5)));
                       } else {
                           phyrate_found = 0;
                           for (j = 0; j < TP_MONITOR_PHYRATE_TBL_SZ; j++) {
                              if (phyrate == tp_monitor_phyrate_map_tbl[j][0]) {
                                   phyrate = tp_monitor_phyrate_map_tbl[j][1];
                                   phyrate_found = 1;
                                   break;
                               }
                           }

                           if (phyrate_found == 0) {
                               sta_sts_in_ap->phyrate = 0;
                               nvt_dbg(CLEARVIEW,
                                  "%s: PhyRate:0x%x mapping failed, set to 0\n",
                                  __func__, sta_sts_in_ap->phyrate);
                           } else {
                               // Info from MAC (TX size/frame cnt in VO/BE)
                               sta_sts_in_ap->mac_votxsz = mac_votxsz;
                               sta_sts_in_ap->mac_betxsz = mac_betxsz;
                               sta_sts_in_ap->mac_votxframe = mac_votxframe;
                               sta_sts_in_ap->mac_betxframe = mac_betxframe;

                               // Info from MAC (Drop size/frame cnt in VO/BE)
                               sta_sts_in_ap->mac_vodropsz = mac_vodropsz;
                               sta_sts_in_ap->mac_bedropsz = mac_bedropsz;
                               sta_sts_in_ap->mac_vodropframe = mac_vodropframe;
                               sta_sts_in_ap->mac_bedropframe = mac_bedropframe;

                               nvt_dbg(CLEARVIEW, "@ mac_votx:%04d,"
                                    "mac_votxsz:%08d, mac_betx:%04d,"
                                    "mac_betxsz:%08d\n",
                                    mac_votxframe, mac_votxsz,
                                    mac_betxframe, mac_betxsz);
                               nvt_dbg(CLEARVIEW, "mac_vodrop:%04d,"
                                   "mac_vodropsz:%08d, mac_bedrop:%04d,"
                                   "mac_bedropsz:%08d\n",
                                   mac_vodropframe, mac_vodropsz,
                                   mac_bedropframe, mac_bedropsz);
                               nvt_dbg(CLEARVIEW, "outq_num_vo:%04d,"
                                   "outq_sz_vo:%08d, outq_num_be:%04d,"
                                   "outq_sz_be:%08d\n",
                                   outq_num_vo, outq_sz_vo,
                                   outq_num_be, outq_sz_be);
                               nvt_dbg(CLEARVIEW, "drop_num_be: %04d\n",
                                       drop_num_be);
                               nvt_dbg(CLEARVIEW, "phyrate: %02d.%d Mb/s\n",
                                   phyrate/1000, (phyrate/100)%10);

                               #if 0  /* print format 1 */
                               printk("@ mac_votx:%04d, mac_votxsz:%08d,"
                                      "mac_betx:%04d, mac_betxsz:%08d\n",
                                      mac_votxframe, mac_votxsz,
                                      mac_betxframe, mac_betxsz);
                               printk("mac_vodrop:%04d, mac_vodropsz:%08d,"
                                      "mac_bedrop:%04d, mac_bedropsz:%08d\n",
                                      mac_vodropframe, mac_vodropsz,
                                      mac_bedropframe, mac_bedropsz);
                               printk("outq_num_vo:%04d, outq_sz_vo:%08d,"
                                      "outq_num_be:%04d, outq_sz_be:%08d\n",
                                      outq_num_vo, outq_sz_vo,
                                      outq_num_be, outq_sz_be);
                               printk("drop_num_be: %04d\n", drop_num_be);
                               printk("phyrate: %02d.%d Mb/s\n",
                                   phyrate/1000, (phyrate/100)%10);
                               #endif

                               #if 0  /* print format 2 */
                               printk("             VO                BE\n");
                               printk("==============================================\n");
                               printk("   drv tx:   ");
                               printk("%-4d(%-8d)    ",
                                       outq_num_vo, outq_sz_vo);
                               printk("%-4d(%-8d)\n", outq_num_be, outq_sz_be);

                               printk(" drv drop:   ");
                               printk("%-4d              ",
                                      tx_info->stats[NV_TX_FIFO_AC_VO].drop);
                               printk("%-4d\n", drop_num_be);

                               printk("   mac tx:   ");
                               printk("%-4d(%-8d)    ",
                                       mac_votxframe, mac_votxsz);
                               printk("%-4d(%-8d)\n",
                                       mac_betxframe, mac_betxsz);

                               printk(" mac drop:   ");
                               printk("%-4d(%-8d)    ",
                                      mac_vodropframe, mac_vodropsz);
                               printk("%-4d(%-8d)\n",
                                      mac_bedropframe, mac_bedropsz);

                               printk("phyrate: %02d.%d Mb/s\n", phyrate/1000,
                                      (phyrate/100)%10);
                               #endif

                               #if 1  /* print format 3 */
                               printk("@ %02d.%d ",
                                      phyrate/1000, (phyrate/100)%10);
                               printk("%04d %08d %04d %08d ",
                                      mac_votxframe, mac_votxsz,
                                      mac_vodropframe, mac_vodropsz);
                               printk("%04d %08d %04d %08d\n",
                                      mac_betxframe, mac_betxsz,
                                      mac_bedropframe, mac_bedropsz);
                               printk("Drv %04d %08d %04d %08d %04d\n",
                                      outq_num_vo, outq_sz_vo, outq_num_be,
                                      outq_sz_be, drop_num_be);
                               #endif

                               // Record the last records
                         // Note: The latest one will at the beginning of array
                               for (j = (win_sz - 1); j > 0; j--) {
                                   sta_sts_in_ap->phyrate_history[j] =
                                       sta_sts_in_ap->phyrate_history[j-1];
                                   sta_sts_in_ap->mac_vodropframe_history[j] =
                                    sta_sts_in_ap->mac_vodropframe_history[j-1];
                                   sta_sts_in_ap->mac_bedropframe_history[j] =
                                    sta_sts_in_ap->mac_bedropframe_history[j-1];
                                   sta_sts_in_ap->outq_vo_be_history[j] =
                                       sta_sts_in_ap->outq_vo_be_history[j-1];
                                   sta_sts_in_ap->drop_be_history[j] =
                                       sta_sts_in_ap->drop_be_history[j-1];
                               }
                               sta_sts_in_ap->mac_vodropframe_history[0] =
                                   sta_sts_in_ap->mac_vodropframe;
                               sta_sts_in_ap->mac_bedropframe_history[0] =
                                   sta_sts_in_ap->mac_bedropframe;
                               sta_sts_in_ap->outq_vo_be_history[0] =
                                   outq_num_be + outq_num_vo;
                               sta_sts_in_ap->drop_be_history[0] =
                                   drop_num_be;
                               sta_sts_in_ap->phyrate_history[0] = phyrate;

                               // Average it
                               sta_sts_in_ap->enq_in_vo_be = 0;
                               sta_sts_in_ap->drop_in_be = 0;
                               for (j = 0; j < win_sz; j++) {
                                   sta_sts_in_ap->enq_in_vo_be +=
                                       sta_sts_in_ap->outq_vo_be_history[j];
                                   sta_sts_in_ap->drop_in_be +=
                                       sta_sts_in_ap->drop_be_history[j];
                               }
                               sta_sts_in_ap->enq_in_vo_be /= win_sz;
                               sta_sts_in_ap->drop_in_be /= win_sz;

                    #if 0 /* average the last 8 PhyRate */
                               for (j = 0; j < win_sz; j++) {
                                   if (!sta_sts_in_ap->phyrate_history[j])
                                       break;
                               }

                               if (j == win_sz) {
                                   sta_sts_in_ap->phyrate = 0;
                                   for (j = 0; j < win_sz; j++) {
                                       sta_sts_in_ap->phyrate +=
                                           sta_sts_in_ap->phyrate_history[j];
                                   }
                                   sta_sts_in_ap->phyrate /= win_sz;
                               } else {
                                   sta_sts_in_ap->phyrate = phyrate;
                               }
                    #endif
                    #if 0 /* show the hisotry of phyrate */
                               for (j = 0; j < win_sz; j++) {
                                   dbg_msg(DBG_FLAG_TP_MONITOR, ("%06d ",
                                       sta_sts_in_ap->phyrate_history[j]));
                               }
                               dbg_msg(DBG_FLAG_TP_MONITOR, ("%06d\n",
                                   sta_sts_in_ap->phyrate));
                    #endif
                               cview_predict_tp(sta_sts_in_ap, win_sz);
                           }
                       }
                   } else {
                       nvt_dbg(ERROR, "%s: MAC is zero\n", __func__);
                   }
                   i += MAC_Status_Len;
               } while (i < monitor_info_p_len);
           }

       }

       return 0;
}

s32 cview_predict_tp(struct _sta_sts_in_ap *sta_sts_in_ap, int win_sz)
{
    int i, ret = 0;
    unsigned int ori_t_put, first = 0;

    for (i = 1; i < win_sz; i++) {
        if (sta_sts_in_ap->phyrate_history[i])
            break;
    }

    if (i == win_sz) {
        // Initial value. Using PhyRate to estimate T-Put cause we don't have
        // any information related to T-Put.
        // phyrate*0.5 (bits)
        sta_sts_in_ap->t_put =
            (sta_sts_in_ap->phyrate_history[0] >> 1) * 1000;

        nvt_dbg(CLEARVIEW,
                "cview_predict_tp: phyrate_history[0]:%d, init t_put:%d\n",
                 sta_sts_in_ap->phyrate_history[0], sta_sts_in_ap->t_put);

        ori_t_put = sta_sts_in_ap->phyrate_history[0];
        first = 1;

        sta_sts_in_ap->incr_counter = 0;
    } else {
        // Changing initial T-Put especial as it changed DRate as 1 or 2
//         if(sta_sts_in_ap->phyrate_history[1] > 2000) {
//             if(sta_sts_in_ap->phyrate_history[0] == 2000) {
//                 sta_sts_in_ap->t_put = TPUT_IN_PHY_2_0;
//             }
//             else if(sta_sts_in_ap->phyrate_history[0] == 1000) {
//                 sta_sts_in_ap->t_put = TPUT_IN_PHY_1_0;
//             }
//         }

        ori_t_put = sta_sts_in_ap->t_put;
        first = 0;

        if (sta_sts_in_ap->mac_vodropframe_history[0]) {
            //sta_sts_in_ap->t_put *= (0.5);
            sta_sts_in_ap->t_put = ori_t_put / 2;
            sta_sts_in_ap->incr_counter = 0;
        } else if (sta_sts_in_ap->drop_be_history[0]) {
            //sta_sts_in_ap->t_put *= (0.7);
            sta_sts_in_ap->t_put = (ori_t_put*7)/10 + (ori_t_put*7)%10;
            sta_sts_in_ap->incr_counter = 0;
        } else if (sta_sts_in_ap->mac_bedropframe_history[0] > 0) {

        #if 0
            for (j = 1; j < VCLEAR_MACBEDROP_THRES; j++) {
                if (sta_sts_in_ap->mac_bedropframe_history[j] < 10)
                    break;
            }

            if (j == VCLEAR_MACBEDROP_THRES) {
                sta_sts_in_ap->t_put *= (0.9);
            } else {
                // hold the t-put
            }
        #else
            //sta_sts_in_ap->t_put *= (0.9);
            sta_sts_in_ap->t_put = (ori_t_put*9)/10 + (ori_t_put*9)%10;

            if ((sta_sts_in_ap->mac_bedropframe_history[1] > 0) &&
                (sta_sts_in_ap->mac_bedropframe_history[0] >=
                sta_sts_in_ap->mac_bedropframe_history[1])) {
                //sta_sts_in_ap->t_put *= (0.9);
                sta_sts_in_ap->t_put = (ori_t_put*9)/10 + (ori_t_put*9)%10;
            }
        #endif
            sta_sts_in_ap->incr_counter = 0;
        } else {
            #if 0
            for (j = 0; j < VCLEAR_INCR_TP_THRES; j++) {
                if (sta_sts_in_ap->mac_bedropframe_history[j] ||
                   sta_sts_in_ap->mac_bedropframe_history[j] ||
                   sta_sts_in_ap->drop_be_history[j]) {
                    break;
                }
            }

            if (j == VCLEAR_INCR_TP_THRES) {
                sta_sts_in_ap->t_put *= (1.1);
            } else {
                // hold this t-put
            }
            #else
            if (!sta_sts_in_ap->mac_bedropframe_history[0]) {
                sta_sts_in_ap->incr_counter++;
            } else {
                sta_sts_in_ap->incr_counter = 0;
            }
            #if 0
            if (sta_sts_in_ap->incr_counter == 4) {
                // increase more as its t-put less than 20KB or 30KB
                if (sta_sts_in_ap->t_put < 20 * 1000 * 8) {
                    sta_sts_in_ap->t_put *= (1.3);
                } else if (sta_sts_in_ap->t_put < 30 * 1000 * 8) {
                    sta_sts_in_ap->t_put *= (1.2);
                } else {
                    sta_sts_in_ap->t_put *= (1.1);
                }
                sta_sts_in_ap->incr_counter = 0;
            }
            #else
            if ((sta_sts_in_ap->t_put < QP_turning_point_3) &&
                (sta_sts_in_ap->incr_counter == 4)) {
                //sta_sts_in_ap->t_put *= (1.8);
                sta_sts_in_ap->t_put = (ori_t_put*18)/10 + (ori_t_put*18)%10;

                sta_sts_in_ap->incr_counter = 0;
            } else if (sta_sts_in_ap->incr_counter == 8) {
                //sta_sts_in_ap->t_put *= (1.2);
                sta_sts_in_ap->t_put = (ori_t_put*12)/10 + (ori_t_put*12)%10;

                sta_sts_in_ap->incr_counter = 0;
            }
            #endif
            #endif
        }
    }

    // Prevent T-put from exceeding its limitation
    if (sta_sts_in_ap->t_put > VCLEAR_MAX_TP) {
        sta_sts_in_ap->t_put = VCLEAR_MAX_TP;
    } else if (sta_sts_in_ap->phyrate_history[0] == 1000) {
        if (sta_sts_in_ap->t_put > TPUT_IN_PHY_1_0) {
            sta_sts_in_ap->t_put = TPUT_IN_PHY_1_0;
        }
    } else if (sta_sts_in_ap->phyrate_history[0] == 2000) {
        if (sta_sts_in_ap->t_put > TPUT_IN_PHY_2_0) {
            sta_sts_in_ap->t_put = TPUT_IN_PHY_2_0;
        }
    }

    // Set 10KB as its lowerbound
    if (sta_sts_in_ap->t_put < VCLEAR_MIN_TP)
        sta_sts_in_ap->t_put = VCLEAR_MIN_TP;

    nvt_dbg(CLEARVIEW, "%s: [%02x:%02x:%02x:%02x:%02x:%02x] t_put: %d\n",
                     __func__,
                     sta_sts_in_ap->mac_addr[0], sta_sts_in_ap->mac_addr[1],
                     sta_sts_in_ap->mac_addr[2], sta_sts_in_ap->mac_addr[3],
                     sta_sts_in_ap->mac_addr[4], sta_sts_in_ap->mac_addr[5],
                     sta_sts_in_ap->t_put);
    //*
    printk("[%02x:%02x:%02x:%02x:%02x:%02x] first:%d t_put:%08d (ori:%08d)\n",
           sta_sts_in_ap->mac_addr[0], sta_sts_in_ap->mac_addr[1],
           sta_sts_in_ap->mac_addr[2], sta_sts_in_ap->mac_addr[3],
           sta_sts_in_ap->mac_addr[4], sta_sts_in_ap->mac_addr[5],
           first,
           sta_sts_in_ap->t_put, ori_t_put);
    //*/
    return ret;
}

#endif

static void remote_fw_input(struct sk_buff *skb)
{
    u8 const *delim = ",";
    u8 *token;
    int fuart_mode = 0;
    int ret;
    unsigned int src_pid, src_seq;
    void *data;
    struct nlmsghdr *nlh;
    struct net_device *dev;
    struct _nvt_if *nvt_if;
        struct _nvt_bus *nvt_bus;
    struct _nvt_priv *nvt_priv;

    nlh = (struct nlmsghdr *)skb->data;
    src_pid = nlh->nlmsg_pid;
    src_seq = nlh->nlmsg_seq;
    data = NLMSG_DATA(nlh);
    //printk("string from user-space:%s\n", data);
    token = strsep((char **)&data, delim);
    dev = __dev_get_by_name(&init_net, token);
    if (!dev) {
        nvt_dbg(ERROR, "Incorrect Network Name: %s\n", token);
        goto err;
    }
    nvt_if = netdev_priv(dev);
    nvt_bus = nvt_if->nvt_adapter->nvt_bus;
    nvt_priv = &nvt_if->nvt_adapter->nvt_priv;

    token = strsep((char **)&data, delim);
    if (!strncmp(token, "RemoteDebugStart", 16)) {
        nvt_dbg(REMOTEDBG, "%s: RemoteDebug is starting...\n",
                __func__);
        nvt_priv->remote_debug_pid = src_pid;
        nvt_priv->remote_debug_start = 1;
        nvt_dbg(REMOTEDBG, "%s: utility_pid:%d\n", __func__,
            nvt_priv->remote_debug_pid);

        /* remote debug mode */
        fuart_mode = 2;

        ret = nvt_icfg_lock(nvt_bus);
        if (ret < 0) {
            goto icfg_send_err;
        }
        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
            goto icfg_send_err;
        }
        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_UART_MODE,
            (u8 *)&fuart_mode, sizeof(fuart_mode));
        if (ret < 0) {
            goto icfg_send_err;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
        nvt_icfg_unlock(nvt_bus);
    } else if (!strncmp(token, "RemoteDebugStop", 15)) {
        nvt_dbg(REMOTEDBG, "%s: RemoteDebug is closing...\n", __func__);

        fuart_mode = 1;
        ret = nvt_icfg_lock(nvt_bus);
        if (ret < 0) {
             goto icfg_send_err;
        }
        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
             goto icfg_send_err;
        }
        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_UART_MODE,
                  (u8 *)&fuart_mode, sizeof(fuart_mode));
        if (ret < 0) {
             goto icfg_send_err;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
        nvt_icfg_unlock(nvt_bus);

        nvt_priv->remote_debug_start = 0;
        nvt_priv->remote_debug_pid = 0;
    } else {
        nvt_dbg(ERROR, "Not support string:%s\n", token);
    }

    //kfree_skb(skb);
    return;

icfg_send_err:
        nvt_icfg_unlock(nvt_bus);
err:
    //kfree_skb(skb);
    return;
}

static s32 nvt_remote_fwlog(struct _nvt_if *nvt_if, const u16 msg_len, u8 *data)
{
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    struct _nvt_priv *nvt_priv = &nvt_if->nvt_adapter->nvt_priv;
    struct sock *remote_fwlog_sock = nvt_priv->remote_fwlog_sock;
    int ret = 0;
    /* 1 more byte is null character */
    int len = NLMSG_SPACE(msg_len + 1);

    if (nvt_priv->remote_debug_start == 1) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
        skb = alloc_skb(len, GFP_ATOMIC);
#else
        skb = nlmsg_new(len, GFP_ATOMIC);
#endif
        if (!skb) {
          ret = 1;
          goto skb_alloc_failed;
        }
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
        nlh = NLMSG_PUT(skb, 0, 0, 0, msg_len + 1);
        nlh->nlmsg_flags = 0;
#else
        nlh = nlmsg_put(skb, 0, 0, 0, msg_len + 1, 0);
#endif
        memcpy(NLMSG_DATA(nlh), data, msg_len);
        *((char *)(NLMSG_DATA(nlh) + msg_len)) = '\0';
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 7, 0)
        /* from Kernel */
        NETLINK_CB(skb).pid = 0;
#else
        /* from Kernel */
        NETLINK_CB(skb).portid = 0;
#endif
        /* unicast */
        NETLINK_CB(skb).dst_group = 0;
        ret = netlink_unicast(remote_fwlog_sock, skb,
                  nvt_priv->remote_debug_pid, MSG_DONTWAIT);

        if (ret < 0) {
            nvt_dbg(ERROR, "%s:send failed\n", __func__);
        } else {
            nvt_dbg(REMOTEDBG, "%s:Log has sent, pid:%d\n",
                    __func__, nvt_priv->remote_debug_pid);
        }
    } else {
        nvt_dbg(REMOTEDBG, "%s:remote_debug_start:%d rather than 1\n",
                  __func__, nvt_priv->remote_debug_start);
    }
    return 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
/* used by NLMSG_PUT macro */
nlmsg_failure:
    kfree_skb(skb);
#endif
skb_alloc_failed:
    return ret;
}

static s32 nvt_fifo_credit_init(struct _nvt_if *nvt_if,
        const u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_priv->tx_info;
        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;

        u8 fc_enable_flag = *((u8 *)(data + 12));
        u8 *credits = (u8 *)(data + 2);
        s32 i = 0;
        ulong flags;

        nvt_dbg(TX, "%s\n", __func__);

        spin_lock_irqsave(&tx_info->lock, flags);
        if (fc_enable_flag & (1<<7)) {
                tx_info->is_credit_borrow = 1;
        } else {
                tx_info->is_credit_borrow = 0;
        }
        spin_unlock_irqrestore(&tx_info->lock, flags);

        /*initialize fc_borrow_credit info*/
        if (fc_enable_flag & (1<<7)) {
                if (nvt_icfg_lock(nvt_bus) < 0) {
                        goto next_check;
                }

                if (nvt_icfg_reset(nvt_bus) < 0) {
                        goto next_check;
                }

                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_FC_BORROW,
                        (u8 *)&(tx_info->is_credit_borrow),
                        sizeof(tx_info->is_credit_borrow)) < 0) {
                        goto next_check;
                }

                if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                        goto next_check;
                }
next_check:
                nvt_icfg_unlock(nvt_bus);
        }

        spin_lock_irqsave(&tx_info->lock, flags);
        for (i = 0; i < NV_TX_FIFO_COUNT; i++) {
                if (*credits) {
                        tx_info->fifo_credit_map |= 1 << i;
                } else {
                        tx_info->fifo_credit_map &= ~(1 << i);
                }
                tx_info->fifo_max_credit[i] = *credits++;
        }

        for (i = 0; i < NV_TX_FIFO_COUNT; i++) {
                if (*credits >= tx_info->fifo_max_credit[i]) {
                        tx_info->fifo_credit_map &= ~(1 << i);
                        tx_info->fifo_credit[i] = 0;
                } else {
                        tx_info->fifo_credit[i] =
                                tx_info->fifo_max_credit[i] - *credits;
                }
                credits++;
        }

        tx_info->fcmode = *credits++;
        tx_info->is_netif_stop = 0;

        /* tx_info->fcmode == 0 means no flow control */
        if (tx_info->fcmode == 0) {
                for (i = 0; i < NV_TX_FIFO_COUNT; i++)
                        tx_info->fifo_credit_map &= ~(1 << i);
        } else {
                /* Do nothing */
        }

        for (i = 0; i < NV_TX_FIFO_COUNT; i++) {
                tx_info->fifo_credit_update[i] = *credits++;
        }

        for (i = 0; i < NV_TX_FIFO_COUNT - 1; i++) {
                tx_info->fifo_public_inuse[i] = *credits++;
        }

        for (i = 0; i < NV_TX_FIFO_COUNT - 1; i++) {
                tx_info->fifo_public_update[i] = *credits++;
        }

        nvt_dbg(TX, "fifo_creditmap=%x, fcmode=%d\n",
                                tx_info->fifo_credit_map, tx_info->fcmode);

        for (i = 0; i < NV_TX_FIFO_COUNT; i++) {
                nvt_dbg(TX, "fifo[%d]=max:%d, credit:%d, update:%d\n",
                                i, tx_info->fifo_max_credit[i],
                                tx_info->fifo_credit[i],
                                tx_info->fifo_credit_update[i]);
        }

        for (i = 0; i < NV_TX_FIFO_COUNT - 1; i++) {
                nvt_dbg(TX, "fifo[%d]=public_inuse:%d, public_update:%d\n",
                                i, tx_info->fifo_public_inuse[i],
                                tx_info->fifo_public_update[i]);
        }

        nvt_shedule_tx_dequeue(tx_info);
        spin_unlock_irqrestore(&tx_info->lock, flags);

        return 0;
}

s32 nvt_reset_fcmode(struct _nvt_if *nvt_if, u8 given_type)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_priv->tx_info;
        if (given_type != nvt_if->mode) {
                if (tx_info->fcmode) {
                        tx_info->fcmode = 0;
                }
        }
        return 0;
}

static s32 nvt_fifocredit_indicate(struct _nvt_if *nvt_if,
        const u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_priv->tx_info;
        u8 *credits = (u8 *)(data + 2);
        u8 i = 0;
        u8 inc = 0;
        u32 ok = 0;
        u32 fail = 0;
        ulong flags;
        u32 total_cnt = 0;

        nvt_dbg(TX, "%s\n", __func__);

        spin_lock_irqsave(&tx_info->lock, flags);
        for (i = 0; i < NV_TX_FIFO_COUNT; i++) {
                if (credits[i] >= tx_info->fifo_credit_update[i]) {
                        inc = credits[i] - tx_info->fifo_credit_update[i];
                } else {
                        inc = 255 - (tx_info->fifo_credit_update[i] -
                                credits[i]) + 1;
                }
                tx_info->fifo_credit_update[i] = credits[i];
                tx_info->fifo_credit[i] += inc;
                if (tx_info->fifo_credit[i] > tx_info->fifo_max_credit[i]) {
                        nvt_dbg(TX, "fifo[%d],inc=%d,over!\n", i, inc);
                        tx_info->fifo_credit[i] = tx_info->fifo_max_credit[i];
                }

                if (tx_info->fifo_credit[i] > 0) {
                        tx_info->fifo_credit_map |= 1 << i;
                } else {
                        tx_info->fifo_credit_map &= ~(1 << i);
                }

                nvt_dbg(TX, "update fifo_credit[%d] = %d\n",
                        i, tx_info->fifo_credit[i]);
        }

        ok = (credits[8] << 24) | (credits[7] << 16) |
                (credits[6] << 8) | credits[5];
        fail = (credits[12] << 24) | (credits[11] << 16) |
                (credits[10] << 8) | credits[9];

        nvt_dbg(TX, "tx ok=%d, fail=%d\n", ok, fail);

        for (i = 0; i < NV_TX_FIFO_COUNT - 1; i++) {
                if (credits[21 + i] >= tx_info->fifo_public_update[i]) {
                        inc = credits[21 + i] - tx_info->fifo_public_update[i];
                } else {
                        inc = 255 - (tx_info->fifo_public_update[i] -
                                credits[21 + i]) + 1;
                }
                tx_info->fifo_public_update[i] = credits[21 + i];
                tx_info->fifo_public_inuse[i] -= inc;
                if (tx_info->fifo_public_inuse[i] >
                        tx_info->fifo_max_credit[NV_TX_FIFO_PUBLIC]) {
                        nvt_dbg(TX, "fifo[%d],inc=%d,in_use=%d,over!\n", i, inc,
                                tx_info->fifo_public_inuse[i]);
                        tx_info->fifo_public_inuse[i] = 0;
                }

                nvt_dbg(TX, "update fifo_public_inuse[%d] = %d\n",
                        i, tx_info->fifo_public_inuse[i]);
                total_cnt += tx_info->fifo_public_inuse[i];
        }

        if (total_cnt + tx_info->fifo_credit[NV_TX_FIFO_PUBLIC] !=
                tx_info->fifo_max_credit[NV_TX_FIFO_PUBLIC]) {
                nvt_dbg(TX, "Public credit mismatch!!!\n");
        }

        nvt_shedule_tx_dequeue(tx_info);
        spin_unlock_irqrestore(&tx_info->lock, flags);

        return 0;
}

static void update_piggyback_fifocredit(struct _nvt_if *nvt_if,
        struct _nvt_hwinfo_rx *info)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_priv->tx_info;
        u8 data[30] = {0};
        u16 msg_len = 30;
        s32 err = 0;
        static u8 old_pri;
        static u8 new_pri;
        static u8 old_public;
        static u8 new_public;

        /* tx_info->fcmode == 0 means no flow control */
        if (tx_info->fcmode == 0) {
                return;
        }

        nvt_dbg(TX, "%s\n", __func__);

        data[2] = (ntohl(info->word2) & 0xFF000000) >> 24;
        data[3] = (ntohl(info->word2) & 0x00FF0000) >> 16;
        data[4] = (ntohl(info->word2) & 0x0000FF00) >> 8;
        data[5] = (ntohl(info->word2) & 0x000000FF);

        data[23] = info->word4.vo_piggy;
        data[24] = info->word5.vi_piggy;
        data[25] = info->word5.be_piggy;
        data[26] = info->word5.bk_piggy;

        data[6] = data[23] + data[24] + data[25] + data[26];

        new_pri = data[2] + data[3] + data[4] + data[5];
        new_public = data[6];

        nvt_dbg(TX, "old_pri=%d,new_pri=%d,old_public=%d,new_public=%d\n",
                old_pri, new_pri, old_public, new_public);
        if ((new_pri == old_pri) && (new_public == old_public)) {
                return;
        }
        old_pri = new_pri;
        old_public = new_public;

        nvt_dbg(TX, "VO=%d,VI=%d,BE=%d,BK=%d,pub=%d\n",
                data[2], data[3], data[4], data[5], data[6]);
        nvt_dbg(TX, "PUB: VO=%d,VI=%d,BE=%d,BK=%d\n",
                data[23], data[24], data[25], data[26]);

        err = nvt_fifocredit_indicate(nvt_if, msg_len, data);

}

/**
 * nvt_is_priv_pkt - this function checks whether the received frame is
 *                   event/iconfig or not
 * @nvt_bus: bus structrue
 * @skb: rx skb buffer
 *
 * Return: true - the received frame is event/iconfig.
 *         false - the received frame is not event/iconfig.
 */
s32 nvt_is_priv_pkt(struct _nvt_bus *nvt_bus, struct sk_buff *skb)
{
        struct _nvt_hwinfo_rx *info;

        nvt_dbg(REORDER, "%s\n", __func__);

        info = (struct _nvt_hwinfo_rx *)skb->data;

        if ((ntohl(info->word0) & 0x80000000) == 0x80000000) {
                return 1;
        } else {
                return 0;
        }
}

/**
 * nvt_process_priv_pkt - this function process event/iconfig to do
 *                        specific action.
 * @nvt_bus: bus structrue
 * @skb: rx skb buffer
 *
 * we add the event/iconfig handler here.
 *
 * Return: zero - success.
 *         negative - failure.
 */
s32 nvt_process_priv_pkt(struct _nvt_bus *nvt_bus, struct sk_buff *skb)
{
        /* Event Message Format                                     */
        /* -------------------------------------------------------- */
        /* | Message Type | ID | Length | WID Type | WID Data     | */
        /* -------------------------------------------------------- */
        /* | 1            | 1  | 2      | 2        |  event_len   | */
        /* -------------------------------------------------------- */
        struct _nvt_adapter *nvt_adapter = nvt_bus->nvt_adapter;
        struct _nvt_priv *nvt_priv;
        struct _nvt_hwinfo_rx *info;
        struct _nvt_if *nvt_if;
        u8 *msg_type;
        u8 *data;
        u16 *wid;
        u16 msg_len;
        u8 ifidx = 0;
        s32 err = 0;

        info = (struct _nvt_hwinfo_rx *)skb->data;
        msg_type = (u8 *)(skb->data + info->word1.offset + WID_EVENT_OFFSET);
        data = (u8 *)(skb->data + info->word1.offset +
                WID_EVENT_OFFSET + WID_VALUE_OFFSET);
        wid = (u16 *)(skb->data + info->word1.offset +
                WID_EVENT_OFFSET + WID_VALUE_OFFSET);
        msg_len = *((u16 *)(skb->data + info->word1.offset +
                WID_EVENT_OFFSET + WID_LENGTH_OFFSET)) - 4;

        nvt_dbg(INFO, "%s\n", __func__);
        nvt_dbg(INFO, "msg_type = %c, wid = 0x%04x\n", *msg_type, *wid);

        //20150806 nash: handling firmware ready event
        if (*wid == WID_EVENT_FW_READY) {
                nvt_bus->fw_rdy_completed = 1;
                if (waitqueue_active(&nvt_bus->fw_rdy_wait)) {
                        wake_up(&nvt_bus->fw_rdy_wait);
                } else {
                        nvt_dbg(INFO, "%s: no active wait event\n", __func__);
                }
                return 0;
        }

        if (nvt_bus->nvt_adapter == NULL) {
                nvt_dbg(INFO, "%s: nvt_adapter is NULL\n", __func__);
                return -1;
        }
        nvt_priv = &nvt_adapter->nvt_priv;

        /* Get nvt_if pointer */
        nvt_if = nvt_get_if_by_index(nvt_adapter, ifidx);
        if (!nvt_if) {
                dev_kfree_skb_any(skb);
                nvt_dbg(INFO, "ifidx = %d, nvt_if not match\n", ifidx);
                return -1;
        }

        /* pull out Rx Host Header */
        skb_pull(skb, info->word1.offset);

        /* Process FW Message_Type == 'E' */
        if (*wid == WID_EVENT_FC_CREDIT_INIT) {
                nvt_dbg(INFO,
                        "FC: vo=%d, vi=%d, be=%d, bk=%d, pub=%d\n",
                        *(data + 2), *(data + 3), *(data + 4),
                        *(data + 5), *(data + 6));

                err = nvt_fifo_credit_init(nvt_if, msg_len, data);
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_FC_INFO) {
                nvt_dbg(TX,
                        "FC: seq=%d,vo=%d,vi=%d,be=%d,bk=%d,pub=%d\n",
                        *(msg_type + 1), *(data + 2), *(data + 3),
                        *(data + 4), *(data + 5), *(data + 6));
                err = nvt_fifocredit_indicate(nvt_if, msg_len, data);
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_ADDBA_REQ) {
                nvt_dbg(INFO, "ADDBA REQ EVENT\n");
                err = nvt_handle_addba_req_evt(nvt_if, msg_len, data);
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_DELBA_REQ) {
                nvt_dbg(INFO, "DELBA REQ EVENT\n");
                err = nvt_handle_delba_evt(nvt_if, msg_len, data);
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_BAR) {
                nvt_dbg(INFO, "BAR REQ EVENT\n");
                err = nvt_handle_bar_evt(nvt_if, msg_len, data);
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_DHCP_RENEW_FIRST_TIME) {
                if (*(data + 2) == 0) {
                        nvt_dbg(INFO, "Send the first DHCP REQ Success\n");
                } else {
                        nvt_dbg(INFO, "Send the first DHCP REQ Fail\n");
                }
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_TP_MONITOR) {
                nvt_dbg(INFO, "Got TP_MONITOR EVENT\n");
                #ifdef CONFIG_WIFI_TUNING_PHASE_II
                err = nvt_notify_tp_monitor(nvt_if, msg_len, data);
                #endif
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_DEBUG_MESSAGE_INFO) {
                err = nvt_remote_fwlog(nvt_if, (msg_len - 2), (u8 *)(data + 2));
                dev_kfree_skb_any(skb);
        } else if (*wid == WID_EVENT_SETKEY_INFO) {
                nvt_dbg(INFO, "Got WID_EVENT_SETKEY_INFO EVENT\n");
                err = nvt_setkey_info(nvt_if, msg_len, data);
                dev_kfree_skb_any(skb);
        } else if (*wid == 0x0500) {
                nvt_dbg(INFO, "Got ADDKEY_MSG4_INFO_ind\n");
                atomic_dec(&nvt_if->eapol_cnt);
                nvt_dbg(INFO, "pend_8021x_cnt = %d\n",
                        atomic_read(&nvt_if->eapol_cnt));

                if (waitqueue_active(&nvt_if->addkey_wait)) {
                        wake_up(&nvt_if->addkey_wait);
                        nvt_dbg(INFO, "wake_up\n");
                }
                dev_kfree_skb_any(skb);
        } else {
                nvt_dbg(INFO, "put WID('E') in workqueue\n");
                skb_queue_tail(&nvt_priv->fw_evt_list, skb);
                schedule_work(&nvt_priv->fw_evt_work);
        }
        return err;
}

static s32 nvt_evt_handle(struct _nvt_adapter *nvt_adapter,
        struct sk_buff *skb)
{
        /* Event Message Format                                     */
        /* -------------------------------------------------------- */
        /* | Message Type | ID | Length | WID Type | WID Data     | */
        /* -------------------------------------------------------- */
        /* | 1            | 1  | 2      | 2        |  event_len   | */
        /* -------------------------------------------------------- */
        struct _nvt_if *nvt_if;
        u8 *msg_type = (u8 *)(skb->data + WID_EVENT_OFFSET);
        u8 *data = (u8 *)(skb->data + WID_EVENT_OFFSET + WID_VALUE_OFFSET);
        u16 *wid = (u16 *)(skb->data + WID_EVENT_OFFSET + WID_VALUE_OFFSET);
        u16 msg_len = *((u16 *)(skb->data +
                WID_EVENT_OFFSET + WID_LENGTH_OFFSET)) - 4;
        s32 err = 0;
        u8 ifidx = 0;
        u8 sta_type = 0;

        nvt_dbg(INFO, "%s\n", __func__);
        nvt_dbg(INFO, "msg_type = %c, wid = 0x%04x\n", *msg_type, *wid);

        /* Get nvt_if pointer */
        nvt_if = nvt_get_if_by_index(nvt_adapter, ifidx);
        if (!nvt_if) {
                nvt_dbg(ERROR, "ifidx = %d, nvt_if not match\n", ifidx);
                return -1;
        }

        switch (*msg_type) {
        case EVENT:
                nvt_dbg(INFO, "WID(EVENT) is handle here\n");

                if (*wid == WID_EVENT_BSS_INFO) {
                        nvt_dbg(INFO, "Scan Result\n");
                        err = nvt_scan_result(nvt_if, msg_len, data);
                } else if (*wid == WID_EVENT_SCAN_DONE) {
                        nvt_dbg(INFO, "Scan Done\n");
                        err = nvt_scan_result(nvt_if, msg_len, data);
                } else if ((*wid == WID_EVENT_ASSOC_REQ) &&
                           (nvt_if->mode == NVT_FW_AP)) {
                        nvt_dbg(INFO, "Assoc_req Info\n");
                        err = nvt_connect_status_ap(nvt_if, msg_len, data);
                } else if (*wid == WID_EVENT_CONNECT_INFO) {
                        nvt_dbg(INFO, "Connect Info\n");
                        err  = nvt_connect_result(nvt_if, msg_len, data);
                } else if (*wid == WID_EVENT_RX_PROBEREQ_FRAME) {
                        nvt_dbg(INFO, "Receive RX_PROBEREQ_FRAME\n");
                        err = nvt_rx_mgmt_probereq(nvt_if, msg_len, data);
                } else if (*wid == WID_EVENT_RX_ACTION_FRAME) {
                        nvt_dbg(INFO, "Receive RX_ACTION_FRAME\n");
                        err = nvt_rx_action_frame(nvt_if, msg_len, data);
                } else if (*wid == WID_EVENT_LISTEN_DONE) {
                        nvt_dbg(INFO, "Listen Done\n");
                        err = nvt_p2p_roc_done(nvt_if, msg_len, data, false);
                } else if ((*wid == WID_STA_JOIN_INFO) &&
                           (nvt_if->mode == NVT_FW_AP)) {
                        nvt_dbg(INFO, "Notify STA Disconnected\n");
                        sta_type = *(data + 9);
                        /* check if station leaving */
                        if (sta_type == 0) {
                                err = nvt_connect_status_ap(nvt_if,
                                        msg_len, data);
                        }
                } else if (*wid == WID_STATUS) {
                        nvt_dbg(INFO, "WID_STATUS\n");
                        if (nvt_if->mode == NVT_AP_MODE) {
                                err = nvt_enable_status_ap(nvt_if,
                                                        msg_len, data);
                        } else {
                                err = nvt_connect_status_station(nvt_if,
                                                        msg_len, data);
                        }
                } else if (*wid == WID_MIC_FAILURE_INFO) {
                        nvt_dbg(INFO, "Got MIC_fail_ind\n");
                        err = nvt_micfail_status(nvt_if, msg_len, data);
                } else if (*wid == WID_EVENT_RESUME) {
                        nvt_dbg(INFO, "Got Resume event, code=%d\n", data[2]);
                        err = nvt_resume_result(nvt_if, msg_len, data);
                } else if (*wid == WID_EVENT_REKEY_OFFLOAD) {
                        nvt_dbg(INFO, "Got Rekey_Offload Info\n");
                        err = nvt_rekeyoffload_info(nvt_if, msg_len, data);
                } else {
                        /* Do Nothing */
                }
                break;
        default:
                nvt_dbg(ERROR, "WID Coming here is weird!!!!!!\n");
                /* Do nothing */
                break;
        }
        return err;
}

static void nvt_fw_evt_worker(struct work_struct *work)
{
        struct _nvt_priv *nvt_priv;
        struct _nvt_adapter *nvt_adapter;
        struct sk_buff *skb = NULL;
        s32 err = 0;

        nvt_priv = container_of(work, struct _nvt_priv, fw_evt_work);
        nvt_adapter = container_of(nvt_priv, struct _nvt_adapter, nvt_priv);

        nvt_dbg(INFO, "%s\n", __func__);

         /* Dequeue Event is here */
        while ((skb = skb_dequeue(&nvt_priv->fw_evt_list))) {
                err = nvt_evt_handle(nvt_adapter, skb);

                if (err) {
                        nvt_dbg(ERROR, "Event handled is fail!!\n");
                        err = 0;
                }
                /* Free SKB Buffer */
                dev_kfree_skb_any(skb);
        }
}

/**
 * nvt_priv_init - init nvt priv struct parameters
 *
 * @nvt_adapter: struct _nvt_adapter
 *
 * Return: return 0 success
 */
s32 nvt_priv_init(struct _nvt_adapter *nvt_adapter)
{
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;
        struct _ba_struct_t *ba_ctxt = NULL;
        struct _tx_info *tx_info;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        struct netlink_kernel_cfg netlink_cfg;
#endif
        s32 i = 0;
        s32 j = 0;
        u8 idx;
        s32 err = 0;
        nvt_priv->pn_bcmc = 0;
        /* RX_BA Initialization */
        for (i = 0; i < ASSOC_NUM; i++) {
                for (j = 0; j < NUM_TIDS; j++) {
                        idx = i * NUM_TIDS + j;
                        ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
                        ba_ctxt->sta_index = i + 1;
                        ba_ctxt->tid = j;
                        spin_lock_init(&(ba_ctxt->ba_lock));
                        INIT_WORK(&(ba_ctxt->ba_timeout_work),
                                nvt_ba_timeout_worker);
                        nvt_priv->last_seq_ctl[idx] = 0xFFFF;
                }
                nvt_priv->last_seq_ctl[ASSOC_NUM * NUM_TIDS + i] = 0xFFFF;
        }

        /* Event/Iconfig Inititialization */
        INIT_WORK(&nvt_priv->fw_evt_work, nvt_fw_evt_worker);
        skb_queue_head_init(&nvt_priv->fw_evt_list);

        /* Tx Inititialization */
        tx_info = kzalloc(sizeof(struct _tx_info), GFP_KERNEL);
        if (!tx_info) {
                err = -ENOMEM;
                goto fail;
        }
        nvt_priv->tx_info = tx_info;
        INIT_WORK(&tx_info->tx_queue_work, nvt_tx_dequeue_worker);
        skb_queue_head_init(&tx_info->be_queue_list);
        skb_queue_head_init(&tx_info->bk_queue_list);
        skb_queue_head_init(&tx_info->vi_queue_list);
        skb_queue_head_init(&tx_info->vo_queue_list);
        spin_lock_init(&tx_info->lock);
        tx_info->nvt_adapter = nvt_adapter;
        tx_info->fcmode = 0;
        tx_info->is_netif_stop = 0;
        /*  watermark is used for multi-queue */
        #ifndef CONFIG_WIFI_TUNING_PHASE_I
        tx_info->fc_hi_watermark = 128;
        tx_info->fc_low_watermark = 64;
        #else
        tx_info->fc_hi_watermark[NV_TX_FIFO_AC_VO] = 128;
        tx_info->fc_hi_watermark[NV_TX_FIFO_AC_VI] = 64;
        tx_info->fc_hi_watermark[NV_TX_FIFO_AC_BE] = 64;
        tx_info->fc_hi_watermark[NV_TX_FIFO_AC_BK] = 64;

        //tx_info->fc_low_watermark[NV_TX_FIFO_AC_VO] = 64;         // no use
        //tx_info->fc_low_watermark[NV_TX_FIFO_AC_VI] = 0;          // no use
        //tx_info->fc_low_watermark[NV_TX_FIFO_AC_BE] = 0;          // no use
        //tx_info->fc_low_watermark[NV_TX_FIFO_AC_BK] = 0;          // no use
        #endif
        /*  max_qcnt is used for single queue */
        tx_info->max_qcnt = 256;
        /*  max_fqcnt is used for forward packet */
        tx_info->max_fqcnt = 1000;
        tx_info->is_credit_borrow = 0;
        for (i = NV_TX_FIFO_AC_VO; i < NV_TX_FIFO_COUNT; i++) {
                tx_info->fifo_credit_update[i] = 0;
        }
        for (i = NV_TX_FIFO_AC_VO; i < NV_TX_FIFO_COUNT - 1; i++) {
                tx_info->fifo_public_update[i] = 0;
        }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        netlink_cfg.input = remote_fw_input;
        netlink_cfg.groups = 0;
        nvt_priv->remote_fwlog_sock =  netlink_kernel_create(&init_net,
                                              NETLINK_USERSOCK, &netlink_cfg);
#else
        /*  initial netlink sock for remote fw log */
        nvt_priv->remote_fwlog_sock = netlink_kernel_create(&init_net,
                      NETLINK_USERSOCK, 0, remote_fw_input, NULL, THIS_MODULE);
#endif
        if (!nvt_priv->remote_fwlog_sock) {
             nvt_dbg(ERROR, "%s: create netlink sock failed\n", __func__);
             err = -ENOMEM;
             goto fail;
        }
        //remote_debug_start = 0;

        return 0;
fail:
        return err;
}

/**
 * nvt_priv_deinit - deinit nvt priv struct parameters
 *
 * @nvt_adapter: struct _nvt_adapter
 *
 * Return: return 0 success
 */
s32 nvt_priv_deinit(struct _nvt_adapter *nvt_adapter)
{
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_priv->tx_info;
        struct sk_buff *skb;

        /* Event/Iconfig De-inititialization */
        cancel_work_sync(&nvt_priv->fw_evt_work);
        while ((skb = skb_dequeue(&nvt_priv->fw_evt_list))) {
                dev_kfree_skb_any(skb);
        }

        /* Tx De-inititialization */
        if (tx_info) {
                nvt_priv->tx_info = NULL;
                kfree(tx_info);
        }

        if (nvt_priv->remote_fwlog_sock) {
                netlink_kernel_release(nvt_priv->remote_fwlog_sock);
                //sock_release(nvt_priv->remote_fwlog_sock->sk_socket);
        }

        return 0;
}

void nvt_flush_txq(struct _nvt_adapter *nvt_adapter)
{
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_priv->tx_info;
        struct sk_buff *skb = NULL;

        if (tx_info != NULL) {
                while ((skb = skb_dequeue(&tx_info->bk_queue_list))) {
                        dev_kfree_skb_any(skb);
                }
                while ((skb = skb_dequeue(&tx_info->be_queue_list))) {
                        dev_kfree_skb_any(skb);
                }
                while ((skb = skb_dequeue(&tx_info->vi_queue_list))) {
                        dev_kfree_skb_any(skb);
                }
                while ((skb = skb_dequeue(&tx_info->vo_queue_list))) {
                        dev_kfree_skb_any(skb);
                }
        }
}
