document.getElementById('saveGeoJsonButton').addEventListener('click', function() {
    let geojsonData = {
        type: "FeatureCollection",
        features: []
    }

    zoneLayout.leafletMap.drawnItems.eachLayer(function(layer) {
        //Can onlly access the layer's properties if it is a GeoJSON layer via leafletMap Object
        let feature = layer.toGeoJSON();
        feature.properties.color = layer.options.color;
        geojsonData.features.push(feature);
    });

    //Create a download link for the geojson file
    let dataStr = 'data:text/json;charset=utf-8,' + encodeURIComponent(JSON.stringify(geojsonData));
    let downloadAnchorNode = document.createElement('a');
    downloadAnchorNode.setAttribute("href", dataStr);
    downloadAnchorNode.setAttribute("download", "drawnItems.json");
    document.body.appendChild(downloadAnchorNode); // required for firefox
    downloadAnchorNode.click();
    downloadAnchorNode.remove();
})