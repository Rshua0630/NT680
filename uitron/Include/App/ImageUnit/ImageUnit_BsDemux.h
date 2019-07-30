/*
    Copyright   Novatek Microelectronics Corp. 2005~2018.  All rights reserved.

    @file       ImageUnit_BsDemux.h
    @ingroup    mBsDemux

    @note       Nothing.

    @date       2018/03/06
*/

#ifndef IMAGEUNIT_BSDEMUX_H
#define IMAGEUNIT_BSDEMUX_H

#include "ImageStream.h"

/*
	type definition of parameter.

	@note IMM: effective immediately
*/
typedef enum {
	BSDEMUX_PARAM_START = 0x00000000,
	/* control */
	BSDEMUX_PARAM_RESET_IMM = BSDEMUX_PARAM_START,        ///< reset i/o port matching
	BSDEMUX_PARAM_EVENT_CB,                               ///< register ImageApp callback function
	/* output port */
	BSDEMUX_PARAM_BYPASS_IMM,                             ///< bypass matching type
	BSDEMUX_PARAM_CONNECT_IMM,                            ///< connect i/o port matching
	BSDEMUX_PARAM_DISCONNECT_IMM,                         ///< disconnect i/o port matching
	BSDEMUX_PARAM_UPDATE_PLAY,                            ///< start play
	BSDEMUX_PARAM_PAUSE,                                  ///< pause play
	BSDEMUX_PARAM_RESUME,                                 ///< resume play
	BSDEMUX_PARAM_FILEPLAY_MODE,                          ///< set TRUE when file play mode
	/* input port */
	BSDEMUX_PARAM_VDO_ENABLE,                             ///< video enable (default: 0)
	BSDEMUX_PARAM_VDO_CODEC,                              ///< video codec
	BSDEMUX_PARAM_VDO_FR,                                 ///< video frame rate
	BSDEMUX_PARAM_VDO_TTFRM,                              ///< video total frame
	BSDEMUX_PARAM_VDO_WID,                                ///< video width
	BSDEMUX_PARAM_VDO_HEI,                                ///< video height
	BSDEMUX_PARAM_AUD_ENABLE,                             ///< audio enable (default: 0)
	BSDEMUX_PARAM_AUD_SR,                                 ///< audio sample rate
	BSDEMUX_PARAM_AUD_TTFRM,                              ///< total audio frame
	BSDEMUX_PARAM_IPORT_TYPE,                             ///< setting iport connecting type (e.g. ISF_CONNECT_PUSH or ISF_CONNECT_PULL)
	BSDEMUX_PARAM_CONTAINER,                              ///< media container
	BSDEMUX_PARAM_BLK_TTNUM,                              ///< total file block number
	BSDEMUX_PARAM_BLK_SIZE,                               ///< file block size
	BSDEMUX_PARAM_GOP,                                    ///< group of picture
	BSDEMUX_PARAM_DECDESC,                                ///< decode description
	BSDEMUX_PARAM_FILE_FMT,                               ///< file format (mp4/mov or ts)
	BSDEMUX_PARAM_DISP_IDX,                               ///< current display video index
	BSDEMUX_PARAM_TRIG_TSDMX,                             ///< trigger TS demux
	BSDEMUX_PARAM_FILESIZE,                               ///< total file size
	BSDEMUX_PARAM_MAX,
	ENUM_DUMMY4WORD(BSDEMUX_PARAM)
} BSDEMUX_PARAM;

typedef enum {
	ISF_BSDEMUX_EVENT_START = 0,
	ISF_BSDEMUX_EVENT_REFRESH_VDO,                        /// refresh video queue
	ISF_BSDEMUX_EVENT_MAX,
	ENUM_DUMMY4WORD(ISF_BSDEMUX_CB_EVENT)
} ISF_BSDEMUX_CB_EVENT;

typedef void (IsfBsDemuxEventCb)(CHAR *Name, UINT32 event_id, UINT32 value);

extern ISF_UNIT ISF_BsDemux;

#endif //IMAGEUNIT_BSDEMUX_H

