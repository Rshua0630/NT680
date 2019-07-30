/**

    This header file of nvtrecord APIs.

    This header file of nvtrecord APIs for query record config

    @file       nvtrecord.h
    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.

*/

#ifndef _NVTRECORD_H
#define _NVTRECORD_H
#ifdef __cplusplus
extern "C" {
#endif

#include <nvtdevinfo.h>
#include <nvtcodec.h>

#define RECORD_UNIX_SOCKET_SESSION "/tmp/unix_socket_record"
#define RECORD_NOTIFY_CHANGE "NOTIFY_CHANGE"



typedef struct _RECORD_CFG{

    RECORD_ENABLE enable;
    RECORD_ENABLE schedule_always;
    int schedule[7];//for Monday ~Sunday, using 24 bits to set 24 hours   
    int video_len; //set video segment len, (min)
    NVT_RECORD_TYPE record_type;
    char sd_name[128];
    char iscsi_name[128];

}RECORD_CFG;



typedef enum _RECORD_ER{

    RECORD_ER_OK    =  0,// no error
    RECORD_ER_GRR   = -1// general error


}RECORD_ER;


RECORD_ER nvtRecord_get_record_cfg(RECORD_CFG *record_cfg);
RECORD_ER nvtRecord_set_record_cfg(RECORD_CFG *record_cfg);


#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif 








