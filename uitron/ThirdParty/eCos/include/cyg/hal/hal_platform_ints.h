#ifndef CYGONCE_HAL_PLATFORM_INTS_H
#define CYGONCE_HAL_PLATFORM_INTS_H

#include <pkgconf/hal_arm_nvt.h>

//#define CYGNUM_HAL_INTERRUPT_GLOBALTMR          27  /* SCU Global Timer interrupt */
#define CYGNUM_HAL_INTERRUPT_SCU_CPUTMR             27  /* CA53 Virtual timer */
//#define CYGNUM_HAL_INTERRUPT_SCU_WDTTMR         30  /* SCU Private WDT interrupt */

typedef enum _EN_KER_IRQ_ID {
/*
    EN_KER_IRQ_ID_AUD_OUT		= 32,	// IRQ D[32]: Audio Output interrupt
    EN_KER_IRQ_ID_CPE			= 33,	// IRQ D[33]: CPE interrupt
    EN_KER_IRQ_ID_AUD_IN		= 34,	// IRQ D[34]: Audio Input interrupt
    EN_KER_IRQ_ID_DEMUX0		= 35,	// IRQ D[35]: MPEG Demux 0 interrupt
    EN_KER_IRQ_ID_TV_ENC		= 36,	// IRQ D[36]: TV interrupt
    EN_KER_IRQ_ID_VA			= 37,	// IRQ D[37]: IMVQ Decoder engine interrupt
    EN_KER_IRQ_ID_LEON			= 38,	// IRQ D[38]: LEON interrupt
    EN_KER_IRQ_ID_DEMUX1		= 39,	// IRQ D[39]: MPEG Demux 1 interrupt
    EN_KER_IRQ_ID_CBUS			= 40,	// IRQ D[40]: CBUS interrupt
    EN_KER_IRQ_ID_IP0			= 41,	// IRQ D[41]: IP0 related interrupt
    EN_KER_IRQ_ID_TCON_LED		= 42,	// IRQ D[42]: TCON Led intterupt
    EN_KER_IRQ_ID_AUD_HDMI_0	= 43,	// IRQ D[43]: HDMI Audio axi#0 interrupt
    EN_KER_IRQ_ID_AUD_HDMI_1	= 44,	// IRQ D[44]: HDMI Audio axi#1 interrupt
    EN_KER_IRQ_ID_MPEG2IP		= 45,	// IRQ D[45]: DP_int_t interrupt
    EN_KER_IRQ_ID_AGE			= 47,	// IRQ D[47]: AGE interrupt
    EN_KER_IRQ_ID_STBC_RTC		= 48,	// IRQ D[48]: STBC RTC interrupt
    EN_KER_IRQ_ID_VSGEN			= 49,	// IRQ D[49]: VSGEN interrupt
    EN_KER_IRQ_ID_VD			= 50,	// IRQ D[50]: VIDEO decoder interrupt
    EN_KER_IRQ_ID_CRYPTOGRAPH2	= 51,	// IRQ D[51]: Cryptographic services interrupt
    EN_KER_IRQ_ID_CRYPTOGRAPH	= 52,	// IRQ D[52]: HDCP Cryptographic services interrupt
    EN_KER_IRQ_ID_SUB_DP		= 53,	// IRQ D[53]: DP_int_t interrupt
    EN_KER_IRQ_ID_LBM			= 54,	// IRQ D[54]: DP_int_t interrupt
    EN_KER_IRQ_ID_DP			= 55,	// IRQ D[55]: DP_int_t interrupt
    EN_KER_IRQ_ID_TCON_LED_HV	= 57,	// IRQ D[57]: TCON LED
    EN_KER_IRQ_ID_CPU1_1		= 58,	// IRQ D[58]: CPU 1 general interrupt 1
    EN_KER_IRQ_ID_CPU1_0		= 59,	// IRQ D[59]: CPU 1 general interrupt 0
    EN_KER_IRQ_ID_IP1			= 60,	// IRQ D[60]: IP1 related interrupt
    EN_KER_IRQ_ID_UART1			= 61,	// IRQ D[61]: UART 1 interrupt
    EN_KER_IRQ_ID_TCON_IR		= 62,	// IRQ D[62]: TCON IR
    EN_KER_IRQ_ID_MAC0			= 74,	// IRQ D[74]: MAC 0  interrupt
    EN_KER_IRQ_ID_TIMER0		= 75,	// IRQ D[75]: Timer1 interrupt
    EN_KER_IRQ_ID_TIMER1		= 76,	// IRQ D[76]: Timer1 interrupt
    EN_KER_IRQ_ID_UART2			= 77,	// IRQ D[77]: UART 2 interrupt
    EN_KER_IRQ_ID_TIMER2		= 78,	// IRQ D[78]: Timer2 interrupt
    EN_KER_IRQ_ID_TIMER3		= 79,	// IRQ D[79]: Timer1 interrupt
    EN_KER_IRQ_ID_DP_OSD		= 80,	// IRQ D[80]: DP_OSD_ISR
    EN_KER_IRQ_ID_STBC			= 82,	// IRQ D[82]: STBC interrupt
    EN_KER_IRQ_ID_UART0			= 83,	// IRQ D[83]: UART interrupt
    EN_KER_IRQ_ID_GPIO			= 84,	// IRQ D[84]: GPIO interrupt
    EN_KER_IRQ_ID_USB_2			= 87,	// IRQ D[87]: USB 2.0 host/device controller interrupt 2
    EN_KER_IRQ_ID_USB_3			= 88,	// IRQ D[88]: USB 2.0 host/device controller interrupt 3
    EN_KER_IRQ_ID_ATF			= 89,	// IRQ D[89]: ATF interrupt
    EN_KER_IRQ_ID_HDMI_A		= 90,	// IRQ D[90]: HDMI interrupt A
    EN_KER_IRQ_ID_HDMI_B		= 91,	// IRQ D[91]: HDMI interrupt B
    EN_KER_IRQ_ID_HDMI_C		= 92,	// IRQ D[92]: HDMI interrupt C
    EN_KER_IRQ_ID_AHBDMA		= 94,	// IRQ D[94]: AXI TO AHB DMA
    EN_KER_IRQ_ID_SDMMC			= 95,	// IRQ D[95]: SD/MMC/NAND controller interrupt
    EN_KER_IRQ_ID_UART3			= 96,	// IRQ D[96]: USRT 3
    EN_KER_IRQ_ID_MIPS_I2C		= 99,	// IRQ D[99]: I2C Master interrupt
    EN_KER_IRQ_ID_FRAMEWX		= 100,	// IRQ D[100]: FRAMEWX Master interrupt
    EN_KER_IRQ_ID_SPI			= 101,	// IRQ D[101]: SPI interrupt
    EN_KER_IRQ_ID_CI			= 102,	// IRQ D[102]: CI interrupt
    EN_KER_IRQ_ID_STBC_IR		= 104,	// IRQ D[104]: STBC IR interrupt
    EN_KER_IRQ_ID_STBC_CEC		= 105,	// IRQ D[105]: CEC interrupt
    EN_KER_IRQ_ID_STBC_GPIO		= 106,	// IRQ D[106]: STBC GPIO interrupt
    EN_KER_IRQ_ID_STBC_ADC		= 107,	// IRQ D[107]: ADC controller interrupt
    EN_KER_IRQ_ID_USB_0			= 108,	// IRQ D[108]: USB 2.0 host/device controller interrupt 0
    EN_KER_IRQ_ID_STBC_I2C		= 109,	// IRQ D[109]: I2C slave interrupt
    EN_KER_IRQ_ID_AXI_DMA		= 110,	// IRQ D[110]: AXI DMA interrupt
    EN_KER_IRQ_ID_TT			= 111,	// IRQ D[111]: Teletext interrupt
    EN_KER_IRQ_ID_AXI_SLAVE0	= 112,	// IRQ D[112]: AXI slave interrupt0
    EN_KER_IRQ_ID_SCUEVABORT	= 115,	// IRQ D[115]: ARM scuevabort interrupt
    EN_KER_IRQ_ID_WDRESETREQ0	= 116,	// IRQ D[116]: ARM wdresetreq interrupt 0
    EN_KER_IRQ_ID_COMMRX0		= 120,	// IRQ D[120]: ARM COMMRX interrupt 0
    EN_KER_IRQ_ID_COMMTX0		= 124,	// IRQ D[124]: ARM COMMTX interrupt 0
    EN_KER_IRQ_ID_L2_ERR		= 128,	// IRQ D[128]: L2 cache error interrupt
    EN_KER_IRQ_ID_LZ77			= 129,	// IRQ D[129]: LZ77 interrupt
    EN_KER_IRQ_ID_SECURE_TIMER	= 130,	// IRQ D[130]: Secure timer interrupt
    EN_KER_IRQ_ID_RTC_TIMER		= 131,	// IRQ D[131]: RTC timer interrupt
    EN_KER_IRQ_ID_TZASC_MAU0	= 132,	// IRQ D[132]: TZASC MAU0 interrupt
    EN_KER_IRQ_ID_TZASC_ACP		= 134,	// IRQ D[134]: TZASC ACP interrupt
    EN_KER_IRQ_ID_TS_MUX		= 135,	// IRQ D[135]: TX MUXER interrupt
    EN_KER_IRQ_ID_FRE			= 136,	// IRQ D[136]: FRE interrupt
    EN_KER_IRQ_ID_PMU0			= 137,	// IRQ D[137]: Performance unit interrupt 0
    EN_KER_IRQ_ID_ADMA			= 141,	// IRQ D[141]: ADMA interrupt
    EN_KER_IRQ_ID_TCON_VP		= 146,	// IRQ D[146]: TCON VP interrupt
    EN_KER_IRQ_ID_PSBM			= 147,	// IRQ D[147]: PSBM interrupt
    EN_KER_IRQ_ID_TSIF_CI14		= 148,	// IRQ D[148]: tsif_ci14 interrupt
    EN_KER_IRQ_ID_DISP_AGE		= 149,	// IRQ D[149]: Display AGE interrupt
    EN_KER_IRQ_ID_SCE			= 151,	// IRQ D[151]: SCE interrupt
    EN_KER_IRQ_ID_DEMOD_T2		= 152,	// IRQ D[152]: DEMOD T2 interrupt
    EN_KER_IRQ_ID_JPEG_ENC		= 153,	// IRQ D[153]: JPEG Encoder interrupt
    EN_KER_IRQ_ID_SEC_NFC		= 166,	// IRQ D[166]: Secure NFC interrupt

    EN_KER_IRQ_ID_MAX
*/
    ECOS_INT_ID_TIMER = 32,
	ECOS_INT_ID_SIE,
	ECOS_INT_ID_SIE2,
	ECOS_INT_ID_SIE3,

	ECOS_INT_ID_SIE4,
	ECOS_INT_ID_IPE,
	ECOS_INT_ID_IME,
	ECOS_INT_ID_DCE,

	ECOS_INT_ID_IFE,
	ECOS_INT_ID_IFE2,
	ECOS_INT_ID_DIS,
	ECOS_INT_ID_FDE,

	ECOS_INT_ID_ETH_LPI,         //Ethernet_LPI
	ECOS_INT_ID_RHE,
	ECOS_INT_ID_DRE,
	ECOS_INT_ID_DAI,             //15

	ECOS_INT_ID_H26X,
	ECOS_INT_ID_JPEG,
	ECOS_INT_ID_GRAPHIC,
	ECOS_INT_ID_GRAPHIC2,

	ECOS_INT_ID_AFFINE,
	ECOS_INT_ID_ISE,
	ECOS_INT_ID_TGE,
	ECOS_INT_ID_TSMUX,

	ECOS_INT_ID_GPIO,
	ECOS_INT_ID_REMOTE,
	ECOS_INT_ID_PWM,
	ECOS_INT_ID_USB,

	ECOS_INT_ID_USB3,
	ECOS_INT_ID_NAND,
	ECOS_INT_ID_SDIO,
	ECOS_INT_ID_SDIO2,           //31

	ECOS_INT_ID_SDIO3,
	ECOS_INT_ID_DMA,
	ECOS_INT_ID_ETHERNET,
	ECOS_INT_ID_SPI,

	ECOS_INT_ID_SPI2,
	ECOS_INT_ID_SPI3,
	ECOS_INT_ID_SPI4,
	ECOS_INT_ID_ETH_REV_MII,     //Ethernet_RevMII

	ECOS_INT_ID_SIF,
	ECOS_INT_ID_I2C,
	ECOS_INT_ID_I2C2,
	ECOS_INT_ID_UART,            //43

	ECOS_INT_ID_UART2,
	ECOS_INT_ID_UART3,
	ECOS_INT_ID_UART4,
	ECOS_INT_ID_ADC,

	ECOS_INT_ID_IDE,
	ECOS_INT_ID_IDE2,
	ECOS_INT_ID_DSI,
	ECOS_INT_ID_MI,              //51

	ECOS_INT_ID_HDMI,
	ECOS_INT_ID_DMA2,            //New @ NT96680
	ECOS_INT_ID_LVDS,
	ECOS_INT_ID_LVDS2,           //55

	ECOS_INT_ID_RTC,
	ECOS_INT_ID_WDT,
	ECOS_INT_ID_CG,
	ECOS_INT_ID_CC,              //59

	ECOS_INT_ID_I2C3,            //60
//  New add @ NT96680
	ECOS_INT_ID_SLVS_EC,
	ECOS_INT_ID_LVDS3,           //share with HiSPi3/CSI3
	ECOS_INT_ID_LVDS4,           //share with HiSPi4/CSI4

	ECOS_INT_ID_LVDS5,           //share with HiSPi5/CSI5
	ECOS_INT_ID_LVDS6,           //share with HiSPi6/CSI6
	ECOS_INT_ID_LVDS7,           //share with HiSPi7/CSI7
	ECOS_INT_ID_LVDS8,           //share with HiSPi8/CSI8

	ECOS_INT_ID_SIE5,
	ECOS_INT_ID_SIE6,
	ECOS_INT_ID_SIE7,
	ECOS_INT_ID_SIE8,

	ECOS_INT_ID_IVE,
	ECOS_INT_ID_SVM,
	ECOS_INT_ID_SDE,
	ECOS_INT_ID_CNN,             //75

	ECOS_INT_ID_DSP,
	ECOS_INT_ID_DSP2,
	ECOS_INT_ID_CANBUS,
	ECOS_INT_ID_CRYPTO,          //79

	ECOS_INT_ID_COPY,
	ECOS_INT_ID_ROTATE,
	ECOS_INT_ID_I2C4,
	ECOS_INT_ID_I2C5,            //83

	ECOS_INT_ID_DRTC,
	ECOS_INT_ID_ISE2,
	ECOS_INT_ID_TIMER2,
	ECOS_INT_ID_GPIO2,           //87

	ECOS_INT_ID_PWM2,
	ECOS_INT_ID_CC2,             //89
	ECOS_INT_ID_SIF2,

	ECOS_INT_ID_DSP_2,
	ECOS_INT_ID_DSP_2_2,
	ECOS_INT_ID_TSDEMUX,         //93

	ECOS_INT_ID_CNT,
	ECOS_INT_ID_MAX = ECOS_INT_ID_CNT - 32, //94


	ECOS_INT_ID_WFI = 229,
	ECOS_INT_ID_GIC_TOTAL = 256,
} EN_KER_IRQ_ID;


#define CYGNUM_HAL_ISR_MIN                      1
#define CYGNUM_HAL_ISR_MAX                      ECOS_INT_ID_CNT - 1

#define CYGNUM_HAL_ISR_COUNT                    (CYGNUM_HAL_ISR_MAX + 1)

#define CYGNUM_HAL_INTERRUPT_RTC                CYGNUM_HAL_INTERRUPT_SCU_CPUTMR

externC void hal_arm_reset(void);
#define HAL_PLATFORM_RESET()                    hal_arm_reset()
#define HAL_PLATFORM_RESET_ENTRY                0x0

#endif // CYGONCE_HAL_PLATFORM_INTS_H
