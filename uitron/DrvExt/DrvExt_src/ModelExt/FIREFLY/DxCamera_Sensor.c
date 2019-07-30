/**
    DxSensor

    Sensor common interface

    @file       DxSensor.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "DxSensor.h"
#include "sif.h"
#include "pll.h"
#include "IOCfg.h"
#include "Delay.h"
#include "sensor_drv.h"
#include "lvds.h"
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" // *=All, [mark]=CustomClass
#include "DebugModule.h"

typedef enum {
	POW_CTRL_TURN_ON = 0,
	POW_CTRL_TURN_OFF,
	POW_CTRL_SAVE_ON,
	POW_CTRL_SAVE_OFF,
} POW_CTRL_FLAG;

typedef SENSOR_DRV_TAB *(*SEN_GET_DRV)(void);
typedef SENSOR_INIT_OBJ(*SEN_GET_INI_OBJ)(SENSOR_ID id);
typedef BOOL (*SEN_PLUG_IN)(SENSOR_ID id);
typedef void (*SEN_POWER_CTRL)(SENSOR_ID id, POW_CTRL_FLAG flag);

typedef struct {
	CHAR *name;
	SEN_GET_DRV get_drv_tab;
	SEN_GET_INI_OBJ get_ini_obj;
	SEN_POWER_CTRL pow_ctrl;
	SEN_PLUG_IN det_plug_in;
	SEN_PLUG_IN plug_in_cb;
} SEN_MAP_INFO;

static CHAR sensor_name[SENSOR_ID_MAX_NUM][30] = {
	_SENSOR1_,
	_SENSOR2_,
	_SENSOR3_,
	_SENSOR4_,
	_SENSOR5_,
	_SENSOR6_,
	_SENSOR7_,
	_SENSOR8_,
};


#if (defined(_CMOS_AR0237_))
static SENSOR_INIT_OBJ sensor_get_obj_ar0237(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = FALSE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/SLVC/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;
	return init_obj;
}
static void sen_power_ctrl_ar0237(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:

        //2.8v
        gpio_setDir(P_GPIO_19, GPIO_DIR_OUTPUT);
        gpio_setPin(P_GPIO_19);
        Delay_DelayMs(1);

        //1.8v
        gpio_setDir(P_GPIO_18, GPIO_DIR_OUTPUT);
        gpio_setPin(P_GPIO_18);
        Delay_DelayMs(1);
		// set clock information
		pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

        //reset
		gpio_setDir(S_GPIO_5, GPIO_DIR_OUTPUT);
		gpio_clearPin(S_GPIO_5);
		Delay_DelayMs(30);
		gpio_setPin(S_GPIO_5);
		Delay_DelayMs(1);

		break;

	case POW_CTRL_TURN_OFF:

    	gpio_setDir(P_GPIO_18, GPIO_DIR_OUTPUT);
        gpio_clearPin(P_GPIO_18);
        Delay_DelayMs(1);

    	gpio_setDir(P_GPIO_19, GPIO_DIR_OUTPUT);
        gpio_clearPin(P_GPIO_19);
        Delay_DelayMs(1);
		// set power off sequence
		pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_IMX317M_))

static void csi_hw_rst_imx317m(SENSOR_ID sensor_id) {
	gpio_setDir(S_GPIO_4, GPIO_DIR_OUTPUT);
	gpio_clearPin(S_GPIO_4);
	Delay_DelayMs(1);
	gpio_setPin(S_GPIO_4);
	Delay_DelayUs(10);
}

static SENSOR_INIT_OBJ sensor_get_obj_imx317m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.interface_info.csi.fp_hw_rst = csi_hw_rst_imx317m;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = TRUE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 3;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;

	return init_obj;
}
#define PLL5_CLK2REG(clk) ((clk)*131072/12)
static void sen_power_ctrl_imx317m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:

		// set clock information
		pll_setPLLEn(PLL_ID_5,      DISABLE);
		UINT32 sie_clk_src = 300; // tmp value, wait DE ready
		pll_setPLL(PLL_ID_5, PLL5_CLK2REG(sie_clk_src));
		//DBG_ERR("%dM 0x%x\r\n", sie_clk_src, PLL5_CLK2REG(sie_clk_src));
		pll_setPLLEn(PLL_ID_5,      ENABLE);
		pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_480);
		pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000);

		pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		/*
		gpio_setDir(S_GPIO_4, GPIO_DIR_OUTPUT);
		gpio_clearPin(S_GPIO_4);
		Delay_DelayMs(100);
		gpio_setPin(S_GPIO_4);
		Delay_DelayMs(100);
		*/
		csi_hw_rst_imx317m(id);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_IMX117CQT_))
static SENSOR_INIT_OBJ sensor_get_obj_imx117cqt(SENSOR_ID id)
{
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_SIF;
	init_obj.cmd_info.info.sif.channel = SIF_CH7;
	init_obj.cmd_info.info.sif.bus_clk = 24000000;
	init_obj.cmd_info.info.sif.sen_d_s = 1;
	init_obj.cmd_info.info.sif.sen_h = 1;
	init_obj.cmd_info.info.sif.dma_en = DISABLE;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;

	init_obj.interface_info.tge.b_enable = TRUE;
	init_obj.interface_info.tge.interrupt = 0;
	init_obj.interface_info.tge.tge_clk_src = CLKSRC_MCLK;
	init_obj.interface_info.tge.fp_tge_isr_cb = NULL;
	init_obj.interface_info.tge.swap_info = TGE_OUT_NO_SWAP;
	init_obj.interface_info.tge.b_chg_vd_to_sie_src = FALSE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;
	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = 2;
	init_obj.sen_2_serial_pin_map[3] = 4;
	init_obj.sen_2_serial_pin_map[4] = 5;
	init_obj.sen_2_serial_pin_map[5] = 7;
	init_obj.sen_2_serial_pin_map[6] = 8;
	init_obj.sen_2_serial_pin_map[7] = 9;
	return init_obj;
}
#define PLL5_CLK2REG(clk) ((clk)*131072/12)
static void sen_power_ctrl_imx117cqt(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		//	set dir
		//CHKPNT;
		gpio_setDir(P_GPIO_17,GPIO_DIR_OUTPUT); //1.2V
		gpio_setDir(P_GPIO_18,GPIO_DIR_OUTPUT); //1.8V
		gpio_setDir(P_GPIO_19,GPIO_DIR_OUTPUT); //2.8V
		gpio_setDir(S_GPIO_5, GPIO_DIR_OUTPUT);//reset

		//clear
		gpio_clearPin(S_GPIO_5);
		gpio_clearPin(P_GPIO_17);
		gpio_clearPin(P_GPIO_18);
		gpio_clearPin(P_GPIO_19);

		// set power on sequence
//1.2V -> 1.8V -> 2.8V -> XCLR
		gpio_setPin(P_GPIO_17);
   		gpio_setPin(P_GPIO_18);

    	gpio_setPin(P_GPIO_19);

		pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
		Delay_DelayMs(100);
		gpio_setPin(S_GPIO_5);
		Delay_DelayMs(20);
		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif
#if (defined(_CMOS_IMX291LQR_))
#if (defined(_SENSOR1_CMOS_IMX291LQR_) && defined(_SENSOR2_CMOS_IMX291LQR_) && defined(_SENSOR3_CMOS_IMX291LQR_) && defined(_SENSOR4_CMOS_IMX291LQR_) && \
       defined(_SENSOR5_CMOS_OFF_) && defined(_SENSOR6_CMOS_OFF_) && defined(_SENSOR7_CMOS_OFF_) && defined(_SENSOR8_CMOS_OFF_))
static SENSOR_INIT_OBJ sensor_get_obj_imx291lqr(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = FALSE;
	switch (id) {
	case SENSOR_ID_1:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	case SENSOR_ID_2:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C3_3RD;
		break;
	case SENSOR_ID_3:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
		break;
	case SENSOR_ID_4:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C4_2ND;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	}

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_MCLK;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;
	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[1] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;

	switch (id) {
	case SENSOR_ID_1:
		init_obj.sen_2_serial_pin_map[0] = 0;
		init_obj.sen_2_serial_pin_map[1] = 1;
		break;
	case SENSOR_ID_2:
		init_obj.sen_2_serial_pin_map[4] = 0;
		init_obj.sen_2_serial_pin_map[5] = 1;
		break;
	case SENSOR_ID_3:
		init_obj.sen_2_serial_pin_map[2] = 0;
		init_obj.sen_2_serial_pin_map[3] = 1;
		break;
	case SENSOR_ID_4:
		init_obj.sen_2_serial_pin_map[6] = 0;
		init_obj.sen_2_serial_pin_map[7] = 1;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.sen_2_serial_pin_map[0] = 0;
		init_obj.sen_2_serial_pin_map[1] = 1;
		break;
	}

	return init_obj;
}

static BOOL sen_pwr_on[SENSOR_ID_MAX_NUM] = {FALSE};
static BOOL chk_sen_pwr_on(void)
{
	UINT32 i = 0;
	for (i = 0; i < SENSOR_ID_MAX_NUM; i++) {
		if (sen_pwr_on[i]) {
			return TRUE;
		}
	}
	return FALSE;
}
static void sen_power_ctrl_imx291lqr(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;

	switch (id) {
	case SENSOR_ID_1:
		reset_pin = S_GPIO_4;
		break;
	case SENSOR_ID_2:
		reset_pin = S_GPIO_6;
		break;
	case SENSOR_ID_3:
		reset_pin = S_GPIO_5;
		break;
	case SENSOR_ID_4:
		reset_pin = S_GPIO_7;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		reset_pin = S_GPIO_4;
		break;
	}

	switch (flag) {
	case POW_CTRL_TURN_ON:

		// set clock information
		if (!chk_sen_pwr_on()) {
		    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

		}

		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(100);
		gpio_setPin(reset_pin);
		Delay_DelayMs(100);

		sen_pwr_on[id] = TRUE;

		break;

	case POW_CTRL_TURN_OFF:
		sen_pwr_on[id] = FALSE;
		// set power off sequence
		if (!chk_sen_pwr_on()) {
			pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		}
		Delay_DelayMs(10);
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);

		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#else // single IMX291
static SENSOR_INIT_OBJ sensor_get_obj_imx291lqr(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = TRUE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;

	return init_obj;
}

static BOOL sen_pwr_on[SENSOR_ID_MAX_NUM] = {FALSE};
static BOOL chk_sen_pwr_on(void)
{
	UINT32 i = 0;
	for (i = 0; i < SENSOR_ID_MAX_NUM; i++) {
		if (sen_pwr_on[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

static void sen_power_ctrl_imx291lqr(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:

		// set clock information
		pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		gpio_setDir(S_GPIO_4, GPIO_DIR_OUTPUT);
		gpio_clearPin(S_GPIO_4);
		Delay_DelayMs(100);
		gpio_setPin(S_GPIO_4);
		Delay_DelayMs(100);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		if (!chk_sen_pwr_on()) {
			pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		}
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif
#endif

#if (defined(_CMOS_IMX290LQR_))
#if (defined(_SENSOR1_CMOS_IMX290LQR_) && defined(_SENSOR2_CMOS_IMX290LQR_) && defined(_SENSOR3_CMOS_IMX290LQR_) && defined(_SENSOR4_CMOS_IMX290LQR_) && \
       defined(_SENSOR5_CMOS_OFF_) && defined(_SENSOR6_CMOS_OFF_) && defined(_SENSOR7_CMOS_OFF_) && defined(_SENSOR8_CMOS_OFF_))
static SENSOR_INIT_OBJ sensor_get_obj_imx290lqr(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = FALSE;
	switch (id) {
	case SENSOR_ID_1:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	case SENSOR_ID_2:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C3_3RD;
		break;
	case SENSOR_ID_3:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
		break;
	case SENSOR_ID_4:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C4_2ND;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	}

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_MCLK;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[1] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;

	switch (id) {
	case SENSOR_ID_1:
		init_obj.sen_2_serial_pin_map[0] = 0;
		init_obj.sen_2_serial_pin_map[1] = 1;
		break;
	case SENSOR_ID_2:
		init_obj.sen_2_serial_pin_map[4] = 0;
		init_obj.sen_2_serial_pin_map[5] = 1;
		break;
	case SENSOR_ID_3:
		init_obj.sen_2_serial_pin_map[2] = 0;
		init_obj.sen_2_serial_pin_map[3] = 1;
		break;
	case SENSOR_ID_4:
		init_obj.sen_2_serial_pin_map[6] = 0;
		init_obj.sen_2_serial_pin_map[7] = 1;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.sen_2_serial_pin_map[0] = 0;
		init_obj.sen_2_serial_pin_map[1] = 1;
		break;
	}

	return init_obj;
}

static BOOL sen_pwr_on[SENSOR_ID_MAX_NUM] = {FALSE};
static BOOL chk_sen_pwr_on(void)
{
	UINT32 i = 0;
	for (i = 0; i < SENSOR_ID_MAX_NUM; i++) {
		if (sen_pwr_on[i]) {
			return TRUE;
		}
	}
	return FALSE;
}
static void sen_power_ctrl_imx290lqr(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	UINT32 reset_pin = S_GPIO_4;

	switch (id) {
	case SENSOR_ID_1:
		reset_pin = S_GPIO_4;
		break;
	case SENSOR_ID_2:
		reset_pin = S_GPIO_6;
		break;
	case SENSOR_ID_3:
		reset_pin = S_GPIO_5;
		break;
	case SENSOR_ID_4:
		reset_pin = S_GPIO_7;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		reset_pin = S_GPIO_4;
		break;
	}

	switch (flag) {
	case POW_CTRL_TURN_ON:

		// set clock information
		if (!chk_sen_pwr_on()) {
		    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
		}

		// set power on sequence
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(100);
		gpio_setPin(reset_pin);
		Delay_DelayMs(100);

		sen_pwr_on[id] = TRUE;

		break;

	case POW_CTRL_TURN_OFF:
		sen_pwr_on[id] = FALSE;
		// set power off sequence
		if (!chk_sen_pwr_on()) {
			pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		}

		Delay_DelayMs(10);
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);

		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#else // single IMX290
static SENSOR_INIT_OBJ sensor_get_obj_imx290lqr(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = TRUE;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = 0;
	init_obj.sen_2_serial_pin_map[1] = 1;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;

	return init_obj;
}
static BOOL sen_pwr_on[SENSOR_ID_MAX_NUM] = {FALSE};
static BOOL chk_sen_pwr_on(void)
{
	UINT32 i = 0;
	for (i = 0; i < SENSOR_ID_MAX_NUM; i++) {
		if (sen_pwr_on[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

static void sen_power_ctrl_imx290lqr(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:

		// set clock information
		pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
		// set power on sequence
		gpio_setDir(S_GPIO_4, GPIO_DIR_OUTPUT);
		gpio_clearPin(S_GPIO_4);
		Delay_DelayMs(100);
		gpio_setPin(S_GPIO_4);
		Delay_DelayMs(100);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		if (!chk_sen_pwr_on()) {
			pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		}
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif
#endif


#if (defined(_CMOS_IMX291LVDS_))
#if (defined(_SENSOR1_CMOS_IMX291LVDS_) && defined(_SENSOR2_CMOS_IMX291LVDS_) && defined(_SENSOR3_CMOS_IMX291LVDS_) && defined(_SENSOR4_CMOS_IMX291LVDS_) && \
       defined(_SENSOR5_CMOS_OFF_) && defined(_SENSOR6_CMOS_OFF_) && defined(_SENSOR7_CMOS_OFF_) && defined(_SENSOR8_CMOS_OFF_))
static SENSOR_INIT_OBJ sensor_get_obj_imx291lvds(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = FALSE;
	switch (id) {
	case SENSOR_ID_1:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	case SENSOR_ID_2:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C3_3RD;
		break;
	case SENSOR_ID_3:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
		break;
	case SENSOR_ID_4:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C4_2ND;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	}

	// set mclk change enable to notify sensor driver
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_MCLK;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;
	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[1] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;

	switch (id) {
	case SENSOR_ID_1:
		init_obj.sen_2_serial_pin_map[0] = 3;
		init_obj.sen_2_serial_pin_map[1] = 4;
		break;
	case SENSOR_ID_2:
		init_obj.sen_2_serial_pin_map[4] = 3;
		init_obj.sen_2_serial_pin_map[5] = 4;
		break;
	case SENSOR_ID_3:
		init_obj.sen_2_serial_pin_map[2] = 3;
		init_obj.sen_2_serial_pin_map[3] = 4;
		break;
	case SENSOR_ID_4:
		init_obj.sen_2_serial_pin_map[6] = 3;
		init_obj.sen_2_serial_pin_map[7] = 4;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.sen_2_serial_pin_map[0] = 3;
		init_obj.sen_2_serial_pin_map[1] = 4;
		break;
	}

	return init_obj;
}

static BOOL sen_pwr_on[SENSOR_ID_MAX_NUM] = {FALSE};
static BOOL chk_sen_pwr_on(void)
{
	UINT32 i = 0;
	for (i = 0; i < SENSOR_ID_MAX_NUM; i++) {
		if (sen_pwr_on[i]) {
			return TRUE;
		}
	}
	return FALSE;
}
static void sen_power_ctrl_imx291lvds(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	// set power on sequence
	UINT32 reset_pin = S_GPIO_4;

	switch (id) {
	case SENSOR_ID_1:
		reset_pin = S_GPIO_4;
		break;
	case SENSOR_ID_2:
		reset_pin = S_GPIO_6;
		break;
	case SENSOR_ID_3:
		reset_pin = S_GPIO_5;
		break;
	case SENSOR_ID_4:
		reset_pin = S_GPIO_7;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		reset_pin = S_GPIO_4;
		break;
	}

	switch (flag) {
	case POW_CTRL_TURN_ON:

		// set clock information
		if (!chk_sen_pwr_on()) {
			pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
		}

		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(100);
		gpio_setPin(reset_pin);
		Delay_DelayMs(100);

		sen_pwr_on[id] = TRUE;

		break;

	case POW_CTRL_TURN_OFF:
		sen_pwr_on[id] = FALSE;

		// set power off sequence
		if (!chk_sen_pwr_on()) {
			pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		}

		Delay_DelayMs(10);
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		break;

	case POW_CTRL_SAVE_ON:
		sensor_sleep(id);
		break;

	case POW_CTRL_SAVE_OFF:
		sensor_wakeup(id);
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#else // single imx291lvds
static SENSOR_INIT_OBJ sensor_get_obj_imx291lvds(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = TRUE;

	// set mclk change enable to notify sensor driver
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_MCLK;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = 3;
	init_obj.sen_2_serial_pin_map[1] = 4;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;

	return init_obj;
}

static BOOL sen_pwr_on[SENSOR_ID_MAX_NUM] = {FALSE};
static BOOL chk_sen_pwr_on(void)
{
	UINT32 i = 0;
	for (i = 0; i < SENSOR_ID_MAX_NUM; i++) {
		if (sen_pwr_on[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

static void sen_power_ctrl_imx291lvds(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		gpio_setDir(S_GPIO_4, GPIO_DIR_OUTPUT);
		gpio_clearPin(S_GPIO_4);
		Delay_DelayMs(100);
		gpio_setPin(S_GPIO_4);
		Delay_DelayMs(100);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		if (!chk_sen_pwr_on()) {
			pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		}
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif
#endif

#if (defined(_CMOS_NVP6124B_))
static SENSOR_INIT_OBJ sensor_get_obj_nvp6124b(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C4_2ND;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = TRUE;

	init_obj.option = SENSOR_OPTION_USER;

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	//init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	//init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	return init_obj;
}
static void sen_power_ctrl_nvp6124b(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		pll_setPLLEn(PLL_ID_5,      DISABLE);
		pll_setPLL(PLL_ID_5,        0x90000);//54MHz
		pll_setPLLEn(PLL_ID_5,      ENABLE);

		pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
		pll_setClkFreq(PLL_CLK_SIEMCLK, 27000000);
		pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

		// set power on sequence
		// 1.2v enable
		// default high on EVB
		//gpio_setDir(S_GPIO_8, GPIO_DIR_OUTPUT);
		//gpio_setPin(S_GPIO_8);
		// power on pin
		// default high on EVB
		//gpio_setDir(S_GPIO_8, GPIO_DIR_OUTPUT);
		//gpio_setPin(S_GPIO_8);
		// reset pin
		gpio_setDir(P_GPIO_27, GPIO_DIR_OUTPUT);
		gpio_clearPin(P_GPIO_27);
		Delay_DelayMs(20);
		gpio_setPin(P_GPIO_27);
		Delay_DelayMs(20);

		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		pll_setPLLEn(PLL_ID_5, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_OV7251M_))
static SENSOR_INIT_OBJ sensor_get_obj_ov7251m(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};
	init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
	init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
	init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
	init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
	init_obj.serial_clane_switch = FALSE;
	switch (id) {
	case SENSOR_ID_1:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	case SENSOR_ID_2:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C3_1ST;
		break;
	case SENSOR_ID_3:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C3_1ST;
		break;
	case SENSOR_ID_4:
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C3_1ST;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
		break;
	}

	// set mclk change enable to notify sensor driver
	// init_obj.chg_mclk_en = ENABLE;
	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_MCLK;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	// set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
	init_obj.sen_2_serial_pin_map[0] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[1] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[8] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[9] = SEN_IGNORE;

	switch (id) {
	case SENSOR_ID_1:
		init_obj.sen_2_serial_pin_map[0] = 0;
		break;
	case SENSOR_ID_2:
		init_obj.sen_2_serial_pin_map[0] = 4;
		break;
	case SENSOR_ID_3:
		init_obj.sen_2_serial_pin_map[0] = 2;
		break;
	case SENSOR_ID_4:
		init_obj.sen_2_serial_pin_map[0] = 6;
		break;
	default:
		DBG_ERR("sensor id not implement %d\r\n", id);
		init_obj.sen_2_serial_pin_map[0] = 0;
		break;
	}
	return init_obj;
}

static void sen_power_ctrl_ov7251m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	static BOOL b_set_clk = FALSE;
	switch (flag) {
	case POW_CTRL_TURN_ON:
		// set clock information
		if (!b_set_clk) {
			pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_480);
			pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000);
			pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

			b_set_clk = TRUE;
		}

		// set power on sequence
		UINT32 reset_pin = S_GPIO_4;

		switch (id) {
		case SENSOR_ID_1:
			reset_pin = S_GPIO_4;
			break;
		case SENSOR_ID_2:
			reset_pin = S_GPIO_6;
			break;
		case SENSOR_ID_3:
			reset_pin = S_GPIO_5;
			break;
		case SENSOR_ID_4:
			reset_pin = S_GPIO_7;
			break;
		default:
			DBG_ERR("sensor id not implement %d\r\n", id);
			reset_pin = S_GPIO_4;
			break;
		}
		gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
		gpio_clearPin(reset_pin);
		Delay_DelayMs(100);
		gpio_setPin(reset_pin);
		Delay_DelayMs(100);
		break;

	case POW_CTRL_TURN_OFF:
		// set power off sequence
		pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif

#if (defined(_CMOS_IMX377M_))

static SENSOR_INIT_OBJ sensor_get_obj_imx377m(SENSOR_ID id)
{

    // set sensor command interface info (Vx1/SIF/I2C/IO/...)
    SENSOR_INIT_OBJ init_obj = {0};
    init_obj.cmd_info.cmd_type = SENSOR_CMD_I2C;
    init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
    init_obj.cmd_info.info.i2c.w_addr_sel = SEN_I2C_W_ADDR_DFT;
    init_obj.sen_map_if = SENSOR_MAP_IF_DEFAULT;
    init_obj.serial_clane_switch = TRUE;

    switch (id) {
    case SENSOR_ID_1:
        init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
        break;
    case SENSOR_ID_2:
        init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C3_3RD;
        break;
    case SENSOR_ID_3:
        init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C1_1ST;
        break;
    case SENSOR_ID_4:
        init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C4_2ND;
        break;
    default:
        DBG_ERR("sensor id not implement %d\r\n", id);
        init_obj.cmd_info.info.i2c.i2c = SENSOR_I2C2_1ST;
        break;
    }

    // set mclk change enable to notify sensor driver
    // init_obj.chg_mclk_en = ENABLE;
    init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
    init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

    // set sensor interface pin map (LVDS/CSI/SLVS-EC/...)
    init_obj.sen_2_serial_pin_map[0] = SEN_IGNORE;
    init_obj.sen_2_serial_pin_map[1] = SEN_IGNORE;
    init_obj.sen_2_serial_pin_map[2] = SEN_IGNORE;
    init_obj.sen_2_serial_pin_map[3] = SEN_IGNORE;
    init_obj.sen_2_serial_pin_map[4] = SEN_IGNORE;
    init_obj.sen_2_serial_pin_map[5] = SEN_IGNORE;
    init_obj.sen_2_serial_pin_map[6] = SEN_IGNORE;
    init_obj.sen_2_serial_pin_map[7] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[8] = SEN_IGNORE;
	init_obj.sen_2_serial_pin_map[9] = SEN_IGNORE;

    switch (id) {
    case SENSOR_ID_1:
        init_obj.sen_2_serial_pin_map[0] = 0;
        init_obj.sen_2_serial_pin_map[1] = 1;
        init_obj.sen_2_serial_pin_map[2] = 2;
        init_obj.sen_2_serial_pin_map[3] = 3;
        break;
    case SENSOR_ID_2:
        init_obj.sen_2_serial_pin_map[0] = 4;
        init_obj.sen_2_serial_pin_map[1] = 5;
        break;
    case SENSOR_ID_3:
        init_obj.sen_2_serial_pin_map[0] = 2;
        init_obj.sen_2_serial_pin_map[1] = 3;
        break;
    case SENSOR_ID_4:
        init_obj.sen_2_serial_pin_map[0] = 6;
        init_obj.sen_2_serial_pin_map[1] = 7;
        break;
    default:
        DBG_ERR("sensor id not implement %d\r\n", id);
        init_obj.sen_2_serial_pin_map[0] = 0;
        init_obj.sen_2_serial_pin_map[1] = 1;
        init_obj.sen_2_serial_pin_map[2] = 2;
        init_obj.sen_2_serial_pin_map[3] = 3;
    	break;
    }
    return init_obj;
}


static BOOL sen_pwr_on[SENSOR_ID_MAX_NUM] = {FALSE};

static BOOL chk_sen_pwr_on(void)
{
    UINT32 i = 0;
    for (i = 0; i < SENSOR_ID_MAX_NUM; i++) {
        if (sen_pwr_on[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

static void sen_power_ctrl_imx377m(SENSOR_ID id, POW_CTRL_FLAG flag)
{
    UINT32 reset_pin = S_GPIO_4;

    switch (id) {
    case SENSOR_ID_1:
        reset_pin = S_GPIO_4;
        break;
    case SENSOR_ID_2:
        reset_pin = S_GPIO_6;
        break;
    case SENSOR_ID_3:
        reset_pin = S_GPIO_5;
        break;
    case SENSOR_ID_4:
        reset_pin = S_GPIO_7;
        break;
    default:
        DBG_ERR("sensor id not implement %d\r\n", id);
        reset_pin = S_GPIO_4;
        break;
    }

    switch (flag) {
    case POW_CTRL_TURN_ON:
        if (!chk_sen_pwr_on()) {

            gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
            gpio_clearPin(reset_pin);
            Delay_DelayMs(100);
            gpio_setPin(reset_pin);
            Delay_DelayMs(100);

            pll_setPLLEn(PLL_ID_5,      DISABLE);
            pll_setPLL(PLL_ID_5,        0x2A0000);//252Mhz,2a;378MHz,3f
            pll_setPLLEn(PLL_ID_5,      ENABLE);

            pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_480);
            pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000);
            pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

        }

        sen_pwr_on[id] = TRUE;

        break;

    case POW_CTRL_TURN_OFF:
        sen_pwr_on[id] = FALSE;
        // set power off sequence
        if (!chk_sen_pwr_on()) {
            pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
            pll_setPLLEn(PLL_ID_5, DISABLE);
        }

        Delay_DelayMs(10);
        gpio_setDir(reset_pin, GPIO_DIR_OUTPUT);
        gpio_clearPin(reset_pin);

        break;

    case POW_CTRL_SAVE_ON:
        break;

    case POW_CTRL_SAVE_OFF:
        break;

    default:
        DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
        break;
    }
}
#endif

#if (defined(_CMOS_SIEPATGEN_))
static SENSOR_INIT_OBJ sensor_get_obj_siepatgen(SENSOR_ID id)
{
	// set sensor command interface info (Vx1/SIF/I2C/IO/...)
	SENSOR_INIT_OBJ init_obj = {0};

	init_obj.sie_mclk_info.mclk_src = SEN_SIEMCLK_SRC_DFT;
	init_obj.sie_mclk_info.chg_mclk_en = ENABLE;

	return init_obj;
}

static void sen_power_ctrl_siepatgen(SENSOR_ID id, POW_CTRL_FLAG flag)
{
	switch (flag) {
	case POW_CTRL_TURN_ON:
		if (id == SENSOR_ID_1) {
			pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
		}
		else {
			pll_setClkEn(PLL_CLK_SIEMCLK2, ENABLE);
		}

		break;

	case POW_CTRL_TURN_OFF:
		break;

	case POW_CTRL_SAVE_ON:
		break;

	case POW_CTRL_SAVE_OFF:
		break;

	default:
		DBG_ERR("SenPowerCtrl fail (%d)\r\n", flag);
		break;
	}
}
#endif
static SEN_MAP_INFO sensor_map_tab[SENSOR_ID_MAX_NUM + 1] = {
#if (defined(_CMOS_AR0237_))
	{"CMOS_AR0237", sensor_get_drv_tab_ar0237, sensor_get_obj_ar0237, sen_power_ctrl_ar0237, NULL, NULL},
#endif

#if (defined(_CMOS_IMX317M_))
	{"CMOS_IMX317M", sensor_get_drv_tab_imx317m, sensor_get_obj_imx317m, sen_power_ctrl_imx317m, NULL, NULL},
#endif

#if (defined(_CMOS_IMX117CQT_))
	{"CMOS_IMX117CQT", sensor_get_drv_tab_imx117cqt, sensor_get_obj_imx117cqt, sen_power_ctrl_imx117cqt, NULL, NULL},
#endif

#if (defined(_CMOS_IMX291LQR_))
	{"CMOS_IMX291LQR", sensor_get_drv_tab_imx291lqr, sensor_get_obj_imx291lqr, sen_power_ctrl_imx291lqr, NULL, NULL},
#endif

#if (defined(_CMOS_IMX290LQR_))
	{"CMOS_IMX290LQR", sensor_get_drv_tab_imx290lqr, sensor_get_obj_imx290lqr, sen_power_ctrl_imx290lqr, NULL, NULL},
#endif

#if (defined(_CMOS_IMX291LVDS_))
	{"CMOS_IMX291LVDS", sensor_get_drv_tab_imx291lvds, sensor_get_obj_imx291lvds, sen_power_ctrl_imx291lvds, NULL, NULL},
#endif

#if (defined(_CMOS_NVP6124B_))
	{"CMOS_NVP6124B", sensor_get_drv_tab_nvp6124b, sensor_get_obj_nvp6124b, sen_power_ctrl_nvp6124b, NULL, NULL},
#endif

#if (defined(_CMOS_OV7251M_))
	{"CMOS_OV7251M", sensor_get_drv_tab_ov7251m, sensor_get_obj_ov7251m, sen_power_ctrl_ov7251m, NULL, NULL},
#endif

#if (defined(_CMOS_IMX377M_))
	{"CMOS_IMX377M", sensor_get_drv_tab_imx377m, sensor_get_obj_imx377m, sen_power_ctrl_imx377m, NULL, NULL},
#endif

#if (defined(_CMOS_SIEPATGEN_))
	{"CMOS_SIEPATGEN", sensor_get_drv_tab_siepatgen, sensor_get_obj_siepatgen, sen_power_ctrl_siepatgen, NULL, NULL},
#endif

	{"END", NULL, NULL, NULL, NULL, NULL},
};


#if 0
#endif
/************************************************************************/
/*                      common function                                 */
/************************************************************************/
static SEN_MAP_INFO *drv_sensor_search(CHAR *search_name)
{
	UINT32 i;
	SEN_MAP_INFO *sen_map_info;

	DBG_IND("search drv name = %s\r\n", search_name);
	sen_map_info = sensor_map_tab;

	for (i = 0; (strcmp(sen_map_info[i].name, "END") != 0); i ++) {
		if (strcmp(sen_map_info[i].name, search_name) == 0) {
			return &sen_map_info[i];
		}
	}
	return NULL;
}

BOOL drv_sensor_repalce_drv(SENSOR_ID id, CHAR *repalce_name)
{
	if (id >= SENSOR_ID_MAX_NUM) {
		DBG_ERR("id(%d) overflow\r\n");
		return FALSE;
	}
	DBG_IND("id = %d, repalce drv name = %s\r\n", id, repalce_name);

	if (drv_sensor_search(repalce_name) == NULL) {
		DBG_ERR("search sensor driver fail (%s)\r\n", repalce_name);
		return FALSE;
	}

	strncpy(&sensor_name[id][0], repalce_name, sizeof(sensor_name[id]));

	return TRUE;
}

SENSOR_MULTI_ID sensor_gp_map[SENSOR_ID_MAX_NUM] = {0}, sensor_subgp_map[SENSOR_ID_MAX_NUM] = {0};
ER drv_sensor_group_cfg(SENSOR_MULTI_ID* sensor_gp, SENSOR_MULTI_ID* sensor_subgp)
{
	UINT32 cnt, chk_cnt;

	for (cnt = 0; cnt < SENSOR_ID_MAX_NUM; cnt++) {
		if (sensor_gp == SENSOR_MULTI_ID_NONE) {
			continue;
		}
		for (chk_cnt = 0; chk_cnt < SENSOR_ID_MAX_NUM; chk_cnt++) {
			if (sensor_chk_multiid(chk_cnt, sensor_gp[cnt])) {
				sensor_gp_map[chk_cnt] |= sensor_gp[cnt];
			}
		}
	}
	for (cnt = 0; cnt < SENSOR_ID_MAX_NUM; cnt++) {
		if (sensor_subgp == SENSOR_MULTI_ID_NONE) {
			continue;
		}
		for (chk_cnt = 0; chk_cnt < SENSOR_ID_MAX_NUM; chk_cnt++) {
			if (sensor_chk_multiid(chk_cnt, sensor_subgp[cnt])) {
				sensor_subgp_map[chk_cnt] |= sensor_subgp[cnt];
			}
		}
	}

	return E_OK;
}

BOOL drv_sensor_first_cfg(void)
{
	UINT32 cnt_id;
	SEN_MAP_INFO *sen_map_info;
	ER rt = E_OK;

	for (cnt_id = 0; cnt_id < SENSOR_ID_MAX_NUM; cnt_id++) {
		if (sensor_debug_item[cnt_id] & SENSOR_DEBUG_MODELEXT) {
			debug_msg("^C%s\r\n", __func__);
		}
	}

	// Check that no sensor is open
	for (cnt_id = 0; cnt_id < SENSOR_ID_MAX_NUM; cnt_id++) {
		if (sensor_is_open(cnt_id)) {
			DBG_ERR("sensor id %d is open\r\n", cnt_id);
			return FALSE;
		}
	}

	// set group info.
	sensor_set_group_info(sensor_gp_map, sensor_subgp_map);

	// set mclk info for SIE MCLK & set data speed info for SIE CLK
	for (cnt_id = 0; cnt_id < SENSOR_ID_MAX_NUM; cnt_id++) {
		sen_map_info = drv_sensor_search(sensor_name[cnt_id]);
		if ((strcmp(sensor_name[cnt_id], "CMOS_OFF") != 0) && (sen_map_info != NULL)) {
			SENSOR_INIT_OBJ init_obj = sen_map_info->get_ini_obj(cnt_id);
			rt = sensor_cfg_sie_mclk(cnt_id, &init_obj, sen_map_info->get_drv_tab());
			if (rt != E_OK) {
				DBG_ERR("id %d sensor_cfg_sie_mclk error %d\r\n", cnt_id, rt);
			}
			rt = sensor_cfg_sie_max_clk(cnt_id, &init_obj, sen_map_info->get_drv_tab());
			if (rt != E_OK) {
				DBG_ERR("id %d sensor_cfg_sie_clk error %d\r\n", cnt_id, rt);
			}
		}
	}

	// set pll clk
	rt = sensor_cfg_pll_clk();
	if (rt != E_OK) {
		DBG_ERR("sensor_cfg_pll_clk %d\r\n", rt);
	}

	return FALSE;
}

BOOL drv_sensor_init(SENSOR_ID id, SENSOR_HANDLE *handle)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return FALSE;
	}

	if ((sen_map_info->get_drv_tab == NULL) || (sen_map_info->get_ini_obj == NULL)) {
		DBG_ERR("fp NULL fail\r\n");
		return FALSE;
	}

	if (handle != NULL) {
		handle->drv_tab = sen_map_info->get_drv_tab();
		handle->ini_obj = sen_map_info->get_ini_obj(id);
		return TRUE;
	}

	return FALSE;
}

void drv_sensor_power_turn_on(SENSOR_ID id)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sen_map_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_on(id)) {
		sen_map_info->pow_ctrl(id, POW_CTRL_TURN_ON);
	}
}

void drv_sensor_power_turn_off(SENSOR_ID id)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sen_map_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_off(id)) {
		sen_map_info->pow_ctrl(id, POW_CTRL_TURN_OFF);
	}
}

void drv_sensor_power_save_on(SENSOR_ID id)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sen_map_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_save_on(id)) {
		sen_map_info->pow_ctrl(id, POW_CTRL_SAVE_ON);
	}
}

void drv_sensor_power_save_off(SENSOR_ID id)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return;
	}

	if (sen_map_info->pow_ctrl == NULL) {
		DBG_IND("driver NULL\r\n");
		return;
	}

	if (sensor_chk_power_save_off(id)) {
		sen_map_info->pow_ctrl(id, POW_CTRL_SAVE_OFF);
	}
}

BOOL drv_sensor_det_plug_in(SENSOR_ID id)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return FALSE;
	}

	if (sen_map_info->det_plug_in == NULL) {
		DBG_IND("driver NULL\r\n");
		return FALSE;
	}

	return sen_map_info->det_plug_in(id);
}

BOOL drv_sensor_plug_in_cb(SENSOR_ID id)
{
	SEN_MAP_INFO *sen_map_info = drv_sensor_search(sensor_name[id]);

	if (sensor_debug_item[id] & SENSOR_DEBUG_MODELEXT) {
		debug_msg("^C%s id %d\r\n", __func__, id);
	}

	if (sen_map_info == NULL) {
		DBG_ERR("driver get fail\r\n");
		return FALSE;
	}

	if (sen_map_info->plug_in_cb == NULL) {
		DBG_IND("driver NULL\r\n");
		return FALSE;
	}

	return sen_map_info->plug_in_cb(id);
}
