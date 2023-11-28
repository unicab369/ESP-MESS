class Serv_IO {
    public:
        uint8_t irSwitchp = 255;
        uint8_t ws2812p = 255;
        uint8_t buzzer1 = 255;
        uint8_t led1 = 255; uint8_t led2 = 255;
        uint8_t pir1 = 255; uint8_t pir2 = 255;
        uint8_t btn1 = 255; uint8_t btn2 = 255;
        uint8_t relay1p = 255; uint8_t relay2 = 255;
        
        //! i2C lines
        uint8_t sda0 = 255; uint8_t scl0 = 255;
        uint8_t sda1 = 255; uint8_t scl1 = 255;

        //! SPI lines
        uint8_t mosi0 = 255, miso0 = 255, sck0 = 255, rst0 = 255;
        uint8_t out0 = 255, out1 = 255, out2 = 255, out3 = 255;

        uint8_t mosi1 = 255, miso1 = 255, sck1 = 255, rst1 = 255;

        uint8_t cs1 = 255; uint8_t di01 = 255;
        uint8_t swRx = 255; uint8_t swTx = 255;
        uint8_t rotaryA = 255; uint8_t rotaryB = 255;

        WS28xx ws2812;
        MyButton button1;
        IRSwitch irSwitch;
        EdgeDetector edgeDetector;
        ExtraSerial xSerial;

        PulseController led, buzzer;
        PinWritable relay1;
        RotaryEncoder rotary;

        bool checkHSPIPins() {
            return sck0 != 255 && miso0 != 255 && mosi0 != 255;
        }

        bool checkWire0() {
            return sda0 != 255 && scl0 != 255;
        }

        bool checkWire1() {
            return sda1 != 255 && scl1 != 255;
        }

        void setupIOs() {
            led.setup(led1);
            led.uniformPluse1000ms();
            relay1.begin(relay1p);

            // buzzer.setup(conf->buzzer1);
            ws2812.setup(ws2812p);
            xSerial.setup(swRx, swTx);

            irSwitch.load(irSwitchp);
            edgeDetector.setup(pir1);
            button1.setup(btn1);
            rotary.setup(rotaryA, rotaryB);
        }

        
};