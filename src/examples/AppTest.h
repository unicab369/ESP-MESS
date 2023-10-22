#ifdef CONFIG_IDF_TARGET_ESP32C3
   uint8_t ledPin = 12;
#elif defined(ESP32)
   uint8_t ledPin = 22;
#else
   uint8_t ledPin = 2;
#endif

// #define TEST_PCA96 1
// #define TEST_BEHAVIOR 1
// #define TEST_PWM 1
// #define TEST_AUDIO 1
// #define TEST_BLINK 1
#define TEST_MICROPHONE 1

Loggable TestLog("Test"); 
Mng_Config conf;
MyButton button1;
RotaryEncoder rotary;
SerialControl serial;

void testSetup() {
   conf.setup();
   rotary.setup(conf.rotaryA, conf.rotaryB, 10);
   Wire.begin(conf.sda0, conf.scl0);
}

void testRun() {
   button1.run();
   rotary.run();
   serial.run();
}

#ifdef TEST_BLINK
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

#ifdef TEST_MICROPHONE
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
      button1.setup(conf.btn1, &buttonCb);

      rotary.onCallback = &rotaryCb;
      rotary.setup(conf.rotaryA, conf.rotaryB, 10, { 90, 500 });
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
   void setup() {
      Serial.begin(115200);
   }

   void loop() {

   }
#endif
