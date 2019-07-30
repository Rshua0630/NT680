/**
    CvUserDef library API.

    User-Defined CV functions API.

    @file       cvuser_lib.h
    @ingroup    CvUserDef
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _CVUSER_LIB_H
#define _CVUSER_LIB_H

#include "Type.h"
#include "cvuser_funs.h"

// Common types ----------------------------------------------------------------
/**
    Application preprocess callback

    @param[in] src_prms_addr: address of function parameters passed by the caller
    @param[in] dst_prms_addr: address of function parameters of IPC internal
*/
typedef VOID (*CVUSER_PREPROCESS_CB) (UINT32 src_prms_addr, UINT32 dst_prms_addr);

/**
    Application postprocess callback
*/
typedef VOID (*CVUSER_POSTPROCESS_CB)(UINT32 src_prms_addr, UINT32 dst_prms_addr);

/**
    Information of callback functions.
*/
typedef struct _CVUSER_CB_INFO {
	CVUSER_FUN fun;                         ///< function enum
	CVUSER_PREPROCESS_CB  preprocess_cb;    ///< preprocessing callback
	CVUSER_POSTPROCESS_CB postprocess_cb;   ///< postprocessing callback
} CVUSER_CB_INFO;


// Functions -------------------------------------------------------------------
/**
    Install required system resource.
*/
extern VOID cvuser_install_id(VOID) _SECTION(".kercfg_text");

/**
    Set preprocessing/postprocessing callback, which is called in cvuser_preprocess/cvuser_postprocess

    @param[in] p_cb: pointer of callback info

    @return the result of setting callback
*/
extern CVUSER_STA cvuser_set_process_cb(CVUSER_CB_INFO *p_cb);

/**
    Open CvUserDef

    @param[in] dsp_core: IPC destination DSP core

    @return the result of opening
*/
extern CVUSER_STA cvuser_open(CVUSER_SENDTO dsp_core);

/**
    Application preprocess

    @param[in] fun: function enum
    @param[in] prms_addr: address of function parameters
    @param[in] dsp_core: IPC destination DSP core

    @return the result of preprocessing
*/
extern CVUSER_STA cvuser_preprocess(CVUSER_FUN fun, UINT32 prms_addr, CVUSER_SENDTO dsp_core);

/**
    CvUserDef function process

    Note
    1. Both the input image(s) and the structure of function parameter(s) MUST be written back if they are in cache.
    2. Nested CvUserDef function execution is not allowed
    3. CvUserDef is non-blocking execution, thus user can do other things after an CvUserDef function is issued.

    @param[in] fun: function enum
    @param[in] prms_addr: address of function parameters
    @param[in] dsp_core: IPC destination DSP core

    @return the result of processing
*/
extern CVUSER_STA cvuser_process(CVUSER_FUN fun, UINT32 prms_addr, CVUSER_SENDTO dsp_core);

/**
    Wait CvUserDef processing done

    @param[in] dsp_core: IPC destination DSP core

    @return the result of waiting
*/
extern CVUSER_STA cvuser_wait_process_done(CVUSER_SENDTO dsp_core);

/**
    Application postprocess

    @param[in] fun: function enum
    @param[in] prms_addr: address of function parameters
    @param[in] dsp_core: IPC destination DSP core

    @return the result of postprocessing
*/
extern CVUSER_STA cvuser_postprocess(CVUSER_FUN fun, UINT32 prms_addr, CVUSER_SENDTO dsp_core);

/**
    Close CvUserDef

    @param[in] dsp_core: IPC destination DSP core

    @return the result of closing
*/
extern CVUSER_STA cvuser_close(CVUSER_SENDTO dsp_core);

#endif  //_CVUSER_LIB_H
