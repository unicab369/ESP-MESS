const url_saveGroup = 'http://' + globalIP + '/' + 'saveGroup'

const mockData3 = [
   { id: 0, value1: "GroupName1" },
   { id: 1, value1: "GroupName2" },
   { id: 2, value1: "GroupName3" },
   { id: 3, value1: "" },
   { id: 4, value1: "" },
   { id: 5, value1: "" },
];

//! Clone
const mockData3Out = [...mockData3]

function loadSection3() {
   const section = new PageSection("section3")

   mockData3.forEach((item, index) => {
      section.insertRow()
      section.addIdCell(item)

      const input = section.addInputCell(item, "value1")
      input.placeholder = 'name'

      const buttonModels = [
         {
            text: 'Save', // Replace with the desired button text
            callback: function(event) {
               const target = mockData2Out[index]
               const dataStr = target.value1         
               sendConf(url_saveGroup, dataStr)
            }
         }
      ]

      section.addButtonsCell(buttonModels)
   }); 
}
