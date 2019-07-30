#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "SysCfg.h"
#include "Utility.h"
#include "HwClock.h"
#include "GxImage.h"
#include "MovieStamp.h"
#include "alg_MovieStamp.h"
#include "DateStampFont10x16.h"
#include "DateStampFont12x20.h"
#include "DateStampFont18x30.h"
#include "DateStampFont20x44.h"
#include "DateStampFont26x44.h"
#if (MOVIE_FR_FUNC== ENABLE)
#include "DateStampFont_WautoH44.h"
#endif
#include "iq_utility.h"
#include "timer.h"
#include "SwTimer.h"
#include "MovieStampAPI.h"

#if _TODO
#include "adas_Apps.h"     //#NT#2016/04/13#Correct the include file#KCHong
#include "fcws_lib.h"
#endif
#include "PrjCfg.h"
#include "UIInfo.h"
#include "UIPhotoFuncInfo.h"

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
#include "UniFont.h"
#endif

#include "dma.h"
#include "grph.h"
#include "VideoSpriteIpc.h"
#include "MovieStampID.h"



//#NT#2016/05/23#David Tsai -begin
//#NT# Support tampering detection function

//#NT#2016/03/24#KCHong -begin
//#NT#Log ADAS/IPP info in GPS section
#include "UIAppMovie.h"
//#NT#2016/03/24#KCHong -end
#if (IPCAM_FUNC)
#include <HwMem.h>
#include "UIControl.h"
#include "UIControlWnd.h"
#endif
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

#if MOVIE_AE_LOG
#include "NMediaRecVdoEnc.h"
#endif
#if 1//(STAMP_PRIVATE_POOL == ENABLE)
#include "nvtmpp.h"
#endif

#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
#include "videosprite.h"
#include "nvtvideosprite_protected.h"
#include "FontConv.h"
#include "DateStampFontTbl10x16.h"
#include "DateStampFontTbl12x20.h"
#include "DateStampFontTbl18x30.h"
#include "DateStampFontTbl20x44.h"
#include "DateStampFontTbl26x44.h"
#endif
//#NT#2016/10/17#Bin Xiao -end
#define __MODULE__              MovieStamp
#define __DBGLVL__              2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__              "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define swap(x,y) {int t; t=x; x=y; y=t; }

#define CHECK_STAMP_TIME        DISABLE
#define CHECK_ENC_INFO          DISABLE
//#define WATERLOGO_FUNCTION      DISABLE // TEMPORARY!     //#NT#2015/11/04#Move this definition to PrjCfg_xxx.h#KCHong

#define COLOR_IDX_2BIT          0   // 2-bit color index
#define COLOR_IDX_4BIT          1   // 4-bit color index
#define COLOR_IDX_MODE          COLOR_IDX_2BIT

#define COLOR_BG_Y              0x00
#define COLOR_BG_U              0x80
#define COLOR_BG_V              0x80
#define COLOR_FR_Y              0x00
#define COLOR_FR_U              0x80
#define COLOR_FR_V              0x80
#define COLOR_FG_Y              RGB_GET_Y(250, 160, 10)
#define COLOR_FG_U              RGB_GET_U(250, 160, 10)
#define COLOR_FG_V              RGB_GET_V(250, 160, 10)
#define COLOR_CK_Y              0x01
#define COLOR_CK_U              0x01
#define COLOR_CK_V              0x01
#define COLOR_ID_BG             0
#define COLOR_ID_FR             1
#define COLOR_ID_FG             2
#define COLORKEY_BG_YUV         0x00000000  // stamp BG data for color key operation
#define COLORKEY_YUV            0x00010101  // color key for transparent background

#define STAMP_WIDTH_MAX         1920        // maximum stamp width (for stamp buffer line-offset setting)
#define STAMP_HEIGHT_MAX         64
#if (THUMB_STAMP_FUNC == ENABLE)
#define THUMB_STAMP_WIDTH_MAX   640
#endif
#define STAMP_WIDTH_TOLERANCE   8           // total font width error tolerance
#define STAMP_LOGO_GAP          8           // date stamp and water logo position gap
#define VIDEO_IN_MAX            13//5           // max video paths, main:0,2,4,6; clone:1,3,5,7; wifi: 12
#define MOVIE_STAMP_BUF_0       0
#define MOVIE_STAMP_BUF_1       1
#define MOVIE_STAMP_CHK_TIME    50          // 50ms check once
#define MOVIE_STAMP_MAX_LEN     256
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
#define LINGUAL_MAX               32
#define LINGUAL_STAMP_BUF_0       0
#define LINGUAL_STAMP_BUF_1       1
#define LINGUAL_STAMP_BUF_TATAL   2
#define LINGUAL_STAMP_BUF_OFF     0xFFFFFFFF
#endif
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
#define VSFONT_BUF_RATIO  (550)
#endif
#define ALIGN_CEIL_2(a)         ALIGN_CEIL(a, 4)    ///< Round Up to 4

//variable declare
static STAMP_POS    g_MovieStampPos[VIDEO_IN_MAX] = {
	{0, 0}, // stamp position for primary image 1
	{0, 0},  // stamp position for primary image 2
	{0, 0}
};
char         g_cMovieStampStr[VIDEO_IN_MAX][MOVIE_STAMP_MAX_LEN];
static UINT32       g_uiMovieStampSetup[VIDEO_IN_MAX] = {STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,
                                                                                      STAMP_OFF,};
static STAMP_INFO   g_MovieStampInfo[VIDEO_IN_MAX];
static struct tm    g_CurDateTime;
static UINT32       g_uiMovieStampActBuf = MOVIE_STAMP_BUF_0;   // movie stamp active buffer
//#NT#2016/08/19#Hideo Lin -begin
//#NT#To avoid different stamp overlap for same source image
static UINT32       g_uiMovieSrcYAddr[VIDEO_IN_MAX] = {0};
//#NT#2016/08/19#Hideo Lin -end
static UINT32       g_uiMovieStampYAddr[VIDEO_IN_MAX][2] = {0}; // movie stamp Y address (2 means double buffer)
static UINT32       g_uiMovieStampSize[VIDEO_IN_MAX] = {0}; // buffer size for movie stamp
static UINT32       g_uiMovieStampWidth[VIDEO_IN_MAX] = {0};    // movie stamp data width
static UINT32       g_uiWaterLogoYAddr[VIDEO_IN_MAX] = {0}, g_uiWaterLogoUVAddr[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualStampYAddr[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualScaledStampYAddr[VIDEO_IN_MAX][2] = {0};      // (2 means double buffer)
//static UINT32       g_uiMovieImageWidth[VIDEO_IN_MAX] = {0};    // movie image width
static UINT32       g_uiMovieStampFormat[VIDEO_IN_MAX] = {STAMP_FORMAT_YUV};  // movie stamp format (YUV or color index)
//static UINT32       g_uiMovieStampHeightMax = 44; // maximum height of date stamp font (gDateStampFont26x44)
static BOOL         g_bStampColorSetup[VIDEO_IN_MAX] = {FALSE};
static BOOL         g_bMovieStampSwTimerOpen = FALSE;
static SWTIMER_ID   g_MovieStampSwTimerID;
static WATERLOGO_BUFFER g_MovieWaterInfo[VIDEO_IN_MAX] = {0};
static BOOL         g_bWaterLogoEnable[VIDEO_IN_MAX] = {0};
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
static UINT32       g_uiLingualStampActBuf = LINGUAL_STAMP_BUF_OFF;          // Set lignual stamp active buffer to disable
static STAMP_POS    g_LingualStampPos[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualStampWidth[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualScaledStampWidth[VIDEO_IN_MAX][2] = {0};
static UINT32       g_uiLingualStampHeight = 0;
static UINT32       g_uiLingualScaledStampHeight = 0;
static UINT32       g_uiLingualStampScalingFactor[VIDEO_IN_MAX] = {44, 44, 44,44, 44, 44,44, 44, 44,44, 44,44, 44};

// sample unicode string
UINT16 unicode[] = {
	0x806f, 0x8a60, 0x79d1, 0x6280, 0x80a1, 0x4efd, 0x6709, 0x9650, 0x516c, 0x53f8
};

LINGUAL_INFO    LingualInfo[LINGUAL_MAX];
#endif //

#if (THUMB_STAMP_FUNC == ENABLE)
static STAMP_POS    g_ThumbStampPos = {0, 0};
char         		g_cThumbStampStr[MOVIE_STAMP_MAX_LEN];
static UINT32		g_uiThumbStampSetup = STAMP_OFF;
static STAMP_INFO   g_ThumbStampInfo = {0};
static UINT32		g_uiThumbStampYAddr = 0;
static UINT32       g_uiThumbStampWidth = 0;
static UINT32       g_uiThumbStampFormat = STAMP_FORMAT_YUV;  // thumb stamp format (YUV or color index)
static BOOL			g_bThumbStampColorSetup = FALSE;
#if (STAMP_PRIVATE_POOL == ENABLE)
static NVTMPP_VB_POOL g_MovieStampThumbPool[1]={NVTMPP_VB_INVALID_POOL};
static NVTMPP_VB_POOL g_MovieStampThumbPoolAddr[1]={0};
#endif
#endif

#if (STAMP_PRIVATE_POOL == ENABLE)
static NVTMPP_VB_POOL g_MovieStampPool[VIDEO_IN_MAX]=
                                                                                        {NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL};
static UINT32 g_MovieStampPoolAddr[VIDEO_IN_MAX]={0};
#endif

#if (MOVIE_FR_FUNC== ENABLE)
#define FACE_CNT_MAX           5           // max 5 Face

static NVTMPP_VB_POOL g_FaceNameStampPool[FACE_CNT_MAX]=
                                                                                        {NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                            NVTMPP_VB_INVALID_POOL,
                                                                                           };
static UINT32 g_FaceNameStampPoolAddr[FACE_CNT_MAX]={0};
#endif

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
UINT32 g_DDD_DowsyState;
//#NT#2016/06/02#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
void Movie_DrawDDD(PIMG_BUF pDstImg);
//#NT#2016/06/02#Nestor Yang -end
#endif
//#NT#2016/04/22#Nestor Yang -end
//#NT#2018/03/22#Brain Yen -begin
//#NT#add videosprite stamp function
static UINT32 g_uiVsStampAddr[VIDEO_IN_MAX] = {0};
static UINT32 g_uiVsFontAddr[VIDEO_IN_MAX] = {0};
static UINT32 g_VsStampEn=FALSE;
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
UINT32 g_VsOsdWidth[VIDEO_IN_MAX] = {0};
UINT32 g_VsOsdHight[VIDEO_IN_MAX] = {0};
static UINT32 g_uiVsStampSize = 0;
static UINT32 g_uiVsFontSize = 0;
FONT_CONV_IN g_VsFontIn[VIDEO_IN_MAX]={0};
FONT_CONV_OUT g_VsFontOut[VIDEO_IN_MAX]={0};
UINT16* g_pVsFontDataAddr[VIDEO_IN_MAX];
#endif
//#NT#2018/03/22#Brain Yen -end

void MovieStamp_InitImeStamp(UINT32 uiVidEncId);

#if MOVIE_FTG_FUNC
//#NT#2016/12/07#Charlie Chang -begin
//#NT# for FTG draw frame count
UINT32 g_ftg_draw_delay_count = 5;  //how many frame update ftg infomation
UINT32 g_ftg_rlst_type = ftg_rlst_all_info;
#endif
//#NT#2016/12/07#Charlie Chang -end

//-------------------------------------------------------------------------------------------------
static void MovieStamp_SwTimerHdl(UINT32 uiEvent)
{
	struct tm   CurDateTime;

	CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);

	// check time varying
#if MOVIE_AE_LOG || MOVIE_ADAS_LOG
	if (1)
#else
	if ((g_CurDateTime.tm_sec  != CurDateTime.tm_sec)  ||
		(g_CurDateTime.tm_min  != CurDateTime.tm_min)  ||
		(g_CurDateTime.tm_hour != CurDateTime.tm_hour) ||
		(g_CurDateTime.tm_mday != CurDateTime.tm_mday) ||
		(g_CurDateTime.tm_mon  != CurDateTime.tm_mon)  ||
		(g_CurDateTime.tm_year != CurDateTime.tm_year))
#endif
	{
		g_CurDateTime = CurDateTime;

		// time varied, update stamp database
		MovieStampTsk_TrigUpdate();
	}
}

static void MovieStamp_SwTimerOpen(void)
{
	if (g_bMovieStampSwTimerOpen == FALSE) {
		if (SwTimer_Open(&g_MovieStampSwTimerID, MovieStamp_SwTimerHdl) != E_OK) {
			DBG_ERR("Sw timer open failed!\r\n");
			return;
		}

		SwTimer_Cfg(g_MovieStampSwTimerID, MOVIE_STAMP_CHK_TIME, SWTIMER_MODE_FREE_RUN);
		SwTimer_Start(g_MovieStampSwTimerID);
		g_bMovieStampSwTimerOpen = TRUE;
	}
}

static void MovieStamp_SwTimerClose(void)
{
	if (g_bMovieStampSwTimerOpen) {
		SwTimer_Stop(g_MovieStampSwTimerID);
		SwTimer_Close(g_MovieStampSwTimerID);
		g_bMovieStampSwTimerOpen = FALSE;
	}
}

void MovieStamp_Enable(void)
{
	g_CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);

	//#NT#2016/08/19#Hideo Lin -begin
	//#NT#To avoid different stamp overlap for same source image
	UINT32 i;
	for (i = 0; i < VIDEO_IN_MAX; i++) {
		g_uiMovieSrcYAddr[i] = 0;
	}
	//#NT#2016/08/19#Hideo Lin -end

#if (THUMB_STAMP_FUNC == ENABLE)
	MovieStamp_UpdateData_Thumb(0, TRUE);
#endif

	MovieStamp_UpdateData();

	// register movie stamp update callback
	MovieStampTsk_RegUpdateCB(MovieStamp_UpdateData);
	// open movie stamp task to wait for stamp update flag
	MovieStampTsk_Open();

	// use SW timer to check current time
	MovieStamp_SwTimerOpen();
}

void MovieStamp_Disable(void)
{
    	UINT32 i;
	// close SW timer
	MovieStamp_SwTimerClose();

	// close movie stamp task
	MovieStampTsk_Close();

	MovieStamp_VsClose();
    	for (i = 0; i < VIDEO_IN_MAX; i++) {
        	MovieStamp_Setup(
        		i,
        		STAMP_OFF,
        		0,
        		0,
        		NULL);
    	}
    	//MovieStamp_Setup_Thumb (i, STAMP_OFF, 0, 0, NULL);
}

//-------------------------------------------------------------------------------------------------
void MovieStamp_Setup(UINT32 uiVidEncId, UINT32 uiFlag, UINT32 uiImageWidth, UINT32 uiImageHeight, WATERLOGO_BUFFER *pWaterLogoBuf)
{
	PSTAMP_INFO     pStampInfo;
	ICON_DB const   *pDB;
	UINT32          uiIconID;
	UINT32          uiStrOffset;

	g_uiMovieStampFormat[uiVidEncId] = STAMP_FORMAT_YUV;
	g_uiMovieStampSetup[uiVidEncId] = uiFlag;

	if ((uiFlag & STAMP_SWITCH_MASK) == STAMP_OFF) {
		return;
	}

	g_bWaterLogoEnable[uiVidEncId] = pWaterLogoBuf ? TRUE : FALSE;

#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
	{
		if (g_bWaterLogoEnable[uiVidEncId]) {
			// setup water logo
			memcpy((void *)&g_MovieWaterInfo[uiVidEncId], (const void *)pWaterLogoBuf, sizeof(WATERLOGO_BUFFER));
			g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr = g_uiWaterLogoYAddr[uiVidEncId];
			g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr = g_uiWaterLogoUVAddr[uiVidEncId];
		}
	}
#endif

	pStampInfo = &g_MovieStampInfo[uiVidEncId];
	pStampInfo->pi8Str = &g_cMovieStampStr[uiVidEncId][0];

	// set date stamp font data base
	switch (uiImageWidth) {
    case 3840:  // 3840x2160
        pStampInfo->pDataBase = &gDateStampFont26x44;
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
		g_VsFontIn[uiVidEncId].pFont=(FONT *)gDateStampFontTbl26x44;
#endif
        break;
	case 2560:  // 2560x1440
	case 2304:  // 2304x1296
	case 1920:  // 1920x1080
	case 1536:  // 1536x1536

#if MOVIE_AE_LOG || MOVIE_ADAS_LOG
		pStampInfo->pDataBase = &gDateStampFont12x20;
#else
		pStampInfo->pDataBase = &gDateStampFont26x44;
#endif
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    #if MOVIE_AE_LOG || MOVIE_ADAS_LOG
		g_VsFontIn[uiVidEncId].pFont=(FONT *)gDateStampFontTbl12x20;
    #else
		g_VsFontIn[uiVidEncId].pFont=(FONT *)gDateStampFontTbl26x44;
    #endif
#endif
		break;

	case 2880:  // 2880x2160 (DAR 16:9)
	case 1728:  // 1728x1296 (DAR 16:9)
	case 1440:  // 1440x1080 (DAR 16:9)
		pStampInfo->pDataBase = &gDateStampFont20x44;
		g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 3) / 4;
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
		g_VsFontIn[uiVidEncId].pFont=(FONT *)gDateStampFontTbl20x44;
#endif
		break;

	case 1280:  // 1280x720
		pStampInfo->pDataBase = &gDateStampFont18x30;
		g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 30) / 44;
		g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 30) / 44;
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
		g_VsFontIn[uiVidEncId].pFont=(FONT *)gDateStampFontTbl18x30;
#endif
		break;

	case 320:   // QVGA
		pStampInfo->pDataBase = &gDateStampFont10x16;
		g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 16) / 44;
		g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 16) / 44;
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
		g_VsFontIn[uiVidEncId].pFont=(FONT *)gDateStampFontTbl10x16;
#endif
		break;

	default:    // VGA & others
		pStampInfo->pDataBase = &gDateStampFont12x20;
		g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 20) / 44;
		g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 20) / 44;
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
		g_VsFontIn[uiVidEncId].pFont=(FONT *)gDateStampFontTbl12x20;
#endif
		break;
	}

#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE) && (VIDEOSPRITE_STAMP_FUNC == DISABLE)
	{
		if (g_bWaterLogoEnable[uiVidEncId]) {
			// do water logo scaling
			UINT32      uiLineOffset[2] = {0};
			UINT32      uiPxlAddr[2] = {0};
			UINT32      uiScaleYAddr, uiScaleUVAddr;
			IMG_BUF     SrcImg, DstImg;
			IRECT       SrcRegion = {0};
			IRECT       DstRegion = {0};
			BOOL        bScale = TRUE;

			g_MovieWaterInfo[uiVidEncId].uiWidth = ALIGN_CEIL_4(g_MovieWaterInfo[uiVidEncId].uiWidth);
			g_MovieWaterInfo[uiVidEncId].uiHeight = ALIGN_CEIL_4(g_MovieWaterInfo[uiVidEncId].uiHeight);

			if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_YUV) {
				// D2D: scale logo data to stamp buffer
				uiScaleYAddr = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
				uiScaleUVAddr = g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr;
			} else {
				// IME direct: use temp buffer to do scaling, then transfer YUV422 UV-packed to YUV422 YUYV-packed
				// use date time stamp buffer for logo scaling since it's not in use at this moment
				uiScaleYAddr = g_uiMovieStampYAddr[uiVidEncId][0];
				uiScaleUVAddr = uiScaleYAddr + g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset * g_MovieWaterInfo[uiVidEncId].uiHeight;
			}

			if (bScale) {
				// Init source buffer
				uiLineOffset[0] = pWaterLogoBuf->uiWaterLogoYLineOffset;
				uiLineOffset[1] = pWaterLogoBuf->uiWaterLogoYLineOffset;
				uiPxlAddr[0]    = pWaterLogoBuf->uiWaterLogoYAddr;
				uiPxlAddr[1]    = pWaterLogoBuf->uiWaterLogoUVAddr;

				GxImg_InitBufEx(&SrcImg,
								pWaterLogoBuf->uiWidth,
								pWaterLogoBuf->uiHeight,
								GX_IMAGE_PIXEL_FMT_YUV422_PACKED,
								uiLineOffset,
								uiPxlAddr);

				// Init destination buffer
				uiLineOffset[0] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
				uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
				uiPxlAddr[0]    = uiScaleYAddr;
				uiPxlAddr[1]    = uiScaleUVAddr;

				GxImg_InitBufEx(&DstImg,
								g_MovieWaterInfo[uiVidEncId].uiWidth,
								g_MovieWaterInfo[uiVidEncId].uiHeight,
								GX_IMAGE_PIXEL_FMT_YUV422_PACKED,
								uiLineOffset,
								uiPxlAddr);

				// Set scale region
				SrcRegion.x = 0;
				SrcRegion.y = 0;
				SrcRegion.w = pWaterLogoBuf->uiWidth;
				SrcRegion.h = pWaterLogoBuf->uiHeight;
				//DBG_DUMP("^YLogo Src: x %d, y %d, w %d, h %d\r\n", SrcRegion.x, SrcRegion.y, SrcRegion.w, SrcRegion.h);

				DstRegion.x = 0;
				DstRegion.y = 0;
				DstRegion.w = g_MovieWaterInfo[uiVidEncId].uiWidth;
				DstRegion.h = g_MovieWaterInfo[uiVidEncId].uiHeight;
				//DBG_DUMP("^YLogo Dst: x %d, y %d, w %d, h %d\r\n", DstRegion.x, DstRegion.y, DstRegion.w, DstRegion.h);

				// Scale image by ISE
				GxImg_ScaleData(&SrcImg, &SrcRegion, &DstImg, &DstRegion);
			}

			if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_IDX) {
				// IME direct: YUV422 UV-packed to YUV422 YUYV-packed
				GRPH_REQUEST request = {0};
				GRPH_IMG imgA = {0};
				GRPH_IMG imgB = {0};
				GRPH_IMG imgC = {0};

				// setup image A/B/C
				imgA.imgID = GRPH_IMG_ID_A;
				imgA.uiAddress = uiScaleYAddr;
				imgA.uiLineoffset = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
				imgA.uiWidth = g_MovieWaterInfo[uiVidEncId].uiWidth;
				imgA.uiHeight = g_MovieWaterInfo[uiVidEncId].uiHeight;
				imgA.pNext = &imgB;
				imgB.imgID = GRPH_IMG_ID_B;
				imgB.uiAddress = uiScaleUVAddr;
				imgB.uiLineoffset = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
				imgB.pNext = &imgC;
				imgC.imgID = GRPH_IMG_ID_C;
				imgC.uiAddress = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
				imgC.uiLineoffset = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset * 2;
				imgC.pNext = NULL;

				request.command = GRPH_CMD_PACKING;
				request.format = GRPH_FORMAT_8BITS;
				request.pImageDescript = &imgA;

				graph_open(GRPH_ID_1);
				graph_request(GRPH_ID_1, &request);
				graph_close(GRPH_ID_1);
			}
		}
	}
#endif

	// set stamp string (for calculating stamp position)
	switch (uiFlag & STAMP_DATE_TIME_MASK) {
	case STAMP_DATE_TIME_AMPM:
		//sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00 AM");
		snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00 AM");
		break;

	case STAMP_DATE: // date only is not suitable for movie stamp (it's suitable for still image stamp)
	case STAMP_TIME:
		//sprintf(pStampInfo->pi8Str, "00:00:00");
		snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "00:00:00");
		break;

	case STAMP_DATE_TIME:
	default:
		//sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00");
		snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00");
		break;
	}

	// set font width and height (use the width of 1st font, so the total width may have some error)
	pDB = pStampInfo->pDataBase;
	uiStrOffset = pDB->uiDrawStrOffset;
	uiIconID = pStampInfo->pi8Str[0] - uiStrOffset; // 1st font
	pStampInfo->ui32FontWidth  = pDB->pIconHeader[uiIconID].uiWidth;
	pStampInfo->ui32FontHeight = pDB->pIconHeader[uiIconID].uiHeight;
	pStampInfo->ui32DstHeight  = pStampInfo->ui32FontHeight; // no scaling

	// Set date stamp position
	if ((uiFlag & STAMP_OPERATION_MASK) == STAMP_AUTO) {
		UINT32  uiStampWidth = (pStampInfo->ui32DstHeight * pStampInfo->ui32FontWidth) / pStampInfo->ui32FontHeight;

		switch (uiFlag & STAMP_POSITION_MASK) {
		case STAMP_TOP_LEFT:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_MovieStampPos[uiVidEncId].uiX = 0;
				g_MovieStampPos[uiVidEncId].uiY = 0;
			} else {
#if 0
				g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
				g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
#else
				g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
				g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
#endif
			}
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
			{
				g_LingualStampPos[uiVidEncId].uiX = uiStampWidth * (strlen(pStampInfo->pi8Str) + 16);
				g_LingualStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2;
			}
#endif
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
			if (g_bWaterLogoEnable[uiVidEncId]) {
				if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS) {
					g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX;
					g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY;
					g_MovieStampPos[uiVidEncId].uiX += (g_MovieWaterInfo[uiVidEncId].uiWidth + STAMP_LOGO_GAP);
				}
			}
#endif
			break;

		case STAMP_TOP_RIGHT:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
				g_MovieStampPos[uiVidEncId].uiY = 0;
			} else {
#if 0
				g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
				g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
#else
				g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
				g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
#endif
			}
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
			{
				g_LingualStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
				g_LingualStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2;
			}
#endif
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
			if (g_bWaterLogoEnable[uiVidEncId]) {
				if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS) {
					g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX - g_MovieWaterInfo[uiVidEncId].uiWidth - STAMP_LOGO_GAP;
					g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY;
				}
			}
#endif
			break;

		case STAMP_BOTTOM_LEFT:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_MovieStampPos[uiVidEncId].uiX = 0;
				g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
			} else {
#if 0
				g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
				g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
#else
				g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
				g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
#endif
			}
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
			{
				g_LingualStampPos[uiVidEncId].uiX = uiStampWidth * (strlen(pStampInfo->pi8Str) + 16); // 1 font width gap
				g_LingualStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2;
			}
#endif
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
			if (g_bWaterLogoEnable[uiVidEncId]) {
				if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS) {
					g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX;
					g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY + pStampInfo->ui32DstHeight - g_MovieWaterInfo[uiVidEncId].uiHeight;
					g_MovieStampPos[uiVidEncId].uiX += (g_MovieWaterInfo[uiVidEncId].uiWidth + STAMP_LOGO_GAP);
				}
			}
#endif
			break;

		case STAMP_BOTTOM_RIGHT:
		default:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
				g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
			} else {
#if 0
				g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
				g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
#else
				g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
				g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
#endif
			}
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
			{
				g_LingualStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
				g_LingualStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2;
			}
#endif
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
			if (g_bWaterLogoEnable[uiVidEncId]) {
				if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS) {
					g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX - g_MovieWaterInfo[uiVidEncId].uiWidth - STAMP_LOGO_GAP;
					g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY + pStampInfo->ui32DstHeight - g_MovieWaterInfo[uiVidEncId].uiHeight;
				}
			}
#endif
			break;
		}
	}

#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
	//this is for DDD debug
	g_MovieStampPos[uiVidEncId].uiX -= (pStampInfo->ui32DstHeight * pStampInfo->ui32FontWidth) / pStampInfo->ui32FontHeight;;
#endif

	g_MovieStampPos[uiVidEncId].uiX = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiX);
	g_MovieStampPos[uiVidEncId].uiY = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiY);

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
	g_LingualStampPos[uiVidEncId].uiX = ALIGN_FLOOR_4(g_LingualStampPos[uiVidEncId].uiX);
	g_LingualStampPos[uiVidEncId].uiY = ALIGN_FLOOR_4(g_LingualStampPos[uiVidEncId].uiY);
#endif

#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
	if (g_bWaterLogoEnable[uiVidEncId]) {
		g_MovieWaterInfo[uiVidEncId].uiXPos = ALIGN_FLOOR_4(g_MovieWaterInfo[uiVidEncId].uiXPos);
		g_MovieWaterInfo[uiVidEncId].uiYPos = ALIGN_FLOOR_4(g_MovieWaterInfo[uiVidEncId].uiYPos);
	}
#endif

	// set default stamp color if necessary
	if (g_bStampColorSetup[uiVidEncId] == FALSE) {
		// Stamp background color
		pStampInfo->Color[COLOR_ID_BG].ucY = COLOR_BG_Y;
		pStampInfo->Color[COLOR_ID_BG].ucU = COLOR_BG_U;
		pStampInfo->Color[COLOR_ID_BG].ucV = COLOR_BG_V;

		// Stamp frame color
		pStampInfo->Color[COLOR_ID_FR].ucY = COLOR_FR_Y;
		pStampInfo->Color[COLOR_ID_FR].ucU = COLOR_FR_U;
		pStampInfo->Color[COLOR_ID_FR].ucV = COLOR_FR_V;

		// Stamp foreground color (text body)
		pStampInfo->Color[COLOR_ID_FG].ucY = COLOR_FG_Y;
		pStampInfo->Color[COLOR_ID_FG].ucU = COLOR_FG_U;
		pStampInfo->Color[COLOR_ID_FG].ucV = COLOR_FG_V;
	}

	// init IME stamp data
	if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_IDX) {
		MovieStamp_InitImeStamp(uiVidEncId);
	}

	// set image width
	//g_uiMovieImageWidth[uiVidEncId] = uiImageWidth;

	// set active image buffer
	g_uiMovieStampActBuf = MOVIE_STAMP_BUF_0;

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
	g_uiLingualStampActBuf = LINGUAL_STAMP_BUF_OFF;
#endif

	// Reset reference time
	g_CurDateTime.tm_sec = 61;

#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    	ISIZE szStamp;
    #if MOVIE_AE_LOG
	////<< spark add AE function
	IQS_GetLogString(uiVidEncId, &g_cMovieStampStr[uiVidEncId][0], 256);
	//NMR_VdoEnc_GetRC(i, &g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN);
    #endif

    #if MOVIE_ADAS_LOG
	//#NT#2016/03/25#KCHong -begin
	//#NT#New ADAS
	ADAS_GetAppsLogString(uiVidEncId, &g_cMovieStampStr[uiVidEncId][0], 256);
	//#NT#2016/03/25#KCHong -end
    #endif
	GxGfx_SetTextStroke((const FONT *)g_VsFontIn[uiVidEncId].pFont, FONTSTYLE_NORMAL, SCALE_1X);
	GxGfx_Text(0, 0, 0, pStampInfo->pi8Str); //not really draw
	szStamp = GxGfx_GetTextLastSize(); //just get text size
	g_VsOsdWidth[uiVidEncId]=ALIGN_CEIL(szStamp.w ,8);
	g_VsOsdHight[uiVidEncId]=ALIGN_CEIL(szStamp.h, 2);
	//g_VsOsdWidth[uiVidEncId]=ALIGN_CEIL(uiStampWidth * strlen(pStampInfo->pi8Str), 4);
	//g_VsOsdHight[uiVidEncId]=ALIGN_CEIL(pStampInfo->ui32DstHeight, 2);
	//DBG_DUMP("g_VsOsdWidth=%d, %d\r\n",g_VsOsdWidth[uiVidEncId], g_VsOsdHight[uiVidEncId] );

      //MovieStamp_VsConfig(uiVidEncId, g_VsOsdWidth[uiVidEncId], g_VsOsdHight[uiVidEncId]);
      MovieStamp_VsConfig(uiVidEncId, STAMP_WIDTH_MAX, STAMP_HEIGHT_MAX);

      MovieStamp_VsFontConfig(uiVidEncId);
      MovieStamp_VsAllocFontBuf(uiVidEncId, g_VsOsdWidth[uiVidEncId], g_VsOsdHight[uiVidEncId]);
#endif
}

#if (THUMB_STAMP_FUNC == ENABLE)
void MovieStamp_Setup_Thumb(UINT32 uiVidEncId, UINT32 uiFlag, UINT32 uiImageWidth, UINT32 uiImageHeight, WATERLOGO_BUFFER *pWaterLogoBuf)
{
	PSTAMP_INFO     pThumbStampInfo;
	ICON_DB const   *pDB;
	UINT32          uiIconID;
	UINT32          uiStrOffset;

	g_uiThumbStampFormat = STAMP_FORMAT_YUV;
	g_uiThumbStampSetup  = uiFlag;

	pThumbStampInfo = &g_ThumbStampInfo;
	pThumbStampInfo->pi8Str = &g_cThumbStampStr[0];
	pThumbStampInfo->pDataBase = &gDateStampFont12x20;

	switch (uiFlag & STAMP_DATE_TIME_MASK) {
	case STAMP_DATE_TIME_AMPM:
		//sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00 AM");
		snprintf(pThumbStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00 AM");
		break;

	case STAMP_DATE: // date only is not suitable for movie stamp (it's suitable for still image stamp)
	case STAMP_TIME:
		//sprintf(pStampInfo->pi8Str, "00:00:00");
		snprintf(pThumbStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "00:00:00");
		break;

	case STAMP_DATE_TIME:
	default:
		//sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00");
		snprintf(pThumbStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00");
		break;
	}

	pDB = pThumbStampInfo->pDataBase;
	uiStrOffset = pDB->uiDrawStrOffset;
	uiIconID = pThumbStampInfo->pi8Str[0] - uiStrOffset; // 1st font
	pThumbStampInfo->ui32FontWidth  = pDB->pIconHeader[uiIconID].uiWidth;
	pThumbStampInfo->ui32FontHeight = pDB->pIconHeader[uiIconID].uiHeight;
	pThumbStampInfo->ui32DstHeight  = pThumbStampInfo->ui32FontHeight; // no scaling

	if (g_bThumbStampColorSetup == FALSE) {
		// Stamp background color
		pThumbStampInfo->Color[COLOR_ID_BG].ucY = COLOR_BG_Y;
		pThumbStampInfo->Color[COLOR_ID_BG].ucU = COLOR_BG_U;
		pThumbStampInfo->Color[COLOR_ID_BG].ucV = COLOR_BG_V;

		// Stamp frame color
		pThumbStampInfo->Color[COLOR_ID_FR].ucY = COLOR_FR_Y;
		pThumbStampInfo->Color[COLOR_ID_FR].ucU = COLOR_FR_U;
		pThumbStampInfo->Color[COLOR_ID_FR].ucV = COLOR_FR_V;

		// Stamp foreground color (text body)
		pThumbStampInfo->Color[COLOR_ID_FG].ucY = COLOR_FG_Y;
		pThumbStampInfo->Color[COLOR_ID_FG].ucU = COLOR_FG_U;
		pThumbStampInfo->Color[COLOR_ID_FG].ucV = COLOR_FG_V;
	}

	// Set date stamp position
	if ((uiFlag & STAMP_OPERATION_MASK) == STAMP_AUTO) {
		UINT32  uiStampWidth = (pThumbStampInfo->ui32DstHeight * pThumbStampInfo->ui32FontWidth) / pThumbStampInfo->ui32FontHeight;

		switch (uiFlag & STAMP_POSITION_MASK) {
		case STAMP_TOP_LEFT:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_ThumbStampPos.uiX = 0;
				g_ThumbStampPos.uiY = 0;
			} else {
				g_ThumbStampPos.uiX = uiStampWidth; // 1 font width gap
				g_ThumbStampPos.uiY = pThumbStampInfo->ui32DstHeight / 2; // 1/2 font height gap
			}
			break;

		case STAMP_TOP_RIGHT:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_ThumbStampPos.uiX = uiImageWidth - uiStampWidth * strlen(pThumbStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
				g_ThumbStampPos.uiY = 0;
			} else {
				g_ThumbStampPos.uiX = uiImageWidth - uiStampWidth * (strlen(pThumbStampInfo->pi8Str) + 1); // 1 font width gap
				g_ThumbStampPos.uiY = pThumbStampInfo->ui32DstHeight / 2; // 1/2 font height gap
			}
			break;

		case STAMP_BOTTOM_LEFT:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_ThumbStampPos.uiX = 0;
				g_ThumbStampPos.uiY = uiImageHeight - pThumbStampInfo->ui32DstHeight;
			} else {
				g_ThumbStampPos.uiX = uiStampWidth; // 1 font width gap
				g_ThumbStampPos.uiY = uiImageHeight - (pThumbStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
			}
			break;

		case STAMP_BOTTOM_RIGHT:
		default:
			if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END) {
				g_ThumbStampPos.uiX = uiImageWidth - uiStampWidth * strlen(pThumbStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
				g_ThumbStampPos.uiY = uiImageHeight - pThumbStampInfo->ui32DstHeight;
			} else {
				g_ThumbStampPos.uiX = uiImageWidth - uiStampWidth * (strlen(pThumbStampInfo->pi8Str) + 1); // 1 font width gap
				g_ThumbStampPos.uiY = uiImageHeight - (pThumbStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
			}
			break;
		}
	}

}
#endif

void MovieStamp_SetPos(UINT32 uiVidEncId, UINT32 x, UINT32 y)
{
	g_MovieStampPos[uiVidEncId].uiX = x;
	g_MovieStampPos[uiVidEncId].uiY = y;
}

void MovieStamp_SetColor(UINT32 uiVidEncId, PSTAMP_COLOR pStampColorBg, PSTAMP_COLOR pStampColorFr, PSTAMP_COLOR pStampColorFg)
{
	g_bStampColorSetup[uiVidEncId] = TRUE;

	// Stamp background color
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY = pStampColorBg->ucY;
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU = pStampColorBg->ucU;
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV = pStampColorBg->ucV;

	// Stamp frame color
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucY = pStampColorFr->ucY;
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucU = pStampColorFr->ucU;
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucV = pStampColorFr->ucV;

	// Stamp foreground color (text body)
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucY = pStampColorFg->ucY;
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucU = pStampColorFg->ucU;
	g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucV = pStampColorFg->ucV;

#if (THUMB_STAMP_FUNC == ENABLE)
	g_bThumbStampColorSetup = TRUE;

	// Stamp background color
	g_ThumbStampInfo.Color[COLOR_ID_BG].ucY = pStampColorBg->ucY;
	g_ThumbStampInfo.Color[COLOR_ID_BG].ucU = pStampColorBg->ucU;
	g_ThumbStampInfo.Color[COLOR_ID_BG].ucV = pStampColorBg->ucV;

	// Stamp frame color
	g_ThumbStampInfo.Color[COLOR_ID_FR].ucY = pStampColorFr->ucY;
	g_ThumbStampInfo.Color[COLOR_ID_FR].ucU = pStampColorFr->ucU;
	g_ThumbStampInfo.Color[COLOR_ID_FR].ucV = pStampColorFr->ucV;

	// Stamp foreground color (text body)
	g_ThumbStampInfo.Color[COLOR_ID_FG].ucY = pStampColorFg->ucY;
	g_ThumbStampInfo.Color[COLOR_ID_FG].ucU = pStampColorFg->ucU;
	g_ThumbStampInfo.Color[COLOR_ID_FG].ucV = pStampColorFg->ucV;
#endif
}

#if 0
UINT32 MovieStamp_GetMaxFontHeight(void)
{
	return g_uiMovieStampHeightMax;
}
#endif

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
static UINT32 MovieStamp_GetLingualDataWidth(LINGUAL_INFO *pingualInfo, UINT32 uiStrLen)
{
	UINT32  i;
	UINT32  uiDataWidth;

	uiDataWidth = 0;
	for (i = 0; i < uiStrLen; i++) {
		uiDataWidth += pingualInfo[i].glyph.GLYPHINFO.wBoxX;
		uiDataWidth += ((pingualInfo[i].glyph.GLYPHINFO.wBoxX) % 2);        // if the font width is odd, append one pixel fot make it even.
	}
	return ALIGN_CEIL_4(uiDataWidth);
}

static BOOL MovieStamp_DrawMultiLingualIcon420UV(PSTAMP_INFO pStampInfo, LINGUAL_INFO LingualInfo, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
	UINT8   *uiIconData;
	UINT8   *pChar, *pCharU, *pCharV;
	UINT32  uiIconWidth;
	UINT32  uiIconHeight;
	UINT32  uiPixelCount;
	UINT32  i, j;
	UINT32  uiIndex;

	uiIconWidth = LingualInfo.glyph.GLYPHINFO.wBoxX;
	uiIconHeight = LingualInfo.glyph.GLYPHINFO.wBoxY;
	uiIconData = &LingualInfo.GlyphData[0];

	uiPixelCount = 0;

	for (i = 0; i < uiIconHeight; i++) {
		pChar = (UINT8 *)uiFBY;
		pCharU = (UINT8 *)uiFBCb;
		pCharV = (UINT8 *)uiFBCb + 1;

		for (j = 0; j < uiIconWidth; j ++) {
			uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

			if (uiIndex) {
				if (uiIndex == 1) { // frame color
					*pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
				} else { // foreground color
					*pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
				}

				if (((j & 1) == 0) && ((i & 1) == 0)) {
					if (uiIndex == 1) { // frame color
						*pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
					} else { // foreground color
						*pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
					}
				}
			}
			pChar++;
			if ((j & 1) == 0) {
				pCharU += 2;
				pCharV += 2;
			}
			uiPixelCount ++;
			if (uiPixelCount % 4 == 0) {
				uiIconData ++;
			}

		}
		uiFBY += uiLineOffset;
		if ((i & 1) == 0) {
			uiFBCb += uiLineOffset;
			uiFBCr += uiLineOffset;
		}
	}
	return TRUE;

}

static BOOL MovieStamp_DrawMultiLingualIcon422UV(PSTAMP_INFO pStampInfo, LINGUAL_INFO LingualInfo, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
	UINT8   *uiIconData;
	UINT8   *pChar, *pCharU, *pCharV;
	UINT32  uiIconWidth;
	UINT32  uiIconHeight;
	UINT32  uiPixelCount;
	UINT32  i, j;
	UINT32  uiIndex;

	uiIconWidth = LingualInfo.glyph.GLYPHINFO.wBoxX;
	uiIconHeight = LingualInfo.glyph.GLYPHINFO.wBoxY;
	uiIconData = &LingualInfo.GlyphData[0];

	uiPixelCount = 0;

	for (i = 0; i < uiIconHeight; i++) {
		pChar = (UINT8 *)uiFBY;
		pCharU = (UINT8 *)uiFBCb;
		pCharV = (UINT8 *)uiFBCb + 1;

		for (j = 0; j < uiIconWidth; j ++) {
			uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

			if (uiIndex) {
				if (uiIndex == 1) { // frame color
					*pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
				} else { // foreground color
					*pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
				}

				if ((j & 1) == 0) {
					if (uiIndex == 1) { // frame color
						*pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
					} else { // foreground color
						*pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
					}
				}
			}
			pChar++;
			if ((j & 1) == 0) {
				pCharU += 2;
				pCharV += 2;
			}
			uiPixelCount ++;
			if (uiPixelCount % 4 == 0) {
				uiIconData ++;
			}

		}
		uiFBY += uiLineOffset;
//        if  ((i&1) == 0)
		{
			uiFBCb += uiLineOffset;
			uiFBCr += uiLineOffset;
		}
	}
	return TRUE;

}

static UINT32 MovieStamp_DrawMultiLingualString(PSTAMP_INFO pStampInfo, PSTAMP_BUFFER pStampBuffer, UINT16 *pUniCodeArr, UINT32 datalen)
{
	UINT32  i, j;
	UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
	UINT32  uiDataWidth;

	uiFBAddrY  = pStampBuffer->uiYAddr;
	uiFBAddrCb = pStampBuffer->uiUAddr;
	uiFBAddrCr = pStampBuffer->uiVAddr;

	uiDataWidth = 0;
	for (i = 0 ; i < datalen ; i++) {
		j = LingualInfo[i].glyph.GLYPHINFO.wBoxX;
		uiDataWidth += j;

		if (j % 2 != 0) {
			j++;
		}

		if (pStampBuffer->uiFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
			MovieStamp_DrawMultiLingualIcon420UV(pStampInfo, LingualInfo[i], uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
		} else {
			MovieStamp_DrawMultiLingualIcon422UV(pStampInfo, LingualInfo[i], uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
		}

		uiFBAddrY  += (j);
		uiFBAddrCb += (j);
		uiFBAddrCr += (j);
	}
	return uiDataWidth;
}
#endif

static BOOL MovieStamp_DrawIcon420UV(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
	ICON_HEADER pIconHeader;
	ICON_DB const *pDB;
	UINT8   const *uiIconData;
	UINT8   *pChar, *pCharU, *pCharV;
	UINT32  uiIconWidth;
	UINT32  uiIconHeight;
	UINT32  uiPixelCount;
	UINT32  i, j;
	UINT32  uiIndex;

	pDB = pStampInfo->pDataBase;
	pIconHeader = pDB->pIconHeader[uiIconID];
	uiIconWidth = pIconHeader.uiWidth;
	uiIconHeight = pIconHeader.uiHeight;
	uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

	uiPixelCount = 0;

	for (i = 0; i < uiIconHeight; i++) {
		pChar = (UINT8 *)uiFBY;
		pCharU = (UINT8 *)uiFBCb;
		pCharV = (UINT8 *)uiFBCb + 1;

		for (j = 0; j < uiIconWidth; j ++) {
			uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

			if (uiIndex) {
				if (uiIndex == 1) { // frame color
					*pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
				} else { // foreground color
					*pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
				}

				if (((j & 1) == 0) && ((i & 1) == 0)) {
					if (uiIndex == 1) { // frame color
						*pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
					} else { // foreground color
						*pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
					}
				}
			}
			pChar++;
			if ((j & 1) == 0) {
				pCharU += 2;
				pCharV += 2;
			}
			uiPixelCount ++;
			if (uiPixelCount % 4 == 0) {
				uiIconData ++;
			}

		}
		uiFBY += uiLineOffset;
		if ((i & 1) == 0) {
			uiFBCb += uiLineOffset;
			uiFBCr += uiLineOffset;
		}
	}
	return TRUE;
}

static BOOL MovieStamp_DrawIcon422UV(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
	ICON_HEADER pIconHeader;
	ICON_DB const *pDB;
	UINT8   const *uiIconData;
	UINT8   *pChar, *pCharU, *pCharV;
	UINT32  uiIconWidth;
	UINT32  uiIconHeight;
	UINT32  uiPixelCount;
	UINT32  i, j;
	UINT32  uiIndex;

	pDB = pStampInfo->pDataBase;
	pIconHeader = pDB->pIconHeader[uiIconID];
	uiIconWidth = pIconHeader.uiWidth;
	uiIconHeight = pIconHeader.uiHeight;
	uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

	uiPixelCount = 0;

	for (i = 0; i < uiIconHeight; i++) {
		pChar = (UINT8 *)uiFBY;
		pCharU = (UINT8 *)uiFBCb;
		pCharV = (UINT8 *)uiFBCb + 1;

		for (j = 0; j < uiIconWidth; j ++) {
			uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

			if (uiIndex) {
				if (uiIndex == 1) { // frame color
					*pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
				} else { // foreground color
					*pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
				}

				//if (((j&1)==0)&&((i&1)==0))
				if ((j & 1) == 0) {
					if (uiIndex == 1) { // frame color
						*pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
					} else { // foreground color
						*pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
						*pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
					}
				}
			}
			pChar++;
			if ((j & 1) == 0) {
				pCharU += 2;
				pCharV += 2;
			}
			uiPixelCount ++;
			if (uiPixelCount % 4 == 0) {
				uiIconData ++;
			}

		}
		uiFBY += uiLineOffset;
		//if  ((i&1) == 0)
		{
			uiFBCb += uiLineOffset;
			uiFBCr += uiLineOffset;
		}
	}
	return TRUE;
}

#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
static BOOL MovieStamp_DrawIcon4bitIdx(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiLineOffset, UINT32 uiPixelShift)
{
	ICON_HEADER pIconHeader;
	ICON_DB const *pDB;
	UINT8   const *uiIconData;
	UINT8   *pChar;
	UINT8   u4ColorIdx;
	UINT32  uiIconWidth;
	UINT32  uiIconHeight;
	UINT32  uiPixelCount;
	UINT32  i, j;
	UINT32  uiIndex;

	pDB = pStampInfo->pDataBase;
	pIconHeader = pDB->pIconHeader[uiIconID];
	uiIconWidth = pIconHeader.uiWidth;
	uiIconHeight = pIconHeader.uiHeight;
	uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

	uiPixelCount = 0;
	DBG_MSG("uiIconWidth %d, uiIconHeight %d, uiFBY 0x%08x, uiLineOffset %d\r\n", uiIconWidth, uiIconHeight, uiFBY, uiLineOffset);

	for (i = 0; i < uiIconHeight; i++) {
		pChar = (UINT8 *)uiFBY;

		for (j = 0; j < uiIconWidth; j ++) {
			uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

			switch (uiIndex) {
			case COLOR_ID_BG: // background color
				u4ColorIdx = 0;
				break;

			case COLOR_ID_FR: // frame color
				u4ColorIdx = 1;
				break;

			default: // foreground color
				u4ColorIdx = 2;
				break;
			}

			switch ((j + uiPixelShift) % 2) {
			case 1:
				*pChar = (*pChar & 0x0f) | (u4ColorIdx << 4);
				pChar++;
				break;

			default:
				*pChar = (*pChar & 0xf0) | u4ColorIdx;
				break;
			}

			uiPixelCount ++;
			if (uiPixelCount % 4 == 0) {
				uiIconData ++;
			}

		}
		uiFBY += uiLineOffset;
	}
	return TRUE;
}

#else // COLOR_IDX_2BIT

static BOOL MovieStamp_DrawIcon2bitIdx(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiLineOffset, UINT32 uiPixelShift)
{
	ICON_HEADER pIconHeader;
	ICON_DB const *pDB;
	UINT8   const *uiIconData;
	UINT8   *pChar;
	UINT8   u4ColorIdx;
	UINT32  uiIconWidth;
	UINT32  uiIconHeight;
	UINT32  uiPixelCount;
	UINT32  i, j;
	UINT32  uiIndex;

	pDB = pStampInfo->pDataBase;
	pIconHeader = pDB->pIconHeader[uiIconID];
	uiIconWidth = pIconHeader.uiWidth;
	uiIconHeight = pIconHeader.uiHeight;
	uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

	uiPixelCount = 0;
	DBG_MSG("uiIconWidth %d, uiIconHeight %d, uiFBY 0x%08x, uiLineOffset %d\r\n", uiIconWidth, uiIconHeight, uiFBY, uiLineOffset);

	for (i = 0; i < uiIconHeight; i++) {
		pChar = (UINT8 *)uiFBY;

		for (j = 0; j < uiIconWidth; j ++) {
			uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

			switch (uiIndex) {
			case COLOR_ID_BG: // background color
				u4ColorIdx = 0;
				break;

			case COLOR_ID_FR: // frame color
				u4ColorIdx = 1;
				break;

			default: // foreground color
				u4ColorIdx = 2;
				break;
			}

			switch ((j + uiPixelShift) % 4) {
			case 1:
				*pChar = (*pChar & 0xf3) | (u4ColorIdx << 2);
				break;

			case 2:
				*pChar = (*pChar & 0xcf) | (u4ColorIdx << 4);
				break;

			case 3:
				*pChar = (*pChar & 0x3f) | (u4ColorIdx << 6);
				pChar++;
				break;

			default:
				*pChar = (*pChar & 0xfc) | u4ColorIdx;
				break;
			}

			uiPixelCount ++;
			if (uiPixelCount % 4 == 0) {
				uiIconData ++;
			}

		}
		uiFBY += uiLineOffset;
	}
	return TRUE;
}
#endif

static UINT32 MovieStamp_GetStampDataWidth(PSTAMP_INFO pStampInfo)
{
	ICON_DB const *pDB;
	UINT32  i;
	UINT32  uiStrLen;
	UINT32  uiDataWidth;
	UINT32  uiIconID;
	UINT32  uiStrOffset;

	uiStrLen = strlen(pStampInfo->pi8Str);

	pDB = pStampInfo->pDataBase;
	uiStrOffset = pDB->uiDrawStrOffset;

	uiDataWidth = 0;
	for (i = 0; i < uiStrLen; i++) {
		//get icon database
		uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
		uiDataWidth += pDB->pIconHeader[uiIconID].uiWidth;
	}

	return uiDataWidth;
}

static UINT32 MovieStamp_DrawString(UINT32 uiVidEncId, PSTAMP_INFO pStampInfo, PSTAMP_BUFFER pStampBuffer)
{
	ICON_DB const *pDB;
	UINT32  i, j;
	UINT32  uiStrLen;
	UINT32  uiDataWidth;
	UINT32  uiIconID;
	UINT32  uiStrOffset;
	UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
	UINT32  uiStampAddr, uiPixelShift;

	if(g_VsStampEn==TRUE)	{
		return 0;
 	}

	uiStrLen = strlen(pStampInfo->pi8Str);

	pDB = pStampInfo->pDataBase;
	uiStrOffset = pDB->uiDrawStrOffset;

	uiFBAddrY  = pStampBuffer->uiYAddr;
	uiFBAddrCb = pStampBuffer->uiUAddr;
	uiFBAddrCr = pStampBuffer->uiVAddr;

	uiDataWidth = 0;
	for (i = 0; i < uiStrLen; i++) {
		//get icon database
		uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
		j = pDB->pIconHeader[uiIconID].uiWidth;

		//if (j % 2)
		//{
		//    j++;
		//}

		if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_IDX) { // color index format (for direct path only)
#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
			uiStampAddr = uiFBAddrY + uiDataWidth / 2;
			uiPixelShift = uiDataWidth % 2;
			MovieStamp_DrawIcon4bitIdx(pStampInfo, uiIconID, uiStampAddr, pStampBuffer->uiYLineOffset, uiPixelShift);
#else // COLOR_IDX_2BIT
			uiStampAddr = uiFBAddrY + uiDataWidth / 4;
			uiPixelShift = uiDataWidth % 4;
			MovieStamp_DrawIcon2bitIdx(pStampInfo, uiIconID, uiStampAddr, pStampBuffer->uiYLineOffset, uiPixelShift);
#endif
		} else { // YUV format
			if (pStampBuffer->uiFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
				MovieStamp_DrawIcon420UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
			} else {
				MovieStamp_DrawIcon422UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
			}

			uiFBAddrY  += (j);
			uiFBAddrCb += (j);
			uiFBAddrCr += (j);
		}

		uiDataWidth += j;
	}

	return uiDataWidth;
}

#if (THUMB_STAMP_FUNC == ENABLE)
static UINT32 MovieStamp_DrawString_Thumb(UINT32 uiVidEncId, PSTAMP_INFO pStampInfo, PSTAMP_BUFFER pStampBuffer)
{
	ICON_DB const *pDB;
	UINT32  i, j;
	UINT32  uiStrLen;
	UINT32  uiDataWidth;
	UINT32  uiIconID;
	UINT32  uiStrOffset;
	UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
	UINT32  uiStampAddr, uiPixelShift;
	if(g_VsStampEn==TRUE)	{
		return 0;
 	}

	uiStrLen = strlen(pStampInfo->pi8Str);

	pDB = pStampInfo->pDataBase;
	uiStrOffset = pDB->uiDrawStrOffset;

	uiFBAddrY  = pStampBuffer->uiYAddr;
	uiFBAddrCb = pStampBuffer->uiUAddr;
	uiFBAddrCr = pStampBuffer->uiVAddr;

	uiDataWidth = 0;
	for (i = 0; i < uiStrLen; i++) {
		//get icon database
		uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
		j = pDB->pIconHeader[uiIconID].uiWidth;

		//if (j % 2)
		//{
		//    j++;
		//}

		if (g_uiThumbStampFormat == STAMP_FORMAT_IDX) { // color index format (for direct path only)
#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
			uiStampAddr = uiFBAddrY + uiDataWidth / 2;
			uiPixelShift = uiDataWidth % 2;
			MovieStamp_DrawIcon4bitIdx(pStampInfo, uiIconID, uiStampAddr, pStampBuffer->uiYLineOffset, uiPixelShift);
#else // COLOR_IDX_2BIT
			uiStampAddr = uiFBAddrY + uiDataWidth / 4;
			uiPixelShift = uiDataWidth % 4;
			MovieStamp_DrawIcon2bitIdx(pStampInfo, uiIconID, uiStampAddr, pStampBuffer->uiYLineOffset, uiPixelShift);
#endif
		} else { // YUV format
			if (pStampBuffer->uiFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
				MovieStamp_DrawIcon420UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
			} else {
				MovieStamp_DrawIcon422UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
			}

			uiFBAddrY  += (j);
			uiFBAddrCb += (j);
			uiFBAddrCr += (j);
		}

		uiDataWidth += j;
	}

	return uiDataWidth;
}
#endif
#if (STAMP_PRIVATE_POOL == ENABLE)
UINT32 MovieStamp_GetBufAddr(UINT32 uiVidEncId, UINT32 blk_size)
{
        NVTMPP_VB_BLK  blk=0;
        CHAR pool_name[20] ={0};

        //DBG_ERR("g_MovieStampPool[%d]=%d\r\n",uiVidEncId,g_MovieStampPool[uiVidEncId]);

        if(g_MovieStampPool[uiVidEncId]==NVTMPP_VB_INVALID_POOL)  {
            sprintf(pool_name,"MovieStamp_%d",uiVidEncId);
            g_MovieStampPool[uiVidEncId]=nvtmpp_vb_create_pool(pool_name, blk_size , 1, NVTMPP_DDR_1);
            if (NVTMPP_VB_INVALID_POOL == g_MovieStampPool[uiVidEncId])	{
        		DBG_ERR("create private pool err\r\n");
        		return NVTEVT_CONSUME;
            }

            blk = nvtmpp_vb_get_block(0, g_MovieStampPool[uiVidEncId], blk_size, NVTMPP_DDR_1);
            if (NVTMPP_VB_INVALID_BLK == blk) {
        		DBG_ERR("get vb block err\r\n");
                	return NVTEVT_CONSUME;
            }
            g_MovieStampPoolAddr[uiVidEncId]=nvtmpp_vb_block2addr(blk);
        }
        if(g_MovieStampPoolAddr[uiVidEncId] == 0)
    		DBG_ERR("get buf addr err\r\n");
        return g_MovieStampPoolAddr[uiVidEncId];
}
#endif
void MovieStamp_DestroyBuff(void)
{
#if (STAMP_PRIVATE_POOL == ENABLE)
	INT32 i, ret;
	for (i=0;i<VIDEO_IN_MAX;i++) {
		if(g_MovieStampPool[i] != NVTMPP_VB_INVALID_POOL){
			ret = nvtmpp_vb_destroy_pool(g_MovieStampPool[i]);
			if (NVTMPP_ER_OK != ret) {
				DBG_ERR("destory pool error pool 0x%x   ret=%d\r\n", g_MovieStampPool[i], ret);
				break;
			}
			g_MovieStampPool[i]=NVTMPP_VB_INVALID_POOL;
			g_MovieStampPoolAddr[i]=0;
		}
	}
#if (THUMB_STAMP_FUNC == ENABLE)
	for (i=0;i<1;i++) {
		if(g_MovieStampThumbPool[i] != NVTMPP_VB_INVALID_POOL){
			ret = nvtmpp_vb_destroy_pool(g_MovieStampThumbPool[i]);
			if (NVTMPP_ER_OK != ret) {
				DBG_ERR("destory thumb pool error pool 0x%x   ret=%d\r\n", g_MovieStampThumbPool[i], ret);
				break;
			}
			g_MovieStampThumbPool[i]=NVTMPP_VB_INVALID_POOL;
			g_MovieStampThumbPoolAddr[i]=0;
		}
	}
#endif
#endif
#if (MOVIE_FR_FUNC== ENABLE)
	MovieStamp_DestroyFaceNameBuff();
#endif
}
#if (THUMB_STAMP_FUNC == ENABLE  &&  STAMP_PRIVATE_POOL == ENABLE)
UINT32 MovieStampThumb_GetBufAddr(UINT32 blk_size)
{
        NVTMPP_VB_BLK  blk;
        if(g_MovieStampThumbPool[0]==NVTMPP_VB_INVALID_POOL)   {
            g_MovieStampThumbPool[0]=nvtmpp_vb_create_pool("MovieStampThumb", blk_size , 1, NVTMPP_DDR_1);
            if (NVTMPP_VB_INVALID_POOL == g_MovieStampThumbPool[0])	{
        		DBG_ERR("create private pool err\r\n");
                	return NVTEVT_CONSUME;
            }
            blk = nvtmpp_vb_get_block(0, g_MovieStampThumbPool[0], blk_size, NVTMPP_DDR_1);
            if (NVTMPP_VB_INVALID_BLK == blk) {
        		DBG_ERR("get vb block err\r\n");
        		return NVTEVT_CONSUME;
            }
            g_MovieStampThumbPoolAddr[0]=nvtmpp_vb_block2addr(blk);
        }
        if(g_MovieStampThumbPoolAddr[0] == 0)
    		DBG_ERR("get buf addr err\r\n");

        return g_MovieStampThumbPoolAddr[0];
}
#endif
void MovieStamp_SetBuffer(UINT32 uiVidEncId, UINT32 uiAddr, UINT32 uiSize)
{
	//#NT#2015/11/04#KCHong -begin
	// The required buffer size now is based on the definition switch.
	// For current project setting (if waterlogo and multilingual function is enabled.
	// Total buffer size                = 1MB (for each VidEncId)
	// Waterlog buffer size             =  64K (256*128*2)
	// Multilingual buffer size         = 120K (1920*32*2) ==> Assume font height should less than 32, otherwise need to re-adjust the buffer size.
	// Multilingual scaling buffer size = 330K (1920*44*2*2) ==> Ping-Pong buffer. Assume scaled font height should less than 44, otherwise need to re-adjust the buffer size.
	// Movie stamp buffer               = 510K (1920*68*2*2) ==> Use rest memories. Ping-pong buffer.

	/*
	g_uiWaterLogoYAddr[uiVidEncId]     = uiAddr;
	g_uiWaterLogoUVAddr[uiVidEncId]    = uiAddr + WATERLOGO_BUF_SIZE / 2; // YUV422, UV-packed
	g_uiMovieStampYAddr[uiVidEncId][0] = uiAddr + WATERLOGO_BUF_SIZE;
	g_uiMovieStampYAddr[uiVidEncId][1] = uiAddr + (uiSize - WATERLOGO_BUF_SIZE) / 2;
	*/
	g_uiWaterLogoYAddr[uiVidEncId]     = uiAddr;
	g_uiWaterLogoUVAddr[uiVidEncId]    = g_uiWaterLogoYAddr[uiVidEncId] + WATERLOGO_BUF_SIZE / 2; // YUV422, UV-packed

	g_uiLingualStampYAddr[uiVidEncId] = uiAddr + WATERLOGO_BUF_SIZE;
	g_uiLingualScaledStampYAddr[uiVidEncId][0] = g_uiLingualStampYAddr[uiVidEncId] + MULTILINGUAL_BUF_SIZE;
	g_uiLingualScaledStampYAddr[uiVidEncId][1] = g_uiLingualScaledStampYAddr[uiVidEncId][0] + MULTILINGUAL_SCALLED_BUF_SIZE;

	g_uiMovieStampYAddr[uiVidEncId][0] = g_uiLingualScaledStampYAddr[uiVidEncId][1] + MULTILINGUAL_SCALLED_BUF_SIZE;
	//#NT#2016/04/22#YongChang Qui -begin
	//#NT#IPCam osd try to avoid buffer overrun
	g_uiMovieStampYAddr[uiVidEncId][1] = g_uiMovieStampYAddr[uiVidEncId][0] + (uiAddr + uiSize - g_uiMovieStampYAddr[uiVidEncId][0]) / 2;
	g_uiMovieStampSize[uiVidEncId] = (g_uiMovieStampYAddr[uiVidEncId][1] - g_uiMovieStampYAddr[uiVidEncId][0]);
	//#NT#2016/04/22#YongChang Qui -end
	//#NT#2015/11/04#KCHong -end
#if (THUMB_STAMP_FUNC == ENABLE)
#if (STAMP_PRIVATE_POOL == DISABLE)
	//g_uiThumbStampYAddr = g_uiMovieStampYAddr[uiVidEncId][0] + g_uiMovieStampSize[uiVidEncId]*2;
	g_uiThumbStampYAddr = (OS_GetMempoolAddr(POOL_ID_DATEIMPRINT)+POOL_SIZE_DATEIMPRINT);
#else
	//private pool ENABLE, photo mode not want to set thumb buffer, so setThumb buffer in movie mode MovieExe_OnRecStart()
#endif
#endif
    	g_uiVsStampAddr[uiVidEncId]=g_uiMovieStampYAddr[uiVidEncId][0];

    	//g_uiVsFontAddr[uiVidEncId]=g_uiMovieStampYAddr[uiVidEncId][1];
    	//coe+ FontConv= Wx FontHeight x4 + Wx FontHeight x5.5
    	g_uiVsFontAddr[uiVidEncId]=g_uiVsStampAddr[uiVidEncId] + (uiSize-(WATERLOGO_BUF_SIZE + MULTILINGUAL_BUF_SIZE +MULTILINGUAL_SCALLED_BUF_SIZE))*4*10/95;

}
UINT32 MovieStamp_CalcBufSize(UINT32 Width, UINT32 Height)
{
    UINT32 BufSize=0;
    UINT32 FontHeight=0;

    switch (Width) {
    case 3840:  // 3840x2160
    	FontHeight=44;
    	break;
    case 2560:  // 2560x1440
    case 2304:  // 2304x1296
    case 1920:  // 1920x1080
    case 1536:  // 1536x1536
    	FontHeight=44;
    	break;

    case 2880:  // 2880x2160 (DAR 16:9)
    case 1728:  // 1728x1296 (DAR 16:9)
    case 1440:  // 1440x1080 (DAR 16:9)
    	FontHeight=44;
    	break;

    case 1280:  // 1280x720
    	FontHeight=30;
    	break;

    case 320:   // QVGA
    	FontHeight=16;
    	break;

    default:    // VGA & others
    	FontHeight=20;
    	break;
    }
    FontHeight =64;
    BufSize = WATERLOGO_BUF_SIZE +
             MULTILINGUAL_BUF_SIZE +
             MULTILINGUAL_SCALLED_BUF_SIZE +
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
             videosprite_query_size(VDS_PHASE_BTN, VDS_BUF_TYPE_PING_PONG, Width, FontHeight) +
             Width*FontHeight*VSFONT_BUF_RATIO/100;
#else
             (Width*FontHeight*3/2)*2; //*3/2 for YUV420, *2 for pingpong buffer
#endif
    return BufSize;
}
void MovieStampThumb_SetBuffer(void)
{
#if (THUMB_STAMP_FUNC == ENABLE  &&  STAMP_PRIVATE_POOL == ENABLE)
    #if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
        g_uiThumbStampYAddr = MovieStampThumb_GetBufAddr(640*20*2);//WxHx2, 2 for argb4444
    #else
        g_uiThumbStampYAddr = MovieStampThumb_GetBufAddr(POOL_SIZE_DATEIMPRINT_THUMB);
    #endif
#endif
}
#if (CHECK_ENC_INFO == ENABLE)
extern void MediaRec_GetEncInfo(UINT32 uiVidEncId, UINT32 puiParam[7]);
#endif

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
void MultiLingualString_UpdateData(void)
{
	GX_IMAGE_PIXEL_FMT GxImgFormat;
	IMG_BUF         SrcImg;
	UINT32          uiPreWidth, uiWidth;
	UINT32          uiBgData;
	UINT32          uiLineOffset[2] = {0};
	UINT32          uiPxlAddr[2] = {0};
	UINT32          i;
	UINT32          uiUnicodeLen = 0;
	STAMP_BUFFER    LingualStampBuffer = {0};
	STAMP_BUFFER    LingualScaleStampBuffer = {0};
	IMG_BUF         DstImg;
	IRECT           SrcRegion = {0};
	IRECT           DstRegion = {0};

	for (i = 0; i < VIDEO_IN_MAX; i++) {
		if ((g_uiMovieStampSetup[i] & STAMP_SWITCH_MASK) == STAMP_ON) {
			switch (g_uiMovieStampSetup[i] & STAMP_IMG_FORMAT_MASK) {
			case STAMP_IMG_420UV:
				GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
				break;

			case STAMP_IMG_422UV:
				GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
				break;

			default:
				DBG_ERR("Only support 420/422 UV pack!\r\n");
				return;
			}

			for (uiUnicodeLen = 0; uiUnicodeLen < (sizeof(unicode) / sizeof(UINT16)); uiUnicodeLen++) {
				UniFont_GetGlyphFromUnicode(unicode[uiUnicodeLen], &LingualInfo[uiUnicodeLen]);
				if (LingualInfo[uiUnicodeLen].glyph.GLYPHINFO.wBoxY > g_uiLingualStampHeight) {
					g_uiLingualStampHeight = LingualInfo[uiUnicodeLen].glyph.GLYPHINFO.wBoxY;
				}
			}

			LingualStampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
			LingualStampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
			LingualStampBuffer.uiYAddr         = g_uiLingualStampYAddr[i];
			LingualStampBuffer.uiUAddr         = LingualStampBuffer.uiYAddr + LingualStampBuffer.uiYLineOffset * g_uiLingualStampHeight;
			LingualStampBuffer.uiVAddr         = LingualStampBuffer.uiUAddr; // for 420 UV pack
			LingualStampBuffer.uiFormat        = GxImgFormat;

			uiPreWidth = g_uiLingualStampWidth[i]; // previous stamp data width
			g_uiLingualStampWidth[i] = MovieStamp_GetLingualDataWidth(&LingualInfo[0], uiUnicodeLen);
			g_uiLingualStampWidth[i] = ALIGN_CEIL_4(g_uiLingualStampWidth[i]); // limitation of index color stamp mode
			uiWidth = (uiPreWidth > g_uiLingualStampWidth[i]) ? uiPreWidth : g_uiLingualStampWidth[i];

			// Set background data, share same uiBgData setting with Movie Stamp
			if (g_uiMovieStampFormat[i] == STAMP_FORMAT_IDX) { // color index format (for direct path only)
#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
				uiWidth >>= 1;
#else
				uiWidth >>= 2;
#endif
				uiBgData = COLOR_ID_BG;
			} else { // STAMP_FORMAT_YUV
				if ((g_uiMovieStampSetup[i] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT) {
					//uiBgData = COLORKEY_BG_YUV; // for LE op
					uiBgData = COLORKEY_YUV; // for EQ op
				} else {
					uiBgData = ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucV << 16) |
							   ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucU << 8)  |
							   g_MovieStampInfo[i].Color[COLOR_ID_BG].ucY;
				}
			}

			// Clear source (stamp) buffer (in fact, index color mode doesn't need to copy Y, U, V data,
			// it only needs to copy "color index" data; but however, use GxImg function temporarily.
			uiLineOffset[0] = LingualStampBuffer.uiYLineOffset;
			uiLineOffset[1] = LingualStampBuffer.uiUVLineOffset;
			uiPxlAddr[0]    = LingualStampBuffer.uiYAddr;
			uiPxlAddr[1]    = LingualStampBuffer.uiUAddr;

			GxImg_InitBufEx(&SrcImg,
							uiWidth,
							g_uiLingualStampHeight,
							GxImgFormat,
							uiLineOffset,
							uiPxlAddr);
			GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);

			MovieStamp_DrawMultiLingualString(&g_MovieStampInfo[i], &LingualStampBuffer, &unicode[0], uiUnicodeLen);

			g_uiLingualScaledStampHeight = (g_uiLingualStampHeight * g_uiLingualStampScalingFactor[i]) / g_uiLingualStampHeight;
			g_uiLingualScaledStampWidth[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL] = (g_uiLingualStampWidth[i] * g_uiLingualStampScalingFactor[i]) / g_uiLingualStampHeight;

			// Init source (stamp) buffer
			uiLineOffset[0] = LingualStampBuffer.uiYLineOffset;
			uiLineOffset[1] = LingualStampBuffer.uiUVLineOffset;
			uiPxlAddr[0]    = LingualStampBuffer.uiYAddr;
			uiPxlAddr[1]    = LingualStampBuffer.uiUAddr;

			GxImg_InitBufEx(&SrcImg,
							g_uiLingualStampWidth[i],
							g_uiLingualStampHeight,
							GxImgFormat,
							uiLineOffset,
							uiPxlAddr);

			LingualScaleStampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
			LingualScaleStampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
			LingualScaleStampBuffer.uiYAddr         = g_uiLingualScaledStampYAddr[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL];
			LingualScaleStampBuffer.uiUAddr         = LingualScaleStampBuffer.uiYAddr + LingualScaleStampBuffer.uiYLineOffset * g_uiLingualScaledStampHeight;
			LingualScaleStampBuffer.uiVAddr         = LingualScaleStampBuffer.uiUAddr; // for 420 UV pack
			LingualScaleStampBuffer.uiFormat        = GxImgFormat;

			uiLineOffset[0] = LingualScaleStampBuffer.uiYLineOffset;
			uiLineOffset[1] = LingualScaleStampBuffer.uiUVLineOffset;
			uiPxlAddr[0]    = LingualScaleStampBuffer.uiYAddr;
			uiPxlAddr[1]    = LingualScaleStampBuffer.uiUAddr;

			GxImg_InitBufEx(&DstImg,
							g_uiLingualScaledStampWidth[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL],
							g_uiLingualScaledStampHeight,
							GxImgFormat,
							uiLineOffset,
							uiPxlAddr);

			// Set scale region
			SrcRegion.x = 0;
			SrcRegion.y = 0;
			SrcRegion.w = g_uiLingualStampWidth[i];
			SrcRegion.h = g_uiLingualStampHeight;

			DstRegion.x = 0;
			DstRegion.y = 0;
			DstRegion.w = g_uiLingualScaledStampWidth[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL];
			DstRegion.h = g_uiLingualScaledStampHeight;

			// Scale image by ISE
			GxImg_SetParm(GXIMG_PARM_SCALE_METHOD, GXIMG_SCALE_NEAREST);
			GxImg_ScaleData(&SrcImg, &SrcRegion, &DstImg, &DstRegion);
			GxImg_SetParm(GXIMG_PARM_SCALE_METHOD, GXIMG_SCALE_AUTO);
		}
	}
	if (g_uiLingualStampScalingFactor[0]) {
		g_uiLingualStampActBuf = (g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL;
	}
}
#endif
void MovieStamp_UpdateData(void)
{
	GX_IMAGE_PIXEL_FMT GxImgFormat;
	STAMP_BUFFER    StampBuffer;
	UINT32          uiStampDataWidth[VIDEO_IN_MAX] = {0};
	UINT32          uiStampDataHeight;
#if(VIDEOSPRITE_STAMP_FUNC == DISABLE)
	UINT32          uiPreWidth, uiWidth;
	UINT32          uiBgData;
	UINT32          uiLineOffset[2] = {0};
	UINT32          uiPxlAddr[2] = {0};
    	IMG_BUF         SrcImg = {0};
#endif
	UINT32          i;
#if (CHECK_STAMP_TIME == ENABLE)
	UINT32          uiTime, uiTimeCount; // for performance measurement
#endif
#if (MOVIE_AE_LOG == 0 && MOVIE_ADAS_LOG == 0)
	struct tm       CurDateTime;
	//CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);
	CurDateTime = g_CurDateTime;
#endif
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
	//UINT16* pVsFontDataAddr;
#endif
	for (i = 0; i < VIDEO_IN_MAX; i++) {
		if ((g_uiMovieStampSetup[i] & STAMP_SWITCH_MASK) == STAMP_ON) {
			switch (g_uiMovieStampSetup[i] & STAMP_IMG_FORMAT_MASK) {
			case STAMP_IMG_420UV:
				GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
				break;

			case STAMP_IMG_422UV:
				GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
				break;

			default:
				DBG_ERR("Only support 420/422 UV pack!\r\n");
				return;
			}

			uiStampDataHeight = g_MovieStampInfo[i].ui32DstHeight;
			StampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
			StampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
			if (g_uiMovieStampActBuf == MOVIE_STAMP_BUF_0) {
				StampBuffer.uiYAddr     = g_uiMovieStampYAddr[i][MOVIE_STAMP_BUF_1];
			} else {
				StampBuffer.uiYAddr     = g_uiMovieStampYAddr[i][MOVIE_STAMP_BUF_0];
			}
			StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
			StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
			StampBuffer.uiFormat        = GxImgFormat;

#if (MOVIE_AE_LOG == 0 && MOVIE_ADAS_LOG == 0)
			// Prepare date-time string
			if ((g_uiMovieStampSetup[i] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME) {
				switch (g_uiMovieStampSetup[i] & STAMP_DATE_FORMAT_MASK) {
				case STAMP_DD_MM_YY:
					//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
					snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
					break;
				case STAMP_MM_DD_YY:
					//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
					snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
					break;
				default:
					//sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
					snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
					break;
				}
			} else if ((g_uiMovieStampSetup[i] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME_AMPM) {
				if (CurDateTime.tm_hour >= 12) {
					if (CurDateTime.tm_hour > 12) {
						CurDateTime.tm_hour -= 12;
					}

					switch (g_uiMovieStampSetup[i] & STAMP_DATE_FORMAT_MASK) {
					case STAMP_DD_MM_YY:
						//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						break;
					case STAMP_MM_DD_YY:
						//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						break;
					default:
						//sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						break;
					}
				} else {
					switch (g_uiMovieStampSetup[i] & STAMP_DATE_FORMAT_MASK) {
					case STAMP_DD_MM_YY:
						//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						break;
					case STAMP_MM_DD_YY:
						//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						break;
					default:
						//sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
						break;
					}
				}
			} else {
				//sprintf(&g_cMovieStampStr[i][0], "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			}
#endif

#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
			{
				//this is for DDD debug
				snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%06x %02d/%02d %02d:%02d:%02d", g_DDD_DowsyState, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			}
#endif

#if MOVIE_AE_LOG
			////<< spark add AE function
			IQS_GetLogString(i, &g_cMovieStampStr[i][0], 256);
			//NMR_VdoEnc_GetRCStr(i, &g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN);
#endif

#if MOVIE_ADAS_LOG
			//#NT#2016/03/25#KCHong -begin
			//#NT#New ADAS
			ADAS_GetAppsLogString(i, &g_cMovieStampStr[i][0], 256);
			//#NT#2016/03/25#KCHong -end
#endif

#if (CHECK_ENC_INFO == ENABLE)
			{
				UINT32 uiParam[7];

				MediaRec_GetEncInfo(i, uiParam);
				//sprintf(&g_cMovieStampStr[i][0], "%d %d %d %d / %d %d %d",
				//    uiParam[0], uiParam[1], uiParam[2], uiParam[3], uiParam[4], uiParam[5], uiParam[6]);
				//sprintf(&g_cMovieStampStr[i][0], "%d %d %d %d / %d %d %d",
				//    uiParam[3], uiParam[4], uiParam[5], uiParam[6], uiParam[0], uiParam[1], uiParam[2]);
				//sprintf(&g_cMovieStampStr[i][0], "%d", uiParam[0]);
				snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%d", uiParam[0]);
			}
#endif

			uiStampDataWidth[i] = MovieStamp_GetStampDataWidth(&g_MovieStampInfo[i]);
			uiStampDataWidth[i] = ALIGN_CEIL_4(uiStampDataWidth[i]); // limitation of index color stamp mode
#if (VIDEOSPRITE_STAMP_FUNC == DISABLE)
			uiPreWidth          = g_uiMovieStampWidth[i]; // previous stamp data width
			uiWidth             = (uiPreWidth > uiStampDataWidth[i]) ? uiPreWidth : uiStampDataWidth[i];
			//DBG_DUMP("^Ystamp w %d, h %d\r\n", uiWidth, uiStampDataHeight);



			// Set background data
#if (CHECK_STAMP_TIME == ENABLE)
			uiTime = Perf_GetCurrent();
#endif

			if (g_uiMovieStampFormat[i] == STAMP_FORMAT_IDX) { // color index format (for direct path only)
#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
				uiWidth >>= 1;
#else
				uiWidth >>= 2;
#endif
				uiBgData = COLOR_ID_BG;
			} else { // STAMP_FORMAT_YUV
				if ((g_uiMovieStampSetup[i] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT) {
					//uiBgData = COLORKEY_BG_YUV; // for LE op
					uiBgData = COLORKEY_YUV; // for EQ op
				} else {
					uiBgData = ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucV << 16) |
							   ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucU << 8)  |
							   g_MovieStampInfo[i].Color[COLOR_ID_BG].ucY;
				}
			}

			// Clear source (stamp) buffer (in fact, index color mode doesn't need to copy Y, U, V data,
			// it only needs to copy "color index" data; but however, use GxImg function temporarily.
			uiLineOffset[0] = StampBuffer.uiYLineOffset;
			uiLineOffset[1] = StampBuffer.uiUVLineOffset;
			uiPxlAddr[0]    = StampBuffer.uiYAddr;
			uiPxlAddr[1]    = StampBuffer.uiUAddr;

			GxImg_InitBufEx(&SrcImg,
							uiWidth,
							uiStampDataHeight,
							GxImgFormat,
							uiLineOffset,
							uiPxlAddr);

			GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);

#if (CHECK_STAMP_TIME == ENABLE)
			DBG_DUMP("time1: %d us\r\n", Perf_GetCurrent() - uiTime);
			uiTime = Perf_GetCurrent();
#endif
#endif //#if(VIDEOSPRITE_STAMP_FUNC == DISABLE)

			// Draw string by getting data from date stamp font
			MovieStamp_DrawString(i, &g_MovieStampInfo[i], &StampBuffer);


#if (CHECK_STAMP_TIME == ENABLE)
			DBG_DUMP("time2: %d us\r\n", Perf_GetCurrent() - uiTime);
#endif

#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
                   g_VsFontIn[i].pStr=g_MovieStampInfo[i].pi8Str;

                   if (FontConv(&g_VsFontIn[i], &g_VsFontOut[i]) != E_OK) {
                        DBG_ERR("FontConv err\r\n");
                   }
                   g_pVsFontDataAddr[i]=(UINT16 *)g_VsFontOut[i].GenImg.PxlAddr[0];
                   //DBG_DUMP("update pi8Str=%s\r\n", g_MovieStampInfo[i].pi8Str);
                   //DBG_DUMP("i=%d, w=%d, %d\r\n", i,g_VsOsdWidth[i], g_VsOsdHight[i]);
                   MovieStamp_VsUpdateOsd(i, TRUE, i, 1, g_MovieStampPos[i].uiX, g_MovieStampPos[i].uiY, g_VsOsdWidth[i], g_VsOsdHight[i], (void*)g_pVsFontDataAddr[i]);
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
                   MovieStamp_VsUpdateOsd(i, TRUE, (16+i), 2, g_MovieWaterInfo[i].uiXPos, g_MovieWaterInfo[i].uiYPos, g_MovieWaterInfo[i].uiWidth, g_MovieWaterInfo[i].uiHeight, (void*)g_ucWaterMark_ARGB4444);
#endif
                   MovieStamp_VsSwapOsd(i);
#endif
		}
	}

	for (i = 0; i < VIDEO_IN_MAX; i++) {
		g_uiMovieStampWidth[i] = uiStampDataWidth[i];
	}

	if (g_uiMovieStampActBuf == MOVIE_STAMP_BUF_0) {
		g_uiMovieStampActBuf = MOVIE_STAMP_BUF_1;
	} else {
		g_uiMovieStampActBuf = MOVIE_STAMP_BUF_0;
	}

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
	MultiLingualString_UpdateData();        // For MultiLingualString demo
#endif
}
void MovieStamp_RestoreData(UINT32 vid)
{
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    //DBG_DUMP("vid=%d\r\n",vid);
    MovieStamp_VsUpdateOsd(vid, TRUE, vid, 1, g_MovieStampPos[vid].uiX, g_MovieStampPos[vid].uiY, g_VsOsdWidth[vid], g_VsOsdHight[vid], (void*)g_pVsFontDataAddr[vid]);
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
    MovieStamp_VsUpdateOsd(vid, TRUE, (16+vid), 2, g_MovieWaterInfo[vid].uiXPos, g_MovieWaterInfo[vid].uiYPos, g_MovieWaterInfo[vid].uiWidth, g_MovieWaterInfo[vid].uiHeight, (void*)g_ucWaterMark_ARGB4444);
#endif

    MovieStamp_VsSwapOsd(vid);
#endif
}
#if (THUMB_STAMP_FUNC == ENABLE)
void MovieStamp_UpdateData_Thumb(UINT32 vid, BOOL bInit)
{
	GX_IMAGE_PIXEL_FMT GxImgFormat;
	STAMP_BUFFER    StampBuffer;
#if (VIDEOSPRITE_STAMP_FUNC == DISABLE)
	IMG_BUF         SrcImg = {0};
	UINT32          uiBgData;
	UINT32          uiLineOffset[2] = {0};
	UINT32          uiPxlAddr[2] = {0};
	UINT32          uiPreWidth, uiWidth;
#endif
	UINT32          uiStampDataHeight, uiStampDataWidth;

#if (MOVIE_AE_LOG == 0 && MOVIE_ADAS_LOG == 0)
	struct tm       CurDateTime;
	//CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);
	CurDateTime = g_CurDateTime;
#endif
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    	ISIZE szStamp;
	//UINT16* pVsFontDataAddr=NULL;
#endif

	if ((g_uiThumbStampSetup & STAMP_SWITCH_MASK) == STAMP_OFF) {
		return;
	}

	switch (g_uiThumbStampSetup & STAMP_IMG_FORMAT_MASK) {
		case STAMP_IMG_420UV:
			GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
			break;

		case STAMP_IMG_422UV:
			GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
			break;

		default:
			DBG_ERR("Only support 420/422 UV pack!\r\n");
			return;
	}

	uiStampDataHeight 			= g_ThumbStampInfo.ui32DstHeight;
	StampBuffer.uiYLineOffset   = THUMB_STAMP_WIDTH_MAX;
	StampBuffer.uiUVLineOffset  = THUMB_STAMP_WIDTH_MAX;
	StampBuffer.uiYAddr     	= g_uiThumbStampYAddr;
	StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
	StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
	StampBuffer.uiFormat        = GxImgFormat;
	uiStampDataWidth=0;

#if (VIDEOSPRITE_STAMP_FUNC == DISABLE)
	uiStampDataWidth 	= MovieStamp_GetStampDataWidth(&g_ThumbStampInfo);
	uiStampDataWidth 	= ALIGN_CEIL_4(uiStampDataWidth); // limitation of index color stamp mode

	uiPreWidth          = g_uiThumbStampWidth; // previous stamp data width
	uiWidth             = (uiPreWidth > uiStampDataWidth) ? uiPreWidth : uiStampDataWidth;
	//DBG_DUMP("^Ystamp w %d, h %d\r\n", uiWidth, uiStampDataHeight);
#endif
#if (MOVIE_AE_LOG == 0 && MOVIE_ADAS_LOG == 0)
	// Prepare date-time string
	if ((g_uiThumbStampSetup & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME) {
		switch (g_uiThumbStampSetup & STAMP_DATE_FORMAT_MASK) {
		case STAMP_DD_MM_YY:
			//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			break;
		case STAMP_MM_DD_YY:
			//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			break;
		default:
			//sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
			break;
		}
	} else if ((g_uiThumbStampSetup & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME_AMPM) {
		if (CurDateTime.tm_hour >= 12) {
			if (CurDateTime.tm_hour > 12) {
				CurDateTime.tm_hour -= 12;
			}

			switch (g_uiThumbStampSetup & STAMP_DATE_FORMAT_MASK) {
			case STAMP_DD_MM_YY:
				//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				break;
			case STAMP_MM_DD_YY:
				//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				break;
			default:
				//sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				break;
			}
		} else {
			switch (g_uiThumbStampSetup & STAMP_DATE_FORMAT_MASK) {
			case STAMP_DD_MM_YY:
				//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				break;
			case STAMP_MM_DD_YY:
				//sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				break;
			default:
				//sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
				break;
			}
		}
	} else {
		//sprintf(&g_cMovieStampStr[i][0], "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
		snprintf(&g_cThumbStampStr[0], MOVIE_STAMP_MAX_LEN, "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
	}
#endif

#if (VIDEOSPRITE_STAMP_FUNC == DISABLE)

	// Set background data
	if (g_uiThumbStampFormat == STAMP_FORMAT_IDX) { // color index format (for direct path only)
#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
		uiWidth >>= 1;
#else
		uiWidth >>= 2;
#endif
		uiBgData = COLOR_ID_BG;
	} else { // STAMP_FORMAT_YUV
		if ((g_uiThumbStampFormat & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT) {
			//uiBgData = COLORKEY_BG_YUV; // for LE op
			uiBgData = COLORKEY_YUV; // for EQ op
		} else {
			uiBgData = ((UINT32)g_ThumbStampInfo.Color[COLOR_ID_BG].ucV << 16) |
					   ((UINT32)g_ThumbStampInfo.Color[COLOR_ID_BG].ucU << 8)  |
					   g_ThumbStampInfo.Color[COLOR_ID_BG].ucY;
		}
	}

	// Clear source (stamp) buffer (in fact, index color mode doesn't need to copy Y, U, V data,
	// it only needs to copy "color index" data; but however, use GxImg function temporarily.
	uiLineOffset[0] = StampBuffer.uiYLineOffset;
	uiLineOffset[1] = StampBuffer.uiUVLineOffset;
	uiPxlAddr[0]    = StampBuffer.uiYAddr;
	uiPxlAddr[1]    = StampBuffer.uiUAddr;

	GxImg_InitBufEx(&SrcImg,
					uiWidth,
					uiStampDataHeight,
					GxImgFormat,
					uiLineOffset,
					uiPxlAddr);

	GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);
#endif

	// Draw string by getting data from date stamp font
	MovieStamp_DrawString_Thumb(0, &g_ThumbStampInfo, &StampBuffer);

#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
	//UINT32 vid=0;
	static UINT32 VsOsdWidth=0, VsOsdHight=0;
	FONT  *pOldFont=g_VsFontIn[vid].pFont;

	//start rec, and get thumb stamp data for all path
	if(bInit == TRUE){
        	g_VsFontIn[vid].pFont=(FONT *)gDateStampFontTbl12x20;
        	GxGfx_SetTextStroke((const FONT *)g_VsFontIn[vid].pFont, FONTSTYLE_NORMAL, SCALE_1X);
        	GxGfx_Text(0, 0, 0, g_ThumbStampInfo.pi8Str); //not really draw
        	szStamp = GxGfx_GetTextLastSize(); //just get text size
        	VsOsdWidth=ALIGN_CEIL(szStamp.w ,8);
        	VsOsdHight=ALIGN_CEIL(szStamp.h, 2);
        	//DBG_DUMP("VsOsdWidth=%d, %d\r\n",VsOsdWidth,VsOsdHight);

        	g_VsFontIn[vid].pStr=g_ThumbStampInfo.pi8Str;
        	if (FontConv(&g_VsFontIn[vid], &g_VsFontOut[vid]) != E_OK) {
        	    DBG_ERR("FontConv err\r\n");
        	}

        	memcpy((void *)g_uiThumbStampYAddr, (void *)g_VsFontOut[vid].GenImg.PxlAddr[0], (VsOsdWidth*VsOsdHight*2));
        	//DBG_DUMP("update pi8Str=%s\r\n", g_MovieStampInfo[i].pi8Str);
        	//DBG_DUMP("i=%d, w=%d, %d\r\n", i,g_VsOsdWidth[i], g_VsOsdHight[i]);
       }

       if(VsOsdWidth==0 || VsOsdHight==0){
            DBG_ERR("VsOsdWidth err!\r\n");
       }
       MovieStamp_VsUpdateOsd(vid, TRUE, vid,  1, g_ThumbStampPos.uiX, g_ThumbStampPos.uiY, VsOsdWidth, VsOsdHight, (void*)g_uiThumbStampYAddr);

       //disable water logo
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
       MovieStamp_VsUpdateOsd(vid, FALSE, (16+vid), 2, g_MovieWaterInfo[vid].uiXPos, g_MovieWaterInfo[vid].uiYPos, g_MovieWaterInfo[vid].uiWidth, g_MovieWaterInfo[vid].uiHeight, (void*)g_ucWaterMark_ARGB4444);
#endif

       MovieStamp_VsSwapOsd(vid);

       g_VsFontIn[vid].pFont=pOldFont;
#endif

	// Update width
	g_uiThumbStampWidth = uiStampDataWidth;
}
#endif

// copy stamp data to image buffer (for D2D path)
void MovieStamp_CopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
	GX_IMAGE_PIXEL_FMT GxImgFormat;
	STAMP_BUFFER    StampBuffer;
	IMG_BUF         SrcImg = {0}, DstImg = {0};
	UINT32          x, y;
	UINT32          uiYAddrOffset, uiUAddrOffset;
	UINT32          uiStampDataHeight;
	UINT32          uiLineOffset[2] = {0};
	UINT32          uiPxlAddr[2] = {0};
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
	STAMP_BUFFER    LingualScaledStampBuffer;
#endif

#if (CHECK_STAMP_TIME == ENABLE)
	UINT32          uiTime, uiTimeCount; // for performance measurement
#endif

	if (uiVidEncId >= VIDEO_IN_MAX) {
		//DBG_ERR("uiVidEncId = %d\r\n",uiVidEncId);
		return;
	}

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	if (uiVidEncId == 0) {
		mp4log_IncFrmCnt(1);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

	if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_SWITCH_MASK) == STAMP_OFF) {
		return;
	}

	if (yAddr == 0 || cbAddr == 0) {
		return;
	}
	if(g_VsStampEn==TRUE)	{
		return;
 	}
	//#NT#2016/08/19#Hideo Lin -begin
	//#NT#To avoid different stamp overlap for same source image
	{
#if _TODO
		UINT32 frame_rate = 0, rec_format = 0, video_id = uiVidEncId;
		MovRec_GetInfo(MOVREC_GETINFO_VIDEO_FPS, &frame_rate, &video_id, 0);
		MovRec_GetInfo(MOVREC_GETINFO_RECFORMAT, &rec_format, 0, 0);
		if ((g_uiMovieSrcYAddr[uiVidEncId] == yAddr) &&
			(rec_format != MEDIAREC_TIMELAPSE) &&
			(rec_format != MEDIAREC_MERGEYUV) &&
			(frame_rate >= 15)) {
			// if Y address of current encode path is the same as previous one (i.e. the same source image),
			// do not apply data stamp otherwise the stamp data will overlap
			return;
		}
		g_uiMovieSrcYAddr[uiVidEncId] = yAddr;
#endif
	}
	//#NT#2016/08/19#Hideo Lin -end

	switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_IMG_FORMAT_MASK) {
	case STAMP_IMG_420UV:
		GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
		break;

	case STAMP_IMG_422UV:
		GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
		break;

	default:
		DBG_ERR("Only support 420/422 UV pack!\r\n");
		return;
	}

	x = g_MovieStampPos[uiVidEncId].uiX;
	y = g_MovieStampPos[uiVidEncId].uiY;

	// Y address offset of destination image to put Y stamp data
	uiYAddrOffset = lineY * y + x;
	// UV address offset of destination image to put UV stamp data
	if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
		uiUAddrOffset = lineY * y / 2 + x;
	} else {
		uiUAddrOffset = lineY * y + x;
	}

	uiStampDataHeight = g_MovieStampInfo[uiVidEncId].ui32DstHeight;

	StampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
	StampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
	StampBuffer.uiYAddr         = g_uiMovieStampYAddr[uiVidEncId][g_uiMovieStampActBuf];
	StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
	StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
	StampBuffer.uiFormat        = GxImgFormat;

#if (CHECK_STAMP_TIME == ENABLE)
	//Perf_Open(); // for performance measurement
	uiTime = Perf_GetCurrent();
#endif

	// copy data from date stamp draw buffer to image buffer
	// Init source (stamp) buffer
	uiLineOffset[0] = StampBuffer.uiYLineOffset;
	uiLineOffset[1] = StampBuffer.uiUVLineOffset;
	uiPxlAddr[0]    = StampBuffer.uiYAddr;
	uiPxlAddr[1]    = StampBuffer.uiUAddr;

	GxImg_InitBufEx(&SrcImg,
					g_uiMovieStampWidth[uiVidEncId],
					uiStampDataHeight,
					GxImgFormat,
					uiLineOffset,
					uiPxlAddr);

	// Init destination (image) buffer
	uiLineOffset[0] = lineY;
	uiLineOffset[1] = lineY;
	uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
	uiPxlAddr[1]    = cbAddr + uiUAddrOffset;

	GxImg_InitBufEx(&DstImg,
					imgWidth,
					240, // don't care, but should be > 2
					GxImgFormat,
					uiLineOffset,
					uiPxlAddr);

	if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT) {
		GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE, GX_IMAGE_CP_ENG_DFT);
	} else {
		GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
	}

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
	uiYAddrOffset = lineY * g_LingualStampPos[uiVidEncId].uiY + g_LingualStampPos[uiVidEncId].uiX;
	if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
		uiUAddrOffset = lineY * g_LingualStampPos[uiVidEncId].uiY / 2 + g_LingualStampPos[uiVidEncId].uiX;
	} else {
		uiUAddrOffset = lineY * g_LingualStampPos[uiVidEncId].uiY + g_LingualStampPos[uiVidEncId].uiX;
	}

	// Update scaling factor
	switch (imgWidth) {
	case  1920: // 1920x1080
	case  1536: // 1536x1536, 26x44
		g_uiLingualStampScalingFactor[uiVidEncId] = 44;
		break;
	case  1440: // 1440x1080
	case  1728: // 1728x1296, (DAR 16:9)20x44
		g_uiLingualStampScalingFactor[uiVidEncId] = 33;
		break;
	case  1280: // 1280x720, 18x30
		g_uiLingualStampScalingFactor[uiVidEncId] = 30;
		break;
	case  640: //  640x480, 12x20
	case  848: //  848x480, 12x20
		g_uiLingualStampScalingFactor[uiVidEncId] = 20;
		break;
	default: // 320x240, 10x16
		g_uiLingualStampScalingFactor[uiVidEncId] = 1;
		break;
	}

	if (g_uiLingualStampActBuf != LINGUAL_STAMP_BUF_OFF) {

		LingualScaledStampBuffer.uiYLineOffset    = STAMP_WIDTH_MAX;
		LingualScaledStampBuffer.uiUVLineOffset   = STAMP_WIDTH_MAX;
		LingualScaledStampBuffer.uiYAddr          = g_uiLingualScaledStampYAddr[uiVidEncId][g_uiLingualStampActBuf];
		LingualScaledStampBuffer.uiUAddr          = LingualScaledStampBuffer.uiYAddr + LingualScaledStampBuffer.uiYLineOffset * g_uiLingualScaledStampHeight;
		LingualScaledStampBuffer.uiVAddr          = LingualScaledStampBuffer.uiUAddr; // for 420 UV pack
		LingualScaledStampBuffer.uiFormat         = GxImgFormat;
		uiLineOffset[0] = LingualScaledStampBuffer.uiYLineOffset;
		uiLineOffset[1] = LingualScaledStampBuffer.uiUVLineOffset;
		uiPxlAddr[0]    = LingualScaledStampBuffer.uiYAddr;
		uiPxlAddr[1]    = LingualScaledStampBuffer.uiUAddr;
		GxImg_InitBufEx(&SrcImg,
						g_uiLingualScaledStampWidth[uiVidEncId][g_uiLingualStampActBuf],
						g_uiLingualScaledStampHeight,
						GxImgFormat,
						uiLineOffset,
						uiPxlAddr);

		// Init destination (image) buffer
		uiLineOffset[0] = lineY;
		uiLineOffset[1] = lineY;
		uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
		uiPxlAddr[1]    = cbAddr + uiUAddrOffset;

		GxImg_InitBufEx(&DstImg,
						imgWidth,
						240, // don't care, but should be > 2
						GxImgFormat,
						uiLineOffset,
						uiPxlAddr);

		if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT) {
			GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE, GX_IMAGE_CP_ENG_DFT);
		} else {
			GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
		}
	}
#endif
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
	{
		if (g_bWaterLogoEnable[uiVidEncId]) {
			UINT32      x2, y2, uiYAddrOffset2, uiUAddrOffset2;
			IMG_BUF     SrcImg2, DstImg2;

			x2 = g_MovieWaterInfo[uiVidEncId].uiXPos;
			y2 = g_MovieWaterInfo[uiVidEncId].uiYPos;

			uiYAddrOffset2 = lineY * y2 + x2;
			if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
				uiUAddrOffset2 = lineY * y2 / 2 + x2;
			} else {
				uiUAddrOffset2 = lineY * y2 + x2;
			}

			// Init source (watermark) buffer
			uiLineOffset[0] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
			if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
				uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset * 2; // *2 for YUV422 => YUV420
			} else { // GX_IMAGE_PIXEL_FMT_YUV422_PACKED
				uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
			}
			uiPxlAddr[0]    = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
			uiPxlAddr[1]    = g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr;

			GxImg_InitBufEx(&SrcImg2,
							g_MovieWaterInfo[uiVidEncId].uiWidth,
							g_MovieWaterInfo[uiVidEncId].uiHeight,
							GxImgFormat,
							uiLineOffset,
							uiPxlAddr);

			// Init destination (image) buffer
			uiLineOffset[0] = lineY;
			uiLineOffset[1] = lineY;
			uiPxlAddr[0]    = yAddr  + uiYAddrOffset2;
			uiPxlAddr[1]    = cbAddr + uiUAddrOffset2;

			GxImg_InitBufEx(&DstImg2,
							imgWidth,
							240, // don't care, but should be > 2
							GxImgFormat,
							uiLineOffset,
							uiPxlAddr);

			if (g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey == WATERLOGO_FILL_COLOR) {
				GxImg_CopyData(&SrcImg2, REGION_MATCH_IMG, &DstImg2, REGION_MATCH_IMG);
			} else {
				GxImg_CopyColorkeyData(&DstImg2, REGION_MATCH_IMG, &SrcImg2, REGION_MATCH_IMG, g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey, FALSE, GX_IMAGE_CP_ENG_DFT);
			}
		}
	}
#endif

	//#NT#2016/06/08#Lincy Lin -begin
	//#NT#Implement generic OSD and video drawing mechanism for ALG function
	{
		UINT32   imgHeight;
		// Init destination (image) buffer
		uiLineOffset[0] = lineY;
		uiLineOffset[1] = lineY;
		uiPxlAddr[0]    = yAddr;
		uiPxlAddr[1]    = cbAddr;
		imgHeight       = (cbAddr - yAddr) / lineY;

		GxImg_InitBufEx(&DstImg,
						imgWidth,
						imgHeight,
						GxImgFormat,
						uiLineOffset,
						uiPxlAddr);
		MovieStamp_DrawALG(uiVidEncId, &DstImg);
	}
	//#NT#2016/06/08#Lincy Lin -end
#if (CHECK_STAMP_TIME == ENABLE)
	//DBG_DUMP("time3: %d us\r\n", Perf_GetCurrent() - uiTime);
	{
		static UINT32 k = 0;
		static UINT32 uiTime3 = 0;

		uiTimeCount = Perf_GetCurrent() - uiTime;
		uiTime3 += uiTimeCount;
		k++;
		if (k == 10) {
			DBG_DUMP("^Gtime3: %d us\r\n", uiTime3 / 10);
			uiTime3 = 0;
			k = 0;
		}
	}
	//Perf_Close();
#endif
}

#if (THUMB_STAMP_FUNC == ENABLE)
// copy stamp data to thumbnail image buffer (for D2D path)
void MovieStamp_CopyData_Thumb(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
	GX_IMAGE_PIXEL_FMT GxImgFormat;
	STAMP_BUFFER    StampBuffer;
	IMG_BUF         SrcImg = {0}, DstImg = {0};
	UINT32          x, y;
	UINT32          uiYAddrOffset, uiUAddrOffset;
	UINT32          uiStampDataHeight;
	UINT32          uiLineOffset[2] = {0};
	UINT32          uiPxlAddr[2] = {0};

	if (uiVidEncId >= VIDEO_IN_MAX) {
		//DBG_ERR("uiVidEncId = %d\r\n",uiVidEncId);
		return;
	}

	if ((g_uiThumbStampSetup & STAMP_SWITCH_MASK) == STAMP_OFF) {
		return;
	}

	if (yAddr == 0 || cbAddr == 0) {
		return;
	}
	if(g_VsStampEn==TRUE)	{
		return;
 	}

	switch (g_uiThumbStampSetup & STAMP_IMG_FORMAT_MASK) {
	case STAMP_IMG_420UV:
		GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
		break;

	case STAMP_IMG_422UV:
		GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
		break;

	default:
		DBG_ERR("Only support 420/422 UV pack!\r\n");
		return;
	}

	x = g_ThumbStampPos.uiX;
	y = g_ThumbStampPos.uiY;

	// Y address offset of destination image to put Y stamp data
	uiYAddrOffset = lineY * y + x;
	// UV address offset of destination image to put UV stamp data
	if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
		uiUAddrOffset = lineY * y / 2 + x;
	} else {
		uiUAddrOffset = lineY * y + x;
	}

	uiStampDataHeight = g_ThumbStampInfo.ui32DstHeight;

	StampBuffer.uiYLineOffset   = 640;
	StampBuffer.uiUVLineOffset  = 640;
	StampBuffer.uiYAddr         = g_uiThumbStampYAddr;
	StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
	StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
	StampBuffer.uiFormat        = GxImgFormat;

	// copy data from date stamp draw buffer to image buffer
	// Init source (stamp) buffer
	uiLineOffset[0] = StampBuffer.uiYLineOffset;
	uiLineOffset[1] = StampBuffer.uiUVLineOffset;
	uiPxlAddr[0]    = StampBuffer.uiYAddr;
	uiPxlAddr[1]    = StampBuffer.uiUAddr;

	GxImg_InitBufEx(&SrcImg,
					g_uiThumbStampWidth,
					uiStampDataHeight,
					GxImgFormat,
					uiLineOffset,
					uiPxlAddr);

	// Init destination (image) buffer
	uiLineOffset[0] = lineY;
	uiLineOffset[1] = lineY;
	uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
	uiPxlAddr[1]    = cbAddr + uiUAddrOffset;

	GxImg_InitBufEx(&DstImg,
					imgWidth,
					240, // don't care, but should be > 2
					GxImgFormat,
					uiLineOffset,
					uiPxlAddr);

	if ((g_uiThumbStampSetup & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT) {
		GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE, GX_IMAGE_CP_ENG_DFT);
	} else {
		GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
	}

	UINT32   imgHeight;
	// Init destination (image) buffer
	uiLineOffset[0] = lineY;
	uiLineOffset[1] = lineY;
	uiPxlAddr[0]    = yAddr;
	uiPxlAddr[1]    = cbAddr;
	imgHeight       = (cbAddr - yAddr) / lineY;

	GxImg_InitBufEx(&DstImg,
					imgWidth,
					imgHeight,
					GxImgFormat,
					uiLineOffset,
					uiPxlAddr);
	MovieStamp_DrawALG(uiVidEncId, &DstImg);
}
#endif

//-------------------------------------------------------------------------------------------------
// set stamp data to IME (for direct path)

// max. 16 colors for index color
#define STAMP_IDX_COLOR_MAX     16

static UINT32 g_ColorLutY[VIDEO_IN_MAX][STAMP_IDX_COLOR_MAX] = {0};
static UINT32 g_ColorLutU[VIDEO_IN_MAX][STAMP_IDX_COLOR_MAX] = {0};
static UINT32 g_ColorLutV[VIDEO_IN_MAX][STAMP_IDX_COLOR_MAX] = {0};

void MovieStamp_InitImeStamp(UINT32 uiVidEncId)
{
	// Stamp background color
	if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT) {
		g_ColorLutY[uiVidEncId][COLOR_ID_BG] = COLOR_CK_Y;
		g_ColorLutU[uiVidEncId][COLOR_ID_BG] = COLOR_CK_U;
		g_ColorLutV[uiVidEncId][COLOR_ID_BG] = COLOR_CK_V;
	} else {
		g_ColorLutY[uiVidEncId][COLOR_ID_BG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY;
		g_ColorLutU[uiVidEncId][COLOR_ID_BG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU;
		g_ColorLutV[uiVidEncId][COLOR_ID_BG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV;
	}

	// Stamp frame color
	g_ColorLutY[uiVidEncId][COLOR_ID_FR] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucY;
	g_ColorLutU[uiVidEncId][COLOR_ID_FR] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucU;
	g_ColorLutV[uiVidEncId][COLOR_ID_FR] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucV;

	// Stamp foreground color (text body)
	g_ColorLutY[uiVidEncId][COLOR_ID_FG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucY;
	g_ColorLutU[uiVidEncId][COLOR_ID_FG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucU;
	g_ColorLutV[uiVidEncId][COLOR_ID_FG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucV;
}

void MovieStamp_SetImeStamp(UINT32 uiVidEncId, IMESTAMP_INFO *pImeStamp)
{
	// IME set0: support YUYV, for waterlogo
	// IME set1~3: support color index, for general stamp data

	// just enable/disable set1 temporarily
	if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_SWITCH_MASK) == STAMP_ON) {
		pImeStamp->DS_Set1.DSEn = TRUE;
#if _TODO
#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
		pImeStamp->DS_Set1.DSImgInfo.DsMode             = _4B; // use 4-bit color index
#else
		pImeStamp->DS_Set1.DSImgInfo.DsMode             = _2B; // use 2-bit color index
#endif
#endif
		pImeStamp->DS_Set1.DSImgInfo.DS_SIZE.uiSizeH    = g_uiMovieStampWidth[uiVidEncId];
		pImeStamp->DS_Set1.DSImgInfo.DS_SIZE.uiSizeV    = g_MovieStampInfo[uiVidEncId].ui32DstHeight;
		pImeStamp->DS_Set1.DSImgInfo.DsBlendPos.x       = g_MovieStampPos[uiVidEncId].uiX;
		pImeStamp->DS_Set1.DSImgInfo.DsBlendPos.y       = g_MovieStampPos[uiVidEncId].uiY;
		pImeStamp->DS_Set1.DSImgInfo.uiDSLineoffset     = STAMP_WIDTH_MAX; // use fixed stamp buffer size temporarily
		pImeStamp->DS_Set1.DSImgInfo.uiDSAddress        = g_uiMovieStampYAddr[uiVidEncId][g_uiMovieStampActBuf];
#if _TODO
		pImeStamp->DS_Set1.DSIqInfo.uiDsColorKY         = COLOR_CK_Y;
		pImeStamp->DS_Set1.DSIqInfo.uiDsColorKU         = COLOR_CK_U;
		pImeStamp->DS_Set1.DSIqInfo.uiDsColorKV         = COLOR_CK_V;
		pImeStamp->DS_Set1.DSIqInfo.uiBlendWt           = 0x0F; // 4-bit precision
		pImeStamp->ColorLutYAddr                        = (UINT32)&g_ColorLutY[uiVidEncId][0];
		pImeStamp->ColorLutUAddr                        = (UINT32)&g_ColorLutU[uiVidEncId][0];
		pImeStamp->ColorLutVAddr                        = (UINT32)&g_ColorLutV[uiVidEncId][0];
#endif


#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
		if (g_bWaterLogoEnable[uiVidEncId]) {
			pImeStamp->DS_Set0.DSEn = TRUE;
			pImeStamp->DS_Set0.DSImgInfo.DS_SIZE.uiSizeH    = g_MovieWaterInfo[uiVidEncId].uiWidth;
			pImeStamp->DS_Set0.DSImgInfo.DS_SIZE.uiSizeV    = g_MovieWaterInfo[uiVidEncId].uiHeight;
			pImeStamp->DS_Set0.DSImgInfo.DsBlendPos.x       = g_MovieWaterInfo[uiVidEncId].uiXPos;
			pImeStamp->DS_Set0.DSImgInfo.DsBlendPos.y       = g_MovieWaterInfo[uiVidEncId].uiYPos;
			pImeStamp->DS_Set0.DSImgInfo.uiDSLineoffset     = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset * 2;
			pImeStamp->DS_Set0.DSImgInfo.uiDSAddress        = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
#if _TODO
			pImeStamp->DS_Set0.DSIqInfo.uiDsColorKY         = g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey & 0xFF;
			pImeStamp->DS_Set0.DSIqInfo.uiDsColorKU         = (g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey & 0xFF00) >> 8;
			pImeStamp->DS_Set0.DSIqInfo.uiDsColorKV         = (g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey & 0xFF0000) >> 16;
			pImeStamp->DS_Set0.DSIqInfo.uiBlendWt           = 0x0F; // 4-bit precision
#endif
		}
#endif
	} else {
		pImeStamp->DS_Set1.DSEn = FALSE;
	}

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	if (uiVidEncId == 0) {
		mp4log_IncFrmCnt(1);
	}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
}

//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function

void MovieStamp_DrawLineUV(INT32 x1, INT32 y1, INT32 x2, INT32 y2, UINT32 DrawColor, PIMG_BUF pDstImg)
{
	INT32 x, y, x2x1diff, y2y1diff;
	UINT32 LineOff_UV;
	UINT16 CbCr;
	UINT32 UVAddr;

	if (x1 >= (INT32)pDstImg->Width ||
		y1 >= (INT32)pDstImg->Height ||
		x2 >= (INT32)pDstImg->Width ||
		y2 >= (INT32)pDstImg->Height) {
		return;
	}

	if (pDstImg->PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
		return;
	}

	UVAddr = pDstImg->PxlAddr[1];
	LineOff_UV = pDstImg->LineOffset[1];
	CbCr = COLOR_YUVD_GET_U(DrawColor) | (COLOR_YUVD_GET_V(DrawColor) << 8);

	if (y1 == y2) {
		x1 &= ~0x00000001; //address U must be even (UV packed)
		x2 &= ~0x00000001; //address U must be even (UV packed)
		y = y1;
		for (x = min(x1, x2); x < max(x1, x2); x += 2) {
			*(UINT16 *)(UVAddr + y / 2 * LineOff_UV + x) = CbCr;
		}
	} else if (x1 == x2) {
		x = x1 & ~0x00000001;
		for (y = min(y1, y2); y < max(y1, y2); y++) {
			*(UINT16 *)(UVAddr + y / 2 * LineOff_UV + x) = CbCr;
		}
	} else {
		if (y1 > y2) {
			swap(x1, x2);
			swap(y1, y2);
		}

		x2x1diff = x2 - x1;
		y2y1diff = y2 - y1;

		if (abs(x2x1diff) > abs(y2y1diff)) {
			x1 &= ~0x00000001; //address U must be even (UV packed)
			x2 &= ~0x00000001; //address U must be even (UV packed)
			for (x = min(x1, x2); x < max(x1, x2); x += 2) {
				y = y1 + y2y1diff * (x - x1) / x2x1diff;
				*(UINT16 *)(UVAddr + y / 2 * LineOff_UV + x) = CbCr;
			}
		} else {
			for (y = y1; y < y2; y++) {
				x = x1 + x2x1diff * (y - y1) / y2y1diff;
				x &= ~0x00000001; //address U must be even (UV packed)
				*(UINT16 *)(UVAddr + y / 2 * LineOff_UV + x) = CbCr;
			}
		}
	}
}

void MovieStamp_DrawPointUV(INT32 x, INT32 y, UINT32 DrawColor, PIMG_BUF pDstImg)
{
	UINT32 LineOff_UV;
	UINT32 UVAddr;
	UINT16 CbCr;

	if (x >= (INT32)pDstImg->Width ||
		y >= (INT32)pDstImg->Height) {
		return;
	}

	if (pDstImg->PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
		return;
	}

	UVAddr = pDstImg->PxlAddr[1];
	LineOff_UV = pDstImg->LineOffset[1];
	CbCr = COLOR_YUVD_GET_U(DrawColor) | (COLOR_YUVD_GET_V(DrawColor) << 8);

	x &= ~0x00000001; //address U must be even (UV packed)
	*(UINT16 *)(UVAddr + y / 2 * LineOff_UV + x) = CbCr;
}

void MovieStamp_DrawRectUV(IRECT *pRect, UINT32 DrawColor, PIMG_BUF pDstImg)
{
	MovieStamp_DrawLineUV(pRect->x, pRect->y, pRect->x + pRect->w, pRect->y, DrawColor, pDstImg);
	MovieStamp_DrawLineUV(pRect->x, pRect->y, pRect->x, pRect->y + pRect->h, DrawColor, pDstImg);
	MovieStamp_DrawLineUV(pRect->x + pRect->w, pRect->y, pRect->x + pRect->w, pRect->y + pRect->h, DrawColor, pDstImg);
	MovieStamp_DrawLineUV(pRect->x, pRect->y + pRect->h, pRect->x + pRect->w, pRect->y + pRect->h, DrawColor, pDstImg);
}

void MovieStamp_DrawRect(PIMG_BUF pDstImg, IRECT  *DrawRect, UINT32 drawColor, UINT32 border)
{
	IRECT  LineRect;

	if (border < 2) {
		border = 2;
	}

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y;
	LineRect.w = DrawRect->w;
	LineRect.h = border;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y + DrawRect->h - border;
	LineRect.w = DrawRect->w;
	LineRect.h = border;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y;
	LineRect.w = border;
	LineRect.h = DrawRect->h;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x + DrawRect->w - border;
	LineRect.y = DrawRect->y;
	LineRect.w = border;
	LineRect.h = DrawRect->h;
	GxImg_FillData(pDstImg, &LineRect, drawColor);
}
#if (MOVIE_FR_FUNC== ENABLE)
UINT32 MovieStamp_GetFaceNameBufAddr(UINT32 FaceId, UINT32 blk_size)
{
        NVTMPP_VB_BLK  blk=0;
        CHAR pool_name[20] ={0};

        //DBG_ERR("g_FaceNameStampPool[%d]=%d\r\n",FaceId,g_FaceNameStampPool[FaceId]);

        if(g_FaceNameStampPool[FaceId]==NVTMPP_VB_INVALID_POOL)  {
            sprintf(pool_name,"FaceNameStamp_%d",FaceId);
            g_FaceNameStampPool[FaceId]=nvtmpp_vb_create_pool(pool_name, blk_size , 1, NVTMPP_DDR_1);
            if (NVTMPP_VB_INVALID_POOL == g_FaceNameStampPool[FaceId])	{
        		DBG_ERR("create private pool err\r\n");
        		return NVTEVT_CONSUME;
            }

            blk = nvtmpp_vb_get_block(0, g_FaceNameStampPool[FaceId], blk_size, NVTMPP_DDR_1);
            if (NVTMPP_VB_INVALID_BLK == blk) {
        		DBG_ERR("get vb block err\r\n");
                	return NVTEVT_CONSUME;
            }
            g_FaceNameStampPoolAddr[FaceId]=nvtmpp_vb_block2addr(blk);
        }
        if(g_FaceNameStampPoolAddr[FaceId] == 0)
    		DBG_ERR("get buf addr err\r\n");
        return g_FaceNameStampPoolAddr[FaceId];
}
void MovieStamp_DestroyFaceNameBuff(void)
{
	INT32 i, ret;
	for (i=0;i<FACE_CNT_MAX;i++) {
		if(g_FaceNameStampPool[i] != NVTMPP_VB_INVALID_POOL){
			ret = nvtmpp_vb_destroy_pool(g_FaceNameStampPool[i]);
			if (NVTMPP_ER_OK != ret) {
				DBG_ERR("destory pool error pool 0x%x   ret=%d\r\n", g_FaceNameStampPool[i], ret);
				break;
			}
			g_FaceNameStampPool[i]=NVTMPP_VB_INVALID_POOL;
			g_FaceNameStampPoolAddr[i]=0;
		}
	}
}
#endif
void MovieStamp_DrawFaceName(PIMG_BUF pDstImg, UINT32 FaceId, UPOINT *Pos, char *pName)
{
#if (MOVIE_FR_FUNC== ENABLE)
    UINT32          uiLineOffset[2] = {0};
    UINT32          uiPxlAddr[2] = {0};
    IMG_BUF         SrcImg = {0}, DstImg = {0};
    UINT16 uiFontWidth=0;
    UINT16 uiFontHeight;
    UINT16 uiMaxNameCharCnt=15;
    UINT16 uiMaxFontWidth=42;

    STAMP_BUFFER    StampBuffer;
    ICON_DB const   *pDB;
    UINT32 uiIconID, i, j;

    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
    STAMP_INFO   sMovieStampInfo;
    if(FaceId>= FACE_CNT_MAX){
        DBG_ERR("FaceId over flow ,FaceId=%d\r\n",FaceId);
        return;
    }


    if((Pos->x >pDstImg->Width)  || (Pos->y > pDstImg->Height)){
        DBG_ERR("x=%d, y=%d\r\n",Pos->x,Pos->y);
        return;
    }

    if(pName == NULL){
        return;
    }
    if(strlen(pName) == 0){
        return;
    }

    //pDB = &gDateStampFont26x44;
    pDB = &gDateStampFont_WautoH44;
    uiIconID = pName[0] -  pDB->uiDrawStrOffset; // 1st font
    uiFontHeight=pDB->pIconHeader[uiIconID].uiHeight;
    sMovieStampInfo.pi8Str=pName;
    sMovieStampInfo.pDataBase=pDB;
    //icon width must be even
    uiFontWidth=MovieStamp_GetStampDataWidth(&sMovieStampInfo);
    uiFontWidth=ALIGN_CEIL_4(uiFontWidth);
    //DBG_DUMP("uiFontWidth=%d\r\n",uiFontWidth);
    if(uiFontWidth > uiMaxNameCharCnt*uiMaxFontWidth){
        DBG_ERR("buffer over flow, wid=%d, %d\r\n",uiFontWidth,uiMaxNameCharCnt*uiMaxFontWidth);
        return;
    }
    //StampBuffer.uiYAddr     = MovieStamp_GetFaceBufAddr(FaceId, (uiFontWidth*uiFontHeight*3/2)); //YUV 420
    StampBuffer.uiYAddr     = MovieStamp_GetFaceNameBufAddr(FaceId, (uiMaxNameCharCnt*uiMaxFontWidth*uiFontHeight*3/2)); //YUV 420, Get Max Buffer

    StampBuffer.uiYLineOffset=uiFontWidth;
    StampBuffer.uiUVLineOffset=StampBuffer.uiYLineOffset;
    StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiFontHeight;
    StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
    StampBuffer.uiFormat        = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;

    uiLineOffset[0] = StampBuffer.uiYLineOffset;
    uiLineOffset[1] = StampBuffer.uiUVLineOffset;
    uiPxlAddr[0]    = StampBuffer.uiYAddr;
    uiPxlAddr[1]    = StampBuffer.uiUAddr;

    GxImg_InitBufEx(&SrcImg,
    				uiFontWidth,
    				uiFontHeight,
    				StampBuffer.uiFormat,
    				uiLineOffset,
    				uiPxlAddr);

    GxImg_FillData(&SrcImg, REGION_MATCH_IMG, COLORKEY_YUV);



    uiFBAddrY  = StampBuffer.uiYAddr;
    uiFBAddrCb = StampBuffer.uiUAddr;
    uiFBAddrCr = StampBuffer.uiVAddr;

    STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
    //STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 0, 0), RGB_GET_U(224, 0, 0), RGB_GET_V(224, 0, 0)}; // date stamp foreground color
    STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

    // Stamp frame color
    sMovieStampInfo.Color[COLOR_ID_FR].ucY = StampColorFr.ucY;
    sMovieStampInfo.Color[COLOR_ID_FR].ucU = StampColorFr.ucU;
    sMovieStampInfo.Color[COLOR_ID_FR].ucV = StampColorFr.ucV;
    // Stamp foreground color (text body)
    sMovieStampInfo.Color[COLOR_ID_FG].ucY = StampColorFg.ucY;
    sMovieStampInfo.Color[COLOR_ID_FG].ucU = StampColorFg.ucU;
    sMovieStampInfo.Color[COLOR_ID_FG].ucV = StampColorFg.ucV;

    for (i = 0; i < strlen(pName); i++) {
        //get icon database
        uiIconID = pName[i] -  pDB->uiDrawStrOffset;
        j = pDB->pIconHeader[uiIconID].uiWidth;


        if (StampBuffer.uiFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
            MovieStamp_DrawIcon420UV(&sMovieStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, StampBuffer.uiYLineOffset);
        } else {
            MovieStamp_DrawIcon422UV(&sMovieStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, StampBuffer.uiYLineOffset);
        }

        uiFBAddrY  += (j);
        uiFBAddrCb += (j);
        uiFBAddrCr += (j);
    }

    UINT32          uiYAddrOffset, uiUAddrOffset;
    Pos->x=ALIGN_CEIL_2(Pos->x);
    uiYAddrOffset = (pDstImg->LineOffset[0]) * (Pos->y) + (Pos->x);
    // UV address offset of destination image to put UV stamp data
    if (StampBuffer.uiFormat  == GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
        uiUAddrOffset = (pDstImg->LineOffset[0]) * (Pos->y / 2) + Pos->x;
    } else {
        uiUAddrOffset = (pDstImg->LineOffset[0]) * (Pos->y) + Pos->x;
    }

    uiLineOffset[0] = pDstImg->LineOffset[0];
    uiLineOffset[1] = pDstImg->LineOffset[1];
    uiPxlAddr[0]    = pDstImg->PxlAddr[0]  + uiYAddrOffset;
    uiPxlAddr[1]    = pDstImg->PxlAddr[1] + uiUAddrOffset;

    GxImg_InitBufEx(&DstImg,
    pDstImg->Width,
    240, // don't care, but should be > 2
    StampBuffer.uiFormat,
    uiLineOffset,
    uiPxlAddr);

    GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE, GX_IMAGE_CP_ENG_DFT);
#endif
}



//#NT#2018/03/22#Brain Yen -begin
//#NT#add videosprite stamp function
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
void MovieStamp_VsConfigOsd(CodecOSDIMAGE *image, unsigned int id, UINT32 layer, UINT32 region, UINT32 vid, UINT32 enable, VDS_FMT fmt, UINT32 x, UINT32 y, UINT32 w, UINT32 h, void *data)
{
    memset(image, 0, sizeof(CodecOSDIMAGE));
    image->id                 = id;
    image->phase             = VDS_PHASE_BTN;
    image->data.btn.layer     = layer;
    image->data.btn.region    = region;
    image->data.btn.vid       = vid;
    image->data.btn.en        = enable;
    image->data.btn.fmt       = fmt;
    image->data.btn.x         = x;
    image->data.btn.y         = y;
    image->data.btn.w         = w;
    image->data.btn.h         = h;
    image->data.btn.alpha     = 255;
    image->data.btn.addr      = (UINT32)data;
}
ER MovieStamp_VsAllocOsdBuf(VDS_PHASE phase, UINT32 uiVidEncId, UINT32 width, UINT32 height)
{
    int Result=0;
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
    //DBG_DUMP("Water %d, height=%d\r\n", g_WaterLogo.uiWidth, g_WaterLogo.uiHeight);
    g_uiVsStampSize = videosprite_query_size(phase, VDS_BUF_TYPE_PING_PONG, g_WaterLogo.uiWidth, g_WaterLogo.uiHeight);
    videosprite_config_ping_pong_buf(phase, (16+uiVidEncId), g_uiWaterLogoYAddr[uiVidEncId], g_uiVsStampSize);
#endif

    //DBG_DUMP("width=%d, height=%d\r\n",width, height);
    g_uiVsStampSize = videosprite_query_size(phase, VDS_BUF_TYPE_PING_PONG, width, height);
    Result=videosprite_config_ping_pong_buf(phase, uiVidEncId, g_uiVsStampAddr[uiVidEncId], g_uiVsStampSize);
    return Result;
}
#endif
ER MovieStamp_VsConfig(UINT32 uiVidEncId, UINT32 uiOSDWidth, UINT32 uiOSDHeight)
{
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    g_VsStampEn =TRUE;
    if (MovieStamp_VsAllocOsdBuf(VDS_PHASE_BTN, uiVidEncId, uiOSDWidth, uiOSDHeight)) {
        DBG_ERR("fail to allocate buffer for BTN image 0\r\n");
        return -1;
    }
#endif
    return TRUE;
}
ER MovieStamp_VsUpdateOsd(UINT32 uiVidEncId, UINT32 enable, UINT32 id, UINT32 region, UINT32 uiX, UINT32 uiY, UINT32 uiOSDWidth, UINT32 uiOSDHeight, void *picture)
{
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    CodecOSDIMAGE image;
    //DBG_DUMP("uiX=%d, uiY=%d\r\n",uiX,uiY);
    //DBG_DUMP("Add buf 0\r\n");
    //********** 1. config and add osd#0 to btn output#2 **********//
    MovieStamp_VsConfigOsd(&image, id, 1, region, uiVidEncId, enable, VDS_FMT_PICTURE_ARGB4444, uiX, uiY, uiOSDWidth, uiOSDHeight, picture);
    if (videoimage_add(&image)) {
        DBG_ERR("fail to add btn osd 0\r\n");
        return -1;
    }

    //********** 3. render osd#0 & osd#1 to btn output#2**********//
    //MovieStamp_VsSwapOsd(uiVidEncId);
#endif
    return TRUE;
}
void MovieStamp_VsFontConfig(UINT32 uiVidEncId)
{
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    g_VsFontIn[uiVidEncId].ciSolid = 0xFFFF;
    g_VsFontIn[uiVidEncId].ciFrame = 0xF000;
    g_VsFontIn[uiVidEncId].ciTransparet = 0x0000;
    g_VsFontIn[uiVidEncId].Format = GX_IMAGE_PIXEL_FMT_ARGB4444_PACKED;
    g_VsFontIn[uiVidEncId].bEnableSmooth = FALSE;
    g_VsFontIn[uiVidEncId].ScaleFactor = 65536; //1.0x
#endif
}
void MovieStamp_VsAllocFontBuf(UINT32 uiVidEncId, UINT32 width, UINT32 height)
{
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    g_uiVsFontSize = width*height*VSFONT_BUF_RATIO/100; //5.5xWxH
     //DBG_DUMP("font vid=%d, YAddr=0x%x, size=0x%x, end=0x%x\r\n",uiVidEncId,g_uiVsFontAddr[uiVidEncId],g_uiVsFontSize, (g_uiVsFontAddr[uiVidEncId]+g_uiVsFontSize));

    g_VsFontIn[uiVidEncId].MemAddr = g_uiVsFontAddr[uiVidEncId];
    g_VsFontIn[uiVidEncId].MemSize = g_uiVsFontSize;
#endif
}

void MovieStamp_VsClose(void)
{
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    g_VsStampEn=FALSE;
    videosprite_lock();
    videosprite_reset();
    videosprite_release();
#endif
}
void MovieStamp_VsSwapOsd(UINT32 uiVidEncId)
{
#if(VIDEOSPRITE_STAMP_FUNC == ENABLE)
    videosprite_lock();
    videoimage_swap_btn_ext(uiVidEncId);
    videosprite_release();
#endif
}
//#NT#2018/03/22#Brain Yen -end
BOOL MovieStamp_IsEnable(void)
{
#if(VIDEOSPRITE_STAMP_FUNC == DISABLE)
    return (g_bMovieStampSwTimerOpen & MovieStampTsk_IsOpen());
#else
    return (g_bMovieStampSwTimerOpen & MovieStampTsk_IsOpen() & (BOOL)g_VsStampEn);
#endif
}
