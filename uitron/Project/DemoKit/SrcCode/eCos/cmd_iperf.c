#if (defined (__ECOS))

#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/maincmd.h>
#include <cyg/infra/mainfunc.h>
#include "Iperf.h"
#include "Type.h"
#include "stdio.h"

MAINFUNC_ENTRY(iperf,argc,argv)
{
    Iperf_Close();
    Iperf_Open(argc, argv);
    return 0;
}

MAINFUNC_ENTRY(iperfc,argc,argv)
{
    Iperf_RunClient(argc, argv);
    return 0;
}

MAINFUNC_ENTRY(iperfresult,argc,argv)
{
    UINT32 ResultAddr = 0;
    if(argc == 2)
    {
        sscanf(argv[1],"%u",&ResultAddr);
        *(float *)ResultAddr = Iperf_GetResult();
    }
    printf("\nIperf Result = %f Mbits/sec @ 0x%X\n",Iperf_GetResult(),ResultAddr);
    return 0;
}

#endif //#if (defined (__ECOS))