/**
    Playback Edit plugin module.

    All function for playback Edit, This module can be a plug-in to Application Playback.

    @file       PBXEdit.h
    @ingroup    mILibPbEditPlugIn

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _PBXEDIT_H
#define _PBXEDIT_H

#include "Type.h"
#include "GxImage.h"

/**
    @addtogroup mILibPbEditPlugIn
*/

/**
     Edit function parameter.

     For edit function apply, the user must input the parameters in this structure.
*/
typedef struct _PBX_EDIT_PARAM {
	PIMG_BUF         pSrcImg;        ///< [in]the source image of edit effect.
	UINT32           workBuff;       ///< [in]the working buffer address
	UINT32           workBufSize;    ///< [in]the working buffer size
	PIMG_BUF         pOutImg;        ///< [out]the output image of edit effect.
	UINT32           paramNum;       ///< [in]the extra parameter number of edit effect, detail please reference each edit function note.
	UINT32          *param;          ///< [in]the extra parameter array of edit effect, detail please reference each edit function note.
} PBX_EDIT_PARAM, *PPBX_EDIT_PARAM;

/**
     NovaLight level.

*/
typedef enum _PBXEDIT_NLIGHT_LEVEL_SEL {
	PBXEDIT_NLIGHT_LEVEL_1 = 0,       ///< level 1
	PBXEDIT_NLIGHT_LEVEL_2 = 1,       ///< level 2
	PBXEDIT_NLIGHT_LEVEL_3 = 2,       ///< level 3 (recommended)
	PBXEDIT_NLIGHT_LEVEL_4 = 3,       ///< level 4
	PBXEDIT_NLIGHT_LEVEL_5 = 4,       ///< level 5
	PBXEDIT_NLIGHT_LEVEL_6 = 5,       ///< level 6
	PBXEDIT_NLIGHT_LEVEL_7 = 6,       ///< level 7
	PBXEDIT_NLIGHT_LEVEL_8 = 7,       ///< level 8
	PBXEDIT_NLIGHT_LEVEL_MAX = 8,     ///< total level numbers
	ENUM_DUMMY4WORD(PBXEDIT_NLIGHT_LEVEL_SEL)
} PBXEDIT_NLIGHT_LEVEL_SEL;


/**
    Edit funcion proto type
*/
typedef ER(*PBX_EDIT_FUNC)(PPBX_EDIT_PARAM    p_pbxEditParam);


//typedef INT32 (*PBX_EDIT_CB)(INT32 thisCBvalue, INT32 totalCB);


/**
     Set Edit Effect function.

     This function will register the edit effect funtion for apply.

     @param[in]  p_pbxEditFunc       the edit function pointer.

     Example:
     @code
     {

        PBXEdit_SetFunc(EDIT_Resize);
     }
     @endcode
*/
extern void     PBXEdit_SetFunc(PBX_EDIT_FUNC p_pbxEditFunc);


/**
     Apply Edit Effect .

     This function will apply the edit effect that register in PBXEdit_SetFunc.

     @param[in]  p_pbxEditParam  the edit parameter is different for different edit effect, please refer each edit effect for detail.

     Example:
     @code
     {
        PBXEdit_SetFunc(EDIT_Resize);
        PBXEdit_Apply(&EditParam);
     }
     @endcode
*/
extern ER       PBXEdit_Apply(PPBX_EDIT_PARAM p_pbxEditParam);

/**
     Resize a Image.

     This function will resize a image to specified Width & Height,
     the output image lineoffset will be align 16.

     @param[in] p_pbxEditParam description.

     @note: the resized output image will be put in the work buff\n
            p_pbxEditParam->paramNum -> the parameter number should be 2.\n
            p_pbxEditParam->param[0] -> the resize image Width.\n
            p_pbxEditParam->param[1] -> the resize image Height.

     @return
         - @b E_PAR:     input parameter error
         - @b E_NOMEM:   input working buffer memory not enouth
         - @b E_OK:      result ok

     Example:
     @code
     {
        IMG_BUF DecodedBuf;
        IMG_BUF OutImg;
        UINT32  Param[2];

        EditParam.pSrcImg = &DecodedBuf;
        EditParam.pOutImg = &OutImg;
        EditParam.workBuff = g_uiTmpBuf;
        EditParam.workBufSize = guiTmpBufSize;
        EditParam.paramNum = 2;// should be 2
        Param[0] = 640; // width
        Param[1] = 480; // height
        EditParam.param = Param;
        // set the function pointer to EDIT_Resize
        PBXEdit_SetFunc(EDIT_Resize);
        // Run the resize function
        if (PBXEdit_Apply(&EditParam)!=E_OK)
        {
           // error handle
        }
     }
     @endcode
*/
extern ER    EDIT_Resize(PPBX_EDIT_PARAM p_pbxEditParam);

/**
     Crop a Image.

     This function will crop a specified region from the source image,\n
     the image crop region x,y,w,h should be 2 bytes align, \n
     the output image lineoffset will be align 16.

     @param[in] p_pbxEditParam description.

     @note: p_pbxEditParam->paramNum->the parameter number should be 1.\n
            p_pbxEditParam->param[0] type is (IRECT* pCropRegion), the crop region

     @return
         - @b E_PAR:     input parameter error
         - @b E_NOMEM:   input working buffer memory not enouth
         - @b E_OK:      result ok

     Example:
     @code
     {
        IMG_BUF DecodedBuf;
        IMG_BUF OutImg;
        UINT32  Param[1];
        IRECT   CropRegion = {0};

        EditParam.pSrcImg = &DecodedBuf;
        EditParam.pOutImg = &OutImg;
        EditParam.workBuff = tmpBuff;
        EditParam.workBufSize = WORKBUFF_SIZE;
        EditParam.paramNum = 1; // should be 1
        CropRegion.x = x;  // crop region x
        CropRegion.y = y;  // crop region y
        CropRegion.w = w;  // crop region width
        CropRegion.h = h;  // crop region height
        Param[0] = (UINT32)&CropRegion;
        EditParam.param = Param;
        // set the function pointer to EDIT_Crop
        PBXEdit_SetFunc(EDIT_Crop);
        // Run the crop function
        if (PBXEdit_Apply(&EditParam)!=E_OK)
        {
            // error handle
        }
     }
     @endcode
*/
extern ER    EDIT_Crop(PPBX_EDIT_PARAM p_pbxEditParam);


#endif

//@}
