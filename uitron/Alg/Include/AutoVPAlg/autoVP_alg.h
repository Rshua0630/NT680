/**
    AutoVP detection library.

    @file       autoVP_alg.h
    @ingroup    mILibAutoVP_Nvt

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _AUTO_VP_ALG_H
#define _AUTO_VP_ALG_H

/**
    @addtogroup mILibAutoVP_Nvt
*/

/**
     @name AutoVPreturn status
*/
//@{
#define AUTO_VPALG_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define AUTO_VPALG_STA_ERROR               (-1)                  ///<  some error occurred
//@}

#ifndef bool
#define bool    int
#endif
#ifndef true
#define true    1
#endif
#ifndef false
#define false   0
#endif

/**
    AutoVP failure info
*/
//@{
#define AUTO_VP_FAIL_ROI        1       ///<  AVP out of ROI
#define AUTO_VP_FAIL_AVPS       2       ///<  Too much failure AVP
#define AUTO_VP_FAIL_FEW_AVP    4       ///<  Few AVP detected
//@}

typedef struct _AutoVP_ROI
{
    UINT32 left;
    UINT32 top;
    UINT32 right;
    UINT32 bottom;
}AutoVP_ROI;

typedef struct _AutoVP_image
{
    UINT8* adress;
    UINT32 width;
    UINT32 height;
    UINT32 lineOffset;
}AutoVP_image;

typedef struct _AutoVP_point
{
    int x;
    int y;
}AutoVP_point;

typedef struct _AutoVP_line
{
    AutoVP_point sP;
    AutoVP_point eP;
}AutoVP_line;

typedef struct _AutoVP_rect
{
    int x;
    int y;
    int width;
    int height;
}AutoVP_rect;

/**
                 uiVPSrcWidth
 -------------------------------------------------
 - - (uiVPRoiSx, uiVPRoiSy)                      -
 -   -                                           -
 -     ------------uiVPRoiWidth-----------       -
 -     -                                 -       -
 -     -   (iLx1,iLy1)*    * (iRx1,iRy1) -       -
 -     -                          uiVPRoiHeight  - uiVPSrcHeight
 -     -                                 -       -
 -     -(iLx2,iLy2)*           * (iRx2,iRy2)-    -
 -     ----------------------------------        -
 -                                               -
 -                                               -
 -------------------------------------------------
*/
/**
     AutoVP detection input structure.
*/
//@{
typedef struct _AUTO_VP_SRCIMG_INFO
{
    UINT32 uiVPSrcBufAddr;      ///<  Auto VP input buffer address with ROI IME image
    UINT32 uiVPSrcWidth;        ///<  width of Auto VP input image with ROI IME image
    UINT32 uiVPSrcHeight;       ///<  height of Auto VP input image with ROI IME image
    UINT32 uiVPSrcLineofs;      ///<  lineofset of Auto VP input image with ROI IME image
    UINT32 uiFrameWidth;        ///<  width of IME image
    UINT32 uiFrameHeight;       ///<  height of IME image
    UINT32 uiRoiWidth;          ///<  width of ROI region of IME image
    UINT32 uiRoiHeight;         ///<  height of ROI region of IME image
    UINT32 uiRoiSx;             ///<  start x coordinate of ROI region of IME image
    UINT32 uiRoiSy;             ///<  start y coordinate of ROI region of IME image
    UINT32 uiScaleUp;           ///<  IME image scale up
    UINT32 uiScaleDown;         ///<  IME image scale down
    UINT32 uiScaleRsl;          ///<  IME image scale resolution
} AUTO_VP_SRCIMG_INFO;
//@}

//@{
typedef struct _AUTO_VP_SRCDATA_INFO
{
    AUTO_VP_SRCIMG_INFO AutoVPSrcImgInfo;
    UINT32 uiWorkBufAddr;
    UINT32 uiCacheBufAddr;
} AUTO_VP_SRCDATA_INFO;
//@}

//@{
typedef struct _AUTO_VP_RESULT_INFO
{
    AutoVP_line             HoughLines[8];
    UINT32                  HoughCount;         /// Hough Count Lines
    UINT32                  HoughFlag[8];       /// flag: 1~4
    AutoVP_line             ChosedLine[4];
    UINT32                  FailureStatus;
    AutoVP_point            uiVPNew;
    UINT32                  ISETime;
    UINT32                  GraphicTime;
} AUTO_VP_RESULT_INFO;
//@}

typedef struct _AUTO_VP_DEBUG_INFO
{
    BOOL isECosHoughEnable;
    BOOL isLinuxHoughEnable;
    BOOL isCPU2Open;
} AUTO_VP_DEBUG_INFO;

/**
    initial AutoVP parameters with default value.
*/
extern void AutoVP_InitDefParms(void);

/**
    initial AutoVP Lib.
*/
extern void AutoVP_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf);

/**
    uninitial AutoVP Lib.
*/
extern void AutoVP_Uninit(void);

/**
    AutoVP Detection
*/
extern UINT32 AutoVP_Detection(AUTO_VP_SRCIMG_INFO Src_Img_info, UINT32 InputUsableMemAddr, UINT32 InputUsableCacheMemAddr, AUTO_VP_RESULT_INFO *Auto_VP_Info, AUTO_VP_DEBUG_INFO *Auto_VP_Dbg_Info);

/**
    get AutoVP initial flag
*/
extern UINT32 AutoVP_GetInitFlag(void);

/**
    Get AutoVP results info
*/
extern AUTO_VP_RESULT_INFO ADAS_GetAutoVPRstInfo(void);

/**
    Get AutoVP source info
*/
extern AUTO_VP_SRCIMG_INFO ADAS_GetAutoVPSrcInfo(void);

/**
    Get AutoVP debug info
*/
extern AUTO_VP_DEBUG_INFO ADAS_GetAutoVPDbgInfo(void);

/**
    Get AutoVP ROI
*/
extern AutoVP_ROI GetAutoVPROI(void);

/**
    Set AutoVP ROI with default value
*/
extern void setAutoVPDefault(void);

/**
    Set AutoVP ROI
*/
extern void SetAutoVPROI(AutoVP_ROI roi);

#endif

