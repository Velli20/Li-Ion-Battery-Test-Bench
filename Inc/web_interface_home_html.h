
#ifndef __WEB_INTERFACE_HOME_HTML_H
#define __WEB_INTERFACE_HOME_HTML_H

#if defined(__ICCARM__)
#pragma location = \
    .qspi static const char home_html[] = {
#else
__attribute__((section(".qspi"))) static const char home_html[]= {
#endif

"HTTP/1.0 200 OK\r\n"
"Server: lwIP/pre-0.6\r\n"
"Content-type: text/html\r\n\r\n"

"<!doctype html>\n"
"\n"
"<html lang=\"en\">\n"
"\n"
"<head>\n"
"    <meta charset=\"utf-8\">\n"
"    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, minimum-scale=1.0\">\n"
"    <title>Home</title>\n"
"    <link rel=\"stylesheet\" href=\"./styles.css\" type=\"text/css\"></link>\n"
"    <script type=\"text/javascript\" src=\"home.js\"></script>\n"
"</head>\n"
"\n"
"<body onload=\"onLoadEvent()\" class=\"body\">\n"
"\n"
"    <div class=\"card\">\n"
"        <div class=\"container\">\n"
"            <h2 class=\"sub_title\">CONTROLS</h2>\n"
"            <button id=\"start_button\" class=\"button button_start\" onclick=\"bms_start_cycle_test()\">START</button>\n"
"            <button id=\"stop_button\" class=\"button button_stop\" onclick=\"bms_stop_cycle_test()\">STOP</button>\n"
"            <div>\n"
"                <label class=\"switch\">\n"
"                    <input id=\"bms_simulate_toggle\" type=\"checkbox\" onclick=\"bms_simulate_toggle_event()\">\n"
"                    <span class=\"slider round\"></span>\n"
"                </label>\n"
"                <h2 class=\"switch_title\">SIMULATE DATA</h2>\n"
"            </div>\n"
"        </div>\n"
"    </div>\n"
"    <div class=\"card\">\n"
"        <div class=\"container\">\n"
"            <h2 class=\"sub_title\">CELL VOLTAGE</h2>\n"
"            <h2 class=\"voltage_text\" id='cell_1_voltage'>Cell 1:</h2>\n"
"            <h2 class=\"voltage_text\" id='cell_2_voltage'>Cell 2:</h2>\n"
"            <h2 class=\"voltage_text\" id='cell_3_voltage'>Cell 3:</h2>\n"
"            <h2 class=\"voltage_text\" id='cell_4_voltage'>Cell 4:</h2>\n"
"        </div>\n"
"    </div>\n"
"    <div class=\"card\">\n"
"        <div class=\"container\">\n"
"            <h2 class=\"sub_title\">RTOS tasks</h2>\n"
"            <div id=\"rtos_status\" class=\"system_info_container\">\n"
"            </div>\n"
"        </div>\n"
"    </div>\n"
"    <div class=\"card\">\n"
"        <div id=\"file_list\" class=\"container \">\n"
"            <h2 class=\"sub_title\">TEST RESULTS</h2>\n"
"        </div>\n"
"    </div>\n"
"</body>\n"
"\n"
"</html>\n"
};

#endif
