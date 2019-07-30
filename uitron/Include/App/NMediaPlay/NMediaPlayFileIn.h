#ifndef _NMEDIAPLAYFILEIN_H
#define _NMEDIAPLAYFILEIN_H

// for TS format
typedef enum {
	NMI_FILEIN_TS_BLK_SIZE  = 0x200000, ///< one block size
	NMI_FILEIN_TS_PL_BLKNUM = 3,        ///< preload block number
} NMI_FILEIN_TS_BUFINFO;

typedef enum {
	NMI_FILEIN_PARAM_FILEHDL,           ///< file handle
	NMI_FILEIN_PARAM_FILESIZE,          ///< total file size
	NMI_FILEIN_PARAM_FILEDUR,           ///< total file duration (sec)
	NMI_FILEIN_PARAM_FILEFMT,           ///< file format
	NMI_FILEIN_PARAM_BLK_TIME,          ///< play time per one block (e.g. 1 sec per block)
	NMI_FILEIN_PARAM_TT_BLKNUM,         ///< total block number
	NMI_FILEIN_PARAM_PL_BLKNUM,         ///< preload block number
	NMI_FILEIN_PARAM_RSV_BLKNUM,        ///< reserve block number (as buffer area)
	NMI_FILEIN_PARAM_NEED_MEMSIZE,      ///< get internal need mem size
	//NMI_FILEIN_PARAM_BLK_STARTADDR,     ///< file block starting addr
	NMI_FILEIN_PARAM_MEM_RANGE,         ///< allocated mem addr & size
	NMI_FILEIN_PARAM_CUR_VDOBS,         ///< current used video bs addr & size
	NMI_FILEIN_PARAM_EVENT_CB,          ///< callback event
} NMI_FILEIN_PARAM;

typedef enum {
	NMI_FILEIN_ACTION_PRELOAD,            ///< reload file block for initialization
	NMI_FILEIN_ACTION_MAX,
} NMI_FILEIN_ACTION;

typedef enum {
	NMI_FILEIN_STATE_PRELOAD,            ///< preload state
	NMI_FILEIN_STATE_NORMAL,             ///< normal state
} NMI_FILEIN_STATE;

typedef struct {
	UINT32 Addr;    ///< Memory buffer starting address
	UINT32 Size;    ///< Memory buffer size
} NMI_FILEIN_MEM_RANGE, *PNMI_FILEIN_MEM_RANGE;

typedef struct {
	UINT32 blk_time;                    ///< block time for one block (ms)
	UINT32 tt_blknum;                   ///< total block number
	UINT32 pl_blknum;                   ///< preload block number
	UINT32 rsv_blknum;                  ///< reserve block number (as buffer area)
} NMI_FILEIN_USERBUF_INFO, *PNMI_FILEIN_USERBUF_INFO;

extern void NMP_FileIn_AddUnit(void);

#endif //_NMEDIAPLAYFILEIN_H

