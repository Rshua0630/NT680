#include "Type.h"
#include "GxSystem.h"
#include "GxDisplay.h"
#include "SysCfg.h"
#include "UIAppPlay.h"
#include "PlaybackTsk.h"
#include "UIPlayComm.h"

////////////////////////////////////////////////////////////////////////////////////////////
#include "GxGfx.h"
//#include "GxImageJPG.h"
#include "GxImage.h"
#include "UIPlaySlide.h"
///////////////////////////////////////////////////////////////////////


//#NT#2010/01/14#Steven feng -begin
#define UIBASE_W 640
//#NT#2010/01/14#Steven feng -end

UINT32 gPlaySSP_full_w = 0;
UINT32 gPlaySSP_full_h = 0;

UINT32 gPlaySSP_buf_w = 0;
UINT32 gPlaySSP_buf_h = 0;
UINT32 gPlaySSP_buf_a1 = 0;
UINT32 gPlaySSP_buf_a2 = 0;
UINT32 gPlaySSP_win_x = 0;
UINT32 gPlaySSP_win_y = 0;
UINT32 gPlaySSP_win_w = 0;
UINT32 gPlaySSP_win_h = 0;

IMAGE *gPlaySSP_Img1 = 0;
IMAGE *gPlaySSP_Img2 = 0;
//#NT#2011/11/01#Lincy Lin -begin
//#NT#650GxImage
IMG_BUF gPlaySSImgBuf1 = {0};
IMG_BUF gPlaySSImgBuf2 = {0};
//#NT#2011/11/01#Lincy Lin -end

UINT32 gPlaySSP_iEffect = 0;

void UISlide_OpenView(UIPlay_SlideView *pViewCfg, BOOL bOpenFromPB)
{
	gPlaySSP_full_w = pViewCfg->scr_w;
	gPlaySSP_full_h = pViewCfg->scr_h;
	gPlaySSP_win_x = pViewCfg->x;
	gPlaySSP_win_y = pViewCfg->y;
	gPlaySSP_win_w = pViewCfg->w;
	gPlaySSP_win_h = pViewCfg->h;
	gPlaySSP_Img1 = pViewCfg->pMyImg1;
	gPlaySSP_Img2 = pViewCfg->pMyImg2;
	if (bOpenFromPB) {
		//#NT#2010/02/10#Ben Wang -begin
		//#NT#fix the bug(F0407) that press "start" to slideshow will display the previous frame buffer
		UIPlay_PlaySingle(PB_SINGLE_CURR);
		//#NT#2010/02/10#Ben Wang -end
	}
}

void UISlide_CloseView(BOOL bReturnToPB)
{
	if (bReturnToPB) {
		//#NT#2010/03/03#Scottie -begin
		//#NT#Need to re-play current image to avoid using wrong IDE setting
		UIPlay_PlaySingle(PB_SINGLE_CURR);
		//#NT#2010/03/03#Scottie -end
	}
}

typedef struct _RAW_IMAGE {
	UINT16 w, h, pitch;
	UINT16 fmt;
	UINT32 address;
	UINT32 size;
}
RAW_IMAGE;

#define MAKE_ALIGN(s, bytes)    ((((UINT32)(s))+((bytes)-1)) & ~((UINT32)((bytes)-1)))


