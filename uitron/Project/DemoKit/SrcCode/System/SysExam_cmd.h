#ifndef SYSEXAM_CMD_H
#define SYSEXAM_CMD_H

typedef enum _SYSEXAM_TMTAG {
	SYSEXAM_TMTAG_UNKNOWN,
	SYSEXAM_TMTAG_PREVIEW_RDY,
	SYSEXAM_TMTAG_RECORD_RDY,
}SYSEXAM_TMTAG;

//only for autotest to save time of off-2-preivew, off-2-record
void SysExam_SetTimeTag(SYSEXAM_TMTAG tag);

#endif//SYSEXAM_CMD_H
