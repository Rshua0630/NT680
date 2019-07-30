#ifndef _NMEDIAPLAYBSDEMUX_H
#define _NMEDIAPLAYBSDEMUX_H

#include "Type.h"

#define    NMP_BSDEMUX_US_IN_SECOND       (1000*1000)

typedef enum {
	/* set param */
	NMI_BSDEMUX_PARAM_VDO_ENABLE,                             ///< video enable (default: 0)
	NMI_BSDEMUX_PARAM_VDO_CODEC,                              ///< video codec
	NMI_BSDEMUX_PARAM_VDO_FR,                                 ///< video frame rate
	NMI_BSDEMUX_PARAM_VDO_TTFRM,                              ///< video total frames
	NMI_BSDEMUX_PARAM_VDO_WID,                                ///< video width
	NMI_BSDEMUX_PARAM_VDO_HEI,                                ///< video height
	NMI_BSDEMUX_PARAM_AUD_ENABLE,                             ///< audio enable (default: 0)
	NMI_BSDEMUX_PARAM_AUD_SR,                                 ///< audio sample rate
	NMI_BSDEMUX_PARAM_AUD_TTFRM,                              ///< audio total frames
	NMI_BSDEMUX_PARAM_CONTAINER,                              ///< media container
	NMI_BSDEMUX_PARAM_MEM_RANGE,                              ///< set memory range (allocate memory)
	NMI_BSDEMUX_PARAM_BLK_TTNUM,                              ///< total file block number
	NMI_BSDEMUX_PARAM_BLK_SIZE,                               ///< file block size
	NMI_BSDEMUX_PARAM_REG_CB,                                 ///< register callback
	NMI_BSDEMUX_PARAM_GOP,                                    ///< group of pictures
	NMI_BSDEMUX_PARAM_DECDESC,                                ///< decode description
	NMI_BSDEMUX_PARAM_FILE_FMT,                               ///< file format
	NMI_BSDEMUX_PARAM_DISP_IDX,                               ///< current display video index
	NMI_BSDEMUX_PARAM_TRIG_TSDMX,                             ///< trigger TS demux
	NMI_BSDEMUX_PARAM_FILESIZE,                               ///< total file size
	/* get param */
	NMI_BSDEMUX_PARAM_ALLOC_SIZE,                             ///< allocate memory
	/* trigger function */
	NMI_BSDEMUX_PARAM_TRIG_AUDIO,                             ///< trigger next audio bs
	NMI_BSDEMUX_PARAM_TRIG_VIDEO,                             ///< trigger video
	NMI_BSDEMUX_PARAM_START_PLAY,                             ///< start play (speed and direct)
} NMI_BSDEMUX_PARAM;

typedef enum {
	NMI_BSDEMUX_ACTION_PRELOADBS,                             ///< pre-load bitstream
	NMI_BSDEMUX_ACTION_START_VDO,                             ///< set interval and start timer trigger
	NMI_BSDEMUX_ACTION_STOP,                                  ///< stop timer trigger
	NMI_BSDEMUX_ACTION_START,                                 ///< start play
	NMI_BSDEMUX_ACTION_PAUSE,                                 ///< pause play
	NMI_BSDEMUX_ACTION_RESUME,                                ///< resume play
	NMI_BSDEMUX_ACTION_MAX,
} NMI_BSDEMUX_ACTION;

typedef enum {
	NMI_BSDEMUX_EVENT_PUSH_VDOBS,
    NMI_BSDEMUX_EVENT_PUSH_AUDBS,
   	NMI_BSDEMUX_EVENT_READ_FILE,
   	NMI_BSDEMUX_EVENT_REFRESH_VDO,
	NMI_BSDEMUX_EVENT_MAX,
	ENUM_DUMMY4WORD(NMI_BSDEMUX_CB_EVENT)
} NMI_BSDEMUX_CB_EVENT;

/**
    BsDemux Bitstream Source
*/
typedef struct _NMI_BSDEMUX_BS_INFO {
    UINT32 BsAddr;      ///< Bit-Stream address
	UINT32 BsSize;      ///< Bit-Stream size
	UINT32 FrmIdx;      ///< Frame Index
	UINT32 BufID;       ///< Buffer ID
	UINT64 TimeStamp;   ///< Timestamp
	UINT8  bIsAud;      ///< Is is audio or not
	UINT8  bIsEOF;      ///< End of Frame flag
} NMI_BSDEMUX_BS_INFO, *PNMI_BSDEMUX_BS_INFO;

typedef struct {
	UINT32 Addr;        ///< Memory buffer starting address
	UINT32 Size;        ///< Memory buffer size
} NMI_BSDEMUX_MEM_RANGE, *PNMI_BSDEMUX_MEM_RANGE;

typedef struct {
	UINT32 speed;       ///< current play speed
	UINT32 direct;      ///< current play direction
} NMI_BSDEMUX_PLAY_INFO, *PNMI_BSDEMUX_PLAY_INFO;

typedef struct {
	UINT32 addr;
	UINT32 size;
	UINT32 read_state;
} NMI_TSDEMUX_READBUF_INFO, *PNMI_TSDEMUX_READBUF_INFO;

typedef void (NMI_BSDEMUX_CB)(UINT32 event, UINT32 param);

extern void NMP_BsDemux_AddUnit(void);

#endif //_NMEDIAPLAYBSDEMUX_H

