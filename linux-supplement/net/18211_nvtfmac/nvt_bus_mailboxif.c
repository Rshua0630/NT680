/*
 * NT18211 Linux driver HIL low level design
 * Design follows driver 2.0 HW Interface Layer(HIF) spec.
 * Note:skb control buffer is used to put dma address (sk_buff.cb)
 */
#include "nvt_bus.h"
#include "nvt_util_dbg.h"

static s32 is_sw_mailbox_q_full(struct mailbox_q *q)
{
        unsigned long flags;
        u32 next;

        spin_lock_irqsave(&(q->spinlock), flags);
        //return if queue is full
        next = (q->head_id + 1) & (q->q_depth-1);
        if (q->desc[next].Word0.Own == NOT_MY_OWN) {
                spin_unlock_irqrestore(&(q->spinlock), flags);
                return TRUE;
        } else {
                spin_unlock_irqrestore(&(q->spinlock), flags);
                return FALSE;
        }
}

/*
 * sw_mailbox_q_get() - get from mailbox queue
 * @q:       mailbox queue.
 * @pAddr:   pointer for buffer address
 * @pLen:    pointer for buffer length
 * @pOffset: pointer for offset to start of data in buffer
 * @pDesc:   pointer for host buffer descriptor
 *
 * get from mailbox queue, non-interruptable multi-thread-safe
 * to against race condition
 *
 * Return: EC_SUCCESS
 *         EC_FAIL: mailbox empty
 */
s32 sw_mailbox_q_get(struct mailbox_q *q, u32 *pAddr, u16 *pLen, u8 *pOffset,
        u32 *pDesc)
{
        unsigned long flags;

        spin_lock_irqsave(&(q->spinlock), flags);

        //get buffer address, offset, length and host buf desc from mailbox
        if (q->desc[q->head_id].Word0.Own == MY_OWN) {
                *pAddr = q->desc[q->head_id].BufferAddr;
                *pOffset = q->desc[q->head_id].Word1.Offset;
                *pLen = q->desc[q->head_id].Word1.BufferLen;
                *pDesc = q->desc[q->head_id].HostBufDesc;
                //Add wmb() on Linux, to ensure OWN bit is the last bit written
                wmb();
                q->desc[q->head_id].Word0.Own = NOT_MY_OWN;
                q->head_id = (q->head_id+1) & (q->q_depth-1);
                //interrupt to firmware one message is retrieved
                //TBD q->cbfn_interrupt();

                spin_unlock_irqrestore(&(q->spinlock), flags);
                return EC_SUCCESS;
        } else {
                //no message available
                spin_unlock_irqrestore(&(q->spinlock), flags);
                return EC_FAIL;
        }
}


/*
 * sw_mailbox_q_put() - put to mailbox queue
 * @q:      mailbox queue.
 * @addr:   buffer address
 * @len:    buffer length
 * @offset: offset to start of data in buffer
 * @desc:   host buffer descriptor
 *
 * put to mailbox queue, non-interruptable multi-thread-safe
 * to against race condition
 *
 * Return: EC_SUCCESS
 *         EC_FAIL: mailbox full
 */
s32 sw_mailbox_q_put(struct mailbox_q *q, u32 addr, u16 len, u8 offset,
        u32 desc)
{
        unsigned long flags;
        u32 next;

        spin_lock_irqsave(&(q->spinlock), flags);
        //return if queue is full
        next = (q->head_id + 1) & (q->q_depth-1);
        if (q->desc[next].Word0.Own == NOT_MY_OWN) {
                //queue full
                spin_unlock_irqrestore(&(q->spinlock), flags);
                return EC_FAIL;
        }

        //put buffer address, offset, length and host buf desc to mailbox
        if (q->desc[q->head_id].Word0.Own == MY_OWN) {
                q->desc[q->head_id].BufferAddr = addr;
                q->desc[q->head_id].Word1.Offset = offset;
                q->desc[q->head_id].Word1.BufferLen = len;
                q->desc[q->head_id].HostBufDesc = desc;
                //Add wmb() on Linux, to ensure OWN bit is the last bit written
                wmb();
                q->desc[q->head_id].Word0.Own = NOT_MY_OWN;
                q->head_id = next;
                //interrupt the other side
                //TBD q->cbfn_interrupt();

                spin_unlock_irqrestore(&(q->spinlock), flags);
                return EC_SUCCESS;
        } else {
                spin_unlock_irqrestore(&(q->spinlock), flags);
                //fatal error, unexpected own bit
                //TODO : ASSERT
                nvt_dbg(ERROR, "fata:sw_mailbox_q_put own bit error\n");
                return EC_FAIL;
        }
}


/*
 * sw_mailbox_q_peek() - peek from mailbox queue
 * @q:       mailbox queue.
 * @pAddr:   pointer for buffer address
 * @pLen:    pointer for buffer length
 * @pOffset: pointer for offset to start of data in buffer
 * @pDesc:   pointer for host buffer descriptor
 *
 * peek at mailbox queue, return the first message, but does't update
 * own bit. Non-interruptable multi-thread-safe to against race condition
 *
 * Return: EC_SUCCESS
 *         EC_FAIL: mailbox empty
 */
s32 sw_mailbox_q_peek(struct mailbox_q *q, u32 *pAddr, u16 *pLen, u8 *pOffset,
    u32 *pDesc)
{
        unsigned long flags;

        spin_lock_irqsave(&(q->spinlock), flags);

        //get buffer address, offset, length and host buf desc from mailbox
        if (q->desc[q->head_id].Word0.Own == MY_OWN) {
                *pAddr = q->desc[q->head_id].BufferAddr;
                *pOffset = q->desc[q->head_id].Word1.Offset;
                *pLen = q->desc[q->head_id].Word1.BufferLen;
                *pDesc = q->desc[q->head_id].HostBufDesc;

                spin_unlock_irqrestore(&(q->spinlock), flags);
                return EC_SUCCESS;
        } else {
                //no message available
                spin_unlock_irqrestore(&(q->spinlock), flags);
                return EC_FAIL;
        }
}

int fill_rxa_mailbox_queue(struct _nvt_bus *bus, s32 num)
{
        struct mailbox_q *q = &bus->type.wdev_mailbox.sw_mailbox_rxa_q;
        s32 i = 0;
        dma_addr_t phyaddr;
        struct sk_buff *skb;

        for (i = 0; i < num; i++) {
                if (is_sw_mailbox_q_full(q)) {
                        nvt_dbg(ERROR, "RxA mailbox full\n");
                        break;
                }

                //allocate skb
                skb = dev_alloc_skb(MAX_RX_BUFFER_LEN);
                if (skb == NULL) {
                        nvt_dbg(ERROR, "alloc skb fails for RxA\n");
                        break;
                }

                //map buffer for dma
                phyaddr = dma_map_single(bus->dev, skb->data, MAX_RX_BUFFER_LEN,
                        DMA_FROM_DEVICE);
                if (unlikely(dma_mapping_error(bus->dev, phyaddr))) {
                        nvt_dbg(ERROR, "rxa buffer dma mapping error\n");
                        dev_kfree_skb_any(skb);
                        break;
                }

                *(dma_addr_t *)skb->cb = phyaddr;

                //put to RxA mailbox
                sw_mailbox_q_put(&bus->type.wdev_mailbox.sw_mailbox_rxa_q,
                        phyaddr, skb->len, 0, (u32)skb);
        }
        return i;
}

#if 0
void sw_maibox_ack_isr(struct mailbox_q *q)
{
        s32 i = 0;
        unsigned long flags;

        spin_lock_irqsave(&(q->spinlock), flags);
        //loop to check own bit and update tail pointer till own bit is not mine
        while (q->tail_id != q->head_id) {
                if (q->desc[tail_id].Word0.Own == MY_OWN) {
                        q->tail_id = (q->tail_id+1) & (q->q_depth-1);
                }

                if (i++ > q->q_depth) {
                        //TODO : ASSERT here, fatal error
                        nvt_dbg(ERROR, "sw mailbox ack isr:own error\n");
                        break;
                }
        }

        spin_unlock_irqrestore(&(q->spinlock), flags);
        return;
}
#endif

void mailbox_txc_tasklet(unsigned long busptr)
{
        struct _nvt_bus *bus = (struct _nvt_bus *)busptr;
        struct sk_buff *skb;
        s32 addr;
        s16 len;
        s8 offset;
        s32 ret;
        struct sk_buff_head skblist;

        skb_queue_head_init(&skblist);

        //retrieve Tx completed skb, do unmap
        while ((ret = sw_mailbox_q_get(&bus->type.wdev_mailbox.sw_mailbox_txc_q,
                &addr, &len, &offset, (u32 *)&skb)) != EC_FAIL) {
                //error checking, skb->data shall be addr
                if (skb->data != (u8 *)addr) {
                        nvt_dbg(ERROR, "fatal:txc skb->data not match\n");
                        //ASSERT;
                        BUG();
                }

                dma_unmap_single(bus->dev, *(dma_addr_t *)skb->cb,
                        skb->len+MINIMAL_HEADROOM_SIZE, DMA_TO_DEVICE);
                skb_queue_tail(&skblist, skb);
        }

        //if skb_list is not empty, send skb list to upper layer
        if (!skb_queue_empty(&skblist)) {
                //TBD hif_out_data_complete(bus, skb_list);
        }

        return;
}

void mailbox_rxc_tasklet(unsigned long busptr)
{
        struct _nvt_bus *bus = (struct _nvt_bus *)busptr;
        struct sk_buff *skb;
        s32 addr;
        s16 len;
        s8 offset;
        struct sk_buff_head skblist;
        s32 ret;
        s32 i = 0;

        skb_queue_head_init(&skblist);

        while ((ret = sw_mailbox_q_get(&bus->type.wdev_mailbox.sw_mailbox_rxc_q,
                &addr, &len, &offset, (u32 *)&skb)) != EC_FAIL) {

                //error checking, skb->data shall be addr
                if (skb->data != (u8 *)addr) {
                        nvt_dbg(ERROR, "fatal:rxc skb->data not match\n");
                        BUG();
                }

                //DMA unmap
                dma_unmap_single(bus->dev, *(dma_addr_t *)skb->cb,
                        MAX_RX_BUFFER_LEN, DMA_FROM_DEVICE);

                //TBD offset shall be zero?
                skb_put(skb, len);
                skb_queue_tail(&skblist, skb);
                i++;
        }

        //if skb_list is not empty, send skb list to upper layer
        if (!skb_queue_empty(&skblist)) {
                //TBD hif_in_data_complete(bus, skb_list);
        }

        //refill Rx skb to firmware
        ret = fill_rxa_mailbox_queue(bus, i + bus->type.wdev_mailbox.refillnum);

        /*
         * if fails to refill, need a mechanism to recover
         * ret is number to be refill, to be retried next time.
         */
        if (ret < i) {
                bus->type.wdev_mailbox.refillnum = i - ret;
                nvt_dbg(ERROR, "error:rxc refill\n");
        }

        return;
}

void mailbox_cmd_tasklet(unsigned long busptr)
{
        struct _nvt_bus *bus = (struct _nvt_bus *)busptr;
        s32 i = 0;
        u32 next_id;
        struct mailbox_q *q = &bus->type.wdev_mailbox.sw_mailbox_cmd_q;
        u32 buf;
        s32 len;

        /*
         * check own bit and update tail pointer till own bit is not mine
         * ASSERT if trapped in the loop more than expected times
         */
        while (1) {
                next_id = (q->tail_id+1) & (q->q_depth-1);

                if (q->desc[next_id].Word0.Own == MY_OWN) {
                        buf = q->desc[next_id].HostBufDesc;
                        len = q->desc[next_id].Word1.BufferLen;
                        dma_unmap_single(bus->dev, (dma_addr_t)buf, len,
                                DMA_TO_DEVICE);
                        //TBD hif_out_ctrl_complete(bus, buf, len);
                        q->tail_id = next_id;
                } else {
                    break;
                }

                if (i++ > q->q_depth) {
                        //error, shall not trapped in while loop so many times
                        WARN_ON_ONCE(1);
                        nvt_dbg(ERROR, "fatal:too many cmd ack\n");
                        break;
                }
        }
        return;
}

void mailbox_rsp_tasklet(unsigned long busptr)
{
        struct _nvt_bus *bus = (struct _nvt_bus *)busptr;
        u32 buf;
        u8 *newbuf;
        s16 len;
        s8 offset;
        s32 ret;
        u32 viraddr;
        dma_addr_t phyaddr;
        u32 addr;
        struct sk_buff *skb;
        struct sk_buff_head skblist;

        skb_queue_head_init(&skblist);
        //TBD, fw changed to use rxa buffer
        while ((ret = sw_mailbox_q_peek(
                &bus->type.wdev_mailbox.sw_mailbox_rsp_q,
                &buf, &len, &offset, &viraddr)) != EC_FAIL) {
                //copy response or event to a new buffer
                newbuf = kmalloc(len, GFP_KERNEL);
                if (newbuf != NULL) {
                        dma_unmap_single(bus->dev, (dma_addr_t)buf,
                                MAX_RSP_BUF_LEN, DMA_FROM_DEVICE);
                        memcpy(newbuf, (u8 *)(viraddr+offset), len);
                        phyaddr = dma_map_single(bus->dev, (u8 *)viraddr,
                                MAX_RSP_BUF_LEN, DMA_FROM_DEVICE);
                        if (unlikely(dma_mapping_error(bus->dev, phyaddr))) {
                                nvt_dbg(ERROR, "rsp dma mapping error\n");
                                /*
                                 * TODO : to handle exception is cumbersome,
                                 *  is dma mapping error is really possible?
                                 */
                                BUG();
                                //20151221 nash: coverity#48961
                                kfree(newbuf);
                                break;
                        }
                        //TBD hif_in_ctrl_complete(bus, newbuf, len);
                        ret = sw_mailbox_q_get(
                                &bus->type.wdev_mailbox.sw_mailbox_rsp_q,
                                &addr, &len, &offset, (u32 *)&skb);
                        if (ret != 0) {
                                nvt_dbg(ERROR, "%s: sw_mailbox_q_get() fail",
                                        __func__);
                        }
                } else {
                        /*
                         * TODO : error handling, start a time to handle
                         * or defer till next interrupt from firmware
                         * or do vmalloc()? slow memory but easier to allocate,
                         * shall works here
                         */

                        nvt_dbg(ERROR, "error:rsp alloc fails\n");
                        break;
                }
                //20151221 nash: coverity#48961
                kfree(newbuf);
        }

        return;
}

/*
 * sw_mailbox_q_init() - init all mailboxes
 * @bus:     bus handler
 *
 * Init all mailboxes
 *
 * Return: 0 if success or error code if fail
 */
int sw_mailbox_q_init(struct _nvt_bus *bus)
{
        u8 *mem;
        int i;
        struct mailbox_q *q;
        int ret;

        //allocate and clear memory
        mem = dma_zalloc_coherent(bus->dev, sizeof(struct mailbox_desc)*
                (SW_MAILBOX_TX_Q_DEPTH+SW_MAILBOX_RSP_Q_DEPTH+
                SW_MAILBOX_TXC_Q_DEPTH+SW_MAILBOX_RXA_Q_DEPTH+
                SW_MAILBOX_RXC_Q_DEPTH+SW_MAILBOX_CMD_Q_DEPTH),
                &bus->type.wdev_mailbox.qphyaddr, GFP_KERNEL);

        if (mem == NULL) {
                return -ENOMEM;
        }
        bus->type.wdev_mailbox.qviraddr = mem;

        //TX queue owned by host
        q = &bus->type.wdev_mailbox.sw_mailbox_tx_q;
        q->desc = (struct mailbox_desc *)mem;
        q->head_id = 0;
        q->tail_id = SW_MAILBOX_TX_Q_DEPTH - 1;
        q->q_depth = SW_MAILBOX_TX_Q_DEPTH;
        for (i = 0; i < q->q_depth; i++) {
                q->desc[i].Word0.Own = OWN_BY_HOST;
        }
        spin_lock_init(&q->spinlock);
        mem += SW_MAILBOX_TX_Q_DEPTH*sizeof(struct mailbox_q);

        //TXC queue owned by fw
        q = &bus->type.wdev_mailbox.sw_mailbox_txc_q;
        q->desc = (struct mailbox_desc *)mem;
        q->head_id = 0;
        q->tail_id = SW_MAILBOX_TXC_Q_DEPTH - 1;
        q->q_depth = SW_MAILBOX_TXC_Q_DEPTH;
        for (i = 0; i < q->q_depth; i++) {
                q->desc[i].Word0.Own = OWN_BY_FW;
        }
        spin_lock_init(&q->spinlock);
        mem += SW_MAILBOX_TXC_Q_DEPTH*sizeof(struct mailbox_q);

        //RXA queue owned by host
        q = &bus->type.wdev_mailbox.sw_mailbox_rxa_q;
        q->desc = (struct mailbox_desc *)mem;
        q->head_id = 0;
        q->tail_id = SW_MAILBOX_RXA_Q_DEPTH - 1;
        q->q_depth = SW_MAILBOX_RXA_Q_DEPTH;
        for (i = 0; i < q->q_depth; i++) {
                q->desc[i].Word0.Own = OWN_BY_HOST;
        }
        spin_lock_init(&q->spinlock);
        mem += SW_MAILBOX_RXA_Q_DEPTH*sizeof(struct mailbox_q);

        //RXC queue owned by fw
        q = &bus->type.wdev_mailbox.sw_mailbox_rxc_q;
        q->desc = (struct mailbox_desc *)mem;
        q->head_id = 0;
        q->tail_id = SW_MAILBOX_RXC_Q_DEPTH - 1;
        q->q_depth = SW_MAILBOX_RXC_Q_DEPTH;
        for (i = 0; i < q->q_depth; i++) {
                q->desc[i].Word0.Own = OWN_BY_FW;
        }
        spin_lock_init(&q->spinlock);
        mem += SW_MAILBOX_RXC_Q_DEPTH*sizeof(struct mailbox_q);

        //CMD queue owned by host
        q = &bus->type.wdev_mailbox.sw_mailbox_cmd_q;
        q->desc = (struct mailbox_desc *)mem;
        q->head_id = 0;
        q->tail_id = SW_MAILBOX_CMD_Q_DEPTH - 1;
        q->q_depth = SW_MAILBOX_CMD_Q_DEPTH;
        for (i = 0; i < q->q_depth; i++) {
                q->desc[i].Word0.Own = OWN_BY_HOST;
        }
        spin_lock_init(&q->spinlock);
        mem += SW_MAILBOX_CMD_Q_DEPTH*sizeof(struct mailbox_q);

        //RSP queue owned by FW
        q = &bus->type.wdev_mailbox.sw_mailbox_rsp_q;
        q->desc = (struct mailbox_desc *)mem;
        q->head_id = 0;
        q->tail_id = SW_MAILBOX_RSP_Q_DEPTH - 1;
        q->q_depth = SW_MAILBOX_RSP_Q_DEPTH;
        for (i = 0; i < q->q_depth; i++) {
                q->desc[i].Word0.Own = OWN_BY_FW;
        }
        spin_lock_init(&q->spinlock);

        //allocate Rx buffer and put to RXA queue
        ret = fill_rxa_mailbox_queue(bus, SW_MAILBOX_RXA_Q_DEPTH-1);
        bus->type.wdev_mailbox.refillnum = 0;
        if (ret == SW_MAILBOX_RXA_Q_DEPTH) {
                return 0;
        } else {
                return -ENOMEM;
        }
}

/*
 * sw_mailbox_q_destroy() - destroy all mailboxes
 * @bus:     bus handler
 *
 * Destroy all mailboxes
 *
 * Return: void
 */
void sw_mailbox_q_destroy(struct _nvt_bus *bus)
{
        /*
         * TBD : Unmap and free buffer associated to each DESC
         * Must be extremely careful, if buffer or skb is dma unmap and freed to
         * kernel while fw or wlan DMA is still accessing it, unexpected fatal
         * error is guaranteed. Must ensure FW and driver are in sync.
         * TBD : use buffer manager to clean up(free) Tx and Rx buffers?
         */


        //Free all queues
        dma_free_coherent(bus->dev, sizeof(struct mailbox_desc)*
                (SW_MAILBOX_TX_Q_DEPTH+SW_MAILBOX_RSP_Q_DEPTH+
                SW_MAILBOX_TXC_Q_DEPTH+SW_MAILBOX_RXA_Q_DEPTH+
                SW_MAILBOX_RXC_Q_DEPTH+SW_MAILBOX_CMD_Q_DEPTH),
                bus->type.wdev_mailbox.qviraddr,
                bus->type.wdev_mailbox.qphyaddr);
        return;
}

/*
 * hif_init() - initialize mailbox interface
 * @bus:     bus handler
 *
 * initialize mailbox interface
 *
 * Return: void
 */
int hif_init(struct _nvt_bus *bus)
{
        /*
         * Allocate memory and init mailbox queues, return -ENOMEM if failed
         * and map queues for DMA
         */
        sw_mailbox_q_init(bus);

        //Init tasklets
        tasklet_init(&bus->type.wdev_mailbox.txc_tasklet, mailbox_txc_tasklet,
                (unsigned long)bus);
        tasklet_init(&bus->type.wdev_mailbox.rxc_tasklet, mailbox_rxc_tasklet,
                (unsigned long)bus);
        tasklet_init(&bus->type.wdev_mailbox.cmd_tasklet, mailbox_cmd_tasklet,
                (unsigned long)bus);
        tasklet_init(&bus->type.wdev_mailbox.rsp_tasklet, mailbox_rsp_tasklet,
                (unsigned long)bus);

        //Enable host-to-fw queue interrupt
        ENABLE_MAILBOX_TX_INT;
        ENABLE_MAILBOX_RXA_INT;
        ENABLE_MAILBOX_CMD_INT;

        //update state flag
        bus->type.wdev_mailbox.suspend_state = MAILBOX_ACTIVE_STATE;

        //TODO : Inform firmware the address of mailbox via config register

        return 0;
}

/*
 * hif_destroy() - destroy mailbox interface
 * @bus:     bus handler
 *
 * Destroy mailbox interface
 *
 * Return: void
 */
int hif_destroy(struct _nvt_bus *bus)
{
        //update state flag
        bus->type.wdev_mailbox.suspend_state = MAILBOX_DESTROYED_STATE;

        //Disable all interrupts
        DISABLE_MAILBOX_TX_INT;
        DISABLE_MAILBOX_TXC_INT;
        DISABLE_MAILBOX_RXA_INT;
        DISABLE_MAILBOX_RXC_INT;
        DISABLE_MAILBOX_CMD_INT;
        DISABLE_MAILBOX_RSP_INT;

        //Kill tasklets
        tasklet_kill(&bus->type.wdev_mailbox.txc_tasklet);
        tasklet_kill(&bus->type.wdev_mailbox.rxc_tasklet);
        tasklet_kill(&bus->type.wdev_mailbox.cmd_tasklet);
        tasklet_kill(&bus->type.wdev_mailbox.rsp_tasklet);

        /*
         * Free skb and buffer on mailbox queues, then free mailbox queues
         * and unmap queues for DMA
         */
        sw_mailbox_q_destroy(bus);

        return 0;
}

/*
 * hif_start_in() - start IN-data and IN-ctrl operation
 * @bus:     bus handler
 *
 * start IN-data and IN-ctrl operation
 *
 * Return: void
 */
void hif_start_in(struct _nvt_bus *bus)
{
        //Enable fw-to-host mailbox queue interrupt
        ENABLE_MAILBOX_RXC_INT;
        ENABLE_MAILBOX_RSP_INT;

        return;
}

/*
 * hif_stop_in() - stop IN-data and IN-ctrl operation
 * @bus:     bus handler
 *
 * stop IN-data and IN-ctrl operation
 *
 * Return: void
 */
void hif_stop_in(struct _nvt_bus *bus)
{
        //Disable fw-to-host mailbox queue interrupt
        DISABLE_MAILBOX_RXC_INT;
        DISABLE_MAILBOX_RSP_INT;

        return;
}


/*
 * hif_suspend() - suspend interface
 * @bus:     bus handler
 *
 * suspend interface, non-blocking
 *
 * Return: void
 */
void hif_suspend(struct _nvt_bus *bus)
{
        //TBD system suspend/resume sequence to be designed

        //Change state to suspend
        bus->type.wdev_mailbox.suspend_state = MAILBOX_SUSPEND_STATE;

        //Disable all interrupts
        DISABLE_MAILBOX_TX_INT;
        DISABLE_MAILBOX_TXC_INT;
        DISABLE_MAILBOX_RXA_INT;
        DISABLE_MAILBOX_RXC_INT;
        DISABLE_MAILBOX_CMD_INT;
        DISABLE_MAILBOX_RSP_INT;

        //TBD hif_suspend_complete(bus);

        return;
}

/*
 * hif_resume() - resume interface
 * @bus:     bus handler
 *
 * resume interface, non-blocking
 *
 * Return: void
 */
void hif_resume(struct _nvt_bus *bus)
{
        //TBD system suspend/resume sequence to be designed

        //Enable all interrupts
        ENABLE_MAILBOX_TX_INT;
        ENABLE_MAILBOX_TXC_INT;
        ENABLE_MAILBOX_RXA_INT;
        ENABLE_MAILBOX_RXC_INT;
        ENABLE_MAILBOX_CMD_INT;
        ENABLE_MAILBOX_RSP_INT;

        //Change state to active
        bus->type.wdev_mailbox.suspend_state = MAILBOX_ACTIVE_STATE;

        //TBD hif_resume_complete(bus);

        return;
}

/*
 * hif_out_data() - Send one skb to OUT-data channel
 * @bus:     bus handler
 * @skb:     skb buffer to be sent
 *
 * Send one skb to OUT-data channel
 *
 * Return: 0 if success, error code if fail
 */
int hif_out_data(struct _nvt_bus *bus, struct sk_buff *skb)
{
        dma_addr_t phyaddr;
        struct sk_buff *newskb;

        //if HW interface is in suspend state, return error to bus layer
        if (unlikely(!IS_MAILBOX_ACTIVE(bus))) {
                nvt_dbg(ERROR, "hif_out_data() called while suspend\n");

                return -EACCES;
        }

        //error checking, make sure there is enough head room space
        if (unlikely(skb_headroom(skb) < MINIMAL_HEADROOM_SIZE)) {
                //TODO: shall we copy to new skb instead?
                //Kernel shall preserve headroom
                //if ((newskb = skb_copy(skb, KEERNEL)) != NULL) {
                //    dev_kfree_skb_any(skb)
                //}
                //else {
                //    return -ENOMEM;
                //}
                BUG();
        } else {
                newskb = skb;
        }

        //get physical address and map buffer for DMA
        phyaddr = dma_map_single(bus->dev, newskb->data-MINIMAL_HEADROOM_SIZE,
                newskb->len+MINIMAL_HEADROOM_SIZE, DMA_TO_DEVICE);
        if (unlikely(dma_mapping_error(bus->dev, phyaddr))) {
                nvt_dbg(ERROR, "hif_out_data() dma mapping error\n");

                return -EBUSY;
        }

        *(dma_addr_t *)newskb->cb = phyaddr;

        //put to Tx mailbox queue, if queue full, return error
        sw_mailbox_q_put(&bus->type.wdev_mailbox.sw_mailbox_tx_q, phyaddr,
                newskb->len, 0, (u32)newskb);

        return 0;
}

/*
 * hif_out_ctrl() - Send one buffer to OUT-ctrl channel
 * @bus:        bus handler
 * @data:       buffer to be sent
 * @data_len:   buffer length
 *
 * Send one buffer to OUT-ctrl channel
 *
 * Return: 0 if success, error code if fail
 */
int hif_out_ctrl(struct _nvt_bus *bus, u8 *data, u32 data_len)
{
        dma_addr_t phyaddr;

        //if HW interface is in suspend state, return error to bus layer
        if (!IS_MAILBOX_ACTIVE(bus)) {
                //output error message
                nvt_dbg(ERROR, "hif_out_ctrl() called while suspend\n");

                return -EACCES;
        }

        //get physical address and map buffer for DMA
        phyaddr = dma_map_single(bus->dev, data, data_len, DMA_TO_DEVICE);
        if (unlikely(dma_mapping_error(bus->dev, phyaddr))) {
                nvt_dbg(ERROR, "hif_out_ctrl() dma mapping error\n");

                return -EBUSY;
        }

        //put to CMD mailbox queue, if queue full, return error
        sw_mailbox_q_put(&bus->type.wdev_mailbox.sw_mailbox_cmd_q, phyaddr,
                data_len, (u32)data, 0);
        return 0;
}

//interrupt handlers------------------------------------------------------------

//TxC ISR
void mailbox_txc_ack_isr(struct _nvt_bus *bus)
{
        tasklet_schedule(&bus->type.wdev_mailbox.txc_tasklet);
        return;
}

//RxC ISR
void mailbox_rxc_isr(struct _nvt_bus *bus)
{
        tasklet_schedule(&bus->type.wdev_mailbox.rxc_tasklet);
        return;
}

//CMD ACK ISR
void mailbox_cmd_ack_isr(struct _nvt_bus *bus)
{
        tasklet_schedule(&bus->type.wdev_mailbox.cmd_tasklet);
        return;
}

//RSP ISR
void mailbox_rsp_isr(struct _nvt_bus *bus)
{
        tasklet_schedule(&bus->type.wdev_mailbox.rsp_tasklet);
        return;
}

#if 0
//Tx ACK ISR, no need to handle this interrupt in current implementation
void mailbox_tx_ack_isr(struct _nvt_bus *bus)
{
        struct mailbox_q *q = &bus->type.wdev_mailbox.sw_mailbox_tx_q;

        sw_maibox_ack_isr(q);

        return;
}
#endif

#if 0
//RxA ACK ISR, no need to handle this interrupt in current implementation
void mailbox_rxa_ack_isr(struct _nvt_bus *bus)
{
        struct mailbox_q *q = &bus->type.wdev_mailbox.sw_mailbox_rxa_q;

        sw_maibox_ack_isr(q);

        return;
}
#endif

static struct _nvt_wdev_bus_ops nvt_bus_ops = {
        .tx_data = hif_out_data,
        .init = hif_init,
        .stop = hif_destroy,
        .tx_ctrl = hif_out_ctrl,
};

int hif_probe(struct _nvt_bus *bus)
{
        /* This function is to emulate bus probe entry
         * put the mailbox related initialization code here
         */

        //Init call back functions
        bus->nvt_wdev_bus_ops = nvt_bus_ops;
        return 0;
}
