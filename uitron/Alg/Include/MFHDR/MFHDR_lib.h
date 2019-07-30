#ifndef MFHDR_LIB_H
#define MFHDR_LIB_H

#include <Type.h>

//---------------------------------------------------------------------------------------------

/**
    HDR library data for external usage
*/
typedef struct _HDR_DATA_INFO {
	UINT32 uiInputDataAddr[6];              ///> start address of UV packed input, use cache address
	UINT32 uiWorkBufStartAddr;              ///> Start address of working buffer, use cache address
	UINT32 uiWorkBufSize;                   ///> working buffer size
	UINT32 uiWidth;                         ///> input image width
	UINT32 uiHeight;                        ///> input image height
	UINT32 uiEvGap;                         ///> EvGap if input images, 1 & 2 are available
} HDR_DATA_INFO;

/**
    Status of all HDR process
*/
typedef enum {
	MFHDR_SUCCESS = 0,                        ///> HDR process success
	MFHDR_FAIL = 1                            ///> HDR process fail
} HDR_STATUS;

//---------------------------------------------------------------------------------------------

/**
    HDR library API to do all HDR process.
    Library will proceed accord to the parameters provided
    in the HDR_DATA_INFO object.
*/
extern UINT32 MFHDR_DoAllProcess(HDR_DATA_INFO *pHdrParam);

/**
    HDR library API to assign output data address.
    Please give 2 addresses for separated Y & UV plane.
*/
extern VOID MFHDR_AssignYUVpOutAddr(UINT32 uiOutYAddr, UINT32 uiOutUVAddr);

#endif
