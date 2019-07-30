/**
    Public header file for ipl obj

    This file is the header file that define the API and data type for ipl ctrl obj.

    @file       ipl_obj.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _PQ_OBJ_H
#define _PQ_OBJ_H

#include "Type.h"
#include "sensor.h"

#define SENSOR_PARAM_MAX    SENSOR_ID_MAX_NUM
#define AE_PARAM_ID_MAX     8
#define AWB_PARAM_ID_MAX    8
#define IQ_PARAM_ID_MAX     8

/**
    iq libext init function
*/
typedef struct{
	void (*_iqs_param_init)(UINT32 Id);
} IQS_HANDLE;

typedef struct {
	CHAR *name;
	void *_iqs_param_init;
} IQS_MAP_INFO;

extern BOOL pqs_iq_check(UINT32 id);
extern BOOL pqs_iq_init(UINT32 id, IQS_HANDLE *handle);

/**
    awb libext init function
*/
typedef struct{
	UINT32 (*_awb_param_init)(UINT32 *init);
} AWB_HANDLE;

typedef struct {
	CHAR *name;
	void  *_awb_param_init;
} AWB_MAP_INFO;

extern BOOL pqs_awb_check(UINT32 id);
extern BOOL pqs_awb_init(UINT32 id, UINT32 *init);

/**
    ae libext init function
*/
typedef struct{
	UINT32 (*_ae_param_init)(UINT32 *init);
} AE_HANDLE;

typedef struct {
	CHAR *name;
	void *_ae_param_init;
} AES_MAP_INFO;

extern BOOL pqs_ae_check(UINT32 id);
extern BOOL pqs_ae_init(UINT32 id, UINT32 *init);

/**
    pqs libext setting name
*/
typedef struct {
    CHAR isp_name[32];
    CHAR sensor_name[SENSOR_PARAM_MAX][32];
	CHAR iqs_name[IQ_PARAM_ID_MAX][32];
	CHAR awb_name[AWB_PARAM_ID_MAX][32];
	CHAR aes_name[AE_PARAM_ID_MAX][32];
} pqs_drv_INFO;

extern pqs_drv_INFO *pqs_get_drv_name(void);
//@}
#endif
