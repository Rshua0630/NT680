#ifndef _NVT_SDIOREG_H_
#define _NVT_SDIOREG_H_

#define SDIO_TX_WPTR                                    (0x00000000)
#define SDIO_TX_RPTR                                    (0x00000004)
//20160222 nash:
#define SDIO_TX_CONFIG                                  (0x00000010)
        #define AUTO_TX_PKT_DONE                        (0x01 << 16)
        #define AUTO_RX_PKT_DONE                        (0x01 << 17)

#define SDIO_TX_BD_DEPTH                                (0x00000014)
#define SDIO_RX_WPTR                                    (0x00000020)
#define SDIO_RX_RPTR                                    (0x00000024)
#define SDIO_RX_BD_DEPTH                                (0x00000034)
#define SDIO_HOST2CARD_INT                              (0x00000040)
        #define DIAG_REQ                                (0x1 << 0)
        #define INTO_DEEP_SLEEP                         (0x1 << 1)
        #define TX_PKT_DONE                             (0x1 << 5)
        #define TX_RETRY                                (0x1 << 6)
        #define RX_PKT_DONE                             (0x1 << 21)
#define SDIO_CARD2HOST_INT                              (0x00000044)
        #define DIAG_COMP                               (0x1 << 0)
        //20170203 nash: RX packet done work around
        #define RX_PKT_WORK_ARD                         (0x1 << 1)
        #define PKT_VLD                                 (0x1 << 4)
        #define PKT_TX_SIZE_ERR                         (0x1 << 5)
#define SDIO_HOST2CARD_INT_MASK                         (0x00000050)
        #define DIAG_REQ_MASK                           (0x1 << 0)
        #define INTO_DEEP_SLEEP_MASK                    (0x1 << 1)
        #define WBD_DONE_MASK                           (0x1 << 4)
        #define RBD_DONE_MASK                           (0x1 << 20)
#define SDIO_CARD2HOST_INT_MASK                         (0x00000054)
        #define DIAG_RESP_MASK                          (0x1 << 1)
        #define PKT_VLD_MASK                            (0x1 << 4)
#define SDIO_RX_PORT                                    (0x00000060)
#define SDIO_TX_PORT                                    (0x00000068)
#define SDIO_RESUME                                     (0x00000080)
        #define SDIO_WAKEUP                             (0x1 << 0)
#define SDIO_RESUME_MASK                                (0x00000084)
        #define SDIO_WAKEUP_MASK                        (0x1 << 0)

#define SDIO_ADDR_ZONE                                  (0x000000F0)
        #define SDIO_ADDR_ZONE_DIAG                     3

#define SDIO_DIAG_DATA_ADDR_OFFSET                      (0x0001F000)
#endif
