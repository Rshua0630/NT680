/**
    Flashlight Algorithm.

    @file       Flashlight_api.h
    @ingroup    mILibFlashLightAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _FLASHLIGHT_API_H_
#define _FLASHLIGHT_API_H_


typedef struct FLASH_PARAM_STRUCT {
	BOOL Enable;                ///< flashlight disable/enable
	UINT32 RedeyeEnable;        ///< Redeye removal disable/enable
	UINT32 FakeFlash;           ///< Fake flash in high brightness without mechanical shutter
	UINT32 WidthMainFlash;      ///< flashlight pulse width for capture
	UINT32 WidthPreFlash;       ///< flashlight pluse width for pre-flash
	UINT32 DelayLineMainFlash;  ///< Set delay line to trigger flash for capture
	UINT32 DelayLinePreFlash;   ///< Set delay line to trigger flash for pre-flash
	UINT32 YPreFlash;           ///< The Lum of pre-flash
	UINT32 YNoFlash;            ///< The Lum of non-flash
	UINT32 ISO_MainFlash;       ///< The ISO setting of capture
	UINT32 ExpoTime_MainFlash;  ///< The exposure time of capture
} FLASH_PARAM;



typedef enum {
	FLASH_HIGH_ACT,             ///< Trigger flashlight by high(rising) signal
	FLASH_LOW_ACT               ///< Trigger flashlight by low(falling) signal
} FLASH_POLARITY;

typedef enum {
	FLASH_TRI_MODE_NORMAL = 0,      ///< Trigger flashlight by delay line setting
	FLASH_TRI_MODE_GPIO_EDGE,       ///< Trigger flashlight by external gpio signal
	FLASH_TRI_MODE_IMD,             ///< Trigger flashlight Immediately
} FLASH_TRIGGER_MODE;

typedef struct FLASH_SETTING_STRUCT {
	BOOL TriggerEnable;                 ///< Flashlight trigger disable/enable
	FLASH_POLARITY TriggerPolarity;     ///< The polarity of trigger signal
	FLASH_TRIGGER_MODE TriggerMode;     ///< The trigger condition
	UINT32 DelayLine;                   ///< The delay line of trigger signal
	UINT32 DelayPixel;                  ///< The delay pixel of last line of trigger signal
	UINT32 FlashWidth;                  ///< The pulse width of trigger signal
	UINT32 Flash_Cap_ISO;               ///< The ISO setting of capture
	UINT32 Flash_Cap_ExpoTime;          ///< The exposure time of capture
	//UINT32 Flash_Cap_AWB_Rgain;         ///< The WBgain of capture
	//UINT32 Flash_Cap_AWB_Bgain;         ///< The WBgain of capture
} FLASH_INFOR;

typedef void (* FLASH_PARA_SET)(FLASH_INFOR *Data);

/**
    Red eye reduction flow, use preflash to avoid red eye.

    @param[in] FLASH_PARA_SET the function point to do preflash
    @return None
*/
extern void flash_RedEyeReduct(FLASH_PARA_SET FlashSetting_api);


/**
    Refer to UI & AE condition and decide enable/disable flashlight

    @return None
*/
extern void flash_Evaluate(void);


/**
    Pre-flash flow, use to calculate main flashlight

    @param[in] FLASH_PARA_SET the function point to do preflash
    @return FLASH_INFOR
        - @b main flashlight parameters
*/
extern FLASH_INFOR flash_PreFlashFlow(FLASH_PARA_SET FlashSetting_api);


/**
    Get flash status & parameters

    @return FLASH_PARAM
        - @b flashlight parameters
*/
extern FLASH_PARAM *GetFlashParam(void);


/**
    Get color gain of main flashlight

    @param[out] UINT32 R gain
    @param[out] UINT32 G gain
    @param[out] UINT32 B gain

    @return None
*/
extern void Flash_GetColorGian(UINT32 *Rg, UINT32 *Gg, UINT32 *Bg);


/**
    Used to measure flashlight "distance-pulse width" table

    @return None
*/
extern void flash_PreFlashFlow_Test(void);

extern UINT32 Flash_WriteDebugInfo(UINT32 Addr);

#endif
