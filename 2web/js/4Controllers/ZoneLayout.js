class ZoneItem {
   constructor(startLat, startLon, translationalMap, fillColor) {
      this.startLat = startLat
      this.startLon = startLon
      this.translationalMap = translationalMap
      this.fillColor = fillColor
   }

   calculateNewCoordinates(oldLat, oldLon, xDisplacement, yDisplacement) {
      // Constants for rough estimation (for illustrative purposes)
      // let meterTranslation = 111132.0  // meters (approximate at equator)
      // let newLat = oldLat + (yDisplacement / meterTranslation)     // Calculate new latitude
      // let newLon = oldLon + (xDisplacement / meterTranslation)    // Calculate new longitude
      // return [newLat, newLon]

      return [oldLat+xDisplacement, oldLon+yDisplacement]
   }

   makeZone() {
      let ref = [this.startLat, this.startLon]
      var output = [ref]

      this.translationalMap.forEach(e => {
         let newCoords = this.calculateNewCoordinates(ref[0], ref[1], e[0], e[1])
         ref = newCoords
         output.push(ref)
      })

      return output
   }
}

class ZoneLayout {
   constructor() {
      this.leafletMap = new LeafletMapObject();
   }

   setup() {
      this.leafletMap.showMap()
      this.leafletMap.generateCoordinates()
      // this.leafletMap.addDots()
      this.leafletMap.addMarker([41.114233553848415, -85.14135853514709], "startPoint")
   }

   addZones() {
      let zones = [
         new ZoneItem(41.11423, -85.14137, [
            [-.00071, .00058],
            [.00027, .000599],
            [.00071, -.00060]
         ], '#CA6F1E'),
         new ZoneItem(41.11451, -85.14074, [
            [-.000677, .00054], [.000479, .000993],
            [.00043, -.0000], [.000, -.00099],
         ], '#27AE60'),
         new ZoneItem(41.11418, -85.13943, [
            [-.00066, .00051], [.00, .00073],
            [.00099, .000], [.000, -.00099],
            [-.00019, .000],
         ], '#D0D3D4'),
         new ZoneItem(41.11474, -85.14029, [
            [.00098, .00],
            [.00, .0011],
            [-.00098, .000],
         ], '#F1C40F'),
         new ZoneItem(41.11573, -85.13920, [
            [.000, .00164],
            [-.00119, .00],
            [.0000, -.00160],
         ], '#34495E'),
         new ZoneItem(41.11555, -85.13755, [
            [.000, .00087],
            [.00103, .00],
            [.000, -.00085],
         ], '#E59866'),
         new ZoneItem(41.11575, -85.13974, [
            [.00090, .000], [.00, .00037],
            [-.00044, .00], [.000, .00047],
            [.00068, .000], [.000, .00135],
            [-.00115, .00]
         ], '#DFFF00'),
         new ZoneItem(41.11690, -85.13939, [
            [-.00068, .000], 
            [.000, .00045],
            [.00068, .000], 
         ], '#DE3163'),
         new ZoneItem(41.11626, -85.13977, [
            [.000, -.0020],
            [-.0005, .00], 
            [.000, .0020], 
         ], '#B048C9'),
      ]

      zones.forEach((z) => {
         this.leafletMap.addPoly(z.makeZone(), z.fillColor)
      })
   }
}