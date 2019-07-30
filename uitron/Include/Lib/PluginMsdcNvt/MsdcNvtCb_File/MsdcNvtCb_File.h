/**
    Copy file from PC to DSC

    Copy a file from PC to DSC for MsdcNvt plugin

    @file       MsdcNvtCb_Disp.h
    @ingroup    mMSDCNVT

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _MSDCNVTCB_FILE_H
#define _MSDCNVTCB_FILE_H

/**
    @addtogroup mMSDCNVT
*/
//@{

/**
     Msdc-Nvt File-Copy Initial Structure

     A structure for MsdcNvtCbFile_Init to initial this module
*/
typedef struct _MSDCNVTCBFILE_INIT {
	UINT32 uiWorkingAddr; ///< Working Buffer Address
	UINT32 uiWorkingSize; ///< Working Buffer Size
} MSDCNVTCBFILE_INIT, *PMSDCNVTCBFILE_INIT;

/**
     Msdc-Nvt File-Copy Initial

     This MsdcNvt Plug-in has to initial before using this module.

     @return Description of data returned.
         - @b TRUE: Success
         - @b FALSE: Failed to initial
*/
extern BOOL MsdcNvtCbFile_Init(MSDCNVTCBFILE_INIT *pInit);

/**
     Msdc-Nvt File-Copy Module

     Plug-in MsdcNvt file copy.

     @return Description of data returned.
         - @b TRUE: Success
         - @b FALSE: Failed to plug-in
*/
extern BOOL MsdcNvtRegBi_File(void);

//@}
#endif