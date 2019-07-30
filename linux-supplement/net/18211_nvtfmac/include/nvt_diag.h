#ifndef _NVT_DIAG_H_
#define _NVT_DIAG_H_

#include <linux/cdev.h>

#define DIAG_NODE_NAME  "diag"
#define NVT_DIAG_HDR_LEN        4
#define NVT_DIAG_CMD_RESP_OFFSET        8

#define NVT_DIAG_RSP_FLAG       0x80

enum diag_cmd_class {
        NVT_DIAG_CMD_CLASS_0 = 0xC0,
        NVT_DIAG_CMD_CLASS_1 = 0xC1,
        NVT_DIAG_CMD_CLASS_2 = 0xC2,
        NVT_SYS_CMD = 0xFF,
};

enum diag_c0_request {
        NVT_DIAG_WRITE_CMD = 0,
        NVT_DIAG_READ_CMD,
        NVT_DIAG_ECHO_CMD,
        NVT_DIAG_WRITE_LOCK_CMD,
        NVT_DIAG_WRITE_UNLOCK_CMD,
        NVT_DIAG_CHECK_SUM_CMD,
        NVT_DIAG_GET_FW_VER_CMD,
        NVT_DIAG_GET_MAC_ADDR_CMD,
        NVT_DIAG_SET_MAC_ADDR_CMD,
        NVT_DIAG_JUMP_TO_ADDR_CMD,
        NVT_DIAG_REBOOT_CMD,
};

enum diag_c1_request {
        NVT_DIAG_ICONFIG_CMD = 0,
};

enum sys_ff_request {
        SYS_CMD_DEEPSLEEP = 0x00,
        SYS_CMD_WAKEUP,
        SYS_CMD_NUMBER,
};

struct _nvt_diag_req {
        u8 req_class;
        u8 req_cmd;
        u16 req_len;

        union {
                struct {
                        u32 addr;
                        u8  data[1024];
                } write;

                struct {
                        u32 addr;
                        u32 len;
                } read;

                struct {
                        u8 data[1024];
                } echo;

                struct {
                        u8 data[7];
                } wr_locked;

                struct {
                        u8 data[9];
                } wr_unlocked;

                struct {
                        u32 addr;
                        u32 len;
                } cksum_veri;

                struct {
                        u32 boot_type;
                } reboot_cmd;
                struct {
                        u8 data[1024];
                } iconfig_cmd;
        } sel;
};

struct _nvt_diag_resp {
        u8 resp_class;
        u8 resp_cmd;
        u16 resp_len;
        u32 err_code;

        union {
                struct {
                        u32 addr;
                        u8 data[1024];
                } read;

                struct {
                        u8 data[1024];
                } echo;

                struct {
                        u32 cksum_val;
                } cksum_veri;

                struct {
                        u8 fw_ver[4];
                        u8 fw_type[4];
                        u32 fw_chipid;
                } get_fw_ver;

                struct {
                        u8 mac_addr[6];
                } get_mac_addr;

                struct {
                        u8 data[1024];
                } iconfig_resp;


                /* c2 response */
                struct {
                        u8 bandwidth;
                        u8 primary_channel;
                } hut_rx_setting_resp;
        } sel;

};

struct _nvt_sys_req {
        u8 req_class;
        u8 req_cmd;
        u16 req_len;
};

struct _nvt_diag {
        s32 diag_resp_rdy;
        wait_queue_head_t diag_resp_wait;
        s32 major_nm;
        s32 minor_nm;
        dev_t dev_n;
        struct cdev chr_dev;
        struct class *diag_class;
        struct device *diag_device;
        void *private_data;

};

struct _nvt_bus;
extern s32 nvt_diag_create_node(struct _nvt_bus *bus);
extern s32 nvt_diag_delete_node(void *data);
extern s32 nvt_diag_pack_req(u8 cmd_class, u8 type, u32 addr,
                u32 data_len, u8 *buf, struct _nvt_diag_req *diag_req,
                s32 *pkt_len);
extern s32 nvt_diag_rsp_header_check(struct _nvt_diag_resp *diag_resp);

#endif
