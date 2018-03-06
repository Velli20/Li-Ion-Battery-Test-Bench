#ifndef __WEB_INTERFACE_HOME_JS_H
#define __WEB_INTERFACE_HOME_JS_H

#if defined(__ICCARM__)
#pragma location = \
    .qspi static const char home_js[] = {
#else
__attribute__((section(".qspi"))) static const char home_js[]= {
#endif

"HTTP/1.0 200 OK\r\n"
"Server: lwIP/pre-0.6\r\n"
"Content-type: application/javascript\r\n\r\n"

"function onLoadEvent() {\n"
"    var source = new EventSource('event_callback');\n"
"\n"
"    source.onmessage = function(event) {\n"
"\n"
"        // Read bms data.\n"
"\n"
"        read_bms_data();\n"
"    };\n"
"\n"
"    source.onerror = function(error) {\n"
"        console.log(error);\n"
"    };\n"
"\n"
"    read_bms_data();\n"
"}\n"
"\n"
"function get_json_data_file(file, callback) {\n"
"    var rawFile = new XMLHttpRequest();\n"
"\n"
"    rawFile.overrideMimeType(\"application/json\");\n"
"    rawFile.open(\"GET\", file, true);\n"
"\n"
"    rawFile.onreadystatechange = function() {\n"
"        if (rawFile.readyState === 4 && rawFile.status == \"200\") {\n"
"            callback(rawFile.responseText);\n"
"        }\n"
"    }\n"
"    rawFile.send(null);\n"
"}\n"
"\n"
"function read_bms_data() {\n"
"    get_json_data_file(\"./bms_data.json\", function(text) {\n"
"        var json_data = JSON.parse(text);\n"
"\n"
"        // Set cell voltage text.\n"
"\n"
"        document.getElementById(\"cell_1_voltage\").innerText = \"Cell 1: \" + json_data.cell_voltage[0] + \" V\";\n"
"        document.getElementById(\"cell_2_voltage\").innerText = \"Cell 2: \" + json_data.cell_voltage[1] + \" V\";\n"
"        document.getElementById(\"cell_3_voltage\").innerText = \"Cell 3: \" + json_data.cell_voltage[2] + \" V\";\n"
"        document.getElementById(\"cell_4_voltage\").innerText = \"Cell 4: \" + json_data.cell_voltage[3] + \" V\";\n"
"\n"
"        // Read bms state and set visibility of the start and stop buttons.\n"
"\n"
"        var bms_state = json_data.state;\n"
"\n"
"        document.getElementById(\"start_button\").style.visibility = bms_state < 2 ? \"visible\" : \"hidden\";\n"
"        document.getElementById(\"stop_button\").style.visibility = bms_state > 1 ? \"visible\" : \"hidden\";\n"
"\n"
"        var simulation_on = json_data.bms_simulate;\n"
"\n"
"        document.getElementById(\"bms_simulate_toggle\").checked = (simulation_on == 1 ? true : false);\n"
"    });\n"
"}\n"
"\n"
"function bms_start_cycle_test() {\n"
"    var xhr = new XMLHttpRequest();\n"
"\n"
"    xhr.open(\"POST\", \"/command/\", true);\n"
"    xhr.setRequestHeader('Content-Type', 'application/json');\n"
"    xhr.send(JSON.stringify({\n"
"        command: \"start\"\n"
"    }));\n"
"}\n"
"\n"
"function bms_stop_cycle_test() {\n"
"    var xhr = new XMLHttpRequest();\n"
"\n"
"    xhr.open(\"POST\", \"/command/\", true);\n"
"    xhr.setRequestHeader('Content-Type', 'application/json');\n"
"    xhr.send(JSON.stringify({\n"
"        command: \"stop\"\n"
"    }));\n"
"}\n"
"\n"
"function bms_simulate_toggle_event() {\n"
"    var simulation_toggle = document.getElementById(\"bms_simulate_toggle\");\n"
"    var xhr = new XMLHttpRequest();\n"
"\n"
"    xhr.open(\"POST\", \"/command/\", true);\n"
"    xhr.setRequestHeader('Content-Type', 'application/json');\n"
"\n"
"    if (simulation_toggle.checked == true) {\n"
"        xhr.send(JSON.stringify({\n"
"            simulation: \"enabled\"\n"
"        }));\n"
"    } else {\n"
"        xhr.send(JSON.stringify({\n"
"            simulation: \"disabled\"\n"
"        }));\n"
"    }\n"
"}\n"
};

#endif