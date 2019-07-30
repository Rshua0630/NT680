#ifndef _NVTDEVINFO_H
#define _NVTDEVINFO_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtdevinfo.h

    @version

    @date
*/
#include <stdio.h>
#include <nvt_type.h>
#include <nvtcfg_define.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Config functions (PStore name length must less than 12) */
#define CFG_CATEGORY_NUM        13
#define CFG_DEV_INFO_NAME       "devInfo"
#define CFG_USER_LIST_NAME      "userList"
#define CFG_NETWORK_INFO_NAME   "networkInfo"
#define CFG_DEV_DISCOVER_NAME   "devDiscover"
#define CFG_EVENT_INFO_NAME     "eventInfo"
#define CFG_MEDIA_INFO_NAME     "mediaInfo"
#define CFG_STREAM_INFO_NAME    "streamInfo"
#define CFG_PTZ_INFO_NAME       "ptzInfo"
#define CFG_IMAGE_INFO_NAME     "imageInfo"
#define CFG_DATETIME_INFO_NAME  "dateTime"
#define CFG_DETECTION_INFO_NAME "detectInfo"
#define CFG_EVENT_CONFIG_NAME   "eventConfig"
#define CFG_RECORD_CONFIG_NAME  "recConfig"

#define CFG_NAME_LENGTH_MAX     PS_SEC_NAME_MAX_LEN
#define CFG_All_CONFIG_NAME     "allConfig"
/* Only used in reset and save config (soft reset) */
#define CFG_EXCLUDE_NETCFG_NAME "excludeNetCfg"


NVTRET NvtDevInfo_UserLevel_Set(enum tt__UserLevel newLevel);

void *NvtDevInfo_Cfg_SHMAttach(const char *cfgName);
NVTRET NvtDevInfo_Cfg_SHMDeAttach(void *shmAddr);
NVTRET NvtDevInfo_Cfg_Reset(const char *cfgName);
NVTRET NvtDevInfo_Cfg_WaitCmdFinish(const char *cfgName);
NVTRET NvtDevInfo_Cfg_Save(const char *cfgName);
NVTRET NvtDevInfo_Cfg_Backup(const char *fileName);
NVTRET NvtDevInfo_Cfg_Restore(const char *fileName);

/* Only called by nvtsystem */
NVTRET NvtDevInfo_Cfg_Init(void);
NVTRET NvtDevInfo_Cfg_ClearCmdRecord(const char *pidStr, const char *cfgName);
NVTRET NvtDevInfo_Cfg_Release(void);

NVTRET NvtDevInfo_Cfg_RealSave(const char *cfgName);

/* ID functions */
#define ID_ITEM_LEN_MAX     128
#define ID_MANUFACTURER     "manufacturer"
#define ID_MODEL            "model"
#define ID_SERIAL_NUMBER    "serialnumber"
#define ID_HARDWARE_ID      "hwid"
#define ID_ETH_MAC          "eth_MAC"

NVTRET NvtDevInfo_ID_Init(void);
NVTRET NvtDevInfo_ID_Release(void);
NVTRET NvtDevInfo_ID_Reset(void);
NVTRET NvtDevInfo_ID_Save(void);
NVTRET NvtDevInfo_ID_Find(const char name[ID_ITEM_LEN_MAX], char value[ID_ITEM_LEN_MAX]);
NVTRET NvtDevInfo_ID_List_Reset(void);
NVTRET NvtDevInfo_ID_List_Each(char name[ID_ITEM_LEN_MAX], char value[ID_ITEM_LEN_MAX]);
NVTRET NvtDevInfo_ID_Set(const char name[ID_ITEM_LEN_MAX], const char value[ID_ITEM_LEN_MAX]);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTDEVINFO_H  */
