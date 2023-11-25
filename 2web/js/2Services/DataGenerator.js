class DataGenerator {
   constructor() {
      this.START_TIME = '2021-01-01T00:00:00Z'
      this.MINUTES = 20
      this.startTime = new Date(this.START_TIME).getTime() / 1000
      this.endTime = this.startTime + this.MINUTES * 60
      this.timestamps = []

      for (let time = this.startTime; time < this.endTime; time++) {
         this.timestamps.push(time)
      }

      this.arrayA = new Array(this.timestamps.length).fill(0)
      this.arrayB = Array.from(this.arrayA)
   }
   
   // Generate a random value
   makeRandomNumber(min=-4, max=5) {
      return Math.floor(Math.random() * (max - min + 1)) + min
   }

   // Append a new point and remove the first one for subsequent calls
   updateValue(array) {
      const lastValue = parseFloat(array[array.length - 1]);   //! convert to Float in order to add to the randomValue
      let randomIncrement = this.makeRandomNumber();    // Adjust the range as needed
      let newValue = lastValue + randomIncrement;

      while (newValue<0 || newValue>50) {
         randomIncrement = this.makeRandomNumber();     // Adjust the range as needed
         newValue = lastValue + randomIncrement;
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