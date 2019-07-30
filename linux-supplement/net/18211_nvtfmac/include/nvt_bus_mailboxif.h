#ifndef __NVT_MAILBOX_H
#define __NVT_MAILBOX_H
#include <linux/interrupt.h>
#include "nvt_bus.h"

//extern void nvtfmac_mailbox_init(void);
//extern void nvtfmac_mailbox_exit(void);

//TBD, shall be defined in common header file
#define EC_SUCCESS                  1
#define EC_FAIL                     0
#define TRUE                        1
#define FALSE                       0
#define debug_msg(x, y)

#define MAILBOX_DESTROYED_STATE     0
#define MAILBOX_ACTIVE_STATE        1
#define MAILBOX_SUSPEND_STATE       2

#define IS_MAILBOX_ACTIVE(bus)     (bus->type.wdev_mailbox.suspend_state \
                                        == MAILBOX_ACTIVE_STATE)

//TBD, write interrupt mask register to control interrupt
#define ENABLE_MAILBOX_TX_INT      /* write int mask register to enable int */
#define DISABLE_MAILBOX_TX_INT     /* write int mask register to disable int */
#define ENABLE_MAILBOX_TXC_INT     /* write int mask register to enable int */
#define DISABLE_MAILBOX_TXC_INT    /* write int mask register to disable int */
#define ENABLE_MAILBOX_RXA_INT     /* write int mask register to enable int */
#define DISABLE_MAILBOX_RXA_INT    /* write int mask register to disable int */
#define ENABLE_MAILBOX_RXC_INT     /* write int mask register to enable int */
#define DISABLE_MAILBOX_RXC_INT    /* write int mask register to disable int */
#define ENABLE_MAILBOX_CMD_INT     /* write int mask register to enable int */
#define DISABLE_MAILBOX_CMD_INT    /* write int mask register to disable int */
#define ENABLE_MAILBOX_RSP_INT     /* write int mask register to enable int */
#define DISABLE_MAILBOX_RSP_INT    /* write int mask register to disable int */

#define MINIMAL_HEADROOM_SIZE      36 /*
                                       * TBD minimal headroom required for fw
                                       * to grow WLAN header in the buffer
                                       * WLAN header(30 for HT)
                                       * + LLC(8)
                                       * + host header (8)
                                       * -Ethernet header (14)
                                       */
#define MAX_RSP_BUF_LEN            256
#define MAX_RX_BUFFER_LEN          1800
#define TOTAL_NUM_OF_QUEUE         6;

//Common MACRO---------------------------------------------------

//debug flag for software mailbox for shared-memory interface
#define DBG_FLAG_SW_MAILBOX 0x1000

//OWN bit definitions, 1 for host, 0 for FW
#define OWN_BY_HOST 1
#define OWN_BY_FW 0

//Queue depth definitions
#define SW_MAILBOX_TX_Q_DEPTH   128
#define SW_MAILBOX_TXC_Q_DEPTH  128
#define SW_MAILBOX_RXA_Q_DEPTH  128
#define SW_MAILBOX_RXC_Q_DEPTH  128
#define SW_MAILBOX_CMD_Q_DEPTH  32
#define SW_MAILBOX_RSP_Q_DEPTH  32

//end of common MACRO---------------------------------------------------

//Depends on host or FW, defined as OWN_BY_HOST for host and OWN_BY_FW for FW.
#define MY_OWN OWN_BY_HOST
#define NOT_MY_OWN (MY_OWN ^ 1)

struct _nvt_bus;

//Call back function in BUS layer

//OUT-data transfer completed (list of skb)
extern void hif_out_data_complete(struct _nvt_bus *bus,
    struct sk_buff_head *skb_list);

//OUT-ctrl transfer completed
extern void hif_out_ctrl_complete(struct _nvt_bus *bus, u8 *data,
        u32 data_len);

//interface suspend completed
extern void hif_suspend_complete(struct _nvt_bus *bus);

//interface resume completed
extern void hif_resume_complete(struct _nvt_bus *bus);

//IN-data transfer completed (list of skb)
extern void hif_in_data_complete(struct _nvt_bus *bus,
        struct sk_buff_head *skb_list);

//IN-ctrl transfer completed
extern void hif_in_ctrl_complete(struct _nvt_bus *bus, u8 *data, u32 data_len);

//Descriptor data structure
struct mailbox_desc {
    struct {
        u32 Own:1; /* 1:owned by host, 0:owned by FW */
        /* reserved to support scatter-gather, first buffer */
        u32 ReservedFirst:1;
        /* reserved to support scatter-gather, last buffer */
        u32 ReservedLast:1;
        u32 Reserved:29;
    } Word0;
    struct {
        u32 Reserved:8;
        u32 Offset:8; /* offset of data in the buffer */
        u32 BufferLen:16; /* length of buffer, max 65K */
    } Word1;
    u32 BufferAddr;  /* address to the buffer */
    u32 HostBufDesc; /*
                      * address to buffer descriptor on host,
                      * used by host to store OS specific buffer descriptor,
                      * transparent to FW, FW MUST not modify this.
                      */
};

//Common structure:Queue data structure
struct mailbox_q {
    //head index
    u32 head_id;
    //tail index
    u32 tail_id;
    //pointer mailbox ring
    struct mailbox_desc *desc;
    //queue depth
    u32 q_depth;
    //function to trigger interrupt
    //VOID_CBFN cbfn_interrupt;
    //spinlock
    spinlock_t spinlock;
};

//Data structure
struct _nvt_wdev_mailbox {
    s32 suspend_state; /* state of HIL, active or suspend */
    dma_addr_t qphyaddr;
    u8 *qviraddr;

    struct mailbox_q sw_mailbox_tx_q;  /* mailbox for tx buffer */
    struct mailbox_q sw_mailbox_txc_q; /* mailbox for tx complete buffer */
    struct mailbox_q sw_mailbox_rxa_q; /* mailbox for new rx allocated buffer */
    struct mailbox_q sw_mailbox_rxc_q; /* mailbox for rx complete buffer */
    struct mailbox_q sw_mailbox_cmd_q; /* mailbox for WID command */
    struct mailbox_q sw_mailbox_rsp_q; /* mailbox for WID response and event */

    //Tasklet
    struct tasklet_struct txc_tasklet;
    struct tasklet_struct rxc_tasklet;
    struct tasklet_struct cmd_tasklet;
    struct tasklet_struct rsp_tasklet;

    s32 refillnum; /* number of rx buffer to be refilled */
};

#endif /* __NVT_MAILBOX_H */
