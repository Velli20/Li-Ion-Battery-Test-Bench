// Includes

#include <string.h>
#include <stdio.h>

#include "main.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "ethernetif.h"
#include "server.h"
#include "cmsis_os.h"
#include "log.h"

// Web interface html files

#include "web_interface_home_html.h"
#include "web_interface_home_js.h"
#include "web_interface_styles_css.h"
#include "web_interface_not_found_html.h"

// Variables

extern osMessageQId    main_thread_queue;
static struct netconn* sse_client;
static osMutexId       server_mutex;

// Macros

#define IS_HTTP_GET_COMMAND(HEADER, HTML)                       \
(                                                               \
    strncmp(HEADER, ("GET /" HTML), 5 + (sizeof(HTML)-1)) == 0  \
) 

#define IS_HTTP_POST_COMMAND(HEADER, HTML)                      \
(                                                               \
    strncmp(HEADER, ("POST /" HTML), 6 + (sizeof(HTML)-1)) == 0 \
)

// Response headers

#define HTTP_RESPONSE_OK_200              \
    "HTTP/1.1 200 OK\r\n\r\n"

#define HTTP_SSE_EVENT_STREAM_HEADER      \
    "id: 0\ndata: NONE\n\n\r\n"                               

#define HTTP_SSE_RESPONSE_HEADER          \
    "HTTP/1.1 200 OK\r\n"                 \
    "Access-Control-Allow-Origin: *\r\n"  \
    "Content-Type: text/event-stream\r\n" \
    "Cache-Control: no-cache\r\n"         \
    "Connection: keep-alive\r\n"          \
    "\r\n"                                 

// http_server_write_rtos_task_stats

static uint8_t http_server_write_rtos_task_stats(struct netconn* conn)
{
    portCHAR task_list_buffer[400];
    err_t    result;

    if ( !conn )
        return (SERVER_RESULT_ERROR);

    // Write header.

    result= netconn_write(conn, HTTP_RESPONSE_HEADER_TEXT_FILE, strlen((char*)HTTP_RESPONSE_HEADER_TEXT_FILE), NETCONN_NOCOPY);
    if ( result != ERR_OK )
    {
        DEBUG_LOG("Error occured while writing HTTP header");
        return (SERVER_RESULT_ERROR);
    }

    // Get task list.

    if ( osThreadList((unsigned char *)(task_list_buffer)) != osOK )
    {
        DEBUG_LOG("Error occured while accuiring task list");
        return (SERVER_RESULT_ERROR);
    }

    // Write task list.

    result= netconn_write(conn, task_list_buffer, strlen((char*)task_list_buffer), NETCONN_COPY);
    if ( result != ERR_OK )
    {
        DEBUG_LOG("Error occured while writing task list");
        return (SERVER_RESULT_ERROR);
    }

    return (SERVER_RESULT_OK);
}

// http_server_begin_sse

static uint8_t http_server_begin_sse(struct netconn* conn)
{
    if ( !conn )
        return (SERVER_RESULT_ERROR);
    
    // Close old connection and free its resources.

    if ( sse_client )
    {
        netconn_close(sse_client);
        netconn_delete(sse_client);
    }

    sse_client= conn;

    if ( netconn_write(conn, HTTP_SSE_RESPONSE_HEADER, sizeof(HTTP_SSE_RESPONSE_HEADER)-1, NETCONN_NOCOPY) != ERR_OK )
        return (SERVER_RESULT_ERROR);

    return (SERVER_RESULT_OK);
}

// http_server_serve

static uint8_t http_server_serve(struct netconn* conn,
                                 BMS_DATA*       bms_data) 
{
    struct netbuf* inbuf;
    char*          buf;
    err_t          recv_err;
    uint16_t       buflen;
    uint8_t        result;
    uint8_t        message;
    
    // Receive data from netconn.

    recv_err= netconn_recv(conn, &inbuf);
    if ( recv_err != ERR_OK )
        goto end;

    // Check if an error occured while reading request.

    if ( netconn_err(conn) != ERR_OK ) 
        goto end;

    // Read buffer.

    if ( (netbuf_data(inbuf, (void**)&buf, &buflen) != ERR_OK) || (!buf || !*buf) )
        goto end;
      
    // Check for HTTP POST command.

    // if ( (buflen >= 6) && (strncmp(buf, "POST /", 6) != 0) )
    //     goto post_command;
    
    // Check if home page is requested.

    if ( IS_HTTP_GET_COMMAND(buf, "home.html") || (strncmp(buf, "GET / ", 6) == 0) ) 
    {
        netconn_write(conn, home_html, sizeof(home_html)-1, NETCONN_NOCOPY);
    }

    else if ( IS_HTTP_GET_COMMAND(buf, "styles.css") )
    {         
        netconn_write(conn, styles_css, sizeof(styles_css)-1, NETCONN_NOCOPY);
    }

    // Check if home page javascript file is requested.

    else if ( IS_HTTP_GET_COMMAND(buf, "home.js") )
    {    
        netconn_write(conn, home_js, sizeof(home_js)-1, NETCONN_NOCOPY);
    }

    // Check if BMS data json file is requested.

    else if ( IS_HTTP_GET_COMMAND(buf, "bms_data.json") )
    {    
        server_write_bms_json_data(conn, bms_data);
    }

    // Write file list.

    else if ( IS_HTTP_GET_COMMAND(buf, "test_results.json") )
    {
        server_write_file_list_json_data(conn);
    }

    // Write RTOS task list.

    else if ( IS_HTTP_GET_COMMAND(buf, "rtos_tasks.text") )
    {
        http_server_write_rtos_task_stats(conn);
    }

    // Check for SSE stream request.

    else if ( IS_HTTP_GET_COMMAND(buf, "event_callback") )
    {   
        // Try to start sse stream.

        if ( http_server_begin_sse(conn) != SERVER_RESULT_OK )
            goto end;
        
        // Deallocate input buffer and keep connection alive.

        netbuf_delete(inbuf);

        return (SERVER_RESULT_KEEP_CONNECTION_ALIVE);
    }
    
    // Check for if start/stop button is clicked in browser.

    else if ( IS_HTTP_POST_COMMAND(buf, "command") )
    {
        // Start button clicked. Pass event to main thread.

        if ( strstr(buf, "\"command\":\"start\"") )
            message= CMSIS_OS_EVENT_START_BUTTON_CLICKED;

        // Stop button clicked.

        else if ( strstr(buf, "\"command\":\"stop\"") )
            message= CMSIS_OS_EVENT_STOP_BUTTON_CLICKED;

        // BMS data simulation on.

        else if ( strstr(buf, "\"simulation\":\"enabled\"") )
            message= CMSIS_OS_EVENT_BMS_DATA_SIMULATION_ON;

        // BMS data simulation off.

        else if ( strstr(buf, "\"simulation\":\"disabled\"") )
            message= CMSIS_OS_EVENT_BMS_DATA_SIMULATION_OFF;

        // Unknown command.

        else
            goto end;

        // Put message to main thread queue.
        
        result= osMessagePut(main_thread_queue, message, osWaitForever);
        DEBUG_ASSERT(result == osOK);

        // Send OK 200 response.

        netconn_write(conn, HTTP_RESPONSE_OK_200, sizeof(HTTP_RESPONSE_OK_200)-1, NETCONN_NOCOPY);
    }

    // Check if battery test cycle text file is requested. 
    // Temporary implementation. TODO: regex implementation.

    else if ( strstr(buf, "CYCLE_0.txt") )
    {
        server_write_cycle_result_text_file(conn, 0);
    }

    // Unknown request. Return 404 html.

    else 
    {   
        netconn_write(conn, not_found_html, sizeof(not_found_html)-1, NETCONN_NOCOPY);
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("Request not recoqnized. Header:\n%s\n", buf);
#endif
              
    }

    end:
  
    // Close connection.

    netconn_close(conn);
  
    // Clear buffer.

    netbuf_delete(inbuf);
    
    return (SERVER_RESULT_OK);
}

// http_server_netconn_thread

static void http_server_netconn_thread(void* arg)
{ 
    struct netconn* conn; 
    struct netconn* newconn;
    BMS_DATA*       bms_data;
    err_t           error;
    uint8_t         result;

    bms_data= (BMS_DATA*)arg;
    DEBUG_ASSERT(bms_data);

    // Create new TCP connection struct.

    conn= netconn_new(NETCONN_TCP);
    if ( !conn )
    {
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("Error while creating TCP connection");
#endif
        goto end_thread;
    }

    // Bind to port 80 with default IP address.

    if ( netconn_bind(conn, NULL, 80) != ERR_OK )
    {
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("Failed to bind netconn to port 80");
#endif
        goto end_thread;
    }

    // Set netconn into LISTEN mode.

    netconn_listen(conn);
  
    while ( 1 ) 
    {
        // Accept incoming connetion.

        error= netconn_accept(conn, &newconn);
        if ( error != ERR_OK )
        {
#if defined(WITH_DEBUG_LOG)
            DEBUG_LOG("Error while accepting incoming connection. error=%d", error);
#endif
            if ( sse_client )
            {
                netconn_close(sse_client);
                netconn_delete(sse_client);
                sse_client= 0;
            }
            continue;
        }
        
        // Try to obtain server mutex.

        DEBUG_ASSERT(osMutexWait(server_mutex, osWaitForever) == osOK);

        // Handle connection.

        result= http_server_serve(newconn, bms_data);
        DEBUG_ASSERT(result == SERVER_RESULT_KEEP_CONNECTION_ALIVE || result == SERVER_RESULT_OK);

        if ( result != SERVER_RESULT_KEEP_CONNECTION_ALIVE )
        {
            // Close connection and free its resources.

            DEBUG_ASSERT(netconn_delete(newconn) == ERR_OK);
        }

        DEBUG_ASSERT(osMutexRelease(server_mutex) == osOK);
    }
    
    end_thread:

    // Terminate this thread.

    osThreadTerminate(osThreadGetId());
}

// http_server_netconn_interface_init

static void http_server_netconn_interface_init(struct netif* gnetif)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
 
#ifdef USE_DHCP
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
#else
    IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
    IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
    IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif
    
    // Initialize lwIP network interface.

    DEBUG_ASSERT(netif_add(gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input) != 0);
  
    // Registers the default network interface.

    netif_set_default(gnetif);
  
    if ( netif_is_link_up(gnetif) )
    {
        // When the netif is fully configured this function must be called.

        netif_set_up(gnetif);
    }
    else
    {
        // When the netif link is down this function must be called.

        netif_set_down(gnetif);
    }
}

// http_server_netconn_init

uint8_t http_server_netconn_init(struct netif* gnetif, 
                                 BMS_DATA*     data)
{
    if ( !gnetif || !data )
        return (SERVER_RESULT_ERROR);

    // Initialize ethernet interface.

    http_server_netconn_interface_init(gnetif);

    // Create server mutex to limit netconn writing to one thread at the time. 

    osMutexDef(http_mutex);
    server_mutex= osMutexCreate(osMutex(http_mutex));
    if ( !server_mutex )
        return (SERVER_RESULT_ERROR);

    // Start new HTTP connection thread.

    sys_thread_new("HTTP", http_server_netconn_thread, data, DEFAULT_THREAD_STACKSIZE, osPriorityAboveNormal);

    return (SERVER_RESULT_OK);
}

// http_server_send_event_to_client
// Push event to browser using Server Sent Event API.

uint8_t http_server_send_event_to_client(uint8_t event)
{    
    if ( !sse_client )
        return (SERVER_RESULT_ERROR);
    
    // Try to obtain server mutex.

    if ( osMutexWait(server_mutex, osWaitForever) != osOK )
        return (SERVER_RESULT_ERROR);

    // Push data to browser.

    if ( netconn_write(sse_client, HTTP_SSE_EVENT_STREAM_HEADER, sizeof(HTTP_SSE_EVENT_STREAM_HEADER)-1, NETCONN_NOCOPY) != ERR_OK )
    {
        // In case of error close sse connection.

        netconn_close(sse_client);
        netconn_delete(sse_client);
        sse_client= 0;
    }

    // Release mutex.

    if ( osMutexRelease(server_mutex) != osOK )
        return (SERVER_RESULT_ERROR);

    return (SERVER_RESULT_OK);
}
