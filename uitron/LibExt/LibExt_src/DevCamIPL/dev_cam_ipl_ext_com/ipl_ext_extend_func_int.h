/**
    ipl_ext_extend_func_int.h


    @file       ipl_extend_func_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef IPL_EXT_EXTEND_FUNC_INT_H_
#define IPL_EXT_EXTEND_FUNC_INT_H_

typedef struct WDR_INTERVAL_PARAM {
	UINT32 VideoFps;
	UINT32 TrigIntervel;
} WDR_INTERVAL_PARAM;

#define DEFAULT_TRIG_FPS 3	//default wdr trigger fps

/**
    IPL ExtFunc mode

    Extfunc main function.
*/
typedef enum _EXTFUNC_MODE {
    EXTFUNC_MODE_DR = 0,
    EXTFUNC_MODE_DEFOG,
    EXTFUNC_MODE_MAX,
    ENUM_DUMMY4WORD(EXTFUNC_MODE)
} EXTFUNC_MODE;

/**
    IPL ExtFunc DR mode

    ExtFunc DR sub-function.
*/
typedef enum _EXTFUNC_DR {
    EXTFUNC_DR_NONE = 0,        // Linear sensor, without  post-process
    EXTFUNC_DR_SHDR_OFF,        // HDR    sensor, with HDR post-process
    EXTFUNC_DR_SHDR_ON,         // HDR    sensor, with HDR post-process
    EXTFUNC_DR_SHDR_WDR,        // HDR    sensor, with WDR post-process
    EXTFUNC_DR_WDR_OFF,         // Linear sensor, with WDR post-process
    EXTFUNC_DR_WDR_ON,          // Linear sensor, with WDR post-process
    ENUM_DUMMY4WORD(EXTFUNC_DR)
} EXTFUNC_DR;

/**
    IPL ExtFunc Defog mode

    ExtFunc Defog sub-function.
*/
typedef enum _EXTFUNC_DEFOG {
	EXTFUNC_DEFOG_NONE = 0,
    EXTFUNC_DEFOG_OFF,
    EXTFUNC_DEFOG_ON,
    ENUM_DUMMY4WORD(EXTFUNC_DEFOG)
} EXTFUNC_DEFOG;

/**
    IPL ExtFunc Stage

    Preventing racing issue when switch between different states.
*/
typedef enum _EXTFUNC_STAGE {
    EXTFUNC_STAGE_N = 0,

    EXTFUNC_STAGE_1,
    EXTFUNC_STAGE_2,
    EXTFUNC_STAGE_3,
    EXTFUNC_STAGE_4,
    EXTFUNC_STAGE_5,

    EXTFUNC_STAGE_ERR,

    EXTFUNC_STAGE_MAX,

    ENUM_DUMMY4WORD(EXTFUNC_STAGE)
} EXTFUNC_STAGE;


/**
    IPL ExtFunc Init

    Call initiallization at the beginning.
*/
void ExtFunc_Init(UINT32 Id);

/**
    IPL ExtFunc Un-init

    Call un-initiallization before next initiallization.
*/
void ExtFunc_UnInit(UINT32 Id);

/**
    IPL ExtFunc Change mode

    Switch different function modes.
    isInstant -
    	FALSE:	Delay runtime change HDR mode until next ExtFunc_Process() triggered
    	TRUE:	Runtime change RHE mode immediately
*/
void ExtFunc_ChgMode(UINT32 Id, BOOL isInstant);

/**
    IPL ExtFunc Process

    Trigger ExtFunc process.
*/
void ExtFunc_Process(UINT32 Id);

#endif //IPL_EXT_EXTEND_FUNC_INT_H_
