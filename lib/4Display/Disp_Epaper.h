#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(GxEPD2_213_B74(/*CS*/ 12, /*DC*/ 0, /*RST*/ 5, /*BUSY*/ 4)); // GDEH0154D67

class Disp_Epaper {
   char HelloWorld[16] = "Hello World!";
   int number = 0;

   public:
      Disp_Epaper() {}

      void setup() {
         display.init();

         display.setRotation(1);
         display.setFont(&FreeMonoBold9pt7b);
         display.setTextColor(GxEPD_BLACK);
         int16_t tbx, tby; uint16_t tbw, tbh;
         display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
         // center the bounding box by transposition of the origin:
         // uint16_t x = ((display.width() - tbw) / 2) - tbx;
         // uint16_t y = ((display.height() - tbh) / 2) - tby;
         display.setFullWindow();
         display.firstPage();
      }

      void printLn() {
         String output = "Hello World! " + String(number);
         strcpy(HelloWorld, output.c_str());
         number++;
         
         do
         {
            display.fillScreen(GxEPD_WHITE);
            display.setCursor(0, 0);
            display.print(HelloWorld);

            display.setCursor(0, 20);
            display.print(HelloWorld);

            display.setCursor(0, 40);
            display.print(HelloWorld);
         }
         while (display.nextPage());
         display.hibernate();
      }
};