// Add event listener to button to trigger file input click event
document.getElementById('loadDevicesButtonLabel').addEventListener('click', function() {
    document.getElementById('loadDevicesButton').click();
});

document.getElementById('loadDevicesButton').addEventListener('change', function(e){

    let file = e.target.files[0];
    console.log(file)
    if (!file){
        return;
    }

    // Update the span text
    document.getElementById('selectedDevicesFile').textContent = file.name;

    // Create a new FileReader
    var reader = new FileReader();

    // Set the onLoad function

    reader.onload = function(e) {
        // Parse the file contents as JSON
        var devices = JSON.parse(e.target.result);

        //add the devices to the map
        map.addDevices(devices);
    }

    // read the file as text

    reader.readAsText(file);
})