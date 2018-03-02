// Includes

#include <string.h>
#include <stdio.h>

#include "main.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "httpserver-netconn.h"
#include "cmsis_os.h"
#include "log.h"

// Variables

extern osMessageQId main_thread_queue;

// Macros

#define IS_HTTP_GET_COMMAND(HEADER, HTML)                       \
(                                                               \
    strncmp(HEADER, ("GET /" HTML), 5 + (sizeof(HTML)-1)) == 0  \
) 

#define IS_HTTP_POST_COMMAND(HEADER, HTML)                      \
(                                                               \
    strncmp(HEADER, ("POST /" HTML), 6 + (sizeof(HTML)-1)) == 0 \
)

#define HTTP_RESPONSE_OK_200 "HTTP/1.0 200 OK\r\n\r\n"

// http_server_serve

static void http_server_serve(struct netconn* conn) 
{
    struct netbuf* inbuf;
    struct fs_file file;
    char*          buf;
    err_t          recv_err;
    uint16_t       buflen;
    uint8_t        result;
    
    // Receive data from netconn.

    recv_err= netconn_recv(conn, &inbuf);
    if ( recv_err != ERR_OK )
        goto end;

    // Check if there an error occured while reading request.

    if ( netconn_err(conn) != ERR_OK ) 
        goto end;

    // Read buffer.

    if ( (netbuf_data(inbuf, (void**)&buf, &buflen) != ERR_OK) || (!buf || !*buf) )
        goto end;
      
    // Check for HTTP POST command.

    // if ( (buflen >= 6) && (strncmp(buf, "POST /", 6) != 0) )
    //     goto post_command;
    
    if ( IS_HTTP_GET_COMMAND(buf, "home.html") || (strncmp(buf, "GET / ", 6) == 0) ) 
    {
        fs_open(&file, "/home.html"); 
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
    }

    else if ( IS_HTTP_GET_COMMAND(buf, "battery.html") ) 
    {
        fs_open(&file, "/battery.html"); 
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
    }
    else if ( IS_HTTP_GET_COMMAND(buf, "material.css") )
    {        
        fs_open(&file, "/material.css"); 
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
    }

    else if ( IS_HTTP_GET_COMMAND(buf, "styles.css") )
    {         
        fs_open(&file, "/styles.css");         
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
    }
    else if ( IS_HTTP_GET_COMMAND(buf, "loader.js") )
    {    
        fs_open(&file, "/loader.js"); 
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
    }
    else if ( IS_HTTP_GET_COMMAND(buf, "material.js") )
    {          
        fs_open(&file, "/material.js");
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
    }
    else if ( IS_HTTP_GET_COMMAND(buf, "home.js") )
    {    
        fs_open(&file, "/home.js");
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);   
    }
    else if ( IS_HTTP_GET_COMMAND(buf, "battery_data.json") )
    {    
        fs_open(&file, "/battery_data.json");
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);   
    }
    
    else if ( IS_HTTP_POST_COMMAND(buf, "command") )
    {
        if ( strncmp(buf, "\"command\": \"start\"", 18) )
        {
            printf("Start cycle\n");

            result= osMessagePut(main_thread_queue, CMSIS_OS_EVENT_START_BUTTON_CLICKED, osWaitForever);
            DEBUG_ASSERT(result == osOK);
        }
        else
            printf("Button clicked ------command-------\n%s\n", buf);

        netconn_write(conn, HTTP_RESPONSE_OK_200, sizeof(HTTP_RESPONSE_OK_200)-1, NETCONN_NOCOPY);
    }
    else 
    {
#if defined(WITH_DEBUG_LOG)
        DEBUG_LOG("Command not recoqnized");
        printf("---------command----------\n%s\n", buf);
#endif         
        fs_open(&file, "/404.html"); 
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
    }

    end:
  
    // Close connection.

    netconn_close(conn);
  
    // Clear buffer.

    netbuf_delete(inbuf);
}

// http_server_netconn_thread

static void http_server_netconn_thread(void* arg)
{ 
    struct netconn* conn; 
    struct netconn* newconn;

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

        if ( netconn_accept(conn, &newconn) != ERR_OK )
        {
#if defined(WITH_DEBUG_LOG)
            DEBUG_LOG("Error while accepting incoming connection.");
#endif
            continue;
        }
        
        // Handle connection.

        http_server_serve(newconn);

        // Close connection and free its resources.

        netconn_delete(newconn);
    }
    
    end_thread:

    // Terminate this thread.

    osThreadTerminate(osThreadGetId());
}

// http_server_netconn_init

void http_server_netconn_init(void)
{
    // Start new HTTP connection thread.

    sys_thread_new("HTTP", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, osPriorityAboveNormal);
}
