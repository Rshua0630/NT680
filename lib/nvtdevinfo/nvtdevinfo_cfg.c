/* =================================================================
 *
 *      nvtdevinfo_cfg.c
 *
 *      Device config and inforation library for Novatek NT9666x
 *
 * =================================================================
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <nvtpstore.h>
#include <openssl/md5.h>
#include "nvtdevinfo.h"
#include "nvtdevinfo_cfg.h"
#include "nvtdevinfo_ctrl.h"

/* =================================================================
 *   Default Setting
 * =================================================================
 */

#define DEBUG
#define PRINT_OUTFILE stderr
#define DBG_WRN(fmtstr, args...)  fprintf(PRINT_OUTFILE, "\033[33m%s(): \033[0m" fmtstr,__func__, ##args)

#define DBG_ERR(fmtstr, args...)  fprintf(PRINT_OUTFILE, "\033[31m%s(): \033[0m" fmtstr,__func__, ##args)

#define DBG_DUMP(fmtstr, args...) fprintf(PRINT_OUTFILE, fmtstr, ##args)

#ifndef DEBUG
#define DBG_IND(fmtstr, args...)
#else
#define DBG_IND(fmtstr, args...)  fprintf(PRINT_OUTFILE, "%s(): " fmtstr, __func__, ##args)
#endif

extern ONVIF_DEVICE_INFO gDefaultDeviceInfo;
extern ONVIF_USER gDefaultUserList[USER_MAX_NUM];
extern ONVIF_NETWORK_INFO gDefaultNetworkInfo;
extern ONVIF_DEVICE_DISCOVERY gDefaultDeviceDiscovery;
extern ONVIF_EVENT_INFO gDefaultEventInfo;
extern ONVIF_MEDIA_INFO gDefaultMediaInfo;
extern ONVIF_STREAM_INFO gDefaultStreamInfo;
extern ONVIF_PTZ_CONFIG gDefaultPTZInfo[PTZ_MAX_NUM];
extern ONVIF_IMAGE_INFO gDefaultImageInfo;
extern ONVIF_DATETIME_INFO gDefaultDateTimeInfo;
extern ONVIF_DETECTION_INFO gDefaultDetectionInfo;
extern ONVIF_EVENT_CONFIG gDefaultEventConfig;
extern ONVIF_RECORD_CONFIG gDefaultRecordConfig;

CFG_HEADER_PSTORE gCfgHeader[CFG_GROUP_NUM]={0};
void *defaultSettingPtr[CFG_GROUP_NUM] = {(void *)&gDefaultDeviceInfo,
                                          (void *)&gDefaultUserList,
                                          (void *)&gDefaultNetworkInfo,
                                          (void *)&gDefaultDeviceDiscovery,
                                          (void *)&gDefaultEventInfo,
                                          (void *)&gDefaultMediaInfo,
                                          (void *)&gDefaultStreamInfo,
                                          (void *)&gDefaultPTZInfo,
                                          (void *)&gDefaultImageInfo,
                                          (void *)&gDefaultDateTimeInfo,
                                          (void *)&gDefaultDetectionInfo,
                                          (void *)&gDefaultEventConfig,
                                          (void *)&gDefaultRecordConfig};
unsigned int settingSize[CFG_GROUP_NUM] = {sizeof(gDefaultDeviceInfo),
                                           sizeof(gDefaultUserList),
                                           sizeof(gDefaultNetworkInfo),
                                           sizeof(gDefaultDeviceDiscovery),
                                           sizeof(gDefaultEventInfo),
                                           sizeof(gDefaultMediaInfo),
                                           sizeof(gDefaultStreamInfo),
                                           sizeof(gDefaultPTZInfo),
                                           sizeof(gDefaultImageInfo),
                                           sizeof(gDefaultDateTimeInfo),
                                           sizeof(gDefaultDetectionInfo),
                                           sizeof(gDefaultEventConfig),
                                           sizeof(gDefaultRecordConfig)};

extern enum tt__UserLevel gUserLevel;

static ID_HEADER_PSTORE gIDHeader={0};
static ID_MEM_BUF gIDMem={0, 0, 0, NULL};
/* =================================================================
 *   Private Functions
 * =================================================================
 */

CFG_SHM_KEY NvtDevInfo_Cfg_NameToKey(const char *cfgName)
{
    if (!strcmp(cfgName, CFG_DEV_INFO_NAME))
        return CFG_SHM_KEY_DEV_INFO;
    else if (!strcmp(cfgName, CFG_USER_LIST_NAME))
        return CFG_SHM_KEY_USER_LIST;
    else if (!strcmp(cfgName, CFG_NETWORK_INFO_NAME))
        return CFG_SHM_KEY_NETWORK_INFO;
    else if (!strcmp(cfgName, CFG_DEV_DISCOVER_NAME))
        return CFG_SHM_KEY_DEV_DISCOVER;
    else if (!strcmp(cfgName, CFG_EVENT_INFO_NAME))
        return CFG_SHM_KEY_EVENT_INFO;
    else if (!strcmp(cfgName, CFG_MEDIA_INFO_NAME))
        return CFG_SHM_KEY_MEDIA_INFO;
    else if (!strcmp(cfgName, CFG_STREAM_INFO_NAME))
        return CFG_SHM_KEY_STREAM_INFO;
    else if (!strcmp(cfgName, CFG_PTZ_INFO_NAME))
        return CFG_SHM_KEY_PTZ_INFO;
    else if (!strcmp(cfgName, CFG_IMAGE_INFO_NAME))
        return CFG_SHM_KEY_IMAGE_INFO;
    else if (!strcmp(cfgName, CFG_DATETIME_INFO_NAME))
        return CFG_SHM_KEY_DATETIME_INFO;
    else if (!strcmp(cfgName, CFG_DETECTION_INFO_NAME))
        return CFG_SHM_KEY_DETECTION_INFO;
    else if (!strcmp(cfgName, CFG_EVENT_CONFIG_NAME))
        return CFG_SHM_KEY_EVENT_CONFIG;
    else if (!strcmp(cfgName, CFG_RECORD_CONFIG_NAME))
        return CFG_SHM_KEY_RECORD_CONFIG;
    else
    {
        DBG_ERR("Invalid config name : %s\n", cfgName);
        return CFG_SHM_KEY_MAX;
    }

    return CFG_SHM_KEY_MAX;
}

NVTRET NvtDevInfo_Cfg_KeyToName(const CFG_SHM_KEY shmKey, char *cfgName)
{
    switch (shmKey)
    {
        case CFG_SHM_KEY_DEV_INFO:
            strncpy(cfgName, CFG_DEV_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_USER_LIST:
            strncpy(cfgName, CFG_USER_LIST_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_NETWORK_INFO:
            strncpy(cfgName, CFG_NETWORK_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_DEV_DISCOVER:
            strncpy(cfgName, CFG_DEV_DISCOVER_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_EVENT_INFO:
            strncpy(cfgName, CFG_EVENT_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_MEDIA_INFO:
            strncpy(cfgName, CFG_MEDIA_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_STREAM_INFO:
            strncpy(cfgName, CFG_STREAM_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_PTZ_INFO:
            strncpy(cfgName, CFG_PTZ_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_IMAGE_INFO:
            strncpy(cfgName, CFG_IMAGE_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_DATETIME_INFO:
            strncpy(cfgName, CFG_DATETIME_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_DETECTION_INFO:
            strncpy(cfgName, CFG_DETECTION_INFO_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_EVENT_CONFIG:
            strncpy(cfgName, CFG_EVENT_CONFIG_NAME, CFG_NAME_LENGTH_MAX);
            break;
        case CFG_SHM_KEY_RECORD_CONFIG:
            strncpy(cfgName, CFG_RECORD_CONFIG_NAME, CFG_NAME_LENGTH_MAX);
            break;
        default:
            DBG_ERR("Invalid shm key : %d\n", shmKey);
            return ER_ITEM_NOT_SUPPORT;
    }

    DBG_IND("SHM Key %d => cfgName %s\n", shmKey, cfgName);

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_SaveToPStore(const char *cfgName, void *shmAddr)
{
    int psHandle=-1;
    CFG_SHM_KEY shmKey = NvtDevInfo_Cfg_NameToKey(cfgName);
    MD5_CTX ctx;

    if (shmKey == CFG_SHM_KEY_MAX)
    {
        DBG_ERR("Invalid config name!\n");
        return ER_INVALID_INPUT_DATA;
    }

    if ((psHandle = nvtpstore_open((char *)cfgName, PS_CREATE | PS_WRONLY)) < 0)
    {
        DBG_ERR("Open pstore %s fail\r\n", cfgName);
        return ER_STORAGE_OPEN_FAIL;
    }

    memset(&gCfgHeader[CFG_GROUP_IDX(shmKey)], 0, sizeof(CFG_HEADER_PSTORE));
	strcpy(gCfgHeader[CFG_GROUP_IDX(shmKey)].cVersion, FIRMWARE_VERSION);
	gCfgHeader[CFG_GROUP_IDX(shmKey)].uiCfgSize = settingSize[CFG_GROUP_IDX(shmKey)];
    strncpy(gCfgHeader[CFG_GROUP_IDX(shmKey)].cCfgName, cfgName, sizeof(gCfgHeader[CFG_GROUP_IDX(shmKey)].cCfgName)-1);
    MD5_Init(&ctx);
    MD5_Update(&ctx, (uint8_t *)shmAddr, settingSize[CFG_GROUP_IDX(shmKey)]);
    MD5_Final((uint8_t *)(gCfgHeader[CFG_GROUP_IDX(shmKey)].uiMD5Checksum), &ctx);

    if (write(psHandle, &gCfgHeader[CFG_GROUP_IDX(shmKey)], sizeof(CFG_HEADER_PSTORE)) !=
        sizeof(CFG_HEADER_PSTORE))
    {
        DBG_ERR("Write setting header(shmKey = %d) to pstore fail!\n");
        nvtpstore_close(psHandle);
        return ER_STORAGE_WRITE_FAIL;
    }

    if (write(psHandle, shmAddr, settingSize[CFG_GROUP_IDX(shmKey)]) !=
        (int)settingSize[CFG_GROUP_IDX(shmKey)])
    {
        DBG_ERR("Write setting(shmKey = %d) to pstore fail!\n");
        nvtpstore_close(psHandle);
        return ER_STORAGE_WRITE_FAIL;
    }

    nvtpstore_close(psHandle);

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_LoadFromPStore(const CFG_SHM_KEY shmKey, void *shmAddr)
{
    int psHandle=-1;
    char cfgName[CFG_NAME_LENGTH_MAX+1];
    CFG_HEADER_PSTORE tmpCfgHeader;
    BOOL defaultFlag=NVT_FALSE;
    MD5_CTX ctx;
    UINT8 digest[16];

    if (NvtDevInfo_Cfg_KeyToName(shmKey, cfgName) != 0)
    {
        DBG_ERR("Get config name by shm key fail!\n");
        return ER_ITEM_NOT_SUPPORT;
    }

    if ((psHandle = nvtpstore_open((char *)cfgName, PS_RDONLY)) < 0)
    {
        DBG_WRN("Open pstore %s fail, use default setting\r\n", cfgName);
        defaultFlag = NVT_TRUE;
		goto CFG_LOAD_FROM_PSTORE_EXIT;
    }

    // Read config from PStore
    if (read(psHandle, &tmpCfgHeader, sizeof(CFG_HEADER_PSTORE)) == sizeof(CFG_HEADER_PSTORE))
    {
        if (tmpCfgHeader.uiCfgSize != settingSize[CFG_GROUP_IDX(shmKey)])
        {
            DBG_WRN("Length of setting (shmKey %d) not sync with pstore %s, use default setting\n", shmKey, cfgName);
            defaultFlag = NVT_TRUE;
			goto CFG_LOAD_FROM_PSTORE_EXIT;
        }
        else
        {
            memcpy(&gCfgHeader[CFG_GROUP_IDX(shmKey)], &tmpCfgHeader, sizeof(CFG_HEADER_PSTORE));

            if (read(psHandle, shmAddr, settingSize[CFG_GROUP_IDX(shmKey)]) !=
                (int)settingSize[CFG_GROUP_IDX(shmKey)])
            {
                DBG_WRN("Read setting (shmKey %d) from pstore %s fail, use default setting\n", shmKey, cfgName);
                defaultFlag = NVT_TRUE;
            }
            else
            {
                MD5_Init(&ctx);
                MD5_Update(&ctx, (uint8_t *)shmAddr, settingSize[CFG_GROUP_IDX(shmKey)]);
                MD5_Final((uint8_t *)digest, &ctx);

                if (memcmp(gCfgHeader[CFG_GROUP_IDX(shmKey)].uiMD5Checksum, digest, sizeof(digest)))
                {
                    DBG_WRN("MD5 checksum (shmKey %d) from pstore %s check fail, use default setting\n", shmKey, cfgName);
                    defaultFlag = NVT_TRUE;
					goto CFG_LOAD_FROM_PSTORE_EXIT;
                }
                else if (strcmp(gCfgHeader[CFG_GROUP_IDX(shmKey)].cVersion, FIRMWARE_VERSION))
                {
                    DBG_WRN("Version (shmKey %d) from pstore %s check fail, use default setting\n", shmKey, cfgName);
                    defaultFlag = NVT_TRUE;
					goto CFG_LOAD_FROM_PSTORE_EXIT;
                }
            }
        }
    }

CFG_LOAD_FROM_PSTORE_EXIT:
	if (psHandle >= 0)
	    nvtpstore_close(psHandle);

    if (defaultFlag == NVT_TRUE)
    {
        memcpy(shmAddr, defaultSettingPtr[CFG_GROUP_IDX(shmKey)], settingSize[CFG_GROUP_IDX(shmKey)]);
        if (NvtDevInfo_Cfg_SaveToPStore(cfgName, shmAddr) < 0)
            DBG_ERR("Write setting (shmKey %d) to pstore %s fail!\n", shmKey, cfgName);
    }

    return ER_SUCCESS;
}

/* =================================================================
 *   Public Function
 * =================================================================
 */

void *NvtDevInfo_Cfg_SHMAttach(const char *cfgName)
{
    CFG_SHM_KEY shmKey;
    int shmID=-1, retry=5;
    void *shmAddr;

    if ((shmKey = NvtDevInfo_Cfg_NameToKey(cfgName)) >= CFG_SHM_KEY_MAX)
    {
        DBG_ERR("Convert config name (%s) to key fail!\n", cfgName);
        return NULL;
    }

    while ((shmID = shmget(shmKey, settingSize[CFG_GROUP_IDX(shmKey)], 0666)) < 0 &&
           retry > 0)
    {
        sleep(1);
        retry--;
    }

    if (shmID < 0)
    {
        DBG_ERR("shmget fail!(shmKey = %d, pid = %d), error: %s\n", shmKey, getpid(), strerror(errno));
        return NULL;
    }

    if ((shmAddr = shmat(shmID, NULL, 0)) == NULL)
    {
        DBG_ERR("shmat fail!(shmKey = %d, pid = %d), error: %s\n", shmKey, getpid(), strerror(errno));
        return NULL;
    }

    return shmAddr;
}

NVTRET NvtDevInfo_Cfg_SHMDeAttach(void *shmAddr)
{
    if (shmdt(shmAddr) < 0)
    {
        DBG_ERR("shmdt fail!(shmAddr = 0x%08X), error: %s\n", shmAddr, strerror(errno));
        return ER_RESOURCE_RELEASE_FAIL;
    }

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_Reset(const char *cfgName)
{
    CFG_SHM_KEY shmKey, shmKeyTarget=CFG_SHM_KEY_MAX;
    BOOL bAllCfgFlag=NVT_FALSE, bExcludeNetCfgFlag=NVT_FALSE;
    char cfgNameConvert[CFG_NAME_LENGTH_MAX+1]={0};
    void *shmAddr;

    DBG_DUMP("Config reset %s start\n", cfgName);

    if (!strcmp(cfgName, CFG_All_CONFIG_NAME))
    {
        bAllCfgFlag = NVT_TRUE;
    }
    else if (!strcmp(cfgName, CFG_EXCLUDE_NETCFG_NAME))
    {
        bAllCfgFlag = NVT_TRUE;
        bExcludeNetCfgFlag = NVT_TRUE;
    }
    else
    {
        if ((shmKeyTarget = NvtDevInfo_Cfg_NameToKey(cfgName)) >= CFG_SHM_KEY_MAX)
        {
            DBG_ERR("Convert config name (%s) to key fail!\n", cfgName);
            return ER_ITEM_NOT_SUPPORT;
        }
    }

    for (shmKey = CFG_SHM_KEY_DEV_INFO; shmKey < CFG_SHM_KEY_MAX; shmKey++)
    {
        // If bExcludeNetCfgFlag == NVT_TRUE, skip network info
        if (bExcludeNetCfgFlag == NVT_TRUE && shmKey == CFG_SHM_KEY_NETWORK_INFO)
        {
            continue;
        }
        else if (bAllCfgFlag == NVT_TRUE || shmKey == shmKeyTarget)
        {
            NvtDevInfo_Cfg_KeyToName(shmKey, cfgNameConvert);
            if ((shmAddr = NvtDevInfo_Cfg_SHMAttach(cfgNameConvert)) != NULL)
            {
                memcpy(shmAddr, defaultSettingPtr[CFG_GROUP_IDX(shmKey)], settingSize[CFG_GROUP_IDX(shmKey)]);
                if (NvtDevInfo_Cfg_SaveToPStore(cfgNameConvert, shmAddr) < 0)
                    DBG_ERR("Write setting (shmKey %d) to pstore %s fail!\n", shmKey, cfgName);
                NvtDevInfo_Cfg_SHMDeAttach(shmAddr);
            }
            else
            {
                DBG_ERR("Get shared memory fail!\n");
                return ER_GET_DATA_FAIL;
            }
        }
    }

    DBG_DUMP("Config reset %s end\n", cfgName);

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_SaveCmdRecord(const char *cfgName)
{
    int shmID;
    unsigned int saveCmdIdx;
    size_t shmSize=CFG_SAVE_CMD_LEN_MAX * CFG_SAVE_CMD_NUM_MAX;
    void *shmAddr=NULL;
    char *cmdRecordPtr;

    DBG_IND("Cfg save command record start\n");

    DBG_IND("shmKey %d: shmSize = %d\n", CFG_SHM_KEY_SAVE_CMD, shmSize);

    if ((shmID = shmget(CFG_SHM_KEY_SAVE_CMD, shmSize, IPC_CREAT | 0666)) < 0)
    {

        if (errno == EEXIST)
        {
            DBG_WRN("shmget(shmKey = %d) => %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
            return ER_SUCCESS;
        }
        else
        {
            DBG_ERR("shmget fail!(shmKey = %d), error: %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
            return ER_GET_DATA_FAIL;
        }
    }

    if ((shmAddr = shmat(shmID, NULL, 0)) == NULL)
    {
        DBG_ERR("shmat fail!(shmKey = %d), error: %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
        return ER_GET_DATA_FAIL;
    }

    for (saveCmdIdx = 0; saveCmdIdx < CFG_SAVE_CMD_NUM_MAX; saveCmdIdx++)
    {
        cmdRecordPtr=(char*)(shmAddr + (saveCmdIdx * CFG_SAVE_CMD_LEN_MAX));

        if (*cmdRecordPtr == '\0')
        {
            snprintf(cmdRecordPtr, CFG_SAVE_CMD_LEN_MAX-1, "%d:%s", getpid(), cfgName);
            DBG_IND("Save command record %s in index %d\n", cmdRecordPtr, saveCmdIdx);
            break;
        }
    }

    if (saveCmdIdx >= CFG_SAVE_CMD_NUM_MAX)
        DBG_WRN("Cfg save command record buffer full!\n");

    if (shmdt(shmAddr) < 0)
        DBG_ERR("shmdt fail!(shmAddr = 0x%08X), error: %s\n", shmAddr, strerror(errno));

    DBG_IND("Cfg save command record end\n");

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_WaitCmdFinish(const char *cfgName)
{
    int shmID, cmdRecordLen;
    unsigned int saveCmdIdx;
    size_t shmSize=CFG_SAVE_CMD_LEN_MAX * CFG_SAVE_CMD_NUM_MAX;
    void *shmAddr=NULL;
    char *cmdRecordPtr=NULL, cmdRecordStr[CFG_SAVE_CMD_LEN_MAX];
    int retry100ms=100;

    DBG_IND("Cfg wait command finish start\n");

    if ((shmID = shmget(CFG_SHM_KEY_SAVE_CMD, shmSize, 0666)) < 0)
    {

        if (errno == EEXIST)
        {
            DBG_WRN("shmget(shmKey = %d) => %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
            return ER_SUCCESS;
        }
        else
        {
            DBG_ERR("shmget fail!(shmKey = %d), error: %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
            return ER_GET_DATA_FAIL;
        }
    }

    if ((shmAddr = shmat(shmID, NULL, 0)) == NULL)
    {
        DBG_ERR("shmat fail!(shmKey = %d), error: %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
        return ER_GET_DATA_FAIL;
    }

    cmdRecordLen = snprintf(cmdRecordStr, CFG_SAVE_CMD_LEN_MAX-1, "%d:%s", getpid(), cfgName);

    DBG_IND("Wait save command : %s\n", cmdRecordStr);

    for (saveCmdIdx = 0; saveCmdIdx < CFG_SAVE_CMD_NUM_MAX; saveCmdIdx++)
    {
        cmdRecordPtr=(char*)(shmAddr + (saveCmdIdx * CFG_SAVE_CMD_LEN_MAX));

        if (*cmdRecordPtr != '\0' && !strncmp(cmdRecordPtr, cmdRecordStr, cmdRecordLen))
        {
            DBG_IND("Wait %s in index %d\n", cmdRecordStr, saveCmdIdx);
            while (*cmdRecordPtr != '\0' && retry100ms >= 0)
            {
                DBG_IND("Wait save command, retry = %d\n", retry100ms);
                usleep(100000);
                retry100ms--;
            }
            break;
        }
    }

    if (shmdt(shmAddr) < 0)
        DBG_ERR("shmdt fail!(shmAddr = 0x%08X), error: %s\n", shmAddr, strerror(errno));

    DBG_IND("Cfg wait command finish end\n");

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_ClearCmdRecord(const char *pidStr, const char *cfgName)
{
    int shmID, cmdRecordLen;
    unsigned int saveCmdIdx;
    size_t shmSize=CFG_SAVE_CMD_LEN_MAX * CFG_SAVE_CMD_NUM_MAX;
    void *shmAddr=NULL;
    char *cmdRecordPtr=NULL, cmdRecordStr[CFG_SAVE_CMD_LEN_MAX];

    DBG_IND("Cfg clear command record start\n");

    if ((shmID = shmget(CFG_SHM_KEY_SAVE_CMD, shmSize, 0666)) < 0)
    {

        if (errno == EEXIST)
        {
            DBG_WRN("shmget(shmKey = %d) => %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
            return ER_SUCCESS;
        }
        else
        {
            DBG_ERR("shmget fail!(shmKey = %d), error: %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
            return ER_GET_DATA_FAIL;
        }
    }

    if ((shmAddr = shmat(shmID, NULL, 0)) == NULL)
    {
        DBG_ERR("shmat fail!(shmKey = %d), error: %s\n", CFG_SHM_KEY_SAVE_CMD, strerror(errno));
        return ER_GET_DATA_FAIL;
    }

    cmdRecordLen = snprintf(cmdRecordStr, CFG_SAVE_CMD_LEN_MAX-1, "%s:%s", pidStr, cfgName);

    DBG_IND("Clear command record : %s\n", cmdRecordStr);

    for (saveCmdIdx = 0; saveCmdIdx < CFG_SAVE_CMD_NUM_MAX; saveCmdIdx++)
    {
        cmdRecordPtr=(char*)(shmAddr + (saveCmdIdx * CFG_SAVE_CMD_LEN_MAX));

        if (*cmdRecordPtr != '\0' && !strncmp(cmdRecordPtr, cmdRecordStr, cmdRecordLen))
        {
            DBG_IND("Clear %s in index %d\n", cmdRecordStr, saveCmdIdx);
            *cmdRecordPtr = '\0';
            break;
        }
    }

    if (shmdt(shmAddr) < 0)
        DBG_ERR("shmdt fail!(shmAddr = 0x%08X), error: %s\n", shmAddr, strerror(errno));

    DBG_IND("Cfg clear command record end\n");

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_Save(const char *cfgName)
{
    char msgCmd[SYS_MSG_LEN_MAX];
    int msgCmdLen;

    NvtDevInfo_Cfg_SaveCmdRecord(cfgName);
    msgCmdLen = snprintf(msgCmd, SYS_MSG_LEN_MAX - 1, "%s:%d:%s", SYS_CMD_CFG_CHANGE, getpid(), cfgName);

    return NvtDevInfo_NotifyMsg_Send(msgCmd, msgCmdLen);
}

NVTRET NvtDevInfo_Cfg_RealSave(const char *cfgName)
{
    CFG_SHM_KEY shmKey, shmKeyTarget=CFG_SHM_KEY_MAX;
    BOOL bAllCfgFlag=NVT_FALSE, bExcludeNetCfgFlag=NVT_FALSE;
    char cfgNameConvert[CFG_NAME_LENGTH_MAX+1]={0};
    void *shmAddr;

    DBG_DUMP("Config save %s start\n", cfgName);

    if (!strcmp(cfgName, CFG_All_CONFIG_NAME))
    {
        bAllCfgFlag = NVT_TRUE;
    }
    else if (!strcmp(cfgName, CFG_EXCLUDE_NETCFG_NAME))
    {
        bAllCfgFlag = NVT_TRUE;
        bExcludeNetCfgFlag = NVT_TRUE;
    }
    else
    {
        if ((shmKeyTarget = NvtDevInfo_Cfg_NameToKey(cfgName)) >= CFG_SHM_KEY_MAX)
        {
            DBG_ERR("Convert config name (%s) to key fail!\n", cfgName);
            return ER_ITEM_NOT_SUPPORT;
        }
    }

    for (shmKey = CFG_SHM_KEY_DEV_INFO; shmKey < CFG_SHM_KEY_MAX; shmKey++)
    {
        // If bExcludeNetCfgFlag == NVT_TRUE, skip network info
        if (bExcludeNetCfgFlag == NVT_TRUE && shmKey == CFG_SHM_KEY_NETWORK_INFO)
        {
            continue;
        }
        else if (bAllCfgFlag == NVT_TRUE || shmKey == shmKeyTarget)
        {
            NvtDevInfo_Cfg_KeyToName(shmKey, cfgNameConvert);

            if ((shmAddr = NvtDevInfo_Cfg_SHMAttach(cfgNameConvert)) != NULL)
            {
                if (NvtDevInfo_Cfg_SaveToPStore(cfgNameConvert, shmAddr) < 0)
                    DBG_ERR("Write setting (shmKey %d) to pstore %s fail!\n", shmKey, cfgName);

                NvtDevInfo_Cfg_SHMDeAttach(shmAddr);
            }
            else
            {
                DBG_ERR("Get shared memory fail!\n");
                return ER_GET_DATA_FAIL;
            }
        }
    }

    DBG_DUMP("Config save %s end\n", cfgName);

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_Backup(const char *fileName)
{
    NvtErrorCode ret=ER_SUCCESS;
    CFG_SHM_KEY shmKey;
    char cfgName[CFG_NAME_LENGTH_MAX+1];
    CFG_HEADER_PSTORE tmpCfgHeader;
    int bufLen=0;
    void *tmpBuf=NULL;
    int fd=-1, psHandle=-1;

    DBG_DUMP("Config backup start\n");

    if ((fd = open(fileName, O_CREAT | O_WRONLY)) < 0)
    {
        DBG_ERR("Open %s fail, error = %s\n", fileName, strerror(errno));
        ret = ER_INVALID_INPUT_DATA;
        goto BACKUP_EXIT;
    }

    for (shmKey = CFG_SHM_KEY_DEV_INFO; shmKey < CFG_SHM_KEY_MAX; shmKey++)
    {
        NvtDevInfo_Cfg_KeyToName(shmKey, cfgName);

        DBG_IND("Handle config %s\n", cfgName);

        if ((psHandle = nvtpstore_open(cfgName, PS_RDONLY)) < 0)
        {
            DBG_ERR("Open pstore %s fail(ret=%d)\r\n", cfgName, psHandle);
            ret = ER_STORAGE_OPEN_FAIL;
            goto BACKUP_EXIT;
        }

        // Read config from PStore
        if ((read(psHandle, &tmpCfgHeader, sizeof(CFG_HEADER_PSTORE)) != sizeof(CFG_HEADER_PSTORE)) ||
			tmpCfgHeader.uiCfgSize == 0 || tmpCfgHeader.uiCfgSize > CFG_GROUP_MAX_SIZE)
        {
            DBG_ERR("Read config header (%s) from pstore fail\r\n", cfgName);
            ret = ER_STORAGE_OPEN_FAIL;
            goto BACKUP_EXIT;
        }

        if (write(fd, &tmpCfgHeader, sizeof(CFG_HEADER_PSTORE)) != sizeof(CFG_HEADER_PSTORE))
        {
            DBG_ERR("Write config header (%s) to %s fail\r\n", cfgName, fileName);
            ret = ER_STORAGE_WRITE_FAIL;
            goto BACKUP_EXIT;
        }

        // Allocate buffer memory
        if ((int)tmpCfgHeader.uiCfgSize > bufLen)
        {
            if (tmpBuf == NULL)
                tmpBuf = malloc(tmpCfgHeader.uiCfgSize);
            else
                tmpBuf = realloc(tmpBuf, tmpCfgHeader.uiCfgSize);

			if (tmpBuf == NULL)
			{
				DBG_ERR("malloc fail!\n");
				goto BACKUP_EXIT;
			}

            bufLen = tmpCfgHeader.uiCfgSize;
        }

        // Read config data to buffer
        if (read(psHandle, tmpBuf, tmpCfgHeader.uiCfgSize) != (int)tmpCfgHeader.uiCfgSize)
        {
            DBG_ERR("Read config data (%s) from pstore fail\r\n", cfgName);
            ret = ER_STORAGE_OPEN_FAIL;
            goto BACKUP_EXIT;
        }

        // Write config data to file
        if (write(fd, tmpBuf, tmpCfgHeader.uiCfgSize) != (int)tmpCfgHeader.uiCfgSize)
        {
            DBG_ERR("Write config data (%s) to %s fail\r\n", cfgName, fileName);
            ret = ER_STORAGE_WRITE_FAIL;
            goto BACKUP_EXIT;
        }

        nvtpstore_close(psHandle);
        psHandle = -1;
    }

BACKUP_EXIT:
    if (psHandle >= 0)
        nvtpstore_close(psHandle);

    if (tmpBuf)
        free(tmpBuf);

    if (fd >= 0)
    {
        close(fd);
        sync();
    }
    if (ret != ER_SUCCESS){
        
        if(unlink(fileName)< 0){
            DBG_ERR("unlink error!\r\n");
        }
    }
    DBG_DUMP("Config backup end\n");

    return ret;
}

NVTRET NvtDevInfo_Cfg_Restore(const char *fileName)
{
    NvtErrorCode ret=ER_SUCCESS;
    CFG_HEADER_PSTORE tmpCfgHeader={0};
    CFG_SHM_KEY shmKey;
    int bufLen=0, readHeaderLen=0;
    void *tmpBuf=NULL;
    int fd=-1, psHandle=-1;

    DBG_DUMP("Config restore start\n");

    if ((fd = open(fileName, O_RDONLY)) < 0)
    {
        DBG_ERR("Open %s fail, error = %s\n", fileName, strerror(errno));
        ret = ER_INVALID_INPUT_DATA;
        goto RESTORE_EXIT;
    }

    while ((readHeaderLen = read(fd, &tmpCfgHeader, sizeof(CFG_HEADER_PSTORE))) == sizeof(CFG_HEADER_PSTORE))
    {
        tmpCfgHeader.cCfgName[sizeof(tmpCfgHeader.cCfgName)-1] = '\0';
        tmpCfgHeader.cVersion[sizeof(tmpCfgHeader.cVersion)-1] = '\0';
        DBG_IND("Config Name = %s\n", tmpCfgHeader.cCfgName);
        DBG_IND("  Version = %s\n", tmpCfgHeader.cVersion);
        DBG_IND("  Size = %lu\n", tmpCfgHeader.uiCfgSize);
        // Check config version
        if (strcmp(tmpCfgHeader.cVersion, FIRMWARE_VERSION))
        {
            DBG_ERR("Check version fail!\n");
            ret = ER_INVALID_INPUT_DATA;
            goto RESTORE_EXIT;
        }

        // Check config name
        if ((shmKey = NvtDevInfo_Cfg_NameToKey(tmpCfgHeader.cCfgName)) >= CFG_SHM_KEY_MAX)
        {
            DBG_ERR("Check config name fail!\n");
            ret = ER_INVALID_INPUT_DATA;
            goto RESTORE_EXIT;
        }

        // Check config size
        if (tmpCfgHeader.uiCfgSize != settingSize[CFG_GROUP_IDX(shmKey)])
        {
            DBG_ERR("Check config size fail!\n");
            ret = ER_INVALID_INPUT_DATA;
            goto RESTORE_EXIT;
        }

        // Allocate buffer memory
        if ((int)tmpCfgHeader.uiCfgSize > bufLen)
        {
            if (tmpBuf == NULL)
                tmpBuf = malloc(tmpCfgHeader.uiCfgSize);
            else
                tmpBuf = realloc(tmpBuf, tmpCfgHeader.uiCfgSize);

			if (tmpBuf == NULL)
				goto RESTORE_EXIT;

            bufLen = tmpCfgHeader.uiCfgSize;
        }

        // Read config data
        if (read(fd, tmpBuf, tmpCfgHeader.uiCfgSize) != (int)tmpCfgHeader.uiCfgSize)
        {
            DBG_ERR("Read config data fail!\n");
            ret = ER_INVALID_INPUT_DATA;
            goto RESTORE_EXIT;
        }

        // Write config header
        if ((psHandle = nvtpstore_open(tmpCfgHeader.cCfgName, PS_CREATE | PS_WRONLY)) < 0)
        {
            DBG_ERR("Open pstore %s fail(ret=%d)\r\n", tmpCfgHeader.cCfgName, psHandle);
            ret = ER_STORAGE_OPEN_FAIL;
            goto RESTORE_EXIT;
        }

        if (write(psHandle, &tmpCfgHeader, sizeof(CFG_HEADER_PSTORE)) != sizeof(CFG_HEADER_PSTORE))
        {
            DBG_ERR("Write config header to pstore %s fail!\n", tmpCfgHeader.cCfgName);
            ret = ER_STORAGE_WRITE_FAIL;
            goto RESTORE_EXIT;
        }

        // Write config data
        if (write(psHandle, tmpBuf, tmpCfgHeader.uiCfgSize) != (int)tmpCfgHeader.uiCfgSize)
        {
            DBG_ERR("Write config data to pstore %s fail!\n", tmpCfgHeader.cCfgName);
            ret = ER_STORAGE_WRITE_FAIL;
            goto RESTORE_EXIT;
        }

        nvtpstore_close(psHandle);
        psHandle = -1;
    }

    if (readHeaderLen != 0)
    {
        DBG_ERR("Read config header fail!\n");
        ret = ER_INVALID_INPUT_DATA;
    }

RESTORE_EXIT:
    if (psHandle >= 0)
        nvtpstore_close(psHandle);

    if (tmpBuf)
        free(tmpBuf);

    if (fd >= 0)
        close(fd);

    DBG_DUMP("Config restore end\n");

    return ret;
}

NVTRET NvtDevInfo_Cfg_Init(void)
{
    CFG_SHM_KEY shmKey;
    int shmID;
    void *shmAddr;

    DBG_DUMP("Config init start\n");

    for (shmKey = CFG_SHM_KEY_DEV_INFO; shmKey < CFG_SHM_KEY_MAX; shmKey++)
    {
        DBG_IND("shmKey %d: shmSize = %d\n", shmKey, settingSize[CFG_GROUP_IDX(shmKey)]);

        if ((shmID = shmget(shmKey, settingSize[CFG_GROUP_IDX(shmKey)], IPC_CREAT | IPC_EXCL | 0666)) < 0)
        {

            if (errno == EEXIST)
            {
                DBG_WRN("shmget(shmKey = %d) => %s\n", shmKey, strerror(errno));
                return ER_SUCCESS;
            }
            else
            {
                DBG_ERR("shmget fail!(shmKey = %d), error: %s\n", shmKey, strerror(errno));
                return ER_GET_DATA_FAIL;
            }
        }

        if ((shmAddr = shmat(shmID, NULL, 0)) == NULL)
        {
            DBG_ERR("shmat fail!(shmKey = %d), error: %s\n", shmKey, strerror(errno));
            return ER_GET_DATA_FAIL;
        }

        NvtDevInfo_Cfg_LoadFromPStore(shmKey, shmAddr);

        if (shmdt(shmAddr) < 0)
            DBG_ERR("shmdt fail!, error: %s\n", strerror(errno));
    }

    DBG_DUMP("Config init end\n");

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_Cfg_Release(void)
{
#if 0
    CFG_SHM_KEY shmKey;
    int shmID;

    for (shmKey = CFG_SHM_KEY_DEV_INFO; shmKey < CFG_SHM_KEY_MAX; shmKey++)
    {
        DBG_IND("Remove shm(shmKey = %d)...", shmKey);
        if ((shmID = shmget(shmKey, settingSize[CFG_GROUP_IDX(shmKey)], 0666)) > 0)
        {
            DBG_IND("success\n");
            shmctl(shmID, IPC_RMID, 0);
        }
        else
            DBG_IND("fail\n");
    }
#endif
    return ER_SUCCESS;
}

/* ID PStore Functions */
NVTRET NvtDevInfo_ID_Init(void)
{
    int psHandle=-1;

    if (gIDMem.uiBufSize == 0)
    {
        if ((gIDMem.cMemBuf = (char *)calloc(1, ID_PSTORE_SIZE_MAX+(ID_ITEM_LEN_MAX*2))) == NULL)
        {
            DBG_ERR("calloc fail!\n");
            return ER_MEM_ALLOC_FAIL;
        }
        gIDMem.uiBufSize = ID_PSTORE_SIZE_MAX;
        gIDMem.uiDataSize = 0;
        gIDMem.uiListIdx = 0;
    }

    if ((psHandle = nvtpstore_open(ID_PSTORE_NAME, PS_RDONLY)) < 0)
    {
        DBG_IND("PStore %s not exist\r\n", ID_PSTORE_NAME);
        return ER_SUCCESS;
    }

    // Read identity from PStore
    if (read(psHandle, &gIDHeader, sizeof(ID_HEADER_PSTORE)) == sizeof(ID_HEADER_PSTORE))
    {
		if (gIDHeader.uiIDSize == 0 || gIDHeader.uiIDSize > sizeof(gIDMem.cMemBuf))
		{
			DBG_ERR("Read pstore %s fail\r\n", ID_PSTORE_NAME);
	        nvtpstore_close(psHandle);
	        gIDMem.uiDataSize = 0;
	        gIDMem.cMemBuf[0] = '\0';
	        return ER_STORAGE_READ_FAIL;
		}

        if (read(psHandle, gIDMem.cMemBuf, gIDHeader.uiIDSize) == (int)(gIDHeader.uiIDSize))
        {
            gIDMem.uiDataSize = gIDHeader.uiIDSize;
            gIDMem.cMemBuf[gIDMem.uiDataSize] = '\0';
        }
        else
        {
            DBG_ERR("Read pstore %s fail\r\n", ID_PSTORE_NAME);
            nvtpstore_close(psHandle);
            gIDMem.uiDataSize = 0;
            gIDMem.cMemBuf[0] = '\0';
            return ER_STORAGE_READ_FAIL;
        }
    }
    else
    {
        DBG_ERR("Read pstore %s fail\r\n", ID_PSTORE_NAME);
        nvtpstore_close(psHandle);
        gIDMem.uiDataSize = 0;
        gIDMem.cMemBuf[0] = '\0';
        return ER_STORAGE_READ_FAIL;
    }

    nvtpstore_close(psHandle);

    DBG_IND("Load buf : \n%s\n\n", gIDMem.cMemBuf);
    return ER_SUCCESS;
}

NVTRET NvtDevInfo_ID_Release(void)
{
    if (gIDMem.cMemBuf != NULL)
    {
        free(gIDMem.cMemBuf);
        gIDMem.cMemBuf = NULL;
    }

    gIDMem.uiBufSize = 0;
    gIDMem.uiDataSize = 0;
    gIDMem.uiListIdx = 0;

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_ID_Reset(void)
{
    if (gIDMem.uiBufSize == 0)
    {
        DBG_ERR("NvtDevinfo ID not initialized!\n");
        return ER_RESOURCE_NOT_INIT;
    }

    nvtpstore_remove(ID_PSTORE_NAME);
    gIDMem.uiDataSize = 0;
    gIDMem.cMemBuf[0] = '\0';
    gIDMem.uiListIdx = 0;

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_ID_Save(void)
{
    int psHandle=-1;

    if (gIDMem.uiBufSize == 0)
    {
        DBG_ERR("NvtDevinfo ID not initialized!\n");
        return ER_RESOURCE_NOT_INIT;
    }

    if (gIDMem.uiDataSize == 0)
    {
        DBG_WRN("There is no data to save!\n");
        return ER_GET_DATA_FAIL;
    }

    //nvtpstore_remove(ID_PSTORE_NAME);

    if ((psHandle = nvtpstore_open(ID_PSTORE_NAME, PS_CREATE | PS_WRONLY)) < 0)
    {
        DBG_ERR("Open pstore %s fail\r\n", ID_PSTORE_NAME);
        return ER_STORAGE_OPEN_FAIL;
    }

    gIDHeader.uiIDSize = gIDMem.uiDataSize;

    if (write(psHandle, &gIDHeader, sizeof(ID_HEADER_PSTORE)) !=
        sizeof(ID_HEADER_PSTORE))
    {
        DBG_ERR("Write ID header to pstore fail!\n");
        nvtpstore_close(psHandle);
        return ER_STORAGE_WRITE_FAIL;
    }

    if (write(psHandle, gIDMem.cMemBuf, gIDMem.uiDataSize) !=
        (int)gIDMem.uiDataSize)
    {
        DBG_ERR("Write ID data to pstore fail!\n");
        nvtpstore_close(psHandle);
        return ER_STORAGE_WRITE_FAIL;
    }

    nvtpstore_close(psHandle);

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_ID_List_Reset(void)
{
    if (gIDMem.uiBufSize == 0)
    {
        DBG_ERR("NvtDevinfo ID not initialized!\n");
        return ER_RESOURCE_NOT_INIT;
    }

    gIDMem.uiListIdx = 0;

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_ID_List_Each(char name[ID_ITEM_LEN_MAX], char value[ID_ITEM_LEN_MAX])
{
    char *startPtr=NULL, *token=NULL;

    if (gIDMem.uiBufSize == 0)
    {
        DBG_ERR("NvtDevinfo ID not initialized!\n");
        return ER_RESOURCE_NOT_INIT;
    }

    if (gIDMem.uiDataSize == 0)
    {
        DBG_WRN("There is no data!\n");
        return ER_GET_DATA_FAIL;
    }

    if (name == NULL || value == NULL)
    {
        DBG_ERR("Invalid input data\n");
        return ER_INVALID_INPUT_DATA;
    }

    if (gIDMem.uiListIdx >= gIDMem.uiDataSize)
        return ER_RESOURCE_OUT_OF_RANGE;

    startPtr = gIDMem.cMemBuf + gIDMem.uiListIdx;
    if ((token = strstr(startPtr, ID_VALUE_SEPARATOR)) != NULL)
    {
        unsigned int tokenLen;

        tokenLen = token - startPtr;
        strncpy(name, startPtr, tokenLen);
        name[tokenLen] = '\0';
        token += strlen(ID_VALUE_SEPARATOR);
        startPtr = token;
        if ((token = strstr(startPtr, ID_PAIR_SEPARATOR)) != NULL)
        {
            tokenLen = token - startPtr;
            strncpy(value, startPtr, tokenLen);
            value[tokenLen] = '\0';
            token += strlen(ID_PAIR_SEPARATOR);
            gIDMem.uiListIdx = token - gIDMem.cMemBuf;
        }
        else
        {
            DBG_ERR("Can't find pair separator!\n");
            return ER_GET_DATA_FAIL;
        }
    }
    else
    {
        DBG_ERR("Can't find value separator!\n");
        return ER_GET_DATA_FAIL;
    }

    return ER_SUCCESS;
}
NVTRET NvtDevInfo_ID_Find(const char name[ID_ITEM_LEN_MAX], char value[ID_ITEM_LEN_MAX])
{
    char tmpName[ID_ITEM_LEN_MAX]={0}, tmpValue[ID_ITEM_LEN_MAX]={0};

    if (gIDMem.uiBufSize == 0)
    {
        DBG_ERR("NvtDevinfo ID not initialized!\n");
        return ER_RESOURCE_NOT_INIT;
    }

    if (gIDMem.uiDataSize == 0)
    {
        DBG_WRN("There is no data!\n");
        return ER_GET_DATA_FAIL;
    }

    if (name == NULL || value == NULL || strlen(name) == 0)
    {
        DBG_ERR("Invalid input data\n");
        return ER_INVALID_INPUT_DATA;
    }

    NvtDevInfo_ID_List_Reset();

    while (NvtDevInfo_ID_List_Each(tmpName, tmpValue) == ER_SUCCESS)
    {
        if (!strcmp(name, tmpName))
        {
            strcpy(value, tmpValue);
            return ER_SUCCESS;
        }
    }

    return ER_RESOURCE_NOT_EXIST;
}

NVTRET NvtDevInfo_ID_Set(const char name[ID_ITEM_LEN_MAX], const char value[ID_ITEM_LEN_MAX])
{
    char *startPtr=NULL, *endPtr=NULL, *token=NULL;
    unsigned int nameLen, valueLen;
    BOOL justClearFlag=NVT_FALSE, findFlag=NVT_FALSE;

    if (gIDMem.uiBufSize == 0)
    {
        DBG_ERR("NvtDevinfo ID not initialized!\n");
        return ER_RESOURCE_NOT_INIT;
    }

    if (name == NULL || (nameLen = strlen(name)) == 0)
    {
        DBG_ERR("Invalid input data\n");
        return ER_INVALID_INPUT_DATA;
    }

    if (value == NULL || (valueLen = strlen(value)) == 0)
        justClearFlag = NVT_TRUE;

    /* If buffer is empty, handle ID directly */
    if (gIDMem.uiDataSize == 0)
    {
        if (justClearFlag == NVT_TRUE)
        {
            DBG_IND("ID %s not exist!\n", name);
            return ER_SUCCESS;
        }

        strcpy(gIDMem.cMemBuf, name);
        gIDMem.uiDataSize += nameLen;
        strcat(gIDMem.cMemBuf, ID_VALUE_SEPARATOR);
        gIDMem.uiDataSize += strlen(ID_VALUE_SEPARATOR);
        strcat(gIDMem.cMemBuf, value);
        gIDMem.uiDataSize += strlen(value);
        strcat(gIDMem.cMemBuf, ID_PAIR_SEPARATOR);
        gIDMem.uiDataSize += strlen(ID_PAIR_SEPARATOR);

        return ER_SUCCESS;
    }

    if (justClearFlag != NVT_TRUE &&
        (gIDMem.uiDataSize + nameLen + valueLen + strlen(ID_VALUE_SEPARATOR) + strlen(ID_PAIR_SEPARATOR)) >
        gIDMem.uiBufSize)
    {
        DBG_ERR("Buffer size out of range\n");
        return ER_RESOURCE_OUT_OF_RANGE;
    }

    gIDMem.uiListIdx = 0;
    startPtr = gIDMem.cMemBuf;

    /* Find ID and clear */
    while (startPtr < (gIDMem.cMemBuf + gIDMem.uiDataSize))
    {
        if ((token = strstr(startPtr, ID_VALUE_SEPARATOR)) != NULL)
        {
            unsigned int tokenLen;

            tokenLen = token - startPtr;

            if (nameLen == tokenLen && !strncmp(name, startPtr, tokenLen))
            {
                DBG_IND("Find ID %s\n", name);
                findFlag = NVT_TRUE;
            }

            endPtr = token + strlen(ID_VALUE_SEPARATOR);
            if ((token = strstr(endPtr, ID_PAIR_SEPARATOR)) != NULL)
            {
                endPtr = token + strlen(ID_PAIR_SEPARATOR);
            }
            else
            {
                DBG_ERR("Can't find pair separator!\n");
                return ER_GET_DATA_FAIL;
            }

            /* Find ID, clear it */
            if (findFlag == NVT_TRUE)
            {
                unsigned int moveLen, clearLen = endPtr - startPtr;

                while (endPtr < (gIDMem.cMemBuf + gIDMem.uiDataSize))
                {
                    if ((gIDMem.uiDataSize - (endPtr - gIDMem.cMemBuf)) < clearLen)
                        moveLen = gIDMem.uiDataSize - (endPtr - gIDMem.cMemBuf);
                    else
                        moveLen = clearLen;

                    memcpy(startPtr, endPtr, moveLen);
                    startPtr = endPtr;
                    endPtr += moveLen;
                }

                gIDMem.uiDataSize -= clearLen;
                gIDMem.cMemBuf[gIDMem.uiDataSize] = '\0';
                break;
            }
            /* Not find, set startPtr to next ID pair */
            else
            {
                startPtr = endPtr;
            }
        }
        else
        {
            DBG_ERR("Can't find value separator!\n");
            return ER_GET_DATA_FAIL;
        }
    }

    /* Append new ID in buffer */
    if (justClearFlag != NVT_TRUE)
    {
        strcat(gIDMem.cMemBuf, name);
        gIDMem.uiDataSize += nameLen;
        strcat(gIDMem.cMemBuf, ID_VALUE_SEPARATOR);
        gIDMem.uiDataSize += strlen(ID_VALUE_SEPARATOR);
        strcat(gIDMem.cMemBuf, value);
        gIDMem.uiDataSize += strlen(value);
        strcat(gIDMem.cMemBuf, ID_PAIR_SEPARATOR);
        gIDMem.uiDataSize += strlen(ID_PAIR_SEPARATOR);
    }

    DBG_IND("Current buf(Length=%lu) : \n%s\n\n", gIDMem.uiDataSize, gIDMem.cMemBuf);

    return ER_SUCCESS;
}

