function generateTimestampsAndData(hours) {
    const START_TIME = '2021-01-01T00:00:00Z';
    // Convert start date to Unix timestamp
    let startTime = new Date(START_TIME).getTime() / 1000;
    // Calculate end time
    let endTime = startTime + hours * 60 * 60;
    // Generate timestamps and data
    let timestamps = [];
    let data = [];
    for (let time = startTime; time < endTime; time++) {
        timestamps.push(time);
        let amplitude = (Math.random() * 100).toFixed(2);
        let value = amplitude * Math.sin(time)
        data.push(value.toFixed(2));
    }

    return [timestamps, data];
}

class uPlotOption {
    constructor(title='setTitle', label = 'setLabel', strokeColor ='blue', width = 1920, height = 600) {
        this.width = width;
        this.height = height;
        this.strokeColor = strokeColor;
        this.label = label;
        this.title = title;
    }

    getOptions (){
        return {
            title: this.title,
            width: this.width,
            height: this.height,
            scales: {
                x: {
                    time:true
                },
                y: {
                    //Automatically set range
                }
            },
            series: [
                {},
                {
                    stroke: this.strokeColor,
                    label: this.label,
                }
            ]
        }
    }
}