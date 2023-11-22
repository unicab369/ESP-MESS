let globalIP = sessionStorage.getItem('globalIP') || "192.168.4.1";

function greet() {
   alert("Hello, world!");
}

function navToAbout() {
   window.location.href = 'PageAbout.html';
}

function navToDeviceConf() {
   window.location.href = 'PageBehaviors.html';
}

function navToGraphData() {
   window.location.href = 'PageGraph.html';
}

function navBack() {
   window.history.back();
}

function navToHome() {
   window.location.href = 'PageHome.html';
}

function validateIP(inputField) {
   inputField.value = inputField.value.replace(/[^0-9.]/g, '');
}

function saveDevIP() {
   const inputField = document.getElementById("devIP_tField");
   globalIP = inputField.value;
   alert("IP address saved: " + globalIP)
   sessionStorage.setItem('globalIP', globalIP);
}

function initializePage() {
   const devIPInput = document.getElementById("devIP_tField");
   devIPInput.value = globalIP;
   sendCORSRequest();
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
