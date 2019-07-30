#ifndef _NVTIMETH_DEF_H
#define _NVTIMETH_DEF_H

#include <cyg/infra/cyg_type.h>

/**
    ethernet link status

    @note For NVTIMETH_LINK_CB
*/
#define NVTIMETH_LINK_DOWN          (0x01)          //< link down
#define NVTIMETH_LINK_UP            (0x02)          //< link up

/**
    ethernet duplex mode

    @note For nvtimeth_set_duplex()
*/
#define NVTIMETH_DUPLEX_HALF        (0x01)          //< half duplex
#define NVTIMETH_DUPLEX_FULL        (0x02)          //< full duplex

/*
    ethernet link status callback

    @param[in] sts          new link status
                            - @b NVTIMETH_LINK_DOWN
                            - @b NVTIMETH_LINK_UP

    @return void
*/
//@{
typedef void (*NVTIMETH_LINK_CB)(unsigned int sts);

__externC void nvtimeth_register_link_cb(NVTIMETH_LINK_CB fp);
__externC void nvtimeth_set_speed(cyg_uint32 speed);
__externC void nvtimeth_set_duplex(cyg_uint32 duplex);

#endif