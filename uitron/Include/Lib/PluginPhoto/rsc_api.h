/**
    Public header file for extend lib

    This file is the header file that define the API and data type for extend lib.

    @file       ipl_api.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _RSC_API_H
#define _RSC_API_H

//----------Header file include-------------------------------------------------
#include "rsc_obj.h"

/**
    extend rsc lib

    @param[in] id sensor id
    @param[in] rsc mode
    @param[out] rsc switch
    @return
        - @b 0 suceess, < 0 failed
 */
ER rsc_ext_setswitch(UINT32 id, SEL_RSC_MODECHG Value, SEL_RSC_STATUS Value2);
/**
    extend rsc lib

    @param[in] id sensor id
    @return
        - @b 0 suceess, < 0 failed
 */
SEL_RSC_STATUS rsc_ext_getenable(UINT32 id);
/**
    extend rsc lib

    @param[in] id sensor id
     @param[in] sensor mode
    @return
        - @b 0 suceess, < 0 failed
 */
UINT32 rsc_ext_geteisratio(UINT32 id,SEL_RSC_GETRATIO Ratio_Type,SENSOR_MODE nextsensoemode);
extern ER rsc_ext_init(UINT32 id);
extern ER rsc_ext_uninit(UINT32 id);
extern void Install_RSCAPIID(void) _SECTION(".kercfg_text");
//@}
#endif
