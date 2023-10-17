function addIdCell(index, row, item) {
   const cell = row.insertCell(index);
   cell.textContent = item.id;
   cell.style.cssText = "width: 25px; text-align: center;";
}

function addInputCell(index, row, item, key) {
   const cell = row.insertCell(index);
   cell.style.cssText = 'width: 100%; display: flex;'

   const input = document.createElement('input');
   input.style.cssText = 'font-size: 1.2rem; width: 180px; flex: .7;';
   input.type = 'text'
   input.value = item[key]
   input.addEventListener('change', function(event) {
      item[key] = input.value;   //! store update
   })
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

   const options = ['NONE', 'OUTPUT', 'WS2812', 'SEND_MSG']
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

const url_saveConf = 'http://' + globalIP + '/' + 'saveConf'
const url_testConf = 'http://' + globalIP + '/' + 'testConf'

function sendRequest() {
   const data = {
      key1: 'val1', key2: 'val2'
   }

   fetch(url_conf, {
      method: 'POST',
      mode: 'no-cors',
      headers: {'Content-Type': 'application/json'},
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

function sendConf(url, dataStr) {
   fetch(url, {
      method: 'POST',
      mode: 'no-cors',
      headers: {'Content-Type': 'application/json'},
      body: dataStr,
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