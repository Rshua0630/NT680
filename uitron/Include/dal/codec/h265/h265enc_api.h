#ifndef _H265_API_H_
#define _H265_API_H_

#include "Type.h"
#include "h26x_def.h"
#include "h265_def.h"

#define H265ENC_IN_MAX          1   /*!<  maximum input source number for H.264 encoding simultaneously. */
#define H265ENC_NUM_REF         1

typedef struct _H265ENC_INIT_ {
	// resolution and lineoffset //
	UINT32          uiWidth;            ///< Original source width, ex: 1920 x 1080, width = 1920
	UINT32          uiHeight;           ///< Original source height, ex: 1920 x 1080, height = 1080 (not 1088)
	UINT32          uiSarWidth;         ///< Sample Aspect Ratio width, ex: 1920 x 1080, width = 1920
	UINT32          uiSarHeight;        ///< Sample Aspect Ratio height, ex: 1920 x 1080, height = 1080 (not 1088)
	UINT32          uiDisplayWidth;     ///< Display source width, ex: 720 x 480, uiWidth = 736, uiDisplayWidth = 720

	// Bitstream Buffer  //
	UINT32          uiEncBufAddr;       ///< H265 encoder internal buffer starting address
	UINT32          uiEncBufSize;       ///< H265 encoder internal buffer size

	// Codec Settings //
	UINT32          uiEncProfile;       ///< H.264 encoder profile (baseline, main, high profile)
	UINT32          uiEncGopFrameNum;   ///< H.265 encoder GOP frame number

	// SVC //
	UINT32          uiSVCLayer;         ///< H.265 SVC Layer, range : 0,1,2

	// Long-term //
	UINT32          uiLTRInterval;      /// < H.265 long-term reference frame interval. range : 0 ~ 4095
	UINT32          uiLTRPreRef;        /// < H.265 long-term reference setting. 0 : all long-term reference to IDR frame, 1: reference latest long-term reference frame.

	// Deblocking //
	UINT8           ucDisableDB;        ///< H.265 disable_filter_idc (0=Filter, 1=No Filter, 2=Slice Mode)
	INT8            cDBAlpha;           ///< H.265 deblocking Alpha & C0 offset div. 2, {-6, -5, ... 0, +1, .. +6}
	INT8            cDBBeta;            ///< H.265 deblocking Beta offset div. 2, {-6, -5, ... 0, +1, .. +6}

	// SAO //
	UINT32          uiSAO;              // Sample Adaptive Offset : 0 : enable, 1 : disable
	UINT32          uiSaoLumaFlag;      // SAO Luma Flag
	UINT32          uiSaoChromaFlag;    // SAO Chroma Flag

	// rate control //
	UINT32          uiBitRate;          ///< Bit rate (bits per second)
	UINT32          uiFrameRate;        ///< frame rate (frames per second * 1000)

	UINT8           ucIQP;              ///< H.265 Rate Control's Init IQP
	UINT8           ucPQP;              ///< H.265 Rate Control's Init PQP
	UINT32          uiMaxIQp;           ///< H.265 Rate Control's Max I Qp
	UINT32          uiMinIQp;           ///< H.265 Rate Control's Min I Qp
	UINT32          uiMaxPQp;           ///< H.265 Rate Control's Max P Qp
	UINT32          uiMinPQp;           ///< H.265 Rate Control's Min P Qp
	INT32           iIPQPoffset;        ///< H.265 Rate Control's I P QP offset (not ready)
	UINT32          uiStaticTime;       ///< H.265 rate control's static time, unit: second

	// row-level rate control //
	UINT32          uiRowRcEnalbe;      ///< H.265 Rate Control's enalbe row level rate control
	UINT32          uiRowRcQpDelta;     ///< H.265 Rate Control's row level rate control QP-delta
	UINT32 			uiRowRcQpStep;		///< H.265 Rate Control's row level rate control QP-Step

	// NT96680 //
	UINT32          uiRefreshLineNum;   /// <.H265 Intra refresh row numbers
	UINT32          uiRecCompressEn;    // reconstruct compression (ECLS) //
	UINT32          uiRotate;           // rotate option : 0 : no rotate , 1 : counterclockwise rotate , 2 : clockwise rotate //
	UINT32          uiUsrQpSize;        // 0 : 64x64, 1 : 16x16

    // R Frame
	UINT32          uiKeyPPeriod;
	INT32           iKeyPQPOffset;

	// fast search range select //
	UINT8           ucFastSearch;		/// < H.265 fast search range select : 0 : small range, 1 : big range
	// SEI //
	UINT8           ucSEIIdfEn;
    INT32           iIPWeight;

	UINT8           ucColorRange;		// 0 : tv range ,  1 : full range //
	UINT32          uiPrjMode;
	
	UINT32			uiMultiTLayer;		// H.265 Use Multi Temporal Layer : 0 : disable, 1: enable
} H265ENC_INIT, *PH265ENC_INIT;

typedef struct _H265ENC_RESULT_ {
	UINT32 uiSrcTimeStamp;
	UINT32 uiFrameType; //0~3:	define the same with SLICE_TYPE,  4: KeyP 
	UINT32 uiBaseQP;
	UINT32 uiBsAddr;
	UINT32 uiBsSize;
	UINT32 uiTemporalId;
	UINT32 uiNaluNum;
	UINT32 uiNaluBsLen[8];
} H265ENC_RESULT;

typedef struct _H265ENC_INFO_ {
	UINT32      uiSrcYAddr[H265ENC_IN_MAX];     ///< Source Y address
	UINT32      uiSrcUVAddr[H265ENC_IN_MAX];    ///< Source UV address
	UINT32      uiSrcCompressEn;                ///< source compression
	H26XEncSrcDecomp *pSrcCompressInfo;         ///< src compression info
	UINT32      uiYLineOffset;                  ///< Y line offset
	UINT32      uiUVLineOffset;                 ///< UV line offset
	UINT32      uiBSAddr[H265ENC_IN_MAX];       ///< Encoded bit-stream address
	UINT32      uiBSSize[H265ENC_IN_MAX];       ///< Encoded bit-stream size
	UINT32      uiMaxBSLen;                     ///< Maximum bit-stream length
	SLICE_TYPE  uiEncFrameType;                 ///< Encode frame type (I, Idr, P, or B)
	UINT32      uiEncFrameNum;                  ///< Encode frame number (single, 2-B, ...)
	UINT32      uiSVCSize[H265ENC_IN_MAX];      ///< SVC header size
	UINT32      uiTemporalId[H265ENC_IN_MAX];
	UINT32      uiSrcOutYAddr[H265ENC_IN_MAX];  ///< Source Out (3DNR) Y address
	UINT32      uiSrcOutUVAddr[H265ENC_IN_MAX]; ///< Source Out (3DNR) UV address
	UINT32      uiSrcOutEnable;
	UINT32		uiSrcOutYLineOffset;
	UINT32 		uiSrcOutUVLineOffset;
	UINT32      uiSrcTimeStamp;
	UINT32		uiTuneQpEn;
} H265ENC_INFO, *PH265ENC_INFO;

typedef struct _H265ENC_VAR_ {
	UINT32 uiEncId;
	void   *pContext;
} H265ENC_VAR;

typedef struct _H265ENC_RC_INFO_
{
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
} H265ENC_RC_INFO, *PH265ENC_RC_INFO;

extern UINT32 h265Enc_queryMemSize(const H26XEncMemAttr *pMemAttr);
extern INT32 h265Enc_initEncoder(PH265ENC_INIT pH264EncInit, H265ENC_VAR *pH265EncVar);
extern INT32 h265Enc_getH265Header(UINT32 *puiAddr, UINT32 *puiLen, H265ENC_VAR *pH265EncVar);
extern INT32 h265Enc_encodeOnePicture(PH265ENC_INFO pH265EncInfo, H265ENC_VAR *pH265EncVar);
extern INT32 h265Enc_IsEncodeFinished(H265ENC_VAR *pH265EncVar);
extern INT32 h265Enc_getResult(H265ENC_INFO *pH265EncInfo, H265ENC_RESULT *pEncResult, H265ENC_VAR *pH265EncVar);
extern void h265Enc_stopEncoder(H265ENC_VAR *pH265EncVar);
extern INT32 h265Enc_waitEncodeFinished(H265ENC_VAR *pH265EncVar);
extern UINT32 h265Enc_AskEncodeFinished(H265ENC_VAR *pH265EncVar);
extern INT32 h265Enc_rewriteHeader(PH265ENC_INIT pH265EncInit, H265ENC_VAR *pH265EncVar);

// User control APIs //
extern INT32 h265Enc_setAqCfg(H265ENC_VAR *pH265EncVar, const H26XEncAq *pAqAttr);
extern INT32 h265Enc_getAqCfg(H265ENC_VAR *pH265EncVar, H26XEncAq *pAqAttr);
extern INT32 h265Enc_setRoiCfg(H265ENC_VAR *pH265EncVar, UINT32 uiIdx, const H26XEncRoi *pRoiAttr);
extern INT32 h265Enc_getRoiCfg(H265ENC_VAR *pH265EncVar, UINT32 uiIdx, H26XEncRoi *pRoiAttr);
extern INT32 h265Enc_setSmartRoiCfg(H265ENC_VAR *pH265EncVar, const H26XEncSmartRoi *pSmartRoiAttr);
extern INT32 h265Enc_getSmartRoiCfg(H265ENC_VAR *pH265EncVar, H26XEncSmartRoi *pSmartRoiAttr);
extern INT32 h265Enc_setSliceSplit(H265ENC_VAR *pH265EncVar, const H26XEncSliceSplit *pSliceSplit);
extern INT32 h265Enc_getSliceSplit(H265ENC_VAR *pH265EncVar, H26XEncSliceSplit *pSliceSplit);
extern INT32 h265Enc_setColor2Grey(H265ENC_VAR *pH265EncVar, UINT32 uiEnable);
extern INT32 h265Enc_getColor2Grey(H265ENC_VAR *pH265EncVar, UINT32 *uiEnable);
extern INT32 h265Enc_setForceISlice(H265ENC_VAR *pH265EncVar, UINT32 uiStartSlice, UINT32 uiSliceNum);
extern INT32 h265Enc_getForceISlice(H265ENC_VAR *pH265EncVar, UINT32 *pStartSlice, UINT32 *pSliceNum);
extern INT32 h265Enc_setGdr(H265ENC_VAR *pH265EncVar, UINT32 uiEnable, UINT32 uiRefreshLineNum);
extern INT32 h265Enc_getGdr(H265ENC_VAR *pH265EncVar, UINT32 *uiEnable, UINT32 *uiRefreshLineNum);

// Rate control APIs //
extern INT32 h265Enc_setRcCBR(H265ENC_VAR *pH265EncVar, const H26XEncRcCBR *pRcCBRAttr);
extern INT32 h265Enc_getRcCBR(H265ENC_VAR *pH265EncVar, H26XEncRcCBR *pRcCBRAttr);
extern INT32 h265Enc_setRcVBR(H265ENC_VAR *pH265EncVar, const H26XEncRcVBR *pRcVBRAttr);
extern INT32 h265Enc_getRcVBR(H265ENC_VAR *pH265EncVar, H26XEncRcVBR *pRcVBRAttr);
extern INT32 h265Enc_setRcEVBR(H265ENC_VAR *pH265EncVar, const H26XEncRcEVBR *pRcEVBRAttr);
extern INT32 h265Enc_getRcEVBR(H265ENC_VAR *pH265EncVar, H26XEncRcEVBR *pRcEVBRAttr);
extern INT32 h265Enc_setRcFixQp(H265ENC_VAR *pH265EncVar, const H26XEncRcFixQP *pRcFixQpAttr);
extern INT32 h265Enc_getRcFixQp(H265ENC_VAR *pH265EncVar, H26XEncRcFixQP *pRcFixQpAttr);
extern INT32 h265Enc_setRcQPMap(H265ENC_VAR *pH265EncVar, UINT32 uiEnable, UINT8 *QP_MAP);
extern INT32 h265Enc_getRcQPMap(H265ENC_VAR *pH265EncVar, UINT32 *uiEnable, UINT8 *QP_MAP);
extern INT32 h265Enc_setRowRcCfg(H265ENC_VAR *pH265EncVar, const H26XEncRowRc *pRowRcCfg);
extern INT32 h265Enc_getRowRcCfg(H265ENC_VAR *pH265EncVar, H26XEncRowRc *pRowRcCfg);
extern INT32 h265Enc_setRowRcEn(H265ENC_VAR *pH265EncVar, UINT32 uiEnable);
extern INT32 h265Enc_dumpRcStatus(H265ENC_VAR *pH265EncVar);
extern INT32 h265Enc_getRcStatus(H265ENC_VAR *pH265EncVar, H265ENC_RC_INFO *pRcInfo);

// others //
extern INT32 h265Enc_setTnrCfg(H265ENC_VAR *pH265EncVar, const H26XEncTnr *pH265TnrAttr);
extern INT32 h265Enc_getTnrCfg(H265ENC_VAR *pH265EncVar, H26XEncTnr *pH265TnrAttr);
extern INT32 h265Enc_setFroCfg(H265ENC_VAR *pH265EncVar, const H26XEncFro *pH265FroAttr);
extern INT32 h265Enc_getFroCfg(H265ENC_VAR *pH265EncVar, H26XEncFro *pH265FroAttr);
extern INT32 h265Enc_setMDInfo(H265ENC_VAR *pH265EncVar, const H26XEncMDInfo *pMDInfo);
extern UINT32 h265Enc_getSuggestPicType(H265ENC_VAR *pH265EncVar);

extern INT32 h265Enc_setNextBsBuf(H265ENC_VAR *pH265EncVar, UINT32 uiBufAddr, UINT32 BufLen);

// this is sample code for get codec rate control information //
typedef struct _H265SeqInfo_{
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
}H265SeqInfo;

extern UINT32 h265Enc_getSeqInfo(H265ENC_VAR *pH264EncVar, H265SeqInfo *pSeqInfo);
// end : this is sample code for get codec rate control information //


extern void h265Enc_printDebugMsg(void);
#endif


