//! saveGeoJsonButton
document.getElementById('saveGeoJsonButton').addEventListener('click', function() {
   let geojsonData = {
      type: "FeatureCollection",
      features: []
   }

   map.drawnItems.eachLayer(function(layer) {
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


//! loadZoneLabel
// Add event listener to button to trigger file input click event
document.getElementById('loadZoneLabel').addEventListener('click', function() {
   document.getElementById('loadZoneButton').click();
});


document.getElementById('loadZoneButton').addEventListener('change', function(e) {
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
         map.drawnItems.addLayer(layer);
      
      })
   };
   reader.readAsText(file);
});


//! SaveDevicesGeoJSON
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

//! loadDevicesButton
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