/*
    Copyright   Novatek Microelectronics Corp. 2005~2017.  All rights reserved.

    @file       ImageUnit_FileOut.h
    @ingroup    mFileOut

    @note       Nothing.

    @date       2017/04/12
*/

#ifndef IMAGEUNIT_FILEOUT_H
#define IMAGEUNIT_FILEOUT_H

#include "ImageStream.h"
#include "NMediaRecFileOut.h"

#define ISF_FILEOUT_IN_NUM        16
#define ISF_FILEOUT_OUT_NUM       16

typedef enum {
	FILEOUT_PARAM_START                 = 0x0000F000,
	FILEOUT_PARAM_MAX_FILE              = FILEOUT_PARAM_START,
	FILEOUT_PARAM_EVENT_CB,
	FILEOUT_PARAM_A_RESV_SIZE_IMM,
	FILEOUT_PARAM_B_RESV_SIZE_IMM,
	FILEOUT_PARAM_GET_REMAIN_SIZE_A,
	FILEOUT_PARAM_GET_REMAIN_SIZE_B,
	FILEOUT_PARAM_REG_CB,
	FILEOUT_PARAM_MAX_POP_SIZE_IMM, //max pop size to be merged
	FILEOUT_PARAM_MAX,
	ENUM_DUMMY4WORD(FILEOUT_PARAM)
} FILEOUT_PARAM;

typedef enum {
	FILEOUT_PARAM_PORT_START            = 0x0000FF00,
	FILEOUT_PARAM_PORT_REG_CB           = FILEOUT_PARAM_PORT_START,
	FILEOUT_PARAM_PORT_MAX,
	ENUM_DUMMY4WORD(FILEOUT_PARAM_PORT)
} FILEOUT_PARAM_PORT;

typedef enum {
	FILEOUT_CB_EVENT_NAMING,
	FILEOUT_CB_EVENT_CLOSED,
	FILEOUT_CB_EVENT_DELETE,
	FILEOUT_CB_EVENT_LOOPREC_FULL,
	ENUM_DUMMY4WORD(FILEOUT_CB_EVENT)
} FILEOUT_CB_EVENT;

typedef struct {
	UINT32 iport;
	UINT32 event;
	UINT32 type;
	char *p_fpath; //to get a new filename
	UINT32 fpath_size; //buffer size of the filename
} FILEOUT_CB_EVDATA_NAMING;

typedef struct {
	UINT32 iport;
	char *p_fpath;
	UINT32 fpath_size;
} FILEOUT_CB_EVDATA_CLOSED;

typedef struct {
	UINT32 port_num;
	UINT32 remain_size;
	CHAR drive;
} FILEOUT_CB_EVDATA_DELETE;

typedef INT32 (*FILEOUT_EVENT_CB)(CHAR *p_name, FILEOUT_CB_EVENT event_id, void *p_evdata);

extern ISF_UNIT ISF_FileOut;

extern void ISF_FileOut_InstallID(void) _SECTION(".kercfg_text");

#endif //IMAGEUNIT_FILEOUT_H

