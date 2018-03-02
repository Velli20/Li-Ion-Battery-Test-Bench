
// Includes


#include "string.h"
#include <stdio.h>
#include "httpserver-netconn.h"

#define PAGE_HEADER                                               \
    "HTTP/1.0 200 OK                                              \
     Server: lwIP/pre-0.6 (http://www.sics.se/~adam/lwip/)        \
     Content-type: text/javascript\r\n\r\n"                       \

// Google line chart add row string formatter.

#define GOOGLE_LINE_CHART_ADD_POINT(BUFFER, YEAR, MONTH, DAY_OF_MONTH, HOUR, MINUTE, LEVEL)               \
                                   (sprintf(BUFFER, "data.addRow([new Date(%d, %d, %d, %d, %d), %d]);\n", \
                                    YEAR, MONTH, DAY_OF_MONTH, HOUR, MINUTE, LEVEL))

// JS script start.

#define PAGE_START                                                 \
    "google.charts.load('current', { 'packages': ['corechart'] }); \
     google.charts.setOnLoadCallback(drawChart);                   \
     function drawChart() {                                        \
         var options;                                              \
         var data;                                                 \
         var chart;                                                \
         data= new google.visualization.DataTable();               \
         data.addColumn('date', 'Time');                           \
         data.addColumn('number', 'Battery 1');"

// JS script end.

#define PAGE_END                                                   \
    "options= {                                                    \
        title: '28.1.2018',                                        \
        curveType: 'function',                                     \
        legend: { position: 'bottom' },                            \
        vAxis: { maxValue: 100, title: 'Charge %' },               \
        xAxis: { title: 'Time' },                                  \
        hAxis: {                                                   \
            gridlines: {                                           \
                count: -2,                                         \
                units: {                                           \
                    days: { format: ['MMM dd'] },                  \
                    hours: { format: ['HH:MM'] },                  \
                }                                                  \
            }                                                      \
        }                                                          \
     };                                                            \
     chart= new google.visualization.LineChart(                    \
                          document.getElementById('curve_chart')); \
     chart.draw(data, options);}"

// netconn_write_battery_js

void netconn_write_battery_js(struct netconn* conn)
{
    portCHAR PAGE_BODY[512];
    char     temp_buffer[60];
    uint16_t i;

    memset(PAGE_BODY, 0,512);

    for ( i= 0; i < 10; i++ )
    {
        memset(temp_buffer, 0, 60);
        GOOGLE_LINE_CHART_ADD_POINT(temp_buffer, 2018, 1, 28, 12, i+10, ((i*10) +i));
        strcat(PAGE_BODY, temp_buffer);
    }
    strcat(PAGE_BODY, "\0");

    // Write generated battery js file.
    
    netconn_write(conn, PAGE_HEADER, strlen((char*)PAGE_HEADER), NETCONN_COPY);
    netconn_write(conn, PAGE_START, strlen((char*)PAGE_START), NETCONN_COPY);
    netconn_write(conn, PAGE_BODY, strlen(PAGE_BODY), NETCONN_COPY);
    netconn_write(conn, PAGE_END, strlen((char*)PAGE_END), NETCONN_COPY);
}
