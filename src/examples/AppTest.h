#ifdef CONFIG_IDF_TARGET_ESP32C3
   uint8_t ledPin = 12;
   uint8_t ledPin2 = 13;
#elif CONFIG_IDF_TARGET_ESP32S3 
   uint8_t ledPin = 47;
   uint8_t ledPin2 = 48;
#elif defined(ESP32)
   // uint8_t ledPin = 22;
   uint8_t ledPin = 1;
   uint8_t ledPin2 = 255;
#else
   uint8_t ledPin = 2;
   uint8_t ledPin2 = 255;
#endif

// #define TEST_PCA96 1
// #define TEST_BEHAVIOR 1
// #define TEST_PWM 1
// #define TEST_AUDIO 1
// #define TEST_BLINK 1
// #define TEST_MICROPHONE 1
// #define TEST_BLE 1
// #define TEST_BLE_BEACON 1
// #define TEST_BLE5 1
#define TEST_BLE_SCANNER 1

// Loggable TestLog("Test"); 
// Mng_Config conf;
// MyButton button1;
// RotaryEncoder rotary;
// SerialControl serial;

void testSetup() {
   // conf.setup();
   // Wire.begin(conf.sda0, conf.scl0);
}

void testRun() {
   // button1.run();
   // rotary.run();
   // serial.run();
}

#ifdef TEST_BLINK
   // void setup() {
   //    Serial.begin(115200);
   //    AppPrint("\nIM HERE 000");
   //    pinMode(ledPin, OUTPUT);
   //    // pinMode(13, OUTPUT);
   //    // pinMode(5, OUTPUT);
   // }

   // void loop() {
   //    TestLog.xLog("\nIM HERE 11122");
   //    uint8_t read = digitalRead(ledPin);
   //    digitalWrite(ledPin, !read);
   //    // digitalWrite(13, read);
   //    // digitalWrite(5, read);
   //    delay(500);
   // }

   // #include <BMI160Gen.h>

   // float convertRawGyro(int gRaw) {
   //    // since we are using 250 degrees/seconds range
   //    // -250 maps to a raw value of -32768
   //    // +250 maps to a raw value of 32767

   //    float g = (gRaw * 250.0) / 32768.0;
   //    return g;
   // }

   // void setup() {
   //    Serial.begin(115200); // initialize Serial communication
   //    while (!Serial);    // wait for the serial port to open

   //    Wire.begin(33, 32);

   //    // initialize device
   //    Serial.println("Initializing IMU device...");
   //    // BMI160.begin(BMI160GenClass::SPI_MODE, /* SS pin# = */10);
   //    BMI160.begin(BMI160GenClass::I2C_MODE);
   //    uint8_t dev_id = BMI160.getDeviceID();
   //    Serial.print("DEVICE ID: ");
   //    Serial.println(dev_id, HEX);

   //       // Set the accelerometer range to 250 degrees/second
   //    BMI160.setGyroRange(250);
   //    Serial.println("Initializing IMU device...done.");
   // }

   // void loop() {
   //    int gxRaw, gyRaw, gzRaw;         // raw gyro values
   //    float gx, gy, gz;

   //    // read raw gyro measurements from device
   //    BMI160.readGyro(gxRaw, gyRaw, gzRaw);

   //    // convert the raw gyro data to degrees/second
   //    gx = convertRawGyro(gxRaw);
   //    gy = convertRawGyro(gyRaw);
   //    gz = convertRawGyro(gzRaw);

   //    // display tab-separated gyro x/y/z values
   //    Serial.print("g:\t");
   //    Serial.print(gx);
   //    Serial.print("\t");
   //    Serial.print(gy);
   //    Serial.print("\t");
   //    Serial.print(gz);
   //    Serial.println();

   //    delay(500);
   // }


   // #include <DFRobot_BMI160.h>

   // DFRobot_BMI160 bmi160;
   // const int8_t i2c_addr = 0x68;
   
   // void setup(){
   //    Serial.begin(115200);
   //    Wire.begin(33, 32);
   //    delay(100);
      
   //    //init the hardware bmin160  
   //    if (bmi160.softReset() != BMI160_OK){
   //       Serial.println("reset false");
   //       while(1);
   //    }
      
   //    //set and init the bmi160 i2c address
   //    if (bmi160.I2cInit(i2c_addr) != BMI160_OK){
   //       Serial.println("init false");
   //       while(1);
   //    }
   // }

   // void loop(){  
   //    int i = 0;
   //    int rslt;
   //    int16_t accelGyro[6]={0}; 
      
   //    //get both accel and gyro data from bmi160
   //    //parameter accelGyro is the pointer to store the data
   //    rslt = bmi160.getAccelGyroData(accelGyro);
   //    if(rslt == 0){
   //       for(i=0;i<6;i++){
   //          if (i<3){
   //          //the first three are gyro data
   //          Serial.print(accelGyro[i]*3.14/180.0);Serial.print("\t");
   //          }else{
   //          //the following three data are accel data
   //          Serial.print(accelGyro[i]/16384.0);Serial.print("\t");
   //          }
   //       }
   //       Serial.println();
   //    }else{
   //       Serial.println("err");
   //    }
   //    delay(100);
   //    /*
   //       * //only read accel data from bmi160
   //       * int16_t onlyAccel[3]={0};
   //       * bmi160.getAccelData(onlyAccel);
   //       */

   //    /*
   //       * ////only read gyro data from bmi160
   //       * int16_t onlyGyro[3]={0};
   //       * bmi160.getGyroData(onlyGyro);
   //       */
   // }

   // #include <Adafruit_Sensor.h>
   // #include <Adafruit_ADXL343.h>

   // /* Create two sensor objects and assign unique IDs */
   // Adafruit_ADXL343 accel1 = Adafruit_ADXL343(12345);

   // void setup(void) {
   //    Serial.begin(115200);
   //    while (!Serial);

   //    Serial.println("Two Accelerometers on one Arduino"); Serial.println("");
   //    Wire.begin(33, 32);

   //    /* Initialise the first sensors, this uses the default address */
   //    if(!accel1.begin()) {
   //       /* There was a problem detecting the ADXL343 ... check your connections */
   //       Serial.println("Ooops, no ADXL343 nr1 detected ... Check your wiring!");
   //       while(1);
   //    }
      
   //    /* Set the range and data rate to whatever is appropriate for your project */
   //    /* See the sensortest example for more details */
   //    accel1.setRange(ADXL343_RANGE_2_G);
   //    accel1.setDataRate(ADXL343_DATARATE_1600_HZ);

   //    /* Display some basic information on these sensors */
   //    accel1.printSensorDetails();
   //    Serial.println("");
   // }

   // void loop(void) {
   //    /* Get new sensor events */
   //    sensors_event_t event1;
   //    sensors_event_t event2;
   //    accel1.getEvent(&event1);

   //    /* Display the results (acceleration is measured in m/s^2) */
   //    Serial.print(event1.acceleration.x); Serial.print(",");
   //    Serial.print(event1.acceleration.y); Serial.print(",");
   //    Serial.print(event1.acceleration.z); Serial.println(".");
   // }

   // #include "SparkFun_MMA8452Q.h"    // Click here to get the library: http://librarymanager/All#SparkFun_MMA8452Q

   // MMA8452Q accel;                   // create instance of the MMA8452 class

   // void setup() {
   //    Serial.begin(115200);
   //    Serial.println("MMA8452Q Orientation Test Code!");
   //    Wire.begin(33, 32);

   //    if (accel.begin(Wire, 0x1C) == false) {
   //       Serial.println("Not Connected. Please check connections and read the hookup guide.");
   //       while (1);
   //    }
   // }

   // void loop() {
   //    if (accel.available()) {      // Wait for new data from accelerometer
   //       // Orientation of board (Right, Left, Down, Up);
   //       if (accel.isRight() == true) {
   //          Serial.println("Right");
   //       }
   //       else if (accel.isLeft() == true) {
   //          Serial.println("Left");
   //       }
   //       else if (accel.isUp() == true) {
   //          Serial.println("Up");
   //       }
   //       else if (accel.isDown() == true) {
   //          Serial.println("Down");
   //       }
   //       else if (accel.isFlat() == true) {
   //          Serial.println("Flat");
   //       }
   //    }
   // }

   // #include <Adafruit_LIS3DH.h>
   // #include <Adafruit_Sensor.h>

   // // software SPI
   // //Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
   // // hardware SPI
   // //Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
   // // I2C
   // Adafruit_LIS3DH lis = Adafruit_LIS3DH();

   // void setup(void) {
   //    Serial.begin(115200);
   //    while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

   //    Serial.println("LIS3DH test!");
   //    Wire.begin(33, 32);

   //    if (! lis.begin(0x19)) {   // change this to 0x19 for alternative i2c address
   //       Serial.println("Couldnt start");
   //       while (1) yield();
   //    }
   //    Serial.println("LIS3DH found!");

   //    // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

   //    Serial.print("Range = "); Serial.print(2 << lis.getRange());
   //    Serial.println("G");

   //    // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
   //    Serial.print("Data rate set to: ");
   //    switch (lis.getDataRate()) {
   //       case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
   //       case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
   //       case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
   //       case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
   //       case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
   //       case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
   //       case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

   //       case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
   //       case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
   //       case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("16 Khz Low Power"); break;
   //    }
   // }

   // void loop() {
   //    lis.read();      // get X Y and Z data at once
   //    // Then print out the raw data
   //    Serial.print("X:  "); Serial.print(lis.x);
   //    Serial.print("  \tY:  "); Serial.print(lis.y);
   //    Serial.print("  \tZ:  "); Serial.print(lis.z);

   //    /* Or....get a new sensor event, normalized */
   //    sensors_event_t event;
   //    lis.getEvent(&event);

   //    /* Display the results (acceleration is measured in m/s^2) */
   //    Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
   //    Serial.print(" \tY: "); Serial.print(event.acceleration.y);
   //    Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
   //    Serial.println(" m/s^2 ");

   //    Serial.println();
   //    delay(200);
   // }

   // #include <MPU9250_WE.h>
   // #include <Wire.h>
   // #define MPU9250_ADDR 0x68

   // /* There are several ways to create your MPU9250 object:
   // * MPU9250_WE myMPU9250 = MPU9250_WE()              -> uses Wire / I2C Address = 0x68
   // * MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR)  -> uses Wire / MPU9250_ADDR
   // * MPU9250_WE myMPU9250 = MPU9250_WE(&wire2)        -> uses the TwoWire object wire2 / MPU9250_ADDR
   // * MPU9250_WE myMPU9250 = MPU9250_WE(&wire2, MPU9250_ADDR) -> all together
   // * Successfully tested with two I2C busses on an ESP32
   // */
   // MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

   // void setup() {
   //    Serial.begin(115200);
   //    Wire.begin(33, 32);

   //    if(!myMPU9250.init()){
   //       Serial.println("MPU9250 does not respond");
   //    }
   //    else{
   //       Serial.println("MPU9250 is connected");
   //    }

   //    /*  This is a more accurate method for calibration than the autoOffset. You have to 
   //       *  determine the minimum and maximum raw acceleration values of the axes determined 
   //       *  in the range +/- 2 g. 
   //       *  You call the function as follows: setAccOffsets(xMin,xMax,yMin,yMax,zMin,zMax);
   //       *  Use either autoOffset or setAccOffsets, not both.
   //       */
   //    //myMPU9250.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);

   //    /*  The gyroscope data is not zero, even if don't move the MPU9250. 
   //       *  To start at zero, you can apply offset values. These are the gyroscope raw values you obtain
   //       *  using the +/- 250 degrees/s range. 
   //       *  Use either autoOffset or setGyrOffsets, not both.
   //       */
   //    //myMPU9250.setGyrOffsets(45.0, 145.0, -105.0);

   //    /*  You can enable or disable the digital low pass filter (DLPF). If you disable the DLPF, you 
   //       *  need to select the bandwidth, which can be either 8800 or 3600 Hz. 8800 Hz has a shorter delay,
   //       *  but higher noise level. If DLPF is disabled, the output rate is 32 kHz.
   //       *  MPU9250_BW_WO_DLPF_3600 
   //       *  MPU9250_BW_WO_DLPF_8800
   //       */
   //    myMPU9250.enableGyrDLPF();
   //    //myMPU9250.disableGyrDLPF(MPU9250_BW_WO_DLPF_8800); // bandwidth without DLPF
      
   //    /*  Digital Low Pass Filter for the gyroscope must be enabled to choose the level. 
   //       *  MPU9250_DPLF_0, MPU9250_DPLF_2, ...... MPU9250_DPLF_7 
   //       *  
   //       *  DLPF    Bandwidth [Hz]   Delay [ms]   Output Rate [kHz]
   //       *    0         250            0.97             8
   //       *    1         184            2.9              1
   //       *    2          92            3.9              1
   //       *    3          41            5.9              1
   //       *    4          20            9.9              1
   //       *    5          10           17.85             1
   //       *    6           5           33.48             1
   //       *    7        3600            0.17             8
   //       *    
   //       *    You achieve lowest noise using level 6  
   //       */
   //    myMPU9250.setGyrDLPF(MPU9250_DLPF_6);  // lowest noise
      
   //    /*  MPU9250_GYRO_RANGE_250       250 degrees per second (default)
   //       *  MPU9250_GYRO_RANGE_500       500 degrees per second
   //       *  MPU9250_GYRO_RANGE_1000     1000 degrees per second
   //       *  MPU9250_GYRO_RANGE_2000     2000 degrees per second
   //       */
   //    myMPU9250.setGyrRange(MPU9250_GYRO_RANGE_250);

   //    /*  MPU9250_ACC_RANGE_2G      2 g   
   //       *  MPU9250_ACC_RANGE_4G      4 g
   //       *  MPU9250_ACC_RANGE_8G      8 g   
   //       *  MPU9250_ACC_RANGE_16G    16 g
   //       */
   //    myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);

   //    /*  Enable/disable the digital low pass filter for the accelerometer 
   //       *  If disabled the bandwidth is 1.13 kHz, delay is 0.75 ms, output rate is 4 kHz
   //       */
   //    myMPU9250.enableAccDLPF(true);

   //    /*  Digital low pass filter (DLPF) for the accelerometer, if enabled 
   //       *  MPU9250_DPLF_0, MPU9250_DPLF_2, ...... MPU9250_DPLF_7 
   //       *   DLPF     Bandwidth [Hz]      Delay [ms]    Output rate [kHz]
   //       *     0           460               1.94           1
   //       *     1           184               5.80           1
   //       *     2            92               7.80           1
   //       *     3            41              11.80           1
   //       *     4            20              19.80           1
   //       *     5            10              35.70           1
   //       *     6             5              66.96           1
   //       *     7           460               1.94           1
   //       */
   //    myMPU9250.setAccDLPF(MPU9250_DLPF_6);  // lowest noise

   //    Serial.print("Turn your MPU9250 slowly(!) in all directions to determine ");
   //    Serial.println(" the min/max raw acceleration values.");
   //    Serial.println("For the gyroscope offsets just note the gyro raw values for the unmoved sensor");
   //    delay(1000);
   // }

   // void loop() {
   //    xyzFloat accRaw;
   //    xyzFloat gyrRaw;
   //    xyzFloat corrAccRaw;
   //    xyzFloat corrGyrRaw;
   //    accRaw = myMPU9250.getAccRawValues();
   //    gyrRaw = myMPU9250.getGyrRawValues();
   //    corrAccRaw = myMPU9250.getCorrectedAccRawValues();
   //    corrGyrRaw = myMPU9250.getCorrectedGyrRawValues();
      
   //    Serial.println("Acceleration raw values without offset:");
   //    Serial.print(accRaw.x);
   //    Serial.print("   ");
   //    Serial.print(accRaw.y);
   //    Serial.print("   ");
   //    Serial.println(accRaw.z);

   //    Serial.println("Gyroscope raw values without offset:");
   //    Serial.print(gyrRaw.x);
   //    Serial.print("   ");
   //    Serial.print(gyrRaw.y);
   //    Serial.print("   ");
   //    Serial.println(gyrRaw.z);

   //    Serial.println("Acceleration raw values with offset:");
   //    Serial.print(corrAccRaw.x);
   //    Serial.print("   ");
   //    Serial.print(corrAccRaw.y);
   //    Serial.print("   ");
   //    Serial.println(corrAccRaw.z);

   //    Serial.println("Gyroscope raw values with offset:");
   //    Serial.print(corrGyrRaw.x);
   //    Serial.print("   ");
   //    Serial.print(corrGyrRaw.y);
   //    Serial.print("   ");
   //    Serial.println(corrGyrRaw.z);
   //    delay(100);
   // }

   // #include <SensirionI2CScd4x.h>

   // SensirionI2CScd4x scd4x;

   // void printUint16Hex(uint16_t value) {
   //    Serial.print(value < 4096 ? "0" : "");
   //    Serial.print(value < 256 ? "0" : "");
   //    Serial.print(value < 16 ? "0" : "");
   //    Serial.print(value, HEX);
   // }

   // void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
   //    Serial.print("Serial: 0x");
   //    printUint16Hex(serial0);
   //    printUint16Hex(serial1);
   //    printUint16Hex(serial2);
   //    Serial.println();
   // }

   // void setup() {
   //    Serial.begin(115200);
   //    Wire.begin(33, 32);

   //    uint16_t error;
   //    char errorMessage[256];

   //    scd4x.begin(Wire);

   //    // stop potentially previously started measurement
   //    error = scd4x.stopPeriodicMeasurement();
   //    if (error) {
   //       Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
   //       errorToString(error, errorMessage, 256);
   //       Serial.println(errorMessage);
   //    }

   //    uint16_t serial0;
   //    uint16_t serial1;
   //    uint16_t serial2;
   //    error = scd4x.getSerialNumber(serial0, serial1, serial2);
   //    if (error) {
   //       Serial.print("Error trying to execute getSerialNumber(): ");
   //       errorToString(error, errorMessage, 256);
   //       Serial.println(errorMessage);
   //    } else {
   //       printSerialNumber(serial0, serial1, serial2);
   //    }

   //    // Start Measurement
   //    error = scd4x.startPeriodicMeasurement();
   //    if (error) {
   //       Serial.print("Error trying to execute startPeriodicMeasurement(): ");
   //       errorToString(error, errorMessage, 256);
   //       Serial.println(errorMessage);
   //    }

   //    Serial.println("Waiting for first measurement... (5 sec)");
   // }

   // void loop() {
   //    uint16_t error;
   //    char errorMessage[256];

   //    delay(100);

   //    // Read Measurement
   //    uint16_t co2 = 0;
   //    float temperature = 0.0f;
   //    float humidity = 0.0f;
   //    bool isDataReady = false;
   //    error = scd4x.getDataReadyFlag(isDataReady);
   //    if (error) {
   //       Serial.print("Error trying to execute getDataReadyFlag(): ");
   //       errorToString(error, errorMessage, 256);
   //       Serial.println(errorMessage);
   //       return;
   //    }
   //    if (!isDataReady) {
   //       return;
   //    }
   //    error = scd4x.readMeasurement(co2, temperature, humidity);
   //    if (error) {
   //       Serial.print("Error trying to execute readMeasurement(): ");
   //       errorToString(error, errorMessage, 256);
   //       Serial.println(errorMessage);
   //    } else if (co2 == 0) {
   //       Serial.println("Invalid sample detected, skipping.");
   //    } else {
   //       Serial.print("Co2:");
   //       Serial.print(co2);
   //       Serial.print("\t");
   //       Serial.print("Temperature:");
   //       Serial.print(temperature);
   //       Serial.print("\t");
   //       Serial.print("Humidity:");
   //       Serial.println(humidity);
   //    }
   // }

   #include "INA226.h"

   INA226 INA(0x40);

   void measure(uint8_t count) {
      // delay(3000);
      Serial.println("\nBUS\tSHUNT\tCURRENT\tPOWER");
      Serial.println(" V\t mV\t mA\t mW");
      for (int i = 0; i < count; i++)
      {
         Serial.print(INA.getBusVoltage(), 3);
         Serial.print("\t");
         Serial.print(INA.getShuntVoltage_mV(), 3);
         Serial.print("\t");
         Serial.print(INA.getCurrent_mA(), 3);
         Serial.print("\t");
         Serial.print(INA.getPower_mW(), 3);
         Serial.println();
         delay(500);
      }
   }


   void printConfig() {
      Serial.print("LSB:\t");
      Serial.println(INA.getCurrentLSB(), 10);
      Serial.print("LSB_uA:\t");
      Serial.println(INA.getCurrentLSB_uA(), 3);
      Serial.print("shunt:\t");
      Serial.println(INA.getShunt(), 3);
      Serial.print("maxCur:\t");
      Serial.println(INA.getMaxCurrent(), 3);
      Serial.println();
   }

   void setup() {
      Serial.begin(115200);
      Serial.println(__FILE__);

      Wire.begin(33, 32);
      if (!INA.begin() )
      {
         Serial.println("could not connect. Fix and Reboot");
      }

      Serial.println();
      Serial.print("MAN:\t");
      Serial.println(INA.getManufacturerID(), HEX);
      Serial.print("DIE:\t");
      Serial.println(INA.getDieID(), HEX);
      Serial.println();
      delay(100);


      INA.setMaxCurrentShunt(1, 0.002);
      Serial.println("normalized = true (default)");
      printConfig();

      INA.setMaxCurrentShunt(1, 0.002, false);
      Serial.println("normalized = false");
      printConfig();


      INA.setMaxCurrentShunt(15, 0.002);
      Serial.println("normalized = true (default)");
      printConfig();

      INA.setMaxCurrentShunt(15, 0.002, false);
      Serial.println("normalized = false");
      printConfig();
   }


   void loop() {
      INA.setMaxCurrentShunt(1, 0.002);
      measure(20);

      INA.setMaxCurrentShunt(1, 0.002, false);
      measure(20);
   }

#elif TEST_BLE_SCANNER
   #include <Net_Bluetooth.h>

   Net_Bluetooth bluetooth;

   void setup() {
      Serial.begin(115200);
      Serial.println("Scanning...");
      pinMode(ledPin, OUTPUT);

      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      bluetooth.setup();
   }

   void loop() {
      // put your main code here, to run repeatedly:
      // bluetooth.run();
      bluetooth.scanForDevice2("98:89:13:0a:69:9b");
   }

#elif TEST_BLE5
   #include <BLEDevice.h>
   #include <BLEAdvertising.h>

   esp_ble_gap_ext_adv_params_t ext_adv_params_1M = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_CONNECTABLE,
      .interval_min = 0x30,
      .interval_max = 0x30,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_CODED,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_1M,
      .sid = 0,
      .scan_req_notif = false,
   };

   esp_ble_gap_ext_adv_params_t ext_adv_params_2M = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_SCANNABLE,
      .interval_min = 0x40,
      .interval_max = 0x40,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_1M,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_2M,
      .sid = 1,
      .scan_req_notif = false,
   };

   esp_ble_gap_ext_adv_params_t legacy_adv_params = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_IND,
      .interval_min = 0x45,
      .interval_max = 0x45,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_1M,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_1M,
      .sid = 2,
      .scan_req_notif = false,
   };

   esp_ble_gap_ext_adv_params_t ext_adv_params_coded = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_SCANNABLE,
      .interval_min = 0x50,
      .interval_max = 0x50,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_1M,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_CODED,
      .sid = 3,
      .scan_req_notif = false,
   };

   static uint8_t raw_adv_data_1m[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x12, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', '1', 'M', 0X0
   };

   static uint8_t raw_scan_rsp_data_2m[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x12, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', '2', 'M', 0X0
   };

   static uint8_t legacy_adv_data[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x15, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', 'C', 'O', 'D', 'E', 'D', 0X0
   };
   
   static uint8_t legacy_scan_rsp_data[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x16, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', 'L', 'E', 'G', 'A', 'C', 'Y', 0X0
   };

   static uint8_t raw_scan_rsp_data_coded[] = {
      0x37, 0x09, 'V', 'E', 'R', 'Y', '_', 'L', 'O', 'N', 'G', '_', 'D', 'E', 'V', 'I', 'C', 'E', '_', 'N', 'A', 'M', 'E', '_',
      'S', 'E', 'N', 'T', '_', 'U', 'S', 'I', 'N', 'G', '_', 'E', 'X', 'T', 'E', 'N', 'D', 'E', 'D', '_', 'A', 'D', 'V', 'E', 'R', 'T', 'I', 'S', 'I', 'N', 'G', 0X0
   };


   uint8_t addr_1m[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x01};
   uint8_t addr_2m[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x02};
   uint8_t addr_legacy[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x03};
   uint8_t addr_coded[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x04};

   BLEMultiAdvertising advert(4); // max number of advertisement data 

   void setup() {
      Serial.begin(115200);
      Serial.println("Multi-Advertising...");

      BLEDevice::init("");

      advert.setAdvertisingParams(0, &ext_adv_params_1M);
      advert.setAdvertisingData(0, sizeof(raw_adv_data_1m), &raw_adv_data_1m[0]);
      advert.setInstanceAddress(0, addr_1m);
      advert.setDuration(5);

      // advert.setAdvertisingParams(1, &ext_adv_params_2M);
      // advert.setScanRspData(1, sizeof(raw_scan_rsp_data_2m), &raw_scan_rsp_data_2m[0]);
      // advert.setInstanceAddress(1, addr_2m);
      // advert.setDuration(1);

      // advert.setAdvertisingParams(2, &legacy_adv_params);
      // advert.setAdvertisingData(2, sizeof(legacy_adv_data), &legacy_adv_data[0]);
      // advert.setScanRspData(2, sizeof(legacy_scan_rsp_data), &legacy_scan_rsp_data[0]);
      // advert.setInstanceAddress(2, addr_legacy);
      // advert.setDuration(2);

      // advert.setAdvertisingParams(3, &ext_adv_params_coded);
      // advert.setDuration(3);
      // advert.setScanRspData(3, sizeof(raw_scan_rsp_data_coded), &raw_scan_rsp_data_coded[0]);
      // advert.setInstanceAddress(3, addr_coded);

      delay(1000);
      advert.start(1, 0);
   }

   void loop() {
      delay(4000);
      Serial.println("IM HERE");
   }

#elif TEST_BLE_BEACON
   #include <BLEDevice.h>
   #include <BLEUtils.h>
   #include <BLEServer.h>
   #include <BLEBeacon.h>
   #include "esp_sleep.h"

   #define GPIO_DEEP_SLEEP_DURATION     3  // sleep x seconds and then wake up
   RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
   RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

   BLEAdvertising *pAdvertising;   // BLE Advertisement type
   #define BEACON_UUID "87b99b2c-90fd-11e9-bc42-526af7764f64" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)

   void setBeacon() {
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
      oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
      oBeacon.setMajor((bootcount & 0xFFFF0000) >> 16);
      oBeacon.setMinor(bootcount & 0xFFFF);

      BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
      BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
      oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

      std::string strServiceData = "a";
      strServiceData += (char)26;     // Len
      strServiceData += (char)0xFF;   // Type
      strServiceData += oBeacon.getData();
      oAdvertisementData.addData(strServiceData);
      pAdvertising->setAdvertisementData(oAdvertisementData);
      pAdvertising->setScanResponseData(oScanResponseData);
   }

   void setup() {
      Serial.begin(115200);

      Serial.printf("start ESP32 %d\n", bootcount++);

      // Create the BLE Device
      BLEDevice::init("ESP32 as iBeacon");
      pinMode(12, INPUT);
      // digitalWrite(12, LOW);
      // delay(200);
      // digitalWrite(12, HIGH);

      // delay(1000);

      // Create the BLE Server
      BLEServer *pServer = BLEDevice::createServer(); // <-- no longer required to instantiate BLEServer, less flash and ram usage
      pAdvertising = BLEDevice::getAdvertising();
      BLEDevice::startAdvertising();
      setBeacon();

      // Start advertising
      pAdvertising->start();
      Serial.println("Advertizing started...");
      delay(100);
      pAdvertising->stop();
      Serial.printf("enter deep sleep\n");
      esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
      Serial.printf("in deep sleep\n");
   }

   void loop() {

   }

#elif TEST_BLE
   #include <BLEDevice.h>
   #include <BLEServer.h>
   #include <BLEUtils.h>
   #include <BLE2902.h>

   BLEServer* pServer = NULL;
   BLECharacteristic* pCharacteristic = NULL;
   BLEDescriptor *pDescr;
   BLE2902 *pBLE2902;

   bool deviceConnected = false;
   bool oldDeviceConnected = false;
   uint32_t value = 0;

   // See the following for generating UUIDs:
   // https://www.uuidgenerator.net/

   #define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
   #define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

   class MyServerCallbacks: public BLEServerCallbacks {
      void onConnect(BLEServer* pServer) {
         deviceConnected = true;
      };

      void onDisconnect(BLEServer* pServer) {
         deviceConnected = false;
      }
   };

   void setup() {
      Serial.begin(115200);

      // Create the BLE Device
      BLEDevice::init("ESP32");

      // Create the BLE Server
      pServer = BLEDevice::createServer();
      pServer->setCallbacks(new MyServerCallbacks());

      // Create the BLE Service
      BLEService *pService = pServer->createService(SERVICE_UUID);

      // Create a BLE Characteristic
      pCharacteristic = pService->createCharacteristic(
                           CHARACTERISTIC_UUID,
                           BLECharacteristic::PROPERTY_NOTIFY
                        );                   

      // Create a BLE Descriptor
      
      pDescr = new BLEDescriptor((uint16_t)0x2901);
      pDescr->setValue("A very interesting variable");
      pCharacteristic->addDescriptor(pDescr);
      
      pBLE2902 = new BLE2902();
      pBLE2902->setNotifications(true);
      pCharacteristic->addDescriptor(pBLE2902);

      // Start the service
      pService->start();

      // Start advertising
      BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
      pAdvertising->addServiceUUID(SERVICE_UUID);
      pAdvertising->setScanResponse(false);
      pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
      BLEDevice::startAdvertising();
      Serial.println("Waiting a client connection to notify...");
   }

   void loop() {
      // notify changed value
      if (deviceConnected) {
         pCharacteristic->setValue(value);
         pCharacteristic->notify();
         value++;
         delay(10000);
      }
      // disconnecting
      if (!deviceConnected && oldDeviceConnected) {
         delay(500); // give the bluetooth stack the chance to get things ready
         pServer->startAdvertising(); // restart advertising
         Serial.println("start advertising");
         oldDeviceConnected = deviceConnected;
      }
      // connecting
      if (deviceConnected && !oldDeviceConnected) {
         // do stuff here on connecting
         oldDeviceConnected = deviceConnected;
      }
   }

#elif TEST_MICROPHONE
   #include "AudioTools.h"
   
   void setup() {
      Serial.begin(115200);
      AppPrint("IM HERE 000");
      pinMode(ledPin, OUTPUT);
   }

   void loop() {
      AppPrint("IM HERE 111");
      uint8_t read = digitalRead(12);
      digitalWrite(ledPin, !read);
      delay(1000);
   }

#elif defined(TEST_PCA96)
   PCA96Controller pca96z;

   //! ButtonPress Callback
   std::function<void(BTN_Action, BNT_Hold, uint32_t)> buttonCb = 
                     [&](BTN_Action action, BNT_Hold hold, uint32_t elapse) {
      TestLog.xLog("Button Pressed = %u", action);

      switch (action) {
         case ACTION_SINGLE_CLICK: {
               ControlOutput action1(11, 22);    
               break;
         }   
         case ACTION_DOUBLE_CLICK: {
               break;
         }
         case ACTION_PRESS_ACTIVE: {
               break;
         }
         case ACTION_PRESS_END: {
               break;
         }
      }
   };

   //! RotaryEncoder Callback
   std::function<void(RotaryDirection, uint16_t)> rotaryCb = [&](RotaryDirection state, uint16_t counter) {
      TestLog.xLog("Rotary counter = %llu", counter);
      pca96z.drivePWM(0, counter);
      // pca96z.drivePWM(1, counter);
   };

   void setup() {
      Serial.begin(115200);
      testSetup();

      pca96z.setup(&Wire);
      button1.setup(conf.btn1);
      button1.callback = &buttonCb;

      Serial.print("RotA = "); Serial.println(conf.rotaryA);
      Serial.print("RotB = "); Serial.println(conf.rotaryB);
      
      rotary.setup(conf.rotaryA, conf.rotaryB, 10, { 90, 500 });
      rotary.onCallback = &rotaryCb;
   }

   void loop() {
      // pca96z.test();
      testRun();
   }

#elif defined(TEST_AUDIO)
   #include "MusicDefinitions.h"
   #include "SoundData.h"
   #include "XT_DAC_Audio.h"

   int8_t PROGMEM TwinkleTwinkle[] = {
      NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
      NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_2,
      NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
      NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
      NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
      NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_4,  
      NOTE_SILENCE,BEAT_5,SCORE_END
   };


   XT_Wav_Class ForceWithYou(Force);  
   XT_DAC_Audio_Class DacAudio(25,0);    // Use GPIO 25, one of the 2 DAC pins and timer 0
   XT_MusicScore_Class Music(TwinkleTwinkle,TEMPO_ALLEGRO,INSTRUMENT_PIANO);

   void setup() {
      Serial.begin(115200);
      DacAudio.Play(&Music); 
   }

   void loop() {
      DacAudio.FillBuffer();                // Fill the sound buffer with data
      // if(ForceWithYou.Playing==false)       // if not playing,
      //    // DacAudio.Play(&ForceWithYou);       // play it, this will cause it to repeat and repeat...
      //    DacAudio.Play(&Music); 
   }

#elif defined(TEST_BEHAVIOR)
   Mng_Storage storage;
   Serv_Behavior servBehav;

   uint8_t peer1Mac[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00};

   //! ButtonPress Callback
   std::function<void(BTN_Action, BNT_Hold, uint32_t)> buttonCb = 
                     [&](BTN_Action action, BNT_Hold hold, uint32_t elapse) {
      switch (action) {
         case ACTION_SINGLE_CLICK: {
               ControlOutput action1(11, 22);    
               servBehav.storeAction<TRIGGER_SINGLECLICK>(0, &action1, peer1Mac);
               break;
         }   
         case ACTION_DOUBLE_CLICK: {
               break;
         }
         case ACTION_PRESS_ACTIVE: {
               break;
         }
         case ACTION_PRESS_END: {
               servBehav.deleteData();
               break;
         }
      }
   };

   //! RotaryEncoder Callback
   std::function<void(RotaryDirection, uint16_t)> rotaryCb = [&](RotaryDirection state, uint16_t counter) {
      uint8_t value = (state == CLOCKWISE) ? 1 : (state == COUNTERCLOCKWISE) ? -1 : 0;
      peer1Mac[5] = peer1Mac[5] + value;
      Serial.print("Update Peer1Mac: ");
      AppPrintHex(peer1Mac, sizeof(peer1Mac));
   };

   std::function<void(char*)> storeCredCb = [&](char* inputStr) {
      storage.handleConsoleStr(inputStr);
   };

   void setup() {
      Serial.begin(115200);

      testSetup();
      storage.setup();
      servBehav.setup();

      // servBehav.stoPeer.printAllPeers();
      servBehav.stoBehav.printAll();      

      // uint8_t peer1Mac[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x07};
      // ControlOutput action1(11, 22);    
      // // servBehav.storeAction<TRIGGER_SINGLECLICK>(0, &action1, peer1Mac);
      // servBehav.stoPeer.addPeer(peer1Mac);

      // ControlWS2812 action2(33, 44);
      // servBehav.storeAction<TRIGGER_DOUBLECLICK>(1, &action2, &peer1);


      button1.setup(conf.btn1, &buttonCb);
      serial.onParseString = &storeCredCb;

      rotary.onCallback = &rotaryCb;
   }

   void loop() {
      testRun();
   }

#elif defined(TEST_PWM)
   PulseController led;

   void setup() {
      Serial.begin(115200);
      led.setup(ledPin, false);
      // led.singlePulses(100);
      // led.doublePulses(100);
      // led.pulse20ms();
      led.uniformPulse500ms();
   }

   void loop() {
      led.run();
   }
#else
   void donothing() {

   }
#endif
