#ifndef _UIAPPMOVIE_AUDNR_H
#define _UIAPPMOVIE_AUDNR_H

//
// Sepc. of Zoom Noise
//
extern UINT32           gAudNrZoomStopFreq;
extern UINT32           gAudNrZoomReplaceFreq;
extern UINT32           gAudNrZoomDecreaseLvl;
extern UINT32           gAudNrZoomMixBackgrd;
extern AUDNR_NOISE_SPEC gpAudNrNoiseSpec_Zoom;
//extern INT32  gAudNrZoomBandNum;
//extern INT32  gpAudNrZoomNoiseFreqBand[];
//extern INT64  gpAudNrZoomNoisePower[];
//extern INT32  gpAudNrZoomNoiseSpec[];

//
// Sepc. of Focus Noise
//
extern UINT32           gAudNrFocusStopFreq;
extern UINT32           gAudNrFocusReplaceFreq;
extern UINT32           gAudNrFocusDecreaseLvl;
extern UINT32           gAudNrFocusMixBackgrd;
extern AUDNR_NOISE_SPEC gpAudNrNoiseSpec_Focus;
//extern INT32  gAudNrFocusBandNum;
//extern INT32  gpAudNrFocusNoiseFreqBand[];
//extern INT64  gpAudNrFocusNoisePower[];
//extern INT32  gpAudNrFocusNoiseSpec[];

//
// Sepc. of Iris Noise
//
extern UINT32           gAudNrIrisStopFreq;
extern UINT32           gAudNrIrisReplaceFreq;
extern UINT32           gAudNrIrisDecreaseLvl;
extern UINT32           gAudNrIrisMixBackgrd;
extern AUDNR_NOISE_SPEC gpAudNrNoiseSpec_Iris;
//extern INT32  gAudNrIrisBandNum;
//extern INT32  gpAudNrIrisNoiseFreqBand[];
//extern INT64  gpAudNrIrisNoisePower[];
//extern INT32  gpAudNrIrisNoiseSpec[];

#endif
