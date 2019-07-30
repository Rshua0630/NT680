
#include <config.h>
#include <common.h>
#include <serial.h>
#include <linux/compiler.h>
#include <asm/arch/nt96660_regs.h>

static void serial_config_port(uint char_len, uint stop_bit, uint parity,
		uint parity_sel, uint stick_parity)
{
	uint config;

	config = (char_len | stop_bit | parity | parity_sel | stick_parity);
	HAL_WRITE_UINT32(HAL_UARTx_LCR(CONFIG_SYS_UART), config);

	config = 0;
	/* UART FIFO ENABLE */
	HAL_READ_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config);
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config | HAL_UART_FCR_FIFO_ENABLE);

	/* UART DMA MODE */
	HAL_READ_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config);
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config | HAL_UART_FCR_MODE_DMA);

	/* UART RX TRIGGER LEVEL */
	HAL_READ_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config);
	config &= ~HAL_UART_FCR_RXTRIGGER_LV;
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config | FCR_RXTRIGGER_LV14);

	config = 0xc9;
	/* UART RX FIFO RESET */
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config | HAL_UART_FCR_RXFIFO_RST);
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config);
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config & ~HAL_UART_FCR_RXFIFO_RST);

	config = 0xc9;
	/* UART TX FIFO RESET */
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config | HAL_UART_FCR_TXFIFO_RST);
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config);
	HAL_WRITE_UINT32(HAL_UARTx_IFCR(CONFIG_SYS_UART), config & ~HAL_UART_FCR_TXFIFO_RST);

	config = 0;
	/* UART RX LINE STATUS ENABLE */
	HAL_READ_UINT32(HAL_UARTx_IER(CONFIG_SYS_UART), config);
	HAL_WRITE_UINT32(HAL_UARTx_IER(CONFIG_SYS_UART), config | HAL_UART_IER_LINESTATUS);

	/* UART RX DATA ENABLE */
	HAL_READ_UINT32(HAL_UARTx_IER(CONFIG_SYS_UART), config);
	HAL_WRITE_UINT32(HAL_UARTx_IER(CONFIG_SYS_UART), config | HAL_UART_IER_RXDATA);
}
static void nt96660_serial_setbrg(void)
{
	uint divisor, baudrate, val;

	baudrate = CONFIG_BAUDRATE;

	divisor = (HAL_UARTX_CLKMHZ(CONFIG_SYS_UART)) / (baudrate * 16);

    //printf("freq = 0x%x, baudrate = 0x%x,ahb_freq = 0x%x,  divisor = 0x%x\n",freq[0], baudrate,ahb_freq, divisor);

	HAL_READ_UINT32(HAL_UARTx_LCR(CONFIG_SYS_UART), val);

	HAL_WRITE_UINT32(HAL_UARTx_LCR(CONFIG_SYS_UART), val | HAL_UART_LCR_DLAB);

	HAL_WRITE_UINT32(HAL_UARTx_DLABLO(CONFIG_SYS_UART), divisor);
	HAL_WRITE_UINT32(HAL_UARTx_DLABHI(CONFIG_SYS_UART), 0);

	HAL_WRITE_UINT32(HAL_UARTx_LCR(CONFIG_SYS_UART), val & ~HAL_UART_LCR_DLAB);
}

static void nt96660_serial_putc(const char c)
{
	uint status;

	if (c == '\n')
		serial_putc( '\r' );

	HAL_READ_UINT32(HAL_UARTx_LSR(CONFIG_SYS_UART), status);
	while (!(status & HAL_UART_LSR_THREMPTY))
	{
		/* do nothing, wait for characters in FIFO sent */
		HAL_READ_UINT32(HAL_UARTx_LSR(CONFIG_SYS_UART), status);
	}
	HAL_WRITE_UINT8(HAL_UARTx_RBR(CONFIG_SYS_UART), c);
}

static int nt96660_serial_getc(void)
{
#if (CONFIG_SYS_UART==0)
    //no get on uart 0
    while(1)
    {
        __asm__("nop");
    }
#elif (CONFIG_SYS_UART==1)
	int c;

	while ( !serial_tstc() )
	{
		/* do nothing, wait for incoming characters */
	}
	HAL_READ_UINT8(HAL_UARTx_RBR(CONFIG_SYS_UART), c);

  	return c;
#endif
}

static int nt96660_serial_tstc(void)
{
	uint status;

	HAL_READ_UINT32(HAL_UARTx_LSR(CONFIG_SYS_UART), status);
	if (status & HAL_UART_LSR_DATAREADY)
		return 1;	/* data ready */
	else
		return 0;	/* no character available */
}

static int nt96660_serial_init(void)
{
#if (CONFIG_SYS_UART==1)
	uint val;

	/* UART INTERRUPT DISABLE */
	HAL_READ_UINT32(HAL_UARTx_IER(CONFIG_SYS_UART), val);
	HAL_WRITE_UINT32(HAL_UARTx_IER(CONFIG_SYS_UART), val & ~(HAL_UART_IER_INTMASK));

	serial_setbrg();

	serial_config_port(LCR_CHAR_LEN8, LCR_STOP1, LCR_PARITY_DISABLE,
			0, LCR_PARITY_STICK_DISABLE);
#endif
	return 0;
}

static struct serial_device nt96660_serial_drv = {
	.name	= "nt96660_serial",
	.start	= nt96660_serial_init,
	.stop	= NULL,
	.setbrg	= nt96660_serial_setbrg,
	.putc	= nt96660_serial_putc,
	.puts	= default_serial_puts,
	.getc	= nt96660_serial_getc,
	.tstc	= nt96660_serial_tstc,
};

__weak struct serial_device *default_serial_console(void)
{
	return &nt96660_serial_drv;
}
