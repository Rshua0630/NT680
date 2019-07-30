#ifndef _H265DEC_API_H_
#define _H265DEC_API_H_

#include "Type.h"
#include "h265_def.h"
#include "h26x_def.h"

#define H265DEC_OUT_MAX     2

#define H265DEC_SUCCESS     1
#define H265DEC_FAIL        0

typedef struct _H265DEC_CODECINFO {
	PROFILE_TYPE  uiProfile;    ///< H.265 codec profile
	UINT32  uiLevel;            ///< H.265 codec level
	UINT32  uiRefFrameNum;      ///< H.265 reference frame number
} H265DEC_CODECINFO, *PH265DEC_CODECINFO;

typedef struct _H265DEC_INIT {
	UINT32 uiWidth;             ///< Decoded image width (buffer width should be aligned to 16)
	UINT32 uiHeight;            ///< Decoded image height (buffer height should be aligned to 16)
	UINT32 uiYLineOffset;       ///< Decoded image Y line offset
	UINT32 uiUVLineOffset;      ///< Decoded image UV line offset
	UINT32 uiDecBufAddr;        ///< H.265 decoder internal buffer starting address
	UINT32 uiDecBufSize;        ///< H.265 decoder internal buffer size
	UINT32 uiH265DescAddr;      ///< H.265 description data starting address
	UINT32 uiH265DescSize;      ///< H.265 description data size
	UINT32 uiDisplayWidth;      ///< Display source width, ex: 720 x 480, uiWidth = 736, uiDisplayWidth = 720

	H265DEC_CODECINFO  CodecInfo;   ///< H.265 codec information
} H265DEC_INIT, *PH265DEC_INIT;

typedef struct _H265DEC_INFO {
	UINT32 uiSrcYAddr[H265DEC_OUT_MAX];     ///< Output (decoded) source image Y address
	UINT32 uiSrcUVAddr[H265DEC_OUT_MAX];    ///< Output (decoded) source image UV address
	UINT32 uiBSAddr[H265DEC_OUT_MAX];       ///< Input bit-stream address
	UINT32 uiBSSize[H265DEC_OUT_MAX];       ///< Input bit-stream size
	UINT32 uiDecFrameType;                  ///< Decoded frame type: I, P, or B
	UINT32 uiDecFrameNum;                   ///< Decode frame number (single, 2-B, ...)
} H265DEC_INFO, *PH265DEC_INFO;


typedef struct _H265DEC_VAR {
	UINT32         uiDecId;
	void           *pContext;
} H265DEC_VAR,  *PH265DEC_VAR;



extern INT32 h265Dec_initDecoder(PH265DEC_INIT pH265DecInit, H265DEC_VAR *pH265DecVar);
extern INT32 h265Dec_decodeOnePicture(PH265DEC_INFO pH265DecInfo, H265DEC_VAR *pH265DecVar);
extern void h265Dec_resetGop(void);
extern INT32 h265Dec_IsDecodeFinish(void);
extern void h265Dec_stopDecoder(H265DEC_VAR *pH265DecVar);
extern void h265Dec_reset(void);

#endif // _H265DEC_API_H_
