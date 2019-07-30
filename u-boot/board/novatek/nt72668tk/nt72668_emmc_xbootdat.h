/*
 *  board/novatek/nt72668tk/nt72668_emmc_xbootdat.h
 *
 *  Author:	Alvin lin
 *  Created:	Jun 5, 2015
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NT72668_EMMC_XBOOT_DAT_H__
#define __NT72668_EMMC_XBOOT_DAT_H__

#include <asm/types.h>

#define PANEL_PARAM_SZ 		512
#define PANEL_PARAM_VER 	1
#define PANEL_PARM_NUM 		10
#define PANEL_INDEX_SIGN 	"PIDX"
#define PANEL_PARAM_SIGN 	"PANL"

typedef enum {
	XDAT_HELP_DUMP,
	XDAT_HELP_SAVE,
	XDAT_HELP_ERASE,
	XDAT_HELP_MAX
} NVT_EMMC_XBOOTDAT_HELP_TYPE;

typedef struct _nvt_emmc_xbootdat_info {
	char *xdat_name;
	int (*xdathelp)(struct _nvt_emmc_xbootdat_info *this, NVT_EMMC_XBOOTDAT_HELP_TYPE help_type);
	int (*xdatdump)(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
	int (*xdatsave)(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
	int (*xdaterase)(struct _nvt_emmc_xbootdat_info *this, int argc, char* const argv[]);
	unsigned int start_sector;//count in sector
	unsigned int size;//count in sector
} nvt_emmc_xbootdat_info;

typedef struct _stMode {
	u16 u16Width;	
	u16 u16Height;
	u16 u16TypHTotal;
	u8 u8HSyncWidth;
	u16 u16HSyncBackPorch;
	u16 u16HSyncStart;
	u16 u16TypVTotal;
	u8 u8VSyncWidth;
	u16 u16VSyncBackPorch;
	u16 u16VSyncStart;
	u16 u16PLL;
} stMode;

typedef struct _panel_param {
	u8 sign[4];
	u32 version;
	u8 u8PanelID;
	u8 b8Mode50Hz;
	u8 u8Panel;
	u8 au8Name[16];
	u8 b8DualPixelOutput;
	u8 b8ABSwap;
	u8 b8ChannelSwap;
	u8 b8FrameSyncEnable;
	u8 u8FrameSyncMode;
	u8 u8MISC;
	u8 enLvdsFormat;
	u8 b8RBSwap;
	u8 u8DitheringBit;
	u16 u16LVDSPhase;
	u16 u16LVDSPhase_PortA;
	u16 u16LVDSPhase_PortB;
	u16 u16VFreqDiff_Min;
	u16 u16VFreqDiff_Max;
	u16 u16VTotalDiff_Min;
	u16 u16VTotalDiff_Max;
	stMode stMode50Hz;
	stMode stMode60Hz;
	u16 u16PanelPowerOnDelay1;
	u16 u16PanelPowerOnDelay2;
	u16 u16PanelPowerOFFDelay1;
	u16 u16PanelPowerOFFDelay2;
	u16 u16BackLightLevel;
	u8 b8PWMDutyInv;
	u8 b8HFlip;
	u8 b8VFlip;
	u8 b8OutputPanelFHD;
	u8 VX1;
	u8 FRC;
	u8 SEP;
	u8 b8Blinking;
	u8 u8PanelType;
	u8 b8ForcePanelTiming2D;
	u8 b8ForcePanelTiming2DSport;
	u8 b8ForcePanelTiming3D;
	u16 u16PanelTiming2D;
	u16 u16PanelTiming2DSport;
	u16 u16PanelTiming3D;
	u16 u16PWM2DSync;
	u16 u16PWM3DSync;
	u16 u16PDIMFreq2D50Hz;
	u16 u16PDIMFreq2D60Hz;
	u16 u16PDIMFreq2DSport50Hz;
	u16 u16PDIMFreq2DSport60Hz;
	u16 u16PDIMFreq3D50Hz;
	u16 u16PDIMFreq3D60Hz;
	u16 u16PDIMDelay2D50Hz;
	u16 u16PDIMDelay2D60Hz;
	u16 u16PDIMDelay2DSport50Hz;
	u16 u16PDIMDelay2DSport60Hz;
	u16 u16PDIMDelay3D50Hz;
	u16 u16PDIMDelay3D60Hz;
	u16 u16ADIMFreq2D50Hz;
	u16 u16ADIMFreq2D60Hz;
	u16 u16ADIMFreq2DSport50Hz;
	u16 u16ADIMFreq2DSport60Hz;
	u16 u16ADIMFreq3D50Hz;
	u16 u16ADIMFreq3D60Hz;
	u8 b8LocalDIMControl;
	u16 u16LRDelay3D;
	u16 u16BTDelay3D;
	u8 b8PDIMConByPanel3D;
	u8 u8Tcon;
	u8 u8TconType;
	u16 u16VX1PreEmphasisG1;
	u16 u16VX1PreEmphasisG2;
	u16 u16VX1PreEmphasisG3;
	u16 u16VX1SwingG1;
	u16 u16VX1SwingG2;
	u16 u16VX1SwingG3;
}panel_param;

typedef struct {
	panel_param p;
	u8 padding[PANEL_PARAM_SZ - sizeof(panel_param)];
} panel_param_blk;

typedef struct {
	u8 sign[4];
	u32 version;
	u32 index;
	u32 param_cnt;
} panel_index;

typedef struct {
	panel_index idx;
	u8 padding[PANEL_PARAM_SZ - sizeof(panel_index)];
} panel_index_blk;


void xbootdat_init(unsigned int xbootdat_start_offset);

#endif
