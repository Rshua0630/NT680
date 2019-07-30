/**
    @file       pwm.h
    @ingroup    mIDrvIO_PWM

    @brief      Header file for PWM module
                This file is the header file that define the API for PWM module

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

*/
#ifndef _PWM_H
#define _PWM_H

/**
    @addtogroup mIDrvIO_PWM
*/
//@{


/**
    PWM type (PWM or Micro step)
*/
typedef enum
{
	PWM_TYPE_PWM = 0x0,             ///< PWM
	PWM_TYPE_MICROSTEP,             ///< Micro step
//	ENUM_DUMMY4WORD(PWM_TYPE)
} PWM_TYPE;

/**
    PWM micro step direction

    Micro step direction configuration

    @note for pwm_mstepConfig() pwm_mstepConfigSet()
*/
typedef enum
{
	MS_DIR_INCREASE = 0x0,          ///< CounterClockwise
	MS_DIR_DECREASE,                ///< Clockwise
	MS_DIR_CNT,
//	ENUM_DUMMY4WORD(PWM_MS_DIR)
} PWM_MS_DIR;

/**
    PWM micro step phase type (1-2 or 2-2 phase)

    @note for pwm_mstepConfig() pwm_mstepConfigSet()
*/
typedef enum
{
	PWM_MS_1_2_PHASE_TYPE = 0x0,    ///< 1 unit each operation
	PWM_MS_2_2_PHASE_TYPE,          ///< 2 unit each operation
	PWM_MS_PHASE_TYPE_CNT,
//	ENUM_DUMMY4WORD(PWM_MS_PHASE_TYPE)
} PWM_MS_PHASE_TYPE;

/**
    PWM Micro step step per phase

    PWM Micro step step per phase configuration

    @note for pwm_mstepConfig() pwm_mstepConfigSet()
*/
typedef enum
{
	TOTAL_08_STEP_PER_PHASE = 8,    ///< 8  steps each phase
	TOTAL_16_STEP_PER_PHASE = 16,   ///< 16 steps each phase
	TOTAL_32_STEP_PER_PHASE = 32,   ///< 32 steps each phase
	TOTAL_64_STEP_PER_PHASE = 64,   ///< 64 steps each phase
//	ENUM_DUMMY4WORD(PWM_MS_STEP_PER_PHASE)
} PWM_MS_STEP_PER_PHASE;

/**
    PWM Micro step channel level

    PWM Micro step channel level configuation
*/
typedef enum
{
	PWM_MS_CHANNEL_LEVEL_LOW = 0x0, ///< PWM MS channel level low
	PWM_MS_CHANNEL_LEVEL_HIGH,      ///< PWM MS channel level high

//	ENUM_DUMMY4WORD(PWM_MS_CHANNEL_LEVEL)
} PWM_MS_CHANNEL_LEVEL;

/**
    PWM Micro step channel set

    PWM Micro step channel set definition

    @note for pwm_openSet, pwm_closeSet, pwm_waitSet(), pwm_configClockDivSet(), pwm_mstepConfigSet(),\n
              pwm_pwmEnableSet(), pwm_pwmDisableSet(), pwm_mstepEnableSet(), pwm_mstepDisableSet()
*/
typedef enum
{
	PWM_MS_SET_0 = 0x0,             ///< PWM MS channel set 0, including PWM0-3
	PWM_MS_SET_1,                   ///< PWM MS channel set 1, including PWM4-7
	PWM_MS_SET_2,                   ///< PWM MS channel set 2, including PWM8-11
	PWM_MS_SET_3,                   ///< PWM MS channel set 3, including PWM12-15
	PWM_MS_SET_TOTAL,

//	ENUM_DUMMY4WORD(PWM_MS_CHANNEL_SET)
} PWM_MS_CHANNEL_SET;

/**
    Micro step target count set(s)

    @note for pwm_mstepConfigTargetCountEnable()
*/
typedef enum
{
	PWM_00_03_TGT_CNT   = 0x0,          ///< PWM0~PWM3 target count
	PWM_04_07_TGT_CNT,                  ///< PWM4~PWM7 target count
	PWM_08_11_TGT_CNT,                  ///< PWM8~PWM11 target count

	PWM_TGT_CNT_NUM,

//	ENUM_DUMMY4WORD(PWM_TGT_COUNT)
} PWM_TGT_COUNT;


/**
    PWM clock divid

    @note for pwm_pwmConfigClockDiv()
*/
typedef enum
{
	PWM0_3_CLKDIV   = 0x0,          ///< PWM0~PWM3 clock divid
	PWM4_7_CLKDIV,                  ///< PWM4~PWM7 clock divid
	PWM8_11_CLKDIV,                 ///< PWM8~PWM11 clock divid
	PWM12_CLKDIV    = 12,           ///< PWM12 clock divid
	PWM13_CLKDIV,                   ///< PWM13 clock divid
	PWM14_CLKDIV,                   ///< PWM14 clock divid
	PWM15_CLKDIV,                   ///< PWM15 clock divid
	PWM16_CLKDIV,                   ///< PWM16 clock divid
	PWM17_CLKDIV,                   ///< PWM17 clock divid
	PWM18_CLKDIV,                   ///< PWM18 clock divid
	PWM19_CLKDIV,                   ///< PWM19 clock divid
//	ENUM_DUMMY4WORD(PWM_CLOCK_DIV)
} PWM_CLOCK_DIV;

/**
    PWM status selection
*/
typedef enum
{
	PWM_MS_STS,		///< PWM micro stepping status
	PWM_STS,		///< PWM status
	PWM_CLKLOAD_STS,	///< PWM clock load status
	PWM_TGTCNT_STS,		///< PWM target count status
//	ENUM_DUMMY4WORD(PWM_STS_SELECTION)
} PWM_STS_SELECTION;


/**
    PWM configuration identifier

    @note For pwm_setConfig()
*/
typedef enum
{
	PWM_CONFIG_ID_AUTOPINMUX,                   ///< Context can be one of the following:
						///< - @b TRUE  : disable pinmux when pwm driver close
						///< - @b FALSE : not disable pinmux when pwm driver close

//	ENUM_DUMMY4WORD(PWM_CONFIG_ID)
} PWM_CONFIG_ID;


/**
    PWM configuration structure

    @note for pwm_set()
*/
typedef struct
{
	u32  uiPrd;                              ///< Base period, how many PWM clock per period, 2 ~ 255
						///< @note uiRise <= uiFall <= uiPrd
	u32  uiRise;                             ///< Rising at which clock
						///< @note uiRise <= uiFall <= uiPrd
	u32  uiFall;                             ///< Falling at which clock
						///< @note uiRise <= uiFall <= uiPrd
	u32  uiOnCycle;                          ///< Output cycle, 0 ~ 65535
						///< - @b PWM_FREE_RUN: Free Run
						///< - @b Others: How many cycles (PWM will stop after output the cycle counts)
	u32  uiInv;                              ///< Invert PWM output signal or not
						///< - @b PWM_SIGNAL_NORMAL: Don't invert PWM output signal
						///< - @b PWM_SIGNAL_INVERT: Invert PWM output signal
} PWM_CFG, *PPWM_CFG;

/**
    MSTP configuration structure

    @note for pwm_mstepConfig()
*/
typedef struct
{
	u32  uiPh;                               ///< Micro step starting phase, phase 0 ~ 7
	u32  uiDir;                              ///< Micro step moving direction ,
						///< - @b MS_DIR_INCREASE
						///< - @b MS_DIR_DECREASE
	u32  uiOnCycle;                          ///< Number of phase for Micro step mode
	u32  uiStepPerPhase;                     ///< Step per phase
						///< - @b TOTAL_08_STEP_PER_PHASE (8 steps each phase)
						///< - @b TOTAL_16_STEP_PER_PHASE (16 steps each phase)
						///< - @b TOTAL_32_STEP_PER_PHASE (32 steps each phase)
						///< - @b TOTAL_64_STEP_PER_PHASE (64 steps each phase)
	u32  uiPhaseType;                        ///< 1-2 or 2-2 phase
						///< - @b PWM_MS_1_2_PHASE_TYPE (1 unit each operation)
						///< - @b PWM_MS_2_2_PHASE_TYPE (2 unit each operation)
	BOOL    uiThresholdEn;                      ///< Enable threshold filter(always set as FALSE), TRUE for test only
	BOOL    uiThreshold;                        ///< Max 0x63
} MSTP_CFG, *PMSTP_CFG;

/**
    Micro step channels (within a set) phase configuration

    @note for pwm_mstepConfigSet()
*/
typedef struct
{
	u32  uiCH0_Phase;                       ///< Specific MS set channel 0 start phase
	u32  uiCH1_Phase;                       ///< Specific MS set channel 1 start phase
	u32  uiCH2_Phase;                       ///< Specific MS set channel 2 start phase
	u32  uiCH3_Phase;                       ///< Specific MS set channel 3 start phase

} MS_CH_PHASE_CFG, *PMS_CH_PHASE_CFG;

/**
    Micro step channels (within a set) level configuration

    @note for PWM_MS_CHANNEL_LEVEL_LOW or PWM_MS_CHANNEL_LEVEL_HIGH,
*/
typedef struct
{
	u32  uiCH0_Level;                       ///< Specific MS channel 0 level
	u32  uiCH1_Level;                       ///< Specific MS channel 1 level
	u32  uiCH2_Level;                       ///< Specific MS channel 2 level
	u32  uiCH3_Level;                       ///< Specific MS channel 3 level
} MS_CH_LEVEL_CFG, *PMS_CH_LEVEL_CFG;

/**
    MSTP configuration structure

    @note for pwm_mstepConfigSet()
*/
typedef struct
{
	u32  uiDir;                              ///< Micro step moving direction ,
						///< - @b MS_DIR_INCREASE
						///< - @b MS_DIR_DECREASE
	u32  uiOnCycle;                          ///< Number of phase for Micro step mode
	u32  uiStepPerPhase;                     ///< Step per phase
						///< - @b TOTAL_08_STEP_PER_PHASE (8 steps each phase)
						///< - @b TOTAL_16_STEP_PER_PHASE (16 steps each phase)
						///< - @b TOTAL_32_STEP_PER_PHASE (32 steps each phase)
						///< - @b TOTAL_64_STEP_PER_PHASE (64 steps each phase)
	u32  uiPhaseType;                        ///< 1-2 or 2-2 phase
						///< - @b PWM_MS_1_2_PHASE_TYPE (1 unit each operation)
						///< - @b PWM_MS_2_2_PHASE_TYPE (2 unit each operation)
	BOOL    uiThresholdEn;                      ///< Enable threshold filter
	BOOL    uiThreshold;                        ///< Max 0x63
} MSCOMMON_CFG, *PMSCOMMON_CFG;

/**
    @name PWM ID

    PWM ID for PWM driver API

    @note for pwm_open(), pwm_set(), pwm_setCCNT(), pwm_setCCNTToutEN(), pwm_wait(), pwm_stop(), pwm_en(), pwm_reload(),\n
              pwm_ms_set(), pwm_ms_stop(), pwm_ms_en(), pwm_getCycleNumber(), pwm_ccntGetCurrentVal(), pwm_ccntGetCurrentVal(),\n
              pwm_ccntEnable()
*/
//@{
#define PWMID_0		0x00000001  ///< PWM ID 0
#define PWMID_1		0x00000002  ///< PWM ID 1
#define PWMID_2		0x00000004  ///< PWM ID 2
#define PWMID_3		0x00000008  ///< PWM ID 3
#define PWMID_4		0x00000010  ///< PWM ID 4
#define PWMID_5		0x00000020  ///< PWM ID 5
#define PWMID_6		0x00000040  ///< PWM ID 6
#define PWMID_7		0x00000080  ///< PWM ID 7
#define PWMID_8		0x00000100  ///< PWM ID 8
#define PWMID_9		0x00000200  ///< PWM ID 9
#define PWMID_10	0x00000400  ///< PWM ID 10
#define PWMID_11	0x00000800  ///< PWM ID 11
#define PWMID_12	0x00001000  ///< PWM ID 12
#define PWMID_13	0x00002000  ///< PWM ID 13
#define PWMID_14	0x00004000  ///< PWM ID 14
#define PWMID_15	0x00008000  ///< PWM ID 15
#define PWMID_16	0x00010000  ///< PWM ID 16
#define PWMID_17	0x00020000  ///< PWM ID 17
#define PWMID_18	0x00040000  ///< PWM ID 18
#define PWMID_19	0x00080000  ///< PWM ID 19

//@}

// PWM Driver API
extern int	pwm_open(u32 uiPWMId);
extern int	pwm_openSet(PWM_MS_CHANNEL_SET uiMSSet);
extern u32	pwm_pwmGetCycleNumber(u32 uiPWMId);
extern int	pwm_close(u32 uiPWMId, BOOL bWaitAutoDisableDone);
extern int	pwm_closeSet(PWM_MS_CHANNEL_SET uiMSSet, BOOL bWaitAutoDisableDone);
extern int	pwm_wait(u32 uiPWMId, PWM_TYPE uiPwmType);
extern int	pwm_waitSet(PWM_MS_CHANNEL_SET uiMSSet);

extern int	pwm_pwmReload(u32 uiPWMId);
extern int	pwm_pwmEnable(u32 uiPWMId);
extern int	pwm_pwmDisable(u32 uiPWMId);
extern int	pwm_pwmEnableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern int	pwm_pwmDisableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern int	pwm_pwmConfig(u32 uiPWMId, PPWM_CFG pPWMCfg);
extern int	pwm_pwmConfigClockDiv(PWM_CLOCK_DIV uiClkSrc, u32 uiDiv);
extern int	pwm_pwmReloadConfig(u32 uiPWMId, u32 iRise, u32 iFall, u32 iBasePeriod);

extern int	pwm_mstepEnable(u32 uiPWMId);
extern int	pwm_mstepDisable(u32 uiPWMId, BOOL bWait);
extern int	pwm_mstepEnableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern int	pwm_mstepDisableSet(PWM_MS_CHANNEL_SET uiMSSet);
extern int	pwm_mstepClockDivReload(PWM_CLOCK_DIV uiPWMClkDiv, BOOL  bWaitDone);
extern int	pwm_mstepConfig(u32 uiPWMId, PMSTP_CFG pMSTPCfg);
extern int	pwm_mstepTargetCountWaitDone(PWM_TGT_COUNT uiTargetCnt);
extern int	pwm_mstepClkDivWaitLoadDone(PWM_CLOCK_DIV uiPWMClkDivSrc);
extern int	pwm_mstepConfigClockDiv(PWM_MS_CHANNEL_SET uiMSSet, u32 uiDiv);
extern int	pwm_mstepConfigTargetCountEnable(PWM_TGT_COUNT uiTargetCntSet, u32 uiTargetCnt, BOOL bEnableTargetCnt);
extern int	pwm_mstepConfigSet(PWM_MS_CHANNEL_SET uiMSSet, PMS_CH_PHASE_CFG pMSSetChPhCfg, PMSCOMMON_CFG pMSCommonCfg);
extern void	pwm_setup_start(u32 pwm_id, u32 duty_cycle, u32 signal_inv);
extern void	pwm_reload(u32 pwm_id, u32 duty_cycle, u32 signal_inv);

//@}

#endif
