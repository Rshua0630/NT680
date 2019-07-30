/*
    Copyright   Novatek Microelectronics Corp. 2005~2018.  All rights reserved.

    @file       ImageUnit_FileIn.h
    @ingroup    mFileIn

    @note       Nothing.

    @date       2018/03/06
*/

#ifndef IMAGEUNIT_FILEIN_H
#define IMAGEUNIT_FILEIN_H

#include "ImageStream.h"

typedef enum {
	FILEIN_PARAM_START = 0x00001000,
	FILEIN_PARAM_FILEHDL = FILEIN_PARAM_START,       ///< file handle
	FILEIN_PARAM_FILESIZE,                           ///< total file size
	FILEIN_PARAM_FILEDUR,                            ///< total file duration (sec)
	FILEIN_PARAM_FILE_FMT,                            ///< file format
	FILEIN_PARAM_BLK_TIME,                           ///< play time per block
	FILEIN_PARAM_BLK_PLNUM,                          ///< preload block number
	FILEIN_PARAM_BLK_TTNUM,                          ///< total block number
	FILEIN_PARAM_CUR_VDOBS,                          ///< current used video bs addr & size
	FILEIN_PARAM_MEM_RANGE,                          ///< get memory range
	FILEIN_PARAM_EVENT_CB,                           ///< callback event
	FILEIN_PARAM_MAX,
	ENUM_DUMMY4WORD(FILEIN_PARAM)
} FILEIN_PARAM;

typedef enum {
	FILEIN_IN_TYPE_CHK_FILEBLK = 0,                 ///< checking is it the target bs in valid block range
	FILEIN_IN_TYPE_READ_ONEFRM,                     ///< read one frame only (for ff/fr, step play)
	FILEIN_IN_TYPE_RELOAD_BUF,                      ///< re-init buffer info and read one file block
	ENUM_DUMMY4WORD(FILEIN_IN_TYPE)
} FILEIN_IN_TYPE;

typedef enum {
	ISF_FILEIN_EVENT_MEM_RANGE = 0,                 ///< FileIn memory range
	ISF_FILEIN_EVENT_READ_ONE_BLK,
	ENUM_DUMMY4WORD(ISF_FILEIN_CB_EVENT)
} ISF_FILEIN_CB_EVENT;

/*typedef enum {
	FILEIN_PARAM_PORT_START = 0x00011000,
	FILEIN_PARAM_PORT_MAX,
	ENUM_DUMMY4WORD(FILEIN_PARAM_PORT)
} FILEIN_PARAM_PORT;*/

/**
	FileIn callback info
*/
typedef struct {
	UINT64 read_accum_offset;                        ///< [out]accumulation of file reading data
	UINT64 last_used_offset;                         ///< [out]accumulation of last used data
	UINT64 bs_offset;                                ///< [in]target bs offset
	UINT32 bs_size;                                  ///< [in]target bs size
	UINT32 blk_start_addr;                           ///< [out]read file starting addr
	UINT64 blk_size;                                 ///< [out]file block size
	UINT32 tt_blknum;                                ///< [out]total file block number
	UINT32 event;                                    ///< [in] read input event type (refer to FILEIN_IN_TYPE)
	//ER     er;                                       ///< [out]error code
} FILEIN_CB_INFO, *PFILEIN_CB_INFO;

extern ISF_UNIT ISF_FileIn;

typedef void (IsfFileInEventCb)(CHAR *Name, UINT32 event_id, UINT32 value);

#endif //IMAGEUNIT_FILEIN_H

