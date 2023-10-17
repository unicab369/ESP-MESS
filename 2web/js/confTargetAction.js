const mockData1 = [
   { id: 0, action: 'OUTPUT', value1: '11', value2: '200 1' },
   { id: 1, action: 'WS2812', value1: '22', value2: '200 0' },
   { id: 2, action:'SEND_MSG', value1: '33', value2: '200 2' },
];

//! Clone
const mockData1Out = [...mockData1];

function loadSection1() {
   const section = document.getElementById("section1");
   section.innerHTML = ""; // Clear previous data

   mockData1Out.forEach((item, index) => {
      const row = section.insertRow()
      row.style.cssText = 'height: 30px; width: 100%; background-color: green;'

      addIdCell(0, row, item);
      const input = addInputCell(1, row, item, "value1");

      input.addEventListener('input', function(event) {
         let inputValue = event.target.value;
         inputValue = inputValue.replace(/\D/g, '');
         event.target.value = inputValue;
      });

      addDropdownCell(2, row, item);
      addInputCell(3, row, item, "value2");

      const target = mockData1Out[index]
      const dataStr = target.value1 + " " + target.action + " " + target.value2

      const buttonModels = [
         {
            text: 'Test', // Replace with the desired button text
            callback: function(event) {
               sendConf(url_testConf, dataStr)
            }
         }, {
            text: 'Save',
            callback: function(event) {
               sendConf(url_saveConf, dataStr)
            }
         }
      ]

      addButtonsCell(4, row, buttonModels);
   }); 
}