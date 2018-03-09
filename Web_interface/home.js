function onLoadEvent() {
    var source = new EventSource('event_callback');

    source.onmessage = function(event) {

        // Read bms data.

        read_bms_data();

        // Update task list.

        load_rtos_tasks();
    };

    source.onerror = function(error) {
        console.log(error);
    };

    read_bms_data();

    load_file_list();

    load_rtos_tasks();
}

function load_file_list() {
    get_json_data_file("./test_results.json", function(text) {
        var json_data = JSON.parse(text);

        var i;

        for (i = 0; i < json_data.file_count; i++) {
            var container = document.createElement('div');
            container.className = "file_list_item_container";

            var link = document.createElement('a');
            link.textContent = "CYCLE_" + i + ".txt";
            link.href = "CYCLE_" + i + ".txt"
            link.className = "file_list_text";

            container.appendChild(link);

            document.getElementById("file_list").appendChild(container);
        }
    });
}

function load_rtos_tasks() {
    get_json_data_file("./rtos_tasks.text", function(text) {
        document.getElementById("rtos_status").textContent = "Name    State  Priority  Stack   Task number\n\n" + text;
    });
}

function get_json_data_file(file, callback) {
    var rawFile = new XMLHttpRequest();

    rawFile.overrideMimeType("application/json");
    rawFile.open("GET", file, true);

    rawFile.onreadystatechange = function() {
        if (rawFile.readyState === 4 && rawFile.status == "200") {
            callback(rawFile.responseText);
        }
    }
    rawFile.send(null);
}

function read_bms_data() {
    get_json_data_file("./bms_data.json", function(text) {
        var json_data = JSON.parse(text);

        // Set cell voltage text.

        document.getElementById("cell_1_voltage").innerText = "Cell 1: " + json_data.cell_voltage[0] + " V";
        document.getElementById("cell_2_voltage").innerText = "Cell 2: " + json_data.cell_voltage[1] + " V";
        document.getElementById("cell_3_voltage").innerText = "Cell 3: " + json_data.cell_voltage[2] + " V";
        document.getElementById("cell_4_voltage").innerText = "Cell 4: " + json_data.cell_voltage[3] + " V";

        // Read bms state and set visibility of the start and stop buttons.

        var bms_state = json_data.state;

        document.getElementById("start_button").style.display = bms_state < 2 ? "inline" : "none";
        document.getElementById("stop_button").style.display = bms_state > 1 ? "inline" : "none";

        var simulation_on = json_data.bms_simulate;

        document.getElementById("bms_simulate_toggle").checked = (simulation_on == 1 ? true : false);
    });
}

function bms_start_cycle_test() {
    var xhr = new XMLHttpRequest();

    xhr.open("POST", "/command/", true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.send(JSON.stringify({
        command: "start"
    }));
}

function bms_stop_cycle_test() {
    var xhr = new XMLHttpRequest();

    xhr.open("POST", "/command/", true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.send(JSON.stringify({
        command: "stop"
    }));
}

function bms_simulate_toggle_event() {
    var simulation_toggle = document.getElementById("bms_simulate_toggle");
    var xhr = new XMLHttpRequest();

    xhr.open("POST", "/command/", true);
    xhr.setRequestHeader('Content-Type', 'application/json');

    if (simulation_toggle.checked == true) {
        xhr.send(JSON.stringify({
            simulation: "enabled"
        }));
    } else {
        xhr.send(JSON.stringify({
            simulation: "disabled"
        }));
    }
}