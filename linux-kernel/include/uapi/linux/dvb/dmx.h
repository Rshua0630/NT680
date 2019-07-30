/*
 * dmx.h
 *
 * Copyright (C) 2000 Marcus Metzler <marcus@convergence.de>
 *                  & Ralph  Metzler <ralph@convergence.de>
 *                    for convergence integrated media GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef _UAPI_DVBDMX_H_
#define _UAPI_DVBDMX_H_

#include <linux/types.h>
#ifndef __KERNEL__
#include <time.h>
#endif


#define DMX_FILTER_SIZE 16
#ifdef CONFIG_NVT_TK_DRIVER
#define DMX_BIG_SECTION_BUFFER_SIZE 	(128*1024)
#define DMX_SMALL_SECTION_BUFFER_SIZE	(8*1024)
#endif

typedef enum
{
	DMX_OUT_DECODER, /* Streaming directly to decoder. */
	DMX_OUT_TAP,     /* Output going to a memory buffer */
			 /* (to be retrieved via the read command).*/
	DMX_OUT_TS_TAP,  /* Output multiplexed into a new TS  */
			 /* (to be retrieved by reading from the */
			 /* logical DVR device).                 */
	DMX_OUT_TSDEMUX_TAP /* Like TS_TAP but retrieved from the DMX device */
} dmx_output_t;


typedef enum
{
	DMX_IN_FRONTEND, /* Input from a front-end device.  */
	DMX_IN_DVR       /* Input from the logical DVR device.  */
} dmx_input_t;

#ifdef CONFIG_NVT_TK_DRIVER
typedef enum {
	DMX_REC_MODE_DIGITAL,
	DMX_REC_MODE_ANALOG,
}record_mode_t;
#endif

typedef enum dmx_ts_pes
{
	DMX_PES_AUDIO0,
	DMX_PES_VIDEO0,
	DMX_PES_TELETEXT0,
	DMX_PES_SUBTITLE0,
	DMX_PES_PCR0,

	DMX_PES_AUDIO1,
	DMX_PES_VIDEO1,
	DMX_PES_TELETEXT1,
	DMX_PES_SUBTITLE1,
	DMX_PES_PCR1,

	DMX_PES_AUDIO2,
	DMX_PES_VIDEO2,
	DMX_PES_TELETEXT2,
	DMX_PES_SUBTITLE2,
	DMX_PES_PCR2,

	DMX_PES_AUDIO3,
	DMX_PES_VIDEO3,
	DMX_PES_TELETEXT3,
	DMX_PES_SUBTITLE3,
	DMX_PES_PCR3,

	DMX_PES_OTHER
} dmx_pes_type_t;

#ifdef CONFIG_NVT_TK_DRIVER
typedef enum dmx_ca_type
{
	DMX_CA_BYPASS = 0,
	DMX_CA_DES_ECB,
	DMX_CA_3DES_CBC,
	DMX_CA_3DES_ECB,
	DMX_CA_AES_ECB,
	DMX_CA_AES_CBC,
	DMX_CA_MULTI2,
	DMX_CA_DVB_CSA,
	DMX_CA_AES_CTR,
} dmx_ca_type_t;
#endif /* CONFIG_NVT_TK_DRIVER */

#define DMX_PES_AUDIO    DMX_PES_AUDIO0
#define DMX_PES_VIDEO    DMX_PES_VIDEO0
#define DMX_PES_TELETEXT DMX_PES_TELETEXT0
#define DMX_PES_SUBTITLE DMX_PES_SUBTITLE0
#define DMX_PES_PCR      DMX_PES_PCR0


typedef struct dmx_filter
{
	__u8  filter[DMX_FILTER_SIZE];
	__u8  mask[DMX_FILTER_SIZE];
	__u8  mode[DMX_FILTER_SIZE];
} dmx_filter_t;


struct dmx_sct_filter_params
{
	__u16          pid;
	dmx_filter_t   filter;
	__u32          timeout;
	__u32          flags;
#define DMX_CHECK_CRC       1
#define DMX_ONESHOT         2
#define DMX_IMMEDIATE_START 4
#define DMX_KERNEL_CLIENT   0x8000
};


struct dmx_pes_filter_params
{
	__u16          pid;
	dmx_input_t    input;
	dmx_output_t   output;
	dmx_pes_type_t pes_type;
	__u32          flags;
};

typedef struct dmx_caps {
	__u32 caps;
	int num_decoders;
} dmx_caps_t;

typedef enum {
	DMX_SOURCE_FRONT0 = 0,
	DMX_SOURCE_FRONT1,
	DMX_SOURCE_FRONT2,
	DMX_SOURCE_FRONT3,
#ifdef CONFIG_NVT_TK_DRIVER
	DMX_SOURCE_FRONT4,	/*First Serial*/
	DMX_SOURCE_FRONT5,	/*Second Serial*/
#endif /* CONFIG_NVT_TK_DRIVER */

	DMX_SOURCE_DVR0   = 16,
	DMX_SOURCE_DVR1,
	DMX_SOURCE_DVR2,
	DMX_SOURCE_DVR3
} dmx_source_t;

#ifdef CONFIG_NVT_TK_DRIVER
typedef enum {
	DMX_OUTPUT_NONE,
	DMX_OUTPUT_EXT1,
	DMX_OUTPUT_EXT2,
	DMX_OUTPUT_EXT3,
	DMX_OUTPUT_EXT4,
	DMX_OUTPUT_EXT5,
	DMX_OUTPUT_PVR1,
	DMX_OUTPUT_PVR2,
} dmx_redirect_t;
#endif /* CONFIG_NVT_TK_DRIVER */

struct dmx_stc {
	unsigned int num;	/* input : which STC? 0..N */
	unsigned int base;	/* output: divisor for stc to get 90 kHz clock */
	__u64 stc;		/* output: stc in 'base'*90 kHz units */
};

#ifdef CONFIG_NVT_TK_DRIVER
enum dmx_status_cmd {
	DEMUX_STATUS_TSDLOCK
};

struct dmx_status {
	__u32 cmd;
	__u32 data;
};

#define DRY_CRYPT_KEY_LEN 16
typedef struct dvr_key_info
{
    __u8  key_table[DRY_CRYPT_KEY_LEN];    // < [in] key value (in little endian and in the order of first key,second key, etc.)
    __u32 key_len;    // < [in] Length of the key (in number of bytes)
    __u8* iv;     // < [in] IV (Initial Vector) value used only in CBC mode
    __u32 iv_len;     // < [in] IV length used only in CBC mode
    __u8* modulo;///< [in] Pointer of 256bytes Modulo values. used only in RSA mode
    __u32 modulo_len; ///< [in] Length of 256bytes Modulo values. used only in RSA mode
}dvr_key_info_t;
    
typedef struct dvr_key
{
   __s32 odd_key; /* 1: odd, 0: even */
   dvr_key_info_t key;
}dvr_scr_t;


typedef enum {
    DVR_PIC_TYPE_I, ///< I Picture
    DVR_PIC_TYPE_B, ///< B Picture
    DVR_PIC_TYPE_P, ///< P Picture
    DVR_SEQ_HDR, ///< Sequence Header
    DVR_PIC_TYPE_NULL,
} dvr_pic_type_t;


typedef struct
{
    dvr_pic_type_t pic_type;  
    __u64 pkg_count;          
    __u32 time;            
    __u64 pts;   //added for PVR_PTS           
} dvr_index_info_t;

typedef enum {
    DVR_VIDEO_FAMAT_MPEG,
    DVR_VIDEO_FAMAT_H264,
    DVR_VIDEO_FAMAT_AVS,
    DVR_VIDEO_FAMAT_HEVC,
    DVR_VIDEO_FAMAT_NONE, /* audio only channel */
} dvr_video_format_t;

typedef struct {
    dvr_video_format_t video_format;
 __u32 interval; /* time in ms between index events for audio only channel. */
} dvr_index_config_t;

/* Parameter for DVR_SET_DATA_TYPE */
#define DVR_DATA_TYPE_NORMAL        0
#define DVR_DATA_TYPE_SCRAMBLED     1


typedef struct dvr_hdr {
	__u64 pkg_count;
	__u64 pts;
	__u32 picture_type;
#define DVR_PIC_TYPE_I		0
#define DVR_PIC_TYPE_P		1
#define DVR_PIC_TYPE_B		2
#define DVR_SEQ_HDR		3
#define DVR_PIC_TYPE_NULL	4
	__u32 time;
} dvr_hdr_t;

typedef struct dvr_play_buff {
	int level;
	int total;
} dvr_play_buff_t;

typedef struct ca_config {
	int mode;
	dmx_ca_type_t ca_type;
} ca_config_t;

typedef enum dvr_play_state
{
	DVR_PLAY_STATE_STOP,
	DVR_PLAY_STATE_PLAY,
	DVR_PLAY_STATE_PAUSE,
	DVR_PLAY_STATE_RESUME,
	DVR_PLAY_STATE_MAX
} dvr_play_state_t;
#endif /* CONFIG_NVT_TK_DRIVER */

#define DMX_START                _IO('o', 41)
#define DMX_STOP                 _IO('o', 42)
#define DMX_SET_FILTER           _IOW('o', 43, struct dmx_sct_filter_params)
#define DMX_SET_PES_FILTER       _IOW('o', 44, struct dmx_pes_filter_params)
#define DMX_SET_BUFFER_SIZE      _IO('o', 45)
#define DMX_GET_PES_PIDS         _IOR('o', 47, __u16[5])
#define DMX_GET_CAPS             _IOR('o', 48, dmx_caps_t)
#define DMX_SET_SOURCE           _IOW('o', 49, dmx_source_t)
#define DMX_GET_STC              _IOWR('o', 50, struct dmx_stc)
#define DMX_ADD_PID              _IOW('o', 51, __u16)
#define DMX_REMOVE_PID           _IOW('o', 52, __u16)

#ifdef CONFIG_NVT_TK_DRIVER
//#define DVR_SET_DATA_TYPE        _IOW('o', 53, __u32) /* set weather the dvr record data encrypted */
#define DVR_SET_DESCR            _IOW('o', 53, dvr_scr_t) /* set dvr decrypt key for playback scrambled data */
#define DVR_SET_SCR              _IOW('o', 54, dvr_scr_t) /* set dvr encrypt key */
#define DVR_IDX_SET_BUFFER_SIZE  _IO('o', 55) /* set dvr index data buffer size */
#define DMX_GET_STATUS           _IOR('o', 57, struct dmx_status)
#define DMX_SET_RECORD_MODE      _IO('o', 58)

/* IOCTL numbers From 80 are private */
#define DMX_IOCTL_BASE		96
#define SDP_SET_TSD_SOURCE		_IOW('o', 97, dmx_source_t)
#define SDP_SET_TSD_REDIRECT    _IOW('o', 98, dmx_redirect_t)
#define DVR_IDX_SET_CONFIG      _IOWR('o', 99, dvr_index_config_t) /* set dvr recording config. */
#define DVR_GET_PLAY_BUFFER     _IOWR('o', 100, dvr_play_buff_t) /* get pvr play buffer level */
#define SDP_SET_CA_CONFIG       _IOW('o', 101, __u32)		/* set descrambler on/off */
#define SDP_SET_CA_CTRL         _IOW('o', 102, ca_config_t)		/* set descrambler on/off, ca_type */
#define DVR_SET_PLAY_STATE      _IOW('o', 103, dvr_play_state_t)  /* set PVR play state */
#define DVR_FLUSH               _IOWR('o', 104, dvr_play_buff_t) /* flush PVR play es data */ 

#define DMX_IOCTL_END		127
#endif /* CONFIG_NVT_TK_DRIVER */
#endif /* _UAPI_DVBDMX_H_ */
