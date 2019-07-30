#ifndef _NVTDEVINFO_CTRL_H
#define _NVTDEVINFO_CTRL_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtdevinfo_ctrl.h

    @version

    @date
*/
#include <stdio.h>
#include <time.h>
#include <nvtcfg_define.h>
#include "nvt_type.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Communication with nvtsystem */
#define SYS_MSG_QUEUE_NAME  "/nvtsystem.mq"
#define SYS_MSG_NUM_MAX     10   // /proc/sys/fs/mqueue/msg_max is 10
#define SYS_MSG_LEN_MAX     256  // /proc/sys/fs/mqueue/msgsize_max is 8192

#define ONVIF_DEVICE_MGT_MSG_QID    0x4320
#define ONVIF_DEVICE_MGT            1
#define ONVIF_DEVICE_MGT_HELLO      20
#define ONVIF_DEVICE_MGT_BYE		21

/*
   Command format : command:content
   Example        : ConfigChange:networkInfo,mediaInfo
                    ConfigReset:allConfig
                    ConfigReset:-networkInfo,mediaInfo
                    ActionApply:reboot
*/
#define SYS_CMD_CFG_CHANGE  "ConfigChange"
#define SYS_CMD_CFG_RESET   "ConfigReset"
#define SYS_CMD_ACT_APPLY   "ActionApply"

#define SYS_ACT_REBOOT      "reboot"
#define SYS_ACT_NTP_UPDATE  "ntpUpdate"
#define SYS_ACT_FW_UPGRADE  "FWUpgrade"
#define SYS_ACT_RTC_TZ_SYNC "RTCTZSync"
#define SYS_ACT_PT_CTRL     "PTCtrl"

typedef enum {
    PT_MOVE_ACTION_STOP=0,
    PT_MOVE_ACTION_UP,
    PT_MOVE_ACTION_RIGHTUP,
    PT_MOVE_ACTION_RIGHT,
    PT_MOVE_ACTION_RIGHTDOWN,
    PT_MOVE_ACTION_DOWN,
    PT_MOVE_ACTION_LEFTDOWN,
    PT_MOVE_ACTION_LEFT,
    PT_MOVE_ACTION_LEFTUP,
    PT_MOVE_ACTION_MAX
} PT_MOVE_ACTION;

#define CRONTAB_USER_PATH   "/var/spool/cron/user_define/"
#define CRONTAB_UPDATE_FILE "/var/spool/cron/crontabs/cron.update"

NVTRET NvtDevInfo_NotifyMsg_Send(const char *cmdMsg, const int cmdMsgLen);
NVTRET ONVIF_device_sendHelloMsg(void);

NVTRET NvtDevInfo_Crontab_Update(void);
/*
    tmSetup don't need handle Linux date base(1900) or RTC date base(1970)
    Just setup normal date format : 2016/1/5 10:15:20
*/
NVTRET ONVIF_dateTime_setup(struct tm *tmSetup);
NVTRET ONVIF_timeZone_setup(const char *tzString, const char *dstString, enum xsd__boolean bDSTEnable);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTDEVINFO_CTRL_H  */
