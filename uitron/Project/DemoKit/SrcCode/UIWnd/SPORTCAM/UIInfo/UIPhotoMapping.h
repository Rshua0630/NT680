
#ifndef PHOTO_MAPPING_H
#define PHOTO_MAPPING_H
extern UINT32 GetPhotoSizeWidth(UINT32 ubIndex);
extern UINT32 GetPhotoSizeHeight(UINT32 ubIndex);
extern UINT32 GetPhotoSizeRatio(UINT32 ubIndex);
extern CHAR   *Get_DZoomRatioString(void);
extern UINT32 Get_AFWindowIndexFromValue(UINT32 value);
extern UINT32 Get_AFWindowValue(UINT32 uhIndex);
extern UINT32 Get_MacroValue(UINT32 uhIndex);
extern UINT32 Get_QualityValue(UINT32 uhIndex);
extern UINT32 Get_SceneModeValue(UINT32 uhIndex);
extern UINT32 Get_SceneModeIndexFromValue(UINT32 uhIndex);
extern UINT32 Get_EVIndexFromValue(UINT32 value);
extern UINT32 Get_EVValue(UINT32 uhIndex);
extern UINT32 Get_MeteringValue(UINT32 uhIndex);
extern UINT32 Get_WBIndexFromValue(UINT32 value);
extern UINT32 Get_WBValue(UINT32 uhIndex);
extern UINT32 Get_ISOValue(UINT32 uhIndex);
extern UINT32 Get_ColorValue(UINT32 uhIndex);
extern UINT32 Get_SaturationIndexFromValue(UINT32 value);
extern UINT32 Get_SharpnessIndexFromValue(UINT32 value);
extern UINT32 Get_SharpnessValue(UINT32 uhIndex);
extern UINT32 Get_FlashIndexFromValue(UINT32 value);
extern UINT32 Get_FlashValue(UINT32 uiIndex);
extern UINT32 Get_SceneModeFlashValue(UINT32 uhIndex);
extern UINT32 Get_SceneModeEVValue(UINT32 uhIndex);
extern UINT32 Get_CaptureModeValue(UINT32 uhIndex);
extern UINT32 Get_DatePrintValue(UINT32 value);
extern UINT32 Get_SharpnessValue(UINT32 value);
extern UINT32 Get_SaturationValue(UINT32 value);
extern UINT32 Get_ImageEffectValue(UINT32 value);
extern const CHAR *Get_SizeString(UINT32 uiIndex);
extern UINT32 Get_FDImageRatioValue(UINT32 value);
extern UINT32 Get_WDRValue(UINT32 uhIndex);
extern UINT32 Get_SHDRValue(UINT32 uhIndex);
extern UINT32 Get_AntishakeValue(UINT32 uhIndex);
extern UINT32 Get_DefogValue(UINT32 uhIndex);
#endif
