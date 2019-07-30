#ifndef _AWB_UI_INFOR_H_
#define _AWB_UI_INFOR_H_

#include "Type.h"
#include "ipl_utility.h"

#define AWB_UI_INFOR_ERR   0xFFFFFFFF

typedef struct _AWB_UI_INFO_TAB {
	UINT32 value;
	UINT32 max_value;
} AWB_UI_INFO_TAB;

/**
    AWB informatin selection index(awb_set_ui_info() or awb_get_ui_info())
*/
typedef enum _AWB_UI_INFO {
	AWB_UI_WB,                 	///<WB mode, data type #AWB_WB	
	AWB_UI_MAX_CNT,
	ENUM_DUMMY4WORD(AWB_UI_INFO)
} AWB_UI_INFO;

/**
    AWB scene mode
*/
typedef enum _AWB_UI_WB {
	AWB_WB_AUTO,            ///<Auto mode
	AWB_WB_DAYLIGHT,        ///<Daylight mode
	AWB_WB_CLOUDY,          ///<Cloudy mode
	AWB_WB_TUNGSTEN,        ///<Tugsten mode
	AWB_WB_SUNSET,          ///<Sunset mode
	AWB_WB_CUSTOMER1,       ///<-
	AWB_WB_CUSTOMER2,       ///<-
	AWB_WB_CUSTOMER3,       ///<-
	AWB_WB_CUSTOMER4,       ///<-
	AWB_WB_CUSTOMER5,       ///<-
	AWB_WB_CUSTOMER6,       ///<-
	AWB_WB_MGAIN,           ///<-Manually R/G/B gain
	AWB_WB_MAX_CNT,
	ENUM_DUMMY4WORD(AWB_WB)
} AWB_WB;


extern ER awb_set_ui_info(IPL_PROC_ID id, AWB_UI_INFO index, UINT32 value);
extern UINT32 awb_get_ui_info(IPL_PROC_ID id, AWB_UI_INFO index);

#endif
