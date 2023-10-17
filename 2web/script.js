function addIdCell(index, row, item) {
   const cell = row.insertCell(index);
   cell.textContent = item.id;
   cell.style.cssText = "width: 25px; text-align: center;";
}

function addInputCell(index, row, item) {
   const cell = row.insertCell(index);
   cell.style.cssText = 'width: 100%; display: flex;'

   const input = document.createElement('input');
   input.type = 'text'
   input.style.cssText = 'font-size: 1.2rem; width: 180px; flex: .7;';
   cell.appendChild(input)

   const clearBtn = document.createElement('button')
   clearBtn.textContent = 'x'
   clearBtn.style.cssText = 'font-size: 1.2rem; width: 30px;'
   clearBtn.addEventListener('click', function(event) {
      input.value = "";   
   })
   cell.appendChild(clearBtn)
   
   return input;
}

function addButtonsCell(index, row, models) {
   const cell = row.insertCell(index);
   cell.style.cssText = 'width: 100px;'

   models.forEach((item) => {
      const button = document.createElement('button')
      button.textContent = item.text; // Set the button text
      button.addEventListener('click', item.callback)
      cell.appendChild(button)
   })
}

function addDropdownCell(index, row, item) {
   const cell = row.insertCell(index);
   cell.style.cssText = 'width: 100px;'

   const select = document.createElement('select')
   select.style.cssText = 'font-size: 1.2rem;'

   const options = ['OUTPUT', 'WS2812', 'SEND_MSG']
   options.forEach(optionText => {
      const option = document.createElement('option')
      option.text = optionText
      option.selected = option.text == item.action
      select.add(option);
   });

   // Add an event listener to the select element to track changes
   select.addEventListener('change', function() {
      item.action = select.value
      console.log(item)
   });

   cell.appendChild(select);
}

function sendCORSRequest() {
   const url = 'http://' + globalIP + '/' + 'devConf'
   const xhr = new XMLHttpRequest()

   //! send OPTIONS request
   xhr.open('OPTIONS', url, true)

   xhr.onload = function () {
      console.log("IM HERE AAAAAAAAAAAAAAAAAA")
   };

   xhr.send()
}

function sendRequest() {
   const data = {
      key1: 'val1', key2: 'val2'
   }

   const url = 'http://' + globalIP + '/' + 'devConf'

   fetch(url, {
      method: 'POST',
      mode: 'no-cors',
      headers: {
         'Content-Type': 'application/json',
      },
      body: JSON.stringify(data),
   })
   .then(response => {
      if (!response.ok) {
         // Check if the response status is not in the range of 200-299 (indicating an error)
         throw new Error(`Request failed with status: ${response.status}`);
      }
      return response.text(); // Read the response body as text
   })
   .then(data => {
      // Handle the response from the server.
      console.log('Response data:', data);
   })
   .catch(error => {
      console.error('Error:', error);
   });
}


const mockData1 = [
   { id: 0, name: "John", action: 'OUTPUT' },
   { id: 1, name: "Alice", action: 'WS2812' },
   { id: 2, name: "Bob", action:'SEND_MSG' },
];

function loadSection1() {
   const section = document.getElementById("section1");
   section.innerHTML = ""; // Clear previous data
   
   mockData1.forEach((item, index) => {
      const row = section.insertRow()
      row.style.cssText = 'height: 30px; width: 100%; background-color: green;'

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
               sendRequest()
            }
         }, {
            text: 'Save',
            callback: function(event) {
               console.log("zzzz = " + index)
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
      row.style.cssText = 'height: 30px; width: 100%; background-color: green;'

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
      row.style.cssText = 'height: 30px; width: 100%; background-color: green;'

      addIdCell(0, row, item);

      const input = addInputCell(1, row, item)
      input.placeholder = 'name'

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