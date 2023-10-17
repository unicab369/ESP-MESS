const mockData2 = [
   { id: 0, value1: "AA:AA:AA:AA:AA:01" },
   { id: 1, value1: "AA:AA:AA:AA:AA:02" },
   { id: 2, value1: "AA:AA:AA:AA:AA:03" },
   { id: 3, value1: "" },
   { id: 4, value1: ""  },
   { id: 5, value1: "" },
];

//! Clone
const mockData2Out = [...mockData2];

function loadSection2() {
   mockData2Out.forEach((item, index) => {
      const row = section2.insertRow();
      row.style.cssText = 'height: 30px; width: 100%; background-color: green;'

      addIdCell(0, row, item);
      const input = addInputCell(1, row, item, "value1");
      input.placeholder = '00:00:00:00:00:00';

      input.addEventListener('input', function (event) {
         const inputValue = event.target.value;
         let filter = inputValue.replace(/:/g, ''); // Remove existing colons
         filter = inputValue.replace(/[^0-9A-Fa-f]/g, '');
         const sanitizedValue = filter.split('').map((char, index) => {
            if (index > 0 && index % 2 === 0) { return `:${char}`; }
            return char;
         }).join('');
   
         // Update the input field with the formatted value
         event.target.value = sanitizedValue.slice(0, 17).toUpperCase();

         const isValid = /^[0-9a-fA-F:]*$/.test(inputValue);
         input.style.backgroundColor = isValid ? '' : 'red';
      });

      const target = mockData2Out[index]
      const dataStr = target.value1

      const buttonModels = [
         {
            text: 'Save', // Replace with the desired button text
            callback: function(event) {
               alert("IM HERE 1");
            }
         }
      ]

      addButtonsCell(2, row, buttonModels);
   }); 
}
