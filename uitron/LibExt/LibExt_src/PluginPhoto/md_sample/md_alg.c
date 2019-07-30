/**
    Motion Detection Algorithm.

    depend on CA status to detect the object motion

    @file       md_alg.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "md_alg.h"
#include "md_alg_int.h"

#define __MODULE__ MDALG
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"


UINT32 DiffBlockCnt_Ref[4] = {0,0,0,0};

UINT32 MD_GetDiffBlockCnt(UINT32 Id)
{
    return DiffBlockCnt_Ref[Id];
}

BOOL MDAlg_Start(UINT32 Id, UINT32 buf, UINT32 cachebuf, MDALG_INFO MDInfo)
{
    static UINT16 PreBlkTab[MD_PROC_CNT][MDALG_BLK_TOTAL], DetectionCnt[MD_PROC_CNT] = {0};
    UINT32 i, Diff, Total_diff=0, DiffCnt=0;
    UINT32 reslt;
    DBG_IND("BlockDiffThres =  %d, BlockCntThres = %d, TotalDiffThres = %d\r\n", MDInfo.BlockDiffThres, MDInfo.BlockCntThres, MDInfo.TotalDiffThres);

    DiffBlockCnt_Ref[Id] = 0;

    for(i=0;i<MDALG_BLK_TOTAL;i++)
    {
        if(PreBlkTab[Id][i]>MDInfo.AccTab[MDALG_CH_G][i])
            Diff = (PreBlkTab[Id][i]-MDInfo.AccTab[MDALG_CH_G][i]);
        else
            Diff = (MDInfo.AccTab[MDALG_CH_G][i]-PreBlkTab[Id][i]);

        Total_diff += Diff;
        if(Diff > MDInfo.BlockDiffThres)
            DiffCnt ++;
        if(Diff > MDInfo.BlockDiffThres_Ref)
            DiffBlockCnt_Ref[Id] ++;
    }

    for(i=0;i<MDALG_BLK_TOTAL;i++)
    {
        PreBlkTab[Id][i] = MDInfo.AccTab[MDALG_CH_G][i];
    }

    if(DetectionCnt[Id] < 2) // for initialization
    {
	    DetectionCnt[Id] ++;
        return FALSE;
    }
    
    if((DiffCnt > MDInfo.BlockCntThres) || (Total_diff > MDInfo.TotalDiffThres))
    {
        DBG_IND("DiffCnt = %d > %d,    Total_diff = %d > %d\r\n", DiffCnt, MDInfo.BlockCntThres, Total_diff, MDInfo.TotalDiffThres);
        reslt  = TRUE;
    }
    else
        reslt = FALSE;

    return reslt;
}
