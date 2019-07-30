#ifndef _AF_UI_INFOR_H_
#define _AF_UI_INFOR_H_

#include "Type.h"
#include "ipl_utility.h"

#define AF_UI_INFOR_ERR   0xFFFFFFFF

typedef struct _AF_UI_INFO_TAB {
	UINT32 value;
	UINT32 max_value;
} AF_UI_INFO_TAB;

/**
    AF informatin selection index(af_set_ui_info() or af_get_ui_info())
*/
typedef enum _AF_UI_INFO {
	AF_UI_F,                        ///<AF mode, data type #AF_F
	AF_UI_WINDOW,                   ///<AF window selection, data type #AF_WINDOW
	AF_UI_POSITION,             	///<AF position, data type, #AF_POSITION	
	AF_UI_MAX_CNT,
	ENUM_DUMMY4WORD(AF_UI_INFO)
} AF_UI_INFO;

/**
    AF focus
*/
typedef enum _AF_F {
	AF_F_AUTO    ,        ///<Auto focus
	AF_F_MACRO,           ///<Marco mode
	AF_F_SUPER_MACRO,     ///<Super marco mode
	AF_F_INIFINITE,       ///<-
	AF_F_CONTINUEFOCUS,   ///<-
	AF_F_FACEDETECTION,   ///<-
	AF_F_MANUAL,          ///<-
	AF_F_MAX_CNT,
	ENUM_DUMMY4WORD(AF_F)
} AF_F;

/**
    AF window
*/
typedef enum _AF_WINDOW {
	AF_WINDOW_CENTER,            ///<Center
	AF_WINDOW_AUTO,              ///<Auto
	AF_WINDOW_MANUAL,            ///<Manual position
	AF_WINDOW_MAX_CNT,
	ENUM_DUMMY4WORD(AF_WINDOW)
} AF_WINDOW;

/**
    AF position
*/
typedef enum _AF_POSITION {
	AF_POSITION_1,            
	AF_POSITION_2,            
	AF_POSITION_3,            
	AF_POSITION_4,             
	AF_POSITION_5,              
	AF_POSITION_6,             
	AF_POSITION_7,              
	AF_POSITION_8,              
	AF_POSITION_9,               
	AF_POSITION_MAX_CNT,
	ENUM_DUMMY4WORD(AF_POSITION)
} AF_POSITION;


extern ER af_set_ui_info(IPL_PROC_ID id, AF_UI_INFO index, UINT32 value);
extern UINT32 af_get_ui_info(IPL_PROC_ID id, AF_UI_INFO index);

#endif
