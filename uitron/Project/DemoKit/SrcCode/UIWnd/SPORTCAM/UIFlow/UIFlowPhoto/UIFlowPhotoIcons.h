
#ifndef __UIFlowWndPhotoIcons_H
#define __UIFlowWndPhotoIcons_H

#include "Type.h"

extern void FlowPhoto_IconDrawMultiSnapMode(VControl *pCtrl);
extern void FlowPhoto_IconHideMultiSnapMode(VControl *pCtrl);
extern void FlowPhoto_IconDrawSize(VControl *pCtrl);
extern void FlowPhoto_IconHideSize(VControl *pCtrl);
extern void FlowPhoto_IconDrawQuality(VControl *pCtrl);
extern void FlowPhoto_IconHideQuality(VControl *pCtrl);
extern void FlowPhoto_IconDrawAntishake(VControl *pCtrl);
extern void FlowPhoto_IconHideAntishake(VControl *pCtrl);
extern void FlowPhoto_IconDrawFD(VControl *pCtrl);
extern void FlowPhoto_IconHideFD(VControl *pCtrl);
extern void FlowPhoto_IconDrawSD(VControl *pCtrl);
extern void FlowPhoto_IconHideSD(VControl *pCtrl);
extern void FlowPhoto_IconDrawMacro(VControl *pCtrl);
extern void FlowPhoto_IconHideMacro(VControl *pCtrl);
extern void FlowPhoto_IconDrawFreePicNumber(VControl *pCtrl);
extern void FlowPhoto_IconHideFreePicNumber(VControl *pCtrl);
extern void FlowPhoto_IconDrawScene(VControl *pCtrl);
extern void FlowPhoto_IconHideScene(VControl *pCtrl);
extern void FlowPhoto_IconDrawStorage(VControl *pCtrl);
extern void FlowPhoto_IconHideStorage(VControl *pCtrl);
extern void FlowPhoto_IconDrawVivilink(VControl *pCtrl);
extern void FlowPhoto_IconHideVivilink(VControl *pCtrl);
extern void FlowPhoto_IconDrawRamDisk(VControl *pCtrl);
extern void FlowPhoto_IconHideRamDisk(VControl *pCtrl);
extern void FlowPhoto_IconDrawSelftimer(VControl *pCtrl);
extern void FlowPhoto_IconHideSelftimer(VControl *pCtrl);
extern void FlowPhoto_IconDrawWB(VControl *pCtrl);
extern void FlowPhoto_IconHideWB(VControl *pCtrl);
extern void FlowPhoto_IconDrawSharpness(VControl *pCtrl);
extern void FlowPhoto_IconHideSharpness(VControl *pCtrl);
extern void FlowPhoto_IconDrawEV(VControl *pCtrl);
extern void FlowPhoto_IconHideEV(VControl *pCtrl);
extern void FlowPhoto_IconDrawISO(VControl *pCtrl);
extern void FlowPhoto_IconHideISO(VControl *pCtrl);
extern void FlowPhoto_IconDrawMetering(VControl *pCtrl);
extern void FlowPhoto_IconHideMetering(VControl *pCtrl);
extern void FlowPhoto_IconDrawBattery(VControl *pCtrl);
extern void FlowPhoto_IconHideBattery(VControl *pCtrl);

extern void FlowPhoto_IconDrawSelftimerTime(VControl *pCtrl, UINT32 time);
extern void FlowPhoto_IconHideSelftimerTime(VControl *pCtrl);
extern void FlowPhoto_IconDrawDZoom(VControl *pCtrl);
extern void FlowPhoto_IconHideDZoom(VControl *pCtrl);

extern void FlowPhoto_UpdateIcons(BOOL bShow);
#endif //__UIFlowWndPlayIcons_H
