#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/core.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
#include <linux/suspend.h>
#include <linux/jiffies.h>

#include <linux/delay.h>
#include <linux/wait.h>

#include <linux/pm.h>

#include "nvt_bus.h"
#include "nvt_wlan_linux.h"
#include "nvt_bus_sdioif.h"
#include "nvt_util_dbg.h"
#include "nvt_diag.h"
#include "nvt_wlan_priv.h"
#include "nvt_iw.h"


//20161020 nash: work around for power-saving wake up
#define NVT_SDIO_CMD_RETRY                  3

//20161020 nash: sdio read/write function with retry
//               (work around for hw power-saving)
static void nvt_retry_sdio_writeb(struct sdio_func *func, u8 b,
        u32 addr, s32 *err_ret)
{
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                sdio_writeb(func, b, addr, err_ret);

        } while (*err_ret && --cmd_retry > 0);
}

static s32 nvt_retry_sdio_writesb(struct sdio_func *func, u32 addr,
        void *src, s32 count)
{
        s32 ret;
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                ret = sdio_writesb(func, addr, src, count);

        } while (ret && --cmd_retry > 0);

        return ret;
}

static void nvt_retry_sdio_writew(struct sdio_func *func, u16 b,
        u32 addr, s32 *err_ret)
{
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                sdio_writew(func, b, addr, err_ret);

        } while (*err_ret && --cmd_retry > 0);
}

static void nvt_retry_sdio_writel(struct sdio_func *func, u32 b,
        u32 addr, s32 *err_ret)
{
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                sdio_writel(func, b, addr, err_ret);

        } while (*err_ret && --cmd_retry > 0);
}


static s32 nvt_retry_sdio_readsb(struct sdio_func *func, void *dst,
        u32 addr, s32 count)
{
        s32 ret;
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                ret = sdio_readsb(func, dst, addr, count);

        } while (ret && --cmd_retry > 0);

        return ret;
}

static u8 nvt_retry_sdio_readb(struct sdio_func *func,
        u32 addr, s32 *err_ret)
{
        u8 val;
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                val = sdio_readb(func, addr, err_ret);

        } while (*err_ret && --cmd_retry > 0);

        return val;

}

static u32 nvt_retry_sdio_readl(struct sdio_func *func, u32 addr, s32 *err_ret)
{
        u32 val;
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                val = sdio_readl(func, addr, err_ret);

        } while (*err_ret && --cmd_retry > 0);

        return val;
}

static u8 nvt_retry_sdio_f0_readb(struct sdio_func *func,
        u32 addr, s32 *err_ret)
{
        u8 val;
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                val = sdio_f0_readb(func, addr, err_ret);

        } while (*err_ret && --cmd_retry > 0);

        return val;
}

static void nvt_retry_sdio_f0_writeb(struct sdio_func *func, u8 b,
        u32 addr, s32 *err_ret)
{
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                sdio_f0_writeb(func, b, addr, err_ret);

        } while (*err_ret && --cmd_retry > 0);
}

static s32 nvt_retry_sdio_memcpy_toio(struct sdio_func *func, u32 addr,
        void *src, s32 count)
{
        s32 ret;
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                ret = sdio_memcpy_toio(func, addr, src, count);

        } while (ret && --cmd_retry > 0);

        return ret;
}

static s32 nvt_retry_sdio_memcpy_fromio(struct sdio_func *func, void *dst,
        u32 addr, s32 count)
{
        s32 ret;
        s32 cmd_retry = NVT_SDIO_CMD_RETRY;

        do {
                ret = sdio_memcpy_fromio(func, dst, addr, count);

        } while (ret && --cmd_retry > 0);

        return ret;
}


/**
 * nvt_usb_bus_tx_done_cb - default bus TX done callback function
 * @skb: transmitted socket buffer
 * @status_code: TX completed error code
 *
 * This function is the default callback function for TX done. the given socket
 * buffer would be freed.
 */
static void nvt_sdio_bus_tx_done_cb(struct sk_buff *skb, s32 status_code)
{
        dev_kfree_skb_any(skb);
}

/**
 * nvt_sdio_pm_wait_resume - wait for resume
 * @nvt_wdev_sdio: _nvt_wdev_sdio structure
 * @wait_q: wait queue
 *
 * Return: 0:resume already, -1:still in suspend state
 */
static s32 nvt_sdio_pm_wait_resume(struct _nvt_wdev_sdio *nvt_wdev_sdio,
        wait_queue_head_t *wait_q)
{

        return 0;
}

static s32 nvt_sdio_wait_ctrl_msg_resp(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        s32 ret = 0;

        if (wait_event_timeout(
                nvt_wdev_sdio->ctrl_msg_resp_wait,
                nvt_wdev_sdio->ctrl_msg_resp_rdy,
                msecs_to_jiffies(NVT_SDIO_CTRL_MSG_TIMEOUT)) == 0) {
                nvt_dbg(ERROR, "%s: wait timeout\n", __func__);
                ret = -1;
        }

        return ret;

}


static s32 nvt_sdio_rwu(struct _nvt_wdev_sdio *nvt_wdev_sdio, bool write,
        u8 func_num, u32 addr, void *data, u32 data_len)
{
        s32 ret;
        s32 retry = 0;

        if (nvt_wdev_sdio == NULL) {
                nvt_dbg(ERROR, "%s: nvt_wdev_sdio is NULL\n", __func__);
                return -EFAULT;
        }

        if (func_num != NVT_SDIO_FUNC0 && func_num != NVT_SDIO_FUNC1) {
                nvt_dbg(ERROR, "%s: error sdio func\n", __func__);
                return -ENODEV;
        }

        if (data_len != 1 && data_len != 2 && data_len != 4) {
                nvt_dbg(ERROR, "%s: data_len fail\n", __func__);
                return -EPERM;
        }

        do {
                if (retry) {
                        udelay(NVT_SDIO_ACCESS_SLEEP_US);
                }

                ret = nvt_sdio_pm_wait_resume(nvt_wdev_sdio,
                        &nvt_wdev_sdio->req_byte_wait);
                if (ret) {
                        nvt_dbg(ERROR, "%s: waiting resume..\n", __func__);
                        continue;
                }

                if (write) {
                        if (data_len == 1) {
                                //20161020 nash: nvt sdio retry fn
                                //(work around for hw power-saving)
                                nvt_retry_sdio_writeb(
                                        nvt_wdev_sdio->func[func_num],
                                        *(u8 *)data, addr, &ret);

                        } else if (data_len == 2) {
                                //20161020 nash: nvt sdio retry fn
                                //(work around for hw power-saving)
                                nvt_retry_sdio_writew(
                                        nvt_wdev_sdio->func[func_num],
                                        *(u16 *)data, addr, &ret);

                        } else if (data_len == 4) {
                                //20161020 nash: nvt sdio retry fn
                                //(work around for hw power-saving)
                                nvt_retry_sdio_writel(
                                        nvt_wdev_sdio->func[func_num],
                                        *(u32 *)data, addr, &ret);
                        }
                        if (ret) {
                                nvt_dbg(ERROR, "%s: sdio_writeX fail(%d)\n",
                                        __func__, ret);
                                continue;
                        }

                } else {
                        if (data_len == 1) {
                                //20161020 nash: nvt sdio retry fn
                                //(work around for hw power-saving)
                                *(u8 *)data = nvt_retry_sdio_readb(
                                        nvt_wdev_sdio->func[func_num], addr,
                                        &ret);

                        } else if (data_len == 2) {
                                //20161020 nash: nvt sdio retry fn
                                //(work around for hw power-saving)
                                *(u16 *)data = nvt_retry_sdio_readl(
                                        nvt_wdev_sdio->func[func_num], addr,
                                        &ret);

                        } else if (data_len == 4) {
                                //20161020 nash: nvt sdio retry fn
                                //(work around for hw power-saving)
                                *(u32 *)data = nvt_retry_sdio_readl(
                                        nvt_wdev_sdio->func[func_num], addr,
                                        &ret);
                        }
                        if (ret) {
                                nvt_dbg(ERROR, "%s: sdio_readX fail(%d)\n",
                                        __func__, ret);
                                continue;
                        }
                }

        } while (retry++ < NVT_SDIO_ACCESS_RETRY_LIMIT && ret != 0);

        return ret;

}

static s32 nvt_sdio_readu8(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 func_num,
        u32 addr, u8 *ret_data)
{
        return nvt_sdio_rwu(nvt_wdev_sdio, false, func_num,
                addr, (void *)ret_data, 1);
}

static s32 nvt_sdio_readu32(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 func_num,
        u32 addr, u32 *ret_data)
{
        return nvt_sdio_rwu(nvt_wdev_sdio, false, func_num,
                addr, (void *)ret_data, 4);

}

static s32 nvt_sdio_writeu8(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 func_num,
        u32 addr, u8 w_data)
{
         return nvt_sdio_rwu(nvt_wdev_sdio, true, func_num,
                addr, (void *)&w_data, 1);
}

static s32 nvt_sdio_writeu32(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 func_num,
        u32 addr, u32 w_data)
{
        return nvt_sdio_rwu(nvt_wdev_sdio, true, func_num,
                addr, (void *)&w_data, 4);

}

/**
 * nvt_sdio_dataread() - read data from SDIO interface
 * @nvt_wdev_sdio: NVT SDIO device structure
 * @func_num: SDIO function number
 * @addr: address for reading
 * @ret_data: data buffer for storing read data
 * @rec_len: reading length
 * @read_type: NVT_SDIO_FIFO_DATA or NVT_SDIO_CHUNK_DATA
 *
 * Return: 0:success, negative number:fail
 */
static s32 nvt_sdio_dataread(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 func_num,
        u32 addr, u8 *ret_data, u32 rec_len, s32 read_type)
{
        s32 ret;

        if (nvt_wdev_sdio == NULL) {
                nvt_dbg(ERROR, "%s: nvt_wdev_sdio is NULL\n", __func__);
                return -EFAULT;
        }

        if (func_num != NVT_SDIO_FUNC0 && func_num != NVT_SDIO_FUNC1) {
                nvt_dbg(ERROR, "%s: error sdio func\n", __func__);
                return -ENODEV;
        }

        ret = nvt_sdio_pm_wait_resume(nvt_wdev_sdio,
                                &nvt_wdev_sdio->req_byte_wait);
        if (ret) {
                nvt_dbg(ERROR, "%s: waiting resume..\n", __func__);
                return -EPERM;
        }

        if (read_type == NVT_SDIO_FIFO_DATA) {
                return nvt_retry_sdio_readsb(nvt_wdev_sdio->func[func_num],
                        (u8 *)ret_data, addr, rec_len);

        } else if (read_type == NVT_SDIO_CHUNK_DATA) {
                //20161020 nash: nvt sdio retry fn
                //               (work around for hw power-saving)
                return nvt_retry_sdio_memcpy_fromio(
                        nvt_wdev_sdio->func[func_num],
                        (u8 *)ret_data, addr, rec_len);
        } else {
                return -EPERM;
        }

}

static s32 nvt_sdio_datawrite(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 func_num,
        u32 addr, u8 *data, u32 data_len, s32 write_type)
{
        s32 ret;

        if (nvt_wdev_sdio == NULL) {
                nvt_dbg(ERROR, "%s: nvt_wdev_sdio is NULL\n", __func__);
                return -EFAULT;
        }

        if (func_num != NVT_SDIO_FUNC0 && func_num != NVT_SDIO_FUNC1) {
                nvt_dbg(ERROR, "%s: error sdio func\n", __func__);
                return -ENODEV;
        }

        ret = nvt_sdio_pm_wait_resume(nvt_wdev_sdio,
                                &nvt_wdev_sdio->req_byte_wait);
        if (ret) {
                nvt_dbg(ERROR, "%s: waiting resume..\n", __func__);
                return -EPERM;
        }
        if (write_type == NVT_SDIO_FIFO_DATA) {
                //20161020 nash: nvt sdio retry fn
                //         (work around for hw power-saving)
                return nvt_retry_sdio_writesb(nvt_wdev_sdio->func[func_num],
                        addr, (void *)data, data_len);
        } else if (write_type == NVT_SDIO_CHUNK_DATA) {
                //20161020 nash: nvt sdio retry fn
                //               (work around for hw power-saving)
                return nvt_retry_sdio_memcpy_toio(nvt_wdev_sdio->func[func_num],
                        addr, (void *)data, data_len);
        } else {
                return -EPERM;
        }

}

s32 nvt_sdio_abort(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 fn)
{

        //20160119 nash: using f0 api
        s32 ret = 0;
        /* issue abort cmd52 command through F0 */
        //nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC0, SDIO_CCCR_SELx, fn);
        //20161020 nash: nvt sdio retry fn(work around for hw power-saving)
        nvt_retry_sdio_f0_writeb(nvt_wdev_sdio->func[0], fn,
                SDIO_CCCR_SELx, &ret);
        return ret;
}

static s32 nvt_sdio_bus_release(struct _nvt_wdev_sdio *nvt_wdev_sdio, u8 fn)
{
        unsigned long timeout;
        s32 ret = 0;
        /* step1. indicate fun number */
        //nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC0, SDIO_CCCR_SELx, fn);
        //20160119 nash: using f0 api
        //20161020 nash: nvt sdio retry fn(work around for hw power-saving)
        nvt_retry_sdio_f0_writeb(nvt_wdev_sdio->func[0], fn,
                SDIO_CCCR_SELx, &ret);
        if (ret) {
                nvt_dbg(ERROR, "%s: write SDIO_CCCR_SELx fail(%d)!!\n",
                                __func__,
                                ret);
                return ret;
        }
        /* step2. release the bus indicated in step1*/
        fn = (1 << 1);
        //nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC0, SDIO_CCCR_SUSPEND,
        //        fn);
        //20160119 nash: using f0 api
        //20161020 nash: nvt sdio retry fn(work around for hw power-saving)
        nvt_retry_sdio_f0_writeb(nvt_wdev_sdio->func[0], fn,
                SDIO_CCCR_SUSPEND, &ret);
        if (ret) {
                nvt_dbg(ERROR, "%s: write SDIO_CCCR_SUSPEND fail(%d)!!\n",
                                __func__,
                                ret);
                return ret;
        }

        timeout = jiffies + 2*HZ;
        while (time_before(jiffies, timeout)) {
                //20160119 nash: using f0 api
                //nvt_sdio_readu8(nvt_wdev_sdio, NVT_SDIO_FUNC0,
                //        SDIO_CCCR_SUSPEND, &fn);
                //20161020 nash: nvt sdio retry fn
                //               (work around for hw power-saving)
                fn = nvt_retry_sdio_f0_readb(nvt_wdev_sdio->func[0],
                        SDIO_CCCR_SUSPEND, &ret);
                if (ret) {
                        nvt_dbg(ERROR,
                                "%s: read SDIO_CCCR_SUSPEND fail(%d)!!\n",
                                __func__,
                                ret);
                        return ret;
                }
                if (!(fn & (1 << 1))) {
                        break;
                }
        }

        if (fn & (1 << 1)) {
                nvt_dbg(ERROR, "%s: the BR request isn't be satisified\n",
                        __func__);
        }

        return 0;
}

static s32 nvt_sdio_send_tx_retry(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        s32 ret;
        s32 retry_cnt = 3;
        //unsigned long timeout;

        //timeout = jiffies + 10*HZ;
        do {
                ret = nvt_sdio_writeu32(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                        SDIO_HOST2CARD_INT, TX_RETRY);

                if (ret) {
                        nvt_sdio_abort(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                        nvt_sdio_bus_release(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                }
        } while (--retry_cnt > 0 && ret != 0);

        return 0;
}

static s32 nvt_sdio_send_sync_pkt(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        s32 ret;

        if (nvt_wdev_sdio == NULL) {
                nvt_dbg(ERROR, "%s: nvt_wdev_sdio is NULL\n", __func__);
                return -1;
        }

        ret = nvt_sdio_writeu32(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                SDIO_HOST2CARD_INT, DIAG_REQ);
        if (ret) {
                nvt_dbg(ERROR, "%s: write u32 fail\n", __func__);
        }

        return ret;
}

static s32 nvt_sdio_read_int_status(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        s32 ret;
        u32 ret_data = 0;
        u32 n;

        ret = nvt_sdio_readu32(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                SDIO_CARD2HOST_INT, &ret_data);
        if (ret) {
                nvt_dbg(ERROR, "%s: read int status fail\n", __func__);
                atomic_set(&nvt_wdev_sdio->intstatus, 0);
        } else if (ret_data) {
                //20160706 nash: to prevent deadlock when trying to send
                //diagnostic message after receiving a event
                if (ret_data & DIAG_COMP) {
                        nvt_wdev_sdio->ctrl_msg_resp_rdy = 1;
                        if (waitqueue_active
                                (&nvt_wdev_sdio->ctrl_msg_resp_wait)) {
                                wake_up(&nvt_wdev_sdio->ctrl_msg_resp_wait);
                        }
                        ret_data &= ~DIAG_COMP;
                }

                if (ret_data & PKT_VLD) {
                        /*Record packet length for further use */
                        nvt_wdev_sdio->pkt_len = (ret_data >> 16);
                        nvt_wdev_sdio->rx_int_cnt++;
                }

                ret_data &= 0xFFFF;

                //nash:work around for twice ISR call for one interrupt
                if (ret_data == 0) {
                        return -1;
                }

                for_each_set_bit(n, (unsigned long *)&ret_data, 32) {
                        set_bit(n,
                        (unsigned long *)&nvt_wdev_sdio->intstatus.counter);
                }
        }

        return ret;

}

static void nvt_sdio_irq_handler(struct sdio_func *func)
{
        struct _nvt_bus *nvt_bus;
        struct _nvt_wdev_sdio *nvt_wdev_sdio;
        s32 ret;

        nvt_bus = dev_get_drvdata(&func->dev);
        //20151218 nash: coverity#48945
        if (nvt_bus == NULL) {
                return;
        }
        if (nvt_bus->state == NVT_BUS_STATE_DOWN) {
                return;
        }

        nvt_wdev_sdio = nvt_bus->type.nvt_wdev_sdio;

        if (in_interrupt()) {
                atomic_set(&nvt_wdev_sdio->int_pending, 1);
        } else {
                ret = nvt_sdio_read_int_status(nvt_wdev_sdio);
                if (ret) {
                        return;
                }
        }

        atomic_inc(&nvt_wdev_sdio->datahdl_tskcnt);
        schedule_work(&nvt_wdev_sdio->datahdl_work);

        return;
}

static s32 nvt_sdio_register_isr(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        s32 ret;
        s32 retry = 0;

        sdio_claim_host(nvt_wdev_sdio->func[1]);
        //20170123 nash: retry (a work around for power-saving/wake up)
        do {
                ret = sdio_claim_irq(nvt_wdev_sdio->func[1],
                        &nvt_sdio_irq_handler);
        } while (retry++ < NVT_SDIO_ACCESS_RETRY_LIMIT && ret != 0);

        sdio_release_host(nvt_wdev_sdio->func[1]);

        return ret;
}

static s32 nvt_sdio_unregister_isr(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        s32 ret;
        s32 retry = 0;

        sdio_claim_host(nvt_wdev_sdio->func[1]);
        //20170123 nash: retry (a work around for power-saving/wake up)
        do {
        ret = sdio_release_irq(nvt_wdev_sdio->func[1]);
        } while (retry++ < NVT_SDIO_ACCESS_RETRY_LIMIT && ret != 0);

        sdio_release_host(nvt_wdev_sdio->func[1]);

        return ret;
}

static s32 nvt_sdio_data_handler_rx(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        s32 ret;
        u32 pad_len;
        struct sk_buff *pkt_skb;
        struct _nvt_bus *nvt_bus;
        s32 max_rxburst = NVT_SDIO_RX_BRUST_NUM;
        u32 ret_data = 0;

        nvt_bus = dev_get_drvdata(nvt_wdev_sdio->dev);
        if (nvt_bus == NULL) {
                return -1;
        }

#ifdef BUS_UTILIZATION
        if (nvt_bus->nvt_bu->rx_enable && nvt_bus->nvt_bu->start_time == 0) {
                nvt_bus->nvt_bu->start_time = get_jiffies_64();
        }
#endif
        ret = nvt_sdio_unregister_isr(nvt_wdev_sdio);
        if (ret) {
                nvt_dbg(ERROR, "%s: unregister ISR fail\n", __func__);
                return ret;
        }

        while (1) {
                /* Align read packet length with block size */
                pad_len = roundup(nvt_wdev_sdio->pkt_len,
                        nvt_wdev_sdio->blocksize) - nvt_wdev_sdio->pkt_len;

                pkt_skb = dev_alloc_skb(nvt_wdev_sdio->pkt_len + pad_len);
                if (!pkt_skb) {
                        nvt_dbg(ERROR, "%s: dev_alloc_skb() fail\n", __func__);
                        //nash:TODO
                        sdio_claim_host(nvt_wdev_sdio->func[1]);
                        nvt_sdio_abort(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                        nvt_sdio_bus_release(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                        sdio_release_host(nvt_wdev_sdio->func[1]);
                        goto fail;
                }

                sdio_claim_host(nvt_wdev_sdio->func[1]);
                ret = nvt_sdio_dataread(nvt_wdev_sdio,
                        NVT_SDIO_FUNC1, SDIO_RX_PORT,
                        (u8 *)pkt_skb->data,
                        nvt_wdev_sdio->pkt_len + pad_len,
                        NVT_SDIO_FIFO_DATA);
                sdio_release_host(nvt_wdev_sdio->func[1]);
                if (ret) {
                        nvt_dbg(ERROR, "%s: read packet fail(%d)\n", __func__,
                                ret);
                        dev_kfree_skb_any(pkt_skb);
                        sdio_claim_host(nvt_wdev_sdio->func[1]);
                        nvt_sdio_abort(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                        nvt_sdio_bus_release(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                        sdio_release_host(nvt_wdev_sdio->func[1]);

                        nvt_bus->statistic.pkt_rx_err_cnt++;

                        goto fail;
                }

                skb_put(pkt_skb, (nvt_wdev_sdio->pkt_len + pad_len));
                skb_trim(pkt_skb, nvt_wdev_sdio->pkt_len);

#if (NVT_SDIO_CFG_AUTO_PKT_DONE == 0)
                /* Tell the device that read is done */
                sdio_claim_host(nvt_wdev_sdio->func[1]);
                ret = nvt_sdio_writeu32(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                        SDIO_HOST2CARD_INT, RX_PKT_DONE);
                sdio_release_host(nvt_wdev_sdio->func[1]);
                if (ret) {
                        nvt_dbg(ERROR, "%s: send packet done fail\n", __func__);
                        dev_kfree_skb_any(pkt_skb);
                        //nash:TODO
                        sdio_claim_host(nvt_wdev_sdio->func[1]);
                        nvt_sdio_abort(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                        nvt_sdio_bus_release(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                        sdio_release_host(nvt_wdev_sdio->func[1]);

                        //20170120 nash: should enable ISR again
                        nvt_sdio_register_isr(nvt_wdev_sdio);

                        nvt_bus->statistic.pkt_rx_err_cnt++;

                        return ret;
                }
#endif

#ifdef BUS_UTILIZATION
                if (nvt_bus->nvt_bu->rx_enable) {
                        nvt_bus->nvt_bu->total_pkt_size_in_byte +=
                                pkt_skb->len;

                        if (++nvt_bus->nvt_bu->rx_runtime_cnt >=
                                nvt_bus->nvt_bu->rx_total_cnt) {

                                nvt_bus_bu_show_result(nvt_bus, false);
                        }
                }
                dev_kfree_skb_any(pkt_skb);
#else
                /* Send to upper layer */
                skb_queue_tail(&nvt_wdev_sdio->rx_skb_list, pkt_skb);
#endif

                nvt_bus->statistic.pkt_rx_cnt++;

                if (--max_rxburst <= 0) {
                        break;
                }
                ret_data = 0;
                sdio_claim_host(nvt_wdev_sdio->func[1]);
                ret = nvt_sdio_readu32(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                        SDIO_CARD2HOST_INT, &ret_data);
                sdio_release_host(nvt_wdev_sdio->func[1]);

                if (ret_data & PKT_VLD) {
                        nvt_wdev_sdio->pkt_len = (ret_data >> 16);
                } else {
                        break;
                }

                nvt_bus->statistic.pkt_rx_cnt++;

        }

        nvt_sdio_register_isr(nvt_wdev_sdio);
        if (ret) {
                nvt_dbg(ERROR, "%s: register ISR fail\n", __func__);
        }

        nvt_process_rx_list(nvt_bus, &nvt_wdev_sdio->rx_skb_list);

        return ret;

fail:

        //20170120 nash: should enable ISR again
        nvt_sdio_register_isr(nvt_wdev_sdio);

//20160222 nash:
#if (NVT_SDIO_CFG_AUTO_PKT_DONE == 0)
        nvt_dbg(ERROR, "%s: fail and tell rx done\n", __func__);

        /* Tell the device that read is done */
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_writeu32(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                SDIO_HOST2CARD_INT, RX_PKT_DONE);
        sdio_release_host(nvt_wdev_sdio->func[1]);
#endif

        return ret;

}

/**
 * nvt_sdio_data_handler_tx - TX data handler
 * @nvt_wdev_sdio: _nvt_wdev_sdio structure
 *
 * return: 1:tx data pending, 0:no tx data pending, < 0: error
 */
static s32 nvt_sdio_data_handler_tx(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        struct _nvt_bus *nvt_bus;

        //nash: adjust later
        s32 max_frame_tx_cnt = 1;
        s32 i;
        s32 ret = 0;
        u32 pad_len;
        u32 pkt_len = 0;
        s32 retry_cnt;
        u32 q_len;
        struct sk_buff *skb;

#if (NVT_SDIO_GET_AVAI_SPACE_BEFORE_TX == 1)
        u8 txbd_ptr[8];
#endif
        nvt_bus = dev_get_drvdata(nvt_wdev_sdio->dev);
        //20151218 nash: coverity#48944
        if (nvt_bus == NULL) {
                return -1;
        }

        q_len = skb_queue_len(&nvt_wdev_sdio->tx_skb_list);
        if (q_len == 0) {
                return 0;
        }

        for (i = 0; i < max_frame_tx_cnt; i++) {

                //20160304 nash:
                #if (NVT_SDIO_GET_AVAI_SPACE_BEFORE_TX == 1)
                retry_cnt = 1000;
                do {
                        sdio_claim_host(nvt_wdev_sdio->func[1]);
                        ret = nvt_sdio_dataread(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                                SDIO_TX_WPTR, txbd_ptr, 8,
                                NVT_SDIO_CHUNK_DATA);
                        sdio_release_host(nvt_wdev_sdio->func[1]);
                        if (ret) {
                                nvt_dbg(ERROR, "%s: read TX BD FAIL(%d)\n",
                                        __func__, ret);

                                sdio_claim_host(nvt_wdev_sdio->func[1]);
                                nvt_sdio_abort(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                                nvt_sdio_bus_release(nvt_wdev_sdio,
                                        NVT_SDIO_FUNC1);
                                sdio_release_host(nvt_wdev_sdio->func[1]);
                                return -1;
                        }
                        if (txbd_ptr[0] != txbd_ptr[4]) {
                                break;
                        } else {
                                mdelay(1);
                        }

                } while (--retry_cnt > 0);

                if (retry_cnt == 0) {
                        nvt_dbg(ERROR,
                        "%s: Waiting TX BD table is available timeout\n",
                        __func__);
                        return -1;
                }
                #endif
                skb = skb_dequeue(&nvt_wdev_sdio->tx_skb_list);
                if (skb == NULL) {
                        break;
                }

                /* Align read packet length with block size */
                pad_len = roundup(skb->len,
                        nvt_wdev_sdio->blocksize) - skb->len;
                //nash:TODO
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        if (skb_availroom(skb) > pad_len) {
        #else
        if (skb_tailroom(skb) > pad_len) {
        #endif
                        skb_put(skb, pad_len);
                        pkt_len = skb->len;
                } else {
                        pkt_len = skb->len + pad_len;
                }

                retry_cnt = 3;
                sdio_claim_host(nvt_wdev_sdio->func[1]);
                do {
                        ret = nvt_sdio_datawrite(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                                SDIO_TX_PORT, (u8 *)skb->data, pkt_len,
                                NVT_SDIO_FIFO_DATA);

                        if (ret) {
                                nvt_dbg(ERROR, "%s: data write fail(%d)\n",
                                        __func__, ret);

                                nvt_sdio_abort(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                                nvt_sdio_bus_release(nvt_wdev_sdio,
                                        NVT_SDIO_FUNC1);

                                nvt_sdio_send_tx_retry(nvt_wdev_sdio);
//20160222 nash:
#if (NVT_SDIO_CFG_AUTO_PKT_DONE == 1)
                        }
#else
                        } else {
                                ret = nvt_sdio_writeu32(nvt_wdev_sdio,
                                                NVT_SDIO_FUNC1,
                                                SDIO_HOST2CARD_INT,
                                                TX_PKT_DONE);
                                if (ret) {
                                        nvt_dbg(ERROR,
                                        "%s: send TX packet done fail\n",
                                        __func__);
                                }
                        }
#endif
                        retry_cnt--;
                } while (ret != 0 && retry_cnt >= 0);
                sdio_release_host(nvt_wdev_sdio->func[1]);

                if (ret != 0) {
                        nvt_dbg(ERROR, "%s: tx packet fail\n", __func__);
                        nvt_bus->statistic.pkt_tx_err_cnt++;
                        return ret;
                }

#ifdef BUS_UTILIZATION
                if (nvt_bus->nvt_bu->tx_enable) {
                        nvt_bus->nvt_bu->total_pkt_size_in_byte += skb->len;
                }
#endif
                nvt_bus->tx_done_callback(skb, 0);
                nvt_bus->statistic.pkt_tx_cnt++;

        }

        //nash:TODO:tx flow control


#ifdef BUS_UTILIZATION

        if (nvt_bus->nvt_bu->tx_enable) {

                if (++nvt_bus->nvt_bu->tx_runtime_cnt >=
                        nvt_bus->nvt_bu->tx_total_cnt) {

                        nvt_bus_bu_show_result(nvt_bus, true);
                }
        }
#endif

        if (q_len > max_frame_tx_cnt) {
                return 1;
        }

        return ret;
}

static void nvt_sdio_data_handler(struct work_struct *work)
{
        struct _nvt_wdev_sdio *nvt_wdev_sdio;
        s32 ret;
        u32 int_status;
        s32 n;
        u8 rxbd_ptr[8] = {0};

        nvt_wdev_sdio = container_of(work, struct _nvt_wdev_sdio, datahdl_work);

      do {
        if (!atomic_read(&nvt_wdev_sdio->datahdl_tskcnt)) {
            nvt_dbg(ERROR, "%s: weird, Event triggered, but, tskcnt=0\n",
                    __func__);
        } else {
            atomic_dec(&nvt_wdev_sdio->datahdl_tskcnt);
        }
                ret = 0;
                if (atomic_read(&nvt_wdev_sdio->int_pending)) {
                        atomic_set(&nvt_wdev_sdio->int_pending, 0);
                        sdio_claim_host(nvt_wdev_sdio->func[1]);
                        nvt_sdio_read_int_status(nvt_wdev_sdio);
                        sdio_release_host(nvt_wdev_sdio->func[1]);
                }

                int_status = atomic_xchg(&nvt_wdev_sdio->intstatus, 0);

                if (int_status & DIAG_COMP) {
                        nvt_wdev_sdio->ctrl_msg_resp_rdy = 1;
                        //wake_up_interruptible(
                        //        &nvt_wdev_sdio->ctrl_msg_resp_wait);
                        if (waitqueue_active
                                (&nvt_wdev_sdio->ctrl_msg_resp_wait)) {
                                wake_up(&nvt_wdev_sdio->ctrl_msg_resp_wait);
                        }
                        int_status &= ~DIAG_COMP;
                }

                if (int_status & (PKT_VLD | RX_PKT_WORK_ARD)) {
                        if ((int_status & (PKT_VLD)) == 0) {
                                sdio_claim_host(nvt_wdev_sdio->func[1]);
                                ret = nvt_sdio_dataread(nvt_wdev_sdio,
                                    NVT_SDIO_FUNC1, SDIO_RX_WPTR, rxbd_ptr, 8,
                                    NVT_SDIO_CHUNK_DATA);
                                sdio_release_host(nvt_wdev_sdio->func[1]);
                                if (ret) {
                                        nvt_dbg(ERROR,
                                                "%s: read RX BD FAIL(%d)\n",
                                                __func__, ret);

                                        sdio_claim_host(nvt_wdev_sdio->func[1]);
                                        nvt_sdio_abort(nvt_wdev_sdio,
                                                NVT_SDIO_FUNC1);
                                        nvt_sdio_bus_release(nvt_wdev_sdio,
                                                NVT_SDIO_FUNC1);
                                        sdio_release_host(
                                                nvt_wdev_sdio->func[1]);

                                //Read RX data only if RX write ptr != read ptr
                                } else if (rxbd_ptr[0] != rxbd_ptr[4]) {
                                        nvt_dbg(ERROR,
                                            "%s: read RX by work ard\n",
                                            __func__);
                                        nvt_sdio_data_handler_rx(nvt_wdev_sdio);
                                } else {
                                        nvt_dbg(ERROR,
                                            "%s: RX_WORK_ARD, but no data!!\n",
                                            __func__);
                                }

                        } else {
                        nvt_sdio_data_handler_rx(nvt_wdev_sdio);
                        }
                        int_status &= ~(PKT_VLD | RX_PKT_WORK_ARD);
                }

//20170717 nash:
#if 0
                if (int_status & RX_PKT_WORK_ARD) {
                        nvt_dbg(INFO, "%s: RX_PKT_WORK_ARD\n",
                                __func__);
//20160222 nash: TODO
#if (NVT_SDIO_CFG_AUTO_PKT_DONE == 0)
                        /* work around */
                        sdio_claim_host(nvt_wdev_sdio->func[1]);
                        nvt_sdio_writeu32(nvt_wdev_sdio,
                                NVT_SDIO_FUNC1,
                                SDIO_HOST2CARD_INT, RX_PKT_DONE);
                        sdio_release_host(nvt_wdev_sdio->func[1]);
#endif
                        int_status &= ~RX_PKT_WORK_ARD;
                }
#endif

                if (int_status & PKT_TX_SIZE_ERR) {
                        nvt_dbg(INFO, "%s: PKT_TX_SIZE_ERR\n",
                                __func__);
                }

                if (int_status) {
                        for_each_set_bit(n, (unsigned long *)&int_status, 32) {
                                set_bit(n,
                                (unsigned long *)
                                &nvt_wdev_sdio->intstatus.counter);
                        }
                }

                ret = nvt_sdio_data_handler_tx(nvt_wdev_sdio);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: data tx fail\n", __func__);
                }

                //nash:CHECK
                //if (atomic_read(&nvt_wdev_sdio->intstatus)&0xFF || ret == 1) {
                if (ret == 1) {
                        atomic_inc(&nvt_wdev_sdio->datahdl_tskcnt);
                }

        } while (atomic_read(&nvt_wdev_sdio->datahdl_tskcnt) > 0);
}

static s32 nvt_sdio_bus_init(struct _nvt_bus *bus)
{
        s32 ret;

        ret = nvt_sdio_register_isr(bus->type.nvt_wdev_sdio);

        bus->state = NVT_BUS_STATE_UP;

        /* set default tx_done callback function */
        ret |= nvt_bus_register_txdone_callback(bus, nvt_sdio_bus_tx_done_cb);

        return ret;
}

static s32 nvt_sdio_bus_down(struct _nvt_bus *bus)
{
        struct _nvt_wdev_sdio *nvt_wdev_sdio;
        struct sk_buff *cur;
        struct sk_buff *next;

        bus->state = NVT_BUS_STATE_DOWN;

        nvt_wdev_sdio = bus->type.nvt_wdev_sdio;

        nvt_sdio_unregister_isr(nvt_wdev_sdio);

        cancel_work_sync(&nvt_wdev_sdio->datahdl_work);

        skb_queue_walk_safe(&nvt_wdev_sdio->tx_skb_list, cur, next) {
                skb_unlink(cur, &nvt_wdev_sdio->tx_skb_list);
                dev_kfree_skb_any(cur);
        }

        return 0;
}

static s32 nvt_sdio_bus_tx(struct _nvt_bus *nvt_bus, struct sk_buff *skb)
{
        //struct _nvt_adapter *nvt_adapter;
        //struct _tx_info *tx_info;
        struct _nvt_wdev_sdio *nvt_wdev_sdio;


        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL\n", __func__);
                return -1;
        }

        if (nvt_bus->state != NVT_BUS_STATE_UP) {
                return -1;
        }

        nvt_wdev_sdio = nvt_bus->type.nvt_wdev_sdio;

        skb_queue_tail(&nvt_wdev_sdio->tx_skb_list, skb);

        //nash:TODO:flow control
        #if 0
        nvt_adapter = nvt_bus->nvt_adapter;
        tx_info = nvt_adapter->nvt_priv.tx_info;
        if (nvt_wdev_usb->tx_blkurb_inuse_count > nvt_wdev_usb->tx_wm_h &&
                !tx_info->bus_flow_blocked) {
                tx_info->bus_flow_blocked = true;
        }
        #endif

        if (atomic_read(&nvt_wdev_sdio->datahdl_tskcnt) == 0) {
                atomic_inc(&nvt_wdev_sdio->datahdl_tskcnt);
                schedule_work(&nvt_wdev_sdio->datahdl_work);
        }

        return 0;

}

static s32 nvt_sdio_bus_ctrl_tx(struct _nvt_bus *bus, u8 *buf, u32 len)
{
        s32 ret;
        s32 retval;
        u8 addr_zone_ori;
        s32 retry = 0;

        struct _nvt_wdev_sdio *nvt_wdev_sdio = bus->type.nvt_wdev_sdio;


        if (bus->state != NVT_BUS_STATE_UP) {
                nvt_dbg(ERROR, "%s: bus state is not UP\n", __func__);
                return -EIO;
        }

        /*switch to diagnostic address zone */
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_readu8(nvt_wdev_sdio, NVT_SDIO_FUNC1, SDIO_ADDR_ZONE,
                &addr_zone_ori);
        if (ret) {
                sdio_release_host(nvt_wdev_sdio->func[1]);
                nvt_dbg(ERROR, "%s: read addr_zone reg fail\n", __func__);
                bus->statistic.ctl_tx_err_cnt++;
                return ret;
        }

        ret = nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC1, SDIO_ADDR_ZONE,
                (u8)SDIO_ADDR_ZONE_DIAG);
        if (ret) {
                sdio_release_host(nvt_wdev_sdio->func[1]);
                nvt_dbg(ERROR, "%s: write addr_zone reg fail\n", __func__);
                bus->statistic.ctl_tx_err_cnt++;
                return ret;
        }
        sdio_release_host(nvt_wdev_sdio->func[1]);

        do {
                sdio_claim_host(nvt_wdev_sdio->func[1]);
                ret = nvt_sdio_datawrite(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                        SDIO_DIAG_DATA_ADDR_OFFSET, buf, len,
                        NVT_SDIO_CHUNK_DATA);
                if (ret) {
                        nvt_dbg(ERROR, "%s: data write fail\n", __func__);
                        sdio_release_host(nvt_wdev_sdio->func[1]);
                        continue;
                }

                //send the packet for sync
                nvt_wdev_sdio->ctrl_msg_resp_rdy = 0;
                ret = nvt_sdio_send_sync_pkt(nvt_wdev_sdio);
                if (ret) {
                        nvt_dbg(ERROR, "%s: send sync packet fail\n", __func__);
                        sdio_release_host(nvt_wdev_sdio->func[1]);
                        continue;
                }
                sdio_release_host(nvt_wdev_sdio->func[1]);

                ret = nvt_sdio_wait_ctrl_msg_resp(nvt_wdev_sdio);
                if (ret) {
                        ret = -ETIME;
                        nvt_dbg(ERROR, "%s: wait resp timeout\n", __func__);
                        break;
                }

        } while (ret != 0 && --retry > 0);

        retval = ret;

        /*restore original address zone */
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC1, SDIO_ADDR_ZONE,
                addr_zone_ori);
        sdio_release_host(nvt_wdev_sdio->func[1]);
        if (ret) {
                nvt_dbg(ERROR, "%s: recover addr_zone fail\n", __func__);
                return -1;
        }

        if (retval) {
                bus->statistic.ctl_tx_err_cnt++;
        } else {
                bus->statistic.ctl_tx_cnt++;
        }
        return retval;
}

static s32 nvt_sdio_bus_ctrl_rx(struct _nvt_bus *bus, u8 *buf, u32 len)
{
        s32 ret;
        s32 retval = 0;
        u8 addr_zone_ori;
        u8 *nvt_hdr;
        u32 read_len;

        struct _nvt_wdev_sdio *nvt_wdev_sdio = bus->type.nvt_wdev_sdio;


        if (bus->state != NVT_BUS_STATE_UP) {
                nvt_dbg(ERROR, "%s: bus state is not UP\n", __func__);
                bus->statistic.ctl_rx_err_cnt++;
                return -EIO;
        }

        /*switch to diagnostic address zone */
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_readu8(nvt_wdev_sdio, NVT_SDIO_FUNC1, SDIO_ADDR_ZONE,
                &addr_zone_ori);
        if (ret) {
                sdio_release_host(nvt_wdev_sdio->func[1]);
                nvt_dbg(ERROR, "%s: read addr_zone reg fail\n", __func__);
                bus->statistic.ctl_rx_err_cnt++;
                return -1;
        }

        ret = nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC1, SDIO_ADDR_ZONE,
                SDIO_ADDR_ZONE_DIAG);
        if (ret) {
                sdio_release_host(nvt_wdev_sdio->func[1]);
                nvt_dbg(ERROR, "%s: write addr_zone reg fail\n", __func__);
                bus->statistic.ctl_rx_err_cnt++;
                return -1;
        }
        sdio_release_host(nvt_wdev_sdio->func[1]);

        nvt_hdr = kmalloc(NVT_DIAG_HDR_LEN, GFP_KERNEL);
        if (!nvt_hdr) {
                nvt_dbg(ERROR, "%s: alloc fail\n", __func__);
                retval = -ENOMEM;
                bus->statistic.ctl_rx_err_cnt++;
                goto alloc_fail;
        }

        //read first 4 bytes (header)
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_dataread(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                SDIO_DIAG_DATA_ADDR_OFFSET, nvt_hdr, NVT_DIAG_HDR_LEN,
                NVT_SDIO_CHUNK_DATA);
        sdio_release_host(nvt_wdev_sdio->func[1]);
        if (ret) {
                nvt_dbg(ERROR, "%s: read header fail\n", __func__);
                retval = -1;
                bus->statistic.ctl_rx_err_cnt++;
                goto alloc_fail;
        }

        //read whole message
        ret = nvt_diag_rsp_header_check((struct _nvt_diag_resp *)(nvt_hdr));
        if (ret) {
                nvt_dbg(ERROR, "%s: RX header error (%d)\n", __func__, ret);
                retval = -1;
                bus->statistic.ctl_rx_err_cnt++;
                goto alloc_fail;
        }
        read_len = ((struct _nvt_diag_resp *)(nvt_hdr))->resp_len;
        read_len += NVT_DIAG_HDR_LEN;
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_dataread(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                SDIO_DIAG_DATA_ADDR_OFFSET, buf, read_len, NVT_SDIO_CHUNK_DATA);
        sdio_release_host(nvt_wdev_sdio->func[1]);
        if (ret) {
                nvt_dbg(ERROR, "%s: read message fail\n", __func__);
                retval = -1;
                bus->statistic.ctl_rx_err_cnt++;
                goto alloc_fail;
        }

        retval = read_len;

        bus->statistic.ctl_rx_cnt++;

alloc_fail:

        kfree(nvt_hdr);

        /*restore original address zone */
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC1, SDIO_ADDR_ZONE,
                addr_zone_ori);
        sdio_release_host(nvt_wdev_sdio->func[1]);
        if (ret) {
                nvt_dbg(ERROR, "%s: recover addr_zone fail\n", __func__);
                return -1;
        }


        return retval;
}

static struct _nvt_wdev_bus_ops nvt_sdio_bus_ops = {
        .init = nvt_sdio_bus_init,
        .stop = nvt_sdio_bus_down,
        .tx_data = nvt_sdio_bus_tx,
        .tx_ctrl = nvt_sdio_bus_ctrl_tx,
        .rx_ctrl = nvt_sdio_bus_ctrl_rx,
};

static struct sdio_device_id nvt_sdio_supp_table[] = {
        { SDIO_DEVICE(NVT_SDIO_VENDOR_ID, NVT_SDIO_DEVICE_ID_18202) },
        { }
};

static s32 nvt_sdio_attach_bus(struct _nvt_wdev_sdio *nvt_wdev_sdio)
{
        struct _nvt_bus *nvt_bus;
        s32 ret = 0;

        if (nvt_wdev_sdio == NULL) {
                return -1;
        }


        sdio_claim_host(nvt_wdev_sdio->func[1]);

        /* set F1 blocksize */
        ret = sdio_set_block_size(nvt_wdev_sdio->func[1],
                NVT_SDIO_F1_BLOCKSIZE);
        if (ret) {
                nvt_dbg(ERROR, "%s: set F1 block size fail\n", __func__);
                sdio_release_host(nvt_wdev_sdio->func[1]);
                return ret;
        }

        ret = sdio_enable_func(nvt_wdev_sdio->func[1]);
        if (ret) {
                nvt_dbg(ERROR, "%s: enable F1 fail(%d)\n", __func__, ret);
                sdio_release_host(nvt_wdev_sdio->func[1]);
                return ret;
        }
        sdio_release_host(nvt_wdev_sdio->func[1]);

        /* Make block size work */
        nvt_wdev_sdio->func[1]->card->quirks |= MMC_QUIRK_BLKSZ_FOR_BYTE_MODE;

        /* struct member init */
        atomic_set(&nvt_wdev_sdio->suspend, false);
        init_waitqueue_head(&nvt_wdev_sdio->req_byte_wait);
        init_waitqueue_head(&nvt_wdev_sdio->req_word_wait);
        init_waitqueue_head(&nvt_wdev_sdio->req_buffer_wait);
        init_waitqueue_head(&nvt_wdev_sdio->ctrl_msg_resp_wait);

        nvt_wdev_sdio->blocksize = NVT_SDIO_F1_BLOCKSIZE;
        //nash:TODO??
        nvt_wdev_sdio->rxpending = false;
        //nash:TODO??
        nvt_wdev_sdio->txminmax = 1;
        nvt_wdev_sdio->pkt_len = 0;
        nvt_wdev_sdio->is_polling = false;
        if (nvt_wdev_sdio->is_polling == true) {
                nvt_wdev_sdio->pollrate = 1;
        }
        nvt_wdev_sdio->polltick = 0;
        nvt_wdev_sdio->activity = true;
        nvt_wdev_sdio->idletime = 1;
        nvt_wdev_sdio->idlecount = 0;

        nvt_wdev_sdio->rx_int_cnt = 0;
        INIT_WORK(&nvt_wdev_sdio->datahdl_work, nvt_sdio_data_handler);

        atomic_set(&nvt_wdev_sdio->datahdl_tskcnt, 0);
        atomic_set(&nvt_wdev_sdio->intstatus, 0);

        skb_queue_head_init(&nvt_wdev_sdio->tx_skb_list);
        skb_queue_head_init(&nvt_wdev_sdio->rx_skb_list);

        nvt_bus = nvt_bus_attach(nvt_wdev_sdio->dev, nvt_sdio_bus_ops);
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus created fail\n", __func__);
                //20151218 nash: coverity#48936
                //ret = -ENOMEM;
                return -ENOMEM;
        }

        dev_set_drvdata(&nvt_wdev_sdio->func[1]->dev, nvt_bus);
        nvt_wdev_sdio->dev = &nvt_wdev_sdio->func[1]->dev;

        nvt_bus->type.nvt_wdev_sdio = nvt_wdev_sdio;

        nvt_bus->nvt_wdev_bus_ops.init(nvt_bus);

        nvt_diag_create_node(nvt_bus);

#if (NVT_SDIO_CFG_AUTO_PKT_DONE == 1)
        //20160222 nash: config SDIO to atuo packet done mode
        sdio_claim_host(nvt_wdev_sdio->func[1]);
        ret = nvt_sdio_writeu32(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                SDIO_TX_CONFIG,
                (AUTO_TX_PKT_DONE | AUTO_RX_PKT_DONE));

        sdio_release_host(nvt_wdev_sdio->func[1]);
        if (ret) {
                nvt_dbg(ERROR, "%s: config to auto pkt done mode fail\n",
                        __func__);

                sdio_claim_host(nvt_wdev_sdio->func[1]);
                nvt_sdio_abort(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                nvt_sdio_bus_release(nvt_wdev_sdio, NVT_SDIO_FUNC1);
                sdio_release_host(nvt_wdev_sdio->func[1]);
                return -1;
        }
#endif

        return 0;
}

#ifdef CONFIG_PM_SLEEP

/**
 * nvt_sdio_pm_is_fw_wakeup: polling if fw is ready when resume
 * @nvt_bus: _nvt_bus structure
 *
 * Use fw get version diagnostic command to poll if firmware is ready
 *
 * Return: < 0: not wakeup or failure, 1:waked up
 */
//20160818 nash: for new deep sleep follow, we did not need this function
#if 0
static s32 nvt_sdio_pm_is_fw_wakeup(struct _nvt_bus *nvt_bus)
{
        s32 ret;
        //20151218 nash: coverity#48981
        s32 pkt_len = 0;
        s32 retry = 10;

        struct _nvt_diag_req *diag_req = NULL;
        struct _nvt_diag_resp *diag_resp = NULL;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(ERROR, "%s: diag_req kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(ERROR, "%s: diag_resp kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_GET_FW_VER_CMD, 0, 0,
                NULL, diag_req, &pkt_len);

        do {
                ret = nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus,
                        (u8 *)diag_req, pkt_len);
                if (ret < 0) {
                        if (ret == -ETIME) {
                                retry = 2;
                        }

                        nvt_dbg(ERROR, "%s: failed to send diag req!!\n",
                                __func__);
                        mdelay(100);
                        continue;
                }

                ret = nvt_bus->nvt_wdev_bus_ops.rx_ctrl(nvt_bus, (
                        u8 *)diag_resp, sizeof(struct _nvt_diag_resp));
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: failed to recv diag resp!!\n",
                                __func__);
                        mdelay(100);
                        continue;
                } else {
                        ret = 1;
                }
        } while (--retry > 0 && ret < 0);


alloc_failed:
        kfree(diag_req);
        kfree(diag_resp);

        return ret;

}
#endif

static s32 nvt_sdio_pm_suspend(struct device *dev)
{
        mmc_pm_flag_t flags;
        struct _nvt_bus *nvt_bus;
        struct _nvt_if *nvt_if;
        struct _nvt_wdev_sdio *nvt_wdev_sdio;
        struct sk_buff *skb;
        struct sk_buff *pnext;
        s32 ret;

        nvt_dbg(INFO, "%s: enter\n", __func__);

        nvt_bus = dev_get_drvdata(dev);
        //20151218 nash:  coverity#48947
        if (nvt_bus == NULL) {
                return -1;
        }
        nvt_wdev_sdio = nvt_bus->type.nvt_wdev_sdio;
        nvt_if = nvt_get_if_by_index(nvt_bus->nvt_adapter, 0);
        if (!nvt_if) {
                nvt_dbg(ERROR, "%s: get NULL nvt_if!!\n", __func__);
                return -EINVAL;
        }

        nvt_dbg(INFO, "%s: mode=%d\n", __func__, nvt_if->sleep_mode);

        if (nvt_if->sleep_mode == NVT_SLEEP_MODE_DISABLE) {
                nvt_dbg(ERROR, "%s: sleep mode is disable!!\n", __func__);
                return -EINVAL;

        } else if (nvt_if->sleep_mode == NVT_SLEEP_MODE_NORMAL ||
                        nvt_if->sleep_mode == NVT_SLEEP_MODE_WOW) {

                /* keep power */
                flags = sdio_get_host_pm_caps(nvt_wdev_sdio->func[1]);
                if (flags & MMC_PM_KEEP_POWER) {
                        ret = sdio_set_host_pm_flags(nvt_wdev_sdio->func[1],
                                        MMC_PM_KEEP_POWER);
                        if (ret) {
                                nvt_dbg(ERROR,
                                        "%s: set KEEP_POWER flag fail!!\n",
                                        __func__);
                                return ret;
                        }
                } else {
                        nvt_dbg(ERROR, "%s: not support KEEP_POWER\n",
                                        __func__);
                }

                //20160726 nash: for new deep sleep flow, request cpf first
                ret = nvt_fw_request_cpf(nvt_bus);
                if (ret) {
                        nvt_dbg(ERROR, "%s: request cpf fail!!\n", __func__);
                        return ret;
                }

                //data flush
                nvt_flush_txq(nvt_bus->nvt_adapter);

                cancel_work_sync(&nvt_wdev_sdio->datahdl_work);
                atomic_set(&nvt_wdev_sdio->datahdl_tskcnt, 0);

                skb_queue_walk_safe(&nvt_wdev_sdio->tx_skb_list, skb, pnext) {
                        skb_unlink(skb, &nvt_wdev_sdio->tx_skb_list);
                        dev_kfree_skb_any(skb);
                }

                skb_queue_walk_safe(&nvt_wdev_sdio->rx_skb_list, skb, pnext) {
                        skb_unlink(skb, &nvt_wdev_sdio->rx_skb_list);
                        dev_kfree_skb_any(skb);
                }

                atomic_set(&nvt_wdev_sdio->int_pending, 0);

        }


        return 0;

}

static s32 nvt_sdio_pm_resume(struct device *dev)
{
        s32 ret;
        struct _nvt_bus *nvt_bus;
        struct _nvt_if *nvt_if;
        struct _nvt_wdev_sdio *nvt_wdev_sdio;
//        u32 entry_addr;

        nvt_dbg(INFO, "%s: enter\n", __func__);

        nvt_bus = dev_get_drvdata(dev);
        //20151218 nash:  coverity#48946
        if (nvt_bus == NULL) {
                return -1;
        }
        nvt_wdev_sdio = nvt_bus->type.nvt_wdev_sdio;
        nvt_if = nvt_get_if_by_index(nvt_bus->nvt_adapter, 0);
        if (!nvt_if) {
                nvt_dbg(ERROR, "%s: get NULL nvt_if!!\n", __func__);
                return -EINVAL;
        }

        nvt_dbg(INFO, "%s: mode=%d\n", __func__, nvt_if->sleep_mode);

        if (nvt_if->sleep_mode == NVT_SLEEP_MODE_DISABLE) {
                nvt_dbg(ERROR, "%s: sleep mode is disable!!\n", __func__);
                return -EINVAL;

        } else if (nvt_if->sleep_mode == NVT_SLEEP_MODE_NORMAL) {
                sdio_claim_host(nvt_wdev_sdio->func[1]);
                ret = nvt_sdio_writeu8(nvt_wdev_sdio, NVT_SDIO_FUNC1,
                                SDIO_RESUME, (u8)SDIO_WAKEUP);
                sdio_release_host(nvt_wdev_sdio->func[1]);
                if (ret) {
                        nvt_dbg(ERROR, "%s: set SDIO_WAKEUP fail\n", __func__);
                        return ret;
                }

                //20160715 nash: for FW new deep sleep flow
                ret = nvt_fw_deep_sleep_resume(nvt_bus);
                if (ret) {
                        nvt_dbg(ERROR, "%s: deep sleep resume follow fail!!\n",
                                __func__);
                        return ret;
                }

                #if 0
                ret = nvt_sdio_pm_is_fw_wakeup(nvt_bus);
                if (ret != 1) {
                        nvt_dbg(ERROR, "%s: polling if waked up fail!!\n",
                                __func__);
                        return ret;
                }
                #endif
        } else if (nvt_if->sleep_mode != NVT_SLEEP_MODE_WOW) {
                return -EINVAL;
        }

        return 0;
}

static const struct dev_pm_ops nvt_sdio_pm_ops = {
        .suspend = nvt_sdio_pm_suspend,
        .resume = nvt_sdio_pm_resume,
};

/* 20151113: nash:work around because nt96660 mmc host driver suspend/resume not
    ready yet
 */
s32 nvt_sdio_wa_suspend(struct _nvt_bus *nvt_bus)
{
        struct _nvt_wdev_sdio *nvt_wdev_sdio = nvt_bus->type.nvt_wdev_sdio;

        return nvt_sdio_pm_suspend(nvt_wdev_sdio->dev);
}

s32 nvt_sdio_wa_resume(struct _nvt_bus *nvt_bus)
{
        struct _nvt_wdev_sdio *nvt_wdev_sdio = nvt_bus->type.nvt_wdev_sdio;

        return nvt_sdio_pm_resume(nvt_wdev_sdio->dev);
}
//nash:work around:end

#else
/* 20151113: nash:work around because nt96660 mmc host driver suspend/resume not
    ready yet
 */
s32 nvt_sdio_wa_suspend(struct _nvt_bus *nvt_bus)
{
        return -1;
}

s32 nvt_sdio_wa_resume(struct _nvt_bus *nvt_bus)
{
        return -1;
}
//nash:work around:end
#endif /* CONFIG_PM_SLEEP */

static s32 nvt_sdio_probe(struct sdio_func *func,
                                const struct sdio_device_id *id)
{
        struct _nvt_wdev_sdio *nvt_wdev_sdio;
        struct _nvt_bus *nvt_bus = NULL;
        s32 ret;

        nvt_dbg(INFO, "%s: enter\n", __func__);
        nvt_dbg(INFO, "%s: Class=%x\n", __func__, func->class);
        nvt_dbg(INFO, "%s: sdio vendor ID: 0x%04x\n", __func__, func->vendor);
        nvt_dbg(INFO, "%s: sdio device ID: 0x%04x\n", __func__, func->device);
        nvt_dbg(INFO, "%s: Function#: %d\n", __func__, func->num);

        //nash:TODO
        /* Function number check */

        nvt_wdev_sdio = kmalloc(sizeof(struct _nvt_wdev_sdio), GFP_KERNEL);
        if (nvt_wdev_sdio == NULL) {
                nvt_dbg(ERROR, "%s: alloc nvt_wdev_sdio fail\n", __func__);
                ret = -ENOMEM;
                goto alloc_failed;
        }

        nvt_wdev_sdio->func[0] = func->card->sdio_func[0];
        nvt_wdev_sdio->func[1] = func;
        nvt_wdev_sdio->dev = &nvt_wdev_sdio->func[1]->dev;

        ret = nvt_sdio_attach_bus(nvt_wdev_sdio);
        if (ret) {
                nvt_dbg(ERROR, "%s: nvt_sdio_attach_bus fail(err:%d)\n",
                        __func__, ret);
                goto dscr_err;
        }

        nvt_bus = dev_get_drvdata(nvt_wdev_sdio->dev);
        //20151218 nash:  coverity#48948
        if (nvt_bus == NULL) {
                goto dscr_err;
        }

        ret = nvt_fw_download(nvt_bus, NVT_FW_DEFAULT_FW_MODE);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: fw download fail\r\n", __func__);
                goto attach_bus_err;
        }

        ret = nvt_register_to_system(nvt_wdev_sdio->dev);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: nvt_register_to_system fail(%d)\n",
                        __func__, ret);
                goto attach_bus_err;
        }

        /* get firmware capability */
        nvt_set_firmware_capability(nvt_bus->nvt_adapter);

        /* Intialized fw configuration by WIDs */
        nvt_init_fw_by_wids(nvt_bus, NL80211_IFTYPE_STATION);
        return 0;

attach_bus_err:
        nvt_diag_delete_node(nvt_bus);
        nvt_bus->nvt_wdev_bus_ops.stop(nvt_bus);

dscr_err:
        kfree(nvt_wdev_sdio);

alloc_failed:
        return ret;
}

static void nvt_sdio_remove(struct sdio_func *func)
{
        struct _nvt_wdev_sdio *nvt_wdev_sdio;
        struct _nvt_bus *nvt_bus;
        struct _nvt_adapter *nvt_adapter;

        nvt_dbg(INFO, "%s: enter\n", __func__);

        nvt_bus = dev_get_drvdata(&func->dev);
        if (nvt_bus == NULL) {
                nvt_dbg(INFO, "%s: nvt_bus is NULL\n", __func__);
                return;
        }

        nvt_wdev_sdio = nvt_bus->type.nvt_wdev_sdio;

        nvt_adapter = nvt_bus->nvt_adapter;

        nvt_diag_delete_node(nvt_bus);

        if (nvt_bus->nvt_adapter != NULL) {
                nvt_del_all_if(nvt_bus->nvt_adapter);
        }
        nvt_bus->nvt_wdev_bus_ops.stop(nvt_bus);

        nvt_bus_detach(nvt_wdev_sdio->dev);

        kfree(nvt_wdev_sdio);

        kfree(nvt_adapter);

}

static struct sdio_driver nvtfmac_sdio_dvr = {
        .name = "nvtwifi_sdio",
        .probe = nvt_sdio_probe,
        .remove = nvt_sdio_remove,
        .id_table = nvt_sdio_supp_table,

#ifdef CONFIG_PM_SLEEP
        .drv = {
                .pm = &nvt_sdio_pm_ops,
        },
#endif /* CONFIG_PM_SLEEP */
};

void nvtfmac_sdio_init(void)
{
        nvt_dbg(INFO, "%s:\n", __func__);
        sdio_register_driver(&nvtfmac_sdio_dvr);
}

void nvtfmac_sdio_exit(void)
{
        nvt_dbg(INFO, "%s:\n", __func__);
        sdio_unregister_driver(&nvtfmac_sdio_dvr);
}

