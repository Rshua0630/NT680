#ifndef _UISTORAGECHECK_H
#define _UISTORAGECHECK_H

// Storage Check
#define STORAGE_CHECK_FOLDER_FULL       0
#define STORAGE_CHECK_FULL              1
#define STORAGE_CHECK_LOCKED            2
#define STORAGE_CHECK_ERROR             3
#define STORAGE_CHECK_DCIM_READONLY     4
#define STORAGE_CHECK_CLUSTER           8
#define STORAGE_CHECK_SDCLASS4          0x10
#define STORAGE_CHECK_SDCLASS6          0x20
#define DIRID_MAX    0x00000001
#define FILEID_MAX   0x00000002

extern void   UIStorageCheck_AllSize(UINT64 *uiTotal, UINT64 *uiRemain);
extern void   UIStorageCheck_ResetFileReachMax(void);
extern BOOL   UIStorageCheck_CurrDirWritable(void);
extern BOOL   UIStorageCheck_DCIMWritable(void);
extern UINT32 UIStorageCheck_DirFileMax(void);
extern BOOL   UIStorageCheck(UINT32 uiCheckItem, UINT32 *pFreeCount);
#endif
