////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "UICommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIResource.h"
#include "UIInfo.h"
//#include "Demo_Logo.h"

#include "PStore.h"
//#include "PSDrv.h"
#include "lz.h"
#include "UIAppPhoto.h"

#include "UIFlow.h"

//#NT#2010/10/15#Niven Cho -begin
//#NT#CMP_LANG
//#define CFG_COMPRESSED_LANG ENABLE
#define CFG_COMPRESSED_LANG DISABLE
//#NT#2010/10/15#Niven Cho -end

#define UIRes_Debug     debug_ind
//#NT#2010/10/15#Niven Cho -begin
//#NT#CMP_LANG
#if(CFG_COMPRESSED_LANG==ENABLE)
#define DECOMP_LANG(x) UIRes_LangDecompress(x,x##_size)
#else
#define DECOMP_LANG(x) (UINT32)&x
#endif
//#NT#2010/10/15#Niven Cho -end

///////////////////////////////////////////////////////////////////////////////
//
//  Palette


///////////////////////////////////////////////////////////////////////////////
//
//  Font


///////////////////////////////////////////////////////////////////////////////
//
//  Wallpaper (JPG)

#define PS_WALLPAPER_IMAGE       "WALL"
#define PS_LOGO_IMAGE            "LOGO"
#define PS_PHOTOFRAME_IMAGE      "PHFE"

char    uiUpdPhotoframeImg[]   = "A:\\Demo_PhotoFrameJPG.bin";
UINT32          p_gPhotoFrameJPG = 0;
//#NT#2010/10/15#Niven Cho -begin
//#NT#CMP_LANG
static STRING_TABLE g_LangCurrTbl = 0;
//#NT#2010/10/15#Niven Cho -end

//#JPG using Pstore
UINT32   p_gMenuJPG = 0;
UINT32   p_gMenuSize = 0;
UINT32   p_gLogoJPG = 0;
UINT32   p_gLogoSize = 0;
IMAGE   gMenuImage;
IMAGE   gLogoImage;
UINT32   p_gDefLogoJPG = 0;
IMAGE   gDefLogoImage;
//MOVIE_SIZE_TAG
char *gUserStr[STRID_USER_END - STRID_USER_START] = {
	"MCTF",
	"Edge",
	"NR",
	"WiFi/ETH",
	"6400",
	"12800",
	"UHD P50",
	"UHD P30",
	"UHD P24",
	"2.7K P60",
	"QHD P80",
	"QHD P60",
	"QHD P30",
	"3MHD P30",
	"FHD P120",
	"FHD P96",
	"FHD P60",
	"FHD P30",
	"HD P240",
	"HD P120",
	"HD P60",
	"HD P30",
	"WVGA P30",
	"VGA P240",
	"VGA P30",
	"QVGA P30",
	"FHD P30+FHD P30",
	"FHD P30 x 3",
	"FHD P30 x 4",
	"FHD P30+FHD P30",
	"FHD P30+HD P30",
	"QHD P30+WVGA P30",
	"3MHD P30+WVGA P30",
	"FHD P60+WVGA P30",
	"FHD P60+VGA P30", //640x360
	"FHD P30+WVGA P30",
	"2048x2048 P30 + 480x480 P30",
	"Both2",
	"SideBySide",
	"Burst 30",
	"5M 2992x1696",
	"Codec",
	"MJPG",
	"H264",
	"H265"
};
char *UIRes_GetUserString(UINT32 TxtId)
{
	return gUserStr[TxtId - STRID_USER_START];
}

//#NT#2010/10/15#Niven Cho -begin
//#NT#CMP_LANG
#if(CFG_COMPRESSED_LANG==ENABLE)
static UINT32 UIRes_LangDecompress(const unsigned char *pLang, const unsigned long size)
{
	UINT32 uiPoolAddr;
	unsigned int length = 0;

	uiPoolAddr = OS_GetMempoolAddr(POOL_ID_CACHE);
	uiPoolAddr += POOL_CACHE_OFFSET_LANGUAGE;
	length = LZ_Uncompress((unsigned char *)pLang, (unsigned char *)uiPoolAddr, (unsigned int)size);

	if (length > POOL_SIZE_CACHE_LANGUAGE) {
		debug_msg("^Rlang cache size is not enough!\r\n");
	}
	return uiPoolAddr;
}
#endif
//#NT#2010/10/15#Niven Cho -end


///////////////////////////////////////////////////////////////////////////////
//
//  Image (JPG)

//#JPG using Pstore
#if (_JPG_STORE_ == _PSTORE_)
#define PS_JPG_IMAGE            "JPGI"
#endif

//#JPG using Pstore
#if (_JPG_STORE_ == _PSTORE_)
static UINT8    uiUpdateJpgImg[]   = "A:\\Demo_PFJPG.bin";
#endif

//#JPG using Pstore
UINT32   p_gImageJPG = 0;

UINT32 Get_IMGTable(UINT32 uhIndex)
{
	//#JPG using Pstore
#if (_JPG_STORE_ == _PSTORE_)
	return p_gImageJPG;
#else
	return (UINT32)(void *)gDemoKit_Image;
#endif
}


//#JPG using Pstore
BOOL UIRes_ChkWriteIMG(void)
{
#if (_JPG_STORE_ == _PSTORE_)
	FST_FILE filehdl = NULL;
	INT32 ret = FST_STA_ERROR;
	UINT32    filesize = POOL_SIZE_GFX_IMAGE;
	UINT32   *pBinSize;
	UINT32    uiBuffer;
	PPSTORE_SECTION_HANDLE  pSection;

	uiBuffer = OS_GetMempoolAddr(POOL_ID_GFX_IMAGE);
	UIRes_Debug(("\n[IMG] Pool size read %d \r\n", OS_GetMempoolSize(POOL_ID_GFX_IMAGE)));
	// Update English Language begin

	filesize = POOL_SIZE_GFX_IMAGE - 4;
	filehdl = FileSys_OpenFile(uiUpdateJpgImg, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
	// Param filesize must > acture filesize
	ret = FileSys_WriteFile(filehdl, (UINT8 *)(uiBuffer + 4), &filesize, 0, 0);
	FileSys_CloseFile(filehdl);
	if (filesize != 0) {
		UIRes_Debug(("\n[IMG] PStore read IMG size = %d\r\n", filesize));
		if (filesize == POOL_SIZE_GFX_IMAGE - 4) {
			debug_err(("^R[IMG] Need check if POOL_SIZE_GFX_IMAGE enough\r\n"));
			return FALSE;
		} else if ((pSection = PStore_OpenSection(PS_JPG_IMAGE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
			pBinSize = (UINT32 *)uiBuffer;
			*pBinSize = filesize;
			PStore_WriteSection((UINT8 *)uiBuffer, 0, (*pBinSize) + 4, pSection);
			PStore_CloseSection(pSection);
			UIRes_Debug(("\n[IMG] PStore Write IMG success size = %d\r\n", *pBinSize));
			return TRUE;
		}
	}
	return FALSE;
#else
	return FALSE;
#endif
}

void UIRes_InitReadIMG(void)
{
#if (_JPG_STORE_ == _PSTORE_)
	UINT32    BufSize;
	UINT32    uiBuffer;
	PPSTORE_SECTION_HANDLE  pSection;

	uiBuffer = OS_GetMempoolAddr(POOL_ID_GFX_IMAGE);

	//load jpg only once
	if (!p_gImageJPG) {
		p_gImageJPG = uiBuffer;

		// Read system info from PStore
		if ((pSection = PStore_OpenSection(PS_JPG_IMAGE, PS_RDONLY)) != E_PS_SECHDLER) {
			PStore_ReadSection((UINT8 *)&BufSize, 0, 4, pSection);
			PStore_ReadSection((UINT8 *)uiBuffer, 4, BufSize, pSection);
			PStore_CloseSection(pSection);
			UIRes_Debug(("\n[IMG] PStore read IMG size = %d\r\n", BufSize));

		} else {
			debug_err(("[IMG] PStore Read IMG fail\r\n"));
		}
	}
#endif
}

void UIRes_DumpMemIMG(void)
{
#if (_JPG_STORE_ == _PSTORE_)
	FST_FILE filehdl = NULL;
	INT32 ret = FST_STA_ERROR;
	UINT32    BufSize;
	UINT32    uiBuffer;
	PPSTORE_SECTION_HANDLE  pSection;

	uiBuffer = OS_GetMempoolAddr(POOL_ID_GFX_IMAGE);

	// Read system info from PStore
	if ((pSection = PStore_OpenSection(PS_JPG_IMAGE, PS_RDONLY)) != E_PS_SECHDLER) {
		PStore_ReadSection((UINT8 *)&BufSize, 0, 4, pSection);
		PStore_CloseSection(pSection);
		UIRes_Debug(("\n[MG] PStore read IMG size = %d\r\n", BufSize));

	} else {
		debug_err(("[IMG] PStore read IMG fail\r\n"));
	}

	UIRes_Debug(("BufSize=%d", BufSize));
	filehdl = FileSys_OpenFile("A:\\dump.bin", FST_CREATE_ALWAYS | FST_OPEN_WRITE);
	ret = FileSys_WriteFile(filehdl, (UINT8 *)uiBuffer, &BufSize, 0, 0);
	FileSys_CloseFile(filehdl);
	UIRes_Debug(("[IMG] dump ok\r\n"));
#endif
}

void UIRes_DeleteIMG(void)
{
#if (_JPG_STORE_ == _PSTORE_)
	PStore_DeleteSection(PS_JPG_IMAGE);
	UIRes_Debug(("[IMG] delete ok\r\n"));
#endif
}

// Photo frame
BOOL UIRes_ChkWritePHFE(void)
{
	FST_FILE  filehdl;
	INT32     ret;
	UINT32    filesize = POOL_SIZE_APP;
	UINT32   *pBinSize;
	UINT32    uiBuffer;
	PPSTORE_SECTION_HANDLE  pSection;

	uiBuffer = OS_GetMempoolAddr(POOL_ID_APP);
	UIRes_Debug(("PHFE Pool size read %d \r\n", OS_GetMempoolSize(POOL_ID_APP)));

	filesize = POOL_SIZE_APP - 4;
	// Param filesize must > acture filesize
	filehdl = FileSys_OpenFile(uiUpdPhotoframeImg, FST_OPEN_READ);
	ret = FileSys_ReadFile(filehdl, (UINT8 *)(uiBuffer + 4), &filesize, 0, 0);
	FileSys_CloseFile(filehdl);
	if ((ret == FST_STA_OK) && (filesize != 0)) {
		UIRes_Debug(("Read PHFE size = %d\r\n", filesize));
		if (filesize == POOL_SIZE_APP - 4) {
			debug_err(("^RPHFE Need check if POOL_ID_APP enough\r\n"));
			return FALSE;
		} else if ((pSection = PStore_OpenSection(PS_PHOTOFRAME_IMAGE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
			pBinSize = (UINT32 *)uiBuffer;
			*pBinSize = filesize + 4;
			PStore_WriteSection((UINT8 *)uiBuffer, 0, (*pBinSize) + 4, pSection);
			PStore_CloseSection(pSection);
			UIRes_Debug(("Write PHFE success size = %d\r\n", *pBinSize));
			return TRUE;
		} else {
			debug_err(("Create PF sec Fail %d\r\n", pSection));
			return FALSE;
		}
	} else {
		debug_err(("PF Write Fail %d,%d\r\n", ret, filesize));
		return FALSE;
	}
}

void UIRes_InitReadPHFE(void)
{
	PPSTORE_SECTION_HANDLE  pSection;
	UINT16   tmp = 0;
	ER 	     result = 0;
	//load jpg only once
	if ((pSection = PStore_OpenSection(PS_PHOTOFRAME_IMAGE, PS_RDONLY)) != E_PS_SECHDLER) {
		result = PStore_ReadSection((UINT8 *)&gPFInfo, 0, sizeof(gPFInfo), pSection);
		if(result == E_PS_OK) {
			result = PStore_ReadSection((UINT8 *)&tmp, gPFInfo.TotalLen - 2, 2, pSection);
		} else {
			debug_err(("PStore_ReadSection(%s) fail = %d\r\n",PS_PHOTOFRAME_IMAGE,result));
		}
		if (result == E_PS_OK && gPFInfo.Ver == PF_PSTORE_VER &&  tmp == 0xD9FF) {
			UI_SetData(FL_PhotoFrameWriteSts, TRUE);
		}
		//debug_msg("PF ver=0x%08x, FileEnd=0x%x, Sts=%d\r\n",gPFInfo.Ver,tmp,UI_GetData(UI_PhotoFrameWriteSts));
		PStore_CloseSection(pSection);
	}
}

void UIRes_DeletePHFE(void)
{
	ER result = PStore_DeleteSection(PS_PHOTOFRAME_IMAGE);
	if(result!=E_PS_OK)
		UIRes_Debug(("[PF] delete fail\r\n"));
}

//#NT#2011/02/14#Steven feng -begin

//Define the number of Slideshow WAV files
//#define SSWAV_NUM 4

//Define the Pstroe section name
#define PS_SSWAV_SIZ        "WSIZ"
#define PS_WAV1             "WAVA"
#define PS_WAV2             "WAVB"
#define PS_WAV3             "WAVC"
#define PS_WAV4             "WAVD"

//int current_SSWAV_id = 0;

//Define the file name for updateing WAV files
static char     uiUpdateWAV1[]   = "A:\\BGM1.wav";
static char     uiUpdateWAV2[]   = "A:\\BGM2.wav";
static char     uiUpdateWAV3[]   = "A:\\BGM3.wav";
static char     uiUpdateWAV4[]   = "A:\\BGM5.wav";

//Slide show WAV files Info.
SSWAV_Info g_SSWAVInfo[SSWAV_NUM] = {
	{PS_WAV1,       uiUpdateWAV1},
	{PS_WAV2,       uiUpdateWAV2},
	{PS_WAV3,       uiUpdateWAV3},
	{PS_WAV4,       uiUpdateWAV4},
};

//Define the file size of each Slideshow WAV file
UINT32 g_pSSWAVSize_[SSWAV_NUM];

//Read Buffer
UINT32 uiWAVBuffer;


//SlideShow WAV
BOOL UIRes_ChkWriteSSWAV(void)
{
	FST_FILE filehdl = NULL;
	UINT32    filesize = POOL_SIZE_APP;
	UINT32    uiWAVBuffer;
	INT32           cnt;
	PPSTORE_SECTION_HANDLE  pSection;
	BOOL      bIsUpdate = FALSE;

	uiWAVBuffer = OS_GetMempoolAddr(POOL_ID_APP);
	debug_err(("\n[SSWAV] Pool size read %d \r\n", OS_GetMempoolSize(POOL_ID_APP)));

	//#NT#2011/02/16#Steven feng -begin
	//Set Default Status is FALSE
	UI_SetData(FL_SSWAVWriteSts, FALSE);
	//#NT#2011/02/16#Steven feng -end

	for (cnt = 0; cnt < SSWAV_NUM; cnt ++) {
		debug_err(("\n[SSWAV] PStore read file = %s\r\n", g_SSWAVInfo[cnt].ui_pFilePathName));

		filesize = POOL_SIZE_APP;
		// Param filesize must > acture filesize
		filehdl = FileSys_OpenFile(g_SSWAVInfo[cnt].ui_pFilePathName, FST_OPEN_READ);
		FileSys_ReadFile(filehdl, (UINT8 *)uiWAVBuffer, &filesize, 0, 0);
		FileSys_CloseFile(filehdl);
		if (filesize != 0) {
			bIsUpdate = TRUE;
			debug_err(("\n[SSWAV] PStore read %d WAV size = %d\r\n", cnt, filesize));
			g_pSSWAVSize_[cnt] = filesize;
			if ((pSection = PStore_OpenSection(g_SSWAVInfo[cnt].uiPS_SectionName, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
				PStore_WriteSection((UINT8 *)uiWAVBuffer, 0, filesize, pSection);
				PStore_CloseSection(pSection);
				debug_err(("\n[SSWAV] PStore write section %d WAV success size = %d\r\n", cnt, filesize));
			}

			//current_SSWAV_id = cnt;
		}
	}

	if (bIsUpdate == TRUE) {
		if ((pSection = PStore_OpenSection(PS_SSWAV_SIZ, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
			PStore_WriteSection((UINT8 *)&g_pSSWAVSize_, 0, sizeof(g_pSSWAVSize_), pSection);
			PStore_CloseSection(pSection);
		}
		return TRUE;
	}
	return FALSE;

}

//#NT#2011/02/16#Lily Kao -begin
//return: size of the wav file
UINT32 UIRes_ReadSSWAV(int SSWAV_id, UINT32 uiPoolAddr, UINT32 uiSize)
{
	INT32 retV = 0;
	PPSTORE_SECTION_HANDLE  pSection;

	if ((pSection = PStore_OpenSection(PS_SSWAV_SIZ, PS_RDWR)) != E_PS_SECHDLER) {
		retV = PStore_ReadSection((UINT8 *)g_pSSWAVSize_, 0, sizeof(g_pSSWAVSize_), pSection);
		if (E_PS_OK != retV) {
			debug_err(("Read WavSize-PStore Fail:%d\r\n", retV));
			return 0;
		}
		UIRes_Debug(("W1S=0x%x,W2S=0x%x,W3S=0x%x,W4S=0x%x\r\n", g_pSSWAVSize_[0], g_pSSWAVSize_[1], g_pSSWAVSize_[2], g_pSSWAVSize_[3]));
		retV = PStore_CloseSection(pSection);
		if (E_PS_OK != retV) {
			debug_err(("Close WavSize-PStore Fail:%d\r\n", retV));
			return 0;
		}
	} else {
		debug_err(("Open WavSize-PStore Fail\r\n"));
		return 0;
	}
	UIRes_Debug(("PStoreR WAV[%d] size = 0x%x\r\n", SSWAV_id, g_pSSWAVSize_[SSWAV_id]));
	if ((uiSize & 0xFFFFFFC0) < g_pSSWAVSize_[SSWAV_id]) {
		debug_err(("PStoreR Space=0x%x,0x%x < 0x%x\r\n", uiSize, (uiSize & 0xFFFFFFC0), g_pSSWAVSize_[SSWAV_id]));
		return 0;
	}
	if ((pSection = PStore_OpenSection(g_SSWAVInfo[SSWAV_id].uiPS_SectionName, PS_RDWR)) != E_PS_SECHDLER) {
		retV = PStore_ReadSection((UINT8 *)uiPoolAddr, 0, g_pSSWAVSize_[SSWAV_id], pSection);
		if (E_PS_OK != retV) {
			debug_err(("Read Wav[%d]-PStore Fail:%d\r\n", SSWAV_id, retV));
			return 0;
		}
		retV = PStore_CloseSection(pSection);
		if (E_PS_OK != retV) {
			debug_err(("Close Wav[%d]-PStore Fail:%d\r\n", SSWAV_id, retV));
			return 0;
		}
	} else {
		debug_err(("Open Wav[%d]-PStore Fail\r\n", SSWAV_id));
		return 0;
	}
	return g_pSSWAVSize_[SSWAV_id];
}
//#NT#2011/02/16#Lily Kao -end

void UIRes_InitReadSSWAV(int SSWAV_id)
{
	PPSTORE_SECTION_HANDLE  pSection;
	ER result = 0;
	uiWAVBuffer = OS_GetMempoolAddr(POOL_ID_APP);

	if ((pSection = PStore_OpenSection(PS_SSWAV_SIZ, PS_RDWR)) != E_PS_SECHDLER) {
		result = PStore_ReadSection((UINT8 *)g_pSSWAVSize_, 0, sizeof(g_pSSWAVSize_), pSection);
		if(result !=E_PS_OK)
			debug_err(("[SSWAV] PStore read fail %d\r\n",result));

		PStore_CloseSection(pSection);
	} else {
		debug_err(("^R[SSWAV] PStore read WAV size fail!!\r\n"));
	}

	debug_err(("[SSWAV] PStore prepare read WAV size = %d  WAV_id %d\r\n", g_pSSWAVSize_[SSWAV_id], SSWAV_id));
	if ((pSection = PStore_OpenSection(g_SSWAVInfo[SSWAV_id].uiPS_SectionName, PS_RDWR)) != E_PS_SECHDLER) {
		//#NT#2011/02/16#Steven feng -begin
		if (SSWAV_id == SSWAV_NUM - 1) {
			UI_SetData(FL_SSWAVWriteSts, TRUE);
		} else {
			UI_SetData(FL_SSWAVWriteSts, FALSE);
		}
		//#NT#2011/02/16#Steven feng -end

		result = PStore_ReadSection((UINT8 *)uiWAVBuffer, 0, g_pSSWAVSize_[SSWAV_id], pSection);
		if(result !=E_PS_OK)
			debug_err(("[SSWAV] PStore read fail %d\r\n",result));

		PStore_CloseSection(pSection);
		//current_SSWAV_id = SSWAV_id;
	} else {
		debug_err(("^R[SSWAV] PStore read WAV data fail!!\r\n"));
	}
}

//#NT#2011/02/14#Steven feng -end



///////////////////////////////////////////////////////////////////////////////
//
//  String (LANG)

//!!!POOL_ID_LANG need to sync current_lang_id for load language correct
int current_lang_id = 0; //default = ENG

#if (_LANG_STORE_ == _PSTORE_)
#define PS_LANG_SIZ             "LSIZ"
#define PS_LANG_ENG             "LENG"
#endif


#if(_LANG_STORE_ == _PSTORE_)

static UINT8    uiUpdateLangEng[]   = "A:\\Demo_String_ENGLISH.bin";

Language_Info g_LanguageInfo[LANGUAGE_SETTING_MAX] = {
	{PS_LANG_ENG,       uiUpdateLangEng},   //ENGLISH
};

UINT32 g_pLanguageSize_[LANGUAGE_SETTING_MAX];
#endif

#if(_LANG_STORE_ == _INSIDECODE_)
//#NT#2010/10/15#Niven Cho -begin
//#NT#CMP_LANG
UINT32 Get_LanguageValue(UINT32 uhIndex)
{

	switch (uhIndex) {
	case LANG_EN:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_EN;
		break;
	case LANG_DE:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_DE;
		break;
	case LANG_FR:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_FR;
		break;
	case LANG_ES:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_ES;
		break;
	case LANG_IT:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_IT;
		break;
	case LANG_PO:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_PO;
		break;
	case LANG_TC:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_TC;
		break;
	case LANG_SC:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_SC;
		break;
	case LANG_RU:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_RU;
		break;
	case LANG_JP:
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_JP;
		break;
	}
	return g_LangCurrTbl;
}
//#NT#2010/10/15#Niven Cho -end
#endif


//#Multi lang using Pstore
BOOL UIRes_ChkWriteLang(void)
{
#if(_LANG_STORE_ == _PSTORE_)
	FST_FILE filehdl = NULL;
	INT32 ret = FST_STA_ERROR;
	UINT32    filesize = POOL_SIZE_LANG;
	UINT32    uiLangBuffer;
	INT32       cnt;
	PPSTORE_SECTION_HANDLE  pSection;
	BOOL      bIsUpdate = FALSE;

	uiLangBuffer = OS_GetMempoolAddr(POOL_ID_LANG);
	UIRes_Debug(("\n[LANG] Pool size read %d \r\n", OS_GetMempoolSize(POOL_ID_LANG)));
	// Update English Language begin
	for (cnt = 0; cnt < LANGUAGE_SETTING_MAX; cnt ++) {
		UIRes_Debug(("\n[LANG] PStore read file = %s\r\n", g_LanguageInfo[cnt].ui_pFilePathName));

		filesize = POOL_SIZE_LANG;
		// Param filesize must > acture filesize
		filehdl = FileSys_OpenFile(g_LanguageInfo[cnt].ui_pFilePathName, FST_OPEN_READ);
		ret = FileSys_ReadFile(filehdl, (UINT8 *)uiLangBuffer, &filesize, 0, 0);
		FileSys_CloseFile(filehdl);
		if (filesize != 0) {
			bIsUpdate = TRUE;
			UIRes_Debug(("\n[LANG] PStore read %d lang size = %d\r\n", cnt, filesize));
			g_pLanguageSize_[cnt] = filesize;
			if ((pSection = PStore_OpenSection(g_LanguageInfo[cnt].uiPS_SectionName, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
				PStore_WriteSection((UINT8 *)uiLangBuffer, 0, filesize, pSection);
				PStore_CloseSection(pSection);
				UIRes_Debug(("\n[LANG] PStore write section %d lang success size = %d\r\n", cnt, filesize));
			}
			// POOL_ID_LANG need to sync current_lang_id for load language correct
			current_lang_id = cnt;
		}
	}
	// Update English Language end

	// Pstore has update language
	if (bIsUpdate == TRUE) {
		// Update Language size data structure
		if ((pSection = PStore_OpenSection(PS_LANG_SIZ, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER) {
			PStore_WriteSection((UINT8 *)&g_pLanguageSize_, 0, sizeof(g_pLanguageSize_), pSection);
			PStore_CloseSection(pSection);
		}
		return TRUE;
	}
	return FALSE;
#else
	return FALSE;
#endif
}


void UIRes_InitReadLang(int lang_id)
{
#if(_LANG_STORE_ == _PSTORE_)
	PPSTORE_SECTION_HANDLE  pSection;
	UINT32                  uiLangBuffer;
	uiLangBuffer = OS_GetMempoolAddr(POOL_ID_LANG);

	// Read specific language information size
	if ((pSection = PStore_OpenSection(PS_LANG_SIZ, PS_RDWR)) != E_PS_SECHDLER) {
		PStore_ReadSection((UINT8 *)g_pLanguageSize_, 0, sizeof(g_pLanguageSize_), pSection);
		PStore_CloseSection(pSection);
	}
	//#Multi lang using Pstore
	// Read specific language
	//debug_err(("[Init_MenuInfo][PStore prepare read language size = %d]\r\n", g_pLanguageSize_[Get_LanguageValue(lang_id)]));
	//if ((pSection = PStore_OpenSection(g_LanguageInfo[Get_LanguageValue(lang_id)].uiPS_SectionName, PS_RDWR)) != E_PS_SECHDLER)
	UIRes_Debug(("[LANG] PStore prepare read language size = %d\r\n", g_pLanguageSize_[lang_id]));
	if ((pSection = PStore_OpenSection(g_LanguageInfo[lang_id].uiPS_SectionName, PS_RDWR)) != E_PS_SECHDLER) {
		//PStore_ReadSection((UINT8 *)uiLangBuffer, 0, g_pLanguageSize_[Get_LanguageValue(lang_id)], pSection);
		PStore_ReadSection((UINT8 *)uiLangBuffer, 0, g_pLanguageSize_[lang_id], pSection);
		PStore_CloseSection(pSection);
		current_lang_id = lang_id;
	} else {
		memcpy((void *)uiLangBuffer, (void *)gDemo_String_ENG, POOL_SIZE_LANG);
		debug_err(("^R[LANG] PStore read lang data fail, set default lang\r\n"));
	}
#endif
}

UINT32 Get_LanguageTable(void)
{
//#NT#2010/10/15#Niven Cho -begin
//#NT#CMP_LANG
	if (g_LangCurrTbl == 0) {
		g_LangCurrTbl = (STRING_TABLE)&gDemoKit_String_EN;
	}
	return g_LangCurrTbl;
//#NT#2010/10/15#Niven Cho -end
}



//Load Language
void UIRes_SetLang(int lang_id)
{
//#NT#2009/08/12#Lincy Lin -begin
//#Multi lang using Pstore
#if(_LANG_STORE_ == _PSTORE_)
	UINT32                  uiLangBuffer;
	PPSTORE_SECTION_HANDLE  pSection;
	BOOL                    bFlag;
#endif

#if(_LANG_STORE_ == _INSIDECODE_)

	UI_SetData(FL_LANGUAGE, lang_id);
	UIRes_Debug(("[LANG] selected = %d\r\n", lang_id));
	GxGfx_SetStringTable((const STRING_TABLE *)Get_LanguageValue(lang_id));
#else
	//UINT32                  uiLangBuffer;
	//PPSTORE_SECTION_HANDLE  pSection;
	//BOOL                    bFlag;


	Set_LanguageIndex(lang_id);
	uiLangBuffer = OS_GetMempoolAddr(POOL_ID_LANG);
	bFlag = FALSE;

	if (current_lang_id != lang_id)
		//while(!bFlag)  //fix:if no pstore resource,would always while 1
	{
		//debug_err(("\n[SetupExe_OnLanguage][PStore read size = %d]\r\n", g_pLanguageSize_[Get_LanguageValue(lang_id)]));
		//if ((pSection = PStore_OpenSection(g_LanguageInfo[Get_LanguageValue(lang_id)].uiPS_SectionName, PS_RDWR)) != E_PS_SECHDLER)
		UIRes_Debug(("\n[LANG] PStore read size = %d\r\n", g_pLanguageSize_[lang_id]));
		if ((pSection = PStore_OpenSection(g_LanguageInfo[lang_id].uiPS_SectionName, PS_RDWR)) != E_PS_SECHDLER) {
			//PStore_ReadSection((UINT8 *)uiLangBuffer, 0, g_pLanguageSize_[Get_LanguageValue(lang_id)], pSection);
			PStore_ReadSection((UINT8 *)uiLangBuffer, 0, g_pLanguageSize_[lang_id], pSection);
			PStore_CloseSection(pSection);
			//debug_ind(("\n[SetupExe_OnLanguage][PStore read lang success size = %d]\r\n", g_pLanguageSize_[Get_LanguageValue(lang_id)]));
			UIRes_Debug(("\n[LANG] PStore read lang success size = %d\r\n", g_pLanguageSize_[lang_id]));
			current_lang_id = lang_id;
			bFlag = TRUE;
		} else {
			debug_err(("\n[LANG] PStore read lang data fail\r\n"));
			bFlag = FALSE;
		}
	}
	GxGfx_SetStringTable((const STRING_TABLE *)uiLangBuffer);
#endif
//#NT#2009/08/12#Lincy Lin -end
}

void UIRes_DumpMemLang(void)
{
#if(_LANG_STORE_ == _PSTORE_)
	FST_FILE filehdl = NULL;
	INT32 ret = FST_STA_ERROR;
	UINT32 lang_id = Get_LanguageIndex();
	UINT32    BufSize = g_pLanguageSize_[lang_id];
	UINT32    uiLangBuffer;

	uiLangBuffer = OS_GetMempoolAddr(POOL_ID_LANG);

	UIRes_Debug(("lang_id=%d", lang_id));
	UIRes_Debug(("BufSize=%d", BufSize));
	filehdl = FileSys_OpenFile("A:\\dump.bin", FST_CREATE_ALWAYS | FST_OPEN_WRITE);
	ret = FileSys_WriteFile(filehdl, (UINT8 *)uiLangBuffer, &BufSize, 0, 0);
	FileSys_CloseFile(filehdl);
	UIRes_Debug(("[LANG] dump ok\r\n"));
#endif
}

void UIRes_DeleteLang(void)
{
#if(_LANG_STORE_ == _PSTORE_)
	INT32       cnt;
	for (cnt = 0; cnt < LANGUAGE_SETTING_MAX; cnt ++) {
		PStore_DeleteSection(g_LanguageInfo[cnt].uiPS_SectionName);
	}
	UIRes_Debug(("[LANG] delete ok\r\n"));
#endif
}
