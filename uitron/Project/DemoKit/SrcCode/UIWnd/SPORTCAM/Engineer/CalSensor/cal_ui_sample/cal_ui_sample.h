#ifndef _CAL_UI_SAMPLE_INT_H_
#define _CAL_UI_SAMPLE_INT_H_
#include "Type.h"
#include "PrjCfg.h"
#if (CALIBRATION_FUNC == ENABLE)
#include "ipl_calibration.h"

/**
    calibration UI style
*/
typedef enum {
	CAL_UI_STYLE_NORMAL = 0,
	CAL_UI_STYLE_USB    = 1,
	CAL_UI_STYLE_WEB    = 2,
	CAL_UI_STYLE_MAX    = 3,
	ENUM_DUMMY4WORD(CAL_UI_STYLE)
} CAL_UI_STYLE;

/**
    calibration UI color
*/
typedef enum
{
    CAL_UICOL_NONE      = 0,    ///< -
    CAL_UICOL_RED       = 1,    ///< -
    CAL_UICOL_GREEN     = 2,    ///< -
    CAL_UICOL_BLUE      = 3,    ///< -
    CAL_UICOL_WHITE     = 4,    ///< -
    CAL_UICOL_BLACK     = 5,    ///< -
    CAL_UICOL_GRAY      = 6,    ///< -
    CAL_UICOL_YELLOW    = 7,    ///< -
    CAL_UICOL_MAX       = 8,    ///< -
    ENUM_DUMMY4WORD(CAL_UI_COLOR)
}CAL_UI_COLOR;

typedef void(*WAITKEY_FP)(void);
typedef struct {
	void	(*init)(CAL_UI_STYLE cal_ui_style);
	void	(*uninit)(void);

	void    (*ui_waitkey)(UINT32 key_mask, WAITKEY_FP waitkey_fp);
	void	(*ui_showline)(CHAR *p_str, CAL_UI_COLOR str_color, CAL_UI_COLOR bg_color);
	void	(*ui_deleteline)(void);
	void	(*ui_showstring)(UINT32 idx, CHAR *p_str, CAL_UI_COLOR str_color, PURECT p_rect, BOOL b_clear, BOOL show_rect);
	void	(*ui_cleanscreen)(void);
	void	(*ui_setbackground)(CAL_UI_COLOR bg_color);

	ISIZE 	(*get_dispsize)(void);

} CAL_UI_OBJ, *pCAL_UI_OBJ;

extern pCAL_UI_OBJ cal_ui(void);

#endif

#endif
