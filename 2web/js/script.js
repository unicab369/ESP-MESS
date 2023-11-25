// function addIdCell(index, row, item) {
//    const cell = row.insertCell(index);
//    cell.textContent = item.id;
//    cell.style.cssText = "width: 25px; text-align: center;";
// }

// function addInputCell(index, row, item, key) {
//    const cell = row.insertCell(index);
//    cell.style.cssText = 'width: 100%; display: flex;'

//    const input = document.createElement('input');
//    input.style.cssText = 'font-size: 1.2rem; width: 180px; flex: .7;';
//    input.type = 'text'
//    input.value = item[key]
//    input.addEventListener('change', function(event) {
//       item[key] = input.value;   //! store update
//    })
//    cell.appendChild(input)

//    const clearBtn = document.createElement('button')
//    clearBtn.textContent = 'x'
//    clearBtn.style.cssText = 'font-size: 1.2rem; width: 30px;'
//    clearBtn.addEventListener('click', function(event) {
//       input.value = "";   
//    })
//    cell.appendChild(clearBtn)
   
//    return input;
// }

// function addButtonsCell(index, row, models) {
//    const cell = row.insertCell(index);
//    cell.style.cssText = 'width: 100px;'

//    models.forEach((item) => {
//       const button = document.createElement('button')
//       button.textContent = item.text; // Set the button text
//       button.addEventListener('click', item.callback)
//       cell.appendChild(button)
//    })
// }

// function addDropdownCell(index, row, item, options) {
//    const cell = row.insertCell(index);
//    cell.style.cssText = 'width: 100px;'

//    const select = document.createElement('select')
//    select.style.cssText = 'font-size: 1.2rem;'

//    options.forEach(optionText => {
//       const option = document.createElement('option')
//       option.text = optionText
//       option.selected = option.text == item.action
//       select.add(option);
//    });

//    // Add an event listener to the select element to track changes
//    select.addEventListener('change', function() {
//       item.action = select.value
//       console.log(item)
//    });

//    cell.appendChild(select);
// }

function dataRequest() {
   const data = generateTimestampsAndData(1)

   fetch(url_conf, {
      method: 'GET',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify(data),
   })
   .then(response =>{
      if(!response.ok){
         throw new Error(`Request failed with status: ${response.status}`)
      }
      return response.json()
   })
   .catch(error => {
      console.error('Error:', error);
   });
}

const dataGen = new DataGenerator();

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
   console.log("IM HERE " + url)
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
      return response.json(); // Read the response body as text
   })
   .then(data => {
      // Handle the response from the server.
      console.log('Response data:', data);
   })
   .catch(error => {
      console.error(error);
   });
}


class PageSection {
   constructor(id) {
      this.section = document.getElementById(id)
      this.section.innerHTML = ""
   }

   insertRow() {
      this.row = this.section.insertRow()
      this.row.style.cssText = 'height: 50px; width: 100%; background-color: green;'
      this.cellIndex = 0
   }

   insertCell() {
      const cell = this.row.insertCell(this.cellIndex)
      this.cellIndex++
      return cell
   }

   addIdCell(item) {
      const cell = this.insertCell()
      cell.textContent = item.id
      cell.style.cssText = "width: 30px; text-align: center; font-size: 2rem;"
   }

   addButtonsCell(models) {
      const cell = this.insertCell()
   
      models.forEach((item) => {
         const button = document.createElement('button')
         button.style.cssText = 'font-size: 2rem;'
         button.textContent = item.text; // Set the button text
         button.addEventListener('click', item.callback)
         cell.appendChild(button)
      })
   }

   addInputCell(item, key, width = '350px') {
      const cell = this.insertCell()
      cell.style.cssText = 'height: 100%;'
   
      const input = document.createElement('input');
      input.style.cssText = 'font-size: 2rem; heigh: 100%; width: ' + width;
      input.type = 'text'
      input.value = item[key]
      input.addEventListener('change', function(event) {
         item[key] = input.value;   //! store update
      })

      const container = document.createElement('div');
      container.style.cssText = "height: 100%;"
      container.appendChild(input);
      cell.appendChild(container)
   
      const clearBtn = document.createElement('button')
      clearBtn.textContent = 'x'
      clearBtn.style.cssText = 'font-size: 2rem; width: 30px;'
      clearBtn.addEventListener('click', function(event) {
         input.value = ""
      })
      container.appendChild(clearBtn)

      return input
   }

   addDropdownCell(item, key, options) {
      const cell = this.insertCell()
      cell.style.cssText = 'width: 150px;'
      
      const select = document.createElement('select')
      select.style.cssText = 'font-size: 1.5rem;'
      select.className = "SizeFull"
   
      options.forEach(optionText => {
         const option = document.createElement('option')
         option.text = optionText
         option.selected = optionText == item[key]
         select.add(option);
      });
   
      // Add an event listener to the select element to track changes
      select.addEventListener('change', function() {
         item[key] = select.value
         console.log(item)
      });
   
      cell.appendChild(select);
   }
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