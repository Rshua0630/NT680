#ifndef _NETFS_H_
#define _NETFS_H_

#include "FileSysTsk.h"

typedef UINT32  *NET_FILE;

#define NETFS_STA_OK                  (0)                   ///<  the status is ok
#define NETFS_STA_ERROR               (-1)                  ///<  some error happened
#define NETFS_STA_PARAM_ERR           (-6)                  ///<  the input parameters are wrong



enum {
	NETFS_PARAM_REMOTE_IP      = 0x00000100,                     ///<  the IP address of remote host
	NETFS_PARAM_ROOT_DIR,                                        ///<  the root dir of netfs
	NETFS_PARAM_USE_SSL,                                         ///<  if use ssl (https)

};

INT32     NetFs_SetParam(UINT32 param, UINT32 value);
FST_FILE  NetFs_OpenFile(char *pPath, UINT32 Flag);
INT32     NetFs_CloseFile(FST_FILE pFile);
INT32     NetFs_SeekFile(FST_FILE pFile, UINT64 offset, FST_SEEK_CMD fromwhere);
INT32     NetFs_ReadFile(FST_FILE pFile, UINT8 *pBuf,  UINT32 *pBufSize, UINT32 Flag, FileSys_CB CB);
INT32     NetFs_WriteFile(FST_FILE pFile, UINT8 *pBuf, UINT32 *pBufSize, UINT32 Flag, FileSys_CB CB);
UINT64    NetFs_TellFile(FST_FILE pFile);
INT32     NetFs_SetUploadSrcFilePath(FST_FILE pFile, char *pPath);
#endif //_NETFS_H_
