function RecordOption(label, color) {
   this.label = label
   this.color = color
}

class uPlotOption {
   constructor(title, recordOptions, width, height) {
      this.title = title;
      this.recordOptions = recordOptions
      this.width = width;
      this.height = height;
   }

   getOptions (){
      return {
         title: this.title,
         width: this.width,
         height: this.height,
         scales: {
            x: { time: true }, 
            y: {
               range: [0, 55]
            }
         },
         series: [
            {
               label: "Label_X",
            },
            {
               points: { show: true, size: 1 }, width: 1,
               label: this.recordOptions[0].label,
               stroke: '#'+this.recordOptions[0].color,
            }
            ,
            {
               points: { show: true, size: 1 }, width: 1,
               label: this.recordOptions[1].label,
               stroke: '#'+this.recordOptions[1].color,
            }
         ]
      }
   }
}