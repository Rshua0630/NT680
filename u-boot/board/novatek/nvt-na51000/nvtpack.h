#ifndef _NVTPACK_H
#define _NVTPACK_H

#define NVTPACK_CHKSUM_HDR_VERSION       0x16040719U
#define NVTPACK_FW_HDR2_VERSION   0x16071515U

typedef enum _NVTPACK_ER {
	NVTPACK_ER_SUCCESS = 0,
	NVTPACK_ER_FAILED = -1,
	NVTPACK_ER_UNKNOWN_FORMAT = -2,
	NVTPACK_ER_CHECK_SUM = -3,
	NVTPACK_ER_NOT_FOUND = -4,
	NVTPACK_ER_USER_BREAK = -5,
	NVTPACK_ER_PARAM = -6,
	NVTPACK_ER_MEM_NOT_ENOUGH = -7,
	NVTPACK_ER_UITRON_SIZE_ALIGN4 = -8, ///< uITRON size must align by 4
	NVTPACK_ER_INVALID_WHOLE_SIZE = -9, ///< after nvtpack, whole size must align by 4
	NVTPACK_ER_SRC_NAME_BLANK = -10, ///< resource's SrcName cannot be NULL or blank
	NVTPACK_ER_SRC_NAME_EXCEED = -11, ///< resource's SrcName cannot exceed 12 bytes
	NVTPACK_ER_SRC_NAME_INVALID = -12, ///< resource's SrcName include invalid character
	NVTPACK_ER_SRC_NAME_ALIKE = -13, ///< there resource's SrcName are alike.
} NVTPACK_ER;

typedef enum _NVTPACK_VER {
	NVTPACK_VER_UNKNOWN,   ///< UNKNOWN
	NVTPACK_VER_13012816,      ///< NT9666X, NT9850X
	NVTPACK_VER_16072017,      ///< NT9668X
} NVTPACK_VER;

#if !defined(_WIN32)
typedef struct _GUID {
	unsigned int   Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif

typedef struct _NVTPACK_RES_HDR {
	GUID         guid;          ///< {B5CB64AE-05DD-46FA-B906-0C62D22958D6}
	unsigned int TotalSize;
	unsigned int CheckSum;
	unsigned int ContentSize;
	char         SrcName[16];   ///< original packed bin file name
	char         DstName[16];   ///< target file name
} NVTPACK_RES_HDR;

typedef struct _NVTPACK_FW_HDR {
	GUID         guid;          ///< {8827BE90-36CD-4FC2-A987-73A8484E84B1}
	unsigned int TotalSize;     ///< sizeof(NVTPACK_FW_HDR) + n*sizeof(NVTPACK_PARTITION_HDR)
	unsigned int CheckSum;      ///< check sum of sizeof(NVTPACK_FW_HDR) + n*sizeof(NVTPACK_PARTITION_HDR)
	unsigned int TotalRes;      ///< total partitions
} NVTPACK_FW_HDR;

typedef struct _NVTPACK_FW_HDR2 {
	GUID         guid;          ///< {D6012E07-10BC-4F91-B28A-352F82261A50}
	unsigned int uiVersion;	    ///< NVTPACK_FW_HDR2_VERSION
	unsigned int uiHdrSize;     ///< sizeof(NVTPACK_FW_HDR2);
	unsigned int TotalRes;      ///< total resource counts
	unsigned int TotalSize;     ///< whole bin size
	unsigned int uiChkMethod;	///< 0: CheckSum
	unsigned int uiChkValue;	///< check sum value or crc value within NVTPACK_FW_HDR2
	unsigned int uiUserData[4]; ///< user defined data that may be used in future
	unsigned int uiReversed[18];///< align to 128 bytes
} NVTPACK_FW_HDR2;

typedef struct _NVTPACK_PARTITION_HDR {
	unsigned int Offset;
	unsigned int Size;
	unsigned int PartitionID;
} NVTPACK_PARTITION_HDR;

typedef struct _NVTPACK_CHKSUM_HDR {
	unsigned int uiFourCC ;	    ///< 'C','K','S','M'
	unsigned int uiVersion;	    ///< NVTPACK_CHKSUM_HDR header version
	unsigned int uiChkMethod;	///< 0: CheckSum
	unsigned int uiChkValue;	///< check sum value or crc value within NVTPACK_CHKSUM_HDR
	unsigned int uiDataOffset;  ///< real data starting offset without NVTPACK_CHKSUM_HDR
	unsigned int uiDataSize;    ///< real data size without NVTPACK_CHKSUM_HDR
	unsigned int uiPaddingSize; ///< padding bytes for check sum
	unsigned int uiEmbType;     ///< relate to EMBTYPE_?????
	unsigned int uiReversed[8]; ///< align to 64 bytes
} NVTPACK_CHKSUM_HDR;

typedef struct _NVTPACK_BFC_HDR {
	unsigned int uiFourCC;    ///< FourCC = BCL1
	unsigned int uiAlgorithm; ///< algorithm always is 9
	unsigned int uiSizeUnComp;///< big endian uncompressed size
	unsigned int uiSizeComp;  ///< big endian compressed size
} NVTPACK_BFC_HDR;

typedef struct _NVTPACK_BININFO_HDR {
	unsigned int CodeEntry;     ///< [0x00] fw CODE entry (4) ----- r by Ld
	unsigned int Resv1[19];     ///< [0x04~0x50] reserved (4*19) -- reserved, its mem value will filled by Ld
	char BinInfo_1[8];          ///< [0x50~0x58] CHIP-NAME (8) ---- r by Ep
	char BinInfo_2[8];          ///< [0x58~0x60] SDK version (8)
	char BinInfo_3[8];          ///< [0x60~0x68] SDK release-date (8)
	unsigned int BinLength;     ///< [0x68] Bin File Length (4) --- w by Ep/bfc
	unsigned int Checksum;      ///< [0x6c] Check Sum or CRC (4) ----- w by Ep/Epcrc
	unsigned int CRCLenCheck;   ///< [0x70~0x74] Length check for CRC (4) ----- w by Epcrc (total len ^ 0xAA55)
	unsigned int Resv2;         ///< [0x74~0x78] reserved (4) --- reserved for other bin tools
	unsigned int BinCtrl;       ///< [0x78~0x7C] Bin flag (4) --- w by bfc
	///< BIT 0.compressed enable (w by bfc)
	unsigned int CRCBinaryTag;  ///< [0x7C~0x80] Binary Tag for CRC (4) ----- w by Epcrc (0xAA55 + "NT")
} NVTPACK_BININFO_HDR;

typedef struct _NVTPACK_MEM {
	void *p_data;
	unsigned int len;
} NVTPACK_MEM;

typedef struct _NVTPACK_VERIFY_OUTPUT {
	NVTPACK_VER ver;            ///< indicate package type
	unsigned int partition_cnt; ///< total partition counts
} NVTPACK_VERIFY_OUTPUT;

typedef struct _NVTPACK_GET_PARTITION_INPUT {
	unsigned int id;
	NVTPACK_MEM  mem;
} NVTPACK_GET_PARTITION_INPUT;

typedef struct _NVTPACK_GET_RESOURCE_INPUT {
	const char *filename;
	NVTPACK_MEM mem;
} NVTPACK_GET_RESOURCE_INPUT;

typedef struct _NVTPACK_ENUM_PARTITION_INPUT {
	NVTPACK_MEM mem;
	void *p_user_data;   ///< [Optional] e.g. if use c class, you can pass that class point to your static callback.
	int (*fp_enum)(unsigned int id, NVTPACK_MEM *p_mem, void *p_user_data);
} NVTPACK_ENUM_PARTITION_INPUT;

typedef struct _NVTPACK_ENUM_RESOURCE_INPUT {
	NVTPACK_MEM mem;
	void *p_user_data;   ///< [Optional] e.g. if use c class, you can pass that class point to your static callback.
	int (*fp_enum)(NVTPACK_RES_HDR *p_hdr, NVTPACK_MEM *p_mem, void *p_user_data);
} NVTPACK_ENUM_RESOURCE_INPUT;

typedef struct _NVTPACK_PARTITION_ITEM {
	unsigned int id;
	NVTPACK_MEM mem;
} NVTPACK_PARTITION_ITEM;

typedef struct _NVTPACK_MAKE_PARTITION_INPUT {
	NVTPACK_VER ver;                    ///< package version
	NVTPACK_PARTITION_ITEM *p_list;     ///< data list
	int num;                            ///< data counts
	NVTPACK_MEM mem_work;               ///< working buffer
} NVTPACK_MAKE_PARTITION_INPUT;

typedef struct _NVTPACK_RESOURCE_ITEM {
	char SrcName[16];  ///< original name
	char DstName[16];  ///< target file name
	NVTPACK_MEM mem;
} NVTPACK_RESOURCE_ITEM;

typedef struct _NVTPACK_MAKE_RESOURCE_INPUT {
	NVTPACK_RESOURCE_ITEM *p_list;
	int num;
	NVTPACK_MEM mem_work;
} NVTPACK_MAKE_RESOURCE_INPUT;

#ifndef IsEqualGUID
#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))
#endif

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) | ((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24 ))
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int nvtpack_calc_nvt_sum(NVTPACK_MEM *p_mem);
extern NVTPACK_ER nvtpack_verify(NVTPACK_MEM *p_mem, NVTPACK_VERIFY_OUTPUT *p_output);
extern NVTPACK_ER nvtpack_get_partition(NVTPACK_GET_PARTITION_INPUT *p_input, NVTPACK_MEM *p_out);
extern NVTPACK_ER nvtpack_enum_partition(NVTPACK_ENUM_PARTITION_INPUT *p_input);
//NVTPACK_VER_13012816: need sizeof(NVTPACK_FW_HDR) +n*(sizeof(NVTPACK_PARTITION_HDR) + 4) + whole_data_size
//NVTPACK_VER_16072017: need sizeof(NVTPACK_FW_HDR2) +n*(sizeof(NVTPACK_PARTITION_HDR) + 4) + whole_data_size
extern NVTPACK_ER nvtpack_make_parition_pkg(NVTPACK_MAKE_PARTITION_INPUT *p_input, NVTPACK_MEM *p_out);

extern NVTPACK_ER nvtpack_get_resource(NVTPACK_GET_RESOURCE_INPUT *p_input, NVTPACK_MEM *p_out);
extern NVTPACK_ER nvtpack_enum_resource(NVTPACK_ENUM_RESOURCE_INPUT *p_input);
//buf need n*(sizeof(NVTPACK_RES_HDR)+32)+ whole_data_size
extern NVTPACK_ER nvtpack_make_resource_pkg(NVTPACK_MAKE_RESOURCE_INPUT *p_input, NVTPACK_MEM *p_out);

#ifdef __cplusplus
}
#endif

#endif