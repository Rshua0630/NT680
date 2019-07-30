#ifndef CYGONCE_VAR_SERIAL_H
#define CYGONCE_VAR_SERIAL_H

#include <pkgconf/system.h>
#include <pkgconf/io_serial.h>

#include <cyg/io/io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/io/devtab.h>
#include <cyg/io/serial.h>

#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_misc.h>

#include <cyg/infra/diag.h>
#include <cyg/infra/cyg_ass.h>


typedef void (* micon_rcv_callback)(serial_channel *chan, unsigned char c);

externC bool cyg_hal_micon_putc(unsigned char c);
externC unsigned char cyg_hal_micon_getc(void);
externC Cyg_ErrNo cyg_hal_micon_set_config(cyg_uint32 key, const void *xbuf,
                                           cyg_uint32 *len);
externC void cyg_hal_micon_rcv_callback_register(micon_rcv_callback fn);
externC micon_rcv_callback cyg_hal_micon_rcv_get_callback(void);

#endif /* CYGONCE_PLF_SERIAL_H */
