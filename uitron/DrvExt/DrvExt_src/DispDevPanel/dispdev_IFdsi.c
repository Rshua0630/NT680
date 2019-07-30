/*
    Display device object Golden Sample for driving Memory interface panel

    @file       dispdev_gsinfmi.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "kernel.h"
#include "ide.h"
#include "top.h"
#include "gpio.h"
#include "pll.h"
#include "dsi.h"
#include "Utility.h"
#include "display.h"
#include "dispdev_IFdsi.h"
#include "dispdev_panlcomn.h"

#define PORCH_ALIGN  20
static BOOL ubTEON = FALSE;
static BOOL ubTEExtPin = FALSE;

static void     DispDeviceInit(tLCD_PARAM *pMode);

static ER       dispdev_openIFDsi(void);
static ER       dispdev_closeIFDsi(void);
static ER       dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot);
static void     dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue);
static UINT32   dispdev_readLcdReg(UINT32 uiAddr);
static void     dispdev_setIFDsiIOCTRL(FP pIoCtrlFunc);
static ER       dispdev_getLcdDsiSize(PDISPDEV_GET_PRESIZE tLCDSize);

#define MAX_DSI_CMD         200
#define IDE_SRC_CLK_480     5242880
static UINT8    dsiCmdParam[MAX_DSI_CMD];

#define LOG_DSI_CFG_MSG DISABLE

#if LOG_DSI_CFG_MSG == ENABLE
#define DSI_CFG_MSG(...)      DBG_IND(__VA_ARGS__)
#define DSI_ERR_MSG(...)      DBG_ERR(__VA_ARGS__)
#define DSI_IND_MSG(...)      DBG_IND(__VA_ARGS__)
#define DSI_CMD_MSG(...)
#else
#define DSI_CFG_MSG(...)
#define DSI_ERR_MSG(...)
#define DSI_IND_MSG(...)
#define DSI_CMD_MSG(...)
#endif

/*
    Exporting display device object
*/
#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
static DISPDEV_OBJ     DispDev_IFDsiObj_Lcd1 = { dispdev_openIFDsi, dispdev_closeIFDsi, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdDsiSize, dispdev_setIFDsiIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIFDsiIoControl1 = NULL;
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
static DISPDEV_OBJ     DispDev_IFDsiObj_Lcd2 = { dispdev_openIFDsi, dispdev_closeIFDsi, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_getLcdDsiSize, dispdev_setIFDsiIOCTRL, {NULL, NULL}};
static volatile DISPDEV_IOCTRL  pIFDsiIoControl2 = NULL;
#endif

static UINT32   gDispDev_SrcFreq = 0;

#define _CFG_USE_LP_ 1
#if _CFG_USE_LP_
static UINT8 testPat2[210];
static UINT32 EccPoly[6] = {0xF12CB7, 0xF2555B, 0x749A6D, 0xB8E38E, 0xDF03F0, 0xEFFC00};

static UINT32 dsi_GenECC(UINT32 uiPktHdr)
{
	UINT32  CsiPktHdr;
	UINT32  i, j;
	BOOL    tmp;

	CsiPktHdr = 0;

	uiPktHdr &= 0xFFFFFF;


	for (j = 0; j < 6; j++) {
		tmp = 0;
		for (i = 0; i < 24; i++) {
			tmp ^= ((uiPktHdr >> i) & 0x1 & (EccPoly[j] >> i));
		}
#if 1
		CsiPktHdr |= (tmp << (/*24+*/j));
#else
		CsiPktHdr |= (tmp << (31 - j));
#endif
	}

	//emu_msg(("0x%08X\r\n",CsiPktHdr));
	return CsiPktHdr;
}

#define CSI_CRCPOLY 0x8408
static UINT16 dsi_GenCRC(char *data_p, UINT16 length)
{
	unsigned char   i;
	unsigned int    data;
	unsigned int    crc = 0xffff;

	if (length == 0) {
		return (UINT16)(crc);
	}

	do {
		for (i = 0, data = (unsigned int)0xff & *data_p++; i < 8; i++, data >>= 1) {
			if ((crc & 0x0001) ^ (data & 0x0001)) {
				crc = (crc >> 1) ^ CSI_CRCPOLY;
			} else {
				crc >>= 1;
			}
		}

	} while (--length);

	return (UINT16)((crc) & 0xFFFF);

}


static ER dsi_LPWriteCmd(DSI_LANESEL uiDataLane, UINT32 uiDCSCmd, UINT8 *param, UINT32 paramNo, DSI_PACKET_TYPE pktType, BOOL bEOT)
{
	UINT32 uiPKTHD = 0;
	//UINT32 uiPacketHeader;
	UINT32 uiTransData;
	UINT32 uiParamCnt;
	UINT32 uiECCCode;

	DBG_DUMP("[%d]Low power Cmd => 0x%02x\r\n", pktType, uiDCSCmd);
	if (pktType == DSI_SHORT_PACKET) {
		if (paramNo >= 2) {
			DBG_ERR("Low power DT param exceed 1 => %d\r\n", paramNo);
			return E_SYS;
		}

		if (paramNo == 0) {
			uiPKTHD = DATA_TYPE_SHORT_WRITE_NO_PARAM;
		} else {
			uiPKTHD = DATA_TYPE_SHORT_WRITE_1_PARAM;
		}

#if (DISPDEV_IFDSI_TYPE==DISPDEV_IFDSI_NT35410)
		if (uiDCSCmd == 0x32) {
			uiPKTHD = 0x32;
		}
#elif (DISPDEV_IFDSI_TYPE==DISPDEV_IFDSI_NT35510)
		//if(uiDCSCmd == 0x35 && ubTEON == FALSE)
		//    uiPKTHD = 0x32;
#endif
		uiPKTHD |= (uiDCSCmd << 8);
		for (uiParamCnt = 0; uiParamCnt < paramNo; uiParamCnt++) {
			uiPKTHD |= (param[uiParamCnt] << 16);
		}
		//DSI_IND_MSG("emu_dsiLPWriteCmd = 0x%08x\r\n", uiPKTHD);

		uiECCCode = (dsi_GenECC(uiPKTHD) & 0xFF);

		//EMU_DSI_LOG_MSG("emu_dsiLPWriteCmd uiECCCode = 0x%08x\r\n", uiECCCode);

		uiPKTHD |= (uiECCCode << 24);

		DSI_IND_MSG("emu_dsiLPWriteCmd = 0x%08x\r\n", uiPKTHD);

		dsi_setEscapeEntry(uiDataLane, (UINT32)ULPS_ENTRY_CMD_LPDT, FALSE);

		for (uiParamCnt = 0; uiParamCnt < 3; uiParamCnt++) {
			uiTransData = ((uiPKTHD >> (uiParamCnt * 8)) & 0xFF);
			dsi_setEscapeTransmission(uiDataLane, uiTransData, DSI_SET_ESC_NOT_STOP);
		}
		uiTransData = ((uiPKTHD >> 24) & 0xFF);

		if (bEOT == FALSE) {
			dsi_setEscapeTransmission(uiDataLane, uiTransData, DSI_SET_ESC_STOP_WITH_ESC_CMD);
		} else {
			dsi_setEscapeTransmission(uiDataLane, uiTransData, DSI_SET_ESC_NOT_STOP);
			//keep sending EoT packet
			dsi_setEscapeTransmission(uiDataLane, 0x08, DSI_SET_ESC_NOT_STOP);
			dsi_setEscapeTransmission(uiDataLane, 0x0F, DSI_SET_ESC_NOT_STOP);
			dsi_setEscapeTransmission(uiDataLane, 0x0F, DSI_SET_ESC_NOT_STOP);
			uiPKTHD = 0x000F0F08;
			uiECCCode = (dsi_GenECC(uiPKTHD) & 0xFF);
			//debug_msg("EoT ECC = 0x%02x\r\n", uiECCCode);
			dsi_setEscapeTransmission(uiDataLane, uiECCCode, DSI_SET_ESC_STOP_WITH_ESC_CMD);
		}
	} else {
		uiPKTHD = DATA_TYPE_LONG_WRITE;
		uiPKTHD |= ((paramNo + 1) << 8);
		uiECCCode = (dsi_GenECC(uiPKTHD) & 0xFF);

		//EMU_DSI_LOG_MSG("emu_dsiLPWriteCmd uiECCCode = 0x%08x\r\n", uiECCCode);

		uiPKTHD |= (uiECCCode << 24);

		DSI_IND_MSG("emu_dsiLPWriteCmd Packet header = 0x%08x\r\n", uiPKTHD);

		DSI_IND_MSG("   [ DI] [0x%02x]\r\n", (uiPKTHD & 0xFF));
		DSI_IND_MSG("   [WC0] [0x%02x]\r\n", ((uiPKTHD & 0xFF00) >> 8));
		DSI_IND_MSG("   [WC1] [0x%02x]\r\n", ((uiPKTHD & 0xFF0000) >> 16));
		DSI_IND_MSG("   [ECC] [0x%02x]\r\n", ((uiPKTHD & 0xFF000000) >> 24));
		dsi_setEscapeEntry(uiDataLane, (UINT32)ULPS_ENTRY_CMD_LPDT, FALSE);

		//1. Packet Header
		//("Start send long packet via Low power => ");
		for (uiParamCnt = 0; uiParamCnt <= 3; uiParamCnt++) {
			uiTransData = ((uiPKTHD >> (uiParamCnt * 8)) & 0xFF);
			dsi_setEscapeTransmission(uiDataLane, uiTransData, DSI_SET_ESC_NOT_STOP);
			//EMU_DSI_LOG_MSG("[0x%02x] ", uiTransData);
		}

		//2. DCS command always be 1st byte of packet data in long packet
		dsi_setEscapeTransmission(uiDataLane, uiDCSCmd, DSI_SET_ESC_NOT_STOP);

		DSI_IND_MSG(" [Data00] : [0x%02x]\r\n", uiDCSCmd);

		testPat2[0] = uiDCSCmd;

		//3. Send remain data 1 ~ n
		for (uiParamCnt = 0; uiParamCnt < paramNo; uiParamCnt++) {
			dsi_setEscapeTransmission(uiDataLane, param[uiParamCnt], DSI_SET_ESC_NOT_STOP);
			DSI_IND_MSG(" [Data%02d] : [0x%02x]\r\n", uiParamCnt + 1, param[uiParamCnt]);
			testPat2[uiParamCnt + 1] = param[uiParamCnt];
		}

		//4. Final generate 2 bytes Checksum data
		uiTransData = dsi_GenCRC((CHAR *)&testPat2[0], (UINT16)(paramNo + 1));

		DSI_IND_MSG("    CRC   : [0x%04x]\r\n", uiTransData);

		//debug_msg("Generate CRC = 0x%04x\r\n", uiTransData);

		dsi_setEscapeTransmission(uiDataLane, (uiTransData & 0xFF), DSI_SET_ESC_NOT_STOP);

		dsi_setEscapeTransmission(uiDataLane, ((uiTransData >> 8) & 0xFF), DSI_SET_ESC_STOP_WITH_ESC_CMD);
	}

	return E_OK;
}
#endif

static void DispDeviceInit(tLCD_PARAM *pMode)
{
	DSI_CMD_RW_CTRL_PARAM   dcsContext;
	DSI_CMD_CTRL_PARAM      dcsCtrlContext;
	UINT32                  i, dataCnt;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl1;
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl2;
#endif
	BOOL                    bEoPT = TRUE;
	ubTEON = FALSE;
	ubTEExtPin = FALSE;
	dcsContext.ubBTAEN = FALSE;
	dcsContext.ubBTAOnly = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_GET_PANEL_ADJUST, &DevIOCtrl);

	dcsCtrlContext.uiParamCnt = 0;

	if (DevIOCtrl.SEL.GET_PANEL_ADJUST.pfpAdjust != NULL) {
		DevIOCtrl.SEL.GET_PANEL_ADJUST.pfpAdjust();
	} else if (pMode->pCmdQueue != NULL) {
		for (i = 0; i < pMode->nCmd;) {
			if (pMode->pCmdQueue[i].uiAddress == CMDDELAY_MS) {
				Delay_DelayMs(pMode->pCmdQueue[i].uiValue);
				i++;
			} else if (pMode->pCmdQueue[i].uiAddress == CMDDELAY_US) {
				Delay_DelayUs(pMode->pCmdQueue[i].uiValue);
				i++;
			} else if (pMode->pCmdQueue[i].uiAddress == CMDBTA) {
				DSI_CFG_MSG(" Issue BTA\r\n");
				dcsContext.ubBTAEN = TRUE;
				dcsContext.ubBTAOnly = TRUE;
				dcsContext.uiCmdNo = 1;
				dcsContext.uiSramOfs = 0x80;
				dcsContext.ubEOTEN = FALSE;

				dcsCtrlContext.uiDataType = DATA_TYPE_SHORT_READ_NO_PARAM;
				dcsCtrlContext.uiPacketType = DSI_SHORT_PACKET;
				dcsCtrlContext.uiVirtualChannel = 0;
				dcsCtrlContext.uiDCSCmd = 0x00;
				dcsCtrlContext.uiParam = NULL;
				dcsCtrlContext.uiParamCnt = 0;

				dcsContext.pDSICmdCtx = &dcsCtrlContext;
				dsi_setHSDcsCommand(&dcsContext);
				i++;
			} else if (pMode->pCmdQueue[i].uiAddress == CMDTEON) {
				if (dsi_setConfig(DSI_CONFIG_ID_SYNC_EN, (BOOL)pMode->pCmdQueue[i].uiValue) != E_OK) {
					DSI_ERR_MSG(" Configure pixel TE fail\r\n");
				} else {
					DSI_CFG_MSG(" Configure pixel TE %d success\r\n", (BOOL)pMode->pCmdQueue[i].uiValue);
				}
				ubTEON = TRUE;
				i++;
			} else if (pMode->pCmdQueue[i].uiAddress == CMDTEEXTPIN) {
				if (dsi_setConfig(DSI_CONFIG_ID_SYNC_SRC, (BOOL)pMode->pCmdQueue[i].uiValue) != E_OK) {
					DSI_ERR_MSG(" Configure pixel TE ext pin fail\r\n");
				} else {
					DSI_CFG_MSG(" Configure pixel TE ext pin %d success\r\n", (BOOL)pMode->pCmdQueue[i].uiValue);
				}
				ubTEExtPin = TRUE;
				i++;
			} else if (pMode->pCmdQueue[i].uiAddress == CMDTEONEACHFRAME) {
				if (dsi_setConfig(DSI_CONFIG_ID_SYNC_WITH_SETTEON, (BOOL)pMode->pCmdQueue[i].uiValue) != E_OK) {
					DSI_ERR_MSG(" Configure pixel TE ON each frame fail\r\n");
				} else {
					DSI_CFG_MSG(" Configure pixel TE TE ON each frame %d success\r\n", (BOOL)pMode->pCmdQueue[i].uiValue);
				}
				i++;
			} else if (pMode->pCmdQueue[i].uiAddress == CMDTEONEACHFRAMERTY) {
				if (dsi_setConfig(DSI_CONFIG_ID_SYNC_WITH_SETTEON_RTY, (BOOL)pMode->pCmdQueue[i].uiValue) != E_OK) {
					DSI_ERR_MSG(" Configure pixel TE ON each frame retry fail\r\n");
				} else {
					DSI_CFG_MSG(" Configure pixel TE TE ON each frame %d retry success\r\n", (BOOL)pMode->pCmdQueue[i].uiValue);
				}
				i++;
			} else if (pMode->pCmdQueue[i].uiAddress == CMDTEONEACHFRAMERTYTWICE) {
				if (dsi_setConfig(DSI_CONFIG_ID_SYNC_WITH_SETTEON_RTY_TWICEBTA, (BOOL)pMode->pCmdQueue[i].uiValue) != E_OK) {
					DSI_ERR_MSG(" Configure pixel TE ON each frame retry twice fail\r\n");
				} else {
					DSI_CFG_MSG(" Configure pixel TE TE ON each frame %d retry twice success\r\n", (BOOL)pMode->pCmdQueue[i].uiValue);
				}
				i++;
			} else {
				if (pMode->pCmdQueue[i].uiAddress == DSICMD_CMD) {
					dcsContext.uiCmdNo = 1;
					dcsContext.ubEOTEN = FALSE;
					dcsContext.ubBTAOnly = FALSE;
					dcsCtrlContext.uiParamCnt = 0;
					dcsCtrlContext.uiVirtualChannel = 0;
					dcsContext.uiSramOfs = 128;
					dcsCtrlContext.uiDCSCmd = pMode->pCmdQueue[i].uiValue;

					DSI_CMD_MSG("     [Cmd]=[0x%02x]\r\n", pMode->pCmdQueue[i].uiValue);

					for (dataCnt = i + 1; dataCnt < pMode->nCmd; dataCnt++) {
						if (pMode->pCmdQueue[dataCnt].uiAddress == DSICMD_DATA) {
							//data cnt++;
							if (pMode->pCmdQueue[dataCnt].uiValue > 0xFF) {
								DSI_ERR_MSG("MIPI command exceed 1 bytes error 0x%08x\r\n", pMode->pCmdQueue[dataCnt].uiValue);
							} else {
								DSI_CFG_MSG("  [Data%02d]=[0x%02x]\r\n", (dataCnt - (i + 1)), pMode->pCmdQueue[dataCnt].uiValue);
							}
							if (dcsCtrlContext.uiParamCnt > (MAX_DSI_CMD - 1)) {
								DSI_ERR_MSG("DSI data exceed [%03d] %03d\r\n", MAX_DSI_CMD, dcsCtrlContext.uiParamCnt);
								break;
							}
							dsiCmdParam[dcsCtrlContext.uiParamCnt] = pMode->pCmdQueue[dataCnt].uiValue;
							dcsCtrlContext.uiParamCnt++;
						} else {
							break;
						}
					}
					if (dcsCtrlContext.uiParamCnt == 0) {
						dcsCtrlContext.uiPacketType = DSI_SHORT_PACKET;
						dcsCtrlContext.uiDataType = DATA_TYPE_SHORT_WRITE_NO_PARAM;
						dcsCtrlContext.uiParam = NULL;
#if _CFG_USE_LP_
						dsiCmdParam[0] = 0x0;
						dsi_LPWriteCmd(DSI_DATA_LANE_0, dcsCtrlContext.uiDCSCmd, dsiCmdParam, 0, DSI_SHORT_PACKET, bEoPT);
#endif
					} else if (dcsCtrlContext.uiParamCnt == 1) {
						dcsCtrlContext.uiPacketType = DSI_SHORT_PACKET;
						dcsCtrlContext.uiDataType = DATA_TYPE_SHORT_WRITE_1_PARAM;
						dcsCtrlContext.uiParam = (UINT8 *)&dsiCmdParam;
#if _CFG_USE_LP_
						dsi_LPWriteCmd(DSI_DATA_LANE_0, dcsCtrlContext.uiDCSCmd, dsiCmdParam, 1, DSI_SHORT_PACKET, bEoPT);
#endif
					} else {
						dcsCtrlContext.uiPacketType = DSI_LONG_PACKET;
						dcsCtrlContext.uiDataType = DATA_TYPE_LONG_WRITE;
						dcsCtrlContext.uiParam = (UINT8 *)&dsiCmdParam[0];
#if _CFG_USE_LP_
						dsi_LPWriteCmd(DSI_DATA_LANE_0, dcsCtrlContext.uiDCSCmd, dsiCmdParam, dcsCtrlContext.uiParamCnt, DSI_LONG_PACKET, bEoPT);
#endif
					}
#if !_CFG_USE_LP_
					dcsContext.pDSICmdCtx = &dcsCtrlContext;
					dsi_setHSDcsCommand(&dcsContext);
#endif
				}



				//dispdev_writeLcdReg(pMode->pCmdQueue[i].uiAddress, pMode->pCmdQueue[i].uiValue);
				i += (dcsCtrlContext.uiParamCnt + 1);
			}
		}
	}
}


/*
    Display Device API of open LCD with DSI interface

    Display Device API of open LCD with DSI interface

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_NOSPT:  Driver without supporting the specified LCD Mode.
     - @b E_OK:     LCD Open done and success.
*/
//static UINT8 testPat[300];

static ER dispdev_openIFDsi(void)
{
	//UINT32                  uiErrorRpt;
	tLCD_PARAM              *pMode      = NULL;
	tLCD_PARAM              *pConfigMode = NULL;
	PINMUX_LCDINIT          LcdMode;
	UINT32                  ModeNumber;
	tDSI_CONFIG              DsiConfig;
	PINMUX_FUNC_ID          PinFuncID;
#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl1;
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl2;
#endif
	DISPDEV_IOCTRL_PARAM    DevIoCtrl;

	DSI_PIXEL_FORMATSEL     uiPixelFmt          = DSI_PIXEL_RGB_565;
	DSI_VIDEOPKT_TYPESEL    uiVideoSyncMode     = DSI_VIDEOPKT_TYPE_SYNC_EVENT;
	DSI_PIXPKT_MODESEL      uiPixelPktModeSel   = DSI_PIXMODE_VIDEO_MODE;
	UINT32                  uiLaneCnt;



	UINT32                  uiHactByteCount;
	UINT32                  uiHFPByteCount = 0;
	UINT32                  uiHBPByteCount = 0;
	UINT32                  uiBllPByteCount;
	UINT32                  uiHSA, uiVSA;

	UINT32                  CurFreq;
	PLL_ID                  SrcPLL;

	//UINT32                  THS_TRAIL;
	//UINT32                  THS_EXIT;
	//UINT32                  THS_CLK_POST;
	//UINT32                  THS_CLK_TRAIL;
	//UINT32                  THS_CLK_PREPARE;
	//UINT32                  THS_CLK_ZERO;
	//UINT32                  THS_CLK_PRE;
	//UINT32                  TLPX;
	//UINT32                  THS_PREPARE;
	//UINT32                  THS_ZERO;
	//These two param always be 1
	//UINT32                  THS_SYNC = 1;
	//UINT32                  TLP_transition_time = 1;
	//UINT32                  TLP_TOTAL;
	//UINT32                  uiVIDEData;
	//UINT32                  uiVtotal_Sub_Vactive;

	dispanl_debug(("^GopenIFDsi START\r\n"));

	uiHSA = 0;

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	//
	//  Sortig the SysInit Config Mode support
	//

#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID);

	pConfigMode = dispdev_getConfigModeDSI(&ModeNumber);

#if 0
	if ((pConfigMode != NULL) && (ModeNumber > 0)) {
		for (i = 0; i < ModeNumber; i++) {
			if (pConfigMode[i].Panel.LCDMode == LcdMode) {
				pMode = (tLCD_PARAM *)&pConfigMode[i];
				break;
			}
		}
	}
#else
	if (pConfigMode != NULL) {
		pMode = (tLCD_PARAM *)&pConfigMode[0];
	}
#endif
	if (pMode == NULL) {
		DBG_ERR("LCDMode=%d not support\r\n", LcdMode);
		return E_NOSPT;
	}

	if (dsi_isOpened()) {
		dsi_close();
	}

	dsi_open();

#if 0
#if _FPGA_EMULATION_
	DsiConfig.fDsiSrcClk = 24.0;
#else
	// Max clock = 480
	DsiConfig.fDsiSrcClk = 480.0;
#endif
#endif
	dispdev_setDSIConfig(&DsiConfig);

#if 1
	if (dsi_setConfig(DSI_CONFIG_ID_MODE, DSI_MODE_MANUAL_MODE) != E_OK) {
		DSI_ERR_MSG("Configure manual mode fail\r\n");
	}


	if ((pMode->Panel.LCDMode >= PINMUX_DSI_1_LANE_CMD_MODE_RGB565) && (pMode->Panel.LCDMode <= PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888)) {
		if (pMode->Panel.LCDMode <= PINMUX_DSI_1_LANE_CMD_MODE_RGB888) {
			uiPixelPktModeSel = DSI_PIXMODE_COMMAND_MODE;
			DSI_CFG_MSG(" Panel is command mode\r\n");
		} else {
			uiPixelPktModeSel = DSI_PIXMODE_VIDEO_MODE;
			DSI_CFG_MSG(" Panel is video mode\r\n");
		}
		DSI_CFG_MSG(" Panel is one lane configuration\r\n");
		dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
		uiLaneCnt = 1;
	}

	// Config & Enable display device, send manual mode command
	DispDeviceInit(pMode);

#if 0
	if (dsi_setConfig(DSI_CFG_PIXEL_FMT, uiPixelFmt) != E_OK) {
		DBG_DUMP("Configure pixel format fail\r\n");
	}
#endif
	DSI_CFG_MSG("=====IDE configuration====== \r\n");
	DSI_CFG_MSG("=VSyncBackPorch      = %04d=\r\n", pMode->Panel.uiVSyncBackPorchOdd);
	DSI_CFG_MSG("=VSyncTotalPeriod    = %04d=\r\n", pMode->Panel.uiVSyncTotalPeriod);
	DSI_CFG_MSG("=HSyncTotalPeriod    = %04d=\r\n", pMode->Panel.uiHSyncTotalPeriod);
	DSI_CFG_MSG("=HSyncBackPorch      = %04d=\r\n", pMode->Panel.uiHSyncBackPorch);
	DSI_CFG_MSG("=HSyncActivePeriod   = %04d=\r\n", pMode->Panel.uiHSyncActivePeriod);
	DSI_CFG_MSG("=VSyncActivePeriod   = %04d=\r\n", pMode->Panel.uiVSyncActivePeriod);
	DSI_CFG_MSG("=====IDE configuration====== \r\n");


#endif
	//dsi_setConfig(DSI_CFG_RGB_SWAP, TRUE);



	if ((pMode->Panel.LCDMode >= PINMUX_DSI_2_LANE_CMD_MODE_RGB565) && (pMode->Panel.LCDMode <= PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888)) {
		if (pMode->Panel.LCDMode <= PINMUX_DSI_2_LANE_CMD_MODE_RGB888) {
			uiPixelPktModeSel = DSI_PIXMODE_COMMAND_MODE;
			DSI_CFG_MSG(" Panel is command mode\r\n");
		} else {
			uiPixelPktModeSel = DSI_PIXMODE_VIDEO_MODE;
			DSI_CFG_MSG(" Panel is video mode\r\n");
		}
		DSI_CFG_MSG(" Panel is two lane configuration\r\n");
		dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
		uiLaneCnt = 2;
	}

	if ((pMode->Panel.LCDMode >= PINMUX_DSI_4_LANE_CMD_MODE_RGB565) && (pMode->Panel.LCDMode <= PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888)) {
		if (pMode->Panel.LCDMode <= PINMUX_DSI_4_LANE_CMD_MODE_RGB888) {
			uiPixelPktModeSel = DSI_PIXMODE_COMMAND_MODE;
			DSI_CFG_MSG(" Panel is command mode\r\n");
		} else {
			uiPixelPktModeSel = DSI_PIXMODE_VIDEO_MODE;
			DSI_CFG_MSG(" Panel is video mode\r\n");
		}
		DSI_CFG_MSG(" Panel is four lane configuration\r\n");
		dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_3);
		uiLaneCnt = 4;
	}

	switch (pMode->Panel.LCDMode) {
	case PINMUX_DSI_1_LANE_CMD_MODE_RGB565:
	case PINMUX_DSI_2_LANE_CMD_MODE_RGB565:
	case PINMUX_DSI_4_LANE_CMD_MODE_RGB565:
	case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565:
	case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565:
	case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB565:
	case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565:
	case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565:
	case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB565:
		uiPixelFmt = DSI_PIXEL_RGB_565;
		DSI_CFG_MSG(" Panel is RGB565 configuration\r\n");
		break;

	case PINMUX_DSI_1_LANE_CMD_MODE_RGB666P:
	case PINMUX_DSI_2_LANE_CMD_MODE_RGB666P:
	case PINMUX_DSI_4_LANE_CMD_MODE_RGB666P:
	case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P:
	case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P:
	case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P:
	case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666P:
	case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666P:
	case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666P:
		if (dsi_getConfig(DSI_CONFIG_ID_CHIP_VER) == DSI_DRV_CHIPVER_A) {
			if ((pMode->Panel.uiHSyncActivePeriod % 16) != 0) {
				DBG_ERR("RGB666P must be 16-pixel alignment\r\n");
			}
		}
		uiPixelFmt = DSI_PIXEL_RGB_666_PACKETED;
		DSI_CFG_MSG(" Panel is RGB666P configuration\r\n");
		break;

	case PINMUX_DSI_1_LANE_CMD_MODE_RGB666L:
	case PINMUX_DSI_2_LANE_CMD_MODE_RGB666L:
	case PINMUX_DSI_4_LANE_CMD_MODE_RGB666L:
	case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L:
	case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L:
	case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L:
	case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666L:
	case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666L:
	case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666L:
		uiPixelFmt = DSI_PIXEL_RGB_666_LOOSELY;
		DSI_CFG_MSG(" Panel is RGB666L configuration\r\n");
		break;

	case PINMUX_DSI_1_LANE_CMD_MODE_RGB888:
	case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB888:
	case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888:
	case PINMUX_DSI_2_LANE_CMD_MODE_RGB888:
	case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888:
	case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888:
	case PINMUX_DSI_4_LANE_CMD_MODE_RGB888:
	case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB888:
	case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888:
		uiPixelFmt = DSI_PIXEL_RGB_888;
		DSI_CFG_MSG(" Panel is RGB888 configuration\r\n");
		break;
	default:
		DSI_CFG_MSG(" Unknow DSI pixel format mode\r\n");
		break;
	}

	if (uiPixelPktModeSel == DSI_PIXMODE_VIDEO_MODE) {
		switch (pMode->Panel.LCDMode) {
		case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565:
		case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565:
		case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB565:
		case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P:
		case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P:
		case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666P:
		case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L:
		case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L:
		case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666L:
		case PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB888:
		case PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888:
		case PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB888:
			uiVideoSyncMode = DSI_VIDEOPKT_TYPE_SYNC_PULSE;
			DSI_CFG_MSG(" Panel is sync pulse configuration\r\n");
			break;

		case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565:
		case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565:
		case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB565:
		case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P:
		case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666P:
		case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666P:
		case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L:
		case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666L:
		case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666L:
		case PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888:
		case PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888:
		case PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888:
			uiVideoSyncMode = DSI_VIDEOPKT_TYPE_SYNC_EVENT;
			DSI_CFG_MSG(" Panel is sync event configuration\r\n");
			break;
		default:
			DSI_CFG_MSG(" Unknow DSI sync mode\r\n");
			break;
		}
	}

	if (dsi_setConfig(DSI_CONFIG_ID_PIXEL_FMT, uiPixelFmt) != E_OK) {
		DSI_ERR_MSG(" Configure pixel format fail\r\n");
	} else {
		DSI_CFG_MSG(" Configure pixel format 0x%02x success\r\n", uiPixelFmt);
	}


	if (uiPixelPktModeSel == DSI_PIXMODE_COMMAND_MODE) {
		dsi_setConfig(DSI_CONFIG_ID_BLANK_CTRL, TRUE);

		dsi_setConfig(DSI_CONFIG_ID_INTER_PKT_LP, TRUE);
#if 0
		if (dsi_setConfig(DSI_CONFIG_ID_SYNC_EN, pMode->Panel.bTEON) != E_OK) {
			DSI_ERR_MSG(" Configure pixel TE fail\r\n");
		} else {
			DSI_CFG_MSG(" Configure pixel TE %d success\r\n", pMode->Panel.bTEON);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_SYNC_SRC, pMode->Panel.bExtPin) != E_OK) {
			DSI_ERR_MSG(" Configure pixel TE sync src\r\n");
		} else {
			DSI_CFG_MSG(" Configure pixel TE sync src %d success\r\n", pMode->Panel.bExtPin);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_SYNC_WITH_SETTEON, pMode->Panel.bTEONEachFrame) != E_OK) {
			DSI_ERR_MSG(" Configure pixel TE sync each frame\r\n");
		} else {
			DSI_CFG_MSG(" Configure pixel TE sync each frame %d success\r\n", pMode->Panel.bTEONEachFrame);
		}
#endif
		if (uiPixelFmt == DSI_PIXEL_RGB_666_PACKETED) {
			uiHactByteCount = (pMode->Panel.uiHSyncActivePeriod * 9) / 4 ;
			uiHFPByteCount = (pMode->Panel.uiHSyncBackPorch * 9) / 4;
			uiHBPByteCount = (pMode->Panel.uiHSyncBackPorch * 9) / 4;
			uiBllPByteCount = (pMode->Panel.uiHSyncTotalPeriod * 9) / 4;

			//uiBllPByteCount = pMode->Panel.uiHSyncTotalPeriod * 2;
		} else if (uiPixelFmt == DSI_PIXEL_RGB_565) {
			uiHactByteCount = (pMode->Panel.uiHSyncActivePeriod) * 2;
			uiHFPByteCount = pMode->Panel.uiHSyncBackPorch * 2;
			uiHBPByteCount = pMode->Panel.uiHSyncBackPorch * 2;
			//uiBllPByteCount = pMode->Panel.uiHSyncTotalPeriod * 2;

			uiBllPByteCount = uiHactByteCount + uiHFPByteCount + uiHBPByteCount;
		}
		// DSI_PIXEL_RGB_666_LOOSELY & DSI_PIXEL_RGB_888
		//else if(uiPixelFmt == DSI_PIXEL_RGB_666_LOOSELY)
		//{
		//}
		else {
			uiHactByteCount = (pMode->Panel.uiHSyncActivePeriod) * 3;
			uiHFPByteCount = pMode->Panel.uiHSyncBackPorch * 3;
			uiHBPByteCount = pMode->Panel.uiHSyncBackPorch * 3;
			uiBllPByteCount = uiHactByteCount + uiHFPByteCount + uiHBPByteCount;
		}

#if 0
		if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_MODE, DSI_PIXMODE_COMMAND_MODE) != E_OK) {
			DBG_DUMP("Configure pixel mode command mode fail\r\n");
		}
#endif
		if (dsi_setConfig(DSI_CONFIG_ID_HFP, uiHFPByteCount) != E_OK) {
			DSI_ERR_MSG(" Configure HFP as 0x%08x error\r\n", uiHFPByteCount);
		} else {
			DSI_CFG_MSG(" Configure HFP as %03d success\r\n", uiHFPByteCount);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_HBP, uiHBPByteCount) != E_OK) {
			DSI_ERR_MSG(" Configure HPB as 0x%08x error\r\n", uiHBPByteCount);
		} else {
			DSI_CFG_MSG(" Configure HPB as %03d success\r\n", uiHBPByteCount);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_HACT, uiHactByteCount) != E_OK) {
			DSI_ERR_MSG(" Configure HACT as %3d fail\r\n", uiHactByteCount);
		} else {
			DSI_CFG_MSG(" Configure HACT as %03d success\r\n", uiHactByteCount);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_VSA, pMode->Panel.uiVSyncBackPorchEven) != E_OK) {
			DSI_ERR_MSG(" Configure VSA as 0x%08x\r\n", pMode->Panel.uiVSyncBackPorchEven);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_PH_DT, DATA_TYPE_LONG_WRITE) != E_OK) {
			DSI_ERR_MSG(" Configure pkt packet header data type fail\r\n");
		}

		if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_PH_VC, 0) != E_OK) {
			DSI_ERR_MSG(" Configure pkt packet header vc fail\r\n");
		}

		if (dsi_setConfig(DSI_CONFIG_ID_DCS_CT0, DCS_CMD_MEMORY_WRITE) != E_OK) {
			DSI_ERR_MSG(" Configure CT0 as mem write fail\r\n");
		}

		if (dsi_setConfig(DSI_CONFIG_ID_DCS_CT1, DCS_CMD_MEMORY_WRITE_CONT) != E_OK) {
			DSI_ERR_MSG(" Configure CT1 as mem write cont fail\r\n");
		}

		if (dsi_setConfig(DSI_CONFIG_ID_VTOTAL, pMode->Panel.uiVSyncTotalPeriod + 10 * PORCH_ALIGN) != E_OK) {
			DSI_ERR_MSG(" Configure VTotal as %03d\r\n", pMode->Panel.uiVSyncTotalPeriod);
		} else {
			DSI_CFG_MSG(" Configure VTotal as %03d success\r\n", pMode->Panel.uiVSyncTotalPeriod);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_VVALID_START, pMode->Panel.uiVSyncBackPorchEven + PORCH_ALIGN) != E_OK) {
			DSI_ERR_MSG(" Configure VStart as 0x%08x error\r\n", pMode->Panel.uiVSyncBackPorchEven);
		} else {
			DSI_CFG_MSG(" Configure VStart as %03d success\r\n", pMode->Panel.uiVSyncBackPorchEven);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_VVALID_END, (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + PORCH_ALIGN) - 1) != E_OK) {
			DSI_ERR_MSG(" Configure VEnd as %03d error\r\n", ((pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + PORCH_ALIGN) - 1));
		} else {
			DSI_CFG_MSG(" Configure VEnd as %03d success\r\n", ((pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + PORCH_ALIGN) - 1));
		}
	}


	else {
		if ((pMode->Panel.LCDMode >= PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB565) &&
			((pMode->Panel.LCDMode <= PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888))) {
			dsi_setConfig(DSI_CONFIG_ID_BLANK_CTRL, TRUE);
		} else {
			dsi_setConfig(DSI_CONFIG_ID_BLANK_CTRL, FALSE);
		}

		dsi_setConfig(DSI_CONFIG_ID_INTER_PKT_LP, FALSE);

		uiVSA = pMode->Panel.uiVSyncBackPorchOdd / 2;

		if (uiPixelFmt == DSI_PIXEL_RGB_666_PACKETED) {
			if (uiVideoSyncMode == DSI_VIDEOPKT_TYPE_SYNC_PULSE) {
				uiHactByteCount = (pMode->Panel.uiHSyncActivePeriod * 9) / 4 ;
				uiHFPByteCount = (pMode->Panel.uiHSyncBackPorch * 9) / 4;
				uiHBPByteCount = (pMode->Panel.uiHSyncBackPorch * 9) / 4;
				uiBllPByteCount = uiHactByteCount + uiHFPByteCount + uiHBPByteCount + 12;
				if (uiHBPByteCount % 2 != 0) {
					uiHSA = uiHBPByteCount / 2;
					uiHBPByteCount = uiHBPByteCount / 2 + 1;
				} else {
					uiHSA = uiHBPByteCount / 2;
					uiHBPByteCount = uiHBPByteCount / 2;
				}
			} else {
				uiHactByteCount = (pMode->Panel.uiHSyncActivePeriod * 9) / 4 ;
				uiHFPByteCount = (pMode->Panel.uiHSyncBackPorch * 9) / 4;
				uiHBPByteCount = (pMode->Panel.uiHSyncBackPorch * 9) / 4;
				uiBllPByteCount = uiHactByteCount + uiHFPByteCount + uiHBPByteCount + 12;
			}
			if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_PH_DT, DATA_TYPE_PACKET_STREAM_RGB_666_PACKED) != E_OK) {
				DSI_ERR_MSG(" Configure pixel packet header error [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_565_PACKED);
			} else {
				DSI_CFG_MSG(" Configure pixel packet header success [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_565_PACKED);
			}
		} else if (uiPixelFmt == DSI_PIXEL_RGB_565) {
			if (uiVideoSyncMode == DSI_VIDEOPKT_TYPE_SYNC_PULSE) {
				uiHactByteCount = pMode->Panel.uiHSyncActivePeriod * 2;
				uiHFPByteCount = (pMode->Panel.uiHSyncBackPorch + PORCH_ALIGN) * 2;
				uiHBPByteCount = (pMode->Panel.uiHSyncBackPorch + PORCH_ALIGN) * 2;

				uiHSA = uiHBPByteCount / 2;
				uiHBPByteCount = uiHBPByteCount / 2;

				uiBllPByteCount = uiHactByteCount + uiHFPByteCount + uiHBPByteCount + 12;
			} else {
				uiHactByteCount = pMode->Panel.uiHSyncActivePeriod * 2;
				uiHFPByteCount = (pMode->Panel.uiHSyncBackPorch + PORCH_ALIGN) * 2;
				uiHBPByteCount = (pMode->Panel.uiHSyncBackPorch + PORCH_ALIGN) * 2;
				uiBllPByteCount = uiHactByteCount + uiHFPByteCount + uiHBPByteCount + 12;
			}
			if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_PH_DT, DATA_TYPE_PACKET_STREAM_RGB_565_PACKED) != E_OK) {
				DSI_ERR_MSG(" Configure pixel packet header error [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_565_PACKED);
			} else {
				DSI_CFG_MSG(" Configure pixel packet header success [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_565_PACKED);
			}
		} else {
			UINT32 uiHBPHFPSum;
			//UINT32 X = 5;
			//UINT32 Y = 0;

			//*********************************************************************************************************
			//((Htotal + X) * DSI_CLK)/IDE_CLK = (22+HBP+HFP+HACT)/2
			//((uiHSyncTotalPeriod + X) * DSI_CLK)/IDE_CLK * LANENUM - 22 - HACT(byteCount) = HBP+HFP
			//*********************************************************************************************************

			//DSI_VIDEOPKT_TYPE_SYNC_EVENT
			uiHactByteCount = pMode->Panel.uiHSyncActivePeriod * 3;             //HACT


			//uiHBPHFPSum = (((pMode->Panel.uiHSyncTotalPeriod + X) * 3) / 2) * uiLaneCnt - 22 - uiHactByteCount;

			uiHBPHFPSum = ((pMode->Panel.uiHSyncTotalPeriod) * 3) - uiHactByteCount - 22;

			DSI_CFG_MSG("uiHSyncTotalPeriod = %d\r\n", pMode->Panel.uiHSyncTotalPeriod * 3);
			DSI_CFG_MSG("uiHactByteCount= %d, uiHBPHFPSum = %d\r\n", uiHactByteCount, uiHBPHFPSum);

			if (uiHBPHFPSum % 2) {
				DBG_ERR("uiHBPHFPSum = %d MUST be even number\r\n", uiHBPHFPSum);
				//??return E_SYS; //??Shirley remove for NT35410
			}
			//else
			//{
			uiHBPByteCount = (uiHBPHFPSum >> 1) /*- 10*/;
			uiHFPByteCount = (uiHBPHFPSum >> 1) /*+ 10*/;
			//}

			if (uiHBPByteCount % 4) {
				DBG_ERR("uiHBPByteCount = %d MUST be 4x number\r\n", uiHBPByteCount);
				uiHBPByteCount = (uiHBPByteCount >> 2) << 2;
				uiHFPByteCount = (uiHFPByteCount >> 2) << 2;
			}

			//Calculate LP timing
			//THS_TRAIL           = dsi_getConfig(DSI_CONFIG_ID_THS_TRAIL) + 1;
			//THS_EXIT            = dsi_getConfig(DSI_CONFIG_ID_THS_EXIT) + 1;
			//THS_CLK_POST        = dsi_getConfig(DSI_CONFIG_ID_TCLK_POST) + 1;
			//THS_CLK_TRAIL       = dsi_getConfig(DSI_CONFIG_ID_TCLK_TRAIL) + 1;
			//THS_CLK_PREPARE     = dsi_getConfig(DSI_CONFIG_ID_TCLK_PREPARE) + 1;
			//THS_CLK_ZERO        = dsi_getConfig(DSI_CONFIG_ID_TCLK_ZERO) + 1;
			//THS_CLK_PRE         = dsi_getConfig(DSI_CONFIG_ID_TCLK_PRE) + 1;
			//TLPX                = dsi_getConfig(DSI_CONFIG_ID_TLPX) + 1;
			//THS_PREPARE         = dsi_getConfig(DSI_CONFIG_ID_THS_PREPARE) + 1;
			//THS_ZERO            = dsi_getConfig(DSI_CONFIG_ID_THS_ZERO) + 1;
			/*
			            TLP_TOTAL           = THS_TRAIL + \
			                                  THS_EXIT + \
			                                  TLP_transition_time + \
			                                  THS_CLK_POST + \
			                                  THS_CLK_TRAIL + \
			                                  THS_EXIT + \
			                                  TLPX + \
			                                  THS_CLK_PREPARE + \
			                                  THS_CLK_ZERO + \
			                                  THS_CLK_PRE + \
			                                  TLP_transition_time +\
			                                  TLPX + \
			                                  THS_PREPARE + \
			                                  THS_ZERO + \
			                                  THS_SYNC;

			*/
			//uiVtotal_Sub_Vactive= pMode->Panel.uiVSyncTotalPeriod - pMode->Panel.uiVSyncActivePeriod;
//          uiVIDEData = (Vtotal - VACT       ) * Htotal                          * DSI_CLK/IDE_CLK
//          uiVIDEData = (uiVtotal_Sub_Vactive) * pMode->Panel.uiHSyncTotalPeriod * 3 / 2;

			//uiBllPByteCount = (((uiVIDEData - TLP_TOTAL - Y -3) * uiLaneCnt) / (uiVtotal_Sub_Vactive)) - 10;
			//uiBllPByteCount = (((uiVIDEData - TLP_TOTAL -3) * uiLaneCnt) / (uiVtotal_Sub_Vactive)) - 10;
			uiBllPByteCount = uiHBPByteCount + uiHFPByteCount + uiHactByteCount + 22 - 10;


			//DBG_DUMP("uiHFPByteCount = 0x%08x\r\n", uiHFPByteCount);
			//DBG_DUMP("uiBllPByteCount = 0x%08x LP = %d\r\n", uiBllPByteCount, TLP_TOTAL);
			//DBG_DUMP("uiBHBPByteCount = 0x%08x\r\n", uiHBPByteCount);


			if (uiVideoSyncMode == DSI_VIDEOPKT_TYPE_SYNC_PULSE) {
				/*if(uiHBPByteCount % 2 != 0)
				{
				    uiHSA = uiHBPByteCount / 2;
				    uiHBPByteCount = uiHBPByteCount / 2 + 1;
				}
				else
				{
				    uiHSA = uiHBPByteCount / 2;
				    uiHBPByteCount = uiHBPByteCount / 2;
				}*/
				uiHSA = uiHBPByteCount / 2;
				uiHBPByteCount = uiHBPByteCount - uiHSA - 10;
			}


			if (uiPixelFmt == DSI_PIXEL_RGB_666_LOOSELY) {
				if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_PH_DT, DATA_TYPE_PACKET_STREAM_RGB_666_LOOSELY) != E_OK) {
					DSI_ERR_MSG(" Configure pixel packet header error [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_666_LOOSELY);
				} else {
					DSI_CFG_MSG(" Configure pixel packet header success [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_666_LOOSELY);
				}
			} else {
				if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_PH_DT, DATA_TYPE_PACKET_STREAM_RGB_888_PACKED) != E_OK) {
					DSI_ERR_MSG(" Configure pixel packet header error [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_888_PACKED);
				} else {
					DSI_CFG_MSG(" Configure pixel packet header success [0x%02x]\r\n", DATA_TYPE_PACKET_STREAM_RGB_888_PACKED);
				}
			}
			if ((pMode->Panel.LCDMode >= PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB565) &&
				((pMode->Panel.LCDMode <= PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888))) {
				//if (dsi_getConfig(DSI_CONFIG_ID_BLANK_CTRL) == TRUE)
				{
					uiHBPByteCount = uiHBPByteCount / uiLaneCnt;
					uiHFPByteCount = uiHFPByteCount / uiLaneCnt;
					uiBllPByteCount = uiBllPByteCount / uiLaneCnt;
				}
				//DBG_DUMP("2.uiHFPByteCount = 0x%08x\r\n", uiHFPByteCount);
				//DBG_DUMP("2.uiBllPByteCount = 0x%08x LP = %d\r\n", uiBllPByteCount, TLP_TOTAL);
				//DBG_DUMP("2.uiBHBPByteCount = 0x%08x\r\n", uiHBPByteCount);
			}
		}

		if (dsi_setConfig(DSI_CONFIG_ID_VTOTAL, pMode->Panel.uiVSyncTotalPeriod) != E_OK) {
			DSI_ERR_MSG(" Configure VTotal as %03d error\r\n", pMode->Panel.uiVSyncTotalPeriod);
		} else {
			DSI_CFG_MSG(" Configure VTotal as %03d success\r\n", pMode->Panel.uiVSyncTotalPeriod);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_VVALID_START, pMode->Panel.uiVSyncBackPorchEven + 2) != E_OK) {
			DSI_ERR_MSG(" Configure VStart as 0x%08x error\r\n", pMode->Panel.uiVSyncBackPorchEven + 2);
		} else {
			DSI_CFG_MSG(" Configure VStart as %03d success\r\n", pMode->Panel.uiVSyncBackPorchEven + 2);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_VVALID_END, (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + 2) - 1) != E_OK) {
			DSI_ERR_MSG(" Configure VEnd as %03d error\r\n", (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + 2) - 1);
		} else {
			DSI_CFG_MSG(" Configure VEnd as %03d success\r\n", (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + 2) - 1);
		}

#if 0
		if (dsi_setConfig(DSI_CFG_PIXPKT_MODE, DSI_PIXMODE_VIDEO_MODE) != E_OK) {
			DBG_DUMP("^R Configure pixel mode video mode fail\r\n");
		} else {
			DBG_DUMP("^G Configure pixel mode video mode success\r\n");
		}
#endif
		if (dsi_setConfig(DSI_CONFIG_ID_VDOPKT_TYPE, uiVideoSyncMode) != E_OK) {
			DSI_ERR_MSG(" Configure video sync mode [%d] fail\r\n", uiVideoSyncMode);
		} else {
			DSI_CFG_MSG(" Configure video sync mode [%d] success\r\n", uiVideoSyncMode);
		}
		if (uiVideoSyncMode == DSI_VIDEOPKT_TYPE_SYNC_PULSE) {
			if (dsi_setConfig(DSI_CONFIG_ID_VSA, uiVSA) != E_OK) {
				DSI_ERR_MSG(" Configure VSA as %03d fail\r\n", uiVSA);
			} else {
				DSI_CFG_MSG(" Configure VSA as %03d success\r\n", uiVSA);
			}

			if (dsi_setConfig(DSI_CONFIG_ID_HSA, uiHSA) != E_OK) {
				DSI_CFG_MSG(" Configure HSA as %3d fail\r\n", uiHSA);
			} else {
				DSI_CFG_MSG(" Configure HSA as %03d success\r\n", uiHSA);
			}
		} else {
			dsi_setConfig(DSI_CONFIG_ID_SYNCEVT_NULL_LEN, 0x20);

			dsi_setConfig(DSI_CONFIG_ID_SYNCEVT_SLICE_NO, 0);

			//uiHactByteCount = uiHactByteCount / 2;

			//uiBllPByteCount += (0x20 + 18);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_HBP, uiHBPByteCount) != E_OK) {
			DSI_ERR_MSG(" Configure HPB as 0x%08x error\r\n", uiHBPByteCount);
		} else {
			DSI_CFG_MSG(" Configure HPB as %03d success\r\n", uiHBPByteCount);
		}


		if (dsi_setConfig(DSI_CONFIG_ID_HFP, uiHFPByteCount) != E_OK) {
			DSI_ERR_MSG(" Configure HFP as 0x%08x error\r\n", uiHFPByteCount);
		} else {
			DSI_CFG_MSG(" Configure HFP as %03d success\r\n", uiHFPByteCount);
		}

		if (dsi_setConfig(DSI_CONFIG_ID_HACT, uiHactByteCount) != E_OK) {
			DSI_ERR_MSG(" Configure HACT as %3d fail\r\n", uiHactByteCount);
		} else {
			DSI_CFG_MSG(" Configure HACT as %03d success\r\n", uiHactByteCount);
		}
	}

	if (dsi_setConfig(DSI_CONFIG_ID_IDEHVALID, pMode->Panel.uiHSyncActivePeriod) != E_OK) {
		DSI_ERR_MSG(" Configure IDEHVALID as %03d error\r\n", (pMode->Panel.uiHSyncActivePeriod));
	} else {
		DSI_CFG_MSG(" Configure IDEHVALID as %03d success\r\n", (pMode->Panel.uiHSyncActivePeriod));
	}

	if (dsi_setConfig(DSI_CONFIG_ID_PIXPKT_MODE, uiPixelPktModeSel) != E_OK) {
		DSI_ERR_MSG(" Configure pixel mode [%d] mode fail => (0:video/1:command)\r\n", uiPixelPktModeSel);
	} else {
		DSI_CFG_MSG(" Configure pixel mode [%d] mode success => (0:video/1:command)\r\n", uiPixelPktModeSel);
	}

	if (dsi_setConfig(DSI_CONFIG_ID_BLLP, uiBllPByteCount) != E_OK) {
		DSI_ERR_MSG(" Configure BLLP as 0x%08x error\r\n", uiBllPByteCount);
	} else {
		DSI_CFG_MSG(" Configure BLLP as %03d success\r\n", uiBllPByteCount);
	}

#if 0
	if (dsi_setConfig(DSI_CONFIG_ID_VTOTAL, pMode->Panel.uiVSyncTotalPeriod + 10 * PORCH_ALIGN) != E_OK) {
		DSI_ERR_MSG(" Configure VTotal as %03d error\r\n", pMode->Panel.uiVSyncTotalPeriod);
	} else {
		DSI_CFG_MSG(" Configure VTotal as %03d success\r\n", pMode->Panel.uiVSyncTotalPeriod);
	}

	if (dsi_setConfig(DSI_CONFIG_ID_VVALID_START, pMode->Panel.uiVSyncBackPorchEven + 2) != E_OK) {
		DSI_ERR_MSG(" Configure VStart as 0x%08x error\r\n", pMode->Panel.uiVSyncBackPorchEven + 2);
	} else {
		DSI_CFG_MSG(" Configure VStart as %03d success\r\n", pMode->Panel.uiVSyncBackPorchEven + 2);
	}

	if (dsi_setConfig(DSI_CONFIG_ID_VVALID_END, (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + 2) - 1) != E_OK) {
		DSI_ERR_MSG(" Configure VEnd as %03d error\r\n", (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + 2) - 1);
	} else {
		DSI_CFG_MSG(" Configure VEnd as %03d success\r\n", (pMode->Panel.uiVSyncActivePeriod + pMode->Panel.uiVSyncBackPorchEven + 2) - 1);
	}
#endif
#if 0
	if (dsi_setConfig(DSI_CONFIG_ID_MODE, DSI_MODE_AUTO_MODE1) != E_OK) {
		DSI_ERR_MSG(" Configure auto mode 1 fail\r\n");
	}
#else
	if (dsi_setConfig(DSI_CONFIG_ID_MODE, DSI_MODE_AUTO_MODE2) != E_OK) {
		DSI_ERR_MSG(" Configure auto mode 2 fail\r\n");
	}
#endif
	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIoCtrl);

	switch (DevIoCtrl.SEL.GET_SRCCLK.SrcClk) {
	case DISPCTRL_SRCCLK_PLL1:
	case DISPCTRL_SRCCLK_IDE2_PLL1: {
			SrcPLL = PLL_ID_1;
		}
		break;

	case DISPCTRL_SRCCLK_PLL2:
	case DISPCTRL_SRCCLK_IDE2_PLL2: {
			SrcPLL = PLL_ID_2;
		}
		break;

	case DISPCTRL_SRCCLK_PLL4:
	case DISPCTRL_SRCCLK_IDE2_PLL4: {
			SrcPLL = PLL_ID_4;
		}
		break;

	case DISPCTRL_SRCCLK_PLL14:
	case DISPCTRL_SRCCLK_IDE2_PLL14: {
			SrcPLL = PLL_ID_14;
		}
		break;

	default: {
			DBG_ERR("Error source clock %d\r\n", DevIoCtrl.SEL.GET_SRCCLK.SrcClk);
			return E_SYS;
		}

	}


	CurFreq = pll_getPLLFreq(SrcPLL) / 1000000;

	if (SrcPLL != PLL_ID_1) {
		DBG_IND("Original SrcClk(%d)Mhz\r\n", CurFreq);
		//pll_setPLLEn(SrcPLL,    FALSE);
		//pll_setPLL(SrcPLL,      IDE_SRC_CLK_480);
		//pll_setPLLEn(SrcPLL,    TRUE);
		gDispDev_SrcFreq = CurFreq;
		DBG_IND("DEVDSI: Chg PLL%d to(%d)Mhz\r\n", (SrcPLL + 1), pll_getPLLFreq(SrcPLL) / 1000000);
	} else {
		gDispDev_SrcFreq = 0;
		DBG_IND("Original SrcClk already PLL1(%d)Mhz\r\n", CurFreq);
	}

	//enable PLL for DSI (PLL11)
	pll_setPLLEn(PLL_ID_11,    TRUE);

	// Config & Enable Display physical engine (IDE engine)
	dispdev_setDisplay(pDispDevControl, tLCD_INF_MIPI_DSI, pMode, PinFuncID);

	if (ubTEON == TRUE && ubTEExtPin == TRUE) {
		//switch PINMUX
		UINT32 uiReg;

		uiReg = *(UINT32 *)0xF0010008;
		uiReg &= ~(0x400000F);
		uiReg |= 0x4000008;

		*(UINT32 *)0xF0010008 = uiReg;
	}

	dsi_setTXEN(TRUE, TRUE);

	dispanl_debug(("^GopenIFDsi Done\r\n"));
#if 0
	// Test Panel Frame Rate
	{
		UINT32 i;

		Perf_Open();
		Perf_Mark();
		for (i = 0; i < 1000; i++) {
			pDevIF->waitFrmEnd();
		}
		DBG_DUMP("Time tick = %d us\r\n", Perf_GetDuration());
		Perf_Close();
	}
#endif

	return E_OK;
}

/*
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @param[in] pDevIF Please refer to dispdevctrl.h for details.

    @return
     - @b E_OK:     LCD Close done and success.
*/
static ER dispdev_closeIFDsi(void)
{
	UINT32                  i, CmdNumber, dataCnt;
	tPANEL_CMD              *pStandby = NULL;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if !_CFG_USE_LP_
	DSI_CMD_RW_CTRL_PARAM   dcsContext;
#endif
	DSI_CMD_CTRL_PARAM      dcsCtrlContext;
#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl1;
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl2;
#endif


	BOOL                    bEoPT = TRUE;
	UINT32                  SrcPLL;



	dispanl_debug(("^GcloseIFDsi START\r\n"));

	DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
	/*    if(dsi_getConfig(DSI_CONFIG_ID_PIXPKT_MODE) == DSI_PIXMODE_VIDEO_MODE)
	    {
	        pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
	        pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl); //Disable IDE
	        dsi_waitFrameEnd();
	        dsi_setTXEN(FALSE, TRUE);                               //Disable DSI
	    }
	    else
	    {
	        dsi_waitFrameEnd();
	        pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl); //Disable IDE
	        dsi_setTXEN(FALSE, TRUE);
	        pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
	        pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
	    }*/
	{
		//DBG_ERR("wait dsi_waitFrameEnd\r\n");
		dsi_waitFrameEnd();
		loc_cpu();
		pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl); //Disable IDE
		dsi_setTXEN(FALSE, FALSE);                              //Disable DSI
		unl_cpu();
		//DBG_ERR("wait ide frame end .....\r\n");
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
		//DBG_ERR("wait frame 1\r\n");
		pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);
		//DBG_ERR("wait frame 2\r\n");

		DevIOCtrl.SEL.SET_DEVICE.DispDevType = DISPDEV_TYPE_RGB_SERIAL; //switch to display format 0
		pDispDevControl(DISPDEV_IOCTRL_SET_DEVICE, &DevIOCtrl);

	}
#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	DevIOCtrl.SEL.SET_CLK_EN.bClkEn = FALSE;
	pDispDevControl(DISPDEV_IOCTRL_SET_CLK_EN, &DevIOCtrl);
#endif

	pStandby = dispdev_getStandbyCmdDSI(&CmdNumber);

	dcsCtrlContext.uiParamCnt = 0;

	if (pStandby != NULL) {
		if (dsi_setConfig(DSI_CONFIG_ID_MODE, DSI_MODE_MANUAL_MODE) != E_OK) {
			DSI_ERR_MSG("^R Configure manual mode fail\r\n");
		}

		// Set display device into Standby or powerdown.
		for (i = 0; i < CmdNumber;) {
			if (pStandby[i].uiAddress == CMDDELAY_MS) {
				Delay_DelayMs(pStandby[i].uiValue);
				i++;
			} else if (pStandby[i].uiAddress == CMDDELAY_US) {
				Delay_DelayUs(pStandby[i].uiValue);
				i++;
			} else {
				if (pStandby[i].uiAddress == DSICMD_CMD) {
#if !_CFG_USE_LP_
					dcsContext.ubBTAEN = FALSE;
					dcsContext.ubBTAOnly = FALSE;
					dcsContext.uiCmdNo = 1;
					dcsContext.ubEOTEN = FALSE;
					dcsContext.uiSramOfs = 128;
#endif
					dcsCtrlContext.uiParamCnt = 0;
					dcsCtrlContext.uiVirtualChannel = 0;

					dcsCtrlContext.uiDCSCmd = pStandby[i].uiValue;

					for (dataCnt = i + 1; dataCnt < CmdNumber; dataCnt++) {
						if (pStandby[dataCnt].uiAddress == DSICMD_DATA) {
							//data cnt++;
							if (pStandby[dataCnt].uiValue > 0xFF) {
								DSI_ERR_MSG("MIPI command exceed 1 bytes error 0x%08x\r\n", pStandby[dataCnt].uiValue);
							}
							dsiCmdParam[dcsCtrlContext.uiParamCnt] = pStandby[dataCnt].uiValue;
							dcsCtrlContext.uiParamCnt++;
						} else {
							break;
						}
					}
					if (dcsCtrlContext.uiParamCnt == 0) {
						dcsCtrlContext.uiPacketType = DSI_SHORT_PACKET;
						dcsCtrlContext.uiDataType = DATA_TYPE_SHORT_WRITE_NO_PARAM;
						dcsCtrlContext.uiParam = NULL;
#if _CFG_USE_LP_
						dsiCmdParam[0] = 0x0;
						dsi_LPWriteCmd(DSI_DATA_LANE_0, dcsCtrlContext.uiDCSCmd, dsiCmdParam, 0, DSI_SHORT_PACKET, bEoPT);
#endif

					} else if (dcsCtrlContext.uiParamCnt == 1) {
						dcsCtrlContext.uiPacketType = DSI_SHORT_PACKET;
						dcsCtrlContext.uiDataType = DATA_TYPE_SHORT_WRITE_1_PARAM;
						dcsCtrlContext.uiParam = (UINT8 *)&dsiCmdParam;
#if _CFG_USE_LP_
						dsiCmdParam[0] = 0x0;
						dsi_LPWriteCmd(DSI_DATA_LANE_0, dcsCtrlContext.uiDCSCmd, dsiCmdParam, 1, DSI_SHORT_PACKET, bEoPT);
#endif
					} else {
						dcsCtrlContext.uiPacketType = DSI_LONG_PACKET;
						dcsCtrlContext.uiDataType = DATA_TYPE_LONG_WRITE;
						dcsCtrlContext.uiParam = (UINT8 *)&dsiCmdParam;
#if _CFG_USE_LP_
						dsiCmdParam[0] = 0x0;
						dsi_LPWriteCmd(DSI_DATA_LANE_0, dcsCtrlContext.uiDCSCmd, dsiCmdParam, dcsCtrlContext.uiParamCnt, DSI_LONG_PACKET, bEoPT);
#endif
					}
					/*
					else
					{
					    DSI_ERR_MSG("Max 8 param \r\n");
					}
					*/
#if !_CFG_USE_LP_
					dcsContext.pDSICmdCtx = &dcsCtrlContext;
					dsi_setHSDcsCommand(&dcsContext);
#endif
				}
				//dispdev_writeLcdReg(pMode->pCmdQueue[i].uiAddress, pMode->pCmdQueue[i].uiValue);
				i += (dcsCtrlContext.uiParamCnt + 1);
			}
		}
	}

	dsi_close();


	pDispDevControl(DISPDEV_IOCTRL_GET_SRCCLK, &DevIOCtrl);

	switch (DevIOCtrl.SEL.GET_SRCCLK.SrcClk) {
	case DISPCTRL_SRCCLK_PLL1:
	case DISPCTRL_SRCCLK_IDE2_PLL1: {
			SrcPLL = PLL_ID_1;
		}
		break;

	case DISPCTRL_SRCCLK_PLL2:
	case DISPCTRL_SRCCLK_IDE2_PLL2: {
			SrcPLL = PLL_ID_2;
		}
		break;

	case DISPCTRL_SRCCLK_PLL4:
	case DISPCTRL_SRCCLK_IDE2_PLL4: {
			SrcPLL = PLL_ID_4;
		}
		break;

	case DISPCTRL_SRCCLK_PLL14:
	case DISPCTRL_SRCCLK_IDE2_PLL14: {
			SrcPLL = PLL_ID_14;
		}
		break;
	default:
		DBG_ERR("Unknow src clock = %d\r\n", DevIOCtrl.SEL.GET_SRCCLK.SrcClk);
		return E_SYS;
		break;
	}

	if (gDispDev_SrcFreq != 0) {
		DBG_IND("DEVDSI: Chg PLL%d from (%d)MHz to(%d)MHz\r\n", (SrcPLL + 1), pll_getPLLFreq(SrcPLL) / 1000000, gDispDev_SrcFreq);
		pll_setPLLEn(SrcPLL, FALSE);
		pll_setPLL(SrcPLL, (gDispDev_SrcFreq * 131072 / 12));
		pll_setPLLEn(SrcPLL,    TRUE);
		DBG_IND("DEVDSI: Chg PLL%d to(%d)Mhz done\r\n", (SrcPLL + 1), pll_getPLLFreq(SrcPLL) / 1000000);
	}
	dispanl_debug(("^GcloseIFDsi Done\r\n"));
	return E_OK;
}

/*
    Write LCD Register

    Write LCD Register

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.
    @param[in] uiValue  Assigned LCD Register Value

    @return void
*/
static void dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue)
{
	//#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	//DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl1;
	//#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	//DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl2;
	//#endif
	//dispdev_writeToLcdMi(pDispDevControl, uiAddr, uiValue);
}

/*
    Read LCD Register

    Read LCD Register

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.

    @return Read back LCD Register Value
*/
static UINT32  dispdev_readLcdReg(UINT32 uiAddr)
{
	UINT32                  uiValue = 0x0;
	//#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	//DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl1;
	//#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	//DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl2;
	//#endif

	//dispdev_readFromLcdMi(pDispDevControl, uiAddr, &uiValue);
	return uiValue;
}

/*
    PreGet LCD size

    PreGet LCD size

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.
    @param[in] uiValue  Assigned LCD Register Value

    @return void
*/
static ER dispdev_getLcdDsiSize(DISPDEV_GET_PRESIZE *tSize)
{
	//UINT32                  i;
	tLCD_PARAM              *pMode      = NULL;
	tLCD_PARAM              *pConfigMode = NULL;
	PINMUX_LCDINIT          LcdMode;
	UINT32                  ModeNumber;
	PINMUX_FUNC_ID          PinFuncID;
	//DISPDEV_IOCTRL_PARAM    DevIOCtrl;
#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl1;
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	DISPDEV_IOCTRL          pDispDevControl = pIFDsiIoControl2;
#endif


	dispanl_debug(("^GgetLcdSize START\r\n"));

	if (pDispDevControl == NULL) {
		return E_PAR;
	}

	tSize->uiBufWidth = 0;
	tSize->uiBufHeight = 0;

	//
	//  Sortig the SysInit Config Mode support
	//
#if (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	PinFuncID = PINMUX_FUNC_ID_LCD;
#elif ((DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2))
	PinFuncID = PINMUX_FUNC_ID_LCD2;
#endif
	LcdMode = pinmux_getDispMode(PinFuncID);
	pConfigMode = dispdev_getConfigModeDSI(&ModeNumber);
#if 0
	if ((pConfigMode != NULL) && (ModeNumber > 0)) {
		for (i = 0; i < ModeNumber; i++) {
			if (pConfigMode[i].Panel.LCDMode == LcdMode) {
				pMode = (tLCD_PARAM *)&pConfigMode[i];
				break;
			}
		}
	}
#else
	if (pConfigMode != NULL) {
		pMode = (tLCD_PARAM *)&pConfigMode[0];
	}
#endif
	if (pMode == NULL) {
		DBG_ERR("LCDMode=%d not support\r\n", LcdMode);
		return E_NOSPT;
	} else {
		tSize->uiBufWidth = pMode->Panel.uiBufferWidth;
		tSize->uiBufHeight = pMode->Panel.uiBufferHeight;
	}
	return E_OK;
}



/*
    Set TV Display Device IOCTRL function pointer
*/
static void dispdev_setIFDsiIOCTRL(FP pIoCtrlFunc)
{
#if (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
	pIFDsiIoControl1 = (DISPDEV_IOCTRL)pIoCtrlFunc;
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
	pIFDsiIoControl2 = (DISPDEV_IOCTRL)pIoCtrlFunc;
#endif
}

/*
    Set LCD Rotation
*/
static ER dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot)
{
	//UINT32          ModeNo,index;
	//tLCD_ROT        *pCmdRot=NULL;
	//BOOL            bValid = FALSE;

	return E_NOSPT;
#if 0
	pCmdRot = dispdev_getLcdRotateDSICmd(&ModeNo);

	// Search Valid Rotattion Config table index
	for (index = 0; index < ModeNo; index++) {
		if (pCmdRot[index].Rotation == Rot) {
			bValid = TRUE;
			break;
		}
	}

	if (bValid == FALSE) {
		DBG_ERR("DispDev NO valid rotate!\r\n\r\n");
		return E_PAR;
	}

	//mi_setEn(FALSE);
	//Delay_DelayMs(40);
	//mi_setManual(TRUE);

#if 0
	if (pCmdRot[index].pCmdQueue != NULL) {
		for (i = 0; i < pCmdRot[index].nCmd; i++) {
			if (pCmdRot[index].pCmdQueue[i].uiAddress == CMDDELAY_MS) {
				Delay_DelayMs(pCmdRot[index].pCmdQueue[i].uiValue);
			} else if (pCmdRot[index].pCmdQueue[i].uiAddress == CMDDELAY_US) {
				Delay_DelayUs(pCmdRot[index].pCmdQueue[i].uiValue);
			} else {
				dispdev_writeLcdReg(pCmdRot[index].pCmdQueue[i].uiAddress, pCmdRot[index].pCmdQueue[i].uiValue);
			}
		}
	}

	dispdev_setMiAutoCmd();

	mi_setManual(FALSE);
	mi_setEn(TRUE);
#endif
	return E_OK;
#endif
}



#if   (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD1)
/**
    Get LCD 1 Device Object

    Get LCD 1 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd1DevObj(void)
{
	return &DispDev_IFDsiObj_Lcd1;
}
#elif (DISPLCDSEL_IFDSI_TYPE == DISPLCDSEL_IFDSI_LCD2)
/**
    Get LCD 2 Device Object

    Get LCD 2 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd2DevObj(void)
{
	return &DispDev_IFDsiObj_Lcd2;
}
#endif

