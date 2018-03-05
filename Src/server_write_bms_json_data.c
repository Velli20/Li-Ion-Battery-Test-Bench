
// Includes

#include <string.h>
#include <stdio.h>
#include "server.h"
#include "main.h"

// Response header

#define PAGE_HEADER                          \
    "HTTP/1.0 200 OK"                        \
    "Server: lwIP/pre-0.6"                   \
    "Content-type: application/json\r\n\r\n" \

// server_write_bms_json_data

uint8_t server_write_bms_json_data(struct netconn* conn, 
                                   BMS_DATA*       bms_data)
{
    char buffer[256];

    if ( !conn || !bms_data )
        return (SERVER_RESULT_ERROR);

    snprintf(buffer, sizeof(buffer), "{ \"cell_voltage\": [\"%.2f\", \"%.2f\", \"%.2f\", \"%.2f\"], \"cycle_count\": [\"%lu\"], \"state\": [\"%d\"], \"bms_simulate\": [\"%d\"] }",
             bms_data->cell_voltage[0],
             bms_data->cell_voltage[1],
             bms_data->cell_voltage[2],
             bms_data->cell_voltage[3],
             bms_data->bms_cycle,
             bms_data->bms_state,
             bms_data->bms_simulate_data);

    netconn_write(conn, PAGE_HEADER, strlen((char*)PAGE_HEADER), NETCONN_NOCOPY);
    netconn_write(conn, buffer,      strlen((char*)buffer), NETCONN_COPY);

    return (SERVER_RESULT_OK);
}
