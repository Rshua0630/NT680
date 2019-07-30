#ifndef CYGONCE_HAL_DIAG_H
#define CYGONCE_HAL_DIAG_H

#include <pkgconf/hal.h>
#include <cyg/infra/cyg_type.h>

#if defined(CYGSEM_HAL_VIRTUAL_VECTOR_DIAG)

#include <cyg/hal/hal_if.h>

#define HAL_DIAG_INIT()          hal_if_diag_init()
#define HAL_DIAG_WRITE_CHAR(_c_) hal_if_diag_write_char(_c_)
#define HAL_DIAG_READ_CHAR(_c_)  hal_if_diag_read_char(&_c_)
#else

/*---------------------------------------------------------------------------*/
/* functions implemented in hal_diag.c                                       */

externC void hal_diag_init(void);
externC void hal_diag_write_char(char c);
externC void hal_diag_read_char(char *c);

/*---------------------------------------------------------------------------*/

#define HAL_DIAG_INIT()             hal_diag_init()
#define HAL_DIAG_WRITE_CHAR(_c_)    hal_diag_write_char(_c_)
#define HAL_DIAG_READ_CHAR(_c_)     hal_diag_read_char(&_c_)

#endif
#endif // CYGONCE_HAL_DIAG_H
