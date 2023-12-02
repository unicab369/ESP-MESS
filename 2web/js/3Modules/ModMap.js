function generateNearbyCoordinates(center, radiusInMiles, numberofDevices) {
    // Convert latitude and longitude to radians
    let lat = center[0]*(Math.PI/180);
    let lon = center[1]*(Math.PI/180);
    let output = [];

    for (let i = 0; i < numberofDevices; i++) {
        let radiusInKm = radiusInMiles * 1.60934;       // Convert radius from miles to kilometers
        let radiusInRadians = radiusInKm / 6371;        // Convert radius to radians
        let bearing = Math.random() * 2 * Math.PI;      // Generate a random bearing (direction)

        //! Calculate new coordinates
        let newLat = Math.asin(Math.sin(lat) * Math.cos(radiusInRadians) + Math.cos(lat) * Math.sin(radiusInRadians) * Math.cos(bearing));
        let newLon = lon + Math.atan2(Math.sin(bearing) * Math.sin(radiusInRadians) * Math.cos(lat), Math.cos(radiusInRadians) - Math.sin(lat) * Math.sin(newLat));

        //! Convert coordinates from radians to degrees
        newLat = newLat * (180 / Math.PI);
        newLon = newLon * (180 / Math.PI);

        // Create a device object
        let device = {
            device_name: "Device " + (i+1),
            coordinate: [newLat, newLon]
        }
        output.push(device);
    }
    return output;
}


// map API https://leafletjs.com/reference.html#map-example
class LeafletMapObject {
    // let main_coordinate = [37.78425807817066, -122.40636030396368]; // San Francisco

    constructor(center = [41.11536086134376, -85.13907697741708], zoom = 17) {
        let mapOptions = {
            zoomControl: true, 
            dragging: true, 
            keyboard: false, 
        }

        this.center = center
        this.map = L.map('map', mapOptions).setView(center, zoom)
        // Monitor devices on map
        this.devices= new Map(); 
    }

    setup() {
        this.showMap()
        this.addDrawingTool()
        // this.drawnItems = new L.LayerGroup().addTo(this.map)/
     }

    showMap(){
        this.urlTemplate = 'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png';
        this.baseLayer = L.tileLayer.offline(this.urlTemplate,{
            attribution: 'Map data {attribution.OpenStreetMap}',
            subdomains: 'abc',
            // minZoom: this.zoom,
            // maxZoom: this.zoom,
        }).addTo(this.map)

        this.addMapControl()
        
        // Adding coordinate control on map - need Leaflet.Coordinates-0.1.5.min.js
		L.control.coordinates().addTo(this.map);

        //Zoom Function for this.map
        this.map.on('zoomend', () =>{
            this.map.setView(this.main_coordinate, this.map.getZoom())
        })
    }

    addMarker(coordinate, label) {
        return L.marker(coordinate).addTo(this.map).bindPopup(label).openPopup()
    }

    removeDevice(deviceName) {
        // Check if the device exists in the map
        if (this.devices.has(deviceName)) {
            // Get the marker for the device
            const marker = this.devices.get(deviceName);

            // Remove the marker from the map
            this.map.removeLayer(marker);

            // Remove the device from the devices map
            this.devices.delete(deviceName);
        }
    }

    addDevices(device_info) {

        // Define a custom icon for the markers
        var dotIcon = L.divIcon({
            className: 'dot-icon',
            iconSize: [8, 8],  // Adjust the size of the dot
            iconAnchor: [4, 4], // Center the dot
        })

        device_info.forEach((info, index) => {
            // Create a marker for the device
            var marker = L.marker(info.coordinate, {icon: dotIcon}).addTo(this.map).bindPopup(info.device_name).openPopup()

            //Track added devices on map
            this.devices.set(info.device_name, marker)
        });

        
    }

    generateCoordinates() {
        return generateNearbyCoordinates(this.center, 0.05, 5);
    }

    addPoly(coordinates, fillColor = '#8E44AD', color = 'green') {
        var poly1 = L.polygon(coordinates, {
            color: color,
            fillColor: fillColor,
            fillOpacity: 0.7,
        }).addTo(this.map)
    }

    addCircle() {
        var circle = L.circle(this.center, {
            color: 'red',
            fillColor: '#16A085',
            fillOpacity: 0.5,
            radius: 500
        }).addTo(this.map)
    }

    addMapControl(){
        // Add map control buttons to save tiles in area viewed
        const control = L.control.savetiles(this.baseLayer, {
            zoomlevels: [this.zoom], // optional zoomlevels to save, default current zoomlevel
            confirm(layer, successCallback) {
                // eslint-disable-next-line no-alert
                if (window.confirm(`Save ${layer._tilesforSave.length}`)) {
                    successCallback()
                }
            },
            confirmRemoval(layer, successCallback) {
                // eslint-disable-next-line no-alert
                if (window.confirm('Remove all the tiles?')) {
                    successCallback()
                }
            },
            saveText:
                '<i class="download-icon" aria-hidden="true" title="Save tiles"></i>',
            rmText: '<i class="trash-icon" aria-hidden="true"  title="Remove tiles"></i>',
        })
        control.addTo(this.map)
    }  

    addDrawingTool(){
        // Add drawing tool
        this.drawnItems = new L.FeatureGroup().addTo(this.map);

        // Initialize the draw control and pass it the FeatureGroup of editable layers
        let drawControl = new L.Control.Draw({
            draw: {
                polyline: false,
                polygon: {
                    shapeOptions: {
                        color: '#1ABC9C',
                        fillOpacity: 0.2
                    }
                },
                rectangle: {
                    shapeOptions: {
                        color: '#1ABC9C',
                        fillOpacity: 0.2
                    }
                },
                circle: false,
                circlemarker: false,
                marker: false
            },
            edit: {
                featureGroup: this.drawnItems
            }
        })

        // Add drawing tool to map
        this.map.addControl(drawControl);

        // Add event listener to save drawn items to geoJSON
        this.map.on(L.Draw.Event.CREATED, (e) => {
            let type = e.layerType,
                layer = e.layer;

            if (type === 'polygon') {
                let color = prompt("Enter the color for the shape:");
                if (color) {
                    layer.setStyle({
                        color: color
                    });
                }
            }

            this.drawnItems.addLayer(layer);
        })
    }

    getDrawnItemsGeoJSON() {
        // Return the geoJSON of the drawn items
        return this.drawnItems.toGeoJSON();
    }

    getDevicesArray(){
        return Array.from(this.devices, ([device_name, coordinate]) => ({device_name, coordinate: coordinate._latlng}));
    }
}

