#ifndef _NMEDIAPLAYVDODEC_H
#define _NMEDIAPLAYVDODEC_H

#include "NvtMediaInterface.h"


typedef enum {
	NMI_VDODEC_PARAM_ALLOC_SIZE,
	NMI_VDODEC_PARAM_MEM_RANGE,
	NMI_VDODEC_PARAM_MAX_MEM_INFO,      ///< Set max buf info and get calculated buf size
	NMI_VDODEC_PARAM_MAX_ALLOC_SIZE,
	NMI_VDODEC_PARAM_CODEC,
	NMI_VDODEC_PARAM_DECDESC,
	NMI_VDODEC_PARAM_WIDTH,
	NMI_VDODEC_PARAM_HEIGHT,
	NMI_VDODEC_PARAM_GOP,
	NMI_VDODEC_PARAM_RAW_STARADDR,
	NMI_VDODEC_PARAM_RAW_BUFSIZE,
	NMI_VDODEC_PARAM_REG_CB,
	NMI_VDODEC_PARAM_REFRESH_VDO,       ///< refresh video queue
} NMI_VDODEC_PARAM;

typedef enum {
	NMI_VDODEC_ACTION_START,
	NMI_VDODEC_ACTION_STOP,
	NMI_VDODEC_ACTION_MAX,
} NMI_VDODEC_ACTION;

typedef enum {
	NMI_VDODEC_EVENT_RAW_READY,
	NMI_VDODEC_EVENT_CUR_VDOBS,   ///< callback current used video bitstream address & size
	NMI_VDODEC_EVENT_MAX,
	ENUM_DUMMY4WORD(NMI_VDODEC_EVENT)
} NMI_VDODEC_EVENT;

typedef struct {
	UINT32 Addr;    ///< Memory buffer starting address
	UINT32 Size;    ///< Memory buffer size
} NMI_VDODEC_MEM_RANGE, *PNMI_VDODEC_MEM_RANGE;

/**
    Struct definition of Media Video Decoder Bit-Stream Source
*/
typedef struct {
	UINT32						uiBSAddr;						///< Bit-Stream address
	UINT32						uiBSSize;						///< Bit-Stream size
	UINT32						uiThisFrmIdx;					///< This Frame Index
	UINT32						bIsEOF;						    ///< End of Frame flag
	UINT32						BufID;							///< Buffer ID
	UINT64						TimeStamp;						///< Timestamp
} NMI_VDODEC_BS_SRC, *PNMI_VDODEC_BS_SRC;


/**
    Struct definition of Media Video Decoder Output RAW Data
*/
typedef struct {
	UINT32						uiRawAddr;						///< Raw address
	UINT32						uiRawSize;						///< Raw address
	UINT32                      uiYAddr;                        ///< Y address
	UINT32                      uiUVAddr;                       ///< UV address
	UINT32						uiThisFrmIdx;					///< This Frame Index
	UINT32						bIsEOF;						    ///< End of Frame flag
	UINT64						TimeStamp;						///< Timestamp
} NMI_VDODEC_RAW_INFO, *PNMI_VDODEC_RAW_INFO;

typedef struct {
	UINT32                      uiVdoCodec;                     ///< in: codec type
	UINT32                      uiWidth;                  		///< in: width
	UINT32                      uiHeight;                      	///< in: height
	BOOL                        bRelease;
    UINT32                      uiDecsize;		                ///< out: dec buf size
} NMI_VDODEC_MAX_MEM_INFO, *PNMI_VDODEC_MAX_MEM_INFO;

typedef void (NMI_VDODEC_CB)(UINT32 event, UINT32 param);

extern void NMP_VdoDec_AddUnit(void);

#endif //_NMEDIAPLAYVDODEC_H