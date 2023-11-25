class uPlotOption {
   constructor(title='setTitle', label = 'setLabel', strokeColor ='blue', width = 1920, height = 600) {
      this.title = title;
      this.label = label;
      this.strokeColor = strokeColor;
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
               label: "Label_Y",
               points: { show: true, size: 5 },
               stroke: 'green',
               width: 1,
            }
            ,
            {
               label: "Label_Z",
               points: { show: true, size: 5 },
               stroke: 'red',
               width: 1,
            }
         ]
      }
   }
}