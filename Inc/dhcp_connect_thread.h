#ifndef __DHCP_CONNECT_THREAD_H
#define __DHCP_CONNECT_THREAD_H

// Gateway Address

#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1 

// Static IP ADDRESS

#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   10
   
// NETMASK

#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

// Defines

#define STATE_WAIT_IP_ADDRESS   0x0
#define STATE_ADDRESS_ASSIGNED  0x1
#define STATE_TIMEOUT           0x2
#define STATE_ERROR             0x3

#define MAX_DHCP_TRIES          4

// dhcp_connect_thread

void dhcp_connect_thread(void const* argument);

#endif
