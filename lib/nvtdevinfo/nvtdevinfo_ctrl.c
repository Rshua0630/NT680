/* =================================================================
 *
 *      nvtdevinfo_ctrl.c
 *
 *      Device config and inforation library for Novatek NT9666x
 *
 * =================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/rtc.h>
#include "nvtdevinfo.h"
#include "nvtdevinfo_ctrl.h"

//#define DEBUG
#define PRINT_OUTFILE stderr
#define DBG_WRN(fmtstr, args...)  fprintf(PRINT_OUTFILE, "\033[33m%s(): \033[0m" fmtstr,__func__, ##args)

#define DBG_ERR(fmtstr, args...)  fprintf(PRINT_OUTFILE, "\033[31m%s(): \033[0m" fmtstr,__func__, ##args)

#define DBG_DUMP(fmtstr, args...) fprintf(PRINT_OUTFILE, fmtstr, ##args)

#ifndef DEBUG
#define DBG_IND(fmtstr, args...)
#else
#define DBG_IND(fmtstr, args...)  fprintf(PRINT_OUTFILE, "%s(): " fmtstr, __func__, ##args)
#endif

#define TIMEZONE_FILE   "/etc/timezone/localtime"
#define TIMEZONE_BASE   "/etc/timezone/localtime.base"
#define TIMEZONE_TEMP   "/etc/timezone/localtime.tmp"

enum tt__UserLevel gUserLevel=tt__UserLevel__Extended;


/* =================================================================
 *   Private Functions
 * =================================================================
 */

/* Notify onvif to send hello message */
typedef struct _DEVICE_MGT_IPC_MSG{
    long      mtype;                  ///<   msg type
    long      uiIPCcmd;               ///<
}DEVICE_MGT_IPC_MSG;

static int getDeviceMgtQueueID(void)
{
	int  deviceQid_int;
	key_t key ;
	key = ONVIF_DEVICE_MGT_MSG_QID;
	deviceQid_int = msgget(key,0);

	if(  deviceQid_int  < 0 )
	{
		deviceQid_int  = msgget(key,IPC_CREAT|0666);
	}
    if(  deviceQid_int  < 0 )
	{
		DBG_ERR("msgget\r\n");
	}
	return deviceQid_int;
}

typedef struct
{
    char tzName[32];
    char tzInfo[16];
    char dstShotName[8];
} TIME_ZONE_ELEMENT;

TIME_ZONE_ELEMENT gTimeZoneTable[] =
{
    // -12
    {"DatelineStandardTime", "<GMT+12>+12", "GMST"},
    // -11
    {"<UTC-11>", "<GMT+11>+11", "GMST"},
    {"SamoaStandardTime", "SST+11", "SDT"},
    // -10
    {"HawaiianStandardTime", "HST10", "GMST"},
    // -9
    {"AlaskanStandardTime", "AKST9", "AKDT"},
    // -8
    {"PacificStandardTimeMexico", "PST8", "PDT"},
    {"PacificStandardTime", "<GMT+8>+8", "GMST"},
    // -7
    {"MountainStandardTime", "MST7", "MDT"},
    {"USMountainStandardTime", "MST7", "MDT"},
    {"MountainStandardTimeMexico", "MST7", "MDT"},
    // -6
    {"CentralAmericaStandardTime", "CST6", "CDT"},
    {"CentralStandardTime", "CST6", "CDT"},
    {"CentralStandardTimeMexico", "CST6", "CDT"},
    {"CanadaCentralStandardTime", "CST6", "CDT"},
    // -5
    {"USEasternStandardTime", "EST5", "EDT"},
    {"EasternStandardTime", "EST5", "EDT"},
    {"SAPacificStandardTime", "COT5", "COST"},
    // -4:30
    {"VenezuelaStandardTime", "VET4:30", "VEST"},
    // -4
    {"AtlanticStandardTime", "AST4", "ADT"},
    {"CentralBrazilianStandardTime", "AMT4", "AMST"},
    {"SAWesternStandardTime", "BOT4", "BOST"},
    {"ParaguayStandardTime", "PYT4", "PYST"},
    {"PacificSAStandardTime", "CLT3", "CLST"},
    // -3:30
    {"NewfoundlandStandardTime", "NST3:30", "NDT"},
    // -3
    {"ESouthAmericaStandardTime", "BRT3", "BRST"},
    {"ArgentinaStandardTime", "ART3", "ARST"},
    {"GreenlandStandardTime", "WGT3", "WGST"},
    {"SAEasternStandardTime", "GFT3", "GFST"},
    {"MontevideoStandardTime", "UYT3", "UYST"},
    // -2
    {"<Mid-AtlanticStandardTime>", "GST2", "GSDT"},
    {"<UTC-02>", "<GMT+2>+2", "GMST"},
    // -1
    {"AzoresStandardTime", "AZOT1", "AZOST"},
    {"CapeVerdeStandardTime", "CVT1", "CVST"},
    // 0
    {"MoroccoStandardTime", "WET0", "WEST"},
    {"UTC", "UTC", "UTST"},
    {"GMTStandardTime", "GMT0", "BST"},
    {"GreenwichStandardTime", "<GMT>0", "GMST"},
    // +1
    {"WCentralAfricaStandardTime", "WAT-1", "WAST"},
    {"RomanceStandardTime", "CET-1", "CEST"},
    {"CentralEuropeStandardTime", "CET-1", "CEST"},
    {"WEuropeStandardTime", "CET-1", "CEST"},
    {"CentralEuropeanStandardTime", "CET-1", "CEST"},
    {"NamibiaStandardTime", "WAT-1", "WAST"},
    // +2
    {"SyriaStandardTime", "EET-2", "EEST"},
    {"JordanStandardTime", "EET-2", "EEST"},
    {"MiddleEastStandardTime", "EET-2", "EEST"},
    {"EEuropeStandardTime", "EET-2", "EEST"},
    {"SouthAfricaStandardTime", "SAST-2", "SADT"},
    {"IsraelStandardTime", "IST-2", "IDT"},
    {"EgyptStandardTime", "EET-2", "EEST"},
    {"GTBStandardTime", "EET-2", "EEST"},
    {"FLEStandardTime", "EET-2", "EEST"},
    // +3
    {"ArabicStandardTime", "AST-3", "ADT"},
    {"EAfricaStandardTime", "EAT-3", "EAST"},
    {"ArabStandardTime", "AST-3", "ADT"},
    {"RussianStandardTime", "MSK-3", "MSD"},
    {"IranStandardTime", "IRST-3", "IRDT"},
    // +4
    {"AzerbaijanStandardTime", "AZT-4", "AZST"},
    {"ArabianStandardTime", "GST-4", "GDT"},
    {"GeorgianStandardTime", "GET-4", "GEST"},
    {"CaucasusStandardTime", "AMT-4", "AMST"},
    {"MauritiusStandardTime", "MUT-4", "MUST"},
    // +4:30
    {"AfghanistanStandardTime", "AFT-4:30", "AFST"},
    // +5
    {"EkaterinburgStandardTime", "YEKT-5", "YEKST"},
    {"PakistanStandardTime", "PKT-5", "PKST"},
    {"WestAsiaStandardTime", "UZT-5", "UZST"},
    // +5:30
    {"IndiaStandardTime", "IST-5:30", "IDT"},
    // +5:45
    {"NepalStandardTime", "NPT-5:45", "NPST"},
    // +6
    {"CentralAsiaStandardTime", "ALMT-6", "ALMST"},
    {"NCentralAsiaStandardTime", "NOVT-6", "NOVST"},
    {"BangladeshStandardTime", "BDT-6", "BDST"},
    // +6:30
    {"MyanmarStandardTime", "MMT-6:30", "MMST"},
    // +7:00
    {"NorthAsiaStandardTime", "KRAT-7", "KRAST"},
    {"SEAsiaStandardTime", "ICT-7", "ICST"},
    // +8
    {"ChinaStandardTime", "CST-8", "CDT"},
    {"TaipeiStandardTime", "CST-8", "CDT"},
    {"NorthAsiaEastStandardTime", "IRKT-8", "IRKST"},
    {"SingaporeStandardTime", "SGT-8", "SGST"},
    {"WAustraliaStandardTime", "AWST-8", "AWDT"},
    {"UlaanbaatarStandardTime", "ULAT-8", "ULAST"},
    // +9
    {"TokyoStandardTime", "JST-9", "JDT"},
    {"YakutskStandardTime", "YAKT-9", "YAKST"},
    {"KoreaStandardTime", "KST-9", "KDT"},
    // +9:30
    {"CenAustraliaStandardTime", "ACST-9:30", "ACDT"},
    {"AUSCentralStandardTime", "ACST-9:30", "ACDT"},
    // +10
    {"EAustraliaStandardTime", "AEST-10", "AEDT"},
    {"AUSEasternStandardTime", "AEST-10", "AEDT"},
    {"VladivostokStandardTime", "VLAT-10", "VLST"},
    {"TasmaniaStandardTime", "AEST-10", "AEDT"},
    {"WestPacificStandardTime", "PGT-10", "PGST"},
    // +11
    {"CentralPacificStandardTime", "SBT-11", "SBST"},
    {"MagadanStandardTime", "MAGT-11", "MAGST"},
    // +12
    {"KamchatkaStandardTime", "PETT-12", "PETST"},
    {"<UTC+12>", "<GMT-12>-12", "GMST"},
    {"FijiStandardTime", "FJT-12", "FJST"},
    {"NewZealandStandardTime", "NZST-12", "NZDT"},
    // +13
    {"TongaStandardTime", "TOT-13", "TOST"},
    // End
    {"", "", ""},
};

static const int uiRTCDayOfMonth[2][12] =
{
    // Not leap year
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    // Leap year
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static const int uiRTCDayOfYear[2] = {365, 366};

typedef struct
{
	int day_int;
	int mon_int;
	int year_int;
} dateS;

// Base date
static dateS RTCBaseDate = {1, 1, 1970};

inline int rtc_isLeapYear(int uiYear)
{
    return (int)((((uiYear % 4) == 0) && (((uiYear % 100) != 0) || ((uiYear % 400) == 0))));
}

uint32_t rtc_convertDate2Days(struct tm *tmSetup)
{
    uint32_t uiDays=0;
    int i;

    // Year
    for (i = RTCBaseDate.year_int; i < tmSetup->tm_year; i++)
    {
        uiDays += uiRTCDayOfYear[rtc_isLeapYear(i)];
    }

    // Month
    for (i = RTCBaseDate.mon_int; i < tmSetup->tm_mon; i++)
    {
        uiDays += uiRTCDayOfMonth[rtc_isLeapYear(tmSetup->tm_year)][i-1];
    }

    uiDays += tmSetup->tm_mday - RTCBaseDate.day_int;

    return uiDays;
}

void rtc_set_time(struct tm *tmSetup)
{
    int fd = 0, ret = 0;
    struct rtc_time rtc_tm;

    rtc_tm.tm_sec = tmSetup->tm_sec;           /* second */
    rtc_tm.tm_min = tmSetup->tm_min;           /* minute */
    rtc_tm.tm_hour = tmSetup->tm_hour;         /* hour */
    rtc_tm.tm_mday = tmSetup->tm_mday;         /* day of the month */
    rtc_tm.tm_mon = tmSetup->tm_mon - 1;       /* month Jan=0, Feb=1 ... */
    rtc_tm.tm_year = tmSetup->tm_year - 1900;  /* = year - epoch */

    DBG_IND("RTC time set with: %d:%d:%d %d-%d-%d\n",
                                                rtc_tm.tm_hour,
                                                rtc_tm.tm_min,
                                                rtc_tm.tm_sec,
                                                rtc_tm.tm_year,
                                                (rtc_tm.tm_mon + 1),
                                                rtc_tm.tm_mday
                                                );

    if ((fd = open("/dev/rtc0", O_RDWR)) >= 0)
    {
        ret = ioctl(fd, RTC_SET_TIME, &rtc_tm);
        if (ret == -1)
            DBG_ERR("Failed getting RTC Date/Time, errno = %s!\n", strerror(errno));

        close(fd);
    }
}

void tz_file_handle(const char *tzString)
{
    int fd_base=-1, fd_temp=-1;
    ssize_t tzStringLen = strlen(tzString), defTZStringLen=strlen("CST-8");

    if ((fd_base = open(TIMEZONE_BASE, O_RDONLY)) >= 0)
    {
        if ((fd_temp = open(TIMEZONE_TEMP, O_WRONLY | O_CREAT)) >= 0)
        {
            char buf[256];
            ssize_t nread, nwritten;
            char *out_ptr=buf;

            if ((nread = read(fd_base, buf, sizeof(buf))) < 0)
            {
                DBG_ERR("read %s fail!\n", TIMEZONE_BASE);
                close(fd_base);
                close(fd_temp);
                return;
            }

            if (tzStringLen <= defTZStringLen || (nread + (tzStringLen - defTZStringLen)) <= (ssize_t)sizeof(buf))
            {
                memcpy(buf+nread-defTZStringLen-1, tzString, tzStringLen);
                if (tzStringLen > defTZStringLen)
                    nread += (tzStringLen - defTZStringLen);
                else if (tzStringLen < defTZStringLen)
                    nread -= (defTZStringLen - tzStringLen);
            }

            if (nread >= (ssize_t)sizeof(buf))
                nread = sizeof(buf) - 1;
			else if (nread <= 0)
				nread = 1;

            buf[nread-1] = 0x0A;  // file end mark

            do
            {
                nwritten = write(fd_temp, out_ptr, nread);

                if (nwritten >= 0)
                {
                    nread -= nwritten;
                    out_ptr += nwritten;
                }
                else if (errno != EINTR)
                {
                    close(fd_temp);
                    close(fd_base);
                    DBG_ERR("Write %s fail!\n", TIMEZONE_TEMP);
                    return;
                }
            } while (nread > 0);

            close(fd_temp);
        }
        else
            DBG_ERR("Open %s fail!\n", TIMEZONE_TEMP);

        close(fd_base);
    }
    else
        DBG_ERR("Open %s fail!\n", TIMEZONE_BASE);

    int ret =0;
    ret = unlink(TIMEZONE_FILE);
    if(ret < 0){
        DBG_ERR("unlink error ret:%d\r\n",ret);
    }
    if (rename(TIMEZONE_TEMP, TIMEZONE_FILE) == 0)
    {
	    if (chmod(TIMEZONE_FILE, 0644) < 0)
			DBG_ERR("chmod %s fail\n", TIMEZONE_FILE);
    }
	else
		DBG_ERR("rename %s to %s fail!\n", TIMEZONE_TEMP, TIMEZONE_FILE);
}

/* =================================================================
 *   Public Function
 * =================================================================
 */
NVTRET NvtDevInfo_UserLevel_Set(enum tt__UserLevel newLevel)
{
    if (newLevel < tt__UserLevel__Extended)
        gUserLevel = newLevel;

    return ER_SUCCESS;
}

NVTRET NvtDevInfo_NotifyMsg_Send(const char *cmdMsg, const int cmdMsgLen)
{
    mqd_t mqfd=-1;
    struct mq_attr attr;

    DBG_IND("Send message : %s\n", cmdMsg);

    /* Initialize message queue */
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = SYS_MSG_NUM_MAX;
    attr.mq_msgsize = SYS_MSG_LEN_MAX;
    attr.mq_curmsgs = 0;

    if ((mqfd = mq_open(SYS_MSG_QUEUE_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &attr)) < 0)
    {
        DBG_ERR("mq_open fail(name = %s), cmd = %s, error : %s\n", SYS_MSG_QUEUE_NAME, cmdMsg, strerror(errno));
        return -1;
    }

    if (mq_send(mqfd, cmdMsg, cmdMsgLen, 0) < 0)
    {
        DBG_ERR("mq_send fail(mqfd = %d), cmd = %s, error : %s\n", mqfd, cmdMsg, strerror(errno));
        return -1;
    }

    mq_close(mqfd);

    return ER_SUCCESS;
}

NVTRET ONVIF_device_sendHelloMsg(void)
{
#if 0
    DEVICE_MGT_IPC_MSG   Msg;
	int ret_int=0;
	int deviceQid_int;
    ONVIF_DEVICE_DISCOVERY *pDeviceDiscovery=
        (ONVIF_DEVICE_DISCOVERY *)NvtDevInfo_Cfg_SHMAttach(CFG_DEV_DISCOVER_NAME);

	if (pDeviceDiscovery)
	{
	    DBG_IND("DiscoveryMode = %d\r\n", pDeviceDiscovery->DiscoveryMode);
	    // send hello
	    if (pDeviceDiscovery->DiscoveryMode == tt__DiscoveryMode__Discoverable)
	    {
	        if ((deviceQid_int = getDeviceMgtQueueID()) >= 0)
	        {
		        Msg.mtype = ONVIF_DEVICE_MGT;
		        Msg.uiIPCcmd = ONVIF_DEVICE_MGT_HELLO;
		    	ret_int = msgsnd(deviceQid_int, (void *)&Msg, sizeof(Msg.uiIPCcmd), 0);
	        }
	    }

	    NvtDevInfo_Cfg_SHMDeAttach(pDeviceDiscovery);
	}
    return (ret_int == 0) ? ER_SUCCESS : ER_UNCLASSIFIED_ERROR;
#else
    DBG_ERR("not support\r\n");
    return  ER_UNCLASSIFIED_ERROR;
#endif
}

NVTRET NvtDevInfo_Crontab_Update(void)
{
    char sysCmd[128];
    FILE *fp;

    snprintf(sysCmd, sizeof(sysCmd), "cat %s/*.cron > /var/spool/cron/crontabs/root", CRONTAB_USER_PATH);
    system(sysCmd);

    if ((fp = fopen(CRONTAB_UPDATE_FILE, "w")) != NULL)
    {
        fprintf(fp, "root");
        fclose(fp);
    }

    return ER_SUCCESS;
}

NVTRET ONVIF_dateTime_setup(struct tm *tmSetup)
{
    struct timeval tv = {0};
    uint32_t Days;

    Days = rtc_convertDate2Days(tmSetup);
    DBG_IND("Days = %d\r\n",Days);
    tv.tv_sec = (Days)*24*3600 +(tmSetup->tm_hour)*3600+(tmSetup->tm_min)*60+tmSetup->tm_sec;
    tv.tv_usec = 0;

    DBG_IND("%04d/%02d/%02d \n", tmSetup->tm_year, tmSetup->tm_mon, tmSetup->tm_mday);
    DBG_IND("%02d:%02d:%02d \n", tmSetup->tm_hour, tmSetup->tm_min, tmSetup->tm_sec);

    DBG_IND("time = %ld sec\n", tv.tv_sec);

    if (settimeofday(&tv, NULL))
        DBG_ERR("settimeofday errno2 %s\r\n", strerror(errno));

    rtc_set_time(tmSetup);

    return ER_SUCCESS;
}

NVTRET ONVIF_timeZone_setup(const char *tzString, const char *dstString, enum xsd__boolean bDSTEnable)
{
    unsigned int tzTableIdx=0, tzTableNameLen=0;
    int timezoneMinus=0, timezonevalH=0, timezonevalM=0;
    int timezonstrlen=0;
    int findvalue=0, alphacount=0, i=0;
    unsigned int tzFullInfoLen=0;
    char tzFullInfo[128]={0}, systemCmd[100];

    if (tzString == NULL)
        return ER_INVALID_INPUT_DATA;

    DBG_IND("TimeZone = %s, dst is %s\n", tzString, bDSTEnable == xsd__boolean__true_ ? "on" : "off");

    while ((tzTableNameLen = strlen(gTimeZoneTable[tzTableIdx].tzName)) != 0)
    {
        if (!strncmp(gTimeZoneTable[tzTableIdx].tzName, tzString, tzTableNameLen))
        {
            tzFullInfoLen = snprintf(tzFullInfo, sizeof(tzFullInfo)-1, "%s", gTimeZoneTable[tzTableIdx].tzInfo);
            if (bDSTEnable == xsd__boolean__true_ && tzFullInfoLen < sizeof(tzFullInfo)-1)
                snprintf(tzFullInfo+tzFullInfoLen, sizeof(tzFullInfo)-1-tzFullInfoLen, "%s,%s", gTimeZoneTable[tzTableIdx].dstShotName, dstString);

            tz_file_handle(tzFullInfo);
            snprintf(systemCmd, sizeof(systemCmd), "%s:%s", SYS_CMD_ACT_APPLY, SYS_ACT_RTC_TZ_SYNC);
            NvtDevInfo_NotifyMsg_Send(systemCmd, strlen(systemCmd));
            DBG_IND("set TZ: %s\n", tzFullInfo);
            return ER_SUCCESS;
        }
        tzTableIdx++;
    }

    timezonstrlen = strlen(tzString);

    if (tzString[i] == '<')
    {
        while (tzString[i] != '>')
            i++;

        i++;

        alphacount = i;
    }

    while (i < timezonstrlen)
    {
        if (tzString[i] == '-' || tzString[i] == '+' || tzString[i] == ':' || isdigit(tzString[i]))
        {
            if (alphacount < 3)
            {
                DBG_ERR("TimeZone error1 %s\n", tzString);
                return ER_INVALID_INPUT_DATA;
            }
            findvalue = 1;
            if (tzString[i] == '-' || tzString[i] == '+' || isdigit(tzString[i]))
            {
                if (tzString[i] == '-')
                    timezoneMinus = 1;

                if (!isdigit(tzString[i]))
                    i++;

                do
                {
                    timezonevalH *= 10;
                    timezonevalH += (tzString[i] - '0');
                    i++;
                } while (i < timezonstrlen && isdigit(tzString[i]));
            }

            if (tzString[i] == ':')
            {
                do
                {
                    i++;
                    timezonevalM *= 10;
                    timezonevalM += (tzString[i] - '0');
                } while (i < timezonstrlen && isdigit(tzString[i+1]));
            }
        }
        if (isalpha(tzString[i]))
        {
            alphacount++;
        }
        i++;
    }
    if (!findvalue)
    {
        DBG_ERR("TimeZone error2 %s\n", tzString);
        return ER_INVALID_INPUT_DATA;
    }

    DBG_IND("timezoneMinus = %d ,timezoneval = %d:%d\n", timezoneMinus,
                                                         timezonevalH,
                                                         timezonevalM);

    if (timezoneMinus)
    {
        if (timezonevalM != 0)
            tzFullInfoLen = snprintf(tzFullInfo, sizeof(tzFullInfo)-1, "<GMT-%d>-%d:%d", timezonevalH, timezonevalH, timezonevalM);
        else
            tzFullInfoLen = snprintf(tzFullInfo, sizeof(tzFullInfo)-1, "<GMT-%d>-%d", timezonevalH, timezonevalH);
    }
    else
    {
        if (timezonevalM != 0)
            tzFullInfoLen = snprintf(tzFullInfo, sizeof(tzFullInfo)-1, "<GMT+%d>+%d:%d", timezonevalH, timezonevalH, timezonevalM);
        else
            tzFullInfoLen = snprintf(tzFullInfo, sizeof(tzFullInfo)-1, "<GMT+%d>+%d", timezonevalH, timezonevalH);
    }

    if (bDSTEnable == xsd__boolean__true_ && tzFullInfoLen < sizeof(tzFullInfo)-1)
        snprintf(tzFullInfo+tzFullInfoLen, sizeof(tzFullInfo)-1-tzFullInfoLen, "GMST,%s", dstString);

    tz_file_handle(tzFullInfo);
    snprintf(systemCmd, sizeof(systemCmd), "%s:%s", SYS_CMD_ACT_APPLY, SYS_ACT_RTC_TZ_SYNC);
    NvtDevInfo_NotifyMsg_Send(systemCmd, strlen(systemCmd));
    DBG_IND("set TZ: %s\n", tzFullInfo);

    return ER_ITEM_NOT_SUPPORT;
}

