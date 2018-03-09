
// Includes

#include <string.h>
#include <stdio.h>
#include "server.h"
#include "main.h"

// JSON HTTP response header

#define PAGE_HEADER_JSON                     \
    "HTTP/1.0 200 OK"                        \
    "Server: lwIP/pre-0.6"                   \
    "Content-type: application/json\r\n\r\n" \

#define TEST_COUNT 10

// server_write_bms_json_data

uint8_t server_write_file_list_json_data(struct netconn* conn)
{
    char buffer[64];
    
    if ( !conn )
        return (SERVER_RESULT_ERROR);
        
    snprintf(buffer, sizeof(buffer), "{ \"file_count\": [\"%d\"] }", TEST_COUNT);
    
    // TODO: Read files on sd card and return file list in json format.
    
    netconn_write(conn, PAGE_HEADER_JSON, strlen((char*)PAGE_HEADER_JSON), NETCONN_NOCOPY);
    netconn_write(conn, buffer,           strlen((char*)buffer), NETCONN_COPY);

    return (SERVER_RESULT_OK);
}
