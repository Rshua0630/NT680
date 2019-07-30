/**
    NVT CV library functions.

    Definitions of NVT CV library functions.

    @file       NvtCvFuns.h
    @ingroup    NvtCv
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _NVTCVFUNS_H
#define _NVTCVFUNS_H

typedef enum
{
	// NOTE: DO NOT INSERT NEW CV FUNCTION ENUM INTO THE ENUM LIST.
	//       ADD NEW CV FUNCTION ENUM AT THE END.

	// - NvtCv functions -------------------------------------------------------
	NVTCV_FUN_BASE,
	NVTCV_TEST,

	// add new function below
	NVTCV_CNN_PREPROCESS,
	NVTCV_CNN_CONCAT,
	NVTCV_CNN_DC,
	NVTCV_CNN_DATA_TRANS,
	NVTCV_CNN_CONV,
	NVTCV_CNN_DECONV,
	NVTCV_CNN_RPN,
	NVTCV_CNN_ROI,
	NVTCV_CNN_FastPost,
	NVTCV_CDNN,
	NVTCV_CNN_YUV,
	NVTCV_CNN_SVM_TRANS,
	NVTCV_TSR,
	NVTCV_OT,
	NVTCV_DEPTH_FUSION,
	NVTCV_DEPTH_NTR,
	NVTCV_BC,
	NVTCV_CDNN_INIT,
	NVTCV_CDNN_EXEC,
	NVTCV_CDNN_FINAL,

	NVTCV_FUN_MAX,
	// - End NvtCv functions ---------------------------------------------------

	ENUM_DUMMY4WORD(NVTCV_FUN)
} NVTCV_FUN;


////////////////////////////////////////////////////////////////////////////////
typedef struct _NVTCV_TESTFUN_PRMS
{
    UINT32 addrSrcImg;
    UINT32 addrDstImg;
    UINT32 width;
    UINT32 height;
} NVTCV_TESTFUN_PRMS;

typedef struct _CNN_PREPROCESS_PRMS
{
    UINT32 addrSrcImg0;
    UINT32 addrSrcImg1;
    UINT32 addrSrcImg2;
    UINT32 addrMeanImg;
    UINT32 addrDstImg0;
    UINT32 addrDstImg1;
    UINT32 addrDstImg2;

    UINT32 input_width;
    UINT32 input_height;
    UINT32 input_channel;
    UINT32 mean_width;
    UINT32 mean_height;
    UINT32 mean_channel;
    UINT32 targetShape;
    UINT32 bitTransformMode;
    UINT32 isImgInput;
    UINT32 floatBase;
    UINT32 scale_ratio;
    UINT32 pad;
    UINT32 yuvTransform;
} CNN_PREPROCESS_PRMS;

typedef struct _CNN_CONCAT_PRMS
{
    UINT32 addrSrcImg1;
    UINT32 addrSrcImg2;
    UINT32 addrDstImg;
    UINT32 input_width;
    UINT32 input_height;
    UINT32 input_channel;
} CNN_CONCAT_PRMS;

typedef struct _CNN_DC_PRMS
{
	UINT32 srcFrameAddr;
	UINT32 dstFrameAddr;
    UINT32 workingBuffer;
	UINT32 DC_flag;
	UINT32 width;
	UINT32 height;
	UINT32 effectW;
	UINT32 effectH;
	UINT32 imGridH;
	UINT32 imGridW;
	UINT32 maxInput;
	UINT32 imNormalC;
	UINT32 overlapSize;
	UINT32 blockSize;
	UINT32 blockStep;
	UINT32 pad;
 	UINT32 m;
	UINT32 addPad;
    UINT32 nextLayerPool;
    UINT32 para_index;
    UINT32 bitTransform;
} CNN_DC_PRMS;

typedef struct _CNN_DATA_TRANS_PRMS
{
	UINT32 addrSrcImg0;
    UINT32 addrSrcImg1;
    UINT32 addrSrcImg2;
    UINT32 addrDstImg0;
    UINT32 addrDstImg1;
    UINT32 addrDstImg2;

    UINT32 input_width;
    UINT32 input_height;
    UINT32 input_channel;
    UINT32 bitTransformMode;
    UINT32 isImgInput;
} CNN_DATA_TRANS_PRMS;

typedef struct _CNN_CONV_PRMS
{
	UINT32 inputFeat;
    UINT32 inputKernel;
    UINT32 inputBias;
    UINT32 outputFeat;
    UINT32 workingBuffer;

    UINT32 feat_width;
    UINT32 feat_height;
    UINT32 feat_channel;
    UINT32 kernel_width;
    UINT32 kernel_height;
    UINT32 kernel_channel;
    UINT32 kernel_setnum;
    UINT32 output_width;
    UINT32 output_height;
    UINT32 output_channel;

    UINT32 stride;
    UINT32 pad;
    UINT32 group;
    UINT32 relu;
    UINT32 conv_shift;
    UINT32 conv_shift_b;
    //UINT32 biasShiftNum;
} CNN_CONV_PRMS;

typedef struct _CNN_DECONV_PRMS
{
	UINT32 inputFeat;
    UINT32 inputKernel;
    UINT32 inputBias;
    UINT32 outputFeat;
    UINT32 workingBuffer;

    UINT32 feat_width;
    UINT32 feat_height;
    UINT32 feat_channel;
    UINT32 kernel_width;
    UINT32 kernel_height;
    UINT32 kernel_channel;
    UINT32 kernel_setnum;
    UINT32 output_width;
    UINT32 output_height;
    UINT32 output_channel;

    UINT32 stride;
    UINT32 pad;
    UINT32 relu;
    UINT32 conv_shift;
    UINT32 conv_shift_b;
    //UINT32 biasShiftNum;
} CNN_DECONV_PRMS;

typedef struct _CNN_RPN_PRMS
{
	UINT32 p_bbox;
    UINT32 p_score;
    UINT32 p_anchor;
    UINT32 output;

    UINT32 perNMSTopN;
    UINT32 minScore;
    UINT32 nmsOverlapThres;
	UINT32 afterNMSTopN;
	UINT32 img_height;
	UINT32 img_width;
	UINT32 bbox_dim1;
	UINT32 bbox_dim2;
	UINT32 bbox_dim3;
	UINT32 bbox_dim4;
    UINT32 numOutput;
} CNN_RPN_PRMS;

typedef struct _CNN_ROI_PRMS
{
	UINT32 inputFeat;
    UINT32 rois;
    UINT32 output;

    UINT32 feat_width;
    UINT32 feat_height;
    UINT32 feat_channel;
    UINT32 rois_num;
    UINT32 pool_w;
    UINT32 pool_h;
    UINT32 spatialScaleFixPnt;
    UINT32 floatBitNum;
    UINT32 output_byte_size;
} CNN_ROI_PRMS;

typedef struct _CNN_FastPost_PRMS
{
	UINT32 p_box;
    UINT32 p_score;
    UINT32 p_box_delta;
    UINT32 output;

    UINT32 perNMSTopN;
    UINT32 nmsOverlapThres;
    UINT32 boxNum;
    UINT32 classNum;
    UINT32 perClasstopN;
    UINT32 img_height;
    UINT32 img_width;
    UINT32 thres;
    UINT32 numOutput;
    UINT32 scale;
} CNN_FastPost_PRMS;

typedef struct _CDNN_RSLT_OBJ
{
    CHAR label[256];
    INT16 score;
}CDNN_RSLT_OBJ;

typedef struct _CDNN_RSLT_DATA
{
    UINT8 length;
    CDNN_RSLT_OBJ obj[5];
}CDNN_RSLT_DATA;

typedef struct _CDNN_SEQHAND
{
	UINT32 srcFrameAddr;
	UINT32 pNetwork;
	UINT32 dstFrameAddr;
    UINT32 FrameAddr;
    UINT32 rsltAddr;
} CDNN_SEQHAND;

typedef struct _CNN_YUV_PRMS
{
	UINT32 addrSrcImgY;
    UINT32 addrSrcImgUV;
    UINT32 addrDstImg;

    UINT32 input_width;
    UINT32 input_height;
    UINT32 input_channel;
} CNN_YUV_PRMS;

typedef struct _CNN_SVMTRANS_PRMS
{
	UINT32 addrSrcImg;
    UINT32 addrDstImg;

    UINT32 input_size;
    UINT32 input_channel;
    UINT32 input_lineoffset;
} CNN_SVMTRANS_PRMS;

typedef struct _StereoInImage
{
	UINT32 src_Refined_addr;
	UINT32 width;
	UINT32 lofs;
	UINT32 height;
	UINT8 maxD;
	UINT8 Inval;
} StereoInImage;

typedef struct _DEPTH_FUSION_PRMS
{
	StereoInImage srcInput[3];
	UINT32 src_Y_addr;
	UINT32 src_Yrz_addr;
	UINT32 dst_Fusion_addr;
	UINT32 dst_wmf_addr;

	UINT32 max_disp;

	UINT32 dst_Conf_addr;
	UINT32 tmp_Pad1_addr;
	UINT32 tmp_Pad2_addr;
	UINT32 dst_AD1_addr;
	UINT32 dst_AD2_addr;
	UINT32 dst_AD1_pad_addr	;

	UINT8 Fusion_InvalOut;
	UINT8 Fusion_diff_th;
	UINT8 Fusion_tolerance;

	UINT8 NTREnable;

} DEPTH_FUSION_PRMS;

typedef struct _DEPTH_NTR_PRMS // Nontexture removal
{
	UINT32 src_AD1_addr;
	UINT32 src_AD2_addr;
	UINT32 src_AD1_label_addr;
	UINT32 src_AD1_label_u8_addr;
	UINT32 src_Diff_conf_addr;
	UINT32 src_Fusion_addr;

	UINT32 dst_ADcombine_addr;
	UINT32 dst_NTR_depth_addr;

	UINT32 width;
	UINT32 lofs;
	UINT32 height;
	UINT32 numLabels;

} DEPTH_NTR_PRMS;

#endif
