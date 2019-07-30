/**
    Motion Detection Algorithm.



    @file       MDAlg.h
    @ingroup    mILibMDAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _MD_ALG_H_
#define _MD_ALG_H_

#define MD_PROC_CNT          2
#define MD_CAWIN_X           32
#define MD_CAWIN_Y           32
#define MD_CAWIN_TOTAL       (MD_CAWIN_X * MD_CAWIN_Y)

/**
     @name MD Alg. default value
     @note
*/
typedef enum _MDALG_CH
{
    MDALG_CH_R = 0,
    MDALG_CH_G,
    MDALG_CH_B,
    MDALG_CH_MAX,
    ENUM_DUMMY4WORD(MDALG_CH)
}MDALG_CH;

typedef struct _MDALG_INFO
{
    UINT32 CAWinX;
    UINT32 CAWinY;
    UINT16 *AccTab[MDALG_CH_MAX];   // CA Info

    UINT32 BlockDiffThres;          // Threshold for Y different
    UINT32 BlockCntThres;
    UINT32 TotalDiffThres;          // Threshold for diff sum
    //#NT#2016/10/05#Harry Tsai -begin
    //#NEW, Add for ROI's reference
    UINT32 BlockDiffThres_Ref;          // Threshold for Y different
    //#NT#2016/10/05#Harry Tsai -end
} MDALG_INFO;

extern UINT32 MDAlg_GetBufSize(void);
extern UINT32 MDAlg_GetCacheSize(void);
extern BOOL MDAlg_Start(UINT32 Id, UINT32 buf, UINT32 cachebuf, MDALG_INFO MDInfo);
//#NT#2016/10/05#Harry Tsai -begin
//#NEW, Add for ROI's reference
extern UINT32 MD_GetDiffBlockCnt(UINT32 Id);
//#NT#2016/10/05#Harry Tsai -end
#endif //_AE_ALG_INT_H_
