#ifndef _H264_API_H_
#define _H264_API_H_

#include "Type.h"
#include "h26x_def.h"
#include "h264_def.h"

#define H264ENC_IN_MAX          1   /*!<  maximum input source number for H.264 encoding simultaneously. */
#define H264ENC_NUM_REF         1

typedef enum {
	H264ENC_FAIL = 0,
	H264ENC_SUCCESS,
} H264EncStatus;

typedef struct _H264EncFro_ {
	INT32 rounding_offset_method;
	INT32 fro_dc[3][4];
	INT32 fro_ac[3][4];
	INT32 fro_st[3][4];
	INT32 fro_mx[3][4];
} H264EncFro;

typedef struct _H264ENC_INIT_ {
	// resolution and lineoffset //
	UINT32          uiWidth;            ///< Original source width, ex: 1920 x 1080, width = 1920
	UINT32          uiHeight;           ///< Original source height, ex: 1920 x 1080, height = 1080 (not 1088)
	UINT32          uiSarWidth;         ///< Sample Aspect Ratio width, ex: 1920 x 1080, width = 1920
	UINT32          uiSarHeight;        ///< Sample Aspect Ratio height, ex: 1920 x 1080, height = 1080 (not 1088)
	UINT32          uiDisplayWidth;     ///< Display source width, ex: 720 x 480, uiWidth = 736, uiDisplayWidth = 720

	// Bitstream Buffer  //
	UINT32          uiEncBufAddr;       ///< H264 encoder internal buffer starting address
	UINT32          uiEncBufSize;       ///< H264 encoder internal buffer size

	// Codec Settings //
	PROFILE_TYPE    uiEncProfile;       ///< H.264 encoder profile (baseline, main, high profile)
	UINT32          uiEncGopFrameNum;   ///< H.264 encoder GOP frame number

	// SVC //
	UINT32          uiSVCLayer;         ///< H.264 SVC Layer, range : 0 , 1, 2

	// Long-term //
	UINT32          uiLTRInterval;      /// < H.264 long-term reference frame interval. range : 0 ~ 4095
	UINT32          uiLTRPreRef;        /// < H.264 long-term reference setting. 0 : all long-term reference to IDR frame, 1: reference latest long-term reference frame.

	// Deblocking //
	UINT8           ucDisableDB;        ///< H.264 disable_filter_idc (0=Filter, 1=No Filter, 2=Slice Mode)
	INT8            cDBAlpha;           ///< H.264 deblocking Alpha & C0 offset div. 2, {-6, -5, ... 0, +1, .. +6}
	INT8            cDBBeta;            ///< H.264 deblocking Beta offset div. 2, {-6, -5, ... 0, +1, .. +6}

	// rate control //
	UINT32          uiBitRate;          ///< H.264 Bit rate (bits per second)
	UINT32          uiFrameRate;        ///< H.264 frame rate (frames per second * 1000)

	UINT8           ucIQP;              ///< H.264 Rate Control's Init IQP
	UINT8           ucPQP;              ///< H.264 Rate Control's Init PQP
	UINT32          uiMaxIQp;           ///< H.264 Rate Control's Max I Qp
	UINT32          uiMinIQp;           ///< H.264 Rate Control's Min I Qp
	UINT32          uiMaxPQp;           ///< H.264 Rate Control's Max P Qp
	UINT32          uiMinPQp;           ///< H.264 Rate Control's Min P Qp
	INT32           iIPQPoffset;        ///< H.264 Rate Control's I P QP offset (not ready)
	UINT32          uiStaticTime;       /// < H.264 rate control's static time, unit: second

	// row-level rate control //
	UINT32          uiRowRcEnalbe;      ///< H.264 Rate Control's row level rate control enable
	UINT32          uiRowRcQpDelta;     ///< H.264 Rate Control's row level rate control QP-delta (QP-Range)
	UINT32 			uiRowRcQpStep;		///< H.264 Rate Control's row level rate control QP-Step 
	
	// NT96680  //
	UINT32          uiRefreshLineNum;   /// <.H264 Intra refresh row numbers
	UINT32          uiRecCompressEn;    /// <.H264 reconstruct compression (ECLS)
	UINT32          uiRotate;           /// <.H264 rotate source image : 0 : no rotate , 1 : counterclockwise rotate , 2 : clockwise rotate (NOTE : shall be set to 0 while enable source compression)

    // R frame
    UINT32          uiKeyPPeriod;
    INT32           iKeyPQPOffset;

	// fast search range select //
	UINT8           ucFastSearch;		/// < H.264 fast search range select : 0 : small range, 1 : big range
	// SEI //
	UINT8           ucSEIIdfEn;
    INT32           iIPWeight;

	UINT8           ucColorRange;		// 0 : tv range ,  1 : full range //
	UINT32          uiPrjMode;

	UINT32			uiMultiTLayer;		// H.264 Use Multi Temporal Layer : 0 : disable, 1: enable
} H264ENC_INIT, *PH264ENC_INIT;

typedef struct _H264ENC_RESULT_ {
	UINT32 uiSrcTimeStamp;
	UINT32 uiFrameType; //0~3:	define the same with SLICE_TYPE,  4: KeyP 
	UINT32 uiBaseQP;
	UINT32 uiBsAddr;
	UINT32 uiBsSize;
	UINT32 uiTemporalId;
	UINT32 uiNaluNum;
	UINT32 uiNaluBsLen[8];
} H264ENC_RESULT;

typedef struct _H264ENC_INFO_ {
	UINT32      uiSrcYAddr[H264ENC_IN_MAX];     ///< Source Y address
	UINT32      uiSrcUVAddr[H264ENC_IN_MAX];    ///< Source UV address
	UINT32      uiSrcCompressEn;                ///< H264 source compression
	H26XEncSrcDecomp *pSrcCompressInfo;         ///< src compression info
	UINT32      uiYLineOffset;                  ///< Y line offset
	UINT32      uiUVLineOffset;                 ///< UV line offset
	UINT32      uiBSAddr[H264ENC_IN_MAX];       ///< Encoded bit-stream address
	UINT32      uiBSSize[H264ENC_IN_MAX];       ///< Encoded bit-stream size
	UINT32      uiMaxBSLen;                     ///< Maximum bit-stream length
	SLICE_TYPE  uiEncFrameType;                 ///< Encode frame type (I, Idr, P, or B)
	UINT32      uiSrcOutYAddr[H264ENC_IN_MAX];  ///< Source Out (3DNR) Y address
	UINT32      uiSrcOutUVAddr[H264ENC_IN_MAX]; ///< Source Out (3DNR) UV address
	UINT32      uiSrcOutEnable;
	UINT32		uiSrcOutYLineOffset;
	UINT32 		uiSrcOutUVLineOffset;
	UINT32      uiSrcTimeStamp;
	UINT32      uiTemporalId[H264ENC_IN_MAX];

#if 1
	// HOPE TO REMOVE //
	// SVC SEI header //
	UINT32      uiSVCSize[H264ENC_IN_MAX];      ///< SVC header size
	// THIS ITME ARE NOT SUPPORT IN NT96680 //
	UINT32      uiEncFrameNum;                  ///< Encode frame number (single, 2-B, ...)
#endif
	UINT32		uiTuneQpEn;
} H264ENC_INFO, *PH264ENC_INFO;

typedef struct _H264ENC_VAR_ {
	UINT32         uiEncId;
	void           *pContext;
} H264ENC_VAR;

typedef struct _H264ENC_RC_INFO_ {
	UINT32 uiSliceType;
	UINT32 uiFrameRate;
	UINT32 uiGOPSize;
	UINT32 uiStaticFrame;
	UINT32 uiQp;
	UINT32 uiFrameByte;
	UINT32 uiMotionRatio;
	UINT32 uiEVBRState;
	UINT32 RESERVED0;
	UINT32 RESERVED1;
	UINT32 RESERVED2;
	UINT32 RESERVED3;
	UINT32 RESERVED4;
	UINT32 RESERVED5;
} H264ENC_RC_INFO, *PH264ENC_RC_INFO;

extern UINT32 h264Enc_queryMemSize(const H26XEncMemAttr *pMemAttr);
extern INT32 h264Enc_initEncoder(H264ENC_INIT *pH264EncInit, H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_getH264Header(UINT32 *puiAddr, UINT32 *puiLen, H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_encodeOnePicture(H264ENC_INFO *pH264EncInfo, H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_IsEncodeFinished(H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_getResult(H264ENC_INFO *pH264EncInfo, H264ENC_RESULT *pEncResult, H264ENC_VAR *pH264EncVar);
extern void  h264Enc_stopEncoder(H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_waitEncodeFinished(H264ENC_VAR *pH264EncVar);
extern UINT32 h264Enc_AskEncodeFinished(H264ENC_VAR *pH264EncVar);

// User control APIs //
extern INT32 h264Enc_setAqCfg(H264ENC_VAR *pH264EncVar, const H26XEncAq *pAqAttr);
extern INT32 h264Enc_getAqCfg(H264ENC_VAR *pH264EncVar, H26XEncAq *pAqAttr);
extern INT32 h264Enc_setRoiCfg(H264ENC_VAR *pH264EncVar, UINT32 uiIdx, const H26XEncRoi *pRoiAttr);
extern INT32 h264Enc_getRoiCfg(H264ENC_VAR *pH264EncVar, UINT32 uiIdx, H26XEncRoi *pRoiAttr);
extern INT32 h264Enc_setSmartRoiCfg(H264ENC_VAR *pH264EncVar, const H26XEncSmartRoi *pSmartRoiAttr);
extern INT32 h264Enc_getSmartRoiCfg(H264ENC_VAR *pH264EncVar, H26XEncSmartRoi *pSmartRoiAttr);
extern INT32 h264Enc_setSliceSplit(H264ENC_VAR *pH264EncVar, const H26XEncSliceSplit *pSliceSplit);
extern INT32 h264Enc_getSliceSplit(H264ENC_VAR *pH264EncVar, H26XEncSliceSplit *pSliceSplit);
extern INT32 h264Enc_setColor2Grey(H264ENC_VAR *pH264EncVar, UINT32 uiEnable);
extern INT32 h264Enc_getColor2Grey(H264ENC_VAR *pH264EncVar, UINT32 *uiEnable);
extern INT32 h264Enc_setForceISlice(H264ENC_VAR *pH264EncVar, UINT32 uiStartSlice, UINT32 uiSliceNum);
extern INT32 h264Enc_getForceISlice(H264ENC_VAR *pH264EncVar, UINT32 *pStartSlice, UINT32 *pSliceNum);

extern INT32 h264Enc_setGdr(H264ENC_VAR *pH264EncVar, UINT32 uiEnable, UINT32 uiRefreshLineNum);
extern INT32 h264Enc_getGdr(H264ENC_VAR *pH264EncVar, UINT32 *uiEnable, UINT32 *uiRefreshLineNum);

// Rate control APIs //
extern INT32 h264Enc_setRcCBR(H264ENC_VAR *pH264EncVar, const H26XEncRcCBR *pRcCBRAttr);
extern INT32 h264Enc_getRcCBR(H264ENC_VAR *pH264EncVar, H26XEncRcCBR *pRcCBRAttr);
extern INT32 h264Enc_setRcVBR(H264ENC_VAR *pH264EncVar, const H26XEncRcVBR *pRcVBRAttr);
extern INT32 h264Enc_getRcVBR(H264ENC_VAR *pH264EncVar, H26XEncRcVBR *pRcVBRAttr);
extern INT32 h264Enc_setRcEVBR(H264ENC_VAR *pH264EncVar, const H26XEncRcEVBR *pRcEVBRAttr);
extern INT32 h264Enc_getRcEVBR(H264ENC_VAR *pH264EncVar, H26XEncRcEVBR *pRcEVBRAttr);
extern INT32 h264Enc_setRcFixQp(H264ENC_VAR *pH264EncVar, const H26XEncRcFixQP *pRcFixQpAttr);
extern INT32 h264Enc_getRcFixQp(H264ENC_VAR *pH264EncVar, H26XEncRcFixQP *pRcFixQpAttr);
extern INT32 h264Enc_setRcQPMap(H264ENC_VAR *pH264EncVar, UINT32 uiEnable, UINT8 *QP_MAP);
extern INT32 h264Enc_getRcQPMap(H264ENC_VAR *pH264EncVar, UINT32 *uiEnable, UINT8 *QP_MAP);
extern INT32 h264Enc_setRowRcCfg(H264ENC_VAR *pH264EncVar, const H26XEncRowRc *pRowRcCfg);
extern INT32 h264Enc_getRowRcCfg(H264ENC_VAR *pH264EncVar, H26XEncRowRc *pRowRcCfg);
extern INT32 h264Enc_setRowRcEn(H264ENC_VAR *pH264EncVar, UINT32 uiEnable);
extern INT32 h264Enc_dumpRcStatus(H264ENC_VAR *pH264EncVar);
extern INT32 h264Enc_getRcStatus(H264ENC_VAR *pH264EncVar, H264ENC_RC_INFO *pRcInfo);

// others //
extern INT32 h264Enc_setTnrCfg(H264ENC_VAR *pH264EncVar, const H26XEncTnr *pTnrAttr);
extern INT32 h264Enc_getTnrCfg(H264ENC_VAR *pH264EncVar, H26XEncTnr *pTnrAttr);
extern INT32 h264Enc_setFroCfg(H264ENC_VAR *pH264EncVar, const H264EncFro *pFroAttr);
extern INT32 h264Enc_getFroCfg(H264ENC_VAR *pH264EncVar, H264EncFro *pFroAttr);
extern INT32 h264Enc_setGopAttr(H264ENC_VAR *pH264EncVar, const H26XEncGop *pGopAttr);
extern INT32 h264Enc_getGopAttr(H264ENC_VAR *pH264EncVar, H26XEncGop *pGopAttr);
extern INT32 h264Enc_setRdoAttr(H264ENC_VAR *pH264EncVar, const H26XEncRdo *pRdoAttr);
extern INT32 h264Enc_getRdoAttr(H264ENC_VAR *pH264EncVar, H26XEncRdo *pRdoAttr);
extern INT32 h264Enc_setMDInfo(H264ENC_VAR *pH264EncVar, const H26XEncMDInfo *pMDInfo);
extern UINT32 h264Enc_getSuggestPicType(H264ENC_VAR *pH264EncVar);

extern INT32 h264Enc_setNextBsBuf(H264ENC_VAR *pH264EncVar, UINT32 uiBufAddr, UINT32 BufLen);

// this is sample code for get codec rate control information //
typedef struct _H264SeqInfo_{
	UINT32 uiRcMode;
	
	UINT32 uiBitRate;
	UINT32 uiFrameRate;
	UINT32 uiGop;
	UINT32 uiMinIQp;
	UINT32 uiMaxIQp;
	UINT32 uiMinPQp;
	UINT32 uiMaxPQp;
	UINT32 uiIPQPOffset;
	UINT32 uiStaticTime;
	UINT32 uiChangePos;
	UINT32 uiFixIQp;
	UINT32 uiFixPQp;

	// AQ //
	UINT32 uiAqStr;
	UINT32 uiAqMax;
	UINT32 uiAqMin;

	// Row RC //
	UINT32 uiRowRcEn;
	UINT32 uiRowQpRange;
	UINT32 uiRowQpStep;	
}H264SeqInfo;

extern UINT32 h264Enc_getSeqInfo(H264ENC_VAR *pH264EncVar, H264SeqInfo *pSeqInfo);
// end : this is sample code for get codec rate control information //

extern void h264Enc_printDebugMsg(void);

#endif

