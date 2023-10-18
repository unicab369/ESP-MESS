const url_saveConf = 'http://' + globalIP + '/' + 'saveConf'
const url_testConf = 'http://' + globalIP + '/' + 'testConf'

const triggerItem = [
   { trigger: '1CLICK' },
   { trigger: '2CLICK' },
   { trigger: 'CUE_PIR'},
];

const actionItem = [
   { id: 0, action: 'OUTPUT', value1: '11', value2: '200 1' },
   { id: 1, action: 'WS2812', value1: '22', value2: '200 0' },
   { id: 2, action:'SEND_MSG', value1: '33', value2: '200 2' },
];

//! Clone
const actionItem2 = [...actionItem];
const triggerItem2 = [...triggerItem];

function loadSection1() {
   const section = new PageSection("section1")

   actionItem2.forEach((item, index) => {
      section.insertRow()
      section.addIdCell(item)
      const input = section.addInputCell(item, "value1", '70px')

      input.addEventListener('input', function(event) {
         let inputValue = event.target.value;
         inputValue = inputValue.replace(/\D/g, '');
         event.target.value = inputValue;
      });

      const options1 = ['1CLICK', '2CLICK', 'CUE_PIR', 'CUE_IR', 
                           'CUE_STATE', 'CUE_THRESHOLD']
      section.addDropdownCell(triggerItem2[index], "trigger", options1)

      const options2 = ['NONE', 'OUTPUT', 'WS2812', 'PUBLISH']
      section.addDropdownCell(item, "action", options2)

      section.addInputCell(item, "value2", '150px')

      const buttonModels = [
         {
            text: 'Test', // Replace with the desired button text
            callback: function(event) {
               const target = actionItem2[index]
               const trigg = triggerItem2[index]
               const dataStr = target.value1 + " " + target.action + " " + target.value2
               sendConf(url_testConf, dataStr)
            }
         }, {
            text: 'Save',
            callback: function(event) {
               const target = actionItem2[index]
               const trigg = triggerItem2[index]
               const dataStr = target.value1 + " " + trigg.trigger + 
                                 " " + target.action + " " + target.value2
               sendConf(url_saveConf, dataStr)
            }
         }
      ]
      section.addButtonsCell(buttonModels)
   }); 
}