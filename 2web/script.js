// script.js
function greet() {
   alert("Hello, world!");
}

function navToAbout() {
   window.location.href = 'PageAbout.html';
}

function navToDeviceConf() {
   window.location.href = 'PageDeviceConf.html';
}

function navBack() {
   window.history.back();
}

const mockData1 = [
   { id: 0, name: "John", age: 30 },
   { id: 1, name: "Alice", age: 25 },
   { id: 2, name: "Bob", age: 35 },
];

function addIdCell(index, row, item) {
   const cell0 = row.insertCell(index);
   cell0.textContent = item.id;
   cell0.style.cssText = "background-color: red;";
}

function addInputCell(index, row, item) {
   const cell = row.insertCell(index);
   

   const input = document.createElement('input');
   input.type = 'text'
   input.style.cssText = 'font-size: 1.5rem; width: 80%; float: left;';
   cell.appendChild(input)

   const clearBtn = document.createElement('button')
   clearBtn.textContent = 'x'
   clearBtn.style.cssText = 'font-size: 1.5rem; width: 30px; float: left;'
   clearBtn.addEventListener('click', function(event) {
      input.value = "";   
   })
   cell.appendChild(clearBtn)
   
   return input;
}

function addButtonsCell(index, row, models) {
   const cell = row.insertCell(index);
   
   models.forEach((item) => {
      const button = document.createElement('button')
      button.textContent = item.text; // Set the button text
      button.addEventListener('click', item.callback)
      cell.appendChild(button)
   })
}

function addDropdownCell(index, row, item) {
   const cell2 = row.insertCell(index);
   cell2.insertAdjacentHTML('beforeend', 
   "<select id='stacked-state' style='width: 100%; font-size: 1.5rem;'>\
      <option>OUTPUT</option>\
      <option>WS2812</option>\
      <option>SEND_MSG</option>\
   </select>");
}

function loadSection1() {
   // Data is fetched successfully, now render it in the table
   const section1 = document.getElementById("section1");
   section1.innerHTML = ""; // Clear previous data
   
   mockData1.forEach((item) => {
      const row = section1.insertRow();
      addIdCell(0, row, item);
      const input = addInputCell(1, row, item);

      input.addEventListener('input', function(event) {
         let inputValue = event.target.value;
         inputValue = inputValue.replace(/\D/g, '');
         event.target.value = inputValue;
      });

      addDropdownCell(2, row, item);
      addInputCell(3, row, item);

      const buttonModels = [
         {
            text: 'Test', // Replace with the desired button text
            callback: function(event) {
               alert("IM HERE 1");
            }
         }, {
            text: 'Save',
            callback: function(event) {
               alert("IM HERE 222");
            }
         }
      ]

      addButtonsCell(4, row, buttonModels);
   }); 
}

const mockData2 = [
   { id: 0, name: "AA:AA:AA:AA:AA:01", age: 30 },
   { id: 1, name: "AA:AA:AA:AA:AA:02", age: 25 },
   { id: 2, name: "AA:AA:AA:AA:AA:03", age: 35 },
   { id: 3, name: "", age: 35 },
   { id: 4, name: "", age: 35 },
   { id: 5, name: "", age: 35 },
];

function loadSection2() {
   mockData2.forEach((item) => {
      const row = section2.insertRow();
      addIdCell(0, row, item);
      const input = addInputCell(1, row, item);
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

      const buttonModels = [
         {
            text: 'Save', // Replace with the desired button text
            callback: function(event) {
               alert("IM HERE 1");
            }
         }, {
            text: 'Delete',
            callback: function(event) {
               alert("IM HERE 222");
            }
         }
      ]

      addButtonsCell(2, row, buttonModels);
   }); 
}

function loadSection3() {
   const mockData2 = [
      { id: 0, name: "aaaaaaaaaa", age: 30 },
      { id: 1, name: "bbbbbbbbbb", age: 25 },
      { id: 2, name: "ccccccc", age: 35 },
      { id: 3, name: "", age: 35 },
      { id: 4, name: "", age: 35 },
      { id: 5, name: "", age: 35 },
   ];

   mockData2.forEach((item) => {
      const row = section3.insertRow();
      addIdCell(0, row, item);

      const input = addInputCell(1, row, item)
      input.placeholder = 'name'

      const buttonModels = [
         {
            text: 'Save', // Replace with the desired button text
            callback: function(event) {
               alert("IM HERE 1");
            }
         }, {
            text: 'Delete',
            callback: function(event) {
               alert("IM HERE 222");
            }
         }
      ]

      addButtonsCell(2, row, buttonModels);
   }); 
}

// function loadData() {
//     // Define the URL for your data API
//     const apiUrl = "https://api.example.com/data"; // Replace with your API URL

//     // Make an HTTP GET request to fetch data
//     fetch(apiUrl)
//         .then((response) => response.json())
//         .then((data) => {
//             // Data is fetched successfully, now render it in the table
//             const tableBody = document.querySelector("#dataTable tbody");
//             tableBody.innerHTML = ""; // Clear previous data

//             data.forEach((item) => {
//                 const row = tableBody.insertRow();
//                 row.insertCell(0).textContent = item.id;
//                 row.insertCell(1).textContent = item.name;
//                 row.insertCell(2).textContent = item.email;
//             });
//         })
//         .catch((error) => {
//             console.error("Error fetching data: ", error);
//         });   
// }