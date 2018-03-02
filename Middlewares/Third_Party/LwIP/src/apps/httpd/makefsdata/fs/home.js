var jsonData;

function readTextFile(file, callback) {
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

readTextFile("./battery_data.json", function(text) {
    var i, x = "";

    jsonData = JSON.parse(text);
    console.log(jsonData);


    for (i in jsonData.data) {
        x += jsonData.data[i] + "<br>";
    }

    document.getElementById("demo").innerHTML = x;

});

function bms_start_cycle_test() {

    var xhr = new XMLHttpRequest();

    xhr.open("POST", "/command/", true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.send(JSON.stringify({
        command: "start"
    }));

}