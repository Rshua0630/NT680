/*
 * dmxdev.h
 *
 * Copyright (C) 2000 Ralph Metzler & Marcus Metzler
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

#ifndef _DMXDEV_H_
#define _DMXDEV_H_

#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/slab.h>

#include <linux/dvb/dmx.h>

#include "dvbdev.h"
#include "demux.h"
#ifdef CONFIG_NVT_TK_DRIVER
#include "dvb_demux.h"
#endif
#include "dvb_ringbuffer.h"

enum dmxdev_type {
	DMXDEV_TYPE_NONE,
	DMXDEV_TYPE_SEC,
	DMXDEV_TYPE_PES,
};

enum dmxdev_state {
	DMXDEV_STATE_FREE,
	DMXDEV_STATE_ALLOCATED,
	DMXDEV_STATE_SET,
	DMXDEV_STATE_GO,
	DMXDEV_STATE_DONE,
	DMXDEV_STATE_TIMEDOUT
};

struct dmxdev_feed {
	u16 pid;
	struct dmx_ts_feed *ts;
	struct list_head next;
};

struct dmxdev_filter {
	union {
		struct dmx_section_filter *sec;
	} filter;

	union {
		/* list of TS and PES feeds (struct dmxdev_feed) */
		struct list_head ts;
		struct dmx_section_feed *sec;
	} feed;

	union {
		struct dmx_sct_filter_params sec;
		struct dmx_pes_filter_params pes;
	} params;

	enum dmxdev_type type;
	enum dmxdev_state state;
	struct dmxdev *dev;
	struct dvb_ringbuffer buffer;

	struct mutex mutex;

	/* only for sections */
	struct timer_list timer;
	int todo;
	u8 secheader[3];
};

#ifdef CONFIG_NVT_TK_DRIVER
struct dmx_scr_dvr {
        u32 capabilities;            /* Bitfield of capability flags */
#define DMXDEV_HARDWARE_INDEX_DVR 1     
        struct dmxdev* dmxdev;    /* Front-end connected to the dmxdev */
        void* priv;                  /* Pointer to private data of the API client */
#ifdef CONFIG_DVB_LINKED_RBUF
	struct dvb_llbuffer index_dvr_buffer;
#else
        struct dvb_ringbuffer index_dvr_buffer;
#endif
#define INDEX_DVR_BUFFER_SIZE (sizeof(dvr_index_info_t)*128)

        // Open hardware DVR recorder.
        int (*open) (struct dmx_scr_dvr* demux);
        int (*set_data_type) (struct dmx_scr_dvr* scr_dvr, u32 data_type);
        int (*set_scr) (struct dmx_scr_dvr* scr_dvr, dvr_scr_t *scr);
        int (*set_descr) (struct dmx_scr_dvr* scr_dvr, dvr_scr_t *scr);
        int (*close) (struct dmx_scr_dvr* scr_dvr);
};
/*~~ jur add */	
#endif

struct dmxdev {
	struct dvb_device *dvbdev;
	struct dvb_device *dvr_dvbdev;
#ifdef CONFIG_NVT_TK_DRIVER
	struct dvb_device *dvr_hdr_dvbdev;
#endif

	struct dmxdev_filter *filter;
	struct dmx_demux *demux;
#ifdef CONFIG_NVT_TK_DRIVER
	struct dmx_scr_dvr *scr_dvr;
#endif

	int filternum;
	int capabilities;

	unsigned int exit:1;
#define DMXDEV_CAP_DUPLEX 1

#ifdef CONFIG_NVT_TK_DRIVER
// Hardware PVR support index, scrambled data capability
#define DMXDEV_CAP_SCRAMBLE_DVR 2
#endif
	
	struct dmx_frontend *dvr_orig_fe;

	struct dvb_ringbuffer dvr_buffer;
#define DVR_BUFFER_SIZE (10*188*1024)
#ifdef CONFIG_NVT_TK_DRIVER
#ifdef CONFIG_DVB_LINKED_RBUF
	struct dvb_llbuffer dvr_hdr_buffer;
#else
	struct dvb_ringbuffer dvr_hdr_buffer;
#endif
#define DVR_HDR_BUFFER_SIZE (sizeof(dvr_hdr_t))
#endif

	struct mutex mutex;
	spinlock_t lock;
};


int dvb_dmxdev_init(struct dmxdev *dmxdev, struct dvb_adapter *);
void dvb_dmxdev_release(struct dmxdev *dmxdev);
#ifdef CONFIG_NVT_TK_DRIVER
int dvb_dmxdev_index_callback(const u8 *buffer1, size_t buffer1_len,
				  const u8 *buffer2, size_t buffer2_len,
				  struct dmx_ts_feed *feed,
				  enum dmx_success success);
#endif
#endif /* _DMXDEV_H_ */
