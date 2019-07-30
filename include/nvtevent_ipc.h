#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "nvtevent.h"

#define NVTEVENT_FOURCC   0x45564E54 ///< fourcc 'EVNT', to defect shared memory sanity
#define NVTEVENT_SHM_SIZE 0x10000 ///< share memory size for shmget

typedef enum _NVTEVENT_MSG_TYPE {
	NVTEVENT_MSG_TYPE_UNKNOWN,
	NVTEVENT_MSG_TYPE_SUBSCRIBE,
	NVTEVENT_MSG_TYPE_UNSUBSCRIBE,
	NVTEVENT_MSG_TYPE_DELIVERY,
	NVTEVENT_MSG_TYPE_TERMINATE,
	NVTEVENT_MSG_TYPE_TIMEOUT, ///< for client-wait-delivery only to implement wait timeout mechanism
	NVTEVENT_MSG_TYPE_MAX_COUNT,
	NVTEVENT_MSG_TYPE_DUMMY = (long)(-1),
} NVTEVENT_MSG_TYPE;

typedef struct _NVTEVENT_HEADER {
	NVTEVENT_MSG_TYPE type;
} NVTEVENT_HEADER;

typedef struct _NVTEVENT_SUBSCRIBE {
	NVTEVENT_MSG_TYPE type; ///< must be NVTEVENT_MSG_TYPE_SUBSCRIBE
	NVTEVENT_CHANNEL  channel;
	int               pid;
	int               msg_key;
} NVTEVENT_SUBSCRIBE;

typedef struct _NVTEVENT_UNSUBSCRIBE {
	NVTEVENT_MSG_TYPE type; ///< must be NVTEVENT_MSG_TYPE_UNSUBSCRIBE
	int               pid;
	int               msg_key;
} NVTEVENT_UNSUBSCRIBE;

typedef struct _NVTEVENT_DELIVERY {
	NVTEVENT_MSG_TYPE type;     ///< must be NVTEVENT_MSG_TYPE_DELIVERY
	NVTEVENT_CHANNEL channel;   ///< indicate the channel of this package
	unsigned int     seq_idx;   ///< sequence number, if seq_idx differs form p_data, indicate data is invalid.
	unsigned int     ofs_data;  ///< data offset in shared memory
	unsigned int     size_data; ///< data size in shared memory
} NVTEVENT_DELIVERY;

typedef struct _NVTEVENT_TIMEOUT {
	NVTEVENT_MSG_TYPE type; ///< must be NVTEVENT_MSG_TYPE_TIMEOUT
	int               timeout_id;   ///< timeout id to detect expired timeout event
} NVTEVENT_TIMEOUT;

typedef struct _NVTEVENT_SHM_HEADER {
	unsigned int fourcc; ///< must be NVTEVENT_FOURCC
	unsigned int seq_idx; ///< sequence number, if seq_idx differs form DELIVERY, indicate data is invalid.
}NVTEVENT_SHM_HEADER;

typedef struct _NVTEVENT_MSG_BUF {
	long    mtype;
	union {
		char buf[64];
		NVTEVENT_HEADER header;
		NVTEVENT_SUBSCRIBE subscribe;
		NVTEVENT_UNSUBSCRIBE unsubscribe;
		NVTEVENT_DELIVERY delivery;
		NVTEVENT_TIMEOUT timeout;
	};
} NVTEVENT_MSG_BUF;


#ifdef __cplusplus
}
#endif
