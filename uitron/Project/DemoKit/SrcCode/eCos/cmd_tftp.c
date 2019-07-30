#if (defined (__ECOS))

#include <stdio.h>
#include <stdlib.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/maincmd.h>
#include <cyg/infra/mainfunc.h>
#include "Type.h"
#include "stdio.h"
#include <network.h>
#include <tftp_support.h>
#include <arpa/inet.h>

#define min(x,y) (x<y ? x : y)

#define GETFILE "TCP.txt"
//#define GETFILE "phytool"
#define PUTFILE "tftp_put.txt"

static char buf[512*1024];

MAINFUNC_ENTRY(tftp,argc,argv)
{
    int res, err, len;
    struct sockaddr_in host;
    printf("\n%s\n",__func__);

    memset((char *)&host, 0, sizeof(host));
    printf(">>>>>>>>>>\nsizeof(host) = %d\n<<<<<<<<<<<<<<\n",sizeof(host));
    host.sin_len = sizeof(host);
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = inet_addr("192.168.0.1");
    host.sin_port = 0;

    printf("Trying tftp_get %s %16s...\n", GETFILE, inet_ntoa(host.sin_addr));
    printf("host = 0x%x\n", &host);
    res = tftp_get( GETFILE, &host, buf, sizeof(buf), TFTP_OCTET, &err);
    printf("res = %d, err = %d\n", res, err);
    if (res > 0) {
        diag_dump_buf(buf, min(res,1024));
    }

    len = res;
    printf("Trying tftp_put %s %16s, length %d\n",
                PUTFILE, inet_ntoa(host.sin_addr), len);
    res = tftp_put( PUTFILE, &host, buf, len, TFTP_OCTET, &err);
    printf("put - res: %d\n", res);

    return 0;
}

#endif //#if (defined (__ECOS))