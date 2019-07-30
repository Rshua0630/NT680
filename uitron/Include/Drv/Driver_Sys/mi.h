/**
    IDE Memory Interface header

    IDE Memory Interface header

    @file       mi.h
    @ingroup    mIDrvDisp_MI
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/


#ifndef _MI_H
#define _MI_H

#include "Type.h"

/**
    @addtogroup mIDrvDisp_MI
*/
//@{


/**
    memory interface mode

    memory interface mode, used for mi_setCtrl().
*/
typedef enum {
	PARALLEL_I80,       ///< Intel 8080 system interface
	PARALLEL_M68,       ///< Motorola 6800 system interface
	SERIAL_3WSPI,       ///< 3 wire serial interface with RS bit.
	SERIAL_4WSPI,       ///< 4 wire serial interface with RS pin.
	SERIAL_3WSPI_SB,    ///< 3 wire serial interface with start byte.

	ENUM_DUMMY4WORD(MI_TYPE)
} MI_TYPE;

/**
    memory interface input source

    memory interface input source, used for mi_setSrc().
*/
typedef enum {
	SRC_IDE,        ///< From IDE
	SRC_DRAM,       ///< From DRAM (not support)
	SRC_IDE2,       ///< From IDE2

	ENUM_DUMMY4WORD(MI_SRC)
} MI_SRC;


/**
    memory interface auto mode

    memory interface auto mode, used for mi_setCtrl().
*/
typedef enum {
	MI_AUTOFMT0,        ///< One frame only, auto clear MI enable after each frame.
	MI_AUTOFMT1,        ///< FRM1 + FRM2 + ...
	MI_AUTOFMT2,        ///< CMDs + FRM1 + FRM2 + ...
	MI_AUTOFMT3,        ///< CMDs + FRM1 + CMDs + FRM2 + ...

	ENUM_DUMMY4WORD(MI_AUTO_FMT)
} MI_AUTO_FMT;

/**
    memory interface auto dataformat.

    memory interface auto dataformat, used for mi_setCtrl().
*/
typedef enum {
	MI_DATAFMT0,        ///< 8bit/Serial - RGB444
	MI_DATAFMT1,        ///< 8bit/Serial - RGB565
	MI_DATAFMT2,        ///< 8bit/Serial - RGB565
	MI_DATAFMT3,        ///< 8bit/Serial - RGB666
	MI_DATAFMT4,        ///< 8bit/Serial - RGB666
	MI_DATAFMT5,        ///< 8bit/Serial - RGB666
	MI_DATAFMT6,        ///< 8bit/Serial - RGB675
	MI_DATAFMT7,        ///< 9bit - RGB666
	MI_DATAFMT8,        ///< 16bit - RGB565
	MI_DATAFMT9,        ///< 16bit - RGB666
	MI_DATAFMT10,       ///< 18bit - RGB666
	MI_DATAFMT11,       ///< 8bit - RGB888
	MI_DATAFMT12,       ///< 16bit - RGB888
	MI_DATAFMT13,       ///< 16bit - RGB666
	MI_DATAFMT14,       ///< 16bit - RGB666

	ENUM_DUMMY4WORD(MI_DATAFMT)
} MI_DATAFMT;

/**
    memory interface CMD type.

    memory interface CMD type, used for mi_setCmd*().
*/
typedef enum {
	WrCMD = 0,          ///< Write command
	WrDATA,             ///< Write data
	RdCMD,              ///< Read command
	RdDATA,             ///< Read data

	ENUM_DUMMY4WORD(MI_CMD_TYPE)
} MI_CMD_TYPE;

/**
    memory interface SCL mode.

    memory interface SCL mode, used for mi_setPolCtrl().
*/
typedef enum {
	SCLMOD0 = 0,        ///< Serial mode 0
	SCLMOD1,            ///< Serial mode 1
	SCLMOD2,            ///< Serial mode 2
	SCLMOD3,            ///< Serial mode 3

	ENUM_DUMMY4WORD(MI_SCLMODE)
} MI_SCLMODE;

/**
    memory interface timing config.

    memory interface timing config.
*/
typedef enum {
	MI_CONFIG_ID_FREQ,                   ///< MI module target clock (Unit: MHz).
	MI_CONFIG_ID_TCS,                    ///< MI tCS Timing
	MI_CONFIG_ID_TRS,                    ///< MI tRS Timing

	MI_CONFIG_ID_TRD_R,                  ///< MI tRD release period for RD signal
	MI_CONFIG_ID_TRD_A,                  ///< MI tRD active period for RD signal
	MI_CONFIG_ID_TWR_R,                  ///< MI tWR release period for WR signal
	MI_CONFIG_ID_TWR_A,                  ///< MI tWR active period for WR signal
	MI_CONFIG_ID_CMD_DELAY,              ///< MI tCMD delay
	MI_CONFIG_ID_CMD_PIXEL_DELAY_MIN,    ///< MI min. delay of CMD and pixel
	MI_CONFIG_ID_AUTO_FRAME_DELAY_MIN,   ///< MI min. delay between frames in auto mode.

	ENUM_DUMMY4WORD(MI_CONFIG_ID)
} MI_CONFIG_ID;


/**
    MI Timing configuration.

    MI Timing configuration, use for mi_setTiming().
*/
typedef struct {
	UINT32  uiTCS;       ///< tCS Timing
	UINT32  uiTRS;       ///< tRS Timing
	UINT32  uiTRD_R;     ///< tRD release period for RD signal
	UINT32  uiTRD_A;     ///< tRD active period for RD signal
	UINT32  uiTWR_R;     ///< tWR release period for WR signal
	UINT32  uiTWR_A;     ///< tWR active period for WR signal
	UINT32  uiCMD_DLY;   ///< tCMD delay
} MI_TIMING, *PMI_TIMING;


//-------------------------------------------------
// The general api for MI
//-------------------------------------------------
extern  ER          mi_attach(void);
extern  ER          mi_detach(void);
extern  ER          mi_open(void);
extern  ER          mi_close(void);
extern  BOOL        mi_isOpened(void);

extern  ER          mi_setEn(BOOL bEn);
extern  BOOL        mi_getEn(void);
extern  ER          mi_setCtrl(MI_TYPE Type, MI_AUTO_FMT AutoType,  MI_DATAFMT DataFMT,
							   BOOL bRGBSWAP, BOOL bBITSWAP, BOOL bSync, BOOL bPINMUX);
extern  ER          mi_getCtrl(MI_TYPE *Type, MI_AUTO_FMT *AutoType,  MI_DATAFMT *DataFMT,
							   BOOL *bRGBSWAP, BOOL *bBITSWAP, BOOL *bSync, BOOL *bPINMUX);

extern  MI_TYPE     mi_getType(void);
extern  ER          mi_setManual(BOOL bEn);
extern  MI_DATAFMT  mi_getDataFmt(void);
extern  ER          mi_setSyncTiming(UINT32 uiClkCnt, UINT8 uiLineCnt, BOOL bPolarity, BOOL bSyncSel);

extern  ER          mi_setCmds(UINT8 uiCmdCnt, UINT32 *Cmds, MI_CMD_TYPE *CmdType);
extern  ER          mi_setCmd1(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd2(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd3(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd4(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd5(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd6(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd7(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmd8(MI_CMD_TYPE CmdType, UINT32 Cmd);
extern  ER          mi_setCmdCnt(UINT8 uiCmdCnt);

extern  UINT32      mi_getReadBack(UINT8 CmdIdx);
extern  ER          mi_setPolCtrl(BOOL bCSINV, BOOL bRSINV, BOOL bRDINV, BOOL bWRINV, MI_SCLMODE bSCLMOD);
extern  ER          mi_setSerialCsCtrl(BOOL bSerCS);
extern  ER          mi_setCsAuto(BOOL bCsAuto);
extern  ER          mi_setCsLevel(BOOL bLevel);
extern  ER          mi_setIntEnable(BOOL bCmdDone, BOOL bFrmEnd, BOOL bFifoOver, BOOL bSyncErr);
extern  ER          mi_getIntEnable(BOOL *pbCmdDone, BOOL *pbFrmEnd, BOOL *pbFifoOver, BOOL *pbSyncErr);
extern  ER          mi_setSerialAutoRsBitValue(BOOL bVal);
extern  ER          mi_waitCmdDone(void);
extern  ER          mi_waitFrameEnd(void);


extern  void        mi_isr(void);
extern  ER          mi_setSrc(MI_SRC Src);
extern  ER          mi_getSrc(MI_SRC *Src);
extern  void        mi_setTiming(MI_TIMING mi_timing);

extern  ER          mi_setConfig(MI_CONFIG_ID ConfigID, UINT32 uiConfig);

extern  UINT32      mi_getConfig(MI_CONFIG_ID ConfigID);


//@}
#endif
