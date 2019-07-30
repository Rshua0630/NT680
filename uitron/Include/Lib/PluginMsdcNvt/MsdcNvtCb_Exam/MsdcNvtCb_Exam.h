#ifndef _MSDCNVTCB_EXAM_H
#define _MSDCNVTCB_EXAM_H

#define MSDCNVTCBEXAM_MSG_SIZE 0x10000 //64KB, must be order 2^n

/**
     Msdc-Nvt File-Copy Initial Structure

     A structure for MsdcNvtCbFile_Init to initial this module
*/
typedef struct _MSDCNVTCBEXAM_INIT {
	UINT32 uiWorkingAddr; ///< Working Buffer Address
	UINT32 uiWorkingSize; ///< Working Buffer Size
} MSDCNVTCBEXAM_INIT, *PMSDCNVTCBEXAM_INIT;

//APIs for initial
extern void MsdcNvtCb_ExamInstallID(void) _SECTION(".kercfg_text");

//APIs for module use
extern INT32 MsdcNvtCb_ExamMsg(const char *fmtstr, va_list marker);
extern INT32 MsdcNvtCb_ExamSetName(const char *name); //set name to 'TARGET' or 'MONITOR'
extern void MsdcNvtCb_RegCmdFilterCB(BOOL (*fpCmdFilterCB)(CHAR *pcCmdStr, int len));

/**
     Msdc-Nvt Exam Initial

     This MsdcNvt Plug-in has to initial before using this module.

     @return Description of data returned.
         - @b TRUE: Success
         - @b FALSE: Failed to initial
*/
extern BOOL MsdcNvtCbExam_Init(MSDCNVTCBEXAM_INIT *pInit);

//Register to MsdcNvt
extern BOOL MsdcNvtRegBi_Exam(void);

#endif
