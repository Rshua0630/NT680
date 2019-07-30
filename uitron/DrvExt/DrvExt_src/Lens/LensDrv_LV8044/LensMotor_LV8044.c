/**
 * Novatek SANYO LV8044 motor driver.
 * 
 * Reference motor driver implementation.
 * 
 * @file LensMotor_LV8044.c
 * @note Nothing.
 *
 * Copyright (C) 2018 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include "gpio.h"
#if 0
#include "sif.h"
#else
#include "spi.h"
#endif
#include "LensMotor_LV8044.h"


// prototype declaration
static void   motorLV8044_Init(PLENS_DEVICE_OBJ plens_dev);
static void   motorLV8044_SetSignal(MD_SIGNAL signal, UINT32 on);
static UINT32 motorLV8044_GetSignal(MD_SIGNAL signal);
static UINT32 motorLV8044_GetBusyState(void);
static UINT32 motorLV8044_ZoomGetSpeed(void);
static void   motorLV8044_ZoomSetState(MOTOR_ZOOM_ACT state, UINT32 param);
static UINT32 motorLV8044_FocusGetSpeed(void);
static BOOL   motorLV8044_FocusSetState(MOTOR_FOCUS_ACT state, UINT32 param);
static void   motorLV8044_ApertureSetState(MOTOR_APERTURE_ACT state, UINT32 param);
static void   motorLV8044_ShutterSetState(MOTOR_SHUTTER_ACT state, UINT32 param);
static void   motorLV8044_IrcutSetState(MOTOR_IRCUT_ACT state, UINT32 param);

static UINT32 g_uiLensDrvSema = 0xFFFFFFFF;


//----------------------------------------------------------------------------------------------------------------------
// general
//----------------------------------------------------------------------------------------------------------------------
LENSMOTOR_INFO* LV8044_GetMotorInfo(void)
{
    return &g_LensMotorInfo;
}


PMOTOR_TAB DrvLens_GetMotor(void)
{
    DBG_MSG("%s\r\n", __func__);

    OS_CONFIG_SEMPHORE(g_uiLensDrvSema, 0, 1, 1);

    g_LensMotorInfo.LensMotor.init              = motorLV8044_Init;
    g_LensMotorInfo.LensMotor.setSignal         = motorLV8044_SetSignal;
    g_LensMotorInfo.LensMotor.getSignal         = motorLV8044_GetSignal;
    g_LensMotorInfo.LensMotor.getBusyState      = motorLV8044_GetBusyState;
    g_LensMotorInfo.LensMotor.zoom_getSpeed     = motorLV8044_ZoomGetSpeed;
    g_LensMotorInfo.LensMotor.zoom_setState     = motorLV8044_ZoomSetState;
    g_LensMotorInfo.LensMotor.focus_getSpeed    = motorLV8044_FocusGetSpeed;
    g_LensMotorInfo.LensMotor.focus_setState    = motorLV8044_FocusSetState;
    g_LensMotorInfo.LensMotor.aperture_setState = motorLV8044_ApertureSetState;
    g_LensMotorInfo.LensMotor.shutter_setState  = motorLV8044_ShutterSetState;
    g_LensMotorInfo.LensMotor.ircut_setState    = motorLV8044_IrcutSetState;

    return &g_LensMotorInfo.LensMotor;
}


static void LV8044_InitSPI(void)
{
#if 0
    if (sif_isOpened(MOTOR_LV8044_SIF_CH) == FALSE) {
        sif_open(MOTOR_LV8044_SIF_CH);
    }

    sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_BUSMODE, SIF_MODE_02);
    sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_BUSCLOCK, 4000000);    // bus clock = 4 MHz (1~4 MHz)
    //sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_SEND, 4000);         // 24 half bus clock = 4000 ns
    sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_SENS, 1);              // 0 half bus clock = 0 ns
    sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_SENH, 1);              // 0 half bus clock = 0 ns
    sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_LENGTH, 8);            // 8 bits per transmission
    sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_DIR, SIF_DIR_LSB);     // LSB shift out first
    //sif_setConfig(MOTOR_LV8044_SIF_CH, SIF_CONFIG_ID_DELAY, 200);         // delay between successive transmission
#else
    spi_setConfig(MOTOR_LV8044_SPI_CH, SPI_CONFIG_ID_BUSMODE, SPI_MODE_0);
    spi_setConfig(MOTOR_LV8044_SPI_CH, SPI_CONFIG_ID_FREQ, 5000000);    // 2000000 * 2.5 = 5000000  bus clock = 2 MHz (~4 MHz)
    spi_setConfig(MOTOR_LV8044_SPI_CH, SPI_CONFIG_ID_MSB_LSB, SPI_LSB);
    spi_setConfig(MOTOR_LV8044_SPI_CH, SPI_CONFIG_ID_CS_ACT_LEVEL, SPI_CS_ACT_LEVEL_LOW);
    spi_setConfig(MOTOR_LV8044_SPI_CH, SPI_CONFIG_ID_CS_CK_DLY, 0);

    spi_open(MOTOR_LV8044_SPI_CH);
#endif
}


static void LV8044_Write2SPI(UINT32 value)
{
#if 0
    sif_send(MOTOR_LV8044_SIF_CH, value, 0, 0);
    sif_waitTransferEnd(MOTOR_LV8044_SIF_CH);
#else
    spi_setCSActive(MOTOR_LV8044_SPI_CH, TRUE);

    // transmit 8-bit address & data
    spi_setTransferLen(MOTOR_LV8044_SPI_CH, SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(MOTOR_LV8044_SPI_CH, (value & 0xFF));

    LENS_DELAY_US(4);

    spi_setCSActive(MOTOR_LV8044_SPI_CH, FALSE);

    DBG_MSG("SPI cmd = 0x%02X\r\n", value);
#endif
}


static UINT32 LV8044_Read4SPI(UINT8 addr)
{
#if 0
    UINT32 rd_data = 0;

    spi_setCSActive(MOTOR_LV8044_SPI_CH, TRUE);

    // transmit 8-bit address & data
    spi_setTransferLen(MOTOR_LV8044_SPI_CH, SPI_TRANSFER_LEN_1BYTE);
    spi_writeReadSingle(MOTOR_LV8044_SPI_CH, (UINT32)(addr & 0x07), &rd_data);

    spi_setCSActive(MOTOR_LV8044_SPI_CH, FALSE);

    return (rd_data & 0xFF);
#else
    UINT32 *prd_reg;

    prd_reg = (UINT32 *)&g_LensMotorInfo.LensMotorReg.REG[0];

    return prd_reg[addr];
#endif
}


INT32 LV8044_TransmitCmd(LV8044_CHNEL_NUM chnel, LV8044_CMD_TYPE cmd_type, INT32 cmd_arg)
{
    INT32 cmd_status = 0;
    UINT32 wr_idx = 0;
    UINT32 *pwr_reg;
    SPI_DATA_FMT spi_ctl;
    MOTOR_CONFIG *pmotor_cfg;

    pmotor_cfg = (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == chnel) ? &g_LensMotorInfo.LensMotorConfig[0]
              : ((g_LensMotorInfo.LensMotorConfig[1].eChnelSel == chnel) ? &g_LensMotorInfo.LensMotorConfig[1] : NULL);

    if (pmotor_cfg == NULL) {
        return -1;
    }

    wai_sem(g_uiLensDrvSema);

    pwr_reg = (UINT32 *)&g_LensMotorInfo.LensMotorReg.REG[0];

    if ((cmd_type == LV8044_CMD_SET_MODE) || (cmd_type == LV8044_CMD_SET_CURR)) {
        wr_idx = (pmotor_cfg->eChnelSel == LV8044_CHNEL1) ? 0 : 2;
    } else if ((cmd_type == LV8044_CMD_DIRECTION) || (cmd_type == LV8044_CMD_RESET) || (cmd_type == LV8044_CMD_ENABLE)) {
        wr_idx = (pmotor_cfg->eChnelSel == LV8044_CHNEL1) ? 1 : 3;
    }

    spi_ctl.data = (UINT8)pwr_reg[wr_idx];

    switch (cmd_type) {
    case LV8044_CMD_SET_MODE:
        if ((LV8044_MACROSTEP_MODE)cmd_arg == LV8044_MODE_2PHASE) {
            spi_ctl.B.b3 = 0;
            spi_ctl.B.b4 = 0;
        } else if ((LV8044_MACROSTEP_MODE)cmd_arg == LV8044_MODE_12PHASE_FULL) {
            spi_ctl.B.b3 = 1;
            spi_ctl.B.b4 = 0;
        } else if ((LV8044_MACROSTEP_MODE)cmd_arg == LV8044_MODE_12PHASE) {
            spi_ctl.B.b3 = 0;
            spi_ctl.B.b4 = 1;
        } else if ((LV8044_MACROSTEP_MODE)cmd_arg == LV8044_MODE_4W12PHASE) {
            spi_ctl.B.b3 = 1;
            spi_ctl.B.b4 = 1;
        } else {
            cmd_status = -1;
        }
        break;

    case LV8044_CMD_SET_CURR:
        if (cmd_arg == 0) {           // referense voltage 100%
            spi_ctl.B.b5 = 0;
            spi_ctl.B.b6 = 0;
        } else if (cmd_arg == 1) {    // referense voltage 67%
            spi_ctl.B.b5 = 1;
            spi_ctl.B.b6 = 0;
        } else if (cmd_arg == 2) {    // referense voltage 50%
            spi_ctl.B.b5 = 0;
            spi_ctl.B.b6 = 1;
        } else {                      // referense voltage 33%
            spi_ctl.B.b5 = 1;
            spi_ctl.B.b6 = 1;
        }
        break;

    case LV8044_CMD_DIRECTION:
        if (pmotor_cfg->iDefPositiveDir == 0) {
            spi_ctl.B.b3 = (cmd_arg == MOVE_DIR_FWD) ? 0 : 1;
        } else {
            spi_ctl.B.b3 = (cmd_arg == MOVE_DIR_FWD) ? 1 : 0;
        }
        break;

    case LV8044_CMD_RESET:
        spi_ctl.B.b4 = 0;    // micro step hold
        spi_ctl.B.b5 = 0;    // reset
        spi_ctl.B.b6 = 0;    // disable output
        break;

    case LV8044_CMD_ENABLE:
        spi_ctl.B.b4 = 0;    // micro step hold
        spi_ctl.B.b5 = 1;    // reset
        spi_ctl.B.b6 = (cmd_arg != 0) ? 1 : 0;    // enable/disable output
        break;

    case LV8044_CMD_UNKNOWN:
    default:
        cmd_status = -1;
        break;
    }

    wr_idx = LENS_MIN(wr_idx, 3);

    if (cmd_status != -1) {
        LV8044_Write2SPI((UINT32)spi_ctl.data);

        pwr_reg[wr_idx] = (UINT32)spi_ctl.data;
    } else {
        DBG_MSG("%s err!\r\n", __func__);
    }

    sig_sem(g_uiLensDrvSema);

    return cmd_status;
}


static UINT8 LV8044_ReceiveCmd(LV8044_CHNEL_NUM chnel, LV8044_CMD_TYPE cmd_type)
{
    INT32 cmd_status = 0;
    UINT32 rd_idx = 0;
    UINT32 *prd_reg;
    MOTOR_CONFIG *pmotor_cfg;

    pmotor_cfg = (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == chnel) ? &g_LensMotorInfo.LensMotorConfig[0]
              : ((g_LensMotorInfo.LensMotorConfig[1].eChnelSel == chnel) ? &g_LensMotorInfo.LensMotorConfig[1] : NULL);

    if (pmotor_cfg == NULL) {
        return 0xFF;
    }

    prd_reg = (UINT32 *)&g_LensMotorInfo.LensMotorReg.REG[0];

    switch (cmd_type) {
    case LV8044_CMD_SET_MODE:
    case LV8044_CMD_SET_CURR:
        rd_idx = (pmotor_cfg->eChnelSel == LV8044_CHNEL1) ? 0 : 2;
        break;

    case LV8044_CMD_DIRECTION:
    case LV8044_CMD_RESET:
    case LV8044_CMD_ENABLE:
        rd_idx = (pmotor_cfg->eChnelSel == LV8044_CHNEL1) ? 1 : 3;
        break;

    default:
        cmd_status = -1;
        break;
    }

    rd_idx = LENS_MIN(rd_idx, 7);

    if (cmd_status != -1) {
        return (UINT8)prd_reg[rd_idx];
    } else {
        DBG_MSG("%s err!\r\n", __func__);

        return 0xFF;
    }
}


static INT32 LV8044_WaitMoveEnd(MOTOR_SEL motor_sel, const UINT32 timeout_ms)
{
    UINT32 cnt = 0;
    MOTOR_CONFIG *pmotor_cfg;

    pmotor_cfg = (motor_sel == MOTOR_FOCUS) ? &g_LensMotorInfo.LensMotorConfig[0] : &g_LensMotorInfo.LensMotorConfig[1];

    while (pmotor_cfg->iMotorStatus != MOTOR_STATUS_FREE) {
        if (cnt++ >= timeout_ms) {
            DBG_ERR("%s timeout pin(%u)!\r\n", __func__, motor_sel);

            return -1;
        }

        LENS_DELAY_MS(1);
    }

    return 0;
}


static INT32 LV8044_MotorInit(MOTOR_SEL motor_sel)
{
    INT32 pwm_freq;
    UINT32 clk_div;
    MOTOR_CONFIG *pmotor_cfg;

    pmotor_cfg = (motor_sel == MOTOR_FOCUS) ? &g_LensMotorInfo.LensMotorConfig[0] :
                ((motor_sel == MOTOR_ZOOM)  ? &g_LensMotorInfo.LensMotorConfig[1] : NULL);

    if ((motor_sel >= MOTOR_IRIS) || (pmotor_cfg == NULL)) {
        DBG_MSG("%s unknown motor channel!\r\n", __func__);

        return -1;
    }

    LV8044_TransmitCmd(pmotor_cfg->eChnelSel, LV8044_CMD_SET_MODE, pmotor_cfg->eMicroStepMode);
    LV8044_TransmitCmd(pmotor_cfg->eChnelSel, LV8044_CMD_ENABLE, 1);

    pwm_freq = (pmotor_cfg->eMicroStepMode != LV8044_MODE_12PHASE) ? ((INT32)pmotor_cfg->eMicroStepMode * pmotor_cfg->uiPwmFreq)
                                                                   : (((INT32)pmotor_cfg->eMicroStepMode - 1) * pmotor_cfg->uiPwmFreq);

    if (LV8044_ReceiveCmd(pmotor_cfg->eChnelSel, LV8044_CMD_RESET) == 0xFF) {
        DBG_MSG("motor IC may have no existence!\r\n");

        return -1;
    }

    // request PWM
    if (pwm_open(pmotor_cfg->uiPwmOutputPin) != E_OK) {
        DBG_ERR("fail to request PWM%u!\r\n", pmotor_cfg->uiPwmOutputPin);

        return -1;
    }

    pmotor_cfg->uiPwmBaseNum = 60 * (UINT32)LV8044_MOTOR_SPD_1X / (UINT32)pmotor_cfg->eMoveSpeed;

    // output freq = 120M / (clk_div+1) / base_period = uiPwmFreq
    clk_div = 120000000 / 60 / (UINT32)pwm_freq - 1;
    clk_div = LENS_CLAMP(clk_div, 59, 960);

    if ((pmotor_cfg->uiPwmOutputPin >= PWMID_0) && (pmotor_cfg->uiPwmOutputPin <= PWMID_3)) {
        pwm_pwmConfigClockDiv(PWM0_3_CLKDIV, clk_div);
    } else if ((pmotor_cfg->uiPwmOutputPin >= PWMID_4) && (pmotor_cfg->uiPwmOutputPin <= PWMID_7)) {
        pwm_pwmConfigClockDiv(PWM4_7_CLKDIV, clk_div);
    } else if ((pmotor_cfg->uiPwmOutputPin >= PWMID_8) && (pmotor_cfg->uiPwmOutputPin <= PWMID_11)) {
        pwm_pwmConfigClockDiv(PWM8_11_CLKDIV, clk_div);
    } else if (pmotor_cfg->uiPwmOutputPin == PWMID_12) {
        pwm_pwmConfigClockDiv(PWM12_CLKDIV, clk_div);
    } else if (pmotor_cfg->uiPwmOutputPin == PWMID_13) {
        pwm_pwmConfigClockDiv(PWM13_CLKDIV, clk_div);
    } else if (pmotor_cfg->uiPwmOutputPin == PWMID_14) {
        pwm_pwmConfigClockDiv(PWM14_CLKDIV, clk_div);
    } else if (pmotor_cfg->uiPwmOutputPin == PWMID_15) {
        pwm_pwmConfigClockDiv(PWM15_CLKDIV, clk_div);
    }

    return 0;
}


static INT32 LV8044_SetMotorPosition(MOTOR_SEL motor_sel, INT32 step_pos)
{
    PWM_CFG pwm_cfg = {60, 0, 30, 1, 0};    // uiPrd, uiRise, uiFall, uiOnCycle (0 ~ 65535), uiInv
    MOTOR_CONFIG *pmotor_cfg;

    pmotor_cfg = (motor_sel == MOTOR_FOCUS) ? &g_LensMotorInfo.LensMotorConfig[0] :
                ((motor_sel == MOTOR_ZOOM)  ? &g_LensMotorInfo.LensMotorConfig[1] : NULL);

    if (pmotor_cfg == NULL) {
        DBG_MSG("%s unknown motor channel!\r\n", __func__);

        return -1;
    }

    if (step_pos != 0) {
        pmotor_cfg->iWorkStatus = pmotor_cfg->iMotorStatus = MOTOR_STATUS_BUSY;

        LV8044_TransmitCmd(pmotor_cfg->eChnelSel, LV8044_CMD_DIRECTION, pmotor_cfg->iMoveDir);
        LENS_DELAY_MS(2);    // 2 msec delay for direction cmd

        step_pos = LENS_ABS(step_pos);

        switch (pmotor_cfg->eMicroStepMode) {
        case LV8044_MODE_2PHASE:
            step_pos *= 2;
            break;

        case LV8044_MODE_12PHASE_FULL:
        case LV8044_MODE_12PHASE:
        default:
            step_pos *= 4;
            break;

        case LV8044_MODE_4W12PHASE:
            step_pos *= 32;
            break;
        }

        pwm_cfg.uiPrd = pmotor_cfg->uiPwmBaseNum;
        pwm_cfg.uiFall = pmotor_cfg->uiPwmBaseNum / 2;
        pwm_cfg.uiOnCycle = step_pos;
        pwm_pwmConfig(pmotor_cfg->uiPwmOutputPin, &pwm_cfg);
        pwm_pwmEnable(pmotor_cfg->uiPwmOutputPin);

        pwm_wait(pmotor_cfg->uiPwmOutputPin, PWM_TYPE_PWM);

        pmotor_cfg->iMotorStatus = MOTOR_STATUS_FREE;

        if (pmotor_cfg->bSetFinalStep == TRUE) {
            LENS_DELAY_MS(3);    // 3 msec delay for power down
 
            LV8044_TransmitCmd(pmotor_cfg->eChnelSel, LV8044_CMD_SET_CURR, 3);

            pmotor_cfg->iWorkStatus = MOTOR_STATUS_FREE;
            pmotor_cfg->bSetFinalStep = FALSE;
        }
    } else {
        LV8044_TransmitCmd(pmotor_cfg->eChnelSel, LV8044_CMD_SET_CURR, 3);
    }

    return step_pos;
}


static void motorLV8044_Init(PLENS_DEVICE_OBJ plens_dev)
{
    UINT8 i;
    UINT32 *pinit_reg;

    LV8044_InitSPI();

    // initialize GPIO
    for (i = 0; i < LENS_IO_MAX; i++) {
        g_LensMotorInfo.uiPinIn[i] = plens_dev->pGPIOArray[i];
    }

    // reset motor driver IC
    if (g_LensMotorInfo.uiPinIn[LENS_IO_RESET] > 0) {
        gpio_clearPin(g_LensMotorInfo.uiPinIn[LENS_IO_RESET]);
        LENS_DELAY_MS(5);
        gpio_setPin(g_LensMotorInfo.uiPinIn[LENS_IO_RESET]);
    }

    // write initial table to LV8044 register
    pinit_reg = (UINT32 *)&g_LensMotorInfo.LensMotorReg.REG[0];

    for (i = 0; i < 8; i++) {
        LV8044_Write2SPI(pinit_reg[i]);
        LENS_DELAY_MS(5);

        DBG_MSG("SPI read back @ 0x%02X = 0x%02X\r\n", i, LV8044_Read4SPI(i));
    }

    LV8044_Read4SPI(0x00);    // add dumy code for compile

    LV8044_MotorInit(MOTOR_ZOOM);
    LV8044_MotorInit(MOTOR_FOCUS);
}


static void motorLV8044_SetSignal(MD_SIGNAL signal, UINT32 on)
{
    T_CHNEL12_MOVE_CTRL_01 reg_0x01;
    T_CHNEL34_MOVE_CTRL_03 reg_0x03;

    DBG_MSG("%s signal=%u, on/off=%u\r\n", __func__, signal, (on != 0 ? TRUE : FALSE));

    switch (signal) {
    case MD_SIGNAL_STANDBY:    // power on/off motor driver
        reg_0x01.Reg = g_LensMotorInfo.LensMotorReg.REG[1].Reg;    // focus or zoom
        reg_0x03.Reg = g_LensMotorInfo.LensMotorReg.REG[3].Reg;    // zoom or focus
        reg_0x01.Bit.ENDISAB = reg_0x03.Bit.ENDISCD = (on > 0) ? 1 : 0;

        LV8044_Write2SPI(reg_0x01.Reg);
        LV8044_Write2SPI(reg_0x03.Reg);
        g_LensMotorInfo.LensMotorReg.REG[1].Reg = reg_0x01.Reg;
        g_LensMotorInfo.LensMotorReg.REG[3].Reg = reg_0x03.Reg;
        break;

    case MD_SIGNAL_ZPI:    // enable ZPI output signal
        break;

    case MD_SIGNAL_ZPR:    // enable ZPR output signal
        break;

    case MD_SIGNAL_FPI:    // enable FPI output signal
        break;

    default:
        DBG_ERR("%s unknown!\r\n", __func__);
        break;
    }
}


static UINT32 motorLV8044_GetSignal(MD_SIGNAL signal)
{
    UINT32 io_status = 0;
    T_CHNEL12_MOVE_CTRL_01 reg_0x01;
    T_CHNEL34_MOVE_CTRL_03 reg_0x03;

    DBG_MSG("%s signal=%u\r\n", __func__, signal);

    switch (signal) {
    case MD_SIGNAL_STANDBY:
        reg_0x01.Reg = g_LensMotorInfo.LensMotorReg.REG[1].Reg;    // focus or zoom
        reg_0x03.Reg = g_LensMotorInfo.LensMotorReg.REG[3].Reg;    // zoom or focus

        if (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == LV8044_CHNEL1) {
            io_status = (reg_0x01.Bit.ENDISAB) ? (io_status | 0x01) : io_status;
            io_status = (reg_0x03.Bit.ENDISCD) ? (io_status | 0x02) : io_status;
        } else if (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == LV8044_CHNEL3) {
            io_status = (reg_0x03.Bit.ENDISCD) ? (io_status | 0x01) : io_status;
            io_status = (reg_0x01.Bit.ENDISAB) ? (io_status | 0x02) : io_status;
        }
        break;

    case MD_SIGNAL_ZPI:
        break;

    case MD_SIGNAL_ZPR:
        break;

    case MD_SIGNAL_FPI:
        break;

    default:
        DBG_ERR("%s unknown!\r\n", __func__);
        break;
    }

    return io_status;
}


static UINT32 motorLV8044_GetBusyState(void)
{
    return (UINT32)((g_LensMotorInfo.LensMotorConfig[0].iWorkStatus << 1) | g_LensMotorInfo.LensMotorConfig[1].iWorkStatus);
}


//----------------------------------------------------------------------------------------------------------------------
// zoom
//----------------------------------------------------------------------------------------------------------------------
static INT32 LV8044_Zoom2Tele(INT32 steps, const INT32 gear_backlash)
{
    INT32 set_step = 0;

    if (LV8044_WaitMoveEnd(MOTOR_ZOOM, 200) == 0) {
        set_step = steps + gear_backlash;

        if (set_step != 0) {
            g_LensMotorInfo.LensMotorConfig[1].iMoveDir = MOVE_DIR_FWD;
        }

        LV8044_SetMotorPosition(MOTOR_ZOOM, set_step);

        g_LensMotorInfo.LensMotorConfig[1].iCurrPosition += steps;
    }

    return set_step;
}


static INT32 LV8044_Zoom2Wide(INT32 steps, const INT32 gear_backlash)
{
    INT32 set_step = 0;

    if (LV8044_WaitMoveEnd(MOTOR_ZOOM, 200) == 0) {
        set_step = steps + gear_backlash;

        if (set_step != 0) {
            g_LensMotorInfo.LensMotorConfig[1].iMoveDir = MOVE_DIR_BWD;
        }

        LV8044_SetMotorPosition(MOTOR_ZOOM, -set_step);

        g_LensMotorInfo.LensMotorConfig[1].iCurrPosition -= steps;
    }

    return set_step;
}


static void LV8044_ZoomSetSpeed(UINT32 speed)
{
    UINT32 i, tmp, idx = 1;

    speed = LENS_CLAMP(speed, SPEED_FULL/5, SPEED_FULL);
    tmp = (speed / (SPEED_FULL / 5)) - 1;

    if (tmp != 0) {
        for (i = 0; i < tmp; i++) {
            idx *= 2;
        }
    }

    g_LensMotorInfo.LensMotorConfig[1].eMoveSpeed = (LV8044_MOTOR_SPEED)idx;
    g_LensMotorInfo.LensMotorConfig[1].uiPwmBaseNum = 60 * (UINT32)LV8044_MOTOR_SPD_1X / (UINT32)g_LensMotorInfo.LensMotorConfig[1].eMoveSpeed;
}


static UINT32 motorLV8044_ZoomGetSpeed(void)
{
    UINT32 i, tmp, idx = 1;

    tmp = (UINT32)g_LensMotorInfo.LensMotorConfig[1].eMoveSpeed;

    for (i = 0; i < 5; i++) {
        tmp /= 2;

        if (tmp != 0) {
            idx++;
        } else {
            break;
        }
    }

    return LENS_CLAMP(idx*SPEED_FULL/5, SPEED_FULL/5, SPEED_FULL);
}


static void motorLV8044_ZoomSetState(MOTOR_ZOOM_ACT state, UINT32 param)
{
    T_CHNEL12_MOVE_CTRL_01 reg_0x01;
    T_CHNEL34_MOVE_CTRL_03 reg_0x03;

    DBG_MSG("%s, state=%d, param=%d\r\n", __func__, state, param);

    switch (state) {
    case MOTOR_ZOOM_BRAKE:
        if (g_LensMotorInfo.LensMotorConfig[1].eChnelSel == LV8044_CHNEL1) {
            reg_0x01.Reg = g_LensMotorInfo.LensMotorReg.REG[1].Reg;
            reg_0x01.Bit.HOLDAB = 1;

            LV8044_Write2SPI(reg_0x01.Reg);
            g_LensMotorInfo.LensMotorReg.REG[1].Reg = reg_0x01.Reg;
        } else if (g_LensMotorInfo.LensMotorConfig[1].eChnelSel == LV8044_CHNEL3) {
            reg_0x03.Reg = g_LensMotorInfo.LensMotorReg.REG[3].Reg;
            reg_0x03.Bit.HOLDCD = 1;

            LV8044_Write2SPI(reg_0x03.Reg);
            g_LensMotorInfo.LensMotorReg.REG[3].Reg = reg_0x03.Reg;
        }
        break;

    case MOTOR_ZOOM_FWD:
        LV8044_Zoom2Tele((INT32)(param & 0xFFFF), (INT32)((param >> 16) & 0xFFFF));
        break;

    case MOTOR_ZOOM_BWD:
        LV8044_Zoom2Wide((INT32)(param & 0xFFFF), (INT32)((param >> 16) & 0xFFFF));
        break;

    case MOTOR_ZOOM_OFF:
        if (g_LensMotorInfo.LensMotorConfig[1].eChnelSel == LV8044_CHNEL1) {
            reg_0x01.Reg = g_LensMotorInfo.LensMotorReg.REG[1].Reg;
            reg_0x01.Bit.ENDISAB = 0;

            LV8044_Write2SPI(reg_0x01.Reg);
            g_LensMotorInfo.LensMotorReg.REG[1].Reg = reg_0x01.Reg;
        } else if (g_LensMotorInfo.LensMotorConfig[1].eChnelSel == LV8044_CHNEL3) {
            reg_0x03.Reg = g_LensMotorInfo.LensMotorReg.REG[3].Reg;
            reg_0x03.Bit.ENDISCD = 0;

            LV8044_Write2SPI(reg_0x03.Reg);
            g_LensMotorInfo.LensMotorReg.REG[3].Reg = reg_0x03.Reg;
        }
        break;

    case MOTOR_ZOOM_SETSPEED:
        LV8044_ZoomSetSpeed(param);
        break;

    default:
        DBG_ERR("%s unknown!\r\n", __func__);
        break;
    }
}


//----------------------------------------------------------------------------------------------------------------------
// focus
//----------------------------------------------------------------------------------------------------------------------
static INT32 LV8044_Focus2Infinite(INT32 steps, const INT32 gear_backlash)
{
    INT32 set_step = 0;

    if (LV8044_WaitMoveEnd(MOTOR_FOCUS, 200) == 0) {
        set_step = steps + gear_backlash;

        if (set_step != 0) {
            g_LensMotorInfo.LensMotorConfig[0].iMoveDir = MOVE_DIR_FWD;
        }

        LV8044_SetMotorPosition(MOTOR_FOCUS, set_step);

        g_LensMotorInfo.LensMotorConfig[0].iCurrPosition += steps;
    }

    return set_step;
}


static INT32 LV8044_Focus2Near(INT32 steps, const INT32 gear_backlash)
{
    INT32 set_step = 0;

    if (LV8044_WaitMoveEnd(MOTOR_FOCUS, 200) == 0) {
        set_step = steps + gear_backlash;

        if (set_step != 0) {
            g_LensMotorInfo.LensMotorConfig[0].iMoveDir = MOVE_DIR_BWD;
        }

        LV8044_SetMotorPosition(MOTOR_FOCUS, -set_step);

        g_LensMotorInfo.LensMotorConfig[0].iCurrPosition -= steps;
    }

    return set_step;
}


static void LV8044_FocusSetSpeed(UINT32 speed)
{
    UINT32 i, tmp, idx = 1;

    speed = LENS_CLAMP(speed, SPEED_FULL/5, SPEED_FULL);
    tmp = (speed / (SPEED_FULL / 5)) - 1;

    if (tmp != 0) {
        for (i = 0; i < tmp; i++) {
            idx *= 2;
        }
    }

    g_LensMotorInfo.LensMotorConfig[0].eMoveSpeed = (LV8044_MOTOR_SPEED)idx;
    g_LensMotorInfo.LensMotorConfig[0].uiPwmBaseNum = 60 * (UINT32)LV8044_MOTOR_SPD_1X / (UINT32)g_LensMotorInfo.LensMotorConfig[0].eMoveSpeed;
}


static UINT32 motorLV8044_FocusGetSpeed(void)
{
    UINT32 i, tmp, idx = 1;

    tmp = (UINT32)g_LensMotorInfo.LensMotorConfig[0].eMoveSpeed;

    for (i = 0; i < 5; i++) {
        tmp /= 2;

        if (tmp != 0) {
            idx++;
        } else {
            break;
        }
    }

    return LENS_CLAMP(idx*SPEED_FULL/5, SPEED_FULL/5, SPEED_FULL);
}


static BOOL motorLV8044_FocusSetState(MOTOR_FOCUS_ACT state, UINT32 param)
{
    BOOL ret_state = TRUE;
    T_CHNEL12_MOVE_CTRL_01 reg_0x01;
    T_CHNEL34_MOVE_CTRL_03 reg_0x03;

    DBG_MSG("%s, state=%d, param=%d\r\n", __func__, state, param);

    switch (state) {
    case MOTOR_FOCUS_EXCITE_ON:
        if (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == LV8044_CHNEL1) {
            reg_0x01.Reg = g_LensMotorInfo.LensMotorReg.REG[1].Reg;
            reg_0x01.Bit.ENDISAB = 1;

            LV8044_Write2SPI(reg_0x01.Reg);
            g_LensMotorInfo.LensMotorReg.REG[1].Reg = reg_0x01.Reg;
        } else if (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == LV8044_CHNEL3) {
            reg_0x03.Reg = g_LensMotorInfo.LensMotorReg.REG[3].Reg;
            reg_0x03.Bit.ENDISCD = 1;

            LV8044_Write2SPI(reg_0x03.Reg);
            g_LensMotorInfo.LensMotorReg.REG[3].Reg = reg_0x03.Reg;
        }
        break;

    case MOTOR_FOCUS_FWD:
        LV8044_Focus2Infinite((INT32)(param & 0xFFFF), (INT32)((param >> 16) & 0xFFFF));
        break;

    case MOTOR_FOCUS_BWD:
        LV8044_Focus2Near((INT32)(param & 0xFFFF), (INT32)((param >> 16) & 0xFFFF));
        break;

    case MOTOR_FOCUS_EXCITE_OFF:
        if (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == LV8044_CHNEL1) {
            reg_0x01.Reg = g_LensMotorInfo.LensMotorReg.REG[1].Reg;
            reg_0x01.Bit.ENDISAB = 0;

            LV8044_Write2SPI(reg_0x01.Reg);
            g_LensMotorInfo.LensMotorReg.REG[1].Reg = reg_0x01.Reg;
        } else if (g_LensMotorInfo.LensMotorConfig[0].eChnelSel == LV8044_CHNEL3) {
            reg_0x03.Reg = g_LensMotorInfo.LensMotorReg.REG[3].Reg;
            reg_0x03.Bit.ENDISCD = 0;

            LV8044_Write2SPI(reg_0x03.Reg);
            g_LensMotorInfo.LensMotorReg.REG[3].Reg = reg_0x03.Reg;
        }
        break;

    case MOTOR_FOCUS_SETSPEED:
        LV8044_FocusSetSpeed(param);
        break;

    default:
        DBG_ERR("%s unknown!\r\n", __func__);
        break;
    }

    return ret_state;
}


//----------------------------------------------------------------------------------------------------------------------
// aperture
//----------------------------------------------------------------------------------------------------------------------
static void motorLV8044_ApertureSetState(MOTOR_APERTURE_ACT state, UINT32 param)
{
    DBG_MSG("not support!\r\n");

    switch (state) {
    case MOTOR_APERTURE_NORMAL:
        break;

    case MOTOR_APERTURE_DUMMYLOAD:
        break;

    case MOTOR_APERTURE_FWD:
        break;

    case MOTOR_APERTURE_BWD:
        break;

    case MOTOR_APERTURE_EXCITE_ON:
        break;

    case MOTOR_APERTURE_EXCITE_OFF:
        break;

    case MOTOR_APERTURE_RESET:
        break;

    default:
        DBG_ERR("%s unknown!\r\n", __func__);
        break;
    }
}


//----------------------------------------------------------------------------------------------------------------------
// shutter
//----------------------------------------------------------------------------------------------------------------------
static void motorLV8044_ShutterSetState(MOTOR_SHUTTER_ACT state, UINT32 param)
{
    DBG_MSG("not support!\r\n");

    switch (state) {
    case MOTOR_SHUTTER_NORMAL:
        break;

    case MOTOR_SHUTTER_CAP:
        break;

    case MOTOR_SHUTTER_DUMMYLOAD:
        break;

    case MOTOR_SHUTTER_PSEUDO_OPEN:
        break;

    case MOTOR_SHUTTER_PSEUDO_CLOSE:
        break;

    default:
        DBG_ERR("%s unknown!\r\n", __func__);
        break;
    }
}


//----------------------------------------------------------------------------------------------------------------------
// IR cut
//----------------------------------------------------------------------------------------------------------------------
static void motorLV8044_IrcutSetState(MOTOR_IRCUT_ACT state, UINT32 param)
{
    param = LENS_MIN(param, 200);

    if (state == MOTOR_IRCUT_OPEN) {
        gpio_setPin(g_LensMotorInfo.uiPinIn[LENS_IO_IN_0]);
    } else if (state == MOTOR_IRCUT_CLOSE) {
        gpio_setPin(g_LensMotorInfo.uiPinIn[LENS_IO_IN_1]);
    }

    LENS_DELAY_MS(param);

    gpio_clearPin(g_LensMotorInfo.uiPinIn[LENS_IO_IN_0]);
    gpio_clearPin(g_LensMotorInfo.uiPinIn[LENS_IO_IN_1]);
}


