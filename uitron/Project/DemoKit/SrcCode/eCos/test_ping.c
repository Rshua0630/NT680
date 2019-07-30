#if (defined (__ECOS))

#include <cyg/infra/diag.h>
#include <network.h>
#include <arpa/inet.h>

#define PING_TEST_DBG            0
#if PING_TEST_DBG
#define DbgMsg_PingTestIO        diag_printf
#define DbgMsg_PingTestErr       diag_printf
#else
#define DbgMsg_PingTestIO(...)
#define DbgMsg_PingTestErr       diag_printf
#endif

#define USE_MAINFUNC_ENTRY      1
#define NUM_PINGS               16
#define MAX_PACKET              4096
#define MIN_PACKET              64
#define MAX_SEND                4000
#define PACKET_ADD              ((MAX_SEND - MIN_PACKET)/NUM_PINGS)
#define UNIQUEID                0x1234


static unsigned char pkt1[MAX_PACKET], pkt2[MAX_PACKET];

void ping_cmd(char *addr_str);



#if USE_MAINFUNC_ENTRY
#include <cyg/infra/mainfunc.h>
MAINFUNC_ENTRY(ping,argc,argv)
#else
int main( int argc, char **argv )
#endif
{
    DbgMsg_PingTestIO("+%s: %d\r\n",__func__,argc);
    if (argc < 2)
    {
        DbgMsg_PingTestErr("Syntax: ping command\r\n");
        DbgMsg_PingTestErr("command:\r\n");
        DbgMsg_PingTestErr("    ping xxx.xxx.xxx.xxx  - ping the host with IP = xxx.xxx.xxx.xxx\r\n");
        return -1;
    }
    #if PING_TEST_DBG
    {
        int i = 0;
        for (i = 0; i < argc; i++)
        {
            DbgMsg_PingTestIO("%s ", argv[i]);
        }
        DbgMsg_PingTestIO("\r\n");
    }
    #endif

    if (strncmp("-help",argv[1],5)==0)
    {
        DbgMsg_PingTestErr("Syntax: ping command\r\n");
        DbgMsg_PingTestErr("command:\r\n");
        DbgMsg_PingTestErr("    ping xxx.xxx.xxx.xxx  - ping the host with IP = xxx.xxx.xxx.xxx\r\n");
    }
    else if (argc == 2)
    {
        ping_cmd(argv[1]);
    }
    else
    {
        DbgMsg_PingTestErr("Not Support command!\r\n");
        return 1;
    }

    return 0;
}


// Compute INET checksum
static int inet_cksum(u_short *addr, int len)
{
    register int nleft = len;
    register u_short *w = addr;
    register u_short answer;
    register u_int sum = 0;
    u_short odd_byte = 0;

    /*
     *  Our algorithm is simple, using a 32 bit accumulator (sum),
     *  we add sequential 16 bit words to it, and at the end, fold
     *  back all the carry bits from the top 16 bits into the lower
     *  16 bits.
     */
    while( nleft > 1 )  {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if( nleft == 1 ) {
        *(u_char *)(&odd_byte) = *(u_char *)w;
        sum += odd_byte;
    }

    /*
     * add back carry outs from top 16 bits to low 16 bits
     */
    sum = (sum >> 16) + (sum & 0x0000ffff); /* add hi 16 to low 16 */
    sum += (sum >> 16);                     /* add carry */
    answer = ~sum;                          /* truncate to 16 bits */
    return (answer);
}

static int show_icmp(unsigned char *pkt, int len,
          struct sockaddr_in *from, struct sockaddr_in *to)
{
    cyg_tick_count_t *tp, tv;
    struct ip *ip;
    struct icmp *icmp;
    tv = cyg_current_time();
    ip = (struct ip *)pkt;
    if (((unsigned int)len < sizeof(*ip)) || ip->ip_v != IPVERSION) {
        diag_printf("%s: Short packet or not IP! - Len: %d, Version: %d\n",
                    inet_ntoa(from->sin_addr), len, ip->ip_v);
        return 0;
    }
    icmp = (struct icmp *)(pkt + sizeof(*ip));
    len -= (sizeof(*ip) + 8);
    tp = (cyg_tick_count_t *)&icmp->icmp_data;
    if (icmp->icmp_type != ICMP_ECHOREPLY) {
        diag_printf("%s: Invalid ICMP - type: %d\n",
                    inet_ntoa(from->sin_addr), icmp->icmp_type);
        return 0;
    }
    if (icmp->icmp_id != UNIQUEID) {
        diag_printf("%s: ICMP received for wrong id - sent: %x, recvd: %x\n",
                    inet_ntoa(from->sin_addr), UNIQUEID, icmp->icmp_id);
    }
    diag_printf("%d bytes from %s: ", len, inet_ntoa(from->sin_addr));
    diag_printf("icmp_seq=%d", icmp->icmp_seq);
    diag_printf(", time=%dms\n", (int)(tv - *tp)*10);
    return (from->sin_addr.s_addr == to->sin_addr.s_addr);
}

static void ping_host(int s, struct sockaddr_in *host)
{
    struct icmp *icmp = (struct icmp *)pkt1;
    int icmp_len = MIN_PACKET;
    int seq, ok_recv, bogus_recv;
    cyg_tick_count_t *tp;
    long *dp;
    struct sockaddr_in from;
    int i, len, fromlen;

    ok_recv = 0;
    bogus_recv = 0;
    diag_printf("#PING server %s\n", inet_ntoa(host->sin_addr));
    for (seq = 0;  seq < NUM_PINGS;  seq++, icmp_len += PACKET_ADD ) {
        // Build ICMP packet
        icmp->icmp_type = ICMP_ECHO;
        icmp->icmp_code = 0;
        icmp->icmp_cksum = 0;
        icmp->icmp_seq = seq;
        icmp->icmp_id = 0x1234;
        // Set up ping data
        tp = (cyg_tick_count_t *)&icmp->icmp_data;
        *tp++ = cyg_current_time();
        dp = (long *)tp;
        for (i = sizeof(*tp);  i < icmp_len;  i += sizeof(*dp)) {
            *dp++ = i;
        }
        // Add checksum
        icmp->icmp_cksum = inet_cksum( (u_short *)icmp, icmp_len+8);
        // Send it off
        if (sendto(s, icmp, icmp_len+8, 0, (struct sockaddr *)host, sizeof(*host)) < 0) {
            diag_printf("Err: sendto\r\n");
            continue;
        }
        // Wait for a response
        fromlen = sizeof(from);
        len = recvfrom(s, pkt2, sizeof(pkt2), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
        if (len < 0) {
            diag_printf("No Response from IP=%s\r\n",inet_ntoa(host->sin_addr));
            //diag_printf("Err: recvfrom\r\n");;
            icmp_len = MIN_PACKET - PACKET_ADD; // go back to small packet size
            seq+=4;
        } else {
            //diag_printf("Got Response from IP=%s\r\n",inet_ntoa(from.sin_addr));
            if (show_icmp(pkt2, len, &from, host)) {
                ok_recv++;
            } else {
                bogus_recv++;
            }
        }
    }
}
//ping test ...
void ping_cmd(char *addr_str)
{
    struct protoent *p;
    struct timeval tv;
    struct sockaddr_in host;
    int s;
    in_addr_t ping_addr;

    diag_printf("+%s:%s\r\n",__func__, addr_str);

    if (addr_str)
    {
        ping_addr = inet_addr(addr_str);
        diag_printf("+%s:addr=%s => 0x%x\r\n",__func__,addr_str,ping_addr);
    }
    else
    {
        diag_printf("+%s:addr NULL\r\n",__func__);
        return;
    }

    if ((p = getprotobyname("icmp")) == (struct protoent *)0) {
        diag_printf("Err: getprotobyname\r\n");
        return;
    }
    s = socket(AF_INET, SOCK_RAW, p->p_proto);
    if (s < 0) {
        diag_printf("Err: socket\r\n");
        return;
    }
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    //20180104 nash: coverity fix, CID#109266
    //setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))) {
        diag_printf("Err: setsockopt\r\n");
        ///20180104 nash: coverity fix, CID#110855
        close(s);
        return;
    }

    // Set up host address
    host.sin_family = AF_INET;
    host.sin_len = sizeof(host);
    host.sin_addr.s_addr = ping_addr;
    host.sin_port = 0;
    ping_host(s, &host);
    close(s);
    diag_printf("-%s:addr=0x%x,s=%d\r\n",__func__,ping_addr,s);
}

#endif //#if (defined (__ECOS))
