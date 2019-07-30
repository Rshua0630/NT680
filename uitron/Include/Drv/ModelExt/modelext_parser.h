#ifndef _MODELEXT_PARSER_H
#define _MODELEXT_PARSER_H

#include "modelext_info.h"

/**
    modelext inline parse.

    modelext info tables are set to an certain memory address,
    which use to get data pointer you want.

    @param[in] modelext info address.
    @param[in] refer to MODELEXT_TYPE_
    @param[in] a memory space will be stored with description of modelext type.
    @return
        - @b address: the data pointer of result.
*/
static inline unsigned char *modelext_get_cfg(unsigned char *buf, unsigned int type, MODELEXT_HEADER **header)
{
#define MAGIC "MODELEXT"
	MODELEXT_HEADER *h_tail = NULL;
	MODELEXT_HEADER *h = (MODELEXT_HEADER *)buf;
	MODELEXT_INFO *d = (MODELEXT_INFO *)(buf + sizeof(MODELEXT_HEADER));

	if (type >= MODELEXT_TYPE_MAX) {
		if (header) {
			*header = NULL;
		}
		return NULL;
	}

	if (memcmp(d->name, MAGIC, strlen(MAGIC))) {
		if (header) {
			*header = NULL;
		}
		return NULL;
	}

	h_tail = (MODELEXT_HEADER *)(buf + d->ext_bin_length);

	while (h->size) {
		if (h->type == type) {
			break;
		} else {
			h = (MODELEXT_HEADER *)(((char *)h) + h->size);
			if (h >= h_tail) {
				return NULL;
			}
		}
	}

	if (!h->size) {
		if (header) {
			*header = NULL;
		}
		return NULL;
	}

	if (header) {
		*header = h;
	}

	return (unsigned char *)(((unsigned char *)h) + sizeof(MODELEXT_HEADER));
}

#endif
