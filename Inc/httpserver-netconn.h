#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#include "lwip/api.h"

// http_server_netconn_init

void http_server_netconn_init(void);

// netconn_write_battery_js

void netconn_write_battery_js(struct netconn* conn);

#endif
