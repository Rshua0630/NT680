/**
    MIPI-DSI Controller global header

    MIPI-DSI Controller global header

    @file       dsi.h
    @ingroup    mIDrvDisp_DSI
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DSI_H
#define _DSI_H

/**
    @addtogroup mIDrvDisp_DSI
*/
//@{

/**
    @name   Host to peripheral packet data types

*/
//@{
#define DATA_TYPE_SHORT_READ_NO_PARAM           0x06    ///< packet size short, no parameter.
#define DATA_TYPE_GENERIC_SHORT_READ_2_PARAM    0x24    ///< By manufacturer.
#define DATA_TYPE_SHORT_WRITE_NO_PARAM          0x05    ///< Short packet write packet size short, no parameter.
#define DATA_TYPE_SHORT_WRITE_1_PARAM           0x15    ///< Short packet write packet size short, 1 parameter.
#define DATA_TYPE_SET_MAX_RT_PKT_SIZE           0x37    ///< Set Max. return packet size.
#define DATA_TYPE_LONG_WRITE                    0x39    ///< Long packet write.
#define DATA_TYPE_PACKET_STREAM_RGB_565_PACKED  0x0E    ///< Packet stream RGB 565 packed.
#define DATA_TYPE_PACKET_STREAM_RGB_666_PACKED  0x1E    ///< Packet stream RGB 666 packed.
#define DATA_TYPE_PACKET_STREAM_RGB_666_LOOSELY 0x2E    ///< Packet stream RGB 666 loosely.
#define DATA_TYPE_PACKET_STREAM_RGB_888_PACKED  0x3E    ///< Packet stream RGB 888 packed.
//@}

/**
    @name   Ultra low power state entry command pattern

    @note for dsi_setEscapeEntry()
*/
//@{
#define ULPS_ENTRY_CMD_LPDT                     0x87    ///< Low power data transmission
#define ULPS_ENTRY_CMD_ULPS                     0x78    ///< Enter ultra Low power state
#define ULPS_ENTRY_CMD_RESET                    0x46    ///< reset trigger
#define ULPS_ENTRY_CMD_TE                       0xBA    ///< Tearing effect
#define ULPS_ENTRY_CMD_ACK                      0x12    ///< Ack
//@}

/**
    @name   Display command Set (DCS)

*/
//@{
#define DCS_CMD_SW_RESET                        0x01    ///< Power for the display panel is on.
#define DCS_CMD_EXIT_SLEEP_MODE                 0x11    ///< Power for the display panel is on.
#define DCS_CMD_SET_DISPLAY_ON                  0x29    ///< Show the image on the display device.
#define DCS_CMD_SET_ALL_PIX_ON                  0x23    ///< Set all pixel on.
#define DCS_CMD_SET_ALL_PIX_OFF                 0x22    ///< Set all pixel off.
#define DCS_CMD_MEMORY_ACCESS_CTRL              0x36    ///< Show the image on the display device.
#define DCS_CMD_MEMORY_WRITE                    0x2C    ///< memory write.
#define DCS_CMD_MEMORY_WRITE_CONT               0x3C    ///< memory write continue.
#define DCS_CMD_COLUMN_ADDR_SET                 0x2A    ///< set column address
#define DCS_CMD_GAMMA_SET                       0x26    ///< set gamma
#define DCS_CMD_NULL_PKT                        0x09    ///< set null packet
#define DCS_CMD_READ_ID0                        0x04    ///< read id0
#define DCS_CMD_READ_PWR_MODE                   0x0A    ///< read power mode
#define DCS_CMD_READ_ID1                        0xDA    ///< read id1
#define DCS_CMD_READ_ID2                        0xDB    ///< read id2
#define DCS_CMD_READ_ID3                        0xDC    ///< read id3
#define DCS_CMD_READ_STATUS                     0xF2    ///< read status
#define DCS_CMD_SET_TE_OFF                      0x34    ///< set TE off
#define DCS_CMD_SET_TE_ON                       0x35    ///< set TE on
//@}

/**
    DSI mode select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_MODE_MANUAL_MODE = 0,   ///< Manual Commands Only: Send CMD(s) and auto clear DSI_TX_EN
	DSI_MODE_AUTO_MODE1,        ///< Send single frame only and auto clear DSI_TX_EN at frame end (No commands)
	DSI_MODE_AUTO_MODE2,        ///< Auto Mode 2: FRM1 + FRM2 + ¡K
	DSI_MODE_AUTO_MODE3,        ///< Auto Mode 3: CMD(s) + FRM1 + CMD(s) + FRM2 + ...

	DSI_MODE_CNT,

	ENUM_DUMMY4WORD(DSI_MODESEL)
} DSI_MODESEL;

/**
    DSI pixel format select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_PIXEL_RGB_565 = 0,      ///< 16 bits / pixel
	DSI_PIXEL_RGB_666_PACKETED, ///< 18 bits / pixel, size should multiple of 9 bytes
	DSI_PIXEL_RGB_666_LOOSELY,  ///< 18 bits / pixel, size should multiple of 3 bytes
	DSI_PIXEL_RGB_888,          ///< 24 bits / pixel

	DSI_PIXEL_FMT_CNT,
	ENUM_DUMMY4WORD(DSI_PIXEL_FORMATSEL)
} DSI_PIXEL_FORMATSEL;


/**
    DSI pixel packet mode select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_PIXMODE_VIDEO_MODE = 0, ///< video   mode (without framebuffer)
	DSI_PIXMODE_COMMAND_MODE,   ///< command mode (with framebuffer)

	DSI_PIXMODE_CNT,
	ENUM_DUMMY4WORD(DSI_PIXPKT_MODESEL)
} DSI_PIXPKT_MODESEL;


/**
    DSI video packet type select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_VIDEOPKT_TYPE_SYNC_PULSE = 0,   ///< Sync pulse
	DSI_VIDEOPKT_TYPE_SYNC_EVENT,       ///< Sync evnet, Burst mode

	ENUM_DUMMY4WORD(DSI_VIDEOPKT_TYPESEL)
} DSI_VIDEOPKT_TYPESEL;

/**
    DSI Lane select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_DATA_LANE_0 = 0,        ///< Data lane 0
	DSI_DATA_LANE_1,            ///< Data lane 1
	DSI_DATA_LANE_2,            ///< Data lane 2
	DSI_DATA_LANE_3,            ///< Data lane 3

	DSI_DATA_LANE_CNT,

	ENUM_DUMMY4WORD(DSI_LANESEL)
} DSI_LANESEL;


/**
    DSI CLK ULP select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_ULP_EXIT = 0,           ///< Clock lane exit ULPS
	DSI_ULP_ENTER = 1,          ///< Clock lane enter ULPS
	DSI_ULP_SEL_CNT,

	ENUM_DUMMY4WORD(DSI_ULP_SEL)
} DSI_ULP_SEL;


/**
    DSI escape control operation select

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_ESCAPE_TRIGGER = 0,      ///< Escape command trigger
	DSI_ESCAPE_START,            ///< Escape command start procedure
	DSI_ESCAPE_STOP,             ///< Escape command stop procedure
	ENUM_DUMMY4WORD(DSI_ESC_OP)
} DSI_ESC_OP;


/**
    DSI escape control operation stop action selection

    @note for dsi_setConfig()
*/
typedef enum {
	DSI_SET_ESC_NOT_STOP = 0x0,         ///< Escape command not stop
	DSI_SET_ESC_STOP_WITH_ESC_CMD,      ///< Escape command stop but not send exit cmd
	DSI_SET_ESC_STOP_WITHOUT_ESC_CMD,   ///< Escape command stopp and send exit cmd

	ENUM_DUMMY4WORD(DSI_CFG_ESCAPE_CMD_STOP_TYPE)
} DSI_CFG_ESCAPE_CMD_STOP_TYPE;

/**
    DSI  input source

    DSI  input source, used for dsi_setConfig(DSI_CONFIG_ID_SRC, SRC_IDE).
*/
typedef enum {
	DSI_SRC_IDE = 0x0,  ///< From IDE
	DSI_SRC_IDE2,       ///< From IDE2

	ENUM_DUMMY4WORD(DSI_SRC)
} DSI_SRC;


/**
    DSI chip version

    @note for dsi_getConfig()
*/
#define DSI_DRV_CHIPVER_A           0x0
#define DSI_DRV_CHIPVER_B           0x1
#define DSI_DRV_CHIPVER_C           0x2

/**
    DSI Command control

    Set DSI Command control
*/
typedef struct {
	UINT32  uiDataType;                 ///< Data type
	UINT32  uiVirtualChannel;           ///< virtual channel id

	UINT32  uiPacketType;               ///< packet type
	UINT32  uiDCSCmd;                   ///< DSC commnd
	UINT8    *uiParam;                  ///< point to parameter
	UINT32  uiParamCnt;                 ///< parmeter count
} DSI_CMD_CTRL_PARAM, *PDSI_CMD_CTRL_PARAM;

/**
    DSI Command R/W control

    Set DSI Command R/W control
*/
typedef struct {
	UINT32                  uiCmdNo;    ///< commnad number
	BOOL                    ubBTAEN;    ///< BTA enable/disable
	BOOL                    ubBTAOnly;  ///< issue BTA only
	UINT32                  uiSramOfs;  ///< set the sram offset
	BOOL                    ubEOTEN;    ///< set the EOT enable or not
	PDSI_CMD_CTRL_PARAM     pDSICmdCtx; ///< set commnd control
} DSI_CMD_RW_CTRL_PARAM, *PDSI_CMD_RW_CTRL_PARAM;

/**
    DSI Functional Configuration Selection

    This definition is used in dsi_setConfig()/dsi_getConfig() specify which of the dsi function is selected
    to assign new configurations.
*/
typedef enum {
	DSI_CONFIG_ID_MODE,         ///< Configure DSI mode
	///< @note for DSI_CFG_MODE_SEL
	///< Context can be any of:
	///< - @b DSI_MODE_MANUAL_MODE  : Manual Commands Only: Send CMD(s) and auto clear DSI_TX_EN
	///< - @b DSI_MODE_AUTO_MODE1   : Send single frame only and auto clear DSI_TX_EN at frame end (No commands)
	///< - @b DSI_MODE_AUTO_MODE2   : Auto Mode 2: FRM1 + FRM2 + ...
	///< - @b DSI_MODE_AUTO_MODE3   : Auto Mode 3: CMD(s) + FRM1 + CMD(s) + FRM2 + ...

	DSI_CONFIG_ID_PIXEL_FMT,    ///< Configure DSI pinxel format. Use DSI_PIXEL_RGB_888/DSI_PIXEL_RGB_565/DSI_PIXEL_RGB_666_PACKETED/DSI_PIXEL_RGB_666_LOOSELY
	DSI_CONFIG_ID_PIXPKT_MODE,  ///< Configure DSI pinxel mode.
	DSI_CONFIG_ID_VDOPKT_TYPE,  ///< Configure DSI video packet type.
	DSI_CONFIG_ID_ECC_CHK_EN,   ///< Configure DSI Ecc check enable/disable.
	DSI_CONFIG_ID_FRMEND_BTA_EN,  ///< Configure DSI frameend BTA enable/disable.
	DSI_CONFIG_ID_EOT_PKT_EN,   ///< Configure DSI EOT packet enable/disable
	DSI_CONFIG_ID_BLANK_CTRL,   ///< Configure DSI  blank control
	DSI_CONFIG_ID_INTER_PKT_LP, ///< Configure DSI inter-packet enter LP or not.
	DSI_CONFIG_ID_CLK_LP_CTRL,  ///< Configure DSI clock enter LP or not.
	DSI_CONFIG_ID_SYNC_EN,      ///< Configure DSI sync enable/disable.
	DSI_CONFIG_ID_SYNC_SRC,     ///< Configure DSI sync source.
	DSI_CONFIG_ID_SYNC_WITH_SETTEON,  ///< Configure DSI sync with set te on.
	DSI_CONFIG_ID_SYNC_WITH_SETTEON_RTY, ///< Configure DSI sync with set te on retry.
	DSI_CONFIG_ID_RGB_SWAP,     ///< Configure DSI RGB swap
	DSI_CONFIG_ID_RGB_BITFLIP,  ///< Configure DSI  RGB bit swap
	DSI_CONFIG_ID_SYNC_WITH_SETTEON_RTY_TWICEBTA,  //< Configure DSI sync with set te on retry and BTA twice.
	DSI_CONFIG_ID_DATALANE_NO,  ///< Configure DSI  data lane number.
	DSI_CONFIG_ID_SRC,          ///< Configure DSI souce from IDE or IDE2

	DSI_CONFIG_ID_PIXPKT_PH_DT, ///< Configure DSI pixel Packet header DataType.
	DSI_CONFIG_ID_PIXPKT_PH_VC, ///< Configure DSI pixel Packet header VirtualChannel.
	DSI_CONFIG_ID_DCS_CT0,      ///< Configure DSI DSC command, this field would be inserted in front of the first pixel packet.
	DSI_CONFIG_ID_DCS_CT1,      ///< Configure DSI DSC command,  this field would be inserted in front of the pixel packet except the first pixel packet.

	DSI_CONFIG_ID_SYNCEVT_SLICE_NO, ///< Configure DSI sync event slice number.
	DSI_CONFIG_ID_SYNCEVT_NULL_LEN, ///< Configure DSI sync even null length. (Byte count)

	DSI_CONFIG_ID_VSA,          ///< Configure DSI vertical sync active timing.
	DSI_CONFIG_ID_VTOTAL,       ///< Configure DSI vertical total timing.

	DSI_CONFIG_ID_VVALID_START, ///< Configure DSI vertical valid start timing.
	DSI_CONFIG_ID_VVALID_END,   ///< Configure DSI vertical valid end timing.

	DSI_CONFIG_ID_HSA,          ///< Configure DSI horizontal sync active timing.
	DSI_CONFIG_ID_BLLP,         ///< Configure DSI BLLP period, this period can trasmit HS packets or entering LP11.

	DSI_CONFIG_ID_HBP,          ///< Configure DSI horizontal back porch period.
	DSI_CONFIG_ID_HFP,          ///< Configure DSI horizontal front  porch period.

	DSI_CONFIG_ID_HACT,         ///< Configure DSI horizontal active period.

	DSI_CONFIG_ID_TLPX,         ///< Configure DSI LTPX timing.
	DSI_CONFIG_ID_BTA_TA_GO,    ///< Configure DSI TA_GO timing.
	DSI_CONFIG_ID_BTA_TA_SURE,  ///< Configure DSI TA_SURE timing.
	DSI_CONFIG_ID_BTA_TA_GET,   ///< Configure DSI TA_GET timing.
	DSI_CONFIG_ID_THS_PREPARE,  ///< Configure DSI THS_PREPARE timing.
	DSI_CONFIG_ID_THS_ZERO,     ///< Configure DSI THS_ZERO timing.
	DSI_CONFIG_ID_THS_TRAIL,    ///< Configure DSI THS_TRAIL timing.
	DSI_CONFIG_ID_THS_EXIT,     ///< Configure DSI THS_EXIT timing.
	DSI_CONFIG_ID_TWAKEUP,      ///< Configure DSI wakeup timing.
	DSI_CONFIG_ID_TCLK_PREPARE, ///< Configure DSI TCLK_PREPARE timing.
	DSI_CONFIG_ID_TCLK_ZERO,    ///< Configure DSI TCLK_ZERO timing.
	DSI_CONFIG_ID_TCLK_POST,    ///< Configure DSI TCLK_POST timing.
	DSI_CONFIG_ID_TCLK_PRE,     ///< Configure DSI TCLK_PRE timing.
	DSI_CONFIG_ID_TCLK_TRAIL,   ///< Configure DSI TCLK_TRAIL timing.

	DSI_CONFIG_ID_BTA_TMOUT_VAL,      ///< Configure DSI BTA timeout.
	DSI_CONFIG_ID_BTA_HANDSK_TMOUT_VAL,  ///< Configure DSI BTA handshake timeout.

	DSI_CONFIG_ID_SYNC_POL,     ///< Configure DSI SYNC polarity.
	DSI_CONFIG_ID_SYNC_SEL,     ///< Configure DSI SYNC faster or slower than peripheral selection.
	DSI_CONFIG_ID_SYNC_DLY_CNT, ///< Configure DSI SYNC delay count.
	DSI_CONFIG_ID_TE_BTA_INTERVAL, ///< Configure DSI TE BAT issue interval.

	DSI_CONFIG_ID_PHY_DRVING,   ///< Configure DSI PHY driving

	DSI_CONFIG_ID_CLK_PHASE_OFS,   ///< Configure DSI PHY clock phase offset.
	DSI_CONFIG_ID_DAT0_PHASE_OFS,  ///< Configure DSI PHY data0 phase offset.
	DSI_CONFIG_ID_DAT1_PHASE_OFS,  ///< Configure DSI PHY data1 phase offset.
	DSI_CONFIG_ID_DAT2_PHASE_OFS,  ///< Configure DSI PHY data2 phase offset.
	DSI_CONFIG_ID_DAT3_PHASE_OFS,  ///< Configure DSI PHY data3 phase offset.
	DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, ///< Configure DSI PHY phase delay enable.

	//DSI_CFG_ID_FREQ
	DSI_CONFIG_ID_FREQ,         ///< DSI module target clock (Unit: Hz)
	DSI_CONFIG_ID_LPFREQ,       ///< DSI module LP clock (Unit: Hz)
	DSI_CONFIG_ID_IDEHVALID,    ///< DSI module of IDE HVALID setting

	DSI_CONFIG_ID_LANSEL_D0,    ///< Configure DSI DAT0 Lane mapping
	DSI_CONFIG_ID_LANSEL_D1,    ///< Configure DSI DAT1 Lane mapping
	DSI_CONFIG_ID_LANSEL_D2,    ///< Configure DSI DAT2 Lane mapping
	DSI_CONFIG_ID_LANSEL_D3,    ///< Configure DSI DAT3 Lane mapping

	DSI_CONFIG_ID_PHY_LP_RX_DAT0,///< Configure DSI LP RX DAT0 enable/disable

	DSI_CONFIG_ID_CHIP_VER,     ///< Get DSI chip version
	ENUM_DUMMY4WORD(DSI_CONFIG_ID)
} DSI_CONFIG_ID;


#if 0
typedef enum {
	DSI_CFG_MODE_CMD_ONLY,
	DSI_CFG_MODE_SINGLE_FRM,
	DSI_CFG_MODE_MULTI_FRM,
	DSI_CFG_MODE_MULTI_CMD_FRM,

	ENUM_DUMMY4WORD(DSI_CFG_MODE_SEL)
} DSI_CFG_MODE_SEL;


typedef enum {
	DSI_CFG_PIXEL_FMT_RGB565,
	DSI_CFG_PIXEL_FMT_RGB666_PACKED,
	DSI_CFG_PIXEL_FMT_RGB666_LOOSELY,
	DSI_CFG_PIXEL_FMT_RGB888,

	ENUM_DUMMY4WORD(DSI_CFG_PIXEL_FMT_SEL)
} DSI_CFG_PIXEL_FMT_SEL;


typedef enum {
	DSI_CFG_PIXPKT_VIDEO_MODE,
	DSI_CFG_PIXPKT_COMMAND_MODE,

	ENUM_DUMMY4WORD(DSI_CFG_PIXPKT_MODE_SEL)
} DSI_CFG_PIXPKT_MODE_SEL;


typedef enum {
	DSI_CFG_VDOPKT_SYNCPULSE_TYPE,
	DSI_CFG_VDOPKT_SYNCEVENT_TYPE,

	ENUM_DUMMY4WORD(DSI_CFG_VDOPKT_TYPE_SEL)
} DSI_CFG_VDOPKT_TYPE_SEL;
#endif

// -----------------------------------------------------------------------------
// DSI Command RW control (0x2C)
// -----------------------------------------------------------------------------
/**
    DSI command RW control configuration

    @note for dsi_setCmdRWCtrl()
*/
typedef enum {
	DSI_SET_CMD_NUMBER = 0x0,           ///< The number of (DSI_CMD_NUMBER+1) commands would be sent in DSI_MODE= 0/3/4.
	///< Context is
	///< - @b UINT32 : 1 - 8
	DSI_SET_BTA_EN,                     ///< Enable the Bus Turn Around (BTA) process
	///< after the final commands are sent.
	///< The READ command should put at the final commands,
	///< and the data would be read back after BTA.
	///< This field is valid for DSI_MODE = 0/3/4.
	///< Context is
	///< - @b UINT32 : 1 : Enable
	///< - @b UINT32 : 0 : Disable
	DSI_SET_BTA_ONLY,                   ///< Context is
	///< - @b UINT32 : 1 : Issue BTA only
	///< - @b UINT32 : 0 : Disable
	DSI_SET_SRAM_READ_OFS,              ///< Context is
	///< - @b UINT32 : Sram offset 0~255 (256 bytes total)
	DSI_GET_SRAM_READ_CNT,              ///< Context is
	///< - @b UINT32*: Sram offset 0~256


	DSI_RW_CMD_CTRL_CNT,
	ENUM_DUMMY4WORD(DSI_CFG_CMD_RW_CTRL)
} DSI_CFG_CMD_RW_CTRL;


// -----------------------------------------------------------------------------
// DSI Command Register 0,1 (0x30,0x34)
// -----------------------------------------------------------------------------
/**
    DSI packet type

    @note for dsi_setCmdRegister()
*/
//@{
typedef enum {
	DSI_SHORT_PACKET = 0x0,             ///< This DSI short packet
	DSI_LONG_PACKET,                    ///< This DSI long packet

	DSI_PT_CNT,
	ENUM_DUMMY4WORD(DSI_PACKET_TYPE)
} DSI_PACKET_TYPE;
//@}

/**
    DSI command register set

    @note for dsi_setCmdRegister()
*/
typedef enum {
	DSI_CMD_REG0 = 0x0,                 ///< Command register 0
	DSI_CMD_REG1,                       ///< Command register 1
	DSI_CMD_REG2,                       ///< Command register 2
	DSI_CMD_REG3,                       ///< Command register 3
	DSI_CMD_REG4,                       ///< Command register 4
	DSI_CMD_REG5,                       ///< Command register 5
	DSI_CMD_REG6,                       ///< Command register 6
	DSI_CMD_REG7,                       ///< Command register 7

	DSI_CMD_SET_CNT,
	ENUM_DUMMY4WORD(DSI_CMD_REG_NUM)
} DSI_CMD_REG_NUM;


/**
    DSI command register configuration

    @note for dsi_setCmdRegister()
*/
typedef enum {
	DSI_SET_CMD_DT = 0x0,                   // The command n Data Type in the Data Identification (DI) field of the DSI packet header.
	DSI_SET_CMD_VC,                         // The virtual channel ID for the command n
	DSI_SET_CMD_WC,                         // If the command n is the Short Packet, this field is the command data for this short packet command.
	// If the command n is the Long Packet, this field is the length for this long packet command.
	// The long packet command data is stored in the SRAM in serially by commands. The command data n is from the SRAM address offset 0.

	DSI_SET_CMD_DATA = DSI_SET_CMD_WC,
	DSI_SET_CMD_PT,                         // The Packet Type of the command n.
	// 0: Short Packet
	// 1: Long Packet
	ENUM_DUMMY4WORD(DSI_CFG_CMD_REG)
} DSI_CFG_CMD_REG;

extern ER       dsi_open(void);
extern ER       dsi_close(void);
extern BOOL     dsi_isOpened(void);
extern ER       dsi_setTXEN(BOOL ubEN, BOOL ubWait);
extern ER       dsi_waitTXDone(void);
extern ER       dsi_waitFrameEnd(void);
extern ER       dsi_issueBTA(void);
extern ER       dsi_ULPSTrigger(void);
extern UINT32   dsi_getErrorReport(void);
extern ER       dsi_setLPSClockSel(DSI_ULP_SEL uiULPSel);


extern ER       dsi_setHSDcsCommand(PDSI_CMD_RW_CTRL_PARAM pDcsCtx);
extern ER       dsi_setConfig(DSI_CONFIG_ID CfgID, UINT32 uiConfig);
extern ER       dsi_setCmdRWCtrl(DSI_CFG_CMD_RW_CTRL uiCmd, UINT32 param);
extern ER       dsi_setLPDcsCommand(DSI_LANESEL uiDataLane, UINT32 uiLPCmd);
extern ER       dsi_setEscapeEntry(DSI_LANESEL uiDataLane, UINT32 uiEntryCmd, BOOL bStop);
extern ER       dsi_setEscapeControl(DSI_LANESEL uiDataLane, DSI_ESC_OP uiEscOp, BOOL ubEN);
extern ER       dsi_setCmdRegister(DSI_CMD_REG_NUM uiCmdRegNo, DSI_CFG_CMD_REG uiCmdReg, UINT32 param);
extern ER       dsi_setEscapeTransmission(DSI_LANESEL uiDataLane, UINT32 uiCmd, DSI_CFG_ESCAPE_CMD_STOP_TYPE exit);
extern BOOL     dsi_getPhaseDelayInfo(UINT32 *puiClkPhase, UINT32 *puiD0Phase, UINT32 *puiD1Phase);

extern UINT32   dsi_getConfig(DSI_CONFIG_ID ConfigID);




//@}

#endif
