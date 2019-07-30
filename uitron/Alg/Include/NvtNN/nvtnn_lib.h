/**
    NvtNN library functions.

    Definitions of NvtNN Neural Network functions.

    @file       nvtnn_lib.h
    @ingroup    NvtNN
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _NVTNN_LIB_H
#define _NVTNN_LIB_H

#include "Type.h"
#include "NvtIpcAPI.h"

/**
    Function list.
*/
typedef enum {
	// preprocessing
	NVTNN_PREPROCESS,                       ///< preprocessing
	NVTNN_YUV2RGB,                          ///< YUV-to-RGB color transform
	NVTNN_DIVCOMB,                          ///< divide and combine
	NVTNN_MEANSUB,                          ///< mean subtraction
	NVTNN_FDCNNPREPRO1,                      ///< fdcnn preprocessing

	// vision layers
	NVTNN_CONV,                             ///< convolution
	NVTNN_DECONV,                           ///< deconvolution
	NVTNN_POOL,                             ///< pooling
	NVTNN_LRN,                              ///< LRN (by hardware)
	NVTNN_ROI,                              ///< ROI pooling

	// normalization layers
	NVTNN_BATCHNORM,                        ///< batch normalization

	// activation layers
	NVTNN_RELU,                             ///< ReLU
	NVTNN_SCALE,                            ///< scale

	// utility layers
	NVTNN_RESHAPE,                          ///< reshape
	NVTNN_CONCAT,                           ///< concat
	NVTNN_ELTWISE,                          ///< eltwise
	NVTNN_SOFTMAX,                          ///< softmax
	NVTNN_STRIDE,                           ///< stride sampling (Novatek function)
	NVTNN_SVMTRANS,                         ///< SVM format transformation (Novatek function)
	
	// post-process
	NVTNN_RPNPOST,                           ///< RPN post process
	NVTNN_FRCNNPOST,                        ///< FRCNN post process

	NVTNN_FUN_MAX,
	ENUM_DUMMY4WORD(NVTNN_FUN)
} NVTNN_FUN;

// Common types ----------------------------------------------------------------
/**
    IPC destination core.
*/
typedef enum {
	NVTNN_SENDTO_DSP1 = NVTIPC_SENDTO_CORE3,///< sending the request to DSP1 (core 3)
	NVTNN_SENDTO_DSP2 = NVTIPC_SENDTO_CORE4,///< sending the request to DSP2 (core 4)
	ENUM_DUMMY4WORD(NVTNN_SENDTO)
} NVTNN_SENDTO;

/**
    Return status.
*/
typedef enum {
	NVTNN_STA_OK            = 0,            ///< the status is OK
	NVTNN_STA_ERROR         = -1,           ///< some error occurred
	NVTNN_STA_INVALID_FUN   = -2,           ///< invalid function enum
	ENUM_DUMMY4WORD(NVTNN_STA)
} NVTNN_STA;

/**
    Data dimensions.
*/
typedef struct {
	INT32 num;                              ///< Caffe blob: batch size
	INT32 channels;                         ///< Caffe blob: number of channels
	INT32 height;                           ///< Caffe blob: height
	INT32 width;                            ///< Caffe blob: width
	INT32 lineoffset;                       ///< distance between two lines (rows); unit: byte
} NVTNN_SHAPE;

// Preprocessing ---------------------------------------------------------------
typedef enum {
	NVTNN_BIT_TRANS_12TO16,                 ///< bit transform (12->16 bits)
	NVTNN_BIT_TRANS_16TO12,                 ///< bit transform (16->12 bits)
	ENUM_DUMMY4WORD(NVTNN_BIT_TRANS_MODE)
} NVTNN_BIT_TRANS_MODE;

typedef struct {
	UINT8 trans_input;                      ///< 1: do bit transform (12->16 bits) on input ; 0: not do
	UINT8 trans_output;                     ///< 1: do bit transform (16->12 bits) on output; 0: not do
	INT16 *input_buf;                       ///< address of buffer for bit transform on input
	INT16 *output_buf;                      ///< address of buffer for bit transform on output
} NVTNN_BIT_TRANS_PRMS;

typedef enum {
	NVTNN_PREPROC_OUT_SQUARE,               ///< output height is equal to output width
	NVTNN_PREPROC_OUT_RECT,                 ///< output height is not equal to output width
	ENUM_DUMMY4WORD(NVTNN_PREPROC_OUT_TYPE)
} NVTNN_PREPROC_OUT_TYPE;

typedef struct {
	UINT8 *input_ch1;                       ///< address of input channel 1
	UINT8 *input_ch2;                       ///< address of input channel 2
	UINT8 *input_ch3;                       ///< address of input channel 3
	UINT8 *output_ch1;                      ///< address of output channel 1
	UINT8 *output_ch2;                      ///< address of output channel 2
	UINT8 *output_ch3;                      ///< address of output channel 3
	INT16 *input_mean;                      ///< address of input means (set null if ch_mean is set)
	INT16 *ch_mean;                         ///< address of channel means (set null if input_mean is set)

	NVTNN_SHAPE input_shape;                ///< input data dimensions
	NVTNN_SHAPE mean_shape;                 ///< input mean dimensions
	INT32 target_shape;                     ///< target output dimension

	NVTNN_BIT_TRANS_MODE bit_trans_mode;    ///< bit transform mode
	UINT8 is_img_input;                     ///< 1: input data is image data; 0: not

	UINT8 scale_q;                          ///< number of fraction bits of scaling
	UINT32 *scale_ratio;                    ///< address of ratio of scaling (output)

	INT32 pad;                              ///< padding size
	UINT8 yuv_trans;                        ///< 1: YUV-to-RGB transform; 0: no transform
	NVTNN_PREPROC_OUT_TYPE output_type;     ///< output type
} NVTNN_PREPROCESS_PRMS;

// Resize ----------------------------------------------------------------------
typedef struct {
	NVTNN_SHAPE input_shape;                ///< input data dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output data dimensions (lineoffset is not used)
	INT32 small_side;                       ///< target size of small side of output rectangle
	INT32 large_side;                       ///< target size of large side of output rectangle
} NVTNN_GET_RESIZE_SHAPE_PRMS;

typedef enum {
	NVTNN_RESIZE_BY_SIZE,                   ///< resize using output shape
	NVTNN_RESIZE_BY_RATIO,                  ///< resize using resize ratio
	ENUM_DUMMY4WORD(NVTNN_RESIZE_MODE)
} NVTNN_RESIZE_MODE;

typedef struct {
	UINT8 *input_ch1;                       ///< address of input channel 1
	UINT8 *input_ch2;                       ///< address of input channel 2
	UINT8 *input_ch3;                       ///< address of input channel 3
	UINT8 *output_ch1;                      ///< address of output channel 1
	UINT8 *output_ch2;                      ///< address of output channel 2
	UINT8 *output_ch3;                      ///< address of output channel 3

	NVTNN_SHAPE input_shape;                ///< input data dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output data dimensions (lineoffset is not used)

	NVTNN_RESIZE_MODE mode;                 ///< resizing mode
	INT32 resize_ratio;                     ///< resizing ratio
	UINT8 do_transpose;                     ///< 1: do transpose first; 0: not do
} NVTNN_RESIZE_PRMS;

// YUV-to-RGB color transformation ---------------------------------------------
typedef struct {
	UINT8 *input_y;                         ///< address of input Y channel
	UINT8 *input_uv;                        ///< address of input U/V channel
	INT16 *output_r;                        ///< address of output R channel
	INT16 *output_g;                        ///< address of output G channel
	INT16 *output_b;                        ///< address of output B channel

	NVTNN_SHAPE input_shape;                ///< input data dimensions (lineoffset is not used)

	UINT8 reshape;                          ///< 1: reshape and color transform; 0: color transform only
} NVTNN_YUV2RGB_PRMS;

// Divide and combine ----------------------------------------------------------
typedef enum {
	NVTNN_DIV,                              ///< divide
	NVTNN_COMB,                             ///< combine
	ENUM_DUMMY4WORD(NVTNN_DIVCOMB_OP)
} NVTNN_DIVCOMB_OP;

typedef struct {
	UINT8 *input;                           ///< address of input features
	UINT8 *output;                          ///< address of output features

	NVTNN_SHAPE input_shape;                ///< input feature dimensions (lineoffset is not used)

	INT32 effect_h;                         ///< effect output height of combining
	INT32 effect_w;                         ///< effect output width of combining
	INT32 grid_h;                           ///< grid height
	INT32 grid_w;                           ///< grid width
	INT32 max_input;                        ///< maximal size of input features

	INT32 block_size;                       ///< block size
	INT32 block_step;                       ///< block step

	INT32 pad;                              ///< padding size
	INT32 m;                                ///< block border size
	INT32 add_pad;                          ///< additional padding size

	UINT8 next_layer_pool;                  ///< 1: the next layer is pooling layer; 0: is not
	NVTNN_DIVCOMB_OP op;                    ///< dividing or combining operation
} NVTNN_DIVCOMB_PRMS;

// Mean Subtraction ------------------------------------------------------------
typedef struct {
	INT16 *input_ch1;                       ///< address of input channel 1
	INT16 *input_ch2;                       ///< address of input channel 2
	INT16 *input_ch3;                       ///< address of input channel 3
	INT16 *output_ch1;                      ///< address of output channel 1
	INT16 *output_ch2;                      ///< address of output channel 2
	INT16 *output_ch3;                      ///< address of output channel 3
	INT16 *input_mean;                      ///< address of input means

	NVTNN_SHAPE shape;                      ///< input/output data dimensions (lineoffset is not used)
} NVTNN_MEANSUB_PRMS;

// Convolution -----------------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input features
	INT32 input_size;                       ///< length of input features; unit: byte
	INT16 *output;                          ///< address of output features
	INT32 output_size;                      ///< length of output features; unit: byte
	INT16 *kernel;                          ///< address of kernel weights
	INT32 kernel_size;                      ///< length of kernel weights; unit: byte
	INT32 *bias;                            ///< address of kernel biases
	INT32 bias_size;                        ///< length of kernel biases; unit: byte
	INT16 *input_buf;                       ///< (optional) address of buffer for padding input (required if pad_h > 0 or pad_w > 0)

	NVTNN_SHAPE input_shape;                ///< input feature dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output feature dimensions (lineoffset is not used)

	INT32 pad_h;                            ///< padding height
	INT32 pad_w;                            ///< padding width
	INT32 kernel_h;                         ///< kernel height
	INT32 kernel_w;                         ///< kernel width
	INT32 stride_h;                         ///< stride height
	INT32 stride_w;                         ///< stride width
	INT32 group;                            ///< number of groups for group convolution; support 1 only

	UINT8 input_q;                          ///< number of fraction bits of input features
	UINT8 output_q;                         ///< number of fraction bits of output features
	UINT8 kernel_q;                         ///< number of fraction bits of kernel weights
	UINT8 bias_q;                           ///< number of fraction bits of kernel biases

	UINT8 relu;                             ///< 1: do ReLU on output; 0: not do
	UINT8 reshape;                          ///< 1: do reshape on output (order: C, W, H, N); 0: not do
} NVTNN_CONV_PRMS;

// Deconvolution -----------------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input features
	INT16 *output;                          ///< address of output features
	INT16 *kernel;                          ///< address of kernel weights
	INT32 *bias;                            ///< address of kernel biases
	INT16 *input_buf;                       ///< (optional) address of buffer for padding input (required if pad_h > 0 or pad_w > 0)

	NVTNN_SHAPE input_shape;                ///< input feature dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output feature dimensions (lineoffset is not used)

	INT32 pad_h;                            ///< padding height
	INT32 pad_w;                            ///< padding width
	INT32 kernel_h;                         ///< kernel height
	INT32 kernel_w;                         ///< kernel width
	INT32 stride_h;                         ///< stride height
	INT32 stride_w;                         ///< stride width

	UINT8 input_q;                          ///< number of fraction bits of input features
	UINT8 output_q;                         ///< number of fraction bits of output features
	UINT8 kernel_q;                         ///< number of fraction bits of kernel weights
	UINT8 bias_q;                           ///< number of fraction bits of kernel biases

	UINT8 relu;                             ///< 1: do ReLU on output; 0: not do
} NVTNN_DECONV_PRMS;

// Pooling ---------------------------------------------------------------------
typedef enum {
	NVTNN_POOL_MAX,                         ///< maximum method
	NVTNN_POOL_AVE,                         ///< average method
	ENUM_DUMMY4WORD(NVTNN_POOL_OP)
} NVTNN_POOL_OP;

typedef struct {
	INT16 *input;                           ///< address of input features
	INT16 *output;                          ///< address of output features

	NVTNN_SHAPE input_shape;                ///< input feature dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output feature dimensions (lineoffset is not used)

	INT32 kernel_size;                      ///< kernel size
	INT32 stride;                           ///< stride size
	INT32 pad;                              ///< padding size

	UINT8 global_pooling;                   ///< 1: do global pooling; 0: not do
	NVTNN_POOL_OP op;                       ///< pooling method
} NVTNN_POOL_PRMS;

// Batch Normalization ---------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input features
	INT16 *output;                          ///< address of output features
	INT16 *mean;                            ///< address of input means
	UINT16 *inv_std;                        ///< address of input standard-deviation inverse (1 / std)

	NVTNN_SHAPE input_shape;                ///< input feature dimensions
	NVTNN_SHAPE output_shape;               ///< output feature dimensions

	INT8 input_mean_q;                      ///< number of fraction bits of input features and means; the Qm.n of input is same as that of mean; Qm.n format: reference http://blog.csdn.net/nizhenyuchina/article/details/50382649
	INT8 inv_std_q;                         ///< number of fraction bits of input standard-deviation inverse
	INT8 output_q;                          ///< number of fraction bits of output features

	INT32 stride;                           ///< sampling step
} NVTNN_BATCHNORM_PRMS;

// ReLU ------------------------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input data
	INT16 *output;                          ///< address of output data

	NVTNN_SHAPE shape;                      ///< input/output data dimensions (lineoffset is not used)
} NVTNN_RELU_PRMS;

// Scale -----------------------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input data
	INT16 *output;                          ///< address of output data
	INT16 *scale;                           ///< address of scaling data
	INT32 *bias;                            ///< address of bias data

	NVTNN_SHAPE shape;                      ///< input/output data dimensions (lineoffset is not used)

	INT16 scale0;                           ///< scaling factor (used if scale is null)
	INT32 bias0;                            ///< bias (used if bias is null)
	UINT8 output_shift;                     ///< number of shift bits of output data
} NVTNN_SCALE_PRMS;

// Reshape ---------------------------------------------------------------------
typedef struct {
	NVTNN_SHAPE input_shape;                ///< input data dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output data dimensions (lineoffset is not used)
} NVTNN_RESHAPE_PRMS;

// Concat ----------------------------------------------------------------------
typedef struct {
	INT16 **input;                          ///< addresses of input data
	INT16 *output;                          ///< address of output data

	NVTNN_SHAPE *input_shape;               ///< array of input data dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output data dimensions (lineoffset is not used)

	INT32 input_num;                        ///< number of input
	INT32 concat_dim;                       ///< 0: concatenating by num; 1: concatenating by channels

	UINT8 *trans_input;                     ///< array of size input_num; for element<i>, 1: do bit transform (12->16) on input<i>; 0: not do
	UINT8 trans_output;                     ///< 1: do bit transform (16->12) on output; 0: not do
	INT16 **input_buf;                      ///< addresses of buffers for bit transform on input array
	INT16 *output_buf;                      ///< address of buffer for bit transform on output
} NVTNN_CONCAT_PRMS;

// Eltwise ---------------------------------------------------------------------
typedef enum {
    NVTNN_ELTWISE_PROD,                     ///< product operation
	NVTNN_ELTWISE_SUM,                      ///< sum operation
	NVTNN_ELTWISE_MAX,                      ///< maximum operation
	ENUM_DUMMY4WORD(NVTNN_ELTWISE_OP)
} NVTNN_ELTWISE_OP;

typedef struct {
	INT16 *input1;                          ///< address of first input data
	INT16 *input2;                          ///< address of second input data
	INT16 *output;                          ///< address of output data (may be address of input1 or input2)

	NVTNN_SHAPE shape;                      ///< input/output data dimensions (lineoffset is not used)

	INT16 coeff1;                           ///< first coefficient of sum operation
	INT16 coeff2;                           ///< second coefficient of sum operation
	UINT8 input1_q;                         ///< number of fraction bits of first input data
	UINT8 input2_q;                         ///< number of fraction bits of second input data
	UINT8 output_q;                         ///< number of fraction bits of output data
	UINT8 coeff_q;                          ///< number of fraction bits of coeff1/coeff2

	UINT8 trans_input1;                     ///< 1: do bit transform (12->16) on input1; 0: not do
	UINT8 trans_input2;                     ///< 1: do bit transform (12->16) on input2; 0: not do
	UINT8 trans_output;                     ///< 1: do bit transform (16->12) on output; 0: not do
	INT16 *input1_buf;                      ///< address of buffer for bit transform on input1
	INT16 *input2_buf;                      ///< address of buffer for bit transform on input2
	INT16 *output_buf;                      ///< address of buffer for bit transform on output

	NVTNN_ELTWISE_OP op;                    ///< element-wise operation
} NVTNN_ELTWISE_PRMS;

// Softmax ---------------------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input data
	INT16 *output;                          ///< address of output data

	NVTNN_SHAPE shape;                      ///< input/output data dimensions (lineoffset is not used)

	UINT8 input_shift;                      ///< number of shift bits of input data
	UINT8 output_shift;                     ///< number of shift bits of output data
} NVTNN_SOFTMAX_PRMS;

// Stride sampling -------------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input data
	INT16 *output;                          ///< address of output data (may be address of input)

	NVTNN_SHAPE input_shape;                ///< input data dimensions (lineoffset is not used)
	NVTNN_SHAPE output_shape;               ///< output data dimensions (lineoffset is not used)

	INT32 stride;                           ///< sampling step
} NVTNN_STRIDE_PRMS;

// SVM format transformation ---------------------------------------------------
typedef struct {
	UINT8 *input;                           ///< address of input data
	UINT8 *output;                          ///< address of output data

	INT32 input_size;                       ///< length of input data; unit: byte
	INT32 input_channels;                   ///< number of channels
	INT32 input_lineoffset;                 ///< distance between two lines (rows); unit: byte
} NVTNN_SVMTRANS_PRMS;

// Padding ---------------------------------------------------------------------
typedef struct {
	INT16 *input;                           ///< address of input data
	INT16 *output;                          ///< address of output data

	NVTNN_SHAPE shape;                      ///< input/output data dimensions

	INT32 pad_h;                            ///< padding height
	INT32 pad_w;                            ///< padding width
} NVTNN_PAD_PRMS;

// RPN post-process -------------------------------------------------------------
typedef struct {
	INT16 *input_box_delta;                ///< address of input box delta
	INT16 *input_score;                    ///< address of input score
	INT16 *input_box;                      ///< address of input box
	INT16 *output;                         ///< address of output box
	INT16 *work_buf;                       ///< address of working buff

	INT32 img_height;                      ///< img height
	INT32 img_width;                       ///< img width
	INT32 input_box_num;	               ///< input box number

	INT32 per_nms_top_n;	               ///< the max number of input box for nms
	INT32 min_score;                       ///< threshold for filtering boxes
	INT32 nms_overlap_thres;               ///< threshold for doing nms
	INT32 max_output_box_num;              ///< the max number of output box

	INT32 *output_box_num;                 ///< address of output number
} NVTNN_RPN_POSTPROC_PRMS;

//ROIPOOLING process ----------------------------------------------------------
typedef struct {
	INT16 *input;							///< address of input data
	INT16 *rois;							///< address of rois data
	INT16 *output;							///< address of output data

	NVTNN_SHAPE input_shape;                ///< input data dimensions (lineoffset is not used)

    UINT32 rois_num;
    UINT32 pool_h;
    UINT32 pool_w;
    UINT32 spatial_scale_fix_pnt;
    UINT32 spatial_scale_q;
    INT32 *output_byte_size;
} NVTNN_ROI_PRMS;

typedef struct {
	INT16 *input_box_delta;                ///< address of input box delta
	INT16 *input_score;                    ///< address of input score
	INT16 *input_box;                      ///< address of input box
	INT16 *output;                         ///< address of output data
	INT16 *work_buf;                       ///< address of working buff

	INT32 img_height;                      ///< img height
	INT32 img_width;                       ///< img width
	INT32 input_box_num;                   ///< input box number
	INT32 input_class_num;                 ///< input class number

	INT32 per_class_top_n;                 ///< the max number of each class
	INT32 *map_class_thres;	               ///< map threshold for each class
	INT32 per_nms_top_n;	               ///< the max number of input box for nms
	INT32 nms_overlap_thres;               ///< threshold for doing nms

	INT32 *output_box_num;	               ///< address of output number
	INT32 scale;                           ///< scale of output box
	INT32 det_score_thres;                 ///< minimum detection score
} NVTNN_FRCNN_POSTPROC_PRMS;

// Functions -------------------------------------------------------------------
/**
    Install required system resource.
*/
extern VOID nvtnn_install_id(VOID) _SECTION(".kercfg_text");

// Preprocessing
extern NVTNN_STA nvtnn_preproc_process          (NVTNN_PREPROCESS_PRMS       *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_preproc_process_nowait   (NVTNN_PREPROCESS_PRMS       *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_preproc_waitdone         (NVTNN_PREPROCESS_PRMS       *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_yuv2rgb_process          (NVTNN_YUV2RGB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_yuv2rgb_process_nowait   (NVTNN_YUV2RGB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_yuv2rgb_waitdone         (NVTNN_YUV2RGB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_divcomb_process          (NVTNN_DIVCOMB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_divcomb_process_nowait   (NVTNN_DIVCOMB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_divcomb_waitdone         (NVTNN_DIVCOMB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_meansub_process          (NVTNN_MEANSUB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_meansub_process_nowait   (NVTNN_MEANSUB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_meansub_waitdone         (NVTNN_MEANSUB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_get_resize_shape         (NVTNN_GET_RESIZE_SHAPE_PRMS *prms);
extern NVTNN_STA nvtnn_resize                   (NVTNN_RESIZE_PRMS           *prms);
extern NVTNN_STA nvtnn_fdcnnprepro1_process          (NVTNN_YUV2RGB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_fdcnnprepro1_process_nowait   (NVTNN_YUV2RGB_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_fdcnnprepro1_waitdone         (NVTNN_YUV2RGB_PRMS          *prms, NVTNN_SENDTO dsp_core);


// Vision layers
extern NVTNN_STA nvtnn_conv_process             (NVTNN_CONV_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_conv_process_nowait      (NVTNN_CONV_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_conv_waitdone            (NVTNN_CONV_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_deconv_process			    (NVTNN_DECONV_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_deconv_process_nowait		(NVTNN_DECONV_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_deconv_waitdone				(NVTNN_DECONV_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_pool_process             (NVTNN_POOL_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_pool_process_nowait      (NVTNN_POOL_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_pool_waitdone            (NVTNN_POOL_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_roi_process			    (NVTNN_ROI_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_roi_process_nowait		(NVTNN_ROI_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_roi_waitdone				(NVTNN_ROI_PRMS             *prms, NVTNN_SENDTO dsp_core);

// Normalization layers
extern NVTNN_STA nvtnn_batchnorm_process        (NVTNN_BATCHNORM_PRMS        *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_batchnorm_process_nowait (NVTNN_BATCHNORM_PRMS        *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_batchnorm_waitdone       (NVTNN_BATCHNORM_PRMS        *prms, NVTNN_SENDTO dsp_core);

// Activation layers
extern NVTNN_STA nvtnn_relu_process             (NVTNN_RELU_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_relu_process_nowait      (NVTNN_RELU_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_relu_waitdone            (NVTNN_RELU_PRMS             *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_scale_process            (NVTNN_SCALE_PRMS            *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_scale_process_nowait     (NVTNN_SCALE_PRMS            *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_scale_waitdone           (NVTNN_SCALE_PRMS            *prms, NVTNN_SENDTO dsp_core);

// Utility layers
extern NVTNN_STA nvtnn_reshape_process          (NVTNN_RESHAPE_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_reshape_process_nowait   (NVTNN_RESHAPE_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_reshape_waitdone         (NVTNN_RESHAPE_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_concat_process           (NVTNN_CONCAT_PRMS           *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_concat_process_nowait    (NVTNN_CONCAT_PRMS           *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_concat_waitdone          (NVTNN_CONCAT_PRMS           *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_eltwise_process          (NVTNN_ELTWISE_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_eltwise_process_nowait   (NVTNN_ELTWISE_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_eltwise_waitdone         (NVTNN_ELTWISE_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_softmax_process          (NVTNN_SOFTMAX_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_softmax_process_nowait   (NVTNN_SOFTMAX_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_softmax_waitdone         (NVTNN_SOFTMAX_PRMS          *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_stride_process           (NVTNN_STRIDE_PRMS           *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_stride_process_nowait    (NVTNN_STRIDE_PRMS           *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_stride_waitdone          (NVTNN_STRIDE_PRMS           *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_svmtrans_process         (NVTNN_SVMTRANS_PRMS         *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_svmtrans_process_nowait  (NVTNN_SVMTRANS_PRMS         *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_svmtrans_waitdone        (NVTNN_SVMTRANS_PRMS         *prms, NVTNN_SENDTO dsp_core);

// Bit depth transform setting
extern NVTNN_STA nvtnn_set_bit_trans_prms       (NVTNN_BIT_TRANS_PRMS        *prms, NVTNN_SENDTO dsp_core);

// Utility functions
extern NVTNN_STA nvtnn_pad                      (NVTNN_PAD_PRMS              *prms);

// post process
extern NVTNN_STA nvtnn_rpn_postproc_process         (NVTNN_RPN_POSTPROC_PRMS *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_rpn_postproc_process_nowait  (NVTNN_RPN_POSTPROC_PRMS *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_rpn_postproc_waitdone        (NVTNN_RPN_POSTPROC_PRMS *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_frcnn_postproc_process         (NVTNN_FRCNN_POSTPROC_PRMS *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_frcnn_postproc_process_nowait  (NVTNN_FRCNN_POSTPROC_PRMS *prms, NVTNN_SENDTO dsp_core);
extern NVTNN_STA nvtnn_frcnn_postproc_waitdone        (NVTNN_FRCNN_POSTPROC_PRMS *prms, NVTNN_SENDTO dsp_core);


#endif  //_NVTNN_LIB_H
