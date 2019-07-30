/**
    UDTA atom (User Data) maker/parser sample code
    Just sample code to show how to make/parse the user data in MOV file
    @file       MovieUdtaVendor.c
    @ingroup    Predefined_group_name
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
#include <string.h>
#include <stdio.h>
#include "SysKer.h"
#include "PrjCfg.h"
#include "ImageApp_Movie.h"
#include "MovieUdtaVendor.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          MovieUdtaVendor
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass

#include "DebugModule.h"



#define MOVIEUDTA_MAKE_VENDOR_USER_DATA                  ENABLE
#define MOVIEUDTA_PARSE_VENDOR_USER_DATA                 DISABLE

extern BOOL g_bTsFileFormat;

#if (MOVIEUDTA_MAKE_VENDOR_USER_DATA == ENABLE)
#include "MOVLib.h"
#include "AVFile_MakerTS.h"
#endif
#if (MOVIEUDTA_MAKE_VENDOR_USER_DATA == ENABLE)
static UINT8         gUserMakerModelDataBuf[128];
static unsigned char gAppMovieManuNameStr[] = MOVAPP_COMP_MANU_STRING;
static unsigned char gAppMovieCompNameStr[] = MOVAPP_COMP_NAME_STRING;
#endif //MOVIEUDTA_MAKE_VENDOR_USER_DATA


#if (MOVIEUDTA_PARSE_VENDOR_USER_DATA == ENABLE)
#define NVT_ATOM_MAX_MAKER_SIZE   32
#define NVT_ATOM_MAX_MODEL_SIZE   32
#define TEXT_STR_TAGID_fmt        0xA9666D74
#define TEXT_STR_TAGID_inf        0xA9696E66


static UINT32 MovieUdta_ReadB32Bits(UINT8 *pb)//big endian
{
	UINT32 value = 0;
	value = (*(pb)) << 24;
	value += (*(pb + 1)) << 16;
	value += (*(pb + 2)) << 8;
	value += (*(pb + 3));
	return value;
}

static UINT32 MovieUdta_ReadB16Bits(UINT8 *pb)//big endian
{
	UINT32 value = 0;
	value = (*(pb)) << 8;
	value += (*(pb + 1));
	return value;
}
#endif // MOVIEUDTA_PARSE_VENDOR_USER_DATA


#if (MOVIEUDTA_MAKE_VENDOR_USER_DATA == ENABLE)
#define MOVIE_MAX_USER_MAKER_SIZE        32
#define MOVIE_MAX_USER_MODEL_SIZE        32
#endif
//
// Maker User Data in Movie File
//
BOOL MovieUdta_MakeVendorUserData(UINT32 *UDtaAddr, UINT32 *UDtaSize)
{
	BOOL bMakeOK = FALSE;
#if (MOVIEUDTA_MAKE_VENDOR_USER_DATA == ENABLE)
	UINT32             buf;
	MOV_USER_MAKERINFO makeinfo;

	buf = (UINT32) gUserMakerModelDataBuf;
	makeinfo.ouputAddr = buf;
	makeinfo.makerLen = sizeof(gAppMovieManuNameStr);
	makeinfo.pMaker = gAppMovieManuNameStr;
	makeinfo.modelLen =  sizeof(gAppMovieCompNameStr);
	makeinfo.pModel = gAppMovieCompNameStr;

	if ((makeinfo.makerLen > MOVIE_MAX_USER_MAKER_SIZE) || (makeinfo.modelLen > MOVIE_MAX_USER_MODEL_SIZE)) {
		DBG_ERR("The Maker/Model name is exceed than maximum! maker=%d max=%d, model=%d max=%d\r\n",
				makeinfo.makerLen, MOVIE_MAX_USER_MAKER_SIZE,
				makeinfo.modelLen, MOVIE_MAX_USER_MODEL_SIZE);

		return FALSE;
	}

	*UDtaAddr = (UINT32) gUserMakerModelDataBuf;
	// Make data
	if (g_bTsFileFormat == TRUE) {
		TSWriteLib_UserMakerModelData(&makeinfo);
	} else {
		*UDtaSize = MOVWrite_UserMakerModelData(&makeinfo);
	}
	bMakeOK = TRUE;

#endif //MOVIEUDTA_MAKE_VENDOR_USER_DATA

	return bMakeOK;
}

//
// Parse User Data in Movie File
//
BOOL MovieUdta_ParseVendorUserData(UINT32 UDtaAddr, UINT32 UDtaSize)
{
#if (MOVIEUDTA_PARSE_VENDOR_USER_DATA == ENABLE)
	unsigned char sUdtaMakerName[] = MOVAPP_COMP_MANU_STRING;
	unsigned char sUdtaModelName[] = MOVAPP_COMP_NAME_STRING;
	UINT8         u8MakerNameData[NVT_ATOM_MAX_MAKER_SIZE];
	UINT8         u8ModelNameData[NVT_ATOM_MAX_MODEL_SIZE];
	UINT32        i;
	UINT32        uiSize, uiData;
	UINT32        uiMakerSize, uiModelSize;
	UINT8         *pb8;

	if (!UDtaSize) {
		DBG_ERR("No user data\r\n");
		return FALSE;
	}

	pb8 = (UINT8 *)UDtaAddr;
	uiSize = MovieUdta_ReadB32Bits(pb8); // Get size
	pb8 += 4;
	uiData = MovieUdta_ReadB32Bits(pb8); // Get '@fmt' tag
	pb8 += 4;

	if (uiData == TEXT_STR_TAGID_fmt) {
		// Get Maker name
		uiMakerSize = MovieUdta_ReadB16Bits(pb8);
		pb8 += 4;
		for (i = 0; i < uiMakerSize; i++) {
			if (i > NVT_ATOM_MAX_MAKER_SIZE) {
				break;
			}

			u8MakerNameData[i] = *pb8;
			pb8++;
		}
	} else {
		DBG_ERR("No Maker name\r\n");
		return FALSE;
	}

	uiSize = MovieUdta_ReadB32Bits(pb8); // Get size
	pb8 += 4;
	uiData = MovieUdta_ReadB32Bits(pb8); // Get '@inf' tag
	pb8 += 4;

	if (uiData == TEXT_STR_TAGID_inf) {
		// Get Model name
		uiModelSize = MovieUdta_ReadB16Bits(pb8);
		pb8 += 4;
		for (i = 0; i < uiModelSize; i++) {
			if (i > NVT_ATOM_MAX_MODEL_SIZE) {
				break;
			}

			u8ModelNameData[i] = *pb8;
			pb8++;
		}
	} else {
		DBG_ERR("No Model name\r\n");
		return FALSE;
	}

	//DBG_ERR("u8MakerNameData =  %s \r\n", u8MakerNameData);
	//DBG_ERR("sUdtaMakerName =  %s \r\n", sUdtaMakerName);
	//DBG_ERR("u8ModelNameData =  %s \r\n", u8ModelNameData);
	//DBG_ERR("sUdtaModelName =  %s \r\n", sUdtaModelName);

	// Check Maker name and Model name
	if ((!memcmp(u8MakerNameData, sUdtaMakerName, uiMakerSize)) &&
		(!memcmp(u8ModelNameData, sUdtaModelName, uiModelSize))) {
		return TRUE;
	} else {
		return FALSE;
	}

#else

	return TRUE;

#endif // MOVIEUDTA_PARSE_VENDOR_USER_DATA
}

