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
        { id: 1, name: "John", age: 30 },
        { id: 2, name: "Alice", age: 25 },
        { id: 3, name: "Bob", age: 35 },
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
        { id: 1, name: "John", age: 30 },
        { id: 2, name: "Alice", age: 25 },
        { id: 3, name: "Bob", age: 35 },
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
        cell1.appendChild(input);

        //! Cell2
        const input2 = document.createElement('input');
        input.type = 'text';
        cell2.appendChild(input2);

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