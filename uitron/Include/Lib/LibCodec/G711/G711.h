/**
    @file       G711.h
    @ingroup    mIAVG711

    @brief      Header file of G.711 u-law, A-law Encode/Decode API Library.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

/**
    @addtogroup mIAVG711
*/
//@{

#ifndef _G711_H
#define _G711_H

#include "Type.h"

/**
    G.711 u-Law encode function

    G.711 u-Law encode function.

    @param[in]  pInData         16-bit audio data input
    @param[out] pOutData        8-bit audio data output
    @param[in]  uiSampleCount   audio sample count
    @param[in]  bInSwap         input data swap or not

    @return error code
*/
extern ER G711_uLawEncode(INT16 *pInData, UINT8 *pOutData, UINT32 uiSampleCount, BOOL bInSwap);

/**
    G.711 u-Law decode function

    G.711 u-Law decode function.

    @param[in]  pInData         8-bit audio data input
    @param[out] pOutData        16-bit audio data output
    @param[in]  uiSampleCount   audio sample count
    @param[in]  bDupCh          duplicate channel or not
    @param[in]  bOutSwap        output data swap or not

    @return error code
*/
extern ER G711_uLawDecode(UINT8 *pInData, INT16 *pOutData, UINT32 uiSampleCount, BOOL bDupCh, BOOL bOutSwap);

/**
    G.711 A-Law encode function

    G.711 A-Law encode function.

    @param[in]  pInData         16-bit audio data input
    @param[out] pOutData        8-bit audio data output
    @param[in]  uiSampleCount   audio sample count
    @param[in]  bInSwap         input data swap or not

    @return error code
*/
extern ER G711_aLawEncode(INT16 *pInData, UINT8 *pOutData, UINT32 uiSampleCount, BOOL bInSwap);

/**
    G.711 A-Law decode function

    G.711 A-Law decode function.

    @param[in]  pInData         8-bit audio data input
    @param[out] pOutData        16-bit audio data output
    @param[in]  uiSampleCount   audio sample count
    @param[in]  bDupCh          duplicate channel or not
    @param[in]  bOutSwap        output data swap or not

    @return error code
*/
extern ER G711_aLawDecode(UINT8 *pInData, INT16 *pOutData, UINT32 uiSampleCount, BOOL bDupCh, BOOL bOutSwap);

//@}

#endif
