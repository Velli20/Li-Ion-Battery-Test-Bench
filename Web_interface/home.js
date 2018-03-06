function onLoadEvent() {
    var source = new EventSource('event_callback');

    source.onmessage = function(event) {

        // Read bms data.

        read_bms_data();
    };

    source.onerror = function(error) {
        console.log(error);
    };

    read_bms_data();
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

        document.getElementById("start_button").style.visibility = bms_state < 2 ? "visible" : "hidden";
        document.getElementById("stop_button").style.visibility = bms_state > 1 ? "visible" : "hidden";

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