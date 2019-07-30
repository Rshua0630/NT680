/*
    Panel device open/close Common control header

    @file       dispdev_panel.h
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _DISPDEV_PANLCOMN_H
#define _DISPDEV_PANLCOMN_H

#include "display.h"
#include "top.h"
#include "ide.h"
#include "dispdev_ioctrl.h"

// Default debug level
#ifndef __DBGLVL__
#define __DBGLVL__  1       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
#define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"

#define DISPANL_DEBUG  DISABLE
#if DISPANL_DEBUG
#define dispanl_debug(msg)       DBG_DUMP msg
#else
#define dispanl_debug(msg)
#endif



/*
    Structure for standard format of Panel Register Access
*/
typedef struct {
	UINT32  uiAddress;                      // Panel Register Address
	UINT32  uiValue;                        // Panel Register config value
} tPANEL_CMD;


/*
    Structure for standard format of panel open parameters
*/
typedef struct {
	/* Old prototype */

	PINMUX_LCDINIT  LCDMode;                // LCD Mode Select
	float           fDCLK;                  // LCD Target Clock Rate
											// ------------------------------------------
											// LCDMode				 |	panel clock 	|
											// ------------------------------------------
											// RGB Serial 8b/6b		 |	fDCLK*1			|
											// RGBD 8b/6b			 |	fDCLK*4			|
											// RGB through 8b/6b	 |	fDCLK*3			|
											// YUV 8b				 |	fDCLK*2			|
											// CCIR656 8b			 |	fDCLK*2			|
											// CCIR601 8b			 |	fDCLK*2			|
											// CCIR656 16b			 |	fDCLK*1			|
											// CCIR601 16b			 |	fDCLK*1			|
											// RGB888				 |	fDCLK*1			|
											// 16 bit RGB			 |	fDCLK*1			|
											// 16 bit RGB through	 |	fDCLK*1.5		|
											// 16 bit RGB(CLK 1/2_EN)|	fDCLK*0.5		|

	UINT32          uiHSyncTotalPeriod;     // Horizontal Total Period
	UINT32          uiHSyncActivePeriod;    // Horizontal Active Width
	UINT32          uiHSyncBackPorch;       // Horizontal Backporth

	UINT32          uiVSyncTotalPeriod;     // Vertical Total Period
	UINT32          uiVSyncActivePeriod;    // Vertical Active Width
	UINT32          uiVSyncBackPorchOdd;    // Vertical Odd Lines Backporch
	UINT32          uiVSyncBackPorchEven;   // Vertical Even Lines Backporch

	UINT32          uiBufferWidth;          // Buffer Width returned for upper layer
	UINT32          uiBufferHeight;         // Buffer Height returned for upper layer
	UINT32          uiWindowWidth;          // Window Width returned for upper layer
	UINT32          uiWindowHeight;         // Window Height returned for upper layer
	BOOL            bYCbCrFormat;           // TRUE(YUV): YUV/UPS051/UPS052,  FALSE(RGB): RGB/RGBD

	/* New added parameters */
	UINT32          uiHSyncSYNCwidth;       // Horizontal Sync pulse Width
	UINT32          uiVSyncSYNCwidth;       // Vertical Sync pulse Width

} tPANEL_PARAM;



/*
    Structure for standard format of IDE paramrters
*/
typedef struct {
	/* Old prototype */

	PINMUX_LCD_SEL  pinmux_select_lcd;  // LCD Pinmux select
	ICST_TYPE       icst;               // ICST Output Type
	BOOL            dithering[2];       // [0]: Dither Enable.   [1]: Dither FreeRun
	IDE_DEVICE_TYPE device;             // **DONT-CARE** Useless parameter in current version
	IDE_PDIR        pdir;               // RGB Pixel Output Direction
	IDE_PORDER      odd;                // LCD Odd Lines pixel Start from R or G or B.
	IDE_PORDER      even;               // LCD Odd Lines pixel Start from R or G or B.
	BOOL            hsinv;              // H-SYNC Invert
	BOOL            vsinv;              // V-SYNC Invert
	BOOL            hvldinv;            // H-Valid Invert
	BOOL            vvldinv;            // V-Valid Invert
	BOOL            clkinv;             // PIXEL CLOCK Invert
	BOOL            fieldinv;           // FIELD Invert
	BOOL            rgbdummy;           // **DONT-CARE** Useless parameter in current version
	BOOL            interlace;          // Output interlaced format
	BOOL            startfield;         // **DONT-CARE** Useless parameter in current version
	UINT32          ctrst;              // Contrast Value
	UINT32          brt;                // Brightness Value
	UINT32          cmults;             // Saturation Value
	BOOL            cex;                // Cb/Cr Exchane
	BOOL            standby_signal;     // **DONT-CARE** Useless parameter in current version
	BOOL            clamp;              // clamp
	BOOL            tv_powerdown;       // Assert TV power down in panel open
	UINT32          sif_startend[2];    // **DONT-CARE** Useless parameter in current version


	/* New added parameters */
	BOOL             YCex;               // Y/C exchange
	BOOL             HLPF;               // Horizontal lowpass filter
	BOOL             subpixOdd[3];       // Sub-Pixel ON/OFF for Odd lines. [0]:R   [1]:G   [2]:B
	BOOL             subpixEven[3];      // Sub-Pixel ON/OFF for Even lines.[0]:R   [1]:G   [2]:B
	IDE_DITHER_VBITS DitherBits[3];      // [0]:R dither bits. [1]:G dither bits. [2]:B dither bits. This is valid only if dithering[0]=TRUE.
	BOOL             clk1_2;             // clock 1/2
										 // Only use in RGB 16 bits output
										 // clock will be 1/2 and data will be 2x
} tIDE_PARAM;



typedef struct {
	tPANEL_PARAM Panel;         // Panel Related Paramters
	tIDE_PARAM   IDE;           // IDE Related Paramters
	tPANEL_CMD  *pCmdQueue;     // Panel Register Command Queue
	UINT32       nCmd;          // Panel Register Command Queue Number
} tLCD_PARAM;



typedef enum {
	tLCD_INF_SERIAL_8BITS,      // Such as RGB-Serial/RGB-Dummy/RGB-Through/YUV
	tLCD_INF_PARALLEL_16BITS,   // Such as Parallel RGB565 / YUV
	tLCD_INF_MI,                // Interface is Memory interface
	tLCD_INF_MIPI_DSI,

	ENUM_DUMMY4WORD(tLCD_INF)
} tLCD_INF;



typedef struct {
	DISPDEV_LCD_ROTATE  Rotation;            // LCD Roate ID
	tPANEL_CMD         *pCmdQueue;      // LCD Roate ID Command Queue
	UINT32              nCmd;           // LCD Roate ID Command Queue Number
} tLCD_ROT;

typedef struct {
	UINT32                  uiSIFMode;          ///< Bus mode. Please refer to Application Note for more detailed information.
	///< @note  Valid value: SIF_MODE_00 ~ SIF_MODE_15
	UINT32                  uiBusClock;         ///< Bus clock. Must be configured before SIF_CONFIG_ID_SEND, SIF_CONFIG_ID_SENS,
	///< SIF_CONFIG_ID_SENH and SIF_CONFIG_ID_DELAY.
	///< @note  Valid value: 187,500 ~ 48,000,000 , unit: Hz.
	///<        Real bus clock might lower than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_BUSCLOCK) to confirm.
	///<        Formula: Bus Clock = 96MHz / (2 * (CLKDIV + 1))
	UINT32                  uiSENS;             ///< SEN setup time for SIF_MODE_00 ~ SIF_MODE_07. Must be configured after SIF_CONFIG_ID_BUSCLOCK.
	///< Please refer to Application Note for more detailed information.
	///< @note  Valid value: (0 ~ 15) * half bus clock, unit: ns. Default: 0 ns.
	///<        Real SEN setup time might greater than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_SENS) to confirm.
	UINT32                  uiSENH;             ///< SEN hold time for all modes. Must be configured after SIF_CONFIG_ID_BUSCLOCK.
	///< Please refer to Application Note for more detailed information.
	///< @note  Valid value: (0 ~ 7) * half bus clcok, unit: ns. Default: 0 ns.
	///<        Real SEN hold time might greater than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_SENH) to confirm.
	UINT32                  uiTransferLength;   ///< How many bits will be transfered per transmission.
	///< @note  Valid value: 1 ~ 255
	SIF_DIR                 uiSIFDir;           ///< Shift out direction
	///< - @b SIF_DIR_LSB   : LSb is shifted out first (Defalut value)
	///< - @b SIF_DIR_MSB   : MSb is shifted out first
} tLCD_SIF_PARAM;




extern ER dispdev_setDisplay(DISPDEV_IOCTRL pDispDevControl, tLCD_INF LcdInf, tLCD_PARAM *pMode, PINMUX_FUNC_ID PinFuncID);

#endif
