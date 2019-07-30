/**
    IPL calibration data read/write dram & storage

    @file       ipl_cal_rw.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_RW_H_
#define _IPL_CAL_RW_H_

#include "Type.h"
#include "ErrorNo.h"
#include "stdio.h"
#include "ipl_utility.h"
#include "ipl_calibration.h"
#include "ipl_ini.h"

#define CAL_RW_TYPE_PSTORE 0
#define CAL_RW_TYPE_FILESYSTEM 1

#define CAL_RW_TYPE_SEL CAL_RW_TYPE_PSTORE

#define CAL_HEADER_SEC_LEN 12
#define CAL_HEADER_KEY_LEN 100
#define CAL_HEADER_VAL_LEN 100
#if (CAL_RW_TYPE_SEL == CAL_RW_TYPE_PSTORE)
#define CAL_FILENAME_LEN (12) // pstore.h #define SEC_NAME_LEN 12
#elif (CAL_RW_TYPE_SEL == CAL_RW_TYPE_FILESYSTEM)
#define CAL_FILENAME_LEN (100)
#else
#define CAL_FILENAME_LEN (100)
#endif

// need to mapping cal_rw_inival_string2value!!!!!!!!
#define HEADER_VAL_MAX_DATA_NUM 5
typedef struct {
	CAL_STATUS cal_status;
	UINT32 data[HEADER_VAL_MAX_DATA_NUM]; // for CAL_ALG_XX_RST
	UINT32 read_data_size; // for ECS/DP/... big data
} HEADER_VAL;

typedef struct {
	HEADER_SEC sec;
	HEADER_KEY key;
	HEADER_VAL value;
} CAL_RW_HEADER_INFO;

/*
typedef struct {
	HEADER_SEC sec;
	HEADER_KEY key;
} CAL_RW_DATA_INFO;
*/

/*
    ini format

    [section_A]
    key_1 = value_1
    key_2 = value_2
    ...
    [section_B]
    key_5 = value_3
    key_2 = value_4
    ...
*/
typedef struct {
	ER(*fp_init)(CAL_RW_STORE_OP *rw_store_fp);
	ER(*fp_uninit)(void); // need to check close header

	ER(*fp_header_open)(MEM_RANGE mem_range);
	ER(*fp_header_close)(void);
	ER(*fp_header_write)(CAL_RW_HEADER_INFO *info);
	ER(*fp_header_read)(CAL_RW_HEADER_INFO *info, UINT32 *size); // input: sec,key / output: value,size, E_OBJ: null section or key
	ER(*fp_header_delete)(CAL_RW_HEADER_INFO *info);

	ER(*fp_header_set_dram_sft)(CAL_RW_DATA_INFO *info, UINT32 sft);
	ER(*fp_header_get_dram_sft)(CAL_RW_DATA_INFO *info, UINT32 *sft);

	// read/write data to storage (ex:Pstore,efuse,...)
	ER(*fp_storage_write)(CAL_RW_DATA_INFO *info, UINT32 buf_addr, UINT32 buf_size); // need to write after header
	ER(*fp_storage_read)(CAL_RW_DATA_INFO *info, UINT32 buf_addr, UINT32 buf_size);
	ER(*fp_storage_delete)(CAL_RW_DATA_INFO *info);
	ER(*fp_storege_update_dram_data)(UINT32 start_addr);

#if 0
	// transfer ini string
	CHAR *(*fp_sec_value2string)(CAL_ITEM cal_item);
	ER(*fp_sec_string2value)(CHAR *inisecname, CAL_ITEM *cal_item);
	CHAR *(*fp_key_value2string)(IPL_PROC_ID id, SENSOR_MODE mode, CAL_COND cal_cond);
	ER(*fp_key_string2value)(CHAR *inikeyname, IPL_PROC_ID *id, SENSOR_MODE *mode, CAL_COND *cal_cond);
	ER(*fp_sec_string2value)(CHAR *inisecname, HEADER_SEC *sec);
	ER(*fp_key_string2value)(CHAR *inikeyname, HEADER_KEY *key);
#endif

	UINT32(*fp_get_storage_header_size)(void);
	UINT32(*fp_get_storage_data_size)(IPL_PROC_ID id, HEADER_SEC sec);

	UINT32(*fp_get_key_num)(HEADER_SEC sec);
	UINT32(*fp_get_key_length)(void);
	UINT32(*fp_get_val_length)(void);

	BOOL(*fp_search_sec_init)(HEADER_SEC sec);
	BOOL(*fp_search_sec)(HEADER_KEY *key, HEADER_VAL *val);

} CAL_RW_OBJ, *pCAL_RW_OBJ;

extern pCAL_RW_OBJ cal_rw(void);
//@}
#endif

