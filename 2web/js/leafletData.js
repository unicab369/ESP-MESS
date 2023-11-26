function generateNearbyCoordinates(main_coordinate, radiusInMiles, numberofDevices) {
    
    // Convert latitude and longitude to radians
    let lat = main_coordinate[0]*(Math.PI/180);
    let lon = main_coordinate[1]*(Math.PI/180);

    
    let devices_coordinates = [];

    for (let i = 0; i < numberofDevices; i++) {
        // Convert radius from miles to kilometers
        let radiusInKm = radiusInMiles * 1.60934;
        // Convert radius to radians
        let radiusInRadians = radiusInKm / 6371;

        // Generate a random bearing (direction)
        let bearing = Math.random() * 2 * Math.PI;

        // Calculate new devices_coordinates
        let newLat = Math.asin(Math.sin(lat) * Math.cos(radiusInRadians) + Math.cos(lat) * Math.sin(radiusInRadians) * Math.cos(bearing));
        let newLon = lon + Math.atan2(Math.sin(bearing) * Math.sin(radiusInRadians) * Math.cos(lat), Math.cos(radiusInRadians) - Math.sin(lat) * Math.sin(newLat));

        // Convert devices_coordinates from radians to degrees
        newLat = newLat * (180 / Math.PI);
        newLon = newLon * (180 / Math.PI);

        devices_coordinates.push([newLat, newLon]);
    }
    return devices_coordinates;
}

class LeafletMapObject {
    constructor(options) {
        this.id = options.id;
        this.main_coordinate = options.main_coordinate;
        this.devices_coordinates = options.devices_coordinates;
        this.zoom = options.zoom;
        this.urlTemplate = 'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png';
        this.map = L.map(this.id, options.mapOptions).setView(this.main_coordinate, this.zoom);
        this.baseLayer = L.tileLayer.offline(this.urlTemplate,{
            attribution: 'Map data {attribution.OpenStreetMap}',
            subdomains: 'abc',
            minZoom: this.zoom,
            maxZoom: this.zoom,
        }).addTo(this.map);
    }

    showMap(){
        //Create Map with baseLayer.
        this.baseLayer;
        this.addMarkers(this.devices_coordinates);
        this.addMapControl();
        //Zoom Function for this.map
        this.map.on('zoomend', () =>{
            this.map.setView(this.main_coordinate, this.map.getZoom());
        })
    }


    addMarkers() {
        //Label device Locations with each coordinate
        this.devices_coordinates.forEach((coordinate, index) => {
            L.marker(coordinate).addTo(this.map)
                .bindPopup(`Device ID${index + 1}`)
                .openPopup();
        });
    }

    addMapControl(){
        // Add map control buttons to save tiles in area viewed
        const baseLayer = this.baseLayer;
        const control = L.control.savetiles(baseLayer, {
            zoomlevels: [this.zoom], // optional zoomlevels to save, default current zoomlevel
            confirm(layer, successCallback) {
                // eslint-disable-next-line no-alert
                if (window.confirm(`Save ${layer._tilesforSave.length}`)) {
                    successCallback();
                }
            },
            confirmRemoval(layer, successCallback) {
                // eslint-disable-next-line no-alert
                if (window.confirm('Remove all the tiles?')) {
                    successCallback();
                }
            },
            saveText:
                '<i class="download-icon" aria-hidden="true" title="Save tiles"></i>',
            rmText: '<i class="trash-icon" aria-hidden="true"  title="Remove tiles"></i>',
        })
        control.addTo(this.map);
    }  
}

