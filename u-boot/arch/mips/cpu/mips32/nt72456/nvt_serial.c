
#include <config.h>
#include <common.h>
#include <serial.h>
#include <linux/compiler.h>

static int nt72456_serial_init(void)
{
	return 0;
}

static void nt72456_serial_setbrg(void)
{

}

static void nt72456_serial_putc(const char c)
{

}

static int nt72456_serial_getc(void)
{
	char c = '0';

	return c;
}

static nt72456_serial_tstc(void)
{
	return 0;
}

static struct serial_device nt72456_serial_drv = {
	.name	= "nvt_serial",
	.start	= nt72456_serial_init,
	.stop	= NULL,
	.setbrg	= nt72456_serial_setbrg,
	.putc	= nt72456_serial_putc,
	.puts	= default_serial_puts,
	.getc	= nt72456_serial_getc,
	.tstc	= nt72456_serial_tstc,
};

__weak struct serial_device *default_serial_console(void)
{
	return &nt72456_serial_drv;
}
