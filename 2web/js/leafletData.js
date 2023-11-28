function generateNearbyCoordinates(center, radiusInMiles, numberofDevices) {
    // Convert latitude and longitude to radians
    let lat = center[0]*(Math.PI/180);
    let lon = center[1]*(Math.PI/180);
    let devices_coordinates = [];

    for (let i = 0; i < numberofDevices; i++) {
        let radiusInKm = radiusInMiles * 1.60934;       // Convert radius from miles to kilometers
        let radiusInRadians = radiusInKm / 6371;        // Convert radius to radians
        let bearing = Math.random() * 2 * Math.PI;      // Generate a random bearing (direction)

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


// map API https://leafletjs.com/reference.html#map-example
class LeafletMapObject {
    // let main_coordinate = [37.78425807817066, -122.40636030396368]; // San Francisco

    constructor(id = 'map', center = [41.116220, -85.140657], zoom = 14) {
        let mapOptions = {
            zoomControl: true, 
            dragging: true, 
            keyboard: false, 
        };

        this.center = center
        this.map = L.map(id, mapOptions).setView(center, zoom);
    }

    showMap(){
        this.urlTemplate = 'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png';
        this.baseLayer = L.tileLayer.offline(this.urlTemplate,{
            attribution: 'Map data {attribution.OpenStreetMap}',
            subdomains: 'abc',
            // minZoom: this.zoom,
            // maxZoom: this.zoom,
        }).addTo(this.map)

        this.addMapControl();
        
        //Zoom Function for this.map
        this.map.on('zoomend', () =>{
            this.map.setView(this.main_coordinate, this.map.getZoom());
        })
    }


    addMarkers() {
        this.devices_coordinates = generateNearbyCoordinates(this.center, 0.05, 5);

        //Label device Locations with each coordinate
        this.devices_coordinates.forEach((coordinate, index) => {
            L.marker(coordinate).addTo(this.map)
                .bindPopup(`Device ID${index + 1}`)
                .openPopup();
        });
    }

    addPolygons() {
        var poly1 = L.polygon([
            [41.121143, -85.144377],
            [41.120426, -85.133347],
            [41.111572, -85.141099]
        ], {
            color: 'green',
            fillColor: '#f03',
            fillOpacity: 0.5,
        }).addTo(this.map);
    }

    addCircle() {
        var circle = L.circle(this.center, {
            color: 'red',
            fillColor: '#16A085',
            fillOpacity: 0.5,
            radius: 500
        }).addTo(this.map);
    }

    addMapControl(){
        // Add map control buttons to save tiles in area viewed
        const control = L.control.savetiles(this.baseLayer, {
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

