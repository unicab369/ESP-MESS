#include "NeoController.h"

class WS28xx {
    NeoController neoController;
    SingleColorPixelFrame frame1;
    CRGB colorsA[2] = { CRGB::Red, CRGB::Blue };    
    CRGB colors3_A[3] = { CRGB::Red, CRGB::Green, CRGB::Blue };
    CRGB colors4_A[4] = { CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::Purple };
    CRGB colors4_B[4] = { CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Red };

    NeoStaticFrame staticFrame1;
    NeoStaticFrame staticFrame2;
    NeoStaticFrame staticFrame3;
    NeoStaticFrame staticFrame4;
    NeoRainbowFrame rainbow1;

    NeoStationFrame station1;
    NeoChasingBar chasingBar1;
    NeoChasingFrame chasingFrame1;

    NeoTrailingFrame tralingFrame1;

    ColorChunk colorChunksA[3] = { ColorChunk::make(1, CRGB::Red), 
                                    ColorChunk::make(2, CRGB::Green),
                                    ColorChunk::make(3, CRGB::Blue) };

    ColorChunk colorChunksB[3] = { ColorChunk::make(1, CRGB::Black), 
                                ColorChunk::make(1, CRGB::Red),
                                ColorChunk::make(1, CRGB::Black) };

    ColorChunk colorChunksC[2] = { ColorChunk::make(3, CRGB::Red), ColorChunk::make(3, CRGB::Blue) };
    ColorChunk colorChunksD[2] = { ColorChunk::make(7, CRGB::Red), ColorChunk::make(7, CRGB::Blue) };

    bool configured = false; 

    //Theatre-style crawling lights.
    void theaterChase(uint32_t c, uint8_t wait) {
        // for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        //     for (int q=0; q < 3; q++) {
        //         for (uint16_t i=0; i < neoController.strip.numPixels(); i=i+3) {
        //             neoController.strip.setPixelColor(i+q, c);    //turn every third pixel on
        //         }
        //         neoController.strip.show();

        //         delay(wait);

        //         for (uint16_t i=0; i < neoController.strip.numPixels(); i=i+3) {
        //             neoController.strip.setPixelColor(i+q, 0);        //turn every third pixel off
        //         }
        //     }
        // }
    }

    // uint32_t Wheel(byte WheelPos) {
    //     WheelPos = 255 - WheelPos;
    //     if(WheelPos < 85) {
    //         return neoController.strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    //     }
    //     if(WheelPos < 170) {
    //         WheelPos -= 85;
    //         return neoController.strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    //     }
    //     WheelPos -= 170;
    //     return neoController.strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    // }

    //Theatre-style crawling lights with rainbow effect
    void theaterChaseRainbow(uint8_t wait) {
        // for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        //     for (int q=0; q < 3; q++) {
        //         for (uint16_t i=0; i < neoController.strip.numPixels(); i=i+3) {
        //             neoController.strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        //         }
        //         neoController.strip.show();

        //         delay(wait);

        //         for (uint16_t i=0; i < neoController.strip.numPixels(); i=i+3) {
        //             neoController.strip.setPixelColor(i+q, 0);        //turn every third pixel off
        //         }
        //     }
        // }
    }

    int segments[3] = { 4, 1, 2 };

    public: 
        // ColorArrayPixelFrame frame1;
        // SingleColorPixelFrame i_pir1;

        void setup(int pin, int length = 7, byte brightness = 20) {
            if (!neoController.begin(pin, length, brightness)) { return; }
            
            staticFrame1.reload(&neoController, MultiColors::make(2, colorsA));
            staticFrame2.reload(&neoController, MultiColors::make(2, colorsA, DIRECTION_REVERSE));
            rainbow1.reload(&neoController);
            station1.reload(&neoController, MultiColors::make(3, colorsA), 2, 10);
            chasingBar1.reload(&neoController, colorChunksA, 3);
            chasingFrame1.reload(&neoController);
            tralingFrame1.reload(&neoController, MultiColors::make(2, colorsA));
            neoController.fillColor(CRGB::Black);
            // frame1.setup(&strip);
            // i_pir1.setup(&strip);
            configured = true;
        }

        void run() {
            if (!configured) { return; }
            // rainbow1.run();
            station1.run();
            // chasingBar1.run();
            // chasingFrame1.run();
            // tralingFrame1.run();
            // staticFrame1.run();
            // staticFrame2.run();

            // theaterChaseRainbow(500);
            // frame1.run();
            // i_pir1.run();
        }
};
