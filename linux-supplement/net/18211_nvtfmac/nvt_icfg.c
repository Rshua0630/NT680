#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/mutex.h>

#include "nvt_bus.h"
#include "nvt_util_dbg.h"
#include "nvt_diag.h"
#include "nvt_icfg.h"

/**
 * nvt_icfg_lock - mutex lock for iconfig
 * @nvt_bus: bus structrue
 *
 * Mutex lock for iconfig procedure, including reset(nvt_icfg_reset), add(
 * nvt_icfg_add), and send(nvt_icfg_send_and_get_resp). Call this
 * function before nvt_icfg_reset
 *
 */
s32 nvt_icfg_lock(struct _nvt_bus *nvt_bus)
{
        mutex_lock(&nvt_bus->icfg_pkg_buff->iconfig_mutex);
        return 1;
}

/**
 * nvt_icfg_unlock - mutex unlock for iconfig
 * @nvt_bus: bus structrue
 *
 * Do this function after calling nvt_icfg_send_and_get_resp
 *
 */
s32 nvt_icfg_unlock(struct _nvt_bus *nvt_bus)
{
        mutex_unlock(&nvt_bus->icfg_pkg_buff->iconfig_mutex);
        return 1;
}

/**
 * nvt_icfg_reset - reset TX buffer for iconfig command
 * @nvt_bus: bus structure
 *
 * When you want to send a iconfig command, you should call this function
 * first for initialization. Then you should call nvt_icfg_add(), and finally
 * call nvt_icfg_send_and_get_resp() to send the command and get response
 *
 * Return: 0:success, <0:fail
 *
 */
s32 nvt_icfg_reset(struct _nvt_bus *nvt_bus)
{
        if (nvt_bus == NULL) {
                return -EFAULT;
        }

        memset((u8 *)nvt_bus->icfg_pkg_buff->msg_body, 0x00,
                NVT_ICFG_MAX_MSG_BODY);
        nvt_bus->icfg_pkg_buff->msg_len[0] = NVT_ICFG_MSG_HDR_LEN;
        nvt_bus->icfg_pkg_buff->msg_len[1] = 0;
        nvt_bus->icfg_pkg_buff->seq = 0;
        nvt_bus->icfg_pkg_buff->cur_index = 0;

        return 0;
}

/**
 * nvt_icfg_add - add a iconfig command
 * @nvt_bus: bus structrue
 * @dir: get or set
 * @wid: command ID
 * @wid_val: command data
 * @size: size of command data
 *
 * The WID message to firmware is little endian. This function would
 * take care the endian of wid, but you need to take care about wid_val
 * yourself. When dir is SET, the parameter, size, should not be 0.
 * When dir is GET. the parameters, wid_val and size, should NULL and 0.
 * You can call this function multiple times before calling
 * nvt_icfg_send_and_get_resp()
 *
 * Return: 0:success, <0:fail
 *
 */
s32 nvt_icfg_add(struct _nvt_bus *nvt_bus, enum nvt_icfg_dir dir, u32 wid,
        u8 *wid_val, u16 size)
{
        s32 i;
        u16 total_msg_len;
        u16 buff_idx;
        u16 body_len;
        u8 check_sum;

        if (nvt_bus == NULL || (dir == NVT_ICFG_SET && wid_val == NULL)) {
                return -EFAULT;
        }

        total_msg_len = 0;
        total_msg_len += nvt_bus->icfg_pkg_buff->msg_len[0];
        total_msg_len += (nvt_bus->icfg_pkg_buff->msg_len[1] << 8);

        buff_idx = nvt_bus->icfg_pkg_buff->cur_index;

        switch (dir) {
        case NVT_ICFG_GET:
                nvt_bus->icfg_pkg_buff->msg_type = 'Q';
                if ((buff_idx + NVT_ICFG_WID_SIZE) > NVT_ICFG_MAX_MSG_BODY) {
                        return -1;
                }
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = (u8)(wid & 0xFF);
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)((wid & 0xFF00) >> 8);
                total_msg_len += NVT_ICFG_WID_SIZE;
                nvt_bus->icfg_pkg_buff->msg_len[0] = total_msg_len & 0x00FF;
                nvt_bus->icfg_pkg_buff->msg_len[1] =
                        (total_msg_len & 0xFF00) >> 8;
                return 0;

        case NVT_ICFG_SET:
                nvt_bus->icfg_pkg_buff->msg_type = 'W';
                break;

        default:
                nvt_dbg(ERROR, "%s[0] error direction\r\n", __func__);
                return -1;
        }


        /* Handling SET */
        /*char wid */
        if (wid < NVT_ICFG_SHORT_WID_START) {
                if ((buff_idx + NVT_ICFG_CHAR_CMD_LEN) >
                        NVT_ICFG_MAX_MSG_BODY) {
                        return -1;
                }
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = (u8)(wid & 0xFF);
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)((wid & 0xFF00) >> 8);

                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        NVT_ICFG_CHAR_WID_LEN;

                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        *wid_val;

                nvt_bus->icfg_pkg_buff->cur_index = buff_idx;

                total_msg_len += NVT_ICFG_CHAR_CMD_LEN;

        /* short wid */
        } else if (wid < NVT_ICFG_INT_WID_START) {
                if ((buff_idx + NVT_ICFG_SHORT_CMD_LEN) >
                      NVT_ICFG_MAX_MSG_BODY) {
                        return -1;
                }
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = (u8)(wid & 0xFF);
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)((wid & 0xFF00) >> 8);

                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        NVT_ICFG_SHORT_WID_LEN;

                //nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = *(wid_val);
                //nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = *(wid_val + 1);
                memcpy(&nvt_bus->icfg_pkg_buff->msg_body[buff_idx], wid_val,
                        NVT_ICFG_SHORT_WID_LEN);

                buff_idx += NVT_ICFG_SHORT_WID_LEN;

                nvt_bus->icfg_pkg_buff->cur_index = buff_idx;

                total_msg_len += NVT_ICFG_SHORT_CMD_LEN;

         /* int wid */
        } else if (wid < NVT_ICFG_STR_WID_START) {
                if ((buff_idx + NVT_ICFG_INT_CMD_LEN) > NVT_ICFG_MAX_MSG_BODY) {
                        return -1;
                }
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)(wid & 0xFF);

                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)((wid & 0xFF00) >> 8);

                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        NVT_ICFG_INT_WID_LEN;

                //nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = *(wid_val);
                //nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = *(wid_val + 1);
                //nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = *(wid_val + 2);
                //nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = *(wid_val + 3);
                memcpy(&nvt_bus->icfg_pkg_buff->msg_body[buff_idx], wid_val,
                        NVT_ICFG_INT_WID_LEN);

                buff_idx += NVT_ICFG_INT_WID_LEN;

                nvt_bus->icfg_pkg_buff->cur_index = buff_idx;

                total_msg_len += NVT_ICFG_INT_CMD_LEN;

        /* string wid */
        } else if (wid < NVT_ICFG_BIN_WID_START) {

                if (size == 0) {
                        body_len = strlen(wid_val);
                } else {
                        body_len = size;
                }

                if ((buff_idx + NVT_ICFG_STR_SET_CMD_LEN + body_len)
                        > NVT_ICFG_MAX_MSG_BODY) {
                        return -1;
                }
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)(wid & 0xFF);
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)((wid & 0xFF00) >> 8);

                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = body_len;

                memcpy(&nvt_bus->icfg_pkg_buff->msg_body[buff_idx],
                        wid_val, body_len);
                buff_idx += body_len;

                total_msg_len += (NVT_ICFG_STR_SET_CMD_LEN + body_len);

                nvt_bus->icfg_pkg_buff->cur_index = buff_idx;
        /* bin wid */
        } else if (wid < NVT_ICFG_MAX_WID_START) {
                if ((buff_idx + NVT_ICFG_BIN_SET_CMD_LEN + size) >
                     NVT_ICFG_MAX_MSG_BODY) {
                        return -1;
                }
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)(wid & 0xFF);
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (u8)((wid & 0xFF00) >> 8);

                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (size & 0x00FF);
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                        (size & 0xFF00) >> 8;

                check_sum = 0;
                for (i = 0; i < size; i++) {
                        nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] =
                                *(wid_val + i);

                        check_sum += *(wid_val + i);
                }
                nvt_bus->icfg_pkg_buff->msg_body[buff_idx++] = check_sum;

                total_msg_len += NVT_ICFG_BIN_SET_CMD_LEN + size;

                nvt_bus->icfg_pkg_buff->cur_index = buff_idx;
        }

        nvt_bus->icfg_pkg_buff->msg_len[0] = total_msg_len & 0x00FF;
        nvt_bus->icfg_pkg_buff->msg_len[1] = (total_msg_len & 0xFF00) >> 8;

        return 0;
}

/**
 * nvt_icfg_send_and_get_resp - send iconfig command and get response
 * @nvt_bus: bus structrue
 * @resp: buffer for response
 * @resp_buf_size: size of the buffer
 *
 * Send the iconfig command and put the response into resp argument
 *
 * Return: the length of response. return a negative value if fail
 *
 */
 s32 nvt_icfg_send_and_get_resp(struct _nvt_bus *nvt_bus,
        u8 *resp, u16 resp_buf_size)
{
        s32 msg_len;
        s32 retval = 0;
        s32 packed_len;

        struct _nvt_diag_req *diag_req = NULL;
        struct _nvt_diag_resp *diag_resp = NULL;

        msg_len =  nvt_bus->icfg_pkg_buff->msg_len[0];
        msg_len +=  nvt_bus->icfg_pkg_buff->msg_len[1] << 8;
        if (msg_len > NVT_ICFG_MAX_MSG_BODY) {
                return -1;
        }

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(ERROR, "%s: kzalloc for diag_req is failed\n",
                        __func__);
                retval = -1;
                goto alloc_failed;
        }

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(DIAG, "%s: kzalloc for diag_resp is failed\n",
                        __func__);
                retval = -1;
                goto alloc_failed;
        }

        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_1, NVT_DIAG_ICONFIG_CMD, 0,
                msg_len, (u8 *)nvt_bus->icfg_pkg_buff, diag_req, &packed_len);

        nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus, (u8 *)diag_req, packed_len);

        if (resp != NULL) {
                /* get diag_resp of iconfig cmd from FW via diagnostic module */
                retval = nvt_bus->nvt_wdev_bus_ops.rx_ctrl(nvt_bus,
                        (u8 *)diag_resp,
                        sizeof(struct _nvt_diag_resp));
                if (retval < 0) {
                        goto alloc_failed;
                }
                retval -= NVT_DIAG_CMD_RESP_OFFSET;
                if (retval > resp_buf_size) {
                        nvt_dbg(ERROR, "%s: resp len > resp len!\n", __func__);
                        retval = -1;
                        goto alloc_failed;
                }
                memcpy(resp, diag_resp->sel.iconfig_resp.data, retval);

        }


alloc_failed:

        nvt_bus->icfg_pkg_buff->cur_index = 0;

        kfree(diag_req);
        kfree(diag_resp);

        return retval;

}
/**
 * nvt_icfg_getwid_msg_len : To know how many bytes in the response
 * buffer
 *
 * @resp_buff : The response buffer
 * @wid : WID
 */
s32 nvt_icfg_get_wid_msg_len(u8 *resp_buff, u32 wid)
{
        u32 wid_len;
        u8 len_field_size;
        u8 i;
        wid_len = 0;
        /*type except BIN*/
        if (wid < NVT_ICFG_BIN_WID_START) {
                len_field_size = 1;
        /* Binary WID */
        } else {
                len_field_size = 2;
        }
        for (i = 0; i < len_field_size; i++) {
                wid_len += ((resp_buff[NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + i]) << (8 * i));
        }
        return wid_len;
}

/**
 * nvt_icfg_get_wid_value : Extract the wid value from the response buffer
 * NOTE: This functio is added for supporting the cfg80211 ops
 * @resp_buff: the response buffer
 * @wid: WID
 * @wid_value: return the value started position
 *
 */
s32 nvt_icfg_get_wid_value(u8 *resp_buff, u32 wid, u8 *wid_value)
{

        if (wid < NVT_ICFG_STR_WID_START) {
                *wid_value = NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1;
        /* String WID */
        } else if (wid < NVT_ICFG_BIN_WID_START) {
                *wid_value = NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1;

        /* Binary WID */
        } else if (wid < NVT_ICFG_MAX_WID_START) {
                *wid_value = NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 2;
        } else {
                return -1;
        }
        return 0;

}
/**
 *nvt_get_wid_value : pack all the five funciotns used to extract the wid_value
 *
 *
 */
s32 nvt_get_wid_value(struct _nvt_bus *nvt_bus, u32 wid, u8 *resp_buff,
        u32 buff_size, u8 *wid_value)
{
        s32 ret_val;
        //20151218 nash: coverity#48923
        ret_val = nvt_icfg_lock(nvt_bus);
        if (ret_val < 0)
                goto fail;

        ret_val = nvt_icfg_reset(nvt_bus);
        if (ret_val < 0) {
                goto fail;
        }
        ret_val = nvt_icfg_add(nvt_bus, NVT_ICFG_GET, wid, NULL, 0);
        if (ret_val < 0) {
                goto fail;
        }
        if (nvt_icfg_send_and_get_resp(nvt_bus, resp_buff, buff_size) < 0) {
                goto fail;
        }
        ret_val = nvt_icfg_get_wid_value(resp_buff, wid, wid_value);
        if (ret_val < 0)
                goto fail;
fail:
        nvt_icfg_unlock(nvt_bus);
        if (ret_val < 0)
                return -1;
        else
                return nvt_icfg_get_wid_msg_len(resp_buff, wid);
}


