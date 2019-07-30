#ifndef _NVTPACKINFO_H
#define _NVTPACKINFO_H

typedef struct _GUID {
    UINT32   Data1;
    UINT16   Data2;
    UINT16   Data3;
    UINT8    Data4[8];
} GUID;

typedef struct _HEADER_RES{
    GUID    guid;
    UINT32  TotalSize;
    UINT32  CheckSum;
    UINT32  ContentSize;
    char    SrcName[16];  ///< original packed bin file name
    char    FileName[16]; ///< target file name
}HEADER_RES;

typedef struct _HEADER_FW{
    GUID    guid;
    UINT32  TotalSize;
    UINT32  CheckSum;
    UINT32  TotalRes;
}HEADER_FW;

typedef struct _HEADER_PARTITION{
    UINT32  Offset;
    UINT32  Size;
    UINT32  PartitionID;
}HEADER_PARTITION;

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))

#endif