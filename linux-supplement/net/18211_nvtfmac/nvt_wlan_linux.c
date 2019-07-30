#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/etherdevice.h>
#include <linux/module.h>
#include <net/rtnetlink.h>
#include <net/ieee80211_radiotap.h>

#include "nvt_wlan_linux.h"
#include "nvt_wlan_priv.h"
#include "nvt_util_dbg.h"
#include "nvt_bus_usbif.h"
#include "nvt_bus_sdioif.h"
#include "nvt_cfg80211.h"
#include "nvt_iw.h"
#include "nvt_wlan_priv.h"
#include "nvt_icfg.h"
#include "nvt_ver.h"

static s32 nvt_if_idx_counter;

static s32 nvt_p2p_enable;
module_param_named(p2p, nvt_p2p_enable, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(p2p, "enable P2P");

u32 nvt_txmq = 1;
module_param_named(txmq, nvt_txmq, uint, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(txmq, "enable txmq");

/*AT add for testing 4way offload*/
s32 nvt_ptk_offload_enable = 0;
module_param_named(ptk_offload, nvt_ptk_offload_enable, int, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(ptk_offload, "enable ptk offload");

#define RX_RADIOTAP_PRESENT (                   \
         (1 << IEEE80211_RADIOTAP_FLAGS) |       \
         (1 << IEEE80211_RADIOTAP_RATE) |        \
         (1 << IEEE80211_RADIOTAP_DB_ANTSIGNAL) |\
         0)

#define NVT_SKB_CB(__skb) ((struct _nvt_skb_cb *)&((__skb)->cb[0]))

#ifdef CONFIG_WIFI_TUNING_PHASE_II
//#define NVTWLAN_IOCTL_TP_MONI (SIOCIWFIRSTPRIV+0xa)   //0x89F5
#define NVTWLAN_IOCTL_TP_MONI (SIOCIWFIRSTPRIV + 28)
#endif

/* Mapping 802.1d priority */
static const int nvt_tx_prio2fifo[] = {
        NV_TX_FIFO_AC_BE,
        NV_TX_FIFO_AC_BK,
        NV_TX_FIFO_AC_BK,
        NV_TX_FIFO_AC_BE,
        NV_TX_FIFO_AC_VI,
        NV_TX_FIFO_AC_VI,
        NV_TX_FIFO_AC_VO,
        NV_TX_FIFO_AC_VO
};

static bool nvt_fc_active(struct _tx_info *tx_info)
{
        return tx_info->fcmode;
}

static u32 nvt_get_total_skb_counts(struct _tx_info *tx_info, u32 fifo)
{
        u32 pkt_counts = 0;

        if (nvt_txmq == 0) {
                pkt_counts += tx_info->bk_queue_list.qlen;
                pkt_counts += tx_info->be_queue_list.qlen;
                pkt_counts += tx_info->vi_queue_list.qlen;
                pkt_counts += tx_info->vo_queue_list.qlen;
        } else {
                switch (fifo) {
                case NV_TX_FIFO_AC_BK:
                        pkt_counts += tx_info->bk_queue_list.qlen;
                        break;
                case NV_TX_FIFO_AC_BE:
                        pkt_counts += tx_info->be_queue_list.qlen;
                        break;
                case NV_TX_FIFO_AC_VI:
                        pkt_counts += tx_info->vi_queue_list.qlen;
                        break;
                case NV_TX_FIFO_AC_VO:
                        pkt_counts += tx_info->vo_queue_list.qlen;
                        break;
                default:
                        break;
                }
        }
        return pkt_counts;
}

#ifndef CONFIG_WIFI_TUNING_PHASE_I
static bool nvt_fc_block(struct _tx_info *tx_info,
                bool is_check_stop, struct _nvt_if *nvt_if, u32 fifo)
{
        u32 pkt_counts = nvt_get_total_skb_counts(tx_info, fifo);
        u32 fc_high_watermark = tx_info->fc_hi_watermark;
        u32 fc_low_watermark = tx_info->fc_low_watermark;
        struct netdev_queue *txq;
        /* check stop when enqueue */
        if (is_check_stop) {
                if (pkt_counts >= fc_high_watermark) {
                        nvt_dbg(TX, "%s, pkt > highwatermark\n", __func__);
                        if (nvt_txmq == 0) {
                                if (tx_info->is_netif_stop == 0) {
                                        /* do nothing now */
                                        /* netif_stop_queue(nvt_if->ndev); */
                                        /* tx_info->is_netif_stop = 1; */
                                }
                        } else {
                                txq = netdev_get_tx_queue(nvt_if->ndev, fifo);
                                if (!netif_tx_queue_stopped(txq)) {
                                        netif_tx_stop_queue(txq);
                                        nvt_dbg(TX,
                                                "stop fifo=%d queue\n", fifo);
                                }
                        }
                        return true;
                } else {
                        return false;
                }
        /* check wakeup when dequeue */
        } else {
                if (pkt_counts < fc_low_watermark) {
                        if (nvt_txmq == 0) {
                                if (tx_info->is_netif_stop == 1) {
                                        /* do nothing now */
                                        /* netif_wake_queue(nvt_if->ndev); */
                                        /* tx_info->is_netif_stop = 0; */
                                }
                        } else {
                                txq = netdev_get_tx_queue(nvt_if->ndev, fifo);
                                if (netif_tx_queue_stopped(txq)) {
                                        netif_tx_wake_queue(txq);
                                        nvt_dbg(TX,
                                                "wake fifo=%d queue\n", fifo);
                                }
                        }
                        return true;
                } else {
                        return false;
                }
        }
}
#else   /* CONFIG_WIFI_TUNING_PHASE_I == 1 */
static bool nvt_fc_drop_check(struct _tx_info *tx_info, u32 fifo, bool eap)
{

        u32 pkt_counts = nvt_get_total_skb_counts(tx_info, fifo);
        #if 0  /* debug */
        u32 pkt_counts_1 = tx_info->fifo_enqpkt[fifo];

        nvt_dbg(ERROR, "(%d,%d,%d)\n",
                pkt_counts, pkt_counts_1, tx_info->fc_hi_watermark[fifo]);
        #endif

        if (eap)
            return true;

        if (pkt_counts >= tx_info->fc_hi_watermark[fifo]) {   /* queue full */
            //tx_info->stats[fifo].drop++;
            return true;
        } else
            return false;

}

static void nvt_tx_block_check(struct _tx_info *tx_info,
                        struct _nvt_if *nvt_if, u32 fifo)
{
        u32 pkt_counts = nvt_get_total_skb_counts(tx_info, fifo);
        //u32 fc_high_watermark = tx_info->fc_hi_watermark;    // 128
        //u32 fc_low_watermark = tx_info->fc_low_watermark;    // 64
        struct netdev_queue *txq;
        //int i=0;

        /* check stop when enqueue */
        if (pkt_counts >= tx_info->fc_hi_watermark[fifo]) {
             nvt_dbg(TX, "%s, pkt > highwatermark\n", __func__);
             if (nvt_txmq == 0) {
                  if (tx_info->is_netif_stop == 0) {
                      /* do nothing now */
                      /* netif_stop_queue(nvt_if->ndev); */
                      /* tx_info->is_netif_stop = 1; */
                  }
             } else {
                  txq = netdev_get_tx_queue(nvt_if->ndev, fifo);
                  if (!netif_tx_queue_stopped(txq)) {
                       netif_tx_stop_queue(txq);
                  }
             }
        } else if (pkt_counts < tx_info->fc_hi_watermark[fifo]) {
             nvt_dbg(TX, "%s, pkt < highwatermark\n", __func__);
             if (nvt_txmq == 0) {
                  if (tx_info->is_netif_stop == 1) {
                      /* do nothing now */
                      /* netif_wake_queue(nvt_if->ndev); */
                      /* tx_info->is_netif_stop = 0; */
                  }
             } else {
                  txq = netdev_get_tx_queue(nvt_if->ndev, fifo);
                  if (netif_tx_queue_stopped(txq)) {
                       netif_tx_wake_queue(txq);
                  }
             }
        }
}
#endif

static int nvt_tx_enqueue(struct _tx_info *tx_info, u32 fifo,
                struct sk_buff *skb, struct _nvt_if *nvt_if, bool eap)
{
        int err = 0;
        /* Do not handle forward packet here */
        u8 forward_magic_pat = NVT_SKB_CB(skb)->forward_magic_pat;
        if (forward_magic_pat != 0x7f) {
                /* Avoid stop BE queue, because of special frame, e.g. eapol */
                #ifndef CONFIG_WIFI_TUNING_PHASE_I
                if (nvt_txmq == 0 || fifo == NV_TX_FIFO_AC_BE) {
                #else
                if (nvt_txmq == 0 || eap) {
                #endif
                        /* do nothing now */
                        /* err = nvt_fc_block(tx_info, true, nvt_if, fifo); */
                } else {
                        #ifndef CONFIG_WIFI_TUNING_PHASE_I
                        err = nvt_fc_block(tx_info, true, nvt_if, fifo);
                        #else
                        if (fifo != NV_TX_FIFO_AC_VO)
                            err = nvt_fc_drop_check(tx_info, fifo, eap);
                        #endif
                }
        }
        /* If special frame, enqueue it in vo_list_head unconditionally */
        if (eap == 1) {
                skb_queue_head(&tx_info->vo_queue_list, skb);
                goto done;
        }
        if (err == true)
                return -EINVAL;
        switch (fifo) {
        case NV_TX_FIFO_AC_BK:
                #ifndef CONFIG_WIFI_TUNING_PHASE_I
                if (forward_magic_pat != 0x7f) {
                        if (tx_info->bk_queue_list.qlen > tx_info->max_qcnt) {
                                err = true;
                                goto done;
                        }
                } else {
                        if (tx_info->bk_queue_list.qlen > tx_info->max_fqcnt) {
                                err = true;
                                goto done;
                        }
                }
                #endif
                skb_queue_tail(&tx_info->bk_queue_list, skb);
                break;
        case NV_TX_FIFO_AC_BE:
                #ifndef CONFIG_WIFI_TUNING_PHASE_I
                if (forward_magic_pat != 0x7f) {
                        if (tx_info->be_queue_list.qlen > tx_info->max_qcnt) {
                                err = true;
                                goto done;
                        }
                } else {
                        if (tx_info->be_queue_list.qlen > tx_info->max_fqcnt) {
                                err = true;
                                goto done;
                        }
                }
                #endif
                skb_queue_tail(&tx_info->be_queue_list, skb);
                break;
        case NV_TX_FIFO_AC_VI:
                #ifndef CONFIG_WIFI_TUNING_PHASE_I
                if (forward_magic_pat != 0x7f) {
                        if (tx_info->vi_queue_list.qlen > tx_info->max_qcnt) {
                                err = true;
                                goto done;
                        }
                } else {
                        if (tx_info->vi_queue_list.qlen > tx_info->max_fqcnt) {
                                err = true;
                                goto done;
                        }
                }
                #endif
                skb_queue_tail(&tx_info->vi_queue_list, skb);
                break;
        case NV_TX_FIFO_AC_VO:
                #ifndef CONFIG_WIFI_TUNING_PHASE_I
                if (forward_magic_pat != 0x7f) {
                        if (tx_info->vo_queue_list.qlen > tx_info->max_qcnt) {
                                err = true;
                                goto done;
                        }
                } else {
                        if (tx_info->vo_queue_list.qlen > tx_info->max_fqcnt) {
                                err = true;
                                goto done;
                        }
                }
                #endif
                skb_queue_tail(&tx_info->vo_queue_list, skb);
                break;
        default:
                err = -EINVAL;
                /* Do nothing */
                break;
        }
done:
        return err;
}

/**
 * nvt_shedule_tx_dequeue - schedule to dequeue tx skb or not
 * @tx_info: tx structrue
 *
 * if flow control is enable
 *     1. if we have the credit or public credit, and then we call
 *        schedule work to dequeue tx skb
 *     2. if we don't have credit or public credit, and we do not call schedule
 *        work to dequeue tx skb
 * if flow control is disable
 *        we always call schedule work once we have tx skb
 *
 * Return: NULL
 */
void nvt_shedule_tx_dequeue(struct _tx_info *tx_info)
{
        u8 fifo_deq = 0;
        nvt_dbg(TX, "%s\n", __func__);
        nvt_dbg(TX, "fifo_credit_map=0x%x, tx_queue=0x%x\n",
                tx_info->fifo_credit_map, tx_info->fifo_txqueue_map);
        /* If some packets in a queue and that queue has credit */
        if (tx_info->fifo_credit_map & tx_info->fifo_txqueue_map) {
                fifo_deq = 1;
        /* If some packet in a queue and public has credit */
        } else if ((tx_info->fifo_txqueue_map > 0) &&
                   (tx_info->fifo_credit_map & (1 << 4))) {
                fifo_deq = 1;
        } else {
                fifo_deq = 0;
        }
        /* fifo_deq = 1 or some packets in queue without flow control */
        if ((fifo_deq == 1) ||
            (!nvt_fc_active(tx_info) && tx_info->fifo_txqueue_map)) {
                schedule_work(&tx_info->tx_queue_work);
        }
}

static struct sk_buff *nvt_tx_dequeue(struct _tx_info *tx_info, u32 fifo)
{
        struct sk_buff *skb = NULL;

        switch (fifo) {
        case NV_TX_FIFO_AC_BK:
                skb = skb_dequeue(&tx_info->bk_queue_list);
                break;
        case NV_TX_FIFO_AC_BE:
                skb = skb_dequeue(&tx_info->be_queue_list);
                break;
        case NV_TX_FIFO_AC_VI:
                skb = skb_dequeue(&tx_info->vi_queue_list);
                break;
        case NV_TX_FIFO_AC_VO:
                skb = skb_dequeue(&tx_info->vo_queue_list);
                break;
        default:
                /* Do nothing */
                break;
        }
        return skb;
}

static void nvt_tx_hosthdrpush(struct sk_buff *skb,
        u32 fifo, bool is_public, s32 credit_lender)
{
        struct _nvt_hwinfo_tx *nvt_hwinfo_tx;
        struct ethhdr *eh = (struct ethhdr *)(skb->data);
        //bool pae = eh->h_proto == htons(ETH_P_PAE);
        bool pae = 0;
        u8 q_num = 0;
        s32 q_lender_num = 0;
        skb_push(skb, NVT_TX_HOSTHEADER);

        nvt_hwinfo_tx = (struct _nvt_hwinfo_tx *)(skb->data);
        memset(nvt_hwinfo_tx, 0x0, sizeof(struct _nvt_hwinfo_tx));

        nvt_hwinfo_tx->word1.pktlen = cpu_to_be16(skb->len - NVT_TX_HOSTHEADER);

        if ((fifo & 0x07) == 0x00) {
                q_num = 1; /* VO */
        } else if ((fifo & 0x07) == 0x01) {
                q_num = 2; /* VI */
        } else if ((fifo & 0x07) == 0x02) {
                q_num = 3; /* BE */
        } else {
                q_num = 4; /* BK */
        }
        nvt_hwinfo_tx->word0 = (q_num & 0x07) | ((is_public & 0x01) << 29);

        /*get and fill the q_lender in host header*/
        if (credit_lender != -1) {
                if ((credit_lender & 0x07) == 0x00) {
                        q_lender_num = 0; /* VO */
                } else if ((credit_lender & 0x07) == 0x01) {
                        q_lender_num = 1; /* VI */
                } else if ((credit_lender & 0x07) == 0x02) {
                        q_lender_num = 2; /* BE */
                } else {
                        q_lender_num = 3; /* BK */
                }
                nvt_hwinfo_tx->word0 |= (0x00c00000 & (q_lender_num<<22));
                /*set the bit24 to indicate this skb is borrowed by other AC*/
                nvt_hwinfo_tx->word0 |= (1 << 24);
                nvt_dbg(TX, "q_lender_num: %d\n", q_lender_num + 1);
        }

        /* disable control bit of word0 */
        nvt_hwinfo_tx->word0 = nvt_hwinfo_tx->word0 & 0x7fffffff;
        /* enable ethernet bit of word0 */
        nvt_hwinfo_tx->word0 = nvt_hwinfo_tx->word0 | 0x00080000;

        if (eh->h_proto == htons(ETH_P_PAE) ||
                eh->h_proto == htons(ETH_P_WAPI)) {
                pae = 1;
        }
        if (pae) {
                nvt_dbg(TX, "This is TX EAPOL\n");
                nvt_hwinfo_tx->word0 = nvt_hwinfo_tx->word0 | 0x08000000;
        }

        nvt_hwinfo_tx->word0 = cpu_to_be32(nvt_hwinfo_tx->word0);
        nvt_hwinfo_tx->word1.offset = 8;
}

s32 nvt_get_credit_lender(struct _tx_info *tx_info, u8 fifo_borrower)
{
        s8 fifo;

        /*Check traffic before get credit lender*/
        for (fifo = NV_TX_FIFO_AC_BK; fifo > -1; fifo--) {
                if (fifo != fifo_borrower) {
                        if (tx_info->fifo_enqpkt[fifo])
                                return -1;
                }

        }

        for (fifo = NV_TX_FIFO_AC_BK; fifo > -1; fifo--) {
                /*check if overflow*/
                if (fifo > NV_TX_FIFO_AC_BK)
                        break;
                /*check if potential lender has no enqueue packets
                 * and has credit*/
                if (fifo != fifo_borrower && (!tx_info->fifo_enqpkt[fifo]) &&
                                tx_info->fifo_credit[fifo]) {

                        nvt_dbg(TX, "Credit lender:%d\n", fifo);
                        return fifo;
                }
        }
        return -1;
}

bool is_remain_lender_credits(struct _tx_info *tx_info, u8 fifo_borrower)
{
        u8 vo_remains = tx_info->fifo_credit[NV_TX_FIFO_AC_VO];
        u8 vi_remains = tx_info->fifo_credit[NV_TX_FIFO_AC_VI];
        u8 be_remains = tx_info->fifo_credit[NV_TX_FIFO_AC_BK];
        u8 remain_credits = vo_remains + vi_remains + be_remains;
        if (tx_info->fifo_credit[fifo_borrower] == 0 && remain_credits > 0) {
                return 1;
        } else {
            return 0;
        }
}

/* nvt_tx_dequeue_by_borrow_credit should be used cautionly
 * Make sure there is tx_info lock protection outside this
 * function
 */
void nvt_tx_dequeue_by_borrow_credit(struct _tx_info *tx_info,
                u8 fifo_borrower)
{
        struct sk_buff *skb;
        struct _nvt_adapter *nvt_adapter;
        s32 fifo_lender;
        s32 err = 0;
        nvt_adapter = tx_info->nvt_adapter;

        //spin_lock_irqsave(&tx_info->lock, flags);
        while (is_remain_lender_credits(tx_info, fifo_borrower)) {
                /*find the lender*/
                fifo_lender = nvt_get_credit_lender(tx_info,
                                fifo_borrower);
                if (fifo_lender > -1) {
                        skb = nvt_tx_dequeue(tx_info, fifo_borrower);
                        nvt_dbg(TX, "credit borrow: %d\n", fifo_borrower);
                        if (skb == NULL) {
                                break;
                        }
                        if (tx_info->fifo_enqpkt[fifo_borrower] == 0) {
                               nvt_dbg(ERROR,
                                     "fifo borrower has no enqued pkts\n");
                               break;
                        } else {
                               tx_info->fifo_enqpkt[fifo_borrower]--;
                               #if 1   /* tx statistic */
                               tx_info->stats[fifo_borrower].out++;
                               tx_info->stats[fifo_borrower].out_sz += skb->len;
                               #endif
                        }
                        if (tx_info->fifo_enqpkt[fifo_borrower] == 0) {
                                tx_info->fifo_txqueue_map &=
                                        ~(1 << fifo_borrower);
                        }

                        tx_info->fifo_credit[fifo_lender]--;
                        if (tx_info->fifo_credit[fifo_lender] == 0) {
                                tx_info->fifo_credit_map &= ~(1 << fifo_lender);
                        }

                        nvt_tx_hosthdrpush(skb, fifo_borrower, 0, fifo_lender);
                        tx_info->nvt_tx_test.tx_deque_cnt++;
                        tx_info->nvt_tx_test.end_time = jiffies;
                        //spin_unlock_irqrestore(&tx_info->lock, flags);
                        if ((tx_info->nvt_tx_test.tx_test_mode &
                                                NVT_TX_DEQUE_POINT) != 0) {
                                err = -1;
                        } else {
                                err = nvt_bus_txdata(nvt_adapter->nvt_bus, skb);
                        }
                        if (err < 0) {
                                tx_info->nvt_tx_test.tx_deque_drop_cnt++;
                                dev_kfree_skb(skb);
                        }
                } else {
                        break;
                }
        }
        nvt_dbg(TX, "End of nvt_tx_dequeue_by_borrow: %d\n", fifo_borrower);
}

/**
 * nvt_tx_dequeue_worker - dequeue tx skb and send it to usb
 * @work: work queue structrue
 *
 * we always dequeue tx queue from high priority to low priority
 * if flow control is enable
 *     1. First, we check own credit. If we ran out of the own credit,
 *        we check public credit
 *     2. if we don't have own credit and public credit, and we do not
 *        dequeue tx skb which is enqueued.
 * if flow control is disable
 *        we always dequeue skb once we have tx skb which is enqueued
 *
 * Return: NULL
 */
void nvt_tx_dequeue_worker(struct work_struct *work)
{
        struct _tx_info *tx_info;
        struct _nvt_adapter *nvt_adapter;
        struct _nvt_if *nvt_if = NULL;
        struct sk_buff *skb;
        u32 fifo;
        ulong flags;
        s32 err = 0;
        u8 ifidx = 0;

        tx_info = container_of(work, struct _tx_info, tx_queue_work);
        nvt_adapter = tx_info->nvt_adapter;
        /* Get nvt_if pointer */
        nvt_if = nvt_get_if_by_index(nvt_adapter, ifidx);
        if (!nvt_if) {
                nvt_dbg(TX, "%s, nvt_if is NULL, return\n", __func__);
                return;
        }

        nvt_dbg(TX, "%s, tx_info->bus_flow_blocked=%d\n",
                __func__, tx_info->bus_flow_blocked);

        /* Strictly Priority Dequeue Tx form VO -> VI -> BE -> BK. */
        nvt_dbg(TX, "Strictly Priority Dequeue Algorithm\n");
        spin_lock_irqsave(&tx_info->lock, flags);
        //for (fifo = NV_TX_FIFO_AC_VO; fifo <= 3 &&
        //        !tx_info->bus_flow_blocked; fifo++) {
        for (fifo = NV_TX_FIFO_AC_VO; fifo <= 3; fifo++) {
                nvt_dbg(TX, "fifo=%d is prepare Dequeueing\n", fifo);
                /* Handle Tx packet without flow control here */
                if (!nvt_fc_active(tx_info)) {
                        while ((skb = nvt_tx_dequeue(tx_info, fifo))) {
                                nvt_dbg(TX, "fifo=%d, fifo_enqpkt=%d\n",
                                        fifo, tx_info->fifo_enqpkt[fifo]);
                                /* prevent enqpkt overflow */
                                if (tx_info->fifo_enqpkt[fifo] == 0) {
                                        nvt_dbg(TX,
                                                "fifo_enqpkt=0, keep zero\n");
                                } else {
                                        tx_info->fifo_enqpkt[fifo]--;
                                        #if 1  /* tx statistic */
                                        tx_info->stats[fifo].out++;
                                        tx_info->stats[fifo].out_sz += skb->len;
                                        #endif
                                }
                                if (tx_info->fifo_enqpkt[fifo] == 0) {
                                        tx_info->fifo_txqueue_map &=
                                                ~(1 << fifo);
                                }
                                #ifndef CONFIG_WIFI_TUNING_PHASE_I
                                nvt_fc_block(tx_info, false, nvt_if, fifo);
                                #else
                                if (fifo == NV_TX_FIFO_AC_VO)
                                     nvt_tx_block_check(tx_info, nvt_if, fifo);
                                #endif
                                /* Insert Tx Host Header */
                                nvt_tx_hosthdrpush(skb, fifo, 0, -1);
                                tx_info->nvt_tx_test.tx_deque_cnt++;
                                tx_info->nvt_tx_test.end_time = jiffies;
                                spin_unlock_irqrestore(&tx_info->lock, flags);
                                if ((tx_info->nvt_tx_test.tx_test_mode &        
                                                NVT_TX_DEQUE_POINT) != 0) {
                                        err = -1;
                                } else {
                                        err = nvt_bus_txdata(nvt_adapter->nvt_bus,
                                                skb);
                                }
                                spin_lock_irqsave(&tx_info->lock, flags);
                                if (err < 0) {
                                        tx_info->nvt_tx_test.tx_deque_drop_cnt++;
                                        dev_kfree_skb(skb);
                                }
                                //if (tx_info->bus_flow_blocked) {
                                //        nvt_dbg(TX, "bus_flow_blocked on\n");
                                //        break;
                                //}
                        }
                        continue;
                }
                /* Handle Tx packet with flow control here */
                /* Using available fifo credit */
                while (tx_info->fifo_credit[fifo]) {
                        skb = nvt_tx_dequeue(tx_info, fifo);
                        if (skb == NULL) {
                                break;
                        }
                        nvt_dbg(TX, "fifo=%d, fifo_enqpkt=%d, fifo_credit=%d\n",
                                fifo,
                                tx_info->fifo_enqpkt[fifo],
                                tx_info->fifo_credit[fifo]);
                        /* prevent enqpkt overflow */
                        if (tx_info->fifo_enqpkt[fifo] == 0) {
                                nvt_dbg(TX, "fifo_enqpkt=0, keep zero\n");
                        } else {
                                tx_info->fifo_enqpkt[fifo]--;
                        }
                        if (tx_info->fifo_enqpkt[fifo] == 0) {
                                tx_info->fifo_txqueue_map &= ~(1 << fifo);
                        }
                        tx_info->fifo_credit[fifo]--;
                        #if 1  /* tx statistic */
                        tx_info->stats[fifo].out++;
                        tx_info->stats[fifo].out_sz += skb->len;
                        #endif
                        if (tx_info->fifo_credit[fifo] == 0) {
                                tx_info->fifo_credit_map &= ~(1 << fifo);
                        }
                        #ifndef CONFIG_WIFI_TUNING_PHASE_I
                        nvt_fc_block(tx_info, false, nvt_if, fifo);
                        #else
                        if (fifo == NV_TX_FIFO_AC_VO)
                            nvt_tx_block_check(tx_info, nvt_if, fifo);
                        #endif
                        /* Insert Tx Host Header */
                        nvt_tx_hosthdrpush(skb, fifo, 0, -1);
                        tx_info->nvt_tx_test.tx_deque_cnt++;
                        tx_info->nvt_tx_test.end_time = jiffies;
                        spin_unlock_irqrestore(&tx_info->lock, flags);
                        if ((tx_info->nvt_tx_test.tx_test_mode & NVT_TX_DEQUE_POINT) != 0) {
                                err = -1;
                        } else {
                                err = nvt_bus_txdata(nvt_adapter->nvt_bus, skb);
                        }
                        spin_lock_irqsave(&tx_info->lock, flags);
                        if (err < 0) {
                                tx_info->nvt_tx_test.tx_deque_drop_cnt++;
                                dev_kfree_skb(skb);
                        }
                        //if (tx_info->bus_flow_blocked) {
                        //        nvt_dbg(TX, "bus_flow_blocked is ongoing\n");
                        //        break;
                        //}
                }
                /* Handle Tx packet with flow control here */
                /* Using public credit */
                while ((tx_info->fifo_credit[fifo] == 0) &&
                       (tx_info->fifo_credit[NV_TX_FIFO_PUBLIC] != 0) &&
                       (!tx_info->bus_flow_blocked)) {
                        if ((100 * (tx_info->fifo_public_inuse[fifo])) >
                                (PUBLIC_WEIGHT *
                                tx_info->fifo_max_credit[NV_TX_FIFO_PUBLIC])) {
                                nvt_dbg(TX, "over public credit\n");
                                break;
                        }
                        skb = nvt_tx_dequeue(tx_info, fifo);
                        if (skb == NULL) {
                                break;
                        }
                        nvt_dbg(TX, "fifo=%d,fifo_enqpkt=%d,public_credit=%d\n",
                                fifo,
                                tx_info->fifo_enqpkt[fifo],
                                tx_info->fifo_credit[NV_TX_FIFO_PUBLIC]);
                        /* prevent enqpkt overflow */
                        if (tx_info->fifo_enqpkt[fifo] == 0) {
                                nvt_dbg(TX, "fifo_enqpkt=0, keep zero\n");
                        } else {
                                tx_info->fifo_enqpkt[fifo]--;
                                #if 1  /* tx statistic */
                                tx_info->stats[fifo].out++;
                                tx_info->stats[fifo].out_sz += skb->len;
                                #endif
                        }
                        if (tx_info->fifo_enqpkt[fifo] == 0) {
                                tx_info->fifo_txqueue_map &= ~(1 << fifo);
                        }
                        tx_info->fifo_credit[NV_TX_FIFO_PUBLIC]--;
                        tx_info->fifo_public_inuse[fifo]++;
                        nvt_dbg(TX, "fifo_public_inuse=%d\n",
                                tx_info->fifo_public_inuse[fifo]);
                        if (tx_info->fifo_credit[NV_TX_FIFO_PUBLIC] == 0) {
                                tx_info->fifo_credit_map &=
                                        ~(1 << NV_TX_FIFO_PUBLIC);
                        }
                        #ifndef CONFIG_WIFI_TUNING_PHASE_I
                        nvt_fc_block(tx_info, false, nvt_if, fifo);
                        #else
                        if (fifo == NV_TX_FIFO_AC_VO)
                            nvt_tx_block_check(tx_info, nvt_if, fifo);
                        #endif
                        /* Insert Tx Host Header */
                        nvt_tx_hosthdrpush(skb, fifo, 1, -1);
                        tx_info->nvt_tx_test.tx_deque_cnt++;
                        tx_info->nvt_tx_test.end_time = jiffies;
                        spin_unlock_irqrestore(&tx_info->lock, flags);
                        if ((tx_info->nvt_tx_test.tx_test_mode &
                                                NVT_TX_DEQUE_POINT) != 0) {
                                err = -1;
                        } else {
                                err = nvt_bus_txdata(nvt_adapter->nvt_bus, skb);
                        }
                        spin_lock_irqsave(&tx_info->lock, flags);
                        if (err < 0) {
                                tx_info->nvt_tx_test.tx_deque_drop_cnt++;
                                dev_kfree_skb(skb);
                        }
                        //if (tx_info->bus_flow_blocked) {
                        //        nvt_dbg(TX, "bus_flow_blocked is on\n");
                        //        break;
                        //}
                }

                if (fifo == NV_TX_FIFO_AC_BE &&
                                tx_info->is_credit_borrow) {
                        nvt_tx_dequeue_by_borrow_credit(tx_info, fifo);
                }
        }
        spin_unlock_irqrestore(&tx_info->lock, flags);
}

static void nvt_process_tx_frame(struct _nvt_if *nvt_if,
        struct sk_buff *skb)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        struct ethhdr *eh = (struct ethhdr *)(skb->data);
        //bool eap = eh->h_proto == htons(ETH_P_PAE);
        //u8 *tmp = (u8 *)(skb->data);
        bool eap = 0;
        u32 fifo;
        ulong flags;
        u32 i = 0;

        nvt_dbg(TX, "%s\n", __func__);

        tx_info->nvt_tx_test.tx_total_cnt++;
        if ((tx_info->nvt_tx_test.tx_test_mode & NVT_TX_START_POINT) != 0) {
                dev_kfree_skb(skb);
                return;
        }

        /* match magic pattern */
        if (NVT_SKB_CB(skb)->forward_magic_pat != 0x7f) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 14, 0)
                skb->priority = cfg80211_classify8021d(skb);
#else
                skb->priority = cfg80211_classify8021d(skb, NULL);
#endif
        } else {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 14, 0)
                nvt_dbg(TX, "Forward packet tos_prio=%d, wifi_prio=%d\n",
                        cfg80211_classify8021d(skb), skb->priority);
#else
                nvt_dbg(TX, "Forward packet tos_prio=%d, wifi_prio=%d\n",
                        cfg80211_classify8021d(skb, NULL), skb->priority);
#endif
        }
        fifo = nvt_tx_prio2fifo[skb->priority];

        /* special frame force to highest priority queue */
        if (eh->h_proto == htons(ETH_P_PAE) ||
                eh->h_proto == htons(ETH_P_WAPI)) {
                eap = 1;
                fifo = NV_TX_FIFO_AC_VO;
        }

        if (eap) {
                nvt_dbg(TX, "This is Tx EAPOL\n");
                for (i = 0; i < skb->len; i++)
                        //printk("%02x:", *(tmp + i));
                nvt_dbg(TX, "\n");
                if (nvt_if->mode == NVT_FW_STA) {
                        atomic_inc(&nvt_if->eapol_cnt);
                }
                nvt_dbg(TX, "pend_8021x_cnt = %d\n",
                        atomic_read(&nvt_if->eapol_cnt));
        }

        nvt_dbg(TX, "fifo=%d skb->priority=%d\n", fifo, skb->priority);
        nvt_dbg(TX, "skb->queue_mapping=%d\n", skb->queue_mapping);

        if (tx_info->nvt_tx_test.start_time == 0) {
                tx_info->nvt_tx_test.start_time = jiffies;
        }

        spin_lock_irqsave(&tx_info->lock, flags);
        /* Enqueue Tx packet */
        if (nvt_tx_enqueue(tx_info, fifo, skb, nvt_if, eap)) {
                tx_info->nvt_tx_test.rx_enque_drop_cnt++;
                nvt_dbg(TX, "tx_enqueue fail, drop fifo=%d SKB\n", fifo);
                #if 1  /* tx statistic */
                tx_info->stats[fifo].drop++;
                #endif
                dev_kfree_skb(skb);
        } else {
                tx_info->nvt_tx_test.tx_enque_cnt++;
                tx_info->fifo_enqpkt[fifo]++;
                tx_info->fifo_txqueue_map |= 1 << fifo;
                #if 1  /* tx statistic */
                tx_info->stats[fifo].en++;
                #endif
                nvt_dbg(TX, "fifo_enqpkt=%d, fifo_txqueue=0x%x, credit=%d\n",
                        tx_info->fifo_enqpkt[fifo], tx_info->fifo_txqueue_map,
                        tx_info->fifo_credit[fifo]);
                #ifdef CONFIG_WIFI_TUNING_PHASE_I
                if (fifo == NV_TX_FIFO_AC_VO)
                    nvt_tx_block_check(tx_info, nvt_if, fifo);
                #endif
                nvt_shedule_tx_dequeue(tx_info);
        }
        spin_unlock_irqrestore(&tx_info->lock, flags);
}

static int nvt_wdev_open(struct net_device *ndev)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        nvt_dbg(INFO, "%s: do net dev open\n", __func__);

        if (nvt_bus->state != NVT_BUS_STATE_UP) {
                nvt_dbg(ERROR, "%s: bus is not ready!\n", __func__);
        }

        ndev->features &= ~NETIF_F_IP_CSUM;

        set_bit(NVT_IF_ENABLED, &nvt_if->state_flags);

        if (nvt_txmq == 0) {
                netif_start_queue(ndev);
        } else {
                netif_tx_start_all_queues(ndev);
        }
        return 0;
}

static s32 nvt_wdev_stop(struct net_device *ndev)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_cfg80211 *nvt_cfg80211 = nvt_if->nvt_adapter->nvt_cfg80211;

        nvt_dbg(INFO, "%s: do net dev stop\n", __func__);

        nvt_abort_scanning(nvt_cfg80211);

        clear_bit(NVT_IF_ENABLED, &nvt_if->state_flags);

        if (nvt_txmq == 0) {
                netif_stop_queue(ndev);
        } else {
                netif_tx_stop_all_queues(ndev);
        }

        return 0;
}

struct net_device_stats *nvt_wdev_get_stats(struct net_device *ndev)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);

        //nvt_dbg(INFO, "%s: get stats from if:%d, mode=%d\n", __func__,
        //        nvt_if->fw_if_idx, nvt_if->mode);

        return &nvt_if->net_stats;
}

#ifdef CONFIG_WIFI_TUNING_PHASE_II
static s32 wlan_io_wifi_tuning(struct _nvt_if *nvt_if, void *req_msg)
{
        s32 ret = 0;

        u8   found, rsp_idx;
        u8   mac_addr[6], rsp[18], rsp_sz = 18;
        //u8   clr_after_get;
        u8   start;
        //s32  qid;
        u32  wid, wid_set_val, wid_set_len, interval;
        //unsigned int mac_addr_tmp;
        struct _sta_sts_in_ap *sta_sts_in_ap;
        struct iwreq *wrq = (struct	iwreq *)req_msg;

        struct _nvt_priv *nvt_priv = &(nvt_if->nvt_adapter->nvt_priv);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        struct _wifi_tuning_cmd *wifi_tuning_cmd;
        wifi_tuning_cmd = (struct _wifi_tuning_cmd *)wrq->u.data.pointer;

        if (wifi_tuning_cmd->type == WIFI_TUNING_SET) {

            wid = wid_set_val = wid_set_len = 0;
            if (wifi_tuning_cmd->cmd_code.set == WIFI_TUNING_SET_INTERVAL) {
                if (wifi_tuning_cmd->buf_len != 4) {
                    ret = -1;
                    goto exit;
                }
                interval = *((u32 *)(wifi_tuning_cmd->buf));
                if (interval < 1 || interval > 5000) {
                    ret = -1;
                    goto exit;
                }
                wid = WID_TP_MIN_INTERVAL;
                wid_set_val = (interval & 0xffffffff);
                wid_set_len = 4;
                nvt_dbg(ERROR, "%s: interval:%d\n", __func__, interval);
            } else if (wifi_tuning_cmd->cmd_code.set == WIFI_TUNING_SET_START) {
                if (wifi_tuning_cmd->buf_len != 1) {
                    ret = -1;
                    goto exit;
                }
                start = *((u8 *)(wifi_tuning_cmd->buf));
                if (start != 1 && start != 0) {
                    ret = -1;
                    goto exit;
                }
                nvt_dbg(ERROR, "%s: start:%d\n", __func__, start);

                wid = WID_TP_MONITOR;
                wid_set_val = (start & 0xff);
                wid_set_len = 1;
            } else {
                ret = -1;
                goto exit;
            }

            // Send wid to MAC
            if (wid || wid_set_val || wid_set_len) {
                ret = nvt_icfg_lock(nvt_bus);
                if (ret < 0) {
                    goto exit;
                }
                ret = nvt_icfg_reset(nvt_bus);
                if (ret < 0) {
                    nvt_icfg_unlock(nvt_bus);
                    goto exit;
                }
                ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET, wid,
                                   (u8 *)&wid_set_val, wid_set_len);
                if (ret < 0) {
                    nvt_icfg_unlock(nvt_bus);
                    goto exit;
                }
                ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
                if (ret < 0) {
                    nvt_icfg_unlock(nvt_bus);
                    goto exit;
                }

                nvt_icfg_unlock(nvt_bus);
            }
        } else if (wifi_tuning_cmd->type == WIFI_TUNING_GET) {
            if (wifi_tuning_cmd->cmd_code.get == WIFI_TUNING_GET_STATUS) {
                if (wifi_tuning_cmd->buf_len != ETH_ALEN + 1) {
                    ret = -1;
                    goto exit;
                }

                memcpy(mac_addr, wifi_tuning_cmd->buf, ETH_ALEN);

                #if 0  /* debug */
                memcpy((void *)&clr_after_get,
                       (void *)((u8 *)(wifi_tuning_cmd->buf) + ETH_ALEN), 1);

                nvt_dbg(ERROR, "%s: MAC %02x:%02x:%02x:%02x:%02x:%02x ",
                        __func__, mac_addr[0], mac_addr[1], mac_addr[2],
                        mac_addr[3], mac_addr[4], mac_addr[5]);
                nvt_dbg(ERROR, "clr:%d\n", clr_after_get);
                #endif

                found = 0;
                list_for_each_entry(sta_sts_in_ap, &nvt_priv->sta_list_in_ap,
                                  list)
                {
                    if (!memcmp(&sta_sts_in_ap->mac_addr, mac_addr, ETH_ALEN)) {
                        found = 1;
                        break;
                    }
                }

                if (found == 1) {

                    nvt_dbg(ERROR,
                           "%s: found MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
                           __func__, mac_addr[0], mac_addr[1], mac_addr[2],
                           mac_addr[3], mac_addr[4], mac_addr[5]);

                    rsp_idx = 0;
                    memcpy(&rsp[rsp_idx], sta_sts_in_ap->mac_addr, ETH_ALEN);

                    rsp_idx += ETH_ALEN;
                    #if 0
                    memcpy(&rsp[rsp_idx], &sta_sts_in_ap->phyrate,
                        sizeof(sta_sts_in_ap->phyrate));
                    #else
                    memcpy(&rsp[rsp_idx], &sta_sts_in_ap->t_put,
                        sizeof(sta_sts_in_ap->t_put));
                    #endif

                    rsp_idx += sizeof(sta_sts_in_ap->phyrate);
                    memcpy(&rsp[rsp_idx], &sta_sts_in_ap->enq_in_vo_be,
                        sizeof(sta_sts_in_ap->enq_in_vo_be));

                    rsp_idx += sizeof(sta_sts_in_ap->enq_in_vo_be);
                    #if 0
                    memcpy(&rsp[rsp_idx], &sta_sts_in_ap->drop_in_be,
                        sizeof(sta_sts_in_ap->drop_in_be));
                    #else
                    memcpy(&rsp[rsp_idx], &sta_sts_in_ap->mac_vodropframe,
                        sizeof(sta_sts_in_ap->mac_vodropframe));
                    #endif

                } else {
                    nvt_dbg(ERROR,
                        "%s: No info for MAC:%02x:%02x:%02x:%02x:%02x:%02x\n",
                        __func__, mac_addr[0], mac_addr[1], mac_addr[2],
                        mac_addr[3], mac_addr[4], mac_addr[5]);
                    memset(rsp, 0, rsp_sz);
                }

                memcpy(wifi_tuning_cmd->buf, rsp, rsp_sz);
                wifi_tuning_cmd->buf_len = rsp_sz;
            } else {

            }
        }
exit:
    return ret;

}
#endif

static s32 nvt_wdev_ioctl(struct net_device *ndev, struct ifreq *ifr, s32 cmd)
{
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        //struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        #endif

        int result = 0;

        nvt_dbg(ERROR, "%s: cmd:%x\n", __func__, cmd);

        switch (cmd) {
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        case NVTWLAN_IOCTL_TP_MONI:
             result = wlan_io_wifi_tuning(nvt_if, ifr);
             break;
        #endif
        default:
             result = -1;
             break;
        }

        return result;

}

static int nvt_tx_xmit(struct sk_buff *skb,
                        struct net_device *ndev)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_cfg80211 *cfg = nvt_adapter->nvt_cfg80211;
        struct ethhdr *eh;

        /* Check Interface is OK */
        if (!(ndev->flags & IFF_UP)) {
                nvt_dbg(TX, "Interface is not ready for TX\n");
                if (nvt_txmq == 0) {
                        netif_stop_queue(ndev);
                } else {
                        netif_tx_stop_all_queues(ndev);
                }
                dev_kfree_skb(skb);
                goto done;
        }

        /* Make sure there's enough room for any header */
        if (skb_headroom(skb) < nvt_if->hdrlen) {
                struct sk_buff *skb2;

                nvt_dbg(TX, "insufficient headroom, %d\n", skb_headroom(skb));
                skb2 = skb_realloc_headroom(skb, ndev->hard_header_len);
                dev_kfree_skb(skb);
                skb = skb2;
                if (skb == NULL) {
                        nvt_dbg(TX, "skb_realloc_headroom failed\n");
                        goto done;
                }
        }

        /* Check length for ethernet packet */
        if (skb->len < sizeof(*eh)) {
                dev_kfree_skb(skb);
                goto done;
        }

        /* Check STA mode can send data */
        if (!test_bit(NVT_IF_CONNECTED, &nvt_if->state_flags)) {
                nvt_dbg(TX, "device is not in connected state\n");
                dev_kfree_skb(skb);
                goto done;
        } else {
                /* Check Scanning or not */
                if (test_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                        nvt_dbg(TX, "Scanning is process now\n");
                        dev_kfree_skb(skb);
                        goto done;
                }
                nvt_process_tx_frame(nvt_if, skb);
        }
        nvt_if->net_stats.tx_packets++;
        nvt_if->net_stats.tx_bytes += skb->len;
done:
        return NETDEV_TX_OK;
}

static int nvt_wdev_xmit_forward(struct sk_buff *skb,
        struct net_device *ndev)
{
        nvt_dbg(TX, "%s\n", __func__);
        memset(skb->cb, 0, sizeof(skb->cb));
        //set the forward magic pattern
        NVT_SKB_CB(skb)->forward_magic_pat = 0x7f;
        return nvt_tx_xmit(skb, ndev);
}

static int nvt_wdev_start_xmit(struct sk_buff *skb,
        struct net_device *ndev)
{
        nvt_dbg(TX, "%s\n", __func__);
        //Clear skb->cb
        memset(skb->cb, 0, sizeof(skb->cb));
        return nvt_tx_xmit(skb, ndev);
}

static s32 nvt_wdev_set_mac_addr(struct net_device *ndev, void *addr)
{
        s32 ret;
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct sockaddr *saddr = addr;

        memcpy(nvt_adapter->mac_addr, saddr->sa_data, ETH_ALEN);

        ret = nvt_set_macaddress_to_fw(nvt_adapter);
        if (ret < 0) {
                nvt_dbg(INFO, "%s: set MAC address fail\n", __func__);
                return -EPERM;
        }

        memcpy(ndev->dev_addr, nvt_if->nvt_adapter->mac_addr, ETH_ALEN);

        return 0;
}

/* Not supported yet */
#if 0
static void nvt_wdev_set_rx_mode(struct net_device *ndev)
{
        //nash:TODO
        nvt_dbg(ERROR, "%s: not supported yet\n", __func__);
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
static u16 nvt_wdev_select_wmm_queue(struct net_device *dev,
                                     struct sk_buff *skb,
                                     void *accesl_priv,
                                     select_queue_fallback_t fallback)
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3, 12, 0)
static u16 nvt_wdev_select_wmm_queue(struct net_device *dev,
                                     struct sk_buff *skb,
                                     void *accel_priv)
#else
static u16 nvt_wdev_select_wmm_queue(struct net_device *dev,
        struct sk_buff *skb)
#endif
{
        if (nvt_txmq == 0) {
                return 0;
        } else {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 14, 0)
                skb->priority = cfg80211_classify8021d(skb);
#else
                skb->priority = cfg80211_classify8021d(skb, NULL);
#endif
                return nvt_tx_prio2fifo[skb->priority];
        }
}

static const struct net_device_ops nvt_wdev_ops = {
        .ndo_open = nvt_wdev_open,
        .ndo_stop = nvt_wdev_stop,
        .ndo_get_stats = nvt_wdev_get_stats,
        .ndo_do_ioctl = nvt_wdev_ioctl,
        .ndo_start_xmit = nvt_wdev_start_xmit,
        .ndo_set_mac_address = nvt_wdev_set_mac_addr,
        //20150716 nash: not support multicast list filtering
        //.ndo_set_rx_mode = nvt_wdev_set_rx_mode
        .ndo_select_queue = nvt_wdev_select_wmm_queue,
};


/**
 * nvt_deaggr_security_amsdu - de-aggregate amsdu packet
 * @pkt     : skb buffer list
 * @info    : nvt rx hostheader
 * @nvt_if  : interface structure
 * @pn      : pn_start_ptr
 * this function will process skb
 *      we do de-aggregate it, and send them to kernel one by one.
 *
 * Return: NULL
 */

void nvt_deaggr_security_amsdu(struct sk_buff *pkt,
        struct _nvt_hwinfo_rx *info, struct _nvt_if *nvt_if,
        u8 *pn_start_ptr, enum _cipher_t ct)
{
        u8 amsdu_flags = info->word4.amsdu_flags;
        //u8 buff_num_curr_mpdu = info->word4.buff_num_curr_mpdu;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        //check whether this buffer is the starting amsdu buffer
        if ((amsdu_flags & 0x2) && (amsdu_flags & 0x1)) {
                if (nvt_check_sec_rx_mpdu(nvt_adapter, ct, pkt,
                        pn_start_ptr)) {
                        dev_kfree_skb_any(pkt);
                } else {
                        nvt_deaggr_amsdu(pkt, info, nvt_if);
                }
        } else {
                nvt_deaggr_amsdu(pkt, info, nvt_if);
        }
}
/**
 * nvt_deaggr_amsdu - de-aggregate amsdu packet
 * @pkt: skb buffer list
 * @info: nvt rx hostheader
 * @nvt_if: interface structure
 *
 * this function will process skb
 *      we do de-aggregate it, and send them to kernel one by one.
 *
 * Return: NULL
 */
void nvt_deaggr_amsdu(struct sk_buff *pkt,
        struct _nvt_hwinfo_rx *info, struct _nvt_if *nvt_if)
{
        struct sk_buff *buf;
        struct _nvt_hwinfo_rx info_record;
        int num_msdu = 0;
        int cnt = 0;
        int buf_len = 0;
        u16 msdu_len = 0;
        u8 msdu_padlen = 0;
        u8 submsdu_gap = 16;
        u8 *da;
        u8 *sa;
        u8 *payload;
        u8 rfc1042_header[] = { 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00 };

        nvt_dbg(REORDER, "%s\n", __func__);

        /* De-aggregate AMSDU skb */
        num_msdu = info->word5.num_amsdu;
        nvt_dbg(REORDER, "AMSDU frame, num_msdu=%d\n", num_msdu);
        memcpy(&info_record, info, sizeof(struct _nvt_hwinfo_rx));
        info_record.word1.offset = 32;

        for (cnt = num_msdu; cnt > 0; cnt--) {
                if (cnt == num_msdu) {
                        payload = pkt->data + info->word1.offset;
                        skb_pull(pkt, info->word1.offset);
                } else {
                        payload = pkt->data;
                }
                nvt_dbg(REORDER, "rx skb_len=%d\n", pkt->len);
                da = payload;
                sa = payload + ETH_ALEN;
                msdu_len = (*(payload + 12) << 8) + (*(payload + 13));

                if ((14 + msdu_len) & 0x3) {
                        msdu_padlen = 4 - ((14 + msdu_len) & 0x3);
                } else {
                        msdu_padlen = 0;
                }
                skb_pull(pkt, ETH_HLEN + sizeof(rfc1042_header));
                memcpy(skb_push(pkt, ETH_ALEN), sa, ETH_ALEN);
                memcpy(skb_push(pkt, ETH_ALEN), da, ETH_ALEN);
                memcpy(skb_push(pkt, sizeof(struct _nvt_hwinfo_rx)),
                        &info_record, sizeof(struct _nvt_hwinfo_rx));

                if (cnt != 1) {
                        buf_len = pkt->len - msdu_len - ETH_HLEN -
                                sizeof(struct _nvt_hwinfo_rx) + 8;

                        if (buf_len <= 0) {
                                nvt_dbg(ERROR, "Remain len error\n");
                                nvt_dbg(ERROR, "skb_len=%d\n", pkt->len);
                                nvt_dbg(ERROR, "msdu_len=%d\n", msdu_len);
                                break;
                        }

                        buf = dev_alloc_skb(buf_len);
                        skb_split(pkt, buf, msdu_len + ETH_HLEN +
                                sizeof(struct _nvt_hwinfo_rx) - 8);
                        nvt_dbg(REORDER,
                                "part msdu[%d],skb_len=%d,msdu_len=%d\n",
                                cnt, pkt->len, msdu_len);
                        nvt_netif_rx(nvt_if, pkt);
                        /* pull out sub-msdu gap */
                        skb_pull(buf, submsdu_gap + msdu_padlen);
                        pkt = buf;
                        nvt_dbg(REORDER, "remain skb_len=%d\n", pkt->len);
                } else {
                        skb_trim(pkt, sizeof(struct _nvt_hwinfo_rx) +
                                ETH_HLEN + msdu_len - 8);
                        nvt_dbg(REORDER,
                                "last msdu[%d],skb_len=%d,msdu_len=%d\n",
                                cnt, pkt->len, msdu_len);
                        nvt_netif_rx(nvt_if, pkt);
                }
        }
}

static u8 nvt_frame_get_retry(u8* header)
{
    return (header[1] & 0x08) >> 3;
}

/*handle the broadcast packet or MPDU datas*/
void nvt_handle_bcmc_mpdu_skb(struct _nvt_hwinfo_rx *info,
        struct sk_buff *skb, struct _nvt_if *nvt_if,
        u8 ba_idx)
{
        u8 is_retry;
        u8 is_BC;
        u8 is_MC;
        u8 is_QoS;
        u8 tid;
        u8 sta_index;
        u8 *pn_start_ptr;
        u16 seq_num;
        u16 last_seq;
        enum _cipher_t ct;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;

        is_BC = (ntohl(info->word0) & 0x10000000) >> 28;
        is_MC = (ntohl(info->word0) & 0x20000000) >> 29;
        is_QoS = (ntohl(info->word0) & 0x00000004) >> 2;
        sta_index = info->word4.sta_idx;
        tid = (u8)(ntohl(info->word0) & 0x000000F0) >> 4;
        ct = (enum _cipher_t)((ntohl(info->word7.value) & 0x0000f000) >> 12);
        /* Duplicate detection */
        if ((is_BC == 1) || (is_MC == 1)) {
                /* do nothing */
        } else {
                seq_num = (u16)(ntohl(info->word7.value) & 0x00000fff);
                is_retry = nvt_frame_get_retry(skb->data + NVT_RX_HOSTHEADER);
                if (is_QoS == 1) {
                        last_seq = nvt_adapter->nvt_priv.last_seq_ctl[ba_idx];
                        if (is_retry && (last_seq == seq_num)) {
                                nvt_dbg(REORDER, "duplicate QoS seq[%d]\n",
                                                                seq_num);
                                dev_kfree_skb_any(skb);
                                return;
                        }
                        nvt_adapter->nvt_priv.last_seq_ctl[ba_idx] = seq_num;
                } else {
                        last_seq = nvt_adapter->nvt_priv.last_seq_ctl[
                                        ASSOC_NUM * NUM_TIDS + (sta_index - 1)];
                        if (is_retry && (last_seq == seq_num)) {
                                nvt_dbg(REORDER, "duplicate non-QoS seq[%d]\n",
                                                                seq_num);
                                dev_kfree_skb_any(skb);
                                return;
                        }
                        nvt_adapter->
                                nvt_priv.last_seq_ctl[ASSOC_NUM * NUM_TIDS +
                                                (sta_index - 1)] = seq_num;
                }
        }
        if ((ct != TKIP) && (ct != CCMP)) {
                /* check this skb is AMSDU or not */
                if ((ntohl(info->word0) & 0x08000000) == 0) {
                        nvt_netif_rx(nvt_if, skb);
                } else {
                        nvt_deaggr_amsdu(skb, info, nvt_if);
                }
        } else {
                if (((is_BC == 1) || (is_MC == 1)) &&
                        (nvt_if->mode == NVT_FW_STA)) {
                        //nvt_netif_rx(nvt_if, skb);
                        /* sanity check for bcmc pn if ptk offload is enable */
                        if (nvt_ptk_offload_enable && !nvt_adapter->nvt_priv.pn_bcmc) {
                                nvt_dbg(ERROR, "BCMC PN is NULL!!\n");
                                /* allocate 6 bytes bcmc pn */
                                nvt_adapter->nvt_priv.pn_bcmc = kzalloc(6, GFP_KERNEL);
                                if (nvt_adapter->nvt_priv.pn_bcmc == NULL) {
                                        nvt_dbg(ERROR, "BC PN allocate fail!!\n");
                                        dev_kfree_skb_any(skb);
                                        return;
                                }
                                memset(nvt_adapter->nvt_priv.pn_bcmc, 0, 6);
                        }
                        pn_start_ptr = nvt_adapter->nvt_priv.pn_bcmc;
                } else {

                        pn_start_ptr = nvt_adapter->
                                nvt_priv.pn_unicast[ba_idx];
                        if (pn_start_ptr == NULL) {
                                nvt_adapter->nvt_priv.pn_unicast[ba_idx] =
                                    kzalloc(6, GFP_ATOMIC);
                                 pn_start_ptr = nvt_adapter->
                                         nvt_priv.pn_unicast[ba_idx];
                        }
                }
                //check this skb is AMSDU or not
                if ((ntohl(info->word0) & 0x08000000) == 0) {
                        if (nvt_check_sec_rx_mpdu(nvt_adapter, ct,
                                skb, pn_start_ptr)) {
                                dev_kfree_skb_any(skb);
                        } else {
                                nvt_netif_rx(nvt_if, skb);
                        }
                } else {
                        nvt_deaggr_security_amsdu(skb, info, nvt_if,
                                pn_start_ptr, ct);
                }
        }
}

/**
 * nvt_process_rx_list - process rx_list
 * @ndev: net device structure
 * @skb_list: rx skb list
 *
 * this function will process skb one by one through rx_skb_list.
 *
 * Return: NULL
 */
void nvt_process_rx_list(struct _nvt_bus *nvt_bus,
        struct sk_buff_head *skb_list)
{
        struct _nvt_adapter *nvt_adapter = nvt_bus->nvt_adapter;
        struct _nvt_if *nvt_if = NULL;
        struct sk_buff *skb;
        struct sk_buff *pnext;
        struct _nvt_hwinfo_rx *info;
        ulong flags;
        u8 ifidx = 0;
        s32 ba_index = 0;

        struct _ba_struct_t *ba_ctxt;
        nvt_dbg(REORDER, "%s\n", __func__);

        if (skb_queue_empty(skb_list)) {
                //20150921 nash:
                //nvt_dbg(ERROR, "skb_list is empty\n");
                nvt_dbg(REORDER, "skb_list is empty\n");
                return;
        }

        skb_queue_walk_safe(skb_list, skb, pnext) {
                skb_unlink(skb, skb_list);

                if (nvt_is_priv_pkt(nvt_bus, skb)) {
                        nvt_dbg(REORDER, "This is Iconfig/Event packet\n");
                        if (nvt_process_priv_pkt(nvt_bus, skb)) {
                                nvt_dbg(ERROR, "Iconfig/Event handle error!\n");
                                continue;
                        } else {
                                continue;
                        }
                } else {
                        info = (struct _nvt_hwinfo_rx *)skb->data;

                        /* Get nvt_if pointer */
                        nvt_if = nvt_get_if_by_index(nvt_adapter, ifidx);
                        nvt_is_trigger_pkt(nvt_bus, skb);
                        if (!nvt_if || !nvt_if->ndev) {
                                dev_kfree_skb_any(skb);
                                continue;
                        }

                        if (nvt_if->wdev.iftype == NL80211_IFTYPE_MONITOR) {
                                nvt_netif_rx(nvt_if, skb);
                                continue;
                        }

                        /*Sanity check for TID and sta index*/
                        ba_index = get_ba_index_by_hosthdr(info, skb);
                        if (ba_index < 0) {
                                continue;
                        }
                        if (nvt_ba_process(nvt_if, info, skb, ba_index)
                                == false) {
                                ba_ctxt = &(nvt_adapter->
                                        nvt_priv.ba_rx[ba_index]);
                                spin_lock_irqsave(&ba_ctxt->ba_lock, flags);
                                nvt_handle_bcmc_mpdu_skb(info, skb,
                                                nvt_if, ba_index);
                                spin_unlock_irqrestore(&ba_ctxt->ba_lock,
                                                flags);
                        } else {
                                continue;
                        }

                }
        }
}


void nvt_is_trigger_pkt(struct _nvt_bus *nvt_bus, struct sk_buff *skb)
{
        struct _nvt_hwinfo_rx *info;
        u8 flag;

        nvt_dbg(REORDER, "%s\n", __func__);

        info = (struct _nvt_hwinfo_rx *)skb->data;
        /* look at the second byte in word 0       */
        /* BIT8 in that byte is magic-frame flag   */
        /* BIT9 in that byte is pattern-frame flag */
        flag = ((ntohl(info->word0)) >> 8) & 0x03;
        if (flag == 1) {
                nvt_dbg(CFG80211, "%s: this is magic frame\n",
                                __func__);
        } else if (flag == 2) {
                nvt_dbg(CFG80211, "%s: this is pattern frame\n",
                                __func__);
        } else {
                nvt_dbg(CFG80211, "%s: neither magic nor pattern frame\n",
                                __func__);
        }
}


/**
 * nvt_netif_rx - dequeue tx skb and send it to usb
 * @nvt_if: interface structrue
 * @skb: rx skb buffer
 *
 * this function will process skb
 *     1. it may be forwarded to tx(bridge)
 *     2. it may be sent to kernel(higher layer)
 *
 * Return: NULL
 */
void nvt_netif_rx(struct _nvt_if *nvt_if, struct sk_buff *skb)
{
        struct _nvt_hwinfo_rx *info = NULL;
        struct sk_buff *skb2 = NULL;
        struct nvt_rx_radiotap_hdr radiotap_hdr;
        struct nvt_rx_radiotap_hdr *pradiotap_hdr;

        skb->dev = nvt_if->ndev;

        nvt_dbg(REORDER, "%s\n", __func__);

        info = (struct _nvt_hwinfo_rx *)skb->data;
        if ((ntohl(info->word0) & DESTI_MASK) == TO_WIFI) {
                skb->priority = (u8)(ntohl(info->word0) & 0x000000F0) >> 4;
        }
        /* pull out Rx Host Header */
        skb_pull(skb, info->word1.offset);
        nvt_dbg(REORDER, "skb->protocol=0x%x, skb->len=%d\n",
                ntohs(skb->protocol), skb->len);

        nvt_if->net_stats.rx_bytes += skb->len;
        nvt_if->net_stats.rx_packets++;
        if (nvt_if->mode == NVT_FW_AP) {
                if ((ntohl(info->word0) & DESTI_MASK) == TO_WIFI) {
                        nvt_dbg(REORDER, "Forward Rx packet to Tx Directly\n");
                        skb->protocol = cpu_to_be16(ETH_P_802_3);
                        if (in_interrupt()) {
                                nvt_wdev_xmit_forward(skb, skb->dev);
                        } else {
                                nvt_wdev_xmit_forward(skb, skb->dev);
                                /* dev_queue_xmit(skb); */
                        }
                } else {
                        if (is_multicast_ether_addr(skb->data)) {
                                skb2 = skb_copy(skb, GFP_ATOMIC);
                        }
                        if (skb2 == NULL) {
                                if (is_multicast_ether_addr(skb->data)) {
                                        nvt_dbg(REORDER,
                                                "SKB2 NULL,can't forward tx\n");
                                }
                        } else {
                                nvt_dbg(REORDER,
                                        "Forward Rx to Tx & to kernel\n");
                                skb2->dev = nvt_if->ndev;
                                skb2->protocol = cpu_to_be16(ETH_P_802_3);
                                if (in_interrupt()) {
                                        nvt_wdev_start_xmit(skb2, skb2->dev);
                                } else {
                                        nvt_wdev_start_xmit(skb2, skb2->dev);
                                        /* dev_queue_xmit(skb2); */
                                }
                        }
                        skb->protocol = eth_type_trans(skb, skb->dev);

                        nvt_dbg(REORDER, "skb->protocol=0x%x\n",
                                ntohs(skb->protocol));
                        if (in_interrupt()) {
                                netif_rx(skb);
                        } else {
                                netif_rx_ni(skb);
                        }
                }
        } else {
                if (nvt_if->wdev.iftype == NL80211_IFTYPE_MONITOR) {
                        u8 rx_radio_len = sizeof(struct nvt_rx_radiotap_hdr);
                        memset(&radiotap_hdr, 0, sizeof(radiotap_hdr));
                        radiotap_hdr.hdr.it_len =
                                cpu_to_le16(sizeof(struct nvt_rx_radiotap_hdr));
                        radiotap_hdr.hdr.it_present =
                                cpu_to_le32(RX_RADIOTAP_PRESENT);
                        radiotap_hdr.rate = 2;
                        radiotap_hdr.antsignal = 10;
                        if (skb_headroom(skb) < rx_radio_len) {
                                nvt_dbg(ERROR, "monitor, size too small\n");
                                dev_kfree_skb(skb);
                                return;
                        }

                        pradiotap_hdr = (void *)skb_push(skb, rx_radio_len);
                        if (pradiotap_hdr != NULL) {
                                memcpy(pradiotap_hdr, &radiotap_hdr,
                                        rx_radio_len);
                        }
                        skb->ip_summed = CHECKSUM_NONE;
                        skb->protocol = eth_type_trans(skb, skb->dev);
                        netif_rx(skb);
                        return;
                }

                skb->protocol = eth_type_trans(skb, skb->dev);

                nvt_dbg(REORDER, "skb->protocol=0x%x,\n", ntohs(skb->protocol));

                if (in_interrupt()) {
                        netif_rx(skb);
                } else {
                        netif_rx_ni(skb);
                }
        }
}

/**
 * nvt_get_if_by_index - get the nvt_if structure
 * @nvt_adapter: adapter structrue
 * @ifidx: interface index
 *
 * this function will get the nvt_if according to
 *     1. nvt_adapter pointer
 *     2. interface index which is received from FW.
 *
 * Return: nvt_if
 */
struct _nvt_if *nvt_get_if_by_index(struct _nvt_adapter *nvt_adapter,
        u8 ifidx)
{
        struct _nvt_if *nvt_if, *nvt_found = NULL;
        ulong flags;

        //if (WARN_ON(ifidx > (nvt_adapter->if_max - 1)))
        //      return NULL;

        //20150806 nash: NULL check
        if (nvt_adapter == NULL) {
                nvt_dbg(ERROR, "%s: nvt_adapter is NULL\n", __func__);
                return NULL;
        }
        /* Locking if_list */
        spin_lock_irqsave(&nvt_adapter->list_lock, flags);
        list_for_each_entry(nvt_if, &nvt_adapter->if_list, list) {
                if (nvt_if->fw_if_idx == ifidx) {
                        nvt_found = nvt_if;
                        break;
                }
        }
        spin_unlock_irqrestore(&nvt_adapter->list_lock, flags);

        return nvt_found;

}

struct _nvt_if *nvt_create_if(struct _nvt_adapter *nvt_adapter, u8 *name,
        bool is_p2p)
{
        struct _nvt_if *nvt_if = NULL;
        struct net_device *ndev;

        if (nvt_p2p_enable && is_p2p) {
                /* this is P2P_DEVICE interface */
                nvt_dbg(INFO, "%s: allocate p2p interface\n", __func__);
                nvt_if = kzalloc(sizeof(struct _nvt_if), GFP_KERNEL);
                if (nvt_if == NULL) {
                        return ERR_PTR(-ENOMEM);
                }
        } else {
                nvt_dbg(INFO, "%s: allocate netdev interface\n", __func__);
                /* Allocate netdev, including space for private structure */
                if (nvt_txmq == 0) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 17, 0)
                        ndev = alloc_netdev(sizeof(struct _nvt_if),
                                name, ether_setup);
#else
                        ndev = alloc_netdev(sizeof(struct _nvt_if),
                                name, NET_NAME_UNKNOWN, ether_setup);
#endif
                } else {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 17, 0)
                        ndev = alloc_netdev_mqs(sizeof(struct _nvt_if), name,
                                ether_setup, IEEE80211_NUM_ACS, 1);
#else
                        ndev = alloc_netdev_mqs(sizeof(struct _nvt_if), name,
                                    NET_NAME_UNKNOWN, ether_setup,
                                    IEEE80211_NUM_ACS, 1);
#endif
                }
                if (!ndev) {
                        nvt_dbg(ERROR, "%s: alloc_netdev fail\n", __func__);
                        return ERR_PTR(-ENOMEM);
                }
                nvt_if = netdev_priv(ndev);
                nvt_if->ndev = ndev;
        }

        nvt_if->nvt_adapter = nvt_adapter;
        nvt_if->fw_if_idx = nvt_if_idx_counter;
        nvt_if->rts_val = RTS_INIT_VAL;
        nvt_if->frag_val = FRAG_INIT_VAL;
        nvt_if->retry_short_val = SHORT_RETRY_INIT_VAL;
        nvt_if->retry_long_val = LONG_RETRY_INIT_VAL;
        nvt_if_idx_counter++;
        nvt_if->is_p2p = is_p2p;
        nvt_if->hdrlen = NVT_TX_HOSTHEADER;

        INIT_LIST_HEAD(&nvt_if->list);
        memset(&nvt_if->nvt_wconf, 0x00, sizeof(struct _nvt_wlan_conf));
        memset(&nvt_if->nvt_wconf_ap, 0x00, sizeof(struct _nvt_wlan_conf_ap));
        nvt_if->nvt_wconf_ap.acs_channel = 0xFF;
        nvt_if->nvt_wconf_ap.acs_weight[0] = 0xFF;
        init_waitqueue_head(&nvt_if->addkey_wait);
        init_waitqueue_head(&nvt_if->disconnect_wait);
        mutex_init(&nvt_if->disconnect_lock);
        atomic_set(&nvt_if->eapol_cnt, 0);
        atomic_set(&nvt_if->disconnect_cnt, 0);

        list_add_tail(&nvt_if->list, &nvt_adapter->if_list);

        nvt_dbg(INFO, "%s: add if:%d\n", __func__, nvt_if->fw_if_idx);

        return nvt_if;
}

struct _nvt_if *nvt_vif_first(struct _nvt_adapter *nvt_adapter)
{
        struct _nvt_if *vif;
        spin_lock_bh(&nvt_adapter->list_lock);
        if (list_empty(&nvt_adapter->if_list)) {
                spin_unlock_bh(&nvt_adapter->list_lock);
                return NULL;
        }

        vif = list_first_entry(&nvt_adapter->if_list, struct _nvt_if, list);

        spin_unlock_bh(&nvt_adapter->list_lock);

        return vif;

}

void nvt_del_if(struct _nvt_if *nvt_if)
{
        if (nvt_if == NULL) {
                return;
        }

        nvt_dbg(INFO, "%s:delete if_idx=%d\n", __func__, nvt_if->fw_if_idx);
        if (nvt_if->ndev) {
                if (nvt_if->is_p2p) {
                        if (nvt_txmq == 0) {
                                netif_stop_queue(nvt_if->ndev);
                        } else {
                                netif_tx_stop_all_queues(nvt_if->ndev);
                        }
                } else {
                        rtnl_lock();
                        //if usb was disconnected, we should not send urb
                        //through usb sub-system
                        //nash:TODO: 1. do link down
                        //nash:TODO: 2. abort scan
                        //nvt_abort_scanning(
                        //nvt_if->nvt_adapter->nvt_cfg80211);

                        if (nvt_txmq == 0) {
                                netif_stop_queue(nvt_if->ndev);
                        } else {
                                netif_tx_stop_all_queues(nvt_if->ndev);
                        }
                        rtnl_unlock();
                }

                //cancel_work_sync(&nvt_if->setfwdownload_work);
                if (nvt_if->ndev->netdev_ops != NULL) {
                        unregister_netdev(nvt_if->ndev);
                        nvt_cfg80211_deinit(nvt_if->nvt_adapter);
                        nvt_priv_deinit(nvt_if->nvt_adapter);
                        free_netdev(nvt_if->ndev);
                } else {
                        nvt_cfg80211_deinit(nvt_if->nvt_adapter);
                        nvt_priv_deinit(nvt_if->nvt_adapter);
                }


        } else {
                kfree(nvt_if);
        }

}

/**
 * nvt_del_all_if - delete all interface
 * @nvt_adapter: struct _nvt_adapter
 *
 *
 *
 * Return: 0:success, a negative value:fail
 */
s32 nvt_del_all_if(struct _nvt_adapter *nvt_adapter)
{
        struct _nvt_if *nvt_if;
        struct _nvt_if *tmp;
        list_for_each_entry_safe(nvt_if, tmp, &nvt_adapter->if_list, list) {
                list_del(&nvt_if->list);
                nvt_del_if(nvt_if);
        }
        //20150709 nash:
        nvt_if_idx_counter = 0;

        return 0;
}

static void nvt_work_set_fw_download(struct work_struct *work)
{
        s32 ret;
        struct _nvt_if *nvt_if;
        nvt_if = container_of(work, struct _nvt_if, setfwdownload_work);
        //20151223 nash: coverity#48928
        ret = nvt_fw_change_opmode(nvt_if, nvt_if->fw_download_type);
        if (ret) {
                nvt_dbg(ERROR, "%s change opmode fail\n", __func__);
        }
}

static s32 nvt_get_macaddr_from_fw(struct _nvt_adapter *nvt_adapter)
{
        s32 ret;
        s32 pkt_len = 0;
        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;
        struct _nvt_diag_req  *diag_req = NULL;
        struct _nvt_diag_resp *diag_resp = NULL;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(ERROR, "%s: kzalloc for req is failed\n", __func__);
                ret = -1;
                goto fail;
        }

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(ERROR, "%s: kzalloc for resp is failed\n", __func__);
                ret = -1;
                goto fail;
        }

        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_GET_MAC_ADDR_CMD, 0, 0,
                NULL, diag_req, &pkt_len);

        ret = nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus,
                (u8 *)diag_req, pkt_len);
        if (ret < 0) {
                goto fail;
        }

        ret = nvt_bus->nvt_wdev_bus_ops.rx_ctrl(nvt_bus, (u8 *)diag_resp,
                sizeof(struct _nvt_diag_resp));
        if (ret < 0) {
                goto fail;
        }

        memcpy(nvt_adapter->mac_addr, diag_resp->sel.get_mac_addr.mac_addr,
                sizeof(nvt_adapter->mac_addr));

 fail:

        kfree(diag_req);
        kfree(diag_resp);

        return ret;
}

/**
 * nvt_set_macaddress_to_fw - set MAC address to firmware
 * @nvt_adapter: _nvt_adapter structure
 *
 *
 * Return: 0:success, -1:fail
 */
s32 nvt_set_macaddress_to_fw(struct _nvt_adapter *nvt_adapter)
{
        s32 ret = 0;
        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;

        ret = nvt_icfg_lock(nvt_bus);
        if (ret < 0) {
                return -EPERM;
        }

        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
                return -EPERM;
        }

        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_MAC_ADDR,
                nvt_adapter->mac_addr, ETH_ALEN);
        if (ret < 0) {
                return -EPERM;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
        if (ret < 0) {
                return -EPERM;
        }

        nvt_icfg_unlock(nvt_bus);

        return 0;
}

static s32 nvt_netdev_register(struct _nvt_if *nvt_if)
{
        s32 ret;
        struct net_device *ndev;
        struct _nvt_cfg80211 *nvt_cfg80211 = nvt_if->nvt_adapter->nvt_cfg80211;

        /* setup wireless_dev */
        nvt_if->wdev.wiphy = nvt_cfg80211->wiphy;
        nvt_if->wdev.iftype = NL80211_IFTYPE_STATION;

        ndev = nvt_if->ndev;
        ndev->netdev_ops = &nvt_wdev_ops;

        //20150710 nash:
        ndev->hard_header_len += nvt_if->hdrlen;

        /* assign ieee80211_ptr before registering */
        ndev->ieee80211_ptr = &nvt_if->wdev;

        SET_NETDEV_DEV(ndev, wiphy_dev(nvt_cfg80211->wiphy));

        ret = nvt_get_macaddr_from_fw(nvt_if->nvt_adapter);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: mac addr retrieve fail\n", __func__);
                goto fail;
        }
        memcpy(ndev->dev_addr, nvt_if->nvt_adapter->mac_addr, ETH_ALEN);

        ret = nvt_iw_register_handler(ndev);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: iw_handler registration fail!\n", __func__);
                goto fail;
        }

        INIT_WORK(&nvt_if->setfwdownload_work, nvt_work_set_fw_download);

        ret = register_netdev(ndev);
        if (ret != 0) {
                nvt_dbg(ERROR, "%s: couldn't register the net device\n",
                        __func__);
                goto fail;
        } else {
                nvt_dbg(INFO, "%s:register net device sucessfully!!\n",
                        __func__);
        }

        nvt_dbg(INFO, "%s: netdev name=%s\n", __func__, ndev->name);

        return 0;

fail:
        ndev->netdev_ops = NULL;
        return -EBADE;

}

/**
 * nvt_register_to_system - do related initialization and setup
 * @dev: struct device
 *
 *
 *
 * Return:
 */
s32 nvt_register_to_system(struct device *dev)
{
        s32 ret;
        struct _nvt_adapter *nvt_adapter;
        struct _nvt_bus *bus;
        struct _nvt_if *nvt_if = NULL;
        struct _nvt_if *p2p_if = NULL;

        nvt_dbg(INFO, "%s:\n", __func__);

        nvt_adapter = kzalloc(sizeof(struct _nvt_adapter), GFP_ATOMIC);
        if (!nvt_adapter) {
                return -ENOMEM;
        }
        bus = (struct _nvt_bus *)dev_get_drvdata(dev);
        if (!bus) {
                //20160104 nash: coverity#49515
                kfree(nvt_adapter);
                return -EPERM;
        }
        bus->nvt_adapter = nvt_adapter;
        nvt_adapter->nvt_bus = bus;
        INIT_LIST_HEAD(&nvt_adapter->if_list);
        spin_lock_init(&nvt_adapter->list_lock);
        nvt_if = nvt_create_if(nvt_adapter, "wlan%d", false);
        if (IS_ERR(nvt_if)) {
                return PTR_ERR(nvt_if);
        }
        //20150709 nash: record mode
        nvt_if->mode = NVT_FW_DEFAULT_FW_MODE;

        //20151110 nash:
        nvt_if->sleep_mode = NVT_SLEEP_MODE_DISABLE;

        nvt_dbg(INFO, "%s: create if:%d ok\n", __func__, nvt_if->fw_if_idx);

        if (nvt_p2p_enable) {
                p2p_if = nvt_create_if(nvt_adapter, "p2p%d", true);
        }

        //20160601 nash: coverity #61618
        #if 0
        if (IS_ERR(p2p_if)) {
                p2p_if = NULL;
        }
        #endif

        nvt_adapter->nvt_bus->state = NVT_BUS_STATE_UP;

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                goto fail;
        }

        ret = nvt_cfg80211_init(nvt_adapter);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: cfg80211 register fail\n", __func__);
                goto fail;
        }

        ret = nvt_priv_init(nvt_adapter);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: nvt_priv_init fail\n", __func__);
                goto fail;
        }

        ret = nvt_netdev_register(nvt_if);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: net attach fail!!\n", __func__);
                goto fail;
        }
fail:
        //nash:TODO:check
        if (ret < 0) {
                nvt_del_all_if(nvt_adapter);
                #if 0
                if (nvt_if) {
                        nvt_del_if(nvt_if);
                        list_del(&nvt_if->list);
                }
                if (p2p_if) {
                        nvt_del_if(p2p_if);
                        list_del(&p2p_if->list);
                }
                if (nvt_adapter->nvt_cfg80211) {
                        nvt_cfg80211_deinit(nvt_if->nvt_adapter);
                        nvt_priv_deinit(nvt_if->nvt_adapter);
                }
                #endif
                return ret;
        }
        //nash:TODO
        #if 0
        if (p2p_if) {
                if (nvt_net_p2p_attach(p2p_ifp) < 0)
                        nvt_p2p_enable = 0;
        }
        #endif
        return ret;

}

static s32 __init nvt_module_init(void)
{
        nvt_dbg(INFO, "%s\n", __func__);
        //nvtfmac_usb_init();

        nvtfmac_sdio_init();
        return 0;
}

static void __exit nvt_module_exit(void)
{
        nvt_dbg(INFO, "%s\n", __func__);
        //nvtfmac_usb_exit();
        nvtfmac_sdio_exit();
}

u32 nvt_dbg_level = (NVT_DBG_ERROR | NVT_DBG_WARN | NVT_DBG_INFO);
module_param_named(debug, nvt_dbg_level, uint, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(debug, "debug level");
u32 nvt_data_mode = stream_mode;
module_param_named(streaming, nvt_data_mode, int, S_IRUSR | S_IWUSR);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION(NVT_VERSION);


module_init(nvt_module_init);
module_exit(nvt_module_exit);
