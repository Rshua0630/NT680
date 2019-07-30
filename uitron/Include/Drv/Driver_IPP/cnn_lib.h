/**
    Public header file for CNN module.

    @file       cnn_lib.h
    @ingroup    mIDrvIPP_CNN

    @brief

    How to start:\n
    [Movie]\n
    1. cnn_open\n
    2. cnn_setMode\n
    3. cnn_start\n


    How to stop:\n
    1. cnn_pause\n
    2. cnn_close\n

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _CNN_LIB_H
#define _CNN_LIB_H

#include "Type.h"
/**
    @addtogroup mIDrvIPP_CNN
*/
//@{

/*
    Define CNN interrput enable.
*/
//@{
#define CNN_INT_FRM                    0x00000001    ///< Issue once when a frame is done
#define CNN_INT_CONV                   0x00000002    ///< Issue once when convolution is done
#define CNN_INT_RELU                   0x00000004    ///< Issue once when ReLU is done
#define CNN_INT_POOL                   0x00000008    ///< Issue once when Pooling is done
#define CNN_INT_LRN                    0x00000010    ///< Issue once when LRN is done
#define CNN_INT_ALL                    0x0000001F
//@}

// Structure definition
/**
    CNN Engine Operation Selection.

    Select CNN module operation.
*/
//@{
typedef enum {
	CONV_RELU         = 0,   ///< convolution and Rectified linear unit
	POOL              = 1,   ///< pooling
	LRN               = 2,   ///< local response normalization
	ENUM_DUMMY4WORD(CNN_MODESEL)
} CNN_MODESEL;
//@}

/**
    CNN convolution kernel size.

    Select CNN convolution kernel size.
*/
//@{
typedef enum {
	CONV_K11X11         = 0,   ///< kernel 11x11
	CONV_K5X5           = 1,   ///< kernel 5x5
	CONV_K3X3           = 2,   ///< kernel 3x3
	ENUM_DUMMY4WORD(CNN_CONV_KERNEL)
} CNN_CONV_KERNEL;
//@}

/**
    CNN convolution stride for kernel 11x11.

    Select CNN convolution stride for kernel 11x11.
*/
//@{
typedef enum {
	CONV_K11_STRIDE2         = 0,   ///< stride = 2
	CONV_K11_STRIDE4         = 1,   ///< stride = 4
	ENUM_DUMMY4WORD(CNN_CONVK11_STRIDE)
} CNN_CONVK11_STRIDE;
//@}

/**
    CNN convolution stride for kernel 5x5 or 3x3.

    Select CNN convolution stride for kernel 5x5 or 3x3.
*/
//@{
typedef enum {
	CONV_K5N3_STRIDE1         = 0,   ///< stride = 1
	CONV_K5N3_STRIDE2         = 1,   ///< stride = 2
	ENUM_DUMMY4WORD(CNN_CONVK5N3_STRIDE)
} CNN_CONVK5N3_STRIDE;
//@}

/**
    CNN ReLU kernel.

    Select CNN ReLU kernel.
*/
//@{
typedef enum {
	RELU_KER_MAX         = 0,   ///< kernel = max(0,x)
	RELU_KER_ABS         = 1,   ///< kernel = absolute value
	ENUM_DUMMY4WORD(CNN_RELU_KERNEL)
} CNN_RELU_KERNEL;
//@}

/**
    CNN Pooling kernel.

    Select CNN Pooling kernel.
*/
//@{
typedef enum {
	POOL_KER_MAX         = 0,   ///< kernel = max(0,x)
	POOL_KER_ABS         = 1,   ///< kernel = absolute value
	ENUM_DUMMY4WORD(CNN_POOL_KERNEL)
} CNN_POOL_KERNEL;
//@}

/**
    CNN Pooling kernel size.

    Select CNN Pooling kernel size.
*/
//@{
typedef enum {
	POOL_KERSZ_3X3         = 0,   ///< kernel size 3x3
	POOL_KERSZ_5X5         = 1,   ///< kernel size 5x5
	POOL_KERSZ_7X7         = 2,   ///< kernel size 7x7
	ENUM_DUMMY4WORD(CNN_POOL_KERSIZE)
} CNN_POOL_KERSIZE;
//@}

/**
    CNN LRN channel number.

    Select CNN LRN channel number.
*/
//@{
typedef enum {
	LRN_NCH_3         = 0,   ///< channel number N = 3
	LRN_NCH_5         = 1,   ///< channel number N = 5
	ENUM_DUMMY4WORD(CNN_LRN_NCH)
} CNN_LRN_NCH;
//@}



/**
    CNN Open Object.

    Open Object is used for opening CNN module.
    \n Used for cnn_open()

*/
//@{
typedef struct _CNN_OPENOBJ {
	void (*FP_CNNISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 uiCnnClockSel;                     ///< CNN clock selection
} CNN_OPENOBJ;
//@}

/**
    CNN Convolution Parameters.

    This is used for concolution configuration.

*/
//@{
typedef struct _CNN_CONV_PAPAM {
	CNN_CONV_KERNEL kernel;                 ///< convolution kernel size
	CNN_CONVK11_STRIDE stride_k11;          ///< stride of kernel 11x11
	CNN_CONVK5N3_STRIDE stride_k5n3;        ///< stride of kernel 5x5 or 3x3
	UINT32 uiConvSetNum;                    ///< convolution set number
	UINT32 uiConvShift;                     ///< convolution right shift
	UINT32 uiConvShiftB;                    ///< convolution right shift B
	UINT32 uiParaAddr;                      ///< dram addr of convolution parameters
} CNN_CONV_PAPAM;
//@}

/**
    CNN ReLU Parameters.

    This is used for configuration of Rectified linear unit.

*/
//@{
typedef struct _CNN_RELU_PAPAM {
	CNN_RELU_KERNEL kernel;                 ///< ReLU kernel type
} CNN_RELU_PAPAM;
//@}

/**
    CNN Pooling Parameters.

    This is used for configuration of pooling.

*/
//@{
typedef struct _CNN_POOL_PAPAM {
	CNN_POOL_KERNEL kernelType;             ///< Pooling kernel type
	UINT32 uiPoolStride;                    ///< Pooling stride
	CNN_POOL_KERSIZE kerSize;               ///< Pooling kernel size
} CNN_POOL_PAPAM;
//@}

/**
    CNN LRN Parameters.

    This is used for configuration of local response normalization.

*/
//@{
typedef struct _CNN_LRN_PAPAM {
	UINT32 uiAlpha;                     ///< alpha
	UINT32 uiBeta;                      ///< beta
	CNN_LRN_NCH chN;                    ///< channel number N
	UINT32 uiLRNShift;                  ///< LRN left shift
} CNN_LRN_PAPAM;
//@}

/**
    CNN structure - CNN input size info.
*/
//@{
typedef struct {
	UINT32 uiCnnWidth;              ///< CNN input width
	UINT32 uiCnnHeight;             ///< CNN input Height
	UINT32 uiCnnCh;                 ///< CNN input channel number
} CNN_IN_SIZE;
//@}

/**
    CNN All Parameters.

    This is used for CNN setmode.

*/
//@{
typedef struct _CNN_PAPAM {
	CNN_MODESEL mode;               ///< CNN operation mode
	UINT32 uiInAddr0;
	UINT32 uiInAddr1;
	UINT32 uiInAddr2;
	UINT32 uiInLofs;
	UINT32 uiOutAddr;
	UINT32 uiOutLofs;
	UINT32 uiIntEn;
	CNN_IN_SIZE *pCnnSize;
	CNN_CONV_PAPAM *pConvPara;           ///< convolution parameters
	CNN_RELU_PAPAM *pReluPara;           ///< ReLU parameters
	CNN_POOL_PAPAM *pPoolPara;           ///< pooling parameters
	CNN_LRN_PAPAM *pLRNPara;             ///< LRN parameters
} CNN_PAPAM;
//@}




//
extern ER cnn_open(CNN_OPENOBJ *pObjCB);
extern BOOL cnn_isOpened(void);
extern ER cnn_close(void);
extern ER cnn_pause(void);
extern ER cnn_start(void);
extern void cnn_clrFrameEnd(void);
extern void cnn_waitFrameEnd(BOOL bClrFlag);
extern void cnn_clrIntrStatus(UINT32 uiStatus);
extern ER cnn_setMode(CNN_PAPAM *pCnnPara);
//
extern UINT32 cnn_getIntEnable(void);
extern UINT32 cnn_getIntrStatus(void);
extern UINT32 cnn_getClockRate(void);



//@}
#endif
