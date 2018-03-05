
// Includes

#include "dhcp_connect_thread.h"
#include "lwip/opt.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "ui.h"

#if defined(WITH_DEBUG_LOG)
#include "log.h"
#endif

// use_static_ip_address

static inline void use_static_ip_address(struct netif* netif)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;

    if ( !netif )
        return;

    IP_ADDR4(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
    IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
    netif_set_addr(netif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
}

// negotiate_ip_address

static inline uint8_t negotiate_ip_address(struct netif* netif)
{
    struct dhcp* dhcp;

    if ( !netif )
        return(STATE_ERROR);

    // Start DHCP negotiation.

    ip_addr_set_zero_ip4(&netif->ip_addr);
    ip_addr_set_zero_ip4(&netif->netmask);
    ip_addr_set_zero_ip4(&netif->gw);       
    dhcp_start(netif);

    for ( ;; )
    {
        if ( dhcp_supplied_address(netif) ) 
        {
            return(STATE_ADDRESS_ASSIGNED);
        }
        else 
        {
            // Check if max amount of retries has been exceeded.

            dhcp= (struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
            if ( dhcp && dhcp->tries > MAX_DHCP_TRIES )
            {
                // Stop DHCP.

                dhcp_stop(netif);
            
                // Use static ip address.

                use_static_ip_address(netif);

                return(STATE_TIMEOUT);
            }
        }

        // Sleep for 250 ms.

        osDelay(250);
    }
}

// dhcp_connect_thread

void dhcp_connect_thread(void const* argument)
{
    struct netif* netif;
    uint8_t       status_text[128];
    uint8_t       state;

    // Get LwIP data struct.

    netif= (struct netif*)argument;
    if ( !netif )
        goto end_thread;

    // Check that network cable is connected.

    if ( !netif_is_up(netif) )
    {
#if defined(WITH_DEBUG_LOG) 
        DEBUG_LOG("Network cable not connected.");
#endif
        // Display connection status.

        ui_display_status_bar_string((uint8_t*)"Network cable not connected.");

        goto end_thread;
    }

    ui_display_status_bar_string((uint8_t*)"Connecting to DHCP server...");

    // Attempt to negotiate ip address with DHCP server.

    state= negotiate_ip_address(netif);
    if ( state == STATE_ADDRESS_ASSIGNED )
    {
        sprintf((char *)status_text, "IP address from DHCP server: %s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
    }
    else // state == STATE_TIMEOUT
    {
        sprintf((char *)status_text, "DHCP Timeout. Static IP address: %s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
    }

    // Display connection status.

    ui_display_status_bar_string((uint8_t*)status_text);

#if defined(WITH_DEBUG_LOG) 
    DEBUG_LOG((char *)status_text);
#endif

    end_thread:

    // Terminate DHCP thread.

    osThreadTerminate(NULL);
}
