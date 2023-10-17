const url_saveGroup = 'http://' + globalIP + '/' + 'saveGroup'

const mockData3 = [
   { id: 0, value1: "aaaaaaaaaa" },
   { id: 1, value1: "bbbbbbbbbb" },
   { id: 2, value1: "ccccccc" },
   { id: 3, value1: "" },
   { id: 4, value1: "" },
   { id: 5, value1: "" },
];

//! Clone
const mockData3Out = [...mockData3]

function loadSection3() {
   const section3 = document.getElementById("section3");
   section3.innerHTML = ""; // Clear previous data

   mockData3.forEach((item, index) => {
      const row = section3.insertRow();
      row.style.cssText = 'height: 30px; width: 100%; background-color: green;'

      addIdCell(0, row, item);

      const input = addInputCell(1, row, item, "value1")
      input.placeholder = 'name'

      const target = mockData2Out[index]
      const dataStr = target.value1

      const buttonModels = [
         {
            text: 'Save', // Replace with the desired button text
            callback: function(event) {
               sendConf(url_saveGroup, dataStr)
            }
         }
      ]

      addButtonsCell(2, row, buttonModels);
   }); 
}
