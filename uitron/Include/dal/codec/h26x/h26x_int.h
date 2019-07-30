#ifndef _H26X_INT_H_
#define _H26X_INT_H_

#include "Type.h"

#include "h26x_def.h"

typedef enum {
	FRM_IDX_ST_0 = 0,
	FRM_IDX_LT_0,
	FRM_IDX_ST_1,
	FRM_IDX_NON_REF,
	FRM_IDX_MAX,
	FRM_IDX_TMP_0 = FRM_IDX_MAX
} H26XFRM_IDX;

typedef struct _H26XEncAddr {
	UINT32 uiSrcYAddr, uiSrcUVAddr;
	UINT32 uiFrmBufNum;
	UINT32 uiRefAndRecEn;
	UINT32 uiRefAndRecYAddr[FRM_IDX_MAX], uiRefAndRecUVAddr[FRM_IDX_MAX];
	INT32  iRefAndRecPOC[FRM_IDX_MAX];
	INT32  iColRefAndRecPOC[FRM_IDX_MAX];
	UINT32 uiRefAndRecIsIntra[FRM_IDX_MAX];
	UINT32 uiRefAndRecIsLT[FRM_IDX_MAX];	
	UINT32 uiRef0Idx;
	UINT32 uiRecIdx;
	UINT32 uiPicBsAddr;
	UINT32 uiEDKTabl[FRM_IDX_MAX][9];
	UINT32 uiIlfSideInfoAddr[FRM_IDX_MAX];
	UINT32 uiIlfRIdx;
	UINT32 uiIlfWIdx;
	UINT32 uiCMDBufAddr;
	UINT32 uiHwBsAddr;
	UINT32 uiCacheSramOpt;

	UINT32 uiSeqHeaderAddr;

	UINT32 uiHwHeaderNum;
	UINT32 uiHwHeaderAddr;
	UINT32 uiHwHeaderSize[H26X_MAX_BSDMA_NUM];

	UINT32 uiColMvsAddr[FRM_IDX_MAX+1];
	UINT32 uiUserQpAddr;
	UINT32 uiNROutYAddr;
	UINT32 uiNROutUVAddr;
	UINT32 uiRCRefBufAddr[2][2];
	UINT32 uiRcRefRdAddr;
	UINT32 uiRcRefWrAddr;

	UINT32 uiSdeSideInfoAddr;
    UINT32 uiUserQpTmpAddr;
} H26XEncAddr;

typedef struct _H26XDecAddr {
	UINT32 uiFrmBufNum;
	UINT32 uiRefAndRecYAddr[FRM_IDX_MAX], uiRefAndRecUVAddr[FRM_IDX_MAX];
	INT32  iRefAndRecPOC[FRM_IDX_MAX];
	INT32  iColRefAndRecPOC[FRM_IDX_MAX];
	UINT32 uiRefAndRecIsIntra[FRM_IDX_MAX];
	UINT32 uiRefAndRecIsLT[FRM_IDX_MAX];	
	UINT32 uiRef0Idx;
	UINT32 uiRecIdx;
	UINT32 uiURIAddr;
	UINT32 uiRecURAddr;
	UINT32 uiIlfUpBotAddr;
	UINT32 uiIlfSideInfoAddr[2];
	UINT32 uiIlfRIdx;
	UINT32 uiIlfWIdx;
	UINT32 uiCMDBufAddr;
	UINT32 uiHwBsAddr;

	UINT32 uiHwBSCmdNum;
	UINT32 uiHwBsCmdSize[H26X_MAX_BSDMA_NUM];

	UINT32 uiColMvsAddr[FRM_IDX_MAX];
	UINT32 uiColMvRIdx;
	UINT32 uiColMvWIdx;
} H26XDecAddr;


#endif //_H26X_INT_H_

