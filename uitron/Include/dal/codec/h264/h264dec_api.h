#ifndef _H264DEC_API_H_
#define _H264DEC_API_H_

#include "Type.h"
#include "h264_def.h"
#include "h26x_def.h"

#define H264DEC_OUT_MAX     2

#define H264DEC_SUCCESS     1
#define H264DEC_FAIL        0

typedef struct _H264DEC_CODECINFO {
	PROFILE_TYPE  uiProfile;    ///< H.264 codec profile
	UINT32  uiLevel;            ///< H.264 codec level
	UINT32  uiRefFrameNum;      ///< H.264 reference frame number
} H264DEC_CODECINFO, *PH264DEC_CODECINFO;

typedef struct _H264DEC_INIT {
	UINT32 uiWidth;             ///< Decoded image width (buffer width should be aligned to 16)
	UINT32 uiHeight;            ///< Decoded image height (buffer height should be aligned to 16)
	UINT32 uiYLineOffset;       ///< Decoded image Y line offset
	UINT32 uiUVLineOffset;      ///< Decoded image UV line offset
	UINT32 uiDecBufAddr;        ///< H.264 decoder internal buffer starting address
	UINT32 uiDecBufSize;        ///< H.264 decoder internal buffer size
	UINT32 uiH264DescAddr;      ///< H.264 description data starting address
	UINT32 uiH264DescSize;      ///< H.264 description data size
	UINT32 uiDisplayWidth;      ///< Display source width, ex: 720 x 480, uiWidth = 736, uiDisplayWidth = 720

	H264DEC_CODECINFO  CodecInfo;   ///< H.264 codec information
} H264DEC_INIT, *PH264DEC_INIT;

typedef struct _H264DEC_INFO {
	UINT32 uiSrcYAddr[H264DEC_OUT_MAX];     ///< Output (decoded) source image Y address
	UINT32 uiSrcUVAddr[H264DEC_OUT_MAX];    ///< Output (decoded) source image UV address
	UINT32 uiBSAddr[H264DEC_OUT_MAX];       ///< Input bit-stream address
	UINT32 uiBSSize[H264DEC_OUT_MAX];       ///< Input bit-stream size

	// shall be to remove //
	UINT32 uiDecFrameType;                  ///< Decoded frame type: I, P, or B
	UINT32 uiDecFrameNum;                   ///< Decode frame number (single, 2-B, ...)
} H264DEC_INFO, *PH264DEC_INFO;


typedef struct _H264DEC_VAR {
	UINT32         uiDecId;
	void           *pContext;
} H264DEC_VAR,  *PH264DEC_VAR;



extern INT32 h264Dec_initDecoder(PH264DEC_INIT pH264DecInit, H264DEC_VAR *pH264DecVar);
extern INT32 h264Dec_decodeOnePicture(PH264DEC_INFO pH264DecInfo, H264DEC_VAR *pH264DecVar);
extern void h264Dec_resetGop(void);
extern INT32 h264Dec_IsDecodeFinish(void);
extern void h264Dec_stopDecoder(H264DEC_VAR *pH264DecVar);
extern void h264Dec_reset(void);

#endif // _H264DEC_API_H_
