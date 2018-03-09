#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#include "lwip/api.h"

#define SERVER_RESULT_OK                    ((uint8_t)0x01)
#define SERVER_RESULT_ERROR                 ((uint8_t)0x00)
#define SERVER_RESULT_KEEP_CONNECTION_ALIVE ((uint8_t)0x03)

#define HTTP_RESPONSE_HEADER_TEXT_FILE    \
    "HTTP/1.1 200 OK\r\n"                 \
    "Access-Control-Allow-Origin: *\r\n"  \
    "Content-Type: application/text\r\n"  \
    "\r\n\r\n"

// BMS_DATA struct forward declaration

typedef struct bms_data BMS_DATA;

// http_server_netconn_init

uint8_t http_server_netconn_init(struct netif* gnetif, 
                                 BMS_DATA*     data);

// http_server_send_event_to_client

uint8_t http_server_send_event_to_client(uint8_t event);

// server_write_bms_json_data

uint8_t server_write_bms_json_data(struct netconn* conn, 
                                   BMS_DATA*       bms_data);

// server_write_cycle_result_text_file

uint8_t server_write_cycle_result_text_file(struct netconn* conn,
                                            uint32_t        cycle);

// server_write_file_list_json_data

uint8_t server_write_file_list_json_data(struct netconn* conn);

#endif
