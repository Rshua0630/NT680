#include "PrjCfg.h"
#include <stdio.h>
#include <string.h>
#include "SysKer.h"
#include "SxCmd.h"
#if (CURL_FUNC == ENABLE)
#include "CurlNvtAPI.h"
//global debug level: PRJ_DBG_LVL
#include "NetFs.h"
#endif
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          NetFs
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#if (CURL_FUNC == ENABLE)

#define CFG_NETFS_INIT_KEY         MAKEFOURCC('N','F','S','K')
#define NETFS_MAX_HANDLE_NUM       2

#define NETFS_CMD_READ             0x10
#define NETFS_CMD_WRITE            0x11
#define NETFS_CMD_TELL             0x12


typedef struct {
	UINT32 uiInitKey;
	UINT32 rwCmd;
	UINT32 read_offset;
	UINT32 write_offset;
	UINT32 buffAddr;
	UINT32 buffSize;
	UINT64 fileSize;
	UINT64 filePos;
	char   path[128];
	char   curlcmd[256];
	char   uploadSrcPath[128];
	FileSys_CB CB;
	UINT32 isAbort;

} NETFS_HANDLE_INFO;


typedef struct {
	char               remoteHostIP[64];
	char               rootdir[12];
	BOOL               isSSL;

	NETFS_HANDLE_INFO  *hdlInfo[NETFS_MAX_HANDLE_NUM];
} NETFS_INFO;

static NETFS_HANDLE_INFO    ghdlInfo[NETFS_MAX_HANDLE_NUM] = {0};
static NETFS_INFO    gNetFsInfo = {"192.168.1.254",
								   "A:\\",
								   0,
								   &ghdlInfo[0], &ghdlInfo[1]
								  };
#if 0
static void CurlCmd_dumpmem(UINT32 Addr, UINT32 size)
{
	UINT32 i, j;
	DBG_DUMP("\r\n Addr=0x%x, size =0x%x\r\n", Addr, size);


	for (j = Addr; j < (Addr + size); j += 0x10) {
		DBG_DUMP("^R 0x%8x:", j);
		for (i = 0; i < 16; i++) {
			DBG_DUMP("0x%2x,", *(char *)(j + i));
		}
		DBG_DUMP("\r\n");
	}
	DBG_DUMP("Data %s\r\n", (char *)Addr);

}
#endif

static int NetFs_caseless_compare_withlen(const char *name1, const char *name2, int len)
{
	int    i = 0;
	char   c1, c2;

	while (i < len) {
		c1 = name1[i];
		c2 = name2[i];
		if (c1 >= 'a' && c1 <= 'z') {
			c1 -= 0x20;
		}
		if (c2 >= 'a' && c2 <= 'z') {
			c2 -= 0x20;
		}
		if (c1 != c2) {
			return FALSE;
		}
		i++;
	}
	return TRUE;
}

static void  NetFs_CurlCmd_FinishCB(CURL_ER cmdSts, void *userdata)
{
	NETFS_HANDLE_INFO *pHdlInfo = (NETFS_HANDLE_INFO *)userdata;

	DBG_IND("cmdSts = %d, userdata = 0x%x\r\n", cmdSts, userdata);
	if (pHdlInfo->CB) {
		pHdlInfo->CB(cmdSts, cmdSts, 0, 0);
	}
}

static size_t NetFs_CurlCmd_ReadCB(void *buffer, size_t sz, size_t nmemb, void *userdata)
{
	NETFS_HANDLE_INFO *pHdlInfo = (NETFS_HANDLE_INFO *)userdata;
	size_t     rsize  = sz * nmemb;
	UINT32              bContine = TRUE;

	DBG_IND("buffer = 0x%x, size %d, userdata 0x%x, offset=0x%x\r\n", (int)buffer, (int)(rsize), (int)userdata, pHdlInfo->read_offset);
	if (pHdlInfo->rwCmd ==  NETFS_CMD_READ) {
		DBG_DUMP("%s\r\n", buffer);
		return rsize;
	}
	if (pHdlInfo->read_offset + rsize > pHdlInfo->buffSize) {
		//DBG_ERR("r offset 0x%x+ rsize 0x%x > buffSize 0x%x\r\n",pHdlInfo->read_offset,rsize,pHdlInfo->buffSize);
		//return 0;
		rsize = pHdlInfo->buffSize - pHdlInfo->read_offset;
	}
	if (pHdlInfo->CB) {
		pHdlInfo->CB(NETFS_CMD_READ, 0, (UINT32)&bContine, 0);
		if (bContine == FALSE) {
			DBG_DUMP("curl abort\r\n");
			pHdlInfo->isAbort = TRUE;
			return CURL_READFUNC_ABORT;
		}
	}
	memcpy(buffer, (void *)(pHdlInfo->buffAddr + pHdlInfo->read_offset), rsize);
	//CurlCmd_dumpmem((UINT32)buffer,rsize);
	pHdlInfo->read_offset += (rsize);
	pHdlInfo->filePos += (rsize);
	DBG_IND("r offset = 0x%x , pos = 0x%x\r\n", pHdlInfo->read_offset, pHdlInfo->filePos);
	return rsize;
}

static size_t NetFs_CurlCmd_WriteCB(void *buffer, size_t sz, size_t nmemb, void *userdata)
{
	NETFS_HANDLE_INFO *pHdlInfo = (NETFS_HANDLE_INFO *)userdata;
	size_t     wsize  = sz * nmemb;
	UINT32              bContine = TRUE;

	DBG_IND("buffer = 0x%x, size %d, userdata 0x%x, offset=0x%x\r\n", (int)buffer, (int)(wsize), (int)userdata, pHdlInfo->write_offset);
	if (pHdlInfo->rwCmd ==  NETFS_CMD_WRITE || pHdlInfo->rwCmd ==  NETFS_CMD_TELL) {
		//DBG_DUMP("%s\r\n",buffer);
		return wsize;
	}
	if (pHdlInfo->write_offset + wsize > pHdlInfo->buffSize) {
		DBG_ERR("w offset 0x%x+ wsize 0x%x > buffSize 0x%x\r\n", pHdlInfo->write_offset, wsize, pHdlInfo->buffSize);
		return 0;
	}
	if (pHdlInfo->CB) {
		pHdlInfo->CB(NETFS_CMD_READ,  0, (UINT32)&bContine, 0);
		if (bContine == FALSE) {
			DBG_DUMP("curl abort\r\n");
			pHdlInfo->isAbort = TRUE;
			return CURL_READFUNC_ABORT;
		}
	}
	memcpy((void *)(pHdlInfo->buffAddr + pHdlInfo->write_offset), buffer, wsize);
	pHdlInfo->write_offset += (wsize);
	pHdlInfo->filePos += (wsize);
	DBG_IND("w offset = 0x%x, pos = 0x%x\r\n", pHdlInfo->write_offset, pHdlInfo->filePos);
	return wsize;
}

static size_t NetFs_CurlCmd_HeaderCB(void *buffer, size_t sz, size_t nmemb, void *userdata)
{
	NETFS_HANDLE_INFO *pHdlInfo = (NETFS_HANDLE_INFO *)userdata;
	size_t     wsize  = sz * nmemb;

	//DBG_IND("buffer = 0x%x, size %d, userdata 0x%x, offset=0x%x\r\n",(int)buffer,(int)(wsize),(int)userdata,pHdlInfo->write_offset);
	if (pHdlInfo->rwCmd ==  NETFS_CMD_TELL) {
		char keyTempstr[30];
		int  len;
		char *pBuf = (char *)buffer;

		len = sprintf(keyTempstr, "Content-Length: ");
		if (NetFs_caseless_compare_withlen(keyTempstr, buffer, len)) {
			char *value = (char *)pBuf + len;

			pBuf[wsize - 1] = 0;
			sscanf(value, "%lld", &pHdlInfo->fileSize);
			DBG_IND(" value =%s, fileSize %d \r\n", value, pHdlInfo->fileSize);
		}
	}
	return wsize;
}

static NETFS_HANDLE_INFO  *NetFs_GetFreeHandle(void)
{
	UINT32      i;
	NETFS_HANDLE_INFO  *pHdlInfo;
	for (i = 0; i < NETFS_MAX_HANDLE_NUM; i++) {
		pHdlInfo = gNetFsInfo.hdlInfo[i];
		if (pHdlInfo->uiInitKey != CFG_NETFS_INIT_KEY) {
			return pHdlInfo;
		}
	}
	return NULL;
}

static BOOL  NetFs_ChkValidHandle(NET_FILE  pFile)
{
	UINT32      i;
	NETFS_HANDLE_INFO  *pHdlInfo;
	for (i = 0; i < NETFS_MAX_HANDLE_NUM; i++) {
		pHdlInfo = gNetFsInfo.hdlInfo[i];
		if (pFile == (NET_FILE)pHdlInfo && pHdlInfo->uiInitKey == CFG_NETFS_INIT_KEY) {
			return TRUE;
		}
	}
	return FALSE;
}

INT32     NetFs_SetParam(UINT32 param, UINT32 value)
{
	DBG_IND("parm=%d \r\n", param);
	switch (param) {
	case NETFS_PARAM_REMOTE_IP: {
			char *ip = (char *)value;
			if (!ip) {
				DBG_ERR("param = %d, value=%d\r\n", param, value);
				return NETFS_STA_PARAM_ERR;
			}
			strncpy(gNetFsInfo.remoteHostIP, ip, sizeof(gNetFsInfo.remoteHostIP) - 1);
		}
		break;
	case NETFS_PARAM_ROOT_DIR: {
			char *rootdir = (char *)value;
			if (!rootdir) {
				DBG_ERR("param = %d, value=%d\r\n", param, value);
				return NETFS_STA_PARAM_ERR;
			}
			strncpy(gNetFsInfo.rootdir, rootdir, sizeof(gNetFsInfo.rootdir) - 1);
		}
		break;
	case NETFS_PARAM_USE_SSL:
		gNetFsInfo.isSSL = value;
		break;

	default:
		DBG_ERR("param = %d\r\n", param);
		return NETFS_STA_PARAM_ERR;
	}
	return NETFS_STA_OK;
}

// the input pPath ex:  A:\NOVATEK\PHOTO\2000_0101_002223_001.JPG
// the output url  ex:  http://192.168.1.254/NOVATEK/PHOTO/2000_0101_002223_001.JPG
static void  NetFs_filepath2url(char *pPath, char *url, UINT32 urlBuflen)
{
	UINT32 len;
	char *pin, *pout;

	if (gNetFsInfo.remoteHostIP[0] == 0) {
		DBG_ERR("NotConfig remoteHostIP\r\n");
	}
	if (gNetFsInfo.isSSL) {
		len = snprintf(url, urlBuflen, "https://%s/", gNetFsInfo.remoteHostIP);
	} else {
		len = snprintf(url, urlBuflen, "http://%s/", gNetFsInfo.remoteHostIP);
	}
	pin = pPath + strlen(gNetFsInfo.rootdir); // skip  "A:\"
	pout = url + len;
	while (len < urlBuflen) {
		if (*pin == '\\') {
			*pout = '/';
		} else {
			*pout = *pin;
		}
		if (*pin == 0) {
			break;
		}
		pin++;
		pout++;
	}
	url[urlBuflen - 1] = 0;
	DBG_IND("pPath = %s, url = %s\r\n", pPath, url);
}

static void NetFs_Nvt2ecosPath(const char *ecosRoot, const char *inPath, char *outPath, UINT32 outPathlen)
{
	outPath += snprintf(outPath, outPathlen, ecosRoot);
	inPath += strlen(gNetFsInfo.rootdir);
	while (*inPath != 0) {
		if (*inPath == '\\') {
			*outPath = '/';
		} else {
			*outPath = *inPath;
		}
		inPath++;
		outPath++;
	}
	*outPath = 0;
}

NET_FILE  NetFs_OpenFile(char *pPath, UINT32 Flag)
{
	NETFS_HANDLE_INFO  *pHdlInfo;

	DBG_IND("pPath = %s, Flag = 0x%x\r\n", pPath, Flag);

	pHdlInfo = NetFs_GetFreeHandle();
	if (pHdlInfo == NULL) {
		DBG_ERR("NetFs handles are all used\r\n");
		return NULL;
	}
	memset(pHdlInfo, 0x00, sizeof(NETFS_HANDLE_INFO));
	pHdlInfo->uiInitKey = CFG_NETFS_INIT_KEY;
	strncpy(pHdlInfo->path, pPath, sizeof(pHdlInfo->path) - 1);
	return (NET_FILE)pHdlInfo;
}

INT32     NetFs_CloseFile(NET_FILE pFile)
{
	NETFS_HANDLE_INFO  *pHdlInfo = (NETFS_HANDLE_INFO *)pFile;
	if (!NetFs_ChkValidHandle(pFile)) {
		DBG_ERR("Invalide pFile 0x%x\r\n", pFile);
		return NETFS_STA_PARAM_ERR;
	}
	pHdlInfo->uiInitKey = 0;
	return NETFS_STA_OK;
}

INT32     NetFs_SeekFile(NET_FILE pFile, UINT64 offset, FST_SEEK_CMD fromwhere)
{
	NETFS_HANDLE_INFO  *pHdlInfo = (NETFS_HANDLE_INFO *)pFile;
	if (!NetFs_ChkValidHandle(pFile)) {
		DBG_ERR("Invalide pFile 0x%x\r\n", pFile);
		return NETFS_STA_PARAM_ERR;
	}
	pHdlInfo->filePos = offset;
	return NETFS_STA_OK;
}
INT32     NetFs_ReadFile(NET_FILE pFile, UINT8 *pBuf,  UINT32 *pBufSize, UINT32 Flag, FileSys_CB CB)
{
	NETFS_HANDLE_INFO  *pHdlInfo = (NETFS_HANDLE_INFO *)pFile;
	CURLNVT_CMD curlCmd = {0};
	int         ret;
	char        url[256];


	DBG_IND("pFile = 0x%x, pBuf = 0x%x, pBufSize = 0x%x, Flag = 0x%x, CB = 0x%x\r\n", pFile, pBuf, *pBufSize, Flag, CB);
	if (!NetFs_ChkValidHandle(pFile)) {
		DBG_ERR("Invalide pFile 0x%x\r\n", pFile);
		return NETFS_STA_PARAM_ERR;
	}
	if (!pBuf) {
		DBG_ERR("pBuf 0x%x\r\n", pBuf);
		return NETFS_STA_PARAM_ERR;
	}
	if (!pBufSize) {
		DBG_ERR("pBufSize 0x%x\r\n", pBufSize);
		return NETFS_STA_PARAM_ERR;
	}
	if (!(*pBufSize)) {
		DBG_ERR("Read 0 bytes\r\n");
		return NETFS_STA_PARAM_ERR;
	}
	pHdlInfo->buffAddr = (UINT32)pBuf;
	pHdlInfo->buffSize = *pBufSize;
	if (Flag & FST_FLAG_ASYNC_MODE) {
		DBG_ERR("Not support async mode\r\n");
		return NETFS_STA_PARAM_ERR;
	}
	curlCmd.isWaitFinish = TRUE;
	pHdlInfo->CB = CB;
	pHdlInfo->read_offset = 0;
	pHdlInfo->write_offset = 0;
	pHdlInfo->rwCmd =  NETFS_CMD_READ;
	NetFs_filepath2url(pHdlInfo->path, url, sizeof(url));
	snprintf(pHdlInfo->curlcmd, sizeof(pHdlInfo->curlcmd), "curl -q --range %d-%d --output /sdcard/test2.bin --url %s", (int)pHdlInfo->filePos, (int)(pHdlInfo->filePos + pHdlInfo->buffSize - 1), url);
	curlCmd.strCmd = pHdlInfo->curlcmd;
	DBG_DUMP("curlCmd = %s\r\n", curlCmd.strCmd);
	curlCmd.cmdFinishCB = NetFs_CurlCmd_FinishCB;
	curlCmd.cmdWriteCB = NetFs_CurlCmd_WriteCB;
	curlCmd.cmdReadCB = NetFs_CurlCmd_ReadCB;
	curlCmd.userdata = (void *)pHdlInfo;
	ret = CurlNvt_Cmd(&curlCmd);
	DBG_IND("CurlNvt_Cmd ret = %d\r\n", ret);
	if (ret != CURL_ER_OK && (!pHdlInfo->isAbort)) {
		DBG_ERR("CurlNvt_Cmd ret = %d\r\n", ret);
		return NETFS_STA_ERROR;
	}
	return NETFS_STA_OK;
}

INT32     NetFs_WriteFile(NET_FILE pFile, UINT8 *pBuf,  UINT32 *pBufSize, UINT32 Flag, FileSys_CB CB)
{
	NETFS_HANDLE_INFO  *pHdlInfo = (NETFS_HANDLE_INFO *)pFile;
	CURLNVT_CMD curlCmd = {0};
	int         ret;
	char        url[256];
	char        ecosPath[128];

	DBG_IND("pFile = 0x%x, pBuf = 0x%x, pBufSize = 0x%x, Flag = 0x%x, CB = 0x%x\r\n", pFile, pBuf, *pBufSize, Flag, CB);
	if (!NetFs_ChkValidHandle(pFile)) {
		DBG_ERR("Invalide pFile 0x%x\r\n", pFile);
		return NETFS_STA_PARAM_ERR;
	}
	if (!pBuf) {
		DBG_ERR("pBuf 0x%x\r\n", pBuf);
		return NETFS_STA_PARAM_ERR;
	}
	if (!pBufSize) {
		DBG_ERR("pBufSize 0x%x\r\n", pBufSize);
		return NETFS_STA_PARAM_ERR;
	}
	if (!(*pBufSize)) {
		DBG_ERR("Read 0 bytes\r\n");
		return NETFS_STA_PARAM_ERR;
	}
	pHdlInfo->buffAddr = (UINT32)pBuf;
	pHdlInfo->buffSize = *pBufSize;
	if (Flag & FST_FLAG_ASYNC_MODE) {
		DBG_ERR("Not support async mode\r\n");
		return NETFS_STA_PARAM_ERR;
	}
	curlCmd.isWaitFinish = TRUE;
	pHdlInfo->CB = CB;
	pHdlInfo->read_offset = 0;
	pHdlInfo->write_offset = 0;
	pHdlInfo->rwCmd =  NETFS_CMD_WRITE;
	NetFs_Nvt2ecosPath("/sdcard/", pHdlInfo->uploadSrcPath, ecosPath, sizeof(ecosPath));
	NetFs_filepath2url(pHdlInfo->path, url, sizeof(url));
	snprintf(pHdlInfo->curlcmd, sizeof(pHdlInfo->curlcmd), "curl -q --range %d-%d --upload-file %s --url %s", (int)pHdlInfo->filePos, (int)(pHdlInfo->filePos + pHdlInfo->buffSize - 1), ecosPath, url);
	curlCmd.strCmd = pHdlInfo->curlcmd;
	DBG_DUMP("curlCmd = %s\r\n", curlCmd.strCmd);
	curlCmd.cmdFinishCB = NetFs_CurlCmd_FinishCB;
	curlCmd.cmdWriteCB = NetFs_CurlCmd_WriteCB;
	curlCmd.cmdReadCB = NetFs_CurlCmd_ReadCB;
	curlCmd.userdata = (void *)pHdlInfo;
	ret = CurlNvt_Cmd(&curlCmd);
	DBG_IND("CurlNvt_Cmd ret = %d\r\n", ret);
	if (ret != CURL_ER_OK && (!pHdlInfo->isAbort)) {
		DBG_ERR("CurlNvt_Cmd ret = %d\r\n", ret);
		return NETFS_STA_ERROR;
	}
	return NETFS_STA_OK;
}

UINT64    NetFs_TellFile(FST_FILE pFile)
{
	NETFS_HANDLE_INFO  *pHdlInfo = (NETFS_HANDLE_INFO *)pFile;
	CURLNVT_CMD         curlCmd = {0};
	int                 ret;
	char                url[256];

	if (!NetFs_ChkValidHandle(pFile)) {
		DBG_ERR("Invalide pFile 0x%x\r\n", pFile);
		return 0;
	}
	curlCmd.isWaitFinish = TRUE;
	pHdlInfo->rwCmd =  NETFS_CMD_TELL;
	NetFs_filepath2url(pHdlInfo->path, url, sizeof(url));
	snprintf(pHdlInfo->curlcmd, sizeof(pHdlInfo->curlcmd), "curl -q --head --url %s", url);
	curlCmd.strCmd = pHdlInfo->curlcmd;
	DBG_DUMP("curlCmd = %s\r\n", curlCmd.strCmd);
	curlCmd.cmdHeaderCB = NetFs_CurlCmd_HeaderCB;
	curlCmd.userdata = (void *)pHdlInfo;
	ret = CurlNvt_Cmd(&curlCmd);
	DBG_IND("CurlNvt_Cmd ret = %d\r\n", ret);
	if (ret != CURL_ER_OK) {
		DBG_ERR("CurlNvt_Cmd ret = %d\r\n", ret);
		return 0;
	}
	return pHdlInfo->fileSize;
}

INT32     NetFs_SetUploadSrcFilePath(FST_FILE pFile, char *pPath)
{
	NETFS_HANDLE_INFO  *pHdlInfo = (NETFS_HANDLE_INFO *)pFile;
	DBG_IND("pFile = 0x%x, pPath = %s, pBufSize = 0x%x, Flag = 0x%x, CB = 0x%x\r\n", pFile, pPath);
	if (!NetFs_ChkValidHandle(pFile)) {
		DBG_ERR("Invalide pFile 0x%x\r\n", pFile);
		return NETFS_STA_PARAM_ERR;
	}
	if (!pPath) {
		DBG_ERR("pPath is NULL\r\n");
		return NETFS_STA_PARAM_ERR;
	}
	strncpy(pHdlInfo->uploadSrcPath, pPath, sizeof(pHdlInfo->uploadSrcPath) - 1);
	return NETFS_STA_OK;
}
#endif

