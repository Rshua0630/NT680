/**
    Motion Detection Algorithm.

    depend on CA status to detect the object motion

    @file       md_alg.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "md_sample_int.h"
#include "md_api_660.h"
#include "md_alg.h"
#include "ipl_utility.h"
#include "GxImage.h"

#define __MODULE__ MDALG
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"
//UINT32 ContinueCnt = 0;
#define MD_LEVEL_MAXIDX 33
//UINT32 MD_LevelMap[11] = {400,500,600,700,800,900,1000,2000,3000,4000,6400};
//strong ~ middle
/*
UINT32 MD_BlockThrs[MD_LEVEL_MAX] = {6,5,5,4,4,4,3,3,2,1,1};
UINT32 MD_BlockCnt[MD_LEVEL_MAX] = {6,6,5,5,4,3,3,2,2,1,1};
UINT32 MD_LPFCnt[MD_LEVEL_MAX] = {4,4,4,4,4,3,3,3,2,2,1};
*/
//middle ~ low
/*
UINT32 MD_BlockThrs[MD_LEVEL_MAX] = {24,21,18,16,14,12,11,10,9,8,7};
UINT32 MD_BlockCnt[MD_LEVEL_MAX] = {24,21,18,16,14,13,12,11,10,9,8};
*/
//low ~ lowest
UINT32 MD_BlockThrs[MD_LEVEL_MAXIDX] = {75,60,50,45,42,39,36,33,30,28,26,24,21,18,16,14,12,11,10,9,8,7,6,5,5,4,4,4,3,3,2,1,1};
UINT32 MD_BlockCnt[MD_LEVEL_MAXIDX] = {100,70,55,45,42,39,36,33,30,28,26,24,21,18,16,14,13,12,11,10,9,8,6,6,5,5,4,3,3,2,2,1,1};
UINT32 MD_LPFCnt[MD_LEVEL_MAXIDX] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,2,2,1};

UINT32 MD_TotalThrs[MD_LEVEL_MAXIDX] = {100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000,100000};

static UINT16 MDAccLum1[MD_LAWIN_TOTAL];
static UINT16 MDAccLum2[MD_LAWIN_TOTAL];
static UINT16 MDAccHisto[MD_LAWIN_TOTAL];
MDALG_INFO MdInfo[MD_PROC_CNT] = {0};
static UINT16 MDAccLumAVG[MD_LAWIN_TOTAL] = {0};

UINT32 MD_BlkDiff_Thr[MD_PROC_CNT] = {MD_BLKDIFF_THR, MD_BLKDIFF_THR};
UINT32 MD_BlkCnt_Thr[MD_PROC_CNT] = {MD_BLKCNT_THR, MD_BLKCNT_THR};
UINT32 MD_DiffSum_Thr[MD_PROC_CNT] = {MD_DIFFSUM_THR, MD_DIFFSUM_THR};
UINT32 gMDRatio[MD_PROC_CNT] = {0};
BOOL MD_Det_Win_Init[MD_PROC_CNT] = {FALSE, FALSE};
UINT32 gMD_Level = MD_LEVEL_NORMAL;
UINT32 gMD_LPF_Cnt = 0;

UINT8 MD_DetWin[MD_DETWIN_Y][MD_DETWIN_X] = {0};

//#NT#2016/10/05#Harry Tsai -begin
//#NEW, Add for ROI's reference
UINT32 MD_BlkDiff_Thr_Ref[MD_PROC_CNT] = {MD_BLKDIFF_THR, MD_BLKDIFF_THR};
//#NT#2016/10/05#Harry Tsai -end

BOOL MD_Process(UINT32 Id,   IMG_BUF* pBuf);

void MD_SetDetWin(UINT32 Id, UINT8 *win)
{
    UINT16 i, j, IdxY;
    UINT32 Ratio_X, Ratio_Y;

    Ratio_X = MD_DETWIN_X*100/16;// UI is 16x12, need to mapping window index
    Ratio_Y = MD_DETWIN_Y*100/12;

    if (MD_Det_Win_Init[Id] == FALSE)
    {
        memset(MD_DetWin, 1, sizeof(UINT8)*MD_DETWIN_Y*MD_DETWIN_X);
        MD_Det_Win_Init[Id] = TRUE;
    }

    //memcpy(MD_DetWin, win, sizeof(UINT8)*MD_DETWIN_Y*MD_DETWIN_X);
    for(j = 0; j< MD_DETWIN_Y; j++)
    {
        IdxY = j*100/Ratio_Y;
        for(i = 0; i< MD_DETWIN_X; i++)
        {
            MD_DetWin[j][i] = win[IdxY*16+ i*100/Ratio_X];
        }
    }


}

//void MD_SetLevel(UINT32 Id, MD_LEVEL level)
void MD_SetLevel(UINT32 Id, UINT32 level)
{
    if (level >= MD_LEVEL_MAXIDX)
    {
        DBG_ERR("Id: %d, Level %d Overflow, force set to MD_LEVEL_NORMAL", Id, level);
        level = MD_LEVEL_NORMAL;
    }

    gMD_Level = level;
    //MDLevel = MD_LevelMap[level];

    //MD_BlkDiff_Thr[Id] = MD_BLKDIFF_THR * 100 / MDLevel;
    //MD_BlkCnt_Thr[Id] = MD_BLKCNT_THR * 100 / MDLevel;
    //MD_DiffSum_Thr[Id] = MD_DIFFSUM_THR * 100 / MDLevel;
    MD_BlkDiff_Thr[Id] = MD_BlockThrs[level];
    MD_BlkCnt_Thr[Id] = MD_BlockCnt[level];
    MD_DiffSum_Thr[Id] = MD_TotalThrs[level];
    gMD_LPF_Cnt = MD_LPFCnt[level];

}

/*
static void MD_getLA(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo, UINT32 *WinXNum, UINT32 *WinYNum)
{
    IPL_LA_INFOR LAInfo;
    IPL_LA_SET_INFOR LASetInfo = {0};
    UINT16 i, j;
    UINT32 Ratio_X, Ratio_Y;
    UINT32 WinCnt = 0;

    if (MD_Det_Win_Init[Id] == FALSE)
    {
        memset(MD_DetWin, 1, sizeof(UINT8)*MD_DETWIN_Y*MD_DETWIN_X);
        MD_Det_Win_Init[Id] = TRUE;
    }

    LASetInfo.Id = Id;
    IPL_GetCmd(IPL_GET_LA_INFOR, (void *)&LASetInfo);

    if (LASetInfo.WinXNum >= MD_DETWIN_X)
    {
        Ratio_X = LASetInfo.WinXNum/MD_DETWIN_X;
        Ratio_Y = LASetInfo.WinYNum/MD_DETWIN_Y;
    }
    else
    {
        if(LASetInfo.WinXNum != 0)
            Ratio_X = MD_DETWIN_X/LASetInfo.WinXNum;
        else
        {
            DBG_ERR("LASetInfo.WinXNum is zero!!\r\n");
            Ratio_X = 1;
        }

        if(LASetInfo.WinYNum != 0)
            Ratio_Y = MD_DETWIN_Y/LASetInfo.WinYNum;
        else
        {
            DBG_ERR("LASetInfo.WinYNum is zero!!\r\n");
            Ratio_Y = 1;
        }

    }

    LAInfo.Id = Id;
    LAInfo.Lum1 = Lum1;
    LAInfo.Lum2 = Lum2;
    LAInfo.Histo = Histo;
    IPL_GetCmd(IPL_GET_CUR_LA_INFOR, (void *)&LAInfo);

    for (j = 0; j < LASetInfo.WinYNum; j ++)
    {
        for (i = 0; i < LASetInfo.WinXNum; i ++)
        {
            if (MD_DetWin[j/Ratio_Y][i/Ratio_X] > 1)
                MD_DetWin[j/Ratio_Y][i/Ratio_X] = 1;

            Lum1[j * LASetInfo.WinXNum + i] = ((Lum1[j * LASetInfo.WinXNum + i] >> (12-MD_ACC_BIT)) * MD_DetWin[j/Ratio_Y][i/Ratio_X]);
            Lum2[j * LASetInfo.WinXNum + i] = ((Lum2[j * LASetInfo.WinXNum + i] >> (10-MD_ACC_BIT)) * MD_DetWin[j/Ratio_Y][i/Ratio_X]);
            WinCnt += MD_DetWin[j/Ratio_Y][i/Ratio_X];
        }
    }

    if(0)//((LASetInfo.WinXNum != 0) && (LASetInfo.WinYNum != 0))
        gMDRatio[Id] = ((WinCnt * MD_NORMAL_BASE) / (LASetInfo.WinXNum * LASetInfo.WinYNum));
    else
    {
        //DBG_ERR("LASetInfo.WinXNum = %d, LASetInfo.WinYNum =%d!!\r\n");
        gMDRatio[Id] = MD_NORMAL_BASE;
    }

    *WinXNum = LASetInfo.WinXNum;
    *WinYNum = LASetInfo.WinYNum;
}
*/

//#NT#2016/02/26#Harry Tsai -begin
//#NEW, calculate Lum from IME output for CCIR-in
static UINT16 BlockAverageY(UINT32 YAddr,UINT32 x_start,UINT32 y_start,UINT32 width,UINT32 height,UINT32 offset)
{
    UINT16    i,j;
    UINT32    Yall=0,YAvg=0,Ypixel=0,cnt=0,tmp=0;

    for(j=0;j<height;j+=4)
    {
        for(i=0;i<width;i+=16)
        {
            tmp = *(volatile UINT32 *)(YAddr+(offset*(y_start+j))+4*((x_start+i)/4));
            Ypixel=(tmp    )&(0xff);
            Yall+=Ypixel;
            Ypixel=(tmp>>8 )&(0xff);
            Yall+=Ypixel;
            Ypixel=(tmp>>16)&(0xff);
            Yall+=Ypixel;
            Ypixel=(tmp>>24)&(0xff);
            Yall+=Ypixel;
            cnt+=4;
        }
    }

    if(cnt != 0)
        YAvg = Yall/cnt;
    else
        YAvg = 0;

    return (UINT16)(YAvg);
}

static void MD_getImeY(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo, UINT32 *WinXNum, UINT32 *WinYNum, IMG_BUF *pBuf)
{
    UINT16 i,j,Lum,Idx;
    UINT32 h_size, v_size, offset;
   // IPL_YCC_IMG_INFO FB_para = {0};

    //#NT#2017/12/10#Harry Tsai -begin
    //get IME ready buffer address here

    //FB_para = IPL_GetIMEReadyBufferAddr(1,Id);

    //#NT#2017/12/10#Harry Tsai -end

   // h_size = FB_para.ch[0].width;
   // v_size = FB_para.ch[0].height;
   // offset = FB_para.ch[0].line_ofs;
	h_size = pBuf->Width;
	v_size = pBuf->Height;
	offset = pBuf->LineOffset[0];
    for(j=0;j<MD_LAWIN_Y;j++)
    {
        for(i=0;i<MD_LAWIN_X;i++)
        {
            Idx = j*MD_DETWIN_X+i;
            Lum = BlockAverageY(pBuf->PxlAddr[0],((i*h_size)/MD_LAWIN_X),((j*v_size)/MD_LAWIN_Y),(h_size/MD_LAWIN_X),(v_size/MD_LAWIN_Y),offset);
            Lum1[Idx] = Lum;
            Lum2[Idx] = Lum;

            if (MD_DetWin[j][i] > 1)
                MD_DetWin[j][i] = 1;
            
            Lum1[Idx] = (Lum1[Idx] * MD_DetWin[j][i]);
            Lum2[Idx] = Lum1[Idx];
        }
    }

    gMDRatio[Id] = (MD_NORMAL_BASE);

    *WinXNum = MD_LAWIN_X;
    *WinYNum = MD_LAWIN_Y;
}
//#NT#2016/02/26#Harry Tsai -end


BOOL MD_Process(UINT32 Id, IMG_BUF *pBuf)
{
//#NT#2016/05/25#Harry_Tsai -begin
    BOOL rslt;// = TRUE;
//#NT#2016/05/25#Harry_Tsai -end
    UINT32 WinXNum, WinYNum;
    static UINT32 LPFCnt = 0;
    static BOOL prv_result = FALSE;

    MD_getImeY(Id, MDAccLum1, MDAccLum2, MDAccHisto, &WinXNum, &WinYNum,pBuf);

    UINT32 BlockCnt;
    if(LPFCnt == 0)
    {
        for(BlockCnt = 0; BlockCnt < MD_LAWIN_TOTAL; BlockCnt ++)
        {
            MDAccLumAVG[BlockCnt] = MDAccLum1[BlockCnt];
        }
        LPFCnt ++;
    }
    else
    {
        if(LPFCnt >= gMD_LPF_Cnt)
        {
            for(BlockCnt = 0; BlockCnt < MD_LAWIN_TOTAL; BlockCnt ++)
            {
                MDAccLumAVG[BlockCnt] /= LPFCnt;
            }

            MdInfo[Id].CAWinX = WinXNum;
            MdInfo[Id].CAWinY = WinYNum;
            MdInfo[Id].AccTab[MDALG_CH_R] = MDAccLumAVG;
            MdInfo[Id].AccTab[MDALG_CH_G] = MDAccLumAVG;
            MdInfo[Id].AccTab[MDALG_CH_B] = MDAccLumAVG;
            MdInfo[Id].BlockDiffThres = MD_BlkDiff_Thr[Id];
            MdInfo[Id].BlockCntThres = MD_BlkCnt_Thr[Id];
            MdInfo[Id].TotalDiffThres = MD_DiffSum_Thr[Id];
            MdInfo[Id].BlockDiffThres_Ref = MD_BlkDiff_Thr_Ref[Id];

            rslt = MDAlg_Start(Id, (UINT32)NULL, (UINT32)NULL, MdInfo[Id]);
/*
            if(rslt)
                DBG_ERR("Motion=%d\r\n",rslt);
            else
                DBG_DUMP("Motion=%d\r\n",rslt);
*/
            LPFCnt = 0;
            prv_result = rslt;
            return rslt;
        }
        else
        {
            for(BlockCnt = 0; BlockCnt < MD_LAWIN_TOTAL; BlockCnt ++)
            {
                MDAccLumAVG[BlockCnt] += MDAccLum1[BlockCnt];
            }

            LPFCnt++;
        }
    }
    //return FALSE;
    return prv_result;
}

void MD_GetInfo(MD_INFO *Info)
{
    Info->Level = gMD_Level;
    Info->WeightWin = &MD_DetWin[0][0];
}
