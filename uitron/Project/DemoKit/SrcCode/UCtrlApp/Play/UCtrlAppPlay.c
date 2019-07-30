#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "UIInfo.h"
#include "UIAppPhoto.h"
#include "UIControl.h"
#include "UIControlWnd.h"
#include "uToken.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "GxVideoFile.h"
#include "Exif.h"
#include "ExifDef.h"
#include "UCtrlAppPlay.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlPlay
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
#include "DebugModule.h"


#define SXCMD_TEST          0

#define MOUNT_CARD_ROOT     "/mnt/sd"

#define TEMP_PATH_SIZE      128
static FST_FILE          m_Filehdl = NULL;

static INT32 Linux2NvtPath(const char *inPath, char *outPath)
{
	if (strncmp(MOUNT_CARD_ROOT, inPath, strlen(MOUNT_CARD_ROOT))) {
		DBG_ERR("Only support files under %s\r\n", MOUNT_CARD_ROOT);
		return -1;
	}
	//outPath+=sprintf(outPath,"A:");
	outPath += snprintf(outPath, TEMP_PATH_SIZE, "A:");
	inPath += strlen(MOUNT_CARD_ROOT);
	while (*inPath != 0) {
		if (*inPath == '/') {
			*outPath = '\\';
		} else {
			*outPath = *inPath;
		}
		inPath++;
		outPath++;
	}
	//*outPath++ = '\\'; //If adding this, it will be regarded as folder.
	*outPath = 0;
	return 0;
}
static ER VdoFileReadCB(UINT32 pos, UINT32 size, UINT32 addr)
{
	ER result = E_SYS;

	DBG_IND("XML_VdoReadCB  pos=0x%x, size=%d, addr=0x%x\r\n", pos, size, addr);
	if (m_Filehdl) {
		FileSys_SeekFile(m_Filehdl, pos, FST_SEEK_SET);
		//not close file,close file in XML_GetThumbnail()
		result = FileSys_ReadFile(m_Filehdl, (UINT8 *)addr, &size, 0, 0);
	}
	return result;
}
static ER xGetMediaInfo(char *path, BOOL bThumbnail)
{
	FST_FILE          filehdl;
	char              tempPath[TEMP_PATH_SIZE];
	UINT32            TempLen, TempBuf;
	char             *pch;
	UINT32            result = 0;
	UINT32            MediaOffset = 0, MediaSize = 0;

	if (Linux2NvtPath(path, tempPath)) {
		return E_PAR;
	}

	pch = strchr(tempPath, '.');

	if (!pch) {
		DBG_ERR("Path Error!\r\n");
		return E_PAR;
	}

	if ((strncmp(pch + 1, "mov", 3) == 0) || (strncmp(pch + 1, "MOV", 3) == 0)) {
		FST_FILE_STATUS FileStat = {0};
		GXVIDEO_INFO VideoInfo;
		MEM_RANGE WorkBuf;
		UINT32 uiBufferNeeded = 0;

		m_Filehdl = FileSys_OpenFile(tempPath, FST_OPEN_READ);
		if (m_Filehdl) {
			FileSys_StatFile(m_Filehdl, &FileStat);
			GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
			TempBuf = SxCmd_GetTempMem(uiBufferNeeded);
			DBG_MSG("TempBuf=0x%X, uiBufferNeeded=%d\r\n", TempBuf, uiBufferNeeded);
			if (!TempBuf) {
				DBG_ERR("No Buffer!\r\n");
				FileSys_CloseFile(m_Filehdl);
				return E_NOMEM;
			}
			//parse video info for single mode only
			WorkBuf.Addr = TempBuf;
			WorkBuf.Size = uiBufferNeeded;
			result = GxVidFile_ParseVideoInfo(VdoFileReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
			FileSys_CloseFile(m_Filehdl);
			if ((result == GXVIDEO_PRSERR_OK) && (VideoInfo.uiThumbSize)) {
				if (bThumbnail) {
					MediaOffset = VideoInfo.uiThumbOffset;
					MediaSize = VideoInfo.uiThumbSize;
				} else {
					MediaOffset = VideoInfo.uiScraOffset;
					MediaSize = VideoInfo.uiScraSize;
				}

			} else {
				DBG_ERR("thumb error\r\n");
			    goto GetMediaInfo_Err;
			}
		} else {

			DBG_ERR("no %s\r\n", tempPath);
			goto GetMediaInfo_Err;
		}

	} else {

		filehdl = FileSys_OpenFile(tempPath, FST_OPEN_READ);

		if (filehdl) {
			MEM_RANGE ExifData;
			EXIF_GETTAG exifTag;
			BOOL bIsLittleEndian = 0;
			UINT32 uiTiffOffsetBase = 0;

			TempLen = MAX_APP1_SIZE;
			TempBuf = SxCmd_GetTempMem(TempLen);
			DBG_MSG("TempBuf=0x%X, NeedSize=%d\r\n", TempBuf, TempLen);
			if (!TempBuf) {
				DBG_ERR("No Buffer!\r\n");
				FileSys_CloseFile(filehdl);
				return E_NOMEM;
			}
			FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0, 0);
			FileSys_CloseFile(filehdl);

			ExifData.Size = MAX_APP1_SIZE;
			ExifData.Addr = TempBuf;
			if (EXIF_ER_OK == EXIF_ParseExif(EXIF_HDL_ID_1, &ExifData)) {
				if (bThumbnail) {
					EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
					EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_TIFF_OFFSET_BASE, &uiTiffOffsetBase, sizeof(uiTiffOffsetBase));
					//find thumbnail
					exifTag.uiTagIfd = EXIF_IFD_1ST;
					exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT;
					if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
						MediaOffset = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian) + uiTiffOffsetBase;
						exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT_LENGTH;
						if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
							MediaSize = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
						}
					}
				} else {
					MAKERNOTE_INFO MakernoteInfo = {0};
					//find screennail
					EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_MAKERNOTE, &MakernoteInfo, sizeof(MakernoteInfo));
					MediaOffset = MakernoteInfo.uiScreennailOffset;
					MediaSize = MakernoteInfo.uiScreennailSize;
				}
			} else {
				DBG_ERR("exif error\r\n");
			    goto GetMediaInfo_Err;
			}
		} else {
			DBG_ERR("no %s\r\n", tempPath);
			goto GetMediaInfo_Err;
		}
	}

	//sprintf(tempPath, "Offset=%d,Size=%d", MediaOffset,MediaSize);
	snprintf(tempPath, sizeof(tempPath), "Offset=%d,Size=%d", MediaOffset, MediaSize);
	NvtUctrl_SetRetString(tempPath);
	DBG_DUMP("RetString: %s\r\n", tempPath);
    SxCmd_RelTempMem(TempBuf);
	return E_OK;


GetMediaInfo_Err:
    SxCmd_RelTempMem(TempBuf);
	return E_SYS;


}


static INT32 param_getthumbnail(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("path: %s\r\n", argv[0]);
	return xGetMediaInfo(argv[0], TRUE);
}
static INT32 param_getscreennail(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("path: %s\r\n", argv[0]);
	return xGetMediaInfo(argv[0], FALSE);
}

static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	return 0;
}

static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[64];
	DBG_IND("\r\n");
	if (p_info->err_code < 0) {
		//sprintf(retStr, "ERR: parm %s", p_info->failed_name);
		snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}
	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_play_param[] = {
	{"getthumbnail", param_getthumbnail},
	{"getscreennail", param_getscreennail},
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_play = {
	"uplay",
	tbl_play_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST
static BOOL Cmd_uplay_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_play);
	return TRUE;
}

SXCMD_BEGIN(uplay, "uplay")
SXCMD_ITEM("set %", Cmd_uplay_set, "")
SXCMD_END()
#endif

void UCtrlAppPlay_InstallCmd(void)
{
#if SXCMD_TEST
	SxCmd_AddTable(uplay);
#endif
}

