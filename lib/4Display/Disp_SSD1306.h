#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

static const unsigned char PROGMEM logo_bmp[] =
{ 
    B00000000, B11000000,
    B00000001, B11000000,
    B00000001, B11000000,
    B00000011, B11100000,
    B11110011, B11100000,
    B11111110, B11111000,
    B01111110, B11111111,
    B00110011, B10011111,
    B00011111, B11111100,
    B00001101, B01110000,
    B00011011, B10100000,
    B00111111, B11100000,
    B00111111, B11110000,
    B01111100, B11110000,
    B01110000, B01110000,
    B00000000, B00110000 
};

class Disp_SSD13062 {
    Adafruit_SSD1306 disp;
    TimeoutItem timer;       //* print time logger
    bool isLoaded = false;

    public:
        Disp_SSD13062() {}

        //! Addr 0x3C : 0x3D
        void setup(TwoWire *wire) {
            // Display required some time on power up to work
            // Wire.beginTransmission(addr);
            // bool error = Wire.endTransmission();
            disp = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, wire, OLED_RESET);
            bool check = disp.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);

            Serial.print("[SSD13] Init: "); Serial.println(check ? "success" : "failed");
            if (!check) return; 
            
            disp.clearDisplay();
            disp.setTextSize(1);
            disp.setTextColor(WHITE, BLACK);        // Draw white text
            isLoaded = true;
        }

        void printline(const String str, uint16_t line, uint16_t offset = 0) {
            // takes about 38ms to print each line
            if (!isLoaded) { return; }
            uint16_t height = 9;    // 17 for size 2
            uint16_t pos = line*height + offset;
            // timer.reset();

            disp.fillRect(0, pos, disp.width(), height, BLACK);
            // AppPrint("1 fillRectTime", timer.elapsed());
            // timer.reset();

            disp.setCursor(0, pos);
            disp.print(str);
            // AppPrint("2 printTime", timer.elapsed());
            // timer.reset();

            //? Need to confirm the process time is mainly by this command
            disp.display();
            // AppPrint("3 diplayTime", timer.elapsed());
        }

        void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color = WHITE) {
            disp.drawLine(x0, y0, x1, y1, color);
        }

        void clear() {
            if (!isLoaded) { return; }
            disp.clearDisplay();
        }

        void testAll() {
            testdrawline();
            testdrawrect();
            testdrawrect(true);
            testdrawcircle();
            testdrawcircle(true);
            testdrawroundrect();
            testdrawroundrect(true);
            testdrawtriangle();
            testdrawtriangle(true);
            testdrawchar();
            testdrawstyles();
            testscrolltext();
            testdrawbitmap();
        }

        void testdrawline() {
            int16_t i;

            disp.clearDisplay(); // Clear display buffer

            for(i=0; i<disp.width(); i+=4) {
                disp.drawLine(0, 0, i, disp.height()-1, SSD1306_WHITE);
                disp.display(); // Update screen with each newly-drawn line
                delay(1);
            }
            for(i=0; i<disp.height(); i+=4) {
                disp.drawLine(0, 0, disp.width()-1, i, SSD1306_WHITE);
                disp.display();
                delay(1);
            }
            delay(250);

            disp.clearDisplay();

            for(i=0; i<disp.width(); i+=4) {
                disp.drawLine(0, disp.height()-1, i, 0, SSD1306_WHITE);
                disp.display();
                delay(1);
            }
            for(i=disp.height()-1; i>=0; i-=4) {
                disp.drawLine(0, disp.height()-1, disp.width()-1, i, SSD1306_WHITE);
                disp.display();
                delay(1);
            }
            delay(250);

            disp.clearDisplay();

            for(i=disp.width()-1; i>=0; i-=4) {
                disp.drawLine(disp.width()-1, disp.height()-1, i, 0, SSD1306_WHITE);
                disp.display();
                delay(1);
            }
            for(i=disp.height()-1; i>=0; i-=4) {
                disp.drawLine(disp.width()-1, disp.height()-1, 0, i, SSD1306_WHITE);
                disp.display();
                delay(1);
            }
            delay(250);

            disp.clearDisplay();

            for(i=0; i<disp.height(); i+=4) {
                disp.drawLine(disp.width()-1, 0, 0, i, SSD1306_WHITE);
                disp.display();
                delay(1);
            }
            for(i=0; i<disp.width(); i+=4) {
                disp.drawLine(disp.width()-1, 0, i, disp.height()-1, SSD1306_WHITE);
                disp.display();
                delay(1);
            }

            delay(2000); // Pause for 2 seconds
        }

        void testdrawrect(bool fill = false) {
            disp.clearDisplay();

            for(int16_t i=0; i<disp.height()/2; i+=2) {
                if (fill) {
                    disp.fillRect(i, i, disp.width()-i*2, disp.height()-i*2, SSD1306_INVERSE);
                } else {
                    disp.drawRect(i, i, disp.width()-2*i, disp.height()-2*i, SSD1306_WHITE);
                }
                
                disp.display(); // Update screen with each newly-drawn rectangle
                delay(1);
            }

            delay(2000);
        }

        void testdrawcircle(bool fill = false) {
            disp.clearDisplay();

            for(int16_t i=0; i<max(disp.width(),disp.height())/2; i+=2) {
                if (fill) {
                    disp.fillCircle(disp.width() / 2, disp.height() / 2, i, SSD1306_INVERSE);
                } else {
                    disp.drawCircle(disp.width()/2, disp.height()/2, i, SSD1306_WHITE);
                }
                
                disp.display();
                delay(1);
            }

            delay(2000);
        }

        void testdrawroundrect(bool fill = false) {
            disp.clearDisplay();

            for(int16_t i=0; i<disp.height()/2-2; i+=2) {
                if (fill) {
                    disp.fillRoundRect(i, i, disp.width()-2*i, disp.height()-2*i,
                    disp.height()/4, SSD1306_INVERSE);
                } else {
                    disp.drawRoundRect(i, i, disp.width()-2*i, disp.height()-2*i,
                        disp.height()/4, SSD1306_WHITE);
                }

                disp.display();
                delay(1);
            }

            delay(2000);
        }

        void testdrawtriangle(bool fill = false) {
            disp.clearDisplay();

            if (fill) {
                for(int16_t i=max(disp.width(),disp.height())/2; i>0; i-=5) {
                    // The INVERSE color is used so triangles alternate white/black
                    disp.fillTriangle(
                    disp.width()/2  , disp.height()/2-i,
                    disp.width()/2-i, disp.height()/2+i,
                    disp.width()/2+i, disp.height()/2+i, SSD1306_INVERSE);
                    disp.display();
                    delay(1);
                }
            } else {
                for(int16_t i=0; i<max(disp.width(),disp.height())/2; i+=5) {
                    disp.drawTriangle(
                    disp.width()/2  , disp.height()/2-i,
                    disp.width()/2-i, disp.height()/2+i,
                    disp.width()/2+i, disp.height()/2+i, SSD1306_WHITE);
                    disp.display();
                    delay(1);
                }
            }


            delay(2000);
        }

        void testdrawchar(void) {
            disp.clearDisplay();

            disp.setTextSize(1);      // Normal 1:1 pixel scale
            disp.setTextColor(SSD1306_WHITE); // Draw white text
            disp.setCursor(0, 0);     // Start at top-left corner
            disp.cp437(true);         // Use full 256 char 'Code Page 437' font

            // Not all the characters will fit on the display. This is normal.
            // Library will draw what it can and the rest will be clipped.
            for(int16_t i=0; i<256; i++) {
                if(i == '\n') disp.write(' ');
                else          disp.write(i);
            }

            disp.display();
            delay(2000);
        }

        void testdrawstyles(void) {
            disp.clearDisplay();
            disp.setTextSize(1);             // Normal 1:1 pixel scale
            disp.setTextColor(SSD1306_WHITE);        // Draw white text
            disp.setCursor(0,0);             // Start at top-left corner
            disp.println(F("Hello, world!"));

            disp.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
            disp.println(3.141592);

            disp.setTextSize(2);             // Draw 2X-scale text
            disp.setTextColor(SSD1306_WHITE);
            disp.print(F("0x")); disp.println(0xDEADBEEF, HEX);

            disp.display();
            delay(2000);
        }

        void testscrolltext(void) {
            disp.clearDisplay();

            disp.setTextSize(2); // Draw 2X-scale text
            disp.setTextColor(SSD1306_WHITE);
            disp.setCursor(10, 0);
            disp.println(F("scroll"));
            disp.display();      // Show initial text
            delay(100);

            // Scroll in various directions, pausing in-between:
            disp.startscrollright(0x00, 0x0F);
            delay(2000);
            disp.stopscroll();
            delay(1000);
            disp.startscrollleft(0x00, 0x0F);
            delay(2000);
            disp.stopscroll();
            delay(1000);
            disp.startscrolldiagright(0x00, 0x07);
            delay(2000);
            disp.startscrolldiagleft(0x00, 0x07);
            delay(2000);
            disp.stopscroll();
            delay(1000);
        }

        void testdrawbitmap(void) {
            disp.clearDisplay();

            disp.drawBitmap(
                (disp.width()  - LOGO_WIDTH ) / 2,
                (disp.height() - LOGO_HEIGHT) / 2,
                logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
            disp.display();
            delay(1000);
        }

        #define XPOS   0 // Indexes into the 'icons' array in function below
        #define YPOS   1
        #define DELTAY 2

        // testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
        void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
            int8_t f, icons[NUMFLAKES][3];

            // Initialize 'snowflake' positions
            for(f=0; f< NUMFLAKES; f++) {
                icons[f][XPOS]   = random(1 - LOGO_WIDTH, disp.width());
                icons[f][YPOS]   = -LOGO_HEIGHT;
                icons[f][DELTAY] = random(1, 6);
                Serial.print(F("x: "));
                Serial.print(icons[f][XPOS], DEC);
                Serial.print(F(" y: "));
                Serial.print(icons[f][YPOS], DEC);
                Serial.print(F(" dy: "));
                Serial.println(icons[f][DELTAY], DEC);
            }

            for(;;) { // Loop forever...
                disp.clearDisplay(); // Clear the display buffer

                // Draw each snowflake:
                for(f=0; f< NUMFLAKES; f++) {
                disp.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
                }

                disp.display(); // Show the display buffer on the screen
                delay(200);        // Pause for 1/10 second

                // Then update coordinates of each flake...
                for(f=0; f< NUMFLAKES; f++) {
                icons[f][YPOS] += icons[f][DELTAY];
                // If snowflake is off the bottom of the screen...
                if (icons[f][YPOS] >= disp.height()) {
                    // Reinitialize to a random position, just off the top
                    icons[f][XPOS]   = random(1 - LOGO_WIDTH, disp.width());
                    icons[f][YPOS]   = -LOGO_HEIGHT;
                    icons[f][DELTAY] = random(1, 6);
                }
                }
            }
        }
};