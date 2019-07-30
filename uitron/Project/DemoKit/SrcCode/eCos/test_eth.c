#if (defined (__ECOS))

#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/maincmd.h>
#include <cyg/infra/mainfunc.h>
#include "Type.h"
#include "stdio.h"
#include <network.h>

MAINFUNC_ENTRY(eth,argc,argv)
{
    printf("\n%s\n",__func__);
    //init_all_network_interfaces_statically();
    init_all_network_interfaces();

    return 0;
}

#endif //#if (defined (__ECOS))