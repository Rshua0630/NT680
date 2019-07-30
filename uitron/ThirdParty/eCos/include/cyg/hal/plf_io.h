#ifndef CYGONCE_HAL_PLF_IO_H
#define CYGONCE_HAL_PLF_IO_H

#ifndef __ASSEMBLER__
extern void hal_plf_hardware_init(void);
#define HAL_PLF_HARDWARE_INIT() \
    hal_plf_hardware_init()

#endif  //__ASSEMBLER__

#endif // CYGONCE_HAL_PLF_IO_H

