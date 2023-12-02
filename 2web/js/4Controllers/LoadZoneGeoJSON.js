// Add event listener to button to trigger file input click event
document.getElementById('loadGeoJsonButtonLabel').addEventListener('click', function() {
    document.getElementById('loadGeoJsonButton').click();
});


document.getElementById('loadGeoJsonButton').addEventListener('change', function(e) {
    //Get the file from the input
    let file = e.target.files[0];
    if (!file) {
        return;
    }

    // Update the span text
    document.getElementById('selectedFileName').textContent = file.name;

    //Read the file and add the geojson layer to the map
    let reader = new FileReader();
    reader.onload = function(e) {
        let contents = e.target.result;
        let geoJsonData = JSON.parse(contents);
        let geoJsonLayer = L.geoJSON(geoJsonData, {
            style: function(feature) {
                return {color: feature.properties.color}
            }
        });

        geoJsonLayer.eachLayer(function(layer){
            zoneLayout.leafletMap.drawnItems.addLayer(layer);
        
        })
    };
    reader.readAsText(file);
});