////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "UIInfo.h"
#include "UIPhotoInfo.h"
#include "DateTimeInfo.h"
#include "UIDateImprint.h"
#include "UIDateImprintID.h"
#include "UIResource.h"
#include "Utility.h"
#include "FontConv.h"
#include "UIMode.h"
#include "GxImage.h"
#include "UIPhotoMapping.h"
#include "DateStampFontTbl56x96.h"
#include "DateStampFontTbl42x72.h"
#include "DateStampFontTbl36x60.h"
#include "DateStampFontTbl26x44.h"
#include "DateStampFontTbl20x44.h"
#include "DateStampFontTbl18x30.h"
#include "DateStampFontTbl12x20.h"
#include "DateStampFontTbl10x16.h"
#include "nvtmpp.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIDateImprint
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define  USE_DBG_STAMP          DISABLE

#define DATE_COLOR_TRANSPARENT  0x00808000
#define DATE_COLOR_WHITE        0x008080FF
#define DATE_COLOR_BLACK        0x00818101
#define DATE_COLOR_ORANGED      0x00D4328A
#define DATE_COLOR_RED          0x00FF554E

typedef enum {
	DATE_IMPRINT_EVENT_QV = 0,            ///<quick view event
	DATE_IMPRINT_EVENT_SCR,               ///<screennail event
	DATE_IMPRINT_EVENT_PRI,               ///<primary image event
	DATE_IMPRINT_EVENT_MAX,               ///<Maximun value
} DATE_IMPRINT_EVENT;


typedef struct{
    UINT32  PosX;
    UINT32  PosY;
    IMG_BUF Img;
    UINT32  ColorKey;                ///< format 0x00YYUUVV
    UINT32  StampWeight;             ///< 0 ~ 255
} DS_STAMP_INFOR;


typedef struct {
	IMG_CAP_DATASTAMP_INFO *pCapInfo;
	UINT32                 MemAddr;
	UINT32                 MemSize;
	FONT_CONV_IN           tYuvDesc;
	FONT_CONV_OUT          tYuvInfo;
	DS_STAMP_INFOR         StampInfo;
	GX_IMAGE_CP_ENG        copyEngine;
	UINT32                 lockptn;
	BOOL                   isStrDirty;
	CHAR                   StrBuf[64];
	UINT32                 pic_cnt;
} DATE_IMPRINT_INFO;


typedef struct {
	DATE_IMPRINT_INFO     info[DATE_IMPRINT_EVENT_MAX];

} DATE_IMPRINT_CTRL;

static DATE_IMPRINT_CTRL  gDateImprintCtrl = {0};
static DATE_IMPRINT_CTRL  *pCtrl = &gDateImprintCtrl;
//static char               gUiDateImprint_StrBuf[64] = {0};
static NVTMPP_VB_POOL g_DateImprintPool[DATE_IMPRINT_EVENT_MAX]={NVTMPP_VB_INVALID_POOL, NVTMPP_VB_INVALID_POOL, NVTMPP_VB_INVALID_POOL};

void UiDateImprint_Lock(UINT32 ptn)
{
	FLGPTN       uiFlag = 0;
	wai_flg(&uiFlag, UI_DATEIMPRINT_FLG_ID, ptn, TWF_CLR);
}

void UiDateImprint_UnLock(UINT32 ptn)
{
	set_flg(UI_DATEIMPRINT_FLG_ID, ptn);
}


void UiDateImprint_InitBuff(void)
{
	DATE_IMPRINT_INFO   *pInfo;
	UINT32               i,genMaxw, genMaxh;
	NVTMPP_VB_BLK  blk;

	DBG_IND("\r\n");
	if (!UI_DATEIMPRINT_FLG_ID) {
		DBG_ERR("ID is not installed\r\n");
		return;
	}
	//UiDateImprint_DestroyBuff();

	for (i=0;i<DATE_IMPRINT_EVENT_MAX;i++) {
		pInfo = &pCtrl->info[i];
        if ( i == DATE_IMPRINT_EVENT_QV && g_DateImprintPool[i] == NVTMPP_VB_INVALID_POOL) {

			pInfo->MemSize = 0x80000;
             	g_DateImprintPool[DATE_IMPRINT_EVENT_QV]=nvtmpp_vb_create_pool("DateImprint_QV", pInfo->MemSize, 1, NVTMPP_DDR_1);
                	if (NVTMPP_VB_INVALID_POOL == g_DateImprintPool[DATE_IMPRINT_EVENT_QV])	{
                		DBG_ERR("create private pool err\r\n");
                		return;
                	}
                	blk = nvtmpp_vb_get_block(0, g_DateImprintPool[DATE_IMPRINT_EVENT_QV], pInfo->MemSize, NVTMPP_DDR_1);
                	if (NVTMPP_VB_INVALID_BLK == blk) {
                		DBG_ERR("get vb block err\r\n");
                		return;
                	}
        		pInfo->MemAddr = nvtmpp_vb_block2addr(blk);
			pInfo->copyEngine = GX_IMAGE_CP_ENG1;
			pInfo->lockptn = i;
        }
		else if ( i == DATE_IMPRINT_EVENT_SCR && g_DateImprintPool[i] == NVTMPP_VB_INVALID_POOL) {
			pInfo->MemSize = 0x80000;
             	g_DateImprintPool[DATE_IMPRINT_EVENT_SCR]=nvtmpp_vb_create_pool("DateImprint_SCR", pInfo->MemSize, 1, NVTMPP_DDR_1);
                	if (NVTMPP_VB_INVALID_POOL == g_DateImprintPool[DATE_IMPRINT_EVENT_SCR])	{
                		DBG_ERR("create private pool err\r\n");
                		return;
                	}
                	blk = nvtmpp_vb_get_block(0, g_DateImprintPool[DATE_IMPRINT_EVENT_SCR], pInfo->MemSize, NVTMPP_DDR_1);
                	if (NVTMPP_VB_INVALID_BLK == blk) {
                		DBG_ERR("get vb block err\r\n");
                		return;
                	}
        		pInfo->MemAddr = nvtmpp_vb_block2addr(blk);
			pInfo->copyEngine = GX_IMAGE_CP_ENG2;
			pInfo->lockptn = i;
        }
		else if ( i == DATE_IMPRINT_EVENT_PRI && g_DateImprintPool[i] == NVTMPP_VB_INVALID_POOL) {
        		pInfo->MemSize = 0x80000;
             	g_DateImprintPool[DATE_IMPRINT_EVENT_PRI]=nvtmpp_vb_create_pool("DateImprint_PRI", pInfo->MemSize, 1, NVTMPP_DDR_1);
                	if (NVTMPP_VB_INVALID_POOL == g_DateImprintPool[DATE_IMPRINT_EVENT_PRI])	{
                		DBG_ERR("create private pool err\r\n");
                		return;
                	}
                	blk = nvtmpp_vb_get_block(0, g_DateImprintPool[DATE_IMPRINT_EVENT_PRI], pInfo->MemSize, NVTMPP_DDR_1);
                	if (NVTMPP_VB_INVALID_BLK == blk) {
                		DBG_ERR("get vb block err\r\n");
                		return;
                	}
        		pInfo->MemAddr = nvtmpp_vb_block2addr(blk);
			pInfo->copyEngine = GX_IMAGE_CP_ENG2;
            pInfo->lockptn = i;
		}
		pInfo->isStrDirty = TRUE;
		pInfo->StrBuf[0] = 0;
		pInfo->pic_cnt = 0;
		genMaxw = GetPhotoSizeWidth(0) / 2;
		genMaxh = GetPhotoSizeHeight(0) * 0.04;
		pInfo->StampInfo.Img.LineOffset[0] = genMaxw;
		pInfo->StampInfo.Img.LineOffset[1] = genMaxw;
		pInfo->StampInfo.Img.Width = genMaxw;
		pInfo->StampInfo.Img.Height = genMaxh;
		pInfo->StampInfo.Img.PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422;
	}
	set_flg(UI_DATEIMPRINT_FLG_ID, 0xFFFFFFFF);

}
void UiDateImprint_DestroyBuff(void)
{
	INT32 i, ret;
	for (i=0;i<DATE_IMPRINT_EVENT_MAX;i++) {
		if(g_DateImprintPool[i] != NVTMPP_VB_INVALID_POOL){
			ret = nvtmpp_vb_destroy_pool(g_DateImprintPool[i]);
			if (NVTMPP_ER_OK != ret) {
				DBG_ERR("destory pool error pool 0x%x   ret=%d\r\n", g_DateImprintPool[i], ret);
				return;
			}
			g_DateImprintPool[i]=NVTMPP_VB_INVALID_POOL;
		}
	}
}
static BOOL UiDateImprint_UpdateDate(char *StrBuf, UINT32 buff_len)
{
	char  *str;
	DBG_IND("\r\n");

	DateTime_Load();
	switch (UI_GetData(FL_DATE_STAMP)) {
		case DATEIMPRINT_DATE:
			str = DateTime_MakeYMD();
			if (strncmp(str,StrBuf,strlen(str))) {
				strncpy(StrBuf, str, buff_len - 1);
				return TRUE;
			}
			break;

		case DATEIMPRINT_DATE_TIME:
			str = DateTime_MakeYMDHMS();
			if (strncmp(str,StrBuf,strlen(str))) {
				strncpy(StrBuf, str, buff_len - 1);
				return TRUE;
			}
			break;
		default:
			StrBuf[0] = 0; //Empty String
	}
	return FALSE;
}


UINT32 UiDateImprint_GetMaxWorkBufSize(DS_STAMP_INFOR *stampInfo)
{
    return GxImg_CalcRequireSize(stampInfo->Img.Width,stampInfo->Img.Height,stampInfo->Img.PxlFmt,stampInfo->Img.LineOffset[0]);
}

void UiDateImprint_SelStampFont(UINT32 uiImageWidth, char **font, UINT32 *ScaleFactor)
{
	if (uiImageWidth >= 3840) {
		*font = (char *)gDateStampFontTbl56x96;
		*ScaleFactor = 65536;
	}
	else if(uiImageWidth >=3600) {
		*font = (char *)&gDateStampFontTbl56x96;
		*ScaleFactor = 65536*95/100;
	}
	else if(uiImageWidth >=3200) {
		*font = (char *)&gDateStampFontTbl56x96;
		*ScaleFactor = 65536*90/100;
	}
	else if(uiImageWidth >=2880) {
		*font = (char *)&gDateStampFontTbl42x72;
		*ScaleFactor = 65536;
	}
	else if(uiImageWidth >=1920) {
		*font = (char *)&gDateStampFontTbl36x60;
		*ScaleFactor = 65536;
	}
	else if(uiImageWidth >=1080) {
		*font = (char *)&gDateStampFontTbl26x44;
		*ScaleFactor = 65536;
	}
	else if(uiImageWidth >=640) {
		*font = (char *)&gDateStampFontTbl12x20;
		*ScaleFactor = 65536;
	}
	else if(uiImageWidth >=320) {
		*font = (char *)&gDateStampFontTbl10x16;
		*ScaleFactor = 65536;
	}
	else {
		*font = (char *)&gDateStampFontTbl10x16;
		*ScaleFactor = 65536/3;
	}
}

DATE_IMPRINT_INFO   *UiDateImprint_Event2Info(IMG_CAP_DATASTAMP_EVENT  event)
{
	DATE_IMPRINT_INFO   *pInfo;
	if (event == CAP_DS_EVENT_QV)
		pInfo = &pCtrl->info[DATE_IMPRINT_EVENT_QV];
	else if (event == CAP_DS_EVENT_SCR)
		pInfo = &pCtrl->info[DATE_IMPRINT_EVENT_SCR];
	else if (event == CAP_DS_EVENT_PRI)
		pInfo = &pCtrl->info[DATE_IMPRINT_EVENT_PRI];
    else
		pInfo = NULL;
	return pInfo;
}


static INT32 UiDateImprint_GenYuvData(DATE_IMPRINT_INFO   *pInfo)
{
	FONT_CONV_IN  *pIn =  &pInfo->tYuvDesc;
	FONT_CONV_OUT *pOut = &pInfo->tYuvInfo;
	UINT32 photo_w = pInfo->pCapInfo->ImgInfo.ch[0].width;
	//UINT32 photo_h = pInfo->pCapInfo->ImgInfo.ch[0].height;

    DBG_IND("\r\n");
	pIn->MemAddr = pInfo->MemAddr;
	pIn->MemSize = pInfo->MemSize;
	pIn->pStr = pInfo->StrBuf;
	pIn->ciSolid = DATE_COLOR_RED;
	pIn->ciFrame = DATE_COLOR_TRANSPARENT;
	pIn->ciTransparet = DATE_COLOR_TRANSPARENT;
	pIn->Format = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
	pIn->bEnableSmooth = FALSE;
	//ImgSize = photo_w*photo_h;
    #if 0
	if (photo_w * photo_h <= 160 * 120) {
		//thumbnail Size
		pIn->pFont = (FONT *)gDemo_SmallFont;
		pIn->ScaleFactor = 65536 / 3; //0.33x
	}
	else if (photo_w * photo_h <= 640 * 480) {
		//VGA Size
		pIn->pFont = (FONT *)gDemo_SmallFont;
		pIn->ScaleFactor = 65536; //1.0x
	} else {
		UINT32 font_h, stamp_ratio;
		pIn->pFont = (FONT *)gDemo_BigFont;

		font_h = 190;
		stamp_ratio = 0x00000A00; //0.04x
		pIn->ScaleFactor = photo_h * stamp_ratio / font_h;
	}
	#else
	UiDateImprint_SelStampFont(photo_w, (char**)&pIn->pFont, &pIn->ScaleFactor);
	#endif
	if (FontConv(pIn, pOut) != E_OK) {
		DBG_ERR("FontConv err\r\n");
		return -1;
	}
	return 0;
}

static INT32 UiDateImprint_CalcPosition(DATE_IMPRINT_INFO   *pInfo, UPOINT *pos)
{
	IMG_CAP_DATASTAMP_INFO *pCapInfo = pInfo->pCapInfo;
	FONT_CONV_OUT *pYuvInfo = &pInfo->tYuvInfo;

	UINT32 photo_w = pCapInfo->ImgInfo.ch[0].width;
	UINT32 photo_h = pCapInfo->ImgInfo.ch[0].height;

	if (pYuvInfo->GenImg.Width > photo_w)
	{
		DBG_ERR("GenImg w %d> photo_w %d\r\n",pYuvInfo->GenImg.Width,photo_w);
		return -1;
	}
	if (pYuvInfo->GenImg.Height > photo_h)
	{
		DBG_ERR("GenImg h %d> photo_h %d\r\n",pYuvInfo->GenImg.Height,photo_h);
		return -1;
	}
    DBG_IND("photo_w =%d, photo_h=%d, gen w=%d, h=%d\r\n",photo_w,photo_h,pYuvInfo->GenImg.Width,pYuvInfo->GenImg.Height);
	pos->x = ALIGN_ROUND_4(photo_w - pYuvInfo->GenImg.Width - pYuvInfo->GenImg.Height);
	pos->y = ALIGN_ROUND_4(photo_h - pYuvInfo->GenImg.Height - pYuvInfo->GenImg.Height);
	return 0;
}

static ER UiDateImprint_UpdateData(DATE_IMPRINT_INFO   *pInfo)
{
	DS_STAMP_INFOR  *pStampInfo = &pInfo->StampInfo;
	FONT_CONV_OUT   *pYuvInfo = &pInfo->tYuvInfo;
	UPOINT           pos = {0};

	if (UiDateImprint_GenYuvData(pInfo)< 0){
		return E_SYS;
	}
	if (UiDateImprint_CalcPosition(pInfo,&pos) < 0){
		return E_SYS;
	}
	pStampInfo->PosX = pos.x;
	pStampInfo->PosY = pos.y;
	pStampInfo->Img = pYuvInfo->GenImg;
	pStampInfo->ColorKey = COLOR_MAKE_YUVD(pYuvInfo->ColorKeyY, pYuvInfo->ColorKeyCb, pYuvInfo->ColorKeyCr);
	pStampInfo->StampWeight = 255;
	DBG_IND("MemMax Use = %d bytes, MemMin Use=%d bytes\r\n", pYuvInfo->UsedMaxMemSize, pYuvInfo->UsedMemSize);
	return E_OK;
}

static void UiDateImprint_ChkUpdateData(DATE_IMPRINT_INFO   *pInfo)
{
	if (pInfo->pCapInfo->event == CAP_DS_EVENT_QV || pInfo->pCapInfo->event == CAP_DS_EVENT_PRI) {
 		if ((UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_BURST_3)
			#if defined(_BSP_NA51000_)
			|| (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_BURST_30)
			#endif
			) {
			if (pInfo->pic_cnt == 0)
				pInfo->isStrDirty = UiDateImprint_UpdateDate(pInfo->StrBuf,sizeof(pInfo->StrBuf));
			else
				pInfo->isStrDirty = FALSE;
		}
		else {
			pInfo->isStrDirty = UiDateImprint_UpdateDate(pInfo->StrBuf,sizeof(pInfo->StrBuf));
		}
	}
	// need to update primary info to screenail
	if (pInfo->pCapInfo->event == CAP_DS_EVENT_PRI)
	{
		DATE_IMPRINT_INFO   *pScrInfo;

		pScrInfo= UiDateImprint_Event2Info(CAP_DS_EVENT_SCR);
		pScrInfo->isStrDirty = pInfo->isStrDirty;
	    if (pInfo->isStrDirty == TRUE)
			strncpy(pScrInfo->StrBuf,pInfo->StrBuf,sizeof(pScrInfo->StrBuf));
	}
	pInfo->pic_cnt++;
}
static ER UiDateImprint_CopyData(DS_STAMP_INFOR *stampInfo,PIMG_BUF Img, GX_IMAGE_CP_ENG  copyEngine)
{
	//UINT32          BufSize;
    //IMG_BUF         tmpImg = {0};
    IRECT  srcRegion;
    IPOINT DstLocation;
    ER     ret =E_OK;


    DBG_IND("\r\n");

    #if 0
	BufSize = UiDateImprint_GetMaxWorkBufSize(stampInfo);
    if (stampInfo->StampWeight < 255 && BufSize > WorkBufSize)
    {
        DBG_ERR("WorkBufSize too small 0x%.8x > 0x%.8x\r\n", BufSize, WorkBufSize);
        return E_SYS;
    }
	#endif


    // the StampWeight is a alphaPlane Pointer
    if (stampInfo->StampWeight > 255)
    {
        UINT8*    alphaPlane = (UINT8*)stampInfo->StampWeight;

        DstLocation.x = stampInfo->PosX;
        DstLocation.y = stampInfo->PosY;
        ret = GxImg_CopyBlendingDataEx(&stampInfo->Img,REGION_MATCH_IMG,Img,&DstLocation, alphaPlane);
    }
    else if (stampInfo->StampWeight == 255)
    {
        srcRegion.x = stampInfo->PosX;
        srcRegion.y = stampInfo->PosY;
        srcRegion.w = stampInfo->Img.Width;
        srcRegion.h = stampInfo->Img.Height;
        DstLocation.x = stampInfo->PosX;
        DstLocation.y = stampInfo->PosY;
		ret = GxImg_CopyColorkeyData(Img,&srcRegion,&stampInfo->Img,REGION_MATCH_IMG,stampInfo->ColorKey,FALSE, copyEngine);
    }
    else
    {
		DBG_ERR("Not support StampWeight = %d\r\n",stampInfo->StampWeight);
		/*
        srcRegion.x = stampInfo->PosX;
        srcRegion.y = stampInfo->PosY;
        srcRegion.w = stampInfo->Img.Width;
        srcRegion.h = stampInfo->Img.Height;
		DstLocation.x = stampInfo->PosX;
        DstLocation.y = stampInfo->PosY;
        GxImg_InitBuf(&tmpImg,stampInfo->Img.Width,stampInfo->Img.Height,stampInfo->Img.PxlFmt,stampInfo->Img.Width,WorkBuf,WorkBufSize);
        DBG_MSG("w= %d, h=%d, colorkey=0x%x\r\n",tmpImg.Width,tmpImg.Height,stampInfo->ColorKey);
        ret = GxImg_CopyData(Img,&srcRegion,&tmpImg,REGION_MATCH_IMG);
        if (ret == E_OK)
        {
            //(B == KEY) ? A:B -> destination with color key (=)
            ret = GxImg_CopyColorkeyData(&tmpImg,REGION_MATCH_IMG,&stampInfo->Img,REGION_MATCH_IMG,stampInfo->ColorKey,FALSE, copyEngine);
            if (ret == E_OK)
            {
                ret = GxImg_CopyBlendingData(&tmpImg,REGION_MATCH_IMG,Img, &DstLocation,stampInfo->StampWeight, copyEngine);
            }
        }
        */
    }
    return ret;
}






BOOL UiDateImprint_GenData(IMG_CAP_DATASTAMP_INFO *pCapInfo)
{
	DATE_IMPRINT_INFO   *pInfo;
	IMG_BUF Img = {0};
    UINT32  LineOff[MAX_PLANE_NUM];
    UINT32  PxlAddr[MAX_PLANE_NUM];
#if USE_DBG_STAMP
	UINT32  time_start,time_end;
#endif

	DBG_IND("event %d, width =%d, height =%d\r\n",pCapInfo->event,pCapInfo->ImgInfo.ch[0].width,pCapInfo->ImgInfo.ch[0].height);

	pInfo = UiDateImprint_Event2Info(pCapInfo->event);
	if (pInfo == NULL){
		DBG_ERR("Unknown event %d\r\n",pCapInfo->event);
		return FALSE;
	}
	UiDateImprint_Lock(pInfo->lockptn);
	pInfo->pCapInfo = pCapInfo;
	UiDateImprint_ChkUpdateData(pInfo);
	DBG_IND("isStrDirty %d\r\n",pInfo->isStrDirty);
#if USE_DBG_STAMP
	time_start = Perf_GetCurrent();
#endif
	if (pInfo->isStrDirty) {
		if (UiDateImprint_UpdateData(pInfo)!=E_OK)
			return FALSE;
	}
#if 0
	GxImg_DumpBuf(&pYuvInfo->GenImg, TRUE);
#endif
	PxlAddr[0] = pCapInfo->ImgInfo.pixel_addr[0];
    PxlAddr[1] = pCapInfo->ImgInfo.pixel_addr[1];
    LineOff[0] = pCapInfo->ImgInfo.ch[0].line_ofs;
    LineOff[1] = pCapInfo->ImgInfo.ch[1].line_ofs;
    GxImg_InitBufEx(&Img, pCapInfo->ImgInfo.ch[0].width, pCapInfo->ImgInfo.ch[0].height, GX_IMAGE_PIXEL_FMT_YUV422_PACKED, LineOff, PxlAddr);
	UiDateImprint_CopyData(&pInfo->StampInfo,&Img,pInfo->copyEngine);

#if USE_DBG_STAMP
    time_end = Perf_GetCurrent();
	DBG_DUMP("Time Use = %d ms\r\n", (time_end-time_start) / 1000);
#endif
	UiDateImprint_UnLock(pInfo->lockptn);

	return TRUE;
}




