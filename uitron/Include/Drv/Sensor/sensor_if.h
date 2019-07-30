/**
    @file       sensor_if.h

    @ingroup    mIDrvSensor
    @brief      Define sensor internal function
    @note      Sensor interface APIs

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _SENSOR_IF_H
#define _SENSOR_IF_H
// sensor
#include "sensor.h"
// Driver_Sys
#include "lvds.h"
#include "csi.h"
#include "vx1.h"
#include "pll.h"
#include "slvsec.h"
#include "senphy.h"
// Driver_IPP
#include "sie1_lib.h"
#include "sie2_lib.h"
#include "sie3_lib.h"
#include "sie4_lib.h"
#include "sie5_lib.h"
#include "sie6_lib.h"
#include "sie7_lib.h"
#include "sie8_lib.h"
#include "tge_lib.h"
// system
#include "stdio.h"


typedef enum {
	SENSOR_TGE_CHGPARAM_VD_PHASE,
	SENSOR_TGE_CHGPARAM_HD_PHASE,

	SENSOR_TGE_CHGPARAM_VD_INV,
	SENSOR_TGE_CHGPARAM_HD_INV,

	SENSOR_TGE_CHGPARAM_VD_PERIOD,
	SENSOR_TGE_CHGPARAM_VD_ASSERT,
	SENSOR_TGE_CHGPARAM_VD_FRONTBLANK,
	SENSOR_TGE_CHGPARAM_HD_PERIOD,
	SENSOR_TGE_CHGPARAM_HD_ASSERT,
	SENSOR_TGE_CHGPARAM_HD_CNT,

	SENSOR_TGE_CHGPARAM_PAUSE,
	SENSOR_TGE_CHGPARAM_MODE,
	SENSOR_TGE_CHGPARAM_BREAKPOINT,

	ENUM_DUMMY4WORD(SENSOR_TGE_CHGPARAM_SEL)
} SENSOR_TGE_CHGPARAM_SEL;

typedef enum {
	SENSOR_TGE_WAIT_EVENT_VD,
	SENSOR_TGE_WAIT_EVENT_BP,

	ENUM_DUMMY4WORD(SENSOR_TGE_WAIT_EVENT_SEL)
} SENSOR_TGE_WAIT_EVENT_SEL;

typedef UINT32(*SEN_SERIAL_ORDER_FP)(SENSOR_ID, UINT32);
typedef struct {
	ER(*open)(SENSOR_ID id);                                                            ///< lvds module driver open
	ER(*close)(SENSOR_ID id);                                                           ///< lvds module driver close
	BOOL (*is_opened)(SENSOR_ID id);                                                     ///< check lvds module driver is opened

	ER(*set_enable)(SENSOR_ID id, BOOL b_enable);                                                   ///< set lvds module enable/disable
	BOOL (*get_enable)(SENSOR_ID id);                                                    ///< get lvds module enable/disable
	ER(*enable_streaming)(SENSOR_ID id);                                                 ///< lvds module enable streaming

	void (*set_config)(SENSOR_ID id, LVDS_CONFIG_ID config_id, UINT32 ui_cfg_value);              ///< set lvds module configurations
	UINT32(*get_config)(SENSOR_ID id, LVDS_CONFIG_ID config_id);                                ///< get lvds module configurations
	void (*set_ch_config)(SENSOR_ID id, LVDS_IN_VALID  valid_lane, LVDS_CH_CONFIG_ID config_id, UINT32 ui_cfg_value);
	void (*set_syncword)(SENSOR_ID id, UINT32 sync_word_length, UINT32 *p_sync_word);              ///< set lvds data sync words.
	void (*set_padpin)(SENSOR_ID id, UINT32 *output_pixel_order, SEN_SERIAL_ORDER_FP data_order_fp, SEN_SERIAL_ORDER_FP valid_lane_fp);

	LVDS_INTERRUPT(*wait_interrupt)(SENSOR_ID id, LVDS_INTERRUPT waited_flag);                  ///< wait lvds interrupt event
} SENSOR_IF_LVDS;

typedef struct {
	ER(*open)(SENSOR_ID id);                                                            ///< csi module driver open
	ER(*close)(SENSOR_ID id);                                                           ///< csi module driver close
	BOOL (*is_opened)(SENSOR_ID id);                                                     ///< check csi module driver is opened

	ER(*set_enable)(SENSOR_ID id, BOOL b_enable);                                                   ///< set csi module enable/disable
	BOOL (*get_enable)(SENSOR_ID id);                                                    ///< get csi module enable/disable

	void (*set_config)(SENSOR_ID id, CSI_CONFIG_ID config_id, UINT32 ui_cfg_value);              ///< set csi module configurations
	UINT32(*get_config)(SENSOR_ID id, CSI_CONFIG_ID config_id);                                ///< get csi module configurations

	CSI_INTERRUPT(*wait_interrupt)(SENSOR_ID id, CSI_INTERRUPT waited_flag);                   ///< wait csi interrupt event
} SENSOR_IF_CSI;

typedef struct {
	ER(*open)(SENSOR_ID id);
	ER(*close)(SENSOR_ID id);
	BOOL (*is_opened)(SENSOR_ID id);

	void(*set_enable)(SENSOR_ID id, BOOL b_enable);
	BOOL (*get_enable)(SENSOR_ID id);

	void (*set_config)(SENSOR_ID id, VX1_CONFIG_ID config_id, UINT32 ui_cfg_value);
	UINT32(*get_config)(SENSOR_ID id, VX1_CONFIG_ID config_id);

	VX1_INTERRUPT(*wait_interrupt)(SENSOR_ID id, VX1_INTERRUPT waited_flag);

	ER(*openSublink)(SENSOR_ID id);
	ER(*closeSublink)(SENSOR_ID id);
	BOOL (*isSublinkOpened)(SENSOR_ID id);
	void (*setSublinkConfig)(SENSOR_ID id, VX1SBL_CONFIG_ID ConfigID, UINT32 uiCfgValue);
	UINT32(*getSublinkConfig)(SENSOR_ID id, VX1SBL_CONFIG_ID ConfigID);
	VX1_I2CSTS(*writeI2cDevice)(SENSOR_ID id, UINT32 uiDevAddr, UINT32 uiRegAddr, UINT32 uiValue);
	VX1_I2CSTS(*writeI2cDeviceSequential)(SENSOR_ID id, UINT32 uiDevAddr, UINT32 uiRegStartAddr, UINT32 uiLength, UINT32 uiBufferAddr);
	VX1_I2CSTS(*readI2cDevice)(SENSOR_ID id, UINT32 uiDevAddr, UINT32 uiRegAddr, UINT32 *puiData);
	ER(*initTransmitter)(SENSOR_ID id, VX1_TXMODULE TxModule, VX1_TXINIT Init);
	BOOL (*checkSublinkReady)(SENSOR_ID id);
	ER(*cfgTransmitterGPIO)(SENSOR_ID id, VX1_TXMODULE TxModule, VX1_TXGPIO TxGpio, VX1_GPIODIR GpioDir, BOOL *pbLevel);
	void (*setThcv241PLL)(SENSOR_ID id, UINT8 *pPLL);
	void (*dumpMainlink)(SENSOR_ID id, UINT32 uiPage);
	void (*dumpSublinkMaster)(SENSOR_ID id);
	void (*dumpSublinkSlave)(SENSOR_ID id);
	BOOL (*bet)(SENSOR_ID id, UINT32 uiTestTime);

} SENSOR_IF_VX1; // refer to VX1OBJ

typedef struct {
	ER(*open)(void);
	ER(*close)(void);
	BOOL (*is_opened)(void);
	void (*init)(void);
	ER(*set_enable)(BOOL b_enable);
	BOOL (*get_enable)(void);
	void (*set_config)(SLVSEC_CFG_ID config_id, UINT32 ui_cfg_value);
	UINT32(*get_config)(SLVSEC_CFG_ID config_id);
	void (*wait_interrupt)(SLVSEC_INTERRUPT waited_flag);
} SENSOR_IF_SLVSEC;

typedef struct {
	void(*set_info)(SENSOR_ID id, SEN_TGE tge_info);
	BOOL (*chk_permit)(SENSOR_ID id);
	ER(*open)(SENSOR_ID id, SENSOR_DRV_INFO *sensor_drv_info);
	ER(*close)(SENSOR_ID id);
	BOOL(*is_opened)(void);

	ER(*start)(SENSOR_ID id);
	ER(*pause)(SENSOR_ID id);
	BOOL(*is_pause)(SENSOR_ID id);

	ER(*set_enable)(SENSOR_ID id, BOOL b_enable);
	BOOL(*get_enable)(SENSOR_ID id);

	ER(*chg_param)(SENSOR_ID id, SENSOR_TGE_CHGPARAM_SEL chg_param_sel, UINT32 value);
	ER(*wait_event)(SENSOR_ID id, SENSOR_TGE_WAIT_EVENT_SEL wait_event_sel, BOOL b_clear_flag);
	ER(*reset)(SENSOR_ID id, SENSOR_MULTI_ID multi_id);
} SENSOR_IF_TGE;

//sie table
typedef struct {
	ER(*sie_clk_open_clock)(SENSOR_ID id, SENSOR_MULTI_ID multi_id, UINT32 multi_id_num);
	ER(*sie_clk_close_clock)(SENSOR_ID id, SENSOR_MULTI_ID multi_id, UINT32 multi_id_num);
	ER(*sie_clk_chg_clock_source)(SENSOR_ID id, SENSOR_MULTI_ID multi_id, UINT32 multi_id_num, SIE_CLKSRC_SEL clk_src);
	ER(*sie_clk_chg_clock_rate)(SENSOR_ID id, SENSOR_MULTI_ID multi_id, UINT32 multi_id_num, UINT32 clk_rate);
} SENSOR_IF_SIE;

//pll table
typedef struct {
	ER(*set_clk_en)(SENSOR_ID id, PLL_CLK clk_id, BOOL b_enable);
	ER(*set_clk_freq)(SENSOR_ID id, PLL_CLK clk_id, UINT32 ui_freq);

	// for sensor driver chgmode change sie clk
	ER(*runtime_chg_mclk_freq)(SENSOR_ID id, UINT32 ui_freq);
} SENSOR_IF_PLL;

// internal table
typedef struct {
	UINT32(*get_version)(void);	///< return DRV_CHIPVER_A / DRV_CHIPVER_B / ...
} SENSOR_INT_INFO;

// sensor interface extern api
extern SENSOR_IF_LVDS *sensor_if_get_lvds_ctrl_obj(void);
extern SENSOR_IF_CSI *sensor_if_get_csi_ctrl_obj(void);
extern SENSOR_IF_VX1 *sensor_if_get_vx1_ctrl_obj(void);
extern SENSOR_IF_SIE *sensor_if_get_sie_ctrl_obj(void);
extern SENSOR_IF_TGE *sensor_if_get_tge_ctrl_obj(void);
extern SENSOR_IF_SLVSEC *sensor_if_get_slvsec_ctrl_obj(SENSOR_ID sen_id);
extern SENSOR_IF_PLL *sensor_if_get_pll_ctrl_obj(void);
extern SENSOR_INT_INFO *sensor_get_int_ctrl_obj(void);

// sensor internal convert api
// common
extern BOOL sensor_chk_multiid(SENSOR_ID id, SENSOR_MULTI_ID multi_id);
extern BOOL sensor_chk_modetype_multiframe(SENSOR_MODE_TYPE mode_type);
extern UINT32 sensor_multiid_cnt(SENSOR_MULTI_ID multi_id);
extern ER sensor_chk_mode_multiid_legal(SENSOR_ID id, SENSOR_MULTI_ID multi_id, UINT32 multi_id_cnt, SENSOR_MODE mode, SENSOR_MODE_INFO *mode_info);
extern UINT32 conv_sensor_mode(SENSOR_MODE sen_mode, SENSOR_MODE cur_mode);
extern ER sensor_set_if_map(SENSOR_ID id, SENSOR_MAP_IF sen_map_if);
extern SENSOR_MAP_IF sensor_get_if_map(SENSOR_ID id);
extern ER sensor_reset_if_map(SENSOR_ID id);
extern ER sensor_chk_serial_datalane(SENSOR_ID id, SENSOR_DATA_TYPE data_type, UINT32 *datalane);
extern ER sensor_clear_serial_datalane(SENSOR_ID id);
extern UINT32 sensor_lcm(UINT32 a, UINT32 b);


// lvds
extern LVDS_INTERRUPT sensor_multiid_conv2_lvds_vd(SENSOR_ID id, SENSOR_MULTI_ID multi_id);
extern LVDS_INTERRUPT sensor_multiid_conv2_lvds_fmd(SENSOR_ID id, SENSOR_MULTI_ID multi_id);
extern LVDS_CH_CONFIG_ID sensor_id_conv2_lvds_ch_cfg_vd(SENSOR_ID id);
extern LVDS_CONFIG_ID sensor_id_conv2_lvds_chid_bit(UINT32 bit_ofs);
extern LVDS_CONFIG_ID sensor_id_conv2_lvds_chid(SENSOR_ID id);
extern LVDS_CONFIG_ID sensor_id_conv2_lvds_valid_h(SENSOR_ID id);
extern UINT32 sensor_lvds_pixdepth_2_real(LVDS_PIXDEPTH lvds_pixdepth);
extern UINT32 sensor_lvds_datalane_2_real(LVDS_DATLANE_NO lvds_datalane);

// csi
extern CSI_INTERRUPT sensor_multiid_conv2_csi_fmd(SENSOR_ID id, SENSOR_MULTI_ID multi_id);
extern CSI_CONFIG_ID sensor_id_conv2_csi_virtual_id(SENSOR_ID id);
extern CSI_CONFIG_ID sensor_id_conv2_csi_chid_bit(UINT32 bit_ofs);
extern CSI_CONFIG_ID sensor_id_conv2_csi_valid_h(SENSOR_ID id);
extern UINT32 sensor_mipi_pixdepth_2_real(SEN_MIPI_PIXDEPTH mipi_pixdepth);
extern UINT32 sensor_mipi_datalane_2_real(CSI_DATLANE_NUM mipi_datalane);

// slvs-ec
extern SLVSEC_INTERRUPT sensor_multiid_conv2_slvsec_vd(SENSOR_ID id, SENSOR_MULTI_ID multi_id);
extern SLVSEC_INTERRUPT sensor_multiid_conv2_slvsec_fmd(SENSOR_ID id, SENSOR_MULTI_ID multi_id);
extern SLVSEC_CFG_ID sensor_id_conv2_slvsec_data_id(SENSOR_ID id);
extern SLVSEC_CFG_ID sensor_id_conv2_slvsec_valid_h(SENSOR_ID id);
extern UINT32 sensor_slvsec_pixdepth_2_real(SLVSEC_PIXDEPTH slvsec_pixdepth);
extern UINT32 sensor_slvsec_datalane_2_real(SLVSEC_DATLANE_NO slvsec_datalane);

// vx1
extern VX1_ID sensor_conv2_vx1_Id(SENSOR_ID sen_id);
extern VX1_CONFIG_ID sensor_id_conv2_vx1_virtual_id(SENSOR_ID id);

#endif

