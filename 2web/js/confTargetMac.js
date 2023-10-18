const url_saveMac = 'http://' + globalIP + '/' + 'saveMac'

const mockData2 = [
   { id: 0, value1: "AA:AA:AA:AA:AA:01" },
   { id: 1, value1: "AA:AA:AA:AA:AA:02" },
   { id: 2, value1: "AA:AA:AA:AA:AA:03" },
   { id: 3, value1: "" },
];

//! Clone
const mockData2Out = [...mockData2];

function loadSection2() {
   const section = new PageSection("section2")

   mockData2Out.forEach((item, index) => {
      section.insertRow()
      section.addIdCell(item)
      const input = section.addInputCell(item, "value1")
      input.placeholder = '00:00:00:00:00:00';

      input.addEventListener('input', function (event) {
         const inputValue = event.target.value;
         let filter = inputValue.replace(/:/g, ''); // Remove existing colons
         filter = inputValue.replace(/[^0-9A-Fa-f]/g, '');
         const sanitizedValue = filter.split('').map((char, index) => {
            if (index > 0 && index % 2 === 0) { return `:${char}` }
            return char
         }).join('')
   
         // Update the input field with the formatted value
         event.target.value = sanitizedValue.slice(0, 17).toUpperCase()
         mockData2Out[index].value1 = target.value

         const isValid = /^[0-9a-fA-F:]*$/.test(inputValue)
         input.style.backgroundColor = isValid ? '' : 'red'
      });

      const buttonModels = [
         {
            text: 'Save', // Replace with the desired button text
            callback: function(event) {
               const target = mockData2Out[index]
               const dataStr = index + " " + target.value1
               sendConf(url_saveMac, dataStr)
            }
         }
      ]

      section.addButtonsCell(buttonModels)
   }); 
}
