/**
    Motion Detection api.


    @file       md_api.h
    @ingroup    mILibMDAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _MD_API_H_
#define _MD_API_H_
#include "GxImage.h"

typedef enum _MD_LEVEL
{
    MD_LEVEL_N5,
    MD_LEVEL_N4,
    MD_LEVEL_N3,
    MD_LEVEL_N2,
    MD_LEVEL_N1,
    MD_LEVEL_NORMAL,
    MD_LEVEL_P1,
    MD_LEVEL_P2,
    MD_LEVEL_P3,
    MD_LEVEL_P4,
    MD_LEVEL_P5,
    MD_LEVEL_MAX    = MD_LEVEL_P5+1,
    ENUM_DUMMY4WORD(MD_LEVEL)
}MD_LEVEL;

typedef struct _MD_INFO
{
    UINT32 Level;
    UINT8 *WeightWin;
}MD_INFO;

//extern void MD_SetLevel(UINT32 Id, MD_LEVEL level);
extern void MD_SetLevel(UINT32 Id, UINT32 level);
extern BOOL MD_Process(UINT32 Id,   IMG_BUF* pBuf);
extern void MD_SetDetWin(UINT32 Id, UINT8 *win);
extern void MD_GetInfo(MD_INFO *Info);
//#NT#2016/10/05#Harry Tsai -begin
//#NEW, Add for ROI's reference
extern UINT32 MD_GetDiffBlockCnt(UINT32 Id);
//#NT#2016/10/05#Harry Tsai -end

#endif //_MD_API_H_
