#include "nvt_bus.h"
#include "nvt_util_dbg.h"
#include "nvt_icfg.h"

/**
 * nvt_bus_txdata - send tx skb to bus
 * @nvt_bus: bus structrue
 * @skb: tx skb buffer
 *
 * this function will call bus tx_data ops to send it to bus.
 *
 * Return: zero - success
 *         negative - failure
 */
s32 nvt_bus_txdata(struct _nvt_bus *nvt_bus, struct sk_buff *skb)
{
        s32 err = 0;

#ifdef BUS_UTILIZATION
        //do not accept any tx data when we are in bus utilization mode
        if (nvt_bus->nvt_bu->tx_enable) {
                return -1;
        }
#endif
        err = nvt_bus->nvt_wdev_bus_ops.tx_data(nvt_bus, skb);

        return err;
}

/**
 * nvt_bus_attach - nvt bus allocation and initialization
 * @dev - set nvt_bus to this device
 * @bus_ops - nvt_bus operation table
 *
 * Return: a nvt_bus pointer
 */
struct _nvt_bus *nvt_bus_attach(struct device *dev,
        struct _nvt_wdev_bus_ops bus_ops)
{
        s32 ret = 0;
        struct _nvt_bus *nvt_bus;
        nvt_bus = kzalloc(sizeof(struct _nvt_bus), GFP_ATOMIC);
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: Alloc nvt_bus failed\n", __func__);
                ret = -ENOMEM;
                goto alloc_failed;
        }

        nvt_bus->icfg_pkg_buff = kzalloc(sizeof(struct _nvt_icfg_pkt_buff),
                GFP_ATOMIC);
        mutex_init(&nvt_bus->icfg_pkg_buff->iconfig_mutex);

        nvt_bus->dev = dev;
        nvt_bus->state = NVT_BUS_STATE_DOWN;
        dev_set_drvdata(dev, nvt_bus);
        nvt_bus->nvt_wdev_bus_ops = bus_ops;

        init_waitqueue_head(&nvt_bus->fw_rdy_wait);
        nvt_bus->fw_rdy_completed = 0;

        //20160122 nash: bus statistic
        nvt_bus->statistic.pkt_tx_cnt = 0;
        nvt_bus->statistic.pkt_rx_cnt = 0;
        nvt_bus->statistic.pkt_tx_err_cnt = 0;
        nvt_bus->statistic.pkt_rx_err_cnt = 0;

        nvt_bus->statistic.ctl_tx_cnt = 0;
        nvt_bus->statistic.ctl_rx_cnt = 0;
        nvt_bus->statistic.ctl_tx_err_cnt = 0;
        nvt_bus->statistic.ctl_rx_err_cnt = 0;

#ifdef BUS_UTILIZATION
        nvt_bus->nvt_bu = kzalloc(sizeof(struct _nvt_bus_utilization),
                        GFP_ATOMIC);
        if (!nvt_bus->nvt_bu) {
                nvt_dbg(ERROR, "%s: bus utilization alloc fail\n", __func__);
                kfree(nvt_bus);
                return NULL;
        }

        nvt_bus->nvt_bu->tx_enable = false;
        nvt_bus->nvt_bu->rx_enable = false;
        nvt_bus->nvt_bu->tx_total_cnt = 0;
        nvt_bus->nvt_bu->tx_runtime_cnt = 0;
        nvt_bus->nvt_bu->rx_total_cnt = 0;
        nvt_bus->nvt_bu->rx_runtime_cnt = 0;
        nvt_bus->nvt_bu->start_time = 0;
        nvt_bus->nvt_bu->end_time = 0;
        nvt_bus->nvt_bu->total_pkt_size_in_byte = 0;
        nvt_bus->nvt_bu->tx_mode = NVT_BUS_BU_TX_VARIABLE_LEN;
#endif

        return nvt_bus;
alloc_failed:
        return NULL;
}

/**
 * nvt_bus_detach - free nvt_bus
 * @dev - get nvt_bus to this device
 *
 *
 * Return: 0 for success, a negative value for fail
 */
s32 nvt_bus_detach(struct device *dev)
{
        struct _nvt_bus *nvt_bus = dev_get_drvdata(dev);
        kfree(nvt_bus);
        return 0;
}

/**
 * nvt_bus_register_txdone_callback - register your own TX done callback
 * function
 * @nvt_bus: nvt_bus structure
 * @txdone_cb: callback function pointer
 *
 * Register your own TX done callback function by this function. When TX done,
 * your callback would be executed
 *
 * Return: 0:success, -1:fail
 */
s32 nvt_bus_register_txdone_callback(struct _nvt_bus *nvt_bus,
        void (*txdone_cb)(struct sk_buff *, s32 status_code))
{
        if (nvt_bus == NULL) {
                return -1;
        }

        nvt_bus->tx_done_callback = txdone_cb;
        return 0;
}

#ifdef BUS_UTILIZATION
/**
 * nvt_bus_bu_tx_on - control TX bus utilization on or off
 * @nvt_bus: nvt bus structure
 * @on: true:on, false:off
 * @pkt_cnt: packet number for transmit
 *
 * Return: 0:success, -1:fail
 */
s32 nvt_bus_bu_tx_on(struct _nvt_bus *nvt_bus, bool on, u32 pkt_cnt)
{
        if (!nvt_bus) {
                return -1;
        }

        if (on) {
                nvt_bus->nvt_bu->tx_total_cnt = pkt_cnt;
                nvt_bus->nvt_bu->tx_runtime_cnt = 0;
                nvt_bus->nvt_bu->start_time = get_jiffies_64();
                nvt_bus->nvt_bu->total_pkt_size_in_byte = 0;
                nvt_bus->nvt_bu->tx_enable = true;
        } else {
                nvt_bus->nvt_bu->tx_enable = false;
                nvt_bus->nvt_bu->tx_total_cnt = 0;
        }

        return 0;
}

/**
 * nvt_bus_bu_rx_on - control RX bus utilization on or off
 * @nvt_bus: nvt bus structure
 * @on: true:on, false:off
 * @pkt_cnt: packet number for transmit
 *
 * Return: 0:success, -1:fail
 */
s32 nvt_bus_bu_rx_on(struct _nvt_bus *nvt_bus, bool on, u32 pkt_cnt)
{
        if (!nvt_bus) {
                return -1;
        }

        if (on) {
                nvt_bus->nvt_bu->rx_total_cnt = pkt_cnt;
                nvt_bus->nvt_bu->rx_runtime_cnt = 0;
                nvt_bus->nvt_bu->start_time = 0;
                nvt_bus->nvt_bu->total_pkt_size_in_byte = 0;
                nvt_bus->nvt_bu->rx_enable = true;
        } else {
                nvt_bus->nvt_bu->rx_enable = false;
                nvt_bus->nvt_bu->rx_total_cnt = 0;
        }

        return 0;

}

/**
 * nvt_bus_bu_show_result - show bus utilization result for both TX/RX
 * @nvt_bus: nvt bus structure
 * @is_tx: true:TX, false:RX
 *
 * Return: none
 */
void nvt_bus_bu_show_result(struct _nvt_bus *nvt_bus, bool is_tx)
{
        u64 interval = 0;
        u64 bu_rate = 0;
        u8 *str_tbl[2] = {"TX", "RX"};
        u8 *str;
        s32 pkt_cnt = 0;
        u32 ret_val;

        if (!nvt_bus) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL!!\n", __func__);
                return;
        }

        nvt_bus->nvt_bu->end_time = get_jiffies_64();
        interval = nvt_bus->nvt_bu->end_time - nvt_bus->nvt_bu->start_time;

        if (is_tx) {
                nvt_bus->nvt_bu->tx_enable = false;
                str = str_tbl[0];
                pkt_cnt = nvt_bus->nvt_bu->tx_runtime_cnt;
        } else {
                nvt_bus->nvt_bu->rx_enable = false;
                str = str_tbl[1];
                pkt_cnt = nvt_bus->nvt_bu->rx_runtime_cnt;
        }

        nvt_dbg(INFO, "\n[BU]: %s %d packets took %llu jiffies\n",
                str, pkt_cnt, interval);

        nvt_dbg(INFO, "[BU]: total pkt size=%llu\n",
                nvt_bus->nvt_bu->total_pkt_size_in_byte);

        bu_rate = nvt_bus->nvt_bu->total_pkt_size_in_byte * HZ;
        if (interval > 0) {
                ret_val = do_div(bu_rate, interval);
                bu_rate *= 8;
                nvt_dbg(INFO, "[BU]: SDIO %s bus utilization: %llu bps\n",
                        str, bu_rate);
        }

        return;
}

#endif

