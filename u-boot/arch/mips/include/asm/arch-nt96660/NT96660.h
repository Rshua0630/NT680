#ifndef __NT96660_H__
#define __NT96660_H__

typedef volatile u8	NT72568_REG8;
typedef volatile u16	NT72568_REG16;
typedef volatile u32	NT72568_REG32;

enum NT72568_uarts_nr {
	NT72568_UART0,
	NT72568_UART1
};

/*system registers*/
#define SYSTEM_DTV_REGISTERS                      0xCC0D0000
#define SYSTEM_ATV_REGISTERS                      0xCC0C0000

/* bit mapping of system controller0 enable register*/
#define _SYSTEM_UART0_ENABLE			0x00010000
#define _SYSTEM_TIMER1_ENABLE			0x00020000
#define _SYSTEM_TIMER2_ENABLE			0x00040000
#define _SYSTEM_TIMER3_PWM0_ENABLE		0x00080000
#define _SYSTEM_TIMER4_PWM1_ENABLE		0x00100000
#define _SYSTEM_TIMER5_PWM2_ENABLE		0x00200000
#define _SYSTEM_USB_ENABLE		0x00000100

/* bit mapping of system controller1 enable register*/			//	0x30 System Control Register 2                                            
#define _SYSTEM_SPI_ENABLE				0x00000001			    //		bit0:SPI controller enable.                                           
#define _SYSTEM_CI_ENABLE				0x00000002				//		bit1:CI controller enable                                             
#define _SYSTEM_MS_ENABLE				0x00000004				//		bit2 = MS enable                                                      
#define _SYSTEM_SPDIF_ENABLE			0x00000008				//		bit3 = S/PDIF enable                                                  
#define _SYSTEM_UART1_ENABLE			0x00000010				//		bit4 = UART1/SmartCard1 enable                                        
#define _SYSTEM_SPDIF_CLOCK_ENABLE		0x00000020				//		bit5 = Audio-SPDIF local clock enable                                 
#define _SYSTEM_IIS_ENABLE				0x00000040				//		bit6 = IIS enable, it is strongly suggested to reset IIS after enabled
#define _SYSTEM_IIS_CLOCK_ENABLE		0x00000080				//		bit7 = Audio-IIS local clock enable                                   
#define _SYSTEM_TS_IN_CLOCK_ENABLE		0x00000100				//		bit8 = TS input clock enable                                          
#define _SYSTEM_DVII_CLOCK_ENABLE		0x00000200				//		bit9 = DVII input clock enable                                        





#define NT72568_USB_HOST_BASE		0xCC140000
#define NT72568_INTERRUPT_BASE		0xCD0E0000
#define NT72568_UART_BASE		0xCD090000
#define NT72568_TIMER_BASE		0xCD0A0000



#define NT72568_USB_ENABLE_BIT			0x00000100 
#define NT72568_USB_SUSPEND_ENABLE_BIT				0x0080000 //bit 19
#define NT72568_SYSTEM_CONTROLLER_0					0xcc0d0000
#define NT72568_ATV_SYSTEM					0xcc0c0000

#define USB_BASE								0xcc140000
#define setMode 		(1<<4|1<<5|1<<10|1<<8|1<<9|1<<11|1<<12)
#define clearMode 		(1|1<<5|1<<10|1<<8|1<<9)





typedef struct {
        /*0x00*/
        union 
        {
			NT72568_REG32    UTXH;
	    	NT72568_REG32    URXH;
	    	NT72568_REG32    UBRDIVLOW;
        };
        /*0x04 interrupt enable & DLM*/
        union 
        {
            NT72568_REG32    UBRDIVHIGH;
            NT72568_REG32    UIER;
        };
        /*0x08  FCR & IIR*/
		NT72568_REG32	UFCON;
        /*0x0C  LCR */
		NT72568_REG32	ULCON;
        /*0x10 Modem control*/
		NT72568_REG32	UMCON;
        /*0x14 Line status*/
		NT72568_REG32	UTRSTAT;
        /*0x18 Modem status*/
		NT72568_REG32	UMSTAT;
        /*0x1C SCR scratch*/

} __attribute__((__packed__)) NT72568_UART;

typedef struct {
        /*0x00*/
		NT72568_REG32    Control_0;
		/*0x04*/
		NT72568_REG32    Control_1;
		/*0x08*/
		NT72568_REG32    Reset_0;
		/*0x0c*/
		NT72568_REG32    Reset_1;
		/*0x10*/
		NT72568_REG32    SIF_Control;
} __attribute__((__packed__)) NT72568_SYS_DTV;


/* bit mapping for FCR registers */
#define _UART_FCR_FIFO_ENABLE				0x00000001
#define _UART_FCR_RX_FIFO_RESET				0x00000002
#define _UART_FCR_TX_FIFO_RESET				0x00000004
#define _UART_FCR_DMA_MODE					0x00000008
#define _UART_FCR_RX_TRIGGER_LEVEL			0x000000C0

/*uart trigger level*/
#define _UART_TRIGGER_LEVEL1				0
#define _UART_TRIGGER_LEVEL4				1
#define _UART_TRIGGER_LEVEL8				2
#define _UART_TRIGGER_LEVEL14				3

#define _UART_FCR_RX_TRIGGER_SHIFTBIT		6

#define _UART_FCR_RX_TRIGGER_LEVEL1			(0x00 << _UART_FCR_RX_TRIGGER_SHIFTBIT)
#define _UART_FCR_RX_TRIGGER_LEVEL4			(0x01 << _UART_FCR_RX_TRIGGER_SHIFTBIT)
#define _UART_FCR_RX_TRIGGER_LEVEL8			(0x02 << _UART_FCR_RX_TRIGGER_SHIFTBIT)
#define _UART_FCR_RX_TRIGGER_LEVEL14		(0x03 << _UART_FCR_RX_TRIGGER_SHIFTBIT)


static inline NT72568_SYS_DTV
	*NT72568_get_DTV_base(void)
{
	return SYSTEM_DTV_REGISTERS;
}

static inline void NT72568_dtv_sys_enable(unsigned long n,unsigned long gp)
{
	NT72568_SYS_DTV *dtv_base = NT72568_get_DTV_base();
	if(gp==0)
	{
		dtv_base->Control_0|=n;
	}
	else
	{
		dtv_base->Control_1|=n;
	}
}

static inline void NT72568_dtv_sys_reset(unsigned long n,unsigned long gp)
{
	int tmp;
	
	NT72568_SYS_DTV *dtv_base = NT72568_get_DTV_base();
	if(gp==0)
	{
		dtv_base->Reset_0 |= n;
	}
	else
	{
		dtv_base->Reset_1 |= n;
	}
	//udelay(100);
	for(tmp=0;tmp<0x100;tmp++) ;
	if(gp==0)
	{
		dtv_base->Reset_0 &= ~n;
	}
	else
	{
		dtv_base->Reset_1 &= ~n;
	}
	
}
static inline NT72568_UART
	*NT72568_get_base_uart(enum NT72568_uarts_nr n)
{
	return (NT72568_UART *)(NT72568_UART_BASE + (n * 0x1000));
}

#endif


