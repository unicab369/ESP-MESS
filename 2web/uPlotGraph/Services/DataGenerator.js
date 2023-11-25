class DataGenerator {
   constructor() {
      this.START_TIME = '2021-01-01T00:00:00Z'
      this.MINUTES = 20
      this.startTime = new Date(this.START_TIME).getTime() / 1000
      this.endTime = this.startTime + this.MINUTES * 60
      this.timestamps = []
      this.arrayA = []
      this.arrayB = []

      for (let time = this.startTime; time < this.endTime; time++) {
         this.timestamps.push(time)
         this.makeValue(this.arrayA)
         this.makeValue(this.arrayB)
      }
   }
   
   makeRandomNumber(min, max) {
      return Math.floor(Math.random() * (max - min + 1)) + min
   }

   // Generate a random value
   makeValue(refArray) {
      let value = this.makeRandomNumber(0, 50)
      refArray.push(value.toFixed(2))
   }

   // Append a new point and remove the first one for subsequent calls
   updateValue(array) {
      const lastValue = array[array.length - 1];
      let randomIncrement = this.makeRandomNumber(-10, 10); // Adjust the range as needed
      let newValue = parseFloat(lastValue) + randomIncrement;

      while (newValue<0 || newValue>50) {
         randomIncrement = this.makeRandomNumber(-10, 10); // Adjust the range as needed
         newValue = parseFloat(lastValue) + randomIncrement;
      }

      array.push(newValue);
      array.shift()
   }

   generate() {
      this.updateValue(this.arrayA)
      this.updateValue(this.arrayB)
      return [this.timestamps, this.arrayA, this.arrayB]
   }

   requestMockData() {
      return new Promise((resolve, reject) => {
         setTimeout(() => {
            resolve(this.generate())
         }, 100)
      })
   }
}