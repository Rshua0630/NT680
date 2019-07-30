#ifndef _DHCP_NVT_H
#define _DHCP_NVT_H
#include "Type.h"

/**
     @name Support 660-IPC architecture or not.
*/
//@{
#define DHCP_IPC_SUPPORT               1
//@}

/**
     @name The maximum length of a host name string is 32 byte and the last one byte must be 0.
*/
//@{
#define DHCP_HOST_NAME_MAX_LEN               60
//@}

/**
     @name The maximum number of interface is 3.
*/
//@{
#define NET_IP_PAIR_MAX_CNT         3
//@}

/**
     @name The maximum length of an IP string is 16 byte and the last one byte must be 0.
*/
//@{
#define NET_IP_MAX_LEN              16
//@}

/**
     @name The maximum length of a network device name is 16 byte and the last one byte must be 0.
*/
//@{
#define NET_INTERFACE_MAX_LEN       16
//@}



/**
     IP configuration of a Host.

     Configure IP related setting of the DSC.
     @note: please follow the network rules to calculate broadcast-ip by ip and netmsk_ip.
*/
typedef struct _NET_IP_PAIR {
	char ip[NET_IP_MAX_LEN];                ///< ip of the host
	char netmask_ip[NET_IP_MAX_LEN];        ///< ip of the subnet mask
	char broadcast_ip[NET_IP_MAX_LEN];      ///< ip of the broadcast;please follow the network rules to calculate broadcast-ip by ip and netmsk_ip.
	char gateway_ip[NET_IP_MAX_LEN];        ///< ip of the gateway
	char server_ip[NET_IP_MAX_LEN];         ///< ip of the server
	unsigned int intrfIdx;                  ///< index of the network interface
} NET_IP_PAIR, *pNET_IP_PAIR;

/**
     Return values of DHCP Dep-Lib Functions.

*/
typedef enum {
	NET_DHCP_RET_OK = 0,                        ///< Success
	NET_DHCP_RET_FAIL,                          ///< Fail
	NET_DHCP_RET_WRONG_IP,                      ///< Fail IP setting
	NET_DHCP_RET_NOT_IMPLEMENT,                 ///< Empty function now
	NET_DHCP_RET_MAX
} NET_DHCP_RET;

/**
     Lease configuration of the DHCP server.

     Configure lease related setting for the DSC to be a DHCP server.
     @note: These fieds,ip_pool_start,ip_pool_end, and ip of NET_IP_PAIR are related.
*/
typedef struct _NET_DHCP_LEASE_CONF {
	char ip_pool_start[NET_IP_MAX_LEN];     ///< minimum ip address of the leases
	char ip_pool_end[NET_IP_MAX_LEN];       ///< maximum ip address of the leases
	unsigned int lease_time;                    ///< a lifecycle of a lease,in unit of second
	unsigned int max_lease_cnt;                 ///< maximum number of leases the DHCP server provides
	char ip_server[NET_IP_MAX_LEN];         ///< ip of the server
	char ip_gateway[NET_IP_MAX_LEN];        ///< ip of the gateway
	char ip_subnetmask[NET_IP_MAX_LEN];     ///< ip of the subnet mask
	char ip_broadcast[NET_IP_MAX_LEN];      ///< ip of the broadcast
	char ip_dns[NET_IP_MAX_LEN];            ///< ip of the DNS server
	char ip_wins[NET_IP_MAX_LEN];           ///< ip of the WINS server
} NET_DHCP_LEASE_CONF, *pNET_DHCP_LEASE_CONF;

/**
     Time-Out and Try-Count of request-IP of the DHCP client.

     Configure time-out interval and frequency of requests that DHCP client sends out.
     @note: These fieds,ip_pool_start,ip_pool_end, and ip of NET_IP_PAIR are related.
*/
typedef struct _NET_DHCP_CLI_TO_CONF {
	long req_tval;              ///< interval of time-out, in unit of microsecond
	long req_max_cnt;           ///< maximum number of retry
} NET_DHCP_CLI_TO_CONF, *PNET_DHCP_CLI_TO_CONF;

typedef struct _DHCP_ASSIGN_IP_INFO {
	unsigned char  macaddr[NET_IP_MAX_LEN];    // MAC adress
	unsigned int ipaddr;                     // IP
} DHCP_ASSIGN_IP_INFO, *PDHCP_ASSIGN_IP_INFO;

#if 0
struct _DHCP_LEASE_TBL {
	unsigned char chaddr[NET_IP_MAX_LEN];    // MAC adress !
	unsigned int yiaddr;   // IP network order
	unsigned int expires;  // host order
} DHCP_LEASE_TBL, *PDHCP_LEASE_TBL; //24byte
#endif

typedef enum {
	DHCPD_CLI_STS_RELEASE_IP = 0,          //release an ip
	DHCPD_CLI_STS_REQUEST_IP,              //request an ip
	DHCPD_CLI_STS_DECLINE_IP,              //decline an ip
	DHCPD_CLI_STS_SRVSTART,                //dhcp server open
	DHCPD_CLI_STS_SRVREADY,                //dhcp server is ready
	DHCPD_CLI_STS_MAX
} DHCPD_CLI_STS;

typedef void (*DHCP_CLI_ATTACH_CB_FUNC)(DHCPD_CLI_STS cliSts, DHCP_ASSIGN_IP_INFO *ipInfo);

ER Dhcp_RegSrvCBCliAttach(DHCP_CLI_ATTACH_CB_FUNC pFunc);

/**
     callback function type.

*/
typedef enum {
	NET_DHCP_CB_TYPE_CLI_REQ_CNT = 0,           ///< DHCP Client Request Cycle and count
	NET_DHCP_CB_TYPE_CLI_REQ_RESULT,            ///< DHCP Client Request IP success or fail(0)
	NET_DHCP_CB_TYPE_MAX
} NET_DHCP_CB_TYPE;

/**
     Prototype of DHCP-Client Call-back function.

     @param[in] cbType       one of the enum NET_DHCP_CB_TYPE.
     @param[in] p1           parameter 1 that depends on definition of different NET_DHCP_CB_TYPE.
     @param[in] p2           parameter 2 that depends on definition of different NET_DHCP_CB_TYPE.
     @param[in] p3           parameter 3 that depends on definition of different NET_DHCP_CB_TYPE.

*/
typedef void (*NET_DHCP_CB_FUNC)(NET_DHCP_CB_TYPE cbType, unsigned int p1, unsigned int p2, unsigned int p3);

/**
     Register a DHCP callback function.

     Callback only if a non-null function is registered.

     @param[in] pFunc       points to a function with type, NET_DHCP_CB_FUNC.
     @return
         - @b NET_DHCP_RET_OK:     Success
         - @b NET_DHCP_RET_Fail:   pFunc is null.
*/
NET_DHCP_RET Dhcp_Reg_Cb(NET_DHCP_CB_FUNC pFunc);

/**
     Set the basic lease information for dhcp server.

     There are internal default setting if the function isn't called.
     If the ip_pool_start/end are set,then the ip of the dhcp server should also be set by the function,Dhcp_Init_Network_With_IP.

     @param[in] pLeaseInfo       points to a structure of NET_DHCP_LEASE_CONF.
     @return
         - @b NET_DHCP_RET_OK:   Success
*/
NET_DHCP_RET Dhcp_Conf_Lease(NET_DHCP_LEASE_CONF *pLeaseInfo);

/**
     Set IP for the DSC.

     The IP works when the DSC initializes the network next time and is not a DHCP client.
     There are internal default settings(ref. note) if the function isn't called.

     @param[in] pIPPairAry        points to a structure of NET_IP_PAIR.
     @param[in] numEthIntrf       how many IP-setting pointed by pIPPairAry.
     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
         - @b NET_DHCP_RET_WRONG_IP interface index, intrfIdx, is not smaller than NET_IP_PAIR_MAX_CNT
     @note:
        Eth0:
            ip = 192.168.0.2
            network mask = 255.255.255.0
            broadcast = 192.168.0.255
            gateway = 192.168.0.1
            server = 192.168.0.1
            interface index = 0
        Eth1:
            ip = 192.168.1.2
            network mask = 255.255.255.0
            broadcast = 192.168.1.255
            gateway = 192.168.1.1
            server = 192.168.1.1
            interface index  = 1
*/
NET_DHCP_RET Dhcp_SetIP_For_Network(NET_IP_PAIR *pIPPairAry, UINT32 numEthIntrf);


/**
     Initialize the network of the DSC.

     Initialize the network with default IPs (ref. note) or that pointed by pIPPairAry.
     This function should not be called after dhcp-client or dhcp-server started.
     There are internal default settings(ref. note) if numEthIntrf is zero.

     @param[in] pIPPairAry        points to a structure of NET_IP_PAIR.
     @param[in] numEthIntrf       how many IP-setting pointed by pIPPairAry.
     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
     @note:
        Eth0:
            ip = 192.168.0.2
            network mask = 255.255.255.0
            broadcast = 192.168.0.255
            gateway = 192.168.0.1
            server = 192.168.0.1
            interface index = 0
        Eth1:
            ip = 192.168.1.2
            network mask = 255.255.255.0
            broadcast = 192.168.1.255
            gateway = 192.168.1.1
            server = 192.168.1.1
            interface index  = 1
*/
NET_DHCP_RET Dhcp_Init_Network_With_IP(NET_IP_PAIR *pIPPairAry, UINT32 numEthIntrf);

/**
     Uninitialize the network of the DSC.

     The DSC un-initializes its network and unbinds its IP.

     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
*/
NET_DHCP_RET Dhcp_UnInit_Network(void);

/**
     Start the DSC as a DHCP client.

     The DSC will automatically get an IP from the DHCP Server if that is started and in the same domain group.

     @param[in] pName             points to a string of host name,and it must be end with 0.
     @param[in] pFunc             points to a function with type, NET_DHCP_CB_FUNC.
     @param[in] forceStopIfPrvExist     force the previous started dhcp-client to stop or not.

     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
     @note:
         The default name is "nvtdsc01cli_dep" if pName is null.
*/
NET_DHCP_RET Dhcp_Client_Start(char *pName, NET_DHCP_CB_FUNC pFunc, BOOL forceStopIfPrvExist);

/**
     Stop the DSC being a DHCP client.

     The DSC is not a DHCP client now and its got IP will be released.

     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
*/
NET_DHCP_RET Dhcp_Client_Stop(void);

/**
     Start the DSC as a DHCP server.

     The DSC will start to be a DHCP Server. Its IP is either the default (note) or
     the one that is set by the function, Dep_Set_IP_For_Network.
     Dhcp server automatically initialize all the network interfces if they are not initialized.

     @param[in] pName             points to a string of host name,and it must be end with 0.

     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
     @note:
        Eth0:
            ip = 192.168.0.2
            network mask = 255.255.255.0
            broadcast = 192.168.0.255
            gateway = 192.168.0.1
            server = 192.168.0.1
            interface index = 0
        Eth1:
            ip = 192.168.1.2
            network mask = 255.255.255.0
            broadcast = 192.168.1.255
            gateway = 192.168.1.1
            server = 192.168.1.1
            interface index  = 1

        The default name is "nvtdsc01srv_dep" if pName is null.

*/
NET_DHCP_RET  Dhcp_Server_Start(char *pName);

/**
     Stop the DSC being a DHCP server.

     The DSC is not a DHCP server now and its IP can be either bound or unbound.

     @param[in] uninitIP            TRUE = unbound, FALSE = bound.

     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
*/
NET_DHCP_RET  Dhcp_Server_Stop(BOOL uninitIP);

/**
     Ping

     A simple network ping command.

     @param[in] addr_str            points to a string in the format of an IP address,end with 0.s
     @param[in] packSize           packet size in byte
     @param[in] queryCnt           the number of ping

     @return
         - @b NET_DHCP_RET_OK:      OK
         - @b NET_DHCP_RET_NOT_IMPLEMENT:    Not implement
*/
NET_DHCP_RET Dhcp_Net_Ping(char *addr_str, int packSize, unsigned int queryCnt);

/**
     Set the timing that DHCP client asks an IP.

     Set the time-out interval and number of retry that the DSC, be a DHCP client,
     requests an IP from a DHCP Server. If they are not set, the default setting (ref. note) will be used.

     @param[in] pDhcpCliTo          points to a NET_DHCP_CLI_TO_CONF structure.

     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_FAIL:    Fail
     @note:
        time-out interval: 500000   microsecond    (100000 ~ 10000000)
        number of retry  : 7        ( 1 ~ 30)

*/
NET_DHCP_RET Dhcp_Set_Cli_TO(NET_DHCP_CLI_TO_CONF *pDhcpCliTo);

/**
     Get the DHCP Client's IP.

     Get the DHCP Client's IP. It works only after Dhcp_Client_Start is called.

     @param[in] pIP          points to an IP in the format of unsigned int.

     @return
         - @b NET_DHCP_RET_OK:      Success

     @note:
        *pIP == 0: No IP

*/
NET_DHCP_RET Dhcp_Client_Get_IP(unsigned int *pIP);

//#NT#Add for gettting server's ip
/**
     Get the Server-IP of the DHCP Client.

     Get the Server-IP of the DHCP Client. It works only after the Dhcp Client got its IP.

     @param[in] pIP          points to an IP in the format of unsigned int.

     @return
         - @b NET_DHCP_RET_OK:      Success

     @note:
        *pIP == 0: No IP

*/
NET_DHCP_RET Dhcp_Client_Get_ServerIP(unsigned int *pIP);

/**
     Notify the DHCP library that the non-blocking Dhcp_Client_Start() got an IP.

     Notify the DHCP library that the non-blocking Dhcp_Client_Start() got an IP.

     @param[in] IP          the got-ip in the DHCP callback function

     @return
         - @b NET_DHCP_RET_OK:      Success
         - @b NET_DHCP_RET_NOT_IMPLEMENT:    Not implement

*/
//NET_DHCP_RET Dhcp_Client_Start_Ok(unsigned int IP);


/**
     Set the network Interface.

     Set the network Interface.

     @param[in] pIntf          "eth0", "eth1", "wlan0"

     @return
         - @b NET_DHCP_RET_OK:               Success
         - @b NET_DHCP_RET_FAIL:             Fail
         - @b NET_DHCP_RET_NOT_IMPLEMENT:    Not implement

*/
NET_DHCP_RET Dhcp_Set_Interface(char *pIntf);

/**
     Get network ip.

     Get ip of the network interface, *pIntf.

     @param[in]  pIntf          "eth0", "eth1", "wlan0"
     @param[out] pIP            point to an IP string,in the format xxx.xxx.xxx.xxx.

     @return
         - @b NET_DHCP_RET_OK:               Success
         - @b NET_DHCP_RET_FAIL:             Fail
         - @b NET_DHCP_RET_NOT_IMPLEMENT:    Not implement

     @note:
        Please prepare a character array with least 'NET_IP_MAX_LEN' bytes (16)

*/
//NET_DHCP_RET Dhcp_Get_IP(char *pIntf, char *pIP);

/**
     DHCP Client is opened or not.

     DHCP Client is opened or not.

     @return
         - @b FALSE:               Not opened.
         - @b TRUE:                Opened

*/
BOOL Dhcp_Client_IsOpened(void);

/**
     Open DHCP IPC Mechanism.

     Open DHCP IPC Mechanism.

     @return
         - @b E_OK:               Ok
         - @b Other:              Fail

*/
ER Dhcp_IpcOpen(UINT32 shareMemStart, UINT32 shareMemSize);

/**
     Close DHCP IPC Mechanism.

     Close DHCP IPC Mechanism.

     @return
         - @b E_OK:               Ok
         - @b Other:              Fail

*/
ER Dhcp_IpcClose(void);

/**
     DHCP IPC Memory Size.

     Get how much memory space that DHCP-IPC needed.

     @return  memory size

*/
UINT32 Dhcp_IpcMemSize(void);


/**
     Install DhcpIpc flag and semaphore id.

*/
void Dhcp_InstallID(void) _SECTION(".kercfg_text");


/**
     Get DHCP Server Lease Table information.

     Get DHCP Server Lease Table information.

     @param[in]  pAssignedIPInfo          point to a array of structure "DHCP_ASSIGN_IP_INFO"
     @param[in]  inCnt                    number of element in the array

     @return
         - @b number of element in the arrary

*/
UINT32 Dhcp_GetAssignedIPInfo(DHCP_ASSIGN_IP_INFO *pAssignedIPInfo, UINT32 inCnt);


//Just for debug
void xDhcp_DmpMemLayout(UINT32 prtNum);


#endif  //_DHCP_NVT_H

