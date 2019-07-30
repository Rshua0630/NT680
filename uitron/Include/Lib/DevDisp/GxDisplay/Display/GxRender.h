#ifndef _GX_RENDER_H_
#define _GX_RENDER_H_

#include "GxCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------------------
// render
//--------------------------------------------------------------------------------------

typedef UINT32(*GXRENDER_INPUT_CB)(void);

void GxRender_SetInputCB(UINT32 LayerID, GXRENDER_INPUT_CB pfInputCB);
void GxRender_TriggerInput(UINT32 LayerID);
void GxRender_SetScale(int scale);          //set render scale
void GxRender_Reflash(void);               //render output to screen
UINT32 GxRender_GetRenderObject(void);


#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_RENDER_H_