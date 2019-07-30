#ifndef _NVTNN_COMMON_H_
#define _NVTNN_COMMON_H_

#include "GxImage.h"
#include "ise_lib.h"
#include "cnn_lib.h"
#include "nvtnn_lib.h"
#include "NvtSvm.h"
#include "NvtCvFuns.h"
#include "NvtCv_lib.h"

#define MAX_DETECTED_OBJECT     16
#define __CNN_PERF__        0
#define __CNN_WRITE_FILE__  0

typedef enum //
OD_NETWORK{
    CNN_ALEXNET= 0,
    CDNN_ALEXNET,
    CNN_NODNET,
    CNN_NODNETLITE,
    CNN_NODNETPIPE,
    CNN_NODNETPIPE_TEST,
    CNN_RESNET50,
    CNN_RESNET101,
} OD_NETWORK;

///{
///		"Memory related" struct : NN_DATA, NN_Buffer, NN_IOMEM
///}
//{
typedef struct _NN_DATA
{
    UINT32 address;
    UINT32 size;
}NN_DATA;

typedef struct _NN_Buffer
{
	UINT32 buffer_LayerID;
	UINT32 buffer_InAddr;
	UINT32 buffer_OutAddr;
	UINT32 buffer_Life;
}NN_Buffer;

typedef struct _NN_IOMEM
{
    NN_DATA SAI[6];
    NN_DATA SAO[3];
    NN_DATA SAP[3]; //Parameter
    //NN_DATA SAI_para2; //Parameter
}NN_IOMEM;
//}

typedef struct _NN_Mean_IMG
{
    UINT32 address;
    UINT32 width;
    UINT32 height;
    UINT32 channel;
}NN_Mean_IMG;

typedef struct _NN_SVM_MODEL
{
    NN_DATA sv;
    NN_DATA alpha;
    NN_DATA rho;
}NN_SVM_MODEL;


typedef enum
{
    NN_CONV = 0,
    NN_POOL,
    NN_LRN,
    DIV_CONV_COM,
    DIV_CONV_COM2,
    DSP_ROI_POOL,
    NN_SVM,
    DSP_DECONV,
    CONCAT25,
    DSP_CONV,

    DSP_CONV_RPN = 10,
    NN_BATCHNORM,
    NN_SCALE,
    NN_RELU,
    NN_ELTWISE,
    NN_SOFTMAX,


/// Add new function index here
    MAX_MODE,
} NN_MODE;

typedef struct _NN_LAYER_IDX_Reg
{
    INT32 layer_index;
    INT32 last_layerIndex[10];
    INT32 pre_layerNum;
	INT32 next_layerNum;
}NN_LAYER_IDX_Reg;

typedef struct _NN_LAYER_CFG_Reg
{
    NN_MODE mode;
    INT32 isDsp;
	INT32 isPara;
    INT32 DC_flag;
    INT32 isIn12to16;
    INT32 isOut16to12;
}NN_LAYER_CFG_Reg;

typedef struct _NN_DEF_BIT_Reg
{
    INT32 in_sign_bit;
    INT32 in_int_bit;
    INT32 in_frac_bit;
    INT32 out_sign_bit;
    INT32 out_int_bit;
    INT32 out_frac_bit;
    INT32 kernel_frac_bit;
    INT32 bias_frac_bit;
}NN_DEF_BIT_Reg;

typedef struct _NN_SIZE_Reg
{
    UINT32 in_width;
    UINT32 in_height;
    UINT32 in_channel;

    UINT32 out_width;
    UINT32 out_height;
    UINT32 out_channel;

    UINT32 in_lofs;
    UINT32 out_lofs;
}NN_SIZE_Reg;

typedef struct _NN_CONV_POOL_Reg
{
    INT32 kerl_size; // kernel_size
    INT32 num_output;
    INT32 pad;
    INT32 stride;
    INT32 kerl_w; // kernel_w
    INT32 kerl_h; //kernel_h
    INT32 pad_w;
    INT32 pad_h;
    INT32 group;
    INT32 conv_shift;

    INT32 conv_shift_B;
    INT32 relu_kerl; // do_relu
    INT32 pooling_type;
    INT32 global_pooling;
    INT32 bias_term;
}NN_CONV_POOL_Reg;

typedef struct _NN_LRN_Reg
{
    INT32 alpha;
    INT32 beta;
    INT32 n;
    INT32 shift;
}NN_LRN_Reg;

typedef struct _NN_ROI_POOLING_Reg
{
    INT32 pool_w; // pooled_w
    INT32 pool_h; // pooled_h
    float spatial_scale;
}NN_ROI_POOLING_Reg;

typedef struct _NN_ELTWISE_Reg
{
    NVTNN_ELTWISE_OP op;
    INT32 coeff1;
    INT32 coeff2;
    INT32 coeff_q;
}NN_ELTWISE_Reg;

typedef struct _NN_SVM_Reg
{
    INT32 op;
    INT32 coeff1;
    INT32 coeff2;
    INT32 coeff_q;
}NN_SVM_Reg;

typedef struct _DC_Reg
{
    UINT32 isNextLayerPool;
    UINT32 bitTransform;
}NN_DC_DIV_Reg, NN_DC_Comb_Reg;

typedef struct _NN_Reg
{
    NN_LAYER_IDX_Reg layerIdxReg;
    NN_LAYER_CFG_Reg layerCfg;
    NN_DEF_BIT_Reg defineBit;
    NN_SIZE_Reg sizeReg;
    NN_CONV_POOL_Reg convPoolReg;
    NN_LRN_Reg lrnReg;
    NN_ROI_POOLING_Reg roiPoolReg;
    NN_ELTWISE_Reg eltwiseReg;
    SVM_KERL_PARM svmKerlReg;
    //NN_DC_DIV_Reg divReg[2];
    //NN_DC_Comb_Reg combReg[2];
}NN_Reg;

// CNN_Reg: old version, keep it for NODNet reg
typedef struct _CNN_Reg
{
    int layer_index;
    int last_layerIndex[10];
    int pre_layerNum;
	int next_layerNum;
    int mode;
    int input_lofs;
    int output_lofs;
    int width;
    int height;
    int channel;
    int kernel_size;

    int stride;
	int pad;
    int num_output;
    int conv_shift;
    int conv_shift_B;
    int relu_kernel;
    int pooling_type;
    int alpha;
    int beta;
    int local_size;

    int LRN_shift;
    int group;
    int pooled_w;
    int pooled_h;
    int DC_flag;
    float spatial_scale;
    NN_DC_DIV_Reg divReg[2];
    NN_DC_Comb_Reg combReg[2];
}CNN_Reg;

///{
///		"Hardware" CNN struct : CNN_LIB_PARA ,NN_SVM_CLAS_PARA
///}
//{
typedef struct _CNN_LIB_PARA
{
    CNN_PAPAM *pCnnPara;
    UINT32 rois_num;
    UINT32 gridNum;
    UINT32 group;
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAI2_Addr;
    UINT32 SAI3_Addr;
    UINT32 SAO_Addr;
}CNN_LIB_PARA;

typedef struct _CNN_CONV_POOL_LRN_PARA
{
    CNN_PAPAM cnnPara;
    UINT32 rois_num;
    UINT32 gridNum;
    UINT32 group;
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAI2_Addr;
    UINT32 SAI3_Addr;
    UINT32 SAO_Addr;
}CNN_CONV_POOL_LRN_PARA;

typedef struct _NN_SVM_CLAS_PARA
{
    SVM_KERL_PARM svmKerlParms;
    SVM_DMAIO_ADDR dmaIO_Addr;
    UINT32 vecSize;
    UINT32 supVecNum;
    UINT32 rois_num;
}NN_SVM_CLAS_PARA;

typedef struct _NN_Result
{
    UINT32 class;
    INT32 score;
    UINT32 x;
    UINT32 y;
    UINT32 w;
    UINT32 h;
    char name[256];
}NN_Result;

typedef struct _NN_POST_PARA_NODNet
{
    // rpn
    INT32 min_score;

    // detection
    INT32 input_class_num;
    INT32 det_score_thres;
}NN_POST_PARA_NODNet;

///{
///		"DSP" CNN struct : CNN_LIB_PARA ,NN_SVM_CLAS_PARA
///}
//{
typedef struct _NN_ROI_POOL_PARA
{
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAO_Addr;
    CNN_IN_SIZE cnnSize;
    UINT32 rois_num;
    UINT32 pool_w;
    UINT32 pool_h;
    UINT32 spatialScaleFixPnt;
    UINT32 floatBitNum;
}NN_ROI_POOL_PARA;

typedef struct _NN_RPN_PARA
{
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAO_Addr;
    UINT32 anchor_Addr;
    UINT32 perNMSTopN;
    INT32 minScore;
    UINT32 nmsOverlapThres;
    UINT32 afterNMSTopN;
    INT32 img_height;
    UINT32 img_width;
    UINT32 bbox_dim1;
    UINT32 bbox_dim2;
    UINT32 bbox_dim3;
    UINT32 bbox_dim4;
    UINT32 *rois_num;
}NN_RPN_PARA;

typedef struct _NN_FASTPOST_PARA
{
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAI2_Addr;
    UINT32 SAO_Addr;
    UINT32 perNMSTopN;
    UINT32 nmsOverlapThres;
    UINT32 boxNum;
    UINT32 classNum;
    UINT32 perClasstopN;
    UINT32 img_height;
    UINT32 img_width;
    UINT32 scale;
    UINT32 *oNumBox;
}NN_FASTPOST_PARA;

typedef struct _NN_DECONV_PARA
{
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAI2_Addr;
    UINT32 SAO_Addr;
    UINT32 wrokBuff;
    UINT32 kernelAddr;
    UINT32 biasAddr;
    CNN_IN_SIZE feat_size;
    UINT32 kernel_size;
    UINT32 kernel_setnum;
    UINT32 stride;
    UINT32 pad;
    UINT32 relu;
    UINT32 conv_shift;
    UINT32 conv_shift_b;
    //UINT32 biasShiftNum;
}NN_DECONV_PARA;

typedef struct _NN_DC_PARA
{
    UINT32 SAI_Addr;
    UINT32 SAO_Addr;
    UINT32 workBuff;
    UINT32 width;
    UINT32 height;
    UINT32 channel;
    UINT32 pad;
    CNN_CONV_KERNEL conv_kersel;
    CNN_CONVK5N3_STRIDE conv_stride;
    UINT32 DorC_flag;
    UINT32 isNextLayerPool;
    UINT32 para_index;
    UINT32 bitTransform;
    UINT32 *oGridNum;
}NN_DC_PARA;

typedef struct _NN_DSP_CONV_PARA
{
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAI2_Addr;
    UINT32 SAO_Addr;
    UINT32 workBuff;
    UINT32 kerAddr;
    UINT32 biasAddr;
    CNN_IN_SIZE feat_size;
    UINT32 kernel_size;
    UINT32 kernel_setnum;
    UINT32 stride;
    UINT32 pad;
    UINT32 group;
    UINT32 relu;
    UINT32 conv_shift;
    UINT32 conv_shift_b;
    //UINT32 biasShiftNum;
}NN_DSP_CONV_PARA;

typedef struct _NN_PREPROCESS_PARA
{
    UINT32 SAI0_Addr;
    UINT32 SAI1_Addr;
    UINT32 SAI2_Addr;
    UINT32 SAO0_Addr;
    UINT32 SAO1_Addr;
    UINT32 SAO2_Addr;
    UINT32 meanImgAddr;
    CNN_IN_SIZE inSize;
    CNN_IN_SIZE outSize;
    CNN_IN_SIZE meanImgSize;
    UINT32 meanImgSharp;
    UINT32 pad;
    UINT32 yuvTransform;
    UINT32 *scale_ratio;
}NN_PREPROCESS_PARA;

typedef struct _FRAC_BIT
{
    UINT32 input_fracbit;
    UINT32 output_fracbit;
    UINT32 kernel_fracbit;
    UINT32 bias_fracbit;
}FRAC_BIT;

static const UINT32 CNN_Clk = 480;

/// ======== NN set parameters function  =============
void NN_SetConvPrms(NVTNN_CONV_PRMS *pPrms, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void NN_SetBatchPrms(NVTNN_BATCHNORM_PRMS *pPrms, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void NN_SetScalePrms(NVTNN_SCALE_PRMS *pPrms, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void NN_SetReluPrms(NVTNN_RELU_PRMS *pPrms, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void NN_SetPoolPrms(NVTNN_POOL_PRMS *pPrms, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void NN_SetEltPrms(NVTNN_ELTWISE_PRMS *pPrms, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void NN_SetSvmPrms(NN_SVM_CLAS_PARA *pPara, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void NN_SetSoftmaxPrms(NVTNN_SOFTMAX_PRMS *pPrms, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void Cnn_SetCnnLibPara(CNN_LIB_PARA *pPara, UINT32 rois_num, UINT32 gridNum, NN_Reg *pRegPara, NN_IOMEM *pMem, UINT32 layer_index);
void Cnn_SetCnnLibPara_NODNet(CNN_LIB_PARA *pPara, UINT32 rois_num, UINT32 gridNum, NN_Reg *pRegPara, NN_IOMEM mem, UINT32 layer_index);
void NN_SetDivCnnDCPara(NVTNN_DIVCOMB_PRMS *prms_dc, UINT32 SAI_Addr, UINT32 SAO_Addr, UINT32 layer_index, UINT32 *gridNum, CNN_Reg *gReg_para);
void NN_SetDivNNDCPara(NVTNN_DIVCOMB_PRMS *prms_dc, UINT32 SAI_Addr, UINT32 SAO_Addr, UINT32 layer_index, UINT32 *gridNum, NN_Reg *gReg_para);
void NN_SetCombCnnDCPara(NVTNN_DIVCOMB_PRMS *prms_dc, UINT32 SAI_Addr, UINT32 SAO_Addr, UINT32 layer_index, UINT32 *gridNum, CNN_Reg *gReg_para, NN_DC_Comb_Reg comb_reg);
void NN_SetCombNNDCPara(NVTNN_DIVCOMB_PRMS *prms_dc, UINT32 SAI_Addr, UINT32 SAO_Addr, UINT32 layer_index, UINT32 *gridNum, NN_Reg *gReg_para, UINT8 next_layer_pool);
void NN_SetBitTransPara(NVTNN_BIT_TRANS_PRMS *prms, UINT8 trans_input, UINT8 trans_output, UINT32 input_buf, UINT32 output_buf);
void NN_SetPreProcessPara(NVTNN_PREPROCESS_PRMS *prms, NN_IOMEM mem, NVTNN_SHAPE input_shape, NVTNN_SHAPE mean_shape, INT32 target_shape, INT32 pad, UINT8 yuv_trans, UINT32 addrMeanImg, NVTNN_BIT_TRANS_MODE bit_trans_mode, UINT8 is_img_input, UINT8 scale_q, UINT32 *scale_ratio, NVTNN_PREPROC_OUT_TYPE output_type);
void NN_SetDspConvPara(NVTNN_CONV_PRMS *pPara, UINT32 SAI0_Addr, UINT32 SAI1_Addr, NN_DATA kernel, NN_DATA bias, UINT32 workBuff, CNN_Reg gReg_para_in, CNN_Reg gReg_para_out, UINT32 layer_index, FRAC_BIT frac_bit, UINT8 relu, UINT8 reshape, UINT32 rois_num, UINT32 maxInputSpace);
void NN_SetDspConvPara_NODNet(NVTNN_CONV_PRMS *pPara, UINT32 SAI0_Addr, UINT32 SAO0_Addr, NN_DATA kernel, NN_DATA bias, UINT32 workBuff, NN_Reg *gRegPara, UINT32 layer_index, UINT8 reshape, UINT32 rois_num, UINT32 maxInputSpace);
VOID NN_SetRPNPostProcPara(NVTNN_RPN_POSTPROC_PRMS *prms, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAI2_Addr, UINT32 SAO0_Addr, UINT32 work_buff, INT32 img_height, INT32 img_width, INT32 input_box_num, INT32 per_nms_top_n, INT32 min_score, INT32 nms_overlap_thres, INT32 max_output_box_num);
VOID NN_SetFRCNNPostProcPara(NVTNN_FRCNN_POSTPROC_PRMS *prms, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAI2_Addr, UINT32 SAO0_Addr, INT32 *out_box_num, UINT32 work_buff, INT32 *map_class_thres, INT32 input_class_num, INT32 img_height, INT32 img_width, INT32 input_box_num, INT32 per_class_top_n, INT32 per_nms_top_n, INT32 nms_overlap_thres, INT32 scale, INT32 det_score_thres);
VOID NN_SetDeconvProcPara(NVTNN_DECONV_PRMS *prms, UINT32 SAI0_Addr, UINT32 kernelAddr, UINT32 biasAddr, UINT32 SAO0_Addr, UINT32 work_buff, UINT32 layer_index, CNN_Reg *gReg_para, UINT8 input_q, UINT8 kernel_q, UINT8 output_q, UINT8 bias_q);
VOID NN_SetDeconvProcPara_NODNet(NVTNN_DECONV_PRMS *prms, UINT32 SAI0_Addr, UINT32 kernelAddr, UINT32 biasAddr, UINT32 SAO0_Addr, UINT32 work_buff, UINT32 layer_index, NN_Reg *gReg_para);
//VOID NN_SetDeconvProcParaa(NVTNN_DECONV_PRMS *prms, UINT32 inputAddr, UINT32 kernelAddr, UINT32 biasAddr, UINT32 outputAddr, UINT32 workAddr, UINT32 img_ch, UINT32 img_h, UINT32 img_w, UINT32 ker_w, UINT32 ker_h, UINT32 out_ch, INT32 stride_h, INT32 stride_w, INT32 pad_h, INT32 pad_w, UINT8 relu, UINT8 input_q, UINT8 kernel_q, UINT8 output_q, UINT8 bias_q);
VOID NN_SetRoiProcPara(NVTNN_ROI_PRMS *prms, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAO0_Addr, UINT32 work_buff, UINT32 layer_index, CNN_Reg *gReg_para, UINT32 rois_num, UINT32 spatialScaleFixPnt, UINT32 floatBitNum);
VOID NN_SetRoiProcPara_NODNet(NVTNN_ROI_PRMS *prms, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAO0_Addr, UINT32 work_buff, UINT32 layer_index, NN_Reg *gReg_para, UINT32 rois_num, UINT32 spatialScaleFixPnt, UINT32 floatBitNum);
void NN_SetSvmTransPara(NVTNN_SVMTRANS_PRMS *prms, UINT32 SAI0_Addr, UINT32 SAO0_Addr, UINT32 rois_num, UINT32 layer_index, CNN_Reg *gReg_para);
void NN_SetSvmTransPara_NODNet(NVTNN_SVMTRANS_PRMS *prms, UINT32 SAI0_Addr, UINT32 SAO0_Addr, UINT32 rois_num, UINT32 layer_index, NN_Reg *gReg_para);

/// ======== SVM Function  ========
void NN_SVM_Classify(UINT32 used_Start_Address, NN_SVM_CLAS_PARA para);
/// ======== CNN Buffer Function  ========
UINT32 NN_CalAddress(NN_IOMEM *oMem, NN_Reg *pRegPara, UINT32 layers_num, UINT32 blkMemSize, UINT32 *used_start_Addr, UINT32 workBuff_Addr);

void NN_LoadData(char *pFIleName, UINT32 addr, NN_DATA *pOutData);
void NN_Save_Data(char *ImgFilePath, UINT32 dataAddr, UINT32 dataSize);
BOOL NN_SwitchVal(UINT32 *pval1, UINT32 *pval2);
UINT32 NN_LoadSVMModel(UINT32 addr, char *svFile, char *alphaFile, char *rhoFile, NN_SVM_MODEL *oModel);

ER NN_resizeInputData(IMG_BUF *pInputImg, IMG_BUF *pOutImg, URECT roi, ISE_IO_PACK_SEL packSel, UINT32 plane);
ER NN_resizeYData(IMG_BUF *pInputImg, IMG_BUF *pOutImg, URECT roi);
ER NN_resizeUVData(IMG_BUF *pInputImg, IMG_BUF *pOutImg, URECT roi);
ER NN_resizeYUVData(IMG_BUF *pInputImg, IMG_BUF *pOutImg, URECT roi);

/// ======== CNN Function ==========
void Cnn_Copy(UINT32 SAI0_Addr,UINT32 SAO_Addr,UINT32 input_width,UINT32 input_height,UINT32 input_channel);
void NN_Feature_Copy(UINT32 SAI0_Addr,UINT32 SAO_Addr,UINT32 input_width,UINT32 input_height,UINT32 input_channel);
void CNN_Init(UINT32 clk);
void CNN_Process(UINT32 used_Start_Address, CNN_LIB_PARA para);

/// old functions
void CNN_SetParms(CNN_PAPAM *pPara, UINT32 layer_index, CNN_Reg *nn_reg);
//void Cnn_SetDivCnnDCPara(NN_DC_PARA *pPara, UINT32 SAI_Addr, UINT32 SAO_Addr, UINT32 workBuff, UINT32 layer_index, NN_DC_DIV_Reg divReg, UINT32 *pGridNum, CNN_Reg *gReg_para);
void Cnn_SetConvPoolLrnPara(CNN_LIB_PARA *pPara, CNN_PAPAM cnnPara, UINT32 rois_num, UINT32 gridNum, UINT32 layer_index, NN_IOMEM mem, CNN_Reg *gReg_para);
//void Cnn_SetCombCnnDCPara(NN_DC_PARA *pPara, UINT32 SAI_Addr, UINT32 SAO_Addr, UINT32 workBuff, UINT32 layer_index, NN_DC_Comb_Reg combReg, UINT32 *pGridNum, CNN_Reg *gReg_para);
void CNN_SetSvmTransPara_NODNet(CNN_SVMTRANS_PRMS *pPara, UINT32 SAI0_Addr, UINT32 SAO0_Addr, UINT32 rois_num, UINT32 layer_index, CNN_Reg *gReg_para);
//void Cnn_SetCnnDeconvPara(NN_DECONV_PARA *pPara, NN_IOMEM mem, UINT32 work_Addr, UINT32 layer_index, CNN_Reg *gReg_para, NN_DATA kernel, NN_DATA bias);
void Cnn_SetCnnDeconvPara(NN_DECONV_PARA *pPara, NN_IOMEM mem, UINT32 work_Addr, UINT32 layer_index, CNN_Reg *gReg_para, NN_DATA kernel, NN_DATA bias);
//void Cnn_SetCnnRpnPara(NN_RPN_PARA *pPara, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAO_Addr, UINT32 layer_index, UINT32 *pRois_num, CNN_Reg *gReg_para, NN_DATA RPN_anchor);
void Cnn_SetCnnRpnPara(NN_RPN_PARA *pPara, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAO_Addr, UINT32 layer_index, UINT32 *pRois_num, CNN_Reg *gReg_para, NN_DATA RPN_anchor, UINT32 isLite);
void Cnn_SetNNRpnPara(NN_RPN_PARA *pPara, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAO_Addr, UINT32 layer_index, UINT32 *pRois_num, NN_Reg *gReg_para, NN_DATA RPN_anchor, UINT32 isLite);
void Cnn_SetDspConvPara(NN_DSP_CONV_PARA *pPara, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAI2_Addr, UINT32 SAO_Addr, UINT32 workBuff, UINT32 layer_index, CNN_Reg *gReg_para, NN_DATA conv_bbox_kernel, NN_DATA conv_bbox_bias, NN_DATA conv_cls_kernel, NN_DATA conv_cls_bias, UINT32 wflag);
void Cnn_SetDspConvPara_NODNet(NN_DSP_CONV_PARA *pPara, UINT32 SAI0_Addr, UINT32 SAI1_Addr, UINT32 SAI2_Addr, UINT32 SAO_Addr, UINT32 workBuff, UINT32 layer_index, NN_Reg *gReg_para, NN_DATA conv_bbox_kernel, NN_DATA conv_bbox_bias, NN_DATA conv_cls_kernel, NN_DATA conv_cls_bias, UINT32 wflag);
void CNN_SetRoiPoolPara(NN_ROI_POOL_PARA *pPara, NN_IOMEM mem, UINT32 rois_num, UINT32 layer_index, CNN_Reg *gReg_para);

//void NvtCv_DC_pre(UINT32 uiAddrPrms);
void NvtCv_Conv_pre(UINT32 uiAddrPrms);
void NvtCv_Deconv_pre(UINT32 uiAddrPrms);
void NvtCv_RPN_pre(UINT32 uiAddrPrms);
void NvtCv_ROIpooling_pre(UINT32 uiAddrPrms);
void NvtCv_FastPost_pre(UINT32 uiAddrPrms);
//void NvtCv_Preprocessing_pre(UINT32 uiAddrPrms);
void NvtCv_trans_svm_in(UINT32 uiAddrPrms);

//void CnnToDsp_DC(UINT32 CNN_Used_Start_Address, NN_DC_PARA para);
void CnnToDsp_Conv(UINT32 CNN_Used_Start_Address, NN_DSP_CONV_PARA para, NVTCV_SENDTO sendTo);
void CnnToDsp_Deconv(UINT32 CNN_Used_Start_Address, NN_DECONV_PARA para, NVTCV_SENDTO sendTo);
UINT32 CnnToDsp_RPN(UINT32 CNN_Used_Start_Address, NN_RPN_PARA para, NVTCV_SENDTO sendTo);
void CnnToDsp_ROIpooling(UINT32 CNN_Used_Start_Address, NN_ROI_POOL_PARA para, NVTCV_SENDTO sendTo);
UINT32 CnnToDsp_FastPost(UINT32 CNN_Used_Start_Address, NN_FASTPOST_PARA para, NVTCV_SENDTO sendTo);
void CnnToDsp_svmTransInput(UINT32 CNN_Used_Start_Address, CNN_SVMTRANS_PRMS prms, NVTCV_SENDTO sendTo);
//void CnnToDsp_Preprocessing(UINT32 CNN_Used_Start_Address, NN_PREPROCESS_PARA para);
#endif
