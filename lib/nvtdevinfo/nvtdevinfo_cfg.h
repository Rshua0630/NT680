#ifndef _NVTDEVINFO_CFG_H
#define _NVTDEVINFO_CFG_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtcfg.h

    @version

    @date
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "nvt_type.h"
#ifdef __cplusplus
extern "C" {
#endif

#define CFG_GROUP_IDX(shmKey)	((shmKey) - CFG_SHM_KEY_DEV_INFO)
#define CFG_GROUP_NUM			(CFG_SHM_KEY_MAX - CFG_SHM_KEY_DEV_INFO)
#define CFG_GROUP_MAX_SIZE		262144 // 256KB

typedef enum {
    CFG_SHM_KEY_DEV_INFO=1331,
    CFG_SHM_KEY_USER_LIST,
    CFG_SHM_KEY_NETWORK_INFO,
    CFG_SHM_KEY_DEV_DISCOVER,
    CFG_SHM_KEY_EVENT_INFO,
    CFG_SHM_KEY_MEDIA_INFO,
    CFG_SHM_KEY_STREAM_INFO,
    CFG_SHM_KEY_PTZ_INFO,
    CFG_SHM_KEY_IMAGE_INFO,
    CFG_SHM_KEY_DATETIME_INFO,
    CFG_SHM_KEY_DETECTION_INFO,
    CFG_SHM_KEY_EVENT_CONFIG,
    CFG_SHM_KEY_RECORD_CONFIG,
    CFG_SHM_KEY_MAX
} CFG_SHM_KEY;

#define CFG_SHM_KEY_SAVE_CMD    23252
#define CFG_SAVE_CMD_LEN_MAX    128
#define CFG_SAVE_CMD_NUM_MAX    32

typedef struct {
    CHAR cVersion[12];
    UINT32 uiCfgSize;
    CHAR cCfgName[16];
    UINT8 uiMD5Checksum[16];
    CHAR cReserved[16];
} CFG_HEADER_PSTORE;

/*
 *   Data save format example:
 *     eth_MAC=00:80:21:4C:51:B5, serial_number=12345678
 *     => eth_MAC@$%00:80:21:4C:51:B5^*&serial_number@$%12345678^*&
 */
#define ID_PAIR_SEPARATOR   "^*&"
#define ID_VALUE_SEPARATOR  "@$%"
#define ID_PSTORE_SIZE_MAX  5120
#define ID_PSTORE_NAME      "identity"

typedef struct {
    UINT32 uiVersion;
    UINT32 uiIDSize;
    CHAR cMD5Checksum[16];
    CHAR cReserved[16];
} ID_HEADER_PSTORE;

typedef struct {
    UINT32 uiBufSize;
    UINT32 uiDataSize;
    UINT32 uiListIdx;
    char *cMemBuf;
} ID_MEM_BUF;

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTDEVINFO_CFG_H  */
