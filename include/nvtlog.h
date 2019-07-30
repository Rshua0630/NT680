/**

    This header file of nvtlog APIs.

    This header file of nvtlog APIs can save/get log from database 

    @file       nvtlog.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

*/


#include <stdio.h>
#define PATH_LOG_DB "/etc/NVT_LOG.db3"
#define LOG_TABLE "NVT_LOG"


#ifdef __cplusplus
extern "C"{
#endif

typedef enum _LOG_TYPE{LT_INFO, LT_WARNING, LT_ERROR, LT_ALL} LOG_TYPE;

typedef enum _LOG_SORT_NAME{LSN_TYPE, LSN_DATE, LSN_MSG} LOG_SOURT_NAME;
typedef enum _LOG_SORT_DIR{LSD_ASC, LSD_DESC} LOG_SORT_DIR;

#define MAX_LOG_MSG 256
#define NVT_LOG_TYPE_INFO "INFO"
#define NVT_LOG_TYPE_WARNING "WARNING"
#define NVT_LOG_TYPE_ERROR "ERROR"
#define NVT_LOG_TYPE_ALL "ALL"

typedef struct _NVT_LOG{

    struct _NVT_LOG *next;
    LOG_TYPE type;
    char date[64];
    char message[MAX_LOG_MSG];

}NVT_LOG;


typedef struct _NVT_LOG_LIST{

    int state;  //0:ok   -1:fail
    int tdata_count; // the number of data by type value
    int query_count; // the number of date by actually get
    NVT_LOG *log; 

}NVT_LOG_LIST;

typedef struct _NVT_LOG_CFG{

    LOG_TYPE type;
    int start;
    int limit;// number of log will shoe
    LOG_SOURT_NAME sort_name;
    LOG_SORT_DIR dir;

}NVT_LOG_CFG;


int nvt_set_log(LOG_TYPE type,char *data);
NVT_LOG_LIST *nvt_attach_log (LOG_TYPE type, int start, int limit, LOG_SOURT_NAME sort_name, LOG_SORT_DIR dir);
int nvt_detach_log (NVT_LOG_LIST * log_list);
int nvt_reset_log(void);

#ifdef __cplusplus
}
#endif

