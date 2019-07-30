/**
    NVT CDNN library.

    Library of NVT CDNN (CEVA Deep Neural Network) functions.

    @file       NvtCDNN.h
    @ingroup    NvtCDNN

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _NVTCDNN_H
#define _NVTCDNN_H

// Include ---------------------------------------------------------------------

#include "Type.h"
#include "GxImage.h"
#include "NvtIpcAPI.h"

// Definitions -----------------------------------------------------------------

#define CDNN_INPUT_FMT          GX_IMAGE_PIXEL_FMT_YUV422_PACKED    ///< input format

#define CDNN_MAX_FILE_IN_WIDTH  1280    ///< maximal file input width
#define CDNN_MAX_FILE_IN_HEIGHT 720     ///< maximal file input height

#define CDNN_OUTPUT_LBL_LEN     256     ///< maximal length of output class label
#define CDNN_OUTPUT_CLASS_NUM   5       ///< maximal number of output classes

// Enumeration -----------------------------------------------------------------

/**
    Network list.
*/
typedef enum {
	CDNN_GOOGLENET,                     ///< GoogLeNet
	CDNN_FRCNN_RESNET50,                ///< Faster-RCNN-ResNet-50
	CDNN_MOBILENET_SSD,                 ///< MobileNet-SSD

	CDNN_NET_NUM,                       ///< number of networks
	ENUM_DUMMY4WORD(CDNN_NET)
} CDNN_NET;

/**
    Input source.
*/
typedef enum {
	CDNN_SENSOR_INPUT,                  ///< input from sensor
	CDNN_FILE_INPUT,                    ///< input from file
	ENUM_DUMMY4WORD(CDNN_INPUT_SRC)
} CDNN_INPUT_SRC;

/**
    DSP core to run CDNN.
*/
typedef enum {
	CDNN_DSP1 = NVTIPC_SENDTO_CORE3,    ///< run with DSP1 (core 3)
	CDNN_DSP2 = NVTIPC_SENDTO_CORE4,    ///< run with DSP2 (core 4)
	ENUM_DUMMY4WORD(CDNN_DSP)
} CDNN_DSP;

/**
    DRAM for buffers.
*/
typedef enum {
	CDNN_DRAM1 = 1,                     ///< DRAM1
	CDNN_DRAM2 = 2,                     ///< DRAM2
	ENUM_DUMMY4WORD(CDNN_DRAM)
} CDNN_DRAM;

/**
    Return status.
*/
typedef enum {
	CDNN_STA_OK             = 0,        ///< the status is OK
	CDNN_STA_ERR            = -1,       ///< some error occurred
	CDNN_STA_INVALID_NET    = -2,       ///< invalid network enum
	ENUM_DUMMY4WORD(CDNN_STA)
} CDNN_STA;

// Structures ------------------------------------------------------------------

/**
    Parameters of CDNN creation.
*/
typedef struct {
	CDNN_DSP dsp;                       ///< target DSP
	CDNN_INPUT_SRC input_src;           ///< input source
	UINT8 use_dram2;                    ///< 1: use DRAM2; 0: not
} CDNN_CREATE_PRMS;

/**
    Parameters of getting buffer size.
*/
typedef struct {
	CDNN_DSP dsp;                       ///< target DSP
	CDNN_NET net;                       ///< network
	CDNN_DRAM dram;                     ///< DRAM
} CDNN_BUF_PRMS;

/**
    Parameters of CDNN initialization.
*/
typedef struct {
	CDNN_DSP dsp;                       ///< target DSP
	CDNN_NET net;                       ///< network
	CHAR *filename;                     ///< network model file name
	CHAR *input2_filename;              ///< input2 file name (for FRCNN)
	UINT32 width;                       ///< input width
	UINT32 height;                      ///< input height
	MEM_RANGE *buf;                     ///< memory range of working buffer in DRAM1
	MEM_RANGE *buf2;                    ///< memory range of working buffer in DRAM2
	MEM_RANGE *cache_buf;               ///< memory range of working cache buffer
} CDNN_INIT_PRMS;

/**
    Parameters of sensor input processing.
*/
typedef struct {
	CDNN_DSP dsp;                       ///< target DSP
	IMG_BUF *data;                      ///< input data
} CDNN_SENSOR_PRMS;

/**
    Parameters of file input processing.
*/
typedef struct {
	CDNN_DSP dsp;                       ///< target DSP
	CHAR *filename;                     ///< input file name
} CDNN_FILE_PRMS;

/**
    Output class.
*/
typedef struct {
	CHAR label[CDNN_OUTPUT_LBL_LEN];    ///< class label
	INT32 score;                        ///< class score
} CDNN_OUTPUT_CLASS;

/**
    Output class list.
*/
typedef struct {
	UINT32 class_num;                   ///< number of classes
	CDNN_OUTPUT_CLASS classes[CDNN_OUTPUT_CLASS_NUM];   ///< class list
} CDNN_OUTPUT_CLASSES;

/**
    Output of Faster RCNN.
*/
typedef struct {
	UINT32 proposal_score_addr;         ///< address of proposal scores
	UINT32 bbox_addr;                   ///< address of bounding boxes
	UINT32 cls_addr;                    ///< address of classes
	UINT32 proposal_rois_addr;          ///< address of proposal ROI's
	UINT32 output_size;                 ///< output size of classes
	UINT32 proposal_rois_output_size;   ///< output size of proposal ROI's
} CDNN_OUTPUT_FRCNN;

/**
    Output of MobileNet-SSD.
*/
typedef struct {
	UINT32 class_id_addr;               ///< class_id per roi
	UINT32 class_id_size;               ///< buffer size
	UINT32 prob_roi_info_addr;          ///< probability, x_min, y_min, x_max, y_max per roi
	UINT32 prob_roi_info_size;          ///< buffer size
} CDNN_OUTPUT_MOBILENET_SSD;

// Functions -------------------------------------------------------------------

/**
    Install internal semaphore ID.
*/
extern VOID cdnn_install_id(VOID) _SECTION(".kercfg_text");

/**
    Create CDNN.

    @param[in] prms: creation parameters.

    @return the result status.
*/
extern CDNN_STA cdnn_create(CDNN_CREATE_PRMS *prms);

/**
    Get the required working buffer size.

    @param[in] prms: buffer parameters.

    @return the required working buffer size.
*/
extern UINT32 cdnn_get_buf_size(CDNN_BUF_PRMS *prms);

/**
    Get the required working cache buffer size.

    @param[in] prms: buffer parameters.

    @return the required working cache buffer size.
*/
extern UINT32 cdnn_get_cache_buf_size(CDNN_BUF_PRMS *prms);

/**
    Initialize CDNN.

    @param[in] prms: initialization parameters.

    @return the result status.
*/
extern CDNN_STA cdnn_init(CDNN_INIT_PRMS *prms);

/**
    Process sensor input data.

    @param[in] prms: input parameters.

    @return the result status.
*/
extern CDNN_STA cdnn_proc_sensor(CDNN_SENSOR_PRMS *prms);

/**
    Process sensor input data. Input size is smaller than the size in cdnn_init().

    @param[in] prms: input parameters.
    @param[in] in_width: input width.
    @param[in] in_height: input height.

    @return the result status.
*/
extern CDNN_STA cdnn_proc_sensor_varsize(CDNN_SENSOR_PRMS *prms, UINT32 in_width, UINT32 in_height);

/**
    Process file input data.

    @param[in] prms: input parameters.

    @return the result status.
*/
extern CDNN_STA cdnn_proc_file(CDNN_FILE_PRMS *prms);

/**
    Process file input data. Input size is smaller than the size in cdnn_init().

    @param[in] prms: input parameters.
    @param[in] in_width: input width.
    @param[in] in_height: input height.

    @return the result status.
*/
extern CDNN_STA cdnn_proc_file_varsize(CDNN_FILE_PRMS *prms, UINT32 in_width, UINT32 in_height);

/**
    Get output data.

    @param[in] dsp: target DSP.

    @return pointer of the output data.
*/
extern VOID *cdnn_get_output(CDNN_DSP dsp);

/**
    Finalize CDNN.

    @param[in] dsp: target DSP.

    @return the result status.
*/
extern CDNN_STA cdnn_final(CDNN_DSP dsp);

#endif  //_NVTCDNN_H
