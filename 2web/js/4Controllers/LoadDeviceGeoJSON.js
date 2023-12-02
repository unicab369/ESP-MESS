document.addEventListener('DOMContentLoaded', function() {
    // Initialize the map
    // var map = L.map('map').setView([51.505, -0.09], 13);

    // Define your GeoJSON data
    var geoJsonData = {
        "type": "FeatureCollection",
        "features": [
            {
                "type": "Feature",
                "properties": {
                    "name": "Device 1"
                },
                "geometry": {
                    "type": "Point",
                    "coordinates": [41.114678956077924, -85.13939852294905]
                }
            },
            {
                "type": "Feature",
                "properties": {
                    "name": "Device 2"
                },
                "geometry": {
                    "type": "Point",
                    "coordinates": [41.115416593025465, -85.1400346632466]
                }
            },
            // Add more devices as needed...
        ]
    };

    // Add the GeoJSON layer to the map
    // let geoJsonLayer = L.geoJSON(geoJsonData, {
    //     style: function(feature) {
    //         return {color: feature.properties.color}
    //     }
    // });

    // geoJsonLayer.eachLayer(function(layer){
    //     zoneLayout.leafletMap.drawnItems.addLayer(layer);
    
    // })

    let geoJsonLayer = L.geoJSON(geoJsonData, {
        pointToLayer: function (feature, latlng) {
            return L.circleMarker(latlng, {
                radius:50,}) //Increase or decrease the radius of point
        },
        onEachFeature: function (feature, layer) {
            // Bind a popup to each feature
            layer.bindPopup(feature.properties.name);
        }
    });

    geoJsonLayer.eachLayer(function(layer){
        zoneLayout.leafletMap.drawnItems.addLayer(layer);
    
    })
});