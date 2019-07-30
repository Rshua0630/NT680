#include "PrjCfg.h"
#include "FileSysTsk.h"
#include "FileDB.h"
#include "PBXFileList_FileDB.h"
#include "DCF.h"
#include "SysCommon.h"
#include "WifiAppCmd.h"
#include "UIAppMoviePlay.h"
#include "Exif.h"
#include "ExifDef.h"
#include "HwMem.h"
#include "UIInfo.h"
#include "GxVideoFile.h"
#include "WifiAppXML.h"
#include "ProjectInfo.h"
//#include "WiFiIpcAPI.h"
#include "UIAppNetwork.h"
#include "PlaybackTsk.h"
#include "UIFlow.h"
#include "SysCfg.h"
#include "WifiAppCmdMapping.h"
#include "nvtmpp.h"
#if(WIFI_AP_FUNC==ENABLE)
#include "nvt_auth.h"
#include "ImageApp_Photo.h"
#include "ImageApp_Movie.h"
#if (MOVIE_MAPPING_MULTIREC	== ENABLE)
#include "ImageApp_MovieMulti.h"
#endif
#if (FWS_FUNC == ENABLE)
#include "FwSrvApi.h"
#endif
#include "GxImageFile.h"
#include "media_def.h"


#define TRANS_JPG_FROM_H264IDR  1
#define CREATE_PHOTO_FILEDB     0
#define THIS_DBGLVL             2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          WifiAppXML
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define QUERY_XML_PATH  "A:\\query.xml"
#define QUERY_CMD_CUR_STS_XML_PATH  "A:\\query_cmd_cur_sts.xml"
#define CMD_STR "custom=1&cmd="
#define PAR_STR "&par="
#define QUERY_FMT  "{\"mfr\":\"%s\",\"type\":\"%d\",\"model\":\"%s\",\"p2p_uuid\":\"\"}"
/*mfr:廠商名稱
type:1-行車紀錄器 2- IP camera
model:產品型號
p2p_uuid:用戶p2p標誌,沒有可以不填 ""
*/

#define XML_PATH_LEN                 (128)
#define FAT_GET_DAY_FROM_DATE(x)     (x & 0x1F)              ///<  get day from date
#define FAT_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)       ///<  get month from date
#define FAT_GET_YEAR_FROM_DATE(x)    ((x >> 9) & 0x7F)+1980  ///<  get year from date
#define FAT_GET_SEC_FROM_TIME(x)     (x & 0x001F)<<1         ///<  seconds(2 seconds / unit)
#define FAT_GET_MIN_FROM_TIME(x)     (x & 0x07E0)>>5         ///<  Minutes
#define FAT_GET_HOUR_FROM_TIME(x)    (x & 0xF800)>>11        ///<  hours
#define XML_STRCPY(dst, src, dst_size)  do { strncpy(dst, src, dst_size-1); dst[dst_size-1] = '\0'; } while(0)


void XML_StringResult(UINT32 cmd, char *str, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType);
void XML_ValueResult(UINT32 cmd, UINT64 value, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType);


#define HW_TV_ENABLE                           0x00000001
#define HW_GSENDOR_ENABLE                      0x00000002
#define HW_WIFI_SOCKET_HANDSHAKE_ENABLE        0x00000004
#define HW_WIFI_CLIENT_SOCKET_NOTIFY_ENABLE    0x00000008
#define HW_SET_AUTO_RECORDING_DISABLE 	       0x00000010 //WIFIAPP_CMD_SET_AUTO_RECORDING (2012)


typedef enum {
	WIFI_AUTH_TYPE_WPA_TKIP = 0,
	WIFI_AUTH_TYPE_WPA_AES,
	WIFI_AUTH_TYPE_WPA_AES_TKIP,
	WIFI_AUTH_TYPE_WPA2_TKIP,
	WIFI_AUTH_TYPE_WPA2_AES,
	WIFI_AUTH_TYPE_WPA2_AES_TKIP,
	WIFI_AUTH_TYPE_NONE,
} WIFI_AUTH_TYPE;

#if CREATE_PHOTO_FILEDB
static FILEDB_INIT_OBJ gWifiFDBInitObj = {
	"A:\\",  //rootPath
	NULL,  //defaultfolder
	NULL,  //filterfolder
	TRUE,  //bIsRecursive
	TRUE,  //bIsCyclic
	TRUE,  //bIsMoveToLastFile
	TRUE, //bIsSupportLongName
	FALSE, //bIsDCFFileOnly
	TRUE,  //bIsFilterOutSameDCFNumFolder
	{'N', 'V', 'T', 'I', 'M'}, //OurDCFDirName[5]
	{'I', 'M', 'A', 'G'}, //OurDCFFileName[4]
	FALSE,  //bIsFilterOutSameDCFNumFile
	FALSE, //bIsChkHasFile
	60,    //u32MaxFilePathLen
	10000,  //u32MaxFileNum
	(FILEDB_FMT_JPG | FILEDB_FMT_AVI | FILEDB_FMT_MOV | FILEDB_FMT_MP4 | FILEDB_FMT_TS),
	0,     //u32MemAddr
	0,     //u32MemSize
	NULL   //fpChkAbort

};
#endif
static MEM_RANGE XMLTmpPool;
static NVTMPP_VB_BLK blk_xml =0;
INT32 XML_RelPrivateMem(void)
{
	NVTMPP_VB_POOL pool;

	if(!blk_xml)
	{
		return NVTMPP_ER_OK;
	}
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PLAYBACK) {
		DBG_WRN("not pb mode\r\n");
	}

    pool = nvtmpp_vb_addr2pool(nvtmpp_vb_block2addr(blk_xml));
	if (pool == NVTMPP_VB_INVALID_POOL){
		DBG_ERR("invalid block 0x%08X\r\n",blk_xml);
        return FALSE;
	} else {
		blk_xml =0;
		return nvtmpp_vb_destroy_pool(pool);
	}
}

UINT32 XML_AllocPrivateMem(UINT32 blk_size)
{
	NVTMPP_VB_POOL pool;
	//release previous,because always use the same pool
	XML_RelPrivateMem();

	//only in playback can get prvivate memory,and need release
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PLAYBACK) {
		DBG_ERR("not pb mode fail\r\n");
		return 0;
	}

	pool = nvtmpp_vb_create_pool("XML", blk_size, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == pool)
	{
		DBG_ERR("create private pool err\r\n");
        return 0;
	}
	blk_xml = nvtmpp_vb_get_block(0, pool, blk_size, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_BLK == blk_xml)
	{
		DBG_ERR("get vb block err\r\n");
        return 0;

	}
	return nvtmpp_vb_block2addr(blk_xml);

}

void XML_SetTempMem(UINT32 uiAddr, UINT32 uiSize)
{
	XMLTmpPool.Addr = uiAddr;
	XMLTmpPool.Size = uiSize;
}
UINT32 XML_GetTempMem(UINT32 uiSize)
{
	void *pBuf = 0;

	if (uiSize <= XMLTmpPool.Size) {
		pBuf = (void *)XMLTmpPool.Addr;
		//DBGH(pBuf);
	}else {
	    DBG_ERR("uiSize %x > Max %x \r\n",uiSize,XMLTmpPool.Size);
    }
	if (pBuf == 0) {
		DBG_ERR("get buffer fail\r\n");
	}
	return (UINT32)pBuf;
}

UINT32 XML_snprintf(char **buf, UINT32 *size, const char *fmt, ...)
{
	UINT32 len = 0;
	va_list marker;

	va_start(marker, fmt);       // Initialize variable arguments.
#ifdef __ECOS
#else
	if (marker)
#endif
	{
		len = vsnprintf(*buf, *size, fmt, marker);
	}

	va_end(marker);                // Reset variable arguments.

	*buf += len;
	*size = *size - len;
	return len;
}

static void XML_ecos2NvtPath(const char *inPath, char *outPath, UINT32 outPathSize)
{
	char *outOrgPath = outPath;
	outPath += snprintf(outPath, outPathSize, "A:");
	//inPath+=strlen(MOUNT_FS_ROOT);
	while (*inPath != 0) {
		if (*inPath == '/') {
			*outPath = '\\';
		} else {
			*outPath = *inPath;
		}
		inPath++;
		outPath++;
		if ((UINT32)outPath - (UINT32)outOrgPath == outPathSize - 1) {
			break;
		}
	}
	//*outPath++ = '\\'; //If adding this, it will be regarded as folder.
	*outPath = 0;
}
static void XML_Nvt2ecosPath(const char *inPath, char *outPath, UINT32 outPathSize)
{
	char *outOrgPath = outPath;

	//outPath+=sprintf(outPath,MOUNT_FS_ROOT);
	inPath += strlen("A:");
	while (*inPath != 0) {
		if (*inPath == '\\') {
			*outPath = '/';
		} else {
			*outPath = *inPath;
		}
		inPath++;
		outPath++;
		if ((UINT32)outPath - (UINT32)outOrgPath == outPathSize - 1) {
			break;
		}

	}
	*outPath = 0;
}
#if USE_FILEDB
int XML_PBGetData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	static UINT32     gIndex;
	UINT32            fileCount, i, bufflen = *bufSize;
	char             *buf = (char *)bufAddr;
	PPBX_FLIST_OBJ    pFlist = PBXFList_FDB_getObject();
	UINT32            FileDBHandle = 0;
	UINT32            xmlBufSize = *bufSize;

	PB_WaitCommandFinish(PB_WAIT_INFINITE);

	if (segmentCount == 0) {
		XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
	}

	pFlist->GetInfo(PBX_FLIST_GETINFO_DB_HANDLE, &FileDBHandle, 0);
	fileCount = FileDB_GetTotalFileNum(FileDBHandle);

	if (segmentCount == 0) {
		// reset some global variables
		gIndex = 0;
	}
	DBG_IND("gIndex = %d %d \r\n", gIndex, fileCount);
	for (i = gIndex; i < fileCount; i++) {
		PFILEDB_FILE_ATTR        pFileAttr;
		// check buffer length , reserved 512 bytes
		// should not write data over buffer length
		if ((HFS_U32)buf - bufAddr > bufflen - 512) {
			DBG_IND("totallen=%d >  bufflen(%d)-512\r\n", (HFS_U32)buf - bufAddr, bufflen);
			*bufSize = (HFS_U32)(buf) - bufAddr;
			gIndex = i;
			return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
		}
		// get dcf file
		pFileAttr = FileDB_SearhFile(FileDBHandle, i);
		if (pFileAttr) {
			//DBG_IND("file %s %d\r\n",pFileAttr->filePath,pFileAttr->fileSize);
			XML_snprintf(&buf, &xmlBufSize, "<ALLFile><File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH>\n", pFileAttr->filename, pFileAttr->filePath);
			//#NT#2016/03/30#Nestor Yang -begin
			//#NT# Support fileSize larger than 4GB
			//XML_snprintf(&buf,&xmlBufSize,"<SIZE>%d</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n",pFileAttr->fileSize,(pFileAttr->lastWriteDate <<16) + pFileAttr->lastWriteTime,
			XML_snprintf(&buf, &xmlBufSize, "<SIZE>%lld</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n", pFileAttr->fileSize64, (pFileAttr->lastWriteDate << 16) + pFileAttr->lastWriteTime,
						 FAT_GET_YEAR_FROM_DATE(pFileAttr->lastWriteDate), FAT_GET_MONTH_FROM_DATE(pFileAttr->lastWriteDate), FAT_GET_DAY_FROM_DATE(pFileAttr->lastWriteDate),
						 FAT_GET_HOUR_FROM_TIME(pFileAttr->lastWriteTime), FAT_GET_MIN_FROM_TIME(pFileAttr->lastWriteTime), FAT_GET_SEC_FROM_TIME(pFileAttr->lastWriteTime),
						 pFileAttr->attrib);
			//#NT#2016/03/30#Nestor Yang -end
		}
	}
	XML_snprintf(&buf, &xmlBufSize, "</LIST>\n");
	*bufSize = (HFS_U32)(buf) - bufAddr;
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}


#else

int XML_PBGetData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	static UINT32     gIndex;
	UINT32            fileCount, i, bufflen = *bufSize, FileType;
	char             *buf = (char *)bufAddr;
	char              tempPath[XML_PATH_LEN];
	FST_FILE_STATUS   FileStat;
	FST_FILE          filehdl;
	char              dcfFilePath[XML_PATH_LEN];
	UINT32            xmlBufSize = *bufSize;

	// set the data mimetype
	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);
	XML_ecos2NvtPath(path, tempPath, sizeof(tempPath));

	if (segmentCount == 0) {
		XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
	}
	fileCount = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
	if (segmentCount == 0) {
		// reset some global variables
		gIndex = 1;
	}
	DBG_IND("gIndex = %d %d \r\n", gIndex, fileCount);
	for (i = gIndex; i <= fileCount; i++) {
		// check buffer length , reserved 512 bytes
		// should not write data over buffer length
		if ((HFS_U32)buf - bufAddr > bufflen - 512) {
			DBG_IND("totallen=%d >  bufflen(%d)-512\r\n", (HFS_U32)buf - bufAddr, bufflen);
			*bufSize = (HFS_U32)(buf) - bufAddr;
			gIndex = i;
			return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
		}
		// get dcf file
		DCF_SetCurIndex(i);
		FileType = DCF_GetDBInfo(DCF_INFO_CUR_FILE_TYPE);

		if (FileType & DCF_FILE_TYPE_JPG) {
			DCF_GetObjPath(i, DCF_FILE_TYPE_JPG, dcfFilePath);
		} else if (FileType & DCF_FILE_TYPE_MOV) {
			DCF_GetObjPath(i, DCF_FILE_TYPE_MOV, dcfFilePath);
		} else {
			continue;
		}
		// get file state
		filehdl = FileSys_OpenFile(dcfFilePath, FST_OPEN_READ);
		FileSys_StatFile(filehdl, &FileStat);
		FileSys_CloseFile(filehdl);
		XML_Nvt2ecosPath(dcfFilePath, tempPath, sizeof(tempPath));

		// this is a dir
		if (M_IsDirectory(FileStat.uiAttrib)) {
			XML_snprintf(&buf, &xmlBufSize, "<Dir>\n<NAME>\n<![CDATA[%s]]></NAME><FPATH>\n<![CDATA[%s]]></FPATH>", &tempPath[15], tempPath);
			XML_snprintf(&buf, &xmlBufSize, "<TIMECODE>%ld</TIMECODE><TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n</Dir>\n", (FileStat.uiModifiedDate << 16) + FileStat.uiModifiedTime,
						 FAT_GET_YEAR_FROM_DATE(FileStat.uiModifiedDate), FAT_GET_MONTH_FROM_DATE(FileStat.uiModifiedDate), FAT_GET_DAY_FROM_DATE(FileStat.uiModifiedDate),
						 FAT_GET_HOUR_FROM_TIME(FileStat.uiModifiedTime), FAT_GET_MIN_FROM_TIME(FileStat.uiModifiedTime), FAT_GET_SEC_FROM_TIME(FileStat.uiModifiedTime));
		}
		// this is a file
		else {
			XML_snprintf(&buf, &xmlBufSize, "<ALLFile><File>\n<NAME>\n<![CDATA[%s]]></NAME><FPATH>\n<![CDATA[%s]]></FPATH>", &tempPath[15], tempPath);
			XML_snprintf(&buf, &xmlBufSize, "<SIZE>%lld</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n</File>\n</ALLFile>\n", FileStat.uiFileSize, (FileStat.uiModifiedDate << 16) + FileStat.uiModifiedTime,
						 FAT_GET_YEAR_FROM_DATE(FileStat.uiModifiedDate), FAT_GET_MONTH_FROM_DATE(FileStat.uiModifiedDate), FAT_GET_DAY_FROM_DATE(FileStat.uiModifiedDate),
						 FAT_GET_HOUR_FROM_TIME(FileStat.uiModifiedTime), FAT_GET_MIN_FROM_TIME(FileStat.uiModifiedTime), FAT_GET_SEC_FROM_TIME(FileStat.uiModifiedTime));
		}
	}
	XML_snprintf(&buf, &xmlBufSize, "</LIST>\n");
	*bufSize = (HFS_U32)(buf) - bufAddr;
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}
#endif


int XML_GetModeData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char             *buf = (char *)bufAddr;
	UINT32            par = 0;
	char              tmpString[32];
	UINT32            xmlBufSize = *bufSize;

	DBG_IND("path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n", path, argument, mimeType, *bufSize, segmentCount);

	// set the data mimetype
	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);

	snprintf(tmpString, sizeof(tmpString), "custom=1&cmd=%d&par=", WIFIAPP_CMD_MODECHANGE);
	if (strncmp(argument, tmpString, strlen(tmpString)) == 0) {
		sscanf_s(argument + strlen(tmpString), "%d", &par);
	} else {
		*bufSize = 0;
		DBG_ERR("error par\r\n");
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	if (par == WIFI_APP_MODE_PLAYBACK) {
		return XML_PBGetData(path, argument, bufAddr, bufSize, mimeType, segmentCount);
	} else if (par == WIFI_APP_MODE_PHOTO) {
		XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
		XML_snprintf(&buf, &xmlBufSize, "<FREEPICNUM>%d</FREEPICNUM>\n", UI_GetData(FL_WIFI_PHOTO_FREEPICNUM));
		*bufSize = (HFS_U32)(buf) - bufAddr;

		return CYG_HFS_CB_GETDATA_RETURN_OK;
	} else if (par == WIFI_APP_MODE_MOVIE) {
		XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
		XML_snprintf(&buf, &xmlBufSize, "<MAXRECTIME>%d</MAXRECTIME>\n", UI_GetData(FL_WIFI_MOVIE_MAXRECTIME));
		*bufSize = (HFS_U32)(buf) - bufAddr;
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	*bufSize = 0;
	DBG_ERR("error mode\r\n");
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}




int XML_QueryCmd(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	UINT32            i = 0;
	char             *buf = (char *)bufAddr;
	FST_FILE          filehdl;
	char              pFilePath[64];
	UINT32            fileLen = *bufSize - 512;
	UINT32            bufflen = *bufSize - 512;
	WIFI_CMD_ENTRY   *appCmd = 0;
	UINT32            xmlBufSize = *bufSize;

	// set the data mimetype
	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);

	snprintf(pFilePath, sizeof(pFilePath), "%s", QUERY_XML_PATH); //html of all command list
	filehdl = FileSys_OpenFile(pFilePath, FST_OPEN_READ);
	if (filehdl) {
		FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0, 0);
		FileSys_CloseFile(filehdl);
		*bufSize = fileLen;
		*(buf + fileLen) = '\0';
	} else {
		if (segmentCount == 0) {
			XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
		}

		XML_snprintf(&buf, &xmlBufSize, "<Function>\n");
		appCmd = WifiCmd_GetExecTable();
		while (appCmd[i].cmd != 0) {
			XML_snprintf(&buf, &xmlBufSize, "<Cmd>%d</Cmd>\n", appCmd[i].cmd);
			i++;
		}
		XML_snprintf(&buf, &xmlBufSize, "</Function>\n");

		*bufSize = (HFS_U32)(buf) - bufAddr;
	}

	if (*bufSize > bufflen) {
		DBG_ERR("no buffer\r\n");
		*bufSize = bufflen;
	}


	return CYG_HFS_CB_GETDATA_RETURN_OK;
}


FST_FILE          gMovFilehdl;
ER XML_VdoReadCB(UINT32 pos, UINT32 size, UINT32 addr)
{
	ER result = E_SYS;

	DBG_IND("XML_VdoReadCB  pos=0x%x, size=%d, addr=0x%x\r\n", pos, size, addr);
	if (gMovFilehdl) {
		FileSys_SeekFile(gMovFilehdl, pos, FST_SEEK_SET);
		//not close file,close file in XML_GetThumbnail()
		result = FileSys_ReadFile(gMovFilehdl, (UINT8 *)addr, &size, 0, 0);
	}
	return result;
}

#if TRANS_JPG_FROM_H264IDR
#include "JpgEnc.h"
#include "SizeConvert.h"

#define SCALE_BUFF_SIZE     ALIGN_CEIL_32(3840*2160*3/2)
#define JPEG_BS_SIZE        ALIGN_CEIL_32(SCALE_BUFF_SIZE/4)
#define WIFI_RESERVE_SIZE   ALIGN_CEIL_32(2*1024*1024)
#define XML_THUMB_W         320
#define XML_THUMB_H         240

typedef struct {
	UINT32  uiJpgWidth;     ///< JPEG width
	UINT32  uiJpgHeight;    ///< JPEG height
	UINT32  uiJpgBsAddr;    ///< JPEG bs address
	UINT32  uiJpgBsSize;    ///< JPEG bs size
	UINT32  uiDAR;          ///< original image DAR
	UINT32  uiFileSize;     ///< original file size
} IDR_TO_JPG, *PIDR_TO_JPG;
//#NT#2016/02/15#Ben Wang -begin
//#NEW,re-encode thumbnail for the photo that doesn't have EXIF.
static INT32 XML_MakeJpgFromIFrame(IDR_TO_JPG *pIdrToJpg, PIMG_BUF pDecodedImgBuf, UINT32 JpgBsBuf)
{
	MEM_RANGE           WorkBuf;
	IMG_BUF             DstImgBuf;
	UINT32              uiDecodeBufSize = 0;
	UINT32              uiTempBuf, uiDecodeBuf, uiScaleBuf;
	UINT32              uiJpgBsBuf, uiJpgBsSize;
	UINT32              uiEncQ;
	INT32               iResult;
	JPG_ENC_CFG         JPGParam = {0};
	UINT32              uiLineOffset[2] = {0};
	UINT32              uiPxlAddr[2] = {0};
	IMG_BUF             SrcImg = {0}, DstImg = {0};
	IRECT               SrcRegion = {0};
	IRECT               DstRegion = {0};
	SIZECONVERT_INFO    ScaleInfo = {0};

	if (NULL == pDecodedImgBuf) { //make JPG from I frame
		GxVidFile_Query1stFrameWkBufSize(&uiDecodeBufSize, pIdrToJpg->uiFileSize);
		#if 0
		uiTempBuf = XML_GetTempMem(WIFI_RESERVE_SIZE + uiDecodeBufSize + JPEG_BS_SIZE + SCALE_BUFF_SIZE);
		#else
		uiTempBuf = XML_AllocPrivateMem(WIFI_RESERVE_SIZE + uiDecodeBufSize + JPEG_BS_SIZE + SCALE_BUFF_SIZE);
		#endif
		DBG_IND("Temp buf 0x%x, need size 0x%x\r\n", uiTempBuf, uiDecodeBufSize);
		if (!uiTempBuf) {
			DBG_ERR("buf not enough!\r\n");
			return E_SYS;
		}

		uiDecodeBuf = uiTempBuf + WIFI_RESERVE_SIZE;
		uiJpgBsBuf  = uiDecodeBuf + uiDecodeBufSize;
		uiScaleBuf  = uiJpgBsBuf + JPEG_BS_SIZE;

		// decode 1st I-frame
		WorkBuf.Addr = uiDecodeBuf;
		WorkBuf.Size = uiDecodeBufSize;
		iResult = GxVidFile_Decode1stFrame(XML_VdoReadCB, &WorkBuf, &DstImgBuf);
		DBG_IND("w %d, h %d, fmt %d, Y 0x%x, U 0x%x, V 0x%x, YL %d, UL %d, VL %d\r\n",
				DstImgBuf.Width, DstImgBuf.Height, DstImgBuf.PxlFmt,
				DstImgBuf.PxlAddr[0], DstImgBuf.PxlAddr[1], DstImgBuf.PxlAddr[2],
				DstImgBuf.LineOffset[0], DstImgBuf.LineOffset[1], DstImgBuf.LineOffset[2]);

		if (iResult != GXVIDEO_PRSERR_OK) {
			DBG_ERR("decode 1st I error!\r\n");
			XML_RelPrivateMem();
			return E_SYS;
		}

		// only support YUV420 UV-packed format
		if (DstImgBuf.PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED) {
			DBG_ERR("format not 420-packed!\r\n");
			return E_SYS;
		}
	} else { //make JPG from decoded YUV data
		memcpy((void *) &DstImgBuf, (void *) pDecodedImgBuf, sizeof(IMG_BUF));
		uiJpgBsBuf  = JpgBsBuf;
		uiScaleBuf  = uiJpgBsBuf + JPEG_BS_SIZE;
	}
//#NT#2016/02/15#Ben Wang -end
	///////////////////////////////
	// scaled to JPEG resolution //
	///////////////////////////////

	// Init source buffer
	uiLineOffset[0] = DstImgBuf.LineOffset[0];
	uiLineOffset[1] = DstImgBuf.LineOffset[1];
	uiPxlAddr[0]    = DstImgBuf.PxlAddr[0];
	uiPxlAddr[1]    = DstImgBuf.PxlAddr[1];

	GxImg_InitBufEx(&SrcImg,
					DstImgBuf.Width,
					DstImgBuf.Height,
					DstImgBuf.PxlFmt,
					uiLineOffset,
					uiPxlAddr);

	// Init destination buffer
	uiLineOffset[0] = ALIGN_CEIL_32(pIdrToJpg->uiJpgWidth);
	uiLineOffset[1] = ALIGN_CEIL_32(pIdrToJpg->uiJpgWidth);
	uiPxlAddr[0]    = uiScaleBuf;
	uiPxlAddr[1]    = uiScaleBuf + ALIGN_CEIL_32(pIdrToJpg->uiJpgWidth) * ALIGN_CEIL_16(pIdrToJpg->uiJpgHeight);

	GxImg_InitBufEx(&DstImg,
					pIdrToJpg->uiJpgWidth,
					pIdrToJpg->uiJpgHeight,
					DstImgBuf.PxlFmt,
					uiLineOffset,
					uiPxlAddr);

	// Set scale region
	SrcRegion.x = 0;
	SrcRegion.y = 0;
	SrcRegion.w = DstImgBuf.Width;
	SrcRegion.h = DstImgBuf.Height;

	if (pIdrToJpg->uiDAR == MEDIAREC_DAR_16_9) {
		ScaleInfo.uiSrcWidth  = 16;
		ScaleInfo.uiSrcHeight = 9;
	} else {
		ScaleInfo.uiSrcWidth  = DstImgBuf.Width;
		ScaleInfo.uiSrcHeight = DstImgBuf.Height;
	}
	ScaleInfo.uiDstWidth  = pIdrToJpg->uiJpgWidth;
	ScaleInfo.uiDstHeight = pIdrToJpg->uiJpgHeight;
	ScaleInfo.uiDstWRatio = pIdrToJpg->uiJpgWidth;
	ScaleInfo.uiDstHRatio = pIdrToJpg->uiJpgHeight;
	ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;
	DisplaySizeConvert(&ScaleInfo);

	DstRegion.x = ScaleInfo.uiOutX;
	DstRegion.y = ScaleInfo.uiOutY;
	DstRegion.w = ScaleInfo.uiOutWidth;
	DstRegion.h = ScaleInfo.uiOutHeight;
	DBG_IND("Dst: x %d, y %d, w %d, h %d\r\n", DstRegion.x, DstRegion.y, DstRegion.w, DstRegion.h);

	// Clear buffer by black
	GxImg_FillData(&DstImg, REGION_MATCH_IMG, COLOR_YUV_BLACK);

	// Scale image by ISE
	GxImg_ScaleData(&SrcImg, &SrcRegion, &DstImg, &DstRegion);

	//////////////////////
	// do JPEG encoding //
	//////////////////////
	uiJpgBsSize                 = JPEG_BS_SIZE;
	JPGParam.Width              = pIdrToJpg->uiJpgWidth;
	JPGParam.Height             = pIdrToJpg->uiJpgHeight;
	JPGParam.JPGBSAddr          = uiJpgBsBuf;
	JPGParam.JPGBSBufSize       = &uiJpgBsSize;
	JPGParam.RawImageAddr[0]    = uiPxlAddr[0];
	JPGParam.RawImageAddr[1]    = uiPxlAddr[1];
	JPGParam.RawImageAddr[2]    = uiPxlAddr[1];
	JPGParam.LineOffset[0]      = uiLineOffset[0];
	JPGParam.LineOffset[1]      = uiLineOffset[1];
	JPGParam.LineOffset[2]      = uiLineOffset[1];
	JPGParam.YUVFormat          = JPG_FMT_YUV420;
	JPGParam.PxlLayout          = JPG_PXL_UVPACKED;
	JPGParam.DCOutCfg.DCEnable  = FALSE;
	JPGParam.BrcEn              = FALSE;

	uiEncQ = 85; // default Q
	while (uiEncQ > 40) { // minimum Q
		uiJpgBsSize = JPEG_BS_SIZE; // reset target JPEG bs size to bs buffer size
		DBG_IND("Q %d, bs size %d, buf size %d\r\n", uiEncQ, uiJpgBsSize, JPEG_BS_SIZE);
		iResult = Jpg_EncOneImg(&JPGParam, uiEncQ);
		if (iResult == E_OK) {
			break;
		}
		uiEncQ -= 5;
	}
	DBG_IND("^M Q %d, bs size %d, buf size %d\r\n", uiEncQ, uiJpgBsSize, JPEG_BS_SIZE);

	pIdrToJpg->uiJpgBsAddr = uiJpgBsBuf;
	pIdrToJpg->uiJpgBsSize = uiJpgBsSize;
	XML_RelPrivateMem();
	return E_OK;
}
#endif


typedef enum _XML_VID_TYPE {
	TYPE_MOV     =   0,
	TYPE_MP4     =   1,
	TYPE_MOV_MP4 =   2,
	TYPE_TS      =   3,
	ENUM_DUMMY4WORD(XML_VID_TYPE)
} XML_VID_TYPE;

BOOL XML_CheckSub(char *pch, XML_VID_TYPE  type)
{
	switch (type) {
	case TYPE_MOV:
		return ((strncmp(pch + 1, "mov", 3) == 0) || (strncmp(pch + 1, "MOV", 3) == 0));

	case TYPE_MP4:
		return ((strncmp(pch + 1, "mp4", 3) == 0) || (strncmp(pch + 1, "MP4", 3) == 0));

	case TYPE_MOV_MP4:
		return ((strncmp(pch + 1, "mov", 3) == 0) || (strncmp(pch + 1, "MOV", 3) == 0) ||
				(strncmp(pch + 1, "mp4", 3) == 0) || (strncmp(pch + 1, "MP4", 3) == 0));

	case TYPE_TS:
		return ((strncmp(pch + 1, "ts", 2) == 0) || (strncmp(pch + 1, "TS", 2) == 0));

	default:
		return FALSE;
	}
}

int XML_GetThumbnail(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char             *buf = (char *)bufAddr;
	FST_FILE          filehdl;
	char              tempPath[XML_PATH_LEN];
	UINT32            TempLen, TempBuf;
	UINT32            bufflen = *bufSize - 512;
	UINT32            ThumbOffset = 0;
	UINT32            ThumbSize = 0;
	static UINT32     remain = 0;
	static UINT32     ThumbBuf = 0;
	char             *pch;
	UINT32            result = 0;
#if !TRANS_JPG_FROM_H264IDR
	UINT8            *ptr = 0;
#endif
	#if 0
	TempBuf = XML_GetTempMem(POOL_SIZE_TEMP);
	#else
	TempBuf = XML_AllocPrivateMem(POOL_SIZE_TEMP);
	#endif
	if (!TempBuf) {
		XML_DefaultFormat(WIFIAPP_CMD_THUMB, WIFIAPP_RET_NOBUF, bufAddr, bufSize, mimeType);
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	// set the data mimetype
	XML_STRCPY(mimeType, "image/jpeg", CYG_HFS_MIMETYPE_MAXLEN);

	XML_ecos2NvtPath(path, tempPath, sizeof(tempPath));

	pch = strchr(tempPath, '.');

	if (!pch) {
		XML_RelPrivateMem();
		XML_DefaultFormat(WIFIAPP_CMD_THUMB, WIFIAPP_RET_PAR_ERR, bufAddr, bufSize, mimeType);
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	if (XML_CheckSub(pch, TYPE_MOV_MP4) || XML_CheckSub(pch, TYPE_TS)) {
		if (segmentCount == 0) {
			FST_FILE_STATUS FileStat = {0};
			GXVIDEO_INFO VideoInfo = {0};
			MEM_RANGE WorkBuf = {0};
			UINT32 uiBufferNeeded = 0;
			UINT32 thumbAddr = 0, thumbSize = 0;

			gMovFilehdl = FileSys_OpenFile(tempPath, FST_OPEN_READ);
			if (gMovFilehdl) {
				FileSys_StatFile(gMovFilehdl, &FileStat);
				//GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
				uiBufferNeeded = POOL_SIZE_TEMP;
				{
					//parse video info for single mode only
					WorkBuf.Addr = TempBuf;
					WorkBuf.Size = uiBufferNeeded;

					if (XML_CheckSub(pch, TYPE_MOV_MP4)) {
						result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
					} else if (XML_CheckSub(pch, TYPE_TS)) {
						result = GxVidFile_GetTsThumb(XML_VdoReadCB, &WorkBuf, &thumbAddr, &thumbSize);
					}


					if ((result == GXVIDEO_PRSERR_OK) &&
						(XML_CheckSub(pch, TYPE_MOV_MP4)) && (VideoInfo.uiThumbSize)) {
						FileSys_SeekFile(gMovFilehdl, VideoInfo.uiThumbOffset, FST_SEEK_SET);
						FileSys_ReadFile(gMovFilehdl, (UINT8 *)TempBuf, &VideoInfo.uiThumbSize, 0, 0);
						FileSys_CloseFile(gMovFilehdl);
						//*bufSize = VideoInfo.uiThumbSize;
						remain = VideoInfo.uiThumbSize;
						ThumbBuf = TempBuf;
					} else if ((result == GXVIDEO_PRSERR_OK) &&
							   (XML_CheckSub(pch, TYPE_TS)) && (thumbSize)) {
						remain = thumbSize;
						ThumbBuf = thumbAddr;
						FileSys_CloseFile(gMovFilehdl);
					} else if (VideoInfo.ui1stFrameSize) { // Return 1st I frame back to APP.
#if TRANS_JPG_FROM_H264IDR
						IDR_TO_JPG          IdrToJpg = {0};
						INT32               iResult;

						// no DAR param now, suppose 2880x2160 is DAR 16:9
						if ((VideoInfo.uiVidWidth == 2880) && (VideoInfo.uiVidHeight == 2160)) {
							IdrToJpg.uiDAR = MEDIAREC_DAR_16_9;
						} else {
							IdrToJpg.uiDAR = MEDIAREC_DAR_DEFAULT;
						}
						IdrToJpg.uiJpgWidth  = XML_THUMB_W;
						IdrToJpg.uiJpgHeight = XML_THUMB_H;
						IdrToJpg.uiFileSize  = FileStat.uiFileSize;
						//#NT#2016/02/15#Ben Wang -begin
						//#NEW,re-encode thumbnail for the photo that doesn't have EXIF.
						iResult = XML_MakeJpgFromIFrame(&IdrToJpg, 0, 0);
						//#NT#2016/02/15#Ben Wang -end

						FileSys_CloseFile(gMovFilehdl);

						if (iResult == E_OK) {
							ThumbBuf = IdrToJpg.uiJpgBsAddr;
							remain = IdrToJpg.uiJpgBsSize;
						} else {
							ThumbBuf = 0;
							remain = 0;
						}
#else
						DBG_DUMP("I frame pos:%d, size:%d\r\n", VideoInfo.ui1stFramePos, VideoInfo.ui1stFrameSize);
						DBG_DUMP("SPS/PPS header size:%d\r\n", VideoInfo.uiH264DescSize);

						if (VideoInfo.uiH264DescSize > 0x40) {
							result = WIFIAPP_RET_FILE_ERROR;
							XML_RelPrivateMem();
							XML_DefaultFormat(WIFIAPP_CMD_THUMB, result, bufAddr, bufSize, mimeType);
							FileSys_CloseFile(gMovFilehdl);
							return CYG_HFS_CB_GETDATA_RETURN_OK;
						}
						// H264 SPS/PPS header.
						hwmem_open();
						hwmem_memcpy((UINT32)TempBuf, VideoInfo.uiH264DescAddr, VideoInfo.uiH264DescSize);
						hwmem_close();

						// 1st I frame.
						FileSys_SeekFile(gMovFilehdl, VideoInfo.ui1stFramePos, FST_SEEK_SET);
						FileSys_ReadFile(gMovFilehdl, (UINT8 *)TempBuf + VideoInfo.uiH264DescSize, &VideoInfo.ui1stFrameSize, 0, 0);
						FileSys_CloseFile(gMovFilehdl);

						// Manipulate first 4 bytes in I frame as 0x01000000.
						ptr = (UINT8 *)(TempBuf + VideoInfo.uiH264DescSize);
						ptr[0] = 0x00;
						ptr[1] = 0x00;
						ptr[2] = 0x00;
						ptr[3] = 0x01;

						ThumbBuf = TempBuf;
						remain = VideoInfo.uiH264DescSize + VideoInfo.ui1stFrameSize;
#endif
					} else {
						FileSys_CloseFile(gMovFilehdl);
						result = WIFIAPP_RET_EXIF_ERR;
						DBG_ERR("No Thumbnail and I frame.\r\n");
					}
				}
			} else {
				result = WIFIAPP_RET_NOFILE;
				DBG_ERR("no %s\r\n", tempPath);
			}
		}
	} else {
		if (segmentCount == 0) {

			filehdl = FileSys_OpenFile(tempPath, FST_OPEN_READ);

			if (filehdl) {
				MEM_RANGE ExifData;
				EXIF_GETTAG exifTag;
				BOOL bIsLittleEndian = 0;
				UINT32 uiTiffOffsetBase = 0;

				TempLen = MAX_APP1_SIZE;
				FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0, 0);
				FileSys_CloseFile(filehdl);

				ExifData.Size = MAX_APP1_SIZE;
				ExifData.Addr = TempBuf;
				if (EXIF_ER_OK == EXIF_ParseExif(EXIF_HDL_ID_1, &ExifData)) {
					EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
					EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_TIFF_OFFSET_BASE, &uiTiffOffsetBase, sizeof(uiTiffOffsetBase));
					//find thumbnail
					exifTag.uiTagIfd = EXIF_IFD_1ST;
					exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT;
					if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
						ThumbOffset = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian) + uiTiffOffsetBase;
						exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT_LENGTH;
						if (EXIF_ER_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag)) {
							ThumbSize = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
						}
					}
				} else {
//#NT#2016/02/15#Ben Wang -begin
//#NEW,re-encode thumbnail for the photo that doesn't have EXIF.
					GXIMGDEC_SCALE_DOWN_LEVEL ScaleDownlevel = 0 ;
					MEM_RANGE AvailableMem;
					MEM_RANGE SrcFile;
					UINT32              uiTempBuf, uiDecodeBuf, uiDecodeBufSize, uiJpgBsBuf;
					IMG_BUF DecodedImgBuf = {0};
					GXIMG_HEADER_INFO HeaderInfo = {0};
					uiDecodeBufSize = SCALE_BUFF_SIZE;
					#if 0
					uiTempBuf = XML_GetTempMem(WIFI_RESERVE_SIZE + uiDecodeBufSize + JPEG_BS_SIZE + SCALE_BUFF_SIZE);
					#else
					uiTempBuf = XML_AllocPrivateMem(WIFI_RESERVE_SIZE + uiDecodeBufSize + JPEG_BS_SIZE + SCALE_BUFF_SIZE);
					#endif
            		if (!uiTempBuf) {
						result = WIFIAPP_RET_NOBUF;
            			DBG_ERR("buf not enough!\r\n");
            			goto GET_THUMBNAIL_RET;
            		}

					TempLen = FileSys_GetFileLen(tempPath);
					if (TempLen > WIFI_RESERVE_SIZE) {
						result = WIFIAPP_RET_NOBUF;
						DBG_ERR("size too large\r\n");
						goto GET_THUMBNAIL_RET;
					}
					filehdl = FileSys_OpenFile(tempPath, FST_OPEN_READ);
					FileSys_ReadFile(filehdl, (UINT8 *)uiTempBuf, &TempLen, 0, 0);
					FileSys_CloseFile(filehdl);

					SrcFile.Addr = uiTempBuf;
					SrcFile.Size = TempLen;
					GxImgFile_ParseHeader(&SrcFile, &HeaderInfo);

					uiDecodeBuf = uiTempBuf + WIFI_RESERVE_SIZE;
					uiJpgBsBuf  = uiDecodeBuf + uiDecodeBufSize;

					AvailableMem.Addr = uiDecodeBuf;
					AvailableMem.Size = uiDecodeBufSize;
					if (E_OK == GxImgFile_AllocDecBuf(&DecodedImgBuf, &AvailableMem, &HeaderInfo, &ScaleDownlevel)) {
						ER Ret;
						if (ScaleDownlevel) {
							Ret = GxImgFile_DecodeByDownsample(&DecodedImgBuf, &SrcFile, ScaleDownlevel);
						} else {
							Ret = GxImgFile_Decode(&DecodedImgBuf, &SrcFile);
						}
						if (E_OK == Ret) {
							IDR_TO_JPG          IdrToJpg = {0};
							INT32               iResult;

							// no DAR param now, suppose 2880x2160 is DAR 16:9
							if ((HeaderInfo.uiImageWidth == 2880) && (HeaderInfo.uiImageHeight == 2160)) {
								IdrToJpg.uiDAR = MEDIAREC_DAR_16_9;
							} else {
								IdrToJpg.uiDAR = MEDIAREC_DAR_DEFAULT;
							}
							IdrToJpg.uiJpgWidth  = 160;
							IdrToJpg.uiJpgHeight = 120;
							iResult = XML_MakeJpgFromIFrame(&IdrToJpg, &DecodedImgBuf, uiJpgBsBuf);
							if (iResult == E_OK) {
								ThumbOffset = 0;//means NOT from EXIF
								ThumbBuf = IdrToJpg.uiJpgBsAddr;
								ThumbSize = IdrToJpg.uiJpgBsSize;
							} else {
								DBG_ERR("Decode Error!(0x%X)\r\n", Ret);
								result = WIFIAPP_RET_FILE_ERROR;
								goto GET_THUMBNAIL_RET;
							}
						} else {
							DBG_ERR("Decode Error!(0x%X)\r\n", Ret);
							result = WIFIAPP_RET_FILE_ERROR;
							goto GET_THUMBNAIL_RET;

						}
					} else {
						result = WIFIAPP_RET_NOBUF;
						DBG_ERR("size too large\r\n");
						goto GET_THUMBNAIL_RET;
					}
				}

				if (ThumbSize) {
					if (ThumbSize < bufflen) {
						hwmem_open();
						if (ThumbOffset) {
							hwmem_memcpy((UINT32)buf, TempBuf + ThumbOffset, ThumbSize);
						} else {
							hwmem_memcpy((UINT32)buf, ThumbBuf, ThumbSize);
						}
						hwmem_close();
						*bufSize = ThumbSize;
						//debug_msg("photo thumb trans %d\r\n",*bufSize);

					} else {
						result = WIFIAPP_RET_NOBUF;
						DBG_ERR("size too large\r\n");
					}
				}
			} else {
				result = WIFIAPP_RET_NOFILE;
				DBG_ERR("no %s\r\n", tempPath);
			}

			ThumbBuf = 0; // JPEG thumbnail is very samll. It just takes one time to transfer.
		}
	}

	if (ThumbBuf) {
		if (remain > bufflen) {
			hwmem_open();
			hwmem_memcpy((UINT32)buf, ThumbBuf + bufflen * segmentCount, bufflen);
			hwmem_close();
			*bufSize = bufflen;
			remain -= bufflen;
			XML_RelPrivateMem();
			return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
		} else {
			hwmem_open();
			hwmem_memcpy((UINT32)buf, ThumbBuf + bufflen * segmentCount, remain);
			hwmem_close();
			*bufSize = remain;
			remain = 0;
			ThumbBuf = 0;
			//debug_msg("last trans ok\r\n");
		}
	}
GET_THUMBNAIL_RET:
//#NT#2016/02/15#Ben Wang -end
	if (result != 0) {
		// result OK return data,fail return err status
		XML_DefaultFormat(WIFIAPP_CMD_THUMB, result, bufAddr, bufSize, mimeType);
	}
	XML_RelPrivateMem();
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetMovieFileInfo(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	UINT32            TempBuf;
	UINT32            result = 0;
	GXVIDEO_INFO      VideoInfo;
	char              szFileInfo[XML_PATH_LEN];
	char              tempPath[XML_PATH_LEN];
	char             *pch;

	TempBuf = XML_GetTempMem(POOL_SIZE_TEMP);
	if (!TempBuf) {
		XML_DefaultFormat(WIFIAPP_CMD_MOVIE_FILE_INFO, WIFIAPP_RET_NOBUF, bufAddr, bufSize, mimeType);
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	// set the data mimetype
	XML_STRCPY(mimeType, "image/jpeg", CYG_HFS_MIMETYPE_MAXLEN);
	XML_ecos2NvtPath(path, tempPath, sizeof(tempPath));

	pch = strchr(tempPath, '.');

	if (!pch) {
		XML_DefaultFormat(WIFIAPP_CMD_MOVIE_FILE_INFO, WIFIAPP_RET_PAR_ERR, bufAddr, bufSize, mimeType);
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	if (XML_CheckSub(pch, TYPE_MOV_MP4) || XML_CheckSub(pch, TYPE_TS)) {
		FST_FILE_STATUS FileStat = {0};
		MEM_RANGE WorkBuf = {0};
		UINT32 uiBufferNeeded = 0;


		gMovFilehdl = FileSys_OpenFile(tempPath, FST_OPEN_READ);
		if (gMovFilehdl) {
			FileSys_StatFile(gMovFilehdl, &FileStat);
			GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);  // 2016.09.22 unmark by Boyan : we have to call this function to update GxVideo's "guiGxVidTotalFileSize" so the TS format could calculate correct movie second.
			uiBufferNeeded = POOL_SIZE_TEMP;
			{
				//parse video info for single mode only
				WorkBuf.Addr = TempBuf;
				WorkBuf.Size = uiBufferNeeded;
				result = GxVidFile_ParseVideoInfo(XML_VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
				if (result == GXVIDEO_PRSERR_OK) {
					FileSys_CloseFile(gMovFilehdl);
				} else {
					FileSys_CloseFile(gMovFilehdl);
					result = WIFIAPP_RET_EXIF_ERR;
					DBG_ERR("exif error\r\n");
				}
			}
		} else {
			result = WIFIAPP_RET_NOFILE;
			DBG_ERR("no %s\r\n", tempPath);
		}

	} else {
		result = WIFIAPP_RET_FILE_ERROR;
		DBG_ERR("no %s\r\n", tempPath);
	}


	if (result != 0) {
		XML_DefaultFormat(WIFIAPP_CMD_MOVIE_FILE_INFO, result, bufAddr, bufSize, mimeType);
	} else {
		snprintf(szFileInfo, sizeof(szFileInfo), "Width:%d, Height:%d, Length:%d sec", VideoInfo.uiVidWidth, VideoInfo.uiVidHeight, VideoInfo.uiToltalSecs);
		XML_StringResult(WIFIAPP_CMD_MOVIE_FILE_INFO, szFileInfo, bufAddr, bufSize, mimeType);
	}

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetRawEncJpg(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char             *buf = (char *)bufAddr;
	UINT32            bufflen = *bufSize - 512;
	static UINT32     remain = 0;
	static UINT32     uiJpgAddr = 0;
	UINT32            uiJpgSize = 0;
	UINT32            result = 0;


	// set the data mimetype
	XML_STRCPY(mimeType, "image/jpeg", CYG_HFS_MIMETYPE_MAXLEN);

	if (segmentCount == 0) {
		//MovRec_RawEncGetJpgData(&uiJpgAddr, &uiJpgSize, 0);

        ImageUnit_Begin(&ISF_VdoEnc, 0);
        uiJpgAddr = ImageUnit_GetParam(&ISF_VdoEnc, ISF_OUT1, VDOENC_PARAM_IMGCAP_GET_JPG_ADDR);
        uiJpgSize = ImageUnit_GetParam(&ISF_VdoEnc, ISF_OUT1, VDOENC_PARAM_IMGCAP_GET_JPG_SIZE);
        DBG_DUMP("ImageApp A=0x%x, S=%d\r\n", uiJpgAddr, uiJpgSize);
        ImageUnit_End();

		if (uiJpgAddr) {
			remain = uiJpgSize;
		} else {
			result = WIFIAPP_RET_NOFILE;
			DBG_ERR("no JPG file\r\n");
		}
	}

	if (uiJpgAddr) {
		if (remain > bufflen) {
			hwmem_open();
			hwmem_memcpy((UINT32)buf, uiJpgAddr + bufflen * segmentCount, bufflen);
			hwmem_close();
			*bufSize = bufflen;
			remain -= bufflen;
			return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
		} else { // last data.
			hwmem_open();
			hwmem_memcpy((UINT32)buf, uiJpgAddr + bufflen * segmentCount, remain);
			hwmem_close();
			*bufSize = remain;
			remain = 0;
			uiJpgAddr = 0;
			//debug_msg("last trans ok\r\n");
		}
	}

	if (result != 0) {
		XML_DefaultFormat(WIFIAPP_CMD_MOVIE_GET_RAWENC_JPG, result, bufAddr, bufSize, mimeType);
	}

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetVersion(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	XML_StringResult(WIFIAPP_CMD_VERSION, Prj_GetVersionString(), bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetLiveViewFmt(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char *buf = (char *)bufAddr;
	UINT32 xmlBufSize = *bufSize;


	XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");

#if _TODO
	// Check movie live view streaming format and assign corresponding streaming link.
	if (ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE) == NETMOVIE_TYPE_HTTP) {
		XML_snprintf(&buf, &xmlBufSize, "<MovieLiveViewLink>http://%s:8192</MovieLiveViewLink>\n", UINet_GetIP());
	} else {
		XML_snprintf(&buf, &xmlBufSize, "<MovieLiveViewLink>rtsp://%s/xxx.mov</MovieLiveViewLink>\n", UINet_GetIP());
	}
#else

	XML_snprintf(&buf, &xmlBufSize, "<MovieLiveViewLink>rtsp://%s/xxx.mov</MovieLiveViewLink>\n", UINet_GetIP());

#endif

	if (ImageApp_Photo_GetConfig(PHOTO_CFG_STRM_TYPE) == PHOTO_STRM_TYPE_HTTP) {
		XML_snprintf(&buf, &xmlBufSize, "<PhotoLiveViewLink>http://%s:8192</PhotoLiveViewLink>\n", UINet_GetIP());
	} else {
		XML_snprintf(&buf, &xmlBufSize, "<PhotoLiveViewLink>rtsp://%s/xxx.mov</PhotoLiveViewLink>\n", UINet_GetIP());
	}

	XML_snprintf(&buf, &xmlBufSize, "</LIST>\n");
	*bufSize = (HFS_U32)(buf) - bufAddr;

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_FileList(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
#if CREATE_PHOTO_FILEDB
	PFILEDB_INIT_OBJ        pFDBInitObj = &gWifiFDBInitObj;
#endif
	static FILEDB_HANDLE    FileDBHandle = 0;
	static UINT32           gIndex;
	UINT32                  fileCount, i, bufflen = *bufSize;
	char                   *buf = (char *)bufAddr;
	UINT32                  xmlBufSize = *bufSize;

	Perf_Mark();
#if CREATE_PHOTO_FILEDB
	if ((segmentCount == 0) && (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO)) {
		pFDBInitObj->u32MemAddr = XML_GetTempMem(POOL_SIZE_FILEDB);

		if (!pFDBInitObj->u32MemAddr) {
			XML_DefaultFormat(WIFIAPP_CMD_FILELIST, WIFIAPP_RET_NOBUF, bufAddr, bufSize, mimeType);
			return CYG_HFS_CB_GETDATA_RETURN_OK;
		}

		pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
		FileDBHandle = FileDB_Create(pFDBInitObj);
		DBG_IND("createTime = %04d ms \r\n", Perf_GetDuration() / 1000);
		Perf_Mark();
		FileDB_SortBy(FileDBHandle, FILEDB_SORT_BY_MODDATE, FALSE);
		DBG_IND("sortTime = %04d ms \r\n", Perf_GetDuration() / 1000);
	} else //PRIMARY_MODE_PLAYBACK,PRIMARY_MODE_MOVIE use default FileDB 0
#endif
	{
		FileDBHandle = 0;
		FileDB_Refresh(0);
	}
	//FileDB_DumpInfo(FileDBHandle);

	if (segmentCount == 0) {
		XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
	}

	fileCount = FileDB_GetTotalFileNum(FileDBHandle);
	debug_msg("fileCount  %d\r\n", fileCount);

	if (segmentCount == 0) {
		// reset some global variables
		gIndex = 0;
	}
	DBG_IND("gIndex = %d %d \r\n", gIndex, fileCount);
	for (i = gIndex; i < fileCount; i++) {
		PFILEDB_FILE_ATTR        pFileAttr;
		// check buffer length , reserved 512 bytes
		// should not write data over buffer length
		if ((HFS_U32)buf - bufAddr > bufflen - 512) {
			DBG_IND("totallen=%d >  bufflen(%d)-512\r\n", (HFS_U32)buf - bufAddr, bufflen);
			*bufSize = (HFS_U32)(buf) - bufAddr;
			gIndex = i;
			return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
		}
		// get dcf file
		pFileAttr = FileDB_SearhFile(FileDBHandle, i);
		if (pFileAttr) {
			//#NT#2016/03/30#Nestor Yang -begin
			//#NT# Support fileSize larger than 4GB
			//debug_msg("file %d %s %d\r\n",i,pFileAttr->filePath,pFileAttr->fileSize);
			debug_msg("file %d %s %lld\r\n", i, pFileAttr->filePath, pFileAttr->fileSize64);
			//#NT#2016/03/30#Nestor Yang -end

			XML_snprintf(&buf, &xmlBufSize, "<ALLFile><File>\n<NAME>%s</NAME>\n<FPATH>%s</FPATH>\n", pFileAttr->filename, pFileAttr->filePath);
			//#NT#2016/03/30#Nestor Yang -begin
			//#NT# Support fileSize larger than 4GB
			//XML_snprintf(&buf,&xmlBufSize,"<SIZE>%d</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n",pFileAttr->fileSize,(pFileAttr->lastWriteDate <<16) + pFileAttr->lastWriteTime,
			XML_snprintf(&buf, &xmlBufSize, "<SIZE>%lld</SIZE>\n<TIMECODE>%ld</TIMECODE>\n<TIME>%04d/%02d/%02d %02d:%02d:%02d</TIME>\n<ATTR>%d</ATTR></File>\n</ALLFile>\n", pFileAttr->fileSize64, (pFileAttr->lastWriteDate << 16) + pFileAttr->lastWriteTime,
						 FAT_GET_YEAR_FROM_DATE(pFileAttr->lastWriteDate), FAT_GET_MONTH_FROM_DATE(pFileAttr->lastWriteDate), FAT_GET_DAY_FROM_DATE(pFileAttr->lastWriteDate),
						 FAT_GET_HOUR_FROM_TIME(pFileAttr->lastWriteTime), FAT_GET_MIN_FROM_TIME(pFileAttr->lastWriteTime), FAT_GET_SEC_FROM_TIME(pFileAttr->lastWriteTime),
						 pFileAttr->attrib);
			//#NT#2016/03/30#Nestor Yang -end
		}
	}
	XML_snprintf(&buf, &xmlBufSize, "</LIST>\n");
	*bufSize = (HFS_U32)(buf) - bufAddr;
#if CREATE_PHOTO_FILEDB
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO) {
		FileDB_Release(FileDBHandle);
	}
#endif
	return CYG_HFS_CB_GETDATA_RETURN_OK;

}


void XML_DefaultFormat(UINT32 cmd, UINT32 ret, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType)
{
	UINT32 bufflen = *bufSize - 512;
	char  *buf = (char *)bufAddr;
	UINT32 xmlBufSize = *bufSize;

	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);

	XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
	XML_snprintf(&buf, &xmlBufSize, DEF_XML_RET, cmd, ret);
	*bufSize = (HFS_U32)(buf) - bufAddr;

	if (*bufSize > bufflen) {
		DBG_ERR("no buffer\r\n");
		*bufSize = bufflen;
	}
}

void XML_StringResult(UINT32 cmd, char *str, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType)
{
	UINT32 bufflen = *bufSize - 512;
	char  *buf = (char *)bufAddr;
	UINT32 xmlBufSize = *bufSize;

	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);
	XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
	XML_snprintf(&buf, &xmlBufSize, DEF_XML_STR, cmd, 0, str); //status OK
	*bufSize = (HFS_U32)(buf) - bufAddr;

	if (*bufSize > bufflen) {
		DBG_ERR("no buffer\r\n");
		*bufSize = bufflen;
	}
}

void XML_ValueResult(UINT32 cmd, UINT64 value, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType)
{
	UINT32 bufflen = *bufSize - 512;
	char  *buf = (char *)bufAddr;
	UINT32 xmlBufSize = *bufSize;

	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);
	XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
	XML_snprintf(&buf, &xmlBufSize, DEF_XML_VALUE, cmd, 0, value); //status OK
	*bufSize = (HFS_U32)(buf) - bufAddr;

	if (*bufSize > bufflen) {
		DBG_ERR("no buffer\r\n");
		*bufSize = bufflen;
	}
}

int XML_QueryCmd_CurSts(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	static UINT32     uiCmdIndex;
	char             *buf = (char *)bufAddr;
	FST_FILE          filehdl;
	char              pFilePath[64];
	UINT32            fileLen = *bufSize - 512;
	UINT32            bufflen = *bufSize - 512;
	WIFI_CMD_ENTRY   *appCmd = 0;
	UINT32            xmlBufSize = *bufSize;


	// set the data mimetype
	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);
	snprintf(pFilePath, sizeof(pFilePath), "%s", QUERY_CMD_CUR_STS_XML_PATH); //html of all command status list
	filehdl = FileSys_OpenFile(pFilePath, FST_OPEN_READ);
	if (filehdl) {
		FileSys_ReadFile(filehdl, (UINT8 *)buf, &fileLen, 0, 0);
		FileSys_CloseFile(filehdl);
		*bufSize = fileLen;
		*(buf + fileLen) = '\0';

		uiCmdIndex = 0;
	} else {
		if (segmentCount == 0) {
			XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
			uiCmdIndex = 0;
		}

		XML_snprintf(&buf, &xmlBufSize, "<Function>\n");
		appCmd = WifiCmd_GetExecTable();
		while (appCmd[uiCmdIndex].cmd != 0) {
			if (appCmd[uiCmdIndex].UIflag != FL_NULL) {
#if (WIFI_FINALCAM_APP_STYLE == DISABLE)
				XML_snprintf(&buf, &xmlBufSize, DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, UI_GetData(appCmd[uiCmdIndex].UIflag));
#else
				UINT32 uiRecSizeIdx = 0;

				if (appCmd[uiCmdIndex].UIflag != FL_MOVIE_SIZE) {
					XML_snprintf(&buf, &xmlBufSize, DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, UI_GetData(appCmd[uiCmdIndex].UIflag));
				} else {
					// Transfer movie size index for FinalCam APP.
					switch (UI_GetData(appCmd[uiCmdIndex].UIflag)) {
					default:
					case MOVIE_SIZE_FRONT_1920x1080P30:   // 1920 x 1080
						uiRecSizeIdx = FINALCAM_MOVIE_REC_SIZE_1080P;
						break;

					case MOVIE_SIZE_FRONT_1280x720P30:    // 1280 x 720
						uiRecSizeIdx = FINALCAM_MOVIE_REC_SIZE_720P;
						break;

					case MOVIE_SIZE_FRONT_848x480P30:    // 848 x 480
						uiRecSizeIdx = FINALCAM_MOVIE_REC_SIZE_WVGA;
						break;

					case MOVIE_SIZE_FRONT_640x480P30:     // 640 x 480
						uiRecSizeIdx = FINALCAM_MOVIE_REC_SIZE_VGA;
						break;
					}

					XML_snprintf(&buf, &xmlBufSize, DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, uiRecSizeIdx);
				}
#endif

				//debug_msg(DEF_XML_CMD_CUR_STS, appCmd[uiCmdIndex].cmd, UI_GetData(appCmd[uiCmdIndex].UIflag));
			}
			uiCmdIndex++;

			// check buffer length , reserved 512 bytes
			// should not write data over buffer length
			if ((HFS_U32)buf - bufAddr > bufflen - 512) {
				DBG_IND("totallen=%d >  bufflen(%d)-512\r\n", (HFS_U32)buf - bufAddr, bufflen);
				*bufSize = (HFS_U32)(buf) - bufAddr;
				return CYG_HFS_CB_GETDATA_RETURN_CONTINUE;
			}
		}

		XML_snprintf(&buf, &xmlBufSize, "</Function>\n");
		*bufSize = (HFS_U32)(buf) - bufAddr;
	}

	if (*bufSize > bufflen) {
		DBG_ERR("no buffer\r\n");
		*bufSize = bufflen;
	}


	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetHeartBeat(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	XML_DefaultFormat(WIFIAPP_CMD_HEARTBEAT, 0, bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetFreePictureNum(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	XML_ValueResult(WIFIAPP_CMD_FREE_PIC_NUM, PhotoExe_GetFreePicNum(), bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}
int XML_GetMaxRecordTime(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	UINT32 sec = MovieExe_GetFreeRecSec();
	XML_ValueResult(WIFIAPP_CMD_MAX_RECORD_TIME, sec, bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetDiskFreeSpace(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	DBG_IND("FileSys_GetDiskInfo(FST_INFO_DISK_SIZE) %lld\r\n", FileSys_GetDiskInfo(FST_INFO_DISK_SIZE));
	XML_ValueResult(WIFIAPP_CMD_DISK_FREE_SPACE, FileSys_GetDiskInfo(FST_INFO_FREE_SPACE), bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_DeleteOnePicture(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char    fullPath[XML_PATH_LEN] = {0};
	int     ret;
	char   *pch = 0;
	UINT8   attrib = 0;

	pch = strrchr(argument, '&');
	if (pch) {
		if (strncmp(pch, PARS_STR, strlen(PARS_STR)) == 0) {
			sscanf_s(pch + strlen(PARS_STR), "%s", &fullPath, sizeof(fullPath));
		}
		DBG_IND("fullPath path=%s\r\n", fullPath);
	} else {
		XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE, WIFIAPP_RET_FILE_LOCKED, bufAddr, bufSize, mimeType);
	}

	ret = FileSys_GetAttrib(fullPath, &attrib);
	if ((ret == E_OK) && (M_IsReadOnly(attrib) == TRUE)) {
		DBG_IND("File Locked\r\n");
		XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE, WIFIAPP_RET_FILE_LOCKED, bufAddr, bufSize, mimeType);
	} else if (ret == FST_STA_FILE_NOT_EXIST) {
		DBG_IND("File not existed\r\n");
		XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE, WIFIAPP_RET_NOFILE, bufAddr, bufSize, mimeType);
	} else {
		ret = FileSys_DeleteFile(fullPath);
		DBG_IND("ret = %d\r\n", ret);
		if (ret != FST_STA_OK) {
			XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE, WIFIAPP_RET_FILE_ERROR, bufAddr, bufSize, mimeType);
		} else {
#if 0
			Index = FileDB_GetIndexByPath(FileDBHdl, fullPath);
			DBG_IND("Index = %04d\r\n", Index);
			if (Index != FILEDB_SEARCH_ERR) {
				FileDB_DeleteFile(FileDBHdl, Index);
			}
#endif

			XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE, WIFIAPP_RET_OK, bufAddr, bufSize, mimeType);
		}
	}

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_DeleteAllPicture(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char  tempPath[XML_PATH_LEN];
	BOOL  isCurrFileReadOnly = FALSE;
	UINT32 Index;
	PFILEDB_FILE_ATTR FileAttr = NULL;
	INT32             ret;
	INT32             FileNum, i;
#if CREATE_PHOTO_FILEDB
	PFILEDB_INIT_OBJ        pFDBInitObj = &gWifiFDBInitObj;
#endif
	FILEDB_HANDLE      FileDBHandle = 0;

	Perf_Mark();

#if CREATE_PHOTO_FILEDB
	if ((segmentCount == 0) && (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO)) {
		pFDBInitObj->u32MemAddr = XML_GetTempMem(POOL_SIZE_FILEDB);

		if (!pFDBInitObj->u32MemAddr) {
			XML_DefaultFormat(WIFIAPP_CMD_DELETE_ALL, WIFIAPP_RET_NOBUF, bufAddr, bufSize, mimeType);
			return CYG_HFS_CB_GETDATA_RETURN_OK;
		}

		pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
		FileDBHandle = FileDB_Create(pFDBInitObj);
		DBG_IND("createTime = %04d ms \r\n", Perf_GetDuration() / 1000);
		Perf_Mark();
		FileDB_SortBy(FileDBHandle, FILEDB_SORT_BY_MODDATE, FALSE);
		DBG_IND("sortTime = %04d ms \r\n", Perf_GetDuration() / 1000);
	} else //PRIMARY_MODE_PLAYBACK,PRIMARY_MODE_MOVIE use default FileDB 0
#endif
	{
		FileDBHandle = 0;
		FileDB_Refresh(0);
	}
	//FileDB_DumpInfo(FileDBHandle);

	FileAttr = FileDB_CurrFile(FileDBHandle);
	if (FileAttr && M_IsReadOnly(FileAttr->attrib)) {
		isCurrFileReadOnly = TRUE;
		XML_STRCPY(tempPath, FileAttr->filePath, sizeof(tempPath));
	}

	FileNum = FileDB_GetTotalFileNum(FileDBHandle);
	for (i = FileNum - 1; i >= 0; i--) {
		FileAttr = FileDB_SearhFile(FileDBHandle, i);
		if (FileAttr) {
			if (M_IsReadOnly(FileAttr->attrib)) {
				//DBG_IND("File Locked\r\n");
				DBG_IND("File %s is locked\r\n", FileAttr->filePath);
				continue;
			}
			ret = FileSys_DeleteFile(FileAttr->filePath);
			//DBG_IND("i = %04d path=%s\r\n",i,FileAttr->filePath);
			if (ret != FST_STA_OK) {
				DBG_IND("Delete %s failed\r\n", FileAttr->filePath);
			} else {
				FileDB_DeleteFile(FileDBHandle, i);
				DBG_IND("Delete %s OK\r\n", FileAttr->filePath);
			}
		} else {
			DBG_IND("FileAttr not existed\r\n");
		}
	}

	if (isCurrFileReadOnly) {
		Index = FileDB_GetIndexByPath(FileDBHandle, tempPath);
		FileDB_SearhFile(FileDBHandle, Index);
	}

#if CREATE_PHOTO_FILEDB
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO) {
		FileDB_Release(FileDBHandle);
	}
#endif

	XML_DefaultFormat(WIFIAPP_CMD_DELETE_ALL, WIFIAPP_RET_OK, bufAddr, bufSize, mimeType);

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
extern UINT32 WifiCmd_WaitFinish(FLGPTN waiptn);

int XML_APP_STARTUP(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
#if 0 // Return fail if it's not allowed to run Wi-Fi APP command.
	XML_DefaultFormat(WIFIAPP_CMD_APP_STARTUP, WIFIAPP_RET_FAIL, bufAddr, bufSize, mimeType);
#else
	XML_DefaultFormat(WIFIAPP_CMD_APP_STARTUP, WIFIAPP_RET_OK, bufAddr, bufSize, mimeType);

//#NT#2016/05/11#Isiah Chang -begin
//#NT#clear wifi flag before waiting for it.
	WifiCmd_ClrFlg(WIFIFLAG_MODE_DONE);
//#NT#2016/05/11#Isiah Chang -end
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, PRIMARY_MODE_MOVIE, SYS_SUBMODE_WIFI);

	WifiCmd_WaitFinish(WIFIFLAG_MODE_DONE); // Moved to WifiAppCmd.c. Fill waiting flag in WIFI_CMD_ITEM table.

#endif

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}
//#NT#2016/03/23#Isiah Chang -end

int XML_GetPictureEnd(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char             *buf = (char *)bufAddr;
	UINT32            result = 0;
	UINT32            FreePicNum = 0;
	//char             *pFilePath = 0;
	//char             *pFileName = 0;
	UINT32            xmlBufSize = *bufSize;

	// set the data mimetype
	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);

	DBG_IND("path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n", path, argument, mimeType, *bufSize, segmentCount);


	if (UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL)) {
		result = WIFIAPP_RET_FOLDER_FULL;
		DBG_ERR("folder full\r\n");
	} else if (UIStorageCheck(STORAGE_CHECK_FULL, &FreePicNum)) {
		result = WIFIAPP_RET_STORAGE_FULL;
		DBG_ERR("storage full\r\n");
	} else if (System_GetState(SYS_STATE_FS) == FS_DISK_ERROR) {
		result = WIFIAPP_RET_FILE_ERROR;
		DBG_ERR("write file fail\r\n");
	} else {
	    #if 1
        //not support get file after capture,get from file list
        result = 0;
        #else
		extern CHAR *ImageApp_CamFile_GetLastWriteFilePath(void);
		pFilePath = ImageApp_CamFile_GetLastWriteFilePath();
		//DBG_ERR("path %s %d",pFilePath,strlen(pFilePath));
		if (pFilePath && strlen(pFilePath)) {
			pFileName = strrchr(pFilePath, '\\') + 1;
		} else {
			result = WIFIAPP_RET_FILE_ERROR;
			DBG_ERR("write file fail\r\n");
		}
        #endif
	}
	if (result == 0) {
		XML_snprintf(&buf, &xmlBufSize, DEF_XML_HEAD);
		XML_snprintf(&buf, &xmlBufSize, "<Function>\n");
		XML_snprintf(&buf, &xmlBufSize, DEF_XML_CMD_CUR_STS, WIFIAPP_CMD_CAPTURE, result);
		XML_snprintf(&buf, &xmlBufSize, "<File>\n<NAME></NAME>\n<FPATH></FPATH></File>\n");
		XML_snprintf(&buf, &xmlBufSize, "<FREEPICNUM>%d</FREEPICNUM>\n", FreePicNum);
		XML_snprintf(&buf, &xmlBufSize, "</Function>\n");
		*bufSize = (HFS_U32)(buf) - bufAddr;
	} else {
		XML_DefaultFormat(WIFIAPP_CMD_CAPTURE, result, bufAddr, bufSize, mimeType);
	}
	DBG_DUMP("%s", bufAddr);
	return CYG_HFS_CB_GETDATA_RETURN_OK;

}

int XML_GetBattery(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	XML_ValueResult(WIFIAPP_CMD_GET_BATTERY, GetBatteryLevel(), bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}
int XML_HWCapability(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	UINT64 value = 0;
#if (TV_FUNC == ENABLE)
	value |= HW_TV_ENABLE;
#endif
#if (GSENSOR_FUNCTION == ENABLE)
	value |= HW_GSENDOR_ENABLE;
#endif
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
	value |= HW_WIFI_SOCKET_HANDSHAKE_ENABLE;
#endif
#if (USOCKET_CLIENT == ENABLE) // Enable client socket notification for Android APP to get DHCP client's IP.
	value |= HW_WIFI_CLIENT_SOCKET_NOTIFY_ENABLE;
#endif
	//auto recording is not support after 96680 platform
	value |= HW_SET_AUTO_RECORDING_DISABLE;

	//DBG_ERR("XML_HWCapability value %x\r\n",value);
	XML_ValueResult(WIFIAPP_CMD_GET_HW_CAP, value, bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}


int XML_GetMovieRecStatus(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	INT32 iStatus = WiFiCmd_GetStatus();

	if (iStatus >= WIFI_MOV_ST_MAX) {
		DBG_ERR("Rec Sts err\r\n");
		return CYG_HFS_CB_GETDATA_RETURN_ERROR;
	}

	if (iStatus == WIFI_MOV_ST_RECORD) {
		XML_ValueResult(WIFIAPP_CMD_MOVIE_RECORDING_TIME, FlowMovie_GetRecCurrTime(), bufAddr, bufSize, mimeType);
	} else {
		XML_ValueResult(WIFIAPP_CMD_MOVIE_RECORDING_TIME, 0, bufAddr, bufSize, mimeType);
	}

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetCardStatus(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	// check card inserted
#if 0
	XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, System_GetState(SYS_STATE_CARD), bufAddr, bufSize, mimeType);
#else
	//Fix CID 32318
    UINT32 cardStatus = System_GetState(SYS_STATE_CARD);

	if (cardStatus  != CARD_REMOVED) {
		if (UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == FALSE) {
			XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, cardStatus, bufAddr, bufSize, mimeType);
		} else { // Filesystem error happened.
			// Return WIFIAPP_CMD_GET_CARD_STATUS + UI_FSStatus.
			/*
			typedef enum
			{
			    FS_DISK_ERROR = 0,
			    FS_UNKNOWN_FORMAT,
			    FS_UNFORMATTED,
			    FS_NOT_INIT,
			    FS_INIT_OK,
			    FS_NUM_FULL
			} FS_STATUS; */
			// coverity[check_return]:this function is for get card and fs status
			XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, WIFIAPP_CMD_GET_CARD_STATUS + System_GetState(SYS_STATE_FS), bufAddr, bufSize, mimeType);
		}
	} else {
		XML_ValueResult(WIFIAPP_CMD_GET_CARD_STATUS, cardStatus, bufAddr, bufSize, mimeType);
	}
#endif
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
int XML_GetCntModeStatus(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	INT32 iCurMode = System_GetState(SYS_STATE_CURRMODE);
	INT32 iCurStatus = WiFiCmd_GetStatus();
	// coverity[check_return]:this function is getting record status
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	UINT32 isRec = ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_1) |
	               ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_2) |
	               ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_3) |
	               ImageApp_MovieMulti_IsStreamRunning(_CFG_REC_ID_4);
#else
	UINT32 isRec = ImageApp_Movie_Is_Stream_Running(_CFG_REC_ID_1);
#endif
	if (iCurMode == SYS_MODE_UNKNOWN || iCurStatus >= WIFI_MOV_ST_MAX) {
		DBG_ERR("Mode err or Sts err!\r\n");
		return CYG_HFS_CB_GETDATA_RETURN_ERROR;
	}

	if (iCurMode == PRIMARY_MODE_MOVIE) {
		XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS, isRec, bufAddr, bufSize, mimeType);
	} else if (iCurMode == PRIMARY_MODE_PLAYBACK) {
		XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS, 3, bufAddr, bufSize, mimeType);
	} else if (iCurMode == PRIMARY_MODE_PHOTO) {
		XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS, 4, bufAddr, bufSize, mimeType);
	} else {
		XML_ValueResult(WIFIAPP_CMD_GET_MODE_STAUTS, iCurStatus, bufAddr, bufSize, mimeType);
	}

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}
//#NT#2016/06/01#Isiah Chang -end
int XML_GetDownloadURL(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	XML_StringResult(WIFIAPP_CMD_GET_DOWNLOAD_URL, WIFI_APP_DOWNLOAD_URL, bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

int XML_GetUpdateFWPath(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char url[XML_PATH_LEN];
	char FWPath[XML_PATH_LEN];

	XML_Nvt2ecosPath(FW_UPDATE_NAME, FWPath, sizeof(FWPath));
	snprintf(url, sizeof(url), "http://%s%s", UINet_GetIP(), (char *)FWPath);

	XML_StringResult(WIFIAPP_CMD_GET_UPDATEFW_PATH, url, bufAddr, bufSize, mimeType);
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}
//#NT#2016/06/06#Charlie Chang -begin
//#NT# support wifi AP search
static NVT_SS_STATUS_T UINet_ss_status;

int XML_GetWifiAP(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{

	char *buf = (char *)bufAddr;
	UINT32 xmlBufSize = *bufSize;
	WIFI_AUTH_TYPE auth_type = WIFI_AUTH_TYPE_NONE;
	char ssidbuf[40], tmp2Buf[20], tmp3Buf[20];
	int i = 0;
	struct nvt_bss_desc *pBss;

	UINet_SiteSurvey("wlan0", &UINet_ss_status);

	XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");

	XML_snprintf(&buf, &xmlBufSize, "<List>\n");
	for (i = 0; i < UINet_ss_status.number && UINet_ss_status.number != 0xff; i++) {
		pBss = &UINet_ss_status.bssdb[i];
		DBG_DUMP("search AP===============\r\n");
		XML_snprintf(&buf, &xmlBufSize, "<AP_index>\n");
		memset(ssidbuf, '\0', sizeof(ssidbuf));
		memcpy(ssidbuf, pBss->ssid, pBss->ssidlen);
		ssidbuf[pBss->ssidlen] = '\0';
		DBG_DUMP("%s\r\n", ssidbuf);
		XML_snprintf(&buf, &xmlBufSize, "<SSID>%s</SSID>\n", ssidbuf);


		DBG_DUMP("MAC %02X:%02X:%02X:%02X:%02X:%02X\r\n",
				 pBss->bssid[0], pBss->bssid[1], pBss->bssid[2],
				 pBss->bssid[3], pBss->bssid[4], pBss->bssid[5]);


		// refer to get_security_info() for details
		if ((pBss->capability & nvt_cPrivacy) == 0) {
			/////NONE
			auth_type = WIFI_AUTH_TYPE_NONE;
		} else {
			if (pBss->t_stamp[0] == 0) {
				////WEP
				DBG_DUMP("\r\n");
			} else {
				int wpa_exist = 0, idx = 0;
				if (pBss->t_stamp[0] & 0x0000ffff) {

					////WPA
					idx = sprintf(tmp2Buf, "%s", "WPA");
					if (((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x4) {
						////PSK
						idx += sprintf(tmp2Buf + idx, "%s", "-PSK");
					} else if (((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x2) {
						/////-1X
						idx += sprintf(tmp2Buf + idx, "%s", "-1X");
					}

					wpa_exist = 1;

					if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x5) {
						//////AES/TKIP
						sprintf(tmp3Buf, "%s", "AES/TKIP");
						auth_type = WIFI_AUTH_TYPE_WPA_AES_TKIP;
					} else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x4) {
						/////AES
						sprintf(tmp3Buf, "%s", "AES");
						auth_type = WIFI_AUTH_TYPE_WPA_AES;
					} else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x1) {
						////TKIP
						sprintf(tmp3Buf, "%s", "TKIP");
						auth_type = WIFI_AUTH_TYPE_WPA_TKIP;
					}
				}
				if (pBss->t_stamp[0] & 0xffff0000) {

					if (wpa_exist) {
						idx += sprintf(tmp2Buf + idx, "%s", "/");
					}
					////WPA2
					idx += sprintf(tmp2Buf + idx, "%s", "WPA2");
					if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x4) {
						////-PSK
						idx += sprintf(tmp2Buf + idx, "%s", "-PSK");
					} else if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x2) {
						////-1X;
						idx += sprintf(tmp2Buf + idx, "%s", "-1X");
					}

					if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x5) {
						////AES/TKIP
						sprintf(tmp3Buf, "%s", "AES/TKIP");
						auth_type = WIFI_AUTH_TYPE_WPA2_AES_TKIP;
					} else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x4) {
						////AES
						sprintf(tmp3Buf, "%s", "AES");
						auth_type = WIFI_AUTH_TYPE_WPA2_AES;
					} else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x1) {
						////TKIP
						sprintf(tmp3Buf, "%s", "TKIP");
						auth_type = WIFI_AUTH_TYPE_WPA2_TKIP;
					}
				}
			}
		}
		DBG_DUMP("%-10s ", tmp3Buf);
		DBG_DUMP("%-16s\r\n", tmp2Buf);
		XML_snprintf(&buf, &xmlBufSize, "<Auth_type>%d</Auth_type>\n", auth_type);

		DBG_DUMP("rssi %d\r\n", pBss->rssi);
		XML_snprintf(&buf, &xmlBufSize, "<RSSI>%d dBm</RSSI>\n", pBss->rssi);

		XML_snprintf(&buf, &xmlBufSize, "</AP_index>\n");
	}
	XML_snprintf(&buf, &xmlBufSize, "</List>\n");
	*bufSize = (HFS_U32)(buf) - bufAddr;
	return CYG_HFS_CB_GETDATA_RETURN_OK;

}
//#NT#2016/06/06#Charlie Chang -end
int XML_GetSSID_passphrase(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char *buf = (char *)bufAddr;
	UINT32 xmlBufSize = *bufSize;

	XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");

	XML_snprintf(&buf, &xmlBufSize, "<SSID>%s</SSID>\n", UINet_GetSSID());

	XML_snprintf(&buf, &xmlBufSize, "<PASSPHRASE>%s</PASSPHRASE>\n", UINet_GetPASSPHRASE());

	XML_snprintf(&buf, &xmlBufSize, "</LIST>\n");
	*bufSize = (HFS_U32)(buf) - bufAddr;

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}


int XML_GetMovieSizeCapability(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char   *buf = (char *)bufAddr;
	TM_ITEM    *pItem = 0;
	TM_OPTION  *pOption;
	UINT16      i = 0;
	UINT32      xmlBufSize = *bufSize;

	pItem = TM_GetItem(&gMovieMenu, IDM_MOVIE_SIZE);
	if (pItem) {
		XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);

		XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");
		for (i = 0; i < pItem->Count; i++) {
			pOption = &pItem->pOptions[i];
			if ((pOption->Status & TM_OPTION_STATUS_MASK) == TM_OPTION_ENABLE) {
				XML_snprintf(&buf, &xmlBufSize, "<Item>\n");

				XML_snprintf(&buf, &xmlBufSize, "<Name>%s</Name>\n", UIRes_GetUserString(pOption->TextId));

				XML_snprintf(&buf, &xmlBufSize, "<Index>%d</Index>\n", i);

				XML_snprintf(&buf, &xmlBufSize, "<Size>%d*%d</Size>\n", GetMovieSizeWidth_2p(0, i), GetMovieSizeHeight_2p(0, i));

				XML_snprintf(&buf, &xmlBufSize, "<FrameRate>%d</FrameRate>\n", GetMovieFrameRate_2p(0, i));

				XML_snprintf(&buf, &xmlBufSize, "<Type>%d</Type>\n", GetMovieRecType_2p(i));

				XML_snprintf(&buf, &xmlBufSize, "</Item>\n");
			}
		}

		XML_snprintf(&buf, &xmlBufSize, "</LIST>\n");
		*bufSize = (HFS_U32)(buf) - bufAddr;
	} else {
		XML_ValueResult(WIFIAPP_CMD_QUERY_MOVIE_SIZE, WIFIAPP_RET_FAIL, bufAddr, bufSize, mimeType);
	}
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

#define QUERYMENU_CMD_STR "custom=1&cmd=3031&str="

int XML_GetMenuItem(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	char  *buf = (char *)bufAddr;
	WIFI_CMD_ENTRY  *appCmd = 0;
	UINT32          uiCmdIndex = 0;
	WIFIAPPINDEXMAP  *mapTbl = 0;
	WIFIAPPMAP       *wifiAppFlagMap;
	UINT32 i = 0, j = 0;
	UINT8 auth[2] = {0};
	UINT8 key[4] = {0};
	UINT32 xmlBufSize = *bufSize;

	appCmd = WifiCmd_GetExecTable();
	wifiAppFlagMap = WifiCmd_GetMapTbl();

	if ((!appCmd) || (!wifiAppFlagMap)) {
		XML_DefaultFormat(WIFIAPP_CMD_QUERY_MENUITEM, WIFIAPP_RET_FAIL, bufAddr, bufSize, mimeType);
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);
	XML_snprintf(&buf, &xmlBufSize, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<LIST>\n");

	while (appCmd[uiCmdIndex].cmd != 0) {
		if (appCmd[uiCmdIndex].UIflag != FL_NULL) {
			i = 0;
			while (wifiAppFlagMap[i].uiFlag != FL_NULL) {
				if (wifiAppFlagMap[i].uiFlag == appCmd[uiCmdIndex].UIflag) {
					XML_snprintf(&buf, &xmlBufSize, "<Item>\n");
					XML_snprintf(&buf, &xmlBufSize, "<Cmd>%d</Cmd>\n", appCmd[uiCmdIndex].cmd);
					XML_snprintf(&buf, &xmlBufSize, "<Name>%s</Name>\n", wifiAppFlagMap[i].name);
					XML_snprintf(&buf, &xmlBufSize, "<MenuList>\n");

					mapTbl = wifiAppFlagMap[i].maptbl;
					j = 0;
					while (mapTbl[j].uiIndex != INDEX_END) {
						XML_snprintf(&buf, &xmlBufSize, "<Option>\n");
						XML_snprintf(&buf, &xmlBufSize, "<Index>%d</Index>\n", mapTbl[j].uiIndex);
						XML_snprintf(&buf, &xmlBufSize, "<Id>%s</Id>\n", mapTbl[j].string);
						XML_snprintf(&buf, &xmlBufSize, "</Option>\n");
						j++;
					}
					XML_snprintf(&buf, &xmlBufSize, "</MenuList>\n");
					XML_snprintf(&buf, &xmlBufSize, "</Item>\n");
				}
				i++;
			}
		}
		uiCmdIndex++;
	}

	//DBG_DUMP("Data2 path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n",path,argument, mimeType, *bufSize, segmentCount);

	if (strncmp(argument, QUERYMENU_CMD_STR, strlen(QUERYMENU_CMD_STR)) == 0) {
		UINT32 keyValue = 0;
		sscanf_s(argument + strlen(QUERYMENU_CMD_STR), "%x", &keyValue);

		key[0] = keyValue >> 24 & 0xFF;
		key[1] = keyValue >> 16 & 0xFF;
		key[2] = keyValue >> 8 & 0xFF;
		key[3] = keyValue & 0xFF;
	} else {
		DBG_ERR("no key\r\n");
		XML_DefaultFormat(WIFIAPP_CMD_QUERY_CUR_STATUS, WIFIAPP_RET_PAR_ERR, bufAddr, bufSize, mimeType);
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	Nvt_AuthGen(key, (UINT8 *)bufAddr, (UINT32)((buf) - bufAddr), auth);

	XML_snprintf(&buf, &xmlBufSize, "<CHK>%02X%02X</CHK>\n", auth[0], auth[1]);

	XML_snprintf(&buf, &xmlBufSize, "</LIST>\n");
	*bufSize = (HFS_U32)(buf) - bufAddr;

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}
INT32 WifiCmd_getQueryData(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	UINT32 bufflen = *bufSize - 512;
	char  *buf = (char *)bufAddr;
	UINT32 xmlBufSize = *bufSize;

	DBG_DUMP("Data2 path = %s, argument -> %s, mimeType= %s, bufsize= %d, segmentCount= %d\r\n", path, argument, mimeType, *bufSize, segmentCount);

	XML_STRCPY(mimeType, "text/xml", CYG_HFS_MIMETYPE_MAXLEN);

	XML_snprintf(&buf, &xmlBufSize, QUERY_FMT, WIFI_APP_MANUFACTURER, 1, WIFI_APP_MODLE);
	DBG_DUMP(buf);

	*bufSize = (HFS_U32)(buf) - bufAddr;

	if (*bufSize > bufflen) {
		DBG_ERR("no buffer\r\n");
		*bufSize = bufflen;
	}
	return CYG_HFS_CB_GETDATA_RETURN_OK;

}

int XML_AutoTestCmdDone(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 *bufSize, char *mimeType, HFS_U32 segmentCount)
{
	UINT32            par = 0;
	char              tmpString[32];

	snprintf(tmpString, sizeof(tmpString), "custom=1&cmd=%d&par=", WIFIAPP_CMD_AUTO_TEST_CMD_DONE);
	if (strncmp(argument, tmpString, strlen(tmpString)) == 0) {
		sscanf_s(argument + strlen(tmpString), "%d", &par);
		DBG_DUMP("(`';)\r\n");
	} else {
		*bufSize = 0;
		DBG_ERR("error par\r\n");

		XML_ValueResult(WIFIAPP_CMD_AUTO_TEST_CMD_DONE, WIFIAPP_RET_FAIL, bufAddr, bufSize, mimeType);
		return CYG_HFS_CB_GETDATA_RETURN_OK;
	}

	XML_ValueResult(WIFIAPP_CMD_AUTO_TEST_CMD_DONE, WIFIAPP_RET_OK, bufAddr, bufSize, mimeType);

	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

#define MAX_UPLOAD_FILE_SIZE                 0x00A00000
#define FW_UPLOAD_USAGE_SAVE_AS_FILE         0x00
#define FW_UPLOAD_USAGE_UPDATE_FW_IN_DRAM    0x01
#define FW_UPLOAD_USAGE_SAVE_IN_DRAM         0x02
#if _TODO
static FWSRV_CMD Cmd = {0};
#endif
int XML_UploadFile(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 bufSize, HFS_U32 segmentCount, HFS_PUT_STATUS putStatus)
{
	static      FST_FILE fhdl = 0;
	static      UINT32   uiTempBuf = 0, uiCopiedBytes = 0;
	static      UINT32   uiUsage = FW_UPLOAD_USAGE_SAVE_AS_FILE;
	UINT32      fileSize = 0;
#if _TODO
	FWSRV_ER    fws_er;
#endif
	char        tempPath[XML_PATH_LEN];
	char        tmpString[32];

	DBG_IND("path =%s, argument = %s, bufAddr = 0x%x, bufSize =0x%x , segmentCount  =%d , putStatus = %d\r\n", path, argument, bufAddr, bufSize, segmentCount, putStatus);

	if (putStatus == HFS_PUT_STATUS_ERR) {
		DBG_ERR("receive data error\r\n");
		return CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR;
	} else {
		if (segmentCount == 0) {
			// Check if upload to a file or DRAM buffer.
			snprintf(tmpString, sizeof(tmpString), "custom=1&cmd=%d&par=", WIFIAPP_CMD_UPLOAD);
			if (strncmp(argument, tmpString, strlen(tmpString)) == 0) {
				sscanf_s(argument + strlen(tmpString), "%d", &uiUsage);
			} else {
				uiUsage = FW_UPLOAD_USAGE_SAVE_AS_FILE;
			}

			if (uiUsage == FW_UPLOAD_USAGE_SAVE_AS_FILE) {
				XML_ecos2NvtPath(path, tempPath, sizeof(tempPath));
				fhdl = FileSys_OpenFile(tempPath, FST_CREATE_ALWAYS | FST_OPEN_WRITE);
				FileSys_SeekFile(fhdl, 0, FST_SEEK_SET);
			} else {
				uiCopiedBytes = 0;
				uiTempBuf = XML_GetTempMem(MAX_UPLOAD_FILE_SIZE);
                if(!uiTempBuf){
					DBG_ERR("buf not enough\r\n");
					return CYG_HFS_UPLOAD_FAIL_WRITE_ERROR;
                }

			}
		}

		if (uiUsage == FW_UPLOAD_USAGE_SAVE_AS_FILE) {
			fileSize = bufSize;
			FileSys_SeekFile(fhdl, 0, FST_SEEK_END);
			FileSys_WriteFile(fhdl, (UINT8 *)bufAddr, &fileSize, 0, NULL);
		} else { // Copy file to a buffer.
			if ((uiCopiedBytes + bufSize) <= MAX_UPLOAD_FILE_SIZE) {
				hwmem_open();
				hwmem_memcpy((UINT32)(uiTempBuf + uiCopiedBytes), bufAddr, bufSize);
				hwmem_close();

				uiCopiedBytes += bufSize;
				//debug_msg("^BCopying...%d\r\n", uiCopiedBytes);
			} else {
				DBG_ERR("Out of bound of buffer!\r\n");
				return CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR;
			}
		}

		if (putStatus == HFS_PUT_STATUS_FINISH) {
			if (uiUsage == FW_UPLOAD_USAGE_SAVE_AS_FILE) {
				FileSys_CloseFile(fhdl);
				fhdl = 0;
			} else if (uiUsage == FW_UPLOAD_USAGE_UPDATE_FW_IN_DRAM) { // Update FW into Flash. Might send POWER_OFF command then.
				debug_msg("^GCopied bytes:%d\r\n", uiCopiedBytes);

#if 1
#if (FWS_FUNC == ENABLE)
#if _TODO
				debug_msg("^BUpdate FW - begin\r\n");
				//#NT#2016/12/28#Niven Cho -begin
				//#NT#FIXED, update from dram should still be all-in-one
				FWSRV_BIN_UPDATE_ALL_IN_ONE FwInfo = {0};
				FwInfo.uiSrcBufAddr = uiTempBuf;
				FwInfo.uiSrcBufSize = uiCopiedBytes;
				FwInfo.uiWorkBufAddr = uiTempBuf + ALIGN_CEIL_16(uiCopiedBytes);
				FwInfo.uiWorkBufSize = MAX_UPLOAD_FILE_SIZE;
				memset(&Cmd, 0, sizeof(Cmd));
				Cmd.Idx = FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE;
				Cmd.In.pData = &FwInfo;
				Cmd.In.uiNumByte = sizeof(FwInfo);
				Cmd.Prop.bExitCmdFinish = TRUE;
				fws_er = FwSrv_Cmd(&Cmd);
				//#NT#2016/12/28#Niven Cho -end
				if (fws_er != FWSRV_ER_OK) {
					debug_msg("FW bin write failed\r\n");
					return UPDNAND_STS_FW_WRITE_CHK_ERR;
				}

				debug_msg("^BUpdate FW - end\r\n");
#endif
#endif
#else // Save as file to do verification.
				{
					fhdl = FileSys_OpenFile("A:\\test.bin", FST_CREATE_ALWAYS | FST_OPEN_WRITE);
					FileSys_WriteFile(fhdl, (UINT8 *)uiTempBuf, &uiCopiedBytes, 0, NULL);
					FileSys_CloseFile(fhdl);
					fhdl = 0;
				}
#endif
			} else {
				//debug_msg("File saved in DRAM.\r\n");
			}
		}
	}

	return CYG_HFS_UPLOAD_OK;
}

#if defined(_CPU2_LINUX_)
int XML_UploadAudio(char *path, char *argument, HFS_U32 bufAddr, HFS_U32 bufSize, HFS_U32 segmentCount, HFS_PUT_STATUS putStatus)
{
	//static      UINT32   uiUsage = 0;
	//char        tmpString[32];

	//DBG_MSG("path =%s, argument = %s, bufAddr = 0x%x, bufSize =0x%x , segmentCount  =%d , putStatus = %d\r\n",path,argument ,bufAddr, bufSize, segmentCount, putStatus);
	/*
	[path]

	[argument]
	http://192.168.0.3/?custom=1&cmd=5002&par=0  //
	http://192.168.0.3/?custom=1&cmd=5002&par=1
	http://192.168.0.3/?custom=1&cmd=5002&par=2

	[bufAddr]

	[bufSize]

	[segmentCount]
	0, 1, 2, ....

	[putStatus]
	HFS_PUT_STATUS_CONTINUE
	HFS_PUT_STATUS_FINISH
	HFS_PUT_STATUS_ERR
	*/
	if (putStatus == HFS_PUT_STATUS_ERR) {
		DBG_ERR("receive data error\r\n");
		return CYG_HFS_UPLOAD_FAIL_RECEIVE_ERROR;
	} else {
		if (segmentCount == 0) { //first-segment
			//send data to system audio
			System_PutAudioData(0, (UINT32)bufAddr, (UINT32 *)&bufSize);
		}
		if (putStatus == HFS_PUT_STATUS_FINISH) { //last-segment
			//DBG_MSG("HFS: AUDIO UPLOAD - end\r\n");
		}
	}

	return CYG_HFS_UPLOAD_OK;
}
#endif
#endif

