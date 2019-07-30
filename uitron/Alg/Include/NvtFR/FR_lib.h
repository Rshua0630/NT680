/**
    Convolutional Neural Networks library.

    @file       Exam_CNN.h
    @ingroup

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef CN_H
#define CN_H

#include    "Type.h"
#include    "GxImage.h"
#include    "nvtnn_lib.h"
#include    "svm_lib.h"
#include    "fdcnn_lib.h"
#include    "fd_lib.h"


#define CDNN_IPC_PRM_MEM_SZ         0x800
#define FR_STA_OK   1
#define FR_STA_ERR  -1

#define		__CNN_FLOW__       	    1
#define MAX_NUM   5  ///< max obj number in FR
#define MAX_ID_num   100


typedef struct _CNN_Backup_section_FR
{
    UINT32 Addr;
    UINT32 OutSize;
    UINT32 layerIndex;
}CNN_Backup_section_FR;


typedef enum //FR_MODE
{
	NN_CONV = 0,
	NN_POOL,
	NN_LRN,
	DIV_CONV_COM,
	DIV_CONV_COM2,
	DSP_ROI_POOL,
	NN_SVM,
	DSP_DECONV,
	DSP_CONV,
	NN_CONCAT,

	DSP_CONV_RPN = 10,
	NN_BATCHNORM,
	NN_SCALE,
	NN_RELU,
	NN_ELTWISE,
	NN_SOFTMAX,

	/// Add new function index here
	MAX_MODE,
} FR_MODE;
typedef struct _LAYER_IDX_Reg_FR
{
	INT32 layer_index;
	INT32 last_layerIndex[12];
	INT32 pre_layerNum;
	INT32 next_layerNum;
}LAYER_IDX_Reg_FR;

typedef struct _LAYER_CFG_Reg_FR
{
	FR_MODE mode;
	INT32 isDsp;
	INT32 isPara;
	INT32 DC_flag;
	INT32 isIn12to16;
	INT32 isOut16to12;
}LAYER_CFG_Reg_FR;

typedef struct _DEF_BIT_Reg_FR
{
	INT32 in_sign_bit;
	INT32 in_int_bit;
	INT32 in_frac_bit;
	INT32 out_sign_bit;
	INT32 out_int_bit;
	INT32 out_frac_bit;
	INT32 kernel_frac_bit;
	INT32 bias_frac_bit;
}DEF_BIT_Reg_FR;

typedef struct _SIZE_Reg_FR
{
	UINT32 in_width;
	UINT32 in_height;
	UINT32 in_channel;

	UINT32 out_width;
	UINT32 out_height;
	UINT32 out_channel;

	UINT32 in_lofs;
	UINT32 out_lofs;
}SIZE_Reg_FR;

typedef struct _CONV_POOL_Reg_FR
{
	INT32 kerl_size;
	INT32 num_output;
	INT32 pad;
	INT32 stride;
	INT32 kerl_w;
	INT32 kerl_h;
	INT32 pad_w;
	INT32 pad_h;
	INT32 group;
	INT32 conv_shift;

	INT32 conv_shift_B;
	INT32 relu_kerl;
	INT32 pooling_type;
	INT32 global_pooling;
	INT32 bias_term;
}CONV_POOL_Reg_FR;

typedef struct _LRN_Reg_FR
{
	INT32 alpha;
	INT32 beta;
	INT32 n;
	INT32 shift;
}LRN_Reg_FR;

typedef struct _ROI_POOLING_Reg_FR
{
	INT32 pool_w;
	INT32 pool_h;
	float spatial_scale;
}ROI_POOLING_Reg_FR;

typedef struct _ELTWISE_Reg_FR
{
	NVTNN_ELTWISE_OP op;
	INT32 coeff1;
	INT32 coeff2;
	INT32 coeff_q;
}ELTWISE_Reg_FR;

typedef struct _SVM_Reg_FR
{
	INT32 op;
	INT32 coeff1;
	INT32 coeff2;
	INT32 coeff_q;
}SVM_Reg_FR;

typedef struct _DC_Reg_FR
{
	UINT32 isNextLayerPool;
	UINT32 bitTransform;
}DC_DIV_Reg_FR, DC_Comb_Reg_FR;

typedef struct _CNN_Reg_FR
{
	LAYER_IDX_Reg_FR layerIdxReg;
	LAYER_CFG_Reg_FR layerCfg;
	DEF_BIT_Reg_FR defineBit;
	SIZE_Reg_FR sizeReg;
	CONV_POOL_Reg_FR convPoolReg;
	LRN_Reg_FR lrnReg;
	ROI_POOLING_Reg_FR roiPoolReg;
	ELTWISE_Reg_FR eltwiseReg;
	SVM_KERL_PARM svmKerlReg;
	DC_DIV_Reg_FR divReg[2];
	DC_Comb_Reg_FR combReg[2];
}CNN_Reg_FR;

typedef struct _FR_Result
{
    INT16 feature[160];
 
}FR_Result;
typedef struct _FR_Result_fina
{
    FLOAT feature[320];
 
}FR_Result_fina;
typedef struct _FR_Result_fina_160
{
    FLOAT feature[160];
 
}FR_Result_fina_160;


typedef struct _FRCNN_result
{
	INT32 x;
	INT32 y;
	INT32 w;
	INT32 h;
	char name[16];
	UINT32 ID_num;
}FRCNN_result;



#define FLGOD_RSTDONE  FLGPTN_BIT(1)       //0x00000010


void FR_InstallID(void);

#if __CNN_FLOW__
void FR_recognition(IMG_BUF *pInputImg_left);
UINT32 FR_Register(IMG_BUF *pInputImg_left,char *name);


#else

#endif

UINT32 FR_CalcCacheBuffSize(void);

UINT32 FR_CalcBuffSize(void);

UINT32 FR_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 ProcessRate,UINT32 isDSP);

UINT32 FR_LoadModel(UINT32 isDSP);

void FR_GetResult(FR_Result *result);

void FR_NetSelect(UINT32 sel);
UINT32 FR_getrsltobj_for_draw(FRCNN_result *frcnn_result,URECT* pTargetCoord);
//void  FR_config (UINT32  thre,  UINT32  num,UINT32  mode);

//void  FR_getconfig (UINT32  thre,  UINT32  num,UINT32  mode);
void  FR_config (UINT32  thre,  UINT32  num,UINT32  mode,UINT32 DSP_mode);
void  FR_getconfig (UINT32  thre,  UINT32  num,UINT32  mode,UINT32 DSP_mode);

void  FR_reset (void);

void FR_reg_set(UINT32 FR_state);
//void FR_reg_get(UINT32 FR_state);

UINT32 FR_reg_get(void);

#endif

