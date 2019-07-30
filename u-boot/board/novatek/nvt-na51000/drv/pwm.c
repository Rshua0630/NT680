/*
    @file       pwm.c
    @ingroup    mIDrvIO_PWM

    @brief      Driver file for PWM module
                This file is the driver file that define the driver API for PWM module.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <common.h>
#include <linux/types.h>
#include <asm/io.h>
#include "pwm-reg.h"
#include "pwm-int.h"

/*
    PWM Clock enable/disable function

    @return Return void
*/
static void pwm_enable_clk(BOOL enable, u32 channel)
{
	u32 pwm_clk_reg = 0;

	HAL_READ_UINT32(IOADDR_CG_PWM_REG_BASE, pwm_clk_reg);

	if(enable)
		pwm_clk_reg |= 1 << channel;
	else
		pwm_clk_reg &= ~(1 << channel);

	HAL_WRITE_UINT32(IOADDR_CG_PWM_REG_BASE, pwm_clk_reg);
}


/*
    PWM Set clock rate function

    @return Return void
*/
static void pwm_clk_set_rate(u32 channel, u32 div)
{
	u32 clkdiv_reg = 0;

	if (!div)
		div = 1;

	switch(channel) {
	case PWMID_NO_0:
	case PWMID_NO_1:
	case PWMID_NO_2:
	case PWMID_NO_3:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		break;

	case PWMID_NO_4:
	case PWMID_NO_5:
	case PWMID_NO_6:
	case PWMID_NO_7:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE, clkdiv_reg);
		break;

	case PWMID_NO_8:
	case PWMID_NO_9:
	case PWMID_NO_10:
	case PWMID_NO_11:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		break;

	case PWMID_NO_12:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x4, clkdiv_reg);
		break;

	case PWMID_NO_13:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		break;

	case PWMID_NO_14:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x8, clkdiv_reg);
		break;

	case PWMID_NO_15:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0xC, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0xC, clkdiv_reg);
		break;

	case PWMID_NO_16:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0xC, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0xC, clkdiv_reg);
		break;

	case PWMID_NO_17:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x10, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x10, clkdiv_reg);
		break;

	case PWMID_NO_18:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x10, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF0000;
		clkdiv_reg |= div << 16;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x10, clkdiv_reg);
		break;

	case PWMID_NO_19:
		HAL_READ_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x14, clkdiv_reg);
		clkdiv_reg &= ~0x3FFF;
		clkdiv_reg |= div;
		HAL_WRITE_UINT32(IOADDR_CLKRATE_PWM_REG_BASE + 0x14, clkdiv_reg);
		break;

	default:
		printf("unsupport ID\n");
	}
}

/*
    PWM Polling function

    Status checking point for PWM driver.

    @return Return void
*/
static void pwm_polling(PWM_STS_SELECTION pwm_status, int channel)
{
	u32 tmpStatus;
	union PWM_CTRL_INTSTS       pwm_int_sts;
	union PWM_MS_CTRL_INTSTS    ms_int_sts;
	union PWM_CLKDIV_LOAD_STS   pwm_clkdiv_load_sts;


	switch (pwm_status) {
	case PWM_MS_STS:
		while (1) {
			ms_int_sts.reg = PWM_GETREG(PWM_MS_CTRL_INTSTS_OFS);
			tmpStatus = ms_int_sts.reg & (1 << channel);
			if (tmpStatus) {
				PWM_SETREG(PWM_MS_CTRL_INTSTS_OFS, ms_int_sts.reg);
				break;
			}
		}

		break;
	case PWM_STS:
		while (1) {
			pwm_int_sts.reg = PWM_GETREG(PWM_CTRL_INTSTS_OFS);
			tmpStatus = pwm_int_sts.reg & (1 << channel);
			if (tmpStatus) {
				PWM_SETREG(PWM_CTRL_INTSTS_OFS, pwm_int_sts.reg);
				break;
			}
		}
		break;

	case PWM_CLKLOAD_STS:
		while (1) {
			pwm_clkdiv_load_sts.reg = PWM_GETREG(PWM_CLKDIV_LOAD_STS_OFS);
			tmpStatus = pwm_clkdiv_load_sts.reg & 0x7;
			if (tmpStatus & (1 << channel)) {
				PWM_SETREG(PWM_CLKDIV_LOAD_STS_OFS, (pwm_clkdiv_load_sts.reg & 0x7));
				break;
			}
		}

		break;

	case PWM_TGTCNT_STS:
		while (1) {
			pwm_clkdiv_load_sts.reg = PWM_GETREG(PWM_CLKDIV_LOAD_STS_OFS);
			tmpStatus = ((pwm_clkdiv_load_sts.reg & 0x70000) >> 16);
			if (tmpStatus & (1 << channel)) {
				PWM_SETREG(PWM_CLKDIV_LOAD_STS_OFS, (pwm_clkdiv_load_sts.reg & 0x70000));
				break;
			}
		}

		break;

	default:
		break;

	}
}

/*
    Check PWM ID

    Check whether the PWM ID is valid or not

    @param[in] uiStartBit   Start search bit
    @param[in] uiPWMId      PWM ID (bitwise), one ID at a time

    @return Check ID status
        - @b PWM_INVALID_ID: Not a valid PWM ID
        - @b Other: ID offset (0 ~ 2 for PWM, 4 for CCNT)
*/
static u32 pwm_isValidId(u32 uiStartBit, u32 uiPWMId)
{
	u32 i;
	for(i=uiStartBit; i< (PWM_ALLCH_BITS); i++) {
		if(uiPWMId & (1<<i))
			return i;
	}

	return PWM_INVALID_ID;
}



/*
    Get if PWM enable

    Get specific PWM channel is enabled or not

    @param[in] uiPWMId PWM ID(one PWM id per time)

    @return Start status
        - @b TRUE   : Enabled
        - @b FALSE  : Disabled
*/
static BOOL pwm_pwmIsEn(u32 uiPWMId)
{
	if((PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId) == uiPWMId)
		return TRUE;
	else
		return FALSE;
}


/*
    Attach PWM driver

    Pre-initialize PWM driver before PWM driver is opened.
    This function should be the very first function to be invoked.

    @param[in] uiEnumPWMID      PWM ID (enum), one ID at a time.

    @return void
*/

static void pwm_attach(u32 uiEnumPWMID)
{
	u32 pwm_pinmux_reg = 0;

	if(uiEnumPWMID >= PWMID_NO_TOTAL_CNT)
		return;

	/*Enable PWM Pinmux*/
	HAL_READ_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg |= (1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	HAL_READ_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg &= ~(1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);

	/*Release reset*/
	HAL_READ_UINT32(IOADDR_RESET_PWM_REG_BASE, pwm_pinmux_reg);

	if (!(pwm_pinmux_reg & 0x100)) {
		pwm_pinmux_reg |= 0x100;

		HAL_WRITE_UINT32(IOADDR_RESET_PWM_REG_BASE, pwm_pinmux_reg);
	}

	pwm_enable_clk(ENABLE, uiEnumPWMID);

}


/*
    Detach PWM driver

    De-initialize PWM driver after SPI driver is closed.

    @param[in] uiEnumPWMID  PWM ID (enum), one ID at a time.

    @return void
*/
static void pwm_detach(u32 uiEnumPWMID)
{
	u32 pwm_pinmux_reg = 0;

	if(uiEnumPWMID >= PWMID_NO_TOTAL_CNT)
		return;

	pwm_enable_clk(DISABLE, uiEnumPWMID);

	/*Disable PWM Pinmux*/
	HAL_READ_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg &= ~(1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX_PWM_REG_BASE, pwm_pinmux_reg);

	HAL_READ_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);

	pwm_pinmux_reg |= (1 << uiEnumPWMID);

	HAL_WRITE_UINT32(IOADDR_PINMUX2_PWM_REG_BASE, pwm_pinmux_reg);
}
/*
    Internal open flow.

    Internal open function shared by pwm_open() and pwm_open_NoSwitchPinmux().

    @param[in] uiPWMId          PWM ID (bitwise), allow multiple ID at a time.

    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Open PWM failed
*/
static int pwm_openInternal(u32 uiPWMId)
{
	int                      erReturn;
	u32                     uiEnumPWMID;

	union PWM_CTRL_INTEN        pwm_ctrl_int_en;
	union PWM_CTRL_INTSTS       pwm_ctrl_int_sts;

	union PWM_MS_CTRL_INTEN     ms_ctrl_int_en;
	union PWM_MS_CTRL_INTSTS    ms_ctrl_int_sts;


	if ((uiEnumPWMID = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID!\r\n");
		return E_PAR;
	}

	// Clear all PWM interrupt status
	pwm_ctrl_int_sts.reg = PWM_GETREG(PWM_CTRL_INTSTS_OFS);
	pwm_ctrl_int_sts.reg |= PWM_INT_STS_ALL;
	PWM_SETREG(PWM_CTRL_INTSTS_OFS, pwm_ctrl_int_sts.reg);

	// Clear all MS interrupt status
	ms_ctrl_int_sts.reg = PWM_GETREG(PWM_MS_CTRL_INTSTS_OFS);
	ms_ctrl_int_sts.reg |= PWM_INT_MS_STS_ALL;
	PWM_SETREG(PWM_MS_CTRL_INTSTS_OFS, ms_ctrl_int_sts.reg);

	// Only enable specific id (opened)
	// Enable specific PWM interrupts
	pwm_ctrl_int_en.reg = PWM_GETREG(PWM_CTRL_INTEN_OFS);
	pwm_ctrl_int_en.reg |= (1<<uiEnumPWMID);
	PWM_SETREG(PWM_CTRL_INTEN_OFS, pwm_ctrl_int_en.reg);

	// Enable specific MS interrupts
	ms_ctrl_int_en.reg = PWM_GETREG(PWM_MS_CTRL_INTEN_OFS);
	ms_ctrl_int_en.reg |= (1<<uiEnumPWMID);
	PWM_SETREG(PWM_MS_CTRL_INTEN_OFS, ms_ctrl_int_en.reg);


	// Enable clock and switch pinmux
	pwm_attach(uiEnumPWMID);

	return erReturn;
}

/*
    Internal close flow.

    Internal close function shared by pwm_close() and pwm_closeNoPatch().

    @param[in] uiPWMId      PWM ID (bitwise), one ID at a time
    @param[in] bWait        driver is closed after waiting PWM signal level back to correct level

    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Close PWM failed
*/
static int pwm_closeInternal(u32 uiPWMId, BOOL bWait)
{
	u32 uiEnumPWMID;
	int erReturn;
	union PWM_CTRL_INTEN        pwm_ctrl_int_en;
	union PWM_MS_CTRL_INTEN     ms_ctrl_int_en;


	if ((uiEnumPWMID = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID)
		return E_PAR;

	// Stop at close
	if(bWait == TRUE) {
		if (PWM_GETREG(PWM_ENABLE_OFS) & (1<<uiEnumPWMID)) {
			erReturn = pwm_pwmDisable(uiPWMId);
			if (erReturn != E_OK)
				return erReturn;
		}
		pwm_wait(uiPWMId, PWM_TYPE_PWM);
	} else {
		if (PWM_GETREG(PWM_ENABLE_OFS) & (1<<uiEnumPWMID)) {
			erReturn = pwm_pwmDisable(uiPWMId);
			if (erReturn != E_OK)
				return erReturn;
		}
	}

	// Only enable specific id (opened)
	// Enable specific PWM interrupts
	pwm_ctrl_int_en.reg = PWM_GETREG(PWM_CTRL_INTEN_OFS);
	pwm_ctrl_int_en.reg &= ~(1<<uiEnumPWMID);
	PWM_SETREG(PWM_CTRL_INTEN_OFS, pwm_ctrl_int_en.reg);

	// Enable specific MS interrupts
	ms_ctrl_int_en.reg = PWM_GETREG(PWM_MS_CTRL_INTEN_OFS);
	ms_ctrl_int_en.reg &= ~(1<<uiEnumPWMID);
	PWM_SETREG(PWM_MS_CTRL_INTEN_OFS, ms_ctrl_int_en.reg);

	// Disable clock and switch pinmux back to GPIO
	pwm_detach(uiEnumPWMID);

	return E_OK;
}


/**
    @name PWM public driver API
*/
//@{
/**
    Open specific pwm channel.

    If want to access pwm channel, need call pwm open API, will\n
    lock related semphare, enable clock and interrupt.

    @note   pwm_open do not switch pinmux(config at top API), only obtain semphare,\n
            and enable clock

    @param[in] uiPWMId  pwm id, one id at a time
    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Open PWM failed
*/
int pwm_open(u32 uiPWMId)
{
	return pwm_openInternal(uiPWMId);
}

/**
    Open specific pwm micro step by set unit

    where PWM0-3 : set 0, PWM4-7 : set 1, PWM8-11 : set 2, PWM12-15 : set 3

    @note pwm_openSet do not switch pinmux(config at top API)

    @param[in] uiMSSet  pwm set id, set0:PWM0-3, set1:PWM4-7, set2:PWM8-11, set3:PWM12-15
    @return Open PWM set status
        - @b E_OK: Success
        - @b Others: Open PWM set failed
*/
int pwm_openSet(PWM_MS_CHANNEL_SET uiMSSet)
{
	u32 uiSetCnt;
	for(uiSetCnt = (uiMSSet * 4); uiSetCnt < ((uiMSSet + 1) * 4); uiSetCnt++) {
		if(pwm_openInternal(1<<uiSetCnt) != E_OK)
			return E_SYS;
	}
	return E_OK;
}



/**
    Close specific pwm channel.

    Let other tasks to use specific pwm channel, including set specific channel\n
    disable (finish at an end of cycle). Disable clock, and unlock semphare

    @note Because of pwm_close() include pwm_pwmDisable(), so MUST called behind\n
          pwm_pwmEnable(). In other word, it's illegal that called pwm_close(pwm_ch, TRUE)\n
          without calling pwm_pwnEnable(pwm_ch).

    @param[in] uiPWMId                  pwm id, one id at a time(PWMID_0, PWMID_1 ... PWMID_19)
    @param[in] bWaitAutoDisableDone     wait a complete cycle done or not after pwm disable completed.\n
                                        (system will halt once if close a disabled PWM channel.)

    Example:
    @code
    {
        pwm_open(pwm_ch_x);
        pwm_pwmConfig(pwm_ch_x, xxx);
        pwm_pwmEnable(pwm_ch_x);

        //case1:
        pwm_pwmDisable(pwm_ch_x);
        //Need wait disable done
        pwm_wait(pwm_ch_x);
        //Fill FALSE due to already disable done
        pwm_close(pwm_ch_x, FALSE);

        //case2:
        //Fill TRUE if pwm_close called directly
        pwm_close(pwm_ch_x, TRUE);

        //Flow of case1 are equal to case2
    }
    @endcode
    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Close PWM failed
*/
static int drv_pwm_close(u32 uiPWMId, BOOL bWaitAutoDisableDone)
{
	return pwm_closeInternal(uiPWMId, bWaitAutoDisableDone);
}

/**
    Close micro step by set.

    Let other tasks to use specific pwm channel, include set specific \n
    channel disable (finish at an end of cycle). Disable clock, and \n
    unlock semphare

    @param[in] uiMSSet                  pwm set id, set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15
    @param[in] bWaitAutoDisableDone     wait a complete cycle done or not after pwm disable completed.\n
                                        (system will halt once if close a disabled PWM channel within uiMSSet)

    @return Open PWM status
            - @b E_OK: Success
            - @b Others: Close PWM failed
*/
int pwm_closeSet(PWM_MS_CHANNEL_SET uiMSSet, BOOL bWaitAutoDisableDone)
{
	u32 uiSetCnt;
	for(uiSetCnt = (uiMSSet * 4); uiSetCnt < (((uiMSSet + 1) * 4) - 1); uiSetCnt++)
		pwm_closeInternal((1<<uiSetCnt), FALSE);

	return pwm_closeInternal((1<<(((uiMSSet + 1) * 4) - 1)), bWaitAutoDisableDone);
}



/**
    Get cycle count number.

    Get PWM(or micro step) cycle count operation number

    @param uiPWMId      PWM ID (from PWMID_CCNT_NO_0 ~ PWMID_CCNT_NO_3)

    @return Set cycle count parameter status
        - @b Cycle count
*/
u32 pwm_pwmGetCycleNumber(u32 uiPWMId)
{
	u32 uiEnumPWMID;
	union PWM_CTRL_REG_BUF  uiCtrlReg;

	if ((uiEnumPWMID = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID!\r\n");
		return E_PAR;
	}

	if(pwm_isValidId(uiEnumPWMID+1, uiPWMId) != PWM_INVALID_ID) {
		printf("more than 1 channel to wait done at one time\r\n");
		return E_SYS;
	}

	if(uiEnumPWMID < 16)
		uiCtrlReg.reg = PWM_GETREG(PWM_CTRL_REG_BUF_OFS + uiEnumPWMID*PWM_PER_CH_OFFSET);
	else
		uiCtrlReg.reg = PWM_GETREG(PWM_CTRL_REG_BUF_OFS + uiEnumPWMID*PWM_PER_CH_OFFSET + 0x20);

	return (u32)uiCtrlReg.bit.pwm_on;
}

/**
    Set PWM clock divided by micro step set unit(for micro step usage)

    Set clock divid by micro step set unit where\n
    set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15

    @param[in] uiMSSet  pwm set id, set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15
    @param[in] uiDiv    PWM divid
    @return Set cycle count parameter status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_mstepConfigClockDiv(PWM_MS_CHANNEL_SET uiMSSet, u32 uiDiv)
{
	u32  uiOffset;
	u32  uiSetCnt;
	int ret = E_OK;

	if(uiMSSet <= PWM_MS_SET_2) {
		uiOffset = (uiMSSet * 4);
		pwm_clk_set_rate(uiOffset, uiDiv);
	} else if(uiMSSet == PWM_MS_SET_3) {
		for(uiSetCnt = (uiMSSet * 4); uiSetCnt < ((uiMSSet + 1) * 4); uiSetCnt++)
			pwm_clk_set_rate(uiSetCnt, uiDiv);
	} else
		ret = E_PAR;

	return ret;
}


/**
    Set PWM clock divided by micro step set unit(for micro step usage)

    Set clock divid by micro step set unit where\n
    set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15

    @param[in] uiClkSrc     pwm clock divided source
    @param[in] uiDiv        PWM divid

    @note for PWM_CLOCK_DIV
    @return Set cycle count parameter status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmConfigClockDiv(PWM_CLOCK_DIV uiClkSrc, u32 uiDiv)
{
	switch(uiClkSrc) {
	case PWM0_3_CLKDIV:
		if((PWM_GETREG(PWM_ENABLE_OFS) & (PWMID_0 | PWMID_1 | PWMID_2 | PWMID_3)) != 0) {
			debug("PWM0~3 are using same clock div, make sure PWM0~3 are all disabled\r\n");
			return E_SYS;
		}

		pwm_clk_set_rate(pwm_isValidId(0, PWMID_0), uiDiv);
		break;

	case PWM4_7_CLKDIV:
		if((PWM_GETREG(PWM_ENABLE_OFS) & (PWMID_4 | PWMID_5| PWMID_6 | PWMID_7)) != 0) {
			debug("PWM4~7 are using same clock div, make sure PWM4~7 are all disabled\r\n");
			return E_SYS;
		}

		pwm_clk_set_rate(pwm_isValidId(0, PWMID_4), uiDiv);
		break;

	case PWM8_11_CLKDIV:
		if((PWM_GETREG(PWM_ENABLE_OFS) & (PWMID_8 | PWMID_9| PWMID_10 | PWMID_11)) != 0) {
			debug("PWM8~11 are using same clock div, make sure PWM8~11 are all disabled\r\n");
			return E_SYS;
		}

		pwm_clk_set_rate(pwm_isValidId(0, PWMID_8), uiDiv);
		break;

	case PWM12_CLKDIV:
	case PWM13_CLKDIV:
	case PWM14_CLKDIV:
	case PWM15_CLKDIV:
	case PWM16_CLKDIV:
	case PWM17_CLKDIV:
	case PWM18_CLKDIV:
	case PWM19_CLKDIV:
		if(pwm_pwmIsEn(1<<uiClkSrc) == TRUE) {
			printf("make sure PWM%d are disabled\r\n", uiClkSrc);
			return E_SYS;
		}

		pwm_clk_set_rate(uiClkSrc, uiDiv);
		break;
	default:
		printf("Unknown clock source \r\n");
		return E_SYS;
		break;
	}

	return E_OK;
}



/**
    Wait operation done

    Calling this API to wait until the operation of target channel is done

    @param[in] uiPWMId      PWM ID, one ID at a time
    @param[in] uiPwmType    determine is PWM, CCNT or micro step, wait different flag
    @return Operation status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_wait(u32 uiPWMId, PWM_TYPE uiPwmType)
{
	u32 uiEnumPWMID;
	int uiResult = E_OK;

	if ((uiEnumPWMID = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID!\r\n");
		return E_PAR;
	}

	if(pwm_isValidId(uiEnumPWMID+1, uiPWMId) != PWM_INVALID_ID) {
		printf("more than 1 channel to wait done at one time\r\n");
		return E_SYS;
	}


	if(uiPwmType == PWM_TYPE_PWM)
		pwm_polling(PWM_STS, uiEnumPWMID);
	else
		pwm_polling(PWM_MS_STS, uiEnumPWMID);

	return uiResult;
}

/**
    Wait operation done by set

    Calling this API to wait until the operation of target channel is done

    @param[in] uiMSSet  pwm set id, set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15

    @return Open PWM status
        - @b E_OK: Success
        - @b Others: Close PWM failed
*/
int pwm_waitSet(PWM_MS_CHANNEL_SET uiMSSet)
{
	u32 uiSetCnt;
	int uiResult = E_OK;
	uiSetCnt = (uiMSSet * 4);
	uiResult = pwm_wait((1<<uiSetCnt), PWM_TYPE_MICROSTEP);
	return uiResult;
}

/**
    Wait micro step clock load done

    After micro step clock load done,  this API to wait until the operation of target channel is done

    @param[in] uiPWMClkDivSrc   clock div source(only valid @ div_0_3 / div_4_7 / div_8_11
    @return Operation status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_mstepClkDivWaitLoadDone(PWM_CLOCK_DIV uiPWMClkDivSrc)
{

	if (uiPWMClkDivSrc > PWM8_11_CLKDIV) {
		printf("Invalid PWM clock div src only 0-3/4-7/8-11 can be load during operation\r\n");
		return E_PAR;
	}

	pwm_polling(PWM_CLKLOAD_STS, uiPWMClkDivSrc);

	return E_OK;
}

/**
    Wait micro step target count arrived

    Micro step target count engine arrived target count value

    @param[in] uiTargetCnt      clock div source(only valid @ div_0_3 / div_4_7 / div_8_11
    @return Operation status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_mstepTargetCountWaitDone(PWM_TGT_COUNT uiTargetCnt)
{

	if (uiTargetCnt >= PWM_TGT_CNT_NUM) {
		printf("Invalid PWM clock div src only 0-3/4-7/8-11 can be load during operation\r\n");
		return E_PAR;
	}

	pwm_polling(PWM_TGTCNT_STS, uiTargetCnt+3);
	return E_OK;
}

//@}


/**
    @name PWM pwm function related API
*/
//@{
/**
    Enable(Start) PWM

    Start PWM based on PWM parameter descriptor set by pwm_set().

    @param[in] uiPWMId PWM ID, could be OR's of any PWM ID

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmEnable(u32 uiPWMId)
{
	union PWM_ENABLE pwm_ctrl_en;

	if (PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId) {
		printf("pwm_pwmEnable(), do not re enable same channel!! (%x)\r\n", uiPWMId);
		return E_PAR;
	} else {
		pwm_ctrl_en.reg = uiPWMId;
		PWM_SETREG(PWM_ENABLE_OFS, pwm_ctrl_en.reg);
	}
	// Enable PWM_x
	// Because enable bit is in APB clock domain. Need double sync to PWM clock
	// domain. So after PWM enable bit set => read until 1(represent PWM clock domain is active)
	while(!(PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId));
	return E_OK;
}


/**
    Disable(stop) PWM

    Disable PWM if it is running in free run mode.

    @param[in] uiPWMId PWM ID, one ID at a time
    @return Stop status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmDisable(u32 uiPWMId)
{
	union PWM_DISABLE pwm_stop;

	// Disable PWM
	if (PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId) {
		pwm_stop.reg = uiPWMId;
		PWM_SETREG(PWM_DISABLE_OFS, pwm_stop.reg);
		while(PWM_GETREG(PWM_ENABLE_OFS) & uiPWMId);
		return E_OK;
	} else {
		printf("pwm_pwmDisable(), do not disable the channel not enable yet!! (%x)\r\n", uiPWMId);
		return E_PAR;
	}

}

/**
    Enable PWM by micro step set group

    Enable PWM by micro step set group

    @param[in] uiMSSet  pwm set id, set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15

    @note for pwm_pwmEnable()

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmEnableSet(PWM_MS_CHANNEL_SET uiMSSet)
{
	u32 uiPWMEnBit;
	u32 uiSetCnt;
	uiPWMEnBit = 0;

	if (uiMSSet >= PWM_MS_SET_TOTAL) {
		printf("set error\r\n");
		return E_PAR;
	}

	for(uiSetCnt = (uiMSSet * 4); uiSetCnt < ((uiMSSet + 1) * 4); uiSetCnt++)
		uiPWMEnBit |= (1<<uiSetCnt);

	return pwm_pwmEnable(uiPWMEnBit);
}


/**
    Disable PWM by micro stepping set group

    Disable PWM by micro step set group

    @param[in] uiMSSet  pwm set id, set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15

    @note for pwm_pwmEnable()

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmDisableSet(PWM_MS_CHANNEL_SET uiMSSet)
{
	u32 uiPWMEnBit;
	u32 uiSetCnt;
	int uRet = E_OK;
	uiPWMEnBit = 0;

	if (uiMSSet >= PWM_MS_SET_TOTAL) {
		printf("set error\r\n");
		return E_PAR;
	}

	for(uiSetCnt = (uiMSSet * 4); uiSetCnt < ((uiMSSet + 1) * 4); uiSetCnt++)
		uiPWMEnBit |= (1<<uiSetCnt);

	uRet = pwm_pwmDisable(uiPWMEnBit);

	if(uRet != E_OK)
		return uRet;

	return pwm_wait(1<<(uiMSSet * 4), PWM_TYPE_PWM);
}

/**
    Set PWM parameters.

    This function sets PWM duty cycles, repeat count and signal level.\n
    After set, the specified pwm channel can be started and stopped by API\n
    functions pwm_pwmEnable() and pwm_pwmDisable(). If the on cycle is not PWM_FREE_RUN,\n
    PWM will issue an interrupt after all cycles are done.

    @param[in] uiPWMId  pwm id, one id at a time
    @param[in] pPWMCfg  PWM parameter descriptor

    @return Set parameter status.
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID
*/
int pwm_pwmConfig(u32 uiPWMId, PPWM_CFG pPWMCfg)
{
	u32 uiOffset;
	union PWM_CTRL_REG_BUF reg_pwm_ctrl_buf;
	union PWM_PERIOD_REG_BUF reg_pwm_period_buf;
	union PWM_PERIOD_REG_BUF org_pwm_period_buf;
	union PWM_EXPEBD_PERIOD_REG_BUF exp_period_buf = {0};


	reg_pwm_ctrl_buf.reg = 0;
	reg_pwm_period_buf.reg = 0;

	if((uiOffset = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID 0x%08x\r\n", uiPWMId);
		return E_PAR;
	}


	if(uiOffset < 16)
		org_pwm_period_buf.reg = PWM_GETREG(PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET);
	else
		org_pwm_period_buf.reg = PWM_GETREG((PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20));




	if(pPWMCfg->uiPrd < 2) {
		printf("invalid PWM base period %d MUST 2~255\r\n", pPWMCfg->uiPrd);
		return E_PAR;
	}

	reg_pwm_ctrl_buf.bit.pwm_on     = pPWMCfg->uiOnCycle;
	reg_pwm_ctrl_buf.bit.pwm_type   = PWM_TYPE_PWM;
	reg_pwm_period_buf.bit.pwm_r    = pPWMCfg->uiRise;
	reg_pwm_period_buf.bit.pwm_f    = pPWMCfg->uiFall;
	reg_pwm_period_buf.bit.pwm_prd  = pPWMCfg->uiPrd;
	reg_pwm_period_buf.bit.pwm_inv  = pPWMCfg->uiInv;

	if(uiOffset < 8) {
		exp_period_buf.reg = PWM_GETREG(PWM0_EXPEND_PERIOD_REG_BUF_OFS + (uiOffset << 2));
		exp_period_buf.bit.pwm_r    = (pPWMCfg->uiRise >> 8) & 0xFF;
		exp_period_buf.bit.pwm_f    = (pPWMCfg->uiFall >> 8) & 0xFF;
		exp_period_buf.bit.pwm_prd  = (pPWMCfg->uiPrd >> 8) & 0xFF;
		PWM_SETREG(PWM0_EXPEND_PERIOD_REG_BUF_OFS + (uiOffset << 2), exp_period_buf.reg);

	}

	if(org_pwm_period_buf.bit.pwm_inv != pPWMCfg->uiInv && pwm_pwmIsEn(uiPWMId)) {
		printf("Try to change Invert signal but PWM [%d] is enabled already\r\n", pwm_isValidId(0, uiPWMId));
		return E_PAR;
	}

	if(uiOffset < 16) {
		PWM_SETREG(PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_period_buf.reg);
		PWM_SETREG(PWM_PWM0_CTRL_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_ctrl_buf.reg);
	} else {
		PWM_SETREG((PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20), reg_pwm_period_buf.reg);
		PWM_SETREG((PWM_PWM0_CTRL_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20), reg_pwm_ctrl_buf.reg);
	}

	return E_OK;
}

/**
    Set PWM reload parameters.

    During pwm is active, rising and falling time and base period can be changed \n
    without disable pwm\n

    @param[in] uiPWMId      pwm id, one id at a time
    @param[in] iRise        rising time (-1 if not modified)
    @param[in] iFall        falling time(-1 if not modified)
    @param[in] iBasePeriod  base period (-1 if not modified)


    @return Set parameter status.
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID
*/
int pwm_pwmReloadConfig(u32 uiPWMId, u32 iRise, u32 iFall, u32 iBasePeriod)
{
	u32 uiOffset;
	union PWM_PERIOD_REG_BUF reg_pwm_period_buf;
	union PWM_EXPEBD_PERIOD_REG_BUF exp_period_buf = {0};


	reg_pwm_period_buf.reg = 0;

	if ((uiOffset = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID 0x%08x\r\n", uiPWMId);
		return E_PAR;
	}


	if(uiOffset < 16)
		reg_pwm_period_buf.reg = PWM_GETREG(PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET);
	else
		reg_pwm_period_buf.reg = PWM_GETREG((PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20));


	if(iBasePeriod < 2 && iBasePeriod >= 0) {
		printf("invalid PWM base period %d MUST 2~255\r\n", iBasePeriod);
		return E_PAR;
	}

	if(iRise >= 0)
		reg_pwm_period_buf.bit.pwm_r = iRise;

	if(iFall >= 0)
		reg_pwm_period_buf.bit.pwm_f = iFall;

	if(iBasePeriod >= 2)
		reg_pwm_period_buf.bit.pwm_prd  = iBasePeriod;

	if(uiOffset < 16)
		PWM_SETREG(PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_period_buf.reg);
	else
		PWM_SETREG((PWM_PWM0_PRD_OFS+uiOffset*PWM_PER_CH_OFFSET+0x20), reg_pwm_period_buf.reg);

	if(uiOffset < 8) {
		exp_period_buf.reg = PWM_GETREG((PWM0_EXPEND_PERIOD_REG_BUF_OFS + (uiOffset << 2)));
		exp_period_buf.bit.pwm_r    = (iRise >> 8) & 0xFF;
		exp_period_buf.bit.pwm_f    = (iFall >> 8) & 0xFF;
		exp_period_buf.bit.pwm_prd  = (iBasePeriod >> 8) & 0xFF;
		PWM_SETREG((PWM0_EXPEND_PERIOD_REG_BUF_OFS + (uiOffset << 2)), exp_period_buf.reg);
	}

	return E_OK;
}


/**
    Reload PWM

    Reload PWM based on PWM parameter descriptor set by pwm_set().

    @param[in] uiPWMId  PWM ID, could be OR's of any PWM ID

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_pwmReload(u32 uiPWMId)
{
	union PWM_LOAD pwmLoadReg;

	if(pwm_pwmIsEn(uiPWMId) == FALSE) {
		printf("pwm[0x%08x]not enable yet!\r\n", uiPWMId);
		return E_SYS;
	}

	// Reload PWM
	pwmLoadReg.reg = PWM_GETREG(PWM_LOAD_OFS);
	pwmLoadReg.reg |= uiPWMId;

	// Avoid PWM channel(s) are under reloading(wait until done if under reloading)
	while((PWM_GETREG(PWM_LOAD_OFS) & uiPWMId));
	PWM_SETREG(PWM_LOAD_OFS, pwmLoadReg.reg);

	return E_OK;
}

/**
    Reload Micro step clock

    Reload clock divided during micro step operation

    @note Only valid under micro step mode

    @param[in] uiPWMClkDiv  PWM clock divided source
    @param[in] bWaitDone    After load new clock divided, wait load done or not

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_mstepClockDivReload(PWM_CLOCK_DIV uiPWMClkDiv, BOOL  bWaitDone)
{
	u32  uiClkDivStartOfs;
	union PWM_CLKDIV_LOAD pwmClockDivLoadReg;
	union PWM_CLKDIV_LOAD_INTEN pwmClockLoadIntEnReg;
	union PWM_CLKDIV_LOAD_STS pwmClockLoadIntEnSts;

	if(uiPWMClkDiv > PWM8_11_CLKDIV)
		return E_PAR;

	uiClkDivStartOfs = (uiPWMClkDiv << 2);

	if((PWM_GETREG(PWM_ENABLE_OFS) & (0xF << uiClkDivStartOfs)) != (u32)(0xF << uiClkDivStartOfs)) {
		printf("PWM%d - %d not all enable yet!\r\n", uiClkDivStartOfs, uiClkDivStartOfs + 3);
		return E_PAR;
	}

	// Clear load done status
	pwmClockLoadIntEnSts.reg = PWM_GETREG(PWM_CLKDIV_LOAD_STS_OFS);
	pwmClockLoadIntEnSts.reg |= (1<<uiPWMClkDiv);
	PWM_SETREG(PWM_CLKDIV_LOAD_STS_OFS, pwmClockLoadIntEnSts.reg);

	// Enable load done interrupt
	pwmClockLoadIntEnReg.reg = PWM_GETREG(PWM_CLKDIV_LOAD_INTEN_OFS);
	pwmClockLoadIntEnReg.reg |= (1<<uiPWMClkDiv);
	PWM_SETREG(PWM_CLKDIV_LOAD_INTEN_OFS, pwmClockLoadIntEnReg.reg);

	// Reload PWM
	pwmClockDivLoadReg.reg = PWM_GETREG(PWM_CLKDIV_LOAD_OFS);
	pwmClockDivLoadReg.reg |= (1<<uiPWMClkDiv);

	// Avoid PWM channel(s) are under reloading(wait until done if under reloading)
	while((PWM_GETREG(PWM_CLKDIV_LOAD_OFS) & (1<<uiPWMClkDiv)));
	PWM_SETREG(PWM_CLKDIV_LOAD_OFS, pwmClockDivLoadReg.reg);

	if(bWaitDone)
		pwm_mstepClkDivWaitLoadDone(uiPWMClkDiv);

	return E_OK;
}

//@}


/**
    @name PWM micro step function related API
*/
//@{
/**
    Start MSTP

    Start MSTP based on MSTP parameter descriptor set by pwm_ms_set().

    @param uiPWMId PWM ID, one or more ID at a time(bitwise)
    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_mstepEnable(u32 uiPWMId)
{
	union PWM_MS_START pwm_ms_ctrl_en;
	// Enable PWM
	pwm_ms_ctrl_en.reg = PWM_GETREG(PWM_MS_START_OFS);

	if (PWM_GETREG(PWM_MS_START_OFS) & uiPWMId) {
		printf("pwm_mstepEnable(), do not re enable same channel!! (%x)\r\n", uiPWMId);
		return E_PAR;
	} else {
		pwm_ms_ctrl_en.reg = uiPWMId;
		PWM_SETREG(PWM_MS_START_OFS, pwm_ms_ctrl_en.reg);
		return E_OK;
	}
}

/**
    Stop micro step by specific channel

    Stop micro step if it is running in free run mode.

    @param[in] uiPWMId  PWM ID, one ID at a time
    @param[in] bWait    Wait a complete cycle done or not after micro step stopped
    @return Stop status
        - @b E_OK: Success
*/
int pwm_mstepDisable(u32 uiPWMId, BOOL bWait)
{
	union PWM_MS_STOP pwm_ms_stop;

	if (PWM_GETREG(PWM_MS_START_OFS) & uiPWMId){
		pwm_ms_stop.reg = (uiPWMId);
		PWM_SETREG(PWM_MS_STOP_OFS, pwm_ms_stop.reg);
	} else {
		printf("pwm_mstepDisable(), do not disable the channel not enable yet!! (%x)\r\n", uiPWMId);
		return E_PAR;
	}

	if(bWait)
		pwm_wait(uiPWMId, PWM_TYPE_MICROSTEP);

	return E_OK;
}

/**
    Enable micro step by micro step set group

    Enable micro step by micro step set group

    @param[in] uiMSSet  pwm set id, set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15

    @note for pwm_pwmEnable()

    @return Start status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_mstepEnableSet(PWM_MS_CHANNEL_SET uiMSSet)
{
	u32 uiPWMEnBit;
	u32 uiSetCnt;
	uiPWMEnBit = 0;

	if (uiMSSet >= PWM_MS_SET_TOTAL) {
		printf("set error\r\n");
		return E_PAR;
	}

	for(uiSetCnt = (uiMSSet * 4); uiSetCnt < ((uiMSSet + 1) * 4); uiSetCnt++)
		uiPWMEnBit |= (1<<uiSetCnt);

	return pwm_mstepEnable(uiPWMEnBit);
}

/**
    Set micro step parameters.

    This function sets Mirco step parameters.

    @param[in] uiPWMId          PWM ID, one ID at a time
    @param[in] pMSTPCfg         Micro step control configuration

    @return Set micro step result
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or error configuration
*/
int pwm_mstepConfig(u32 uiPWMId, PMSTP_CFG pMSTPCfg)
{
	u32 uiOffset;
	union PWM_CTRL_REG_BUF reg_pwm_ctrl;
	union PWM_MS_EXT_REG_BUF reg_pwm_ms_ctrl;
	u32 uiGradPerStep;
	u32 uiStepPerUnit;

	reg_pwm_ms_ctrl.reg = 0;
	reg_pwm_ctrl.reg = 0;

	if ((uiOffset = pwm_isValidId(0, uiPWMId)) == PWM_INVALID_ID) {
		printf("invalid PWM ID 0x%08x\r\n", uiPWMId);
		return E_PAR;
	}

	reg_pwm_ctrl.reg = PWM_GETREG(PWM_CTRL_REG_BUF_OFS+uiOffset*PWM_PER_CH_OFFSET);
	reg_pwm_ctrl.bit.pwm_on = 0;
	PWM_SETREG(PWM_CTRL_REG_BUF_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_ctrl.reg);

	while(pwm_pwmIsEn(uiPWMId)) {
		reg_pwm_ctrl.reg = PWM_GETREG(PWM_CTRL_REG_BUF_OFS+uiOffset*PWM_PER_CH_OFFSET);
		if(reg_pwm_ctrl.bit.pwm_on == 1)
		break;
	}

	if(pMSTPCfg->uiStepPerPhase % 8 != 0) {
		printf("invalid PWM step per phase param %2d multiple of 8\r\n", pMSTPCfg->uiStepPerPhase);
		return E_PAR;
	}

	if(pMSTPCfg->uiPh > 7) {
		printf("invalid PWM start phase param %d, 0~7\r\n", pMSTPCfg->uiPh);
		return E_PAR;
	}

	if(pMSTPCfg->uiThreshold > 0x63)
	{
		printf("invalid MS threshold %02d MUST less than 100\r\n", (int)pMSTPCfg->uiThreshold);
		return E_PAR;
	}

	reg_pwm_ctrl.bit.pwm_on = pMSTPCfg->uiOnCycle;
	reg_pwm_ctrl.bit.pwm_type = PWM_TYPE_MICROSTEP;
	reg_pwm_ctrl.bit.pwm_ms_dir = pMSTPCfg->uiDir;
	reg_pwm_ctrl.bit.pwm_ms_threshold_en = pMSTPCfg->uiThresholdEn;
	reg_pwm_ctrl.bit.pwm_ms_threshold = pMSTPCfg->uiThreshold;
	reg_pwm_ms_ctrl.bit.pwm_ms_start_grads = pMSTPCfg->uiPh * 64;

	uiGradPerStep = 64 / pMSTPCfg->uiStepPerPhase;

	if(uiGradPerStep == 1)
		reg_pwm_ms_ctrl.bit.pwm_ms_grads_per_step = 0x0;
	else if(uiGradPerStep == 2)
		reg_pwm_ms_ctrl.bit.pwm_ms_grads_per_step = 0x1;
	else if(uiGradPerStep == 4)
		reg_pwm_ms_ctrl.bit.pwm_ms_grads_per_step = 0x2;
	else if(uiGradPerStep == 8)
		reg_pwm_ms_ctrl.bit.pwm_ms_grads_per_step = 0x3;
	else {
		printf("invalid MS threshold %02d MUST less than 100\r\n", (int)pMSTPCfg->uiThreshold);
		return E_PAR;
	}

	if(PWM_MS_1_2_PHASE_TYPE == pMSTPCfg->uiPhaseType)
		uiStepPerUnit = 64 / uiGradPerStep;
	else
		uiStepPerUnit = (64 / uiGradPerStep) * 2;

	if(uiStepPerUnit == 8)
		reg_pwm_ms_ctrl.bit.pwm_ms_step_per_unit = 0x3;
	else if(uiStepPerUnit == 16)
		reg_pwm_ms_ctrl.bit.pwm_ms_step_per_unit = 0x4;
	else if(uiStepPerUnit == 32)
		reg_pwm_ms_ctrl.bit.pwm_ms_step_per_unit = 0x5;
	else if(uiStepPerUnit == 64)
		reg_pwm_ms_ctrl.bit.pwm_ms_step_per_unit = 0x6;
	else if(uiStepPerUnit == 128)
		reg_pwm_ms_ctrl.bit.pwm_ms_step_per_unit = 0x7;
	else
		printf("invalid uiStepPerUnit %d\r\n", uiStepPerUnit);

	PWM_SETREG(PWM_CTRL_REG_BUF_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_ctrl.reg);
	PWM_SETREG(PWM_MS_EXT_REG_BUF_OFS+uiOffset*PWM_PER_CH_OFFSET, reg_pwm_ms_ctrl.reg);

	return E_OK;
}

/**
    Set micro step parameters config by set

    This function sets Mirco step parameters.

    @param[in] uiMSSet              Micro set(4 sets total), PWM0-3(set0), PWM4-7(set1), PWM8-11(set2), PWM12-15(set3)
    @param[in] pMSSetChPhCfg        Start phase of each channel in specific MS set(4 channel / per set)
    @param[in] pMSCommonCfg         Common configuration of micro step

    @note for pwm_ms_set()

    @return Set micro step result
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or error configuration
*/
int pwm_mstepConfigSet(PWM_MS_CHANNEL_SET uiMSSet, PMS_CH_PHASE_CFG pMSSetChPhCfg, PMSCOMMON_CFG pMSCommonCfg)
{
	u32 uiSetCnt, uiCnt;
	MSTP_CFG msChannelCfg;


	if(uiMSSet >= PWM_MS_SET_TOTAL)
		printf("invalid uiMSSet %d\r\n", uiMSSet);

	uiCnt = 0;
	msChannelCfg.uiDir = pMSCommonCfg->uiDir;
	msChannelCfg.uiOnCycle = pMSCommonCfg->uiOnCycle;
	msChannelCfg.uiPhaseType = pMSCommonCfg->uiPhaseType;
	msChannelCfg.uiStepPerPhase = pMSCommonCfg->uiStepPerPhase;
	msChannelCfg.uiThreshold = pMSCommonCfg->uiThreshold;
	msChannelCfg.uiThresholdEn= pMSCommonCfg->uiThresholdEn;

	for(uiSetCnt = (uiMSSet * 4); uiSetCnt < ((uiMSSet + 1) * 4); uiSetCnt++) {
		if(uiCnt == 0)
			msChannelCfg.uiPh = pMSSetChPhCfg->uiCH0_Phase;
		else if(uiCnt == 1)
			msChannelCfg.uiPh = pMSSetChPhCfg->uiCH1_Phase;
		else if(uiCnt == 2)
			msChannelCfg.uiPh = pMSSetChPhCfg->uiCH2_Phase;
		else
			msChannelCfg.uiPh = pMSSetChPhCfg->uiCH3_Phase;

		if(pwm_mstepConfig((1<<uiSetCnt), &msChannelCfg) != E_OK) {
			printf("Config Micro step channel %d fail\r\n", uiSetCnt);
			return E_SYS;
		}

		uiCnt++;
	}

	return E_OK;
}

/**
    Disable micro step by set group

    Disable(stop) micro step by set if it is running in free run mode.

    @param[in] uiMSSet  pwm set id, set0:pwm0-3, set1:pwm4-7, set2:pwm8-11, set3:pwm12-15
    @return Stop status
        - @b E_OK: Success
*/
int pwm_mstepDisableSet(PWM_MS_CHANNEL_SET uiMSSet)
{
	u32 uiSetCnt;
	u32 uiMSBit = 0;
	union PWM_MS_STOP pwm_ms_stop = {0};

	for(uiSetCnt = (uiMSSet * 4); uiSetCnt < ((uiMSSet + 1) * 4); uiSetCnt++)
		uiMSBit |= (1<<uiSetCnt);

	pwm_ms_stop.reg = uiMSBit;

	if (PWM_GETREG(PWM_MS_START_OFS) & uiMSBit)
		PWM_SETREG(PWM_MS_STOP_OFS, pwm_ms_stop.reg);
	else {
		printf("pwm_mstepDisableSet(), do not disable the channel not enable yet!! (%x)\r\n", uiMSSet);
		return E_PAR;
	}

	return pwm_wait((1<<(uiMSSet * 4)), PWM_TYPE_MICROSTEP);
}

/**
    Set cycle target count number parameter.

    PWM controller can count down target step number

    @param[in] uiTargetCntSet       Micro step target count set
    @param[in] uiTargetCnt          Micro step target count number
    @param[in] bEnableTargetCnt     Enable target count down action or not

    @note for PWM_TGT_COUNT
    @return Set cycle count parameter status
        - @b E_OK: Success
        - @b E_PAR: Invalid PWM ID or not opened yet
*/
int pwm_mstepConfigTargetCountEnable(PWM_TGT_COUNT uiTargetCntSet, u32 uiTargetCnt, BOOL bEnableTargetCnt)
{
	union PWM_TGT_CNT_REG0 uTgtCntReg0 = {0};
	union PWM_TGT_CNT_REG1 uTgtCntReg1 = {0};
	union PWM_TGT_CNT_ENABLE uTgtCntEnReg = {0};
	union PWM_TGT_CNT_DISABLE uTgtCntDisReg ={0};
	union PWM_CLKDIV_LOAD_INTEN uTgtCntIntEnReg = {0};

	if (!uiTargetCnt) {
		printf("Target count should > 0\r\n");
		return E_PAR;
	}

	uTgtCntEnReg.reg = PWM_GETREG(PWM_TGT_CNT_ENABLE_OFS);

	if(bEnableTargetCnt) {
		if((uTgtCntEnReg.reg & (1 << uiTargetCntSet)) == (u32)(1 << uiTargetCntSet)) {
			printf("Target count set [%d] already enabled\r\n", uiTargetCntSet);
			return E_PAR;
		}
	} else {
		if((uTgtCntEnReg.reg & (1 << uiTargetCntSet)) != (u32)(1 << uiTargetCntSet)) {
			printf("Target count set [%d] already disabled\r\n", uiTargetCntSet);
			return E_PAR;
		} else {
			uTgtCntDisReg.reg |= (1<<uiTargetCntSet);
			PWM_SETREG(PWM_TGT_CNT_DISABLE_OFS, uTgtCntDisReg.reg);
			return E_OK;
		}
	}

	uTgtCntReg0.reg = PWM_GETREG(PWM_TGT_CNT_REG0_OFS);
	uTgtCntReg1.reg = PWM_GETREG(PWM_TGT_CNT_REG1_OFS);

	switch(uiTargetCntSet) {
	case PWM_00_03_TGT_CNT:
		uTgtCntReg0.bit.pwm00_03_target_cnt = uiTargetCnt;
		PWM_SETREG(PWM_TGT_CNT_REG0_OFS, uTgtCntReg0.reg);
		uTgtCntEnReg.bit.pwm00_03_target_cnt_en = 1;
		uTgtCntIntEnReg.bit.clkdiv_00_03_tgt_cnt_inten = 1;
		break;

	case PWM_04_07_TGT_CNT:
		uTgtCntReg0.bit.pwm04_07_target_cnt = uiTargetCnt;
		PWM_SETREG(PWM_TGT_CNT_REG0_OFS, uTgtCntReg0.reg);
		uTgtCntEnReg.bit.pwm04_07_target_cnt_en = 1;
		uTgtCntIntEnReg.bit.clkdiv_04_07_tgt_cnt_inten = 1;
		break;

	case PWM_08_11_TGT_CNT:
		uTgtCntReg1.bit.pwm08_11_target_cnt = uiTargetCnt;
		PWM_SETREG(PWM_TGT_CNT_REG1_OFS, uTgtCntReg1.reg);
		uTgtCntEnReg.bit.pwm08_11_target_cnt_en = 1;
		uTgtCntIntEnReg.bit.clkdiv_08_11_tgt_cnt_inten = 1;
		break;

	default:
		return E_SYS;
		break;
	}

	PWM_SETREG(PWM_CLKDIV_LOAD_INTEN_OFS, uTgtCntIntEnReg.reg);
	PWM_SETREG(PWM_TGT_CNT_ENABLE_OFS, uTgtCntEnReg.reg);

	while(!(PWM_GETREG(PWM_TGT_CNT_ENABLE_OFS) & (1<<uiTargetCntSet)));

	return E_OK;
}

static void drv_pwm_setup_start(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	PWM_CFG pwm_info = {0};
	PWM_CLOCK_DIV clock_div_section;

	if (duty_cycle > 100) {
		printf("Duty cycle should not larger than 100\n");
		return;
	}

	if (pwm_id < PWMID_4)
		clock_div_section = PWM0_3_CLKDIV;
	else if (pwm_id < PWMID_8)
		clock_div_section = PWM4_7_CLKDIV;
	else if (pwm_id < PWMID_12)
		clock_div_section = PWM8_11_CLKDIV;
	else if (pwm_id == PWMID_12)
		clock_div_section = PWM12_CLKDIV;
	else if (pwm_id == PWMID_13)
		clock_div_section = PWM13_CLKDIV;
	else if (pwm_id == PWMID_14)
		clock_div_section = PWM14_CLKDIV;
	else if (pwm_id == PWMID_15)
		clock_div_section = PWM15_CLKDIV;
	else if (pwm_id == PWMID_16)
		clock_div_section = PWM16_CLKDIV;
	else if (pwm_id == PWMID_17)
		clock_div_section = PWM17_CLKDIV;
	else if (pwm_id == PWMID_18)
		clock_div_section = PWM18_CLKDIV;
	else
		clock_div_section = PWM19_CLKDIV;

	pwm_open(pwm_id);

	if (pwm_id < PWMID_8) {
		pwm_pwmConfigClockDiv(clock_div_section, 0x2EDF);
		pwm_info.uiPrd = 0x2710;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (0x2710*duty_cycle) / 100;
	} else {
		pwm_pwmConfigClockDiv(clock_div_section, 0x3FFF);
		pwm_info.uiPrd = 255;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (255*duty_cycle) / 100;
	}
	pwm_info.uiInv = inv;

	pwm_pwmConfig(pwm_id, &pwm_info);

	pwm_pwmEnable(pwm_id);
}

static void drv_pwm_reload(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	PWM_CFG pwm_info = {0};

	if (duty_cycle > 100) {
		printf("Duty cycle should not larger than 100\n");
		return;
	}

	if (pwm_id < PWMID_8) {
		pwm_info.uiPrd = 0x2710;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (0x2710*duty_cycle) / 100;
	} else {
		pwm_info.uiPrd = 255;
		pwm_info.uiRise = 0;
		pwm_info.uiFall = (255*duty_cycle) / 100;
	}
	pwm_info.uiInv = inv;

	pwm_pwmConfig(pwm_id, &pwm_info);

	pwm_pwmReload(pwm_id);
}


void pwm_setup_start(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	u32 i = 0;

	for (i = 0; i < PWMID_NO_TOTAL_CNT; i++) {
		if ((pwm_id >> i) & 0x1)
			drv_pwm_setup_start((0x1 << i), duty_cycle, inv);
	}
}

void pwm_reload(u32 pwm_id, u32 duty_cycle, u32 inv)
{
	u32 i = 0;

	for (i = 0; i < PWMID_NO_TOTAL_CNT; i++) {
		if ((pwm_id >> i) & 0x1)
			drv_pwm_reload((0x1 << i), duty_cycle, inv);
	}
}

int pwm_close(u32 uiPWMId, BOOL bWaitAutoDisableDone)
{
	u32 i = 0, value;

	for (i = 0; i < PWMID_NO_TOTAL_CNT; i++) {
		if ((uiPWMId >> i) & 0x1) {
			value = drv_pwm_close((0x1 << i), bWaitAutoDisableDone);
			if (value)
				printf("PWM%d close fail!\n", i);
		}
	}
	return 0;
}
