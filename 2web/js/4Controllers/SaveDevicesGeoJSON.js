document.getElementById("SaveDevicesGeoJSON").onclick = function () {
    var devices = map.getDevicesArray()
    
    // Convert the device data to a JSON string
    var json = JSON.stringify(devices);

    // Create a blob object from the JSON string
    var blob = new Blob([json], {type: "application/json"});

    // Create a URL for the blob
    var url = URL.createObjectURL(blob);

    // Create a link element
    var link = document.createElement("a");
    link.href = url;
    link.download = "devices.json";

    // Append the link to the body
    document.body.appendChild(link);

    link.click();

    document.body.removeChild(link);
}