/*
    Public header file for IFE2 module.

    This file is the header file that define the API and data type for IFE2
    module.

    @file       ife2_lib.h
    @ingroup    mIIPPIFE2

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _IFE2_LIB_H_
#define _IFE2_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "Type.h"


/**
    @addtogroup mIDrvIPP_IFE2
*/
//@{


/**
    Define IFE2 interrput enable.
*/
#define IFE2_INTE_OVFL     0x40000000  ///< overflow interrupt
#define IFE2_INTE_FRMEND   0x80000000  ///< frame end interrupt
#define IFE2_INTE_ALL      (IFE2_INTE_OVFL | IFE2_INTE_FRMEND)

/**
    Define IFE2 interrput status.
*/
#define IFE2_INT_OVFL     0x40000000  ///< overflow interrupt
#define IFE2_INT_FRMEND   0x80000000  ///< frame end interrupt
#define IFE2_INT_ALL      (IFE2_INTE_OVFL | IFE2_INTE_FRMEND)
//------------------------------------------------------------------


/**
    IFE2 enum - function enable/disable
*/
typedef enum {
	IFE2_FUNC_DISABLE   = 0,           ///< function disable
	IFE2_FUNC_ENABLE    = 1,           ///< function enable
	ENUM_DUMMY4WORD(IFE2_FUNC_EN)
} IFE2_FUNC_EN;
//------------------------------------------------------------------


/**
    IFE2 enum - operation mode selection
*/
typedef enum {
	IFE2_OPMODE_D2D     = 0,    ///< IFE2 only
	IFE2_OPMODE_RHE_IPP = 1,    ///< RDE + IFE->DCE->IPE->IME(IFE2)
	IFE2_OPMODE_SIE_IPP = 2,    ///< SIE + IFE->DCE->IPE->IME(IFE2) (all direct)
	IFE2_OPMODE_UNKNOWN = 3,    ///< Undefined mode
	ENUM_DUMMY4WORD(IFE2_OPMODE)
} IFE2_OPMODE;
//------------------------------------------------------------------


/**
    IFE2 enum - input image data format
*/
typedef enum {
	IFE2_FMT_YUVP   = 0,           ///< YUVYUV Packing
	IFE2_FMT_SIGL   = 1,           ///< Single Channel
	IFE2_FMT_Y_UVP  = 2,           ///< Y/UV-Packing
	ENUM_DUMMY4WORD(IFE2_FMT)
} IFE2_FMT;
//------------------------------------------------------------------


/**
    IFE2 enum - filter kernel size selection
*/
typedef enum {
	IFE2_FLTR_SIZE_3x3 = 0,           ///< 3x3 window
	IFE2_FLTR_SIZE_5x5 = 1,           ///< 5x5 window
	IFE2_FLTR_SIZE_7x7 = 2,           ///< 7x7 window
	IFE2_FLTR_SIZE_9x9 = 3,           ///< 9x9 window
	ENUM_DUMMY4WORD(IFE2_FILT_SIZE)
} IFE2_FILT_SIZE;
//------------------------------------------------------------------


/**
    IFE2 enum - edge kernel size selection
*/
typedef enum {
	IFE2_EKNL_SIZE_3x3 = 0,           ///< 3x3 window
	IFE2_EKNL_SIZE_5x5 = 1,           ///< 5x5 window
	ENUM_DUMMY4WORD(IFE2_EDGE_KERNEL_SIZE)
} IFE2_EDGE_KERNEL_SIZE;
//------------------------------------------------------------------


/**
    IFE2 enum - DMA burst length selection
*/
typedef enum {
	IFE2_BURST_64W = 0,           ///< burst 64 word
	IFE2_BURST_48W = 1,           ///< burst 48 word
	IFE2_BURST_96W = 2,           ///< burst 96 word
	IFE2_BURST_128W = 3,           ///< burst 128 word
	ENUM_DUMMY4WORD(IFE2_BURST_SEL)
} IFE2_BURST_SEL;
//------------------------------------------------------------------

/**

    IFE2 DMA Channel Handle Selection
*/
typedef enum {
	IFE2_DMAHDL_IN    = 0,  ///< selection input DMA channel
	IFE2_DMAHDL_OUT0  = 1,  ///< selection output DMA channel0
	ENUM_DUMMY4WORD(IFE2_DMAHDL_SEL)
} IFE2_DMAHDL_SEL;
//------------------------------------------------------------------

/**

    IFE2 Get Burst Length Selection
*/
typedef enum {
	IFE2_GET_BURST_IN  = 0,  ///< get input burst length
	IFE2_GET_BURST_OUT = 1,  ///< get output burst length
	ENUM_DUMMY4WORD(IFE2_GET_BURST_SEL)
} IFE2_GET_BURST_SEL;
//------------------------------------------------------------------


/**
    IFE2 structure - engine open object
*/
typedef struct _IFE2_OPENOBJ {
	void (*pfIfe2IsrCb)(UINT32 uiIntpStatus); ///< isr callback function
	UINT32 uiIfe2ClkSel;                    ///< Engine clock-rate in MHz; Selection: 48/60/80/120/160/192/220/240
} IFE2_OPENOBJ;
//------------------------------------------------------------------


/**
    IFE2 structure - image size parameters
*/
typedef struct _IFE2_IMG_SIZE {
	UINT32 uiImgSizeH;   ///< input horizontal size, image width
	UINT32 uiImgSizeV;  ///< input vertical size, image height
} IFE2_IMG_SIZE;
//------------------------------------------------------------------


/**
    IFE2 structure - image lineoffset parameters
*/
typedef struct _IFE2_LINEOFFSET {
	UINT32 uiLineoffsetY;   ///< lineoffset for Y channel
} IFE2_LINEOFFSET;
//------------------------------------------------------------------


/**
    IFE2 structure - input image DMA address
*/
typedef struct _IFE2_DMA_ADDR {
	UINT32 uiAddrY;   ///< lineoffset for Y channel
} IFE2_DMA_ADDR;
//------------------------------------------------------------------


/**
    IFE2 structure - reference center computation parameters
*/
typedef struct _IFE2_REFCENT_SET {
	UINT32 uiRTh[3];   ///< Range filter threshold for reference center
	UINT32 uiRWt[4];   ///< Range filter weighting for reference center
	UINT32 uiCntWt;    ///< Reference center weighting
	UINT32 uiOutlDth;  ///< Outlier difference threshold
	UINT32 uiOutlTh;   ///< Reference center outlier threshold
} IFE2_REFCENT_SET;
//------------------------------------------------------------------


/**
    IFE2 structure - edge direction threshold parameters
*/
typedef struct _IFE2_EDRCT_TH {
	UINT32  uiEdthHV;   ///< Edge direction threshold for two diagonal directions
	UINT32  uiEdthPN;   ///< Edge direction threshold for horizontal and vertical directions
} IFE2_EDRCT_TH;
//------------------------------------------------------------------


/**
    IFE2 structure - filter threshold and weighting LUT parameters
*/
typedef struct _IFE2_FILTER_SET {
	UINT32 uiFtrTh[5];  ///< Filter threshold LUT
	UINT32 uiFtrWt[6];  ///< Filter weighting LUT
} IFE2_FILTER_SET;
//------------------------------------------------------------------


/**
    IFE2 structure - gray statistical parameters
*/
typedef struct _IFE2_GRAY_STATAL {
	UINT32 uiUTh0;  ///< Gray Statistical threshold for U channel
	UINT32 uiUTh1;  ///< Gray Statistical threshold for U channel
	UINT32 uiVTh0;  ///< Gray Statistical threshold for V channel
	UINT32 uiVTh1;  ///< Gray Statistical threshold for V channel
} IFE2_GRAY_STATAL;
//------------------------------------------------------------------


/**
    IFE2 structure - input and output burst size parameters
*/
typedef struct _IFE2_BURST_LENGHT {
	IFE2_BURST_SEL  BurstLenInput;      ///< engine input burst lenght
	IFE2_BURST_SEL  BurstLenOutput;      ///< engine output burst lenght, if direct to IME, fixed as 64 words
} IFE2_BURST_LENGHT;


/**
    IFE2 structure - DMA address changing parameters
*/
typedef struct _IFE2_ADDR_PARAM {
	IFE2_DMAHDL_SEL     ChlSel;  ///< channel selection
	IFE2_DMA_ADDR       Addr;       ///< changed dma address
} IFE2_ADDR_PARAM;
//------------------------------------------------------------------

/**
    IFE2 structure -lineoffset changing parameters
*/
typedef struct _IFE2_LOFS_PARAM {
	IFE2_DMAHDL_SEL     ChlSel;  ///< channel selection
	IFE2_LINEOFFSET     Ofs;        ///< channel lineoffset
} IFE2_LOFS_PARAM;
//------------------------------------------------------------------

/**
    IFE2 structure - reference center changing parameters
*/
typedef struct _IFE2_REFCENT_PARAM {
	IFE2_REFCENT_SET    RefCentY;   ///< reference center parameters for Y channel
	IFE2_REFCENT_SET    RefCentUV;   ///< reference center parameters for UV channel
} IFE2_REFCENT_PARAM;
//------------------------------------------------------------------

/**
    IFE2 structure - filter parameter changing
*/
typedef struct _IFE2_FILTER_PARAM {
	IFE2_FUNC_EN            FtrYEn;         ///< filter Y channel enable
	IFE2_FILT_SIZE          FtrSize;        ///< filter size
	IFE2_EDRCT_TH           FtrEdth;        ///< filter edge direction threshold
	IFE2_EDGE_KERNEL_SIZE   EdgeKerSize;    ///< edge kernel size
	IFE2_FILTER_SET         FtrSetY;        ///< filter parameter for Y channel
	IFE2_FILTER_SET         FtrSetU;        ///< filter parameter for U channel
	IFE2_FILTER_SET         FtrSetV;        ///< filter parameter for V channel
} IFE2_FILTER_PARAM;
//------------------------------------------------------------------

/**
    IFE2 structure - full engine parameters

    @note integrated structure
*/
typedef struct _IFE2_PARAM {
	IFE2_OPMODE             OpMode;         ///< engine operation mode

	IFE2_IMG_SIZE           ImgSize;        ///< image size
	IFE2_FMT                SrcFmt;         ///< input image format
	IFE2_FMT                DstFmt;         ///< output image format

	IFE2_LOFS_PARAM         InOfs;          ///< input lineoffset
	IFE2_ADDR_PARAM         InAddr;         ///< input dma address


	IFE2_LOFS_PARAM         OutOfs;         ///< output lineoffset for Y/UV
	IFE2_ADDR_PARAM         OutAddr0;       ///< output dma address for set0

	IFE2_REFCENT_PARAM      RefCent;        ///< reference center parameters for Y / UV channel

	IFE2_FILTER_PARAM       Ftr;            ///< filter parameters

	IFE2_GRAY_STATAL        GrayStatal;     ///< gray statistical
	IFE2_BURST_LENGHT       BurstSize;      ///< engine burst lenght

	UINT32                  uiIntrpEn;      ///< interrupt enable
} IFE2_PARAM;
//------------------------------------------------------------------


/**
    check IFE2 is opened.

    check if IFE2 module is opened.

    @return
        -@b TRUE: IFE2 is open.
        -@b FALSE: IFE2 is closed.
*/
extern BOOL ife2_isOpened(VOID);
//------------------------------------------------------------------

/**
  Open IFE2 driver

  This function should be called before calling any other functions

  @param[in] pObjCB ISR callback function

  @return
        - @b E_OK:  Done with no error.
        - Others:   Error occured.
*/
extern ER ife2_open(IFE2_OPENOBJ *pObjCB);
//------------------------------------------------------------------

/**
  set IFE2 mode

  Set IFE2 mode to execute

  @param[in] pIfe2Param Parameter for IFE2 execution
  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_setMode(IFE2_PARAM *pIfe2Param);
//------------------------------------------------------------------


/**
  Pause IFE2

  Set IFE2 status to Pause.

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_pause(VOID);
//------------------------------------------------------------------

/**
  Start IFE2

  Set IFE2 start bit to 1.

  @param[in] VOID

  @return  VOID
*/
extern ER ife2_start(VOID);
//------------------------------------------------------------------

/**
  Close IFE2 driver

  Release IFE2 driver

  @return
        - @b E_OK:  Done with no error.
        - Others:   Error occured.
*/
extern ER ife2_close(VOID);
//------------------------------------------------------------------

/**
  Wait IFE2 Processing done

  Wait IFE2 Processing done

  @param[in] isClrFlg   Decide to clear flag before waiting frame-end

  @return void
*/
extern VOID ife2_waitFlagFrameEnd(BOOL isClrFlg);
//------------------------------------------------------------------

/**
  Clear IFE2 Processing done Flag

  Clear IFE2 Processing done Flag

  @return void
*/
extern VOID ife2_clearFlagFrameEnd(VOID);
//------------------------------------------------------------------

/**
  Check IFE2 Processing done Flag

  Check IFE2 Processing done Flag

  @return void
*/
extern BOOL ife2_checkFlagFrameEnd(VOID);
//------------------------------------------------------------------

/**
  IFE2 interrupt status.

  Check IFE2 interrupt status.

  @return IFE2 interrupt status readout.
*/
extern UINT32 ife2_getIntStatus(VOID);
//------------------------------------------------------------------

/**
  IFE2 clear interrupt status.

  IFE2 clear interrupt status.

  @param[in] uiIntrp 1's in this word will clear corresponding interrupt

  @return  VOID
*/
extern VOID ife2_clearInt(UINT32 uiIntrp);
//------------------------------------------------------------------

/**
  Set input size

  Set input size

  @param[in] pChgImgSize    input image size

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_chgInSize(IFE2_IMG_SIZE *pChgImgSize);
//------------------------------------------------------------------

/**
  Get input size

  Get input size

  @param[in] puiHSize  input horizontal size
  @param[in] puiVSize  input vertical size

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern VOID ife2_getInSize(UINT32 *puiHSize, UINT32 *puiVSize);
//------------------------------------------------------------------

/**
  Set input source format

  Set input source format

  @param[in] pSrcFmt input image format

  @return  VOID
*/
extern ER ife2_chgSrcFmt(IFE2_FMT SrcFmt);
//------------------------------------------------------------------

/**
  Get input source format

  Get input source format

  @param[in] pSrcFmt source format 0: normal; 1:RAW

  @return  engine source format
*/
extern UINT32 ife2_getSrcFmt(VOID);
//------------------------------------------------------------------

/**
  Set output destination format

  Set output destination format

  @param[in] DstFmt output image format

  @return  VOID
*/
extern ER ife2_chgDstFmt(IFE2_FMT DstFmt);
//------------------------------------------------------------------

/**
  Get output source format

  Get output source format

  @return UINT32  output image format
*/
extern UINT32 ife2_getDstFmt(VOID);
//------------------------------------------------------------------

/**
  change input and output lineoffset

  change input and output DMA address

  @param[in] pChgLofs   lineoffset parameters

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_chgLineoffset(IFE2_LOFS_PARAM *pChgLofs);
//------------------------------------------------------------------

/**
  Get input and output lineoffset

  @param[in] ChlSel     channel selection

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern VOID ife2_getLineoffset(IFE2_DMAHDL_SEL ChlSel, IFE2_LINEOFFSET *pGetLofs);
//------------------------------------------------------------------

/**
  Change input and output DMA address

  @param[in] pChgAddr   change channel DMA address parameters

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_chgDmaAddr(IFE2_ADDR_PARAM *pChgAddr);
//------------------------------------------------------------------

/*
  Get input and output DMA address

  Get input and output DMA address

  @param[in] ChlSel     input or output channel selection
  @param[in] pGetAddr   return channel address

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern VOID ife2_getDmaAddr(IFE2_DMAHDL_SEL ChlSel, IFE2_DMA_ADDR *pGetAddr);
//------------------------------------------------------------------

/**
  Change Reference Center Parameters

  @param[in] pChgRefCent    reference Center Parameters

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_chgRefCenterParam(IFE2_REFCENT_PARAM *pChgRefCent);
//------------------------------------------------------------------

/**
  Change Filter Parameters

  @param[in] pChgFtr    Filter Parameters

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_chgFilterParam(IFE2_FILTER_PARAM *pChgFtr);
//------------------------------------------------------------------

/**
  Change IFE2 Gray Statistical

  @param[in] pChgGStl   output edge information

  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_chgGrayStatalParam(IFE2_GRAY_STATAL *pChgGStl);
//------------------------------------------------------------------



/**
  Get IFE2 clock-rate

  @param[in] VOID

  @return IFE2 clock-rate in MHz
*/
extern UINT32 ife2_getClockRate(VOID);
//------------------------------------------------------------------

/**
  Get IFE2 Base Address of Register

  @param[in] VOID

  @return UINT32 Address
*/
extern UINT32 ife2_getRegBaseAddr(VOID);
//------------------------------------------------------------------

/**
  Get IFE2 Get gray averagel for U and V channels

  @param[in] puiGetAvgU Get gray average of U channel
  @param[in] puiGetAvgV Get gray average of V channel

  @return VOID
*/
extern VOID ife2_getGrayAverage(UINT32 *puiGetAvgU, UINT32 *puiGetAvgV);
//------------------------------------------------------------------

/**
  Get IFE2 HW Debug Function

  @param[in] uiEdgeEn Output edge information
  @param[in] uiRefCentEn Output reference center information

  @return VOID
*/
extern VOID ife2_setDebugFunctionEnable(IFE2_FUNC_EN uiEdgeEn, IFE2_FUNC_EN uiRefCentEn, IFE2_FUNC_EN uiEngEn);
//------------------------------------------------------------------

/**
  Change all parameters

  @param[in] pIfe2Param Parameter for IFE2 execution
  @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER ife2_chgAllParam(IFE2_PARAM *pIfe2Param);
//------------------------------------------------------------------

/**
    Get burst length information

    @param[in] pIfe2Param Parameter for IFE2 execution
    @return INT32
*/
extern INT32 ife2_getBurstLength(IFE2_GET_BURST_SEL GetBstSel);
//------------------------------------------------------------------


#ifdef __cplusplus
}  // extern "C"
#endif


#endif

//@}
