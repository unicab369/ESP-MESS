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

function loadData() {
   // Data is fetched successfully, now render it in the table
   const section1 = document.getElementById("section1");
   section1.innerHTML = ""; // Clear previous data

   const mockData1 = [
      { id: 0, name: "John", age: 30 },
      { id: 1, name: "Alice", age: 25 },
      { id: 2, name: "Bob", age: 35 },
   ];

   mockData1.forEach((item) => {
      const row = section1.insertRow();
      const cell0 = row.insertCell(0);
      const cell1 = row.insertCell(1);
      const cell2 = row.insertCell(2);
      const cell3 = row.insertCell(3);

      //! Cell0
      cell0.textContent = item.id;

      //! Cell1
      const input = document.createElement('input');
      input.type = 'text';
      cell1.appendChild(input);

      //! Cell2
      cell2.insertAdjacentHTML('beforeend', "<select id='stacked-state'>\
         <option>OUTPUT</option>\
         <option>WS2812</option>\
         <option>SEND_MSG</option>\
      </select>");

      //! Cell3
      const button2 = document.createElement('button');
      button2.textContent = 'Click Me'; // Set the button text
      cell3.appendChild(button2);
   }); 


   const mockData2 = [
      { id: 0, name: "AA:AA:AA:AA:AA:01", age: 30 },
      { id: 1, name: "AA:AA:AA:AA:AA:02", age: 25 },
      { id: 2, name: "AA:AA:AA:AA:AA:03", age: 35 },
      { id: 3, name: "", age: 35 },
      { id: 4, name: "", age: 35 },
      { id: 5, name: "", age: 35 },
   ];

   mockData2.forEach((item) => {
      const row = section2.insertRow();
      const cell0 = row.insertCell(0);
      const cell1 = row.insertCell(1);
      const cell2 = row.insertCell(2);
      const cell3 = row.insertCell(3);
      const cell4 = row.insertCell(4);

      //! Cell0
      cell0.textContent = item.id;

      //! Cell1
      const input = document.createElement('input');
      input.type = 'text';
      input.value = item.name;
      input.placeholder = '00:00:00:00:00:00'
      cell1.appendChild(input);

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
         
      //! Cell3
      const button2 = document.createElement('button');
      button2.textContent = 'Save'; // Set the button text
      cell3.appendChild(button2);

      //! Cell4
      const button3 = document.createElement('button');
      button3.textContent = 'Del'; // Set the button text
      cell3.appendChild(button3);
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