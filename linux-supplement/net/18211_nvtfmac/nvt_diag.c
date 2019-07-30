#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <linux/netdevice.h>
#include <linux/sched.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/card.h>

#include <asm/uaccess.h>

#include "nvt_bus.h"
#include "nvt_diag.h"
#include "nvt_util_dbg.h"

static int ref_count;


/**
 * nvt_diag_read -read operation for diagnostic device node
 *
 *
 */
ssize_t nvt_diag_read(struct file *filp, char __user *buf,
                size_t count, loff_t *f_pos)
{
        ssize_t retval = 0;
        struct _nvt_diag_resp *diag_resp;
        //struct _nvt_diag *diag_node = filp->private_data;
        struct _nvt_bus *bus = filp->private_data;

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(DIAG, "%s: kzalloc is failed\n", __func__);
                retval = -1;
                return retval;
        }

        //diag_node->rece_resp((void *)diag_node, diag_resp);
        retval = bus->nvt_wdev_bus_ops.rx_ctrl(bus, (u8 *)diag_resp,
                        sizeof(struct _nvt_diag_resp));
        if (retval < 0) {
                nvt_dbg(DIAG, "%s: failed to recv diag resp!!\n", __func__);
                kfree(diag_resp);
                return retval;
        }

        if (copy_to_user(buf, (void *)diag_resp,
                        (diag_resp->resp_len + NVT_DIAG_HDR_LEN))) {
                nvt_dbg(DIAG, "%s: copy err code to user failed\n", __func__);
                //20151218 nash: coverity#48965
                kfree(diag_resp);
                return -1;
        }

        kfree(diag_resp);

        return 0;
}

/**
 * nvt_diag_write -write operation for diagnostic device node
 *
 *
 */
ssize_t nvt_diag_write(struct file *filp, const char __user *buf,
                size_t count, loff_t *f_pos)
{
        u8 *cfu_buf;
        ssize_t retval = 0;
        struct _nvt_bus *bus = filp->private_data;

        cfu_buf = kzalloc(count, GFP_KERNEL);

        if (cfu_buf == NULL) {
                nvt_dbg(DIAG, "%s: kzalloc is failed\n", __func__);
                retval = -1;
                goto alloc_cfu_err;
        }

        if (copy_from_user(cfu_buf, buf, count)) {
                nvt_dbg(DIAG, "%s: copy_from_user is failed\n", __func__);
                retval = -1;
                goto cfu_err;
        }

        if (*cfu_buf == NVT_DIAG_CMD_CLASS_0 ||
                        *cfu_buf == NVT_DIAG_CMD_CLASS_1 ||
                        *cfu_buf == NVT_DIAG_CMD_CLASS_2) {
                bus->nvt_wdev_bus_ops.tx_ctrl(bus, cfu_buf, count);
        } else {
                nvt_dbg(DIAG, "%s: Unsupported!(Data:0x%08x)\n", __func__,
                                *((s32 *)(cfu_buf)));
        }

cfu_err:
        kfree(cfu_buf);
alloc_cfu_err:
        return retval;
}

/**
 * nvt_diag_open() -open operation for diagnostic device node
 *
 *
 */
int nvt_diag_open(struct inode *inode, struct file *filp)
{
        struct _nvt_diag *diag_node;

        ref_count++;

        diag_node = container_of(inode->i_cdev, struct _nvt_diag, chr_dev);

        /* private_data is _nvt_bus */
        filp->private_data = diag_node->private_data;
        nvt_dbg(DIAG, "Major num:%d\n", diag_node->major_nm);

        return 0;
}

/**
 * nvt_diag_release() -release operation for diagnostic device node
 *
 *
 */
int nvt_diag_release(struct inode *inode, struct file *filp)
{
        ref_count--;
        return 0;
}

const struct file_operations diag_fops = {
        .owner = THIS_MODULE,
        .read = nvt_diag_read,
        .write = nvt_diag_write,
        .open = nvt_diag_open,
};

/**
 * nvt_diag_create_node - create a node for diagnostic
 * @bus_if: bus interface
 *
 * Return: 0:success, negative value:fail
 */
int nvt_diag_create_node(struct _nvt_bus *bus_if)
{
        int ret;
        dev_t dev;
        struct _nvt_diag *diag_node;

        diag_node = &bus_if->nvt_diag;
        diag_node->private_data = (void *)bus_if;

        init_waitqueue_head(&diag_node->diag_resp_wait);

        nvt_dbg(DIAG, "%s: diag_node=%p\n", __func__, diag_node);

        diag_node->diag_class = class_create(THIS_MODULE, "diag");
        if (IS_ERR(diag_node->diag_class)) {
                nvt_dbg(DIAG, "%s: diag class_create is failed\n", __func__);
                ret = -1;
                goto class_create_failed;
        }

        ret = alloc_chrdev_region(&diag_node->dev_n, 0, 1, DIAG_NODE_NAME);
        if (ret < 0) {
                nvt_dbg(DIAG, "%s: alloc_chrdev_region is failed(err:0x%x)\n",
                        __func__, ret);
                goto alloc_chrdev_region_failed;
        }

        diag_node->major_nm = MAJOR(diag_node->dev_n);
        diag_node->minor_nm = MINOR(diag_node->dev_n);

        cdev_init(&diag_node->chr_dev, &diag_fops);
        ret = cdev_add(&diag_node->chr_dev, diag_node->dev_n, 1);
        if (ret < 0) {
                nvt_dbg(DIAG, "%s: cdev_add is failed (err:0x%x)\n",
                        __func__, ret);
                goto cdev_add_failed;
        }

        dev = MKDEV(diag_node->major_nm, diag_node->minor_nm);
        diag_node->diag_device = device_create(diag_node->diag_class, NULL,
                        dev, NULL, "diag%d", diag_node->minor_nm);
        if (IS_ERR(diag_node->diag_device))
                goto device_create_failed;


        nvt_dbg(DIAG, "%s: Major num for Diag:%d\n",
                __func__, diag_node->major_nm);
        return 0;

device_create_failed:
        cdev_del(&diag_node->chr_dev);
cdev_add_failed:
        unregister_chrdev_region(diag_node->dev_n, 1);
alloc_chrdev_region_failed:
        class_destroy(diag_node->diag_class);
class_create_failed:
        return ret;

}

/**
 * nvt_diag_delete_node - delete the diagnostic device node
 * @data: struct _nvt_bus *
 *
 * Return: 0:success, negative value:fail
 */
 int nvt_diag_delete_node(void *data)
 {
        struct _nvt_diag *diag_node;
        dev_t dev;

        struct _nvt_bus *bus = (struct _nvt_bus *)(data);
        diag_node = &bus->nvt_diag;
        dev = MKDEV(diag_node->major_nm, diag_node->minor_nm);

        device_destroy(diag_node->diag_class, dev);
        cdev_del(&diag_node->chr_dev);
        unregister_chrdev_region(diag_node->dev_n , 1);
        class_destroy(diag_node->diag_class);
        nvt_dbg(DIAG, "%s: removed diag node\n", __func__);

        return 0;
}

/**
 * nvt_diag_pack_req - pack a diagnostic request for sending
 * @cmd_class: c0, c1, or c2 command class
 * @type: command type
 * @addr: address for memory read/write
 * @data_len: data byte length
 * @buf: data byte buffer
 * @diag_req:
 * @pkt_len: length of returned diagnostic request
 *
 * This function will pack a diagnostic request and put it in diag_req
 *
 * Return: 0:success; -1:fail
 */
int nvt_diag_pack_req(u8 cmd_class, u8 type, u32 addr, u32 data_len,
                u8 *buf, struct _nvt_diag_req *diag_req, s32 *pkt_len)
{
        diag_req->req_class = cmd_class;

        nvt_dbg(DIAG, "%s: cmd_class=%x\n", __func__, cmd_class);

        if (cmd_class == NVT_DIAG_CMD_CLASS_0) {

                switch (type) {
                case NVT_DIAG_WRITE_CMD:
                        nvt_dbg(DIAG, "%s: write cmd\n", __func__);
                        diag_req->req_cmd = NVT_DIAG_WRITE_CMD;
                        diag_req->req_len = (u16)(data_len + NVT_DIAG_HDR_LEN);
                        diag_req->sel.write.addr = (u32)addr;
                        if (buf && data_len > 0) {
                                memcpy(diag_req->sel.write.data, buf, data_len);
                        }

                        *pkt_len = NVT_DIAG_HDR_LEN +
                                sizeof(diag_req->sel.write.addr) +
                                data_len;
                        nvt_dbg(DIAG, "pkt_len=%x\r\n", *pkt_len);
                        break;
                case NVT_DIAG_READ_CMD:
                        nvt_dbg(DIAG, "%s: read cmd\n", __func__);
                        diag_req->req_cmd = NVT_DIAG_READ_CMD;
                        diag_req->req_len = sizeof(diag_req->sel.read);
                        diag_req->sel.read.addr = (u32)addr;
                        diag_req->sel.read.len = (u32)data_len;
                        *pkt_len = NVT_DIAG_HDR_LEN + diag_req->req_len;
                        break;
                case NVT_DIAG_ECHO_CMD:
                        nvt_dbg(DIAG, "%s: echo cmd\n", __func__);
                        diag_req->req_cmd = NVT_DIAG_ECHO_CMD;
                        diag_req->req_len = (u32)data_len;
                        if (buf && data_len > 0)
                                memcpy(diag_req->sel.echo.data, buf, data_len);

                        *pkt_len = NVT_DIAG_HDR_LEN + data_len;
                        break;
                case NVT_DIAG_CHECK_SUM_CMD:
                        nvt_dbg(DIAG, "%s: crc cmd\n", __func__);
                        diag_req->req_cmd = NVT_DIAG_CHECK_SUM_CMD;
                        diag_req->req_len = sizeof(diag_req->sel.cksum_veri);
                        diag_req->sel.cksum_veri.addr = (u32)addr;
                        diag_req->sel.cksum_veri.len = (u32)data_len;
                        *pkt_len = NVT_DIAG_HDR_LEN + diag_req->req_len;
                        break;
                case NVT_DIAG_GET_FW_VER_CMD:
                        nvt_dbg(DIAG, "%s: ver cmd\n", __func__);
                        diag_req->req_cmd = NVT_DIAG_GET_FW_VER_CMD;
                        diag_req->req_len = 0;
                        *pkt_len = NVT_DIAG_HDR_LEN;
                        break;
                case NVT_DIAG_GET_MAC_ADDR_CMD:
                        nvt_dbg(DIAG, "%s: get mac addr cmd\n", __func__);
                        diag_req->req_cmd = NVT_DIAG_GET_MAC_ADDR_CMD;
                        diag_req->req_len = 0;
                        *pkt_len = NVT_DIAG_HDR_LEN;
                        break;
                case NVT_DIAG_REBOOT_CMD:
                        nvt_dbg(DIAG, "%s: reboot cmd\n", __func__);
                        diag_req->req_cmd = NVT_DIAG_REBOOT_CMD;
                        diag_req->req_len = sizeof(diag_req->sel.reboot_cmd);
                        if (buf && data_len > 0) {
                                memcpy((u8 *)&(diag_req->sel.reboot_cmd.
                                        boot_type), buf, data_len);
                        }
                        *pkt_len = NVT_DIAG_HDR_LEN + diag_req->req_len;
                        break;
                default:
                        return -1;
                }

        } else if (cmd_class == NVT_DIAG_CMD_CLASS_1) {

                switch (type) {
                case NVT_DIAG_ICONFIG_CMD:
                        diag_req->req_cmd = NVT_DIAG_ICONFIG_CMD;
                        diag_req->req_len = (u16)data_len;
                        if (buf && data_len > 0) {
                                memcpy(diag_req->sel.iconfig_cmd.data,
                                                buf, data_len);
                        }
                        *pkt_len = NVT_DIAG_HDR_LEN + data_len;
                        break;
                default:
                        return -1;
                }
        }

        return 0;
}


/**
 * nvt_diag_rsp_header_check - diag response header check
 * @diag_resp:
 *
 * diag response header check
 *
 * Return: 0:success; -1:fail
 */
s32 nvt_diag_rsp_header_check(struct _nvt_diag_resp *diag_resp)
{
        if (diag_resp == NULL) {
                return -1;
        }

        if (diag_resp->resp_class != NVT_DIAG_CMD_CLASS_0 &&
                diag_resp->resp_class != NVT_DIAG_CMD_CLASS_1 &&
                diag_resp->resp_class != NVT_DIAG_CMD_CLASS_2) {
                nvt_dbg(ERROR, "%s: class error!!\n", __func__);
                return -2;
        }

        if ((diag_resp->resp_cmd & NVT_DIAG_RSP_FLAG) != NVT_DIAG_RSP_FLAG) {
                nvt_dbg(ERROR, "%s: cmd error!!\n", __func__);
                return -3;
        }

        return 0;
}

