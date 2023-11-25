
class ModPlot {
   constructor(title, recordOptions) {
      let plotOpt1 = new uPlotOption(title, recordOptions, 1200, 300)
      this.plot = new uPlot(plotOpt1.getOptions(), [[],[],[]], document.body)
      this.dataGen = new DataGenerator()
   }

   async plotData() {
      let mockData = await this.dataGen.requestMockData()
      if (mockData.length > 0) this.plot.setData(mockData)
   }
}

let plot1 = new ModPlot('TEMPERATURE (°C)', [
   new RecordOption('Dev_A', 'FF4500'),
   new RecordOption('Dev_B', 'FFA500')
])

let plot2 = new ModPlot('HUMIDITY (%H)', [
   new RecordOption('Dev_C', '008B8B'),
   new RecordOption('Dev_D', 'BA55D3')
])

let plot3 = new ModPlot('AMBIENT LIGHT (LUX)', [
   new RecordOption('Dev_E', 'A9A9A9'),
   new RecordOption('Dev_F', '000000')
])


function updatePlots() {
   plot1.plotData()
   plot2.plotData()
   plot3.plotData()
}

// Update data every x ms
updatePlots()
let interval = setInterval(()=> {
   updatePlots()
}, 200)

// Clear the interval after 10 minutes to prevent memory leaks and unecessary operations.
setTimeout(()=>{
   clearInterval(interval)
}, 600000)
