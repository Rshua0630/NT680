#ifndef __DEFOG_LIB_H__
#define __DEFOG_LIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Type.h"
#include "ErrorNo.h"
#include "Debug.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    e_auto = 0,
    e_manual,
}E_DEFOG_LIB_MODE;

typedef struct {
    UINT8 auto_defog_lut[9]; // default {0  ,90 ,130,150,170,190,201,230,255,}
    UINT8 percentages;  //default  255
}S_DEFOG_LIB_INIT_PARA;

typedef struct {
    UINT32 *p_subImg; //pointer of min sub-image.
    UINT8 user_strength;//default 128, range [0~255]
	UINT8 blending_factor;//0~255. 255 : Repeat last strength; 0: No considering with history strength.
    //unsigned int y_gamma[33]; //Current RHE_YGAMMA[33]
}S_DEFOG_LIB_PARA;

//Set to defog reg.
typedef struct {
    UINT32 RHE_YGAMMA[33];
	UINT32 BLDRTO[9];
    UINT32 RHE_MIN_DIV;
	UINT32 RHE_MIN_CLAMP;
    UINT32 RHE_MAX_DIV;
	UINT32 RHE_MAX_CLAMP;
	UINT32 RHE_CTADJ_MIN_DIF;
	UINT32 REH_CTADJ_RTO;
	UINT32 RHE_CTADJ_GRATIO;
}S_DEFOG_LIB_RET;


//Defog API

extern ER defog_lib_get_subimg_size(UINT32 sie_id, UINT32 *ret_size);
extern ER defog_lib_init(UINT32 sie_id, S_DEFOG_LIB_INIT_PARA *para);
extern ER defog_lib_proc(UINT32 sie_id, S_DEFOG_LIB_PARA *para);
extern ER defog_lib_close(UINT32 sie_id);
extern ER defog_lib_get_para(UINT32 sie_id, S_DEFOG_LIB_RET *ret);


void chg_defog_percentages(UINT8 percen, UINT32 sie_id);
void chg_defog_mode(UINT32 mode, UINT32 sie_id);
void chg_defog_strength(UINT8 str, UINT32 sie_id);
void chg_defog_lut(UINT8 *pLut, UINT32 sie_id);






#ifdef __cplusplus
}
#endif

#endif //__DEFOG_LIB_H__
