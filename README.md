# What is it?
ESP-MESS is an open-source firmware for ESP8266 and ESP32 microcontrollers, enabling\
rapid IoT prototyping for home automation, smart farming, smart business, and more. 

## Why ESP-Mess? 
Back in 2020 during Covid, I got an opportunity to look back and decide on what I want to do next. I thought about what I liked as a kid so I decided to build a remote control car. During my search for a suitable microcontroller, I stumbled upon the NodeMcu, a development board that house the ESP8266 module. Initially underestimating its capabilities, I soon realized its potential. Since then, my journey has led me to acquire skills in soldering, basic electronics, embedded programming, web development for microcontrollers, PCB design, and various wireless communication technologies.

I am building a Wireless Bridge System that can manage multiple devices using a variety of wireless technologies. The system is designed to be flexible and scalable, allowing it to adapt to different wireless application requirements. My goal is to explore and discover different IoT applications while keeping the cost minimal. To achieve this, I created ESP-MESS.

## Why a Wireless Bridge System (WBSys)?
Because it's cool. It's very cool. Ok, seriously, different wireless technologies have their own unique advantages and disadvantages. For instance, LoRa technology allows devices to communicate with each other over long distances, which is not possible with WiFi or Bluetooth. However, LoRa has a low data rate, which makes it unsuitable for transmitting large amounts of data such as videos or audios. On the other hand, UWB (Ultra Wide Band) technology has a short range but enables devices to be accurately positioned relative to another device. WiFi can stream a lot more data over the air compared to Bluetooth, but at the cost of higher power consumption, making it less suitable for battery-operated devices than Bluetooth or BLE. These are just a few examples of the advantages and disadvantages of different wireless technologies.

A Wireless Bridge System offers a versatile solution by allowing the integration of diverse wireless technologies to address various challenges. Users can enhance their networks by adding supported modules to the bridge, taking advantage of the strengths of each wireless technology. This flexibility empowers users to tailor custom wireless networks to fulfill specific needs, whether it's creating a wireless sensor network, a smart home automation system, a mesh network for industrial automation, or a communication system for autonomous vehicles. The system facilitates the extension, management, monitoring, and control of device networks. ESP-MESS is an implementation of this system, it offers carefully chosen supporting devices that balance performance and cost, enabling users to choose components that align with their requirements and budgets.

checkout our discord channel: 
https://discord.com/channels/793348436475904022/858758498875342878

A Live ESP-MESS Node:
This is an ESP-MESS Node that runs on a Lithium-ion 18650 battery and a solar panel. The device records the temperature, humidity, ambient light level, battery voltage level, and the charging current from a solar panel. It utilizes ESP-Now to send signals to another Node, which is connected to my WiFi network. This Node publishes the readings to IoTPlotter for storage and data analysis.
https://iotplotter.com/user/feed/627987619219815565

### A Solar Node
<img src="src/Images/SolarNode.jpg" alt="drawing" width="200"/>

### Data visualization
<img src="src/Images/SolarNodeReadings.png" alt="drawing" width="50%"/>

<br>

# What is the vision?
### Below is the rough draft of the initial vision. Please excuse my English.
<img src="src/Images/projectFirefly_4.png" alt="drawing" width="50%"/>

### An example a configuration
<img src="src/Images/ExampleConfiguration.jpg" alt="drawing" width="50%"/>

# what is the state of the project?
Right now it's functional but I have not finished document the setup. 
I will try to make a video with instructions on how to work it.

Currently I have got ESP-Now working and some works for the LoRa communication has been completed.
Next step is to refine the ESP-Now, LoRa, and then add support for BLE.

### What am I working on right now?
Currently, I'm testing a few BLE modules. I'm trying to measure the power consumption of different devices and compare them with each other. This way I can pick one that I think is best suited for a low power device that can monitor multiple sensor nodes and can last on a battery for the minimum of 1 year.


make all monitor