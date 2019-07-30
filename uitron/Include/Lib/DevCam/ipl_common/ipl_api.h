/**
    Public header file for extend lib

    This file is the header file that define the API and data type for extend lib.

    @file       ipl_api.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPL_API_H
#define _IPL_API_H

//----------Header file include-------------------------------------------------
#include "ipl_cmd.h"
#include "ipl_obj.h"

/**
    General extend lib APIs
*/
/**
    extend lib change mode

    @param[in] ipl current mode
    @param[in] ipl next mode
    @return
        - @b 0 suceess, < 0 failed
 */
extern IPL_CMD_CHGMODE_FP ipl_ext_change_mode(UINT32 id, UINT32 cur_ipl_mode, UINT32 next_ipl_mode);

/**
    extend lib

    @param[in] id sensor id
    @param[in] sensor mode
    @param[in] sensor info
    @param[in] vdo out info
    @param[out] dzoom table maximum index
    @return
        - @b 0 suceess, < 0 failed
 */
extern UINT32 *ipl_ext_senmode2tbl(UINT32 id,UINT32 *dz_max_idx);

/**
    extend lib

    @param[in] id sensor id
    @param[in] ipl mode
    @param[out] sensor info
    @return
        - @b 0 suceess, < 0 failed
 */
extern ER ipl_ext_get_sensor_info(UINT32 id, UINT32 ipl_mode, SENSOR_INFO *sen_info);

extern ER ipl_ext_init(UINT32 id);
extern ER ipl_ext_uninit(UINT32 id);

//@}
#endif
