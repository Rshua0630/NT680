#ifndef EXAM_RESIZE_H
#define EXAM_RESIZE_H
#include <Type.h>

UINT32 ExamResize_CalBufSize(VOID);

UINT32 ExamResize_CalCacheBufSize(VOID);

BOOL ExamResize_Open(MEM_RANGE* buf, MEM_RANGE *cache);

BOOL ExamResize_Close(VOID);

#endif // EXAM_RESIZE_H
