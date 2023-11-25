import { uPlotOption } from "../1DataModel/UPlotOption.js"
import { DataGenerator } from "../2Services/DataGenerator.js"

export class ModPlot {
   constructor() {
      this.dataGen = new DataGenerator()

      let uPlotOPtion = new uPlotOption('LUX', 'lux', 'red', 1300, 600);
      let opts = uPlotOPtion.getOptions();
      this.plot = new uPlot(opts, [[],[],[]], document.body);
   }

   async plotData() {
      let data2 = await dataGen.requestMockData();
      if (data2.length > 0) this.plot.setData(data2);
   }

   makePlot() {

      // let u3 = new uPlot(opts, [[],[],[]], document.body);
      // plotData()
      
      // Update data every 1 minute
      let interval = setInterval(plotData, 200);

      // Clear the interval after 10 minutes to prevent memory leaks and unecessary operations.
      setTimeout(() =>{
         clearInterval(interval)
      }, 300000);      
   }
}