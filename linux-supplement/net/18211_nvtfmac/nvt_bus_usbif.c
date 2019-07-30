#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/etherdevice.h>
#include <linux/usb.h>

#include "nvt_bus.h"
#include "nvt_bus_usbif.h"
#include "nvt_util_dbg.h"
#include "nvt_fw.h"
#include "nvt_wlan_linux.h"
#include "nvt_iw.h"

static s32 nvt_usb_sumbit_all_rx_urb(struct _nvt_wdev_usb *nvt_wdev_usb);
static void nvt_usb_bulkrx_to_workq(struct work_struct *work);

/**
 * nvt_usb_bus_tx_done_cb - default bus TX done callback function
 * @skb: transmitted socket buffer
 * @status_code: TX completed error code
 *
 * This function is the default callback function for TX done. the given socket
 * buffer would be freed.
 */
static void nvt_usb_bus_tx_done_cb(struct sk_buff *skb, s32 status_code)
{
        dev_kfree_skb_any(skb);
}

static s32 nvt_usb_init_from_dscr(struct usb_device *usb,
        struct _nvt_wdev_usb *nvt_wdev_usb)
{
        u8 ep_num;
        s32 ret = 0;
        struct usb_host_config *usb_host_cfg = usb->actconfig;
        struct usb_config_descriptor *usb_dscr = &(usb_host_cfg->desc);
        struct usb_endpoint_descriptor *ep_dscr;

        nvt_dbg(USB, "%s\n", __func__);

        if (usb->descriptor.bNumConfigurations != 1) {
                nvt_dbg(ERROR, "%s: NumConfiguration != 1\n", __func__);
                ret = -ENODEV;
                goto failed;
        }

        if (usb->descriptor.bDeviceClass != USB_CLASS_VENDOR_SPEC) {
                nvt_dbg(ERROR, "%s: DeviceClass != 0x%02x\n", __func__,
                        USB_CLASS_VENDOR_SPEC);
                ret = -ENODEV;
                goto failed;
        }

        if (usb_dscr->bNumInterfaces != 1) {
                nvt_dbg(ERROR, "%s: NumInterface != 1\n", __func__);
                ret = -ENODEV;
                goto failed;
        }

        nvt_wdev_usb->vendor_id = usb->descriptor.idVendor;
        nvt_wdev_usb->product_id = usb->descriptor.idProduct;

        /*
         * USB descrptor sequence from FW looks like:
         *  EP3 IN  (Interrupt)
         *  EP1 IN  (Bulk)
         *  EP2 OUT (Bulk)
         */
        ep_dscr = &IF_EP_DSCR(usb, 0, 0, BULK_IN_EP_DSCR_SEQ);
        ep_num = ep_dscr->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
        nvt_wdev_usb->rx_bulk_pipe = usb_rcvbulkpipe(usb, ep_num);

        ep_dscr = &IF_EP_DSCR(usb, 0, 0, BULK_OUT_EP_DSCR_SEQ);
        ep_num = ep_dscr->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
        nvt_wdev_usb->tx_bulk_pipe = usb_sndbulkpipe(usb, ep_num);

        if (usb->speed == USB_SPEED_HIGH) {
                nvt_dbg(USB, "%s: NVT WIFI detected(High Speed)\n", __func__);
        } else {
                nvt_dbg(USB, "%s: NVT WIFI detected(Full Speed)\n", __func__);
        }
failed:
        return ret;
}

static s32 nvt_usb_q_init(struct _nvt_wdev_usb *nvt_wdev_usb)
{
        s32 i;
        //20151221 nash: coverity#48982
        s32 ret = 0;
        struct _nvt_usb_reqs *nvt_usb_reqs;

        // Init RX free queue
        nvt_usb_reqs = kcalloc(NVT_RXQ_NUM, sizeof(struct _nvt_usb_reqs),
                GFP_ATOMIC);
        if (nvt_usb_reqs == NULL) {
                nvt_dbg(ERROR, "%s: alloc mem for rx freeq fail\n", __func__);
                ret = -ENOMEM;
                goto alloc_rxq_fail;
        }
        nvt_wdev_usb->rx_reqs = nvt_usb_reqs;

        for (i = 0; i < NVT_RXQ_NUM; i++) {
                nvt_usb_reqs->urb = usb_alloc_urb(0, GFP_ATOMIC);
                if (nvt_usb_reqs->urb == NULL) {
                        nvt_dbg(ERROR, "%s: alloc urb(rxq) fail\n", __func__);
                        ret = -ENOMEM;
                        goto alloc_urb_in_rxq_fail;
                }
                nvt_usb_reqs->nvt_wdev_usb = nvt_wdev_usb;

                INIT_LIST_HEAD(&nvt_usb_reqs->list);
                list_add_tail(&nvt_usb_reqs->list, &nvt_wdev_usb->rx_free_q);
                nvt_usb_reqs++;
        }

        return 0;

alloc_urb_in_rxq_fail:
        while (!list_empty(&nvt_wdev_usb->rx_free_q)) {
                nvt_usb_reqs = list_entry(nvt_wdev_usb->rx_free_q.next,
                        struct _nvt_usb_reqs, list);
                if (nvt_usb_reqs->urb) {
                        usb_free_urb(nvt_usb_reqs->urb);
                        list_del(nvt_wdev_usb->rx_free_q.next);
                }
        }
        kfree(nvt_wdev_usb->rx_reqs);
alloc_rxq_fail:
        return ret;
}

static s32 nvt_usb_q_deinit(struct _nvt_wdev_usb *nvt_wdev_usb)
{
        struct _nvt_usb_reqs *nvt_usb_reqs;
        s32 i;

        nvt_wdev_usb->state = NVT_USB_STATE_DOWN;

        nvt_usb_reqs = nvt_wdev_usb->rx_reqs;

        for (i = 0; i < NVT_RXQ_NUM; i++) {
                if (nvt_usb_reqs->urb) {
                        usb_kill_urb(nvt_usb_reqs->urb);
                        usb_free_urb(nvt_usb_reqs->urb);
                }

                nvt_usb_reqs->nvt_wdev_usb = NULL;

                nvt_usb_reqs++;
        }
        kfree(nvt_wdev_usb->rx_reqs);

        return 0;
}

static struct _nvt_usb_reqs *nvt_usb_q_deq(struct _nvt_wdev_usb *nvt_wdev_usb,
        struct list_head *list_p)
{
        ulong flags;
        struct _nvt_usb_reqs *nvt_usb_reqs = NULL;

        //20150818 nash: add spin lock
        spin_lock_irqsave(&nvt_wdev_usb->qlock, flags);

        if (list_empty(list_p)) {
                spin_unlock_irqrestore(&nvt_wdev_usb->qlock, flags);
                return NULL;
        }
        nvt_usb_reqs = list_first_entry(list_p, struct _nvt_usb_reqs, list);
        list_del_init(&nvt_usb_reqs->list);
        spin_unlock_irqrestore(&nvt_wdev_usb->qlock, flags);

        return nvt_usb_reqs;
}

static s32 nvt_usb_q_enq(struct _nvt_wdev_usb *nvt_wdev_usb,
        struct list_head *list_p,
        struct _nvt_usb_reqs *nvt_usb_reqs)
{
        //20150818 nash: add spin lock
        ulong flags;
        spin_lock_irqsave(&nvt_wdev_usb->qlock, flags);
        list_add_tail(&nvt_usb_reqs->list, list_p);
        spin_unlock_irqrestore(&nvt_wdev_usb->qlock, flags);
        return 0;
}

static void nvt_usb_bulkrx_to_workq(struct work_struct *work)
{
        struct _nvt_wdev_usb *nvt_wdev_usb;
        struct _nvt_bus *nvt_bus;

        nvt_wdev_usb = container_of(work, struct _nvt_wdev_usb, bulkrx_work);

        nvt_bus = dev_get_drvdata(nvt_wdev_usb->dev);
        //20151221 nash: coverity#48949
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL\n", __func__);
                return;
        }
        //skb_queue_head_init(&skb_list);
        //skb_queue_tail(&skb_list, nvt_usb_reqs->skb);

        nvt_process_rx_list(nvt_bus, &nvt_wdev_usb->skb_list);


}

static void nvt_usb_ctrlurb_txcomplete(struct urb *urb)
{
        struct _nvt_wdev_usb *wdev_usb =
                (struct _nvt_wdev_usb *)urb->context;

        /* Record statistic */
        if (urb->status == 0) {
                wdev_usb->usb_statistic.tx_ctrlpkt_cnt++;
        } else {
                nvt_dbg(ERROR, "%s: urb fail, status=%d\n", __func__,
                        urb->status);
                wdev_usb->usb_statistic.tx_ctrlpkt_err_cnt++;
        }

        wdev_usb->ctl_urb_status = urb->status;
        wdev_usb->ctl_completed = true;

        nvt_dbg(USB, "%s: wake up event\n", __func__);
        if (waitqueue_active(&wdev_usb->ctl_resp_wait)) {
                wake_up(&wdev_usb->ctl_resp_wait);
        }
}

static void nvt_usb_ctrlurb_rxcomplete(struct urb *urb)
{
        struct _nvt_wdev_usb *wdev_usb =
                (struct _nvt_wdev_usb *)urb->context;

        /* Record statistic */
        if (urb->status == 0) {
                wdev_usb->usb_statistic.rx_ctrlpkt_cnt++;
        } else {
                nvt_dbg(ERROR, "%s: urb fail, status=%d\n", __func__,
                        urb->status);
                wdev_usb->usb_statistic.rx_ctrlpkt_err_cnt++;
        }

        wdev_usb->ctl_urb_status = urb->status;
        wdev_usb->ctl_completed = true;

        nvt_dbg(USB, "%s: wake up event\n", __func__);
        if (waitqueue_active(&wdev_usb->ctl_resp_wait)) {
                wake_up(&wdev_usb->ctl_resp_wait);
        }
}

static void nvt_usb_bulkurb_txcomplete(struct urb *urb)
{
        struct _nvt_usb_reqs *nvt_usb_reqs;
        struct _nvt_wdev_usb *nvt_wdev_usb;
        struct _nvt_bus *nvt_bus;
        struct _nvt_adapter *nvt_adapter;
        struct _tx_info *tx_info;
        ulong flags;

        nvt_dbg(TX, "%s\n", __func__);

        nvt_usb_reqs = urb->context;
        nvt_wdev_usb = nvt_usb_reqs->nvt_wdev_usb;
        spin_lock_irqsave(&nvt_wdev_usb->txflow_lock, flags);
        nvt_wdev_usb->tx_blkurb_inuse_count--;
        nvt_dbg(TX, "%s, bulkurb_count=%d\n", __func__,
                nvt_wdev_usb->tx_blkurb_inuse_count);
        spin_unlock_irqrestore(&nvt_wdev_usb->txflow_lock, flags);

        if (urb->status != 0 || urb->actual_length == 0) {
                nvt_wdev_usb->usb_statistic.tx_pkt_err_cnt++;
        } else {
                nvt_wdev_usb->usb_statistic.tx_pkt_cnt++;
        }

        nvt_bus = dev_get_drvdata(nvt_wdev_usb->dev);
        //20151221 nash: coverity#48950
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL\n", __func__);
                return;
        }
        nvt_bus->tx_done_callback(nvt_usb_reqs->skb, urb->status);

        usb_free_urb(urb);
        kfree(nvt_usb_reqs);

        nvt_adapter = nvt_bus->nvt_adapter;
        tx_info = nvt_adapter->nvt_priv.tx_info;
#if 0
        if (nvt_wdev_usb->tx_blkurb_inuse_count < nvt_wdev_usb->tx_wm_l &&
                tx_info->bus_flow_blocked) {
                tx_info->bus_flow_blocked = false;
                nvt_shedule_tx_dequeue(tx_info);
        }
#endif
}

static void nvt_usb_bulkurb_rxcomplete(struct urb *urb)
{
        struct _nvt_usb_reqs *nvt_usb_reqs;
        struct _nvt_wdev_usb *nvt_wdev_usb;

        nvt_usb_reqs = urb->context;
        nvt_wdev_usb = nvt_usb_reqs->nvt_wdev_usb;

        if (nvt_wdev_usb->state != NVT_USB_STATE_UP) {
                nvt_dbg(INFO, "%s: usb down\n", __func__);
                return;
        }

        if (urb->status != 0 || urb->actual_length == 0) {
                nvt_dbg(ERROR, "%s: urb fail, enq, status=%d, act_len=%d\n",
                        __func__, urb->status, urb->actual_length);
                nvt_wdev_usb->usb_statistic.rx_pkt_err_cnt++;

                //20150901 nash: prevent memory leakage
                if (nvt_usb_reqs->skb != NULL) {
                        dev_kfree_skb(nvt_usb_reqs->skb);
                        nvt_usb_reqs->skb = NULL;
                }

                nvt_usb_q_enq(nvt_wdev_usb, &nvt_wdev_usb->rx_free_q,
                        nvt_usb_reqs);
        } else {
                //20150918 nash: NULL pointer workaround
                if (nvt_usb_reqs->skb == NULL) {
                        nvt_dbg(INFO, "%s:NULL: act_len=%d\n", __func__,
                                urb->actual_length);

                        nvt_usb_q_enq(nvt_wdev_usb, &nvt_wdev_usb->rx_free_q,
                                nvt_usb_reqs);

                        nvt_usb_sumbit_all_rx_urb(nvt_usb_reqs->nvt_wdev_usb);

                        return;
                }

                nvt_wdev_usb->usb_statistic.rx_pkt_cnt++;
                skb_put(nvt_usb_reqs->skb, urb->actual_length);


                //20150716 nash: move to process context;
                //schedule_work(&nvt_usb_reqs->bulkrx_work);

                //20150915 nash:
                skb_queue_tail(&nvt_wdev_usb->skb_list, nvt_usb_reqs->skb);
                schedule_work(&nvt_wdev_usb->bulkrx_work);

                nvt_usb_q_enq(nvt_wdev_usb,
                &nvt_usb_reqs->nvt_wdev_usb->rx_free_q, nvt_usb_reqs);

                nvt_usb_reqs->skb = NULL;
                nvt_usb_sumbit_all_rx_urb(nvt_usb_reqs->nvt_wdev_usb);
        }

}

static s32 nvt_usb_sumbit_all_rx_urb(struct _nvt_wdev_usb *nvt_wdev_usb)
{
        s32 ret;
        s32 success_cnt = 0;
        struct _nvt_usb_reqs *nvt_usb_reqs = NULL;
        struct sk_buff *skb;

        do {
                nvt_usb_reqs = nvt_usb_q_deq(nvt_wdev_usb,
                        &nvt_wdev_usb->rx_free_q);
                if (nvt_usb_reqs == NULL) {
                        //20151221 nash: coverity#48932
                        break;
                }
                skb = dev_alloc_skb(nvt_wdev_usb->mtu);
                if (!skb) {
                        nvt_dbg(ERROR, "%s: alloc skb fail\n", __func__);
                        nvt_usb_q_enq(nvt_wdev_usb, &nvt_wdev_usb->rx_free_q,
                                nvt_usb_reqs);
                        continue;
                }

                nvt_usb_reqs->skb = skb;

                usb_fill_bulk_urb(nvt_usb_reqs->urb,
                        nvt_wdev_usb->usbdev,
                        nvt_wdev_usb->rx_bulk_pipe,
                        skb->data,
                        skb_tailroom(skb),
                        nvt_usb_bulkurb_rxcomplete,
                        nvt_usb_reqs);

                ret = usb_submit_urb(nvt_usb_reqs->urb, GFP_ATOMIC);
                if (ret) {
                        nvt_dbg(ERROR, "%s: submit urb fail err=%d\n",
                                __func__, ret);
                        dev_kfree_skb(nvt_usb_reqs->skb);
                        nvt_usb_q_enq(nvt_wdev_usb, &nvt_wdev_usb->rx_free_q,
                                nvt_usb_reqs);
                } else {
                        success_cnt++;
                }
        } while (nvt_usb_reqs != NULL);

        return 0;
}

/**
 * nvt_usb_send_ctl() - Send USB control URB
 * @wdev_usb: NVT usb device structure
 * @buf: data buffer for sending
 * @len: length of data buffer
 *
 * Return: 0:success, negative number:fail
 */
static s32 nvt_usb_send_ctl(struct _nvt_wdev_usb *wdev_usb,
                u8 *buf, int len)
{
        s32 ret;
        u8 *buf_coherent;
        u16 size;

        if (wdev_usb == NULL || buf == NULL ||
            len == 0 || wdev_usb->ctrl_urb == NULL) {
                return -EINVAL;
        }

        buf_coherent = usb_alloc_coherent(wdev_usb->usbdev, len, GFP_ATOMIC,
                        &wdev_usb->ctrl_urb->transfer_dma);
        if (buf_coherent == NULL) {
                nvt_dbg(ERROR, "%s: Alloc buf from usb_alloc_coherent fail\r\n",
                        __func__);
                return -1;
        }
        memcpy(buf_coherent, buf, len);

        size = len;
        wdev_usb->ctl_write.wLength = cpu_to_le16p(&size);
        wdev_usb->ctrl_urb->transfer_buffer_length = size;
        //devinfo->ctl_urb_actual_length = 0;

        usb_fill_control_urb(wdev_usb->ctrl_urb,
                wdev_usb->usbdev,
                wdev_usb->tx_ctrl_pipe,
                (unsigned char *) &wdev_usb->ctl_write,
                buf_coherent, size,
                (usb_complete_t)nvt_usb_ctrlurb_txcomplete,
                wdev_usb);
        wdev_usb->ctrl_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

        wdev_usb->ctl_completed = false;

        ret = usb_submit_urb(wdev_usb->ctrl_urb, GFP_ATOMIC);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: usb_submit_urb failed %d\n",
                        __func__, ret);
                goto free_coherent;
        }

        nvt_dbg(USB, "%s: wait complete event...\n", __func__);
        if (wait_event_timeout(wdev_usb->ctl_resp_wait, wdev_usb->ctl_completed,
                        msecs_to_jiffies(NVT_CTRL_URB_TIMEOUT)) == 0) {
                nvt_dbg(ERROR, "%s: timeout, try to unlink control urb\n",
                        __func__);

                //20150917 nash: cancel urb for further using
                usb_unlink_urb(wdev_usb->ctrl_urb);

                ret = -1;
        } else {
                nvt_dbg(USB, "%s: done\r\n", __func__);
        }

free_coherent:
        usb_free_coherent(wdev_usb->usbdev, len, buf_coherent,
                        wdev_usb->ctrl_urb->transfer_dma);

        return ret;
}

/**
 * nvt_usb_recv_ctl() - Receive USB control URB
 * @wdev_usb: NVT usb device structure
 * @buf: received data buffer
 *
 * Return: length of the received data buffer. A negative number if fail
 */
static s32 nvt_usb_recv_ctl(struct _nvt_wdev_usb *wdev_usb, u8 *buf)
{
        s32 ret;
        u32 len;
        u8 *buf_coherent;

        if ((wdev_usb == NULL) || (buf == NULL) ||
            (wdev_usb->ctrl_urb == NULL)) {
                return -EINVAL;
        }

        buf_coherent = usb_alloc_coherent(wdev_usb->usbdev,
                sizeof(struct _nvt_diag_resp), GFP_ATOMIC,
                &wdev_usb->ctrl_urb->transfer_dma);

        if (buf_coherent == NULL) {
                nvt_dbg(ERROR, "%s: Alloc buf from usb_alloc_coherent fail\r\n",
                        __func__);
                return -1;
        }

        wdev_usb->ctl_read.wLength = cpu_to_le16(NVT_DIAG_HDR_LEN);
        wdev_usb->ctl_read.bRequest = 0;

        /* Read response header first to find total message length */
        usb_fill_control_urb(wdev_usb->ctrl_urb,
                wdev_usb->usbdev,
                wdev_usb->rx_ctrl_pipe,
                (u8 *) &wdev_usb->ctl_read,
                buf_coherent, NVT_DIAG_HDR_LEN,
                (usb_complete_t)nvt_usb_ctrlurb_rxcomplete,
                wdev_usb);

        wdev_usb->ctl_completed = false;
        ret = usb_submit_urb(wdev_usb->ctrl_urb, GFP_ATOMIC);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: usb_submit_urb failed(%d)\n", __func__,
                        ret);
                goto free_coherent;
        }

        nvt_dbg(USB, "%s: wait complete event1...\n", __func__);
        if (wait_event_timeout(wdev_usb->ctl_resp_wait, wdev_usb->ctl_completed,
                        msecs_to_jiffies(NVT_CTRL_URB_TIMEOUT)) == 0) {
                nvt_dbg(ERROR, "%s: timeout1, try to unlink control urb\n",
                        __func__);

                //20150917 nash: cancel urb for further using
                usb_unlink_urb(wdev_usb->ctrl_urb);
                ret = -1;
                goto free_coherent;
        } else {
                nvt_dbg(USB, "%s: done\n", __func__);
        }

        /* read whole response */
        //20161020 nash: response header check
        ret = nvt_diag_rsp_header_check(
                (struct _nvt_diag_resp *)(buf_coherent));
        if (ret) {
                nvt_dbg(ERROR, "%s: RX header error (%d)\n", __func__, ret);
                goto free_coherent;
        }

        len = ((struct _nvt_diag_resp *)(buf_coherent))->resp_len;
        len += NVT_DIAG_HDR_LEN;
        if (len > sizeof(struct _nvt_diag_resp)) {
                nvt_dbg(ERROR, "%s: resp_len out of bound!\n", __func__);
                goto free_coherent;
        }

        wdev_usb->ctl_read.wLength = cpu_to_le16(len);
        usb_fill_control_urb(wdev_usb->ctrl_urb,
                wdev_usb->usbdev,
                wdev_usb->rx_ctrl_pipe,
                (u8 *) &wdev_usb->ctl_read,
                buf_coherent, len,
                (usb_complete_t)nvt_usb_ctrlurb_rxcomplete,
                wdev_usb);

        wdev_usb->ctl_completed = false;
        ret = usb_submit_urb(wdev_usb->ctrl_urb, GFP_ATOMIC);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: usb_submit_urb failed %d\n", __func__, ret);
                goto free_coherent;
        }

        nvt_dbg(USB, "%s: wait complete event2...\n", __func__);
        if (wait_event_timeout(wdev_usb->ctl_resp_wait, wdev_usb->ctl_completed,
                        msecs_to_jiffies(NVT_CTRL_URB_TIMEOUT)) == 0) {
                nvt_dbg(USB, "%s: timeout2, try to unlink control urb\n",
                        __func__);

                //20150917 nash: cancel urb for further using
                usb_unlink_urb(wdev_usb->ctrl_urb);
                ret = -1;
                goto free_coherent;
        } else {
                nvt_dbg(USB, "%s: done\r\n", __func__);
        }

        memcpy(buf, buf_coherent, len);
        ret = len;

free_coherent:
        usb_free_coherent(wdev_usb->usbdev, sizeof(struct _nvt_diag_resp),
                        buf_coherent, wdev_usb->ctrl_urb->transfer_dma);

        return ret;
}

static s32 nvt_usb_bus_tx(struct _nvt_bus *nvt_bus, struct sk_buff *skb)
{
        s32 ret = 0;
        struct urb *usb_urb;
        struct _nvt_usb_reqs *nvt_usb_reqs;
        struct _nvt_wdev_usb *nvt_wdev_usb = nvt_bus->type.nvt_wdev_usb;
        struct _nvt_adapter *nvt_adapter;
        struct _tx_info *tx_info;
        ulong flags;

        if (nvt_bus->state != NVT_BUS_STATE_UP) {
                return -1;
        }

        nvt_usb_reqs = kzalloc(sizeof(struct _nvt_usb_reqs), GFP_ATOMIC);
        if (nvt_usb_reqs == NULL) {
                nvt_dbg(ERROR, "%s: kzalloc fail\n", __func__);
                ret = -1;
                goto fail;
        }

        usb_urb = usb_alloc_urb(0, GFP_ATOMIC);
        if (usb_urb == NULL) {
                nvt_dbg(ERROR, "%s: alloc urb fail\n", __func__);
                ret = -1;
                goto urb_fail;
        }

        nvt_usb_reqs->nvt_wdev_usb = nvt_wdev_usb;
        nvt_usb_reqs->skb = skb;
        nvt_usb_reqs->urb = usb_urb;
        spin_lock_irqsave(&nvt_wdev_usb->txflow_lock, flags);
        nvt_wdev_usb->tx_blkurb_inuse_count++;
        nvt_dbg(TX, "%s, bulkurb_count=%d\n", __func__,
                nvt_wdev_usb->tx_blkurb_inuse_count);
        spin_unlock_irqrestore(&nvt_wdev_usb->txflow_lock, flags);

        usb_fill_bulk_urb(nvt_usb_reqs->urb,
                                nvt_wdev_usb->usbdev,
                                nvt_wdev_usb->tx_bulk_pipe,
                                skb->data,
                                skb->len,
                                nvt_usb_bulkurb_txcomplete,
                                nvt_usb_reqs);

        ret = usb_submit_urb(nvt_usb_reqs->urb, GFP_ATOMIC);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: usb_submit_urb fail:%d\n", __func__, ret);
                spin_lock_irqsave(&nvt_wdev_usb->txflow_lock, flags);
                nvt_wdev_usb->tx_blkurb_inuse_count--;
                nvt_dbg(TX, "%s, bulkurb_count=%d\n", __func__,
                        nvt_wdev_usb->tx_blkurb_inuse_count);
                spin_unlock_irqrestore(&nvt_wdev_usb->txflow_lock, flags);
                goto submit_fail;
        }

        nvt_adapter = nvt_bus->nvt_adapter;
        tx_info = nvt_adapter->nvt_priv.tx_info;
#if 0

        if (nvt_wdev_usb->tx_blkurb_inuse_count > nvt_wdev_usb->tx_wm_h &&
                !tx_info->bus_flow_blocked) {
                tx_info->bus_flow_blocked = true;
        }
#endif
        return ret;

submit_fail:
        usb_free_urb(nvt_usb_reqs->urb);
urb_fail:
        kfree(nvt_usb_reqs);
fail:
        return ret;
}

static s32 nvt_usb_bus_init(struct _nvt_bus *bus)
{
        u16 ifnum;
        struct _nvt_wdev_usb *wdev_usb = bus->type.nvt_wdev_usb;
        struct usb_device *usb_dev = wdev_usb->usbdev;
        if (bus->state == NVT_BUS_STATE_UP) {
                return 0;
        }

        bus->state = NVT_BUS_STATE_UP;

        /* set default tx_done callback function */
        nvt_bus_register_txdone_callback(bus, nvt_usb_bus_tx_done_cb);

        if (wdev_usb->ctrl_urb) {
                wdev_usb->rx_ctrl_pipe = usb_rcvctrlpipe(wdev_usb->usbdev, 0);
                wdev_usb->tx_ctrl_pipe = usb_sndctrlpipe(wdev_usb->usbdev, 0);

                ifnum = (&((usb_dev)->actconfig)->desc)->bNumInterfaces;

                //devinfo->ctl_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
                wdev_usb->ctl_write.bRequestType = (USB_DIR_OUT |
                        USB_TYPE_VENDOR);
                wdev_usb->ctl_write.bRequest = 0;
                wdev_usb->ctl_write.wValue = cpu_to_le16(0);
                wdev_usb->ctl_write.wIndex = cpu_to_le16(ifnum);

                wdev_usb->ctl_read.bRequestType =
                        (USB_DIR_IN | USB_TYPE_VENDOR);
                wdev_usb->ctl_read.bRequest = 0;
                wdev_usb->ctl_read.wValue = cpu_to_le16(0);
                wdev_usb->ctl_read.wIndex = cpu_to_le16(ifnum);
        }

        nvt_usb_sumbit_all_rx_urb(wdev_usb);
        wdev_usb->state = NVT_USB_STATE_UP;

        return 0;
}

static s32 nvt_usb_bus_down(struct _nvt_bus *bus)
{
        bus->state = NVT_BUS_STATE_DOWN;
        if (bus->type.nvt_wdev_usb->ctrl_urb) {
                usb_kill_urb(bus->type.nvt_wdev_usb->ctrl_urb);
                usb_free_urb(bus->type.nvt_wdev_usb->ctrl_urb);
        }

        nvt_usb_q_deinit(bus->type.nvt_wdev_usb);

        return 0;
}

static s32 nvt_usb_bus_ctrl_tx(struct _nvt_bus *bus, u8 *buf, u32 len)
{
        s32 err = 0;

        struct _nvt_wdev_usb *wdev_usb = bus->type.nvt_wdev_usb;

        if (bus->state != NVT_BUS_STATE_UP) {
                nvt_dbg(ERROR, "%s: bus state is not UP\r\n", __func__);
                return -EIO;
        }

        if (test_and_set_bit(0, &wdev_usb->ctl_op)) {
                nvt_dbg(ERROR, "%s: ctl_op fail\r\n", __func__);
                return -EIO;
        }

        wdev_usb->ctl_completed = false;

        err = nvt_usb_send_ctl(wdev_usb, buf, len);
        if (err) {
                nvt_dbg(ERROR, "%s: fail %d bytes: %d\r\n", __func__, err, len);
                clear_bit(0, &wdev_usb->ctl_op);
                return err;
        }

        clear_bit(0, &wdev_usb->ctl_op);

        return err;
}

static s32 nvt_usb_bus_ctrl_rx(struct _nvt_bus *bus, u8 *buf, u32 len)
{
        s32 read_len = 0;
        s32 retry = 3;
        struct _nvt_wdev_usb *wdeb_usb = bus->type.nvt_wdev_usb;

        //if (bus->type.nvt_wdev_usb.state != NVT_USB_STATE_UP) {
        if (bus->state != NVT_BUS_STATE_UP) {
                return -EIO;
        }

        if (test_and_set_bit(0, &wdeb_usb->ctl_op)) {
                return -EIO;
        }

        //20161021 nash: rx retry
        do {
        wdeb_usb->ctl_completed = false;
        read_len = nvt_usb_recv_ctl(wdeb_usb, buf);
        clear_bit(0, &wdeb_usb->ctl_op);
        if (read_len < 0) {
                nvt_dbg(ERROR, "%s: fail(%d)\n", __func__, read_len);
        }
        } while (--retry > 0 && read_len < 0);

        return read_len;
}

static struct _nvt_wdev_bus_ops nvt_bus_ops = {
        .tx_data = nvt_usb_bus_tx,
        .init = nvt_usb_bus_init,
        .stop = nvt_usb_bus_down,
        .tx_ctrl = nvt_usb_bus_ctrl_tx,
        .rx_ctrl = nvt_usb_bus_ctrl_rx,
};

static s32 nvt_usb_attach_bus(struct _nvt_wdev_usb *nvt_wdev_usb)
{
        s32 ret = 0;
        struct _nvt_bus *nvt_bus;
        struct device *dev = nvt_wdev_usb->dev;

        nvt_dbg(USB, "%s\n", __func__);


        nvt_wdev_usb->rxq_num = NVT_RXQ_NUM;
        nvt_wdev_usb->rx_wm_l = NVT_RX_WATERMARK_L;

        nvt_wdev_usb->txq_num = NVT_TXQ_NUM;
        nvt_wdev_usb->tx_wm_l = NVT_TX_WATERMARK_L;
        nvt_wdev_usb->tx_wm_h = NVT_TX_WATERMARK_L * 3;

        nvt_wdev_usb->mtu = NVT_USB_MTU;

        nvt_wdev_usb->state = NVT_USB_STATE_DOWN;

        nvt_wdev_usb->rx_ctrl_pipe = 0;
        nvt_wdev_usb->tx_ctrl_pipe = 0;

        /* Initialize the spinlocks */
        spin_lock_init(&nvt_wdev_usb->qlock);
        spin_lock_init(&nvt_wdev_usb->txflow_lock);

        init_waitqueue_head(&nvt_wdev_usb->ctl_resp_wait);

        INIT_LIST_HEAD(&nvt_wdev_usb->rx_free_q);

        //20150918 nash:
        INIT_WORK(&nvt_wdev_usb->bulkrx_work, nvt_usb_bulkrx_to_workq);
        skb_queue_head_init(&nvt_wdev_usb->skb_list);

        ret = nvt_usb_q_init(nvt_wdev_usb);
        if (ret) {
                nvt_dbg(ERROR, "%s: Alloc nvt_usb_q_init failed\n", __func__);
                goto initq_fail;
        }

        nvt_wdev_usb->ctrl_urb = usb_alloc_urb(0, GFP_KERNEL);
        if (nvt_wdev_usb->ctrl_urb == NULL) {
                nvt_dbg(ERROR, "%s: alloc ctrl_urb fail\n", __func__);
                ret = -ENOMEM;
                goto alloc_ctrl_urb_fail;
        }

        nvt_bus = nvt_bus_attach(dev, nvt_bus_ops);
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL\n", __func__);
                ret = -ENOMEM;
                goto bus_attach_failed;
        }

        nvt_bus->type.nvt_wdev_usb = nvt_wdev_usb;

        nvt_bus->nvt_wdev_bus_ops.init(nvt_bus);

        nvt_diag_create_node(nvt_bus);
        return 0;

bus_attach_failed:
        usb_free_urb(nvt_wdev_usb->ctrl_urb);

alloc_ctrl_urb_fail:
        nvt_usb_q_deinit(nvt_wdev_usb);
initq_fail:
        return ret;
}

static s32 nvt_usb_probe(struct usb_interface *intf,
        const struct usb_device_id *id)
{
        s32 ret;
        struct _nvt_wdev_usb *nvt_wdev_usb;
        struct _nvt_bus *nvt_bus = NULL;
        struct usb_device *usb = interface_to_usbdev(intf);

        nvt_dbg(USB, "%s\n", __func__);

        nvt_wdev_usb = kzalloc(sizeof(*nvt_wdev_usb), GFP_ATOMIC);
        if (nvt_wdev_usb == NULL) {
                nvt_dbg(ERROR, "%s: Alloc nvt_wdev_usb fail\n", __func__);
                ret = -ENOMEM;
                goto alloc_nvt_wdev_usb_failed;
        }
        nvt_wdev_usb->usbdev = usb;
        nvt_wdev_usb->dev = &usb->dev;
        usb_set_intfdata(intf, nvt_wdev_usb);

        ret = nvt_usb_init_from_dscr(usb, nvt_wdev_usb);
        if (ret) {
                nvt_dbg(ERROR, "%s: nvt_usb_init_from_dscr fail(err:%d)\n",
                        __func__, ret);
                goto dscr_err;
        }

        ret = nvt_usb_attach_bus(nvt_wdev_usb);
        if (ret) {
                nvt_dbg(ERROR, "%s: nvt_usb_attach_bus fail(err:%d)\n",
                        __func__, ret);
                goto dscr_err;
        }

        nvt_bus = dev_get_drvdata(nvt_wdev_usb->dev);
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL\n", __func__);
                goto dscr_err;
        }

        ret = nvt_fw_download(nvt_bus, NVT_FW_DEFAULT_FW_MODE);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: fw download fail\n", __func__);
                goto attach_bus_err;
        }
        usb_clear_halt(usb, 0);

        ret = nvt_register_to_system(nvt_wdev_usb->dev);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: nvt_register_to_system fail(%d)\n",
                        __func__, ret);
                goto attach_bus_err;
        }

        /* get firmware capability */
        nvt_set_firmware_capability(nvt_bus->nvt_adapter);

        /* Initialized fw configuration by WIDs */
        nvt_init_fw_by_wids(nvt_bus, NL80211_IFTYPE_STATION);
        return 0;

attach_bus_err:
        nvt_diag_delete_node(nvt_bus);
        nvt_bus->nvt_wdev_bus_ops.stop(nvt_bus);

dscr_err:
        usb_set_intfdata(intf, NULL);
        kfree(nvt_wdev_usb);

alloc_nvt_wdev_usb_failed:
        return ret;
}

static void nvt_usb_disconnect(struct usb_interface *intf)
{
        struct _nvt_wdev_usb *wdev_usb;
        struct _nvt_bus *nvt_bus;
        struct _nvt_adapter *nvt_adapter;

        nvt_dbg(INFO, "%s:\n", __func__);

        wdev_usb = (struct _nvt_wdev_usb *)usb_get_intfdata(intf);
        if (wdev_usb == NULL) {
                nvt_dbg(ERROR, "%s: wdev_usb is NULL\n", __func__);
                return;
        }
        nvt_bus = dev_get_drvdata(wdev_usb->dev);
        //20151221 nash: coverity#48951
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL\n", __func__);
                return;
        }
        nvt_adapter = nvt_bus->nvt_adapter;

        nvt_diag_delete_node(nvt_bus);

        if (nvt_bus->nvt_adapter != NULL) {
                nvt_del_all_if(nvt_bus->nvt_adapter);
        }
        nvt_bus->nvt_wdev_bus_ops.stop(nvt_bus);

        nvt_bus_detach(wdev_usb->dev);

        kfree(wdev_usb);

        kfree(nvt_adapter);
}

static s32 nvt_usb_suspend(struct usb_interface *intf, pm_message_t state)
{
        return 0;
}

static s32 nvt_usb_resume(struct usb_interface *intf)
{
        return 0;
}

static struct usb_device_id nvt_usb_supp_table[] = {
        { USB_DEVICE(NVT_USB_VENDOR_ID, NVT_USB_DEVICE_ID) },
        { }
};

static struct usb_driver nvtfmac_usb_drv = {
        .name = KBUILD_MODNAME,
        .probe = nvt_usb_probe,
        .disconnect = nvt_usb_disconnect,
        .id_table = nvt_usb_supp_table,
        .suspend = nvt_usb_suspend,
        .resume = nvt_usb_resume,
};

void nvtfmac_usb_init(void)
{
        nvt_dbg(USB, "%s:\r\n", __func__);
        usb_register(&nvtfmac_usb_drv);
}

void nvtfmac_usb_exit(void)
{
        nvt_dbg(USB, "%s:\r\n", __func__);
        usb_deregister(&nvtfmac_usb_drv);
}
