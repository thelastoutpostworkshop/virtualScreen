// Screen Layout : 4 screens on a single row
// See the tutorial : https://youtu.be/eCfeJh9Pyns
//

#include "virtualScreen.h"
#include "night_earth_960x240.h"
#include "newyork_960x240.h"


VirtualDisplay *tft;
ScreenBuilder screens;

void setup()
{
    Serial.begin(115200);

    // Adjust this setup according to your actual screen layout
    screens.addRow({{16, 0}, {15, 0}, {6, 0}, {7, 0}});


    tft = new VirtualDisplay(screens.width(), screens.height(), &screens);
    tft->begin();

    tft->fillScreen(TFT_BLACK);
    tft->drawRGBBitmap(0, 0, (uint16_t *)night_earth, night_earth_width, night_earth_height);
    tft->output();
    delay(5000);
    tft->fillScreen(TFT_BLACK);
    tft->drawRGBBitmap(0, 0, (uint16_t *)newyork, newyork_width, newyork_height);
    tft->output();
}


void loop()
{
 }
