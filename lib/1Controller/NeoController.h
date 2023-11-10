#include <Adafruit_NeoPixel.h>
// #include <FastLED.h>
#include "pixeltypes.h"

uint8_t fadeColor(uint8_t current, uint8_t target, uint8_t amount) {
    int output = current;

    if (current < target) {
        output = current + amount;
        if (output > target) { output = target; }
    } else if (current > target) {
        output = current - amount;
        if (output < target) { output = target; }
    } 

    return output;
}

CRGB fadeToward(CRGB initial, CRGB target, uint8_t amount) {
    uint8_t r = fadeColor(initial.r, target.r, amount);
    uint8_t g = fadeColor(initial.g, target.g, amount);
    uint8_t b = fadeColor(initial.b, target.b, amount);
    return CRGB(r, g, b);
}

class NeoController {
    static uint32_t encodeColor(uint8_t r, uint8_t g, uint8_t b) {
        return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
    }

    public:
        Adafruit_NeoPixel strip = Adafruit_NeoPixel();

        NeoController() {}

        bool begin(int pin, int length, byte brightness) {
            if (pin == 255) { return false; }
            strip.setPin(pin);
            strip.updateLength(length);
            strip.setBrightness(brightness);
            strip.updateType(NEO_GRB + NEO_KHZ800);
            strip.begin();
            fillColor(CRGB::Black);
            return true;
        }

        void loadPixel(uint16_t pixel, uint32_t value) {
            strip.setPixelColor(pixel, value);
        }

        void loadPixelCRGB(uint16_t pixel, CRGB crgb) {
            uint32_t color = crgb.raw[0]<<16 | crgb.raw[1] << 8 | crgb.raw[2];
            loadPixel(pixel, color);
        }

        void show() {
            strip.show();
        }

        uint16_t numPixels() {
            return strip.numPixels();
        }

        void fillColor(CRGB color) {
            for (uint16_t i=0; i<strip.numPixels(); i++) {
                loadPixelCRGB(i, color);
            }

            show(); 
        }
};


class PixelFrame {
    NeoController *controller;
    Cycle_Timer cycle;

    protected:
        virtual uint32_t valueForIndex(int index) { return 0; }

        void reloadWithDuration(int index, int duration, int frameDur) {
            std::function<void()> callbackPtr1 = [this, index]() {
                controller->loadPixel(index, valueForIndex(index));
                controller->show();
            };

            std::function<void()> callbackPtr2 = [this, index]() {
                controller->loadPixel(index, 0);
                controller->show();
            };

            cycle.loadCb(&callbackPtr1, &callbackPtr2);
            cycle.start(frameDur, duration);
        }

    public:
        PixelFrame() {}

        void setup(NeoController *_controller) {
            controller = _controller;
        }

        void reload(int index, int count, int frameDur) {
            reloadWithDuration(index, frameDur*(2*count-1), frameDur);
        }

        void run() {
            cycle.run();
        }
};

class SingleColorPixelFrame: public PixelFrame {
    uint32_t color = 0;

    uint32_t valueForIndex(int index) {
        return color;
    }

    public:
        SingleColorPixelFrame(): PixelFrame() {}

        void reload2(uint32_t _color, int index, int count = 1, int frameDur = 1000) {
            color = _color;
            this->reload(index, count, frameDur);
        }
};



/////////////////////////////////////////////////////////////

struct MultiColors {
    int colorCount;
    Direction direction;
    CRGB *colorArr;

    bool isForward() { return direction == DIRECTION_FORWARD; }

    static MultiColors make(int colorCount, CRGB *colorArr, 
            Direction direction = DIRECTION_FORWARD) {
        MultiColors output;
        output.colorCount = colorCount;
        output.direction = direction;
        output.colorArr = colorArr;
        return output;
    }
};

class SegmentPosition {
    bool repeat;
    IntCycle intCycle;

    public:
        int *segmentArr;
        int segmentCount;

        void load(int _segmentCount, int *_segmentArr, bool _repeat = true) {
            segmentCount = _segmentCount;
            segmentArr = _segmentArr;
            repeat = _repeat;
            intCycle.reload(segmentCount);
        }

        // example: [4,2,3]
        int startingValue() {
            if (intCycle.isZero()) { return 0; }
            int ref = intCycle.currentValue();
            int output = 0;
            while (ref-->=0) {
                output += segmentArr[ref];
            }

            return output;
        }

        int endingValue() {
            return startingValue() + segmentArr[intCycle.currentValue()];
        }

        void step() {
            intCycle.step();
        }
};

struct ColorChunk {
    int width;
    CRGB color;

    static ColorChunk make(int width, CRGB color) {
        ColorChunk output;
        output.width = width;
        output.color = color;
        return output;
    }
};

class NeoFrame {
    Cycle_Timer cycle;

    std::function<void()> cycleHandler = [this]() {
        onHandleCycle();
        controller->show();
    };

    protected:
        NeoController *controller;
        virtual void onHandleCycle() {}


        void load(NeoController *_controller, int runTime = 0, int frameDur = 500) {
            controller = _controller;
            cycle.loadCb(&cycleHandler);
            cycle.start(frameDur, runTime);
        }

    public:
        void run() {
            cycle.run();
        } 
};

class ColorFader {
    IntCycle colorIndex;
    MultiColors colors;
    CRGB refColor;

    CRGB targetColor() { return colors.colorArr[colorIndex.currentValue()]; }

    public:
        CRGB displayingColor() { return refColor; }

        void load(MultiColors _colors) {
            colorIndex.reload(_colors.colorCount);
            colors = _colors;
            refColor = targetColor();
        }

        void step() {
            // if (refColor == targetColor()) {
            //     colorIndex.step();
            // } else {
            //     refColor = fadeToward(refColor, targetColor(), 10);
            // }
        }
};

//! NeoStaticFrame
class NeoStaticFrame: public NeoFrame {
    IntCycle frameIndex;
    MultiColors colors;

    void onHandleCycle() {
        for (int i=0; i<colors.colorCount; i++) {
            int ref = colors.isForward() ? i : -i;
            int pixelIdx = frameIndex.nextValue(ref);
            controller->loadPixelCRGB(pixelIdx, colors.colorArr[i]);
        }

        frameIndex.step((colors.isForward()) ? 1 : -1);
    }

    public:
        void reload(NeoController *controller, MultiColors _segmentItem, int runTime = 0) {
            colors = _segmentItem;
            frameIndex.reload(controller->numPixels());
            load(controller, runTime);
        }
};


//! NeoStationFrame
class NeoStationFrame: public NeoFrame  {
    ColorFader colorFader;
    int targetPixel;

    void onHandleCycle() {
        CRGB color = colorFader.displayingColor();

        if (targetPixel == -1) {
            for(int i=0; i<controller->numPixels(); i++) {
                controller->loadPixelCRGB(i, color);
            }
        } else {
            controller->loadPixelCRGB(targetPixel, color);
        }

        colorFader.step();
    }

    public:
        void reload(NeoController *controller, MultiColors colors, int _targetPixel = -1,
                int frameTime = 200, int runTime = 0) {
            colorFader.load(colors);  // ORDER DOES MATTER
            targetPixel = _targetPixel;
            load(controller, runTime, frameTime);
        }
};


//! NeoChasingFrame
class NeoChasingFrame: public NeoFrame {
    SequenceFader fader;
    int pixelChunks[4] = { 1, 1, 2, 2 };
    int pixelChunkCount = 4;
    CRGB refColor = CRGB::Red;

    CRGB colorForOffsetIndex(int index) {
        int tracker = 0;
        int targetIndex = 0;

        for (int i=0; i<pixelChunkCount; i++) {
            tracker += pixelChunks[i];
            if (tracker>index) { targetIndex = i; break; }
        }   

        return (targetIndex%2==0) ? refColor : CRGB::Black;
    }
    
    void onHandleCycle() {
        for(int i=0; i<controller->numPixels(); i++) {
            CRGB color = colorForOffsetIndex(fader.valueForIndex(i));
            controller->loadPixelCRGB(i, color);
        }

        refColor = fadeToward(refColor, CRGB::Blue, 10);
        fader.step();
    }

    public:
        void reload(NeoController *controller, int runTime = 0) {
            int sequenceLength = 0;
            for (byte i=0; i<pixelChunkCount; i++) {
                sequenceLength += pixelChunks[i];
            }
            
            fader.reload(sequenceLength);
            load(controller, runTime, 200);
        }
};

//! NeoChasingBar
class NeoChasingBar: public NeoFrame {
    SequenceFader fader;
    ColorChunk *colorChunks;
    int chunkCount;
    
    CRGB colorForOffsetIndex(int index) {
        int tracker = 0;

        for (int i=0; i<chunkCount; i++) {
            tracker += colorChunks[i].width;
            if (tracker>index) { return colorChunks[i].color; }
        }   

        return CRGB::Black;
    }

    void onHandleCycle() {
        for(int i=0; i<controller->numPixels(); i++) {
            CRGB color = colorForOffsetIndex(fader.valueForIndex(i));
            controller->loadPixelCRGB(i, color);
        }

        fader.step();
    }

    public:
        void reload(NeoController *controller, ColorChunk *chunks, int _chunkCount, int runTime = 0) {
            colorChunks = chunks;
            chunkCount = _chunkCount;
            
            int sequenceLength = 0;
            for (byte i=0; i<_chunkCount; i++) {
                sequenceLength += colorChunks[i].width;
            }
            fader.reload(sequenceLength);
            load(controller, runTime, 400);
        }
};


//! NeoTrailingFrame
class NeoTrailingFrame: public NeoFrame {
    SequenceFader fader;
    MultiColors colors;

    CRGB colorForOffsetIndex(int index) {
        int value = index*80;
        CRGB color = fadeToward(CRGB::Blue, CRGB::Black, value>255 ? 255 : value);
        return color;
    }

    void onHandleCycle() {
        for(int i=0; i<controller->numPixels(); i++) {
            int value = fader.valueForRevertIndex(i);
            CRGB color = colorForOffsetIndex(value);

            // Serial.print("*pixel="); Serial.print(i);
            // Serial.print(" | "); Serial.print(fader.currentValue());
            // Serial.print(" | "); Serial.print(value);
            // Serial.print(" | "); Serial.println(color.b);

            controller->loadPixelCRGB(i, color);
        }

        // Serial.println("--------------------");
        fader.step();
    }


    public:
        void reload(NeoController *controller, MultiColors _colors, int runTime = 0) {
            fader.reload(5);
            colors = _colors;
            load(controller, runTime, 200);
        }
};


//! NeoRainbowFrame
class NeoRainbowFrame: public NeoFrame {
    uint8_t hue;

    void onHandleCycle() {
        for(int i=0; i<controller->numPixels(); i++) {
            CRGB rgbValue = CRGB(0x000000);
            // rgbValue.setHue(hue+i*256/(controller->numPixels()*2));
            controller->loadPixelCRGB(i, rgbValue);
        }
        
        hue++;
    }

    public:
        void reload(NeoController *controller, int runTime = 0) {
            load(controller, runTime, 10);
        }
};